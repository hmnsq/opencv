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

#ضمن "precomp.hpp"

#ifndef HAVE_OPENCL
#ضمن "ocl_disabled.impl.hpp"
#وإلا // HAVE_OPENCL

#ضمن <list>
#ضمن <map>
#ضمن <deque>
#ضمن <set>
#ضمن <string>
#ضمن <sstream>
#ضمن <fstream>
#إذا !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#ضمن <inttypes.h>
#endif

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/ocl_genbase.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "opencv2/core/utils/lock.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "OpenCL allocator: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#ضمن "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# إذا 0
#   define LOG_BUFFER_POOL printf
# وإلا
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#إذا CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#وإلا
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#إذا CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#وإلا
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#ضمن "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ضمن "opencv2/core/opencl/opencl_svm.hpp"
#endif

#ضمن "umatrix.hpp"

نطاق cv { نطاق ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    فراغ addref() { CV_XADD(&refcount, 1); } \
    فراغ release() { إذا( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete هذا; } \
    صحيح refcount

ساكن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    أرجع opencl_allocator_stats;
}

#ifndef _DEBUG
ساكن منطقي isRaiseError()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", خطأ);
        initialized = true;
    }
    أرجع value;
}
#endif

ساكن فراغ onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    منطقي value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", خطأ);
    إذا (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#إذا CV_OPENCL_TRACE_CHECK
ساكن مضمن
فراغ traceOpenCLCheck(cl_int status, ثابت حرف* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#وإلا
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_پەیام(check_result, پەیام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, پەیام)

#define CV_OCL_CHECK_RESULT(check_result, پەیام) \
    افعل { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, پەیام); \
        إذا (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پەیام), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پەیام = CV_OCL_API_ERROR_پەیام(check_result, پەیام); \
            CV_Error(Error::OpenCLApiCallError, error_پەیام); \
        } \
    } طالما (0)

#define CV_OCL_CHECK_(دەربڕین, check_result) افعل { دەربڕین; CV_OCL_CHECK_RESULT(check_result, #expr); } طالما (0)

#define CV_OCL_CHECK(دەربڕین) افعل { cl_int __cl_result = (دەربڕین); CV_OCL_CHECK_RESULT(__cl_result, #expr); } طالما (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, پەیام) CV_OCL_CHECK_RESULT(check_result, پەیام)
#define CV_OCL_DBG_CHECK(دەربڕین) CV_OCL_CHECK(دەربڕین)
#define CV_OCL_DBG_CHECK_(دەربڕین, check_result) CV_OCL_CHECK_(دەربڕین, check_result)
#وإلا
#define CV_OCL_DBG_CHECK_RESULT(check_result, پەیام) \
    افعل { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, پەیام); \
        إذا (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پەیام), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پەیام = CV_OCL_API_ERROR_پەیام(check_result, پەیام); \
            CV_Error(Error::OpenCLApiCallError, error_پەیام); \
        } \
    } طالما (0)
#define CV_OCL_DBG_CHECK_(دەربڕین, check_result) افعل { دەربڕین; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } طالما (0)
#define CV_OCL_DBG_CHECK(دەربڕین) افعل { cl_int __cl_result = (دەربڕین); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } طالما (0)
#endif


ساكن ثابت منطقي CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", true);
ساكن ثابت منطقي CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", true);
ساكن ثابت منطقي CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", true);
ساكن ثابت منطقي CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", true);

#إذا CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS
ساكن ثابت منطقي CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS_بەها = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", خطأ);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساكن ثابت منطقي CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#إذا 1 //def __APPLE__
        true
#وإلا
        خطأ
#endif
);

ساكن String getBuildExtraOptions()
{
    ساكن String param_buildExtraOptions;
    ساكن منطقي initialized = خطأ;
    إذا (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = true;
        إذا (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(عدم, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    أرجع param_buildExtraOptions;
}

ساكن ثابت منطقي CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", true);
ساكن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


بنية UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        step = (صحيح)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    صحيح offset;
    صحيح step;
    صحيح rows;
    صحيح cols;
};

بنية UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        step = (صحيح)m.step.p[1];
        slicestep = (صحيح)m.step.p[0];
        slices = (صحيح)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    صحيح offset;
    صحيح slicestep;
    صحيح step;
    صحيح slices;
    صحيح rows;
    صحيح cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ساكن uint64 crc64( ثابت uchar* data, size_t size, uint64 crc0=0 )
{
    ساكن uint64 table[256];
    ساكن منطقي initialized = خطأ;

    إذا( !initialized )
    {
        لكل( صحيح i = 0; i < 256; i++ )
        {
            uint64 c = i;
            لكل( صحيح j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = true;
    }

    uint64 crc = ~crc0;
    لكل( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    أرجع ~crc;
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
بنية OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    تعريفنوع std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(عدم, "Initializing OpenCL cache configuration...");
        إذا (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(عدم, "OpenCL cache is disabled");
            أرجع;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        إذا (cache_path_.empty())
        {
            CV_LOG_INFO(عدم, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        افعل
        {
            حاول
            {
                إذا (cache_path_.empty())
                    اكسر;
                إذا (cache_path_ == "disabled")
                    اكسر;
                إذا (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(عدم, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    اكسر;
                }

                إذا (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    إذا (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(عدم, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        إذا (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(عدم, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            اكسر;
                        }
                    }

                    حاول
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... Done!");
                    }
                    امسك (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    امسك (...)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                وإلا
                {
                    إذا (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(عدم, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    وإلا
                    {
                        CV_LOG_INFO(عدم, "OpenCL cache lock is disabled");
                    }
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } طالما (0);
        إذا (!cache_path_.empty())
        {
            إذا (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(عدم, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    فراغ clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_prefix,
            ثابت std::string& cleanup_prefix)
    {
        إذا (cache_path_.empty())
            أرجع std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        إذا (found_it != prepared_contexts_.end())
            أرجع found_it->second;

        CV_LOG_INFO(عدم, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        منطقي result = utils::fs::isDirectory(target_directory);
        إذا (!result)
        {
            حاول
            {
                CV_LOG_VERBOSE(عدم, 0, "Creating directory: " << target_directory);
                إذا (utils::fs::createDirectories(target_directory))
                {
                    result = true;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "Can't create directory: " << target_directory);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        إذا (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            حاول
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, خطأ, true);
                std::vector<String> remove_entries;
                لكل (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    إذا (0 == name.find(cleanup_prefix))
                    {
                        إذا (0 == name.find(ctx_prefix))
                            استمر; // skip current
                        remove_entries.push_back(name);
                    }
                }
                إذا (!remove_entries.empty())
                {
                    CV_LOG_WARNING(عدم, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(عدم, "Trying to remove these directories...");
                    لكل (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(عدم, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(عدم, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    لكل (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ثابت String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        حاول
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(عدم, "Removed: " << path);
                        }
                        امسك (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(عدم, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(عدم, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        أرجع target_directory;
    }

    ساكن OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, جديد OpenCLBinaryCacheConfigurator());
    }
};
صنف دووانەیی_Program_File
{
    تعداد { MAX_ENTRIES = 64 };

    تعريفنوع unsigned صحيح uint32_t;

    بنية CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    بنية CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t ژمار_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    بنية CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t کلیل_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت sourceSignature_;
    ثابت size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        أرجع hash & (MAX_ENTRIES - 1);
    }

    مضمن size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        أرجع fileSize;
    }
    مضمن uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.اقرأ((حرف*)&res, حجم(uint32_t));
        CV_Assert(!f.fail());
        أرجع res;
    }
    مضمن فراغ writeUInt32(ثابت uint32_t value)
    {
        uint32_t v = value;
        f.اكتب((حرف*)&v, حجم(uint32_t));
        CV_Assert(!f.fail());
    }

    مضمن فراغ seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    مضمن فراغ seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    مضمن فراغ seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    فراغ clearFile()
    {
        f.close();
        إذا (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(عدم, "Can't remove: " << fileName_);
        أرجع;
    }

عام:
    دووانەیی_Program_File(ثابت std::string& fileName, ثابت حرف* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != عدم);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, حجم(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        إذا(f.is_open() && getFileSize() > 0)
        {
            منطقي isValid = خطأ;
            حاول
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                إذا (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<حرف> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.اقرأ(fileSourceSignature.data(), fileSourceSignatureSize);
                    إذا (f.eof())
                    {
                        CV_LOG_ERROR(عدم, "Unexpected EOF");
                    }
                    وإلا إذا (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = true;
                    }
                }
                إذا (!isValid)
                {
                    CV_LOG_ERROR(عدم, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            امسك (...)
            {
                CV_LOG_ERROR(عدم, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            إذا (!isValid)
            {
                clearFile();
            }
            وإلا
            {
                seekReadAbsolute(0);
            }
        }
    }

    منطقي اقرأ(ثابت std::string& کلیل, std::vector<حرف>& buf)
    {
        إذا (!f.is_open())
            أرجع خطأ;

        size_t fileSize = getFileSize();
        إذا (fileSize == 0)
        {
            CV_LOG_ERROR(عدم, "Invalid file (empty): " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t ژمار_Of_Entries = readUInt32();
        CV_Assert(ژمار_Of_Entries > 0);
        إذا (ژمار_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        f.اقرأ((حرف*)&entryOffsets[0], حجم(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ژمارە = getHash(کلیل);

        uint32_t entryOffset = entryOffsets[entry_ژمارە];
        FileEntry entry;
        طالما (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&entry, حجم(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<حرف> file_کلیل(entry.کلیل_Size + 1);
            إذا (کلیل.size() == entry.کلیل_Size)
            {
                إذا (entry.کلیل_Size > 0)
                {
                    f.اقرأ(file_کلیل.data(), entry.کلیل_Size);
                    CV_Assert(!f.fail());
                }
                إذا (memcmp(file_کلیل.data(), کلیل.c_str(), entry.کلیل_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.اقرأ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(عدم, 0, "Read...");
                    أرجع true;
                }
            }
            إذا (entry.nextEntryFileOffset == 0)
                اكسر;
            entryOffset = entry.nextEntryFileOffset;
        }
        أرجع خطأ;
    }

    منطقي اكتب(ثابت std::string& کلیل, std::vector<حرف>& buf)
    {
        إذا (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            إذا (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                إذا (!f.is_open())
                {
                    CV_LOG_ERROR(عدم, "Can't create file: " << fileName_);
                    أرجع خطأ;
                }
            }
        }

        size_t fileSize = getFileSize();
        إذا (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.اكتب(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, حجم(entryOffsets));
            f.اكتب((حرف*)entryOffsets, حجم(entryOffsets));
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

        uint32_t ژمار_Of_Entries = readUInt32();
        CV_Assert(ژمار_Of_Entries > 0);
        إذا (ژمار_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.اقرأ((حرف*)&entryOffsets[0], حجم(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_ژمارە = getHash(کلیل);

        uint32_t entryOffset = entryOffsets[entry_ژمارە];
        FileEntry entry;
        طالما (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&entry, حجم(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<حرف> file_کلیل(entry.کلیل_Size + 1);
            إذا (کلیل.size() == entry.کلیل_Size)
            {
                إذا (entry.کلیل_Size > 0)
                {
                    f.اقرأ(file_کلیل.data(), entry.کلیل_Size);
                    CV_Assert(!f.fail());
                }
                إذا (0 == memcmp(file_کلیل.data(), کلیل.c_str(), entry.کلیل_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(عدم, 0, "Duplicate key ignored: " << fileName_);
                    أرجع خطأ;
                }
            }
            إذا (entry.nextEntryFileOffset == 0)
                اكسر;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        إذا (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.اكتب((حرف*)&entry, حجم(entry));
            CV_Assert(!f.fail());
        }
        وإلا
        {
            entryOffsets[entry_ژمارە] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.اكتب((حرف*)entryOffsets, حجم(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.کلیل_Size = (uint32_t)کلیل.size();
        f.اكتب((حرف*)&entry, حجم(entry));
        CV_Assert(!f.fail());
        f.اكتب(کلیل.c_str(), entry.کلیل_Size);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(عدم, 0, "Write... (" << buf.size() << " bytes)");
        أرجع true;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



بنية OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحيح device_;  // device index in context
    ocl::Queue queue_;
    صحيح useOpenCL_;

محمي:
    Impl() = delete;

    فراغ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي found = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            إذا (dhandle == deviceID)
            {
                device_ = i;
                found = true;
                اكسر;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    فراغ _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي found = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            إذا (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = true;
                اكسر;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

عام:
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

    Impl(ثابت ocl::Context& context, ثابت صحيح device, ثابت ocl::Queue& queue)
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

    مضمن منطقي useOpenCL() ثابت { أرجع const_cast<Impl*>(هذا)->useOpenCL(); }
    منطقي useOpenCL()
    {
        إذا (useOpenCL_ < 0)
        {
            حاول
            {
                useOpenCL_ = 0;
                إذا (!context_.empty() && context_.ndevices() > 0)
                {
                    ثابت Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            امسك (ثابت cv::Exception&)
            {
                // nothing
            }
            إذا (!useOpenCL_)
                CV_LOG_INFO(عدم, "OpenCL: can't use OpenCL execution context");
        }
        أرجع useOpenCL_ > 0;
    }

    فراغ setUseOpenCL(منطقي flag)
    {
        إذا (!flag)
            useOpenCL_ = 0;
        وإلا
            useOpenCL_ = -1;
    }

    ساكن ثابت std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(عدم, "OpenCL: initializing thread execution context");

        ساكن منطقي initialized = خطأ;
        ساكن std::shared_ptr<Impl> g_primaryExecutionContext;

        إذا (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            إذا (!initialized)
            {
                CV_LOG_INFO(عدم, "OpenCL: creating new execution context...");
                حاول
                {
                    Context c = ocl::Context::create(std::string());
                    إذا (c.ndevices())
                    {
                        صحيح deviceId = 0;
                        auto& d = c.device(deviceId);
                        إذا (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            إذا (!q.ptr())
                            {
                                CV_LOG_ERROR(عدم, "OpenCL: Can't create default OpenCL queue");
                            }
                            وإلا
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(عدم, "OpenCL: device=" << d.name());
                            }
                        }
                        وإلا
                        {
                            CV_LOG_ERROR(عدم, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    وإلا
                    {
                        CV_LOG_INFO(عدم, "OpenCL: context is not available/disabled");
                    }
                }
                امسك (ثابت std::exception& e)
                {
                    CV_LOG_INFO(عدم, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                امسك (...)
                {
                    CV_LOG_WARNING(عدم, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = true;
            }
        }
        أرجع g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ثابت
{
    CV_Assert(p);
    أرجع p->context_;
}
Device& OpenCLExecutionContext::getDevice() ثابت
{
    CV_Assert(p);
    أرجع p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ثابت
{
    CV_Assert(p);
    أرجع p->queue_;
}

منطقي OpenCLExecutionContext::useOpenCL() ثابت
{
    إذا (p)
        أرجع p->useOpenCL();
    أرجع خطأ;
}
فراغ OpenCLExecutionContext::setUseOpenCL(منطقي flag)
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
    إذا (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = true;
        إذا (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    أرجع c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    أرجع c;
}

فراغ OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *هذا;
    data.oclExecutionContextInitialized = true;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت Queue q(getContext(), getDevice());
    أرجع cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ثابت ocl::Queue& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != عدم);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    أرجع c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    أرجع ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device)
{
    CV_TRACE_FUNCTION();
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    أرجع ctx;

}

فراغ OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ساكن منطقي g_isOpenCLInitialized = خطأ;
ساكن منطقي g_isOpenCLAvailable = خطأ;

منطقي haveOpenCL()
{
    CV_TRACE_FUNCTION();

    إذا (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        إذا (!envPath.empty())
        {
            إذا (envPath == "disabled")
            {
                g_isOpenCLAvailable = خطأ;
                g_isOpenCLInitialized = true;
                أرجع خطأ;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(عدم, "Initialize OpenCL runtime...");
        حاول
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, عدم, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(عدم, "OpenCL: found " << n << " platforms");
        }
        امسك (...)
        {
            g_isOpenCLAvailable = خطأ;
        }
        g_isOpenCLInitialized = true;
    }
    أرجع g_isOpenCLAvailable;
}

منطقي useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    إذا (data.useOpenCL < 0)
    {
        حاول
        {
            data.useOpenCL = 0;
            إذا (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    أرجع data.useOpenCL > 0;
}

منطقي isOpenCLActivated()
{
    إذا (!g_isOpenCLAvailable)
        أرجع خطأ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    أرجع useOpenCL();
}

فراغ setUseOpenCL(منطقي flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    إذا (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    وإلا
    {
        إذا (!flag)
            data.useOpenCL = 0;
        وإلا
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

صنف AmdBlasHelper
{
عام:
    ساكن AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, جديد AmdBlasHelper())
    }

    منطقي isAvailable() ثابت
    {
        أرجع g_isAmdBlasAvailable;
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

محمي:
    AmdBlasHelper()
    {
        إذا (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            إذا (!g_isAmdBlasInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    امسك (...)
                    {
                        g_isAmdBlasAvailable = خطأ;
                    }
                }
                وإلا
                    g_isAmdBlasAvailable = خطأ;

                g_isAmdBlasInitialized = true;
            }
        }
    }

خاص:
    ساكن منطقي g_isAmdBlasInitialized;
    ساكن منطقي g_isAmdBlasAvailable;
};

منطقي AmdBlasHelper::g_isAmdBlasAvailable = خطأ;
منطقي AmdBlasHelper::g_isAmdBlasInitialized = خطأ;

منطقي haveAmdBlas()
{
    أرجع AmdBlasHelper::getInstance().isAvailable();
}

#وإلا

منطقي haveAmdBlas()
{
    أرجع خطأ;
}

#endif

#ifdef HAVE_CLAMDFFT

صنف AmdFftHelper
{
عام:
    ساكن AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, جديد AmdFftHelper())
    }

    منطقي isAvailable() ثابت
    {
        أرجع g_isAmdFftAvailable;
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

محمي:
    AmdFftHelper()
    {
        إذا (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            إذا (!g_isAmdFftInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = true;
                    }
                    امسك (ثابت Exception &)
                    {
                        g_isAmdFftAvailable = خطأ;
                    }
                }
                وإلا
                    g_isAmdFftAvailable = خطأ;

                g_isAmdFftInitialized = true;
            }
        }
    }

خاص:
    ساكن clfftSetupData setupData;
    ساكن منطقي g_isAmdFftInitialized;
    ساكن منطقي g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
منطقي AmdFftHelper::g_isAmdFftAvailable = خطأ;
منطقي AmdFftHelper::g_isAmdFftInitialized = خطأ;

منطقي haveAmdFft()
{
    أرجع AmdFftHelper::getInstance().isAvailable();
}

#وإلا

منطقي haveAmdFft()
{
    أرجع خطأ;
}

#endif

منطقي haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    أرجع true;
#وإلا
    أرجع خطأ;
#endif
}

فراغ finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

بنية Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = خطأ;
    }

    ~Impl() {}

    فراغ init()
    {
        إذا( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            إذا( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            إذا( handle != 0 )
            {
                حرف buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, حجم(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = true;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    منطقي initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    إذا(p)
        p->release();
}

Platform::Platform(ثابت Platform& pl)
{
    p = (Impl*)pl.p;
    إذا(p)
        p->addref();
}

Platform& Platform::operator = (ثابت Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    إذا (هذا != &pl) {
        إذا(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    أرجع *هذا;
}

فراغ* Platform::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(عدم, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ساكن Platform p;
    إذا( !p.p )
    {
        p.p = جديد Impl;
        p.p->init();
    }
    أرجع p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ساكن فراغ parseOpenCLVersion(ثابت String &version, صحيح &major, صحيح &minor)
{
    major = minor = 0;
    إذا (10 >= version.length())
        أرجع;
    ثابت حرف *pstr = version.c_str();
    إذا (0 != strncmp(pstr, "OpenCL ", 7))
        أرجع;
    size_t ppos = version.find('.', 7);
    إذا (String::npos == ppos)
        أرجع;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

بنية Device::Impl
{
    Impl(فراغ* d)
        : refcount(1)
        , handle(0)
    {
        حاول
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        امسك (...)
        {
            ارم;
        }
    }

    فراغ _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, صحيح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, صحيح>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, صحيح>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        طالما (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            إذا (pos2 == String::npos)
                pos2 = extensions_.size();
            إذا (pos2 > pos)
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
        إذا (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        وإلا إذا (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وإلا إذا (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وإلا
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        إذا (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ثابت size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            إذا (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(عدم, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#إذا 0
        إذا (isExtensionSupported("cl_khr_spir"))
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
        إذا (!cv::__termination)
#endif
        {
            إذا (handle)
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

        أرجع clGetDeviceInfo(handle, prop, حجم(temp), &temp, &sz) == CL_SUCCESS &&
            sz == حجم(temp) ? _TpOut(temp) : _TpOut();
    }

    منطقي getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(temp), &temp, &sz) == CL_SUCCESS &&
            sz == حجم(temp) ? temp != 0 : خطأ;
    }

    String getStrProp(cl_device_info prop) ثابت
    {
        حرف buf[4096];
        size_t sz=0;
        أرجع clGetDeviceInfo(handle, prop, حجم(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < حجم(buf) ? String(buf) : String();
    }

    منطقي isExtensionSupported(ثابت std::string& extensionName) ثابت
    {
        أرجع extensions_set_.ژماردن_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    صحيح doubleFPConfig_;
    منطقي khr_fp64_support_;
    صحيح halfFPConfig_;
    منطقي khr_fp16_support_;
    منطقي hostUnifiedMemory_;
    صحيح maxComputeUnits_;
    size_t maxWorkGroupSize_;
    صحيح type_;
    صحيح addressBits_;
    صحيح deviceVersionMajor_;
    صحيح deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    صحيح vendorID_;
    منطقي intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(فراغ* d)
{
    p = 0;
    set(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    إذا(p)
        p->addref();
}

Device& Device::operator = (ثابت Device& d)
{
    Impl* newp = (Impl*)d.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    إذا (هذا != &d) {
        إذا(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    أرجع *هذا;
}

Device::~Device()
{
    إذا(p)
        p->release();
}

فراغ Device::set(فراغ* d)
{
    إذا(p)
        p->release();
    p = جديد Impl(d);
    إذا (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(فراغ* d)
{
    Device device(d);
    أرجع device;
}

فراغ* Device::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

String Device::name() ثابت
{ أرجع p ? p->name_ : String(); }

String Device::extensions() ثابت
{ أرجع p ? String(p->extensions_) : String(); }

منطقي Device::isExtensionSupported(ثابت String& extensionName) ثابت
{ أرجع p ? p->isExtensionSupported(extensionName) : خطأ; }

String Device::version() ثابت
{ أرجع p ? p->version_ : String(); }

String Device::vendorName() ثابت
{ أرجع p ? p->vendorName_ : String(); }

صحيح Device::vendorID() ثابت
{ أرجع p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

صحيح Device::deviceVersionMajor() ثابت
{ أرجع p ? p->deviceVersionMajor_ : 0; }

صحيح Device::deviceVersionMinor() ثابت
{ أرجع p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ثابت
{ أرجع p ? p->driverVersion_ : String(); }

صحيح Device::type() ثابت
{ أرجع p ? p->type_ : 0; }

صحيح Device::addressBits() ثابت
{ أرجع p ? p->addressBits_ : 0; }

منطقي Device::available() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : خطأ; }

منطقي Device::compilerAvailable() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : خطأ; }

منطقي Device::linkerAvailable() ثابت
#ifdef CL_VERSION_1_2
{ أرجع p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : خطأ; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

صحيح Device::doubleFPConfig() ثابت
{ أرجع p ? p->doubleFPConfig_ : 0; }

صحيح Device::singleFPConfig() ثابت
{ أرجع p ? p->getProp<cl_device_fp_config, صحيح>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

صحيح Device::halfFPConfig() ثابت
{ أرجع p ? p->halfFPConfig_ : 0; }

منطقي Device::hasFP64() ثابت
{ أرجع p ? p->khr_fp64_support_ : خطأ; }
منطقي Device::hasFP16() ثابت
{ أرجع p ? p->khr_fp16_support_ : خطأ; }

منطقي Device::endianLittle() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : خطأ; }

منطقي Device::errorCorrectionSupport() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : خطأ; }

صحيح Device::executionCapabilities() ثابت
{ أرجع p ? p->getProp<cl_device_exec_capabilities, صحيح>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

صحيح Device::globalMemCacheType() ثابت
{ أرجع p ? p->getProp<cl_device_mem_cache_type, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحيح Device::global_Mem_Cache_هێڵ_Size() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

صحيح Device::localMemType() ثابت
{ أرجع p ? p->getProp<cl_device_local_mem_type, صحيح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقي Device::hostUnifiedMemory() ثابت
{ أرجع p ? p->hostUnifiedMemory_ : خطأ; }

منطقي Device::imageSupport() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : خطأ; }

منطقي Device::imageFromBufferSupport() ثابت
{
    أرجع p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : خطأ;
}

uint Device::imagePitchAlignment() ثابت
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#endif
}

uint Device::imageBaseAddressAlignment() ثابت
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#endif
}

size_t Device::image2DMaxWidth() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ثابت
#ifdef CL_VERSION_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ثابت
#ifdef CL_VERSION_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

منطقي Device::intelSubgroupsSupport() ثابت
{ أرجع p ? p->intelSubgroupsSupport_ : خطأ; }

صحيح Device::maxClockFrequency() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحيح Device::maxComputeUnits() ثابت
{ أرجع p ? p->maxComputeUnits_ : 0; }

صحيح Device::maxConstantArgs() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

صحيح Device::maxReadImageArgs() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

صحيح Device::maxWriteImageArgs() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

صحيح Device::maxSamplers() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ثابت
{ أرجع p ? p->maxWorkGroupSize_ : 0; }

صحيح Device::maxWorkItemDims() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

فراغ Device::maxWorkItemSizes(size_t* sizes) ثابت
{
    إذا(p)
    {
        ثابت صحيح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحيح Device::memBaseAddrAlign() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

صحيح Device::nativeVectorWidthChar() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

صحيح Device::nativeVectorWidthShort() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

صحيح Device::nativeVectorWidthInt() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

صحيح Device::nativeVectorWidthLong() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

صحيح Device::nativeVectorWidthFloat() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

صحيح Device::nativeVectorWidthDouble() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

صحيح Device::nativeVectorWidthHalf() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

صحيح Device::preferredVectorWidthChar() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

صحيح Device::preferredVectorWidthShort() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

صحيح Device::preferredVectorWidthInt() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

صحيح Device::preferredVectorWidthLong() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

صحيح Device::preferredVectorWidthFloat() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

صحيح Device::preferredVectorWidthDouble() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

صحيح Device::preferredVectorWidthHalf() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ثابت
#ifdef CL_VERSION_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        أرجع c.getDevice();
    }

    ساكن Device dummy;
    أرجع dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
مضمن cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, عدم, &required);
    إذا (err != CL_SUCCESS)
        أرجع err;

    param.clear();
    إذا (required > 0)
    {
        AutoBuffer<حرف> buf(required + 1);
        حرف* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, عدم);
        إذا (err != CL_SUCCESS)
            أرجع err;
        param = ptr;
    }

    أرجع CL_SUCCESS;
}

ساكن فراغ split(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    إذا (s.size() == 0)
        أرجع;
    std::istringstream ss(s);
    std::string item;
    طالما (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ساكن منطقي parseOpenCLDeviceConfiguration(ثابت std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    إذا (parts.size() > 3)
    {
        CV_LOG_ERROR(عدم, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        أرجع خطأ;
    }
    إذا (parts.size() > 2)
        deviceNameOrID = parts[2];
    إذا (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    إذا (parts.size() > 0)
    {
        platform = parts[0];
    }
    أرجع true;
}

ساكن cl_device_id selectOpenCLDevice(ثابت std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    إذا (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    إذا (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        أرجع عدم;

    منطقي isID = خطأ;
    صحيح deviceID = -1;
    إذا (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = true;
        لكل (size_t i = 0; i < deviceName.length(); i++)
        {
            إذا (!isdigit(deviceName[i]))
            {
                isID = خطأ;
                اكسر;
            }
        }
        إذا (isID)
        {
            deviceID = atoi(deviceName.c_str());
            إذا (deviceID < 0)
                أرجع عدم;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint ژمارە_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &ژمارە_Platforms));

        إذا (ژمارە_Platforms == 0)
            أرجع عدم;
        platforms.resize((size_t)ژمارە_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(ژمارە_Platforms, &platforms[0], &ژمارە_Platforms));
        platforms.resize(ژمارە_Platforms);
    }

    إذا (platform.length() > 0)
    {
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            إذا (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            وإلا
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        إذا (platforms.size() == 0)
        {
            CV_LOG_ERROR(عدم, "OpenCL: Can't find OpenCL platform by name: " << platform);
            اذهب not_found;
        }
    }
    إذا (deviceTypes.size() == 0)
    {
        إذا (!isID)
        {
            deviceTypes.push_back("GPU");
            إذا (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        وإلا
            deviceTypes.push_back("ALL");
    }
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
    {
        صحيح deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        إذا (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        وإلا إذا (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        وإلا إذا (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وإلا إذا (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        وإلا
        {
            CV_LOG_ERROR(عدم, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            اذهب not_found;
        }

        std::vector<cl_device_id> devices;
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ژماردن_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, عدم, &ژماردن_2);
            إذا (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            إذا (ژماردن_2 == 0)
                استمر;
            size_t base = devices.size();
            devices.resize(base + ژماردن_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, ژماردن_2, &devices[base], &ژماردن_2);
            إذا (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        لكل (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : true) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = true;
            إذا(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, حجم(isIGPU), &isIGPU, عدم));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            إذا ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                أرجع devices[i];
            }
        }
    }

not_found:
    إذا (configuration.empty())
        أرجع عدم; // suppress messages on stderr

    std::ostringstream پەیام;
    پەیام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
        پەیام << ' ' << deviceTypes[t];

    پەیام << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(عدم, پەیام.str());
    أرجع عدم;
}

#ifdef HAVE_OPENCL_SVM
نطاق svm {

تعداد AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

ساكن منطقي checkForceSVMUmatUsage()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", خطأ);
        initialized = true;
    }
    أرجع force;
}
ساكن منطقي checkDisableSVMUMatUsage()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", خطأ);
        initialized = true;
    }
    أرجع force;
}
ساكن منطقي checkDisableSVM()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", خطأ);
        initialized = true;
    }
    أرجع force;
}
// see SVMCapabilities
ساكن unsigned صحيح getSVMCapabilitiesMask()
{
    ساكن منطقي initialized = خطأ;
    ساكن unsigned صحيح mask = 0;
    إذا (!initialized)
    {
        ثابت std::string env_بەها = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        إذا (env_بەها.empty())
        {
            أرجع ~0U; // all bits 1
        }
        mask = atoi(env_بەها.c_str());
        initialized = true;
    }
    أرجع mask;
}
} // namespace
#endif

ساكن size_t get_Program_ژماردن_Limit()
{
    ساكن منطقي initialized = خطأ;
    ساكن size_t ژماردن_2 = 0;
    إذا (!initialized)
    {
        ژماردن_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = true;
    }
    أرجع ژماردن_2;
}

ساكن صحيح g_contextId = 0;

صنف OpenCLBufferPoolImpl;
صنف OpenCLSVMBufferPoolImpl;

بنية Context::Impl
{
    ساكن Context::Impl* get(Context& context) { أرجع context.getImpl(); }

    تعريفنوع std::deque<Context::Impl*> container_t;
    ساكن container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ساكن container_t* g_contexts = جديد container_t();
        أرجع *g_contexts;
    }

محمي:
    Impl(ثابت std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(خطأ)
#endif
    {
        إذا (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = هذا;
    }

    ~Impl()
    {
#ifdef _WIN32
        إذا (!cv::__termination)
#endif
        {
            إذا (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = عدم;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = عدم;
        }
    }

    فراغ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_ژمارە_DEVICES, حجم(ndevices), &ndevices, عدم));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        لكل (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    فراغ __init_buffer_pools();  // w/o synchronization
    فراغ _init_buffer_pools() ثابت
    {
        إذا (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            إذا (!bufferPool_)
            {
                const_cast<Impl*>(هذا)->__init_buffer_pools();
            }
        }
    }
عام:
    ساكن Impl* findContext(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        إذا (configuration.empty() && !container.empty())
            أرجع container[0];
        لكل (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            إذا (i && i->configuration == configuration)
            {
                أرجع i;
            }
        }
        أرجع عدم;
    }

    ساكن Impl* findOrCreateContext(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        إذا (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            إذا (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        إذا (impl)
        {
            CV_LOG_INFO(عدم, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            أرجع impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        إذا (d == عدم)
            أرجع عدم;

        impl = جديد Impl(configuration);
        حاول
        {
            impl->createFromDevice(d);
            إذا (impl->handle)
                أرجع impl;
            delete impl;
            أرجع عدم;
        }
        امسك (...)
        {
            delete impl;
            ارم;
        }
    }

    ساكن Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (فراغ*)h);
        Impl* impl = findContext(configuration);
        إذا (impl)
        {
            CV_LOG_INFO(عدم, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            أرجع impl;
        }

        impl = جديد Impl(configuration);
        حاول
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            أرجع impl;
        }
        امسك (...)
        {
            delete impl;
            ارم;
        }
    }

    ساكن Impl* findOrCreateContext(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (فراغ*)d);
        Impl* impl = findContext(configuration);
        إذا (impl)
        {
            CV_LOG_INFO(عدم, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            أرجع impl;
        }

        impl = جديد Impl(configuration);
        حاول
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            أرجع impl;
        }
        امسك (...)
        {
            delete impl;
            ارم;
        }
    }

    فراغ setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        إذا (d == عدم)
            أرجع;

        createFromDevice(d);
    }

    فراغ createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == عدم);

        cl_platform_id pl = عدم;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, حجم(cl_platform_id), &pl, عدم));

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

        منطقي ok = handle != 0 && status == CL_SUCCESS;
        إذا( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        وإلا
            handle = عدم;
    }

    Program getProg(ثابت ProgramSource& src, ثابت String& buildflags, String& errmsg);

    فراغ unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        لكل (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              إذا (it != phash.end())
              {
                  إذا (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      أرجع;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        إذا (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            إذا (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحيح bits = d.addressBits();
                إذا (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                لكل (size_t i = 0; i < prefix.size(); i++)
                {
                    حرف c = prefix[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        أرجع prefix;
    }

    std::string& getPrefixBase()
    {
        إذا (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            إذا (prefix_base.empty())
            {
                ثابت Device& d = devices[0];
                صحيح bits = d.addressBits();
                إذا (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                لكل (size_t i = 0; i < prefix_base.size(); i++)
                {
                    حرف c = prefix_base[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        أرجع prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحيح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    تعريفنوع std::map<std::string, Program> phash_t;
    phash_t phash;
    تعريفنوع std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        أرجع *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        أرجع *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    فراغ setUserContext(std::type_index typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        إذا (it != userContextStorage.end())
            أرجع it->second;
        وإلا
            أرجع nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    منطقي svmInitialized;
    منطقي svmAvailable;
    منطقي svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    فراغ svmInit()
    {
        CV_Assert(handle != عدم);
        ثابت Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((فراغ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, حجم(deviceCaps), &deviceCaps, عدم);
        إذا (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            اذهب noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (صحيح)deviceCaps);
        CV_Assert(((فراغ)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.بەها_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.بەها_2 &= svm::getSVMCapabilitiesMask();
        إذا (svmCapabilities.بەها_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            اذهب noSVM;
        }
        حاول
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            فراغ* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            إذا (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            حاول
            {
                منطقي error = خطأ;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                إذا (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                حاول
                {
                    ((صحيح*)ptr)[0] = 100;
                }
                امسك (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = true;
                }
                إذا (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                إذا (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ارم;
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
        امسك (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            حاول
            {
                // Try HSA extension
                String extensions = device.extensions();
                إذا (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    اذهب noSVM;
                }
                cl_platform_id p = عدم;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, حجم(cl_platform_id), &p, عدم));
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
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                اذهب noSVM;
            }
        }

        svmAvailable = true;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = true;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        أرجع;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = خطأ;
        svmEnabled = خطأ;
        svmCapabilities.بەها_2 = 0;
        svmInitialized = true;
        svmFunctions.fn_clSVMAlloc = عدم;
        أرجع;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ثابت
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        أرجع *bufferPoolSVM_.get();
    }
#endif

    friend صنف Program;
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
Context::Context(صحيح dtype)
{
    p = 0;
    create(dtype);
}

فراغ Context::release()
{
    إذا (p)
    {
        p->release();
        p = عدم;
    }
}

منطقي Context::create()
{
    release();
    إذا (!haveOpenCL())
        أرجع خطأ;
    p = Impl::findOrCreateContext(std::string());
    إذا (p && p->handle)
        أرجع true;
    release();
    أرجع خطأ;
}

// deprecated
منطقي Context::create(صحيح dtype)
{
    إذا( !haveOpenCL() )
        أرجع خطأ;
    release();
    إذا (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    وإلا
    {
        CV_LOG_ERROR(عدم, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    إذا (p && !p->handle)
    {
        release();
    }
    أرجع p != 0;
}

Context::Context(ثابت Context& c)
{
    p = (Impl*)c.p;
    إذا(p)
        p->addref();
}

Context& Context::operator = (ثابت Context& c)
{
    Impl* newp = (Impl*)c.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    إذا (هذا != &c) {
        إذا(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    أرجع *هذا;
}

فراغ* Context::ptr() ثابت
{
    أرجع p == عدم ? عدم : p->handle;
}

size_t Context::ndevices() ثابت
{
    أرجع p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ثابت
{
    ساكن Device dummy;
    أرجع !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        auto& ctx = c.getContext();
        أرجع ctx;
    }

    ساكن Context dummy;
    أرجع dummy;
}

Program Context::getProg(ثابت ProgramSource& prog,
                         ثابت String& buildopts, String& errmsg)
{
    أرجع p ? p->getProg(prog, buildopts, errmsg) : Program();
}

فراغ Context::unloadProg(Program& prog)
{
    إذا (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(فراغ* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    أرجع ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    أرجع ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    أرجع ctx;
}

فراغ* Context::getOpenCLContextProperty(صحيح propertyId) ثابت
{
    إذا (p == عدم)
        أرجع nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, عدم, &size));
    std::vector<cl_context_properties> prop(size / حجم(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), عدم));
    لكل (size_t i = 0; i < prop.size(); i += 2)
    {
        إذا (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(عدم, "OpenCL: found context property=" << propertyId << ") => " << (فراغ*)prop[i + 1]);
            أرجع (فراغ*)prop[i + 1];
        }
    }
    أرجع nullptr;
}

#ifdef HAVE_OPENCL_SVM
منطقي Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmEnabled;
}
فراغ Context::setUseSVM(منطقي enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    إذا (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#وإلا
منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::setUseSVM(منطقي enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
نطاق svm {

ثابت SVMCapabilities getSVMCapabilitites(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmCapabilities;
}

CV_EXPORTS ثابت SVMFunctions* getSVMFunctions(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != عدم);
    أرجع &i->svmFunctions;
}

CV_EXPORTS منطقي useSVM(UMatUsageFlags usageFlags)
{
    إذا (checkForceSVMUmatUsage())
        أرجع true;
    إذا (checkDisableSVMUMatUsage())
        أرجع خطأ;
    إذا ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        أرجع true;
    أرجع خطأ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

فراغ Context::setUserContext(std::type_index typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    أرجع p->getUserContext(typeId);
}

ساكن فراغ get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<حرف> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ثابت std::string& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID
)
{
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    إذا (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    منطقي platformAvailable = خطأ;

    // check if external platformName contained in list of available platforms in OpenCV
    لكل (unsigned صحيح i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        إذا (platformName == availablePlatformName)
        {
            platformAvailable = true;
            اكسر;
        }
    }

    إذا (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    إذا (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    أرجع ctx;
}

فراغ initializeContextFromHandle(Context& ctx, فراغ* _platform, فراغ* _context, فراغ* _device)
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

بنية Queue::Impl
{
    مضمن فراغ __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = خطأ;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, حجم(cl_command_queue_properties), &props, عدم));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, منطقي isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ثابت Context& c, ثابت Device& d, منطقي withProfiling = خطأ)
    {
        __init();

        ثابت Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        إذا( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        إذا( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        إذا (!cv::__termination)
#endif
        {
            إذا(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = عدم;
            }
        }
    }

    ثابت cv::ocl::Queue& getProfilingQueue(ثابت cv::ocl::Queue& self)
    {
        إذا (isProfilingQueue_)
            أرجع self;

        إذا (profiling_queue_.ptr())
            أرجع profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, حجم(cl_context), &ctx, عدم));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, حجم(cl_device_id), &device, عدم));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = جديد Impl(q, true);
        profiling_queue_ = queue;

        أرجع profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    منطقي isProfilingQueue_;
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
    إذا(p)
        p->addref();
}

Queue& Queue::operator = (ثابت Queue& q)
{
    Impl* newp = (Impl*)q.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    إذا (هذا != &q) {
        إذا(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    أرجع *هذا;
}

Queue::~Queue()
{
    إذا(p)
        p->release();
}

منطقي Queue::create(ثابت Context& c, ثابت Device& d)
{
    إذا(p)
        p->release();
    p = جديد Impl(c, d);
    أرجع p->handle != 0;
}

فراغ Queue::finish()
{
    إذا(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ثابت Queue& Queue::getProfilingQueue() ثابت
{
    CV_Assert(p);
    أرجع p->getProfilingQueue(*هذا);
}

فراغ* Queue::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        auto& q = c.getQueue();
        أرجع q;
    }
    ساكن Queue dummy;
    أرجع dummy;
}

ساكن cl_command_queue getQueue(ثابت Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    إذا(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    أرجع qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحيح _flags, UMat* _m, صحيح _wscale, صحيح _iwscale, ثابت فراغ* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != عدم);
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    CV_Assert(m.isContinuous());
    أرجع KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

بنية Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت Program& prog) :
        refcount(1), handle(عدم), isInProgress(خطأ), isAsyncRun(خطأ), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        إذا (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = خطأ;
        haveTempSrcUMats = خطأ;
    }

    فراغ cleanupUMats()
    {
        منطقي exceptionOccurred = خطأ;
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
        {
            إذا( u[i] )
            {
                إذا( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    حاول
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    امسك(ثابت std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(عدم, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = true;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = خطأ;
        haveTempSrcUMats = خطأ;
        CV_Assert(!exceptionOccurred);
    }

    فراغ addUMat(ثابت UMat& m, منطقي dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        إذا(dst && m.u->tempUMat())
            haveTempDstUMats = true;
        إذا(m.u->originalUMatData == عدم && m.u->tempUMat())
            haveTempSrcUMats = true;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    فراغ registerImageArgument(صحيح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        إذا (arg < (صحيح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    فراغ finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = خطأ;
        حاول
        {
            cleanupUMats();
        }
        امسك(...)
        {
            release();
            ارم;
        }
        release();
    }

    منطقي run(صحيح dims, size_t _globalsize[], size_t _localsize[],
            منطقي متزامن, int64* timeNS, ثابت Queue& q);

    ~Impl()
    {
        إذا(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    تعداد { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    منطقي isInProgress;
    منطقي isAsyncRun;  // true if kernel was scheduled in async mode
    صحيح nu;
    std::vector<Image2D> shadow_images;
    منطقي haveTempDstUMats;
    منطقي haveTempSrcUMats;
};

}} // namespace cv::ocl

خارجي "C" {

ساكن فراغ CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, فراغ *p)
{
    حاول
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    امسك (ثابت cv::Exception& exc)
    {
        CV_LOG_ERROR(عدم, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    امسك (ثابت std::exception& exc)
    {
        CV_LOG_ERROR(عدم, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    امسك (...)
    {
        CV_LOG_ERROR(عدم, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

نطاق cv { نطاق ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ثابت حرف* kname, ثابت Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت حرف* kname, ثابت ProgramSource& src,
               ثابت String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ثابت Kernel& k)
{
    p = k.p;
    إذا(p)
        p->addref();
}

Kernel& Kernel::operator = (ثابت Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    إذا (هذا != &k) {
        إذا(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    أرجع *هذا;
}

Kernel::~Kernel()
{
    إذا(p)
        p->release();
}

منطقي Kernel::create(ثابت حرف* kname, ثابت Program& prog)
{
    إذا(p)
        p->release();
    p = جديد Impl(kname, prog);
    إذا(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    أرجع p != 0;
}

منطقي Kernel::create(ثابت حرف* kname, ثابت ProgramSource& src,
                    ثابت String& buildopts, String* errmsg)
{
    إذا(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    إذا( !errmsg ) errmsg = &tempmsg;
    ثابت Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    أرجع create(kname, prog);
}

فراغ* Kernel::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

منطقي Kernel::empty() ثابت
{
    أرجع ptr() == 0;
}

ساكن cv::String dump_بەها(size_t sz, ثابت فراغ* p)
{
    إذا (!p)
        أرجع "NULL";
    إذا (sz == 2)
        أرجع cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    إذا (sz == 4)
        أرجع cv::format("%d / %uu / 0x%08x / %g", *(صحيح*)p, *(صحيح*)p, *(صحيح*)p, *(عائم*)p);
    إذا (sz == 8)
        أرجع cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(مزدوج*)p);
    أرجع cv::format("%p", p);
}

صحيح Kernel::set(صحيح i, ثابت فراغ* value, size_t sz)
{
    إذا (!p || !p->handle)
        أرجع -1;
    إذا (i < 0)
        أرجع i;
    إذا( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحيح)i, (صحيح)sz, dump_بەها(sz, value).c_str()).c_str());
    إذا (retval != CL_SUCCESS)
        أرجع -1;
    أرجع i+1;
}

صحيح Kernel::set(صحيح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحيح res = set(i, &h, حجم(h));
    إذا (res >= 0)
        p->registerImageArgument(i, image2D);
    أرجع res;
}

صحيح Kernel::set(صحيح i, ثابت UMat& m)
{
    أرجع set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساكن منطقي isCudaBacked(ثابت UMatData* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    أرجع u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحيح Kernel::set(صحيح i, ثابت KernelArg& arg)
{
    إذا( !p || !p->handle )
        أرجع -1;
    إذا (i < 0)
    {
        CV_LOG_ERROR(عدم, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (صحيح)i));
        أرجع i;
    }
    إذا( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    إذا( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        منطقي ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        إذا (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)عدم;
            status = clSetKernelArg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحيح)i).c_str());
            أرجع i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        إذا (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(عدم, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (صحيح)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            أرجع -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        إذا (!h)
        {
            CV_LOG_ERROR(عدم, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (صحيح)i, (صحيح)arg.flags, arg.m));
            p->release();
            p = 0;
            أرجع -1;
        }

#ifdef HAVE_OPENCL_SVM
        إذا ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#إذا 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#وإلا
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحيح)i, (فراغ*)svmDataPtr).c_str());
        }
        وإلا
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحيح)i, (فراغ*)h).c_str());
        }

        إذا (ptronly)
        {
            i++;
        }
        وإلا إذا( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), حجم(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u2d.offset).c_str());
            i += 3;

            إذا( !(arg.flags & KernelArg::NO_SIZE) )
            {
                صحيح cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, حجم(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), حجم(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)cols).c_str());
                i += 2;
            }
        }
        وإلا
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), حجم(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+3), (صحيح)u3d.offset).c_str());
            i += 4;
            إذا( !(arg.flags & KernelArg::NO_SIZE) )
            {
                صحيح cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), حجم(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), حجم(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        أرجع i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحيح)i, (صحيح)arg.sz, (فراغ*)arg.obj).c_str());
    أرجع i+1;
}

منطقي Kernel::run(صحيح dims, size_t _globalsize[], size_t _localsize[],
                 منطقي متزامن, ثابت Queue& q)
{
    إذا (!p)
        أرجع خطأ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != عدم);
    لكل (صحيح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحيح)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        إذا (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned صحيح)val) * val;
    }
    CV_Assert(total > 0);

    أرجع p->run(dims, globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::run_(صحيح dims, size_t _globalsize[], size_t _localsize[],
                  منطقي متزامن, ثابت Queue& q)
{
    CV_Assert(p);
    أرجع p->run(dims, _globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::Impl::run(صحيح dims, size_t globalsize[], size_t localsize[],
        منطقي متزامن, int64* timeNS, ثابت Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    إذا (!handle)
    {
        CV_LOG_ERROR(عدم, "OpenCL kernel has zero handle: " << name);
        أرجع خطأ;
    }

    إذا (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !متزامن;

    إذا (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#إذا CV_OPENCL_SYNC_RUN_KERNELS
    متزامن = true;
#endif

    cl_command_queue qq = getQueue(q);
    إذا (haveTempDstUMats)
        متزامن = true;
    إذا (haveTempSrcUMats)
        متزامن = true;
    إذا (timeNS)
        متزامن = true;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           عدم, globalsize, localsize, 0, 0,
                                           (متزامن && !timeNS) ? 0 : &asyncEvent);
#إذا !CV_OPENCL_SHOW_RUN_KERNELS
    إذا (retval != CL_SUCCESS)
#endif
    {
        cv::String پەیام = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحيح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        متزامن ? "true" : "false"
                        );
        إذا (retval != CL_SUCCESS)
        {
            پەیام = CV_OCL_API_ERROR_پەیام(retval, پەیام.c_str());
        }
#إذا CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, پەیام.c_str());
#وإلا
        printf("%s\n", پەیام.c_str());
        fflush(stdout);
#endif
    }
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        إذا (timeNS)
        {
            إذا (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, حجم(startTime), &startTime, عدم));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, حجم(stopTime), &stopTime, عدم));
                *timeNS = (int64)(stopTime - startTime);
            }
            وإلا
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    وإلا
    {
        addref();
        isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, هذا));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

منطقي Kernel::runTask(منطقي متزامن, ثابت Queue& q)
{
    إذا(!p || !p->handle || p->isInProgress)
        أرجع خطأ;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, متزامن ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), متزامن ? "true" : "false").c_str());
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    وإلا
    {
        p->addref();
        p->isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    منطقي res = p->run(dims, globalsize, localsize, true, &timeNs, profilingQueue);
    أرجع res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, حجم(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    أرجع status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, حجم(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    أرجع status == CL_SUCCESS ? val : 0;
}

منطقي Kernel::compileWorkGroupSize(size_t wsz[]) ثابت
{
    إذا(!p || !p->handle || !wsz)
        أرجع 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    أرجع status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, حجم(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    أرجع status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

بنية ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    تعداد KIND {
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
    فراغ init(تعداد KIND kind, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = عدم;
        sourceSize_ = 0;
        isHashUpdated = خطأ;
    }

    فراغ initFromSource(ثابت String& codeStr, ثابت String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        إذا (sourceHash_.empty())
        {
            updateHash();
        }
        وإلا
        {
            isHashUpdated = true;
        }
    }

    فراغ updateHash(ثابت حرف* hashStr = عدم)
    {
        إذا (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = true;
            أرجع;
        }
        uint64 hash = 0;
        بدل (kind_)
        {
        حالة PROGRAM_SOURCE_CODE:
            إذا (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            وإلا
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            اكسر;
        حالة PROGRAM_BINARIES:
        حالة PROGRAM_SPIR:
        حالة PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            اكسر;
        افتراضي:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = true;
    }

    Impl(تعداد KIND kind,
            ثابت String& module, ثابت String& name,
            ثابت unsigned حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    ساكن ProgramSource fromSourceWithStaticLifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* sourceCodeStaticStr, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جديد Impl(PROGRAM_SOURCE_CODE, module, name,
                (ثابت unsigned حرف*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        أرجع result;
    }

    ساكن ProgramSource from_دووانەیی(ثابت String& module, ثابت String& name,
            ثابت unsigned حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جديد Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        أرجع result;
    }

    ساكن ProgramSource fromSPIR(ثابت String& module, ثابت String& name,
            ثابت unsigned حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = جديد Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        أرجع result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ثابت unsigned حرف* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    منطقي isHashUpdated;

    friend بنية Program::Impl;
    friend بنية internal::ProgramEntry;
    friend بنية Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ثابت String& module, ثابت String& name, ثابت String& codeStr, ثابت String& codeHash)
{
    p = جديد Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ثابت حرف* prog)
{
    p = جديد Impl(prog);
}

ProgramSource::ProgramSource(ثابت String& prog)
{
    p = جديد Impl(prog);
}

ProgramSource::~ProgramSource()
{
    إذا(p)
        p->release();
}

ProgramSource::ProgramSource(ثابت ProgramSource& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ثابت ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    أرجع *هذا;
}

ثابت String& ProgramSource::source() ثابت
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == عدم); // method returns reference - can't construct temporary object
    أرجع p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_دووانەیی(ثابت String& module, ثابت String& name,
        ثابت unsigned حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دووانەیی(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت unsigned حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دووانەیی(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ثابت
{
    إذا (هذا->pProgramSource == عدم)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        إذا (هذا->pProgramSource == عدم)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(هذا->module, هذا->name, هذا->programCode, هذا->programHash, cv::String());
            ProgramSource* ptr = جديد ProgramSource(ps);
            const_cast<ProgramEntry*>(هذا)->pProgramSource = ptr;
        }
    }
    أرجع *هذا->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ساكن
cv::String joinBuildOptions(ثابت cv::String& a, ثابت cv::String& b)
{
    إذا (b.empty())
        أرجع a;
    إذا (a.empty())
        أرجع b;
    إذا (b[0] == ' ')
        أرجع a + b;
    أرجع a + (cv::String(" ") + b);
}

بنية Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت ProgramSource& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(عدم),
         buildflags(_buildflags)
    {
        ثابت ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        إذا (ctx.ptr() == عدم || device.ptr() == عدم)
            أرجع;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        إذا (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            إذا (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            وإلا إذا (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ثابت String param_buildExtraOptions = getBuildExtraOptions();
            إذا (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#إذا CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(عدم, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#إذا CV_OPENCL_SHOW_BUILD_KERNELS
        إذا (handle)
        {
            size_t retsz = 0;
            حرف kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, حجم(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            إذا (result == CL_SUCCESS && retsz < حجم(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(عدم, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    منطقي compile(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        إذا (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(عدم, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دووانەیی(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            أرجع isLoaded;
        }
        أرجع compileWithCache(ctx, src_, errmsg);
    }

    منطقي compileWithCache(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ثابت String& hash_str = src_->sourceHash_;
        cv::String fname;
        إذا (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        إذا (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            حاول
            {
                std::vector<حرف> دووانەیی_Buf;
                منطقي res = خطأ;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    دووانەیی_Program_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, دووانەیی_Buf);
                }
                إذا (res)
                {
                    CV_Assert(!دووانەیی_Buf.empty());
                    CV_LOG_VERBOSE(عدم, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقي isLoaded = create_From_دووانەیی(ctx, دووانەیی_Buf, errmsg);
                    إذا (isLoaded)
                        أرجع true;
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            امسك (...)
            {
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == عدم);
        إذا (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            إذا (!buildFromSources(ctx, src_, errmsg))
            {
                أرجع خطأ;
            }
        }
        وإلا إذا (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            إذا ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(عدم, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دووانەیی(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            إذا (!isLoaded)
                أرجع خطأ;
        }
        وإلا إذا (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وإلا
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != عدم);
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        إذا (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            حاول
            {
                std::vector<حرف> دووانەیی_Buf;
                get_Program_دووانەیی(دووانەیی_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    دووانەیی_Program_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, دووانەیی_Buf);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS
        إذا (CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS_بەها)
        {
            std::vector<حرف> دووانەیی_Buf;
            get_Program_دووانەیی(دووانەیی_Buf);
            إذا (!دووانەیی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = عدم;
                create_From_دووانەیی(ctx, دووانەیی_Buf, errmsg);
            }
        }
#endif
        أرجع handle != عدم;
    }

    فراغ dumpBuildLog_(cl_int result, ثابت cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<حرف, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        إذا (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            إذا (log_retval == CL_SUCCESS)
            {
                إذا (retsz < buffer.size())
                    buffer[retsz] = 0;
                وإلا
                    buffer[buffer.size() - 1] = 0;
            }
            وإلا
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

    منطقي buildFromSources(ثابت Context& ctx, ثابت ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(عدم, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ثابت حرف* srcptr = src_->sourceAddr_ ? ((ثابت حرف*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != عدم);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        إذا (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            لكل (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#إذا !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            إذا (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                إذا (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = عدم;
                }
                إذا (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#إذا CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS
            إذا (handle && CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS_بەها)
            {
                CV_LOG_INFO(عدم, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, حجم(kernels_buffer), &kernels_buffer[0], &retsz);
                إذا (retsz < حجم(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                وإلا
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(عدم, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        أرجع handle != عدم;
    }

    فراغ get_Program_دووانەیی(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_دووانەیی_SIZES, حجم(sz), &sz, عدم));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, حجم(ptr), &ptr, عدم));
    }

    منطقي create_From_دووانەیی(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        أرجع create_From_دووانەیی(ctx, (ثابت unsigned حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقي create_From_دووانەیی(ثابت Context& ctx, ثابت unsigned حرف* دووانەیی_Addr, ثابت size_t دووانەیی_Size, String& errmsg)
    {
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(عدم, 0, "Load from binary... (" << دووانەیی_Size << " bytes)");

        CV_Assert(دووانەیی_Size > 0);

        size_t ndevices = (صحيح)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ثابت uchar*> دووانەیی_Ptrs(ndevices);
        AutoBuffer<size_t> دووانەیی_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ثابت uchar** دووانەیی_Ptrs_2 = دووانەیی_Ptrs.data();
        size_t* دووانەیی_Sizes_2 = دووانەیی_Sizes.data();
        لكل (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            دووانەیی_Ptrs_2[i] = دووانەیی_Addr;
            دووانەیی_Sizes_2[i] = دووانەیی_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_دووانەیی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           دووانەیی_Sizes_2, دووانەیی_Ptrs_2, عدم, &result);
        إذا (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پەیام(result, "clCreateProgramWithBinary"));
            إذا (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = عدم;
            }
        }
        إذا (!handle)
        {
            أرجع خطأ;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            إذا (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = عدم;
                }
                أرجع خطأ;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    حجم(build_status), &build_status, &retsz));
            إذا (result == CL_SUCCESS)
            {
                إذا (build_status == CL_BUILD_SUCCESS)
                {
                    أرجع true;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "clGetProgramBuildInfo() returns " << build_status);
                    أرجع خطأ;
                }
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پەیام(result, "clGetProgramBuildInfo()"));
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = عدم;
                }
            }
        }
#إذا CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS
        إذا (handle && CV_OPENCL_VALIDATE_دووانەیی_PROGRAMS_بەها)
        {
            CV_LOG_INFO(عدم, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, حجم(kernels_buffer), &kernels_buffer[0], &retsz);
            إذا (retsz < حجم(kernels_buffer))
                kernels_buffer[retsz] = 0;
            وإلا
                kernels_buffer[0] = 0;
            CV_LOG_INFO(عدم, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        أرجع handle != عدم;
    }

    ~Impl()
    {
        إذا( handle )
        {
#ifdef _WIN32
            إذا (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = عدم;
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
    إذا(p)
        p->addref();
}

Program& Program::operator = (ثابت Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    أرجع *هذا;
}

Program::~Program()
{
    إذا(p)
        p->release();
}

منطقي Program::create(ثابت ProgramSource& src,
            ثابت String& buildflags, String& errmsg)
{
    إذا(p)
    {
        p->release();
        p = عدم;
    }
    p = جديد Impl(src, buildflags, errmsg);
    إذا(!p->handle)
    {
        p->release();
        p = 0;
    }
    أرجع p != 0;
}

فراغ* Program::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ثابت ProgramSource& Program::source() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي Program::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي Program::اكتب(String& bin) ثابت
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ثابت
{
    إذا(!p)
        أرجع String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

فراغ Program::get_دووانەیی(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_Program_دووانەیی(binary);
}

Program Context::Impl::getProg(ثابت ProgramSource& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t limit = get_Program_ژماردن_Limit();
    ثابت ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلیل = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(کلیل);
        إذا (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), کلیل);
            إذا (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(کلیل);
            }
            أرجع it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            إذا (limit > 0 && sz >= limit)
            {
                ساكن منطقي warningFlag = خطأ;
                إذا (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = true;
                }
                طالما (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    إذا (c != 0)
                        اكسر;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(کلیل, prog));
        cacheList.push_front(کلیل);
    }
    أرجع prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
صنف OpenCLBufferPool
{
محمي:
    ~OpenCLBufferPool() { }
عام:
    virtual T allocate(size_t size) = 0;
    virtual فراغ release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
صنف OpenCLBufferPoolBaseImpl : عام BufferPoolController, عام OpenCLBufferPool<T>
{
خاص:
    مضمن Derived& derived() { أرجع *static_cast<Derived*>(هذا); }
محمي:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقي _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        لكل (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            إذا (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                أرجع true;
            }
        }
        أرجع خطأ;
    }

    // synchronized
    منطقي _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ثابت size_t size)
    {
        إذا (reservedEntries_.empty())
            أرجع خطأ;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        لكل (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            إذا (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                إذا (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    إذا (diff == 0)
                        اكسر;
                }
            }
        }
        إذا (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            أرجع true;
        }
        أرجع خطأ;
    }

    // synchronized
    فراغ _checkSizeOfReservedEntries()
    {
        طالما (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    مضمن size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        إذا (size < 1024*1024)
            أرجع 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        وإلا إذا (size < 16*1024*1024)
            أرجع 64*1024;
        وإلا
            أرجع 1024*1024;
    }

عام:
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
عام:
    virtual T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        إذا (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        وإلا
        {
            derived()._allocateBufferEntry(entry, size);
        }
        أرجع entry.clBuffer_;
    }
    virtual فراغ release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        إذا (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        وإلا
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    virtual size_t getReservedSize() ثابت CV_OVERRIDE { أرجع currentReservedSize; }
    virtual size_t getMaxReservedSize() ثابت CV_OVERRIDE { أرجع maxReservedSize; }
    virtual فراغ setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        إذا (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            لكل (; i != reservedEntries_.end();)
            {
                ثابت BufferEntry& entry = *i;
                إذا (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    استمر;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    virtual فراغ freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        لكل (; i != reservedEntries_.end(); ++i)
        {
            ثابت BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

بنية CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)عدم), capacity_(0) { }
};

صنف OpenCLBufferPoolImpl CV_FINAL : عام OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
عام:
    تعريفنوع بنية CLBufferEntry BufferEntry;
محمي:
    صحيح createFlags_;
عام:
    OpenCLBufferPoolImpl(صحيح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    فراغ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == عدم);
        entry.capacity_ = alignSize(size, (صحيح)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long صحيح)entry.capacity_, (فراغ*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != عدم);
        إذا(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    فراغ _releaseBufferEntry(ثابت BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != عدم);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
بنية CLSVMBufferEntry
{
    فراغ* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(عدم), capacity_(0) { }
};
صنف OpenCLSVMBufferPoolImpl CV_FINAL : عام OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, فراغ*>
{
عام:
    تعريفنوع بنية CLSVMBufferEntry BufferEntry;
عام:
    OpenCLSVMBufferPoolImpl()
    {
    }

    فراغ _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == عدم);
        entry.capacity_ = alignSize(size, (صحيح)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقي isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحيح)entry.capacity_);
        فراغ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    فراغ _releaseBufferEntry(ثابت BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != عدم);
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



template <منطقي readAccess, منطقي writeAccess>
صنف AlignedDataPtr
{
محمي:
    ثابت size_t size_;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عام:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        إذا (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~AlignedDataPtr()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    AlignedDataPtr(ثابت AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ثابت AlignedDataPtr&); // disabled
};

template <منطقي readAccess, منطقي writeAccess>
صنف AlignedDataPtr2D
{
محمي:
    ثابت size_t size_;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

عام:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != عدم);
        إذا (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                لكل (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~AlignedDataPtr2D()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                لكل (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    AlignedDataPtr2D(ثابت AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ثابت AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


فراغ Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(عدم, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

صنف OpenCLAllocator CV_FINAL : عام MatAllocator
{
عام:
    تعداد AllocatorFlags
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

    UMatData* defaultAllocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        أرجع u;
    }

    ساكن منطقي isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ساكن منطقي value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", خطأ);
        أرجع value;
    }
    ساكن منطقي isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ساكن منطقي value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", خطأ);
        أرجع value;
    }

    فراغ getBestFlags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحيح& createFlags, UMatData::MemoryFlag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        إذا ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        إذا (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        وإلا
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!useOpenCL())
            أرجع defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        إذا (!ctx.getImpl())
            أرجع defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحيح createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        فراغ* handle = عدم;
        صحيح allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        إذا (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            منطقي isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        وإلا
#endif
        إذا (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        وإلا إذا (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        وإلا
        {
            CV_Assert(handle != عدم); // Unsupported, throw
        }

        إذا (!handle)
            أرجع defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = جديد UMatData(هذا);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<فراغ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع u;
    }

    منطقي allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع خطأ;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        إذا(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحيح createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقي copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحيح allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            فراغ* handle = عدم;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقي useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            إذا (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (صحيح)u->size, handle);
            }
            وإلا إذا (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                إذا (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    منطقي isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (صحيح)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = عدم;
                    إذا (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحيح)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, عدم, عدم);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    إذا (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, عدم, عدم);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            وإلا
#endif
            {
                إذا( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                إذا (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحيح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                }
                إذا((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (فراغ*)handle).c_str());
            إذا(!handle || retval != CL_SUCCESS)
                أرجع خطأ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = هذا;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع true;
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

    فراغ deallocate(UMatData* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        إذا (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        وإلا
            deallocate_(u);
    }

    فراغ deallocate_(UMatData* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        إذا ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        إذا (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            أرجع;             // avoid any OpenCL calls
#endif
        إذا(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            إذا (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    إذا( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        منطقي isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = عدم;
                        إذا (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        إذا (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, عدم, عدم);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    وإلا
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وإلا
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    إذا( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<خطأ, true> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وإلا
                    {
                        cl_int retval = 0;
                        إذا (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            فراغ* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (long long صحيح)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            إذا (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, data, (long long صحيح)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(خطأ);
            }
            وإلا
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                إذا( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وإلا
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (فراغ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = عدم;
            إذا(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = عدم;
        }
        وإلا
        {
            CV_Assert(u->origdata == عدم);
            إذا(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(true);
            }
            إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                وإلا إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, عدم, عدم);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((فراغ*)u->handle);
            }
#endif
            وإلا
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            delete u;
            u = عدم;
        }
        CV_Assert(u == عدم);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    فراغ map(UMatData* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            إذا( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(true);
                    أرجع;
                }
#endif

                cl_int retval = CL_SUCCESS;
                إذا (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (long long صحيح)u->size, u->data).c_str());
                }
                إذا (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(true);
                    أرجع;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            إذا(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(true);
            }
        }

        إذا (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<خطأ, true> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (long long صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(خطأ);
        }
    }

    فراغ unmap(UMatData* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        إذا( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != عدم);
#ifdef HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, عدم, عدم);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                إذا (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(true);
                أرجع;
            }
#endif
            إذا (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, u->data, (long long صحيح)u->size).c_str());
                إذا (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(خطأ);
                u->data = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(true);
            }
        }
        وإلا إذا( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<true, خطأ> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (long long صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(خطأ);
            u->markHostCopyObsolete(true);
        }
    }

    منطقي checkContinuous(صحيح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقي iscontinuous = true;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        لكل( صحيح i = dims-2; i >= 0; i-- )
        {
            إذا( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = خطأ;
            total *= sz[i];
            إذا( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            إذا( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        إذا( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            إذا( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                إذا( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                إذا( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            وإلا
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                إذا( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                إذا( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        أرجع iscontinuous;
    }

    فراغ download(UMatData* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;
        UMatDataAutoLock autolock(u);

        إذا( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            أرجع;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == عدم || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            إذا( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            وإلا
            {
                // This code is from MatAllocator::download()
                صحيح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    إذا( sz[i] == 0 )
                    أرجع;
                    إذا( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (صحيح)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ثابت Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                لكل( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        وإلا
#endif
        {
            إذا( iscontinuous )
            {
                AlignedDataPtr<خطأ, true> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            وإلا إذا (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وإلا
            {
                AlignedDataPtr2D<خطأ, true> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    فراغ upload(UMatData* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        إذا( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(خطأ);
            u->markDeviceCopyObsolete(true);
            أرجع;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == عدم || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            إذا( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            وإلا
            {
                // This code is from MatAllocator::upload()
                صحيح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    إذا( sz[i] == 0 )
                    أرجع;
                    إذا( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (صحيح)sz[i];
                }

                Mat src(dims, isz, CV_8U, (فراغ*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ثابت Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                لكل( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        وإلا
#endif
        {
            إذا( iscontinuous )
            {
                AlignedDataPtr<true, خطأ> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (فراغ*)u->handle, (long long صحيح)dstrawofs, (long long صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            وإلا
            {
                AlignedDataPtr2D<true, خطأ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
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
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#endif
        {
            u->markHostCopyObsolete(true);
        }
        u->markDeviceCopyObsolete(خطأ);
    }

    فراغ copy(UMatData* src, UMatData* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي _sync) ثابت CV_OVERRIDE
    {
        إذا(!src || !dst)
            أرجع;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        إذا( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            أرجع;
        }
        إذا( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(خطأ);
#ifdef HAVE_OPENCL_SVM
            إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وإلا
#endif
            {
                dst->markDeviceCopyObsolete(true);
            }
            أرجع;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                إذا( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحيح)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, عدم, عدم);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                وإلا
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    صحيح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        إذا( sz[i] == 0 )
                        أرجع;
                        إذا( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (صحيح)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        إذا( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ثابت Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    لكل( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            وإلا
            {
                إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                وإلا
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        وإلا
#endif
        {
            إذا( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (فراغ*)src->handle, (فراغ*)dst->handle, (long long صحيح)srcrawofs, (long long صحيح)dstrawofs, (long long صحيح)total).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<خطأ, خطأ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<خطأ, خطأ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            وإلا
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        إذا (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#endif
        {
            dst->markHostCopyObsolete(true);
        }
        dst->markDeviceCopyObsolete(خطأ);

        إذا( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        إذا (ctx.empty())
            أرجع عدم;
#ifdef HAVE_OPENCL_SVM
        إذا ((svm::checkForceSVMUmatUsage() && (id == عدم || strcmp(id, "OCL") == 0)) || (id != عدم && strcmp(id, "SVM") == 0))
        {
            أرجع &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        إذا (id != عدم && strcmp(id, "HOST_ALLOC") == 0)
        {
            أرجع &ctx.getImpl()->getBufferPoolHostPtr();
        }
        إذا (id != عدم && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        أرجع &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    فراغ flushCleanupQueue() ثابت
    {
        // TSAN false positive: see #27638
        إذا (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            لكل (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    فراغ addToCleanupQueue(UMatData* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

ساكن OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ساكن OpenCLAllocator* g_allocator = جديد OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    أرجع g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


نطاق cv {

// three funcs below are implemented in umatrix.cpp
فراغ setSize( UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* _steps,
              منطقي autoSteps = خطأ );
فراغ finalizeHdr(UMat& m);

} // namespace cv


نطاق cv { نطاق ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
فراغ convertFromBuffer(فراغ* cl_mem_buffer, size_t step, صحيح rows, صحيح cols, صحيح type, UMat& dst)
{
    صحيح d = 2;
    صحيح sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, true);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, حجم(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحيح)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = جديد UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    أرجع;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
فراغ convertFromImage(فراغ* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, حجم(cl_image_format), &fmt, 0));

    صحيح depth = CV_8U;
    بدل (fmt.image_channel_data_type)
    {
    حالة CL_UNORM_INT8:
    حالة CL_UNSIGNED_INT8:
        depth = CV_8U;
        اكسر;

    حالة CL_SNORM_INT8:
    حالة CL_SIGNED_INT8:
        depth = CV_8S;
        اكسر;

    حالة CL_UNORM_INT16:
    حالة CL_UNSIGNED_INT16:
        depth = CV_16U;
        اكسر;

    حالة CL_SNORM_INT16:
    حالة CL_SIGNED_INT16:
        depth = CV_16S;
        اكسر;

    حالة CL_SIGNED_INT32:
        depth = CV_32S;
        اكسر;

    حالة CL_FLOAT:
        depth = CV_32F;
        اكسر;

    حالة CL_HALF_FLOAT:
        depth = CV_16F;
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    صحيح type = CV_8UC1;
    بدل (fmt.image_channel_order)
    {
    حالة CL_R:
    حالة CL_A:
    حالة CL_INTENSITY:
    حالة CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        اكسر;

    حالة CL_RG:
    حالة CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        اكسر;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالة CL_RGBA:
    حالة CL_BGRA:
    حالة CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        اكسر;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, حجم(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, حجم(size_t), &h, 0));

    dst.create((صحيح)h, (صحيح)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, عدم, عدم));

    CV_OCL_CHECK(clFinish(q));

    أرجع;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساكن فراغ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint ژمارە_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, عدم, &ژمارە_Devices);
    إذا (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &ژمارە_Devices).c_str());
    }

    إذا (ژمارە_Devices == 0)
    {
        devices.clear();
        أرجع;
    }

    devices.resize((size_t)ژمارە_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, ژمارە_Devices, &devices[0], &ژمارە_Devices));
}

بنية PlatformInfo::Impl
{
    Impl(فراغ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ثابت
    {
        حرف buf[1024];
        size_t sz=0;
        أرجع clGetPlatformInfo(handle, prop, حجم(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < حجم(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    صحيح versionMajor_;
    صحيح versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(فراغ* platform_id)
{
    p = جديد Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    إذا(p)
        p->release();
}

PlatformInfo::PlatformInfo(ثابت PlatformInfo& i)
{
    إذا (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ثابت PlatformInfo& i)
{
    إذا (i.p != p)
    {
        إذا (i.p)
            i.p->addref();
        إذا (p)
            p->release();
        p = i.p;
    }
    أرجع *هذا;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    أرجع *هذا;
}

صحيح PlatformInfo::device_ژمار() ثابت
{
    أرجع p ? (صحيح)p->devices.size() : 0;
}

فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت
{
    CV_Assert(p && d < (صحيح)p->devices.size() );
    إذا(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ثابت
{
    أرجع p ? p->version_ : String();
}

صحيح PlatformInfo::versionMajor() ثابت
{
    CV_Assert(p);
    أرجع p->versionMajor_;
}

صحيح PlatformInfo::versionMinor() ثابت
{
    CV_Assert(p);
    أرجع p->versionMinor_;
}

ساكن فراغ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint ژمارە_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &ژمارە_Platforms));

    إذا (ژمارە_Platforms == 0)
    {
        platforms.clear();
        أرجع;
    }

    platforms.resize((size_t)ژمارە_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(ژمارە_Platforms, &platforms[0], &ژمارە_Platforms));
}

فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    لكل (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((فراغ*)&platforms[i]) );
}

ثابت حرف* typeToStr(صحيح type)
{
    ساكن ثابت حرف* tab[CV_DEPTH_MAX*16]=
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
    صحيح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت حرف* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    أرجع result;
}

ثابت حرف* memopTypeToStr(صحيح type)
{
    ساكن ثابت حرف* tab[CV_DEPTH_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت حرف* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    أرجع result;
}

ثابت حرف* vecopTypeToStr(صحيح type)
{
    ساكن ثابت حرف* tab[CV_DEPTH_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ثابت حرف* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    أرجع result;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    أرجع convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf, size_t buf_size)
{
    إذا( sdepth == ddepth )
        أرجع "noconvert";
    ثابت حرف *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    إذا( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    وإلا إذا( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    وإلا
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    أرجع buf;
}

ثابت حرف* getOpenCLErrorString(صحيح errorCode)
{
#define CV_OCL_CODE(id) حالة id: أرجع #id
#define CV_OCL_CODE_(id, name) حالة id: أرجع #name
    بدل (errorCode)
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
    CV_OCL_CODE(CL_INVALID_بەها);
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
    CV_OCL_CODE(CL_INVALID_دووانەیی);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_بەها);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_ژماردن);
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
    افتراضي: أرجع "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
ساكن std::string kerToStr(ثابت Mat & k)
{
    صحيح width = k.cols - 1, depth = k.depth();
    ثابت T * ثابت data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    إذا (depth <= CV_8S)
    {
        لكل (صحيح i = 0; i < width; ++i)
            stream << "DIG(" << (صحيح)data[i] << ")";
        stream << "DIG(" << (صحيح)data[width] << ")";
    }
    وإلا إذا (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    وإلا إذا (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < width; ++i)
            stream << "DIG(" << (عائم)data[i] << "h)";
        stream << "DIG(" << (عائم)data[width] << "h)";
    }
    وإلا
    {
        لكل (صحيح i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    أرجع stream.str();
}

String kernelToStr(تێخستن_Array _kernel, صحيح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحيح depth = kernel.depth();
    إذا (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    إذا (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    إذا (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    تعريفنوع std::string (* func_t)(ثابت Mat &);
    ساكن ثابت func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<حرف>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<صحيح>, kerToStr<عائم>, kerToStr<مزدوج>, kerToStr<hfloat>,
                                                0, kerToStr<منطقي>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    أرجع cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    افعل \
    { \
        إذا (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            صحيح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            إذا (cwidth < ckercn || ckercn <= 0) \
                أرجع 1; \
            cols.push_back(cwidth); \
            إذا (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                أرجع 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    طالما ((فراغ)0, 0)

صحيح predictOptimalVectorWidth(تێخستن_Array src1, تێخستن_Array src2, تێخستن_Array src3,
                              تێخستن_Array src4, تێخستن_Array src5, تێخستن_Array src6,
                              تێخستن_Array src7, تێخستن_Array src8, تێخستن_Array src9,
                              OclVectorStrategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحيح vectorWidths[CV_DEPTH_MAX] = {
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
    إذا (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    أرجع checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحيح checkOptimalVectorWidth([[maybe_unused]] ثابت صحيح *vectorWidths,
                            [[maybe_unused]] تێخستن_Array src1,
                            [[maybe_unused]] تێخستن_Array src2,
                            [[maybe_unused]] تێخستن_Array src3,
                            [[maybe_unused]] تێخستن_Array src4,
                            [[maybe_unused]] تێخستن_Array src5,
                            [[maybe_unused]] تێخستن_Array src6,
                            [[maybe_unused]] تێخستن_Array src7,
                            [[maybe_unused]] تێخستن_Array src8,
                            [[maybe_unused]] تێخستن_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    أرجع 1;
#وإلا
    CV_Assert(vectorWidths);

    صحيح ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<صحيح> dividers, kercns;
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

    لكل (size_t i = 0; i < size; ++i)
        طالما (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحيح kercn = *std::min_element(kercns.begin(), kercns.end());

    أرجع kercn;
#endif
}

صحيح predictOptimalVectorWidthMax(تێخستن_Array src1, تێخستن_Array src2, تێخستن_Array src3,
                                 تێخستن_Array src4, تێخستن_Array src5, تێخستن_Array src6,
                                 تێخستن_Array src7, تێخستن_Array src8, تێخستن_Array src9)
{
    أرجع predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
فراغ buildOptionsAddMatrixDescription(String& buildOptions, ثابت String& name, تێخستن_Array _m)
{
    إذا (!buildOptions.empty())
        buildOptions += " ";
    صحيح type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (صحيح)CV_MAT_CN(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحيح)depth
            );
}


بنية Image2D::Impl
{
    Impl(ثابت UMat &src, منطقي norm, منطقي alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        إذا (handle)
            clReleaseMemObject(handle);
    }

    ساكن cl_image_format getImageFormat(صحيح depth, صحيح cn, منطقي norm)
    {
        cl_image_format format;
        ساكن ثابت صحيح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساكن ثابت صحيح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساكن ثابت صحيح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحيح channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        صحيح channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        أرجع format;
    }

    ساكن منطقي isFormatSupported(cl_image_format format)
    {
        إذا (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        إذا (!context)
            أرجع خطأ;

        // Figure out how many formats are supported by this context.
        cl_uint ژمارە_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, ژمارە_Formats,
                                                عدم, &ژمارە_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        إذا (ژمارە_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(ژمارە_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, ژمارە_Formats,
                                             formats.data(), عدم);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            لكل (cl_uint i = 0; i < ژمارە_Formats; ++i)
            {
                إذا (!memcmp(&formats[i], &format, حجم(format)))
                {
                    أرجع true;
                }
            }
        }
        أرجع خطأ;
    }

    فراغ init(ثابت UMat &src, منطقي norm, منطقي alias)
    {
        إذا (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        صحيح err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        إذا (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        إذا (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحيح minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        إذا (1 < major || (1 == major && 2 <= minor))
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
            desc.ژمارە_mip_levels   = 0;
            desc.ژمارە_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, عدم, &err);
        }
        وإلا
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, عدم, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        إذا (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), عدم, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long صحيح)(src.cols * src.rows * src.elemSize()), (فراغ*)devData
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, عدم, عدم));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        وإلا
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != عدم);

        إذا (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, عدم, 0));
            إذا (!src.isContinuous())
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
    p = عدم;
}

Image2D::Image2D(ثابت UMat &src, منطقي norm, منطقي alias)
{
    p = جديد Impl(src, norm, alias);
}

منطقي Image2D::canCreateAlias(ثابت UMat &m)
{
    منطقي ret = خطأ;
    ثابت Device & d = ocl::Device::getDefault();
    إذا (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        إذا (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            إذا (!m.u->tempUMat())
            {
                ret = true;
            }
        }
    }
    أرجع ret;
}

منطقي Image2D::isFormatSupported(صحيح depth, صحيح cn, منطقي norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    أرجع Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    إذا (p)
        p->addref();
}

Image2D & Image2D::operator = (ثابت Image2D & i)
{
    إذا (i.p != p)
    {
        إذا (i.p)
            i.p->addref();
        إذا (p)
            p->release();
        p = i.p;
    }
    أرجع *هذا;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    أرجع *هذا;
}

Image2D::~Image2D()
{
    إذا (p)
        p->release();
}

فراغ* Image2D::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

منطقي internal::isOpenCLForced()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", خطأ);
        initialized = true;
    }
    أرجع value;
}

منطقي internal::isPerformanceCheckBypassed()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", خطأ);
        initialized = true;
    }
    أرجع value;
}

منطقي internal::isCLBuffer(UMat& u)
{
    إذا (isCudaBacked(u.u))
        أرجع خطأ;
    فراغ* h = u.handle(ACCESS_RW);
    إذا (!h)
        أرجع true;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#إذا 1
    إذا ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        أرجع خطأ;
#وإلا
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, حجم(type), &type, عدم);
    إذا (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        أرجع خطأ;
#endif
    أرجع true;
}

بنية Timer::Impl
{
    ثابت Queue queue;

    Impl(ثابت Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    فراغ start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    فراغ stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        أرجع (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت Queue& q) : p(جديد Impl(q)) { }
Timer::~Timer() { delete p; }

فراغ Timer::start()
{
    CV_Assert(p);
    p->start();
}

فراغ Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ثابت
{
    CV_Assert(p);
    أرجع p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
