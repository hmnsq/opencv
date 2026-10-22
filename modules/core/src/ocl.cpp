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

#అన్తర్భావ "precomp.hpp"

#నిర్వచితం_కాకపోతే HAVE_OPENCL
#అన్తర్భావ "ocl_disabled.impl.hpp"
#లేకపోతే // HAVE_OPENCL

#అన్తర్భావ <list>
#అన్తర్భావ <map>
#అన్తర్భావ <deque>
#అన్తర్భావ <set>
#అన్తర్భావ <string>
#అన్తర్భావ <sstream>
#అన్తర్భావ <fstream>
#ఒకవేళ !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#అన్తర్భావ <inttypes.h>
#అయితే_ముగింపు

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచనం_రద్దు CV_LOG_STRIP_LEVEL
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#అన్తర్భావ "opencv2/core/ocl_genbase.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#అన్తర్భావ "opencv2/core/utils/lock.private.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.hpp"
#అన్తర్భావ "opencv2/core/utils/filesystem.private.hpp"

#నిర్వచించు CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ఖాళీ, 0, "OpenCL allocator: " << __VA_ARGS__)
#అన్తర్భావ "opencv2/core/utils/allocator_stats.impl.hpp"
#నిర్వచనం_రద్దు CV__ALLOCATOR_STATS_LOG

#నిర్వచించు CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#నిర్వచించు CV_OPENCL_SHOW_BUILD_OPTIONS             0
#నిర్వచించు CV_OPENCL_SHOW_BUILD_KERNELS             0

#నిర్వచించు CV_OPENCL_SHOW_నిరంతరం_KERNELS               0
#నిర్వచించు CV_OPENCL_SYNC_నిరంతరం_KERNELS               0
#నిర్వచించు CV_OPENCL_TRACE_CHECK                    0

#నిర్వచించు CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS       1

#నిర్వచించు CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#నిర్వచించు CV_OPENCL_SHOW_SVM_LOG                   0

#అన్తర్భావ "opencv2/core/bufferpool.hpp"
#నిర్వచితం_కాకపోతే LOG_బఫరు_POOL
# ఒకవేళ 0
#   define LOG_బఫరు_POOL రాయి
# లేకపోతే
#   define LOG_బఫరు_POOL(...)
# endif
#అయితే_ముగింపు

#ఒకవేళ CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#నిర్వచించు CV_OPENCL_SVM_TRACE_P రాయి("line %d (ocl.cpp): ", __LINE__); రాయి
#లేకపోతే
#నిర్వచించు CV_OPENCL_SVM_TRACE_P(...)
#అయితే_ముగింపు

#ఒకవేళ CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#నిర్వచించు CV_OPENCL_SVM_TRACE_ERROR_P రాయి("Error on line %d (ocl.cpp): ", __LINE__); రాయి
#లేకపోతే
#నిర్వచించు CV_OPENCL_SVM_TRACE_ERROR_P(...)
#అయితే_ముగింపు

#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_core.hpp"

#నిర్వచితమైతే HAVE_OPENCL_SVM
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#అన్తర్భావ "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#అన్తర్భావ "opencv2/core/opencl/opencl_svm.hpp"
#అయితే_ముగింపు

#అన్తర్భావ "umatrix.hpp"

నామాకాశ cv { నామాకాశ ocl {

#నిర్వచించు IMPLEMENT_REFCOUNTABLE() \
    శూన్య addref() { CV_XADD(&refcount, 1); } \
    శూన్య release() { ఒకవేళ( CV_XADD(&refcount, -1) == 1 && !cv::__termination) తొలగించు ఇది; } \
    పూర్ణాంకం refcount

స్థిర cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    తిరిగి opencl_allocator_stats;
}

#నిర్వచితం_కాకపోతే _DEBUG
స్థిర తార్కిక isRaiseError()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక value = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి value;
}
#అయితే_ముగింపు

స్థిర శూన్య onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    తార్కిక value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", అబద్ధం);
    ఒకవేళ (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#ఒకవేళ CV_OPENCL_TRACE_CHECK
స్థిర అంతఃవరుస
శూన్య traceOpenCLCheck(cl_int స్థితి_2, స్థిరరాశి అక్షరం* message)
{
    std::బయటకు << "OpenCV(OpenCL:" << స్థితి_2 << "): " << message << std::పంక్తి_ముగింపు << std::flush;
}
#నిర్వచించు CV_OCL_TRACE_CHECK_ఫలితం(స్థితి_2, message) traceOpenCLCheck(స్థితి_2, message)
#లేకపోతే
#నిర్వచించు CV_OCL_TRACE_CHECK_ఫలితం(స్థితి_2, message) /* nothing */
#అయితే_ముగింపు

#నిర్వచించు CV_OCL_API_ERROR_సందేశం(check_ఫలితం, సందేశం) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_ఫలితం), check_ఫలితం, సందేశం)

#నిర్వచించు CV_OCL_CHECK_ఫలితం(check_ఫలితం, సందేశం) \
    చేయి { \
        CV_OCL_TRACE_CHECK_ఫలితం(check_ఫలితం, సందేశం); \
        ఒకవేళ (check_ఫలితం != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(సందేశం), స్థిరరాశి అక్షరం*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_సందేశం_2 = CV_OCL_API_ERROR_సందేశం(check_ఫలితం, సందేశం); \
            CV_Error(Error::OpenCLApiCallError, error_సందేశం_2); \
        } \
    } అయితే (0)

#నిర్వచించు CV_OCL_CHECK_(సమాసం, check_ఫలితం) చేయి { సమాసం; CV_OCL_CHECK_ఫలితం(check_ఫలితం, #expr); } అయితే (0)

#నిర్వచించు CV_OCL_CHECK(సమాసం) చేయి { cl_int __cl_result = (సమాసం); CV_OCL_CHECK_ఫలితం(__cl_result, #expr); } అయితే (0)

#నిర్వచితమైతే _DEBUG
#నిర్వచించు CV_OCL_DBG_CHECK_ఫలితం(check_ఫలితం, సందేశం) CV_OCL_CHECK_ఫలితం(check_ఫలితం, సందేశం)
#నిర్వచించు CV_OCL_DBG_CHECK(సమాసం) CV_OCL_CHECK(సమాసం)
#నిర్వచించు CV_OCL_DBG_CHECK_(సమాసం, check_ఫలితం) CV_OCL_CHECK_(సమాసం, check_ఫలితం)
#లేకపోతే
#నిర్వచించు CV_OCL_DBG_CHECK_ఫలితం(check_ఫలితం, సందేశం) \
    చేయి { \
        CV_OCL_TRACE_CHECK_ఫలితం(check_ఫలితం, సందేశం); \
        ఒకవేళ (check_ఫలితం != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(సందేశం), స్థిరరాశి అక్షరం*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_సందేశం_2 = CV_OCL_API_ERROR_సందేశం(check_ఫలితం, సందేశం); \
            CV_Error(Error::OpenCLApiCallError, error_సందేశం_2); \
        } \
    } అయితే (0)
#నిర్వచించు CV_OCL_DBG_CHECK_(సమాసం, check_ఫలితం) చేయి { సమాసం; CV_OCL_DBG_CHECK_ఫలితం(check_ఫలితం, #expr); } అయితే (0)
#నిర్వచించు CV_OCL_DBG_CHECK(సమాసం) చేయి { cl_int __cl_result = (సమాసం); CV_OCL_DBG_CHECK_ఫలితం(__cl_result, #expr); } అయితే (0)
#అయితే_ముగింపు


స్థిర స్థిరరాశి తార్కిక CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", నిజం);
స్థిర స్థిరరాశి తార్కిక CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", నిజం);
స్థిర స్థిరరాశి తార్కిక CV_OPENCL_CACHE_తాళం_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", నిజం);
స్థిర స్థిరరాశి తార్కిక CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", నిజం);

#ఒకవేళ CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS
స్థిర స్థిరరాశి తార్కిక CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS_విలువ = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", అబద్ధం);
#అయితే_ముగింపు

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
స్థిర స్థిరరాశి తార్కిక CV_OPENCL_DISABLE_బఫరు_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#ఒకవేళ 1 //def __APPLE__
        నిజం
#లేకపోతే
        అబద్ధం
#అయితే_ముగింపు
);

స్థిర String getBuildExtraOptions()
{
    స్థిర String param_buildExtraOptions;
    స్థిర తార్కిక initialized = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = నిజం;
        ఒకవేళ (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(ఖాళీ, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    తిరిగి param_buildExtraOptions;
}

స్థిర స్థిరరాశి తార్కిక CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", నిజం);
స్థిర స్థిరరాశి size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


నిర్మాణం UMat2D
{
    UMat2D(స్థిరరాశి UMat& m)
    {
        offset = (పూర్ణాంకం)m.offset;
        అడుగు = (పూర్ణాంకం)m.అడుగు;
        అడ్డువరుసలు = m.అడ్డువరుసలు;
        నిలువుల_సంఖ్య = m.నిలువుల_సంఖ్య;
    }
    పూర్ణాంకం offset;
    పూర్ణాంకం అడుగు;
    పూర్ణాంకం అడ్డువరుసలు;
    పూర్ణాంకం నిలువుల_సంఖ్య;
};

నిర్మాణం UMat3D
{
    UMat3D(స్థిరరాశి UMat& m)
    {
        offset = (పూర్ణాంకం)m.offset;
        అడుగు = (పూర్ణాంకం)m.అడుగు.p[1];
        slicestep = (పూర్ణాంకం)m.అడుగు.p[0];
        slices = (పూర్ణాంకం)m.size.p[0];
        అడ్డువరుసలు = m.size.p[1];
        నిలువుల_సంఖ్య = m.size.p[2];
    }
    పూర్ణాంకం offset;
    పూర్ణాంకం slicestep;
    పూర్ణాంకం అడుగు;
    పూర్ణాంకం slices;
    పూర్ణాంకం అడ్డువరుసలు;
    పూర్ణాంకం నిలువుల_సంఖ్య;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
స్థిర uint64 crc64( స్థిరరాశి uchar* దత్తాంశం, size_t size, uint64 crc0=0 )
{
    స్థిర uint64 table[256];
    స్థిర తార్కిక initialized = అబద్ధం;

    ఒకవేళ( !initialized )
    {
        కొరకు( పూర్ణాంకం i = 0; i < 256; i++ )
        {
            uint64 c = i;
            కొరకు( పూర్ణాంకం j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = నిజం;
    }

    uint64 చక్రీయ_తనిఖీ = ~crc0;
    కొరకు( size_t idx = 0; idx < size; idx++ )
        చక్రీయ_తనిఖీ = table[(uchar)చక్రీయ_తనిఖీ ^ దత్తాంశం[idx]] ^ (చక్రీయ_తనిఖీ >> 8);

    తిరిగి ~చక్రీయ_తనిఖీ;
}

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
నిర్మాణం OpenCLBinaryCacheConfigurator
{
    cv::String cache_మార్గం;
    cv::String cache_తాళం_filename;
    cv::Ptr<utils::fs::File_తాళం> cache_తాళం;

    రకనిర్వచనం std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(ఖాళీ, "Initializing OpenCL cache configuration...");
        ఒకవేళ (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL cache is disabled");
            తిరిగి;
        }
        cache_మార్గం = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        ఒకవేళ (cache_మార్గం.empty())
        {
            CV_LOG_INFO(ఖాళీ, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        చేయి
        {
            ప్రయత్నించు
            {
                ఒకవేళ (cache_మార్గం.empty())
                    ఆపు;
                ఒకవేళ (cache_మార్గం == "disabled")
                    ఆపు;
                ఒకవేళ (!utils::fs::createDirectories(cache_మార్గం))
                {
                    CV_LOG_DEBUG(ఖాళీ, "Can't use OpenCL cache directory: " << cache_మార్గం);
                    clear();
                    ఆపు;
                }

                ఒకవేళ (CV_OPENCL_CACHE_తాళం_ENABLE)
                {
                    cache_తాళం_filename = cache_మార్గం + ".lock";
                    ఒకవేళ (!utils::fs::exists(cache_తాళం_filename))
                    {
                        CV_LOG_DEBUG(ఖాళీ, "Creating lock file... (" << cache_తాళం_filename << ")");
                        std::ofstream తాళం_filename(cache_తాళం_filename.c_str(), std::ios::out);
                        ఒకవేళ (!తాళం_filename.is_open())
                        {
                            CV_LOG_WARNING(ఖాళీ, "Can't create lock file for OpenCL program cache: " << cache_తాళం_filename);
                            ఆపు;
                        }
                    }

                    ప్రయత్నించు
                    {
                        cache_తాళం = makePtr<utils::fs::File_తాళం>(cache_తాళం_filename.c_str());
                        CV_LOG_VERBOSE(ఖాళీ, 0, "Checking cache lock... (" << cache_తాళం_filename << ")");
                        {
                            utils::shared_తాళం_guard<utils::fs::File_తాళం> lock(*cache_తాళం);
                        }
                        CV_LOG_VERBOSE(ఖాళీ, 0, "Checking cache lock... Done!");
                    }
                    పట్టుకో (స్థిరరాశి cv::Exception& e)
                    {
                        CV_LOG_WARNING(ఖాళీ, "Can't create OpenCL program cache lock: " << cache_తాళం_filename << std::పంక్తి_ముగింపు << e.what());
                    }
                    పట్టుకో (...)
                    {
                        CV_LOG_WARNING(ఖాళీ, "Can't create OpenCL program cache lock: " << cache_తాళం_filename);
                    }
                }
                లేకపోతే
                {
                    ఒకవేళ (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(ఖాళీ, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    లేకపోతే
                    {
                        CV_LOG_INFO(ఖాళీ, "OpenCL cache lock is disabled");
                    }
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                CV_LOG_WARNING(ఖాళీ, "Can't prepare OpenCL program cache: " << cache_మార్గం << std::పంక్తి_ముగింపు << e.what());
                clear();
            }
        } అయితే (0);
        ఒకవేళ (!cache_మార్గం.empty())
        {
            ఒకవేళ (cache_తాళం.empty() && CV_OPENCL_CACHE_తాళం_ENABLE)
            {
                CV_LOG_WARNING(ఖాళీ, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            లేకపోతే
            {
                CV_LOG_INFO(ఖాళీ, "Successfully initialized OpenCL cache directory: " << cache_మార్గం);
            }
        }
    }

    శూన్య clear()
    {
        cache_మార్గం.clear();
        cache_తాళం_filename.clear();
        cache_తాళం.release();
    }

    std::string prepareCacheDirectoryForContext(స్థిరరాశి std::string& ctx_ఉపసర్గ,
            స్థిరరాశి std::string& cleanup_ఉపసర్గ)
    {
        ఒకవేళ (cache_మార్గం.empty())
            తిరిగి std::string();

        Auto_తాళం lock(mutex_prepared_contexts_);

        ContextCacheType::iterator దొరికింది_it = prepared_contexts_.find(ctx_ఉపసర్గ);
        ఒకవేళ (దొరికింది_it != prepared_contexts_.end())
            తిరిగి దొరికింది_it->second;

        CV_LOG_INFO(ఖాళీ, "Preparing OpenCL cache configuration for context: " << ctx_ఉపసర్గ);

        std::string లక్ష్యం_directory = cache_మార్గం + ctx_ఉపసర్గ + "/";
        తార్కిక ఫలితం = utils::fs::isDirectory(లక్ష్యం_directory);
        ఒకవేళ (!ఫలితం)
        {
            ప్రయత్నించు
            {
                CV_LOG_VERBOSE(ఖాళీ, 0, "Creating directory: " << లక్ష్యం_directory);
                ఒకవేళ (utils::fs::createDirectories(లక్ష్యం_directory))
                {
                    ఫలితం = నిజం;
                }
                లేకపోతే
                {
                    CV_LOG_WARNING(ఖాళీ, "Can't create directory: " << లక్ష్యం_directory);
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                CV_LOG_ERROR(ఖాళీ, "Can't create OpenCL program cache directory for context: " << లక్ష్యం_directory << std::పంక్తి_ముగింపు << e.what());
            }
        }
        లక్ష్యం_directory = ఫలితం ? లక్ష్యం_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_ఉపసర్గ, లక్ష్యం_directory));

        ఒకవేళ (ఫలితం && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_ఉపసర్గ.empty())
        {
            ప్రయత్నించు
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_మార్గం, cleanup_ఉపసర్గ + "*", entries, అబద్ధం, నిజం);
                std::vector<String> remove_entries;
                కొరకు (size_t i = 0; i < entries.size(); i++)
                {
                    స్థిరరాశి String& name = entries[i];
                    ఒకవేళ (0 == name.find(cleanup_ఉపసర్గ))
                    {
                        ఒకవేళ (0 == name.find(ctx_ఉపసర్గ))
                            కొనసాగు; // skip current
                        remove_entries.push_back(name);
                    }
                }
                ఒకవేళ (!remove_entries.empty())
                {
                    CV_LOG_WARNING(ఖాళీ, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(ఖాళీ, "Trying to remove these directories...");
                    కొరకు (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(ఖాళీ, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(ఖాళీ, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    కొరకు (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        స్థిరరాశి String& name = remove_entries[i];
                        cv::String మార్గం = utils::fs::join(cache_మార్గం, name);
                        ప్రయత్నించు
                        {
                            utils::fs::remove_all(మార్గం);
                            CV_LOG_WARNING(ఖాళీ, "Removed: " << మార్గం);
                        }
                        పట్టుకో (స్థిరరాశి cv::Exception& e)
                        {
                            CV_LOG_ERROR(ఖాళీ, "Exception during removal of obsolete OpenCL cache directory: " << మార్గం << std::పంక్తి_ముగింపు << e.what());
                        }
                    }
                }
            }
            పట్టుకో (...)
            {
                CV_LOG_WARNING(ఖాళీ, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(ఖాళీ, 1, "  Result: " << (లక్ష్యం_directory.empty() ? std::string("Failed") : లక్ష్యం_directory));
        తిరిగి లక్ష్యం_directory;
    }

    స్థిర OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, కొత్త OpenCLBinaryCacheConfigurator());
    }
};
తరగతి ద్విమాన_కార్యక్రమం_File
{
    గణన { MAX_ENTRIES = 64 };

    రకనిర్వచనం గుర్తులేని పూర్ణాంకం uint32_t;

    నిర్మాణం CV_DECL_ALIGNED(4) File_శీర్షిక
    {
        uint32_t ఆధారం_Signature_కొలత;
        //char sourceSignature[];
    };

    నిర్మాణం CV_DECL_ALIGNED(4) File_పట్టిక
    {
        uint32_t సంఖ్య_యొక్క_Entries;
        //uint32_t firstEntryOffset[];
    };

    నిర్మాణం CV_DECL_ALIGNED(4) File_నమోదు
    {
        uint32_t తదుపరి_నమోదు_File_స్థానభ్రంశం; // 0 for the last entry in chain
        uint32_t కీలకం_కొలత;
        uint32_t దత్తాంశం_కొలత;
        //char key[];
        //char data[];
    };

    స్థిరరాశి std::string fileName_;
    స్థిరరాశి అక్షరం* స్థిరరాశి ఆధారం_Signature;
    స్థిరరాశి size_t ఆధారం_Signature_కొలత_2;

    std::fstream f;

    uint32_t నమోదు_Offsets[MAX_ENTRIES];

    uint32_t getHash(స్థిరరాశి std::string& options)
    {
        uint64 hash = crc64((స్థిరరాశి uchar*)options.c_str(), options.size(), 0);
        తిరిగి hash & (MAX_ENTRIES - 1);
    }

    అంతఃవరుస size_t get_File_కొలత()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_కొలత = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        తిరిగి file_కొలత;
    }
    అంతఃవరుస uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.పఠ((అక్షరం*)&res, పరిమాణం(uint32_t));
        CV_Assert(!f.fail());
        తిరిగి res;
    }
    అంతఃవరుస శూన్య writeUInt32(స్థిరరాశి uint32_t value)
    {
        uint32_t v = value;
        f.లిఖ((అక్షరం*)&v, పరిమాణం(uint32_t));
        CV_Assert(!f.fail());
    }

    అంతఃవరుస శూన్య seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    అంతఃవరుస శూన్య seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    అంతఃవరుస శూన్య seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    శూన్య clearFile()
    {
        f.close();
        ఒకవేళ (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(ఖాళీ, "Can't remove: " << fileName_);
        తిరిగి;
    }

బహిరంగ:
    ద్విమాన_కార్యక్రమం_File(స్థిరరాశి std::string& fileName, స్థిరరాశి అక్షరం* ఆధారం_Signature_2)
        : fileName_(fileName), ఆధారం_Signature(ఆధారం_Signature_2), ఆధారం_Signature_కొలత_2(ఆధారం_Signature ? అక్షరశ్రేణి_పొడవు(ఆధారం_Signature) : 0)
    {
        CV_StaticAssert(పరిమాణం(uint32_t) == 4, "");
        CV_Assert(ఆధారం_Signature != ఖాళీ);
        CV_Assert(ఆధారం_Signature_కొలత_2 > 0);
        memset(నమోదు_Offsets, 0, పరిమాణం(నమోదు_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        ఒకవేళ(f.is_open() && get_File_కొలత() > 0)
        {
            తార్కిక isValid = అబద్ధం;
            ప్రయత్నించు
            {
                uint32_t file_ఆధారం_Signature_కొలత = readUInt32();
                ఒకవేళ (file_ఆధారం_Signature_కొలత == ఆధారం_Signature_కొలత_2)
                {
                    cv::Auto_బఫరు<అక్షరం> file_ఆధారం_Signature(file_ఆధారం_Signature_కొలత + 1);
                    f.పఠ(file_ఆధారం_Signature.దత్తాంశం(), file_ఆధారం_Signature_కొలత);
                    ఒకవేళ (f.eof())
                    {
                        CV_LOG_ERROR(ఖాళీ, "Unexpected EOF");
                    }
                    లేకపోతే ఒకవేళ (memcmp(ఆధారం_Signature_2, file_ఆధారం_Signature.దత్తాంశం(), file_ఆధారం_Signature_కొలత) == 0)
                    {
                        isValid = నిజం;
                    }
                }
                ఒకవేళ (!isValid)
                {
                    CV_LOG_ERROR(ఖాళీ, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                CV_LOG_ERROR(ఖాళీ, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            పట్టుకో (...)
            {
                CV_LOG_ERROR(ఖాళీ, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            ఒకవేళ (!isValid)
            {
                clearFile();
            }
            లేకపోతే
            {
                seekReadAbsolute(0);
            }
        }
    }

    తార్కిక పఠ(స్థిరరాశి std::string& కీలకం, std::vector<అక్షరం>& buf)
    {
        ఒకవేళ (!f.is_open())
            తిరిగి అబద్ధం;

        size_t file_కొలత = get_File_కొలత();
        ఒకవేళ (file_కొలత == 0)
        {
            CV_LOG_ERROR(ఖాళీ, "Invalid file (empty): " << fileName_);
            clearFile();
            తిరిగి అబద్ధం;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ఆధారం_Signature_కొలత = readUInt32();
        CV_Assert(file_ఆధారం_Signature_కొలత > 0);
        seekReadRelative(file_ఆధారం_Signature_కొలత);

        uint32_t సంఖ్య_యొక్క_Entries = readUInt32();
        CV_Assert(సంఖ్య_యొక్క_Entries > 0);
        ఒకవేళ (సంఖ్య_యొక్క_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ఖాళీ, "Invalid file: " << fileName_);
            clearFile();
            తిరిగి అబద్ధం;
        }
        f.పఠ((అక్షరం*)&నమోదు_Offsets[0], పరిమాణం(నమోదు_Offsets));
        CV_Assert(!f.fail());

        uint32_t నమోదు_అంకె = getHash(కీలకం);

        uint32_t నమోదు_స్థానభ్రంశం = నమోదు_Offsets[నమోదు_అంకె];
        File_నమోదు నమోదు_2;
        అయితే (నమోదు_స్థానభ్రంశం > 0)
        {
            seekReadAbsolute(నమోదు_స్థానభ్రంశం);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.పఠ((అక్షరం*)&నమోదు_2, పరిమాణం(నమోదు_2));
            CV_Assert(!f.fail());
            cv::Auto_బఫరు<అక్షరం> file_కీలకం(నమోదు_2.కీలకం_కొలత + 1);
            ఒకవేళ (కీలకం.size() == నమోదు_2.కీలకం_కొలత)
            {
                ఒకవేళ (నమోదు_2.కీలకం_కొలత > 0)
                {
                    f.పఠ(file_కీలకం.దత్తాంశం(), నమోదు_2.కీలకం_కొలత);
                    CV_Assert(!f.fail());
                }
                ఒకవేళ (memcmp(file_కీలకం.దత్తాంశం(), కీలకం.c_str(), నమోదు_2.కీలకం_కొలత) == 0)
                {
                    buf.resize(నమోదు_2.దత్తాంశం_కొలత);
                    f.పఠ(&buf[0], నమోదు_2.దత్తాంశం_కొలత);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(ఖాళీ, 0, "Read...");
                    తిరిగి నిజం;
                }
            }
            ఒకవేళ (నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం == 0)
                ఆపు;
            నమోదు_స్థానభ్రంశం = నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం;
        }
        తిరిగి అబద్ధం;
    }

    తార్కిక లిఖ(స్థిరరాశి std::string& కీలకం, std::vector<అక్షరం>& buf)
    {
        ఒకవేళ (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            ఒకవేళ (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                ఒకవేళ (!f.is_open())
                {
                    CV_LOG_ERROR(ఖాళీ, "Can't create file: " << fileName_);
                    తిరిగి అబద్ధం;
                }
            }
        }

        size_t file_కొలత = get_File_కొలత();
        ఒకవేళ (file_కొలత == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)ఆధారం_Signature_కొలత_2);
            f.లిఖ(ఆధారం_Signature, ఆధారం_Signature_కొలత_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(నమోదు_Offsets, 0, పరిమాణం(నమోదు_Offsets));
            f.లిఖ((అక్షరం*)నమోదు_Offsets, పరిమాణం(నమోదు_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_కొలత = get_File_కొలత();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ఆధారం_Signature_కొలత = readUInt32();
        CV_Assert(file_ఆధారం_Signature_కొలత == ఆధారం_Signature_కొలత_2);
        seekReadRelative(file_ఆధారం_Signature_కొలత);

        uint32_t సంఖ్య_యొక్క_Entries = readUInt32();
        CV_Assert(సంఖ్య_యొక్క_Entries > 0);
        ఒకవేళ (సంఖ్య_యొక్క_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(ఖాళీ, "Invalid file: " << fileName_);
            clearFile();
            తిరిగి అబద్ధం;
        }
        size_t పట్టిక_Entries_స్థానభ్రంశం = (size_t)f.tellg();
        f.పఠ((అక్షరం*)&నమోదు_Offsets[0], పరిమాణం(నమోదు_Offsets));
        CV_Assert(!f.fail());

        uint32_t నమోదు_అంకె = getHash(కీలకం);

        uint32_t నమోదు_స్థానభ్రంశం = నమోదు_Offsets[నమోదు_అంకె];
        File_నమోదు నమోదు_2;
        అయితే (నమోదు_స్థానభ్రంశం > 0)
        {
            seekReadAbsolute(నమోదు_స్థానభ్రంశం);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.పఠ((అక్షరం*)&నమోదు_2, పరిమాణం(నమోదు_2));
            CV_Assert(!f.fail());
            cv::Auto_బఫరు<అక్షరం> file_కీలకం(నమోదు_2.కీలకం_కొలత + 1);
            ఒకవేళ (కీలకం.size() == నమోదు_2.కీలకం_కొలత)
            {
                ఒకవేళ (నమోదు_2.కీలకం_కొలత > 0)
                {
                    f.పఠ(file_కీలకం.దత్తాంశం(), నమోదు_2.కీలకం_కొలత);
                    CV_Assert(!f.fail());
                }
                ఒకవేళ (0 == memcmp(file_కీలకం.దత్తాంశం(), కీలకం.c_str(), నమోదు_2.కీలకం_కొలత))
                {
                    // duplicate
                    CV_LOG_VERBOSE(ఖాళీ, 0, "Duplicate key ignored: " << fileName_);
                    తిరిగి అబద్ధం;
                }
            }
            ఒకవేళ (నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం == 0)
                ఆపు;
            నమోదు_స్థానభ్రంశం = నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం;
        }
        seekReadAbsolute(0);
        ఒకవేళ (నమోదు_స్థానభ్రంశం > 0)
        {
            seekWriteAbsolute(నమోదు_స్థానభ్రంశం);
            నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం = (uint32_t)file_కొలత;
            f.లిఖ((అక్షరం*)&నమోదు_2, పరిమాణం(నమోదు_2));
            CV_Assert(!f.fail());
        }
        లేకపోతే
        {
            నమోదు_Offsets[నమోదు_అంకె] = (uint32_t)file_కొలత;
            seekWriteAbsolute(పట్టిక_Entries_స్థానభ్రంశం);
            f.లిఖ((అక్షరం*)నమోదు_Offsets, పరిమాణం(నమోదు_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_కొలత);
        నమోదు_2.తదుపరి_నమోదు_File_స్థానభ్రంశం = 0;
        నమోదు_2.దత్తాంశం_కొలత = (uint32_t)buf.size();
        నమోదు_2.కీలకం_కొలత = (uint32_t)కీలకం.size();
        f.లిఖ((అక్షరం*)&నమోదు_2, పరిమాణం(నమోదు_2));
        CV_Assert(!f.fail());
        f.లిఖ(కీలకం.c_str(), నమోదు_2.కీలకం_కొలత);
        CV_Assert(!f.fail());
        f.లిఖ(&buf[0], నమోదు_2.దత్తాంశం_కొలత);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(ఖాళీ, 0, "Write... (" << buf.size() << " bytes)");
        తిరిగి నిజం;
    }
};
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT



నిర్మాణం OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    పూర్ణాంకం device_;  // device index in context
    ocl::వరుస వరుస_3;
    పూర్ణాంకం useOpenCL_;

రక్షిత:
    Impl() = తొలగించు;

    శూన్య _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        పూర్ణాంకం ndevices = (పూర్ణాంకం)context_.ndevices();
        CV_Assert(ndevices > 0);
        తార్కిక దొరికింది = అబద్ధం;
        కొరకు (పూర్ణాంకం i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            ఒకవేళ (dhandle == deviceID)
            {
                device_ = i;
                దొరికింది = నిజం;
                ఆపు;
            }
        }
        CV_Assert(దొరికింది && "OpenCL device can't work with passed OpenCL context");
    }

    శూన్య _init_device(స్థిరరాశి ocl::Device& device)
    {
        CV_Assert(device.ptr());
        పూర్ణాంకం ndevices = (పూర్ణాంకం)context_.ndevices();
        CV_Assert(ndevices > 0);
        తార్కిక దొరికింది = అబద్ధం;
        కొరకు (పూర్ణాంకం i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            ఒకవేళ (d.getImpl() == device.getImpl())
            {
                device_ = i;
                దొరికింది = నిజం;
                ఆపు;
            }
        }
        CV_Assert(దొరికింది && "OpenCL device can't work with passed OpenCL context");
    }

బహిరంగ:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_వాడని(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        వరుస_3 = వరుస(context_, context_.device(device_));
    }

    Impl(స్థిరరాశి ocl::Context& context, స్థిరరాశి ocl::Device& device, స్థిరరాశి ocl::వరుస& వరుస_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        వరుస_3 = వరుస_2;
    }

    Impl(స్థిరరాశి ocl::Context& context, స్థిరరాశి ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        వరుస_3 = వరుస(context_, context_.device(device_));
    }

    Impl(స్థిరరాశి ocl::Context& context, స్థిరరాశి పూర్ణాంకం device, స్థిరరాశి ocl::వరుస& వరుస_2)
        : context_(context)
        , device_(device)
        , వరుస_3(వరుస_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(స్థిరరాశి Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , వరుస_3(other.వరుస_3)
        , useOpenCL_(-1)
    {
        // nothing
    }

    అంతఃవరుస తార్కిక useOpenCL() స్థిరరాశి { తిరిగి const_cast<Impl*>(ఇది)->useOpenCL(); }
    తార్కిక useOpenCL()
    {
        ఒకవేళ (useOpenCL_ < 0)
        {
            ప్రయత్నించు
            {
                useOpenCL_ = 0;
                ఒకవేళ (!context_.empty() && context_.ndevices() > 0)
                {
                    స్థిరరాశి Device& d = context_.device(device_);
                    useOpenCL_ = d.అందుబాటు();
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception&)
            {
                // nothing
            }
            ఒకవేళ (!useOpenCL_)
                CV_LOG_INFO(ఖాళీ, "OpenCL: can't use OpenCL execution context");
        }
        తిరిగి useOpenCL_ > 0;
    }

    శూన్య సమితి_Use_Open_CL(తార్కిక flag)
    {
        ఒకవేళ (!flag)
            useOpenCL_ = 0;
        లేకపోతే
            useOpenCL_ = -1;
    }

    స్థిర స్థిరరాశి std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(ఖాళీ, "OpenCL: initializing thread execution context");

        స్థిర తార్కిక initialized = అబద్ధం;
        స్థిర std::shared_ptr<Impl> g_primaryExecutionContext;

        ఒకవేళ (!initialized)
        {
            cv::Auto_తాళం lock(getInitializationMutex());
            ఒకవేళ (!initialized)
            {
                CV_LOG_INFO(ఖాళీ, "OpenCL: creating new execution context...");
                ప్రయత్నించు
                {
                    Context c = ocl::Context::create(std::string());
                    ఒకవేళ (c.ndevices())
                    {
                        పూర్ణాంకం deviceId = 0;
                        స్వయంచాలక& d = c.device(deviceId);
                        ఒకవేళ (d.అందుబాటు())
                        {
                            స్వయంచాలక q = ocl::వరుస(c, d);
                            ఒకవేళ (!q.ptr())
                            {
                                CV_LOG_ERROR(ఖాళీ, "OpenCL: Can't create default OpenCL queue");
                            }
                            లేకపోతే
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(ఖాళీ, "OpenCL: device=" << d.name());
                            }
                        }
                        లేకపోతే
                        {
                            CV_LOG_ERROR(ఖాళీ, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    లేకపోతే
                    {
                        CV_LOG_INFO(ఖాళీ, "OpenCL: context is not available/disabled");
                    }
                }
                పట్టుకో (స్థిరరాశి std::exception& e)
                {
                    CV_LOG_INFO(ఖాళీ, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                పట్టుకో (...)
                {
                    CV_LOG_WARNING(ఖాళీ, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = నిజం;
            }
        }
        తిరిగి g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->context_;
}
Device& OpenCLExecutionContext::getDevice() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->context_.device(p->device_);
}
వరుస& OpenCLExecutionContext::get_వరుస() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->వరుస_3;
}

తార్కిక OpenCLExecutionContext::useOpenCL() స్థిరరాశి
{
    ఒకవేళ (p)
        తిరిగి p->useOpenCL();
    తిరిగి అబద్ధం;
}
శూన్య OpenCLExecutionContext::సమితి_Use_Open_CL(తార్కిక flag)
{
    CV_Assert(p);
    p->సమితి_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    OpenCLExecutionContext& c = దత్తాంశం.oclExecutionContext;
    ఒకవేళ (!దత్తాంశం.oclExecutionContextInitialized)
    {
        దత్తాంశం.oclExecutionContextInitialized = నిజం;
        ఒకవేళ (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    తిరిగి c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    OpenCLExecutionContext& c = దత్తాంశం.oclExecutionContext;
    తిరిగి c;
}

శూన్య OpenCLExecutionContext::bind() స్థిరరాశి
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    దత్తాంశం.oclExecutionContext = *ఇది;
    దత్తాంశం.oclExecutionContextInitialized = నిజం;
    దత్తాంశం.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_వరుస() స్థిరరాశి
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    స్థిరరాశి వరుస q(getContext(), getDevice());
    తిరిగి clone_With_New_వరుస(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_వరుస(స్థిరరాశి ocl::వరుస& q) స్థిరరాశి
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != ఖాళీ);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    తిరిగి c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(స్థిరరాశి Context& context, స్థిరరాశి Device& device, స్థిరరాశి ocl::వరుస& వరుస_2)
{
    CV_TRACE_FUNCTION();
    ఒకవేళ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, వరుస_2);
    తిరిగి ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(స్థిరరాశి Context& context, స్థిరరాశి Device& device)
{
    CV_TRACE_FUNCTION();
    ఒకవేళ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    తిరిగి ctx;

}

శూన్య OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
స్థిర తార్కిక g_isOpenCLInitialized = అబద్ధం;
స్థిర తార్కిక g_isOpenCLAvailable = అబద్ధం;

తార్కిక haveOpenCL()
{
    CV_TRACE_FUNCTION();

    ఒకవేళ (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string env_మార్గం = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        ఒకవేళ (!env_మార్గం.empty())
        {
            ఒకవేళ (env_మార్గం == "disabled")
            {
                g_isOpenCLAvailable = అబద్ధం;
                g_isOpenCLInitialized = నిజం;
                తిరిగి అబద్ధం;
            }
        }

        cv::Auto_తాళం lock(getInitializationMutex());
        CV_LOG_INFO(ఖాళీ, "Initialize OpenCL runtime...");
        ప్రయత్నించు
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, ఖాళీ, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(ఖాళీ, "OpenCL: found " << n << " platforms");
        }
        పట్టుకో (...)
        {
            g_isOpenCLAvailable = అబద్ధం;
        }
        g_isOpenCLInitialized = నిజం;
    }
    తిరిగి g_isOpenCLAvailable;
}

తార్కిక useOpenCL()
{
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    ఒకవేళ (దత్తాంశం.useOpenCL < 0)
    {
        ప్రయత్నించు
        {
            దత్తాంశం.useOpenCL = 0;
            ఒకవేళ (haveOpenCL())
            {
                స్వయంచాలక c = OpenCLExecutionContext::getCurrent();
                దత్తాంశం.useOpenCL = c.useOpenCL();
            }
        }
        పట్టుకో (...)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    తిరిగి దత్తాంశం.useOpenCL > 0;
}

తార్కిక isOpenCLActivated()
{
    ఒకవేళ (!g_isOpenCLAvailable)
        తిరిగి అబద్ధం; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    తిరిగి useOpenCL();
}

శూన్య సమితి_Use_Open_CL(తార్కిక flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    స్వయంచాలక& c = OpenCLExecutionContext::getCurrentRef();
    ఒకవేళ (!c.empty())
    {
        c.సమితి_Use_Open_CL(flag);
        దత్తాంశం.useOpenCL = c.useOpenCL();
    }
    లేకపోతే
    {
        ఒకవేళ (!flag)
            దత్తాంశం.useOpenCL = 0;
        లేకపోతే
            దత్తాంశం.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#నిర్వచితమైతే HAVE_CLAMDBLAS

తరగతి AmdBlasHelper
{
బహిరంగ:
    స్థిర AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, కొత్త AmdBlasHelper())
    }

    తార్కిక is_అందుబాటు() స్థిరరాశి
    {
        తిరిగి g_is_Amd_Blas_అందుబాటు;
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

రక్షిత:
    AmdBlasHelper()
    {
        ఒకవేళ (!g_isAmdBlasInitialized)
        {
            Auto_తాళం lock(getInitializationMutex());

            ఒకవేళ (!g_isAmdBlasInitialized)
            {
                ఒకవేళ (haveOpenCL())
                {
                    ప్రయత్నించు
                    {
                        g_is_Amd_Blas_అందుబాటు = clblas_అమరిక() == clblasSuccess;
                    }
                    పట్టుకో (...)
                    {
                        g_is_Amd_Blas_అందుబాటు = అబద్ధం;
                    }
                }
                లేకపోతే
                    g_is_Amd_Blas_అందుబాటు = అబద్ధం;

                g_isAmdBlasInitialized = నిజం;
            }
        }
    }

అంతరంగ:
    స్థిర తార్కిక g_isAmdBlasInitialized;
    స్థిర తార్కిక g_is_Amd_Blas_అందుబాటు;
};

తార్కిక AmdBlasHelper::g_is_Amd_Blas_అందుబాటు = అబద్ధం;
తార్కిక AmdBlasHelper::g_isAmdBlasInitialized = అబద్ధం;

తార్కిక haveAmdBlas()
{
    తిరిగి AmdBlasHelper::getInstance().is_అందుబాటు();
}

#లేకపోతే

తార్కిక haveAmdBlas()
{
    తిరిగి అబద్ధం;
}

#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_CLAMDFFT

తరగతి AmdFftHelper
{
బహిరంగ:
    స్థిర AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, కొత్త AmdFftHelper())
    }

    తార్కిక is_అందుబాటు() స్థిరరాశి
    {
        తిరిగి g_is_Amd_Fft_అందుబాటు;
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

రక్షిత:
    AmdFftHelper()
    {
        ఒకవేళ (!g_isAmdFftInitialized)
        {
            Auto_తాళం lock(getInitializationMutex());

            ఒకవేళ (!g_isAmdFftInitialized)
            {
                ఒకవేళ (haveOpenCL())
                {
                    ప్రయత్నించు
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_అమరిక_దత్తాంశం(&అమరిక_దత్తాంశం) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_సంస్కరణ(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_అందుబాటు = నిజం;
                    }
                    పట్టుకో (స్థిరరాశి Exception &)
                    {
                        g_is_Amd_Fft_అందుబాటు = అబద్ధం;
                    }
                }
                లేకపోతే
                    g_is_Amd_Fft_అందుబాటు = అబద్ధం;

                g_isAmdFftInitialized = నిజం;
            }
        }
    }

అంతరంగ:
    స్థిర clfft_అమరిక_దత్తాంశం అమరిక_దత్తాంశం;
    స్థిర తార్కిక g_isAmdFftInitialized;
    స్థిర తార్కిక g_is_Amd_Fft_అందుబాటు;
};

clfft_అమరిక_దత్తాంశం AmdFftHelper::అమరిక_దత్తాంశం;
తార్కిక AmdFftHelper::g_is_Amd_Fft_అందుబాటు = అబద్ధం;
తార్కిక AmdFftHelper::g_isAmdFftInitialized = అబద్ధం;

తార్కిక haveAmdFft()
{
    తిరిగి AmdFftHelper::getInstance().is_అందుబాటు();
}

#లేకపోతే

తార్కిక haveAmdFft()
{
    తిరిగి అబద్ధం;
}

#అయితే_ముగింపు

తార్కిక haveSVM()
{
#నిర్వచితమైతే HAVE_OPENCL_SVM
    తిరిగి నిజం;
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య ముగింపు_2()
{
    వరుస::getDefault().ముగింపు_2();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

నిర్మాణం Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = అబద్ధం;
    }

    ~Impl() {}

    శూన్య init()
    {
        ఒకవేళ( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            ఒకవేళ( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            ఒకవేళ( handle != 0 )
            {
                అక్షరం buf[1000];
                size_t len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, పరిమాణం(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = నిజం;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    తార్కిక initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    ఒకవేళ(p)
        p->release();
}

Platform::Platform(స్థిరరాశి Platform& pl)
{
    p = (Impl*)pl.p;
    ఒకవేళ(p)
        p->addref();
}

Platform& Platform::క్రియాకారకం = (స్థిరరాశి Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = శూన్యసూచిక;
}

Platform& Platform::క్రియాకారకం = (Platform&& pl) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &pl) {
        ఒకవేళ(p)
            p->release();
        p = pl.p;
        pl.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

శూన్య* Platform::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(ఖాళీ, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    స్థిర Platform p;
    ఒకవేళ( !p.p )
    {
        p.p = కొత్త Impl;
        p.p->init();
    }
    తిరిగి p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
స్థిర శూన్య parseOpenCLVersion(స్థిరరాశి String &సంస్కరణ, పూర్ణాంకం &major, పూర్ణాంకం &minor)
{
    major = minor = 0;
    ఒకవేళ (10 >= సంస్కరణ.length())
        తిరిగి;
    స్థిరరాశి అక్షరం *pstr = సంస్కరణ.c_str();
    ఒకవేళ (0 != అక్షరశ్రేణి_పోలిక(pstr, "OpenCL ", 7))
        తిరిగి;
    size_t ppos = సంస్కరణ.find('.', 7);
    ఒకవేళ (String::npos == ppos)
        తిరిగి;
    String తాత్కాలిక_2 = సంస్కరణ.substr(7, ppos - 7);
    major = atoi(తాత్కాలిక_2.c_str());
    తాత్కాలిక_2 = సంస్కరణ.substr(ppos + 1);
    minor = atoi(తాత్కాలిక_2.c_str());
}

నిర్మాణం Device::Impl
{
    Impl(శూన్య* d)
        : refcount(1)
        , handle(0)
    {
        ప్రయత్నించు
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        పట్టుకో (...)
        {
            విసురు;
        }
    }

    శూన్య _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        సంస్కరణ_3 = getStrProp(CL_DEVICE_సంస్కరణ);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, పూర్ణాంకం>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, పూర్ణాంకం>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_జ్ఞాపకం = getBoolProp(CL_DEVICE_HOST_UNIFIED_జ్ఞాపకం);
        maxComputeUnits_ = getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_సమూహం_కొలత_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_సమూహం_కొలత);
        type_ = getProp<cl_device_type, పూర్ణాంకం>(CL_DEVICE_TYPE);
        driver_సంస్కరణ_2 = getStrProp(CL_DRIVER_సంస్కరణ);
        చిరునామా_బిట్లు_2 = getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_చిరునామా_బిట్లు);

        String device_సంస్కరణ = getStrProp(CL_DEVICE_సంస్కరణ);
        parseOpenCLVersion(device_సంస్కరణ, device_సంస్కరణ_Major_2, device_సంస్కరణ_Minor_2);

        size_t pos = 0;
        అయితే (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            ఒకవేళ (pos2 == String::npos)
                pos2 = extensions_.size();
            ఒకవేళ (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_సమితి.insert(extensionName);
            }
            pos = pos2 + 1;
        }

        khr_fp64_support_ = isExtensionSupported("cl_khr_fp64");
        khr_fp16_support_ = isExtensionSupported("cl_khr_fp16");

        intelSubgroupsSupport_ = isExtensionSupported("cl_intel_subgroups");

        vendorName_ = getStrProp(CL_DEVICE_VENDOR);
        ఒకవేళ (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        లేకపోతే ఒకవేళ (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || అక్షరశ్రేణి_శోధన(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        లేకపోతే ఒకవేళ (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        లేకపోతే
            vendorID_ = UNKNOWN_VENDOR;

        స్థిరరాశి size_t CV_OPENCL_DEVICE_MAX_WORK_సమూహం_కొలత = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        ఒకవేళ (CV_OPENCL_DEVICE_MAX_WORK_సమూహం_కొలత > 0)
        {
            స్థిరరాశి size_t new_max_Work_సమూహం_కొలత = std::min(max_Work_సమూహం_కొలత_2, CV_OPENCL_DEVICE_MAX_WORK_సమూహం_కొలత);
            ఒకవేళ (new_max_Work_సమూహం_కొలత != max_Work_సమూహం_కొలత_2)
                CV_LOG_WARNING(ఖాళీ, "OpenCL: using workgroup size: " << new_max_Work_సమూహం_కొలత << " (was " << max_Work_సమూహం_కొలత_2 << ")");
            max_Work_సమూహం_కొలత_2 = new_max_Work_సమూహం_కొలత;
        }
#ఒకవేళ 0
        ఒకవేళ (isExtensionSupported("cl_khr_spir"))
        {
#నిర్వచితం_కాకపోతే CL_DEVICE_SPIR_VERSIONS
#నిర్వచించు CL_DEVICE_SPIR_VERSIONS                     0x40E0
#అయితే_ముగింపు
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::బయటకు << spir_versions << std::పంక్తి_ముగింపు;
        }
#అయితే_ముగింపు
    }

    ~Impl()
    {
#నిర్వచితమైతే _WIN32
        ఒకవేళ (!cv::__termination)
#అయితే_ముగింపు
        {
            ఒకవేళ (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    మూస<రకం_పేరు _TpCL, రకం_పేరు Tp_బహిర్గతం>
    Tp_బహిర్గతం getProp(cl_device_info prop) స్థిరరాశి
    {
        _TpCL తాత్కాలిక_2=_TpCL();
        size_t sz = 0;

        తిరిగి clGetDeviceInfo(handle, prop, పరిమాణం(తాత్కాలిక_2), &తాత్కాలిక_2, &sz) == CL_SUCCESS &&
            sz == పరిమాణం(తాత్కాలిక_2) ? Tp_బహిర్గతం(తాత్కాలిక_2) : Tp_బహిర్గతం();
    }

    తార్కిక getBoolProp(cl_device_info prop) స్థిరరాశి
    {
        cl_bool తాత్కాలిక_2 = CL_FALSE;
        size_t sz = 0;

        తిరిగి clGetDeviceInfo(handle, prop, పరిమాణం(తాత్కాలిక_2), &తాత్కాలిక_2, &sz) == CL_SUCCESS &&
            sz == పరిమాణం(తాత్కాలిక_2) ? తాత్కాలిక_2 != 0 : అబద్ధం;
    }

    String getStrProp(cl_device_info prop) స్థిరరాశి
    {
        అక్షరం buf[4096];
        size_t sz=0;
        తిరిగి clGetDeviceInfo(handle, prop, పరిమాణం(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < పరిమాణం(buf) ? String(buf) : String();
    }

    తార్కిక isExtensionSupported(స్థిరరాశి std::string& extensionName) స్థిరరాశి
    {
        తిరిగి extensions_సమితి.లెక్క_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String సంస్కరణ_3;
    std::string extensions_;
    పూర్ణాంకం doubleFPConfig_;
    తార్కిక khr_fp64_support_;
    పూర్ణాంకం halfFPConfig_;
    తార్కిక khr_fp16_support_;
    తార్కిక host_Unified_జ్ఞాపకం;
    పూర్ణాంకం maxComputeUnits_;
    size_t max_Work_సమూహం_కొలత_2;
    పూర్ణాంకం type_;
    పూర్ణాంకం చిరునామా_బిట్లు_2;
    పూర్ణాంకం device_సంస్కరణ_Major_2;
    పూర్ణాంకం device_సంస్కరణ_Minor_2;
    String driver_సంస్కరణ_2;
    String vendorName_;
    పూర్ణాంకం vendorID_;
    తార్కిక intelSubgroupsSupport_;

    std::సమితి<std::string> extensions_సమితి;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(శూన్య* d)
{
    p = 0;
    సమితి(d);
}

Device::Device(స్థిరరాశి Device& d)
{
    p = d.p;
    ఒకవేళ(p)
        p->addref();
}

Device& Device::క్రియాకారకం = (స్థిరరాశి Device& d)
{
    Impl* newp = (Impl*)d.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = శూన్యసూచిక;
}

Device& Device::క్రియాకారకం = (Device&& d) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &d) {
        ఒకవేళ(p)
            p->release();
        p = d.p;
        d.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

Device::~Device()
{
    ఒకవేళ(p)
        p->release();
}

శూన్య Device::సమితి(శూన్య* d)
{
    ఒకవేళ(p)
        p->release();
    p = కొత్త Impl(d);
    ఒకవేళ (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(శూన్య* d)
{
    Device device(d);
    తిరిగి device;
}

శూన్య* Device::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

String Device::name() స్థిరరాశి
{ తిరిగి p ? p->name_ : String(); }

String Device::extensions() స్థిరరాశి
{ తిరిగి p ? String(p->extensions_) : String(); }

తార్కిక Device::isExtensionSupported(స్థిరరాశి String& extensionName) స్థిరరాశి
{ తిరిగి p ? p->isExtensionSupported(extensionName) : అబద్ధం; }

String Device::సంస్కరణ() స్థిరరాశి
{ తిరిగి p ? p->సంస్కరణ_3 : String(); }

String Device::vendorName() స్థిరరాశి
{ తిరిగి p ? p->vendorName_ : String(); }

పూర్ణాంకం Device::vendorID() స్థిరరాశి
{ తిరిగి p ? p->vendorID_ : 0; }

String Device::Open_CL_C_సంస్కరణ() స్థిరరాశి
{ తిరిగి p ? p->getStrProp(CL_DEVICE_OPENCL_C_సంస్కరణ) : String(); }

String Device::OpenCLVersion() స్థిరరాశి
{ తిరిగి p ? p->getStrProp(CL_DEVICE_సంస్కరణ) : String(); }

పూర్ణాంకం Device::device_సంస్కరణ_Major() స్థిరరాశి
{ తిరిగి p ? p->device_సంస్కరణ_Major_2 : 0; }

పూర్ణాంకం Device::device_సంస్కరణ_Minor() స్థిరరాశి
{ తిరిగి p ? p->device_సంస్కరణ_Minor_2 : 0; }

String Device::driver_సంస్కరణ() స్థిరరాశి
{ తిరిగి p ? p->driver_సంస్కరణ_2 : String(); }

పూర్ణాంకం Device::type() స్థిరరాశి
{ తిరిగి p ? p->type_ : 0; }

పూర్ణాంకం Device::చిరునామా_బిట్లు() స్థిరరాశి
{ తిరిగి p ? p->చిరునామా_బిట్లు_2 : 0; }

తార్కిక Device::అందుబాటు() స్థిరరాశి
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_అందుబాటు) : అబద్ధం; }

తార్కిక Device::compiler_అందుబాటు() స్థిరరాశి
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_COMPILER_అందుబాటు) : అబద్ధం; }

తార్కిక Device::linker_అందుబాటు() స్థిరరాశి
#నిర్వచితమైతే CL_సంస్కరణ_1_2
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_LINKER_అందుబాటు) : అబద్ధం; }
#లేకపోతే
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#అయితే_ముగింపు

పూర్ణాంకం Device::doubleFPConfig() స్థిరరాశి
{ తిరిగి p ? p->doubleFPConfig_ : 0; }

పూర్ణాంకం Device::singleFPConfig() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_device_fp_config, పూర్ణాంకం>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

పూర్ణాంకం Device::halfFPConfig() స్థిరరాశి
{ తిరిగి p ? p->halfFPConfig_ : 0; }

తార్కిక Device::hasFP64() స్థిరరాశి
{ తిరిగి p ? p->khr_fp64_support_ : అబద్ధం; }
తార్కిక Device::hasFP16() స్థిరరాశి
{ తిరిగి p ? p->khr_fp16_support_ : అబద్ధం; }

తార్కిక Device::endianLittle() స్థిరరాశి
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : అబద్ధం; }

తార్కిక Device::errorCorrectionSupport() స్థిరరాశి
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : అబద్ధం; }

పూర్ణాంకం Device::executionCapabilities() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_device_exec_capabilities, పూర్ణాంకం>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_కొలత) : 0; }

పూర్ణాంకం Device::globalMemCacheType() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_device_mem_cache_type, పూర్ణాంకం>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

పూర్ణాంకం Device::global_Mem_Cache_పంక్తి_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_GLOBAL_MEM_CACHELINE_కొలత) : 0; }

size_t Device::global_Mem_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_కొలత) : 0; }

size_t Device::local_Mem_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_కొలత) : 0; }

పూర్ణాంకం Device::localMemType() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_device_local_mem_type, పూర్ణాంకం>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

తార్కిక Device::host_Unified_జ్ఞాపకం_2() స్థిరరాశి
{ తిరిగి p ? p->host_Unified_జ్ఞాపకం : అబద్ధం; }

తార్కిక Device::imageSupport() స్థిరరాశి
{ తిరిగి p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : అబద్ధం; }

తార్కిక Device::image_From_బఫరు_Support() స్థిరరాశి
{
    తిరిగి p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : అబద్ధం;
}

uint Device::imagePitchAlignment() స్థిరరాశి
{
#నిర్వచితమైతే CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    తిరిగి p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
}

uint Device::image_పునాది_చిరునామా_Alignment() స్థిరరాశి
{
#నిర్వచితమైతే CL_DEVICE_IMAGE_పునాది_చిరునామా_ALIGNMENT
    తిరిగి p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_పునాది_చిరునామా_ALIGNMENT) : 0;
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
}

size_t Device::image2_DMax_వెడల్పు() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_వెడల్పు) : 0; }

size_t Device::image2_DMax_ఎత్తు() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ఎత్తు) : 0; }

size_t Device::image3_DMax_వెడల్పు() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_వెడల్పు) : 0; }

size_t Device::image3_DMax_ఎత్తు() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ఎత్తు) : 0; }

size_t Device::image3_DMax_లోతు() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_లోతు) : 0; }

size_t Device::image_Max_బఫరు_కొలత() స్థిరరాశి
#నిర్వచితమైతే CL_సంస్కరణ_1_2
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_బఫరు_కొలత) : 0; }
#లేకపోతే
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#అయితే_ముగింపు

size_t Device::image_Max_Array_కొలత() స్థిరరాశి
#నిర్వచితమైతే CL_సంస్కరణ_1_2
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_కొలత) : 0; }
#లేకపోతే
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#అయితే_ముగింపు

తార్కిక Device::intelSubgroupsSupport() స్థిరరాశి
{ తిరిగి p ? p->intelSubgroupsSupport_ : అబద్ధం; }

పూర్ణాంకం Device::maxClockFrequency() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

పూర్ణాంకం Device::maxComputeUnits() స్థిరరాశి
{ తిరిగి p ? p->maxComputeUnits_ : 0; }

పూర్ణాంకం Device::max_Constant_వాదనలు() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_CONSTANT_వాదనలు) : 0; }

size_t Device::max_Constant_బఫరు_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_బఫరు_కొలత) : 0; }

size_t Device::max_Mem_Alloc_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_కొలత) : 0; }

size_t Device::max_Parameter_కొలత() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_కొలత) : 0; }

పూర్ణాంకం Device::max_Read_Image_వాదనలు() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_READ_IMAGE_వాదనలు) : 0; }

పూర్ణాంకం Device::max_Write_Image_వాదనలు() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_WRITE_IMAGE_వాదనలు) : 0; }

పూర్ణాంకం Device::maxSamplers() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_సమూహం_కొలత() స్థిరరాశి
{ తిరిగి p ? p->max_Work_సమూహం_కొలత_2 : 0; }

పూర్ణాంకం Device::max_Work_అంశం_Dims() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MAX_WORK_అంశం_DIMENSIONS) : 0; }

శూన్య Device::max_Work_అంశం_Sizes(size_t* sizes) స్థిరరాశి
{
    ఒకవేళ(p)
    {
        స్థిరరాశి పూర్ణాంకం MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_అంశం_SIZES,
                MAX_DIMS*పరిమాణం(sizes[0]), &sizes[0], &retsz));
    }
}

పూర్ణాంకం Device::mem_పునాది_Addr_Align() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_MEM_పునాది_ADDR_ALIGN) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Char() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_CHAR) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Short() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_SHORT) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Int() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_INT) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Long() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_LONG) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Float() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_FLOAT) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Double() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_DOUBLE) : 0; }

పూర్ణాంకం Device::native_సదిశ_వెడల్పు_Half() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_NATIVE_సదిశ_వెడల్పు_HALF) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Char() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_CHAR) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Short() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_SHORT) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Int() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_INT) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Long() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_LONG) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Float() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_FLOAT) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Double() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_DOUBLE) : 0; }

పూర్ణాంకం Device::preferred_సదిశ_వెడల్పు_Half() స్థిరరాశి
{ తిరిగి p ? p->getProp<cl_uint, పూర్ణాంకం>(CL_DEVICE_PREFERRED_సదిశ_వెడల్పు_HALF) : 0; }

size_t Device::printf_బఫరు_కొలత() స్థిరరాశి
#నిర్వచితమైతే CL_సంస్కరణ_1_2
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_బఫరు_కొలత) : 0; }
#లేకపోతే
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#అయితే_ముగింపు


size_t Device::profilingTimerResolution() స్థిరరాశి
{ తిరిగి p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

స్థిరరాశి Device& Device::getDefault()
{
    స్వయంచాలక& c = OpenCLExecutionContext::getCurrent();
    ఒకవేళ (!c.empty())
    {
        తిరిగి c.getDevice();
    }

    స్థిర Device dummy;
    తిరిగి dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

మూస <రకం_పేరు Functor, రకం_పేరు ObjectType>
అంతఃవరుస cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, ఖాళీ, &required);
    ఒకవేళ (err != CL_SUCCESS)
        తిరిగి err;

    param.clear();
    ఒకవేళ (required > 0)
    {
        Auto_బఫరు<అక్షరం> buf(required + 1);
        అక్షరం* ptr = buf.దత్తాంశం(); // cleanup is not needed
        err = f(obj, name, required, ptr, ఖాళీ);
        ఒకవేళ (err != CL_SUCCESS)
            తిరిగి err;
        param = ptr;
    }

    తిరిగి CL_SUCCESS;
}

స్థిర శూన్య విభజించు(స్థిరరాశి std::string &s, అక్షరం delim, std::vector<std::string> &elems)
{
    elems.clear();
    ఒకవేళ (s.size() == 0)
        తిరిగి;
    std::istringstream ss(s);
    std::string అంశం;
    అయితే (!ss.eof())
    {
        std::getline(ss, అంశం, delim);
        elems.push_back(అంశం);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
స్థిర తార్కిక parseOpenCLDeviceConfiguration(స్థిరరాశి std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    విభజించు(configurationStr, ':', parts);
    ఒకవేళ (parts.size() > 3)
    {
        CV_LOG_ERROR(ఖాళీ, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        తిరిగి అబద్ధం;
    }
    ఒకవేళ (parts.size() > 2)
        deviceNameOrID = parts[2];
    ఒకవేళ (parts.size() > 1)
    {
        విభజించు(parts[1], '|', deviceTypes);
    }
    ఒకవేళ (parts.size() > 0)
    {
        platform = parts[0];
    }
    తిరిగి నిజం;
}

స్థిర cl_device_id selectOpenCLDevice(స్థిరరాశి std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    ఒకవేళ (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    ఒకవేళ (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        తిరిగి ఖాళీ;

    తార్కిక isID = అబద్ధం;
    పూర్ణాంకం deviceID = -1;
    ఒకవేళ (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = నిజం;
        కొరకు (size_t i = 0; i < deviceName.length(); i++)
        {
            ఒకవేళ (!అంకెనా(deviceName[i]))
            {
                isID = అబద్ధం;
                ఆపు;
            }
        }
        ఒకవేళ (isID)
        {
            deviceID = atoi(deviceName.c_str());
            ఒకవేళ (deviceID < 0)
                తిరిగి ఖాళీ;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint అంకె_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms));

        ఒకవేళ (అంకె_Platforms == 0)
            తిరిగి ఖాళీ;
        platforms.resize((size_t)అంకె_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(అంకె_Platforms, &platforms[0], &అంకె_Platforms));
        platforms.resize(అంకె_Platforms);
    }

    ఒకవేళ (platform.length() > 0)
    {
        కొరకు (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            ఒకవేళ (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            లేకపోతే
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        ఒకవేళ (platforms.size() == 0)
        {
            CV_LOG_ERROR(ఖాళీ, "OpenCL: Can't find OpenCL platform by name: " << platform);
            వెళ్ళు not_దొరికింది;
        }
    }
    ఒకవేళ (deviceTypes.size() == 0)
    {
        ఒకవేళ (!isID)
        {
            deviceTypes.push_back("GPU");
            ఒకవేళ (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        లేకపోతే
            deviceTypes.push_back("ALL");
    }
    కొరకు (size_t t = 0; t < deviceTypes.size(); t++)
    {
        పూర్ణాంకం deviceType = 0;
        std::string తాత్కాలిక_Str_Device_Type = deviceTypes[t];
        std::transform(తాత్కాలిక_Str_Device_Type.begin(), తాత్కాలిక_Str_Device_Type.end(), తాత్కాలిక_Str_Device_Type.begin(), details::char_tolower);

        ఒకవేళ (తాత్కాలిక_Str_Device_Type == "gpu" || తాత్కాలిక_Str_Device_Type == "dgpu" || తాత్కాలిక_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        లేకపోతే ఒకవేళ (తాత్కాలిక_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        లేకపోతే ఒకవేళ (తాత్కాలిక_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        లేకపోతే ఒకవేళ (తాత్కాలిక_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        లేకపోతే
        {
            CV_LOG_ERROR(ఖాళీ, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            వెళ్ళు not_దొరికింది;
        }

        std::vector<cl_device_id> devices;
        కొరకు (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint లెక్క_2 = 0;
            cl_int స్థితి_2 = clGetDeviceIDs(*currentPlatform, deviceType, 0, ఖాళీ, &లెక్క_2);
            ఒకవేళ (!(స్థితి_2 == CL_SUCCESS || స్థితి_2 == CL_DEVICE_NOT_దొరికింది))
            {
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, "clGetDeviceIDs get count");
            }
            ఒకవేళ (లెక్క_2 == 0)
                కొనసాగు;
            size_t పునాది = devices.size();
            devices.resize(పునాది + లెక్క_2);
            స్థితి_2 = clGetDeviceIDs(*currentPlatform, deviceType, లెక్క_2, &devices[పునాది], &లెక్క_2);
            ఒకవేళ (!(స్థితి_2 == CL_SUCCESS || స్థితి_2 == CL_DEVICE_NOT_దొరికింది))
            {
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, "clGetDeviceIDs get IDs");
            }
        }

        కొరకు (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : నిజం) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = నిజం;
            ఒకవేళ(తాత్కాలిక_Str_Device_Type == "dgpu" || తాత్కాలిక_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_జ్ఞాపకం, పరిమాణం(isIGPU), &isIGPU, ఖాళీ));
                useGPU = తాత్కాలిక_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            ఒకవేళ ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                తిరిగి devices[i];
            }
        }
    }

not_దొరికింది:
    ఒకవేళ (configuration.empty())
        తిరిగి ఖాళీ; // suppress messages on stderr

    std::ostringstream సందేశం;
    సందేశం << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::పంక్తి_ముగింపు
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::పంక్తి_ముగింపు
        << "    Device types:";
    కొరకు (size_t t = 0; t < deviceTypes.size(); t++)
        సందేశం << ' ' << deviceTypes[t];

    సందేశం << std::పంక్తి_ముగింపు << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(ఖాళీ, సందేశం.str());
    తిరిగి ఖాళీ;
}

#నిర్వచితమైతే HAVE_OPENCL_SVM
నామాకాశ svm {

గణన AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_బఫరు = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_బఫరు = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_బఫరు_ముసుగు = 3 << 16,
        OPENCL_SVM_బఫరు_MAP = 4 << 16
};

స్థిర తార్కిక checkForceSVMUmatUsage()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక force = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి force;
}
స్థిర తార్కిక checkDisableSVMUMatUsage()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక force = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి force;
}
స్థిర తార్కిక checkDisableSVM()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక force = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి force;
}
// see SVMCapabilities
స్థిర గుర్తులేని పూర్ణాంకం get_SVMCapabilities_ముసుగు()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర గుర్తులేని పూర్ణాంకం ముసుగు = 0;
    ఒకవేళ (!initialized)
    {
        స్థిరరాశి std::string env_విలువ = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        ఒకవేళ (env_విలువ.empty())
        {
            తిరిగి ~0U; // all bits 1
        }
        ముసుగు = atoi(env_విలువ.c_str());
        initialized = నిజం;
    }
    తిరిగి ముసుగు;
}
} // namespace
#అయితే_ముగింపు

స్థిర size_t get_కార్యక్రమం_లెక్క_హద్దు()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర size_t లెక్క_2 = 0;
    ఒకవేళ (!initialized)
    {
        లెక్క_2 = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = నిజం;
    }
    తిరిగి లెక్క_2;
}

స్థిర పూర్ణాంకం g_contextId = 0;

తరగతి OpenCLBufferPoolImpl;
తరగతి OpenCLSVMBufferPoolImpl;

నిర్మాణం Context::Impl
{
    స్థిర Context::Impl* get(Context& context) { తిరిగి context.getImpl(); }

    రకనిర్వచనం std::deque<Context::Impl*> container_t;
    స్థిర container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        స్థిర container_t* g_contexts = కొత్త container_t();
        తిరిగి *g_contexts;
    }

రక్షిత:
    Impl(స్థిరరాశి std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#నిర్వచితమైతే HAVE_OPENCL_SVM
        , svmInitialized(అబద్ధం)
#అయితే_ముగింపు
    {
        ఒకవేళ (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_తాళం lock(cv::getInitializationMutex());
        స్వయంచాలక& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = ఇది;
    }

    ~Impl()
    {
#నిర్వచితమైతే _WIN32
        ఒకవేళ (!cv::__termination)
#అయితే_ముగింపు
        {
            ఒకవేళ (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = ఖాళీ;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::Auto_తాళం lock(cv::getInitializationMutex());
            స్వయంచాలక& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = ఖాళీ;
        }
    }

    శూన్య init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_అంకె_DEVICES, పరిమాణం(ndevices), &ndevices, ఖాళీ));
        CV_Assert(ndevices > 0);

        cv::Auto_బఫరు<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_కొలత = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * పరిమాణం(cl_device_id), &cl_devices[0], &devices_ret_కొలత));
        CV_CheckEQ(devices_ret_కొలత, cl_devices.size() * పరిమాణం(cl_device_id), "");

        devices.clear();
        కొరకు (గుర్తులేని i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    శూన్య __init_buffer_pools();  // w/o synchronization
    శూన్య init_బఫరు_pools() స్థిరరాశి
    {
        ఒకవేళ (!బఫరు_Pool_2)
        {
            cv::Auto_తాళం lock(cv::getInitializationMutex());
            ఒకవేళ (!బఫరు_Pool_2)
            {
                const_cast<Impl*>(ఇది)->__init_buffer_pools();
            }
        }
    }
బహిరంగ:
    స్థిర Impl* కనుగొను_Context(స్థిరరాశి std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_తాళం lock(cv::getInitializationMutex());
        స్వయంచాలక& container = getGlobalContainer();
        ఒకవేళ (configuration.empty() && !container.empty())
            తిరిగి container[0];
        కొరకు (స్వయంచాలక it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            ఒకవేళ (i && i->configuration == configuration)
            {
                తిరిగి i;
            }
        }
        తిరిగి ఖాళీ;
    }

    స్థిర Impl* కనుగొను_Or_Create_Context(స్థిరరాశి std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        ఒకవేళ (configuration_.empty())
        {
            స్థిరరాశి std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            ఒకవేళ (!c.empty())
                configuration = c;
        }
        Impl* impl = కనుగొను_Context(configuration);
        ఒకవేళ (impl)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            తిరిగి impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        ఒకవేళ (d == ఖాళీ)
            తిరిగి ఖాళీ;

        impl = కొత్త Impl(configuration);
        ప్రయత్నించు
        {
            impl->createFromDevice(d);
            ఒకవేళ (impl->handle)
                తిరిగి impl;
            తొలగించు impl;
            తిరిగి ఖాళీ;
        }
        పట్టుకో (...)
        {
            తొలగించు impl;
            విసురు;
        }
    }

    స్థిర Impl* కనుగొను_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (శూన్య*)h);
        Impl* impl = కనుగొను_Context(configuration);
        ఒకవేళ (impl)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            తిరిగి impl;
        }

        impl = కొత్త Impl(configuration);
        ప్రయత్నించు
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            తిరిగి impl;
        }
        పట్టుకో (...)
        {
            తొలగించు impl;
            విసురు;
        }
    }

    స్థిర Impl* కనుగొను_Or_Create_Context(స్థిరరాశి ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (శూన్య*)d);
        Impl* impl = కనుగొను_Context(configuration);
        ఒకవేళ (impl)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            తిరిగి impl;
        }

        impl = కొత్త Impl(configuration);
        ప్రయత్నించు
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            తిరిగి impl;
        }
        పట్టుకో (...)
        {
            తొలగించు impl;
            విసురు;
        }
    }

    శూన్య సమితి_Default()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        ఒకవేళ (d == ఖాళీ)
            తిరిగి;

        createFromDevice(d);
    }

    శూన్య createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == ఖాళీ);

        cl_platform_id pl = ఖాళీ;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, పరిమాణం(cl_platform_id), &pl, ఖాళీ));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int స్థితి_2;

        handle = clCreateContext(prop, nd, &d, 0, 0, &స్థితి_2);
        CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, "clCreateContext");

        తార్కిక ok = handle != 0 && స్థితి_2 == CL_SUCCESS;
        ఒకవేళ( ok )
        {
            devices.resize(nd);
            devices[0].సమితి(d);
        }
        లేకపోతే
            handle = ఖాళీ;
    }

    కార్యక్రమం_2 getProg(స్థిరరాశి కార్యక్రమం_ఆధారం& src, స్థిరరాశి String& buildflags, String& errmsg);

    శూన్య unloadProg(కార్యక్రమం_2& prog)
    {
        cv::Auto_తాళం lock(కార్యక్రమం_cache_mutex);
        కొరకు (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              ఒకవేళ (it != phash.end())
              {
                  ఒకవేళ (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      తిరిగి;
                  }
              }
        }
    }

    std::string& get_ఉపసర్గ_String()
    {
        ఒకవేళ (ఉపసర్గ.empty())
        {
            cv::Auto_తాళం lock(కార్యక్రమం_cache_mutex);
            ఒకవేళ (ఉపసర్గ.empty())
            {
                CV_Assert(!devices.empty());
                స్థిరరాశి Device& d = devices[0];
                పూర్ణాంకం బిట్లు_2 = d.చిరునామా_బిట్లు();
                ఒకవేళ (బిట్లు_2 > 0 && బిట్లు_2 != 64)
                    ఉపసర్గ = cv::format("%d-bit--", బిట్లు_2);
                ఉపసర్గ += d.vendorName() + "--" + d.name() + "--" + d.driver_సంస్కరణ();
                // sanitize chars
                కొరకు (size_t i = 0; i < ఉపసర్గ.size(); i++)
                {
                    అక్షరం c = ఉపసర్గ[i];
                    ఒకవేళ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ఉపసర్గ[i] = '_';
                    }
                }
            }
        }
        తిరిగి ఉపసర్గ;
    }

    std::string& get_ఉపసర్గ_పునాది()
    {
        ఒకవేళ (ఉపసర్గ_పునాది.empty())
        {
            cv::Auto_తాళం lock(కార్యక్రమం_cache_mutex);
            ఒకవేళ (ఉపసర్గ_పునాది.empty())
            {
                స్థిరరాశి Device& d = devices[0];
                పూర్ణాంకం బిట్లు_2 = d.చిరునామా_బిట్లు();
                ఒకవేళ (బిట్లు_2 > 0 && బిట్లు_2 != 64)
                    ఉపసర్గ_పునాది = cv::format("%d-bit--", బిట్లు_2);
                ఉపసర్గ_పునాది += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                కొరకు (size_t i = 0; i < ఉపసర్గ_పునాది.size(); i++)
                {
                    అక్షరం c = ఉపసర్గ_పునాది[i];
                    ఒకవేళ (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        ఉపసర్గ_పునాది[i] = '_';
                    }
                }
            }
        }
        తిరిగి ఉపసర్గ_పునాది;
    }

    IMPLEMENT_REFCOUNTABLE();

    స్థిరరాశి పూర్ణాంకం contextId;  // global unique ID
    స్థిరరాశి std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string ఉపసర్గ;
    std::string ఉపసర్గ_పునాది;

    cv::Mutex కార్యక్రమం_cache_mutex;
    రకనిర్వచనం std::map<std::string, కార్యక్రమం_2> phash_t;
    phash_t phash;
    రకనిర్వచనం std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> బఫరు_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> బఫరు_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_బఫరు_Pool() స్థిరరాశి
    {
        init_బఫరు_pools();
        CV_DbgAssert(బఫరు_Pool_2);
        తిరిగి *బఫరు_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_బఫరు_Pool_Host_Ptr() స్థిరరాశి
    {
        init_బఫరు_pools();
        CV_DbgAssert(బఫరు_Pool_Host_Ptr);
        తిరిగి *బఫరు_Pool_Host_Ptr.get();
    }

    std::map<std::type_సూచీ, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    శూన్య సమితి_User_Context(std::type_సూచీ typeId, స్థిరరాశి std::shared_ptr<UserContext>& userContext) {
        cv::Auto_తాళం lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_సూచీ typeId) {
        cv::Auto_తాళం lock(userContextMutex);
        స్వయంచాలక it = userContextStorage.find(typeId);
        ఒకవేళ (it != userContextStorage.end())
            తిరిగి it->second;
        లేకపోతే
            తిరిగి శూన్యసూచిక;
    }

#నిర్వచితమైతే HAVE_OPENCL_SVM
    తార్కిక svmInitialized;
    తార్కిక svm_అందుబాటు;
    తార్కిక svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    శూన్య svmInit()
    {
        CV_Assert(handle != ఖాళీ);
        స్థిరరాశి Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((శూన్య)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int స్థితి_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, పరిమాణం(deviceCaps), &deviceCaps, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", స్థితి_2);
            వెళ్ళు noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (పూర్ణాంకం)deviceCaps);
        CV_Assert(((శూన్య)0, CL_DEVICE_SVM_COARSE_GRAIN_బఫరు == CL_DEVICE_SVM_COARSE_GRAIN_బఫరు_AMD)); // Check assumption
        svmCapabilities.విలువ_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_బఫరు) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_బఫరు : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_బఫరు) ? svm::SVMCapabilities::SVM_FINE_GRAIN_బఫరు : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.విలువ_2 &= svm::get_SVMCapabilities_ముసుగు();
        ఒకవేళ (svmCapabilities.విలువ_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            వెళ్ళు noSVM;
        }
        ప్రయత్నించు
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            శూన్య* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            ఒకవేళ (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            ప్రయత్నించు
            {
                తార్కిక error = అబద్ధం;
                cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
                ఒకవేళ (CL_SUCCESS != cl_వరుసలో_చేర్చు_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, ఖాళీ, ఖాళీ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_ముగింపు(q);
                ప్రయత్నించు
                {
                    ((పూర్ణాంకం*)ptr)[0] = 100;
                }
                పట్టుకో (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = నిజం;
                }
                ఒకవేళ (CL_SUCCESS != cl_వరుసలో_చేర్చు_SVMUnmap(q, ptr, 0, ఖాళీ, ఖాళీ))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_ముగింపు(q);
                ఒకవేళ (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            పట్టుకో (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                విసురు;
            }
            clSVMFree(handle, ptr);
            svmFunctions.fn_clSVMAlloc = clSVMAlloc;
            svmFunctions.fn_clSVMFree = clSVMFree;
            svmFunctions.fn_cl_సమితి_Kernel_Arg_SVMPointer = cl_సమితి_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMemcpy = cl_వరుసలో_చేర్చు_SVMMemcpy;
            svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMem_Fill = cl_వరుసలో_చేర్చు_SVMMem_Fill;
            svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMap = cl_వరుసలో_చేర్చు_SVMMap;
            svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMUnmap = cl_వరుసలో_చేర్చు_SVMUnmap;
        }
        పట్టుకో (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            ప్రయత్నించు
            {
                // Try HSA extension
                String extensions = device.extensions();
                ఒకవేళ (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    వెళ్ళు noSVM;
                }
                cl_platform_id p = ఖాళీ;
                CV_OCL_CHECK(స్థితి_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, పరిమాణం(cl_platform_id), &p, ఖాళీ));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_సమితి_Kernel_Arg_SVMPointer = (cl_సమితి_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMemcpy = (cl_వరుసలో_చేర్చు_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMem_Fill = (cl_వరుసలో_చేర్చు_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMMap = (cl_వరుసలో_చేర్చు_SVMMap_AMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_వరుసలో_చేర్చు_SVMUnmap = (cl_వరుసలో_చేర్చు_SVMUnmap_AMD_fn)cl_Get_Extension_Function_చిరునామా_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            పట్టుకో (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                వెళ్ళు noSVM;
            }
        }

        svm_అందుబాటు = నిజం;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = నిజం;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        తిరిగి;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_అందుబాటు = అబద్ధం;
        svmEnabled = అబద్ధం;
        svmCapabilities.విలువ_2 = 0;
        svmInitialized = నిజం;
        svmFunctions.fn_clSVMAlloc = ఖాళీ;
        తిరిగి;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> బఫరు_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_బఫరు_Pool_SVM() స్థిరరాశి
    {
        init_బఫరు_pools();
        CV_DbgAssert(బఫరు_Pool_SVM);
        తిరిగి *బఫరు_Pool_SVM.get();
    }
#అయితే_ముగింపు

    మిత్ర తరగతి కార్యక్రమం_2;
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
Context::Context(పూర్ణాంకం dtype)
{
    p = 0;
    create(dtype);
}

శూన్య Context::release()
{
    ఒకవేళ (p)
    {
        p->release();
        p = ఖాళీ;
    }
}

తార్కిక Context::create()
{
    release();
    ఒకవేళ (!haveOpenCL())
        తిరిగి అబద్ధం;
    p = Impl::కనుగొను_Or_Create_Context(std::string());
    ఒకవేళ (p && p->handle)
        తిరిగి నిజం;
    release();
    తిరిగి అబద్ధం;
}

// deprecated
తార్కిక Context::create(పూర్ణాంకం dtype)
{
    ఒకవేళ( !haveOpenCL() )
        తిరిగి అబద్ధం;
    release();
    ఒకవేళ (dtype == CL_DEVICE_TYPE_DEFAULT || (గుర్తులేని)dtype == (గుర్తులేని)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::కనుగొను_Or_Create_Context("");
    }
    లేకపోతే ఒకవేళ (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::కనుగొను_Or_Create_Context(":GPU:");
    }
    లేకపోతే ఒకవేళ (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::కనుగొను_Or_Create_Context(":CPU:");
    }
    లేకపోతే
    {
        CV_LOG_ERROR(ఖాళీ, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    ఒకవేళ (p && !p->handle)
    {
        release();
    }
    తిరిగి p != 0;
}

Context::Context(స్థిరరాశి Context& c)
{
    p = (Impl*)c.p;
    ఒకవేళ(p)
        p->addref();
}

Context& Context::క్రియాకారకం = (స్థిరరాశి Context& c)
{
    Impl* newp = (Impl*)c.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = శూన్యసూచిక;
}

Context& Context::క్రియాకారకం = (Context&& c) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &c) {
        ఒకవేళ(p)
            p->release();
        p = c.p;
        c.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

శూన్య* Context::ptr() స్థిరరాశి
{
    తిరిగి p == ఖాళీ ? ఖాళీ : p->handle;
}

size_t Context::ndevices() స్థిరరాశి
{
    తిరిగి p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) స్థిరరాశి
{
    స్థిర Device dummy;
    తిరిగి !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    స్వయంచాలక& c = OpenCLExecutionContext::getCurrent();
    ఒకవేళ (!c.empty())
    {
        స్వయంచాలక& ctx = c.getContext();
        తిరిగి ctx;
    }

    స్థిర Context dummy;
    తిరిగి dummy;
}

కార్యక్రమం_2 Context::getProg(స్థిరరాశి కార్యక్రమం_ఆధారం& prog,
                         స్థిరరాశి String& buildopts, String& errmsg)
{
    తిరిగి p ? p->getProg(prog, buildopts, errmsg) : కార్యక్రమం_2();
}

శూన్య Context::unloadProg(కార్యక్రమం_2& prog)
{
    ఒకవేళ (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(శూన్య* context)
{
    Context ctx;
    ctx.p = Impl::కనుగొను_Or_Create_Context((cl_context)context);
    తిరిగి ctx;
}

/* static */
Context Context::fromDevice(స్థిరరాశి ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::కనుగొను_Or_Create_Context(device);
    తిరిగి ctx;
}

/* static */
Context Context::create(స్థిరరాశి std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::కనుగొను_Or_Create_Context(configuration);
    తిరిగి ctx;
}

శూన్య* Context::getOpenCLContextProperty(పూర్ణాంకం propertyId) స్థిరరాశి
{
    ఒకవేళ (p == ఖాళీ)
        తిరిగి శూన్యసూచిక;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, ఖాళీ, &size));
    std::vector<cl_context_properties> prop(size / పరిమాణం(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.దత్తాంశం(), ఖాళీ));
    కొరకు (size_t i = 0; i < prop.size(); i += 2)
    {
        ఒకవేళ (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(ఖాళీ, "OpenCL: found context property=" << propertyId << ") => " << (శూన్య*)prop[i + 1]);
            తిరిగి (శూన్య*)prop[i + 1];
        }
    }
    తిరిగి శూన్యసూచిక;
}

#నిర్వచితమైతే HAVE_OPENCL_SVM
తార్కిక Context::useSVM() స్థిరరాశి
{
    Context::Impl* i = p;
    CV_Assert(i);
    ఒకవేళ (!i->svmInitialized)
        i->svmInit();
    తిరిగి i->svmEnabled;
}
శూన్య Context::సమితి_Use_SVM(తార్కిక enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    ఒకవేళ (!i->svmInitialized)
        i->svmInit();
    ఒకవేళ (enabled && !i->svm_అందుబాటు)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#లేకపోతే
తార్కిక Context::useSVM() స్థిరరాశి { తిరిగి అబద్ధం; }
శూన్య Context::సమితి_Use_SVM(తార్కిక enabled) { CV_Assert(!enabled); }
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENCL_SVM
నామాకాశ svm {

స్థిరరాశి SVMCapabilities getSVMCapabilitites(స్థిరరాశి ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    ఒకవేళ (!i->svmInitialized)
        i->svmInit();
    తిరిగి i->svmCapabilities;
}

CV_EXPORTS స్థిరరాశి SVMFunctions* getSVMFunctions(స్థిరరాశి ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != ఖాళీ);
    తిరిగి &i->svmFunctions;
}

CV_EXPORTS తార్కిక useSVM(UMatUsageFlags usageFlags)
{
    ఒకవేళ (checkForceSVMUmatUsage())
        తిరిగి నిజం;
    ఒకవేళ (checkDisableSVMUMatUsage())
        తిరిగి అబద్ధం;
    ఒకవేళ ((usageFlags & USAGE_ALLOCATE_SHARED_జ్ఞాపకం) != 0)
        తిరిగి నిజం;
    తిరిగి అబద్ధం; // don't use SVM by default
}

} // namespace cv::ocl::svm
#అయితే_ముగింపు // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

శూన్య Context::సమితి_User_Context(std::type_సూచీ typeId, స్థిరరాశి std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->సమితి_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_సూచీ typeId)
{
    CV_Assert(p);
    తిరిగి p->getUserContext(typeId);
}

స్థిర శూన్య get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_బఫరు<అక్షరం> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.దత్తాంశం(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.దత్తాంశం();
}

/*
// Attaches OpenCL context to OpenCV
*/
శూన్య attachContext(స్థిరరాశి String& platformName, శూన్య* platformID, శూన్య* context, శూన్య* deviceID)
{
    స్వయంచాలక ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        స్థిరరాశి std::string& platformName, శూన్య* platformID, శూన్య* context, శూన్య* deviceID
)
{
    ఒకవేళ (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    ఒకవేళ (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    తార్కిక platform_అందుబాటు = అబద్ధం;

    // check if external platformName contained in list of available platforms in OpenCV
    కొరకు (గుర్తులేని పూర్ణాంకం i = 0; i < cnt; i++)
    {
        String అందుబాటు_Platform_Name;
        get_platform_name(platforms[i], అందుబాటు_Platform_Name);
        // external platform is found in the list of available platforms
        ఒకవేళ (platformName == అందుబాటు_Platform_Name)
        {
            platform_అందుబాటు = నిజం;
            ఆపు;
        }
    }

    ఒకవేళ (!platform_అందుబాటు)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    ఒకవేళ (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    తిరిగి ctx;
}

శూన్య initializeContextFromHandle(Context& ctx, శూన్య* _platform, శూన్య* _context, శూన్య* _device)
{
    // internal call, less checks
    cl_platform_id platformID = (cl_platform_id)_platform;
    cl_context context = (cl_context)_context;
    cl_device_id deviceID = (cl_device_id)_device;

    std::string platformName = PlatformInfo(&platformID).name();

    స్వయంచాలక clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

నిర్మాణం వరుస::Impl
{
    అంతఃవరుస శూన్య __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_వరుస = అబద్ధం;
    }

    Impl(cl_command_వరుస q)
    {
        __init();
        handle = q;

        cl_command_వరుస_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_వరుస_Info(handle, CL_వరుస_PROPERTIES, పరిమాణం(cl_command_వరుస_properties), &props, ఖాళీ));
        is_Profiling_వరుస = !!(props & CL_వరుస_PROFILING_ENABLE);
    }

    Impl(cl_command_వరుస q, తార్కిక is_Profiling_వరుస_2)
    {
        __init();
        handle = q;
        is_Profiling_వరుస = is_Profiling_వరుస_2;
    }

    Impl(స్థిరరాశి Context& c, స్థిరరాశి Device& d, తార్కిక withProfiling = అబద్ధం)
    {
        __init();

        స్థిరరాశి Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        ఒకవేళ( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        ఒకవేళ( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_వరుస_properties props = withProfiling ? CL_వరుస_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_వరుస(ch, dh, props, &retval), retval);
        is_Profiling_వరుస = withProfiling;
    }

    ~Impl()
    {
#నిర్వచితమైతే _WIN32
        ఒకవేళ (!cv::__termination)
#అయితే_ముగింపు
        {
            ఒకవేళ(handle)
            {
                CV_OCL_DBG_CHECK(cl_ముగింపు(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_వరుస(handle));
                handle = ఖాళీ;
            }
        }
    }

    స్థిరరాశి cv::ocl::వరుస& get_Profiling_వరుస(స్థిరరాశి cv::ocl::వరుస& self)
    {
        ఒకవేళ (is_Profiling_వరుస)
            తిరిగి self;

        ఒకవేళ (profiling_వరుస.ptr())
            తిరిగి profiling_వరుస;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_వరుస_Info(handle, CL_వరుస_CONTEXT, పరిమాణం(cl_context), &ctx, ఖాళీ));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_వరుస_Info(handle, CL_వరుస_DEVICE, పరిమాణం(cl_device_id), &device, ఖాళీ));

        cl_int ఫలితం = CL_SUCCESS;
        cl_command_వరుస_properties props = CL_వరుస_PROFILING_ENABLE;
        cl_command_వరుస q = cl_Create_Command_వరుస(ctx, device, props, &ఫలితం);
        CV_OCL_DBG_CHECK_ఫలితం(ఫలితం, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        వరుస వరుస_2;
        వరుస_2.p = కొత్త Impl(q, నిజం);
        profiling_వరుస = వరుస_2;

        తిరిగి profiling_వరుస;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_వరుస handle;
    తార్కిక is_Profiling_వరుస;
    cv::ocl::వరుస profiling_వరుస;
};

వరుస::వరుస() CV_NOEXCEPT
{
    p = 0;
}

వరుస::వరుస(స్థిరరాశి Context& c, స్థిరరాశి Device& d)
{
    p = 0;
    create(c, d);
}

వరుస::వరుస(స్థిరరాశి వరుస& q)
{
    p = q.p;
    ఒకవేళ(p)
        p->addref();
}

వరుస& వరుస::క్రియాకారకం = (స్థిరరాశి వరుస& q)
{
    Impl* newp = (Impl*)q.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

వరుస::వరుస(వరుస&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = శూన్యసూచిక;
}

వరుస& వరుస::క్రియాకారకం = (వరుస&& q) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &q) {
        ఒకవేళ(p)
            p->release();
        p = q.p;
        q.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

వరుస::~వరుస()
{
    ఒకవేళ(p)
        p->release();
}

తార్కిక వరుస::create(స్థిరరాశి Context& c, స్థిరరాశి Device& d)
{
    ఒకవేళ(p)
        p->release();
    p = కొత్త Impl(c, d);
    తిరిగి p->handle != 0;
}

శూన్య వరుస::ముగింపు_2()
{
    ఒకవేళ(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_ముగింపు(p->handle));
    }
}

స్థిరరాశి వరుస& వరుస::get_Profiling_వరుస() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->get_Profiling_వరుస(*ఇది);
}

శూన్య* వరుస::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

వరుస& వరుస::getDefault()
{
    స్వయంచాలక& c = OpenCLExecutionContext::getCurrent();
    ఒకవేళ (!c.empty())
    {
        స్వయంచాలక& q = c.get_వరుస();
        తిరిగి q;
    }
    స్థిర వరుస dummy;
    తిరిగి dummy;
}

స్థిర cl_command_వరుస get_వరుస(స్థిరరాశి వరుస& q)
{
    cl_command_వరుస qq = (cl_command_వరుస)q.ptr();
    ఒకవేళ(!qq)
        qq = (cl_command_వరుస)వరుస::getDefault().ptr();
    తిరిగి qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(పూర్ణాంకం _flags, UMat* _m, పూర్ణాంకం _wscale, పూర్ణాంకం _iwscale, స్థిరరాశి శూన్య* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != ఖాళీ);
}

KernelArg KernelArg::Constant(స్థిరరాశి Mat& m)
{
    CV_Assert(m.isContinuous());
    తిరిగి KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.మొత్తం()*m.elem_కొలత());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

నిర్మాణం Kernel::Impl
{
    Impl(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_2& prog) :
        refcount(1), handle(ఖాళీ), is_లోపల_పురోగతి(అబద్ధం), is_Async_నిరంతరం(అబద్ధం), nu(0)
    {
        cl_కార్యక్రమం ph = (cl_కార్యక్రమం)prog.ptr();
        cl_int retval = 0;
        name = kname;
        ఒకవేళ (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        కొరకు( పూర్ణాంకం i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_తాత్కాలిక_Dst_UMats = అబద్ధం;
        have_తాత్కాలిక_Src_UMats = అబద్ధం;
    }

    శూన్య cleanupUMats()
    {
        తార్కిక exceptionOccurred = అబద్ధం;
        కొరకు( పూర్ణాంకం i = 0; i < MAX_ARRS; i++ )
        {
            ఒకవేళ( u[i] )
            {
                ఒకవేళ( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_దత్తాంశం::ASYNC_CLEANUP;
                    ప్రయత్నించు
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    పట్టుకో(స్థిరరాశి std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(ఖాళీ, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = నిజం;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_తాత్కాలిక_Dst_UMats = అబద్ధం;
        have_తాత్కాలిక_Src_UMats = అబద్ధం;
        CV_Assert(!exceptionOccurred);
    }

    శూన్య కూడు_UMat(స్థిరరాశి UMat& m, తార్కిక dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        ఒకవేళ(dst && m.u->తాత్కాలిక_UMat())
            have_తాత్కాలిక_Dst_UMats = నిజం;
        ఒకవేళ(m.u->original_UMat_దత్తాంశం == ఖాళీ && m.u->తాత్కాలిక_UMat())
            have_తాత్కాలిక_Src_UMats = నిజం;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    శూన్య రిజిస్టరు_Image_వాదం(పూర్ణాంకం arg, స్థిరరాశి Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        ఒకవేళ (arg < (పూర్ణాంకం)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_లోపల_పురోగతి, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    శూన్య finit(cl_event e)
    {
        CV_వాడని(e);
        is_లోపల_పురోగతి = అబద్ధం;
        ప్రయత్నించు
        {
            cleanupUMats();
        }
        పట్టుకో(...)
        {
            release();
            విసురు;
        }
        release();
    }

    తార్కిక నిరంతరం(పూర్ణాంకం dims, size_t _globalsize[], size_t _localsize[],
            తార్కిక సమకాలీకరణ, int64* సమయం_NS, స్థిరరాశి వరుస& q);

    ~Impl()
    {
        ఒకవేళ(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    గణన { MAX_ARRS = 16 };
    UMat_దత్తాంశం* u[MAX_ARRS];
    తార్కిక is_లోపల_పురోగతి;
    తార్కిక is_Async_నిరంతరం;  // true if kernel was scheduled in async mode
    పూర్ణాంకం nu;
    std::vector<Image2D> shadow_images;
    తార్కిక have_తాత్కాలిక_Dst_UMats;
    తార్కిక have_తాత్కాలిక_Src_UMats;
};

}} // namespace cv::ocl

బాహ్య "C" {

స్థిర శూన్య CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, శూన్య *p)
{
    ప్రయత్నించు
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    పట్టుకో (స్థిరరాశి cv::Exception& exc)
    {
        CV_LOG_ERROR(ఖాళీ, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    పట్టుకో (స్థిరరాశి std::exception& exc)
    {
        CV_LOG_ERROR(ఖాళీ, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    పట్టుకో (...)
    {
        CV_LOG_ERROR(ఖాళీ, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

నామాకాశ cv { నామాకాశ ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_ఆధారం& src,
               స్థిరరాశి String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(స్థిరరాశి Kernel& k)
{
    p = k.p;
    ఒకవేళ(p)
        p->addref();
}

Kernel& Kernel::క్రియాకారకం = (స్థిరరాశి Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = శూన్యసూచిక;
}

Kernel& Kernel::క్రియాకారకం = (Kernel&& k) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &k) {
        ఒకవేళ(p)
            p->release();
        p = k.p;
        k.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

Kernel::~Kernel()
{
    ఒకవేళ(p)
        p->release();
}

తార్కిక Kernel::create(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_2& prog)
{
    ఒకవేళ(p)
        p->release();
    p = కొత్త Impl(kname, prog);
    ఒకవేళ(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#నిర్వచితమైతే CV_OPENCL_నిరంతరం_ASSERT // check kernel compilation fails
    CV_Assert(p);
#అయితే_ముగింపు
    తిరిగి p != 0;
}

తార్కిక Kernel::create(స్థిరరాశి అక్షరం* kname, స్థిరరాశి కార్యక్రమం_ఆధారం& src,
                    స్థిరరాశి String& buildopts, String* errmsg)
{
    ఒకవేళ(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    ఒకవేళ( !errmsg ) errmsg = &tempmsg;
    స్థిరరాశి కార్యక్రమం_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    తిరిగి create(kname, prog);
}

శూన్య* Kernel::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

తార్కిక Kernel::empty() స్థిరరాశి
{
    తిరిగి ptr() == 0;
}

స్థిర cv::String dump_విలువ(size_t sz, స్థిరరాశి శూన్య* p)
{
    ఒకవేళ (!p)
        తిరిగి "NULL";
    ఒకవేళ (sz == 2)
        తిరిగి cv::format("%d / %uu / 0x%04x", *(పొట్టి*)p, *(గుర్తులేని పొట్టి*)p, *(పొట్టి*)p);
    ఒకవేళ (sz == 4)
        తిరిగి cv::format("%d / %uu / 0x%08x / %g", *(పూర్ణాంకం*)p, *(పూర్ణాంకం*)p, *(పూర్ణాంకం*)p, *(దశాంశం*)p);
    ఒకవేళ (sz == 8)
        తిరిగి cv::format("%lld / %lluu / 0x%16llx / %g", *(దీర్ఘ దీర్ఘ*)p, *(దీర్ఘ దీర్ఘ*)p, *(దీర్ఘ దీర్ఘ*)p, *(ద్విగుణ*)p);
    తిరిగి cv::format("%p", p);
}

పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి శూన్య* value, size_t sz)
{
    ఒకవేళ (!p || !p->handle)
        తిరిగి -1;
    ఒకవేళ (i < 0)
        తిరిగి i;
    ఒకవేళ( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (పూర్ణాంకం)i, (పూర్ణాంకం)sz, dump_విలువ(sz, value).c_str()).c_str());
    ఒకవేళ (retval != CL_SUCCESS)
        తిరిగి -1;
    తిరిగి i+1;
}

పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    పూర్ణాంకం res = సమితి(i, &h, పరిమాణం(h));
    ఒకవేళ (res >= 0)
        p->రిజిస్టరు_Image_వాదం(i, image2D);
    తిరిగి res;
}

పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి UMat& m)
{
    తిరిగి సమితి(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
స్థిర తార్కిక isCudaBacked(స్థిరరాశి UMat_దత్తాంశం* u)
{
    స్థిరరాశి MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    తిరిగి u && cudaAllocator && u->currAllocator == cudaAllocator;
}

పూర్ణాంకం Kernel::సమితి(పూర్ణాంకం i, స్థిరరాశి KernelArg& arg)
{
    ఒకవేళ( !p || !p->handle )
        తిరిగి -1;
    ఒకవేళ (i < 0)
    {
        CV_LOG_ERROR(ఖాళీ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (పూర్ణాంకం)i));
        తిరిగి i;
    }
    ఒకవేళ( i == 0 )
        p->cleanupUMats();
    cl_int స్థితి_2 = 0;
    ఒకవేళ( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        తార్కిక ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        ఒకవేళ (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)ఖాళీ;
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, పరిమాణం(h_null), &h_null);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (పూర్ణాంకం)i).c_str());
            తిరిగి i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        ఒకవేళ (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(ఖాళీ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (పూర్ణాంకం)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            తిరిగి -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        ఒకవేళ (!h)
        {
            CV_LOG_ERROR(ఖాళీ, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (పూర్ణాంకం)i, (పూర్ణాంకం)arg.flags, arg.m));
            p->release();
            p = 0;
            తిరిగి -1;
        }

#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
        {
            స్థిరరాశి Context& ctx = Context::getDefault();
            స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_దత్తాంశం_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_దత్తాంశం_Ptr);
#ఒకవేళ 1 // TODO
            స్థితి_2 = svmFns->fn_cl_సమితి_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_దత్తాంశం_Ptr);
#లేకపోతే
            స్థితి_2 = svmFns->fn_cl_సమితి_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_దత్తాంశం_Ptr);
#అయితే_ముగింపు
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (పూర్ణాంకం)i, (శూన్య*)svm_దత్తాంశం_Ptr).c_str());
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, పరిమాణం(h), &h);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (పూర్ణాంకం)i, (శూన్య*)h).c_str());
        }

        ఒకవేళ (ptronly)
        {
            i++;
        }
        లేకపోతే ఒకవేళ( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+1), పరిమాణం(u2d.అడుగు), &u2d.అడుగు);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+1), (పూర్ణాంకం)u2d.అడుగు).c_str());
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+2), పరిమాణం(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+2), (పూర్ణాంకం)u2d.offset).c_str());
            i += 3;

            ఒకవేళ( !(arg.flags & KernelArg::NO_కొలత) )
            {
                పూర్ణాంకం నిలువుల_సంఖ్య = u2d.నిలువుల_సంఖ్య*arg.wscale/arg.iwscale;
                స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, పరిమాణం(u2d.అడ్డువరుసలు), &u2d.అడ్డువరుసలు);
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (పూర్ణాంకం)i, (పూర్ణాంకం)u2d.అడ్డువరుసలు).c_str());
                స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+1), పరిమాణం(నిలువుల_సంఖ్య), &నిలువుల_సంఖ్య);
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+1), (పూర్ణాంకం)నిలువుల_సంఖ్య).c_str());
                i += 2;
            }
        }
        లేకపోతే
        {
            UMat3D u3d(*arg.m);
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+1), పరిమాణం(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+1), (పూర్ణాంకం)u3d.slicestep).c_str());
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+2), పరిమాణం(u3d.అడుగు), &u3d.అడుగు);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+2), (పూర్ణాంకం)u3d.అడుగు).c_str());
            స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+3), పరిమాణం(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+3), (పూర్ణాంకం)u3d.offset).c_str());
            i += 4;
            ఒకవేళ( !(arg.flags & KernelArg::NO_కొలత) )
            {
                పూర్ణాంకం నిలువుల_సంఖ్య = u3d.నిలువుల_సంఖ్య*arg.wscale/arg.iwscale;
                స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, పరిమాణం(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (పూర్ణాంకం)i, (పూర్ణాంకం)u3d.slices).c_str());
                స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+1), పరిమాణం(u3d.అడ్డువరుసలు), &u3d.అడ్డువరుసలు);
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+1), (పూర్ణాంకం)u3d.అడ్డువరుసలు).c_str());
                స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)(i+2), పరిమాణం(u3d.నిలువుల_సంఖ్య), &నిలువుల_సంఖ్య);
                CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (పూర్ణాంకం)(i+2), (పూర్ణాంకం)నిలువుల_సంఖ్య).c_str());
                i += 3;
            }
        }
        p->కూడు_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        తిరిగి i;
    }
    స్థితి_2 = cl_సమితి_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (పూర్ణాంకం)i, (పూర్ణాంకం)arg.sz, (శూన్య*)arg.obj).c_str());
    తిరిగి i+1;
}

తార్కిక Kernel::నిరంతరం(పూర్ణాంకం dims, size_t _globalsize[], size_t _localsize[],
                 తార్కిక సమకాలీకరణ, స్థిరరాశి వరుస& q)
{
    ఒకవేళ (!p)
        తిరిగి అబద్ధం;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t మొత్తం = 1;
    CV_Assert(_globalsize != ఖాళీ);
    కొరకు (పూర్ణాంకం i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(పూర్ణాంకం)(i>0)) : 1;
        CV_Assert( val > 0 );
        మొత్తం *= _globalsize[i];
        ఒకవేళ (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_పైకి(_globalsize[i], (గుర్తులేని పూర్ణాంకం)val) * val;
    }
    CV_Assert(మొత్తం > 0);

    తిరిగి p->నిరంతరం(dims, globalsize, _localsize, సమకాలీకరణ, ఖాళీ, q);
}


తార్కిక Kernel::నిరంతరం_2(పూర్ణాంకం dims, size_t _globalsize[], size_t _localsize[],
                  తార్కిక సమకాలీకరణ, స్థిరరాశి వరుస& q)
{
    CV_Assert(p);
    తిరిగి p->నిరంతరం(dims, _globalsize, _localsize, సమకాలీకరణ, ఖాళీ, q);
}


తార్కిక Kernel::Impl::నిరంతరం(పూర్ణాంకం dims, size_t globalsize[], size_t localsize[],
        తార్కిక సమకాలీకరణ, int64* సమయం_NS, స్థిరరాశి వరుస& q)
{
    CV_INSTRUMENT_REGION_OPENCL_నిరంతరం(name.c_str());

    ఒకవేళ (!handle)
    {
        CV_LOG_ERROR(ఖాళీ, "OpenCL kernel has zero handle: " << name);
        తిరిగి అబద్ధం;
    }

    ఒకవేళ (is_Async_నిరంతరం)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_నిరంతరం = !సమకాలీకరణ;

    ఒకవేళ (is_లోపల_పురోగతి)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#ఒకవేళ CV_OPENCL_SYNC_నిరంతరం_KERNELS
    సమకాలీకరణ = నిజం;
#అయితే_ముగింపు

    cl_command_వరుస qq = get_వరుస(q);
    ఒకవేళ (have_తాత్కాలిక_Dst_UMats)
        సమకాలీకరణ = నిజం;
    ఒకవేళ (have_తాత్కాలిక_Src_UMats)
        సమకాలీకరణ = నిజం;
    ఒకవేళ (సమయం_NS)
        సమకాలీకరణ = నిజం;
    cl_event asyncEvent = 0;
    cl_int retval = cl_వరుసలో_చేర్చు_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           ఖాళీ, globalsize, localsize, 0, 0,
                                           (సమకాలీకరణ && !సమయం_NS) ? 0 : &asyncEvent);
#ఒకవేళ !CV_OPENCL_SHOW_నిరంతరం_KERNELS
    ఒకవేళ (retval != CL_SUCCESS)
#అయితే_ముగింపు
    {
        cv::String సందేశం = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (పూర్ణాంకం)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        సమకాలీకరణ ? "true" : "false"
                        );
        ఒకవేళ (retval != CL_SUCCESS)
        {
            సందేశం = CV_OCL_API_ERROR_సందేశం(retval, సందేశం.c_str());
        }
#ఒకవేళ CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_ఫలితం(retval, సందేశం.c_str());
#లేకపోతే
        రాయి("%s\n", సందేశం.c_str());
        fflush(stdout);
#అయితే_ముగింపు
    }
    ఒకవేళ (సమకాలీకరణ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ముగింపు(qq));
        ఒకవేళ (సమయం_NS)
        {
            ఒకవేళ (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong ప్రారంభం_సమయం, stop_సమయం;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_ప్రారంభం, పరిమాణం(ప్రారంభం_సమయం), &ప్రారంభం_సమయం, ఖాళీ));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_చివర, పరిమాణం(stop_సమయం), &stop_సమయం, ఖాళీ));
                *సమయం_NS = (int64)(stop_సమయం - ప్రారంభం_సమయం);
            }
            లేకపోతే
            {
                *సమయం_NS = -1;
            }
        }
        cleanupUMats();
    }
    లేకపోతే
    {
        addref();
        is_లోపల_పురోగతి = నిజం;
        CV_OCL_CHECK(cl_సమితి_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, ఇది));
    }
    ఒకవేళ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    తిరిగి retval == CL_SUCCESS;
}

తార్కిక Kernel::నిరంతరం_Task(తార్కిక సమకాలీకరణ, స్థిరరాశి వరుస& q)
{
    ఒకవేళ(!p || !p->handle || p->is_లోపల_పురోగతి)
        తిరిగి అబద్ధం;

    cl_command_వరుస qq = get_వరుస(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_వరుసలో_చేర్చు_Task(qq, p->handle, 0, 0, సమకాలీకరణ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), సమకాలీకరణ ? "true" : "false").c_str());
    ఒకవేళ (సమకాలీకరణ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_ముగింపు(qq));
        p->cleanupUMats();
    }
    లేకపోతే
    {
        p->addref();
        p->is_లోపల_పురోగతి = నిజం;
        CV_OCL_CHECK(cl_సమితి_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    ఒకవేళ (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    తిరిగి retval == CL_SUCCESS;
}

int64 Kernel::నిరంతరం_Profiling(పూర్ణాంకం dims, size_t globalsize[], size_t localsize[], స్థిరరాశి వరుస& q_)
{
    CV_Assert(p && p->handle && !p->is_లోపల_పురోగతి);
    వరుస q = q_.ptr() ? q_ : వరుస::getDefault();
    CV_Assert(q.ptr());
    q.ముగింపు_2(); // call clFinish() on base queue
    వరుస profiling_వరుస_2 = q.get_Profiling_వరుస();
    int64 సమయం_Ns = -1;
    తార్కిక res = p->నిరంతరం(dims, globalsize, localsize, నిజం, &సమయం_Ns, profiling_వరుస_2);
    తిరిగి res ? సమయం_Ns : -1;
}

size_t Kernel::work_సమూహం_కొలత() స్థిరరాశి
{
    ఒకవేళ(!p || !p->handle)
        తిరిగి 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int స్థితి_2 = cl_Get_Kernel_Work_సమూహం_Info(p->handle, dev, CL_KERNEL_WORK_సమూహం_కొలత, పరిమాణం(val), &val, &retsz);
    CV_OCL_CHECK_ఫలితం(స్థితి_2, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    తిరిగి స్థితి_2 == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_సమూహం_కొలత_గుణిజం() స్థిరరాశి
{
    ఒకవేళ(!p || !p->handle)
        తిరిగి 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int స్థితి_2 = cl_Get_Kernel_Work_సమూహం_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_సమూహం_కొలత_గుణిజం, పరిమాణం(val), &val, &retsz);
    CV_OCL_CHECK_ఫలితం(స్థితి_2, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    తిరిగి స్థితి_2 == CL_SUCCESS ? val : 0;
}

తార్కిక Kernel::compile_Work_సమూహం_కొలత(size_t wsz[]) స్థిరరాశి
{
    ఒకవేళ(!p || !p->handle || !wsz)
        తిరిగి 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int స్థితి_2 = cl_Get_Kernel_Work_సమూహం_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_సమూహం_కొలత, పరిమాణం(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_ఫలితం(స్థితి_2, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    తిరిగి స్థితి_2 == CL_SUCCESS;
}

size_t Kernel::local_Mem_కొలత() స్థిరరాశి
{
    ఒకవేళ(!p || !p->handle)
        తిరిగి 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int స్థితి_2 = cl_Get_Kernel_Work_సమూహం_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_కొలత, పరిమాణం(val), &val, &retsz);
    CV_OCL_CHECK_ఫలితం(స్థితి_2, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    తిరిగి స్థితి_2 == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

నిర్మాణం కార్యక్రమం_ఆధారం::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    గణన రకం_4 {
        కార్యక్రమం_ఆధారం_సంకేతం = 0,
        కార్యక్రమం_BINARIES,
        కార్యక్రమం_SPIR,
        కార్యక్రమం_SPIRV
    } రకం_5;

    Impl(స్థిరరాశి String& src)
    {
        init(కార్యక్రమం_ఆధారం_సంకేతం, cv::String(), cv::String());
        init_From_ఆధారం(src, cv::String());
    }
    Impl(స్థిరరాశి String& module, స్థిరరాశి String& name, స్థిరరాశి String& సంకేతం_Str, స్థిరరాశి String& సంకేతం_Hash)
    {
        init(కార్యక్రమం_ఆధారం_సంకేతం, module, name);
        init_From_ఆధారం(సంకేతం_Str, సంకేతం_Hash);
    }

    /// reset fields
    శూన్య init(గణన రకం_4 రకం, స్థిరరాశి String& module, స్థిరరాశి String& name)
    {
        refcount = 1;
        రకం_5 = రకం;
        module_ = module;
        name_ = name;

        ఆధారం_Addr = ఖాళీ;
        ఆధారం_కొలత = 0;
        isHashUpdated = అబద్ధం;
    }

    శూన్య init_From_ఆధారం(స్థిరరాశి String& సంకేతం_Str, స్థిరరాశి String& సంకేతం_Hash)
    {
        సంకేతం_Str_2 = సంకేతం_Str;
        ఆధారం_Hash = సంకేతం_Hash;
        ఒకవేళ (ఆధారం_Hash.empty())
        {
            updateHash();
        }
        లేకపోతే
        {
            isHashUpdated = నిజం;
        }
    }

    శూన్య updateHash(స్థిరరాశి అక్షరం* hashStr = ఖాళీ)
    {
        ఒకవేళ (hashStr)
        {
            ఆధారం_Hash = cv::String(hashStr);
            isHashUpdated = నిజం;
            తిరిగి;
        }
        uint64 hash = 0;
        ఎంపిక (రకం_5)
        {
        స్థితి కార్యక్రమం_ఆధారం_సంకేతం:
            ఒకవేళ (ఆధారం_Addr)
            {
                CV_Assert(సంకేతం_Str_2.empty());
                hash = crc64(ఆధారం_Addr, ఆధారం_కొలత); // static storage
            }
            లేకపోతే
            {
                CV_Assert(!సంకేతం_Str_2.empty());
                hash = crc64((uchar*)సంకేతం_Str_2.c_str(), సంకేతం_Str_2.size());
            }
            ఆపు;
        స్థితి కార్యక్రమం_BINARIES:
        స్థితి కార్యక్రమం_SPIR:
        స్థితి కార్యక్రమం_SPIRV:
            hash = crc64(ఆధారం_Addr, ఆధారం_కొలత);
            ఆపు;
        అప్రమేయ:
            CV_Error(Error::Sts_అంతర్గత, "Internal error");
        }
        ఆధారం_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = నిజం;
    }

    Impl(గణన రకం_4 రకం,
            స్థిరరాశి String& module, స్థిరరాశి String& name,
            స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size,
            స్థిరరాశి cv::String& buildOptions = cv::String())
    {
        init(రకం, module, name);

        ఆధారం_Addr = binary;
        ఆధారం_కొలత = size;

        buildOptions_ = buildOptions;
    }

    స్థిర కార్యక్రమం_ఆధారం from_ఆధారం_With_Static_Lifetime(స్థిరరాశి String& module, స్థిరరాశి String& name,
            స్థిరరాశి అక్షరం* ఆధారం_సంకేతం_Static_Str, స్థిరరాశి అక్షరం* hashStaticStr,
            స్థిరరాశి cv::String& buildOptions)
    {
        కార్యక్రమం_ఆధారం ఫలితం;
        ఫలితం.p = కొత్త Impl(కార్యక్రమం_ఆధారం_సంకేతం, module, name,
                (స్థిరరాశి గుర్తులేని అక్షరం*)ఆధారం_సంకేతం_Static_Str, అక్షరశ్రేణి_పొడవు(ఆధారం_సంకేతం_Static_Str), buildOptions);
        ఫలితం.p->updateHash(hashStaticStr);
        తిరిగి ఫలితం;
    }

    స్థిర కార్యక్రమం_ఆధారం from_ద్విమాన(స్థిరరాశి String& module, స్థిరరాశి String& name,
            స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size,
            స్థిరరాశి cv::String& buildOptions)
    {
        కార్యక్రమం_ఆధారం ఫలితం;
        ఫలితం.p = కొత్త Impl(కార్యక్రమం_BINARIES, module, name, binary, size, buildOptions);
        తిరిగి ఫలితం;
    }

    స్థిర కార్యక్రమం_ఆధారం fromSPIR(స్థిరరాశి String& module, స్థిరరాశి String& name,
            స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size,
            స్థిరరాశి cv::String& buildOptions)
    {
        కార్యక్రమం_ఆధారం ఫలితం;
        ఫలితం.p = కొత్త Impl(కార్యక్రమం_SPIR, module, name, binary, size, buildOptions);
        తిరిగి ఫలితం;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String సంకేతం_Str_2; // PROGRAM_SOURCE_CODE only

    స్థిరరాశి గుర్తులేని అక్షరం* ఆధారం_Addr;
    size_t ఆధారం_కొలత;

    cv::String buildOptions_;

    String ఆధారం_Hash;
    తార్కిక isHashUpdated;

    మిత్ర నిర్మాణం కార్యక్రమం_2::Impl;
    మిత్ర నిర్మాణం internal::కార్యక్రమం_నమోదు;
    మిత్ర నిర్మాణం Context::Impl;
};


కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం() CV_NOEXCEPT
{
    p = 0;
}

కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి String& module, స్థిరరాశి String& name, స్థిరరాశి String& సంకేతం_Str, స్థిరరాశి String& సంకేతం_Hash)
{
    p = కొత్త Impl(module, name, సంకేతం_Str, సంకేతం_Hash);
}

కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి అక్షరం* prog)
{
    p = కొత్త Impl(prog);
}

కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి String& prog)
{
    p = కొత్త Impl(prog);
}

కార్యక్రమం_ఆధారం::~కార్యక్రమం_ఆధారం()
{
    ఒకవేళ(p)
        p->release();
}

కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(స్థిరరాశి కార్యక్రమం_ఆధారం& prog)
{
    p = prog.p;
    ఒకవేళ(p)
        p->addref();
}

కార్యక్రమం_ఆధారం& కార్యక్రమం_ఆధారం::క్రియాకారకం = (స్థిరరాశి కార్యక్రమం_ఆధారం& prog)
{
    Impl* newp = (Impl*)prog.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

కార్యక్రమం_ఆధారం::కార్యక్రమం_ఆధారం(కార్యక్రమం_ఆధారం&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = శూన్యసూచిక;
}

కార్యక్రమం_ఆధారం& కార్యక్రమం_ఆధారం::క్రియాకారకం = (కార్యక్రమం_ఆధారం&& prog) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &prog) {
        ఒకవేళ(p)
            p->release();
        p = prog.p;
        prog.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

స్థిరరాశి String& కార్యక్రమం_ఆధారం::ఆధారం() స్థిరరాశి
{
    CV_Assert(p);
    CV_Assert(p->రకం_5 == Impl::కార్యక్రమం_ఆధారం_సంకేతం);
    CV_Assert(p->ఆధారం_Addr == ఖాళీ); // method returns reference - can't construct temporary object
    తిరిగి p->సంకేతం_Str_2;
}

కార్యక్రమం_ఆధారం::hash_t కార్యక్రమం_ఆధారం::hash() స్థిరరాశి
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

కార్యక్రమం_ఆధారం కార్యక్రమం_ఆధారం::from_ద్విమాన(స్థిరరాశి String& module, స్థిరరాశి String& name,
        స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size,
        స్థిరరాశి cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    తిరిగి Impl::from_ద్విమాన(module, name, binary, size, buildOptions);
}

కార్యక్రమం_ఆధారం కార్యక్రమం_ఆధారం::fromSPIR(స్థిరరాశి String& module, స్థిరరాశి String& name,
        స్థిరరాశి గుర్తులేని అక్షరం* binary, స్థిరరాశి size_t size,
        స్థిరరాశి cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    తిరిగి Impl::from_ద్విమాన(module, name, binary, size, buildOptions);
}


internal::కార్యక్రమం_నమోదు::క్రియాకారకం కార్యక్రమం_ఆధారం&() స్థిరరాశి
{
    ఒకవేళ (ఇది->p_కార్యక్రమం_ఆధారం == ఖాళీ)
    {
        cv::Auto_తాళం lock(cv::getInitializationMutex());
        ఒకవేళ (ఇది->p_కార్యక్రమం_ఆధారం == ఖాళీ)
        {
            కార్యక్రమం_ఆధారం ps = కార్యక్రమం_ఆధారం::Impl::from_ఆధారం_With_Static_Lifetime(ఇది->module, ఇది->name, ఇది->కార్యక్రమం_సంకేతం, ఇది->కార్యక్రమం_Hash, cv::String());
            కార్యక్రమం_ఆధారం* ptr = కొత్త కార్యక్రమం_ఆధారం(ps);
            const_cast<కార్యక్రమం_నమోదు*>(ఇది)->p_కార్యక్రమం_ఆధారం = ptr;
        }
    }
    తిరిగి *ఇది->p_కార్యక్రమం_ఆధారం;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

స్థిర
cv::String joinBuildOptions(స్థిరరాశి cv::String& a, స్థిరరాశి cv::String& b)
{
    ఒకవేళ (b.empty())
        తిరిగి a;
    ఒకవేళ (a.empty())
        తిరిగి b;
    ఒకవేళ (b[0] == ' ')
        తిరిగి a + b;
    తిరిగి a + (cv::String(" ") + b);
}

నిర్మాణం కార్యక్రమం_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(స్థిరరాశి కార్యక్రమం_ఆధారం& src,
         స్థిరరాశి String& _buildflags, String& errmsg) :
         refcount(1),
         handle(ఖాళీ),
         buildflags(_buildflags)
    {
        స్థిరరాశి కార్యక్రమం_ఆధారం::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        ఆధారం_Module = src_->module_;
        ఆధారం_Name = src_->name_;
        స్థిరరాశి Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        ఒకవేళ (ctx.ptr() == ఖాళీ || device.ptr() == ఖాళీ)
            తిరిగి;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        ఒకవేళ (src.getImpl()->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_ఆధారం_సంకేతం)
        {
            ఒకవేళ (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            లేకపోతే ఒకవేళ (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            స్థిరరాశి String param_buildExtraOptions = getBuildExtraOptions();
            ఒకవేళ (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#ఒకవేళ CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(ఖాళీ, "OpenCL program '" << ఆధారం_Module << "/" << ఆధారం_Name << "' options:" << buildflags);
#అయితే_ముగింపు
        compile(ctx, src_, errmsg);
#ఒకవేళ CV_OPENCL_SHOW_BUILD_KERNELS
        ఒకవేళ (handle)
        {
            size_t retsz = 0;
            అక్షరం kernels_బఫరు[4096] = {0};
            cl_int ఫలితం = cl_Get_కార్యక్రమం_Info(handle, CL_కార్యక్రమం_KERNEL_పేర్లు, పరిమాణం(kernels_బఫరు), &kernels_బఫరు[0], &retsz);
            CV_OCL_DBG_CHECK_ఫలితం(ఫలితం, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", ఆధారం_Module.c_str(), ఆధారం_Name.c_str()).c_str());
            ఒకవేళ (ఫలితం == CL_SUCCESS && retsz < పరిమాణం(kernels_బఫరు))
            {
                kernels_బఫరు[retsz] = 0;
                CV_LOG_INFO(ఖాళీ, "OpenCL program '" << ఆధారం_Module << "/" << ఆధారం_Name << "' kernels: '" << kernels_బఫరు << "'");
            }
            లేకపోతే
            {
                CV_LOG_ERROR(ఖాళీ, "OpenCL program '" << ఆధారం_Module << "/" << ఆధారం_Name << "' can't retrieve kernel names!");
            }
        }
#అయితే_ముగింపు
    }

    తార్కిక compile(స్థిరరాశి Context& ctx, స్థిరరాశి కార్యక్రమం_ఆధారం::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        ఒకవేళ (src_->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_BINARIES)
        {
            CV_LOG_VERBOSE(ఖాళీ, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            తార్కిక isLoaded = create_From_ద్విమాన(ctx, src_->ఆధారం_Addr, src_->ఆధారం_కొలత, errmsg);
            తిరిగి isLoaded;
        }
        తిరిగి compileWithCache(ctx, src_, errmsg);
    }

    తార్కిక compileWithCache(స్థిరరాశి Context& ctx, స్థిరరాశి కార్యక్రమం_ఆధారం::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->రకం_5 != కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_BINARIES);

#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        స్థిరరాశి std::string పునాది_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_ఉపసర్గ_String(),
                ctx.getImpl()->get_ఉపసర్గ_పునాది()
        );
        స్థిరరాశి String& hash_str = src_->ఆధారం_Hash;
        cv::String fname;
        ఒకవేళ (!పునాది_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(పునాది_dir, fname);
        }
        స్థిరరాశి cv::Ptr<utils::fs::File_తాళం> file_తాళం = config.cache_తాళం; // can be empty
        ఒకవేళ (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            ప్రయత్నించు
            {
                std::vector<అక్షరం> ద్విమాన_Buf;
                తార్కిక res = అబద్ధం;
                {
                    cv::utils::optional_shared_తాళం_guard<cv::utils::fs::File_తాళం> తాళం_fs(file_తాళం.get());
                    ద్విమాన_కార్యక్రమం_File file(fname, hash_str.c_str());
                    res = file.పఠ(buildflags, ద్విమాన_Buf);
                }
                ఒకవేళ (res)
                {
                    CV_Assert(!ద్విమాన_Buf.empty());
                    CV_LOG_VERBOSE(ఖాళీ, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    తార్కిక isLoaded = create_From_ద్విమాన(ctx, ద్విమాన_Buf, errmsg);
                    ఒకవేళ (isLoaded)
                        తిరిగి నిజం;
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                CV_వాడని(e);
                CV_LOG_VERBOSE(ఖాళీ, 0, "Can't load OpenCL binary: " + fname << std::పంక్తి_ముగింపు << e.what());
            }
            పట్టుకో (...)
            {
                CV_LOG_VERBOSE(ఖాళీ, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == ఖాళీ);
        ఒకవేళ (src_->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_ఆధారం_సంకేతం)
        {
            ఒకవేళ (!buildFromSources(ctx, src_, errmsg))
            {
                తిరిగి అబద్ధం;
            }
        }
        లేకపోతే ఒకవేళ (src_->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            ఒకవేళ ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(ఖాళీ, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            తార్కిక isLoaded = create_From_ద్విమాన(ctx, src_->ఆధారం_Addr, src_->ఆధారం_కొలత, errmsg);
            ఒకవేళ (!isLoaded)
                తిరిగి అబద్ధం;
        }
        లేకపోతే ఒకవేళ (src_->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        లేకపోతే
        {
            CV_Error(Error::Sts_అంతర్గత, "Internal error");
        }
        CV_Assert(handle != ఖాళీ);
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
        ఒకవేళ (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            ప్రయత్నించు
            {
                std::vector<అక్షరం> ద్విమాన_Buf;
                get_కార్యక్రమం_ద్విమాన(ద్విమాన_Buf);
                {
                    cv::utils::optional_తాళం_guard<cv::utils::fs::File_తాళం> తాళం_fs(file_తాళం.get());
                    ద్విమాన_కార్యక్రమం_File file(fname, hash_str.c_str());
                    file.లిఖ(buildflags, ద్విమాన_Buf);
                }
            }
            పట్టుకో (స్థిరరాశి cv::Exception& e)
            {
                CV_LOG_WARNING(ఖాళీ, "Can't save OpenCL binary into cache: " + fname << std::పంక్తి_ముగింపు << e.what());
            }
            పట్టుకో (...)
            {
                CV_LOG_WARNING(ఖాళీ, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
#ఒకవేళ CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS
        ఒకవేళ (CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS_విలువ)
        {
            std::vector<అక్షరం> ద్విమాన_Buf;
            get_కార్యక్రమం_ద్విమాన(ద్విమాన_Buf);
            ఒకవేళ (!ద్విమాన_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_కార్యక్రమం(handle));
                handle = ఖాళీ;
                create_From_ద్విమాన(ctx, ద్విమాన_Buf, errmsg);
            }
        }
#అయితే_ముగింపు
        తిరిగి handle != ఖాళీ;
    }

    శూన్య dumpBuildLog_(cl_int ఫలితం, స్థిరరాశి cl_device_id* deviceList, String& errmsg)
    {
        Auto_బఫరు<అక్షరం, 4096> బఫరు; బఫరు[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_కార్యక్రమం_Build_Info(handle, deviceList[0],
                                                  CL_కార్యక్రమం_BUILD_LOG, 0, 0, &retsz);
        ఒకవేళ (log_retval == CL_SUCCESS && retsz > 1)
        {
            బఫరు.resize(retsz + 16);
            log_retval = cl_Get_కార్యక్రమం_Build_Info(handle, deviceList[0],
                                               CL_కార్యక్రమం_BUILD_LOG, retsz+1, బఫరు.దత్తాంశం(), &retsz);
            ఒకవేళ (log_retval == CL_SUCCESS)
            {
                ఒకవేళ (retsz < బఫరు.size())
                    బఫరు[retsz] = 0;
                లేకపోతే
                    బఫరు[బఫరు.size() - 1] = 0;
            }
            లేకపోతే
            {
                బఫరు[0] = 0;
            }
        }

        errmsg = String(బఫరు.దత్తాంశం());
        రాయి("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                ఆధారం_Module.c_str(), ఆధారం_Name.c_str(),
                ఫలితం, getOpenCLErrorString(ఫలితం),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    తార్కిక buildFromSources(స్థిరరాశి Context& ctx, స్థిరరాశి కార్యక్రమం_ఆధారం::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->రకం_5 == కార్యక్రమం_ఆధారం::Impl::కార్యక్రమం_ఆధారం_సంకేతం);
        CV_Assert(handle == ఖాళీ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                ఆధారం_Module.c_str(), ఆధారం_Name.c_str(),
                src_->ఆధారం_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(ఖాళీ, 0, "Compile... " << ఆధారం_Module.c_str() << "/" << ఆధారం_Name.c_str());

        స్థిరరాశి అక్షరం* srcptr = src_->ఆధారం_Addr ? ((స్థిరరాశి అక్షరం*)src_->ఆధారం_Addr) : src_->సంకేతం_Str_2.c_str();
        size_t srclen = src_->ఆధారం_Addr ? src_->ఆధారం_కొలత : src_->సంకేతం_Str_2.size();
        CV_Assert(srcptr != ఖాళీ);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_కార్యక్రమం_With_ఆధారం((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_ఫలితం(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        ఒకవేళ (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_బఫరు<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.దత్తాంశం();
            కొరకు (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_కార్యక్రమం(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_ఫలితం(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#ఒకవేళ !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            ఒకవేళ (retval != CL_SUCCESS)
#అయితే_ముగింపు
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                ఒకవేళ (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_కార్యక్రమం(handle));
                    handle = ఖాళీ;
                }
                ఒకవేళ (retval != CL_SUCCESS &&
                    ఆధారం_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#ఒకవేళ CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS
            ఒకవేళ (handle && CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS_విలువ)
            {
                CV_LOG_INFO(ఖాళీ, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                అక్షరం kernels_బఫరు[4096] = {0};
                cl_int ఫలితం = cl_Get_కార్యక్రమం_Info(handle, CL_కార్యక్రమం_KERNEL_పేర్లు, పరిమాణం(kernels_బఫరు), &kernels_బఫరు[0], &retsz);
                ఒకవేళ (retsz < పరిమాణం(kernels_బఫరు))
                    kernels_బఫరు[retsz] = 0;
                లేకపోతే
                    kernels_బఫరు[0] = 0;
                CV_LOG_INFO(ఖాళీ, ఫలితం << ": Kernels='" << kernels_బఫరు << "'");
            }
#అయితే_ముగింపు
        }
        తిరిగి handle != ఖాళీ;
    }

    శూన్య get_కార్యక్రమం_ద్విమాన(std::vector<అక్షరం>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_కార్యక్రమం_Info(handle, CL_కార్యక్రమం_ద్విమాన_SIZES, పరిమాణం(sz), &sz, ఖాళీ));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_కార్యక్రమం_Info(handle, CL_కార్యక్రమం_BINARIES, పరిమాణం(ptr), &ptr, ఖాళీ));
    }

    తార్కిక create_From_ద్విమాన(స్థిరరాశి Context& ctx, స్థిరరాశి std::vector<అక్షరం>& buf, String& errmsg)
    {
        తిరిగి create_From_ద్విమాన(ctx, (స్థిరరాశి గుర్తులేని అక్షరం*)&buf[0], buf.size(), errmsg);
    }

    తార్కిక create_From_ద్విమాన(స్థిరరాశి Context& ctx, స్థిరరాశి గుర్తులేని అక్షరం* ద్విమాన_Addr, స్థిరరాశి size_t ద్విమాన_కొలత, String& errmsg)
    {
        CV_Assert(handle == ఖాళీ);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(ఖాళీ, 0, "Load from binary... (" << ద్విమాన_కొలత << " bytes)");

        CV_Assert(ద్విమాన_కొలత > 0);

        size_t ndevices = (పూర్ణాంకం)ctx.ndevices();
        Auto_బఫరు<cl_device_id> devices_(ndevices);
        Auto_బఫరు<స్థిరరాశి uchar*> ద్విమాన_Ptrs(ndevices);
        Auto_బఫరు<size_t> ద్విమాన_Sizes(ndevices);

        cl_device_id* devices = devices_.దత్తాంశం();
        స్థిరరాశి uchar** ద్విమాన_Ptrs_2 = ద్విమాన_Ptrs.దత్తాంశం();
        size_t* ద్విమాన_Sizes_2 = ద్విమాన_Sizes.దత్తాంశం();
        కొరకు (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ద్విమాన_Ptrs_2[i] = ద్విమాన_Addr;
            ద్విమాన_Sizes_2[i] = ద్విమాన_కొలత;
        }

        cl_int ఫలితం = 0;
        handle = cl_Create_కార్యక్రమం_With_ద్విమాన((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.దత్తాంశం(),
                                           ద్విమాన_Sizes_2, ద్విమాన_Ptrs_2, ఖాళీ, &ఫలితం);
        ఒకవేళ (ఫలితం != CL_SUCCESS)
        {
            CV_LOG_ERROR(ఖాళీ, CV_OCL_API_ERROR_సందేశం(ఫలితం, "clCreateProgramWithBinary"));
            ఒకవేళ (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_కార్యక్రమం(handle));
                handle = ఖాళీ;
            }
        }
        ఒకవేళ (!handle)
        {
            తిరిగి అబద్ధం;
        }
        // call clBuildProgram()
        {
            ఫలితం = cl_Build_కార్యక్రమం(handle, (cl_uint)ndevices, devices_.దత్తాంశం(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_ఫలితం(ఫలితం, cv::format("clBuildProgram(binary: %s/%s)", ఆధారం_Module.c_str(), ఆధారం_Name.c_str()).c_str());
            ఒకవేళ (ఫలితం != CL_SUCCESS)
            {
                dumpBuildLog_(ఫలితం, devices, errmsg);
                ఒకవేళ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_కార్యక్రమం(handle));
                    handle = ఖాళీ;
                }
                తిరిగి అబద్ధం;
            }
        }
        // check build status
        {
            cl_build_స్థితి build_స్థితి = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(ఫలితం = cl_Get_కార్యక్రమం_Build_Info(handle, devices[0], CL_కార్యక్రమం_BUILD_స్థితి,
                    పరిమాణం(build_స్థితి), &build_స్థితి, &retsz));
            ఒకవేళ (ఫలితం == CL_SUCCESS)
            {
                ఒకవేళ (build_స్థితి == CL_BUILD_SUCCESS)
                {
                    తిరిగి నిజం;
                }
                లేకపోతే
                {
                    CV_LOG_WARNING(ఖాళీ, "clGetProgramBuildInfo() returns " << build_స్థితి);
                    తిరిగి అబద్ధం;
                }
            }
            లేకపోతే
            {
                CV_LOG_ERROR(ఖాళీ, CV_OCL_API_ERROR_సందేశం(ఫలితం, "clGetProgramBuildInfo()"));
                ఒకవేళ (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_కార్యక్రమం(handle));
                    handle = ఖాళీ;
                }
            }
        }
#ఒకవేళ CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS
        ఒకవేళ (handle && CV_OPENCL_VALIDATE_ద్విమాన_PROGRAMS_విలువ)
        {
            CV_LOG_INFO(ఖాళీ, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            అక్షరం kernels_బఫరు[4096] = {0};
            ఫలితం = cl_Get_కార్యక్రమం_Info(handle, CL_కార్యక్రమం_KERNEL_పేర్లు, పరిమాణం(kernels_బఫరు), &kernels_బఫరు[0], &retsz);
            ఒకవేళ (retsz < పరిమాణం(kernels_బఫరు))
                kernels_బఫరు[retsz] = 0;
            లేకపోతే
                kernels_బఫరు[0] = 0;
            CV_LOG_INFO(ఖాళీ, ఫలితం << ": Kernels='" << kernels_బఫరు << "'");
        }
#అయితే_ముగింపు
        తిరిగి handle != ఖాళీ;
    }

    ~Impl()
    {
        ఒకవేళ( handle )
        {
#నిర్వచితమైతే _WIN32
            ఒకవేళ (!cv::__termination)
#అయితే_ముగింపు
            {
                cl_Release_కార్యక్రమం(handle);
            }
            handle = ఖాళీ;
        }
    }

    cl_కార్యక్రమం handle;

    String buildflags;
    String ఆధారం_Module;
    String ఆధారం_Name;
};


కార్యక్రమం_2::కార్యక్రమం_2() CV_NOEXCEPT
{
    p = 0;
}

కార్యక్రమం_2::కార్యక్రమం_2(స్థిరరాశి కార్యక్రమం_ఆధారం& src,
        స్థిరరాశి String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

కార్యక్రమం_2::కార్యక్రమం_2(స్థిరరాశి కార్యక్రమం_2& prog)
{
    p = prog.p;
    ఒకవేళ(p)
        p->addref();
}

కార్యక్రమం_2& కార్యక్రమం_2::క్రియాకారకం = (స్థిరరాశి కార్యక్రమం_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    ఒకవేళ(newp)
        newp->addref();
    ఒకవేళ(p)
        p->release();
    p = newp;
    తిరిగి *ఇది;
}

కార్యక్రమం_2::కార్యక్రమం_2(కార్యక్రమం_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = శూన్యసూచిక;
}

కార్యక్రమం_2& కార్యక్రమం_2::క్రియాకారకం = (కార్యక్రమం_2&& prog) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &prog) {
        ఒకవేళ(p)
            p->release();
        p = prog.p;
        prog.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

కార్యక్రమం_2::~కార్యక్రమం_2()
{
    ఒకవేళ(p)
        p->release();
}

తార్కిక కార్యక్రమం_2::create(స్థిరరాశి కార్యక్రమం_ఆధారం& src,
            స్థిరరాశి String& buildflags, String& errmsg)
{
    ఒకవేళ(p)
    {
        p->release();
        p = ఖాళీ;
    }
    p = కొత్త Impl(src, buildflags, errmsg);
    ఒకవేళ(!p->handle)
    {
        p->release();
        p = 0;
    }
    తిరిగి p != 0;
}

శూన్య* కార్యక్రమం_2::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

#నిర్వచితం_కాకపోతే OPENCV_REMOVE_DEPRECATED_API
స్థిరరాశి కార్యక్రమం_ఆధారం& కార్యక్రమం_2::ఆధారం() స్థిరరాశి
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

తార్కిక కార్యక్రమం_2::పఠ(స్థిరరాశి String& bin, స్థిరరాశి String& buildflags)
{
    CV_వాడని(bin); CV_వాడని(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

తార్కిక కార్యక్రమం_2::లిఖ(String& bin) స్థిరరాశి
{
    CV_వాడని(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String కార్యక్రమం_2::get_ఉపసర్గ() స్థిరరాశి
{
    ఒకవేళ(!p)
        తిరిగి String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    తిరిగి cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ఉపసర్గ_String().c_str(), p->buildflags.c_str());
}

String కార్యక్రమం_2::get_ఉపసర్గ(స్థిరరాశి String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        తిరిగి cv::format("opencl=%s\nbuildflags=%s", ctx_->get_ఉపసర్గ_String().c_str(), buildflags.c_str());
}
#అయితే_ముగింపు // OPENCV_REMOVE_DEPRECATED_API

శూన్య కార్యక్రమం_2::get_ద్విమాన(std::vector<అక్షరం>& binary) స్థిరరాశి
{
    CV_Assert(p && "Empty program");
    p->get_కార్యక్రమం_ద్విమాన(binary);
}

కార్యక్రమం_2 Context::Impl::getProg(స్థిరరాశి కార్యక్రమం_ఆధారం& src,
                               స్థిరరాశి String& buildflags, String& errmsg)
{
    size_t హద్దు = get_కార్యక్రమం_లెక్క_హద్దు();
    స్థిరరాశి కార్యక్రమం_ఆధారం::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String కీలకం = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->ఆధారం_Hash.c_str(),
            get_ఉపసర్గ_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_తాళం lock(కార్యక్రమం_cache_mutex);
        phash_t::iterator it = phash.find(కీలకం);
        ఒకవేళ (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), కీలకం);
            ఒకవేళ (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(కీలకం);
            }
            తిరిగి it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            ఒకవేళ (హద్దు > 0 && sz >= హద్దు)
            {
                స్థిర తార్కిక warningFlag = అబద్ధం;
                ఒకవేళ (!warningFlag)
                {
                    రాయి("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = నిజం;
                }
                అయితే (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    ఒకవేళ (c != 0)
                        ఆపు;
                }
            }
        }
    }
    కార్యక్రమం_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_తాళం lock(కార్యక్రమం_cache_mutex);
        phash.insert(std::pair<std::string, కార్యక్రమం_2>(కీలకం, prog));
        cacheList.push_front(కీలకం);
    }
    తిరిగి prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

మూస<రకం_పేరు T>
తరగతి OpenCLBufferPool
{
రక్షిత:
    ~OpenCLBufferPool() { }
బహిరంగ:
    మాయా T allocate(size_t size) = 0;
    మాయా శూన్య release(T బఫరు) = 0;
};

మూస <రకం_పేరు Derived, రకం_పేరు బఫరు_నమోదు, రకం_పేరు T>
తరగతి Open_CLBuffer_Pool_పునాది_Impl : బహిరంగ బఫరు_Pool_Controller, బహిరంగ OpenCLBufferPool<T>
{
అంతరంగ:
    అంతఃవరుస Derived& derived() { తిరిగి *static_cast<Derived*>(ఇది); }
రక్షిత:
    Mutex mutex_;

    size_t current_Reserved_కొలత;
    size_t max_Reserved_కొలత;

    std::list<బఫరు_నమోదు> allocatedEntries_; // Allocated and used entries
    std::list<బఫరు_నమోదు> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    తార్కిక కనుగొను_And_Remove_నమోదు_From_Allocated_List(CV_బహిర్గతం బఫరు_నమోదు& నమోదు_2, T బఫరు)
    {
        రకం_పేరు std::list<బఫరు_నమోదు>::iterator i = allocatedEntries_.begin();
        కొరకు (; i != allocatedEntries_.end(); ++i)
        {
            బఫరు_నమోదు& e = *i;
            ఒకవేళ (e.cl_బఫరు_2 == బఫరు)
            {
                నమోదు_2 = e;
                allocatedEntries_.erase(i);
                తిరిగి నిజం;
            }
        }
        తిరిగి అబద్ధం;
    }

    // synchronized
    తార్కిక కనుగొను_And_Remove_నమోదు_From_Reserved_List(CV_బహిర్గతం బఫరు_నమోదు& నమోదు_2, స్థిరరాశి size_t size)
    {
        ఒకవేళ (reservedEntries_.empty())
            తిరిగి అబద్ధం;
        రకం_పేరు std::list<బఫరు_నమోదు>::iterator i = reservedEntries_.begin();
        రకం_పేరు std::list<బఫరు_నమోదు>::iterator ఫలితం_pos = reservedEntries_.end();
        బఫరు_నమోదు ఫలితం;
        size_t minDiff = (size_t)(-1);
        కొరకు (; i != reservedEntries_.end(); ++i)
        {
            బఫరు_నమోదు& e = *i;
            ఒకవేళ (e.సామర్థ్యం >= size)
            {
                size_t diff = e.సామర్థ్యం - size;
                ఒకవేళ (diff < std::max((size_t)4096, size / 8) && (ఫలితం_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    ఫలితం_pos = i;
                    ఫలితం = e;
                    ఒకవేళ (diff == 0)
                        ఆపు;
                }
            }
        }
        ఒకవేళ (ఫలితం_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(ఫలితం_pos);
            నమోదు_2 = ఫలితం;
            current_Reserved_కొలత -= నమోదు_2.సామర్థ్యం;
            allocatedEntries_.push_back(నమోదు_2);
            తిరిగి నిజం;
        }
        తిరిగి అబద్ధం;
    }

    // synchronized
    శూన్య check_కొలత_యొక్క_Reserved_Entries()
    {
        అయితే (current_Reserved_కొలత > max_Reserved_కొలత)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            స్థిరరాశి బఫరు_నమోదు& నమోదు_2 = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_కొలత >= నమోదు_2.సామర్థ్యం);
            current_Reserved_కొలత -= నమోదు_2.సామర్థ్యం;
            derived().release_బఫరు_నమోదు(నమోదు_2);
            reservedEntries_.pop_back();
        }
    }

    అంతఃవరుస size_t కేటాయింపు_Granularity(size_t size)
    {
        // heuristic values
        ఒకవేళ (size < 1024*1024)
            తిరిగి 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        లేకపోతే ఒకవేళ (size < 16*1024*1024)
            తిరిగి 64*1024;
        లేకపోతే
            తిరిగి 1024*1024;
    }

బహిరంగ:
    Open_CLBuffer_Pool_పునాది_Impl()
        : current_Reserved_కొలత(0),
          max_Reserved_కొలత(0)
    {
        // nothing
    }
    మాయా ~Open_CLBuffer_Pool_పునాది_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
బహిరంగ:
    మాయా T allocate(size_t size) CV_OVERRIDE
    {
        Auto_తాళం locker(mutex_);
        బఫరు_నమోదు నమోదు_2;
        ఒకవేళ (max_Reserved_కొలత > 0 && కనుగొను_And_Remove_నమోదు_From_Reserved_List(నమోదు_2, size))
        {
            CV_DbgAssert(size <= నమోదు_2.సామర్థ్యం);
            LOG_బఫరు_POOL("Reuse reserved buffer: %p\n", నమోదు_2.cl_బఫరు_2);
        }
        లేకపోతే
        {
            derived().allocate_బఫరు_నమోదు(నమోదు_2, size);
        }
        తిరిగి నమోదు_2.cl_బఫరు_2;
    }
    మాయా శూన్య release(T బఫరు) CV_OVERRIDE
    {
        Auto_తాళం locker(mutex_);
        బఫరు_నమోదు నమోదు_2;
        CV_Assert(కనుగొను_And_Remove_నమోదు_From_Allocated_List(నమోదు_2, బఫరు));
        ఒకవేళ (max_Reserved_కొలత == 0 || నమోదు_2.సామర్థ్యం > max_Reserved_కొలత / 8)
        {
            derived().release_బఫరు_నమోదు(నమోదు_2);
        }
        లేకపోతే
        {
            reservedEntries_.push_front(నమోదు_2);
            current_Reserved_కొలత += నమోదు_2.సామర్థ్యం;
            check_కొలత_యొక్క_Reserved_Entries();
        }
    }

    మాయా size_t get_Reserved_కొలత() స్థిరరాశి CV_OVERRIDE { తిరిగి current_Reserved_కొలత; }
    మాయా size_t get_Max_Reserved_కొలత() స్థిరరాశి CV_OVERRIDE { తిరిగి max_Reserved_కొలత; }
    మాయా శూన్య సమితి_Max_Reserved_కొలత(size_t size) CV_OVERRIDE
    {
        Auto_తాళం locker(mutex_);
        size_t old_Max_Reserved_కొలత = max_Reserved_కొలత;
        max_Reserved_కొలత = size;
        ఒకవేళ (max_Reserved_కొలత < old_Max_Reserved_కొలత)
        {
            రకం_పేరు std::list<బఫరు_నమోదు>::iterator i = reservedEntries_.begin();
            కొరకు (; i != reservedEntries_.end();)
            {
                స్థిరరాశి బఫరు_నమోదు& నమోదు_2 = *i;
                ఒకవేళ (నమోదు_2.సామర్థ్యం > max_Reserved_కొలత / 8)
                {
                    CV_DbgAssert(current_Reserved_కొలత >= నమోదు_2.సామర్థ్యం);
                    current_Reserved_కొలత -= నమోదు_2.సామర్థ్యం;
                    derived().release_బఫరు_నమోదు(నమోదు_2);
                    i = reservedEntries_.erase(i);
                    కొనసాగు;
                }
                ++i;
            }
            check_కొలత_యొక్క_Reserved_Entries();
        }
    }
    మాయా శూన్య freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_తాళం locker(mutex_);
        రకం_పేరు std::list<బఫరు_నమోదు>::const_iterator i = reservedEntries_.begin();
        కొరకు (; i != reservedEntries_.end(); ++i)
        {
            స్థిరరాశి బఫరు_నమోదు& నమోదు_2 = *i;
            derived().release_బఫరు_నమోదు(నమోదు_2);
        }
        reservedEntries_.clear();
        current_Reserved_కొలత = 0;
    }
};

నిర్మాణం CLBuffer_నమోదు
{
    cl_mem cl_బఫరు_2;
    size_t సామర్థ్యం;
    CLBuffer_నమోదు() : cl_బఫరు_2((cl_mem)ఖాళీ), సామర్థ్యం(0) { }
};

తరగతి OpenCLBufferPoolImpl CV_FINAL : బహిరంగ Open_CLBuffer_Pool_పునాది_Impl<OpenCLBufferPoolImpl, CLBuffer_నమోదు, cl_mem>
{
బహిరంగ:
    రకనిర్వచనం నిర్మాణం CLBuffer_నమోదు బఫరు_నమోదు;
రక్షిత:
    పూర్ణాంకం createFlags_;
బహిరంగ:
    OpenCLBufferPoolImpl(పూర్ణాంకం createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    శూన్య allocate_బఫరు_నమోదు(బఫరు_నమోదు& నమోదు_2, size_t size)
    {
        CV_DbgAssert(నమోదు_2.cl_బఫరు_2 == ఖాళీ);
        నమోదు_2.సామర్థ్యం = align_కొలత(size, (పూర్ణాంకం)కేటాయింపు_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        నమోదు_2.cl_బఫరు_2 = cl_Create_బఫరు((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, నమోదు_2.సామర్థ్యం, 0, &retval);
        CV_OCL_CHECK_ఫలితం(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (దీర్ఘ దీర్ఘ పూర్ణాంకం)నమోదు_2.సామర్థ్యం, (శూన్య*)నమోదు_2.cl_బఫరు_2).c_str());
        CV_Assert(నమోదు_2.cl_బఫరు_2 != ఖాళీ);
        ఒకవేళ(retval == CL_SUCCESS)
        {
            CV_IMPL_కూడు(CV_IMPL_OCL);
        }
        LOG_బఫరు_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, నమోదు_2.cl_బఫరు_2);
        allocatedEntries_.push_back(నమోదు_2);
    }

    శూన్య release_బఫరు_నమోదు(స్థిరరాశి బఫరు_నమోదు& నమోదు_2)
    {
        CV_Assert(నమోదు_2.సామర్థ్యం != 0);
        CV_Assert(నమోదు_2.cl_బఫరు_2 != ఖాళీ);
        LOG_బఫరు_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                నమోదు_2.cl_బఫరు_2, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం);
        CV_OCL_DBG_CHECK(clReleaseMemObject(నమోదు_2.cl_బఫరు_2));
    }
};

#నిర్వచితమైతే HAVE_OPENCL_SVM
నిర్మాణం CLSVMBuffer_నమోదు
{
    శూన్య* cl_బఫరు_2;
    size_t సామర్థ్యం;
    CLSVMBuffer_నమోదు() : cl_బఫరు_2(ఖాళీ), సామర్థ్యం(0) { }
};
తరగతి OpenCLSVMBufferPoolImpl CV_FINAL : బహిరంగ Open_CLBuffer_Pool_పునాది_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_నమోదు, శూన్య*>
{
బహిరంగ:
    రకనిర్వచనం నిర్మాణం CLSVMBuffer_నమోదు బఫరు_నమోదు;
బహిరంగ:
    OpenCLSVMBufferPoolImpl()
    {
    }

    శూన్య allocate_బఫరు_నమోదు(బఫరు_నమోదు& నమోదు_2, size_t size)
    {
        CV_DbgAssert(నమోదు_2.cl_బఫరు_2 == ఖాళీ);
        నమోదు_2.సామర్థ్యం = align_కొలత(size, (పూర్ణాంకం)కేటాయింపు_Granularity(size));

        Context& ctx = Context::getDefault();
        స్థిరరాశి svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        తార్కిక is_Fine_Grain_బఫరు = svmCaps.is_Support_Fine_Grain_బఫరు();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_బఫరు ? CL_MEM_SVM_FINE_GRAIN_బఫరు : 0);

        స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (పూర్ణాంకం)నమోదు_2.సామర్థ్యం);
        శూన్య *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, నమోదు_2.సామర్థ్యం, 0);
        CV_Assert(buf);

        నమోదు_2.cl_బఫరు_2 = buf;
        {
            CV_IMPL_కూడు(CV_IMPL_OCL);
        }
        LOG_బఫరు_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, నమోదు_2.cl_బఫరు_2);
        allocatedEntries_.push_back(నమోదు_2);
    }

    శూన్య release_బఫరు_నమోదు(స్థిరరాశి బఫరు_నమోదు& నమోదు_2)
    {
        CV_Assert(నమోదు_2.సామర్థ్యం != 0);
        CV_Assert(నమోదు_2.cl_బఫరు_2 != ఖాళీ);
        LOG_బఫరు_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                నమోదు_2.cl_బఫరు_2, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం, (దీర్ఘ దీర్ఘ)నమోదు_2.సామర్థ్యం);
        Context& ctx = Context::getDefault();
        స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  నమోదు_2.cl_బఫరు_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), నమోదు_2.cl_బఫరు_2);
    }
};
#అయితే_ముగింపు



మూస <తార్కిక readAccess, తార్కిక writeAccess>
తరగతి Aligned_దత్తాంశం_Ptr
{
రక్షిత:
    స్థిరరాశి size_t కొలత_3;
    uchar* స్థిరరాశి originPtr_;
    స్థిరరాశి size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

బహిరంగ:
    Aligned_దత్తాంశం_Ptr(uchar* ptr, size_t size, size_t alignment)
        : కొలత_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ఖాళీ)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        ఒకవేళ (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = కొత్త uchar[కొలత_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ఒకవేళ (readAccess)
            {
                జ్ఞాపకం_నకలు(ptr_, originPtr_, కొలత_3);
            }
        }
    }

    uchar* getAlignedPtr() స్థిరరాశి
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        తిరిగి ptr_;
    }

    ~Aligned_దత్తాంశం_Ptr()
    {
        ఒకవేళ (allocatedPtr_)
        {
            ఒకవేళ (writeAccess)
            {
                జ్ఞాపకం_నకలు(originPtr_, ptr_, కొలత_3);
            }
            తొలగించు[] allocatedPtr_;
            allocatedPtr_ = ఖాళీ;
        }
        ptr_ = ఖాళీ;
    }
అంతరంగ:
    Aligned_దత్తాంశం_Ptr(స్థిరరాశి Aligned_దత్తాంశం_Ptr&); // disabled
    Aligned_దత్తాంశం_Ptr& క్రియాకారకం=(స్థిరరాశి Aligned_దత్తాంశం_Ptr&); // disabled
};

మూస <తార్కిక readAccess, తార్కిక writeAccess>
తరగతి Aligned_దత్తాంశం_Ptr2_D
{
రక్షిత:
    స్థిరరాశి size_t కొలత_3;
    uchar* స్థిరరాశి originPtr_;
    స్థిరరాశి size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t అడ్డువరుసలు_2;
    size_t నిలువుల_సంఖ్య_2;
    size_t అడుగు_2;

బహిరంగ:
    Aligned_దత్తాంశం_Ptr2_D(uchar* ptr, size_t అడ్డువరుసలు, size_t నిలువుల_సంఖ్య, size_t అడుగు, size_t alignment, size_t extrabytes=0)
        : కొలత_3(అడ్డువరుసలు*అడుగు), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(ఖాళీ), అడ్డువరుసలు_2(అడ్డువరుసలు), నిలువుల_సంఖ్య_2(నిలువుల_సంఖ్య), అడుగు_2(అడుగు)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != ఖాళీ);
        ఒకవేళ (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = కొత్త uchar[కొలత_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            ఒకవేళ (readAccess)
            {
                కొరకు (size_t i = 0; i < అడ్డువరుసలు_2; i++)
                    జ్ఞాపకం_నకలు(ptr_ + i*అడుగు_2, originPtr_ + i*అడుగు_2, నిలువుల_సంఖ్య_2);
            }
        }
    }

    uchar* getAlignedPtr() స్థిరరాశి
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        తిరిగి ptr_;
    }

    ~Aligned_దత్తాంశం_Ptr2_D()
    {
        ఒకవేళ (allocatedPtr_)
        {
            ఒకవేళ (writeAccess)
            {
                కొరకు (size_t i = 0; i < అడ్డువరుసలు_2; i++)
                    జ్ఞాపకం_నకలు(originPtr_ + i*అడుగు_2, ptr_ + i*అడుగు_2, నిలువుల_సంఖ్య_2);
            }
            తొలగించు[] allocatedPtr_;
            allocatedPtr_ = ఖాళీ;
        }
        ptr_ = ఖాళీ;
    }
అంతరంగ:
    Aligned_దత్తాంశం_Ptr2_D(స్థిరరాశి Aligned_దత్తాంశం_Ptr2_D&); // disabled
    Aligned_దత్తాంశం_Ptr2_D& క్రియాకారకం=(స్థిరరాశి Aligned_దత్తాంశం_Ptr2_D&); // disabled
};

#నిర్వచితం_కాకపోతే CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT
#నిర్వచించు CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT 16
#అయితే_ముగింపు


శూన్య Context::Impl::__init_buffer_pools()
{
    బఫరు_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& బఫరు_Pool_3 = *బఫరు_Pool_2.get();
    బఫరు_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& బఫరు_Pool_Host_Ptr_2 = *బఫరు_Pool_Host_Ptr.get();

    size_t default_Pool_కొలత = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_కొలత = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_కొలత);
    బఫరు_Pool_3.సమితి_Max_Reserved_కొలత(pool_కొలత);
    size_t pool_కొలత_Host_Ptr = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_కొలత);
    బఫరు_Pool_Host_Ptr_2.సమితి_Max_Reserved_కొలత(pool_కొలత_Host_Ptr);

#నిర్వచితమైతే HAVE_OPENCL_SVM
    బఫరు_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& బఫరు_Pool_SVM_2 = *బఫరు_Pool_SVM.get();
    size_t pool_కొలత_SVM = utils::get_Configuration_Parameter_కొలత_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_కొలత);
    బఫరు_Pool_SVM_2.సమితి_Max_Reserved_కొలత(pool_కొలత_SVM);
#అయితే_ముగింపు

    CV_LOG_INFO(ఖాళీ, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_కొలత << " poolSizeHostPtr=" << pool_కొలత_Host_Ptr);
}

తరగతి OpenCLAllocator CV_FINAL : బహిరంగ MatAllocator
{
బహిరంగ:
    గణన AllocatorFlags
    {
        ALLOCATOR_FLAGS_బఫరు_POOL_వాడిన = 1 << 0,
        ALLOCATOR_FLAGS_బఫరు_POOL_HOST_PTR_వాడిన = 1 << 1,
#నిర్వచితమైతే HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_బఫరు_POOL_SVM_వాడిన = 1 << 2,
#అయితే_ముగింపు
        ALLOCATOR_FLAGS_EXTERNAL_బఫరు = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_వరుస();
    }

    UMat_దత్తాంశం* defaultAllocate(పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type, శూన్య* దత్తాంశం, size_t* అడుగు,
            AccessFlag flags, UMatUsageFlags usageFlags) స్థిరరాశి
    {
        UMat_దత్తాంశం* u = matStdAllocator->allocate(dims, sizes, type, దత్తాంశం, అడుగు, flags, usageFlags);
        తిరిగి u;
    }

    స్థిర తార్కిక isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        స్థిర తార్కిక value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", అబద్ధం);
        తిరిగి value;
    }
    స్థిర తార్కిక isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        స్థిర తార్కిక value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", అబద్ధం);
        తిరిగి value;
    }

    శూన్య get_ఉత్తమ_Flags(స్థిరరాశి Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, పూర్ణాంకం& createFlags, UMat_దత్తాంశం::జ్ఞాపకం_Flag& flags0) స్థిరరాశి
    {
        స్థిరరాశి Device& dev = ctx.device(0);
        createFlags = 0;
        ఒకవేళ ((usageFlags & USAGE_ALLOCATE_HOST_జ్ఞాపకం) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        ఒకవేళ (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_జ్ఞాపకం_2()
#నిర్వచితం_కాకపోతే __APPLE__
                || dev.isIntel()
#అయితే_ముగింపు
                )
            )
        )
            flags0 = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
        లేకపోతే
            flags0 = UMat_దత్తాంశం::COPY_ON_MAP;
    }

    UMat_దత్తాంశం* allocate(పూర్ణాంకం dims, స్థిరరాశి పూర్ణాంకం* sizes, పూర్ణాంకం type,
                       శూన్య* దత్తాంశం, size_t* అడుగు, AccessFlag flags, UMatUsageFlags usageFlags) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!useOpenCL())
            తిరిగి defaultAllocate(dims, sizes, type, దత్తాంశం, అడుగు, flags, usageFlags);

        flush_Cleanup_వరుస();

        CV_Assert(దత్తాంశం == 0);
        size_t మొత్తం = CV_ELEM_కొలత(type);
        కొరకు( పూర్ణాంకం i = dims-1; i >= 0; i-- )
        {
            ఒకవేళ( అడుగు )
                అడుగు[i] = మొత్తం;
            మొత్తం *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        ఒకవేళ (!ctx.getImpl())
            తిరిగి defaultAllocate(dims, sizes, type, దత్తాంశం, అడుగు, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        పూర్ణాంకం createFlags = 0;
        UMat_దత్తాంశం::జ్ఞాపకం_Flag flags0 = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
        get_ఉత్తమ_Flags(ctx, flags, usageFlags, createFlags, flags0);

        శూన్య* handle = ఖాళీ;
        పూర్ణాంకం allocatorFlags = 0;

#నిర్వచితమైతే HAVE_OPENCL_SVM
        స్థిరరాశి svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        ఒకవేళ (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_బఫరు_POOL_SVM_వాడిన;
            handle = ctxImpl.get_బఫరు_Pool_SVM().allocate(మొత్తం);

            // this property is constant, so single buffer pool can be used here
            తార్కిక is_Fine_Grain_బఫరు = svmCaps.is_Support_Fine_Grain_బఫరు();
            allocatorFlags |= is_Fine_Grain_బఫరు ? svm::OPENCL_SVM_FINE_GRAIN_బఫరు : svm::OPENCL_SVM_COARSE_GRAIN_బఫరు;
        }
        లేకపోతే
#అయితే_ముగింపు
        ఒకవేళ (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_బఫరు_POOL_వాడిన;
            handle = ctxImpl.get_బఫరు_Pool().allocate(మొత్తం);
        }
        లేకపోతే ఒకవేళ (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_బఫరు_POOL_HOST_PTR_వాడిన;
            handle = ctxImpl.get_బఫరు_Pool_Host_Ptr().allocate(మొత్తం);
        }
        లేకపోతే
        {
            CV_Assert(handle != ఖాళీ); // Unsupported, throw
        }

        ఒకవేళ (!handle)
            తిరిగి defaultAllocate(dims, sizes, type, దత్తాంశం, అడుగు, flags, usageFlags);

        UMat_దత్తాంశం* u = కొత్త UMat_దత్తాంశం(ఇది);
        u->దత్తాంశం = 0;
        u->size = మొత్తం;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<శూన్య>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->తాత్కాలిక_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(నిజం);
        opencl_allocator_stats.onAllocate(u->size);
        తిరిగి u;
    }

    తార్కిక allocate(UMat_దత్తాంశం* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి అబద్ధం;

        flush_Cleanup_వరుస();

        UMat_దత్తాంశం_Auto_తాళం lock(u);

        ఒకవేళ(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            పూర్ణాంకం createFlags = 0;
            UMat_దత్తాంశం::జ్ఞాపకం_Flag flags0 = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
            get_ఉత్తమ_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            తార్కిక copyOnMap = (flags0 & UMat_దత్తాంశం::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            పూర్ణాంకం allocatorFlags = 0;
            UMat_దత్తాంశం::జ్ఞాపకం_Flag తాత్కాలిక_UMat_Flags = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
            శూన్య* handle = ఖాళీ;
            cl_int retval = CL_SUCCESS;

#నిర్వచితమైతే HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            తార్కిక useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            ఒకవేళ (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                తాత్కాలిక_UMat_Flags = UMat_దత్తాంశం::తాత్కాలిక_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (పూర్ణాంకం)u->size, handle);
            }
            లేకపోతే ఒకవేళ (useSVM && (svmCaps.is_Support_Fine_Grain_బఫరు() || svmCaps.is_Support_Coarse_Grain_బఫరు()))
            {
                ఒకవేళ (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    తార్కిక is_Fine_Grain_బఫరు = svmCaps.is_Support_Fine_Grain_బఫరు();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_బఫరు ? CL_MEM_SVM_FINE_GRAIN_బఫరు : 0);

                    స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (పూర్ణాంకం)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_వరుస q = ఖాళీ;
                    ఒకవేళ (!is_Fine_Grain_బఫరు)
                    {
                        q = (cl_command_వరుస)వరుస::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (పూర్ణాంకం)u->size);
                        cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, ఖాళీ, ఖాళీ);
                        CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMap()");

                    }
                    జ్ఞాపకం_నకలు(handle, u->origdata, u->size);
                    ఒకవేళ (!is_Fine_Grain_బఫరు)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, handle, 0, ఖాళీ, ఖాళీ);
                        CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                    }

                    తాత్కాలిక_UMat_Flags = UMat_దత్తాంశం::తాత్కాలిక_UMAT | UMat_దత్తాంశం::తాత్కాలిక_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_బఫరు ? svm::OPENCL_SVM_FINE_GRAIN_బఫరు
                                                : svm::OPENCL_SVM_COARSE_GRAIN_బఫరు;
                }
            }
            లేకపోతే
#అయితే_ముగింపు
            {
                ఒకవేళ( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                తాత్కాలిక_UMat_Flags = UMat_దత్తాంశం::తాత్కాలిక_UMAT;
                ఒకవేళ (
                #నిర్వచితమైతే __APPLE__
                    !copyOnMap &&
                #అయితే_ముగింపు
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (పూర్ణాంకం)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_దత్తాంశం && u->original_UMat_దత్తాంశం->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_బఫరు(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, u->origdata, (శూన్య*)handle).c_str());
                }
                ఒకవేళ((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_బఫరు(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, u->origdata, (శూన్య*)handle).c_str());
                    తాత్కాలిక_UMat_Flags |= UMat_దత్తాంశం::తాత్కాలిక_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clCreateBuffer() => %p", (శూన్య*)handle).c_str());
            ఒకవేళ(!handle || retval != CL_SUCCESS)
                తిరిగి అబద్ధం;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = ఇది;
            u->flags |= తాత్కాలిక_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        ఒకవేళ (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(నిజం);
        opencl_allocator_stats.onAllocate(u->size);
        తిరిగి నిజం;
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

    శూన్య deallocate(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        ఒకవేళ (!!(u->flags & UMat_దత్తాంశం::ASYNC_CLEANUP))
            కూడు_గమ్యం_Cleanup_వరుస(u);
        లేకపోతే
            deallocate_(u);
    }

    శూన్య deallocate_(UMat_దత్తాంశం* u) స్థిరరాశి
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        ఒకవేళ ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_బఫరు) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#నిర్వచితమైతే _WIN32
        ఒకవేళ (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            తిరిగి;             // avoid any OpenCL calls
#అయితే_ముగింపు
        ఒకవేళ(u->తాత్కాలిక_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            ఒకవేళ (u->hostCopyObsolete())
            {
#నిర్వచితమైతే HAVE_OPENCL_SVM
                ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
                {
                    Context& ctx = Context::getDefault();
                    స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    ఒకవేళ( u->తాత్కాలిక_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు);
                        తార్కిక is_Fine_Grain_బఫరు = (u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు;
                        cl_command_వరుస q = ఖాళీ;
                        ఒకవేళ (!is_Fine_Grain_బఫరు)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) == 0));
                            q = (cl_command_వరుస)వరుస::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (పూర్ణాంకం)u->size);
                            cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, ఖాళీ, ఖాళీ);
                            CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMap()");
                        }
                        cl_ముగింపు(q);
                        జ్ఞాపకం_నకలు(u->origdata, u->handle, u->size);
                        ఒకవేళ (!is_Fine_Grain_బఫరు)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, u->handle, 0, ఖాళీ, ఖాళీ);
                            CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                        }
                    }
                    లేకపోతే
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                లేకపోతే
#అయితే_ముగింపు
                {
                    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
                    ఒకవేళ( u->తాత్కాలిక_Copied_UMat() )
                    {
                        Aligned_దత్తాంశం_Ptr<అబద్ధం, నిజం> alignedPtr(u->origdata, u->size, CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    లేకపోతే
                    {
                        cl_int retval = 0;
                        ఒకవేళ (u->తాత్కాలిక_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_వరుస(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            శూన్య* దత్తాంశం = cl_వరుసలో_చేర్చు_Map_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (శూన్య*)u->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, దత్తాంశం).c_str());
                            CV_Assert(u->origdata == దత్తాంశం && "Details: https://github.com/opencv/opencv/issues/6293");
                            ఒకవేళ (u->original_UMat_దత్తాంశం)
                            {
                                CV_Assert(u->original_UMat_దత్తాంశం->దత్తాంశం == దత్తాంశం);
                            }
                            retval = cl_వరుసలో_చేర్చు_Unmap_Mem_Object(q, (cl_mem)u->handle, దత్తాంశం, 0, 0, 0);
                            CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (శూన్య*)u->handle, దత్తాంశం, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_ముగింపు(q));
                        }
                    }
                }
                u->markHostCopyObsolete(అబద్ధం);
            }
            లేకపోతే
            {
                // nothing
            }
#నిర్వచితమైతే HAVE_OPENCL_SVM
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
            {
                ఒకవేళ( u->తాత్కాలిక_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            లేకపోతే
#అయితే_ముగింపు
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clReleaseMemObject(ptr=%p)", (శూన్య*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(నిజం);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = ఖాళీ;
            ఒకవేళ(u->దత్తాంశం && u->copyOnMap() && u->దత్తాంశం != u->origdata)
                fastFree(u->దత్తాంశం);
            u->దత్తాంశం = u->origdata;
            u->currAllocator->deallocate(u);
            u = ఖాళీ;
        }
        లేకపోతే
        {
            CV_Assert(u->origdata == ఖాళీ);
            ఒకవేళ(u->దత్తాంశం && u->copyOnMap() && u->దత్తాంశం != u->origdata)
            {
                fastFree(u->దత్తాంశం);
                u->దత్తాంశం = 0;
                u->markHostCopyObsolete(నిజం);
            }
            ఒకవేళ (u->allocatorFlags_ & ALLOCATOR_FLAGS_బఫరు_POOL_వాడిన)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_బఫరు_Pool().release((cl_mem)u->handle);
            }
            లేకపోతే ఒకవేళ (u->allocatorFlags_ & ALLOCATOR_FLAGS_బఫరు_POOL_HOST_PTR_వాడిన)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_బఫరు_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#నిర్వచితమైతే HAVE_OPENCL_SVM
            లేకపోతే ఒకవేళ (u->allocatorFlags_ & ALLOCATOR_FLAGS_బఫరు_POOL_SVM_వాడిన)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                లేకపోతే ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
                {
                    స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

                    ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, u->handle, 0, ఖాళీ, ఖాళీ);
                        CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_బఫరు_Pool_SVM().release((శూన్య*)u->handle);
            }
#అయితే_ముగింపు
            లేకపోతే
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(నిజం);
            తొలగించు u;
            u = ఖాళీ;
        }
        CV_Assert(u == ఖాళీ);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    శూన్య map(UMat_దత్తాంశం* u, AccessFlag accessFlags) స్థిరరాశి CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        ఒకవేళ (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(నిజం);

        cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

        {
            ఒకవేళ( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#నిర్వచితమైతే HAVE_OPENCL_SVM
                ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
                {
                    ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
                    {
                        Context& ctx = Context::getDefault();
                        స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (పూర్ణాంకం)u->size);
                            cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, ఖాళీ, ఖాళీ);
                            CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_బఫరు_MAP;
                        }
                    }
                    cl_ముగింపు(q);
                    u->దత్తాంశం = (uchar*)u->handle;
                    u->markHostCopyObsolete(అబద్ధం);
                    u->markDeviceMemMapped(నిజం);
                    తిరిగి;
                }
#అయితే_ముగింపు

                cl_int retval = CL_SUCCESS;
                ఒకవేళ (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->దత్తాంశం = (uchar*)cl_వరుసలో_చేర్చు_Map_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_ఫలితం(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (శూన్య*)u->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, u->దత్తాంశం).c_str());
                }
                ఒకవేళ (u->దత్తాంశం && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(అబద్ధం);
                    u->markDeviceMemMapped(నిజం);
                    తిరిగి;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_దత్తాంశం::COPY_ON_MAP;
            }

            ఒకవేళ(!u->దత్తాంశం)
            {
                u->దత్తాంశం = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(నిజం);
            }
        }

        ఒకవేళ (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_దత్తాంశం_Ptr<అబద్ధం, నిజం> alignedPtr(u->దత్తాంశం, u->size, CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
#నిర్వచితమైతే HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == 0);
#అయితే_ముగింపు
            cl_int retval = cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (శూన్య*)u->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(అబద్ధం);
        }
    }

    శూన్య unmap(UMat_దత్తాంశం* u) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి;


        CV_Assert(u->handle != 0);

        UMat_దత్తాంశం_Auto_తాళం autolock(u);

        cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
        cl_int retval = 0;
        ఒకవేళ( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->దత్తాంశం != ఖాళీ);
#నిర్వచితమైతే HAVE_OPENCL_SVM
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
            {
                ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
                {
                    Context& ctx = Context::getDefault();
                    స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, u->handle,
                                0, ఖాళీ, ఖాళీ);
                        CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                        cl_ముగింపు(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_బఫరు_MAP;
                    }
                }
                ఒకవేళ (u->refcount == 0)
                    u->దత్తాంశం = 0;
                u->markDeviceCopyObsolete(అబద్ధం);
                u->markHostCopyObsolete(నిజం);
                తిరిగి;
            }
#అయితే_ముగింపు
            ఒకవేళ (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_వరుసలో_చేర్చు_Unmap_Mem_Object(q, (cl_mem)u->handle, u->దత్తాంశం, 0, 0, 0);
                CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (శూన్య*)u->handle, u->దత్తాంశం, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size).c_str());
                ఒకవేళ (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_ముగింపు(q));
                }
                u->markDeviceMemMapped(అబద్ధం);
                u->దత్తాంశం = 0;
                u->markDeviceCopyObsolete(అబద్ధం);
                u->markHostCopyObsolete(నిజం);
            }
        }
        లేకపోతే ఒకవేళ( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_దత్తాంశం_Ptr<నిజం, అబద్ధం> alignedPtr(u->దత్తాంశం, u->size, CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
#నిర్వచితమైతే HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == 0);
#అయితే_ముగింపు
            retval = cl_వరుసలో_చేర్చు_Write_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (శూన్య*)u->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(అబద్ధం);
            u->markHostCopyObsolete(నిజం);
        }
    }

    తార్కిక checkContinuous(పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                         స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
                         స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[],
                         size_t& మొత్తం, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) స్థిరరాశి
    {
        తార్కిక iscontinuous = నిజం;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        మొత్తం = sz[dims-1];
        కొరకు( పూర్ణాంకం i = dims-2; i >= 0; i-- )
        {
            ఒకవేళ( i >= 0 && (మొత్తం != srcstep[i] || మొత్తం != dststep[i]) )
                iscontinuous = అబద్ధం;
            మొత్తం *= sz[i];
            ఒకవేళ( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            ఒకవేళ( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        ఒకవేళ( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            ఒకవేళ( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                ఒకవేళ( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                ఒకవేళ( dstofs )
                {
                    new_dstofs[0] = dstofs[1];
                    new_dstofs[1] = dstofs[0];
                    new_dstofs[2] = 0;
                }

                new_srcstep[0] = srcstep[0]; new_srcstep[1] = 0;
                new_dststep[0] = dststep[0]; new_dststep[1] = 0;
            }
            లేకపోతే
            {
                // we could check for dims == 3 here,
                // but from user perspective this one is more informative
                CV_Assert(dims <= 3);
                new_sz[0] = sz[2]; new_sz[1] = sz[1]; new_sz[2] = sz[0];
                ఒకవేళ( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                ఒకవేళ( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        తిరిగి iscontinuous;
    }

    శూన్య download(UMat_దత్తాంశం* u, శూన్య* dstptr, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                  స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
                  స్థిరరాశి size_t dststep[]) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి;
        UMat_దత్తాంశం_Auto_తాళం autolock(u);

        ఒకవేళ( u->దత్తాంశం && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            తిరిగి;
        }
        CV_Assert( u->handle != 0 );

        cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

        size_t మొత్తం = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        తార్కిక iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            మొత్తం, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
        {
            CV_DbgAssert(u->దత్తాంశం == ఖాళీ || u->దత్తాంశం == u->handle);
            Context& ctx = Context::getDefault();
            స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) == 0);
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (పూర్ణాంకం)u->size);
                cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, ఖాళీ, ఖాళీ);
                CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMap()");
            }
            cl_ముగింపు(q);
            ఒకవేళ( iscontinuous )
            {
                జ్ఞాపకం_నకలు(dstptr, (uchar*)u->handle + srcrawofs, మొత్తం);
            }
            లేకపోతే
            {
                // This code is from MatAllocator::download()
                పూర్ణాంకం isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
                    ఒకవేళ( sz[i] == 0 )
                    తిరిగి;
                    ఒకవేళ( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (పూర్ణాంకం)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                స్థిరరాశి Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                కొరకు( j = 0; j < it.nplanes; j++, ++it )
                    జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
            }
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, u->handle,
                        0, ఖాళీ, ఖాళీ);
                CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                cl_ముగింపు(q);
            }
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            ఒకవేళ( iscontinuous )
            {
                Aligned_దత్తాంశం_Ptr<అబద్ధం, నిజం> alignedPtr((uchar*)dstptr, మొత్తం, CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, మొత్తం, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            లేకపోతే ఒకవేళ (CV_OPENCL_DISABLE_బఫరు_RECT_OPERATIONS)
            {
                స్థిరరాశి size_t padding = CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_దత్తాంశం_Ptr2_D<అబద్ధం, అబద్ధం> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                మొత్తం = align_కొలత(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                మొత్తం = std::min(మొత్తం, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, మొత్తం, ptr, 0, 0, 0));
                కొరకు( size_t i = 0; i < new_sz[1]; i++ )
                    జ్ఞాపకం_నకలు( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            లేకపోతే
            {
                Aligned_దత్తాంశం_Ptr2_D<అబద్ధం, నిజం> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    శూన్య upload(UMat_దత్తాంశం* u, స్థిరరాశి శూన్య* srcptr, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
                స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[],
                స్థిరరాశి size_t srcstep[]) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!u)
            తిరిగి;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->తాత్కాలిక_UMat());

        size_t మొత్తం = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        తార్కిక iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            మొత్తం, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_దత్తాంశం_Auto_తాళం autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        ఒకవేళ( u->దత్తాంశం && (u->hostCopyObsolete() < u->deviceCopyObsolete() || మొత్తం == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(అబద్ధం);
            u->markDeviceCopyObsolete(నిజం);
            తిరిగి;
        }

        CV_Assert( u->handle != 0 );
        cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
        {
            CV_DbgAssert(u->దత్తాంశం == ఖాళీ || u->దత్తాంశం == u->handle);
            Context& ctx = Context::getDefault();
            స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_MAP) == 0);
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (పూర్ణాంకం)u->size);
                cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, ఖాళీ, ఖాళీ);
                CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMap()");
            }
            cl_ముగింపు(q);
            ఒకవేళ( iscontinuous )
            {
                జ్ఞాపకం_నకలు((uchar*)u->handle + dstrawofs, srcptr, మొత్తం);
            }
            లేకపోతే
            {
                // This code is from MatAllocator::upload()
                పూర్ణాంకం isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
                    ఒకవేళ( sz[i] == 0 )
                    తిరిగి;
                    ఒకవేళ( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (పూర్ణాంకం)sz[i];
                }

                Mat src(dims, isz, CV_8U, (శూన్య*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                స్థిరరాశి Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                కొరకు( j = 0; j < it.nplanes; j++, ++it )
                    జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
            }
            ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_COARSE_GRAIN_బఫరు)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMUnmap(q, u->handle,
                        0, ఖాళీ, ఖాళీ);
                CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMUnmap()");
                cl_ముగింపు(q);
            }
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            ఒకవేళ( iscontinuous )
            {
                Aligned_దత్తాంశం_Ptr<నిజం, అబద్ధం> alignedPtr((uchar*)srcptr, మొత్తం, CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
                cl_int retval = cl_వరుసలో_చేర్చు_Write_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, మొత్తం, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (శూన్య*)u->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)dstrawofs, (దీర్ఘ దీర్ఘ పూర్ణాంకం)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            లేకపోతే ఒకవేళ (CV_OPENCL_DISABLE_బఫరు_RECT_OPERATIONS)
            {
                స్థిరరాశి size_t padding = CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_దత్తాంశం_Ptr2_D<అబద్ధం, అబద్ధం> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                మొత్తం = align_కొలత(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                మొత్తం = std::min(మొత్తం, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, మొత్తం, ptr, 0, 0, 0));
                కొరకు( size_t i = 0; i < new_sz[1]; i++ )
                    జ్ఞాపకం_నకలు( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Write_బఫరు(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, మొత్తం, ptr, 0, 0, 0));
            }
            లేకపోతే
            {
                Aligned_దత్తాంశం_Ptr2_D<నిజం, అబద్ధం> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Write_బఫరు_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(నిజం);
#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            u->markHostCopyObsolete(నిజం);
        }
        u->markDeviceCopyObsolete(అబద్ధం);
    }

    శూన్య copy(UMat_దత్తాంశం* src, UMat_దత్తాంశం* dst, పూర్ణాంకం dims, స్థిరరాశి size_t sz[],
              స్థిరరాశి size_t srcofs[], స్థిరరాశి size_t srcstep[],
              స్థిరరాశి size_t dstofs[], స్థిరరాశి size_t dststep[], తార్కిక _sync) స్థిరరాశి CV_OVERRIDE
    {
        ఒకవేళ(!src || !dst)
            తిరిగి;

        size_t మొత్తం = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        తార్కిక iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            మొత్తం, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_దత్తాంశం_Auto_తాళం src_autolock(src, dst);

        ఒకవేళ( !src->handle || (src->దత్తాంశం && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->దత్తాంశం + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            తిరిగి;
        }
        ఒకవేళ( !dst->handle || (dst->దత్తాంశం && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->దత్తాంశం + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(అబద్ధం);
#నిర్వచితమైతే HAVE_OPENCL_SVM
            ఒకవేళ ((dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            లేకపోతే
#అయితే_ముగింపు
            {
                dst->markDeviceCopyObsolete(నిజం);
            }
            తిరిగి;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((src->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
        {
            ఒకవేళ ((src->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
            {
                Context& ctx = Context::getDefault();
                స్థిరరాశి svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                ఒకవేళ( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (పూర్ణాంకం)మొత్తం);
                    cl_int స్థితి_2 = svmFns->fn_cl_వరుసలో_చేర్చు_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            మొత్తం, 0, ఖాళీ, ఖాళీ);
                    CV_OCL_CHECK_ఫలితం(స్థితి_2, "clEnqueueSVMMemcpy()");
                }
                లేకపోతే
                {
                    cl_ముగింపు(q);
                    // This code is from MatAllocator::download()/upload()
                    పూర్ణాంకం isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)గరిష్ఠ_పూర్ణాంకం );
                        ఒకవేళ( sz[i] == 0 )
                        తిరిగి;
                        ఒకవేళ( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (పూర్ణాంకం)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    కొరకు( పూర్ణాంకం i = 0; i < dims; i++ )
                    {
                        ఒకవేళ( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    స్థిరరాశి Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    కొరకు( j = 0; j < it.nplanes; j++, ++it )
                        జ్ఞాపకం_నకలు(ptrs[1], ptrs[0], planesz);
                }
            }
            లేకపోతే
            {
                ఒకవేళ ((src->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->దత్తాంశం + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                లేకపోతే
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->దత్తాంశం + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            ఒకవేళ( iscontinuous )
            {
                retval = cl_వరుసలో_చేర్చు_Copy_బఫరు(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, మొత్తం, 0, 0, 0);
                CV_OCL_CHECK_ఫలితం(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (శూన్య*)src->handle, (శూన్య*)dst->handle, (దీర్ఘ దీర్ఘ పూర్ణాంకం)srcrawofs, (దీర్ఘ దీర్ఘ పూర్ణాంకం)dstrawofs, (దీర్ఘ దీర్ఘ పూర్ణాంకం)మొత్తం).c_str());
            }
            లేకపోతే ఒకవేళ (CV_OPENCL_DISABLE_బఫరు_RECT_OPERATIONS)
            {
                స్థిరరాశి size_t padding = CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_దత్తాంశం_Ptr2_D<అబద్ధం, అబద్ధం> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT, padding*2);
                Aligned_దత్తాంశం_Ptr2_D<అబద్ధం, అబద్ధం> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_దత్తాంశం_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_మొత్తం = align_కొలత(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_మొత్తం = std::min(src_మొత్తం, src->size - new_srcrawofs);
                size_t dst_మొత్తం = align_కొలత(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_మొత్తం = std::min(dst_మొత్తం, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_మొత్తం, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Read_బఫరు(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_మొత్తం, dstptr, 0, 0, 0));

                కొరకు( size_t i = 0; i < new_sz[1]; i++ )
                    జ్ఞాపకం_నకలు( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Write_బఫరు(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_మొత్తం, dstptr, 0, 0, 0));
            }
            లేకపోతే
            {
                CV_OCL_CHECK(retval = cl_వరుసలో_చేర్చు_Copy_బఫరు_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        ఒకవేళ (retval == CL_SUCCESS)
        {
            CV_IMPL_కూడు(CV_IMPL_OCL)
        }

#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_బఫరు ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_బఫరు_ముసుగు) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            dst->markHostCopyObsolete(నిజం);
        }
        dst->markDeviceCopyObsolete(అబద్ధం);

        ఒకవేళ( _sync )
        {
            CV_OCL_DBG_CHECK(cl_ముగింపు(q));
        }
    }

    బఫరు_Pool_Controller* get_బఫరు_Pool_Controller(స్థిరరాశి అక్షరం* id) స్థిరరాశి CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        ఒకవేళ (ctx.empty())
            తిరిగి ఖాళీ;
#నిర్వచితమైతే HAVE_OPENCL_SVM
        ఒకవేళ ((svm::checkForceSVMUmatUsage() && (id == ఖాళీ || strcmp(id, "OCL") == 0)) || (id != ఖాళీ && strcmp(id, "SVM") == 0))
        {
            తిరిగి &ctx.getImpl()->get_బఫరు_Pool_SVM();
        }
#అయితే_ముగింపు
        ఒకవేళ (id != ఖాళీ && strcmp(id, "HOST_ALLOC") == 0)
        {
            తిరిగి &ctx.getImpl()->get_బఫరు_Pool_Host_Ptr();
        }
        ఒకవేళ (id != ఖాళీ && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        తిరిగి &ctx.getImpl()->get_బఫరు_Pool();
    }

    MatAllocator* matStdAllocator;

    మార్చదగిన cv::Mutex cleanup_వరుస_Mutex;
    మార్చదగిన std::deque<UMat_దత్తాంశం*> cleanup_వరుస;

    శూన్య flush_Cleanup_వరుస() స్థిరరాశి
    {
        // TSAN false positive: see #27638
        ఒకవేళ (!cleanup_వరుస.empty())
        {
            std::deque<UMat_దత్తాంశం*> q;
            {
                cv::Auto_తాళం lock(cleanup_వరుస_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_వరుస);
            }
            కొరకు (std::deque<UMat_దత్తాంశం*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    శూన్య కూడు_గమ్యం_Cleanup_వరుస(UMat_దత్తాంశం* u) స్థిరరాశి
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_తాళం lock(cleanup_వరుస_Mutex);
            cleanup_వరుస.push_back(u);
        }
    }
};

స్థిర OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    స్థిర OpenCLAllocator* g_allocator = కొత్త OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    తిరిగి g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


నామాకాశ cv {

// three funcs below are implemented in umatrix.cpp
శూన్య సమితి_కొలత( UMat& m, పూర్ణాంకం _dims, స్థిరరాశి పూర్ణాంకం* _sz, స్థిరరాశి size_t* అడుగులు_2,
              తార్కిక auto_అడుగులు = అబద్ధం );
శూన్య finalizeHdr(UMat& m);

} // namespace cv


నామాకాశ cv { నామాకాశ ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
శూన్య convert_From_బఫరు(శూన్య* cl_mem_బఫరు, size_t అడుగు, పూర్ణాంకం అడ్డువరుసలు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం type, UMat& dst)
{
    పూర్ణాంకం d = 2;
    పూర్ణాంకం sizes[] = { అడ్డువరుసలు, నిలువుల_సంఖ్య };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_ముసుగు) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    సమితి_కొలత(dst, d, sizes, 0, నిజం);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_బఫరు;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, పరిమాణం(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_బఫరు == mem_type);

    size_t మొత్తం = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_కొలత, పరిమాణం(size_t), &మొత్తం, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((పూర్ణాంకం)అడుగు >= నిలువుల_సంఖ్య * CV_ELEM_కొలత(type));
    CV_Assert(మొత్తం >= అడ్డువరుసలు * అడుగు);

    // attach clBuffer to UMatData
    dst.u = కొత్త UMat_దత్తాంశం(getOpenCLAllocator());
    dst.u->దత్తాంశం            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_బఫరు;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_దత్తాంశం::జ్ఞాపకం_Flag>(0);
    dst.u->handle          = cl_mem_బఫరు;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = మొత్తం;

    finalizeHdr(dst);
    dst.addref();

    తిరిగి;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
శూన్య convertFromImage(శూన్య* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, పరిమాణం(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, పరిమాణం(cl_image_format), &fmt, 0));

    పూర్ణాంకం లోతు = CV_8U;
    ఎంపిక (fmt.image_channel_దత్తాంశం_type)
    {
    స్థితి CL_UNORM_INT8:
    స్థితి CL_UNSIGNED_INT8:
        లోతు = CV_8U;
        ఆపు;

    స్థితి CL_SNORM_INT8:
    స్థితి CL_SIGNED_INT8:
        లోతు = CV_8S;
        ఆపు;

    స్థితి CL_UNORM_INT16:
    స్థితి CL_UNSIGNED_INT16:
        లోతు = CV_16U;
        ఆపు;

    స్థితి CL_SNORM_INT16:
    స్థితి CL_SIGNED_INT16:
        లోతు = CV_16S;
        ఆపు;

    స్థితి CL_SIGNED_INT32:
        లోతు = CV_32S;
        ఆపు;

    స్థితి CL_FLOAT:
        లోతు = CV_32F;
        ఆపు;

    స్థితి CL_HALF_FLOAT:
        లోతు = CV_16F;
        ఆపు;

    అప్రమేయ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    పూర్ణాంకం type = CV_8UC1;
    ఎంపిక (fmt.image_channel_క్రమం)
    {
    స్థితి CL_R:
    స్థితి CL_A:
    స్థితి CL_INTENSITY:
    స్థితి CL_LUMINANCE:
        type = CV_MAKE_TYPE(లోతు, 1);
        ఆపు;

    స్థితి CL_RG:
    స్థితి CL_RA:
        type = CV_MAKE_TYPE(లోతు, 2);
        ఆపు;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    స్థితి CL_RGBA:
    స్థితి CL_BGRA:
    స్థితి CL_ARGB:
        type = CV_MAKE_TYPE(లోతు, 4);
        ఆపు;

    అప్రమేయ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        ఆపు;
    }

    size_t అడుగు = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_అడ్డువరుస_PITCH, పరిమాణం(size_t), &అడుగు, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_వెడల్పు, పరిమాణం(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ఎత్తు, పరిమాణం(size_t), &h, 0));

    dst.create((పూర్ణాంకం)h, (పూర్ణాంకం)w, type);

    cl_mem cl_బఫరు = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, src_origin, region, offset, 0, ఖాళీ, ఖాళీ));

    CV_OCL_CHECK(cl_ముగింపు(q));

    తిరిగి;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

స్థిర శూన్య getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint అంకె_Devices = 0;
    cl_int స్థితి_2 = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, ఖాళీ, &అంకె_Devices);
    ఒకవేళ (స్థితి_2 != CL_DEVICE_NOT_దొరికింది) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_ఫలితం(స్థితి_2,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &అంకె_Devices).c_str());
    }

    ఒకవేళ (అంకె_Devices == 0)
    {
        devices.clear();
        తిరిగి;
    }

    devices.resize((size_t)అంకె_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, అంకె_Devices, &devices[0], &అంకె_Devices));
}

నిర్మాణం PlatformInfo::Impl
{
    Impl(శూన్య* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        సంస్కరణ_3 = getStrProp(CL_PLATFORM_సంస్కరణ);
        parseOpenCLVersion(సంస్కరణ_3, సంస్కరణ_Major, సంస్కరణ_Minor);
    }

    String getStrProp(cl_platform_info prop) స్థిరరాశి
    {
        అక్షరం buf[1024];
        size_t sz=0;
        తిరిగి clGetPlatformInfo(handle, prop, పరిమాణం(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < పరిమాణం(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String సంస్కరణ_3;
    పూర్ణాంకం సంస్కరణ_Major;
    పూర్ణాంకం సంస్కరణ_Minor;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(శూన్య* platform_id)
{
    p = కొత్త Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    ఒకవేళ(p)
        p->release();
}

PlatformInfo::PlatformInfo(స్థిరరాశి PlatformInfo& i)
{
    ఒకవేళ (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::క్రియాకారకం =(స్థిరరాశి PlatformInfo& i)
{
    ఒకవేళ (i.p != p)
    {
        ఒకవేళ (i.p)
            i.p->addref();
        ఒకవేళ (p)
            p->release();
        p = i.p;
    }
    తిరిగి *ఇది;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = శూన్యసూచిక;
}

PlatformInfo& PlatformInfo::క్రియాకారకం = (PlatformInfo&& i) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &i) {
        ఒకవేళ(p)
            p->release();
        p = i.p;
        i.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

పూర్ణాంకం PlatformInfo::device_సంఖ్య() స్థిరరాశి
{
    తిరిగి p ? (పూర్ణాంకం)p->devices.size() : 0;
}

శూన్య PlatformInfo::getDevice(Device& device, పూర్ణాంకం d) స్థిరరాశి
{
    CV_Assert(p && d < (పూర్ణాంకం)p->devices.size() );
    ఒకవేళ(p)
        device.సమితి(p->devices[d]);
}

String PlatformInfo::name() స్థిరరాశి
{
    తిరిగి p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() స్థిరరాశి
{
    తిరిగి p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::సంస్కరణ() స్థిరరాశి
{
    తిరిగి p ? p->సంస్కరణ_3 : String();
}

పూర్ణాంకం PlatformInfo::సంస్కరణ_Major_2() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->సంస్కరణ_Major;
}

పూర్ణాంకం PlatformInfo::సంస్కరణ_Minor_2() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->సంస్కరణ_Minor;
}

స్థిర శూన్య getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint అంకె_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms));

    ఒకవేళ (అంకె_Platforms == 0)
    {
        platforms.clear();
        తిరిగి;
    }

    platforms.resize((size_t)అంకె_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(అంకె_Platforms, &platforms[0], &అంకె_Platforms));
}

శూన్య getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    కొరకు (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((శూన్య*)&platforms[i]) );
}

స్థిరరాశి అక్షరం* type_గమ్యం_Str(పూర్ణాంకం type)
{
    స్థిర స్థిరరాశి అక్షరం* tab[CV_లోతు_MAX*16]=
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
    పూర్ణాంకం cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type);
    స్థిరరాశి అక్షరం* ఫలితం = cn > 16 ? శూన్యసూచిక : tab[లోతు*16 + cn-1];
    CV_Assert(ఫలితం);
    తిరిగి ఫలితం;
}

స్థిరరాశి అక్షరం* memop_Type_గమ్యం_Str(పూర్ణాంకం type)
{
    స్థిర స్థిరరాశి అక్షరం* tab[CV_లోతు_MAX*16] =
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
    పూర్ణాంకం cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type);
    స్థిరరాశి అక్షరం* ఫలితం = cn > 16 ? శూన్యసూచిక : tab[లోతు*16 + cn-1];
    CV_Assert(ఫలితం);
    తిరిగి ఫలితం;
}

స్థిరరాశి అక్షరం* vecop_Type_గమ్యం_Str(పూర్ణాంకం type)
{
    స్థిర స్థిరరాశి అక్షరం* tab[CV_లోతు_MAX*16] =
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
    పూర్ణాంకం cn = CV_MAT_CN(type), లోతు = CV_MAT_లోతు(type);
    స్థిరరాశి అక్షరం* ఫలితం = cn > 16 ? 0 : tab[లోతు*16 + cn-1];
    CV_Assert(ఫలితం);
    తిరిగి ఫలితం;
}

// Deprecated due to size of buf buffer being unknowable.
స్థిరరాశి అక్షరం* convertTypeStr(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth, పూర్ణాంకం cn, అక్షరం* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    తిరిగి convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

స్థిరరాశి అక్షరం* convertTypeStr(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth, పూర్ణాంకం cn, అక్షరం* buf, size_t buf_కొలత_2)
{
    ఒకవేళ( sdepth == ddepth )
        తిరిగి "noconvert";
    స్థిరరాశి అక్షరం *typestr = type_గమ్యం_Str(CV_MAKETYPE(ddepth, cn));
    ఒకవేళ( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_కొలత_2, "convert_%s", typestr);
    }
    లేకపోతే ఒకవేళ( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_కొలత_2, "convert_%s_sat_rte", typestr);
    లేకపోతే
        snprintf(buf, buf_కొలత_2, "convert_%s_sat", typestr);

    తిరిగి buf;
}

స్థిరరాశి అక్షరం* getOpenCLErrorString(పూర్ణాంకం error_సంకేతం)
{
#నిర్వచించు CV_OCL_సంకేతం(id) స్థితి id: తిరిగి #id
#నిర్వచించు CV_OCL_సంకేతం_2(id, name) స్థితి id: తిరిగి #name
    ఎంపిక (error_సంకేతం)
    {
    CV_OCL_సంకేతం(CL_SUCCESS);
    CV_OCL_సంకేతం(CL_DEVICE_NOT_దొరికింది);
    CV_OCL_సంకేతం(CL_DEVICE_NOT_అందుబాటు);
    CV_OCL_సంకేతం(CL_COMPILER_NOT_అందుబాటు);
    CV_OCL_సంకేతం(CL_MEM_OBJECT_కేటాయింపు_FAILURE);
    CV_OCL_సంకేతం(CL_బహిర్గతం_యొక్క_RESOURCES);
    CV_OCL_సంకేతం(CL_బహిర్గతం_యొక్క_HOST_జ్ఞాపకం);
    CV_OCL_సంకేతం(CL_PROFILING_INFO_NOT_అందుబాటు);
    CV_OCL_సంకేతం(CL_MEM_COPY_OVERLAP);
    CV_OCL_సంకేతం(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_సంకేతం(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_సంకేతం(CL_BUILD_కార్యక్రమం_FAILURE);
    CV_OCL_సంకేతం(CL_MAP_FAILURE);
    CV_OCL_సంకేతం(CL_MISALIGNED_SUB_బఫరు_స్థానభ్రంశం);
    CV_OCL_సంకేతం(CL_EXEC_స్థితి_ERROR_FOR_EVENTS_లోపల_WAIT_LIST);
    CV_OCL_సంకేతం(CL_COMPILE_కార్యక్రమం_FAILURE);
    CV_OCL_సంకేతం(CL_LINKER_NOT_అందుబాటు);
    CV_OCL_సంకేతం(CL_LINK_కార్యక్రమం_FAILURE);
    CV_OCL_సంకేతం(CL_DEVICE_విభజన_FAILED);
    CV_OCL_సంకేతం(CL_KERNEL_ARG_INFO_NOT_అందుబాటు);
    CV_OCL_సంకేతం(CL_INVALID_విలువ);
    CV_OCL_సంకేతం(CL_INVALID_DEVICE_TYPE);
    CV_OCL_సంకేతం(CL_INVALID_PLATFORM);
    CV_OCL_సంకేతం(CL_INVALID_DEVICE);
    CV_OCL_సంకేతం(CL_INVALID_CONTEXT);
    CV_OCL_సంకేతం(CL_INVALID_వరుస_PROPERTIES);
    CV_OCL_సంకేతం(CL_INVALID_COMMAND_వరుస);
    CV_OCL_సంకేతం(CL_INVALID_HOST_PTR);
    CV_OCL_సంకేతం(CL_INVALID_MEM_OBJECT);
    CV_OCL_సంకేతం(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_సంకేతం(CL_INVALID_IMAGE_కొలత);
    CV_OCL_సంకేతం(CL_INVALID_SAMPLER);
    CV_OCL_సంకేతం(CL_INVALID_ద్విమాన);
    CV_OCL_సంకేతం(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_సంకేతం(CL_INVALID_కార్యక్రమం);
    CV_OCL_సంకేతం(CL_INVALID_కార్యక్రమం_EXECUTABLE);
    CV_OCL_సంకేతం(CL_INVALID_KERNEL_NAME);
    CV_OCL_సంకేతం(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_సంకేతం(CL_INVALID_KERNEL);
    CV_OCL_సంకేతం(CL_INVALID_ARG_సూచీ);
    CV_OCL_సంకేతం(CL_INVALID_ARG_విలువ);
    CV_OCL_సంకేతం(CL_INVALID_ARG_కొలత);
    CV_OCL_సంకేతం(CL_INVALID_KERNEL_వాదనలు);
    CV_OCL_సంకేతం(CL_INVALID_WORK_DIMENSION);
    CV_OCL_సంకేతం(CL_INVALID_WORK_సమూహం_కొలత);
    CV_OCL_సంకేతం(CL_INVALID_WORK_అంశం_కొలత);
    CV_OCL_సంకేతం(CL_INVALID_GLOBAL_స్థానభ్రంశం);
    CV_OCL_సంకేతం(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_సంకేతం(CL_INVALID_EVENT);
    CV_OCL_సంకేతం(CL_INVALID_OPERATION);
    CV_OCL_సంకేతం(CL_INVALID_GL_OBJECT);
    CV_OCL_సంకేతం(CL_INVALID_బఫరు_కొలత);
    CV_OCL_సంకేతం(CL_INVALID_MIP_LEVEL);
    CV_OCL_సంకేతం(CL_INVALID_GLOBAL_WORK_కొలత);
    // OpenCL 1.1
    CV_OCL_సంకేతం(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_సంకేతం(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_సంకేతం(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_సంకేతం(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_సంకేతం(CL_INVALID_DEVICE_విభజన_లెక్క);
    // OpenCL 2.0
    CV_OCL_సంకేతం_2(-69, CL_INVALID_PIPE_కొలత);
    CV_OCL_సంకేతం_2(-70, CL_INVALID_DEVICE_వరుస);
    // Extensions
    CV_OCL_సంకేతం_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_సంకేతం_2(-1001, CL_PLATFORM_NOT_దొరికింది_KHR);
    CV_OCL_సంకేతం_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_సంకేతం_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_సంకేతం_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_సంకేతం_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    అప్రమేయ: తిరిగి "Unknown OpenCL error";
    }
#నిర్వచనం_రద్దు CV_OCL_సంకేతం
#నిర్వచనం_రద్దు CV_OCL_సంకేతం_2
}

మూస <రకం_పేరు T>
స్థిర std::string ker_గమ్యం_Str(స్థిరరాశి Mat & k)
{
    పూర్ణాంకం వెడల్పు = k.నిలువుల_సంఖ్య - 1, లోతు = k.లోతు();
    స్థిరరాశి T * స్థిరరాశి దత్తాంశం = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    ఒకవేళ (లోతు <= CV_8S)
    {
        కొరకు (పూర్ణాంకం i = 0; i < వెడల్పు; ++i)
            stream << "DIG(" << (పూర్ణాంకం)దత్తాంశం[i] << ")";
        stream << "DIG(" << (పూర్ణాంకం)దత్తాంశం[వెడల్పు] << ")";
    }
    లేకపోతే ఒకవేళ (లోతు == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        కొరకు (పూర్ణాంకం i = 0; i < వెడల్పు; ++i)
            stream << "DIG(" << దత్తాంశం[i] << "f)";
        stream << "DIG(" << దత్తాంశం[వెడల్పు] << "f)";
    }
    లేకపోతే ఒకవేళ (లోతు == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        కొరకు (పూర్ణాంకం i = 0; i < వెడల్పు; ++i)
            stream << "DIG(" << (దశాంశం)దత్తాంశం[i] << "h)";
        stream << "DIG(" << (దశాంశం)దత్తాంశం[వెడల్పు] << "h)";
    }
    లేకపోతే
    {
        కొరకు (పూర్ణాంకం i = 0; i < వెడల్పు; ++i)
            stream << "DIG(" << దత్తాంశం[i] << ")";
        stream << "DIG(" << దత్తాంశం[వెడల్పు] << ")";
    }

    తిరిగి stream.str();
}

String kernel_గమ్యం_Str(ఆదానం_Array _kernel, పూర్ణాంకం ddepth, స్థిరరాశి అక్షరం * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    పూర్ణాంకం లోతు = kernel.లోతు();
    ఒకవేళ (ddepth < 0)
        ddepth = లోతు;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    ఒకవేళ (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    ఒకవేళ (ddepth != లోతు)
        kernel.convert_గమ్యం(kernel, ddepth);

    రకనిర్వచనం std::string (* func_t)(స్థిరరాశి Mat &);
    స్థిర స్థిరరాశి func_t funcs[CV_లోతు_MAX] = { ker_గమ్యం_Str<uchar>, ker_గమ్యం_Str<అక్షరం>, ker_గమ్యం_Str<ushort>, ker_గమ్యం_Str<పొట్టి>,
                                                ker_గమ్యం_Str<పూర్ణాంకం>, ker_గమ్యం_Str<దశాంశం>, ker_గమ్యం_Str<ద్విగుణ>, ker_గమ్యం_Str<hfloat>,
                                                0, ker_గమ్యం_Str<తార్కిక>, ker_గమ్యం_Str<uint64_t>, ker_గమ్యం_Str<int64_t>, ker_గమ్యం_Str<గుర్తులేని> };
    స్థిరరాశి func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    తిరిగి cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#నిర్వచించు PROCESS_SRC(src) \
    చేయి \
    { \
        ఒకవేళ (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            కొలత csize = src.size(); \
            పూర్ణాంకం ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_లోతు(ctype), \
                ckercn = సదిశ_Widths[cdepth], cwidth = ccn * csize.వెడల్పు; \
            ఒకవేళ (cwidth < ckercn || ckercn <= 0) \
                తిరిగి 1; \
            నిలువుల_సంఖ్య.push_back(cwidth); \
            ఒకవేళ (strat == OCL_సదిశ_OWN && ctype != ref_type) \
                తిరిగి 1; \
            offsets.push_back(src.offset()); \
            అడుగులు.push_back(src.అడుగు()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    అయితే ((శూన్య)0, 0)

పూర్ణాంకం predict_Optimal_సదిశ_వెడల్పు(ఆదానం_Array src1, ఆదానం_Array src2, ఆదానం_Array src3,
                              ఆదానం_Array src4, ఆదానం_Array src5, ఆదానం_Array src6,
                              ఆదానం_Array src7, ఆదానం_Array src8, ఆదానం_Array src9,
                              Ocl_సదిశ_Strategy strat)
{
    స్థిరరాశి ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    పూర్ణాంకం సదిశ_Widths[CV_లోతు_MAX] = {
        d.preferred_సదిశ_వెడల్పు_Char(), d.preferred_సదిశ_వెడల్పు_Char(),   // CV_8U,  CV_8S
        d.preferred_సదిశ_వెడల్పు_Short(), d.preferred_సదిశ_వెడల్పు_Short(), // CV_16U, CV_16S
        d.preferred_సదిశ_వెడల్పు_Int(), d.preferred_సదిశ_వెడల్పు_Float(),   // CV_32S, CV_32F
        d.preferred_సదిశ_వెడల్పు_Double(), d.preferred_సదిశ_వెడల్పు_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_సదిశ_వెడల్పు_Char(),                                 // CV_Bool
        d.preferred_సదిశ_వెడల్పు_Long(), d.preferred_సదిశ_వెడల్పు_Long(),   // CV_64U, CV_64S
        d.preferred_సదిశ_వెడల్పు_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    ఒకవేళ (సదిశ_Widths[0] == 1)
    {
        // it's heuristic
        సదిశ_Widths[CV_8U] = సదిశ_Widths[CV_8S] = 4;
        సదిశ_Widths[CV_16U] = సదిశ_Widths[CV_16S] = సదిశ_Widths[CV_16F] = 2;
        సదిశ_Widths[CV_32S] = సదిశ_Widths[CV_32F] = సదిశ_Widths[CV_64F] = 1;
    }

    తిరిగి check_Optimal_సదిశ_వెడల్పు(సదిశ_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

పూర్ణాంకం check_Optimal_సదిశ_వెడల్పు([[maybe_unused]] స్థిరరాశి పూర్ణాంకం *సదిశ_Widths,
                            [[maybe_unused]] ఆదానం_Array src1,
                            [[maybe_unused]] ఆదానం_Array src2,
                            [[maybe_unused]] ఆదానం_Array src3,
                            [[maybe_unused]] ఆదానం_Array src4,
                            [[maybe_unused]] ఆదానం_Array src5,
                            [[maybe_unused]] ఆదానం_Array src6,
                            [[maybe_unused]] ఆదానం_Array src7,
                            [[maybe_unused]] ఆదానం_Array src8,
                            [[maybe_unused]] ఆదానం_Array src9,
                            [[maybe_unused]] Ocl_సదిశ_Strategy strat)
{
#నిర్వచితమైతే __APPLE__
    తిరిగి 1;
#లేకపోతే
    CV_Assert(సదిశ_Widths);

    పూర్ణాంకం ref_type = src1.type();

    std::vector<size_t> offsets, అడుగులు, నిలువుల_సంఖ్య;
    std::vector<పూర్ణాంకం> dividers, kercns;
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

    కొరకు (size_t i = 0; i < size; ++i)
        అయితే (offsets[i] % dividers[i] != 0 || అడుగులు[i] % dividers[i] != 0 || నిలువుల_సంఖ్య[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    పూర్ణాంకం kercn = *std::min_element(kercns.begin(), kercns.end());

    తిరిగి kercn;
#అయితే_ముగింపు
}

పూర్ణాంకం predict_Optimal_సదిశ_వెడల్పు_Max(ఆదానం_Array src1, ఆదానం_Array src2, ఆదానం_Array src3,
                                 ఆదానం_Array src4, ఆదానం_Array src5, ఆదానం_Array src6,
                                 ఆదానం_Array src7, ఆదానం_Array src8, ఆదానం_Array src9)
{
    తిరిగి predict_Optimal_సదిశ_వెడల్పు(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_సదిశ_MAX);
}

#నిర్వచనం_రద్దు PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
శూన్య build_Options_కూడు_మాతృక_Description(String& buildOptions, స్థిరరాశి String& name, ఆదానం_Array _m)
{
    ఒకవేళ (!buildOptions.empty())
        buildOptions += " ";
    పూర్ణాంకం type = _m.type(), లోతు = CV_MAT_లోతు(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_గమ్యం_Str(type),
            name.c_str(), ocl::type_గమ్యం_Str(CV_MAKE_TYPE(లోతు, 1)),
            name.c_str(), (పూర్ణాంకం)CV_MAT_CN(type),
            name.c_str(), (పూర్ణాంకం)CV_ELEM_కొలత(type),
            name.c_str(), (పూర్ణాంకం)CV_ELEM_SIZE1(type),
            name.c_str(), (పూర్ణాంకం)లోతు
            );
}


నిర్మాణం Image2D::Impl
{
    Impl(స్థిరరాశి UMat &src, తార్కిక norm, తార్కిక alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        ఒకవేళ (handle)
            clReleaseMemObject(handle);
    }

    స్థిర cl_image_format getImageFormat(పూర్ణాంకం లోతు, పూర్ణాంకం cn, తార్కిక norm)
    {
        cl_image_format format;
        స్థిర స్థిరరాశి పూర్ణాంకం channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        స్థిర స్థిరరాశి పూర్ణాంకం channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        స్థిర స్థిరరాశి పూర్ణాంకం channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        పూర్ణాంకం channelType = norm ? channelTypesNorm[లోతు] : channelTypes[లోతు];
        పూర్ణాంకం channel_క్రమం = channelOrders[cn];
        format.image_channel_దత్తాంశం_type = (cl_channel_type)channelType;
        format.image_channel_క్రమం = (cl_channel_క్రమం)channel_క్రమం;
        తిరిగి format;
    }

    స్థిర తార్కిక isFormatSupported(cl_image_format format)
    {
        ఒకవేళ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        ఒకవేళ (!context)
            తిరిగి అబద్ధం;

        // Figure out how many formats are supported by this context.
        cl_uint అంకె_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, అంకె_Formats,
                                                ఖాళీ, &అంకె_Formats);
        CV_OCL_DBG_CHECK_ఫలితం(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        ఒకవేళ (అంకె_Formats > 0)
        {
            Auto_బఫరు<cl_image_format> formats(అంకె_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, అంకె_Formats,
                                             formats.దత్తాంశం(), ఖాళీ);
            CV_OCL_DBG_CHECK_ఫలితం(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            కొరకు (cl_uint i = 0; i < అంకె_Formats; ++i)
            {
                ఒకవేళ (!memcmp(&formats[i], &format, పరిమాణం(format)))
                {
                    తిరిగి నిజం;
                }
            }
        }
        తిరిగి అబద్ధం;
    }

    శూన్య init(స్థిరరాశి UMat &src, తార్కిక norm, తార్కిక alias)
    {
        ఒకవేళ (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        పూర్ణాంకం err, లోతు = src.లోతు(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(లోతు, cn, norm);

        ఒకవేళ (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        ఒకవేళ (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_వరుస వరుస_2 = (cl_command_వరుస)వరుస::getDefault().ptr();

#నిర్వచితమైతే CL_సంస్కరణ_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        స్థిరరాశి Device & d = ocl::Device::getDefault();
        పూర్ణాంకం minor = d.device_సంస్కరణ_Minor(), major = d.device_సంస్కరణ_Major();
        CV_Assert(!alias || canCreateAlias(src));
        ఒకవేళ (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_వెడల్పు      = src.నిలువుల_సంఖ్య;
            desc.image_ఎత్తు     = src.అడ్డువరుసలు;
            desc.image_లోతు      = 0;
            desc.image_array_కొలత = 1;
            desc.image_అడ్డువరుస_pitch  = alias ? src.అడుగు[0] : 0;
            desc.image_ముక్క_pitch = 0;
            desc.బఫరు           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.అంకె_mip_స్థాయిలు   = 0;
            desc.అంకె_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, ఖాళీ, &err);
        }
        లేకపోతే
#అయితే_ముగింపు
        {
            CV_SUPPRESS_DEPRECATED_ప్రారంభం
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.నిలువుల_సంఖ్య, src.అడ్డువరుసలు, 0, ఖాళీ, &err);
            CV_SUPPRESS_DEPRECATED_చివర
        }
        CV_OCL_DBG_CHECK_ఫలితం(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.నిలువుల_సంఖ్య), static_cast<size_t>(src.అడ్డువరుసలు), 1 };

        cl_mem dev_దత్తాంశం;
        ఒకవేళ (!alias && !src.isContinuous())
        {
            dev_దత్తాంశం = cl_Create_బఫరు(context, CL_MEM_READ_ONLY, src.నిలువుల_సంఖ్య * src.అడ్డువరుసలు * src.elem_కొలత(), ఖాళీ, &err);
            CV_OCL_CHECK_ఫలితం(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (దీర్ఘ దీర్ఘ పూర్ణాంకం)(src.నిలువుల_సంఖ్య * src.అడ్డువరుసలు * src.elem_కొలత()), (శూన్య*)dev_దత్తాంశం
                ).c_str());

            స్థిరరాశి size_t roi[3] = {static_cast<size_t>(src.నిలువుల_సంఖ్య) * src.elem_కొలత(), static_cast<size_t>(src.అడ్డువరుసలు), 1};
            CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Copy_బఫరు_Rect(వరుస_2, (cl_mem)src.handle(ACCESS_READ), dev_దత్తాంశం, origin, origin,
                roi, src.అడుగు, 0, src.నిలువుల_సంఖ్య * src.elem_కొలత(), 0, 0, ఖాళీ, ఖాళీ));
            CV_OCL_DBG_CHECK(clFlush(వరుస_2));
        }
        లేకపోతే
        {
            dev_దత్తాంశం = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_దత్తాంశం != ఖాళీ);

        ఒకవేళ (!alias)
        {
            CV_OCL_CHECK(cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(వరుస_2, dev_దత్తాంశం, handle, 0, origin, region, 0, ఖాళీ, 0));
            ఒకవేళ (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(వరుస_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_దత్తాంశం));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = ఖాళీ;
}

Image2D::Image2D(స్థిరరాశి UMat &src, తార్కిక norm, తార్కిక alias)
{
    p = కొత్త Impl(src, norm, alias);
}

తార్కిక Image2D::canCreateAlias(స్థిరరాశి UMat &m)
{
    తార్కిక ret = అబద్ధం;
    స్థిరరాశి Device & d = ocl::Device::getDefault();
    ఒకవేళ (d.image_From_బఫరు_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        ఒకవేళ (pitchAlign && !(m.అడుగు % (pitchAlign * m.elem_కొలత())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            ఒకవేళ (!m.u->తాత్కాలిక_UMat())
            {
                ret = నిజం;
            }
        }
    }
    తిరిగి ret;
}

తార్కిక Image2D::isFormatSupported(పూర్ణాంకం లోతు, పూర్ణాంకం cn, తార్కిక norm)
{
    cl_image_format format = Impl::getImageFormat(లోతు, cn, norm);

    తిరిగి Impl::isFormatSupported(format);
}

Image2D::Image2D(స్థిరరాశి Image2D & i)
{
    p = i.p;
    ఒకవేళ (p)
        p->addref();
}

Image2D & Image2D::క్రియాకారకం = (స్థిరరాశి Image2D & i)
{
    ఒకవేళ (i.p != p)
    {
        ఒకవేళ (i.p)
            i.p->addref();
        ఒకవేళ (p)
            p->release();
        p = i.p;
    }
    తిరిగి *ఇది;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = శూన్యసూచిక;
}

Image2D& Image2D::క్రియాకారకం = (Image2D&& i) CV_NOEXCEPT
{
    ఒకవేళ (ఇది != &i) {
        ఒకవేళ (p)
            p->release();
        p = i.p;
        i.p = శూన్యసూచిక;
    }
    తిరిగి *ఇది;
}

Image2D::~Image2D()
{
    ఒకవేళ (p)
        p->release();
}

శూన్య* Image2D::ptr() స్థిరరాశి
{
    తిరిగి p ? p->handle : 0;
}

తార్కిక internal::isOpenCLForced()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక value = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి value;
}

తార్కిక internal::isPerformanceCheckBypassed()
{
    స్థిర తార్కిక initialized = అబద్ధం;
    స్థిర తార్కిక value = అబద్ధం;
    ఒకవేళ (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", అబద్ధం);
        initialized = నిజం;
    }
    తిరిగి value;
}

తార్కిక internal::isCLBuffer(UMat& u)
{
    ఒకవేళ (isCudaBacked(u.u))
        తిరిగి అబద్ధం;
    శూన్య* h = u.handle(ACCESS_RW);
    ఒకవేళ (!h)
        తిరిగి నిజం;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#ఒకవేళ 1
    ఒకవేళ ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        తిరిగి అబద్ధం;
#లేకపోతే
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, పరిమాణం(type), &type, ఖాళీ);
    ఒకవేళ (ret != CL_SUCCESS || type != CL_MEM_OBJECT_బఫరు)
        తిరిగి అబద్ధం;
#అయితే_ముగింపు
    తిరిగి నిజం;
}

నిర్మాణం Timer::Impl
{
    స్థిరరాశి వరుస వరుస_2;

    Impl(స్థిరరాశి వరుస& q)
        : వరుస_2(q)
    {
    }

    ~Impl(){}

    శూన్య ప్రారంభం()
    {
        CV_OCL_DBG_CHECK(cl_ముగింపు((cl_command_వరుస)వరుస_2.ptr()));
        timer.ప్రారంభం();
    }

    శూన్య stop()
    {
        CV_OCL_DBG_CHECK(cl_ముగింపు((cl_command_వరుస)వరుస_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() స్థిరరాశి
    {
        తిరిగి (uint64)(timer.get_సమయం_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(స్థిరరాశి వరుస& q) : p(కొత్త Impl(q)) { }
Timer::~Timer() { తొలగించు p; }

శూన్య Timer::ప్రారంభం()
{
    CV_Assert(p);
    p->ప్రారంభం();
}

శూన్య Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() స్థిరరాశి
{
    CV_Assert(p);
    తిరిగి p->durationNS();
}

}} // namespace

#అయితే_ముగింపు // HAVE_OPENCL
