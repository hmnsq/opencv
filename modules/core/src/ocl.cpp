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

#شامل "precomp.hpp"

#ifndef HAVE_OPENCL
#شامل "ocl_disabled.impl.hpp"
#وگرنه // HAVE_OPENCL

#شامل <list>
#شامل <map>
#شامل <deque>
#شامل <set>
#شامل <string>
#شامل <sstream>
#شامل <fstream>
#اگر !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#شامل <inttypes.h>
#endif

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#شامل <opencv2/core/utils/logger.hpp>

#شامل "opencv2/core/ocl_genbase.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "opencv2/core/utils/lock.private.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"
#شامل "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(پوچ, 0, "OpenCL allocator: " << __VA_ARGS__)
#شامل "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_دودویی_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#شامل "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# اگر 0
#   define LOG_BUFFER_POOL printf
# وگرنه
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#اگر CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#وگرنه
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#اگر CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#وگرنه
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#شامل "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#شامل "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#شامل "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#شامل "opencv2/core/opencl/opencl_svm.hpp"
#endif

#شامل "umatrix.hpp"

فضاینام cv { فضاینام ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    تهی addref() { CV_XADD(&refcount, 1); } \
    تهی release() { اگر( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete این; } \
    صحیح refcount

ایستا cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    بازگشت opencl_allocator_stats;
}

#ifndef _DEBUG
ایستا منطقی isRaiseError()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی value = نادرست;
    اگر (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", نادرست);
        initialized = درست;
    }
    بازگشت value;
}
#endif

ایستا تهی onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", نادرست);
    اگر (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#اگر CV_OPENCL_TRACE_CHECK
ایستا درونخطی
تهی traceOpenCLCheck(cl_int status, ثابت نویسه* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#وگرنه
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_پیام(check_result, پیام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, پیام)

#define CV_OCL_CHECK_RESULT(check_result, پیام) \
    انجامبده { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, پیام); \
        اگر (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پیام), ثابت نویسه*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پیام = CV_OCL_API_ERROR_پیام(check_result, پیام); \
            CV_Error(Error::OpenCLApiCallError, error_پیام); \
        } \
    } وقتی (0)

#define CV_OCL_CHECK_(عبارت, check_result) انجامبده { عبارت; CV_OCL_CHECK_RESULT(check_result, #expr); } وقتی (0)

#define CV_OCL_CHECK(عبارت) انجامبده { cl_int __cl_result = (عبارت); CV_OCL_CHECK_RESULT(__cl_result, #expr); } وقتی (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, پیام) CV_OCL_CHECK_RESULT(check_result, پیام)
#define CV_OCL_DBG_CHECK(عبارت) CV_OCL_CHECK(عبارت)
#define CV_OCL_DBG_CHECK_(عبارت, check_result) CV_OCL_CHECK_(عبارت, check_result)
#وگرنه
#define CV_OCL_DBG_CHECK_RESULT(check_result, پیام) \
    انجامبده { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, پیام); \
        اگر (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پیام), ثابت نویسه*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پیام = CV_OCL_API_ERROR_پیام(check_result, پیام); \
            CV_Error(Error::OpenCLApiCallError, error_پیام); \
        } \
    } وقتی (0)
#define CV_OCL_DBG_CHECK_(عبارت, check_result) انجامبده { عبارت; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } وقتی (0)
#define CV_OCL_DBG_CHECK(عبارت) انجامبده { cl_int __cl_result = (عبارت); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } وقتی (0)
#endif


ایستا ثابت منطقی CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", درست);
ایستا ثابت منطقی CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", درست);
ایستا ثابت منطقی CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", درست);
ایستا ثابت منطقی CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", درست);

#اگر CV_OPENCL_VALIDATE_دودویی_PROGRAMS
ایستا ثابت منطقی CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", نادرست);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ایستا ثابت منطقی CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#اگر 1 //def __APPLE__
        درست
#وگرنه
        نادرست
#endif
);

ایستا String getBuildExtraOptions()
{
    ایستا String param_buildExtraOptions;
    ایستا منطقی initialized = نادرست;
    اگر (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = درست;
        اگر (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(پوچ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    بازگشت param_buildExtraOptions;
}

ایستا ثابت منطقی CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", درست);
ایستا ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ساختار UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحیح)m.offset;
        step = (صحیح)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    صحیح offset;
    صحیح step;
    صحیح rows;
    صحیح cols;
};

ساختار UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحیح)m.offset;
        step = (صحیح)m.step.p[1];
        slicestep = (صحیح)m.step.p[0];
        slices = (صحیح)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    صحیح offset;
    صحیح slicestep;
    صحیح step;
    صحیح slices;
    صحیح rows;
    صحیح cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ایستا uint64 crc64( ثابت uchar* data, size_t size, uint64 crc0=0 )
{
    ایستا uint64 table[256];
    ایستا منطقی initialized = نادرست;

    اگر( !initialized )
    {
        برای( صحیح i = 0; i < 256; i++ )
        {
            uint64 c = i;
            برای( صحیح j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = درست;
    }

    uint64 crc = ~crc0;
    برای( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    بازگشت ~crc;
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ساختار OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    تعریفنوع std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(پوچ, "Initializing OpenCL cache configuration...");
        اگر (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(پوچ, "OpenCL cache is disabled");
            بازگشت;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        اگر (cache_path_.empty())
        {
            CV_LOG_INFO(پوچ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        انجامبده
        {
            تلاش
            {
                اگر (cache_path_.empty())
                    بشکن;
                اگر (cache_path_ == "disabled")
                    بشکن;
                اگر (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(پوچ, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    بشکن;
                }

                اگر (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    اگر (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(پوچ, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        اگر (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(پوچ, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            بشکن;
                        }
                    }

                    تلاش
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(پوچ, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(پوچ, 0, "Checking cache lock... Done!");
                    }
                    بگیر (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(پوچ, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    بگیر (...)
                    {
                        CV_LOG_WARNING(پوچ, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                وگرنه
                {
                    اگر (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(پوچ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    وگرنه
                    {
                        CV_LOG_INFO(پوچ, "OpenCL cache lock is disabled");
                    }
                }
            }
            بگیر (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(پوچ, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } وقتی (0);
        اگر (!cache_path_.empty())
        {
            اگر (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(پوچ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وگرنه
            {
                CV_LOG_INFO(پوچ, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    تهی clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_prefix,
            ثابت std::string& cleanup_prefix)
    {
        اگر (cache_path_.empty())
            بازگشت std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        اگر (found_it != prepared_contexts_.end())
            بازگشت found_it->second;

        CV_LOG_INFO(پوچ, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        منطقی result = utils::fs::isDirectory(target_directory);
        اگر (!result)
        {
            تلاش
            {
                CV_LOG_VERBOSE(پوچ, 0, "Creating directory: " << target_directory);
                اگر (utils::fs::createDirectories(target_directory))
                {
                    result = درست;
                }
                وگرنه
                {
                    CV_LOG_WARNING(پوچ, "Can't create directory: " << target_directory);
                }
            }
            بگیر (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(پوچ, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        اگر (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            تلاش
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, نادرست, درست);
                std::vector<String> remove_entries;
                برای (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    اگر (0 == name.find(cleanup_prefix))
                    {
                        اگر (0 == name.find(ctx_prefix))
                            ادامه; // skip current
                        remove_entries.push_back(name);
                    }
                }
                اگر (!remove_entries.empty())
                {
                    CV_LOG_WARNING(پوچ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(پوچ, "Trying to remove these directories...");
                    برای (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(پوچ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(پوچ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    برای (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ثابت String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        تلاش
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(پوچ, "Removed: " << path);
                        }
                        بگیر (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(پوچ, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            بگیر (...)
            {
                CV_LOG_WARNING(پوچ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(پوچ, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        بازگشت target_directory;
    }

    ایستا OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, جدید OpenCLBinaryCacheConfigurator());
    }
};
رده دودویی_Program_File
{
    شمارش { MAX_ENTRIES = 64 };

    تعریفنوع unsigned صحیح uint32_t;

    ساختار CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    ساختار CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t عدد_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    ساختار CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t کلید_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت نویسه* ثابت sourceSignature_;
    ثابت size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        بازگشت hash & (MAX_ENTRIES - 1);
    }

    درونخطی size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        بازگشت fileSize;
    }
    درونخطی uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.اقرأ((نویسه*)&res, اندازه(uint32_t));
        CV_Assert(!f.fail());
        بازگشت res;
    }
    درونخطی تهی writeUInt32(ثابت uint32_t value)
    {
        uint32_t v = value;
        f.اكتب((نویسه*)&v, اندازه(uint32_t));
        CV_Assert(!f.fail());
    }

    درونخطی تهی seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    درونخطی تهی seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    درونخطی تهی seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    تهی clearFile()
    {
        f.close();
        اگر (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(پوچ, "Can't remove: " << fileName_);
        بازگشت;
    }

عمومی:
    دودویی_Program_File(ثابت std::string& fileName, ثابت نویسه* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(اندازه(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != پوچ);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, اندازه(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        اگر(f.is_open() && getFileSize() > 0)
        {
            منطقی isValid = نادرست;
            تلاش
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                اگر (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<نویسه> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.اقرأ(fileSourceSignature.data(), fileSourceSignatureSize);
                    اگر (f.eof())
                    {
                        CV_LOG_ERROR(پوچ, "Unexpected EOF");
                    }
                    وگرنه اگر (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = درست;
                    }
                }
                اگر (!isValid)
                {
                    CV_LOG_ERROR(پوچ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            بگیر (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(پوچ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            بگیر (...)
            {
                CV_LOG_ERROR(پوچ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            اگر (!isValid)
            {
                clearFile();
            }
            وگرنه
            {
                seekReadAbsolute(0);
            }
        }
    }

    منطقی اقرأ(ثابت std::string& کلید, std::vector<نویسه>& buf)
    {
        اگر (!f.is_open())
            بازگشت نادرست;

        size_t fileSize = getFileSize();
        اگر (fileSize == 0)
        {
            CV_LOG_ERROR(پوچ, "Invalid file (empty): " << fileName_);
            clearFile();
            بازگشت نادرست;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t عدد_Of_Entries = readUInt32();
        CV_Assert(عدد_Of_Entries > 0);
        اگر (عدد_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(پوچ, "Invalid file: " << fileName_);
            clearFile();
            بازگشت نادرست;
        }
        f.اقرأ((نویسه*)&entryOffsets[0], اندازه(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_رقم = getHash(کلید);

        uint32_t entryOffset = entryOffsets[entry_رقم];
        FileEntry entry;
        وقتی (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((نویسه*)&entry, اندازه(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<نویسه> file_کلید(entry.کلید_Size + 1);
            اگر (کلید.size() == entry.کلید_Size)
            {
                اگر (entry.کلید_Size > 0)
                {
                    f.اقرأ(file_کلید.data(), entry.کلید_Size);
                    CV_Assert(!f.fail());
                }
                اگر (memcmp(file_کلید.data(), کلید.c_str(), entry.کلید_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.اقرأ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(پوچ, 0, "Read...");
                    بازگشت درست;
                }
            }
            اگر (entry.nextEntryFileOffset == 0)
                بشکن;
            entryOffset = entry.nextEntryFileOffset;
        }
        بازگشت نادرست;
    }

    منطقی اكتب(ثابت std::string& کلید, std::vector<نویسه>& buf)
    {
        اگر (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            اگر (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                اگر (!f.is_open())
                {
                    CV_LOG_ERROR(پوچ, "Can't create file: " << fileName_);
                    بازگشت نادرست;
                }
            }
        }

        size_t fileSize = getFileSize();
        اگر (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.اكتب(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, اندازه(entryOffsets));
            f.اكتب((نویسه*)entryOffsets, اندازه(entryOffsets));
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

        uint32_t عدد_Of_Entries = readUInt32();
        CV_Assert(عدد_Of_Entries > 0);
        اگر (عدد_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(پوچ, "Invalid file: " << fileName_);
            clearFile();
            بازگشت نادرست;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.اقرأ((نویسه*)&entryOffsets[0], اندازه(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_رقم = getHash(کلید);

        uint32_t entryOffset = entryOffsets[entry_رقم];
        FileEntry entry;
        وقتی (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((نویسه*)&entry, اندازه(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<نویسه> file_کلید(entry.کلید_Size + 1);
            اگر (کلید.size() == entry.کلید_Size)
            {
                اگر (entry.کلید_Size > 0)
                {
                    f.اقرأ(file_کلید.data(), entry.کلید_Size);
                    CV_Assert(!f.fail());
                }
                اگر (0 == memcmp(file_کلید.data(), کلید.c_str(), entry.کلید_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(پوچ, 0, "Duplicate key ignored: " << fileName_);
                    بازگشت نادرست;
                }
            }
            اگر (entry.nextEntryFileOffset == 0)
                بشکن;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        اگر (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.اكتب((نویسه*)&entry, اندازه(entry));
            CV_Assert(!f.fail());
        }
        وگرنه
        {
            entryOffsets[entry_رقم] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.اكتب((نویسه*)entryOffsets, اندازه(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.کلید_Size = (uint32_t)کلید.size();
        f.اكتب((نویسه*)&entry, اندازه(entry));
        CV_Assert(!f.fail());
        f.اكتب(کلید.c_str(), entry.کلید_Size);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(پوچ, 0, "Write... (" << buf.size() << " bytes)");
        بازگشت درست;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



ساختار OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحیح device_;  // device index in context
    ocl::Queue queue_;
    صحیح useOpenCL_;

محافظتشده:
    Impl() = delete;

    تهی _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحیح ndevices = (صحیح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقی found = نادرست;
        برای (صحیح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            اگر (dhandle == deviceID)
            {
                device_ = i;
                found = درست;
                بشکن;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    تهی _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحیح ndevices = (صحیح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقی found = نادرست;
        برای (صحیح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            اگر (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = درست;
                بشکن;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

عمومی:
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

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device, ثابت ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت صحیح device, ثابت ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ثابت Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    درونخطی منطقی useOpenCL() ثابت { بازگشت const_cast<Impl*>(این)->useOpenCL(); }
    منطقی useOpenCL()
    {
        اگر (useOpenCL_ < 0)
        {
            تلاش
            {
                useOpenCL_ = 0;
                اگر (!context_.empty() && context_.ndevices() > 0)
                {
                    ثابت Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            بگیر (ثابت cv::Exception&)
            {
                // nothing
            }
            اگر (!useOpenCL_)
                CV_LOG_INFO(پوچ, "OpenCL: can't use OpenCL execution context");
        }
        بازگشت useOpenCL_ > 0;
    }

    تهی setUseOpenCL(منطقی flag)
    {
        اگر (!flag)
            useOpenCL_ = 0;
        وگرنه
            useOpenCL_ = -1;
    }

    ایستا ثابت std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(پوچ, "OpenCL: initializing thread execution context");

        ایستا منطقی initialized = نادرست;
        ایستا std::shared_ptr<Impl> g_primaryExecutionContext;

        اگر (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            اگر (!initialized)
            {
                CV_LOG_INFO(پوچ, "OpenCL: creating new execution context...");
                تلاش
                {
                    Context c = ocl::Context::create(std::string());
                    اگر (c.ndevices())
                    {
                        صحیح deviceId = 0;
                        auto& d = c.device(deviceId);
                        اگر (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            اگر (!q.ptr())
                            {
                                CV_LOG_ERROR(پوچ, "OpenCL: Can't create default OpenCL queue");
                            }
                            وگرنه
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(پوچ, "OpenCL: device=" << d.name());
                            }
                        }
                        وگرنه
                        {
                            CV_LOG_ERROR(پوچ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    وگرنه
                    {
                        CV_LOG_INFO(پوچ, "OpenCL: context is not available/disabled");
                    }
                }
                بگیر (ثابت std::exception& e)
                {
                    CV_LOG_INFO(پوچ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                بگیر (...)
                {
                    CV_LOG_WARNING(پوچ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = درست;
            }
        }
        بازگشت g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ثابت
{
    CV_Assert(p);
    بازگشت p->context_;
}
Device& OpenCLExecutionContext::getDevice() ثابت
{
    CV_Assert(p);
    بازگشت p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ثابت
{
    CV_Assert(p);
    بازگشت p->queue_;
}

منطقی OpenCLExecutionContext::useOpenCL() ثابت
{
    اگر (p)
        بازگشت p->useOpenCL();
    بازگشت نادرست;
}
تهی OpenCLExecutionContext::setUseOpenCL(منطقی flag)
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
    اگر (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = درست;
        اگر (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    بازگشت c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    بازگشت c;
}

تهی OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *این;
    data.oclExecutionContextInitialized = درست;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت Queue q(getContext(), getDevice());
    بازگشت cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ثابت ocl::Queue& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != پوچ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    بازگشت c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    اگر (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    بازگشت ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device)
{
    CV_TRACE_FUNCTION();
    اگر (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    بازگشت ctx;

}

تهی OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ایستا منطقی g_isOpenCLInitialized = نادرست;
ایستا منطقی g_isOpenCLAvailable = نادرست;

منطقی haveOpenCL()
{
    CV_TRACE_FUNCTION();

    اگر (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        اگر (!envPath.empty())
        {
            اگر (envPath == "disabled")
            {
                g_isOpenCLAvailable = نادرست;
                g_isOpenCLInitialized = درست;
                بازگشت نادرست;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(پوچ, "Initialize OpenCL runtime...");
        تلاش
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, پوچ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(پوچ, "OpenCL: found " << n << " platforms");
        }
        بگیر (...)
        {
            g_isOpenCLAvailable = نادرست;
        }
        g_isOpenCLInitialized = درست;
    }
    بازگشت g_isOpenCLAvailable;
}

منطقی useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    اگر (data.useOpenCL < 0)
    {
        تلاش
        {
            data.useOpenCL = 0;
            اگر (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        بگیر (...)
        {
            CV_LOG_INFO(پوچ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    بازگشت data.useOpenCL > 0;
}

منطقی isOpenCLActivated()
{
    اگر (!g_isOpenCLAvailable)
        بازگشت نادرست; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    بازگشت useOpenCL();
}

تهی setUseOpenCL(منطقی flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    اگر (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    وگرنه
    {
        اگر (!flag)
            data.useOpenCL = 0;
        وگرنه
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

رده AmdBlasHelper
{
عمومی:
    ایستا AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, جدید AmdBlasHelper())
    }

    منطقی isAvailable() ثابت
    {
        بازگشت g_isAmdBlasAvailable;
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

محافظتشده:
    AmdBlasHelper()
    {
        اگر (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            اگر (!g_isAmdBlasInitialized)
            {
                اگر (haveOpenCL())
                {
                    تلاش
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    بگیر (...)
                    {
                        g_isAmdBlasAvailable = نادرست;
                    }
                }
                وگرنه
                    g_isAmdBlasAvailable = نادرست;

                g_isAmdBlasInitialized = درست;
            }
        }
    }

خصوصی:
    ایستا منطقی g_isAmdBlasInitialized;
    ایستا منطقی g_isAmdBlasAvailable;
};

منطقی AmdBlasHelper::g_isAmdBlasAvailable = نادرست;
منطقی AmdBlasHelper::g_isAmdBlasInitialized = نادرست;

منطقی haveAmdBlas()
{
    بازگشت AmdBlasHelper::getInstance().isAvailable();
}

#وگرنه

منطقی haveAmdBlas()
{
    بازگشت نادرست;
}

#endif

#ifdef HAVE_CLAMDFFT

رده AmdFftHelper
{
عمومی:
    ایستا AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, جدید AmdFftHelper())
    }

    منطقی isAvailable() ثابت
    {
        بازگشت g_isAmdFftAvailable;
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

محافظتشده:
    AmdFftHelper()
    {
        اگر (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            اگر (!g_isAmdFftInitialized)
            {
                اگر (haveOpenCL())
                {
                    تلاش
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = درست;
                    }
                    بگیر (ثابت Exception &)
                    {
                        g_isAmdFftAvailable = نادرست;
                    }
                }
                وگرنه
                    g_isAmdFftAvailable = نادرست;

                g_isAmdFftInitialized = درست;
            }
        }
    }

خصوصی:
    ایستا clfftSetupData setupData;
    ایستا منطقی g_isAmdFftInitialized;
    ایستا منطقی g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
منطقی AmdFftHelper::g_isAmdFftAvailable = نادرست;
منطقی AmdFftHelper::g_isAmdFftInitialized = نادرست;

منطقی haveAmdFft()
{
    بازگشت AmdFftHelper::getInstance().isAvailable();
}

#وگرنه

منطقی haveAmdFft()
{
    بازگشت نادرست;
}

#endif

منطقی haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    بازگشت درست;
#وگرنه
    بازگشت نادرست;
#endif
}

تهی finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ساختار Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = نادرست;
    }

    ~Impl() {}

    تهی init()
    {
        اگر( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            اگر( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            اگر( handle != 0 )
            {
                نویسه buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, اندازه(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = درست;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    منطقی initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    اگر(p)
        p->release();
}

Platform::Platform(ثابت Platform& pl)
{
    p = (Impl*)pl.p;
    اگر(p)
        p->addref();
}

Platform& Platform::operator = (ثابت Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    اگر (این != &pl) {
        اگر(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    بازگشت *این;
}

تهی* Platform::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(پوچ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ایستا Platform p;
    اگر( !p.p )
    {
        p.p = جدید Impl;
        p.p->init();
    }
    بازگشت p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ایستا تهی parseOpenCLVersion(ثابت String &version, صحیح &major, صحیح &minor)
{
    major = minor = 0;
    اگر (10 >= version.length())
        بازگشت;
    ثابت نویسه *pstr = version.c_str();
    اگر (0 != strncmp(pstr, "OpenCL ", 7))
        بازگشت;
    size_t ppos = version.find('.', 7);
    اگر (String::npos == ppos)
        بازگشت;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

ساختار Device::Impl
{
    Impl(تهی* d)
        : refcount(1)
        , handle(0)
    {
        تلاش
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        بگیر (...)
        {
            پرتابکن;
        }
    }

    تهی _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحیح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحیح>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, صحیح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, صحیح>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, صحیح>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        وقتی (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            اگر (pos2 == String::npos)
                pos2 = extensions_.size();
            اگر (pos2 > pos)
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
        اگر (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        وگرنه اگر (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وگرنه اگر (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وگرنه
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        اگر (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ثابت size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            اگر (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(پوچ, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#اگر 0
        اگر (isExtensionSupported("cl_khr_spir"))
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
        اگر (!cv::__termination)
#endif
        {
            اگر (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) ثابت
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        بازگشت clGetDeviceInfo(handle, prop, اندازه(temp), &temp, &sz) == CL_SUCCESS &&
            sz == اندازه(temp) ? _TpOut(temp) : _TpOut();
    }

    منطقی getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        بازگشت clGetDeviceInfo(handle, prop, اندازه(temp), &temp, &sz) == CL_SUCCESS &&
            sz == اندازه(temp) ? temp != 0 : نادرست;
    }

    String getStrProp(cl_device_info prop) ثابت
    {
        نویسه buf[4096];
        size_t sz=0;
        بازگشت clGetDeviceInfo(handle, prop, اندازه(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < اندازه(buf) ? String(buf) : String();
    }

    منطقی isExtensionSupported(ثابت std::string& extensionName) ثابت
    {
        بازگشت extensions_set_.شمار_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    صحیح doubleFPConfig_;
    منطقی khr_fp64_support_;
    صحیح halfFPConfig_;
    منطقی khr_fp16_support_;
    منطقی hostUnifiedMemory_;
    صحیح maxComputeUnits_;
    size_t maxWorkGroupSize_;
    صحیح type_;
    صحیح addressBits_;
    صحیح deviceVersionMajor_;
    صحیح deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    صحیح vendorID_;
    منطقی intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(تهی* d)
{
    p = 0;
    set(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    اگر(p)
        p->addref();
}

Device& Device::operator = (ثابت Device& d)
{
    Impl* newp = (Impl*)d.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    اگر (این != &d) {
        اگر(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    بازگشت *این;
}

Device::~Device()
{
    اگر(p)
        p->release();
}

تهی Device::set(تهی* d)
{
    اگر(p)
        p->release();
    p = جدید Impl(d);
    اگر (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(تهی* d)
{
    Device device(d);
    بازگشت device;
}

تهی* Device::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

String Device::name() ثابت
{ بازگشت p ? p->name_ : String(); }

String Device::extensions() ثابت
{ بازگشت p ? String(p->extensions_) : String(); }

منطقی Device::isExtensionSupported(ثابت String& extensionName) ثابت
{ بازگشت p ? p->isExtensionSupported(extensionName) : نادرست; }

String Device::version() ثابت
{ بازگشت p ? p->version_ : String(); }

String Device::vendorName() ثابت
{ بازگشت p ? p->vendorName_ : String(); }

صحیح Device::vendorID() ثابت
{ بازگشت p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ثابت
{ بازگشت p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ثابت
{ بازگشت p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

صحیح Device::deviceVersionMajor() ثابت
{ بازگشت p ? p->deviceVersionMajor_ : 0; }

صحیح Device::deviceVersionMinor() ثابت
{ بازگشت p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ثابت
{ بازگشت p ? p->driverVersion_ : String(); }

صحیح Device::type() ثابت
{ بازگشت p ? p->type_ : 0; }

صحیح Device::addressBits() ثابت
{ بازگشت p ? p->addressBits_ : 0; }

منطقی Device::available() ثابت
{ بازگشت p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : نادرست; }

منطقی Device::compilerAvailable() ثابت
{ بازگشت p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : نادرست; }

منطقی Device::linkerAvailable() ثابت
#ifdef CL_VERSION_1_2
{ بازگشت p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : نادرست; }
#وگرنه
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

صحیح Device::doubleFPConfig() ثابت
{ بازگشت p ? p->doubleFPConfig_ : 0; }

صحیح Device::singleFPConfig() ثابت
{ بازگشت p ? p->getProp<cl_device_fp_config, صحیح>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

صحیح Device::halfFPConfig() ثابت
{ بازگشت p ? p->halfFPConfig_ : 0; }

منطقی Device::hasFP64() ثابت
{ بازگشت p ? p->khr_fp64_support_ : نادرست; }
منطقی Device::hasFP16() ثابت
{ بازگشت p ? p->khr_fp16_support_ : نادرست; }

منطقی Device::endianLittle() ثابت
{ بازگشت p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : نادرست; }

منطقی Device::errorCorrectionSupport() ثابت
{ بازگشت p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : نادرست; }

صحیح Device::executionCapabilities() ثابت
{ بازگشت p ? p->getProp<cl_device_exec_capabilities, صحیح>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

صحیح Device::globalMemCacheType() ثابت
{ بازگشت p ? p->getProp<cl_device_mem_cache_type, صحیح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحیح Device::global_Mem_Cache_سطر_Size() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

صحیح Device::localMemType() ثابت
{ بازگشت p ? p->getProp<cl_device_local_mem_type, صحیح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقی Device::hostUnifiedMemory() ثابت
{ بازگشت p ? p->hostUnifiedMemory_ : نادرست; }

منطقی Device::imageSupport() ثابت
{ بازگشت p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : نادرست; }

منطقی Device::imageFromBufferSupport() ثابت
{
    بازگشت p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : نادرست;
}

uint Device::imagePitchAlignment() ثابت
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    بازگشت p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وگرنه
    بازگشت 0;
#endif
}

uint Device::imageBaseAddressAlignment() ثابت
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    بازگشت p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#وگرنه
    بازگشت 0;
#endif
}

size_t Device::image2DMaxWidth() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ثابت
#ifdef CL_VERSION_1_2
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#وگرنه
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ثابت
#ifdef CL_VERSION_1_2
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#وگرنه
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

منطقی Device::intelSubgroupsSupport() ثابت
{ بازگشت p ? p->intelSubgroupsSupport_ : نادرست; }

صحیح Device::maxClockFrequency() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحیح Device::maxComputeUnits() ثابت
{ بازگشت p ? p->maxComputeUnits_ : 0; }

صحیح Device::maxConstantArgs() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ثابت
{ بازگشت p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

صحیح Device::maxReadImageArgs() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

صحیح Device::maxWriteImageArgs() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

صحیح Device::maxSamplers() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ثابت
{ بازگشت p ? p->maxWorkGroupSize_ : 0; }

صحیح Device::maxWorkItemDims() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

تهی Device::maxWorkItemSizes(size_t* sizes) ثابت
{
    اگر(p)
    {
        ثابت صحیح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*اندازه(sizes[0]), &sizes[0], &retsz));
    }
}

صحیح Device::memBaseAddrAlign() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

صحیح Device::nativeVectorWidthChar() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

صحیح Device::nativeVectorWidthShort() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

صحیح Device::nativeVectorWidthInt() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

صحیح Device::nativeVectorWidthLong() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

صحیح Device::nativeVectorWidthFloat() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

صحیح Device::nativeVectorWidthDouble() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

صحیح Device::nativeVectorWidthHalf() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

صحیح Device::preferredVectorWidthChar() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

صحیح Device::preferredVectorWidthShort() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

صحیح Device::preferredVectorWidthInt() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

صحیح Device::preferredVectorWidthLong() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

صحیح Device::preferredVectorWidthFloat() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

صحیح Device::preferredVectorWidthDouble() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

صحیح Device::preferredVectorWidthHalf() ثابت
{ بازگشت p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ثابت
#ifdef CL_VERSION_1_2
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#وگرنه
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ثابت
{ بازگشت p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        بازگشت c.getDevice();
    }

    ایستا Device dummy;
    بازگشت dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
درونخطی cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, پوچ, &required);
    اگر (err != CL_SUCCESS)
        بازگشت err;

    param.clear();
    اگر (required > 0)
    {
        AutoBuffer<نویسه> buf(required + 1);
        نویسه* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, پوچ);
        اگر (err != CL_SUCCESS)
            بازگشت err;
        param = ptr;
    }

    بازگشت CL_SUCCESS;
}

ایستا تهی split(ثابت std::string &s, نویسه delim, std::vector<std::string> &elems)
{
    elems.clear();
    اگر (s.size() == 0)
        بازگشت;
    std::istringstream ss(s);
    std::string item;
    وقتی (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ایستا منطقی parseOpenCLDeviceConfiguration(ثابت std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    اگر (parts.size() > 3)
    {
        CV_LOG_ERROR(پوچ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        بازگشت نادرست;
    }
    اگر (parts.size() > 2)
        deviceNameOrID = parts[2];
    اگر (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    اگر (parts.size() > 0)
    {
        platform = parts[0];
    }
    بازگشت درست;
}

ایستا cl_device_id selectOpenCLDevice(ثابت std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    اگر (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    اگر (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        بازگشت پوچ;

    منطقی isID = نادرست;
    صحیح deviceID = -1;
    اگر (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = درست;
        برای (size_t i = 0; i < deviceName.length(); i++)
        {
            اگر (!isdigit(deviceName[i]))
            {
                isID = نادرست;
                بشکن;
            }
        }
        اگر (isID)
        {
            deviceID = atoi(deviceName.c_str());
            اگر (deviceID < 0)
                بازگشت پوچ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint رقم_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, پوچ, &رقم_Platforms));

        اگر (رقم_Platforms == 0)
            بازگشت پوچ;
        platforms.resize((size_t)رقم_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(رقم_Platforms, &platforms[0], &رقم_Platforms));
        platforms.resize(رقم_Platforms);
    }

    اگر (platform.length() > 0)
    {
        برای (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            اگر (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            وگرنه
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        اگر (platforms.size() == 0)
        {
            CV_LOG_ERROR(پوچ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            برو not_found;
        }
    }
    اگر (deviceTypes.size() == 0)
    {
        اگر (!isID)
        {
            deviceTypes.push_back("GPU");
            اگر (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        وگرنه
            deviceTypes.push_back("ALL");
    }
    برای (size_t t = 0; t < deviceTypes.size(); t++)
    {
        صحیح deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        اگر (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        وگرنه اگر (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        وگرنه اگر (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وگرنه اگر (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        وگرنه
        {
            CV_LOG_ERROR(پوچ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            برو not_found;
        }

        std::vector<cl_device_id> devices;
        برای (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint شمار_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, پوچ, &شمار_2);
            اگر (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            اگر (شمار_2 == 0)
                ادامه;
            size_t base = devices.size();
            devices.resize(base + شمار_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, شمار_2, &devices[base], &شمار_2);
            اگر (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        برای (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : درست) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = درست;
            اگر(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, اندازه(isIGPU), &isIGPU, پوچ));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            اگر ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                بازگشت devices[i];
            }
        }
    }

not_found:
    اگر (configuration.empty())
        بازگشت پوچ; // suppress messages on stderr

    std::ostringstream پیام;
    پیام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    برای (size_t t = 0; t < deviceTypes.size(); t++)
        پیام << ' ' << deviceTypes[t];

    پیام << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(پوچ, پیام.str());
    بازگشت پوچ;
}

#ifdef HAVE_OPENCL_SVM
فضاینام svm {

شمارش AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

ایستا منطقی checkForceSVMUmatUsage()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی force = نادرست;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", نادرست);
        initialized = درست;
    }
    بازگشت force;
}
ایستا منطقی checkDisableSVMUMatUsage()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی force = نادرست;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", نادرست);
        initialized = درست;
    }
    بازگشت force;
}
ایستا منطقی checkDisableSVM()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی force = نادرست;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", نادرست);
        initialized = درست;
    }
    بازگشت force;
}
// see SVMCapabilities
ایستا unsigned صحیح getSVMCapabilitiesMask()
{
    ایستا منطقی initialized = نادرست;
    ایستا unsigned صحیح mask = 0;
    اگر (!initialized)
    {
        ثابت std::string env_مقدار = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        اگر (env_مقدار.empty())
        {
            بازگشت ~0U; // all bits 1
        }
        mask = atoi(env_مقدار.c_str());
        initialized = درست;
    }
    بازگشت mask;
}
} // namespace
#endif

ایستا size_t get_Program_شمار_Limit()
{
    ایستا منطقی initialized = نادرست;
    ایستا size_t شمار_2 = 0;
    اگر (!initialized)
    {
        شمار_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = درست;
    }
    بازگشت شمار_2;
}

ایستا صحیح g_contextId = 0;

رده OpenCLBufferPoolImpl;
رده OpenCLSVMBufferPoolImpl;

ساختار Context::Impl
{
    ایستا Context::Impl* get(Context& context) { بازگشت context.getImpl(); }

    تعریفنوع std::deque<Context::Impl*> container_t;
    ایستا container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ایستا container_t* g_contexts = جدید container_t();
        بازگشت *g_contexts;
    }

محافظتشده:
    Impl(ثابت std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(نادرست)
#endif
    {
        اگر (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = این;
    }

    ~Impl()
    {
#ifdef _WIN32
        اگر (!cv::__termination)
#endif
        {
            اگر (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = پوچ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = پوچ;
        }
    }

    تهی init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_رقم_DEVICES, اندازه(ndevices), &ndevices, پوچ));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * اندازه(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * اندازه(cl_device_id), "");

        devices.clear();
        برای (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    تهی __init_buffer_pools();  // w/o synchronization
    تهی _init_buffer_pools() ثابت
    {
        اگر (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            اگر (!bufferPool_)
            {
                const_cast<Impl*>(این)->__init_buffer_pools();
            }
        }
    }
عمومی:
    ایستا Impl* findContext(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        اگر (configuration.empty() && !container.empty())
            بازگشت container[0];
        برای (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            اگر (i && i->configuration == configuration)
            {
                بازگشت i;
            }
        }
        بازگشت پوچ;
    }

    ایستا Impl* findOrCreateContext(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        اگر (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            اگر (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(پوچ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            بازگشت impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        اگر (d == پوچ)
            بازگشت پوچ;

        impl = جدید Impl(configuration);
        تلاش
        {
            impl->createFromDevice(d);
            اگر (impl->handle)
                بازگشت impl;
            delete impl;
            بازگشت پوچ;
        }
        بگیر (...)
        {
            delete impl;
            پرتابکن;
        }
    }

    ایستا Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (تهی*)h);
        Impl* impl = findContext(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(پوچ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            بازگشت impl;
        }

        impl = جدید Impl(configuration);
        تلاش
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            بازگشت impl;
        }
        بگیر (...)
        {
            delete impl;
            پرتابکن;
        }
    }

    ایستا Impl* findOrCreateContext(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (تهی*)d);
        Impl* impl = findContext(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(پوچ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            بازگشت impl;
        }

        impl = جدید Impl(configuration);
        تلاش
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            بازگشت impl;
        }
        بگیر (...)
        {
            delete impl;
            پرتابکن;
        }
    }

    تهی setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        اگر (d == پوچ)
            بازگشت;

        createFromDevice(d);
    }

    تهی createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == پوچ);

        cl_platform_id pl = پوچ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, اندازه(cl_platform_id), &pl, پوچ));

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

        منطقی ok = handle != 0 && status == CL_SUCCESS;
        اگر( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        وگرنه
            handle = پوچ;
    }

    Program getProg(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg);

    تهی unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        برای (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              اگر (it != phash.end())
              {
                  اگر (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      بازگشت;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        اگر (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            اگر (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحیح bits = d.addressBits();
                اگر (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                برای (size_t i = 0; i < prefix.size(); i++)
                {
                    نویسه c = prefix[i];
                    اگر (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        بازگشت prefix;
    }

    std::string& getPrefixBase()
    {
        اگر (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            اگر (prefix_base.empty())
            {
                ثابت Device& d = devices[0];
                صحیح bits = d.addressBits();
                اگر (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                برای (size_t i = 0; i < prefix_base.size(); i++)
                {
                    نویسه c = prefix_base[i];
                    اگر (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        بازگشت prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحیح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    تعریفنوع std::map<std::string, Program> phash_t;
    phash_t phash;
    تعریفنوع std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        بازگشت *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        بازگشت *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    تهی setUserContext(std::type_index typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        اگر (it != userContextStorage.end())
            بازگشت it->second;
        وگرنه
            بازگشت nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    منطقی svmInitialized;
    منطقی svmAvailable;
    منطقی svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    تهی svmInit()
    {
        CV_Assert(handle != پوچ);
        ثابت Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((تهی)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, اندازه(deviceCaps), &deviceCaps, پوچ);
        اگر (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            برو noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (صحیح)deviceCaps);
        CV_Assert(((تهی)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.مقدار_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.مقدار_2 &= svm::getSVMCapabilitiesMask();
        اگر (svmCapabilities.مقدار_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            برو noSVM;
        }
        تلاش
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            تهی* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            اگر (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            تلاش
            {
                منطقی error = نادرست;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                اگر (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, پوچ, پوچ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                تلاش
                {
                    ((صحیح*)ptr)[0] = 100;
                }
                بگیر (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = درست;
                }
                اگر (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, پوچ, پوچ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                اگر (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            بگیر (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                پرتابکن;
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
        بگیر (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            تلاش
            {
                // Try HSA extension
                String extensions = device.extensions();
                اگر (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    برو noSVM;
                }
                cl_platform_id p = پوچ;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, اندازه(cl_platform_id), &p, پوچ));
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
            بگیر (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                برو noSVM;
            }
        }

        svmAvailable = درست;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = درست;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        بازگشت;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = نادرست;
        svmEnabled = نادرست;
        svmCapabilities.مقدار_2 = 0;
        svmInitialized = درست;
        svmFunctions.fn_clSVMAlloc = پوچ;
        بازگشت;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        بازگشت *bufferPoolSVM_.get();
    }
#endif

    friend رده Program;
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
Context::Context(صحیح dtype)
{
    p = 0;
    create(dtype);
}

تهی Context::release()
{
    اگر (p)
    {
        p->release();
        p = پوچ;
    }
}

منطقی Context::create()
{
    release();
    اگر (!haveOpenCL())
        بازگشت نادرست;
    p = Impl::findOrCreateContext(std::string());
    اگر (p && p->handle)
        بازگشت درست;
    release();
    بازگشت نادرست;
}

// deprecated
منطقی Context::create(صحیح dtype)
{
    اگر( !haveOpenCL() )
        بازگشت نادرست;
    release();
    اگر (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    وگرنه اگر (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    وگرنه اگر (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    وگرنه
    {
        CV_LOG_ERROR(پوچ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    اگر (p && !p->handle)
    {
        release();
    }
    بازگشت p != 0;
}

Context::Context(ثابت Context& c)
{
    p = (Impl*)c.p;
    اگر(p)
        p->addref();
}

Context& Context::operator = (ثابت Context& c)
{
    Impl* newp = (Impl*)c.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    اگر (این != &c) {
        اگر(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    بازگشت *این;
}

تهی* Context::ptr() ثابت
{
    بازگشت p == پوچ ? پوچ : p->handle;
}

size_t Context::ndevices() ثابت
{
    بازگشت p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ثابت
{
    ایستا Device dummy;
    بازگشت !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        auto& ctx = c.getContext();
        بازگشت ctx;
    }

    ایستا Context dummy;
    بازگشت dummy;
}

Program Context::getProg(ثابت ProgramSource& prog,
                         ثابت String& buildopts, String& errmsg)
{
    بازگشت p ? p->getProg(prog, buildopts, errmsg) : Program();
}

تهی Context::unloadProg(Program& prog)
{
    اگر (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(تهی* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    بازگشت ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    بازگشت ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    بازگشت ctx;
}

تهی* Context::getOpenCLContextProperty(صحیح propertyId) ثابت
{
    اگر (p == پوچ)
        بازگشت nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, پوچ, &size));
    std::vector<cl_context_properties> prop(size / اندازه(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), پوچ));
    برای (size_t i = 0; i < prop.size(); i += 2)
    {
        اگر (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(پوچ, "OpenCL: found context property=" << propertyId << ") => " << (تهی*)prop[i + 1]);
            بازگشت (تهی*)prop[i + 1];
        }
    }
    بازگشت nullptr;
}

#ifdef HAVE_OPENCL_SVM
منطقی Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    بازگشت i->svmEnabled;
}
تهی Context::setUseSVM(منطقی enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    اگر (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#وگرنه
منطقی Context::useSVM() ثابت { بازگشت نادرست; }
تهی Context::setUseSVM(منطقی enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
فضاینام svm {

ثابت SVMCapabilities getSVMCapabilitites(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    بازگشت i->svmCapabilities;
}

CV_EXPORTS ثابت SVMFunctions* getSVMFunctions(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != پوچ);
    بازگشت &i->svmFunctions;
}

CV_EXPORTS منطقی useSVM(UMatUsageFlags usageFlags)
{
    اگر (checkForceSVMUmatUsage())
        بازگشت درست;
    اگر (checkDisableSVMUMatUsage())
        بازگشت نادرست;
    اگر ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        بازگشت درست;
    بازگشت نادرست; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

تهی Context::setUserContext(std::type_index typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    بازگشت p->getUserContext(typeId);
}

ایستا تهی get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<نویسه> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
تهی attachContext(ثابت String& platformName, تهی* platformID, تهی* context, تهی* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ثابت std::string& platformName, تهی* platformID, تهی* context, تهی* deviceID
)
{
    اگر (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    اگر (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    منطقی platformAvailable = نادرست;

    // check if external platformName contained in list of available platforms in OpenCV
    برای (unsigned صحیح i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        اگر (platformName == availablePlatformName)
        {
            platformAvailable = درست;
            بشکن;
        }
    }

    اگر (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    اگر (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    بازگشت ctx;
}

تهی initializeContextFromHandle(Context& ctx, تهی* _platform, تهی* _context, تهی* _device)
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

ساختار Queue::Impl
{
    درونخطی تهی __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = نادرست;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, اندازه(cl_command_queue_properties), &props, پوچ));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, منطقی isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ثابت Context& c, ثابت Device& d, منطقی withProfiling = نادرست)
    {
        __init();

        ثابت Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        اگر( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        اگر( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        اگر (!cv::__termination)
#endif
        {
            اگر(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = پوچ;
            }
        }
    }

    ثابت cv::ocl::Queue& getProfilingQueue(ثابت cv::ocl::Queue& self)
    {
        اگر (isProfilingQueue_)
            بازگشت self;

        اگر (profiling_queue_.ptr())
            بازگشت profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, اندازه(cl_context), &ctx, پوچ));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, اندازه(cl_device_id), &device, پوچ));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = جدید Impl(q, درست);
        profiling_queue_ = queue;

        بازگشت profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    منطقی isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ثابت Context& c, ثابت Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ثابت Queue& q)
{
    p = q.p;
    اگر(p)
        p->addref();
}

Queue& Queue::operator = (ثابت Queue& q)
{
    Impl* newp = (Impl*)q.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    اگر (این != &q) {
        اگر(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    بازگشت *این;
}

Queue::~Queue()
{
    اگر(p)
        p->release();
}

منطقی Queue::create(ثابت Context& c, ثابت Device& d)
{
    اگر(p)
        p->release();
    p = جدید Impl(c, d);
    بازگشت p->handle != 0;
}

تهی Queue::finish()
{
    اگر(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ثابت Queue& Queue::getProfilingQueue() ثابت
{
    CV_Assert(p);
    بازگشت p->getProfilingQueue(*این);
}

تهی* Queue::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        auto& q = c.getQueue();
        بازگشت q;
    }
    ایستا Queue dummy;
    بازگشت dummy;
}

ایستا cl_command_queue getQueue(ثابت Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    اگر(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    بازگشت qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحیح _flags, UMat* _m, صحیح _wscale, صحیح _iwscale, ثابت تهی* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != پوچ);
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    CV_Assert(m.isContinuous());
    بازگشت KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ساختار Kernel::Impl
{
    Impl(ثابت نویسه* kname, ثابت Program& prog) :
        refcount(1), handle(پوچ), isInProgress(نادرست), isAsyncRun(نادرست), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        اگر (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        برای( صحیح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = نادرست;
        haveTempSrcUMats = نادرست;
    }

    تهی cleanupUMats()
    {
        منطقی exceptionOccurred = نادرست;
        برای( صحیح i = 0; i < MAX_ARRS; i++ )
        {
            اگر( u[i] )
            {
                اگر( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    تلاش
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    بگیر(ثابت std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(پوچ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = درست;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = نادرست;
        haveTempSrcUMats = نادرست;
        CV_Assert(!exceptionOccurred);
    }

    تهی addUMat(ثابت UMat& m, منطقی dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        اگر(dst && m.u->tempUMat())
            haveTempDstUMats = درست;
        اگر(m.u->originalUMatData == پوچ && m.u->tempUMat())
            haveTempSrcUMats = درست;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    تهی registerImageArgument(صحیح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        اگر (arg < (صحیح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    تهی finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = نادرست;
        تلاش
        {
            cleanupUMats();
        }
        بگیر(...)
        {
            release();
            پرتابکن;
        }
        release();
    }

    منطقی run(صحیح dims, size_t _globalsize[], size_t _localsize[],
            منطقی همگام, int64* timeNS, ثابت Queue& q);

    ~Impl()
    {
        اگر(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    شمارش { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    منطقی isInProgress;
    منطقی isAsyncRun;  // true if kernel was scheduled in async mode
    صحیح nu;
    std::vector<Image2D> shadow_images;
    منطقی haveTempDstUMats;
    منطقی haveTempSrcUMats;
};

}} // namespace cv::ocl

بیرونی "C" {

ایستا تهی CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, تهی *p)
{
    تلاش
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    بگیر (ثابت cv::Exception& exc)
    {
        CV_LOG_ERROR(پوچ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    بگیر (ثابت std::exception& exc)
    {
        CV_LOG_ERROR(پوچ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    بگیر (...)
    {
        CV_LOG_ERROR(پوچ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

فضاینام cv { فضاینام ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ثابت نویسه* kname, ثابت Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت نویسه* kname, ثابت ProgramSource& src,
               ثابت String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ثابت Kernel& k)
{
    p = k.p;
    اگر(p)
        p->addref();
}

Kernel& Kernel::operator = (ثابت Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    اگر (این != &k) {
        اگر(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    بازگشت *این;
}

Kernel::~Kernel()
{
    اگر(p)
        p->release();
}

منطقی Kernel::create(ثابت نویسه* kname, ثابت Program& prog)
{
    اگر(p)
        p->release();
    p = جدید Impl(kname, prog);
    اگر(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    بازگشت p != 0;
}

منطقی Kernel::create(ثابت نویسه* kname, ثابت ProgramSource& src,
                    ثابت String& buildopts, String* errmsg)
{
    اگر(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    اگر( !errmsg ) errmsg = &tempmsg;
    ثابت Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    بازگشت create(kname, prog);
}

تهی* Kernel::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

منطقی Kernel::empty() ثابت
{
    بازگشت ptr() == 0;
}

ایستا cv::String dump_مقدار(size_t sz, ثابت تهی* p)
{
    اگر (!p)
        بازگشت "NULL";
    اگر (sz == 2)
        بازگشت cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    اگر (sz == 4)
        بازگشت cv::format("%d / %uu / 0x%08x / %g", *(صحیح*)p, *(صحیح*)p, *(صحیح*)p, *(اعشاری*)p);
    اگر (sz == 8)
        بازگشت cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(دوگانه*)p);
    بازگشت cv::format("%p", p);
}

صحیح Kernel::set(صحیح i, ثابت تهی* value, size_t sz)
{
    اگر (!p || !p->handle)
        بازگشت -1;
    اگر (i < 0)
        بازگشت i;
    اگر( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحیح)i, (صحیح)sz, dump_مقدار(sz, value).c_str()).c_str());
    اگر (retval != CL_SUCCESS)
        بازگشت -1;
    بازگشت i+1;
}

صحیح Kernel::set(صحیح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحیح res = set(i, &h, اندازه(h));
    اگر (res >= 0)
        p->registerImageArgument(i, image2D);
    بازگشت res;
}

صحیح Kernel::set(صحیح i, ثابت UMat& m)
{
    بازگشت set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ایستا منطقی isCudaBacked(ثابت UMatData* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    بازگشت u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحیح Kernel::set(صحیح i, ثابت KernelArg& arg)
{
    اگر( !p || !p->handle )
        بازگشت -1;
    اگر (i < 0)
    {
        CV_LOG_ERROR(پوچ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (صحیح)i));
        بازگشت i;
    }
    اگر( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    اگر( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        منطقی ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        اگر (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)پوچ;
            status = clSetKernelArg(p->handle, (cl_uint)i, اندازه(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحیح)i).c_str());
            بازگشت i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        اگر (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(پوچ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (صحیح)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            بازگشت -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        اگر (!h)
        {
            CV_LOG_ERROR(پوچ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (صحیح)i, (صحیح)arg.flags, arg.m));
            p->release();
            p = 0;
            بازگشت -1;
        }

#ifdef HAVE_OPENCL_SVM
        اگر ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#اگر 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#وگرنه
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحیح)i, (تهی*)svmDataPtr).c_str());
        }
        وگرنه
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, اندازه(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحیح)i, (تهی*)h).c_str());
        }

        اگر (ptronly)
        {
            i++;
        }
        وگرنه اگر( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), اندازه(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), اندازه(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)u2d.offset).c_str());
            i += 3;

            اگر( !(arg.flags & KernelArg::NO_SIZE) )
            {
                صحیح cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, اندازه(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحیح)i, (صحیح)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), اندازه(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)cols).c_str());
                i += 2;
            }
        }
        وگرنه
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), اندازه(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), اندازه(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), اندازه(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحیح)(i+3), (صحیح)u3d.offset).c_str());
            i += 4;
            اگر( !(arg.flags & KernelArg::NO_SIZE) )
            {
                صحیح cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, اندازه(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحیح)i, (صحیح)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), اندازه(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), اندازه(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        بازگشت i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحیح)i, (صحیح)arg.sz, (تهی*)arg.obj).c_str());
    بازگشت i+1;
}

منطقی Kernel::run(صحیح dims, size_t _globalsize[], size_t _localsize[],
                 منطقی همگام, ثابت Queue& q)
{
    اگر (!p)
        بازگشت نادرست;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != پوچ);
    برای (صحیح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحیح)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        اگر (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned صحیح)val) * val;
    }
    CV_Assert(total > 0);

    بازگشت p->run(dims, globalsize, _localsize, همگام, پوچ, q);
}


منطقی Kernel::run_(صحیح dims, size_t _globalsize[], size_t _localsize[],
                  منطقی همگام, ثابت Queue& q)
{
    CV_Assert(p);
    بازگشت p->run(dims, _globalsize, _localsize, همگام, پوچ, q);
}


منطقی Kernel::Impl::run(صحیح dims, size_t globalsize[], size_t localsize[],
        منطقی همگام, int64* timeNS, ثابت Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    اگر (!handle)
    {
        CV_LOG_ERROR(پوچ, "OpenCL kernel has zero handle: " << name);
        بازگشت نادرست;
    }

    اگر (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !همگام;

    اگر (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#اگر CV_OPENCL_SYNC_RUN_KERNELS
    همگام = درست;
#endif

    cl_command_queue qq = getQueue(q);
    اگر (haveTempDstUMats)
        همگام = درست;
    اگر (haveTempSrcUMats)
        همگام = درست;
    اگر (timeNS)
        همگام = درست;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           پوچ, globalsize, localsize, 0, 0,
                                           (همگام && !timeNS) ? 0 : &asyncEvent);
#اگر !CV_OPENCL_SHOW_RUN_KERNELS
    اگر (retval != CL_SUCCESS)
#endif
    {
        cv::String پیام = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحیح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        همگام ? "true" : "false"
                        );
        اگر (retval != CL_SUCCESS)
        {
            پیام = CV_OCL_API_ERROR_پیام(retval, پیام.c_str());
        }
#اگر CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, پیام.c_str());
#وگرنه
        printf("%s\n", پیام.c_str());
        fflush(stdout);
#endif
    }
    اگر (همگام || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        اگر (timeNS)
        {
            اگر (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, اندازه(startTime), &startTime, پوچ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, اندازه(stopTime), &stopTime, پوچ));
                *timeNS = (int64)(stopTime - startTime);
            }
            وگرنه
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    وگرنه
    {
        addref();
        isInProgress = درست;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, این));
    }
    اگر (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    بازگشت retval == CL_SUCCESS;
}

منطقی Kernel::runTask(منطقی همگام, ثابت Queue& q)
{
    اگر(!p || !p->handle || p->isInProgress)
        بازگشت نادرست;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, همگام ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), همگام ? "true" : "false").c_str());
    اگر (همگام || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    وگرنه
    {
        p->addref();
        p->isInProgress = درست;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    اگر (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    بازگشت retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(صحیح dims, size_t globalsize[], size_t localsize[], ثابت Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    منطقی res = p->run(dims, globalsize, localsize, درست, &timeNs, profilingQueue);
    بازگشت res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ثابت
{
    اگر(!p || !p->handle)
        بازگشت 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, اندازه(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    بازگشت status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ثابت
{
    اگر(!p || !p->handle)
        بازگشت 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, اندازه(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    بازگشت status == CL_SUCCESS ? val : 0;
}

منطقی Kernel::compileWorkGroupSize(size_t wsz[]) ثابت
{
    اگر(!p || !p->handle || !wsz)
        بازگشت 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, اندازه(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    بازگشت status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ثابت
{
    اگر(!p || !p->handle)
        بازگشت 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, اندازه(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    بازگشت status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ساختار ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    شمارش KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ثابت String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& codeStr, ثابت String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    تهی init(شمارش KIND kind, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = پوچ;
        sourceSize_ = 0;
        isHashUpdated = نادرست;
    }

    تهی initFromSource(ثابت String& codeStr, ثابت String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        اگر (sourceHash_.empty())
        {
            updateHash();
        }
        وگرنه
        {
            isHashUpdated = درست;
        }
    }

    تهی updateHash(ثابت نویسه* hashStr = پوچ)
    {
        اگر (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = درست;
            بازگشت;
        }
        uint64 hash = 0;
        انتخاب (kind_)
        {
        حالت PROGRAM_SOURCE_CODE:
            اگر (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            وگرنه
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            بشکن;
        حالت PROGRAM_BINARIES:
        حالت PROGRAM_SPIR:
        حالت PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            بشکن;
        پیشفرض:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = درست;
    }

    Impl(شمارش KIND kind,
            ثابت String& module, ثابت String& name,
            ثابت unsigned نویسه* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    ایستا ProgramSource fromSourceWithStaticLifetime(ثابت String& module, ثابت String& name,
            ثابت نویسه* sourceCodeStaticStr, ثابت نویسه* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جدید Impl(PROGRAM_SOURCE_CODE, module, name,
                (ثابت unsigned نویسه*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        بازگشت result;
    }

    ایستا ProgramSource from_دودویی(ثابت String& module, ثابت String& name,
            ثابت unsigned نویسه* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جدید Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        بازگشت result;
    }

    ایستا ProgramSource fromSPIR(ثابت String& module, ثابت String& name,
            ثابت unsigned نویسه* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جدید Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        بازگشت result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ثابت unsigned نویسه* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    منطقی isHashUpdated;

    friend ساختار Program::Impl;
    friend ساختار internal::ProgramEntry;
    friend ساختار Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ثابت String& module, ثابت String& name, ثابت String& codeStr, ثابت String& codeHash)
{
    p = جدید Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ثابت نویسه* prog)
{
    p = جدید Impl(prog);
}

ProgramSource::ProgramSource(ثابت String& prog)
{
    p = جدید Impl(prog);
}

ProgramSource::~ProgramSource()
{
    اگر(p)
        p->release();
}

ProgramSource::ProgramSource(ثابت ProgramSource& prog)
{
    p = prog.p;
    اگر(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ثابت ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    اگر (این != &prog) {
        اگر(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    بازگشت *این;
}

ثابت String& ProgramSource::source() ثابت
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == پوچ); // method returns reference - can't construct temporary object
    بازگشت p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_دودویی(ثابت String& module, ثابت String& name,
        ثابت unsigned نویسه* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    بازگشت Impl::from_دودویی(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت unsigned نویسه* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    بازگشت Impl::from_دودویی(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ثابت
{
    اگر (این->pProgramSource == پوچ)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        اگر (این->pProgramSource == پوچ)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(این->module, این->name, این->programCode, این->programHash, cv::String());
            ProgramSource* ptr = جدید ProgramSource(ps);
            const_cast<ProgramEntry*>(این)->pProgramSource = ptr;
        }
    }
    بازگشت *این->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ایستا
cv::String joinBuildOptions(ثابت cv::String& a, ثابت cv::String& b)
{
    اگر (b.empty())
        بازگشت a;
    اگر (a.empty())
        بازگشت b;
    اگر (b[0] == ' ')
        بازگشت a + b;
    بازگشت a + (cv::String(" ") + b);
}

ساختار Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت ProgramSource& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(پوچ),
         buildflags(_buildflags)
    {
        ثابت ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        اگر (ctx.ptr() == پوچ || device.ptr() == پوچ)
            بازگشت;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        اگر (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            اگر (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            وگرنه اگر (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ثابت String param_buildExtraOptions = getBuildExtraOptions();
            اگر (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#اگر CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(پوچ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#اگر CV_OPENCL_SHOW_BUILD_KERNELS
        اگر (handle)
        {
            size_t retsz = 0;
            نویسه kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, اندازه(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            اگر (result == CL_SUCCESS && retsz < اندازه(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(پوچ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            وگرنه
            {
                CV_LOG_ERROR(پوچ, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    منطقی compile(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        اگر (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(پوچ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقی isLoaded = create_From_دودویی(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            بازگشت isLoaded;
        }
        بازگشت compileWithCache(ctx, src_, errmsg);
    }

    منطقی compileWithCache(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ثابت String& hash_str = src_->sourceHash_;
        cv::String fname;
        اگر (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        اگر (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            تلاش
            {
                std::vector<نویسه> دودویی_Buf;
                منطقی res = نادرست;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    دودویی_Program_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, دودویی_Buf);
                }
                اگر (res)
                {
                    CV_Assert(!دودویی_Buf.empty());
                    CV_LOG_VERBOSE(پوچ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقی isLoaded = create_From_دودویی(ctx, دودویی_Buf, errmsg);
                    اگر (isLoaded)
                        بازگشت درست;
                }
            }
            بگیر (ثابت cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(پوچ, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            بگیر (...)
            {
                CV_LOG_VERBOSE(پوچ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == پوچ);
        اگر (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            اگر (!buildFromSources(ctx, src_, errmsg))
            {
                بازگشت نادرست;
            }
        }
        وگرنه اگر (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            اگر ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(پوچ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقی isLoaded = create_From_دودویی(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            اگر (!isLoaded)
                بازگشت نادرست;
        }
        وگرنه اگر (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وگرنه
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != پوچ);
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
        اگر (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            تلاش
            {
                std::vector<نویسه> دودویی_Buf;
                get_Program_دودویی(دودویی_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    دودویی_Program_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, دودویی_Buf);
                }
            }
            بگیر (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(پوچ, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            بگیر (...)
            {
                CV_LOG_WARNING(پوچ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#اگر CV_OPENCL_VALIDATE_دودویی_PROGRAMS
        اگر (CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
        {
            std::vector<نویسه> دودویی_Buf;
            get_Program_دودویی(دودویی_Buf);
            اگر (!دودویی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = پوچ;
                create_From_دودویی(ctx, دودویی_Buf, errmsg);
            }
        }
#endif
        بازگشت handle != پوچ;
    }

    تهی dumpBuildLog_(cl_int result, ثابت cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<نویسه, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        اگر (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            اگر (log_retval == CL_SUCCESS)
            {
                اگر (retsz < buffer.size())
                    buffer[retsz] = 0;
                وگرنه
                    buffer[buffer.size() - 1] = 0;
            }
            وگرنه
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

    منطقی buildFromSources(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == پوچ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(پوچ, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ثابت نویسه* srcptr = src_->sourceAddr_ ? ((ثابت نویسه*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != پوچ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        اگر (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            برای (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#اگر !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            اگر (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                اگر (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = پوچ;
                }
                اگر (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#اگر CV_OPENCL_VALIDATE_دودویی_PROGRAMS
            اگر (handle && CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
            {
                CV_LOG_INFO(پوچ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                نویسه kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, اندازه(kernels_buffer), &kernels_buffer[0], &retsz);
                اگر (retsz < اندازه(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                وگرنه
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(پوچ, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        بازگشت handle != پوچ;
    }

    تهی get_Program_دودویی(std::vector<نویسه>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_دودویی_SIZES, اندازه(sz), &sz, پوچ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, اندازه(ptr), &ptr, پوچ));
    }

    منطقی create_From_دودویی(ثابت Context& ctx, ثابت std::vector<نویسه>& buf, String& errmsg)
    {
        بازگشت create_From_دودویی(ctx, (ثابت unsigned نویسه*)&buf[0], buf.size(), errmsg);
    }

    منطقی create_From_دودویی(ثابت Context& ctx, ثابت unsigned نویسه* دودویی_Addr, ثابت size_t دودویی_Size, String& errmsg)
    {
        CV_Assert(handle == پوچ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(پوچ, 0, "Load from binary... (" << دودویی_Size << " bytes)");

        CV_Assert(دودویی_Size > 0);

        size_t ndevices = (صحیح)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ثابت uchar*> دودویی_Ptrs(ndevices);
        AutoBuffer<size_t> دودویی_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ثابت uchar** دودویی_Ptrs_2 = دودویی_Ptrs.data();
        size_t* دودویی_Sizes_2 = دودویی_Sizes.data();
        برای (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            دودویی_Ptrs_2[i] = دودویی_Addr;
            دودویی_Sizes_2[i] = دودویی_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_دودویی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           دودویی_Sizes_2, دودویی_Ptrs_2, پوچ, &result);
        اگر (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(پوچ, CV_OCL_API_ERROR_پیام(result, "clCreateProgramWithBinary"));
            اگر (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = پوچ;
            }
        }
        اگر (!handle)
        {
            بازگشت نادرست;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            اگر (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                اگر (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = پوچ;
                }
                بازگشت نادرست;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    اندازه(build_status), &build_status, &retsz));
            اگر (result == CL_SUCCESS)
            {
                اگر (build_status == CL_BUILD_SUCCESS)
                {
                    بازگشت درست;
                }
                وگرنه
                {
                    CV_LOG_WARNING(پوچ, "clGetProgramBuildInfo() returns " << build_status);
                    بازگشت نادرست;
                }
            }
            وگرنه
            {
                CV_LOG_ERROR(پوچ, CV_OCL_API_ERROR_پیام(result, "clGetProgramBuildInfo()"));
                اگر (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = پوچ;
                }
            }
        }
#اگر CV_OPENCL_VALIDATE_دودویی_PROGRAMS
        اگر (handle && CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
        {
            CV_LOG_INFO(پوچ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            نویسه kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, اندازه(kernels_buffer), &kernels_buffer[0], &retsz);
            اگر (retsz < اندازه(kernels_buffer))
                kernels_buffer[retsz] = 0;
            وگرنه
                kernels_buffer[0] = 0;
            CV_LOG_INFO(پوچ, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        بازگشت handle != پوچ;
    }

    ~Impl()
    {
        اگر( handle )
        {
#ifdef _WIN32
            اگر (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = پوچ;
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

Program::Program(ثابت ProgramSource& src,
        ثابت String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ثابت Program& prog)
{
    p = prog.p;
    اگر(p)
        p->addref();
}

Program& Program::operator = (ثابت Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    بازگشت *این;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    اگر (این != &prog) {
        اگر(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    بازگشت *این;
}

Program::~Program()
{
    اگر(p)
        p->release();
}

منطقی Program::create(ثابت ProgramSource& src,
            ثابت String& buildflags, String& errmsg)
{
    اگر(p)
    {
        p->release();
        p = پوچ;
    }
    p = جدید Impl(src, buildflags, errmsg);
    اگر(!p->handle)
    {
        p->release();
        p = 0;
    }
    بازگشت p != 0;
}

تهی* Program::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ثابت ProgramSource& Program::source() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقی Program::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقی Program::اكتب(String& bin) ثابت
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ثابت
{
    اگر(!p)
        بازگشت String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    بازگشت cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        بازگشت cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

تهی Program::get_دودویی(std::vector<نویسه>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_Program_دودویی(binary);
}

Program Context::Impl::getProg(ثابت ProgramSource& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t limit = get_Program_شمار_Limit();
    ثابت ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلید = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(کلید);
        اگر (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), کلید);
            اگر (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(کلید);
            }
            بازگشت it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            اگر (limit > 0 && sz >= limit)
            {
                ایستا منطقی warningFlag = نادرست;
                اگر (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = درست;
                }
                وقتی (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    اگر (c != 0)
                        بشکن;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(کلید, prog));
        cacheList.push_front(کلید);
    }
    بازگشت prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
رده OpenCLBufferPool
{
محافظتشده:
    ~OpenCLBufferPool() { }
عمومی:
    مجازی T allocate(size_t size) = 0;
    مجازی تهی release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
رده OpenCLBufferPoolBaseImpl : عمومی BufferPoolController, عمومی OpenCLBufferPool<T>
{
خصوصی:
    درونخطی Derived& derived() { بازگشت *static_cast<Derived*>(این); }
محافظتشده:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقی _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        برای (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            اگر (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                بازگشت درست;
            }
        }
        بازگشت نادرست;
    }

    // synchronized
    منطقی _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ثابت size_t size)
    {
        اگر (reservedEntries_.empty())
            بازگشت نادرست;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        برای (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            اگر (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                اگر (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    اگر (diff == 0)
                        بشکن;
                }
            }
        }
        اگر (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            بازگشت درست;
        }
        بازگشت نادرست;
    }

    // synchronized
    تهی _checkSizeOfReservedEntries()
    {
        وقتی (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    درونخطی size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        اگر (size < 1024*1024)
            بازگشت 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        وگرنه اگر (size < 16*1024*1024)
            بازگشت 64*1024;
        وگرنه
            بازگشت 1024*1024;
    }

عمومی:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    مجازی ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عمومی:
    مجازی T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        اگر (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        وگرنه
        {
            derived()._allocateBufferEntry(entry, size);
        }
        بازگشت entry.clBuffer_;
    }
    مجازی تهی release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        اگر (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        وگرنه
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    مجازی size_t getReservedSize() ثابت CV_OVERRIDE { بازگشت currentReservedSize; }
    مجازی size_t getMaxReservedSize() ثابت CV_OVERRIDE { بازگشت maxReservedSize; }
    مجازی تهی setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        اگر (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            برای (; i != reservedEntries_.end();)
            {
                ثابت BufferEntry& entry = *i;
                اگر (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    ادامه;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    مجازی تهی freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        برای (; i != reservedEntries_.end(); ++i)
        {
            ثابت BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

ساختار CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)پوچ), capacity_(0) { }
};

رده OpenCLBufferPoolImpl CV_FINAL : عمومی OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
عمومی:
    تعریفنوع ساختار CLBufferEntry BufferEntry;
محافظتشده:
    صحیح createFlags_;
عمومی:
    OpenCLBufferPoolImpl(صحیح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    تهی _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == پوچ);
        entry.capacity_ = alignSize(size, (صحیح)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long صحیح)entry.capacity_, (تهی*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != پوچ);
        اگر(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    تهی _releaseBufferEntry(ثابت BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != پوچ);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
ساختار CLSVMBufferEntry
{
    تهی* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(پوچ), capacity_(0) { }
};
رده OpenCLSVMBufferPoolImpl CV_FINAL : عمومی OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, تهی*>
{
عمومی:
    تعریفنوع ساختار CLSVMBufferEntry BufferEntry;
عمومی:
    OpenCLSVMBufferPoolImpl()
    {
    }

    تهی _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == پوچ);
        entry.capacity_ = alignSize(size, (صحیح)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقی isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحیح)entry.capacity_);
        تهی *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    تهی _releaseBufferEntry(ثابت BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != پوچ);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <منطقی readAccess, منطقی writeAccess>
رده AlignedDataPtr
{
محافظتشده:
    ثابت size_t size_;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عمومی:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(پوچ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        اگر (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جدید uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            اگر (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        بازگشت ptr_;
    }

    ~AlignedDataPtr()
    {
        اگر (allocatedPtr_)
        {
            اگر (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = پوچ;
        }
        ptr_ = پوچ;
    }
خصوصی:
    AlignedDataPtr(ثابت AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ثابت AlignedDataPtr&); // disabled
};

template <منطقی readAccess, منطقی writeAccess>
رده AlignedDataPtr2D
{
محافظتشده:
    ثابت size_t size_;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

عمومی:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(پوچ), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != پوچ);
        اگر (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جدید uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            اگر (readAccess)
            {
                برای (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        بازگشت ptr_;
    }

    ~AlignedDataPtr2D()
    {
        اگر (allocatedPtr_)
        {
            اگر (writeAccess)
            {
                برای (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = پوچ;
        }
        ptr_ = پوچ;
    }
خصوصی:
    AlignedDataPtr2D(ثابت AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ثابت AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


تهی Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(پوچ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

رده OpenCLAllocator CV_FINAL : عمومی MatAllocator
{
عمومی:
    شمارش AllocatorFlags
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

    UMatData* defaultAllocate(صحیح dims, ثابت صحیح* sizes, صحیح type, تهی* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        بازگشت u;
    }

    ایستا منطقی isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ایستا منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", نادرست);
        بازگشت value;
    }
    ایستا منطقی isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ایستا منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", نادرست);
        بازگشت value;
    }

    تهی getBestFlags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحیح& createFlags, UMatData::MemoryFlag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        اگر ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        اگر (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        وگرنه
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       تهی* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        اگر(!useOpenCL())
            بازگشت defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        برای( صحیح i = dims-1; i >= 0; i-- )
        {
            اگر( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        اگر (!ctx.getImpl())
            بازگشت defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحیح createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        تهی* handle = پوچ;
        صحیح allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        اگر (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            منطقی isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        وگرنه
#endif
        اگر (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        وگرنه اگر (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        وگرنه
        {
            CV_Assert(handle != پوچ); // Unsupported, throw
        }

        اگر (!handle)
            بازگشت defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = جدید UMatData(این);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<تهی>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(درست);
        opencl_allocator_stats.onAllocate(u->size);
        بازگشت u;
    }

    منطقی allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت نادرست;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        اگر(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحیح createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقی copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحیح allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            تهی* handle = پوچ;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقی useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            اگر (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (صحیح)u->size, handle);
            }
            وگرنه اگر (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                اگر (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    منطقی isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (صحیح)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = پوچ;
                    اگر (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحیح)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, پوچ, پوچ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    اگر (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, پوچ, پوچ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            وگرنه
#endif
            {
                اگر( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                اگر (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحیح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long صحیح)u->size, u->origdata, (تهی*)handle).c_str());
                }
                اگر((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long صحیح)u->size, u->origdata, (تهی*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (تهی*)handle).c_str());
            اگر(!handle || retval != CL_SUCCESS)
                بازگشت نادرست;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = این;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(درست);
        opencl_allocator_stats.onAllocate(u->size);
        بازگشت درست;
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

    تهی deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        اگر (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        وگرنه
            deallocate_(u);
    }

    تهی deallocate_(UMatData* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        اگر ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        اگر (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            بازگشت;             // avoid any OpenCL calls
#endif
        اگر(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            اگر (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    اگر( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        منطقی isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = پوچ;
                        اگر (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, پوچ, پوچ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        اگر (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, پوچ, پوچ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    وگرنه
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وگرنه
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    اگر( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<نادرست, درست> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وگرنه
                    {
                        cl_int retval = 0;
                        اگر (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            تهی* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (تهی*)u->handle, (long long صحیح)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            اگر (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (تهی*)u->handle, data, (long long صحیح)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(نادرست);
            }
            وگرنه
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                اگر( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وگرنه
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (تهی*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(درست);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = پوچ;
            اگر(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = پوچ;
        }
        وگرنه
        {
            CV_Assert(u->origdata == پوچ);
            اگر(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(درست);
            }
            اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            وگرنه اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            وگرنه اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                وگرنه اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, پوچ, پوچ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((تهی*)u->handle);
            }
#endif
            وگرنه
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(درست);
            delete u;
            u = پوچ;
        }
        CV_Assert(u == پوچ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    تهی map(UMatData* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(درست);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            اگر( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, پوچ, پوچ);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(نادرست);
                    u->markDeviceMemMapped(درست);
                    بازگشت;
                }
#endif

                cl_int retval = CL_SUCCESS;
                اگر (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (تهی*)u->handle, (long long صحیح)u->size, u->data).c_str());
                }
                اگر (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(نادرست);
                    u->markDeviceMemMapped(درست);
                    بازگشت;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            اگر(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(درست);
            }
        }

        اگر (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<نادرست, درست> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (تهی*)u->handle, (long long صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(نادرست);
        }
    }

    تهی unmap(UMatData* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        اگر( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != پوچ);
#ifdef HAVE_OPENCL_SVM
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, پوچ, پوچ);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                اگر (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(نادرست);
                u->markHostCopyObsolete(درست);
                بازگشت;
            }
#endif
            اگر (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (تهی*)u->handle, u->data, (long long صحیح)u->size).c_str());
                اگر (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(نادرست);
                u->data = 0;
                u->markDeviceCopyObsolete(نادرست);
                u->markHostCopyObsolete(درست);
            }
        }
        وگرنه اگر( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<درست, نادرست> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (تهی*)u->handle, (long long صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(نادرست);
            u->markHostCopyObsolete(درست);
        }
    }

    منطقی checkContinuous(صحیح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقی iscontinuous = درست;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        برای( صحیح i = dims-2; i >= 0; i-- )
        {
            اگر( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = نادرست;
            total *= sz[i];
            اگر( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            اگر( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        اگر( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            اگر( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                اگر( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                اگر( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            وگرنه
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                اگر( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                اگر( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        بازگشت iscontinuous;
    }

    تهی download(UMatData* u, تهی* dstptr, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت;
        UMatDataAutoLock autolock(u);

        اگر( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            بازگشت;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == پوچ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, پوچ, پوچ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            اگر( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            وگرنه
            {
                // This code is from MatAllocator::download()
                صحیح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                برای( صحیح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    اگر( sz[i] == 0 )
                    بازگشت;
                    اگر( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (صحیح)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ثابت Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                برای( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, پوچ, پوچ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        وگرنه
#endif
        {
            اگر( iscontinuous )
            {
                AlignedDataPtr<نادرست, درست> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            وگرنه اگر (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<نادرست, نادرست> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                برای( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وگرنه
            {
                AlignedDataPtr2D<نادرست, درست> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    تهی upload(UMatData* u, ثابت تهی* srcptr, صحیح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        اگر(!u)
            بازگشت;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        اگر( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(نادرست);
            u->markDeviceCopyObsolete(درست);
            بازگشت;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == پوچ || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, پوچ, پوچ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            اگر( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            وگرنه
            {
                // This code is from MatAllocator::upload()
                صحیح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                برای( صحیح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    اگر( sz[i] == 0 )
                    بازگشت;
                    اگر( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (صحیح)sz[i];
                }

                Mat src(dims, isz, CV_8U, (تهی*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ثابت Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                برای( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, پوچ, پوچ);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        وگرنه
#endif
        {
            اگر( iscontinuous )
            {
                AlignedDataPtr<درست, نادرست> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (تهی*)u->handle, (long long صحیح)dstrawofs, (long long صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            وگرنه اگر (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<نادرست, نادرست> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                برای( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            وگرنه
            {
                AlignedDataPtr2D<درست, نادرست> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(درست);
#ifdef HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وگرنه
#endif
        {
            u->markHostCopyObsolete(درست);
        }
        u->markDeviceCopyObsolete(نادرست);
    }

    تهی copy(UMatData* src, UMatData* dst, صحیح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقی _sync) ثابت CV_OVERRIDE
    {
        اگر(!src || !dst)
            بازگشت;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        اگر( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            بازگشت;
        }
        اگر( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(نادرست);
#ifdef HAVE_OPENCL_SVM
            اگر ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وگرنه
#endif
            {
                dst->markDeviceCopyObsolete(درست);
            }
            بازگشت;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                اگر( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحیح)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, پوچ, پوچ);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                وگرنه
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    صحیح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    برای( صحیح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        اگر( sz[i] == 0 )
                        بازگشت;
                        اگر( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (صحیح)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    برای( صحیح i = 0; i < dims; i++ )
                    {
                        اگر( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ثابت Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    برای( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            وگرنه
            {
                اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                وگرنه
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        وگرنه
#endif
        {
            اگر( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (تهی*)src->handle, (تهی*)dst->handle, (long long صحیح)srcrawofs, (long long صحیح)dstrawofs, (long long صحیح)total).c_str());
            }
            وگرنه اگر (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<نادرست, نادرست> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<نادرست, نادرست> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                برای( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            وگرنه
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        اگر (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        اگر ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وگرنه
#endif
        {
            dst->markHostCopyObsolete(درست);
        }
        dst->markDeviceCopyObsolete(نادرست);

        اگر( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ثابت نویسه* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        اگر (ctx.empty())
            بازگشت پوچ;
#ifdef HAVE_OPENCL_SVM
        اگر ((svm::checkForceSVMUmatUsage() && (id == پوچ || strcmp(id, "OCL") == 0)) || (id != پوچ && strcmp(id, "SVM") == 0))
        {
            بازگشت &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        اگر (id != پوچ && strcmp(id, "HOST_ALLOC") == 0)
        {
            بازگشت &ctx.getImpl()->getBufferPoolHostPtr();
        }
        اگر (id != پوچ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        بازگشت &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    تهی flushCleanupQueue() ثابت
    {
        // TSAN false positive: see #27638
        اگر (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            برای (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    تهی addToCleanupQueue(UMatData* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

ایستا OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ایستا OpenCLAllocator* g_allocator = جدید OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    بازگشت g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


فضاینام cv {

// three funcs below are implemented in umatrix.cpp
تهی setSize( UMat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* _steps,
              منطقی autoSteps = نادرست );
تهی finalizeHdr(UMat& m);

} // namespace cv


فضاینام cv { فضاینام ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
تهی convertFromBuffer(تهی* cl_mem_buffer, size_t step, صحیح rows, صحیح cols, صحیح type, UMat& dst)
{
    صحیح d = 2;
    صحیح sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, درست);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, اندازه(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, اندازه(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحیح)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = جدید UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    بازگشت;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
تهی convertFromImage(تهی* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, اندازه(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, اندازه(cl_image_format), &fmt, 0));

    صحیح depth = CV_8U;
    انتخاب (fmt.image_channel_data_type)
    {
    حالت CL_UNORM_INT8:
    حالت CL_UNSIGNED_INT8:
        depth = CV_8U;
        بشکن;

    حالت CL_SNORM_INT8:
    حالت CL_SIGNED_INT8:
        depth = CV_8S;
        بشکن;

    حالت CL_UNORM_INT16:
    حالت CL_UNSIGNED_INT16:
        depth = CV_16U;
        بشکن;

    حالت CL_SNORM_INT16:
    حالت CL_SIGNED_INT16:
        depth = CV_16S;
        بشکن;

    حالت CL_SIGNED_INT32:
        depth = CV_32S;
        بشکن;

    حالت CL_FLOAT:
        depth = CV_32F;
        بشکن;

    حالت CL_HALF_FLOAT:
        depth = CV_16F;
        بشکن;

    پیشفرض:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    صحیح type = CV_8UC1;
    انتخاب (fmt.image_channel_order)
    {
    حالت CL_R:
    حالت CL_A:
    حالت CL_INTENSITY:
    حالت CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        بشکن;

    حالت CL_RG:
    حالت CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        بشکن;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالت CL_RGBA:
    حالت CL_BGRA:
    حالت CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        بشکن;

    پیشفرض:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        بشکن;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, اندازه(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, اندازه(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, اندازه(size_t), &h, 0));

    dst.create((صحیح)h, (صحیح)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, پوچ, پوچ));

    CV_OCL_CHECK(clFinish(q));

    بازگشت;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ایستا تهی getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint رقم_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, پوچ, &رقم_Devices);
    اگر (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &رقم_Devices).c_str());
    }

    اگر (رقم_Devices == 0)
    {
        devices.clear();
        بازگشت;
    }

    devices.resize((size_t)رقم_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, رقم_Devices, &devices[0], &رقم_Devices));
}

ساختار PlatformInfo::Impl
{
    Impl(تهی* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ثابت
    {
        نویسه buf[1024];
        size_t sz=0;
        بازگشت clGetPlatformInfo(handle, prop, اندازه(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < اندازه(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    صحیح versionMajor_;
    صحیح versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(تهی* platform_id)
{
    p = جدید Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    اگر(p)
        p->release();
}

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i)
{
    اگر (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ثابت PlatformInfo& i)
{
    اگر (i.p != p)
    {
        اگر (i.p)
            i.p->addref();
        اگر (p)
            p->release();
        p = i.p;
    }
    بازگشت *این;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    اگر (این != &i) {
        اگر(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    بازگشت *این;
}

صحیح PlatformInfo::device_عدد() ثابت
{
    بازگشت p ? (صحیح)p->devices.size() : 0;
}

تهی PlatformInfo::getDevice(Device& device, صحیح d) ثابت
{
    CV_Assert(p && d < (صحیح)p->devices.size() );
    اگر(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    بازگشت p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    بازگشت p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ثابت
{
    بازگشت p ? p->version_ : String();
}

صحیح PlatformInfo::versionMajor() ثابت
{
    CV_Assert(p);
    بازگشت p->versionMajor_;
}

صحیح PlatformInfo::versionMinor() ثابت
{
    CV_Assert(p);
    بازگشت p->versionMinor_;
}

ایستا تهی getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint رقم_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, پوچ, &رقم_Platforms));

    اگر (رقم_Platforms == 0)
    {
        platforms.clear();
        بازگشت;
    }

    platforms.resize((size_t)رقم_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(رقم_Platforms, &platforms[0], &رقم_Platforms));
}

تهی getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    برای (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((تهی*)&platforms[i]) );
}

ثابت نویسه* typeToStr(صحیح type)
{
    ایستا ثابت نویسه* tab[CV_DEPTH_MAX*16]=
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
    صحیح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت نویسه* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    بازگشت result;
}

ثابت نویسه* memopTypeToStr(صحیح type)
{
    ایستا ثابت نویسه* tab[CV_DEPTH_MAX*16] =
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
    صحیح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت نویسه* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    بازگشت result;
}

ثابت نویسه* vecopTypeToStr(صحیح type)
{
    ایستا ثابت نویسه* tab[CV_DEPTH_MAX*16] =
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
    صحیح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت نویسه* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    بازگشت result;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت نویسه* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, نویسه* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    بازگشت convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت نویسه* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, نویسه* buf, size_t buf_size)
{
    اگر( sdepth == ddepth )
        بازگشت "noconvert";
    ثابت نویسه *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    اگر( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    وگرنه اگر( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    وگرنه
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    بازگشت buf;
}

ثابت نویسه* getOpenCLErrorString(صحیح errorCode)
{
#define CV_OCL_CODE(id) حالت id: بازگشت #id
#define CV_OCL_CODE_(id, name) حالت id: بازگشت #name
    انتخاب (errorCode)
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
    CV_OCL_CODE(CL_INVALID_مقدار);
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
    CV_OCL_CODE(CL_INVALID_دودویی);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_مقدار);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_شمار);
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
    پیشفرض: بازگشت "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
ایستا std::string kerToStr(ثابت Mat & k)
{
    صحیح width = k.cols - 1, depth = k.depth();
    ثابت T * ثابت data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    اگر (depth <= CV_8S)
    {
        برای (صحیح i = 0; i < width; ++i)
            stream << "DIG(" << (صحیح)data[i] << ")";
        stream << "DIG(" << (صحیح)data[width] << ")";
    }
    وگرنه اگر (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        برای (صحیح i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    وگرنه اگر (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        برای (صحیح i = 0; i < width; ++i)
            stream << "DIG(" << (اعشاری)data[i] << "h)";
        stream << "DIG(" << (اعشاری)data[width] << "h)";
    }
    وگرنه
    {
        برای (صحیح i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    بازگشت stream.str();
}

String kernelToStr(درون_داد_Array _kernel, صحیح ddepth, ثابت نویسه * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحیح depth = kernel.depth();
    اگر (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    اگر (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    اگر (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    تعریفنوع std::string (* func_t)(ثابت Mat &);
    ایستا ثابت func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<نویسه>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<صحیح>, kerToStr<اعشاری>, kerToStr<دوگانه>, kerToStr<hfloat>,
                                                0, kerToStr<منطقی>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    بازگشت cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    انجامبده \
    { \
        اگر (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            صحیح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            اگر (cwidth < ckercn || ckercn <= 0) \
                بازگشت 1; \
            cols.push_back(cwidth); \
            اگر (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                بازگشت 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    وقتی ((تهی)0, 0)

صحیح predictOptimalVectorWidth(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
                              درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
                              درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
                              OclVectorStrategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحیح vectorWidths[CV_DEPTH_MAX] = {
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
    اگر (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    بازگشت checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحیح checkOptimalVectorWidth([[maybe_unused]] ثابت صحیح *vectorWidths,
                            [[maybe_unused]] درون_داد_Array src1,
                            [[maybe_unused]] درون_داد_Array src2,
                            [[maybe_unused]] درون_داد_Array src3,
                            [[maybe_unused]] درون_داد_Array src4,
                            [[maybe_unused]] درون_داد_Array src5,
                            [[maybe_unused]] درون_داد_Array src6,
                            [[maybe_unused]] درون_داد_Array src7,
                            [[maybe_unused]] درون_داد_Array src8,
                            [[maybe_unused]] درون_داد_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    بازگشت 1;
#وگرنه
    CV_Assert(vectorWidths);

    صحیح ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<صحیح> dividers, kercns;
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

    برای (size_t i = 0; i < size; ++i)
        وقتی (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحیح kercn = *std::min_element(kercns.begin(), kercns.end());

    بازگشت kercn;
#endif
}

صحیح predictOptimalVectorWidthMax(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
                                 درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
                                 درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9)
{
    بازگشت predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
تهی buildOptionsAddMatrixDescription(String& buildOptions, ثابت String& name, درون_داد_Array _m)
{
    اگر (!buildOptions.empty())
        buildOptions += " ";
    صحیح type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (صحیح)CV_MAT_CN(type),
            name.c_str(), (صحیح)CV_ELEM_SIZE(type),
            name.c_str(), (صحیح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحیح)depth
            );
}


ساختار Image2D::Impl
{
    Impl(ثابت UMat &src, منطقی norm, منطقی alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        اگر (handle)
            clReleaseMemObject(handle);
    }

    ایستا cl_image_format getImageFormat(صحیح depth, صحیح cn, منطقی norm)
    {
        cl_image_format format;
        ایستا ثابت صحیح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ایستا ثابت صحیح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ایستا ثابت صحیح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحیح channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        صحیح channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        بازگشت format;
    }

    ایستا منطقی isFormatSupported(cl_image_format format)
    {
        اگر (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        اگر (!context)
            بازگشت نادرست;

        // Figure out how many formats are supported by this context.
        cl_uint رقم_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, رقم_Formats,
                                                پوچ, &رقم_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        اگر (رقم_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(رقم_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, رقم_Formats,
                                             formats.data(), پوچ);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            برای (cl_uint i = 0; i < رقم_Formats; ++i)
            {
                اگر (!memcmp(&formats[i], &format, اندازه(format)))
                {
                    بازگشت درست;
                }
            }
        }
        بازگشت نادرست;
    }

    تهی init(ثابت UMat &src, منطقی norm, منطقی alias)
    {
        اگر (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        صحیح err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        اگر (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        اگر (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحیح minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        اگر (1 < major || (1 == major && 2 <= minor))
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
            desc.رقم_mip_levels   = 0;
            desc.رقم_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, پوچ, &err);
        }
        وگرنه
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, پوچ, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        اگر (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), پوچ, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long صحیح)(src.cols * src.rows * src.elemSize()), (تهی*)devData
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, پوچ, پوچ));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        وگرنه
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != پوچ);

        اگر (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, پوچ, 0));
            اگر (!src.isContinuous())
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
    p = پوچ;
}

Image2D::Image2D(ثابت UMat &src, منطقی norm, منطقی alias)
{
    p = جدید Impl(src, norm, alias);
}

منطقی Image2D::canCreateAlias(ثابت UMat &m)
{
    منطقی ret = نادرست;
    ثابت Device & d = ocl::Device::getDefault();
    اگر (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        اگر (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            اگر (!m.u->tempUMat())
            {
                ret = درست;
            }
        }
    }
    بازگشت ret;
}

منطقی Image2D::isFormatSupported(صحیح depth, صحیح cn, منطقی norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    بازگشت Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    اگر (p)
        p->addref();
}

Image2D & Image2D::operator = (ثابت Image2D & i)
{
    اگر (i.p != p)
    {
        اگر (i.p)
            i.p->addref();
        اگر (p)
            p->release();
        p = i.p;
    }
    بازگشت *این;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    اگر (این != &i) {
        اگر (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    بازگشت *این;
}

Image2D::~Image2D()
{
    اگر (p)
        p->release();
}

تهی* Image2D::ptr() ثابت
{
    بازگشت p ? p->handle : 0;
}

منطقی internal::isOpenCLForced()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی value = نادرست;
    اگر (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", نادرست);
        initialized = درست;
    }
    بازگشت value;
}

منطقی internal::isPerformanceCheckBypassed()
{
    ایستا منطقی initialized = نادرست;
    ایستا منطقی value = نادرست;
    اگر (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", نادرست);
        initialized = درست;
    }
    بازگشت value;
}

منطقی internal::isCLBuffer(UMat& u)
{
    اگر (isCudaBacked(u.u))
        بازگشت نادرست;
    تهی* h = u.handle(ACCESS_RW);
    اگر (!h)
        بازگشت درست;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#اگر 1
    اگر ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        بازگشت نادرست;
#وگرنه
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, اندازه(type), &type, پوچ);
    اگر (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        بازگشت نادرست;
#endif
    بازگشت درست;
}

ساختار Timer::Impl
{
    ثابت Queue queue;

    Impl(ثابت Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    تهی start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    تهی stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        بازگشت (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت Queue& q) : p(جدید Impl(q)) { }
Timer::~Timer() { delete p; }

تهی Timer::start()
{
    CV_Assert(p);
    p->start();
}

تهی Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ثابت
{
    CV_Assert(p);
    بازگشت p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
