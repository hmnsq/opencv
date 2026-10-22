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

#অন্তর্ভাব "precomp.hpp"

#ifndef HAVE_OPENCL
#অন্তর্ভাব "ocl_disabled.impl.hpp"
#অন্যথা // HAVE_OPENCL

#অন্তর্ভাব <list>
#অন্তর্ভাব <map>
#অন্তর্ভাব <deque>
#অন্তর্ভাব <set>
#অন্তর্ভাব <string>
#অন্তর্ভাব <sstream>
#অন্তর্ভাব <fstream>
#যদি !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#অন্তর্ভাব <inttypes.h>
#endif

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাব <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#অন্তর্ভাব "opencv2/core/ocl_genbase.hpp"
#অন্তর্ভাব "opencl_kernels_core.hpp"

#অন্তর্ভাব "opencv2/core/utils/lock.private.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.hpp"
#অন্তর্ভাব "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ফাঁকা, 0, "OpenCL allocator: " << __VA_ARGS__)
#অন্তর্ভাব "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#অন্তর্ভাব "opencv2/core/bufferpool.hpp"
#ifndef LOG_বাফার_POOL
# যদি 0
#   define LOG_বাফার_POOL printf
# অন্যথা
#   define LOG_বাফার_POOL(...)
# endif
#endif

#যদি CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#অন্যথা
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#যদি CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#অন্যথা
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#অন্তর্ভাব "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#অন্তর্ভাব "opencv2/core/opencl/opencl_svm.hpp"
#endif

#অন্তর্ভাব "umatrix.hpp"

নামাকাশ cv { নামাকাশ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    শূন্য addref() { CV_XADD(&refcount, 1); } \
    শূন্য release() { যদি( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete এই; } \
    পূর্ণসংখ্যা refcount

স্থির cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ফেরত opencl_allocator_stats;
}

#ifndef _DEBUG
স্থির যৌক্তিক isRaiseError()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক value = মিথ্যা;
    যদি (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত value;
}
#endif

স্থির শূন্য onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    যৌক্তিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", মিথ্যা);
    যদি (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#যদি CV_OPENCL_TRACE_CHECK
স্থির অন্তঃরেখ
শূন্য traceOpenCLCheck(cl_int status, ধ্রুবক অক্ষর* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_ফলাফল(status, message) traceOpenCLCheck(status, message)
#অন্যথা
#define CV_OCL_TRACE_CHECK_ফলাফল(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_বার্তা(check_ফলাফল, বার্তা) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_ফলাফল), check_ফলাফল, বার্তা)

#define CV_OCL_CHECK_ফলাফল(check_ফলাফল, বার্তা) \
    করো { \
        CV_OCL_TRACE_CHECK_ফলাফল(check_ফলাফল, বার্তা); \
        যদি (check_ফলাফল != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(বার্তা), ধ্রুবক অক্ষর*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_বার্তা = CV_OCL_API_ERROR_বার্তা(check_ফলাফল, বার্তা); \
            CV_Error(Error::OpenCLApiCallError, error_বার্তা); \
        } \
    } যখন (0)

#define CV_OCL_CHECK_(রাশিমালা, check_ফলাফল) করো { রাশিমালা; CV_OCL_CHECK_ফলাফল(check_ফলাফল, #expr); } যখন (0)

#define CV_OCL_CHECK(রাশিমালা) করো { cl_int __cl_result = (রাশিমালা); CV_OCL_CHECK_ফলাফল(__cl_result, #expr); } যখন (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_ফলাফল(check_ফলাফল, বার্তা) CV_OCL_CHECK_ফলাফল(check_ফলাফল, বার্তা)
#define CV_OCL_DBG_CHECK(রাশিমালা) CV_OCL_CHECK(রাশিমালা)
#define CV_OCL_DBG_CHECK_(রাশিমালা, check_ফলাফল) CV_OCL_CHECK_(রাশিমালা, check_ফলাফল)
#অন্যথা
#define CV_OCL_DBG_CHECK_ফলাফল(check_ফলাফল, বার্তা) \
    করো { \
        CV_OCL_TRACE_CHECK_ফলাফল(check_ফলাফল, বার্তা); \
        যদি (check_ফলাফল != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(বার্তা), ধ্রুবক অক্ষর*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_বার্তা = CV_OCL_API_ERROR_বার্তা(check_ফলাফল, বার্তা); \
            CV_Error(Error::OpenCLApiCallError, error_বার্তা); \
        } \
    } যখন (0)
#define CV_OCL_DBG_CHECK_(রাশিমালা, check_ফলাফল) করো { রাশিমালা; CV_OCL_DBG_CHECK_ফলাফল(check_ফলাফল, #expr); } যখন (0)
#define CV_OCL_DBG_CHECK(রাশিমালা) করো { cl_int __cl_result = (রাশিমালা); CV_OCL_DBG_CHECK_ফলাফল(__cl_result, #expr); } যখন (0)
#endif


স্থির ধ্রুবক যৌক্তিক CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", সত্য);
স্থির ধ্রুবক যৌক্তিক CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", সত্য);
স্থির ধ্রুবক যৌক্তিক CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", সত্য);
স্থির ধ্রুবক যৌক্তিক CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", সত্য);

#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
স্থির ধ্রুবক যৌক্তিক CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", মিথ্যা);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
স্থির ধ্রুবক যৌক্তিক CV_OPENCL_DISABLE_বাফার_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#যদি 1 //def __APPLE__
        সত্য
#অন্যথা
        মিথ্যা
#endif
);

স্থির String getBuildExtraOptions()
{
    স্থির String param_buildExtraOptions;
    স্থির যৌক্তিক initialized = মিথ্যা;
    যদি (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = সত্য;
        যদি (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ফাঁকা, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ফেরত param_buildExtraOptions;
}

স্থির ধ্রুবক যৌক্তিক CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", সত্য);
স্থির ধ্রুবক size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


কাঠামো UMat2D
{
    UMat2D(ধ্রুবক UMat& m)
    {
        offset = (পূর্ণসংখ্যা)m.offset;
        step = (পূর্ণসংখ্যা)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    পূর্ণসংখ্যা offset;
    পূর্ণসংখ্যা step;
    পূর্ণসংখ্যা rows;
    পূর্ণসংখ্যা cols;
};

কাঠামো UMat3D
{
    UMat3D(ধ্রুবক UMat& m)
    {
        offset = (পূর্ণসংখ্যা)m.offset;
        step = (পূর্ণসংখ্যা)m.step.p[1];
        slicestep = (পূর্ণসংখ্যা)m.step.p[0];
        slices = (পূর্ণসংখ্যা)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    পূর্ণসংখ্যা offset;
    পূর্ণসংখ্যা slicestep;
    পূর্ণসংখ্যা step;
    পূর্ণসংখ্যা slices;
    পূর্ণসংখ্যা rows;
    পূর্ণসংখ্যা cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
স্থির uint64 crc64( ধ্রুবক uchar* তথ্য, size_t size, uint64 crc0=0 )
{
    স্থির uint64 table[256];
    স্থির যৌক্তিক initialized = মিথ্যা;

    যদি( !initialized )
    {
        জন্য( পূর্ণসংখ্যা i = 0; i < 256; i++ )
        {
            uint64 c = i;
            জন্য( পূর্ণসংখ্যা j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = সত্য;
    }

    uint64 crc = ~crc0;
    জন্য( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ তথ্য[idx]] ^ (crc >> 8);

    ফেরত ~crc;
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
কাঠামো OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    প্রকারলেখ std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ফাঁকা, "Initializing OpenCL cache configuration...");
        যদি (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL cache is disabled");
            ফেরত;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        যদি (cache_path_.empty())
        {
            CV_LOG_INFO(ফাঁকা, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        করো
        {
            চেষ্টা
            {
                যদি (cache_path_.empty())
                    বিরতি;
                যদি (cache_path_ == "disabled")
                    বিরতি;
                যদি (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(ফাঁকা, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    বিরতি;
                }

                যদি (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    যদি (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(ফাঁকা, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        যদি (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(ফাঁকা, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            বিরতি;
                        }
                    }

                    চেষ্টা
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(ফাঁকা, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(ফাঁকা, 0, "Checking cache lock... Done!");
                    }
                    ধরো (ধ্রুবক cv::Exception& e)
                    {
                        CV_LOG_WARNING(ফাঁকা, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    ধরো (...)
                    {
                        CV_LOG_WARNING(ফাঁকা, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                অন্যথা
                {
                    যদি (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ফাঁকা, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    অন্যথা
                    {
                        CV_LOG_INFO(ফাঁকা, "OpenCL cache lock is disabled");
                    }
                }
            }
            ধরো (ধ্রুবক cv::Exception& e)
            {
                CV_LOG_WARNING(ফাঁকা, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } যখন (0);
        যদি (!cache_path_.empty())
        {
            যদি (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(ফাঁকা, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            অন্যথা
            {
                CV_LOG_INFO(ফাঁকা, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    শূন্য clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ধ্রুবক std::string& ctx_prefix,
            ধ্রুবক std::string& cleanup_prefix)
    {
        যদি (cache_path_.empty())
            ফেরত std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        যদি (found_it != prepared_contexts_.end())
            ফেরত found_it->second;

        CV_LOG_INFO(ফাঁকা, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        যৌক্তিক ফলাফল = utils::fs::isDirectory(target_directory);
        যদি (!ফলাফল)
        {
            চেষ্টা
            {
                CV_LOG_VERBOSE(ফাঁকা, 0, "Creating directory: " << target_directory);
                যদি (utils::fs::createDirectories(target_directory))
                {
                    ফলাফল = সত্য;
                }
                অন্যথা
                {
                    CV_LOG_WARNING(ফাঁকা, "Can't create directory: " << target_directory);
                }
            }
            ধরো (ধ্রুবক cv::Exception& e)
            {
                CV_LOG_ERROR(ফাঁকা, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = ফলাফল ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        যদি (ফলাফল && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            চেষ্টা
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, মিথ্যা, সত্য);
                std::vector<String> remove_entries;
                জন্য (size_t i = 0; i < entries.size(); i++)
                {
                    ধ্রুবক String& name = entries[i];
                    যদি (0 == name.find(cleanup_prefix))
                    {
                        যদি (0 == name.find(ctx_prefix))
                            চালু; // skip current
                        remove_entries.push_back(name);
                    }
                }
                যদি (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ফাঁকা, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ফাঁকা, "Trying to remove these directories...");
                    জন্য (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ফাঁকা, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ফাঁকা, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    জন্য (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ধ্রুবক String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        চেষ্টা
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(ফাঁকা, "Removed: " << path);
                        }
                        ধরো (ধ্রুবক cv::Exception& e)
                        {
                            CV_LOG_ERROR(ফাঁকা, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            ধরো (...)
            {
                CV_LOG_WARNING(ফাঁকা, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ফাঁকা, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        ফেরত target_directory;
    }

    স্থির OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, নতুন OpenCLBinaryCacheConfigurator());
    }
};
শ্রেণী দ্বিমিক_Program_File
{
    গণনা { MAX_ENTRIES = 64 };

    প্রকারলেখ unsigned পূর্ণসংখ্যা uint32_t;

    কাঠামো CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t source_Signature_আকার;
        //char sourceSignature[];
    };

    কাঠামো CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t সংখ্যা_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    কাঠামো CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t পরবর্তী_Entry_File_Offset; // 0 for the last entry in chain
        uint32_t চাবি_আকার;
        uint32_t তথ্য_আকার;
        //char key[];
        //char data[];
    };

    ধ্রুবক std::string fileName_;
    ধ্রুবক অক্ষর* ধ্রুবক sourceSignature_;
    ধ্রুবক size_t source_Signature_আকার_2;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ধ্রুবক std::string& options)
    {
        uint64 hash = crc64((ধ্রুবক uchar*)options.c_str(), options.size(), 0);
        ফেরত hash & (MAX_ENTRIES - 1);
    }

    অন্তঃরেখ size_t get_File_আকার()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_আকার = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ফেরত file_আকার;
    }
    অন্তঃরেখ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.পঠ((অক্ষর*)&res, আকার(uint32_t));
        CV_Assert(!f.fail());
        ফেরত res;
    }
    অন্তঃরেখ শূন্য writeUInt32(ধ্রুবক uint32_t value)
    {
        uint32_t v = value;
        f.লিখ((অক্ষর*)&v, আকার(uint32_t));
        CV_Assert(!f.fail());
    }

    অন্তঃরেখ শূন্য seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    অন্তঃরেখ শূন্য seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    অন্তঃরেখ শূন্য seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    শূন্য clearFile()
    {
        f.close();
        যদি (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ফাঁকা, "Can't remove: " << fileName_);
        ফেরত;
    }

সর্বজনীন:
    দ্বিমিক_Program_File(ধ্রুবক std::string& fileName, ধ্রুবক অক্ষর* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), source_Signature_আকার_2(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(আকার(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != ফাঁকা);
        CV_Assert(source_Signature_আকার_2 > 0);
        memset(entryOffsets, 0, আকার(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        যদি(f.is_open() && get_File_আকার() > 0)
        {
            যৌক্তিক isValid = মিথ্যা;
            চেষ্টা
            {
                uint32_t file_Source_Signature_আকার = readUInt32();
                যদি (file_Source_Signature_আকার == source_Signature_আকার_2)
                {
                    cv::Auto_বাফার<অক্ষর> fileSourceSignature(file_Source_Signature_আকার + 1);
                    f.পঠ(fileSourceSignature.তথ্য(), file_Source_Signature_আকার);
                    যদি (f.eof())
                    {
                        CV_LOG_ERROR(ফাঁকা, "Unexpected EOF");
                    }
                    অন্যথা যদি (memcmp(sourceSignature, fileSourceSignature.তথ্য(), file_Source_Signature_আকার) == 0)
                    {
                        isValid = সত্য;
                    }
                }
                যদি (!isValid)
                {
                    CV_LOG_ERROR(ফাঁকা, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            ধরো (ধ্রুবক cv::Exception& e)
            {
                CV_LOG_ERROR(ফাঁকা, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            ধরো (...)
            {
                CV_LOG_ERROR(ফাঁকা, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            যদি (!isValid)
            {
                clearFile();
            }
            অন্যথা
            {
                seekReadAbsolute(0);
            }
        }
    }

    যৌক্তিক পঠ(ধ্রুবক std::string& চাবি, std::vector<অক্ষর>& buf)
    {
        যদি (!f.is_open())
            ফেরত মিথ্যা;

        size_t file_আকার = get_File_আকার();
        যদি (file_আকার == 0)
        {
            CV_LOG_ERROR(ফাঁকা, "Invalid file (empty): " << fileName_);
            clearFile();
            ফেরত মিথ্যা;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_আকার = readUInt32();
        CV_Assert(file_Source_Signature_আকার > 0);
        seekReadRelative(file_Source_Signature_আকার);

        uint32_t সংখ্যা_Of_Entries = readUInt32();
        CV_Assert(সংখ্যা_Of_Entries > 0);
        যদি (সংখ্যা_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ফাঁকা, "Invalid file: " << fileName_);
            clearFile();
            ফেরত মিথ্যা;
        }
        f.পঠ((অক্ষর*)&entryOffsets[0], আকার(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_অঙ্ক = getHash(চাবি);

        uint32_t entryOffset = entryOffsets[entry_অঙ্ক];
        FileEntry entry;
        যখন (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.পঠ((অক্ষর*)&entry, আকার(entry));
            CV_Assert(!f.fail());
            cv::Auto_বাফার<অক্ষর> file_চাবি(entry.চাবি_আকার + 1);
            যদি (চাবি.size() == entry.চাবি_আকার)
            {
                যদি (entry.চাবি_আকার > 0)
                {
                    f.পঠ(file_চাবি.তথ্য(), entry.চাবি_আকার);
                    CV_Assert(!f.fail());
                }
                যদি (memcmp(file_চাবি.তথ্য(), চাবি.c_str(), entry.চাবি_আকার) == 0)
                {
                    buf.resize(entry.তথ্য_আকার);
                    f.পঠ(&buf[0], entry.তথ্য_আকার);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ফাঁকা, 0, "Read...");
                    ফেরত সত্য;
                }
            }
            যদি (entry.পরবর্তী_Entry_File_Offset == 0)
                বিরতি;
            entryOffset = entry.পরবর্তী_Entry_File_Offset;
        }
        ফেরত মিথ্যা;
    }

    যৌক্তিক লিখ(ধ্রুবক std::string& চাবি, std::vector<অক্ষর>& buf)
    {
        যদি (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            যদি (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                যদি (!f.is_open())
                {
                    CV_LOG_ERROR(ফাঁকা, "Can't create file: " << fileName_);
                    ফেরত মিথ্যা;
                }
            }
        }

        size_t file_আকার = get_File_আকার();
        যদি (file_আকার == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)source_Signature_আকার_2);
            f.লিখ(sourceSignature_, source_Signature_আকার_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, আকার(entryOffsets));
            f.লিখ((অক্ষর*)entryOffsets, আকার(entryOffsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_আকার = get_File_আকার();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_আকার = readUInt32();
        CV_Assert(file_Source_Signature_আকার == source_Signature_আকার_2);
        seekReadRelative(file_Source_Signature_আকার);

        uint32_t সংখ্যা_Of_Entries = readUInt32();
        CV_Assert(সংখ্যা_Of_Entries > 0);
        যদি (সংখ্যা_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ফাঁকা, "Invalid file: " << fileName_);
            clearFile();
            ফেরত মিথ্যা;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.পঠ((অক্ষর*)&entryOffsets[0], আকার(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_অঙ্ক = getHash(চাবি);

        uint32_t entryOffset = entryOffsets[entry_অঙ্ক];
        FileEntry entry;
        যখন (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.পঠ((অক্ষর*)&entry, আকার(entry));
            CV_Assert(!f.fail());
            cv::Auto_বাফার<অক্ষর> file_চাবি(entry.চাবি_আকার + 1);
            যদি (চাবি.size() == entry.চাবি_আকার)
            {
                যদি (entry.চাবি_আকার > 0)
                {
                    f.পঠ(file_চাবি.তথ্য(), entry.চাবি_আকার);
                    CV_Assert(!f.fail());
                }
                যদি (0 == memcmp(file_চাবি.তথ্য(), চাবি.c_str(), entry.চাবি_আকার))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ফাঁকা, 0, "Duplicate key ignored: " << fileName_);
                    ফেরত মিথ্যা;
                }
            }
            যদি (entry.পরবর্তী_Entry_File_Offset == 0)
                বিরতি;
            entryOffset = entry.পরবর্তী_Entry_File_Offset;
        }
        seekReadAbsolute(0);
        যদি (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.পরবর্তী_Entry_File_Offset = (uint32_t)file_আকার;
            f.লিখ((অক্ষর*)&entry, আকার(entry));
            CV_Assert(!f.fail());
        }
        অন্যথা
        {
            entryOffsets[entry_অঙ্ক] = (uint32_t)file_আকার;
            seekWriteAbsolute(tableEntriesOffset);
            f.লিখ((অক্ষর*)entryOffsets, আকার(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_আকার);
        entry.পরবর্তী_Entry_File_Offset = 0;
        entry.তথ্য_আকার = (uint32_t)buf.size();
        entry.চাবি_আকার = (uint32_t)চাবি.size();
        f.লিখ((অক্ষর*)&entry, আকার(entry));
        CV_Assert(!f.fail());
        f.লিখ(চাবি.c_str(), entry.চাবি_আকার);
        CV_Assert(!f.fail());
        f.লিখ(&buf[0], entry.তথ্য_আকার);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ফাঁকা, 0, "Write... (" << buf.size() << " bytes)");
        ফেরত সত্য;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



কাঠামো OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    পূর্ণসংখ্যা device_;  // device index in context
    ocl::Queue queue_;
    পূর্ণসংখ্যা useOpenCL_;

সুরক্ষিত:
    Impl() = delete;

    শূন্য _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        পূর্ণসংখ্যা ndevices = (পূর্ণসংখ্যা)context_.ndevices();
        CV_Assert(ndevices > 0);
        যৌক্তিক found = মিথ্যা;
        জন্য (পূর্ণসংখ্যা i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            যদি (dhandle == deviceID)
            {
                device_ = i;
                found = সত্য;
                বিরতি;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    শূন্য _init_device(ধ্রুবক ocl::Device& device)
    {
        CV_Assert(device.ptr());
        পূর্ণসংখ্যা ndevices = (পূর্ণসংখ্যা)context_.ndevices();
        CV_Assert(ndevices > 0);
        যৌক্তিক found = মিথ্যা;
        জন্য (পূর্ণসংখ্যা i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            যদি (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = সত্য;
                বিরতি;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

সর্বজনীন:
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

    Impl(ধ্রুবক ocl::Context& context, ধ্রুবক ocl::Device& device, ধ্রুবক ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ধ্রুবক ocl::Context& context, ধ্রুবক ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ধ্রুবক ocl::Context& context, ধ্রুবক পূর্ণসংখ্যা device, ধ্রুবক ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ধ্রুবক Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    অন্তঃরেখ যৌক্তিক useOpenCL() ধ্রুবক { ফেরত const_cast<Impl*>(এই)->useOpenCL(); }
    যৌক্তিক useOpenCL()
    {
        যদি (useOpenCL_ < 0)
        {
            চেষ্টা
            {
                useOpenCL_ = 0;
                যদি (!context_.empty() && context_.ndevices() > 0)
                {
                    ধ্রুবক Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            ধরো (ধ্রুবক cv::Exception&)
            {
                // nothing
            }
            যদি (!useOpenCL_)
                CV_LOG_INFO(ফাঁকা, "OpenCL: can't use OpenCL execution context");
        }
        ফেরত useOpenCL_ > 0;
    }

    শূন্য setUseOpenCL(যৌক্তিক flag)
    {
        যদি (!flag)
            useOpenCL_ = 0;
        অন্যথা
            useOpenCL_ = -1;
    }

    স্থির ধ্রুবক std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ফাঁকা, "OpenCL: initializing thread execution context");

        স্থির যৌক্তিক initialized = মিথ্যা;
        স্থির std::shared_ptr<Impl> g_primaryExecutionContext;

        যদি (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            যদি (!initialized)
            {
                CV_LOG_INFO(ফাঁকা, "OpenCL: creating new execution context...");
                চেষ্টা
                {
                    Context c = ocl::Context::create(std::string());
                    যদি (c.ndevices())
                    {
                        পূর্ণসংখ্যা deviceId = 0;
                        auto& d = c.device(deviceId);
                        যদি (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            যদি (!q.ptr())
                            {
                                CV_LOG_ERROR(ফাঁকা, "OpenCL: Can't create default OpenCL queue");
                            }
                            অন্যথা
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ফাঁকা, "OpenCL: device=" << d.name());
                            }
                        }
                        অন্যথা
                        {
                            CV_LOG_ERROR(ফাঁকা, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    অন্যথা
                    {
                        CV_LOG_INFO(ফাঁকা, "OpenCL: context is not available/disabled");
                    }
                }
                ধরো (ধ্রুবক std::exception& e)
                {
                    CV_LOG_INFO(ফাঁকা, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                ধরো (...)
                {
                    CV_LOG_WARNING(ফাঁকা, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = সত্য;
            }
        }
        ফেরত g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->context_;
}
Device& OpenCLExecutionContext::getDevice() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->queue_;
}

যৌক্তিক OpenCLExecutionContext::useOpenCL() ধ্রুবক
{
    যদি (p)
        ফেরত p->useOpenCL();
    ফেরত মিথ্যা;
}
শূন্য OpenCLExecutionContext::setUseOpenCL(যৌক্তিক flag)
{
    CV_Assert(p);
    p->setUseOpenCL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    OpenCLExecutionContext& c = তথ্য.oclExecutionContext;
    যদি (!তথ্য.oclExecutionContextInitialized)
    {
        তথ্য.oclExecutionContextInitialized = সত্য;
        যদি (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ফেরত c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    OpenCLExecutionContext& c = তথ্য.oclExecutionContext;
    ফেরত c;
}

শূন্য OpenCLExecutionContext::bind() ধ্রুবক
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    তথ্য.oclExecutionContext = *এই;
    তথ্য.oclExecutionContextInitialized = সত্য;
    তথ্য.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ধ্রুবক
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ধ্রুবক Queue q(getContext(), getDevice());
    ফেরত cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ধ্রুবক ocl::Queue& q) ধ্রুবক
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ফাঁকা);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ফেরত c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুবক Context& context, ধ্রুবক Device& device, ধ্রুবক ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    যদি (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    ফেরত ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুবক Context& context, ধ্রুবক Device& device)
{
    CV_TRACE_FUNCTION();
    যদি (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    ফেরত ctx;

}

শূন্য OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
স্থির যৌক্তিক g_isOpenCLInitialized = মিথ্যা;
স্থির যৌক্তিক g_isOpenCLAvailable = মিথ্যা;

যৌক্তিক haveOpenCL()
{
    CV_TRACE_FUNCTION();

    যদি (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        যদি (!envPath.empty())
        {
            যদি (envPath == "disabled")
            {
                g_isOpenCLAvailable = মিথ্যা;
                g_isOpenCLInitialized = সত্য;
                ফেরত মিথ্যা;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(ফাঁকা, "Initialize OpenCL runtime...");
        চেষ্টা
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ফাঁকা, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ফাঁকা, "OpenCL: found " << n << " platforms");
        }
        ধরো (...)
        {
            g_isOpenCLAvailable = মিথ্যা;
        }
        g_isOpenCLInitialized = সত্য;
    }
    ফেরত g_isOpenCLAvailable;
}

যৌক্তিক useOpenCL()
{
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    যদি (তথ্য.useOpenCL < 0)
    {
        চেষ্টা
        {
            তথ্য.useOpenCL = 0;
            যদি (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                তথ্য.useOpenCL = c.useOpenCL();
            }
        }
        ধরো (...)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ফেরত তথ্য.useOpenCL > 0;
}

যৌক্তিক isOpenCLActivated()
{
    যদি (!g_isOpenCLAvailable)
        ফেরত মিথ্যা; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ফেরত useOpenCL();
}

শূন্য setUseOpenCL(যৌক্তিক flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    যদি (!c.empty())
    {
        c.setUseOpenCL(flag);
        তথ্য.useOpenCL = c.useOpenCL();
    }
    অন্যথা
    {
        যদি (!flag)
            তথ্য.useOpenCL = 0;
        অন্যথা
            তথ্য.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

শ্রেণী AmdBlasHelper
{
সর্বজনীন:
    স্থির AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, নতুন AmdBlasHelper())
    }

    যৌক্তিক isAvailable() ধ্রুবক
    {
        ফেরত g_isAmdBlasAvailable;
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

সুরক্ষিত:
    AmdBlasHelper()
    {
        যদি (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            যদি (!g_isAmdBlasInitialized)
            {
                যদি (haveOpenCL())
                {
                    চেষ্টা
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    ধরো (...)
                    {
                        g_isAmdBlasAvailable = মিথ্যা;
                    }
                }
                অন্যথা
                    g_isAmdBlasAvailable = মিথ্যা;

                g_isAmdBlasInitialized = সত্য;
            }
        }
    }

ব্যক্তিগত:
    স্থির যৌক্তিক g_isAmdBlasInitialized;
    স্থির যৌক্তিক g_isAmdBlasAvailable;
};

যৌক্তিক AmdBlasHelper::g_isAmdBlasAvailable = মিথ্যা;
যৌক্তিক AmdBlasHelper::g_isAmdBlasInitialized = মিথ্যা;

যৌক্তিক haveAmdBlas()
{
    ফেরত AmdBlasHelper::getInstance().isAvailable();
}

#অন্যথা

যৌক্তিক haveAmdBlas()
{
    ফেরত মিথ্যা;
}

#endif

#ifdef HAVE_CLAMDFFT

শ্রেণী AmdFftHelper
{
সর্বজনীন:
    স্থির AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, নতুন AmdFftHelper())
    }

    যৌক্তিক isAvailable() ধ্রুবক
    {
        ফেরত g_isAmdFftAvailable;
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

সুরক্ষিত:
    AmdFftHelper()
    {
        যদি (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            যদি (!g_isAmdFftInitialized)
            {
                যদি (haveOpenCL())
                {
                    চেষ্টা
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_Setup_তথ্য(&setup_তথ্য) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = সত্য;
                    }
                    ধরো (ধ্রুবক Exception &)
                    {
                        g_isAmdFftAvailable = মিথ্যা;
                    }
                }
                অন্যথা
                    g_isAmdFftAvailable = মিথ্যা;

                g_isAmdFftInitialized = সত্য;
            }
        }
    }

ব্যক্তিগত:
    স্থির clfft_Setup_তথ্য setup_তথ্য;
    স্থির যৌক্তিক g_isAmdFftInitialized;
    স্থির যৌক্তিক g_isAmdFftAvailable;
};

clfft_Setup_তথ্য AmdFftHelper::setup_তথ্য;
যৌক্তিক AmdFftHelper::g_isAmdFftAvailable = মিথ্যা;
যৌক্তিক AmdFftHelper::g_isAmdFftInitialized = মিথ্যা;

যৌক্তিক haveAmdFft()
{
    ফেরত AmdFftHelper::getInstance().isAvailable();
}

#অন্যথা

যৌক্তিক haveAmdFft()
{
    ফেরত মিথ্যা;
}

#endif

যৌক্তিক haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    ফেরত সত্য;
#অন্যথা
    ফেরত মিথ্যা;
#endif
}

শূন্য finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

কাঠামো Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = মিথ্যা;
    }

    ~Impl() {}

    শূন্য init()
    {
        যদি( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            যদি( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            যদি( handle != 0 )
            {
                অক্ষর buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, আকার(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = সত্য;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    যৌক্তিক initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    যদি(p)
        p->release();
}

Platform::Platform(ধ্রুবক Platform& pl)
{
    p = (Impl*)pl.p;
    যদি(p)
        p->addref();
}

Platform& Platform::operator = (ধ্রুবক Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    যদি (এই != &pl) {
        যদি(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    ফেরত *এই;
}

শূন্য* Platform::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ফাঁকা, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    স্থির Platform p;
    যদি( !p.p )
    {
        p.p = নতুন Impl;
        p.p->init();
    }
    ফেরত p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
স্থির শূন্য parseOpenCLVersion(ধ্রুবক String &version, পূর্ণসংখ্যা &major, পূর্ণসংখ্যা &minor)
{
    major = minor = 0;
    যদি (10 >= version.length())
        ফেরত;
    ধ্রুবক অক্ষর *pstr = version.c_str();
    যদি (0 != strncmp(pstr, "OpenCL ", 7))
        ফেরত;
    size_t ppos = version.find('.', 7);
    যদি (String::npos == ppos)
        ফেরত;
    String অস্থায়ী_2 = version.substr(7, ppos - 7);
    major = atoi(অস্থায়ী_2.c_str());
    অস্থায়ী_2 = version.substr(ppos + 1);
    minor = atoi(অস্থায়ী_2.c_str());
}

কাঠামো Device::Impl
{
    Impl(শূন্য* d)
        : refcount(1)
        , handle(0)
    {
        চেষ্টা
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        ধরো (...)
        {
            ছোড়ো;
        }
    }

    শূন্য _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, পূর্ণসংখ্যা>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, পূর্ণসংখ্যা>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_Group_আকার_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_আকার);
        type_ = getProp<cl_device_type, পূর্ণসংখ্যা>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        যখন (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            যদি (pos2 == String::npos)
                pos2 = extensions_.size();
            যদি (pos2 > pos)
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
        যদি (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        অন্যথা যদি (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        অন্যথা যদি (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        অন্যথা
            vendorID_ = UNKNOWN_VENDOR;

        ধ্রুবক size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_আকার = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        যদি (CV_OPENCL_DEVICE_MAX_WORK_GROUP_আকার > 0)
        {
            ধ্রুবক size_t new_max_Work_Group_আকার = std::min(max_Work_Group_আকার_2, CV_OPENCL_DEVICE_MAX_WORK_GROUP_আকার);
            যদি (new_max_Work_Group_আকার != max_Work_Group_আকার_2)
                CV_LOG_WARNING(ফাঁকা, "OpenCL: using workgroup size: " << new_max_Work_Group_আকার << " (was " << max_Work_Group_আকার_2 << ")");
            max_Work_Group_আকার_2 = new_max_Work_Group_আকার;
        }
#যদি 0
        যদি (isExtensionSupported("cl_khr_spir"))
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
        যদি (!cv::__termination)
#endif
        {
            যদি (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) ধ্রুবক
    {
        _TpCL অস্থায়ী_2=_TpCL();
        size_t sz = 0;

        ফেরত clGetDeviceInfo(handle, prop, আকার(অস্থায়ী_2), &অস্থায়ী_2, &sz) == CL_SUCCESS &&
            sz == আকার(অস্থায়ী_2) ? _TpOut(অস্থায়ী_2) : _TpOut();
    }

    যৌক্তিক getBoolProp(cl_device_info prop) ধ্রুবক
    {
        cl_bool অস্থায়ী_2 = CL_FALSE;
        size_t sz = 0;

        ফেরত clGetDeviceInfo(handle, prop, আকার(অস্থায়ী_2), &অস্থায়ী_2, &sz) == CL_SUCCESS &&
            sz == আকার(অস্থায়ী_2) ? অস্থায়ী_2 != 0 : মিথ্যা;
    }

    String getStrProp(cl_device_info prop) ধ্রুবক
    {
        অক্ষর buf[4096];
        size_t sz=0;
        ফেরত clGetDeviceInfo(handle, prop, আকার(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < আকার(buf) ? String(buf) : String();
    }

    যৌক্তিক isExtensionSupported(ধ্রুবক std::string& extensionName) ধ্রুবক
    {
        ফেরত extensions_set_.গুনতি_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    পূর্ণসংখ্যা doubleFPConfig_;
    যৌক্তিক khr_fp64_support_;
    পূর্ণসংখ্যা halfFPConfig_;
    যৌক্তিক khr_fp16_support_;
    যৌক্তিক hostUnifiedMemory_;
    পূর্ণসংখ্যা maxComputeUnits_;
    size_t max_Work_Group_আকার_2;
    পূর্ণসংখ্যা type_;
    পূর্ণসংখ্যা addressBits_;
    পূর্ণসংখ্যা deviceVersionMajor_;
    পূর্ণসংখ্যা deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    পূর্ণসংখ্যা vendorID_;
    যৌক্তিক intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(শূন্য* d)
{
    p = 0;
    set(d);
}

Device::Device(ধ্রুবক Device& d)
{
    p = d.p;
    যদি(p)
        p->addref();
}

Device& Device::operator = (ধ্রুবক Device& d)
{
    Impl* newp = (Impl*)d.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    যদি (এই != &d) {
        যদি(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    ফেরত *এই;
}

Device::~Device()
{
    যদি(p)
        p->release();
}

শূন্য Device::set(শূন্য* d)
{
    যদি(p)
        p->release();
    p = নতুন Impl(d);
    যদি (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(শূন্য* d)
{
    Device device(d);
    ফেরত device;
}

শূন্য* Device::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

String Device::name() ধ্রুবক
{ ফেরত p ? p->name_ : String(); }

String Device::extensions() ধ্রুবক
{ ফেরত p ? String(p->extensions_) : String(); }

যৌক্তিক Device::isExtensionSupported(ধ্রুবক String& extensionName) ধ্রুবক
{ ফেরত p ? p->isExtensionSupported(extensionName) : মিথ্যা; }

String Device::version() ধ্রুবক
{ ফেরত p ? p->version_ : String(); }

String Device::vendorName() ধ্রুবক
{ ফেরত p ? p->vendorName_ : String(); }

পূর্ণসংখ্যা Device::vendorID() ধ্রুবক
{ ফেরত p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ধ্রুবক
{ ফেরত p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ধ্রুবক
{ ফেরত p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

পূর্ণসংখ্যা Device::deviceVersionMajor() ধ্রুবক
{ ফেরত p ? p->deviceVersionMajor_ : 0; }

পূর্ণসংখ্যা Device::deviceVersionMinor() ধ্রুবক
{ ফেরত p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ধ্রুবক
{ ফেরত p ? p->driverVersion_ : String(); }

পূর্ণসংখ্যা Device::type() ধ্রুবক
{ ফেরত p ? p->type_ : 0; }

পূর্ণসংখ্যা Device::addressBits() ধ্রুবক
{ ফেরত p ? p->addressBits_ : 0; }

যৌক্তিক Device::available() ধ্রুবক
{ ফেরত p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : মিথ্যা; }

যৌক্তিক Device::compilerAvailable() ধ্রুবক
{ ফেরত p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : মিথ্যা; }

যৌক্তিক Device::linkerAvailable() ধ্রুবক
#ifdef CL_VERSION_1_2
{ ফেরত p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : মিথ্যা; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

পূর্ণসংখ্যা Device::doubleFPConfig() ধ্রুবক
{ ফেরত p ? p->doubleFPConfig_ : 0; }

পূর্ণসংখ্যা Device::singleFPConfig() ধ্রুবক
{ ফেরত p ? p->getProp<cl_device_fp_config, পূর্ণসংখ্যা>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

পূর্ণসংখ্যা Device::halfFPConfig() ধ্রুবক
{ ফেরত p ? p->halfFPConfig_ : 0; }

যৌক্তিক Device::hasFP64() ধ্রুবক
{ ফেরত p ? p->khr_fp64_support_ : মিথ্যা; }
যৌক্তিক Device::hasFP16() ধ্রুবক
{ ফেরত p ? p->khr_fp16_support_ : মিথ্যা; }

যৌক্তিক Device::endianLittle() ধ্রুবক
{ ফেরত p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : মিথ্যা; }

যৌক্তিক Device::errorCorrectionSupport() ধ্রুবক
{ ফেরত p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : মিথ্যা; }

পূর্ণসংখ্যা Device::executionCapabilities() ধ্রুবক
{ ফেরত p ? p->getProp<cl_device_exec_capabilities, পূর্ণসংখ্যা>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_আকার) : 0; }

পূর্ণসংখ্যা Device::globalMemCacheType() ধ্রুবক
{ ফেরত p ? p->getProp<cl_device_mem_cache_type, পূর্ণসংখ্যা>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

পূর্ণসংখ্যা Device::global_Mem_Cache_সারি_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_GLOBAL_MEM_CACHELINE_আকার) : 0; }

size_t Device::global_Mem_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_আকার) : 0; }

size_t Device::local_Mem_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_আকার) : 0; }

পূর্ণসংখ্যা Device::localMemType() ধ্রুবক
{ ফেরত p ? p->getProp<cl_device_local_mem_type, পূর্ণসংখ্যা>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

যৌক্তিক Device::hostUnifiedMemory() ধ্রুবক
{ ফেরত p ? p->hostUnifiedMemory_ : মিথ্যা; }

যৌক্তিক Device::imageSupport() ধ্রুবক
{ ফেরত p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : মিথ্যা; }

যৌক্তিক Device::image_From_বাফার_Support() ধ্রুবক
{
    ফেরত p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : মিথ্যা;
}

uint Device::imagePitchAlignment() ধ্রুবক
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ফেরত p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#অন্যথা
    ফেরত 0;
#endif
}

uint Device::imageBaseAddressAlignment() ধ্রুবক
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    ফেরত p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#অন্যথা
    ফেরত 0;
#endif
}

size_t Device::image2DMaxWidth() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::image_Max_বাফার_আকার() ধ্রুবক
#ifdef CL_VERSION_1_2
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_বাফার_আকার) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::image_Max_Array_আকার() ধ্রুবক
#ifdef CL_VERSION_1_2
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_আকার) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

যৌক্তিক Device::intelSubgroupsSupport() ধ্রুবক
{ ফেরত p ? p->intelSubgroupsSupport_ : মিথ্যা; }

পূর্ণসংখ্যা Device::maxClockFrequency() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

পূর্ণসংখ্যা Device::maxComputeUnits() ধ্রুবক
{ ফেরত p ? p->maxComputeUnits_ : 0; }

পূর্ণসংখ্যা Device::maxConstantArgs() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::max_Constant_বাফার_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_বাফার_আকার) : 0; }

size_t Device::max_Mem_Alloc_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_আকার) : 0; }

size_t Device::max_Parameter_আকার() ধ্রুবক
{ ফেরত p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_আকার) : 0; }

পূর্ণসংখ্যা Device::maxReadImageArgs() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

পূর্ণসংখ্যা Device::maxWriteImageArgs() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

পূর্ণসংখ্যা Device::maxSamplers() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_Group_আকার() ধ্রুবক
{ ফেরত p ? p->max_Work_Group_আকার_2 : 0; }

পূর্ণসংখ্যা Device::maxWorkItemDims() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

শূন্য Device::maxWorkItemSizes(size_t* sizes) ধ্রুবক
{
    যদি(p)
    {
        ধ্রুবক পূর্ণসংখ্যা MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*আকার(sizes[0]), &sizes[0], &retsz));
    }
}

পূর্ণসংখ্যা Device::memBaseAddrAlign() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthChar() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthShort() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthInt() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthLong() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthFloat() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthDouble() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

পূর্ণসংখ্যা Device::nativeVectorWidthHalf() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthChar() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthShort() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthInt() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthLong() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthFloat() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthDouble() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

পূর্ণসংখ্যা Device::preferredVectorWidthHalf() ধ্রুবক
{ ফেরত p ? p->getProp<cl_uint, পূর্ণসংখ্যা>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printf_বাফার_আকার() ধ্রুবক
#ifdef CL_VERSION_1_2
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_বাফার_আকার) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ধ্রুবক
{ ফেরত p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ধ্রুবক Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        ফেরত c.getDevice();
    }

    স্থির Device dummy;
    ফেরত dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
অন্তঃরেখ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ফাঁকা, &required);
    যদি (err != CL_SUCCESS)
        ফেরত err;

    param.clear();
    যদি (required > 0)
    {
        Auto_বাফার<অক্ষর> buf(required + 1);
        অক্ষর* ptr = buf.তথ্য(); // cleanup is not needed
        err = f(obj, name, required, ptr, ফাঁকা);
        যদি (err != CL_SUCCESS)
            ফেরত err;
        param = ptr;
    }

    ফেরত CL_SUCCESS;
}

স্থির শূন্য split(ধ্রুবক std::string &s, অক্ষর delim, std::vector<std::string> &elems)
{
    elems.clear();
    যদি (s.size() == 0)
        ফেরত;
    std::istringstream ss(s);
    std::string item;
    যখন (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
স্থির যৌক্তিক parseOpenCLDeviceConfiguration(ধ্রুবক std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    যদি (parts.size() > 3)
    {
        CV_LOG_ERROR(ফাঁকা, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ফেরত মিথ্যা;
    }
    যদি (parts.size() > 2)
        deviceNameOrID = parts[2];
    যদি (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    যদি (parts.size() > 0)
    {
        platform = parts[0];
    }
    ফেরত সত্য;
}

স্থির cl_device_id selectOpenCLDevice(ধ্রুবক std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    যদি (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    যদি (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        ফেরত ফাঁকা;

    যৌক্তিক isID = মিথ্যা;
    পূর্ণসংখ্যা deviceID = -1;
    যদি (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = সত্য;
        জন্য (size_t i = 0; i < deviceName.length(); i++)
        {
            যদি (!isdigit(deviceName[i]))
            {
                isID = মিথ্যা;
                বিরতি;
            }
        }
        যদি (isID)
        {
            deviceID = atoi(deviceName.c_str());
            যদি (deviceID < 0)
                ফেরত ফাঁকা;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint অঙ্ক_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ফাঁকা, &অঙ্ক_Platforms));

        যদি (অঙ্ক_Platforms == 0)
            ফেরত ফাঁকা;
        platforms.resize((size_t)অঙ্ক_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], &অঙ্ক_Platforms));
        platforms.resize(অঙ্ক_Platforms);
    }

    যদি (platform.length() > 0)
    {
        জন্য (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            যদি (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            অন্যথা
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        যদি (platforms.size() == 0)
        {
            CV_LOG_ERROR(ফাঁকা, "OpenCL: Can't find OpenCL platform by name: " << platform);
            যাও not_found;
        }
    }
    যদি (deviceTypes.size() == 0)
    {
        যদি (!isID)
        {
            deviceTypes.push_back("GPU");
            যদি (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        অন্যথা
            deviceTypes.push_back("ALL");
    }
    জন্য (size_t t = 0; t < deviceTypes.size(); t++)
    {
        পূর্ণসংখ্যা deviceType = 0;
        std::string অস্থায়ী_Str_Device_Type = deviceTypes[t];
        std::transform(অস্থায়ী_Str_Device_Type.begin(), অস্থায়ী_Str_Device_Type.end(), অস্থায়ী_Str_Device_Type.begin(), details::char_tolower);

        যদি (অস্থায়ী_Str_Device_Type == "gpu" || অস্থায়ী_Str_Device_Type == "dgpu" || অস্থায়ী_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        অন্যথা যদি (অস্থায়ী_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        অন্যথা যদি (অস্থায়ী_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        অন্যথা যদি (অস্থায়ী_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        অন্যথা
        {
            CV_LOG_ERROR(ফাঁকা, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            যাও not_found;
        }

        std::vector<cl_device_id> devices;
        জন্য (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint গুনতি_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, ফাঁকা, &গুনতি_2);
            যদি (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_ফলাফল(status, "clGetDeviceIDs get count");
            }
            যদি (গুনতি_2 == 0)
                চালু;
            size_t base = devices.size();
            devices.resize(base + গুনতি_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, গুনতি_2, &devices[base], &গুনতি_2);
            যদি (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_ফলাফল(status, "clGetDeviceIDs get IDs");
            }
        }

        জন্য (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : সত্য) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = সত্য;
            যদি(অস্থায়ী_Str_Device_Type == "dgpu" || অস্থায়ী_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, আকার(isIGPU), &isIGPU, ফাঁকা));
                useGPU = অস্থায়ী_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            যদি ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ফেরত devices[i];
            }
        }
    }

not_found:
    যদি (configuration.empty())
        ফেরত ফাঁকা; // suppress messages on stderr

    std::ostringstream বার্তা;
    বার্তা << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    জন্য (size_t t = 0; t < deviceTypes.size(); t++)
        বার্তা << ' ' << deviceTypes[t];

    বার্তা << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ফাঁকা, বার্তা.str());
    ফেরত ফাঁকা;
}

#ifdef HAVE_OPENCL_SVM
নামাকাশ svm {

গণনা AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_বাফার = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_বাফার = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_বাফার_MASK = 3 << 16,
        OPENCL_SVM_বাফার_MAP = 4 << 16
};

স্থির যৌক্তিক checkForceSVMUmatUsage()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক force = মিথ্যা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত force;
}
স্থির যৌক্তিক checkDisableSVMUMatUsage()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক force = মিথ্যা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত force;
}
স্থির যৌক্তিক checkDisableSVM()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক force = মিথ্যা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত force;
}
// see SVMCapabilities
স্থির unsigned পূর্ণসংখ্যা getSVMCapabilitiesMask()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির unsigned পূর্ণসংখ্যা mask = 0;
    যদি (!initialized)
    {
        ধ্রুবক std::string env_মান = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        যদি (env_মান.empty())
        {
            ফেরত ~0U; // all bits 1
        }
        mask = atoi(env_মান.c_str());
        initialized = সত্য;
    }
    ফেরত mask;
}
} // namespace
#endif

স্থির size_t get_Program_গুনতি_Limit()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির size_t গুনতি_2 = 0;
    যদি (!initialized)
    {
        গুনতি_2 = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = সত্য;
    }
    ফেরত গুনতি_2;
}

স্থির পূর্ণসংখ্যা g_contextId = 0;

শ্রেণী OpenCLBufferPoolImpl;
শ্রেণী OpenCLSVMBufferPoolImpl;

কাঠামো Context::Impl
{
    স্থির Context::Impl* get(Context& context) { ফেরত context.getImpl(); }

    প্রকারলেখ std::deque<Context::Impl*> container_t;
    স্থির container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        স্থির container_t* g_contexts = নতুন container_t();
        ফেরত *g_contexts;
    }

সুরক্ষিত:
    Impl(ধ্রুবক std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(মিথ্যা)
#endif
    {
        যদি (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = এই;
    }

    ~Impl()
    {
#ifdef _WIN32
        যদি (!cv::__termination)
#endif
        {
            যদি (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ফাঁকা;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ফাঁকা;
        }
    }

    শূন্য init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_অঙ্ক_DEVICES, আকার(ndevices), &ndevices, ফাঁকা));
        CV_Assert(ndevices > 0);

        cv::Auto_বাফার<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_আকার = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * আকার(cl_device_id), &cl_devices[0], &devices_ret_আকার));
        CV_CheckEQ(devices_ret_আকার, cl_devices.size() * আকার(cl_device_id), "");

        devices.clear();
        জন্য (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    শূন্য __init_buffer_pools();  // w/o synchronization
    শূন্য init_বাফার_pools() ধ্রুবক
    {
        যদি (!বাফার_Pool_2)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            যদি (!বাফার_Pool_2)
            {
                const_cast<Impl*>(এই)->__init_buffer_pools();
            }
        }
    }
সর্বজনীন:
    স্থির Impl* findContext(ধ্রুবক std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        যদি (configuration.empty() && !container.empty())
            ফেরত container[0];
        জন্য (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            যদি (i && i->configuration == configuration)
            {
                ফেরত i;
            }
        }
        ফেরত ফাঁকা;
    }

    স্থির Impl* findOrCreateContext(ধ্রুবক std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        যদি (configuration_.empty())
        {
            ধ্রুবক std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            যদি (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        যদি (impl)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ফেরত impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        যদি (d == ফাঁকা)
            ফেরত ফাঁকা;

        impl = নতুন Impl(configuration);
        চেষ্টা
        {
            impl->createFromDevice(d);
            যদি (impl->handle)
                ফেরত impl;
            delete impl;
            ফেরত ফাঁকা;
        }
        ধরো (...)
        {
            delete impl;
            ছোড়ো;
        }
    }

    স্থির Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (শূন্য*)h);
        Impl* impl = findContext(configuration);
        যদি (impl)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ফেরত impl;
        }

        impl = নতুন Impl(configuration);
        চেষ্টা
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ফেরত impl;
        }
        ধরো (...)
        {
            delete impl;
            ছোড়ো;
        }
    }

    স্থির Impl* findOrCreateContext(ধ্রুবক ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (শূন্য*)d);
        Impl* impl = findContext(configuration);
        যদি (impl)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ফেরত impl;
        }

        impl = নতুন Impl(configuration);
        চেষ্টা
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ফেরত impl;
        }
        ধরো (...)
        {
            delete impl;
            ছোড়ো;
        }
    }

    শূন্য setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        যদি (d == ফাঁকা)
            ফেরত;

        createFromDevice(d);
    }

    শূন্য createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ফাঁকা);

        cl_platform_id pl = ফাঁকা;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, আকার(cl_platform_id), &pl, ফাঁকা));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int status;

        handle = clCreateContext(prop, nd, &d, 0, 0, &status);
        CV_OCL_DBG_CHECK_ফলাফল(status, "clCreateContext");

        যৌক্তিক ok = handle != 0 && status == CL_SUCCESS;
        যদি( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        অন্যথা
            handle = ফাঁকা;
    }

    Program getProg(ধ্রুবক ProgramSource& src, ধ্রুবক String& buildflags, String& errmsg);

    শূন্য unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        জন্য (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              যদি (it != phash.end())
              {
                  যদি (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ফেরত;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        যদি (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            যদি (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ধ্রুবক Device& d = devices[0];
                পূর্ণসংখ্যা bits = d.addressBits();
                যদি (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                জন্য (size_t i = 0; i < prefix.size(); i++)
                {
                    অক্ষর c = prefix[i];
                    যদি (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        ফেরত prefix;
    }

    std::string& getPrefixBase()
    {
        যদি (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            যদি (prefix_base.empty())
            {
                ধ্রুবক Device& d = devices[0];
                পূর্ণসংখ্যা bits = d.addressBits();
                যদি (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                জন্য (size_t i = 0; i < prefix_base.size(); i++)
                {
                    অক্ষর c = prefix_base[i];
                    যদি (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        ফেরত prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ধ্রুবক পূর্ণসংখ্যা contextId;  // global unique ID
    ধ্রুবক std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    প্রকারলেখ std::map<std::string, Program> phash_t;
    phash_t phash;
    প্রকারলেখ std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> বাফার_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> বাফার_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_বাফার_Pool() ধ্রুবক
    {
        init_বাফার_pools();
        CV_DbgAssert(বাফার_Pool_2);
        ফেরত *বাফার_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_বাফার_Pool_Host_Ptr() ধ্রুবক
    {
        init_বাফার_pools();
        CV_DbgAssert(বাফার_Pool_Host_Ptr);
        ফেরত *বাফার_Pool_Host_Ptr.get();
    }

    std::map<std::type_সূচক, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    শূন্য setUserContext(std::type_সূচক typeId, ধ্রুবক std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_সূচক typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        যদি (it != userContextStorage.end())
            ফেরত it->second;
        অন্যথা
            ফেরত nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    যৌক্তিক svmInitialized;
    যৌক্তিক svmAvailable;
    যৌক্তিক svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    শূন্য svmInit()
    {
        CV_Assert(handle != ফাঁকা);
        ধ্রুবক Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((শূন্য)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, আকার(deviceCaps), &deviceCaps, ফাঁকা);
        যদি (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            যাও noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (পূর্ণসংখ্যা)deviceCaps);
        CV_Assert(((শূন্য)0, CL_DEVICE_SVM_COARSE_GRAIN_বাফার == CL_DEVICE_SVM_COARSE_GRAIN_বাফার_AMD)); // Check assumption
        svmCapabilities.মান_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_বাফার) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_বাফার : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_বাফার) ? svm::SVMCapabilities::SVM_FINE_GRAIN_বাফার : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.মান_2 &= svm::getSVMCapabilitiesMask();
        যদি (svmCapabilities.মান_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            যাও noSVM;
        }
        চেষ্টা
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            শূন্য* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            যদি (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            চেষ্টা
            {
                যৌক্তিক error = মিথ্যা;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                যদি (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ফাঁকা, ফাঁকা))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                চেষ্টা
                {
                    ((পূর্ণসংখ্যা*)ptr)[0] = 100;
                }
                ধরো (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = সত্য;
                }
                যদি (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, ফাঁকা, ফাঁকা))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                যদি (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            ধরো (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ছোড়ো;
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
        ধরো (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            চেষ্টা
            {
                // Try HSA extension
                String extensions = device.extensions();
                যদি (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    যাও noSVM;
                }
                cl_platform_id p = ফাঁকা;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, আকার(cl_platform_id), &p, ফাঁকা));
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
            ধরো (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                যাও noSVM;
            }
        }

        svmAvailable = সত্য;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = সত্য;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ফেরত;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = মিথ্যা;
        svmEnabled = মিথ্যা;
        svmCapabilities.মান_2 = 0;
        svmInitialized = সত্য;
        svmFunctions.fn_clSVMAlloc = ফাঁকা;
        ফেরত;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> বাফার_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_বাফার_Pool_SVM() ধ্রুবক
    {
        init_বাফার_pools();
        CV_DbgAssert(বাফার_Pool_SVM);
        ফেরত *বাফার_Pool_SVM.get();
    }
#endif

    friend শ্রেণী Program;
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
Context::Context(পূর্ণসংখ্যা dtype)
{
    p = 0;
    create(dtype);
}

শূন্য Context::release()
{
    যদি (p)
    {
        p->release();
        p = ফাঁকা;
    }
}

যৌক্তিক Context::create()
{
    release();
    যদি (!haveOpenCL())
        ফেরত মিথ্যা;
    p = Impl::findOrCreateContext(std::string());
    যদি (p && p->handle)
        ফেরত সত্য;
    release();
    ফেরত মিথ্যা;
}

// deprecated
যৌক্তিক Context::create(পূর্ণসংখ্যা dtype)
{
    যদি( !haveOpenCL() )
        ফেরত মিথ্যা;
    release();
    যদি (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    অন্যথা যদি (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    অন্যথা যদি (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    অন্যথা
    {
        CV_LOG_ERROR(ফাঁকা, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    যদি (p && !p->handle)
    {
        release();
    }
    ফেরত p != 0;
}

Context::Context(ধ্রুবক Context& c)
{
    p = (Impl*)c.p;
    যদি(p)
        p->addref();
}

Context& Context::operator = (ধ্রুবক Context& c)
{
    Impl* newp = (Impl*)c.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    যদি (এই != &c) {
        যদি(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    ফেরত *এই;
}

শূন্য* Context::ptr() ধ্রুবক
{
    ফেরত p == ফাঁকা ? ফাঁকা : p->handle;
}

size_t Context::ndevices() ধ্রুবক
{
    ফেরত p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ধ্রুবক
{
    স্থির Device dummy;
    ফেরত !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        auto& ctx = c.getContext();
        ফেরত ctx;
    }

    স্থির Context dummy;
    ফেরত dummy;
}

Program Context::getProg(ধ্রুবক ProgramSource& prog,
                         ধ্রুবক String& buildopts, String& errmsg)
{
    ফেরত p ? p->getProg(prog, buildopts, errmsg) : Program();
}

শূন্য Context::unloadProg(Program& prog)
{
    যদি (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(শূন্য* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    ফেরত ctx;
}

/* static */
Context Context::fromDevice(ধ্রুবক ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    ফেরত ctx;
}

/* static */
Context Context::create(ধ্রুবক std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    ফেরত ctx;
}

শূন্য* Context::getOpenCLContextProperty(পূর্ণসংখ্যা propertyId) ধ্রুবক
{
    যদি (p == ফাঁকা)
        ফেরত nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ফাঁকা, &size));
    std::vector<cl_context_properties> prop(size / আকার(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.তথ্য(), ফাঁকা));
    জন্য (size_t i = 0; i < prop.size(); i += 2)
    {
        যদি (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ফাঁকা, "OpenCL: found context property=" << propertyId << ") => " << (শূন্য*)prop[i + 1]);
            ফেরত (শূন্য*)prop[i + 1];
        }
    }
    ফেরত nullptr;
}

#ifdef HAVE_OPENCL_SVM
যৌক্তিক Context::useSVM() ধ্রুবক
{
    Context::Impl* i = p;
    CV_Assert(i);
    যদি (!i->svmInitialized)
        i->svmInit();
    ফেরত i->svmEnabled;
}
শূন্য Context::setUseSVM(যৌক্তিক enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    যদি (!i->svmInitialized)
        i->svmInit();
    যদি (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#অন্যথা
যৌক্তিক Context::useSVM() ধ্রুবক { ফেরত মিথ্যা; }
শূন্য Context::setUseSVM(যৌক্তিক enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
নামাকাশ svm {

ধ্রুবক SVMCapabilities getSVMCapabilitites(ধ্রুবক ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    যদি (!i->svmInitialized)
        i->svmInit();
    ফেরত i->svmCapabilities;
}

CV_EXPORTS ধ্রুবক SVMFunctions* getSVMFunctions(ধ্রুবক ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ফাঁকা);
    ফেরত &i->svmFunctions;
}

CV_EXPORTS যৌক্তিক useSVM(UMatUsageFlags usageFlags)
{
    যদি (checkForceSVMUmatUsage())
        ফেরত সত্য;
    যদি (checkDisableSVMUMatUsage())
        ফেরত মিথ্যা;
    যদি ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        ফেরত সত্য;
    ফেরত মিথ্যা; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

শূন্য Context::setUserContext(std::type_সূচক typeId, ধ্রুবক std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_সূচক typeId)
{
    CV_Assert(p);
    ফেরত p->getUserContext(typeId);
}

স্থির শূন্য get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_বাফার<অক্ষর> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.তথ্য(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.তথ্য();
}

/*
// Attaches OpenCL context to OpenCV
*/
শূন্য attachContext(ধ্রুবক String& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ধ্রুবক std::string& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID
)
{
    যদি (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    যদি (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    যৌক্তিক platformAvailable = মিথ্যা;

    // check if external platformName contained in list of available platforms in OpenCV
    জন্য (unsigned পূর্ণসংখ্যা i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        যদি (platformName == availablePlatformName)
        {
            platformAvailable = সত্য;
            বিরতি;
        }
    }

    যদি (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    যদি (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    ফেরত ctx;
}

শূন্য initializeContextFromHandle(Context& ctx, শূন্য* _platform, শূন্য* _context, শূন্য* _device)
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

কাঠামো Queue::Impl
{
    অন্তঃরেখ শূন্য __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = মিথ্যা;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, আকার(cl_command_queue_properties), &props, ফাঁকা));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, যৌক্তিক isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ধ্রুবক Context& c, ধ্রুবক Device& d, যৌক্তিক withProfiling = মিথ্যা)
    {
        __init();

        ধ্রুবক Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        যদি( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        যদি( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        যদি (!cv::__termination)
#endif
        {
            যদি(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = ফাঁকা;
            }
        }
    }

    ধ্রুবক cv::ocl::Queue& getProfilingQueue(ধ্রুবক cv::ocl::Queue& self)
    {
        যদি (isProfilingQueue_)
            ফেরত self;

        যদি (profiling_queue_.ptr())
            ফেরত profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, আকার(cl_context), &ctx, ফাঁকা));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, আকার(cl_device_id), &device, ফাঁকা));

        cl_int ফলাফল = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &ফলাফল);
        CV_OCL_DBG_CHECK_ফলাফল(ফলাফল, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = নতুন Impl(q, সত্য);
        profiling_queue_ = queue;

        ফেরত profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    যৌক্তিক isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ধ্রুবক Context& c, ধ্রুবক Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ধ্রুবক Queue& q)
{
    p = q.p;
    যদি(p)
        p->addref();
}

Queue& Queue::operator = (ধ্রুবক Queue& q)
{
    Impl* newp = (Impl*)q.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    যদি (এই != &q) {
        যদি(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    ফেরত *এই;
}

Queue::~Queue()
{
    যদি(p)
        p->release();
}

যৌক্তিক Queue::create(ধ্রুবক Context& c, ধ্রুবক Device& d)
{
    যদি(p)
        p->release();
    p = নতুন Impl(c, d);
    ফেরত p->handle != 0;
}

শূন্য Queue::finish()
{
    যদি(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ধ্রুবক Queue& Queue::getProfilingQueue() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->getProfilingQueue(*এই);
}

শূন্য* Queue::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        auto& q = c.getQueue();
        ফেরত q;
    }
    স্থির Queue dummy;
    ফেরত dummy;
}

স্থির cl_command_queue getQueue(ধ্রুবক Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    যদি(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    ফেরত qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(পূর্ণসংখ্যা _flags, UMat* _m, পূর্ণসংখ্যা _wscale, পূর্ণসংখ্যা _iwscale, ধ্রুবক শূন্য* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ফাঁকা);
}

KernelArg KernelArg::Constant(ধ্রুবক Mat& m)
{
    CV_Assert(m.isContinuous());
    ফেরত KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.যোগ()*m.elem_আকার());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

কাঠামো Kernel::Impl
{
    Impl(ধ্রুবক অক্ষর* kname, ধ্রুবক Program& prog) :
        refcount(1), handle(ফাঁকা), isInProgress(মিথ্যা), isAsyncRun(মিথ্যা), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        যদি (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        জন্য( পূর্ণসংখ্যা i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_অস্থায়ী_Dst_UMats = মিথ্যা;
        have_অস্থায়ী_Src_UMats = মিথ্যা;
    }

    শূন্য cleanupUMats()
    {
        যৌক্তিক exceptionOccurred = মিথ্যা;
        জন্য( পূর্ণসংখ্যা i = 0; i < MAX_ARRS; i++ )
        {
            যদি( u[i] )
            {
                যদি( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_তথ্য::ASYNC_CLEANUP;
                    চেষ্টা
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    ধরো(ধ্রুবক std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ফাঁকা, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = সত্য;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_অস্থায়ী_Dst_UMats = মিথ্যা;
        have_অস্থায়ী_Src_UMats = মিথ্যা;
        CV_Assert(!exceptionOccurred);
    }

    শূন্য addUMat(ধ্রুবক UMat& m, যৌক্তিক dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        যদি(dst && m.u->অস্থায়ী_UMat())
            have_অস্থায়ী_Dst_UMats = সত্য;
        যদি(m.u->original_UMat_তথ্য == ফাঁকা && m.u->অস্থায়ী_UMat())
            have_অস্থায়ী_Src_UMats = সত্য;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    শূন্য registerImageArgument(পূর্ণসংখ্যা arg, ধ্রুবক Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        যদি (arg < (পূর্ণসংখ্যা)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    শূন্য finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = মিথ্যা;
        চেষ্টা
        {
            cleanupUMats();
        }
        ধরো(...)
        {
            release();
            ছোড়ো;
        }
        release();
    }

    যৌক্তিক run(পূর্ণসংখ্যা dims, size_t _globalsize[], size_t _localsize[],
            যৌক্তিক সমলয়, int64* timeNS, ধ্রুবক Queue& q);

    ~Impl()
    {
        যদি(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    গণনা { MAX_ARRS = 16 };
    UMat_তথ্য* u[MAX_ARRS];
    যৌক্তিক isInProgress;
    যৌক্তিক isAsyncRun;  // true if kernel was scheduled in async mode
    পূর্ণসংখ্যা nu;
    std::vector<Image2D> shadow_images;
    যৌক্তিক have_অস্থায়ী_Dst_UMats;
    যৌক্তিক have_অস্থায়ী_Src_UMats;
};

}} // namespace cv::ocl

বাহ্যিক "C" {

স্থির শূন্য CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, শূন্য *p)
{
    চেষ্টা
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    ধরো (ধ্রুবক cv::Exception& exc)
    {
        CV_LOG_ERROR(ফাঁকা, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    ধরো (ধ্রুবক std::exception& exc)
    {
        CV_LOG_ERROR(ফাঁকা, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    ধরো (...)
    {
        CV_LOG_ERROR(ফাঁকা, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

নামাকাশ cv { নামাকাশ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ধ্রুবক অক্ষর* kname, ধ্রুবক Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ধ্রুবক অক্ষর* kname, ধ্রুবক ProgramSource& src,
               ধ্রুবক String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ধ্রুবক Kernel& k)
{
    p = k.p;
    যদি(p)
        p->addref();
}

Kernel& Kernel::operator = (ধ্রুবক Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    যদি (এই != &k) {
        যদি(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    ফেরত *এই;
}

Kernel::~Kernel()
{
    যদি(p)
        p->release();
}

যৌক্তিক Kernel::create(ধ্রুবক অক্ষর* kname, ধ্রুবক Program& prog)
{
    যদি(p)
        p->release();
    p = নতুন Impl(kname, prog);
    যদি(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    ফেরত p != 0;
}

যৌক্তিক Kernel::create(ধ্রুবক অক্ষর* kname, ধ্রুবক ProgramSource& src,
                    ধ্রুবক String& buildopts, String* errmsg)
{
    যদি(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    যদি( !errmsg ) errmsg = &tempmsg;
    ধ্রুবক Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ফেরত create(kname, prog);
}

শূন্য* Kernel::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

যৌক্তিক Kernel::empty() ধ্রুবক
{
    ফেরত ptr() == 0;
}

স্থির cv::String dump_মান(size_t sz, ধ্রুবক শূন্য* p)
{
    যদি (!p)
        ফেরত "NULL";
    যদি (sz == 2)
        ফেরত cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    যদি (sz == 4)
        ফেরত cv::format("%d / %uu / 0x%08x / %g", *(পূর্ণসংখ্যা*)p, *(পূর্ণসংখ্যা*)p, *(পূর্ণসংখ্যা*)p, *(দশমিক*)p);
    যদি (sz == 8)
        ফেরত cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(দ্বিগুণ*)p);
    ফেরত cv::format("%p", p);
}

পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক শূন্য* value, size_t sz)
{
    যদি (!p || !p->handle)
        ফেরত -1;
    যদি (i < 0)
        ফেরত i;
    যদি( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (পূর্ণসংখ্যা)i, (পূর্ণসংখ্যা)sz, dump_মান(sz, value).c_str()).c_str());
    যদি (retval != CL_SUCCESS)
        ফেরত -1;
    ফেরত i+1;
}

পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    পূর্ণসংখ্যা res = set(i, &h, আকার(h));
    যদি (res >= 0)
        p->registerImageArgument(i, image2D);
    ফেরত res;
}

পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক UMat& m)
{
    ফেরত set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
স্থির যৌক্তিক isCudaBacked(ধ্রুবক UMat_তথ্য* u)
{
    ধ্রুবক MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ফেরত u && cudaAllocator && u->currAllocator == cudaAllocator;
}

পূর্ণসংখ্যা Kernel::set(পূর্ণসংখ্যা i, ধ্রুবক KernelArg& arg)
{
    যদি( !p || !p->handle )
        ফেরত -1;
    যদি (i < 0)
    {
        CV_LOG_ERROR(ফাঁকা, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (পূর্ণসংখ্যা)i));
        ফেরত i;
    }
    যদি( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    যদি( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        যৌক্তিক ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        যদি (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ফাঁকা;
            status = clSetKernelArg(p->handle, (cl_uint)i, আকার(h_null), &h_null);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (পূর্ণসংখ্যা)i).c_str());
            ফেরত i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        যদি (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ফাঁকা, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (পূর্ণসংখ্যা)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ফেরত -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        যদি (!h)
        {
            CV_LOG_ERROR(ফাঁকা, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (পূর্ণসংখ্যা)i, (পূর্ণসংখ্যা)arg.flags, arg.m));
            p->release();
            p = 0;
            ফেরত -1;
        }

#ifdef HAVE_OPENCL_SVM
        যদি ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
        {
            ধ্রুবক Context& ctx = Context::getDefault();
            ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_তথ্য_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_তথ্য_Ptr);
#যদি 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svm_তথ্য_Ptr);
#অন্যথা
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svm_তথ্য_Ptr);
#endif
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (পূর্ণসংখ্যা)i, (শূন্য*)svm_তথ্য_Ptr).c_str());
        }
        অন্যথা
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, আকার(h), &h);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (পূর্ণসংখ্যা)i, (শূন্য*)h).c_str());
        }

        যদি (ptronly)
        {
            i++;
        }
        অন্যথা যদি( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), আকার(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+1), (পূর্ণসংখ্যা)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), আকার(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+2), (পূর্ণসংখ্যা)u2d.offset).c_str());
            i += 3;

            যদি( !(arg.flags & KernelArg::NO_আকার) )
            {
                পূর্ণসংখ্যা cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, আকার(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)i, (পূর্ণসংখ্যা)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), আকার(cols), &cols);
                CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+1), (পূর্ণসংখ্যা)cols).c_str());
                i += 2;
            }
        }
        অন্যথা
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), আকার(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+1), (পূর্ণসংখ্যা)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), আকার(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+2), (পূর্ণসংখ্যা)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), আকার(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+3), (পূর্ণসংখ্যা)u3d.offset).c_str());
            i += 4;
            যদি( !(arg.flags & KernelArg::NO_আকার) )
            {
                পূর্ণসংখ্যা cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, আকার(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)i, (পূর্ণসংখ্যা)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), আকার(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+1), (পূর্ণসংখ্যা)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), আকার(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (পূর্ণসংখ্যা)(i+2), (পূর্ণসংখ্যা)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ফেরত i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_ফলাফল(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (পূর্ণসংখ্যা)i, (পূর্ণসংখ্যা)arg.sz, (শূন্য*)arg.obj).c_str());
    ফেরত i+1;
}

যৌক্তিক Kernel::run(পূর্ণসংখ্যা dims, size_t _globalsize[], size_t _localsize[],
                 যৌক্তিক সমলয়, ধ্রুবক Queue& q)
{
    যদি (!p)
        ফেরত মিথ্যা;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t যোগ = 1;
    CV_Assert(_globalsize != ফাঁকা);
    জন্য (পূর্ণসংখ্যা i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(পূর্ণসংখ্যা)(i>0)) : 1;
        CV_Assert( val > 0 );
        যোগ *= _globalsize[i];
        যদি (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned পূর্ণসংখ্যা)val) * val;
    }
    CV_Assert(যোগ > 0);

    ফেরত p->run(dims, globalsize, _localsize, সমলয়, ফাঁকা, q);
}


যৌক্তিক Kernel::run_(পূর্ণসংখ্যা dims, size_t _globalsize[], size_t _localsize[],
                  যৌক্তিক সমলয়, ধ্রুবক Queue& q)
{
    CV_Assert(p);
    ফেরত p->run(dims, _globalsize, _localsize, সমলয়, ফাঁকা, q);
}


যৌক্তিক Kernel::Impl::run(পূর্ণসংখ্যা dims, size_t globalsize[], size_t localsize[],
        যৌক্তিক সমলয়, int64* timeNS, ধ্রুবক Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    যদি (!handle)
    {
        CV_LOG_ERROR(ফাঁকা, "OpenCL kernel has zero handle: " << name);
        ফেরত মিথ্যা;
    }

    যদি (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !সমলয়;

    যদি (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#যদি CV_OPENCL_SYNC_RUN_KERNELS
    সমলয় = সত্য;
#endif

    cl_command_queue qq = getQueue(q);
    যদি (have_অস্থায়ী_Dst_UMats)
        সমলয় = সত্য;
    যদি (have_অস্থায়ী_Src_UMats)
        সমলয় = সত্য;
    যদি (timeNS)
        সমলয় = সত্য;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           ফাঁকা, globalsize, localsize, 0, 0,
                                           (সমলয় && !timeNS) ? 0 : &asyncEvent);
#যদি !CV_OPENCL_SHOW_RUN_KERNELS
    যদি (retval != CL_SUCCESS)
#endif
    {
        cv::String বার্তা = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (পূর্ণসংখ্যা)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        সমলয় ? "true" : "false"
                        );
        যদি (retval != CL_SUCCESS)
        {
            বার্তা = CV_OCL_API_ERROR_বার্তা(retval, বার্তা.c_str());
        }
#যদি CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_ফলাফল(retval, বার্তা.c_str());
#অন্যথা
        printf("%s\n", বার্তা.c_str());
        fflush(stdout);
#endif
    }
    যদি (সমলয় || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        যদি (timeNS)
        {
            যদি (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, আকার(startTime), &startTime, ফাঁকা));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, আকার(stopTime), &stopTime, ফাঁকা));
                *timeNS = (int64)(stopTime - startTime);
            }
            অন্যথা
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    অন্যথা
    {
        addref();
        isInProgress = সত্য;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, এই));
    }
    যদি (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ফেরত retval == CL_SUCCESS;
}

যৌক্তিক Kernel::runTask(যৌক্তিক সমলয়, ধ্রুবক Queue& q)
{
    যদি(!p || !p->handle || p->isInProgress)
        ফেরত মিথ্যা;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, সমলয় ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), সমলয় ? "true" : "false").c_str());
    যদি (সমলয় || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    অন্যথা
    {
        p->addref();
        p->isInProgress = সত্য;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    যদি (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ফেরত retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(পূর্ণসংখ্যা dims, size_t globalsize[], size_t localsize[], ধ্রুবক Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    যৌক্তিক res = p->run(dims, globalsize, localsize, সত্য, &timeNs, profilingQueue);
    ফেরত res ? timeNs : -1;
}

size_t Kernel::work_Group_আকার() ধ্রুবক
{
    যদি(!p || !p->handle)
        ফেরত 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_আকার, আকার(val), &val, &retsz);
    CV_OCL_CHECK_ফলাফল(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ফেরত status == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_Group_আকার_Multiple() ধ্রুবক
{
    যদি(!p || !p->handle)
        ফেরত 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_আকার_MULTIPLE, আকার(val), &val, &retsz);
    CV_OCL_CHECK_ফলাফল(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ফেরত status == CL_SUCCESS ? val : 0;
}

যৌক্তিক Kernel::compile_Work_Group_আকার(size_t wsz[]) ধ্রুবক
{
    যদি(!p || !p->handle || !wsz)
        ফেরত 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_আকার, আকার(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_ফলাফল(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ফেরত status == CL_SUCCESS;
}

size_t Kernel::local_Mem_আকার() ধ্রুবক
{
    যদি(!p || !p->handle)
        ফেরত 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_আকার, আকার(val), &val, &retsz);
    CV_OCL_CHECK_ফলাফল(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ফেরত status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

কাঠামো ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    গণনা KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ধ্রুবক String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ধ্রুবক String& module, ধ্রুবক String& name, ধ্রুবক String& codeStr, ধ্রুবক String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    শূন্য init(গণনা KIND kind, ধ্রুবক String& module, ধ্রুবক String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = ফাঁকা;
        source_আকার = 0;
        isHashUpdated = মিথ্যা;
    }

    শূন্য initFromSource(ধ্রুবক String& codeStr, ধ্রুবক String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        যদি (sourceHash_.empty())
        {
            updateHash();
        }
        অন্যথা
        {
            isHashUpdated = সত্য;
        }
    }

    শূন্য updateHash(ধ্রুবক অক্ষর* hashStr = ফাঁকা)
    {
        যদি (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = সত্য;
            ফেরত;
        }
        uint64 hash = 0;
        নির্বাচন (kind_)
        {
        ক্ষেত্র PROGRAM_SOURCE_CODE:
            যদি (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, source_আকার); // static storage
            }
            অন্যথা
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            বিরতি;
        ক্ষেত্র PROGRAM_BINARIES:
        ক্ষেত্র PROGRAM_SPIR:
        ক্ষেত্র PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, source_আকার);
            বিরতি;
        পূর্বনির্ধারিত:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = সত্য;
    }

    Impl(গণনা KIND kind,
            ধ্রুবক String& module, ধ্রুবক String& name,
            ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size,
            ধ্রুবক cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        source_আকার = size;

        buildOptions_ = buildOptions;
    }

    স্থির ProgramSource fromSourceWithStaticLifetime(ধ্রুবক String& module, ধ্রুবক String& name,
            ধ্রুবক অক্ষর* sourceCodeStaticStr, ধ্রুবক অক্ষর* hashStaticStr,
            ধ্রুবক cv::String& buildOptions)
    {
        ProgramSource ফলাফল;
        ফলাফল.p = নতুন Impl(PROGRAM_SOURCE_CODE, module, name,
                (ধ্রুবক unsigned অক্ষর*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        ফলাফল.p->updateHash(hashStaticStr);
        ফেরত ফলাফল;
    }

    স্থির ProgramSource from_দ্বিমিক(ধ্রুবক String& module, ধ্রুবক String& name,
            ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size,
            ধ্রুবক cv::String& buildOptions)
    {
        ProgramSource ফলাফল;
        ফলাফল.p = নতুন Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        ফেরত ফলাফল;
    }

    স্থির ProgramSource fromSPIR(ধ্রুবক String& module, ধ্রুবক String& name,
            ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size,
            ধ্রুবক cv::String& buildOptions)
    {
        ProgramSource ফলাফল;
        ফলাফল.p = নতুন Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        ফেরত ফলাফল;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ধ্রুবক unsigned অক্ষর* sourceAddr_;
    size_t source_আকার;

    cv::String buildOptions_;

    String sourceHash_;
    যৌক্তিক isHashUpdated;

    friend কাঠামো Program::Impl;
    friend কাঠামো internal::ProgramEntry;
    friend কাঠামো Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ধ্রুবক String& module, ধ্রুবক String& name, ধ্রুবক String& codeStr, ধ্রুবক String& codeHash)
{
    p = নতুন Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ধ্রুবক অক্ষর* prog)
{
    p = নতুন Impl(prog);
}

ProgramSource::ProgramSource(ধ্রুবক String& prog)
{
    p = নতুন Impl(prog);
}

ProgramSource::~ProgramSource()
{
    যদি(p)
        p->release();
}

ProgramSource::ProgramSource(ধ্রুবক ProgramSource& prog)
{
    p = prog.p;
    যদি(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ধ্রুবক ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    যদি (এই != &prog) {
        যদি(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ফেরত *এই;
}

ধ্রুবক String& ProgramSource::source() ধ্রুবক
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == ফাঁকা); // method returns reference - can't construct temporary object
    ফেরত p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ধ্রুবক
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_দ্বিমিক(ধ্রুবক String& module, ধ্রুবক String& name,
        ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size,
        ধ্রুবক cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ফেরত Impl::from_দ্বিমিক(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ধ্রুবক String& module, ধ্রুবক String& name,
        ধ্রুবক unsigned অক্ষর* binary, ধ্রুবক size_t size,
        ধ্রুবক cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ফেরত Impl::from_দ্বিমিক(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ধ্রুবক
{
    যদি (এই->pProgramSource == ফাঁকা)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (এই->pProgramSource == ফাঁকা)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(এই->module, এই->name, এই->programCode, এই->programHash, cv::String());
            ProgramSource* ptr = নতুন ProgramSource(ps);
            const_cast<ProgramEntry*>(এই)->pProgramSource = ptr;
        }
    }
    ফেরত *এই->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

স্থির
cv::String joinBuildOptions(ধ্রুবক cv::String& a, ধ্রুবক cv::String& b)
{
    যদি (b.empty())
        ফেরত a;
    যদি (a.empty())
        ফেরত b;
    যদি (b[0] == ' ')
        ফেরত a + b;
    ফেরত a + (cv::String(" ") + b);
}

কাঠামো Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ধ্রুবক ProgramSource& src,
         ধ্রুবক String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ফাঁকা),
         buildflags(_buildflags)
    {
        ধ্রুবক ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ধ্রুবক Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        যদি (ctx.ptr() == ফাঁকা || device.ptr() == ফাঁকা)
            ফেরত;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        যদি (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            যদি (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            অন্যথা যদি (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ধ্রুবক String param_buildExtraOptions = getBuildExtraOptions();
            যদি (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#যদি CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ফাঁকা, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#যদি CV_OPENCL_SHOW_BUILD_KERNELS
        যদি (handle)
        {
            size_t retsz = 0;
            অক্ষর kernels_বাফার[4096] = {0};
            cl_int ফলাফল = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, আকার(kernels_বাফার), &kernels_বাফার[0], &retsz);
            CV_OCL_DBG_CHECK_ফলাফল(ফলাফল, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            যদি (ফলাফল == CL_SUCCESS && retsz < আকার(kernels_বাফার))
            {
                kernels_বাফার[retsz] = 0;
                CV_LOG_INFO(ফাঁকা, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_বাফার << "'");
            }
            অন্যথা
            {
                CV_LOG_ERROR(ফাঁকা, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    যৌক্তিক compile(ধ্রুবক Context& ctx, ধ্রুবক ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(ফাঁকা, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            যৌক্তিক isLoaded = create_From_দ্বিমিক(ctx, src_->sourceAddr_, src_->source_আকার, errmsg);
            ফেরত isLoaded;
        }
        ফেরত compileWithCache(ctx, src_, errmsg);
    }

    যৌক্তিক compileWithCache(ধ্রুবক Context& ctx, ধ্রুবক ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ধ্রুবক std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ধ্রুবক String& hash_str = src_->sourceHash_;
        cv::String fname;
        যদি (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ধ্রুবক cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        যদি (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            চেষ্টা
            {
                std::vector<অক্ষর> দ্বিমিক_Buf;
                যৌক্তিক res = মিথ্যা;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    দ্বিমিক_Program_File file(fname, hash_str.c_str());
                    res = file.পঠ(buildflags, দ্বিমিক_Buf);
                }
                যদি (res)
                {
                    CV_Assert(!দ্বিমিক_Buf.empty());
                    CV_LOG_VERBOSE(ফাঁকা, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    যৌক্তিক isLoaded = create_From_দ্বিমিক(ctx, দ্বিমিক_Buf, errmsg);
                    যদি (isLoaded)
                        ফেরত সত্য;
                }
            }
            ধরো (ধ্রুবক cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(ফাঁকা, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            ধরো (...)
            {
                CV_LOG_VERBOSE(ফাঁকা, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ফাঁকা);
        যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            যদি (!buildFromSources(ctx, src_, errmsg))
            {
                ফেরত মিথ্যা;
            }
        }
        অন্যথা যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            যদি ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ফাঁকা, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            যৌক্তিক isLoaded = create_From_দ্বিমিক(ctx, src_->sourceAddr_, src_->source_আকার, errmsg);
            যদি (!isLoaded)
                ফেরত মিথ্যা;
        }
        অন্যথা যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        অন্যথা
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != ফাঁকা);
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
        যদি (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            চেষ্টা
            {
                std::vector<অক্ষর> দ্বিমিক_Buf;
                get_Program_দ্বিমিক(দ্বিমিক_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    দ্বিমিক_Program_File file(fname, hash_str.c_str());
                    file.লিখ(buildflags, দ্বিমিক_Buf);
                }
            }
            ধরো (ধ্রুবক cv::Exception& e)
            {
                CV_LOG_WARNING(ফাঁকা, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            ধরো (...)
            {
                CV_LOG_WARNING(ফাঁকা, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
        যদি (CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান)
        {
            std::vector<অক্ষর> দ্বিমিক_Buf;
            get_Program_দ্বিমিক(দ্বিমিক_Buf);
            যদি (!দ্বিমিক_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ফাঁকা;
                create_From_দ্বিমিক(ctx, দ্বিমিক_Buf, errmsg);
            }
        }
#endif
        ফেরত handle != ফাঁকা;
    }

    শূন্য dumpBuildLog_(cl_int ফলাফল, ধ্রুবক cl_device_id* deviceList, String& errmsg)
    {
        Auto_বাফার<অক্ষর, 4096> বাফার; বাফার[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        যদি (log_retval == CL_SUCCESS && retsz > 1)
        {
            বাফার.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, বাফার.তথ্য(), &retsz);
            যদি (log_retval == CL_SUCCESS)
            {
                যদি (retsz < বাফার.size())
                    বাফার[retsz] = 0;
                অন্যথা
                    বাফার[বাফার.size() - 1] = 0;
            }
            অন্যথা
            {
                বাফার[0] = 0;
            }
        }

        errmsg = String(বাফার.তথ্য());
        printf("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                sourceModule_.c_str(), sourceName_.c_str(),
                ফলাফল, getOpenCLErrorString(ফলাফল),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    যৌক্তিক buildFromSources(ধ্রুবক Context& ctx, ধ্রুবক ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == ফাঁকা);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ফাঁকা, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ধ্রুবক অক্ষর* srcptr = src_->sourceAddr_ ? ((ধ্রুবক অক্ষর*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->source_আকার : src_->codeStr_.size();
        CV_Assert(srcptr != ফাঁকা);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_ফলাফল(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        যদি (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_বাফার<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.তথ্য();
            জন্য (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_ফলাফল(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#যদি !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            যদি (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                যদি (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ফাঁকা;
                }
                যদি (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
            যদি (handle && CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান)
            {
                CV_LOG_INFO(ফাঁকা, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                অক্ষর kernels_বাফার[4096] = {0};
                cl_int ফলাফল = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, আকার(kernels_বাফার), &kernels_বাফার[0], &retsz);
                যদি (retsz < আকার(kernels_বাফার))
                    kernels_বাফার[retsz] = 0;
                অন্যথা
                    kernels_বাফার[0] = 0;
                CV_LOG_INFO(ফাঁকা, ফলাফল << ": Kernels='" << kernels_বাফার << "'");
            }
#endif
        }
        ফেরত handle != ফাঁকা;
    }

    শূন্য get_Program_দ্বিমিক(std::vector<অক্ষর>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_দ্বিমিক_SIZES, আকার(sz), &sz, ফাঁকা));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, আকার(ptr), &ptr, ফাঁকা));
    }

    যৌক্তিক create_From_দ্বিমিক(ধ্রুবক Context& ctx, ধ্রুবক std::vector<অক্ষর>& buf, String& errmsg)
    {
        ফেরত create_From_দ্বিমিক(ctx, (ধ্রুবক unsigned অক্ষর*)&buf[0], buf.size(), errmsg);
    }

    যৌক্তিক create_From_দ্বিমিক(ধ্রুবক Context& ctx, ধ্রুবক unsigned অক্ষর* দ্বিমিক_Addr, ধ্রুবক size_t দ্বিমিক_আকার, String& errmsg)
    {
        CV_Assert(handle == ফাঁকা);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ফাঁকা, 0, "Load from binary... (" << দ্বিমিক_আকার << " bytes)");

        CV_Assert(দ্বিমিক_আকার > 0);

        size_t ndevices = (পূর্ণসংখ্যা)ctx.ndevices();
        Auto_বাফার<cl_device_id> devices_(ndevices);
        Auto_বাফার<ধ্রুবক uchar*> দ্বিমিক_Ptrs(ndevices);
        Auto_বাফার<size_t> দ্বিমিক_Sizes(ndevices);

        cl_device_id* devices = devices_.তথ্য();
        ধ্রুবক uchar** দ্বিমিক_Ptrs_2 = দ্বিমিক_Ptrs.তথ্য();
        size_t* দ্বিমিক_Sizes_2 = দ্বিমিক_Sizes.তথ্য();
        জন্য (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            দ্বিমিক_Ptrs_2[i] = দ্বিমিক_Addr;
            দ্বিমিক_Sizes_2[i] = দ্বিমিক_আকার;
        }

        cl_int ফলাফল = 0;
        handle = cl_Create_Program_With_দ্বিমিক((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.তথ্য(),
                                           দ্বিমিক_Sizes_2, দ্বিমিক_Ptrs_2, ফাঁকা, &ফলাফল);
        যদি (ফলাফল != CL_SUCCESS)
        {
            CV_LOG_ERROR(ফাঁকা, CV_OCL_API_ERROR_বার্তা(ফলাফল, "clCreateProgramWithBinary"));
            যদি (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = ফাঁকা;
            }
        }
        যদি (!handle)
        {
            ফেরত মিথ্যা;
        }
        // call clBuildProgram()
        {
            ফলাফল = clBuildProgram(handle, (cl_uint)ndevices, devices_.তথ্য(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_ফলাফল(ফলাফল, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            যদি (ফলাফল != CL_SUCCESS)
            {
                dumpBuildLog_(ফলাফল, devices, errmsg);
                যদি (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ফাঁকা;
                }
                ফেরত মিথ্যা;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(ফলাফল = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    আকার(build_status), &build_status, &retsz));
            যদি (ফলাফল == CL_SUCCESS)
            {
                যদি (build_status == CL_BUILD_SUCCESS)
                {
                    ফেরত সত্য;
                }
                অন্যথা
                {
                    CV_LOG_WARNING(ফাঁকা, "clGetProgramBuildInfo() returns " << build_status);
                    ফেরত মিথ্যা;
                }
            }
            অন্যথা
            {
                CV_LOG_ERROR(ফাঁকা, CV_OCL_API_ERROR_বার্তা(ফলাফল, "clGetProgramBuildInfo()"));
                যদি (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = ফাঁকা;
                }
            }
        }
#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
        যদি (handle && CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান)
        {
            CV_LOG_INFO(ফাঁকা, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            অক্ষর kernels_বাফার[4096] = {0};
            ফলাফল = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, আকার(kernels_বাফার), &kernels_বাফার[0], &retsz);
            যদি (retsz < আকার(kernels_বাফার))
                kernels_বাফার[retsz] = 0;
            অন্যথা
                kernels_বাফার[0] = 0;
            CV_LOG_INFO(ফাঁকা, ফলাফল << ": Kernels='" << kernels_বাফার << "'");
        }
#endif
        ফেরত handle != ফাঁকা;
    }

    ~Impl()
    {
        যদি( handle )
        {
#ifdef _WIN32
            যদি (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = ফাঁকা;
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

Program::Program(ধ্রুবক ProgramSource& src,
        ধ্রুবক String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ধ্রুবক Program& prog)
{
    p = prog.p;
    যদি(p)
        p->addref();
}

Program& Program::operator = (ধ্রুবক Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ফেরত *এই;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    যদি (এই != &prog) {
        যদি(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ফেরত *এই;
}

Program::~Program()
{
    যদি(p)
        p->release();
}

যৌক্তিক Program::create(ধ্রুবক ProgramSource& src,
            ধ্রুবক String& buildflags, String& errmsg)
{
    যদি(p)
    {
        p->release();
        p = ফাঁকা;
    }
    p = নতুন Impl(src, buildflags, errmsg);
    যদি(!p->handle)
    {
        p->release();
        p = 0;
    }
    ফেরত p != 0;
}

শূন্য* Program::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ধ্রুবক ProgramSource& Program::source() ধ্রুবক
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

যৌক্তিক Program::পঠ(ধ্রুবক String& bin, ধ্রুবক String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

যৌক্তিক Program::লিখ(String& bin) ধ্রুবক
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ধ্রুবক
{
    যদি(!p)
        ফেরত String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ফেরত cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ধ্রুবক String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ফেরত cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

শূন্য Program::get_দ্বিমিক(std::vector<অক্ষর>& binary) ধ্রুবক
{
    CV_Assert(p && "Empty program");
    p->get_Program_দ্বিমিক(binary);
}

Program Context::Impl::getProg(ধ্রুবক ProgramSource& src,
                               ধ্রুবক String& buildflags, String& errmsg)
{
    size_t limit = get_Program_গুনতি_Limit();
    ধ্রুবক ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String চাবি = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(চাবি);
        যদি (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), চাবি);
            যদি (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(চাবি);
            }
            ফেরত it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            যদি (limit > 0 && sz >= limit)
            {
                স্থির যৌক্তিক warningFlag = মিথ্যা;
                যদি (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = সত্য;
                }
                যখন (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    যদি (c != 0)
                        বিরতি;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(চাবি, prog));
        cacheList.push_front(চাবি);
    }
    ফেরত prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
শ্রেণী OpenCLBufferPool
{
সুরক্ষিত:
    ~OpenCLBufferPool() { }
সর্বজনীন:
    আভাসী T allocate(size_t size) = 0;
    আভাসী শূন্য release(T বাফার) = 0;
};

template <typename Derived, typename বাফার_Entry, typename T>
শ্রেণী OpenCLBufferPoolBaseImpl : সর্বজনীন বাফার_Pool_Controller, সর্বজনীন OpenCLBufferPool<T>
{
ব্যক্তিগত:
    অন্তঃরেখ Derived& derived() { ফেরত *static_cast<Derived*>(এই); }
সুরক্ষিত:
    Mutex mutex_;

    size_t current_Reserved_আকার;
    size_t max_Reserved_আকার;

    std::list<বাফার_Entry> allocatedEntries_; // Allocated and used entries
    std::list<বাফার_Entry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    যৌক্তিক _findAndRemoveEntryFromAllocatedList(CV_OUT বাফার_Entry& entry, T বাফার)
    {
        typename std::list<বাফার_Entry>::iterator i = allocatedEntries_.begin();
        জন্য (; i != allocatedEntries_.end(); ++i)
        {
            বাফার_Entry& e = *i;
            যদি (e.cl_বাফার_2 == বাফার)
            {
                entry = e;
                allocatedEntries_.erase(i);
                ফেরত সত্য;
            }
        }
        ফেরত মিথ্যা;
    }

    // synchronized
    যৌক্তিক _findAndRemoveEntryFromReservedList(CV_OUT বাফার_Entry& entry, ধ্রুবক size_t size)
    {
        যদি (reservedEntries_.empty())
            ফেরত মিথ্যা;
        typename std::list<বাফার_Entry>::iterator i = reservedEntries_.begin();
        typename std::list<বাফার_Entry>::iterator ফলাফল_pos = reservedEntries_.end();
        বাফার_Entry ফলাফল;
        size_t minDiff = (size_t)(-1);
        জন্য (; i != reservedEntries_.end(); ++i)
        {
            বাফার_Entry& e = *i;
            যদি (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                যদি (diff < std::max((size_t)4096, size / 8) && (ফলাফল_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    ফলাফল_pos = i;
                    ফলাফল = e;
                    যদি (diff == 0)
                        বিরতি;
                }
            }
        }
        যদি (ফলাফল_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(ফলাফল_pos);
            entry = ফলাফল;
            current_Reserved_আকার -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            ফেরত সত্য;
        }
        ফেরত মিথ্যা;
    }

    // synchronized
    শূন্য check_আকার_Of_Reserved_Entries()
    {
        যখন (current_Reserved_আকার > max_Reserved_আকার)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ধ্রুবক বাফার_Entry& entry = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_আকার >= entry.capacity_);
            current_Reserved_আকার -= entry.capacity_;
            derived().release_বাফার_Entry(entry);
            reservedEntries_.pop_back();
        }
    }

    অন্তঃরেখ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        যদি (size < 1024*1024)
            ফেরত 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        অন্যথা যদি (size < 16*1024*1024)
            ফেরত 64*1024;
        অন্যথা
            ফেরত 1024*1024;
    }

সর্বজনীন:
    OpenCLBufferPoolBaseImpl()
        : current_Reserved_আকার(0),
          max_Reserved_আকার(0)
    {
        // nothing
    }
    আভাসী ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
সর্বজনীন:
    আভাসী T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        বাফার_Entry entry;
        যদি (max_Reserved_আকার > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_বাফার_POOL("Reuse reserved buffer: %p\n", entry.cl_বাফার_2);
        }
        অন্যথা
        {
            derived().allocate_বাফার_Entry(entry, size);
        }
        ফেরত entry.cl_বাফার_2;
    }
    আভাসী শূন্য release(T বাফার) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        বাফার_Entry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, বাফার));
        যদি (max_Reserved_আকার == 0 || entry.capacity_ > max_Reserved_আকার / 8)
        {
            derived().release_বাফার_Entry(entry);
        }
        অন্যথা
        {
            reservedEntries_.push_front(entry);
            current_Reserved_আকার += entry.capacity_;
            check_আকার_Of_Reserved_Entries();
        }
    }

    আভাসী size_t get_Reserved_আকার() ধ্রুবক CV_OVERRIDE { ফেরত current_Reserved_আকার; }
    আভাসী size_t get_Max_Reserved_আকার() ধ্রুবক CV_OVERRIDE { ফেরত max_Reserved_আকার; }
    আভাসী শূন্য set_Max_Reserved_আকার(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t old_Max_Reserved_আকার = max_Reserved_আকার;
        max_Reserved_আকার = size;
        যদি (max_Reserved_আকার < old_Max_Reserved_আকার)
        {
            typename std::list<বাফার_Entry>::iterator i = reservedEntries_.begin();
            জন্য (; i != reservedEntries_.end();)
            {
                ধ্রুবক বাফার_Entry& entry = *i;
                যদি (entry.capacity_ > max_Reserved_আকার / 8)
                {
                    CV_DbgAssert(current_Reserved_আকার >= entry.capacity_);
                    current_Reserved_আকার -= entry.capacity_;
                    derived().release_বাফার_Entry(entry);
                    i = reservedEntries_.erase(i);
                    চালু;
                }
                ++i;
            }
            check_আকার_Of_Reserved_Entries();
        }
    }
    আভাসী শূন্য freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<বাফার_Entry>::const_iterator i = reservedEntries_.begin();
        জন্য (; i != reservedEntries_.end(); ++i)
        {
            ধ্রুবক বাফার_Entry& entry = *i;
            derived().release_বাফার_Entry(entry);
        }
        reservedEntries_.clear();
        current_Reserved_আকার = 0;
    }
};

কাঠামো CLBufferEntry
{
    cl_mem cl_বাফার_2;
    size_t capacity_;
    CLBufferEntry() : cl_বাফার_2((cl_mem)ফাঁকা), capacity_(0) { }
};

শ্রেণী OpenCLBufferPoolImpl CV_FINAL : সর্বজনীন OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
সর্বজনীন:
    প্রকারলেখ কাঠামো CLBufferEntry বাফার_Entry;
সুরক্ষিত:
    পূর্ণসংখ্যা createFlags_;
সর্বজনীন:
    OpenCLBufferPoolImpl(পূর্ণসংখ্যা createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    শূন্য allocate_বাফার_Entry(বাফার_Entry& entry, size_t size)
    {
        CV_DbgAssert(entry.cl_বাফার_2 == ফাঁকা);
        entry.capacity_ = align_আকার(size, (পূর্ণসংখ্যা)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.cl_বাফার_2 = cl_Create_বাফার((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_ফলাফল(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long পূর্ণসংখ্যা)entry.capacity_, (শূন্য*)entry.cl_বাফার_2).c_str());
        CV_Assert(entry.cl_বাফার_2 != ফাঁকা);
        যদি(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_বাফার_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.cl_বাফার_2);
        allocatedEntries_.push_back(entry);
    }

    শূন্য release_বাফার_Entry(ধ্রুবক বাফার_Entry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.cl_বাফার_2 != ফাঁকা);
        LOG_বাফার_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.cl_বাফার_2, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.cl_বাফার_2));
    }
};

#ifdef HAVE_OPENCL_SVM
কাঠামো CLSVMBufferEntry
{
    শূন্য* cl_বাফার_2;
    size_t capacity_;
    CLSVMBufferEntry() : cl_বাফার_2(ফাঁকা), capacity_(0) { }
};
শ্রেণী OpenCLSVMBufferPoolImpl CV_FINAL : সর্বজনীন OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, শূন্য*>
{
সর্বজনীন:
    প্রকারলেখ কাঠামো CLSVMBufferEntry বাফার_Entry;
সর্বজনীন:
    OpenCLSVMBufferPoolImpl()
    {
    }

    শূন্য allocate_বাফার_Entry(বাফার_Entry& entry, size_t size)
    {
        CV_DbgAssert(entry.cl_বাফার_2 == ফাঁকা);
        entry.capacity_ = align_আকার(size, (পূর্ণসংখ্যা)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ধ্রুবক svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        যৌক্তিক is_Fine_Grain_বাফার = svmCaps.is_Support_Fine_Grain_বাফার();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_বাফার ? CL_MEM_SVM_FINE_GRAIN_বাফার : 0);

        ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (পূর্ণসংখ্যা)entry.capacity_);
        শূন্য *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.cl_বাফার_2 = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_বাফার_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.cl_বাফার_2);
        allocatedEntries_.push_back(entry);
    }

    শূন্য release_বাফার_Entry(ধ্রুবক বাফার_Entry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.cl_বাফার_2 != ফাঁকা);
        LOG_বাফার_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.cl_বাফার_2, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.cl_বাফার_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.cl_বাফার_2);
    }
};
#endif



template <যৌক্তিক readAccess, যৌক্তিক writeAccess>
শ্রেণী Aligned_তথ্য_Ptr
{
সুরক্ষিত:
    ধ্রুবক size_t আকার_4;
    uchar* ধ্রুবক originPtr_;
    ধ্রুবক size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

সর্বজনীন:
    Aligned_তথ্য_Ptr(uchar* ptr, size_t size, size_t alignment)
        : আকার_4(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ফাঁকা)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        যদি (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = নতুন uchar[আকার_4 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            যদি (readAccess)
            {
                memcpy(ptr_, originPtr_, আকার_4);
            }
        }
    }

    uchar* getAlignedPtr() ধ্রুবক
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ফেরত ptr_;
    }

    ~Aligned_তথ্য_Ptr()
    {
        যদি (allocatedPtr_)
        {
            যদি (writeAccess)
            {
                memcpy(originPtr_, ptr_, আকার_4);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ফাঁকা;
        }
        ptr_ = ফাঁকা;
    }
ব্যক্তিগত:
    Aligned_তথ্য_Ptr(ধ্রুবক Aligned_তথ্য_Ptr&); // disabled
    Aligned_তথ্য_Ptr& operator=(ধ্রুবক Aligned_তথ্য_Ptr&); // disabled
};

template <যৌক্তিক readAccess, যৌক্তিক writeAccess>
শ্রেণী Aligned_তথ্য_Ptr2_D
{
সুরক্ষিত:
    ধ্রুবক size_t আকার_4;
    uchar* ধ্রুবক originPtr_;
    ধ্রুবক size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

সর্বজনীন:
    Aligned_তথ্য_Ptr2_D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : আকার_4(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ফাঁকা), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ফাঁকা);
        যদি (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = নতুন uchar[আকার_4 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            যদি (readAccess)
            {
                জন্য (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ধ্রুবক
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ফেরত ptr_;
    }

    ~Aligned_তথ্য_Ptr2_D()
    {
        যদি (allocatedPtr_)
        {
            যদি (writeAccess)
            {
                জন্য (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = ফাঁকা;
        }
        ptr_ = ফাঁকা;
    }
ব্যক্তিগত:
    Aligned_তথ্য_Ptr2_D(ধ্রুবক Aligned_তথ্য_Ptr2_D&); // disabled
    Aligned_তথ্য_Ptr2_D& operator=(ধ্রুবক Aligned_তথ্য_Ptr2_D&); // disabled
};

#ifndef CV_OPENCL_তথ্য_PTR_ALIGNMENT
#define CV_OPENCL_তথ্য_PTR_ALIGNMENT 16
#endif


শূন্য Context::Impl::__init_buffer_pools()
{
    বাফার_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& বাফার_Pool_3 = *বাফার_Pool_2.get();
    বাফার_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& বাফার_Pool_Host_Ptr_2 = *বাফার_Pool_Host_Ptr.get();

    size_t default_Pool_আকার = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_আকার = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_আকার);
    বাফার_Pool_3.set_Max_Reserved_আকার(pool_আকার);
    size_t pool_আকার_Host_Ptr = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_আকার);
    বাফার_Pool_Host_Ptr_2.set_Max_Reserved_আকার(pool_আকার_Host_Ptr);

#ifdef HAVE_OPENCL_SVM
    বাফার_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& বাফার_Pool_SVM_2 = *বাফার_Pool_SVM.get();
    size_t pool_আকার_SVM = utils::get_Configuration_Parameter_আকার_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_আকার);
    বাফার_Pool_SVM_2.set_Max_Reserved_আকার(pool_আকার_SVM);
#endif

    CV_LOG_INFO(ফাঁকা, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_আকার << " poolSizeHostPtr=" << pool_আকার_Host_Ptr);
}

শ্রেণী OpenCLAllocator CV_FINAL : সর্বজনীন MatAllocator
{
সর্বজনীন:
    গণনা AllocatorFlags
    {
        ALLOCATOR_FLAGS_বাফার_POOL_USED = 1 << 0,
        ALLOCATOR_FLAGS_বাফার_POOL_HOST_PTR_USED = 1 << 1,
#ifdef HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_বাফার_POOL_SVM_USED = 1 << 2,
#endif
        ALLOCATOR_FLAGS_EXTERNAL_বাফার = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flushCleanupQueue();
    }

    UMat_তথ্য* defaultAllocate(পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type, শূন্য* তথ্য, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ধ্রুবক
    {
        UMat_তথ্য* u = matStdAllocator->allocate(dims, sizes, type, তথ্য, step, flags, usageFlags);
        ফেরত u;
    }

    স্থির যৌক্তিক isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        স্থির যৌক্তিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", মিথ্যা);
        ফেরত value;
    }
    স্থির যৌক্তিক isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        স্থির যৌক্তিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", মিথ্যা);
        ফেরত value;
    }

    শূন্য getBestFlags(ধ্রুবক Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, পূর্ণসংখ্যা& createFlags, UMat_তথ্য::MemoryFlag& flags0) ধ্রুবক
    {
        ধ্রুবক Device& dev = ctx.device(0);
        createFlags = 0;
        যদি ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        যদি (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMat_তথ্য::MemoryFlag>(0);
        অন্যথা
            flags0 = UMat_তথ্য::COPY_ON_MAP;
    }

    UMat_তথ্য* allocate(পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* sizes, পূর্ণসংখ্যা type,
                       শূন্য* তথ্য, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ধ্রুবক CV_OVERRIDE
    {
        যদি(!useOpenCL())
            ফেরত defaultAllocate(dims, sizes, type, তথ্য, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(তথ্য == 0);
        size_t যোগ = CV_ELEM_আকার(type);
        জন্য( পূর্ণসংখ্যা i = dims-1; i >= 0; i-- )
        {
            যদি( step )
                step[i] = যোগ;
            যোগ *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        যদি (!ctx.getImpl())
            ফেরত defaultAllocate(dims, sizes, type, তথ্য, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        পূর্ণসংখ্যা createFlags = 0;
        UMat_তথ্য::MemoryFlag flags0 = static_cast<UMat_তথ্য::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        শূন্য* handle = ফাঁকা;
        পূর্ণসংখ্যা allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ধ্রুবক svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        যদি (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_বাফার_POOL_SVM_USED;
            handle = ctxImpl.get_বাফার_Pool_SVM().allocate(যোগ);

            // this property is constant, so single buffer pool can be used here
            যৌক্তিক is_Fine_Grain_বাফার = svmCaps.is_Support_Fine_Grain_বাফার();
            allocatorFlags |= is_Fine_Grain_বাফার ? svm::OPENCL_SVM_FINE_GRAIN_বাফার : svm::OPENCL_SVM_COARSE_GRAIN_বাফার;
        }
        অন্যথা
#endif
        যদি (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_বাফার_POOL_USED;
            handle = ctxImpl.get_বাফার_Pool().allocate(যোগ);
        }
        অন্যথা যদি (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_বাফার_POOL_HOST_PTR_USED;
            handle = ctxImpl.get_বাফার_Pool_Host_Ptr().allocate(যোগ);
        }
        অন্যথা
        {
            CV_Assert(handle != ফাঁকা); // Unsupported, throw
        }

        যদি (!handle)
            ফেরত defaultAllocate(dims, sizes, type, তথ্য, step, flags, usageFlags);

        UMat_তথ্য* u = নতুন UMat_তথ্য(এই);
        u->তথ্য = 0;
        u->size = যোগ;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<শূন্য>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->অস্থায়ী_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(সত্য);
        opencl_allocator_stats.onAllocate(u->size);
        ফেরত u;
    }

    যৌক্তিক allocate(UMat_তথ্য* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত মিথ্যা;

        flushCleanupQueue();

        UMat_তথ্য_Auto_Lock lock(u);

        যদি(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            পূর্ণসংখ্যা createFlags = 0;
            UMat_তথ্য::MemoryFlag flags0 = static_cast<UMat_তথ্য::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            যৌক্তিক copyOnMap = (flags0 & UMat_তথ্য::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            পূর্ণসংখ্যা allocatorFlags = 0;
            UMat_তথ্য::MemoryFlag অস্থায়ী_UMat_Flags = static_cast<UMat_তথ্য::MemoryFlag>(0);
            শূন্য* handle = ফাঁকা;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            যৌক্তিক useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            যদি (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                অস্থায়ী_UMat_Flags = UMat_তথ্য::অস্থায়ী_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (পূর্ণসংখ্যা)u->size, handle);
            }
            অন্যথা যদি (useSVM && (svmCaps.is_Support_Fine_Grain_বাফার() || svmCaps.is_Support_Coarse_Grain_বাফার()))
            {
                যদি (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    যৌক্তিক is_Fine_Grain_বাফার = svmCaps.is_Support_Fine_Grain_বাফার();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_বাফার ? CL_MEM_SVM_FINE_GRAIN_বাফার : 0);

                    ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (পূর্ণসংখ্যা)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = ফাঁকা;
                    যদি (!is_Fine_Grain_বাফার)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (পূর্ণসংখ্যা)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ফাঁকা, ফাঁকা);
                        CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    যদি (!is_Fine_Grain_বাফার)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, ফাঁকা, ফাঁকা);
                        CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                    }

                    অস্থায়ী_UMat_Flags = UMat_তথ্য::অস্থায়ী_UMAT | UMat_তথ্য::অস্থায়ী_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_বাফার ? svm::OPENCL_SVM_FINE_GRAIN_বাফার
                                                : svm::OPENCL_SVM_COARSE_GRAIN_বাফার;
                }
            }
            অন্যথা
#endif
            {
                যদি( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                অস্থায়ী_UMat_Flags = UMat_তথ্য::অস্থায়ী_UMAT;
                যদি (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (পূর্ণসংখ্যা)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_তথ্য && u->original_UMat_তথ্য->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_বাফার(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long পূর্ণসংখ্যা)u->size, u->origdata, (শূন্য*)handle).c_str());
                }
                যদি((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_বাফার(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long পূর্ণসংখ্যা)u->size, u->origdata, (শূন্য*)handle).c_str());
                    অস্থায়ী_UMat_Flags |= UMat_তথ্য::অস্থায়ী_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clCreateBuffer() => %p", (শূন্য*)handle).c_str());
            যদি(!handle || retval != CL_SUCCESS)
                ফেরত মিথ্যা;
            u->handle = handle;
            u->পূর্ব_Allocator = u->currAllocator;
            u->currAllocator = এই;
            u->flags |= অস্থায়ী_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(সত্য);
        opencl_allocator_stats.onAllocate(u->size);
        ফেরত সত্য;
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

    শূন্য deallocate(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        যদি (!!(u->flags & UMat_তথ্য::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        অন্যথা
            deallocate_(u);
    }

    শূন্য deallocate_(UMat_তথ্য* u) ধ্রুবক
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        যদি ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_বাফার) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        যদি (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ফেরত;             // avoid any OpenCL calls
#endif
        যদি(u->অস্থায়ী_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            যদি (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    যদি( u->অস্থায়ী_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার);
                        যৌক্তিক is_Fine_Grain_বাফার = (u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার;
                        cl_command_queue q = ফাঁকা;
                        যদি (!is_Fine_Grain_বাফার)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণসংখ্যা)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ফাঁকা, ফাঁকা);
                            CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        যদি (!is_Fine_Grain_বাফার)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ফাঁকা, ফাঁকা);
                            CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    অন্যথা
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                অন্যথা
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    যদি( u->অস্থায়ী_Copied_UMat() )
                    {
                        Aligned_তথ্য_Ptr<মিথ্যা, সত্য> alignedPtr(u->origdata, u->size, CV_OPENCL_তথ্য_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    অন্যথা
                    {
                        cl_int retval = 0;
                        যদি (u->অস্থায়ী_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            শূন্য* তথ্য = cl_Enqueue_Map_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (শূন্য*)u->handle, (long long পূর্ণসংখ্যা)u->size, তথ্য).c_str());
                            CV_Assert(u->origdata == তথ্য && "Details: https://github.com/opencv/opencv/issues/6293");
                            যদি (u->original_UMat_তথ্য)
                            {
                                CV_Assert(u->original_UMat_তথ্য->তথ্য == তথ্য);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, তথ্য, 0, 0, 0);
                            CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (শূন্য*)u->handle, তথ্য, (long long পূর্ণসংখ্যা)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(মিথ্যা);
            }
            অন্যথা
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
            {
                যদি( u->অস্থায়ী_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            অন্যথা
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clReleaseMemObject(ptr=%p)", (শূন্য*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(সত্য);
            u->currAllocator = u->পূর্ব_Allocator;
            u->পূর্ব_Allocator = ফাঁকা;
            যদি(u->তথ্য && u->copyOnMap() && u->তথ্য != u->origdata)
                fastFree(u->তথ্য);
            u->তথ্য = u->origdata;
            u->currAllocator->deallocate(u);
            u = ফাঁকা;
        }
        অন্যথা
        {
            CV_Assert(u->origdata == ফাঁকা);
            যদি(u->তথ্য && u->copyOnMap() && u->তথ্য != u->origdata)
            {
                fastFree(u->তথ্য);
                u->তথ্য = 0;
                u->markHostCopyObsolete(সত্য);
            }
            যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_বাফার_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_বাফার_Pool().release((cl_mem)u->handle);
            }
            অন্যথা যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_বাফার_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_বাফার_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            অন্যথা যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_বাফার_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                অন্যথা যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
                {
                    ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, ফাঁকা, ফাঁকা);
                        CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_বাফার_Pool_SVM().release((শূন্য*)u->handle);
            }
#endif
            অন্যথা
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(সত্য);
            delete u;
            u = ফাঁকা;
        }
        CV_Assert(u == ফাঁকা);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    শূন্য map(UMat_তথ্য* u, AccessFlag accessFlags) ধ্রুবক CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(সত্য);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            যদি( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
                {
                    যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
                    {
                        Context& ctx = Context::getDefault();
                        ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণসংখ্যা)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ফাঁকা, ফাঁকা);
                            CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_বাফার_MAP;
                        }
                    }
                    clFinish(q);
                    u->তথ্য = (uchar*)u->handle;
                    u->markHostCopyObsolete(মিথ্যা);
                    u->markDeviceMemMapped(সত্য);
                    ফেরত;
                }
#endif

                cl_int retval = CL_SUCCESS;
                যদি (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->তথ্য = (uchar*)cl_Enqueue_Map_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_ফলাফল(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (শূন্য*)u->handle, (long long পূর্ণসংখ্যা)u->size, u->তথ্য).c_str());
                }
                যদি (u->তথ্য && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(মিথ্যা);
                    u->markDeviceMemMapped(সত্য);
                    ফেরত;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_তথ্য::COPY_ON_MAP;
            }

            যদি(!u->তথ্য)
            {
                u->তথ্য = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(সত্য);
            }
        }

        যদি (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_তথ্য_Ptr<মিথ্যা, সত্য> alignedPtr(u->তথ্য, u->size, CV_OPENCL_তথ্য_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == 0);
#endif
            cl_int retval = cl_Enqueue_Read_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (শূন্য*)u->handle, (long long পূর্ণসংখ্যা)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(মিথ্যা);
        }
    }

    শূন্য unmap(UMat_তথ্য* u) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত;


        CV_Assert(u->handle != 0);

        UMat_তথ্য_Auto_Lock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        যদি( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->তথ্য != ফাঁকা);
#ifdef HAVE_OPENCL_SVM
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
            {
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, ফাঁকা, ফাঁকা);
                        CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_বাফার_MAP;
                    }
                }
                যদি (u->refcount == 0)
                    u->তথ্য = 0;
                u->markDeviceCopyObsolete(মিথ্যা);
                u->markHostCopyObsolete(সত্য);
                ফেরত;
            }
#endif
            যদি (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->তথ্য, 0, 0, 0);
                CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (শূন্য*)u->handle, u->তথ্য, (long long পূর্ণসংখ্যা)u->size).c_str());
                যদি (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(মিথ্যা);
                u->তথ্য = 0;
                u->markDeviceCopyObsolete(মিথ্যা);
                u->markHostCopyObsolete(সত্য);
            }
        }
        অন্যথা যদি( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_তথ্য_Ptr<সত্য, মিথ্যা> alignedPtr(u->তথ্য, u->size, CV_OPENCL_তথ্য_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == 0);
#endif
            retval = cl_Enqueue_Write_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (শূন্য*)u->handle, (long long পূর্ণসংখ্যা)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(মিথ্যা);
            u->markHostCopyObsolete(সত্য);
        }
    }

    যৌক্তিক checkContinuous(পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                         ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
                         ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[],
                         size_t& যোগ, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ধ্রুবক
    {
        যৌক্তিক iscontinuous = সত্য;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        যোগ = sz[dims-1];
        জন্য( পূর্ণসংখ্যা i = dims-2; i >= 0; i-- )
        {
            যদি( i >= 0 && (যোগ != srcstep[i] || যোগ != dststep[i]) )
                iscontinuous = মিথ্যা;
            যোগ *= sz[i];
            যদি( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            যদি( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        যদি( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            যদি( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                যদি( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                যদি( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            অন্যথা
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                যদি( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                যদি( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        ফেরত iscontinuous;
    }

    শূন্য download(UMat_তথ্য* u, শূন্য* dstptr, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                  ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
                  ধ্রুবক size_t dststep[]) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত;
        UMat_তথ্য_Auto_Lock autolock(u);

        যদি( u->তথ্য && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ফেরত;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t যোগ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        যৌক্তিক iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            যোগ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
        {
            CV_DbgAssert(u->তথ্য == ফাঁকা || u->তথ্য == u->handle);
            Context& ctx = Context::getDefault();
            ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) == 0);
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণসংখ্যা)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ফাঁকা, ফাঁকা);
                CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            যদি( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, যোগ);
            }
            অন্যথা
            {
                // This code is from MatAllocator::download()
                পূর্ণসংখ্যা isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    যদি( sz[i] == 0 )
                    ফেরত;
                    যদি( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (পূর্ণসংখ্যা)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ধ্রুবক Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                জন্য( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ফাঁকা, ফাঁকা);
                CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                Aligned_তথ্য_Ptr<মিথ্যা, সত্য> alignedPtr((uchar*)dstptr, যোগ, CV_OPENCL_তথ্য_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, যোগ, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_বাফার_RECT_OPERATIONS)
            {
                ধ্রুবক size_t padding = CV_OPENCL_তথ্য_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_তথ্য_Ptr2_D<মিথ্যা, মিথ্যা> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_তথ্য_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                যোগ = align_আকার(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                যোগ = std::min(যোগ, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, যোগ, ptr, 0, 0, 0));
                জন্য( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            অন্যথা
            {
                Aligned_তথ্য_Ptr2_D<মিথ্যা, সত্য> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_তথ্য_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_Enqueue_Read_বাফার_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    শূন্য upload(UMat_তথ্য* u, ধ্রুবক শূন্য* srcptr, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
                ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[],
                ধ্রুবক size_t srcstep[]) ধ্রুবক CV_OVERRIDE
    {
        যদি(!u)
            ফেরত;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->অস্থায়ী_UMat());

        size_t যোগ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        যৌক্তিক iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            যোগ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_তথ্য_Auto_Lock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        যদি( u->তথ্য && (u->hostCopyObsolete() < u->deviceCopyObsolete() || যোগ == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(মিথ্যা);
            u->markDeviceCopyObsolete(সত্য);
            ফেরত;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
        {
            CV_DbgAssert(u->তথ্য == ফাঁকা || u->তথ্য == u->handle);
            Context& ctx = Context::getDefault();
            ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MAP) == 0);
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণসংখ্যা)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ফাঁকা, ফাঁকা);
                CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            যদি( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, যোগ);
            }
            অন্যথা
            {
                // This code is from MatAllocator::upload()
                পূর্ণসংখ্যা isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    যদি( sz[i] == 0 )
                    ফেরত;
                    যদি( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (পূর্ণসংখ্যা)sz[i];
                }

                Mat src(dims, isz, CV_8U, (শূন্য*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ধ্রুবক Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                জন্য( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_বাফার)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, ফাঁকা, ফাঁকা);
                CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                Aligned_তথ্য_Ptr<সত্য, মিথ্যা> alignedPtr((uchar*)srcptr, যোগ, CV_OPENCL_তথ্য_PTR_ALIGNMENT);
                cl_int retval = cl_Enqueue_Write_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, যোগ, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (শূন্য*)u->handle, (long long পূর্ণসংখ্যা)dstrawofs, (long long পূর্ণসংখ্যা)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_বাফার_RECT_OPERATIONS)
            {
                ধ্রুবক size_t padding = CV_OPENCL_তথ্য_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_তথ্য_Ptr2_D<মিথ্যা, মিথ্যা> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_তথ্য_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                যোগ = align_আকার(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                যোগ = std::min(যোগ, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, যোগ, ptr, 0, 0, 0));
                জন্য( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_Enqueue_Write_বাফার(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, যোগ, ptr, 0, 0, 0));
            }
            অন্যথা
            {
                Aligned_তথ্য_Ptr2_D<সত্য, মিথ্যা> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_তথ্য_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_Enqueue_Write_বাফার_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(সত্য);
#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        অন্যথা
#endif
        {
            u->markHostCopyObsolete(সত্য);
        }
        u->markDeviceCopyObsolete(মিথ্যা);
    }

    শূন্য copy(UMat_তথ্য* src, UMat_তথ্য* dst, পূর্ণসংখ্যা dims, ধ্রুবক size_t sz[],
              ধ্রুবক size_t srcofs[], ধ্রুবক size_t srcstep[],
              ধ্রুবক size_t dstofs[], ধ্রুবক size_t dststep[], যৌক্তিক _sync) ধ্রুবক CV_OVERRIDE
    {
        যদি(!src || !dst)
            ফেরত;

        size_t যোগ = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        যৌক্তিক iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            যোগ, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_তথ্য_Auto_Lock src_autolock(src, dst);

        যদি( !src->handle || (src->তথ্য && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->তথ্য + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ফেরত;
        }
        যদি( !dst->handle || (dst->তথ্য && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->তথ্য + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(মিথ্যা);
#ifdef HAVE_OPENCL_SVM
            যদি ((dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            অন্যথা
#endif
            {
                dst->markDeviceCopyObsolete(সত্য);
            }
            ফেরত;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
        {
            যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ধ্রুবক svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                যদি( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (পূর্ণসংখ্যা)যোগ);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            যোগ, 0, ফাঁকা, ফাঁকা);
                    CV_OCL_CHECK_ফলাফল(status, "clEnqueueSVMMemcpy()");
                }
                অন্যথা
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    পূর্ণসংখ্যা isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        যদি( sz[i] == 0 )
                        ফেরত;
                        যদি( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (পূর্ণসংখ্যা)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    জন্য( পূর্ণসংখ্যা i = 0; i < dims; i++ )
                    {
                        যদি( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ধ্রুবক Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    জন্য( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            অন্যথা
            {
                যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->তথ্য + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                অন্যথা
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->তথ্য + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                retval = cl_Enqueue_Copy_বাফার(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, যোগ, 0, 0, 0);
                CV_OCL_CHECK_ফলাফল(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (শূন্য*)src->handle, (শূন্য*)dst->handle, (long long পূর্ণসংখ্যা)srcrawofs, (long long পূর্ণসংখ্যা)dstrawofs, (long long পূর্ণসংখ্যা)যোগ).c_str());
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_বাফার_RECT_OPERATIONS)
            {
                ধ্রুবক size_t padding = CV_OPENCL_তথ্য_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_তথ্য_Ptr2_D<মিথ্যা, মিথ্যা> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_তথ্য_PTR_ALIGNMENT, padding*2);
                Aligned_তথ্য_Ptr2_D<মিথ্যা, মিথ্যা> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_তথ্য_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_যোগ = align_আকার(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_যোগ = std::min(src_যোগ, src->size - new_srcrawofs);
                size_t dst_যোগ = align_আকার(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_যোগ = std::min(dst_যোগ, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_যোগ, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_Enqueue_Read_বাফার(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_যোগ, dstptr, 0, 0, 0));

                জন্য( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_Enqueue_Write_বাফার(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_যোগ, dstptr, 0, 0, 0));
            }
            অন্যথা
            {
                CV_OCL_CHECK(retval = cl_Enqueue_Copy_বাফার_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        যদি (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        যদি ((dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_বাফার ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_বাফার_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        অন্যথা
#endif
        {
            dst->markHostCopyObsolete(সত্য);
        }
        dst->markDeviceCopyObsolete(মিথ্যা);

        যদি( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    বাফার_Pool_Controller* get_বাফার_Pool_Controller(ধ্রুবক অক্ষর* id) ধ্রুবক CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        যদি (ctx.empty())
            ফেরত ফাঁকা;
#ifdef HAVE_OPENCL_SVM
        যদি ((svm::checkForceSVMUmatUsage() && (id == ফাঁকা || strcmp(id, "OCL") == 0)) || (id != ফাঁকা && strcmp(id, "SVM") == 0))
        {
            ফেরত &ctx.getImpl()->get_বাফার_Pool_SVM();
        }
#endif
        যদি (id != ফাঁকা && strcmp(id, "HOST_ALLOC") == 0)
        {
            ফেরত &ctx.getImpl()->get_বাফার_Pool_Host_Ptr();
        }
        যদি (id != ফাঁকা && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ফেরত &ctx.getImpl()->get_বাফার_Pool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMat_তথ্য*> cleanupQueue;

    শূন্য flushCleanupQueue() ধ্রুবক
    {
        // TSAN false positive: see #27638
        যদি (!cleanupQueue.empty())
        {
            std::deque<UMat_তথ্য*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            জন্য (std::deque<UMat_তথ্য*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    শূন্য addToCleanupQueue(UMat_তথ্য* u) ধ্রুবক
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

স্থির OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    স্থির OpenCLAllocator* g_allocator = নতুন OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ফেরত g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


নামাকাশ cv {

// three funcs below are implemented in umatrix.cpp
শূন্য set_আকার( UMat& m, পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, ধ্রুবক size_t* _steps,
              যৌক্তিক autoSteps = মিথ্যা );
শূন্য finalizeHdr(UMat& m);

} // namespace cv


নামাকাশ cv { নামাকাশ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
শূন্য convert_From_বাফার(শূন্য* cl_mem_বাফার, size_t step, পূর্ণসংখ্যা rows, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা type, UMat& dst)
{
    পূর্ণসংখ্যা d = 2;
    পূর্ণসংখ্যা sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    set_আকার(dst, d, sizes, 0, সত্য);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_বাফার;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, আকার(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_বাফার == mem_type);

    size_t যোগ = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_আকার, আকার(size_t), &যোগ, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((পূর্ণসংখ্যা)step >= cols * CV_ELEM_আকার(type));
    CV_Assert(যোগ >= rows * step);

    // attach clBuffer to UMatData
    dst.u = নতুন UMat_তথ্য(getOpenCLAllocator());
    dst.u->তথ্য            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_বাফার;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_তথ্য::MemoryFlag>(0);
    dst.u->handle          = cl_mem_বাফার;
    dst.u->origdata        = 0;
    dst.u->পূর্ব_Allocator   = 0;
    dst.u->size            = যোগ;

    finalizeHdr(dst);
    dst.addref();

    ফেরত;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
শূন্য convertFromImage(শূন্য* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, আকার(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, আকার(cl_image_format), &fmt, 0));

    পূর্ণসংখ্যা depth = CV_8U;
    নির্বাচন (fmt.image_channel_তথ্য_type)
    {
    ক্ষেত্র CL_UNORM_INT8:
    ক্ষেত্র CL_UNSIGNED_INT8:
        depth = CV_8U;
        বিরতি;

    ক্ষেত্র CL_SNORM_INT8:
    ক্ষেত্র CL_SIGNED_INT8:
        depth = CV_8S;
        বিরতি;

    ক্ষেত্র CL_UNORM_INT16:
    ক্ষেত্র CL_UNSIGNED_INT16:
        depth = CV_16U;
        বিরতি;

    ক্ষেত্র CL_SNORM_INT16:
    ক্ষেত্র CL_SIGNED_INT16:
        depth = CV_16S;
        বিরতি;

    ক্ষেত্র CL_SIGNED_INT32:
        depth = CV_32S;
        বিরতি;

    ক্ষেত্র CL_FLOAT:
        depth = CV_32F;
        বিরতি;

    ক্ষেত্র CL_HALF_FLOAT:
        depth = CV_16F;
        বিরতি;

    পূর্বনির্ধারিত:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    পূর্ণসংখ্যা type = CV_8UC1;
    নির্বাচন (fmt.image_channel_order)
    {
    ক্ষেত্র CL_R:
    ক্ষেত্র CL_A:
    ক্ষেত্র CL_INTENSITY:
    ক্ষেত্র CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        বিরতি;

    ক্ষেত্র CL_RG:
    ক্ষেত্র CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        বিরতি;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    ক্ষেত্র CL_RGBA:
    ক্ষেত্র CL_BGRA:
    ক্ষেত্র CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        বিরতি;

    পূর্বনির্ধারিত:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        বিরতি;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, আকার(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, আকার(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, আকার(size_t), &h, 0));

    dst.create((পূর্ণসংখ্যা)h, (পূর্ণসংখ্যা)w, type);

    cl_mem cl_বাফার = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_Enqueue_Copy_Image_To_বাফার(q, clImage, cl_বাফার, src_origin, region, offset, 0, ফাঁকা, ফাঁকা));

    CV_OCL_CHECK(clFinish(q));

    ফেরত;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

স্থির শূন্য getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint অঙ্ক_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ফাঁকা, &অঙ্ক_Devices);
    যদি (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_ফলাফল(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &অঙ্ক_Devices).c_str());
    }

    যদি (অঙ্ক_Devices == 0)
    {
        devices.clear();
        ফেরত;
    }

    devices.resize((size_t)অঙ্ক_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, অঙ্ক_Devices, &devices[0], &অঙ্ক_Devices));
}

কাঠামো PlatformInfo::Impl
{
    Impl(শূন্য* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ধ্রুবক
    {
        অক্ষর buf[1024];
        size_t sz=0;
        ফেরত clGetPlatformInfo(handle, prop, আকার(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < আকার(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    পূর্ণসংখ্যা versionMajor_;
    পূর্ণসংখ্যা versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(শূন্য* platform_id)
{
    p = নতুন Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    যদি(p)
        p->release();
}

PlatformInfo::PlatformInfo(ধ্রুবক PlatformInfo& i)
{
    যদি (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ধ্রুবক PlatformInfo& i)
{
    যদি (i.p != p)
    {
        যদি (i.p)
            i.p->addref();
        যদি (p)
            p->release();
        p = i.p;
    }
    ফেরত *এই;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    যদি (এই != &i) {
        যদি(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ফেরত *এই;
}

পূর্ণসংখ্যা PlatformInfo::device_সংখ্যা() ধ্রুবক
{
    ফেরত p ? (পূর্ণসংখ্যা)p->devices.size() : 0;
}

শূন্য PlatformInfo::getDevice(Device& device, পূর্ণসংখ্যা d) ধ্রুবক
{
    CV_Assert(p && d < (পূর্ণসংখ্যা)p->devices.size() );
    যদি(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ধ্রুবক
{
    ফেরত p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ধ্রুবক
{
    ফেরত p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ধ্রুবক
{
    ফেরত p ? p->version_ : String();
}

পূর্ণসংখ্যা PlatformInfo::versionMajor() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->versionMajor_;
}

পূর্ণসংখ্যা PlatformInfo::versionMinor() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->versionMinor_;
}

স্থির শূন্য getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint অঙ্ক_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ফাঁকা, &অঙ্ক_Platforms));

    যদি (অঙ্ক_Platforms == 0)
    {
        platforms.clear();
        ফেরত;
    }

    platforms.resize((size_t)অঙ্ক_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], &অঙ্ক_Platforms));
}

শূন্য getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    জন্য (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((শূন্য*)&platforms[i]) );
}

ধ্রুবক অক্ষর* typeToStr(পূর্ণসংখ্যা type)
{
    স্থির ধ্রুবক অক্ষর* tab[CV_DEPTH_MAX*16]=
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
    পূর্ণসংখ্যা cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুবক অক্ষর* ফলাফল = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(ফলাফল);
    ফেরত ফলাফল;
}

ধ্রুবক অক্ষর* memopTypeToStr(পূর্ণসংখ্যা type)
{
    স্থির ধ্রুবক অক্ষর* tab[CV_DEPTH_MAX*16] =
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
    পূর্ণসংখ্যা cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুবক অক্ষর* ফলাফল = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(ফলাফল);
    ফেরত ফলাফল;
}

ধ্রুবক অক্ষর* vecopTypeToStr(পূর্ণসংখ্যা type)
{
    স্থির ধ্রুবক অক্ষর* tab[CV_DEPTH_MAX*16] =
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
    পূর্ণসংখ্যা cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুবক অক্ষর* ফলাফল = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(ফলাফল);
    ফেরত ফলাফল;
}

// Deprecated due to size of buf buffer being unknowable.
ধ্রুবক অক্ষর* convertTypeStr(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth, পূর্ণসংখ্যা cn, অক্ষর* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ফেরত convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ধ্রুবক অক্ষর* convertTypeStr(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth, পূর্ণসংখ্যা cn, অক্ষর* buf, size_t buf_আকার_2)
{
    যদি( sdepth == ddepth )
        ফেরত "noconvert";
    ধ্রুবক অক্ষর *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    যদি( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_আকার_2, "convert_%s", typestr);
    }
    অন্যথা যদি( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_আকার_2, "convert_%s_sat_rte", typestr);
    অন্যথা
        snprintf(buf, buf_আকার_2, "convert_%s_sat", typestr);

    ফেরত buf;
}

ধ্রুবক অক্ষর* getOpenCLErrorString(পূর্ণসংখ্যা errorCode)
{
#define CV_OCL_CODE(id) ক্ষেত্র id: ফেরত #id
#define CV_OCL_CODE_(id, name) ক্ষেত্র id: ফেরত #name
    নির্বাচন (errorCode)
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
    CV_OCL_CODE(CL_MISALIGNED_SUB_বাফার_OFFSET);
    CV_OCL_CODE(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
    CV_OCL_CODE(CL_COMPILE_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_LINKER_NOT_AVAILABLE);
    CV_OCL_CODE(CL_LINK_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_DEVICE_PARTITION_FAILED);
    CV_OCL_CODE(CL_KERNEL_ARG_INFO_NOT_AVAILABLE);
    CV_OCL_CODE(CL_INVALID_মান);
    CV_OCL_CODE(CL_INVALID_DEVICE_TYPE);
    CV_OCL_CODE(CL_INVALID_PLATFORM);
    CV_OCL_CODE(CL_INVALID_DEVICE);
    CV_OCL_CODE(CL_INVALID_CONTEXT);
    CV_OCL_CODE(CL_INVALID_QUEUE_PROPERTIES);
    CV_OCL_CODE(CL_INVALID_COMMAND_QUEUE);
    CV_OCL_CODE(CL_INVALID_HOST_PTR);
    CV_OCL_CODE(CL_INVALID_MEM_OBJECT);
    CV_OCL_CODE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_IMAGE_আকার);
    CV_OCL_CODE(CL_INVALID_SAMPLER);
    CV_OCL_CODE(CL_INVALID_দ্বিমিক);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_সূচক);
    CV_OCL_CODE(CL_INVALID_ARG_মান);
    CV_OCL_CODE(CL_INVALID_ARG_আকার);
    CV_OCL_CODE(CL_INVALID_KERNEL_ARGS);
    CV_OCL_CODE(CL_INVALID_WORK_DIMENSION);
    CV_OCL_CODE(CL_INVALID_WORK_GROUP_আকার);
    CV_OCL_CODE(CL_INVALID_WORK_ITEM_আকার);
    CV_OCL_CODE(CL_INVALID_GLOBAL_OFFSET);
    CV_OCL_CODE(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_CODE(CL_INVALID_EVENT);
    CV_OCL_CODE(CL_INVALID_OPERATION);
    CV_OCL_CODE(CL_INVALID_GL_OBJECT);
    CV_OCL_CODE(CL_INVALID_বাফার_আকার);
    CV_OCL_CODE(CL_INVALID_MIP_LEVEL);
    CV_OCL_CODE(CL_INVALID_GLOBAL_WORK_আকার);
    // OpenCL 1.1
    CV_OCL_CODE(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_CODE(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_গুনতি);
    // OpenCL 2.0
    CV_OCL_CODE_(-69, CL_INVALID_PIPE_আকার);
    CV_OCL_CODE_(-70, CL_INVALID_DEVICE_QUEUE);
    // Extensions
    CV_OCL_CODE_(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_CODE_(-1001, CL_PLATFORM_NOT_FOUND_KHR);
    CV_OCL_CODE_(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_CODE_(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_CODE_(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_CODE_(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    পূর্বনির্ধারিত: ফেরত "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
স্থির std::string kerToStr(ধ্রুবক Mat & k)
{
    পূর্ণসংখ্যা width = k.cols - 1, depth = k.depth();
    ধ্রুবক T * ধ্রুবক তথ্য = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    যদি (depth <= CV_8S)
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < width; ++i)
            stream << "DIG(" << (পূর্ণসংখ্যা)তথ্য[i] << ")";
        stream << "DIG(" << (পূর্ণসংখ্যা)তথ্য[width] << ")";
    }
    অন্যথা যদি (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        জন্য (পূর্ণসংখ্যা i = 0; i < width; ++i)
            stream << "DIG(" << তথ্য[i] << "f)";
        stream << "DIG(" << তথ্য[width] << "f)";
    }
    অন্যথা যদি (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        জন্য (পূর্ণসংখ্যা i = 0; i < width; ++i)
            stream << "DIG(" << (দশমিক)তথ্য[i] << "h)";
        stream << "DIG(" << (দশমিক)তথ্য[width] << "h)";
    }
    অন্যথা
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < width; ++i)
            stream << "DIG(" << তথ্য[i] << ")";
        stream << "DIG(" << তথ্য[width] << ")";
    }

    ফেরত stream.str();
}

String kernelToStr(প্রবেশ_Array _kernel, পূর্ণসংখ্যা ddepth, ধ্রুবক অক্ষর * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    পূর্ণসংখ্যা depth = kernel.depth();
    যদি (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    যদি (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    যদি (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    প্রকারলেখ std::string (* func_t)(ধ্রুবক Mat &);
    স্থির ধ্রুবক func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<অক্ষর>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<পূর্ণসংখ্যা>, kerToStr<দশমিক>, kerToStr<দ্বিগুণ>, kerToStr<hfloat>,
                                                0, kerToStr<যৌক্তিক>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ধ্রুবক func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ফেরত cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    করো \
    { \
        যদি (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            আকার_2 csize = src.size(); \
            পূর্ণসংখ্যা ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            যদি (cwidth < ckercn || ckercn <= 0) \
                ফেরত 1; \
            cols.push_back(cwidth); \
            যদি (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                ফেরত 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    যখন ((শূন্য)0, 0)

পূর্ণসংখ্যা predictOptimalVectorWidth(প্রবেশ_Array src1, প্রবেশ_Array src2, প্রবেশ_Array src3,
                              প্রবেশ_Array src4, প্রবেশ_Array src5, প্রবেশ_Array src6,
                              প্রবেশ_Array src7, প্রবেশ_Array src8, প্রবেশ_Array src9,
                              OclVectorStrategy strat)
{
    ধ্রুবক ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    পূর্ণসংখ্যা vectorWidths[CV_DEPTH_MAX] = {
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
    যদি (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    ফেরত checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

পূর্ণসংখ্যা checkOptimalVectorWidth([[maybe_unused]] ধ্রুবক পূর্ণসংখ্যা *vectorWidths,
                            [[maybe_unused]] প্রবেশ_Array src1,
                            [[maybe_unused]] প্রবেশ_Array src2,
                            [[maybe_unused]] প্রবেশ_Array src3,
                            [[maybe_unused]] প্রবেশ_Array src4,
                            [[maybe_unused]] প্রবেশ_Array src5,
                            [[maybe_unused]] প্রবেশ_Array src6,
                            [[maybe_unused]] প্রবেশ_Array src7,
                            [[maybe_unused]] প্রবেশ_Array src8,
                            [[maybe_unused]] প্রবেশ_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    ফেরত 1;
#অন্যথা
    CV_Assert(vectorWidths);

    পূর্ণসংখ্যা ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<পূর্ণসংখ্যা> dividers, kercns;
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

    জন্য (size_t i = 0; i < size; ++i)
        যখন (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    পূর্ণসংখ্যা kercn = *std::min_element(kercns.begin(), kercns.end());

    ফেরত kercn;
#endif
}

পূর্ণসংখ্যা predictOptimalVectorWidthMax(প্রবেশ_Array src1, প্রবেশ_Array src2, প্রবেশ_Array src3,
                                 প্রবেশ_Array src4, প্রবেশ_Array src5, প্রবেশ_Array src6,
                                 প্রবেশ_Array src7, প্রবেশ_Array src8, প্রবেশ_Array src9)
{
    ফেরত predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
শূন্য buildOptionsAddMatrixDescription(String& buildOptions, ধ্রুবক String& name, প্রবেশ_Array _m)
{
    যদি (!buildOptions.empty())
        buildOptions += " ";
    পূর্ণসংখ্যা type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (পূর্ণসংখ্যা)CV_MAT_CN(type),
            name.c_str(), (পূর্ণসংখ্যা)CV_ELEM_আকার(type),
            name.c_str(), (পূর্ণসংখ্যা)CV_ELEM_SIZE1(type),
            name.c_str(), (পূর্ণসংখ্যা)depth
            );
}


কাঠামো Image2D::Impl
{
    Impl(ধ্রুবক UMat &src, যৌক্তিক norm, যৌক্তিক alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        যদি (handle)
            clReleaseMemObject(handle);
    }

    স্থির cl_image_format getImageFormat(পূর্ণসংখ্যা depth, পূর্ণসংখ্যা cn, যৌক্তিক norm)
    {
        cl_image_format format;
        স্থির ধ্রুবক পূর্ণসংখ্যা channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        স্থির ধ্রুবক পূর্ণসংখ্যা channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        স্থির ধ্রুবক পূর্ণসংখ্যা channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        পূর্ণসংখ্যা channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        পূর্ণসংখ্যা channelOrder = channelOrders[cn];
        format.image_channel_তথ্য_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        ফেরত format;
    }

    স্থির যৌক্তিক isFormatSupported(cl_image_format format)
    {
        যদি (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        যদি (!context)
            ফেরত মিথ্যা;

        // Figure out how many formats are supported by this context.
        cl_uint অঙ্ক_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, অঙ্ক_Formats,
                                                ফাঁকা, &অঙ্ক_Formats);
        CV_OCL_DBG_CHECK_ফলাফল(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        যদি (অঙ্ক_Formats > 0)
        {
            Auto_বাফার<cl_image_format> formats(অঙ্ক_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, অঙ্ক_Formats,
                                             formats.তথ্য(), ফাঁকা);
            CV_OCL_DBG_CHECK_ফলাফল(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            জন্য (cl_uint i = 0; i < অঙ্ক_Formats; ++i)
            {
                যদি (!memcmp(&formats[i], &format, আকার(format)))
                {
                    ফেরত সত্য;
                }
            }
        }
        ফেরত মিথ্যা;
    }

    শূন্য init(ধ্রুবক UMat &src, যৌক্তিক norm, যৌক্তিক alias)
    {
        যদি (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        পূর্ণসংখ্যা err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        যদি (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        যদি (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ধ্রুবক Device & d = ocl::Device::getDefault();
        পূর্ণসংখ্যা minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        যদি (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_width      = src.cols;
            desc.image_height     = src.rows;
            desc.image_depth      = 0;
            desc.image_array_আকার = 1;
            desc.image_row_pitch  = alias ? src.step[0] : 0;
            desc.image_slice_pitch = 0;
            desc.বাফার           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.অঙ্ক_mip_levels   = 0;
            desc.অঙ্ক_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ফাঁকা, &err);
        }
        অন্যথা
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, ফাঁকা, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_ফলাফল(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem dev_তথ্য;
        যদি (!alias && !src.isContinuous())
        {
            dev_তথ্য = cl_Create_বাফার(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elem_আকার(), ফাঁকা, &err);
            CV_OCL_CHECK_ফলাফল(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long পূর্ণসংখ্যা)(src.cols * src.rows * src.elem_আকার()), (শূন্য*)dev_তথ্য
                ).c_str());

            ধ্রুবক size_t roi[3] = {static_cast<size_t>(src.cols) * src.elem_আকার(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(cl_Enqueue_Copy_বাফার_Rect(queue, (cl_mem)src.handle(ACCESS_READ), dev_তথ্য, origin, origin,
                roi, src.step, 0, src.cols * src.elem_আকার(), 0, 0, ফাঁকা, ফাঁকা));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        অন্যথা
        {
            dev_তথ্য = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_তথ্য != ফাঁকা);

        যদি (!alias)
        {
            CV_OCL_CHECK(cl_Enqueue_Copy_বাফার_To_Image(queue, dev_তথ্য, handle, 0, origin, region, 0, ফাঁকা, 0));
            যদি (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(queue));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_তথ্য));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = ফাঁকা;
}

Image2D::Image2D(ধ্রুবক UMat &src, যৌক্তিক norm, যৌক্তিক alias)
{
    p = নতুন Impl(src, norm, alias);
}

যৌক্তিক Image2D::canCreateAlias(ধ্রুবক UMat &m)
{
    যৌক্তিক ret = মিথ্যা;
    ধ্রুবক Device & d = ocl::Device::getDefault();
    যদি (d.image_From_বাফার_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        যদি (pitchAlign && !(m.step % (pitchAlign * m.elem_আকার())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            যদি (!m.u->অস্থায়ী_UMat())
            {
                ret = সত্য;
            }
        }
    }
    ফেরত ret;
}

যৌক্তিক Image2D::isFormatSupported(পূর্ণসংখ্যা depth, পূর্ণসংখ্যা cn, যৌক্তিক norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    ফেরত Impl::isFormatSupported(format);
}

Image2D::Image2D(ধ্রুবক Image2D & i)
{
    p = i.p;
    যদি (p)
        p->addref();
}

Image2D & Image2D::operator = (ধ্রুবক Image2D & i)
{
    যদি (i.p != p)
    {
        যদি (i.p)
            i.p->addref();
        যদি (p)
            p->release();
        p = i.p;
    }
    ফেরত *এই;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    যদি (এই != &i) {
        যদি (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ফেরত *এই;
}

Image2D::~Image2D()
{
    যদি (p)
        p->release();
}

শূন্য* Image2D::ptr() ধ্রুবক
{
    ফেরত p ? p->handle : 0;
}

যৌক্তিক internal::isOpenCLForced()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক value = মিথ্যা;
    যদি (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত value;
}

যৌক্তিক internal::isPerformanceCheckBypassed()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
    স্থির যৌক্তিক value = মিথ্যা;
    যদি (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", মিথ্যা);
        initialized = সত্য;
    }
    ফেরত value;
}

যৌক্তিক internal::isCLBuffer(UMat& u)
{
    যদি (isCudaBacked(u.u))
        ফেরত মিথ্যা;
    শূন্য* h = u.handle(ACCESS_RW);
    যদি (!h)
        ফেরত সত্য;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#যদি 1
    যদি ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ফেরত মিথ্যা;
#অন্যথা
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, আকার(type), &type, ফাঁকা);
    যদি (ret != CL_SUCCESS || type != CL_MEM_OBJECT_বাফার)
        ফেরত মিথ্যা;
#endif
    ফেরত সত্য;
}

কাঠামো Timer::Impl
{
    ধ্রুবক Queue queue;

    Impl(ধ্রুবক Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    শূন্য start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    শূন্য stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ধ্রুবক
    {
        ফেরত (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ধ্রুবক Queue& q) : p(নতুন Impl(q)) { }
Timer::~Timer() { delete p; }

শূন্য Timer::start()
{
    CV_Assert(p);
    p->start();
}

শূন্য Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ধ্রুবক
{
    CV_Assert(p);
    ফেরত p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
