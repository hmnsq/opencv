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

#અન્તર્ભાવ "precomp.hpp"

#ifndef HAVE_OPENCL
#અન્તર્ભાવ "ocl_disabled.impl.hpp"
#નહીંતર // HAVE_OPENCL

#અન્તર્ભાવ <list>
#અન્તર્ભાવ <map>
#અન્તર્ભાવ <deque>
#અન્તર્ભાવ <set>
#અન્તર્ભાવ <string>
#અન્તર્ભાવ <sstream>
#અન્તર્ભાવ <fstream>
#જો !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#અન્તર્ભાવ <inttypes.h>
#endif

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#અન્તર્ભાવ "opencv2/core/ocl_genbase.hpp"
#અન્તર્ભાવ "opencl_kernels_core.hpp"

#અન્તર્ભાવ "opencv2/core/utils/lock.private.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.hpp"
#અન્તર્ભાવ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ખાલી, 0, "OpenCL allocator: " << __VA_ARGS__)
#અન્તર્ભાવ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#અન્તર્ભાવ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# જો 0
#   define LOG_BUFFER_POOL printf
# નહીંતર
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#જો CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#નહીંતર
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#જો CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#નહીંતર
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#અન્તર્ભાવ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#અન્તર્ભાવ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#અન્તર્ભાવ "umatrix.hpp"

નામાકાશ cv { નામાકાશ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    શૂન્ય addref() { CV_XADD(&refcount, 1); } \
    શૂન્ય release() { જો( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete આ; } \
    પૂર્ણાંક refcount

સ્થિર cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    પાછા opencl_allocator_stats;
}

#ifndef _DEBUG
સ્થિર તાર્કિક isRaiseError()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક value = ખોટું;
    જો (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", ખોટું);
        initialized = સાચું;
    }
    પાછા value;
}
#endif

સ્થિર શૂન્ય onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    તાર્કિક value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", ખોટું);
    જો (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#જો CV_OPENCL_TRACE_CHECK
સ્થિર અંતઃરેખ
શૂન્ય traceOpenCLCheck(cl_int status, અચલ અક્ષર* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#નહીંતર
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_સંદેશ(check_result, સંદેશ) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, સંદેશ)

#define CV_OCL_CHECK_RESULT(check_result, સંદેશ) \
    કરો { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, સંદેશ); \
        જો (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(સંદેશ), અચલ અક્ષર*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_સંદેશ = CV_OCL_API_ERROR_સંદેશ(check_result, સંદેશ); \
            CV_Error(Error::OpenCLApiCallError, error_સંદેશ); \
        } \
    } જ્યારે (0)

#define CV_OCL_CHECK_(પદાવલિ, check_result) કરો { પદાવલિ; CV_OCL_CHECK_RESULT(check_result, #expr); } જ્યારે (0)

#define CV_OCL_CHECK(પદાવલિ) કરો { cl_int __cl_result = (પદાવલિ); CV_OCL_CHECK_RESULT(__cl_result, #expr); } જ્યારે (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, સંદેશ) CV_OCL_CHECK_RESULT(check_result, સંદેશ)
#define CV_OCL_DBG_CHECK(પદાવલિ) CV_OCL_CHECK(પદાવલિ)
#define CV_OCL_DBG_CHECK_(પદાવલિ, check_result) CV_OCL_CHECK_(પદાવલિ, check_result)
#નહીંતર
#define CV_OCL_DBG_CHECK_RESULT(check_result, સંદેશ) \
    કરો { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, સંદેશ); \
        જો (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(સંદેશ), અચલ અક્ષર*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_સંદેશ = CV_OCL_API_ERROR_સંદેશ(check_result, સંદેશ); \
            CV_Error(Error::OpenCLApiCallError, error_સંદેશ); \
        } \
    } જ્યારે (0)
#define CV_OCL_DBG_CHECK_(પદાવલિ, check_result) કરો { પદાવલિ; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } જ્યારે (0)
#define CV_OCL_DBG_CHECK(પદાવલિ) કરો { cl_int __cl_result = (પદાવલિ); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } જ્યારે (0)
#endif


સ્થિર અચલ તાર્કિક CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", સાચું);
સ્થિર અચલ તાર્કિક CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", સાચું);
સ્થિર અચલ તાર્કિક CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", સાચું);
સ્થિર અચલ તાર્કિક CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", સાચું);

#જો CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS
સ્થિર અચલ તાર્કિક CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS_કિંમત = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", ખોટું);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
સ્થિર અચલ તાર્કિક CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#જો 1 //def __APPLE__
        સાચું
#નહીંતર
        ખોટું
#endif
);

સ્થિર String getBuildExtraOptions()
{
    સ્થિર String param_buildExtraOptions;
    સ્થિર તાર્કિક initialized = ખોટું;
    જો (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = સાચું;
        જો (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ખાલી, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    પાછા param_buildExtraOptions;
}

સ્થિર અચલ તાર્કિક CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", સાચું);
સ્થિર અચલ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


સંરચના UMat2D
{
    UMat2D(અચલ UMat& m)
    {
        offset = (પૂર્ણાંક)m.offset;
        step = (પૂર્ણાંક)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    પૂર્ણાંક offset;
    પૂર્ણાંક step;
    પૂર્ણાંક rows;
    પૂર્ણાંક cols;
};

સંરચના UMat3D
{
    UMat3D(અચલ UMat& m)
    {
        offset = (પૂર્ણાંક)m.offset;
        step = (પૂર્ણાંક)m.step.p[1];
        slicestep = (પૂર્ણાંક)m.step.p[0];
        slices = (પૂર્ણાંક)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    પૂર્ણાંક offset;
    પૂર્ણાંક slicestep;
    પૂર્ણાંક step;
    પૂર્ણાંક slices;
    પૂર્ણાંક rows;
    પૂર્ણાંક cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
સ્થિર uint64 crc64( અચલ uchar* data, size_t size, uint64 crc0=0 )
{
    સ્થિર uint64 table[256];
    સ્થિર તાર્કિક initialized = ખોટું;

    જો( !initialized )
    {
        માટે( પૂર્ણાંક i = 0; i < 256; i++ )
        {
            uint64 c = i;
            માટે( પૂર્ણાંક j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = સાચું;
    }

    uint64 crc = ~crc0;
    માટે( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    પાછા ~crc;
}

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
સંરચના OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    પ્રકારવ્યાખ્યા std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ખાલી, "Initializing OpenCL cache configuration...");
        જો (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ખાલી, "OpenCL cache is disabled");
            પાછા;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        જો (cache_path_.empty())
        {
            CV_LOG_INFO(ખાલી, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        કરો
        {
            પ્રયાસ
            {
                જો (cache_path_.empty())
                    તોડો;
                જો (cache_path_ == "disabled")
                    તોડો;
                જો (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(ખાલી, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    તોડો;
                }

                જો (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    જો (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(ખાલી, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        જો (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(ખાલી, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            તોડો;
                        }
                    }

                    પ્રયાસ
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(ખાલી, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(ખાલી, 0, "Checking cache lock... Done!");
                    }
                    પકડો (અચલ cv::Exception& e)
                    {
                        CV_LOG_WARNING(ખાલી, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    પકડો (...)
                    {
                        CV_LOG_WARNING(ખાલી, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                નહીંતર
                {
                    જો (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ખાલી, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    નહીંતર
                    {
                        CV_LOG_INFO(ખાલી, "OpenCL cache lock is disabled");
                    }
                }
            }
            પકડો (અચલ cv::Exception& e)
            {
                CV_LOG_WARNING(ખાલી, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } જ્યારે (0);
        જો (!cache_path_.empty())
        {
            જો (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(ખાલી, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            નહીંતર
            {
                CV_LOG_INFO(ખાલી, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    શૂન્ય clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(અચલ std::string& ctx_prefix,
            અચલ std::string& cleanup_prefix)
    {
        જો (cache_path_.empty())
            પાછા std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        જો (found_it != prepared_contexts_.end())
            પાછા found_it->second;

        CV_LOG_INFO(ખાલી, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        તાર્કિક result = utils::fs::isDirectory(target_directory);
        જો (!result)
        {
            પ્રયાસ
            {
                CV_LOG_VERBOSE(ખાલી, 0, "Creating directory: " << target_directory);
                જો (utils::fs::createDirectories(target_directory))
                {
                    result = સાચું;
                }
                નહીંતર
                {
                    CV_LOG_WARNING(ખાલી, "Can't create directory: " << target_directory);
                }
            }
            પકડો (અચલ cv::Exception& e)
            {
                CV_LOG_ERROR(ખાલી, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        જો (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            પ્રયાસ
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, ખોટું, સાચું);
                std::vector<String> remove_entries;
                માટે (size_t i = 0; i < entries.size(); i++)
                {
                    અચલ String& name = entries[i];
                    જો (0 == name.find(cleanup_prefix))
                    {
                        જો (0 == name.find(ctx_prefix))
                            ચાલુ; // skip current
                        remove_entries.push_back(name);
                    }
                }
                જો (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ખાલી, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ખાલી, "Trying to remove these directories...");
                    માટે (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ખાલી, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ખાલી, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    માટે (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        અચલ String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        પ્રયાસ
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(ખાલી, "Removed: " << path);
                        }
                        પકડો (અચલ cv::Exception& e)
                        {
                            CV_LOG_ERROR(ખાલી, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            પકડો (...)
            {
                CV_LOG_WARNING(ખાલી, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ખાલી, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        પાછા target_directory;
    }

    સ્થિર OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, નવું OpenCLBinaryCacheConfigurator());
    }
};
વર્ગ દ્વિઅંકી_Program_File
{
    ગણના { MAX_ENTRIES = 64 };

    પ્રકારવ્યાખ્યા unsigned પૂર્ણાંક uint32_t;

    સંરચના CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    સંરચના CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t સંખ્યા_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    સંરચના CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t ચાવી_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    અચલ std::string fileName_;
    અચલ અક્ષર* અચલ sourceSignature_;
    અચલ size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(અચલ std::string& options)
    {
        uint64 hash = crc64((અચલ uchar*)options.c_str(), options.size(), 0);
        પાછા hash & (MAX_ENTRIES - 1);
    }

    અંતઃરેખ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        પાછા fileSize;
    }
    અંતઃરેખ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.પઠ((અક્ષર*)&res, કદ(uint32_t));
        CV_Assert(!f.fail());
        પાછા res;
    }
    અંતઃરેખ શૂન્ય writeUInt32(અચલ uint32_t value)
    {
        uint32_t v = value;
        f.લિખ((અક્ષર*)&v, કદ(uint32_t));
        CV_Assert(!f.fail());
    }

    અંતઃરેખ શૂન્ય seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    અંતઃરેખ શૂન્ય seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    અંતઃરેખ શૂન્ય seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    શૂન્ય clearFile()
    {
        f.close();
        જો (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ખાલી, "Can't remove: " << fileName_);
        પાછા;
    }

જાહેર:
    દ્વિઅંકી_Program_File(અચલ std::string& fileName, અચલ અક્ષર* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(કદ(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != ખાલી);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, કદ(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        જો(f.is_open() && getFileSize() > 0)
        {
            તાર્કિક isValid = ખોટું;
            પ્રયાસ
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                જો (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<અક્ષર> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.પઠ(fileSourceSignature.data(), fileSourceSignatureSize);
                    જો (f.eof())
                    {
                        CV_LOG_ERROR(ખાલી, "Unexpected EOF");
                    }
                    નહીંતર જો (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = સાચું;
                    }
                }
                જો (!isValid)
                {
                    CV_LOG_ERROR(ખાલી, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            પકડો (અચલ cv::Exception& e)
            {
                CV_LOG_ERROR(ખાલી, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            પકડો (...)
            {
                CV_LOG_ERROR(ખાલી, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            જો (!isValid)
            {
                clearFile();
            }
            નહીંતર
            {
                seekReadAbsolute(0);
            }
        }
    }

    તાર્કિક પઠ(અચલ std::string& ચાવી, std::vector<અક્ષર>& buf)
    {
        જો (!f.is_open())
            પાછા ખોટું;

        size_t fileSize = getFileSize();
        જો (fileSize == 0)
        {
            CV_LOG_ERROR(ખાલી, "Invalid file (empty): " << fileName_);
            clearFile();
            પાછા ખોટું;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t સંખ્યા_Of_Entries = readUInt32();
        CV_Assert(સંખ્યા_Of_Entries > 0);
        જો (સંખ્યા_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ખાલી, "Invalid file: " << fileName_);
            clearFile();
            પાછા ખોટું;
        }
        f.પઠ((અક્ષર*)&entryOffsets[0], કદ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_અંક = getHash(ચાવી);

        uint32_t entryOffset = entryOffsets[entry_અંક];
        FileEntry entry;
        જ્યારે (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.પઠ((અક્ષર*)&entry, કદ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<અક્ષર> file_ચાવી(entry.ચાવી_Size + 1);
            જો (ચાવી.size() == entry.ચાવી_Size)
            {
                જો (entry.ચાવી_Size > 0)
                {
                    f.પઠ(file_ચાવી.data(), entry.ચાવી_Size);
                    CV_Assert(!f.fail());
                }
                જો (memcmp(file_ચાવી.data(), ચાવી.c_str(), entry.ચાવી_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.પઠ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ખાલી, 0, "Read...");
                    પાછા સાચું;
                }
            }
            જો (entry.nextEntryFileOffset == 0)
                તોડો;
            entryOffset = entry.nextEntryFileOffset;
        }
        પાછા ખોટું;
    }

    તાર્કિક લિખ(અચલ std::string& ચાવી, std::vector<અક્ષર>& buf)
    {
        જો (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            જો (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                જો (!f.is_open())
                {
                    CV_LOG_ERROR(ખાલી, "Can't create file: " << fileName_);
                    પાછા ખોટું;
                }
            }
        }

        size_t fileSize = getFileSize();
        જો (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.લિખ(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, કદ(entryOffsets));
            f.લિખ((અક્ષર*)entryOffsets, કદ(entryOffsets));
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

        uint32_t સંખ્યા_Of_Entries = readUInt32();
        CV_Assert(સંખ્યા_Of_Entries > 0);
        જો (સંખ્યા_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ખાલી, "Invalid file: " << fileName_);
            clearFile();
            પાછા ખોટું;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.પઠ((અક્ષર*)&entryOffsets[0], કદ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_અંક = getHash(ચાવી);

        uint32_t entryOffset = entryOffsets[entry_અંક];
        FileEntry entry;
        જ્યારે (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.પઠ((અક્ષર*)&entry, કદ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<અક્ષર> file_ચાવી(entry.ચાવી_Size + 1);
            જો (ચાવી.size() == entry.ચાવી_Size)
            {
                જો (entry.ચાવી_Size > 0)
                {
                    f.પઠ(file_ચાવી.data(), entry.ચાવી_Size);
                    CV_Assert(!f.fail());
                }
                જો (0 == memcmp(file_ચાવી.data(), ચાવી.c_str(), entry.ચાવી_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ખાલી, 0, "Duplicate key ignored: " << fileName_);
                    પાછા ખોટું;
                }
            }
            જો (entry.nextEntryFileOffset == 0)
                તોડો;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        જો (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.લિખ((અક્ષર*)&entry, કદ(entry));
            CV_Assert(!f.fail());
        }
        નહીંતર
        {
            entryOffsets[entry_અંક] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.લિખ((અક્ષર*)entryOffsets, કદ(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.ચાવી_Size = (uint32_t)ચાવી.size();
        f.લિખ((અક્ષર*)&entry, કદ(entry));
        CV_Assert(!f.fail());
        f.લિખ(ચાવી.c_str(), entry.ચાવી_Size);
        CV_Assert(!f.fail());
        f.લિખ(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ખાલી, 0, "Write... (" << buf.size() << " bytes)");
        પાછા સાચું;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



સંરચના OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    પૂર્ણાંક device_;  // device index in context
    ocl::Queue queue_;
    પૂર્ણાંક useOpenCL_;

સુરક્ષિત:
    Impl() = delete;

    શૂન્ય _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        પૂર્ણાંક ndevices = (પૂર્ણાંક)context_.ndevices();
        CV_Assert(ndevices > 0);
        તાર્કિક found = ખોટું;
        માટે (પૂર્ણાંક i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            જો (dhandle == deviceID)
            {
                device_ = i;
                found = સાચું;
                તોડો;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    શૂન્ય _init_device(અચલ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        પૂર્ણાંક ndevices = (પૂર્ણાંક)context_.ndevices();
        CV_Assert(ndevices > 0);
        તાર્કિક found = ખોટું;
        માટે (પૂર્ણાંક i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            જો (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = સાચું;
                તોડો;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

જાહેર:
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

    Impl(અચલ ocl::Context& context, અચલ ocl::Device& device, અચલ ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(અચલ ocl::Context& context, અચલ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(અચલ ocl::Context& context, અચલ પૂર્ણાંક device, અચલ ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(અચલ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    અંતઃરેખ તાર્કિક useOpenCL() અચલ { પાછા const_cast<Impl*>(આ)->useOpenCL(); }
    તાર્કિક useOpenCL()
    {
        જો (useOpenCL_ < 0)
        {
            પ્રયાસ
            {
                useOpenCL_ = 0;
                જો (!context_.empty() && context_.ndevices() > 0)
                {
                    અચલ Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            પકડો (અચલ cv::Exception&)
            {
                // nothing
            }
            જો (!useOpenCL_)
                CV_LOG_INFO(ખાલી, "OpenCL: can't use OpenCL execution context");
        }
        પાછા useOpenCL_ > 0;
    }

    શૂન્ય setUseOpenCL(તાર્કિક flag)
    {
        જો (!flag)
            useOpenCL_ = 0;
        નહીંતર
            useOpenCL_ = -1;
    }

    સ્થિર અચલ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ખાલી, "OpenCL: initializing thread execution context");

        સ્થિર તાર્કિક initialized = ખોટું;
        સ્થિર std::shared_ptr<Impl> g_primaryExecutionContext;

        જો (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            જો (!initialized)
            {
                CV_LOG_INFO(ખાલી, "OpenCL: creating new execution context...");
                પ્રયાસ
                {
                    Context c = ocl::Context::create(std::string());
                    જો (c.ndevices())
                    {
                        પૂર્ણાંક deviceId = 0;
                        auto& d = c.device(deviceId);
                        જો (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            જો (!q.ptr())
                            {
                                CV_LOG_ERROR(ખાલી, "OpenCL: Can't create default OpenCL queue");
                            }
                            નહીંતર
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ખાલી, "OpenCL: device=" << d.name());
                            }
                        }
                        નહીંતર
                        {
                            CV_LOG_ERROR(ખાલી, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    નહીંતર
                    {
                        CV_LOG_INFO(ખાલી, "OpenCL: context is not available/disabled");
                    }
                }
                પકડો (અચલ std::exception& e)
                {
                    CV_LOG_INFO(ખાલી, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                પકડો (...)
                {
                    CV_LOG_WARNING(ખાલી, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = સાચું;
            }
        }
        પાછા g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() અચલ
{
    CV_Assert(p);
    પાછા p->context_;
}
Device& OpenCLExecutionContext::getDevice() અચલ
{
    CV_Assert(p);
    પાછા p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() અચલ
{
    CV_Assert(p);
    પાછા p->queue_;
}

તાર્કિક OpenCLExecutionContext::useOpenCL() અચલ
{
    જો (p)
        પાછા p->useOpenCL();
    પાછા ખોટું;
}
શૂન્ય OpenCLExecutionContext::setUseOpenCL(તાર્કિક flag)
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
    જો (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = સાચું;
        જો (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    પાછા c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    પાછા c;
}

શૂન્ય OpenCLExecutionContext::bind() અચલ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *આ;
    data.oclExecutionContextInitialized = સાચું;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() અચલ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    અચલ Queue q(getContext(), getDevice());
    પાછા cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(અચલ ocl::Queue& q) અચલ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ખાલી);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    પાછા c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(અચલ Context& context, અચલ Device& device, અચલ ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    જો (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    પાછા ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(અચલ Context& context, અચલ Device& device)
{
    CV_TRACE_FUNCTION();
    જો (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    પાછા ctx;

}

શૂન્ય OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
સ્થિર તાર્કિક g_isOpenCLInitialized = ખોટું;
સ્થિર તાર્કિક g_isOpenCLAvailable = ખોટું;

તાર્કિક haveOpenCL()
{
    CV_TRACE_FUNCTION();

    જો (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        જો (!envPath.empty())
        {
            જો (envPath == "disabled")
            {
                g_isOpenCLAvailable = ખોટું;
                g_isOpenCLInitialized = સાચું;
                પાછા ખોટું;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(ખાલી, "Initialize OpenCL runtime...");
        પ્રયાસ
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ખાલી, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ખાલી, "OpenCL: found " << n << " platforms");
        }
        પકડો (...)
        {
            g_isOpenCLAvailable = ખોટું;
        }
        g_isOpenCLInitialized = સાચું;
    }
    પાછા g_isOpenCLAvailable;
}

તાર્કિક useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    જો (data.useOpenCL < 0)
    {
        પ્રયાસ
        {
            data.useOpenCL = 0;
            જો (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        પકડો (...)
        {
            CV_LOG_INFO(ખાલી, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    પાછા data.useOpenCL > 0;
}

તાર્કિક isOpenCLActivated()
{
    જો (!g_isOpenCLAvailable)
        પાછા ખોટું; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    પાછા useOpenCL();
}

શૂન્ય setUseOpenCL(તાર્કિક flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    જો (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    નહીંતર
    {
        જો (!flag)
            data.useOpenCL = 0;
        નહીંતર
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

વર્ગ AmdBlasHelper
{
જાહેર:
    સ્થિર AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, નવું AmdBlasHelper())
    }

    તાર્કિક isAvailable() અચલ
    {
        પાછા g_isAmdBlasAvailable;
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

સુરક્ષિત:
    AmdBlasHelper()
    {
        જો (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            જો (!g_isAmdBlasInitialized)
            {
                જો (haveOpenCL())
                {
                    પ્રયાસ
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    પકડો (...)
                    {
                        g_isAmdBlasAvailable = ખોટું;
                    }
                }
                નહીંતર
                    g_isAmdBlasAvailable = ખોટું;

                g_isAmdBlasInitialized = સાચું;
            }
        }
    }

ખાનગી:
    સ્થિર તાર્કિક g_isAmdBlasInitialized;
    સ્થિર તાર્કિક g_isAmdBlasAvailable;
};

તાર્કિક AmdBlasHelper::g_isAmdBlasAvailable = ખોટું;
તાર્કિક AmdBlasHelper::g_isAmdBlasInitialized = ખોટું;

તાર્કિક haveAmdBlas()
{
    પાછા AmdBlasHelper::getInstance().isAvailable();
}

#નહીંતર

તાર્કિક haveAmdBlas()
{
    પાછા ખોટું;
}

#endif

#ifdef HAVE_CLAMDFFT

વર્ગ AmdFftHelper
{
જાહેર:
    સ્થિર AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, નવું AmdFftHelper())
    }

    તાર્કિક isAvailable() અચલ
    {
        પાછા g_isAmdFftAvailable;
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

સુરક્ષિત:
    AmdFftHelper()
    {
        જો (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            જો (!g_isAmdFftInitialized)
            {
                જો (haveOpenCL())
                {
                    પ્રયાસ
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = સાચું;
                    }
                    પકડો (અચલ Exception &)
                    {
                        g_isAmdFftAvailable = ખોટું;
                    }
                }
                નહીંતર
                    g_isAmdFftAvailable = ખોટું;

                g_isAmdFftInitialized = સાચું;
            }
        }
    }

ખાનગી:
    સ્થિર clfftSetupData setupData;
    સ્થિર તાર્કિક g_isAmdFftInitialized;
    સ્થિર તાર્કિક g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
તાર્કિક AmdFftHelper::g_isAmdFftAvailable = ખોટું;
તાર્કિક AmdFftHelper::g_isAmdFftInitialized = ખોટું;

તાર્કિક haveAmdFft()
{
    પાછા AmdFftHelper::getInstance().isAvailable();
}

#નહીંતર

તાર્કિક haveAmdFft()
{
    પાછા ખોટું;
}

#endif

તાર્કિક haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    પાછા સાચું;
#નહીંતર
    પાછા ખોટું;
#endif
}

શૂન્ય finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

સંરચના Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = ખોટું;
    }

    ~Impl() {}

    શૂન્ય init()
    {
        જો( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            જો( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            જો( handle != 0 )
            {
                અક્ષર buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, કદ(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = સાચું;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    તાર્કિક initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    જો(p)
        p->release();
}

Platform::Platform(અચલ Platform& pl)
{
    p = (Impl*)pl.p;
    જો(p)
        p->addref();
}

Platform& Platform::operator = (અચલ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    જો (આ != &pl) {
        જો(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    પાછા *આ;
}

શૂન્ય* Platform::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ખાલી, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    સ્થિર Platform p;
    જો( !p.p )
    {
        p.p = નવું Impl;
        p.p->init();
    }
    પાછા p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
સ્થિર શૂન્ય parseOpenCLVersion(અચલ String &version, પૂર્ણાંક &major, પૂર્ણાંક &minor)
{
    major = minor = 0;
    જો (10 >= version.length())
        પાછા;
    અચલ અક્ષર *pstr = version.c_str();
    જો (0 != strncmp(pstr, "OpenCL ", 7))
        પાછા;
    size_t ppos = version.find('.', 7);
    જો (String::npos == ppos)
        પાછા;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

સંરચના Device::Impl
{
    Impl(શૂન્ય* d)
        : refcount(1)
        , handle(0)
    {
        પ્રયાસ
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        પકડો (...)
        {
            ફેંકો;
        }
    }

    શૂન્ય _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, પૂર્ણાંક>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, પૂર્ણાંક>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, પૂર્ણાંક>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        જ્યારે (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            જો (pos2 == String::npos)
                pos2 = extensions_.size();
            જો (pos2 > pos)
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
        જો (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        નહીંતર જો (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        નહીંતર જો (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        નહીંતર
            vendorID_ = UNKNOWN_VENDOR;

        અચલ size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        જો (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            અચલ size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            જો (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(ખાલી, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#જો 0
        જો (isExtensionSupported("cl_khr_spir"))
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
        જો (!cv::__termination)
#endif
        {
            જો (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) અચલ
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        પાછા clGetDeviceInfo(handle, prop, કદ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == કદ(temp) ? _TpOut(temp) : _TpOut();
    }

    તાર્કિક getBoolProp(cl_device_info prop) અચલ
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        પાછા clGetDeviceInfo(handle, prop, કદ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == કદ(temp) ? temp != 0 : ખોટું;
    }

    String getStrProp(cl_device_info prop) અચલ
    {
        અક્ષર buf[4096];
        size_t sz=0;
        પાછા clGetDeviceInfo(handle, prop, કદ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < કદ(buf) ? String(buf) : String();
    }

    તાર્કિક isExtensionSupported(અચલ std::string& extensionName) અચલ
    {
        પાછા extensions_set_.ગણતરી_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    પૂર્ણાંક doubleFPConfig_;
    તાર્કિક khr_fp64_support_;
    પૂર્ણાંક halfFPConfig_;
    તાર્કિક khr_fp16_support_;
    તાર્કિક hostUnifiedMemory_;
    પૂર્ણાંક maxComputeUnits_;
    size_t maxWorkGroupSize_;
    પૂર્ણાંક type_;
    પૂર્ણાંક addressBits_;
    પૂર્ણાંક deviceVersionMajor_;
    પૂર્ણાંક deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    પૂર્ણાંક vendorID_;
    તાર્કિક intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(શૂન્ય* d)
{
    p = 0;
    set(d);
}

Device::Device(અચલ Device& d)
{
    p = d.p;
    જો(p)
        p->addref();
}

Device& Device::operator = (અચલ Device& d)
{
    Impl* newp = (Impl*)d.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    જો (આ != &d) {
        જો(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    પાછા *આ;
}

Device::~Device()
{
    જો(p)
        p->release();
}

શૂન્ય Device::set(શૂન્ય* d)
{
    જો(p)
        p->release();
    p = નવું Impl(d);
    જો (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(શૂન્ય* d)
{
    Device device(d);
    પાછા device;
}

શૂન્ય* Device::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

String Device::name() અચલ
{ પાછા p ? p->name_ : String(); }

String Device::extensions() અચલ
{ પાછા p ? String(p->extensions_) : String(); }

તાર્કિક Device::isExtensionSupported(અચલ String& extensionName) અચલ
{ પાછા p ? p->isExtensionSupported(extensionName) : ખોટું; }

String Device::version() અચલ
{ પાછા p ? p->version_ : String(); }

String Device::vendorName() અચલ
{ પાછા p ? p->vendorName_ : String(); }

પૂર્ણાંક Device::vendorID() અચલ
{ પાછા p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() અચલ
{ પાછા p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() અચલ
{ પાછા p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

પૂર્ણાંક Device::deviceVersionMajor() અચલ
{ પાછા p ? p->deviceVersionMajor_ : 0; }

પૂર્ણાંક Device::deviceVersionMinor() અચલ
{ પાછા p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() અચલ
{ પાછા p ? p->driverVersion_ : String(); }

પૂર્ણાંક Device::type() અચલ
{ પાછા p ? p->type_ : 0; }

પૂર્ણાંક Device::addressBits() અચલ
{ પાછા p ? p->addressBits_ : 0; }

તાર્કિક Device::available() અચલ
{ પાછા p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : ખોટું; }

તાર્કિક Device::compilerAvailable() અચલ
{ પાછા p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : ખોટું; }

તાર્કિક Device::linkerAvailable() અચલ
#ifdef CL_VERSION_1_2
{ પાછા p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : ખોટું; }
#નહીંતર
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

પૂર્ણાંક Device::doubleFPConfig() અચલ
{ પાછા p ? p->doubleFPConfig_ : 0; }

પૂર્ણાંક Device::singleFPConfig() અચલ
{ પાછા p ? p->getProp<cl_device_fp_config, પૂર્ણાંક>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

પૂર્ણાંક Device::halfFPConfig() અચલ
{ પાછા p ? p->halfFPConfig_ : 0; }

તાર્કિક Device::hasFP64() અચલ
{ પાછા p ? p->khr_fp64_support_ : ખોટું; }
તાર્કિક Device::hasFP16() અચલ
{ પાછા p ? p->khr_fp16_support_ : ખોટું; }

તાર્કિક Device::endianLittle() અચલ
{ પાછા p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : ખોટું; }

તાર્કિક Device::errorCorrectionSupport() અચલ
{ પાછા p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : ખોટું; }

પૂર્ણાંક Device::executionCapabilities() અચલ
{ પાછા p ? p->getProp<cl_device_exec_capabilities, પૂર્ણાંક>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

પૂર્ણાંક Device::globalMemCacheType() અચલ
{ પાછા p ? p->getProp<cl_device_mem_cache_type, પૂર્ણાંક>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

પૂર્ણાંક Device::global_Mem_Cache_લીટી_Size() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

પૂર્ણાંક Device::localMemType() અચલ
{ પાછા p ? p->getProp<cl_device_local_mem_type, પૂર્ણાંક>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

તાર્કિક Device::hostUnifiedMemory() અચલ
{ પાછા p ? p->hostUnifiedMemory_ : ખોટું; }

તાર્કિક Device::imageSupport() અચલ
{ પાછા p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : ખોટું; }

તાર્કિક Device::imageFromBufferSupport() અચલ
{
    પાછા p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : ખોટું;
}

uint Device::imagePitchAlignment() અચલ
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    પાછા p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#નહીંતર
    પાછા 0;
#endif
}

uint Device::imageBaseAddressAlignment() અચલ
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    પાછા p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#નહીંતર
    પાછા 0;
#endif
}

size_t Device::image2DMaxWidth() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() અચલ
#ifdef CL_VERSION_1_2
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#નહીંતર
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() અચલ
#ifdef CL_VERSION_1_2
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#નહીંતર
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

તાર્કિક Device::intelSubgroupsSupport() અચલ
{ પાછા p ? p->intelSubgroupsSupport_ : ખોટું; }

પૂર્ણાંક Device::maxClockFrequency() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

પૂર્ણાંક Device::maxComputeUnits() અચલ
{ પાછા p ? p->maxComputeUnits_ : 0; }

પૂર્ણાંક Device::maxConstantArgs() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() અચલ
{ પાછા p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

પૂર્ણાંક Device::maxReadImageArgs() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

પૂર્ણાંક Device::maxWriteImageArgs() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

પૂર્ણાંક Device::maxSamplers() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() અચલ
{ પાછા p ? p->maxWorkGroupSize_ : 0; }

પૂર્ણાંક Device::maxWorkItemDims() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

શૂન્ય Device::maxWorkItemSizes(size_t* sizes) અચલ
{
    જો(p)
    {
        અચલ પૂર્ણાંક MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*કદ(sizes[0]), &sizes[0], &retsz));
    }
}

પૂર્ણાંક Device::memBaseAddrAlign() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

પૂર્ણાંક Device::nativeVectorWidthChar() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

પૂર્ણાંક Device::nativeVectorWidthShort() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

પૂર્ણાંક Device::nativeVectorWidthInt() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

પૂર્ણાંક Device::nativeVectorWidthLong() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

પૂર્ણાંક Device::nativeVectorWidthFloat() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

પૂર્ણાંક Device::nativeVectorWidthDouble() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

પૂર્ણાંક Device::nativeVectorWidthHalf() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

પૂર્ણાંક Device::preferredVectorWidthChar() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

પૂર્ણાંક Device::preferredVectorWidthShort() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

પૂર્ણાંક Device::preferredVectorWidthInt() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

પૂર્ણાંક Device::preferredVectorWidthLong() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

પૂર્ણાંક Device::preferredVectorWidthFloat() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

પૂર્ણાંક Device::preferredVectorWidthDouble() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

પૂર્ણાંક Device::preferredVectorWidthHalf() અચલ
{ પાછા p ? p->getProp<cl_uint, પૂર્ણાંક>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() અચલ
#ifdef CL_VERSION_1_2
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#નહીંતર
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() અચલ
{ પાછા p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

અચલ Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    જો (!c.empty())
    {
        પાછા c.getDevice();
    }

    સ્થિર Device dummy;
    પાછા dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
અંતઃરેખ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ખાલી, &required);
    જો (err != CL_SUCCESS)
        પાછા err;

    param.clear();
    જો (required > 0)
    {
        AutoBuffer<અક્ષર> buf(required + 1);
        અક્ષર* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, ખાલી);
        જો (err != CL_SUCCESS)
            પાછા err;
        param = ptr;
    }

    પાછા CL_SUCCESS;
}

સ્થિર શૂન્ય split(અચલ std::string &s, અક્ષર delim, std::vector<std::string> &elems)
{
    elems.clear();
    જો (s.size() == 0)
        પાછા;
    std::istringstream ss(s);
    std::string item;
    જ્યારે (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
સ્થિર તાર્કિક parseOpenCLDeviceConfiguration(અચલ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    જો (parts.size() > 3)
    {
        CV_LOG_ERROR(ખાલી, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        પાછા ખોટું;
    }
    જો (parts.size() > 2)
        deviceNameOrID = parts[2];
    જો (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    જો (parts.size() > 0)
    {
        platform = parts[0];
    }
    પાછા સાચું;
}

સ્થિર cl_device_id selectOpenCLDevice(અચલ std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    જો (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    જો (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        પાછા ખાલી;

    તાર્કિક isID = ખોટું;
    પૂર્ણાંક deviceID = -1;
    જો (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = સાચું;
        માટે (size_t i = 0; i < deviceName.length(); i++)
        {
            જો (!isdigit(deviceName[i]))
            {
                isID = ખોટું;
                તોડો;
            }
        }
        જો (isID)
        {
            deviceID = atoi(deviceName.c_str());
            જો (deviceID < 0)
                પાછા ખાલી;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint અંક_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ખાલી, &અંક_Platforms));

        જો (અંક_Platforms == 0)
            પાછા ખાલી;
        platforms.resize((size_t)અંક_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(અંક_Platforms, &platforms[0], &અંક_Platforms));
        platforms.resize(અંક_Platforms);
    }

    જો (platform.length() > 0)
    {
        માટે (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            જો (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            નહીંતર
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        જો (platforms.size() == 0)
        {
            CV_LOG_ERROR(ખાલી, "OpenCL: Can't find OpenCL platform by name: " << platform);
            જાઓ not_found;
        }
    }
    જો (deviceTypes.size() == 0)
    {
        જો (!isID)
        {
            deviceTypes.push_back("GPU");
            જો (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        નહીંતર
            deviceTypes.push_back("ALL");
    }
    માટે (size_t t = 0; t < deviceTypes.size(); t++)
    {
        પૂર્ણાંક deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        જો (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        નહીંતર જો (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        નહીંતર જો (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        નહીંતર જો (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        નહીંતર
        {
            CV_LOG_ERROR(ખાલી, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            જાઓ not_found;
        }

        std::vector<cl_device_id> devices;
        માટે (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ગણતરી_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, ખાલી, &ગણતરી_2);
            જો (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            જો (ગણતરી_2 == 0)
                ચાલુ;
            size_t base = devices.size();
            devices.resize(base + ગણતરી_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, ગણતરી_2, &devices[base], &ગણતરી_2);
            જો (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        માટે (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : સાચું) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = સાચું;
            જો(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, કદ(isIGPU), &isIGPU, ખાલી));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            જો ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                પાછા devices[i];
            }
        }
    }

not_found:
    જો (configuration.empty())
        પાછા ખાલી; // suppress messages on stderr

    std::ostringstream સંદેશ;
    સંદેશ << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    માટે (size_t t = 0; t < deviceTypes.size(); t++)
        સંદેશ << ' ' << deviceTypes[t];

    સંદેશ << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ખાલી, સંદેશ.str());
    પાછા ખાલી;
}

#ifdef HAVE_OPENCL_SVM
નામાકાશ svm {

ગણના AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

સ્થિર તાર્કિક checkForceSVMUmatUsage()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક force = ખોટું;
    જો (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", ખોટું);
        initialized = સાચું;
    }
    પાછા force;
}
સ્થિર તાર્કિક checkDisableSVMUMatUsage()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક force = ખોટું;
    જો (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", ખોટું);
        initialized = સાચું;
    }
    પાછા force;
}
સ્થિર તાર્કિક checkDisableSVM()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક force = ખોટું;
    જો (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", ખોટું);
        initialized = સાચું;
    }
    પાછા force;
}
// see SVMCapabilities
સ્થિર unsigned પૂર્ણાંક getSVMCapabilitiesMask()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર unsigned પૂર્ણાંક mask = 0;
    જો (!initialized)
    {
        અચલ std::string env_કિંમત = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        જો (env_કિંમત.empty())
        {
            પાછા ~0U; // all bits 1
        }
        mask = atoi(env_કિંમત.c_str());
        initialized = સાચું;
    }
    પાછા mask;
}
} // namespace
#endif

સ્થિર size_t get_Program_ગણતરી_Limit()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર size_t ગણતરી_2 = 0;
    જો (!initialized)
    {
        ગણતરી_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = સાચું;
    }
    પાછા ગણતરી_2;
}

સ્થિર પૂર્ણાંક g_contextId = 0;

વર્ગ OpenCLBufferPoolImpl;
વર્ગ OpenCLSVMBufferPoolImpl;

સંરચના Context::Impl
{
    સ્થિર Context::Impl* get(Context& context) { પાછા context.getImpl(); }

    પ્રકારવ્યાખ્યા std::deque<Context::Impl*> container_t;
    સ્થિર container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        સ્થિર container_t* g_contexts = નવું container_t();
        પાછા *g_contexts;
    }

સુરક્ષિત:
    Impl(અચલ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(ખોટું)
#endif
    {
        જો (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = આ;
    }

    ~Impl()
    {
#ifdef _WIN32
        જો (!cv::__termination)
#endif
        {
            જો (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ખાલી;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ખાલી;
        }
    }

    શૂન્ય init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_અંક_DEVICES, કદ(ndevices), &ndevices, ખાલી));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * કદ(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * કદ(cl_device_id), "");

        devices.clear();
        માટે (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    શૂન્ય __init_buffer_pools();  // w/o synchronization
    શૂન્ય _init_buffer_pools() અચલ
    {
        જો (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            જો (!bufferPool_)
            {
                const_cast<Impl*>(આ)->__init_buffer_pools();
            }
        }
    }
જાહેર:
    સ્થિર Impl* findContext(અચલ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        જો (configuration.empty() && !container.empty())
            પાછા container[0];
        માટે (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            જો (i && i->configuration == configuration)
            {
                પાછા i;
            }
        }
        પાછા ખાલી;
    }

    સ્થિર Impl* findOrCreateContext(અચલ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        જો (configuration_.empty())
        {
            અચલ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            જો (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        જો (impl)
        {
            CV_LOG_INFO(ખાલી, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            પાછા impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        જો (d == ખાલી)
            પાછા ખાલી;

        impl = નવું Impl(configuration);
        પ્રયાસ
        {
            impl->createFromDevice(d);
            જો (impl->handle)
                પાછા impl;
            delete impl;
            પાછા ખાલી;
        }
        પકડો (...)
        {
            delete impl;
            ફેંકો;
        }
    }

    સ્થિર Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (શૂન્ય*)h);
        Impl* impl = findContext(configuration);
        જો (impl)
        {
            CV_LOG_INFO(ખાલી, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            પાછા impl;
        }

        impl = નવું Impl(configuration);
        પ્રયાસ
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            પાછા impl;
        }
        પકડો (...)
        {
            delete impl;
            ફેંકો;
        }
    }

    સ્થિર Impl* findOrCreateContext(અચલ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (શૂન્ય*)d);
        Impl* impl = findContext(configuration);
        જો (impl)
        {
            CV_LOG_INFO(ખાલી, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            પાછા impl;
        }

        impl = નવું Impl(configuration);
        પ્રયાસ
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            પાછા impl;
        }
        પકડો (...)
        {
            delete impl;
            ફેંકો;
        }
    }

    શૂન્ય setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        જો (d == ખાલી)
            પાછા;

        createFromDevice(d);
    }

    શૂન્ય createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ખાલી);

        cl_platform_id pl = ખાલી;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, કદ(cl_platform_id), &pl, ખાલી));

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

        તાર્કિક ok = handle != 0 && status == CL_SUCCESS;
        જો( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        નહીંતર
            handle = ખાલી;
    }

    Program getProg(અચલ ProgramSource& src, અચલ String& buildflags, String& errmsg);

    શૂન્ય unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        માટે (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              જો (it != phash.end())
              {
                  જો (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      પાછા;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        જો (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            જો (prefix.empty())
            {
                CV_Assert(!devices.empty());
                અચલ Device& d = devices[0];
                પૂર્ણાંક bits = d.addressBits();
                જો (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                માટે (size_t i = 0; i < prefix.size(); i++)
                {
                    અક્ષર c = prefix[i];
                    જો (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        પાછા prefix;
    }

    std::string& getPrefixBase()
    {
        જો (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            જો (prefix_base.empty())
            {
                અચલ Device& d = devices[0];
                પૂર્ણાંક bits = d.addressBits();
                જો (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                માટે (size_t i = 0; i < prefix_base.size(); i++)
                {
                    અક્ષર c = prefix_base[i];
                    જો (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        પાછા prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    અચલ પૂર્ણાંક contextId;  // global unique ID
    અચલ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    પ્રકારવ્યાખ્યા std::map<std::string, Program> phash_t;
    phash_t phash;
    પ્રકારવ્યાખ્યા std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() અચલ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        પાછા *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() અચલ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        પાછા *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    શૂન્ય setUserContext(std::type_index typeId, અચલ std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        જો (it != userContextStorage.end())
            પાછા it->second;
        નહીંતર
            પાછા nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    તાર્કિક svmInitialized;
    તાર્કિક svmAvailable;
    તાર્કિક svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    શૂન્ય svmInit()
    {
        CV_Assert(handle != ખાલી);
        અચલ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((શૂન્ય)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, કદ(deviceCaps), &deviceCaps, ખાલી);
        જો (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            જાઓ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (પૂર્ણાંક)deviceCaps);
        CV_Assert(((શૂન્ય)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.કિંમત_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.કિંમત_2 &= svm::getSVMCapabilitiesMask();
        જો (svmCapabilities.કિંમત_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            જાઓ noSVM;
        }
        પ્રયાસ
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            શૂન્ય* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            જો (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            પ્રયાસ
            {
                તાર્કિક error = ખોટું;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                જો (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ખાલી, ખાલી))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                પ્રયાસ
                {
                    ((પૂર્ણાંક*)ptr)[0] = 100;
                }
                પકડો (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = સાચું;
                }
                જો (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, ખાલી, ખાલી))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                જો (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            પકડો (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ફેંકો;
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
        પકડો (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            પ્રયાસ
            {
                // Try HSA extension
                String extensions = device.extensions();
                જો (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    જાઓ noSVM;
                }
                cl_platform_id p = ખાલી;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, કદ(cl_platform_id), &p, ખાલી));
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
            પકડો (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                જાઓ noSVM;
            }
        }

        svmAvailable = સાચું;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = સાચું;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        પાછા;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = ખોટું;
        svmEnabled = ખોટું;
        svmCapabilities.કિંમત_2 = 0;
        svmInitialized = સાચું;
        svmFunctions.fn_clSVMAlloc = ખાલી;
        પાછા;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() અચલ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        પાછા *bufferPoolSVM_.get();
    }
#endif

    friend વર્ગ Program;
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
Context::Context(પૂર્ણાંક dtype)
{
    p = 0;
    create(dtype);
}

શૂન્ય Context::release()
{
    જો (p)
    {
        p->release();
        p = ખાલી;
    }
}

તાર્કિક Context::create()
{
    release();
    જો (!haveOpenCL())
        પાછા ખોટું;
    p = Impl::findOrCreateContext(std::string());
    જો (p && p->handle)
        પાછા સાચું;
    release();
    પાછા ખોટું;
}

// deprecated
તાર્કિક Context::create(પૂર્ણાંક dtype)
{
    જો( !haveOpenCL() )
        પાછા ખોટું;
    release();
    જો (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    નહીંતર જો (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    નહીંતર જો (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    નહીંતર
    {
        CV_LOG_ERROR(ખાલી, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    જો (p && !p->handle)
    {
        release();
    }
    પાછા p != 0;
}

Context::Context(અચલ Context& c)
{
    p = (Impl*)c.p;
    જો(p)
        p->addref();
}

Context& Context::operator = (અચલ Context& c)
{
    Impl* newp = (Impl*)c.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    જો (આ != &c) {
        જો(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    પાછા *આ;
}

શૂન્ય* Context::ptr() અચલ
{
    પાછા p == ખાલી ? ખાલી : p->handle;
}

size_t Context::ndevices() અચલ
{
    પાછા p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) અચલ
{
    સ્થિર Device dummy;
    પાછા !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    જો (!c.empty())
    {
        auto& ctx = c.getContext();
        પાછા ctx;
    }

    સ્થિર Context dummy;
    પાછા dummy;
}

Program Context::getProg(અચલ ProgramSource& prog,
                         અચલ String& buildopts, String& errmsg)
{
    પાછા p ? p->getProg(prog, buildopts, errmsg) : Program();
}

શૂન્ય Context::unloadProg(Program& prog)
{
    જો (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(શૂન્ય* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    પાછા ctx;
}

/* static */
Context Context::fromDevice(અચલ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    પાછા ctx;
}

/* static */
Context Context::create(અચલ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    પાછા ctx;
}

શૂન્ય* Context::getOpenCLContextProperty(પૂર્ણાંક propertyId) અચલ
{
    જો (p == ખાલી)
        પાછા nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ખાલી, &size));
    std::vector<cl_context_properties> prop(size / કદ(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), ખાલી));
    માટે (size_t i = 0; i < prop.size(); i += 2)
    {
        જો (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ખાલી, "OpenCL: found context property=" << propertyId << ") => " << (શૂન્ય*)prop[i + 1]);
            પાછા (શૂન્ય*)prop[i + 1];
        }
    }
    પાછા nullptr;
}

#ifdef HAVE_OPENCL_SVM
તાર્કિક Context::useSVM() અચલ
{
    Context::Impl* i = p;
    CV_Assert(i);
    જો (!i->svmInitialized)
        i->svmInit();
    પાછા i->svmEnabled;
}
શૂન્ય Context::setUseSVM(તાર્કિક enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    જો (!i->svmInitialized)
        i->svmInit();
    જો (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#નહીંતર
તાર્કિક Context::useSVM() અચલ { પાછા ખોટું; }
શૂન્ય Context::setUseSVM(તાર્કિક enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
નામાકાશ svm {

અચલ SVMCapabilities getSVMCapabilitites(અચલ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    જો (!i->svmInitialized)
        i->svmInit();
    પાછા i->svmCapabilities;
}

CV_EXPORTS અચલ SVMFunctions* getSVMFunctions(અચલ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ખાલી);
    પાછા &i->svmFunctions;
}

CV_EXPORTS તાર્કિક useSVM(UMatUsageFlags usageFlags)
{
    જો (checkForceSVMUmatUsage())
        પાછા સાચું;
    જો (checkDisableSVMUMatUsage())
        પાછા ખોટું;
    જો ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        પાછા સાચું;
    પાછા ખોટું; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

શૂન્ય Context::setUserContext(std::type_index typeId, અચલ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    પાછા p->getUserContext(typeId);
}

સ્થિર શૂન્ય get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<અક્ષર> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
શૂન્ય attachContext(અચલ String& platformName, શૂન્ય* platformID, શૂન્ય* context, શૂન્ય* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        અચલ std::string& platformName, શૂન્ય* platformID, શૂન્ય* context, શૂન્ય* deviceID
)
{
    જો (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    જો (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    તાર્કિક platformAvailable = ખોટું;

    // check if external platformName contained in list of available platforms in OpenCV
    માટે (unsigned પૂર્ણાંક i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        જો (platformName == availablePlatformName)
        {
            platformAvailable = સાચું;
            તોડો;
        }
    }

    જો (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    જો (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    પાછા ctx;
}

શૂન્ય initializeContextFromHandle(Context& ctx, શૂન્ય* _platform, શૂન્ય* _context, શૂન્ય* _device)
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

સંરચના Queue::Impl
{
    અંતઃરેખ શૂન્ય __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = ખોટું;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, કદ(cl_command_queue_properties), &props, ખાલી));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, તાર્કિક isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(અચલ Context& c, અચલ Device& d, તાર્કિક withProfiling = ખોટું)
    {
        __init();

        અચલ Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        જો( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        જો( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        જો (!cv::__termination)
#endif
        {
            જો(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = ખાલી;
            }
        }
    }

    અચલ cv::ocl::Queue& getProfilingQueue(અચલ cv::ocl::Queue& self)
    {
        જો (isProfilingQueue_)
            પાછા self;

        જો (profiling_queue_.ptr())
            પાછા profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, કદ(cl_context), &ctx, ખાલી));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, કદ(cl_device_id), &device, ખાલી));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = નવું Impl(q, સાચું);
        profiling_queue_ = queue;

        પાછા profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    તાર્કિક isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(અચલ Context& c, અચલ Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(અચલ Queue& q)
{
    p = q.p;
    જો(p)
        p->addref();
}

Queue& Queue::operator = (અચલ Queue& q)
{
    Impl* newp = (Impl*)q.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    જો (આ != &q) {
        જો(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    પાછા *આ;
}

Queue::~Queue()
{
    જો(p)
        p->release();
}

તાર્કિક Queue::create(અચલ Context& c, અચલ Device& d)
{
    જો(p)
        p->release();
    p = નવું Impl(c, d);
    પાછા p->handle != 0;
}

શૂન્ય Queue::finish()
{
    જો(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

અચલ Queue& Queue::getProfilingQueue() અચલ
{
    CV_Assert(p);
    પાછા p->getProfilingQueue(*આ);
}

શૂન્ય* Queue::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    જો (!c.empty())
    {
        auto& q = c.getQueue();
        પાછા q;
    }
    સ્થિર Queue dummy;
    પાછા dummy;
}

સ્થિર cl_command_queue getQueue(અચલ Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    જો(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    પાછા qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(પૂર્ણાંક _flags, UMat* _m, પૂર્ણાંક _wscale, પૂર્ણાંક _iwscale, અચલ શૂન્ય* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ખાલી);
}

KernelArg KernelArg::Constant(અચલ Mat& m)
{
    CV_Assert(m.isContinuous());
    પાછા KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

સંરચના Kernel::Impl
{
    Impl(અચલ અક્ષર* kname, અચલ Program& prog) :
        refcount(1), handle(ખાલી), isInProgress(ખોટું), isAsyncRun(ખોટું), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        જો (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        માટે( પૂર્ણાંક i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = ખોટું;
        haveTempSrcUMats = ખોટું;
    }

    શૂન્ય cleanupUMats()
    {
        તાર્કિક exceptionOccurred = ખોટું;
        માટે( પૂર્ણાંક i = 0; i < MAX_ARRS; i++ )
        {
            જો( u[i] )
            {
                જો( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    પ્રયાસ
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    પકડો(અચલ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ખાલી, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = સાચું;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = ખોટું;
        haveTempSrcUMats = ખોટું;
        CV_Assert(!exceptionOccurred);
    }

    શૂન્ય addUMat(અચલ UMat& m, તાર્કિક dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        જો(dst && m.u->tempUMat())
            haveTempDstUMats = સાચું;
        જો(m.u->originalUMatData == ખાલી && m.u->tempUMat())
            haveTempSrcUMats = સાચું;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    શૂન્ય registerImageArgument(પૂર્ણાંક arg, અચલ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        જો (arg < (પૂર્ણાંક)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    શૂન્ય finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = ખોટું;
        પ્રયાસ
        {
            cleanupUMats();
        }
        પકડો(...)
        {
            release();
            ફેંકો;
        }
        release();
    }

    તાર્કિક run(પૂર્ણાંક dims, size_t _globalsize[], size_t _localsize[],
            તાર્કિક સમકાલિક, int64* timeNS, અચલ Queue& q);

    ~Impl()
    {
        જો(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    ગણના { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    તાર્કિક isInProgress;
    તાર્કિક isAsyncRun;  // true if kernel was scheduled in async mode
    પૂર્ણાંક nu;
    std::vector<Image2D> shadow_images;
    તાર્કિક haveTempDstUMats;
    તાર્કિક haveTempSrcUMats;
};

}} // namespace cv::ocl

બાહ્ય "C" {

સ્થિર શૂન્ય CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, શૂન્ય *p)
{
    પ્રયાસ
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    પકડો (અચલ cv::Exception& exc)
    {
        CV_LOG_ERROR(ખાલી, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    પકડો (અચલ std::exception& exc)
    {
        CV_LOG_ERROR(ખાલી, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    પકડો (...)
    {
        CV_LOG_ERROR(ખાલી, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

નામાકાશ cv { નામાકાશ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(અચલ અક્ષર* kname, અચલ Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(અચલ અક્ષર* kname, અચલ ProgramSource& src,
               અચલ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(અચલ Kernel& k)
{
    p = k.p;
    જો(p)
        p->addref();
}

Kernel& Kernel::operator = (અચલ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    જો (આ != &k) {
        જો(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    પાછા *આ;
}

Kernel::~Kernel()
{
    જો(p)
        p->release();
}

તાર્કિક Kernel::create(અચલ અક્ષર* kname, અચલ Program& prog)
{
    જો(p)
        p->release();
    p = નવું Impl(kname, prog);
    જો(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    પાછા p != 0;
}

તાર્કિક Kernel::create(અચલ અક્ષર* kname, અચલ ProgramSource& src,
                    અચલ String& buildopts, String* errmsg)
{
    જો(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    જો( !errmsg ) errmsg = &tempmsg;
    અચલ Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    પાછા create(kname, prog);
}

શૂન્ય* Kernel::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

તાર્કિક Kernel::empty() અચલ
{
    પાછા ptr() == 0;
}

સ્થિર cv::String dump_કિંમત(size_t sz, અચલ શૂન્ય* p)
{
    જો (!p)
        પાછા "NULL";
    જો (sz == 2)
        પાછા cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    જો (sz == 4)
        પાછા cv::format("%d / %uu / 0x%08x / %g", *(પૂર્ણાંક*)p, *(પૂર્ણાંક*)p, *(પૂર્ણાંક*)p, *(દશાંશ*)p);
    જો (sz == 8)
        પાછા cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(દ્વિગુણ*)p);
    પાછા cv::format("%p", p);
}

પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ શૂન્ય* value, size_t sz)
{
    જો (!p || !p->handle)
        પાછા -1;
    જો (i < 0)
        પાછા i;
    જો( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (પૂર્ણાંક)i, (પૂર્ણાંક)sz, dump_કિંમત(sz, value).c_str()).c_str());
    જો (retval != CL_SUCCESS)
        પાછા -1;
    પાછા i+1;
}

પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    પૂર્ણાંક res = set(i, &h, કદ(h));
    જો (res >= 0)
        p->registerImageArgument(i, image2D);
    પાછા res;
}

પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ UMat& m)
{
    પાછા set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
સ્થિર તાર્કિક isCudaBacked(અચલ UMatData* u)
{
    અચલ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    પાછા u && cudaAllocator && u->currAllocator == cudaAllocator;
}

પૂર્ણાંક Kernel::set(પૂર્ણાંક i, અચલ KernelArg& arg)
{
    જો( !p || !p->handle )
        પાછા -1;
    જો (i < 0)
    {
        CV_LOG_ERROR(ખાલી, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (પૂર્ણાંક)i));
        પાછા i;
    }
    જો( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    જો( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        તાર્કિક ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        જો (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ખાલી;
            status = clSetKernelArg(p->handle, (cl_uint)i, કદ(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (પૂર્ણાંક)i).c_str());
            પાછા i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        જો (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ખાલી, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (પૂર્ણાંક)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            પાછા -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        જો (!h)
        {
            CV_LOG_ERROR(ખાલી, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (પૂર્ણાંક)i, (પૂર્ણાંક)arg.flags, arg.m));
            p->release();
            p = 0;
            પાછા -1;
        }

#ifdef HAVE_OPENCL_SVM
        જો ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            અચલ Context& ctx = Context::getDefault();
            અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#જો 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#નહીંતર
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (પૂર્ણાંક)i, (શૂન્ય*)svmDataPtr).c_str());
        }
        નહીંતર
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, કદ(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (પૂર્ણાંક)i, (શૂન્ય*)h).c_str());
        }

        જો (ptronly)
        {
            i++;
        }
        નહીંતર જો( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), કદ(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+1), (પૂર્ણાંક)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), કદ(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+2), (પૂર્ણાંક)u2d.offset).c_str());
            i += 3;

            જો( !(arg.flags & KernelArg::NO_SIZE) )
            {
                પૂર્ણાંક cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, કદ(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (પૂર્ણાંક)i, (પૂર્ણાંક)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), કદ(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+1), (પૂર્ણાંક)cols).c_str());
                i += 2;
            }
        }
        નહીંતર
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), કદ(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+1), (પૂર્ણાંક)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), કદ(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+2), (પૂર્ણાંક)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), કદ(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+3), (પૂર્ણાંક)u3d.offset).c_str());
            i += 4;
            જો( !(arg.flags & KernelArg::NO_SIZE) )
            {
                પૂર્ણાંક cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, કદ(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (પૂર્ણાંક)i, (પૂર્ણાંક)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), કદ(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+1), (પૂર્ણાંક)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), કદ(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (પૂર્ણાંક)(i+2), (પૂર્ણાંક)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        પાછા i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (પૂર્ણાંક)i, (પૂર્ણાંક)arg.sz, (શૂન્ય*)arg.obj).c_str());
    પાછા i+1;
}

તાર્કિક Kernel::run(પૂર્ણાંક dims, size_t _globalsize[], size_t _localsize[],
                 તાર્કિક સમકાલિક, અચલ Queue& q)
{
    જો (!p)
        પાછા ખોટું;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != ખાલી);
    માટે (પૂર્ણાંક i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(પૂર્ણાંક)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        જો (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned પૂર્ણાંક)val) * val;
    }
    CV_Assert(total > 0);

    પાછા p->run(dims, globalsize, _localsize, સમકાલિક, ખાલી, q);
}


તાર્કિક Kernel::run_(પૂર્ણાંક dims, size_t _globalsize[], size_t _localsize[],
                  તાર્કિક સમકાલિક, અચલ Queue& q)
{
    CV_Assert(p);
    પાછા p->run(dims, _globalsize, _localsize, સમકાલિક, ખાલી, q);
}


તાર્કિક Kernel::Impl::run(પૂર્ણાંક dims, size_t globalsize[], size_t localsize[],
        તાર્કિક સમકાલિક, int64* timeNS, અચલ Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    જો (!handle)
    {
        CV_LOG_ERROR(ખાલી, "OpenCL kernel has zero handle: " << name);
        પાછા ખોટું;
    }

    જો (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !સમકાલિક;

    જો (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#જો CV_OPENCL_SYNC_RUN_KERNELS
    સમકાલિક = સાચું;
#endif

    cl_command_queue qq = getQueue(q);
    જો (haveTempDstUMats)
        સમકાલિક = સાચું;
    જો (haveTempSrcUMats)
        સમકાલિક = સાચું;
    જો (timeNS)
        સમકાલિક = સાચું;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           ખાલી, globalsize, localsize, 0, 0,
                                           (સમકાલિક && !timeNS) ? 0 : &asyncEvent);
#જો !CV_OPENCL_SHOW_RUN_KERNELS
    જો (retval != CL_SUCCESS)
#endif
    {
        cv::String સંદેશ = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (પૂર્ણાંક)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        સમકાલિક ? "true" : "false"
                        );
        જો (retval != CL_SUCCESS)
        {
            સંદેશ = CV_OCL_API_ERROR_સંદેશ(retval, સંદેશ.c_str());
        }
#જો CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, સંદેશ.c_str());
#નહીંતર
        printf("%s\n", સંદેશ.c_str());
        fflush(stdout);
#endif
    }
    જો (સમકાલિક || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        જો (timeNS)
        {
            જો (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, કદ(startTime), &startTime, ખાલી));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, કદ(stopTime), &stopTime, ખાલી));
                *timeNS = (int64)(stopTime - startTime);
            }
            નહીંતર
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    નહીંતર
    {
        addref();
        isInProgress = સાચું;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, આ));
    }
    જો (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    પાછા retval == CL_SUCCESS;
}

તાર્કિક Kernel::runTask(તાર્કિક સમકાલિક, અચલ Queue& q)
{
    જો(!p || !p->handle || p->isInProgress)
        પાછા ખોટું;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, સમકાલિક ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), સમકાલિક ? "true" : "false").c_str());
    જો (સમકાલિક || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    નહીંતર
    {
        p->addref();
        p->isInProgress = સાચું;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    જો (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    પાછા retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(પૂર્ણાંક dims, size_t globalsize[], size_t localsize[], અચલ Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    તાર્કિક res = p->run(dims, globalsize, localsize, સાચું, &timeNs, profilingQueue);
    પાછા res ? timeNs : -1;
}

size_t Kernel::workGroupSize() અચલ
{
    જો(!p || !p->handle)
        પાછા 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, કદ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    પાછા status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() અચલ
{
    જો(!p || !p->handle)
        પાછા 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, કદ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    પાછા status == CL_SUCCESS ? val : 0;
}

તાર્કિક Kernel::compileWorkGroupSize(size_t wsz[]) અચલ
{
    જો(!p || !p->handle || !wsz)
        પાછા 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, કદ(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    પાછા status == CL_SUCCESS;
}

size_t Kernel::localMemSize() અચલ
{
    જો(!p || !p->handle)
        પાછા 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, કદ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    પાછા status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

સંરચના ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    ગણના KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(અચલ String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(અચલ String& module, અચલ String& name, અચલ String& codeStr, અચલ String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    શૂન્ય init(ગણના KIND kind, અચલ String& module, અચલ String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = ખાલી;
        sourceSize_ = 0;
        isHashUpdated = ખોટું;
    }

    શૂન્ય initFromSource(અચલ String& codeStr, અચલ String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        જો (sourceHash_.empty())
        {
            updateHash();
        }
        નહીંતર
        {
            isHashUpdated = સાચું;
        }
    }

    શૂન્ય updateHash(અચલ અક્ષર* hashStr = ખાલી)
    {
        જો (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = સાચું;
            પાછા;
        }
        uint64 hash = 0;
        પસંદગી (kind_)
        {
        સ્થિતિ PROGRAM_SOURCE_CODE:
            જો (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            નહીંતર
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            તોડો;
        સ્થિતિ PROGRAM_BINARIES:
        સ્થિતિ PROGRAM_SPIR:
        સ્થિતિ PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            તોડો;
        મૂળભૂત:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = સાચું;
    }

    Impl(ગણના KIND kind,
            અચલ String& module, અચલ String& name,
            અચલ unsigned અક્ષર* binary, અચલ size_t size,
            અચલ cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    સ્થિર ProgramSource fromSourceWithStaticLifetime(અચલ String& module, અચલ String& name,
            અચલ અક્ષર* sourceCodeStaticStr, અચલ અક્ષર* hashStaticStr,
            અચલ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = નવું Impl(PROGRAM_SOURCE_CODE, module, name,
                (અચલ unsigned અક્ષર*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        પાછા result;
    }

    સ્થિર ProgramSource from_દ્વિઅંકી(અચલ String& module, અચલ String& name,
            અચલ unsigned અક્ષર* binary, અચલ size_t size,
            અચલ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = નવું Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        પાછા result;
    }

    સ્થિર ProgramSource fromSPIR(અચલ String& module, અચલ String& name,
            અચલ unsigned અક્ષર* binary, અચલ size_t size,
            અચલ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = નવું Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        પાછા result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    અચલ unsigned અક્ષર* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    તાર્કિક isHashUpdated;

    friend સંરચના Program::Impl;
    friend સંરચના internal::ProgramEntry;
    friend સંરચના Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(અચલ String& module, અચલ String& name, અચલ String& codeStr, અચલ String& codeHash)
{
    p = નવું Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(અચલ અક્ષર* prog)
{
    p = નવું Impl(prog);
}

ProgramSource::ProgramSource(અચલ String& prog)
{
    p = નવું Impl(prog);
}

ProgramSource::~ProgramSource()
{
    જો(p)
        p->release();
}

ProgramSource::ProgramSource(અચલ ProgramSource& prog)
{
    p = prog.p;
    જો(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (અચલ ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    જો (આ != &prog) {
        જો(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    પાછા *આ;
}

અચલ String& ProgramSource::source() અચલ
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == ખાલી); // method returns reference - can't construct temporary object
    પાછા p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() અચલ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_દ્વિઅંકી(અચલ String& module, અચલ String& name,
        અચલ unsigned અક્ષર* binary, અચલ size_t size,
        અચલ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    પાછા Impl::from_દ્વિઅંકી(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(અચલ String& module, અચલ String& name,
        અચલ unsigned અક્ષર* binary, અચલ size_t size,
        અચલ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    પાછા Impl::from_દ્વિઅંકી(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() અચલ
{
    જો (આ->pProgramSource == ખાલી)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        જો (આ->pProgramSource == ખાલી)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(આ->module, આ->name, આ->programCode, આ->programHash, cv::String());
            ProgramSource* ptr = નવું ProgramSource(ps);
            const_cast<ProgramEntry*>(આ)->pProgramSource = ptr;
        }
    }
    પાછા *આ->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

સ્થિર
cv::String joinBuildOptions(અચલ cv::String& a, અચલ cv::String& b)
{
    જો (b.empty())
        પાછા a;
    જો (a.empty())
        પાછા b;
    જો (b[0] == ' ')
        પાછા a + b;
    પાછા a + (cv::String(" ") + b);
}

સંરચના Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(અચલ ProgramSource& src,
         અચલ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ખાલી),
         buildflags(_buildflags)
    {
        અચલ ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        અચલ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        જો (ctx.ptr() == ખાલી || device.ptr() == ખાલી)
            પાછા;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        જો (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            જો (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            નહીંતર જો (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            અચલ String param_buildExtraOptions = getBuildExtraOptions();
            જો (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#જો CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ખાલી, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#જો CV_OPENCL_SHOW_BUILD_KERNELS
        જો (handle)
        {
            size_t retsz = 0;
            અક્ષર kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, કદ(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            જો (result == CL_SUCCESS && retsz < કદ(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(ખાલી, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            નહીંતર
            {
                CV_LOG_ERROR(ખાલી, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    તાર્કિક compile(અચલ Context& ctx, અચલ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        જો (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(ખાલી, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            તાર્કિક isLoaded = create_From_દ્વિઅંકી(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            પાછા isLoaded;
        }
        પાછા compileWithCache(ctx, src_, errmsg);
    }

    તાર્કિક compileWithCache(અચલ Context& ctx, અચલ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        અચલ std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        અચલ String& hash_str = src_->sourceHash_;
        cv::String fname;
        જો (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        અચલ cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        જો (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            પ્રયાસ
            {
                std::vector<અક્ષર> દ્વિઅંકી_Buf;
                તાર્કિક res = ખોટું;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    દ્વિઅંકી_Program_File file(fname, hash_str.c_str());
                    res = file.પઠ(buildflags, દ્વિઅંકી_Buf);
                }
                જો (res)
                {
                    CV_Assert(!દ્વિઅંકી_Buf.empty());
                    CV_LOG_VERBOSE(ખાલી, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    તાર્કિક isLoaded = create_From_દ્વિઅંકી(ctx, દ્વિઅંકી_Buf, errmsg);
                    જો (isLoaded)
                        પાછા સાચું;
                }
            }
            પકડો (અચલ cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(ખાલી, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            પકડો (...)
            {
                CV_LOG_VERBOSE(ખાલી, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ખાલી);
        જો (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            જો (!buildFromSources(ctx, src_, errmsg))
            {
                પાછા ખોટું;
            }
        }
        નહીંતર જો (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            જો ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ખાલી, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            તાર્કિક isLoaded = create_From_દ્વિઅંકી(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            જો (!isLoaded)
                પાછા ખોટું;
        }
        નહીંતર જો (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        નહીંતર
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != ખાલી);
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
        જો (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            પ્રયાસ
            {
                std::vector<અક્ષર> દ્વિઅંકી_Buf;
                get_Program_દ્વિઅંકી(દ્વિઅંકી_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    દ્વિઅંકી_Program_File file(fname, hash_str.c_str());
                    file.લિખ(buildflags, દ્વિઅંકી_Buf);
                }
            }
            પકડો (અચલ cv::Exception& e)
            {
                CV_LOG_WARNING(ખાલી, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            પકડો (...)
            {
                CV_LOG_WARNING(ખાલી, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#જો CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS
        જો (CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS_કિંમત)
        {
            std::vector<અક્ષર> દ્વિઅંકી_Buf;
            get_Program_દ્વિઅંકી(દ્વિઅંકી_Buf);
            જો (!દ્વિઅંકી_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ખાલી;
                create_From_દ્વિઅંકી(ctx, દ્વિઅંકી_Buf, errmsg);
            }
        }
#endif
        પાછા handle != ખાલી;
    }

    શૂન્ય dumpBuildLog_(cl_int result, અચલ cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<અક્ષર, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        જો (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            જો (log_retval == CL_SUCCESS)
            {
                જો (retsz < buffer.size())
                    buffer[retsz] = 0;
                નહીંતર
                    buffer[buffer.size() - 1] = 0;
            }
            નહીંતર
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

    તાર્કિક buildFromSources(અચલ Context& ctx, અચલ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == ખાલી);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ખાલી, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        અચલ અક્ષર* srcptr = src_->sourceAddr_ ? ((અચલ અક્ષર*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != ખાલી);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        જો (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            માટે (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#જો !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            જો (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                જો (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ખાલી;
                }
                જો (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#જો CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS
            જો (handle && CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS_કિંમત)
            {
                CV_LOG_INFO(ખાલી, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                અક્ષર kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, કદ(kernels_buffer), &kernels_buffer[0], &retsz);
                જો (retsz < કદ(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                નહીંતર
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(ખાલી, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        પાછા handle != ખાલી;
    }

    શૂન્ય get_Program_દ્વિઅંકી(std::vector<અક્ષર>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_દ્વિઅંકી_SIZES, કદ(sz), &sz, ખાલી));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, કદ(ptr), &ptr, ખાલી));
    }

    તાર્કિક create_From_દ્વિઅંકી(અચલ Context& ctx, અચલ std::vector<અક્ષર>& buf, String& errmsg)
    {
        પાછા create_From_દ્વિઅંકી(ctx, (અચલ unsigned અક્ષર*)&buf[0], buf.size(), errmsg);
    }

    તાર્કિક create_From_દ્વિઅંકી(અચલ Context& ctx, અચલ unsigned અક્ષર* દ્વિઅંકી_Addr, અચલ size_t દ્વિઅંકી_Size, String& errmsg)
    {
        CV_Assert(handle == ખાલી);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ખાલી, 0, "Load from binary... (" << દ્વિઅંકી_Size << " bytes)");

        CV_Assert(દ્વિઅંકી_Size > 0);

        size_t ndevices = (પૂર્ણાંક)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<અચલ uchar*> દ્વિઅંકી_Ptrs(ndevices);
        AutoBuffer<size_t> દ્વિઅંકી_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        અચલ uchar** દ્વિઅંકી_Ptrs_2 = દ્વિઅંકી_Ptrs.data();
        size_t* દ્વિઅંકી_Sizes_2 = દ્વિઅંકી_Sizes.data();
        માટે (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            દ્વિઅંકી_Ptrs_2[i] = દ્વિઅંકી_Addr;
            દ્વિઅંકી_Sizes_2[i] = દ્વિઅંકી_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_દ્વિઅંકી((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           દ્વિઅંકી_Sizes_2, દ્વિઅંકી_Ptrs_2, ખાલી, &result);
        જો (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(ખાલી, CV_OCL_API_ERROR_સંદેશ(result, "clCreateProgramWithBinary"));
            જો (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ખાલી;
            }
        }
        જો (!handle)
        {
            પાછા ખોટું;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            જો (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                જો (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ખાલી;
                }
                પાછા ખોટું;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    કદ(build_status), &build_status, &retsz));
            જો (result == CL_SUCCESS)
            {
                જો (build_status == CL_BUILD_SUCCESS)
                {
                    પાછા સાચું;
                }
                નહીંતર
                {
                    CV_LOG_WARNING(ખાલી, "clGetProgramBuildInfo() returns " << build_status);
                    પાછા ખોટું;
                }
            }
            નહીંતર
            {
                CV_LOG_ERROR(ખાલી, CV_OCL_API_ERROR_સંદેશ(result, "clGetProgramBuildInfo()"));
                જો (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ખાલી;
                }
            }
        }
#જો CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS
        જો (handle && CV_OPENCL_VALIDATE_દ્વિઅંકી_PROGRAMS_કિંમત)
        {
            CV_LOG_INFO(ખાલી, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            અક્ષર kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, કદ(kernels_buffer), &kernels_buffer[0], &retsz);
            જો (retsz < કદ(kernels_buffer))
                kernels_buffer[retsz] = 0;
            નહીંતર
                kernels_buffer[0] = 0;
            CV_LOG_INFO(ખાલી, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        પાછા handle != ખાલી;
    }

    ~Impl()
    {
        જો( handle )
        {
#ifdef _WIN32
            જો (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = ખાલી;
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

Program::Program(અચલ ProgramSource& src,
        અચલ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(અચલ Program& prog)
{
    p = prog.p;
    જો(p)
        p->addref();
}

Program& Program::operator = (અચલ Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    જો(newp)
        newp->addref();
    જો(p)
        p->release();
    p = newp;
    પાછા *આ;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    જો (આ != &prog) {
        જો(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    પાછા *આ;
}

Program::~Program()
{
    જો(p)
        p->release();
}

તાર્કિક Program::create(અચલ ProgramSource& src,
            અચલ String& buildflags, String& errmsg)
{
    જો(p)
    {
        p->release();
        p = ખાલી;
    }
    p = નવું Impl(src, buildflags, errmsg);
    જો(!p->handle)
    {
        p->release();
        p = 0;
    }
    પાછા p != 0;
}

શૂન્ય* Program::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
અચલ ProgramSource& Program::source() અચલ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

તાર્કિક Program::પઠ(અચલ String& bin, અચલ String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

તાર્કિક Program::લિખ(String& bin) અચલ
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() અચલ
{
    જો(!p)
        પાછા String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    પાછા cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(અચલ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        પાછા cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

શૂન્ય Program::get_દ્વિઅંકી(std::vector<અક્ષર>& binary) અચલ
{
    CV_Assert(p && "Empty program");
    p->get_Program_દ્વિઅંકી(binary);
}

Program Context::Impl::getProg(અચલ ProgramSource& src,
                               અચલ String& buildflags, String& errmsg)
{
    size_t limit = get_Program_ગણતરી_Limit();
    અચલ ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ચાવી = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(ચાવી);
        જો (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ચાવી);
            જો (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ચાવી);
            }
            પાછા it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            જો (limit > 0 && sz >= limit)
            {
                સ્થિર તાર્કિક warningFlag = ખોટું;
                જો (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = સાચું;
                }
                જ્યારે (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    જો (c != 0)
                        તોડો;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(ચાવી, prog));
        cacheList.push_front(ચાવી);
    }
    પાછા prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
વર્ગ OpenCLBufferPool
{
સુરક્ષિત:
    ~OpenCLBufferPool() { }
જાહેર:
    આભાસી T allocate(size_t size) = 0;
    આભાસી શૂન્ય release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
વર્ગ OpenCLBufferPoolBaseImpl : જાહેર BufferPoolController, જાહેર OpenCLBufferPool<T>
{
ખાનગી:
    અંતઃરેખ Derived& derived() { પાછા *static_cast<Derived*>(આ); }
સુરક્ષિત:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    તાર્કિક _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        માટે (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            જો (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                પાછા સાચું;
            }
        }
        પાછા ખોટું;
    }

    // synchronized
    તાર્કિક _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, અચલ size_t size)
    {
        જો (reservedEntries_.empty())
            પાછા ખોટું;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        માટે (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            જો (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                જો (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    જો (diff == 0)
                        તોડો;
                }
            }
        }
        જો (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            પાછા સાચું;
        }
        પાછા ખોટું;
    }

    // synchronized
    શૂન્ય _checkSizeOfReservedEntries()
    {
        જ્યારે (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            અચલ BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    અંતઃરેખ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        જો (size < 1024*1024)
            પાછા 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        નહીંતર જો (size < 16*1024*1024)
            પાછા 64*1024;
        નહીંતર
            પાછા 1024*1024;
    }

જાહેર:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    આભાસી ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
જાહેર:
    આભાસી T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        જો (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        નહીંતર
        {
            derived()._allocateBufferEntry(entry, size);
        }
        પાછા entry.clBuffer_;
    }
    આભાસી શૂન્ય release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        જો (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        નહીંતર
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    આભાસી size_t getReservedSize() અચલ CV_OVERRIDE { પાછા currentReservedSize; }
    આભાસી size_t getMaxReservedSize() અચલ CV_OVERRIDE { પાછા maxReservedSize; }
    આભાસી શૂન્ય setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        જો (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            માટે (; i != reservedEntries_.end();)
            {
                અચલ BufferEntry& entry = *i;
                જો (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    ચાલુ;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    આભાસી શૂન્ય freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        માટે (; i != reservedEntries_.end(); ++i)
        {
            અચલ BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

સંરચના CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)ખાલી), capacity_(0) { }
};

વર્ગ OpenCLBufferPoolImpl CV_FINAL : જાહેર OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
જાહેર:
    પ્રકારવ્યાખ્યા સંરચના CLBufferEntry BufferEntry;
સુરક્ષિત:
    પૂર્ણાંક createFlags_;
જાહેર:
    OpenCLBufferPoolImpl(પૂર્ણાંક createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    શૂન્ય _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ખાલી);
        entry.capacity_ = alignSize(size, (પૂર્ણાંક)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long પૂર્ણાંક)entry.capacity_, (શૂન્ય*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != ખાલી);
        જો(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    શૂન્ય _releaseBufferEntry(અચલ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ખાલી);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
સંરચના CLSVMBufferEntry
{
    શૂન્ય* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(ખાલી), capacity_(0) { }
};
વર્ગ OpenCLSVMBufferPoolImpl CV_FINAL : જાહેર OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, શૂન્ય*>
{
જાહેર:
    પ્રકારવ્યાખ્યા સંરચના CLSVMBufferEntry BufferEntry;
જાહેર:
    OpenCLSVMBufferPoolImpl()
    {
    }

    શૂન્ય _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == ખાલી);
        entry.capacity_ = alignSize(size, (પૂર્ણાંક)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        અચલ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        તાર્કિક isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (પૂર્ણાંક)entry.capacity_);
        શૂન્ય *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    શૂન્ય _releaseBufferEntry(અચલ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != ખાલી);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <તાર્કિક readAccess, તાર્કિક writeAccess>
વર્ગ AlignedDataPtr
{
સુરક્ષિત:
    અચલ size_t size_;
    uchar* અચલ originPtr_;
    અચલ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

જાહેર:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ખાલી)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        જો (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = નવું uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            જો (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() અચલ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        પાછા ptr_;
    }

    ~AlignedDataPtr()
    {
        જો (allocatedPtr_)
        {
            જો (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ખાલી;
        }
        ptr_ = ખાલી;
    }
ખાનગી:
    AlignedDataPtr(અચલ AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(અચલ AlignedDataPtr&); // disabled
};

template <તાર્કિક readAccess, તાર્કિક writeAccess>
વર્ગ AlignedDataPtr2D
{
સુરક્ષિત:
    અચલ size_t size_;
    uchar* અચલ originPtr_;
    અચલ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

જાહેર:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ખાલી), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ખાલી);
        જો (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = નવું uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            જો (readAccess)
            {
                માટે (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() અચલ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        પાછા ptr_;
    }

    ~AlignedDataPtr2D()
    {
        જો (allocatedPtr_)
        {
            જો (writeAccess)
            {
                માટે (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ખાલી;
        }
        ptr_ = ખાલી;
    }
ખાનગી:
    AlignedDataPtr2D(અચલ AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(અચલ AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


શૂન્ય Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(ખાલી, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

વર્ગ OpenCLAllocator CV_FINAL : જાહેર MatAllocator
{
જાહેર:
    ગણના AllocatorFlags
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

    UMatData* defaultAllocate(પૂર્ણાંક dims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type, શૂન્ય* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) અચલ
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        પાછા u;
    }

    સ્થિર તાર્કિક isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        સ્થિર તાર્કિક value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", ખોટું);
        પાછા value;
    }
    સ્થિર તાર્કિક isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        સ્થિર તાર્કિક value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", ખોટું);
        પાછા value;
    }

    શૂન્ય getBestFlags(અચલ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, પૂર્ણાંક& createFlags, UMatData::MemoryFlag& flags0) અચલ
    {
        અચલ Device& dev = ctx.device(0);
        createFlags = 0;
        જો ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        જો (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        નહીંતર
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(પૂર્ણાંક dims, અચલ પૂર્ણાંક* sizes, પૂર્ણાંક type,
                       શૂન્ય* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) અચલ CV_OVERRIDE
    {
        જો(!useOpenCL())
            પાછા defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        માટે( પૂર્ણાંક i = dims-1; i >= 0; i-- )
        {
            જો( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        જો (!ctx.getImpl())
            પાછા defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        પૂર્ણાંક createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        શૂન્ય* handle = ખાલી;
        પૂર્ણાંક allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        અચલ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        જો (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            તાર્કિક isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        નહીંતર
#endif
        જો (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        નહીંતર જો (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        નહીંતર
        {
            CV_Assert(handle != ખાલી); // Unsupported, throw
        }

        જો (!handle)
            પાછા defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = નવું UMatData(આ);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<શૂન્ય>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(સાચું);
        opencl_allocator_stats.onAllocate(u->size);
        પાછા u;
    }

    તાર્કિક allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા ખોટું;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        જો(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            પૂર્ણાંક createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            તાર્કિક copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            પૂર્ણાંક allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            શૂન્ય* handle = ખાલી;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            તાર્કિક useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            જો (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (પૂર્ણાંક)u->size, handle);
            }
            નહીંતર જો (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                જો (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    તાર્કિક isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (પૂર્ણાંક)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = ખાલી;
                    જો (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (પૂર્ણાંક)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ખાલી, ખાલી);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    જો (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, ખાલી, ખાલી);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            નહીંતર
#endif
            {
                જો( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                જો (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (પૂર્ણાંક)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long પૂર્ણાંક)u->size, u->origdata, (શૂન્ય*)handle).c_str());
                }
                જો((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long પૂર્ણાંક)u->size, u->origdata, (શૂન્ય*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (શૂન્ય*)handle).c_str());
            જો(!handle || retval != CL_SUCCESS)
                પાછા ખોટું;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = આ;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        જો (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(સાચું);
        opencl_allocator_stats.onAllocate(u->size);
        પાછા સાચું;
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

    શૂન્ય deallocate(UMatData* u) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        જો (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        નહીંતર
            deallocate_(u);
    }

    શૂન્ય deallocate_(UMatData* u) અચલ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        જો ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        જો (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            પાછા;             // avoid any OpenCL calls
#endif
        જો(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            જો (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    જો( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        તાર્કિક isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = ખાલી;
                        જો (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (પૂર્ણાંક)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ખાલી, ખાલી);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        જો (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ખાલી, ખાલી);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    નહીંતર
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                નહીંતર
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    જો( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<ખોટું, સાચું> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    નહીંતર
                    {
                        cl_int retval = 0;
                        જો (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            શૂન્ય* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (શૂન્ય*)u->handle, (long long પૂર્ણાંક)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            જો (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (શૂન્ય*)u->handle, data, (long long પૂર્ણાંક)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(ખોટું);
            }
            નહીંતર
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                જો( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            નહીંતર
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (શૂન્ય*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(સાચું);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ખાલી;
            જો(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = ખાલી;
        }
        નહીંતર
        {
            CV_Assert(u->origdata == ખાલી);
            જો(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(સાચું);
            }
            જો (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            નહીંતર જો (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            નહીંતર જો (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                નહીંતર જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ખાલી, ખાલી);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((શૂન્ય*)u->handle);
            }
#endif
            નહીંતર
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(સાચું);
            delete u;
            u = ખાલી;
        }
        CV_Assert(u == ખાલી);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    શૂન્ય map(UMatData* u, AccessFlag accessFlags) અચલ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        જો (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(સાચું);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            જો( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (પૂર્ણાંક)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ખાલી, ખાલી);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(ખોટું);
                    u->markDeviceMemMapped(સાચું);
                    પાછા;
                }
#endif

                cl_int retval = CL_SUCCESS;
                જો (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (શૂન્ય*)u->handle, (long long પૂર્ણાંક)u->size, u->data).c_str());
                }
                જો (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(ખોટું);
                    u->markDeviceMemMapped(સાચું);
                    પાછા;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            જો(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(સાચું);
            }
        }

        જો (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<ખોટું, સાચું> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (શૂન્ય*)u->handle, (long long પૂર્ણાંક)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(ખોટું);
        }
    }

    શૂન્ય unmap(UMatData* u) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        જો( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != ખાલી);
#ifdef HAVE_OPENCL_SVM
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, ખાલી, ખાલી);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                જો (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(ખોટું);
                u->markHostCopyObsolete(સાચું);
                પાછા;
            }
#endif
            જો (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (શૂન્ય*)u->handle, u->data, (long long પૂર્ણાંક)u->size).c_str());
                જો (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(ખોટું);
                u->data = 0;
                u->markDeviceCopyObsolete(ખોટું);
                u->markHostCopyObsolete(સાચું);
            }
        }
        નહીંતર જો( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<સાચું, ખોટું> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (શૂન્ય*)u->handle, (long long પૂર્ણાંક)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(ખોટું);
            u->markHostCopyObsolete(સાચું);
        }
    }

    તાર્કિક checkContinuous(પૂર્ણાંક dims, અચલ size_t sz[],
                         અચલ size_t srcofs[], અચલ size_t srcstep[],
                         અચલ size_t dstofs[], અચલ size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) અચલ
    {
        તાર્કિક iscontinuous = સાચું;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        માટે( પૂર્ણાંક i = dims-2; i >= 0; i-- )
        {
            જો( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = ખોટું;
            total *= sz[i];
            જો( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            જો( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        જો( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            જો( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                જો( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                જો( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            નહીંતર
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                જો( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                જો( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        પાછા iscontinuous;
    }

    શૂન્ય download(UMatData* u, શૂન્ય* dstptr, પૂર્ણાંક dims, અચલ size_t sz[],
                  અચલ size_t srcofs[], અચલ size_t srcstep[],
                  અચલ size_t dststep[]) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા;
        UMatDataAutoLock autolock(u);

        જો( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            પાછા;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        તાર્કિક iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ખાલી || u->data == u->handle);
            Context& ctx = Context::getDefault();
            અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (પૂર્ણાંક)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ખાલી, ખાલી);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            જો( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            નહીંતર
            {
                // This code is from MatAllocator::download()
                પૂર્ણાંક isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                માટે( પૂર્ણાંક i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    જો( sz[i] == 0 )
                    પાછા;
                    જો( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (પૂર્ણાંક)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                અચલ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                માટે( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ખાલી, ખાલી);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        નહીંતર
#endif
        {
            જો( iscontinuous )
            {
                AlignedDataPtr<ખોટું, સાચું> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            નહીંતર જો (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                અચલ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<ખોટું, ખોટું> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                માટે( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            નહીંતર
            {
                AlignedDataPtr2D<ખોટું, સાચું> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    શૂન્ય upload(UMatData* u, અચલ શૂન્ય* srcptr, પૂર્ણાંક dims, અચલ size_t sz[],
                અચલ size_t dstofs[], અચલ size_t dststep[],
                અચલ size_t srcstep[]) અચલ CV_OVERRIDE
    {
        જો(!u)
            પાછા;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        તાર્કિક iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        જો( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(ખોટું);
            u->markDeviceCopyObsolete(સાચું);
            પાછા;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == ખાલી || u->data == u->handle);
            Context& ctx = Context::getDefault();
            અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (પૂર્ણાંક)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ખાલી, ખાલી);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            જો( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            નહીંતર
            {
                // This code is from MatAllocator::upload()
                પૂર્ણાંક isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                માટે( પૂર્ણાંક i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    જો( sz[i] == 0 )
                    પાછા;
                    જો( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (પૂર્ણાંક)sz[i];
                }

                Mat src(dims, isz, CV_8U, (શૂન્ય*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                અચલ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                માટે( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ખાલી, ખાલી);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        નહીંતર
#endif
        {
            જો( iscontinuous )
            {
                AlignedDataPtr<સાચું, ખોટું> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (શૂન્ય*)u->handle, (long long પૂર્ણાંક)dstrawofs, (long long પૂર્ણાંક)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            નહીંતર જો (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                અચલ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<ખોટું, ખોટું> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                માટે( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            નહીંતર
            {
                AlignedDataPtr2D<સાચું, ખોટું> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(સાચું);
#ifdef HAVE_OPENCL_SVM
        જો ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        નહીંતર
#endif
        {
            u->markHostCopyObsolete(સાચું);
        }
        u->markDeviceCopyObsolete(ખોટું);
    }

    શૂન્ય copy(UMatData* src, UMatData* dst, પૂર્ણાંક dims, અચલ size_t sz[],
              અચલ size_t srcofs[], અચલ size_t srcstep[],
              અચલ size_t dstofs[], અચલ size_t dststep[], તાર્કિક _sync) અચલ CV_OVERRIDE
    {
        જો(!src || !dst)
            પાછા;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        તાર્કિક iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        જો( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            પાછા;
        }
        જો( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(ખોટું);
#ifdef HAVE_OPENCL_SVM
            જો ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            નહીંતર
#endif
            {
                dst->markDeviceCopyObsolete(સાચું);
            }
            પાછા;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        જો ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            જો ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                અચલ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                જો( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (પૂર્ણાંક)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, ખાલી, ખાલી);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                નહીંતર
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    પૂર્ણાંક isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        જો( sz[i] == 0 )
                        પાછા;
                        જો( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (પૂર્ણાંક)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    માટે( પૂર્ણાંક i = 0; i < dims; i++ )
                    {
                        જો( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    અચલ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    માટે( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            નહીંતર
            {
                જો ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                નહીંતર
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        નહીંતર
#endif
        {
            જો( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (શૂન્ય*)src->handle, (શૂન્ય*)dst->handle, (long long પૂર્ણાંક)srcrawofs, (long long પૂર્ણાંક)dstrawofs, (long long પૂર્ણાંક)total).c_str());
            }
            નહીંતર જો (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                અચલ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<ખોટું, ખોટું> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<ખોટું, ખોટું> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                માટે( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            નહીંતર
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        જો (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        જો ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        નહીંતર
#endif
        {
            dst->markHostCopyObsolete(સાચું);
        }
        dst->markDeviceCopyObsolete(ખોટું);

        જો( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(અચલ અક્ષર* id) અચલ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        જો (ctx.empty())
            પાછા ખાલી;
#ifdef HAVE_OPENCL_SVM
        જો ((svm::checkForceSVMUmatUsage() && (id == ખાલી || strcmp(id, "OCL") == 0)) || (id != ખાલી && strcmp(id, "SVM") == 0))
        {
            પાછા &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        જો (id != ખાલી && strcmp(id, "HOST_ALLOC") == 0)
        {
            પાછા &ctx.getImpl()->getBufferPoolHostPtr();
        }
        જો (id != ખાલી && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        પાછા &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    શૂન્ય flushCleanupQueue() અચલ
    {
        // TSAN false positive: see #27638
        જો (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            માટે (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    શૂન્ય addToCleanupQueue(UMatData* u) અચલ
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

સ્થિર OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    સ્થિર OpenCLAllocator* g_allocator = નવું OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    પાછા g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


નામાકાશ cv {

// three funcs below are implemented in umatrix.cpp
શૂન્ય setSize( UMat& m, પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sz, અચલ size_t* _steps,
              તાર્કિક autoSteps = ખોટું );
શૂન્ય finalizeHdr(UMat& m);

} // namespace cv


નામાકાશ cv { નામાકાશ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
શૂન્ય convertFromBuffer(શૂન્ય* cl_mem_buffer, size_t step, પૂર્ણાંક rows, પૂર્ણાંક cols, પૂર્ણાંક type, UMat& dst)
{
    પૂર્ણાંક d = 2;
    પૂર્ણાંક sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, સાચું);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, કદ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, કદ(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((પૂર્ણાંક)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = નવું UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    પાછા;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
શૂન્ય convertFromImage(શૂન્ય* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, કદ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, કદ(cl_image_format), &fmt, 0));

    પૂર્ણાંક depth = CV_8U;
    પસંદગી (fmt.image_channel_data_type)
    {
    સ્થિતિ CL_UNORM_INT8:
    સ્થિતિ CL_UNSIGNED_INT8:
        depth = CV_8U;
        તોડો;

    સ્થિતિ CL_SNORM_INT8:
    સ્થિતિ CL_SIGNED_INT8:
        depth = CV_8S;
        તોડો;

    સ્થિતિ CL_UNORM_INT16:
    સ્થિતિ CL_UNSIGNED_INT16:
        depth = CV_16U;
        તોડો;

    સ્થિતિ CL_SNORM_INT16:
    સ્થિતિ CL_SIGNED_INT16:
        depth = CV_16S;
        તોડો;

    સ્થિતિ CL_SIGNED_INT32:
        depth = CV_32S;
        તોડો;

    સ્થિતિ CL_FLOAT:
        depth = CV_32F;
        તોડો;

    સ્થિતિ CL_HALF_FLOAT:
        depth = CV_16F;
        તોડો;

    મૂળભૂત:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    પૂર્ણાંક type = CV_8UC1;
    પસંદગી (fmt.image_channel_order)
    {
    સ્થિતિ CL_R:
    સ્થિતિ CL_A:
    સ્થિતિ CL_INTENSITY:
    સ્થિતિ CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        તોડો;

    સ્થિતિ CL_RG:
    સ્થિતિ CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        તોડો;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    સ્થિતિ CL_RGBA:
    સ્થિતિ CL_BGRA:
    સ્થિતિ CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        તોડો;

    મૂળભૂત:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        તોડો;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, કદ(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, કદ(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, કદ(size_t), &h, 0));

    dst.create((પૂર્ણાંક)h, (પૂર્ણાંક)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, ખાલી, ખાલી));

    CV_OCL_CHECK(clFinish(q));

    પાછા;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

સ્થિર શૂન્ય getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint અંક_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ખાલી, &અંક_Devices);
    જો (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &અંક_Devices).c_str());
    }

    જો (અંક_Devices == 0)
    {
        devices.clear();
        પાછા;
    }

    devices.resize((size_t)અંક_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, અંક_Devices, &devices[0], &અંક_Devices));
}

સંરચના PlatformInfo::Impl
{
    Impl(શૂન્ય* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) અચલ
    {
        અક્ષર buf[1024];
        size_t sz=0;
        પાછા clGetPlatformInfo(handle, prop, કદ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < કદ(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    પૂર્ણાંક versionMajor_;
    પૂર્ણાંક versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(શૂન્ય* platform_id)
{
    p = નવું Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    જો(p)
        p->release();
}

PlatformInfo::PlatformInfo(અચલ PlatformInfo& i)
{
    જો (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(અચલ PlatformInfo& i)
{
    જો (i.p != p)
    {
        જો (i.p)
            i.p->addref();
        જો (p)
            p->release();
        p = i.p;
    }
    પાછા *આ;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    જો (આ != &i) {
        જો(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    પાછા *આ;
}

પૂર્ણાંક PlatformInfo::device_સંખ્યા() અચલ
{
    પાછા p ? (પૂર્ણાંક)p->devices.size() : 0;
}

શૂન્ય PlatformInfo::getDevice(Device& device, પૂર્ણાંક d) અચલ
{
    CV_Assert(p && d < (પૂર્ણાંક)p->devices.size() );
    જો(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() અચલ
{
    પાછા p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() અચલ
{
    પાછા p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() અચલ
{
    પાછા p ? p->version_ : String();
}

પૂર્ણાંક PlatformInfo::versionMajor() અચલ
{
    CV_Assert(p);
    પાછા p->versionMajor_;
}

પૂર્ણાંક PlatformInfo::versionMinor() અચલ
{
    CV_Assert(p);
    પાછા p->versionMinor_;
}

સ્થિર શૂન્ય getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint અંક_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ખાલી, &અંક_Platforms));

    જો (અંક_Platforms == 0)
    {
        platforms.clear();
        પાછા;
    }

    platforms.resize((size_t)અંક_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(અંક_Platforms, &platforms[0], &અંક_Platforms));
}

શૂન્ય getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    માટે (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((શૂન્ય*)&platforms[i]) );
}

અચલ અક્ષર* typeToStr(પૂર્ણાંક type)
{
    સ્થિર અચલ અક્ષર* tab[CV_DEPTH_MAX*16]=
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
    પૂર્ણાંક cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    અચલ અક્ષર* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    પાછા result;
}

અચલ અક્ષર* memopTypeToStr(પૂર્ણાંક type)
{
    સ્થિર અચલ અક્ષર* tab[CV_DEPTH_MAX*16] =
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
    પૂર્ણાંક cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    અચલ અક્ષર* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    પાછા result;
}

અચલ અક્ષર* vecopTypeToStr(પૂર્ણાંક type)
{
    સ્થિર અચલ અક્ષર* tab[CV_DEPTH_MAX*16] =
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
    પૂર્ણાંક cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    અચલ અક્ષર* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    પાછા result;
}

// Deprecated due to size of buf buffer being unknowable.
અચલ અક્ષર* convertTypeStr(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth, પૂર્ણાંક cn, અક્ષર* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    પાછા convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

અચલ અક્ષર* convertTypeStr(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth, પૂર્ણાંક cn, અક્ષર* buf, size_t buf_size)
{
    જો( sdepth == ddepth )
        પાછા "noconvert";
    અચલ અક્ષર *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    જો( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    નહીંતર જો( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    નહીંતર
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    પાછા buf;
}

અચલ અક્ષર* getOpenCLErrorString(પૂર્ણાંક errorCode)
{
#define CV_OCL_CODE(id) સ્થિતિ id: પાછા #id
#define CV_OCL_CODE_(id, name) સ્થિતિ id: પાછા #name
    પસંદગી (errorCode)
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
    CV_OCL_CODE(CL_INVALID_કિંમત);
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
    CV_OCL_CODE(CL_INVALID_દ્વિઅંકી);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_કિંમત);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_ગણતરી);
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
    મૂળભૂત: પાછા "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
સ્થિર std::string kerToStr(અચલ Mat & k)
{
    પૂર્ણાંક width = k.cols - 1, depth = k.depth();
    અચલ T * અચલ data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    જો (depth <= CV_8S)
    {
        માટે (પૂર્ણાંક i = 0; i < width; ++i)
            stream << "DIG(" << (પૂર્ણાંક)data[i] << ")";
        stream << "DIG(" << (પૂર્ણાંક)data[width] << ")";
    }
    નહીંતર જો (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        માટે (પૂર્ણાંક i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    નહીંતર જો (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        માટે (પૂર્ણાંક i = 0; i < width; ++i)
            stream << "DIG(" << (દશાંશ)data[i] << "h)";
        stream << "DIG(" << (દશાંશ)data[width] << "h)";
    }
    નહીંતર
    {
        માટે (પૂર્ણાંક i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    પાછા stream.str();
}

String kernelToStr(આગત_Array _kernel, પૂર્ણાંક ddepth, અચલ અક્ષર * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    પૂર્ણાંક depth = kernel.depth();
    જો (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    જો (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    જો (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    પ્રકારવ્યાખ્યા std::string (* func_t)(અચલ Mat &);
    સ્થિર અચલ func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<અક્ષર>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<પૂર્ણાંક>, kerToStr<દશાંશ>, kerToStr<દ્વિગુણ>, kerToStr<hfloat>,
                                                0, kerToStr<તાર્કિક>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    અચલ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    પાછા cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    કરો \
    { \
        જો (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            પૂર્ણાંક ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            જો (cwidth < ckercn || ckercn <= 0) \
                પાછા 1; \
            cols.push_back(cwidth); \
            જો (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                પાછા 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    જ્યારે ((શૂન્ય)0, 0)

પૂર્ણાંક predictOptimalVectorWidth(આગત_Array src1, આગત_Array src2, આગત_Array src3,
                              આગત_Array src4, આગત_Array src5, આગત_Array src6,
                              આગત_Array src7, આગત_Array src8, આગત_Array src9,
                              OclVectorStrategy strat)
{
    અચલ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    પૂર્ણાંક vectorWidths[CV_DEPTH_MAX] = {
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
    જો (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    પાછા checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

પૂર્ણાંક checkOptimalVectorWidth([[maybe_unused]] અચલ પૂર્ણાંક *vectorWidths,
                            [[maybe_unused]] આગત_Array src1,
                            [[maybe_unused]] આગત_Array src2,
                            [[maybe_unused]] આગત_Array src3,
                            [[maybe_unused]] આગત_Array src4,
                            [[maybe_unused]] આગત_Array src5,
                            [[maybe_unused]] આગત_Array src6,
                            [[maybe_unused]] આગત_Array src7,
                            [[maybe_unused]] આગત_Array src8,
                            [[maybe_unused]] આગત_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    પાછા 1;
#નહીંતર
    CV_Assert(vectorWidths);

    પૂર્ણાંક ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<પૂર્ણાંક> dividers, kercns;
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

    માટે (size_t i = 0; i < size; ++i)
        જ્યારે (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    પૂર્ણાંક kercn = *std::min_element(kercns.begin(), kercns.end());

    પાછા kercn;
#endif
}

પૂર્ણાંક predictOptimalVectorWidthMax(આગત_Array src1, આગત_Array src2, આગત_Array src3,
                                 આગત_Array src4, આગત_Array src5, આગત_Array src6,
                                 આગત_Array src7, આગત_Array src8, આગત_Array src9)
{
    પાછા predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
શૂન્ય buildOptionsAddMatrixDescription(String& buildOptions, અચલ String& name, આગત_Array _m)
{
    જો (!buildOptions.empty())
        buildOptions += " ";
    પૂર્ણાંક type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (પૂર્ણાંક)CV_MAT_CN(type),
            name.c_str(), (પૂર્ણાંક)CV_ELEM_SIZE(type),
            name.c_str(), (પૂર્ણાંક)CV_ELEM_SIZE1(type),
            name.c_str(), (પૂર્ણાંક)depth
            );
}


સંરચના Image2D::Impl
{
    Impl(અચલ UMat &src, તાર્કિક norm, તાર્કિક alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        જો (handle)
            clReleaseMemObject(handle);
    }

    સ્થિર cl_image_format getImageFormat(પૂર્ણાંક depth, પૂર્ણાંક cn, તાર્કિક norm)
    {
        cl_image_format format;
        સ્થિર અચલ પૂર્ણાંક channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        સ્થિર અચલ પૂર્ણાંક channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        સ્થિર અચલ પૂર્ણાંક channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        પૂર્ણાંક channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        પૂર્ણાંક channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        પાછા format;
    }

    સ્થિર તાર્કિક isFormatSupported(cl_image_format format)
    {
        જો (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        જો (!context)
            પાછા ખોટું;

        // Figure out how many formats are supported by this context.
        cl_uint અંક_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, અંક_Formats,
                                                ખાલી, &અંક_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        જો (અંક_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(અંક_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, અંક_Formats,
                                             formats.data(), ખાલી);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            માટે (cl_uint i = 0; i < અંક_Formats; ++i)
            {
                જો (!memcmp(&formats[i], &format, કદ(format)))
                {
                    પાછા સાચું;
                }
            }
        }
        પાછા ખોટું;
    }

    શૂન્ય init(અચલ UMat &src, તાર્કિક norm, તાર્કિક alias)
    {
        જો (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        પૂર્ણાંક err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        જો (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        જો (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        અચલ Device & d = ocl::Device::getDefault();
        પૂર્ણાંક minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        જો (1 < major || (1 == major && 2 <= minor))
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
            desc.અંક_mip_levels   = 0;
            desc.અંક_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ખાલી, &err);
        }
        નહીંતર
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, ખાલી, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        જો (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), ખાલી, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long પૂર્ણાંક)(src.cols * src.rows * src.elemSize()), (શૂન્ય*)devData
                ).c_str());

            અચલ size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, ખાલી, ખાલી));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        નહીંતર
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != ખાલી);

        જો (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, ખાલી, 0));
            જો (!src.isContinuous())
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
    p = ખાલી;
}

Image2D::Image2D(અચલ UMat &src, તાર્કિક norm, તાર્કિક alias)
{
    p = નવું Impl(src, norm, alias);
}

તાર્કિક Image2D::canCreateAlias(અચલ UMat &m)
{
    તાર્કિક ret = ખોટું;
    અચલ Device & d = ocl::Device::getDefault();
    જો (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        જો (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            જો (!m.u->tempUMat())
            {
                ret = સાચું;
            }
        }
    }
    પાછા ret;
}

તાર્કિક Image2D::isFormatSupported(પૂર્ણાંક depth, પૂર્ણાંક cn, તાર્કિક norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    પાછા Impl::isFormatSupported(format);
}

Image2D::Image2D(અચલ Image2D & i)
{
    p = i.p;
    જો (p)
        p->addref();
}

Image2D & Image2D::operator = (અચલ Image2D & i)
{
    જો (i.p != p)
    {
        જો (i.p)
            i.p->addref();
        જો (p)
            p->release();
        p = i.p;
    }
    પાછા *આ;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    જો (આ != &i) {
        જો (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    પાછા *આ;
}

Image2D::~Image2D()
{
    જો (p)
        p->release();
}

શૂન્ય* Image2D::ptr() અચલ
{
    પાછા p ? p->handle : 0;
}

તાર્કિક internal::isOpenCLForced()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક value = ખોટું;
    જો (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", ખોટું);
        initialized = સાચું;
    }
    પાછા value;
}

તાર્કિક internal::isPerformanceCheckBypassed()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર તાર્કિક value = ખોટું;
    જો (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", ખોટું);
        initialized = સાચું;
    }
    પાછા value;
}

તાર્કિક internal::isCLBuffer(UMat& u)
{
    જો (isCudaBacked(u.u))
        પાછા ખોટું;
    શૂન્ય* h = u.handle(ACCESS_RW);
    જો (!h)
        પાછા સાચું;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#જો 1
    જો ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        પાછા ખોટું;
#નહીંતર
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, કદ(type), &type, ખાલી);
    જો (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        પાછા ખોટું;
#endif
    પાછા સાચું;
}

સંરચના Timer::Impl
{
    અચલ Queue queue;

    Impl(અચલ Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    શૂન્ય start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    શૂન્ય stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() અચલ
    {
        પાછા (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(અચલ Queue& q) : p(નવું Impl(q)) { }
Timer::~Timer() { delete p; }

શૂન્ય Timer::start()
{
    CV_Assert(p);
    p->start();
}

શૂન્ય Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() અચલ
{
    CV_Assert(p);
    પાછા p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
