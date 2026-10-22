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

#அந்தர்பாவ "precomp.hpp"

#ifndef HAVE_OPENCL
#அந்தர்பாவ "ocl_disabled.impl.hpp"
#இல்லையெனில் // HAVE_OPENCL

#அந்தர்பாவ <list>
#அந்தர்பாவ <map>
#அந்தர்பாவ <deque>
#அந்தர்பாவ <set>
#அந்தர்பாவ <string>
#அந்தர்பாவ <sstream>
#அந்தர்பாவ <fstream>
#எனில் !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#அந்தர்பாவ <inttypes.h>
#endif

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#அந்தர்பாவ "opencv2/core/ocl_genbase.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#அந்தர்பாவ "opencv2/core/utils/lock.private.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.hpp"
#அந்தர்பாவ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(வெற்று, 0, "OpenCL allocator: " << __VA_ARGS__)
#அந்தர்பாவ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_இருமம்_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#அந்தர்பாவ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# எனில் 0
#   define LOG_BUFFER_POOL printf
# இல்லையெனில்
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#எனில் CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#இல்லையெனில்
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#எனில் CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#இல்லையெனில்
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#அந்தர்பாவ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#அந்தர்பாவ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#அந்தர்பாவ "umatrix.hpp"

நாமாகாஶ cv { நாமாகாஶ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    வெறுமை addref() { CV_XADD(&refcount, 1); } \
    வெறுமை release() { எனில்( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete இது; } \
    முழு refcount

நிலையான cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    திரும்பு opencl_allocator_stats;
}

#ifndef _DEBUG
நிலையான தருக்க isRaiseError()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க value = பொய்;
    எனில் (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", பொய்);
        initialized = உண்மை;
    }
    திரும்பு value;
}
#endif

நிலையான வெறுமை onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    தருக்க value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", பொய்);
    எனில் (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#எனில் CV_OPENCL_TRACE_CHECK
நிலையான உள்வரி
வெறுமை traceOpenCLCheck(cl_int status, மாறிலி எழுத்து* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#இல்லையெனில்
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_செய்தி(check_result, செய்தி) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, செய்தி)

#define CV_OCL_CHECK_RESULT(check_result, செய்தி) \
    செய் { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, செய்தி); \
        எனில் (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(செய்தி), மாறிலி எழுத்து*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_செய்தி = CV_OCL_API_ERROR_செய்தி(check_result, செய்தி); \
            CV_Error(Error::OpenCLApiCallError, error_செய்தி); \
        } \
    } போது (0)

#define CV_OCL_CHECK_(கோவை, check_result) செய் { கோவை; CV_OCL_CHECK_RESULT(check_result, #expr); } போது (0)

#define CV_OCL_CHECK(கோவை) செய் { cl_int __cl_result = (கோவை); CV_OCL_CHECK_RESULT(__cl_result, #expr); } போது (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, செய்தி) CV_OCL_CHECK_RESULT(check_result, செய்தி)
#define CV_OCL_DBG_CHECK(கோவை) CV_OCL_CHECK(கோவை)
#define CV_OCL_DBG_CHECK_(கோவை, check_result) CV_OCL_CHECK_(கோவை, check_result)
#இல்லையெனில்
#define CV_OCL_DBG_CHECK_RESULT(check_result, செய்தி) \
    செய் { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, செய்தி); \
        எனில் (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(செய்தி), மாறிலி எழுத்து*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_செய்தி = CV_OCL_API_ERROR_செய்தி(check_result, செய்தி); \
            CV_Error(Error::OpenCLApiCallError, error_செய்தி); \
        } \
    } போது (0)
#define CV_OCL_DBG_CHECK_(கோவை, check_result) செய் { கோவை; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } போது (0)
#define CV_OCL_DBG_CHECK(கோவை) செய் { cl_int __cl_result = (கோவை); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } போது (0)
#endif


நிலையான மாறிலி தருக்க CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", உண்மை);
நிலையான மாறிலி தருக்க CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", உண்மை);
நிலையான மாறிலி தருக்க CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", உண்மை);
நிலையான மாறிலி தருக்க CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", உண்மை);

#எனில் CV_OPENCL_VALIDATE_இருமம்_PROGRAMS
நிலையான மாறிலி தருக்க CV_OPENCL_VALIDATE_இருமம்_PROGRAMS_மதிப்பு = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", பொய்);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
நிலையான மாறிலி தருக்க CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#எனில் 1 //def __APPLE__
        உண்மை
#இல்லையெனில்
        பொய்
#endif
);

நிலையான String getBuildExtraOptions()
{
    நிலையான String param_buildExtraOptions;
    நிலையான தருக்க initialized = பொய்;
    எனில் (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = உண்மை;
        எனில் (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(வெற்று, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    திரும்பு param_buildExtraOptions;
}

நிலையான மாறிலி தருக்க CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", உண்மை);
நிலையான மாறிலி size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


கட்டமைப்பு UMat2D
{
    UMat2D(மாறிலி UMat& m)
    {
        offset = (முழு)m.offset;
        step = (முழு)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    முழு offset;
    முழு step;
    முழு rows;
    முழு cols;
};

கட்டமைப்பு UMat3D
{
    UMat3D(மாறிலி UMat& m)
    {
        offset = (முழு)m.offset;
        step = (முழு)m.step.p[1];
        slicestep = (முழு)m.step.p[0];
        slices = (முழு)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    முழு offset;
    முழு slicestep;
    முழு step;
    முழு slices;
    முழு rows;
    முழு cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
நிலையான uint64 crc64( மாறிலி uchar* data, size_t size, uint64 crc0=0 )
{
    நிலையான uint64 table[256];
    நிலையான தருக்க initialized = பொய்;

    எனில்( !initialized )
    {
        க்கு( முழு i = 0; i < 256; i++ )
        {
            uint64 c = i;
            க்கு( முழு j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = உண்மை;
    }

    uint64 crc = ~crc0;
    க்கு( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    திரும்பு ~crc;
}

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
கட்டமைப்பு OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    வகைவரை std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(வெற்று, "Initializing OpenCL cache configuration...");
        எனில் (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(வெற்று, "OpenCL cache is disabled");
            திரும்பு;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        எனில் (cache_path_.empty())
        {
            CV_LOG_INFO(வெற்று, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        செய்
        {
            முயல்
            {
                எனில் (cache_path_.empty())
                    நிறுத்து;
                எனில் (cache_path_ == "disabled")
                    நிறுத்து;
                எனில் (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(வெற்று, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    நிறுத்து;
                }

                எனில் (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    எனில் (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(வெற்று, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        எனில் (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(வெற்று, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            நிறுத்து;
                        }
                    }

                    முயல்
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(வெற்று, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(வெற்று, 0, "Checking cache lock... Done!");
                    }
                    பிடி (மாறிலி cv::Exception& e)
                    {
                        CV_LOG_WARNING(வெற்று, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    பிடி (...)
                    {
                        CV_LOG_WARNING(வெற்று, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                இல்லையெனில்
                {
                    எனில் (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(வெற்று, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    இல்லையெனில்
                    {
                        CV_LOG_INFO(வெற்று, "OpenCL cache lock is disabled");
                    }
                }
            }
            பிடி (மாறிலி cv::Exception& e)
            {
                CV_LOG_WARNING(வெற்று, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } போது (0);
        எனில் (!cache_path_.empty())
        {
            எனில் (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(வெற்று, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            இல்லையெனில்
            {
                CV_LOG_INFO(வெற்று, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    வெறுமை clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(மாறிலி std::string& ctx_prefix,
            மாறிலி std::string& cleanup_prefix)
    {
        எனில் (cache_path_.empty())
            திரும்பு std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        எனில் (found_it != prepared_contexts_.end())
            திரும்பு found_it->second;

        CV_LOG_INFO(வெற்று, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        தருக்க result = utils::fs::isDirectory(target_directory);
        எனில் (!result)
        {
            முயல்
            {
                CV_LOG_VERBOSE(வெற்று, 0, "Creating directory: " << target_directory);
                எனில் (utils::fs::createDirectories(target_directory))
                {
                    result = உண்மை;
                }
                இல்லையெனில்
                {
                    CV_LOG_WARNING(வெற்று, "Can't create directory: " << target_directory);
                }
            }
            பிடி (மாறிலி cv::Exception& e)
            {
                CV_LOG_ERROR(வெற்று, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        எனில் (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            முயல்
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, பொய், உண்மை);
                std::vector<String> remove_entries;
                க்கு (size_t i = 0; i < entries.size(); i++)
                {
                    மாறிலி String& name = entries[i];
                    எனில் (0 == name.find(cleanup_prefix))
                    {
                        எனில் (0 == name.find(ctx_prefix))
                            தொடர்; // skip current
                        remove_entries.push_back(name);
                    }
                }
                எனில் (!remove_entries.empty())
                {
                    CV_LOG_WARNING(வெற்று, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(வெற்று, "Trying to remove these directories...");
                    க்கு (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(வெற்று, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(வெற்று, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    க்கு (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        மாறிலி String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        முயல்
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(வெற்று, "Removed: " << path);
                        }
                        பிடி (மாறிலி cv::Exception& e)
                        {
                            CV_LOG_ERROR(வெற்று, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            பிடி (...)
            {
                CV_LOG_WARNING(வெற்று, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(வெற்று, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        திரும்பு target_directory;
    }

    நிலையான OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, புதிய OpenCLBinaryCacheConfigurator());
    }
};
வகுப்பு இருமம்_Program_File
{
    எண்ணிக்கை { MAX_ENTRIES = 64 };

    வகைவரை unsigned முழு uint32_t;

    கட்டமைப்பு CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    கட்டமைப்பு CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t எண்_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    கட்டமைப்பு CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t சாவி_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    மாறிலி std::string fileName_;
    மாறிலி எழுத்து* மாறிலி sourceSignature_;
    மாறிலி size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(மாறிலி std::string& options)
    {
        uint64 hash = crc64((மாறிலி uchar*)options.c_str(), options.size(), 0);
        திரும்பு hash & (MAX_ENTRIES - 1);
    }

    உள்வரி size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        திரும்பு fileSize;
    }
    உள்வரி uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.பட((எழுத்து*)&res, அளவு(uint32_t));
        CV_Assert(!f.fail());
        திரும்பு res;
    }
    உள்வரி வெறுமை writeUInt32(மாறிலி uint32_t value)
    {
        uint32_t v = value;
        f.லிக((எழுத்து*)&v, அளவு(uint32_t));
        CV_Assert(!f.fail());
    }

    உள்வரி வெறுமை seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    உள்வரி வெறுமை seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    உள்வரி வெறுமை seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    வெறுமை clearFile()
    {
        f.close();
        எனில் (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(வெற்று, "Can't remove: " << fileName_);
        திரும்பு;
    }

பொது:
    இருமம்_Program_File(மாறிலி std::string& fileName, மாறிலி எழுத்து* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(அளவு(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != வெற்று);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, அளவு(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        எனில்(f.is_open() && getFileSize() > 0)
        {
            தருக்க isValid = பொய்;
            முயல்
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                எனில் (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<எழுத்து> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.பட(fileSourceSignature.data(), fileSourceSignatureSize);
                    எனில் (f.eof())
                    {
                        CV_LOG_ERROR(வெற்று, "Unexpected EOF");
                    }
                    இல்லையெனில் எனில் (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = உண்மை;
                    }
                }
                எனில் (!isValid)
                {
                    CV_LOG_ERROR(வெற்று, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            பிடி (மாறிலி cv::Exception& e)
            {
                CV_LOG_ERROR(வெற்று, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            பிடி (...)
            {
                CV_LOG_ERROR(வெற்று, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            எனில் (!isValid)
            {
                clearFile();
            }
            இல்லையெனில்
            {
                seekReadAbsolute(0);
            }
        }
    }

    தருக்க பட(மாறிலி std::string& சாவி, std::vector<எழுத்து>& buf)
    {
        எனில் (!f.is_open())
            திரும்பு பொய்;

        size_t fileSize = getFileSize();
        எனில் (fileSize == 0)
        {
            CV_LOG_ERROR(வெற்று, "Invalid file (empty): " << fileName_);
            clearFile();
            திரும்பு பொய்;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t எண்_Of_Entries = readUInt32();
        CV_Assert(எண்_Of_Entries > 0);
        எனில் (எண்_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(வெற்று, "Invalid file: " << fileName_);
            clearFile();
            திரும்பு பொய்;
        }
        f.பட((எழுத்து*)&entryOffsets[0], அளவு(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_இலக்கம் = getHash(சாவி);

        uint32_t entryOffset = entryOffsets[entry_இலக்கம்];
        FileEntry entry;
        போது (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.பட((எழுத்து*)&entry, அளவு(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<எழுத்து> file_சாவி(entry.சாவி_Size + 1);
            எனில் (சாவி.size() == entry.சாவி_Size)
            {
                எனில் (entry.சாவி_Size > 0)
                {
                    f.பட(file_சாவி.data(), entry.சாவி_Size);
                    CV_Assert(!f.fail());
                }
                எனில் (memcmp(file_சாவி.data(), சாவி.c_str(), entry.சாவி_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.பட(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(வெற்று, 0, "Read...");
                    திரும்பு உண்மை;
                }
            }
            எனில் (entry.nextEntryFileOffset == 0)
                நிறுத்து;
            entryOffset = entry.nextEntryFileOffset;
        }
        திரும்பு பொய்;
    }

    தருக்க லிக(மாறிலி std::string& சாவி, std::vector<எழுத்து>& buf)
    {
        எனில் (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            எனில் (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                எனில் (!f.is_open())
                {
                    CV_LOG_ERROR(வெற்று, "Can't create file: " << fileName_);
                    திரும்பு பொய்;
                }
            }
        }

        size_t fileSize = getFileSize();
        எனில் (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.லிக(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, அளவு(entryOffsets));
            f.லிக((எழுத்து*)entryOffsets, அளவு(entryOffsets));
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

        uint32_t எண்_Of_Entries = readUInt32();
        CV_Assert(எண்_Of_Entries > 0);
        எனில் (எண்_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(வெற்று, "Invalid file: " << fileName_);
            clearFile();
            திரும்பு பொய்;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.பட((எழுத்து*)&entryOffsets[0], அளவு(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_இலக்கம் = getHash(சாவி);

        uint32_t entryOffset = entryOffsets[entry_இலக்கம்];
        FileEntry entry;
        போது (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.பட((எழுத்து*)&entry, அளவு(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<எழுத்து> file_சாவி(entry.சாவி_Size + 1);
            எனில் (சாவி.size() == entry.சாவி_Size)
            {
                எனில் (entry.சாவி_Size > 0)
                {
                    f.பட(file_சாவி.data(), entry.சாவி_Size);
                    CV_Assert(!f.fail());
                }
                எனில் (0 == memcmp(file_சாவி.data(), சாவி.c_str(), entry.சாவி_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(வெற்று, 0, "Duplicate key ignored: " << fileName_);
                    திரும்பு பொய்;
                }
            }
            எனில் (entry.nextEntryFileOffset == 0)
                நிறுத்து;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        எனில் (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.லிக((எழுத்து*)&entry, அளவு(entry));
            CV_Assert(!f.fail());
        }
        இல்லையெனில்
        {
            entryOffsets[entry_இலக்கம்] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.லிக((எழுத்து*)entryOffsets, அளவு(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.சாவி_Size = (uint32_t)சாவி.size();
        f.லிக((எழுத்து*)&entry, அளவு(entry));
        CV_Assert(!f.fail());
        f.லிக(சாவி.c_str(), entry.சாவி_Size);
        CV_Assert(!f.fail());
        f.லிக(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(வெற்று, 0, "Write... (" << buf.size() << " bytes)");
        திரும்பு உண்மை;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



கட்டமைப்பு OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    முழு device_;  // device index in context
    ocl::Queue queue_;
    முழு useOpenCL_;

பாதுகாக்கப்பட்ட:
    Impl() = delete;

    வெறுமை _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        முழு ndevices = (முழு)context_.ndevices();
        CV_Assert(ndevices > 0);
        தருக்க found = பொய்;
        க்கு (முழு i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            எனில் (dhandle == deviceID)
            {
                device_ = i;
                found = உண்மை;
                நிறுத்து;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    வெறுமை _init_device(மாறிலி ocl::Device& device)
    {
        CV_Assert(device.ptr());
        முழு ndevices = (முழு)context_.ndevices();
        CV_Assert(ndevices > 0);
        தருக்க found = பொய்;
        க்கு (முழு i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            எனில் (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = உண்மை;
                நிறுத்து;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

பொது:
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

    Impl(மாறிலி ocl::Context& context, மாறிலி ocl::Device& device, மாறிலி ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(மாறிலி ocl::Context& context, மாறிலி ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(மாறிலி ocl::Context& context, மாறிலி முழு device, மாறிலி ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(மாறிலி Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    உள்வரி தருக்க useOpenCL() மாறிலி { திரும்பு const_cast<Impl*>(இது)->useOpenCL(); }
    தருக்க useOpenCL()
    {
        எனில் (useOpenCL_ < 0)
        {
            முயல்
            {
                useOpenCL_ = 0;
                எனில் (!context_.empty() && context_.ndevices() > 0)
                {
                    மாறிலி Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            பிடி (மாறிலி cv::Exception&)
            {
                // nothing
            }
            எனில் (!useOpenCL_)
                CV_LOG_INFO(வெற்று, "OpenCL: can't use OpenCL execution context");
        }
        திரும்பு useOpenCL_ > 0;
    }

    வெறுமை setUseOpenCL(தருக்க flag)
    {
        எனில் (!flag)
            useOpenCL_ = 0;
        இல்லையெனில்
            useOpenCL_ = -1;
    }

    நிலையான மாறிலி std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(வெற்று, "OpenCL: initializing thread execution context");

        நிலையான தருக்க initialized = பொய்;
        நிலையான std::shared_ptr<Impl> g_primaryExecutionContext;

        எனில் (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            எனில் (!initialized)
            {
                CV_LOG_INFO(வெற்று, "OpenCL: creating new execution context...");
                முயல்
                {
                    Context c = ocl::Context::create(std::string());
                    எனில் (c.ndevices())
                    {
                        முழு deviceId = 0;
                        auto& d = c.device(deviceId);
                        எனில் (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            எனில் (!q.ptr())
                            {
                                CV_LOG_ERROR(வெற்று, "OpenCL: Can't create default OpenCL queue");
                            }
                            இல்லையெனில்
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(வெற்று, "OpenCL: device=" << d.name());
                            }
                        }
                        இல்லையெனில்
                        {
                            CV_LOG_ERROR(வெற்று, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    இல்லையெனில்
                    {
                        CV_LOG_INFO(வெற்று, "OpenCL: context is not available/disabled");
                    }
                }
                பிடி (மாறிலி std::exception& e)
                {
                    CV_LOG_INFO(வெற்று, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                பிடி (...)
                {
                    CV_LOG_WARNING(வெற்று, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = உண்மை;
            }
        }
        திரும்பு g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->context_;
}
Device& OpenCLExecutionContext::getDevice() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->queue_;
}

தருக்க OpenCLExecutionContext::useOpenCL() மாறிலி
{
    எனில் (p)
        திரும்பு p->useOpenCL();
    திரும்பு பொய்;
}
வெறுமை OpenCLExecutionContext::setUseOpenCL(தருக்க flag)
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
    எனில் (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = உண்மை;
        எனில் (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    திரும்பு c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    திரும்பு c;
}

வெறுமை OpenCLExecutionContext::bind() மாறிலி
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *இது;
    data.oclExecutionContextInitialized = உண்மை;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() மாறிலி
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    மாறிலி Queue q(getContext(), getDevice());
    திரும்பு cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(மாறிலி ocl::Queue& q) மாறிலி
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != வெற்று);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    திரும்பு c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(மாறிலி Context& context, மாறிலி Device& device, மாறிலி ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    எனில் (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    திரும்பு ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(மாறிலி Context& context, மாறிலி Device& device)
{
    CV_TRACE_FUNCTION();
    எனில் (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    திரும்பு ctx;

}

வெறுமை OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
நிலையான தருக்க g_isOpenCLInitialized = பொய்;
நிலையான தருக்க g_isOpenCLAvailable = பொய்;

தருக்க haveOpenCL()
{
    CV_TRACE_FUNCTION();

    எனில் (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        எனில் (!envPath.empty())
        {
            எனில் (envPath == "disabled")
            {
                g_isOpenCLAvailable = பொய்;
                g_isOpenCLInitialized = உண்மை;
                திரும்பு பொய்;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(வெற்று, "Initialize OpenCL runtime...");
        முயல்
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, வெற்று, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(வெற்று, "OpenCL: found " << n << " platforms");
        }
        பிடி (...)
        {
            g_isOpenCLAvailable = பொய்;
        }
        g_isOpenCLInitialized = உண்மை;
    }
    திரும்பு g_isOpenCLAvailable;
}

தருக்க useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    எனில் (data.useOpenCL < 0)
    {
        முயல்
        {
            data.useOpenCL = 0;
            எனில் (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        பிடி (...)
        {
            CV_LOG_INFO(வெற்று, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    திரும்பு data.useOpenCL > 0;
}

தருக்க isOpenCLActivated()
{
    எனில் (!g_isOpenCLAvailable)
        திரும்பு பொய்; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    திரும்பு useOpenCL();
}

வெறுமை setUseOpenCL(தருக்க flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    எனில் (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    இல்லையெனில்
    {
        எனில் (!flag)
            data.useOpenCL = 0;
        இல்லையெனில்
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

வகுப்பு AmdBlasHelper
{
பொது:
    நிலையான AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, புதிய AmdBlasHelper())
    }

    தருக்க isAvailable() மாறிலி
    {
        திரும்பு g_isAmdBlasAvailable;
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

பாதுகாக்கப்பட்ட:
    AmdBlasHelper()
    {
        எனில் (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            எனில் (!g_isAmdBlasInitialized)
            {
                எனில் (haveOpenCL())
                {
                    முயல்
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    பிடி (...)
                    {
                        g_isAmdBlasAvailable = பொய்;
                    }
                }
                இல்லையெனில்
                    g_isAmdBlasAvailable = பொய்;

                g_isAmdBlasInitialized = உண்மை;
            }
        }
    }

தனிப்பட்ட:
    நிலையான தருக்க g_isAmdBlasInitialized;
    நிலையான தருக்க g_isAmdBlasAvailable;
};

தருக்க AmdBlasHelper::g_isAmdBlasAvailable = பொய்;
தருக்க AmdBlasHelper::g_isAmdBlasInitialized = பொய்;

தருக்க haveAmdBlas()
{
    திரும்பு AmdBlasHelper::getInstance().isAvailable();
}

#இல்லையெனில்

தருக்க haveAmdBlas()
{
    திரும்பு பொய்;
}

#endif

#ifdef HAVE_CLAMDFFT

வகுப்பு AmdFftHelper
{
பொது:
    நிலையான AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, புதிய AmdFftHelper())
    }

    தருக்க isAvailable() மாறிலி
    {
        திரும்பு g_isAmdFftAvailable;
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

பாதுகாக்கப்பட்ட:
    AmdFftHelper()
    {
        எனில் (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            எனில் (!g_isAmdFftInitialized)
            {
                எனில் (haveOpenCL())
                {
                    முயல்
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = உண்மை;
                    }
                    பிடி (மாறிலி Exception &)
                    {
                        g_isAmdFftAvailable = பொய்;
                    }
                }
                இல்லையெனில்
                    g_isAmdFftAvailable = பொய்;

                g_isAmdFftInitialized = உண்மை;
            }
        }
    }

தனிப்பட்ட:
    நிலையான clfftSetupData setupData;
    நிலையான தருக்க g_isAmdFftInitialized;
    நிலையான தருக்க g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
தருக்க AmdFftHelper::g_isAmdFftAvailable = பொய்;
தருக்க AmdFftHelper::g_isAmdFftInitialized = பொய்;

தருக்க haveAmdFft()
{
    திரும்பு AmdFftHelper::getInstance().isAvailable();
}

#இல்லையெனில்

தருக்க haveAmdFft()
{
    திரும்பு பொய்;
}

#endif

தருக்க haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    திரும்பு உண்மை;
#இல்லையெனில்
    திரும்பு பொய்;
#endif
}

வெறுமை finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

கட்டமைப்பு Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = பொய்;
    }

    ~Impl() {}

    வெறுமை init()
    {
        எனில்( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            எனில்( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            எனில்( handle != 0 )
            {
                எழுத்து buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, அளவு(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = உண்மை;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    தருக்க initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    எனில்(p)
        p->release();
}

Platform::Platform(மாறிலி Platform& pl)
{
    p = (Impl*)pl.p;
    எனில்(p)
        p->addref();
}

Platform& Platform::operator = (மாறிலி Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    எனில் (இது != &pl) {
        எனில்(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    திரும்பு *இது;
}

வெறுமை* Platform::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(வெற்று, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    நிலையான Platform p;
    எனில்( !p.p )
    {
        p.p = புதிய Impl;
        p.p->init();
    }
    திரும்பு p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
நிலையான வெறுமை parseOpenCLVersion(மாறிலி String &version, முழு &major, முழு &minor)
{
    major = minor = 0;
    எனில் (10 >= version.length())
        திரும்பு;
    மாறிலி எழுத்து *pstr = version.c_str();
    எனில் (0 != strncmp(pstr, "OpenCL ", 7))
        திரும்பு;
    size_t ppos = version.find('.', 7);
    எனில் (String::npos == ppos)
        திரும்பு;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

கட்டமைப்பு Device::Impl
{
    Impl(வெறுமை* d)
        : refcount(1)
        , handle(0)
    {
        முயல்
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        பிடி (...)
        {
            எறி;
        }
    }

    வெறுமை _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, முழு>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, முழு>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, முழு>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, முழு>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, முழு>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        போது (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            எனில் (pos2 == String::npos)
                pos2 = extensions_.size();
            எனில் (pos2 > pos)
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
        எனில் (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        இல்லையெனில் எனில் (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        இல்லையெனில் எனில் (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        இல்லையெனில்
            vendorID_ = UNKNOWN_VENDOR;

        மாறிலி size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        எனில் (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            மாறிலி size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            எனில் (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(வெற்று, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#எனில் 0
        எனில் (isExtensionSupported("cl_khr_spir"))
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
        எனில் (!cv::__termination)
#endif
        {
            எனில் (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) மாறிலி
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        திரும்பு clGetDeviceInfo(handle, prop, அளவு(temp), &temp, &sz) == CL_SUCCESS &&
            sz == அளவு(temp) ? _TpOut(temp) : _TpOut();
    }

    தருக்க getBoolProp(cl_device_info prop) மாறிலி
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        திரும்பு clGetDeviceInfo(handle, prop, அளவு(temp), &temp, &sz) == CL_SUCCESS &&
            sz == அளவு(temp) ? temp != 0 : பொய்;
    }

    String getStrProp(cl_device_info prop) மாறிலி
    {
        எழுத்து buf[4096];
        size_t sz=0;
        திரும்பு clGetDeviceInfo(handle, prop, அளவு(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < அளவு(buf) ? String(buf) : String();
    }

    தருக்க isExtensionSupported(மாறிலி std::string& extensionName) மாறிலி
    {
        திரும்பு extensions_set_.மொத்தம்_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    முழு doubleFPConfig_;
    தருக்க khr_fp64_support_;
    முழு halfFPConfig_;
    தருக்க khr_fp16_support_;
    தருக்க hostUnifiedMemory_;
    முழு maxComputeUnits_;
    size_t maxWorkGroupSize_;
    முழு type_;
    முழு addressBits_;
    முழு deviceVersionMajor_;
    முழு deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    முழு vendorID_;
    தருக்க intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(வெறுமை* d)
{
    p = 0;
    set(d);
}

Device::Device(மாறிலி Device& d)
{
    p = d.p;
    எனில்(p)
        p->addref();
}

Device& Device::operator = (மாறிலி Device& d)
{
    Impl* newp = (Impl*)d.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    எனில் (இது != &d) {
        எனில்(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    திரும்பு *இது;
}

Device::~Device()
{
    எனில்(p)
        p->release();
}

வெறுமை Device::set(வெறுமை* d)
{
    எனில்(p)
        p->release();
    p = புதிய Impl(d);
    எனில் (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(வெறுமை* d)
{
    Device device(d);
    திரும்பு device;
}

வெறுமை* Device::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

String Device::name() மாறிலி
{ திரும்பு p ? p->name_ : String(); }

String Device::extensions() மாறிலி
{ திரும்பு p ? String(p->extensions_) : String(); }

தருக்க Device::isExtensionSupported(மாறிலி String& extensionName) மாறிலி
{ திரும்பு p ? p->isExtensionSupported(extensionName) : பொய்; }

String Device::version() மாறிலி
{ திரும்பு p ? p->version_ : String(); }

String Device::vendorName() மாறிலி
{ திரும்பு p ? p->vendorName_ : String(); }

முழு Device::vendorID() மாறிலி
{ திரும்பு p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() மாறிலி
{ திரும்பு p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() மாறிலி
{ திரும்பு p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

முழு Device::deviceVersionMajor() மாறிலி
{ திரும்பு p ? p->deviceVersionMajor_ : 0; }

முழு Device::deviceVersionMinor() மாறிலி
{ திரும்பு p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() மாறிலி
{ திரும்பு p ? p->driverVersion_ : String(); }

முழு Device::type() மாறிலி
{ திரும்பு p ? p->type_ : 0; }

முழு Device::addressBits() மாறிலி
{ திரும்பு p ? p->addressBits_ : 0; }

தருக்க Device::available() மாறிலி
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : பொய்; }

தருக்க Device::compilerAvailable() மாறிலி
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : பொய்; }

தருக்க Device::linkerAvailable() மாறிலி
#ifdef CL_VERSION_1_2
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : பொய்; }
#இல்லையெனில்
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

முழு Device::doubleFPConfig() மாறிலி
{ திரும்பு p ? p->doubleFPConfig_ : 0; }

முழு Device::singleFPConfig() மாறிலி
{ திரும்பு p ? p->getProp<cl_device_fp_config, முழு>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

முழு Device::halfFPConfig() மாறிலி
{ திரும்பு p ? p->halfFPConfig_ : 0; }

தருக்க Device::hasFP64() மாறிலி
{ திரும்பு p ? p->khr_fp64_support_ : பொய்; }
தருக்க Device::hasFP16() மாறிலி
{ திரும்பு p ? p->khr_fp16_support_ : பொய்; }

தருக்க Device::endianLittle() மாறிலி
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : பொய்; }

தருக்க Device::errorCorrectionSupport() மாறிலி
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : பொய்; }

முழு Device::executionCapabilities() மாறிலி
{ திரும்பு p ? p->getProp<cl_device_exec_capabilities, முழு>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

முழு Device::globalMemCacheType() மாறிலி
{ திரும்பு p ? p->getProp<cl_device_mem_cache_type, முழு>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

முழு Device::global_Mem_Cache_வரி_Size() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

முழு Device::localMemType() மாறிலி
{ திரும்பு p ? p->getProp<cl_device_local_mem_type, முழு>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

தருக்க Device::hostUnifiedMemory() மாறிலி
{ திரும்பு p ? p->hostUnifiedMemory_ : பொய்; }

தருக்க Device::imageSupport() மாறிலி
{ திரும்பு p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : பொய்; }

தருக்க Device::imageFromBufferSupport() மாறிலி
{
    திரும்பு p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : பொய்;
}

uint Device::imagePitchAlignment() மாறிலி
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    திரும்பு p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#இல்லையெனில்
    திரும்பு 0;
#endif
}

uint Device::imageBaseAddressAlignment() மாறிலி
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    திரும்பு p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#இல்லையெனில்
    திரும்பு 0;
#endif
}

size_t Device::image2DMaxWidth() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() மாறிலி
#ifdef CL_VERSION_1_2
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#இல்லையெனில்
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() மாறிலி
#ifdef CL_VERSION_1_2
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#இல்லையெனில்
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

தருக்க Device::intelSubgroupsSupport() மாறிலி
{ திரும்பு p ? p->intelSubgroupsSupport_ : பொய்; }

முழு Device::maxClockFrequency() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

முழு Device::maxComputeUnits() மாறிலி
{ திரும்பு p ? p->maxComputeUnits_ : 0; }

முழு Device::maxConstantArgs() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() மாறிலி
{ திரும்பு p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

முழு Device::maxReadImageArgs() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

முழு Device::maxWriteImageArgs() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

முழு Device::maxSamplers() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() மாறிலி
{ திரும்பு p ? p->maxWorkGroupSize_ : 0; }

முழு Device::maxWorkItemDims() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

வெறுமை Device::maxWorkItemSizes(size_t* sizes) மாறிலி
{
    எனில்(p)
    {
        மாறிலி முழு MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*அளவு(sizes[0]), &sizes[0], &retsz));
    }
}

முழு Device::memBaseAddrAlign() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

முழு Device::nativeVectorWidthChar() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

முழு Device::nativeVectorWidthShort() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

முழு Device::nativeVectorWidthInt() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

முழு Device::nativeVectorWidthLong() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

முழு Device::nativeVectorWidthFloat() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

முழு Device::nativeVectorWidthDouble() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

முழு Device::nativeVectorWidthHalf() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

முழு Device::preferredVectorWidthChar() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

முழு Device::preferredVectorWidthShort() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

முழு Device::preferredVectorWidthInt() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

முழு Device::preferredVectorWidthLong() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

முழு Device::preferredVectorWidthFloat() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

முழு Device::preferredVectorWidthDouble() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

முழு Device::preferredVectorWidthHalf() மாறிலி
{ திரும்பு p ? p->getProp<cl_uint, முழு>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() மாறிலி
#ifdef CL_VERSION_1_2
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#இல்லையெனில்
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() மாறிலி
{ திரும்பு p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

மாறிலி Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    எனில் (!c.empty())
    {
        திரும்பு c.getDevice();
    }

    நிலையான Device dummy;
    திரும்பு dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
உள்வரி cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, வெற்று, &required);
    எனில் (err != CL_SUCCESS)
        திரும்பு err;

    param.clear();
    எனில் (required > 0)
    {
        AutoBuffer<எழுத்து> buf(required + 1);
        எழுத்து* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, வெற்று);
        எனில் (err != CL_SUCCESS)
            திரும்பு err;
        param = ptr;
    }

    திரும்பு CL_SUCCESS;
}

நிலையான வெறுமை split(மாறிலி std::string &s, எழுத்து delim, std::vector<std::string> &elems)
{
    elems.clear();
    எனில் (s.size() == 0)
        திரும்பு;
    std::istringstream ss(s);
    std::string item;
    போது (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
நிலையான தருக்க parseOpenCLDeviceConfiguration(மாறிலி std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    எனில் (parts.size() > 3)
    {
        CV_LOG_ERROR(வெற்று, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        திரும்பு பொய்;
    }
    எனில் (parts.size() > 2)
        deviceNameOrID = parts[2];
    எனில் (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    எனில் (parts.size() > 0)
    {
        platform = parts[0];
    }
    திரும்பு உண்மை;
}

நிலையான cl_device_id selectOpenCLDevice(மாறிலி std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    எனில் (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    எனில் (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        திரும்பு வெற்று;

    தருக்க isID = பொய்;
    முழு deviceID = -1;
    எனில் (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = உண்மை;
        க்கு (size_t i = 0; i < deviceName.length(); i++)
        {
            எனில் (!isdigit(deviceName[i]))
            {
                isID = பொய்;
                நிறுத்து;
            }
        }
        எனில் (isID)
        {
            deviceID = atoi(deviceName.c_str());
            எனில் (deviceID < 0)
                திரும்பு வெற்று;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint இலக்கம்_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms));

        எனில் (இலக்கம்_Platforms == 0)
            திரும்பு வெற்று;
        platforms.resize((size_t)இலக்கம்_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], &இலக்கம்_Platforms));
        platforms.resize(இலக்கம்_Platforms);
    }

    எனில் (platform.length() > 0)
    {
        க்கு (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            எனில் (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            இல்லையெனில்
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        எனில் (platforms.size() == 0)
        {
            CV_LOG_ERROR(வெற்று, "OpenCL: Can't find OpenCL platform by name: " << platform);
            செல் not_found;
        }
    }
    எனில் (deviceTypes.size() == 0)
    {
        எனில் (!isID)
        {
            deviceTypes.push_back("GPU");
            எனில் (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        இல்லையெனில்
            deviceTypes.push_back("ALL");
    }
    க்கு (size_t t = 0; t < deviceTypes.size(); t++)
    {
        முழு deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        எனில் (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        இல்லையெனில் எனில் (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        இல்லையெனில் எனில் (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        இல்லையெனில் எனில் (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        இல்லையெனில்
        {
            CV_LOG_ERROR(வெற்று, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            செல் not_found;
        }

        std::vector<cl_device_id> devices;
        க்கு (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint மொத்தம்_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, வெற்று, &மொத்தம்_2);
            எனில் (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            எனில் (மொத்தம்_2 == 0)
                தொடர்;
            size_t base = devices.size();
            devices.resize(base + மொத்தம்_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, மொத்தம்_2, &devices[base], &மொத்தம்_2);
            எனில் (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        க்கு (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : உண்மை) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = உண்மை;
            எனில்(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, அளவு(isIGPU), &isIGPU, வெற்று));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            எனில் ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                திரும்பு devices[i];
            }
        }
    }

not_found:
    எனில் (configuration.empty())
        திரும்பு வெற்று; // suppress messages on stderr

    std::ostringstream செய்தி;
    செய்தி << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    க்கு (size_t t = 0; t < deviceTypes.size(); t++)
        செய்தி << ' ' << deviceTypes[t];

    செய்தி << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(வெற்று, செய்தி.str());
    திரும்பு வெற்று;
}

#ifdef HAVE_OPENCL_SVM
நாமாகாஶ svm {

எண்ணிக்கை AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

நிலையான தருக்க checkForceSVMUmatUsage()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க force = பொய்;
    எனில் (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", பொய்);
        initialized = உண்மை;
    }
    திரும்பு force;
}
நிலையான தருக்க checkDisableSVMUMatUsage()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க force = பொய்;
    எனில் (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", பொய்);
        initialized = உண்மை;
    }
    திரும்பு force;
}
நிலையான தருக்க checkDisableSVM()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க force = பொய்;
    எனில் (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", பொய்);
        initialized = உண்மை;
    }
    திரும்பு force;
}
// see SVMCapabilities
நிலையான unsigned முழு getSVMCapabilitiesMask()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான unsigned முழு mask = 0;
    எனில் (!initialized)
    {
        மாறிலி std::string env_மதிப்பு = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        எனில் (env_மதிப்பு.empty())
        {
            திரும்பு ~0U; // all bits 1
        }
        mask = atoi(env_மதிப்பு.c_str());
        initialized = உண்மை;
    }
    திரும்பு mask;
}
} // namespace
#endif

நிலையான size_t get_Program_மொத்தம்_Limit()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான size_t மொத்தம்_2 = 0;
    எனில் (!initialized)
    {
        மொத்தம்_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = உண்மை;
    }
    திரும்பு மொத்தம்_2;
}

நிலையான முழு g_contextId = 0;

வகுப்பு OpenCLBufferPoolImpl;
வகுப்பு OpenCLSVMBufferPoolImpl;

கட்டமைப்பு Context::Impl
{
    நிலையான Context::Impl* get(Context& context) { திரும்பு context.getImpl(); }

    வகைவரை std::deque<Context::Impl*> container_t;
    நிலையான container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        நிலையான container_t* g_contexts = புதிய container_t();
        திரும்பு *g_contexts;
    }

பாதுகாக்கப்பட்ட:
    Impl(மாறிலி std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(பொய்)
#endif
    {
        எனில் (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = இது;
    }

    ~Impl()
    {
#ifdef _WIN32
        எனில் (!cv::__termination)
#endif
        {
            எனில் (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = வெற்று;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = வெற்று;
        }
    }

    வெறுமை init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_இலக்கம்_DEVICES, அளவு(ndevices), &ndevices, வெற்று));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * அளவு(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * அளவு(cl_device_id), "");

        devices.clear();
        க்கு (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    வெறுமை __init_buffer_pools();  // w/o synchronization
    வெறுமை _init_buffer_pools() மாறிலி
    {
        எனில் (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            எனில் (!bufferPool_)
            {
                const_cast<Impl*>(இது)->__init_buffer_pools();
            }
        }
    }
பொது:
    நிலையான Impl* findContext(மாறிலி std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        எனில் (configuration.empty() && !container.empty())
            திரும்பு container[0];
        க்கு (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            எனில் (i && i->configuration == configuration)
            {
                திரும்பு i;
            }
        }
        திரும்பு வெற்று;
    }

    நிலையான Impl* findOrCreateContext(மாறிலி std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        எனில் (configuration_.empty())
        {
            மாறிலி std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            எனில் (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        எனில் (impl)
        {
            CV_LOG_INFO(வெற்று, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            திரும்பு impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        எனில் (d == வெற்று)
            திரும்பு வெற்று;

        impl = புதிய Impl(configuration);
        முயல்
        {
            impl->createFromDevice(d);
            எனில் (impl->handle)
                திரும்பு impl;
            delete impl;
            திரும்பு வெற்று;
        }
        பிடி (...)
        {
            delete impl;
            எறி;
        }
    }

    நிலையான Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (வெறுமை*)h);
        Impl* impl = findContext(configuration);
        எனில் (impl)
        {
            CV_LOG_INFO(வெற்று, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            திரும்பு impl;
        }

        impl = புதிய Impl(configuration);
        முயல்
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            திரும்பு impl;
        }
        பிடி (...)
        {
            delete impl;
            எறி;
        }
    }

    நிலையான Impl* findOrCreateContext(மாறிலி ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (வெறுமை*)d);
        Impl* impl = findContext(configuration);
        எனில் (impl)
        {
            CV_LOG_INFO(வெற்று, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            திரும்பு impl;
        }

        impl = புதிய Impl(configuration);
        முயல்
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            திரும்பு impl;
        }
        பிடி (...)
        {
            delete impl;
            எறி;
        }
    }

    வெறுமை setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        எனில் (d == வெற்று)
            திரும்பு;

        createFromDevice(d);
    }

    வெறுமை createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == வெற்று);

        cl_platform_id pl = வெற்று;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, அளவு(cl_platform_id), &pl, வெற்று));

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

        தருக்க ok = handle != 0 && status == CL_SUCCESS;
        எனில்( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        இல்லையெனில்
            handle = வெற்று;
    }

    Program getProg(மாறிலி ProgramSource& src, மாறிலி String& buildflags, String& errmsg);

    வெறுமை unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        க்கு (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              எனில் (it != phash.end())
              {
                  எனில் (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      திரும்பு;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        எனில் (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            எனில் (prefix.empty())
            {
                CV_Assert(!devices.empty());
                மாறிலி Device& d = devices[0];
                முழு bits = d.addressBits();
                எனில் (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                க்கு (size_t i = 0; i < prefix.size(); i++)
                {
                    எழுத்து c = prefix[i];
                    எனில் (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        திரும்பு prefix;
    }

    std::string& getPrefixBase()
    {
        எனில் (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            எனில் (prefix_base.empty())
            {
                மாறிலி Device& d = devices[0];
                முழு bits = d.addressBits();
                எனில் (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                க்கு (size_t i = 0; i < prefix_base.size(); i++)
                {
                    எழுத்து c = prefix_base[i];
                    எனில் (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        திரும்பு prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    மாறிலி முழு contextId;  // global unique ID
    மாறிலி std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    வகைவரை std::map<std::string, Program> phash_t;
    phash_t phash;
    வகைவரை std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() மாறிலி
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        திரும்பு *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() மாறிலி
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        திரும்பு *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    வெறுமை setUserContext(std::type_index typeId, மாறிலி std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        எனில் (it != userContextStorage.end())
            திரும்பு it->second;
        இல்லையெனில்
            திரும்பு nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    தருக்க svmInitialized;
    தருக்க svmAvailable;
    தருக்க svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    வெறுமை svmInit()
    {
        CV_Assert(handle != வெற்று);
        மாறிலி Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((வெறுமை)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, அளவு(deviceCaps), &deviceCaps, வெற்று);
        எனில் (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            செல் noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (முழு)deviceCaps);
        CV_Assert(((வெறுமை)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.மதிப்பு_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.மதிப்பு_2 &= svm::getSVMCapabilitiesMask();
        எனில் (svmCapabilities.மதிப்பு_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            செல் noSVM;
        }
        முயல்
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            வெறுமை* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            எனில் (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            முயல்
            {
                தருக்க error = பொய்;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                எனில் (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, வெற்று, வெற்று))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                முயல்
                {
                    ((முழு*)ptr)[0] = 100;
                }
                பிடி (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = உண்மை;
                }
                எனில் (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, வெற்று, வெற்று))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                எனில் (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            பிடி (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                எறி;
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
        பிடி (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            முயல்
            {
                // Try HSA extension
                String extensions = device.extensions();
                எனில் (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    செல் noSVM;
                }
                cl_platform_id p = வெற்று;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, அளவு(cl_platform_id), &p, வெற்று));
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
            பிடி (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                செல் noSVM;
            }
        }

        svmAvailable = உண்மை;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = உண்மை;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        திரும்பு;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = பொய்;
        svmEnabled = பொய்;
        svmCapabilities.மதிப்பு_2 = 0;
        svmInitialized = உண்மை;
        svmFunctions.fn_clSVMAlloc = வெற்று;
        திரும்பு;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() மாறிலி
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        திரும்பு *bufferPoolSVM_.get();
    }
#endif

    friend வகுப்பு Program;
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
Context::Context(முழு dtype)
{
    p = 0;
    create(dtype);
}

வெறுமை Context::release()
{
    எனில் (p)
    {
        p->release();
        p = வெற்று;
    }
}

தருக்க Context::create()
{
    release();
    எனில் (!haveOpenCL())
        திரும்பு பொய்;
    p = Impl::findOrCreateContext(std::string());
    எனில் (p && p->handle)
        திரும்பு உண்மை;
    release();
    திரும்பு பொய்;
}

// deprecated
தருக்க Context::create(முழு dtype)
{
    எனில்( !haveOpenCL() )
        திரும்பு பொய்;
    release();
    எனில் (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    இல்லையெனில் எனில் (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    இல்லையெனில் எனில் (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    இல்லையெனில்
    {
        CV_LOG_ERROR(வெற்று, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    எனில் (p && !p->handle)
    {
        release();
    }
    திரும்பு p != 0;
}

Context::Context(மாறிலி Context& c)
{
    p = (Impl*)c.p;
    எனில்(p)
        p->addref();
}

Context& Context::operator = (மாறிலி Context& c)
{
    Impl* newp = (Impl*)c.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    எனில் (இது != &c) {
        எனில்(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    திரும்பு *இது;
}

வெறுமை* Context::ptr() மாறிலி
{
    திரும்பு p == வெற்று ? வெற்று : p->handle;
}

size_t Context::ndevices() மாறிலி
{
    திரும்பு p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) மாறிலி
{
    நிலையான Device dummy;
    திரும்பு !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    எனில் (!c.empty())
    {
        auto& ctx = c.getContext();
        திரும்பு ctx;
    }

    நிலையான Context dummy;
    திரும்பு dummy;
}

Program Context::getProg(மாறிலி ProgramSource& prog,
                         மாறிலி String& buildopts, String& errmsg)
{
    திரும்பு p ? p->getProg(prog, buildopts, errmsg) : Program();
}

வெறுமை Context::unloadProg(Program& prog)
{
    எனில் (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(வெறுமை* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    திரும்பு ctx;
}

/* static */
Context Context::fromDevice(மாறிலி ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    திரும்பு ctx;
}

/* static */
Context Context::create(மாறிலி std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    திரும்பு ctx;
}

வெறுமை* Context::getOpenCLContextProperty(முழு propertyId) மாறிலி
{
    எனில் (p == வெற்று)
        திரும்பு nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, வெற்று, &size));
    std::vector<cl_context_properties> prop(size / அளவு(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), வெற்று));
    க்கு (size_t i = 0; i < prop.size(); i += 2)
    {
        எனில் (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(வெற்று, "OpenCL: found context property=" << propertyId << ") => " << (வெறுமை*)prop[i + 1]);
            திரும்பு (வெறுமை*)prop[i + 1];
        }
    }
    திரும்பு nullptr;
}

#ifdef HAVE_OPENCL_SVM
தருக்க Context::useSVM() மாறிலி
{
    Context::Impl* i = p;
    CV_Assert(i);
    எனில் (!i->svmInitialized)
        i->svmInit();
    திரும்பு i->svmEnabled;
}
வெறுமை Context::setUseSVM(தருக்க enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    எனில் (!i->svmInitialized)
        i->svmInit();
    எனில் (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#இல்லையெனில்
தருக்க Context::useSVM() மாறிலி { திரும்பு பொய்; }
வெறுமை Context::setUseSVM(தருக்க enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
நாமாகாஶ svm {

மாறிலி SVMCapabilities getSVMCapabilitites(மாறிலி ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    எனில் (!i->svmInitialized)
        i->svmInit();
    திரும்பு i->svmCapabilities;
}

CV_EXPORTS மாறிலி SVMFunctions* getSVMFunctions(மாறிலி ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != வெற்று);
    திரும்பு &i->svmFunctions;
}

CV_EXPORTS தருக்க useSVM(UMatUsageFlags usageFlags)
{
    எனில் (checkForceSVMUmatUsage())
        திரும்பு உண்மை;
    எனில் (checkDisableSVMUMatUsage())
        திரும்பு பொய்;
    எனில் ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        திரும்பு உண்மை;
    திரும்பு பொய்; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

வெறுமை Context::setUserContext(std::type_index typeId, மாறிலி std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    திரும்பு p->getUserContext(typeId);
}

நிலையான வெறுமை get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<எழுத்து> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
வெறுமை attachContext(மாறிலி String& platformName, வெறுமை* platformID, வெறுமை* context, வெறுமை* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        மாறிலி std::string& platformName, வெறுமை* platformID, வெறுமை* context, வெறுமை* deviceID
)
{
    எனில் (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    எனில் (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    தருக்க platformAvailable = பொய்;

    // check if external platformName contained in list of available platforms in OpenCV
    க்கு (unsigned முழு i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        எனில் (platformName == availablePlatformName)
        {
            platformAvailable = உண்மை;
            நிறுத்து;
        }
    }

    எனில் (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    எனில் (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    திரும்பு ctx;
}

வெறுமை initializeContextFromHandle(Context& ctx, வெறுமை* _platform, வெறுமை* _context, வெறுமை* _device)
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

கட்டமைப்பு Queue::Impl
{
    உள்வரி வெறுமை __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = பொய்;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, அளவு(cl_command_queue_properties), &props, வெற்று));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, தருக்க isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(மாறிலி Context& c, மாறிலி Device& d, தருக்க withProfiling = பொய்)
    {
        __init();

        மாறிலி Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        எனில்( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        எனில்( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        எனில் (!cv::__termination)
#endif
        {
            எனில்(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = வெற்று;
            }
        }
    }

    மாறிலி cv::ocl::Queue& getProfilingQueue(மாறிலி cv::ocl::Queue& self)
    {
        எனில் (isProfilingQueue_)
            திரும்பு self;

        எனில் (profiling_queue_.ptr())
            திரும்பு profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, அளவு(cl_context), &ctx, வெற்று));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, அளவு(cl_device_id), &device, வெற்று));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = புதிய Impl(q, உண்மை);
        profiling_queue_ = queue;

        திரும்பு profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    தருக்க isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(மாறிலி Context& c, மாறிலி Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(மாறிலி Queue& q)
{
    p = q.p;
    எனில்(p)
        p->addref();
}

Queue& Queue::operator = (மாறிலி Queue& q)
{
    Impl* newp = (Impl*)q.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    எனில் (இது != &q) {
        எனில்(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    திரும்பு *இது;
}

Queue::~Queue()
{
    எனில்(p)
        p->release();
}

தருக்க Queue::create(மாறிலி Context& c, மாறிலி Device& d)
{
    எனில்(p)
        p->release();
    p = புதிய Impl(c, d);
    திரும்பு p->handle != 0;
}

வெறுமை Queue::finish()
{
    எனில்(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

மாறிலி Queue& Queue::getProfilingQueue() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->getProfilingQueue(*இது);
}

வெறுமை* Queue::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    எனில் (!c.empty())
    {
        auto& q = c.getQueue();
        திரும்பு q;
    }
    நிலையான Queue dummy;
    திரும்பு dummy;
}

நிலையான cl_command_queue getQueue(மாறிலி Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    எனில்(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    திரும்பு qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(முழு _flags, UMat* _m, முழு _wscale, முழு _iwscale, மாறிலி வெறுமை* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != வெற்று);
}

KernelArg KernelArg::Constant(மாறிலி Mat& m)
{
    CV_Assert(m.isContinuous());
    திரும்பு KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

கட்டமைப்பு Kernel::Impl
{
    Impl(மாறிலி எழுத்து* kname, மாறிலி Program& prog) :
        refcount(1), handle(வெற்று), isInProgress(பொய்), isAsyncRun(பொய்), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        எனில் (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        க்கு( முழு i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = பொய்;
        haveTempSrcUMats = பொய்;
    }

    வெறுமை cleanupUMats()
    {
        தருக்க exceptionOccurred = பொய்;
        க்கு( முழு i = 0; i < MAX_ARRS; i++ )
        {
            எனில்( u[i] )
            {
                எனில்( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    முயல்
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    பிடி(மாறிலி std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(வெற்று, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = உண்மை;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = பொய்;
        haveTempSrcUMats = பொய்;
        CV_Assert(!exceptionOccurred);
    }

    வெறுமை addUMat(மாறிலி UMat& m, தருக்க dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        எனில்(dst && m.u->tempUMat())
            haveTempDstUMats = உண்மை;
        எனில்(m.u->originalUMatData == வெற்று && m.u->tempUMat())
            haveTempSrcUMats = உண்மை;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    வெறுமை registerImageArgument(முழு arg, மாறிலி Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        எனில் (arg < (முழு)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    வெறுமை finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = பொய்;
        முயல்
        {
            cleanupUMats();
        }
        பிடி(...)
        {
            release();
            எறி;
        }
        release();
    }

    தருக்க run(முழு dims, size_t _globalsize[], size_t _localsize[],
            தருக்க ஒத்திசை, int64* timeNS, மாறிலி Queue& q);

    ~Impl()
    {
        எனில்(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    எண்ணிக்கை { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    தருக்க isInProgress;
    தருக்க isAsyncRun;  // true if kernel was scheduled in async mode
    முழு nu;
    std::vector<Image2D> shadow_images;
    தருக்க haveTempDstUMats;
    தருக்க haveTempSrcUMats;
};

}} // namespace cv::ocl

வெளி "C" {

நிலையான வெறுமை CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, வெறுமை *p)
{
    முயல்
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    பிடி (மாறிலி cv::Exception& exc)
    {
        CV_LOG_ERROR(வெற்று, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    பிடி (மாறிலி std::exception& exc)
    {
        CV_LOG_ERROR(வெற்று, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    பிடி (...)
    {
        CV_LOG_ERROR(வெற்று, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

நாமாகாஶ cv { நாமாகாஶ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(மாறிலி எழுத்து* kname, மாறிலி Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(மாறிலி எழுத்து* kname, மாறிலி ProgramSource& src,
               மாறிலி String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(மாறிலி Kernel& k)
{
    p = k.p;
    எனில்(p)
        p->addref();
}

Kernel& Kernel::operator = (மாறிலி Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    எனில் (இது != &k) {
        எனில்(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    திரும்பு *இது;
}

Kernel::~Kernel()
{
    எனில்(p)
        p->release();
}

தருக்க Kernel::create(மாறிலி எழுத்து* kname, மாறிலி Program& prog)
{
    எனில்(p)
        p->release();
    p = புதிய Impl(kname, prog);
    எனில்(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    திரும்பு p != 0;
}

தருக்க Kernel::create(மாறிலி எழுத்து* kname, மாறிலி ProgramSource& src,
                    மாறிலி String& buildopts, String* errmsg)
{
    எனில்(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    எனில்( !errmsg ) errmsg = &tempmsg;
    மாறிலி Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    திரும்பு create(kname, prog);
}

வெறுமை* Kernel::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

தருக்க Kernel::empty() மாறிலி
{
    திரும்பு ptr() == 0;
}

நிலையான cv::String dump_மதிப்பு(size_t sz, மாறிலி வெறுமை* p)
{
    எனில் (!p)
        திரும்பு "NULL";
    எனில் (sz == 2)
        திரும்பு cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    எனில் (sz == 4)
        திரும்பு cv::format("%d / %uu / 0x%08x / %g", *(முழு*)p, *(முழு*)p, *(முழு*)p, *(தசம*)p);
    எனில் (sz == 8)
        திரும்பு cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(இரட்டை*)p);
    திரும்பு cv::format("%p", p);
}

முழு Kernel::set(முழு i, மாறிலி வெறுமை* value, size_t sz)
{
    எனில் (!p || !p->handle)
        திரும்பு -1;
    எனில் (i < 0)
        திரும்பு i;
    எனில்( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (முழு)i, (முழு)sz, dump_மதிப்பு(sz, value).c_str()).c_str());
    எனில் (retval != CL_SUCCESS)
        திரும்பு -1;
    திரும்பு i+1;
}

முழு Kernel::set(முழு i, மாறிலி Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    முழு res = set(i, &h, அளவு(h));
    எனில் (res >= 0)
        p->registerImageArgument(i, image2D);
    திரும்பு res;
}

முழு Kernel::set(முழு i, மாறிலி UMat& m)
{
    திரும்பு set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
நிலையான தருக்க isCudaBacked(மாறிலி UMatData* u)
{
    மாறிலி MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    திரும்பு u && cudaAllocator && u->currAllocator == cudaAllocator;
}

முழு Kernel::set(முழு i, மாறிலி KernelArg& arg)
{
    எனில்( !p || !p->handle )
        திரும்பு -1;
    எனில் (i < 0)
    {
        CV_LOG_ERROR(வெற்று, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (முழு)i));
        திரும்பு i;
    }
    எனில்( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    எனில்( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        தருக்க ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        எனில் (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)வெற்று;
            status = clSetKernelArg(p->handle, (cl_uint)i, அளவு(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (முழு)i).c_str());
            திரும்பு i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        எனில் (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(வெற்று, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (முழு)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            திரும்பு -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        எனில் (!h)
        {
            CV_LOG_ERROR(வெற்று, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (முழு)i, (முழு)arg.flags, arg.m));
            p->release();
            p = 0;
            திரும்பு -1;
        }

#ifdef HAVE_OPENCL_SVM
        எனில் ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            மாறிலி Context& ctx = Context::getDefault();
            மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#எனில் 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#இல்லையெனில்
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (முழு)i, (வெறுமை*)svmDataPtr).c_str());
        }
        இல்லையெனில்
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, அளவு(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (முழு)i, (வெறுமை*)h).c_str());
        }

        எனில் (ptronly)
        {
            i++;
        }
        இல்லையெனில் எனில்( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), அளவு(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (முழு)(i+1), (முழு)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), அளவு(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (முழு)(i+2), (முழு)u2d.offset).c_str());
            i += 3;

            எனில்( !(arg.flags & KernelArg::NO_SIZE) )
            {
                முழு cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, அளவு(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (முழு)i, (முழு)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), அளவு(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (முழு)(i+1), (முழு)cols).c_str());
                i += 2;
            }
        }
        இல்லையெனில்
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), அளவு(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (முழு)(i+1), (முழு)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), அளவு(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (முழு)(i+2), (முழு)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), அளவு(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (முழு)(i+3), (முழு)u3d.offset).c_str());
            i += 4;
            எனில்( !(arg.flags & KernelArg::NO_SIZE) )
            {
                முழு cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, அளவு(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (முழு)i, (முழு)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), அளவு(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (முழு)(i+1), (முழு)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), அளவு(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (முழு)(i+2), (முழு)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        திரும்பு i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (முழு)i, (முழு)arg.sz, (வெறுமை*)arg.obj).c_str());
    திரும்பு i+1;
}

தருக்க Kernel::run(முழு dims, size_t _globalsize[], size_t _localsize[],
                 தருக்க ஒத்திசை, மாறிலி Queue& q)
{
    எனில் (!p)
        திரும்பு பொய்;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != வெற்று);
    க்கு (முழு i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(முழு)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        எனில் (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned முழு)val) * val;
    }
    CV_Assert(total > 0);

    திரும்பு p->run(dims, globalsize, _localsize, ஒத்திசை, வெற்று, q);
}


தருக்க Kernel::run_(முழு dims, size_t _globalsize[], size_t _localsize[],
                  தருக்க ஒத்திசை, மாறிலி Queue& q)
{
    CV_Assert(p);
    திரும்பு p->run(dims, _globalsize, _localsize, ஒத்திசை, வெற்று, q);
}


தருக்க Kernel::Impl::run(முழு dims, size_t globalsize[], size_t localsize[],
        தருக்க ஒத்திசை, int64* timeNS, மாறிலி Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    எனில் (!handle)
    {
        CV_LOG_ERROR(வெற்று, "OpenCL kernel has zero handle: " << name);
        திரும்பு பொய்;
    }

    எனில் (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !ஒத்திசை;

    எனில் (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#எனில் CV_OPENCL_SYNC_RUN_KERNELS
    ஒத்திசை = உண்மை;
#endif

    cl_command_queue qq = getQueue(q);
    எனில் (haveTempDstUMats)
        ஒத்திசை = உண்மை;
    எனில் (haveTempSrcUMats)
        ஒத்திசை = உண்மை;
    எனில் (timeNS)
        ஒத்திசை = உண்மை;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           வெற்று, globalsize, localsize, 0, 0,
                                           (ஒத்திசை && !timeNS) ? 0 : &asyncEvent);
#எனில் !CV_OPENCL_SHOW_RUN_KERNELS
    எனில் (retval != CL_SUCCESS)
#endif
    {
        cv::String செய்தி = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (முழு)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ஒத்திசை ? "true" : "false"
                        );
        எனில் (retval != CL_SUCCESS)
        {
            செய்தி = CV_OCL_API_ERROR_செய்தி(retval, செய்தி.c_str());
        }
#எனில் CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, செய்தி.c_str());
#இல்லையெனில்
        printf("%s\n", செய்தி.c_str());
        fflush(stdout);
#endif
    }
    எனில் (ஒத்திசை || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        எனில் (timeNS)
        {
            எனில் (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, அளவு(startTime), &startTime, வெற்று));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, அளவு(stopTime), &stopTime, வெற்று));
                *timeNS = (int64)(stopTime - startTime);
            }
            இல்லையெனில்
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    இல்லையெனில்
    {
        addref();
        isInProgress = உண்மை;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, இது));
    }
    எனில் (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    திரும்பு retval == CL_SUCCESS;
}

தருக்க Kernel::runTask(தருக்க ஒத்திசை, மாறிலி Queue& q)
{
    எனில்(!p || !p->handle || p->isInProgress)
        திரும்பு பொய்;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, ஒத்திசை ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ஒத்திசை ? "true" : "false").c_str());
    எனில் (ஒத்திசை || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    இல்லையெனில்
    {
        p->addref();
        p->isInProgress = உண்மை;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    எனில் (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    திரும்பு retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(முழு dims, size_t globalsize[], size_t localsize[], மாறிலி Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    தருக்க res = p->run(dims, globalsize, localsize, உண்மை, &timeNs, profilingQueue);
    திரும்பு res ? timeNs : -1;
}

size_t Kernel::workGroupSize() மாறிலி
{
    எனில்(!p || !p->handle)
        திரும்பு 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, அளவு(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    திரும்பு status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() மாறிலி
{
    எனில்(!p || !p->handle)
        திரும்பு 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, அளவு(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    திரும்பு status == CL_SUCCESS ? val : 0;
}

தருக்க Kernel::compileWorkGroupSize(size_t wsz[]) மாறிலி
{
    எனில்(!p || !p->handle || !wsz)
        திரும்பு 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, அளவு(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    திரும்பு status == CL_SUCCESS;
}

size_t Kernel::localMemSize() மாறிலி
{
    எனில்(!p || !p->handle)
        திரும்பு 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, அளவு(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    திரும்பு status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

கட்டமைப்பு ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    எண்ணிக்கை KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(மாறிலி String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(மாறிலி String& module, மாறிலி String& name, மாறிலி String& codeStr, மாறிலி String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    வெறுமை init(எண்ணிக்கை KIND kind, மாறிலி String& module, மாறிலி String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = வெற்று;
        sourceSize_ = 0;
        isHashUpdated = பொய்;
    }

    வெறுமை initFromSource(மாறிலி String& codeStr, மாறிலி String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        எனில் (sourceHash_.empty())
        {
            updateHash();
        }
        இல்லையெனில்
        {
            isHashUpdated = உண்மை;
        }
    }

    வெறுமை updateHash(மாறிலி எழுத்து* hashStr = வெற்று)
    {
        எனில் (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = உண்மை;
            திரும்பு;
        }
        uint64 hash = 0;
        தேர்வு (kind_)
        {
        நிலை PROGRAM_SOURCE_CODE:
            எனில் (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            இல்லையெனில்
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            நிறுத்து;
        நிலை PROGRAM_BINARIES:
        நிலை PROGRAM_SPIR:
        நிலை PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            நிறுத்து;
        இயல்பு:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = உண்மை;
    }

    Impl(எண்ணிக்கை KIND kind,
            மாறிலி String& module, மாறிலி String& name,
            மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size,
            மாறிலி cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    நிலையான ProgramSource fromSourceWithStaticLifetime(மாறிலி String& module, மாறிலி String& name,
            மாறிலி எழுத்து* sourceCodeStaticStr, மாறிலி எழுத்து* hashStaticStr,
            மாறிலி cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = புதிய Impl(PROGRAM_SOURCE_CODE, module, name,
                (மாறிலி unsigned எழுத்து*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        திரும்பு result;
    }

    நிலையான ProgramSource from_இருமம்(மாறிலி String& module, மாறிலி String& name,
            மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size,
            மாறிலி cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = புதிய Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        திரும்பு result;
    }

    நிலையான ProgramSource fromSPIR(மாறிலி String& module, மாறிலி String& name,
            மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size,
            மாறிலி cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = புதிய Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        திரும்பு result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    மாறிலி unsigned எழுத்து* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    தருக்க isHashUpdated;

    friend கட்டமைப்பு Program::Impl;
    friend கட்டமைப்பு internal::ProgramEntry;
    friend கட்டமைப்பு Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(மாறிலி String& module, மாறிலி String& name, மாறிலி String& codeStr, மாறிலி String& codeHash)
{
    p = புதிய Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(மாறிலி எழுத்து* prog)
{
    p = புதிய Impl(prog);
}

ProgramSource::ProgramSource(மாறிலி String& prog)
{
    p = புதிய Impl(prog);
}

ProgramSource::~ProgramSource()
{
    எனில்(p)
        p->release();
}

ProgramSource::ProgramSource(மாறிலி ProgramSource& prog)
{
    p = prog.p;
    எனில்(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (மாறிலி ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    எனில் (இது != &prog) {
        எனில்(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    திரும்பு *இது;
}

மாறிலி String& ProgramSource::source() மாறிலி
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == வெற்று); // method returns reference - can't construct temporary object
    திரும்பு p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() மாறிலி
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_இருமம்(மாறிலி String& module, மாறிலி String& name,
        மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size,
        மாறிலி cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    திரும்பு Impl::from_இருமம்(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(மாறிலி String& module, மாறிலி String& name,
        மாறிலி unsigned எழுத்து* binary, மாறிலி size_t size,
        மாறிலி cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    திரும்பு Impl::from_இருமம்(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() மாறிலி
{
    எனில் (இது->pProgramSource == வெற்று)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        எனில் (இது->pProgramSource == வெற்று)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(இது->module, இது->name, இது->programCode, இது->programHash, cv::String());
            ProgramSource* ptr = புதிய ProgramSource(ps);
            const_cast<ProgramEntry*>(இது)->pProgramSource = ptr;
        }
    }
    திரும்பு *இது->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

நிலையான
cv::String joinBuildOptions(மாறிலி cv::String& a, மாறிலி cv::String& b)
{
    எனில் (b.empty())
        திரும்பு a;
    எனில் (a.empty())
        திரும்பு b;
    எனில் (b[0] == ' ')
        திரும்பு a + b;
    திரும்பு a + (cv::String(" ") + b);
}

கட்டமைப்பு Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(மாறிலி ProgramSource& src,
         மாறிலி String& _buildflags, String& errmsg) :
         refcount(1),
         handle(வெற்று),
         buildflags(_buildflags)
    {
        மாறிலி ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        மாறிலி Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        எனில் (ctx.ptr() == வெற்று || device.ptr() == வெற்று)
            திரும்பு;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        எனில் (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            எனில் (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            இல்லையெனில் எனில் (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            மாறிலி String param_buildExtraOptions = getBuildExtraOptions();
            எனில் (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#எனில் CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(வெற்று, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#எனில் CV_OPENCL_SHOW_BUILD_KERNELS
        எனில் (handle)
        {
            size_t retsz = 0;
            எழுத்து kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, அளவு(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            எனில் (result == CL_SUCCESS && retsz < அளவு(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(வெற்று, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            இல்லையெனில்
            {
                CV_LOG_ERROR(வெற்று, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    தருக்க compile(மாறிலி Context& ctx, மாறிலி ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        எனில் (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(வெற்று, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            தருக்க isLoaded = create_From_இருமம்(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            திரும்பு isLoaded;
        }
        திரும்பு compileWithCache(ctx, src_, errmsg);
    }

    தருக்க compileWithCache(மாறிலி Context& ctx, மாறிலி ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        மாறிலி std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        மாறிலி String& hash_str = src_->sourceHash_;
        cv::String fname;
        எனில் (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        மாறிலி cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        எனில் (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            முயல்
            {
                std::vector<எழுத்து> இருமம்_Buf;
                தருக்க res = பொய்;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    இருமம்_Program_File file(fname, hash_str.c_str());
                    res = file.பட(buildflags, இருமம்_Buf);
                }
                எனில் (res)
                {
                    CV_Assert(!இருமம்_Buf.empty());
                    CV_LOG_VERBOSE(வெற்று, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    தருக்க isLoaded = create_From_இருமம்(ctx, இருமம்_Buf, errmsg);
                    எனில் (isLoaded)
                        திரும்பு உண்மை;
                }
            }
            பிடி (மாறிலி cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(வெற்று, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            பிடி (...)
            {
                CV_LOG_VERBOSE(வெற்று, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == வெற்று);
        எனில் (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            எனில் (!buildFromSources(ctx, src_, errmsg))
            {
                திரும்பு பொய்;
            }
        }
        இல்லையெனில் எனில் (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            எனில் ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(வெற்று, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            தருக்க isLoaded = create_From_இருமம்(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            எனில் (!isLoaded)
                திரும்பு பொய்;
        }
        இல்லையெனில் எனில் (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        இல்லையெனில்
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != வெற்று);
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
        எனில் (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            முயல்
            {
                std::vector<எழுத்து> இருமம்_Buf;
                get_Program_இருமம்(இருமம்_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    இருமம்_Program_File file(fname, hash_str.c_str());
                    file.லிக(buildflags, இருமம்_Buf);
                }
            }
            பிடி (மாறிலி cv::Exception& e)
            {
                CV_LOG_WARNING(வெற்று, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            பிடி (...)
            {
                CV_LOG_WARNING(வெற்று, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#எனில் CV_OPENCL_VALIDATE_இருமம்_PROGRAMS
        எனில் (CV_OPENCL_VALIDATE_இருமம்_PROGRAMS_மதிப்பு)
        {
            std::vector<எழுத்து> இருமம்_Buf;
            get_Program_இருமம்(இருமம்_Buf);
            எனில் (!இருமம்_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = வெற்று;
                create_From_இருமம்(ctx, இருமம்_Buf, errmsg);
            }
        }
#endif
        திரும்பு handle != வெற்று;
    }

    வெறுமை dumpBuildLog_(cl_int result, மாறிலி cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<எழுத்து, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        எனில் (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            எனில் (log_retval == CL_SUCCESS)
            {
                எனில் (retsz < buffer.size())
                    buffer[retsz] = 0;
                இல்லையெனில்
                    buffer[buffer.size() - 1] = 0;
            }
            இல்லையெனில்
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

    தருக்க buildFromSources(மாறிலி Context& ctx, மாறிலி ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == வெற்று);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(வெற்று, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        மாறிலி எழுத்து* srcptr = src_->sourceAddr_ ? ((மாறிலி எழுத்து*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != வெற்று);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        எனில் (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            க்கு (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#எனில் !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            எனில் (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                எனில் (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = வெற்று;
                }
                எனில் (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#எனில் CV_OPENCL_VALIDATE_இருமம்_PROGRAMS
            எனில் (handle && CV_OPENCL_VALIDATE_இருமம்_PROGRAMS_மதிப்பு)
            {
                CV_LOG_INFO(வெற்று, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                எழுத்து kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, அளவு(kernels_buffer), &kernels_buffer[0], &retsz);
                எனில் (retsz < அளவு(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                இல்லையெனில்
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(வெற்று, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        திரும்பு handle != வெற்று;
    }

    வெறுமை get_Program_இருமம்(std::vector<எழுத்து>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_இருமம்_SIZES, அளவு(sz), &sz, வெற்று));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, அளவு(ptr), &ptr, வெற்று));
    }

    தருக்க create_From_இருமம்(மாறிலி Context& ctx, மாறிலி std::vector<எழுத்து>& buf, String& errmsg)
    {
        திரும்பு create_From_இருமம்(ctx, (மாறிலி unsigned எழுத்து*)&buf[0], buf.size(), errmsg);
    }

    தருக்க create_From_இருமம்(மாறிலி Context& ctx, மாறிலி unsigned எழுத்து* இருமம்_Addr, மாறிலி size_t இருமம்_Size, String& errmsg)
    {
        CV_Assert(handle == வெற்று);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(வெற்று, 0, "Load from binary... (" << இருமம்_Size << " bytes)");

        CV_Assert(இருமம்_Size > 0);

        size_t ndevices = (முழு)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<மாறிலி uchar*> இருமம்_Ptrs(ndevices);
        AutoBuffer<size_t> இருமம்_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        மாறிலி uchar** இருமம்_Ptrs_2 = இருமம்_Ptrs.data();
        size_t* இருமம்_Sizes_2 = இருமம்_Sizes.data();
        க்கு (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            இருமம்_Ptrs_2[i] = இருமம்_Addr;
            இருமம்_Sizes_2[i] = இருமம்_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_இருமம்((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           இருமம்_Sizes_2, இருமம்_Ptrs_2, வெற்று, &result);
        எனில் (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(வெற்று, CV_OCL_API_ERROR_செய்தி(result, "clCreateProgramWithBinary"));
            எனில் (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = வெற்று;
            }
        }
        எனில் (!handle)
        {
            திரும்பு பொய்;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            எனில் (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                எனில் (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = வெற்று;
                }
                திரும்பு பொய்;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    அளவு(build_status), &build_status, &retsz));
            எனில் (result == CL_SUCCESS)
            {
                எனில் (build_status == CL_BUILD_SUCCESS)
                {
                    திரும்பு உண்மை;
                }
                இல்லையெனில்
                {
                    CV_LOG_WARNING(வெற்று, "clGetProgramBuildInfo() returns " << build_status);
                    திரும்பு பொய்;
                }
            }
            இல்லையெனில்
            {
                CV_LOG_ERROR(வெற்று, CV_OCL_API_ERROR_செய்தி(result, "clGetProgramBuildInfo()"));
                எனில் (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = வெற்று;
                }
            }
        }
#எனில் CV_OPENCL_VALIDATE_இருமம்_PROGRAMS
        எனில் (handle && CV_OPENCL_VALIDATE_இருமம்_PROGRAMS_மதிப்பு)
        {
            CV_LOG_INFO(வெற்று, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            எழுத்து kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, அளவு(kernels_buffer), &kernels_buffer[0], &retsz);
            எனில் (retsz < அளவு(kernels_buffer))
                kernels_buffer[retsz] = 0;
            இல்லையெனில்
                kernels_buffer[0] = 0;
            CV_LOG_INFO(வெற்று, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        திரும்பு handle != வெற்று;
    }

    ~Impl()
    {
        எனில்( handle )
        {
#ifdef _WIN32
            எனில் (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = வெற்று;
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

Program::Program(மாறிலி ProgramSource& src,
        மாறிலி String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(மாறிலி Program& prog)
{
    p = prog.p;
    எனில்(p)
        p->addref();
}

Program& Program::operator = (மாறிலி Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    எனில்(newp)
        newp->addref();
    எனில்(p)
        p->release();
    p = newp;
    திரும்பு *இது;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    எனில் (இது != &prog) {
        எனில்(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    திரும்பு *இது;
}

Program::~Program()
{
    எனில்(p)
        p->release();
}

தருக்க Program::create(மாறிலி ProgramSource& src,
            மாறிலி String& buildflags, String& errmsg)
{
    எனில்(p)
    {
        p->release();
        p = வெற்று;
    }
    p = புதிய Impl(src, buildflags, errmsg);
    எனில்(!p->handle)
    {
        p->release();
        p = 0;
    }
    திரும்பு p != 0;
}

வெறுமை* Program::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
மாறிலி ProgramSource& Program::source() மாறிலி
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

தருக்க Program::பட(மாறிலி String& bin, மாறிலி String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

தருக்க Program::லிக(String& bin) மாறிலி
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() மாறிலி
{
    எனில்(!p)
        திரும்பு String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    திரும்பு cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(மாறிலி String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        திரும்பு cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

வெறுமை Program::get_இருமம்(std::vector<எழுத்து>& binary) மாறிலி
{
    CV_Assert(p && "Empty program");
    p->get_Program_இருமம்(binary);
}

Program Context::Impl::getProg(மாறிலி ProgramSource& src,
                               மாறிலி String& buildflags, String& errmsg)
{
    size_t limit = get_Program_மொத்தம்_Limit();
    மாறிலி ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String சாவி = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(சாவி);
        எனில் (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), சாவி);
            எனில் (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(சாவி);
            }
            திரும்பு it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            எனில் (limit > 0 && sz >= limit)
            {
                நிலையான தருக்க warningFlag = பொய்;
                எனில் (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = உண்மை;
                }
                போது (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    எனில் (c != 0)
                        நிறுத்து;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(சாவி, prog));
        cacheList.push_front(சாவி);
    }
    திரும்பு prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
வகுப்பு OpenCLBufferPool
{
பாதுகாக்கப்பட்ட:
    ~OpenCLBufferPool() { }
பொது:
    மெய்நிகர் T allocate(size_t size) = 0;
    மெய்நிகர் வெறுமை release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
வகுப்பு OpenCLBufferPoolBaseImpl : பொது BufferPoolController, பொது OpenCLBufferPool<T>
{
தனிப்பட்ட:
    உள்வரி Derived& derived() { திரும்பு *static_cast<Derived*>(இது); }
பாதுகாக்கப்பட்ட:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    தருக்க _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        க்கு (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            எனில் (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                திரும்பு உண்மை;
            }
        }
        திரும்பு பொய்;
    }

    // synchronized
    தருக்க _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, மாறிலி size_t size)
    {
        எனில் (reservedEntries_.empty())
            திரும்பு பொய்;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        க்கு (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            எனில் (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                எனில் (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    எனில் (diff == 0)
                        நிறுத்து;
                }
            }
        }
        எனில் (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            திரும்பு உண்மை;
        }
        திரும்பு பொய்;
    }

    // synchronized
    வெறுமை _checkSizeOfReservedEntries()
    {
        போது (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            மாறிலி BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    உள்வரி size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        எனில் (size < 1024*1024)
            திரும்பு 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        இல்லையெனில் எனில் (size < 16*1024*1024)
            திரும்பு 64*1024;
        இல்லையெனில்
            திரும்பு 1024*1024;
    }

பொது:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    மெய்நிகர் ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
பொது:
    மெய்நிகர் T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        எனில் (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        இல்லையெனில்
        {
            derived()._allocateBufferEntry(entry, size);
        }
        திரும்பு entry.clBuffer_;
    }
    மெய்நிகர் வெறுமை release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        எனில் (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        இல்லையெனில்
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    மெய்நிகர் size_t getReservedSize() மாறிலி CV_OVERRIDE { திரும்பு currentReservedSize; }
    மெய்நிகர் size_t getMaxReservedSize() மாறிலி CV_OVERRIDE { திரும்பு maxReservedSize; }
    மெய்நிகர் வெறுமை setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        எனில் (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            க்கு (; i != reservedEntries_.end();)
            {
                மாறிலி BufferEntry& entry = *i;
                எனில் (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    தொடர்;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    மெய்நிகர் வெறுமை freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        க்கு (; i != reservedEntries_.end(); ++i)
        {
            மாறிலி BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

கட்டமைப்பு CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)வெற்று), capacity_(0) { }
};

வகுப்பு OpenCLBufferPoolImpl CV_FINAL : பொது OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
பொது:
    வகைவரை கட்டமைப்பு CLBufferEntry BufferEntry;
பாதுகாக்கப்பட்ட:
    முழு createFlags_;
பொது:
    OpenCLBufferPoolImpl(முழு createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    வெறுமை _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == வெற்று);
        entry.capacity_ = alignSize(size, (முழு)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long முழு)entry.capacity_, (வெறுமை*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != வெற்று);
        எனில்(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    வெறுமை _releaseBufferEntry(மாறிலி BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != வெற்று);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
கட்டமைப்பு CLSVMBufferEntry
{
    வெறுமை* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(வெற்று), capacity_(0) { }
};
வகுப்பு OpenCLSVMBufferPoolImpl CV_FINAL : பொது OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, வெறுமை*>
{
பொது:
    வகைவரை கட்டமைப்பு CLSVMBufferEntry BufferEntry;
பொது:
    OpenCLSVMBufferPoolImpl()
    {
    }

    வெறுமை _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == வெற்று);
        entry.capacity_ = alignSize(size, (முழு)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        மாறிலி svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        தருக்க isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (முழு)entry.capacity_);
        வெறுமை *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    வெறுமை _releaseBufferEntry(மாறிலி BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != வெற்று);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <தருக்க readAccess, தருக்க writeAccess>
வகுப்பு AlignedDataPtr
{
பாதுகாக்கப்பட்ட:
    மாறிலி size_t size_;
    uchar* மாறிலி originPtr_;
    மாறிலி size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

பொது:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(வெற்று)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        எனில் (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = புதிய uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            எனில் (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() மாறிலி
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        திரும்பு ptr_;
    }

    ~AlignedDataPtr()
    {
        எனில் (allocatedPtr_)
        {
            எனில் (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = வெற்று;
        }
        ptr_ = வெற்று;
    }
தனிப்பட்ட:
    AlignedDataPtr(மாறிலி AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(மாறிலி AlignedDataPtr&); // disabled
};

template <தருக்க readAccess, தருக்க writeAccess>
வகுப்பு AlignedDataPtr2D
{
பாதுகாக்கப்பட்ட:
    மாறிலி size_t size_;
    uchar* மாறிலி originPtr_;
    மாறிலி size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

பொது:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(வெற்று), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != வெற்று);
        எனில் (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = புதிய uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            எனில் (readAccess)
            {
                க்கு (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() மாறிலி
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        திரும்பு ptr_;
    }

    ~AlignedDataPtr2D()
    {
        எனில் (allocatedPtr_)
        {
            எனில் (writeAccess)
            {
                க்கு (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = வெற்று;
        }
        ptr_ = வெற்று;
    }
தனிப்பட்ட:
    AlignedDataPtr2D(மாறிலி AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(மாறிலி AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


வெறுமை Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(வெற்று, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

வகுப்பு OpenCLAllocator CV_FINAL : பொது MatAllocator
{
பொது:
    எண்ணிக்கை AllocatorFlags
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

    UMatData* defaultAllocate(முழு dims, மாறிலி முழு* sizes, முழு type, வெறுமை* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) மாறிலி
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        திரும்பு u;
    }

    நிலையான தருக்க isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        நிலையான தருக்க value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", பொய்);
        திரும்பு value;
    }
    நிலையான தருக்க isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        நிலையான தருக்க value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", பொய்);
        திரும்பு value;
    }

    வெறுமை getBestFlags(மாறிலி Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, முழு& createFlags, UMatData::MemoryFlag& flags0) மாறிலி
    {
        மாறிலி Device& dev = ctx.device(0);
        createFlags = 0;
        எனில் ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        எனில் (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        இல்லையெனில்
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(முழு dims, மாறிலி முழு* sizes, முழு type,
                       வெறுமை* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) மாறிலி CV_OVERRIDE
    {
        எனில்(!useOpenCL())
            திரும்பு defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        க்கு( முழு i = dims-1; i >= 0; i-- )
        {
            எனில்( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        எனில் (!ctx.getImpl())
            திரும்பு defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        முழு createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        வெறுமை* handle = வெற்று;
        முழு allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        மாறிலி svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        எனில் (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            தருக்க isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        இல்லையெனில்
#endif
        எனில் (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        இல்லையெனில் எனில் (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        இல்லையெனில்
        {
            CV_Assert(handle != வெற்று); // Unsupported, throw
        }

        எனில் (!handle)
            திரும்பு defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = புதிய UMatData(இது);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<வெறுமை>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(உண்மை);
        opencl_allocator_stats.onAllocate(u->size);
        திரும்பு u;
    }

    தருக்க allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு பொய்;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        எனில்(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            முழு createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            தருக்க copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            முழு allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            வெறுமை* handle = வெற்று;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            தருக்க useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            எனில் (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (முழு)u->size, handle);
            }
            இல்லையெனில் எனில் (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                எனில் (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    தருக்க isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (முழு)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = வெற்று;
                    எனில் (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (முழு)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, வெற்று, வெற்று);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    எனில் (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, வெற்று, வெற்று);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            இல்லையெனில்
#endif
            {
                எனில்( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                எனில் (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (முழு)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long முழு)u->size, u->origdata, (வெறுமை*)handle).c_str());
                }
                எனில்((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long முழு)u->size, u->origdata, (வெறுமை*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (வெறுமை*)handle).c_str());
            எனில்(!handle || retval != CL_SUCCESS)
                திரும்பு பொய்;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = இது;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        எனில் (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(உண்மை);
        opencl_allocator_stats.onAllocate(u->size);
        திரும்பு உண்மை;
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

    வெறுமை deallocate(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        எனில் (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        இல்லையெனில்
            deallocate_(u);
    }

    வெறுமை deallocate_(UMatData* u) மாறிலி
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        எனில் ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        எனில் (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            திரும்பு;             // avoid any OpenCL calls
#endif
        எனில்(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            எனில் (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    எனில்( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        தருக்க isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = வெற்று;
                        எனில் (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (முழு)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, வெற்று, வெற்று);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        எனில் (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, வெற்று, வெற்று);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    இல்லையெனில்
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                இல்லையெனில்
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    எனில்( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<பொய், உண்மை> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    இல்லையெனில்
                    {
                        cl_int retval = 0;
                        எனில் (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            வெறுமை* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (வெறுமை*)u->handle, (long long முழு)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            எனில் (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (வெறுமை*)u->handle, data, (long long முழு)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(பொய்);
            }
            இல்லையெனில்
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                எனில்( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            இல்லையெனில்
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (வெறுமை*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(உண்மை);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = வெற்று;
            எனில்(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = வெற்று;
        }
        இல்லையெனில்
        {
            CV_Assert(u->origdata == வெற்று);
            எனில்(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(உண்மை);
            }
            எனில் (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            இல்லையெனில் எனில் (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            இல்லையெனில் எனில் (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                இல்லையெனில் எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, வெற்று, வெற்று);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((வெறுமை*)u->handle);
            }
#endif
            இல்லையெனில்
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(உண்மை);
            delete u;
            u = வெற்று;
        }
        CV_Assert(u == வெற்று);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    வெறுமை map(UMatData* u, AccessFlag accessFlags) மாறிலி CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        எனில் (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(உண்மை);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            எனில்( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (முழு)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, வெற்று, வெற்று);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(பொய்);
                    u->markDeviceMemMapped(உண்மை);
                    திரும்பு;
                }
#endif

                cl_int retval = CL_SUCCESS;
                எனில் (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (வெறுமை*)u->handle, (long long முழு)u->size, u->data).c_str());
                }
                எனில் (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(பொய்);
                    u->markDeviceMemMapped(உண்மை);
                    திரும்பு;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            எனில்(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(உண்மை);
            }
        }

        எனில் (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<பொய், உண்மை> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (வெறுமை*)u->handle, (long long முழு)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(பொய்);
        }
    }

    வெறுமை unmap(UMatData* u) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        எனில்( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != வெற்று);
#ifdef HAVE_OPENCL_SVM
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, வெற்று, வெற்று);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                எனில் (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(பொய்);
                u->markHostCopyObsolete(உண்மை);
                திரும்பு;
            }
#endif
            எனில் (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (வெறுமை*)u->handle, u->data, (long long முழு)u->size).c_str());
                எனில் (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(பொய்);
                u->data = 0;
                u->markDeviceCopyObsolete(பொய்);
                u->markHostCopyObsolete(உண்மை);
            }
        }
        இல்லையெனில் எனில்( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<உண்மை, பொய்> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (வெறுமை*)u->handle, (long long முழு)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(பொய்);
            u->markHostCopyObsolete(உண்மை);
        }
    }

    தருக்க checkContinuous(முழு dims, மாறிலி size_t sz[],
                         மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
                         மாறிலி size_t dstofs[], மாறிலி size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) மாறிலி
    {
        தருக்க iscontinuous = உண்மை;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        க்கு( முழு i = dims-2; i >= 0; i-- )
        {
            எனில்( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = பொய்;
            total *= sz[i];
            எனில்( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            எனில்( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        எனில்( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            எனில்( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                எனில்( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                எனில்( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            இல்லையெனில்
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                எனில்( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                எனில்( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        திரும்பு iscontinuous;
    }

    வெறுமை download(UMatData* u, வெறுமை* dstptr, முழு dims, மாறிலி size_t sz[],
                  மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
                  மாறிலி size_t dststep[]) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு;
        UMatDataAutoLock autolock(u);

        எனில்( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            திரும்பு;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        தருக்க iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == வெற்று || u->data == u->handle);
            Context& ctx = Context::getDefault();
            மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (முழு)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, வெற்று, வெற்று);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            எனில்( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            இல்லையெனில்
            {
                // This code is from MatAllocator::download()
                முழு isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                க்கு( முழு i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    எனில்( sz[i] == 0 )
                    திரும்பு;
                    எனில்( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (முழு)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                மாறிலி Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                க்கு( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, வெற்று, வெற்று);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        இல்லையெனில்
#endif
        {
            எனில்( iscontinuous )
            {
                AlignedDataPtr<பொய், உண்மை> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            இல்லையெனில் எனில் (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                மாறிலி size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<பொய், பொய்> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                க்கு( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            இல்லையெனில்
            {
                AlignedDataPtr2D<பொய், உண்மை> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    வெறுமை upload(UMatData* u, மாறிலி வெறுமை* srcptr, முழு dims, மாறிலி size_t sz[],
                மாறிலி size_t dstofs[], மாறிலி size_t dststep[],
                மாறிலி size_t srcstep[]) மாறிலி CV_OVERRIDE
    {
        எனில்(!u)
            திரும்பு;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        தருக்க iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        எனில்( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(பொய்);
            u->markDeviceCopyObsolete(உண்மை);
            திரும்பு;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == வெற்று || u->data == u->handle);
            Context& ctx = Context::getDefault();
            மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (முழு)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, வெற்று, வெற்று);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            எனில்( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            இல்லையெனில்
            {
                // This code is from MatAllocator::upload()
                முழு isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                க்கு( முழு i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    எனில்( sz[i] == 0 )
                    திரும்பு;
                    எனில்( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (முழு)sz[i];
                }

                Mat src(dims, isz, CV_8U, (வெறுமை*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                மாறிலி Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                க்கு( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, வெற்று, வெற்று);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        இல்லையெனில்
#endif
        {
            எனில்( iscontinuous )
            {
                AlignedDataPtr<உண்மை, பொய்> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (வெறுமை*)u->handle, (long long முழு)dstrawofs, (long long முழு)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            இல்லையெனில் எனில் (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                மாறிலி size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<பொய், பொய்> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                க்கு( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            இல்லையெனில்
            {
                AlignedDataPtr2D<உண்மை, பொய்> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(உண்மை);
#ifdef HAVE_OPENCL_SVM
        எனில் ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        இல்லையெனில்
#endif
        {
            u->markHostCopyObsolete(உண்மை);
        }
        u->markDeviceCopyObsolete(பொய்);
    }

    வெறுமை copy(UMatData* src, UMatData* dst, முழு dims, மாறிலி size_t sz[],
              மாறிலி size_t srcofs[], மாறிலி size_t srcstep[],
              மாறிலி size_t dstofs[], மாறிலி size_t dststep[], தருக்க _sync) மாறிலி CV_OVERRIDE
    {
        எனில்(!src || !dst)
            திரும்பு;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        தருக்க iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        எனில்( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            திரும்பு;
        }
        எனில்( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(பொய்);
#ifdef HAVE_OPENCL_SVM
            எனில் ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            இல்லையெனில்
#endif
            {
                dst->markDeviceCopyObsolete(உண்மை);
            }
            திரும்பு;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        எனில் ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            எனில் ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                மாறிலி svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                எனில்( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (முழு)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, வெற்று, வெற்று);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                இல்லையெனில்
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    முழு isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    க்கு( முழு i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        எனில்( sz[i] == 0 )
                        திரும்பு;
                        எனில்( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (முழு)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    க்கு( முழு i = 0; i < dims; i++ )
                    {
                        எனில்( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    மாறிலி Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    க்கு( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            இல்லையெனில்
            {
                எனில் ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                இல்லையெனில்
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        இல்லையெனில்
#endif
        {
            எனில்( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (வெறுமை*)src->handle, (வெறுமை*)dst->handle, (long long முழு)srcrawofs, (long long முழு)dstrawofs, (long long முழு)total).c_str());
            }
            இல்லையெனில் எனில் (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                மாறிலி size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<பொய், பொய்> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<பொய், பொய்> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                க்கு( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            இல்லையெனில்
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        எனில் (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        எனில் ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        இல்லையெனில்
#endif
        {
            dst->markHostCopyObsolete(உண்மை);
        }
        dst->markDeviceCopyObsolete(பொய்);

        எனில்( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(மாறிலி எழுத்து* id) மாறிலி CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        எனில் (ctx.empty())
            திரும்பு வெற்று;
#ifdef HAVE_OPENCL_SVM
        எனில் ((svm::checkForceSVMUmatUsage() && (id == வெற்று || strcmp(id, "OCL") == 0)) || (id != வெற்று && strcmp(id, "SVM") == 0))
        {
            திரும்பு &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        எனில் (id != வெற்று && strcmp(id, "HOST_ALLOC") == 0)
        {
            திரும்பு &ctx.getImpl()->getBufferPoolHostPtr();
        }
        எனில் (id != வெற்று && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        திரும்பு &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    வெறுமை flushCleanupQueue() மாறிலி
    {
        // TSAN false positive: see #27638
        எனில் (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            க்கு (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    வெறுமை addToCleanupQueue(UMatData* u) மாறிலி
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

நிலையான OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    நிலையான OpenCLAllocator* g_allocator = புதிய OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    திரும்பு g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


நாமாகாஶ cv {

// three funcs below are implemented in umatrix.cpp
வெறுமை setSize( UMat& m, முழு _dims, மாறிலி முழு* _sz, மாறிலி size_t* _steps,
              தருக்க autoSteps = பொய் );
வெறுமை finalizeHdr(UMat& m);

} // namespace cv


நாமாகாஶ cv { நாமாகாஶ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
வெறுமை convertFromBuffer(வெறுமை* cl_mem_buffer, size_t step, முழு rows, முழு cols, முழு type, UMat& dst)
{
    முழு d = 2;
    முழு sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, உண்மை);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, அளவு(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, அளவு(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((முழு)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = புதிய UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    திரும்பு;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
வெறுமை convertFromImage(வெறுமை* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, அளவு(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, அளவு(cl_image_format), &fmt, 0));

    முழு depth = CV_8U;
    தேர்வு (fmt.image_channel_data_type)
    {
    நிலை CL_UNORM_INT8:
    நிலை CL_UNSIGNED_INT8:
        depth = CV_8U;
        நிறுத்து;

    நிலை CL_SNORM_INT8:
    நிலை CL_SIGNED_INT8:
        depth = CV_8S;
        நிறுத்து;

    நிலை CL_UNORM_INT16:
    நிலை CL_UNSIGNED_INT16:
        depth = CV_16U;
        நிறுத்து;

    நிலை CL_SNORM_INT16:
    நிலை CL_SIGNED_INT16:
        depth = CV_16S;
        நிறுத்து;

    நிலை CL_SIGNED_INT32:
        depth = CV_32S;
        நிறுத்து;

    நிலை CL_FLOAT:
        depth = CV_32F;
        நிறுத்து;

    நிலை CL_HALF_FLOAT:
        depth = CV_16F;
        நிறுத்து;

    இயல்பு:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    முழு type = CV_8UC1;
    தேர்வு (fmt.image_channel_order)
    {
    நிலை CL_R:
    நிலை CL_A:
    நிலை CL_INTENSITY:
    நிலை CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        நிறுத்து;

    நிலை CL_RG:
    நிலை CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        நிறுத்து;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    நிலை CL_RGBA:
    நிலை CL_BGRA:
    நிலை CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        நிறுத்து;

    இயல்பு:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        நிறுத்து;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, அளவு(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, அளவு(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, அளவு(size_t), &h, 0));

    dst.create((முழு)h, (முழு)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, வெற்று, வெற்று));

    CV_OCL_CHECK(clFinish(q));

    திரும்பு;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

நிலையான வெறுமை getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint இலக்கம்_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, வெற்று, &இலக்கம்_Devices);
    எனில் (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &இலக்கம்_Devices).c_str());
    }

    எனில் (இலக்கம்_Devices == 0)
    {
        devices.clear();
        திரும்பு;
    }

    devices.resize((size_t)இலக்கம்_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, இலக்கம்_Devices, &devices[0], &இலக்கம்_Devices));
}

கட்டமைப்பு PlatformInfo::Impl
{
    Impl(வெறுமை* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) மாறிலி
    {
        எழுத்து buf[1024];
        size_t sz=0;
        திரும்பு clGetPlatformInfo(handle, prop, அளவு(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < அளவு(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    முழு versionMajor_;
    முழு versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(வெறுமை* platform_id)
{
    p = புதிய Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    எனில்(p)
        p->release();
}

PlatformInfo::PlatformInfo(மாறிலி PlatformInfo& i)
{
    எனில் (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(மாறிலி PlatformInfo& i)
{
    எனில் (i.p != p)
    {
        எனில் (i.p)
            i.p->addref();
        எனில் (p)
            p->release();
        p = i.p;
    }
    திரும்பு *இது;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    எனில் (இது != &i) {
        எனில்(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    திரும்பு *இது;
}

முழு PlatformInfo::device_எண்() மாறிலி
{
    திரும்பு p ? (முழு)p->devices.size() : 0;
}

வெறுமை PlatformInfo::getDevice(Device& device, முழு d) மாறிலி
{
    CV_Assert(p && d < (முழு)p->devices.size() );
    எனில்(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() மாறிலி
{
    திரும்பு p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() மாறிலி
{
    திரும்பு p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() மாறிலி
{
    திரும்பு p ? p->version_ : String();
}

முழு PlatformInfo::versionMajor() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->versionMajor_;
}

முழு PlatformInfo::versionMinor() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->versionMinor_;
}

நிலையான வெறுமை getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint இலக்கம்_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms));

    எனில் (இலக்கம்_Platforms == 0)
    {
        platforms.clear();
        திரும்பு;
    }

    platforms.resize((size_t)இலக்கம்_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], &இலக்கம்_Platforms));
}

வெறுமை getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    க்கு (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((வெறுமை*)&platforms[i]) );
}

மாறிலி எழுத்து* typeToStr(முழு type)
{
    நிலையான மாறிலி எழுத்து* tab[CV_DEPTH_MAX*16]=
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
    முழு cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    மாறிலி எழுத்து* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    திரும்பு result;
}

மாறிலி எழுத்து* memopTypeToStr(முழு type)
{
    நிலையான மாறிலி எழுத்து* tab[CV_DEPTH_MAX*16] =
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
    முழு cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    மாறிலி எழுத்து* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    திரும்பு result;
}

மாறிலி எழுத்து* vecopTypeToStr(முழு type)
{
    நிலையான மாறிலி எழுத்து* tab[CV_DEPTH_MAX*16] =
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
    முழு cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    மாறிலி எழுத்து* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    திரும்பு result;
}

// Deprecated due to size of buf buffer being unknowable.
மாறிலி எழுத்து* convertTypeStr(முழு sdepth, முழு ddepth, முழு cn, எழுத்து* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    திரும்பு convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

மாறிலி எழுத்து* convertTypeStr(முழு sdepth, முழு ddepth, முழு cn, எழுத்து* buf, size_t buf_size)
{
    எனில்( sdepth == ddepth )
        திரும்பு "noconvert";
    மாறிலி எழுத்து *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    எனில்( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    இல்லையெனில் எனில்( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    இல்லையெனில்
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    திரும்பு buf;
}

மாறிலி எழுத்து* getOpenCLErrorString(முழு errorCode)
{
#define CV_OCL_CODE(id) நிலை id: திரும்பு #id
#define CV_OCL_CODE_(id, name) நிலை id: திரும்பு #name
    தேர்வு (errorCode)
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
    CV_OCL_CODE(CL_INVALID_மதிப்பு);
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
    CV_OCL_CODE(CL_INVALID_இருமம்);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_மதிப்பு);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_மொத்தம்);
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
    இயல்பு: திரும்பு "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
நிலையான std::string kerToStr(மாறிலி Mat & k)
{
    முழு width = k.cols - 1, depth = k.depth();
    மாறிலி T * மாறிலி data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    எனில் (depth <= CV_8S)
    {
        க்கு (முழு i = 0; i < width; ++i)
            stream << "DIG(" << (முழு)data[i] << ")";
        stream << "DIG(" << (முழு)data[width] << ")";
    }
    இல்லையெனில் எனில் (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        க்கு (முழு i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    இல்லையெனில் எனில் (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        க்கு (முழு i = 0; i < width; ++i)
            stream << "DIG(" << (தசம)data[i] << "h)";
        stream << "DIG(" << (தசம)data[width] << "h)";
    }
    இல்லையெனில்
    {
        க்கு (முழு i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    திரும்பு stream.str();
}

String kernelToStr(உள்ளீடு_Array _kernel, முழு ddepth, மாறிலி எழுத்து * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    முழு depth = kernel.depth();
    எனில் (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    எனில் (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    எனில் (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    வகைவரை std::string (* func_t)(மாறிலி Mat &);
    நிலையான மாறிலி func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<எழுத்து>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<முழு>, kerToStr<தசம>, kerToStr<இரட்டை>, kerToStr<hfloat>,
                                                0, kerToStr<தருக்க>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    மாறிலி func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    திரும்பு cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    செய் \
    { \
        எனில் (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            முழு ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            எனில் (cwidth < ckercn || ckercn <= 0) \
                திரும்பு 1; \
            cols.push_back(cwidth); \
            எனில் (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                திரும்பு 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    போது ((வெறுமை)0, 0)

முழு predictOptimalVectorWidth(உள்ளீடு_Array src1, உள்ளீடு_Array src2, உள்ளீடு_Array src3,
                              உள்ளீடு_Array src4, உள்ளீடு_Array src5, உள்ளீடு_Array src6,
                              உள்ளீடு_Array src7, உள்ளீடு_Array src8, உள்ளீடு_Array src9,
                              OclVectorStrategy strat)
{
    மாறிலி ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    முழு vectorWidths[CV_DEPTH_MAX] = {
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
    எனில் (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    திரும்பு checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

முழு checkOptimalVectorWidth([[maybe_unused]] மாறிலி முழு *vectorWidths,
                            [[maybe_unused]] உள்ளீடு_Array src1,
                            [[maybe_unused]] உள்ளீடு_Array src2,
                            [[maybe_unused]] உள்ளீடு_Array src3,
                            [[maybe_unused]] உள்ளீடு_Array src4,
                            [[maybe_unused]] உள்ளீடு_Array src5,
                            [[maybe_unused]] உள்ளீடு_Array src6,
                            [[maybe_unused]] உள்ளீடு_Array src7,
                            [[maybe_unused]] உள்ளீடு_Array src8,
                            [[maybe_unused]] உள்ளீடு_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    திரும்பு 1;
#இல்லையெனில்
    CV_Assert(vectorWidths);

    முழு ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<முழு> dividers, kercns;
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

    க்கு (size_t i = 0; i < size; ++i)
        போது (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    முழு kercn = *std::min_element(kercns.begin(), kercns.end());

    திரும்பு kercn;
#endif
}

முழு predictOptimalVectorWidthMax(உள்ளீடு_Array src1, உள்ளீடு_Array src2, உள்ளீடு_Array src3,
                                 உள்ளீடு_Array src4, உள்ளீடு_Array src5, உள்ளீடு_Array src6,
                                 உள்ளீடு_Array src7, உள்ளீடு_Array src8, உள்ளீடு_Array src9)
{
    திரும்பு predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
வெறுமை buildOptionsAddMatrixDescription(String& buildOptions, மாறிலி String& name, உள்ளீடு_Array _m)
{
    எனில் (!buildOptions.empty())
        buildOptions += " ";
    முழு type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (முழு)CV_MAT_CN(type),
            name.c_str(), (முழு)CV_ELEM_SIZE(type),
            name.c_str(), (முழு)CV_ELEM_SIZE1(type),
            name.c_str(), (முழு)depth
            );
}


கட்டமைப்பு Image2D::Impl
{
    Impl(மாறிலி UMat &src, தருக்க norm, தருக்க alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        எனில் (handle)
            clReleaseMemObject(handle);
    }

    நிலையான cl_image_format getImageFormat(முழு depth, முழு cn, தருக்க norm)
    {
        cl_image_format format;
        நிலையான மாறிலி முழு channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        நிலையான மாறிலி முழு channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        நிலையான மாறிலி முழு channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        முழு channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        முழு channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        திரும்பு format;
    }

    நிலையான தருக்க isFormatSupported(cl_image_format format)
    {
        எனில் (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        எனில் (!context)
            திரும்பு பொய்;

        // Figure out how many formats are supported by this context.
        cl_uint இலக்கம்_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, இலக்கம்_Formats,
                                                வெற்று, &இலக்கம்_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        எனில் (இலக்கம்_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(இலக்கம்_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, இலக்கம்_Formats,
                                             formats.data(), வெற்று);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            க்கு (cl_uint i = 0; i < இலக்கம்_Formats; ++i)
            {
                எனில் (!memcmp(&formats[i], &format, அளவு(format)))
                {
                    திரும்பு உண்மை;
                }
            }
        }
        திரும்பு பொய்;
    }

    வெறுமை init(மாறிலி UMat &src, தருக்க norm, தருக்க alias)
    {
        எனில் (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        முழு err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        எனில் (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        எனில் (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        மாறிலி Device & d = ocl::Device::getDefault();
        முழு minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        எனில் (1 < major || (1 == major && 2 <= minor))
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
            desc.இலக்கம்_mip_levels   = 0;
            desc.இலக்கம்_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, வெற்று, &err);
        }
        இல்லையெனில்
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, வெற்று, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        எனில் (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), வெற்று, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long முழு)(src.cols * src.rows * src.elemSize()), (வெறுமை*)devData
                ).c_str());

            மாறிலி size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, வெற்று, வெற்று));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        இல்லையெனில்
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != வெற்று);

        எனில் (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, வெற்று, 0));
            எனில் (!src.isContinuous())
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
    p = வெற்று;
}

Image2D::Image2D(மாறிலி UMat &src, தருக்க norm, தருக்க alias)
{
    p = புதிய Impl(src, norm, alias);
}

தருக்க Image2D::canCreateAlias(மாறிலி UMat &m)
{
    தருக்க ret = பொய்;
    மாறிலி Device & d = ocl::Device::getDefault();
    எனில் (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        எனில் (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            எனில் (!m.u->tempUMat())
            {
                ret = உண்மை;
            }
        }
    }
    திரும்பு ret;
}

தருக்க Image2D::isFormatSupported(முழு depth, முழு cn, தருக்க norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    திரும்பு Impl::isFormatSupported(format);
}

Image2D::Image2D(மாறிலி Image2D & i)
{
    p = i.p;
    எனில் (p)
        p->addref();
}

Image2D & Image2D::operator = (மாறிலி Image2D & i)
{
    எனில் (i.p != p)
    {
        எனில் (i.p)
            i.p->addref();
        எனில் (p)
            p->release();
        p = i.p;
    }
    திரும்பு *இது;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    எனில் (இது != &i) {
        எனில் (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    திரும்பு *இது;
}

Image2D::~Image2D()
{
    எனில் (p)
        p->release();
}

வெறுமை* Image2D::ptr() மாறிலி
{
    திரும்பு p ? p->handle : 0;
}

தருக்க internal::isOpenCLForced()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க value = பொய்;
    எனில் (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", பொய்);
        initialized = உண்மை;
    }
    திரும்பு value;
}

தருக்க internal::isPerformanceCheckBypassed()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான தருக்க value = பொய்;
    எனில் (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", பொய்);
        initialized = உண்மை;
    }
    திரும்பு value;
}

தருக்க internal::isCLBuffer(UMat& u)
{
    எனில் (isCudaBacked(u.u))
        திரும்பு பொய்;
    வெறுமை* h = u.handle(ACCESS_RW);
    எனில் (!h)
        திரும்பு உண்மை;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#எனில் 1
    எனில் ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        திரும்பு பொய்;
#இல்லையெனில்
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, அளவு(type), &type, வெற்று);
    எனில் (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        திரும்பு பொய்;
#endif
    திரும்பு உண்மை;
}

கட்டமைப்பு Timer::Impl
{
    மாறிலி Queue queue;

    Impl(மாறிலி Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    வெறுமை start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    வெறுமை stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() மாறிலி
    {
        திரும்பு (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(மாறிலி Queue& q) : p(புதிய Impl(q)) { }
Timer::~Timer() { delete p; }

வெறுமை Timer::start()
{
    CV_Assert(p);
    p->start();
}

வெறுமை Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() மாறிலி
{
    CV_Assert(p);
    திரும்பு p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
