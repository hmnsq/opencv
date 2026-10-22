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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
#ਅਨ੍ਤਰ੍ਭਾਵ "ocl_disabled.impl.hpp"
#ਨਹੀਂਤਾਂ // HAVE_OPENCL

#ਅਨ੍ਤਰ੍ਭਾਵ <list>
#ਅਨ੍ਤਰ੍ਭਾਵ <map>
#ਅਨ੍ਤਰ੍ਭਾਵ <deque>
#ਅਨ੍ਤਰ੍ਭਾਵ <set>
#ਅਨ੍ਤਰ੍ਭਾਵ <string>
#ਅਨ੍ਤਰ੍ਭਾਵ <sstream>
#ਅਨ੍ਤਰ੍ਭਾਵ <fstream>
#ਜੇ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_LOG_STRIP_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/ocl_genbase.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/lock.private.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/filesystem.private.hpp"

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ਨਿਰਾ, 0, "OpenCL allocator: " << __VA_ARGS__)
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/allocator_stats.impl.hpp"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV__ALLOCATOR_STATS_LOG

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SHOW_BUILD_OPTIONS             0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SHOW_BUILD_KERNELS             0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SHOW_ਲੜੀਵਾਰ_KERNELS               0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SYNC_ਲੜੀਵਾਰ_KERNELS               0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_TRACE_CHECK                    0

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS       1

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SHOW_SVM_LOG                   0

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/bufferpool.hpp"
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LOG_ਬਫਰ_POOL
# ਜੇ 0
#   define LOG_ਬਫਰ_POOL ਲਿਖੋ
# ਨਹੀਂਤਾਂ
#   define LOG_ਬਫਰ_POOL(...)
# endif
#ਜੇ_ਅੰਤ

#ਜੇ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SVM_TRACE_P ਲਿਖੋ("line %d (ocl.cpp): ", __LINE__); ਲਿਖੋ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SVM_TRACE_P(...)
#ਜੇ_ਅੰਤ

#ਜੇ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SVM_TRACE_ERROR_P ਲਿਖੋ("Error on line %d (ocl.cpp): ", __LINE__); ਲਿਖੋ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_SVM_TRACE_ERROR_P(...)
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/opencl/opencl_svm.hpp"
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "umatrix.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ocl {

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ IMPLEMENT_REFCOUNTABLE() \
    ਖਾਲੀ addref() { CV_XADD(&refcount, 1); } \
    ਖਾਲੀ release() { ਜੇ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) ਮਿਟਾਓ ਇਹ; } \
    ਪੂਰਨ refcount

ਸਥਿਰ cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ਵਾਪਸ opencl_allocator_stats;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _DEBUG
ਸਥਿਰ ਤਾਰਕਿਕ isRaiseError()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ value = ਝੂਠ;
    ਜੇ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ value;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਖਾਲੀ onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    ਤਾਰਕਿਕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", ਝੂਠ);
    ਜੇ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#ਜੇ CV_OPENCL_TRACE_CHECK
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ traceOpenCLCheck(cl_int ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ* message)
{
    std::ਬਾਹਰ << "OpenCV(OpenCL:" << ਸਥਿਤੀ << "): " << message << std::ਸਤਰ_ਅੰਤ << std::flush;
}
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, message) traceOpenCLCheck(ਸਥਿਤੀ, message)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, message) /* nothing */
#ਜੇ_ਅੰਤ

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_API_ERROR_ਸੁਨੇਹਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_ਨਤੀਜਾ), check_ਨਤੀਜਾ, ਸੁਨੇਹਾ)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ) \
    ਕਰੋ { \
        CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ); \
        ਜੇ (check_ਨਤੀਜਾ != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(ਸੁਨੇਹਾ), ਅਚਲ ਅੱਖਰ*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_ਸੁਨੇਹਾ_2 = CV_OCL_API_ERROR_ਸੁਨੇਹਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ); \
            CV_Error(Error::OpenCLApiCallError, error_ਸੁਨੇਹਾ_2); \
        } \
    } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_CHECK_(ਵਿਅੰਜਕ, check_ਨਤੀਜਾ) ਕਰੋ { ਵਿਅੰਜਕ; CV_OCL_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, #expr); } ਜਦੋਂ (0)

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_CHECK(ਵਿਅੰਜਕ) ਕਰੋ { cl_int __cl_result = (ਵਿਅੰਜਕ); CV_OCL_CHECK_ਨਤੀਜਾ(__cl_result, #expr); } ਜਦੋਂ (0)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _DEBUG
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ) CV_OCL_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK(ਵਿਅੰਜਕ) CV_OCL_CHECK(ਵਿਅੰਜਕ)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK_(ਵਿਅੰਜਕ, check_ਨਤੀਜਾ) CV_OCL_CHECK_(ਵਿਅੰਜਕ, check_ਨਤੀਜਾ)
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ) \
    ਕਰੋ { \
        CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ); \
        ਜੇ (check_ਨਤੀਜਾ != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(ਸੁਨੇਹਾ), ਅਚਲ ਅੱਖਰ*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_ਸੁਨੇਹਾ_2 = CV_OCL_API_ERROR_ਸੁਨੇਹਾ(check_ਨਤੀਜਾ, ਸੁਨੇਹਾ); \
            CV_Error(Error::OpenCLApiCallError, error_ਸੁਨੇਹਾ_2); \
        } \
    } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK_(ਵਿਅੰਜਕ, check_ਨਤੀਜਾ) ਕਰੋ { ਵਿਅੰਜਕ; CV_OCL_DBG_CHECK_ਨਤੀਜਾ(check_ਨਤੀਜਾ, #expr); } ਜਦੋਂ (0)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_DBG_CHECK(ਵਿਅੰਜਕ) ਕਰੋ { cl_int __cl_result = (ਵਿਅੰਜਕ); CV_OCL_DBG_CHECK_ਨਤੀਜਾ(__cl_result, #expr); } ਜਦੋਂ (0)
#ਜੇ_ਅੰਤ


ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", ਸੱਚ);
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", ਸੱਚ);
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_CACHE_ਤਾਲਾ_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", ਸੱਚ);
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", ਸੱਚ);

#ਜੇ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS_ਮੁੱਲ = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", ਝੂਠ);
#ਜੇ_ਅੰਤ

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_DISABLE_ਬਫਰ_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#ਜੇ 1 //def __APPLE__
        ਸੱਚ
#ਨਹੀਂਤਾਂ
        ਝੂਠ
#ਜੇ_ਅੰਤ
);

ਸਥਿਰ String getBuildExtraOptions()
{
    ਸਥਿਰ String param_buildExtraOptions;
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਜੇ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = ਸੱਚ;
        ਜੇ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ਨਿਰਾ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ਵਾਪਸ param_buildExtraOptions;
}

ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", ਸੱਚ);
ਸਥਿਰ ਅਚਲ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ਬਣਤਰ UMat2D
{
    UMat2D(ਅਚਲ UMat& m)
    {
        offset = (ਪੂਰਨ)m.offset;
        ਕਦਮ = (ਪੂਰਨ)m.ਕਦਮ;
        ਪੰਕਤੀਆਂ = m.ਪੰਕਤੀਆਂ;
        ਖਾਨੇ_ਗਿਣਤੀ = m.ਖਾਨੇ_ਗਿਣਤੀ;
    }
    ਪੂਰਨ offset;
    ਪੂਰਨ ਕਦਮ;
    ਪੂਰਨ ਪੰਕਤੀਆਂ;
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ;
};

ਬਣਤਰ UMat3D
{
    UMat3D(ਅਚਲ UMat& m)
    {
        offset = (ਪੂਰਨ)m.offset;
        ਕਦਮ = (ਪੂਰਨ)m.ਕਦਮ.p[1];
        slicestep = (ਪੂਰਨ)m.ਕਦਮ.p[0];
        slices = (ਪੂਰਨ)m.size.p[0];
        ਪੰਕਤੀਆਂ = m.size.p[1];
        ਖਾਨੇ_ਗਿਣਤੀ = m.size.p[2];
    }
    ਪੂਰਨ offset;
    ਪੂਰਨ slicestep;
    ਪੂਰਨ ਕਦਮ;
    ਪੂਰਨ slices;
    ਪੂਰਨ ਪੰਕਤੀਆਂ;
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ਸਥਿਰ uint64 crc64( ਅਚਲ uchar* ਅੰਕੜੇ, size_t size, uint64 crc0=0 )
{
    ਸਥਿਰ uint64 table[256];
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;

    ਜੇ( !initialized )
    {
        ਲਈ( ਪੂਰਨ i = 0; i < 256; i++ )
        {
            uint64 c = i;
            ਲਈ( ਪੂਰਨ j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = ਸੱਚ;
    }

    uint64 ਚੱਕਰੀ_ਜਾਂਚ = ~crc0;
    ਲਈ( size_t idx = 0; idx < size; idx++ )
        ਚੱਕਰੀ_ਜਾਂਚ = table[(uchar)ਚੱਕਰੀ_ਜਾਂਚ ^ ਅੰਕੜੇ[idx]] ^ (ਚੱਕਰੀ_ਜਾਂਚ >> 8);

    ਵਾਪਸ ~ਚੱਕਰੀ_ਜਾਂਚ;
}

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
ਬਣਤਰ OpenCLBinaryCacheConfigurator
{
    cv::String cache_ਰਸਤਾ;
    cv::String cache_ਤਾਲਾ_filename;
    cv::Ptr<utils::fs::File_ਤਾਲਾ> cache_ਤਾਲਾ;

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ਨਿਰਾ, "Initializing OpenCL cache configuration...");
        ਜੇ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL cache is disabled");
            ਵਾਪਸ;
        }
        cache_ਰਸਤਾ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        ਜੇ (cache_ਰਸਤਾ.empty())
        {
            CV_LOG_INFO(ਨਿਰਾ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        ਕਰੋ
        {
            ਕੋਸ਼ਿਸ਼
            {
                ਜੇ (cache_ਰਸਤਾ.empty())
                    ਰੋਕੋ;
                ਜੇ (cache_ਰਸਤਾ == "disabled")
                    ਰੋਕੋ;
                ਜੇ (!utils::fs::createDirectories(cache_ਰਸਤਾ))
                {
                    CV_LOG_DEBUG(ਨਿਰਾ, "Can't use OpenCL cache directory: " << cache_ਰਸਤਾ);
                    clear();
                    ਰੋਕੋ;
                }

                ਜੇ (CV_OPENCL_CACHE_ਤਾਲਾ_ENABLE)
                {
                    cache_ਤਾਲਾ_filename = cache_ਰਸਤਾ + ".lock";
                    ਜੇ (!utils::fs::exists(cache_ਤਾਲਾ_filename))
                    {
                        CV_LOG_DEBUG(ਨਿਰਾ, "Creating lock file... (" << cache_ਤਾਲਾ_filename << ")");
                        std::ofstream ਤਾਲਾ_filename(cache_ਤਾਲਾ_filename.c_str(), std::ios::out);
                        ਜੇ (!ਤਾਲਾ_filename.is_open())
                        {
                            CV_LOG_WARNING(ਨਿਰਾ, "Can't create lock file for OpenCL program cache: " << cache_ਤਾਲਾ_filename);
                            ਰੋਕੋ;
                        }
                    }

                    ਕੋਸ਼ਿਸ਼
                    {
                        cache_ਤਾਲਾ = makePtr<utils::fs::File_ਤਾਲਾ>(cache_ਤਾਲਾ_filename.c_str());
                        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Checking cache lock... (" << cache_ਤਾਲਾ_filename << ")");
                        {
                            utils::shared_ਤਾਲਾ_guard<utils::fs::File_ਤਾਲਾ> lock(*cache_ਤਾਲਾ);
                        }
                        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Checking cache lock... Done!");
                    }
                    ਫੜੋ (ਅਚਲ cv::Exception& e)
                    {
                        CV_LOG_WARNING(ਨਿਰਾ, "Can't create OpenCL program cache lock: " << cache_ਤਾਲਾ_filename << std::ਸਤਰ_ਅੰਤ << e.what());
                    }
                    ਫੜੋ (...)
                    {
                        CV_LOG_WARNING(ਨਿਰਾ, "Can't create OpenCL program cache lock: " << cache_ਤਾਲਾ_filename);
                    }
                }
                ਨਹੀਂਤਾਂ
                {
                    ਜੇ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ਨਿਰਾ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        CV_LOG_INFO(ਨਿਰਾ, "OpenCL cache lock is disabled");
                    }
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "Can't prepare OpenCL program cache: " << cache_ਰਸਤਾ << std::ਸਤਰ_ਅੰਤ << e.what());
                clear();
            }
        } ਜਦੋਂ (0);
        ਜੇ (!cache_ਰਸਤਾ.empty())
        {
            ਜੇ (cache_ਤਾਲਾ.empty() && CV_OPENCL_CACHE_ਤਾਲਾ_ENABLE)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_INFO(ਨਿਰਾ, "Successfully initialized OpenCL cache directory: " << cache_ਰਸਤਾ);
            }
        }
    }

    ਖਾਲੀ clear()
    {
        cache_ਰਸਤਾ.clear();
        cache_ਤਾਲਾ_filename.clear();
        cache_ਤਾਲਾ.release();
    }

    std::string prepareCacheDirectoryForContext(ਅਚਲ std::string& ctx_ਅਗੇਤਰ,
            ਅਚਲ std::string& cleanup_ਅਗੇਤਰ)
    {
        ਜੇ (cache_ਰਸਤਾ.empty())
            ਵਾਪਸ std::string();

        Auto_ਤਾਲਾ lock(mutex_prepared_contexts_);

        ContextCacheType::iterator ਮਿਲਿਆ_it = prepared_contexts_.find(ctx_ਅਗੇਤਰ);
        ਜੇ (ਮਿਲਿਆ_it != prepared_contexts_.end())
            ਵਾਪਸ ਮਿਲਿਆ_it->second;

        CV_LOG_INFO(ਨਿਰਾ, "Preparing OpenCL cache configuration for context: " << ctx_ਅਗੇਤਰ);

        std::string ਨਿਸ਼ਾਨਾ_directory = cache_ਰਸਤਾ + ctx_ਅਗੇਤਰ + "/";
        ਤਾਰਕਿਕ ਨਤੀਜਾ = utils::fs::isDirectory(ਨਿਸ਼ਾਨਾ_directory);
        ਜੇ (!ਨਤੀਜਾ)
        {
            ਕੋਸ਼ਿਸ਼
            {
                CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Creating directory: " << ਨਿਸ਼ਾਨਾ_directory);
                ਜੇ (utils::fs::createDirectories(ਨਿਸ਼ਾਨਾ_directory))
                {
                    ਨਤੀਜਾ = ਸੱਚ;
                }
                ਨਹੀਂਤਾਂ
                {
                    CV_LOG_WARNING(ਨਿਰਾ, "Can't create directory: " << ਨਿਸ਼ਾਨਾ_directory);
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't create OpenCL program cache directory for context: " << ਨਿਸ਼ਾਨਾ_directory << std::ਸਤਰ_ਅੰਤ << e.what());
            }
        }
        ਨਿਸ਼ਾਨਾ_directory = ਨਤੀਜਾ ? ਨਿਸ਼ਾਨਾ_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_ਅਗੇਤਰ, ਨਿਸ਼ਾਨਾ_directory));

        ਜੇ (ਨਤੀਜਾ && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_ਅਗੇਤਰ.empty())
        {
            ਕੋਸ਼ਿਸ਼
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_ਰਸਤਾ, cleanup_ਅਗੇਤਰ + "*", entries, ਝੂਠ, ਸੱਚ);
                std::vector<String> remove_entries;
                ਲਈ (size_t i = 0; i < entries.size(); i++)
                {
                    ਅਚਲ String& name = entries[i];
                    ਜੇ (0 == name.find(cleanup_ਅਗੇਤਰ))
                    {
                        ਜੇ (0 == name.find(ctx_ਅਗੇਤਰ))
                            ਜਾਰੀ; // skip current
                        remove_entries.push_back(name);
                    }
                }
                ਜੇ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ਨਿਰਾ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ਨਿਰਾ, "Trying to remove these directories...");
                    ਲਈ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ਨਿਰਾ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ਨਿਰਾ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    ਲਈ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ਅਚਲ String& name = remove_entries[i];
                        cv::String ਰਸਤਾ = utils::fs::join(cache_ਰਸਤਾ, name);
                        ਕੋਸ਼ਿਸ਼
                        {
                            utils::fs::remove_all(ਰਸਤਾ);
                            CV_LOG_WARNING(ਨਿਰਾ, "Removed: " << ਰਸਤਾ);
                        }
                        ਫੜੋ (ਅਚਲ cv::Exception& e)
                        {
                            CV_LOG_ERROR(ਨਿਰਾ, "Exception during removal of obsolete OpenCL cache directory: " << ਰਸਤਾ << std::ਸਤਰ_ਅੰਤ << e.what());
                        }
                    }
                }
            }
            ਫੜੋ (...)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ਨਿਰਾ, 1, "  Result: " << (ਨਿਸ਼ਾਨਾ_directory.empty() ? std::string("Failed") : ਨਿਸ਼ਾਨਾ_directory));
        ਵਾਪਸ ਨਿਸ਼ਾਨਾ_directory;
    }

    ਸਥਿਰ OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, ਨਵਾਂ OpenCLBinaryCacheConfigurator());
    }
};
ਵਰਗ ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ_File
{
    ਗਿਣਤੀ { MAX_ENTRIES = 64 };

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ uint32_t;

    ਬਣਤਰ CV_DECL_ALIGNED(4) File_ਸਿਰਲੇਖ
    {
        uint32_t ਸਰੋਤ_Signature_ਮਾਪ;
        //char sourceSignature[];
    };

    ਬਣਤਰ CV_DECL_ALIGNED(4) File_ਸਾਰਣੀ
    {
        uint32_t ਸੰਖਿਆ_ਦਾ_Entries;
        //uint32_t firstEntryOffset[];
    };

    ਬਣਤਰ CV_DECL_ALIGNED(4) File_ਇੰਦਰਾਜ
    {
        uint32_t ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ; // 0 for the last entry in chain
        uint32_t ਕੁੰਜੀ_ਮਾਪ;
        uint32_t ਅੰਕੜੇ_ਮਾਪ;
        //char key[];
        //char data[];
    };

    ਅਚਲ std::string fileName_;
    ਅਚਲ ਅੱਖਰ* ਅਚਲ ਸਰੋਤ_Signature;
    ਅਚਲ size_t ਸਰੋਤ_Signature_ਮਾਪ_2;

    std::fstream f;

    uint32_t ਇੰਦਰਾਜ_Offsets[MAX_ENTRIES];

    uint32_t getHash(ਅਚਲ std::string& options)
    {
        uint64 hash = crc64((ਅਚਲ uchar*)options.c_str(), options.size(), 0);
        ਵਾਪਸ hash & (MAX_ENTRIES - 1);
    }

    ਸਤਰਅੰਦਰ size_t get_File_ਮਾਪ()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_ਮਾਪ = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ਵਾਪਸ file_ਮਾਪ;
    }
    ਸਤਰਅੰਦਰ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.ਪਠ((ਅੱਖਰ*)&res, ਆਕਾਰ(uint32_t));
        CV_Assert(!f.fail());
        ਵਾਪਸ res;
    }
    ਸਤਰਅੰਦਰ ਖਾਲੀ writeUInt32(ਅਚਲ uint32_t value)
    {
        uint32_t v = value;
        f.ਲਿਖ((ਅੱਖਰ*)&v, ਆਕਾਰ(uint32_t));
        CV_Assert(!f.fail());
    }

    ਸਤਰਅੰਦਰ ਖਾਲੀ seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    ਸਤਰਅੰਦਰ ਖਾਲੀ seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    ਸਤਰਅੰਦਰ ਖਾਲੀ seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    ਖਾਲੀ clearFile()
    {
        f.close();
        ਜੇ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ਨਿਰਾ, "Can't remove: " << fileName_);
        ਵਾਪਸ;
    }

ਜਨਤਕ:
    ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ_File(ਅਚਲ std::string& fileName, ਅਚਲ ਅੱਖਰ* ਸਰੋਤ_Signature_2)
        : fileName_(fileName), ਸਰੋਤ_Signature(ਸਰੋਤ_Signature_2), ਸਰੋਤ_Signature_ਮਾਪ_2(ਸਰੋਤ_Signature ? ਲੜੀ_ਲੰਬਾਈ(ਸਰੋਤ_Signature) : 0)
    {
        CV_StaticAssert(ਆਕਾਰ(uint32_t) == 4, "");
        CV_Assert(ਸਰੋਤ_Signature != ਨਿਰਾ);
        CV_Assert(ਸਰੋਤ_Signature_ਮਾਪ_2 > 0);
        memset(ਇੰਦਰਾਜ_Offsets, 0, ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        ਜੇ(f.is_open() && get_File_ਮਾਪ() > 0)
        {
            ਤਾਰਕਿਕ isValid = ਝੂਠ;
            ਕੋਸ਼ਿਸ਼
            {
                uint32_t file_ਸਰੋਤ_Signature_ਮਾਪ = readUInt32();
                ਜੇ (file_ਸਰੋਤ_Signature_ਮਾਪ == ਸਰੋਤ_Signature_ਮਾਪ_2)
                {
                    cv::Auto_ਬਫਰ<ਅੱਖਰ> file_ਸਰੋਤ_Signature(file_ਸਰੋਤ_Signature_ਮਾਪ + 1);
                    f.ਪਠ(file_ਸਰੋਤ_Signature.ਅੰਕੜੇ(), file_ਸਰੋਤ_Signature_ਮਾਪ);
                    ਜੇ (f.eof())
                    {
                        CV_LOG_ERROR(ਨਿਰਾ, "Unexpected EOF");
                    }
                    ਨਹੀਂਤਾਂ ਜੇ (memcmp(ਸਰੋਤ_Signature_2, file_ਸਰੋਤ_Signature.ਅੰਕੜੇ(), file_ਸਰੋਤ_Signature_ਮਾਪ) == 0)
                    {
                        isValid = ਸੱਚ;
                    }
                }
                ਜੇ (!isValid)
                {
                    CV_LOG_ERROR(ਨਿਰਾ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            ਫੜੋ (...)
            {
                CV_LOG_ERROR(ਨਿਰਾ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            ਜੇ (!isValid)
            {
                clearFile();
            }
            ਨਹੀਂਤਾਂ
            {
                seekReadAbsolute(0);
            }
        }
    }

    ਤਾਰਕਿਕ ਪਠ(ਅਚਲ std::string& ਕੁੰਜੀ, std::vector<ਅੱਖਰ>& buf)
    {
        ਜੇ (!f.is_open())
            ਵਾਪਸ ਝੂਠ;

        size_t file_ਮਾਪ = get_File_ਮਾਪ();
        ਜੇ (file_ਮਾਪ == 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "Invalid file (empty): " << fileName_);
            clearFile();
            ਵਾਪਸ ਝੂਠ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ਸਰੋਤ_Signature_ਮਾਪ = readUInt32();
        CV_Assert(file_ਸਰੋਤ_Signature_ਮਾਪ > 0);
        seekReadRelative(file_ਸਰੋਤ_Signature_ਮਾਪ);

        uint32_t ਸੰਖਿਆ_ਦਾ_Entries = readUInt32();
        CV_Assert(ਸੰਖਿਆ_ਦਾ_Entries > 0);
        ਜੇ (ਸੰਖਿਆ_ਦਾ_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "Invalid file: " << fileName_);
            clearFile();
            ਵਾਪਸ ਝੂਠ;
        }
        f.ਪਠ((ਅੱਖਰ*)&ਇੰਦਰਾਜ_Offsets[0], ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));
        CV_Assert(!f.fail());

        uint32_t ਇੰਦਰਾਜ_ਅੰਕ = getHash(ਕੁੰਜੀ);

        uint32_t ਇੰਦਰਾਜ_ਵਿੱਥ = ਇੰਦਰਾਜ_Offsets[ਇੰਦਰਾਜ_ਅੰਕ];
        File_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
        ਜਦੋਂ (ਇੰਦਰਾਜ_ਵਿੱਥ > 0)
        {
            seekReadAbsolute(ਇੰਦਰਾਜ_ਵਿੱਥ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ਪਠ((ਅੱਖਰ*)&ਇੰਦਰਾਜ, ਆਕਾਰ(ਇੰਦਰਾਜ));
            CV_Assert(!f.fail());
            cv::Auto_ਬਫਰ<ਅੱਖਰ> file_ਕੁੰਜੀ(ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ + 1);
            ਜੇ (ਕੁੰਜੀ.size() == ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ)
            {
                ਜੇ (ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ > 0)
                {
                    f.ਪਠ(file_ਕੁੰਜੀ.ਅੰਕੜੇ(), ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ);
                    CV_Assert(!f.fail());
                }
                ਜੇ (memcmp(file_ਕੁੰਜੀ.ਅੰਕੜੇ(), ਕੁੰਜੀ.c_str(), ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ) == 0)
                {
                    buf.resize(ਇੰਦਰਾਜ.ਅੰਕੜੇ_ਮਾਪ);
                    f.ਪਠ(&buf[0], ਇੰਦਰਾਜ.ਅੰਕੜੇ_ਮਾਪ);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Read...");
                    ਵਾਪਸ ਸੱਚ;
                }
            }
            ਜੇ (ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ == 0)
                ਰੋਕੋ;
            ਇੰਦਰਾਜ_ਵਿੱਥ = ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ;
        }
        ਵਾਪਸ ਝੂਠ;
    }

    ਤਾਰਕਿਕ ਲਿਖ(ਅਚਲ std::string& ਕੁੰਜੀ, std::vector<ਅੱਖਰ>& buf)
    {
        ਜੇ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            ਜੇ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                ਜੇ (!f.is_open())
                {
                    CV_LOG_ERROR(ਨਿਰਾ, "Can't create file: " << fileName_);
                    ਵਾਪਸ ਝੂਠ;
                }
            }
        }

        size_t file_ਮਾਪ = get_File_ਮਾਪ();
        ਜੇ (file_ਮਾਪ == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)ਸਰੋਤ_Signature_ਮਾਪ_2);
            f.ਲਿਖ(ਸਰੋਤ_Signature, ਸਰੋਤ_Signature_ਮਾਪ_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(ਇੰਦਰਾਜ_Offsets, 0, ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));
            f.ਲਿਖ((ਅੱਖਰ*)ਇੰਦਰਾਜ_Offsets, ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_ਮਾਪ = get_File_ਮਾਪ();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ਸਰੋਤ_Signature_ਮਾਪ = readUInt32();
        CV_Assert(file_ਸਰੋਤ_Signature_ਮਾਪ == ਸਰੋਤ_Signature_ਮਾਪ_2);
        seekReadRelative(file_ਸਰੋਤ_Signature_ਮਾਪ);

        uint32_t ਸੰਖਿਆ_ਦਾ_Entries = readUInt32();
        CV_Assert(ਸੰਖਿਆ_ਦਾ_Entries > 0);
        ਜੇ (ਸੰਖਿਆ_ਦਾ_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "Invalid file: " << fileName_);
            clearFile();
            ਵਾਪਸ ਝੂਠ;
        }
        size_t ਸਾਰਣੀ_Entries_ਵਿੱਥ = (size_t)f.tellg();
        f.ਪਠ((ਅੱਖਰ*)&ਇੰਦਰਾਜ_Offsets[0], ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));
        CV_Assert(!f.fail());

        uint32_t ਇੰਦਰਾਜ_ਅੰਕ = getHash(ਕੁੰਜੀ);

        uint32_t ਇੰਦਰਾਜ_ਵਿੱਥ = ਇੰਦਰਾਜ_Offsets[ਇੰਦਰਾਜ_ਅੰਕ];
        File_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
        ਜਦੋਂ (ਇੰਦਰਾਜ_ਵਿੱਥ > 0)
        {
            seekReadAbsolute(ਇੰਦਰਾਜ_ਵਿੱਥ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ਪਠ((ਅੱਖਰ*)&ਇੰਦਰਾਜ, ਆਕਾਰ(ਇੰਦਰਾਜ));
            CV_Assert(!f.fail());
            cv::Auto_ਬਫਰ<ਅੱਖਰ> file_ਕੁੰਜੀ(ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ + 1);
            ਜੇ (ਕੁੰਜੀ.size() == ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ)
            {
                ਜੇ (ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ > 0)
                {
                    f.ਪਠ(file_ਕੁੰਜੀ.ਅੰਕੜੇ(), ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ);
                    CV_Assert(!f.fail());
                }
                ਜੇ (0 == memcmp(file_ਕੁੰਜੀ.ਅੰਕੜੇ(), ਕੁੰਜੀ.c_str(), ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Duplicate key ignored: " << fileName_);
                    ਵਾਪਸ ਝੂਠ;
                }
            }
            ਜੇ (ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ == 0)
                ਰੋਕੋ;
            ਇੰਦਰਾਜ_ਵਿੱਥ = ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ;
        }
        seekReadAbsolute(0);
        ਜੇ (ਇੰਦਰਾਜ_ਵਿੱਥ > 0)
        {
            seekWriteAbsolute(ਇੰਦਰਾਜ_ਵਿੱਥ);
            ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ = (uint32_t)file_ਮਾਪ;
            f.ਲਿਖ((ਅੱਖਰ*)&ਇੰਦਰਾਜ, ਆਕਾਰ(ਇੰਦਰਾਜ));
            CV_Assert(!f.fail());
        }
        ਨਹੀਂਤਾਂ
        {
            ਇੰਦਰਾਜ_Offsets[ਇੰਦਰਾਜ_ਅੰਕ] = (uint32_t)file_ਮਾਪ;
            seekWriteAbsolute(ਸਾਰਣੀ_Entries_ਵਿੱਥ);
            f.ਲਿਖ((ਅੱਖਰ*)ਇੰਦਰਾਜ_Offsets, ਆਕਾਰ(ਇੰਦਰਾਜ_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_ਮਾਪ);
        ਇੰਦਰਾਜ.ਅਗਲਾ_ਇੰਦਰਾਜ_File_ਵਿੱਥ = 0;
        ਇੰਦਰਾਜ.ਅੰਕੜੇ_ਮਾਪ = (uint32_t)buf.size();
        ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ = (uint32_t)ਕੁੰਜੀ.size();
        f.ਲਿਖ((ਅੱਖਰ*)&ਇੰਦਰਾਜ, ਆਕਾਰ(ਇੰਦਰਾਜ));
        CV_Assert(!f.fail());
        f.ਲਿਖ(ਕੁੰਜੀ.c_str(), ਇੰਦਰਾਜ.ਕੁੰਜੀ_ਮਾਪ);
        CV_Assert(!f.fail());
        f.ਲਿਖ(&buf[0], ਇੰਦਰਾਜ.ਅੰਕੜੇ_ਮਾਪ);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Write... (" << buf.size() << " bytes)");
        ਵਾਪਸ ਸੱਚ;
    }
};
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT



ਬਣਤਰ OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    ਪੂਰਨ device_;  // device index in context
    ocl::ਕਤਾਰ ਕਤਾਰ_3;
    ਪੂਰਨ useOpenCL_;

ਸੁਰੱਖਿਅਤ:
    Impl() = ਮਿਟਾਓ;

    ਖਾਲੀ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        ਪੂਰਨ ndevices = (ਪੂਰਨ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
        ਲਈ (ਪੂਰਨ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            ਜੇ (dhandle == deviceID)
            {
                device_ = i;
                ਮਿਲਿਆ = ਸੱਚ;
                ਰੋਕੋ;
            }
        }
        CV_Assert(ਮਿਲਿਆ && "OpenCL device can't work with passed OpenCL context");
    }

    ਖਾਲੀ _init_device(ਅਚਲ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        ਪੂਰਨ ndevices = (ਪੂਰਨ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
        ਲਈ (ਪੂਰਨ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            ਜੇ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                ਮਿਲਿਆ = ਸੱਚ;
                ਰੋਕੋ;
            }
        }
        CV_Assert(ਮਿਲਿਆ && "OpenCL device can't work with passed OpenCL context");
    }

ਜਨਤਕ:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_ਅਣਵਰਤਿਆ(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        ਕਤਾਰ_3 = ਕਤਾਰ(context_, context_.device(device_));
    }

    Impl(ਅਚਲ ocl::Context& context, ਅਚਲ ocl::Device& device, ਅਚਲ ocl::ਕਤਾਰ& ਕਤਾਰ_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        ਕਤਾਰ_3 = ਕਤਾਰ_2;
    }

    Impl(ਅਚਲ ocl::Context& context, ਅਚਲ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        ਕਤਾਰ_3 = ਕਤਾਰ(context_, context_.device(device_));
    }

    Impl(ਅਚਲ ocl::Context& context, ਅਚਲ ਪੂਰਨ device, ਅਚਲ ocl::ਕਤਾਰ& ਕਤਾਰ_2)
        : context_(context)
        , device_(device)
        , ਕਤਾਰ_3(ਕਤਾਰ_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ਅਚਲ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , ਕਤਾਰ_3(other.ਕਤਾਰ_3)
        , useOpenCL_(-1)
    {
        // nothing
    }

    ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ useOpenCL() ਅਚਲ { ਵਾਪਸ const_cast<Impl*>(ਇਹ)->useOpenCL(); }
    ਤਾਰਕਿਕ useOpenCL()
    {
        ਜੇ (useOpenCL_ < 0)
        {
            ਕੋਸ਼ਿਸ਼
            {
                useOpenCL_ = 0;
                ਜੇ (!context_.empty() && context_.ndevices() > 0)
                {
                    ਅਚਲ Device& d = context_.device(device_);
                    useOpenCL_ = d.ਉਪਲਬਧ();
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception&)
            {
                // nothing
            }
            ਜੇ (!useOpenCL_)
                CV_LOG_INFO(ਨਿਰਾ, "OpenCL: can't use OpenCL execution context");
        }
        ਵਾਪਸ useOpenCL_ > 0;
    }

    ਖਾਲੀ ਸੈੱਟ_Use_Open_CL(ਤਾਰਕਿਕ flag)
    {
        ਜੇ (!flag)
            useOpenCL_ = 0;
        ਨਹੀਂਤਾਂ
            useOpenCL_ = -1;
    }

    ਸਥਿਰ ਅਚਲ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ਨਿਰਾ, "OpenCL: initializing thread execution context");

        ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
        ਸਥਿਰ std::shared_ptr<Impl> g_primaryExecutionContext;

        ਜੇ (!initialized)
        {
            cv::Auto_ਤਾਲਾ lock(getInitializationMutex());
            ਜੇ (!initialized)
            {
                CV_LOG_INFO(ਨਿਰਾ, "OpenCL: creating new execution context...");
                ਕੋਸ਼ਿਸ਼
                {
                    Context c = ocl::Context::create(std::string());
                    ਜੇ (c.ndevices())
                    {
                        ਪੂਰਨ deviceId = 0;
                        ਸਵੈਚਾਲਿਤ& d = c.device(deviceId);
                        ਜੇ (d.ਉਪਲਬਧ())
                        {
                            ਸਵੈਚਾਲਿਤ q = ocl::ਕਤਾਰ(c, d);
                            ਜੇ (!q.ptr())
                            {
                                CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: Can't create default OpenCL queue");
                            }
                            ਨਹੀਂਤਾਂ
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ਨਿਰਾ, "OpenCL: device=" << d.name());
                            }
                        }
                        ਨਹੀਂਤਾਂ
                        {
                            CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        CV_LOG_INFO(ਨਿਰਾ, "OpenCL: context is not available/disabled");
                    }
                }
                ਫੜੋ (ਅਚਲ std::exception& e)
                {
                    CV_LOG_INFO(ਨਿਰਾ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                ਫੜੋ (...)
                {
                    CV_LOG_WARNING(ਨਿਰਾ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = ਸੱਚ;
            }
        }
        ਵਾਪਸ g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->context_;
}
Device& OpenCLExecutionContext::getDevice() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->context_.device(p->device_);
}
ਕਤਾਰ& OpenCLExecutionContext::get_ਕਤਾਰ() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->ਕਤਾਰ_3;
}

ਤਾਰਕਿਕ OpenCLExecutionContext::useOpenCL() ਅਚਲ
{
    ਜੇ (p)
        ਵਾਪਸ p->useOpenCL();
    ਵਾਪਸ ਝੂਠ;
}
ਖਾਲੀ OpenCLExecutionContext::ਸੈੱਟ_Use_Open_CL(ਤਾਰਕਿਕ flag)
{
    CV_Assert(p);
    p->ਸੈੱਟ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    OpenCLExecutionContext& c = ਅੰਕੜੇ.oclExecutionContext;
    ਜੇ (!ਅੰਕੜੇ.oclExecutionContextInitialized)
    {
        ਅੰਕੜੇ.oclExecutionContextInitialized = ਸੱਚ;
        ਜੇ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ਵਾਪਸ c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    OpenCLExecutionContext& c = ਅੰਕੜੇ.oclExecutionContext;
    ਵਾਪਸ c;
}

ਖਾਲੀ OpenCLExecutionContext::bind() ਅਚਲ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    ਅੰਕੜੇ.oclExecutionContext = *ਇਹ;
    ਅੰਕੜੇ.oclExecutionContextInitialized = ਸੱਚ;
    ਅੰਕੜੇ.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ਕਤਾਰ() ਅਚਲ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ਅਚਲ ਕਤਾਰ q(getContext(), getDevice());
    ਵਾਪਸ clone_With_New_ਕਤਾਰ(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_ਕਤਾਰ(ਅਚਲ ocl::ਕਤਾਰ& q) ਅਚਲ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ਨਿਰਾ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ਵਾਪਸ c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ਅਚਲ Context& context, ਅਚਲ Device& device, ਅਚਲ ocl::ਕਤਾਰ& ਕਤਾਰ_2)
{
    CV_TRACE_FUNCTION();
    ਜੇ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, ਕਤਾਰ_2);
    ਵਾਪਸ ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ਅਚਲ Context& context, ਅਚਲ Device& device)
{
    CV_TRACE_FUNCTION();
    ਜੇ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ਵਾਪਸ ctx;

}

ਖਾਲੀ OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ਸਥਿਰ ਤਾਰਕਿਕ g_isOpenCLInitialized = ਝੂਠ;
ਸਥਿਰ ਤਾਰਕਿਕ g_isOpenCLAvailable = ਝੂਠ;

ਤਾਰਕਿਕ haveOpenCL()
{
    CV_TRACE_FUNCTION();

    ਜੇ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string env_ਰਸਤਾ = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        ਜੇ (!env_ਰਸਤਾ.empty())
        {
            ਜੇ (env_ਰਸਤਾ == "disabled")
            {
                g_isOpenCLAvailable = ਝੂਠ;
                g_isOpenCLInitialized = ਸੱਚ;
                ਵਾਪਸ ਝੂਠ;
            }
        }

        cv::Auto_ਤਾਲਾ lock(getInitializationMutex());
        CV_LOG_INFO(ਨਿਰਾ, "Initialize OpenCL runtime...");
        ਕੋਸ਼ਿਸ਼
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ਨਿਰਾ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: found " << n << " platforms");
        }
        ਫੜੋ (...)
        {
            g_isOpenCLAvailable = ਝੂਠ;
        }
        g_isOpenCLInitialized = ਸੱਚ;
    }
    ਵਾਪਸ g_isOpenCLAvailable;
}

ਤਾਰਕਿਕ useOpenCL()
{
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    ਜੇ (ਅੰਕੜੇ.useOpenCL < 0)
    {
        ਕੋਸ਼ਿਸ਼
        {
            ਅੰਕੜੇ.useOpenCL = 0;
            ਜੇ (haveOpenCL())
            {
                ਸਵੈਚਾਲਿਤ c = OpenCLExecutionContext::getCurrent();
                ਅੰਕੜੇ.useOpenCL = c.useOpenCL();
            }
        }
        ਫੜੋ (...)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ਵਾਪਸ ਅੰਕੜੇ.useOpenCL > 0;
}

ਤਾਰਕਿਕ isOpenCLActivated()
{
    ਜੇ (!g_isOpenCLAvailable)
        ਵਾਪਸ ਝੂਠ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ਵਾਪਸ useOpenCL();
}

ਖਾਲੀ ਸੈੱਟ_Use_Open_CL(ਤਾਰਕਿਕ flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    ਸਵੈਚਾਲਿਤ& c = OpenCLExecutionContext::getCurrentRef();
    ਜੇ (!c.empty())
    {
        c.ਸੈੱਟ_Use_Open_CL(flag);
        ਅੰਕੜੇ.useOpenCL = c.useOpenCL();
    }
    ਨਹੀਂਤਾਂ
    {
        ਜੇ (!flag)
            ਅੰਕੜੇ.useOpenCL = 0;
        ਨਹੀਂਤਾਂ
            ਅੰਕੜੇ.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDBLAS

ਵਰਗ AmdBlasHelper
{
ਜਨਤਕ:
    ਸਥਿਰ AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, ਨਵਾਂ AmdBlasHelper())
    }

    ਤਾਰਕਿਕ is_ਉਪਲਬਧ() ਅਚਲ
    {
        ਵਾਪਸ g_is_Amd_Blas_ਉਪਲਬਧ;
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

ਸੁਰੱਖਿਅਤ:
    AmdBlasHelper()
    {
        ਜੇ (!g_isAmdBlasInitialized)
        {
            Auto_ਤਾਲਾ lock(getInitializationMutex());

            ਜੇ (!g_isAmdBlasInitialized)
            {
                ਜੇ (haveOpenCL())
                {
                    ਕੋਸ਼ਿਸ਼
                    {
                        g_is_Amd_Blas_ਉਪਲਬਧ = clblas_ਤਿਆਰੀ() == clblasSuccess;
                    }
                    ਫੜੋ (...)
                    {
                        g_is_Amd_Blas_ਉਪਲਬਧ = ਝੂਠ;
                    }
                }
                ਨਹੀਂਤਾਂ
                    g_is_Amd_Blas_ਉਪਲਬਧ = ਝੂਠ;

                g_isAmdBlasInitialized = ਸੱਚ;
            }
        }
    }

ਨਿਜੀ:
    ਸਥਿਰ ਤਾਰਕਿਕ g_isAmdBlasInitialized;
    ਸਥਿਰ ਤਾਰਕਿਕ g_is_Amd_Blas_ਉਪਲਬਧ;
};

ਤਾਰਕਿਕ AmdBlasHelper::g_is_Amd_Blas_ਉਪਲਬਧ = ਝੂਠ;
ਤਾਰਕਿਕ AmdBlasHelper::g_isAmdBlasInitialized = ਝੂਠ;

ਤਾਰਕਿਕ haveAmdBlas()
{
    ਵਾਪਸ AmdBlasHelper::getInstance().is_ਉਪਲਬਧ();
}

#ਨਹੀਂਤਾਂ

ਤਾਰਕਿਕ haveAmdBlas()
{
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_CLAMDFFT

ਵਰਗ AmdFftHelper
{
ਜਨਤਕ:
    ਸਥਿਰ AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, ਨਵਾਂ AmdFftHelper())
    }

    ਤਾਰਕਿਕ is_ਉਪਲਬਧ() ਅਚਲ
    {
        ਵਾਪਸ g_is_Amd_Fft_ਉਪਲਬਧ;
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

ਸੁਰੱਖਿਅਤ:
    AmdFftHelper()
    {
        ਜੇ (!g_isAmdFftInitialized)
        {
            Auto_ਤਾਲਾ lock(getInitializationMutex());

            ਜੇ (!g_isAmdFftInitialized)
            {
                ਜੇ (haveOpenCL())
                {
                    ਕੋਸ਼ਿਸ਼
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_ਤਿਆਰੀ_ਅੰਕੜੇ(&ਤਿਆਰੀ_ਅੰਕੜੇ) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_ਸੰਸਕਰਣ(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_ਉਪਲਬਧ = ਸੱਚ;
                    }
                    ਫੜੋ (ਅਚਲ Exception &)
                    {
                        g_is_Amd_Fft_ਉਪਲਬਧ = ਝੂਠ;
                    }
                }
                ਨਹੀਂਤਾਂ
                    g_is_Amd_Fft_ਉਪਲਬਧ = ਝੂਠ;

                g_isAmdFftInitialized = ਸੱਚ;
            }
        }
    }

ਨਿਜੀ:
    ਸਥਿਰ clfft_ਤਿਆਰੀ_ਅੰਕੜੇ ਤਿਆਰੀ_ਅੰਕੜੇ;
    ਸਥਿਰ ਤਾਰਕਿਕ g_isAmdFftInitialized;
    ਸਥਿਰ ਤਾਰਕਿਕ g_is_Amd_Fft_ਉਪਲਬਧ;
};

clfft_ਤਿਆਰੀ_ਅੰਕੜੇ AmdFftHelper::ਤਿਆਰੀ_ਅੰਕੜੇ;
ਤਾਰਕਿਕ AmdFftHelper::g_is_Amd_Fft_ਉਪਲਬਧ = ਝੂਠ;
ਤਾਰਕਿਕ AmdFftHelper::g_isAmdFftInitialized = ਝੂਠ;

ਤਾਰਕਿਕ haveAmdFft()
{
    ਵਾਪਸ AmdFftHelper::getInstance().is_ਉਪਲਬਧ();
}

#ਨਹੀਂਤਾਂ

ਤਾਰਕਿਕ haveAmdFft()
{
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ haveSVM()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਅੰਤ_2()
{
    ਕਤਾਰ::getDefault().ਅੰਤ_2();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ਬਣਤਰ Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = ਝੂਠ;
    }

    ~Impl() {}

    ਖਾਲੀ init()
    {
        ਜੇ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            ਜੇ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            ਜੇ( handle != 0 )
            {
                ਅੱਖਰ buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, ਆਕਾਰ(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = ਸੱਚ;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    ਤਾਰਕਿਕ initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    ਜੇ(p)
        p->release();
}

Platform::Platform(ਅਚਲ Platform& pl)
{
    p = (Impl*)pl.p;
    ਜੇ(p)
        p->addref();
}

Platform& Platform::ਸੰਚਾਲਕ = (ਅਚਲ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = ਖਾਲੀ_ਸੂਚਕ;
}

Platform& Platform::ਸੰਚਾਲਕ = (Platform&& pl) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &pl) {
        ਜੇ(p)
            p->release();
        p = pl.p;
        pl.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ* Platform::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ਨਿਰਾ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ਸਥਿਰ Platform p;
    ਜੇ( !p.p )
    {
        p.p = ਨਵਾਂ Impl;
        p.p->init();
    }
    ਵਾਪਸ p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ਸਥਿਰ ਖਾਲੀ parseOpenCLVersion(ਅਚਲ String &ਸੰਸਕਰਣ, ਪੂਰਨ &major, ਪੂਰਨ &minor)
{
    major = minor = 0;
    ਜੇ (10 >= ਸੰਸਕਰਣ.length())
        ਵਾਪਸ;
    ਅਚਲ ਅੱਖਰ *pstr = ਸੰਸਕਰਣ.c_str();
    ਜੇ (0 != ਲੜੀ_ਤੁਲਨਾ(pstr, "OpenCL ", 7))
        ਵਾਪਸ;
    size_t ppos = ਸੰਸਕਰਣ.find('.', 7);
    ਜੇ (String::npos == ppos)
        ਵਾਪਸ;
    String ਆਰਜ਼ੀ_2 = ਸੰਸਕਰਣ.substr(7, ppos - 7);
    major = atoi(ਆਰਜ਼ੀ_2.c_str());
    ਆਰਜ਼ੀ_2 = ਸੰਸਕਰਣ.substr(ppos + 1);
    minor = atoi(ਆਰਜ਼ੀ_2.c_str());
}

ਬਣਤਰ Device::Impl
{
    Impl(ਖਾਲੀ* d)
        : refcount(1)
        , handle(0)
    {
        ਕੋਸ਼ਿਸ਼
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        ਫੜੋ (...)
        {
            ਸੁੱਟੋ;
        }
    }

    ਖਾਲੀ _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        ਸੰਸਕਰਣ_3 = getStrProp(CL_DEVICE_ਸੰਸਕਰਣ);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, ਪੂਰਨ>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, ਪੂਰਨ>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_ਸਮ੍ਰਿਤੀ = getBoolProp(CL_DEVICE_HOST_UNIFIED_ਸਮ੍ਰਿਤੀ);
        maxComputeUnits_ = getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_ਸਮੂਹ_ਮਾਪ_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_ਸਮੂਹ_ਮਾਪ);
        type_ = getProp<cl_device_type, ਪੂਰਨ>(CL_DEVICE_TYPE);
        driver_ਸੰਸਕਰਣ_2 = getStrProp(CL_DRIVER_ਸੰਸਕਰਣ);
        ਪਤਾ_ਬਿੱਟ_2 = getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_ਪਤਾ_ਬਿੱਟ);

        String device_ਸੰਸਕਰਣ = getStrProp(CL_DEVICE_ਸੰਸਕਰਣ);
        parseOpenCLVersion(device_ਸੰਸਕਰਣ, device_ਸੰਸਕਰਣ_Major_2, device_ਸੰਸਕਰਣ_Minor_2);

        size_t pos = 0;
        ਜਦੋਂ (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            ਜੇ (pos2 == String::npos)
                pos2 = extensions_.size();
            ਜੇ (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_ਸੈੱਟ.insert(extensionName);
            }
            pos = pos2 + 1;
        }

        khr_fp64_support_ = isExtensionSupported("cl_khr_fp64");
        khr_fp16_support_ = isExtensionSupported("cl_khr_fp16");

        intelSubgroupsSupport_ = isExtensionSupported("cl_intel_subgroups");

        vendorName_ = getStrProp(CL_DEVICE_VENDOR);
        ਜੇ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        ਨਹੀਂਤਾਂ ਜੇ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || ਲੜੀ_ਖੋਜ(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        ਨਹੀਂਤਾਂ ਜੇ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        ਨਹੀਂਤਾਂ
            vendorID_ = UNKNOWN_VENDOR;

        ਅਚਲ size_t CV_OPENCL_DEVICE_MAX_WORK_ਸਮੂਹ_ਮਾਪ = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        ਜੇ (CV_OPENCL_DEVICE_MAX_WORK_ਸਮੂਹ_ਮਾਪ > 0)
        {
            ਅਚਲ size_t new_max_Work_ਸਮੂਹ_ਮਾਪ = std::min(max_Work_ਸਮੂਹ_ਮਾਪ_2, CV_OPENCL_DEVICE_MAX_WORK_ਸਮੂਹ_ਮਾਪ);
            ਜੇ (new_max_Work_ਸਮੂਹ_ਮਾਪ != max_Work_ਸਮੂਹ_ਮਾਪ_2)
                CV_LOG_WARNING(ਨਿਰਾ, "OpenCL: using workgroup size: " << new_max_Work_ਸਮੂਹ_ਮਾਪ << " (was " << max_Work_ਸਮੂਹ_ਮਾਪ_2 << ")");
            max_Work_ਸਮੂਹ_ਮਾਪ_2 = new_max_Work_ਸਮੂਹ_ਮਾਪ;
        }
#ਜੇ 0
        ਜੇ (isExtensionSupported("cl_khr_spir"))
        {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CL_DEVICE_SPIR_VERSIONS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CL_DEVICE_SPIR_VERSIONS                     0x40E0
#ਜੇ_ਅੰਤ
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::ਬਾਹਰ << spir_versions << std::ਸਤਰ_ਅੰਤ;
        }
#ਜੇ_ਅੰਤ
    }

    ~Impl()
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
        ਜੇ (!cv::__termination)
#ਜੇ_ਅੰਤ
        {
            ਜੇ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ _TpCL, ਕਿਸਮ_ਨਾਂ Tp_ਬਾਹਰਲਾ>
    Tp_ਬਾਹਰਲਾ getProp(cl_device_info prop) ਅਚਲ
    {
        _TpCL ਆਰਜ਼ੀ_2=_TpCL();
        size_t sz = 0;

        ਵਾਪਸ clGetDeviceInfo(handle, prop, ਆਕਾਰ(ਆਰਜ਼ੀ_2), &ਆਰਜ਼ੀ_2, &sz) == CL_SUCCESS &&
            sz == ਆਕਾਰ(ਆਰਜ਼ੀ_2) ? Tp_ਬਾਹਰਲਾ(ਆਰਜ਼ੀ_2) : Tp_ਬਾਹਰਲਾ();
    }

    ਤਾਰਕਿਕ getBoolProp(cl_device_info prop) ਅਚਲ
    {
        cl_bool ਆਰਜ਼ੀ_2 = CL_FALSE;
        size_t sz = 0;

        ਵਾਪਸ clGetDeviceInfo(handle, prop, ਆਕਾਰ(ਆਰਜ਼ੀ_2), &ਆਰਜ਼ੀ_2, &sz) == CL_SUCCESS &&
            sz == ਆਕਾਰ(ਆਰਜ਼ੀ_2) ? ਆਰਜ਼ੀ_2 != 0 : ਝੂਠ;
    }

    String getStrProp(cl_device_info prop) ਅਚਲ
    {
        ਅੱਖਰ buf[4096];
        size_t sz=0;
        ਵਾਪਸ clGetDeviceInfo(handle, prop, ਆਕਾਰ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ਆਕਾਰ(buf) ? String(buf) : String();
    }

    ਤਾਰਕਿਕ isExtensionSupported(ਅਚਲ std::string& extensionName) ਅਚਲ
    {
        ਵਾਪਸ extensions_ਸੈੱਟ.ਗਿਣਤੀ_3(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String ਸੰਸਕਰਣ_3;
    std::string extensions_;
    ਪੂਰਨ doubleFPConfig_;
    ਤਾਰਕਿਕ khr_fp64_support_;
    ਪੂਰਨ halfFPConfig_;
    ਤਾਰਕਿਕ khr_fp16_support_;
    ਤਾਰਕਿਕ host_Unified_ਸਮ੍ਰਿਤੀ;
    ਪੂਰਨ maxComputeUnits_;
    size_t max_Work_ਸਮੂਹ_ਮਾਪ_2;
    ਪੂਰਨ type_;
    ਪੂਰਨ ਪਤਾ_ਬਿੱਟ_2;
    ਪੂਰਨ device_ਸੰਸਕਰਣ_Major_2;
    ਪੂਰਨ device_ਸੰਸਕਰਣ_Minor_2;
    String driver_ਸੰਸਕਰਣ_2;
    String vendorName_;
    ਪੂਰਨ vendorID_;
    ਤਾਰਕਿਕ intelSubgroupsSupport_;

    std::ਸੈੱਟ<std::string> extensions_ਸੈੱਟ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(ਖਾਲੀ* d)
{
    p = 0;
    ਸੈੱਟ(d);
}

Device::Device(ਅਚਲ Device& d)
{
    p = d.p;
    ਜੇ(p)
        p->addref();
}

Device& Device::ਸੰਚਾਲਕ = (ਅਚਲ Device& d)
{
    Impl* newp = (Impl*)d.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = ਖਾਲੀ_ਸੂਚਕ;
}

Device& Device::ਸੰਚਾਲਕ = (Device&& d) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &d) {
        ਜੇ(p)
            p->release();
        p = d.p;
        d.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

Device::~Device()
{
    ਜੇ(p)
        p->release();
}

ਖਾਲੀ Device::ਸੈੱਟ(ਖਾਲੀ* d)
{
    ਜੇ(p)
        p->release();
    p = ਨਵਾਂ Impl(d);
    ਜੇ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(ਖਾਲੀ* d)
{
    Device device(d);
    ਵਾਪਸ device;
}

ਖਾਲੀ* Device::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

String Device::name() ਅਚਲ
{ ਵਾਪਸ p ? p->name_ : String(); }

String Device::extensions() ਅਚਲ
{ ਵਾਪਸ p ? String(p->extensions_) : String(); }

ਤਾਰਕਿਕ Device::isExtensionSupported(ਅਚਲ String& extensionName) ਅਚਲ
{ ਵਾਪਸ p ? p->isExtensionSupported(extensionName) : ਝੂਠ; }

String Device::ਸੰਸਕਰਣ() ਅਚਲ
{ ਵਾਪਸ p ? p->ਸੰਸਕਰਣ_3 : String(); }

String Device::vendorName() ਅਚਲ
{ ਵਾਪਸ p ? p->vendorName_ : String(); }

ਪੂਰਨ Device::vendorID() ਅਚਲ
{ ਵਾਪਸ p ? p->vendorID_ : 0; }

String Device::Open_CL_C_ਸੰਸਕਰਣ() ਅਚਲ
{ ਵਾਪਸ p ? p->getStrProp(CL_DEVICE_OPENCL_C_ਸੰਸਕਰਣ) : String(); }

String Device::OpenCLVersion() ਅਚਲ
{ ਵਾਪਸ p ? p->getStrProp(CL_DEVICE_ਸੰਸਕਰਣ) : String(); }

ਪੂਰਨ Device::device_ਸੰਸਕਰਣ_Major() ਅਚਲ
{ ਵਾਪਸ p ? p->device_ਸੰਸਕਰਣ_Major_2 : 0; }

ਪੂਰਨ Device::device_ਸੰਸਕਰਣ_Minor() ਅਚਲ
{ ਵਾਪਸ p ? p->device_ਸੰਸਕਰਣ_Minor_2 : 0; }

String Device::driver_ਸੰਸਕਰਣ() ਅਚਲ
{ ਵਾਪਸ p ? p->driver_ਸੰਸਕਰਣ_2 : String(); }

ਪੂਰਨ Device::type() ਅਚਲ
{ ਵਾਪਸ p ? p->type_ : 0; }

ਪੂਰਨ Device::ਪਤਾ_ਬਿੱਟ() ਅਚਲ
{ ਵਾਪਸ p ? p->ਪਤਾ_ਬਿੱਟ_2 : 0; }

ਤਾਰਕਿਕ Device::ਉਪਲਬਧ() ਅਚਲ
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_ਉਪਲਬਧ) : ਝੂਠ; }

ਤਾਰਕਿਕ Device::compiler_ਉਪਲਬਧ() ਅਚਲ
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_COMPILER_ਉਪਲਬਧ) : ਝੂਠ; }

ਤਾਰਕਿਕ Device::linker_ਉਪਲਬਧ() ਅਚਲ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_ਸੰਸਕਰਣ_1_2
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_LINKER_ਉਪਲਬਧ) : ਝੂਠ; }
#ਨਹੀਂਤਾਂ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ਜੇ_ਅੰਤ

ਪੂਰਨ Device::doubleFPConfig() ਅਚਲ
{ ਵਾਪਸ p ? p->doubleFPConfig_ : 0; }

ਪੂਰਨ Device::singleFPConfig() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_device_fp_config, ਪੂਰਨ>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

ਪੂਰਨ Device::halfFPConfig() ਅਚਲ
{ ਵਾਪਸ p ? p->halfFPConfig_ : 0; }

ਤਾਰਕਿਕ Device::hasFP64() ਅਚਲ
{ ਵਾਪਸ p ? p->khr_fp64_support_ : ਝੂਠ; }
ਤਾਰਕਿਕ Device::hasFP16() ਅਚਲ
{ ਵਾਪਸ p ? p->khr_fp16_support_ : ਝੂਠ; }

ਤਾਰਕਿਕ Device::endianLittle() ਅਚਲ
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : ਝੂਠ; }

ਤਾਰਕਿਕ Device::errorCorrectionSupport() ਅਚਲ
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : ਝੂਠ; }

ਪੂਰਨ Device::executionCapabilities() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_device_exec_capabilities, ਪੂਰਨ>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_ਮਾਪ) : 0; }

ਪੂਰਨ Device::globalMemCacheType() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_device_mem_cache_type, ਪੂਰਨ>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

ਪੂਰਨ Device::global_Mem_Cache_ਸਤਰ_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_GLOBAL_MEM_CACHELINE_ਮਾਪ) : 0; }

size_t Device::global_Mem_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_ਮਾਪ) : 0; }

size_t Device::local_Mem_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_ਮਾਪ) : 0; }

ਪੂਰਨ Device::localMemType() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_device_local_mem_type, ਪੂਰਨ>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

ਤਾਰਕਿਕ Device::host_Unified_ਸਮ੍ਰਿਤੀ_2() ਅਚਲ
{ ਵਾਪਸ p ? p->host_Unified_ਸਮ੍ਰਿਤੀ : ਝੂਠ; }

ਤਾਰਕਿਕ Device::imageSupport() ਅਚਲ
{ ਵਾਪਸ p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : ਝੂਠ; }

ਤਾਰਕਿਕ Device::image_From_ਬਫਰ_Support() ਅਚਲ
{
    ਵਾਪਸ p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : ਝੂਠ;
}

uint Device::imagePitchAlignment() ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ਵਾਪਸ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

uint Device::image_ਅਧਾਰ_ਪਤਾ_Alignment() ਅਚਲ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_DEVICE_IMAGE_ਅਧਾਰ_ਪਤਾ_ALIGNMENT
    ਵਾਪਸ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_ਅਧਾਰ_ਪਤਾ_ALIGNMENT) : 0;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

size_t Device::image2_DMax_ਚੌੜਾਈ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ਚੌੜਾਈ) : 0; }

size_t Device::image2_DMax_ਉਚਾਈ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ਉਚਾਈ) : 0; }

size_t Device::image3_DMax_ਚੌੜਾਈ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ਚੌੜਾਈ) : 0; }

size_t Device::image3_DMax_ਉਚਾਈ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ਉਚਾਈ) : 0; }

size_t Device::image3_DMax_ਡੂੰਘਾਈ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ਡੂੰਘਾਈ) : 0; }

size_t Device::image_Max_ਬਫਰ_ਮਾਪ() ਅਚਲ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_ਸੰਸਕਰਣ_1_2
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ਬਫਰ_ਮਾਪ) : 0; }
#ਨਹੀਂਤਾਂ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ਜੇ_ਅੰਤ

size_t Device::image_Max_Array_ਮਾਪ() ਅਚਲ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_ਸੰਸਕਰਣ_1_2
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_ਮਾਪ) : 0; }
#ਨਹੀਂਤਾਂ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ਜੇ_ਅੰਤ

ਤਾਰਕਿਕ Device::intelSubgroupsSupport() ਅਚਲ
{ ਵਾਪਸ p ? p->intelSubgroupsSupport_ : ਝੂਠ; }

ਪੂਰਨ Device::maxClockFrequency() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

ਪੂਰਨ Device::maxComputeUnits() ਅਚਲ
{ ਵਾਪਸ p ? p->maxComputeUnits_ : 0; }

ਪੂਰਨ Device::max_Constant_ਦਲੀਲਾਂ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_CONSTANT_ਦਲੀਲਾਂ) : 0; }

size_t Device::max_Constant_ਬਫਰ_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_ਬਫਰ_ਮਾਪ) : 0; }

size_t Device::max_Mem_Alloc_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_ਮਾਪ) : 0; }

size_t Device::max_Parameter_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_ਮਾਪ) : 0; }

ਪੂਰਨ Device::max_Read_Image_ਦਲੀਲਾਂ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_READ_IMAGE_ਦਲੀਲਾਂ) : 0; }

ਪੂਰਨ Device::max_Write_Image_ਦਲੀਲਾਂ() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_WRITE_IMAGE_ਦਲੀਲਾਂ) : 0; }

ਪੂਰਨ Device::maxSamplers() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_ਸਮੂਹ_ਮਾਪ() ਅਚਲ
{ ਵਾਪਸ p ? p->max_Work_ਸਮੂਹ_ਮਾਪ_2 : 0; }

ਪੂਰਨ Device::max_Work_ਵਸਤੂ_Dims() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MAX_WORK_ਵਸਤੂ_DIMENSIONS) : 0; }

ਖਾਲੀ Device::max_Work_ਵਸਤੂ_Sizes(size_t* sizes) ਅਚਲ
{
    ਜੇ(p)
    {
        ਅਚਲ ਪੂਰਨ MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ਵਸਤੂ_SIZES,
                MAX_DIMS*ਆਕਾਰ(sizes[0]), &sizes[0], &retsz));
    }
}

ਪੂਰਨ Device::mem_ਅਧਾਰ_Addr_Align() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_MEM_ਅਧਾਰ_ADDR_ALIGN) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Char() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_CHAR) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Short() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_SHORT) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Int() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_INT) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Long() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_LONG) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Float() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_FLOAT) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Double() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_DOUBLE) : 0; }

ਪੂਰਨ Device::native_ਸਦਿਸ਼_ਚੌੜਾਈ_Half() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_NATIVE_ਸਦਿਸ਼_ਚੌੜਾਈ_HALF) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Char() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_CHAR) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Short() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_SHORT) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Int() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_INT) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Long() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_LONG) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Float() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_FLOAT) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Double() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_DOUBLE) : 0; }

ਪੂਰਨ Device::preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Half() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<cl_uint, ਪੂਰਨ>(CL_DEVICE_PREFERRED_ਸਦਿਸ਼_ਚੌੜਾਈ_HALF) : 0; }

size_t Device::printf_ਬਫਰ_ਮਾਪ() ਅਚਲ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_ਸੰਸਕਰਣ_1_2
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_ਬਫਰ_ਮਾਪ) : 0; }
#ਨਹੀਂਤਾਂ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#ਜੇ_ਅੰਤ


size_t Device::profilingTimerResolution() ਅਚਲ
{ ਵਾਪਸ p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ਅਚਲ Device& Device::getDefault()
{
    ਸਵੈਚਾਲਿਤ& c = OpenCLExecutionContext::getCurrent();
    ਜੇ (!c.empty())
    {
        ਵਾਪਸ c.getDevice();
    }

    ਸਥਿਰ Device dummy;
    ਵਾਪਸ dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Functor, ਕਿਸਮ_ਨਾਂ ObjectType>
ਸਤਰਅੰਦਰ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ਨਿਰਾ, &required);
    ਜੇ (err != CL_SUCCESS)
        ਵਾਪਸ err;

    param.clear();
    ਜੇ (required > 0)
    {
        Auto_ਬਫਰ<ਅੱਖਰ> buf(required + 1);
        ਅੱਖਰ* ptr = buf.ਅੰਕੜੇ(); // cleanup is not needed
        err = f(obj, name, required, ptr, ਨਿਰਾ);
        ਜੇ (err != CL_SUCCESS)
            ਵਾਪਸ err;
        param = ptr;
    }

    ਵਾਪਸ CL_SUCCESS;
}

ਸਥਿਰ ਖਾਲੀ ਵੰਡੋ(ਅਚਲ std::string &s, ਅੱਖਰ delim, std::vector<std::string> &elems)
{
    elems.clear();
    ਜੇ (s.size() == 0)
        ਵਾਪਸ;
    std::istringstream ss(s);
    std::string ਵਸਤੂ;
    ਜਦੋਂ (!ss.eof())
    {
        std::getline(ss, ਵਸਤੂ, delim);
        elems.push_back(ਵਸਤੂ);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ਸਥਿਰ ਤਾਰਕਿਕ parseOpenCLDeviceConfiguration(ਅਚਲ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    ਵੰਡੋ(configurationStr, ':', parts);
    ਜੇ (parts.size() > 3)
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ਵਾਪਸ ਝੂਠ;
    }
    ਜੇ (parts.size() > 2)
        deviceNameOrID = parts[2];
    ਜੇ (parts.size() > 1)
    {
        ਵੰਡੋ(parts[1], '|', deviceTypes);
    }
    ਜੇ (parts.size() > 0)
    {
        platform = parts[0];
    }
    ਵਾਪਸ ਸੱਚ;
}

ਸਥਿਰ cl_device_id selectOpenCLDevice(ਅਚਲ std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    ਜੇ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    ਜੇ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ਵਾਪਸ ਨਿਰਾ;

    ਤਾਰਕਿਕ isID = ਝੂਠ;
    ਪੂਰਨ deviceID = -1;
    ਜੇ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = ਸੱਚ;
        ਲਈ (size_t i = 0; i < deviceName.length(); i++)
        {
            ਜੇ (!ਅੰਕ_ਹੈ(deviceName[i]))
            {
                isID = ਝੂਠ;
                ਰੋਕੋ;
            }
        }
        ਜੇ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            ਜੇ (deviceID < 0)
                ਵਾਪਸ ਨਿਰਾ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint ਅੰਕ_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms));

        ਜੇ (ਅੰਕ_Platforms == 0)
            ਵਾਪਸ ਨਿਰਾ;
        platforms.resize((size_t)ਅੰਕ_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], &ਅੰਕ_Platforms));
        platforms.resize(ਅੰਕ_Platforms);
    }

    ਜੇ (platform.length() > 0)
    {
        ਲਈ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            ਜੇ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            ਨਹੀਂਤਾਂ
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        ਜੇ (platforms.size() == 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            ਜਾਓ not_ਮਿਲਿਆ;
        }
    }
    ਜੇ (deviceTypes.size() == 0)
    {
        ਜੇ (!isID)
        {
            deviceTypes.push_back("GPU");
            ਜੇ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        ਨਹੀਂਤਾਂ
            deviceTypes.push_back("ALL");
    }
    ਲਈ (size_t t = 0; t < deviceTypes.size(); t++)
    {
        ਪੂਰਨ deviceType = 0;
        std::string ਆਰਜ਼ੀ_Str_Device_Type = deviceTypes[t];
        std::transform(ਆਰਜ਼ੀ_Str_Device_Type.begin(), ਆਰਜ਼ੀ_Str_Device_Type.end(), ਆਰਜ਼ੀ_Str_Device_Type.begin(), details::char_tolower);

        ਜੇ (ਆਰਜ਼ੀ_Str_Device_Type == "gpu" || ਆਰਜ਼ੀ_Str_Device_Type == "dgpu" || ਆਰਜ਼ੀ_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        ਨਹੀਂਤਾਂ ਜੇ (ਆਰਜ਼ੀ_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        ਨਹੀਂਤਾਂ ਜੇ (ਆਰਜ਼ੀ_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        ਨਹੀਂਤਾਂ ਜੇ (ਆਰਜ਼ੀ_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        ਨਹੀਂਤਾਂ
        {
            CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            ਜਾਓ not_ਮਿਲਿਆ;
        }

        std::vector<cl_device_id> devices;
        ਲਈ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ਗਿਣਤੀ_3 = 0;
            cl_int ਸਥਿਤੀ = clGetDeviceIDs(*currentPlatform, deviceType, 0, ਨਿਰਾ, &ਗਿਣਤੀ_3);
            ਜੇ (!(ਸਥਿਤੀ == CL_SUCCESS || ਸਥਿਤੀ == CL_DEVICE_NOT_ਮਿਲਿਆ))
            {
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetDeviceIDs get count");
            }
            ਜੇ (ਗਿਣਤੀ_3 == 0)
                ਜਾਰੀ;
            size_t ਅਧਾਰ = devices.size();
            devices.resize(ਅਧਾਰ + ਗਿਣਤੀ_3);
            ਸਥਿਤੀ = clGetDeviceIDs(*currentPlatform, deviceType, ਗਿਣਤੀ_3, &devices[ਅਧਾਰ], &ਗਿਣਤੀ_3);
            ਜੇ (!(ਸਥਿਤੀ == CL_SUCCESS || ਸਥਿਤੀ == CL_DEVICE_NOT_ਮਿਲਿਆ))
            {
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetDeviceIDs get IDs");
            }
        }

        ਲਈ (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : ਸੱਚ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = ਸੱਚ;
            ਜੇ(ਆਰਜ਼ੀ_Str_Device_Type == "dgpu" || ਆਰਜ਼ੀ_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_ਸਮ੍ਰਿਤੀ, ਆਕਾਰ(isIGPU), &isIGPU, ਨਿਰਾ));
                useGPU = ਆਰਜ਼ੀ_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            ਜੇ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ਵਾਪਸ devices[i];
            }
        }
    }

not_ਮਿਲਿਆ:
    ਜੇ (configuration.empty())
        ਵਾਪਸ ਨਿਰਾ; // suppress messages on stderr

    std::ostringstream ਸੁਨੇਹਾ;
    ਸੁਨੇਹਾ << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::ਸਤਰ_ਅੰਤ
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::ਸਤਰ_ਅੰਤ
        << "    Device types:";
    ਲਈ (size_t t = 0; t < deviceTypes.size(); t++)
        ਸੁਨੇਹਾ << ' ' << deviceTypes[t];

    ਸੁਨੇਹਾ << std::ਸਤਰ_ਅੰਤ << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ਨਿਰਾ, ਸੁਨੇਹਾ.str());
    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
ਨਾਮਾਕਾਸ਼ svm {

ਗਿਣਤੀ AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_ਬਫਰ = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_ਬਫਰ = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_ਬਫਰ_ਨਕਾਬ = 3 << 16,
        OPENCL_SVM_ਬਫਰ_MAP = 4 << 16
};

ਸਥਿਰ ਤਾਰਕਿਕ checkForceSVMUmatUsage()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ force = ਝੂਠ;
    ਜੇ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ force;
}
ਸਥਿਰ ਤਾਰਕਿਕ checkDisableSVMUMatUsage()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ force = ਝੂਠ;
    ਜੇ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ force;
}
ਸਥਿਰ ਤਾਰਕਿਕ checkDisableSVM()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ force = ਝੂਠ;
    ਜੇ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ force;
}
// see SVMCapabilities
ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ get_SVMCapabilities_ਨਕਾਬ()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਨਕਾਬ = 0;
    ਜੇ (!initialized)
    {
        ਅਚਲ std::string env_ਮੁੱਲ = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        ਜੇ (env_ਮੁੱਲ.empty())
        {
            ਵਾਪਸ ~0U; // all bits 1
        }
        ਨਕਾਬ = atoi(env_ਮੁੱਲ.c_str());
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ ਨਕਾਬ;
}
} // namespace
#ਜੇ_ਅੰਤ

ਸਥਿਰ size_t get_ਪ੍ਰੋਗਰਾਮ_ਗਿਣਤੀ_ਹੱਦਬੰਦੀ()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ size_t ਗਿਣਤੀ_3 = 0;
    ਜੇ (!initialized)
    {
        ਗਿਣਤੀ_3 = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ ਗਿਣਤੀ_3;
}

ਸਥਿਰ ਪੂਰਨ g_contextId = 0;

ਵਰਗ OpenCLBufferPoolImpl;
ਵਰਗ OpenCLSVMBufferPoolImpl;

ਬਣਤਰ Context::Impl
{
    ਸਥਿਰ Context::Impl* get(Context& context) { ਵਾਪਸ context.getImpl(); }

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::deque<Context::Impl*> container_t;
    ਸਥਿਰ container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ਸਥਿਰ container_t* g_contexts = ਨਵਾਂ container_t();
        ਵਾਪਸ *g_contexts;
    }

ਸੁਰੱਖਿਅਤ:
    Impl(ਅਚਲ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        , svmInitialized(ਝੂਠ)
#ਜੇ_ਅੰਤ
    {
        ਜੇ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਸਵੈਚਾਲਿਤ& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ਇਹ;
    }

    ~Impl()
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
        ਜੇ (!cv::__termination)
#ਜੇ_ਅੰਤ
        {
            ਜੇ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ਨਿਰਾ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
            ਸਵੈਚਾਲਿਤ& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ਨਿਰਾ;
        }
    }

    ਖਾਲੀ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_ਅੰਕ_DEVICES, ਆਕਾਰ(ndevices), &ndevices, ਨਿਰਾ));
        CV_Assert(ndevices > 0);

        cv::Auto_ਬਫਰ<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_ਮਾਪ = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * ਆਕਾਰ(cl_device_id), &cl_devices[0], &devices_ret_ਮਾਪ));
        CV_CheckEQ(devices_ret_ਮਾਪ, cl_devices.size() * ਆਕਾਰ(cl_device_id), "");

        devices.clear();
        ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    ਖਾਲੀ __init_buffer_pools();  // w/o synchronization
    ਖਾਲੀ init_ਬਫਰ_pools() ਅਚਲ
    {
        ਜੇ (!ਬਫਰ_Pool_2)
        {
            cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
            ਜੇ (!ਬਫਰ_Pool_2)
            {
                const_cast<Impl*>(ਇਹ)->__init_buffer_pools();
            }
        }
    }
ਜਨਤਕ:
    ਸਥਿਰ Impl* ਲੱਭੋ_Context(ਅਚਲ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਸਵੈਚਾਲਿਤ& container = getGlobalContainer();
        ਜੇ (configuration.empty() && !container.empty())
            ਵਾਪਸ container[0];
        ਲਈ (ਸਵੈਚਾਲਿਤ it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            ਜੇ (i && i->configuration == configuration)
            {
                ਵਾਪਸ i;
            }
        }
        ਵਾਪਸ ਨਿਰਾ;
    }

    ਸਥਿਰ Impl* ਲੱਭੋ_Or_Create_Context(ਅਚਲ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        ਜੇ (configuration_.empty())
        {
            ਅਚਲ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            ਜੇ (!c.empty())
                configuration = c;
        }
        Impl* impl = ਲੱਭੋ_Context(configuration);
        ਜੇ (impl)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ਵਾਪਸ impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        ਜੇ (d == ਨਿਰਾ)
            ਵਾਪਸ ਨਿਰਾ;

        impl = ਨਵਾਂ Impl(configuration);
        ਕੋਸ਼ਿਸ਼
        {
            impl->createFromDevice(d);
            ਜੇ (impl->handle)
                ਵਾਪਸ impl;
            ਮਿਟਾਓ impl;
            ਵਾਪਸ ਨਿਰਾ;
        }
        ਫੜੋ (...)
        {
            ਮਿਟਾਓ impl;
            ਸੁੱਟੋ;
        }
    }

    ਸਥਿਰ Impl* ਲੱਭੋ_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (ਖਾਲੀ*)h);
        Impl* impl = ਲੱਭੋ_Context(configuration);
        ਜੇ (impl)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ਵਾਪਸ impl;
        }

        impl = ਨਵਾਂ Impl(configuration);
        ਕੋਸ਼ਿਸ਼
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ਵਾਪਸ impl;
        }
        ਫੜੋ (...)
        {
            ਮਿਟਾਓ impl;
            ਸੁੱਟੋ;
        }
    }

    ਸਥਿਰ Impl* ਲੱਭੋ_Or_Create_Context(ਅਚਲ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (ਖਾਲੀ*)d);
        Impl* impl = ਲੱਭੋ_Context(configuration);
        ਜੇ (impl)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ਵਾਪਸ impl;
        }

        impl = ਨਵਾਂ Impl(configuration);
        ਕੋਸ਼ਿਸ਼
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ਵਾਪਸ impl;
        }
        ਫੜੋ (...)
        {
            ਮਿਟਾਓ impl;
            ਸੁੱਟੋ;
        }
    }

    ਖਾਲੀ ਸੈੱਟ_Default()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        ਜੇ (d == ਨਿਰਾ)
            ਵਾਪਸ;

        createFromDevice(d);
    }

    ਖਾਲੀ createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ਨਿਰਾ);

        cl_platform_id pl = ਨਿਰਾ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, ਆਕਾਰ(cl_platform_id), &pl, ਨਿਰਾ));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int ਸਥਿਤੀ;

        handle = clCreateContext(prop, nd, &d, 0, 0, &ਸਥਿਤੀ);
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clCreateContext");

        ਤਾਰਕਿਕ ok = handle != 0 && ਸਥਿਤੀ == CL_SUCCESS;
        ਜੇ( ok )
        {
            devices.resize(nd);
            devices[0].ਸੈੱਟ(d);
        }
        ਨਹੀਂਤਾਂ
            handle = ਨਿਰਾ;
    }

    ਪ੍ਰੋਗਰਾਮ_2 getProg(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src, ਅਚਲ String& buildflags, String& errmsg);

    ਖਾਲੀ unloadProg(ਪ੍ਰੋਗਰਾਮ_2& prog)
    {
        cv::Auto_ਤਾਲਾ lock(ਪ੍ਰੋਗਰਾਮ_cache_mutex);
        ਲਈ (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              ਜੇ (it != phash.end())
              {
                  ਜੇ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ਵਾਪਸ;
                  }
              }
        }
    }

    std::string& get_ਅਗੇਤਰ_String()
    {
        ਜੇ (ਅਗੇਤਰ.empty())
        {
            cv::Auto_ਤਾਲਾ lock(ਪ੍ਰੋਗਰਾਮ_cache_mutex);
            ਜੇ (ਅਗੇਤਰ.empty())
            {
                CV_Assert(!devices.empty());
                ਅਚਲ Device& d = devices[0];
                ਪੂਰਨ ਬਿੱਟ_2 = d.ਪਤਾ_ਬਿੱਟ();
                ਜੇ (ਬਿੱਟ_2 > 0 && ਬਿੱਟ_2 != 64)
                    ਅਗੇਤਰ = cv::format("%d-bit--", ਬਿੱਟ_2);
                ਅਗੇਤਰ += d.vendorName() + "--" + d.name() + "--" + d.driver_ਸੰਸਕਰਣ();
                // sanitize chars
                ਲਈ (size_t i = 0; i < ਅਗੇਤਰ.size(); i++)
                {
                    ਅੱਖਰ c = ਅਗੇਤਰ[i];
                    ਜੇ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ਅਗੇਤਰ[i] = '_';
                    }
                }
            }
        }
        ਵਾਪਸ ਅਗੇਤਰ;
    }

    std::string& get_ਅਗੇਤਰ_ਅਧਾਰ()
    {
        ਜੇ (ਅਗੇਤਰ_ਅਧਾਰ.empty())
        {
            cv::Auto_ਤਾਲਾ lock(ਪ੍ਰੋਗਰਾਮ_cache_mutex);
            ਜੇ (ਅਗੇਤਰ_ਅਧਾਰ.empty())
            {
                ਅਚਲ Device& d = devices[0];
                ਪੂਰਨ ਬਿੱਟ_2 = d.ਪਤਾ_ਬਿੱਟ();
                ਜੇ (ਬਿੱਟ_2 > 0 && ਬਿੱਟ_2 != 64)
                    ਅਗੇਤਰ_ਅਧਾਰ = cv::format("%d-bit--", ਬਿੱਟ_2);
                ਅਗੇਤਰ_ਅਧਾਰ += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                ਲਈ (size_t i = 0; i < ਅਗੇਤਰ_ਅਧਾਰ.size(); i++)
                {
                    ਅੱਖਰ c = ਅਗੇਤਰ_ਅਧਾਰ[i];
                    ਜੇ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ਅਗੇਤਰ_ਅਧਾਰ[i] = '_';
                    }
                }
            }
        }
        ਵਾਪਸ ਅਗੇਤਰ_ਅਧਾਰ;
    }

    IMPLEMENT_REFCOUNTABLE();

    ਅਚਲ ਪੂਰਨ contextId;  // global unique ID
    ਅਚਲ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string ਅਗੇਤਰ;
    std::string ਅਗੇਤਰ_ਅਧਾਰ;

    cv::Mutex ਪ੍ਰੋਗਰਾਮ_cache_mutex;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::map<std::string, ਪ੍ਰੋਗਰਾਮ_2> phash_t;
    phash_t phash;
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> ਬਫਰ_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> ਬਫਰ_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_ਬਫਰ_Pool() ਅਚਲ
    {
        init_ਬਫਰ_pools();
        CV_DbgAssert(ਬਫਰ_Pool_2);
        ਵਾਪਸ *ਬਫਰ_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_ਬਫਰ_Pool_Host_Ptr() ਅਚਲ
    {
        init_ਬਫਰ_pools();
        CV_DbgAssert(ਬਫਰ_Pool_Host_Ptr);
        ਵਾਪਸ *ਬਫਰ_Pool_Host_Ptr.get();
    }

    std::map<std::type_ਸੂਚਕਾਂਕ, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    ਖਾਲੀ ਸੈੱਟ_User_Context(std::type_ਸੂਚਕਾਂਕ typeId, ਅਚਲ std::shared_ptr<UserContext>& userContext) {
        cv::Auto_ਤਾਲਾ lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_ਸੂਚਕਾਂਕ typeId) {
        cv::Auto_ਤਾਲਾ lock(userContextMutex);
        ਸਵੈਚਾਲਿਤ it = userContextStorage.find(typeId);
        ਜੇ (it != userContextStorage.end())
            ਵਾਪਸ it->second;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
    ਤਾਰਕਿਕ svmInitialized;
    ਤਾਰਕਿਕ svm_ਉਪਲਬਧ;
    ਤਾਰਕਿਕ svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    ਖਾਲੀ svmInit()
    {
        CV_Assert(handle != ਨਿਰਾ);
        ਅਚਲ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((ਖਾਲੀ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int ਸਥਿਤੀ = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, ਆਕਾਰ(deviceCaps), &deviceCaps, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", ਸਥਿਤੀ);
            ਜਾਓ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (ਪੂਰਨ)deviceCaps);
        CV_Assert(((ਖਾਲੀ)0, CL_DEVICE_SVM_COARSE_GRAIN_ਬਫਰ == CL_DEVICE_SVM_COARSE_GRAIN_ਬਫਰ_AMD)); // Check assumption
        svmCapabilities.ਮੁੱਲ_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_ਬਫਰ) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_ਬਫਰ : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_ਬਫਰ) ? svm::SVMCapabilities::SVM_FINE_GRAIN_ਬਫਰ : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.ਮੁੱਲ_2 &= svm::get_SVMCapabilities_ਨਕਾਬ();
        ਜੇ (svmCapabilities.ਮੁੱਲ_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            ਜਾਓ noSVM;
        }
        ਕੋਸ਼ਿਸ਼
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            ਖਾਲੀ* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            ਜੇ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ਕੋਸ਼ਿਸ਼
            {
                ਤਾਰਕਿਕ error = ਝੂਠ;
                cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
                ਜੇ (CL_SUCCESS != cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ਨਿਰਾ, ਨਿਰਾ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_ਅੰਤ(q);
                ਕੋਸ਼ਿਸ਼
                {
                    ((ਪੂਰਨ*)ptr)[0] = 100;
                }
                ਫੜੋ (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = ਸੱਚ;
                }
                ਜੇ (CL_SUCCESS != cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, ptr, 0, ਨਿਰਾ, ਨਿਰਾ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_ਅੰਤ(q);
                ਜੇ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            ਫੜੋ (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ਸੁੱਟੋ;
            }
            clSVMFree(handle, ptr);
            svmFunctions.fn_clSVMAlloc = clSVMAlloc;
            svmFunctions.fn_clSVMFree = clSVMFree;
            svmFunctions.fn_cl_ਸੈੱਟ_Kernel_Arg_SVMPointer = cl_ਸੈੱਟ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMemcpy = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMemcpy;
            svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMem_Fill = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMem_Fill;
            svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap;
            svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap;
        }
        ਫੜੋ (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ਕੋਸ਼ਿਸ਼
            {
                // Try HSA extension
                String extensions = device.extensions();
                ਜੇ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    ਜਾਓ noSVM;
                }
                cl_platform_id p = ਨਿਰਾ;
                CV_OCL_CHECK(ਸਥਿਤੀ = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, ਆਕਾਰ(cl_platform_id), &p, ਨਿਰਾ));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_ਸੈੱਟ_Kernel_Arg_SVMPointer = (cl_ਸੈੱਟ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMemcpy = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMem_Fill = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap_AMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap_AMD_fn)cl_Get_Extension_Function_ਪਤਾ_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            ਫੜੋ (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                ਜਾਓ noSVM;
            }
        }

        svm_ਉਪਲਬਧ = ਸੱਚ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = ਸੱਚ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ਵਾਪਸ;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_ਉਪਲਬਧ = ਝੂਠ;
        svmEnabled = ਝੂਠ;
        svmCapabilities.ਮੁੱਲ_2 = 0;
        svmInitialized = ਸੱਚ;
        svmFunctions.fn_clSVMAlloc = ਨਿਰਾ;
        ਵਾਪਸ;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> ਬਫਰ_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_ਬਫਰ_Pool_SVM() ਅਚਲ
    {
        init_ਬਫਰ_pools();
        CV_DbgAssert(ਬਫਰ_Pool_SVM);
        ਵਾਪਸ *ਬਫਰ_Pool_SVM.get();
    }
#ਜੇ_ਅੰਤ

    ਮਿੱਤਰ ਵਰਗ ਪ੍ਰੋਗਰਾਮ_2;
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
Context::Context(ਪੂਰਨ dtype)
{
    p = 0;
    create(dtype);
}

ਖਾਲੀ Context::release()
{
    ਜੇ (p)
    {
        p->release();
        p = ਨਿਰਾ;
    }
}

ਤਾਰਕਿਕ Context::create()
{
    release();
    ਜੇ (!haveOpenCL())
        ਵਾਪਸ ਝੂਠ;
    p = Impl::ਲੱਭੋ_Or_Create_Context(std::string());
    ਜੇ (p && p->handle)
        ਵਾਪਸ ਸੱਚ;
    release();
    ਵਾਪਸ ਝੂਠ;
}

// deprecated
ਤਾਰਕਿਕ Context::create(ਪੂਰਨ dtype)
{
    ਜੇ( !haveOpenCL() )
        ਵਾਪਸ ਝੂਠ;
    release();
    ਜੇ (dtype == CL_DEVICE_TYPE_DEFAULT || (ਬਿਨਾਨਿਸ਼ਾਨੀ)dtype == (ਬਿਨਾਨਿਸ਼ਾਨੀ)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::ਲੱਭੋ_Or_Create_Context("");
    }
    ਨਹੀਂਤਾਂ ਜੇ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::ਲੱਭੋ_Or_Create_Context(":GPU:");
    }
    ਨਹੀਂਤਾਂ ਜੇ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::ਲੱਭੋ_Or_Create_Context(":CPU:");
    }
    ਨਹੀਂਤਾਂ
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    ਜੇ (p && !p->handle)
    {
        release();
    }
    ਵਾਪਸ p != 0;
}

Context::Context(ਅਚਲ Context& c)
{
    p = (Impl*)c.p;
    ਜੇ(p)
        p->addref();
}

Context& Context::ਸੰਚਾਲਕ = (ਅਚਲ Context& c)
{
    Impl* newp = (Impl*)c.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = ਖਾਲੀ_ਸੂਚਕ;
}

Context& Context::ਸੰਚਾਲਕ = (Context&& c) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &c) {
        ਜੇ(p)
            p->release();
        p = c.p;
        c.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਖਾਲੀ* Context::ptr() ਅਚਲ
{
    ਵਾਪਸ p == ਨਿਰਾ ? ਨਿਰਾ : p->handle;
}

size_t Context::ndevices() ਅਚਲ
{
    ਵਾਪਸ p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ਅਚਲ
{
    ਸਥਿਰ Device dummy;
    ਵਾਪਸ !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    ਸਵੈਚਾਲਿਤ& c = OpenCLExecutionContext::getCurrent();
    ਜੇ (!c.empty())
    {
        ਸਵੈਚਾਲਿਤ& ctx = c.getContext();
        ਵਾਪਸ ctx;
    }

    ਸਥਿਰ Context dummy;
    ਵਾਪਸ dummy;
}

ਪ੍ਰੋਗਰਾਮ_2 Context::getProg(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog,
                         ਅਚਲ String& buildopts, String& errmsg)
{
    ਵਾਪਸ p ? p->getProg(prog, buildopts, errmsg) : ਪ੍ਰੋਗਰਾਮ_2();
}

ਖਾਲੀ Context::unloadProg(ਪ੍ਰੋਗਰਾਮ_2& prog)
{
    ਜੇ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(ਖਾਲੀ* context)
{
    Context ctx;
    ctx.p = Impl::ਲੱਭੋ_Or_Create_Context((cl_context)context);
    ਵਾਪਸ ctx;
}

/* static */
Context Context::fromDevice(ਅਚਲ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::ਲੱਭੋ_Or_Create_Context(device);
    ਵਾਪਸ ctx;
}

/* static */
Context Context::create(ਅਚਲ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::ਲੱਭੋ_Or_Create_Context(configuration);
    ਵਾਪਸ ctx;
}

ਖਾਲੀ* Context::getOpenCLContextProperty(ਪੂਰਨ propertyId) ਅਚਲ
{
    ਜੇ (p == ਨਿਰਾ)
        ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ਨਿਰਾ, &size));
    std::vector<cl_context_properties> prop(size / ਆਕਾਰ(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.ਅੰਕੜੇ(), ਨਿਰਾ));
    ਲਈ (size_t i = 0; i < prop.size(); i += 2)
    {
        ਜੇ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ਨਿਰਾ, "OpenCL: found context property=" << propertyId << ") => " << (ਖਾਲੀ*)prop[i + 1]);
            ਵਾਪਸ (ਖਾਲੀ*)prop[i + 1];
        }
    }
    ਵਾਪਸ ਖਾਲੀ_ਸੂਚਕ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
ਤਾਰਕਿਕ Context::useSVM() ਅਚਲ
{
    Context::Impl* i = p;
    CV_Assert(i);
    ਜੇ (!i->svmInitialized)
        i->svmInit();
    ਵਾਪਸ i->svmEnabled;
}
ਖਾਲੀ Context::ਸੈੱਟ_Use_SVM(ਤਾਰਕਿਕ enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    ਜੇ (!i->svmInitialized)
        i->svmInit();
    ਜੇ (enabled && !i->svm_ਉਪਲਬਧ)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#ਨਹੀਂਤਾਂ
ਤਾਰਕਿਕ Context::useSVM() ਅਚਲ { ਵਾਪਸ ਝੂਠ; }
ਖਾਲੀ Context::ਸੈੱਟ_Use_SVM(ਤਾਰਕਿਕ enabled) { CV_Assert(!enabled); }
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
ਨਾਮਾਕਾਸ਼ svm {

ਅਚਲ SVMCapabilities getSVMCapabilitites(ਅਚਲ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    ਜੇ (!i->svmInitialized)
        i->svmInit();
    ਵਾਪਸ i->svmCapabilities;
}

CV_EXPORTS ਅਚਲ SVMFunctions* getSVMFunctions(ਅਚਲ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ਨਿਰਾ);
    ਵਾਪਸ &i->svmFunctions;
}

CV_EXPORTS ਤਾਰਕਿਕ useSVM(UMatUsageFlags usageFlags)
{
    ਜੇ (checkForceSVMUmatUsage())
        ਵਾਪਸ ਸੱਚ;
    ਜੇ (checkDisableSVMUMatUsage())
        ਵਾਪਸ ਝੂਠ;
    ਜੇ ((usageFlags & USAGE_ALLOCATE_SHARED_ਸਮ੍ਰਿਤੀ) != 0)
        ਵਾਪਸ ਸੱਚ;
    ਵਾਪਸ ਝੂਠ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#ਜੇ_ਅੰਤ // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

ਖਾਲੀ Context::ਸੈੱਟ_User_Context(std::type_ਸੂਚਕਾਂਕ typeId, ਅਚਲ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->ਸੈੱਟ_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_ਸੂਚਕਾਂਕ typeId)
{
    CV_Assert(p);
    ਵਾਪਸ p->getUserContext(typeId);
}

ਸਥਿਰ ਖਾਲੀ get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_ਬਫਰ<ਅੱਖਰ> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.ਅੰਕੜੇ(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.ਅੰਕੜੇ();
}

/*
// Attaches OpenCL context to OpenCV
*/
ਖਾਲੀ attachContext(ਅਚਲ String& platformName, ਖਾਲੀ* platformID, ਖਾਲੀ* context, ਖਾਲੀ* deviceID)
{
    ਸਵੈਚਾਲਿਤ ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ਅਚਲ std::string& platformName, ਖਾਲੀ* platformID, ਖਾਲੀ* context, ਖਾਲੀ* deviceID
)
{
    ਜੇ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    ਜੇ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    ਤਾਰਕਿਕ platform_ਉਪਲਬਧ = ਝੂਠ;

    // check if external platformName contained in list of available platforms in OpenCV
    ਲਈ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i = 0; i < cnt; i++)
    {
        String ਉਪਲਬਧ_Platform_Name;
        get_platform_name(platforms[i], ਉਪਲਬਧ_Platform_Name);
        // external platform is found in the list of available platforms
        ਜੇ (platformName == ਉਪਲਬਧ_Platform_Name)
        {
            platform_ਉਪਲਬਧ = ਸੱਚ;
            ਰੋਕੋ;
        }
    }

    ਜੇ (!platform_ਉਪਲਬਧ)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    ਜੇ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ਵਾਪਸ ctx;
}

ਖਾਲੀ initializeContextFromHandle(Context& ctx, ਖਾਲੀ* _platform, ਖਾਲੀ* _context, ਖਾਲੀ* _device)
{
    // internal call, less checks
    cl_platform_id platformID = (cl_platform_id)_platform;
    cl_context context = (cl_context)_context;
    cl_device_id deviceID = (cl_device_id)_device;

    std::string platformName = PlatformInfo(&platformID).name();

    ਸਵੈਚਾਲਿਤ clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

ਬਣਤਰ ਕਤਾਰ::Impl
{
    ਸਤਰਅੰਦਰ ਖਾਲੀ __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_ਕਤਾਰ = ਝੂਠ;
    }

    Impl(cl_command_ਕਤਾਰ q)
    {
        __init();
        handle = q;

        cl_command_ਕਤਾਰ_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_ਕਤਾਰ_Info(handle, CL_ਕਤਾਰ_PROPERTIES, ਆਕਾਰ(cl_command_ਕਤਾਰ_properties), &props, ਨਿਰਾ));
        is_Profiling_ਕਤਾਰ = !!(props & CL_ਕਤਾਰ_PROFILING_ENABLE);
    }

    Impl(cl_command_ਕਤਾਰ q, ਤਾਰਕਿਕ is_Profiling_ਕਤਾਰ_2)
    {
        __init();
        handle = q;
        is_Profiling_ਕਤਾਰ = is_Profiling_ਕਤਾਰ_2;
    }

    Impl(ਅਚਲ Context& c, ਅਚਲ Device& d, ਤਾਰਕਿਕ withProfiling = ਝੂਠ)
    {
        __init();

        ਅਚਲ Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        ਜੇ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        ਜੇ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_ਕਤਾਰ_properties props = withProfiling ? CL_ਕਤਾਰ_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_ਕਤਾਰ(ch, dh, props, &retval), retval);
        is_Profiling_ਕਤਾਰ = withProfiling;
    }

    ~Impl()
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
        ਜੇ (!cv::__termination)
#ਜੇ_ਅੰਤ
        {
            ਜੇ(handle)
            {
                CV_OCL_DBG_CHECK(cl_ਅੰਤ(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_ਕਤਾਰ(handle));
                handle = ਨਿਰਾ;
            }
        }
    }

    ਅਚਲ cv::ocl::ਕਤਾਰ& get_Profiling_ਕਤਾਰ(ਅਚਲ cv::ocl::ਕਤਾਰ& self)
    {
        ਜੇ (is_Profiling_ਕਤਾਰ)
            ਵਾਪਸ self;

        ਜੇ (profiling_ਕਤਾਰ.ptr())
            ਵਾਪਸ profiling_ਕਤਾਰ;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_ਕਤਾਰ_Info(handle, CL_ਕਤਾਰ_CONTEXT, ਆਕਾਰ(cl_context), &ctx, ਨਿਰਾ));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_ਕਤਾਰ_Info(handle, CL_ਕਤਾਰ_DEVICE, ਆਕਾਰ(cl_device_id), &device, ਨਿਰਾ));

        cl_int ਨਤੀਜਾ = CL_SUCCESS;
        cl_command_ਕਤਾਰ_properties props = CL_ਕਤਾਰ_PROFILING_ENABLE;
        cl_command_ਕਤਾਰ q = cl_Create_Command_ਕਤਾਰ(ctx, device, props, &ਨਤੀਜਾ);
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਨਤੀਜਾ, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        ਕਤਾਰ ਕਤਾਰ_2;
        ਕਤਾਰ_2.p = ਨਵਾਂ Impl(q, ਸੱਚ);
        profiling_ਕਤਾਰ = ਕਤਾਰ_2;

        ਵਾਪਸ profiling_ਕਤਾਰ;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_ਕਤਾਰ handle;
    ਤਾਰਕਿਕ is_Profiling_ਕਤਾਰ;
    cv::ocl::ਕਤਾਰ profiling_ਕਤਾਰ;
};

ਕਤਾਰ::ਕਤਾਰ() CV_NOEXCEPT
{
    p = 0;
}

ਕਤਾਰ::ਕਤਾਰ(ਅਚਲ Context& c, ਅਚਲ Device& d)
{
    p = 0;
    create(c, d);
}

ਕਤਾਰ::ਕਤਾਰ(ਅਚਲ ਕਤਾਰ& q)
{
    p = q.p;
    ਜੇ(p)
        p->addref();
}

ਕਤਾਰ& ਕਤਾਰ::ਸੰਚਾਲਕ = (ਅਚਲ ਕਤਾਰ& q)
{
    Impl* newp = (Impl*)q.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

ਕਤਾਰ::ਕਤਾਰ(ਕਤਾਰ&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = ਖਾਲੀ_ਸੂਚਕ;
}

ਕਤਾਰ& ਕਤਾਰ::ਸੰਚਾਲਕ = (ਕਤਾਰ&& q) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &q) {
        ਜੇ(p)
            p->release();
        p = q.p;
        q.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਕਤਾਰ::~ਕਤਾਰ()
{
    ਜੇ(p)
        p->release();
}

ਤਾਰਕਿਕ ਕਤਾਰ::create(ਅਚਲ Context& c, ਅਚਲ Device& d)
{
    ਜੇ(p)
        p->release();
    p = ਨਵਾਂ Impl(c, d);
    ਵਾਪਸ p->handle != 0;
}

ਖਾਲੀ ਕਤਾਰ::ਅੰਤ_2()
{
    ਜੇ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_ਅੰਤ(p->handle));
    }
}

ਅਚਲ ਕਤਾਰ& ਕਤਾਰ::get_Profiling_ਕਤਾਰ() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->get_Profiling_ਕਤਾਰ(*ਇਹ);
}

ਖਾਲੀ* ਕਤਾਰ::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

ਕਤਾਰ& ਕਤਾਰ::getDefault()
{
    ਸਵੈਚਾਲਿਤ& c = OpenCLExecutionContext::getCurrent();
    ਜੇ (!c.empty())
    {
        ਸਵੈਚਾਲਿਤ& q = c.get_ਕਤਾਰ();
        ਵਾਪਸ q;
    }
    ਸਥਿਰ ਕਤਾਰ dummy;
    ਵਾਪਸ dummy;
}

ਸਥਿਰ cl_command_ਕਤਾਰ get_ਕਤਾਰ(ਅਚਲ ਕਤਾਰ& q)
{
    cl_command_ਕਤਾਰ qq = (cl_command_ਕਤਾਰ)q.ptr();
    ਜੇ(!qq)
        qq = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਵਾਪਸ qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ਪੂਰਨ _flags, UMat* _m, ਪੂਰਨ _wscale, ਪੂਰਨ _iwscale, ਅਚਲ ਖਾਲੀ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ਨਿਰਾ);
}

KernelArg KernelArg::Constant(ਅਚਲ Mat& m)
{
    CV_Assert(m.isContinuous());
    ਵਾਪਸ KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.ਕੁੱਲ()*m.elem_ਮਾਪ());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ਬਣਤਰ Kernel::Impl
{
    Impl(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog) :
        refcount(1), handle(ਨਿਰਾ), is_ਵਿੱਚ_ਤਰੱਕੀ(ਝੂਠ), is_Async_ਲੜੀਵਾਰ(ਝੂਠ), nu(0)
    {
        cl_ਪ੍ਰੋਗਰਾਮ ph = (cl_ਪ੍ਰੋਗਰਾਮ)prog.ptr();
        cl_int retval = 0;
        name = kname;
        ਜੇ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        ਲਈ( ਪੂਰਨ i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_ਆਰਜ਼ੀ_Dst_UMats = ਝੂਠ;
        have_ਆਰਜ਼ੀ_Src_UMats = ਝੂਠ;
    }

    ਖਾਲੀ cleanupUMats()
    {
        ਤਾਰਕਿਕ exceptionOccurred = ਝੂਠ;
        ਲਈ( ਪੂਰਨ i = 0; i < MAX_ARRS; i++ )
        {
            ਜੇ( u[i] )
            {
                ਜੇ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_ਅੰਕੜੇ::ASYNC_CLEANUP;
                    ਕੋਸ਼ਿਸ਼
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    ਫੜੋ(ਅਚਲ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ਨਿਰਾ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = ਸੱਚ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_ਆਰਜ਼ੀ_Dst_UMats = ਝੂਠ;
        have_ਆਰਜ਼ੀ_Src_UMats = ਝੂਠ;
        CV_Assert(!exceptionOccurred);
    }

    ਖਾਲੀ ਜੋੜੋ_UMat(ਅਚਲ UMat& m, ਤਾਰਕਿਕ dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        ਜੇ(dst && m.u->ਆਰਜ਼ੀ_UMat())
            have_ਆਰਜ਼ੀ_Dst_UMats = ਸੱਚ;
        ਜੇ(m.u->original_UMat_ਅੰਕੜੇ == ਨਿਰਾ && m.u->ਆਰਜ਼ੀ_UMat())
            have_ਆਰਜ਼ੀ_Src_UMats = ਸੱਚ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    ਖਾਲੀ ਰਜਿਸਟਰ_Image_ਦਲੀਲ(ਪੂਰਨ arg, ਅਚਲ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        ਜੇ (arg < (ਪੂਰਨ)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_ਵਿੱਚ_ਤਰੱਕੀ, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    ਖਾਲੀ finit(cl_event e)
    {
        CV_ਅਣਵਰਤਿਆ(e);
        is_ਵਿੱਚ_ਤਰੱਕੀ = ਝੂਠ;
        ਕੋਸ਼ਿਸ਼
        {
            cleanupUMats();
        }
        ਫੜੋ(...)
        {
            release();
            ਸੁੱਟੋ;
        }
        release();
    }

    ਤਾਰਕਿਕ ਲੜੀਵਾਰ(ਪੂਰਨ dims, size_t _globalsize[], size_t _localsize[],
            ਤਾਰਕਿਕ ਸਮਕਾਲੀ, int64* ਸਮਾਂ_NS, ਅਚਲ ਕਤਾਰ& q);

    ~Impl()
    {
        ਜੇ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ਗਿਣਤੀ { MAX_ARRS = 16 };
    UMat_ਅੰਕੜੇ* u[MAX_ARRS];
    ਤਾਰਕਿਕ is_ਵਿੱਚ_ਤਰੱਕੀ;
    ਤਾਰਕਿਕ is_Async_ਲੜੀਵਾਰ;  // true if kernel was scheduled in async mode
    ਪੂਰਨ nu;
    std::vector<Image2D> shadow_images;
    ਤਾਰਕਿਕ have_ਆਰਜ਼ੀ_Dst_UMats;
    ਤਾਰਕਿਕ have_ਆਰਜ਼ੀ_Src_UMats;
};

}} // namespace cv::ocl

ਬਾਹਰੀ "C" {

ਸਥਿਰ ਖਾਲੀ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ਖਾਲੀ *p)
{
    ਕੋਸ਼ਿਸ਼
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    ਫੜੋ (ਅਚਲ cv::Exception& exc)
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    ਫੜੋ (ਅਚਲ std::exception& exc)
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    ਫੜੋ (...)
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
               ਅਚਲ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ਅਚਲ Kernel& k)
{
    p = k.p;
    ਜੇ(p)
        p->addref();
}

Kernel& Kernel::ਸੰਚਾਲਕ = (ਅਚਲ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = ਖਾਲੀ_ਸੂਚਕ;
}

Kernel& Kernel::ਸੰਚਾਲਕ = (Kernel&& k) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &k) {
        ਜੇ(p)
            p->release();
        p = k.p;
        k.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

Kernel::~Kernel()
{
    ਜੇ(p)
        p->release();
}

ਤਾਰਕਿਕ Kernel::create(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog)
{
    ਜੇ(p)
        p->release();
    p = ਨਵਾਂ Impl(kname, prog);
    ਜੇ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_OPENCL_ਲੜੀਵਾਰ_ASSERT // check kernel compilation fails
    CV_Assert(p);
#ਜੇ_ਅੰਤ
    ਵਾਪਸ p != 0;
}

ਤਾਰਕਿਕ Kernel::create(ਅਚਲ ਅੱਖਰ* kname, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
                    ਅਚਲ String& buildopts, String* errmsg)
{
    ਜੇ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    ਜੇ( !errmsg ) errmsg = &tempmsg;
    ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ਵਾਪਸ create(kname, prog);
}

ਖਾਲੀ* Kernel::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

ਤਾਰਕਿਕ Kernel::empty() ਅਚਲ
{
    ਵਾਪਸ ptr() == 0;
}

ਸਥਿਰ cv::String dump_ਮੁੱਲ(size_t sz, ਅਚਲ ਖਾਲੀ* p)
{
    ਜੇ (!p)
        ਵਾਪਸ "NULL";
    ਜੇ (sz == 2)
        ਵਾਪਸ cv::format("%d / %uu / 0x%04x", *(ਛੋਟਾ*)p, *(ਬਿਨਾਨਿਸ਼ਾਨੀ ਛੋਟਾ*)p, *(ਛੋਟਾ*)p);
    ਜੇ (sz == 4)
        ਵਾਪਸ cv::format("%d / %uu / 0x%08x / %g", *(ਪੂਰਨ*)p, *(ਪੂਰਨ*)p, *(ਪੂਰਨ*)p, *(ਦਸ਼ਮਲਵ*)p);
    ਜੇ (sz == 8)
        ਵਾਪਸ cv::format("%lld / %lluu / 0x%16llx / %g", *(ਲੰਮਾ ਲੰਮਾ*)p, *(ਲੰਮਾ ਲੰਮਾ*)p, *(ਲੰਮਾ ਲੰਮਾ*)p, *(ਦੁੱਗਣਾ*)p);
    ਵਾਪਸ cv::format("%p", p);
}

ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ ਖਾਲੀ* value, size_t sz)
{
    ਜੇ (!p || !p->handle)
        ਵਾਪਸ -1;
    ਜੇ (i < 0)
        ਵਾਪਸ i;
    ਜੇ( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (ਪੂਰਨ)i, (ਪੂਰਨ)sz, dump_ਮੁੱਲ(sz, value).c_str()).c_str());
    ਜੇ (retval != CL_SUCCESS)
        ਵਾਪਸ -1;
    ਵਾਪਸ i+1;
}

ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    ਪੂਰਨ res = ਸੈੱਟ(i, &h, ਆਕਾਰ(h));
    ਜੇ (res >= 0)
        p->ਰਜਿਸਟਰ_Image_ਦਲੀਲ(i, image2D);
    ਵਾਪਸ res;
}

ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ UMat& m)
{
    ਵਾਪਸ ਸੈੱਟ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ਸਥਿਰ ਤਾਰਕਿਕ isCudaBacked(ਅਚਲ UMat_ਅੰਕੜੇ* u)
{
    ਅਚਲ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ਵਾਪਸ u && cudaAllocator && u->currAllocator == cudaAllocator;
}

ਪੂਰਨ Kernel::ਸੈੱਟ(ਪੂਰਨ i, ਅਚਲ KernelArg& arg)
{
    ਜੇ( !p || !p->handle )
        ਵਾਪਸ -1;
    ਜੇ (i < 0)
    {
        CV_LOG_ERROR(ਨਿਰਾ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (ਪੂਰਨ)i));
        ਵਾਪਸ i;
    }
    ਜੇ( i == 0 )
        p->cleanupUMats();
    cl_int ਸਥਿਤੀ = 0;
    ਜੇ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        ਤਾਰਕਿਕ ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        ਜੇ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ਨਿਰਾ;
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, ਆਕਾਰ(h_null), &h_null);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (ਪੂਰਨ)i).c_str());
            ਵਾਪਸ i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        ਜੇ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ਨਿਰਾ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (ਪੂਰਨ)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ਵਾਪਸ -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        ਜੇ (!h)
        {
            CV_LOG_ERROR(ਨਿਰਾ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (ਪੂਰਨ)i, (ਪੂਰਨ)arg.flags, arg.m));
            p->release();
            p = 0;
            ਵਾਪਸ -1;
        }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
        {
            ਅਚਲ Context& ctx = Context::getDefault();
            ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_ਅੰਕੜੇ_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_ਅੰਕੜੇ_Ptr);
#ਜੇ 1 // TODO
            ਸਥਿਤੀ = svmFns->fn_cl_ਸੈੱਟ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_ਅੰਕੜੇ_Ptr);
#ਨਹੀਂਤਾਂ
            ਸਥਿਤੀ = svmFns->fn_cl_ਸੈੱਟ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_ਅੰਕੜੇ_Ptr);
#ਜੇ_ਅੰਤ
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (ਪੂਰਨ)i, (ਖਾਲੀ*)svm_ਅੰਕੜੇ_Ptr).c_str());
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, ਆਕਾਰ(h), &h);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (ਪੂਰਨ)i, (ਖਾਲੀ*)h).c_str());
        }

        ਜੇ (ptronly)
        {
            i++;
        }
        ਨਹੀਂਤਾਂ ਜੇ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+1), ਆਕਾਰ(u2d.ਕਦਮ), &u2d.ਕਦਮ);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+1), (ਪੂਰਨ)u2d.ਕਦਮ).c_str());
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+2), ਆਕਾਰ(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+2), (ਪੂਰਨ)u2d.offset).c_str());
            i += 3;

            ਜੇ( !(arg.flags & KernelArg::NO_ਮਾਪ) )
            {
                ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = u2d.ਖਾਨੇ_ਗਿਣਤੀ*arg.wscale/arg.iwscale;
                ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, ਆਕਾਰ(u2d.ਪੰਕਤੀਆਂ), &u2d.ਪੰਕਤੀਆਂ);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ਪੂਰਨ)i, (ਪੂਰਨ)u2d.ਪੰਕਤੀਆਂ).c_str());
                ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+1), ਆਕਾਰ(ਖਾਨੇ_ਗਿਣਤੀ), &ਖਾਨੇ_ਗਿਣਤੀ);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+1), (ਪੂਰਨ)ਖਾਨੇ_ਗਿਣਤੀ).c_str());
                i += 2;
            }
        }
        ਨਹੀਂਤਾਂ
        {
            UMat3D u3d(*arg.m);
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+1), ਆਕਾਰ(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+1), (ਪੂਰਨ)u3d.slicestep).c_str());
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+2), ਆਕਾਰ(u3d.ਕਦਮ), &u3d.ਕਦਮ);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+2), (ਪੂਰਨ)u3d.ਕਦਮ).c_str());
            ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+3), ਆਕਾਰ(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+3), (ਪੂਰਨ)u3d.offset).c_str());
            i += 4;
            ਜੇ( !(arg.flags & KernelArg::NO_ਮਾਪ) )
            {
                ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ = u3d.ਖਾਨੇ_ਗਿਣਤੀ*arg.wscale/arg.iwscale;
                ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, ਆਕਾਰ(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (ਪੂਰਨ)i, (ਪੂਰਨ)u3d.slices).c_str());
                ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+1), ਆਕਾਰ(u3d.ਪੰਕਤੀਆਂ), &u3d.ਪੰਕਤੀਆਂ);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+1), (ਪੂਰਨ)u3d.ਪੰਕਤੀਆਂ).c_str());
                ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)(i+2), ਆਕਾਰ(u3d.ਖਾਨੇ_ਗਿਣਤੀ), &ਖਾਨੇ_ਗਿਣਤੀ);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ਪੂਰਨ)(i+2), (ਪੂਰਨ)ਖਾਨੇ_ਗਿਣਤੀ).c_str());
                i += 3;
            }
        }
        p->ਜੋੜੋ_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ਵਾਪਸ i;
    }
    ਸਥਿਤੀ = cl_ਸੈੱਟ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (ਪੂਰਨ)i, (ਪੂਰਨ)arg.sz, (ਖਾਲੀ*)arg.obj).c_str());
    ਵਾਪਸ i+1;
}

ਤਾਰਕਿਕ Kernel::ਲੜੀਵਾਰ(ਪੂਰਨ dims, size_t _globalsize[], size_t _localsize[],
                 ਤਾਰਕਿਕ ਸਮਕਾਲੀ, ਅਚਲ ਕਤਾਰ& q)
{
    ਜੇ (!p)
        ਵਾਪਸ ਝੂਠ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t ਕੁੱਲ = 1;
    CV_Assert(_globalsize != ਨਿਰਾ);
    ਲਈ (ਪੂਰਨ i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(ਪੂਰਨ)(i>0)) : 1;
        CV_Assert( val > 0 );
        ਕੁੱਲ *= _globalsize[i];
        ਜੇ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_ਉੱਪਰ(_globalsize[i], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)val) * val;
    }
    CV_Assert(ਕੁੱਲ > 0);

    ਵਾਪਸ p->ਲੜੀਵਾਰ(dims, globalsize, _localsize, ਸਮਕਾਲੀ, ਨਿਰਾ, q);
}


ਤਾਰਕਿਕ Kernel::ਲੜੀਵਾਰ_2(ਪੂਰਨ dims, size_t _globalsize[], size_t _localsize[],
                  ਤਾਰਕਿਕ ਸਮਕਾਲੀ, ਅਚਲ ਕਤਾਰ& q)
{
    CV_Assert(p);
    ਵਾਪਸ p->ਲੜੀਵਾਰ(dims, _globalsize, _localsize, ਸਮਕਾਲੀ, ਨਿਰਾ, q);
}


ਤਾਰਕਿਕ Kernel::Impl::ਲੜੀਵਾਰ(ਪੂਰਨ dims, size_t globalsize[], size_t localsize[],
        ਤਾਰਕਿਕ ਸਮਕਾਲੀ, int64* ਸਮਾਂ_NS, ਅਚਲ ਕਤਾਰ& q)
{
    CV_INSTRUMENT_REGION_OPENCL_ਲੜੀਵਾਰ(name.c_str());

    ਜੇ (!handle)
    {
        CV_LOG_ERROR(ਨਿਰਾ, "OpenCL kernel has zero handle: " << name);
        ਵਾਪਸ ਝੂਠ;
    }

    ਜੇ (is_Async_ਲੜੀਵਾਰ)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_ਲੜੀਵਾਰ = !ਸਮਕਾਲੀ;

    ਜੇ (is_ਵਿੱਚ_ਤਰੱਕੀ)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#ਜੇ CV_OPENCL_SYNC_ਲੜੀਵਾਰ_KERNELS
    ਸਮਕਾਲੀ = ਸੱਚ;
#ਜੇ_ਅੰਤ

    cl_command_ਕਤਾਰ qq = get_ਕਤਾਰ(q);
    ਜੇ (have_ਆਰਜ਼ੀ_Dst_UMats)
        ਸਮਕਾਲੀ = ਸੱਚ;
    ਜੇ (have_ਆਰਜ਼ੀ_Src_UMats)
        ਸਮਕਾਲੀ = ਸੱਚ;
    ਜੇ (ਸਮਾਂ_NS)
        ਸਮਕਾਲੀ = ਸੱਚ;
    cl_event asyncEvent = 0;
    cl_int retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           ਨਿਰਾ, globalsize, localsize, 0, 0,
                                           (ਸਮਕਾਲੀ && !ਸਮਾਂ_NS) ? 0 : &asyncEvent);
#ਜੇ !CV_OPENCL_SHOW_ਲੜੀਵਾਰ_KERNELS
    ਜੇ (retval != CL_SUCCESS)
#ਜੇ_ਅੰਤ
    {
        cv::String ਸੁਨੇਹਾ = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (ਪੂਰਨ)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ਸਮਕਾਲੀ ? "true" : "false"
                        );
        ਜੇ (retval != CL_SUCCESS)
        {
            ਸੁਨੇਹਾ = CV_OCL_API_ERROR_ਸੁਨੇਹਾ(retval, ਸੁਨੇਹਾ.c_str());
        }
#ਜੇ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(retval, ਸੁਨੇਹਾ.c_str());
#ਨਹੀਂਤਾਂ
        ਲਿਖੋ("%s\n", ਸੁਨੇਹਾ.c_str());
        fflush(stdout);
#ਜੇ_ਅੰਤ
    }
    ਜੇ (ਸਮਕਾਲੀ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ਅੰਤ(qq));
        ਜੇ (ਸਮਾਂ_NS)
        {
            ਜੇ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong ਸ਼ੁਰੂ_ਸਮਾਂ, stop_ਸਮਾਂ;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_ਸ਼ੁਰੂ, ਆਕਾਰ(ਸ਼ੁਰੂ_ਸਮਾਂ), &ਸ਼ੁਰੂ_ਸਮਾਂ, ਨਿਰਾ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_ਸਿਰਾ, ਆਕਾਰ(stop_ਸਮਾਂ), &stop_ਸਮਾਂ, ਨਿਰਾ));
                *ਸਮਾਂ_NS = (int64)(stop_ਸਮਾਂ - ਸ਼ੁਰੂ_ਸਮਾਂ);
            }
            ਨਹੀਂਤਾਂ
            {
                *ਸਮਾਂ_NS = -1;
            }
        }
        cleanupUMats();
    }
    ਨਹੀਂਤਾਂ
    {
        addref();
        is_ਵਿੱਚ_ਤਰੱਕੀ = ਸੱਚ;
        CV_OCL_CHECK(cl_ਸੈੱਟ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ਇਹ));
    }
    ਜੇ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ਵਾਪਸ retval == CL_SUCCESS;
}

ਤਾਰਕਿਕ Kernel::ਲੜੀਵਾਰ_Task(ਤਾਰਕਿਕ ਸਮਕਾਲੀ, ਅਚਲ ਕਤਾਰ& q)
{
    ਜੇ(!p || !p->handle || p->is_ਵਿੱਚ_ਤਰੱਕੀ)
        ਵਾਪਸ ਝੂਠ;

    cl_command_ਕਤਾਰ qq = get_ਕਤਾਰ(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Task(qq, p->handle, 0, 0, ਸਮਕਾਲੀ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ਸਮਕਾਲੀ ? "true" : "false").c_str());
    ਜੇ (ਸਮਕਾਲੀ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ਅੰਤ(qq));
        p->cleanupUMats();
    }
    ਨਹੀਂਤਾਂ
    {
        p->addref();
        p->is_ਵਿੱਚ_ਤਰੱਕੀ = ਸੱਚ;
        CV_OCL_CHECK(cl_ਸੈੱਟ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    ਜੇ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ਵਾਪਸ retval == CL_SUCCESS;
}

int64 Kernel::ਲੜੀਵਾਰ_Profiling(ਪੂਰਨ dims, size_t globalsize[], size_t localsize[], ਅਚਲ ਕਤਾਰ& q_)
{
    CV_Assert(p && p->handle && !p->is_ਵਿੱਚ_ਤਰੱਕੀ);
    ਕਤਾਰ q = q_.ptr() ? q_ : ਕਤਾਰ::getDefault();
    CV_Assert(q.ptr());
    q.ਅੰਤ_2(); // call clFinish() on base queue
    ਕਤਾਰ profiling_ਕਤਾਰ_2 = q.get_Profiling_ਕਤਾਰ();
    int64 ਸਮਾਂ_Ns = -1;
    ਤਾਰਕਿਕ res = p->ਲੜੀਵਾਰ(dims, globalsize, localsize, ਸੱਚ, &ਸਮਾਂ_Ns, profiling_ਕਤਾਰ_2);
    ਵਾਪਸ res ? ਸਮਾਂ_Ns : -1;
}

size_t Kernel::work_ਸਮੂਹ_ਮਾਪ() ਅਚਲ
{
    ਜੇ(!p || !p->handle)
        ਵਾਪਸ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ਸਥਿਤੀ = cl_Get_Kernel_Work_ਸਮੂਹ_Info(p->handle, dev, CL_KERNEL_WORK_ਸਮੂਹ_ਮਾਪ, ਆਕਾਰ(val), &val, &retsz);
    CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ਵਾਪਸ ਸਥਿਤੀ == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_ਸਮੂਹ_ਮਾਪ_ਗੁਣਜ() ਅਚਲ
{
    ਜੇ(!p || !p->handle)
        ਵਾਪਸ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ਸਥਿਤੀ = cl_Get_Kernel_Work_ਸਮੂਹ_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_ਸਮੂਹ_ਮਾਪ_ਗੁਣਜ, ਆਕਾਰ(val), &val, &retsz);
    CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ਵਾਪਸ ਸਥਿਤੀ == CL_SUCCESS ? val : 0;
}

ਤਾਰਕਿਕ Kernel::compile_Work_ਸਮੂਹ_ਮਾਪ(size_t wsz[]) ਅਚਲ
{
    ਜੇ(!p || !p->handle || !wsz)
        ਵਾਪਸ 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ਸਥਿਤੀ = cl_Get_Kernel_Work_ਸਮੂਹ_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_ਸਮੂਹ_ਮਾਪ, ਆਕਾਰ(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ਵਾਪਸ ਸਥਿਤੀ == CL_SUCCESS;
}

size_t Kernel::local_Mem_ਮਾਪ() ਅਚਲ
{
    ਜੇ(!p || !p->handle)
        ਵਾਪਸ 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int ਸਥਿਤੀ = cl_Get_Kernel_Work_ਸਮੂਹ_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_ਮਾਪ, ਆਕਾਰ(val), &val, &retsz);
    CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ਵਾਪਸ ਸਥਿਤੀ == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ਬਣਤਰ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ਗਿਣਤੀ ਕਿਸਮ_4 {
        ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ = 0,
        ਪ੍ਰੋਗਰਾਮ_BINARIES,
        ਪ੍ਰੋਗਰਾਮ_SPIR,
        ਪ੍ਰੋਗਰਾਮ_SPIRV
    } ਕਿਸਮ_5;

    Impl(ਅਚਲ String& src)
    {
        init(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ, cv::String(), cv::String());
        init_From_ਸਰੋਤ(src, cv::String());
    }
    Impl(ਅਚਲ String& module, ਅਚਲ String& name, ਅਚਲ String& ਕੋਡ_Str, ਅਚਲ String& ਕੋਡ_Hash)
    {
        init(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ, module, name);
        init_From_ਸਰੋਤ(ਕੋਡ_Str, ਕੋਡ_Hash);
    }

    /// reset fields
    ਖਾਲੀ init(ਗਿਣਤੀ ਕਿਸਮ_4 ਕਿਸਮ, ਅਚਲ String& module, ਅਚਲ String& name)
    {
        refcount = 1;
        ਕਿਸਮ_5 = ਕਿਸਮ;
        module_ = module;
        name_ = name;

        ਸਰੋਤ_Addr = ਨਿਰਾ;
        ਸਰੋਤ_ਮਾਪ = 0;
        isHashUpdated = ਝੂਠ;
    }

    ਖਾਲੀ init_From_ਸਰੋਤ(ਅਚਲ String& ਕੋਡ_Str, ਅਚਲ String& ਕੋਡ_Hash)
    {
        ਕੋਡ_Str_2 = ਕੋਡ_Str;
        ਸਰੋਤ_Hash = ਕੋਡ_Hash;
        ਜੇ (ਸਰੋਤ_Hash.empty())
        {
            updateHash();
        }
        ਨਹੀਂਤਾਂ
        {
            isHashUpdated = ਸੱਚ;
        }
    }

    ਖਾਲੀ updateHash(ਅਚਲ ਅੱਖਰ* hashStr = ਨਿਰਾ)
    {
        ਜੇ (hashStr)
        {
            ਸਰੋਤ_Hash = cv::String(hashStr);
            isHashUpdated = ਸੱਚ;
            ਵਾਪਸ;
        }
        uint64 hash = 0;
        ਚੋਣ (ਕਿਸਮ_5)
        {
        ਹਾਲਤ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ:
            ਜੇ (ਸਰੋਤ_Addr)
            {
                CV_Assert(ਕੋਡ_Str_2.empty());
                hash = crc64(ਸਰੋਤ_Addr, ਸਰੋਤ_ਮਾਪ); // static storage
            }
            ਨਹੀਂਤਾਂ
            {
                CV_Assert(!ਕੋਡ_Str_2.empty());
                hash = crc64((uchar*)ਕੋਡ_Str_2.c_str(), ਕੋਡ_Str_2.size());
            }
            ਰੋਕੋ;
        ਹਾਲਤ ਪ੍ਰੋਗਰਾਮ_BINARIES:
        ਹਾਲਤ ਪ੍ਰੋਗਰਾਮ_SPIR:
        ਹਾਲਤ ਪ੍ਰੋਗਰਾਮ_SPIRV:
            hash = crc64(ਸਰੋਤ_Addr, ਸਰੋਤ_ਮਾਪ);
            ਰੋਕੋ;
        ਮੂਲ:
            CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "Internal error");
        }
        ਸਰੋਤ_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = ਸੱਚ;
    }

    Impl(ਗਿਣਤੀ ਕਿਸਮ_4 ਕਿਸਮ,
            ਅਚਲ String& module, ਅਚਲ String& name,
            ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size,
            ਅਚਲ cv::String& buildOptions = cv::String())
    {
        init(ਕਿਸਮ, module, name);

        ਸਰੋਤ_Addr = binary;
        ਸਰੋਤ_ਮਾਪ = size;

        buildOptions_ = buildOptions;
    }

    ਸਥਿਰ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ from_ਸਰੋਤ_With_Static_Lifetime(ਅਚਲ String& module, ਅਚਲ String& name,
            ਅਚਲ ਅੱਖਰ* ਸਰੋਤ_ਕੋਡ_Static_Str, ਅਚਲ ਅੱਖਰ* hashStaticStr,
            ਅਚਲ cv::String& buildOptions)
    {
        ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਨਤੀਜਾ;
        ਨਤੀਜਾ.p = ਨਵਾਂ Impl(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ, module, name,
                (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)ਸਰੋਤ_ਕੋਡ_Static_Str, ਲੜੀ_ਲੰਬਾਈ(ਸਰੋਤ_ਕੋਡ_Static_Str), buildOptions);
        ਨਤੀਜਾ.p->updateHash(hashStaticStr);
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਸਥਿਰ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ from_ਦੁਆਧਾਰੀ(ਅਚਲ String& module, ਅਚਲ String& name,
            ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size,
            ਅਚਲ cv::String& buildOptions)
    {
        ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਨਤੀਜਾ;
        ਨਤੀਜਾ.p = ਨਵਾਂ Impl(ਪ੍ਰੋਗਰਾਮ_BINARIES, module, name, binary, size, buildOptions);
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਸਥਿਰ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ fromSPIR(ਅਚਲ String& module, ਅਚਲ String& name,
            ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size,
            ਅਚਲ cv::String& buildOptions)
    {
        ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਨਤੀਜਾ;
        ਨਤੀਜਾ.p = ਨਵਾਂ Impl(ਪ੍ਰੋਗਰਾਮ_SPIR, module, name, binary, size, buildOptions);
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String ਕੋਡ_Str_2; // PROGRAM_SOURCE_CODE only

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਸਰੋਤ_Addr;
    size_t ਸਰੋਤ_ਮਾਪ;

    cv::String buildOptions_;

    String ਸਰੋਤ_Hash;
    ਤਾਰਕਿਕ isHashUpdated;

    ਮਿੱਤਰ ਬਣਤਰ ਪ੍ਰੋਗਰਾਮ_2::Impl;
    ਮਿੱਤਰ ਬਣਤਰ internal::ਪ੍ਰੋਗਰਾਮ_ਇੰਦਰਾਜ;
    ਮਿੱਤਰ ਬਣਤਰ Context::Impl;
};


ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ() CV_NOEXCEPT
{
    p = 0;
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ String& module, ਅਚਲ String& name, ਅਚਲ String& ਕੋਡ_Str, ਅਚਲ String& ਕੋਡ_Hash)
{
    p = ਨਵਾਂ Impl(module, name, ਕੋਡ_Str, ਕੋਡ_Hash);
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ ਅੱਖਰ* prog)
{
    p = ਨਵਾਂ Impl(prog);
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ String& prog)
{
    p = ਨਵਾਂ Impl(prog);
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::~ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ()
{
    ਜੇ(p)
        p->release();
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog)
{
    p = prog.p;
    ਜੇ(p)
        p->addref();
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸੰਚਾਲਕ = (ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& prog)
{
    Impl* newp = (Impl*)prog.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = ਖਾਲੀ_ਸੂਚਕ;
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸੰਚਾਲਕ = (ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&& prog) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &prog) {
        ਜੇ(p)
            p->release();
        p = prog.p;
        prog.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਅਚਲ String& ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::ਸਰੋਤ() ਅਚਲ
{
    CV_Assert(p);
    CV_Assert(p->ਕਿਸਮ_5 == Impl::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ);
    CV_Assert(p->ਸਰੋਤ_Addr == ਨਿਰਾ); // method returns reference - can't construct temporary object
    ਵਾਪਸ p->ਕੋਡ_Str_2;
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::hash_t ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::hash() ਅਚਲ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::from_ਦੁਆਧਾਰੀ(ਅਚਲ String& module, ਅਚਲ String& name,
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size,
        ਅਚਲ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ਵਾਪਸ Impl::from_ਦੁਆਧਾਰੀ(module, name, binary, size, buildOptions);
}

ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::fromSPIR(ਅਚਲ String& module, ਅਚਲ String& name,
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* binary, ਅਚਲ size_t size,
        ਅਚਲ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ਵਾਪਸ Impl::from_ਦੁਆਧਾਰੀ(module, name, binary, size, buildOptions);
}


internal::ਪ੍ਰੋਗਰਾਮ_ਇੰਦਰਾਜ::ਸੰਚਾਲਕ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ&() ਅਚਲ
{
    ਜੇ (ਇਹ->p_ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ == ਨਿਰਾ)
    {
        cv::Auto_ਤਾਲਾ lock(cv::getInitializationMutex());
        ਜੇ (ਇਹ->p_ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ == ਨਿਰਾ)
        {
            ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ ps = ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::from_ਸਰੋਤ_With_Static_Lifetime(ਇਹ->module, ਇਹ->name, ਇਹ->ਪ੍ਰੋਗਰਾਮ_ਕੋਡ, ਇਹ->ਪ੍ਰੋਗਰਾਮ_Hash, cv::String());
            ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ* ptr = ਨਵਾਂ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ(ps);
            const_cast<ਪ੍ਰੋਗਰਾਮ_ਇੰਦਰਾਜ*>(ਇਹ)->p_ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ = ptr;
        }
    }
    ਵਾਪਸ *ਇਹ->p_ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ਸਥਿਰ
cv::String joinBuildOptions(ਅਚਲ cv::String& a, ਅਚਲ cv::String& b)
{
    ਜੇ (b.empty())
        ਵਾਪਸ a;
    ਜੇ (a.empty())
        ਵਾਪਸ b;
    ਜੇ (b[0] == ' ')
        ਵਾਪਸ a + b;
    ਵਾਪਸ a + (cv::String(" ") + b);
}

ਬਣਤਰ ਪ੍ਰੋਗਰਾਮ_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
         ਅਚਲ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ਨਿਰਾ),
         buildflags(_buildflags)
    {
        ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        ਸਰੋਤ_Module = src_->module_;
        ਸਰੋਤ_Name = src_->name_;
        ਅਚਲ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        ਜੇ (ctx.ptr() == ਨਿਰਾ || device.ptr() == ਨਿਰਾ)
            ਵਾਪਸ;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        ਜੇ (src.getImpl()->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ)
        {
            ਜੇ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            ਨਹੀਂਤਾਂ ਜੇ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ਅਚਲ String param_buildExtraOptions = getBuildExtraOptions();
            ਜੇ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#ਜੇ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ਨਿਰਾ, "OpenCL program '" << ਸਰੋਤ_Module << "/" << ਸਰੋਤ_Name << "' options:" << buildflags);
#ਜੇ_ਅੰਤ
        compile(ctx, src_, errmsg);
#ਜੇ CV_OPENCL_SHOW_BUILD_KERNELS
        ਜੇ (handle)
        {
            size_t retsz = 0;
            ਅੱਖਰ kernels_ਬਫਰ[4096] = {0};
            cl_int ਨਤੀਜਾ = cl_Get_ਪ੍ਰੋਗਰਾਮ_Info(handle, CL_ਪ੍ਰੋਗਰਾਮ_KERNEL_ਨਾਂ, ਆਕਾਰ(kernels_ਬਫਰ), &kernels_ਬਫਰ[0], &retsz);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਨਤੀਜਾ, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", ਸਰੋਤ_Module.c_str(), ਸਰੋਤ_Name.c_str()).c_str());
            ਜੇ (ਨਤੀਜਾ == CL_SUCCESS && retsz < ਆਕਾਰ(kernels_ਬਫਰ))
            {
                kernels_ਬਫਰ[retsz] = 0;
                CV_LOG_INFO(ਨਿਰਾ, "OpenCL program '" << ਸਰੋਤ_Module << "/" << ਸਰੋਤ_Name << "' kernels: '" << kernels_ਬਫਰ << "'");
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_ERROR(ਨਿਰਾ, "OpenCL program '" << ਸਰੋਤ_Module << "/" << ਸਰੋਤ_Name << "' can't retrieve kernel names!");
            }
        }
#ਜੇ_ਅੰਤ
    }

    ਤਾਰਕਿਕ compile(ਅਚਲ Context& ctx, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        ਜੇ (src_->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_BINARIES)
        {
            CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ਤਾਰਕਿਕ isLoaded = create_From_ਦੁਆਧਾਰੀ(ctx, src_->ਸਰੋਤ_Addr, src_->ਸਰੋਤ_ਮਾਪ, errmsg);
            ਵਾਪਸ isLoaded;
        }
        ਵਾਪਸ compileWithCache(ctx, src_, errmsg);
    }

    ਤਾਰਕਿਕ compileWithCache(ਅਚਲ Context& ctx, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->ਕਿਸਮ_5 != ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_BINARIES);

#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ਅਚਲ std::string ਅਧਾਰ_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_ਅਗੇਤਰ_String(),
                ctx.getImpl()->get_ਅਗੇਤਰ_ਅਧਾਰ()
        );
        ਅਚਲ String& hash_str = src_->ਸਰੋਤ_Hash;
        cv::String fname;
        ਜੇ (!ਅਧਾਰ_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(ਅਧਾਰ_dir, fname);
        }
        ਅਚਲ cv::Ptr<utils::fs::File_ਤਾਲਾ> file_ਤਾਲਾ = config.cache_ਤਾਲਾ; // can be empty
        ਜੇ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ਕੋਸ਼ਿਸ਼
            {
                std::vector<ਅੱਖਰ> ਦੁਆਧਾਰੀ_Buf;
                ਤਾਰਕਿਕ res = ਝੂਠ;
                {
                    cv::utils::optional_shared_ਤਾਲਾ_guard<cv::utils::fs::File_ਤਾਲਾ> ਤਾਲਾ_fs(file_ਤਾਲਾ.get());
                    ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ_File file(fname, hash_str.c_str());
                    res = file.ਪਠ(buildflags, ਦੁਆਧਾਰੀ_Buf);
                }
                ਜੇ (res)
                {
                    CV_Assert(!ਦੁਆਧਾਰੀ_Buf.empty());
                    CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    ਤਾਰਕਿਕ isLoaded = create_From_ਦੁਆਧਾਰੀ(ctx, ਦੁਆਧਾਰੀ_Buf, errmsg);
                    ਜੇ (isLoaded)
                        ਵਾਪਸ ਸੱਚ;
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                CV_ਅਣਵਰਤਿਆ(e);
                CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Can't load OpenCL binary: " + fname << std::ਸਤਰ_ਅੰਤ << e.what());
            }
            ਫੜੋ (...)
            {
                CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ਨਿਰਾ);
        ਜੇ (src_->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ)
        {
            ਜੇ (!buildFromSources(ctx, src_, errmsg))
            {
                ਵਾਪਸ ਝੂਠ;
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ (src_->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            ਜੇ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ਤਾਰਕਿਕ isLoaded = create_From_ਦੁਆਧਾਰੀ(ctx, src_->ਸਰੋਤ_Addr, src_->ਸਰੋਤ_ਮਾਪ, errmsg);
            ਜੇ (!isLoaded)
                ਵਾਪਸ ਝੂਠ;
        }
        ਨਹੀਂਤਾਂ ਜੇ (src_->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Error(Error::Sts_ਅੰਦਰੂਨੀ, "Internal error");
        }
        CV_Assert(handle != ਨਿਰਾ);
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
        ਜੇ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ਕੋਸ਼ਿਸ਼
            {
                std::vector<ਅੱਖਰ> ਦੁਆਧਾਰੀ_Buf;
                get_ਪ੍ਰੋਗਰਾਮ_ਦੁਆਧਾਰੀ(ਦੁਆਧਾਰੀ_Buf);
                {
                    cv::utils::optional_ਤਾਲਾ_guard<cv::utils::fs::File_ਤਾਲਾ> ਤਾਲਾ_fs(file_ਤਾਲਾ.get());
                    ਦੁਆਧਾਰੀ_ਪ੍ਰੋਗਰਾਮ_File file(fname, hash_str.c_str());
                    file.ਲਿਖ(buildflags, ਦੁਆਧਾਰੀ_Buf);
                }
            }
            ਫੜੋ (ਅਚਲ cv::Exception& e)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "Can't save OpenCL binary into cache: " + fname << std::ਸਤਰ_ਅੰਤ << e.what());
            }
            ਫੜੋ (...)
            {
                CV_LOG_WARNING(ਨਿਰਾ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
#ਜੇ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS
        ਜੇ (CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS_ਮੁੱਲ)
        {
            std::vector<ਅੱਖਰ> ਦੁਆਧਾਰੀ_Buf;
            get_ਪ੍ਰੋਗਰਾਮ_ਦੁਆਧਾਰੀ(ਦੁਆਧਾਰੀ_Buf);
            ਜੇ (!ਦੁਆਧਾਰੀ_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_ਪ੍ਰੋਗਰਾਮ(handle));
                handle = ਨਿਰਾ;
                create_From_ਦੁਆਧਾਰੀ(ctx, ਦੁਆਧਾਰੀ_Buf, errmsg);
            }
        }
#ਜੇ_ਅੰਤ
        ਵਾਪਸ handle != ਨਿਰਾ;
    }

    ਖਾਲੀ dumpBuildLog_(cl_int ਨਤੀਜਾ, ਅਚਲ cl_device_id* deviceList, String& errmsg)
    {
        Auto_ਬਫਰ<ਅੱਖਰ, 4096> ਬਫਰ; ਬਫਰ[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_ਪ੍ਰੋਗਰਾਮ_Build_Info(handle, deviceList[0],
                                                  CL_ਪ੍ਰੋਗਰਾਮ_BUILD_LOG, 0, 0, &retsz);
        ਜੇ (log_retval == CL_SUCCESS && retsz > 1)
        {
            ਬਫਰ.resize(retsz + 16);
            log_retval = cl_Get_ਪ੍ਰੋਗਰਾਮ_Build_Info(handle, deviceList[0],
                                               CL_ਪ੍ਰੋਗਰਾਮ_BUILD_LOG, retsz+1, ਬਫਰ.ਅੰਕੜੇ(), &retsz);
            ਜੇ (log_retval == CL_SUCCESS)
            {
                ਜੇ (retsz < ਬਫਰ.size())
                    ਬਫਰ[retsz] = 0;
                ਨਹੀਂਤਾਂ
                    ਬਫਰ[ਬਫਰ.size() - 1] = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                ਬਫਰ[0] = 0;
            }
        }

        errmsg = String(ਬਫਰ.ਅੰਕੜੇ());
        ਲਿਖੋ("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                ਸਰੋਤ_Module.c_str(), ਸਰੋਤ_Name.c_str(),
                ਨਤੀਜਾ, getOpenCLErrorString(ਨਤੀਜਾ),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    ਤਾਰਕਿਕ buildFromSources(ਅਚਲ Context& ctx, ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->ਕਿਸਮ_5 == ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl::ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ_ਕੋਡ);
        CV_Assert(handle == ਨਿਰਾ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                ਸਰੋਤ_Module.c_str(), ਸਰੋਤ_Name.c_str(),
                src_->ਸਰੋਤ_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Compile... " << ਸਰੋਤ_Module.c_str() << "/" << ਸਰੋਤ_Name.c_str());

        ਅਚਲ ਅੱਖਰ* srcptr = src_->ਸਰੋਤ_Addr ? ((ਅਚਲ ਅੱਖਰ*)src_->ਸਰੋਤ_Addr) : src_->ਕੋਡ_Str_2.c_str();
        size_t srclen = src_->ਸਰੋਤ_Addr ? src_->ਸਰੋਤ_ਮਾਪ : src_->ਕੋਡ_Str_2.size();
        CV_Assert(srcptr != ਨਿਰਾ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_ਪ੍ਰੋਗਰਾਮ_With_ਸਰੋਤ((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        ਜੇ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_ਬਫਰ<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.ਅੰਕੜੇ();
            ਲਈ (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_ਪ੍ਰੋਗਰਾਮ(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_ਨਤੀਜਾ(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#ਜੇ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            ਜੇ (retval != CL_SUCCESS)
#ਜੇ_ਅੰਤ
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                ਜੇ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ਪ੍ਰੋਗਰਾਮ(handle));
                    handle = ਨਿਰਾ;
                }
                ਜੇ (retval != CL_SUCCESS &&
                    ਸਰੋਤ_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#ਜੇ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS
            ਜੇ (handle && CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS_ਮੁੱਲ)
            {
                CV_LOG_INFO(ਨਿਰਾ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                ਅੱਖਰ kernels_ਬਫਰ[4096] = {0};
                cl_int ਨਤੀਜਾ = cl_Get_ਪ੍ਰੋਗਰਾਮ_Info(handle, CL_ਪ੍ਰੋਗਰਾਮ_KERNEL_ਨਾਂ, ਆਕਾਰ(kernels_ਬਫਰ), &kernels_ਬਫਰ[0], &retsz);
                ਜੇ (retsz < ਆਕਾਰ(kernels_ਬਫਰ))
                    kernels_ਬਫਰ[retsz] = 0;
                ਨਹੀਂਤਾਂ
                    kernels_ਬਫਰ[0] = 0;
                CV_LOG_INFO(ਨਿਰਾ, ਨਤੀਜਾ << ": Kernels='" << kernels_ਬਫਰ << "'");
            }
#ਜੇ_ਅੰਤ
        }
        ਵਾਪਸ handle != ਨਿਰਾ;
    }

    ਖਾਲੀ get_ਪ੍ਰੋਗਰਾਮ_ਦੁਆਧਾਰੀ(std::vector<ਅੱਖਰ>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_ਪ੍ਰੋਗਰਾਮ_Info(handle, CL_ਪ੍ਰੋਗਰਾਮ_ਦੁਆਧਾਰੀ_SIZES, ਆਕਾਰ(sz), &sz, ਨਿਰਾ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_ਪ੍ਰੋਗਰਾਮ_Info(handle, CL_ਪ੍ਰੋਗਰਾਮ_BINARIES, ਆਕਾਰ(ptr), &ptr, ਨਿਰਾ));
    }

    ਤਾਰਕਿਕ create_From_ਦੁਆਧਾਰੀ(ਅਚਲ Context& ctx, ਅਚਲ std::vector<ਅੱਖਰ>& buf, String& errmsg)
    {
        ਵਾਪਸ create_From_ਦੁਆਧਾਰੀ(ctx, (ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ*)&buf[0], buf.size(), errmsg);
    }

    ਤਾਰਕਿਕ create_From_ਦੁਆਧਾਰੀ(ਅਚਲ Context& ctx, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਦੁਆਧਾਰੀ_Addr, ਅਚਲ size_t ਦੁਆਧਾਰੀ_ਮਾਪ, String& errmsg)
    {
        CV_Assert(handle == ਨਿਰਾ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ਨਿਰਾ, 0, "Load from binary... (" << ਦੁਆਧਾਰੀ_ਮਾਪ << " bytes)");

        CV_Assert(ਦੁਆਧਾਰੀ_ਮਾਪ > 0);

        size_t ndevices = (ਪੂਰਨ)ctx.ndevices();
        Auto_ਬਫਰ<cl_device_id> devices_(ndevices);
        Auto_ਬਫਰ<ਅਚਲ uchar*> ਦੁਆਧਾਰੀ_Ptrs(ndevices);
        Auto_ਬਫਰ<size_t> ਦੁਆਧਾਰੀ_Sizes(ndevices);

        cl_device_id* devices = devices_.ਅੰਕੜੇ();
        ਅਚਲ uchar** ਦੁਆਧਾਰੀ_Ptrs_2 = ਦੁਆਧਾਰੀ_Ptrs.ਅੰਕੜੇ();
        size_t* ਦੁਆਧਾਰੀ_Sizes_2 = ਦੁਆਧਾਰੀ_Sizes.ਅੰਕੜੇ();
        ਲਈ (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ਦੁਆਧਾਰੀ_Ptrs_2[i] = ਦੁਆਧਾਰੀ_Addr;
            ਦੁਆਧਾਰੀ_Sizes_2[i] = ਦੁਆਧਾਰੀ_ਮਾਪ;
        }

        cl_int ਨਤੀਜਾ = 0;
        handle = cl_Create_ਪ੍ਰੋਗਰਾਮ_With_ਦੁਆਧਾਰੀ((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.ਅੰਕੜੇ(),
                                           ਦੁਆਧਾਰੀ_Sizes_2, ਦੁਆਧਾਰੀ_Ptrs_2, ਨਿਰਾ, &ਨਤੀਜਾ);
        ਜੇ (ਨਤੀਜਾ != CL_SUCCESS)
        {
            CV_LOG_ERROR(ਨਿਰਾ, CV_OCL_API_ERROR_ਸੁਨੇਹਾ(ਨਤੀਜਾ, "clCreateProgramWithBinary"));
            ਜੇ (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_ਪ੍ਰੋਗਰਾਮ(handle));
                handle = ਨਿਰਾ;
            }
        }
        ਜੇ (!handle)
        {
            ਵਾਪਸ ਝੂਠ;
        }
        // call clBuildProgram()
        {
            ਨਤੀਜਾ = cl_Build_ਪ੍ਰੋਗਰਾਮ(handle, (cl_uint)ndevices, devices_.ਅੰਕੜੇ(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਨਤੀਜਾ, cv::format("clBuildProgram(binary: %s/%s)", ਸਰੋਤ_Module.c_str(), ਸਰੋਤ_Name.c_str()).c_str());
            ਜੇ (ਨਤੀਜਾ != CL_SUCCESS)
            {
                dumpBuildLog_(ਨਤੀਜਾ, devices, errmsg);
                ਜੇ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ਪ੍ਰੋਗਰਾਮ(handle));
                    handle = ਨਿਰਾ;
                }
                ਵਾਪਸ ਝੂਠ;
            }
        }
        // check build status
        {
            cl_build_ਸਥਿਤੀ build_ਸਥਿਤੀ = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(ਨਤੀਜਾ = cl_Get_ਪ੍ਰੋਗਰਾਮ_Build_Info(handle, devices[0], CL_ਪ੍ਰੋਗਰਾਮ_BUILD_ਸਥਿਤੀ,
                    ਆਕਾਰ(build_ਸਥਿਤੀ), &build_ਸਥਿਤੀ, &retsz));
            ਜੇ (ਨਤੀਜਾ == CL_SUCCESS)
            {
                ਜੇ (build_ਸਥਿਤੀ == CL_BUILD_SUCCESS)
                {
                    ਵਾਪਸ ਸੱਚ;
                }
                ਨਹੀਂਤਾਂ
                {
                    CV_LOG_WARNING(ਨਿਰਾ, "clGetProgramBuildInfo() returns " << build_ਸਥਿਤੀ);
                    ਵਾਪਸ ਝੂਠ;
                }
            }
            ਨਹੀਂਤਾਂ
            {
                CV_LOG_ERROR(ਨਿਰਾ, CV_OCL_API_ERROR_ਸੁਨੇਹਾ(ਨਤੀਜਾ, "clGetProgramBuildInfo()"));
                ਜੇ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_ਪ੍ਰੋਗਰਾਮ(handle));
                    handle = ਨਿਰਾ;
                }
            }
        }
#ਜੇ CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS
        ਜੇ (handle && CV_OPENCL_VALIDATE_ਦੁਆਧਾਰੀ_PROGRAMS_ਮੁੱਲ)
        {
            CV_LOG_INFO(ਨਿਰਾ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            ਅੱਖਰ kernels_ਬਫਰ[4096] = {0};
            ਨਤੀਜਾ = cl_Get_ਪ੍ਰੋਗਰਾਮ_Info(handle, CL_ਪ੍ਰੋਗਰਾਮ_KERNEL_ਨਾਂ, ਆਕਾਰ(kernels_ਬਫਰ), &kernels_ਬਫਰ[0], &retsz);
            ਜੇ (retsz < ਆਕਾਰ(kernels_ਬਫਰ))
                kernels_ਬਫਰ[retsz] = 0;
            ਨਹੀਂਤਾਂ
                kernels_ਬਫਰ[0] = 0;
            CV_LOG_INFO(ਨਿਰਾ, ਨਤੀਜਾ << ": Kernels='" << kernels_ਬਫਰ << "'");
        }
#ਜੇ_ਅੰਤ
        ਵਾਪਸ handle != ਨਿਰਾ;
    }

    ~Impl()
    {
        ਜੇ( handle )
        {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
            ਜੇ (!cv::__termination)
#ਜੇ_ਅੰਤ
            {
                cl_Release_ਪ੍ਰੋਗਰਾਮ(handle);
            }
            handle = ਨਿਰਾ;
        }
    }

    cl_ਪ੍ਰੋਗਰਾਮ handle;

    String buildflags;
    String ਸਰੋਤ_Module;
    String ਸਰੋਤ_Name;
};


ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2() CV_NOEXCEPT
{
    p = 0;
}

ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
        ਅਚਲ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog)
{
    p = prog.p;
    ਜੇ(p)
        p->addref();
}

ਪ੍ਰੋਗਰਾਮ_2& ਪ੍ਰੋਗਰਾਮ_2::ਸੰਚਾਲਕ = (ਅਚਲ ਪ੍ਰੋਗਰਾਮ_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    ਜੇ(newp)
        newp->addref();
    ਜੇ(p)
        p->release();
    p = newp;
    ਵਾਪਸ *ਇਹ;
}

ਪ੍ਰੋਗਰਾਮ_2::ਪ੍ਰੋਗਰਾਮ_2(ਪ੍ਰੋਗਰਾਮ_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = ਖਾਲੀ_ਸੂਚਕ;
}

ਪ੍ਰੋਗਰਾਮ_2& ਪ੍ਰੋਗਰਾਮ_2::ਸੰਚਾਲਕ = (ਪ੍ਰੋਗਰਾਮ_2&& prog) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &prog) {
        ਜੇ(p)
            p->release();
        p = prog.p;
        prog.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਪ੍ਰੋਗਰਾਮ_2::~ਪ੍ਰੋਗਰਾਮ_2()
{
    ਜੇ(p)
        p->release();
}

ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::create(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
            ਅਚਲ String& buildflags, String& errmsg)
{
    ਜੇ(p)
    {
        p->release();
        p = ਨਿਰਾ;
    }
    p = ਨਵਾਂ Impl(src, buildflags, errmsg);
    ਜੇ(!p->handle)
    {
        p->release();
        p = 0;
    }
    ਵਾਪਸ p != 0;
}

ਖਾਲੀ* ਪ੍ਰੋਗਰਾਮ_2::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_REMOVE_DEPRECATED_API
ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& ਪ੍ਰੋਗਰਾਮ_2::ਸਰੋਤ() ਅਚਲ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::ਪਠ(ਅਚਲ String& bin, ਅਚਲ String& buildflags)
{
    CV_ਅਣਵਰਤਿਆ(bin); CV_ਅਣਵਰਤਿਆ(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ਤਾਰਕਿਕ ਪ੍ਰੋਗਰਾਮ_2::ਲਿਖ(String& bin) ਅਚਲ
{
    CV_ਅਣਵਰਤਿਆ(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String ਪ੍ਰੋਗਰਾਮ_2::get_ਅਗੇਤਰ() ਅਚਲ
{
    ਜੇ(!p)
        ਵਾਪਸ String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ਵਾਪਸ cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ਅਗੇਤਰ_String().c_str(), p->buildflags.c_str());
}

String ਪ੍ਰੋਗਰਾਮ_2::get_ਅਗੇਤਰ(ਅਚਲ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ਵਾਪਸ cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ਅਗੇਤਰ_String().c_str(), buildflags.c_str());
}
#ਜੇ_ਅੰਤ // OPENCV_REMOVE_DEPRECATED_API

ਖਾਲੀ ਪ੍ਰੋਗਰਾਮ_2::get_ਦੁਆਧਾਰੀ(std::vector<ਅੱਖਰ>& binary) ਅਚਲ
{
    CV_Assert(p && "Empty program");
    p->get_ਪ੍ਰੋਗਰਾਮ_ਦੁਆਧਾਰੀ(binary);
}

ਪ੍ਰੋਗਰਾਮ_2 Context::Impl::getProg(ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ& src,
                               ਅਚਲ String& buildflags, String& errmsg)
{
    size_t ਹੱਦਬੰਦੀ = get_ਪ੍ਰੋਗਰਾਮ_ਗਿਣਤੀ_ਹੱਦਬੰਦੀ();
    ਅਚਲ ਪ੍ਰੋਗਰਾਮ_ਸਰੋਤ::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ਕੁੰਜੀ = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->ਸਰੋਤ_Hash.c_str(),
            get_ਅਗੇਤਰ_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_ਤਾਲਾ lock(ਪ੍ਰੋਗਰਾਮ_cache_mutex);
        phash_t::iterator it = phash.find(ਕੁੰਜੀ);
        ਜੇ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ਕੁੰਜੀ);
            ਜੇ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ਕੁੰਜੀ);
            }
            ਵਾਪਸ it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            ਜੇ (ਹੱਦਬੰਦੀ > 0 && sz >= ਹੱਦਬੰਦੀ)
            {
                ਸਥਿਰ ਤਾਰਕਿਕ warningFlag = ਝੂਠ;
                ਜੇ (!warningFlag)
                {
                    ਲਿਖੋ("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = ਸੱਚ;
                }
                ਜਦੋਂ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    ਜੇ (c != 0)
                        ਰੋਕੋ;
                }
            }
        }
    }
    ਪ੍ਰੋਗਰਾਮ_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_ਤਾਲਾ lock(ਪ੍ਰੋਗਰਾਮ_cache_mutex);
        phash.insert(std::pair<std::string, ਪ੍ਰੋਗਰਾਮ_2>(ਕੁੰਜੀ, prog));
        cacheList.push_front(ਕੁੰਜੀ);
    }
    ਵਾਪਸ prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਵਰਗ OpenCLBufferPool
{
ਸੁਰੱਖਿਅਤ:
    ~OpenCLBufferPool() { }
ਜਨਤਕ:
    ਆਭਾਸੀ T allocate(size_t size) = 0;
    ਆਭਾਸੀ ਖਾਲੀ release(T ਬਫਰ) = 0;
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ Derived, ਕਿਸਮ_ਨਾਂ ਬਫਰ_ਇੰਦਰਾਜ, ਕਿਸਮ_ਨਾਂ T>
ਵਰਗ Open_CLBuffer_Pool_ਅਧਾਰ_Impl : ਜਨਤਕ ਬਫਰ_Pool_Controller, ਜਨਤਕ OpenCLBufferPool<T>
{
ਨਿਜੀ:
    ਸਤਰਅੰਦਰ Derived& derived() { ਵਾਪਸ *static_cast<Derived*>(ਇਹ); }
ਸੁਰੱਖਿਅਤ:
    Mutex mutex_;

    size_t current_Reserved_ਮਾਪ;
    size_t max_Reserved_ਮਾਪ;

    std::list<ਬਫਰ_ਇੰਦਰਾਜ> allocatedEntries_; // Allocated and used entries
    std::list<ਬਫਰ_ਇੰਦਰਾਜ> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    ਤਾਰਕਿਕ ਲੱਭੋ_And_Remove_ਇੰਦਰਾਜ_From_Allocated_List(CV_ਬਾਹਰਲਾ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ, T ਬਫਰ)
    {
        ਕਿਸਮ_ਨਾਂ std::list<ਬਫਰ_ਇੰਦਰਾਜ>::iterator i = allocatedEntries_.begin();
        ਲਈ (; i != allocatedEntries_.end(); ++i)
        {
            ਬਫਰ_ਇੰਦਰਾਜ& e = *i;
            ਜੇ (e.cl_ਬਫਰ_2 == ਬਫਰ)
            {
                ਇੰਦਰਾਜ = e;
                allocatedEntries_.erase(i);
                ਵਾਪਸ ਸੱਚ;
            }
        }
        ਵਾਪਸ ਝੂਠ;
    }

    // synchronized
    ਤਾਰਕਿਕ ਲੱਭੋ_And_Remove_ਇੰਦਰਾਜ_From_Reserved_List(CV_ਬਾਹਰਲਾ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ, ਅਚਲ size_t size)
    {
        ਜੇ (reservedEntries_.empty())
            ਵਾਪਸ ਝੂਠ;
        ਕਿਸਮ_ਨਾਂ std::list<ਬਫਰ_ਇੰਦਰਾਜ>::iterator i = reservedEntries_.begin();
        ਕਿਸਮ_ਨਾਂ std::list<ਬਫਰ_ਇੰਦਰਾਜ>::iterator ਨਤੀਜਾ_pos = reservedEntries_.end();
        ਬਫਰ_ਇੰਦਰਾਜ ਨਤੀਜਾ;
        size_t minDiff = (size_t)(-1);
        ਲਈ (; i != reservedEntries_.end(); ++i)
        {
            ਬਫਰ_ਇੰਦਰਾਜ& e = *i;
            ਜੇ (e.ਸਮਰੱਥਾ >= size)
            {
                size_t diff = e.ਸਮਰੱਥਾ - size;
                ਜੇ (diff < std::max((size_t)4096, size / 8) && (ਨਤੀਜਾ_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    ਨਤੀਜਾ_pos = i;
                    ਨਤੀਜਾ = e;
                    ਜੇ (diff == 0)
                        ਰੋਕੋ;
                }
            }
        }
        ਜੇ (ਨਤੀਜਾ_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(ਨਤੀਜਾ_pos);
            ਇੰਦਰਾਜ = ਨਤੀਜਾ;
            current_Reserved_ਮਾਪ -= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ;
            allocatedEntries_.push_back(ਇੰਦਰਾਜ);
            ਵਾਪਸ ਸੱਚ;
        }
        ਵਾਪਸ ਝੂਠ;
    }

    // synchronized
    ਖਾਲੀ check_ਮਾਪ_ਦਾ_Reserved_Entries()
    {
        ਜਦੋਂ (current_Reserved_ਮਾਪ > max_Reserved_ਮਾਪ)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ਅਚਲ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_ਮਾਪ >= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
            current_Reserved_ਮਾਪ -= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ;
            derived().release_ਬਫਰ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ);
            reservedEntries_.pop_back();
        }
    }

    ਸਤਰਅੰਦਰ size_t ਵੰਡ_Granularity(size_t size)
    {
        // heuristic values
        ਜੇ (size < 1024*1024)
            ਵਾਪਸ 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        ਨਹੀਂਤਾਂ ਜੇ (size < 16*1024*1024)
            ਵਾਪਸ 64*1024;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ 1024*1024;
    }

ਜਨਤਕ:
    Open_CLBuffer_Pool_ਅਧਾਰ_Impl()
        : current_Reserved_ਮਾਪ(0),
          max_Reserved_ਮਾਪ(0)
    {
        // nothing
    }
    ਆਭਾਸੀ ~Open_CLBuffer_Pool_ਅਧਾਰ_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
ਜਨਤਕ:
    ਆਭਾਸੀ T allocate(size_t size) CV_OVERRIDE
    {
        Auto_ਤਾਲਾ locker(mutex_);
        ਬਫਰ_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
        ਜੇ (max_Reserved_ਮਾਪ > 0 && ਲੱਭੋ_And_Remove_ਇੰਦਰਾਜ_From_Reserved_List(ਇੰਦਰਾਜ, size))
        {
            CV_DbgAssert(size <= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
            LOG_ਬਫਰ_POOL("Reuse reserved buffer: %p\n", ਇੰਦਰਾਜ.cl_ਬਫਰ_2);
        }
        ਨਹੀਂਤਾਂ
        {
            derived().allocate_ਬਫਰ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ, size);
        }
        ਵਾਪਸ ਇੰਦਰਾਜ.cl_ਬਫਰ_2;
    }
    ਆਭਾਸੀ ਖਾਲੀ release(T ਬਫਰ) CV_OVERRIDE
    {
        Auto_ਤਾਲਾ locker(mutex_);
        ਬਫਰ_ਇੰਦਰਾਜ ਇੰਦਰਾਜ;
        CV_Assert(ਲੱਭੋ_And_Remove_ਇੰਦਰਾਜ_From_Allocated_List(ਇੰਦਰਾਜ, ਬਫਰ));
        ਜੇ (max_Reserved_ਮਾਪ == 0 || ਇੰਦਰਾਜ.ਸਮਰੱਥਾ > max_Reserved_ਮਾਪ / 8)
        {
            derived().release_ਬਫਰ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ);
        }
        ਨਹੀਂਤਾਂ
        {
            reservedEntries_.push_front(ਇੰਦਰਾਜ);
            current_Reserved_ਮਾਪ += ਇੰਦਰਾਜ.ਸਮਰੱਥਾ;
            check_ਮਾਪ_ਦਾ_Reserved_Entries();
        }
    }

    ਆਭਾਸੀ size_t get_Reserved_ਮਾਪ() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ current_Reserved_ਮਾਪ; }
    ਆਭਾਸੀ size_t get_Max_Reserved_ਮਾਪ() ਅਚਲ CV_OVERRIDE { ਵਾਪਸ max_Reserved_ਮਾਪ; }
    ਆਭਾਸੀ ਖਾਲੀ ਸੈੱਟ_Max_Reserved_ਮਾਪ(size_t size) CV_OVERRIDE
    {
        Auto_ਤਾਲਾ locker(mutex_);
        size_t old_Max_Reserved_ਮਾਪ = max_Reserved_ਮਾਪ;
        max_Reserved_ਮਾਪ = size;
        ਜੇ (max_Reserved_ਮਾਪ < old_Max_Reserved_ਮਾਪ)
        {
            ਕਿਸਮ_ਨਾਂ std::list<ਬਫਰ_ਇੰਦਰਾਜ>::iterator i = reservedEntries_.begin();
            ਲਈ (; i != reservedEntries_.end();)
            {
                ਅਚਲ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ = *i;
                ਜੇ (ਇੰਦਰਾਜ.ਸਮਰੱਥਾ > max_Reserved_ਮਾਪ / 8)
                {
                    CV_DbgAssert(current_Reserved_ਮਾਪ >= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
                    current_Reserved_ਮਾਪ -= ਇੰਦਰਾਜ.ਸਮਰੱਥਾ;
                    derived().release_ਬਫਰ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ);
                    i = reservedEntries_.erase(i);
                    ਜਾਰੀ;
                }
                ++i;
            }
            check_ਮਾਪ_ਦਾ_Reserved_Entries();
        }
    }
    ਆਭਾਸੀ ਖਾਲੀ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_ਤਾਲਾ locker(mutex_);
        ਕਿਸਮ_ਨਾਂ std::list<ਬਫਰ_ਇੰਦਰਾਜ>::const_iterator i = reservedEntries_.begin();
        ਲਈ (; i != reservedEntries_.end(); ++i)
        {
            ਅਚਲ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ = *i;
            derived().release_ਬਫਰ_ਇੰਦਰਾਜ(ਇੰਦਰਾਜ);
        }
        reservedEntries_.clear();
        current_Reserved_ਮਾਪ = 0;
    }
};

ਬਣਤਰ CLBuffer_ਇੰਦਰਾਜ
{
    cl_mem cl_ਬਫਰ_2;
    size_t ਸਮਰੱਥਾ;
    CLBuffer_ਇੰਦਰਾਜ() : cl_ਬਫਰ_2((cl_mem)ਨਿਰਾ), ਸਮਰੱਥਾ(0) { }
};

ਵਰਗ OpenCLBufferPoolImpl CV_FINAL : ਜਨਤਕ Open_CLBuffer_Pool_ਅਧਾਰ_Impl<OpenCLBufferPoolImpl, CLBuffer_ਇੰਦਰਾਜ, cl_mem>
{
ਜਨਤਕ:
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ CLBuffer_ਇੰਦਰਾਜ ਬਫਰ_ਇੰਦਰਾਜ;
ਸੁਰੱਖਿਅਤ:
    ਪੂਰਨ createFlags_;
ਜਨਤਕ:
    OpenCLBufferPoolImpl(ਪੂਰਨ createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    ਖਾਲੀ allocate_ਬਫਰ_ਇੰਦਰਾਜ(ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ, size_t size)
    {
        CV_DbgAssert(ਇੰਦਰਾਜ.cl_ਬਫਰ_2 == ਨਿਰਾ);
        ਇੰਦਰਾਜ.ਸਮਰੱਥਾ = align_ਮਾਪ(size, (ਪੂਰਨ)ਵੰਡ_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        ਇੰਦਰਾਜ.cl_ਬਫਰ_2 = cl_Create_ਬਫਰ((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, 0, &retval);
        CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, (ਖਾਲੀ*)ਇੰਦਰਾਜ.cl_ਬਫਰ_2).c_str());
        CV_Assert(ਇੰਦਰਾਜ.cl_ਬਫਰ_2 != ਨਿਰਾ);
        ਜੇ(retval == CL_SUCCESS)
        {
            CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
        }
        LOG_ਬਫਰ_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, ਇੰਦਰਾਜ.cl_ਬਫਰ_2);
        allocatedEntries_.push_back(ਇੰਦਰਾਜ);
    }

    ਖਾਲੀ release_ਬਫਰ_ਇੰਦਰਾਜ(ਅਚਲ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ)
    {
        CV_Assert(ਇੰਦਰਾਜ.ਸਮਰੱਥਾ != 0);
        CV_Assert(ਇੰਦਰਾਜ.cl_ਬਫਰ_2 != ਨਿਰਾ);
        LOG_ਬਫਰ_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                ਇੰਦਰਾਜ.cl_ਬਫਰ_2, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
        CV_OCL_DBG_CHECK(clReleaseMemObject(ਇੰਦਰਾਜ.cl_ਬਫਰ_2));
    }
};

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
ਬਣਤਰ CLSVMBuffer_ਇੰਦਰਾਜ
{
    ਖਾਲੀ* cl_ਬਫਰ_2;
    size_t ਸਮਰੱਥਾ;
    CLSVMBuffer_ਇੰਦਰਾਜ() : cl_ਬਫਰ_2(ਨਿਰਾ), ਸਮਰੱਥਾ(0) { }
};
ਵਰਗ OpenCLSVMBufferPoolImpl CV_FINAL : ਜਨਤਕ Open_CLBuffer_Pool_ਅਧਾਰ_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_ਇੰਦਰਾਜ, ਖਾਲੀ*>
{
ਜਨਤਕ:
    ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ CLSVMBuffer_ਇੰਦਰਾਜ ਬਫਰ_ਇੰਦਰਾਜ;
ਜਨਤਕ:
    OpenCLSVMBufferPoolImpl()
    {
    }

    ਖਾਲੀ allocate_ਬਫਰ_ਇੰਦਰਾਜ(ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ, size_t size)
    {
        CV_DbgAssert(ਇੰਦਰਾਜ.cl_ਬਫਰ_2 == ਨਿਰਾ);
        ਇੰਦਰਾਜ.ਸਮਰੱਥਾ = align_ਮਾਪ(size, (ਪੂਰਨ)ਵੰਡ_Granularity(size));

        Context& ctx = Context::getDefault();
        ਅਚਲ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ਤਾਰਕਿਕ is_Fine_Grain_ਬਫਰ = svmCaps.is_Support_Fine_Grain_ਬਫਰ();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_ਬਫਰ ? CL_MEM_SVM_FINE_GRAIN_ਬਫਰ : 0);

        ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (ਪੂਰਨ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
        ਖਾਲੀ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, 0);
        CV_Assert(buf);

        ਇੰਦਰਾਜ.cl_ਬਫਰ_2 = buf;
        {
            CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL);
        }
        LOG_ਬਫਰ_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, ਇੰਦਰਾਜ.cl_ਬਫਰ_2);
        allocatedEntries_.push_back(ਇੰਦਰਾਜ);
    }

    ਖਾਲੀ release_ਬਫਰ_ਇੰਦਰਾਜ(ਅਚਲ ਬਫਰ_ਇੰਦਰਾਜ& ਇੰਦਰਾਜ)
    {
        CV_Assert(ਇੰਦਰਾਜ.ਸਮਰੱਥਾ != 0);
        CV_Assert(ਇੰਦਰਾਜ.cl_ਬਫਰ_2 != ਨਿਰਾ);
        LOG_ਬਫਰ_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                ਇੰਦਰਾਜ.cl_ਬਫਰ_2, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ, (ਲੰਮਾ ਲੰਮਾ)ਇੰਦਰਾਜ.ਸਮਰੱਥਾ);
        Context& ctx = Context::getDefault();
        ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  ਇੰਦਰਾਜ.cl_ਬਫਰ_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), ਇੰਦਰਾਜ.cl_ਬਫਰ_2);
    }
};
#ਜੇ_ਅੰਤ



ਸਾਂਚਾ <ਤਾਰਕਿਕ readAccess, ਤਾਰਕਿਕ writeAccess>
ਵਰਗ Aligned_ਅੰਕੜੇ_Ptr
{
ਸੁਰੱਖਿਅਤ:
    ਅਚਲ size_t ਮਾਪ_3;
    uchar* ਅਚਲ originPtr_;
    ਅਚਲ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

ਜਨਤਕ:
    Aligned_ਅੰਕੜੇ_Ptr(uchar* ptr, size_t size, size_t alignment)
        : ਮਾਪ_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ਨਿਰਾ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        ਜੇ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ਨਵਾਂ uchar[ਮਾਪ_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ਜੇ (readAccess)
            {
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr_, originPtr_, ਮਾਪ_3);
            }
        }
    }

    uchar* getAlignedPtr() ਅਚਲ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ਵਾਪਸ ptr_;
    }

    ~Aligned_ਅੰਕੜੇ_Ptr()
    {
        ਜੇ (allocatedPtr_)
        {
            ਜੇ (writeAccess)
            {
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(originPtr_, ptr_, ਮਾਪ_3);
            }
            ਮਿਟਾਓ[] allocatedPtr_;
            allocatedPtr_ = ਨਿਰਾ;
        }
        ptr_ = ਨਿਰਾ;
    }
ਨਿਜੀ:
    Aligned_ਅੰਕੜੇ_Ptr(ਅਚਲ Aligned_ਅੰਕੜੇ_Ptr&); // disabled
    Aligned_ਅੰਕੜੇ_Ptr& ਸੰਚਾਲਕ=(ਅਚਲ Aligned_ਅੰਕੜੇ_Ptr&); // disabled
};

ਸਾਂਚਾ <ਤਾਰਕਿਕ readAccess, ਤਾਰਕਿਕ writeAccess>
ਵਰਗ Aligned_ਅੰਕੜੇ_Ptr2_D
{
ਸੁਰੱਖਿਅਤ:
    ਅਚਲ size_t ਮਾਪ_3;
    uchar* ਅਚਲ originPtr_;
    ਅਚਲ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t ਪੰਕਤੀਆਂ_2;
    size_t ਖਾਨੇ_ਗਿਣਤੀ_2;
    size_t ਕਦਮ_2;

ਜਨਤਕ:
    Aligned_ਅੰਕੜੇ_Ptr2_D(uchar* ptr, size_t ਪੰਕਤੀਆਂ, size_t ਖਾਨੇ_ਗਿਣਤੀ, size_t ਕਦਮ, size_t alignment, size_t extrabytes=0)
        : ਮਾਪ_3(ਪੰਕਤੀਆਂ*ਕਦਮ), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ਨਿਰਾ), ਪੰਕਤੀਆਂ_2(ਪੰਕਤੀਆਂ), ਖਾਨੇ_ਗਿਣਤੀ_2(ਖਾਨੇ_ਗਿਣਤੀ), ਕਦਮ_2(ਕਦਮ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ਨਿਰਾ);
        ਜੇ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ਨਵਾਂ uchar[ਮਾਪ_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ਜੇ (readAccess)
            {
                ਲਈ (size_t i = 0; i < ਪੰਕਤੀਆਂ_2; i++)
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptr_ + i*ਕਦਮ_2, originPtr_ + i*ਕਦਮ_2, ਖਾਨੇ_ਗਿਣਤੀ_2);
            }
        }
    }

    uchar* getAlignedPtr() ਅਚਲ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ਵਾਪਸ ptr_;
    }

    ~Aligned_ਅੰਕੜੇ_Ptr2_D()
    {
        ਜੇ (allocatedPtr_)
        {
            ਜੇ (writeAccess)
            {
                ਲਈ (size_t i = 0; i < ਪੰਕਤੀਆਂ_2; i++)
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(originPtr_ + i*ਕਦਮ_2, ptr_ + i*ਕਦਮ_2, ਖਾਨੇ_ਗਿਣਤੀ_2);
            }
            ਮਿਟਾਓ[] allocatedPtr_;
            allocatedPtr_ = ਨਿਰਾ;
        }
        ptr_ = ਨਿਰਾ;
    }
ਨਿਜੀ:
    Aligned_ਅੰਕੜੇ_Ptr2_D(ਅਚਲ Aligned_ਅੰਕੜੇ_Ptr2_D&); // disabled
    Aligned_ਅੰਕੜੇ_Ptr2_D& ਸੰਚਾਲਕ=(ਅਚਲ Aligned_ਅੰਕੜੇ_Ptr2_D&); // disabled
};

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT 16
#ਜੇ_ਅੰਤ


ਖਾਲੀ Context::Impl::__init_buffer_pools()
{
    ਬਫਰ_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& ਬਫਰ_Pool_3 = *ਬਫਰ_Pool_2.get();
    ਬਫਰ_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& ਬਫਰ_Pool_Host_Ptr_2 = *ਬਫਰ_Pool_Host_Ptr.get();

    size_t default_Pool_ਮਾਪ = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_ਮਾਪ = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_ਮਾਪ);
    ਬਫਰ_Pool_3.ਸੈੱਟ_Max_Reserved_ਮਾਪ(pool_ਮਾਪ);
    size_t pool_ਮਾਪ_Host_Ptr = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_ਮਾਪ);
    ਬਫਰ_Pool_Host_Ptr_2.ਸੈੱਟ_Max_Reserved_ਮਾਪ(pool_ਮਾਪ_Host_Ptr);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
    ਬਫਰ_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& ਬਫਰ_Pool_SVM_2 = *ਬਫਰ_Pool_SVM.get();
    size_t pool_ਮਾਪ_SVM = utils::get_Configuration_Parameter_ਮਾਪ_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_ਮਾਪ);
    ਬਫਰ_Pool_SVM_2.ਸੈੱਟ_Max_Reserved_ਮਾਪ(pool_ਮਾਪ_SVM);
#ਜੇ_ਅੰਤ

    CV_LOG_INFO(ਨਿਰਾ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_ਮਾਪ << " poolSizeHostPtr=" << pool_ਮਾਪ_Host_Ptr);
}

ਵਰਗ OpenCLAllocator CV_FINAL : ਜਨਤਕ MatAllocator
{
ਜਨਤਕ:
    ਗਿਣਤੀ AllocatorFlags
    {
        ALLOCATOR_FLAGS_ਬਫਰ_POOL_ਵਰਤਿਆ = 1 << 0,
        ALLOCATOR_FLAGS_ਬਫਰ_POOL_HOST_PTR_ਵਰਤਿਆ = 1 << 1,
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_ਬਫਰ_POOL_SVM_ਵਰਤਿਆ = 1 << 2,
#ਜੇ_ਅੰਤ
        ALLOCATOR_FLAGS_EXTERNAL_ਬਫਰ = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_ਕਤਾਰ();
    }

    UMat_ਅੰਕੜੇ* defaultAllocate(ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type, ਖਾਲੀ* ਅੰਕੜੇ, size_t* ਕਦਮ,
            AccessFlag flags, UMatUsageFlags usageFlags) ਅਚਲ
    {
        UMat_ਅੰਕੜੇ* u = matStdAllocator->allocate(dims, sizes, type, ਅੰਕੜੇ, ਕਦਮ, flags, usageFlags);
        ਵਾਪਸ u;
    }

    ਸਥਿਰ ਤਾਰਕਿਕ isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ਸਥਿਰ ਤਾਰਕਿਕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", ਝੂਠ);
        ਵਾਪਸ value;
    }
    ਸਥਿਰ ਤਾਰਕਿਕ isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ਸਥਿਰ ਤਾਰਕਿਕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", ਝੂਠ);
        ਵਾਪਸ value;
    }

    ਖਾਲੀ get_ਉੱਤਮ_Flags(ਅਚਲ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, ਪੂਰਨ& createFlags, UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag& flags0) ਅਚਲ
    {
        ਅਚਲ Device& dev = ctx.device(0);
        createFlags = 0;
        ਜੇ ((usageFlags & USAGE_ALLOCATE_HOST_ਸਮ੍ਰਿਤੀ) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        ਜੇ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_ਸਮ੍ਰਿਤੀ_2()
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __APPLE__
                || dev.isIntel()
#ਜੇ_ਅੰਤ
                )
            )
        )
            flags0 = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
        ਨਹੀਂਤਾਂ
            flags0 = UMat_ਅੰਕੜੇ::COPY_ON_MAP;
    }

    UMat_ਅੰਕੜੇ* allocate(ਪੂਰਨ dims, ਅਚਲ ਪੂਰਨ* sizes, ਪੂਰਨ type,
                       ਖਾਲੀ* ਅੰਕੜੇ, size_t* ਕਦਮ, AccessFlag flags, UMatUsageFlags usageFlags) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!useOpenCL())
            ਵਾਪਸ defaultAllocate(dims, sizes, type, ਅੰਕੜੇ, ਕਦਮ, flags, usageFlags);

        flush_Cleanup_ਕਤਾਰ();

        CV_Assert(ਅੰਕੜੇ == 0);
        size_t ਕੁੱਲ = CV_ELEM_ਮਾਪ(type);
        ਲਈ( ਪੂਰਨ i = dims-1; i >= 0; i-- )
        {
            ਜੇ( ਕਦਮ )
                ਕਦਮ[i] = ਕੁੱਲ;
            ਕੁੱਲ *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        ਜੇ (!ctx.getImpl())
            ਵਾਪਸ defaultAllocate(dims, sizes, type, ਅੰਕੜੇ, ਕਦਮ, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        ਪੂਰਨ createFlags = 0;
        UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag flags0 = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
        get_ਉੱਤਮ_Flags(ctx, flags, usageFlags, createFlags, flags0);

        ਖਾਲੀ* handle = ਨਿਰਾ;
        ਪੂਰਨ allocatorFlags = 0;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਅਚਲ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ਜੇ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_ਬਫਰ_POOL_SVM_ਵਰਤਿਆ;
            handle = ctxImpl.get_ਬਫਰ_Pool_SVM().allocate(ਕੁੱਲ);

            // this property is constant, so single buffer pool can be used here
            ਤਾਰਕਿਕ is_Fine_Grain_ਬਫਰ = svmCaps.is_Support_Fine_Grain_ਬਫਰ();
            allocatorFlags |= is_Fine_Grain_ਬਫਰ ? svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ : svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ;
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਜੇ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_ਬਫਰ_POOL_ਵਰਤਿਆ;
            handle = ctxImpl.get_ਬਫਰ_Pool().allocate(ਕੁੱਲ);
        }
        ਨਹੀਂਤਾਂ ਜੇ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_ਬਫਰ_POOL_HOST_PTR_ਵਰਤਿਆ;
            handle = ctxImpl.get_ਬਫਰ_Pool_Host_Ptr().allocate(ਕੁੱਲ);
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Assert(handle != ਨਿਰਾ); // Unsupported, throw
        }

        ਜੇ (!handle)
            ਵਾਪਸ defaultAllocate(dims, sizes, type, ਅੰਕੜੇ, ਕਦਮ, flags, usageFlags);

        UMat_ਅੰਕੜੇ* u = ਨਵਾਂ UMat_ਅੰਕੜੇ(ਇਹ);
        u->ਅੰਕੜੇ = 0;
        u->size = ਕੁੱਲ;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<ਖਾਲੀ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->ਆਰਜ਼ੀ_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(ਸੱਚ);
        opencl_allocator_stats.onAllocate(u->size);
        ਵਾਪਸ u;
    }

    ਤਾਰਕਿਕ allocate(UMat_ਅੰਕੜੇ* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ ਝੂਠ;

        flush_Cleanup_ਕਤਾਰ();

        UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ lock(u);

        ਜੇ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            ਪੂਰਨ createFlags = 0;
            UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag flags0 = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
            get_ਉੱਤਮ_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            ਤਾਰਕਿਕ copyOnMap = (flags0 & UMat_ਅੰਕੜੇ::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            ਪੂਰਨ allocatorFlags = 0;
            UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag ਆਰਜ਼ੀ_UMat_Flags = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
            ਖਾਲੀ* handle = ਨਿਰਾ;
            cl_int retval = CL_SUCCESS;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            ਤਾਰਕਿਕ useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            ਜੇ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                ਆਰਜ਼ੀ_UMat_Flags = UMat_ਅੰਕੜੇ::ਆਰਜ਼ੀ_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (ਪੂਰਨ)u->size, handle);
            }
            ਨਹੀਂਤਾਂ ਜੇ (useSVM && (svmCaps.is_Support_Fine_Grain_ਬਫਰ() || svmCaps.is_Support_Coarse_Grain_ਬਫਰ()))
            {
                ਜੇ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    ਤਾਰਕਿਕ is_Fine_Grain_ਬਫਰ = svmCaps.is_Support_Fine_Grain_ਬਫਰ();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_ਬਫਰ ? CL_MEM_SVM_FINE_GRAIN_ਬਫਰ : 0);

                    ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (ਪੂਰਨ)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_ਕਤਾਰ q = ਨਿਰਾ;
                    ਜੇ (!is_Fine_Grain_ਬਫਰ)
                    {
                        q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (ਪੂਰਨ)u->size);
                        cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ਨਿਰਾ, ਨਿਰਾ);
                        CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMap()");

                    }
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(handle, u->origdata, u->size);
                    ਜੇ (!is_Fine_Grain_ਬਫਰ)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, handle, 0, ਨਿਰਾ, ਨਿਰਾ);
                        CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                    }

                    ਆਰਜ਼ੀ_UMat_Flags = UMat_ਅੰਕੜੇ::ਆਰਜ਼ੀ_UMAT | UMat_ਅੰਕੜੇ::ਆਰਜ਼ੀ_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_ਬਫਰ ? svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ
                                                : svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ;
                }
            }
            ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
            {
                ਜੇ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                ਆਰਜ਼ੀ_UMat_Flags = UMat_ਅੰਕੜੇ::ਆਰਜ਼ੀ_UMAT;
                ਜੇ (
                #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
                    !copyOnMap &&
                #ਜੇ_ਅੰਤ
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (ਪੂਰਨ)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_ਅੰਕੜੇ && u->original_UMat_ਅੰਕੜੇ->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_ਬਫਰ(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, u->origdata, (ਖਾਲੀ*)handle).c_str());
                }
                ਜੇ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_ਬਫਰ(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, u->origdata, (ਖਾਲੀ*)handle).c_str());
                    ਆਰਜ਼ੀ_UMat_Flags |= UMat_ਅੰਕੜੇ::ਆਰਜ਼ੀ_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clCreateBuffer() => %p", (ਖਾਲੀ*)handle).c_str());
            ਜੇ(!handle || retval != CL_SUCCESS)
                ਵਾਪਸ ਝੂਠ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ਇਹ;
            u->flags |= ਆਰਜ਼ੀ_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        ਜੇ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(ਸੱਚ);
        opencl_allocator_stats.onAllocate(u->size);
        ਵਾਪਸ ਸੱਚ;
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

    ਖਾਲੀ deallocate(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        ਜੇ (!!(u->flags & UMat_ਅੰਕੜੇ::ASYNC_CLEANUP))
            ਜੋੜੋ_ਮੰਜ਼ਿਲ_Cleanup_ਕਤਾਰ(u);
        ਨਹੀਂਤਾਂ
            deallocate_(u);
    }

    ਖਾਲੀ deallocate_(UMat_ਅੰਕੜੇ* u) ਅਚਲ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        ਜੇ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_ਬਫਰ) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
        ਜੇ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ਵਾਪਸ;             // avoid any OpenCL calls
#ਜੇ_ਅੰਤ
        ਜੇ(u->ਆਰਜ਼ੀ_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            ਜੇ (u->hostCopyObsolete())
            {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
                ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    ਜੇ( u->ਆਰਜ਼ੀ_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ);
                        ਤਾਰਕਿਕ is_Fine_Grain_ਬਫਰ = (u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ;
                        cl_command_ਕਤਾਰ q = ਨਿਰਾ;
                        ਜੇ (!is_Fine_Grain_ਬਫਰ)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) == 0));
                            q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ਪੂਰਨ)u->size);
                            cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ਨਿਰਾ, ਨਿਰਾ);
                            CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMap()");
                        }
                        cl_ਅੰਤ(q);
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(u->origdata, u->handle, u->size);
                        ਜੇ (!is_Fine_Grain_ਬਫਰ)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, u->handle, 0, ਨਿਰਾ, ਨਿਰਾ);
                            CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                        }
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
                {
                    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
                    ਜੇ( u->ਆਰਜ਼ੀ_Copied_UMat() )
                    {
                        Aligned_ਅੰਕੜੇ_Ptr<ਝੂਠ, ਸੱਚ> alignedPtr(u->origdata, u->size, CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        cl_int retval = 0;
                        ਜੇ (u->ਆਰਜ਼ੀ_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_ਕਤਾਰ(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            ਖਾਲੀ* ਅੰਕੜੇ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Map_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ਖਾਲੀ*)u->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, ਅੰਕੜੇ).c_str());
                            CV_Assert(u->origdata == ਅੰਕੜੇ && "Details: https://github.com/opencv/opencv/issues/6293");
                            ਜੇ (u->original_UMat_ਅੰਕੜੇ)
                            {
                                CV_Assert(u->original_UMat_ਅੰਕੜੇ->ਅੰਕੜੇ == ਅੰਕੜੇ);
                            }
                            retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Unmap_Mem_Object(q, (cl_mem)u->handle, ਅੰਕੜੇ, 0, 0, 0);
                            CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ਖਾਲੀ*)u->handle, ਅੰਕੜੇ, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_ਅੰਤ(q));
                        }
                    }
                }
                u->markHostCopyObsolete(ਝੂਠ);
            }
            ਨਹੀਂਤਾਂ
            {
                // nothing
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
            {
                ਜੇ( u->ਆਰਜ਼ੀ_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clReleaseMemObject(ptr=%p)", (ਖਾਲੀ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ਸੱਚ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ਨਿਰਾ;
            ਜੇ(u->ਅੰਕੜੇ && u->copyOnMap() && u->ਅੰਕੜੇ != u->origdata)
                fastFree(u->ਅੰਕੜੇ);
            u->ਅੰਕੜੇ = u->origdata;
            u->currAllocator->deallocate(u);
            u = ਨਿਰਾ;
        }
        ਨਹੀਂਤਾਂ
        {
            CV_Assert(u->origdata == ਨਿਰਾ);
            ਜੇ(u->ਅੰਕੜੇ && u->copyOnMap() && u->ਅੰਕੜੇ != u->origdata)
            {
                fastFree(u->ਅੰਕੜੇ);
                u->ਅੰਕੜੇ = 0;
                u->markHostCopyObsolete(ਸੱਚ);
            }
            ਜੇ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ਬਫਰ_POOL_ਵਰਤਿਆ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ਬਫਰ_Pool().release((cl_mem)u->handle);
            }
            ਨਹੀਂਤਾਂ ਜੇ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ਬਫਰ_POOL_HOST_PTR_ਵਰਤਿਆ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ਬਫਰ_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            ਨਹੀਂਤਾਂ ਜੇ (u->allocatorFlags_ & ALLOCATOR_FLAGS_ਬਫਰ_POOL_SVM_ਵਰਤਿਆ)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                ਨਹੀਂਤਾਂ ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
                {
                    ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

                    ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, u->handle, 0, ਨਿਰਾ, ਨਿਰਾ);
                        CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_ਬਫਰ_Pool_SVM().release((ਖਾਲੀ*)u->handle);
            }
#ਜੇ_ਅੰਤ
            ਨਹੀਂਤਾਂ
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ਸੱਚ);
            ਮਿਟਾਓ u;
            u = ਨਿਰਾ;
        }
        CV_Assert(u == ਨਿਰਾ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    ਖਾਲੀ map(UMat_ਅੰਕੜੇ* u, AccessFlag accessFlags) ਅਚਲ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        ਜੇ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ਸੱਚ);

        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

        {
            ਜੇ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
                ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
                {
                    ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
                    {
                        Context& ctx = Context::getDefault();
                        ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ਪੂਰਨ)u->size);
                            cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ਨਿਰਾ, ਨਿਰਾ);
                            CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_ਬਫਰ_MAP;
                        }
                    }
                    cl_ਅੰਤ(q);
                    u->ਅੰਕੜੇ = (uchar*)u->handle;
                    u->markHostCopyObsolete(ਝੂਠ);
                    u->markDeviceMemMapped(ਸੱਚ);
                    ਵਾਪਸ;
                }
#ਜੇ_ਅੰਤ

                cl_int retval = CL_SUCCESS;
                ਜੇ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->ਅੰਕੜੇ = (uchar*)cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Map_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ਖਾਲੀ*)u->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, u->ਅੰਕੜੇ).c_str());
                }
                ਜੇ (u->ਅੰਕੜੇ && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(ਝੂਠ);
                    u->markDeviceMemMapped(ਸੱਚ);
                    ਵਾਪਸ;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_ਅੰਕੜੇ::COPY_ON_MAP;
            }

            ਜੇ(!u->ਅੰਕੜੇ)
            {
                u->ਅੰਕੜੇ = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(ਸੱਚ);
            }
        }

        ਜੇ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_ਅੰਕੜੇ_Ptr<ਝੂਠ, ਸੱਚ> alignedPtr(u->ਅੰਕੜੇ, u->size, CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == 0);
#ਜੇ_ਅੰਤ
            cl_int retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ਖਾਲੀ*)u->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(ਝੂਠ);
        }
    }

    ਖਾਲੀ unmap(UMat_ਅੰਕੜੇ* u) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ;


        CV_Assert(u->handle != 0);

        UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ autolock(u);

        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
        cl_int retval = 0;
        ਜੇ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->ਅੰਕੜੇ != ਨਿਰਾ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
            {
                ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
                {
                    Context& ctx = Context::getDefault();
                    ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, u->handle,
                                0, ਨਿਰਾ, ਨਿਰਾ);
                        CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                        cl_ਅੰਤ(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_ਬਫਰ_MAP;
                    }
                }
                ਜੇ (u->refcount == 0)
                    u->ਅੰਕੜੇ = 0;
                u->markDeviceCopyObsolete(ਝੂਠ);
                u->markHostCopyObsolete(ਸੱਚ);
                ਵਾਪਸ;
            }
#ਜੇ_ਅੰਤ
            ਜੇ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Unmap_Mem_Object(q, (cl_mem)u->handle, u->ਅੰਕੜੇ, 0, 0, 0);
                CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ਖਾਲੀ*)u->handle, u->ਅੰਕੜੇ, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size).c_str());
                ਜੇ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_ਅੰਤ(q));
                }
                u->markDeviceMemMapped(ਝੂਠ);
                u->ਅੰਕੜੇ = 0;
                u->markDeviceCopyObsolete(ਝੂਠ);
                u->markHostCopyObsolete(ਸੱਚ);
            }
        }
        ਨਹੀਂਤਾਂ ਜੇ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_ਅੰਕੜੇ_Ptr<ਸੱਚ, ਝੂਠ> alignedPtr(u->ਅੰਕੜੇ, u->size, CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == 0);
#ਜੇ_ਅੰਤ
            retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Write_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ਖਾਲੀ*)u->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(ਝੂਠ);
            u->markHostCopyObsolete(ਸੱਚ);
        }
    }

    ਤਾਰਕਿਕ checkContinuous(ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                         ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
                         ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[],
                         size_t& ਕੁੱਲ, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ਅਚਲ
    {
        ਤਾਰਕਿਕ iscontinuous = ਸੱਚ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        ਕੁੱਲ = sz[dims-1];
        ਲਈ( ਪੂਰਨ i = dims-2; i >= 0; i-- )
        {
            ਜੇ( i >= 0 && (ਕੁੱਲ != srcstep[i] || ਕੁੱਲ != dststep[i]) )
                iscontinuous = ਝੂਠ;
            ਕੁੱਲ *= sz[i];
            ਜੇ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            ਜੇ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        ਜੇ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            ਜੇ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                ਜੇ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                ਜੇ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            ਨਹੀਂਤਾਂ
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                ਜੇ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                ਜੇ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ਵਾਪਸ iscontinuous;
    }

    ਖਾਲੀ download(UMat_ਅੰਕੜੇ* u, ਖਾਲੀ* dstptr, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                  ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
                  ਅਚਲ size_t dststep[]) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ;
        UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ autolock(u);

        ਜੇ( u->ਅੰਕੜੇ && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ਵਾਪਸ;
        }
        CV_Assert( u->handle != 0 );

        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

        size_t ਕੁੱਲ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ਤਾਰਕਿਕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            ਕੁੱਲ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
        {
            CV_DbgAssert(u->ਅੰਕੜੇ == ਨਿਰਾ || u->ਅੰਕੜੇ == u->handle);
            Context& ctx = Context::getDefault();
            ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) == 0);
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ਪੂਰਨ)u->size);
                cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ਨਿਰਾ, ਨਿਰਾ);
                CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMap()");
            }
            cl_ਅੰਤ(q);
            ਜੇ( iscontinuous )
            {
                ਸਮ੍ਰਿਤੀ_ਨਕਲ(dstptr, (uchar*)u->handle + srcrawofs, ਕੁੱਲ);
            }
            ਨਹੀਂਤਾਂ
            {
                // This code is from MatAllocator::download()
                ਪੂਰਨ isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                    ਜੇ( sz[i] == 0 )
                    ਵਾਪਸ;
                    ਜੇ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (ਪੂਰਨ)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ਅਚਲ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ਲਈ( j = 0; j < it.nplanes; j++, ++it )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
            }
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, u->handle,
                        0, ਨਿਰਾ, ਨਿਰਾ);
                CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                cl_ਅੰਤ(q);
            }
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            ਜੇ( iscontinuous )
            {
                Aligned_ਅੰਕੜੇ_Ptr<ਝੂਠ, ਸੱਚ> alignedPtr((uchar*)dstptr, ਕੁੱਲ, CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, ਕੁੱਲ, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            ਨਹੀਂਤਾਂ ਜੇ (CV_OPENCL_DISABLE_ਬਫਰ_RECT_OPERATIONS)
            {
                ਅਚਲ size_t padding = CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_ਅੰਕੜੇ_Ptr2_D<ਝੂਠ, ਝੂਠ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                ਕੁੱਲ = align_ਮਾਪ(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                ਕੁੱਲ = std::min(ਕੁੱਲ, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, ਕੁੱਲ, ptr, 0, 0, 0));
                ਲਈ( size_t i = 0; i < new_sz[1]; i++ )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            ਨਹੀਂਤਾਂ
            {
                Aligned_ਅੰਕੜੇ_Ptr2_D<ਝੂਠ, ਸੱਚ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    ਖਾਲੀ upload(UMat_ਅੰਕੜੇ* u, ਅਚਲ ਖਾਲੀ* srcptr, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
                ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[],
                ਅਚਲ size_t srcstep[]) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!u)
            ਵਾਪਸ;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->ਆਰਜ਼ੀ_UMat());

        size_t ਕੁੱਲ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ਤਾਰਕਿਕ iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            ਕੁੱਲ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        ਜੇ( u->ਅੰਕੜੇ && (u->hostCopyObsolete() < u->deviceCopyObsolete() || ਕੁੱਲ == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(ਝੂਠ);
            u->markDeviceCopyObsolete(ਸੱਚ);
            ਵਾਪਸ;
        }

        CV_Assert( u->handle != 0 );
        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
        {
            CV_DbgAssert(u->ਅੰਕੜੇ == ਨਿਰਾ || u->ਅੰਕੜੇ == u->handle);
            Context& ctx = Context::getDefault();
            ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_MAP) == 0);
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ਪੂਰਨ)u->size);
                cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ਨਿਰਾ, ਨਿਰਾ);
                CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMap()");
            }
            cl_ਅੰਤ(q);
            ਜੇ( iscontinuous )
            {
                ਸਮ੍ਰਿਤੀ_ਨਕਲ((uchar*)u->handle + dstrawofs, srcptr, ਕੁੱਲ);
            }
            ਨਹੀਂਤਾਂ
            {
                // This code is from MatAllocator::upload()
                ਪੂਰਨ isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                    ਜੇ( sz[i] == 0 )
                    ਵਾਪਸ;
                    ਜੇ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (ਪੂਰਨ)sz[i];
                }

                Mat src(dims, isz, CV_8U, (ਖਾਲੀ*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ਅਚਲ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ਲਈ( j = 0; j < it.nplanes; j++, ++it )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
            }
            ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_COARSE_GRAIN_ਬਫਰ)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMUnmap(q, u->handle,
                        0, ਨਿਰਾ, ਨਿਰਾ);
                CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMUnmap()");
                cl_ਅੰਤ(q);
            }
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            ਜੇ( iscontinuous )
            {
                Aligned_ਅੰਕੜੇ_Ptr<ਸੱਚ, ਝੂਠ> alignedPtr((uchar*)srcptr, ਕੁੱਲ, CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
                cl_int retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Write_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, ਕੁੱਲ, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (ਖਾਲੀ*)u->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)dstrawofs, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            ਨਹੀਂਤਾਂ ਜੇ (CV_OPENCL_DISABLE_ਬਫਰ_RECT_OPERATIONS)
            {
                ਅਚਲ size_t padding = CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_ਅੰਕੜੇ_Ptr2_D<ਝੂਠ, ਝੂਠ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                ਕੁੱਲ = align_ਮਾਪ(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                ਕੁੱਲ = std::min(ਕੁੱਲ, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ਕੁੱਲ, ptr, 0, 0, 0));
                ਲਈ( size_t i = 0; i < new_sz[1]; i++ )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Write_ਬਫਰ(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ਕੁੱਲ, ptr, 0, 0, 0));
            }
            ਨਹੀਂਤਾਂ
            {
                Aligned_ਅੰਕੜੇ_Ptr2_D<ਸੱਚ, ਝੂਠ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Write_ਬਫਰ_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(ਸੱਚ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            u->markHostCopyObsolete(ਸੱਚ);
        }
        u->markDeviceCopyObsolete(ਝੂਠ);
    }

    ਖਾਲੀ copy(UMat_ਅੰਕੜੇ* src, UMat_ਅੰਕੜੇ* dst, ਪੂਰਨ dims, ਅਚਲ size_t sz[],
              ਅਚਲ size_t srcofs[], ਅਚਲ size_t srcstep[],
              ਅਚਲ size_t dstofs[], ਅਚਲ size_t dststep[], ਤਾਰਕਿਕ _sync) ਅਚਲ CV_OVERRIDE
    {
        ਜੇ(!src || !dst)
            ਵਾਪਸ;

        size_t ਕੁੱਲ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ਤਾਰਕਿਕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            ਕੁੱਲ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ਅੰਕੜੇ_Auto_ਤਾਲਾ src_autolock(src, dst);

        ਜੇ( !src->handle || (src->ਅੰਕੜੇ && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->ਅੰਕੜੇ + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ਵਾਪਸ;
        }
        ਜੇ( !dst->handle || (dst->ਅੰਕੜੇ && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->ਅੰਕੜੇ + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(ਝੂਠ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
            ਜੇ ((dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
            {
                dst->markDeviceCopyObsolete(ਸੱਚ);
            }
            ਵਾਪਸ;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((src->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
        {
            ਜੇ ((src->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
            {
                Context& ctx = Context::getDefault();
                ਅਚਲ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                ਜੇ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (ਪੂਰਨ)ਕੁੱਲ);
                    cl_int ਸਥਿਤੀ = svmFns->fn_cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            ਕੁੱਲ, 0, ਨਿਰਾ, ਨਿਰਾ);
                    CV_OCL_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ, "clEnqueueSVMMemcpy()");
                }
                ਨਹੀਂਤਾਂ
                {
                    cl_ਅੰਤ(q);
                    // This code is from MatAllocator::download()/upload()
                    ਪੂਰਨ isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)ਵੱਧ_ਤੋਂ_ਵੱਧ_ਅੰਕ );
                        ਜੇ( sz[i] == 0 )
                        ਵਾਪਸ;
                        ਜੇ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (ਪੂਰਨ)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    ਲਈ( ਪੂਰਨ i = 0; i < dims; i++ )
                    {
                        ਜੇ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ਅਚਲ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    ਲਈ( j = 0; j < it.nplanes; j++, ++it )
                        ਸਮ੍ਰਿਤੀ_ਨਕਲ(ptrs[1], ptrs[0], planesz);
                }
            }
            ਨਹੀਂਤਾਂ
            {
                ਜੇ ((src->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->ਅੰਕੜੇ + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                ਨਹੀਂਤਾਂ
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->ਅੰਕੜੇ + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            ਜੇ( iscontinuous )
            {
                retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, ਕੁੱਲ, 0, 0, 0);
                CV_OCL_CHECK_ਨਤੀਜਾ(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (ਖਾਲੀ*)src->handle, (ਖਾਲੀ*)dst->handle, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)srcrawofs, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)dstrawofs, (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)ਕੁੱਲ).c_str());
            }
            ਨਹੀਂਤਾਂ ਜੇ (CV_OPENCL_DISABLE_ਬਫਰ_RECT_OPERATIONS)
            {
                ਅਚਲ size_t padding = CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_ਅੰਕੜੇ_Ptr2_D<ਝੂਠ, ਝੂਠ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT, padding*2);
                Aligned_ਅੰਕੜੇ_Ptr2_D<ਝੂਠ, ਝੂਠ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_ਅੰਕੜੇ_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_ਕੁੱਲ = align_ਮਾਪ(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_ਕੁੱਲ = std::min(src_ਕੁੱਲ, src->size - new_srcrawofs);
                size_t dst_ਕੁੱਲ = align_ਮਾਪ(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_ਕੁੱਲ = std::min(dst_ਕੁੱਲ, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_ਕੁੱਲ, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Read_ਬਫਰ(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_ਕੁੱਲ, dstptr, 0, 0, 0));

                ਲਈ( size_t i = 0; i < new_sz[1]; i++ )
                    ਸਮ੍ਰਿਤੀ_ਨਕਲ( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Write_ਬਫਰ(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_ਕੁੱਲ, dstptr, 0, 0, 0));
            }
            ਨਹੀਂਤਾਂ
            {
                CV_OCL_CHECK(retval = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        ਜੇ (retval == CL_SUCCESS)
        {
            CV_IMPL_ਜੋੜੋ(CV_IMPL_OCL)
        }

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_ਬਫਰ ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_ਬਫਰ_ਨਕਾਬ) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            dst->markHostCopyObsolete(ਸੱਚ);
        }
        dst->markDeviceCopyObsolete(ਝੂਠ);

        ਜੇ( _sync )
        {
            CV_OCL_DBG_CHECK(cl_ਅੰਤ(q));
        }
    }

    ਬਫਰ_Pool_Controller* get_ਬਫਰ_Pool_Controller(ਅਚਲ ਅੱਖਰ* id) ਅਚਲ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        ਜੇ (ctx.empty())
            ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_SVM
        ਜੇ ((svm::checkForceSVMUmatUsage() && (id == ਨਿਰਾ || strcmp(id, "OCL") == 0)) || (id != ਨਿਰਾ && strcmp(id, "SVM") == 0))
        {
            ਵਾਪਸ &ctx.getImpl()->get_ਬਫਰ_Pool_SVM();
        }
#ਜੇ_ਅੰਤ
        ਜੇ (id != ਨਿਰਾ && strcmp(id, "HOST_ALLOC") == 0)
        {
            ਵਾਪਸ &ctx.getImpl()->get_ਬਫਰ_Pool_Host_Ptr();
        }
        ਜੇ (id != ਨਿਰਾ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ਵਾਪਸ &ctx.getImpl()->get_ਬਫਰ_Pool();
    }

    MatAllocator* matStdAllocator;

    ਬਦਲਣਯੋਗ cv::Mutex cleanup_ਕਤਾਰ_Mutex;
    ਬਦਲਣਯੋਗ std::deque<UMat_ਅੰਕੜੇ*> cleanup_ਕਤਾਰ;

    ਖਾਲੀ flush_Cleanup_ਕਤਾਰ() ਅਚਲ
    {
        // TSAN false positive: see #27638
        ਜੇ (!cleanup_ਕਤਾਰ.empty())
        {
            std::deque<UMat_ਅੰਕੜੇ*> q;
            {
                cv::Auto_ਤਾਲਾ lock(cleanup_ਕਤਾਰ_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_ਕਤਾਰ);
            }
            ਲਈ (std::deque<UMat_ਅੰਕੜੇ*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    ਖਾਲੀ ਜੋੜੋ_ਮੰਜ਼ਿਲ_Cleanup_ਕਤਾਰ(UMat_ਅੰਕੜੇ* u) ਅਚਲ
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_ਤਾਲਾ lock(cleanup_ਕਤਾਰ_Mutex);
            cleanup_ਕਤਾਰ.push_back(u);
        }
    }
};

ਸਥਿਰ OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ਸਥਿਰ OpenCLAllocator* g_allocator = ਨਵਾਂ OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ਵਾਪਸ g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


ਨਾਮਾਕਾਸ਼ cv {

// three funcs below are implemented in umatrix.cpp
ਖਾਲੀ ਸੈੱਟ_ਮਾਪ( UMat& m, ਪੂਰਨ _dims, ਅਚਲ ਪੂਰਨ* _sz, ਅਚਲ size_t* ਕਦਮਾਂ_2,
              ਤਾਰਕਿਕ auto_ਕਦਮਾਂ = ਝੂਠ );
ਖਾਲੀ finalizeHdr(UMat& m);

} // namespace cv


ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
ਖਾਲੀ convert_From_ਬਫਰ(ਖਾਲੀ* cl_mem_ਬਫਰ, size_t ਕਦਮ, ਪੂਰਨ ਪੰਕਤੀਆਂ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ type, UMat& dst)
{
    ਪੂਰਨ d = 2;
    ਪੂਰਨ sizes[] = { ਪੰਕਤੀਆਂ, ਖਾਨੇ_ਗਿਣਤੀ };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_ਨਕਾਬ) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    ਸੈੱਟ_ਮਾਪ(dst, d, sizes, 0, ਸੱਚ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_ਬਫਰ;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, ਆਕਾਰ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_ਬਫਰ == mem_type);

    size_t ਕੁੱਲ = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_ਮਾਪ, ਆਕਾਰ(size_t), &ਕੁੱਲ, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((ਪੂਰਨ)ਕਦਮ >= ਖਾਨੇ_ਗਿਣਤੀ * CV_ELEM_ਮਾਪ(type));
    CV_Assert(ਕੁੱਲ >= ਪੰਕਤੀਆਂ * ਕਦਮ);

    // attach clBuffer to UMatData
    dst.u = ਨਵਾਂ UMat_ਅੰਕੜੇ(getOpenCLAllocator());
    dst.u->ਅੰਕੜੇ            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_ਬਫਰ;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_ਅੰਕੜੇ::ਸਮ੍ਰਿਤੀ_Flag>(0);
    dst.u->handle          = cl_mem_ਬਫਰ;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = ਕੁੱਲ;

    finalizeHdr(dst);
    dst.addref();

    ਵਾਪਸ;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
ਖਾਲੀ convertFromImage(ਖਾਲੀ* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, ਆਕਾਰ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, ਆਕਾਰ(cl_image_format), &fmt, 0));

    ਪੂਰਨ ਡੂੰਘਾਈ = CV_8U;
    ਚੋਣ (fmt.image_channel_ਅੰਕੜੇ_type)
    {
    ਹਾਲਤ CL_UNORM_INT8:
    ਹਾਲਤ CL_UNSIGNED_INT8:
        ਡੂੰਘਾਈ = CV_8U;
        ਰੋਕੋ;

    ਹਾਲਤ CL_SNORM_INT8:
    ਹਾਲਤ CL_SIGNED_INT8:
        ਡੂੰਘਾਈ = CV_8S;
        ਰੋਕੋ;

    ਹਾਲਤ CL_UNORM_INT16:
    ਹਾਲਤ CL_UNSIGNED_INT16:
        ਡੂੰਘਾਈ = CV_16U;
        ਰੋਕੋ;

    ਹਾਲਤ CL_SNORM_INT16:
    ਹਾਲਤ CL_SIGNED_INT16:
        ਡੂੰਘਾਈ = CV_16S;
        ਰੋਕੋ;

    ਹਾਲਤ CL_SIGNED_INT32:
        ਡੂੰਘਾਈ = CV_32S;
        ਰੋਕੋ;

    ਹਾਲਤ CL_FLOAT:
        ਡੂੰਘਾਈ = CV_32F;
        ਰੋਕੋ;

    ਹਾਲਤ CL_HALF_FLOAT:
        ਡੂੰਘਾਈ = CV_16F;
        ਰੋਕੋ;

    ਮੂਲ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    ਪੂਰਨ type = CV_8UC1;
    ਚੋਣ (fmt.image_channel_ਕ੍ਰਮ)
    {
    ਹਾਲਤ CL_R:
    ਹਾਲਤ CL_A:
    ਹਾਲਤ CL_INTENSITY:
    ਹਾਲਤ CL_LUMINANCE:
        type = CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1);
        ਰੋਕੋ;

    ਹਾਲਤ CL_RG:
    ਹਾਲਤ CL_RA:
        type = CV_MAKE_TYPE(ਡੂੰਘਾਈ, 2);
        ਰੋਕੋ;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    ਹਾਲਤ CL_RGBA:
    ਹਾਲਤ CL_BGRA:
    ਹਾਲਤ CL_ARGB:
        type = CV_MAKE_TYPE(ਡੂੰਘਾਈ, 4);
        ਰੋਕੋ;

    ਮੂਲ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ਰੋਕੋ;
    }

    size_t ਕਦਮ = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ਪੰਕਤੀ_PITCH, ਆਕਾਰ(size_t), &ਕਦਮ, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ਚੌੜਾਈ, ਆਕਾਰ(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ਉਚਾਈ, ਆਕਾਰ(size_t), &h, 0));

    dst.create((ਪੂਰਨ)h, (ਪੂਰਨ)w, type);

    cl_mem cl_ਬਫਰ = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, src_origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ));

    CV_OCL_CHECK(cl_ਅੰਤ(q));

    ਵਾਪਸ;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ਸਥਿਰ ਖਾਲੀ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint ਅੰਕ_Devices = 0;
    cl_int ਸਥਿਤੀ = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ਨਿਰਾ, &ਅੰਕ_Devices);
    ਜੇ (ਸਥਿਤੀ != CL_DEVICE_NOT_ਮਿਲਿਆ) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(ਸਥਿਤੀ,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &ਅੰਕ_Devices).c_str());
    }

    ਜੇ (ਅੰਕ_Devices == 0)
    {
        devices.clear();
        ਵਾਪਸ;
    }

    devices.resize((size_t)ਅੰਕ_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, ਅੰਕ_Devices, &devices[0], &ਅੰਕ_Devices));
}

ਬਣਤਰ PlatformInfo::Impl
{
    Impl(ਖਾਲੀ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        ਸੰਸਕਰਣ_3 = getStrProp(CL_PLATFORM_ਸੰਸਕਰਣ);
        parseOpenCLVersion(ਸੰਸਕਰਣ_3, ਸੰਸਕਰਣ_Major, ਸੰਸਕਰਣ_Minor);
    }

    String getStrProp(cl_platform_info prop) ਅਚਲ
    {
        ਅੱਖਰ buf[1024];
        size_t sz=0;
        ਵਾਪਸ clGetPlatformInfo(handle, prop, ਆਕਾਰ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ਆਕਾਰ(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String ਸੰਸਕਰਣ_3;
    ਪੂਰਨ ਸੰਸਕਰਣ_Major;
    ਪੂਰਨ ਸੰਸਕਰਣ_Minor;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(ਖਾਲੀ* platform_id)
{
    p = ਨਵਾਂ Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    ਜੇ(p)
        p->release();
}

PlatformInfo::PlatformInfo(ਅਚਲ PlatformInfo& i)
{
    ਜੇ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::ਸੰਚਾਲਕ =(ਅਚਲ PlatformInfo& i)
{
    ਜੇ (i.p != p)
    {
        ਜੇ (i.p)
            i.p->addref();
        ਜੇ (p)
            p->release();
        p = i.p;
    }
    ਵਾਪਸ *ਇਹ;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = ਖਾਲੀ_ਸੂਚਕ;
}

PlatformInfo& PlatformInfo::ਸੰਚਾਲਕ = (PlatformInfo&& i) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &i) {
        ਜੇ(p)
            p->release();
        p = i.p;
        i.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

ਪੂਰਨ PlatformInfo::device_ਸੰਖਿਆ() ਅਚਲ
{
    ਵਾਪਸ p ? (ਪੂਰਨ)p->devices.size() : 0;
}

ਖਾਲੀ PlatformInfo::getDevice(Device& device, ਪੂਰਨ d) ਅਚਲ
{
    CV_Assert(p && d < (ਪੂਰਨ)p->devices.size() );
    ਜੇ(p)
        device.ਸੈੱਟ(p->devices[d]);
}

String PlatformInfo::name() ਅਚਲ
{
    ਵਾਪਸ p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ਅਚਲ
{
    ਵਾਪਸ p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::ਸੰਸਕਰਣ() ਅਚਲ
{
    ਵਾਪਸ p ? p->ਸੰਸਕਰਣ_3 : String();
}

ਪੂਰਨ PlatformInfo::ਸੰਸਕਰਣ_Major_2() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->ਸੰਸਕਰਣ_Major;
}

ਪੂਰਨ PlatformInfo::ਸੰਸਕਰਣ_Minor_2() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->ਸੰਸਕਰਣ_Minor;
}

ਸਥਿਰ ਖਾਲੀ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint ਅੰਕ_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms));

    ਜੇ (ਅੰਕ_Platforms == 0)
    {
        platforms.clear();
        ਵਾਪਸ;
    }

    platforms.resize((size_t)ਅੰਕ_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], &ਅੰਕ_Platforms));
}

ਖਾਲੀ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    ਲਈ (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((ਖਾਲੀ*)&platforms[i]) );
}

ਅਚਲ ਅੱਖਰ* type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ type)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* tab[CV_ਡੂੰਘਾਈ_MAX*16]=
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
    ਪੂਰਨ cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਅਚਲ ਅੱਖਰ* ਨਤੀਜਾ = cn > 16 ? ਖਾਲੀ_ਸੂਚਕ : tab[ਡੂੰਘਾਈ*16 + cn-1];
    CV_Assert(ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਅਚਲ ਅੱਖਰ* memop_Type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ type)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* tab[CV_ਡੂੰਘਾਈ_MAX*16] =
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
    ਪੂਰਨ cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਅਚਲ ਅੱਖਰ* ਨਤੀਜਾ = cn > 16 ? ਖਾਲੀ_ਸੂਚਕ : tab[ਡੂੰਘਾਈ*16 + cn-1];
    CV_Assert(ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਅਚਲ ਅੱਖਰ* vecop_Type_ਮੰਜ਼ਿਲ_Str(ਪੂਰਨ type)
{
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* tab[CV_ਡੂੰਘਾਈ_MAX*16] =
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
    ਪੂਰਨ cn = CV_MAT_CN(type), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    ਅਚਲ ਅੱਖਰ* ਨਤੀਜਾ = cn > 16 ? 0 : tab[ਡੂੰਘਾਈ*16 + cn-1];
    CV_Assert(ਨਤੀਜਾ);
    ਵਾਪਸ ਨਤੀਜਾ;
}

// Deprecated due to size of buf buffer being unknowable.
ਅਚਲ ਅੱਖਰ* convertTypeStr(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਪੂਰਨ cn, ਅੱਖਰ* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ਵਾਪਸ convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ਅਚਲ ਅੱਖਰ* convertTypeStr(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth, ਪੂਰਨ cn, ਅੱਖਰ* buf, size_t buf_ਮਾਪ_2)
{
    ਜੇ( sdepth == ddepth )
        ਵਾਪਸ "noconvert";
    ਅਚਲ ਅੱਖਰ *typestr = type_ਮੰਜ਼ਿਲ_Str(CV_MAKETYPE(ddepth, cn));
    ਜੇ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_ਮਾਪ_2, "convert_%s", typestr);
    }
    ਨਹੀਂਤਾਂ ਜੇ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_ਮਾਪ_2, "convert_%s_sat_rte", typestr);
    ਨਹੀਂਤਾਂ
        snprintf(buf, buf_ਮਾਪ_2, "convert_%s_sat", typestr);

    ਵਾਪਸ buf;
}

ਅਚਲ ਅੱਖਰ* getOpenCLErrorString(ਪੂਰਨ error_ਕੋਡ)
{
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_ਕੋਡ(id) ਹਾਲਤ id: ਵਾਪਸ #id
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_OCL_ਕੋਡ_2(id, name) ਹਾਲਤ id: ਵਾਪਸ #name
    ਚੋਣ (error_ਕੋਡ)
    {
    CV_OCL_ਕੋਡ(CL_SUCCESS);
    CV_OCL_ਕੋਡ(CL_DEVICE_NOT_ਮਿਲਿਆ);
    CV_OCL_ਕੋਡ(CL_DEVICE_NOT_ਉਪਲਬਧ);
    CV_OCL_ਕੋਡ(CL_COMPILER_NOT_ਉਪਲਬਧ);
    CV_OCL_ਕੋਡ(CL_MEM_OBJECT_ਵੰਡ_FAILURE);
    CV_OCL_ਕੋਡ(CL_ਬਾਹਰਲਾ_ਦਾ_RESOURCES);
    CV_OCL_ਕੋਡ(CL_ਬਾਹਰਲਾ_ਦਾ_HOST_ਸਮ੍ਰਿਤੀ);
    CV_OCL_ਕੋਡ(CL_PROFILING_INFO_NOT_ਉਪਲਬਧ);
    CV_OCL_ਕੋਡ(CL_MEM_COPY_OVERLAP);
    CV_OCL_ਕੋਡ(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_ਕੋਡ(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_ਕੋਡ(CL_BUILD_ਪ੍ਰੋਗਰਾਮ_FAILURE);
    CV_OCL_ਕੋਡ(CL_MAP_FAILURE);
    CV_OCL_ਕੋਡ(CL_MISALIGNED_SUB_ਬਫਰ_ਵਿੱਥ);
    CV_OCL_ਕੋਡ(CL_EXEC_ਸਥਿਤੀ_ERROR_FOR_EVENTS_ਵਿੱਚ_WAIT_LIST);
    CV_OCL_ਕੋਡ(CL_COMPILE_ਪ੍ਰੋਗਰਾਮ_FAILURE);
    CV_OCL_ਕੋਡ(CL_LINKER_NOT_ਉਪਲਬਧ);
    CV_OCL_ਕੋਡ(CL_LINK_ਪ੍ਰੋਗਰਾਮ_FAILURE);
    CV_OCL_ਕੋਡ(CL_DEVICE_ਹਿੱਸੇ_ਕਰੋ_FAILED);
    CV_OCL_ਕੋਡ(CL_KERNEL_ARG_INFO_NOT_ਉਪਲਬਧ);
    CV_OCL_ਕੋਡ(CL_INVALID_ਮੁੱਲ);
    CV_OCL_ਕੋਡ(CL_INVALID_DEVICE_TYPE);
    CV_OCL_ਕੋਡ(CL_INVALID_PLATFORM);
    CV_OCL_ਕੋਡ(CL_INVALID_DEVICE);
    CV_OCL_ਕੋਡ(CL_INVALID_CONTEXT);
    CV_OCL_ਕੋਡ(CL_INVALID_ਕਤਾਰ_PROPERTIES);
    CV_OCL_ਕੋਡ(CL_INVALID_COMMAND_ਕਤਾਰ);
    CV_OCL_ਕੋਡ(CL_INVALID_HOST_PTR);
    CV_OCL_ਕੋਡ(CL_INVALID_MEM_OBJECT);
    CV_OCL_ਕੋਡ(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_ਕੋਡ(CL_INVALID_IMAGE_ਮਾਪ);
    CV_OCL_ਕੋਡ(CL_INVALID_SAMPLER);
    CV_OCL_ਕੋਡ(CL_INVALID_ਦੁਆਧਾਰੀ);
    CV_OCL_ਕੋਡ(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_ਕੋਡ(CL_INVALID_ਪ੍ਰੋਗਰਾਮ);
    CV_OCL_ਕੋਡ(CL_INVALID_ਪ੍ਰੋਗਰਾਮ_EXECUTABLE);
    CV_OCL_ਕੋਡ(CL_INVALID_KERNEL_NAME);
    CV_OCL_ਕੋਡ(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_ਕੋਡ(CL_INVALID_KERNEL);
    CV_OCL_ਕੋਡ(CL_INVALID_ARG_ਸੂਚਕਾਂਕ);
    CV_OCL_ਕੋਡ(CL_INVALID_ARG_ਮੁੱਲ);
    CV_OCL_ਕੋਡ(CL_INVALID_ARG_ਮਾਪ);
    CV_OCL_ਕੋਡ(CL_INVALID_KERNEL_ਦਲੀਲਾਂ);
    CV_OCL_ਕੋਡ(CL_INVALID_WORK_DIMENSION);
    CV_OCL_ਕੋਡ(CL_INVALID_WORK_ਸਮੂਹ_ਮਾਪ);
    CV_OCL_ਕੋਡ(CL_INVALID_WORK_ਵਸਤੂ_ਮਾਪ);
    CV_OCL_ਕੋਡ(CL_INVALID_GLOBAL_ਵਿੱਥ);
    CV_OCL_ਕੋਡ(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_ਕੋਡ(CL_INVALID_EVENT);
    CV_OCL_ਕੋਡ(CL_INVALID_OPERATION);
    CV_OCL_ਕੋਡ(CL_INVALID_GL_OBJECT);
    CV_OCL_ਕੋਡ(CL_INVALID_ਬਫਰ_ਮਾਪ);
    CV_OCL_ਕੋਡ(CL_INVALID_MIP_LEVEL);
    CV_OCL_ਕੋਡ(CL_INVALID_GLOBAL_WORK_ਮਾਪ);
    // OpenCL 1.1
    CV_OCL_ਕੋਡ(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_ਕੋਡ(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_ਕੋਡ(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_ਕੋਡ(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_ਕੋਡ(CL_INVALID_DEVICE_ਹਿੱਸੇ_ਕਰੋ_ਗਿਣਤੀ);
    // OpenCL 2.0
    CV_OCL_ਕੋਡ_2(-69, CL_INVALID_PIPE_ਮਾਪ);
    CV_OCL_ਕੋਡ_2(-70, CL_INVALID_DEVICE_ਕਤਾਰ);
    // Extensions
    CV_OCL_ਕੋਡ_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_ਕੋਡ_2(-1001, CL_PLATFORM_NOT_ਮਿਲਿਆ_KHR);
    CV_OCL_ਕੋਡ_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_ਕੋਡ_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_ਕੋਡ_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_ਕੋਡ_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    ਮੂਲ: ਵਾਪਸ "Unknown OpenCL error";
    }
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_OCL_ਕੋਡ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_OCL_ਕੋਡ_2
}

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
ਸਥਿਰ std::string ker_ਮੰਜ਼ਿਲ_Str(ਅਚਲ Mat & k)
{
    ਪੂਰਨ ਚੌੜਾਈ = k.ਖਾਨੇ_ਗਿਣਤੀ - 1, ਡੂੰਘਾਈ = k.ਡੂੰਘਾਈ();
    ਅਚਲ T * ਅਚਲ ਅੰਕੜੇ = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    ਜੇ (ਡੂੰਘਾਈ <= CV_8S)
    {
        ਲਈ (ਪੂਰਨ i = 0; i < ਚੌੜਾਈ; ++i)
            stream << "DIG(" << (ਪੂਰਨ)ਅੰਕੜੇ[i] << ")";
        stream << "DIG(" << (ਪੂਰਨ)ਅੰਕੜੇ[ਚੌੜਾਈ] << ")";
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        ਲਈ (ਪੂਰਨ i = 0; i < ਚੌੜਾਈ; ++i)
            stream << "DIG(" << ਅੰਕੜੇ[i] << "f)";
        stream << "DIG(" << ਅੰਕੜੇ[ਚੌੜਾਈ] << "f)";
    }
    ਨਹੀਂਤਾਂ ਜੇ (ਡੂੰਘਾਈ == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        ਲਈ (ਪੂਰਨ i = 0; i < ਚੌੜਾਈ; ++i)
            stream << "DIG(" << (ਦਸ਼ਮਲਵ)ਅੰਕੜੇ[i] << "h)";
        stream << "DIG(" << (ਦਸ਼ਮਲਵ)ਅੰਕੜੇ[ਚੌੜਾਈ] << "h)";
    }
    ਨਹੀਂਤਾਂ
    {
        ਲਈ (ਪੂਰਨ i = 0; i < ਚੌੜਾਈ; ++i)
            stream << "DIG(" << ਅੰਕੜੇ[i] << ")";
        stream << "DIG(" << ਅੰਕੜੇ[ਚੌੜਾਈ] << ")";
    }

    ਵਾਪਸ stream.str();
}

String kernel_ਮੰਜ਼ਿਲ_Str(ਨਿਵੇਸ਼_Array _kernel, ਪੂਰਨ ddepth, ਅਚਲ ਅੱਖਰ * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    ਪੂਰਨ ਡੂੰਘਾਈ = kernel.ਡੂੰਘਾਈ();
    ਜੇ (ddepth < 0)
        ddepth = ਡੂੰਘਾਈ;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    ਜੇ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    ਜੇ (ddepth != ਡੂੰਘਾਈ)
        kernel.convert_ਮੰਜ਼ਿਲ(kernel, ddepth);

    ਕਿਸਮਪਰਿਭਾਸ਼ਾ std::string (* func_t)(ਅਚਲ Mat &);
    ਸਥਿਰ ਅਚਲ func_t funcs[CV_ਡੂੰਘਾਈ_MAX] = { ker_ਮੰਜ਼ਿਲ_Str<uchar>, ker_ਮੰਜ਼ਿਲ_Str<ਅੱਖਰ>, ker_ਮੰਜ਼ਿਲ_Str<ushort>, ker_ਮੰਜ਼ਿਲ_Str<ਛੋਟਾ>,
                                                ker_ਮੰਜ਼ਿਲ_Str<ਪੂਰਨ>, ker_ਮੰਜ਼ਿਲ_Str<ਦਸ਼ਮਲਵ>, ker_ਮੰਜ਼ਿਲ_Str<ਦੁੱਗਣਾ>, ker_ਮੰਜ਼ਿਲ_Str<hfloat>,
                                                0, ker_ਮੰਜ਼ਿਲ_Str<ਤਾਰਕਿਕ>, ker_ਮੰਜ਼ਿਲ_Str<uint64_t>, ker_ਮੰਜ਼ਿਲ_Str<int64_t>, ker_ਮੰਜ਼ਿਲ_Str<ਬਿਨਾਨਿਸ਼ਾਨੀ> };
    ਅਚਲ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ਵਾਪਸ cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PROCESS_SRC(src) \
    ਕਰੋ \
    { \
        ਜੇ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            ਮਾਪ csize = src.size(); \
            ਪੂਰਨ ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_ਡੂੰਘਾਈ(ctype), \
                ckercn = ਸਦਿਸ਼_Widths[cdepth], cwidth = ccn * csize.ਚੌੜਾਈ; \
            ਜੇ (cwidth < ckercn || ckercn <= 0) \
                ਵਾਪਸ 1; \
            ਖਾਨੇ_ਗਿਣਤੀ.push_back(cwidth); \
            ਜੇ (strat == OCL_ਸਦਿਸ਼_OWN && ctype != ref_type) \
                ਵਾਪਸ 1; \
            offsets.push_back(src.offset()); \
            ਕਦਮਾਂ.push_back(src.ਕਦਮ()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    ਜਦੋਂ ((ਖਾਲੀ)0, 0)

ਪੂਰਨ predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, ਨਿਵੇਸ਼_Array src3,
                              ਨਿਵੇਸ਼_Array src4, ਨਿਵੇਸ਼_Array src5, ਨਿਵੇਸ਼_Array src6,
                              ਨਿਵੇਸ਼_Array src7, ਨਿਵੇਸ਼_Array src8, ਨਿਵੇਸ਼_Array src9,
                              Ocl_ਸਦਿਸ਼_Strategy strat)
{
    ਅਚਲ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    ਪੂਰਨ ਸਦਿਸ਼_Widths[CV_ਡੂੰਘਾਈ_MAX] = {
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Char(), d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Char(),   // CV_8U,  CV_8S
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Short(), d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Short(), // CV_16U, CV_16S
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Int(), d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Float(),   // CV_32S, CV_32F
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Double(), d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Char(),                                 // CV_Bool
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Long(), d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Long(),   // CV_64U, CV_64S
        d.preferred_ਸਦਿਸ਼_ਚੌੜਾਈ_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    ਜੇ (ਸਦਿਸ਼_Widths[0] == 1)
    {
        // it's heuristic
        ਸਦਿਸ਼_Widths[CV_8U] = ਸਦਿਸ਼_Widths[CV_8S] = 4;
        ਸਦਿਸ਼_Widths[CV_16U] = ਸਦਿਸ਼_Widths[CV_16S] = ਸਦਿਸ਼_Widths[CV_16F] = 2;
        ਸਦਿਸ਼_Widths[CV_32S] = ਸਦਿਸ਼_Widths[CV_32F] = ਸਦਿਸ਼_Widths[CV_64F] = 1;
    }

    ਵਾਪਸ check_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(ਸਦਿਸ਼_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

ਪੂਰਨ check_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ([[maybe_unused]] ਅਚਲ ਪੂਰਨ *ਸਦਿਸ਼_Widths,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src1,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src2,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src3,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src4,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src5,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src6,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src7,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src8,
                            [[maybe_unused]] ਨਿਵੇਸ਼_Array src9,
                            [[maybe_unused]] Ocl_ਸਦਿਸ਼_Strategy strat)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __APPLE__
    ਵਾਪਸ 1;
#ਨਹੀਂਤਾਂ
    CV_Assert(ਸਦਿਸ਼_Widths);

    ਪੂਰਨ ref_type = src1.type();

    std::vector<size_t> offsets, ਕਦਮਾਂ, ਖਾਨੇ_ਗਿਣਤੀ;
    std::vector<ਪੂਰਨ> dividers, kercns;
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

    ਲਈ (size_t i = 0; i < size; ++i)
        ਜਦੋਂ (offsets[i] % dividers[i] != 0 || ਕਦਮਾਂ[i] % dividers[i] != 0 || ਖਾਨੇ_ਗਿਣਤੀ[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    ਪੂਰਨ kercn = *std::min_element(kercns.begin(), kercns.end());

    ਵਾਪਸ kercn;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ_Max(ਨਿਵੇਸ਼_Array src1, ਨਿਵੇਸ਼_Array src2, ਨਿਵੇਸ਼_Array src3,
                                 ਨਿਵੇਸ਼_Array src4, ਨਿਵੇਸ਼_Array src5, ਨਿਵੇਸ਼_Array src6,
                                 ਨਿਵੇਸ਼_Array src7, ਨਿਵੇਸ਼_Array src8, ਨਿਵੇਸ਼_Array src9)
{
    ਵਾਪਸ predict_Optimal_ਸਦਿਸ਼_ਚੌੜਾਈ(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_ਸਦਿਸ਼_MAX);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
ਖਾਲੀ build_Options_ਜੋੜੋ_ਮੈਟ੍ਰਿਕਸ_Description(String& buildOptions, ਅਚਲ String& name, ਨਿਵੇਸ਼_Array _m)
{
    ਜੇ (!buildOptions.empty())
        buildOptions += " ";
    ਪੂਰਨ type = _m.type(), ਡੂੰਘਾਈ = CV_MAT_ਡੂੰਘਾਈ(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_ਮੰਜ਼ਿਲ_Str(type),
            name.c_str(), ocl::type_ਮੰਜ਼ਿਲ_Str(CV_MAKE_TYPE(ਡੂੰਘਾਈ, 1)),
            name.c_str(), (ਪੂਰਨ)CV_MAT_CN(type),
            name.c_str(), (ਪੂਰਨ)CV_ELEM_ਮਾਪ(type),
            name.c_str(), (ਪੂਰਨ)CV_ELEM_SIZE1(type),
            name.c_str(), (ਪੂਰਨ)ਡੂੰਘਾਈ
            );
}


ਬਣਤਰ Image2D::Impl
{
    Impl(ਅਚਲ UMat &src, ਤਾਰਕਿਕ norm, ਤਾਰਕਿਕ alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        ਜੇ (handle)
            clReleaseMemObject(handle);
    }

    ਸਥਿਰ cl_image_format getImageFormat(ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ cn, ਤਾਰਕਿਕ norm)
    {
        cl_image_format format;
        ਸਥਿਰ ਅਚਲ ਪੂਰਨ channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ਸਥਿਰ ਅਚਲ ਪੂਰਨ channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ਸਥਿਰ ਅਚਲ ਪੂਰਨ channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        ਪੂਰਨ channelType = norm ? channelTypesNorm[ਡੂੰਘਾਈ] : channelTypes[ਡੂੰਘਾਈ];
        ਪੂਰਨ channel_ਕ੍ਰਮ = channelOrders[cn];
        format.image_channel_ਅੰਕੜੇ_type = (cl_channel_type)channelType;
        format.image_channel_ਕ੍ਰਮ = (cl_channel_ਕ੍ਰਮ)channel_ਕ੍ਰਮ;
        ਵਾਪਸ format;
    }

    ਸਥਿਰ ਤਾਰਕਿਕ isFormatSupported(cl_image_format format)
    {
        ਜੇ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        ਜੇ (!context)
            ਵਾਪਸ ਝੂਠ;

        // Figure out how many formats are supported by this context.
        cl_uint ਅੰਕ_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, ਅੰਕ_Formats,
                                                ਨਿਰਾ, &ਅੰਕ_Formats);
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        ਜੇ (ਅੰਕ_Formats > 0)
        {
            Auto_ਬਫਰ<cl_image_format> formats(ਅੰਕ_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, ਅੰਕ_Formats,
                                             formats.ਅੰਕੜੇ(), ਨਿਰਾ);
            CV_OCL_DBG_CHECK_ਨਤੀਜਾ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            ਲਈ (cl_uint i = 0; i < ਅੰਕ_Formats; ++i)
            {
                ਜੇ (!memcmp(&formats[i], &format, ਆਕਾਰ(format)))
                {
                    ਵਾਪਸ ਸੱਚ;
                }
            }
        }
        ਵਾਪਸ ਝੂਠ;
    }

    ਖਾਲੀ init(ਅਚਲ UMat &src, ਤਾਰਕਿਕ norm, ਤਾਰਕਿਕ alias)
    {
        ਜੇ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        ਪੂਰਨ err, ਡੂੰਘਾਈ = src.ਡੂੰਘਾਈ(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(ਡੂੰਘਾਈ, cn, norm);

        ਜੇ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        ਜੇ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_ਕਤਾਰ ਕਤਾਰ_2 = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CL_ਸੰਸਕਰਣ_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ਅਚਲ Device & d = ocl::Device::getDefault();
        ਪੂਰਨ minor = d.device_ਸੰਸਕਰਣ_Minor(), major = d.device_ਸੰਸਕਰਣ_Major();
        CV_Assert(!alias || canCreateAlias(src));
        ਜੇ (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_ਚੌੜਾਈ      = src.ਖਾਨੇ_ਗਿਣਤੀ;
            desc.image_ਉਚਾਈ     = src.ਪੰਕਤੀਆਂ;
            desc.image_ਡੂੰਘਾਈ      = 0;
            desc.image_array_ਮਾਪ = 1;
            desc.image_ਪੰਕਤੀ_pitch  = alias ? src.ਕਦਮ[0] : 0;
            desc.image_ਟੁਕੜਾ_pitch = 0;
            desc.ਬਫਰ           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.ਅੰਕ_mip_ਪੱਧਰ   = 0;
            desc.ਅੰਕ_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ਨਿਰਾ, &err);
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        {
            CV_SUPPRESS_DEPRECATED_ਸ਼ੁਰੂ
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.ਖਾਨੇ_ਗਿਣਤੀ, src.ਪੰਕਤੀਆਂ, 0, ਨਿਰਾ, &err);
            CV_SUPPRESS_DEPRECATED_ਸਿਰਾ
        }
        CV_OCL_DBG_CHECK_ਨਤੀਜਾ(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.ਖਾਨੇ_ਗਿਣਤੀ), static_cast<size_t>(src.ਪੰਕਤੀਆਂ), 1 };

        cl_mem dev_ਅੰਕੜੇ;
        ਜੇ (!alias && !src.isContinuous())
        {
            dev_ਅੰਕੜੇ = cl_Create_ਬਫਰ(context, CL_MEM_READ_ONLY, src.ਖਾਨੇ_ਗਿਣਤੀ * src.ਪੰਕਤੀਆਂ * src.elem_ਮਾਪ(), ਨਿਰਾ, &err);
            CV_OCL_CHECK_ਨਤੀਜਾ(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (ਲੰਮਾ ਲੰਮਾ ਪੂਰਨ)(src.ਖਾਨੇ_ਗਿਣਤੀ * src.ਪੰਕਤੀਆਂ * src.elem_ਮਾਪ()), (ਖਾਲੀ*)dev_ਅੰਕੜੇ
                ).c_str());

            ਅਚਲ size_t roi[3] = {static_cast<size_t>(src.ਖਾਨੇ_ਗਿਣਤੀ) * src.elem_ਮਾਪ(), static_cast<size_t>(src.ਪੰਕਤੀਆਂ), 1};
            CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_Rect(ਕਤਾਰ_2, (cl_mem)src.handle(ACCESS_READ), dev_ਅੰਕੜੇ, origin, origin,
                roi, src.ਕਦਮ, 0, src.ਖਾਨੇ_ਗਿਣਤੀ * src.elem_ਮਾਪ(), 0, 0, ਨਿਰਾ, ਨਿਰਾ));
            CV_OCL_DBG_CHECK(clFlush(ਕਤਾਰ_2));
        }
        ਨਹੀਂਤਾਂ
        {
            dev_ਅੰਕੜੇ = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_ਅੰਕੜੇ != ਨਿਰਾ);

        ਜੇ (!alias)
        {
            CV_OCL_CHECK(cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(ਕਤਾਰ_2, dev_ਅੰਕੜੇ, handle, 0, origin, region, 0, ਨਿਰਾ, 0));
            ਜੇ (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(ਕਤਾਰ_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_ਅੰਕੜੇ));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = ਨਿਰਾ;
}

Image2D::Image2D(ਅਚਲ UMat &src, ਤਾਰਕਿਕ norm, ਤਾਰਕਿਕ alias)
{
    p = ਨਵਾਂ Impl(src, norm, alias);
}

ਤਾਰਕਿਕ Image2D::canCreateAlias(ਅਚਲ UMat &m)
{
    ਤਾਰਕਿਕ ret = ਝੂਠ;
    ਅਚਲ Device & d = ocl::Device::getDefault();
    ਜੇ (d.image_From_ਬਫਰ_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        ਜੇ (pitchAlign && !(m.ਕਦਮ % (pitchAlign * m.elem_ਮਾਪ())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            ਜੇ (!m.u->ਆਰਜ਼ੀ_UMat())
            {
                ret = ਸੱਚ;
            }
        }
    }
    ਵਾਪਸ ret;
}

ਤਾਰਕਿਕ Image2D::isFormatSupported(ਪੂਰਨ ਡੂੰਘਾਈ, ਪੂਰਨ cn, ਤਾਰਕਿਕ norm)
{
    cl_image_format format = Impl::getImageFormat(ਡੂੰਘਾਈ, cn, norm);

    ਵਾਪਸ Impl::isFormatSupported(format);
}

Image2D::Image2D(ਅਚਲ Image2D & i)
{
    p = i.p;
    ਜੇ (p)
        p->addref();
}

Image2D & Image2D::ਸੰਚਾਲਕ = (ਅਚਲ Image2D & i)
{
    ਜੇ (i.p != p)
    {
        ਜੇ (i.p)
            i.p->addref();
        ਜੇ (p)
            p->release();
        p = i.p;
    }
    ਵਾਪਸ *ਇਹ;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = ਖਾਲੀ_ਸੂਚਕ;
}

Image2D& Image2D::ਸੰਚਾਲਕ = (Image2D&& i) CV_NOEXCEPT
{
    ਜੇ (ਇਹ != &i) {
        ਜੇ (p)
            p->release();
        p = i.p;
        i.p = ਖਾਲੀ_ਸੂਚਕ;
    }
    ਵਾਪਸ *ਇਹ;
}

Image2D::~Image2D()
{
    ਜੇ (p)
        p->release();
}

ਖਾਲੀ* Image2D::ptr() ਅਚਲ
{
    ਵਾਪਸ p ? p->handle : 0;
}

ਤਾਰਕਿਕ internal::isOpenCLForced()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ value = ਝੂਠ;
    ਜੇ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ value;
}

ਤਾਰਕਿਕ internal::isPerformanceCheckBypassed()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਤਾਰਕਿਕ value = ਝੂਠ;
    ਜੇ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", ਝੂਠ);
        initialized = ਸੱਚ;
    }
    ਵਾਪਸ value;
}

ਤਾਰਕਿਕ internal::isCLBuffer(UMat& u)
{
    ਜੇ (isCudaBacked(u.u))
        ਵਾਪਸ ਝੂਠ;
    ਖਾਲੀ* h = u.handle(ACCESS_RW);
    ਜੇ (!h)
        ਵਾਪਸ ਸੱਚ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#ਜੇ 1
    ਜੇ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ਵਾਪਸ ਝੂਠ;
#ਨਹੀਂਤਾਂ
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, ਆਕਾਰ(type), &type, ਨਿਰਾ);
    ਜੇ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_ਬਫਰ)
        ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ ਸੱਚ;
}

ਬਣਤਰ Timer::Impl
{
    ਅਚਲ ਕਤਾਰ ਕਤਾਰ_2;

    Impl(ਅਚਲ ਕਤਾਰ& q)
        : ਕਤਾਰ_2(q)
    {
    }

    ~Impl(){}

    ਖਾਲੀ ਸ਼ੁਰੂ()
    {
        CV_OCL_DBG_CHECK(cl_ਅੰਤ((cl_command_ਕਤਾਰ)ਕਤਾਰ_2.ptr()));
        timer.ਸ਼ੁਰੂ();
    }

    ਖਾਲੀ stop()
    {
        CV_OCL_DBG_CHECK(cl_ਅੰਤ((cl_command_ਕਤਾਰ)ਕਤਾਰ_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ਅਚਲ
    {
        ਵਾਪਸ (uint64)(timer.get_ਸਮਾਂ_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ਅਚਲ ਕਤਾਰ& q) : p(ਨਵਾਂ Impl(q)) { }
Timer::~Timer() { ਮਿਟਾਓ p; }

ਖਾਲੀ Timer::ਸ਼ੁਰੂ()
{
    CV_Assert(p);
    p->ਸ਼ੁਰੂ();
}

ਖਾਲੀ Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ਅਚਲ
{
    CV_Assert(p);
    ਵਾਪਸ p->durationNS();
}

}} // namespace

#ਜੇ_ਅੰਤ // HAVE_OPENCL
