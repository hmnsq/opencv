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

#यदि_अपरिभाषितम् HAVE_OPENCL
#अन्तर्भाव "ocl_disabled.impl.hpp"
#अन्यथा // HAVE_OPENCL

#अन्तर्भाव <list>
#अन्तर्भाव <map>
#अन्तर्भाव <deque>
#अन्तर्भाव <set>
#अन्तर्भाव <string>
#अन्तर्भाव <sstream>
#अन्तर्भाव <fstream>
#यदि !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#अन्तर्भाव <inttypes.h>
#यदि_अन्तः

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#अपरिभाषय CV_LOG_STRIP_LEVEL
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव "opencv2/core/ocl_genbase.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#अन्तर्भाव "opencv2/core/utils/lock.private.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.hpp"
#अन्तर्भाव "opencv2/core/utils/filesystem.private.hpp"

#परिभाषय CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(रिक्त, 0, "OpenCL allocator: " << __VA_ARGS__)
#अन्तर्भाव "opencv2/core/utils/allocator_stats.impl.hpp"
#अपरिभाषय CV__ALLOCATOR_STATS_LOG

#परिभाषय CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#परिभाषय CV_OPENCL_SHOW_BUILD_OPTIONS             0
#परिभाषय CV_OPENCL_SHOW_BUILD_KERNELS             0

#परिभाषय CV_OPENCL_SHOW_सततम्_KERNELS               0
#परिभाषय CV_OPENCL_SYNC_सततम्_KERNELS               0
#परिभाषय CV_OPENCL_TRACE_CHECK                    0

#परिभाषय CV_OPENCL_VALIDATE_द्विमान_PROGRAMS       1

#परिभाषय CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#परिभाषय CV_OPENCL_SHOW_SVM_LOG                   0

#अन्तर्भाव "opencv2/core/bufferpool.hpp"
#यदि_अपरिभाषितम् LOG_बफर_POOL
# यदि 0
#   define LOG_बफर_POOL प्रारूप_लिख
# अन्यथा
#   define LOG_बफर_POOL(...)
# endif
#यदि_अन्तः

#यदि CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#परिभाषय CV_OPENCL_SVM_TRACE_P प्रारूप_लिख("line %d (ocl.cpp): ", __LINE__); प्रारूप_लिख
#अन्यथा
#परिभाषय CV_OPENCL_SVM_TRACE_P(...)
#यदि_अन्तः

#यदि CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#परिभाषय CV_OPENCL_SVM_TRACE_ERROR_P प्रारूप_लिख("Error on line %d (ocl.cpp): ", __LINE__); प्रारूप_लिख
#अन्यथा
#परिभाषय CV_OPENCL_SVM_TRACE_ERROR_P(...)
#यदि_अन्तः

#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_core.hpp"

#यदि_परिभाषितम् HAVE_OPENCL_SVM
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#अन्तर्भाव "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#अन्तर्भाव "opencv2/core/opencl/opencl_svm.hpp"
#यदि_अन्तः

#अन्तर्भाव "umatrix.hpp"

नामाकाश cv { नामाकाश ocl {

#परिभाषय IMPLEMENT_REFCOUNTABLE() \
    शून्य addref() { CV_XADD(&refcount, 1); } \
    शून्य release() { यदि( CV_XADD(&refcount, -1) == 1 && !cv::__termination) विलोपय एतत्; } \
    पूर्णाङ्क refcount

स्थिर cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    प्रत्यागम opencl_allocator_stats;
}

#यदि_अपरिभाषितम् _DEBUG
स्थिर तार्किक isRaiseError()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक value = असत्य;
    यदि (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम value;
}
#यदि_अन्तः

स्थिर शून्य onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", असत्य);
    यदि (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#यदि CV_OPENCL_TRACE_CHECK
स्थिर अन्तःस्थ
शून्य traceOpenCLCheck(cl_int स्थितिः, ध्रुव वर्ण* message)
{
    std::निर्गमः << "OpenCV(OpenCL:" << स्थितिः << "): " << message << std::पङ्क्त्यन्तः << std::flush;
}
#परिभाषय CV_OCL_TRACE_CHECK_परिणाम(स्थितिः, message) traceOpenCLCheck(स्थितिः, message)
#अन्यथा
#परिभाषय CV_OCL_TRACE_CHECK_परिणाम(स्थितिः, message) /* nothing */
#यदि_अन्तः

#परिभाषय CV_OCL_API_ERROR_सन्देशः(check_परिणाम, सन्देशः) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_परिणाम), check_परिणाम, सन्देशः)

#परिभाषय CV_OCL_CHECK_परिणाम(check_परिणाम, सन्देशः) \
    कुरु { \
        CV_OCL_TRACE_CHECK_परिणाम(check_परिणाम, सन्देशः); \
        यदि (check_परिणाम != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(सन्देशः), ध्रुव वर्ण*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_सन्देशः_2 = CV_OCL_API_ERROR_सन्देशः(check_परिणाम, सन्देशः); \
            CV_Error(Error::OpenCLApiCallError, error_सन्देशः_2); \
        } \
    } यावत् (0)

#परिभाषय CV_OCL_CHECK_(व्यञ्जकम्, check_परिणाम) कुरु { व्यञ्जकम्; CV_OCL_CHECK_परिणाम(check_परिणाम, #expr); } यावत् (0)

#परिभाषय CV_OCL_CHECK(व्यञ्जकम्) कुरु { cl_int __cl_result = (व्यञ्जकम्); CV_OCL_CHECK_परिणाम(__cl_result, #expr); } यावत् (0)

#यदि_परिभाषितम् _DEBUG
#परिभाषय CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, सन्देशः) CV_OCL_CHECK_परिणाम(check_परिणाम, सन्देशः)
#परिभाषय CV_OCL_DBG_CHECK(व्यञ्जकम्) CV_OCL_CHECK(व्यञ्जकम्)
#परिभाषय CV_OCL_DBG_CHECK_(व्यञ्जकम्, check_परिणाम) CV_OCL_CHECK_(व्यञ्जकम्, check_परिणाम)
#अन्यथा
#परिभाषय CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, सन्देशः) \
    कुरु { \
        CV_OCL_TRACE_CHECK_परिणाम(check_परिणाम, सन्देशः); \
        यदि (check_परिणाम != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(सन्देशः), ध्रुव वर्ण*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_सन्देशः_2 = CV_OCL_API_ERROR_सन्देशः(check_परिणाम, सन्देशः); \
            CV_Error(Error::OpenCLApiCallError, error_सन्देशः_2); \
        } \
    } यावत् (0)
#परिभाषय CV_OCL_DBG_CHECK_(व्यञ्जकम्, check_परिणाम) कुरु { व्यञ्जकम्; CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, #expr); } यावत् (0)
#परिभाषय CV_OCL_DBG_CHECK(व्यञ्जकम्) कुरु { cl_int __cl_result = (व्यञ्जकम्); CV_OCL_DBG_CHECK_परिणाम(__cl_result, #expr); } यावत् (0)
#यदि_अन्तः


स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", सत्य);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", सत्य);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_तालकम्_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", सत्य);
स्थिर ध्रुव तार्किक CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", सत्य);

#यदि CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
स्थिर ध्रुव तार्किक CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम् = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", असत्य);
#यदि_अन्तः

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
स्थिर ध्रुव तार्किक CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#यदि 1 //def __APPLE__
        सत्य
#अन्यथा
        असत्य
#यदि_अन्तः
);

स्थिर String getBuildExtraOptions()
{
    स्थिर String param_buildExtraOptions;
    स्थिर तार्किक initialized = असत्य;
    यदि (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = सत्य;
        यदि (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(रिक्त, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    प्रत्यागम param_buildExtraOptions;
}

स्थिर ध्रुव तार्किक CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", सत्य);
स्थिर ध्रुव size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


संरचना UMat2D
{
    UMat2D(ध्रुव UMat& m)
    {
        offset = (पूर्णाङ्क)m.offset;
        चरणः = (पूर्णाङ्क)m.चरणः;
        पङ्क्तिकाः = m.पङ्क्तिकाः;
        स्तम्भसङ्ख्या = m.स्तम्भसङ्ख्या;
    }
    पूर्णाङ्क offset;
    पूर्णाङ्क चरणः;
    पूर्णाङ्क पङ्क्तिकाः;
    पूर्णाङ्क स्तम्भसङ्ख्या;
};

संरचना UMat3D
{
    UMat3D(ध्रुव UMat& m)
    {
        offset = (पूर्णाङ्क)m.offset;
        चरणः = (पूर्णाङ्क)m.चरणः.p[1];
        slicestep = (पूर्णाङ्क)m.चरणः.p[0];
        slices = (पूर्णाङ्क)m.size.p[0];
        पङ्क्तिकाः = m.size.p[1];
        स्तम्भसङ्ख्या = m.size.p[2];
    }
    पूर्णाङ्क offset;
    पूर्णाङ्क slicestep;
    पूर्णाङ्क चरणः;
    पूर्णाङ्क slices;
    पूर्णाङ्क पङ्क्तिकाः;
    पूर्णाङ्क स्तम्भसङ्ख्या;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
स्थिर uint64 crc64( ध्रुव uchar* दत्त, size_t size, uint64 crc0=0 )
{
    स्थिर uint64 table[256];
    स्थिर तार्किक initialized = असत्य;

    यदि( !initialized )
    {
        प्रति( पूर्णाङ्क i = 0; i < 256; i++ )
        {
            uint64 c = i;
            प्रति( पूर्णाङ्क j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = सत्य;
    }

    uint64 crc = ~crc0;
    प्रति( size_t idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ दत्त[idx]] ^ (crc >> 8);

    प्रत्यागम ~crc;
}

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
संरचना OpenCLBinaryCacheConfigurator
{
    cv::String cache_मार्गः;
    cv::String cache_तालकम्_filename;
    cv::Ptr<utils::fs::File_तालकम्> cache_तालकम्;

    प्रकारनिर्देश std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(रिक्त, "Initializing OpenCL cache configuration...");
        यदि (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(रिक्त, "OpenCL cache is disabled");
            प्रत्यागम;
        }
        cache_मार्गः = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        यदि (cache_मार्गः.empty())
        {
            CV_LOG_INFO(रिक्त, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        कुरु
        {
            यत्न
            {
                यदि (cache_मार्गः.empty())
                    भङ्ग;
                यदि (cache_मार्गः == "disabled")
                    भङ्ग;
                यदि (!utils::fs::createDirectories(cache_मार्गः))
                {
                    CV_LOG_DEBUG(रिक्त, "Can't use OpenCL cache directory: " << cache_मार्गः);
                    clear();
                    भङ्ग;
                }

                यदि (CV_OPENCL_CACHE_तालकम्_ENABLE)
                {
                    cache_तालकम्_filename = cache_मार्गः + ".lock";
                    यदि (!utils::fs::exists(cache_तालकम्_filename))
                    {
                        CV_LOG_DEBUG(रिक्त, "Creating lock file... (" << cache_तालकम्_filename << ")");
                        std::ofstream तालकम्_filename(cache_तालकम्_filename.c_str(), std::ios::out);
                        यदि (!तालकम्_filename.is_open())
                        {
                            CV_LOG_WARNING(रिक्त, "Can't create lock file for OpenCL program cache: " << cache_तालकम्_filename);
                            भङ्ग;
                        }
                    }

                    यत्न
                    {
                        cache_तालकम् = makePtr<utils::fs::File_तालकम्>(cache_तालकम्_filename.c_str());
                        CV_LOG_VERBOSE(रिक्त, 0, "Checking cache lock... (" << cache_तालकम्_filename << ")");
                        {
                            utils::shared_तालकम्_guard<utils::fs::File_तालकम्> lock(*cache_तालकम्);
                        }
                        CV_LOG_VERBOSE(रिक्त, 0, "Checking cache lock... Done!");
                    }
                    गृहाण (ध्रुव cv::Exception& e)
                    {
                        CV_LOG_WARNING(रिक्त, "Can't create OpenCL program cache lock: " << cache_तालकम्_filename << std::पङ्क्त्यन्तः << e.what());
                    }
                    गृहाण (...)
                    {
                        CV_LOG_WARNING(रिक्त, "Can't create OpenCL program cache lock: " << cache_तालकम्_filename);
                    }
                }
                अन्यथा
                {
                    यदि (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(रिक्त, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    अन्यथा
                    {
                        CV_LOG_INFO(रिक्त, "OpenCL cache lock is disabled");
                    }
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_WARNING(रिक्त, "Can't prepare OpenCL program cache: " << cache_मार्गः << std::पङ्क्त्यन्तः << e.what());
                clear();
            }
        } यावत् (0);
        यदि (!cache_मार्गः.empty())
        {
            यदि (cache_तालकम्.empty() && CV_OPENCL_CACHE_तालकम्_ENABLE)
            {
                CV_LOG_WARNING(रिक्त, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            अन्यथा
            {
                CV_LOG_INFO(रिक्त, "Successfully initialized OpenCL cache directory: " << cache_मार्गः);
            }
        }
    }

    शून्य clear()
    {
        cache_मार्गः.clear();
        cache_तालकम्_filename.clear();
        cache_तालकम्.release();
    }

    std::string prepareCacheDirectoryForContext(ध्रुव std::string& ctx_उपसर्गः,
            ध्रुव std::string& cleanup_उपसर्गः)
    {
        यदि (cache_मार्गः.empty())
            प्रत्यागम std::string();

        Auto_तालकम् lock(mutex_prepared_contexts_);

        ContextCacheType::iterator प्राप्तम्_it = prepared_contexts_.find(ctx_उपसर्गः);
        यदि (प्राप्तम्_it != prepared_contexts_.end())
            प्रत्यागम प्राप्तम्_it->second;

        CV_LOG_INFO(रिक्त, "Preparing OpenCL cache configuration for context: " << ctx_उपसर्गः);

        std::string target_directory = cache_मार्गः + ctx_उपसर्गः + "/";
        तार्किक परिणाम = utils::fs::isDirectory(target_directory);
        यदि (!परिणाम)
        {
            यत्न
            {
                CV_LOG_VERBOSE(रिक्त, 0, "Creating directory: " << target_directory);
                यदि (utils::fs::createDirectories(target_directory))
                {
                    परिणाम = सत्य;
                }
                अन्यथा
                {
                    CV_LOG_WARNING(रिक्त, "Can't create directory: " << target_directory);
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_ERROR(रिक्त, "Can't create OpenCL program cache directory for context: " << target_directory << std::पङ्क्त्यन्तः << e.what());
            }
        }
        target_directory = परिणाम ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_उपसर्गः, target_directory));

        यदि (परिणाम && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_उपसर्गः.empty())
        {
            यत्न
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_मार्गः, cleanup_उपसर्गः + "*", entries, असत्य, सत्य);
                std::vector<String> remove_entries;
                प्रति (size_t i = 0; i < entries.size(); i++)
                {
                    ध्रुव String& name = entries[i];
                    यदि (0 == name.find(cleanup_उपसर्गः))
                    {
                        यदि (0 == name.find(ctx_उपसर्गः))
                            अनुवर्त; // skip current
                        remove_entries.push_back(name);
                    }
                }
                यदि (!remove_entries.empty())
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
                        cv::String मार्गः = utils::fs::join(cache_मार्गः, name);
                        यत्न
                        {
                            utils::fs::remove_all(मार्गः);
                            CV_LOG_WARNING(रिक्त, "Removed: " << मार्गः);
                        }
                        गृहाण (ध्रुव cv::Exception& e)
                        {
                            CV_LOG_ERROR(रिक्त, "Exception during removal of obsolete OpenCL cache directory: " << मार्गः << std::पङ्क्त्यन्तः << e.what());
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
वर्ग द्विमान_क्रमादेशः_File
{
    गणना { MAX_ENTRIES = 64 };

    प्रकारनिर्देश अचिह्नित पूर्णाङ्क uint32_t;

    संरचना CV_DECL_ALIGNED(4) File_शीर्षकम्
    {
        uint32_t source_Signature_आकार;
        //char sourceSignature[];
    };

    संरचना CV_DECL_ALIGNED(4) File_सारणी
    {
        uint32_t सङ्ख्या_सम्बन्धि_Entries;
        //uint32_t firstEntryOffset[];
    };

    संरचना CV_DECL_ALIGNED(4) File_प्रविष्टिः
    {
        uint32_t अग्र_प्रविष्टिः_File_विस्थापनम्; // 0 for the last entry in chain
        uint32_t कुञ्चिका_आकार;
        uint32_t दत्त_आकार;
        //char key[];
        //char data[];
    };

    ध्रुव std::string fileName_;
    ध्रुव वर्ण* ध्रुव sourceSignature_;
    ध्रुव size_t source_Signature_आकार_2;

    std::fstream f;

    uint32_t प्रविष्टिः_Offsets[MAX_ENTRIES];

    uint32_t getHash(ध्रुव std::string& options)
    {
        uint64 hash = crc64((ध्रुव uchar*)options.c_str(), options.size(), 0);
        प्रत्यागम hash & (MAX_ENTRIES - 1);
    }

    अन्तःस्थ size_t get_File_आकार()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_आकार = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        प्रत्यागम file_आकार;
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
        यदि (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(रिक्त, "Can't remove: " << fileName_);
        प्रत्यागम;
    }

सार्वजनिक:
    द्विमान_क्रमादेशः_File(ध्रुव std::string& fileName, ध्रुव वर्ण* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), source_Signature_आकार_2(sourceSignature_ ? वाक्यदैर्घ्यम्(sourceSignature_) : 0)
    {
        CV_StaticAssert(परिमाण(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != रिक्त);
        CV_Assert(source_Signature_आकार_2 > 0);
        स्मृतिं_पूरय(प्रविष्टिः_Offsets, 0, परिमाण(प्रविष्टिः_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.उद्घाटय(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        यदि(f.is_open() && get_File_आकार() > 0)
        {
            तार्किक isValid = असत्य;
            यत्न
            {
                uint32_t file_Source_Signature_आकार = readUInt32();
                यदि (file_Source_Signature_आकार == source_Signature_आकार_2)
                {
                    cv::Auto_बफर<वर्ण> fileSourceSignature(file_Source_Signature_आकार + 1);
                    f.पठ(fileSourceSignature.दत्त(), file_Source_Signature_आकार);
                    यदि (f.eof())
                    {
                        CV_LOG_ERROR(रिक्त, "Unexpected EOF");
                    }
                    अन्यथा यदि (memcmp(sourceSignature, fileSourceSignature.दत्त(), file_Source_Signature_आकार) == 0)
                    {
                        isValid = सत्य;
                    }
                }
                यदि (!isValid)
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
            यदि (!isValid)
            {
                clearFile();
            }
            अन्यथा
            {
                seekReadAbsolute(0);
            }
        }
    }

    तार्किक पठ(ध्रुव std::string& कुञ्चिका, std::vector<वर्ण>& buf)
    {
        यदि (!f.is_open())
            प्रत्यागम असत्य;

        size_t file_आकार = get_File_आकार();
        यदि (file_आकार == 0)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file (empty): " << fileName_);
            clearFile();
            प्रत्यागम असत्य;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_आकार = readUInt32();
        CV_Assert(file_Source_Signature_आकार > 0);
        seekReadRelative(file_Source_Signature_आकार);

        uint32_t सङ्ख्या_सम्बन्धि_Entries = readUInt32();
        CV_Assert(सङ्ख्या_सम्बन्धि_Entries > 0);
        यदि (सङ्ख्या_सम्बन्धि_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file: " << fileName_);
            clearFile();
            प्रत्यागम असत्य;
        }
        f.पठ((वर्ण*)&प्रविष्टिः_Offsets[0], परिमाण(प्रविष्टिः_Offsets));
        CV_Assert(!f.fail());

        uint32_t प्रविष्टिः_अङ्कः = getHash(कुञ्चिका);

        uint32_t प्रविष्टिः_विस्थापनम् = प्रविष्टिः_Offsets[प्रविष्टिः_अङ्कः];
        File_प्रविष्टिः प्रविष्टिः;
        यावत् (प्रविष्टिः_विस्थापनम् > 0)
        {
            seekReadAbsolute(प्रविष्टिः_विस्थापनम्);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पठ((वर्ण*)&प्रविष्टिः, परिमाण(प्रविष्टिः));
            CV_Assert(!f.fail());
            cv::Auto_बफर<वर्ण> file_कुञ्चिका(प्रविष्टिः.कुञ्चिका_आकार + 1);
            यदि (कुञ्चिका.size() == प्रविष्टिः.कुञ्चिका_आकार)
            {
                यदि (प्रविष्टिः.कुञ्चिका_आकार > 0)
                {
                    f.पठ(file_कुञ्चिका.दत्त(), प्रविष्टिः.कुञ्चिका_आकार);
                    CV_Assert(!f.fail());
                }
                यदि (memcmp(file_कुञ्चिका.दत्त(), कुञ्चिका.c_str(), प्रविष्टिः.कुञ्चिका_आकार) == 0)
                {
                    buf.resize(प्रविष्टिः.दत्त_आकार);
                    f.पठ(&buf[0], प्रविष्टिः.दत्त_आकार);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(रिक्त, 0, "Read...");
                    प्रत्यागम सत्य;
                }
            }
            यदि (प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम् == 0)
                भङ्ग;
            प्रविष्टिः_विस्थापनम् = प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम्;
        }
        प्रत्यागम असत्य;
    }

    तार्किक लिख(ध्रुव std::string& कुञ्चिका, std::vector<वर्ण>& buf)
    {
        यदि (!f.is_open())
        {
            f.उद्घाटय(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            यदि (!f.is_open())
            {
                f.उद्घाटय(fileName_.c_str(), std::ios::out|std::ios::binary);
                यदि (!f.is_open())
                {
                    CV_LOG_ERROR(रिक्त, "Can't create file: " << fileName_);
                    प्रत्यागम असत्य;
                }
            }
        }

        size_t file_आकार = get_File_आकार();
        यदि (file_आकार == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)source_Signature_आकार_2);
            f.लिख(sourceSignature_, source_Signature_आकार_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            स्मृतिं_पूरय(प्रविष्टिः_Offsets, 0, परिमाण(प्रविष्टिः_Offsets));
            f.लिख((वर्ण*)प्रविष्टिः_Offsets, परिमाण(प्रविष्टिः_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.उद्घाटय(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_आकार = get_File_आकार();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_आकार = readUInt32();
        CV_Assert(file_Source_Signature_आकार == source_Signature_आकार_2);
        seekReadRelative(file_Source_Signature_आकार);

        uint32_t सङ्ख्या_सम्बन्धि_Entries = readUInt32();
        CV_Assert(सङ्ख्या_सम्बन्धि_Entries > 0);
        यदि (सङ्ख्या_सम्बन्धि_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(रिक्त, "Invalid file: " << fileName_);
            clearFile();
            प्रत्यागम असत्य;
        }
        size_t सारणी_Entries_विस्थापनम् = (size_t)f.tellg();
        f.पठ((वर्ण*)&प्रविष्टिः_Offsets[0], परिमाण(प्रविष्टिः_Offsets));
        CV_Assert(!f.fail());

        uint32_t प्रविष्टिः_अङ्कः = getHash(कुञ्चिका);

        uint32_t प्रविष्टिः_विस्थापनम् = प्रविष्टिः_Offsets[प्रविष्टिः_अङ्कः];
        File_प्रविष्टिः प्रविष्टिः;
        यावत् (प्रविष्टिः_विस्थापनम् > 0)
        {
            seekReadAbsolute(प्रविष्टिः_विस्थापनम्);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पठ((वर्ण*)&प्रविष्टिः, परिमाण(प्रविष्टिः));
            CV_Assert(!f.fail());
            cv::Auto_बफर<वर्ण> file_कुञ्चिका(प्रविष्टिः.कुञ्चिका_आकार + 1);
            यदि (कुञ्चिका.size() == प्रविष्टिः.कुञ्चिका_आकार)
            {
                यदि (प्रविष्टिः.कुञ्चिका_आकार > 0)
                {
                    f.पठ(file_कुञ्चिका.दत्त(), प्रविष्टिः.कुञ्चिका_आकार);
                    CV_Assert(!f.fail());
                }
                यदि (0 == memcmp(file_कुञ्चिका.दत्त(), कुञ्चिका.c_str(), प्रविष्टिः.कुञ्चिका_आकार))
                {
                    // duplicate
                    CV_LOG_VERBOSE(रिक्त, 0, "Duplicate key ignored: " << fileName_);
                    प्रत्यागम असत्य;
                }
            }
            यदि (प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम् == 0)
                भङ्ग;
            प्रविष्टिः_विस्थापनम् = प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम्;
        }
        seekReadAbsolute(0);
        यदि (प्रविष्टिः_विस्थापनम् > 0)
        {
            seekWriteAbsolute(प्रविष्टिः_विस्थापनम्);
            प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम् = (uint32_t)file_आकार;
            f.लिख((वर्ण*)&प्रविष्टिः, परिमाण(प्रविष्टिः));
            CV_Assert(!f.fail());
        }
        अन्यथा
        {
            प्रविष्टिः_Offsets[प्रविष्टिः_अङ्कः] = (uint32_t)file_आकार;
            seekWriteAbsolute(सारणी_Entries_विस्थापनम्);
            f.लिख((वर्ण*)प्रविष्टिः_Offsets, परिमाण(प्रविष्टिः_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_आकार);
        प्रविष्टिः.अग्र_प्रविष्टिः_File_विस्थापनम् = 0;
        प्रविष्टिः.दत्त_आकार = (uint32_t)buf.size();
        प्रविष्टिः.कुञ्चिका_आकार = (uint32_t)कुञ्चिका.size();
        f.लिख((वर्ण*)&प्रविष्टिः, परिमाण(प्रविष्टिः));
        CV_Assert(!f.fail());
        f.लिख(कुञ्चिका.c_str(), प्रविष्टिः.कुञ्चिका_आकार);
        CV_Assert(!f.fail());
        f.लिख(&buf[0], प्रविष्टिः.दत्त_आकार);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(रिक्त, 0, "Write... (" << buf.size() << " bytes)");
        प्रत्यागम सत्य;
    }
};
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT



संरचना OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    पूर्णाङ्क device_;  // device index in context
    ocl::Queue queue_;
    पूर्णाङ्क useOpenCL_;

रक्षित:
    Impl() = विलोपय;

    शून्य _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        पूर्णाङ्क ndevices = (पूर्णाङ्क)context_.ndevices();
        CV_Assert(ndevices > 0);
        तार्किक प्राप्तम् = असत्य;
        प्रति (पूर्णाङ्क i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            यदि (dhandle == deviceID)
            {
                device_ = i;
                प्राप्तम् = सत्य;
                भङ्ग;
            }
        }
        CV_Assert(प्राप्तम् && "OpenCL device can't work with passed OpenCL context");
    }

    शून्य _init_device(ध्रुव ocl::Device& device)
    {
        CV_Assert(device.ptr());
        पूर्णाङ्क ndevices = (पूर्णाङ्क)context_.ndevices();
        CV_Assert(ndevices > 0);
        तार्किक प्राप्तम् = असत्य;
        प्रति (पूर्णाङ्क i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            यदि (d.getImpl() == device.getImpl())
            {
                device_ = i;
                प्राप्तम् = सत्य;
                भङ्ग;
            }
        }
        CV_Assert(प्राप्तम् && "OpenCL device can't work with passed OpenCL context");
    }

सार्वजनिक:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_अप्रयुक्तम्(platformID);
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
        यदि (useOpenCL_ < 0)
        {
            यत्न
            {
                useOpenCL_ = 0;
                यदि (!context_.empty() && context_.ndevices() > 0)
                {
                    ध्रुव Device& d = context_.device(device_);
                    useOpenCL_ = d.उपलब्धम्();
                }
            }
            गृहाण (ध्रुव cv::Exception&)
            {
                // nothing
            }
            यदि (!useOpenCL_)
                CV_LOG_INFO(रिक्त, "OpenCL: can't use OpenCL execution context");
        }
        प्रत्यागम useOpenCL_ > 0;
    }

    शून्य समुच्चयः_Use_Open_CL(तार्किक flag)
    {
        यदि (!flag)
            useOpenCL_ = 0;
        अन्यथा
            useOpenCL_ = -1;
    }

    स्थिर ध्रुव std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(रिक्त, "OpenCL: initializing thread execution context");

        स्थिर तार्किक initialized = असत्य;
        स्थिर std::shared_ptr<Impl> g_primaryExecutionContext;

        यदि (!initialized)
        {
            cv::Auto_तालकम् lock(getInitializationMutex());
            यदि (!initialized)
            {
                CV_LOG_INFO(रिक्त, "OpenCL: creating new execution context...");
                यत्न
                {
                    Context c = ocl::Context::create(std::string());
                    यदि (c.ndevices())
                    {
                        पूर्णाङ्क deviceId = 0;
                        स्वचालित& d = c.device(deviceId);
                        यदि (d.उपलब्धम्())
                        {
                            स्वचालित q = ocl::Queue(c, d);
                            यदि (!q.ptr())
                            {
                                CV_LOG_ERROR(रिक्त, "OpenCL: Can't create default OpenCL queue");
                            }
                            अन्यथा
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(रिक्त, "OpenCL: device=" << d.name());
                            }
                        }
                        अन्यथा
                        {
                            CV_LOG_ERROR(रिक्त, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    अन्यथा
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
                initialized = सत्य;
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
    यदि (p)
        प्रत्यागम p->useOpenCL();
    प्रत्यागम असत्य;
}
शून्य OpenCLExecutionContext::समुच्चयः_Use_Open_CL(तार्किक flag)
{
    CV_Assert(p);
    p->समुच्चयः_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    OpenCLExecutionContext& c = दत्त.oclExecutionContext;
    यदि (!दत्त.oclExecutionContextInitialized)
    {
        दत्त.oclExecutionContextInitialized = सत्य;
        यदि (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    प्रत्यागम c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    OpenCLExecutionContext& c = दत्त.oclExecutionContext;
    प्रत्यागम c;
}

शून्य OpenCLExecutionContext::bind() ध्रुव
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    दत्त.oclExecutionContext = *एतत्;
    दत्त.oclExecutionContextInitialized = सत्य;
    दत्त.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
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
    यदि (!haveOpenCL())
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
    यदि (!haveOpenCL())
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
स्थिर तार्किक g_isOpenCLInitialized = असत्य;
स्थिर तार्किक g_isOpenCLAvailable = असत्य;

तार्किक haveOpenCL()
{
    CV_TRACE_FUNCTION();

    यदि (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string env_मार्गः = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        यदि (!env_मार्गः.empty())
        {
            यदि (env_मार्गः == "disabled")
            {
                g_isOpenCLAvailable = असत्य;
                g_isOpenCLInitialized = सत्य;
                प्रत्यागम असत्य;
            }
        }

        cv::Auto_तालकम् lock(getInitializationMutex());
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
            g_isOpenCLAvailable = असत्य;
        }
        g_isOpenCLInitialized = सत्य;
    }
    प्रत्यागम g_isOpenCLAvailable;
}

तार्किक useOpenCL()
{
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    यदि (दत्त.useOpenCL < 0)
    {
        यत्न
        {
            दत्त.useOpenCL = 0;
            यदि (haveOpenCL())
            {
                स्वचालित c = OpenCLExecutionContext::getCurrent();
                दत्त.useOpenCL = c.useOpenCL();
            }
        }
        गृहाण (...)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    प्रत्यागम दत्त.useOpenCL > 0;
}

तार्किक isOpenCLActivated()
{
    यदि (!g_isOpenCLAvailable)
        प्रत्यागम असत्य; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    प्रत्यागम useOpenCL();
}

शून्य समुच्चयः_Use_Open_CL(तार्किक flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    स्वचालित& c = OpenCLExecutionContext::getCurrentRef();
    यदि (!c.empty())
    {
        c.समुच्चयः_Use_Open_CL(flag);
        दत्त.useOpenCL = c.useOpenCL();
    }
    अन्यथा
    {
        यदि (!flag)
            दत्त.useOpenCL = 0;
        अन्यथा
            दत्त.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#यदि_परिभाषितम् HAVE_CLAMDBLAS

वर्ग AmdBlasHelper
{
सार्वजनिक:
    स्थिर AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, नूतन AmdBlasHelper())
    }

    तार्किक is_उपलब्धम्() ध्रुव
    {
        प्रत्यागम g_is_Amd_Blas_उपलब्धम्;
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
        यदि (!g_isAmdBlasInitialized)
        {
            Auto_तालकम् lock(getInitializationMutex());

            यदि (!g_isAmdBlasInitialized)
            {
                यदि (haveOpenCL())
                {
                    यत्न
                    {
                        g_is_Amd_Blas_उपलब्धम् = clblas_सज्जता() == clblasSuccess;
                    }
                    गृहाण (...)
                    {
                        g_is_Amd_Blas_उपलब्धम् = असत्य;
                    }
                }
                अन्यथा
                    g_is_Amd_Blas_उपलब्धम् = असत्य;

                g_isAmdBlasInitialized = सत्य;
            }
        }
    }

गुप्त:
    स्थिर तार्किक g_isAmdBlasInitialized;
    स्थिर तार्किक g_is_Amd_Blas_उपलब्धम्;
};

तार्किक AmdBlasHelper::g_is_Amd_Blas_उपलब्धम् = असत्य;
तार्किक AmdBlasHelper::g_isAmdBlasInitialized = असत्य;

तार्किक haveAmdBlas()
{
    प्रत्यागम AmdBlasHelper::getInstance().is_उपलब्धम्();
}

#अन्यथा

तार्किक haveAmdBlas()
{
    प्रत्यागम असत्य;
}

#यदि_अन्तः

#यदि_परिभाषितम् HAVE_CLAMDFFT

वर्ग AmdFftHelper
{
सार्वजनिक:
    स्थिर AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, नूतन AmdFftHelper())
    }

    तार्किक is_उपलब्धम्() ध्रुव
    {
        प्रत्यागम g_is_Amd_Fft_उपलब्धम्;
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
        यदि (!g_isAmdFftInitialized)
        {
            Auto_तालकम् lock(getInitializationMutex());

            यदि (!g_isAmdFftInitialized)
            {
                यदि (haveOpenCL())
                {
                    यत्न
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_सज्जता_दत्त(&सज्जता_दत्त) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_संस्करणम्(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_उपलब्धम् = सत्य;
                    }
                    गृहाण (ध्रुव Exception &)
                    {
                        g_is_Amd_Fft_उपलब्धम् = असत्य;
                    }
                }
                अन्यथा
                    g_is_Amd_Fft_उपलब्धम् = असत्य;

                g_isAmdFftInitialized = सत्य;
            }
        }
    }

गुप्त:
    स्थिर clfft_सज्जता_दत्त सज्जता_दत्त;
    स्थिर तार्किक g_isAmdFftInitialized;
    स्थिर तार्किक g_is_Amd_Fft_उपलब्धम्;
};

clfft_सज्जता_दत्त AmdFftHelper::सज्जता_दत्त;
तार्किक AmdFftHelper::g_is_Amd_Fft_उपलब्धम् = असत्य;
तार्किक AmdFftHelper::g_isAmdFftInitialized = असत्य;

तार्किक haveAmdFft()
{
    प्रत्यागम AmdFftHelper::getInstance().is_उपलब्धम्();
}

#अन्यथा

तार्किक haveAmdFft()
{
    प्रत्यागम असत्य;
}

#यदि_अन्तः

तार्किक haveSVM()
{
#यदि_परिभाषितम् HAVE_OPENCL_SVM
    प्रत्यागम सत्य;
#अन्यथा
    प्रत्यागम असत्य;
#यदि_अन्तः
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
        initialized = असत्य;
    }

    ~Impl() {}

    शून्य init()
    {
        यदि( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            यदि( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            यदि( handle != 0 )
            {
                वर्ण buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, परिमाण(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = सत्य;
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
    यदि(p)
        p->release();
}

Platform::Platform(ध्रुव Platform& pl)
{
    p = (Impl*)pl.p;
    यदि(p)
        p->addref();
}

Platform& Platform::संक्रियकः = (ध्रुव Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = शून्यसूचकः;
}

Platform& Platform::संक्रियकः = (Platform&& pl) CV_NOEXCEPT
{
    यदि (एतत् != &pl) {
        यदि(p)
            p->release();
        p = pl.p;
        pl.p = शून्यसूचकः;
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
    यदि( !p.p )
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
स्थिर शून्य parseOpenCLVersion(ध्रुव String &संस्करणम्, पूर्णाङ्क &major, पूर्णाङ्क &minor)
{
    major = minor = 0;
    यदि (10 >= संस्करणम्.length())
        प्रत्यागम;
    ध्रुव वर्ण *pstr = संस्करणम्.c_str();
    यदि (0 != वाक्ये_तुलय(pstr, "OpenCL ", 7))
        प्रत्यागम;
    size_t ppos = संस्करणम्.find('.', 7);
    यदि (String::npos == ppos)
        प्रत्यागम;
    String अस्थायी_2 = संस्करणम्.substr(7, ppos - 7);
    major = atoi(अस्थायी_2.c_str());
    अस्थायी_2 = संस्करणम्.substr(ppos + 1);
    minor = atoi(अस्थायी_2.c_str());
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
        संस्करणम्_3 = getStrProp(CL_DEVICE_संस्करणम्);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_स्मृतिः = getBoolProp(CL_DEVICE_HOST_UNIFIED_स्मृतिः);
        maxComputeUnits_ = getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_गणः_आकार_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_गणः_आकार);
        type_ = getProp<cl_device_type, पूर्णाङ्क>(CL_DEVICE_TYPE);
        driver_संस्करणम्_2 = getStrProp(CL_DRIVER_संस्करणम्);
        स्थानाङ्कः_द्वयङ्काः_2 = getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_स्थानाङ्कः_द्वयङ्काः);

        String device_संस्करणम् = getStrProp(CL_DEVICE_संस्करणम्);
        parseOpenCLVersion(device_संस्करणम्, device_संस्करणम्_Major_2, device_संस्करणम्_Minor_2);

        size_t pos = 0;
        यावत् (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            यदि (pos2 == String::npos)
                pos2 = extensions_.size();
            यदि (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_समुच्चयः.insert(extensionName);
            }
            pos = pos2 + 1;
        }

        khr_fp64_support_ = isExtensionSupported("cl_khr_fp64");
        khr_fp16_support_ = isExtensionSupported("cl_khr_fp16");

        intelSubgroupsSupport_ = isExtensionSupported("cl_intel_subgroups");

        vendorName_ = getStrProp(CL_DEVICE_VENDOR);
        यदि (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        अन्यथा यदि (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || वाक्ये_अन्विष(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        अन्यथा यदि (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        अन्यथा
            vendorID_ = UNKNOWN_VENDOR;

        ध्रुव size_t CV_OPENCL_DEVICE_MAX_WORK_गणः_आकार = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        यदि (CV_OPENCL_DEVICE_MAX_WORK_गणः_आकार > 0)
        {
            ध्रुव size_t new_max_Work_गणः_आकार = std::min(max_Work_गणः_आकार_2, CV_OPENCL_DEVICE_MAX_WORK_गणः_आकार);
            यदि (new_max_Work_गणः_आकार != max_Work_गणः_आकार_2)
                CV_LOG_WARNING(रिक्त, "OpenCL: using workgroup size: " << new_max_Work_गणः_आकार << " (was " << max_Work_गणः_आकार_2 << ")");
            max_Work_गणः_आकार_2 = new_max_Work_गणः_आकार;
        }
#यदि 0
        यदि (isExtensionSupported("cl_khr_spir"))
        {
#यदि_अपरिभाषितम् CL_DEVICE_SPIR_VERSIONS
#परिभाषय CL_DEVICE_SPIR_VERSIONS                     0x40E0
#यदि_अन्तः
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::निर्गमः << spir_versions << std::पङ्क्त्यन्तः;
        }
#यदि_अन्तः
    }

    ~Impl()
    {
#यदि_परिभाषितम् _WIN32
        यदि (!cv::__termination)
#यदि_अन्तः
        {
            यदि (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    प्रतिरूपम्<प्रकारनाम _TpCL, प्रकारनाम Tp_बहिर्गमनम्>
    Tp_बहिर्गमनम् getProp(cl_device_info prop) ध्रुव
    {
        _TpCL अस्थायी_2=_TpCL();
        size_t sz = 0;

        प्रत्यागम clGetDeviceInfo(handle, prop, परिमाण(अस्थायी_2), &अस्थायी_2, &sz) == CL_SUCCESS &&
            sz == परिमाण(अस्थायी_2) ? Tp_बहिर्गमनम्(अस्थायी_2) : Tp_बहिर्गमनम्();
    }

    तार्किक getBoolProp(cl_device_info prop) ध्रुव
    {
        cl_bool अस्थायी_2 = CL_FALSE;
        size_t sz = 0;

        प्रत्यागम clGetDeviceInfo(handle, prop, परिमाण(अस्थायी_2), &अस्थायी_2, &sz) == CL_SUCCESS &&
            sz == परिमाण(अस्थायी_2) ? अस्थायी_2 != 0 : असत्य;
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
        प्रत्यागम extensions_समुच्चयः.गणनम्_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String संस्करणम्_3;
    std::string extensions_;
    पूर्णाङ्क doubleFPConfig_;
    तार्किक khr_fp64_support_;
    पूर्णाङ्क halfFPConfig_;
    तार्किक khr_fp16_support_;
    तार्किक host_Unified_स्मृतिः;
    पूर्णाङ्क maxComputeUnits_;
    size_t max_Work_गणः_आकार_2;
    पूर्णाङ्क type_;
    पूर्णाङ्क स्थानाङ्कः_द्वयङ्काः_2;
    पूर्णाङ्क device_संस्करणम्_Major_2;
    पूर्णाङ्क device_संस्करणम्_Minor_2;
    String driver_संस्करणम्_2;
    String vendorName_;
    पूर्णाङ्क vendorID_;
    तार्किक intelSubgroupsSupport_;

    std::समुच्चयः<std::string> extensions_समुच्चयः;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(शून्य* d)
{
    p = 0;
    समुच्चयः(d);
}

Device::Device(ध्रुव Device& d)
{
    p = d.p;
    यदि(p)
        p->addref();
}

Device& Device::संक्रियकः = (ध्रुव Device& d)
{
    Impl* newp = (Impl*)d.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = शून्यसूचकः;
}

Device& Device::संक्रियकः = (Device&& d) CV_NOEXCEPT
{
    यदि (एतत् != &d) {
        यदि(p)
            p->release();
        p = d.p;
        d.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

Device::~Device()
{
    यदि(p)
        p->release();
}

शून्य Device::समुच्चयः(शून्य* d)
{
    यदि(p)
        p->release();
    p = नूतन Impl(d);
    यदि (p->handle)
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
{ प्रत्यागम p ? p->isExtensionSupported(extensionName) : असत्य; }

String Device::संस्करणम्() ध्रुव
{ प्रत्यागम p ? p->संस्करणम्_3 : String(); }

String Device::vendorName() ध्रुव
{ प्रत्यागम p ? p->vendorName_ : String(); }

पूर्णाङ्क Device::vendorID() ध्रुव
{ प्रत्यागम p ? p->vendorID_ : 0; }

String Device::Open_CL_C_संस्करणम्() ध्रुव
{ प्रत्यागम p ? p->getStrProp(CL_DEVICE_OPENCL_C_संस्करणम्) : String(); }

String Device::OpenCLVersion() ध्रुव
{ प्रत्यागम p ? p->getStrProp(CL_DEVICE_संस्करणम्) : String(); }

पूर्णाङ्क Device::device_संस्करणम्_Major() ध्रुव
{ प्रत्यागम p ? p->device_संस्करणम्_Major_2 : 0; }

पूर्णाङ्क Device::device_संस्करणम्_Minor() ध्रुव
{ प्रत्यागम p ? p->device_संस्करणम्_Minor_2 : 0; }

String Device::driver_संस्करणम्() ध्रुव
{ प्रत्यागम p ? p->driver_संस्करणम्_2 : String(); }

पूर्णाङ्क Device::type() ध्रुव
{ प्रत्यागम p ? p->type_ : 0; }

पूर्णाङ्क Device::स्थानाङ्कः_द्वयङ्काः() ध्रुव
{ प्रत्यागम p ? p->स्थानाङ्कः_द्वयङ्काः_2 : 0; }

तार्किक Device::उपलब्धम्() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_उपलब्धम्) : असत्य; }

तार्किक Device::compiler_उपलब्धम्() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_COMPILER_उपलब्धम्) : असत्य; }

तार्किक Device::linker_उपलब्धम्() ध्रुव
#यदि_परिभाषितम् CL_संस्करणम्_1_2
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_LINKER_उपलब्धम्) : असत्य; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#यदि_अन्तः

पूर्णाङ्क Device::doubleFPConfig() ध्रुव
{ प्रत्यागम p ? p->doubleFPConfig_ : 0; }

पूर्णाङ्क Device::singleFPConfig() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_fp_config, पूर्णाङ्क>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

पूर्णाङ्क Device::halfFPConfig() ध्रुव
{ प्रत्यागम p ? p->halfFPConfig_ : 0; }

तार्किक Device::hasFP64() ध्रुव
{ प्रत्यागम p ? p->khr_fp64_support_ : असत्य; }
तार्किक Device::hasFP16() ध्रुव
{ प्रत्यागम p ? p->khr_fp16_support_ : असत्य; }

तार्किक Device::endianLittle() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : असत्य; }

तार्किक Device::errorCorrectionSupport() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : असत्य; }

पूर्णाङ्क Device::executionCapabilities() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_exec_capabilities, पूर्णाङ्क>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_आकार) : 0; }

पूर्णाङ्क Device::globalMemCacheType() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_mem_cache_type, पूर्णाङ्क>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

पूर्णाङ्क Device::global_Mem_Cache_पङ्क्तिः_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_GLOBAL_MEM_CACHELINE_आकार) : 0; }

size_t Device::global_Mem_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_आकार) : 0; }

size_t Device::local_Mem_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_आकार) : 0; }

पूर्णाङ्क Device::localMemType() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_device_local_mem_type, पूर्णाङ्क>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

तार्किक Device::host_Unified_स्मृतिः_2() ध्रुव
{ प्रत्यागम p ? p->host_Unified_स्मृतिः : असत्य; }

तार्किक Device::imageSupport() ध्रुव
{ प्रत्यागम p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : असत्य; }

तार्किक Device::image_From_बफर_Support() ध्रुव
{
    प्रत्यागम p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : असत्य;
}

uint Device::imagePitchAlignment() ध्रुव
{
#यदि_परिभाषितम् CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    प्रत्यागम p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः
}

uint Device::image_आधारः_स्थानाङ्कः_Alignment() ध्रुव
{
#यदि_परिभाषितम् CL_DEVICE_IMAGE_आधारः_स्थानाङ्कः_ALIGNMENT
    प्रत्यागम p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_आधारः_स्थानाङ्कः_ALIGNMENT) : 0;
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः
}

size_t Device::image2_DMax_विस्तारः() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_विस्तारः) : 0; }

size_t Device::image2DMaxHeight() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

size_t Device::image3_DMax_विस्तारः() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_विस्तारः) : 0; }

size_t Device::image3DMaxHeight() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

size_t Device::image3_DMax_गभीरता() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_गभीरता) : 0; }

size_t Device::image_Max_बफर_आकार() ध्रुव
#यदि_परिभाषितम् CL_संस्करणम्_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_बफर_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#यदि_अन्तः

size_t Device::image_Max_Array_आकार() ध्रुव
#यदि_परिभाषितम् CL_संस्करणम्_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#यदि_अन्तः

तार्किक Device::intelSubgroupsSupport() ध्रुव
{ प्रत्यागम p ? p->intelSubgroupsSupport_ : असत्य; }

पूर्णाङ्क Device::maxClockFrequency() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

पूर्णाङ्क Device::maxComputeUnits() ध्रुव
{ प्रत्यागम p ? p->maxComputeUnits_ : 0; }

पूर्णाङ्क Device::max_Constant_तर्काः() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_CONSTANT_तर्काः) : 0; }

size_t Device::max_Constant_बफर_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_बफर_आकार) : 0; }

size_t Device::max_Mem_Alloc_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_आकार) : 0; }

size_t Device::max_Parameter_आकार() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_आकार) : 0; }

पूर्णाङ्क Device::max_Read_Image_तर्काः() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_READ_IMAGE_तर्काः) : 0; }

पूर्णाङ्क Device::max_Write_Image_तर्काः() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_WRITE_IMAGE_तर्काः) : 0; }

पूर्णाङ्क Device::maxSamplers() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_गणः_आकार() ध्रुव
{ प्रत्यागम p ? p->max_Work_गणः_आकार_2 : 0; }

पूर्णाङ्क Device::maxWorkItemDims() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

शून्य Device::maxWorkItemSizes(size_t* sizes) ध्रुव
{
    यदि(p)
    {
        ध्रुव पूर्णाङ्क MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*परिमाण(sizes[0]), &sizes[0], &retsz));
    }
}

पूर्णाङ्क Device::mem_आधारः_Addr_Align() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_MEM_आधारः_ADDR_ALIGN) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Char() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_CHAR) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Short() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_SHORT) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Int() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_INT) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Long() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_LONG) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Float() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_FLOAT) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Double() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_DOUBLE) : 0; }

पूर्णाङ्क Device::native_सदिशम्_विस्तारः_Half() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_NATIVE_सदिशम्_विस्तारः_HALF) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Char() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_CHAR) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Short() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_SHORT) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Int() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_INT) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Long() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_LONG) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Float() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_FLOAT) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Double() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_DOUBLE) : 0; }

पूर्णाङ्क Device::preferred_सदिशम्_विस्तारः_Half() ध्रुव
{ प्रत्यागम p ? p->getProp<cl_uint, पूर्णाङ्क>(CL_DEVICE_PREFERRED_सदिशम्_विस्तारः_HALF) : 0; }

size_t Device::printf_बफर_आकार() ध्रुव
#यदि_परिभाषितम् CL_संस्करणम्_1_2
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_बफर_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#यदि_अन्तः


size_t Device::profilingTimerResolution() ध्रुव
{ प्रत्यागम p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ध्रुव Device& Device::getDefault()
{
    स्वचालित& c = OpenCLExecutionContext::getCurrent();
    यदि (!c.empty())
    {
        प्रत्यागम c.getDevice();
    }

    स्थिर Device dummy;
    प्रत्यागम dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

प्रतिरूपम् <प्रकारनाम Functor, प्रकारनाम ObjectType>
अन्तःस्थ cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, रिक्त, &required);
    यदि (err != CL_SUCCESS)
        प्रत्यागम err;

    param.clear();
    यदि (required > 0)
    {
        Auto_बफर<वर्ण> buf(required + 1);
        वर्ण* ptr = buf.दत्त(); // cleanup is not needed
        err = f(obj, name, required, ptr, रिक्त);
        यदि (err != CL_SUCCESS)
            प्रत्यागम err;
        param = ptr;
    }

    प्रत्यागम CL_SUCCESS;
}

स्थिर शून्य विभज(ध्रुव std::string &s, वर्ण delim, std::vector<std::string> &elems)
{
    elems.clear();
    यदि (s.size() == 0)
        प्रत्यागम;
    std::istringstream ss(s);
    std::string item;
    यावत् (!ss.eof())
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
    विभज(configurationStr, ':', parts);
    यदि (parts.size() > 3)
    {
        CV_LOG_ERROR(रिक्त, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        प्रत्यागम असत्य;
    }
    यदि (parts.size() > 2)
        deviceNameOrID = parts[2];
    यदि (parts.size() > 1)
    {
        विभज(parts[1], '|', deviceTypes);
    }
    यदि (parts.size() > 0)
    {
        platform = parts[0];
    }
    प्रत्यागम सत्य;
}

स्थिर cl_device_id selectOpenCLDevice(ध्रुव std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    यदि (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    यदि (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        प्रत्यागम रिक्त;

    तार्किक isID = असत्य;
    पूर्णाङ्क deviceID = -1;
    यदि (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = सत्य;
        प्रति (size_t i = 0; i < deviceName.length(); i++)
        {
            यदि (!अङ्कः_वा(deviceName[i]))
            {
                isID = असत्य;
                भङ्ग;
            }
        }
        यदि (isID)
        {
            deviceID = atoi(deviceName.c_str());
            यदि (deviceID < 0)
                प्रत्यागम रिक्त;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint अङ्कः_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms));

        यदि (अङ्कः_Platforms == 0)
            प्रत्यागम रिक्त;
        platforms.resize((size_t)अङ्कः_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], &अङ्कः_Platforms));
        platforms.resize(अङ्कः_Platforms);
    }

    यदि (platform.length() > 0)
    {
        प्रति (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            यदि (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            अन्यथा
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        यदि (platforms.size() == 0)
        {
            CV_LOG_ERROR(रिक्त, "OpenCL: Can't find OpenCL platform by name: " << platform);
            गच्छ not_प्राप्तम्;
        }
    }
    यदि (deviceTypes.size() == 0)
    {
        यदि (!isID)
        {
            deviceTypes.push_back("GPU");
            यदि (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        अन्यथा
            deviceTypes.push_back("ALL");
    }
    प्रति (size_t t = 0; t < deviceTypes.size(); t++)
    {
        पूर्णाङ्क deviceType = 0;
        std::string अस्थायी_Str_Device_Type = deviceTypes[t];
        std::transform(अस्थायी_Str_Device_Type.begin(), अस्थायी_Str_Device_Type.end(), अस्थायी_Str_Device_Type.begin(), details::char_tolower);

        यदि (अस्थायी_Str_Device_Type == "gpu" || अस्थायी_Str_Device_Type == "dgpu" || अस्थायी_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        अन्यथा यदि (अस्थायी_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        अन्यथा यदि (अस्थायी_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        अन्यथा यदि (अस्थायी_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        अन्यथा
        {
            CV_LOG_ERROR(रिक्त, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            गच्छ not_प्राप्तम्;
        }

        std::vector<cl_device_id> devices;
        प्रति (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint गणनम्_2 = 0;
            cl_int स्थितिः = clGetDeviceIDs(*currentPlatform, deviceType, 0, रिक्त, &गणनम्_2);
            यदि (!(स्थितिः == CL_SUCCESS || स्थितिः == CL_DEVICE_NOT_प्राप्तम्))
            {
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, "clGetDeviceIDs get count");
            }
            यदि (गणनम्_2 == 0)
                अनुवर्त;
            size_t आधारः = devices.size();
            devices.resize(आधारः + गणनम्_2);
            स्थितिः = clGetDeviceIDs(*currentPlatform, deviceType, गणनम्_2, &devices[आधारः], &गणनम्_2);
            यदि (!(स्थितिः == CL_SUCCESS || स्थितिः == CL_DEVICE_NOT_प्राप्तम्))
            {
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, "clGetDeviceIDs get IDs");
            }
        }

        प्रति (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : सत्य) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = सत्य;
            यदि(अस्थायी_Str_Device_Type == "dgpu" || अस्थायी_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_स्मृतिः, परिमाण(isIGPU), &isIGPU, रिक्त));
                useGPU = अस्थायी_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            यदि ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                प्रत्यागम devices[i];
            }
        }
    }

not_प्राप्तम्:
    यदि (configuration.empty())
        प्रत्यागम रिक्त; // suppress messages on stderr

    std::ostringstream सन्देशः;
    सन्देशः << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::पङ्क्त्यन्तः
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::पङ्क्त्यन्तः
        << "    Device types:";
    प्रति (size_t t = 0; t < deviceTypes.size(); t++)
        सन्देशः << ' ' << deviceTypes[t];

    सन्देशः << std::पङ्क्त्यन्तः << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(रिक्त, सन्देशः.str());
    प्रत्यागम रिक्त;
}

#यदि_परिभाषितम् HAVE_OPENCL_SVM
नामाकाश svm {

गणना AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_बफर = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_बफर = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_बफर_आवरणम् = 3 << 16,
        OPENCL_SVM_बफर_MAP = 4 << 16
};

स्थिर तार्किक checkForceSVMUmatUsage()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक force = असत्य;
    यदि (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम force;
}
स्थिर तार्किक checkDisableSVMUMatUsage()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक force = असत्य;
    यदि (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम force;
}
स्थिर तार्किक checkDisableSVM()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक force = असत्य;
    यदि (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम force;
}
// see SVMCapabilities
स्थिर अचिह्नित पूर्णाङ्क get_SVMCapabilities_आवरणम्()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर अचिह्नित पूर्णाङ्क आवरणम् = 0;
    यदि (!initialized)
    {
        ध्रुव std::string env_मानम् = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        यदि (env_मानम्.empty())
        {
            प्रत्यागम ~0U; // all bits 1
        }
        आवरणम् = atoi(env_मानम्.c_str());
        initialized = सत्य;
    }
    प्रत्यागम आवरणम्;
}
} // namespace
#यदि_अन्तः

स्थिर size_t get_क्रमादेशः_गणनम्_सीमा()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर size_t गणनम्_2 = 0;
    यदि (!initialized)
    {
        गणनम्_2 = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = सत्य;
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
#यदि_परिभाषितम् HAVE_OPENCL_SVM
        , svmInitialized(असत्य)
#यदि_अन्तः
    {
        यदि (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_तालकम् lock(cv::getInitializationMutex());
        स्वचालित& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = एतत्;
    }

    ~Impl()
    {
#यदि_परिभाषितम् _WIN32
        यदि (!cv::__termination)
#यदि_अन्तः
        {
            यदि (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = रिक्त;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::Auto_तालकम् lock(cv::getInitializationMutex());
            स्वचालित& container = getGlobalContainer();
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

        cv::Auto_बफर<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_आकार = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * परिमाण(cl_device_id), &cl_devices[0], &devices_ret_आकार));
        CV_CheckEQ(devices_ret_आकार, cl_devices.size() * परिमाण(cl_device_id), "");

        devices.clear();
        प्रति (अचिह्नित i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    शून्य __init_buffer_pools();  // w/o synchronization
    शून्य init_बफर_pools() ध्रुव
    {
        यदि (!बफर_Pool_2)
        {
            cv::Auto_तालकम् lock(cv::getInitializationMutex());
            यदि (!बफर_Pool_2)
            {
                const_cast<Impl*>(एतत्)->__init_buffer_pools();
            }
        }
    }
सार्वजनिक:
    स्थिर Impl* findContext(ध्रुव std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_तालकम् lock(cv::getInitializationMutex());
        स्वचालित& container = getGlobalContainer();
        यदि (configuration.empty() && !container.empty())
            प्रत्यागम container[0];
        प्रति (स्वचालित it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            यदि (i && i->configuration == configuration)
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
        यदि (configuration_.empty())
        {
            ध्रुव std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            यदि (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        यदि (impl)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            प्रत्यागम impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        यदि (d == रिक्त)
            प्रत्यागम रिक्त;

        impl = नूतन Impl(configuration);
        यत्न
        {
            impl->createFromDevice(d);
            यदि (impl->handle)
                प्रत्यागम impl;
            विलोपय impl;
            प्रत्यागम रिक्त;
        }
        गृहाण (...)
        {
            विलोपय impl;
            क्षिप;
        }
    }

    स्थिर Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (शून्य*)h);
        Impl* impl = findContext(configuration);
        यदि (impl)
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
            विलोपय impl;
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
        यदि (impl)
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
            विलोपय impl;
            क्षिप;
        }
    }

    शून्य समुच्चयः_Default()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        यदि (d == रिक्त)
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
        cl_int स्थितिः;

        handle = clCreateContext(prop, nd, &d, 0, 0, &स्थितिः);
        CV_OCL_DBG_CHECK_परिणाम(स्थितिः, "clCreateContext");

        तार्किक ok = handle != 0 && स्थितिः == CL_SUCCESS;
        यदि( ok )
        {
            devices.resize(nd);
            devices[0].समुच्चयः(d);
        }
        अन्यथा
            handle = रिक्त;
    }

    क्रमादेशः_2 getProg(ध्रुव क्रमादेशः_Source& src, ध्रुव String& buildflags, String& errmsg);

    शून्य unloadProg(क्रमादेशः_2& prog)
    {
        cv::Auto_तालकम् lock(क्रमादेशः_cache_mutex);
        प्रति (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              यदि (it != phash.end())
              {
                  यदि (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      प्रत्यागम;
                  }
              }
        }
    }

    std::string& get_उपसर्गः_String()
    {
        यदि (उपसर्गः.empty())
        {
            cv::Auto_तालकम् lock(क्रमादेशः_cache_mutex);
            यदि (उपसर्गः.empty())
            {
                CV_Assert(!devices.empty());
                ध्रुव Device& d = devices[0];
                पूर्णाङ्क द्वयङ्काः_2 = d.स्थानाङ्कः_द्वयङ्काः();
                यदि (द्वयङ्काः_2 > 0 && द्वयङ्काः_2 != 64)
                    उपसर्गः = cv::format("%d-bit--", द्वयङ्काः_2);
                उपसर्गः += d.vendorName() + "--" + d.name() + "--" + d.driver_संस्करणम्();
                // sanitize chars
                प्रति (size_t i = 0; i < उपसर्गः.size(); i++)
                {
                    वर्ण c = उपसर्गः[i];
                    यदि (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        उपसर्गः[i] = '_';
                    }
                }
            }
        }
        प्रत्यागम उपसर्गः;
    }

    std::string& get_उपसर्गः_आधारः()
    {
        यदि (उपसर्गः_आधारः.empty())
        {
            cv::Auto_तालकम् lock(क्रमादेशः_cache_mutex);
            यदि (उपसर्गः_आधारः.empty())
            {
                ध्रुव Device& d = devices[0];
                पूर्णाङ्क द्वयङ्काः_2 = d.स्थानाङ्कः_द्वयङ्काः();
                यदि (द्वयङ्काः_2 > 0 && द्वयङ्काः_2 != 64)
                    उपसर्गः_आधारः = cv::format("%d-bit--", द्वयङ्काः_2);
                उपसर्गः_आधारः += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                प्रति (size_t i = 0; i < उपसर्गः_आधारः.size(); i++)
                {
                    वर्ण c = उपसर्गः_आधारः[i];
                    यदि (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        उपसर्गः_आधारः[i] = '_';
                    }
                }
            }
        }
        प्रत्यागम उपसर्गः_आधारः;
    }

    IMPLEMENT_REFCOUNTABLE();

    ध्रुव पूर्णाङ्क contextId;  // global unique ID
    ध्रुव std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string उपसर्गः;
    std::string उपसर्गः_आधारः;

    cv::Mutex क्रमादेशः_cache_mutex;
    प्रकारनिर्देश std::map<std::string, क्रमादेशः_2> phash_t;
    phash_t phash;
    प्रकारनिर्देश std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> बफर_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> बफर_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_बफर_Pool() ध्रुव
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_2);
        प्रत्यागम *बफर_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_बफर_Pool_Host_Ptr() ध्रुव
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_Host_Ptr);
        प्रत्यागम *बफर_Pool_Host_Ptr.get();
    }

    std::map<std::type_सूचकांक, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    शून्य समुच्चयः_User_Context(std::type_सूचकांक typeId, ध्रुव std::shared_ptr<UserContext>& userContext) {
        cv::Auto_तालकम् lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_सूचकांक typeId) {
        cv::Auto_तालकम् lock(userContextMutex);
        स्वचालित it = userContextStorage.find(typeId);
        यदि (it != userContextStorage.end())
            प्रत्यागम it->second;
        अन्यथा
            प्रत्यागम शून्यसूचकः;
    }

#यदि_परिभाषितम् HAVE_OPENCL_SVM
    तार्किक svmInitialized;
    तार्किक svm_उपलब्धम्;
    तार्किक svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    शून्य svmInit()
    {
        CV_Assert(handle != रिक्त);
        ध्रुव Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((शून्य)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int स्थितिः = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, परिमाण(deviceCaps), &deviceCaps, रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", स्थितिः);
            गच्छ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (पूर्णाङ्क)deviceCaps);
        CV_Assert(((शून्य)0, CL_DEVICE_SVM_COARSE_GRAIN_बफर == CL_DEVICE_SVM_COARSE_GRAIN_बफर_AMD)); // Check assumption
        svmCapabilities.मानम्_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_बफर) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_बफर : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_बफर) ? svm::SVMCapabilities::SVM_FINE_GRAIN_बफर : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.मानम्_2 &= svm::get_SVMCapabilities_आवरणम्();
        यदि (svmCapabilities.मानम्_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            गच्छ noSVM;
        }
        यत्न
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            शून्य* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            यदि (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            यत्न
            {
                तार्किक error = असत्य;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                यदि (CL_SUCCESS != cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, रिक्त, रिक्त))
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
                    error = सत्य;
                }
                यदि (CL_SUCCESS != cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, ptr, 0, रिक्त, रिक्त))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                यदि (error)
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
            svmFunctions.fn_cl_समुच्चयः_Kernel_Arg_SVMPointer = cl_समुच्चयः_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMemcpy = cl_पङ्क्तौ_निक्षिप_SVMMemcpy;
            svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMem_Fill = cl_पङ्क्तौ_निक्षिप_SVMMem_Fill;
            svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMap = cl_पङ्क्तौ_निक्षिप_SVMMap;
            svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap = cl_पङ्क्तौ_निक्षिप_SVMUnmap;
        }
        गृहाण (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            यत्न
            {
                // Try HSA extension
                String extensions = device.extensions();
                यदि (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    गच्छ noSVM;
                }
                cl_platform_id p = रिक्त;
                CV_OCL_CHECK(स्थितिः = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, परिमाण(cl_platform_id), &p, रिक्त));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_समुच्चयः_Kernel_Arg_SVMPointer = (cl_समुच्चयः_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMemcpy = (cl_पङ्क्तौ_निक्षिप_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMem_Fill = (cl_पङ्क्तौ_निक्षिप_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMMap = (cl_पङ्क्तौ_निक्षिप_SVMMap_AMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap = (cl_पङ्क्तौ_निक्षिप_SVMUnmap_AMD_fn)cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            गृहाण (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                गच्छ noSVM;
            }
        }

        svm_उपलब्धम् = सत्य;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = सत्य;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        प्रत्यागम;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_उपलब्धम् = असत्य;
        svmEnabled = असत्य;
        svmCapabilities.मानम्_2 = 0;
        svmInitialized = सत्य;
        svmFunctions.fn_clSVMAlloc = रिक्त;
        प्रत्यागम;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> बफर_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_बफर_Pool_SVM() ध्रुव
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_SVM);
        प्रत्यागम *बफर_Pool_SVM.get();
    }
#यदि_अन्तः

    मित्रम् वर्ग क्रमादेशः_2;
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
    यदि (p)
    {
        p->release();
        p = रिक्त;
    }
}

तार्किक Context::create()
{
    release();
    यदि (!haveOpenCL())
        प्रत्यागम असत्य;
    p = Impl::findOrCreateContext(std::string());
    यदि (p && p->handle)
        प्रत्यागम सत्य;
    release();
    प्रत्यागम असत्य;
}

// deprecated
तार्किक Context::create(पूर्णाङ्क dtype)
{
    यदि( !haveOpenCL() )
        प्रत्यागम असत्य;
    release();
    यदि (dtype == CL_DEVICE_TYPE_DEFAULT || (अचिह्नित)dtype == (अचिह्नित)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    अन्यथा यदि (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    अन्यथा यदि (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    अन्यथा
    {
        CV_LOG_ERROR(रिक्त, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    यदि (p && !p->handle)
    {
        release();
    }
    प्रत्यागम p != 0;
}

Context::Context(ध्रुव Context& c)
{
    p = (Impl*)c.p;
    यदि(p)
        p->addref();
}

Context& Context::संक्रियकः = (ध्रुव Context& c)
{
    Impl* newp = (Impl*)c.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = शून्यसूचकः;
}

Context& Context::संक्रियकः = (Context&& c) CV_NOEXCEPT
{
    यदि (एतत् != &c) {
        यदि(p)
            p->release();
        p = c.p;
        c.p = शून्यसूचकः;
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
    स्वचालित& c = OpenCLExecutionContext::getCurrent();
    यदि (!c.empty())
    {
        स्वचालित& ctx = c.getContext();
        प्रत्यागम ctx;
    }

    स्थिर Context dummy;
    प्रत्यागम dummy;
}

क्रमादेशः_2 Context::getProg(ध्रुव क्रमादेशः_Source& prog,
                         ध्रुव String& buildopts, String& errmsg)
{
    प्रत्यागम p ? p->getProg(prog, buildopts, errmsg) : क्रमादेशः_2();
}

शून्य Context::unloadProg(क्रमादेशः_2& prog)
{
    यदि (p)
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
    यदि (p == रिक्त)
        प्रत्यागम शून्यसूचकः;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, रिक्त, &size));
    std::vector<cl_context_properties> prop(size / परिमाण(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.दत्त(), रिक्त));
    प्रति (size_t i = 0; i < prop.size(); i += 2)
    {
        यदि (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(रिक्त, "OpenCL: found context property=" << propertyId << ") => " << (शून्य*)prop[i + 1]);
            प्रत्यागम (शून्य*)prop[i + 1];
        }
    }
    प्रत्यागम शून्यसूचकः;
}

#यदि_परिभाषितम् HAVE_OPENCL_SVM
तार्किक Context::useSVM() ध्रुव
{
    Context::Impl* i = p;
    CV_Assert(i);
    यदि (!i->svmInitialized)
        i->svmInit();
    प्रत्यागम i->svmEnabled;
}
शून्य Context::समुच्चयः_Use_SVM(तार्किक enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    यदि (!i->svmInitialized)
        i->svmInit();
    यदि (enabled && !i->svm_उपलब्धम्)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#अन्यथा
तार्किक Context::useSVM() ध्रुव { प्रत्यागम असत्य; }
शून्य Context::समुच्चयः_Use_SVM(तार्किक enabled) { CV_Assert(!enabled); }
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENCL_SVM
नामाकाश svm {

ध्रुव SVMCapabilities getSVMCapabilitites(ध्रुव ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    यदि (!i->svmInitialized)
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
    यदि (checkForceSVMUmatUsage())
        प्रत्यागम सत्य;
    यदि (checkDisableSVMUMatUsage())
        प्रत्यागम असत्य;
    यदि ((usageFlags & USAGE_ALLOCATE_SHARED_स्मृतिः) != 0)
        प्रत्यागम सत्य;
    प्रत्यागम असत्य; // don't use SVM by default
}

} // namespace cv::ocl::svm
#यदि_अन्तः // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

शून्य Context::समुच्चयः_User_Context(std::type_सूचकांक typeId, ध्रुव std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->समुच्चयः_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_सूचकांक typeId)
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
    Auto_बफर<वर्ण> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.दत्त(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.दत्त();
}

/*
// Attaches OpenCL context to OpenCV
*/
शून्य attachContext(ध्रुव String& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID)
{
    स्वचालित ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ध्रुव std::string& platformName, शून्य* platformID, शून्य* context, शून्य* deviceID
)
{
    यदि (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    यदि (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    तार्किक platform_उपलब्धम् = असत्य;

    // check if external platformName contained in list of available platforms in OpenCV
    प्रति (अचिह्नित पूर्णाङ्क i = 0; i < cnt; i++)
    {
        String उपलब्धम्_Platform_Name;
        get_platform_name(platforms[i], उपलब्धम्_Platform_Name);
        // external platform is found in the list of available platforms
        यदि (platformName == उपलब्धम्_Platform_Name)
        {
            platform_उपलब्धम् = सत्य;
            भङ्ग;
        }
    }

    यदि (!platform_उपलब्धम्)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    यदि (platformName != actualPlatformName)
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

    स्वचालित clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
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
        isProfilingQueue_ = असत्य;
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

    Impl(ध्रुव Context& c, ध्रुव Device& d, तार्किक withProfiling = असत्य)
    {
        __init();

        ध्रुव Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        यदि( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        यदि( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#यदि_परिभाषितम् _WIN32
        यदि (!cv::__termination)
#यदि_अन्तः
        {
            यदि(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = रिक्त;
            }
        }
    }

    ध्रुव cv::ocl::Queue& getProfilingQueue(ध्रुव cv::ocl::Queue& self)
    {
        यदि (isProfilingQueue_)
            प्रत्यागम self;

        यदि (profiling_queue_.ptr())
            प्रत्यागम profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, परिमाण(cl_context), &ctx, रिक्त));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, परिमाण(cl_device_id), &device, रिक्त));

        cl_int परिणाम = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &परिणाम);
        CV_OCL_DBG_CHECK_परिणाम(परिणाम, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = नूतन Impl(q, सत्य);
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
    यदि(p)
        p->addref();
}

Queue& Queue::संक्रियकः = (ध्रुव Queue& q)
{
    Impl* newp = (Impl*)q.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = शून्यसूचकः;
}

Queue& Queue::संक्रियकः = (Queue&& q) CV_NOEXCEPT
{
    यदि (एतत् != &q) {
        यदि(p)
            p->release();
        p = q.p;
        q.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

Queue::~Queue()
{
    यदि(p)
        p->release();
}

तार्किक Queue::create(ध्रुव Context& c, ध्रुव Device& d)
{
    यदि(p)
        p->release();
    p = नूतन Impl(c, d);
    प्रत्यागम p->handle != 0;
}

शून्य Queue::finish()
{
    यदि(p && p->handle)
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
    स्वचालित& c = OpenCLExecutionContext::getCurrent();
    यदि (!c.empty())
    {
        स्वचालित& q = c.getQueue();
        प्रत्यागम q;
    }
    स्थिर Queue dummy;
    प्रत्यागम dummy;
}

स्थिर cl_command_queue getQueue(ध्रुव Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    यदि(!qq)
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
    प्रत्यागम KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.योग()*m.elem_आकार());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

संरचना Kernel::Impl
{
    Impl(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_2& prog) :
        refcount(1), handle(रिक्त), is_अन्तर्_प्रगतिः(असत्य), is_Async_सततम्(असत्य), nu(0)
    {
        cl_क्रमादेशः ph = (cl_क्रमादेशः)prog.ptr();
        cl_int retval = 0;
        name = kname;
        यदि (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        प्रति( पूर्णाङ्क i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_अस्थायी_Dst_UMats = असत्य;
        have_अस्थायी_Src_UMats = असत्य;
    }

    शून्य cleanupUMats()
    {
        तार्किक exceptionOccurred = असत्य;
        प्रति( पूर्णाङ्क i = 0; i < MAX_ARRS; i++ )
        {
            यदि( u[i] )
            {
                यदि( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_दत्त::ASYNC_CLEANUP;
                    यत्न
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    गृहाण(ध्रुव std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(रिक्त, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = सत्य;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_अस्थायी_Dst_UMats = असत्य;
        have_अस्थायी_Src_UMats = असत्य;
        CV_Assert(!exceptionOccurred);
    }

    शून्य सङ्कलय_UMat(ध्रुव UMat& m, तार्किक dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        यदि(dst && m.u->अस्थायी_UMat())
            have_अस्थायी_Dst_UMats = सत्य;
        यदि(m.u->original_UMat_दत्त == रिक्त && m.u->अस्थायी_UMat())
            have_अस्थायी_Src_UMats = सत्य;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    शून्य पञ्जिकाकोशः_Image_तर्कः(पूर्णाङ्क arg, ध्रुव Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        यदि (arg < (पूर्णाङ्क)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_अन्तर्_प्रगतिः, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    शून्य finit(cl_event e)
    {
        CV_अप्रयुक्तम्(e);
        is_अन्तर्_प्रगतिः = असत्य;
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

    तार्किक सततम्(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
            तार्किक समकाल, int64* timeNS, ध्रुव Queue& q);

    ~Impl()
    {
        यदि(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    गणना { MAX_ARRS = 16 };
    UMat_दत्त* u[MAX_ARRS];
    तार्किक is_अन्तर्_प्रगतिः;
    तार्किक is_Async_सततम्;  // true if kernel was scheduled in async mode
    पूर्णाङ्क nu;
    std::vector<Image2D> shadow_images;
    तार्किक have_अस्थायी_Dst_UMats;
    तार्किक have_अस्थायी_Src_UMats;
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

Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_Source& src,
               ध्रुव String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(ध्रुव Kernel& k)
{
    p = k.p;
    यदि(p)
        p->addref();
}

Kernel& Kernel::संक्रियकः = (ध्रुव Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = शून्यसूचकः;
}

Kernel& Kernel::संक्रियकः = (Kernel&& k) CV_NOEXCEPT
{
    यदि (एतत् != &k) {
        यदि(p)
            p->release();
        p = k.p;
        k.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

Kernel::~Kernel()
{
    यदि(p)
        p->release();
}

तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_2& prog)
{
    यदि(p)
        p->release();
    p = नूतन Impl(kname, prog);
    यदि(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#यदि_परिभाषितम् CV_OPENCL_सततम्_ASSERT // check kernel compilation fails
    CV_Assert(p);
#यदि_अन्तः
    प्रत्यागम p != 0;
}

तार्किक Kernel::create(ध्रुव वर्ण* kname, ध्रुव क्रमादेशः_Source& src,
                    ध्रुव String& buildopts, String* errmsg)
{
    यदि(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    यदि( !errmsg ) errmsg = &tempmsg;
    ध्रुव क्रमादेशः_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
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
    यदि (!p)
        प्रत्यागम "NULL";
    यदि (sz == 2)
        प्रत्यागम cv::format("%d / %uu / 0x%04x", *(ह्रस्व*)p, *(अचिह्नित ह्रस्व*)p, *(ह्रस्व*)p);
    यदि (sz == 4)
        प्रत्यागम cv::format("%d / %uu / 0x%08x / %g", *(पूर्णाङ्क*)p, *(पूर्णाङ्क*)p, *(पूर्णाङ्क*)p, *(भिन्न*)p);
    यदि (sz == 8)
        प्रत्यागम cv::format("%lld / %lluu / 0x%16llx / %g", *(दीर्घ दीर्घ*)p, *(दीर्घ दीर्घ*)p, *(दीर्घ दीर्घ*)p, *(द्विगुण*)p);
    प्रत्यागम cv::format("%p", p);
}

पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव शून्य* value, size_t sz)
{
    यदि (!p || !p->handle)
        प्रत्यागम -1;
    यदि (i < 0)
        प्रत्यागम i;
    यदि( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)sz, dump_मानम्(sz, value).c_str()).c_str());
    यदि (retval != CL_SUCCESS)
        प्रत्यागम -1;
    प्रत्यागम i+1;
}

पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    पूर्णाङ्क res = समुच्चयः(i, &h, परिमाण(h));
    यदि (res >= 0)
        p->पञ्जिकाकोशः_Image_तर्कः(i, image2D);
    प्रत्यागम res;
}

पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव UMat& m)
{
    प्रत्यागम समुच्चयः(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
स्थिर तार्किक isCudaBacked(ध्रुव UMat_दत्त* u)
{
    ध्रुव MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    प्रत्यागम u && cudaAllocator && u->currAllocator == cudaAllocator;
}

पूर्णाङ्क Kernel::समुच्चयः(पूर्णाङ्क i, ध्रुव KernelArg& arg)
{
    यदि( !p || !p->handle )
        प्रत्यागम -1;
    यदि (i < 0)
    {
        CV_LOG_ERROR(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (पूर्णाङ्क)i));
        प्रत्यागम i;
    }
    यदि( i == 0 )
        p->cleanupUMats();
    cl_int स्थितिः = 0;
    यदि( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        तार्किक ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        यदि (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)रिक्त;
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, परिमाण(h_null), &h_null);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (पूर्णाङ्क)i).c_str());
            प्रत्यागम i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        यदि (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (पूर्णाङ्क)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            प्रत्यागम -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        यदि (!h)
        {
            CV_LOG_ERROR(रिक्त, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)arg.flags, arg.m));
            p->release();
            p = 0;
            प्रत्यागम -1;
        }

#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
        {
            ध्रुव Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_दत्त_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_दत्त_Ptr);
#यदि 1 // TODO
            स्थितिः = svmFns->fn_cl_समुच्चयः_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_दत्त_Ptr);
#अन्यथा
            स्थितिः = svmFns->fn_cl_समुच्चयः_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_दत्त_Ptr);
#यदि_अन्तः
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (पूर्णाङ्क)i, (शून्य*)svm_दत्त_Ptr).c_str());
        }
        अन्यथा
#यदि_अन्तः
        {
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, परिमाण(h), &h);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (पूर्णाङ्क)i, (शून्य*)h).c_str());
        }

        यदि (ptronly)
        {
            i++;
        }
        अन्यथा यदि( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+1), परिमाण(u2d.चरणः), &u2d.चरणः);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u2d.चरणः).c_str());
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+2), परिमाण(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)u2d.offset).c_str());
            i += 3;

            यदि( !(arg.flags & KernelArg::NO_आकार) )
            {
                पूर्णाङ्क स्तम्भसङ्ख्या = u2d.स्तम्भसङ्ख्या*arg.wscale/arg.iwscale;
                स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, परिमाण(u2d.पङ्क्तिकाः), &u2d.पङ्क्तिकाः);
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)u2d.पङ्क्तिकाः).c_str());
                स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+1), परिमाण(स्तम्भसङ्ख्या), &स्तम्भसङ्ख्या);
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)स्तम्भसङ्ख्या).c_str());
                i += 2;
            }
        }
        अन्यथा
        {
            UMat3D u3d(*arg.m);
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+1), परिमाण(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u3d.slicestep).c_str());
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+2), परिमाण(u3d.चरणः), &u3d.चरणः);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)u3d.चरणः).c_str());
            स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+3), परिमाण(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+3), (पूर्णाङ्क)u3d.offset).c_str());
            i += 4;
            यदि( !(arg.flags & KernelArg::NO_आकार) )
            {
                पूर्णाङ्क स्तम्भसङ्ख्या = u3d.स्तम्भसङ्ख्या*arg.wscale/arg.iwscale;
                स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, परिमाण(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)u3d.slices).c_str());
                स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+1), परिमाण(u3d.पङ्क्तिकाः), &u3d.पङ्क्तिकाः);
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+1), (पूर्णाङ्क)u3d.पङ्क्तिकाः).c_str());
                स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)(i+2), परिमाण(u3d.स्तम्भसङ्ख्या), &स्तम्भसङ्ख्या);
                CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णाङ्क)(i+2), (पूर्णाङ्क)स्तम्भसङ्ख्या).c_str());
                i += 3;
            }
        }
        p->सङ्कलय_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        प्रत्यागम i;
    }
    स्थितिः = cl_समुच्चयः_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_परिणाम(स्थितिः, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (पूर्णाङ्क)i, (पूर्णाङ्क)arg.sz, (शून्य*)arg.obj).c_str());
    प्रत्यागम i+1;
}

तार्किक Kernel::सततम्(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
                 तार्किक समकाल, ध्रुव Queue& q)
{
    यदि (!p)
        प्रत्यागम असत्य;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t योग = 1;
    CV_Assert(_globalsize != रिक्त);
    प्रति (पूर्णाङ्क i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(पूर्णाङ्क)(i>0)) : 1;
        CV_Assert( val > 0 );
        योग *= _globalsize[i];
        यदि (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_ऊर्ध्वम्(_globalsize[i], (अचिह्नित पूर्णाङ्क)val) * val;
    }
    CV_Assert(योग > 0);

    प्रत्यागम p->सततम्(dims, globalsize, _localsize, समकाल, रिक्त, q);
}


तार्किक Kernel::सततम्_2(पूर्णाङ्क dims, size_t _globalsize[], size_t _localsize[],
                  तार्किक समकाल, ध्रुव Queue& q)
{
    CV_Assert(p);
    प्रत्यागम p->सततम्(dims, _globalsize, _localsize, समकाल, रिक्त, q);
}


तार्किक Kernel::Impl::सततम्(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[],
        तार्किक समकाल, int64* timeNS, ध्रुव Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_सततम्(name.c_str());

    यदि (!handle)
    {
        CV_LOG_ERROR(रिक्त, "OpenCL kernel has zero handle: " << name);
        प्रत्यागम असत्य;
    }

    यदि (is_Async_सततम्)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_सततम् = !समकाल;

    यदि (is_अन्तर्_प्रगतिः)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#यदि CV_OPENCL_SYNC_सततम्_KERNELS
    समकाल = सत्य;
#यदि_अन्तः

    cl_command_queue qq = getQueue(q);
    यदि (have_अस्थायी_Dst_UMats)
        समकाल = सत्य;
    यदि (have_अस्थायी_Src_UMats)
        समकाल = सत्य;
    यदि (timeNS)
        समकाल = सत्य;
    cl_event asyncEvent = 0;
    cl_int retval = cl_पङ्क्तौ_निक्षिप_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           रिक्त, globalsize, localsize, 0, 0,
                                           (समकाल && !timeNS) ? 0 : &asyncEvent);
#यदि !CV_OPENCL_SHOW_सततम्_KERNELS
    यदि (retval != CL_SUCCESS)
#यदि_अन्तः
    {
        cv::String सन्देशः = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (पूर्णाङ्क)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        समकाल ? "true" : "false"
                        );
        यदि (retval != CL_SUCCESS)
        {
            सन्देशः = CV_OCL_API_ERROR_सन्देशः(retval, सन्देशः.c_str());
        }
#यदि CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_परिणाम(retval, सन्देशः.c_str());
#अन्यथा
        प्रारूप_लिख("%s\n", सन्देशः.c_str());
        fflush(stdout);
#यदि_अन्तः
    }
    यदि (समकाल || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        यदि (timeNS)
        {
            यदि (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, परिमाण(startTime), &startTime, रिक्त));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_अन्तः, परिमाण(stopTime), &stopTime, रिक्त));
                *timeNS = (int64)(stopTime - startTime);
            }
            अन्यथा
            {
                *timeNS = -1;
            }
        }
        cleanupUMats();
    }
    अन्यथा
    {
        addref();
        is_अन्तर्_प्रगतिः = सत्य;
        CV_OCL_CHECK(cl_समुच्चयः_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, एतत्));
    }
    यदि (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    प्रत्यागम retval == CL_SUCCESS;
}

तार्किक Kernel::सततम्_Task(तार्किक समकाल, ध्रुव Queue& q)
{
    यदि(!p || !p->handle || p->is_अन्तर्_प्रगतिः)
        प्रत्यागम असत्य;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_पङ्क्तौ_निक्षिप_Task(qq, p->handle, 0, 0, समकाल ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), समकाल ? "true" : "false").c_str());
    यदि (समकाल || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    अन्यथा
    {
        p->addref();
        p->is_अन्तर्_प्रगतिः = सत्य;
        CV_OCL_CHECK(cl_समुच्चयः_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    यदि (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    प्रत्यागम retval == CL_SUCCESS;
}

int64 Kernel::सततम्_Profiling(पूर्णाङ्क dims, size_t globalsize[], size_t localsize[], ध्रुव Queue& q_)
{
    CV_Assert(p && p->handle && !p->is_अन्तर्_प्रगतिः);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    तार्किक res = p->सततम्(dims, globalsize, localsize, सत्य, &timeNs, profilingQueue);
    प्रत्यागम res ? timeNs : -1;
}

size_t Kernel::work_गणः_आकार() ध्रुव
{
    यदि(!p || !p->handle)
        प्रत्यागम 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int स्थितिः = cl_Get_Kernel_Work_गणः_Info(p->handle, dev, CL_KERNEL_WORK_गणः_आकार, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(स्थितिः, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    प्रत्यागम स्थितिः == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_गणः_आकार_गुणजः() ध्रुव
{
    यदि(!p || !p->handle)
        प्रत्यागम 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int स्थितिः = cl_Get_Kernel_Work_गणः_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_गणः_आकार_गुणजः, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(स्थितिः, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    प्रत्यागम स्थितिः == CL_SUCCESS ? val : 0;
}

तार्किक Kernel::compile_Work_गणः_आकार(size_t wsz[]) ध्रुव
{
    यदि(!p || !p->handle || !wsz)
        प्रत्यागम 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int स्थितिः = cl_Get_Kernel_Work_गणः_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_गणः_आकार, परिमाण(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_परिणाम(स्थितिः, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    प्रत्यागम स्थितिः == CL_SUCCESS;
}

size_t Kernel::local_Mem_आकार() ध्रुव
{
    यदि(!p || !p->handle)
        प्रत्यागम 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int स्थितिः = cl_Get_Kernel_Work_गणः_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_आकार, परिमाण(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(स्थितिः, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    प्रत्यागम स्थितिः == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

संरचना क्रमादेशः_Source::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    गणना विधा_4 {
        क्रमादेशः_SOURCE_संहिता = 0,
        क्रमादेशः_BINARIES,
        क्रमादेशः_SPIR,
        क्रमादेशः_SPIRV
    } विधा_5;

    Impl(ध्रुव String& src)
    {
        init(क्रमादेशः_SOURCE_संहिता, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(ध्रुव String& module, ध्रुव String& name, ध्रुव String& संहिता_Str, ध्रुव String& संहिता_Hash)
    {
        init(क्रमादेशः_SOURCE_संहिता, module, name);
        initFromSource(संहिता_Str, संहिता_Hash);
    }

    /// reset fields
    शून्य init(गणना विधा_4 विधा, ध्रुव String& module, ध्रुव String& name)
    {
        refcount = 1;
        विधा_5 = विधा;
        module_ = module;
        name_ = name;

        sourceAddr_ = रिक्त;
        source_आकार = 0;
        isHashUpdated = असत्य;
    }

    शून्य initFromSource(ध्रुव String& संहिता_Str, ध्रुव String& संहिता_Hash)
    {
        संहिता_Str_2 = संहिता_Str;
        sourceHash_ = संहिता_Hash;
        यदि (sourceHash_.empty())
        {
            updateHash();
        }
        अन्यथा
        {
            isHashUpdated = सत्य;
        }
    }

    शून्य updateHash(ध्रुव वर्ण* hashStr = रिक्त)
    {
        यदि (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = सत्य;
            प्रत्यागम;
        }
        uint64 hash = 0;
        विकल्प (विधा_5)
        {
        पक्ष क्रमादेशः_SOURCE_संहिता:
            यदि (sourceAddr_)
            {
                CV_Assert(संहिता_Str_2.empty());
                hash = crc64(sourceAddr_, source_आकार); // static storage
            }
            अन्यथा
            {
                CV_Assert(!संहिता_Str_2.empty());
                hash = crc64((uchar*)संहिता_Str_2.c_str(), संहिता_Str_2.size());
            }
            भङ्ग;
        पक्ष क्रमादेशः_BINARIES:
        पक्ष क्रमादेशः_SPIR:
        पक्ष क्रमादेशः_SPIRV:
            hash = crc64(sourceAddr_, source_आकार);
            भङ्ग;
        स्वभाव:
            CV_Error(Error::Sts_आन्तरिकम्, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = सत्य;
    }

    Impl(गणना विधा_4 विधा,
            ध्रुव String& module, ध्रुव String& name,
            ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions = cv::String())
    {
        init(विधा, module, name);

        sourceAddr_ = binary;
        source_आकार = size;

        buildOptions_ = buildOptions;
    }

    स्थिर क्रमादेशः_Source fromSourceWithStaticLifetime(ध्रुव String& module, ध्रुव String& name,
            ध्रुव वर्ण* source_संहिता_Static_Str, ध्रुव वर्ण* hashStaticStr,
            ध्रुव cv::String& buildOptions)
    {
        क्रमादेशः_Source परिणाम;
        परिणाम.p = नूतन Impl(क्रमादेशः_SOURCE_संहिता, module, name,
                (ध्रुव अचिह्नित वर्ण*)source_संहिता_Static_Str, वाक्यदैर्घ्यम्(source_संहिता_Static_Str), buildOptions);
        परिणाम.p->updateHash(hashStaticStr);
        प्रत्यागम परिणाम;
    }

    स्थिर क्रमादेशः_Source from_द्विमान(ध्रुव String& module, ध्रुव String& name,
            ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions)
    {
        क्रमादेशः_Source परिणाम;
        परिणाम.p = नूतन Impl(क्रमादेशः_BINARIES, module, name, binary, size, buildOptions);
        प्रत्यागम परिणाम;
    }

    स्थिर क्रमादेशः_Source fromSPIR(ध्रुव String& module, ध्रुव String& name,
            ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size,
            ध्रुव cv::String& buildOptions)
    {
        क्रमादेशः_Source परिणाम;
        परिणाम.p = नूतन Impl(क्रमादेशः_SPIR, module, name, binary, size, buildOptions);
        प्रत्यागम परिणाम;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String संहिता_Str_2; // PROGRAM_SOURCE_CODE only

    ध्रुव अचिह्नित वर्ण* sourceAddr_;
    size_t source_आकार;

    cv::String buildOptions_;

    String sourceHash_;
    तार्किक isHashUpdated;

    मित्रम् संरचना क्रमादेशः_2::Impl;
    मित्रम् संरचना internal::क्रमादेशः_प्रविष्टिः;
    मित्रम् संरचना Context::Impl;
};


क्रमादेशः_Source::क्रमादेशः_Source() CV_NOEXCEPT
{
    p = 0;
}

क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव String& module, ध्रुव String& name, ध्रुव String& संहिता_Str, ध्रुव String& संहिता_Hash)
{
    p = नूतन Impl(module, name, संहिता_Str, संहिता_Hash);
}

क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव वर्ण* prog)
{
    p = नूतन Impl(prog);
}

क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव String& prog)
{
    p = नूतन Impl(prog);
}

क्रमादेशः_Source::~क्रमादेशः_Source()
{
    यदि(p)
        p->release();
}

क्रमादेशः_Source::क्रमादेशः_Source(ध्रुव क्रमादेशः_Source& prog)
{
    p = prog.p;
    यदि(p)
        p->addref();
}

क्रमादेशः_Source& क्रमादेशः_Source::संक्रियकः = (ध्रुव क्रमादेशः_Source& prog)
{
    Impl* newp = (Impl*)prog.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

क्रमादेशः_Source::क्रमादेशः_Source(क्रमादेशः_Source&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = शून्यसूचकः;
}

क्रमादेशः_Source& क्रमादेशः_Source::संक्रियकः = (क्रमादेशः_Source&& prog) CV_NOEXCEPT
{
    यदि (एतत् != &prog) {
        यदि(p)
            p->release();
        p = prog.p;
        prog.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

ध्रुव String& क्रमादेशः_Source::source() ध्रुव
{
    CV_Assert(p);
    CV_Assert(p->विधा_5 == Impl::क्रमादेशः_SOURCE_संहिता);
    CV_Assert(p->sourceAddr_ == रिक्त); // method returns reference - can't construct temporary object
    प्रत्यागम p->संहिता_Str_2;
}

क्रमादेशः_Source::hash_t क्रमादेशः_Source::hash() ध्रुव
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

क्रमादेशः_Source क्रमादेशः_Source::from_द्विमान(ध्रुव String& module, ध्रुव String& name,
        ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size,
        ध्रुव cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    प्रत्यागम Impl::from_द्विमान(module, name, binary, size, buildOptions);
}

क्रमादेशः_Source क्रमादेशः_Source::fromSPIR(ध्रुव String& module, ध्रुव String& name,
        ध्रुव अचिह्नित वर्ण* binary, ध्रुव size_t size,
        ध्रुव cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    प्रत्यागम Impl::from_द्विमान(module, name, binary, size, buildOptions);
}


internal::क्रमादेशः_प्रविष्टिः::संक्रियकः क्रमादेशः_Source&() ध्रुव
{
    यदि (एतत्->p_क्रमादेशः_Source == रिक्त)
    {
        cv::Auto_तालकम् lock(cv::getInitializationMutex());
        यदि (एतत्->p_क्रमादेशः_Source == रिक्त)
        {
            क्रमादेशः_Source ps = क्रमादेशः_Source::Impl::fromSourceWithStaticLifetime(एतत्->module, एतत्->name, एतत्->क्रमादेशः_संहिता, एतत्->क्रमादेशः_Hash, cv::String());
            क्रमादेशः_Source* ptr = नूतन क्रमादेशः_Source(ps);
            const_cast<क्रमादेशः_प्रविष्टिः*>(एतत्)->p_क्रमादेशः_Source = ptr;
        }
    }
    प्रत्यागम *एतत्->p_क्रमादेशः_Source;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

स्थिर
cv::String joinBuildOptions(ध्रुव cv::String& a, ध्रुव cv::String& b)
{
    यदि (b.empty())
        प्रत्यागम a;
    यदि (a.empty())
        प्रत्यागम b;
    यदि (b[0] == ' ')
        प्रत्यागम a + b;
    प्रत्यागम a + (cv::String(" ") + b);
}

संरचना क्रमादेशः_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ध्रुव क्रमादेशः_Source& src,
         ध्रुव String& _buildflags, String& errmsg) :
         refcount(1),
         handle(रिक्त),
         buildflags(_buildflags)
    {
        ध्रुव क्रमादेशः_Source::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        ध्रुव Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        यदि (ctx.ptr() == रिक्त || device.ptr() == रिक्त)
            प्रत्यागम;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        यदि (src.getImpl()->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_SOURCE_संहिता)
        {
            यदि (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            अन्यथा यदि (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ध्रुव String param_buildExtraOptions = getBuildExtraOptions();
            यदि (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#यदि CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#यदि_अन्तः
        compile(ctx, src_, errmsg);
#यदि CV_OPENCL_SHOW_BUILD_KERNELS
        यदि (handle)
        {
            size_t retsz = 0;
            वर्ण kernels_बफर[4096] = {0};
            cl_int परिणाम = cl_Get_क्रमादेशः_Info(handle, CL_क्रमादेशः_KERNEL_नामानि, परिमाण(kernels_बफर), &kernels_बफर[0], &retsz);
            CV_OCL_DBG_CHECK_परिणाम(परिणाम, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            यदि (परिणाम == CL_SUCCESS && retsz < परिमाण(kernels_बफर))
            {
                kernels_बफर[retsz] = 0;
                CV_LOG_INFO(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_बफर << "'");
            }
            अन्यथा
            {
                CV_LOG_ERROR(रिक्त, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#यदि_अन्तः
    }

    तार्किक compile(ध्रुव Context& ctx, ध्रुव क्रमादेशः_Source::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        यदि (src_->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_BINARIES)
        {
            CV_LOG_VERBOSE(रिक्त, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            तार्किक isLoaded = create_From_द्विमान(ctx, src_->sourceAddr_, src_->source_आकार, errmsg);
            प्रत्यागम isLoaded;
        }
        प्रत्यागम compileWithCache(ctx, src_, errmsg);
    }

    तार्किक compileWithCache(ध्रुव Context& ctx, ध्रुव क्रमादेशः_Source::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->विधा_5 != क्रमादेशः_Source::Impl::क्रमादेशः_BINARIES);

#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ध्रुव std::string आधारः_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_उपसर्गः_String(),
                ctx.getImpl()->get_उपसर्गः_आधारः()
        );
        ध्रुव String& hash_str = src_->sourceHash_;
        cv::String fname;
        यदि (!आधारः_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(आधारः_dir, fname);
        }
        ध्रुव cv::Ptr<utils::fs::File_तालकम्> file_तालकम् = config.cache_तालकम्; // can be empty
        यदि (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            यत्न
            {
                std::vector<वर्ण> द्विमान_Buf;
                तार्किक res = असत्य;
                {
                    cv::utils::optional_shared_तालकम्_guard<cv::utils::fs::File_तालकम्> तालकम्_fs(file_तालकम्.get());
                    द्विमान_क्रमादेशः_File file(fname, hash_str.c_str());
                    res = file.पठ(buildflags, द्विमान_Buf);
                }
                यदि (res)
                {
                    CV_Assert(!द्विमान_Buf.empty());
                    CV_LOG_VERBOSE(रिक्त, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    तार्किक isLoaded = create_From_द्विमान(ctx, द्विमान_Buf, errmsg);
                    यदि (isLoaded)
                        प्रत्यागम सत्य;
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_अप्रयुक्तम्(e);
                CV_LOG_VERBOSE(रिक्त, 0, "Can't load OpenCL binary: " + fname << std::पङ्क्त्यन्तः << e.what());
            }
            गृहाण (...)
            {
                CV_LOG_VERBOSE(रिक्त, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == रिक्त);
        यदि (src_->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_SOURCE_संहिता)
        {
            यदि (!buildFromSources(ctx, src_, errmsg))
            {
                प्रत्यागम असत्य;
            }
        }
        अन्यथा यदि (src_->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            यदि ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(रिक्त, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            तार्किक isLoaded = create_From_द्विमान(ctx, src_->sourceAddr_, src_->source_आकार, errmsg);
            यदि (!isLoaded)
                प्रत्यागम असत्य;
        }
        अन्यथा यदि (src_->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        अन्यथा
        {
            CV_Error(Error::Sts_आन्तरिकम्, "Internal error");
        }
        CV_Assert(handle != रिक्त);
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
        यदि (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            यत्न
            {
                std::vector<वर्ण> द्विमान_Buf;
                get_क्रमादेशः_द्विमान(द्विमान_Buf);
                {
                    cv::utils::optional_तालकम्_guard<cv::utils::fs::File_तालकम्> तालकम्_fs(file_तालकम्.get());
                    द्विमान_क्रमादेशः_File file(fname, hash_str.c_str());
                    file.लिख(buildflags, द्विमान_Buf);
                }
            }
            गृहाण (ध्रुव cv::Exception& e)
            {
                CV_LOG_WARNING(रिक्त, "Can't save OpenCL binary into cache: " + fname << std::पङ्क्त्यन्तः << e.what());
            }
            गृहाण (...)
            {
                CV_LOG_WARNING(रिक्त, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
#यदि CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
        यदि (CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
        {
            std::vector<वर्ण> द्विमान_Buf;
            get_क्रमादेशः_द्विमान(द्विमान_Buf);
            यदि (!द्विमान_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_क्रमादेशः(handle));
                handle = रिक्त;
                create_From_द्विमान(ctx, द्विमान_Buf, errmsg);
            }
        }
#यदि_अन्तः
        प्रत्यागम handle != रिक्त;
    }

    शून्य dumpBuildLog_(cl_int परिणाम, ध्रुव cl_device_id* deviceList, String& errmsg)
    {
        Auto_बफर<वर्ण, 4096> बफर; बफर[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_क्रमादेशः_Build_Info(handle, deviceList[0],
                                                  CL_क्रमादेशः_BUILD_LOG, 0, 0, &retsz);
        यदि (log_retval == CL_SUCCESS && retsz > 1)
        {
            बफर.resize(retsz + 16);
            log_retval = cl_Get_क्रमादेशः_Build_Info(handle, deviceList[0],
                                               CL_क्रमादेशः_BUILD_LOG, retsz+1, बफर.दत्त(), &retsz);
            यदि (log_retval == CL_SUCCESS)
            {
                यदि (retsz < बफर.size())
                    बफर[retsz] = 0;
                अन्यथा
                    बफर[बफर.size() - 1] = 0;
            }
            अन्यथा
            {
                बफर[0] = 0;
            }
        }

        errmsg = String(बफर.दत्त());
        प्रारूप_लिख("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                sourceModule_.c_str(), sourceName_.c_str(),
                परिणाम, getOpenCLErrorString(परिणाम),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    तार्किक buildFromSources(ध्रुव Context& ctx, ध्रुव क्रमादेशः_Source::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->विधा_5 == क्रमादेशः_Source::Impl::क्रमादेशः_SOURCE_संहिता);
        CV_Assert(handle == रिक्त);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(रिक्त, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        ध्रुव वर्ण* srcptr = src_->sourceAddr_ ? ((ध्रुव वर्ण*)src_->sourceAddr_) : src_->संहिता_Str_2.c_str();
        size_t srclen = src_->sourceAddr_ ? src_->source_आकार : src_->संहिता_Str_2.size();
        CV_Assert(srcptr != रिक्त);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_क्रमादेशः_With_Source((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_परिणाम(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        यदि (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_बफर<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.दत्त();
            प्रति (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_क्रमादेशः(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_परिणाम(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#यदि !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            यदि (retval != CL_SUCCESS)
#यदि_अन्तः
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                यदि (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_क्रमादेशः(handle));
                    handle = रिक्त;
                }
                यदि (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#यदि CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
            यदि (handle && CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
            {
                CV_LOG_INFO(रिक्त, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                वर्ण kernels_बफर[4096] = {0};
                cl_int परिणाम = cl_Get_क्रमादेशः_Info(handle, CL_क्रमादेशः_KERNEL_नामानि, परिमाण(kernels_बफर), &kernels_बफर[0], &retsz);
                यदि (retsz < परिमाण(kernels_बफर))
                    kernels_बफर[retsz] = 0;
                अन्यथा
                    kernels_बफर[0] = 0;
                CV_LOG_INFO(रिक्त, परिणाम << ": Kernels='" << kernels_बफर << "'");
            }
#यदि_अन्तः
        }
        प्रत्यागम handle != रिक्त;
    }

    शून्य get_क्रमादेशः_द्विमान(std::vector<वर्ण>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_क्रमादेशः_Info(handle, CL_क्रमादेशः_द्विमान_SIZES, परिमाण(sz), &sz, रिक्त));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_क्रमादेशः_Info(handle, CL_क्रमादेशः_BINARIES, परिमाण(ptr), &ptr, रिक्त));
    }

    तार्किक create_From_द्विमान(ध्रुव Context& ctx, ध्रुव std::vector<वर्ण>& buf, String& errmsg)
    {
        प्रत्यागम create_From_द्विमान(ctx, (ध्रुव अचिह्नित वर्ण*)&buf[0], buf.size(), errmsg);
    }

    तार्किक create_From_द्विमान(ध्रुव Context& ctx, ध्रुव अचिह्नित वर्ण* द्विमान_Addr, ध्रुव size_t द्विमान_आकार, String& errmsg)
    {
        CV_Assert(handle == रिक्त);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(रिक्त, 0, "Load from binary... (" << द्विमान_आकार << " bytes)");

        CV_Assert(द्विमान_आकार > 0);

        size_t ndevices = (पूर्णाङ्क)ctx.ndevices();
        Auto_बफर<cl_device_id> devices_(ndevices);
        Auto_बफर<ध्रुव uchar*> द्विमान_Ptrs(ndevices);
        Auto_बफर<size_t> द्विमान_Sizes(ndevices);

        cl_device_id* devices = devices_.दत्त();
        ध्रुव uchar** द्विमान_Ptrs_2 = द्विमान_Ptrs.दत्त();
        size_t* द्विमान_Sizes_2 = द्विमान_Sizes.दत्त();
        प्रति (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            द्विमान_Ptrs_2[i] = द्विमान_Addr;
            द्विमान_Sizes_2[i] = द्विमान_आकार;
        }

        cl_int परिणाम = 0;
        handle = cl_Create_क्रमादेशः_With_द्विमान((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.दत्त(),
                                           द्विमान_Sizes_2, द्विमान_Ptrs_2, रिक्त, &परिणाम);
        यदि (परिणाम != CL_SUCCESS)
        {
            CV_LOG_ERROR(रिक्त, CV_OCL_API_ERROR_सन्देशः(परिणाम, "clCreateProgramWithBinary"));
            यदि (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_क्रमादेशः(handle));
                handle = रिक्त;
            }
        }
        यदि (!handle)
        {
            प्रत्यागम असत्य;
        }
        // call clBuildProgram()
        {
            परिणाम = cl_Build_क्रमादेशः(handle, (cl_uint)ndevices, devices_.दत्त(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_परिणाम(परिणाम, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            यदि (परिणाम != CL_SUCCESS)
            {
                dumpBuildLog_(परिणाम, devices, errmsg);
                यदि (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_क्रमादेशः(handle));
                    handle = रिक्त;
                }
                प्रत्यागम असत्य;
            }
        }
        // check build status
        {
            cl_build_स्थितिः build_स्थितिः = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(परिणाम = cl_Get_क्रमादेशः_Build_Info(handle, devices[0], CL_क्रमादेशः_BUILD_स्थितिः,
                    परिमाण(build_स्थितिः), &build_स्थितिः, &retsz));
            यदि (परिणाम == CL_SUCCESS)
            {
                यदि (build_स्थितिः == CL_BUILD_SUCCESS)
                {
                    प्रत्यागम सत्य;
                }
                अन्यथा
                {
                    CV_LOG_WARNING(रिक्त, "clGetProgramBuildInfo() returns " << build_स्थितिः);
                    प्रत्यागम असत्य;
                }
            }
            अन्यथा
            {
                CV_LOG_ERROR(रिक्त, CV_OCL_API_ERROR_सन्देशः(परिणाम, "clGetProgramBuildInfo()"));
                यदि (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_क्रमादेशः(handle));
                    handle = रिक्त;
                }
            }
        }
#यदि CV_OPENCL_VALIDATE_द्विमान_PROGRAMS
        यदि (handle && CV_OPENCL_VALIDATE_द्विमान_PROGRAMS_मानम्)
        {
            CV_LOG_INFO(रिक्त, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            वर्ण kernels_बफर[4096] = {0};
            परिणाम = cl_Get_क्रमादेशः_Info(handle, CL_क्रमादेशः_KERNEL_नामानि, परिमाण(kernels_बफर), &kernels_बफर[0], &retsz);
            यदि (retsz < परिमाण(kernels_बफर))
                kernels_बफर[retsz] = 0;
            अन्यथा
                kernels_बफर[0] = 0;
            CV_LOG_INFO(रिक्त, परिणाम << ": Kernels='" << kernels_बफर << "'");
        }
#यदि_अन्तः
        प्रत्यागम handle != रिक्त;
    }

    ~Impl()
    {
        यदि( handle )
        {
#यदि_परिभाषितम् _WIN32
            यदि (!cv::__termination)
#यदि_अन्तः
            {
                cl_Release_क्रमादेशः(handle);
            }
            handle = रिक्त;
        }
    }

    cl_क्रमादेशः handle;

    String buildflags;
    String sourceModule_;
    String sourceName_;
};


क्रमादेशः_2::क्रमादेशः_2() CV_NOEXCEPT
{
    p = 0;
}

क्रमादेशः_2::क्रमादेशः_2(ध्रुव क्रमादेशः_Source& src,
        ध्रुव String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

क्रमादेशः_2::क्रमादेशः_2(ध्रुव क्रमादेशः_2& prog)
{
    p = prog.p;
    यदि(p)
        p->addref();
}

क्रमादेशः_2& क्रमादेशः_2::संक्रियकः = (ध्रुव क्रमादेशः_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    यदि(newp)
        newp->addref();
    यदि(p)
        p->release();
    p = newp;
    प्रत्यागम *एतत्;
}

क्रमादेशः_2::क्रमादेशः_2(क्रमादेशः_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = शून्यसूचकः;
}

क्रमादेशः_2& क्रमादेशः_2::संक्रियकः = (क्रमादेशः_2&& prog) CV_NOEXCEPT
{
    यदि (एतत् != &prog) {
        यदि(p)
            p->release();
        p = prog.p;
        prog.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

क्रमादेशः_2::~क्रमादेशः_2()
{
    यदि(p)
        p->release();
}

तार्किक क्रमादेशः_2::create(ध्रुव क्रमादेशः_Source& src,
            ध्रुव String& buildflags, String& errmsg)
{
    यदि(p)
    {
        p->release();
        p = रिक्त;
    }
    p = नूतन Impl(src, buildflags, errmsg);
    यदि(!p->handle)
    {
        p->release();
        p = 0;
    }
    प्रत्यागम p != 0;
}

शून्य* क्रमादेशः_2::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

#यदि_अपरिभाषितम् OPENCV_REMOVE_DEPRECATED_API
ध्रुव क्रमादेशः_Source& क्रमादेशः_2::source() ध्रुव
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

तार्किक क्रमादेशः_2::पठ(ध्रुव String& bin, ध्रुव String& buildflags)
{
    CV_अप्रयुक्तम्(bin); CV_अप्रयुक्तम्(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

तार्किक क्रमादेशः_2::लिख(String& bin) ध्रुव
{
    CV_अप्रयुक्तम्(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String क्रमादेशः_2::get_उपसर्गः() ध्रुव
{
    यदि(!p)
        प्रत्यागम String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    प्रत्यागम cv::format("opencl=%s\nbuildflags=%s", ctx_->get_उपसर्गः_String().c_str(), p->buildflags.c_str());
}

String क्रमादेशः_2::get_उपसर्गः(ध्रुव String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        प्रत्यागम cv::format("opencl=%s\nbuildflags=%s", ctx_->get_उपसर्गः_String().c_str(), buildflags.c_str());
}
#यदि_अन्तः // OPENCV_REMOVE_DEPRECATED_API

शून्य क्रमादेशः_2::get_द्विमान(std::vector<वर्ण>& binary) ध्रुव
{
    CV_Assert(p && "Empty program");
    p->get_क्रमादेशः_द्विमान(binary);
}

क्रमादेशः_2 Context::Impl::getProg(ध्रुव क्रमादेशः_Source& src,
                               ध्रुव String& buildflags, String& errmsg)
{
    size_t सीमा = get_क्रमादेशः_गणनम्_सीमा();
    ध्रुव क्रमादेशः_Source::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String कुञ्चिका = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            get_उपसर्गः_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_तालकम् lock(क्रमादेशः_cache_mutex);
        phash_t::iterator it = phash.find(कुञ्चिका);
        यदि (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), कुञ्चिका);
            यदि (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(कुञ्चिका);
            }
            प्रत्यागम it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            यदि (सीमा > 0 && sz >= सीमा)
            {
                स्थिर तार्किक warningFlag = असत्य;
                यदि (!warningFlag)
                {
                    प्रारूप_लिख("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = सत्य;
                }
                यावत् (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    यदि (c != 0)
                        भङ्ग;
                }
            }
        }
    }
    क्रमादेशः_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_तालकम् lock(क्रमादेशः_cache_mutex);
        phash.insert(std::pair<std::string, क्रमादेशः_2>(कुञ्चिका, prog));
        cacheList.push_front(कुञ्चिका);
    }
    प्रत्यागम prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

प्रतिरूपम्<प्रकारनाम T>
वर्ग OpenCLBufferPool
{
रक्षित:
    ~OpenCLBufferPool() { }
सार्वजनिक:
    आभास T allocate(size_t size) = 0;
    आभास शून्य release(T बफर) = 0;
};

प्रतिरूपम् <प्रकारनाम Derived, प्रकारनाम बफर_प्रविष्टिः, प्रकारनाम T>
वर्ग Open_CLBuffer_Pool_आधारः_Impl : सार्वजनिक बफर_Pool_Controller, सार्वजनिक OpenCLBufferPool<T>
{
गुप्त:
    अन्तःस्थ Derived& derived() { प्रत्यागम *static_cast<Derived*>(एतत्); }
रक्षित:
    Mutex mutex_;

    size_t current_Reserved_आकार;
    size_t max_Reserved_आकार;

    std::list<बफर_प्रविष्टिः> allocatedEntries_; // Allocated and used entries
    std::list<बफर_प्रविष्टिः> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    तार्किक find_And_Remove_प्रविष्टिः_From_Allocated_List(CV_बहिर्गमनम् बफर_प्रविष्टिः& प्रविष्टिः, T बफर)
    {
        प्रकारनाम std::list<बफर_प्रविष्टिः>::iterator i = allocatedEntries_.begin();
        प्रति (; i != allocatedEntries_.end(); ++i)
        {
            बफर_प्रविष्टिः& e = *i;
            यदि (e.cl_बफर_2 == बफर)
            {
                प्रविष्टिः = e;
                allocatedEntries_.erase(i);
                प्रत्यागम सत्य;
            }
        }
        प्रत्यागम असत्य;
    }

    // synchronized
    तार्किक find_And_Remove_प्रविष्टिः_From_Reserved_List(CV_बहिर्गमनम् बफर_प्रविष्टिः& प्रविष्टिः, ध्रुव size_t size)
    {
        यदि (reservedEntries_.empty())
            प्रत्यागम असत्य;
        प्रकारनाम std::list<बफर_प्रविष्टिः>::iterator i = reservedEntries_.begin();
        प्रकारनाम std::list<बफर_प्रविष्टिः>::iterator परिणाम_pos = reservedEntries_.end();
        बफर_प्रविष्टिः परिणाम;
        size_t minDiff = (size_t)(-1);
        प्रति (; i != reservedEntries_.end(); ++i)
        {
            बफर_प्रविष्टिः& e = *i;
            यदि (e.क्षमता >= size)
            {
                size_t diff = e.क्षमता - size;
                यदि (diff < std::max((size_t)4096, size / 8) && (परिणाम_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    परिणाम_pos = i;
                    परिणाम = e;
                    यदि (diff == 0)
                        भङ्ग;
                }
            }
        }
        यदि (परिणाम_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(परिणाम_pos);
            प्रविष्टिः = परिणाम;
            current_Reserved_आकार -= प्रविष्टिः.क्षमता;
            allocatedEntries_.push_back(प्रविष्टिः);
            प्रत्यागम सत्य;
        }
        प्रत्यागम असत्य;
    }

    // synchronized
    शून्य check_आकार_सम्बन्धि_Reserved_Entries()
    {
        यावत् (current_Reserved_आकार > max_Reserved_आकार)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ध्रुव बफर_प्रविष्टिः& प्रविष्टिः = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_आकार >= प्रविष्टिः.क्षमता);
            current_Reserved_आकार -= प्रविष्टिः.क्षमता;
            derived().release_बफर_प्रविष्टिः(प्रविष्टिः);
            reservedEntries_.pop_back();
        }
    }

    अन्तःस्थ size_t आवण्टनम्_Granularity(size_t size)
    {
        // heuristic values
        यदि (size < 1024*1024)
            प्रत्यागम 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        अन्यथा यदि (size < 16*1024*1024)
            प्रत्यागम 64*1024;
        अन्यथा
            प्रत्यागम 1024*1024;
    }

सार्वजनिक:
    Open_CLBuffer_Pool_आधारः_Impl()
        : current_Reserved_आकार(0),
          max_Reserved_आकार(0)
    {
        // nothing
    }
    आभास ~Open_CLBuffer_Pool_आधारः_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
सार्वजनिक:
    आभास T allocate(size_t size) CV_OVERRIDE
    {
        Auto_तालकम् locker(mutex_);
        बफर_प्रविष्टिः प्रविष्टिः;
        यदि (max_Reserved_आकार > 0 && find_And_Remove_प्रविष्टिः_From_Reserved_List(प्रविष्टिः, size))
        {
            CV_DbgAssert(size <= प्रविष्टिः.क्षमता);
            LOG_बफर_POOL("Reuse reserved buffer: %p\n", प्रविष्टिः.cl_बफर_2);
        }
        अन्यथा
        {
            derived().allocate_बफर_प्रविष्टिः(प्रविष्टिः, size);
        }
        प्रत्यागम प्रविष्टिः.cl_बफर_2;
    }
    आभास शून्य release(T बफर) CV_OVERRIDE
    {
        Auto_तालकम् locker(mutex_);
        बफर_प्रविष्टिः प्रविष्टिः;
        CV_Assert(find_And_Remove_प्रविष्टिः_From_Allocated_List(प्रविष्टिः, बफर));
        यदि (max_Reserved_आकार == 0 || प्रविष्टिः.क्षमता > max_Reserved_आकार / 8)
        {
            derived().release_बफर_प्रविष्टिः(प्रविष्टिः);
        }
        अन्यथा
        {
            reservedEntries_.push_front(प्रविष्टिः);
            current_Reserved_आकार += प्रविष्टिः.क्षमता;
            check_आकार_सम्बन्धि_Reserved_Entries();
        }
    }

    आभास size_t get_Reserved_आकार() ध्रुव CV_OVERRIDE { प्रत्यागम current_Reserved_आकार; }
    आभास size_t get_Max_Reserved_आकार() ध्रुव CV_OVERRIDE { प्रत्यागम max_Reserved_आकार; }
    आभास शून्य समुच्चयः_Max_Reserved_आकार(size_t size) CV_OVERRIDE
    {
        Auto_तालकम् locker(mutex_);
        size_t old_Max_Reserved_आकार = max_Reserved_आकार;
        max_Reserved_आकार = size;
        यदि (max_Reserved_आकार < old_Max_Reserved_आकार)
        {
            प्रकारनाम std::list<बफर_प्रविष्टिः>::iterator i = reservedEntries_.begin();
            प्रति (; i != reservedEntries_.end();)
            {
                ध्रुव बफर_प्रविष्टिः& प्रविष्टिः = *i;
                यदि (प्रविष्टिः.क्षमता > max_Reserved_आकार / 8)
                {
                    CV_DbgAssert(current_Reserved_आकार >= प्रविष्टिः.क्षमता);
                    current_Reserved_आकार -= प्रविष्टिः.क्षमता;
                    derived().release_बफर_प्रविष्टिः(प्रविष्टिः);
                    i = reservedEntries_.erase(i);
                    अनुवर्त;
                }
                ++i;
            }
            check_आकार_सम्बन्धि_Reserved_Entries();
        }
    }
    आभास शून्य freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_तालकम् locker(mutex_);
        प्रकारनाम std::list<बफर_प्रविष्टिः>::const_iterator i = reservedEntries_.begin();
        प्रति (; i != reservedEntries_.end(); ++i)
        {
            ध्रुव बफर_प्रविष्टिः& प्रविष्टिः = *i;
            derived().release_बफर_प्रविष्टिः(प्रविष्टिः);
        }
        reservedEntries_.clear();
        current_Reserved_आकार = 0;
    }
};

संरचना CLBuffer_प्रविष्टिः
{
    cl_mem cl_बफर_2;
    size_t क्षमता;
    CLBuffer_प्रविष्टिः() : cl_बफर_2((cl_mem)रिक्त), क्षमता(0) { }
};

वर्ग OpenCLBufferPoolImpl CV_FINAL : सार्वजनिक Open_CLBuffer_Pool_आधारः_Impl<OpenCLBufferPoolImpl, CLBuffer_प्रविष्टिः, cl_mem>
{
सार्वजनिक:
    प्रकारनिर्देश संरचना CLBuffer_प्रविष्टिः बफर_प्रविष्टिः;
रक्षित:
    पूर्णाङ्क createFlags_;
सार्वजनिक:
    OpenCLBufferPoolImpl(पूर्णाङ्क createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    शून्य allocate_बफर_प्रविष्टिः(बफर_प्रविष्टिः& प्रविष्टिः, size_t size)
    {
        CV_DbgAssert(प्रविष्टिः.cl_बफर_2 == रिक्त);
        प्रविष्टिः.क्षमता = align_आकार(size, (पूर्णाङ्क)आवण्टनम्_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        प्रविष्टिः.cl_बफर_2 = cl_Create_बफर((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, प्रविष्टिः.क्षमता, 0, &retval);
        CV_OCL_CHECK_परिणाम(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (दीर्घ दीर्घ पूर्णाङ्क)प्रविष्टिः.क्षमता, (शून्य*)प्रविष्टिः.cl_बफर_2).c_str());
        CV_Assert(प्रविष्टिः.cl_बफर_2 != रिक्त);
        यदि(retval == CL_SUCCESS)
        {
            CV_IMPL_सङ्कलय(CV_IMPL_OCL);
        }
        LOG_बफर_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, प्रविष्टिः.cl_बफर_2);
        allocatedEntries_.push_back(प्रविष्टिः);
    }

    शून्य release_बफर_प्रविष्टिः(ध्रुव बफर_प्रविष्टिः& प्रविष्टिः)
    {
        CV_Assert(प्रविष्टिः.क्षमता != 0);
        CV_Assert(प्रविष्टिः.cl_बफर_2 != रिक्त);
        LOG_बफर_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                प्रविष्टिः.cl_बफर_2, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता);
        CV_OCL_DBG_CHECK(clReleaseMemObject(प्रविष्टिः.cl_बफर_2));
    }
};

#यदि_परिभाषितम् HAVE_OPENCL_SVM
संरचना CLSVMBuffer_प्रविष्टिः
{
    शून्य* cl_बफर_2;
    size_t क्षमता;
    CLSVMBuffer_प्रविष्टिः() : cl_बफर_2(रिक्त), क्षमता(0) { }
};
वर्ग OpenCLSVMBufferPoolImpl CV_FINAL : सार्वजनिक Open_CLBuffer_Pool_आधारः_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_प्रविष्टिः, शून्य*>
{
सार्वजनिक:
    प्रकारनिर्देश संरचना CLSVMBuffer_प्रविष्टिः बफर_प्रविष्टिः;
सार्वजनिक:
    OpenCLSVMBufferPoolImpl()
    {
    }

    शून्य allocate_बफर_प्रविष्टिः(बफर_प्रविष्टिः& प्रविष्टिः, size_t size)
    {
        CV_DbgAssert(प्रविष्टिः.cl_बफर_2 == रिक्त);
        प्रविष्टिः.क्षमता = align_आकार(size, (पूर्णाङ्क)आवण्टनम्_Granularity(size));

        Context& ctx = Context::getDefault();
        ध्रुव svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        तार्किक is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_बफर ? CL_MEM_SVM_FINE_GRAIN_बफर : 0);

        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (पूर्णाङ्क)प्रविष्टिः.क्षमता);
        शून्य *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, प्रविष्टिः.क्षमता, 0);
        CV_Assert(buf);

        प्रविष्टिः.cl_बफर_2 = buf;
        {
            CV_IMPL_सङ्कलय(CV_IMPL_OCL);
        }
        LOG_बफर_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, प्रविष्टिः.cl_बफर_2);
        allocatedEntries_.push_back(प्रविष्टिः);
    }

    शून्य release_बफर_प्रविष्टिः(ध्रुव बफर_प्रविष्टिः& प्रविष्टिः)
    {
        CV_Assert(प्रविष्टिः.क्षमता != 0);
        CV_Assert(प्रविष्टिः.cl_बफर_2 != रिक्त);
        LOG_बफर_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                प्रविष्टिः.cl_बफर_2, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता, (दीर्घ दीर्घ)प्रविष्टिः.क्षमता);
        Context& ctx = Context::getDefault();
        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  प्रविष्टिः.cl_बफर_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), प्रविष्टिः.cl_बफर_2);
    }
};
#यदि_अन्तः



प्रतिरूपम् <तार्किक readAccess, तार्किक writeAccess>
वर्ग Aligned_दत्त_Ptr
{
रक्षित:
    ध्रुव size_t आकार_3;
    uchar* ध्रुव originPtr_;
    ध्रुव size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

सार्वजनिक:
    Aligned_दत्त_Ptr(uchar* ptr, size_t size, size_t alignment)
        : आकार_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(रिक्त)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        यदि (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नूतन uchar[आकार_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            यदि (readAccess)
            {
                स्मृतिं_प्रतिलिख(ptr_, originPtr_, आकार_3);
            }
        }
    }

    uchar* getAlignedPtr() ध्रुव
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        प्रत्यागम ptr_;
    }

    ~Aligned_दत्त_Ptr()
    {
        यदि (allocatedPtr_)
        {
            यदि (writeAccess)
            {
                स्मृतिं_प्रतिलिख(originPtr_, ptr_, आकार_3);
            }
            विलोपय[] allocatedPtr_;
            allocatedPtr_ = रिक्त;
        }
        ptr_ = रिक्त;
    }
गुप्त:
    Aligned_दत्त_Ptr(ध्रुव Aligned_दत्त_Ptr&); // disabled
    Aligned_दत्त_Ptr& संक्रियकः=(ध्रुव Aligned_दत्त_Ptr&); // disabled
};

प्रतिरूपम् <तार्किक readAccess, तार्किक writeAccess>
वर्ग Aligned_दत्त_Ptr2_D
{
रक्षित:
    ध्रुव size_t आकार_3;
    uchar* ध्रुव originPtr_;
    ध्रुव size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t पङ्क्तिकाः_2;
    size_t स्तम्भसङ्ख्या_2;
    size_t चरणः_2;

सार्वजनिक:
    Aligned_दत्त_Ptr2_D(uchar* ptr, size_t पङ्क्तिकाः, size_t स्तम्भसङ्ख्या, size_t चरणः, size_t alignment, size_t extrabytes=0)
        : आकार_3(पङ्क्तिकाः*चरणः), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(रिक्त), पङ्क्तिकाः_2(पङ्क्तिकाः), स्तम्भसङ्ख्या_2(स्तम्भसङ्ख्या), चरणः_2(चरणः)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != रिक्त);
        यदि (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नूतन uchar[आकार_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            यदि (readAccess)
            {
                प्रति (size_t i = 0; i < पङ्क्तिकाः_2; i++)
                    स्मृतिं_प्रतिलिख(ptr_ + i*चरणः_2, originPtr_ + i*चरणः_2, स्तम्भसङ्ख्या_2);
            }
        }
    }

    uchar* getAlignedPtr() ध्रुव
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        प्रत्यागम ptr_;
    }

    ~Aligned_दत्त_Ptr2_D()
    {
        यदि (allocatedPtr_)
        {
            यदि (writeAccess)
            {
                प्रति (size_t i = 0; i < पङ्क्तिकाः_2; i++)
                    स्मृतिं_प्रतिलिख(originPtr_ + i*चरणः_2, ptr_ + i*चरणः_2, स्तम्भसङ्ख्या_2);
            }
            विलोपय[] allocatedPtr_;
            allocatedPtr_ = रिक्त;
        }
        ptr_ = रिक्त;
    }
गुप्त:
    Aligned_दत्त_Ptr2_D(ध्रुव Aligned_दत्त_Ptr2_D&); // disabled
    Aligned_दत्त_Ptr2_D& संक्रियकः=(ध्रुव Aligned_दत्त_Ptr2_D&); // disabled
};

#यदि_अपरिभाषितम् CV_OPENCL_दत्त_PTR_ALIGNMENT
#परिभाषय CV_OPENCL_दत्त_PTR_ALIGNMENT 16
#यदि_अन्तः


शून्य Context::Impl::__init_buffer_pools()
{
    बफर_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& बफर_Pool_3 = *बफर_Pool_2.get();
    बफर_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& बफर_Pool_Host_Ptr_2 = *बफर_Pool_Host_Ptr.get();

    size_t default_Pool_आकार = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_आकार = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_3.समुच्चयः_Max_Reserved_आकार(pool_आकार);
    size_t pool_आकार_Host_Ptr = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_Host_Ptr_2.समुच्चयः_Max_Reserved_आकार(pool_आकार_Host_Ptr);

#यदि_परिभाषितम् HAVE_OPENCL_SVM
    बफर_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& बफर_Pool_SVM_2 = *बफर_Pool_SVM.get();
    size_t pool_आकार_SVM = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_SVM_2.समुच्चयः_Max_Reserved_आकार(pool_आकार_SVM);
#यदि_अन्तः

    CV_LOG_INFO(रिक्त, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_आकार << " poolSizeHostPtr=" << pool_आकार_Host_Ptr);
}

वर्ग OpenCLAllocator CV_FINAL : सार्वजनिक MatAllocator
{
सार्वजनिक:
    गणना AllocatorFlags
    {
        ALLOCATOR_FLAGS_बफर_POOL_प्रयुक्तम् = 1 << 0,
        ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_प्रयुक्तम् = 1 << 1,
#यदि_परिभाषितम् HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_बफर_POOL_SVM_प्रयुक्तम् = 1 << 2,
#यदि_अन्तः
        ALLOCATOR_FLAGS_EXTERNAL_बफर = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flushCleanupQueue();
    }

    UMat_दत्त* defaultAllocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type, शून्य* दत्त, size_t* चरणः,
            AccessFlag flags, UMatUsageFlags usageFlags) ध्रुव
    {
        UMat_दत्त* u = matStdAllocator->allocate(dims, sizes, type, दत्त, चरणः, flags, usageFlags);
        प्रत्यागम u;
    }

    स्थिर तार्किक isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        स्थिर तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", असत्य);
        प्रत्यागम value;
    }
    स्थिर तार्किक isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        स्थिर तार्किक value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", असत्य);
        प्रत्यागम value;
    }

    शून्य getBestFlags(ध्रुव Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, पूर्णाङ्क& createFlags, UMat_दत्त::स्मृतिः_Flag& flags0) ध्रुव
    {
        ध्रुव Device& dev = ctx.device(0);
        createFlags = 0;
        यदि ((usageFlags & USAGE_ALLOCATE_HOST_स्मृतिः) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        यदि (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_स्मृतिः_2()
#यदि_अपरिभाषितम् __APPLE__
                || dev.isIntel()
#यदि_अन्तः
                )
            )
        )
            flags0 = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
        अन्यथा
            flags0 = UMat_दत्त::COPY_ON_MAP;
    }

    UMat_दत्त* allocate(पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* sizes, पूर्णाङ्क type,
                       शून्य* दत्त, size_t* चरणः, AccessFlag flags, UMatUsageFlags usageFlags) ध्रुव CV_OVERRIDE
    {
        यदि(!useOpenCL())
            प्रत्यागम defaultAllocate(dims, sizes, type, दत्त, चरणः, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(दत्त == 0);
        size_t योग = CV_ELEM_आकार(type);
        प्रति( पूर्णाङ्क i = dims-1; i >= 0; i-- )
        {
            यदि( चरणः )
                चरणः[i] = योग;
            योग *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        यदि (!ctx.getImpl())
            प्रत्यागम defaultAllocate(dims, sizes, type, दत्त, चरणः, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        पूर्णाङ्क createFlags = 0;
        UMat_दत्त::स्मृतिः_Flag flags0 = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        शून्य* handle = रिक्त;
        पूर्णाङ्क allocatorFlags = 0;

#यदि_परिभाषितम् HAVE_OPENCL_SVM
        ध्रुव svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        यदि (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_SVM_प्रयुक्तम्;
            handle = ctxImpl.get_बफर_Pool_SVM().allocate(योग);

            // this property is constant, so single buffer pool can be used here
            तार्किक is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();
            allocatorFlags |= is_Fine_Grain_बफर ? svm::OPENCL_SVM_FINE_GRAIN_बफर : svm::OPENCL_SVM_COARSE_GRAIN_बफर;
        }
        अन्यथा
#यदि_अन्तः
        यदि (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_प्रयुक्तम्;
            handle = ctxImpl.get_बफर_Pool().allocate(योग);
        }
        अन्यथा यदि (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_प्रयुक्तम्;
            handle = ctxImpl.get_बफर_Pool_Host_Ptr().allocate(योग);
        }
        अन्यथा
        {
            CV_Assert(handle != रिक्त); // Unsupported, throw
        }

        यदि (!handle)
            प्रत्यागम defaultAllocate(dims, sizes, type, दत्त, चरणः, flags, usageFlags);

        UMat_दत्त* u = नूतन UMat_दत्त(एतत्);
        u->दत्त = 0;
        u->size = योग;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<शून्य>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->अस्थायी_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(सत्य);
        opencl_allocator_stats.onAllocate(u->size);
        प्रत्यागम u;
    }

    तार्किक allocate(UMat_दत्त* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम असत्य;

        flushCleanupQueue();

        UMat_दत्त_Auto_तालकम् lock(u);

        यदि(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            पूर्णाङ्क createFlags = 0;
            UMat_दत्त::स्मृतिः_Flag flags0 = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            तार्किक copyOnMap = (flags0 & UMat_दत्त::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            पूर्णाङ्क allocatorFlags = 0;
            UMat_दत्त::स्मृतिः_Flag अस्थायी_UMat_Flags = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
            शून्य* handle = रिक्त;
            cl_int retval = CL_SUCCESS;

#यदि_परिभाषितम् HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            तार्किक useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            यदि (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                अस्थायी_UMat_Flags = UMat_दत्त::अस्थायी_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (पूर्णाङ्क)u->size, handle);
            }
            अन्यथा यदि (useSVM && (svmCaps.is_Support_Fine_Grain_बफर() || svmCaps.is_Support_Coarse_Grain_बफर()))
            {
                यदि (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    तार्किक is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_बफर ? CL_MEM_SVM_FINE_GRAIN_बफर : 0);

                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (पूर्णाङ्क)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = रिक्त;
                    यदि (!is_Fine_Grain_बफर)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (पूर्णाङ्क)u->size);
                        cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, रिक्त, रिक्त);
                        CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMap()");

                    }
                    स्मृतिं_प्रतिलिख(handle, u->origdata, u->size);
                    यदि (!is_Fine_Grain_बफर)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, handle, 0, रिक्त, रिक्त);
                        CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                    }

                    अस्थायी_UMat_Flags = UMat_दत्त::अस्थायी_UMAT | UMat_दत्त::अस्थायी_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_बफर ? svm::OPENCL_SVM_FINE_GRAIN_बफर
                                                : svm::OPENCL_SVM_COARSE_GRAIN_बफर;
                }
            }
            अन्यथा
#यदि_अन्तः
            {
                यदि( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                अस्थायी_UMat_Flags = UMat_दत्त::अस्थायी_UMAT;
                यदि (
                #यदि_परिभाषितम् __APPLE__
                    !copyOnMap &&
                #यदि_अन्तः
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (पूर्णाङ्क)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_दत्त && u->original_UMat_दत्त->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_बफर(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (दीर्घ दीर्घ पूर्णाङ्क)u->size, u->origdata, (शून्य*)handle).c_str());
                }
                यदि((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_बफर(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (दीर्घ दीर्घ पूर्णाङ्क)u->size, u->origdata, (शून्य*)handle).c_str());
                    अस्थायी_UMat_Flags |= UMat_दत्त::अस्थायी_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer() => %p", (शून्य*)handle).c_str());
            यदि(!handle || retval != CL_SUCCESS)
                प्रत्यागम असत्य;
            u->handle = handle;
            u->पूर्व_Allocator = u->currAllocator;
            u->currAllocator = एतत्;
            u->flags |= अस्थायी_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        यदि (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(सत्य);
        opencl_allocator_stats.onAllocate(u->size);
        प्रत्यागम सत्य;
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

    शून्य deallocate(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        यदि (!!(u->flags & UMat_दत्त::ASYNC_CLEANUP))
            सङ्कलय_गन्तव्यम्_Cleanup_Queue(u);
        अन्यथा
            deallocate_(u);
    }

    शून्य deallocate_(UMat_दत्त* u) ध्रुव
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        यदि ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_बफर) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#यदि_परिभाषितम् _WIN32
        यदि (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            प्रत्यागम;             // avoid any OpenCL calls
#यदि_अन्तः
        यदि(u->अस्थायी_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            यदि (u->hostCopyObsolete())
            {
#यदि_परिभाषितम् HAVE_OPENCL_SVM
                यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    यदि( u->अस्थायी_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर);
                        तार्किक is_Fine_Grain_बफर = (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर;
                        cl_command_queue q = रिक्त;
                        यदि (!is_Fine_Grain_बफर)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                            cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, रिक्त, रिक्त);
                            CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        स्मृतिं_प्रतिलिख(u->origdata, u->handle, u->size);
                        यदि (!is_Fine_Grain_बफर)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, u->handle, 0, रिक्त, रिक्त);
                            CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                        }
                    }
                    अन्यथा
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                अन्यथा
#यदि_अन्तः
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    यदि( u->अस्थायी_Copied_UMat() )
                    {
                        Aligned_दत्त_Ptr<असत्य, सत्य> alignedPtr(u->origdata, u->size, CV_OPENCL_दत्त_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    अन्यथा
                    {
                        cl_int retval = 0;
                        यदि (u->अस्थायी_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            शून्य* दत्त = cl_पङ्क्तौ_निक्षिप_Map_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (शून्य*)u->handle, (दीर्घ दीर्घ पूर्णाङ्क)u->size, दत्त).c_str());
                            CV_Assert(u->origdata == दत्त && "Details: https://github.com/opencv/opencv/issues/6293");
                            यदि (u->original_UMat_दत्त)
                            {
                                CV_Assert(u->original_UMat_दत्त->दत्त == दत्त);
                            }
                            retval = cl_पङ्क्तौ_निक्षिप_Unmap_Mem_Object(q, (cl_mem)u->handle, दत्त, 0, 0, 0);
                            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (शून्य*)u->handle, दत्त, (दीर्घ दीर्घ पूर्णाङ्क)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(असत्य);
            }
            अन्यथा
            {
                // nothing
            }
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
            {
                यदि( u->अस्थायी_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            अन्यथा
#यदि_अन्तः
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clReleaseMemObject(ptr=%p)", (शून्य*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(सत्य);
            u->currAllocator = u->पूर्व_Allocator;
            u->पूर्व_Allocator = रिक्त;
            यदि(u->दत्त && u->copyOnMap() && u->दत्त != u->origdata)
                fastFree(u->दत्त);
            u->दत्त = u->origdata;
            u->currAllocator->deallocate(u);
            u = रिक्त;
        }
        अन्यथा
        {
            CV_Assert(u->origdata == रिक्त);
            यदि(u->दत्त && u->copyOnMap() && u->दत्त != u->origdata)
            {
                fastFree(u->दत्त);
                u->दत्त = 0;
                u->markHostCopyObsolete(सत्य);
            }
            यदि (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_प्रयुक्तम्)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool().release((cl_mem)u->handle);
            }
            अन्यथा यदि (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_प्रयुक्तम्)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            अन्यथा यदि (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_SVM_प्रयुक्तम्)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                अन्यथा यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                {
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, u->handle, 0, रिक्त, रिक्त);
                        CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool_SVM().release((शून्य*)u->handle);
            }
#यदि_अन्तः
            अन्यथा
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(सत्य);
            विलोपय u;
            u = रिक्त;
        }
        CV_Assert(u == रिक्त);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    शून्य map(UMat_दत्त* u, AccessFlag accessFlags) ध्रुव CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        यदि (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(सत्य);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            यदि( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#यदि_परिभाषितम् HAVE_OPENCL_SVM
                यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
                {
                    यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                    {
                        Context& ctx = Context::getDefault();
                        ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                            cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, रिक्त, रिक्त);
                            CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_बफर_MAP;
                        }
                    }
                    clFinish(q);
                    u->दत्त = (uchar*)u->handle;
                    u->markHostCopyObsolete(असत्य);
                    u->markDeviceMemMapped(सत्य);
                    प्रत्यागम;
                }
#यदि_अन्तः

                cl_int retval = CL_SUCCESS;
                यदि (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->दत्त = (uchar*)cl_पङ्क्तौ_निक्षिप_Map_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (शून्य*)u->handle, (दीर्घ दीर्घ पूर्णाङ्क)u->size, u->दत्त).c_str());
                }
                यदि (u->दत्त && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(असत्य);
                    u->markDeviceMemMapped(सत्य);
                    प्रत्यागम;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_दत्त::COPY_ON_MAP;
            }

            यदि(!u->दत्त)
            {
                u->दत्त = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(सत्य);
            }
        }

        यदि (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_दत्त_Ptr<असत्य, सत्य> alignedPtr(u->दत्त, u->size, CV_OPENCL_दत्त_PTR_ALIGNMENT);
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == 0);
#यदि_अन्तः
            cl_int retval = cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (शून्य*)u->handle, (दीर्घ दीर्घ पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(असत्य);
        }
    }

    शून्य unmap(UMat_दत्त* u) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;


        CV_Assert(u->handle != 0);

        UMat_दत्त_Auto_तालकम् autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        यदि( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->दत्त != रिक्त);
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
            {
                यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                {
                    Context& ctx = Context::getDefault();
                    ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, u->handle,
                                0, रिक्त, रिक्त);
                        CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_बफर_MAP;
                    }
                }
                यदि (u->refcount == 0)
                    u->दत्त = 0;
                u->markDeviceCopyObsolete(असत्य);
                u->markHostCopyObsolete(सत्य);
                प्रत्यागम;
            }
#यदि_अन्तः
            यदि (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_पङ्क्तौ_निक्षिप_Unmap_Mem_Object(q, (cl_mem)u->handle, u->दत्त, 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (शून्य*)u->handle, u->दत्त, (दीर्घ दीर्घ पूर्णाङ्क)u->size).c_str());
                यदि (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(असत्य);
                u->दत्त = 0;
                u->markDeviceCopyObsolete(असत्य);
                u->markHostCopyObsolete(सत्य);
            }
        }
        अन्यथा यदि( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_दत्त_Ptr<सत्य, असत्य> alignedPtr(u->दत्त, u->size, CV_OPENCL_दत्त_PTR_ALIGNMENT);
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == 0);
#यदि_अन्तः
            retval = cl_पङ्क्तौ_निक्षिप_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (शून्य*)u->handle, (दीर्घ दीर्घ पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(असत्य);
            u->markHostCopyObsolete(सत्य);
        }
    }

    तार्किक checkContinuous(पूर्णाङ्क dims, ध्रुव size_t sz[],
                         ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                         ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                         size_t& योग, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ध्रुव
    {
        तार्किक iscontinuous = सत्य;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        योग = sz[dims-1];
        प्रति( पूर्णाङ्क i = dims-2; i >= 0; i-- )
        {
            यदि( i >= 0 && (योग != srcstep[i] || योग != dststep[i]) )
                iscontinuous = असत्य;
            योग *= sz[i];
            यदि( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            यदि( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        यदि( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            यदि( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                यदि( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                यदि( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            अन्यथा
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                यदि( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                यदि( dstofs )
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

    शून्य download(UMat_दत्त* u, शून्य* dstptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                  ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
                  ध्रुव size_t dststep[]) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;
        UMat_दत्त_Auto_तालकम् autolock(u);

        यदि( u->दत्त && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            प्रत्यागम;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        size_t योग = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
        {
            CV_DbgAssert(u->दत्त == रिक्त || u->दत्त == u->handle);
            Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0);
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMap()");
            }
            clFinish(q);
            यदि( iscontinuous )
            {
                स्मृतिं_प्रतिलिख(dstptr, (uchar*)u->handle + srcrawofs, योग);
            }
            अन्यथा
            {
                // This code is from MatAllocator::download()
                पूर्णाङ्क isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
                    यदि( sz[i] == 0 )
                    प्रत्यागम;
                    यदि( srcofs )
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
                    स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
            }
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, u->handle,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अन्यथा
#यदि_अन्तः
        {
            यदि( iscontinuous )
            {
                Aligned_दत्त_Ptr<असत्य, सत्य> alignedPtr((uchar*)dstptr, योग, CV_OPENCL_दत्त_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, योग, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            अन्यथा यदि (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_दत्त_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_दत्त_Ptr2_D<असत्य, असत्य> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_दत्त_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                योग = align_आकार(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                योग = std::min(योग, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, योग, ptr, 0, 0, 0));
                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    स्मृतिं_प्रतिलिख( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            अन्यथा
            {
                Aligned_दत्त_Ptr2_D<असत्य, सत्य> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_दत्त_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    शून्य upload(UMat_दत्त* u, ध्रुव शून्य* srcptr, पूर्णाङ्क dims, ध्रुव size_t sz[],
                ध्रुव size_t dstofs[], ध्रुव size_t dststep[],
                ध्रुव size_t srcstep[]) ध्रुव CV_OVERRIDE
    {
        यदि(!u)
            प्रत्यागम;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->अस्थायी_UMat());

        size_t योग = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_दत्त_Auto_तालकम् autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        यदि( u->दत्त && (u->hostCopyObsolete() < u->deviceCopyObsolete() || योग == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(असत्य);
            u->markDeviceCopyObsolete(सत्य);
            प्रत्यागम;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
        {
            CV_DbgAssert(u->दत्त == रिक्त || u->दत्त == u->handle);
            Context& ctx = Context::getDefault();
            ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0);
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णाङ्क)u->size);
                cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMap()");
            }
            clFinish(q);
            यदि( iscontinuous )
            {
                स्मृतिं_प्रतिलिख((uchar*)u->handle + dstrawofs, srcptr, योग);
            }
            अन्यथा
            {
                // This code is from MatAllocator::upload()
                पूर्णाङ्क isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
                    यदि( sz[i] == 0 )
                    प्रत्यागम;
                    यदि( dstofs )
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
                    स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
            }
            यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMUnmap(q, u->handle,
                        0, रिक्त, रिक्त);
                CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अन्यथा
#यदि_अन्तः
        {
            यदि( iscontinuous )
            {
                Aligned_दत्त_Ptr<सत्य, असत्य> alignedPtr((uchar*)srcptr, योग, CV_OPENCL_दत्त_PTR_ALIGNMENT);
                cl_int retval = cl_पङ्क्तौ_निक्षिप_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, योग, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (शून्य*)u->handle, (दीर्घ दीर्घ पूर्णाङ्क)dstrawofs, (दीर्घ दीर्घ पूर्णाङ्क)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            अन्यथा यदि (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_दत्त_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_दत्त_Ptr2_D<असत्य, असत्य> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_दत्त_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                योग = align_आकार(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                योग = std::min(योग, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, योग, ptr, 0, 0, 0));
                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    स्मृतिं_प्रतिलिख( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, योग, ptr, 0, 0, 0));
            }
            अन्यथा
            {
                Aligned_दत्त_Ptr2_D<सत्य, असत्य> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_दत्त_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Write_बफर_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(सत्य);
#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अन्यथा
#यदि_अन्तः
        {
            u->markHostCopyObsolete(सत्य);
        }
        u->markDeviceCopyObsolete(असत्य);
    }

    शून्य copy(UMat_दत्त* src, UMat_दत्त* dst, पूर्णाङ्क dims, ध्रुव size_t sz[],
              ध्रुव size_t srcofs[], ध्रुव size_t srcstep[],
              ध्रुव size_t dstofs[], ध्रुव size_t dststep[], तार्किक _sync) ध्रुव CV_OVERRIDE
    {
        यदि(!src || !dst)
            प्रत्यागम;

        size_t योग = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        तार्किक iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_दत्त_Auto_तालकम् src_autolock(src, dst);

        यदि( !src->handle || (src->दत्त && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->दत्त + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            प्रत्यागम;
        }
        यदि( !dst->handle || (dst->दत्त && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->दत्त + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(असत्य);
#यदि_परिभाषितम् HAVE_OPENCL_SVM
            यदि ((dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            अन्यथा
#यदि_अन्तः
            {
                dst->markDeviceCopyObsolete(सत्य);
            }
            प्रत्यागम;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
        {
            यदि ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
            {
                Context& ctx = Context::getDefault();
                ध्रुव svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                यदि( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (पूर्णाङ्क)योग);
                    cl_int स्थितिः = svmFns->fn_cl_पङ्क्तौ_निक्षिप_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            योग, 0, रिक्त, रिक्त);
                    CV_OCL_CHECK_परिणाम(स्थितिः, "clEnqueueSVMMemcpy()");
                }
                अन्यथा
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    पूर्णाङ्क isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)अधिकतम_पूर्णाङ्कः );
                        यदि( sz[i] == 0 )
                        प्रत्यागम;
                        यदि( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (पूर्णाङ्क)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    प्रति( पूर्णाङ्क i = 0; i < dims; i++ )
                    {
                        यदि( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ध्रुव Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    प्रति( j = 0; j < it.nplanes; j++, ++it )
                        स्मृतिं_प्रतिलिख(ptrs[1], ptrs[0], planesz);
                }
            }
            अन्यथा
            {
                यदि ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->दत्त + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                अन्यथा
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->दत्त + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        अन्यथा
#यदि_अन्तः
        {
            यदि( iscontinuous )
            {
                retval = cl_पङ्क्तौ_निक्षिप_Copy_बफर(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, योग, 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (शून्य*)src->handle, (शून्य*)dst->handle, (दीर्घ दीर्घ पूर्णाङ्क)srcrawofs, (दीर्घ दीर्घ पूर्णाङ्क)dstrawofs, (दीर्घ दीर्घ पूर्णाङ्क)योग).c_str());
            }
            अन्यथा यदि (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                ध्रुव size_t padding = CV_OPENCL_दत्त_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_दत्त_Ptr2_D<असत्य, असत्य> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_दत्त_PTR_ALIGNMENT, padding*2);
                Aligned_दत्त_Ptr2_D<असत्य, असत्य> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_दत्त_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_योग = align_आकार(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_योग = std::min(src_योग, src->size - new_srcrawofs);
                size_t dst_योग = align_आकार(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_योग = std::min(dst_योग, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_योग, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Read_बफर(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_योग, dstptr, 0, 0, 0));

                प्रति( size_t i = 0; i < new_sz[1]; i++ )
                    स्मृतिं_प्रतिलिख( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Write_बफर(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_योग, dstptr, 0, 0, 0));
            }
            अन्यथा
            {
                CV_OCL_CHECK(retval = cl_पङ्क्तौ_निक्षिप_Copy_बफर_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        यदि (retval == CL_SUCCESS)
        {
            CV_IMPL_सङ्कलय(CV_IMPL_OCL)
        }

#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_आवरणम्) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अन्यथा
#यदि_अन्तः
        {
            dst->markHostCopyObsolete(सत्य);
        }
        dst->markDeviceCopyObsolete(असत्य);

        यदि( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    बफर_Pool_Controller* get_बफर_Pool_Controller(ध्रुव वर्ण* id) ध्रुव CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        यदि (ctx.empty())
            प्रत्यागम रिक्त;
#यदि_परिभाषितम् HAVE_OPENCL_SVM
        यदि ((svm::checkForceSVMUmatUsage() && (id == रिक्त || वाक्यं_तुलय(id, "OCL") == 0)) || (id != रिक्त && वाक्यं_तुलय(id, "SVM") == 0))
        {
            प्रत्यागम &ctx.getImpl()->get_बफर_Pool_SVM();
        }
#यदि_अन्तः
        यदि (id != रिक्त && वाक्यं_तुलय(id, "HOST_ALLOC") == 0)
        {
            प्रत्यागम &ctx.getImpl()->get_बफर_Pool_Host_Ptr();
        }
        यदि (id != रिक्त && वाक्यं_तुलय(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        प्रत्यागम &ctx.getImpl()->get_बफर_Pool();
    }

    MatAllocator* matStdAllocator;

    परिवर्तनीय cv::Mutex cleanupQueueMutex;
    परिवर्तनीय std::deque<UMat_दत्त*> cleanupQueue;

    शून्य flushCleanupQueue() ध्रुव
    {
        // TSAN false positive: see #27638
        यदि (!cleanupQueue.empty())
        {
            std::deque<UMat_दत्त*> q;
            {
                cv::Auto_तालकम् lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            प्रति (std::deque<UMat_दत्त*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    शून्य सङ्कलय_गन्तव्यम्_Cleanup_Queue(UMat_दत्त* u) ध्रुव
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_तालकम् lock(cleanupQueueMutex);
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
शून्य समुच्चयः_आकार( UMat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* चरणाः_2,
              तार्किक auto_चरणाः = असत्य );
शून्य finalizeHdr(UMat& m);

} // namespace cv


नामाकाश cv { नामाकाश ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
शून्य convert_From_बफर(शून्य* cl_mem_बफर, size_t चरणः, पूर्णाङ्क पङ्क्तिकाः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क type, UMat& dst)
{
    पूर्णाङ्क d = 2;
    पूर्णाङ्क sizes[] = { पङ्क्तिकाः, स्तम्भसङ्ख्या };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_आवरणम्) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    समुच्चयः_आकार(dst, d, sizes, 0, सत्य);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_बफर;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, परिमाण(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_बफर == mem_type);

    size_t योग = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_आकार, परिमाण(size_t), &योग, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((पूर्णाङ्क)चरणः >= स्तम्भसङ्ख्या * CV_ELEM_आकार(type));
    CV_Assert(योग >= पङ्क्तिकाः * चरणः);

    // attach clBuffer to UMatData
    dst.u = नूतन UMat_दत्त(getOpenCLAllocator());
    dst.u->दत्त            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_बफर;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_दत्त::स्मृतिः_Flag>(0);
    dst.u->handle          = cl_mem_बफर;
    dst.u->origdata        = 0;
    dst.u->पूर्व_Allocator   = 0;
    dst.u->size            = योग;

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

    पूर्णाङ्क गभीरता = CV_8U;
    विकल्प (fmt.image_channel_दत्त_type)
    {
    पक्ष CL_UNORM_INT8:
    पक्ष CL_UNSIGNED_INT8:
        गभीरता = CV_8U;
        भङ्ग;

    पक्ष CL_SNORM_INT8:
    पक्ष CL_SIGNED_INT8:
        गभीरता = CV_8S;
        भङ्ग;

    पक्ष CL_UNORM_INT16:
    पक्ष CL_UNSIGNED_INT16:
        गभीरता = CV_16U;
        भङ्ग;

    पक्ष CL_SNORM_INT16:
    पक्ष CL_SIGNED_INT16:
        गभीरता = CV_16S;
        भङ्ग;

    पक्ष CL_SIGNED_INT32:
        गभीरता = CV_32S;
        भङ्ग;

    पक्ष CL_FLOAT:
        गभीरता = CV_32F;
        भङ्ग;

    पक्ष CL_HALF_FLOAT:
        गभीरता = CV_16F;
        भङ्ग;

    स्वभाव:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    पूर्णाङ्क type = CV_8UC1;
    विकल्प (fmt.image_channel_क्रमः)
    {
    पक्ष CL_R:
    पक्ष CL_A:
    पक्ष CL_INTENSITY:
    पक्ष CL_LUMINANCE:
        type = CV_MAKE_TYPE(गभीरता, 1);
        भङ्ग;

    पक्ष CL_RG:
    पक्ष CL_RA:
        type = CV_MAKE_TYPE(गभीरता, 2);
        भङ्ग;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    पक्ष CL_RGBA:
    पक्ष CL_BGRA:
    पक्ष CL_ARGB:
        type = CV_MAKE_TYPE(गभीरता, 4);
        भङ्ग;

    स्वभाव:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        भङ्ग;
    }

    size_t चरणः = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, परिमाण(size_t), &चरणः, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_विस्तारः, परिमाण(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, परिमाण(size_t), &h, 0));

    dst.create((पूर्णाङ्क)h, (पूर्णाङ्क)w, type);

    cl_mem cl_बफर = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Copy_Image_गन्तव्यम्_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, रिक्त, रिक्त));

    CV_OCL_CHECK(clFinish(q));

    प्रत्यागम;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

स्थिर शून्य getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint अङ्कः_Devices = 0;
    cl_int स्थितिः = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, रिक्त, &अङ्कः_Devices);
    यदि (स्थितिः != CL_DEVICE_NOT_प्राप्तम्) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_परिणाम(स्थितिः,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &अङ्कः_Devices).c_str());
    }

    यदि (अङ्कः_Devices == 0)
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

        संस्करणम्_3 = getStrProp(CL_PLATFORM_संस्करणम्);
        parseOpenCLVersion(संस्करणम्_3, संस्करणम्_Major, संस्करणम्_Minor);
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

    String संस्करणम्_3;
    पूर्णाङ्क संस्करणम्_Major;
    पूर्णाङ्क संस्करणम्_Minor;
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
    यदि(p)
        p->release();
}

PlatformInfo::PlatformInfo(ध्रुव PlatformInfo& i)
{
    यदि (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::संक्रियकः =(ध्रुव PlatformInfo& i)
{
    यदि (i.p != p)
    {
        यदि (i.p)
            i.p->addref();
        यदि (p)
            p->release();
        p = i.p;
    }
    प्रत्यागम *एतत्;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = शून्यसूचकः;
}

PlatformInfo& PlatformInfo::संक्रियकः = (PlatformInfo&& i) CV_NOEXCEPT
{
    यदि (एतत् != &i) {
        यदि(p)
            p->release();
        p = i.p;
        i.p = शून्यसूचकः;
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
    यदि(p)
        device.समुच्चयः(p->devices[d]);
}

String PlatformInfo::name() ध्रुव
{
    प्रत्यागम p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ध्रुव
{
    प्रत्यागम p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::संस्करणम्() ध्रुव
{
    प्रत्यागम p ? p->संस्करणम्_3 : String();
}

पूर्णाङ्क PlatformInfo::संस्करणम्_Major_2() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->संस्करणम्_Major;
}

पूर्णाङ्क PlatformInfo::संस्करणम्_Minor_2() ध्रुव
{
    CV_Assert(p);
    प्रत्यागम p->संस्करणम्_Minor;
}

स्थिर शून्य getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint अङ्कः_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms));

    यदि (अङ्कः_Platforms == 0)
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

ध्रुव वर्ण* type_गन्तव्यम्_Str(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_गभीरता_MAX*16]=
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
    पूर्णाङ्क cn = CV_MAT_CN(type), गभीरता = CV_MAT_गभीरता(type);
    ध्रुव वर्ण* परिणाम = cn > 16 ? शून्यसूचकः : tab[गभीरता*16 + cn-1];
    CV_Assert(परिणाम);
    प्रत्यागम परिणाम;
}

ध्रुव वर्ण* memop_Type_गन्तव्यम्_Str(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_गभीरता_MAX*16] =
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
    पूर्णाङ्क cn = CV_MAT_CN(type), गभीरता = CV_MAT_गभीरता(type);
    ध्रुव वर्ण* परिणाम = cn > 16 ? शून्यसूचकः : tab[गभीरता*16 + cn-1];
    CV_Assert(परिणाम);
    प्रत्यागम परिणाम;
}

ध्रुव वर्ण* vecop_Type_गन्तव्यम्_Str(पूर्णाङ्क type)
{
    स्थिर ध्रुव वर्ण* tab[CV_गभीरता_MAX*16] =
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
    पूर्णाङ्क cn = CV_MAT_CN(type), गभीरता = CV_MAT_गभीरता(type);
    ध्रुव वर्ण* परिणाम = cn > 16 ? 0 : tab[गभीरता*16 + cn-1];
    CV_Assert(परिणाम);
    प्रत्यागम परिणाम;
}

// Deprecated due to size of buf buffer being unknowable.
ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    प्रत्यागम convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ध्रुव वर्ण* convertTypeStr(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth, पूर्णाङ्क cn, वर्ण* buf, size_t buf_आकार_2)
{
    यदि( sdepth == ddepth )
        प्रत्यागम "noconvert";
    ध्रुव वर्ण *typestr = type_गन्तव्यम्_Str(CV_MAKETYPE(ddepth, cn));
    यदि( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_आकार_2, "convert_%s", typestr);
    }
    अन्यथा यदि( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_आकार_2, "convert_%s_sat_rte", typestr);
    अन्यथा
        snprintf(buf, buf_आकार_2, "convert_%s_sat", typestr);

    प्रत्यागम buf;
}

ध्रुव वर्ण* getOpenCLErrorString(पूर्णाङ्क error_संहिता)
{
#परिभाषय CV_OCL_संहिता(id) पक्ष id: प्रत्यागम #id
#परिभाषय CV_OCL_संहिता_2(id, name) पक्ष id: प्रत्यागम #name
    विकल्प (error_संहिता)
    {
    CV_OCL_संहिता(CL_SUCCESS);
    CV_OCL_संहिता(CL_DEVICE_NOT_प्राप्तम्);
    CV_OCL_संहिता(CL_DEVICE_NOT_उपलब्धम्);
    CV_OCL_संहिता(CL_COMPILER_NOT_उपलब्धम्);
    CV_OCL_संहिता(CL_MEM_OBJECT_आवण्टनम्_FAILURE);
    CV_OCL_संहिता(CL_बहिर्गमनम्_सम्बन्धि_RESOURCES);
    CV_OCL_संहिता(CL_बहिर्गमनम्_सम्बन्धि_HOST_स्मृतिः);
    CV_OCL_संहिता(CL_PROFILING_INFO_NOT_उपलब्धम्);
    CV_OCL_संहिता(CL_MEM_COPY_OVERLAP);
    CV_OCL_संहिता(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_संहिता(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_संहिता(CL_BUILD_क्रमादेशः_FAILURE);
    CV_OCL_संहिता(CL_MAP_FAILURE);
    CV_OCL_संहिता(CL_MISALIGNED_SUB_बफर_विस्थापनम्);
    CV_OCL_संहिता(CL_EXEC_स्थितिः_ERROR_FOR_EVENTS_अन्तर्_WAIT_LIST);
    CV_OCL_संहिता(CL_COMPILE_क्रमादेशः_FAILURE);
    CV_OCL_संहिता(CL_LINKER_NOT_उपलब्धम्);
    CV_OCL_संहिता(CL_LINK_क्रमादेशः_FAILURE);
    CV_OCL_संहिता(CL_DEVICE_PARTITION_FAILED);
    CV_OCL_संहिता(CL_KERNEL_ARG_INFO_NOT_उपलब्धम्);
    CV_OCL_संहिता(CL_INVALID_मानम्);
    CV_OCL_संहिता(CL_INVALID_DEVICE_TYPE);
    CV_OCL_संहिता(CL_INVALID_PLATFORM);
    CV_OCL_संहिता(CL_INVALID_DEVICE);
    CV_OCL_संहिता(CL_INVALID_CONTEXT);
    CV_OCL_संहिता(CL_INVALID_QUEUE_PROPERTIES);
    CV_OCL_संहिता(CL_INVALID_COMMAND_QUEUE);
    CV_OCL_संहिता(CL_INVALID_HOST_PTR);
    CV_OCL_संहिता(CL_INVALID_MEM_OBJECT);
    CV_OCL_संहिता(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_संहिता(CL_INVALID_IMAGE_आकार);
    CV_OCL_संहिता(CL_INVALID_SAMPLER);
    CV_OCL_संहिता(CL_INVALID_द्विमान);
    CV_OCL_संहिता(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_संहिता(CL_INVALID_क्रमादेशः);
    CV_OCL_संहिता(CL_INVALID_क्रमादेशः_EXECUTABLE);
    CV_OCL_संहिता(CL_INVALID_KERNEL_NAME);
    CV_OCL_संहिता(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_संहिता(CL_INVALID_KERNEL);
    CV_OCL_संहिता(CL_INVALID_ARG_सूचकांक);
    CV_OCL_संहिता(CL_INVALID_ARG_मानम्);
    CV_OCL_संहिता(CL_INVALID_ARG_आकार);
    CV_OCL_संहिता(CL_INVALID_KERNEL_तर्काः);
    CV_OCL_संहिता(CL_INVALID_WORK_DIMENSION);
    CV_OCL_संहिता(CL_INVALID_WORK_गणः_आकार);
    CV_OCL_संहिता(CL_INVALID_WORK_ITEM_आकार);
    CV_OCL_संहिता(CL_INVALID_GLOBAL_विस्थापनम्);
    CV_OCL_संहिता(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_संहिता(CL_INVALID_EVENT);
    CV_OCL_संहिता(CL_INVALID_OPERATION);
    CV_OCL_संहिता(CL_INVALID_GL_OBJECT);
    CV_OCL_संहिता(CL_INVALID_बफर_आकार);
    CV_OCL_संहिता(CL_INVALID_MIP_LEVEL);
    CV_OCL_संहिता(CL_INVALID_GLOBAL_WORK_आकार);
    // OpenCL 1.1
    CV_OCL_संहिता(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_संहिता(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_संहिता(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_संहिता(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_संहिता(CL_INVALID_DEVICE_PARTITION_गणनम्);
    // OpenCL 2.0
    CV_OCL_संहिता_2(-69, CL_INVALID_PIPE_आकार);
    CV_OCL_संहिता_2(-70, CL_INVALID_DEVICE_QUEUE);
    // Extensions
    CV_OCL_संहिता_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_संहिता_2(-1001, CL_PLATFORM_NOT_प्राप्तम्_KHR);
    CV_OCL_संहिता_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_संहिता_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_संहिता_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_संहिता_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    स्वभाव: प्रत्यागम "Unknown OpenCL error";
    }
#अपरिभाषय CV_OCL_संहिता
#अपरिभाषय CV_OCL_संहिता_2
}

प्रतिरूपम् <प्रकारनाम T>
स्थिर std::string ker_गन्तव्यम्_Str(ध्रुव Mat & k)
{
    पूर्णाङ्क विस्तारः = k.स्तम्भसङ्ख्या - 1, गभीरता = k.गभीरता();
    ध्रुव T * ध्रुव दत्त = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    यदि (गभीरता <= CV_8S)
    {
        प्रति (पूर्णाङ्क i = 0; i < विस्तारः; ++i)
            stream << "DIG(" << (पूर्णाङ्क)दत्त[i] << ")";
        stream << "DIG(" << (पूर्णाङ्क)दत्त[विस्तारः] << ")";
    }
    अन्यथा यदि (गभीरता == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        प्रति (पूर्णाङ्क i = 0; i < विस्तारः; ++i)
            stream << "DIG(" << दत्त[i] << "f)";
        stream << "DIG(" << दत्त[विस्तारः] << "f)";
    }
    अन्यथा यदि (गभीरता == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        प्रति (पूर्णाङ्क i = 0; i < विस्तारः; ++i)
            stream << "DIG(" << (भिन्न)दत्त[i] << "h)";
        stream << "DIG(" << (भिन्न)दत्त[विस्तारः] << "h)";
    }
    अन्यथा
    {
        प्रति (पूर्णाङ्क i = 0; i < विस्तारः; ++i)
            stream << "DIG(" << दत्त[i] << ")";
        stream << "DIG(" << दत्त[विस्तारः] << ")";
    }

    प्रत्यागम stream.str();
}

String kernel_गन्तव्यम्_Str(निवेशः_Array _kernel, पूर्णाङ्क ddepth, ध्रुव वर्ण * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    पूर्णाङ्क गभीरता = kernel.गभीरता();
    यदि (ddepth < 0)
        ddepth = गभीरता;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    यदि (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    यदि (ddepth != गभीरता)
        kernel.convert_गन्तव्यम्(kernel, ddepth);

    प्रकारनिर्देश std::string (* func_t)(ध्रुव Mat &);
    स्थिर ध्रुव func_t funcs[CV_गभीरता_MAX] = { ker_गन्तव्यम्_Str<uchar>, ker_गन्तव्यम्_Str<वर्ण>, ker_गन्तव्यम्_Str<ushort>, ker_गन्तव्यम्_Str<ह्रस्व>,
                                                ker_गन्तव्यम्_Str<पूर्णाङ्क>, ker_गन्तव्यम्_Str<भिन्न>, ker_गन्तव्यम्_Str<द्विगुण>, ker_गन्तव्यम्_Str<hfloat>,
                                                0, ker_गन्तव्यम्_Str<तार्किक>, ker_गन्तव्यम्_Str<uint64_t>, ker_गन्तव्यम्_Str<int64_t>, ker_गन्तव्यम्_Str<अचिह्नित> };
    ध्रुव func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    प्रत्यागम cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#परिभाषय PROCESS_SRC(src) \
    कुरु \
    { \
        यदि (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            आकार csize = src.size(); \
            पूर्णाङ्क ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_गभीरता(ctype), \
                ckercn = सदिशम्_Widths[cdepth], cwidth = ccn * csize.विस्तारः; \
            यदि (cwidth < ckercn || ckercn <= 0) \
                प्रत्यागम 1; \
            स्तम्भसङ्ख्या.push_back(cwidth); \
            यदि (strat == OCL_सदिशम्_OWN && ctype != ref_type) \
                प्रत्यागम 1; \
            offsets.push_back(src.offset()); \
            चरणाः.push_back(src.चरणः()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    यावत् ((शून्य)0, 0)

पूर्णाङ्क predict_Optimal_सदिशम्_विस्तारः(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
                              निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
                              निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9,
                              Ocl_सदिशम्_Strategy strat)
{
    ध्रुव ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    पूर्णाङ्क सदिशम्_Widths[CV_गभीरता_MAX] = {
        d.preferred_सदिशम्_विस्तारः_Char(), d.preferred_सदिशम्_विस्तारः_Char(),   // CV_8U,  CV_8S
        d.preferred_सदिशम्_विस्तारः_Short(), d.preferred_सदिशम्_विस्तारः_Short(), // CV_16U, CV_16S
        d.preferred_सदिशम्_विस्तारः_Int(), d.preferred_सदिशम्_विस्तारः_Float(),   // CV_32S, CV_32F
        d.preferred_सदिशम्_विस्तारः_Double(), d.preferred_सदिशम्_विस्तारः_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_सदिशम्_विस्तारः_Char(),                                 // CV_Bool
        d.preferred_सदिशम्_विस्तारः_Long(), d.preferred_सदिशम्_विस्तारः_Long(),   // CV_64U, CV_64S
        d.preferred_सदिशम्_विस्तारः_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    यदि (सदिशम्_Widths[0] == 1)
    {
        // it's heuristic
        सदिशम्_Widths[CV_8U] = सदिशम्_Widths[CV_8S] = 4;
        सदिशम्_Widths[CV_16U] = सदिशम्_Widths[CV_16S] = सदिशम्_Widths[CV_16F] = 2;
        सदिशम्_Widths[CV_32S] = सदिशम्_Widths[CV_32F] = सदिशम्_Widths[CV_64F] = 1;
    }

    प्रत्यागम check_Optimal_सदिशम्_विस्तारः(सदिशम्_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

पूर्णाङ्क check_Optimal_सदिशम्_विस्तारः([[maybe_unused]] ध्रुव पूर्णाङ्क *सदिशम्_Widths,
                            [[maybe_unused]] निवेशः_Array src1,
                            [[maybe_unused]] निवेशः_Array src2,
                            [[maybe_unused]] निवेशः_Array src3,
                            [[maybe_unused]] निवेशः_Array src4,
                            [[maybe_unused]] निवेशः_Array src5,
                            [[maybe_unused]] निवेशः_Array src6,
                            [[maybe_unused]] निवेशः_Array src7,
                            [[maybe_unused]] निवेशः_Array src8,
                            [[maybe_unused]] निवेशः_Array src9,
                            [[maybe_unused]] Ocl_सदिशम्_Strategy strat)
{
#यदि_परिभाषितम् __APPLE__
    प्रत्यागम 1;
#अन्यथा
    CV_Assert(सदिशम्_Widths);

    पूर्णाङ्क ref_type = src1.type();

    std::vector<size_t> offsets, चरणाः, स्तम्भसङ्ख्या;
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
        यावत् (offsets[i] % dividers[i] != 0 || चरणाः[i] % dividers[i] != 0 || स्तम्भसङ्ख्या[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    पूर्णाङ्क kercn = *std::min_element(kercns.begin(), kercns.end());

    प्रत्यागम kercn;
#यदि_अन्तः
}

पूर्णाङ्क predict_Optimal_सदिशम्_विस्तारः_Max(निवेशः_Array src1, निवेशः_Array src2, निवेशः_Array src3,
                                 निवेशः_Array src4, निवेशः_Array src5, निवेशः_Array src6,
                                 निवेशः_Array src7, निवेशः_Array src8, निवेशः_Array src9)
{
    प्रत्यागम predict_Optimal_सदिशम्_विस्तारः(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_सदिशम्_MAX);
}

#अपरिभाषय PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
शून्य build_Options_सङ्कलय_Matrix_Description(String& buildOptions, ध्रुव String& name, निवेशः_Array _m)
{
    यदि (!buildOptions.empty())
        buildOptions += " ";
    पूर्णाङ्क type = _m.type(), गभीरता = CV_MAT_गभीरता(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_गन्तव्यम्_Str(type),
            name.c_str(), ocl::type_गन्तव्यम्_Str(CV_MAKE_TYPE(गभीरता, 1)),
            name.c_str(), (पूर्णाङ्क)CV_MAT_CN(type),
            name.c_str(), (पूर्णाङ्क)CV_ELEM_आकार(type),
            name.c_str(), (पूर्णाङ्क)CV_ELEM_SIZE1(type),
            name.c_str(), (पूर्णाङ्क)गभीरता
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
        यदि (handle)
            clReleaseMemObject(handle);
    }

    स्थिर cl_image_format getImageFormat(पूर्णाङ्क गभीरता, पूर्णाङ्क cn, तार्किक norm)
    {
        cl_image_format format;
        स्थिर ध्रुव पूर्णाङ्क channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        स्थिर ध्रुव पूर्णाङ्क channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        स्थिर ध्रुव पूर्णाङ्क channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        पूर्णाङ्क channelType = norm ? channelTypesNorm[गभीरता] : channelTypes[गभीरता];
        पूर्णाङ्क channel_क्रमः = channelOrders[cn];
        format.image_channel_दत्त_type = (cl_channel_type)channelType;
        format.image_channel_क्रमः = (cl_channel_क्रमः)channel_क्रमः;
        प्रत्यागम format;
    }

    स्थिर तार्किक isFormatSupported(cl_image_format format)
    {
        यदि (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        यदि (!context)
            प्रत्यागम असत्य;

        // Figure out how many formats are supported by this context.
        cl_uint अङ्कः_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, अङ्कः_Formats,
                                                रिक्त, &अङ्कः_Formats);
        CV_OCL_DBG_CHECK_परिणाम(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        यदि (अङ्कः_Formats > 0)
        {
            Auto_बफर<cl_image_format> formats(अङ्कः_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, अङ्कः_Formats,
                                             formats.दत्त(), रिक्त);
            CV_OCL_DBG_CHECK_परिणाम(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            प्रति (cl_uint i = 0; i < अङ्कः_Formats; ++i)
            {
                यदि (!memcmp(&formats[i], &format, परिमाण(format)))
                {
                    प्रत्यागम सत्य;
                }
            }
        }
        प्रत्यागम असत्य;
    }

    शून्य init(ध्रुव UMat &src, तार्किक norm, तार्किक alias)
    {
        यदि (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        पूर्णाङ्क err, गभीरता = src.गभीरता(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(गभीरता, cn, norm);

        यदि (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        यदि (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#यदि_परिभाषितम् CL_संस्करणम्_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ध्रुव Device & d = ocl::Device::getDefault();
        पूर्णाङ्क minor = d.device_संस्करणम्_Minor(), major = d.device_संस्करणम्_Major();
        CV_Assert(!alias || canCreateAlias(src));
        यदि (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_विस्तारः      = src.स्तम्भसङ्ख्या;
            desc.image_height     = src.पङ्क्तिकाः;
            desc.image_गभीरता      = 0;
            desc.image_array_आकार = 1;
            desc.image_row_pitch  = alias ? src.चरणः[0] : 0;
            desc.image_खण्डिका_pitch = 0;
            desc.बफर           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.अङ्कः_mip_स्तराः   = 0;
            desc.अङ्कः_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, रिक्त, &err);
        }
        अन्यथा
#यदि_अन्तः
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.स्तम्भसङ्ख्या, src.पङ्क्तिकाः, 0, रिक्त, &err);
            CV_SUPPRESS_DEPRECATED_अन्तः
        }
        CV_OCL_DBG_CHECK_परिणाम(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.स्तम्भसङ्ख्या), static_cast<size_t>(src.पङ्क्तिकाः), 1 };

        cl_mem dev_दत्त;
        यदि (!alias && !src.isContinuous())
        {
            dev_दत्त = cl_Create_बफर(context, CL_MEM_READ_ONLY, src.स्तम्भसङ्ख्या * src.पङ्क्तिकाः * src.elem_आकार(), रिक्त, &err);
            CV_OCL_CHECK_परिणाम(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (दीर्घ दीर्घ पूर्णाङ्क)(src.स्तम्भसङ्ख्या * src.पङ्क्तिकाः * src.elem_आकार()), (शून्य*)dev_दत्त
                ).c_str());

            ध्रुव size_t roi[3] = {static_cast<size_t>(src.स्तम्भसङ्ख्या) * src.elem_आकार(), static_cast<size_t>(src.पङ्क्तिकाः), 1};
            CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Copy_बफर_Rect(queue, (cl_mem)src.handle(ACCESS_READ), dev_दत्त, origin, origin,
                roi, src.चरणः, 0, src.स्तम्भसङ्ख्या * src.elem_आकार(), 0, 0, रिक्त, रिक्त));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        अन्यथा
        {
            dev_दत्त = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_दत्त != रिक्त);

        यदि (!alias)
        {
            CV_OCL_CHECK(cl_पङ्क्तौ_निक्षिप_Copy_बफर_गन्तव्यम्_Image(queue, dev_दत्त, handle, 0, origin, region, 0, रिक्त, 0));
            यदि (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(queue));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_दत्त));
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
    तार्किक ret = असत्य;
    ध्रुव Device & d = ocl::Device::getDefault();
    यदि (d.image_From_बफर_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        यदि (pitchAlign && !(m.चरणः % (pitchAlign * m.elem_आकार())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            यदि (!m.u->अस्थायी_UMat())
            {
                ret = सत्य;
            }
        }
    }
    प्रत्यागम ret;
}

तार्किक Image2D::isFormatSupported(पूर्णाङ्क गभीरता, पूर्णाङ्क cn, तार्किक norm)
{
    cl_image_format format = Impl::getImageFormat(गभीरता, cn, norm);

    प्रत्यागम Impl::isFormatSupported(format);
}

Image2D::Image2D(ध्रुव Image2D & i)
{
    p = i.p;
    यदि (p)
        p->addref();
}

Image2D & Image2D::संक्रियकः = (ध्रुव Image2D & i)
{
    यदि (i.p != p)
    {
        यदि (i.p)
            i.p->addref();
        यदि (p)
            p->release();
        p = i.p;
    }
    प्रत्यागम *एतत्;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = शून्यसूचकः;
}

Image2D& Image2D::संक्रियकः = (Image2D&& i) CV_NOEXCEPT
{
    यदि (एतत् != &i) {
        यदि (p)
            p->release();
        p = i.p;
        i.p = शून्यसूचकः;
    }
    प्रत्यागम *एतत्;
}

Image2D::~Image2D()
{
    यदि (p)
        p->release();
}

शून्य* Image2D::ptr() ध्रुव
{
    प्रत्यागम p ? p->handle : 0;
}

तार्किक internal::isOpenCLForced()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक value = असत्य;
    यदि (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम value;
}

तार्किक internal::isPerformanceCheckBypassed()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर तार्किक value = असत्य;
    यदि (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", असत्य);
        initialized = सत्य;
    }
    प्रत्यागम value;
}

तार्किक internal::isCLBuffer(UMat& u)
{
    यदि (isCudaBacked(u.u))
        प्रत्यागम असत्य;
    शून्य* h = u.handle(ACCESS_RW);
    यदि (!h)
        प्रत्यागम सत्य;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#यदि 1
    यदि ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        प्रत्यागम असत्य;
#अन्यथा
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, परिमाण(type), &type, रिक्त);
    यदि (ret != CL_SUCCESS || type != CL_MEM_OBJECT_बफर)
        प्रत्यागम असत्य;
#यदि_अन्तः
    प्रत्यागम सत्य;
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
Timer::~Timer() { विलोपय p; }

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

#यदि_अन्तः // HAVE_OPENCL
