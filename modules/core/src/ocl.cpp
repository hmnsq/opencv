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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ifndef HAVE_OPENCL
#ଅନ୍ତର୍ଭାଵ "ocl_disabled.impl.hpp"
#ଅନ୍ୟଥା // HAVE_OPENCL

#ଅନ୍ତର୍ଭାଵ <list>
#ଅନ୍ତର୍ଭାଵ <map>
#ଅନ୍ତର୍ଭାଵ <deque>
#ଅନ୍ତର୍ଭାଵ <set>
#ଅନ୍ତର୍ଭାଵ <string>
#ଅନ୍ତର୍ଭାଵ <sstream>
#ଅନ୍ତର୍ଭାଵ <fstream>
#ଯଦି !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#endif

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ଅନ୍ତର୍ଭାଵ "opencv2/core/ocl_genbase.hpp"
#ଅନ୍ତର୍ଭାଵ "opencl_kernels_core.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/lock.private.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ଖାଲି, 0, "OpenCL allocator: " << __VA_ARGS__)
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#ଅନ୍ତର୍ଭାଵ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# ଯଦି 0
#   define LOG_BUFFER_POOL printf
# ଅନ୍ୟଥା
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#ଯଦି CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#ଅନ୍ୟଥା
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#ଯଦି CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#ଅନ୍ୟଥା
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#ଅନ୍ତର୍ଭାଵ "umatrix.hpp"

ନାମାକାଶ cv { ନାମାକାଶ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    ଶୂନ୍ୟ addref() { CV_XADD(&refcount, 1); } \
    ଶୂନ୍ୟ release() { ଯଦି( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete ଏହି; } \
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା refcount

ସ୍ଥିର cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ଫେରନ୍ତୁ opencl_allocator_stats;
}

#ifndef _DEBUG
ସ୍ଥିର ତାର୍କିକ isRaiseError()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ value = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ value;
}
#endif

ସ୍ଥିର ଶୂନ୍ୟ onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    ତାର୍କିକ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", ମିଥ୍ୟା);
    ଯଦି (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#ଯଦି CV_OPENCL_TRACE_CHECK
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ traceOpenCLCheck(cl_int status, ଅଚଳ ଅକ୍ଷର* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#ଅନ୍ୟଥା
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_ସନ୍ଦେଶ(check_result, ସନ୍ଦେଶ) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, ସନ୍ଦେଶ)

#define CV_OCL_CHECK_RESULT(check_result, ସନ୍ଦେଶ) \
    କର { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, ସନ୍ଦେଶ); \
        ଯଦି (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(ସନ୍ଦେଶ), ଅଚଳ ଅକ୍ଷର*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_ସନ୍ଦେଶ = CV_OCL_API_ERROR_ସନ୍ଦେଶ(check_result, ସନ୍ଦେଶ); \
            CV_Error(Error::OpenCLApiCallError, error_ସନ୍ଦେଶ); \
        } \
    } ଯେତେବେଳେ (0)

#define CV_OCL_CHECK_(ପଦସମଷ୍ଟି, check_result) କର { ପଦସମଷ୍ଟି; CV_OCL_CHECK_RESULT(check_result, #expr); } ଯେତେବେଳେ (0)

#define CV_OCL_CHECK(ପଦସମଷ୍ଟି) କର { cl_int __cl_result = (ପଦସମଷ୍ଟି); CV_OCL_CHECK_RESULT(__cl_result, #expr); } ଯେତେବେଳେ (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, ସନ୍ଦେଶ) CV_OCL_CHECK_RESULT(check_result, ସନ୍ଦେଶ)
#define CV_OCL_DBG_CHECK(ପଦସମଷ୍ଟି) CV_OCL_CHECK(ପଦସମଷ୍ଟି)
#define CV_OCL_DBG_CHECK_(ପଦସମଷ୍ଟି, check_result) CV_OCL_CHECK_(ପଦସମଷ୍ଟି, check_result)
#ଅନ୍ୟଥା
#define CV_OCL_DBG_CHECK_RESULT(check_result, ସନ୍ଦେଶ) \
    କର { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, ସନ୍ଦେଶ); \
        ଯଦି (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(ସନ୍ଦେଶ), ଅଚଳ ଅକ୍ଷର*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_ସନ୍ଦେଶ = CV_OCL_API_ERROR_ସନ୍ଦେଶ(check_result, ସନ୍ଦେଶ); \
            CV_Error(Error::OpenCLApiCallError, error_ସନ୍ଦେଶ); \
        } \
    } ଯେତେବେଳେ (0)
#define CV_OCL_DBG_CHECK_(ପଦସମଷ୍ଟି, check_result) କର { ପଦସମଷ୍ଟି; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } ଯେତେବେଳେ (0)
#define CV_OCL_DBG_CHECK(ପଦସମଷ୍ଟି) କର { cl_int __cl_result = (ପଦସମଷ୍ଟି); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } ଯେତେବେଳେ (0)
#endif


ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", ସତ୍ୟ);
ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", ସତ୍ୟ);
ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", ସତ୍ୟ);
ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", ସତ୍ୟ);

#ଯଦି CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS
ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS_ମୂଲ୍ୟ = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", ମିଥ୍ୟା);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#ଯଦି 1 //def __APPLE__
        ସତ୍ୟ
#ଅନ୍ୟଥା
        ମିଥ୍ୟା
#endif
);

ସ୍ଥିର String getBuildExtraOptions()
{
    ସ୍ଥିର String param_buildExtraOptions;
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = ସତ୍ୟ;
        ଯଦି (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ଖାଲି, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ଫେରନ୍ତୁ param_buildExtraOptions;
}

ସ୍ଥିର ଅଚଳ ତାର୍କିକ CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", ସତ୍ୟ);
ସ୍ଥିର ଅଚଳ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ସଂରଚନା UMat2D
{
    UMat2D(ଅଚଳ UMat& m)
    {
        offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.offset;
        step = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା step;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols;
};

ସଂରଚନା UMat3D
{
    UMat3D(ଅଚଳ UMat& m)
    {
        offset = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.offset;
        step = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.step.p[1];
        slicestep = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.step.p[0];
        slices = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା slicestep;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା step;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା slices;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ସ୍ଥିର uint64 crc64( ଅଚଳ uchar* data, size_t size, uint64 crc0=0 )
{
    ସ୍ଥିର uint64 table[256];
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;

    ଯଦି( !initialized )
    {
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 256; i++ )
        {
            uint64 c = i;
            ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = ସତ୍ୟ;
    }

    uint64 crc = ~crc0;
    ପାଇଁ( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    ଫେରନ୍ତୁ ~crc;
}

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
ସଂରଚନା OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ଖାଲି, "Initializing OpenCL cache configuration...");
        ଯଦି (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL cache is disabled");
            ଫେରନ୍ତୁ;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        ଯଦି (cache_path_.empty())
        {
            CV_LOG_INFO(ଖାଲି, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        କର
        {
            ଚେଷ୍ଟା
            {
                ଯଦି (cache_path_.empty())
                    ଭାଙ୍ଗ;
                ଯଦି (cache_path_ == "disabled")
                    ଭାଙ୍ଗ;
                ଯଦି (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(ଖାଲି, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    ଭାଙ୍ଗ;
                }

                ଯଦି (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    ଯଦି (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(ଖାଲି, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        ଯଦି (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(ଖାଲି, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            ଭାଙ୍ଗ;
                        }
                    }

                    ଚେଷ୍ଟା
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(ଖାଲି, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(ଖାଲି, 0, "Checking cache lock... Done!");
                    }
                    ଧର (ଅଚଳ cv::Exception& e)
                    {
                        CV_LOG_WARNING(ଖାଲି, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    ଧର (...)
                    {
                        CV_LOG_WARNING(ଖାଲି, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                ଅନ୍ୟଥା
                {
                    ଯଦି (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ଖାଲି, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    ଅନ୍ୟଥା
                    {
                        CV_LOG_INFO(ଖାଲି, "OpenCL cache lock is disabled");
                    }
                }
            }
            ଧର (ଅଚଳ cv::Exception& e)
            {
                CV_LOG_WARNING(ଖାଲି, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } ଯେତେବେଳେ (0);
        ଯଦି (!cache_path_.empty())
        {
            ଯଦି (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(ଖାଲି, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_INFO(ଖାଲି, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    ଶୂନ୍ୟ clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ଅଚଳ std::string& ctx_prefix,
            ଅଚଳ std::string& cleanup_prefix)
    {
        ଯଦି (cache_path_.empty())
            ଫେରନ୍ତୁ std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        ଯଦି (found_it != prepared_contexts_.end())
            ଫେରନ୍ତୁ found_it->second;

        CV_LOG_INFO(ଖାଲି, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        ତାର୍କିକ result = utils::fs::isDirectory(target_directory);
        ଯଦି (!result)
        {
            ଚେଷ୍ଟା
            {
                CV_LOG_VERBOSE(ଖାଲି, 0, "Creating directory: " << target_directory);
                ଯଦି (utils::fs::createDirectories(target_directory))
                {
                    result = ସତ୍ୟ;
                }
                ଅନ୍ୟଥା
                {
                    CV_LOG_WARNING(ଖାଲି, "Can't create directory: " << target_directory);
                }
            }
            ଧର (ଅଚଳ cv::Exception& e)
            {
                CV_LOG_ERROR(ଖାଲି, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        ଯଦି (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            ଚେଷ୍ଟା
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, ମିଥ୍ୟା, ସତ୍ୟ);
                std::vector<String> remove_entries;
                ପାଇଁ (size_t i = 0; i < entries.size(); i++)
                {
                    ଅଚଳ String& name = entries[i];
                    ଯଦି (0 == name.find(cleanup_prefix))
                    {
                        ଯଦି (0 == name.find(ctx_prefix))
                            ଜାରି; // skip current
                        remove_entries.push_back(name);
                    }
                }
                ଯଦି (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ଖାଲି, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ଖାଲି, "Trying to remove these directories...");
                    ପାଇଁ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ଖାଲି, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ଖାଲି, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    ପାଇଁ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ଅଚଳ String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        ଚେଷ୍ଟା
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(ଖାଲି, "Removed: " << path);
                        }
                        ଧର (ଅଚଳ cv::Exception& e)
                        {
                            CV_LOG_ERROR(ଖାଲି, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            ଧର (...)
            {
                CV_LOG_WARNING(ଖାଲି, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ଖାଲି, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        ଫେରନ୍ତୁ target_directory;
    }

    ସ୍ଥିର OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, ନୂତନ OpenCLBinaryCacheConfigurator());
    }
};
ଶ୍ରେଣୀ ଦ୍ୱିମିକ_Program_File
{
    ଗଣନା { MAX_ENTRIES = 64 };

    ପ୍ରକାରବ୍ୟାଖ୍ୟା unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା uint32_t;

    ସଂରଚନା CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    ସଂରଚନା CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t ସଂଖ୍ୟା_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    ସଂରଚନା CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t ଚାବି_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ଅଚଳ std::string fileName_;
    ଅଚଳ ଅକ୍ଷର* ଅଚଳ sourceSignature_;
    ଅଚଳ size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ଅଚଳ std::string& options)
    {
        uint64 hash = crc64((ଅଚଳ uchar*)options.c_str(), options.size(), 0);
        ଫେରନ୍ତୁ hash & (MAX_ENTRIES - 1);
    }

    ଧାଡ଼ିଭିତରେ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ଫେରନ୍ତୁ fileSize;
    }
    ଧାଡ଼ିଭିତରେ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.ପଠ((ଅକ୍ଷର*)&res, ଆକାର(uint32_t));
        CV_Assert(!f.fail());
        ଫେରନ୍ତୁ res;
    }
    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ writeUInt32(ଅଚଳ uint32_t value)
    {
        uint32_t v = value;
        f.ଲିଖ((ଅକ୍ଷର*)&v, ଆକାର(uint32_t));
        CV_Assert(!f.fail());
    }

    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    ଶୂନ୍ୟ clearFile()
    {
        f.close();
        ଯଦି (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ଖାଲି, "Can't remove: " << fileName_);
        ଫେରନ୍ତୁ;
    }

ସର୍ବସାଧାରଣ:
    ଦ୍ୱିମିକ_Program_File(ଅଚଳ std::string& fileName, ଅଚଳ ଅକ୍ଷର* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(ଆକାର(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != ଖାଲି);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, ଆକାର(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        ଯଦି(f.is_open() && getFileSize() > 0)
        {
            ତାର୍କିକ isValid = ମିଥ୍ୟା;
            ଚେଷ୍ଟା
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                ଯଦି (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<ଅକ୍ଷର> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.ପଠ(fileSourceSignature.data(), fileSourceSignatureSize);
                    ଯଦି (f.eof())
                    {
                        CV_LOG_ERROR(ଖାଲି, "Unexpected EOF");
                    }
                    ଅନ୍ୟଥା ଯଦି (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = ସତ୍ୟ;
                    }
                }
                ଯଦି (!isValid)
                {
                    CV_LOG_ERROR(ଖାଲି, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            ଧର (ଅଚଳ cv::Exception& e)
            {
                CV_LOG_ERROR(ଖାଲି, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            ଧର (...)
            {
                CV_LOG_ERROR(ଖାଲି, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            ଯଦି (!isValid)
            {
                clearFile();
            }
            ଅନ୍ୟଥା
            {
                seekReadAbsolute(0);
            }
        }
    }

    ତାର୍କିକ ପଠ(ଅଚଳ std::string& ଚାବି, std::vector<ଅକ୍ଷର>& buf)
    {
        ଯଦି (!f.is_open())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        size_t fileSize = getFileSize();
        ଯଦି (fileSize == 0)
        {
            CV_LOG_ERROR(ଖାଲି, "Invalid file (empty): " << fileName_);
            clearFile();
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t ସଂଖ୍ୟା_Of_Entries = readUInt32();
        CV_Assert(ସଂଖ୍ୟା_Of_Entries > 0);
        ଯଦି (ସଂଖ୍ୟା_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ଖାଲି, "Invalid file: " << fileName_);
            clearFile();
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        f.ପଠ((ଅକ୍ଷର*)&entryOffsets[0], ଆକାର(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ଅଙ୍କ = getHash(ଚାବି);

        uint32_t entryOffset = entryOffsets[entry_ଅଙ୍କ];
        FileEntry entry;
        ଯେତେବେଳେ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ପଠ((ଅକ୍ଷର*)&entry, ଆକାର(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ଅକ୍ଷର> file_ଚାବି(entry.ଚାବି_Size + 1);
            ଯଦି (ଚାବି.size() == entry.ଚାବି_Size)
            {
                ଯଦି (entry.ଚାବି_Size > 0)
                {
                    f.ପଠ(file_ଚାବି.data(), entry.ଚାବି_Size);
                    CV_Assert(!f.fail());
                }
                ଯଦି (memcmp(file_ଚାବି.data(), ଚାବି.c_str(), entry.ଚାବି_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.ପଠ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ଖାଲି, 0, "Read...");
                    ଫେରନ୍ତୁ ସତ୍ୟ;
                }
            }
            ଯଦି (entry.nextEntryFileOffset == 0)
                ଭାଙ୍ଗ;
            entryOffset = entry.nextEntryFileOffset;
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ତାର୍କିକ ଲିଖ(ଅଚଳ std::string& ଚାବି, std::vector<ଅକ୍ଷର>& buf)
    {
        ଯଦି (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            ଯଦି (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                ଯଦି (!f.is_open())
                {
                    CV_LOG_ERROR(ଖାଲି, "Can't create file: " << fileName_);
                    ଫେରନ୍ତୁ ମିଥ୍ୟା;
                }
            }
        }

        size_t fileSize = getFileSize();
        ଯଦି (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.ଲିଖ(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, ଆକାର(entryOffsets));
            f.ଲିଖ((ଅକ୍ଷର*)entryOffsets, ଆକାର(entryOffsets));
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

        uint32_t ସଂଖ୍ୟା_Of_Entries = readUInt32();
        CV_Assert(ସଂଖ୍ୟା_Of_Entries > 0);
        ଯଦି (ସଂଖ୍ୟା_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ଖାଲି, "Invalid file: " << fileName_);
            clearFile();
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.ପଠ((ଅକ୍ଷର*)&entryOffsets[0], ଆକାର(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ଅଙ୍କ = getHash(ଚାବି);

        uint32_t entryOffset = entryOffsets[entry_ଅଙ୍କ];
        FileEntry entry;
        ଯେତେବେଳେ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.ପଠ((ଅକ୍ଷର*)&entry, ଆକାର(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ଅକ୍ଷର> file_ଚାବି(entry.ଚାବି_Size + 1);
            ଯଦି (ଚାବି.size() == entry.ଚାବି_Size)
            {
                ଯଦି (entry.ଚାବି_Size > 0)
                {
                    f.ପଠ(file_ଚାବି.data(), entry.ଚାବି_Size);
                    CV_Assert(!f.fail());
                }
                ଯଦି (0 == memcmp(file_ଚାବି.data(), ଚାବି.c_str(), entry.ଚାବି_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ଖାଲି, 0, "Duplicate key ignored: " << fileName_);
                    ଫେରନ୍ତୁ ମିଥ୍ୟା;
                }
            }
            ଯଦି (entry.nextEntryFileOffset == 0)
                ଭାଙ୍ଗ;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        ଯଦି (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.ଲିଖ((ଅକ୍ଷର*)&entry, ଆକାର(entry));
            CV_Assert(!f.fail());
        }
        ଅନ୍ୟଥା
        {
            entryOffsets[entry_ଅଙ୍କ] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.ଲିଖ((ଅକ୍ଷର*)entryOffsets, ଆକାର(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.ଚାବି_Size = (uint32_t)ଚାବି.size();
        f.ଲିଖ((ଅକ୍ଷର*)&entry, ଆକାର(entry));
        CV_Assert(!f.fail());
        f.ଲିଖ(ଚାବି.c_str(), entry.ଚାବି_Size);
        CV_Assert(!f.fail());
        f.ଲିଖ(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ଖାଲି, 0, "Write... (" << buf.size() << " bytes)");
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



ସଂରଚନା OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା device_;  // device index in context
    ocl::Queue queue_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useOpenCL_;

ସୁରକ୍ଷିତ:
    Impl() = delete;

    ଶୂନ୍ୟ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndevices = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)context_.ndevices();
        CV_Assert(ndevices > 0);
        ତାର୍କିକ found = ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            ଯଦି (dhandle == deviceID)
            {
                device_ = i;
                found = ସତ୍ୟ;
                ଭାଙ୍ଗ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    ଶୂନ୍ୟ _init_device(ଅଚଳ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ndevices = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)context_.ndevices();
        CV_Assert(ndevices > 0);
        ତାର୍କିକ found = ମିଥ୍ୟା;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            ଯଦି (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = ସତ୍ୟ;
                ଭାଙ୍ଗ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

ସର୍ବସାଧାରଣ:
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

    Impl(ଅଚଳ ocl::Context& context, ଅଚଳ ocl::Device& device, ଅଚଳ ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ଅଚଳ ocl::Context& context, ଅଚଳ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ଅଚଳ ocl::Context& context, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା device, ଅଚଳ ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ଅଚଳ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    ଧାଡ଼ିଭିତରେ ତାର୍କିକ useOpenCL() ଅଚଳ { ଫେରନ୍ତୁ const_cast<Impl*>(ଏହି)->useOpenCL(); }
    ତାର୍କିକ useOpenCL()
    {
        ଯଦି (useOpenCL_ < 0)
        {
            ଚେଷ୍ଟା
            {
                useOpenCL_ = 0;
                ଯଦି (!context_.empty() && context_.ndevices() > 0)
                {
                    ଅଚଳ Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            ଧର (ଅଚଳ cv::Exception&)
            {
                // nothing
            }
            ଯଦି (!useOpenCL_)
                CV_LOG_INFO(ଖାଲି, "OpenCL: can't use OpenCL execution context");
        }
        ଫେରନ୍ତୁ useOpenCL_ > 0;
    }

    ଶୂନ୍ୟ setUseOpenCL(ତାର୍କିକ flag)
    {
        ଯଦି (!flag)
            useOpenCL_ = 0;
        ଅନ୍ୟଥା
            useOpenCL_ = -1;
    }

    ସ୍ଥିର ଅଚଳ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ଖାଲି, "OpenCL: initializing thread execution context");

        ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
        ସ୍ଥିର std::shared_ptr<Impl> g_primaryExecutionContext;

        ଯଦି (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            ଯଦି (!initialized)
            {
                CV_LOG_INFO(ଖାଲି, "OpenCL: creating new execution context...");
                ଚେଷ୍ଟା
                {
                    Context c = ocl::Context::create(std::string());
                    ଯଦି (c.ndevices())
                    {
                        ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceId = 0;
                        auto& d = c.device(deviceId);
                        ଯଦି (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            ଯଦି (!q.ptr())
                            {
                                CV_LOG_ERROR(ଖାଲି, "OpenCL: Can't create default OpenCL queue");
                            }
                            ଅନ୍ୟଥା
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ଖାଲି, "OpenCL: device=" << d.name());
                            }
                        }
                        ଅନ୍ୟଥା
                        {
                            CV_LOG_ERROR(ଖାଲି, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    ଅନ୍ୟଥା
                    {
                        CV_LOG_INFO(ଖାଲି, "OpenCL: context is not available/disabled");
                    }
                }
                ଧର (ଅଚଳ std::exception& e)
                {
                    CV_LOG_INFO(ଖାଲି, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                ଧର (...)
                {
                    CV_LOG_WARNING(ଖାଲି, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = ସତ୍ୟ;
            }
        }
        ଫେରନ୍ତୁ g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->context_;
}
Device& OpenCLExecutionContext::getDevice() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->queue_;
}

ତାର୍କିକ OpenCLExecutionContext::useOpenCL() ଅଚଳ
{
    ଯଦି (p)
        ଫେରନ୍ତୁ p->useOpenCL();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}
ଶୂନ୍ୟ OpenCLExecutionContext::setUseOpenCL(ତାର୍କିକ flag)
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
    ଯଦି (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = ସତ୍ୟ;
        ଯଦି (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ଫେରନ୍ତୁ c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    ଫେରନ୍ତୁ c;
}

ଶୂନ୍ୟ OpenCLExecutionContext::bind() ଅଚଳ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *ଏହି;
    data.oclExecutionContextInitialized = ସତ୍ୟ;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ଅଚଳ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ଅଚଳ Queue q(getContext(), getDevice());
    ଫେରନ୍ତୁ cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ଅଚଳ ocl::Queue& q) ଅଚଳ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ଖାଲି);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ଫେରନ୍ତୁ c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ଅଚଳ Context& context, ଅଚଳ Device& device, ଅଚଳ ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    ଯଦି (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    ଫେରନ୍ତୁ ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ଅଚଳ Context& context, ଅଚଳ Device& device)
{
    CV_TRACE_FUNCTION();
    ଯଦି (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ଫେରନ୍ତୁ ctx;

}

ଶୂନ୍ୟ OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ସ୍ଥିର ତାର୍କିକ g_isOpenCLInitialized = ମିଥ୍ୟା;
ସ୍ଥିର ତାର୍କିକ g_isOpenCLAvailable = ମିଥ୍ୟା;

ତାର୍କିକ haveOpenCL()
{
    CV_TRACE_FUNCTION();

    ଯଦି (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        ଯଦି (!envPath.empty())
        {
            ଯଦି (envPath == "disabled")
            {
                g_isOpenCLAvailable = ମିଥ୍ୟା;
                g_isOpenCLInitialized = ସତ୍ୟ;
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(ଖାଲି, "Initialize OpenCL runtime...");
        ଚେଷ୍ଟା
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ଖାଲି, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ଖାଲି, "OpenCL: found " << n << " platforms");
        }
        ଧର (...)
        {
            g_isOpenCLAvailable = ମିଥ୍ୟା;
        }
        g_isOpenCLInitialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ g_isOpenCLAvailable;
}

ତାର୍କିକ useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    ଯଦି (data.useOpenCL < 0)
    {
        ଚେଷ୍ଟା
        {
            data.useOpenCL = 0;
            ଯଦି (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        ଧର (...)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ଫେରନ୍ତୁ data.useOpenCL > 0;
}

ତାର୍କିକ isOpenCLActivated()
{
    ଯଦି (!g_isOpenCLAvailable)
        ଫେରନ୍ତୁ ମିଥ୍ୟା; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ଫେରନ୍ତୁ useOpenCL();
}

ଶୂନ୍ୟ setUseOpenCL(ତାର୍କିକ flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    ଯଦି (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    ଅନ୍ୟଥା
    {
        ଯଦି (!flag)
            data.useOpenCL = 0;
        ଅନ୍ୟଥା
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

ଶ୍ରେଣୀ AmdBlasHelper
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, ନୂତନ AmdBlasHelper())
    }

    ତାର୍କିକ isAvailable() ଅଚଳ
    {
        ଫେରନ୍ତୁ g_isAmdBlasAvailable;
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

ସୁରକ୍ଷିତ:
    AmdBlasHelper()
    {
        ଯଦି (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            ଯଦି (!g_isAmdBlasInitialized)
            {
                ଯଦି (haveOpenCL())
                {
                    ଚେଷ୍ଟା
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    ଧର (...)
                    {
                        g_isAmdBlasAvailable = ମିଥ୍ୟା;
                    }
                }
                ଅନ୍ୟଥା
                    g_isAmdBlasAvailable = ମିଥ୍ୟା;

                g_isAmdBlasInitialized = ସତ୍ୟ;
            }
        }
    }

ବ୍ୟକ୍ତିଗତ:
    ସ୍ଥିର ତାର୍କିକ g_isAmdBlasInitialized;
    ସ୍ଥିର ତାର୍କିକ g_isAmdBlasAvailable;
};

ତାର୍କିକ AmdBlasHelper::g_isAmdBlasAvailable = ମିଥ୍ୟା;
ତାର୍କିକ AmdBlasHelper::g_isAmdBlasInitialized = ମିଥ୍ୟା;

ତାର୍କିକ haveAmdBlas()
{
    ଫେରନ୍ତୁ AmdBlasHelper::getInstance().isAvailable();
}

#ଅନ୍ୟଥା

ତାର୍କିକ haveAmdBlas()
{
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

#endif

#ifdef HAVE_CLAMDFFT

ଶ୍ରେଣୀ AmdFftHelper
{
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, ନୂତନ AmdFftHelper())
    }

    ତାର୍କିକ isAvailable() ଅଚଳ
    {
        ଫେରନ୍ତୁ g_isAmdFftAvailable;
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

ସୁରକ୍ଷିତ:
    AmdFftHelper()
    {
        ଯଦି (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            ଯଦି (!g_isAmdFftInitialized)
            {
                ଯଦି (haveOpenCL())
                {
                    ଚେଷ୍ଟା
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = ସତ୍ୟ;
                    }
                    ଧର (ଅଚଳ Exception &)
                    {
                        g_isAmdFftAvailable = ମିଥ୍ୟା;
                    }
                }
                ଅନ୍ୟଥା
                    g_isAmdFftAvailable = ମିଥ୍ୟା;

                g_isAmdFftInitialized = ସତ୍ୟ;
            }
        }
    }

ବ୍ୟକ୍ତିଗତ:
    ସ୍ଥିର clfftSetupData setupData;
    ସ୍ଥିର ତାର୍କିକ g_isAmdFftInitialized;
    ସ୍ଥିର ତାର୍କିକ g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
ତାର୍କିକ AmdFftHelper::g_isAmdFftAvailable = ମିଥ୍ୟା;
ତାର୍କିକ AmdFftHelper::g_isAmdFftInitialized = ମିଥ୍ୟା;

ତାର୍କିକ haveAmdFft()
{
    ଫେରନ୍ତୁ AmdFftHelper::getInstance().isAvailable();
}

#ଅନ୍ୟଥା

ତାର୍କିକ haveAmdFft()
{
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

#endif

ତାର୍କିକ haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ସଂରଚନା Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = ମିଥ୍ୟା;
    }

    ~Impl() {}

    ଶୂନ୍ୟ init()
    {
        ଯଦି( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            ଯଦି( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            ଯଦି( handle != 0 )
            {
                ଅକ୍ଷର buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, ଆକାର(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = ସତ୍ୟ;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    ତାର୍କିକ initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    ଯଦି(p)
        p->release();
}

Platform::Platform(ଅଚଳ Platform& pl)
{
    p = (Impl*)pl.p;
    ଯଦି(p)
        p->addref();
}

Platform& Platform::operator = (ଅଚଳ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &pl) {
        ଯଦି(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ* Platform::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ଖାଲି, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ସ୍ଥିର Platform p;
    ଯଦି( !p.p )
    {
        p.p = ନୂତନ Impl;
        p.p->init();
    }
    ଫେରନ୍ତୁ p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ସ୍ଥିର ଶୂନ୍ୟ parseOpenCLVersion(ଅଚଳ String &version, ପୂର୍ଣ୍ଣସଂଖ୍ୟା &major, ପୂର୍ଣ୍ଣସଂଖ୍ୟା &minor)
{
    major = minor = 0;
    ଯଦି (10 >= version.length())
        ଫେରନ୍ତୁ;
    ଅଚଳ ଅକ୍ଷର *pstr = version.c_str();
    ଯଦି (0 != strncmp(pstr, "OpenCL ", 7))
        ଫେରନ୍ତୁ;
    size_t ppos = version.find('.', 7);
    ଯଦି (String::npos == ppos)
        ଫେରନ୍ତୁ;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

ସଂରଚନା Device::Impl
{
    Impl(ଶୂନ୍ୟ* d)
        : refcount(1)
        , handle(0)
    {
        ଚେଷ୍ଟା
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        ଧର (...)
        {
            ଫିଙ୍ଗ;
        }
    }

    ଶୂନ୍ୟ _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        ଯେତେବେଳେ (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            ଯଦି (pos2 == String::npos)
                pos2 = extensions_.size();
            ଯଦି (pos2 > pos)
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
        ଯଦି (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        ଅନ୍ୟଥା ଯଦି (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        ଅନ୍ୟଥା ଯଦି (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        ଅନ୍ୟଥା
            vendorID_ = UNKNOWN_VENDOR;

        ଅଚଳ size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        ଯଦି (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ଅଚଳ size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            ଯଦି (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(ଖାଲି, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#ଯଦି 0
        ଯଦି (isExtensionSupported("cl_khr_spir"))
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
        ଯଦି (!cv::__termination)
#endif
        {
            ଯଦି (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) ଅଚଳ
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        ଫେରନ୍ତୁ clGetDeviceInfo(handle, prop, ଆକାର(temp), &temp, &sz) == CL_SUCCESS &&
            sz == ଆକାର(temp) ? _TpOut(temp) : _TpOut();
    }

    ତାର୍କିକ getBoolProp(cl_device_info prop) ଅଚଳ
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        ଫେରନ୍ତୁ clGetDeviceInfo(handle, prop, ଆକାର(temp), &temp, &sz) == CL_SUCCESS &&
            sz == ଆକାର(temp) ? temp != 0 : ମିଥ୍ୟା;
    }

    String getStrProp(cl_device_info prop) ଅଚଳ
    {
        ଅକ୍ଷର buf[4096];
        size_t sz=0;
        ଫେରନ୍ତୁ clGetDeviceInfo(handle, prop, ଆକାର(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ଆକାର(buf) ? String(buf) : String();
    }

    ତାର୍କିକ isExtensionSupported(ଅଚଳ std::string& extensionName) ଅଚଳ
    {
        ଫେରନ୍ତୁ extensions_set_.ଗଣନା_3(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା doubleFPConfig_;
    ତାର୍କିକ khr_fp64_support_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା halfFPConfig_;
    ତାର୍କିକ khr_fp16_support_;
    ତାର୍କିକ hostUnifiedMemory_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxComputeUnits_;
    size_t maxWorkGroupSize_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା addressBits_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceVersionMajor_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vendorID_;
    ତାର୍କିକ intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(ଶୂନ୍ୟ* d)
{
    p = 0;
    set(d);
}

Device::Device(ଅଚଳ Device& d)
{
    p = d.p;
    ଯଦି(p)
        p->addref();
}

Device& Device::operator = (ଅଚଳ Device& d)
{
    Impl* newp = (Impl*)d.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &d) {
        ଯଦି(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Device::~Device()
{
    ଯଦି(p)
        p->release();
}

ଶୂନ୍ୟ Device::set(ଶୂନ୍ୟ* d)
{
    ଯଦି(p)
        p->release();
    p = ନୂତନ Impl(d);
    ଯଦି (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(ଶୂନ୍ୟ* d)
{
    Device device(d);
    ଫେରନ୍ତୁ device;
}

ଶୂନ୍ୟ* Device::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

String Device::name() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->name_ : String(); }

String Device::extensions() ଅଚଳ
{ ଫେରନ୍ତୁ p ? String(p->extensions_) : String(); }

ତାର୍କିକ Device::isExtensionSupported(ଅଚଳ String& extensionName) ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->isExtensionSupported(extensionName) : ମିଥ୍ୟା; }

String Device::version() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->version_ : String(); }

String Device::vendorName() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->vendorName_ : String(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::vendorID() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::deviceVersionMajor() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->deviceVersionMajor_ : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::deviceVersionMinor() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->driverVersion_ : String(); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::type() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->type_ : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::addressBits() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->addressBits_ : 0; }

ତାର୍କିକ Device::available() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : ମିଥ୍ୟା; }

ତାର୍କିକ Device::compilerAvailable() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : ମିଥ୍ୟା; }

ତାର୍କିକ Device::linkerAvailable() ଅଚଳ
#ifdef CL_VERSION_1_2
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : ମିଥ୍ୟା; }
#ଅନ୍ୟଥା
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::doubleFPConfig() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->doubleFPConfig_ : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::singleFPConfig() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_device_fp_config, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::halfFPConfig() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->halfFPConfig_ : 0; }

ତାର୍କିକ Device::hasFP64() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->khr_fp64_support_ : ମିଥ୍ୟା; }
ତାର୍କିକ Device::hasFP16() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->khr_fp16_support_ : ମିଥ୍ୟା; }

ତାର୍କିକ Device::endianLittle() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : ମିଥ୍ୟା; }

ତାର୍କିକ Device::errorCorrectionSupport() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : ମିଥ୍ୟା; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::executionCapabilities() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_device_exec_capabilities, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::globalMemCacheType() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_device_mem_cache_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::global_Mem_Cache_ଧାଡ଼ି_Size() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::localMemType() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_device_local_mem_type, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

ତାର୍କିକ Device::hostUnifiedMemory() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->hostUnifiedMemory_ : ମିଥ୍ୟା; }

ତାର୍କିକ Device::imageSupport() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : ମିଥ୍ୟା; }

ତାର୍କିକ Device::imageFromBufferSupport() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : ମିଥ୍ୟା;
}

uint Device::imagePitchAlignment() ଅଚଳ
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ଫେରନ୍ତୁ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
}

uint Device::imageBaseAddressAlignment() ଅଚଳ
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    ଫେରନ୍ତୁ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
}

size_t Device::image2DMaxWidth() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ଅଚଳ
#ifdef CL_VERSION_1_2
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#ଅନ୍ୟଥା
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ଅଚଳ
#ifdef CL_VERSION_1_2
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#ଅନ୍ୟଥା
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

ତାର୍କିକ Device::intelSubgroupsSupport() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->intelSubgroupsSupport_ : ମିଥ୍ୟା; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxClockFrequency() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxComputeUnits() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->maxComputeUnits_ : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxConstantArgs() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxReadImageArgs() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxWriteImageArgs() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxSamplers() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->maxWorkGroupSize_ : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::maxWorkItemDims() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

ଶୂନ୍ୟ Device::maxWorkItemSizes(size_t* sizes) ଅଚଳ
{
    ଯଦି(p)
    {
        ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*ଆକାର(sizes[0]), &sizes[0], &retsz));
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::memBaseAddrAlign() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthChar() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthShort() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthInt() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthLong() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthFloat() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthDouble() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::nativeVectorWidthHalf() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthChar() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthShort() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthInt() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthLong() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthFloat() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthDouble() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Device::preferredVectorWidthHalf() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<cl_uint, ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ଅଚଳ
#ifdef CL_VERSION_1_2
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#ଅନ୍ୟଥା
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ଅଚଳ
{ ଫେରନ୍ତୁ p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ଅଚଳ Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ଯଦି (!c.empty())
    {
        ଫେରନ୍ତୁ c.getDevice();
    }

    ସ୍ଥିର Device dummy;
    ଫେରନ୍ତୁ dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
ଧାଡ଼ିଭିତରେ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ଖାଲି, &required);
    ଯଦି (err != CL_SUCCESS)
        ଫେରନ୍ତୁ err;

    param.clear();
    ଯଦି (required > 0)
    {
        AutoBuffer<ଅକ୍ଷର> buf(required + 1);
        ଅକ୍ଷର* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, ଖାଲି);
        ଯଦି (err != CL_SUCCESS)
            ଫେରନ୍ତୁ err;
        param = ptr;
    }

    ଫେରନ୍ତୁ CL_SUCCESS;
}

ସ୍ଥିର ଶୂନ୍ୟ split(ଅଚଳ std::string &s, ଅକ୍ଷର delim, std::vector<std::string> &elems)
{
    elems.clear();
    ଯଦି (s.size() == 0)
        ଫେରନ୍ତୁ;
    std::istringstream ss(s);
    std::string item;
    ଯେତେବେଳେ (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ସ୍ଥିର ତାର୍କିକ parseOpenCLDeviceConfiguration(ଅଚଳ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    ଯଦି (parts.size() > 3)
    {
        CV_LOG_ERROR(ଖାଲି, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଯଦି (parts.size() > 2)
        deviceNameOrID = parts[2];
    ଯଦି (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    ଯଦି (parts.size() > 0)
    {
        platform = parts[0];
    }
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସ୍ଥିର cl_device_id selectOpenCLDevice(ଅଚଳ std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    ଯଦି (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    ଯଦି (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ଫେରନ୍ତୁ ଖାଲି;

    ତାର୍କିକ isID = ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceID = -1;
    ଯଦି (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = ସତ୍ୟ;
        ପାଇଁ (size_t i = 0; i < deviceName.length(); i++)
        {
            ଯଦି (!isdigit(deviceName[i]))
            {
                isID = ମିଥ୍ୟା;
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (isID)
        {
            deviceID = atoi(deviceName.c_str());
            ଯଦି (deviceID < 0)
                ଫେରନ୍ତୁ ଖାଲି;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint ଅଙ୍କ_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ଖାଲି, &ଅଙ୍କ_Platforms));

        ଯଦି (ଅଙ୍କ_Platforms == 0)
            ଫେରନ୍ତୁ ଖାଲି;
        platforms.resize((size_t)ଅଙ୍କ_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(ଅଙ୍କ_Platforms, &platforms[0], &ଅଙ୍କ_Platforms));
        platforms.resize(ଅଙ୍କ_Platforms);
    }

    ଯଦି (platform.length() > 0)
    {
        ପାଇଁ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            ଯଦି (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            ଅନ୍ୟଥା
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        ଯଦି (platforms.size() == 0)
        {
            CV_LOG_ERROR(ଖାଲି, "OpenCL: Can't find OpenCL platform by name: " << platform);
            ଯାଅ not_found;
        }
    }
    ଯଦି (deviceTypes.size() == 0)
    {
        ଯଦି (!isID)
        {
            deviceTypes.push_back("GPU");
            ଯଦି (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        ଅନ୍ୟଥା
            deviceTypes.push_back("ALL");
    }
    ପାଇଁ (size_t t = 0; t < deviceTypes.size(); t++)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        ଯଦି (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        ଅନ୍ୟଥା ଯଦି (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        ଅନ୍ୟଥା ଯଦି (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        ଅନ୍ୟଥା ଯଦି (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        ଅନ୍ୟଥା
        {
            CV_LOG_ERROR(ଖାଲି, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            ଯାଅ not_found;
        }

        std::vector<cl_device_id> devices;
        ପାଇଁ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ଗଣନା_3 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, ଖାଲି, &ଗଣନା_3);
            ଯଦି (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            ଯଦି (ଗଣନା_3 == 0)
                ଜାରି;
            size_t base = devices.size();
            devices.resize(base + ଗଣନା_3);
            status = clGetDeviceIDs(*currentPlatform, deviceType, ଗଣନା_3, &devices[base], &ଗଣନା_3);
            ଯଦି (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        ପାଇଁ (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : ସତ୍ୟ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = ସତ୍ୟ;
            ଯଦି(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, ଆକାର(isIGPU), &isIGPU, ଖାଲି));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            ଯଦି ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ଫେରନ୍ତୁ devices[i];
            }
        }
    }

not_found:
    ଯଦି (configuration.empty())
        ଫେରନ୍ତୁ ଖାଲି; // suppress messages on stderr

    std::ostringstream ସନ୍ଦେଶ;
    ସନ୍ଦେଶ << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    ପାଇଁ (size_t t = 0; t < deviceTypes.size(); t++)
        ସନ୍ଦେଶ << ' ' << deviceTypes[t];

    ସନ୍ଦେଶ << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ଖାଲି, ସନ୍ଦେଶ.str());
    ଫେରନ୍ତୁ ଖାଲି;
}

#ifdef HAVE_OPENCL_SVM
ନାମାକାଶ svm {

ଗଣନା AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

ସ୍ଥିର ତାର୍କିକ checkForceSVMUmatUsage()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ force = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ force;
}
ସ୍ଥିର ତାର୍କିକ checkDisableSVMUMatUsage()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ force = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ force;
}
ସ୍ଥିର ତାର୍କିକ checkDisableSVM()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ force = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ force;
}
// see SVMCapabilities
ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା getSVMCapabilitiesMask()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = 0;
    ଯଦି (!initialized)
    {
        ଅଚଳ std::string env_ମୂଲ୍ୟ = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        ଯଦି (env_ମୂଲ୍ୟ.empty())
        {
            ଫେରନ୍ତୁ ~0U; // all bits 1
        }
        mask = atoi(env_ମୂଲ୍ୟ.c_str());
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ mask;
}
} // namespace
#endif

ସ୍ଥିର size_t get_Program_ଗଣନା_Limit()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର size_t ଗଣନା_3 = 0;
    ଯଦି (!initialized)
    {
        ଗଣନା_3 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ଗଣନା_3;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା g_contextId = 0;

ଶ୍ରେଣୀ OpenCLBufferPoolImpl;
ଶ୍ରେଣୀ OpenCLSVMBufferPoolImpl;

ସଂରଚନା Context::Impl
{
    ସ୍ଥିର Context::Impl* get(Context& context) { ଫେରନ୍ତୁ context.getImpl(); }

    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::deque<Context::Impl*> container_t;
    ସ୍ଥିର container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ସ୍ଥିର container_t* g_contexts = ନୂତନ container_t();
        ଫେରନ୍ତୁ *g_contexts;
    }

ସୁରକ୍ଷିତ:
    Impl(ଅଚଳ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(ମିଥ୍ୟା)
#endif
    {
        ଯଦି (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ଏହି;
    }

    ~Impl()
    {
#ifdef _WIN32
        ଯଦି (!cv::__termination)
#endif
        {
            ଯଦି (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ଖାଲି;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ଖାଲି;
        }
    }

    ଶୂନ୍ୟ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_ଅଙ୍କ_DEVICES, ଆକାର(ndevices), &ndevices, ଖାଲି));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * ଆକାର(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * ଆକାର(cl_device_id), "");

        devices.clear();
        ପାଇଁ (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    ଶୂନ୍ୟ __init_buffer_pools();  // w/o synchronization
    ଶୂନ୍ୟ _init_buffer_pools() ଅଚଳ
    {
        ଯଦି (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            ଯଦି (!bufferPool_)
            {
                const_cast<Impl*>(ଏହି)->__init_buffer_pools();
            }
        }
    }
ସର୍ବସାଧାରଣ:
    ସ୍ଥିର Impl* findContext(ଅଚଳ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        ଯଦି (configuration.empty() && !container.empty())
            ଫେରନ୍ତୁ container[0];
        ପାଇଁ (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            ଯଦି (i && i->configuration == configuration)
            {
                ଫେରନ୍ତୁ i;
            }
        }
        ଫେରନ୍ତୁ ଖାଲି;
    }

    ସ୍ଥିର Impl* findOrCreateContext(ଅଚଳ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        ଯଦି (configuration_.empty())
        {
            ଅଚଳ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            ଯଦି (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        ଯଦି (impl)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ଫେରନ୍ତୁ impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        ଯଦି (d == ଖାଲି)
            ଫେରନ୍ତୁ ଖାଲି;

        impl = ନୂତନ Impl(configuration);
        ଚେଷ୍ଟା
        {
            impl->createFromDevice(d);
            ଯଦି (impl->handle)
                ଫେରନ୍ତୁ impl;
            delete impl;
            ଫେରନ୍ତୁ ଖାଲି;
        }
        ଧର (...)
        {
            delete impl;
            ଫିଙ୍ଗ;
        }
    }

    ସ୍ଥିର Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (ଶୂନ୍ୟ*)h);
        Impl* impl = findContext(configuration);
        ଯଦି (impl)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ଫେରନ୍ତୁ impl;
        }

        impl = ନୂତନ Impl(configuration);
        ଚେଷ୍ଟା
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ଫେରନ୍ତୁ impl;
        }
        ଧର (...)
        {
            delete impl;
            ଫିଙ୍ଗ;
        }
    }

    ସ୍ଥିର Impl* findOrCreateContext(ଅଚଳ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (ଶୂନ୍ୟ*)d);
        Impl* impl = findContext(configuration);
        ଯଦି (impl)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ଫେରନ୍ତୁ impl;
        }

        impl = ନୂତନ Impl(configuration);
        ଚେଷ୍ଟା
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ଫେରନ୍ତୁ impl;
        }
        ଧର (...)
        {
            delete impl;
            ଫିଙ୍ଗ;
        }
    }

    ଶୂନ୍ୟ setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        ଯଦି (d == ଖାଲି)
            ଫେରନ୍ତୁ;

        createFromDevice(d);
    }

    ଶୂନ୍ୟ createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ଖାଲି);

        cl_platform_id pl = ଖାଲି;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, ଆକାର(cl_platform_id), &pl, ଖାଲି));

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

        ତାର୍କିକ ok = handle != 0 && status == CL_SUCCESS;
        ଯଦି( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        ଅନ୍ୟଥା
            handle = ଖାଲି;
    }

    Program getProg(ଅଚଳ ProgramSource& src, ଅଚଳ String& buildflags, String& errmsg);

    ଶୂନ୍ୟ unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        ପାଇଁ (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              ଯଦି (it != phash.end())
              {
                  ଯଦି (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ଫେରନ୍ତୁ;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        ଯଦି (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            ଯଦି (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ଅଚଳ Device& d = devices[0];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = d.addressBits();
                ଯଦି (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                ପାଇଁ (size_t i = 0; i < prefix.size(); i++)
                {
                    ଅକ୍ଷର c = prefix[i];
                    ଯଦି (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        ଫେରନ୍ତୁ prefix;
    }

    std::string& getPrefixBase()
    {
        ଯଦି (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            ଯଦି (prefix_base.empty())
            {
                ଅଚଳ Device& d = devices[0];
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା bits = d.addressBits();
                ଯଦି (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                ପାଇଁ (size_t i = 0; i < prefix_base.size(); i++)
                {
                    ଅକ୍ଷର c = prefix_base[i];
                    ଯଦି (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        ଫେରନ୍ତୁ prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା contextId;  // global unique ID
    ଅଚଳ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::map<std::string, Program> phash_t;
    phash_t phash;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ଅଚଳ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        ଫେରନ୍ତୁ *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ଅଚଳ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        ଫେରନ୍ତୁ *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    ଶୂନ୍ୟ setUserContext(std::type_index typeId, ଅଚଳ std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        ଯଦି (it != userContextStorage.end())
            ଫେରନ୍ତୁ it->second;
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    ତାର୍କିକ svmInitialized;
    ତାର୍କିକ svmAvailable;
    ତାର୍କିକ svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    ଶୂନ୍ୟ svmInit()
    {
        CV_Assert(handle != ଖାଲି);
        ଅଚଳ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((ଶୂନ୍ୟ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, ଆକାର(deviceCaps), &deviceCaps, ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            ଯାଅ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)deviceCaps);
        CV_Assert(((ଶୂନ୍ୟ)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.ମୂଲ୍ୟ_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.ମୂଲ୍ୟ_2 &= svm::getSVMCapabilitiesMask();
        ଯଦି (svmCapabilities.ମୂଲ୍ୟ_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            ଯାଅ noSVM;
        }
        ଚେଷ୍ଟା
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            ଶୂନ୍ୟ* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            ଯଦି (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ଚେଷ୍ଟା
            {
                ତାର୍କିକ error = ମିଥ୍ୟା;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                ଯଦି (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ଖାଲି, ଖାଲି))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                ଚେଷ୍ଟା
                {
                    ((ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)ptr)[0] = 100;
                }
                ଧର (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = ସତ୍ୟ;
                }
                ଯଦି (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, ଖାଲି, ଖାଲି))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                ଯଦି (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            ଧର (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ଫିଙ୍ଗ;
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
        ଧର (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ଚେଷ୍ଟା
            {
                // Try HSA extension
                String extensions = device.extensions();
                ଯଦି (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    ଯାଅ noSVM;
                }
                cl_platform_id p = ଖାଲି;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, ଆକାର(cl_platform_id), &p, ଖାଲି));
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
            ଧର (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                ଯାଅ noSVM;
            }
        }

        svmAvailable = ସତ୍ୟ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = ସତ୍ୟ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ଫେରନ୍ତୁ;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = ମିଥ୍ୟା;
        svmEnabled = ମିଥ୍ୟା;
        svmCapabilities.ମୂଲ୍ୟ_2 = 0;
        svmInitialized = ସତ୍ୟ;
        svmFunctions.fn_clSVMAlloc = ଖାଲି;
        ଫେରନ୍ତୁ;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ଅଚଳ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        ଫେରନ୍ତୁ *bufferPoolSVM_.get();
    }
#endif

    friend ଶ୍ରେଣୀ Program;
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
Context::Context(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    p = 0;
    create(dtype);
}

ଶୂନ୍ୟ Context::release()
{
    ଯଦି (p)
    {
        p->release();
        p = ଖାଲି;
    }
}

ତାର୍କିକ Context::create()
{
    release();
    ଯଦି (!haveOpenCL())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    p = Impl::findOrCreateContext(std::string());
    ଯଦି (p && p->handle)
        ଫେରନ୍ତୁ ସତ୍ୟ;
    release();
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

// deprecated
ତାର୍କିକ Context::create(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype)
{
    ଯଦି( !haveOpenCL() )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    release();
    ଯଦି (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    ଅନ୍ୟଥା ଯଦି (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    ଅନ୍ୟଥା ଯଦି (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    ଅନ୍ୟଥା
    {
        CV_LOG_ERROR(ଖାଲି, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    ଯଦି (p && !p->handle)
    {
        release();
    }
    ଫେରନ୍ତୁ p != 0;
}

Context::Context(ଅଚଳ Context& c)
{
    p = (Impl*)c.p;
    ଯଦି(p)
        p->addref();
}

Context& Context::operator = (ଅଚଳ Context& c)
{
    Impl* newp = (Impl*)c.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &c) {
        ଯଦି(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ଶୂନ୍ୟ* Context::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p == ଖାଲି ? ଖାଲି : p->handle;
}

size_t Context::ndevices() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ଅଚଳ
{
    ସ୍ଥିର Device dummy;
    ଫେରନ୍ତୁ !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ଯଦି (!c.empty())
    {
        auto& ctx = c.getContext();
        ଫେରନ୍ତୁ ctx;
    }

    ସ୍ଥିର Context dummy;
    ଫେରନ୍ତୁ dummy;
}

Program Context::getProg(ଅଚଳ ProgramSource& prog,
                         ଅଚଳ String& buildopts, String& errmsg)
{
    ଫେରନ୍ତୁ p ? p->getProg(prog, buildopts, errmsg) : Program();
}

ଶୂନ୍ୟ Context::unloadProg(Program& prog)
{
    ଯଦି (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(ଶୂନ୍ୟ* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    ଫେରନ୍ତୁ ctx;
}

/* static */
Context Context::fromDevice(ଅଚଳ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    ଫେରନ୍ତୁ ctx;
}

/* static */
Context Context::create(ଅଚଳ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ* Context::getOpenCLContextProperty(ପୂର୍ଣ୍ଣସଂଖ୍ୟା propertyId) ଅଚଳ
{
    ଯଦି (p == ଖାଲି)
        ଫେରନ୍ତୁ nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ଖାଲି, &size));
    std::vector<cl_context_properties> prop(size / ଆକାର(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), ଖାଲି));
    ପାଇଁ (size_t i = 0; i < prop.size(); i += 2)
    {
        ଯଦି (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ଖାଲି, "OpenCL: found context property=" << propertyId << ") => " << (ଶୂନ୍ୟ*)prop[i + 1]);
            ଫେରନ୍ତୁ (ଶୂନ୍ୟ*)prop[i + 1];
        }
    }
    ଫେରନ୍ତୁ nullptr;
}

#ifdef HAVE_OPENCL_SVM
ତାର୍କିକ Context::useSVM() ଅଚଳ
{
    Context::Impl* i = p;
    CV_Assert(i);
    ଯଦି (!i->svmInitialized)
        i->svmInit();
    ଫେରନ୍ତୁ i->svmEnabled;
}
ଶୂନ୍ୟ Context::setUseSVM(ତାର୍କିକ enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    ଯଦି (!i->svmInitialized)
        i->svmInit();
    ଯଦି (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#ଅନ୍ୟଥା
ତାର୍କିକ Context::useSVM() ଅଚଳ { ଫେରନ୍ତୁ ମିଥ୍ୟା; }
ଶୂନ୍ୟ Context::setUseSVM(ତାର୍କିକ enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
ନାମାକାଶ svm {

ଅଚଳ SVMCapabilities getSVMCapabilitites(ଅଚଳ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    ଯଦି (!i->svmInitialized)
        i->svmInit();
    ଫେରନ୍ତୁ i->svmCapabilities;
}

CV_EXPORTS ଅଚଳ SVMFunctions* getSVMFunctions(ଅଚଳ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ଖାଲି);
    ଫେରନ୍ତୁ &i->svmFunctions;
}

CV_EXPORTS ତାର୍କିକ useSVM(UMatUsageFlags usageFlags)
{
    ଯଦି (checkForceSVMUmatUsage())
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଯଦି (checkDisableSVMUMatUsage())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        ଫେରନ୍ତୁ ସତ୍ୟ;
    ଫେରନ୍ତୁ ମିଥ୍ୟା; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

ଶୂନ୍ୟ Context::setUserContext(std::type_index typeId, ଅଚଳ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->getUserContext(typeId);
}

ସ୍ଥିର ଶୂନ୍ୟ get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<ଅକ୍ଷର> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
ଶୂନ୍ୟ attachContext(ଅଚଳ String& platformName, ଶୂନ୍ୟ* platformID, ଶୂନ୍ୟ* context, ଶୂନ୍ୟ* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ଅଚଳ std::string& platformName, ଶୂନ୍ୟ* platformID, ଶୂନ୍ୟ* context, ଶୂନ୍ୟ* deviceID
)
{
    ଯଦି (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    ଯଦି (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    ତାର୍କିକ platformAvailable = ମିଥ୍ୟା;

    // check if external platformName contained in list of available platforms in OpenCV
    ପାଇଁ (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        ଯଦି (platformName == availablePlatformName)
        {
            platformAvailable = ସତ୍ୟ;
            ଭାଙ୍ଗ;
        }
    }

    ଯଦି (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    ଯଦି (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ଫେରନ୍ତୁ ctx;
}

ଶୂନ୍ୟ initializeContextFromHandle(Context& ctx, ଶୂନ୍ୟ* _platform, ଶୂନ୍ୟ* _context, ଶୂନ୍ୟ* _device)
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

ସଂରଚନା Queue::Impl
{
    ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = ମିଥ୍ୟା;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, ଆକାର(cl_command_queue_properties), &props, ଖାଲି));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, ତାର୍କିକ isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ଅଚଳ Context& c, ଅଚଳ Device& d, ତାର୍କିକ withProfiling = ମିଥ୍ୟା)
    {
        __init();

        ଅଚଳ Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        ଯଦି( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        ଯଦି( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        ଯଦି (!cv::__termination)
#endif
        {
            ଯଦି(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = ଖାଲି;
            }
        }
    }

    ଅଚଳ cv::ocl::Queue& getProfilingQueue(ଅଚଳ cv::ocl::Queue& self)
    {
        ଯଦି (isProfilingQueue_)
            ଫେରନ୍ତୁ self;

        ଯଦି (profiling_queue_.ptr())
            ଫେରନ୍ତୁ profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, ଆକାର(cl_context), &ctx, ଖାଲି));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, ଆକାର(cl_device_id), &device, ଖାଲି));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = ନୂତନ Impl(q, ସତ୍ୟ);
        profiling_queue_ = queue;

        ଫେରନ୍ତୁ profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    ତାର୍କିକ isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ଅଚଳ Context& c, ଅଚଳ Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ଅଚଳ Queue& q)
{
    p = q.p;
    ଯଦି(p)
        p->addref();
}

Queue& Queue::operator = (ଅଚଳ Queue& q)
{
    Impl* newp = (Impl*)q.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &q) {
        ଯଦି(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Queue::~Queue()
{
    ଯଦି(p)
        p->release();
}

ତାର୍କିକ Queue::create(ଅଚଳ Context& c, ଅଚଳ Device& d)
{
    ଯଦି(p)
        p->release();
    p = ନୂତନ Impl(c, d);
    ଫେରନ୍ତୁ p->handle != 0;
}

ଶୂନ୍ୟ Queue::finish()
{
    ଯଦି(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ଅଚଳ Queue& Queue::getProfilingQueue() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->getProfilingQueue(*ଏହି);
}

ଶୂନ୍ୟ* Queue::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    ଯଦି (!c.empty())
    {
        auto& q = c.getQueue();
        ଫେରନ୍ତୁ q;
    }
    ସ୍ଥିର Queue dummy;
    ଫେରନ୍ତୁ dummy;
}

ସ୍ଥିର cl_command_queue getQueue(ଅଚଳ Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    ଯଦି(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    ଫେରନ୍ତୁ qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା _flags, UMat* _m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _wscale, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _iwscale, ଅଚଳ ଶୂନ୍ୟ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ଖାଲି);
}

KernelArg KernelArg::Constant(ଅଚଳ Mat& m)
{
    CV_Assert(m.isContinuous());
    ଫେରନ୍ତୁ KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ସଂରଚନା Kernel::Impl
{
    Impl(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ Program& prog) :
        refcount(1), handle(ଖାଲି), isInProgress(ମିଥ୍ୟା), isAsyncRun(ମିଥ୍ୟା), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        ଯଦି (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = ମିଥ୍ୟା;
        haveTempSrcUMats = ମିଥ୍ୟା;
    }

    ଶୂନ୍ୟ cleanupUMats()
    {
        ତାର୍କିକ exceptionOccurred = ମିଥ୍ୟା;
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < MAX_ARRS; i++ )
        {
            ଯଦି( u[i] )
            {
                ଯଦି( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    ଚେଷ୍ଟା
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    ଧର(ଅଚଳ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ଖାଲି, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = ସତ୍ୟ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = ମିଥ୍ୟା;
        haveTempSrcUMats = ମିଥ୍ୟା;
        CV_Assert(!exceptionOccurred);
    }

    ଶୂନ୍ୟ addUMat(ଅଚଳ UMat& m, ତାର୍କିକ dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        ଯଦି(dst && m.u->tempUMat())
            haveTempDstUMats = ସତ୍ୟ;
        ଯଦି(m.u->originalUMatData == ଖାଲି && m.u->tempUMat())
            haveTempSrcUMats = ସତ୍ୟ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    ଶୂନ୍ୟ registerImageArgument(ପୂର୍ଣ୍ଣସଂଖ୍ୟା arg, ଅଚଳ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        ଯଦି (arg < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    ଶୂନ୍ୟ finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = ମିଥ୍ୟା;
        ଚେଷ୍ଟା
        {
            cleanupUMats();
        }
        ଧର(...)
        {
            release();
            ଫିଙ୍ଗ;
        }
        release();
    }

    ତାର୍କିକ run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t _globalsize[], size_t _localsize[],
            ତାର୍କିକ ସମକାଳ, int64* timeNS, ଅଚଳ Queue& q);

    ~Impl()
    {
        ଯଦି(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ଗଣନା { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    ତାର୍କିକ isInProgress;
    ତାର୍କିକ isAsyncRun;  // true if kernel was scheduled in async mode
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା nu;
    std::vector<Image2D> shadow_images;
    ତାର୍କିକ haveTempDstUMats;
    ତାର୍କିକ haveTempSrcUMats;
};

}} // namespace cv::ocl

ବାହ୍ୟ "C" {

ସ୍ଥିର ଶୂନ୍ୟ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ଶୂନ୍ୟ *p)
{
    ଚେଷ୍ଟା
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    ଧର (ଅଚଳ cv::Exception& exc)
    {
        CV_LOG_ERROR(ଖାଲି, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    ଧର (ଅଚଳ std::exception& exc)
    {
        CV_LOG_ERROR(ଖାଲି, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    ଧର (...)
    {
        CV_LOG_ERROR(ଖାଲି, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

ନାମାକାଶ cv { ନାମାକାଶ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ ProgramSource& src,
               ଅଚଳ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ଅଚଳ Kernel& k)
{
    p = k.p;
    ଯଦି(p)
        p->addref();
}

Kernel& Kernel::operator = (ଅଚଳ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &k) {
        ଯଦି(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Kernel::~Kernel()
{
    ଯଦି(p)
        p->release();
}

ତାର୍କିକ Kernel::create(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ Program& prog)
{
    ଯଦି(p)
        p->release();
    p = ନୂତନ Impl(kname, prog);
    ଯଦି(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    ଫେରନ୍ତୁ p != 0;
}

ତାର୍କିକ Kernel::create(ଅଚଳ ଅକ୍ଷର* kname, ଅଚଳ ProgramSource& src,
                    ଅଚଳ String& buildopts, String* errmsg)
{
    ଯଦି(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    ଯଦି( !errmsg ) errmsg = &tempmsg;
    ଅଚଳ Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ଫେରନ୍ତୁ create(kname, prog);
}

ଶୂନ୍ୟ* Kernel::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

ତାର୍କିକ Kernel::empty() ଅଚଳ
{
    ଫେରନ୍ତୁ ptr() == 0;
}

ସ୍ଥିର cv::String dump_ମୂଲ୍ୟ(size_t sz, ଅଚଳ ଶୂନ୍ୟ* p)
{
    ଯଦି (!p)
        ଫେରନ୍ତୁ "NULL";
    ଯଦି (sz == 2)
        ଫେରନ୍ତୁ cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    ଯଦି (sz == 4)
        ଫେରନ୍ତୁ cv::format("%d / %uu / 0x%08x / %g", *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)p, *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)p, *(ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)p, *(ଦଶମିକ*)p);
    ଯଦି (sz == 8)
        ଫେରନ୍ତୁ cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(ଦ୍ୱିଗୁଣ*)p);
    ଫେରନ୍ତୁ cv::format("%p", p);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ ଶୂନ୍ୟ* value, size_t sz)
{
    ଯଦି (!p || !p->handle)
        ଫେରନ୍ତୁ -1;
    ଯଦି (i < 0)
        ଫେରନ୍ତୁ i;
    ଯଦି( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz, dump_ମୂଲ୍ୟ(sz, value).c_str()).c_str());
    ଯଦି (retval != CL_SUCCESS)
        ଫେରନ୍ତୁ -1;
    ଫେରନ୍ତୁ i+1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = set(i, &h, ଆକାର(h));
    ଯଦି (res >= 0)
        p->registerImageArgument(i, image2D);
    ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ UMat& m)
{
    ଫେରନ୍ତୁ set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ସ୍ଥିର ତାର୍କିକ isCudaBacked(ଅଚଳ UMatData* u)
{
    ଅଚଳ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ଫେରନ୍ତୁ u && cudaAllocator && u->currAllocator == cudaAllocator;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା Kernel::set(ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଅଚଳ KernelArg& arg)
{
    ଯଦି( !p || !p->handle )
        ଫେରନ୍ତୁ -1;
    ଯଦି (i < 0)
    {
        CV_LOG_ERROR(ଖାଲି, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i));
        ଫେରନ୍ତୁ i;
    }
    ଯଦି( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    ଯଦି( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        ତାର୍କିକ ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        ଯଦି (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ଖାଲି;
            status = clSetKernelArg(p->handle, (cl_uint)i, ଆକାର(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i).c_str());
            ଫେରନ୍ତୁ i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        ଯଦି (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ଖାଲି, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ଫେରନ୍ତୁ -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        ଯଦି (!h)
        {
            CV_LOG_ERROR(ଖାଲି, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arg.flags, arg.m));
            p->release();
            p = 0;
            ଫେରନ୍ତୁ -1;
        }

#ifdef HAVE_OPENCL_SVM
        ଯଦି ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ଅଚଳ Context& ctx = Context::getDefault();
            ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#ଯଦି 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#ଅନ୍ୟଥା
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ଶୂନ୍ୟ*)svmDataPtr).c_str());
        }
        ଅନ୍ୟଥା
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, ଆକାର(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ଶୂନ୍ୟ*)h).c_str());
        }

        ଯଦି (ptronly)
        {
            i++;
        }
        ଅନ୍ୟଥା ଯଦି( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), ଆକାର(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+1), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), ଆକାର(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+2), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u2d.offset).c_str());
            i += 3;

            ଯଦି( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, ଆକାର(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), ଆକାର(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+1), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cols).c_str());
                i += 2;
            }
        }
        ଅନ୍ୟଥା
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), ଆକାର(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+1), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), ଆକାର(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+2), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), ଆକାର(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+3), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u3d.offset).c_str());
            i += 4;
            ଯଦି( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, ଆକାର(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), ଆକାର(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+1), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), ଆକାର(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i+2), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ଫେରନ୍ତୁ i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)i, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)arg.sz, (ଶୂନ୍ୟ*)arg.obj).c_str());
    ଫେରନ୍ତୁ i+1;
}

ତାର୍କିକ Kernel::run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t _globalsize[], size_t _localsize[],
                 ତାର୍କିକ ସମକାଳ, ଅଚଳ Queue& q)
{
    ଯଦି (!p)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != ଖାଲି);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        ଯଦି (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)val) * val;
    }
    CV_Assert(total > 0);

    ଫେରନ୍ତୁ p->run(dims, globalsize, _localsize, ସମକାଳ, ଖାଲି, q);
}


ତାର୍କିକ Kernel::run_(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t _globalsize[], size_t _localsize[],
                  ତାର୍କିକ ସମକାଳ, ଅଚଳ Queue& q)
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->run(dims, _globalsize, _localsize, ସମକାଳ, ଖାଲି, q);
}


ତାର୍କିକ Kernel::Impl::run(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t globalsize[], size_t localsize[],
        ତାର୍କିକ ସମକାଳ, int64* timeNS, ଅଚଳ Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    ଯଦି (!handle)
    {
        CV_LOG_ERROR(ଖାଲି, "OpenCL kernel has zero handle: " << name);
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଯଦି (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !ସମକାଳ;

    ଯଦି (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#ଯଦି CV_OPENCL_SYNC_RUN_KERNELS
    ସମକାଳ = ସତ୍ୟ;
#endif

    cl_command_queue qq = getQueue(q);
    ଯଦି (haveTempDstUMats)
        ସମକାଳ = ସତ୍ୟ;
    ଯଦି (haveTempSrcUMats)
        ସମକାଳ = ସତ୍ୟ;
    ଯଦି (timeNS)
        ସମକାଳ = ସତ୍ୟ;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           ଖାଲି, globalsize, localsize, 0, 0,
                                           (ସମକାଳ && !timeNS) ? 0 : &asyncEvent);
#ଯଦି !CV_OPENCL_SHOW_RUN_KERNELS
    ଯଦି (retval != CL_SUCCESS)
#endif
    {
        cv::String ସନ୍ଦେଶ = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ସମକାଳ ? "true" : "false"
                        );
        ଯଦି (retval != CL_SUCCESS)
        {
            ସନ୍ଦେଶ = CV_OCL_API_ERROR_ସନ୍ଦେଶ(retval, ସନ୍ଦେଶ.c_str());
        }
#ଯଦି CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, ସନ୍ଦେଶ.c_str());
#ଅନ୍ୟଥା
        printf("%s\n", ସନ୍ଦେଶ.c_str());
        fflush(stdout);
#endif
    }
    ଯଦି (ସମକାଳ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        ଯଦି (timeNS)
        {
            ଯଦି (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, ଆକାର(startTime), &startTime, ଖାଲି));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, ଆକାର(stopTime), &stopTime, ଖାଲି));
                *timeNS = (int64)(stopTime - startTime);
            }
            ଅନ୍ୟଥା
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    ଅନ୍ୟଥା
    {
        addref();
        isInProgress = ସତ୍ୟ;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ଏହି));
    }
    ଯଦି (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ଫେରନ୍ତୁ retval == CL_SUCCESS;
}

ତାର୍କିକ Kernel::runTask(ତାର୍କିକ ସମକାଳ, ଅଚଳ Queue& q)
{
    ଯଦି(!p || !p->handle || p->isInProgress)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, ସମକାଳ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ସମକାଳ ? "true" : "false").c_str());
    ଯଦି (ସମକାଳ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    ଅନ୍ୟଥା
    {
        p->addref();
        p->isInProgress = ସତ୍ୟ;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    ଯଦି (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ଫେରନ୍ତୁ retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, size_t globalsize[], size_t localsize[], ଅଚଳ Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    ତାର୍କିକ res = p->run(dims, globalsize, localsize, ସତ୍ୟ, &timeNs, profilingQueue);
    ଫେରନ୍ତୁ res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ଅଚଳ
{
    ଯଦି(!p || !p->handle)
        ଫେରନ୍ତୁ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, ଆକାର(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ଫେରନ୍ତୁ status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ଅଚଳ
{
    ଯଦି(!p || !p->handle)
        ଫେରନ୍ତୁ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, ଆକାର(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ଫେରନ୍ତୁ status == CL_SUCCESS ? val : 0;
}

ତାର୍କିକ Kernel::compileWorkGroupSize(size_t wsz[]) ଅଚଳ
{
    ଯଦି(!p || !p->handle || !wsz)
        ଫେରନ୍ତୁ 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, ଆକାର(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ଫେରନ୍ତୁ status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ଅଚଳ
{
    ଯଦି(!p || !p->handle)
        ଫେରନ୍ତୁ 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, ଆକାର(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ଫେରନ୍ତୁ status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ସଂରଚନା ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ଗଣନା KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ଅଚଳ String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ଅଚଳ String& module, ଅଚଳ String& name, ଅଚଳ String& codeStr, ଅଚଳ String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    ଶୂନ୍ୟ init(ଗଣନା KIND kind, ଅଚଳ String& module, ଅଚଳ String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = ଖାଲି;
        sourceSize_ = 0;
        isHashUpdated = ମିଥ୍ୟା;
    }

    ଶୂନ୍ୟ initFromSource(ଅଚଳ String& codeStr, ଅଚଳ String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        ଯଦି (sourceHash_.empty())
        {
            updateHash();
        }
        ଅନ୍ୟଥା
        {
            isHashUpdated = ସତ୍ୟ;
        }
    }

    ଶୂନ୍ୟ updateHash(ଅଚଳ ଅକ୍ଷର* hashStr = ଖାଲି)
    {
        ଯଦି (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = ସତ୍ୟ;
            ଫେରନ୍ତୁ;
        }
        uint64 hash = 0;
        ବାଛ (kind_)
        {
        ସ୍ଥିତି PROGRAM_SOURCE_CODE:
            ଯଦି (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            ଅନ୍ୟଥା
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            ଭାଙ୍ଗ;
        ସ୍ଥିତି PROGRAM_BINARIES:
        ସ୍ଥିତି PROGRAM_SPIR:
        ସ୍ଥିତି PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = ସତ୍ୟ;
    }

    Impl(ଗଣନା KIND kind,
            ଅଚଳ String& module, ଅଚଳ String& name,
            ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size,
            ଅଚଳ cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    ସ୍ଥିର ProgramSource fromSourceWithStaticLifetime(ଅଚଳ String& module, ଅଚଳ String& name,
            ଅଚଳ ଅକ୍ଷର* sourceCodeStaticStr, ଅଚଳ ଅକ୍ଷର* hashStaticStr,
            ଅଚଳ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ନୂତନ Impl(PROGRAM_SOURCE_CODE, module, name,
                (ଅଚଳ unsigned ଅକ୍ଷର*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        ଫେରନ୍ତୁ result;
    }

    ସ୍ଥିର ProgramSource from_ଦ୍ୱିମିକ(ଅଚଳ String& module, ଅଚଳ String& name,
            ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size,
            ଅଚଳ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ନୂତନ Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        ଫେରନ୍ତୁ result;
    }

    ସ୍ଥିର ProgramSource fromSPIR(ଅଚଳ String& module, ଅଚଳ String& name,
            ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size,
            ଅଚଳ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = ନୂତନ Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        ଫେରନ୍ତୁ result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ଅଚଳ unsigned ଅକ୍ଷର* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    ତାର୍କିକ isHashUpdated;

    friend ସଂରଚନା Program::Impl;
    friend ସଂରଚନା internal::ProgramEntry;
    friend ସଂରଚନା Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ଅଚଳ String& module, ଅଚଳ String& name, ଅଚଳ String& codeStr, ଅଚଳ String& codeHash)
{
    p = ନୂତନ Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ଅଚଳ ଅକ୍ଷର* prog)
{
    p = ନୂତନ Impl(prog);
}

ProgramSource::ProgramSource(ଅଚଳ String& prog)
{
    p = ନୂତନ Impl(prog);
}

ProgramSource::~ProgramSource()
{
    ଯଦି(p)
        p->release();
}

ProgramSource::ProgramSource(ଅଚଳ ProgramSource& prog)
{
    p = prog.p;
    ଯଦି(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ଅଚଳ ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &prog) {
        ଯଦି(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ଅଚଳ String& ProgramSource::source() ଅଚଳ
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == ଖାଲି); // method returns reference - can't construct temporary object
    ଫେରନ୍ତୁ p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ଅଚଳ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_ଦ୍ୱିମିକ(ଅଚଳ String& module, ଅଚଳ String& name,
        ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size,
        ଅଚଳ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ଫେରନ୍ତୁ Impl::from_ଦ୍ୱିମିକ(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ଅଚଳ String& module, ଅଚଳ String& name,
        ଅଚଳ unsigned ଅକ୍ଷର* binary, ଅଚଳ size_t size,
        ଅଚଳ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ଫେରନ୍ତୁ Impl::from_ଦ୍ୱିମିକ(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ଅଚଳ
{
    ଯଦି (ଏହି->pProgramSource == ଖାଲି)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        ଯଦି (ଏହି->pProgramSource == ଖାଲି)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(ଏହି->module, ଏହି->name, ଏହି->programCode, ଏହି->programHash, cv::String());
            ProgramSource* ptr = ନୂତନ ProgramSource(ps);
            const_cast<ProgramEntry*>(ଏହି)->pProgramSource = ptr;
        }
    }
    ଫେରନ୍ତୁ *ଏହି->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ସ୍ଥିର
cv::String joinBuildOptions(ଅଚଳ cv::String& a, ଅଚଳ cv::String& b)
{
    ଯଦି (b.empty())
        ଫେରନ୍ତୁ a;
    ଯଦି (a.empty())
        ଫେରନ୍ତୁ b;
    ଯଦି (b[0] == ' ')
        ଫେରନ୍ତୁ a + b;
    ଫେରନ୍ତୁ a + (cv::String(" ") + b);
}

ସଂରଚନା Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ଅଚଳ ProgramSource& src,
         ଅଚଳ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ଖାଲି),
         buildflags(_buildflags)
    {
        ଅଚଳ ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ଅଚଳ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        ଯଦି (ctx.ptr() == ଖାଲି || device.ptr() == ଖାଲି)
            ଫେରନ୍ତୁ;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        ଯଦି (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            ଯଦି (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            ଅନ୍ୟଥା ଯଦି (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ଅଚଳ String param_buildExtraOptions = getBuildExtraOptions();
            ଯଦି (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#ଯଦି CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ଖାଲି, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#ଯଦି CV_OPENCL_SHOW_BUILD_KERNELS
        ଯଦି (handle)
        {
            size_t retsz = 0;
            ଅକ୍ଷର kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ଆକାର(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            ଯଦି (result == CL_SUCCESS && retsz < ଆକାର(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(ଖାଲି, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_ERROR(ଖାଲି, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    ତାର୍କିକ compile(ଅଚଳ Context& ctx, ଅଚଳ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        ଯଦି (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(ଖାଲି, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ତାର୍କିକ isLoaded = create_From_ଦ୍ୱିମିକ(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ଫେରନ୍ତୁ isLoaded;
        }
        ଫେରନ୍ତୁ compileWithCache(ctx, src_, errmsg);
    }

    ତାର୍କିକ compileWithCache(ଅଚଳ Context& ctx, ଅଚଳ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ଅଚଳ std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ଅଚଳ String& hash_str = src_->sourceHash_;
        cv::String fname;
        ଯଦି (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ଅଚଳ cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        ଯଦି (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ଚେଷ୍ଟା
            {
                std::vector<ଅକ୍ଷର> ଦ୍ୱିମିକ_Buf;
                ତାର୍କିକ res = ମିଥ୍ୟା;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ଦ୍ୱିମିକ_Program_File file(fname, hash_str.c_str());
                    res = file.ପଠ(buildflags, ଦ୍ୱିମିକ_Buf);
                }
                ଯଦି (res)
                {
                    CV_Assert(!ଦ୍ୱିମିକ_Buf.empty());
                    CV_LOG_VERBOSE(ଖାଲି, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    ତାର୍କିକ isLoaded = create_From_ଦ୍ୱିମିକ(ctx, ଦ୍ୱିମିକ_Buf, errmsg);
                    ଯଦି (isLoaded)
                        ଫେରନ୍ତୁ ସତ୍ୟ;
                }
            }
            ଧର (ଅଚଳ cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(ଖାଲି, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            ଧର (...)
            {
                CV_LOG_VERBOSE(ଖାଲି, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ଖାଲି);
        ଯଦି (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            ଯଦି (!buildFromSources(ctx, src_, errmsg))
            {
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
        ଅନ୍ୟଥା ଯଦି (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            ଯଦି ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ଖାଲି, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            ତାର୍କିକ isLoaded = create_From_ଦ୍ୱିମିକ(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ଯଦି (!isLoaded)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        ଅନ୍ୟଥା ଯଦି (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        ଅନ୍ୟଥା
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != ଖାଲି);
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
        ଯଦି (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ଚେଷ୍ଟା
            {
                std::vector<ଅକ୍ଷର> ଦ୍ୱିମିକ_Buf;
                get_Program_ଦ୍ୱିମିକ(ଦ୍ୱିମିକ_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ଦ୍ୱିମିକ_Program_File file(fname, hash_str.c_str());
                    file.ଲିଖ(buildflags, ଦ୍ୱିମିକ_Buf);
                }
            }
            ଧର (ଅଚଳ cv::Exception& e)
            {
                CV_LOG_WARNING(ଖାଲି, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            ଧର (...)
            {
                CV_LOG_WARNING(ଖାଲି, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#ଯଦି CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS
        ଯଦି (CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS_ମୂଲ୍ୟ)
        {
            std::vector<ଅକ୍ଷର> ଦ୍ୱିମିକ_Buf;
            get_Program_ଦ୍ୱିମିକ(ଦ୍ୱିମିକ_Buf);
            ଯଦି (!ଦ୍ୱିମିକ_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ଖାଲି;
                create_From_ଦ୍ୱିମିକ(ctx, ଦ୍ୱିମିକ_Buf, errmsg);
            }
        }
#endif
        ଫେରନ୍ତୁ handle != ଖାଲି;
    }

    ଶୂନ୍ୟ dumpBuildLog_(cl_int result, ଅଚଳ cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<ଅକ୍ଷର, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        ଯଦି (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            ଯଦି (log_retval == CL_SUCCESS)
            {
                ଯଦି (retsz < buffer.size())
                    buffer[retsz] = 0;
                ଅନ୍ୟଥା
                    buffer[buffer.size() - 1] = 0;
            }
            ଅନ୍ୟଥା
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

    ତାର୍କିକ buildFromSources(ଅଚଳ Context& ctx, ଅଚଳ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == ଖାଲି);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ଖାଲି, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ଅଚଳ ଅକ୍ଷର* srcptr = src_->sourceAddr_ ? ((ଅଚଳ ଅକ୍ଷର*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != ଖାଲି);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        ଯଦି (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            ପାଇଁ (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#ଯଦି !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            ଯଦି (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                ଯଦି (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ଖାଲି;
                }
                ଯଦି (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#ଯଦି CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS
            ଯଦି (handle && CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS_ମୂଲ୍ୟ)
            {
                CV_LOG_INFO(ଖାଲି, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                ଅକ୍ଷର kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ଆକାର(kernels_buffer), &kernels_buffer[0], &retsz);
                ଯଦି (retsz < ଆକାର(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                ଅନ୍ୟଥା
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(ଖାଲି, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        ଫେରନ୍ତୁ handle != ଖାଲି;
    }

    ଶୂନ୍ୟ get_Program_ଦ୍ୱିମିକ(std::vector<ଅକ୍ଷର>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_ଦ୍ୱିମିକ_SIZES, ଆକାର(sz), &sz, ଖାଲି));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, ଆକାର(ptr), &ptr, ଖାଲି));
    }

    ତାର୍କିକ create_From_ଦ୍ୱିମିକ(ଅଚଳ Context& ctx, ଅଚଳ std::vector<ଅକ୍ଷର>& buf, String& errmsg)
    {
        ଫେରନ୍ତୁ create_From_ଦ୍ୱିମିକ(ctx, (ଅଚଳ unsigned ଅକ୍ଷର*)&buf[0], buf.size(), errmsg);
    }

    ତାର୍କିକ create_From_ଦ୍ୱିମିକ(ଅଚଳ Context& ctx, ଅଚଳ unsigned ଅକ୍ଷର* ଦ୍ୱିମିକ_Addr, ଅଚଳ size_t ଦ୍ୱିମିକ_Size, String& errmsg)
    {
        CV_Assert(handle == ଖାଲି);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ଖାଲି, 0, "Load from binary... (" << ଦ୍ୱିମିକ_Size << " bytes)");

        CV_Assert(ଦ୍ୱିମିକ_Size > 0);

        size_t ndevices = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ଅଚଳ uchar*> ଦ୍ୱିମିକ_Ptrs(ndevices);
        AutoBuffer<size_t> ଦ୍ୱିମିକ_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ଅଚଳ uchar** ଦ୍ୱିମିକ_Ptrs_2 = ଦ୍ୱିମିକ_Ptrs.data();
        size_t* ଦ୍ୱିମିକ_Sizes_2 = ଦ୍ୱିମିକ_Sizes.data();
        ପାଇଁ (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ଦ୍ୱିମିକ_Ptrs_2[i] = ଦ୍ୱିମିକ_Addr;
            ଦ୍ୱିମିକ_Sizes_2[i] = ଦ୍ୱିମିକ_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_ଦ୍ୱିମିକ((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           ଦ୍ୱିମିକ_Sizes_2, ଦ୍ୱିମିକ_Ptrs_2, ଖାଲି, &result);
        ଯଦି (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(ଖାଲି, CV_OCL_API_ERROR_ସନ୍ଦେଶ(result, "clCreateProgramWithBinary"));
            ଯଦି (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ଖାଲି;
            }
        }
        ଯଦି (!handle)
        {
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            ଯଦି (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                ଯଦି (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ଖାଲି;
                }
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    ଆକାର(build_status), &build_status, &retsz));
            ଯଦି (result == CL_SUCCESS)
            {
                ଯଦି (build_status == CL_BUILD_SUCCESS)
                {
                    ଫେରନ୍ତୁ ସତ୍ୟ;
                }
                ଅନ୍ୟଥା
                {
                    CV_LOG_WARNING(ଖାଲି, "clGetProgramBuildInfo() returns " << build_status);
                    ଫେରନ୍ତୁ ମିଥ୍ୟା;
                }
            }
            ଅନ୍ୟଥା
            {
                CV_LOG_ERROR(ଖାଲି, CV_OCL_API_ERROR_ସନ୍ଦେଶ(result, "clGetProgramBuildInfo()"));
                ଯଦି (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ଖାଲି;
                }
            }
        }
#ଯଦି CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS
        ଯଦି (handle && CV_OPENCL_VALIDATE_ଦ୍ୱିମିକ_PROGRAMS_ମୂଲ୍ୟ)
        {
            CV_LOG_INFO(ଖାଲି, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            ଅକ୍ଷର kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, ଆକାର(kernels_buffer), &kernels_buffer[0], &retsz);
            ଯଦି (retsz < ଆକାର(kernels_buffer))
                kernels_buffer[retsz] = 0;
            ଅନ୍ୟଥା
                kernels_buffer[0] = 0;
            CV_LOG_INFO(ଖାଲି, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        ଫେରନ୍ତୁ handle != ଖାଲି;
    }

    ~Impl()
    {
        ଯଦି( handle )
        {
#ifdef _WIN32
            ଯଦି (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = ଖାଲି;
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

Program::Program(ଅଚଳ ProgramSource& src,
        ଅଚଳ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ଅଚଳ Program& prog)
{
    p = prog.p;
    ଯଦି(p)
        p->addref();
}

Program& Program::operator = (ଅଚଳ Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    ଯଦି(newp)
        newp->addref();
    ଯଦି(p)
        p->release();
    p = newp;
    ଫେରନ୍ତୁ *ଏହି;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &prog) {
        ଯଦି(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Program::~Program()
{
    ଯଦି(p)
        p->release();
}

ତାର୍କିକ Program::create(ଅଚଳ ProgramSource& src,
            ଅଚଳ String& buildflags, String& errmsg)
{
    ଯଦି(p)
    {
        p->release();
        p = ଖାଲି;
    }
    p = ନୂତନ Impl(src, buildflags, errmsg);
    ଯଦି(!p->handle)
    {
        p->release();
        p = 0;
    }
    ଫେରନ୍ତୁ p != 0;
}

ଶୂନ୍ୟ* Program::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ଅଚଳ ProgramSource& Program::source() ଅଚଳ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ତାର୍କିକ Program::ପଠ(ଅଚଳ String& bin, ଅଚଳ String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

ତାର୍କିକ Program::ଲିଖ(String& bin) ଅଚଳ
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ଅଚଳ
{
    ଯଦି(!p)
        ଫେରନ୍ତୁ String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ଫେରନ୍ତୁ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ଅଚଳ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ଫେରନ୍ତୁ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

ଶୂନ୍ୟ Program::get_ଦ୍ୱିମିକ(std::vector<ଅକ୍ଷର>& binary) ଅଚଳ
{
    CV_Assert(p && "Empty program");
    p->get_Program_ଦ୍ୱିମିକ(binary);
}

Program Context::Impl::getProg(ଅଚଳ ProgramSource& src,
                               ଅଚଳ String& buildflags, String& errmsg)
{
    size_t limit = get_Program_ଗଣନା_Limit();
    ଅଚଳ ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ଚାବି = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(ଚାବି);
        ଯଦି (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ଚାବି);
            ଯଦି (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ଚାବି);
            }
            ଫେରନ୍ତୁ it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            ଯଦି (limit > 0 && sz >= limit)
            {
                ସ୍ଥିର ତାର୍କିକ warningFlag = ମିଥ୍ୟା;
                ଯଦି (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = ସତ୍ୟ;
                }
                ଯେତେବେଳେ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    ଯଦି (c != 0)
                        ଭାଙ୍ଗ;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(ଚାବି, prog));
        cacheList.push_front(ଚାବି);
    }
    ଫେରନ୍ତୁ prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
ଶ୍ରେଣୀ OpenCLBufferPool
{
ସୁରକ୍ଷିତ:
    ~OpenCLBufferPool() { }
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ T allocate(size_t size) = 0;
    ଆଭାସୀ ଶୂନ୍ୟ release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
ଶ୍ରେଣୀ OpenCLBufferPoolBaseImpl : ସର୍ବସାଧାରଣ BufferPoolController, ସର୍ବସାଧାରଣ OpenCLBufferPool<T>
{
ବ୍ୟକ୍ତିଗତ:
    ଧାଡ଼ିଭିତରେ Derived& derived() { ଫେରନ୍ତୁ *static_cast<Derived*>(ଏହି); }
ସୁରକ୍ଷିତ:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    ତାର୍କିକ _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        ପାଇଁ (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            ଯଦି (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                ଫେରନ୍ତୁ ସତ୍ୟ;
            }
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    // synchronized
    ତାର୍କିକ _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ଅଚଳ size_t size)
    {
        ଯଦି (reservedEntries_.empty())
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        ପାଇଁ (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            ଯଦି (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                ଯଦି (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    ଯଦି (diff == 0)
                        ଭାଙ୍ଗ;
                }
            }
        }
        ଯଦି (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            ଫେରନ୍ତୁ ସତ୍ୟ;
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    // synchronized
    ଶୂନ୍ୟ _checkSizeOfReservedEntries()
    {
        ଯେତେବେଳେ (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ଅଚଳ BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    ଧାଡ଼ିଭିତରେ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        ଯଦି (size < 1024*1024)
            ଫେରନ୍ତୁ 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        ଅନ୍ୟଥା ଯଦି (size < 16*1024*1024)
            ଫେରନ୍ତୁ 64*1024;
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ 1024*1024;
    }

ସର୍ବସାଧାରଣ:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    ଆଭାସୀ ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        ଯଦି (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        ଅନ୍ୟଥା
        {
            derived()._allocateBufferEntry(entry, size);
        }
        ଫେରନ୍ତୁ entry.clBuffer_;
    }
    ଆଭାସୀ ଶୂନ୍ୟ release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        ଯଦି (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        ଅନ୍ୟଥା
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    ଆଭାସୀ size_t getReservedSize() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ currentReservedSize; }
    ଆଭାସୀ size_t getMaxReservedSize() ଅଚଳ CV_OVERRIDE { ଫେରନ୍ତୁ maxReservedSize; }
    ଆଭାସୀ ଶୂନ୍ୟ setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        ଯଦି (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            ପାଇଁ (; i != reservedEntries_.end();)
            {
                ଅଚଳ BufferEntry& entry = *i;
                ଯଦି (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    ଜାରି;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    ଆଭାସୀ ଶୂନ୍ୟ freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        ପାଇଁ (; i != reservedEntries_.end(); ++i)
        {
            ଅଚଳ BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

ସଂରଚନା CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)ଖାଲି), capacity_(0) { }
};

ଶ୍ରେଣୀ OpenCLBufferPoolImpl CV_FINAL : ସର୍ବସାଧାରଣ OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
ସର୍ବସାଧାରଣ:
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା CLBufferEntry BufferEntry;
ସୁରକ୍ଷିତ:
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା createFlags_;
ସର୍ବସାଧାରଣ:
    OpenCLBufferPoolImpl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    ଶୂନ୍ୟ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ଖାଲି);
        entry.capacity_ = alignSize(size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)entry.capacity_, (ଶୂନ୍ୟ*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != ଖାଲି);
        ଯଦି(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ଶୂନ୍ୟ _releaseBufferEntry(ଅଚଳ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ଖାଲି);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
ସଂରଚନା CLSVMBufferEntry
{
    ଶୂନ୍ୟ* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(ଖାଲି), capacity_(0) { }
};
ଶ୍ରେଣୀ OpenCLSVMBufferPoolImpl CV_FINAL : ସର୍ବସାଧାରଣ OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, ଶୂନ୍ୟ*>
{
ସର୍ବସାଧାରଣ:
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା CLSVMBufferEntry BufferEntry;
ସର୍ବସାଧାରଣ:
    OpenCLSVMBufferPoolImpl()
    {
    }

    ଶୂନ୍ୟ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ଖାଲି);
        entry.capacity_ = alignSize(size, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ଅଚଳ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ତାର୍କିକ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)entry.capacity_);
        ଶୂନ୍ୟ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ଶୂନ୍ୟ _releaseBufferEntry(ଅଚଳ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ଖାଲି);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <ତାର୍କିକ readAccess, ତାର୍କିକ writeAccess>
ଶ୍ରେଣୀ AlignedDataPtr
{
ସୁରକ୍ଷିତ:
    ଅଚଳ size_t size_;
    uchar* ଅଚଳ originPtr_;
    ଅଚଳ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

ସର୍ବସାଧାରଣ:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ଖାଲି)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        ଯଦି (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ନୂତନ uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ଯଦି (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ଅଚଳ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ଫେରନ୍ତୁ ptr_;
    }

    ~AlignedDataPtr()
    {
        ଯଦି (allocatedPtr_)
        {
            ଯଦି (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ଖାଲି;
        }
        ptr_ = ଖାଲି;
    }
ବ୍ୟକ୍ତିଗତ:
    AlignedDataPtr(ଅଚଳ AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ଅଚଳ AlignedDataPtr&); // disabled
};

template <ତାର୍କିକ readAccess, ତାର୍କିକ writeAccess>
ଶ୍ରେଣୀ AlignedDataPtr2D
{
ସୁରକ୍ଷିତ:
    ଅଚଳ size_t size_;
    uchar* ଅଚଳ originPtr_;
    ଅଚଳ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

ସର୍ବସାଧାରଣ:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ଖାଲି), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ଖାଲି);
        ଯଦି (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = ନୂତନ uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ଯଦି (readAccess)
            {
                ପାଇଁ (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ଅଚଳ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ଫେରନ୍ତୁ ptr_;
    }

    ~AlignedDataPtr2D()
    {
        ଯଦି (allocatedPtr_)
        {
            ଯଦି (writeAccess)
            {
                ପାଇଁ (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ଖାଲି;
        }
        ptr_ = ଖାଲି;
    }
ବ୍ୟକ୍ତିଗତ:
    AlignedDataPtr2D(ଅଚଳ AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ଅଚଳ AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


ଶୂନ୍ୟ Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(ଖାଲି, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

ଶ୍ରେଣୀ OpenCLAllocator CV_FINAL : ସର୍ବସାଧାରଣ MatAllocator
{
ସର୍ବସାଧାରଣ:
    ଗଣନା AllocatorFlags
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

    UMatData* defaultAllocate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, ଶୂନ୍ୟ* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ଅଚଳ
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        ଫେରନ୍ତୁ u;
    }

    ସ୍ଥିର ତାର୍କିକ isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ସ୍ଥିର ତାର୍କିକ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", ମିଥ୍ୟା);
        ଫେରନ୍ତୁ value;
    }
    ସ୍ଥିର ତାର୍କିକ isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ସ୍ଥିର ତାର୍କିକ value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", ମିଥ୍ୟା);
        ଫେରନ୍ତୁ value;
    }

    ଶୂନ୍ୟ getBestFlags(ଅଚଳ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& createFlags, UMatData::MemoryFlag& flags0) ଅଚଳ
    {
        ଅଚଳ Device& dev = ctx.device(0);
        createFlags = 0;
        ଯଦି ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        ଯଦି (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        ଅନ୍ୟଥା
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* sizes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type,
                       ଶୂନ୍ୟ* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!useOpenCL())
            ଫେରନ୍ତୁ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims-1; i >= 0; i-- )
        {
            ଯଦି( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        ଯଦି (!ctx.getImpl())
            ଫେରନ୍ତୁ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        ଶୂନ୍ୟ* handle = ଖାଲି;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ଅଚଳ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ଯଦି (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            ତାର୍କିକ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        ଅନ୍ୟଥା
#endif
        ଯଦି (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        ଅନ୍ୟଥା ଯଦି (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        ଅନ୍ୟଥା
        {
            CV_Assert(handle != ଖାଲି); // Unsupported, throw
        }

        ଯଦି (!handle)
            ଫେରନ୍ତୁ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = ନୂତନ UMatData(ଏହି);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<ଶୂନ୍ୟ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(ସତ୍ୟ);
        opencl_allocator_stats.onAllocate(u->size);
        ଫେରନ୍ତୁ u;
    }

    ତାର୍କିକ allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        ଯଦି(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            ତାର୍କିକ copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            ଶୂନ୍ୟ* handle = ଖାଲି;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            ତାର୍କିକ useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            ଯଦି (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, handle);
            }
            ଅନ୍ୟଥା ଯଦି (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                ଯଦି (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    ତାର୍କିକ isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = ଖାଲି;
                    ଯଦି (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ଖାଲି, ଖାଲି);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    ଯଦି (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, ଖାଲି, ଖାଲି);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            ଅନ୍ୟଥା
#endif
            {
                ଯଦି( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                ଯଦି (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, u->origdata, (ଶୂନ୍ୟ*)handle).c_str());
                }
                ଯଦି((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, u->origdata, (ଶୂନ୍ୟ*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (ଶୂନ୍ୟ*)handle).c_str());
            ଯଦି(!handle || retval != CL_SUCCESS)
                ଫେରନ୍ତୁ ମିଥ୍ୟା;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ଏହି;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        ଯଦି (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(ସତ୍ୟ);
        opencl_allocator_stats.onAllocate(u->size);
        ଫେରନ୍ତୁ ସତ୍ୟ;
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

    ଶୂନ୍ୟ deallocate(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        ଯଦି (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        ଅନ୍ୟଥା
            deallocate_(u);
    }

    ଶୂନ୍ୟ deallocate_(UMatData* u) ଅଚଳ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        ଯଦି ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        ଯଦି (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ଫେରନ୍ତୁ;             // avoid any OpenCL calls
#endif
        ଯଦି(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            ଯଦି (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    ଯଦି( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        ତାର୍କିକ isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = ଖାଲି;
                        ଯଦି (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ଖାଲି, ଖାଲି);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        ଯଦି (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ଖାଲି, ଖାଲି);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    ଅନ୍ୟଥା
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                ଅନ୍ୟଥା
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    ଯଦି( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<ମିଥ୍ୟା, ସତ୍ୟ> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    ଅନ୍ୟଥା
                    {
                        cl_int retval = 0;
                        ଯଦି (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            ଶୂନ୍ୟ* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ଶୂନ୍ୟ*)u->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            ଯଦି (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ଶୂନ୍ୟ*)u->handle, data, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(ମିଥ୍ୟା);
            }
            ଅନ୍ୟଥା
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                ଯଦି( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            ଅନ୍ୟଥା
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (ଶୂନ୍ୟ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ସତ୍ୟ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ଖାଲି;
            ଯଦି(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = ଖାଲି;
        }
        ଅନ୍ୟଥା
        {
            CV_Assert(u->origdata == ଖାଲି);
            ଯଦି(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(ସତ୍ୟ);
            }
            ଯଦି (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            ଅନ୍ୟଥା ଯଦି (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            ଅନ୍ୟଥା ଯଦି (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                ଅନ୍ୟଥା ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ଖାଲି, ଖାଲି);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((ଶୂନ୍ୟ*)u->handle);
            }
#endif
            ଅନ୍ୟଥା
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ସତ୍ୟ);
            delete u;
            u = ଖାଲି;
        }
        CV_Assert(u == ଖାଲି);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    ଶୂନ୍ୟ map(UMatData* u, AccessFlag accessFlags) ଅଚଳ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        ଯଦି (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ସତ୍ୟ);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            ଯଦି( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ଖାଲି, ଖାଲି);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(ମିଥ୍ୟା);
                    u->markDeviceMemMapped(ସତ୍ୟ);
                    ଫେରନ୍ତୁ;
                }
#endif

                cl_int retval = CL_SUCCESS;
                ଯଦି (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ଶୂନ୍ୟ*)u->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, u->data).c_str());
                }
                ଯଦି (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(ମିଥ୍ୟା);
                    u->markDeviceMemMapped(ସତ୍ୟ);
                    ଫେରନ୍ତୁ;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            ଯଦି(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(ସତ୍ୟ);
            }
        }

        ଯଦି (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<ମିଥ୍ୟା, ସତ୍ୟ> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ଶୂନ୍ୟ*)u->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(ମିଥ୍ୟା);
        }
    }

    ଶୂନ୍ୟ unmap(UMatData* u) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        ଯଦି( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != ଖାଲି);
#ifdef HAVE_OPENCL_SVM
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, ଖାଲି, ଖାଲି);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                ଯଦି (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(ମିଥ୍ୟା);
                u->markHostCopyObsolete(ସତ୍ୟ);
                ଫେରନ୍ତୁ;
            }
#endif
            ଯଦି (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ଶୂନ୍ୟ*)u->handle, u->data, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size).c_str());
                ଯଦି (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(ମିଥ୍ୟା);
                u->data = 0;
                u->markDeviceCopyObsolete(ମିଥ୍ୟା);
                u->markHostCopyObsolete(ସତ୍ୟ);
            }
        }
        ଅନ୍ୟଥା ଯଦି( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<ସତ୍ୟ, ମିଥ୍ୟା> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ଶୂନ୍ୟ*)u->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(ମିଥ୍ୟା);
            u->markHostCopyObsolete(ସତ୍ୟ);
        }
    }

    ତାର୍କିକ checkContinuous(ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                         ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
                         ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ଅଚଳ
    {
        ତାର୍କିକ iscontinuous = ସତ୍ୟ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = dims-2; i >= 0; i-- )
        {
            ଯଦି( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = ମିଥ୍ୟା;
            total *= sz[i];
            ଯଦି( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            ଯଦି( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        ଯଦି( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            ଯଦି( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                ଯଦି( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                ଯଦି( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            ଅନ୍ୟଥା
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                ଯଦି( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                ଯଦି( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ଫେରନ୍ତୁ iscontinuous;
    }

    ଶୂନ୍ୟ download(UMatData* u, ଶୂନ୍ୟ* dstptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                  ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
                  ଅଚଳ size_t dststep[]) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ;
        UMatDataAutoLock autolock(u);

        ଯଦି( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ଫେରନ୍ତୁ;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ତାର୍କିକ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ଖାଲି || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ଖାଲି, ଖାଲି);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            ଯଦି( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            ଅନ୍ୟଥା
            {
                // This code is from MatAllocator::download()
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    ଯଦି( sz[i] == 0 )
                    ଫେରନ୍ତୁ;
                    ଯଦି( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ଅଚଳ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ପାଇଁ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ଖାଲି, ଖାଲି);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        ଅନ୍ୟଥା
#endif
        {
            ଯଦି( iscontinuous )
            {
                AlignedDataPtr<ମିଥ୍ୟା, ସତ୍ୟ> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            ଅନ୍ୟଥା ଯଦି (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ଅଚଳ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<ମିଥ୍ୟା, ମିଥ୍ୟା> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                ପାଇଁ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            ଅନ୍ୟଥା
            {
                AlignedDataPtr2D<ମିଥ୍ୟା, ସତ୍ୟ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    ଶୂନ୍ୟ upload(UMatData* u, ଅଚଳ ଶୂନ୍ୟ* srcptr, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
                ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[],
                ଅଚଳ size_t srcstep[]) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!u)
            ଫେରନ୍ତୁ;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ତାର୍କିକ iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        ଯଦି( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(ମିଥ୍ୟା);
            u->markDeviceCopyObsolete(ସତ୍ୟ);
            ଫେରନ୍ତୁ;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ଖାଲି || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ଖାଲି, ଖାଲି);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            ଯଦି( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            ଅନ୍ୟଥା
            {
                // This code is from MatAllocator::upload()
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    ଯଦି( sz[i] == 0 )
                    ଫେରନ୍ତୁ;
                    ଯଦି( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
                }

                Mat src(dims, isz, CV_8U, (ଶୂନ୍ୟ*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ଅଚଳ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ପାଇଁ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ଖାଲି, ଖାଲି);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        ଅନ୍ୟଥା
#endif
        {
            ଯଦି( iscontinuous )
            {
                AlignedDataPtr<ସତ୍ୟ, ମିଥ୍ୟା> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (ଶୂନ୍ୟ*)u->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstrawofs, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            ଅନ୍ୟଥା ଯଦି (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ଅଚଳ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<ମିଥ୍ୟା, ମିଥ୍ୟା> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                ପାଇଁ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            ଅନ୍ୟଥା
            {
                AlignedDataPtr2D<ସତ୍ୟ, ମିଥ୍ୟା> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(ସତ୍ୟ);
#ifdef HAVE_OPENCL_SVM
        ଯଦି ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ଅନ୍ୟଥା
#endif
        {
            u->markHostCopyObsolete(ସତ୍ୟ);
        }
        u->markDeviceCopyObsolete(ମିଥ୍ୟା);
    }

    ଶୂନ୍ୟ copy(UMatData* src, UMatData* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ size_t sz[],
              ଅଚଳ size_t srcofs[], ଅଚଳ size_t srcstep[],
              ଅଚଳ size_t dstofs[], ଅଚଳ size_t dststep[], ତାର୍କିକ _sync) ଅଚଳ CV_OVERRIDE
    {
        ଯଦି(!src || !dst)
            ଫେରନ୍ତୁ;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        ତାର୍କିକ iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        ଯଦି( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ଫେରନ୍ତୁ;
        }
        ଯଦି( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(ମିଥ୍ୟା);
#ifdef HAVE_OPENCL_SVM
            ଯଦି ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            ଅନ୍ୟଥା
#endif
            {
                dst->markDeviceCopyObsolete(ସତ୍ୟ);
            }
            ଫେରନ୍ତୁ;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        ଯଦି ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ଯଦି ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ଅଚଳ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                ଯଦି( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, ଖାଲି, ଖାଲି);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                ଅନ୍ୟଥା
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        ଯଦି( sz[i] == 0 )
                        ଫେରନ୍ତୁ;
                        ଯଦି( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    ପାଇଁ( ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < dims; i++ )
                    {
                        ଯଦି( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ଅଚଳ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    ପାଇଁ( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            ଅନ୍ୟଥା
            {
                ଯଦି ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                ଅନ୍ୟଥା
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        ଅନ୍ୟଥା
#endif
        {
            ଯଦି( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (ଶୂନ୍ୟ*)src->handle, (ଶୂନ୍ୟ*)dst->handle, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)srcrawofs, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)dstrawofs, (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)total).c_str());
            }
            ଅନ୍ୟଥା ଯଦି (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ଅଚଳ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<ମିଥ୍ୟା, ମିଥ୍ୟା> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<ମିଥ୍ୟା, ମିଥ୍ୟା> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                ପାଇଁ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            ଅନ୍ୟଥା
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        ଯଦି (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        ଯଦି ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ଅନ୍ୟଥା
#endif
        {
            dst->markHostCopyObsolete(ସତ୍ୟ);
        }
        dst->markDeviceCopyObsolete(ମିଥ୍ୟା);

        ଯଦି( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ଅଚଳ ଅକ୍ଷର* id) ଅଚଳ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        ଯଦି (ctx.empty())
            ଫେରନ୍ତୁ ଖାଲି;
#ifdef HAVE_OPENCL_SVM
        ଯଦି ((svm::checkForceSVMUmatUsage() && (id == ଖାଲି || strcmp(id, "OCL") == 0)) || (id != ଖାଲି && strcmp(id, "SVM") == 0))
        {
            ଫେରନ୍ତୁ &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        ଯଦି (id != ଖାଲି && strcmp(id, "HOST_ALLOC") == 0)
        {
            ଫେରନ୍ତୁ &ctx.getImpl()->getBufferPoolHostPtr();
        }
        ଯଦି (id != ଖାଲି && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ଫେରନ୍ତୁ &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    ଶୂନ୍ୟ flushCleanupQueue() ଅଚଳ
    {
        // TSAN false positive: see #27638
        ଯଦି (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            ପାଇଁ (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    ଶୂନ୍ୟ addToCleanupQueue(UMatData* u) ଅଚଳ
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

ସ୍ଥିର OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ସ୍ଥିର OpenCLAllocator* g_allocator = ନୂତନ OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ଫେରନ୍ତୁ g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


ନାମାକାଶ cv {

// three funcs below are implemented in umatrix.cpp
ଶୂନ୍ୟ setSize( UMat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ଅଚଳ size_t* _steps,
              ତାର୍କିକ autoSteps = ମିଥ୍ୟା );
ଶୂନ୍ୟ finalizeHdr(UMat& m);

} // namespace cv


ନାମାକାଶ cv { ନାମାକାଶ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
ଶୂନ୍ୟ convertFromBuffer(ଶୂନ୍ୟ* cl_mem_buffer, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା type, UMat& dst)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା d = 2;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, ସତ୍ୟ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, ଆକାର(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, ଆକାର(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = ନୂତନ UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    ଫେରନ୍ତୁ;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
ଶୂନ୍ୟ convertFromImage(ଶୂନ୍ୟ* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, ଆକାର(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, ଆକାର(cl_image_format), &fmt, 0));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = CV_8U;
    ବାଛ (fmt.image_channel_data_type)
    {
    ସ୍ଥିତି CL_UNORM_INT8:
    ସ୍ଥିତି CL_UNSIGNED_INT8:
        depth = CV_8U;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_SNORM_INT8:
    ସ୍ଥିତି CL_SIGNED_INT8:
        depth = CV_8S;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_UNORM_INT16:
    ସ୍ଥିତି CL_UNSIGNED_INT16:
        depth = CV_16U;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_SNORM_INT16:
    ସ୍ଥିତି CL_SIGNED_INT16:
        depth = CV_16S;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_SIGNED_INT32:
        depth = CV_32S;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_FLOAT:
        depth = CV_32F;
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_HALF_FLOAT:
        depth = CV_16F;
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = CV_8UC1;
    ବାଛ (fmt.image_channel_order)
    {
    ସ୍ଥିତି CL_R:
    ସ୍ଥିତି CL_A:
    ସ୍ଥିତି CL_INTENSITY:
    ସ୍ଥିତି CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        ଭାଙ୍ଗ;

    ସ୍ଥିତି CL_RG:
    ସ୍ଥିତି CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        ଭାଙ୍ଗ;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    ସ୍ଥିତି CL_RGBA:
    ସ୍ଥିତି CL_BGRA:
    ସ୍ଥିତି CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ଭାଙ୍ଗ;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, ଆକାର(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, ଆକାର(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, ଆକାର(size_t), &h, 0));

    dst.create((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)h, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ଖାଲି, ଖାଲି));

    CV_OCL_CHECK(clFinish(q));

    ଫେରନ୍ତୁ;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ସ୍ଥିର ଶୂନ୍ୟ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint ଅଙ୍କ_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ଖାଲି, &ଅଙ୍କ_Devices);
    ଯଦି (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &ଅଙ୍କ_Devices).c_str());
    }

    ଯଦି (ଅଙ୍କ_Devices == 0)
    {
        devices.clear();
        ଫେରନ୍ତୁ;
    }

    devices.resize((size_t)ଅଙ୍କ_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, ଅଙ୍କ_Devices, &devices[0], &ଅଙ୍କ_Devices));
}

ସଂରଚନା PlatformInfo::Impl
{
    Impl(ଶୂନ୍ୟ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ଅଚଳ
    {
        ଅକ୍ଷର buf[1024];
        size_t sz=0;
        ଫେରନ୍ତୁ clGetPlatformInfo(handle, prop, ଆକାର(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < ଆକାର(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା versionMajor_;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(ଶୂନ୍ୟ* platform_id)
{
    p = ନୂତନ Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    ଯଦି(p)
        p->release();
}

PlatformInfo::PlatformInfo(ଅଚଳ PlatformInfo& i)
{
    ଯଦି (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ଅଚଳ PlatformInfo& i)
{
    ଯଦି (i.p != p)
    {
        ଯଦି (i.p)
            i.p->addref();
        ଯଦି (p)
            p->release();
        p = i.p;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &i) {
        ଯଦି(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା PlatformInfo::device_ସଂଖ୍ୟା() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p->devices.size() : 0;
}

ଶୂନ୍ୟ PlatformInfo::getDevice(Device& device, ପୂର୍ଣ୍ଣସଂଖ୍ୟା d) ଅଚଳ
{
    CV_Assert(p && d < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)p->devices.size() );
    ଯଦି(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->version_ : String();
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା PlatformInfo::versionMajor() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->versionMajor_;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା PlatformInfo::versionMinor() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->versionMinor_;
}

ସ୍ଥିର ଶୂନ୍ୟ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint ଅଙ୍କ_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ଖାଲି, &ଅଙ୍କ_Platforms));

    ଯଦି (ଅଙ୍କ_Platforms == 0)
    {
        platforms.clear();
        ଫେରନ୍ତୁ;
    }

    platforms.resize((size_t)ଅଙ୍କ_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(ଅଙ୍କ_Platforms, &platforms[0], &ଅଙ୍କ_Platforms));
}

ଶୂନ୍ୟ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    ପାଇଁ (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((ଶୂନ୍ୟ*)&platforms[i]) );
}

ଅଚଳ ଅକ୍ଷର* typeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* tab[CV_DEPTH_MAX*16]=
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ଅଚଳ ଅକ୍ଷର* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ଫେରନ୍ତୁ result;
}

ଅଚଳ ଅକ୍ଷର* memopTypeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* tab[CV_DEPTH_MAX*16] =
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ଅଚଳ ଅକ୍ଷର* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ଫେରନ୍ତୁ result;
}

ଅଚଳ ଅକ୍ଷର* vecopTypeToStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା type)
{
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* tab[CV_DEPTH_MAX*16] =
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
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ଅଚଳ ଅକ୍ଷର* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    ଫେରନ୍ତୁ result;
}

// Deprecated due to size of buf buffer being unknowable.
ଅଚଳ ଅକ୍ଷର* convertTypeStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅକ୍ଷର* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ଫେରନ୍ତୁ convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ଅଚଳ ଅକ୍ଷର* convertTypeStr(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ଅକ୍ଷର* buf, size_t buf_size)
{
    ଯଦି( sdepth == ddepth )
        ଫେରନ୍ତୁ "noconvert";
    ଅଚଳ ଅକ୍ଷର *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    ଯଦି( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    ଅନ୍ୟଥା ଯଦି( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    ଅନ୍ୟଥା
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    ଫେରନ୍ତୁ buf;
}

ଅଚଳ ଅକ୍ଷର* getOpenCLErrorString(ପୂର୍ଣ୍ଣସଂଖ୍ୟା errorCode)
{
#define CV_OCL_CODE(id) ସ୍ଥିତି id: ଫେରନ୍ତୁ #id
#define CV_OCL_CODE_(id, name) ସ୍ଥିତି id: ଫେରନ୍ତୁ #name
    ବାଛ (errorCode)
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
    CV_OCL_CODE(CL_INVALID_ମୂଲ୍ୟ);
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
    CV_OCL_CODE(CL_INVALID_ଦ୍ୱିମିକ);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_ମୂଲ୍ୟ);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_ଗଣନା);
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
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ: ଫେରନ୍ତୁ "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
ସ୍ଥିର std::string kerToStr(ଅଚଳ Mat & k)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = k.cols - 1, depth = k.depth();
    ଅଚଳ T * ଅଚଳ data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    ଯଦି (depth <= CV_8S)
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < width; ++i)
            stream << "DIG(" << (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)data[i] << ")";
        stream << "DIG(" << (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)data[width] << ")";
    }
    ଅନ୍ୟଥା ଯଦି (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    ଅନ୍ୟଥା ଯଦି (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < width; ++i)
            stream << "DIG(" << (ଦଶମିକ)data[i] << "h)";
        stream << "DIG(" << (ଦଶମିକ)data[width] << "h)";
    }
    ଅନ୍ୟଥା
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    ଫେରନ୍ତୁ stream.str();
}

String kernelToStr(ନିବେଶ_Array _kernel, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth, ଅଚଳ ଅକ୍ଷର * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = kernel.depth();
    ଯଦି (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    ଯଦି (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    ଯଦି (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    ପ୍ରକାରବ୍ୟାଖ୍ୟା std::string (* func_t)(ଅଚଳ Mat &);
    ସ୍ଥିର ଅଚଳ func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<ଅକ୍ଷର>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>, kerToStr<ଦଶମିକ>, kerToStr<ଦ୍ୱିଗୁଣ>, kerToStr<hfloat>,
                                                0, kerToStr<ତାର୍କିକ>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ଅଚଳ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ଫେରନ୍ତୁ cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    କର \
    { \
        ଯଦି (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            ଯଦି (cwidth < ckercn || ckercn <= 0) \
                ଫେରନ୍ତୁ 1; \
            cols.push_back(cwidth); \
            ଯଦି (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                ଫେରନ୍ତୁ 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    ଯେତେବେଳେ ((ଶୂନ୍ୟ)0, 0)

ପୂର୍ଣ୍ଣସଂଖ୍ୟା predictOptimalVectorWidth(ନିବେଶ_Array src1, ନିବେଶ_Array src2, ନିବେଶ_Array src3,
                              ନିବେଶ_Array src4, ନିବେଶ_Array src5, ନିବେଶ_Array src6,
                              ନିବେଶ_Array src7, ନିବେଶ_Array src8, ନିବେଶ_Array src9,
                              OclVectorStrategy strat)
{
    ଅଚଳ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା vectorWidths[CV_DEPTH_MAX] = {
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
    ଯଦି (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    ଫେରନ୍ତୁ checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା checkOptimalVectorWidth([[maybe_unused]] ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା *vectorWidths,
                            [[maybe_unused]] ନିବେଶ_Array src1,
                            [[maybe_unused]] ନିବେଶ_Array src2,
                            [[maybe_unused]] ନିବେଶ_Array src3,
                            [[maybe_unused]] ନିବେଶ_Array src4,
                            [[maybe_unused]] ନିବେଶ_Array src5,
                            [[maybe_unused]] ନିବେଶ_Array src6,
                            [[maybe_unused]] ନିବେଶ_Array src7,
                            [[maybe_unused]] ନିବେଶ_Array src8,
                            [[maybe_unused]] ନିବେଶ_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    ଫେରନ୍ତୁ 1;
#ଅନ୍ୟଥା
    CV_Assert(vectorWidths);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> dividers, kercns;
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

    ପାଇଁ (size_t i = 0; i < size; ++i)
        ଯେତେବେଳେ (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା kercn = *std::min_element(kercns.begin(), kercns.end());

    ଫେରନ୍ତୁ kercn;
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା predictOptimalVectorWidthMax(ନିବେଶ_Array src1, ନିବେଶ_Array src2, ନିବେଶ_Array src3,
                                 ନିବେଶ_Array src4, ନିବେଶ_Array src5, ନିବେଶ_Array src6,
                                 ନିବେଶ_Array src7, ନିବେଶ_Array src8, ନିବେଶ_Array src9)
{
    ଫେରନ୍ତୁ predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
ଶୂନ୍ୟ buildOptionsAddMatrixDescription(String& buildOptions, ଅଚଳ String& name, ନିବେଶ_Array _m)
{
    ଯଦି (!buildOptions.empty())
        buildOptions += " ";
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_MAT_CN(type),
            name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_ELEM_SIZE(type),
            name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_ELEM_SIZE1(type),
            name.c_str(), (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)depth
            );
}


ସଂରଚନା Image2D::Impl
{
    Impl(ଅଚଳ UMat &src, ତାର୍କିକ norm, ତାର୍କିକ alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        ଯଦି (handle)
            clReleaseMemObject(handle);
    }

    ସ୍ଥିର cl_image_format getImageFormat(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ତାର୍କିକ norm)
    {
        cl_image_format format;
        ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ସ୍ଥିର ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        ଫେରନ୍ତୁ format;
    }

    ସ୍ଥିର ତାର୍କିକ isFormatSupported(cl_image_format format)
    {
        ଯଦି (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        ଯଦି (!context)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;

        // Figure out how many formats are supported by this context.
        cl_uint ଅଙ୍କ_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, ଅଙ୍କ_Formats,
                                                ଖାଲି, &ଅଙ୍କ_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        ଯଦି (ଅଙ୍କ_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(ଅଙ୍କ_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, ଅଙ୍କ_Formats,
                                             formats.data(), ଖାଲି);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            ପାଇଁ (cl_uint i = 0; i < ଅଙ୍କ_Formats; ++i)
            {
                ଯଦି (!memcmp(&formats[i], &format, ଆକାର(format)))
                {
                    ଫେରନ୍ତୁ ସତ୍ୟ;
                }
            }
        }
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }

    ଶୂନ୍ୟ init(ଅଚଳ UMat &src, ତାର୍କିକ norm, ତାର୍କିକ alias)
    {
        ଯଦି (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        ଯଦି (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        ଯଦି (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ଅଚଳ Device & d = ocl::Device::getDefault();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        ଯଦି (1 < major || (1 == major && 2 <= minor))
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
            desc.ଅଙ୍କ_mip_levels   = 0;
            desc.ଅଙ୍କ_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ଖାଲି, &err);
        }
        ଅନ୍ୟଥା
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, ଖାଲି, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        ଯଦି (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), ଖାଲି, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(src.cols * src.rows * src.elemSize()), (ଶୂନ୍ୟ*)devData
                ).c_str());

            ଅଚଳ size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, ଖାଲି, ଖାଲି));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        ଅନ୍ୟଥା
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != ଖାଲି);

        ଯଦି (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, ଖାଲି, 0));
            ଯଦି (!src.isContinuous())
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
    p = ଖାଲି;
}

Image2D::Image2D(ଅଚଳ UMat &src, ତାର୍କିକ norm, ତାର୍କିକ alias)
{
    p = ନୂତନ Impl(src, norm, alias);
}

ତାର୍କିକ Image2D::canCreateAlias(ଅଚଳ UMat &m)
{
    ତାର୍କିକ ret = ମିଥ୍ୟା;
    ଅଚଳ Device & d = ocl::Device::getDefault();
    ଯଦି (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        ଯଦି (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            ଯଦି (!m.u->tempUMat())
            {
                ret = ସତ୍ୟ;
            }
        }
    }
    ଫେରନ୍ତୁ ret;
}

ତାର୍କିକ Image2D::isFormatSupported(ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ତାର୍କିକ norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    ଫେରନ୍ତୁ Impl::isFormatSupported(format);
}

Image2D::Image2D(ଅଚଳ Image2D & i)
{
    p = i.p;
    ଯଦି (p)
        p->addref();
}

Image2D & Image2D::operator = (ଅଚଳ Image2D & i)
{
    ଯଦି (i.p != p)
    {
        ଯଦି (i.p)
            i.p->addref();
        ଯଦି (p)
            p->release();
        p = i.p;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    ଯଦି (ଏହି != &i) {
        ଯଦି (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ଫେରନ୍ତୁ *ଏହି;
}

Image2D::~Image2D()
{
    ଯଦି (p)
        p->release();
}

ଶୂନ୍ୟ* Image2D::ptr() ଅଚଳ
{
    ଫେରନ୍ତୁ p ? p->handle : 0;
}

ତାର୍କିକ internal::isOpenCLForced()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ value = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ value;
}

ତାର୍କିକ internal::isPerformanceCheckBypassed()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର ତାର୍କିକ value = ମିଥ୍ୟା;
    ଯଦି (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", ମିଥ୍ୟା);
        initialized = ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ value;
}

ତାର୍କିକ internal::isCLBuffer(UMat& u)
{
    ଯଦି (isCudaBacked(u.u))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଶୂନ୍ୟ* h = u.handle(ACCESS_RW);
    ଯଦି (!h)
        ଫେରନ୍ତୁ ସତ୍ୟ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#ଯଦି 1
    ଯଦି ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, ଆକାର(type), &type, ଖାଲି);
    ଯଦି (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
    ଫେରନ୍ତୁ ସତ୍ୟ;
}

ସଂରଚନା Timer::Impl
{
    ଅଚଳ Queue queue;

    Impl(ଅଚଳ Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    ଶୂନ୍ୟ start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    ଶୂନ୍ୟ stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ଅଚଳ
    {
        ଫେରନ୍ତୁ (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ଅଚଳ Queue& q) : p(ନୂତନ Impl(q)) { }
Timer::~Timer() { delete p; }

ଶୂନ୍ୟ Timer::start()
{
    CV_Assert(p);
    p->start();
}

ଶୂନ୍ୟ Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ଅଚଳ
{
    CV_Assert(p);
    ଫେରନ୍ତୁ p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
