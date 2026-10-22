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

#ifndef HAVE_OPENCL
#ಅನ್ತರ್ಭಾವ "ocl_disabled.impl.hpp"
#ಅನ್ಯಥಾ // HAVE_OPENCL

#ಅನ್ತರ್ಭಾವ <list>
#ಅನ್ತರ್ಭಾವ <map>
#ಅನ್ತರ್ಭಾವ <deque>
#ಅನ್ತರ್ಭಾವ <set>
#ಅನ್ತರ್ಭಾವ <string>
#ಅನ್ತರ್ಭಾವ <sstream>
#ಅನ್ತರ್ಭಾವ <fstream>
#ಯದಿ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#endif

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl_genbase.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/lock.private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "OpenCL allocator: " << __VA_ARGS__)
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#ಅನ್ತರ್ಭಾವ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# ಯದಿ 0
#   define LOG_BUFFER_POOL printf
# ಅನ್ಯಥಾ
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#ಯದಿ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#ಅನ್ಯಥಾ
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#ಯದಿ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#ಅನ್ಯಥಾ
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#ಅನ್ತರ್ಭಾವ "umatrix.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    ಶೂನ್ಯ addref() { CV_XADD(&refcount, 1); } \
    ಶೂನ್ಯ release() { ಯದಿ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete ಏತತ್; } \
    ಪೂರ್ಣಾಙ್ಕ refcount

ಸ್ಥಿರ cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ಪ್ರತ್ಯಾಗಮ opencl_allocator_stats;
}

#ifndef _DEBUG
ಸ್ಥಿರ ತಾರ್ಕಿಕ isRaiseError()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ value;
}
#endif

ಸ್ಥಿರ ಶೂನ್ಯ onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", ಅಸತ್ಯ);
    ಯದಿ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#ಯದಿ CV_OPENCL_TRACE_CHECK
ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ
ಶೂನ್ಯ traceOpenCLCheck(cl_int status, ಧ್ರುವ ವರ್ಣ* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#ಅನ್ಯಥಾ
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_ಸಂದೇಶ(check_result, ಸಂದೇಶ) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, ಸಂದೇಶ)

#define CV_OCL_CHECK_RESULT(check_result, ಸಂದೇಶ) \
    ಕುರು { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, ಸಂದೇಶ); \
        ಯದಿ (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(ಸಂದೇಶ), ಧ್ರುವ ವರ್ಣ*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_ಸಂದೇಶ = CV_OCL_API_ERROR_ಸಂದೇಶ(check_result, ಸಂದೇಶ); \
            CV_Error(Error::OpenCLApiCallError, error_ಸಂದೇಶ); \
        } \
    } ಯಾವತ್ (0)

#define CV_OCL_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_result) ಕುರು { ಅಭಿವ್ಯಕ್ತಿ; CV_OCL_CHECK_RESULT(check_result, #expr); } ಯಾವತ್ (0)

#define CV_OCL_CHECK(ಅಭಿವ್ಯಕ್ತಿ) ಕುರು { cl_int __cl_result = (ಅಭಿವ್ಯಕ್ತಿ); CV_OCL_CHECK_RESULT(__cl_result, #expr); } ಯಾವತ್ (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, ಸಂದೇಶ) CV_OCL_CHECK_RESULT(check_result, ಸಂದೇಶ)
#define CV_OCL_DBG_CHECK(ಅಭಿವ್ಯಕ್ತಿ) CV_OCL_CHECK(ಅಭಿವ್ಯಕ್ತಿ)
#define CV_OCL_DBG_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_result) CV_OCL_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_result)
#ಅನ್ಯಥಾ
#define CV_OCL_DBG_CHECK_RESULT(check_result, ಸಂದೇಶ) \
    ಕುರು { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, ಸಂದೇಶ); \
        ಯದಿ (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(ಸಂದೇಶ), ಧ್ರುವ ವರ್ಣ*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_ಸಂದೇಶ = CV_OCL_API_ERROR_ಸಂದೇಶ(check_result, ಸಂದೇಶ); \
            CV_Error(Error::OpenCLApiCallError, error_ಸಂದೇಶ); \
        } \
    } ಯಾವತ್ (0)
#define CV_OCL_DBG_CHECK_(ಅಭಿವ್ಯಕ್ತಿ, check_result) ಕುರು { ಅಭಿವ್ಯಕ್ತಿ; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } ಯಾವತ್ (0)
#define CV_OCL_DBG_CHECK(ಅಭಿವ್ಯಕ್ತಿ) ಕುರು { cl_int __cl_result = (ಅಭಿವ್ಯಕ್ತಿ); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } ಯಾವತ್ (0)
#endif


ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", ಸತ್ಯ);
ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", ಸತ್ಯ);
ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", ಸತ್ಯ);
ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", ಸತ್ಯ);

#ಯದಿ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", ಅಸತ್ಯ);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#ಯದಿ 1 //def __APPLE__
        ಸತ್ಯ
#ಅನ್ಯಥಾ
        ಅಸತ್ಯ
#endif
);

ಸ್ಥಿರ String getBuildExtraOptions()
{
    ಸ್ಥಿರ String param_buildExtraOptions;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = ಸತ್ಯ;
        ಯದಿ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ರಿಕ್ತ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ಪ್ರತ್ಯಾಗಮ param_buildExtraOptions;
}

ಸ್ಥಿರ ಧ್ರುವ ತಾರ್ಕಿಕ CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", ಸತ್ಯ);
ಸ್ಥಿರ ಧ್ರುವ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ಸಂರಚನಾ UMat2D
{
    UMat2D(ಧ್ರುವ UMat& m)
    {
        offset = (ಪೂರ್ಣಾಙ್ಕ)m.offset;
        step = (ಪೂರ್ಣಾಙ್ಕ)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    ಪೂರ್ಣಾಙ್ಕ offset;
    ಪೂರ್ಣಾಙ್ಕ step;
    ಪೂರ್ಣಾಙ್ಕ rows;
    ಪೂರ್ಣಾಙ್ಕ cols;
};

ಸಂರಚನಾ UMat3D
{
    UMat3D(ಧ್ರುವ UMat& m)
    {
        offset = (ಪೂರ್ಣಾಙ್ಕ)m.offset;
        step = (ಪೂರ್ಣಾಙ್ಕ)m.step.p[1];
        slicestep = (ಪೂರ್ಣಾಙ್ಕ)m.step.p[0];
        slices = (ಪೂರ್ಣಾಙ್ಕ)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    ಪೂರ್ಣಾಙ್ಕ offset;
    ಪೂರ್ಣಾಙ್ಕ slicestep;
    ಪೂರ್ಣಾಙ್ಕ step;
    ಪೂರ್ಣಾಙ್ಕ slices;
    ಪೂರ್ಣಾಙ್ಕ rows;
    ಪೂರ್ಣಾಙ್ಕ cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ಸ್ಥಿರ uint64 crc64( ಧ್ರುವ uchar* data, size_t size, uint64 crc0=0 )
{
    ಸ್ಥಿರ uint64 table[256];
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;

    ಯದಿ( !initialized )
    {
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < 256; i++ )
        {
            uint64 c = i;
            ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = ಸತ್ಯ;
    }

    uint64 crc = ~crc0;
    ಪ್ರತಿ( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    ಪ್ರತ್ಯಾಗಮ ~crc;
}

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
ಸಂರಚನಾ OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    ಪ್ರಕಾರನಿರ್ದೇಶ std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "Initializing OpenCL cache configuration...");
        ಯದಿ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL cache is disabled");
            ಪ್ರತ್ಯಾಗಮ;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        ಯದಿ (cache_path_.empty())
        {
            CV_LOG_INFO(ರಿಕ್ತ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        ಕುರು
        {
            ಯತ್ನ
            {
                ಯದಿ (cache_path_.empty())
                    ಭಙ್ಗ;
                ಯದಿ (cache_path_ == "disabled")
                    ಭಙ್ಗ;
                ಯದಿ (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(ರಿಕ್ತ, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    ಭಙ್ಗ;
                }

                ಯದಿ (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    ಯದಿ (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(ರಿಕ್ತ, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        ಯದಿ (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(ರಿಕ್ತ, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            ಭಙ್ಗ;
                        }
                    }

                    ಯತ್ನ
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Checking cache lock... Done!");
                    }
                    ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
                    {
                        CV_LOG_WARNING(ರಿಕ್ತ, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    ಗೃಹಾಣ (...)
                    {
                        CV_LOG_WARNING(ರಿಕ್ತ, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                ಅನ್ಯಥಾ
                {
                    ಯದಿ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ರಿಕ್ತ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    ಅನ್ಯಥಾ
                    {
                        CV_LOG_INFO(ರಿಕ್ತ, "OpenCL cache lock is disabled");
                    }
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } ಯಾವತ್ (0);
        ಯದಿ (!cache_path_.empty())
        {
            ಯದಿ (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_INFO(ರಿಕ್ತ, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    ಶೂನ್ಯ clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ಧ್ರುವ std::string& ctx_prefix,
            ಧ್ರುವ std::string& cleanup_prefix)
    {
        ಯದಿ (cache_path_.empty())
            ಪ್ರತ್ಯಾಗಮ std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        ಯದಿ (found_it != prepared_contexts_.end())
            ಪ್ರತ್ಯಾಗಮ found_it->second;

        CV_LOG_INFO(ರಿಕ್ತ, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        ತಾರ್ಕಿಕ result = utils::fs::isDirectory(target_directory);
        ಯದಿ (!result)
        {
            ಯತ್ನ
            {
                CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Creating directory: " << target_directory);
                ಯದಿ (utils::fs::createDirectories(target_directory))
                {
                    result = ಸತ್ಯ;
                }
                ಅನ್ಯಥಾ
                {
                    CV_LOG_WARNING(ರಿಕ್ತ, "Can't create directory: " << target_directory);
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        ಯದಿ (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            ಯತ್ನ
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, ಅಸತ್ಯ, ಸತ್ಯ);
                std::vector<String> remove_entries;
                ಪ್ರತಿ (size_t i = 0; i < entries.size(); i++)
                {
                    ಧ್ರುವ String& name = entries[i];
                    ಯದಿ (0 == name.find(cleanup_prefix))
                    {
                        ಯದಿ (0 == name.find(ctx_prefix))
                            ಅನುವರ್ತ; // skip current
                        remove_entries.push_back(name);
                    }
                }
                ಯದಿ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ರಿಕ್ತ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ರಿಕ್ತ, "Trying to remove these directories...");
                    ಪ್ರತಿ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ರಿಕ್ತ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ರಿಕ್ತ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    ಪ್ರತಿ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ಧ್ರುವ String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        ಯತ್ನ
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(ರಿಕ್ತ, "Removed: " << path);
                        }
                        ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
                        {
                            CV_LOG_ERROR(ರಿಕ್ತ, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            ಗೃಹಾಣ (...)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ರಿಕ್ತ, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        ಪ್ರತ್ಯಾಗಮ target_directory;
    }

    ಸ್ಥಿರ OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, ನೂತನ OpenCLBinaryCacheConfigurator());
    }
};
ವರ್ಗ ದ್ವಿಮಾನ_Program_File
{
    ಗಣನಾ { MAX_ENTRIES = 64 };

    ಪ್ರಕಾರನಿರ್ದೇಶ unsigned ಪೂರ್ಣಾಙ್ಕ uint32_t;

    ಸಂರಚನಾ CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    ಸಂರಚನಾ CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t ಸಂಖ್ಯೆ_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    ಸಂರಚನಾ CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t ಕೀಲಿ_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ಧ್ರುವ std::string fileName_;
    ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ sourceSignature_;
    ಧ್ರುವ size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ಧ್ರುವ std::string& options)
    {
        uint64 hash = crc64((ಧ್ರುವ uchar*)options.c_str(), options.size(), 0);
        ಪ್ರತ್ಯಾಗಮ hash & (MAX_ENTRIES - 1);
    }

    ಅನ್ತಃಸ್ಥ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ಪ್ರತ್ಯಾಗಮ fileSize;
    }
    ಅನ್ತಃಸ್ಥ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.ಪಠ((ವರ್ಣ*)&res, ಪರಿಮಾಣ(uint32_t));
        CV_Assert(!f.fail());
        ಪ್ರತ್ಯಾಗಮ res;
    }
    ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ writeUInt32(ಧ್ರುವ uint32_t value)
    {
        uint32_t v = value;
        f.ಲಿಖ((ವರ್ಣ*)&v, ಪರಿಮಾಣ(uint32_t));
        CV_Assert(!f.fail());
    }

    ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    ಶೂನ್ಯ clearFile()
    {
        f.close();
        ಯದಿ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ರಿಕ್ತ, "Can't remove: " << fileName_);
        ಪ್ರತ್ಯಾಗಮ;
    }

ಸಾರ್ವಜನಿಕ:
    ದ್ವಿಮಾನ_Program_File(ಧ್ರುವ std::string& fileName, ಧ್ರುವ ವರ್ಣ* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(ಪರಿಮಾಣ(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != ರಿಕ್ತ);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, ಪರಿಮಾಣ(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        ಯದಿ(f.is_open() && getFileSize() > 0)
        {
            ತಾರ್ಕಿಕ isValid = ಅಸತ್ಯ;
            ಯತ್ನ
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                ಯದಿ (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<ವರ್ಣ> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.ಪಠ(fileSourceSignature.data(), fileSourceSignatureSize);
                    ಯದಿ (f.eof())
                    {
                        CV_LOG_ERROR(ರಿಕ್ತ, "Unexpected EOF");
                    }
                    ಅನ್ಯಥಾ ಯದಿ (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = ಸತ್ಯ;
                    }
                }
                ಯದಿ (!isValid)
                {
                    CV_LOG_ERROR(ರಿಕ್ತ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            ಗೃಹಾಣ (...)
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            ಯದಿ (!isValid)
            {
                clearFile();
            }
            ಅನ್ಯಥಾ
            {
                seekReadAbsolute(0);
            }
        }
    }

    ತಾರ್ಕಿಕ ಪಠ(ಧ್ರುವ std::string& ಕೀಲಿ, std::vector<ವರ್ಣ>& buf)
    {
        ಯದಿ (!f.is_open())
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        size_t fileSize = getFileSize();
        ಯದಿ (fileSize == 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "Invalid file (empty): " << fileName_);
            clearFile();
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t ಸಂಖ್ಯೆ_Of_Entries = readUInt32();
        CV_Assert(ಸಂಖ್ಯೆ_Of_Entries > 0);
        ಯದಿ (ಸಂಖ್ಯೆ_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "Invalid file: " << fileName_);
            clearFile();
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        f.ಪಠ((ವರ್ಣ*)&entryOffsets[0], ಪರಿಮಾಣ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ಅಂಕೆ = getHash(ಕೀಲಿ);

        uint32_t entryOffset = entryOffsets[entry_ಅಂಕೆ];
        FileEntry entry;
        ಯಾವತ್ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ಪಠ((ವರ್ಣ*)&entry, ಪರಿಮಾಣ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ವರ್ಣ> file_ಕೀಲಿ(entry.ಕೀಲಿ_Size + 1);
            ಯದಿ (ಕೀಲಿ.size() == entry.ಕೀಲಿ_Size)
            {
                ಯದಿ (entry.ಕೀಲಿ_Size > 0)
                {
                    f.ಪಠ(file_ಕೀಲಿ.data(), entry.ಕೀಲಿ_Size);
                    CV_Assert(!f.fail());
                }
                ಯದಿ (memcmp(file_ಕೀಲಿ.data(), ಕೀಲಿ.c_str(), entry.ಕೀಲಿ_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.ಪಠ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Read...");
                    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
                }
            }
            ಯದಿ (entry.nextEntryFileOffset == 0)
                ಭಙ್ಗ;
            entryOffset = entry.nextEntryFileOffset;
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ತಾರ್ಕಿಕ ಲಿಖ(ಧ್ರುವ std::string& ಕೀಲಿ, std::vector<ವರ್ಣ>& buf)
    {
        ಯದಿ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            ಯದಿ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                ಯದಿ (!f.is_open())
                {
                    CV_LOG_ERROR(ರಿಕ್ತ, "Can't create file: " << fileName_);
                    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
                }
            }
        }

        size_t fileSize = getFileSize();
        ಯದಿ (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.ಲಿಖ(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, ಪರಿಮಾಣ(entryOffsets));
            f.ಲಿಖ((ವರ್ಣ*)entryOffsets, ಪರಿಮಾಣ(entryOffsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            fileSize = getFileSize();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize == sourceSignatureSize_);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t ಸಂಖ್ಯೆ_Of_Entries = readUInt32();
        CV_Assert(ಸಂಖ್ಯೆ_Of_Entries > 0);
        ಯದಿ (ಸಂಖ್ಯೆ_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "Invalid file: " << fileName_);
            clearFile();
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.ಪಠ((ವರ್ಣ*)&entryOffsets[0], ಪರಿಮಾಣ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ಅಂಕೆ = getHash(ಕೀಲಿ);

        uint32_t entryOffset = entryOffsets[entry_ಅಂಕೆ];
        FileEntry entry;
        ಯಾವತ್ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ಪಠ((ವರ್ಣ*)&entry, ಪರಿಮಾಣ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ವರ್ಣ> file_ಕೀಲಿ(entry.ಕೀಲಿ_Size + 1);
            ಯದಿ (ಕೀಲಿ.size() == entry.ಕೀಲಿ_Size)
            {
                ಯದಿ (entry.ಕೀಲಿ_Size > 0)
                {
                    f.ಪಠ(file_ಕೀಲಿ.data(), entry.ಕೀಲಿ_Size);
                    CV_Assert(!f.fail());
                }
                ಯದಿ (0 == memcmp(file_ಕೀಲಿ.data(), ಕೀಲಿ.c_str(), entry.ಕೀಲಿ_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Duplicate key ignored: " << fileName_);
                    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
                }
            }
            ಯದಿ (entry.nextEntryFileOffset == 0)
                ಭಙ್ಗ;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        ಯದಿ (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.ಲಿಖ((ವರ್ಣ*)&entry, ಪರಿಮಾಣ(entry));
            CV_Assert(!f.fail());
        }
        ಅನ್ಯಥಾ
        {
            entryOffsets[entry_ಅಂಕೆ] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.ಲಿಖ((ವರ್ಣ*)entryOffsets, ಪರಿಮಾಣ(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.ಕೀಲಿ_Size = (uint32_t)ಕೀಲಿ.size();
        f.ಲಿಖ((ವರ್ಣ*)&entry, ಪರಿಮಾಣ(entry));
        CV_Assert(!f.fail());
        f.ಲಿಖ(ಕೀಲಿ.c_str(), entry.ಕೀಲಿ_Size);
        CV_Assert(!f.fail());
        f.ಲಿಖ(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Write... (" << buf.size() << " bytes)");
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



ಸಂರಚನಾ OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    ಪೂರ್ಣಾಙ್ಕ device_;  // device index in context
    ocl::Queue queue_;
    ಪೂರ್ಣಾಙ್ಕ useOpenCL_;

ರಕ್ಷಿತ:
    Impl() = delete;

    ಶೂನ್ಯ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        ಪೂರ್ಣಾಙ್ಕ ndevices = (ಪೂರ್ಣಾಙ್ಕ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            ಯದಿ (dhandle == deviceID)
            {
                device_ = i;
                found = ಸತ್ಯ;
                ಭಙ್ಗ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    ಶೂನ್ಯ _init_device(ಧ್ರುವ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        ಪೂರ್ಣಾಙ್ಕ ndevices = (ಪೂರ್ಣಾಙ್ಕ)context_.ndevices();
        CV_Assert(ndevices > 0);
        ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            ಯದಿ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = ಸತ್ಯ;
                ಭಙ್ಗ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

ಸಾರ್ವಜನಿಕ:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_UNUSED(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ಧ್ರುವ ocl::Context& context, ಧ್ರುವ ocl::Device& device, ಧ್ರುವ ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ಧ್ರುವ ocl::Context& context, ಧ್ರುವ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ಧ್ರುವ ocl::Context& context, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ device, ಧ್ರುವ ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ಧ್ರುವ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ useOpenCL() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ const_cast<Impl*>(ಏತತ್)->useOpenCL(); }
    ತಾರ್ಕಿಕ useOpenCL()
    {
        ಯದಿ (useOpenCL_ < 0)
        {
            ಯತ್ನ
            {
                useOpenCL_ = 0;
                ಯದಿ (!context_.empty() && context_.ndevices() > 0)
                {
                    ಧ್ರುವ Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception&)
            {
                // nothing
            }
            ಯದಿ (!useOpenCL_)
                CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: can't use OpenCL execution context");
        }
        ಪ್ರತ್ಯಾಗಮ useOpenCL_ > 0;
    }

    ಶೂನ್ಯ setUseOpenCL(ತಾರ್ಕಿಕ flag)
    {
        ಯದಿ (!flag)
            useOpenCL_ = 0;
        ಅನ್ಯಥಾ
            useOpenCL_ = -1;
    }

    ಸ್ಥಿರ ಧ್ರುವ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: initializing thread execution context");

        ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
        ಸ್ಥಿರ std::shared_ptr<Impl> g_primaryExecutionContext;

        ಯದಿ (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            ಯದಿ (!initialized)
            {
                CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: creating new execution context...");
                ಯತ್ನ
                {
                    Context c = ocl::Context::create(std::string());
                    ಯದಿ (c.ndevices())
                    {
                        ಪೂರ್ಣಾಙ್ಕ deviceId = 0;
                        auto& d = c.device(deviceId);
                        ಯದಿ (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            ಯದಿ (!q.ptr())
                            {
                                CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: Can't create default OpenCL queue");
                            }
                            ಅನ್ಯಥಾ
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: device=" << d.name());
                            }
                        }
                        ಅನ್ಯಥಾ
                        {
                            CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    ಅನ್ಯಥಾ
                    {
                        CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: context is not available/disabled");
                    }
                }
                ಗೃಹಾಣ (ಧ್ರುವ std::exception& e)
                {
                    CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                ಗೃಹಾಣ (...)
                {
                    CV_LOG_WARNING(ರಿಕ್ತ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = ಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->context_;
}
Device& OpenCLExecutionContext::getDevice() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->queue_;
}

ತಾರ್ಕಿಕ OpenCLExecutionContext::useOpenCL() ಧ್ರುವ
{
    ಯದಿ (p)
        ಪ್ರತ್ಯಾಗಮ p->useOpenCL();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}
ಶೂನ್ಯ OpenCLExecutionContext::setUseOpenCL(ತಾರ್ಕಿಕ flag)
{
    CV_Assert(p);
    p->setUseOpenCL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    ಯದಿ (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = ಸತ್ಯ;
        ಯದಿ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ಪ್ರತ್ಯಾಗಮ c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    ಪ್ರತ್ಯಾಗಮ c;
}

ಶೂನ್ಯ OpenCLExecutionContext::bind() ಧ್ರುವ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *ಏತತ್;
    data.oclExecutionContextInitialized = ಸತ್ಯ;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ಧ್ರುವ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ಧ್ರುವ Queue q(getContext(), getDevice());
    ಪ್ರತ್ಯಾಗಮ cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ಧ್ರುವ ocl::Queue& q) ಧ್ರುವ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ರಿಕ್ತ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ಪ್ರತ್ಯಾಗಮ c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ಧ್ರುವ Context& context, ಧ್ರುವ Device& device, ಧ್ರುವ ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    ಯದಿ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    ಪ್ರತ್ಯಾಗಮ ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ಧ್ರುವ Context& context, ಧ್ರುವ Device& device)
{
    CV_TRACE_FUNCTION();
    ಯದಿ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ಪ್ರತ್ಯಾಗಮ ctx;

}

ಶೂನ್ಯ OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isOpenCLInitialized = ಅಸತ್ಯ;
ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isOpenCLAvailable = ಅಸತ್ಯ;

ತಾರ್ಕಿಕ haveOpenCL()
{
    CV_TRACE_FUNCTION();

    ಯದಿ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        ಯದಿ (!envPath.empty())
        {
            ಯದಿ (envPath == "disabled")
            {
                g_isOpenCLAvailable = ಅಸತ್ಯ;
                g_isOpenCLInitialized = ಸತ್ಯ;
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(ರಿಕ್ತ, "Initialize OpenCL runtime...");
        ಯತ್ನ
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ರಿಕ್ತ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: found " << n << " platforms");
        }
        ಗೃಹಾಣ (...)
        {
            g_isOpenCLAvailable = ಅಸತ್ಯ;
        }
        g_isOpenCLInitialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ g_isOpenCLAvailable;
}

ತಾರ್ಕಿಕ useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    ಯದಿ (data.useOpenCL < 0)
    {
        ಯತ್ನ
        {
            data.useOpenCL = 0;
            ಯದಿ (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        ಗೃಹಾಣ (...)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ಪ್ರತ್ಯಾಗಮ data.useOpenCL > 0;
}

ತಾರ್ಕಿಕ isOpenCLActivated()
{
    ಯದಿ (!g_isOpenCLAvailable)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ಪ್ರತ್ಯಾಗಮ useOpenCL();
}

ಶೂನ್ಯ setUseOpenCL(ತಾರ್ಕಿಕ flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    ಯದಿ (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    ಅನ್ಯಥಾ
    {
        ಯದಿ (!flag)
            data.useOpenCL = 0;
        ಅನ್ಯಥಾ
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

ವರ್ಗ AmdBlasHelper
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, ನೂತನ AmdBlasHelper())
    }

    ತಾರ್ಕಿಕ isAvailable() ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ g_isAmdBlasAvailable;
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

ರಕ್ಷಿತ:
    AmdBlasHelper()
    {
        ಯದಿ (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            ಯದಿ (!g_isAmdBlasInitialized)
            {
                ಯದಿ (haveOpenCL())
                {
                    ಯತ್ನ
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    ಗೃಹಾಣ (...)
                    {
                        g_isAmdBlasAvailable = ಅಸತ್ಯ;
                    }
                }
                ಅನ್ಯಥಾ
                    g_isAmdBlasAvailable = ಅಸತ್ಯ;

                g_isAmdBlasInitialized = ಸತ್ಯ;
            }
        }
    }

ಗುಪ್ತ:
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdBlasInitialized;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdBlasAvailable;
};

ತಾರ್ಕಿಕ AmdBlasHelper::g_isAmdBlasAvailable = ಅಸತ್ಯ;
ತಾರ್ಕಿಕ AmdBlasHelper::g_isAmdBlasInitialized = ಅಸತ್ಯ;

ತಾರ್ಕಿಕ haveAmdBlas()
{
    ಪ್ರತ್ಯಾಗಮ AmdBlasHelper::getInstance().isAvailable();
}

#ಅನ್ಯಥಾ

ತಾರ್ಕಿಕ haveAmdBlas()
{
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

#endif

#ifdef HAVE_CLAMDFFT

ವರ್ಗ AmdFftHelper
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, ನೂತನ AmdFftHelper())
    }

    ತಾರ್ಕಿಕ isAvailable() ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ g_isAmdFftAvailable;
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

ರಕ್ಷಿತ:
    AmdFftHelper()
    {
        ಯದಿ (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            ಯದಿ (!g_isAmdFftInitialized)
            {
                ಯದಿ (haveOpenCL())
                {
                    ಯತ್ನ
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = ಸತ್ಯ;
                    }
                    ಗೃಹಾಣ (ಧ್ರುವ Exception &)
                    {
                        g_isAmdFftAvailable = ಅಸತ್ಯ;
                    }
                }
                ಅನ್ಯಥಾ
                    g_isAmdFftAvailable = ಅಸತ್ಯ;

                g_isAmdFftInitialized = ಸತ್ಯ;
            }
        }
    }

ಗುಪ್ತ:
    ಸ್ಥಿರ clfftSetupData setupData;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdFftInitialized;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
ತಾರ್ಕಿಕ AmdFftHelper::g_isAmdFftAvailable = ಅಸತ್ಯ;
ತಾರ್ಕಿಕ AmdFftHelper::g_isAmdFftInitialized = ಅಸತ್ಯ;

ತಾರ್ಕಿಕ haveAmdFft()
{
    ಪ್ರತ್ಯಾಗಮ AmdFftHelper::getInstance().isAvailable();
}

#ಅನ್ಯಥಾ

ತಾರ್ಕಿಕ haveAmdFft()
{
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

#endif

ತಾರ್ಕಿಕ haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ಸಂರಚನಾ Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = ಅಸತ್ಯ;
    }

    ~Impl() {}

    ಶೂನ್ಯ init()
    {
        ಯದಿ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            ಯದಿ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            ಯದಿ( handle != 0 )
            {
                ವರ್ಣ buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, ಪರಿಮಾಣ(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = ಸತ್ಯ;
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
    ಯದಿ(p)
        p->release();
}

Platform::Platform(ಧ್ರುವ Platform& pl)
{
    p = (Impl*)pl.p;
    ಯದಿ(p)
        p->addref();
}

Platform& Platform::operator = (ಧ್ರುವ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &pl) {
        ಯದಿ(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ* Platform::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ರಿಕ್ತ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ಸ್ಥಿರ Platform p;
    ಯದಿ( !p.p )
    {
        p.p = ನೂತನ Impl;
        p.p->init();
    }
    ಪ್ರತ್ಯಾಗಮ p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ಸ್ಥಿರ ಶೂನ್ಯ parseOpenCLVersion(ಧ್ರುವ String &version, ಪೂರ್ಣಾಙ್ಕ &major, ಪೂರ್ಣಾಙ್ಕ &minor)
{
    major = minor = 0;
    ಯದಿ (10 >= version.length())
        ಪ್ರತ್ಯಾಗಮ;
    ಧ್ರುವ ವರ್ಣ *pstr = version.c_str();
    ಯದಿ (0 != strncmp(pstr, "OpenCL ", 7))
        ಪ್ರತ್ಯಾಗಮ;
    size_t ppos = version.find('.', 7);
    ಯದಿ (String::npos == ppos)
        ಪ್ರತ್ಯಾಗಮ;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

ಸಂರಚನಾ Device::Impl
{
    Impl(ಶೂನ್ಯ* d)
        : refcount(1)
        , handle(0)
    {
        ಯತ್ನ
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        ಗೃಹಾಣ (...)
        {
            ಕ್ಷಿಪ;
        }
    }

    ಶೂನ್ಯ _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        ಯಾವತ್ (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            ಯದಿ (pos2 == String::npos)
                pos2 = extensions_.size();
            ಯದಿ (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_set_.insert(extensionName);
            }
            pos = pos2 + 1;
        }

        khr_fp64_support_ = isExtensionSupported("cl_khr_fp64");
        khr_fp16_support_ = isExtensionSupported("cl_khr_fp16");

        intelSubgroupsSupport_ = isExtensionSupported("cl_intel_subgroups");

        vendorName_ = getStrProp(CL_DEVICE_VENDOR);
        ಯದಿ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        ಅನ್ಯಥಾ ಯದಿ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        ಅನ್ಯಥಾ ಯದಿ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        ಅನ್ಯಥಾ
            vendorID_ = UNKNOWN_VENDOR;

        ಧ್ರುವ size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        ಯದಿ (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ಧ್ರುವ size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            ಯದಿ (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(ರಿಕ್ತ, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#ಯದಿ 0
        ಯದಿ (isExtensionSupported("cl_khr_spir"))
        {
#ifndef CL_DEVICE_SPIR_VERSIONS
#define CL_DEVICE_SPIR_VERSIONS                     0x40E0
#endif
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::cout << spir_versions << std::endl;
        }
#endif
    }

    ~Impl()
    {
#ifdef _WIN32
        ಯದಿ (!cv::__termination)
#endif
        {
            ಯದಿ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) ಧ್ರುವ
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        ಪ್ರತ್ಯಾಗಮ clGetDeviceInfo(handle, prop, ಪರಿಮಾಣ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == ಪರಿಮಾಣ(temp) ? _TpOut(temp) : _TpOut();
    }

    ತಾರ್ಕಿಕ getBoolProp(cl_device_info prop) ಧ್ರುವ
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        ಪ್ರತ್ಯಾಗಮ clGetDeviceInfo(handle, prop, ಪರಿಮಾಣ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == ಪರಿಮಾಣ(temp) ? temp != 0 : ಅಸತ್ಯ;
    }

    String getStrProp(cl_device_info prop) ಧ್ರುವ
    {
        ವರ್ಣ buf[4096];
        size_t sz=0;
        ಪ್ರತ್ಯಾಗಮ clGetDeviceInfo(handle, prop, ಪರಿಮಾಣ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ಪರಿಮಾಣ(buf) ? String(buf) : String();
    }

    ತಾರ್ಕಿಕ isExtensionSupported(ಧ್ರುವ std::string& extensionName) ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ extensions_set_.ಎಣಿಕೆ_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    ಪೂರ್ಣಾಙ್ಕ doubleFPConfig_;
    ತಾರ್ಕಿಕ khr_fp64_support_;
    ಪೂರ್ಣಾಙ್ಕ halfFPConfig_;
    ತಾರ್ಕಿಕ khr_fp16_support_;
    ತಾರ್ಕಿಕ hostUnifiedMemory_;
    ಪೂರ್ಣಾಙ್ಕ maxComputeUnits_;
    size_t maxWorkGroupSize_;
    ಪೂರ್ಣಾಙ್ಕ type_;
    ಪೂರ್ಣಾಙ್ಕ addressBits_;
    ಪೂರ್ಣಾಙ್ಕ deviceVersionMajor_;
    ಪೂರ್ಣಾಙ್ಕ deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    ಪೂರ್ಣಾಙ್ಕ vendorID_;
    ತಾರ್ಕಿಕ intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(ಶೂನ್ಯ* d)
{
    p = 0;
    set(d);
}

Device::Device(ಧ್ರುವ Device& d)
{
    p = d.p;
    ಯದಿ(p)
        p->addref();
}

Device& Device::operator = (ಧ್ರುವ Device& d)
{
    Impl* newp = (Impl*)d.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &d) {
        ಯದಿ(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Device::~Device()
{
    ಯದಿ(p)
        p->release();
}

ಶೂನ್ಯ Device::set(ಶೂನ್ಯ* d)
{
    ಯದಿ(p)
        p->release();
    p = ನೂತನ Impl(d);
    ಯದಿ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(ಶೂನ್ಯ* d)
{
    Device device(d);
    ಪ್ರತ್ಯಾಗಮ device;
}

ಶೂನ್ಯ* Device::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

String Device::name() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->name_ : String(); }

String Device::extensions() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? String(p->extensions_) : String(); }

ತಾರ್ಕಿಕ Device::isExtensionSupported(ಧ್ರುವ String& extensionName) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->isExtensionSupported(extensionName) : ಅಸತ್ಯ; }

String Device::version() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->version_ : String(); }

String Device::vendorName() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->vendorName_ : String(); }

ಪೂರ್ಣಾಙ್ಕ Device::vendorID() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

ಪೂರ್ಣಾಙ್ಕ Device::deviceVersionMajor() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->deviceVersionMajor_ : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::deviceVersionMinor() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->driverVersion_ : String(); }

ಪೂರ್ಣಾಙ್ಕ Device::type() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->type_ : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::addressBits() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->addressBits_ : 0; }

ತಾರ್ಕಿಕ Device::available() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::compilerAvailable() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::linkerAvailable() ಧ್ರುವ
#ifdef CL_VERSION_1_2
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : ಅಸತ್ಯ; }
#ಅನ್ಯಥಾ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

ಪೂರ್ಣಾಙ್ಕ Device::doubleFPConfig() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->doubleFPConfig_ : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::singleFPConfig() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_device_fp_config, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::halfFPConfig() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->halfFPConfig_ : 0; }

ತಾರ್ಕಿಕ Device::hasFP64() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->khr_fp64_support_ : ಅಸತ್ಯ; }
ತಾರ್ಕಿಕ Device::hasFP16() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->khr_fp16_support_ : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::endianLittle() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::errorCorrectionSupport() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : ಅಸತ್ಯ; }

ಪೂರ್ಣಾಙ್ಕ Device::executionCapabilities() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_device_exec_capabilities, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::globalMemCacheType() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_device_mem_cache_type, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::global_Mem_Cache_ಸಾಲು_Size() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::localMemType() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_device_local_mem_type, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

ತಾರ್ಕಿಕ Device::hostUnifiedMemory() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->hostUnifiedMemory_ : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::imageSupport() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : ಅಸತ್ಯ; }

ತಾರ್ಕಿಕ Device::imageFromBufferSupport() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : ಅಸತ್ಯ;
}

uint Device::imagePitchAlignment() ಧ್ರುವ
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

uint Device::imageBaseAddressAlignment() ಧ್ರುವ
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

size_t Device::image2DMaxWidth() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ಧ್ರುವ
#ifdef CL_VERSION_1_2
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#ಅನ್ಯಥಾ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ಧ್ರುವ
#ifdef CL_VERSION_1_2
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#ಅನ್ಯಥಾ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

ತಾರ್ಕಿಕ Device::intelSubgroupsSupport() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->intelSubgroupsSupport_ : ಅಸತ್ಯ; }

ಪೂರ್ಣಾಙ್ಕ Device::maxClockFrequency() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxComputeUnits() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->maxComputeUnits_ : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxConstantArgs() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxReadImageArgs() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxWriteImageArgs() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxSamplers() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->maxWorkGroupSize_ : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::maxWorkItemDims() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

ಶೂನ್ಯ Device::maxWorkItemSizes(size_t* sizes) ಧ್ರುವ
{
    ಯದಿ(p)
    {
        ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*ಪರಿಮಾಣ(sizes[0]), &sizes[0], &retsz));
    }
}

ಪೂರ್ಣಾಙ್ಕ Device::memBaseAddrAlign() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthChar() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthShort() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthInt() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthLong() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthFloat() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthDouble() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::nativeVectorWidthHalf() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthChar() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthShort() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthInt() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthLong() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthFloat() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthDouble() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

ಪೂರ್ಣಾಙ್ಕ Device::preferredVectorWidthHalf() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<cl_uint, ಪೂರ್ಣಾಙ್ಕ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ಧ್ರುವ
#ifdef CL_VERSION_1_2
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#ಅನ್ಯಥಾ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ಧ್ರುವ Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ಯದಿ (!c.empty())
    {
        ಪ್ರತ್ಯಾಗಮ c.getDevice();
    }

    ಸ್ಥಿರ Device dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
ಅನ್ತಃಸ್ಥ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ರಿಕ್ತ, &required);
    ಯದಿ (err != CL_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ err;

    param.clear();
    ಯದಿ (required > 0)
    {
        AutoBuffer<ವರ್ಣ> buf(required + 1);
        ವರ್ಣ* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, ರಿಕ್ತ);
        ಯದಿ (err != CL_SUCCESS)
            ಪ್ರತ್ಯಾಗಮ err;
        param = ptr;
    }

    ಪ್ರತ್ಯಾಗಮ CL_SUCCESS;
}

ಸ್ಥಿರ ಶೂನ್ಯ split(ಧ್ರುವ std::string &s, ವರ್ಣ delim, std::vector<std::string> &elems)
{
    elems.clear();
    ಯದಿ (s.size() == 0)
        ಪ್ರತ್ಯಾಗಮ;
    std::istringstream ss(s);
    std::string item;
    ಯಾವತ್ (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ಸ್ಥಿರ ತಾರ್ಕಿಕ parseOpenCLDeviceConfiguration(ಧ್ರುವ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    ಯದಿ (parts.size() > 3)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಯದಿ (parts.size() > 2)
        deviceNameOrID = parts[2];
    ಯದಿ (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    ಯದಿ (parts.size() > 0)
    {
        platform = parts[0];
    }
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸ್ಥಿರ cl_device_id selectOpenCLDevice(ಧ್ರುವ std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    ಯದಿ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    ಯದಿ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

    ತಾರ್ಕಿಕ isID = ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ deviceID = -1;
    ಯದಿ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = ಸತ್ಯ;
        ಪ್ರತಿ (size_t i = 0; i < deviceName.length(); i++)
        {
            ಯದಿ (!isdigit(deviceName[i]))
            {
                isID = ಅಸತ್ಯ;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            ಯದಿ (deviceID < 0)
                ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint ಅಂಕೆ_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ರಿಕ್ತ, &ಅಂಕೆ_Platforms));

        ಯದಿ (ಅಂಕೆ_Platforms == 0)
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        platforms.resize((size_t)ಅಂಕೆ_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], &ಅಂಕೆ_Platforms));
        platforms.resize(ಅಂಕೆ_Platforms);
    }

    ಯದಿ (platform.length() > 0)
    {
        ಪ್ರತಿ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            ಯದಿ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            ಅನ್ಯಥಾ
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        ಯದಿ (platforms.size() == 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            ಗಚ್ಛ not_found;
        }
    }
    ಯದಿ (deviceTypes.size() == 0)
    {
        ಯದಿ (!isID)
        {
            deviceTypes.push_back("GPU");
            ಯದಿ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        ಅನ್ಯಥಾ
            deviceTypes.push_back("ALL");
    }
    ಪ್ರತಿ (size_t t = 0; t < deviceTypes.size(); t++)
    {
        ಪೂರ್ಣಾಙ್ಕ deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        ಯದಿ (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        ಅನ್ಯಥಾ ಯದಿ (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        ಅನ್ಯಥಾ ಯದಿ (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        ಅನ್ಯಥಾ ಯದಿ (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        ಅನ್ಯಥಾ
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            ಗಚ್ಛ not_found;
        }

        std::vector<cl_device_id> devices;
        ಪ್ರತಿ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ಎಣಿಕೆ_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, ರಿಕ್ತ, &ಎಣಿಕೆ_2);
            ಯದಿ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            ಯದಿ (ಎಣಿಕೆ_2 == 0)
                ಅನುವರ್ತ;
            size_t base = devices.size();
            devices.resize(base + ಎಣಿಕೆ_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, ಎಣಿಕೆ_2, &devices[base], &ಎಣಿಕೆ_2);
            ಯದಿ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        ಪ್ರತಿ (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : ಸತ್ಯ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = ಸತ್ಯ;
            ಯದಿ(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, ಪರಿಮಾಣ(isIGPU), &isIGPU, ರಿಕ್ತ));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            ಯದಿ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ಪ್ರತ್ಯಾಗಮ devices[i];
            }
        }
    }

not_found:
    ಯದಿ (configuration.empty())
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ; // suppress messages on stderr

    std::ostringstream ಸಂದೇಶ;
    ಸಂದೇಶ << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    ಪ್ರತಿ (size_t t = 0; t < deviceTypes.size(); t++)
        ಸಂದೇಶ << ' ' << deviceTypes[t];

    ಸಂದೇಶ << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ರಿಕ್ತ, ಸಂದೇಶ.str());
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#ifdef HAVE_OPENCL_SVM
ನಾಮಾಕಾಶ svm {

ಗಣನಾ AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

ಸ್ಥಿರ ತಾರ್ಕಿಕ checkForceSVMUmatUsage()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ force;
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkDisableSVMUMatUsage()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ force;
}
ಸ್ಥಿರ ತಾರ್ಕಿಕ checkDisableSVM()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ force = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ force;
}
// see SVMCapabilities
ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ getSVMCapabilitiesMask()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ unsigned ಪೂರ್ಣಾಙ್ಕ mask = 0;
    ಯದಿ (!initialized)
    {
        ಧ್ರುವ std::string env_ಮೌಲ್ಯ = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        ಯದಿ (env_ಮೌಲ್ಯ.empty())
        {
            ಪ್ರತ್ಯಾಗಮ ~0U; // all bits 1
        }
        mask = atoi(env_ಮೌಲ್ಯ.c_str());
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ mask;
}
} // namespace
#endif

ಸ್ಥಿರ size_t get_Program_ಎಣಿಕೆ_Limit()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ size_t ಎಣಿಕೆ_2 = 0;
    ಯದಿ (!initialized)
    {
        ಎಣಿಕೆ_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಎಣಿಕೆ_2;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ g_contextId = 0;

ವರ್ಗ OpenCLBufferPoolImpl;
ವರ್ಗ OpenCLSVMBufferPoolImpl;

ಸಂರಚನಾ Context::Impl
{
    ಸ್ಥಿರ Context::Impl* get(Context& context) { ಪ್ರತ್ಯಾಗಮ context.getImpl(); }

    ಪ್ರಕಾರನಿರ್ದೇಶ std::deque<Context::Impl*> container_t;
    ಸ್ಥಿರ container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ಸ್ಥಿರ container_t* g_contexts = ನೂತನ container_t();
        ಪ್ರತ್ಯಾಗಮ *g_contexts;
    }

ರಕ್ಷಿತ:
    Impl(ಧ್ರುವ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(ಅಸತ್ಯ)
#endif
    {
        ಯದಿ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ಏತತ್;
    }

    ~Impl()
    {
#ifdef _WIN32
        ಯದಿ (!cv::__termination)
#endif
        {
            ಯದಿ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ರಿಕ್ತ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ರಿಕ್ತ;
        }
    }

    ಶೂನ್ಯ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_ಅಂಕೆ_DEVICES, ಪರಿಮಾಣ(ndevices), &ndevices, ರಿಕ್ತ));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * ಪರಿಮಾಣ(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * ಪರಿಮಾಣ(cl_device_id), "");

        devices.clear();
        ಪ್ರತಿ (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    ಶೂನ್ಯ __init_buffer_pools();  // w/o synchronization
    ಶೂನ್ಯ _init_buffer_pools() ಧ್ರುವ
    {
        ಯದಿ (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            ಯದಿ (!bufferPool_)
            {
                const_cast<Impl*>(ಏತತ್)->__init_buffer_pools();
            }
        }
    }
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರ Impl* findContext(ಧ್ರುವ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        ಯದಿ (configuration.empty() && !container.empty())
            ಪ್ರತ್ಯಾಗಮ container[0];
        ಪ್ರತಿ (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            ಯದಿ (i && i->configuration == configuration)
            {
                ಪ್ರತ್ಯಾಗಮ i;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    ಸ್ಥಿರ Impl* findOrCreateContext(ಧ್ರುವ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        ಯದಿ (configuration_.empty())
        {
            ಧ್ರುವ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            ಯದಿ (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        ಯದಿ (impl)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಪ್ರತ್ಯಾಗಮ impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        ಯದಿ (d == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;

        impl = ನೂತನ Impl(configuration);
        ಯತ್ನ
        {
            impl->createFromDevice(d);
            ಯದಿ (impl->handle)
                ಪ್ರತ್ಯಾಗಮ impl;
            delete impl;
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
        }
        ಗೃಹಾಣ (...)
        {
            delete impl;
            ಕ್ಷಿಪ;
        }
    }

    ಸ್ಥಿರ Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (ಶೂನ್ಯ*)h);
        Impl* impl = findContext(configuration);
        ಯದಿ (impl)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಪ್ರತ್ಯಾಗಮ impl;
        }

        impl = ನೂತನ Impl(configuration);
        ಯತ್ನ
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ಪ್ರತ್ಯಾಗಮ impl;
        }
        ಗೃಹಾಣ (...)
        {
            delete impl;
            ಕ್ಷಿಪ;
        }
    }

    ಸ್ಥಿರ Impl* findOrCreateContext(ಧ್ರುವ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (ಶೂನ್ಯ*)d);
        Impl* impl = findContext(configuration);
        ಯದಿ (impl)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ಪ್ರತ್ಯಾಗಮ impl;
        }

        impl = ನೂತನ Impl(configuration);
        ಯತ್ನ
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ಪ್ರತ್ಯಾಗಮ impl;
        }
        ಗೃಹಾಣ (...)
        {
            delete impl;
            ಕ್ಷಿಪ;
        }
    }

    ಶೂನ್ಯ setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        ಯದಿ (d == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ;

        createFromDevice(d);
    }

    ಶೂನ್ಯ createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ರಿಕ್ತ);

        cl_platform_id pl = ರಿಕ್ತ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, ಪರಿಮಾಣ(cl_platform_id), &pl, ರಿಕ್ತ));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int status;

        handle = clCreateContext(prop, nd, &d, 0, 0, &status);
        CV_OCL_DBG_CHECK_RESULT(status, "clCreateContext");

        ತಾರ್ಕಿಕ ok = handle != 0 && status == CL_SUCCESS;
        ಯದಿ( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        ಅನ್ಯಥಾ
            handle = ರಿಕ್ತ;
    }

    Program getProg(ಧ್ರುವ ProgramSource& src, ಧ್ರುವ String& buildflags, String& errmsg);

    ಶೂನ್ಯ unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        ಪ್ರತಿ (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              ಯದಿ (it != phash.end())
              {
                  ಯದಿ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ಪ್ರತ್ಯಾಗಮ;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        ಯದಿ (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            ಯದಿ (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ಧ್ರುವ Device& d = devices[0];
                ಪೂರ್ಣಾಙ್ಕ bits = d.addressBits();
                ಯದಿ (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                ಪ್ರತಿ (size_t i = 0; i < prefix.size(); i++)
                {
                    ವರ್ಣ c = prefix[i];
                    ಯದಿ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ prefix;
    }

    std::string& getPrefixBase()
    {
        ಯದಿ (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            ಯದಿ (prefix_base.empty())
            {
                ಧ್ರುವ Device& d = devices[0];
                ಪೂರ್ಣಾಙ್ಕ bits = d.addressBits();
                ಯದಿ (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                ಪ್ರತಿ (size_t i = 0; i < prefix_base.size(); i++)
                {
                    ವರ್ಣ c = prefix_base[i];
                    ಯದಿ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ contextId;  // global unique ID
    ಧ್ರುವ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    ಪ್ರಕಾರನಿರ್ದೇಶ std::map<std::string, Program> phash_t;
    phash_t phash;
    ಪ್ರಕಾರನಿರ್ದೇಶ std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ಧ್ರುವ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        ಪ್ರತ್ಯಾಗಮ *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ಧ್ರುವ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        ಪ್ರತ್ಯಾಗಮ *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    ಶೂನ್ಯ setUserContext(std::type_index typeId, ಧ್ರುವ std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        ಯದಿ (it != userContextStorage.end())
            ಪ್ರತ್ಯಾಗಮ it->second;
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    ತಾರ್ಕಿಕ svmInitialized;
    ತಾರ್ಕಿಕ svmAvailable;
    ತಾರ್ಕಿಕ svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    ಶೂನ್ಯ svmInit()
    {
        CV_Assert(handle != ರಿಕ್ತ);
        ಧ್ರುವ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((ಶೂನ್ಯ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, ಪರಿಮಾಣ(deviceCaps), &deviceCaps, ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            ಗಚ್ಛ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (ಪೂರ್ಣಾಙ್ಕ)deviceCaps);
        CV_Assert(((ಶೂನ್ಯ)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.ಮೌಲ್ಯ_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.ಮೌಲ್ಯ_2 &= svm::getSVMCapabilitiesMask();
        ಯದಿ (svmCapabilities.ಮೌಲ್ಯ_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            ಗಚ್ಛ noSVM;
        }
        ಯತ್ನ
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            ಶೂನ್ಯ* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            ಯದಿ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ಯತ್ನ
            {
                ತಾರ್ಕಿಕ error = ಅಸತ್ಯ;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                ಯದಿ (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ರಿಕ್ತ, ರಿಕ್ತ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                ಯತ್ನ
                {
                    ((ಪೂರ್ಣಾಙ್ಕ*)ptr)[0] = 100;
                }
                ಗೃಹಾಣ (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = ಸತ್ಯ;
                }
                ಯದಿ (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, ರಿಕ್ತ, ರಿಕ್ತ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                ಯದಿ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            ಗೃಹಾಣ (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ಕ್ಷಿಪ;
            }
            clSVMFree(handle, ptr);
            svmFunctions.fn_clSVMAlloc = clSVMAlloc;
            svmFunctions.fn_clSVMFree = clSVMFree;
            svmFunctions.fn_clSetKernelArgSVMPointer = clSetKernelArgSVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_clEnqueueSVMMemcpy = clEnqueueSVMMemcpy;
            svmFunctions.fn_clEnqueueSVMMemFill = clEnqueueSVMMemFill;
            svmFunctions.fn_clEnqueueSVMMap = clEnqueueSVMMap;
            svmFunctions.fn_clEnqueueSVMUnmap = clEnqueueSVMUnmap;
        }
        ಗೃಹಾಣ (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ಯತ್ನ
            {
                // Try HSA extension
                String extensions = device.extensions();
                ಯದಿ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    ಗಚ್ಛ noSVM;
                }
                cl_platform_id p = ರಿಕ್ತ;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, ಪರಿಮಾಣ(cl_platform_id), &p, ರಿಕ್ತ));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSVMFreeAMD");
                svmFunctions.fn_clSetKernelArgSVMPointer = (clSetKernelArgSVMPointerAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_clEnqueueSVMMemcpy = (clEnqueueSVMMemcpyAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_clEnqueueSVMMemFill = (clEnqueueSVMMemFillAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_clEnqueueSVMMap = (clEnqueueSVMMapAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_clEnqueueSVMUnmap = (clEnqueueSVMUnmapAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            ಗೃಹಾಣ (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                ಗಚ್ಛ noSVM;
            }
        }

        svmAvailable = ಸತ್ಯ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = ಸತ್ಯ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ಪ್ರತ್ಯಾಗಮ;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = ಅಸತ್ಯ;
        svmEnabled = ಅಸತ್ಯ;
        svmCapabilities.ಮೌಲ್ಯ_2 = 0;
        svmInitialized = ಸತ್ಯ;
        svmFunctions.fn_clSVMAlloc = ರಿಕ್ತ;
        ಪ್ರತ್ಯಾಗಮ;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ಧ್ರುವ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        ಪ್ರತ್ಯಾಗಮ *bufferPoolSVM_.get();
    }
#endif

    friend ವರ್ಗ Program;
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
Context::Context(ಪೂರ್ಣಾಙ್ಕ dtype)
{
    p = 0;
    create(dtype);
}

ಶೂನ್ಯ Context::release()
{
    ಯದಿ (p)
    {
        p->release();
        p = ರಿಕ್ತ;
    }
}

ತಾರ್ಕಿಕ Context::create()
{
    release();
    ಯದಿ (!haveOpenCL())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    p = Impl::findOrCreateContext(std::string());
    ಯದಿ (p && p->handle)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    release();
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

// deprecated
ತಾರ್ಕಿಕ Context::create(ಪೂರ್ಣಾಙ್ಕ dtype)
{
    ಯದಿ( !haveOpenCL() )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    release();
    ಯದಿ (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    ಅನ್ಯಥಾ ಯದಿ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    ಅನ್ಯಥಾ ಯದಿ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    ಅನ್ಯಥಾ
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    ಯದಿ (p && !p->handle)
    {
        release();
    }
    ಪ್ರತ್ಯಾಗಮ p != 0;
}

Context::Context(ಧ್ರುವ Context& c)
{
    p = (Impl*)c.p;
    ಯದಿ(p)
        p->addref();
}

Context& Context::operator = (ಧ್ರುವ Context& c)
{
    Impl* newp = (Impl*)c.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &c) {
        ಯದಿ(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಶೂನ್ಯ* Context::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p == ರಿಕ್ತ ? ರಿಕ್ತ : p->handle;
}

size_t Context::ndevices() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ಧ್ರುವ
{
    ಸ್ಥಿರ Device dummy;
    ಪ್ರತ್ಯಾಗಮ !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ಯದಿ (!c.empty())
    {
        auto& ctx = c.getContext();
        ಪ್ರತ್ಯಾಗಮ ctx;
    }

    ಸ್ಥಿರ Context dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

Program Context::getProg(ಧ್ರುವ ProgramSource& prog,
                         ಧ್ರುವ String& buildopts, String& errmsg)
{
    ಪ್ರತ್ಯಾಗಮ p ? p->getProg(prog, buildopts, errmsg) : Program();
}

ಶೂನ್ಯ Context::unloadProg(Program& prog)
{
    ಯದಿ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(ಶೂನ್ಯ* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    ಪ್ರತ್ಯಾಗಮ ctx;
}

/* static */
Context Context::fromDevice(ಧ್ರುವ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    ಪ್ರತ್ಯಾಗಮ ctx;
}

/* static */
Context Context::create(ಧ್ರುವ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    ಪ್ರತ್ಯಾಗಮ ctx;
}

ಶೂನ್ಯ* Context::getOpenCLContextProperty(ಪೂರ್ಣಾಙ್ಕ propertyId) ಧ್ರುವ
{
    ಯದಿ (p == ರಿಕ್ತ)
        ಪ್ರತ್ಯಾಗಮ nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ರಿಕ್ತ, &size));
    std::vector<cl_context_properties> prop(size / ಪರಿಮಾಣ(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), ರಿಕ್ತ));
    ಪ್ರತಿ (size_t i = 0; i < prop.size(); i += 2)
    {
        ಯದಿ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, "OpenCL: found context property=" << propertyId << ") => " << (ಶೂನ್ಯ*)prop[i + 1]);
            ಪ್ರತ್ಯಾಗಮ (ಶೂನ್ಯ*)prop[i + 1];
        }
    }
    ಪ್ರತ್ಯಾಗಮ nullptr;
}

#ifdef HAVE_OPENCL_SVM
ತಾರ್ಕಿಕ Context::useSVM() ಧ್ರುವ
{
    Context::Impl* i = p;
    CV_Assert(i);
    ಯದಿ (!i->svmInitialized)
        i->svmInit();
    ಪ್ರತ್ಯಾಗಮ i->svmEnabled;
}
ಶೂನ್ಯ Context::setUseSVM(ತಾರ್ಕಿಕ enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    ಯದಿ (!i->svmInitialized)
        i->svmInit();
    ಯದಿ (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#ಅನ್ಯಥಾ
ತಾರ್ಕಿಕ Context::useSVM() ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; }
ಶೂನ್ಯ Context::setUseSVM(ತಾರ್ಕಿಕ enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
ನಾಮಾಕಾಶ svm {

ಧ್ರುವ SVMCapabilities getSVMCapabilitites(ಧ್ರುವ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    ಯದಿ (!i->svmInitialized)
        i->svmInit();
    ಪ್ರತ್ಯಾಗಮ i->svmCapabilities;
}

CV_EXPORTS ಧ್ರುವ SVMFunctions* getSVMFunctions(ಧ್ರುವ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ರಿಕ್ತ);
    ಪ್ರತ್ಯಾಗಮ &i->svmFunctions;
}

CV_EXPORTS ತಾರ್ಕಿಕ useSVM(UMatUsageFlags usageFlags)
{
    ಯದಿ (checkForceSVMUmatUsage())
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಯದಿ (checkDisableSVMUMatUsage())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

ಶೂನ್ಯ Context::setUserContext(std::type_index typeId, ಧ್ರುವ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->getUserContext(typeId);
}

ಸ್ಥಿರ ಶೂನ್ಯ get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<ವರ್ಣ> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
ಶೂನ್ಯ attachContext(ಧ್ರುವ String& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ಧ್ರುವ std::string& platformName, ಶೂನ್ಯ* platformID, ಶೂನ್ಯ* context, ಶೂನ್ಯ* deviceID
)
{
    ಯದಿ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    ಯದಿ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    ತಾರ್ಕಿಕ platformAvailable = ಅಸತ್ಯ;

    // check if external platformName contained in list of available platforms in OpenCV
    ಪ್ರತಿ (unsigned ಪೂರ್ಣಾಙ್ಕ i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        ಯದಿ (platformName == availablePlatformName)
        {
            platformAvailable = ಸತ್ಯ;
            ಭಙ್ಗ;
        }
    }

    ಯದಿ (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    ಯದಿ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ಪ್ರತ್ಯಾಗಮ ctx;
}

ಶೂನ್ಯ initializeContextFromHandle(Context& ctx, ಶೂನ್ಯ* _platform, ಶೂನ್ಯ* _context, ಶೂನ್ಯ* _device)
{
    // internal call, less checks
    cl_platform_id platformID = (cl_platform_id)_platform;
    cl_context context = (cl_context)_context;
    cl_device_id deviceID = (cl_device_id)_device;

    std::string platformName = PlatformInfo(&platformID).name();

    auto clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

ಸಂರಚನಾ Queue::Impl
{
    ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = ಅಸತ್ಯ;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, ಪರಿಮಾಣ(cl_command_queue_properties), &props, ರಿಕ್ತ));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, ತಾರ್ಕಿಕ isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ಧ್ರುವ Context& c, ಧ್ರುವ Device& d, ತಾರ್ಕಿಕ withProfiling = ಅಸತ್ಯ)
    {
        __init();

        ಧ್ರುವ Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        ಯದಿ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        ಯದಿ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        ಯದಿ (!cv::__termination)
#endif
        {
            ಯದಿ(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = ರಿಕ್ತ;
            }
        }
    }

    ಧ್ರುವ cv::ocl::Queue& getProfilingQueue(ಧ್ರುವ cv::ocl::Queue& self)
    {
        ಯದಿ (isProfilingQueue_)
            ಪ್ರತ್ಯಾಗಮ self;

        ಯದಿ (profiling_queue_.ptr())
            ಪ್ರತ್ಯಾಗಮ profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, ಪರಿಮಾಣ(cl_context), &ctx, ರಿಕ್ತ));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, ಪರಿಮಾಣ(cl_device_id), &device, ರಿಕ್ತ));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = ನೂತನ Impl(q, ಸತ್ಯ);
        profiling_queue_ = queue;

        ಪ್ರತ್ಯಾಗಮ profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    ತಾರ್ಕಿಕ isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ಧ್ರುವ Context& c, ಧ್ರುವ Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ಧ್ರುವ Queue& q)
{
    p = q.p;
    ಯದಿ(p)
        p->addref();
}

Queue& Queue::operator = (ಧ್ರುವ Queue& q)
{
    Impl* newp = (Impl*)q.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &q) {
        ಯದಿ(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Queue::~Queue()
{
    ಯದಿ(p)
        p->release();
}

ತಾರ್ಕಿಕ Queue::create(ಧ್ರುವ Context& c, ಧ್ರುವ Device& d)
{
    ಯದಿ(p)
        p->release();
    p = ನೂತನ Impl(c, d);
    ಪ್ರತ್ಯಾಗಮ p->handle != 0;
}

ಶೂನ್ಯ Queue::finish()
{
    ಯದಿ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ಧ್ರುವ Queue& Queue::getProfilingQueue() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->getProfilingQueue(*ಏತತ್);
}

ಶೂನ್ಯ* Queue::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ಯದಿ (!c.empty())
    {
        auto& q = c.getQueue();
        ಪ್ರತ್ಯಾಗಮ q;
    }
    ಸ್ಥಿರ Queue dummy;
    ಪ್ರತ್ಯಾಗಮ dummy;
}

ಸ್ಥಿರ cl_command_queue getQueue(ಧ್ರುವ Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    ಯದಿ(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    ಪ್ರತ್ಯಾಗಮ qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ಪೂರ್ಣಾಙ್ಕ _flags, UMat* _m, ಪೂರ್ಣಾಙ್ಕ _wscale, ಪೂರ್ಣಾಙ್ಕ _iwscale, ಧ್ರುವ ಶೂನ್ಯ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ರಿಕ್ತ);
}

KernelArg KernelArg::Constant(ಧ್ರುವ Mat& m)
{
    CV_Assert(m.isContinuous());
    ಪ್ರತ್ಯಾಗಮ KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ಸಂರಚನಾ Kernel::Impl
{
    Impl(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ Program& prog) :
        refcount(1), handle(ರಿಕ್ತ), isInProgress(ಅಸತ್ಯ), isAsyncRun(ಅಸತ್ಯ), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        ಯದಿ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = ಅಸತ್ಯ;
        haveTempSrcUMats = ಅಸತ್ಯ;
    }

    ಶೂನ್ಯ cleanupUMats()
    {
        ತಾರ್ಕಿಕ exceptionOccurred = ಅಸತ್ಯ;
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < MAX_ARRS; i++ )
        {
            ಯದಿ( u[i] )
            {
                ಯದಿ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    ಯತ್ನ
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    ಗೃಹಾಣ(ಧ್ರುವ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ರಿಕ್ತ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = ಸತ್ಯ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = ಅಸತ್ಯ;
        haveTempSrcUMats = ಅಸತ್ಯ;
        CV_Assert(!exceptionOccurred);
    }

    ಶೂನ್ಯ addUMat(ಧ್ರುವ UMat& m, ತಾರ್ಕಿಕ dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        ಯದಿ(dst && m.u->tempUMat())
            haveTempDstUMats = ಸತ್ಯ;
        ಯದಿ(m.u->originalUMatData == ರಿಕ್ತ && m.u->tempUMat())
            haveTempSrcUMats = ಸತ್ಯ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    ಶೂನ್ಯ registerImageArgument(ಪೂರ್ಣಾಙ್ಕ arg, ಧ್ರುವ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        ಯದಿ (arg < (ಪೂರ್ಣಾಙ್ಕ)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    ಶೂನ್ಯ finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = ಅಸತ್ಯ;
        ಯತ್ನ
        {
            cleanupUMats();
        }
        ಗೃಹಾಣ(...)
        {
            release();
            ಕ್ಷಿಪ;
        }
        release();
    }

    ತಾರ್ಕಿಕ run(ಪೂರ್ಣಾಙ್ಕ dims, size_t _globalsize[], size_t _localsize[],
            ತಾರ್ಕಿಕ ಸಮಕಾಲ, int64* timeNS, ಧ್ರುವ Queue& q);

    ~Impl()
    {
        ಯದಿ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ಗಣನಾ { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    ತಾರ್ಕಿಕ isInProgress;
    ತಾರ್ಕಿಕ isAsyncRun;  // true if kernel was scheduled in async mode
    ಪೂರ್ಣಾಙ್ಕ nu;
    std::vector<Image2D> shadow_images;
    ತಾರ್ಕಿಕ haveTempDstUMats;
    ತಾರ್ಕಿಕ haveTempSrcUMats;
};

}} // namespace cv::ocl

ಬಾಹ್ಯ "C" {

ಸ್ಥಿರ ಶೂನ್ಯ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ಶೂನ್ಯ *p)
{
    ಯತ್ನ
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& exc)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    ಗೃಹಾಣ (ಧ್ರುವ std::exception& exc)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    ಗೃಹಾಣ (...)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ ProgramSource& src,
               ಧ್ರುವ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ಧ್ರುವ Kernel& k)
{
    p = k.p;
    ಯದಿ(p)
        p->addref();
}

Kernel& Kernel::operator = (ಧ್ರುವ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &k) {
        ಯದಿ(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Kernel::~Kernel()
{
    ಯದಿ(p)
        p->release();
}

ತಾರ್ಕಿಕ Kernel::create(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ Program& prog)
{
    ಯದಿ(p)
        p->release();
    p = ನೂತನ Impl(kname, prog);
    ಯದಿ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    ಪ್ರತ್ಯಾಗಮ p != 0;
}

ತಾರ್ಕಿಕ Kernel::create(ಧ್ರುವ ವರ್ಣ* kname, ಧ್ರುವ ProgramSource& src,
                    ಧ್ರುವ String& buildopts, String* errmsg)
{
    ಯದಿ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    ಯದಿ( !errmsg ) errmsg = &tempmsg;
    ಧ್ರುವ Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ಪ್ರತ್ಯಾಗಮ create(kname, prog);
}

ಶೂನ್ಯ* Kernel::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

ತಾರ್ಕಿಕ Kernel::empty() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ ptr() == 0;
}

ಸ್ಥಿರ cv::String dump_ಮೌಲ್ಯ(size_t sz, ಧ್ರುವ ಶೂನ್ಯ* p)
{
    ಯದಿ (!p)
        ಪ್ರತ್ಯಾಗಮ "NULL";
    ಯದಿ (sz == 2)
        ಪ್ರತ್ಯಾಗಮ cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    ಯದಿ (sz == 4)
        ಪ್ರತ್ಯಾಗಮ cv::format("%d / %uu / 0x%08x / %g", *(ಪೂರ್ಣಾಙ್ಕ*)p, *(ಪೂರ್ಣಾಙ್ಕ*)p, *(ಪೂರ್ಣಾಙ್ಕ*)p, *(ಭಿನ್ನ*)p);
    ಯದಿ (sz == 8)
        ಪ್ರತ್ಯಾಗಮ cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(ದ್ವಿಗುಣ*)p);
    ಪ್ರತ್ಯಾಗಮ cv::format("%p", p);
}

ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ ಶೂನ್ಯ* value, size_t sz)
{
    ಯದಿ (!p || !p->handle)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (i < 0)
        ಪ್ರತ್ಯಾಗಮ i;
    ಯದಿ( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಪೂರ್ಣಾಙ್ಕ)sz, dump_ಮೌಲ್ಯ(sz, value).c_str()).c_str());
    ಯದಿ (retval != CL_SUCCESS)
        ಪ್ರತ್ಯಾಗಮ -1;
    ಪ್ರತ್ಯಾಗಮ i+1;
}

ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    ಪೂರ್ಣಾಙ್ಕ res = set(i, &h, ಪರಿಮಾಣ(h));
    ಯದಿ (res >= 0)
        p->registerImageArgument(i, image2D);
    ಪ್ರತ್ಯಾಗಮ res;
}

ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ UMat& m)
{
    ಪ್ರತ್ಯಾಗಮ set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ಸ್ಥಿರ ತಾರ್ಕಿಕ isCudaBacked(ಧ್ರುವ UMatData* u)
{
    ಧ್ರುವ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ಪ್ರತ್ಯಾಗಮ u && cudaAllocator && u->currAllocator == cudaAllocator;
}

ಪೂರ್ಣಾಙ್ಕ Kernel::set(ಪೂರ್ಣಾಙ್ಕ i, ಧ್ರುವ KernelArg& arg)
{
    ಯದಿ( !p || !p->handle )
        ಪ್ರತ್ಯಾಗಮ -1;
    ಯದಿ (i < 0)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i));
        ಪ್ರತ್ಯಾಗಮ i;
    }
    ಯದಿ( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    ಯದಿ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        ತಾರ್ಕಿಕ ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        ಯದಿ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ರಿಕ್ತ;
            status = clSetKernelArg(p->handle, (cl_uint)i, ಪರಿಮಾಣ(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i).c_str());
            ಪ್ರತ್ಯಾಗಮ i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        ಯದಿ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ರಿಕ್ತ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ಪ್ರತ್ಯಾಗಮ -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        ಯದಿ (!h)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಪೂರ್ಣಾಙ್ಕ)arg.flags, arg.m));
            p->release();
            p = 0;
            ಪ್ರತ್ಯಾಗಮ -1;
        }

#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ಧ್ರುವ Context& ctx = Context::getDefault();
            ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#ಯದಿ 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#ಅನ್ಯಥಾ
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಶೂನ್ಯ*)svmDataPtr).c_str());
        }
        ಅನ್ಯಥಾ
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, ಪರಿಮಾಣ(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಶೂನ್ಯ*)h).c_str());
        }

        ಯದಿ (ptronly)
        {
            i++;
        }
        ಅನ್ಯಥಾ ಯದಿ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), ಪರಿಮಾಣ(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+1), (ಪೂರ್ಣಾಙ್ಕ)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), ಪರಿಮಾಣ(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+2), (ಪೂರ್ಣಾಙ್ಕ)u2d.offset).c_str());
            i += 3;

            ಯದಿ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ಪೂರ್ಣಾಙ್ಕ cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, ಪರಿಮಾಣ(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಪೂರ್ಣಾಙ್ಕ)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), ಪರಿಮಾಣ(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+1), (ಪೂರ್ಣಾಙ್ಕ)cols).c_str());
                i += 2;
            }
        }
        ಅನ್ಯಥಾ
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), ಪರಿಮಾಣ(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+1), (ಪೂರ್ಣಾಙ್ಕ)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), ಪರಿಮಾಣ(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+2), (ಪೂರ್ಣಾಙ್ಕ)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), ಪರಿಮಾಣ(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+3), (ಪೂರ್ಣಾಙ್ಕ)u3d.offset).c_str());
            i += 4;
            ಯದಿ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ಪೂರ್ಣಾಙ್ಕ cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, ಪರಿಮಾಣ(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಪೂರ್ಣಾಙ್ಕ)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), ಪರಿಮಾಣ(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+1), (ಪೂರ್ಣಾಙ್ಕ)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), ಪರಿಮಾಣ(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)(i+2), (ಪೂರ್ಣಾಙ್ಕ)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ಪ್ರತ್ಯಾಗಮ i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)i, (ಪೂರ್ಣಾಙ್ಕ)arg.sz, (ಶೂನ್ಯ*)arg.obj).c_str());
    ಪ್ರತ್ಯಾಗಮ i+1;
}

ತಾರ್ಕಿಕ Kernel::run(ಪೂರ್ಣಾಙ್ಕ dims, size_t _globalsize[], size_t _localsize[],
                 ತಾರ್ಕಿಕ ಸಮಕಾಲ, ಧ್ರುವ Queue& q)
{
    ಯದಿ (!p)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != ರಿಕ್ತ);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(ಪೂರ್ಣಾಙ್ಕ)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        ಯದಿ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned ಪೂರ್ಣಾಙ್ಕ)val) * val;
    }
    CV_Assert(total > 0);

    ಪ್ರತ್ಯಾಗಮ p->run(dims, globalsize, _localsize, ಸಮಕಾಲ, ರಿಕ್ತ, q);
}


ತಾರ್ಕಿಕ Kernel::run_(ಪೂರ್ಣಾಙ್ಕ dims, size_t _globalsize[], size_t _localsize[],
                  ತಾರ್ಕಿಕ ಸಮಕಾಲ, ಧ್ರುವ Queue& q)
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->run(dims, _globalsize, _localsize, ಸಮಕಾಲ, ರಿಕ್ತ, q);
}


ತಾರ್ಕಿಕ Kernel::Impl::run(ಪೂರ್ಣಾಙ್ಕ dims, size_t globalsize[], size_t localsize[],
        ತಾರ್ಕಿಕ ಸಮಕಾಲ, int64* timeNS, ಧ್ರುವ Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    ಯದಿ (!handle)
    {
        CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL kernel has zero handle: " << name);
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಯದಿ (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !ಸಮಕಾಲ;

    ಯದಿ (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#ಯದಿ CV_OPENCL_SYNC_RUN_KERNELS
    ಸಮಕಾಲ = ಸತ್ಯ;
#endif

    cl_command_queue qq = getQueue(q);
    ಯದಿ (haveTempDstUMats)
        ಸಮಕಾಲ = ಸತ್ಯ;
    ಯದಿ (haveTempSrcUMats)
        ಸಮಕಾಲ = ಸತ್ಯ;
    ಯದಿ (timeNS)
        ಸಮಕಾಲ = ಸತ್ಯ;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           ರಿಕ್ತ, globalsize, localsize, 0, 0,
                                           (ಸಮಕಾಲ && !timeNS) ? 0 : &asyncEvent);
#ಯದಿ !CV_OPENCL_SHOW_RUN_KERNELS
    ಯದಿ (retval != CL_SUCCESS)
#endif
    {
        cv::String ಸಂದೇಶ = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ಸಮಕಾಲ ? "true" : "false"
                        );
        ಯದಿ (retval != CL_SUCCESS)
        {
            ಸಂದೇಶ = CV_OCL_API_ERROR_ಸಂದೇಶ(retval, ಸಂದೇಶ.c_str());
        }
#ಯದಿ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, ಸಂದೇಶ.c_str());
#ಅನ್ಯಥಾ
        printf("%s\n", ಸಂದೇಶ.c_str());
        fflush(stdout);
#endif
    }
    ಯದಿ (ಸಮಕಾಲ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        ಯದಿ (timeNS)
        {
            ಯದಿ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, ಪರಿಮಾಣ(startTime), &startTime, ರಿಕ್ತ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, ಪರಿಮಾಣ(stopTime), &stopTime, ರಿಕ್ತ));
                *timeNS = (int64)(stopTime - startTime);
            }
            ಅನ್ಯಥಾ
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    ಅನ್ಯಥಾ
    {
        addref();
        isInProgress = ಸತ್ಯ;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ಏತತ್));
    }
    ಯದಿ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ಪ್ರತ್ಯಾಗಮ retval == CL_SUCCESS;
}

ತಾರ್ಕಿಕ Kernel::runTask(ತಾರ್ಕಿಕ ಸಮಕಾಲ, ಧ್ರುವ Queue& q)
{
    ಯದಿ(!p || !p->handle || p->isInProgress)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, ಸಮಕಾಲ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ಸಮಕಾಲ ? "true" : "false").c_str());
    ಯದಿ (ಸಮಕಾಲ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    ಅನ್ಯಥಾ
    {
        p->addref();
        p->isInProgress = ಸತ್ಯ;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    ಯದಿ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ಪ್ರತ್ಯಾಗಮ retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(ಪೂರ್ಣಾಙ್ಕ dims, size_t globalsize[], size_t localsize[], ಧ್ರುವ Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    ತಾರ್ಕಿಕ res = p->run(dims, globalsize, localsize, ಸತ್ಯ, &timeNs, profilingQueue);
    ಪ್ರತ್ಯಾಗಮ res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ಧ್ರುವ
{
    ಯದಿ(!p || !p->handle)
        ಪ್ರತ್ಯಾಗಮ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, ಪರಿಮಾಣ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ಪ್ರತ್ಯಾಗಮ status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ಧ್ರುವ
{
    ಯದಿ(!p || !p->handle)
        ಪ್ರತ್ಯಾಗಮ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, ಪರಿಮಾಣ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ಪ್ರತ್ಯಾಗಮ status == CL_SUCCESS ? val : 0;
}

ತಾರ್ಕಿಕ Kernel::compileWorkGroupSize(size_t wsz[]) ಧ್ರುವ
{
    ಯದಿ(!p || !p->handle || !wsz)
        ಪ್ರತ್ಯಾಗಮ 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, ಪರಿಮಾಣ(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ಪ್ರತ್ಯಾಗಮ status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ಧ್ರುವ
{
    ಯದಿ(!p || !p->handle)
        ಪ್ರತ್ಯಾಗಮ 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, ಪರಿಮಾಣ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ಪ್ರತ್ಯಾಗಮ status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ಸಂರಚನಾ ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ಗಣನಾ KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ಧ್ರುವ String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ಧ್ರುವ String& module, ಧ್ರುವ String& name, ಧ್ರುವ String& codeStr, ಧ್ರುವ String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    ಶೂನ್ಯ init(ಗಣನಾ KIND kind, ಧ್ರುವ String& module, ಧ್ರುವ String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = ರಿಕ್ತ;
        sourceSize_ = 0;
        isHashUpdated = ಅಸತ್ಯ;
    }

    ಶೂನ್ಯ initFromSource(ಧ್ರುವ String& codeStr, ಧ್ರುವ String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        ಯದಿ (sourceHash_.empty())
        {
            updateHash();
        }
        ಅನ್ಯಥಾ
        {
            isHashUpdated = ಸತ್ಯ;
        }
    }

    ಶೂನ್ಯ updateHash(ಧ್ರುವ ವರ್ಣ* hashStr = ರಿಕ್ತ)
    {
        ಯದಿ (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ;
        }
        uint64 hash = 0;
        ವಿಕಲ್ಪ (kind_)
        {
        ಪಕ್ಷ PROGRAM_SOURCE_CODE:
            ಯದಿ (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            ಅನ್ಯಥಾ
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            ಭಙ್ಗ;
        ಪಕ್ಷ PROGRAM_BINARIES:
        ಪಕ್ಷ PROGRAM_SPIR:
        ಪಕ್ಷ PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            ಭಙ್ಗ;
        ಸ್ವಭಾವ:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = ಸತ್ಯ;
    }

    Impl(ಗಣನಾ KIND kind,
            ಧ್ರುವ String& module, ಧ್ರುವ String& name,
            ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size,
            ಧ್ರುವ cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    ಸ್ಥಿರ ProgramSource fromSourceWithStaticLifetime(ಧ್ರುವ String& module, ಧ್ರುವ String& name,
            ಧ್ರುವ ವರ್ಣ* sourceCodeStaticStr, ಧ್ರುವ ವರ್ಣ* hashStaticStr,
            ಧ್ರುವ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ನೂತನ Impl(PROGRAM_SOURCE_CODE, module, name,
                (ಧ್ರುವ unsigned ವರ್ಣ*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಸ್ಥಿರ ProgramSource from_ದ್ವಿಮಾನ(ಧ್ರುವ String& module, ಧ್ರುವ String& name,
            ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size,
            ಧ್ರುವ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ನೂತನ Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಸ್ಥಿರ ProgramSource fromSPIR(ಧ್ರುವ String& module, ಧ್ರುವ String& name,
            ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size,
            ಧ್ರುವ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ನೂತನ Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        ಪ್ರತ್ಯಾಗಮ result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ಧ್ರುವ unsigned ವರ್ಣ* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    ತಾರ್ಕಿಕ isHashUpdated;

    friend ಸಂರಚನಾ Program::Impl;
    friend ಸಂರಚನಾ internal::ProgramEntry;
    friend ಸಂರಚನಾ Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ಧ್ರುವ String& module, ಧ್ರುವ String& name, ಧ್ರುವ String& codeStr, ಧ್ರುವ String& codeHash)
{
    p = ನೂತನ Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ಧ್ರುವ ವರ್ಣ* prog)
{
    p = ನೂತನ Impl(prog);
}

ProgramSource::ProgramSource(ಧ್ರುವ String& prog)
{
    p = ನೂತನ Impl(prog);
}

ProgramSource::~ProgramSource()
{
    ಯದಿ(p)
        p->release();
}

ProgramSource::ProgramSource(ಧ್ರುವ ProgramSource& prog)
{
    p = prog.p;
    ಯದಿ(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ಧ್ರುವ ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &prog) {
        ಯದಿ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಧ್ರುವ String& ProgramSource::source() ಧ್ರುವ
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == ರಿಕ್ತ); // method returns reference - can't construct temporary object
    ಪ್ರತ್ಯಾಗಮ p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ಧ್ರುವ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_ದ್ವಿಮಾನ(ಧ್ರುವ String& module, ಧ್ರುವ String& name,
        ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size,
        ಧ್ರುವ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ಪ್ರತ್ಯಾಗಮ Impl::from_ದ್ವಿಮಾನ(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ಧ್ರುವ String& module, ಧ್ರುವ String& name,
        ಧ್ರುವ unsigned ವರ್ಣ* binary, ಧ್ರುವ size_t size,
        ಧ್ರುವ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ಪ್ರತ್ಯಾಗಮ Impl::from_ದ್ವಿಮಾನ(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ಧ್ರುವ
{
    ಯದಿ (ಏತತ್->pProgramSource == ರಿಕ್ತ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ಯದಿ (ಏತತ್->pProgramSource == ರಿಕ್ತ)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(ಏತತ್->module, ಏತತ್->name, ಏತತ್->programCode, ಏತತ್->programHash, cv::String());
            ProgramSource* ptr = ನೂತನ ProgramSource(ps);
            const_cast<ProgramEntry*>(ಏತತ್)->pProgramSource = ptr;
        }
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ಸ್ಥಿರ
cv::String joinBuildOptions(ಧ್ರುವ cv::String& a, ಧ್ರುವ cv::String& b)
{
    ಯದಿ (b.empty())
        ಪ್ರತ್ಯಾಗಮ a;
    ಯದಿ (a.empty())
        ಪ್ರತ್ಯಾಗಮ b;
    ಯದಿ (b[0] == ' ')
        ಪ್ರತ್ಯಾಗಮ a + b;
    ಪ್ರತ್ಯಾಗಮ a + (cv::String(" ") + b);
}

ಸಂರಚನಾ Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ಧ್ರುವ ProgramSource& src,
         ಧ್ರುವ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ರಿಕ್ತ),
         buildflags(_buildflags)
    {
        ಧ್ರುವ ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ಧ್ರುವ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        ಯದಿ (ctx.ptr() == ರಿಕ್ತ || device.ptr() == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        ಯದಿ (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            ಯದಿ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            ಅನ್ಯಥಾ ಯದಿ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ಧ್ರುವ String param_buildExtraOptions = getBuildExtraOptions();
            ಯದಿ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#ಯದಿ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ರಿಕ್ತ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#ಯದಿ CV_OPENCL_SHOW_BUILD_KERNELS
        ಯದಿ (handle)
        {
            size_t retsz = 0;
            ವರ್ಣ kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ಪರಿಮಾಣ(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            ಯದಿ (result == CL_SUCCESS && retsz < ಪರಿಮಾಣ(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(ರಿಕ್ತ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_ERROR(ರಿಕ್ತ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    ತಾರ್ಕಿಕ compile(ಧ್ರುವ Context& ctx, ಧ್ರುವ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        ಯದಿ (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ಪ್ರತ್ಯಾಗಮ isLoaded;
        }
        ಪ್ರತ್ಯಾಗಮ compileWithCache(ctx, src_, errmsg);
    }

    ತಾರ್ಕಿಕ compileWithCache(ಧ್ರುವ Context& ctx, ಧ್ರುವ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ಧ್ರುವ std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ಧ್ರುವ String& hash_str = src_->sourceHash_;
        cv::String fname;
        ಯದಿ (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ಧ್ರುವ cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        ಯದಿ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ಯತ್ನ
            {
                std::vector<ವರ್ಣ> ದ್ವಿಮಾನ_Buf;
                ತಾರ್ಕಿಕ res = ಅಸತ್ಯ;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ದ್ವಿಮಾನ_Program_File file(fname, hash_str.c_str());
                    res = file.ಪಠ(buildflags, ದ್ವಿಮಾನ_Buf);
                }
                ಯದಿ (res)
                {
                    CV_Assert(!ದ್ವಿಮಾನ_Buf.empty());
                    CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, ದ್ವಿಮಾನ_Buf, errmsg);
                    ಯದಿ (isLoaded)
                        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            ಗೃಹಾಣ (...)
            {
                CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ರಿಕ್ತ);
        ಯದಿ (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            ಯದಿ (!buildFromSources(ctx, src_, errmsg))
            {
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
        ಅನ್ಯಥಾ ಯದಿ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            ಯದಿ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ತಾರ್ಕಿಕ isLoaded = create_From_ದ್ವಿಮಾನ(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ಯದಿ (!isLoaded)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        ಅನ್ಯಥಾ ಯದಿ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        ಅನ್ಯಥಾ
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != ರಿಕ್ತ);
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
        ಯದಿ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ಯತ್ನ
            {
                std::vector<ವರ್ಣ> ದ್ವಿಮಾನ_Buf;
                get_Program_ದ್ವಿಮಾನ(ದ್ವಿಮಾನ_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ದ್ವಿಮಾನ_Program_File file(fname, hash_str.c_str());
                    file.ಲಿಖ(buildflags, ದ್ವಿಮಾನ_Buf);
                }
            }
            ಗೃಹಾಣ (ಧ್ರುವ cv::Exception& e)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            ಗೃಹಾಣ (...)
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#ಯದಿ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
        ಯದಿ (CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
        {
            std::vector<ವರ್ಣ> ದ್ವಿಮಾನ_Buf;
            get_Program_ದ್ವಿಮಾನ(ದ್ವಿಮಾನ_Buf);
            ಯದಿ (!ದ್ವಿಮಾನ_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ರಿಕ್ತ;
                create_From_ದ್ವಿಮಾನ(ctx, ದ್ವಿಮಾನ_Buf, errmsg);
            }
        }
#endif
        ಪ್ರತ್ಯಾಗಮ handle != ರಿಕ್ತ;
    }

    ಶೂನ್ಯ dumpBuildLog_(cl_int result, ಧ್ರುವ cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<ವರ್ಣ, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        ಯದಿ (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            ಯದಿ (log_retval == CL_SUCCESS)
            {
                ಯದಿ (retsz < buffer.size())
                    buffer[retsz] = 0;
                ಅನ್ಯಥಾ
                    buffer[buffer.size() - 1] = 0;
            }
            ಅನ್ಯಥಾ
            {
                buffer[0] = 0;
            }
        }

        errmsg = String(buffer.data());
        printf("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                sourceModule_.c_str(), sourceName_.c_str(),
                result, getOpenCLErrorString(result),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    ತಾರ್ಕಿಕ buildFromSources(ಧ್ರುವ Context& ctx, ಧ್ರುವ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == ರಿಕ್ತ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ಧ್ರುವ ವರ್ಣ* srcptr = src_->sourceAddr_ ? ((ಧ್ರುವ ವರ್ಣ*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != ರಿಕ್ತ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        ಯದಿ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            ಪ್ರತಿ (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#ಯದಿ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            ಯದಿ (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                ಯದಿ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ರಿಕ್ತ;
                }
                ಯದಿ (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#ಯದಿ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
            ಯದಿ (handle && CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
            {
                CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                ವರ್ಣ kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ಪರಿಮಾಣ(kernels_buffer), &kernels_buffer[0], &retsz);
                ಯದಿ (retsz < ಪರಿಮಾಣ(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                ಅನ್ಯಥಾ
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(ರಿಕ್ತ, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        ಪ್ರತ್ಯಾಗಮ handle != ರಿಕ್ತ;
    }

    ಶೂನ್ಯ get_Program_ದ್ವಿಮಾನ(std::vector<ವರ್ಣ>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_ದ್ವಿಮಾನ_SIZES, ಪರಿಮಾಣ(sz), &sz, ರಿಕ್ತ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, ಪರಿಮಾಣ(ptr), &ptr, ರಿಕ್ತ));
    }

    ತಾರ್ಕಿಕ create_From_ದ್ವಿಮಾನ(ಧ್ರುವ Context& ctx, ಧ್ರುವ std::vector<ವರ್ಣ>& buf, String& errmsg)
    {
        ಪ್ರತ್ಯಾಗಮ create_From_ದ್ವಿಮಾನ(ctx, (ಧ್ರುವ unsigned ವರ್ಣ*)&buf[0], buf.size(), errmsg);
    }

    ತಾರ್ಕಿಕ create_From_ದ್ವಿಮಾನ(ಧ್ರುವ Context& ctx, ಧ್ರುವ unsigned ವರ್ಣ* ದ್ವಿಮಾನ_Addr, ಧ್ರುವ size_t ದ್ವಿಮಾನ_Size, String& errmsg)
    {
        CV_Assert(handle == ರಿಕ್ತ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ರಿಕ್ತ, 0, "Load from binary... (" << ದ್ವಿಮಾನ_Size << " bytes)");

        CV_Assert(ದ್ವಿಮಾನ_Size > 0);

        size_t ndevices = (ಪೂರ್ಣಾಙ್ಕ)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ಧ್ರುವ uchar*> ದ್ವಿಮಾನ_Ptrs(ndevices);
        AutoBuffer<size_t> ದ್ವಿಮಾನ_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ಧ್ರುವ uchar** ದ್ವಿಮಾನ_Ptrs_2 = ದ್ವಿಮಾನ_Ptrs.data();
        size_t* ದ್ವಿಮಾನ_Sizes_2 = ದ್ವಿಮಾನ_Sizes.data();
        ಪ್ರತಿ (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ದ್ವಿಮಾನ_Ptrs_2[i] = ದ್ವಿಮಾನ_Addr;
            ದ್ವಿಮಾನ_Sizes_2[i] = ದ್ವಿಮಾನ_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_ದ್ವಿಮಾನ((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           ದ್ವಿಮಾನ_Sizes_2, ದ್ವಿಮಾನ_Ptrs_2, ರಿಕ್ತ, &result);
        ಯದಿ (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, CV_OCL_API_ERROR_ಸಂದೇಶ(result, "clCreateProgramWithBinary"));
            ಯದಿ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ರಿಕ್ತ;
            }
        }
        ಯದಿ (!handle)
        {
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            ಯದಿ (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                ಯದಿ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ರಿಕ್ತ;
                }
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    ಪರಿಮಾಣ(build_status), &build_status, &retsz));
            ಯದಿ (result == CL_SUCCESS)
            {
                ಯದಿ (build_status == CL_BUILD_SUCCESS)
                {
                    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
                }
                ಅನ್ಯಥಾ
                {
                    CV_LOG_WARNING(ರಿಕ್ತ, "clGetProgramBuildInfo() returns " << build_status);
                    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
                }
            }
            ಅನ್ಯಥಾ
            {
                CV_LOG_ERROR(ರಿಕ್ತ, CV_OCL_API_ERROR_ಸಂದೇಶ(result, "clGetProgramBuildInfo()"));
                ಯದಿ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ರಿಕ್ತ;
                }
            }
        }
#ಯದಿ CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS
        ಯದಿ (handle && CV_OPENCL_VALIDATE_ದ್ವಿಮಾನ_PROGRAMS_ಮೌಲ್ಯ)
        {
            CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            ವರ್ಣ kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ಪರಿಮಾಣ(kernels_buffer), &kernels_buffer[0], &retsz);
            ಯದಿ (retsz < ಪರಿಮಾಣ(kernels_buffer))
                kernels_buffer[retsz] = 0;
            ಅನ್ಯಥಾ
                kernels_buffer[0] = 0;
            CV_LOG_INFO(ರಿಕ್ತ, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        ಪ್ರತ್ಯಾಗಮ handle != ರಿಕ್ತ;
    }

    ~Impl()
    {
        ಯದಿ( handle )
        {
#ifdef _WIN32
            ಯದಿ (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = ರಿಕ್ತ;
        }
    }

    cl_program handle;

    String buildflags;
    String sourceModule_;
    String sourceName_;
};


Program::Program() CV_NOEXCEPT
{
    p = 0;
}

Program::Program(ಧ್ರುವ ProgramSource& src,
        ಧ್ರುವ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ಧ್ರುವ Program& prog)
{
    p = prog.p;
    ಯದಿ(p)
        p->addref();
}

Program& Program::operator = (ಧ್ರುವ Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    ಯದಿ(newp)
        newp->addref();
    ಯದಿ(p)
        p->release();
    p = newp;
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &prog) {
        ಯದಿ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Program::~Program()
{
    ಯದಿ(p)
        p->release();
}

ತಾರ್ಕಿಕ Program::create(ಧ್ರುವ ProgramSource& src,
            ಧ್ರುವ String& buildflags, String& errmsg)
{
    ಯದಿ(p)
    {
        p->release();
        p = ರಿಕ್ತ;
    }
    p = ನೂತನ Impl(src, buildflags, errmsg);
    ಯದಿ(!p->handle)
    {
        p->release();
        p = 0;
    }
    ಪ್ರತ್ಯಾಗಮ p != 0;
}

ಶೂನ್ಯ* Program::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ಧ್ರುವ ProgramSource& Program::source() ಧ್ರುವ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ತಾರ್ಕಿಕ Program::ಪಠ(ಧ್ರುವ String& bin, ಧ್ರುವ String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ತಾರ್ಕಿಕ Program::ಲಿಖ(String& bin) ಧ್ರುವ
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ಧ್ರುವ
{
    ಯದಿ(!p)
        ಪ್ರತ್ಯಾಗಮ String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ಪ್ರತ್ಯಾಗಮ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ಧ್ರುವ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ಪ್ರತ್ಯಾಗಮ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

ಶೂನ್ಯ Program::get_ದ್ವಿಮಾನ(std::vector<ವರ್ಣ>& binary) ಧ್ರುವ
{
    CV_Assert(p && "Empty program");
    p->get_Program_ದ್ವಿಮಾನ(binary);
}

Program Context::Impl::getProg(ಧ್ರುವ ProgramSource& src,
                               ಧ್ರುವ String& buildflags, String& errmsg)
{
    size_t limit = get_Program_ಎಣಿಕೆ_Limit();
    ಧ್ರುವ ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ಕೀಲಿ = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(ಕೀಲಿ);
        ಯದಿ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ಕೀಲಿ);
            ಯದಿ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ಕೀಲಿ);
            }
            ಪ್ರತ್ಯಾಗಮ it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            ಯದಿ (limit > 0 && sz >= limit)
            {
                ಸ್ಥಿರ ತಾರ್ಕಿಕ warningFlag = ಅಸತ್ಯ;
                ಯದಿ (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = ಸತ್ಯ;
                }
                ಯಾವತ್ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    ಯದಿ (c != 0)
                        ಭಙ್ಗ;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(ಕೀಲಿ, prog));
        cacheList.push_front(ಕೀಲಿ);
    }
    ಪ್ರತ್ಯಾಗಮ prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
ವರ್ಗ OpenCLBufferPool
{
ರಕ್ಷಿತ:
    ~OpenCLBufferPool() { }
ಸಾರ್ವಜನಿಕ:
    ಆಭಾಸ T allocate(size_t size) = 0;
    ಆಭಾಸ ಶೂನ್ಯ release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
ವರ್ಗ OpenCLBufferPoolBaseImpl : ಸಾರ್ವಜನಿಕ BufferPoolController, ಸಾರ್ವಜನಿಕ OpenCLBufferPool<T>
{
ಗುಪ್ತ:
    ಅನ್ತಃಸ್ಥ Derived& derived() { ಪ್ರತ್ಯಾಗಮ *static_cast<Derived*>(ಏತತ್); }
ರಕ್ಷಿತ:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    ತಾರ್ಕಿಕ _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        ಪ್ರತಿ (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            ಯದಿ (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
            }
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    // synchronized
    ತಾರ್ಕಿಕ _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ಧ್ರುವ size_t size)
    {
        ಯದಿ (reservedEntries_.empty())
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        ಪ್ರತಿ (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            ಯದಿ (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                ಯದಿ (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    ಯದಿ (diff == 0)
                        ಭಙ್ಗ;
                }
            }
        }
        ಯದಿ (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    // synchronized
    ಶೂನ್ಯ _checkSizeOfReservedEntries()
    {
        ಯಾವತ್ (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ಧ್ರುವ BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    ಅನ್ತಃಸ್ಥ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        ಯದಿ (size < 1024*1024)
            ಪ್ರತ್ಯಾಗಮ 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        ಅನ್ಯಥಾ ಯದಿ (size < 16*1024*1024)
            ಪ್ರತ್ಯಾಗಮ 64*1024;
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ 1024*1024;
    }

ಸಾರ್ವಜನಿಕ:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    ಆಭಾಸ ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
ಸಾರ್ವಜನಿಕ:
    ಆಭಾಸ T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        ಯದಿ (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        ಅನ್ಯಥಾ
        {
            derived()._allocateBufferEntry(entry, size);
        }
        ಪ್ರತ್ಯಾಗಮ entry.clBuffer_;
    }
    ಆಭಾಸ ಶೂನ್ಯ release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        ಯದಿ (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        ಅನ್ಯಥಾ
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    ಆಭಾಸ size_t getReservedSize() ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ currentReservedSize; }
    ಆಭಾಸ size_t getMaxReservedSize() ಧ್ರುವ CV_OVERRIDE { ಪ್ರತ್ಯಾಗಮ maxReservedSize; }
    ಆಭಾಸ ಶೂನ್ಯ setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        ಯದಿ (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            ಪ್ರತಿ (; i != reservedEntries_.end();)
            {
                ಧ್ರುವ BufferEntry& entry = *i;
                ಯದಿ (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    ಅನುವರ್ತ;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    ಆಭಾಸ ಶೂನ್ಯ freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        ಪ್ರತಿ (; i != reservedEntries_.end(); ++i)
        {
            ಧ್ರುವ BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

ಸಂರಚನಾ CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)ರಿಕ್ತ), capacity_(0) { }
};

ವರ್ಗ OpenCLBufferPoolImpl CV_FINAL : ಸಾರ್ವಜನಿಕ OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
ಸಾರ್ವಜನಿಕ:
    ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ CLBufferEntry BufferEntry;
ರಕ್ಷಿತ:
    ಪೂರ್ಣಾಙ್ಕ createFlags_;
ಸಾರ್ವಜನಿಕ:
    OpenCLBufferPoolImpl(ಪೂರ್ಣಾಙ್ಕ createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    ಶೂನ್ಯ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ರಿಕ್ತ);
        entry.capacity_ = alignSize(size, (ಪೂರ್ಣಾಙ್ಕ)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long ಪೂರ್ಣಾಙ್ಕ)entry.capacity_, (ಶೂನ್ಯ*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != ರಿಕ್ತ);
        ಯದಿ(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ಶೂನ್ಯ _releaseBufferEntry(ಧ್ರುವ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ರಿಕ್ತ);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
ಸಂರಚನಾ CLSVMBufferEntry
{
    ಶೂನ್ಯ* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(ರಿಕ್ತ), capacity_(0) { }
};
ವರ್ಗ OpenCLSVMBufferPoolImpl CV_FINAL : ಸಾರ್ವಜನಿಕ OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, ಶೂನ್ಯ*>
{
ಸಾರ್ವಜನಿಕ:
    ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ CLSVMBufferEntry BufferEntry;
ಸಾರ್ವಜನಿಕ:
    OpenCLSVMBufferPoolImpl()
    {
    }

    ಶೂನ್ಯ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ರಿಕ್ತ);
        entry.capacity_ = alignSize(size, (ಪೂರ್ಣಾಙ್ಕ)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ಧ್ರುವ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ತಾರ್ಕಿಕ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (ಪೂರ್ಣಾಙ್ಕ)entry.capacity_);
        ಶೂನ್ಯ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ಶೂನ್ಯ _releaseBufferEntry(ಧ್ರುವ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ರಿಕ್ತ);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <ತಾರ್ಕಿಕ readAccess, ತಾರ್ಕಿಕ writeAccess>
ವರ್ಗ AlignedDataPtr
{
ರಕ್ಷಿತ:
    ಧ್ರುವ size_t size_;
    uchar* ಧ್ರುವ originPtr_;
    ಧ್ರುವ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

ಸಾರ್ವಜನಿಕ:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ರಿಕ್ತ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        ಯದಿ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ನೂತನ uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ಯದಿ (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ಧ್ರುವ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ಪ್ರತ್ಯಾಗಮ ptr_;
    }

    ~AlignedDataPtr()
    {
        ಯದಿ (allocatedPtr_)
        {
            ಯದಿ (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ರಿಕ್ತ;
        }
        ptr_ = ರಿಕ್ತ;
    }
ಗುಪ್ತ:
    AlignedDataPtr(ಧ್ರುವ AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ಧ್ರುವ AlignedDataPtr&); // disabled
};

template <ತಾರ್ಕಿಕ readAccess, ತಾರ್ಕಿಕ writeAccess>
ವರ್ಗ AlignedDataPtr2D
{
ರಕ್ಷಿತ:
    ಧ್ರುವ size_t size_;
    uchar* ಧ್ರುವ originPtr_;
    ಧ್ರುವ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

ಸಾರ್ವಜನಿಕ:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ರಿಕ್ತ), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ರಿಕ್ತ);
        ಯದಿ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ನೂತನ uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ಯದಿ (readAccess)
            {
                ಪ್ರತಿ (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ಧ್ರುವ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ಪ್ರತ್ಯಾಗಮ ptr_;
    }

    ~AlignedDataPtr2D()
    {
        ಯದಿ (allocatedPtr_)
        {
            ಯದಿ (writeAccess)
            {
                ಪ್ರತಿ (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ರಿಕ್ತ;
        }
        ptr_ = ರಿಕ್ತ;
    }
ಗುಪ್ತ:
    AlignedDataPtr2D(ಧ್ರುವ AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ಧ್ರುವ AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


ಶೂನ್ಯ Context::Impl::__init_buffer_pools()
{
    bufferPool_ = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& bufferPool = *bufferPool_.get();
    bufferPoolHostPtr_ = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& bufferPoolHostPtr = *bufferPoolHostPtr_.get();

    size_t defaultPoolSize = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t poolSize = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_BUFFERPOOL_LIMIT", defaultPoolSize);
    bufferPool.setMaxReservedSize(poolSize);
    size_t poolSizeHostPtr = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", defaultPoolSize);
    bufferPoolHostPtr.setMaxReservedSize(poolSizeHostPtr);

#ifdef HAVE_OPENCL_SVM
    bufferPoolSVM_ = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& bufferPoolSVM = *bufferPoolSVM_.get();
    size_t poolSizeSVM = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", defaultPoolSize);
    bufferPoolSVM.setMaxReservedSize(poolSizeSVM);
#endif

    CV_LOG_INFO(ರಿಕ್ತ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

ವರ್ಗ OpenCLAllocator CV_FINAL : ಸಾರ್ವಜನಿಕ MatAllocator
{
ಸಾರ್ವಜನಿಕ:
    ಗಣನಾ AllocatorFlags
    {
        ALLOCATOR_FLAGS_BUFFER_POOL_USED = 1 << 0,
        ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED = 1 << 1,
#ifdef HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED = 1 << 2,
#endif
        ALLOCATOR_FLAGS_EXTERNAL_BUFFER = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flushCleanupQueue();
    }

    UMatData* defaultAllocate(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type, ಶೂನ್ಯ* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ಧ್ರುವ
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        ಪ್ರತ್ಯಾಗಮ u;
    }

    ಸ್ಥಿರ ತಾರ್ಕಿಕ isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ಸ್ಥಿರ ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", ಅಸತ್ಯ);
        ಪ್ರತ್ಯಾಗಮ value;
    }
    ಸ್ಥಿರ ತಾರ್ಕಿಕ isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ಸ್ಥಿರ ತಾರ್ಕಿಕ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", ಅಸತ್ಯ);
        ಪ್ರತ್ಯಾಗಮ value;
    }

    ಶೂನ್ಯ getBestFlags(ಧ್ರುವ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, ಪೂರ್ಣಾಙ್ಕ& createFlags, UMatData::MemoryFlag& flags0) ಧ್ರುವ
    {
        ಧ್ರುವ Device& dev = ctx.device(0);
        createFlags = 0;
        ಯದಿ ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        ಯದಿ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        ಅನ್ಯಥಾ
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* sizes, ಪೂರ್ಣಾಙ್ಕ type,
                       ಶೂನ್ಯ* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!useOpenCL())
            ಪ್ರತ್ಯಾಗಮ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = dims-1; i >= 0; i-- )
        {
            ಯದಿ( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        ಯದಿ (!ctx.getImpl())
            ಪ್ರತ್ಯಾಗಮ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        ಪೂರ್ಣಾಙ್ಕ createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        ಶೂನ್ಯ* handle = ರಿಕ್ತ;
        ಪೂರ್ಣಾಙ್ಕ allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ಧ್ರುವ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ಯದಿ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            ತಾರ್ಕಿಕ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        ಅನ್ಯಥಾ
#endif
        ಯದಿ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        ಅನ್ಯಥಾ ಯದಿ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        ಅನ್ಯಥಾ
        {
            CV_Assert(handle != ರಿಕ್ತ); // Unsupported, throw
        }

        ಯದಿ (!handle)
            ಪ್ರತ್ಯಾಗಮ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = ನೂತನ UMatData(ಏತತ್);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<ಶೂನ್ಯ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(ಸತ್ಯ);
        opencl_allocator_stats.onAllocate(u->size);
        ಪ್ರತ್ಯಾಗಮ u;
    }

    ತಾರ್ಕಿಕ allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        ಯದಿ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            ಪೂರ್ಣಾಙ್ಕ createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            ತಾರ್ಕಿಕ copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            ಪೂರ್ಣಾಙ್ಕ allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            ಶೂನ್ಯ* handle = ರಿಕ್ತ;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            ತಾರ್ಕಿಕ useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            ಯದಿ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (ಪೂರ್ಣಾಙ್ಕ)u->size, handle);
            }
            ಅನ್ಯಥಾ ಯದಿ (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                ಯದಿ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    ತಾರ್ಕಿಕ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (ಪೂರ್ಣಾಙ್ಕ)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = ರಿಕ್ತ;
                    ಯದಿ (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (ಪೂರ್ಣಾಙ್ಕ)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ರಿಕ್ತ, ರಿಕ್ತ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    ಯದಿ (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, ರಿಕ್ತ, ರಿಕ್ತ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            ಅನ್ಯಥಾ
#endif
            {
                ಯದಿ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                ಯದಿ (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (ಪೂರ್ಣಾಙ್ಕ)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long ಪೂರ್ಣಾಙ್ಕ)u->size, u->origdata, (ಶೂನ್ಯ*)handle).c_str());
                }
                ಯದಿ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long ಪೂರ್ಣಾಙ್ಕ)u->size, u->origdata, (ಶೂನ್ಯ*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (ಶೂನ್ಯ*)handle).c_str());
            ಯದಿ(!handle || retval != CL_SUCCESS)
                ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ಏತತ್;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        ಯದಿ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(ಸತ್ಯ);
        opencl_allocator_stats.onAllocate(u->size);
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
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

    ಶೂನ್ಯ deallocate(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        ಯದಿ (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        ಅನ್ಯಥಾ
            deallocate_(u);
    }

    ಶೂನ್ಯ deallocate_(UMatData* u) ಧ್ರುವ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        ಯದಿ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        ಯದಿ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ಪ್ರತ್ಯಾಗಮ;             // avoid any OpenCL calls
#endif
        ಯದಿ(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            ಯದಿ (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    ಯದಿ( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        ತಾರ್ಕಿಕ isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = ರಿಕ್ತ;
                        ಯದಿ (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಙ್ಕ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ರಿಕ್ತ, ರಿಕ್ತ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        ಯದಿ (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ರಿಕ್ತ, ರಿಕ್ತ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    ಅನ್ಯಥಾ
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                ಅನ್ಯಥಾ
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    ಯದಿ( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<ಅಸತ್ಯ, ಸತ್ಯ> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    ಅನ್ಯಥಾ
                    {
                        cl_int retval = 0;
                        ಯದಿ (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            ಶೂನ್ಯ* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ಶೂನ್ಯ*)u->handle, (long long ಪೂರ್ಣಾಙ್ಕ)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            ಯದಿ (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ಶೂನ್ಯ*)u->handle, data, (long long ಪೂರ್ಣಾಙ್ಕ)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(ಅಸತ್ಯ);
            }
            ಅನ್ಯಥಾ
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                ಯದಿ( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            ಅನ್ಯಥಾ
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (ಶೂನ್ಯ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ಸತ್ಯ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ರಿಕ್ತ;
            ಯದಿ(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = ರಿಕ್ತ;
        }
        ಅನ್ಯಥಾ
        {
            CV_Assert(u->origdata == ರಿಕ್ತ);
            ಯದಿ(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(ಸತ್ಯ);
            }
            ಯದಿ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            ಅನ್ಯಥಾ ಯದಿ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            ಅನ್ಯಥಾ ಯದಿ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                ಅನ್ಯಥಾ ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ರಿಕ್ತ, ರಿಕ್ತ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((ಶೂನ್ಯ*)u->handle);
            }
#endif
            ಅನ್ಯಥಾ
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ಸತ್ಯ);
            delete u;
            u = ರಿಕ್ತ;
        }
        CV_Assert(u == ರಿಕ್ತ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    ಶೂನ್ಯ map(UMatData* u, AccessFlag accessFlags) ಧ್ರುವ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        ಯದಿ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ಸತ್ಯ);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            ಯದಿ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಙ್ಕ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ರಿಕ್ತ, ರಿಕ್ತ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(ಅಸತ್ಯ);
                    u->markDeviceMemMapped(ಸತ್ಯ);
                    ಪ್ರತ್ಯಾಗಮ;
                }
#endif

                cl_int retval = CL_SUCCESS;
                ಯದಿ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ಶೂನ್ಯ*)u->handle, (long long ಪೂರ್ಣಾಙ್ಕ)u->size, u->data).c_str());
                }
                ಯದಿ (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(ಅಸತ್ಯ);
                    u->markDeviceMemMapped(ಸತ್ಯ);
                    ಪ್ರತ್ಯಾಗಮ;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            ಯದಿ(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(ಸತ್ಯ);
            }
        }

        ಯದಿ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<ಅಸತ್ಯ, ಸತ್ಯ> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ಶೂನ್ಯ*)u->handle, (long long ಪೂರ್ಣಾಙ್ಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(ಅಸತ್ಯ);
        }
    }

    ಶೂನ್ಯ unmap(UMatData* u) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        ಯದಿ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != ರಿಕ್ತ);
#ifdef HAVE_OPENCL_SVM
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, ರಿಕ್ತ, ರಿಕ್ತ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                ಯದಿ (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(ಅಸತ್ಯ);
                u->markHostCopyObsolete(ಸತ್ಯ);
                ಪ್ರತ್ಯಾಗಮ;
            }
#endif
            ಯದಿ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ಶೂನ್ಯ*)u->handle, u->data, (long long ಪೂರ್ಣಾಙ್ಕ)u->size).c_str());
                ಯದಿ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(ಅಸತ್ಯ);
                u->data = 0;
                u->markDeviceCopyObsolete(ಅಸತ್ಯ);
                u->markHostCopyObsolete(ಸತ್ಯ);
            }
        }
        ಅನ್ಯಥಾ ಯದಿ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<ಸತ್ಯ, ಅಸತ್ಯ> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ಶೂನ್ಯ*)u->handle, (long long ಪೂರ್ಣಾಙ್ಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(ಅಸತ್ಯ);
            u->markHostCopyObsolete(ಸತ್ಯ);
        }
    }

    ತಾರ್ಕಿಕ checkContinuous(ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                         ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
                         ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ಧ್ರುವ
    {
        ತಾರ್ಕಿಕ iscontinuous = ಸತ್ಯ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = dims-2; i >= 0; i-- )
        {
            ಯದಿ( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = ಅಸತ್ಯ;
            total *= sz[i];
            ಯದಿ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            ಯದಿ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        ಯದಿ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            ಯದಿ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                ಯದಿ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                ಯದಿ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            ಅನ್ಯಥಾ
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                ಯದಿ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                ಯದಿ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ಪ್ರತ್ಯಾಗಮ iscontinuous;
    }

    ಶೂನ್ಯ download(UMatData* u, ಶೂನ್ಯ* dstptr, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                  ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
                  ಧ್ರುವ size_t dststep[]) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ;
        UMatDataAutoLock autolock(u);

        ಯದಿ( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ಪ್ರತ್ಯಾಗಮ;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ರಿಕ್ತ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಙ್ಕ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ರಿಕ್ತ, ರಿಕ್ತ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            ಯದಿ( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            ಅನ್ಯಥಾ
            {
                // This code is from MatAllocator::download()
                ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    ಯದಿ( sz[i] == 0 )
                    ಪ್ರತ್ಯಾಗಮ;
                    ಯದಿ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ಧ್ರುವ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ಪ್ರತಿ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ರಿಕ್ತ, ರಿಕ್ತ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        ಅನ್ಯಥಾ
#endif
        {
            ಯದಿ( iscontinuous )
            {
                AlignedDataPtr<ಅಸತ್ಯ, ಸತ್ಯ> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            ಅನ್ಯಥಾ ಯದಿ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ಧ್ರುವ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<ಅಸತ್ಯ, ಅಸತ್ಯ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                ಪ್ರತಿ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            ಅನ್ಯಥಾ
            {
                AlignedDataPtr2D<ಅಸತ್ಯ, ಸತ್ಯ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    ಶೂನ್ಯ upload(UMatData* u, ಧ್ರುವ ಶೂನ್ಯ* srcptr, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
                ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[],
                ಧ್ರುವ size_t srcstep[]) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!u)
            ಪ್ರತ್ಯಾಗಮ;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        ಯದಿ( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(ಅಸತ್ಯ);
            u->markDeviceCopyObsolete(ಸತ್ಯ);
            ಪ್ರತ್ಯಾಗಮ;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ರಿಕ್ತ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ಪೂರ್ಣಾಙ್ಕ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ರಿಕ್ತ, ರಿಕ್ತ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            ಯದಿ( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            ಅನ್ಯಥಾ
            {
                // This code is from MatAllocator::upload()
                ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    ಯದಿ( sz[i] == 0 )
                    ಪ್ರತ್ಯಾಗಮ;
                    ಯದಿ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
                }

                Mat src(dims, isz, CV_8U, (ಶೂನ್ಯ*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ಧ್ರುವ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ಪ್ರತಿ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ರಿಕ್ತ, ರಿಕ್ತ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        ಅನ್ಯಥಾ
#endif
        {
            ಯದಿ( iscontinuous )
            {
                AlignedDataPtr<ಸತ್ಯ, ಅಸತ್ಯ> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (ಶೂನ್ಯ*)u->handle, (long long ಪೂರ್ಣಾಙ್ಕ)dstrawofs, (long long ಪೂರ್ಣಾಙ್ಕ)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            ಅನ್ಯಥಾ ಯದಿ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ಧ್ರುವ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<ಅಸತ್ಯ, ಅಸತ್ಯ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
                ಪ್ರತಿ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            ಅನ್ಯಥಾ
            {
                AlignedDataPtr2D<ಸತ್ಯ, ಅಸತ್ಯ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(ಸತ್ಯ);
#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ಅನ್ಯಥಾ
#endif
        {
            u->markHostCopyObsolete(ಸತ್ಯ);
        }
        u->markDeviceCopyObsolete(ಅಸತ್ಯ);
    }

    ಶೂನ್ಯ copy(UMatData* src, UMatData* dst, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ size_t sz[],
              ಧ್ರುವ size_t srcofs[], ಧ್ರುವ size_t srcstep[],
              ಧ್ರುವ size_t dstofs[], ಧ್ರುವ size_t dststep[], ತಾರ್ಕಿಕ _sync) ಧ್ರುವ CV_OVERRIDE
    {
        ಯದಿ(!src || !dst)
            ಪ್ರತ್ಯಾಗಮ;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ತಾರ್ಕಿಕ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        ಯದಿ( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ಪ್ರತ್ಯಾಗಮ;
        }
        ಯದಿ( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(ಅಸತ್ಯ);
#ifdef HAVE_OPENCL_SVM
            ಯದಿ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            ಅನ್ಯಥಾ
#endif
            {
                dst->markDeviceCopyObsolete(ಸತ್ಯ);
            }
            ಪ್ರತ್ಯಾಗಮ;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ಯದಿ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ಧ್ರುವ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                ಯದಿ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (ಪೂರ್ಣಾಙ್ಕ)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, ರಿಕ್ತ, ರಿಕ್ತ);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                ಅನ್ಯಥಾ
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    ಪೂರ್ಣಾಙ್ಕ isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        ಯದಿ( sz[i] == 0 )
                        ಪ್ರತ್ಯಾಗಮ;
                        ಯದಿ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (ಪೂರ್ಣಾಙ್ಕ)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    ಪ್ರತಿ( ಪೂರ್ಣಾಙ್ಕ i = 0; i < dims; i++ )
                    {
                        ಯದಿ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ಧ್ರುವ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    ಪ್ರತಿ( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            ಅನ್ಯಥಾ
            {
                ಯದಿ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                ಅನ್ಯಥಾ
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        ಅನ್ಯಥಾ
#endif
        {
            ಯದಿ( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (ಶೂನ್ಯ*)src->handle, (ಶೂನ್ಯ*)dst->handle, (long long ಪೂರ್ಣಾಙ್ಕ)srcrawofs, (long long ಪೂರ್ಣಾಙ್ಕ)dstrawofs, (long long ಪೂರ್ಣಾಙ್ಕ)total).c_str());
            }
            ಅನ್ಯಥಾ ಯದಿ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ಧ್ರುವ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<ಅಸತ್ಯ, ಅಸತ್ಯ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<ಅಸತ್ಯ, ಅಸತ್ಯ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_total = alignSize(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_total = std::min(src_total, src->size - new_srcrawofs);
                size_t dst_total = alignSize(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_total = std::min(dst_total, dst->size - new_dstrawofs);

                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_total, srcptr, 0, 0, 0));
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_total, dstptr, 0, 0, 0));

                ಪ್ರತಿ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            ಅನ್ಯಥಾ
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        ಯದಿ (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ಅನ್ಯಥಾ
#endif
        {
            dst->markHostCopyObsolete(ಸತ್ಯ);
        }
        dst->markDeviceCopyObsolete(ಅಸತ್ಯ);

        ಯದಿ( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ಧ್ರುವ ವರ್ಣ* id) ಧ್ರುವ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        ಯದಿ (ctx.empty())
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#ifdef HAVE_OPENCL_SVM
        ಯದಿ ((svm::checkForceSVMUmatUsage() && (id == ರಿಕ್ತ || strcmp(id, "OCL") == 0)) || (id != ರಿಕ್ತ && strcmp(id, "SVM") == 0))
        {
            ಪ್ರತ್ಯಾಗಮ &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        ಯದಿ (id != ರಿಕ್ತ && strcmp(id, "HOST_ALLOC") == 0)
        {
            ಪ್ರತ್ಯಾಗಮ &ctx.getImpl()->getBufferPoolHostPtr();
        }
        ಯದಿ (id != ರಿಕ್ತ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ಪ್ರತ್ಯಾಗಮ &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    ಶೂನ್ಯ flushCleanupQueue() ಧ್ರುವ
    {
        // TSAN false positive: see #27638
        ಯದಿ (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            ಪ್ರತಿ (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    ಶೂನ್ಯ addToCleanupQueue(UMatData* u) ಧ್ರುವ
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

ಸ್ಥಿರ OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ಸ್ಥಿರ OpenCLAllocator* g_allocator = ನೂತನ OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ಪ್ರತ್ಯಾಗಮ g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


ನಾಮಾಕಾಶ cv {

// three funcs below are implemented in umatrix.cpp
ಶೂನ್ಯ setSize( UMat& m, ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಧ್ರುವ size_t* _steps,
              ತಾರ್ಕಿಕ autoSteps = ಅಸತ್ಯ );
ಶೂನ್ಯ finalizeHdr(UMat& m);

} // namespace cv


ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
ಶೂನ್ಯ convertFromBuffer(ಶೂನ್ಯ* cl_mem_buffer, size_t step, ಪೂರ್ಣಾಙ್ಕ rows, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ type, UMat& dst)
{
    ಪೂರ್ಣಾಙ್ಕ d = 2;
    ಪೂರ್ಣಾಙ್ಕ sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, ಸತ್ಯ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, ಪರಿಮಾಣ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, ಪರಿಮಾಣ(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((ಪೂರ್ಣಾಙ್ಕ)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = ನೂತನ UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    ಪ್ರತ್ಯಾಗಮ;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
ಶೂನ್ಯ convertFromImage(ಶೂನ್ಯ* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, ಪರಿಮಾಣ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, ಪರಿಮಾಣ(cl_image_format), &fmt, 0));

    ಪೂರ್ಣಾಙ್ಕ depth = CV_8U;
    ವಿಕಲ್ಪ (fmt.image_channel_data_type)
    {
    ಪಕ್ಷ CL_UNORM_INT8:
    ಪಕ್ಷ CL_UNSIGNED_INT8:
        depth = CV_8U;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_SNORM_INT8:
    ಪಕ್ಷ CL_SIGNED_INT8:
        depth = CV_8S;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_UNORM_INT16:
    ಪಕ್ಷ CL_UNSIGNED_INT16:
        depth = CV_16U;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_SNORM_INT16:
    ಪಕ್ಷ CL_SIGNED_INT16:
        depth = CV_16S;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_SIGNED_INT32:
        depth = CV_32S;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_FLOAT:
        depth = CV_32F;
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_HALF_FLOAT:
        depth = CV_16F;
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    ಪೂರ್ಣಾಙ್ಕ type = CV_8UC1;
    ವಿಕಲ್ಪ (fmt.image_channel_order)
    {
    ಪಕ್ಷ CL_R:
    ಪಕ್ಷ CL_A:
    ಪಕ್ಷ CL_INTENSITY:
    ಪಕ್ಷ CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        ಭಙ್ಗ;

    ಪಕ್ಷ CL_RG:
    ಪಕ್ಷ CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        ಭಙ್ಗ;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    ಪಕ್ಷ CL_RGBA:
    ಪಕ್ಷ CL_BGRA:
    ಪಕ್ಷ CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        ಭಙ್ಗ;

    ಸ್ವಭಾವ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ಭಙ್ಗ;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, ಪರಿಮಾಣ(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, ಪರಿಮಾಣ(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, ಪರಿಮಾಣ(size_t), &h, 0));

    dst.create((ಪೂರ್ಣಾಙ್ಕ)h, (ಪೂರ್ಣಾಙ್ಕ)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ರಿಕ್ತ, ರಿಕ್ತ));

    CV_OCL_CHECK(clFinish(q));

    ಪ್ರತ್ಯಾಗಮ;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ಸ್ಥಿರ ಶೂನ್ಯ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint ಅಂಕೆ_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ರಿಕ್ತ, &ಅಂಕೆ_Devices);
    ಯದಿ (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &ಅಂಕೆ_Devices).c_str());
    }

    ಯದಿ (ಅಂಕೆ_Devices == 0)
    {
        devices.clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    devices.resize((size_t)ಅಂಕೆ_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, ಅಂಕೆ_Devices, &devices[0], &ಅಂಕೆ_Devices));
}

ಸಂರಚನಾ PlatformInfo::Impl
{
    Impl(ಶೂನ್ಯ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ಧ್ರುವ
    {
        ವರ್ಣ buf[1024];
        size_t sz=0;
        ಪ್ರತ್ಯಾಗಮ clGetPlatformInfo(handle, prop, ಪರಿಮಾಣ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ಪರಿಮಾಣ(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    ಪೂರ್ಣಾಙ್ಕ versionMajor_;
    ಪೂರ್ಣಾಙ್ಕ versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(ಶೂನ್ಯ* platform_id)
{
    p = ನೂತನ Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    ಯದಿ(p)
        p->release();
}

PlatformInfo::PlatformInfo(ಧ್ರುವ PlatformInfo& i)
{
    ಯದಿ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ಧ್ರುವ PlatformInfo& i)
{
    ಯದಿ (i.p != p)
    {
        ಯದಿ (i.p)
            i.p->addref();
        ಯದಿ (p)
            p->release();
        p = i.p;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &i) {
        ಯದಿ(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

ಪೂರ್ಣಾಙ್ಕ PlatformInfo::device_ಸಂಖ್ಯೆ() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? (ಪೂರ್ಣಾಙ್ಕ)p->devices.size() : 0;
}

ಶೂನ್ಯ PlatformInfo::getDevice(Device& device, ಪೂರ್ಣಾಙ್ಕ d) ಧ್ರುವ
{
    CV_Assert(p && d < (ಪೂರ್ಣಾಙ್ಕ)p->devices.size() );
    ಯದಿ(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->version_ : String();
}

ಪೂರ್ಣಾಙ್ಕ PlatformInfo::versionMajor() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->versionMajor_;
}

ಪೂರ್ಣಾಙ್ಕ PlatformInfo::versionMinor() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->versionMinor_;
}

ಸ್ಥಿರ ಶೂನ್ಯ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint ಅಂಕೆ_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ರಿಕ್ತ, &ಅಂಕೆ_Platforms));

    ಯದಿ (ಅಂಕೆ_Platforms == 0)
    {
        platforms.clear();
        ಪ್ರತ್ಯಾಗಮ;
    }

    platforms.resize((size_t)ಅಂಕೆ_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], &ಅಂಕೆ_Platforms));
}

ಶೂನ್ಯ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    ಪ್ರತಿ (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((ಶೂನ್ಯ*)&platforms[i]) );
}

ಧ್ರುವ ವರ್ಣ* typeToStr(ಪೂರ್ಣಾಙ್ಕ type)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* tab[CV_DEPTH_MAX*16]=
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
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ಧ್ರುವ ವರ್ಣ* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ಪ್ರತ್ಯಾಗಮ result;
}

ಧ್ರುವ ವರ್ಣ* memopTypeToStr(ಪೂರ್ಣಾಙ್ಕ type)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* tab[CV_DEPTH_MAX*16] =
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
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ಧ್ರುವ ವರ್ಣ* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ಪ್ರತ್ಯಾಗಮ result;
}

ಧ್ರುವ ವರ್ಣ* vecopTypeToStr(ಪೂರ್ಣಾಙ್ಕ type)
{
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* tab[CV_DEPTH_MAX*16] =
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
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ಧ್ರುವ ವರ್ಣ* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    ಪ್ರತ್ಯಾಗಮ result;
}

// Deprecated due to size of buf buffer being unknowable.
ಧ್ರುವ ವರ್ಣ* convertTypeStr(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth, ಪೂರ್ಣಾಙ್ಕ cn, ವರ್ಣ* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ಪ್ರತ್ಯಾಗಮ convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ಧ್ರುವ ವರ್ಣ* convertTypeStr(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth, ಪೂರ್ಣಾಙ್ಕ cn, ವರ್ಣ* buf, size_t buf_size)
{
    ಯದಿ( sdepth == ddepth )
        ಪ್ರತ್ಯಾಗಮ "noconvert";
    ಧ್ರುವ ವರ್ಣ *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    ಯದಿ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    ಅನ್ಯಥಾ ಯದಿ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    ಅನ್ಯಥಾ
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    ಪ್ರತ್ಯಾಗಮ buf;
}

ಧ್ರುವ ವರ್ಣ* getOpenCLErrorString(ಪೂರ್ಣಾಙ್ಕ errorCode)
{
#define CV_OCL_CODE(id) ಪಕ್ಷ id: ಪ್ರತ್ಯಾಗಮ #id
#define CV_OCL_CODE_(id, name) ಪಕ್ಷ id: ಪ್ರತ್ಯಾಗಮ #name
    ವಿಕಲ್ಪ (errorCode)
    {
    CV_OCL_CODE(CL_SUCCESS);
    CV_OCL_CODE(CL_DEVICE_NOT_FOUND);
    CV_OCL_CODE(CL_DEVICE_NOT_AVAILABLE);
    CV_OCL_CODE(CL_COMPILER_NOT_AVAILABLE);
    CV_OCL_CODE(CL_MEM_OBJECT_ALLOCATION_FAILURE);
    CV_OCL_CODE(CL_OUT_OF_RESOURCES);
    CV_OCL_CODE(CL_OUT_OF_HOST_MEMORY);
    CV_OCL_CODE(CL_PROFILING_INFO_NOT_AVAILABLE);
    CV_OCL_CODE(CL_MEM_COPY_OVERLAP);
    CV_OCL_CODE(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_CODE(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_CODE(CL_BUILD_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_MAP_FAILURE);
    CV_OCL_CODE(CL_MISALIGNED_SUB_BUFFER_OFFSET);
    CV_OCL_CODE(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
    CV_OCL_CODE(CL_COMPILE_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_LINKER_NOT_AVAILABLE);
    CV_OCL_CODE(CL_LINK_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_DEVICE_PARTITION_FAILED);
    CV_OCL_CODE(CL_KERNEL_ARG_INFO_NOT_AVAILABLE);
    CV_OCL_CODE(CL_INVALID_ಮೌಲ್ಯ);
    CV_OCL_CODE(CL_INVALID_DEVICE_TYPE);
    CV_OCL_CODE(CL_INVALID_PLATFORM);
    CV_OCL_CODE(CL_INVALID_DEVICE);
    CV_OCL_CODE(CL_INVALID_CONTEXT);
    CV_OCL_CODE(CL_INVALID_QUEUE_PROPERTIES);
    CV_OCL_CODE(CL_INVALID_COMMAND_QUEUE);
    CV_OCL_CODE(CL_INVALID_HOST_PTR);
    CV_OCL_CODE(CL_INVALID_MEM_OBJECT);
    CV_OCL_CODE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_IMAGE_SIZE);
    CV_OCL_CODE(CL_INVALID_SAMPLER);
    CV_OCL_CODE(CL_INVALID_ದ್ವಿಮಾನ);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_ಮೌಲ್ಯ);
    CV_OCL_CODE(CL_INVALID_ARG_SIZE);
    CV_OCL_CODE(CL_INVALID_KERNEL_ARGS);
    CV_OCL_CODE(CL_INVALID_WORK_DIMENSION);
    CV_OCL_CODE(CL_INVALID_WORK_GROUP_SIZE);
    CV_OCL_CODE(CL_INVALID_WORK_ITEM_SIZE);
    CV_OCL_CODE(CL_INVALID_GLOBAL_OFFSET);
    CV_OCL_CODE(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_CODE(CL_INVALID_EVENT);
    CV_OCL_CODE(CL_INVALID_OPERATION);
    CV_OCL_CODE(CL_INVALID_GL_OBJECT);
    CV_OCL_CODE(CL_INVALID_BUFFER_SIZE);
    CV_OCL_CODE(CL_INVALID_MIP_LEVEL);
    CV_OCL_CODE(CL_INVALID_GLOBAL_WORK_SIZE);
    // OpenCL 1.1
    CV_OCL_CODE(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_CODE(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_ಎಣಿಕೆ);
    // OpenCL 2.0
    CV_OCL_CODE_(-69, CL_INVALID_PIPE_SIZE);
    CV_OCL_CODE_(-70, CL_INVALID_DEVICE_QUEUE);
    // Extensions
    CV_OCL_CODE_(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_CODE_(-1001, CL_PLATFORM_NOT_FOUND_KHR);
    CV_OCL_CODE_(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_CODE_(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_CODE_(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_CODE_(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    ಸ್ವಭಾವ: ಪ್ರತ್ಯಾಗಮ "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
ಸ್ಥಿರ std::string kerToStr(ಧ್ರುವ Mat & k)
{
    ಪೂರ್ಣಾಙ್ಕ width = k.cols - 1, depth = k.depth();
    ಧ್ರುವ T * ಧ್ರುವ data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    ಯದಿ (depth <= CV_8S)
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < width; ++i)
            stream << "DIG(" << (ಪೂರ್ಣಾಙ್ಕ)data[i] << ")";
        stream << "DIG(" << (ಪೂರ್ಣಾಙ್ಕ)data[width] << ")";
    }
    ಅನ್ಯಥಾ ಯದಿ (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    ಅನ್ಯಥಾ ಯದಿ (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < width; ++i)
            stream << "DIG(" << (ಭಿನ್ನ)data[i] << "h)";
        stream << "DIG(" << (ಭಿನ್ನ)data[width] << "h)";
    }
    ಅನ್ಯಥಾ
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    ಪ್ರತ್ಯಾಗಮ stream.str();
}

String kernelToStr(ಒಳಹರಿವು_Array _kernel, ಪೂರ್ಣಾಙ್ಕ ddepth, ಧ್ರುವ ವರ್ಣ * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    ಪೂರ್ಣಾಙ್ಕ depth = kernel.depth();
    ಯದಿ (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    ಯದಿ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    ಯದಿ (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    ಪ್ರಕಾರನಿರ್ದೇಶ std::string (* func_t)(ಧ್ರುವ Mat &);
    ಸ್ಥಿರ ಧ್ರುವ func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<ವರ್ಣ>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<ಪೂರ್ಣಾಙ್ಕ>, kerToStr<ಭಿನ್ನ>, kerToStr<ದ್ವಿಗುಣ>, kerToStr<hfloat>,
                                                0, kerToStr<ತಾರ್ಕಿಕ>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ಧ್ರುವ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ಪ್ರತ್ಯಾಗಮ cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    ಕುರು \
    { \
        ಯದಿ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            ಪೂರ್ಣಾಙ್ಕ ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            ಯದಿ (cwidth < ckercn || ckercn <= 0) \
                ಪ್ರತ್ಯಾಗಮ 1; \
            cols.push_back(cwidth); \
            ಯದಿ (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                ಪ್ರತ್ಯಾಗಮ 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    ಯಾವತ್ ((ಶೂನ್ಯ)0, 0)

ಪೂರ್ಣಾಙ್ಕ predictOptimalVectorWidth(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
                              ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
                              ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9,
                              OclVectorStrategy strat)
{
    ಧ್ರುವ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    ಪೂರ್ಣಾಙ್ಕ vectorWidths[CV_DEPTH_MAX] = {
        d.preferredVectorWidthChar(), d.preferredVectorWidthChar(),   // CV_8U,  CV_8S
        d.preferredVectorWidthShort(), d.preferredVectorWidthShort(), // CV_16U, CV_16S
        d.preferredVectorWidthInt(), d.preferredVectorWidthFloat(),   // CV_32S, CV_32F
        d.preferredVectorWidthDouble(), d.preferredVectorWidthHalf(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferredVectorWidthChar(),                                 // CV_Bool
        d.preferredVectorWidthLong(), d.preferredVectorWidthLong(),   // CV_64U, CV_64S
        d.preferredVectorWidthInt()                                   // CV_32U
    };

    // if the device says don't use vectors
    ಯದಿ (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    ಪ್ರತ್ಯಾಗಮ checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

ಪೂರ್ಣಾಙ್ಕ checkOptimalVectorWidth([[maybe_unused]] ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ *vectorWidths,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src1,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src2,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src3,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src4,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src5,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src6,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src7,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src8,
                            [[maybe_unused]] ಒಳಹರಿವು_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    ಪ್ರತ್ಯಾಗಮ 1;
#ಅನ್ಯಥಾ
    CV_Assert(vectorWidths);

    ಪೂರ್ಣಾಙ್ಕ ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<ಪೂರ್ಣಾಙ್ಕ> dividers, kercns;
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

    ಪ್ರತಿ (size_t i = 0; i < size; ++i)
        ಯಾವತ್ (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    ಪೂರ್ಣಾಙ್ಕ kercn = *std::min_element(kercns.begin(), kercns.end());

    ಪ್ರತ್ಯಾಗಮ kercn;
#endif
}

ಪೂರ್ಣಾಙ್ಕ predictOptimalVectorWidthMax(ಒಳಹರಿವು_Array src1, ಒಳಹರಿವು_Array src2, ಒಳಹರಿವು_Array src3,
                                 ಒಳಹರಿವು_Array src4, ಒಳಹರಿವು_Array src5, ಒಳಹರಿವು_Array src6,
                                 ಒಳಹರಿವು_Array src7, ಒಳಹರಿವು_Array src8, ಒಳಹರಿವು_Array src9)
{
    ಪ್ರತ್ಯಾಗಮ predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
ಶೂನ್ಯ buildOptionsAddMatrixDescription(String& buildOptions, ಧ್ರುವ String& name, ಒಳಹರಿವು_Array _m)
{
    ಯದಿ (!buildOptions.empty())
        buildOptions += " ";
    ಪೂರ್ಣಾಙ್ಕ type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)CV_MAT_CN(type),
            name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)CV_ELEM_SIZE(type),
            name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)CV_ELEM_SIZE1(type),
            name.c_str(), (ಪೂರ್ಣಾಙ್ಕ)depth
            );
}


ಸಂರಚನಾ Image2D::Impl
{
    Impl(ಧ್ರುವ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        ಯದಿ (handle)
            clReleaseMemObject(handle);
    }

    ಸ್ಥಿರ cl_image_format getImageFormat(ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ cn, ತಾರ್ಕಿಕ norm)
    {
        cl_image_format format;
        ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ಸ್ಥಿರ ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        ಪೂರ್ಣಾಙ್ಕ channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        ಪೂರ್ಣಾಙ್ಕ channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        ಪ್ರತ್ಯಾಗಮ format;
    }

    ಸ್ಥಿರ ತಾರ್ಕಿಕ isFormatSupported(cl_image_format format)
    {
        ಯದಿ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        ಯದಿ (!context)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

        // Figure out how many formats are supported by this context.
        cl_uint ಅಂಕೆ_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, ಅಂಕೆ_Formats,
                                                ರಿಕ್ತ, &ಅಂಕೆ_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        ಯದಿ (ಅಂಕೆ_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(ಅಂಕೆ_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, ಅಂಕೆ_Formats,
                                             formats.data(), ರಿಕ್ತ);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            ಪ್ರತಿ (cl_uint i = 0; i < ಅಂಕೆ_Formats; ++i)
            {
                ಯದಿ (!memcmp(&formats[i], &format, ಪರಿಮಾಣ(format)))
                {
                    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }

    ಶೂನ್ಯ init(ಧ್ರುವ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
    {
        ಯದಿ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        ಪೂರ್ಣಾಙ್ಕ err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        ಯದಿ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        ಯದಿ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ಧ್ರುವ Device & d = ocl::Device::getDefault();
        ಪೂರ್ಣಾಙ್ಕ minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        ಯದಿ (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_width      = src.cols;
            desc.image_height     = src.rows;
            desc.image_depth      = 0;
            desc.image_array_size = 1;
            desc.image_row_pitch  = alias ? src.step[0] : 0;
            desc.image_slice_pitch = 0;
            desc.buffer           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.ಅಂಕೆ_mip_levels   = 0;
            desc.ಅಂಕೆ_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ರಿಕ್ತ, &err);
        }
        ಅನ್ಯಥಾ
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, ರಿಕ್ತ, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        ಯದಿ (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), ರಿಕ್ತ, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long ಪೂರ್ಣಾಙ್ಕ)(src.cols * src.rows * src.elemSize()), (ಶೂನ್ಯ*)devData
                ).c_str());

            ಧ್ರುವ size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, ರಿಕ್ತ, ರಿಕ್ತ));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        ಅನ್ಯಥಾ
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != ರಿಕ್ತ);

        ಯದಿ (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, ರಿಕ್ತ, 0));
            ಯದಿ (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(queue));
                CV_OCL_DBG_CHECK(clReleaseMemObject(devData));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = ರಿಕ್ತ;
}

Image2D::Image2D(ಧ್ರುವ UMat &src, ತಾರ್ಕಿಕ norm, ತಾರ್ಕಿಕ alias)
{
    p = ನೂತನ Impl(src, norm, alias);
}

ತಾರ್ಕಿಕ Image2D::canCreateAlias(ಧ್ರುವ UMat &m)
{
    ತಾರ್ಕಿಕ ret = ಅಸತ್ಯ;
    ಧ್ರುವ Device & d = ocl::Device::getDefault();
    ಯದಿ (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        ಯದಿ (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            ಯದಿ (!m.u->tempUMat())
            {
                ret = ಸತ್ಯ;
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ ret;
}

ತಾರ್ಕಿಕ Image2D::isFormatSupported(ಪೂರ್ಣಾಙ್ಕ depth, ಪೂರ್ಣಾಙ್ಕ cn, ತಾರ್ಕಿಕ norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    ಪ್ರತ್ಯಾಗಮ Impl::isFormatSupported(format);
}

Image2D::Image2D(ಧ್ರುವ Image2D & i)
{
    p = i.p;
    ಯದಿ (p)
        p->addref();
}

Image2D & Image2D::operator = (ಧ್ರುವ Image2D & i)
{
    ಯದಿ (i.p != p)
    {
        ಯದಿ (i.p)
            i.p->addref();
        ಯದಿ (p)
            p->release();
        p = i.p;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    ಯದಿ (ಏತತ್ != &i) {
        ಯದಿ (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}

Image2D::~Image2D()
{
    ಯದಿ (p)
        p->release();
}

ಶೂನ್ಯ* Image2D::ptr() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ p ? p->handle : 0;
}

ತಾರ್ಕಿಕ internal::isOpenCLForced()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ value;
}

ತಾರ್ಕಿಕ internal::isPerformanceCheckBypassed()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ ತಾರ್ಕಿಕ value = ಅಸತ್ಯ;
    ಯದಿ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", ಅಸತ್ಯ);
        initialized = ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ value;
}

ತಾರ್ಕಿಕ internal::isCLBuffer(UMat& u)
{
    ಯದಿ (isCudaBacked(u.u))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಶೂನ್ಯ* h = u.handle(ACCESS_RW);
    ಯದಿ (!h)
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#ಯದಿ 1
    ಯದಿ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, ಪರಿಮಾಣ(type), &type, ರಿಕ್ತ);
    ಯದಿ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
}

ಸಂರಚನಾ Timer::Impl
{
    ಧ್ರುವ Queue queue;

    Impl(ಧ್ರುವ Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    ಶೂನ್ಯ start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    ಶೂನ್ಯ stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ಧ್ರುವ
    {
        ಪ್ರತ್ಯಾಗಮ (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ಧ್ರುವ Queue& q) : p(ನೂತನ Impl(q)) { }
Timer::~Timer() { delete p; }

ಶೂನ್ಯ Timer::start()
{
    CV_Assert(p);
    p->start();
}

ಶೂನ್ಯ Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ಧ್ರುವ
{
    CV_Assert(p);
    ಪ್ರತ್ಯಾಗಮ p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
