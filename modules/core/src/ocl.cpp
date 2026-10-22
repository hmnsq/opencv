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

#অন্তর্ভাৱ "precomp.hpp"

#ifndef HAVE_OPENCL
#অন্তর্ভাৱ "ocl_disabled.impl.hpp"
#অন্যথা // HAVE_OPENCL

#অন্তর্ভাৱ <list>
#অন্তর্ভাৱ <map>
#অন্তর্ভাৱ <deque>
#অন্তর্ভাৱ <set>
#অন্তর্ভাৱ <string>
#অন্তর্ভাৱ <sstream>
#অন্তর্ভাৱ <fstream>
#যদি !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#অন্তর্ভাৱ <inttypes.h>
#endif

#অন্তর্ভাৱ <opencv2/core/utils/configuration.private.hpp>

#অন্তর্ভাৱ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>

#অন্তর্ভাৱ "opencv2/core/ocl_genbase.hpp"
#অন্তর্ভাৱ "opencl_kernels_core.hpp"

#অন্তর্ভাৱ "opencv2/core/utils/lock.private.hpp"
#অন্তর্ভাৱ "opencv2/core/utils/filesystem.hpp"
#অন্তর্ভাৱ "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(রিক্ত, 0, "OpenCL allocator: " << __VA_ARGS__)
#অন্তর্ভাৱ "opencv2/core/utils/allocator_stats.impl.hpp"
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

#অন্তর্ভাৱ "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# যদি 0
#   define LOG_BUFFER_POOL printf
# অন্যথা
#   define LOG_BUFFER_POOL(...)
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

#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#অন্তর্ভাৱ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#অন্তর্ভাৱ "opencv2/core/opencl/opencl_svm.hpp"
#endif

#অন্তর্ভাৱ "umatrix.hpp"

নামাকাশ cv { নামাকাশ ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    শূন্য addref() { CV_XADD(&refcount, 1); } \
    শূন্য release() { যদি( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete এতত্; } \
    পূর্ণাঙ্ক refcount

স্থির cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    ঘূৰাই opencl_allocator_stats;
}

#ifndef _DEBUG
স্থির তার্কিক isRaiseError()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক value = মিছা;
    যদি (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই value;
}
#endif

স্থির শূন্য onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    তার্কিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", মিছা);
    যদি (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#যদি CV_OPENCL_TRACE_CHECK
স্থির অন্তঃস্থ
শূন্য traceOpenCLCheck(cl_int status, ধ্রুৱ ৱর্ণ* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#অন্যথা
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_বাৰ্তা(check_result, বাৰ্তা) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, বাৰ্তা)

#define CV_OCL_CHECK_RESULT(check_result, বাৰ্তা) \
    কৰা { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, বাৰ্তা); \
        যদি (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(বাৰ্তা), ধ্রুৱ ৱর্ণ*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_বাৰ্তা = CV_OCL_API_ERROR_বাৰ্তা(check_result, বাৰ্তা); \
            CV_Error(Error::OpenCLApiCallError, error_বাৰ্তা); \
        } \
    } যেতিয়ালৈ (0)

#define CV_OCL_CHECK_(ৰাশিমালা, check_result) কৰা { ৰাশিমালা; CV_OCL_CHECK_RESULT(check_result, #expr); } যেতিয়ালৈ (0)

#define CV_OCL_CHECK(ৰাশিমালা) কৰা { cl_int __cl_result = (ৰাশিমালা); CV_OCL_CHECK_RESULT(__cl_result, #expr); } যেতিয়ালৈ (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, বাৰ্তা) CV_OCL_CHECK_RESULT(check_result, বাৰ্তা)
#define CV_OCL_DBG_CHECK(ৰাশিমালা) CV_OCL_CHECK(ৰাশিমালা)
#define CV_OCL_DBG_CHECK_(ৰাশিমালা, check_result) CV_OCL_CHECK_(ৰাশিমালা, check_result)
#অন্যথা
#define CV_OCL_DBG_CHECK_RESULT(check_result, বাৰ্তা) \
    কৰা { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, বাৰ্তা); \
        যদি (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(বাৰ্তা), ধ্রুৱ ৱর্ণ*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_বাৰ্তা = CV_OCL_API_ERROR_বাৰ্তা(check_result, বাৰ্তা); \
            CV_Error(Error::OpenCLApiCallError, error_বাৰ্তা); \
        } \
    } যেতিয়ালৈ (0)
#define CV_OCL_DBG_CHECK_(ৰাশিমালা, check_result) কৰা { ৰাশিমালা; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } যেতিয়ালৈ (0)
#define CV_OCL_DBG_CHECK(ৰাশিমালা) কৰা { cl_int __cl_result = (ৰাশিমালা); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } যেতিয়ালৈ (0)
#endif


স্থির ধ্রুৱ তার্কিক CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", সঁচা);
স্থির ধ্রুৱ তার্কিক CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", সঁচা);
স্থির ধ্রুৱ তার্কিক CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", সঁচা);
স্থির ধ্রুৱ তার্কিক CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", সঁচা);

#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
স্থির ধ্রুৱ তার্কিক CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", মিছা);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
স্থির ধ্রুৱ তার্কিক CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#যদি 1 //def __APPLE__
        সঁচা
#অন্যথা
        মিছা
#endif
);

স্থির String getBuildExtraOptions()
{
    স্থির String param_buildExtraOptions;
    স্থির তার্কিক initialized = মিছা;
    যদি (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = সঁচা;
        যদি (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(রিক্ত, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    ঘূৰাই param_buildExtraOptions;
}

স্থির ধ্রুৱ তার্কিক CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", সঁচা);
স্থির ধ্রুৱ size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


সংরচনা UMat2D
{
    UMat2D(ধ্রুৱ UMat& m)
    {
        offset = (পূর্ণাঙ্ক)m.offset;
        step = (পূর্ণাঙ্ক)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    পূর্ণাঙ্ক offset;
    পূর্ণাঙ্ক step;
    পূর্ণাঙ্ক rows;
    পূর্ণাঙ্ক cols;
};

সংরচনা UMat3D
{
    UMat3D(ধ্রুৱ UMat& m)
    {
        offset = (পূর্ণাঙ্ক)m.offset;
        step = (পূর্ণাঙ্ক)m.step.p[1];
        slicestep = (পূর্ণাঙ্ক)m.step.p[0];
        slices = (পূর্ণাঙ্ক)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    পূর্ণাঙ্ক offset;
    পূর্ণাঙ্ক slicestep;
    পূর্ণাঙ্ক step;
    পূর্ণাঙ্ক slices;
    পূর্ণাঙ্ক rows;
    পূর্ণাঙ্ক cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
স্থির uint64 crc64( ধ্রুৱ uchar* data, size_t size, uint64 crc0=0 )
{
    স্থির uint64 table[256];
    স্থির তার্কিক initialized = মিছা;

    যদি( !initialized )
    {
        বাবে( পূর্ণাঙ্ক i = 0; i < 256; i++ )
        {
            uint64 c = i;
            বাবে( পূর্ণাঙ্ক j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = সঁচা;
    }

    uint64 crc = ~crc0;
    বাবে( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    ঘূৰাই ~crc;
}

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
সংরচনা OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    প্রকারনির্দেশ std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(রিক্ত, "Initializing OpenCL cache configuration...");
        যদি (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(রিক্ত, "OpenCL cache is disabled");
            ঘূৰাই;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        যদি (cache_path_.empty())
        {
            CV_LOG_INFO(রিক্ত, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        কৰা
        {
            যত্ন
            {
                যদি (cache_path_.empty())
                    ভঙ্গ;
                যদি (cache_path_ == "disabled")
                    ভঙ্গ;
                যদি (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(রিক্ত, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    ভঙ্গ;
                }

                যদি (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    যদি (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(রিক্ত, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        যদি (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(রিক্ত, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            ভঙ্গ;
                        }
                    }

                    যত্ন
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(রিক্ত, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(রিক্ত, 0, "Checking cache lock... Done!");
                    }
                    গৃহাণ (ধ্রুৱ cv::Exception& e)
                    {
                        CV_LOG_WARNING(রিক্ত, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    গৃহাণ (...)
                    {
                        CV_LOG_WARNING(রিক্ত, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                অন্যথা
                {
                    যদি (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(রিক্ত, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    অন্যথা
                    {
                        CV_LOG_INFO(রিক্ত, "OpenCL cache lock is disabled");
                    }
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception& e)
            {
                CV_LOG_WARNING(রিক্ত, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } যেতিয়ালৈ (0);
        যদি (!cache_path_.empty())
        {
            যদি (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(রিক্ত, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            অন্যথা
            {
                CV_LOG_INFO(রিক্ত, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    শূন্য clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ধ্রুৱ std::string& ctx_prefix,
            ধ্রুৱ std::string& cleanup_prefix)
    {
        যদি (cache_path_.empty())
            ঘূৰাই std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        যদি (found_it != prepared_contexts_.end())
            ঘূৰাই found_it->second;

        CV_LOG_INFO(রিক্ত, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        তার্কিক result = utils::fs::isDirectory(target_directory);
        যদি (!result)
        {
            যত্ন
            {
                CV_LOG_VERBOSE(রিক্ত, 0, "Creating directory: " << target_directory);
                যদি (utils::fs::createDirectories(target_directory))
                {
                    result = সঁচা;
                }
                অন্যথা
                {
                    CV_LOG_WARNING(রিক্ত, "Can't create directory: " << target_directory);
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception& e)
            {
                CV_LOG_ERROR(রিক্ত, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        যদি (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            যত্ন
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, মিছা, সঁচা);
                std::vector<String> remove_entries;
                বাবে (size_t i = 0; i < entries.size(); i++)
                {
                    ধ্রুৱ String& name = entries[i];
                    যদি (0 == name.find(cleanup_prefix))
                    {
                        যদি (0 == name.find(ctx_prefix))
                            অনুৱর্ত; // skip current
                        remove_entries.push_back(name);
                    }
                }
                যদি (!remove_entries.empty())
                {
                    CV_LOG_WARNING(রিক্ত, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(রিক্ত, "Trying to remove these directories...");
                    বাবে (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(রিক্ত, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(রিক্ত, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    বাবে (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ধ্রুৱ String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        যত্ন
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(রিক্ত, "Removed: " << path);
                        }
                        গৃহাণ (ধ্রুৱ cv::Exception& e)
                        {
                            CV_LOG_ERROR(রিক্ত, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            গৃহাণ (...)
            {
                CV_LOG_WARNING(রিক্ত, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(রিক্ত, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        ঘূৰাই target_directory;
    }

    স্থির OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, নূতন OpenCLBinaryCacheConfigurator());
    }
};
শ্ৰেণী দ্বিমিক_Program_File
{
    গণনা { MAX_ENTRIES = 64 };

    প্রকারনির্দেশ unsigned পূর্ণাঙ্ক uint32_t;

    সংরচনা CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    সংরচনা CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t সংখ্যা_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    সংরচনা CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t চাবি_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ধ্রুৱ std::string fileName_;
    ধ্রুৱ ৱর্ণ* ধ্রুৱ sourceSignature_;
    ধ্রুৱ size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ধ্রুৱ std::string& options)
    {
        uint64 hash = crc64((ধ্রুৱ uchar*)options.c_str(), options.size(), 0);
        ঘূৰাই hash & (MAX_ENTRIES - 1);
    }

    অন্তঃস্থ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        ঘূৰাই fileSize;
    }
    অন্তঃস্থ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.পঠ((ৱর্ণ*)&res, পরিমাণ(uint32_t));
        CV_Assert(!f.fail());
        ঘূৰাই res;
    }
    অন্তঃস্থ শূন্য writeUInt32(ধ্রুৱ uint32_t value)
    {
        uint32_t v = value;
        f.লিখ((ৱর্ণ*)&v, পরিমাণ(uint32_t));
        CV_Assert(!f.fail());
    }

    অন্তঃস্থ শূন্য seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    অন্তঃস্থ শূন্য seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    অন্তঃস্থ শূন্য seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    শূন্য clearFile()
    {
        f.close();
        যদি (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(রিক্ত, "Can't remove: " << fileName_);
        ঘূৰাই;
    }

সার্ৱজনিক:
    দ্বিমিক_Program_File(ধ্রুৱ std::string& fileName, ধ্রুৱ ৱর্ণ* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(পরিমাণ(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != রিক্ত);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, পরিমাণ(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        যদি(f.is_open() && getFileSize() > 0)
        {
            তার্কিক isValid = মিছা;
            যত্ন
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                যদি (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<ৱর্ণ> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.পঠ(fileSourceSignature.data(), fileSourceSignatureSize);
                    যদি (f.eof())
                    {
                        CV_LOG_ERROR(রিক্ত, "Unexpected EOF");
                    }
                    অন্যথা যদি (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = সঁচা;
                    }
                }
                যদি (!isValid)
                {
                    CV_LOG_ERROR(রিক্ত, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception& e)
            {
                CV_LOG_ERROR(রিক্ত, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            গৃহাণ (...)
            {
                CV_LOG_ERROR(রিক্ত, "Can't open binary program file: " << fileName << " : Unknown error");
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

    তার্কিক পঠ(ধ্রুৱ std::string& চাবি, std::vector<ৱর্ণ>& buf)
    {
        যদি (!f.is_open())
            ঘূৰাই মিছা;

        size_t fileSize = getFileSize();
        যদি (fileSize == 0)
        {
            CV_LOG_ERROR(রিক্ত, "Invalid file (empty): " << fileName_);
            clearFile();
            ঘূৰাই মিছা;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t সংখ্যা_Of_Entries = readUInt32();
        CV_Assert(সংখ্যা_Of_Entries > 0);
        যদি (সংখ্যা_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(রিক্ত, "Invalid file: " << fileName_);
            clearFile();
            ঘূৰাই মিছা;
        }
        f.পঠ((ৱর্ণ*)&entryOffsets[0], পরিমাণ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_অঙ্ক = getHash(চাবি);

        uint32_t entryOffset = entryOffsets[entry_অঙ্ক];
        FileEntry entry;
        যেতিয়ালৈ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.পঠ((ৱর্ণ*)&entry, পরিমাণ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ৱর্ণ> file_চাবি(entry.চাবি_Size + 1);
            যদি (চাবি.size() == entry.চাবি_Size)
            {
                যদি (entry.চাবি_Size > 0)
                {
                    f.পঠ(file_চাবি.data(), entry.চাবি_Size);
                    CV_Assert(!f.fail());
                }
                যদি (memcmp(file_চাবি.data(), চাবি.c_str(), entry.চাবি_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.পঠ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(রিক্ত, 0, "Read...");
                    ঘূৰাই সঁচা;
                }
            }
            যদি (entry.nextEntryFileOffset == 0)
                ভঙ্গ;
            entryOffset = entry.nextEntryFileOffset;
        }
        ঘূৰাই মিছা;
    }

    তার্কিক লিখ(ধ্রুৱ std::string& চাবি, std::vector<ৱর্ণ>& buf)
    {
        যদি (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            যদি (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                যদি (!f.is_open())
                {
                    CV_LOG_ERROR(রিক্ত, "Can't create file: " << fileName_);
                    ঘূৰাই মিছা;
                }
            }
        }

        size_t fileSize = getFileSize();
        যদি (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.লিখ(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, পরিমাণ(entryOffsets));
            f.লিখ((ৱর্ণ*)entryOffsets, পরিমাণ(entryOffsets));
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

        uint32_t সংখ্যা_Of_Entries = readUInt32();
        CV_Assert(সংখ্যা_Of_Entries > 0);
        যদি (সংখ্যা_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(রিক্ত, "Invalid file: " << fileName_);
            clearFile();
            ঘূৰাই মিছা;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.পঠ((ৱর্ণ*)&entryOffsets[0], পরিমাণ(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_অঙ্ক = getHash(চাবি);

        uint32_t entryOffset = entryOffsets[entry_অঙ্ক];
        FileEntry entry;
        যেতিয়ালৈ (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.পঠ((ৱর্ণ*)&entry, পরিমাণ(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<ৱর্ণ> file_চাবি(entry.চাবি_Size + 1);
            যদি (চাবি.size() == entry.চাবি_Size)
            {
                যদি (entry.চাবি_Size > 0)
                {
                    f.পঠ(file_চাবি.data(), entry.চাবি_Size);
                    CV_Assert(!f.fail());
                }
                যদি (0 == memcmp(file_চাবি.data(), চাবি.c_str(), entry.চাবি_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(রিক্ত, 0, "Duplicate key ignored: " << fileName_);
                    ঘূৰাই মিছা;
                }
            }
            যদি (entry.nextEntryFileOffset == 0)
                ভঙ্গ;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        যদি (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.লিখ((ৱর্ণ*)&entry, পরিমাণ(entry));
            CV_Assert(!f.fail());
        }
        অন্যথা
        {
            entryOffsets[entry_অঙ্ক] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.লিখ((ৱর্ণ*)entryOffsets, পরিমাণ(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.চাবি_Size = (uint32_t)চাবি.size();
        f.লিখ((ৱর্ণ*)&entry, পরিমাণ(entry));
        CV_Assert(!f.fail());
        f.লিখ(চাবি.c_str(), entry.চাবি_Size);
        CV_Assert(!f.fail());
        f.লিখ(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(রিক্ত, 0, "Write... (" << buf.size() << " bytes)");
        ঘূৰাই সঁচা;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



সংরচনা OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    পূর্ণাঙ্ক device_;  // device index in context
    ocl::Queue queue_;
    পূর্ণাঙ্ক useOpenCL_;

রক্ষিত:
    Impl() = delete;

    শূন্য _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        পূর্ণাঙ্ক ndevices = (পূর্ণাঙ্ক)context_.ndevices();
        CV_Assert(ndevices > 0);
        তার্কিক found = মিছা;
        বাবে (পূর্ণাঙ্ক i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            যদি (dhandle == deviceID)
            {
                device_ = i;
                found = সঁচা;
                ভঙ্গ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    শূন্য _init_device(ধ্রুৱ ocl::Device& device)
    {
        CV_Assert(device.ptr());
        পূর্ণাঙ্ক ndevices = (পূর্ণাঙ্ক)context_.ndevices();
        CV_Assert(ndevices > 0);
        তার্কিক found = মিছা;
        বাবে (পূর্ণাঙ্ক i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            যদি (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = সঁচা;
                ভঙ্গ;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

সার্ৱজনিক:
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

    Impl(ধ্রুৱ ocl::Context& context, ধ্রুৱ ocl::Device& device, ধ্রুৱ ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ধ্রুৱ ocl::Context& context, ধ্রুৱ ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ধ্রুৱ ocl::Context& context, ধ্রুৱ পূর্ণাঙ্ক device, ধ্রুৱ ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ধ্রুৱ Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    অন্তঃস্থ তার্কিক useOpenCL() ধ্রুৱ { ঘূৰাই const_cast<Impl*>(এতত্)->useOpenCL(); }
    তার্কিক useOpenCL()
    {
        যদি (useOpenCL_ < 0)
        {
            যত্ন
            {
                useOpenCL_ = 0;
                যদি (!context_.empty() && context_.ndevices() > 0)
                {
                    ধ্রুৱ Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception&)
            {
                // nothing
            }
            যদি (!useOpenCL_)
                CV_LOG_INFO(রিক্ত, "OpenCL: can't use OpenCL execution context");
        }
        ঘূৰাই useOpenCL_ > 0;
    }

    শূন্য setUseOpenCL(তার্কিক flag)
    {
        যদি (!flag)
            useOpenCL_ = 0;
        অন্যথা
            useOpenCL_ = -1;
    }

    স্থির ধ্রুৱ std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(রিক্ত, "OpenCL: initializing thread execution context");

        স্থির তার্কিক initialized = মিছা;
        স্থির std::shared_ptr<Impl> g_primaryExecutionContext;

        যদি (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            যদি (!initialized)
            {
                CV_LOG_INFO(রিক্ত, "OpenCL: creating new execution context...");
                যত্ন
                {
                    Context c = ocl::Context::create(std::string());
                    যদি (c.ndevices())
                    {
                        পূর্ণাঙ্ক deviceId = 0;
                        auto& d = c.device(deviceId);
                        যদি (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            যদি (!q.ptr())
                            {
                                CV_LOG_ERROR(রিক্ত, "OpenCL: Can't create default OpenCL queue");
                            }
                            অন্যথা
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(রিক্ত, "OpenCL: device=" << d.name());
                            }
                        }
                        অন্যথা
                        {
                            CV_LOG_ERROR(রিক্ত, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    অন্যথা
                    {
                        CV_LOG_INFO(রিক্ত, "OpenCL: context is not available/disabled");
                    }
                }
                গৃহাণ (ধ্রুৱ std::exception& e)
                {
                    CV_LOG_INFO(রিক্ত, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                গৃহাণ (...)
                {
                    CV_LOG_WARNING(রিক্ত, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = সঁচা;
            }
        }
        ঘূৰাই g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->context_;
}
Device& OpenCLExecutionContext::getDevice() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->queue_;
}

তার্কিক OpenCLExecutionContext::useOpenCL() ধ্রুৱ
{
    যদি (p)
        ঘূৰাই p->useOpenCL();
    ঘূৰাই মিছা;
}
শূন্য OpenCLExecutionContext::setUseOpenCL(তার্কিক flag)
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
    যদি (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = সঁচা;
        যদি (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    ঘূৰাই c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    ঘূৰাই c;
}

শূন্য OpenCLExecutionContext::bind() ধ্রুৱ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *এতত্;
    data.oclExecutionContextInitialized = সঁচা;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ধ্রুৱ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ধ্রুৱ Queue q(getContext(), getDevice());
    ঘূৰাই cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ধ্রুৱ ocl::Queue& q) ধ্রুৱ
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != রিক্ত);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    ঘূৰাই c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুৱ Context& context, ধ্রুৱ Device& device, ধ্রুৱ ocl::Queue& queue)
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
    ঘূৰাই ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ধ্রুৱ Context& context, ধ্রুৱ Device& device)
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
    ঘূৰাই ctx;

}

শূন্য OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
স্থির তার্কিক g_isOpenCLInitialized = মিছা;
স্থির তার্কিক g_isOpenCLAvailable = মিছা;

তার্কিক haveOpenCL()
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
                g_isOpenCLAvailable = মিছা;
                g_isOpenCLInitialized = সঁচা;
                ঘূৰাই মিছা;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(রিক্ত, "Initialize OpenCL runtime...");
        যত্ন
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, রিক্ত, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(রিক্ত, "OpenCL: found " << n << " platforms");
        }
        গৃহাণ (...)
        {
            g_isOpenCLAvailable = মিছা;
        }
        g_isOpenCLInitialized = সঁচা;
    }
    ঘূৰাই g_isOpenCLAvailable;
}

তার্কিক useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    যদি (data.useOpenCL < 0)
    {
        যত্ন
        {
            data.useOpenCL = 0;
            যদি (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        গৃহাণ (...)
        {
            CV_LOG_INFO(রিক্ত, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    ঘূৰাই data.useOpenCL > 0;
}

তার্কিক isOpenCLActivated()
{
    যদি (!g_isOpenCLAvailable)
        ঘূৰাই মিছা; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    ঘূৰাই useOpenCL();
}

শূন্য setUseOpenCL(তার্কিক flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    যদি (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    অন্যথা
    {
        যদি (!flag)
            data.useOpenCL = 0;
        অন্যথা
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

শ্ৰেণী AmdBlasHelper
{
সার্ৱজনিক:
    স্থির AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, নূতন AmdBlasHelper())
    }

    তার্কিক isAvailable() ধ্রুৱ
    {
        ঘূৰাই g_isAmdBlasAvailable;
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

রক্ষিত:
    AmdBlasHelper()
    {
        যদি (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            যদি (!g_isAmdBlasInitialized)
            {
                যদি (haveOpenCL())
                {
                    যত্ন
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    গৃহাণ (...)
                    {
                        g_isAmdBlasAvailable = মিছা;
                    }
                }
                অন্যথা
                    g_isAmdBlasAvailable = মিছা;

                g_isAmdBlasInitialized = সঁচা;
            }
        }
    }

গুপ্ত:
    স্থির তার্কিক g_isAmdBlasInitialized;
    স্থির তার্কিক g_isAmdBlasAvailable;
};

তার্কিক AmdBlasHelper::g_isAmdBlasAvailable = মিছা;
তার্কিক AmdBlasHelper::g_isAmdBlasInitialized = মিছা;

তার্কিক haveAmdBlas()
{
    ঘূৰাই AmdBlasHelper::getInstance().isAvailable();
}

#অন্যথা

তার্কিক haveAmdBlas()
{
    ঘূৰাই মিছা;
}

#endif

#ifdef HAVE_CLAMDFFT

শ্ৰেণী AmdFftHelper
{
সার্ৱজনিক:
    স্থির AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, নূতন AmdFftHelper())
    }

    তার্কিক isAvailable() ধ্রুৱ
    {
        ঘূৰাই g_isAmdFftAvailable;
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

রক্ষিত:
    AmdFftHelper()
    {
        যদি (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            যদি (!g_isAmdFftInitialized)
            {
                যদি (haveOpenCL())
                {
                    যত্ন
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = সঁচা;
                    }
                    গৃহাণ (ধ্রুৱ Exception &)
                    {
                        g_isAmdFftAvailable = মিছা;
                    }
                }
                অন্যথা
                    g_isAmdFftAvailable = মিছা;

                g_isAmdFftInitialized = সঁচা;
            }
        }
    }

গুপ্ত:
    স্থির clfftSetupData setupData;
    স্থির তার্কিক g_isAmdFftInitialized;
    স্থির তার্কিক g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
তার্কিক AmdFftHelper::g_isAmdFftAvailable = মিছা;
তার্কিক AmdFftHelper::g_isAmdFftInitialized = মিছা;

তার্কিক haveAmdFft()
{
    ঘূৰাই AmdFftHelper::getInstance().isAvailable();
}

#অন্যথা

তার্কিক haveAmdFft()
{
    ঘূৰাই মিছা;
}

#endif

তার্কিক haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    ঘূৰাই সঁচা;
#অন্যথা
    ঘূৰাই মিছা;
#endif
}

শূন্য finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

সংরচনা Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = মিছা;
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
                ৱর্ণ buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, পরিমাণ(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = সঁচা;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    তার্কিক initialized;
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

Platform::Platform(ধ্রুৱ Platform& pl)
{
    p = (Impl*)pl.p;
    যদি(p)
        p->addref();
}

Platform& Platform::operator = (ধ্রুৱ Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    যদি (এতত্ != &pl) {
        যদি(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

শূন্য* Platform::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(রিক্ত, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    স্থির Platform p;
    যদি( !p.p )
    {
        p.p = নূতন Impl;
        p.p->init();
    }
    ঘূৰাই p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
স্থির শূন্য parseOpenCLVersion(ধ্রুৱ String &version, পূর্ণাঙ্ক &major, পূর্ণাঙ্ক &minor)
{
    major = minor = 0;
    যদি (10 >= version.length())
        ঘূৰাই;
    ধ্রুৱ ৱর্ণ *pstr = version.c_str();
    যদি (0 != strncmp(pstr, "OpenCL ", 7))
        ঘূৰাই;
    size_t ppos = version.find('.', 7);
    যদি (String::npos == ppos)
        ঘূৰাই;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

সংরচনা Device::Impl
{
    Impl(শূন্য* d)
        : refcount(1)
        , handle(0)
    {
        যত্ন
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        গৃহাণ (...)
        {
            ক্ষিপ;
        }
    }

    শূন্য _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, পূর্ণাঙ্ক>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, পূর্ণাঙ্ক>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, পূর্ণাঙ্ক>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        যেতিয়ালৈ (pos < extensions_.size())
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

        ধ্রুৱ size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        যদি (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ধ্রুৱ size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            যদি (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(রিক্ত, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
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
    _TpOut getProp(cl_device_info prop) ধ্রুৱ
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        ঘূৰাই clGetDeviceInfo(handle, prop, পরিমাণ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == পরিমাণ(temp) ? _TpOut(temp) : _TpOut();
    }

    তার্কিক getBoolProp(cl_device_info prop) ধ্রুৱ
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        ঘূৰাই clGetDeviceInfo(handle, prop, পরিমাণ(temp), &temp, &sz) == CL_SUCCESS &&
            sz == পরিমাণ(temp) ? temp != 0 : মিছা;
    }

    String getStrProp(cl_device_info prop) ধ্রুৱ
    {
        ৱর্ণ buf[4096];
        size_t sz=0;
        ঘূৰাই clGetDeviceInfo(handle, prop, পরিমাণ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < পরিমাণ(buf) ? String(buf) : String();
    }

    তার্কিক isExtensionSupported(ধ্রুৱ std::string& extensionName) ধ্রুৱ
    {
        ঘূৰাই extensions_set_.গুনতি_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    পূর্ণাঙ্ক doubleFPConfig_;
    তার্কিক khr_fp64_support_;
    পূর্ণাঙ্ক halfFPConfig_;
    তার্কিক khr_fp16_support_;
    তার্কিক hostUnifiedMemory_;
    পূর্ণাঙ্ক maxComputeUnits_;
    size_t maxWorkGroupSize_;
    পূর্ণাঙ্ক type_;
    পূর্ণাঙ্ক addressBits_;
    পূর্ণাঙ্ক deviceVersionMajor_;
    পূর্ণাঙ্ক deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    পূর্ণাঙ্ক vendorID_;
    তার্কিক intelSubgroupsSupport_;

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

Device::Device(ধ্রুৱ Device& d)
{
    p = d.p;
    যদি(p)
        p->addref();
}

Device& Device::operator = (ধ্রুৱ Device& d)
{
    Impl* newp = (Impl*)d.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    যদি (এতত্ != &d) {
        যদি(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    ঘূৰাই *এতত্;
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
    p = নূতন Impl(d);
    যদি (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(শূন্য* d)
{
    Device device(d);
    ঘূৰাই device;
}

শূন্য* Device::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

String Device::name() ধ্রুৱ
{ ঘূৰাই p ? p->name_ : String(); }

String Device::extensions() ধ্রুৱ
{ ঘূৰাই p ? String(p->extensions_) : String(); }

তার্কিক Device::isExtensionSupported(ধ্রুৱ String& extensionName) ধ্রুৱ
{ ঘূৰাই p ? p->isExtensionSupported(extensionName) : মিছা; }

String Device::version() ধ্রুৱ
{ ঘূৰাই p ? p->version_ : String(); }

String Device::vendorName() ধ্রুৱ
{ ঘূৰাই p ? p->vendorName_ : String(); }

পূর্ণাঙ্ক Device::vendorID() ধ্রুৱ
{ ঘূৰাই p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ধ্রুৱ
{ ঘূৰাই p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ধ্রুৱ
{ ঘূৰাই p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

পূর্ণাঙ্ক Device::deviceVersionMajor() ধ্রুৱ
{ ঘূৰাই p ? p->deviceVersionMajor_ : 0; }

পূর্ণাঙ্ক Device::deviceVersionMinor() ধ্রুৱ
{ ঘূৰাই p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ধ্রুৱ
{ ঘূৰাই p ? p->driverVersion_ : String(); }

পূর্ণাঙ্ক Device::type() ধ্রুৱ
{ ঘূৰাই p ? p->type_ : 0; }

পূর্ণাঙ্ক Device::addressBits() ধ্রুৱ
{ ঘূৰাই p ? p->addressBits_ : 0; }

তার্কিক Device::available() ধ্রুৱ
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : মিছা; }

তার্কিক Device::compilerAvailable() ধ্রুৱ
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : মিছা; }

তার্কিক Device::linkerAvailable() ধ্রুৱ
#ifdef CL_VERSION_1_2
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : মিছা; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

পূর্ণাঙ্ক Device::doubleFPConfig() ধ্রুৱ
{ ঘূৰাই p ? p->doubleFPConfig_ : 0; }

পূর্ণাঙ্ক Device::singleFPConfig() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_device_fp_config, পূর্ণাঙ্ক>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

পূর্ণাঙ্ক Device::halfFPConfig() ধ্রুৱ
{ ঘূৰাই p ? p->halfFPConfig_ : 0; }

তার্কিক Device::hasFP64() ধ্রুৱ
{ ঘূৰাই p ? p->khr_fp64_support_ : মিছা; }
তার্কিক Device::hasFP16() ধ্রুৱ
{ ঘূৰাই p ? p->khr_fp16_support_ : মিছা; }

তার্কিক Device::endianLittle() ধ্রুৱ
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : মিছা; }

তার্কিক Device::errorCorrectionSupport() ধ্রুৱ
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : মিছা; }

পূর্ণাঙ্ক Device::executionCapabilities() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_device_exec_capabilities, পূর্ণাঙ্ক>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

পূর্ণাঙ্ক Device::globalMemCacheType() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_device_mem_cache_type, পূর্ণাঙ্ক>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

পূর্ণাঙ্ক Device::global_Mem_Cache_সাৰি_Size() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

পূর্ণাঙ্ক Device::localMemType() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_device_local_mem_type, পূর্ণাঙ্ক>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

তার্কিক Device::hostUnifiedMemory() ধ্রুৱ
{ ঘূৰাই p ? p->hostUnifiedMemory_ : মিছা; }

তার্কিক Device::imageSupport() ধ্রুৱ
{ ঘূৰাই p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : মিছা; }

তার্কিক Device::imageFromBufferSupport() ধ্রুৱ
{
    ঘূৰাই p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : মিছা;
}

uint Device::imagePitchAlignment() ধ্রুৱ
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    ঘূৰাই p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#অন্যথা
    ঘূৰাই 0;
#endif
}

uint Device::imageBaseAddressAlignment() ধ্রুৱ
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    ঘূৰাই p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#অন্যথা
    ঘূৰাই 0;
#endif
}

size_t Device::image2DMaxWidth() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ধ্রুৱ
#ifdef CL_VERSION_1_2
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ধ্রুৱ
#ifdef CL_VERSION_1_2
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

তার্কিক Device::intelSubgroupsSupport() ধ্রুৱ
{ ঘূৰাই p ? p->intelSubgroupsSupport_ : মিছা; }

পূর্ণাঙ্ক Device::maxClockFrequency() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

পূর্ণাঙ্ক Device::maxComputeUnits() ধ্রুৱ
{ ঘূৰাই p ? p->maxComputeUnits_ : 0; }

পূর্ণাঙ্ক Device::maxConstantArgs() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

পূর্ণাঙ্ক Device::maxReadImageArgs() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

পূর্ণাঙ্ক Device::maxWriteImageArgs() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

পূর্ণাঙ্ক Device::maxSamplers() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ধ্রুৱ
{ ঘূৰাই p ? p->maxWorkGroupSize_ : 0; }

পূর্ণাঙ্ক Device::maxWorkItemDims() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

শূন্য Device::maxWorkItemSizes(size_t* sizes) ধ্রুৱ
{
    যদি(p)
    {
        ধ্রুৱ পূর্ণাঙ্ক MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*পরিমাণ(sizes[0]), &sizes[0], &retsz));
    }
}

পূর্ণাঙ্ক Device::memBaseAddrAlign() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthChar() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthShort() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthInt() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthLong() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthFloat() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthDouble() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

পূর্ণাঙ্ক Device::nativeVectorWidthHalf() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthChar() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthShort() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthInt() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthLong() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthFloat() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthDouble() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

পূর্ণাঙ্ক Device::preferredVectorWidthHalf() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<cl_uint, পূর্ণাঙ্ক>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ধ্রুৱ
#ifdef CL_VERSION_1_2
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#অন্যথা
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ধ্রুৱ
{ ঘূৰাই p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ধ্রুৱ Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        ঘূৰাই c.getDevice();
    }

    স্থির Device dummy;
    ঘূৰাই dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
অন্তঃস্থ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, রিক্ত, &required);
    যদি (err != CL_SUCCESS)
        ঘূৰাই err;

    param.clear();
    যদি (required > 0)
    {
        AutoBuffer<ৱর্ণ> buf(required + 1);
        ৱর্ণ* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, রিক্ত);
        যদি (err != CL_SUCCESS)
            ঘূৰাই err;
        param = ptr;
    }

    ঘূৰাই CL_SUCCESS;
}

স্থির শূন্য split(ধ্রুৱ std::string &s, ৱর্ণ delim, std::vector<std::string> &elems)
{
    elems.clear();
    যদি (s.size() == 0)
        ঘূৰাই;
    std::istringstream ss(s);
    std::string item;
    যেতিয়ালৈ (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
স্থির তার্কিক parseOpenCLDeviceConfiguration(ধ্রুৱ std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    যদি (parts.size() > 3)
    {
        CV_LOG_ERROR(রিক্ত, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        ঘূৰাই মিছা;
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
    ঘূৰাই সঁচা;
}

স্থির cl_device_id selectOpenCLDevice(ধ্রুৱ std::string & configuration_ = std::string())
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
        ঘূৰাই রিক্ত;

    তার্কিক isID = মিছা;
    পূর্ণাঙ্ক deviceID = -1;
    যদি (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = সঁচা;
        বাবে (size_t i = 0; i < deviceName.length(); i++)
        {
            যদি (!isdigit(deviceName[i]))
            {
                isID = মিছা;
                ভঙ্গ;
            }
        }
        যদি (isID)
        {
            deviceID = atoi(deviceName.c_str());
            যদি (deviceID < 0)
                ঘূৰাই রিক্ত;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint অঙ্ক_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, রিক্ত, &অঙ্ক_Platforms));

        যদি (অঙ্ক_Platforms == 0)
            ঘূৰাই রিক্ত;
        platforms.resize((size_t)অঙ্ক_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], &অঙ্ক_Platforms));
        platforms.resize(অঙ্ক_Platforms);
    }

    যদি (platform.length() > 0)
    {
        বাবে (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
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
            CV_LOG_ERROR(রিক্ত, "OpenCL: Can't find OpenCL platform by name: " << platform);
            গচ্ছ not_found;
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
    বাবে (size_t t = 0; t < deviceTypes.size(); t++)
    {
        পূর্ণাঙ্ক deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        যদি (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        অন্যথা যদি (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        অন্যথা যদি (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        অন্যথা যদি (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        অন্যথা
        {
            CV_LOG_ERROR(রিক্ত, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            গচ্ছ not_found;
        }

        std::vector<cl_device_id> devices;
        বাবে (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint গুনতি_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, রিক্ত, &গুনতি_2);
            যদি (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            যদি (গুনতি_2 == 0)
                অনুৱর্ত;
            size_t base = devices.size();
            devices.resize(base + গুনতি_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, গুনতি_2, &devices[base], &গুনতি_2);
            যদি (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        বাবে (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : সঁচা) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = সঁচা;
            যদি(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, পরিমাণ(isIGPU), &isIGPU, রিক্ত));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            যদি ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                ঘূৰাই devices[i];
            }
        }
    }

not_found:
    যদি (configuration.empty())
        ঘূৰাই রিক্ত; // suppress messages on stderr

    std::ostringstream বাৰ্তা;
    বাৰ্তা << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    বাবে (size_t t = 0; t < deviceTypes.size(); t++)
        বাৰ্তা << ' ' << deviceTypes[t];

    বাৰ্তা << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(রিক্ত, বাৰ্তা.str());
    ঘূৰাই রিক্ত;
}

#ifdef HAVE_OPENCL_SVM
নামাকাশ svm {

গণনা AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

স্থির তার্কিক checkForceSVMUmatUsage()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক force = মিছা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই force;
}
স্থির তার্কিক checkDisableSVMUMatUsage()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক force = মিছা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই force;
}
স্থির তার্কিক checkDisableSVM()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক force = মিছা;
    যদি (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই force;
}
// see SVMCapabilities
স্থির unsigned পূর্ণাঙ্ক getSVMCapabilitiesMask()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির unsigned পূর্ণাঙ্ক mask = 0;
    যদি (!initialized)
    {
        ধ্রুৱ std::string env_মান = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        যদি (env_মান.empty())
        {
            ঘূৰাই ~0U; // all bits 1
        }
        mask = atoi(env_মান.c_str());
        initialized = সঁচা;
    }
    ঘূৰাই mask;
}
} // namespace
#endif

স্থির size_t get_Program_গুনতি_Limit()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির size_t গুনতি_2 = 0;
    যদি (!initialized)
    {
        গুনতি_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = সঁচা;
    }
    ঘূৰাই গুনতি_2;
}

স্থির পূর্ণাঙ্ক g_contextId = 0;

শ্ৰেণী OpenCLBufferPoolImpl;
শ্ৰেণী OpenCLSVMBufferPoolImpl;

সংরচনা Context::Impl
{
    স্থির Context::Impl* get(Context& context) { ঘূৰাই context.getImpl(); }

    প্রকারনির্দেশ std::deque<Context::Impl*> container_t;
    স্থির container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        স্থির container_t* g_contexts = নূতন container_t();
        ঘূৰাই *g_contexts;
    }

রক্ষিত:
    Impl(ধ্রুৱ std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(মিছা)
#endif
    {
        যদি (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = এতত্;
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
                handle = রিক্ত;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = রিক্ত;
        }
    }

    শূন্য init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_অঙ্ক_DEVICES, পরিমাণ(ndevices), &ndevices, রিক্ত));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * পরিমাণ(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * পরিমাণ(cl_device_id), "");

        devices.clear();
        বাবে (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    শূন্য __init_buffer_pools();  // w/o synchronization
    শূন্য _init_buffer_pools() ধ্রুৱ
    {
        যদি (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            যদি (!bufferPool_)
            {
                const_cast<Impl*>(এতত্)->__init_buffer_pools();
            }
        }
    }
সার্ৱজনিক:
    স্থির Impl* findContext(ধ্রুৱ std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        যদি (configuration.empty() && !container.empty())
            ঘূৰাই container[0];
        বাবে (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            যদি (i && i->configuration == configuration)
            {
                ঘূৰাই i;
            }
        }
        ঘূৰাই রিক্ত;
    }

    স্থির Impl* findOrCreateContext(ধ্রুৱ std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        যদি (configuration_.empty())
        {
            ধ্রুৱ std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            যদি (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        যদি (impl)
        {
            CV_LOG_INFO(রিক্ত, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ঘূৰাই impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        যদি (d == রিক্ত)
            ঘূৰাই রিক্ত;

        impl = নূতন Impl(configuration);
        যত্ন
        {
            impl->createFromDevice(d);
            যদি (impl->handle)
                ঘূৰাই impl;
            delete impl;
            ঘূৰাই রিক্ত;
        }
        গৃহাণ (...)
        {
            delete impl;
            ক্ষিপ;
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
            CV_LOG_INFO(রিক্ত, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ঘূৰাই impl;
        }

        impl = নূতন Impl(configuration);
        যত্ন
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            ঘূৰাই impl;
        }
        গৃহাণ (...)
        {
            delete impl;
            ক্ষিপ;
        }
    }

    স্থির Impl* findOrCreateContext(ধ্রুৱ ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (শূন্য*)d);
        Impl* impl = findContext(configuration);
        যদি (impl)
        {
            CV_LOG_INFO(রিক্ত, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            ঘূৰাই impl;
        }

        impl = নূতন Impl(configuration);
        যত্ন
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            ঘূৰাই impl;
        }
        গৃহাণ (...)
        {
            delete impl;
            ক্ষিপ;
        }
    }

    শূন্য setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        যদি (d == রিক্ত)
            ঘূৰাই;

        createFromDevice(d);
    }

    শূন্য createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == রিক্ত);

        cl_platform_id pl = রিক্ত;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, পরিমাণ(cl_platform_id), &pl, রিক্ত));

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

        তার্কিক ok = handle != 0 && status == CL_SUCCESS;
        যদি( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        অন্যথা
            handle = রিক্ত;
    }

    Program getProg(ধ্রুৱ ProgramSource& src, ধ্রুৱ String& buildflags, String& errmsg);

    শূন্য unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        বাবে (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              যদি (it != phash.end())
              {
                  যদি (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      ঘূৰাই;
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
                ধ্রুৱ Device& d = devices[0];
                পূর্ণাঙ্ক bits = d.addressBits();
                যদি (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                বাবে (size_t i = 0; i < prefix.size(); i++)
                {
                    ৱর্ণ c = prefix[i];
                    যদি (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        ঘূৰাই prefix;
    }

    std::string& getPrefixBase()
    {
        যদি (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            যদি (prefix_base.empty())
            {
                ধ্রুৱ Device& d = devices[0];
                পূর্ণাঙ্ক bits = d.addressBits();
                যদি (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                বাবে (size_t i = 0; i < prefix_base.size(); i++)
                {
                    ৱর্ণ c = prefix_base[i];
                    যদি (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        ঘূৰাই prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ধ্রুৱ পূর্ণাঙ্ক contextId;  // global unique ID
    ধ্রুৱ std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    প্রকারনির্দেশ std::map<std::string, Program> phash_t;
    phash_t phash;
    প্রকারনির্দেশ std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ধ্রুৱ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        ঘূৰাই *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ধ্রুৱ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        ঘূৰাই *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    শূন্য setUserContext(std::type_index typeId, ধ্রুৱ std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        যদি (it != userContextStorage.end())
            ঘূৰাই it->second;
        অন্যথা
            ঘূৰাই nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    তার্কিক svmInitialized;
    তার্কিক svmAvailable;
    তার্কিক svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    শূন্য svmInit()
    {
        CV_Assert(handle != রিক্ত);
        ধ্রুৱ Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((শূন্য)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, পরিমাণ(deviceCaps), &deviceCaps, রিক্ত);
        যদি (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            গচ্ছ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (পূর্ণাঙ্ক)deviceCaps);
        CV_Assert(((শূন্য)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.মান_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.মান_2 &= svm::getSVMCapabilitiesMask();
        যদি (svmCapabilities.মান_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            গচ্ছ noSVM;
        }
        যত্ন
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            শূন্য* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            যদি (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            যত্ন
            {
                তার্কিক error = মিছা;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                যদি (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, রিক্ত, রিক্ত))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                যত্ন
                {
                    ((পূর্ণাঙ্ক*)ptr)[0] = 100;
                }
                গৃহাণ (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = সঁচা;
                }
                যদি (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, রিক্ত, রিক্ত))
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
            গৃহাণ (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                ক্ষিপ;
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
        গৃহাণ (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            যত্ন
            {
                // Try HSA extension
                String extensions = device.extensions();
                যদি (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    গচ্ছ noSVM;
                }
                cl_platform_id p = রিক্ত;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, পরিমাণ(cl_platform_id), &p, রিক্ত));
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
            গৃহাণ (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                গচ্ছ noSVM;
            }
        }

        svmAvailable = সঁচা;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = সঁচা;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        ঘূৰাই;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = মিছা;
        svmEnabled = মিছা;
        svmCapabilities.মান_2 = 0;
        svmInitialized = সঁচা;
        svmFunctions.fn_clSVMAlloc = রিক্ত;
        ঘূৰাই;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ধ্রুৱ
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        ঘূৰাই *bufferPoolSVM_.get();
    }
#endif

    friend শ্ৰেণী Program;
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
Context::Context(পূর্ণাঙ্ক dtype)
{
    p = 0;
    create(dtype);
}

শূন্য Context::release()
{
    যদি (p)
    {
        p->release();
        p = রিক্ত;
    }
}

তার্কিক Context::create()
{
    release();
    যদি (!haveOpenCL())
        ঘূৰাই মিছা;
    p = Impl::findOrCreateContext(std::string());
    যদি (p && p->handle)
        ঘূৰাই সঁচা;
    release();
    ঘূৰাই মিছা;
}

// deprecated
তার্কিক Context::create(পূর্ণাঙ্ক dtype)
{
    যদি( !haveOpenCL() )
        ঘূৰাই মিছা;
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
        CV_LOG_ERROR(রিক্ত, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    যদি (p && !p->handle)
    {
        release();
    }
    ঘূৰাই p != 0;
}

Context::Context(ধ্রুৱ Context& c)
{
    p = (Impl*)c.p;
    যদি(p)
        p->addref();
}

Context& Context::operator = (ধ্রুৱ Context& c)
{
    Impl* newp = (Impl*)c.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    যদি (এতত্ != &c) {
        যদি(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

শূন্য* Context::ptr() ধ্রুৱ
{
    ঘূৰাই p == রিক্ত ? রিক্ত : p->handle;
}

size_t Context::ndevices() ধ্রুৱ
{
    ঘূৰাই p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ধ্রুৱ
{
    স্থির Device dummy;
    ঘূৰাই !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        auto& ctx = c.getContext();
        ঘূৰাই ctx;
    }

    স্থির Context dummy;
    ঘূৰাই dummy;
}

Program Context::getProg(ধ্রুৱ ProgramSource& prog,
                         ধ্রুৱ String& buildopts, String& errmsg)
{
    ঘূৰাই p ? p->getProg(prog, buildopts, errmsg) : Program();
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
    ঘূৰাই ctx;
}

/* static */
Context Context::fromDevice(ধ্রুৱ ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    ঘূৰাই ctx;
}

/* static */
Context Context::create(ধ্রুৱ std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    ঘূৰাই ctx;
}

শূন্য* Context::getOpenCLContextProperty(পূর্ণাঙ্ক propertyId) ধ্রুৱ
{
    যদি (p == রিক্ত)
        ঘূৰাই nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, রিক্ত, &size));
    std::vector<cl_context_properties> prop(size / পরিমাণ(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), রিক্ত));
    বাবে (size_t i = 0; i < prop.size(); i += 2)
    {
        যদি (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(রিক্ত, "OpenCL: found context property=" << propertyId << ") => " << (শূন্য*)prop[i + 1]);
            ঘূৰাই (শূন্য*)prop[i + 1];
        }
    }
    ঘূৰাই nullptr;
}

#ifdef HAVE_OPENCL_SVM
তার্কিক Context::useSVM() ধ্রুৱ
{
    Context::Impl* i = p;
    CV_Assert(i);
    যদি (!i->svmInitialized)
        i->svmInit();
    ঘূৰাই i->svmEnabled;
}
শূন্য Context::setUseSVM(তার্কিক enabled)
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
তার্কিক Context::useSVM() ধ্রুৱ { ঘূৰাই মিছা; }
শূন্য Context::setUseSVM(তার্কিক enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
নামাকাশ svm {

ধ্রুৱ SVMCapabilities getSVMCapabilitites(ধ্রুৱ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    যদি (!i->svmInitialized)
        i->svmInit();
    ঘূৰাই i->svmCapabilities;
}

CV_EXPORTS ধ্রুৱ SVMFunctions* getSVMFunctions(ধ্রুৱ ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != রিক্ত);
    ঘূৰাই &i->svmFunctions;
}

CV_EXPORTS তার্কিক useSVM(UMatUsageFlags usageFlags)
{
    যদি (checkForceSVMUmatUsage())
        ঘূৰাই সঁচা;
    যদি (checkDisableSVMUMatUsage())
        ঘূৰাই মিছা;
    যদি ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        ঘূৰাই সঁচা;
    ঘূৰাই মিছা; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

শূন্য Context::setUserContext(std::type_index typeId, ধ্রুৱ std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    ঘূৰাই p->getUserContext(typeId);
}

স্থির শূন্য get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<ৱর্ণ> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
শূন্য attachContext(ধ্রুৱ String& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ধ্রুৱ std::string& platformName, শূন্য* platformID, শূন্য* context, শূন্য* deviceID
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

    তার্কিক platformAvailable = মিছা;

    // check if external platformName contained in list of available platforms in OpenCV
    বাবে (unsigned পূর্ণাঙ্ক i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        যদি (platformName == availablePlatformName)
        {
            platformAvailable = সঁচা;
            ভঙ্গ;
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
    ঘূৰাই ctx;
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

সংরচনা Queue::Impl
{
    অন্তঃস্থ শূন্য __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = মিছা;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, পরিমাণ(cl_command_queue_properties), &props, রিক্ত));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, তার্কিক isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ধ্রুৱ Context& c, ধ্রুৱ Device& d, তার্কিক withProfiling = মিছা)
    {
        __init();

        ধ্রুৱ Context* pc = &c;
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
                handle = রিক্ত;
            }
        }
    }

    ধ্রুৱ cv::ocl::Queue& getProfilingQueue(ধ্রুৱ cv::ocl::Queue& self)
    {
        যদি (isProfilingQueue_)
            ঘূৰাই self;

        যদি (profiling_queue_.ptr())
            ঘূৰাই profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, পরিমাণ(cl_context), &ctx, রিক্ত));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, পরিমাণ(cl_device_id), &device, রিক্ত));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = নূতন Impl(q, সঁচা);
        profiling_queue_ = queue;

        ঘূৰাই profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    তার্কিক isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ধ্রুৱ Context& c, ধ্রুৱ Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ধ্রুৱ Queue& q)
{
    p = q.p;
    যদি(p)
        p->addref();
}

Queue& Queue::operator = (ধ্রুৱ Queue& q)
{
    Impl* newp = (Impl*)q.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    যদি (এতত্ != &q) {
        যদি(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

Queue::~Queue()
{
    যদি(p)
        p->release();
}

তার্কিক Queue::create(ধ্রুৱ Context& c, ধ্রুৱ Device& d)
{
    যদি(p)
        p->release();
    p = নূতন Impl(c, d);
    ঘূৰাই p->handle != 0;
}

শূন্য Queue::finish()
{
    যদি(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ধ্রুৱ Queue& Queue::getProfilingQueue() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->getProfilingQueue(*এতত্);
}

শূন্য* Queue::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    যদি (!c.empty())
    {
        auto& q = c.getQueue();
        ঘূৰাই q;
    }
    স্থির Queue dummy;
    ঘূৰাই dummy;
}

স্থির cl_command_queue getQueue(ধ্রুৱ Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    যদি(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    ঘূৰাই qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(পূর্ণাঙ্ক _flags, UMat* _m, পূর্ণাঙ্ক _wscale, পূর্ণাঙ্ক _iwscale, ধ্রুৱ শূন্য* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != রিক্ত);
}

KernelArg KernelArg::Constant(ধ্রুৱ Mat& m)
{
    CV_Assert(m.isContinuous());
    ঘূৰাই KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

সংরচনা Kernel::Impl
{
    Impl(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ Program& prog) :
        refcount(1), handle(রিক্ত), isInProgress(মিছা), isAsyncRun(মিছা), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        যদি (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        বাবে( পূর্ণাঙ্ক i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = মিছা;
        haveTempSrcUMats = মিছা;
    }

    শূন্য cleanupUMats()
    {
        তার্কিক exceptionOccurred = মিছা;
        বাবে( পূর্ণাঙ্ক i = 0; i < MAX_ARRS; i++ )
        {
            যদি( u[i] )
            {
                যদি( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    যত্ন
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    গৃহাণ(ধ্রুৱ std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(রিক্ত, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = সঁচা;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = মিছা;
        haveTempSrcUMats = মিছা;
        CV_Assert(!exceptionOccurred);
    }

    শূন্য addUMat(ধ্রুৱ UMat& m, তার্কিক dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        যদি(dst && m.u->tempUMat())
            haveTempDstUMats = সঁচা;
        যদি(m.u->originalUMatData == রিক্ত && m.u->tempUMat())
            haveTempSrcUMats = সঁচা;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    শূন্য registerImageArgument(পূর্ণাঙ্ক arg, ধ্রুৱ Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        যদি (arg < (পূর্ণাঙ্ক)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
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
        isInProgress = মিছা;
        যত্ন
        {
            cleanupUMats();
        }
        গৃহাণ(...)
        {
            release();
            ক্ষিপ;
        }
        release();
    }

    তার্কিক run(পূর্ণাঙ্ক dims, size_t _globalsize[], size_t _localsize[],
            তার্কিক সমকাল, int64* timeNS, ধ্রুৱ Queue& q);

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
    UMatData* u[MAX_ARRS];
    তার্কিক isInProgress;
    তার্কিক isAsyncRun;  // true if kernel was scheduled in async mode
    পূর্ণাঙ্ক nu;
    std::vector<Image2D> shadow_images;
    তার্কিক haveTempDstUMats;
    তার্কিক haveTempSrcUMats;
};

}} // namespace cv::ocl

বাহ্য "C" {

স্থির শূন্য CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, শূন্য *p)
{
    যত্ন
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    গৃহাণ (ধ্রুৱ cv::Exception& exc)
    {
        CV_LOG_ERROR(রিক্ত, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    গৃহাণ (ধ্রুৱ std::exception& exc)
    {
        CV_LOG_ERROR(রিক্ত, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    গৃহাণ (...)
    {
        CV_LOG_ERROR(রিক্ত, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

নামাকাশ cv { নামাকাশ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ ProgramSource& src,
               ধ্রুৱ String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ধ্রুৱ Kernel& k)
{
    p = k.p;
    যদি(p)
        p->addref();
}

Kernel& Kernel::operator = (ধ্রুৱ Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    যদি (এতত্ != &k) {
        যদি(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

Kernel::~Kernel()
{
    যদি(p)
        p->release();
}

তার্কিক Kernel::create(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ Program& prog)
{
    যদি(p)
        p->release();
    p = নূতন Impl(kname, prog);
    যদি(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    ঘূৰাই p != 0;
}

তার্কিক Kernel::create(ধ্রুৱ ৱর্ণ* kname, ধ্রুৱ ProgramSource& src,
                    ধ্রুৱ String& buildopts, String* errmsg)
{
    যদি(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    যদি( !errmsg ) errmsg = &tempmsg;
    ধ্রুৱ Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    ঘূৰাই create(kname, prog);
}

শূন্য* Kernel::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

তার্কিক Kernel::empty() ধ্রুৱ
{
    ঘূৰাই ptr() == 0;
}

স্থির cv::String dump_মান(size_t sz, ধ্রুৱ শূন্য* p)
{
    যদি (!p)
        ঘূৰাই "NULL";
    যদি (sz == 2)
        ঘূৰাই cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    যদি (sz == 4)
        ঘূৰাই cv::format("%d / %uu / 0x%08x / %g", *(পূর্ণাঙ্ক*)p, *(পূর্ণাঙ্ক*)p, *(পূর্ণাঙ্ক*)p, *(ভিন্ন*)p);
    যদি (sz == 8)
        ঘূৰাই cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(দ্ৱিগুণ*)p);
    ঘূৰাই cv::format("%p", p);
}

পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ শূন্য* value, size_t sz)
{
    যদি (!p || !p->handle)
        ঘূৰাই -1;
    যদি (i < 0)
        ঘূৰাই i;
    যদি( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (পূর্ণাঙ্ক)i, (পূর্ণাঙ্ক)sz, dump_মান(sz, value).c_str()).c_str());
    যদি (retval != CL_SUCCESS)
        ঘূৰাই -1;
    ঘূৰাই i+1;
}

পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    পূর্ণাঙ্ক res = set(i, &h, পরিমাণ(h));
    যদি (res >= 0)
        p->registerImageArgument(i, image2D);
    ঘূৰাই res;
}

পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ UMat& m)
{
    ঘূৰাই set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
স্থির তার্কিক isCudaBacked(ধ্রুৱ UMatData* u)
{
    ধ্রুৱ MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    ঘূৰাই u && cudaAllocator && u->currAllocator == cudaAllocator;
}

পূর্ণাঙ্ক Kernel::set(পূর্ণাঙ্ক i, ধ্রুৱ KernelArg& arg)
{
    যদি( !p || !p->handle )
        ঘূৰাই -1;
    যদি (i < 0)
    {
        CV_LOG_ERROR(রিক্ত, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (পূর্ণাঙ্ক)i));
        ঘূৰাই i;
    }
    যদি( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    যদি( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        তার্কিক ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        যদি (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)রিক্ত;
            status = clSetKernelArg(p->handle, (cl_uint)i, পরিমাণ(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (পূর্ণাঙ্ক)i).c_str());
            ঘূৰাই i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        যদি (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(রিক্ত, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (পূর্ণাঙ্ক)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            ঘূৰাই -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        যদি (!h)
        {
            CV_LOG_ERROR(রিক্ত, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (পূর্ণাঙ্ক)i, (পূর্ণাঙ্ক)arg.flags, arg.m));
            p->release();
            p = 0;
            ঘূৰাই -1;
        }

#ifdef HAVE_OPENCL_SVM
        যদি ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ধ্রুৱ Context& ctx = Context::getDefault();
            ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#যদি 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#অন্যথা
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (পূর্ণাঙ্ক)i, (শূন্য*)svmDataPtr).c_str());
        }
        অন্যথা
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, পরিমাণ(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (পূর্ণাঙ্ক)i, (শূন্য*)h).c_str());
        }

        যদি (ptronly)
        {
            i++;
        }
        অন্যথা যদি( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), পরিমাণ(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+1), (পূর্ণাঙ্ক)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), পরিমাণ(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+2), (পূর্ণাঙ্ক)u2d.offset).c_str());
            i += 3;

            যদি( !(arg.flags & KernelArg::NO_SIZE) )
            {
                পূর্ণাঙ্ক cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, পরিমাণ(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)i, (পূর্ণাঙ্ক)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), পরিমাণ(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+1), (পূর্ণাঙ্ক)cols).c_str());
                i += 2;
            }
        }
        অন্যথা
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), পরিমাণ(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+1), (পূর্ণাঙ্ক)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), পরিমাণ(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+2), (পূর্ণাঙ্ক)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), পরিমাণ(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+3), (পূর্ণাঙ্ক)u3d.offset).c_str());
            i += 4;
            যদি( !(arg.flags & KernelArg::NO_SIZE) )
            {
                পূর্ণাঙ্ক cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, পরিমাণ(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)i, (পূর্ণাঙ্ক)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), পরিমাণ(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+1), (পূর্ণাঙ্ক)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), পরিমাণ(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (পূর্ণাঙ্ক)(i+2), (পূর্ণাঙ্ক)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        ঘূৰাই i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (পূর্ণাঙ্ক)i, (পূর্ণাঙ্ক)arg.sz, (শূন্য*)arg.obj).c_str());
    ঘূৰাই i+1;
}

তার্কিক Kernel::run(পূর্ণাঙ্ক dims, size_t _globalsize[], size_t _localsize[],
                 তার্কিক সমকাল, ধ্রুৱ Queue& q)
{
    যদি (!p)
        ঘূৰাই মিছা;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != রিক্ত);
    বাবে (পূর্ণাঙ্ক i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(পূর্ণাঙ্ক)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        যদি (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned পূর্ণাঙ্ক)val) * val;
    }
    CV_Assert(total > 0);

    ঘূৰাই p->run(dims, globalsize, _localsize, সমকাল, রিক্ত, q);
}


তার্কিক Kernel::run_(পূর্ণাঙ্ক dims, size_t _globalsize[], size_t _localsize[],
                  তার্কিক সমকাল, ধ্রুৱ Queue& q)
{
    CV_Assert(p);
    ঘূৰাই p->run(dims, _globalsize, _localsize, সমকাল, রিক্ত, q);
}


তার্কিক Kernel::Impl::run(পূর্ণাঙ্ক dims, size_t globalsize[], size_t localsize[],
        তার্কিক সমকাল, int64* timeNS, ধ্রুৱ Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    যদি (!handle)
    {
        CV_LOG_ERROR(রিক্ত, "OpenCL kernel has zero handle: " << name);
        ঘূৰাই মিছা;
    }

    যদি (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !সমকাল;

    যদি (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#যদি CV_OPENCL_SYNC_RUN_KERNELS
    সমকাল = সঁচা;
#endif

    cl_command_queue qq = getQueue(q);
    যদি (haveTempDstUMats)
        সমকাল = সঁচা;
    যদি (haveTempSrcUMats)
        সমকাল = সঁচা;
    যদি (timeNS)
        সমকাল = সঁচা;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           রিক্ত, globalsize, localsize, 0, 0,
                                           (সমকাল && !timeNS) ? 0 : &asyncEvent);
#যদি !CV_OPENCL_SHOW_RUN_KERNELS
    যদি (retval != CL_SUCCESS)
#endif
    {
        cv::String বাৰ্তা = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (পূর্ণাঙ্ক)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        সমকাল ? "true" : "false"
                        );
        যদি (retval != CL_SUCCESS)
        {
            বাৰ্তা = CV_OCL_API_ERROR_বাৰ্তা(retval, বাৰ্তা.c_str());
        }
#যদি CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, বাৰ্তা.c_str());
#অন্যথা
        printf("%s\n", বাৰ্তা.c_str());
        fflush(stdout);
#endif
    }
    যদি (সমকাল || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        যদি (timeNS)
        {
            যদি (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, পরিমাণ(startTime), &startTime, রিক্ত));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, পরিমাণ(stopTime), &stopTime, রিক্ত));
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
        isInProgress = সঁচা;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, এতত্));
    }
    যদি (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ঘূৰাই retval == CL_SUCCESS;
}

তার্কিক Kernel::runTask(তার্কিক সমকাল, ধ্রুৱ Queue& q)
{
    যদি(!p || !p->handle || p->isInProgress)
        ঘূৰাই মিছা;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, সমকাল ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), সমকাল ? "true" : "false").c_str());
    যদি (সমকাল || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    অন্যথা
    {
        p->addref();
        p->isInProgress = সঁচা;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    যদি (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    ঘূৰাই retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(পূর্ণাঙ্ক dims, size_t globalsize[], size_t localsize[], ধ্রুৱ Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    তার্কিক res = p->run(dims, globalsize, localsize, সঁচা, &timeNs, profilingQueue);
    ঘূৰাই res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ধ্রুৱ
{
    যদি(!p || !p->handle)
        ঘূৰাই 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, পরিমাণ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    ঘূৰাই status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ধ্রুৱ
{
    যদি(!p || !p->handle)
        ঘূৰাই 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, পরিমাণ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    ঘূৰাই status == CL_SUCCESS ? val : 0;
}

তার্কিক Kernel::compileWorkGroupSize(size_t wsz[]) ধ্রুৱ
{
    যদি(!p || !p->handle || !wsz)
        ঘূৰাই 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, পরিমাণ(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    ঘূৰাই status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ধ্রুৱ
{
    যদি(!p || !p->handle)
        ঘূৰাই 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, পরিমাণ(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    ঘূৰাই status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

সংরচনা ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    গণনা KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ধ্রুৱ String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ধ্রুৱ String& module, ধ্রুৱ String& name, ধ্রুৱ String& codeStr, ধ্রুৱ String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    শূন্য init(গণনা KIND kind, ধ্রুৱ String& module, ধ্রুৱ String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = রিক্ত;
        sourceSize_ = 0;
        isHashUpdated = মিছা;
    }

    শূন্য initFromSource(ধ্রুৱ String& codeStr, ধ্রুৱ String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        যদি (sourceHash_.empty())
        {
            updateHash();
        }
        অন্যথা
        {
            isHashUpdated = সঁচা;
        }
    }

    শূন্য updateHash(ধ্রুৱ ৱর্ণ* hashStr = রিক্ত)
    {
        যদি (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = সঁচা;
            ঘূৰাই;
        }
        uint64 hash = 0;
        ৱিকল্প (kind_)
        {
        পক্ষ PROGRAM_SOURCE_CODE:
            যদি (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            অন্যথা
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            ভঙ্গ;
        পক্ষ PROGRAM_BINARIES:
        পক্ষ PROGRAM_SPIR:
        পক্ষ PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            ভঙ্গ;
        স্ৱভাৱ:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = সঁচা;
    }

    Impl(গণনা KIND kind,
            ধ্রুৱ String& module, ধ্রুৱ String& name,
            ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size,
            ধ্রুৱ cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    স্থির ProgramSource fromSourceWithStaticLifetime(ধ্রুৱ String& module, ধ্রুৱ String& name,
            ধ্রুৱ ৱর্ণ* sourceCodeStaticStr, ধ্রুৱ ৱর্ণ* hashStaticStr,
            ধ্রুৱ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = নূতন Impl(PROGRAM_SOURCE_CODE, module, name,
                (ধ্রুৱ unsigned ৱর্ণ*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        ঘূৰাই result;
    }

    স্থির ProgramSource from_দ্বিমিক(ধ্রুৱ String& module, ধ্রুৱ String& name,
            ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size,
            ধ্রুৱ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = নূতন Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        ঘূৰাই result;
    }

    স্থির ProgramSource fromSPIR(ধ্রুৱ String& module, ধ্রুৱ String& name,
            ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size,
            ধ্রুৱ cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = নূতন Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        ঘূৰাই result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ধ্রুৱ unsigned ৱর্ণ* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    তার্কিক isHashUpdated;

    friend সংরচনা Program::Impl;
    friend সংরচনা internal::ProgramEntry;
    friend সংরচনা Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ধ্রুৱ String& module, ধ্রুৱ String& name, ধ্রুৱ String& codeStr, ধ্রুৱ String& codeHash)
{
    p = নূতন Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ধ্রুৱ ৱর্ণ* prog)
{
    p = নূতন Impl(prog);
}

ProgramSource::ProgramSource(ধ্রুৱ String& prog)
{
    p = নূতন Impl(prog);
}

ProgramSource::~ProgramSource()
{
    যদি(p)
        p->release();
}

ProgramSource::ProgramSource(ধ্রুৱ ProgramSource& prog)
{
    p = prog.p;
    যদি(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ধ্রুৱ ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    যদি (এতত্ != &prog) {
        যদি(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

ধ্রুৱ String& ProgramSource::source() ধ্রুৱ
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == রিক্ত); // method returns reference - can't construct temporary object
    ঘূৰাই p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ধ্রুৱ
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_দ্বিমিক(ধ্রুৱ String& module, ধ্রুৱ String& name,
        ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size,
        ধ্রুৱ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ঘূৰাই Impl::from_দ্বিমিক(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ধ্রুৱ String& module, ধ্রুৱ String& name,
        ধ্রুৱ unsigned ৱর্ণ* binary, ধ্রুৱ size_t size,
        ধ্রুৱ cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    ঘূৰাই Impl::from_দ্বিমিক(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ধ্রুৱ
{
    যদি (এতত্->pProgramSource == রিক্ত)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        যদি (এতত্->pProgramSource == রিক্ত)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(এতত্->module, এতত্->name, এতত্->programCode, এতত্->programHash, cv::String());
            ProgramSource* ptr = নূতন ProgramSource(ps);
            const_cast<ProgramEntry*>(এতত্)->pProgramSource = ptr;
        }
    }
    ঘূৰাই *এতত্->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

স্থির
cv::String joinBuildOptions(ধ্রুৱ cv::String& a, ধ্রুৱ cv::String& b)
{
    যদি (b.empty())
        ঘূৰাই a;
    যদি (a.empty())
        ঘূৰাই b;
    যদি (b[0] == ' ')
        ঘূৰাই a + b;
    ঘূৰাই a + (cv::String(" ") + b);
}

সংরচনা Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ধ্রুৱ ProgramSource& src,
         ধ্রুৱ String& _buildflags, String& errmsg) :
         refcount(1),
         handle(রিক্ত),
         buildflags(_buildflags)
    {
        ধ্রুৱ ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ধ্রুৱ Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        যদি (ctx.ptr() == রিক্ত || device.ptr() == রিক্ত)
            ঘূৰাই;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        যদি (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            যদি (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            অন্যথা যদি (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ধ্রুৱ String param_buildExtraOptions = getBuildExtraOptions();
            যদি (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#যদি CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(রিক্ত, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#যদি CV_OPENCL_SHOW_BUILD_KERNELS
        যদি (handle)
        {
            size_t retsz = 0;
            ৱর্ণ kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, পরিমাণ(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            যদি (result == CL_SUCCESS && retsz < পরিমাণ(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(রিক্ত, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            অন্যথা
            {
                CV_LOG_ERROR(রিক্ত, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    তার্কিক compile(ধ্রুৱ Context& ctx, ধ্রুৱ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(রিক্ত, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            তার্কিক isLoaded = create_From_দ্বিমিক(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            ঘূৰাই isLoaded;
        }
        ঘূৰাই compileWithCache(ctx, src_, errmsg);
    }

    তার্কিক compileWithCache(ধ্রুৱ Context& ctx, ধ্রুৱ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ধ্রুৱ std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ধ্রুৱ String& hash_str = src_->sourceHash_;
        cv::String fname;
        যদি (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ধ্রুৱ cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        যদি (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            যত্ন
            {
                std::vector<ৱর্ণ> দ্বিমিক_Buf;
                তার্কিক res = মিছা;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    দ্বিমিক_Program_File file(fname, hash_str.c_str());
                    res = file.পঠ(buildflags, দ্বিমিক_Buf);
                }
                যদি (res)
                {
                    CV_Assert(!দ্বিমিক_Buf.empty());
                    CV_LOG_VERBOSE(রিক্ত, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    তার্কিক isLoaded = create_From_দ্বিমিক(ctx, দ্বিমিক_Buf, errmsg);
                    যদি (isLoaded)
                        ঘূৰাই সঁচা;
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(রিক্ত, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            গৃহাণ (...)
            {
                CV_LOG_VERBOSE(রিক্ত, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == রিক্ত);
        যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            যদি (!buildFromSources(ctx, src_, errmsg))
            {
                ঘূৰাই মিছা;
            }
        }
        অন্যথা যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            যদি ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(রিক্ত, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            তার্কিক isLoaded = create_From_দ্বিমিক(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            যদি (!isLoaded)
                ঘূৰাই মিছা;
        }
        অন্যথা যদি (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        অন্যথা
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != রিক্ত);
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
        যদি (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            যত্ন
            {
                std::vector<ৱর্ণ> দ্বিমিক_Buf;
                get_Program_দ্বিমিক(দ্বিমিক_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    দ্বিমিক_Program_File file(fname, hash_str.c_str());
                    file.লিখ(buildflags, দ্বিমিক_Buf);
                }
            }
            গৃহাণ (ধ্রুৱ cv::Exception& e)
            {
                CV_LOG_WARNING(রিক্ত, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            গৃহাণ (...)
            {
                CV_LOG_WARNING(রিক্ত, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
        যদি (CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান)
        {
            std::vector<ৱর্ণ> দ্বিমিক_Buf;
            get_Program_দ্বিমিক(দ্বিমিক_Buf);
            যদি (!দ্বিমিক_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = রিক্ত;
                create_From_দ্বিমিক(ctx, দ্বিমিক_Buf, errmsg);
            }
        }
#endif
        ঘূৰাই handle != রিক্ত;
    }

    শূন্য dumpBuildLog_(cl_int result, ধ্রুৱ cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<ৱর্ণ, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        যদি (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            যদি (log_retval == CL_SUCCESS)
            {
                যদি (retsz < buffer.size())
                    buffer[retsz] = 0;
                অন্যথা
                    buffer[buffer.size() - 1] = 0;
            }
            অন্যথা
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

    তার্কিক buildFromSources(ধ্রুৱ Context& ctx, ধ্রুৱ ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == রিক্ত);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(রিক্ত, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ধ্রুৱ ৱর্ণ* srcptr = src_->sourceAddr_ ? ((ধ্রুৱ ৱর্ণ*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != রিক্ত);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        যদি (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            বাবে (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
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
                    handle = রিক্ত;
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
                CV_LOG_INFO(রিক্ত, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                ৱর্ণ kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, পরিমাণ(kernels_buffer), &kernels_buffer[0], &retsz);
                যদি (retsz < পরিমাণ(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                অন্যথা
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(রিক্ত, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        ঘূৰাই handle != রিক্ত;
    }

    শূন্য get_Program_দ্বিমিক(std::vector<ৱর্ণ>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_দ্বিমিক_SIZES, পরিমাণ(sz), &sz, রিক্ত));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, পরিমাণ(ptr), &ptr, রিক্ত));
    }

    তার্কিক create_From_দ্বিমিক(ধ্রুৱ Context& ctx, ধ্রুৱ std::vector<ৱর্ণ>& buf, String& errmsg)
    {
        ঘূৰাই create_From_দ্বিমিক(ctx, (ধ্রুৱ unsigned ৱর্ণ*)&buf[0], buf.size(), errmsg);
    }

    তার্কিক create_From_দ্বিমিক(ধ্রুৱ Context& ctx, ধ্রুৱ unsigned ৱর্ণ* দ্বিমিক_Addr, ধ্রুৱ size_t দ্বিমিক_Size, String& errmsg)
    {
        CV_Assert(handle == রিক্ত);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(রিক্ত, 0, "Load from binary... (" << দ্বিমিক_Size << " bytes)");

        CV_Assert(দ্বিমিক_Size > 0);

        size_t ndevices = (পূর্ণাঙ্ক)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ধ্রুৱ uchar*> দ্বিমিক_Ptrs(ndevices);
        AutoBuffer<size_t> দ্বিমিক_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ধ্রুৱ uchar** দ্বিমিক_Ptrs_2 = দ্বিমিক_Ptrs.data();
        size_t* দ্বিমিক_Sizes_2 = দ্বিমিক_Sizes.data();
        বাবে (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            দ্বিমিক_Ptrs_2[i] = দ্বিমিক_Addr;
            দ্বিমিক_Sizes_2[i] = দ্বিমিক_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_দ্বিমিক((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           দ্বিমিক_Sizes_2, দ্বিমিক_Ptrs_2, রিক্ত, &result);
        যদি (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(রিক্ত, CV_OCL_API_ERROR_বাৰ্তা(result, "clCreateProgramWithBinary"));
            যদি (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = রিক্ত;
            }
        }
        যদি (!handle)
        {
            ঘূৰাই মিছা;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            যদি (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                যদি (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = রিক্ত;
                }
                ঘূৰাই মিছা;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    পরিমাণ(build_status), &build_status, &retsz));
            যদি (result == CL_SUCCESS)
            {
                যদি (build_status == CL_BUILD_SUCCESS)
                {
                    ঘূৰাই সঁচা;
                }
                অন্যথা
                {
                    CV_LOG_WARNING(রিক্ত, "clGetProgramBuildInfo() returns " << build_status);
                    ঘূৰাই মিছা;
                }
            }
            অন্যথা
            {
                CV_LOG_ERROR(রিক্ত, CV_OCL_API_ERROR_বাৰ্তা(result, "clGetProgramBuildInfo()"));
                যদি (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = রিক্ত;
                }
            }
        }
#যদি CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS
        যদি (handle && CV_OPENCL_VALIDATE_দ্বিমিক_PROGRAMS_মান)
        {
            CV_LOG_INFO(রিক্ত, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            ৱর্ণ kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, পরিমাণ(kernels_buffer), &kernels_buffer[0], &retsz);
            যদি (retsz < পরিমাণ(kernels_buffer))
                kernels_buffer[retsz] = 0;
            অন্যথা
                kernels_buffer[0] = 0;
            CV_LOG_INFO(রিক্ত, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        ঘূৰাই handle != রিক্ত;
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
            handle = রিক্ত;
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

Program::Program(ধ্রুৱ ProgramSource& src,
        ধ্রুৱ String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ধ্রুৱ Program& prog)
{
    p = prog.p;
    যদি(p)
        p->addref();
}

Program& Program::operator = (ধ্রুৱ Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    যদি(newp)
        newp->addref();
    যদি(p)
        p->release();
    p = newp;
    ঘূৰাই *এতত্;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    যদি (এতত্ != &prog) {
        যদি(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

Program::~Program()
{
    যদি(p)
        p->release();
}

তার্কিক Program::create(ধ্রুৱ ProgramSource& src,
            ধ্রুৱ String& buildflags, String& errmsg)
{
    যদি(p)
    {
        p->release();
        p = রিক্ত;
    }
    p = নূতন Impl(src, buildflags, errmsg);
    যদি(!p->handle)
    {
        p->release();
        p = 0;
    }
    ঘূৰাই p != 0;
}

শূন্য* Program::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ধ্রুৱ ProgramSource& Program::source() ধ্রুৱ
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

তার্কিক Program::পঠ(ধ্রুৱ String& bin, ধ্রুৱ String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

তার্কিক Program::লিখ(String& bin) ধ্রুৱ
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ধ্রুৱ
{
    যদি(!p)
        ঘূৰাই String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    ঘূৰাই cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ধ্রুৱ String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        ঘূৰাই cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

শূন্য Program::get_দ্বিমিক(std::vector<ৱর্ণ>& binary) ধ্রুৱ
{
    CV_Assert(p && "Empty program");
    p->get_Program_দ্বিমিক(binary);
}

Program Context::Impl::getProg(ধ্রুৱ ProgramSource& src,
                               ধ্রুৱ String& buildflags, String& errmsg)
{
    size_t limit = get_Program_গুনতি_Limit();
    ধ্রুৱ ProgramSource::Impl* src_ = src.getImpl();
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
            ঘূৰাই it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            যদি (limit > 0 && sz >= limit)
            {
                স্থির তার্কিক warningFlag = মিছা;
                যদি (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = সঁচা;
                }
                যেতিয়ালৈ (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    যদি (c != 0)
                        ভঙ্গ;
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
    ঘূৰাই prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
শ্ৰেণী OpenCLBufferPool
{
রক্ষিত:
    ~OpenCLBufferPool() { }
সার্ৱজনিক:
    আভাস T allocate(size_t size) = 0;
    আভাস শূন্য release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
শ্ৰেণী OpenCLBufferPoolBaseImpl : সার্ৱজনিক BufferPoolController, সার্ৱজনিক OpenCLBufferPool<T>
{
গুপ্ত:
    অন্তঃস্থ Derived& derived() { ঘূৰাই *static_cast<Derived*>(এতত্); }
রক্ষিত:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    তার্কিক _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        বাবে (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            যদি (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                ঘূৰাই সঁচা;
            }
        }
        ঘূৰাই মিছা;
    }

    // synchronized
    তার্কিক _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ধ্রুৱ size_t size)
    {
        যদি (reservedEntries_.empty())
            ঘূৰাই মিছা;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        বাবে (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            যদি (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                যদি (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    যদি (diff == 0)
                        ভঙ্গ;
                }
            }
        }
        যদি (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            ঘূৰাই সঁচা;
        }
        ঘূৰাই মিছা;
    }

    // synchronized
    শূন্য _checkSizeOfReservedEntries()
    {
        যেতিয়ালৈ (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ধ্রুৱ BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    অন্তঃস্থ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        যদি (size < 1024*1024)
            ঘূৰাই 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        অন্যথা যদি (size < 16*1024*1024)
            ঘূৰাই 64*1024;
        অন্যথা
            ঘূৰাই 1024*1024;
    }

সার্ৱজনিক:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    আভাস ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
সার্ৱজনিক:
    আভাস T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        যদি (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        অন্যথা
        {
            derived()._allocateBufferEntry(entry, size);
        }
        ঘূৰাই entry.clBuffer_;
    }
    আভাস শূন্য release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        যদি (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        অন্যথা
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    আভাস size_t getReservedSize() ধ্রুৱ CV_OVERRIDE { ঘূৰাই currentReservedSize; }
    আভাস size_t getMaxReservedSize() ধ্রুৱ CV_OVERRIDE { ঘূৰাই maxReservedSize; }
    আভাস শূন্য setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        যদি (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            বাবে (; i != reservedEntries_.end();)
            {
                ধ্রুৱ BufferEntry& entry = *i;
                যদি (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    অনুৱর্ত;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    আভাস শূন্য freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        বাবে (; i != reservedEntries_.end(); ++i)
        {
            ধ্রুৱ BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

সংরচনা CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)রিক্ত), capacity_(0) { }
};

শ্ৰেণী OpenCLBufferPoolImpl CV_FINAL : সার্ৱজনিক OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
সার্ৱজনিক:
    প্রকারনির্দেশ সংরচনা CLBufferEntry BufferEntry;
রক্ষিত:
    পূর্ণাঙ্ক createFlags_;
সার্ৱজনিক:
    OpenCLBufferPoolImpl(পূর্ণাঙ্ক createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    শূন্য _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == রিক্ত);
        entry.capacity_ = alignSize(size, (পূর্ণাঙ্ক)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long পূর্ণাঙ্ক)entry.capacity_, (শূন্য*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != রিক্ত);
        যদি(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    শূন্য _releaseBufferEntry(ধ্রুৱ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != রিক্ত);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
সংরচনা CLSVMBufferEntry
{
    শূন্য* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(রিক্ত), capacity_(0) { }
};
শ্ৰেণী OpenCLSVMBufferPoolImpl CV_FINAL : সার্ৱজনিক OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, শূন্য*>
{
সার্ৱজনিক:
    প্রকারনির্দেশ সংরচনা CLSVMBufferEntry BufferEntry;
সার্ৱজনিক:
    OpenCLSVMBufferPoolImpl()
    {
    }

    শূন্য _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == রিক্ত);
        entry.capacity_ = alignSize(size, (পূর্ণাঙ্ক)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ধ্রুৱ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        তার্কিক isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (পূর্ণাঙ্ক)entry.capacity_);
        শূন্য *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    শূন্য _releaseBufferEntry(ধ্রুৱ BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != রিক্ত);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <তার্কিক readAccess, তার্কিক writeAccess>
শ্ৰেণী AlignedDataPtr
{
রক্ষিত:
    ধ্রুৱ size_t size_;
    uchar* ধ্রুৱ originPtr_;
    ধ্রুৱ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

সার্ৱজনিক:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(রিক্ত)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        যদি (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = নূতন uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            যদি (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ধ্রুৱ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ঘূৰাই ptr_;
    }

    ~AlignedDataPtr()
    {
        যদি (allocatedPtr_)
        {
            যদি (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = রিক্ত;
        }
        ptr_ = রিক্ত;
    }
গুপ্ত:
    AlignedDataPtr(ধ্রুৱ AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ধ্রুৱ AlignedDataPtr&); // disabled
};

template <তার্কিক readAccess, তার্কিক writeAccess>
শ্ৰেণী AlignedDataPtr2D
{
রক্ষিত:
    ধ্রুৱ size_t size_;
    uchar* ধ্রুৱ originPtr_;
    ধ্রুৱ size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

সার্ৱজনিক:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(রিক্ত), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != রিক্ত);
        যদি (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = নূতন uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            যদি (readAccess)
            {
                বাবে (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ধ্রুৱ
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        ঘূৰাই ptr_;
    }

    ~AlignedDataPtr2D()
    {
        যদি (allocatedPtr_)
        {
            যদি (writeAccess)
            {
                বাবে (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = রিক্ত;
        }
        ptr_ = রিক্ত;
    }
গুপ্ত:
    AlignedDataPtr2D(ধ্রুৱ AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ধ্রুৱ AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


শূন্য Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(রিক্ত, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

শ্ৰেণী OpenCLAllocator CV_FINAL : সার্ৱজনিক MatAllocator
{
সার্ৱজনিক:
    গণনা AllocatorFlags
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

    UMatData* defaultAllocate(পূর্ণাঙ্ক dims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type, শূন্য* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ধ্রুৱ
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        ঘূৰাই u;
    }

    স্থির তার্কিক isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        স্থির তার্কিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", মিছা);
        ঘূৰাই value;
    }
    স্থির তার্কিক isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        স্থির তার্কিক value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", মিছা);
        ঘূৰাই value;
    }

    শূন্য getBestFlags(ধ্রুৱ Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, পূর্ণাঙ্ক& createFlags, UMatData::MemoryFlag& flags0) ধ্রুৱ
    {
        ধ্রুৱ Device& dev = ctx.device(0);
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
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        অন্যথা
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(পূর্ণাঙ্ক dims, ধ্রুৱ পূর্ণাঙ্ক* sizes, পূর্ণাঙ্ক type,
                       শূন্য* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!useOpenCL())
            ঘূৰাই defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        বাবে( পূর্ণাঙ্ক i = dims-1; i >= 0; i-- )
        {
            যদি( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        যদি (!ctx.getImpl())
            ঘূৰাই defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        পূর্ণাঙ্ক createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        শূন্য* handle = রিক্ত;
        পূর্ণাঙ্ক allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ধ্রুৱ svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        যদি (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            তার্কিক isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        অন্যথা
#endif
        যদি (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        অন্যথা যদি (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        অন্যথা
        {
            CV_Assert(handle != রিক্ত); // Unsupported, throw
        }

        যদি (!handle)
            ঘূৰাই defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = নূতন UMatData(এতত্);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<শূন্য>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(সঁচা);
        opencl_allocator_stats.onAllocate(u->size);
        ঘূৰাই u;
    }

    তার্কিক allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই মিছা;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        যদি(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            পূর্ণাঙ্ক createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            তার্কিক copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            পূর্ণাঙ্ক allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            শূন্য* handle = রিক্ত;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            তার্কিক useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            যদি (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (পূর্ণাঙ্ক)u->size, handle);
            }
            অন্যথা যদি (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                যদি (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    তার্কিক isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (পূর্ণাঙ্ক)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = রিক্ত;
                    যদি (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (পূর্ণাঙ্ক)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, রিক্ত, রিক্ত);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    যদি (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, রিক্ত, রিক্ত);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            অন্যথা
#endif
            {
                যদি( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                যদি (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (পূর্ণাঙ্ক)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long পূর্ণাঙ্ক)u->size, u->origdata, (শূন্য*)handle).c_str());
                }
                যদি((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long পূর্ণাঙ্ক)u->size, u->origdata, (শূন্য*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (শূন্য*)handle).c_str());
            যদি(!handle || retval != CL_SUCCESS)
                ঘূৰাই মিছা;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = এতত্;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(সঁচা);
        opencl_allocator_stats.onAllocate(u->size);
        ঘূৰাই সঁচা;
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

    শূন্য deallocate(UMatData* u) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        যদি (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        অন্যথা
            deallocate_(u);
    }

    শূন্য deallocate_(UMatData* u) ধ্রুৱ
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        যদি ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        যদি (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            ঘূৰাই;             // avoid any OpenCL calls
#endif
        যদি(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            যদি (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    যদি( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        তার্কিক isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = রিক্ত;
                        যদি (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণাঙ্ক)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, রিক্ত, রিক্ত);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        যদি (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, রিক্ত, রিক্ত);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    অন্যথা
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                অন্যথা
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    যদি( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<মিছা, সঁচা> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    অন্যথা
                    {
                        cl_int retval = 0;
                        যদি (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            শূন্য* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (শূন্য*)u->handle, (long long পূর্ণাঙ্ক)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            যদি (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (শূন্য*)u->handle, data, (long long পূর্ণাঙ্ক)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(মিছা);
            }
            অন্যথা
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                যদি( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            অন্যথা
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (শূন্য*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(সঁচা);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = রিক্ত;
            যদি(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = রিক্ত;
        }
        অন্যথা
        {
            CV_Assert(u->origdata == রিক্ত);
            যদি(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(সঁচা);
            }
            যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            অন্যথা যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            অন্যথা যদি (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                অন্যথা যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, রিক্ত, রিক্ত);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((শূন্য*)u->handle);
            }
#endif
            অন্যথা
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(সঁচা);
            delete u;
            u = রিক্ত;
        }
        CV_Assert(u == রিক্ত);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    শূন্য map(UMatData* u, AccessFlag accessFlags) ধ্রুৱ CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        যদি (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(সঁচা);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            যদি( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণাঙ্ক)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, রিক্ত, রিক্ত);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(মিছা);
                    u->markDeviceMemMapped(সঁচা);
                    ঘূৰাই;
                }
#endif

                cl_int retval = CL_SUCCESS;
                যদি (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (শূন্য*)u->handle, (long long পূর্ণাঙ্ক)u->size, u->data).c_str());
                }
                যদি (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(মিছা);
                    u->markDeviceMemMapped(সঁচা);
                    ঘূৰাই;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            যদি(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(সঁচা);
            }
        }

        যদি (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<মিছা, সঁচা> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (শূন্য*)u->handle, (long long পূর্ণাঙ্ক)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(মিছা);
        }
    }

    শূন্য unmap(UMatData* u) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        যদি( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != রিক্ত);
#ifdef HAVE_OPENCL_SVM
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, রিক্ত, রিক্ত);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                যদি (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(মিছা);
                u->markHostCopyObsolete(সঁচা);
                ঘূৰাই;
            }
#endif
            যদি (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (শূন্য*)u->handle, u->data, (long long পূর্ণাঙ্ক)u->size).c_str());
                যদি (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(মিছা);
                u->data = 0;
                u->markDeviceCopyObsolete(মিছা);
                u->markHostCopyObsolete(সঁচা);
            }
        }
        অন্যথা যদি( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<সঁচা, মিছা> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (শূন্য*)u->handle, (long long পূর্ণাঙ্ক)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(মিছা);
            u->markHostCopyObsolete(সঁচা);
        }
    }

    তার্কিক checkContinuous(পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
                         ধ্রুৱ size_t srcofs[], ধ্রুৱ size_t srcstep[],
                         ধ্রুৱ size_t dstofs[], ধ্রুৱ size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ধ্রুৱ
    {
        তার্কিক iscontinuous = সঁচা;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        বাবে( পূর্ণাঙ্ক i = dims-2; i >= 0; i-- )
        {
            যদি( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = মিছা;
            total *= sz[i];
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
        ঘূৰাই iscontinuous;
    }

    শূন্য download(UMatData* u, শূন্য* dstptr, পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
                  ধ্রুৱ size_t srcofs[], ধ্রুৱ size_t srcstep[],
                  ধ্রুৱ size_t dststep[]) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই;
        UMatDataAutoLock autolock(u);

        যদি( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            ঘূৰাই;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        তার্কিক iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == রিক্ত || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণাঙ্ক)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, রিক্ত, রিক্ত);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            যদি( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            অন্যথা
            {
                // This code is from MatAllocator::download()
                পূর্ণাঙ্ক isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    যদি( sz[i] == 0 )
                    ঘূৰাই;
                    যদি( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (পূর্ণাঙ্ক)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ধ্রুৱ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                বাবে( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, রিক্ত, রিক্ত);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                AlignedDataPtr<মিছা, সঁচা> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ধ্রুৱ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<মিছা, মিছা> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                বাবে( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            অন্যথা
            {
                AlignedDataPtr2D<মিছা, সঁচা> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    শূন্য upload(UMatData* u, ধ্রুৱ শূন্য* srcptr, পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
                ধ্রুৱ size_t dstofs[], ধ্রুৱ size_t dststep[],
                ধ্রুৱ size_t srcstep[]) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!u)
            ঘূৰাই;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        তার্কিক iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        যদি( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(মিছা);
            u->markDeviceCopyObsolete(সঁচা);
            ঘূৰাই;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == রিক্ত || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (পূর্ণাঙ্ক)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, রিক্ত, রিক্ত);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            যদি( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            অন্যথা
            {
                // This code is from MatAllocator::upload()
                পূর্ণাঙ্ক isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    যদি( sz[i] == 0 )
                    ঘূৰাই;
                    যদি( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (পূর্ণাঙ্ক)sz[i];
                }

                Mat src(dims, isz, CV_8U, (শূন্য*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ধ্রুৱ Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                বাবে( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, রিক্ত, রিক্ত);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                AlignedDataPtr<সঁচা, মিছা> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (শূন্য*)u->handle, (long long পূর্ণাঙ্ক)dstrawofs, (long long পূর্ণাঙ্ক)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ধ্রুৱ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<মিছা, মিছা> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                বাবে( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            অন্যথা
            {
                AlignedDataPtr2D<সঁচা, মিছা> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(সঁচা);
#ifdef HAVE_OPENCL_SVM
        যদি ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        অন্যথা
#endif
        {
            u->markHostCopyObsolete(সঁচা);
        }
        u->markDeviceCopyObsolete(মিছা);
    }

    শূন্য copy(UMatData* src, UMatData* dst, পূর্ণাঙ্ক dims, ধ্রুৱ size_t sz[],
              ধ্রুৱ size_t srcofs[], ধ্রুৱ size_t srcstep[],
              ধ্রুৱ size_t dstofs[], ধ্রুৱ size_t dststep[], তার্কিক _sync) ধ্রুৱ CV_OVERRIDE
    {
        যদি(!src || !dst)
            ঘূৰাই;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        তার্কিক iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        যদি( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            ঘূৰাই;
        }
        যদি( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(মিছা);
#ifdef HAVE_OPENCL_SVM
            যদি ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            অন্যথা
#endif
            {
                dst->markDeviceCopyObsolete(সঁচা);
            }
            ঘূৰাই;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ধ্রুৱ svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                যদি( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (পূর্ণাঙ্ক)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, রিক্ত, রিক্ত);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                অন্যথা
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    পূর্ণাঙ্ক isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        যদি( sz[i] == 0 )
                        ঘূৰাই;
                        যদি( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (পূর্ণাঙ্ক)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    বাবে( পূর্ণাঙ্ক i = 0; i < dims; i++ )
                    {
                        যদি( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ধ্রুৱ Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    বাবে( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            অন্যথা
            {
                যদি ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                অন্যথা
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        অন্যথা
#endif
        {
            যদি( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (শূন্য*)src->handle, (শূন্য*)dst->handle, (long long পূর্ণাঙ্ক)srcrawofs, (long long পূর্ণাঙ্ক)dstrawofs, (long long পূর্ণাঙ্ক)total).c_str());
            }
            অন্যথা যদি (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ধ্রুৱ size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<মিছা, মিছা> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<মিছা, মিছা> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                বাবে( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            অন্যথা
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
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
        যদি ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        অন্যথা
#endif
        {
            dst->markHostCopyObsolete(সঁচা);
        }
        dst->markDeviceCopyObsolete(মিছা);

        যদি( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ধ্রুৱ ৱর্ণ* id) ধ্রুৱ CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        যদি (ctx.empty())
            ঘূৰাই রিক্ত;
#ifdef HAVE_OPENCL_SVM
        যদি ((svm::checkForceSVMUmatUsage() && (id == রিক্ত || strcmp(id, "OCL") == 0)) || (id != রিক্ত && strcmp(id, "SVM") == 0))
        {
            ঘূৰাই &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        যদি (id != রিক্ত && strcmp(id, "HOST_ALLOC") == 0)
        {
            ঘূৰাই &ctx.getImpl()->getBufferPoolHostPtr();
        }
        যদি (id != রিক্ত && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        ঘূৰাই &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    শূন্য flushCleanupQueue() ধ্রুৱ
    {
        // TSAN false positive: see #27638
        যদি (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            বাবে (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    শূন্য addToCleanupQueue(UMatData* u) ধ্রুৱ
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
    স্থির OpenCLAllocator* g_allocator = নূতন OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    ঘূৰাই g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


নামাকাশ cv {

// three funcs below are implemented in umatrix.cpp
শূন্য setSize( UMat& m, পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, ধ্রুৱ size_t* _steps,
              তার্কিক autoSteps = মিছা );
শূন্য finalizeHdr(UMat& m);

} // namespace cv


নামাকাশ cv { নামাকাশ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
শূন্য convertFromBuffer(শূন্য* cl_mem_buffer, size_t step, পূর্ণাঙ্ক rows, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক type, UMat& dst)
{
    পূর্ণাঙ্ক d = 2;
    পূর্ণাঙ্ক sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, সঁচা);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, পরিমাণ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, পরিমাণ(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((পূর্ণাঙ্ক)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = নূতন UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    ঘূৰাই;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
শূন্য convertFromImage(শূন্য* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, পরিমাণ(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, পরিমাণ(cl_image_format), &fmt, 0));

    পূর্ণাঙ্ক depth = CV_8U;
    ৱিকল্প (fmt.image_channel_data_type)
    {
    পক্ষ CL_UNORM_INT8:
    পক্ষ CL_UNSIGNED_INT8:
        depth = CV_8U;
        ভঙ্গ;

    পক্ষ CL_SNORM_INT8:
    পক্ষ CL_SIGNED_INT8:
        depth = CV_8S;
        ভঙ্গ;

    পক্ষ CL_UNORM_INT16:
    পক্ষ CL_UNSIGNED_INT16:
        depth = CV_16U;
        ভঙ্গ;

    পক্ষ CL_SNORM_INT16:
    পক্ষ CL_SIGNED_INT16:
        depth = CV_16S;
        ভঙ্গ;

    পক্ষ CL_SIGNED_INT32:
        depth = CV_32S;
        ভঙ্গ;

    পক্ষ CL_FLOAT:
        depth = CV_32F;
        ভঙ্গ;

    পক্ষ CL_HALF_FLOAT:
        depth = CV_16F;
        ভঙ্গ;

    স্ৱভাৱ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    পূর্ণাঙ্ক type = CV_8UC1;
    ৱিকল্প (fmt.image_channel_order)
    {
    পক্ষ CL_R:
    পক্ষ CL_A:
    পক্ষ CL_INTENSITY:
    পক্ষ CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        ভঙ্গ;

    পক্ষ CL_RG:
    পক্ষ CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        ভঙ্গ;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    পক্ষ CL_RGBA:
    পক্ষ CL_BGRA:
    পক্ষ CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        ভঙ্গ;

    স্ৱভাৱ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ভঙ্গ;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, পরিমাণ(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, পরিমাণ(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, পরিমাণ(size_t), &h, 0));

    dst.create((পূর্ণাঙ্ক)h, (পূর্ণাঙ্ক)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, রিক্ত, রিক্ত));

    CV_OCL_CHECK(clFinish(q));

    ঘূৰাই;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

স্থির শূন্য getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint অঙ্ক_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, রিক্ত, &অঙ্ক_Devices);
    যদি (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &অঙ্ক_Devices).c_str());
    }

    যদি (অঙ্ক_Devices == 0)
    {
        devices.clear();
        ঘূৰাই;
    }

    devices.resize((size_t)অঙ্ক_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, অঙ্ক_Devices, &devices[0], &অঙ্ক_Devices));
}

সংরচনা PlatformInfo::Impl
{
    Impl(শূন্য* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ধ্রুৱ
    {
        ৱর্ণ buf[1024];
        size_t sz=0;
        ঘূৰাই clGetPlatformInfo(handle, prop, পরিমাণ(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < পরিমাণ(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    পূর্ণাঙ্ক versionMajor_;
    পূর্ণাঙ্ক versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(শূন্য* platform_id)
{
    p = নূতন Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    যদি(p)
        p->release();
}

PlatformInfo::PlatformInfo(ধ্রুৱ PlatformInfo& i)
{
    যদি (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ধ্রুৱ PlatformInfo& i)
{
    যদি (i.p != p)
    {
        যদি (i.p)
            i.p->addref();
        যদি (p)
            p->release();
        p = i.p;
    }
    ঘূৰাই *এতত্;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    যদি (এতত্ != &i) {
        যদি(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

পূর্ণাঙ্ক PlatformInfo::device_সংখ্যা() ধ্রুৱ
{
    ঘূৰাই p ? (পূর্ণাঙ্ক)p->devices.size() : 0;
}

শূন্য PlatformInfo::getDevice(Device& device, পূর্ণাঙ্ক d) ধ্রুৱ
{
    CV_Assert(p && d < (পূর্ণাঙ্ক)p->devices.size() );
    যদি(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ধ্রুৱ
{
    ঘূৰাই p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ধ্রুৱ
{
    ঘূৰাই p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ধ্রুৱ
{
    ঘূৰাই p ? p->version_ : String();
}

পূর্ণাঙ্ক PlatformInfo::versionMajor() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->versionMajor_;
}

পূর্ণাঙ্ক PlatformInfo::versionMinor() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->versionMinor_;
}

স্থির শূন্য getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint অঙ্ক_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, রিক্ত, &অঙ্ক_Platforms));

    যদি (অঙ্ক_Platforms == 0)
    {
        platforms.clear();
        ঘূৰাই;
    }

    platforms.resize((size_t)অঙ্ক_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], &অঙ্ক_Platforms));
}

শূন্য getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    বাবে (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((শূন্য*)&platforms[i]) );
}

ধ্রুৱ ৱর্ণ* typeToStr(পূর্ণাঙ্ক type)
{
    স্থির ধ্রুৱ ৱর্ণ* tab[CV_DEPTH_MAX*16]=
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
    পূর্ণাঙ্ক cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুৱ ৱর্ণ* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ঘূৰাই result;
}

ধ্রুৱ ৱর্ণ* memopTypeToStr(পূর্ণাঙ্ক type)
{
    স্থির ধ্রুৱ ৱর্ণ* tab[CV_DEPTH_MAX*16] =
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
    পূর্ণাঙ্ক cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুৱ ৱর্ণ* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    ঘূৰাই result;
}

ধ্রুৱ ৱর্ণ* vecopTypeToStr(পূর্ণাঙ্ক type)
{
    স্থির ধ্রুৱ ৱর্ণ* tab[CV_DEPTH_MAX*16] =
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
    পূর্ণাঙ্ক cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ধ্রুৱ ৱর্ণ* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    ঘূৰাই result;
}

// Deprecated due to size of buf buffer being unknowable.
ধ্রুৱ ৱর্ণ* convertTypeStr(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth, পূর্ণাঙ্ক cn, ৱর্ণ* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    ঘূৰাই convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ধ্রুৱ ৱর্ণ* convertTypeStr(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth, পূর্ণাঙ্ক cn, ৱর্ণ* buf, size_t buf_size)
{
    যদি( sdepth == ddepth )
        ঘূৰাই "noconvert";
    ধ্রুৱ ৱর্ণ *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    যদি( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    অন্যথা যদি( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    অন্যথা
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    ঘূৰাই buf;
}

ধ্রুৱ ৱর্ণ* getOpenCLErrorString(পূর্ণাঙ্ক errorCode)
{
#define CV_OCL_CODE(id) পক্ষ id: ঘূৰাই #id
#define CV_OCL_CODE_(id, name) পক্ষ id: ঘূৰাই #name
    ৱিকল্প (errorCode)
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
    CV_OCL_CODE(CL_INVALID_IMAGE_SIZE);
    CV_OCL_CODE(CL_INVALID_SAMPLER);
    CV_OCL_CODE(CL_INVALID_দ্বিমিক);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_মান);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_গুনতি);
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
    স্ৱভাৱ: ঘূৰাই "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
স্থির std::string kerToStr(ধ্রুৱ Mat & k)
{
    পূর্ণাঙ্ক width = k.cols - 1, depth = k.depth();
    ধ্রুৱ T * ধ্রুৱ data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    যদি (depth <= CV_8S)
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < width; ++i)
            stream << "DIG(" << (পূর্ণাঙ্ক)data[i] << ")";
        stream << "DIG(" << (পূর্ণাঙ্ক)data[width] << ")";
    }
    অন্যথা যদি (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        বাবে (পূর্ণাঙ্ক i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    অন্যথা যদি (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        বাবে (পূর্ণাঙ্ক i = 0; i < width; ++i)
            stream << "DIG(" << (ভিন্ন)data[i] << "h)";
        stream << "DIG(" << (ভিন্ন)data[width] << "h)";
    }
    অন্যথা
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    ঘূৰাই stream.str();
}

String kernelToStr(প্ৰবেশ_Array _kernel, পূর্ণাঙ্ক ddepth, ধ্রুৱ ৱর্ণ * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    পূর্ণাঙ্ক depth = kernel.depth();
    যদি (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    যদি (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    যদি (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    প্রকারনির্দেশ std::string (* func_t)(ধ্রুৱ Mat &);
    স্থির ধ্রুৱ func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<ৱর্ণ>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<পূর্ণাঙ্ক>, kerToStr<ভিন্ন>, kerToStr<দ্ৱিগুণ>, kerToStr<hfloat>,
                                                0, kerToStr<তার্কিক>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ধ্রুৱ func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    ঘূৰাই cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    কৰা \
    { \
        যদি (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            পূর্ণাঙ্ক ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            যদি (cwidth < ckercn || ckercn <= 0) \
                ঘূৰাই 1; \
            cols.push_back(cwidth); \
            যদি (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                ঘূৰাই 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    যেতিয়ালৈ ((শূন্য)0, 0)

পূর্ণাঙ্ক predictOptimalVectorWidth(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, প্ৰবেশ_Array src3,
                              প্ৰবেশ_Array src4, প্ৰবেশ_Array src5, প্ৰবেশ_Array src6,
                              প্ৰবেশ_Array src7, প্ৰবেশ_Array src8, প্ৰবেশ_Array src9,
                              OclVectorStrategy strat)
{
    ধ্রুৱ ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    পূর্ণাঙ্ক vectorWidths[CV_DEPTH_MAX] = {
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

    ঘূৰাই checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

পূর্ণাঙ্ক checkOptimalVectorWidth([[maybe_unused]] ধ্রুৱ পূর্ণাঙ্ক *vectorWidths,
                            [[maybe_unused]] প্ৰবেশ_Array src1,
                            [[maybe_unused]] প্ৰবেশ_Array src2,
                            [[maybe_unused]] প্ৰবেশ_Array src3,
                            [[maybe_unused]] প্ৰবেশ_Array src4,
                            [[maybe_unused]] প্ৰবেশ_Array src5,
                            [[maybe_unused]] প্ৰবেশ_Array src6,
                            [[maybe_unused]] প্ৰবেশ_Array src7,
                            [[maybe_unused]] প্ৰবেশ_Array src8,
                            [[maybe_unused]] প্ৰবেশ_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    ঘূৰাই 1;
#অন্যথা
    CV_Assert(vectorWidths);

    পূর্ণাঙ্ক ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<পূর্ণাঙ্ক> dividers, kercns;
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

    বাবে (size_t i = 0; i < size; ++i)
        যেতিয়ালৈ (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    পূর্ণাঙ্ক kercn = *std::min_element(kercns.begin(), kercns.end());

    ঘূৰাই kercn;
#endif
}

পূর্ণাঙ্ক predictOptimalVectorWidthMax(প্ৰবেশ_Array src1, প্ৰবেশ_Array src2, প্ৰবেশ_Array src3,
                                 প্ৰবেশ_Array src4, প্ৰবেশ_Array src5, প্ৰবেশ_Array src6,
                                 প্ৰবেশ_Array src7, প্ৰবেশ_Array src8, প্ৰবেশ_Array src9)
{
    ঘূৰাই predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
শূন্য buildOptionsAddMatrixDescription(String& buildOptions, ধ্রুৱ String& name, প্ৰবেশ_Array _m)
{
    যদি (!buildOptions.empty())
        buildOptions += " ";
    পূর্ণাঙ্ক type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (পূর্ণাঙ্ক)CV_MAT_CN(type),
            name.c_str(), (পূর্ণাঙ্ক)CV_ELEM_SIZE(type),
            name.c_str(), (পূর্ণাঙ্ক)CV_ELEM_SIZE1(type),
            name.c_str(), (পূর্ণাঙ্ক)depth
            );
}


সংরচনা Image2D::Impl
{
    Impl(ধ্রুৱ UMat &src, তার্কিক norm, তার্কিক alias)
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

    স্থির cl_image_format getImageFormat(পূর্ণাঙ্ক depth, পূর্ণাঙ্ক cn, তার্কিক norm)
    {
        cl_image_format format;
        স্থির ধ্রুৱ পূর্ণাঙ্ক channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        স্থির ধ্রুৱ পূর্ণাঙ্ক channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        স্থির ধ্রুৱ পূর্ণাঙ্ক channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        পূর্ণাঙ্ক channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        পূর্ণাঙ্ক channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        ঘূৰাই format;
    }

    স্থির তার্কিক isFormatSupported(cl_image_format format)
    {
        যদি (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        যদি (!context)
            ঘূৰাই মিছা;

        // Figure out how many formats are supported by this context.
        cl_uint অঙ্ক_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, অঙ্ক_Formats,
                                                রিক্ত, &অঙ্ক_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        যদি (অঙ্ক_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(অঙ্ক_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, অঙ্ক_Formats,
                                             formats.data(), রিক্ত);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            বাবে (cl_uint i = 0; i < অঙ্ক_Formats; ++i)
            {
                যদি (!memcmp(&formats[i], &format, পরিমাণ(format)))
                {
                    ঘূৰাই সঁচা;
                }
            }
        }
        ঘূৰাই মিছা;
    }

    শূন্য init(ধ্রুৱ UMat &src, তার্কিক norm, তার্কিক alias)
    {
        যদি (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        পূর্ণাঙ্ক err, depth = src.depth(), cn = src.channels();
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
        ধ্রুৱ Device & d = ocl::Device::getDefault();
        পূর্ণাঙ্ক minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        যদি (1 < major || (1 == major && 2 <= minor))
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
            desc.অঙ্ক_mip_levels   = 0;
            desc.অঙ্ক_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, রিক্ত, &err);
        }
        অন্যথা
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, রিক্ত, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        যদি (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), রিক্ত, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long পূর্ণাঙ্ক)(src.cols * src.rows * src.elemSize()), (শূন্য*)devData
                ).c_str());

            ধ্রুৱ size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, রিক্ত, রিক্ত));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        অন্যথা
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != রিক্ত);

        যদি (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, রিক্ত, 0));
            যদি (!src.isContinuous())
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
    p = রিক্ত;
}

Image2D::Image2D(ধ্রুৱ UMat &src, তার্কিক norm, তার্কিক alias)
{
    p = নূতন Impl(src, norm, alias);
}

তার্কিক Image2D::canCreateAlias(ধ্রুৱ UMat &m)
{
    তার্কিক ret = মিছা;
    ধ্রুৱ Device & d = ocl::Device::getDefault();
    যদি (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        যদি (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            যদি (!m.u->tempUMat())
            {
                ret = সঁচা;
            }
        }
    }
    ঘূৰাই ret;
}

তার্কিক Image2D::isFormatSupported(পূর্ণাঙ্ক depth, পূর্ণাঙ্ক cn, তার্কিক norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    ঘূৰাই Impl::isFormatSupported(format);
}

Image2D::Image2D(ধ্রুৱ Image2D & i)
{
    p = i.p;
    যদি (p)
        p->addref();
}

Image2D & Image2D::operator = (ধ্রুৱ Image2D & i)
{
    যদি (i.p != p)
    {
        যদি (i.p)
            i.p->addref();
        যদি (p)
            p->release();
        p = i.p;
    }
    ঘূৰাই *এতত্;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    যদি (এতত্ != &i) {
        যদি (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    ঘূৰাই *এতত্;
}

Image2D::~Image2D()
{
    যদি (p)
        p->release();
}

শূন্য* Image2D::ptr() ধ্রুৱ
{
    ঘূৰাই p ? p->handle : 0;
}

তার্কিক internal::isOpenCLForced()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক value = মিছা;
    যদি (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই value;
}

তার্কিক internal::isPerformanceCheckBypassed()
{
    স্থির তার্কিক initialized = মিছা;
    স্থির তার্কিক value = মিছা;
    যদি (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", মিছা);
        initialized = সঁচা;
    }
    ঘূৰাই value;
}

তার্কিক internal::isCLBuffer(UMat& u)
{
    যদি (isCudaBacked(u.u))
        ঘূৰাই মিছা;
    শূন্য* h = u.handle(ACCESS_RW);
    যদি (!h)
        ঘূৰাই সঁচা;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#যদি 1
    যদি ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        ঘূৰাই মিছা;
#অন্যথা
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, পরিমাণ(type), &type, রিক্ত);
    যদি (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        ঘূৰাই মিছা;
#endif
    ঘূৰাই সঁচা;
}

সংরচনা Timer::Impl
{
    ধ্রুৱ Queue queue;

    Impl(ধ্রুৱ Queue& q)
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

    uint64 durationNS() ধ্রুৱ
    {
        ঘূৰাই (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ধ্রুৱ Queue& q) : p(নূতন Impl(q)) { }
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

uint64 Timer::durationNS() ধ্রুৱ
{
    CV_Assert(p);
    ঘূৰাই p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
