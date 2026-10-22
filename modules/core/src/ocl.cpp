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

#അന്തര്ഭാവ "precomp.hpp"

#ifndef HAVE_OPENCL
#അന്തര്ഭാവ "ocl_disabled.impl.hpp"
#അല്ലെങ്കിൽ // HAVE_OPENCL

#അന്തര്ഭാവ <list>
#അന്തര്ഭാവ <map>
#അന്തര്ഭാവ <deque>
#അന്തര്ഭാവ <set>
#അന്തര്ഭാവ <string>
#അന്തര്ഭാവ <sstream>
#അന്തര്ഭാവ <fstream>
#എങ്കിൽ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#അന്തര്ഭാവ <inttypes.h>
#endif

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#അന്തര്ഭാവ "opencv2/core/ocl_genbase.hpp"
#അന്തര്ഭാവ "opencl_kernels_core.hpp"

#അന്തര്ഭാവ "opencv2/core/utils/lock.private.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.hpp"
#അന്തര്ഭാവ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ശൂന്യ, 0, "OpenCL allocator: " << __VA_ARGS__)
#അന്തര്ഭാവ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#അന്തര്ഭാവ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# എങ്കിൽ 0
#   define LOG_BUFFER_POOL printf
# അല്ലെങ്കിൽ
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#എങ്കിൽ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#അല്ലെങ്കിൽ
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#എങ്കിൽ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#അല്ലെങ്കിൽ
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#അന്തര്ഭാവ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#അന്തര്ഭാവ "umatrix.hpp"

നാമാകാശ cv { നാമാകാശ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    ശൂന്യം addref() { CV_XADD(&refcount, 1); } \
    ശൂന്യം release() { എങ്കിൽ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete ഇത്; } \
    പൂർണ്ണസംഖ്യ refcount

സ്ഥിതം cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    തിരികെ opencl_allocator_stats;
}

#ifndef _DEBUG
സ്ഥിതം തർക്കം isRaiseError()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം value = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ value;
}
#endif

സ്ഥിതം ശൂന്യം onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    തർക്കം value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", അസത്യം);
    എങ്കിൽ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#എങ്കിൽ CV_OPENCL_TRACE_CHECK
സ്ഥിതം വരിയിൽ
ശൂന്യം traceOpenCLCheck(cl_int status, സ്ഥിരം അക്ഷരം* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#അല്ലെങ്കിൽ
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_സന്ദേശം(check_result, സന്ദേശം) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, സന്ദേശം)

#define CV_OCL_CHECK_RESULT(check_result, സന്ദേശം) \
    ചെയ്യുക { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, സന്ദേശം); \
        എങ്കിൽ (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(സന്ദേശം), സ്ഥിരം അക്ഷരം*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_സന്ദേശം = CV_OCL_API_ERROR_സന്ദേശം(check_result, സന്ദേശം); \
            CV_Error(Error::OpenCLApiCallError, error_സന്ദേശം); \
        } \
    } ആകുമ്പോൾ (0)

#define CV_OCL_CHECK_(വ്യഞ്ജകം, check_result) ചെയ്യുക { വ്യഞ്ജകം; CV_OCL_CHECK_RESULT(check_result, #expr); } ആകുമ്പോൾ (0)

#define CV_OCL_CHECK(വ്യഞ്ജകം) ചെയ്യുക { cl_int __cl_result = (വ്യഞ്ജകം); CV_OCL_CHECK_RESULT(__cl_result, #expr); } ആകുമ്പോൾ (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, സന്ദേശം) CV_OCL_CHECK_RESULT(check_result, സന്ദേശം)
#define CV_OCL_DBG_CHECK(വ്യഞ്ജകം) CV_OCL_CHECK(വ്യഞ്ജകം)
#define CV_OCL_DBG_CHECK_(വ്യഞ്ജകം, check_result) CV_OCL_CHECK_(വ്യഞ്ജകം, check_result)
#അല്ലെങ്കിൽ
#define CV_OCL_DBG_CHECK_RESULT(check_result, സന്ദേശം) \
    ചെയ്യുക { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, സന്ദേശം); \
        എങ്കിൽ (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(സന്ദേശം), സ്ഥിരം അക്ഷരം*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_സന്ദേശം = CV_OCL_API_ERROR_സന്ദേശം(check_result, സന്ദേശം); \
            CV_Error(Error::OpenCLApiCallError, error_സന്ദേശം); \
        } \
    } ആകുമ്പോൾ (0)
#define CV_OCL_DBG_CHECK_(വ്യഞ്ജകം, check_result) ചെയ്യുക { വ്യഞ്ജകം; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } ആകുമ്പോൾ (0)
#define CV_OCL_DBG_CHECK(വ്യഞ്ജകം) ചെയ്യുക { cl_int __cl_result = (വ്യഞ്ജകം); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } ആകുമ്പോൾ (0)
#endif


സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", സത്യം);
സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", സത്യം);
സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", സത്യം);
സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", സത്യം);

#എങ്കിൽ CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS
സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS_മൂല്യം = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", അസത്യം);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#എങ്കിൽ 1 //def __APPLE__
        സത്യം
#അല്ലെങ്കിൽ
        അസത്യം
#endif
);

സ്ഥിതം String getBuildExtraOptions()
{
    സ്ഥിതം String param_buildExtraOptions;
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = സത്യം;
        എങ്കിൽ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ശൂന്യ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    തിരികെ param_buildExtraOptions;
}

സ്ഥിതം സ്ഥിരം തർക്കം CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", സത്യം);
സ്ഥിതം സ്ഥിരം size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ഘടന UMat2D
{
    UMat2D(സ്ഥിരം UMat& m)
    {
        offset = (പൂർണ്ണസംഖ്യ)m.offset;
        step = (പൂർണ്ണസംഖ്യ)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    പൂർണ്ണസംഖ്യ offset;
    പൂർണ്ണസംഖ്യ step;
    പൂർണ്ണസംഖ്യ rows;
    പൂർണ്ണസംഖ്യ cols;
};

ഘടന UMat3D
{
    UMat3D(സ്ഥിരം UMat& m)
    {
        offset = (പൂർണ്ണസംഖ്യ)m.offset;
        step = (പൂർണ്ണസംഖ്യ)m.step.p[1];
        slicestep = (പൂർണ്ണസംഖ്യ)m.step.p[0];
        slices = (പൂർണ്ണസംഖ്യ)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    പൂർണ്ണസംഖ്യ offset;
    പൂർണ്ണസംഖ്യ slicestep;
    പൂർണ്ണസംഖ്യ step;
    പൂർണ്ണസംഖ്യ slices;
    പൂർണ്ണസംഖ്യ rows;
    പൂർണ്ണസംഖ്യ cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
സ്ഥിതം uint64 crc64( സ്ഥിരം uchar* data, size_t size, uint64 crc0=0 )
{
    സ്ഥിതം uint64 table[256];
    സ്ഥിതം തർക്കം initialized = അസത്യം;

    എങ്കിൽ( !initialized )
    {
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < 256; i++ )
        {
            uint64 c = i;
            വേണ്ടി( പൂർണ്ണസംഖ്യ j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = സത്യം;
    }

    uint64 crc = ~crc0;
    വേണ്ടി( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    തിരികെ ~crc;
}

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
ഘടന OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    തരനിർവചനം std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ശൂന്യ, "Initializing OpenCL cache configuration...");
        എങ്കിൽ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL cache is disabled");
            തിരികെ;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        എങ്കിൽ (cache_path_.empty())
        {
            CV_LOG_INFO(ശൂന്യ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        ചെയ്യുക
        {
            ശ്രമിക്കുക
            {
                എങ്കിൽ (cache_path_.empty())
                    നിർത്തുക;
                എങ്കിൽ (cache_path_ == "disabled")
                    നിർത്തുക;
                എങ്കിൽ (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(ശൂന്യ, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    നിർത്തുക;
                }

                എങ്കിൽ (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    എങ്കിൽ (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(ശൂന്യ, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        എങ്കിൽ (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(ശൂന്യ, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            നിർത്തുക;
                        }
                    }

                    ശ്രമിക്കുക
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(ശൂന്യ, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(ശൂന്യ, 0, "Checking cache lock... Done!");
                    }
                    പിടിക്കുക (സ്ഥിരം cv::Exception& e)
                    {
                        CV_LOG_WARNING(ശൂന്യ, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    പിടിക്കുക (...)
                    {
                        CV_LOG_WARNING(ശൂന്യ, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                അല്ലെങ്കിൽ
                {
                    എങ്കിൽ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ശൂന്യ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    അല്ലെങ്കിൽ
                    {
                        CV_LOG_INFO(ശൂന്യ, "OpenCL cache lock is disabled");
                    }
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                CV_LOG_WARNING(ശൂന്യ, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } ആകുമ്പോൾ (0);
        എങ്കിൽ (!cache_path_.empty())
        {
            എങ്കിൽ (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(ശൂന്യ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_INFO(ശൂന്യ, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    ശൂന്യം clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(സ്ഥിരം std::string& ctx_prefix,
            സ്ഥിരം std::string& cleanup_prefix)
    {
        എങ്കിൽ (cache_path_.empty())
            തിരികെ std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        എങ്കിൽ (found_it != prepared_contexts_.end())
            തിരികെ found_it->second;

        CV_LOG_INFO(ശൂന്യ, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        തർക്കം result = utils::fs::isDirectory(target_directory);
        എങ്കിൽ (!result)
        {
            ശ്രമിക്കുക
            {
                CV_LOG_VERBOSE(ശൂന്യ, 0, "Creating directory: " << target_directory);
                എങ്കിൽ (utils::fs::createDirectories(target_directory))
                {
                    result = സത്യം;
                }
                അല്ലെങ്കിൽ
                {
                    CV_LOG_WARNING(ശൂന്യ, "Can't create directory: " << target_directory);
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        എങ്കിൽ (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            ശ്രമിക്കുക
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, അസത്യം, സത്യം);
                std::vector<String> remove_entries;
                വേണ്ടി (size_t i = 0; i < entries.size(); i++)
                {
                    സ്ഥിരം String& name = entries[i];
                    എങ്കിൽ (0 == name.find(cleanup_prefix))
                    {
                        എങ്കിൽ (0 == name.find(ctx_prefix))
                            തുടരുക; // skip current
                        remove_entries.push_back(name);
                    }
                }
                എങ്കിൽ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ശൂന്യ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ശൂന്യ, "Trying to remove these directories...");
                    വേണ്ടി (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ശൂന്യ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ശൂന്യ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    വേണ്ടി (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        സ്ഥിരം String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        ശ്രമിക്കുക
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(ശൂന്യ, "Removed: " << path);
                        }
                        പിടിക്കുക (സ്ഥിരം cv::Exception& e)
                        {
                            CV_LOG_ERROR(ശൂന്യ, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            പിടിക്കുക (...)
            {
                CV_LOG_WARNING(ശൂന്യ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ശൂന്യ, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        തിരികെ target_directory;
    }

    സ്ഥിതം OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, പുതിയ OpenCLBinaryCacheConfigurator());
    }
};
വർഗ്ഗം ദ്വിമാന_Program_File
{
    എണ്ണൽ { MAX_ENTRIES = 64 };

    തരനിർവചനം unsigned പൂർണ്ണസംഖ്യ uint32_t;

    ഘടന CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    ഘടന CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t സംഖ്യ_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    ഘടന CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t താക്കോൽ_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    സ്ഥിരം std::string fileName_;
    സ്ഥിരം അക്ഷരം* സ്ഥിരം sourceSignature_;
    സ്ഥിരം size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(സ്ഥിരം std::string& options)
    {
        uint64 hash = crc64((സ്ഥിരം uchar*)options.c_str(), options.size(), 0);
        തിരികെ hash & (MAX_ENTRIES - 1);
    }

    വരിയിൽ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        തിരികെ fileSize;
    }
    വരിയിൽ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.പഠ((അക്ഷരം*)&res, വലുപ്പം(uint32_t));
        CV_Assert(!f.fail());
        തിരികെ res;
    }
    വരിയിൽ ശൂന്യം writeUInt32(സ്ഥിരം uint32_t value)
    {
        uint32_t v = value;
        f.ലിഖ((അക്ഷരം*)&v, വലുപ്പം(uint32_t));
        CV_Assert(!f.fail());
    }

    വരിയിൽ ശൂന്യം seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    വരിയിൽ ശൂന്യം seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    വരിയിൽ ശൂന്യം seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    ശൂന്യം clearFile()
    {
        f.close();
        എങ്കിൽ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ശൂന്യ, "Can't remove: " << fileName_);
        തിരികെ;
    }

പൊതു:
    ദ്വിമാന_Program_File(സ്ഥിരം std::string& fileName, സ്ഥിരം അക്ഷരം* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(വലുപ്പം(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != ശൂന്യ);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, വലുപ്പം(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        എങ്കിൽ(f.is_open() && getFileSize() > 0)
        {
            തർക്കം isValid = അസത്യം;
            ശ്രമിക്കുക
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                എങ്കിൽ (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<അക്ഷരം> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.പഠ(fileSourceSignature.data(), fileSourceSignatureSize);
                    എങ്കിൽ (f.eof())
                    {
                        CV_LOG_ERROR(ശൂന്യ, "Unexpected EOF");
                    }
                    അല്ലെങ്കിൽ എങ്കിൽ (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = സത്യം;
                    }
                }
                എങ്കിൽ (!isValid)
                {
                    CV_LOG_ERROR(ശൂന്യ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            പിടിക്കുക (...)
            {
                CV_LOG_ERROR(ശൂന്യ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            എങ്കിൽ (!isValid)
            {
                clearFile();
            }
            അല്ലെങ്കിൽ
            {
                seekReadAbsolute(0);
            }
        }
    }

    തർക്കം പഠ(സ്ഥിരം std::string& താക്കോൽ, std::vector<അക്ഷരം>& buf)
    {
        എങ്കിൽ (!f.is_open())
            തിരികെ അസത്യം;

        size_t fileSize = getFileSize();
        എങ്കിൽ (fileSize == 0)
        {
            CV_LOG_ERROR(ശൂന്യ, "Invalid file (empty): " << fileName_);
            clearFile();
            തിരികെ അസത്യം;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t സംഖ്യ_Of_Entries = readUInt32();
        CV_Assert(സംഖ്യ_Of_Entries > 0);
        എങ്കിൽ (സംഖ്യ_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ശൂന്യ, "Invalid file: " << fileName_);
            clearFile();
            തിരികെ അസത്യം;
        }
        f.പഠ((അക്ഷരം*)&entryOffsets[0], വലുപ്പം(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_അക്കം = getHash(താക്കോൽ);

        uint32_t entryOffset = entryOffsets[entry_അക്കം];
        FileEntry entry;
        ആകുമ്പോൾ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.പഠ((അക്ഷരം*)&entry, വലുപ്പം(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<അക്ഷരം> file_താക്കോൽ(entry.താക്കോൽ_Size + 1);
            എങ്കിൽ (താക്കോൽ.size() == entry.താക്കോൽ_Size)
            {
                എങ്കിൽ (entry.താക്കോൽ_Size > 0)
                {
                    f.പഠ(file_താക്കോൽ.data(), entry.താക്കോൽ_Size);
                    CV_Assert(!f.fail());
                }
                എങ്കിൽ (memcmp(file_താക്കോൽ.data(), താക്കോൽ.c_str(), entry.താക്കോൽ_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.പഠ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ശൂന്യ, 0, "Read...");
                    തിരികെ സത്യം;
                }
            }
            എങ്കിൽ (entry.nextEntryFileOffset == 0)
                നിർത്തുക;
            entryOffset = entry.nextEntryFileOffset;
        }
        തിരികെ അസത്യം;
    }

    തർക്കം ലിഖ(സ്ഥിരം std::string& താക്കോൽ, std::vector<അക്ഷരം>& buf)
    {
        എങ്കിൽ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            എങ്കിൽ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                എങ്കിൽ (!f.is_open())
                {
                    CV_LOG_ERROR(ശൂന്യ, "Can't create file: " << fileName_);
                    തിരികെ അസത്യം;
                }
            }
        }

        size_t fileSize = getFileSize();
        എങ്കിൽ (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.ലിഖ(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, വലുപ്പം(entryOffsets));
            f.ലിഖ((അക്ഷരം*)entryOffsets, വലുപ്പം(entryOffsets));
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

        uint32_t സംഖ്യ_Of_Entries = readUInt32();
        CV_Assert(സംഖ്യ_Of_Entries > 0);
        എങ്കിൽ (സംഖ്യ_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ശൂന്യ, "Invalid file: " << fileName_);
            clearFile();
            തിരികെ അസത്യം;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.പഠ((അക്ഷരം*)&entryOffsets[0], വലുപ്പം(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_അക്കം = getHash(താക്കോൽ);

        uint32_t entryOffset = entryOffsets[entry_അക്കം];
        FileEntry entry;
        ആകുമ്പോൾ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.പഠ((അക്ഷരം*)&entry, വലുപ്പം(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<അക്ഷരം> file_താക്കോൽ(entry.താക്കോൽ_Size + 1);
            എങ്കിൽ (താക്കോൽ.size() == entry.താക്കോൽ_Size)
            {
                എങ്കിൽ (entry.താക്കോൽ_Size > 0)
                {
                    f.പഠ(file_താക്കോൽ.data(), entry.താക്കോൽ_Size);
                    CV_Assert(!f.fail());
                }
                എങ്കിൽ (0 == memcmp(file_താക്കോൽ.data(), താക്കോൽ.c_str(), entry.താക്കോൽ_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ശൂന്യ, 0, "Duplicate key ignored: " << fileName_);
                    തിരികെ അസത്യം;
                }
            }
            എങ്കിൽ (entry.nextEntryFileOffset == 0)
                നിർത്തുക;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        എങ്കിൽ (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.ലിഖ((അക്ഷരം*)&entry, വലുപ്പം(entry));
            CV_Assert(!f.fail());
        }
        അല്ലെങ്കിൽ
        {
            entryOffsets[entry_അക്കം] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.ലിഖ((അക്ഷരം*)entryOffsets, വലുപ്പം(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.താക്കോൽ_Size = (uint32_t)താക്കോൽ.size();
        f.ലിഖ((അക്ഷരം*)&entry, വലുപ്പം(entry));
        CV_Assert(!f.fail());
        f.ലിഖ(താക്കോൽ.c_str(), entry.താക്കോൽ_Size);
        CV_Assert(!f.fail());
        f.ലിഖ(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ശൂന്യ, 0, "Write... (" << buf.size() << " bytes)");
        തിരികെ സത്യം;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



ഘടന OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    പൂർണ്ണസംഖ്യ device_;  // device index in context
    ocl::Queue queue_;
    പൂർണ്ണസംഖ്യ useOpenCL_;

സംരക്ഷിത:
    Impl() = delete;

    ശൂന്യം _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        പൂർണ്ണസംഖ്യ ndevices = (പൂർണ്ണസംഖ്യ)context_.ndevices();
        CV_Assert(ndevices > 0);
        തർക്കം found = അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            എങ്കിൽ (dhandle == deviceID)
            {
                device_ = i;
                found = സത്യം;
                നിർത്തുക;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    ശൂന്യം _init_device(സ്ഥിരം ocl::Device& device)
    {
        CV_Assert(device.ptr());
        പൂർണ്ണസംഖ്യ ndevices = (പൂർണ്ണസംഖ്യ)context_.ndevices();
        CV_Assert(ndevices > 0);
        തർക്കം found = അസത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            എങ്കിൽ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = സത്യം;
                നിർത്തുക;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

പൊതു:
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

    Impl(സ്ഥിരം ocl::Context& context, സ്ഥിരം ocl::Device& device, സ്ഥിരം ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(സ്ഥിരം ocl::Context& context, സ്ഥിരം ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(സ്ഥിരം ocl::Context& context, സ്ഥിരം പൂർണ്ണസംഖ്യ device, സ്ഥിരം ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(സ്ഥിരം Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    വരിയിൽ തർക്കം useOpenCL() സ്ഥിരം { തിരികെ const_cast<Impl*>(ഇത്)->useOpenCL(); }
    തർക്കം useOpenCL()
    {
        എങ്കിൽ (useOpenCL_ < 0)
        {
            ശ്രമിക്കുക
            {
                useOpenCL_ = 0;
                എങ്കിൽ (!context_.empty() && context_.ndevices() > 0)
                {
                    സ്ഥിരം Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception&)
            {
                // nothing
            }
            എങ്കിൽ (!useOpenCL_)
                CV_LOG_INFO(ശൂന്യ, "OpenCL: can't use OpenCL execution context");
        }
        തിരികെ useOpenCL_ > 0;
    }

    ശൂന്യം setUseOpenCL(തർക്കം flag)
    {
        എങ്കിൽ (!flag)
            useOpenCL_ = 0;
        അല്ലെങ്കിൽ
            useOpenCL_ = -1;
    }

    സ്ഥിതം സ്ഥിരം std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ശൂന്യ, "OpenCL: initializing thread execution context");

        സ്ഥിതം തർക്കം initialized = അസത്യം;
        സ്ഥിതം std::shared_ptr<Impl> g_primaryExecutionContext;

        എങ്കിൽ (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            എങ്കിൽ (!initialized)
            {
                CV_LOG_INFO(ശൂന്യ, "OpenCL: creating new execution context...");
                ശ്രമിക്കുക
                {
                    Context c = ocl::Context::create(std::string());
                    എങ്കിൽ (c.ndevices())
                    {
                        പൂർണ്ണസംഖ്യ deviceId = 0;
                        auto& d = c.device(deviceId);
                        എങ്കിൽ (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            എങ്കിൽ (!q.ptr())
                            {
                                CV_LOG_ERROR(ശൂന്യ, "OpenCL: Can't create default OpenCL queue");
                            }
                            അല്ലെങ്കിൽ
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ശൂന്യ, "OpenCL: device=" << d.name());
                            }
                        }
                        അല്ലെങ്കിൽ
                        {
                            CV_LOG_ERROR(ശൂന്യ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    അല്ലെങ്കിൽ
                    {
                        CV_LOG_INFO(ശൂന്യ, "OpenCL: context is not available/disabled");
                    }
                }
                പിടിക്കുക (സ്ഥിരം std::exception& e)
                {
                    CV_LOG_INFO(ശൂന്യ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                പിടിക്കുക (...)
                {
                    CV_LOG_WARNING(ശൂന്യ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = സത്യം;
            }
        }
        തിരികെ g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->context_;
}
Device& OpenCLExecutionContext::getDevice() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->queue_;
}

തർക്കം OpenCLExecutionContext::useOpenCL() സ്ഥിരം
{
    എങ്കിൽ (p)
        തിരികെ p->useOpenCL();
    തിരികെ അസത്യം;
}
ശൂന്യം OpenCLExecutionContext::setUseOpenCL(തർക്കം flag)
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
    എങ്കിൽ (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = സത്യം;
        എങ്കിൽ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    തിരികെ c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    തിരികെ c;
}

ശൂന്യം OpenCLExecutionContext::bind() സ്ഥിരം
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *ഇത്;
    data.oclExecutionContextInitialized = സത്യം;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() സ്ഥിരം
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    സ്ഥിരം Queue q(getContext(), getDevice());
    തിരികെ cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(സ്ഥിരം ocl::Queue& q) സ്ഥിരം
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ശൂന്യ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    തിരികെ c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(സ്ഥിരം Context& context, സ്ഥിരം Device& device, സ്ഥിരം ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    എങ്കിൽ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    തിരികെ ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(സ്ഥിരം Context& context, സ്ഥിരം Device& device)
{
    CV_TRACE_FUNCTION();
    എങ്കിൽ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    തിരികെ ctx;

}

ശൂന്യം OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
സ്ഥിതം തർക്കം g_isOpenCLInitialized = അസത്യം;
സ്ഥിതം തർക്കം g_isOpenCLAvailable = അസത്യം;

തർക്കം haveOpenCL()
{
    CV_TRACE_FUNCTION();

    എങ്കിൽ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        എങ്കിൽ (!envPath.empty())
        {
            എങ്കിൽ (envPath == "disabled")
            {
                g_isOpenCLAvailable = അസത്യം;
                g_isOpenCLInitialized = സത്യം;
                തിരികെ അസത്യം;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(ശൂന്യ, "Initialize OpenCL runtime...");
        ശ്രമിക്കുക
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ശൂന്യ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ശൂന്യ, "OpenCL: found " << n << " platforms");
        }
        പിടിക്കുക (...)
        {
            g_isOpenCLAvailable = അസത്യം;
        }
        g_isOpenCLInitialized = സത്യം;
    }
    തിരികെ g_isOpenCLAvailable;
}

തർക്കം useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    എങ്കിൽ (data.useOpenCL < 0)
    {
        ശ്രമിക്കുക
        {
            data.useOpenCL = 0;
            എങ്കിൽ (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        പിടിക്കുക (...)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    തിരികെ data.useOpenCL > 0;
}

തർക്കം isOpenCLActivated()
{
    എങ്കിൽ (!g_isOpenCLAvailable)
        തിരികെ അസത്യം; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    തിരികെ useOpenCL();
}

ശൂന്യം setUseOpenCL(തർക്കം flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    എങ്കിൽ (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    അല്ലെങ്കിൽ
    {
        എങ്കിൽ (!flag)
            data.useOpenCL = 0;
        അല്ലെങ്കിൽ
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

വർഗ്ഗം AmdBlasHelper
{
പൊതു:
    സ്ഥിതം AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, പുതിയ AmdBlasHelper())
    }

    തർക്കം isAvailable() സ്ഥിരം
    {
        തിരികെ g_isAmdBlasAvailable;
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

സംരക്ഷിത:
    AmdBlasHelper()
    {
        എങ്കിൽ (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            എങ്കിൽ (!g_isAmdBlasInitialized)
            {
                എങ്കിൽ (haveOpenCL())
                {
                    ശ്രമിക്കുക
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    പിടിക്കുക (...)
                    {
                        g_isAmdBlasAvailable = അസത്യം;
                    }
                }
                അല്ലെങ്കിൽ
                    g_isAmdBlasAvailable = അസത്യം;

                g_isAmdBlasInitialized = സത്യം;
            }
        }
    }

സ്വകാര്യ:
    സ്ഥിതം തർക്കം g_isAmdBlasInitialized;
    സ്ഥിതം തർക്കം g_isAmdBlasAvailable;
};

തർക്കം AmdBlasHelper::g_isAmdBlasAvailable = അസത്യം;
തർക്കം AmdBlasHelper::g_isAmdBlasInitialized = അസത്യം;

തർക്കം haveAmdBlas()
{
    തിരികെ AmdBlasHelper::getInstance().isAvailable();
}

#അല്ലെങ്കിൽ

തർക്കം haveAmdBlas()
{
    തിരികെ അസത്യം;
}

#endif

#ifdef HAVE_CLAMDFFT

വർഗ്ഗം AmdFftHelper
{
പൊതു:
    സ്ഥിതം AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, പുതിയ AmdFftHelper())
    }

    തർക്കം isAvailable() സ്ഥിരം
    {
        തിരികെ g_isAmdFftAvailable;
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

സംരക്ഷിത:
    AmdFftHelper()
    {
        എങ്കിൽ (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            എങ്കിൽ (!g_isAmdFftInitialized)
            {
                എങ്കിൽ (haveOpenCL())
                {
                    ശ്രമിക്കുക
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = സത്യം;
                    }
                    പിടിക്കുക (സ്ഥിരം Exception &)
                    {
                        g_isAmdFftAvailable = അസത്യം;
                    }
                }
                അല്ലെങ്കിൽ
                    g_isAmdFftAvailable = അസത്യം;

                g_isAmdFftInitialized = സത്യം;
            }
        }
    }

സ്വകാര്യ:
    സ്ഥിതം clfftSetupData setupData;
    സ്ഥിതം തർക്കം g_isAmdFftInitialized;
    സ്ഥിതം തർക്കം g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
തർക്കം AmdFftHelper::g_isAmdFftAvailable = അസത്യം;
തർക്കം AmdFftHelper::g_isAmdFftInitialized = അസത്യം;

തർക്കം haveAmdFft()
{
    തിരികെ AmdFftHelper::getInstance().isAvailable();
}

#അല്ലെങ്കിൽ

തർക്കം haveAmdFft()
{
    തിരികെ അസത്യം;
}

#endif

തർക്കം haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    തിരികെ സത്യം;
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ഘടന Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = അസത്യം;
    }

    ~Impl() {}

    ശൂന്യം init()
    {
        എങ്കിൽ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            എങ്കിൽ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            എങ്കിൽ( handle != 0 )
            {
                അക്ഷരം buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, വലുപ്പം(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = സത്യം;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    തർക്കം initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    എങ്കിൽ(p)
        p->release();
}

Platform::Platform(സ്ഥിരം Platform& pl)
{
    p = (Impl*)pl.p;
    എങ്കിൽ(p)
        p->addref();
}

Platform& Platform::operator = (സ്ഥിരം Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &pl) {
        എങ്കിൽ(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    തിരികെ *ഇത്;
}

ശൂന്യം* Platform::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ശൂന്യ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    സ്ഥിതം Platform p;
    എങ്കിൽ( !p.p )
    {
        p.p = പുതിയ Impl;
        p.p->init();
    }
    തിരികെ p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
സ്ഥിതം ശൂന്യം parseOpenCLVersion(സ്ഥിരം String &version, പൂർണ്ണസംഖ്യ &major, പൂർണ്ണസംഖ്യ &minor)
{
    major = minor = 0;
    എങ്കിൽ (10 >= version.length())
        തിരികെ;
    സ്ഥിരം അക്ഷരം *pstr = version.c_str();
    എങ്കിൽ (0 != strncmp(pstr, "OpenCL ", 7))
        തിരികെ;
    size_t ppos = version.find('.', 7);
    എങ്കിൽ (String::npos == ppos)
        തിരികെ;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

ഘടന Device::Impl
{
    Impl(ശൂന്യം* d)
        : refcount(1)
        , handle(0)
    {
        ശ്രമിക്കുക
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        പിടിക്കുക (...)
        {
            എറിയുക;
        }
    }

    ശൂന്യം _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, പൂർണ്ണസംഖ്യ>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, പൂർണ്ണസംഖ്യ>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, പൂർണ്ണസംഖ്യ>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        ആകുമ്പോൾ (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            എങ്കിൽ (pos2 == String::npos)
                pos2 = extensions_.size();
            എങ്കിൽ (pos2 > pos)
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
        എങ്കിൽ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        അല്ലെങ്കിൽ എങ്കിൽ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        അല്ലെങ്കിൽ എങ്കിൽ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        അല്ലെങ്കിൽ
            vendorID_ = UNKNOWN_VENDOR;

        സ്ഥിരം size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        എങ്കിൽ (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            സ്ഥിരം size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            എങ്കിൽ (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(ശൂന്യ, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#എങ്കിൽ 0
        എങ്കിൽ (isExtensionSupported("cl_khr_spir"))
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
        എങ്കിൽ (!cv::__termination)
#endif
        {
            എങ്കിൽ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) സ്ഥിരം
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        തിരികെ clGetDeviceInfo(handle, prop, വലുപ്പം(temp), &temp, &sz) == CL_SUCCESS &&
            sz == വലുപ്പം(temp) ? _TpOut(temp) : _TpOut();
    }

    തർക്കം getBoolProp(cl_device_info prop) സ്ഥിരം
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        തിരികെ clGetDeviceInfo(handle, prop, വലുപ്പം(temp), &temp, &sz) == CL_SUCCESS &&
            sz == വലുപ്പം(temp) ? temp != 0 : അസത്യം;
    }

    String getStrProp(cl_device_info prop) സ്ഥിരം
    {
        അക്ഷരം buf[4096];
        size_t sz=0;
        തിരികെ clGetDeviceInfo(handle, prop, വലുപ്പം(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < വലുപ്പം(buf) ? String(buf) : String();
    }

    തർക്കം isExtensionSupported(സ്ഥിരം std::string& extensionName) സ്ഥിരം
    {
        തിരികെ extensions_set_.എണ്ണം_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    പൂർണ്ണസംഖ്യ doubleFPConfig_;
    തർക്കം khr_fp64_support_;
    പൂർണ്ണസംഖ്യ halfFPConfig_;
    തർക്കം khr_fp16_support_;
    തർക്കം hostUnifiedMemory_;
    പൂർണ്ണസംഖ്യ maxComputeUnits_;
    size_t maxWorkGroupSize_;
    പൂർണ്ണസംഖ്യ type_;
    പൂർണ്ണസംഖ്യ addressBits_;
    പൂർണ്ണസംഖ്യ deviceVersionMajor_;
    പൂർണ്ണസംഖ്യ deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    പൂർണ്ണസംഖ്യ vendorID_;
    തർക്കം intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(ശൂന്യം* d)
{
    p = 0;
    set(d);
}

Device::Device(സ്ഥിരം Device& d)
{
    p = d.p;
    എങ്കിൽ(p)
        p->addref();
}

Device& Device::operator = (സ്ഥിരം Device& d)
{
    Impl* newp = (Impl*)d.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &d) {
        എങ്കിൽ(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    തിരികെ *ഇത്;
}

Device::~Device()
{
    എങ്കിൽ(p)
        p->release();
}

ശൂന്യം Device::set(ശൂന്യം* d)
{
    എങ്കിൽ(p)
        p->release();
    p = പുതിയ Impl(d);
    എങ്കിൽ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(ശൂന്യം* d)
{
    Device device(d);
    തിരികെ device;
}

ശൂന്യം* Device::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

String Device::name() സ്ഥിരം
{ തിരികെ p ? p->name_ : String(); }

String Device::extensions() സ്ഥിരം
{ തിരികെ p ? String(p->extensions_) : String(); }

തർക്കം Device::isExtensionSupported(സ്ഥിരം String& extensionName) സ്ഥിരം
{ തിരികെ p ? p->isExtensionSupported(extensionName) : അസത്യം; }

String Device::version() സ്ഥിരം
{ തിരികെ p ? p->version_ : String(); }

String Device::vendorName() സ്ഥിരം
{ തിരികെ p ? p->vendorName_ : String(); }

പൂർണ്ണസംഖ്യ Device::vendorID() സ്ഥിരം
{ തിരികെ p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() സ്ഥിരം
{ തിരികെ p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() സ്ഥിരം
{ തിരികെ p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

പൂർണ്ണസംഖ്യ Device::deviceVersionMajor() സ്ഥിരം
{ തിരികെ p ? p->deviceVersionMajor_ : 0; }

പൂർണ്ണസംഖ്യ Device::deviceVersionMinor() സ്ഥിരം
{ തിരികെ p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() സ്ഥിരം
{ തിരികെ p ? p->driverVersion_ : String(); }

പൂർണ്ണസംഖ്യ Device::type() സ്ഥിരം
{ തിരികെ p ? p->type_ : 0; }

പൂർണ്ണസംഖ്യ Device::addressBits() സ്ഥിരം
{ തിരികെ p ? p->addressBits_ : 0; }

തർക്കം Device::available() സ്ഥിരം
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : അസത്യം; }

തർക്കം Device::compilerAvailable() സ്ഥിരം
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : അസത്യം; }

തർക്കം Device::linkerAvailable() സ്ഥിരം
#ifdef CL_VERSION_1_2
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : അസത്യം; }
#അല്ലെങ്കിൽ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

പൂർണ്ണസംഖ്യ Device::doubleFPConfig() സ്ഥിരം
{ തിരികെ p ? p->doubleFPConfig_ : 0; }

പൂർണ്ണസംഖ്യ Device::singleFPConfig() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_device_fp_config, പൂർണ്ണസംഖ്യ>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

പൂർണ്ണസംഖ്യ Device::halfFPConfig() സ്ഥിരം
{ തിരികെ p ? p->halfFPConfig_ : 0; }

തർക്കം Device::hasFP64() സ്ഥിരം
{ തിരികെ p ? p->khr_fp64_support_ : അസത്യം; }
തർക്കം Device::hasFP16() സ്ഥിരം
{ തിരികെ p ? p->khr_fp16_support_ : അസത്യം; }

തർക്കം Device::endianLittle() സ്ഥിരം
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : അസത്യം; }

തർക്കം Device::errorCorrectionSupport() സ്ഥിരം
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : അസത്യം; }

പൂർണ്ണസംഖ്യ Device::executionCapabilities() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_device_exec_capabilities, പൂർണ്ണസംഖ്യ>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

പൂർണ്ണസംഖ്യ Device::globalMemCacheType() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_device_mem_cache_type, പൂർണ്ണസംഖ്യ>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

പൂർണ്ണസംഖ്യ Device::global_Mem_Cache_വരി_Size() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

പൂർണ്ണസംഖ്യ Device::localMemType() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_device_local_mem_type, പൂർണ്ണസംഖ്യ>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

തർക്കം Device::hostUnifiedMemory() സ്ഥിരം
{ തിരികെ p ? p->hostUnifiedMemory_ : അസത്യം; }

തർക്കം Device::imageSupport() സ്ഥിരം
{ തിരികെ p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : അസത്യം; }

തർക്കം Device::imageFromBufferSupport() സ്ഥിരം
{
    തിരികെ p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : അസത്യം;
}

uint Device::imagePitchAlignment() സ്ഥിരം
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    തിരികെ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
}

uint Device::imageBaseAddressAlignment() സ്ഥിരം
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    തിരികെ p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
}

size_t Device::image2DMaxWidth() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() സ്ഥിരം
#ifdef CL_VERSION_1_2
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#അല്ലെങ്കിൽ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() സ്ഥിരം
#ifdef CL_VERSION_1_2
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#അല്ലെങ്കിൽ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

തർക്കം Device::intelSubgroupsSupport() സ്ഥിരം
{ തിരികെ p ? p->intelSubgroupsSupport_ : അസത്യം; }

പൂർണ്ണസംഖ്യ Device::maxClockFrequency() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

പൂർണ്ണസംഖ്യ Device::maxComputeUnits() സ്ഥിരം
{ തിരികെ p ? p->maxComputeUnits_ : 0; }

പൂർണ്ണസംഖ്യ Device::maxConstantArgs() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

പൂർണ്ണസംഖ്യ Device::maxReadImageArgs() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

പൂർണ്ണസംഖ്യ Device::maxWriteImageArgs() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

പൂർണ്ണസംഖ്യ Device::maxSamplers() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() സ്ഥിരം
{ തിരികെ p ? p->maxWorkGroupSize_ : 0; }

പൂർണ്ണസംഖ്യ Device::maxWorkItemDims() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

ശൂന്യം Device::maxWorkItemSizes(size_t* sizes) സ്ഥിരം
{
    എങ്കിൽ(p)
    {
        സ്ഥിരം പൂർണ്ണസംഖ്യ MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*വലുപ്പം(sizes[0]), &sizes[0], &retsz));
    }
}

പൂർണ്ണസംഖ്യ Device::memBaseAddrAlign() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthChar() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthShort() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthInt() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthLong() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthFloat() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthDouble() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

പൂർണ്ണസംഖ്യ Device::nativeVectorWidthHalf() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthChar() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthShort() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthInt() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthLong() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthFloat() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthDouble() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

പൂർണ്ണസംഖ്യ Device::preferredVectorWidthHalf() സ്ഥിരം
{ തിരികെ p ? p->getProp<cl_uint, പൂർണ്ണസംഖ്യ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() സ്ഥിരം
#ifdef CL_VERSION_1_2
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#അല്ലെങ്കിൽ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() സ്ഥിരം
{ തിരികെ p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

സ്ഥിരം Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    എങ്കിൽ (!c.empty())
    {
        തിരികെ c.getDevice();
    }

    സ്ഥിതം Device dummy;
    തിരികെ dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
വരിയിൽ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ശൂന്യ, &required);
    എങ്കിൽ (err != CL_SUCCESS)
        തിരികെ err;

    param.clear();
    എങ്കിൽ (required > 0)
    {
        AutoBuffer<അക്ഷരം> buf(required + 1);
        അക്ഷരം* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, ശൂന്യ);
        എങ്കിൽ (err != CL_SUCCESS)
            തിരികെ err;
        param = ptr;
    }

    തിരികെ CL_SUCCESS;
}

സ്ഥിതം ശൂന്യം split(സ്ഥിരം std::string &s, അക്ഷരം delim, std::vector<std::string> &elems)
{
    elems.clear();
    എങ്കിൽ (s.size() == 0)
        തിരികെ;
    std::istringstream ss(s);
    std::string item;
    ആകുമ്പോൾ (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
സ്ഥിതം തർക്കം parseOpenCLDeviceConfiguration(സ്ഥിരം std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    എങ്കിൽ (parts.size() > 3)
    {
        CV_LOG_ERROR(ശൂന്യ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        തിരികെ അസത്യം;
    }
    എങ്കിൽ (parts.size() > 2)
        deviceNameOrID = parts[2];
    എങ്കിൽ (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    എങ്കിൽ (parts.size() > 0)
    {
        platform = parts[0];
    }
    തിരികെ സത്യം;
}

സ്ഥിതം cl_device_id selectOpenCLDevice(സ്ഥിരം std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    എങ്കിൽ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    എങ്കിൽ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        തിരികെ ശൂന്യ;

    തർക്കം isID = അസത്യം;
    പൂർണ്ണസംഖ്യ deviceID = -1;
    എങ്കിൽ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = സത്യം;
        വേണ്ടി (size_t i = 0; i < deviceName.length(); i++)
        {
            എങ്കിൽ (!isdigit(deviceName[i]))
            {
                isID = അസത്യം;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            എങ്കിൽ (deviceID < 0)
                തിരികെ ശൂന്യ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint അക്കം_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ശൂന്യ, &അക്കം_Platforms));

        എങ്കിൽ (അക്കം_Platforms == 0)
            തിരികെ ശൂന്യ;
        platforms.resize((size_t)അക്കം_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(അക്കം_Platforms, &platforms[0], &അക്കം_Platforms));
        platforms.resize(അക്കം_Platforms);
    }

    എങ്കിൽ (platform.length() > 0)
    {
        വേണ്ടി (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            എങ്കിൽ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            അല്ലെങ്കിൽ
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        എങ്കിൽ (platforms.size() == 0)
        {
            CV_LOG_ERROR(ശൂന്യ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            പോകുക not_found;
        }
    }
    എങ്കിൽ (deviceTypes.size() == 0)
    {
        എങ്കിൽ (!isID)
        {
            deviceTypes.push_back("GPU");
            എങ്കിൽ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        അല്ലെങ്കിൽ
            deviceTypes.push_back("ALL");
    }
    വേണ്ടി (size_t t = 0; t < deviceTypes.size(); t++)
    {
        പൂർണ്ണസംഖ്യ deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        എങ്കിൽ (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        അല്ലെങ്കിൽ എങ്കിൽ (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        അല്ലെങ്കിൽ എങ്കിൽ (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        അല്ലെങ്കിൽ എങ്കിൽ (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        അല്ലെങ്കിൽ
        {
            CV_LOG_ERROR(ശൂന്യ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            പോകുക not_found;
        }

        std::vector<cl_device_id> devices;
        വേണ്ടി (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint എണ്ണം_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, ശൂന്യ, &എണ്ണം_2);
            എങ്കിൽ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            എങ്കിൽ (എണ്ണം_2 == 0)
                തുടരുക;
            size_t base = devices.size();
            devices.resize(base + എണ്ണം_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, എണ്ണം_2, &devices[base], &എണ്ണം_2);
            എങ്കിൽ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        വേണ്ടി (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : സത്യം) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = സത്യം;
            എങ്കിൽ(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, വലുപ്പം(isIGPU), &isIGPU, ശൂന്യ));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            എങ്കിൽ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                തിരികെ devices[i];
            }
        }
    }

not_found:
    എങ്കിൽ (configuration.empty())
        തിരികെ ശൂന്യ; // suppress messages on stderr

    std::ostringstream സന്ദേശം;
    സന്ദേശം << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    വേണ്ടി (size_t t = 0; t < deviceTypes.size(); t++)
        സന്ദേശം << ' ' << deviceTypes[t];

    സന്ദേശം << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ശൂന്യ, സന്ദേശം.str());
    തിരികെ ശൂന്യ;
}

#ifdef HAVE_OPENCL_SVM
നാമാകാശ svm {

എണ്ണൽ AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

സ്ഥിതം തർക്കം checkForceSVMUmatUsage()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം force = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ force;
}
സ്ഥിതം തർക്കം checkDisableSVMUMatUsage()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം force = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ force;
}
സ്ഥിതം തർക്കം checkDisableSVM()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം force = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ force;
}
// see SVMCapabilities
സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ getSVMCapabilitiesMask()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം unsigned പൂർണ്ണസംഖ്യ mask = 0;
    എങ്കിൽ (!initialized)
    {
        സ്ഥിരം std::string env_മൂല്യം = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        എങ്കിൽ (env_മൂല്യം.empty())
        {
            തിരികെ ~0U; // all bits 1
        }
        mask = atoi(env_മൂല്യം.c_str());
        initialized = സത്യം;
    }
    തിരികെ mask;
}
} // namespace
#endif

സ്ഥിതം size_t get_Program_എണ്ണം_Limit()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം size_t എണ്ണം_2 = 0;
    എങ്കിൽ (!initialized)
    {
        എണ്ണം_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = സത്യം;
    }
    തിരികെ എണ്ണം_2;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ g_contextId = 0;

വർഗ്ഗം OpenCLBufferPoolImpl;
വർഗ്ഗം OpenCLSVMBufferPoolImpl;

ഘടന Context::Impl
{
    സ്ഥിതം Context::Impl* get(Context& context) { തിരികെ context.getImpl(); }

    തരനിർവചനം std::deque<Context::Impl*> container_t;
    സ്ഥിതം container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        സ്ഥിതം container_t* g_contexts = പുതിയ container_t();
        തിരികെ *g_contexts;
    }

സംരക്ഷിത:
    Impl(സ്ഥിരം std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(അസത്യം)
#endif
    {
        എങ്കിൽ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ഇത്;
    }

    ~Impl()
    {
#ifdef _WIN32
        എങ്കിൽ (!cv::__termination)
#endif
        {
            എങ്കിൽ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ശൂന്യ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ശൂന്യ;
        }
    }

    ശൂന്യം init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_അക്കം_DEVICES, വലുപ്പം(ndevices), &ndevices, ശൂന്യ));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * വലുപ്പം(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * വലുപ്പം(cl_device_id), "");

        devices.clear();
        വേണ്ടി (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    ശൂന്യം __init_buffer_pools();  // w/o synchronization
    ശൂന്യം _init_buffer_pools() സ്ഥിരം
    {
        എങ്കിൽ (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            എങ്കിൽ (!bufferPool_)
            {
                const_cast<Impl*>(ഇത്)->__init_buffer_pools();
            }
        }
    }
പൊതു:
    സ്ഥിതം Impl* findContext(സ്ഥിരം std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        എങ്കിൽ (configuration.empty() && !container.empty())
            തിരികെ container[0];
        വേണ്ടി (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            എങ്കിൽ (i && i->configuration == configuration)
            {
                തിരികെ i;
            }
        }
        തിരികെ ശൂന്യ;
    }

    സ്ഥിതം Impl* findOrCreateContext(സ്ഥിരം std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        എങ്കിൽ (configuration_.empty())
        {
            സ്ഥിരം std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            എങ്കിൽ (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        എങ്കിൽ (impl)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            തിരികെ impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        എങ്കിൽ (d == ശൂന്യ)
            തിരികെ ശൂന്യ;

        impl = പുതിയ Impl(configuration);
        ശ്രമിക്കുക
        {
            impl->createFromDevice(d);
            എങ്കിൽ (impl->handle)
                തിരികെ impl;
            delete impl;
            തിരികെ ശൂന്യ;
        }
        പിടിക്കുക (...)
        {
            delete impl;
            എറിയുക;
        }
    }

    സ്ഥിതം Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (ശൂന്യം*)h);
        Impl* impl = findContext(configuration);
        എങ്കിൽ (impl)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            തിരികെ impl;
        }

        impl = പുതിയ Impl(configuration);
        ശ്രമിക്കുക
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            തിരികെ impl;
        }
        പിടിക്കുക (...)
        {
            delete impl;
            എറിയുക;
        }
    }

    സ്ഥിതം Impl* findOrCreateContext(സ്ഥിരം ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (ശൂന്യം*)d);
        Impl* impl = findContext(configuration);
        എങ്കിൽ (impl)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            തിരികെ impl;
        }

        impl = പുതിയ Impl(configuration);
        ശ്രമിക്കുക
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            തിരികെ impl;
        }
        പിടിക്കുക (...)
        {
            delete impl;
            എറിയുക;
        }
    }

    ശൂന്യം setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        എങ്കിൽ (d == ശൂന്യ)
            തിരികെ;

        createFromDevice(d);
    }

    ശൂന്യം createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ശൂന്യ);

        cl_platform_id pl = ശൂന്യ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, വലുപ്പം(cl_platform_id), &pl, ശൂന്യ));

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

        തർക്കം ok = handle != 0 && status == CL_SUCCESS;
        എങ്കിൽ( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        അല്ലെങ്കിൽ
            handle = ശൂന്യ;
    }

    Program getProg(സ്ഥിരം ProgramSource& src, സ്ഥിരം String& buildflags, String& errmsg);

    ശൂന്യം unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        വേണ്ടി (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              എങ്കിൽ (it != phash.end())
              {
                  എങ്കിൽ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      തിരികെ;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        എങ്കിൽ (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            എങ്കിൽ (prefix.empty())
            {
                CV_Assert(!devices.empty());
                സ്ഥിരം Device& d = devices[0];
                പൂർണ്ണസംഖ്യ bits = d.addressBits();
                എങ്കിൽ (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                വേണ്ടി (size_t i = 0; i < prefix.size(); i++)
                {
                    അക്ഷരം c = prefix[i];
                    എങ്കിൽ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        തിരികെ prefix;
    }

    std::string& getPrefixBase()
    {
        എങ്കിൽ (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            എങ്കിൽ (prefix_base.empty())
            {
                സ്ഥിരം Device& d = devices[0];
                പൂർണ്ണസംഖ്യ bits = d.addressBits();
                എങ്കിൽ (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                വേണ്ടി (size_t i = 0; i < prefix_base.size(); i++)
                {
                    അക്ഷരം c = prefix_base[i];
                    എങ്കിൽ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        തിരികെ prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    സ്ഥിരം പൂർണ്ണസംഖ്യ contextId;  // global unique ID
    സ്ഥിരം std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    തരനിർവചനം std::map<std::string, Program> phash_t;
    phash_t phash;
    തരനിർവചനം std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() സ്ഥിരം
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        തിരികെ *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() സ്ഥിരം
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        തിരികെ *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    ശൂന്യം setUserContext(std::type_index typeId, സ്ഥിരം std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        എങ്കിൽ (it != userContextStorage.end())
            തിരികെ it->second;
        അല്ലെങ്കിൽ
            തിരികെ nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    തർക്കം svmInitialized;
    തർക്കം svmAvailable;
    തർക്കം svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    ശൂന്യം svmInit()
    {
        CV_Assert(handle != ശൂന്യ);
        സ്ഥിരം Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((ശൂന്യം)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, വലുപ്പം(deviceCaps), &deviceCaps, ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            പോകുക noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (പൂർണ്ണസംഖ്യ)deviceCaps);
        CV_Assert(((ശൂന്യം)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.മൂല്യം_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.മൂല്യം_2 &= svm::getSVMCapabilitiesMask();
        എങ്കിൽ (svmCapabilities.മൂല്യം_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            പോകുക noSVM;
        }
        ശ്രമിക്കുക
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            ശൂന്യം* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            എങ്കിൽ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ശ്രമിക്കുക
            {
                തർക്കം error = അസത്യം;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                എങ്കിൽ (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ശൂന്യ, ശൂന്യ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                ശ്രമിക്കുക
                {
                    ((പൂർണ്ണസംഖ്യ*)ptr)[0] = 100;
                }
                പിടിക്കുക (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = സത്യം;
                }
                എങ്കിൽ (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, ശൂന്യ, ശൂന്യ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                എങ്കിൽ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            പിടിക്കുക (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                എറിയുക;
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
        പിടിക്കുക (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ശ്രമിക്കുക
            {
                // Try HSA extension
                String extensions = device.extensions();
                എങ്കിൽ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    പോകുക noSVM;
                }
                cl_platform_id p = ശൂന്യ;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, വലുപ്പം(cl_platform_id), &p, ശൂന്യ));
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
            പിടിക്കുക (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                പോകുക noSVM;
            }
        }

        svmAvailable = സത്യം;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = സത്യം;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        തിരികെ;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = അസത്യം;
        svmEnabled = അസത്യം;
        svmCapabilities.മൂല്യം_2 = 0;
        svmInitialized = സത്യം;
        svmFunctions.fn_clSVMAlloc = ശൂന്യ;
        തിരികെ;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() സ്ഥിരം
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        തിരികെ *bufferPoolSVM_.get();
    }
#endif

    friend വർഗ്ഗം Program;
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
Context::Context(പൂർണ്ണസംഖ്യ dtype)
{
    p = 0;
    create(dtype);
}

ശൂന്യം Context::release()
{
    എങ്കിൽ (p)
    {
        p->release();
        p = ശൂന്യ;
    }
}

തർക്കം Context::create()
{
    release();
    എങ്കിൽ (!haveOpenCL())
        തിരികെ അസത്യം;
    p = Impl::findOrCreateContext(std::string());
    എങ്കിൽ (p && p->handle)
        തിരികെ സത്യം;
    release();
    തിരികെ അസത്യം;
}

// deprecated
തർക്കം Context::create(പൂർണ്ണസംഖ്യ dtype)
{
    എങ്കിൽ( !haveOpenCL() )
        തിരികെ അസത്യം;
    release();
    എങ്കിൽ (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    അല്ലെങ്കിൽ എങ്കിൽ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    അല്ലെങ്കിൽ എങ്കിൽ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    അല്ലെങ്കിൽ
    {
        CV_LOG_ERROR(ശൂന്യ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    എങ്കിൽ (p && !p->handle)
    {
        release();
    }
    തിരികെ p != 0;
}

Context::Context(സ്ഥിരം Context& c)
{
    p = (Impl*)c.p;
    എങ്കിൽ(p)
        p->addref();
}

Context& Context::operator = (സ്ഥിരം Context& c)
{
    Impl* newp = (Impl*)c.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &c) {
        എങ്കിൽ(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    തിരികെ *ഇത്;
}

ശൂന്യം* Context::ptr() സ്ഥിരം
{
    തിരികെ p == ശൂന്യ ? ശൂന്യ : p->handle;
}

size_t Context::ndevices() സ്ഥിരം
{
    തിരികെ p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) സ്ഥിരം
{
    സ്ഥിതം Device dummy;
    തിരികെ !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    എങ്കിൽ (!c.empty())
    {
        auto& ctx = c.getContext();
        തിരികെ ctx;
    }

    സ്ഥിതം Context dummy;
    തിരികെ dummy;
}

Program Context::getProg(സ്ഥിരം ProgramSource& prog,
                         സ്ഥിരം String& buildopts, String& errmsg)
{
    തിരികെ p ? p->getProg(prog, buildopts, errmsg) : Program();
}

ശൂന്യം Context::unloadProg(Program& prog)
{
    എങ്കിൽ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(ശൂന്യം* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    തിരികെ ctx;
}

/* static */
Context Context::fromDevice(സ്ഥിരം ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    തിരികെ ctx;
}

/* static */
Context Context::create(സ്ഥിരം std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    തിരികെ ctx;
}

ശൂന്യം* Context::getOpenCLContextProperty(പൂർണ്ണസംഖ്യ propertyId) സ്ഥിരം
{
    എങ്കിൽ (p == ശൂന്യ)
        തിരികെ nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ശൂന്യ, &size));
    std::vector<cl_context_properties> prop(size / വലുപ്പം(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), ശൂന്യ));
    വേണ്ടി (size_t i = 0; i < prop.size(); i += 2)
    {
        എങ്കിൽ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ശൂന്യ, "OpenCL: found context property=" << propertyId << ") => " << (ശൂന്യം*)prop[i + 1]);
            തിരികെ (ശൂന്യം*)prop[i + 1];
        }
    }
    തിരികെ nullptr;
}

#ifdef HAVE_OPENCL_SVM
തർക്കം Context::useSVM() സ്ഥിരം
{
    Context::Impl* i = p;
    CV_Assert(i);
    എങ്കിൽ (!i->svmInitialized)
        i->svmInit();
    തിരികെ i->svmEnabled;
}
ശൂന്യം Context::setUseSVM(തർക്കം enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    എങ്കിൽ (!i->svmInitialized)
        i->svmInit();
    എങ്കിൽ (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#അല്ലെങ്കിൽ
തർക്കം Context::useSVM() സ്ഥിരം { തിരികെ അസത്യം; }
ശൂന്യം Context::setUseSVM(തർക്കം enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
നാമാകാശ svm {

സ്ഥിരം SVMCapabilities getSVMCapabilitites(സ്ഥിരം ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    എങ്കിൽ (!i->svmInitialized)
        i->svmInit();
    തിരികെ i->svmCapabilities;
}

CV_EXPORTS സ്ഥിരം SVMFunctions* getSVMFunctions(സ്ഥിരം ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ശൂന്യ);
    തിരികെ &i->svmFunctions;
}

CV_EXPORTS തർക്കം useSVM(UMatUsageFlags usageFlags)
{
    എങ്കിൽ (checkForceSVMUmatUsage())
        തിരികെ സത്യം;
    എങ്കിൽ (checkDisableSVMUMatUsage())
        തിരികെ അസത്യം;
    എങ്കിൽ ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        തിരികെ സത്യം;
    തിരികെ അസത്യം; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

ശൂന്യം Context::setUserContext(std::type_index typeId, സ്ഥിരം std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    തിരികെ p->getUserContext(typeId);
}

സ്ഥിതം ശൂന്യം get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<അക്ഷരം> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
ശൂന്യം attachContext(സ്ഥിരം String& platformName, ശൂന്യം* platformID, ശൂന്യം* context, ശൂന്യം* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        സ്ഥിരം std::string& platformName, ശൂന്യം* platformID, ശൂന്യം* context, ശൂന്യം* deviceID
)
{
    എങ്കിൽ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    എങ്കിൽ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    തർക്കം platformAvailable = അസത്യം;

    // check if external platformName contained in list of available platforms in OpenCV
    വേണ്ടി (unsigned പൂർണ്ണസംഖ്യ i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        എങ്കിൽ (platformName == availablePlatformName)
        {
            platformAvailable = സത്യം;
            നിർത്തുക;
        }
    }

    എങ്കിൽ (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    എങ്കിൽ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    തിരികെ ctx;
}

ശൂന്യം initializeContextFromHandle(Context& ctx, ശൂന്യം* _platform, ശൂന്യം* _context, ശൂന്യം* _device)
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

ഘടന Queue::Impl
{
    വരിയിൽ ശൂന്യം __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = അസത്യം;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, വലുപ്പം(cl_command_queue_properties), &props, ശൂന്യ));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, തർക്കം isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(സ്ഥിരം Context& c, സ്ഥിരം Device& d, തർക്കം withProfiling = അസത്യം)
    {
        __init();

        സ്ഥിരം Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        എങ്കിൽ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        എങ്കിൽ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        എങ്കിൽ (!cv::__termination)
#endif
        {
            എങ്കിൽ(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = ശൂന്യ;
            }
        }
    }

    സ്ഥിരം cv::ocl::Queue& getProfilingQueue(സ്ഥിരം cv::ocl::Queue& self)
    {
        എങ്കിൽ (isProfilingQueue_)
            തിരികെ self;

        എങ്കിൽ (profiling_queue_.ptr())
            തിരികെ profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, വലുപ്പം(cl_context), &ctx, ശൂന്യ));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, വലുപ്പം(cl_device_id), &device, ശൂന്യ));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = പുതിയ Impl(q, സത്യം);
        profiling_queue_ = queue;

        തിരികെ profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    തർക്കം isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(സ്ഥിരം Context& c, സ്ഥിരം Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(സ്ഥിരം Queue& q)
{
    p = q.p;
    എങ്കിൽ(p)
        p->addref();
}

Queue& Queue::operator = (സ്ഥിരം Queue& q)
{
    Impl* newp = (Impl*)q.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &q) {
        എങ്കിൽ(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    തിരികെ *ഇത്;
}

Queue::~Queue()
{
    എങ്കിൽ(p)
        p->release();
}

തർക്കം Queue::create(സ്ഥിരം Context& c, സ്ഥിരം Device& d)
{
    എങ്കിൽ(p)
        p->release();
    p = പുതിയ Impl(c, d);
    തിരികെ p->handle != 0;
}

ശൂന്യം Queue::finish()
{
    എങ്കിൽ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

സ്ഥിരം Queue& Queue::getProfilingQueue() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->getProfilingQueue(*ഇത്);
}

ശൂന്യം* Queue::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    എങ്കിൽ (!c.empty())
    {
        auto& q = c.getQueue();
        തിരികെ q;
    }
    സ്ഥിതം Queue dummy;
    തിരികെ dummy;
}

സ്ഥിതം cl_command_queue getQueue(സ്ഥിരം Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    എങ്കിൽ(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    തിരികെ qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(പൂർണ്ണസംഖ്യ _flags, UMat* _m, പൂർണ്ണസംഖ്യ _wscale, പൂർണ്ണസംഖ്യ _iwscale, സ്ഥിരം ശൂന്യം* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ശൂന്യ);
}

KernelArg KernelArg::Constant(സ്ഥിരം Mat& m)
{
    CV_Assert(m.isContinuous());
    തിരികെ KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ഘടന Kernel::Impl
{
    Impl(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം Program& prog) :
        refcount(1), handle(ശൂന്യ), isInProgress(അസത്യം), isAsyncRun(അസത്യം), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        എങ്കിൽ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = അസത്യം;
        haveTempSrcUMats = അസത്യം;
    }

    ശൂന്യം cleanupUMats()
    {
        തർക്കം exceptionOccurred = അസത്യം;
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < MAX_ARRS; i++ )
        {
            എങ്കിൽ( u[i] )
            {
                എങ്കിൽ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    ശ്രമിക്കുക
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    പിടിക്കുക(സ്ഥിരം std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ശൂന്യ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = സത്യം;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = അസത്യം;
        haveTempSrcUMats = അസത്യം;
        CV_Assert(!exceptionOccurred);
    }

    ശൂന്യം addUMat(സ്ഥിരം UMat& m, തർക്കം dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        എങ്കിൽ(dst && m.u->tempUMat())
            haveTempDstUMats = സത്യം;
        എങ്കിൽ(m.u->originalUMatData == ശൂന്യ && m.u->tempUMat())
            haveTempSrcUMats = സത്യം;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    ശൂന്യം registerImageArgument(പൂർണ്ണസംഖ്യ arg, സ്ഥിരം Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        എങ്കിൽ (arg < (പൂർണ്ണസംഖ്യ)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    ശൂന്യം finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = അസത്യം;
        ശ്രമിക്കുക
        {
            cleanupUMats();
        }
        പിടിക്കുക(...)
        {
            release();
            എറിയുക;
        }
        release();
    }

    തർക്കം run(പൂർണ്ണസംഖ്യ dims, size_t _globalsize[], size_t _localsize[],
            തർക്കം സമന്വയം, int64* timeNS, സ്ഥിരം Queue& q);

    ~Impl()
    {
        എങ്കിൽ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    എണ്ണൽ { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    തർക്കം isInProgress;
    തർക്കം isAsyncRun;  // true if kernel was scheduled in async mode
    പൂർണ്ണസംഖ്യ nu;
    std::vector<Image2D> shadow_images;
    തർക്കം haveTempDstUMats;
    തർക്കം haveTempSrcUMats;
};

}} // namespace cv::ocl

ബാഹ്യ "C" {

സ്ഥിതം ശൂന്യം CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, ശൂന്യം *p)
{
    ശ്രമിക്കുക
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    പിടിക്കുക (സ്ഥിരം cv::Exception& exc)
    {
        CV_LOG_ERROR(ശൂന്യ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    പിടിക്കുക (സ്ഥിരം std::exception& exc)
    {
        CV_LOG_ERROR(ശൂന്യ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    പിടിക്കുക (...)
    {
        CV_LOG_ERROR(ശൂന്യ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

നാമാകാശ cv { നാമാകാശ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം ProgramSource& src,
               സ്ഥിരം String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(സ്ഥിരം Kernel& k)
{
    p = k.p;
    എങ്കിൽ(p)
        p->addref();
}

Kernel& Kernel::operator = (സ്ഥിരം Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &k) {
        എങ്കിൽ(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    തിരികെ *ഇത്;
}

Kernel::~Kernel()
{
    എങ്കിൽ(p)
        p->release();
}

തർക്കം Kernel::create(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം Program& prog)
{
    എങ്കിൽ(p)
        p->release();
    p = പുതിയ Impl(kname, prog);
    എങ്കിൽ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    തിരികെ p != 0;
}

തർക്കം Kernel::create(സ്ഥിരം അക്ഷരം* kname, സ്ഥിരം ProgramSource& src,
                    സ്ഥിരം String& buildopts, String* errmsg)
{
    എങ്കിൽ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    എങ്കിൽ( !errmsg ) errmsg = &tempmsg;
    സ്ഥിരം Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    തിരികെ create(kname, prog);
}

ശൂന്യം* Kernel::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

തർക്കം Kernel::empty() സ്ഥിരം
{
    തിരികെ ptr() == 0;
}

സ്ഥിതം cv::String dump_മൂല്യം(size_t sz, സ്ഥിരം ശൂന്യം* p)
{
    എങ്കിൽ (!p)
        തിരികെ "NULL";
    എങ്കിൽ (sz == 2)
        തിരികെ cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    എങ്കിൽ (sz == 4)
        തിരികെ cv::format("%d / %uu / 0x%08x / %g", *(പൂർണ്ണസംഖ്യ*)p, *(പൂർണ്ണസംഖ്യ*)p, *(പൂർണ്ണസംഖ്യ*)p, *(ദശാംശം*)p);
    എങ്കിൽ (sz == 8)
        തിരികെ cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(ഇരട്ട*)p);
    തിരികെ cv::format("%p", p);
}

പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം ശൂന്യം* value, size_t sz)
{
    എങ്കിൽ (!p || !p->handle)
        തിരികെ -1;
    എങ്കിൽ (i < 0)
        തിരികെ i;
    എങ്കിൽ( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (പൂർണ്ണസംഖ്യ)sz, dump_മൂല്യം(sz, value).c_str()).c_str());
    എങ്കിൽ (retval != CL_SUCCESS)
        തിരികെ -1;
    തിരികെ i+1;
}

പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    പൂർണ്ണസംഖ്യ res = set(i, &h, വലുപ്പം(h));
    എങ്കിൽ (res >= 0)
        p->registerImageArgument(i, image2D);
    തിരികെ res;
}

പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം UMat& m)
{
    തിരികെ set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
സ്ഥിതം തർക്കം isCudaBacked(സ്ഥിരം UMatData* u)
{
    സ്ഥിരം MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    തിരികെ u && cudaAllocator && u->currAllocator == cudaAllocator;
}

പൂർണ്ണസംഖ്യ Kernel::set(പൂർണ്ണസംഖ്യ i, സ്ഥിരം KernelArg& arg)
{
    എങ്കിൽ( !p || !p->handle )
        തിരികെ -1;
    എങ്കിൽ (i < 0)
    {
        CV_LOG_ERROR(ശൂന്യ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (പൂർണ്ണസംഖ്യ)i));
        തിരികെ i;
    }
    എങ്കിൽ( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    എങ്കിൽ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        തർക്കം ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        എങ്കിൽ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ശൂന്യ;
            status = clSetKernelArg(p->handle, (cl_uint)i, വലുപ്പം(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i).c_str());
            തിരികെ i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        എങ്കിൽ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ശൂന്യ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (പൂർണ്ണസംഖ്യ)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            തിരികെ -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        എങ്കിൽ (!h)
        {
            CV_LOG_ERROR(ശൂന്യ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (പൂർണ്ണസംഖ്യ)arg.flags, arg.m));
            p->release();
            p = 0;
            തിരികെ -1;
        }

#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            സ്ഥിരം Context& ctx = Context::getDefault();
            സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#എങ്കിൽ 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#അല്ലെങ്കിൽ
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (ശൂന്യം*)svmDataPtr).c_str());
        }
        അല്ലെങ്കിൽ
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, വലുപ്പം(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (ശൂന്യം*)h).c_str());
        }

        എങ്കിൽ (ptronly)
        {
            i++;
        }
        അല്ലെങ്കിൽ എങ്കിൽ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), വലുപ്പം(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+1), (പൂർണ്ണസംഖ്യ)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), വലുപ്പം(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+2), (പൂർണ്ണസംഖ്യ)u2d.offset).c_str());
            i += 3;

            എങ്കിൽ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                പൂർണ്ണസംഖ്യ cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, വലുപ്പം(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (പൂർണ്ണസംഖ്യ)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), വലുപ്പം(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+1), (പൂർണ്ണസംഖ്യ)cols).c_str());
                i += 2;
            }
        }
        അല്ലെങ്കിൽ
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), വലുപ്പം(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+1), (പൂർണ്ണസംഖ്യ)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), വലുപ്പം(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+2), (പൂർണ്ണസംഖ്യ)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), വലുപ്പം(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+3), (പൂർണ്ണസംഖ്യ)u3d.offset).c_str());
            i += 4;
            എങ്കിൽ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                പൂർണ്ണസംഖ്യ cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, വലുപ്പം(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (പൂർണ്ണസംഖ്യ)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), വലുപ്പം(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+1), (പൂർണ്ണസംഖ്യ)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), വലുപ്പം(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)(i+2), (പൂർണ്ണസംഖ്യ)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        തിരികെ i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (പൂർണ്ണസംഖ്യ)i, (പൂർണ്ണസംഖ്യ)arg.sz, (ശൂന്യം*)arg.obj).c_str());
    തിരികെ i+1;
}

തർക്കം Kernel::run(പൂർണ്ണസംഖ്യ dims, size_t _globalsize[], size_t _localsize[],
                 തർക്കം സമന്വയം, സ്ഥിരം Queue& q)
{
    എങ്കിൽ (!p)
        തിരികെ അസത്യം;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != ശൂന്യ);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(പൂർണ്ണസംഖ്യ)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        എങ്കിൽ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned പൂർണ്ണസംഖ്യ)val) * val;
    }
    CV_Assert(total > 0);

    തിരികെ p->run(dims, globalsize, _localsize, സമന്വയം, ശൂന്യ, q);
}


തർക്കം Kernel::run_(പൂർണ്ണസംഖ്യ dims, size_t _globalsize[], size_t _localsize[],
                  തർക്കം സമന്വയം, സ്ഥിരം Queue& q)
{
    CV_Assert(p);
    തിരികെ p->run(dims, _globalsize, _localsize, സമന്വയം, ശൂന്യ, q);
}


തർക്കം Kernel::Impl::run(പൂർണ്ണസംഖ്യ dims, size_t globalsize[], size_t localsize[],
        തർക്കം സമന്വയം, int64* timeNS, സ്ഥിരം Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    എങ്കിൽ (!handle)
    {
        CV_LOG_ERROR(ശൂന്യ, "OpenCL kernel has zero handle: " << name);
        തിരികെ അസത്യം;
    }

    എങ്കിൽ (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !സമന്വയം;

    എങ്കിൽ (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#എങ്കിൽ CV_OPENCL_SYNC_RUN_KERNELS
    സമന്വയം = സത്യം;
#endif

    cl_command_queue qq = getQueue(q);
    എങ്കിൽ (haveTempDstUMats)
        സമന്വയം = സത്യം;
    എങ്കിൽ (haveTempSrcUMats)
        സമന്വയം = സത്യം;
    എങ്കിൽ (timeNS)
        സമന്വയം = സത്യം;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           ശൂന്യ, globalsize, localsize, 0, 0,
                                           (സമന്വയം && !timeNS) ? 0 : &asyncEvent);
#എങ്കിൽ !CV_OPENCL_SHOW_RUN_KERNELS
    എങ്കിൽ (retval != CL_SUCCESS)
#endif
    {
        cv::String സന്ദേശം = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (പൂർണ്ണസംഖ്യ)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        സമന്വയം ? "true" : "false"
                        );
        എങ്കിൽ (retval != CL_SUCCESS)
        {
            സന്ദേശം = CV_OCL_API_ERROR_സന്ദേശം(retval, സന്ദേശം.c_str());
        }
#എങ്കിൽ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, സന്ദേശം.c_str());
#അല്ലെങ്കിൽ
        printf("%s\n", സന്ദേശം.c_str());
        fflush(stdout);
#endif
    }
    എങ്കിൽ (സമന്വയം || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        എങ്കിൽ (timeNS)
        {
            എങ്കിൽ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, വലുപ്പം(startTime), &startTime, ശൂന്യ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, വലുപ്പം(stopTime), &stopTime, ശൂന്യ));
                *timeNS = (int64)(stopTime - startTime);
            }
            അല്ലെങ്കിൽ
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    അല്ലെങ്കിൽ
    {
        addref();
        isInProgress = സത്യം;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ഇത്));
    }
    എങ്കിൽ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    തിരികെ retval == CL_SUCCESS;
}

തർക്കം Kernel::runTask(തർക്കം സമന്വയം, സ്ഥിരം Queue& q)
{
    എങ്കിൽ(!p || !p->handle || p->isInProgress)
        തിരികെ അസത്യം;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, സമന്വയം ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), സമന്വയം ? "true" : "false").c_str());
    എങ്കിൽ (സമന്വയം || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    അല്ലെങ്കിൽ
    {
        p->addref();
        p->isInProgress = സത്യം;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    എങ്കിൽ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    തിരികെ retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(പൂർണ്ണസംഖ്യ dims, size_t globalsize[], size_t localsize[], സ്ഥിരം Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    തർക്കം res = p->run(dims, globalsize, localsize, സത്യം, &timeNs, profilingQueue);
    തിരികെ res ? timeNs : -1;
}

size_t Kernel::workGroupSize() സ്ഥിരം
{
    എങ്കിൽ(!p || !p->handle)
        തിരികെ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, വലുപ്പം(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    തിരികെ status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() സ്ഥിരം
{
    എങ്കിൽ(!p || !p->handle)
        തിരികെ 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, വലുപ്പം(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    തിരികെ status == CL_SUCCESS ? val : 0;
}

തർക്കം Kernel::compileWorkGroupSize(size_t wsz[]) സ്ഥിരം
{
    എങ്കിൽ(!p || !p->handle || !wsz)
        തിരികെ 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, വലുപ്പം(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    തിരികെ status == CL_SUCCESS;
}

size_t Kernel::localMemSize() സ്ഥിരം
{
    എങ്കിൽ(!p || !p->handle)
        തിരികെ 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, വലുപ്പം(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    തിരികെ status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ഘടന ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    എണ്ണൽ KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(സ്ഥിരം String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(സ്ഥിരം String& module, സ്ഥിരം String& name, സ്ഥിരം String& codeStr, സ്ഥിരം String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    ശൂന്യം init(എണ്ണൽ KIND kind, സ്ഥിരം String& module, സ്ഥിരം String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = ശൂന്യ;
        sourceSize_ = 0;
        isHashUpdated = അസത്യം;
    }

    ശൂന്യം initFromSource(സ്ഥിരം String& codeStr, സ്ഥിരം String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        എങ്കിൽ (sourceHash_.empty())
        {
            updateHash();
        }
        അല്ലെങ്കിൽ
        {
            isHashUpdated = സത്യം;
        }
    }

    ശൂന്യം updateHash(സ്ഥിരം അക്ഷരം* hashStr = ശൂന്യ)
    {
        എങ്കിൽ (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = സത്യം;
            തിരികെ;
        }
        uint64 hash = 0;
        തിരഞ്ഞെടുപ്പ് (kind_)
        {
        അവസ്ഥ PROGRAM_SOURCE_CODE:
            എങ്കിൽ (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            അല്ലെങ്കിൽ
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            നിർത്തുക;
        അവസ്ഥ PROGRAM_BINARIES:
        അവസ്ഥ PROGRAM_SPIR:
        അവസ്ഥ PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = സത്യം;
    }

    Impl(എണ്ണൽ KIND kind,
            സ്ഥിരം String& module, സ്ഥിരം String& name,
            സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size,
            സ്ഥിരം cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    സ്ഥിതം ProgramSource fromSourceWithStaticLifetime(സ്ഥിരം String& module, സ്ഥിരം String& name,
            സ്ഥിരം അക്ഷരം* sourceCodeStaticStr, സ്ഥിരം അക്ഷരം* hashStaticStr,
            സ്ഥിരം cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = പുതിയ Impl(PROGRAM_SOURCE_CODE, module, name,
                (സ്ഥിരം unsigned അക്ഷരം*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        തിരികെ result;
    }

    സ്ഥിതം ProgramSource from_ദ്വിമാന(സ്ഥിരം String& module, സ്ഥിരം String& name,
            സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size,
            സ്ഥിരം cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = പുതിയ Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        തിരികെ result;
    }

    സ്ഥിതം ProgramSource fromSPIR(സ്ഥിരം String& module, സ്ഥിരം String& name,
            സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size,
            സ്ഥിരം cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = പുതിയ Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        തിരികെ result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    സ്ഥിരം unsigned അക്ഷരം* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    തർക്കം isHashUpdated;

    friend ഘടന Program::Impl;
    friend ഘടന internal::ProgramEntry;
    friend ഘടന Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(സ്ഥിരം String& module, സ്ഥിരം String& name, സ്ഥിരം String& codeStr, സ്ഥിരം String& codeHash)
{
    p = പുതിയ Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(സ്ഥിരം അക്ഷരം* prog)
{
    p = പുതിയ Impl(prog);
}

ProgramSource::ProgramSource(സ്ഥിരം String& prog)
{
    p = പുതിയ Impl(prog);
}

ProgramSource::~ProgramSource()
{
    എങ്കിൽ(p)
        p->release();
}

ProgramSource::ProgramSource(സ്ഥിരം ProgramSource& prog)
{
    p = prog.p;
    എങ്കിൽ(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (സ്ഥിരം ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &prog) {
        എങ്കിൽ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    തിരികെ *ഇത്;
}

സ്ഥിരം String& ProgramSource::source() സ്ഥിരം
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == ശൂന്യ); // method returns reference - can't construct temporary object
    തിരികെ p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() സ്ഥിരം
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_ദ്വിമാന(സ്ഥിരം String& module, സ്ഥിരം String& name,
        സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size,
        സ്ഥിരം cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    തിരികെ Impl::from_ദ്വിമാന(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(സ്ഥിരം String& module, സ്ഥിരം String& name,
        സ്ഥിരം unsigned അക്ഷരം* binary, സ്ഥിരം size_t size,
        സ്ഥിരം cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    തിരികെ Impl::from_ദ്വിമാന(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() സ്ഥിരം
{
    എങ്കിൽ (ഇത്->pProgramSource == ശൂന്യ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        എങ്കിൽ (ഇത്->pProgramSource == ശൂന്യ)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(ഇത്->module, ഇത്->name, ഇത്->programCode, ഇത്->programHash, cv::String());
            ProgramSource* ptr = പുതിയ ProgramSource(ps);
            const_cast<ProgramEntry*>(ഇത്)->pProgramSource = ptr;
        }
    }
    തിരികെ *ഇത്->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

സ്ഥിതം
cv::String joinBuildOptions(സ്ഥിരം cv::String& a, സ്ഥിരം cv::String& b)
{
    എങ്കിൽ (b.empty())
        തിരികെ a;
    എങ്കിൽ (a.empty())
        തിരികെ b;
    എങ്കിൽ (b[0] == ' ')
        തിരികെ a + b;
    തിരികെ a + (cv::String(" ") + b);
}

ഘടന Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(സ്ഥിരം ProgramSource& src,
         സ്ഥിരം String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ശൂന്യ),
         buildflags(_buildflags)
    {
        സ്ഥിരം ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        സ്ഥിരം Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        എങ്കിൽ (ctx.ptr() == ശൂന്യ || device.ptr() == ശൂന്യ)
            തിരികെ;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        എങ്കിൽ (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            എങ്കിൽ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            അല്ലെങ്കിൽ എങ്കിൽ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            സ്ഥിരം String param_buildExtraOptions = getBuildExtraOptions();
            എങ്കിൽ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#എങ്കിൽ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ശൂന്യ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#എങ്കിൽ CV_OPENCL_SHOW_BUILD_KERNELS
        എങ്കിൽ (handle)
        {
            size_t retsz = 0;
            അക്ഷരം kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, വലുപ്പം(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            എങ്കിൽ (result == CL_SUCCESS && retsz < വലുപ്പം(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(ശൂന്യ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_ERROR(ശൂന്യ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    തർക്കം compile(സ്ഥിരം Context& ctx, സ്ഥിരം ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        എങ്കിൽ (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(ശൂന്യ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            തർക്കം isLoaded = create_From_ദ്വിമാന(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            തിരികെ isLoaded;
        }
        തിരികെ compileWithCache(ctx, src_, errmsg);
    }

    തർക്കം compileWithCache(സ്ഥിരം Context& ctx, സ്ഥിരം ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        സ്ഥിരം std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        സ്ഥിരം String& hash_str = src_->sourceHash_;
        cv::String fname;
        എങ്കിൽ (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        സ്ഥിരം cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        എങ്കിൽ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ശ്രമിക്കുക
            {
                std::vector<അക്ഷരം> ദ്വിമാന_Buf;
                തർക്കം res = അസത്യം;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ദ്വിമാന_Program_File file(fname, hash_str.c_str());
                    res = file.പഠ(buildflags, ദ്വിമാന_Buf);
                }
                എങ്കിൽ (res)
                {
                    CV_Assert(!ദ്വിമാന_Buf.empty());
                    CV_LOG_VERBOSE(ശൂന്യ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    തർക്കം isLoaded = create_From_ദ്വിമാന(ctx, ദ്വിമാന_Buf, errmsg);
                    എങ്കിൽ (isLoaded)
                        തിരികെ സത്യം;
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(ശൂന്യ, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            പിടിക്കുക (...)
            {
                CV_LOG_VERBOSE(ശൂന്യ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ശൂന്യ);
        എങ്കിൽ (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            എങ്കിൽ (!buildFromSources(ctx, src_, errmsg))
            {
                തിരികെ അസത്യം;
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            എങ്കിൽ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ശൂന്യ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            തർക്കം isLoaded = create_From_ദ്വിമാന(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            എങ്കിൽ (!isLoaded)
                തിരികെ അസത്യം;
        }
        അല്ലെങ്കിൽ എങ്കിൽ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        അല്ലെങ്കിൽ
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != ശൂന്യ);
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
        എങ്കിൽ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ശ്രമിക്കുക
            {
                std::vector<അക്ഷരം> ദ്വിമാന_Buf;
                get_Program_ദ്വിമാന(ദ്വിമാന_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    ദ്വിമാന_Program_File file(fname, hash_str.c_str());
                    file.ലിഖ(buildflags, ദ്വിമാന_Buf);
                }
            }
            പിടിക്കുക (സ്ഥിരം cv::Exception& e)
            {
                CV_LOG_WARNING(ശൂന്യ, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            പിടിക്കുക (...)
            {
                CV_LOG_WARNING(ശൂന്യ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#എങ്കിൽ CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS
        എങ്കിൽ (CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS_മൂല്യം)
        {
            std::vector<അക്ഷരം> ദ്വിമാന_Buf;
            get_Program_ദ്വിമാന(ദ്വിമാന_Buf);
            എങ്കിൽ (!ദ്വിമാന_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ശൂന്യ;
                create_From_ദ്വിമാന(ctx, ദ്വിമാന_Buf, errmsg);
            }
        }
#endif
        തിരികെ handle != ശൂന്യ;
    }

    ശൂന്യം dumpBuildLog_(cl_int result, സ്ഥിരം cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<അക്ഷരം, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        എങ്കിൽ (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            എങ്കിൽ (log_retval == CL_SUCCESS)
            {
                എങ്കിൽ (retsz < buffer.size())
                    buffer[retsz] = 0;
                അല്ലെങ്കിൽ
                    buffer[buffer.size() - 1] = 0;
            }
            അല്ലെങ്കിൽ
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

    തർക്കം buildFromSources(സ്ഥിരം Context& ctx, സ്ഥിരം ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == ശൂന്യ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ശൂന്യ, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        സ്ഥിരം അക്ഷരം* srcptr = src_->sourceAddr_ ? ((സ്ഥിരം അക്ഷരം*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != ശൂന്യ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        എങ്കിൽ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            വേണ്ടി (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#എങ്കിൽ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            എങ്കിൽ (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                എങ്കിൽ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ശൂന്യ;
                }
                എങ്കിൽ (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#എങ്കിൽ CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS
            എങ്കിൽ (handle && CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS_മൂല്യം)
            {
                CV_LOG_INFO(ശൂന്യ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                അക്ഷരം kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, വലുപ്പം(kernels_buffer), &kernels_buffer[0], &retsz);
                എങ്കിൽ (retsz < വലുപ്പം(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                അല്ലെങ്കിൽ
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(ശൂന്യ, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        തിരികെ handle != ശൂന്യ;
    }

    ശൂന്യം get_Program_ദ്വിമാന(std::vector<അക്ഷരം>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_ദ്വിമാന_SIZES, വലുപ്പം(sz), &sz, ശൂന്യ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, വലുപ്പം(ptr), &ptr, ശൂന്യ));
    }

    തർക്കം create_From_ദ്വിമാന(സ്ഥിരം Context& ctx, സ്ഥിരം std::vector<അക്ഷരം>& buf, String& errmsg)
    {
        തിരികെ create_From_ദ്വിമാന(ctx, (സ്ഥിരം unsigned അക്ഷരം*)&buf[0], buf.size(), errmsg);
    }

    തർക്കം create_From_ദ്വിമാന(സ്ഥിരം Context& ctx, സ്ഥിരം unsigned അക്ഷരം* ദ്വിമാന_Addr, സ്ഥിരം size_t ദ്വിമാന_Size, String& errmsg)
    {
        CV_Assert(handle == ശൂന്യ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ശൂന്യ, 0, "Load from binary... (" << ദ്വിമാന_Size << " bytes)");

        CV_Assert(ദ്വിമാന_Size > 0);

        size_t ndevices = (പൂർണ്ണസംഖ്യ)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<സ്ഥിരം uchar*> ദ്വിമാന_Ptrs(ndevices);
        AutoBuffer<size_t> ദ്വിമാന_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        സ്ഥിരം uchar** ദ്വിമാന_Ptrs_2 = ദ്വിമാന_Ptrs.data();
        size_t* ദ്വിമാന_Sizes_2 = ദ്വിമാന_Sizes.data();
        വേണ്ടി (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ദ്വിമാന_Ptrs_2[i] = ദ്വിമാന_Addr;
            ദ്വിമാന_Sizes_2[i] = ദ്വിമാന_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_ദ്വിമാന((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           ദ്വിമാന_Sizes_2, ദ്വിമാന_Ptrs_2, ശൂന്യ, &result);
        എങ്കിൽ (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(ശൂന്യ, CV_OCL_API_ERROR_സന്ദേശം(result, "clCreateProgramWithBinary"));
            എങ്കിൽ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ശൂന്യ;
            }
        }
        എങ്കിൽ (!handle)
        {
            തിരികെ അസത്യം;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            എങ്കിൽ (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                എങ്കിൽ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ശൂന്യ;
                }
                തിരികെ അസത്യം;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    വലുപ്പം(build_status), &build_status, &retsz));
            എങ്കിൽ (result == CL_SUCCESS)
            {
                എങ്കിൽ (build_status == CL_BUILD_SUCCESS)
                {
                    തിരികെ സത്യം;
                }
                അല്ലെങ്കിൽ
                {
                    CV_LOG_WARNING(ശൂന്യ, "clGetProgramBuildInfo() returns " << build_status);
                    തിരികെ അസത്യം;
                }
            }
            അല്ലെങ്കിൽ
            {
                CV_LOG_ERROR(ശൂന്യ, CV_OCL_API_ERROR_സന്ദേശം(result, "clGetProgramBuildInfo()"));
                എങ്കിൽ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ശൂന്യ;
                }
            }
        }
#എങ്കിൽ CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS
        എങ്കിൽ (handle && CV_OPENCL_VALIDATE_ദ്വിമാന_PROGRAMS_മൂല്യം)
        {
            CV_LOG_INFO(ശൂന്യ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            അക്ഷരം kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, വലുപ്പം(kernels_buffer), &kernels_buffer[0], &retsz);
            എങ്കിൽ (retsz < വലുപ്പം(kernels_buffer))
                kernels_buffer[retsz] = 0;
            അല്ലെങ്കിൽ
                kernels_buffer[0] = 0;
            CV_LOG_INFO(ശൂന്യ, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        തിരികെ handle != ശൂന്യ;
    }

    ~Impl()
    {
        എങ്കിൽ( handle )
        {
#ifdef _WIN32
            എങ്കിൽ (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = ശൂന്യ;
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

Program::Program(സ്ഥിരം ProgramSource& src,
        സ്ഥിരം String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(സ്ഥിരം Program& prog)
{
    p = prog.p;
    എങ്കിൽ(p)
        p->addref();
}

Program& Program::operator = (സ്ഥിരം Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    എങ്കിൽ(newp)
        newp->addref();
    എങ്കിൽ(p)
        p->release();
    p = newp;
    തിരികെ *ഇത്;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &prog) {
        എങ്കിൽ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    തിരികെ *ഇത്;
}

Program::~Program()
{
    എങ്കിൽ(p)
        p->release();
}

തർക്കം Program::create(സ്ഥിരം ProgramSource& src,
            സ്ഥിരം String& buildflags, String& errmsg)
{
    എങ്കിൽ(p)
    {
        p->release();
        p = ശൂന്യ;
    }
    p = പുതിയ Impl(src, buildflags, errmsg);
    എങ്കിൽ(!p->handle)
    {
        p->release();
        p = 0;
    }
    തിരികെ p != 0;
}

ശൂന്യം* Program::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
സ്ഥിരം ProgramSource& Program::source() സ്ഥിരം
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

തർക്കം Program::പഠ(സ്ഥിരം String& bin, സ്ഥിരം String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

തർക്കം Program::ലിഖ(String& bin) സ്ഥിരം
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() സ്ഥിരം
{
    എങ്കിൽ(!p)
        തിരികെ String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    തിരികെ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(സ്ഥിരം String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        തിരികെ cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

ശൂന്യം Program::get_ദ്വിമാന(std::vector<അക്ഷരം>& binary) സ്ഥിരം
{
    CV_Assert(p && "Empty program");
    p->get_Program_ദ്വിമാന(binary);
}

Program Context::Impl::getProg(സ്ഥിരം ProgramSource& src,
                               സ്ഥിരം String& buildflags, String& errmsg)
{
    size_t limit = get_Program_എണ്ണം_Limit();
    സ്ഥിരം ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String താക്കോൽ = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(താക്കോൽ);
        എങ്കിൽ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), താക്കോൽ);
            എങ്കിൽ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(താക്കോൽ);
            }
            തിരികെ it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            എങ്കിൽ (limit > 0 && sz >= limit)
            {
                സ്ഥിതം തർക്കം warningFlag = അസത്യം;
                എങ്കിൽ (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = സത്യം;
                }
                ആകുമ്പോൾ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    എങ്കിൽ (c != 0)
                        നിർത്തുക;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(താക്കോൽ, prog));
        cacheList.push_front(താക്കോൽ);
    }
    തിരികെ prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
വർഗ്ഗം OpenCLBufferPool
{
സംരക്ഷിത:
    ~OpenCLBufferPool() { }
പൊതു:
    മായാ T allocate(size_t size) = 0;
    മായാ ശൂന്യം release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
വർഗ്ഗം OpenCLBufferPoolBaseImpl : പൊതു BufferPoolController, പൊതു OpenCLBufferPool<T>
{
സ്വകാര്യ:
    വരിയിൽ Derived& derived() { തിരികെ *static_cast<Derived*>(ഇത്); }
സംരക്ഷിത:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    തർക്കം _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        വേണ്ടി (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            എങ്കിൽ (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                തിരികെ സത്യം;
            }
        }
        തിരികെ അസത്യം;
    }

    // synchronized
    തർക്കം _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, സ്ഥിരം size_t size)
    {
        എങ്കിൽ (reservedEntries_.empty())
            തിരികെ അസത്യം;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        വേണ്ടി (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            എങ്കിൽ (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                എങ്കിൽ (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    എങ്കിൽ (diff == 0)
                        നിർത്തുക;
                }
            }
        }
        എങ്കിൽ (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            തിരികെ സത്യം;
        }
        തിരികെ അസത്യം;
    }

    // synchronized
    ശൂന്യം _checkSizeOfReservedEntries()
    {
        ആകുമ്പോൾ (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            സ്ഥിരം BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    വരിയിൽ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        എങ്കിൽ (size < 1024*1024)
            തിരികെ 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        അല്ലെങ്കിൽ എങ്കിൽ (size < 16*1024*1024)
            തിരികെ 64*1024;
        അല്ലെങ്കിൽ
            തിരികെ 1024*1024;
    }

പൊതു:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    മായാ ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
പൊതു:
    മായാ T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        എങ്കിൽ (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        അല്ലെങ്കിൽ
        {
            derived()._allocateBufferEntry(entry, size);
        }
        തിരികെ entry.clBuffer_;
    }
    മായാ ശൂന്യം release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        എങ്കിൽ (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        അല്ലെങ്കിൽ
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    മായാ size_t getReservedSize() സ്ഥിരം CV_OVERRIDE { തിരികെ currentReservedSize; }
    മായാ size_t getMaxReservedSize() സ്ഥിരം CV_OVERRIDE { തിരികെ maxReservedSize; }
    മായാ ശൂന്യം setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        എങ്കിൽ (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            വേണ്ടി (; i != reservedEntries_.end();)
            {
                സ്ഥിരം BufferEntry& entry = *i;
                എങ്കിൽ (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    തുടരുക;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    മായാ ശൂന്യം freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        വേണ്ടി (; i != reservedEntries_.end(); ++i)
        {
            സ്ഥിരം BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

ഘടന CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)ശൂന്യ), capacity_(0) { }
};

വർഗ്ഗം OpenCLBufferPoolImpl CV_FINAL : പൊതു OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
പൊതു:
    തരനിർവചനം ഘടന CLBufferEntry BufferEntry;
സംരക്ഷിത:
    പൂർണ്ണസംഖ്യ createFlags_;
പൊതു:
    OpenCLBufferPoolImpl(പൂർണ്ണസംഖ്യ createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    ശൂന്യം _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ശൂന്യ);
        entry.capacity_ = alignSize(size, (പൂർണ്ണസംഖ്യ)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long പൂർണ്ണസംഖ്യ)entry.capacity_, (ശൂന്യം*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != ശൂന്യ);
        എങ്കിൽ(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ശൂന്യം _releaseBufferEntry(സ്ഥിരം BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ശൂന്യ);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
ഘടന CLSVMBufferEntry
{
    ശൂന്യം* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(ശൂന്യ), capacity_(0) { }
};
വർഗ്ഗം OpenCLSVMBufferPoolImpl CV_FINAL : പൊതു OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, ശൂന്യം*>
{
പൊതു:
    തരനിർവചനം ഘടന CLSVMBufferEntry BufferEntry;
പൊതു:
    OpenCLSVMBufferPoolImpl()
    {
    }

    ശൂന്യം _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ശൂന്യ);
        entry.capacity_ = alignSize(size, (പൂർണ്ണസംഖ്യ)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        സ്ഥിരം svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        തർക്കം isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (പൂർണ്ണസംഖ്യ)entry.capacity_);
        ശൂന്യം *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    ശൂന്യം _releaseBufferEntry(സ്ഥിരം BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ശൂന്യ);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <തർക്കം readAccess, തർക്കം writeAccess>
വർഗ്ഗം AlignedDataPtr
{
സംരക്ഷിത:
    സ്ഥിരം size_t size_;
    uchar* സ്ഥിരം originPtr_;
    സ്ഥിരം size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

പൊതു:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ശൂന്യ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        എങ്കിൽ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = പുതിയ uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            എങ്കിൽ (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() സ്ഥിരം
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        തിരികെ ptr_;
    }

    ~AlignedDataPtr()
    {
        എങ്കിൽ (allocatedPtr_)
        {
            എങ്കിൽ (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ശൂന്യ;
        }
        ptr_ = ശൂന്യ;
    }
സ്വകാര്യ:
    AlignedDataPtr(സ്ഥിരം AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(സ്ഥിരം AlignedDataPtr&); // disabled
};

template <തർക്കം readAccess, തർക്കം writeAccess>
വർഗ്ഗം AlignedDataPtr2D
{
സംരക്ഷിത:
    സ്ഥിരം size_t size_;
    uchar* സ്ഥിരം originPtr_;
    സ്ഥിരം size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

പൊതു:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ശൂന്യ), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ശൂന്യ);
        എങ്കിൽ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = പുതിയ uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            എങ്കിൽ (readAccess)
            {
                വേണ്ടി (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() സ്ഥിരം
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        തിരികെ ptr_;
    }

    ~AlignedDataPtr2D()
    {
        എങ്കിൽ (allocatedPtr_)
        {
            എങ്കിൽ (writeAccess)
            {
                വേണ്ടി (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ശൂന്യ;
        }
        ptr_ = ശൂന്യ;
    }
സ്വകാര്യ:
    AlignedDataPtr2D(സ്ഥിരം AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(സ്ഥിരം AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


ശൂന്യം Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(ശൂന്യ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

വർഗ്ഗം OpenCLAllocator CV_FINAL : പൊതു MatAllocator
{
പൊതു:
    എണ്ണൽ AllocatorFlags
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

    UMatData* defaultAllocate(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type, ശൂന്യം* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) സ്ഥിരം
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        തിരികെ u;
    }

    സ്ഥിതം തർക്കം isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        സ്ഥിതം തർക്കം value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", അസത്യം);
        തിരികെ value;
    }
    സ്ഥിതം തർക്കം isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        സ്ഥിതം തർക്കം value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", അസത്യം);
        തിരികെ value;
    }

    ശൂന്യം getBestFlags(സ്ഥിരം Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, പൂർണ്ണസംഖ്യ& createFlags, UMatData::MemoryFlag& flags0) സ്ഥിരം
    {
        സ്ഥിരം Device& dev = ctx.device(0);
        createFlags = 0;
        എങ്കിൽ ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        എങ്കിൽ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        അല്ലെങ്കിൽ
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* sizes, പൂർണ്ണസംഖ്യ type,
                       ശൂന്യം* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!useOpenCL())
            തിരികെ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = dims-1; i >= 0; i-- )
        {
            എങ്കിൽ( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        എങ്കിൽ (!ctx.getImpl())
            തിരികെ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        പൂർണ്ണസംഖ്യ createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        ശൂന്യം* handle = ശൂന്യ;
        പൂർണ്ണസംഖ്യ allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        സ്ഥിരം svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        എങ്കിൽ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            തർക്കം isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        അല്ലെങ്കിൽ
#endif
        എങ്കിൽ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        അല്ലെങ്കിൽ എങ്കിൽ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        അല്ലെങ്കിൽ
        {
            CV_Assert(handle != ശൂന്യ); // Unsupported, throw
        }

        എങ്കിൽ (!handle)
            തിരികെ defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = പുതിയ UMatData(ഇത്);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<ശൂന്യം>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(സത്യം);
        opencl_allocator_stats.onAllocate(u->size);
        തിരികെ u;
    }

    തർക്കം allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ അസത്യം;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        എങ്കിൽ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            പൂർണ്ണസംഖ്യ createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            തർക്കം copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            പൂർണ്ണസംഖ്യ allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            ശൂന്യം* handle = ശൂന്യ;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            തർക്കം useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            എങ്കിൽ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (പൂർണ്ണസംഖ്യ)u->size, handle);
            }
            അല്ലെങ്കിൽ എങ്കിൽ (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                എങ്കിൽ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    തർക്കം isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (പൂർണ്ണസംഖ്യ)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = ശൂന്യ;
                    എങ്കിൽ (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (പൂർണ്ണസംഖ്യ)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ശൂന്യ, ശൂന്യ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    എങ്കിൽ (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, ശൂന്യ, ശൂന്യ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            അല്ലെങ്കിൽ
#endif
            {
                എങ്കിൽ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                എങ്കിൽ (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (പൂർണ്ണസംഖ്യ)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long പൂർണ്ണസംഖ്യ)u->size, u->origdata, (ശൂന്യം*)handle).c_str());
                }
                എങ്കിൽ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long പൂർണ്ണസംഖ്യ)u->size, u->origdata, (ശൂന്യം*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (ശൂന്യം*)handle).c_str());
            എങ്കിൽ(!handle || retval != CL_SUCCESS)
                തിരികെ അസത്യം;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ഇത്;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        എങ്കിൽ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(സത്യം);
        opencl_allocator_stats.onAllocate(u->size);
        തിരികെ സത്യം;
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

    ശൂന്യം deallocate(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        എങ്കിൽ (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        അല്ലെങ്കിൽ
            deallocate_(u);
    }

    ശൂന്യം deallocate_(UMatData* u) സ്ഥിരം
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        എങ്കിൽ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        എങ്കിൽ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            തിരികെ;             // avoid any OpenCL calls
#endif
        എങ്കിൽ(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            എങ്കിൽ (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    എങ്കിൽ( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        തർക്കം isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = ശൂന്യ;
                        എങ്കിൽ (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (പൂർണ്ണസംഖ്യ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ശൂന്യ, ശൂന്യ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        എങ്കിൽ (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ശൂന്യ, ശൂന്യ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    അല്ലെങ്കിൽ
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                അല്ലെങ്കിൽ
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    എങ്കിൽ( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<അസത്യം, സത്യം> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    അല്ലെങ്കിൽ
                    {
                        cl_int retval = 0;
                        എങ്കിൽ (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            ശൂന്യം* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ശൂന്യം*)u->handle, (long long പൂർണ്ണസംഖ്യ)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            എങ്കിൽ (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ശൂന്യം*)u->handle, data, (long long പൂർണ്ണസംഖ്യ)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(അസത്യം);
            }
            അല്ലെങ്കിൽ
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                എങ്കിൽ( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            അല്ലെങ്കിൽ
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (ശൂന്യം*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(സത്യം);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ശൂന്യ;
            എങ്കിൽ(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = ശൂന്യ;
        }
        അല്ലെങ്കിൽ
        {
            CV_Assert(u->origdata == ശൂന്യ);
            എങ്കിൽ(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(സത്യം);
            }
            എങ്കിൽ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            അല്ലെങ്കിൽ എങ്കിൽ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            അല്ലെങ്കിൽ എങ്കിൽ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                അല്ലെങ്കിൽ എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ശൂന്യ, ശൂന്യ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((ശൂന്യം*)u->handle);
            }
#endif
            അല്ലെങ്കിൽ
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(സത്യം);
            delete u;
            u = ശൂന്യ;
        }
        CV_Assert(u == ശൂന്യ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    ശൂന്യം map(UMatData* u, AccessFlag accessFlags) സ്ഥിരം CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        എങ്കിൽ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(സത്യം);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            എങ്കിൽ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (പൂർണ്ണസംഖ്യ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ശൂന്യ, ശൂന്യ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(അസത്യം);
                    u->markDeviceMemMapped(സത്യം);
                    തിരികെ;
                }
#endif

                cl_int retval = CL_SUCCESS;
                എങ്കിൽ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (ശൂന്യം*)u->handle, (long long പൂർണ്ണസംഖ്യ)u->size, u->data).c_str());
                }
                എങ്കിൽ (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(അസത്യം);
                    u->markDeviceMemMapped(സത്യം);
                    തിരികെ;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            എങ്കിൽ(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(സത്യം);
            }
        }

        എങ്കിൽ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<അസത്യം, സത്യം> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ശൂന്യം*)u->handle, (long long പൂർണ്ണസംഖ്യ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(അസത്യം);
        }
    }

    ശൂന്യം unmap(UMatData* u) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        എങ്കിൽ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != ശൂന്യ);
#ifdef HAVE_OPENCL_SVM
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, ശൂന്യ, ശൂന്യ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                എങ്കിൽ (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(അസത്യം);
                u->markHostCopyObsolete(സത്യം);
                തിരികെ;
            }
#endif
            എങ്കിൽ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (ശൂന്യം*)u->handle, u->data, (long long പൂർണ്ണസംഖ്യ)u->size).c_str());
                എങ്കിൽ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(അസത്യം);
                u->data = 0;
                u->markDeviceCopyObsolete(അസത്യം);
                u->markHostCopyObsolete(സത്യം);
            }
        }
        അല്ലെങ്കിൽ എങ്കിൽ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<സത്യം, അസത്യം> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (ശൂന്യം*)u->handle, (long long പൂർണ്ണസംഖ്യ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(അസത്യം);
            u->markHostCopyObsolete(സത്യം);
        }
    }

    തർക്കം checkContinuous(പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                         സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
                         സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) സ്ഥിരം
    {
        തർക്കം iscontinuous = സത്യം;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        വേണ്ടി( പൂർണ്ണസംഖ്യ i = dims-2; i >= 0; i-- )
        {
            എങ്കിൽ( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = അസത്യം;
            total *= sz[i];
            എങ്കിൽ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            എങ്കിൽ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        എങ്കിൽ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            എങ്കിൽ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                എങ്കിൽ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                എങ്കിൽ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            അല്ലെങ്കിൽ
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                എങ്കിൽ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                എങ്കിൽ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        തിരികെ iscontinuous;
    }

    ശൂന്യം download(UMatData* u, ശൂന്യം* dstptr, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                  സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
                  സ്ഥിരം size_t dststep[]) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ;
        UMatDataAutoLock autolock(u);

        എങ്കിൽ( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            തിരികെ;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        തർക്കം iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ശൂന്യ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (പൂർണ്ണസംഖ്യ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ശൂന്യ, ശൂന്യ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            എങ്കിൽ( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            അല്ലെങ്കിൽ
            {
                // This code is from MatAllocator::download()
                പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    എങ്കിൽ( sz[i] == 0 )
                    തിരികെ;
                    എങ്കിൽ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                സ്ഥിരം Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                വേണ്ടി( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ശൂന്യ, ശൂന്യ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        അല്ലെങ്കിൽ
#endif
        {
            എങ്കിൽ( iscontinuous )
            {
                AlignedDataPtr<അസത്യം, സത്യം> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            അല്ലെങ്കിൽ എങ്കിൽ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                സ്ഥിരം size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<അസത്യം, അസത്യം> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                വേണ്ടി( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            അല്ലെങ്കിൽ
            {
                AlignedDataPtr2D<അസത്യം, സത്യം> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    ശൂന്യം upload(UMatData* u, സ്ഥിരം ശൂന്യം* srcptr, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
                സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[],
                സ്ഥിരം size_t srcstep[]) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!u)
            തിരികെ;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        തർക്കം iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        എങ്കിൽ( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(അസത്യം);
            u->markDeviceCopyObsolete(സത്യം);
            തിരികെ;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ശൂന്യ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (പൂർണ്ണസംഖ്യ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ശൂന്യ, ശൂന്യ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            എങ്കിൽ( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            അല്ലെങ്കിൽ
            {
                // This code is from MatAllocator::upload()
                പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    എങ്കിൽ( sz[i] == 0 )
                    തിരികെ;
                    എങ്കിൽ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
                }

                Mat src(dims, isz, CV_8U, (ശൂന്യം*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                സ്ഥിരം Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                വേണ്ടി( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ശൂന്യ, ശൂന്യ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        അല്ലെങ്കിൽ
#endif
        {
            എങ്കിൽ( iscontinuous )
            {
                AlignedDataPtr<സത്യം, അസത്യം> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (ശൂന്യം*)u->handle, (long long പൂർണ്ണസംഖ്യ)dstrawofs, (long long പൂർണ്ണസംഖ്യ)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            അല്ലെങ്കിൽ എങ്കിൽ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                സ്ഥിരം size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<അസത്യം, അസത്യം> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                വേണ്ടി( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            അല്ലെങ്കിൽ
            {
                AlignedDataPtr2D<സത്യം, അസത്യം> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(സത്യം);
#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        അല്ലെങ്കിൽ
#endif
        {
            u->markHostCopyObsolete(സത്യം);
        }
        u->markDeviceCopyObsolete(അസത്യം);
    }

    ശൂന്യം copy(UMatData* src, UMatData* dst, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം size_t sz[],
              സ്ഥിരം size_t srcofs[], സ്ഥിരം size_t srcstep[],
              സ്ഥിരം size_t dstofs[], സ്ഥിരം size_t dststep[], തർക്കം _sync) സ്ഥിരം CV_OVERRIDE
    {
        എങ്കിൽ(!src || !dst)
            തിരികെ;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        തർക്കം iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        എങ്കിൽ( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            തിരികെ;
        }
        എങ്കിൽ( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(അസത്യം);
#ifdef HAVE_OPENCL_SVM
            എങ്കിൽ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            അല്ലെങ്കിൽ
#endif
            {
                dst->markDeviceCopyObsolete(സത്യം);
            }
            തിരികെ;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            എങ്കിൽ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                സ്ഥിരം svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                എങ്കിൽ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (പൂർണ്ണസംഖ്യ)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, ശൂന്യ, ശൂന്യ);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                അല്ലെങ്കിൽ
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    പൂർണ്ണസംഖ്യ isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        എങ്കിൽ( sz[i] == 0 )
                        തിരികെ;
                        എങ്കിൽ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (പൂർണ്ണസംഖ്യ)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    വേണ്ടി( പൂർണ്ണസംഖ്യ i = 0; i < dims; i++ )
                    {
                        എങ്കിൽ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    സ്ഥിരം Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    വേണ്ടി( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            അല്ലെങ്കിൽ
            {
                എങ്കിൽ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                അല്ലെങ്കിൽ
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        അല്ലെങ്കിൽ
#endif
        {
            എങ്കിൽ( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (ശൂന്യം*)src->handle, (ശൂന്യം*)dst->handle, (long long പൂർണ്ണസംഖ്യ)srcrawofs, (long long പൂർണ്ണസംഖ്യ)dstrawofs, (long long പൂർണ്ണസംഖ്യ)total).c_str());
            }
            അല്ലെങ്കിൽ എങ്കിൽ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                സ്ഥിരം size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<അസത്യം, അസത്യം> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<അസത്യം, അസത്യം> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                വേണ്ടി( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            അല്ലെങ്കിൽ
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        എങ്കിൽ (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        അല്ലെങ്കിൽ
#endif
        {
            dst->markHostCopyObsolete(സത്യം);
        }
        dst->markDeviceCopyObsolete(അസത്യം);

        എങ്കിൽ( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(സ്ഥിരം അക്ഷരം* id) സ്ഥിരം CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        എങ്കിൽ (ctx.empty())
            തിരികെ ശൂന്യ;
#ifdef HAVE_OPENCL_SVM
        എങ്കിൽ ((svm::checkForceSVMUmatUsage() && (id == ശൂന്യ || strcmp(id, "OCL") == 0)) || (id != ശൂന്യ && strcmp(id, "SVM") == 0))
        {
            തിരികെ &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        എങ്കിൽ (id != ശൂന്യ && strcmp(id, "HOST_ALLOC") == 0)
        {
            തിരികെ &ctx.getImpl()->getBufferPoolHostPtr();
        }
        എങ്കിൽ (id != ശൂന്യ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        തിരികെ &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    ശൂന്യം flushCleanupQueue() സ്ഥിരം
    {
        // TSAN false positive: see #27638
        എങ്കിൽ (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            വേണ്ടി (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    ശൂന്യം addToCleanupQueue(UMatData* u) സ്ഥിരം
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

സ്ഥിതം OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    സ്ഥിതം OpenCLAllocator* g_allocator = പുതിയ OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    തിരികെ g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


നാമാകാശ cv {

// three funcs below are implemented in umatrix.cpp
ശൂന്യം setSize( UMat& m, പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, സ്ഥിരം size_t* _steps,
              തർക്കം autoSteps = അസത്യം );
ശൂന്യം finalizeHdr(UMat& m);

} // namespace cv


നാമാകാശ cv { നാമാകാശ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
ശൂന്യം convertFromBuffer(ശൂന്യം* cl_mem_buffer, size_t step, പൂർണ്ണസംഖ്യ rows, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ type, UMat& dst)
{
    പൂർണ്ണസംഖ്യ d = 2;
    പൂർണ്ണസംഖ്യ sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, സത്യം);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, വലുപ്പം(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, വലുപ്പം(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((പൂർണ്ണസംഖ്യ)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = പുതിയ UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    തിരികെ;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
ശൂന്യം convertFromImage(ശൂന്യം* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, വലുപ്പം(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, വലുപ്പം(cl_image_format), &fmt, 0));

    പൂർണ്ണസംഖ്യ depth = CV_8U;
    തിരഞ്ഞെടുപ്പ് (fmt.image_channel_data_type)
    {
    അവസ്ഥ CL_UNORM_INT8:
    അവസ്ഥ CL_UNSIGNED_INT8:
        depth = CV_8U;
        നിർത്തുക;

    അവസ്ഥ CL_SNORM_INT8:
    അവസ്ഥ CL_SIGNED_INT8:
        depth = CV_8S;
        നിർത്തുക;

    അവസ്ഥ CL_UNORM_INT16:
    അവസ്ഥ CL_UNSIGNED_INT16:
        depth = CV_16U;
        നിർത്തുക;

    അവസ്ഥ CL_SNORM_INT16:
    അവസ്ഥ CL_SIGNED_INT16:
        depth = CV_16S;
        നിർത്തുക;

    അവസ്ഥ CL_SIGNED_INT32:
        depth = CV_32S;
        നിർത്തുക;

    അവസ്ഥ CL_FLOAT:
        depth = CV_32F;
        നിർത്തുക;

    അവസ്ഥ CL_HALF_FLOAT:
        depth = CV_16F;
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    പൂർണ്ണസംഖ്യ type = CV_8UC1;
    തിരഞ്ഞെടുപ്പ് (fmt.image_channel_order)
    {
    അവസ്ഥ CL_R:
    അവസ്ഥ CL_A:
    അവസ്ഥ CL_INTENSITY:
    അവസ്ഥ CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        നിർത്തുക;

    അവസ്ഥ CL_RG:
    അവസ്ഥ CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        നിർത്തുക;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    അവസ്ഥ CL_RGBA:
    അവസ്ഥ CL_BGRA:
    അവസ്ഥ CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        നിർത്തുക;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, വലുപ്പം(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, വലുപ്പം(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, വലുപ്പം(size_t), &h, 0));

    dst.create((പൂർണ്ണസംഖ്യ)h, (പൂർണ്ണസംഖ്യ)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ശൂന്യ, ശൂന്യ));

    CV_OCL_CHECK(clFinish(q));

    തിരികെ;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

സ്ഥിതം ശൂന്യം getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint അക്കം_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ശൂന്യ, &അക്കം_Devices);
    എങ്കിൽ (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &അക്കം_Devices).c_str());
    }

    എങ്കിൽ (അക്കം_Devices == 0)
    {
        devices.clear();
        തിരികെ;
    }

    devices.resize((size_t)അക്കം_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, അക്കം_Devices, &devices[0], &അക്കം_Devices));
}

ഘടന PlatformInfo::Impl
{
    Impl(ശൂന്യം* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) സ്ഥിരം
    {
        അക്ഷരം buf[1024];
        size_t sz=0;
        തിരികെ clGetPlatformInfo(handle, prop, വലുപ്പം(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < വലുപ്പം(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    പൂർണ്ണസംഖ്യ versionMajor_;
    പൂർണ്ണസംഖ്യ versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(ശൂന്യം* platform_id)
{
    p = പുതിയ Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    എങ്കിൽ(p)
        p->release();
}

PlatformInfo::PlatformInfo(സ്ഥിരം PlatformInfo& i)
{
    എങ്കിൽ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(സ്ഥിരം PlatformInfo& i)
{
    എങ്കിൽ (i.p != p)
    {
        എങ്കിൽ (i.p)
            i.p->addref();
        എങ്കിൽ (p)
            p->release();
        p = i.p;
    }
    തിരികെ *ഇത്;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &i) {
        എങ്കിൽ(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    തിരികെ *ഇത്;
}

പൂർണ്ണസംഖ്യ PlatformInfo::device_സംഖ്യ() സ്ഥിരം
{
    തിരികെ p ? (പൂർണ്ണസംഖ്യ)p->devices.size() : 0;
}

ശൂന്യം PlatformInfo::getDevice(Device& device, പൂർണ്ണസംഖ്യ d) സ്ഥിരം
{
    CV_Assert(p && d < (പൂർണ്ണസംഖ്യ)p->devices.size() );
    എങ്കിൽ(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() സ്ഥിരം
{
    തിരികെ p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() സ്ഥിരം
{
    തിരികെ p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() സ്ഥിരം
{
    തിരികെ p ? p->version_ : String();
}

പൂർണ്ണസംഖ്യ PlatformInfo::versionMajor() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->versionMajor_;
}

പൂർണ്ണസംഖ്യ PlatformInfo::versionMinor() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->versionMinor_;
}

സ്ഥിതം ശൂന്യം getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint അക്കം_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ശൂന്യ, &അക്കം_Platforms));

    എങ്കിൽ (അക്കം_Platforms == 0)
    {
        platforms.clear();
        തിരികെ;
    }

    platforms.resize((size_t)അക്കം_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(അക്കം_Platforms, &platforms[0], &അക്കം_Platforms));
}

ശൂന്യം getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    വേണ്ടി (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((ശൂന്യം*)&platforms[i]) );
}

സ്ഥിരം അക്ഷരം* typeToStr(പൂർണ്ണസംഖ്യ type)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* tab[CV_DEPTH_MAX*16]=
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
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    സ്ഥിരം അക്ഷരം* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    തിരികെ result;
}

സ്ഥിരം അക്ഷരം* memopTypeToStr(പൂർണ്ണസംഖ്യ type)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* tab[CV_DEPTH_MAX*16] =
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
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    സ്ഥിരം അക്ഷരം* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    തിരികെ result;
}

സ്ഥിരം അക്ഷരം* vecopTypeToStr(പൂർണ്ണസംഖ്യ type)
{
    സ്ഥിതം സ്ഥിരം അക്ഷരം* tab[CV_DEPTH_MAX*16] =
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
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    സ്ഥിരം അക്ഷരം* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    തിരികെ result;
}

// Deprecated due to size of buf buffer being unknowable.
സ്ഥിരം അക്ഷരം* convertTypeStr(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, പൂർണ്ണസംഖ്യ cn, അക്ഷരം* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    തിരികെ convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

സ്ഥിരം അക്ഷരം* convertTypeStr(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth, പൂർണ്ണസംഖ്യ cn, അക്ഷരം* buf, size_t buf_size)
{
    എങ്കിൽ( sdepth == ddepth )
        തിരികെ "noconvert";
    സ്ഥിരം അക്ഷരം *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    എങ്കിൽ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    അല്ലെങ്കിൽ എങ്കിൽ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    അല്ലെങ്കിൽ
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    തിരികെ buf;
}

സ്ഥിരം അക്ഷരം* getOpenCLErrorString(പൂർണ്ണസംഖ്യ errorCode)
{
#define CV_OCL_CODE(id) അവസ്ഥ id: തിരികെ #id
#define CV_OCL_CODE_(id, name) അവസ്ഥ id: തിരികെ #name
    തിരഞ്ഞെടുപ്പ് (errorCode)
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
    CV_OCL_CODE(CL_INVALID_മൂല്യം);
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
    CV_OCL_CODE(CL_INVALID_ദ്വിമാന);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_മൂല്യം);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_എണ്ണം);
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
    സ്ഥിരസ്ഥിതി: തിരികെ "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
സ്ഥിതം std::string kerToStr(സ്ഥിരം Mat & k)
{
    പൂർണ്ണസംഖ്യ width = k.cols - 1, depth = k.depth();
    സ്ഥിരം T * സ്ഥിരം data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    എങ്കിൽ (depth <= CV_8S)
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < width; ++i)
            stream << "DIG(" << (പൂർണ്ണസംഖ്യ)data[i] << ")";
        stream << "DIG(" << (പൂർണ്ണസംഖ്യ)data[width] << ")";
    }
    അല്ലെങ്കിൽ എങ്കിൽ (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    അല്ലെങ്കിൽ എങ്കിൽ (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < width; ++i)
            stream << "DIG(" << (ദശാംശം)data[i] << "h)";
        stream << "DIG(" << (ദശാംശം)data[width] << "h)";
    }
    അല്ലെങ്കിൽ
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    തിരികെ stream.str();
}

String kernelToStr(നിവേശം_Array _kernel, പൂർണ്ണസംഖ്യ ddepth, സ്ഥിരം അക്ഷരം * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    പൂർണ്ണസംഖ്യ depth = kernel.depth();
    എങ്കിൽ (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    എങ്കിൽ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    എങ്കിൽ (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    തരനിർവചനം std::string (* func_t)(സ്ഥിരം Mat &);
    സ്ഥിതം സ്ഥിരം func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<അക്ഷരം>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<പൂർണ്ണസംഖ്യ>, kerToStr<ദശാംശം>, kerToStr<ഇരട്ട>, kerToStr<hfloat>,
                                                0, kerToStr<തർക്കം>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    സ്ഥിരം func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    തിരികെ cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    ചെയ്യുക \
    { \
        എങ്കിൽ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            പൂർണ്ണസംഖ്യ ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            എങ്കിൽ (cwidth < ckercn || ckercn <= 0) \
                തിരികെ 1; \
            cols.push_back(cwidth); \
            എങ്കിൽ (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                തിരികെ 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    ആകുമ്പോൾ ((ശൂന്യം)0, 0)

പൂർണ്ണസംഖ്യ predictOptimalVectorWidth(നിവേശം_Array src1, നിവേശം_Array src2, നിവേശം_Array src3,
                              നിവേശം_Array src4, നിവേശം_Array src5, നിവേശം_Array src6,
                              നിവേശം_Array src7, നിവേശം_Array src8, നിവേശം_Array src9,
                              OclVectorStrategy strat)
{
    സ്ഥിരം ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    പൂർണ്ണസംഖ്യ vectorWidths[CV_DEPTH_MAX] = {
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
    എങ്കിൽ (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    തിരികെ checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

പൂർണ്ണസംഖ്യ checkOptimalVectorWidth([[maybe_unused]] സ്ഥിരം പൂർണ്ണസംഖ്യ *vectorWidths,
                            [[maybe_unused]] നിവേശം_Array src1,
                            [[maybe_unused]] നിവേശം_Array src2,
                            [[maybe_unused]] നിവേശം_Array src3,
                            [[maybe_unused]] നിവേശം_Array src4,
                            [[maybe_unused]] നിവേശം_Array src5,
                            [[maybe_unused]] നിവേശം_Array src6,
                            [[maybe_unused]] നിവേശം_Array src7,
                            [[maybe_unused]] നിവേശം_Array src8,
                            [[maybe_unused]] നിവേശം_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    തിരികെ 1;
#അല്ലെങ്കിൽ
    CV_Assert(vectorWidths);

    പൂർണ്ണസംഖ്യ ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<പൂർണ്ണസംഖ്യ> dividers, kercns;
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

    വേണ്ടി (size_t i = 0; i < size; ++i)
        ആകുമ്പോൾ (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    പൂർണ്ണസംഖ്യ kercn = *std::min_element(kercns.begin(), kercns.end());

    തിരികെ kercn;
#endif
}

പൂർണ്ണസംഖ്യ predictOptimalVectorWidthMax(നിവേശം_Array src1, നിവേശം_Array src2, നിവേശം_Array src3,
                                 നിവേശം_Array src4, നിവേശം_Array src5, നിവേശം_Array src6,
                                 നിവേശം_Array src7, നിവേശം_Array src8, നിവേശം_Array src9)
{
    തിരികെ predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
ശൂന്യം buildOptionsAddMatrixDescription(String& buildOptions, സ്ഥിരം String& name, നിവേശം_Array _m)
{
    എങ്കിൽ (!buildOptions.empty())
        buildOptions += " ";
    പൂർണ്ണസംഖ്യ type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (പൂർണ്ണസംഖ്യ)CV_MAT_CN(type),
            name.c_str(), (പൂർണ്ണസംഖ്യ)CV_ELEM_SIZE(type),
            name.c_str(), (പൂർണ്ണസംഖ്യ)CV_ELEM_SIZE1(type),
            name.c_str(), (പൂർണ്ണസംഖ്യ)depth
            );
}


ഘടന Image2D::Impl
{
    Impl(സ്ഥിരം UMat &src, തർക്കം norm, തർക്കം alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        എങ്കിൽ (handle)
            clReleaseMemObject(handle);
    }

    സ്ഥിതം cl_image_format getImageFormat(പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ cn, തർക്കം norm)
    {
        cl_image_format format;
        സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        സ്ഥിതം സ്ഥിരം പൂർണ്ണസംഖ്യ channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        പൂർണ്ണസംഖ്യ channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        പൂർണ്ണസംഖ്യ channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        തിരികെ format;
    }

    സ്ഥിതം തർക്കം isFormatSupported(cl_image_format format)
    {
        എങ്കിൽ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        എങ്കിൽ (!context)
            തിരികെ അസത്യം;

        // Figure out how many formats are supported by this context.
        cl_uint അക്കം_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, അക്കം_Formats,
                                                ശൂന്യ, &അക്കം_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        എങ്കിൽ (അക്കം_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(അക്കം_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, അക്കം_Formats,
                                             formats.data(), ശൂന്യ);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            വേണ്ടി (cl_uint i = 0; i < അക്കം_Formats; ++i)
            {
                എങ്കിൽ (!memcmp(&formats[i], &format, വലുപ്പം(format)))
                {
                    തിരികെ സത്യം;
                }
            }
        }
        തിരികെ അസത്യം;
    }

    ശൂന്യം init(സ്ഥിരം UMat &src, തർക്കം norm, തർക്കം alias)
    {
        എങ്കിൽ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        പൂർണ്ണസംഖ്യ err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        എങ്കിൽ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        എങ്കിൽ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        സ്ഥിരം Device & d = ocl::Device::getDefault();
        പൂർണ്ണസംഖ്യ minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        എങ്കിൽ (1 < major || (1 == major && 2 <= minor))
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
            desc.അക്കം_mip_levels   = 0;
            desc.അക്കം_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ശൂന്യ, &err);
        }
        അല്ലെങ്കിൽ
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, ശൂന്യ, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        എങ്കിൽ (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), ശൂന്യ, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long പൂർണ്ണസംഖ്യ)(src.cols * src.rows * src.elemSize()), (ശൂന്യം*)devData
                ).c_str());

            സ്ഥിരം size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, ശൂന്യ, ശൂന്യ));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        അല്ലെങ്കിൽ
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != ശൂന്യ);

        എങ്കിൽ (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, ശൂന്യ, 0));
            എങ്കിൽ (!src.isContinuous())
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
    p = ശൂന്യ;
}

Image2D::Image2D(സ്ഥിരം UMat &src, തർക്കം norm, തർക്കം alias)
{
    p = പുതിയ Impl(src, norm, alias);
}

തർക്കം Image2D::canCreateAlias(സ്ഥിരം UMat &m)
{
    തർക്കം ret = അസത്യം;
    സ്ഥിരം Device & d = ocl::Device::getDefault();
    എങ്കിൽ (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        എങ്കിൽ (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            എങ്കിൽ (!m.u->tempUMat())
            {
                ret = സത്യം;
            }
        }
    }
    തിരികെ ret;
}

തർക്കം Image2D::isFormatSupported(പൂർണ്ണസംഖ്യ depth, പൂർണ്ണസംഖ്യ cn, തർക്കം norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    തിരികെ Impl::isFormatSupported(format);
}

Image2D::Image2D(സ്ഥിരം Image2D & i)
{
    p = i.p;
    എങ്കിൽ (p)
        p->addref();
}

Image2D & Image2D::operator = (സ്ഥിരം Image2D & i)
{
    എങ്കിൽ (i.p != p)
    {
        എങ്കിൽ (i.p)
            i.p->addref();
        എങ്കിൽ (p)
            p->release();
        p = i.p;
    }
    തിരികെ *ഇത്;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    എങ്കിൽ (ഇത് != &i) {
        എങ്കിൽ (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    തിരികെ *ഇത്;
}

Image2D::~Image2D()
{
    എങ്കിൽ (p)
        p->release();
}

ശൂന്യം* Image2D::ptr() സ്ഥിരം
{
    തിരികെ p ? p->handle : 0;
}

തർക്കം internal::isOpenCLForced()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം value = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ value;
}

തർക്കം internal::isPerformanceCheckBypassed()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം തർക്കം value = അസത്യം;
    എങ്കിൽ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", അസത്യം);
        initialized = സത്യം;
    }
    തിരികെ value;
}

തർക്കം internal::isCLBuffer(UMat& u)
{
    എങ്കിൽ (isCudaBacked(u.u))
        തിരികെ അസത്യം;
    ശൂന്യം* h = u.handle(ACCESS_RW);
    എങ്കിൽ (!h)
        തിരികെ സത്യം;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#എങ്കിൽ 1
    എങ്കിൽ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        തിരികെ അസത്യം;
#അല്ലെങ്കിൽ
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, വലുപ്പം(type), &type, ശൂന്യ);
    എങ്കിൽ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        തിരികെ അസത്യം;
#endif
    തിരികെ സത്യം;
}

ഘടന Timer::Impl
{
    സ്ഥിരം Queue queue;

    Impl(സ്ഥിരം Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    ശൂന്യം start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    ശൂന്യം stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() സ്ഥിരം
    {
        തിരികെ (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(സ്ഥിരം Queue& q) : p(പുതിയ Impl(q)) { }
Timer::~Timer() { delete p; }

ശൂന്യം Timer::start()
{
    CV_Assert(p);
    p->start();
}

ശൂന്യം Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() സ്ഥിരം
{
    CV_Assert(p);
    തിരികെ p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
