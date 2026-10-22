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

#अन्तर्भाव "precomp.hpp"

#ifndef HAVE_OPENCL
#अन्तर्भाव "ocl_disabled.impl.hpp"
#अण्णहा // HAVE_OPENCL

#अन्तर्भाव <list>
#अन्तर्भाव <map>
#अन्तर्भाव <deque>
#अन्तर्भाव <set>
#अन्तर्भाव <string>
#अन्तर्भाव <sstream>
#अन्तर्भाव <fstream>
#जइ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#अन्तर्भाव <inttypes.h>
#endif

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "opencv2/core/utils/lock.private.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(रिक्त, 0, "OpenCL allocator: " << __VA_ARGS__)
#अन्तर्भाव "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

#define CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#define CV_OPENCL_SHOW_BUILD_OPTIONS             0
#define CV_OPENCL_SHOW_BUILD_KERNELS             0

#define CV_OPENCL_SHOW_RUN_KERNELS               0
#define CV_OPENCL_SYNC_RUN_KERNELS               0
#define CV_OPENCL_TRACE_CHECK                    0

#define CV_OPENCL_VALIDATE_द्विमान_PROGRAMS       1

#define CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#define CV_OPENCL_SHOW_SVM_LOG                   0

#अन्तर्भाव "opencv2/core/bufferpool.hpp"
#ifndef LOG_BUFFER_POOL
# जइ 0
#   define LOG_BUFFER_POOL printf
# अण्णहा
#   define LOG_BUFFER_POOL(...)
# endif
#endif

#जइ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_P printf("line %d (ocl.cpp): ", __LINE__); printf
#अण्णहा
#define CV_OPENCL_SVM_TRACE_P(...)
#endif

#जइ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#define CV_OPENCL_SVM_TRACE_ERROR_P printf("Error on line %d (ocl.cpp): ", __LINE__); printf
#अण्णहा
#define CV_OPENCL_SVM_TRACE_ERROR_P(...)
#endif

#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_core.hpp"

#ifdef HAVE_OPENCL_SVM
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#अन्तर्भाव "opencv2/core/opencl/opencl_svm.hpp"
#endif

#अन्तर्भाव "umatrix.hpp"

नामाकाश cv { नामाकाश ocl {

#define IMPLEMENT_REFCOUNTABLE() \
    शून्य addref() { CV_XADD(&refcount, 1); } \
    शून्य release() { जइ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) delete एतत्; } \
    पूर्णाङ्क refcount

स्थिर cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    प्रत्यागम opencl_allocator_stats;
}

#ifndef _DEBUG
स्थिर तार्किक isRaiseError()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक value = मिच्छ;
    जइ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम value;
}
#endif

स्थिर शून्य onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", मिच्छ);
    जइ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#जइ CV_OPENCL_TRACE_CHECK
स्थिर अन्तःस्थ
शून्य traceOpenCLCheck(cl_int status, ध्रुव वर्ण* message)
{
    std::cout << "OpenCV(OpenCL:" << status << "): " << message << std::endl << std::flush;
}
#define CV_OCL_TRACE_CHECK_RESULT(status, message) traceOpenCLCheck(status, message)
#अण्णहा
#define CV_OCL_TRACE_CHECK_RESULT(status, message) /* nothing */
#endif

#define CV_OCL_API_ERROR_सन्देशः(check_result, सन्देशः) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_result), check_result, सन्देशः)

#define CV_OCL_CHECK_RESULT(check_result, सन्देशः) \
    करेहि { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, सन्देशः); \
        जइ (check_result != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(सन्देशः), ध्रुव वर्ण*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_सन्देशः = CV_OCL_API_ERROR_सन्देशः(check_result, सन्देशः); \
            CV_Error(Error::OpenCLApiCallError, error_सन्देशः); \
        } \
    } जाव (0)

#define CV_OCL_CHECK_(व्यञ्जकम्, check_result) करेहि { व्यञ्जकम्; CV_OCL_CHECK_RESULT(check_result, #expr); } जाव (0)

#define CV_OCL_CHECK(व्यञ्जकम्) करेहि { cl_int __cl_result = (व्यञ्जकम्); CV_OCL_CHECK_RESULT(__cl_result, #expr); } जाव (0)

#ifdef _DEBUG
#define CV_OCL_DBG_CHECK_RESULT(check_result, सन्देशः) CV_OCL_CHECK_RESULT(check_result, सन्देशः)
#define CV_OCL_DBG_CHECK(व्यञ्जकम्) CV_OCL_CHECK(व्यञ्जकम्)
#define CV_OCL_DBG_CHECK_(व्यञ्जकम्, check_result) CV_OCL_CHECK_(व्यञ्जकम्, check_result)
#अण्णहा
#define CV_OCL_DBG_CHECK_RESULT(check_result, सन्देशः) \
    करेहि { \
        CV_OCL_TRACE_CHECK_RESULT(check_result, सन्देशः); \
        जइ (check_result != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(सन्देशः), ध्रुव वर्ण*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_सन्देशः = CV_OCL_API_ERROR_सन्देशः(check_result, सन्देशः); \
            CV_Error(Error::OpenCLApiCallError, error_सन्देशः); \
        } \
    } जाव (0)
#define CV_OCL_DBG_CHECK_(व्यञ्जकम्, check_result) करेहि { व्यञ्जकम्; CV_OCL_DBG_CHECK_RESULT(check_result, #expr); } जाव (0)
#define CV_OCL_DBG_CHECK(व्यञ्जकम्) करेहि { cl_int __cl_result = (व्यञ्जकम्); CV_OCL_DBG_CHECK_RESULT(__cl_result, #expr); } जाव (0)
#endif


स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", सच्च);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", सच्च);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", सच्च);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", सच्च);

#जइ CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
स्थिर ध्रुव तार्किक CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम् = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", मिच्छ);
#endif

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
स्थिर ध्रुव तार्किक CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#जइ 1 //def __APPLE__
        सच्च
#अण्णहा
        मिच्छ
#endif
);

स्थिर String getBuildExtraOptions()
{
    स्थिर String param_buildExtraOptions;
    स्थिर तार्किक initialized = मिच्छ;
    जइ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = सच्च;
        जइ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(रिक्त, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    प्रत्यागम param_buildExtraOptions;
}

स्थिर ध्रुव तार्किक CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", सच्च);
स्थिर ध्रुव size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


संरचना UMat2D
{
    UMat2D(ध्रुव UMat& m)
    {
        offset = (पूर्णाङ्क)m.offset;
        step = (पूर्णाङ्क)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    पूर्णाङ्क offset;
    पूर्णाङ्क step;
    पूर्णाङ्क rows;
    पूर्णाङ्क cols;
};

संरचना UMat3D
{
    UMat3D(ध्रुव UMat& m)
    {
        offset = (पूर्णाङ्क)m.offset;
        step = (पूर्णाङ्क)m.step.p[1];
        slicestep = (पूर्णाङ्क)m.step.p[0];
        slices = (पूर्णाङ्क)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    पूर्णाङ्क offset;
    पूर्णाङ्क slicestep;
    पूर्णाङ्क step;
    पूर्णाङ्क slices;
    पूर्णाङ्क rows;
    पूर्णाङ्क cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
स्थिर uint64 crc64( ध्रुव uchar* data, size_t size, uint64 crc0=0 )
{
    स्थिर uint64 table[256];
    स्थिर तार्किक initialized = मिच्छ;

    जइ( !initialized )
    {
        प्रति( पूर्णाङ्क i = 0; i < 256; i++ )
        {
            uint64 c = i;
            प्रति( पूर्णाङ्क j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = सच्च;
    }

    uint64 crc = ~crc0;
    प्रति( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ data[idx]] ^ (crc >> 8);

    प्रत्यागम ~crc;
}

#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
संरचना OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    प्रकारनिर्देश std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(रिक्त, "Initializing OpenCL cache configuration...");
        जइ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(रिक्त, "OpenCL cache is disabled");
            प्रत्यागम;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        जइ (cache_path_.empty())
        {
            CV_LOG_INFO(रिक्त, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        करेहि
        {
            यत्न
            {
                जइ (cache_path_.empty())
                    भङ्ग;
                जइ (cache_path_ == "disabled")
                    भङ्ग;
                जइ (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(रिक्त, "Can't use OpenCL cache directory: " << cache_path_);
                    clear();
                    भङ्ग;
                }

                जइ (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    जइ (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(रिक्त, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::ofstream lock_filename(cache_lock_filename_.c_str(), std::ios::out);
                        जइ (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(रिक्त, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            भङ्ग;
                        }
                    }

                    यत्न
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(रिक्त, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(रिक्त, 0, "Checking cache lock... Done!");
                    }
                    गृहाण (ध्रुव cv::Exception& e)
                    {
                        CV_LOG_WARNING(रिक्त, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::endl << e.what());
                    }
                    गृहाण (...)
                    {
                        CV_LOG_WARNING(रिक्त, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                अण्णहा
                {
                    जइ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(रिक्त, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    अण्णहा
                    {
                        CV_LOG_INFO(रिक्त, "OpenCL cache lock is disabled");
                    }
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_WARNING(रिक्त, "Can't prepare OpenCL program cache: " << cache_path_ << std::endl << e.what());
                clear();
            }
        } जाव (0);
        जइ (!cache_path_.empty())
        {
            जइ (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(रिक्त, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            अण्णहा
            {
                CV_LOG_INFO(रिक्त, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    शून्य clear()
    {
        cache_path_.clear();
        cache_lock_filename_.clear();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(ध्रुव std::string& ctx_prefix,
            ध्रुव std::string& cleanup_prefix)
    {
        जइ (cache_path_.empty())
            प्रत्यागम std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        जइ (found_it != prepared_contexts_.end())
            प्रत्यागम found_it->second;

        CV_LOG_INFO(रिक्त, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        तार्किक result = utils::fs::isDirectory(target_directory);
        जइ (!result)
        {
            यत्न
            {
                CV_LOG_VERBOSE(रिक्त, 0, "Creating directory: " << target_directory);
                जइ (utils::fs::createDirectories(target_directory))
                {
                    result = सच्च;
                }
                अण्णहा
                {
                    CV_LOG_WARNING(रिक्त, "Can't create directory: " << target_directory);
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_ERROR(रिक्त, "Can't create OpenCL program cache directory for context: " << target_directory << std::endl << e.what());
            }
        }
        target_directory = result ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        जइ (result && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            यत्न
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, मिच्छ, सच्च);
                std::vector<String> remove_entries;
                प्रति (size_t i = 0; i < entries.size(); i++)
                {
                    ध्रुव String& name = entries[i];
                    जइ (0 == name.find(cleanup_prefix))
                    {
                        जइ (0 == name.find(ctx_prefix))
                            अनुवर्त; // skip current
                        remove_entries.push_back(name);
                    }
                }
                जइ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(रिक्त, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(रिक्त, "Trying to remove these directories...");
                    प्रति (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(रिक्त, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(रिक्त, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    प्रति (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ध्रुव String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        यत्न
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(रिक्त, "Removed: " << path);
                        }
                        गृहाण (ध्रुव cv::Exception& e)
                        {
                            CV_LOG_ERROR(रिक्त, "Exception during removal of obsolete OpenCL cache directory: " << path << std::endl << e.what());
                        }
                    }
                }
            }
            गृहाण (...)
            {
                CV_LOG_WARNING(रिक्त, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(रिक्त, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        प्रत्यागम target_directory;
    }

    स्थिर OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, नूतन OpenCLBinaryCacheConfigurator());
    }
};
वर्ग द्विमान_Program_File
{
    गणना { MAX_ENTRIES = 64 };

    प्रकारनिर्देश unsigned पूर्णाङ्क uint32_t;

    संरचना CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t sourceSignatureSize;
        //char sourceSignature[];
    };

    संरचना CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t सङ्ख्या_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    संरचना CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t nextEntryFileOffset; // 0 for the last entry in chain
        uint32_t कुञ्चिका_Size;
        uint32_t dataSize;
        //char key[];
        //char data[];
    };

    ध्रुव std::string fileName_;
    ध्रुव वर्ण* ध्रुव sourceSignature_;
    ध्रुव size_t sourceSignatureSize_;

    std::fstream f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(ध्रुव std::string& options)
    {
        uint64 hash = crc64((ध्रुव uchar*)options.c_str(), options.size(), 0);
        प्रत्यागम hash & (MAX_ENTRIES - 1);
    }

    अन्तःस्थ size_t getFileSize()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t fileSize = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        प्रत्यागम fileSize;
    }
    अन्तःस्थ uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.पठ((वर्ण*)&res, परिमाण(uint32_t));
        CV_Assert(!f.fail());
        प्रत्यागम res;
    }
    अन्तःस्थ शून्य writeUInt32(ध्रुव uint32_t value)
    {
        uint32_t v = value;
        f.लिख((वर्ण*)&v, परिमाण(uint32_t));
        CV_Assert(!f.fail());
    }

    अन्तःस्थ शून्य seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    अन्तःस्थ शून्य seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    अन्तःस्थ शून्य seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    शून्य clearFile()
    {
        f.close();
        जइ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(रिक्त, "Can't remove: " << fileName_);
        प्रत्यागम;
    }

सार्वजनिक:
    द्विमान_Program_File(ध्रुव std::string& fileName, ध्रुव वर्ण* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), sourceSignatureSize_(sourceSignature_ ? strlen(sourceSignature_) : 0)
    {
        CV_StaticAssert(परिमाण(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != रिक्त);
        CV_Assert(sourceSignatureSize_ > 0);
        memset(entryOffsets, 0, परिमाण(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        जइ(f.is_open() && getFileSize() > 0)
        {
            तार्किक isValid = मिच्छ;
            यत्न
            {
                uint32_t fileSourceSignatureSize = readUInt32();
                जइ (fileSourceSignatureSize == sourceSignatureSize_)
                {
                    cv::AutoBuffer<वर्ण> fileSourceSignature(fileSourceSignatureSize + 1);
                    f.पठ(fileSourceSignature.data(), fileSourceSignatureSize);
                    जइ (f.eof())
                    {
                        CV_LOG_ERROR(रिक्त, "Unexpected EOF");
                    }
                    अण्णहा जइ (memcmp(sourceSignature, fileSourceSignature.data(), fileSourceSignatureSize) == 0)
                    {
                        isValid = सच्च;
                    }
                }
                जइ (!isValid)
                {
                    CV_LOG_ERROR(रिक्त, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_ERROR(रिक्त, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            गृहाण (...)
            {
                CV_LOG_ERROR(रिक्त, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            जइ (!isValid)
            {
                clearFile();
            }
            अण्णहा
            {
                seekReadAbsolute(0);
            }
        }
    }

    तार्किक पठ(ध्रुव std::string& कुञ्चिका, std::vector<वर्ण>& buf)
    {
        जइ (!f.is_open())
            प्रत्यागम मिच्छ;

        size_t fileSize = getFileSize();
        जइ (fileSize == 0)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file (empty): " << fileName_);
            clearFile();
            प्रत्यागम मिच्छ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t fileSourceSignatureSize = readUInt32();
        CV_Assert(fileSourceSignatureSize > 0);
        seekReadRelative(fileSourceSignatureSize);

        uint32_t सङ्ख्या_Of_Entries = readUInt32();
        CV_Assert(सङ्ख्या_Of_Entries > 0);
        जइ (सङ्ख्या_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file: " << fileName_);
            clearFile();
            प्रत्यागम मिच्छ;
        }
        f.पठ((वर्ण*)&entryOffsets[0], परिमाण(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_अङ्कः = getHash(कुञ्चिका);

        uint32_t entryOffset = entryOffsets[entry_अङ्कः];
        FileEntry entry;
        जाव (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पठ((वर्ण*)&entry, परिमाण(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<वर्ण> file_कुञ्चिका(entry.कुञ्चिका_Size + 1);
            जइ (कुञ्चिका.size() == entry.कुञ्चिका_Size)
            {
                जइ (entry.कुञ्चिका_Size > 0)
                {
                    f.पठ(file_कुञ्चिका.data(), entry.कुञ्चिका_Size);
                    CV_Assert(!f.fail());
                }
                जइ (memcmp(file_कुञ्चिका.data(), कुञ्चिका.c_str(), entry.कुञ्चिका_Size) == 0)
                {
                    buf.resize(entry.dataSize);
                    f.पठ(&buf[0], entry.dataSize);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(रिक्त, 0, "Read...");
                    प्रत्यागम सच्च;
                }
            }
            जइ (entry.nextEntryFileOffset == 0)
                भङ्ग;
            entryOffset = entry.nextEntryFileOffset;
        }
        प्रत्यागम मिच्छ;
    }

    तार्किक लिख(ध्रुव std::string& कुञ्चिका, std::vector<वर्ण>& buf)
    {
        जइ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            जइ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                जइ (!f.is_open())
                {
                    CV_LOG_ERROR(रिक्त, "Can't create file: " << fileName_);
                    प्रत्यागम मिच्छ;
                }
            }
        }

        size_t fileSize = getFileSize();
        जइ (fileSize == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)sourceSignatureSize_);
            f.लिख(sourceSignature_, sourceSignatureSize_);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(entryOffsets, 0, परिमाण(entryOffsets));
            f.लिख((वर्ण*)entryOffsets, परिमाण(entryOffsets));
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

        uint32_t सङ्ख्या_Of_Entries = readUInt32();
        CV_Assert(सङ्ख्या_Of_Entries > 0);
        जइ (सङ्ख्या_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file: " << fileName_);
            clearFile();
            प्रत्यागम मिच्छ;
        }
        size_t tableEntriesOffset = (size_t)f.tellg();
        f.पठ((वर्ण*)&entryOffsets[0], परिमाण(entryOffsets));
        CV_Assert(!f.fail());

        uint32_t entry_अङ्कः = getHash(कुञ्चिका);

        uint32_t entryOffset = entryOffsets[entry_अङ्कः];
        FileEntry entry;
        जाव (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पठ((वर्ण*)&entry, परिमाण(entry));
            CV_Assert(!f.fail());
            cv::AutoBuffer<वर्ण> file_कुञ्चिका(entry.कुञ्चिका_Size + 1);
            जइ (कुञ्चिका.size() == entry.कुञ्चिका_Size)
            {
                जइ (entry.कुञ्चिका_Size > 0)
                {
                    f.पठ(file_कुञ्चिका.data(), entry.कुञ्चिका_Size);
                    CV_Assert(!f.fail());
                }
                जइ (0 == memcmp(file_कुञ्चिका.data(), कुञ्चिका.c_str(), entry.कुञ्चिका_Size))
                {
                    // duplicate
                    CV_LOG_VERBOSE(रिक्त, 0, "Duplicate key ignored: " << fileName_);
                    प्रत्यागम मिच्छ;
                }
            }
            जइ (entry.nextEntryFileOffset == 0)
                भङ्ग;
            entryOffset = entry.nextEntryFileOffset;
        }
        seekReadAbsolute(0);
        जइ (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.nextEntryFileOffset = (uint32_t)fileSize;
            f.लिख((वर्ण*)&entry, परिमाण(entry));
            CV_Assert(!f.fail());
        }
        अण्णहा
        {
            entryOffsets[entry_अङ्कः] = (uint32_t)fileSize;
            seekWriteAbsolute(tableEntriesOffset);
            f.लिख((वर्ण*)entryOffsets, परिमाण(entryOffsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(fileSize);
        entry.nextEntryFileOffset = 0;
        entry.dataSize = (uint32_t)buf.size();
        entry.कुञ्चिका_Size = (uint32_t)कुञ्चिका.size();
        f.लिख((वर्ण*)&entry, परिमाण(entry));
        CV_Assert(!f.fail());
        f.लिख(कुञ्चिका.c_str(), entry.कुञ्चिका_Size);
        CV_Assert(!f.fail());
        f.लिख(&buf[0], entry.dataSize);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(रिक्त, 0, "Write... (" << buf.size() << " bytes)");
        प्रत्यागम सच्च;
    }
};
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT



संरचना OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    पूर्णाङ्क device_;  // device index in context
    ocl::Queue queue_;
    पूर्णाङ्क useOpenCL_;

रक्षित:
    Impl() = delete;

    शून्य _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        पूर्णाङ्क ndevices = (पूर्णाङ्क)context_.ndevices();
        CV_Assert(ndevices > 0);
        तार्किक found = मिच्छ;
        प्रति (पूर्णाङ्क i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            जइ (dhandle == deviceID)
            {
                device_ = i;
                found = सच्च;
                भङ्ग;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    शून्य _init_device(ध्रुव ocl::Device& device)
    {
        CV_Assert(device.ptr());
        पूर्णाङ्क ndevices = (पूर्णाङ्क)context_.ndevices();
        CV_Assert(ndevices > 0);
        तार्किक found = मिच्छ;
        प्रति (पूर्णाङ्क i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            जइ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = सच्च;
                भङ्ग;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

सार्वजनिक:
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

    Impl(ध्रुव ocl::Context& context, ध्रुव ocl::Device& device, ध्रुव ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(ध्रुव ocl::Context& context, ध्रुव ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(ध्रुव ocl::Context& context, ध्रुव पूर्णाङ्क device, ध्रुव ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ध्रुव Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    अन्तःस्थ तार्किक useOpenCL() ध्रुव { प्रत्यागम const_cast<Impl*>(एतत्)->useOpenCL(); }
    तार्किक useOpenCL()
    {
        जइ (useOpenCL_ < 0)
        {
            यत्न
            {
                useOpenCL_ = 0;
                जइ (!context_.empty() && context_.ndevices() > 0)
                {
                    ध्रुव Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            गृहाण (ध्रुव cv::Exception&)
            {
                // nothing
            }
            जइ (!useOpenCL_)
                CV_LOG_INFO(रिक्त, "OpenCL: can't use OpenCL execution context");
        }
        प्रत्यागम useOpenCL_ > 0;
    }

    शून्य setUseOpenCL(तार्किक flag)
    {
        जइ (!flag)
            useOpenCL_ = 0;
        अण्णहा
            useOpenCL_ = -1;
    }

    स्थिर ध्रुव std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(रिक्त, "OpenCL: initializing thread execution context");

        स्थिर तार्किक initialized = मिच्छ;
        स्थिर std::shared_ptr<Impl> g_primaryExecutionContext;

        जइ (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            जइ (!initialized)
            {
                CV_LOG_INFO(रिक्त, "OpenCL: creating new execution context...");
                यत्न
                {
                    Context c = ocl::Context::create(std::string());
                    जइ (c.ndevices())
                    {
                        पूर्णाङ्क deviceId = 0;
                        auto& d = c.device(deviceId);
                        जइ (d.available())
                        {
                            auto q = ocl::Queue(c, d);
                            जइ (!q.ptr())
                            {
                                CV_LOG_ERROR(रिक्त, "OpenCL: Can't create default OpenCL queue");
                            }
                            अण्णहा
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(रिक्त, "OpenCL: device=" << d.name());
                            }
                        }
                        अण्णहा
                        {
                            CV_LOG_ERROR(रिक्त, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    अण्णहा
                    {
                        CV_LOG_INFO(रिक्त, "OpenCL: context is not available/disabled");
                    }
                }
                गृहाण (ध्रुव std::exception& e)
                {
                    CV_LOG_INFO(रिक्त, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                गृहाण (...)
                {
                    CV_LOG_WARNING(रिक्त, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = सच्च;
            }
        }
        प्रत्यागम g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->context_;
}
Device& OpenCLExecutionContext::getDevice() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->queue_;
}

तार्किक OpenCLExecutionContext::useOpenCL() ध्रुव
{
    जइ (p)
        प्रत्यागम p->useOpenCL();
    प्रत्यागम मिच्छ;
}
शून्य OpenCLExecutionContext::setUseOpenCL(तार्किक flag)
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
    जइ (!data.oclExecutionContextInitialized)
    {
        data.oclExecutionContextInitialized = सच्च;
        जइ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    प्रत्यागम c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& data = getCoreTlsData();
    OpenCLExecutionContext& c = data.oclExecutionContext;
    प्रत्यागम c;
}

शून्य OpenCLExecutionContext::bind() ध्रुव
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& data = getCoreTlsData();
    data.oclExecutionContext = *एतत्;
    data.oclExecutionContextInitialized = सच्च;
    data.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() ध्रुव
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ध्रुव Queue q(getContext(), getDevice());
    प्रत्यागम cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(ध्रुव ocl::Queue& q) ध्रुव
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != रिक्त);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    प्रत्यागम c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device, ध्रुव ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    जइ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    प्रत्यागम ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ध्रुव Context& context, ध्रुव Device& device)
{
    CV_TRACE_FUNCTION();
    जइ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    प्रत्यागम ctx;

}

शून्य OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
स्थिर तार्किक g_isOpenCLInitialized = मिच्छ;
स्थिर तार्किक g_isOpenCLAvailable = मिच्छ;

तार्किक haveOpenCL()
{
    CV_TRACE_FUNCTION();

    जइ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        जइ (!envPath.empty())
        {
            जइ (envPath == "disabled")
            {
                g_isOpenCLAvailable = मिच्छ;
                g_isOpenCLInitialized = सच्च;
                प्रत्यागम मिच्छ;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(रिक्त, "Initialize OpenCL runtime...");
        यत्न
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, रिक्त, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(रिक्त, "OpenCL: found " << n << " platforms");
        }
        गृहाण (...)
        {
            g_isOpenCLAvailable = मिच्छ;
        }
        g_isOpenCLInitialized = सच्च;
    }
    प्रत्यागम g_isOpenCLAvailable;
}

तार्किक useOpenCL()
{
    CoreTLSData& data = getCoreTlsData();
    जइ (data.useOpenCL < 0)
    {
        यत्न
        {
            data.useOpenCL = 0;
            जइ (haveOpenCL())
            {
                auto c = OpenCLExecutionContext::getCurrent();
                data.useOpenCL = c.useOpenCL();
            }
        }
        गृहाण (...)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    प्रत्यागम data.useOpenCL > 0;
}

तार्किक isOpenCLActivated()
{
    जइ (!g_isOpenCLAvailable)
        प्रत्यागम मिच्छ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    प्रत्यागम useOpenCL();
}

शून्य setUseOpenCL(तार्किक flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& data = getCoreTlsData();
    auto& c = OpenCLExecutionContext::getCurrentRef();
    जइ (!c.empty())
    {
        c.setUseOpenCL(flag);
        data.useOpenCL = c.useOpenCL();
    }
    अण्णहा
    {
        जइ (!flag)
            data.useOpenCL = 0;
        अण्णहा
            data.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#ifdef HAVE_CLAMDBLAS

वर्ग AmdBlasHelper
{
सार्वजनिक:
    स्थिर AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, नूतन AmdBlasHelper())
    }

    तार्किक isAvailable() ध्रुव
    {
        प्रत्यागम g_isAmdBlasAvailable;
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

रक्षित:
    AmdBlasHelper()
    {
        जइ (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            जइ (!g_isAmdBlasInitialized)
            {
                जइ (haveOpenCL())
                {
                    यत्न
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    गृहाण (...)
                    {
                        g_isAmdBlasAvailable = मिच्छ;
                    }
                }
                अण्णहा
                    g_isAmdBlasAvailable = मिच्छ;

                g_isAmdBlasInitialized = सच्च;
            }
        }
    }

गुप्त:
    स्थिर तार्किक g_isAmdBlasInitialized;
    स्थिर तार्किक g_isAmdBlasAvailable;
};

तार्किक AmdBlasHelper::g_isAmdBlasAvailable = मिच्छ;
तार्किक AmdBlasHelper::g_isAmdBlasInitialized = मिच्छ;

तार्किक haveAmdBlas()
{
    प्रत्यागम AmdBlasHelper::getInstance().isAvailable();
}

#अण्णहा

तार्किक haveAmdBlas()
{
    प्रत्यागम मिच्छ;
}

#endif

#ifdef HAVE_CLAMDFFT

वर्ग AmdFftHelper
{
सार्वजनिक:
    स्थिर AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, नूतन AmdFftHelper())
    }

    तार्किक isAvailable() ध्रुव
    {
        प्रत्यागम g_isAmdFftAvailable;
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

रक्षित:
    AmdFftHelper()
    {
        जइ (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            जइ (!g_isAmdFftInitialized)
            {
                जइ (haveOpenCL())
                {
                    यत्न
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfftInitSetupData(&setupData) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = सच्च;
                    }
                    गृहाण (ध्रुव Exception &)
                    {
                        g_isAmdFftAvailable = मिच्छ;
                    }
                }
                अण्णहा
                    g_isAmdFftAvailable = मिच्छ;

                g_isAmdFftInitialized = सच्च;
            }
        }
    }

गुप्त:
    स्थिर clfftSetupData setupData;
    स्थिर तार्किक g_isAmdFftInitialized;
    स्थिर तार्किक g_isAmdFftAvailable;
};

clfftSetupData AmdFftHelper::setupData;
तार्किक AmdFftHelper::g_isAmdFftAvailable = मिच्छ;
तार्किक AmdFftHelper::g_isAmdFftInitialized = मिच्छ;

तार्किक haveAmdFft()
{
    प्रत्यागम AmdFftHelper::getInstance().isAvailable();
}

#अण्णहा

तार्किक haveAmdFft()
{
    प्रत्यागम मिच्छ;
}

#endif

तार्किक haveSVM()
{
#ifdef HAVE_OPENCL_SVM
    प्रत्यागम सच्च;
#अण्णहा
    प्रत्यागम मिच्छ;
#endif
}

शून्य finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

संरचना Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = मिच्छ;
    }

    ~Impl() {}

    शून्य init()
    {
        जइ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            जइ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            जइ( handle != 0 )
            {
                वर्ण buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, परिमाण(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = सच्च;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    तार्किक initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    जइ(p)
        p->release();
}

Platform::Platform(ध्रुव Platform& pl)
{
    p = (Impl*)pl.p;
    जइ(p)
        p->addref();
}

Platform& Platform::operator = (ध्रुव Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::operator = (Platform&& pl) CV_NOEXCEPT
{
    जइ (एतत् != &pl) {
        जइ(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

शून्य* Platform::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(रिक्त, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    स्थिर Platform p;
    जइ( !p.p )
    {
        p.p = नूतन Impl;
        p.p->init();
    }
    प्रत्यागम p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
स्थिर शून्य parseOpenCLVersion(ध्रुव String &version, पूर्णाङ्क &major, पूर्णाङ्क &minor)
{
    major = minor = 0;
    जइ (10 >= version.length())
        प्रत्यागम;
    ध्रुव वर्ण *pstr = version.c_str();
    जइ (0 != strncmp(pstr, "OpenCL ", 7))
        प्रत्यागम;
    size_t ppos = version.find('.', 7);
    जइ (String::npos == ppos)
        प्रत्यागम;
    String temp = version.substr(7, ppos - 7);
    major = atoi(temp.c_str());
    temp = version.substr(ppos + 1);
    minor = atoi(temp.c_str());
}

संरचना Device::Impl
{
    Impl(शून्य* d)
        : refcount(1)
        , handle(0)
    {
        यत्न
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        गृहाण (...)
        {
            क्षिप;
        }
    }

    शून्य _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_COMPUTE_UNITS);
        maxWorkGroupSize_ = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_GROUP_SIZE);
        type_ = getProp<cl_device_type, पूर्णाङ्क>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        size_t pos = 0;
        जाव (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            जइ (pos2 == String::npos)
                pos2 = extensions_.size();
            जइ (pos2 > pos)
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
        जइ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        अण्णहा जइ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || strstr(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        अण्णहा जइ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        अण्णहा
            vendorID_ = UNKNOWN_VENDOR;

        ध्रुव size_t CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        जइ (CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE > 0)
        {
            ध्रुव size_t new_maxWorkGroupSize = std::min(maxWorkGroupSize_, CV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE);
            जइ (new_maxWorkGroupSize != maxWorkGroupSize_)
                CV_LOG_WARNING(रिक्त, "OpenCL: using workgroup size: " << new_maxWorkGroupSize << " (was " << maxWorkGroupSize_ << ")");
            maxWorkGroupSize_ = new_maxWorkGroupSize;
        }
#जइ 0
        जइ (isExtensionSupported("cl_khr_spir"))
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
        जइ (!cv::__termination)
#endif
        {
            जइ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    template<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) ध्रुव
    {
        _TpCL temp=_TpCL();
        size_t sz = 0;

        प्रत्यागम clGetDeviceInfo(handle, prop, परिमाण(temp), &temp, &sz) == CL_SUCCESS &&
            sz == परिमाण(temp) ? _TpOut(temp) : _TpOut();
    }

    तार्किक getBoolProp(cl_device_info prop) ध्रुव
    {
        cl_bool temp = CL_FALSE;
        size_t sz = 0;

        प्रत्यागम clGetDeviceInfo(handle, prop, परिमाण(temp), &temp, &sz) == CL_SUCCESS &&
            sz == परिमाण(temp) ? temp != 0 : मिच्छ;
    }

    String getStrProp(cl_device_info prop) ध्रुव
    {
        वर्ण buf[4096];
        size_t sz=0;
        प्रत्यागम clGetDeviceInfo(handle, prop, परिमाण(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < परिमाण(buf) ? String(buf) : String();
    }

    तार्किक isExtensionSupported(ध्रुव std::string& extensionName) ध्रुव
    {
        प्रत्यागम extensions_set_.गणनम्_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    पूर्णाङ्क doubleFPConfig_;
    तार्किक khr_fp64_support_;
    पूर्णाङ्क halfFPConfig_;
    तार्किक khr_fp16_support_;
    तार्किक hostUnifiedMemory_;
    पूर्णाङ्क maxComputeUnits_;
    size_t maxWorkGroupSize_;
    पूर्णाङ्क type_;
    पूर्णाङ्क addressBits_;
    पूर्णाङ्क deviceVersionMajor_;
    पूर्णाङ्क deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    पूर्णाङ्क vendorID_;
    तार्किक intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(शून्य* d)
{
    p = 0;
    set(d);
}

Device::Device(ध्रुव Device& d)
{
    p = d.p;
    जइ(p)
        p->addref();
}

Device& Device::operator = (ध्रुव Device& d)
{
    Impl* newp = (Impl*)d.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::operator = (Device&& d) CV_NOEXCEPT
{
    जइ (एतत् != &d) {
        जइ(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

Device::~Device()
{
    जइ(p)
        p->release();
}

शून्य Device::set(शून्य* d)
{
    जइ(p)
        p->release();
    p = नूतन Impl(d);
    जइ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(शून्य* d)
{
    Device device(d);
    प्रत्यागम device;
}

शून्य* Device::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

String Device::name() ध्रुव
{ प्रत्यागम p ? p->name_ : String(); }

String Device::extensions() ध्रुव
{ प्रत्यागम p ? String(p->extensions_) : String(); }

तार्किक Device::isExtensionSupported(ध्रुव String& extensionName) ध्रुव
{ प्रत्यागम p ? p->isExtensionSupported(extensionName) : मिच्छ; }

String Device::version() ध्रुव
{ प्रत्यागम p ? p->version_ : String(); }

String Device::vendorName() ध्रुव
{ प्रत्यागम p ? p->vendorName_ : String(); }

पूर्णाङ्क Device::vendorID() ध्रुव
{ प्रत्यागम p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() ध्रुव
{ प्रत्यागम p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() ध्रुव
{ प्रत्यागम p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

पूर्णाङ्क Device::deviceVersionMajor() ध्रुव
{ प्रत्यागम p ? p->deviceVersionMajor_ : 0; }

पूर्णाङ्क Device::deviceVersionMinor() ध्रुव
{ प्रत्यागम p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() ध्रुव
{ प्रत्यागम p ? p->driverVersion_ : String(); }

पूर्णाङ्क Device::type() ध्रुव
{ प्रत्यागम p ? p->type_ : 0; }

पूर्णाङ्क Device::addressBits() ध्रुव
{ प्रत्यागम p ? p->addressBits_ : 0; }

तार्किक Device::available() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : मिच्छ; }

तार्किक Device::compilerAvailable() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : मिच्छ; }

तार्किक Device::linkerAvailable() ध्रुव
#ifdef CL_VERSION_1_2
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : मिच्छ; }
#अण्णहा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

पूर्णाङ्क Device::doubleFPConfig() ध्रुव
{ प्रत्यागम p ? p->doubleFPConfig_ : 0; }

पूर्णाङ्क Device::singleFPConfig() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

पूर्णाङ्क Device::halfFPConfig() ध्रुव
{ प्रत्यागम p ? p->halfFPConfig_ : 0; }

तार्किक Device::hasFP64() ध्रुव
{ प्रत्यागम p ? p->khr_fp64_support_ : मिच्छ; }
तार्किक Device::hasFP16() ध्रुव
{ प्रत्यागम p ? p->khr_fp16_support_ : मिच्छ; }

तार्किक Device::endianLittle() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : मिच्छ; }

तार्किक Device::errorCorrectionSupport() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : मिच्छ; }

पूर्णाङ्क Device::executionCapabilities() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_exec_capabilities, पूर्णाङ्क>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::globalMemCacheSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE) : 0; }

पूर्णाङ्क Device::globalMemCacheType() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_mem_cache_type, पूर्णाङ्क>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

पूर्णाङ्क Device::global_Mem_Cache_पङ्क्तिः_Size() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE) : 0; }

size_t Device::globalMemSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_SIZE) : 0; }

size_t Device::localMemSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_SIZE) : 0; }

पूर्णाङ्क Device::localMemType() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_local_mem_type, पूर्णाङ्क>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

तार्किक Device::hostUnifiedMemory() ध्रुव
{ प्रत्यागम p ? p->hostUnifiedMemory_ : मिच्छ; }

तार्किक Device::imageSupport() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : मिच्छ; }

तार्किक Device::imageFromBufferSupport() ध्रुव
{
    प्रत्यागम p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : मिच्छ;
}

uint Device::imagePitchAlignment() ध्रुव
{
#ifdef CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    प्रत्यागम p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#अण्णहा
    प्रत्यागम 0;
#endif
}

uint Device::imageBaseAddressAlignment() ध्रुव
{
#ifdef CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    प्रत्यागम p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#अण्णहा
    प्रत्यागम 0;
#endif
}

size_t Device::image2DMaxWidth() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

size_t Device::image2DMaxHeight() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxWidth() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

size_t Device::image3DMaxHeight() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3DMaxDepth() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

size_t Device::imageMaxBufferSize() ध्रुव
#ifdef CL_VERSION_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_BUFFER_SIZE) : 0; }
#अण्णहा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

size_t Device::imageMaxArraySize() ध्रुव
#ifdef CL_VERSION_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_SIZE) : 0; }
#अण्णहा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif

तार्किक Device::intelSubgroupsSupport() ध्रुव
{ प्रत्यागम p ? p->intelSubgroupsSupport_ : मिच्छ; }

पूर्णाङ्क Device::maxClockFrequency() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

पूर्णाङ्क Device::maxComputeUnits() ध्रुव
{ प्रत्यागम p ? p->maxComputeUnits_ : 0; }

पूर्णाङ्क Device::maxConstantArgs() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

size_t Device::maxConstantBufferSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE) : 0; }

size_t Device::maxMemAllocSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_SIZE) : 0; }

size_t Device::maxParameterSize() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_SIZE) : 0; }

पूर्णाङ्क Device::maxReadImageArgs() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

पूर्णाङ्क Device::maxWriteImageArgs() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

पूर्णाङ्क Device::maxSamplers() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::maxWorkGroupSize() ध्रुव
{ प्रत्यागम p ? p->maxWorkGroupSize_ : 0; }

पूर्णाङ्क Device::maxWorkItemDims() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

शून्य Device::maxWorkItemSizes(size_t* sizes) ध्रुव
{
    जइ(p)
    {
        ध्रुव पूर्णाङ्क MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*परिमाण(sizes[0]), &sizes[0], &retsz));
    }
}

पूर्णाङ्क Device::memBaseAddrAlign() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

पूर्णाङ्क Device::nativeVectorWidthChar() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

पूर्णाङ्क Device::nativeVectorWidthShort() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

पूर्णाङ्क Device::nativeVectorWidthInt() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

पूर्णाङ्क Device::nativeVectorWidthLong() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

पूर्णाङ्क Device::nativeVectorWidthFloat() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

पूर्णाङ्क Device::nativeVectorWidthDouble() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

पूर्णाङ्क Device::nativeVectorWidthHalf() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

पूर्णाङ्क Device::preferredVectorWidthChar() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

पूर्णाङ्क Device::preferredVectorWidthShort() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

पूर्णाङ्क Device::preferredVectorWidthInt() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

पूर्णाङ्क Device::preferredVectorWidthLong() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

पूर्णाङ्क Device::preferredVectorWidthFloat() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

पूर्णाङ्क Device::preferredVectorWidthDouble() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

पूर्णाङ्क Device::preferredVectorWidthHalf() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

size_t Device::printfBufferSize() ध्रुव
#ifdef CL_VERSION_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_BUFFER_SIZE) : 0; }
#अण्णहा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#endif


size_t Device::profilingTimerResolution() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ध्रुव Device& Device::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    जइ (!c.empty())
    {
        प्रत्यागम c.getDevice();
    }

    स्थिर Device dummy;
    प्रत्यागम dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

template <typename Functor, typename ObjectType>
अन्तःस्थ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, रिक्त, &required);
    जइ (err != CL_SUCCESS)
        प्रत्यागम err;

    param.clear();
    जइ (required > 0)
    {
        AutoBuffer<वर्ण> buf(required + 1);
        वर्ण* ptr = buf.data(); // cleanup is not needed
        err = f(obj, name, required, ptr, रिक्त);
        जइ (err != CL_SUCCESS)
            प्रत्यागम err;
        param = ptr;
    }

    प्रत्यागम CL_SUCCESS;
}

स्थिर शून्य split(ध्रुव std::string &s, वर्ण delim, std::vector<std::string> &elems)
{
    elems.clear();
    जइ (s.size() == 0)
        प्रत्यागम;
    std::istringstream ss(s);
    std::string item;
    जाव (!ss.eof())
    {
        std::getline(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
स्थिर तार्किक parseOpenCLDeviceConfiguration(ध्रुव std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    जइ (parts.size() > 3)
    {
        CV_LOG_ERROR(रिक्त, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        प्रत्यागम मिच्छ;
    }
    जइ (parts.size() > 2)
        deviceNameOrID = parts[2];
    जइ (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    जइ (parts.size() > 0)
    {
        platform = parts[0];
    }
    प्रत्यागम सच्च;
}

स्थिर cl_device_id selectOpenCLDevice(ध्रुव std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    जइ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    जइ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        प्रत्यागम रिक्त;

    तार्किक isID = मिच्छ;
    पूर्णाङ्क deviceID = -1;
    जइ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = सच्च;
        प्रति (size_t i = 0; i < deviceName.length(); i++)
        {
            जइ (!isdigit(deviceName[i]))
            {
                isID = मिच्छ;
                भङ्ग;
            }
        }
        जइ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            जइ (deviceID < 0)
                प्रत्यागम रिक्त;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint अङ्कः_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms));

        जइ (अङ्कः_Platforms == 0)
            प्रत्यागम रिक्त;
        platforms.resize((size_t)अङ्कः_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], &अङ्कः_Platforms));
        platforms.resize(अङ्कः_Platforms);
    }

    जइ (platform.length() > 0)
    {
        प्रति (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            जइ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            अण्णहा
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        जइ (platforms.size() == 0)
        {
            CV_LOG_ERROR(रिक्त, "OpenCL: Can't find OpenCL platform by name: " << platform);
            गच्छ not_found;
        }
    }
    जइ (deviceTypes.size() == 0)
    {
        जइ (!isID)
        {
            deviceTypes.push_back("GPU");
            जइ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        अण्णहा
            deviceTypes.push_back("ALL");
    }
    प्रति (size_t t = 0; t < deviceTypes.size(); t++)
    {
        पूर्णाङ्क deviceType = 0;
        std::string tempStrDeviceType = deviceTypes[t];
        std::transform(tempStrDeviceType.begin(), tempStrDeviceType.end(), tempStrDeviceType.begin(), details::char_tolower);

        जइ (tempStrDeviceType == "gpu" || tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            deviceType = Device::TYPE_GPU;
        अण्णहा जइ (tempStrDeviceType == "cpu")
            deviceType = Device::TYPE_CPU;
        अण्णहा जइ (tempStrDeviceType == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        अण्णहा जइ (tempStrDeviceType == "all")
            deviceType = Device::TYPE_ALL;
        अण्णहा
        {
            CV_LOG_ERROR(रिक्त, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            गच्छ not_found;
        }

        std::vector<cl_device_id> devices;
        प्रति (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint गणनम्_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, रिक्त, &गणनम्_2);
            जइ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get count");
            }
            जइ (गणनम्_2 == 0)
                अनुवर्त;
            size_t base = devices.size();
            devices.resize(base + गणनम्_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, गणनम्_2, &devices[base], &गणनम्_2);
            जइ (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_RESULT(status, "clGetDeviceIDs get IDs");
            }
        }

        प्रति (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : सच्च) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = सच्च;
            जइ(tempStrDeviceType == "dgpu" || tempStrDeviceType == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, परिमाण(isIGPU), &isIGPU, रिक्त));
                useGPU = tempStrDeviceType == "dgpu" ? !isIGPU : isIGPU;
            }
            जइ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                प्रत्यागम devices[i];
            }
        }
    }

not_found:
    जइ (configuration.empty())
        प्रत्यागम रिक्त; // suppress messages on stderr

    std::ostringstream सन्देशः;
    सन्देशः << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::endl
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::endl
        << "    Device types:";
    प्रति (size_t t = 0; t < deviceTypes.size(); t++)
        सन्देशः << ' ' << deviceTypes[t];

    सन्देशः << std::endl << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(रिक्त, सन्देशः.str());
    प्रत्यागम रिक्त;
}

#ifdef HAVE_OPENCL_SVM
नामाकाश svm {

गणना AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_BUFFER = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_BUFFER = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_BUFFER_MASK = 3 << 16,
        OPENCL_SVM_BUFFER_MAP = 4 << 16
};

स्थिर तार्किक checkForceSVMUmatUsage()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक force = मिच्छ;
    जइ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम force;
}
स्थिर तार्किक checkDisableSVMUMatUsage()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक force = मिच्छ;
    जइ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम force;
}
स्थिर तार्किक checkDisableSVM()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक force = मिच्छ;
    जइ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम force;
}
// see SVMCapabilities
स्थिर unsigned पूर्णाङ्क getSVMCapabilitiesMask()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर unsigned पूर्णाङ्क mask = 0;
    जइ (!initialized)
    {
        ध्रुव std::string env_मानम् = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        जइ (env_मानम्.empty())
        {
            प्रत्यागम ~0U; // all bits 1
        }
        mask = atoi(env_मानम्.c_str());
        initialized = सच्च;
    }
    प्रत्यागम mask;
}
} // namespace
#endif

स्थिर size_t get_Program_गणनम्_Limit()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर size_t गणनम्_2 = 0;
    जइ (!initialized)
    {
        गणनम्_2 = utils::getConfigurationParameterSizeT("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = सच्च;
    }
    प्रत्यागम गणनम्_2;
}

स्थिर पूर्णाङ्क g_contextId = 0;

वर्ग OpenCLBufferPoolImpl;
वर्ग OpenCLSVMBufferPoolImpl;

संरचना Context::Impl
{
    स्थिर Context::Impl* get(Context& context) { प्रत्यागम context.getImpl(); }

    प्रकारनिर्देश std::deque<Context::Impl*> container_t;
    स्थिर container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        स्थिर container_t* g_contexts = नूतन container_t();
        प्रत्यागम *g_contexts;
    }

रक्षित:
    Impl(ध्रुव std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#ifdef HAVE_OPENCL_SVM
        , svmInitialized(मिच्छ)
#endif
    {
        जइ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = एतत्;
    }

    ~Impl()
    {
#ifdef _WIN32
        जइ (!cv::__termination)
#endif
        {
            जइ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = रिक्त;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            auto& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = रिक्त;
        }
    }

    शून्य init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_अङ्कः_DEVICES, परिमाण(ndevices), &ndevices, रिक्त));
        CV_Assert(ndevices > 0);

        cv::AutoBuffer<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_size = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * परिमाण(cl_device_id), &cl_devices[0], &devices_ret_size));
        CV_CheckEQ(devices_ret_size, cl_devices.size() * परिमाण(cl_device_id), "");

        devices.clear();
        प्रति (unsigned i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    शून्य __init_buffer_pools();  // w/o synchronization
    शून्य _init_buffer_pools() ध्रुव
    {
        जइ (!bufferPool_)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            जइ (!bufferPool_)
            {
                const_cast<Impl*>(एतत्)->__init_buffer_pools();
            }
        }
    }
सार्वजनिक:
    स्थिर Impl* findContext(ध्रुव std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        auto& container = getGlobalContainer();
        जइ (configuration.empty() && !container.empty())
            प्रत्यागम container[0];
        प्रति (auto it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            जइ (i && i->configuration == configuration)
            {
                प्रत्यागम i;
            }
        }
        प्रत्यागम रिक्त;
    }

    स्थिर Impl* findOrCreateContext(ध्रुव std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        जइ (configuration_.empty())
        {
            ध्रुव std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            जइ (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        जइ (impl)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            प्रत्यागम impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        जइ (d == रिक्त)
            प्रत्यागम रिक्त;

        impl = नूतन Impl(configuration);
        यत्न
        {
            impl->createFromDevice(d);
            जइ (impl->handle)
                प्रत्यागम impl;
            delete impl;
            प्रत्यागम रिक्त;
        }
        गृहाण (...)
        {
            delete impl;
            क्षिप;
        }
    }

    स्थिर Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (शून्य*)h);
        Impl* impl = findContext(configuration);
        जइ (impl)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            प्रत्यागम impl;
        }

        impl = नूतन Impl(configuration);
        यत्न
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            प्रत्यागम impl;
        }
        गृहाण (...)
        {
            delete impl;
            क्षिप;
        }
    }

    स्थिर Impl* findOrCreateContext(ध्रुव ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (शून्य*)d);
        Impl* impl = findContext(configuration);
        जइ (impl)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            प्रत्यागम impl;
        }

        impl = नूतन Impl(configuration);
        यत्न
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            प्रत्यागम impl;
        }
        गृहाण (...)
        {
            delete impl;
            क्षिप;
        }
    }

    शून्य setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        जइ (d == रिक्त)
            प्रत्यागम;

        createFromDevice(d);
    }

    शून्य createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == रिक्त);

        cl_platform_id pl = रिक्त;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, परिमाण(cl_platform_id), &pl, रिक्त));

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

        तार्किक ok = handle != 0 && status == CL_SUCCESS;
        जइ( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        अण्णहा
            handle = रिक्त;
    }

    Program getProg(ध्रुव ProgramSource& src, ध्रुव String& buildflags, String& errmsg);

    शून्य unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        प्रति (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              जइ (it != phash.end())
              {
                  जइ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      प्रत्यागम;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        जइ (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            जइ (prefix.empty())
            {
                CV_Assert(!devices.empty());
                ध्रुव Device& d = devices[0];
                पूर्णाङ्क bits = d.addressBits();
                जइ (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                प्रति (size_t i = 0; i < prefix.size(); i++)
                {
                    वर्ण c = prefix[i];
                    जइ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        प्रत्यागम prefix;
    }

    std::string& getPrefixBase()
    {
        जइ (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            जइ (prefix_base.empty())
            {
                ध्रुव Device& d = devices[0];
                पूर्णाङ्क bits = d.addressBits();
                जइ (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                प्रति (size_t i = 0; i < prefix_base.size(); i++)
                {
                    वर्ण c = prefix_base[i];
                    जइ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        प्रत्यागम prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    ध्रुव पूर्णाङ्क contextId;  // global unique ID
    ध्रुव std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    प्रकारनिर्देश std::map<std::string, Program> phash_t;
    phash_t phash;
    प्रकारनिर्देश std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> bufferPool_;
    std::shared_ptr<OpenCLBufferPoolImpl> bufferPoolHostPtr_;
    OpenCLBufferPoolImpl& getBufferPool() ध्रुव
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPool_);
        प्रत्यागम *bufferPool_.get();
    }
    OpenCLBufferPoolImpl& getBufferPoolHostPtr() ध्रुव
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolHostPtr_);
        प्रत्यागम *bufferPoolHostPtr_.get();
    }

    std::map<std::type_index, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    शून्य setUserContext(std::type_index typeId, ध्रुव std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_index typeId) {
        cv::AutoLock lock(userContextMutex);
        auto it = userContextStorage.find(typeId);
        जइ (it != userContextStorage.end())
            प्रत्यागम it->second;
        अण्णहा
            प्रत्यागम nullptr;
    }

#ifdef HAVE_OPENCL_SVM
    तार्किक svmInitialized;
    तार्किक svmAvailable;
    तार्किक svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    शून्य svmInit()
    {
        CV_Assert(handle != रिक्त);
        ध्रुव Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((शून्य)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, परिमाण(deviceCaps), &deviceCaps, रिक्त);
        जइ (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            गच्छ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (पूर्णाङ्क)deviceCaps);
        CV_Assert(((शून्य)0, CL_DEVICE_SVM_COARSE_GRAIN_BUFFER == CL_DEVICE_SVM_COARSE_GRAIN_BUFFER_AMD)); // Check assumption
        svmCapabilities.मानम्_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_BUFFER) ? svm::SVMCapabilities::SVM_FINE_GRAIN_BUFFER : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.मानम्_2 &= svm::getSVMCapabilitiesMask();
        जइ (svmCapabilities.मानम्_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            गच्छ noSVM;
        }
        यत्न
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            शून्य* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            जइ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            यत्न
            {
                तार्किक error = मिच्छ;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                जइ (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, रिक्त, रिक्त))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                यत्न
                {
                    ((पूर्णाङ्क*)ptr)[0] = 100;
                }
                गृहाण (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = सच्च;
                }
                जइ (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, रिक्त, रिक्त))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                जइ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            गृहाण (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                क्षिप;
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
        गृहाण (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            यत्न
            {
                // Try HSA extension
                String extensions = device.extensions();
                जइ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    गच्छ noSVM;
                }
                cl_platform_id p = रिक्त;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, परिमाण(cl_platform_id), &p, रिक्त));
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
            गृहाण (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                गच्छ noSVM;
            }
        }

        svmAvailable = सच्च;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = सच्च;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        प्रत्यागम;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = मिच्छ;
        svmEnabled = मिच्छ;
        svmCapabilities.मानम्_2 = 0;
        svmInitialized = सच्च;
        svmFunctions.fn_clSVMAlloc = रिक्त;
        प्रत्यागम;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> bufferPoolSVM_;

    OpenCLSVMBufferPoolImpl& getBufferPoolSVM() ध्रुव
    {
        _init_buffer_pools();
        CV_DbgAssert(bufferPoolSVM_);
        प्रत्यागम *bufferPoolSVM_.get();
    }
#endif

    friend वर्ग Program;
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
Context::Context(पूर्णाङ्क dtype)
{
    p = 0;
    create(dtype);
}

शून्य Context::release()
{
    जइ (p)
    {
        p->release();
        p = रिक्त;
    }
}

तार्किक Context::create()
{
    release();
    जइ (!haveOpenCL())
        प्रत्यागम मिच्छ;
    p = Impl::findOrCreateContext(std::string());
    जइ (p && p->handle)
        प्रत्यागम सच्च;
    release();
    प्रत्यागम मिच्छ;
}

// deprecated
तार्किक Context::create(पूर्णाङ्क dtype)
{
    जइ( !haveOpenCL() )
        प्रत्यागम मिच्छ;
    release();
    जइ (dtype == CL_DEVICE_TYPE_DEFAULT || (unsigned)dtype == (unsigned)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    अण्णहा जइ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    अण्णहा जइ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    अण्णहा
    {
        CV_LOG_ERROR(रिक्त, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    जइ (p && !p->handle)
    {
        release();
    }
    प्रत्यागम p != 0;
}

Context::Context(ध्रुव Context& c)
{
    p = (Impl*)c.p;
    जइ(p)
        p->addref();
}

Context& Context::operator = (ध्रुव Context& c)
{
    Impl* newp = (Impl*)c.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::operator = (Context&& c) CV_NOEXCEPT
{
    जइ (एतत् != &c) {
        जइ(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

शून्य* Context::ptr() ध्रुव
{
    प्रत्यागम p == रिक्त ? रिक्त : p->handle;
}

size_t Context::ndevices() ध्रुव
{
    प्रत्यागम p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ध्रुव
{
    स्थिर Device dummy;
    प्रत्यागम !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    जइ (!c.empty())
    {
        auto& ctx = c.getContext();
        प्रत्यागम ctx;
    }

    स्थिर Context dummy;
    प्रत्यागम dummy;
}

Program Context::getProg(ध्रुव ProgramSource& prog,
                         ध्रुव String& buildopts, String& errmsg)
{
    प्रत्यागम p ? p->getProg(prog, buildopts, errmsg) : Program();
}

शून्य Context::unloadProg(Program& prog)
{
    जइ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(शून्य* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    प्रत्यागम ctx;
}

/* static */
Context Context::fromDevice(ध्रुव ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    प्रत्यागम ctx;
}

/* static */
Context Context::create(ध्रुव std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    प्रत्यागम ctx;
}

शून्य* Context::getOpenCLContextProperty(पूर्णाङ्क propertyId) ध्रुव
{
    जइ (p == रिक्त)
        प्रत्यागम nullptr;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, रिक्त, &size));
    std::vector<cl_context_properties> prop(size / परिमाण(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.data(), रिक्त));
    प्रति (size_t i = 0; i < prop.size(); i += 2)
    {
        जइ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(रिक्त, "OpenCL: found context property=" << propertyId << ") => " << (शून्य*)prop[i + 1]);
            प्रत्यागम (शून्य*)prop[i + 1];
        }
    }
    प्रत्यागम nullptr;
}

#ifdef HAVE_OPENCL_SVM
तार्किक Context::useSVM() ध्रुव
{
    Context::Impl* i = p;
    CV_Assert(i);
    जइ (!i->svmInitialized)
        i->svmInit();
    प्रत्यागम i->svmEnabled;
}
शून्य Context::setUseSVM(तार्किक enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    जइ (!i->svmInitialized)
        i->svmInit();
    जइ (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#अण्णहा
तार्किक Context::useSVM() ध्रुव { प्रत्यागम मिच्छ; }
शून्य Context::setUseSVM(तार्किक enabled) { CV_Assert(!enabled); }
#endif

#ifdef HAVE_OPENCL_SVM
नामाकाश svm {

ध्रुव SVMCapabilities getSVMCapabilitites(ध्रुव ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    जइ (!i->svmInitialized)
        i->svmInit();
    प्रत्यागम i->svmCapabilities;
}

CV_EXPORTS ध्रुव SVMFunctions* getSVMFunctions(ध्रुव ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != रिक्त);
    प्रत्यागम &i->svmFunctions;
}

CV_EXPORTS तार्किक useSVM(UMatUsageFlags usageFlags)
{
    जइ (checkForceSVMUmatUsage())
        प्रत्यागम सच्च;
    जइ (checkDisableSVMUMatUsage())
        प्रत्यागम मिच्छ;
    जइ ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        प्रत्यागम सच्च;
    प्रत्यागम मिच्छ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#endif // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

शून्य Context::setUserContext(std::type_index typeId, ध्रुव std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_index typeId)
{
    CV_Assert(p);
    प्रत्यागम p->getUserContext(typeId);
}

स्थिर शून्य get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    AutoBuffer<वर्ण> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.data(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.data();
}

/*
// Attaches OpenCL context to OpenCV
*/
शून्य attachContext(ध्रुव String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID)
{
    auto ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ध्रुव std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID
)
{
    जइ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    जइ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    तार्किक platformAvailable = मिच्छ;

    // check if external platformName contained in list of available platforms in OpenCV
    प्रति (unsigned पूर्णाङ्क i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        जइ (platformName == availablePlatformName)
        {
            platformAvailable = सच्च;
            भङ्ग;
        }
    }

    जइ (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    जइ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    प्रत्यागम ctx;
}

शून्य initializeContextFromHandle(Context& ctx, शून्य* _platform, शून्य* _context, शून्य* _device)
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

संरचना Queue::Impl
{
    अन्तःस्थ शून्य __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = मिच्छ;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, परिमाण(cl_command_queue_properties), &props, रिक्त));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, तार्किक isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(ध्रुव Context& c, ध्रुव Device& d, तार्किक withProfiling = मिच्छ)
    {
        __init();

        ध्रुव Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        जइ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        जइ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#ifdef _WIN32
        जइ (!cv::__termination)
#endif
        {
            जइ(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = रिक्त;
            }
        }
    }

    ध्रुव cv::ocl::Queue& getProfilingQueue(ध्रुव cv::ocl::Queue& self)
    {
        जइ (isProfilingQueue_)
            प्रत्यागम self;

        जइ (profiling_queue_.ptr())
            प्रत्यागम profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, परिमाण(cl_context), &ctx, रिक्त));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, परिमाण(cl_device_id), &device, रिक्त));

        cl_int result = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &result);
        CV_OCL_DBG_CHECK_RESULT(result, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = नूतन Impl(q, सच्च);
        profiling_queue_ = queue;

        प्रत्यागम profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    तार्किक isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(ध्रुव Context& c, ध्रुव Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(ध्रुव Queue& q)
{
    p = q.p;
    जइ(p)
        p->addref();
}

Queue& Queue::operator = (ध्रुव Queue& q)
{
    Impl* newp = (Impl*)q.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::operator = (Queue&& q) CV_NOEXCEPT
{
    जइ (एतत् != &q) {
        जइ(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

Queue::~Queue()
{
    जइ(p)
        p->release();
}

तार्किक Queue::create(ध्रुव Context& c, ध्रुव Device& d)
{
    जइ(p)
        p->release();
    p = नूतन Impl(c, d);
    प्रत्यागम p->handle != 0;
}

शून्य Queue::finish()
{
    जइ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

ध्रुव Queue& Queue::getProfilingQueue() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->getProfilingQueue(*एतत्);
}

शून्य* Queue::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    auto& c = OpenCLExecutionContext::getCurrent();
    जइ (!c.empty())
    {
        auto& q = c.getQueue();
        प्रत्यागम q;
    }
    स्थिर Queue dummy;
    प्रत्यागम dummy;
}

स्थिर cl_command_queue getQueue(ध्रुव Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    जइ(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    प्रत्यागम qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णाङ्क _flags, UMat* _m, पूर्णाङ्क _wscale, पूर्णाङ्क _iwscale, ध्रुव शून्य* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != रिक्त);
}

KernelArg KernelArg::Constant(ध्रुव Mat& m)
{
    CV_Assert(m.isContinuous());
    प्रत्यागम KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.total()*m.elemSize());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

संरचना Kernel::Impl
{
    Impl(ध्रुव वर्ण* kname, ध्रुव Program& prog) :
        refcount(1), handle(रिक्त), isInProgress(मिच्छ), isAsyncRun(मिच्छ), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        जइ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        प्रति( पूर्णाङ्क i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        haveTempDstUMats = मिच्छ;
        haveTempSrcUMats = मिच्छ;
    }

    शून्य cleanupUMats()
    {
        तार्किक exceptionOccurred = मिच्छ;
        प्रति( पूर्णाङ्क i = 0; i < MAX_ARRS; i++ )
        {
            जइ( u[i] )
            {
                जइ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMatData::ASYNC_CLEANUP;
                    यत्न
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    गृहाण(ध्रुव std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(रिक्त, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = सच्च;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        haveTempDstUMats = मिच्छ;
        haveTempSrcUMats = मिच्छ;
        CV_Assert(!exceptionOccurred);
    }

    शून्य addUMat(ध्रुव UMat& m, तार्किक dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        जइ(dst && m.u->tempUMat())
            haveTempDstUMats = सच्च;
        जइ(m.u->originalUMatData == रिक्त && m.u->tempUMat())
            haveTempSrcUMats = सच्च;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    शून्य registerImageArgument(पूर्णाङ्क arg, ध्रुव Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        जइ (arg < (पूर्णाङ्क)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    शून्य finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = मिच्छ;
        यत्न
        {
            cleanupUMats();
        }
        गृहाण(...)
        {
            release();
            क्षिप;
        }
        release();
    }

    तार्किक run(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
            तार्किक समकाल, int64* timeNS, ध्रुव Queue& q);

    ~Impl()
    {
        जइ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    गणना { MAX_ARRS = 16 };
    UMatData* u[MAX_ARRS];
    तार्किक isInProgress;
    तार्किक isAsyncRun;  // true if kernel was scheduled in async mode
    पूर्णाङ्क nu;
    std::vector<Image2D> shadow_images;
    तार्किक haveTempDstUMats;
    तार्किक haveTempSrcUMats;
};

}} // namespace cv::ocl

बाह्य "C" {

स्थिर शून्य CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, शून्य *p)
{
    यत्न
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    गृहाण (ध्रुव cv::Exception& exc)
    {
        CV_LOG_ERROR(रिक्त, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    गृहाण (ध्रुव std::exception& exc)
    {
        CV_LOG_ERROR(रिक्त, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    गृहाण (...)
    {
        CV_LOG_ERROR(रिक्त, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

नामाकाश cv { नामाकाश ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव ProgramSource& src,
               ध्रुव String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ध्रुव Kernel& k)
{
    p = k.p;
    जइ(p)
        p->addref();
}

Kernel& Kernel::operator = (ध्रुव Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::operator = (Kernel&& k) CV_NOEXCEPT
{
    जइ (एतत् != &k) {
        जइ(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

Kernel::~Kernel()
{
    जइ(p)
        p->release();
}

तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव Program& prog)
{
    जइ(p)
        p->release();
    p = नूतन Impl(kname, prog);
    जइ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#ifdef CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#endif
    प्रत्यागम p != 0;
}

तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव ProgramSource& src,
                    ध्रुव String& buildopts, String* errmsg)
{
    जइ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    जइ( !errmsg ) errmsg = &tempmsg;
    ध्रुव Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    प्रत्यागम create(kname, prog);
}

शून्य* Kernel::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

तार्किक Kernel::empty() ध्रुव
{
    प्रत्यागम ptr() == 0;
}

स्थिर cv::String dump_मानम्(size_t sz, ध्रुव शून्य* p)
{
    जइ (!p)
        प्रत्यागम "NULL";
    जइ (sz == 2)
        प्रत्यागम cv::format("%d / %uu / 0x%04x", *(short*)p, *(unsigned short*)p, *(short*)p);
    जइ (sz == 4)
        प्रत्यागम cv::format("%d / %uu / 0x%08x / %g", *(पूर्णाङ्क*)p, *(पूर्णाङ्क*)p, *(पूर्णाङ्क*)p, *(भिन्न*)p);
    जइ (sz == 8)
        प्रत्यागम cv::format("%lld / %lluu / 0x%16llx / %g", *(long long*)p, *(long long*)p, *(long long*)p, *(द्विगुण*)p);
    प्रत्यागम cv::format("%p", p);
}

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव शून्य* value, size_t sz)
{
    जइ (!p || !p->handle)
        प्रत्यागम -1;
    जइ (i < 0)
        प्रत्यागम i;
    जइ( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)sz, dump_मानम्(sz, value).c_str()).c_str());
    जइ (retval != CL_SUCCESS)
        प्रत्यागम -1;
    प्रत्यागम i+1;
}

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    पूर्णाङ्क res = set(i, &h, परिमाण(h));
    जइ (res >= 0)
        p->registerImageArgument(i, image2D);
    प्रत्यागम res;
}

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव UMat& m)
{
    प्रत्यागम set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
स्थिर तार्किक isCudaBacked(ध्रुव UMatData* u)
{
    ध्रुव MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    प्रत्यागम u && cudaAllocator && u->currAllocator == cudaAllocator;
}

पूर्णाङ्क Kernel::set(पूर्णाङ्क i, ध्रुव KernelArg& arg)
{
    जइ( !p || !p->handle )
        प्रत्यागम -1;
    जइ (i < 0)
    {
        CV_LOG_ERROR(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (पूर्णाङ्क)i));
        प्रत्यागम i;
    }
    जइ( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    जइ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        तार्किक ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        जइ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)रिक्त;
            status = clSetKernelArg(p->handle, (cl_uint)i, परिमाण(h_null), &h_null);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (पूर्णाङ्क)i).c_str());
            प्रत्यागम i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        जइ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (पूर्णाङ्क)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            प्रत्यागम -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        जइ (!h)
        {
            CV_LOG_ERROR(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)arg.flags, arg.m));
            p->release();
            p = 0;
            प्रत्यागम -1;
        }

#ifdef HAVE_OPENCL_SVM
        जइ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            ध्रुव Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svmDataPtr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svmDataPtr);
#जइ 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svmDataPtr);
#अण्णहा
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svmDataPtr);
#endif
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (पूर्णाङ्क)i, (शून्य*)svmDataPtr).c_str());
        }
        अण्णहा
#endif
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, परिमाण(h), &h);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (पूर्णाङ्क)i, (शून्य*)h).c_str());
        }

        जइ (ptronly)
        {
            i++;
        }
        अण्णहा जइ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), परिमाण(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), परिमाण(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)u2d.offset).c_str());
            i += 3;

            जइ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                पूर्णाङ्क cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, परिमाण(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), परिमाण(cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)cols).c_str());
                i += 2;
            }
        }
        अण्णहा
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), परिमाण(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), परिमाण(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), परिमाण(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+3), (पूर्णाङ्क)u3d.offset).c_str());
            i += 4;
            जइ( !(arg.flags & KernelArg::NO_SIZE) )
            {
                पूर्णाङ्क cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, परिमाण(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), परिमाण(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), परिमाण(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        प्रत्यागम i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_RESULT(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)arg.sz, (शून्य*)arg.obj).c_str());
    प्रत्यागम i+1;
}

तार्किक Kernel::run(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
                 तार्किक समकाल, ध्रुव Queue& q)
{
    जइ (!p)
        प्रत्यागम मिच्छ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t total = 1;
    CV_Assert(_globalsize != रिक्त);
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(पूर्णाङ्क)(i>0)) : 1;
        CV_Assert( val > 0 );
        total *= _globalsize[i];
        जइ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (unsigned पूर्णाङ्क)val) * val;
    }
    CV_Assert(total > 0);

    प्रत्यागम p->run(dims, globalsize, _localsize, समकाल, रिक्त, q);
}


तार्किक Kernel::run_(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
                  तार्किक समकाल, ध्रुव Queue& q)
{
    CV_Assert(p);
    प्रत्यागम p->run(dims, _globalsize, _localsize, समकाल, रिक्त, q);
}


तार्किक Kernel::Impl::run(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[],
        तार्किक समकाल, int64* timeNS, ध्रुव Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    जइ (!handle)
    {
        CV_LOG_ERROR(रिक्त, "OpenCL kernel has zero handle: " << name);
        प्रत्यागम मिच्छ;
    }

    जइ (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !समकाल;

    जइ (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#जइ CV_OPENCL_SYNC_RUN_KERNELS
    समकाल = सच्च;
#endif

    cl_command_queue qq = getQueue(q);
    जइ (haveTempDstUMats)
        समकाल = सच्च;
    जइ (haveTempSrcUMats)
        समकाल = सच्च;
    जइ (timeNS)
        समकाल = सच्च;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           रिक्त, globalsize, localsize, 0, 0,
                                           (समकाल && !timeNS) ? 0 : &asyncEvent);
#जइ !CV_OPENCL_SHOW_RUN_KERNELS
    जइ (retval != CL_SUCCESS)
#endif
    {
        cv::String सन्देशः = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (पूर्णाङ्क)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        समकाल ? "true" : "false"
                        );
        जइ (retval != CL_SUCCESS)
        {
            सन्देशः = CV_OCL_API_ERROR_सन्देशः(retval, सन्देशः.c_str());
        }
#जइ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_RESULT(retval, सन्देशः.c_str());
#अण्णहा
        printf("%s\n", सन्देशः.c_str());
        fflush(stdout);
#endif
    }
    जइ (समकाल || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        जइ (timeNS)
        {
            जइ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, परिमाण(startTime), &startTime, रिक्त));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, परिमाण(stopTime), &stopTime, रिक्त));
                *timeNS = (int64)(stopTime - startTime);
            }
            अण्णहा
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    अण्णहा
    {
        addref();
        isInProgress = सच्च;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, एतत्));
    }
    जइ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    प्रत्यागम retval == CL_SUCCESS;
}

तार्किक Kernel::runTask(तार्किक समकाल, ध्रुव Queue& q)
{
    जइ(!p || !p->handle || p->isInProgress)
        प्रत्यागम मिच्छ;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, समकाल ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), समकाल ? "true" : "false").c_str());
    जइ (समकाल || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    अण्णहा
    {
        p->addref();
        p->isInProgress = सच्च;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    जइ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    प्रत्यागम retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], ध्रुव Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    तार्किक res = p->run(dims, globalsize, localsize, सच्च, &timeNs, profilingQueue);
    प्रत्यागम res ? timeNs : -1;
}

size_t Kernel::workGroupSize() ध्रुव
{
    जइ(!p || !p->handle)
        प्रत्यागम 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_SIZE, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    प्रत्यागम status == CL_SUCCESS ? val : 0;
}

size_t Kernel::preferedWorkGroupSizeMultiple() ध्रुव
{
    जइ(!p || !p->handle)
        प्रत्यागम 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    प्रत्यागम status == CL_SUCCESS ? val : 0;
}

तार्किक Kernel::compileWorkGroupSize(size_t wsz[]) ध्रुव
{
    जइ(!p || !p->handle || !wsz)
        प्रत्यागम 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_SIZE, परिमाण(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    प्रत्यागम status == CL_SUCCESS;
}

size_t Kernel::localMemSize() ध्रुव
{
    जइ(!p || !p->handle)
        प्रत्यागम 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_SIZE, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_RESULT(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    प्रत्यागम status == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

संरचना ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    गणना KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(ध्रुव String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ध्रुव String& module, ध्रुव String& name, ध्रुव String& codeStr, ध्रुव String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    शून्य init(गणना KIND kind, ध्रुव String& module, ध्रुव String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = रिक्त;
        sourceSize_ = 0;
        isHashUpdated = मिच्छ;
    }

    शून्य initFromSource(ध्रुव String& codeStr, ध्रुव String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        जइ (sourceHash_.empty())
        {
            updateHash();
        }
        अण्णहा
        {
            isHashUpdated = सच्च;
        }
    }

    शून्य updateHash(ध्रुव वर्ण* hashStr = रिक्त)
    {
        जइ (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = सच्च;
            प्रत्यागम;
        }
        uint64 hash = 0;
        विकल्प (kind_)
        {
        पक्ष PROGRAM_SOURCE_CODE:
            जइ (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, sourceSize_); // static storage
            }
            अण्णहा
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            भङ्ग;
        पक्ष PROGRAM_BINARIES:
        पक्ष PROGRAM_SPIR:
        पक्ष PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, sourceSize_);
            भङ्ग;
        स्वभाव:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = सच्च;
    }

    Impl(गणना KIND kind,
            ध्रुव String& module, ध्रुव String& name,
            ध्रुव unsigned वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = binary;
        sourceSize_ = size;

        buildOptions_ = buildOptions;
    }

    स्थिर ProgramSource fromSourceWithStaticLifetime(ध्रुव String& module, ध्रुव String& name,
            ध्रुव वर्ण* sourceCodeStaticStr, ध्रुव वर्ण* hashStaticStr,
            ध्रुव cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = नूतन Impl(PROGRAM_SOURCE_CODE, module, name,
                (ध्रुव unsigned वर्ण*)sourceCodeStaticStr, strlen(sourceCodeStaticStr), buildOptions);
        result.p->updateHash(hashStaticStr);
        प्रत्यागम result;
    }

    स्थिर ProgramSource from_द्विमान(ध्रुव String& module, ध्रुव String& name,
            ध्रुव unsigned वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = नूतन Impl(PROGRAM_BINARIES, module, name, binary, size, buildOptions);
        प्रत्यागम result;
    }

    स्थिर ProgramSource fromSPIR(ध्रुव String& module, ध्रुव String& name,
            ध्रुव unsigned वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions)
    {
        ProgramSource result;
        result.p = नूतन Impl(PROGRAM_SPIR, module, name, binary, size, buildOptions);
        प्रत्यागम result;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    ध्रुव unsigned वर्ण* sourceAddr_;
    size_t sourceSize_;

    cv::String buildOptions_;

    String sourceHash_;
    तार्किक isHashUpdated;

    friend संरचना Program::Impl;
    friend संरचना internal::ProgramEntry;
    friend संरचना Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(ध्रुव String& module, ध्रुव String& name, ध्रुव String& codeStr, ध्रुव String& codeHash)
{
    p = नूतन Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(ध्रुव वर्ण* prog)
{
    p = नूतन Impl(prog);
}

ProgramSource::ProgramSource(ध्रुव String& prog)
{
    p = नूतन Impl(prog);
}

ProgramSource::~ProgramSource()
{
    जइ(p)
        p->release();
}

ProgramSource::ProgramSource(ध्रुव ProgramSource& prog)
{
    p = prog.p;
    जइ(p)
        p->addref();
}

ProgramSource& ProgramSource::operator = (ध्रुव ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::operator = (ProgramSource&& prog) CV_NOEXCEPT
{
    जइ (एतत् != &prog) {
        जइ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

ध्रुव String& ProgramSource::source() ध्रुव
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == रिक्त); // method returns reference - can't construct temporary object
    प्रत्यागम p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() ध्रुव
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_द्विमान(ध्रुव String& module, ध्रुव String& name,
        ध्रुव unsigned वर्ण* binary, ध्रुव size_t size,
        ध्रुव cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    प्रत्यागम Impl::from_द्विमान(module, name, binary, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(ध्रुव String& module, ध्रुव String& name,
        ध्रुव unsigned वर्ण* binary, ध्रुव size_t size,
        ध्रुव cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    प्रत्यागम Impl::from_द्विमान(module, name, binary, size, buildOptions);
}


internal::ProgramEntry::operator ProgramSource&() ध्रुव
{
    जइ (एतत्->pProgramSource == रिक्त)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        जइ (एतत्->pProgramSource == रिक्त)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(एतत्->module, एतत्->name, एतत्->programCode, एतत्->programHash, cv::String());
            ProgramSource* ptr = नूतन ProgramSource(ps);
            const_cast<ProgramEntry*>(एतत्)->pProgramSource = ptr;
        }
    }
    प्रत्यागम *एतत्->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

स्थिर
cv::String joinBuildOptions(ध्रुव cv::String& a, ध्रुव cv::String& b)
{
    जइ (b.empty())
        प्रत्यागम a;
    जइ (a.empty())
        प्रत्यागम b;
    जइ (b[0] == ' ')
        प्रत्यागम a + b;
    प्रत्यागम a + (cv::String(" ") + b);
}

संरचना Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ध्रुव ProgramSource& src,
         ध्रुव String& _buildflags, String& errmsg) :
         refcount(1),
         handle(रिक्त),
         buildflags(_buildflags)
    {
        ध्रुव ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ध्रुव Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        जइ (ctx.ptr() == रिक्त || device.ptr() == रिक्त)
            प्रत्यागम;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        जइ (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            जइ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            अण्णहा जइ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ध्रुव String param_buildExtraOptions = getBuildExtraOptions();
            जइ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#जइ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#endif
        compile(ctx, src_, errmsg);
#जइ CV_OPENCL_SHOW_BUILD_KERNELS
        जइ (handle)
        {
            size_t retsz = 0;
            वर्ण kernels_buffer[4096] = {0};
            cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, परिमाण(kernels_buffer), &kernels_buffer[0], &retsz);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            जइ (result == CL_SUCCESS && retsz < परिमाण(kernels_buffer))
            {
                kernels_buffer[retsz] = 0;
                CV_LOG_INFO(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_buffer << "'");
            }
            अण्णहा
            {
                CV_LOG_ERROR(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#endif
    }

    तार्किक compile(ध्रुव Context& ctx, ध्रुव ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        जइ (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(रिक्त, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            तार्किक isLoaded = create_From_द्विमान(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            प्रत्यागम isLoaded;
        }
        प्रत्यागम compileWithCache(ctx, src_, errmsg);
    }

    तार्किक compileWithCache(ध्रुव Context& ctx, ध्रुव ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ध्रुव std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        ध्रुव String& hash_str = src_->sourceHash_;
        cv::String fname;
        जइ (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        ध्रुव cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        जइ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            यत्न
            {
                std::vector<वर्ण> द्विमान_Buf;
                तार्किक res = मिच्छ;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    द्विमान_Program_File file(fname, hash_str.c_str());
                    res = file.पठ(buildflags, द्विमान_Buf);
                }
                जइ (res)
                {
                    CV_Assert(!द्विमान_Buf.empty());
                    CV_LOG_VERBOSE(रिक्त, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    तार्किक isLoaded = create_From_द्विमान(ctx, द्विमान_Buf, errmsg);
                    जइ (isLoaded)
                        प्रत्यागम सच्च;
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(रिक्त, 0, "Can't load OpenCL binary: " + fname << std::endl << e.what());
            }
            गृहाण (...)
            {
                CV_LOG_VERBOSE(रिक्त, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == रिक्त);
        जइ (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            जइ (!buildFromSources(ctx, src_, errmsg))
            {
                प्रत्यागम मिच्छ;
            }
        }
        अण्णहा जइ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            जइ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(रिक्त, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            तार्किक isLoaded = create_From_द्विमान(ctx, src_->sourceAddr_, src_->sourceSize_, errmsg);
            जइ (!isLoaded)
                प्रत्यागम मिच्छ;
        }
        अण्णहा जइ (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        अण्णहा
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != रिक्त);
#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
        जइ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            यत्न
            {
                std::vector<वर्ण> द्विमान_Buf;
                get_Program_द्विमान(द्विमान_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.get());
                    द्विमान_Program_File file(fname, hash_str.c_str());
                    file.लिख(buildflags, द्विमान_Buf);
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_WARNING(रिक्त, "Can't save OpenCL binary into cache: " + fname << std::endl << e.what());
            }
            गृहाण (...)
            {
                CV_LOG_WARNING(रिक्त, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
#जइ CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
        जइ (CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
        {
            std::vector<वर्ण> द्विमान_Buf;
            get_Program_द्विमान(द्विमान_Buf);
            जइ (!द्विमान_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = रिक्त;
                create_From_द्विमान(ctx, द्विमान_Buf, errmsg);
            }
        }
#endif
        प्रत्यागम handle != रिक्त;
    }

    शून्य dumpBuildLog_(cl_int result, ध्रुव cl_device_id* deviceList, String& errmsg)
    {
        AutoBuffer<वर्ण, 4096> buffer; buffer[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        जइ (log_retval == CL_SUCCESS && retsz > 1)
        {
            buffer.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, buffer.data(), &retsz);
            जइ (log_retval == CL_SUCCESS)
            {
                जइ (retsz < buffer.size())
                    buffer[retsz] = 0;
                अण्णहा
                    buffer[buffer.size() - 1] = 0;
            }
            अण्णहा
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

    तार्किक buildFromSources(ध्रुव Context& ctx, ध्रुव ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == रिक्त);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(रिक्त, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ध्रुव वर्ण* srcptr = src_->sourceAddr_ ? ((ध्रुव वर्ण*)src_->sourceAddr_) : src_->codeStr_.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->sourceSize_ : src_->codeStr_.size();
        CV_Assert(srcptr != रिक्त);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_RESULT(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        जइ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            AutoBuffer<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.data();
            प्रति (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_RESULT(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#जइ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            जइ (retval != CL_SUCCESS)
#endif
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                जइ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = रिक्त;
                }
                जइ (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#जइ CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
            जइ (handle && CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
            {
                CV_LOG_INFO(रिक्त, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                वर्ण kernels_buffer[4096] = {0};
                cl_int result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, परिमाण(kernels_buffer), &kernels_buffer[0], &retsz);
                जइ (retsz < परिमाण(kernels_buffer))
                    kernels_buffer[retsz] = 0;
                अण्णहा
                    kernels_buffer[0] = 0;
                CV_LOG_INFO(रिक्त, result << ": Kernels='" << kernels_buffer << "'");
            }
#endif
        }
        प्रत्यागम handle != रिक्त;
    }

    शून्य get_Program_द्विमान(std::vector<वर्ण>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_द्विमान_SIZES, परिमाण(sz), &sz, रिक्त));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, परिमाण(ptr), &ptr, रिक्त));
    }

    तार्किक create_From_द्विमान(ध्रुव Context& ctx, ध्रुव std::vector<वर्ण>& buf, String& errmsg)
    {
        प्रत्यागम create_From_द्विमान(ctx, (ध्रुव unsigned वर्ण*)&buf[0], buf.size(), errmsg);
    }

    तार्किक create_From_द्विमान(ध्रुव Context& ctx, ध्रुव unsigned वर्ण* द्विमान_Addr, ध्रुव size_t द्विमान_Size, String& errmsg)
    {
        CV_Assert(handle == रिक्त);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(रिक्त, 0, "Load from binary... (" << द्विमान_Size << " bytes)");

        CV_Assert(द्विमान_Size > 0);

        size_t ndevices = (पूर्णाङ्क)ctx.ndevices();
        AutoBuffer<cl_device_id> devices_(ndevices);
        AutoBuffer<ध्रुव uchar*> द्विमान_Ptrs(ndevices);
        AutoBuffer<size_t> द्विमान_Sizes(ndevices);

        cl_device_id* devices = devices_.data();
        ध्रुव uchar** द्विमान_Ptrs_2 = द्विमान_Ptrs.data();
        size_t* द्विमान_Sizes_2 = द्विमान_Sizes.data();
        प्रति (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            द्विमान_Ptrs_2[i] = द्विमान_Addr;
            द्विमान_Sizes_2[i] = द्विमान_Size;
        }

        cl_int result = 0;
        handle = cl_Create_Program_With_द्विमान((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.data(),
                                           द्विमान_Sizes_2, द्विमान_Ptrs_2, रिक्त, &result);
        जइ (result != CL_SUCCESS)
        {
            CV_LOG_ERROR(रिक्त, CV_OCL_API_ERROR_सन्देशः(result, "clCreateProgramWithBinary"));
            जइ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = रिक्त;
            }
        }
        जइ (!handle)
        {
            प्रत्यागम मिच्छ;
        }
        // call clBuildProgram()
        {
            result = clBuildProgram(handle, (cl_uint)ndevices, devices_.data(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_RESULT(result, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            जइ (result != CL_SUCCESS)
            {
                dumpBuildLog_(result, devices, errmsg);
                जइ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = रिक्त;
                }
                प्रत्यागम मिच्छ;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(result = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    परिमाण(build_status), &build_status, &retsz));
            जइ (result == CL_SUCCESS)
            {
                जइ (build_status == CL_BUILD_SUCCESS)
                {
                    प्रत्यागम सच्च;
                }
                अण्णहा
                {
                    CV_LOG_WARNING(रिक्त, "clGetProgramBuildInfo() returns " << build_status);
                    प्रत्यागम मिच्छ;
                }
            }
            अण्णहा
            {
                CV_LOG_ERROR(रिक्त, CV_OCL_API_ERROR_सन्देशः(result, "clGetProgramBuildInfo()"));
                जइ (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = रिक्त;
                }
            }
        }
#जइ CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
        जइ (handle && CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            वर्ण kernels_buffer[4096] = {0};
            result = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, परिमाण(kernels_buffer), &kernels_buffer[0], &retsz);
            जइ (retsz < परिमाण(kernels_buffer))
                kernels_buffer[retsz] = 0;
            अण्णहा
                kernels_buffer[0] = 0;
            CV_LOG_INFO(रिक्त, result << ": Kernels='" << kernels_buffer << "'");
        }
#endif
        प्रत्यागम handle != रिक्त;
    }

    ~Impl()
    {
        जइ( handle )
        {
#ifdef _WIN32
            जइ (!cv::__termination)
#endif
            {
                clReleaseProgram(handle);
            }
            handle = रिक्त;
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

Program::Program(ध्रुव ProgramSource& src,
        ध्रुव String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(ध्रुव Program& prog)
{
    p = prog.p;
    जइ(p)
        p->addref();
}

Program& Program::operator = (ध्रुव Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    जइ(newp)
        newp->addref();
    जइ(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::operator = (Program&& prog) CV_NOEXCEPT
{
    जइ (एतत् != &prog) {
        जइ(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

Program::~Program()
{
    जइ(p)
        p->release();
}

तार्किक Program::create(ध्रुव ProgramSource& src,
            ध्रुव String& buildflags, String& errmsg)
{
    जइ(p)
    {
        p->release();
        p = रिक्त;
    }
    p = नूतन Impl(src, buildflags, errmsg);
    जइ(!p->handle)
    {
        p->release();
        p = 0;
    }
    प्रत्यागम p != 0;
}

शून्य* Program::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

#ifndef OPENCV_REMOVE_DEPRECATED_API
ध्रुव ProgramSource& Program::source() ध्रुव
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

तार्किक Program::पठ(ध्रुव String& bin, ध्रुव String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

तार्किक Program::लिख(String& bin) ध्रुव
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() ध्रुव
{
    जइ(!p)
        प्रत्यागम String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    प्रत्यागम cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(ध्रुव String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        प्रत्यागम cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#endif // OPENCV_REMOVE_DEPRECATED_API

शून्य Program::get_द्विमान(std::vector<वर्ण>& binary) ध्रुव
{
    CV_Assert(p && "Empty program");
    p->get_Program_द्विमान(binary);
}

Program Context::Impl::getProg(ध्रुव ProgramSource& src,
                               ध्रुव String& buildflags, String& errmsg)
{
    size_t limit = get_Program_गणनम्_Limit();
    ध्रुव ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String कुञ्चिका = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(कुञ्चिका);
        जइ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), कुञ्चिका);
            जइ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(कुञ्चिका);
            }
            प्रत्यागम it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            जइ (limit > 0 && sz >= limit)
            {
                स्थिर तार्किक warningFlag = मिच्छ;
                जइ (!warningFlag)
                {
                    printf("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = सच्च;
                }
                जाव (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    जइ (c != 0)
                        भङ्ग;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(कुञ्चिका, prog));
        cacheList.push_front(कुञ्चिका);
    }
    प्रत्यागम prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

template<typename T>
वर्ग OpenCLBufferPool
{
रक्षित:
    ~OpenCLBufferPool() { }
सार्वजनिक:
    आभास T allocate(size_t size) = 0;
    आभास शून्य release(T buffer) = 0;
};

template <typename Derived, typename BufferEntry, typename T>
वर्ग OpenCLBufferPoolBaseImpl : सार्वजनिक BufferPoolController, सार्वजनिक OpenCLBufferPool<T>
{
गुप्त:
    अन्तःस्थ Derived& derived() { प्रत्यागम *static_cast<Derived*>(एतत्); }
रक्षित:
    Mutex mutex_;

    size_t currentReservedSize;
    size_t maxReservedSize;

    std::list<BufferEntry> allocatedEntries_; // Allocated and used entries
    std::list<BufferEntry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    तार्किक _findAndRemoveEntryFromAllocatedList(CV_OUT BufferEntry& entry, T buffer)
    {
        typename std::list<BufferEntry>::iterator i = allocatedEntries_.begin();
        प्रति (; i != allocatedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            जइ (e.clBuffer_ == buffer)
            {
                entry = e;
                allocatedEntries_.erase(i);
                प्रत्यागम सच्च;
            }
        }
        प्रत्यागम मिच्छ;
    }

    // synchronized
    तार्किक _findAndRemoveEntryFromReservedList(CV_OUT BufferEntry& entry, ध्रुव size_t size)
    {
        जइ (reservedEntries_.empty())
            प्रत्यागम मिच्छ;
        typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
        typename std::list<BufferEntry>::iterator result_pos = reservedEntries_.end();
        BufferEntry result;
        size_t minDiff = (size_t)(-1);
        प्रति (; i != reservedEntries_.end(); ++i)
        {
            BufferEntry& e = *i;
            जइ (e.capacity_ >= size)
            {
                size_t diff = e.capacity_ - size;
                जइ (diff < std::max((size_t)4096, size / 8) && (result_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    result_pos = i;
                    result = e;
                    जइ (diff == 0)
                        भङ्ग;
                }
            }
        }
        जइ (result_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(result_pos);
            entry = result;
            currentReservedSize -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            प्रत्यागम सच्च;
        }
        प्रत्यागम मिच्छ;
    }

    // synchronized
    शून्य _checkSizeOfReservedEntries()
    {
        जाव (currentReservedSize > maxReservedSize)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ध्रुव BufferEntry& entry = reservedEntries_.back();
            CV_DbgAssert(currentReservedSize >= entry.capacity_);
            currentReservedSize -= entry.capacity_;
            derived()._releaseBufferEntry(entry);
            reservedEntries_.pop_back();
        }
    }

    अन्तःस्थ size_t _allocationGranularity(size_t size)
    {
        // heuristic values
        जइ (size < 1024*1024)
            प्रत्यागम 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        अण्णहा जइ (size < 16*1024*1024)
            प्रत्यागम 64*1024;
        अण्णहा
            प्रत्यागम 1024*1024;
    }

सार्वजनिक:
    OpenCLBufferPoolBaseImpl()
        : currentReservedSize(0),
          maxReservedSize(0)
    {
        // nothing
    }
    आभास ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
सार्वजनिक:
    आभास T allocate(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        जइ (maxReservedSize > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_BUFFER_POOL("Reuse reserved buffer: %p\n", entry.clBuffer_);
        }
        अण्णहा
        {
            derived()._allocateBufferEntry(entry, size);
        }
        प्रत्यागम entry.clBuffer_;
    }
    आभास शून्य release(T buffer) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        BufferEntry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, buffer));
        जइ (maxReservedSize == 0 || entry.capacity_ > maxReservedSize / 8)
        {
            derived()._releaseBufferEntry(entry);
        }
        अण्णहा
        {
            reservedEntries_.push_front(entry);
            currentReservedSize += entry.capacity_;
            _checkSizeOfReservedEntries();
        }
    }

    आभास size_t getReservedSize() ध्रुव CV_OVERRIDE { प्रत्यागम currentReservedSize; }
    आभास size_t getMaxReservedSize() ध्रुव CV_OVERRIDE { प्रत्यागम maxReservedSize; }
    आभास शून्य setMaxReservedSize(size_t size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        size_t oldMaxReservedSize = maxReservedSize;
        maxReservedSize = size;
        जइ (maxReservedSize < oldMaxReservedSize)
        {
            typename std::list<BufferEntry>::iterator i = reservedEntries_.begin();
            प्रति (; i != reservedEntries_.end();)
            {
                ध्रुव BufferEntry& entry = *i;
                जइ (entry.capacity_ > maxReservedSize / 8)
                {
                    CV_DbgAssert(currentReservedSize >= entry.capacity_);
                    currentReservedSize -= entry.capacity_;
                    derived()._releaseBufferEntry(entry);
                    i = reservedEntries_.erase(i);
                    अनुवर्त;
                }
                ++i;
            }
            _checkSizeOfReservedEntries();
        }
    }
    आभास शून्य freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<BufferEntry>::const_iterator i = reservedEntries_.begin();
        प्रति (; i != reservedEntries_.end(); ++i)
        {
            ध्रुव BufferEntry& entry = *i;
            derived()._releaseBufferEntry(entry);
        }
        reservedEntries_.clear();
        currentReservedSize = 0;
    }
};

संरचना CLBufferEntry
{
    cl_mem clBuffer_;
    size_t capacity_;
    CLBufferEntry() : clBuffer_((cl_mem)रिक्त), capacity_(0) { }
};

वर्ग OpenCLBufferPoolImpl CV_FINAL : सार्वजनिक OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
सार्वजनिक:
    प्रकारनिर्देश संरचना CLBufferEntry BufferEntry;
रक्षित:
    पूर्णाङ्क createFlags_;
सार्वजनिक:
    OpenCLBufferPoolImpl(पूर्णाङ्क createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    शून्य _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == रिक्त);
        entry.capacity_ = alignSize(size, (पूर्णाङ्क)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.clBuffer_ = clCreateBuffer((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_RESULT(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (long long पूर्णाङ्क)entry.capacity_, (शून्य*)entry.clBuffer_).c_str());
        CV_Assert(entry.clBuffer_ != रिक्त);
        जइ(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    शून्य _releaseBufferEntry(ध्रुव BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != रिक्त);
        LOG_BUFFER_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.clBuffer_));
    }
};

#ifdef HAVE_OPENCL_SVM
संरचना CLSVMBufferEntry
{
    शून्य* clBuffer_;
    size_t capacity_;
    CLSVMBufferEntry() : clBuffer_(रिक्त), capacity_(0) { }
};
वर्ग OpenCLSVMBufferPoolImpl CV_FINAL : सार्वजनिक OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, शून्य*>
{
सार्वजनिक:
    प्रकारनिर्देश संरचना CLSVMBufferEntry BufferEntry;
सार्वजनिक:
    OpenCLSVMBufferPoolImpl()
    {
    }

    शून्य _allocateBufferEntry(BufferEntry& entry, size_t size)
    {
        CV_DbgAssert(entry.clBuffer_ == रिक्त);
        entry.capacity_ = alignSize(size, (पूर्णाङ्क)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        ध्रुव svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        तार्किक isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (पूर्णाङ्क)entry.capacity_);
        शून्य *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.clBuffer_ = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_BUFFER_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (long long)entry.capacity_, (long long)entry.capacity_, entry.clBuffer_);
        allocatedEntries_.push_back(entry);
    }

    शून्य _releaseBufferEntry(ध्रुव BufferEntry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.clBuffer_ != रिक्त);
        LOG_BUFFER_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.clBuffer_, (long long)entry.capacity_, (long long)entry.capacity_);
        Context& ctx = Context::getDefault();
        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.clBuffer_);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.clBuffer_);
    }
};
#endif



template <तार्किक readAccess, तार्किक writeAccess>
वर्ग AlignedDataPtr
{
रक्षित:
    ध्रुव size_t size_;
    uchar* ध्रुव originPtr_;
    ध्रुव size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

सार्वजनिक:
    AlignedDataPtr(uchar* ptr, size_t size, size_t alignment)
        : size_(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(रिक्त)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        जइ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नूतन uchar[size_ + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            जइ (readAccess)
            {
                memcpy(ptr_, originPtr_, size_);
            }
        }
    }

    uchar* getAlignedPtr() ध्रुव
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        प्रत्यागम ptr_;
    }

    ~AlignedDataPtr()
    {
        जइ (allocatedPtr_)
        {
            जइ (writeAccess)
            {
                memcpy(originPtr_, ptr_, size_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = रिक्त;
        }
        ptr_ = रिक्त;
    }
गुप्त:
    AlignedDataPtr(ध्रुव AlignedDataPtr&); // disabled
    AlignedDataPtr& operator=(ध्रुव AlignedDataPtr&); // disabled
};

template <तार्किक readAccess, तार्किक writeAccess>
वर्ग AlignedDataPtr2D
{
रक्षित:
    ध्रुव size_t size_;
    uchar* ध्रुव originPtr_;
    ध्रुव size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t rows_;
    size_t cols_;
    size_t step_;

सार्वजनिक:
    AlignedDataPtr2D(uchar* ptr, size_t rows, size_t cols, size_t step, size_t alignment, size_t extrabytes=0)
        : size_(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(रिक्त), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != रिक्त);
        जइ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नूतन uchar[size_ + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            जइ (readAccess)
            {
                प्रति (size_t i = 0; i < rows_; i++)
                    memcpy(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() ध्रुव
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        प्रत्यागम ptr_;
    }

    ~AlignedDataPtr2D()
    {
        जइ (allocatedPtr_)
        {
            जइ (writeAccess)
            {
                प्रति (size_t i = 0; i < rows_; i++)
                    memcpy(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            delete[] allocatedPtr_;
            allocatedPtr_ = रिक्त;
        }
        ptr_ = रिक्त;
    }
गुप्त:
    AlignedDataPtr2D(ध्रुव AlignedDataPtr2D&); // disabled
    AlignedDataPtr2D& operator=(ध्रुव AlignedDataPtr2D&); // disabled
};

#ifndef CV_OPENCL_DATA_PTR_ALIGNMENT
#define CV_OPENCL_DATA_PTR_ALIGNMENT 16
#endif


शून्य Context::Impl::__init_buffer_pools()
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

    CV_LOG_INFO(रिक्त, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << poolSize << " poolSizeHostPtr=" << poolSizeHostPtr);
}

वर्ग OpenCLAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    गणना AllocatorFlags
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

    UMatData* defaultAllocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* data, size_t* step,
            AccessFlag flags, UMatUsageFlags usageFlags) ध्रुव
    {
        UMatData* u = matStdAllocator->allocate(dims, sizes, type, data, step, flags, usageFlags);
        प्रत्यागम u;
    }

    स्थिर तार्किक isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        स्थिर तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", मिच्छ);
        प्रत्यागम value;
    }
    स्थिर तार्किक isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        स्थिर तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", मिच्छ);
        प्रत्यागम value;
    }

    शून्य getBestFlags(ध्रुव Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, पूर्णाङ्क& createFlags, UMatData::MemoryFlag& flags0) ध्रुव
    {
        ध्रुव Device& dev = ctx.device(0);
        createFlags = 0;
        जइ ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        जइ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#ifndef __APPLE__
                || dev.isIntel()
#endif
                )
            )
        )
            flags0 = static_cast<UMatData::MemoryFlag>(0);
        अण्णहा
            flags0 = UMatData::COPY_ON_MAP;
    }

    UMatData* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* data, size_t* step, AccessFlag flags, UMatUsageFlags usageFlags) ध्रुव CV_OVERRIDE
    {
        जइ(!useOpenCL())
            प्रत्यागम defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(data == 0);
        size_t total = CV_ELEM_SIZE(type);
        प्रति( पूर्णाङ्क i = dims-1; i >= 0; i-- )
        {
            जइ( step )
                step[i] = total;
            total *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        जइ (!ctx.getImpl())
            प्रत्यागम defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        पूर्णाङ्क createFlags = 0;
        UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        शून्य* handle = रिक्त;
        पूर्णाङ्क allocatorFlags = 0;

#ifdef HAVE_OPENCL_SVM
        ध्रुव svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        जइ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED;
            handle = ctxImpl.getBufferPoolSVM().allocate(total);

            // this property is constant, so single buffer pool can be used here
            तार्किक isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();
            allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
        }
        अण्णहा
#endif
        जइ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_USED;
            handle = ctxImpl.getBufferPool().allocate(total);
        }
        अण्णहा जइ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED;
            handle = ctxImpl.getBufferPoolHostPtr().allocate(total);
        }
        अण्णहा
        {
            CV_Assert(handle != रिक्त); // Unsupported, throw
        }

        जइ (!handle)
            प्रत्यागम defaultAllocate(dims, sizes, type, data, step, flags, usageFlags);

        UMatData* u = नूतन UMatData(एतत्);
        u->data = 0;
        u->size = total;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<शून्य>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->tempUMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(सच्च);
        opencl_allocator_stats.onAllocate(u->size);
        प्रत्यागम u;
    }

    तार्किक allocate(UMatData* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ध्रुव CV_OVERRIDE
    {
        जइ(!u)
            प्रत्यागम मिच्छ;

        flushCleanupQueue();

        UMatDataAutoLock lock(u);

        जइ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            पूर्णाङ्क createFlags = 0;
            UMatData::MemoryFlag flags0 = static_cast<UMatData::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            तार्किक copyOnMap = (flags0 & UMatData::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            पूर्णाङ्क allocatorFlags = 0;
            UMatData::MemoryFlag tempUMatFlags = static_cast<UMatData::MemoryFlag>(0);
            शून्य* handle = रिक्त;
            cl_int retval = CL_SUCCESS;

#ifdef HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            तार्किक useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            जइ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                tempUMatFlags = UMatData::TEMP_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (पूर्णाङ्क)u->size, handle);
            }
            अण्णहा जइ (useSVM && (svmCaps.isSupportFineGrainBuffer() || svmCaps.isSupportCoarseGrainBuffer()))
            {
                जइ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    तार्किक isFineGrainBuffer = svmCaps.isSupportFineGrainBuffer();

                    cl_svm_mem_flags memFlags = createFlags |
                            (isFineGrainBuffer ? CL_MEM_SVM_FINE_GRAIN_BUFFER : 0);

                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (पूर्णाङ्क)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = रिक्त;
                    जइ (!isFineGrainBuffer)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (पूर्णाङ्क)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, रिक्त, रिक्त);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");

                    }
                    memcpy(handle, u->origdata, u->size);
                    जइ (!isFineGrainBuffer)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, रिक्त, रिक्त);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }

                    tempUMatFlags = UMatData::TEMP_UMAT | UMatData::TEMP_COPIED_UMAT;
                    allocatorFlags |= isFineGrainBuffer ? svm::OPENCL_SVM_FINE_GRAIN_BUFFER
                                                : svm::OPENCL_SVM_COARSE_GRAIN_BUFFER;
                }
            }
            अण्णहा
#endif
            {
                जइ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                tempUMatFlags = UMatData::TEMP_UMAT;
                जइ (
                #ifdef __APPLE__
                    !copyOnMap &&
                #endif
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (पूर्णाङ्क)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->originalUMatData && u->originalUMatData->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = clCreateBuffer(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (long long पूर्णाङ्क)u->size, u->origdata, (शून्य*)handle).c_str());
                }
                जइ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = clCreateBuffer(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (long long पूर्णाङ्क)u->size, u->origdata, (शून्य*)handle).c_str());
                    tempUMatFlags |= UMatData::TEMP_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clCreateBuffer() => %p", (शून्य*)handle).c_str());
            जइ(!handle || retval != CL_SUCCESS)
                प्रत्यागम मिच्छ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = एतत्;
            u->flags |= tempUMatFlags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        जइ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(सच्च);
        opencl_allocator_stats.onAllocate(u->size);
        प्रत्यागम सच्च;
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

    शून्य deallocate(UMatData* u) ध्रुव CV_OVERRIDE
    {
        जइ(!u)
            प्रत्यागम;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        जइ (!!(u->flags & UMatData::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        अण्णहा
            deallocate_(u);
    }

    शून्य deallocate_(UMatData* u) ध्रुव
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        जइ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_BUFFER) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#ifdef _WIN32
        जइ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            प्रत्यागम;             // avoid any OpenCL calls
#endif
        जइ(u->tempUMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            जइ (u->hostCopyObsolete())
            {
#ifdef HAVE_OPENCL_SVM
                जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    जइ( u->tempCopiedUMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER);
                        तार्किक isFineGrainBuffer = (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER;
                        cl_command_queue q = रिक्त;
                        जइ (!isFineGrainBuffer)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, रिक्त, रिक्त);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        memcpy(u->origdata, u->handle, u->size);
                        जइ (!isFineGrainBuffer)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, रिक्त, रिक्त);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    अण्णहा
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                अण्णहा
#endif
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    जइ( u->tempCopiedUMat() )
                    {
                        AlignedDataPtr<मिच्छ, सच्च> alignedPtr(u->origdata, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
                        CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    अण्णहा
                    {
                        cl_int retval = 0;
                        जइ (u->tempUMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            शून्य* data = clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (शून्य*)u->handle, (long long पूर्णाङ्क)u->size, data).c_str());
                            CV_Assert(u->origdata == data && "Details: https://github.com/opencv/opencv/issues/6293");
                            जइ (u->originalUMatData)
                            {
                                CV_Assert(u->originalUMatData->data == data);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, data, 0, 0, 0);
                            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (शून्य*)u->handle, data, (long long पूर्णाङ्क)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(मिच्छ);
            }
            अण्णहा
            {
                // nothing
            }
#ifdef HAVE_OPENCL_SVM
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                जइ( u->tempCopiedUMat() )
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            अण्णहा
#endif
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clReleaseMemObject(ptr=%p)", (शून्य*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(सच्च);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = रिक्त;
            जइ(u->data && u->copyOnMap() && u->data != u->origdata)
                fastFree(u->data);
            u->data = u->origdata;
            u->currAllocator->deallocate(u);
            u = रिक्त;
        }
        अण्णहा
        {
            CV_Assert(u->origdata == रिक्त);
            जइ(u->data && u->copyOnMap() && u->data != u->origdata)
            {
                fastFree(u->data);
                u->data = 0;
                u->markHostCopyObsolete(सच्च);
            }
            जइ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPool().release((cl_mem)u->handle);
            }
            अण्णहा जइ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolHostPtr().release((cl_mem)u->handle);
            }
#ifdef HAVE_OPENCL_SVM
            अण्णहा जइ (u->allocatorFlags_ & ALLOCATOR_FLAGS_BUFFER_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                अण्णहा जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, रिक्त, रिक्त);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->getBufferPoolSVM().release((शून्य*)u->handle);
            }
#endif
            अण्णहा
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(सच्च);
            delete u;
            u = रिक्त;
        }
        CV_Assert(u == रिक्त);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    शून्य map(UMatData* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        जइ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सच्च);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            जइ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#ifdef HAVE_OPENCL_SVM
                जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                    {
                        Context& ctx = Context::getDefault();
                        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, रिक्त, रिक्त);
                            CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_BUFFER_MAP;
                        }
                    }
                    clFinish(q);
                    u->data = (uchar*)u->handle;
                    u->markHostCopyObsolete(मिच्छ);
                    u->markDeviceMemMapped(सच्च);
                    प्रत्यागम;
                }
#endif

                cl_int retval = CL_SUCCESS;
                जइ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->data = (uchar*)clEnqueueMapBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_RESULT(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (शून्य*)u->handle, (long long पूर्णाङ्क)u->size, u->data).c_str());
                }
                जइ (u->data && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(मिच्छ);
                    u->markDeviceMemMapped(सच्च);
                    प्रत्यागम;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMatData::COPY_ON_MAP;
            }

            जइ(!u->data)
            {
                u->data = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(सच्च);
            }
        }

        जइ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            AlignedDataPtr<मिच्छ, सच्च> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            cl_int retval = clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (शून्य*)u->handle, (long long पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(मिच्छ);
        }
    }

    शून्य unmap(UMatData* u) ध्रुव CV_OVERRIDE
    {
        जइ(!u)
            प्रत्यागम;


        CV_Assert(u->handle != 0);

        UMatDataAutoLock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        जइ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->data != रिक्त);
#ifdef HAVE_OPENCL_SVM
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, रिक्त, रिक्त);
                        CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_BUFFER_MAP;
                    }
                }
                जइ (u->refcount == 0)
                    u->data = 0;
                u->markDeviceCopyObsolete(मिच्छ);
                u->markHostCopyObsolete(सच्च);
                प्रत्यागम;
            }
#endif
            जइ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->data, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (शून्य*)u->handle, u->data, (long long पूर्णाङ्क)u->size).c_str());
                जइ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(मिच्छ);
                u->data = 0;
                u->markDeviceCopyObsolete(मिच्छ);
                u->markHostCopyObsolete(सच्च);
            }
        }
        अण्णहा जइ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            AlignedDataPtr<सच्च, मिच्छ> alignedPtr(u->data, u->size, CV_OPENCL_DATA_PTR_ALIGNMENT);
#ifdef HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == 0);
#endif
            retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (शून्य*)u->handle, (long long पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(मिच्छ);
            u->markHostCopyObsolete(सच्च);
        }
    }

    तार्किक checkContinuous(पूर्णाङ्क dims, ध्रुव size_t sz[],
                         ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                         ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                         size_t& total, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ध्रुव
    {
        तार्किक iscontinuous = सच्च;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        total = sz[dims-1];
        प्रति( पूर्णाङ्क i = dims-2; i >= 0; i-- )
        {
            जइ( i >= 0 && (total != srcstep[i] || total != dststep[i]) )
                iscontinuous = मिच्छ;
            total *= sz[i];
            जइ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            जइ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        जइ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            जइ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                जइ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                जइ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            अण्णहा
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                जइ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                जइ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        प्रत्यागम iscontinuous;
    }

    शून्य download(UMatData* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dststep[]) ध्रुव CV_OVERRIDE
    {
        जइ(!u)
            प्रत्यागम;
        UMatDataAutoLock autolock(u);

        जइ( u->data && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            प्रत्यागम;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#ifdef HAVE_OPENCL_SVM
        जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == रिक्त || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            जइ( iscontinuous )
            {
                memcpy(dstptr, (uchar*)u->handle + srcrawofs, total);
            }
            अण्णहा
            {
                // This code is from MatAllocator::download()
                पूर्णाङ्क isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    जइ( sz[i] == 0 )
                    प्रत्यागम;
                    जइ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (पूर्णाङ्क)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ध्रुव Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                प्रति( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अण्णहा
#endif
        {
            जइ( iscontinuous )
            {
                AlignedDataPtr<मिच्छ, सच्च> alignedPtr((uchar*)dstptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            अण्णहा जइ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                AlignedDataPtr2D<मिच्छ, मिच्छ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                total = alignSize(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                total = std::min(total, u->size - new_srcrawofs);
                CV_OCL_CHECK(clEnqueueReadBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, total, ptr, 0, 0, 0));
                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            अण्णहा
            {
                AlignedDataPtr2D<मिच्छ, सच्च> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueReadBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    शून्य upload(UMatData* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                ध्रुव size_t srcstep[]) ध्रुव CV_OVERRIDE
    {
        जइ(!u)
            प्रत्यागम;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->tempUMat());

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        जइ( u->data && (u->hostCopyObsolete() < u->deviceCopyObsolete() || total == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(मिच्छ);
            u->markDeviceCopyObsolete(सच्च);
            प्रत्यागम;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#ifdef HAVE_OPENCL_SVM
        जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            CV_DbgAssert(u->data == रिक्त || u->data == u->handle);
            Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MAP) == 0);
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            जइ( iscontinuous )
            {
                memcpy((uchar*)u->handle + dstrawofs, srcptr, total);
            }
            अण्णहा
            {
                // This code is from MatAllocator::upload()
                पूर्णाङ्क isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)INT_MAX );
                    जइ( sz[i] == 0 )
                    प्रत्यागम;
                    जइ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (पूर्णाङ्क)sz[i];
                }

                Mat src(dims, isz, CV_8U, (शून्य*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ध्रुव Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                प्रति( j = 0; j < it.nplanes; j++, ++it )
                    memcpy(ptrs[1], ptrs[0], planesz);
            }
            जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_BUFFER)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_RESULT(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अण्णहा
#endif
        {
            जइ( iscontinuous )
            {
                AlignedDataPtr<सच्च, मिच्छ> alignedPtr((uchar*)srcptr, total, CV_OPENCL_DATA_PTR_ALIGNMENT);
                cl_int retval = clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, total, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (शून्य*)u->handle, (long long पूर्णाङ्क)dstrawofs, (long long पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            अण्णहा जइ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                AlignedDataPtr2D<मिच्छ, मिच्छ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
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
                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, total, ptr, 0, 0, 0));
            }
            अण्णहा
            {
                AlignedDataPtr2D<सच्च, मिच्छ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_DATA_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(clEnqueueWriteBufferRect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(सच्च);
#ifdef HAVE_OPENCL_SVM
        जइ ((u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अण्णहा
#endif
        {
            u->markHostCopyObsolete(सच्च);
        }
        u->markDeviceCopyObsolete(मिच्छ);
    }

    शून्य copy(UMatData* src, UMatData* dst, पूर्णाङ्क dims, ध्रुव size_t sz[],
              ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
              ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक _sync) ध्रुव CV_OVERRIDE
    {
        जइ(!src || !dst)
            प्रत्यागम;

        size_t total = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            total, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMatDataAutoLock src_autolock(src, dst);

        जइ( !src->handle || (src->data && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            प्रत्यागम;
        }
        जइ( !dst->handle || (dst->data && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(मिच्छ);
#ifdef HAVE_OPENCL_SVM
            जइ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            अण्णहा
#endif
            {
                dst->markDeviceCopyObsolete(सच्च);
            }
            प्रत्यागम;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#ifdef HAVE_OPENCL_SVM
        जइ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
        {
            जइ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                जइ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (पूर्णाङ्क)total);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            total, 0, रिक्त, रिक्त);
                    CV_OCL_CHECK_RESULT(status, "clEnqueueSVMMemcpy()");
                }
                अण्णहा
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    पूर्णाङ्क isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)INT_MAX );
                        जइ( sz[i] == 0 )
                        प्रत्यागम;
                        जइ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (पूर्णाङ्क)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                    {
                        जइ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ध्रुव Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    प्रति( j = 0; j < it.nplanes; j++, ++it )
                        memcpy(ptrs[1], ptrs[0], planesz);
                }
            }
            अण्णहा
            {
                जइ ((src->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->data + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                अण्णहा
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->data + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        अण्णहा
#endif
        {
            जइ( iscontinuous )
            {
                retval = clEnqueueCopyBuffer(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, total, 0, 0, 0);
                CV_OCL_CHECK_RESULT(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (शून्य*)src->handle, (शून्य*)dst->handle, (long long पूर्णाङ्क)srcrawofs, (long long पूर्णाङ्क)dstrawofs, (long long पूर्णाङ्क)total).c_str());
            }
            अण्णहा जइ (CV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_DATA_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                AlignedDataPtr2D<मिच्छ, मिच्छ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_DATA_PTR_ALIGNMENT, padding*2);
                AlignedDataPtr2D<मिच्छ, मिच्छ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
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

                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    memcpy( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(clEnqueueWriteBuffer(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_total, dstptr, 0, 0, 0));
            }
            अण्णहा
            {
                CV_OCL_CHECK(retval = clEnqueueCopyBufferRect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        जइ (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#ifdef HAVE_OPENCL_SVM
        जइ ((dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_BUFFER ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_BUFFER_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अण्णहा
#endif
        {
            dst->markHostCopyObsolete(सच्च);
        }
        dst->markDeviceCopyObsolete(मिच्छ);

        जइ( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    BufferPoolController* getBufferPoolController(ध्रुव वर्ण* id) ध्रुव CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        जइ (ctx.empty())
            प्रत्यागम रिक्त;
#ifdef HAVE_OPENCL_SVM
        जइ ((svm::checkForceSVMUmatUsage() && (id == रिक्त || strcmp(id, "OCL") == 0)) || (id != रिक्त && strcmp(id, "SVM") == 0))
        {
            प्रत्यागम &ctx.getImpl()->getBufferPoolSVM();
        }
#endif
        जइ (id != रिक्त && strcmp(id, "HOST_ALLOC") == 0)
        {
            प्रत्यागम &ctx.getImpl()->getBufferPoolHostPtr();
        }
        जइ (id != रिक्त && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        प्रत्यागम &ctx.getImpl()->getBufferPool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMatData*> cleanupQueue;

    शून्य flushCleanupQueue() ध्रुव
    {
        // TSAN false positive: see #27638
        जइ (!cleanupQueue.empty())
        {
            std::deque<UMatData*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            प्रति (std::deque<UMatData*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    शून्य addToCleanupQueue(UMatData* u) ध्रुव
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

स्थिर OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    स्थिर OpenCLAllocator* g_allocator = नूतन OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    प्रत्यागम g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


नामाकाश cv {

// three funcs below are implemented in umatrix.cpp
शून्य setSize( UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* _steps,
              तार्किक autoSteps = मिच्छ );
शून्य finalizeHdr(UMat& m);

} // namespace cv


नामाकाश cv { नामाकाश ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
शून्य convertFromBuffer(शून्य* cl_mem_buffer, size_t step, पूर्णाङ्क rows, पूर्णाङ्क cols, पूर्णाङ्क type, UMat& dst)
{
    पूर्णाङ्क d = 2;
    पूर्णाङ्क sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    setSize(dst, d, sizes, 0, सच्च);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_buffer;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, परिमाण(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_BUFFER == mem_type);

    size_t total = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_SIZE, परिमाण(size_t), &total, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((पूर्णाङ्क)step >= cols * CV_ELEM_SIZE(type));
    CV_Assert(total >= rows * step);

    // attach clBuffer to UMatData
    dst.u = नूतन UMatData(getOpenCLAllocator());
    dst.u->data            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_BUFFER;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMatData::MemoryFlag>(0);
    dst.u->handle          = cl_mem_buffer;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = total;

    finalizeHdr(dst);
    dst.addref();

    प्रत्यागम;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
शून्य convertFromImage(शून्य* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, परिमाण(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, परिमाण(cl_image_format), &fmt, 0));

    पूर्णाङ्क depth = CV_8U;
    विकल्प (fmt.image_channel_data_type)
    {
    पक्ष CL_UNORM_INT8:
    पक्ष CL_UNSIGNED_INT8:
        depth = CV_8U;
        भङ्ग;

    पक्ष CL_SNORM_INT8:
    पक्ष CL_SIGNED_INT8:
        depth = CV_8S;
        भङ्ग;

    पक्ष CL_UNORM_INT16:
    पक्ष CL_UNSIGNED_INT16:
        depth = CV_16U;
        भङ्ग;

    पक्ष CL_SNORM_INT16:
    पक्ष CL_SIGNED_INT16:
        depth = CV_16S;
        भङ्ग;

    पक्ष CL_SIGNED_INT32:
        depth = CV_32S;
        भङ्ग;

    पक्ष CL_FLOAT:
        depth = CV_32F;
        भङ्ग;

    पक्ष CL_HALF_FLOAT:
        depth = CV_16F;
        भङ्ग;

    स्वभाव:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    पूर्णाङ्क type = CV_8UC1;
    विकल्प (fmt.image_channel_order)
    {
    पक्ष CL_R:
    पक्ष CL_A:
    पक्ष CL_INTENSITY:
    पक्ष CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        भङ्ग;

    पक्ष CL_RG:
    पक्ष CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        भङ्ग;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    पक्ष CL_RGBA:
    पक्ष CL_BGRA:
    पक्ष CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        भङ्ग;

    स्वभाव:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        भङ्ग;
    }

    size_t step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, परिमाण(size_t), &step, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, परिमाण(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, परिमाण(size_t), &h, 0));

    dst.create((पूर्णाङ्क)h, (पूर्णाङ्क)w, type);

    cl_mem clBuffer = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, रिक्त, रिक्त));

    CV_OCL_CHECK(clFinish(q));

    प्रत्यागम;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

स्थिर शून्य getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint अङ्कः_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, रिक्त, &अङ्कः_Devices);
    जइ (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_RESULT(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &अङ्कः_Devices).c_str());
    }

    जइ (अङ्कः_Devices == 0)
    {
        devices.clear();
        प्रत्यागम;
    }

    devices.resize((size_t)अङ्कः_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, अङ्कः_Devices, &devices[0], &अङ्कः_Devices));
}

संरचना PlatformInfo::Impl
{
    Impl(शून्य* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) ध्रुव
    {
        वर्ण buf[1024];
        size_t sz=0;
        प्रत्यागम clGetPlatformInfo(handle, prop, परिमाण(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < परिमाण(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    पूर्णाङ्क versionMajor_;
    पूर्णाङ्क versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(शून्य* platform_id)
{
    p = नूतन Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    जइ(p)
        p->release();
}

PlatformInfo::PlatformInfo(ध्रुव PlatformInfo& i)
{
    जइ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::operator =(ध्रुव PlatformInfo& i)
{
    जइ (i.p != p)
    {
        जइ (i.p)
            i.p->addref();
        जइ (p)
            p->release();
        p = i.p;
    }
    प्रत्यागम *एतत्;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::operator = (PlatformInfo&& i) CV_NOEXCEPT
{
    जइ (एतत् != &i) {
        जइ(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

पूर्णाङ्क PlatformInfo::device_सङ्ख्या() ध्रुव
{
    प्रत्यागम p ? (पूर्णाङ्क)p->devices.size() : 0;
}

शून्य PlatformInfo::getDevice(Device& device, पूर्णाङ्क d) ध्रुव
{
    CV_Assert(p && d < (पूर्णाङ्क)p->devices.size() );
    जइ(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() ध्रुव
{
    प्रत्यागम p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ध्रुव
{
    प्रत्यागम p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() ध्रुव
{
    प्रत्यागम p ? p->version_ : String();
}

पूर्णाङ्क PlatformInfo::versionMajor() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->versionMajor_;
}

पूर्णाङ्क PlatformInfo::versionMinor() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->versionMinor_;
}

स्थिर शून्य getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint अङ्कः_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms));

    जइ (अङ्कः_Platforms == 0)
    {
        platforms.clear();
        प्रत्यागम;
    }

    platforms.resize((size_t)अङ्कः_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], &अङ्कः_Platforms));
}

शून्य getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    प्रति (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((शून्य*)&platforms[i]) );
}

ध्रुव वर्ण* typeToStr(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_DEPTH_MAX*16]=
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
    पूर्णाङ्क cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ध्रुव वर्ण* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    प्रत्यागम result;
}

ध्रुव वर्ण* memopTypeToStr(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_DEPTH_MAX*16] =
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
    पूर्णाङ्क cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ध्रुव वर्ण* result = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(result);
    प्रत्यागम result;
}

ध्रुव वर्ण* vecopTypeToStr(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_DEPTH_MAX*16] =
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
    पूर्णाङ्क cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    ध्रुव वर्ण* result = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(result);
    प्रत्यागम result;
}

// Deprecated due to size of buf buffer being unknowable.
ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    प्रत्यागम convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf, size_t buf_size)
{
    जइ( sdepth == ddepth )
        प्रत्यागम "noconvert";
    ध्रुव वर्ण *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    जइ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_size, "convert_%s", typestr);
    }
    अण्णहा जइ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_size, "convert_%s_sat_rte", typestr);
    अण्णहा
        snprintf(buf, buf_size, "convert_%s_sat", typestr);

    प्रत्यागम buf;
}

ध्रुव वर्ण* getOpenCLErrorString(पूर्णाङ्क errorCode)
{
#define CV_OCL_CODE(id) पक्ष id: प्रत्यागम #id
#define CV_OCL_CODE_(id, name) पक्ष id: प्रत्यागम #name
    विकल्प (errorCode)
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
    CV_OCL_CODE(CL_INVALID_मानम्);
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
    CV_OCL_CODE(CL_INVALID_द्विमान);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_INDEX);
    CV_OCL_CODE(CL_INVALID_ARG_मानम्);
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
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_गणनम्);
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
    स्वभाव: प्रत्यागम "Unknown OpenCL error";
    }
#undef CV_OCL_CODE
#undef CV_OCL_CODE_
}

template <typename T>
स्थिर std::string kerToStr(ध्रुव Mat & k)
{
    पूर्णाङ्क width = k.cols - 1, depth = k.depth();
    ध्रुव T * ध्रुव data = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    जइ (depth <= CV_8S)
    {
        प्रति (पूर्णाङ्क i = 0; i < width; ++i)
            stream << "DIG(" << (पूर्णाङ्क)data[i] << ")";
        stream << "DIG(" << (पूर्णाङ्क)data[width] << ")";
    }
    अण्णहा जइ (depth == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        प्रति (पूर्णाङ्क i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << "f)";
        stream << "DIG(" << data[width] << "f)";
    }
    अण्णहा जइ (depth == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        प्रति (पूर्णाङ्क i = 0; i < width; ++i)
            stream << "DIG(" << (भिन्न)data[i] << "h)";
        stream << "DIG(" << (भिन्न)data[width] << "h)";
    }
    अण्णहा
    {
        प्रति (पूर्णाङ्क i = 0; i < width; ++i)
            stream << "DIG(" << data[i] << ")";
        stream << "DIG(" << data[width] << ")";
    }

    प्रत्यागम stream.str();
}

String kernelToStr(निवेशः_Array _kernel, पूर्णाङ्क ddepth, ध्रुव वर्ण * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    पूर्णाङ्क depth = kernel.depth();
    जइ (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    जइ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    जइ (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    प्रकारनिर्देश std::string (* func_t)(ध्रुव Mat &);
    स्थिर ध्रुव func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<वर्ण>, kerToStr<ushort>, kerToStr<short>,
                                                kerToStr<पूर्णाङ्क>, kerToStr<भिन्न>, kerToStr<द्विगुण>, kerToStr<hfloat>,
                                                0, kerToStr<तार्किक>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<unsigned> };
    ध्रुव func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    प्रत्यागम cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#define PROCESS_SRC(src) \
    करेहि \
    { \
        जइ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            Size csize = src.size(); \
            पूर्णाङ्क ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            जइ (cwidth < ckercn || ckercn <= 0) \
                प्रत्यागम 1; \
            cols.push_back(cwidth); \
            जइ (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                प्रत्यागम 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    जाव ((शून्य)0, 0)

पूर्णाङ्क predictOptimalVectorWidth(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
                              निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
                              निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9,
                              OclVectorStrategy strat)
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    पूर्णाङ्क vectorWidths[CV_DEPTH_MAX] = {
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
    जइ (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    प्रत्यागम checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

पूर्णाङ्क checkOptimalVectorWidth([[maybe_unused]] ध्रुव पूर्णाङ्क *vectorWidths,
                            [[maybe_unused]] निवेशः_Array src1,
                            [[maybe_unused]] निवेशः_Array src2,
                            [[maybe_unused]] निवेशः_Array src3,
                            [[maybe_unused]] निवेशः_Array src4,
                            [[maybe_unused]] निवेशः_Array src5,
                            [[maybe_unused]] निवेशः_Array src6,
                            [[maybe_unused]] निवेशः_Array src7,
                            [[maybe_unused]] निवेशः_Array src8,
                            [[maybe_unused]] निवेशः_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#ifdef __APPLE__
    प्रत्यागम 1;
#अण्णहा
    CV_Assert(vectorWidths);

    पूर्णाङ्क ref_type = src1.type();

    std::vector<size_t> offsets, steps, cols;
    std::vector<पूर्णाङ्क> dividers, kercns;
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

    प्रति (size_t i = 0; i < size; ++i)
        जाव (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    पूर्णाङ्क kercn = *std::min_element(kercns.begin(), kercns.end());

    प्रत्यागम kercn;
#endif
}

पूर्णाङ्क predictOptimalVectorWidthMax(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
                                 निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
                                 निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9)
{
    प्रत्यागम predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#undef PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
शून्य buildOptionsAddMatrixDescription(String& buildOptions, ध्रुव String& name, निवेशः_Array _m)
{
    जइ (!buildOptions.empty())
        buildOptions += " ";
    पूर्णाङ्क type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (पूर्णाङ्क)CV_MAT_CN(type),
            name.c_str(), (पूर्णाङ्क)CV_ELEM_SIZE(type),
            name.c_str(), (पूर्णाङ्क)CV_ELEM_SIZE1(type),
            name.c_str(), (पूर्णाङ्क)depth
            );
}


संरचना Image2D::Impl
{
    Impl(ध्रुव UMat &src, तार्किक norm, तार्किक alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        जइ (handle)
            clReleaseMemObject(handle);
    }

    स्थिर cl_image_format getImageFormat(पूर्णाङ्क depth, पूर्णाङ्क cn, तार्किक norm)
    {
        cl_image_format format;
        स्थिर ध्रुव पूर्णाङ्क channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        स्थिर ध्रुव पूर्णाङ्क channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        स्थिर ध्रुव पूर्णाङ्क channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        पूर्णाङ्क channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        पूर्णाङ्क channelOrder = channelOrders[cn];
        format.image_channel_data_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        प्रत्यागम format;
    }

    स्थिर तार्किक isFormatSupported(cl_image_format format)
    {
        जइ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        जइ (!context)
            प्रत्यागम मिच्छ;

        // Figure out how many formats are supported by this context.
        cl_uint अङ्कः_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, अङ्कः_Formats,
                                                रिक्त, &अङ्कः_Formats);
        CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        जइ (अङ्कः_Formats > 0)
        {
            AutoBuffer<cl_image_format> formats(अङ्कः_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, अङ्कः_Formats,
                                             formats.data(), रिक्त);
            CV_OCL_DBG_CHECK_RESULT(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            प्रति (cl_uint i = 0; i < अङ्कः_Formats; ++i)
            {
                जइ (!memcmp(&formats[i], &format, परिमाण(format)))
                {
                    प्रत्यागम सच्च;
                }
            }
        }
        प्रत्यागम मिच्छ;
    }

    शून्य init(ध्रुव UMat &src, तार्किक norm, तार्किक alias)
    {
        जइ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        पूर्णाङ्क err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        जइ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        जइ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#ifdef CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ध्रुव Device & d = ocl::Device::getDefault();
        पूर्णाङ्क minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        जइ (1 < major || (1 == major && 2 <= minor))
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
            desc.अङ्कः_mip_levels   = 0;
            desc.अङ्कः_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, रिक्त, &err);
        }
        अण्णहा
#endif
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, रिक्त, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_RESULT(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.cols), static_cast<size_t>(src.rows), 1 };

        cl_mem devData;
        जइ (!alias && !src.isContinuous())
        {
            devData = clCreateBuffer(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elemSize(), रिक्त, &err);
            CV_OCL_CHECK_RESULT(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (long long पूर्णाङ्क)(src.cols * src.rows * src.elemSize()), (शून्य*)devData
                ).c_str());

            ध्रुव size_t roi[3] = {static_cast<size_t>(src.cols) * src.elemSize(), static_cast<size_t>(src.rows), 1};
            CV_OCL_CHECK(clEnqueueCopyBufferRect(queue, (cl_mem)src.handle(ACCESS_READ), devData, origin, origin,
                roi, src.step, 0, src.cols * src.elemSize(), 0, 0, रिक्त, रिक्त));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        अण्णहा
        {
            devData = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(devData != रिक्त);

        जइ (!alias)
        {
            CV_OCL_CHECK(clEnqueueCopyBufferToImage(queue, devData, handle, 0, origin, region, 0, रिक्त, 0));
            जइ (!src.isContinuous())
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
    p = रिक्त;
}

Image2D::Image2D(ध्रुव UMat &src, तार्किक norm, तार्किक alias)
{
    p = नूतन Impl(src, norm, alias);
}

तार्किक Image2D::canCreateAlias(ध्रुव UMat &m)
{
    तार्किक ret = मिच्छ;
    ध्रुव Device & d = ocl::Device::getDefault();
    जइ (d.imageFromBufferSupport() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        जइ (pitchAlign && !(m.step % (pitchAlign * m.elemSize())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            जइ (!m.u->tempUMat())
            {
                ret = सच्च;
            }
        }
    }
    प्रत्यागम ret;
}

तार्किक Image2D::isFormatSupported(पूर्णाङ्क depth, पूर्णाङ्क cn, तार्किक norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    प्रत्यागम Impl::isFormatSupported(format);
}

Image2D::Image2D(ध्रुव Image2D & i)
{
    p = i.p;
    जइ (p)
        p->addref();
}

Image2D & Image2D::operator = (ध्रुव Image2D & i)
{
    जइ (i.p != p)
    {
        जइ (i.p)
            i.p->addref();
        जइ (p)
            p->release();
        p = i.p;
    }
    प्रत्यागम *एतत्;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::operator = (Image2D&& i) CV_NOEXCEPT
{
    जइ (एतत् != &i) {
        जइ (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    प्रत्यागम *एतत्;
}

Image2D::~Image2D()
{
    जइ (p)
        p->release();
}

शून्य* Image2D::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

तार्किक internal::isOpenCLForced()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक value = मिच्छ;
    जइ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम value;
}

तार्किक internal::isPerformanceCheckBypassed()
{
    स्थिर तार्किक initialized = मिच्छ;
    स्थिर तार्किक value = मिच्छ;
    जइ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", मिच्छ);
        initialized = सच्च;
    }
    प्रत्यागम value;
}

तार्किक internal::isCLBuffer(UMat& u)
{
    जइ (isCudaBacked(u.u))
        प्रत्यागम मिच्छ;
    शून्य* h = u.handle(ACCESS_RW);
    जइ (!h)
        प्रत्यागम सच्च;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#जइ 1
    जइ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        प्रत्यागम मिच्छ;
#अण्णहा
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, परिमाण(type), &type, रिक्त);
    जइ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_BUFFER)
        प्रत्यागम मिच्छ;
#endif
    प्रत्यागम सच्च;
}

संरचना Timer::Impl
{
    ध्रुव Queue queue;

    Impl(ध्रुव Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    शून्य start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    शून्य stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() ध्रुव
    {
        प्रत्यागम (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ध्रुव Queue& q) : p(नूतन Impl(q)) { }
Timer::~Timer() { delete p; }

शून्य Timer::start()
{
    CV_Assert(p);
    p->start();
}

शून्य Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->durationNS();
}

}} // namespace

#endif // HAVE_OPENCL
