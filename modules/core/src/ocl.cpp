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

#include "precomp.hpp"

#ifndef HAVE_OPENCL
#include "ocl_disabled.impl.hpp"
#དེ་མིན // HAVE_OPENCL

#include <list>
#include <map>
#include <deque>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#གལ་ཏེ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#include <inttypes.h>
#endif

#include <opencv2/core/utils/configuration.private.hpp>

#include <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#include <opencv2/core/utils/logger.hpp>

#include "opencv2/core/ocl_genbase.hpp"
#include "opencl_kernels_core.hpp"

#include "opencv2/core/utils/lock.private.hpp"
#include "opencv2/core/utils/filesystem.hpp"
#include "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(NULL, 0, "OpenCL allocator: " << __VA_ARGS__)
#include "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_BINARY_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#include "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# གལ་ཏེ 0
#   define LOG_BUFFER_POOL printf
# དེ་མིན
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#གལ་ཏེ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#དེ་མིན
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#གལ་ཏེ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#དེ་མིན
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#include "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#include "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#include "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#include "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#include "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#include "opencv2/core/opencl/opencl_svm.hpp"
#endif

#include "umatrix.hpp"

namespace cv { namespace ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    void addref() { CV_XADD(&refcount, 1); } \
    void release() { གལ་ཏེ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete this; } \
    ཧྲིལ refcount

static cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ཕྱིར opencl_allocator_stats;
}

#ifndef _DEBUG
static bool isRaiseError()
{
    static bool initialized = false;
    static bool value = false;
    གལ་ཏེ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", false);
        initialized = true;
    }
    ཕྱིར value;
}
#endif

static void onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", false);
    གལ་ཏེ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#གལ་ཏེ CV_OPENCL_TRACE_CHECK
static anta_hstha
void traceOpenCLCheck(cl_int status, const var_na* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#དེ་མིན
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_MSG(check_result, msg) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, msg)

#define CV_OCL_CHECK_RESULT(check_result, msg) \
    do { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, msg); \
        གལ་ཏེ (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(msg), const var_na*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_msg = CV_OCL_API_ERROR_MSG(check_result, msg); \
            CV_Error(Error::OpenCLApiCallError, error_msg); \
        } \
    } སྐབས (0)

#define CV_OCL_CHECK_(expr, check_result) do { expr; CV_OCL_CHECK_RESULT(check_result, #expr); } སྐབས (0)

#define CV_OCL_CHECK(expr) do { cl_int __cl_result = (expr); CV_OCL_CHECK_RESULT(__cl_result, #expr); } སྐབས (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, msg) CV_OCL_CHECK_RESULT(check_result, msg)
#define CV_OCL_DBG_CHECK(expr) CV_OCL_CHECK(expr)
#define CV_OCL_DBG_CHECK_(expr, check_result) CV_OCL_CHECK_(expr, check_result)
#དེ་མིན
#define CV_OCL_DBG_CHECK_RESULT(check_result, msg) \
    do { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, msg); \
        གལ་ཏེ (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(msg), const var_na*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_msg = CV_OCL_API_ERROR_MSG(check_result, msg); \
            CV_Error(Error::OpenCLApiCallError, error_msg); \
        } \
    } སྐབས (0)
#define CV_OCL_DBG_CHECK_(expr, check_result) do { expr; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } སྐབས (0)
#define CV_OCL_DBG_CHECK(expr) do { cl_int __cl_result = (expr); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } སྐབས (0)
#endif


static const bool CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", true);
static const bool CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", true);
static const bool CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", true);
static const bool CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", true);

#གལ་ཏེ CV_OPENCL_VALIDATE_BINARY_PROGRAMS
static const bool CV_OPENCL_VALIDATE_BINARY_PROGRAMS_VALUE = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", false);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
static const bool CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#གལ་ཏེ 1 //def __APPLE__
        true
#དེ་མིན
        false
#endif
);

static String getBuildExtraOptions()
{
    static String param_buildExtraOptions;
    static bool initialized = false;
    གལ་ཏེ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = true;
        གལ་ཏེ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(NULL, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ཕྱིར param_buildExtraOptions;
}

static const bool CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", true);
static const size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


sa_nracanaa UMat2D
{
    UMat2D(const UMat& m)
    {
        offset = (ཧྲིལ)m.offset;
        step = (ཧྲིལ)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    ཧྲིལ offset;
    ཧྲིལ step;
    ཧྲིལ rows;
    ཧྲིལ cols;
};

sa_nracanaa UMat3D
{
    UMat3D(const UMat& m)
    {
        offset = (ཧྲིལ)m.offset;
        step = (ཧྲིལ)m.step.p[1];
        slicestep = (ཧྲིལ)m.step.p[0];
        slices = (ཧྲིལ)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    ཧྲིལ offset;
    ཧྲིལ slicestep;
    ཧྲིལ step;
    ཧྲིལ slices;
    ཧྲིལ rows;
    ཧྲིལ cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
static uint64 crc64( const uchar* data, size_t size, uint64 crc0=0 )
{
    static uint64 table[256];
    static bool initialized = false;

    གལ་ཏེ( !initialized )
    {
        ཆེད་དུ( ཧྲིལ i = 0; i < 256; i++ )
        {
            uint64 c = i;
            ཆེད་དུ( ཧྲིལ j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = true;
    }

    uint64 crc = ~crc0;
    ཆེད་དུ( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    ཕྱིར ~crc;
}

#གལ་ཏེ OPENCV_HAVE_FILESYSTEM_SUPPORT
sa_nracanaa OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    typedef std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(NULL, "Initializing OpenCL cache configuration...");
        གལ་ཏེ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(NULL, "OpenCL cache is disabled");
            ཕྱིར;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        གལ་ཏེ (cache_path_.empty())
        {
            CV_LOG_INFO(NULL, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        do
        {
            try
            {
                གལ་ཏེ (cache_path_.empty())
                    bha_nga;
                གལ་ཏེ (cache_path_ == "disabled")
                    bha_nga;
                གལ་ཏེ (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(NULL, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    bha_nga;
                }

                གལ་ཏེ (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    གལ་ཏེ (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(NULL, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        གལ་ཏེ (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(NULL, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            bha_nga;
                        }
                    }

                    try
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(NULL, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(NULL, 0, "Checking cache lock... Done!");
                    }
                    gr_haa_na (const cv::Exception& e)
                    {
                        CV_LOG_WARNING(NULL, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    gr_haa_na (...)
                    {
                        CV_LOG_WARNING(NULL, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                དེ་མིན
                {
                    གལ་ཏེ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(NULL, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    དེ་མིན
                    {
                        CV_LOG_INFO(NULL, "OpenCL cache lock is disabled");
                    }
                }
            }
            gr_haa_na (const cv::Exception& e)
            {
                CV_LOG_WARNING(NULL, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } སྐབས (0);
        གལ་ཏེ (!cache_path_.empty())
        {
            གལ་ཏེ (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(NULL, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            དེ་མིན
            {
                CV_LOG_INFO(NULL, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    void clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(const std::string& ctx_prefix,
            const std::string& cleanup_prefix)
    {
        གལ་ཏེ (cache_path_.empty())
            ཕྱིར std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        གལ་ཏེ (found_it != prepared_contexts_.end())
            ཕྱིར found_it->second;

        CV_LOG_INFO(NULL, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        bool result = utils::fs::isDirectory(target_directory);
        གལ་ཏེ (!result)
        {
            try
            {
                CV_LOG_VERBOSE(NULL, 0, "Creating directory: " << target_directory);
                གལ་ཏེ (utils::fs::createDirectories(target_directory))
                {
                    result = true;
                }
                དེ་མིན
                {
                    CV_LOG_WARNING(NULL, "Can't create directory: " << target_directory);
                }
            }
            gr_haa_na (const cv::Exception& e)
            {
                CV_LOG_ERROR(NULL, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        གལ་ཏེ (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            try
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, false, true);
                std::vector<String> remove_entries;
                ཆེད་དུ (size_t i = 0; i < entries.size(); i++)
                {
                    const String& name = entries[i];
                    གལ་ཏེ (0 == name.find(cleanup_prefix))
                    {
                        གལ་ཏེ (0 == name.find(ctx_prefix))
                            continue; // skip current
                        remove_entries.push_back(name);
                    }
                }
                གལ་ཏེ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(NULL, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(NULL, "Trying to remove these directories...");
                    ཆེད་དུ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(NULL, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(NULL, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    ཆེད་དུ (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        const String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        try
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(NULL, "Removed: " << path);
                        }
                        gr_haa_na (const cv::Exception& e)
                        {
                            CV_LOG_ERROR(NULL, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            gr_haa_na (...)
            {
                CV_LOG_WARNING(NULL, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(NULL, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        ཕྱིར target_directory;
    }

    static OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, new OpenCLBinaryCacheConfigurator());
    }
};
class BinaryProgramFile
{
    ga_nanaa { MAX_ENTRIES = 64 };

    typedef unsigned ཧྲིལ uint32_t;

    sa_nracanaa CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    sa_nracanaa CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t numberOfEntries;
        //uint32_t firstEntryOffset[];
    };

    sa_nracanaa CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t keySize;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    const std::string fileName_;
    const var_na* const sourceSignature_;
    const size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(const std::string& options)
    {
        uint64 hash = crc64((const uchar*)options.c_str(), options.size(), 0);
        ཕྱིར hash & (MAX_ENTRIES - 1);
    }

    anta_hstha size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ཕྱིར fileSize;
    }
    anta_hstha uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.read((var_na*)&res, parimaa_na(uint32_t));
        CV_Assert(!f.fail());
        ཕྱིར res;
    }
    anta_hstha void writeUInt32(const uint32_t value)
    {
        uint32_t v = value;
        f.write((var_na*)&v, parimaa_na(uint32_t));
        CV_Assert(!f.fail());
    }

    anta_hstha void seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    anta_hstha void seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    anta_hstha void seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    void clearFile()
    {
        f.close();
        གལ་ཏེ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(NULL, "Can't remove: " << fileName_);
        ཕྱིར;
    }

public:
    BinaryProgramFile(const std::string& fileName, const var_na* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(parimaa_na(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != NULL);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, parimaa_na(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        གལ་ཏེ(f.is_open() && getFileSize() > 0)
        {
            bool isValid = false;
            try
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                གལ་ཏེ (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<var_na> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.read(fileSourceSignature.data(), fileSourceSignatureSize);
                    གལ་ཏེ (f.eof())
                    {
                        CV_LOG_ERROR(NULL, "Unexpected EOF");
                    }
                    དེ་མིན གལ་ཏེ (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = true;
                    }
                }
                གལ་ཏེ (!isValid)
                {
                    CV_LOG_ERROR(NULL, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            gr_haa_na (const cv::Exception& e)
            {
                CV_LOG_ERROR(NULL, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            gr_haa_na (...)
            {
                CV_LOG_ERROR(NULL, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            གལ་ཏེ (!isValid)
            {
                clearFile();
            }
            དེ་མིན
            {
                seekReadAbsolute(0);
            }
        }
    }

    bool read(const std::string& key, std::vector<var_na>& buf)
    {
        གལ་ཏེ (!f.is_open())
            ཕྱིར false;

        size_t fileSize = getFileSize();
        གལ་ཏེ (fileSize == 0)
        {
            CV_LOG_ERROR(NULL, "Invalid file (empty): " << fileName_);
            clearFile();
            ཕྱིར false;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t numberOfEntries = readUInt32();
        CV_Assert(numberOfEntries > 0);
        གལ་ཏེ (numberOfEntries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(NULL, "Invalid file: " << fileName_);
            clearFile();
            ཕྱིར false;
        }
        f.read((var_na*)&entryOffsets[0], parimaa_na(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entryNum = getHash(key);

        uint32_t entryOffset = entryOffsets[entryNum];
        FileEntry entry;
        སྐབས (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.read((var_na*)&entry, parimaa_na(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<var_na> fileKey(entry.keySize + 1);
            གལ་ཏེ (key.size() == entry.keySize)
            {
                གལ་ཏེ (entry.keySize > 0)
                {
                    f.read(fileKey.data(), entry.keySize);
                    CV_Assert(!f.fail());
                }
                གལ་ཏེ (memcmp(fileKey.data(), key.c_str(), entry.keySize) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.read(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(NULL, 0, "Read...");
                    ཕྱིར true;
                }
            }
            གལ་ཏེ (entry.nextEntryFileOffset == 0)
                bha_nga;
            entryOffset = entry.nextEntryFileOffset;
        }
        ཕྱིར false;
    }

    bool write(const std::string& key, std::vector<var_na>& buf)
    {
        གལ་ཏེ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            གལ་ཏེ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                གལ་ཏེ (!f.is_open())
                {
                    CV_LOG_ERROR(NULL, "Can't create file: " << fileName_);
                    ཕྱིར false;
                }
            }
        }

        size_t fileSize = getFileSize();
        གལ་ཏེ (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.write(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, parimaa_na(entryOffsets));
            f.write((var_na*)entryOffsets, parimaa_na(entryOffsets));
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

        uint32_t numberOfEntries = readUInt32();
        CV_Assert(numberOfEntries > 0);
        གལ་ཏེ (numberOfEntries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(NULL, "Invalid file: " << fileName_);
            clearFile();
            ཕྱིར false;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.read((var_na*)&entryOffsets[0], parimaa_na(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entryNum = getHash(key);

        uint32_t entryOffset = entryOffsets[entryNum];
        FileEntry entry;
        སྐབས (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.read((var_na*)&entry, parimaa_na(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<var_na> fileKey(entry.keySize + 1);
            གལ་ཏེ (key.size() == entry.keySize)
            {
                གལ་ཏེ (entry.keySize > 0)
                {
                    f.read(fileKey.data(), entry.keySize);
                    CV_Assert(!f.fail());
                }
                གལ་ཏེ (0 == memcmp(fileKey.data(), key.c_str(), entry.keySize))
                {
                    // duplicate
                    CV_LOG_VERBOSE(NULL, 0, "Duplicate key ignored: " << fileName_);
                    ཕྱིར false;
                }
            }
            གལ་ཏེ (entry.nextEntryFileOffset == 0)
                bha_nga;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        གལ་ཏེ (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.write((var_na*)&entry, parimaa_na(entry));
            CV_Assert(!f.fail());
        }
        དེ་མིན
        {
            entryOffsets[entryNum] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.write((var_na*)entryOffsets, parimaa_na(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.keySize = (uint32_t)key.size();
        f.write((var_na*)&entry, parimaa_na(entry));
        CV_Assert(!f.fail());
        f.write(key.c_str(), entry.keySize);
        CV_Assert(!f.fail());
        f.write(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(NULL, 0, "Write... (" << buf.size() << " bytes)");
        ཕྱིར true;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



sa_nracanaa OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    ཧྲིལ device_;  // device index in context
    ocl::Queue queue_;
    ཧྲིལ useOpenCL_;

rak_sita:
    Impl() = delete;

    void _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        ཧྲིལ ndevices = (ཧྲིལ)context_.ndevices();
        CV_Assert(ndevices > 0);
        bool found = false;
        ཆེད་དུ (ཧྲིལ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            གལ་ཏེ (dhandle == deviceID)
            {
                device_ = i;
                found = true;
                bha_nga;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    void _init_device(const ocl::Device& device)
    {
        CV_Assert(device.ptr());
        ཧྲིལ ndevices = (ཧྲིལ)context_.ndevices();
        CV_Assert(ndevices > 0);
        bool found = false;
        ཆེད་དུ (ཧྲིལ i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            གལ་ཏེ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = true;
                bha_nga;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

public:
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

    Impl(const ocl::Context& context, const ocl::Device& device, const ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(const ocl::Context& context, const ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(const ocl::Context& context, const ཧྲིལ device, const ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(const Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    anta_hstha bool useOpenCL() const { ཕྱིར const_cast<Impl*>(this)->useOpenCL(); }
    bool useOpenCL()
    {
        གལ་ཏེ (useOpenCL_ < 0)
        {
            try
            {
                useOpenCL_ = 0;
                གལ་ཏེ (!context_.empty() && context_.ndevices() > 0)
                {
                    const Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            gr_haa_na (const cv::Exception&)
            {
                // nothing
            }
            གལ་ཏེ (!useOpenCL_)
                CV_LOG_INFO(NULL, "OpenCL: can't use OpenCL execution context");
        }
        ཕྱིར useOpenCL_ > 0;
    }

    void setUseOpenCL(bool flag)
    {
        གལ་ཏེ (!flag)
            useOpenCL_ = 0;
        དེ་མིན
            useOpenCL_ = -1;
    }

    static const std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(NULL, "OpenCL: initializing thread execution context");

        static bool initialized = false;
        static std::shared_ptr<Impl> g_primaryExecutionContext;

        གལ་ཏེ (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            གལ་ཏེ (!initialized)
            {
                CV_LOG_INFO(NULL, "OpenCL: creating new execution context...");
                try
                {
                    Context c = ocl::Context::create(std::string());
                    གལ་ཏེ (c.ndevices())
                    {
                        ཧྲིལ deviceId = 0;
                        auto& d = c.device(deviceId);
                        གལ་ཏེ (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            གལ་ཏེ (!q.ptr())
                            {
                                CV_LOG_ERROR(NULL, "OpenCL: Can't create default OpenCL queue");
                            }
                            དེ་མིན
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(NULL, "OpenCL: device=" << d.name());
                            }
                        }
                        དེ་མིན
                        {
                            CV_LOG_ERROR(NULL, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    དེ་མིན
                    {
                        CV_LOG_INFO(NULL, "OpenCL: context is not available/disabled");
                    }
                }
                gr_haa_na (const std::exception& e)
                {
                    CV_LOG_INFO(NULL, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                gr_haa_na (...)
                {
                    CV_LOG_WARNING(NULL, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = true;
            }
        }
        ཕྱིར g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() const
{
    CV_Assert(p);
    ཕྱིར p->context_;
}
Device& OpenCLExecutionContext::getDevice() const
{
    CV_Assert(p);
    ཕྱིར p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() const
{
    CV_Assert(p);
    ཕྱིར p->queue_;
}

bool OpenCLExecutionContext::useOpenCL() const
{
    གལ་ཏེ (p)
        ཕྱིར p->useOpenCL();
    ཕྱིར false;
}
void OpenCLExecutionContext::setUseOpenCL(bool flag)
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
    གལ་ཏེ (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = true;
        གལ་ཏེ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ཕྱིར c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    ཕྱིར c;
}

void OpenCLExecutionContext::bind() const
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *this;
    data.oclExecutionContextInitialized = true;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() const
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    const Queue q(getContext(), getDevice());
    ཕྱིར cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(const ocl::Queue& q) const
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != NULL);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ཕྱིར c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device, const ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    གལ་ཏེ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    ཕྱིར ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(const Context& context, const Device& device)
{
    CV_TRACE_FUNCTION();
    གལ་ཏེ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ཕྱིར ctx;

}

void OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
static bool g_isOpenCLInitialized = false;
static bool g_isOpenCLAvailable = false;

bool haveOpenCL()
{
    CV_TRACE_FUNCTION();

    གལ་ཏེ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        གལ་ཏེ (!envPath.empty())
        {
            གལ་ཏེ (envPath == "disabled")
            {
                g_isOpenCLAvailable = false;
                g_isOpenCLInitialized = true;
                ཕྱིར false;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(NULL, "Initialize OpenCL runtime...");
        try
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, NULL, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(NULL, "OpenCL: found " << n << " platforms");
        }
        gr_haa_na (...)
        {
            g_isOpenCLAvailable = false;
        }
        g_isOpenCLInitialized = true;
    }
    ཕྱིར g_isOpenCLAvailable;
}

bool useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    གལ་ཏེ (data.useOpenCL < 0)
    {
        try
        {
            data.useOpenCL = 0;
            གལ་ཏེ (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        gr_haa_na (...)
        {
            CV_LOG_INFO(NULL, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ཕྱིར data.useOpenCL > 0;
}

bool isOpenCLActivated()
{
    གལ་ཏེ (!g_isOpenCLAvailable)
        ཕྱིར false; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ཕྱིར useOpenCL();
}

void setUseOpenCL(bool flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    གལ་ཏེ (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    དེ་མིན
    {
        གལ་ཏེ (!flag)
            data.useOpenCL = 0;
        དེ་མིན
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

class AmdBlasHelper
{
public:
    static AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, new AmdBlasHelper())
    }

    bool isAvailable() const
    {
        ཕྱིར g_isAmdBlasAvailable;
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

rak_sita:
    AmdBlasHelper()
    {
        གལ་ཏེ (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            གལ་ཏེ (!g_isAmdBlasInitialized)
            {
                གལ་ཏེ (haveOpenCL())
                {
                    try
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    gr_haa_na (...)
                    {
                        g_isAmdBlasAvailable = false;
                    }
                }
                དེ་མིན
                    g_isAmdBlasAvailable = false;

                g_isAmdBlasInitialized = true;
            }
        }
    }

private:
    static bool g_isAmdBlasInitialized;
    static bool g_isAmdBlasAvailable;
};

bool AmdBlasHelper::g_isAmdBlasAvailable = false;
bool AmdBlasHelper::g_isAmdBlasInitialized = false;

bool haveAmdBlas()
{
    ཕྱིར AmdBlasHelper::getInstance().isAvailable();
}

#དེ་མིན

bool haveAmdBlas()
{
    ཕྱིར false;
}

#endif

#ifdef HAVE_CLAMDFFT

class AmdFftHelper
{
public:
    static AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, new AmdFftHelper())
    }

    bool isAvailable() const
    {
        ཕྱིར g_isAmdFftAvailable;
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

rak_sita:
    AmdFftHelper()
    {
        གལ་ཏེ (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            གལ་ཏེ (!g_isAmdFftInitialized)
            {
                གལ་ཏེ (haveOpenCL())
                {
                    try
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = true;
                    }
                    gr_haa_na (const Exception &)
                    {
                        g_isAmdFftAvailable = false;
                    }
                }
                དེ་མིན
                    g_isAmdFftAvailable = false;

                g_isAmdFftInitialized = true;
            }
        }
    }

private:
    static clfftSetupData setupData;
    static bool g_isAmdFftInitialized;
    static bool g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
bool AmdFftHelper::g_isAmdFftAvailable = false;
bool AmdFftHelper::g_isAmdFftInitialized = false;

bool haveAmdFft()
{
    ཕྱིར AmdFftHelper::getInstance().isAvailable();
}

#དེ་མིན

bool haveAmdFft()
{
    ཕྱིར false;
}

#endif

bool haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    ཕྱིར true;
#དེ་མིན
    ཕྱིར false;
#endif
}

void finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

sa_nracanaa Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = false;
    }

    ~Impl() {}

    void init()
    {
        གལ་ཏེ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            གལ་ཏེ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            གལ་ཏེ( handle != 0 )
            {
                var_na buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, parimaa_na(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = true;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    bool initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    གལ་ཏེ(p)
        p->release();
}

Platform::Platform(const Platform& pl)
{
    p = (Impl*)pl.p;
    གལ་ཏེ(p)
        p->addref();
}

Platform& Platform::operator = (const Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    གལ་ཏེ (this != &pl) {
        གལ་ཏེ(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    ཕྱིར *this;
}

void* Platform::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(NULL, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    static Platform p;
    གལ་ཏེ( !p.p )
    {
        p.p = new Impl;
        p.p->init();
    }
    ཕྱིར p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
static void parseOpenCLVersion(const String &version, ཧྲིལ &major, ཧྲིལ &minor)
{
    major = minor = 0;
    གལ་ཏེ (10 >= version.length())
        ཕྱིར;
    const var_na *pstr = version.c_str();
    གལ་ཏེ (0 != strncmp(pstr, "OpenCL ", 7))
        ཕྱིར;
    size_t ppos = version.find('.', 7);
    གལ་ཏེ (String::npos == ppos)
        ཕྱིར;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

sa_nracanaa Device::Impl
{
    Impl(void* d)
        : refcount(1)
        , handle(0)
    {
        try
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        gr_haa_na (...)
        {
            k_sipa;
        }
    }

    void _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, ཧྲིལ>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, ཧྲིལ>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, ཧྲིལ>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, ཧྲིལ>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        སྐབས (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            གལ་ཏེ (pos2 == String::npos)
                pos2 = extensions_.size();
            གལ་ཏེ (pos2 > pos)
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
        གལ་ཏེ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        དེ་མིན གལ་ཏེ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        དེ་མིན གལ་ཏེ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        དེ་མིན
            vendorID_ = UNKNOWN_VENDOR;

        const size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        གལ་ཏེ (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            const size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            གལ་ཏེ (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(NULL, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#གལ་ཏེ 0
        གལ་ཏེ (isExtensionSupported("cl_khr_spir"))
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
        གལ་ཏེ (!cv::__termination)
#endif
        {
            གལ་ཏེ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) const
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        ཕྱིར clGetDeviceInfo(handle, prop, parimaa_na(temp), &temp, &sz) == CL_SUCCESS &&
            sz == parimaa_na(temp) ? _TpOut(temp) : _TpOut();
    }

    bool getBoolProp(cl_device_info prop) const
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        ཕྱིར clGetDeviceInfo(handle, prop, parimaa_na(temp), &temp, &sz) == CL_SUCCESS &&
            sz == parimaa_na(temp) ? temp != 0 : false;
    }

    String getStrProp(cl_device_info prop) const
    {
        var_na buf[4096];
        size_t sz=0;
        ཕྱིར clGetDeviceInfo(handle, prop, parimaa_na(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < parimaa_na(buf) ? String(buf) : String();
    }

    bool isExtensionSupported(const std::string& extensionName) const
    {
        ཕྱིར extensions_set_.count(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    ཧྲིལ doubleFPConfig_;
    bool khr_fp64_support_;
    ཧྲིལ halfFPConfig_;
    bool khr_fp16_support_;
    bool hostUnifiedMemory_;
    ཧྲིལ maxComputeUnits_;
    size_t maxWorkGroupSize_;
    ཧྲིལ type_;
    ཧྲིལ addressBits_;
    ཧྲིལ deviceVersionMajor_;
    ཧྲིལ deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    ཧྲིལ vendorID_;
    bool intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(void* d)
{
    p = 0;
    set(d);
}

Device::Device(const Device& d)
{
    p = d.p;
    གལ་ཏེ(p)
        p->addref();
}

Device& Device::operator = (const Device& d)
{
    Impl* newp = (Impl*)d.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    གལ་ཏེ (this != &d) {
        གལ་ཏེ(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    ཕྱིར *this;
}

Device::~Device()
{
    གལ་ཏེ(p)
        p->release();
}

void Device::set(void* d)
{
    གལ་ཏེ(p)
        p->release();
    p = new Impl(d);
    གལ་ཏེ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(void* d)
{
    Device device(d);
    ཕྱིར device;
}

void* Device::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

String Device::name() const
{ ཕྱིར p ? p->name_ : String(); }

String Device::extensions() const
{ ཕྱིར p ? String(p->extensions_) : String(); }

bool Device::isExtensionSupported(const String& extensionName) const
{ ཕྱིར p ? p->isExtensionSupported(extensionName) : false; }

String Device::version() const
{ ཕྱིར p ? p->version_ : String(); }

String Device::vendorName() const
{ ཕྱིར p ? p->vendorName_ : String(); }

ཧྲིལ Device::vendorID() const
{ ཕྱིར p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() const
{ ཕྱིར p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() const
{ ཕྱིར p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

ཧྲིལ Device::deviceVersionMajor() const
{ ཕྱིར p ? p->deviceVersionMajor_ : 0; }

ཧྲིལ Device::deviceVersionMinor() const
{ ཕྱིར p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() const
{ ཕྱིར p ? p->driverVersion_ : String(); }

ཧྲིལ Device::type() const
{ ཕྱིར p ? p->type_ : 0; }

ཧྲིལ Device::addressBits() const
{ ཕྱིར p ? p->addressBits_ : 0; }

bool Device::available() const
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : false; }

bool Device::compilerAvailable() const
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : false; }

bool Device::linkerAvailable() const
#ifdef CL_VERSION_1_2
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : false; }
#དེ་མིན
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

ཧྲིལ Device::doubleFPConfig() const
{ ཕྱིར p ? p->doubleFPConfig_ : 0; }

ཧྲིལ Device::singleFPConfig() const
{ ཕྱིར p ? p->getProp<cl_device_fp_config, ཧྲིལ>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

ཧྲིལ Device::halfFPConfig() const
{ ཕྱིར p ? p->halfFPConfig_ : 0; }

bool Device::hasFP64() const
{ ཕྱིར p ? p->khr_fp64_support_ : false; }
bool Device::hasFP16() const
{ ཕྱིར p ? p->khr_fp16_support_ : false; }

bool Device::endianLittle() const
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : false; }

bool Device::errorCorrectionSupport() const
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : false; }

ཧྲིལ Device::executionCapabilities() const
{ ཕྱིར p ? p->getProp<cl_device_exec_capabilities, ཧྲིལ>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

ཧྲིལ Device::globalMemCacheType() const
{ ཕྱིར p ? p->getProp<cl_device_mem_cache_type, ཧྲིལ>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

ཧྲིལ Device::globalMemCacheLineSize() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

ཧྲིལ Device::localMemType() const
{ ཕྱིར p ? p->getProp<cl_device_local_mem_type, ཧྲིལ>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

bool Device::hostUnifiedMemory() const
{ ཕྱིར p ? p->hostUnifiedMemory_ : false; }

bool Device::imageSupport() const
{ ཕྱིར p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : false; }

bool Device::imageFromBufferSupport() const
{
    ཕྱིར p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : false;
}

uint Device::imagePitchAlignment() const
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ཕྱིར p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#དེ་མིན
    ཕྱིར 0;
#endif
}

uint Device::imageBaseAddressAlignment() const
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    ཕྱིར p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#དེ་མིན
    ཕྱིར 0;
#endif
}

size_t Device::image2DMaxWidth() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() const
#ifdef CL_VERSION_1_2
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#དེ་མིན
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() const
#ifdef CL_VERSION_1_2
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#དེ་མིན
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

bool Device::intelSubgroupsSupport() const
{ ཕྱིར p ? p->intelSubgroupsSupport_ : false; }

ཧྲིལ Device::maxClockFrequency() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

ཧྲིལ Device::maxComputeUnits() const
{ ཕྱིར p ? p->maxComputeUnits_ : 0; }

ཧྲིལ Device::maxConstantArgs() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() const
{ ཕྱིར p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

ཧྲིལ Device::maxReadImageArgs() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

ཧྲིལ Device::maxWriteImageArgs() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

ཧྲིལ Device::maxSamplers() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() const
{ ཕྱིར p ? p->maxWorkGroupSize_ : 0; }

ཧྲིལ Device::maxWorkItemDims() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

void Device::maxWorkItemSizes(size_t* sizes) const
{
    གལ་ཏེ(p)
    {
        const ཧྲིལ MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*parimaa_na(sizes[0]), &sizes[0], &retsz));
    }
}

ཧྲིལ Device::memBaseAddrAlign() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

ཧྲིལ Device::nativeVectorWidthChar() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

ཧྲིལ Device::nativeVectorWidthShort() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

ཧྲིལ Device::nativeVectorWidthInt() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

ཧྲིལ Device::nativeVectorWidthLong() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

ཧྲིལ Device::nativeVectorWidthFloat() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

ཧྲིལ Device::nativeVectorWidthDouble() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

ཧྲིལ Device::nativeVectorWidthHalf() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

ཧྲིལ Device::preferredVectorWidthChar() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

ཧྲིལ Device::preferredVectorWidthShort() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

ཧྲིལ Device::preferredVectorWidthInt() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

ཧྲིལ Device::preferredVectorWidthLong() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

ཧྲིལ Device::preferredVectorWidthFloat() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

ཧྲིལ Device::preferredVectorWidthDouble() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

ཧྲིལ Device::preferredVectorWidthHalf() const
{ ཕྱིར p ? p->getProp<cl_uint, ཧྲིལ>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() const
#ifdef CL_VERSION_1_2
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#དེ་མིན
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() const
{ ཕྱིར p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

const Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    གལ་ཏེ (!c.empty())
    {
        ཕྱིར c.getDevice();
    }

    static Device dummy;
    ཕྱིར dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
anta_hstha cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, NULL, &required);
    གལ་ཏེ (err != CL_SUCCESS)
        ཕྱིར err;

    param.clear();
    གལ་ཏེ (required > 0)
    {
        AutoBuffer<var_na> buf(required + 1);
        var_na* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, NULL);
        གལ་ཏེ (err != CL_SUCCESS)
            ཕྱིར err;
        param = ptr;
    }

    ཕྱིར CL_SUCCESS;
}

static void split(const std::string &s, var_na delim, std::vector<std::string> &elems)
{
    elems.clear();
    གལ་ཏེ (s.size() == 0)
        ཕྱིར;
    std::istringstream ss(s);
    std::string item;
    སྐབས (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
static bool parseOpenCLDeviceConfiguration(const std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    གལ་ཏེ (parts.size() > 3)
    {
        CV_LOG_ERROR(NULL, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ཕྱིར false;
    }
    གལ་ཏེ (parts.size() > 2)
        deviceNameOrID = parts[2];
    གལ་ཏེ (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    གལ་ཏེ (parts.size() > 0)
    {
        platform = parts[0];
    }
    ཕྱིར true;
}

static cl_device_id selectOpenCLDevice(const std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    གལ་ཏེ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    གལ་ཏེ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ཕྱིར NULL;

    bool isID = false;
    ཧྲིལ deviceID = -1;
    གལ་ཏེ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = true;
        ཆེད་དུ (size_t i = 0; i < deviceName.length(); i++)
        {
            གལ་ཏེ (!isdigit(deviceName[i]))
            {
                isID = false;
                bha_nga;
            }
        }
        གལ་ཏེ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            གལ་ཏེ (deviceID < 0)
                ཕྱིར NULL;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint numPlatforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, NULL, &numPlatforms));

        གལ་ཏེ (numPlatforms == 0)
            ཕྱིར NULL;
        platforms.resize((size_t)numPlatforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(numPlatforms, &platforms[0], &numPlatforms));
        platforms.resize(numPlatforms);
    }

    གལ་ཏེ (platform.length() > 0)
    {
        ཆེད་དུ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            གལ་ཏེ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            དེ་མིན
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        གལ་ཏེ (platforms.size() == 0)
        {
            CV_LOG_ERROR(NULL, "OpenCL: Can't find OpenCL platform by name: " << platform);
            goto not_found;
        }
    }
    གལ་ཏེ (deviceTypes.size() == 0)
    {
        གལ་ཏེ (!isID)
        {
            deviceTypes.push_back("GPU");
            གལ་ཏེ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        དེ་མིན
            deviceTypes.push_back("ALL");
    }
    ཆེད་དུ (size_t t = 0; t < deviceTypes.size(); t++)
    {
        ཧྲིལ deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        གལ་ཏེ (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        དེ་མིན གལ་ཏེ (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        དེ་མིན གལ་ཏེ (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        དེ་མིན གལ་ཏེ (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        དེ་མིན
        {
            CV_LOG_ERROR(NULL, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            goto not_found;
        }

        std::vector<cl_device_id> devices;
        ཆེད་དུ (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint count = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, NULL, &count);
            གལ་ཏེ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            གལ་ཏེ (count == 0)
                continue;
            size_t base = devices.size();
            devices.resize(base + count);
            status = clGetDeviceIDs(*currentPlatform, deviceType, count, &devices[base], &count);
            གལ་ཏེ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        ཆེད་དུ (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : true) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = true;
            གལ་ཏེ(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, parimaa_na(isIGPU), &isIGPU, NULL));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            གལ་ཏེ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ཕྱིར devices[i];
            }
        }
    }

not_found:
    གལ་ཏེ (configuration.empty())
        ཕྱིར NULL; // suppress messages on stderr

    std::ostringstream msg;
    msg << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    ཆེད་དུ (size_t t = 0; t < deviceTypes.size(); t++)
        msg << ' ' << deviceTypes[t];

    msg << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(NULL, msg.str());
    ཕྱིར NULL;
}

#ifdef HAVE_OPENCL_SVM
namespace svm {

ga_nanaa AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

static bool checkForceSVMUmatUsage()
{
    static bool initialized = false;
    static bool force = false;
    གལ་ཏེ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", false);
        initialized = true;
    }
    ཕྱིར force;
}
static bool checkDisableSVMUMatUsage()
{
    static bool initialized = false;
    static bool force = false;
    གལ་ཏེ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", false);
        initialized = true;
    }
    ཕྱིར force;
}
static bool checkDisableSVM()
{
    static bool initialized = false;
    static bool force = false;
    གལ་ཏེ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", false);
        initialized = true;
    }
    ཕྱིར force;
}
// see SVMCapabilities
static unsigned ཧྲིལ getSVMCapabilitiesMask()
{
    static bool initialized = false;
    static unsigned ཧྲིལ mask = 0;
    གལ་ཏེ (!initialized)
    {
        const std::string envValue = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        གལ་ཏེ (envValue.empty())
        {
            ཕྱིར ~0U; // all bits 1
        }
        mask = atoi(envValue.c_str());
        initialized = true;
    }
    ཕྱིར mask;
}
} // namespace
#endif

static size_t getProgramCountLimit()
{
    static bool initialized = false;
    static size_t count = 0;
    གལ་ཏེ (!initialized)
    {
        count = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = true;
    }
    ཕྱིར count;
}

static ཧྲིལ g_contextId = 0;

class OpenCLBufferPoolImpl;
class OpenCLSVMBufferPoolImpl;

sa_nracanaa Context::Impl
{
    static Context::Impl* get(Context& context) { ཕྱིར context.getImpl(); }

    typedef std::deque<Context::Impl*> container_t;
    static container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        static container_t* g_contexts = new container_t();
        ཕྱིར *g_contexts;
    }

rak_sita:
    Impl(const std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(false)
#endif
    {
        གལ་ཏེ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = this;
    }

    ~Impl()
    {
#ifdef _WIN32
        གལ་ཏེ (!cv::__termination)
#endif
        {
            གལ་ཏེ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = NULL;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = NULL;
        }
    }

    void init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_NUM_DEVICES, parimaa_na(ndevices), &ndevices, NULL));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * parimaa_na(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * parimaa_na(cl_device_id), "");

        devices.clear();
        ཆེད་དུ (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    void __init_buffer_pools();  // w/o synchronization
    void _init_buffer_pools() const
    {
        གལ་ཏེ (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            གལ་ཏེ (!bufferPool_)
            {
                const_cast<Impl*>(this)->__init_buffer_pools();
            }
        }
    }
public:
    static Impl* findContext(const std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        གལ་ཏེ (configuration.empty() && !container.empty())
            ཕྱིར container[0];
        ཆེད་དུ (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            གལ་ཏེ (i && i->configuration == configuration)
            {
                ཕྱིར i;
            }
        }
        ཕྱིར NULL;
    }

    static Impl* findOrCreateContext(const std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        གལ་ཏེ (configuration_.empty())
        {
            const std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            གལ་ཏེ (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        གལ་ཏེ (impl)
        {
            CV_LOG_INFO(NULL, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ཕྱིར impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        གལ་ཏེ (d == NULL)
            ཕྱིར NULL;

        impl = new Impl(configuration);
        try
        {
            impl->createFromDevice(d);
            གལ་ཏེ (impl->handle)
                ཕྱིར impl;
            delete impl;
            ཕྱིར NULL;
        }
        gr_haa_na (...)
        {
            delete impl;
            k_sipa;
        }
    }

    static Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (void*)h);
        Impl* impl = findContext(configuration);
        གལ་ཏེ (impl)
        {
            CV_LOG_INFO(NULL, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ཕྱིར impl;
        }

        impl = new Impl(configuration);
        try
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ཕྱིར impl;
        }
        gr_haa_na (...)
        {
            delete impl;
            k_sipa;
        }
    }

    static Impl* findOrCreateContext(const ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (void*)d);
        Impl* impl = findContext(configuration);
        གལ་ཏེ (impl)
        {
            CV_LOG_INFO(NULL, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ཕྱིར impl;
        }

        impl = new Impl(configuration);
        try
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ཕྱིར impl;
        }
        gr_haa_na (...)
        {
            delete impl;
            k_sipa;
        }
    }

    void setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        གལ་ཏེ (d == NULL)
            ཕྱིར;

        createFromDevice(d);
    }

    void createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == NULL);

        cl_platform_id pl = NULL;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, parimaa_na(cl_platform_id), &pl, NULL));

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

        bool ok = handle != 0 && status == CL_SUCCESS;
        གལ་ཏེ( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        དེ་མིན
            handle = NULL;
    }

    Program getProg(const ProgramSource& src, const String& buildflags, String& errmsg);

    void unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        ཆེད་དུ (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              གལ་ཏེ (it != phash.end())
              {
                  གལ་ཏེ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ཕྱིར;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        གལ་ཏེ (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            གལ་ཏེ (prefix.empty())
            {
                CV_Assert(!devices.empty());
                const Device& d = devices[0];
                ཧྲིལ bits = d.addressBits();
                གལ་ཏེ (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                ཆེད་དུ (size_t i = 0; i < prefix.size(); i++)
                {
                    var_na c = prefix[i];
                    གལ་ཏེ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        ཕྱིར prefix;
    }

    std::string& getPrefixBase()
    {
        གལ་ཏེ (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            གལ་ཏེ (prefix_base.empty())
            {
                const Device& d = devices[0];
                ཧྲིལ bits = d.addressBits();
                གལ་ཏེ (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                ཆེད་དུ (size_t i = 0; i < prefix_base.size(); i++)
                {
                    var_na c = prefix_base[i];
                    གལ་ཏེ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        ཕྱིར prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    const ཧྲིལ contextId;  // global unique ID
    const std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    typedef std::map<std::string, Program> phash_t;
    phash_t phash;
    typedef std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() const
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        ཕྱིར *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() const
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        ཕྱིར *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    void setUserContext(std::type_index typeId, const std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        གལ་ཏེ (it != userContextStorage.end())
            ཕྱིར it->second;
        དེ་མིན
            ཕྱིར nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    bool svmInitialized;
    bool svmAvailable;
    bool svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    void svmInit()
    {
        CV_Assert(handle != NULL);
        const Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((void)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, parimaa_na(deviceCaps), &deviceCaps, NULL);
        གལ་ཏེ (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            goto noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (ཧྲིལ)deviceCaps);
        CV_Assert(((void)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.value_ =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.value_ &= svm::getSVMCapabilitiesMask();
        གལ་ཏེ (svmCapabilities.value_ == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            goto noSVM;
        }
        try
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            void* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            གལ་ཏེ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            try
            {
                bool error = false;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                གལ་ཏེ (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, NULL, NULL))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                try
                {
                    ((ཧྲིལ*)ptr)[0] = 100;
                }
                gr_haa_na (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = true;
                }
                གལ་ཏེ (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, NULL, NULL))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                གལ་ཏེ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            gr_haa_na (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                k_sipa;
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
        gr_haa_na (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            try
            {
                // Try HSA extension
                String extensions = device.extensions();
                གལ་ཏེ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    goto noSVM;
                }
                cl_platform_id p = NULL;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, parimaa_na(cl_platform_id), &p, NULL));
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
            gr_haa_na (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                goto noSVM;
            }
        }

        svmAvailable = true;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = true;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ཕྱིར;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = false;
        svmEnabled = false;
        svmCapabilities.value_ = 0;
        svmInitialized = true;
        svmFunctions.fn_clSVMAlloc = NULL;
        ཕྱིར;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() const
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        ཕྱིར *bufferPoolSVM_.get();
    }
#endif

    friend class Program;
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
Context::Context(ཧྲིལ dtype)
{
    p = 0;
    create(dtype);
}

void Context::release()
{
    གལ་ཏེ (p)
    {
        p->release();
        p = NULL;
    }
}

bool Context::create()
{
    release();
    གལ་ཏེ (!haveOpenCL())
        ཕྱིར false;
    p = Impl::findOrCreateContext(std::string());
    གལ་ཏེ (p && p->handle)
        ཕྱིར true;
    release();
    ཕྱིར false;
}

// deprecated
bool Context::create(ཧྲིལ dtype)
{
    གལ་ཏེ( !haveOpenCL() )
        ཕྱིར false;
    release();
    གལ་ཏེ (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    དེ་མིན གལ་ཏེ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    དེ་མིན གལ་ཏེ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    དེ་མིན
    {
        CV_LOG_ERROR(NULL, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    གལ་ཏེ (p && !p->handle)
    {
        release();
    }
    ཕྱིར p != 0;
}

Context::Context(const Context& c)
{
    p = (Impl*)c.p;
    གལ་ཏེ(p)
        p->addref();
}

Context& Context::operator = (const Context& c)
{
    Impl* newp = (Impl*)c.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    གལ་ཏེ (this != &c) {
        གལ་ཏེ(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    ཕྱིར *this;
}

void* Context::ptr() const
{
    ཕྱིར p == NULL ? NULL : p->handle;
}

size_t Context::ndevices() const
{
    ཕྱིར p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) const
{
    static Device dummy;
    ཕྱིར !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    གལ་ཏེ (!c.empty())
    {
        auto& ctx = c.getContext();
        ཕྱིར ctx;
    }

    static Context dummy;
    ཕྱིར dummy;
}

Program Context::getProg(const ProgramSource& prog,
                         const String& buildopts, String& errmsg)
{
    ཕྱིར p ? p->getProg(prog, buildopts, errmsg) : Program();
}

void Context::unloadProg(Program& prog)
{
    གལ་ཏེ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(void* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    ཕྱིར ctx;
}

/* static */
Context Context::fromDevice(const ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    ཕྱིར ctx;
}

/* static */
Context Context::create(const std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    ཕྱིར ctx;
}

void* Context::getOpenCLContextProperty(ཧྲིལ propertyId) const
{
    གལ་ཏེ (p == NULL)
        ཕྱིར nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, NULL, &size));
    std::vector<cl_context_properties> prop(size / parimaa_na(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), NULL));
    ཆེད་དུ (size_t i = 0; i < prop.size(); i += 2)
    {
        གལ་ཏེ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(NULL, "OpenCL: found context property=" << propertyId << ") => " << (void*)prop[i + 1]);
            ཕྱིར (void*)prop[i + 1];
        }
    }
    ཕྱིར nullptr;
}

#ifdef HAVE_OPENCL_SVM
bool Context::useSVM() const
{
    Context::Impl* i = p;
    CV_Assert(i);
    གལ་ཏེ (!i->svmInitialized)
        i->svmInit();
    ཕྱིར i->svmEnabled;
}
void Context::setUseSVM(bool enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    གལ་ཏེ (!i->svmInitialized)
        i->svmInit();
    གལ་ཏེ (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#དེ་མིན
bool Context::useSVM() const { ཕྱིར false; }
void Context::setUseSVM(bool enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
namespace svm {

const SVMCapabilities getSVMCapabilitites(const ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    གལ་ཏེ (!i->svmInitialized)
        i->svmInit();
    ཕྱིར i->svmCapabilities;
}

CV_EXPORTS const SVMFunctions* getSVMFunctions(const ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != NULL);
    ཕྱིར &i->svmFunctions;
}

CV_EXPORTS bool useSVM(UMatUsageFlags usageFlags)
{
    གལ་ཏེ (checkForceSVMUmatUsage())
        ཕྱིར true;
    གལ་ཏེ (checkDisableSVMUMatUsage())
        ཕྱིར false;
    གལ་ཏེ ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        ཕྱིར true;
    ཕྱིར false; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

void Context::setUserContext(std::type_index typeId, const std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    ཕྱིར p->getUserContext(typeId);
}

static void get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<var_na> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
void attachContext(const String& platformName, void* platformID, void* context, void* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        const std::string& platformName, void* platformID, void* context, void* deviceID
)
{
    གལ་ཏེ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    གལ་ཏེ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    bool platformAvailable = false;

    // check if external platformName contained in list of available platforms in OpenCV
    ཆེད་དུ (unsigned ཧྲིལ i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        གལ་ཏེ (platformName == availablePlatformName)
        {
            platformAvailable = true;
            bha_nga;
        }
    }

    གལ་ཏེ (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    གལ་ཏེ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ཕྱིར ctx;
}

void initializeContextFromHandle(Context& ctx, void* _platform, void* _context, void* _device)
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

sa_nracanaa Queue::Impl
{
    anta_hstha void __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = false;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, parimaa_na(cl_command_queue_properties), &props, NULL));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, bool isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(const Context& c, const Device& d, bool withProfiling = false)
    {
        __init();

        const Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        གལ་ཏེ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        གལ་ཏེ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        གལ་ཏེ (!cv::__termination)
#endif
        {
            གལ་ཏེ(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = NULL;
            }
        }
    }

    const cv::ocl::Queue& getProfilingQueue(const cv::ocl::Queue& self)
    {
        གལ་ཏེ (isProfilingQueue_)
            ཕྱིར self;

        གལ་ཏེ (profiling_queue_.ptr())
            ཕྱིར profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, parimaa_na(cl_context), &ctx, NULL));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, parimaa_na(cl_device_id), &device, NULL));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = new Impl(q, true);
        profiling_queue_ = queue;

        ཕྱིར profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    bool isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(const Context& c, const Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(const Queue& q)
{
    p = q.p;
    གལ་ཏེ(p)
        p->addref();
}

Queue& Queue::operator = (const Queue& q)
{
    Impl* newp = (Impl*)q.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    གལ་ཏེ (this != &q) {
        གལ་ཏེ(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    ཕྱིར *this;
}

Queue::~Queue()
{
    གལ་ཏེ(p)
        p->release();
}

bool Queue::create(const Context& c, const Device& d)
{
    གལ་ཏེ(p)
        p->release();
    p = new Impl(c, d);
    ཕྱིར p->handle != 0;
}

void Queue::finish()
{
    གལ་ཏེ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

const Queue& Queue::getProfilingQueue() const
{
    CV_Assert(p);
    ཕྱིར p->getProfilingQueue(*this);
}

void* Queue::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    གལ་ཏེ (!c.empty())
    {
        auto& q = c.getQueue();
        ཕྱིར q;
    }
    static Queue dummy;
    ཕྱིར dummy;
}

static cl_command_queue getQueue(const Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    གལ་ཏེ(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    ཕྱིར qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(ཧྲིལ _flags, UMat* _m, ཧྲིལ _wscale, ཧྲིལ _iwscale, const void* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != NULL);
}

KernelArg KernelArg::Constant(const Mat& m)
{
    CV_Assert(m.isContinuous());
    ཕྱིར KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

sa_nracanaa Kernel::Impl
{
    Impl(const var_na* kname, const Program& prog) :
        refcount(1), handle(NULL), isInProgress(false), isAsyncRun(false), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        གལ་ཏེ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        ཆེད་དུ( ཧྲིལ i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = false;
        haveTempSrcUMats = false;
    }

    void cleanupUMats()
    {
        bool exceptionOccurred = false;
        ཆེད་དུ( ཧྲིལ i = 0; i < MAX_ARRS; i++ )
        {
            གལ་ཏེ( u[i] )
            {
                གལ་ཏེ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    try
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    gr_haa_na(const std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(NULL, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = true;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = false;
        haveTempSrcUMats = false;
        CV_Assert(!exceptionOccurred);
    }

    void addUMat(const UMat& m, bool dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        གལ་ཏེ(dst && m.u->tempUMat())
            haveTempDstUMats = true;
        གལ་ཏེ(m.u->originalUMatData == NULL && m.u->tempUMat())
            haveTempSrcUMats = true;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    void registerImageArgument(ཧྲིལ arg, const Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        གལ་ཏེ (arg < (ཧྲིལ)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    void finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = false;
        try
        {
            cleanupUMats();
        }
        gr_haa_na(...)
        {
            release();
            k_sipa;
        }
        release();
    }

    bool run(ཧྲིལ dims, size_t _globalsize[], size_t _localsize[],
            bool sync, int64* timeNS, const Queue& q);

    ~Impl()
    {
        གལ་ཏེ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ga_nanaa { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    bool isInProgress;
    bool isAsyncRun;  // true if kernel was scheduled in async mode
    ཧྲིལ nu;
    std::vector<Image2D> shadow_images;
    bool haveTempDstUMats;
    bool haveTempSrcUMats;
};

}} // namespace cv::ocl

extern "C" {

static void CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, void *p)
{
    try
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    gr_haa_na (const cv::Exception& exc)
    {
        CV_LOG_ERROR(NULL, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    gr_haa_na (const std::exception& exc)
    {
        CV_LOG_ERROR(NULL, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    gr_haa_na (...)
    {
        CV_LOG_ERROR(NULL, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

namespace cv { namespace ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(const var_na* kname, const Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(const var_na* kname, const ProgramSource& src,
               const String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(const Kernel& k)
{
    p = k.p;
    གལ་ཏེ(p)
        p->addref();
}

Kernel& Kernel::operator = (const Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    གལ་ཏེ (this != &k) {
        གལ་ཏེ(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    ཕྱིར *this;
}

Kernel::~Kernel()
{
    གལ་ཏེ(p)
        p->release();
}

bool Kernel::create(const var_na* kname, const Program& prog)
{
    གལ་ཏེ(p)
        p->release();
    p = new Impl(kname, prog);
    གལ་ཏེ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    ཕྱིར p != 0;
}

bool Kernel::create(const var_na* kname, const ProgramSource& src,
                    const String& buildopts, String* errmsg)
{
    གལ་ཏེ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    གལ་ཏེ( !errmsg ) errmsg = &tempmsg;
    const Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ཕྱིར create(kname, prog);
}

void* Kernel::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

bool Kernel::empty() const
{
    ཕྱིར ptr() == 0;
}

static cv::String dumpValue(size_t sz, const void* p)
{
    གལ་ཏེ (!p)
        ཕྱིར "NULL";
    གལ་ཏེ (sz == 2)
        ཕྱིར cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    གལ་ཏེ (sz == 4)
        ཕྱིར cv::format("%d / %uu / 0x%08x / %g", *(ཧྲིལ*)p, *(ཧྲིལ*)p, *(ཧྲིལ*)p, *(float*)p);
    གལ་ཏེ (sz == 8)
        ཕྱིར cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(dviguu_na*)p);
    ཕྱིར cv::format("%p", p);
}

ཧྲིལ Kernel::set(ཧྲིལ i, const void* value, size_t sz)
{
    གལ་ཏེ (!p || !p->handle)
        ཕྱིར -1;
    གལ་ཏེ (i < 0)
        ཕྱིར i;
    གལ་ཏེ( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (ཧྲིལ)i, (ཧྲིལ)sz, dumpValue(sz, value).c_str()).c_str());
    གལ་ཏེ (retval != CL_SUCCESS)
        ཕྱིར -1;
    ཕྱིར i+1;
}

ཧྲིལ Kernel::set(ཧྲིལ i, const Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    ཧྲིལ res = set(i, &h, parimaa_na(h));
    གལ་ཏེ (res >= 0)
        p->registerImageArgument(i, image2D);
    ཕྱིར res;
}

ཧྲིལ Kernel::set(ཧྲིལ i, const UMat& m)
{
    ཕྱིར set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
static bool isCudaBacked(const UMatData* u)
{
    const MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ཕྱིར u && cudaAllocator && u->currAllocator == cudaAllocator;
}

ཧྲིལ Kernel::set(ཧྲིལ i, const KernelArg& arg)
{
    གལ་ཏེ( !p || !p->handle )
        ཕྱིར -1;
    གལ་ཏེ (i < 0)
    {
        CV_LOG_ERROR(NULL, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (ཧྲིལ)i));
        ཕྱིར i;
    }
    གལ་ཏེ( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    གལ་ཏེ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        bool ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        གལ་ཏེ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)NULL;
            status = clSetKernelArg(p->handle, (cl_uint)i, parimaa_na(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (ཧྲིལ)i).c_str());
            ཕྱིར i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        གལ་ཏེ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(NULL, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (ཧྲིལ)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ཕྱིར -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        གལ་ཏེ (!h)
        {
            CV_LOG_ERROR(NULL, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (ཧྲིལ)i, (ཧྲིལ)arg.flags, arg.m));
            p->release();
            p = 0;
            ཕྱིར -1;
        }

#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            const Context& ctx = Context::getDefault();
            const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#གལ་ཏེ 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#དེ་མིན
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (ཧྲིལ)i, (void*)svmDataPtr).c_str());
        }
        དེ་མིན
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, parimaa_na(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (ཧྲིལ)i, (void*)h).c_str());
        }

        གལ་ཏེ (ptronly)
        {
            i++;
        }
        དེ་མིན གལ་ཏེ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), parimaa_na(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ཧྲིལ)(i+1), (ཧྲིལ)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), parimaa_na(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ཧྲིལ)(i+2), (ཧྲིལ)u2d.offset).c_str());
            i += 3;

            གལ་ཏེ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ཧྲིལ cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, parimaa_na(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ཧྲིལ)i, (ཧྲིལ)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), parimaa_na(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ཧྲིལ)(i+1), (ཧྲིལ)cols).c_str());
                i += 2;
            }
        }
        དེ་མིན
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), parimaa_na(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (ཧྲིལ)(i+1), (ཧྲིལ)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), parimaa_na(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (ཧྲིལ)(i+2), (ཧྲིལ)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), parimaa_na(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (ཧྲིལ)(i+3), (ཧྲིལ)u3d.offset).c_str());
            i += 4;
            གལ་ཏེ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                ཧྲིལ cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, parimaa_na(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (ཧྲིལ)i, (ཧྲིལ)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), parimaa_na(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (ཧྲིལ)(i+1), (ཧྲིལ)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), parimaa_na(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (ཧྲིལ)(i+2), (ཧྲིལ)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ཕྱིར i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (ཧྲིལ)i, (ཧྲིལ)arg.sz, (void*)arg.obj).c_str());
    ཕྱིར i+1;
}

bool Kernel::run(ཧྲིལ dims, size_t _globalsize[], size_t _localsize[],
                 bool sync, const Queue& q)
{
    གལ་ཏེ (!p)
        ཕྱིར false;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != NULL);
    ཆེད་དུ (ཧྲིལ i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(ཧྲིལ)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        གལ་ཏེ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned ཧྲིལ)val) * val;
    }
    CV_Assert(total > 0);

    ཕྱིར p->run(dims, globalsize, _localsize, sync, NULL, q);
}


bool Kernel::run_(ཧྲིལ dims, size_t _globalsize[], size_t _localsize[],
                  bool sync, const Queue& q)
{
    CV_Assert(p);
    ཕྱིར p->run(dims, _globalsize, _localsize, sync, NULL, q);
}


bool Kernel::Impl::run(ཧྲིལ dims, size_t globalsize[], size_t localsize[],
        bool sync, int64* timeNS, const Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    གལ་ཏེ (!handle)
    {
        CV_LOG_ERROR(NULL, "OpenCL kernel has zero handle: " << name);
        ཕྱིར false;
    }

    གལ་ཏེ (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !sync;

    གལ་ཏེ (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#གལ་ཏེ CV_OPENCL_SYNC_RUN_KERNELS
    sync = true;
#endif

    cl_command_queue qq = getQueue(q);
    གལ་ཏེ (haveTempDstUMats)
        sync = true;
    གལ་ཏེ (haveTempSrcUMats)
        sync = true;
    གལ་ཏེ (timeNS)
        sync = true;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           NULL, globalsize, localsize, 0, 0,
                                           (sync && !timeNS) ? 0 : &asyncEvent);
#གལ་ཏེ !CV_OPENCL_SHOW_RUN_KERNELS
    གལ་ཏེ (retval != CL_SUCCESS)
#endif
    {
        cv::String msg = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (ཧྲིལ)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        sync ? "true" : "false"
                        );
        གལ་ཏེ (retval != CL_SUCCESS)
        {
            msg = CV_OCL_API_ERROR_MSG(retval, msg.c_str());
        }
#གལ་ཏེ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, msg.c_str());
#དེ་མིན
        printf("%s\n", msg.c_str());
        fflush(stdout);
#endif
    }
    གལ་ཏེ (sync || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        གལ་ཏེ (timeNS)
        {
            གལ་ཏེ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, parimaa_na(startTime), &startTime, NULL));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, parimaa_na(stopTime), &stopTime, NULL));
                *timeNS = (int64)(stopTime - startTime);
            }
            དེ་མིན
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    དེ་མིན
    {
        addref();
        isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, this));
    }
    གལ་ཏེ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ཕྱིར retval == CL_SUCCESS;
}

bool Kernel::runTask(bool sync, const Queue& q)
{
    གལ་ཏེ(!p || !p->handle || p->isInProgress)
        ཕྱིར false;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, sync ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), sync ? "true" : "false").c_str());
    གལ་ཏེ (sync || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    དེ་མིན
    {
        p->addref();
        p->isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    གལ་ཏེ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ཕྱིར retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(ཧྲིལ dims, size_t globalsize[], size_t localsize[], const Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    bool res = p->run(dims, globalsize, localsize, true, &timeNs, profilingQueue);
    ཕྱིར res ? timeNs : -1;
}

size_t Kernel::workGroupSize() const
{
    གལ་ཏེ(!p || !p->handle)
        ཕྱིར 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, parimaa_na(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ཕྱིར status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() const
{
    གལ་ཏེ(!p || !p->handle)
        ཕྱིར 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, parimaa_na(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ཕྱིར status == CL_SUCCESS ? val : 0;
}

bool Kernel::compileWorkGroupSize(size_t wsz[]) const
{
    གལ་ཏེ(!p || !p->handle || !wsz)
        ཕྱིར 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, parimaa_na(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ཕྱིར status == CL_SUCCESS;
}

size_t Kernel::localMemSize() const
{
    གལ་ཏེ(!p || !p->handle)
        ཕྱིར 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, parimaa_na(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ཕྱིར status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

sa_nracanaa ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ga_nanaa KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(const String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(const String& module, const String& name, const String& codeStr, const String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    void init(ga_nanaa KIND kind, const String& module, const String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = NULL;
        sourceSize_ = 0;
        isHashUpdated = false;
    }

    void initFromSource(const String& codeStr, const String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        གལ་ཏེ (sourceHash_.empty())
        {
            updateHash();
        }
        དེ་མིན
        {
            isHashUpdated = true;
        }
    }

    void updateHash(const var_na* hashStr = NULL)
    {
        གལ་ཏེ (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = true;
            ཕྱིར;
        }
        uint64 hash = 0;
        switch (kind_)
        {
        pak_sa PROGRAM_SOURCE_CODE:
            གལ་ཏེ (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            དེ་མིན
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            bha_nga;
        pak_sa PROGRAM_BINARIES:
        pak_sa PROGRAM_SPIR:
        pak_sa PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            bha_nga;
        default:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = true;
    }

    Impl(ga_nanaa KIND kind,
            const String& module, const String& name,
            const unsigned var_na* binary, const size_t size,
            const cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    static ProgramSource fromSourceWithStaticLifetime(const String& module, const String& name,
            const var_na* sourceCodeStaticStr, const var_na* hashStaticStr,
            const cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = new Impl(PROGRAM_SOURCE_CODE, module, name,
                (const unsigned var_na*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        ཕྱིར result;
    }

    static ProgramSource fromBinary(const String& module, const String& name,
            const unsigned var_na* binary, const size_t size,
            const cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = new Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        ཕྱིར result;
    }

    static ProgramSource fromSPIR(const String& module, const String& name,
            const unsigned var_na* binary, const size_t size,
            const cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = new Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        ཕྱིར result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    const unsigned var_na* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    bool isHashUpdated;

    friend sa_nracanaa Program::Impl;
    friend sa_nracanaa internal::ProgramEntry;
    friend sa_nracanaa Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(const String& module, const String& name, const String& codeStr, const String& codeHash)
{
    p = new Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(const var_na* prog)
{
    p = new Impl(prog);
}

ProgramSource::ProgramSource(const String& prog)
{
    p = new Impl(prog);
}

ProgramSource::~ProgramSource()
{
    གལ་ཏེ(p)
        p->release();
}

ProgramSource::ProgramSource(const ProgramSource& prog)
{
    p = prog.p;
    གལ་ཏེ(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (const ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    གལ་ཏེ (this != &prog) {
        གལ་ཏེ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ཕྱིར *this;
}

const String& ProgramSource::source() const
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == NULL); // method returns reference - can't construct temporary object
    ཕྱིར p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() const
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::fromBinary(const String& module, const String& name,
        const unsigned var_na* binary, const size_t size,
        const cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ཕྱིར Impl::fromBinary(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(const String& module, const String& name,
        const unsigned var_na* binary, const size_t size,
        const cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ཕྱིར Impl::fromBinary(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() const
{
    གལ་ཏེ (this->pProgramSource == NULL)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        གལ་ཏེ (this->pProgramSource == NULL)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(this->module, this->name, this->programCode, this->programHash, cv::String());
            ProgramSource* ptr = new ProgramSource(ps);
            const_cast<ProgramEntry*>(this)->pProgramSource = ptr;
        }
    }
    ཕྱིར *this->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

static
cv::String joinBuildOptions(const cv::String& a, const cv::String& b)
{
    གལ་ཏེ (b.empty())
        ཕྱིར a;
    གལ་ཏེ (a.empty())
        ཕྱིར b;
    གལ་ཏེ (b[0] == ' ')
        ཕྱིར a + b;
    ཕྱིར a + (cv::String(" ") + b);
}

sa_nracanaa Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(const ProgramSource& src,
         const String& _buildflags, String& errmsg) :
         refcount(1),
         handle(NULL),
         buildflags(_buildflags)
    {
        const ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        const Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        གལ་ཏེ (ctx.ptr() == NULL || device.ptr() == NULL)
            ཕྱིར;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        གལ་ཏེ (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            གལ་ཏེ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            དེ་མིན གལ་ཏེ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            const String param_buildExtraOptions = getBuildExtraOptions();
            གལ་ཏེ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#གལ་ཏེ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(NULL, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#གལ་ཏེ CV_OPENCL_SHOW_BUILD_KERNELS
        གལ་ཏེ (handle)
        {
            size_t retsz = 0;
            var_na kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, parimaa_na(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            གལ་ཏེ (result == CL_SUCCESS && retsz < parimaa_na(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(NULL, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            དེ་མིན
            {
                CV_LOG_ERROR(NULL, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    bool compile(const Context& ctx, const ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        གལ་ཏེ (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(NULL, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            bool isLoaded = createFromBinary(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ཕྱིར isLoaded;
        }
        ཕྱིར compileWithCache(ctx, src_, errmsg);
    }

    bool compileWithCache(const Context& ctx, const ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#གལ་ཏེ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        const std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        const String& hash_str = src_->sourceHash_;
        cv::String fname;
        གལ་ཏེ (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        const cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        གལ་ཏེ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            try
            {
                std::vector<var_na> binaryBuf;
                bool res = false;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    BinaryProgramFile file(fname, hash_str.c_str());
                    res = file.read(buildflags, binaryBuf);
                }
                གལ་ཏེ (res)
                {
                    CV_Assert(!binaryBuf.empty());
                    CV_LOG_VERBOSE(NULL, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    bool isLoaded = createFromBinary(ctx, binaryBuf, errmsg);
                    གལ་ཏེ (isLoaded)
                        ཕྱིར true;
                }
            }
            gr_haa_na (const cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(NULL, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            gr_haa_na (...)
            {
                CV_LOG_VERBOSE(NULL, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == NULL);
        གལ་ཏེ (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            གལ་ཏེ (!buildFromSources(ctx, src_, errmsg))
            {
                ཕྱིར false;
            }
        }
        དེ་མིན གལ་ཏེ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            གལ་ཏེ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(NULL, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            bool isLoaded = createFromBinary(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            གལ་ཏེ (!isLoaded)
                ཕྱིར false;
        }
        དེ་མིན གལ་ཏེ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        དེ་མིན
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != NULL);
#གལ་ཏེ OPENCV_HAVE_FILESYSTEM_SUPPORT
        གལ་ཏེ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            try
            {
                std::vector<var_na> binaryBuf;
                getProgramBinary(binaryBuf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    BinaryProgramFile file(fname, hash_str.c_str());
                    file.write(buildflags, binaryBuf);
                }
            }
            gr_haa_na (const cv::Exception& e)
            {
                CV_LOG_WARNING(NULL, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            gr_haa_na (...)
            {
                CV_LOG_WARNING(NULL, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#གལ་ཏེ CV_OPENCL_VALIDATE_BINARY_PROGRAMS
        གལ་ཏེ (CV_OPENCL_VALIDATE_BINARY_PROGRAMS_VALUE)
        {
            std::vector<var_na> binaryBuf;
            getProgramBinary(binaryBuf);
            གལ་ཏེ (!binaryBuf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = NULL;
                createFromBinary(ctx, binaryBuf, errmsg);
            }
        }
#endif
        ཕྱིར handle != NULL;
    }

    void dumpBuildLog_(cl_int result, const cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<var_na, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        གལ་ཏེ (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            གལ་ཏེ (log_retval == CL_SUCCESS)
            {
                གལ་ཏེ (retsz < buffer.size())
                    buffer[retsz] = 0;
                དེ་མིན
                    buffer[buffer.size() - 1] = 0;
            }
            དེ་མིན
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

    bool buildFromSources(const Context& ctx, const ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == NULL);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(NULL, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        const var_na* srcptr = src_->sourceAddr_ ? ((const var_na*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != NULL);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        གལ་ཏེ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            ཆེད་དུ (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#གལ་ཏེ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            གལ་ཏེ (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                གལ་ཏེ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = NULL;
                }
                གལ་ཏེ (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#གལ་ཏེ CV_OPENCL_VALIDATE_BINARY_PROGRAMS
            གལ་ཏེ (handle && CV_OPENCL_VALIDATE_BINARY_PROGRAMS_VALUE)
            {
                CV_LOG_INFO(NULL, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                var_na kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, parimaa_na(kernels_buffer), &kernels_buffer[0], &retsz);
                གལ་ཏེ (retsz < parimaa_na(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                དེ་མིན
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(NULL, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        ཕྱིར handle != NULL;
    }

    void getProgramBinary(std::vector<var_na>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARY_SIZES, parimaa_na(sz), &sz, NULL));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, parimaa_na(ptr), &ptr, NULL));
    }

    bool createFromBinary(const Context& ctx, const std::vector<var_na>& buf, String& errmsg)
    {
        ཕྱིར createFromBinary(ctx, (const unsigned var_na*)&buf[0], buf.size(), errmsg);
    }

    bool createFromBinary(const Context& ctx, const unsigned var_na* binaryAddr, const size_t binarySize, String& errmsg)
    {
        CV_Assert(handle == NULL);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(NULL, 0, "Load from binary... (" << binarySize << " bytes)");

        CV_Assert(binarySize > 0);

        size_t ndevices = (ཧྲིལ)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<const uchar*> binaryPtrs_(ndevices);
        AutoBuffer<size_t> binarySizes_(ndevices);

        cl_device_id* devices = devices_.data();
        const uchar** binaryPtrs = binaryPtrs_.data();
        size_t* binarySizes = binarySizes_.data();
        ཆེད་དུ (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            binaryPtrs[i] = binaryAddr;
            binarySizes[i] = binarySize;
        }

        cl_int result = 0;
        handle = clCreateProgramWithBinary((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           binarySizes, binaryPtrs, NULL, &result);
        གལ་ཏེ (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(NULL, CV_OCL_API_ERROR_MSG(result, "clCreateProgramWithBinary"));
            གལ་ཏེ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = NULL;
            }
        }
        གལ་ཏེ (!handle)
        {
            ཕྱིར false;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            གལ་ཏེ (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                གལ་ཏེ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = NULL;
                }
                ཕྱིར false;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    parimaa_na(build_status), &build_status, &retsz));
            གལ་ཏེ (result == CL_SUCCESS)
            {
                གལ་ཏེ (build_status == CL_BUILD_SUCCESS)
                {
                    ཕྱིར true;
                }
                དེ་མིན
                {
                    CV_LOG_WARNING(NULL, "clGetProgramBuildInfo() returns " << build_status);
                    ཕྱིར false;
                }
            }
            དེ་མིན
            {
                CV_LOG_ERROR(NULL, CV_OCL_API_ERROR_MSG(result, "clGetProgramBuildInfo()"));
                གལ་ཏེ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = NULL;
                }
            }
        }
#གལ་ཏེ CV_OPENCL_VALIDATE_BINARY_PROGRAMS
        གལ་ཏེ (handle && CV_OPENCL_VALIDATE_BINARY_PROGRAMS_VALUE)
        {
            CV_LOG_INFO(NULL, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            var_na kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, parimaa_na(kernels_buffer), &kernels_buffer[0], &retsz);
            གལ་ཏེ (retsz < parimaa_na(kernels_buffer))
                kernels_buffer[retsz] = 0;
            དེ་མིན
                kernels_buffer[0] = 0;
            CV_LOG_INFO(NULL, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        ཕྱིར handle != NULL;
    }

    ~Impl()
    {
        གལ་ཏེ( handle )
        {
#ifdef _WIN32
            གལ་ཏེ (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = NULL;
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

Program::Program(const ProgramSource& src,
        const String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(const Program& prog)
{
    p = prog.p;
    གལ་ཏེ(p)
        p->addref();
}

Program& Program::operator = (const Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    གལ་ཏེ(newp)
        newp->addref();
    གལ་ཏེ(p)
        p->release();
    p = newp;
    ཕྱིར *this;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    གལ་ཏེ (this != &prog) {
        གལ་ཏེ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ཕྱིར *this;
}

Program::~Program()
{
    གལ་ཏེ(p)
        p->release();
}

bool Program::create(const ProgramSource& src,
            const String& buildflags, String& errmsg)
{
    གལ་ཏེ(p)
    {
        p->release();
        p = NULL;
    }
    p = new Impl(src, buildflags, errmsg);
    གལ་ཏེ(!p->handle)
    {
        p->release();
        p = 0;
    }
    ཕྱིར p != 0;
}

void* Program::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
const ProgramSource& Program::source() const
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

bool Program::read(const String& bin, const String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

bool Program::write(String& bin) const
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() const
{
    གལ་ཏེ(!p)
        ཕྱིར String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ཕྱིར cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(const String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ཕྱིར cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

void Program::getBinary(std::vector<var_na>& binary) const
{
    CV_Assert(p && "Empty program");
    p->getProgramBinary(binary);
}

Program Context::Impl::getProg(const ProgramSource& src,
                               const String& buildflags, String& errmsg)
{
    size_t limit = getProgramCountLimit();
    const ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String key = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(key);
        གལ་ཏེ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), key);
            གལ་ཏེ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(key);
            }
            ཕྱིར it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            གལ་ཏེ (limit > 0 && sz >= limit)
            {
                static bool warningFlag = false;
                གལ་ཏེ (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = true;
                }
                སྐབས (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    གལ་ཏེ (c != 0)
                        bha_nga;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(key, prog));
        cacheList.push_front(key);
    }
    ཕྱིར prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
class OpenCLBufferPool
{
rak_sita:
    ~OpenCLBufferPool() { }
public:
    virtual T allocate(size_t size) = 0;
    virtual void release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
class OpenCLBufferPoolBaseImpl : public BufferPoolController, public OpenCLBufferPool<T>
{
private:
    anta_hstha Derived& derived() { ཕྱིར *static_cast<Derived*>(this); }
rak_sita:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    bool _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        ཆེད་དུ (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            གལ་ཏེ (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                ཕྱིར true;
            }
        }
        ཕྱིར false;
    }

    // synchronized
    bool _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, const size_t size)
    {
        གལ་ཏེ (reservedEntries_.empty())
            ཕྱིར false;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        ཆེད་དུ (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            གལ་ཏེ (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                གལ་ཏེ (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    གལ་ཏེ (diff == 0)
                        bha_nga;
                }
            }
        }
        གལ་ཏེ (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            ཕྱིར true;
        }
        ཕྱིར false;
    }

    // synchronized
    void _checkSizeOfReservedEntries()
    {
        སྐབས (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            const BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    anta_hstha size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        གལ་ཏེ (size < 1024*1024)
            ཕྱིར 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        དེ་མིན གལ་ཏེ (size < 16*1024*1024)
            ཕྱིར 64*1024;
        དེ་མིན
            ཕྱིར 1024*1024;
    }

public:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    virtual ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
public:
    virtual T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        གལ་ཏེ (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        དེ་མིན
        {
            derived()._allocateBufferEntry(entry, size);
        }
        ཕྱིར entry.clBuffer_;
    }
    virtual void release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        གལ་ཏེ (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        དེ་མིན
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    virtual size_t getReservedSize() const CV_OVERRIDE { ཕྱིར currentReservedSize; }
    virtual size_t getMaxReservedSize() const CV_OVERRIDE { ཕྱིར maxReservedSize; }
    virtual void setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        གལ་ཏེ (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            ཆེད་དུ (; i != reservedEntries_.end();)
            {
                const BufferEntry& entry = *i;
                གལ་ཏེ (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    continue;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    virtual void freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        ཆེད་དུ (; i != reservedEntries_.end(); ++i)
        {
            const BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

sa_nracanaa CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)NULL), capacity_(0) { }
};

class OpenCLBufferPoolImpl CV_FINAL : public OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
public:
    typedef sa_nracanaa CLBufferEntry BufferEntry;
rak_sita:
    ཧྲིལ createFlags_;
public:
    OpenCLBufferPoolImpl(ཧྲིལ createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    void _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == NULL);
        entry.capacity_ = alignSize(size, (ཧྲིལ)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long ཧྲིལ)entry.capacity_, (void*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != NULL);
        གལ་ཏེ(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    void _releaseBufferEntry(const BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != NULL);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
sa_nracanaa CLSVMBufferEntry
{
    void* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(NULL), capacity_(0) { }
};
class OpenCLSVMBufferPoolImpl CV_FINAL : public OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, void*>
{
public:
    typedef sa_nracanaa CLSVMBufferEntry BufferEntry;
public:
    OpenCLSVMBufferPoolImpl()
    {
    }

    void _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == NULL);
        entry.capacity_ = alignSize(size, (ཧྲིལ)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        const svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        bool isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (ཧྲིལ)entry.capacity_);
        void *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    void _releaseBufferEntry(const BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != NULL);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <bool readAccess, bool writeAccess>
class AlignedDataPtr
{
rak_sita:
    const size_t size_;
    uchar* const originPtr_;
    const size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

public:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(NULL)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        གལ་ཏེ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = new uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            གལ་ཏེ (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() const
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ཕྱིར ptr_;
    }

    ~AlignedDataPtr()
    {
        གལ་ཏེ (allocatedPtr_)
        {
            གལ་ཏེ (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = NULL;
        }
        ptr_ = NULL;
    }
private:
    AlignedDataPtr(const AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(const AlignedDataPtr&); // disabled
};

template <bool readAccess, bool writeAccess>
class AlignedDataPtr2D
{
rak_sita:
    const size_t size_;
    uchar* const originPtr_;
    const size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

public:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(NULL), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != NULL);
        གལ་ཏེ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = new uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            གལ་ཏེ (readAccess)
            {
                ཆེད་དུ (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() const
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ཕྱིར ptr_;
    }

    ~AlignedDataPtr2D()
    {
        གལ་ཏེ (allocatedPtr_)
        {
            གལ་ཏེ (writeAccess)
            {
                ཆེད་དུ (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = NULL;
        }
        ptr_ = NULL;
    }
private:
    AlignedDataPtr2D(const AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(const AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


void Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(NULL, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

class OpenCLAllocator CV_FINAL : public MatAllocator
{
public:
    ga_nanaa AllocatorFlags
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

    UMatData* defaultAllocate(ཧྲིལ dims, const ཧྲིལ* sizes, ཧྲིལ type, void* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) const
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        ཕྱིར u;
    }

    static bool isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        static bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", false);
        ཕྱིར value;
    }
    static bool isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        static bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", false);
        ཕྱིར value;
    }

    void getBestFlags(const Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, ཧྲིལ& createFlags, UMatData::MemoryFlag& flags0) const
    {
        const Device& dev = ctx.device(0);
        createFlags = 0;
        གལ་ཏེ ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        གལ་ཏེ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        དེ་མིན
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(ཧྲིལ dims, const ཧྲིལ* sizes, ཧྲིལ type,
                       void* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) const CV_OVERRIDE
    {
        གལ་ཏེ(!useOpenCL())
            ཕྱིར defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        ཆེད་དུ( ཧྲིལ i = dims-1; i >= 0; i-- )
        {
            གལ་ཏེ( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        གལ་ཏེ (!ctx.getImpl())
            ཕྱིར defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        ཧྲིལ createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        void* handle = NULL;
        ཧྲིལ allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        const svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        གལ་ཏེ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            bool isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        དེ་མིན
#endif
        གལ་ཏེ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        དེ་མིན གལ་ཏེ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        དེ་མིན
        {
            CV_Assert(handle != NULL); // Unsupported, throw
        }

        གལ་ཏེ (!handle)
            ཕྱིར defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = new UMatData(this);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<void>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        ཕྱིར u;
    }

    bool allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) const CV_OVERRIDE
    {
        གལ་ཏེ(!u)
            ཕྱིར false;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        གལ་ཏེ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            ཧྲིལ createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            bool copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            ཧྲིལ allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            void* handle = NULL;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            bool useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            གལ་ཏེ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (ཧྲིལ)u->size, handle);
            }
            དེ་མིན གལ་ཏེ (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                གལ་ཏེ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    bool isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (ཧྲིལ)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = NULL;
                    གལ་ཏེ (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (ཧྲིལ)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, NULL, NULL);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    གལ་ཏེ (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, NULL, NULL);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            དེ་མིན
#endif
            {
                གལ་ཏེ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                གལ་ཏེ (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (ཧྲིལ)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long ཧྲིལ)u->size, u->origdata, (void*)handle).c_str());
                }
                གལ་ཏེ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long ཧྲིལ)u->size, u->origdata, (void*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (void*)handle).c_str());
            གལ་ཏེ(!handle || retval != CL_SUCCESS)
                ཕྱིར false;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = this;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        གལ་ཏེ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        ཕྱིར true;
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

    void deallocate(UMatData* u) const CV_OVERRIDE
    {
        གལ་ཏེ(!u)
            ཕྱིར;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        གལ་ཏེ (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        དེ་མིན
            deallocate_(u);
    }

    void deallocate_(UMatData* u) const
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        གལ་ཏེ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        གལ་ཏེ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ཕྱིར;             // avoid any OpenCL calls
#endif
        གལ་ཏེ(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            གལ་ཏེ (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    གལ་ཏེ( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        bool isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = NULL;
                        གལ་ཏེ (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ཧྲིལ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, NULL, NULL);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        གལ་ཏེ (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, NULL, NULL);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    དེ་མིན
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                དེ་མིན
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    གལ་ཏེ( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<false, true> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    དེ་མིན
                    {
                        cl_int retval = 0;
                        གལ་ཏེ (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            void* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (void*)u->handle, (long long ཧྲིལ)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            གལ་ཏེ (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (void*)u->handle, data, (long long ཧྲིལ)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(false);
            }
            དེ་མིན
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                གལ་ཏེ( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            དེ་མིན
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (void*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = NULL;
            གལ་ཏེ(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = NULL;
        }
        དེ་མིན
        {
            CV_Assert(u->origdata == NULL);
            གལ་ཏེ(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(true);
            }
            གལ་ཏེ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            དེ་མིན གལ་ཏེ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            དེ་མིན གལ་ཏེ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                དེ་མིན གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, NULL, NULL);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((void*)u->handle);
            }
#endif
            དེ་མིན
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            delete u;
            u = NULL;
        }
        CV_Assert(u == NULL);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    void map(UMatData* u, AccessFlag accessFlags) const CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        གལ་ཏེ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            གལ་ཏེ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ཧྲིལ)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, NULL, NULL);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(false);
                    u->markDeviceMemMapped(true);
                    ཕྱིར;
                }
#endif

                cl_int retval = CL_SUCCESS;
                གལ་ཏེ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (void*)u->handle, (long long ཧྲིལ)u->size, u->data).c_str());
                }
                གལ་ཏེ (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(false);
                    u->markDeviceMemMapped(true);
                    ཕྱིར;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            གལ་ཏེ(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(true);
            }
        }

        གལ་ཏེ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<false, true> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (void*)u->handle, (long long ཧྲིལ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(false);
        }
    }

    void unmap(UMatData* u) const CV_OVERRIDE
    {
        གལ་ཏེ(!u)
            ཕྱིར;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        གལ་ཏེ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != NULL);
#ifdef HAVE_OPENCL_SVM
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, NULL, NULL);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                གལ་ཏེ (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(false);
                u->markHostCopyObsolete(true);
                ཕྱིར;
            }
#endif
            གལ་ཏེ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (void*)u->handle, u->data, (long long ཧྲིལ)u->size).c_str());
                གལ་ཏེ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(false);
                u->data = 0;
                u->markDeviceCopyObsolete(false);
                u->markHostCopyObsolete(true);
            }
        }
        དེ་མིན གལ་ཏེ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<true, false> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (void*)u->handle, (long long ཧྲིལ)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(false);
            u->markHostCopyObsolete(true);
        }
    }

    bool checkContinuous(ཧྲིལ dims, const size_t sz[],
                         const size_t srcofs[], const size_t srcstep[],
                         const size_t dstofs[], const size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) const
    {
        bool iscontinuous = true;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        ཆེད་དུ( ཧྲིལ i = dims-2; i >= 0; i-- )
        {
            གལ་ཏེ( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = false;
            total *= sz[i];
            གལ་ཏེ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            གལ་ཏེ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        གལ་ཏེ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            གལ་ཏེ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                གལ་ཏེ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                གལ་ཏེ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            དེ་མིན
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                གལ་ཏེ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                གལ་ཏེ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ཕྱིར iscontinuous;
    }

    void download(UMatData* u, void* dstptr, ཧྲིལ dims, const size_t sz[],
                  const size_t srcofs[], const size_t srcstep[],
                  const size_t dststep[]) const CV_OVERRIDE
    {
        གལ་ཏེ(!u)
            ཕྱིར;
        UMatDataAutoLock autolock(u);

        གལ་ཏེ( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ཕྱིར;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == NULL || u->data == u->handle);
            Context& ctx = Context::getDefault();
            const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ཧྲིལ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, NULL, NULL);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            གལ་ཏེ( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            དེ་མིན
            {
                // This code is from MatAllocator::download()
                ཧྲིལ isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                ཆེད་དུ( ཧྲིལ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    གལ་ཏེ( sz[i] == 0 )
                    ཕྱིར;
                    གལ་ཏེ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (ཧྲིལ)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                const Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ཆེད་དུ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, NULL, NULL);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        དེ་མིན
#endif
        {
            གལ་ཏེ( iscontinuous )
            {
                AlignedDataPtr<false, true> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            དེ་མིན གལ་ཏེ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                const size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<false, false> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                ཆེད་དུ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            དེ་མིན
            {
                AlignedDataPtr2D<false, true> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    void upload(UMatData* u, const void* srcptr, ཧྲིལ dims, const size_t sz[],
                const size_t dstofs[], const size_t dststep[],
                const size_t srcstep[]) const CV_OVERRIDE
    {
        གལ་ཏེ(!u)
            ཕྱིར;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        གལ་ཏེ( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(false);
            u->markDeviceCopyObsolete(true);
            ཕྱིར;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == NULL || u->data == u->handle);
            Context& ctx = Context::getDefault();
            const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (ཧྲིལ)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, NULL, NULL);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            གལ་ཏེ( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            དེ་མིན
            {
                // This code is from MatAllocator::upload()
                ཧྲིལ isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                ཆེད་དུ( ཧྲིལ i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    གལ་ཏེ( sz[i] == 0 )
                    ཕྱིར;
                    གལ་ཏེ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (ཧྲིལ)sz[i];
                }

                Mat src(dims, isz, CV_8U, (void*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                const Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                ཆེད་དུ( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, NULL, NULL);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        དེ་མིན
#endif
        {
            གལ་ཏེ( iscontinuous )
            {
                AlignedDataPtr<true, false> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (void*)u->handle, (long long ཧྲིལ)dstrawofs, (long long ཧྲིལ)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            དེ་མིན གལ་ཏེ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                const size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<false, false> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                ཆེད་དུ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            དེ་མིན
            {
                AlignedDataPtr2D<true, false> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(true);
#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        དེ་མིན
#endif
        {
            u->markHostCopyObsolete(true);
        }
        u->markDeviceCopyObsolete(false);
    }

    void copy(UMatData* src, UMatData* dst, ཧྲིལ dims, const size_t sz[],
              const size_t srcofs[], const size_t srcstep[],
              const size_t dstofs[], const size_t dststep[], bool _sync) const CV_OVERRIDE
    {
        གལ་ཏེ(!src || !dst)
            ཕྱིར;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        གལ་ཏེ( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ཕྱིར;
        }
        གལ་ཏེ( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(false);
#ifdef HAVE_OPENCL_SVM
            གལ་ཏེ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            དེ་མིན
#endif
            {
                dst->markDeviceCopyObsolete(true);
            }
            ཕྱིར;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            གལ་ཏེ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                const svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                གལ་ཏེ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (ཧྲིལ)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, NULL, NULL);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                དེ་མིན
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    ཧྲིལ isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    ཆེད་དུ( ཧྲིལ i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        གལ་ཏེ( sz[i] == 0 )
                        ཕྱིར;
                        གལ་ཏེ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (ཧྲིལ)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    ཆེད་དུ( ཧྲིལ i = 0; i < dims; i++ )
                    {
                        གལ་ཏེ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    const Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    ཆེད་དུ( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            དེ་མིན
            {
                གལ་ཏེ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                དེ་མིན
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        དེ་མིན
#endif
        {
            གལ་ཏེ( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (void*)src->handle, (void*)dst->handle, (long long ཧྲིལ)srcrawofs, (long long ཧྲིལ)dstrawofs, (long long ཧྲིལ)total).c_str());
            }
            དེ་མིན གལ་ཏེ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                const size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<false, false> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<false, false> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                ཆེད་དུ( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            དེ་མིན
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        གལ་ཏེ (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        དེ་མིན
#endif
        {
            dst->markHostCopyObsolete(true);
        }
        dst->markDeviceCopyObsolete(false);

        གལ་ཏེ( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(const var_na* id) const CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        གལ་ཏེ (ctx.empty())
            ཕྱིར NULL;
#ifdef HAVE_OPENCL_SVM
        གལ་ཏེ ((svm::checkForceSVMUmatUsage() && (id == NULL || strcmp(id, "OCL") == 0)) || (id != NULL && strcmp(id, "SVM") == 0))
        {
            ཕྱིར &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        གལ་ཏེ (id != NULL && strcmp(id, "HOST_ALLOC") == 0)
        {
            ཕྱིར &ctx.getImpl()->getBufferPoolHostPtr();
        }
        གལ་ཏེ (id != NULL && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ཕྱིར &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    void flushCleanupQueue() const
    {
        // TSAN false positive: see #27638
        གལ་ཏེ (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            ཆེད་དུ (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    void addToCleanupQueue(UMatData* u) const
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

static OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    static OpenCLAllocator* g_allocator = new OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ཕྱིར g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


namespace cv {

// three funcs below are implemented in umatrix.cpp
void setSize( UMat& m, ཧྲིལ _dims, const ཧྲིལ* _sz, const size_t* _steps,
              bool autoSteps = false );
void finalizeHdr(UMat& m);

} // namespace cv


namespace cv { namespace ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
void convertFromBuffer(void* cl_mem_buffer, size_t step, ཧྲིལ rows, ཧྲིལ cols, ཧྲིལ type, UMat& dst)
{
    ཧྲིལ d = 2;
    ཧྲིལ sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, true);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, parimaa_na(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, parimaa_na(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((ཧྲིལ)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = new UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    ཕྱིར;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
void convertFromImage(void* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, parimaa_na(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, parimaa_na(cl_image_format), &fmt, 0));

    ཧྲིལ depth = CV_8U;
    switch (fmt.image_channel_data_type)
    {
    pak_sa CL_UNORM_INT8:
    pak_sa CL_UNSIGNED_INT8:
        depth = CV_8U;
        bha_nga;

    pak_sa CL_SNORM_INT8:
    pak_sa CL_SIGNED_INT8:
        depth = CV_8S;
        bha_nga;

    pak_sa CL_UNORM_INT16:
    pak_sa CL_UNSIGNED_INT16:
        depth = CV_16U;
        bha_nga;

    pak_sa CL_SNORM_INT16:
    pak_sa CL_SIGNED_INT16:
        depth = CV_16S;
        bha_nga;

    pak_sa CL_SIGNED_INT32:
        depth = CV_32S;
        bha_nga;

    pak_sa CL_FLOAT:
        depth = CV_32F;
        bha_nga;

    pak_sa CL_HALF_FLOAT:
        depth = CV_16F;
        bha_nga;

    default:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    ཧྲིལ type = CV_8UC1;
    switch (fmt.image_channel_order)
    {
    pak_sa CL_R:
    pak_sa CL_A:
    pak_sa CL_INTENSITY:
    pak_sa CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        bha_nga;

    pak_sa CL_RG:
    pak_sa CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        bha_nga;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    pak_sa CL_RGBA:
    pak_sa CL_BGRA:
    pak_sa CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        bha_nga;

    default:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        bha_nga;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, parimaa_na(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, parimaa_na(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, parimaa_na(size_t), &h, 0));

    dst.create((ཧྲིལ)h, (ཧྲིལ)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, NULL, NULL));

    CV_OCL_CHECK(clFinish(q));

    ཕྱིར;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

static void getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint numDevices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, NULL, &numDevices);
    གལ་ཏེ (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &numDevices).c_str());
    }

    གལ་ཏེ (numDevices == 0)
    {
        devices.clear();
        ཕྱིར;
    }

    devices.resize((size_t)numDevices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, numDevices, &devices[0], &numDevices));
}

sa_nracanaa PlatformInfo::Impl
{
    Impl(void* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) const
    {
        var_na buf[1024];
        size_t sz=0;
        ཕྱིར clGetPlatformInfo(handle, prop, parimaa_na(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < parimaa_na(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    ཧྲིལ versionMajor_;
    ཧྲིལ versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(void* platform_id)
{
    p = new Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    གལ་ཏེ(p)
        p->release();
}

PlatformInfo::PlatformInfo(const PlatformInfo& i)
{
    གལ་ཏེ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(const PlatformInfo& i)
{
    གལ་ཏེ (i.p != p)
    {
        གལ་ཏེ (i.p)
            i.p->addref();
        གལ་ཏེ (p)
            p->release();
        p = i.p;
    }
    ཕྱིར *this;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    གལ་ཏེ (this != &i) {
        གལ་ཏེ(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ཕྱིར *this;
}

ཧྲིལ PlatformInfo::deviceNumber() const
{
    ཕྱིར p ? (ཧྲིལ)p->devices.size() : 0;
}

void PlatformInfo::getDevice(Device& device, ཧྲིལ d) const
{
    CV_Assert(p && d < (ཧྲིལ)p->devices.size() );
    གལ་ཏེ(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() const
{
    ཕྱིར p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() const
{
    ཕྱིར p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() const
{
    ཕྱིར p ? p->version_ : String();
}

ཧྲིལ PlatformInfo::versionMajor() const
{
    CV_Assert(p);
    ཕྱིར p->versionMajor_;
}

ཧྲིལ PlatformInfo::versionMinor() const
{
    CV_Assert(p);
    ཕྱིར p->versionMinor_;
}

static void getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint numPlatforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, NULL, &numPlatforms));

    གལ་ཏེ (numPlatforms == 0)
    {
        platforms.clear();
        ཕྱིར;
    }

    platforms.resize((size_t)numPlatforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(numPlatforms, &platforms[0], &numPlatforms));
}

void getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    ཆེད་དུ (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((void*)&platforms[i]) );
}

const var_na* typeToStr(ཧྲིལ type)
{
    static const var_na* tab[CV_DEPTH_MAX*16]=
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
    ཧྲིལ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    const var_na* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ཕྱིར result;
}

const var_na* memopTypeToStr(ཧྲིལ type)
{
    static const var_na* tab[CV_DEPTH_MAX*16] =
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
    ཧྲིལ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    const var_na* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ཕྱིར result;
}

const var_na* vecopTypeToStr(ཧྲིལ type)
{
    static const var_na* tab[CV_DEPTH_MAX*16] =
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
    ཧྲིལ cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    const var_na* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    ཕྱིར result;
}

// Deprecated due to size of buf buffer being unknowable.
const var_na* convertTypeStr(ཧྲིལ sdepth, ཧྲིལ ddepth, ཧྲིལ cn, var_na* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ཕྱིར convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

const var_na* convertTypeStr(ཧྲིལ sdepth, ཧྲིལ ddepth, ཧྲིལ cn, var_na* buf, size_t buf_size)
{
    གལ་ཏེ( sdepth == ddepth )
        ཕྱིར "noconvert";
    const var_na *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    གལ་ཏེ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    དེ་མིན གལ་ཏེ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    དེ་མིན
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    ཕྱིར buf;
}

const var_na* getOpenCLErrorString(ཧྲིལ errorCode)
{
#define CV_OCL_CODE(id) pak_sa id: ཕྱིར #id
#define CV_OCL_CODE_(id, name) pak_sa id: ཕྱིར #name
    switch (errorCode)
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
    CV_OCL_CODE(CL_INVALID_VALUE);
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
    CV_OCL_CODE(CL_INVALID_BINARY);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_VALUE);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_COUNT);
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
    default: ཕྱིར "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
static std::string kerToStr(const Mat & k)
{
    ཧྲིལ width = k.cols - 1, depth = k.depth();
    const T * const data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    གལ་ཏེ (depth <= CV_8S)
    {
        ཆེད་དུ (ཧྲིལ i = 0; i < width; ++i)
            stream << "DIG(" << (ཧྲིལ)data[i] << ")";
        stream << "DIG(" << (ཧྲིལ)data[width] << ")";
    }
    དེ་མིན གལ་ཏེ (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        ཆེད་དུ (ཧྲིལ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    དེ་མིན གལ་ཏེ (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        ཆེད་དུ (ཧྲིལ i = 0; i < width; ++i)
            stream << "DIG(" << (float)data[i] << "h)";
        stream << "DIG(" << (float)data[width] << "h)";
    }
    དེ་མིན
    {
        ཆེད་དུ (ཧྲིལ i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    ཕྱིར stream.str();
}

String kernelToStr(InputArray _kernel, ཧྲིལ ddepth, const var_na * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    ཧྲིལ depth = kernel.depth();
    གལ་ཏེ (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    གལ་ཏེ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    གལ་ཏེ (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    typedef std::string (* func_t)(const Mat &);
    static const func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<var_na>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<ཧྲིལ>, kerToStr<float>, kerToStr<dviguu_na>, kerToStr<hfloat>,
                                                0, kerToStr<bool>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    const func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ཕྱིར cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    do \
    { \
        གལ་ཏེ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            ཧྲིལ ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            གལ་ཏེ (cwidth < ckercn || ckercn <= 0) \
                ཕྱིར 1; \
            cols.push_back(cwidth); \
            གལ་ཏེ (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                ཕྱིར 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    སྐབས ((void)0, 0)

ཧྲིལ predictOptimalVectorWidth(InputArray src1, InputArray src2, InputArray src3,
                              InputArray src4, InputArray src5, InputArray src6,
                              InputArray src7, InputArray src8, InputArray src9,
                              OclVectorStrategy strat)
{
    const ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    ཧྲིལ vectorWidths[CV_DEPTH_MAX] = {
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
    གལ་ཏེ (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    ཕྱིར checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

ཧྲིལ checkOptimalVectorWidth([[maybe_unused]] const ཧྲིལ *vectorWidths,
                            [[maybe_unused]] InputArray src1,
                            [[maybe_unused]] InputArray src2,
                            [[maybe_unused]] InputArray src3,
                            [[maybe_unused]] InputArray src4,
                            [[maybe_unused]] InputArray src5,
                            [[maybe_unused]] InputArray src6,
                            [[maybe_unused]] InputArray src7,
                            [[maybe_unused]] InputArray src8,
                            [[maybe_unused]] InputArray src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    ཕྱིར 1;
#དེ་མིན
    CV_Assert(vectorWidths);

    ཧྲིལ ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<ཧྲིལ> dividers, kercns;
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

    ཆེད་དུ (size_t i = 0; i < size; ++i)
        སྐབས (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    ཧྲིལ kercn = *std::min_element(kercns.begin(), kercns.end());

    ཕྱིར kercn;
#endif
}

ཧྲིལ predictOptimalVectorWidthMax(InputArray src1, InputArray src2, InputArray src3,
                                 InputArray src4, InputArray src5, InputArray src6,
                                 InputArray src7, InputArray src8, InputArray src9)
{
    ཕྱིར predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
void buildOptionsAddMatrixDescription(String& buildOptions, const String& name, InputArray _m)
{
    གལ་ཏེ (!buildOptions.empty())
        buildOptions += " ";
    ཧྲིལ type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (ཧྲིལ)CV_MAT_CN(type),
            name.c_str(), (ཧྲིལ)CV_ELEM_SIZE(type),
            name.c_str(), (ཧྲིལ)CV_ELEM_SIZE1(type),
            name.c_str(), (ཧྲིལ)depth
            );
}


sa_nracanaa Image2D::Impl
{
    Impl(const UMat &src, bool norm, bool alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        གལ་ཏེ (handle)
            clReleaseMemObject(handle);
    }

    static cl_image_format getImageFormat(ཧྲིལ depth, ཧྲིལ cn, bool norm)
    {
        cl_image_format format;
        static const ཧྲིལ channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        static const ཧྲིལ channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        static const ཧྲིལ channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        ཧྲིལ channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        ཧྲིལ channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        ཕྱིར format;
    }

    static bool isFormatSupported(cl_image_format format)
    {
        གལ་ཏེ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        གལ་ཏེ (!context)
            ཕྱིར false;

        // Figure out how many formats are supported by this context.
        cl_uint numFormats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, numFormats,
                                                NULL, &numFormats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        གལ་ཏེ (numFormats > 0)
        {
            AutoBuffer<cl_image_format> formats(numFormats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, numFormats,
                                             formats.data(), NULL);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            ཆེད་དུ (cl_uint i = 0; i < numFormats; ++i)
            {
                གལ་ཏེ (!memcmp(&formats[i], &format, parimaa_na(format)))
                {
                    ཕྱིར true;
                }
            }
        }
        ཕྱིར false;
    }

    void init(const UMat &src, bool norm, bool alias)
    {
        གལ་ཏེ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        ཧྲིལ err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        གལ་ཏེ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        གལ་ཏེ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        const Device & d = ocl::Device::getDefault();
        ཧྲིལ minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        གལ་ཏེ (1 < major || (1 == major && 2 <= minor))
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
            desc.num_mip_levels   = 0;
            desc.num_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, NULL, &err);
        }
        དེ་མིན
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, NULL, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        གལ་ཏེ (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), NULL, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long ཧྲིལ)(src.cols * src.rows * src.elemSize()), (void*)devData
                ).c_str());

            const size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, NULL, NULL));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        དེ་མིན
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != NULL);

        གལ་ཏེ (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, NULL, 0));
            གལ་ཏེ (!src.isContinuous())
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
    p = NULL;
}

Image2D::Image2D(const UMat &src, bool norm, bool alias)
{
    p = new Impl(src, norm, alias);
}

bool Image2D::canCreateAlias(const UMat &m)
{
    bool ret = false;
    const Device & d = ocl::Device::getDefault();
    གལ་ཏེ (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        གལ་ཏེ (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            གལ་ཏེ (!m.u->tempUMat())
            {
                ret = true;
            }
        }
    }
    ཕྱིར ret;
}

bool Image2D::isFormatSupported(ཧྲིལ depth, ཧྲིལ cn, bool norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    ཕྱིར Impl::isFormatSupported(format);
}

Image2D::Image2D(const Image2D & i)
{
    p = i.p;
    གལ་ཏེ (p)
        p->addref();
}

Image2D & Image2D::operator = (const Image2D & i)
{
    གལ་ཏེ (i.p != p)
    {
        གལ་ཏེ (i.p)
            i.p->addref();
        གལ་ཏེ (p)
            p->release();
        p = i.p;
    }
    ཕྱིར *this;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    གལ་ཏེ (this != &i) {
        གལ་ཏེ (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ཕྱིར *this;
}

Image2D::~Image2D()
{
    གལ་ཏེ (p)
        p->release();
}

void* Image2D::ptr() const
{
    ཕྱིར p ? p->handle : 0;
}

bool internal::isOpenCLForced()
{
    static bool initialized = false;
    static bool value = false;
    གལ་ཏེ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", false);
        initialized = true;
    }
    ཕྱིར value;
}

bool internal::isPerformanceCheckBypassed()
{
    static bool initialized = false;
    static bool value = false;
    གལ་ཏེ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", false);
        initialized = true;
    }
    ཕྱིར value;
}

bool internal::isCLBuffer(UMat& u)
{
    གལ་ཏེ (isCudaBacked(u.u))
        ཕྱིར false;
    void* h = u.handle(ACCESS_RW);
    གལ་ཏེ (!h)
        ཕྱིར true;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#གལ་ཏེ 1
    གལ་ཏེ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ཕྱིར false;
#དེ་མིན
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, parimaa_na(type), &type, NULL);
    གལ་ཏེ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        ཕྱིར false;
#endif
    ཕྱིར true;
}

sa_nracanaa Timer::Impl
{
    const Queue queue;

    Impl(const Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    void start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    void stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() const
    {
        ཕྱིར (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(const Queue& q) : p(new Impl(q)) { }
Timer::~Timer() { delete p; }

void Timer::start()
{
    CV_Assert(p);
    p->start();
}

void Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() const
{
    CV_Assert(p);
    ཕྱིར p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
