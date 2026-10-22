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

#جيڪڏهن_وصف_نه_ٿيل HAVE_OPENCL
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
#جيڪڏهن_پورو

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#وصف_رد CV_LOG_STRIP_LEVEL
#وصف_ڪريو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/ocl_genbase.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "opencv2/core/utils/lock.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#وصف_ڪريو CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "OpenCL allocator: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#وصف_رد CV__ALLOCATOR_STATS_LOG

#وصف_ڪريو CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#وصف_ڪريو CV_OPENCL_SHOW_BUILD_OPTIONS             0
#وصف_ڪريو CV_OPENCL_SHOW_BUILD_KERNELS             0

#وصف_ڪريو CV_OPENCL_SHOW_لاڳيتو_KERNELS               0
#وصف_ڪريو CV_OPENCL_SYNC_لاڳيتو_KERNELS               0
#وصف_ڪريو CV_OPENCL_TRACE_CHECK                    0

#وصف_ڪريو CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS       1

#وصف_ڪريو CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#وصف_ڪريو CV_OPENCL_SHOW_SVM_LOG                   0

#ضمن "opencv2/core/bufferpool.hpp"
#جيڪڏهن_وصف_نه_ٿيل LOG_بفر_POOL
# إذا 0
#   define LOG_بفر_POOL لکو
# وإلا
#   define LOG_بفر_POOL(...)
# endif
#جيڪڏهن_پورو

#إذا CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#وصف_ڪريو CV_OPENCL_SVM_TRACE_P لکو("line %d (ocl.cpp): ", __LINE__); لکو
#وإلا
#وصف_ڪريو CV_OPENCL_SVM_TRACE_P(...)
#جيڪڏهن_پورو

#إذا CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#وصف_ڪريو CV_OPENCL_SVM_TRACE_ERROR_P لکو("Error on line %d (ocl.cpp): ", __LINE__); لکو
#وإلا
#وصف_ڪريو CV_OPENCL_SVM_TRACE_ERROR_P(...)
#جيڪڏهن_پورو

#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
#ضمن "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ضمن "opencv2/core/opencl/opencl_svm.hpp"
#جيڪڏهن_پورو

#ضمن "umatrix.hpp"

نطاق cv { نطاق ocl {

#وصف_ڪريو IMPLEMENT_REFCOUNTABLE() \
    فراغ addref() { CV_XADD(&refcount, 1); } \
    فراغ release() { إذا( CV_XADD(&refcount, -1) == 1 && !cv::__termination) ميٽيو هذا; } \
    صحيح refcount

ساكن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    أرجع opencl_allocator_stats;
}

#جيڪڏهن_وصف_نه_ٿيل _DEBUG
ساكن منطقي isRaiseError()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", خطأ);
        initialized = سچ;
    }
    أرجع value;
}
#جيڪڏهن_پورو

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
فراغ traceOpenCLCheck(cl_int حالت, ثابت حرف* message)
{
    std::ٻاهر << "OpenCV(OpenCL:" << حالت << "): " << message << std::سٽ_پڄاڻي << std::flush;
}
#وصف_ڪريو CV_OCL_TRACE_CHECK_نتيجو(حالت, message) traceOpenCLCheck(حالت, message)
#وإلا
#وصف_ڪريو CV_OCL_TRACE_CHECK_نتيجو(حالت, message) /* nothing */
#جيڪڏهن_پورو

#وصف_ڪريو CV_OCL_API_ERROR_نياپو(check_نتيجو, نياپو) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_نتيجو), check_نتيجو, نياپو)

#وصف_ڪريو CV_OCL_CHECK_نتيجو(check_نتيجو, نياپو) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتيجو(check_نتيجو, نياپو); \
        إذا (check_نتيجو != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(نياپو), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_نياپو = CV_OCL_API_ERROR_نياپو(check_نتيجو, نياپو); \
            CV_Error(Error::OpenCLApiCallError, error_نياپو); \
        } \
    } طالما (0)

#وصف_ڪريو CV_OCL_CHECK_(اظهار, check_نتيجو) افعل { اظهار; CV_OCL_CHECK_نتيجو(check_نتيجو, #expr); } طالما (0)

#وصف_ڪريو CV_OCL_CHECK(اظهار) افعل { cl_int __cl_result = (اظهار); CV_OCL_CHECK_نتيجو(__cl_result, #expr); } طالما (0)

#جيڪڏهن_وصف_ٿيل _DEBUG
#وصف_ڪريو CV_OCL_DBG_CHECK_نتيجو(check_نتيجو, نياپو) CV_OCL_CHECK_نتيجو(check_نتيجو, نياپو)
#وصف_ڪريو CV_OCL_DBG_CHECK(اظهار) CV_OCL_CHECK(اظهار)
#وصف_ڪريو CV_OCL_DBG_CHECK_(اظهار, check_نتيجو) CV_OCL_CHECK_(اظهار, check_نتيجو)
#وإلا
#وصف_ڪريو CV_OCL_DBG_CHECK_نتيجو(check_نتيجو, نياپو) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتيجو(check_نتيجو, نياپو); \
        إذا (check_نتيجو != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(نياپو), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_نياپو = CV_OCL_API_ERROR_نياپو(check_نتيجو, نياپو); \
            CV_Error(Error::OpenCLApiCallError, error_نياپو); \
        } \
    } طالما (0)
#وصف_ڪريو CV_OCL_DBG_CHECK_(اظهار, check_نتيجو) افعل { اظهار; CV_OCL_DBG_CHECK_نتيجو(check_نتيجو, #expr); } طالما (0)
#وصف_ڪريو CV_OCL_DBG_CHECK(اظهار) افعل { cl_int __cl_result = (اظهار); CV_OCL_DBG_CHECK_نتيجو(__cl_result, #expr); } طالما (0)
#جيڪڏهن_پورو


ساكن ثابت منطقي CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_تالو_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", سچ);

#إذا CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS
ساكن ثابت منطقي CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS_قدر = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", خطأ);
#جيڪڏهن_پورو

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساكن ثابت منطقي CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#إذا 1 //def __APPLE__
        سچ
#وإلا
        خطأ
#جيڪڏهن_پورو
);

ساكن String getBuildExtraOptions()
{
    ساكن String param_buildExtraOptions;
    ساكن منطقي initialized = خطأ;
    إذا (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = سچ;
        إذا (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(عدم, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    أرجع param_buildExtraOptions;
}

ساكن ثابت منطقي CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", سچ);
ساكن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


بنية UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        قدم = (صحيح)m.قدم;
        صفون = m.صفون;
        کانن_ڳڻپ = m.کانن_ڳڻپ;
    }
    صحيح offset;
    صحيح قدم;
    صحيح صفون;
    صحيح کانن_ڳڻپ;
};

بنية UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        قدم = (صحيح)m.قدم.p[1];
        slicestep = (صحيح)m.قدم.p[0];
        slices = (صحيح)m.size.p[0];
        صفون = m.size.p[1];
        کانن_ڳڻپ = m.size.p[2];
    }
    صحيح offset;
    صحيح slicestep;
    صحيح قدم;
    صحيح slices;
    صحيح صفون;
    صحيح کانن_ڳڻپ;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ساكن uint64 crc64( ثابت uchar* معلومات, size_t size, uint64 crc0=0 )
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
        initialized = سچ;
    }

    uint64 چڪري_جاچ = ~crc0;
    لكل( size_t idx = 0; idx < size; idx++ )
        چڪري_جاچ = table[(uchar)چڪري_جاچ ^ معلومات[idx]] ^ (چڪري_جاچ >> 8);

    أرجع ~چڪري_جاچ;
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
بنية OpenCLBinaryCacheConfigurator
{
    cv::String cache_رستو;
    cv::String cache_تالو_filename;
    cv::Ptr<utils::fs::File_تالو> cache_تالو;

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
        cache_رستو = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        إذا (cache_رستو.empty())
        {
            CV_LOG_INFO(عدم, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        افعل
        {
            حاول
            {
                إذا (cache_رستو.empty())
                    اكسر;
                إذا (cache_رستو == "disabled")
                    اكسر;
                إذا (!utils::fs::createDirectories(cache_رستو))
                {
                    CV_LOG_DEBUG(عدم, "Can't use OpenCL cache directory: " << cache_رستو);
                    clear();
                    اكسر;
                }

                إذا (CV_OPENCL_CACHE_تالو_ENABLE)
                {
                    cache_تالو_filename = cache_رستو + ".lock";
                    إذا (!utils::fs::exists(cache_تالو_filename))
                    {
                        CV_LOG_DEBUG(عدم, "Creating lock file... (" << cache_تالو_filename << ")");
                        std::ofstream تالو_filename(cache_تالو_filename.c_str(), std::ios::out);
                        إذا (!تالو_filename.is_open())
                        {
                            CV_LOG_WARNING(عدم, "Can't create lock file for OpenCL program cache: " << cache_تالو_filename);
                            اكسر;
                        }
                    }

                    حاول
                    {
                        cache_تالو = makePtr<utils::fs::File_تالو>(cache_تالو_filename.c_str());
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... (" << cache_تالو_filename << ")");
                        {
                            utils::shared_تالو_guard<utils::fs::File_تالو> lock(*cache_تالو);
                        }
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... Done!");
                    }
                    امسك (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_تالو_filename << std::سٽ_پڄاڻي << e.what());
                    }
                    امسك (...)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_تالو_filename);
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
                CV_LOG_WARNING(عدم, "Can't prepare OpenCL program cache: " << cache_رستو << std::سٽ_پڄاڻي << e.what());
                clear();
            }
        } طالما (0);
        إذا (!cache_رستو.empty())
        {
            إذا (cache_تالو.empty() && CV_OPENCL_CACHE_تالو_ENABLE)
            {
                CV_LOG_WARNING(عدم, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Successfully initialized OpenCL cache directory: " << cache_رستو);
            }
        }
    }

    فراغ clear()
    {
        cache_رستو.clear();
        cache_تالو_filename.clear();
        cache_تالو.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_اڳياڙي,
            ثابت std::string& cleanup_اڳياڙي)
    {
        إذا (cache_رستو.empty())
            أرجع std::string();

        Auto_تالو lock(mutex_prepared_contexts_);

        ContextCacheType::iterator مليو_it = prepared_contexts_.find(ctx_اڳياڙي);
        إذا (مليو_it != prepared_contexts_.end())
            أرجع مليو_it->second;

        CV_LOG_INFO(عدم, "Preparing OpenCL cache configuration for context: " << ctx_اڳياڙي);

        std::string هدف_directory = cache_رستو + ctx_اڳياڙي + "/";
        منطقي نتيجو = utils::fs::isDirectory(هدف_directory);
        إذا (!نتيجو)
        {
            حاول
            {
                CV_LOG_VERBOSE(عدم, 0, "Creating directory: " << هدف_directory);
                إذا (utils::fs::createDirectories(هدف_directory))
                {
                    نتيجو = سچ;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "Can't create directory: " << هدف_directory);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCL program cache directory for context: " << هدف_directory << std::سٽ_پڄاڻي << e.what());
            }
        }
        هدف_directory = نتيجو ? هدف_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_اڳياڙي, هدف_directory));

        إذا (نتيجو && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_اڳياڙي.empty())
        {
            حاول
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_رستو, cleanup_اڳياڙي + "*", entries, خطأ, سچ);
                std::vector<String> remove_entries;
                لكل (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    إذا (0 == name.find(cleanup_اڳياڙي))
                    {
                        إذا (0 == name.find(ctx_اڳياڙي))
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
                        cv::String رستو = utils::fs::join(cache_رستو, name);
                        حاول
                        {
                            utils::fs::remove_all(رستو);
                            CV_LOG_WARNING(عدم, "Removed: " << رستو);
                        }
                        امسك (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(عدم, "Exception during removal of obsolete OpenCL cache directory: " << رستو << std::سٽ_پڄاڻي << e.what());
                        }
                    }
                }
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(عدم, 1, "  Result: " << (هدف_directory.empty() ? std::string("Failed") : هدف_directory));
        أرجع هدف_directory;
    }

    ساكن OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, جديد OpenCLBinaryCacheConfigurator());
    }
};
صنف ٻٽو_پروگرام_File
{
    تعداد { MAX_ENTRIES = 64 };

    تعريفنوع بنانشان صحيح uint32_t;

    بنية CV_DECL_ALIGNED(4) File_سرو
    {
        uint32_t ذريعو_Signature_سائيز;
        //char sourceSignature[];
    };

    بنية CV_DECL_ALIGNED(4) File_جدول
    {
        uint32_t عدد_جو_Entries;
        //uint32_t firstEntryOffset[];
    };

    بنية CV_DECL_ALIGNED(4) File_داخلا
    {
        uint32_t ايندڙ_داخلا_File_وٿي; // 0 for the last entry in chain
        uint32_t ڪنجي_سائيز;
        uint32_t معلومات_سائيز;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت ذريعو_Signature;
    ثابت size_t ذريعو_Signature_سائيز_2;

    std::fstream f;

    uint32_t داخلا_Offsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        أرجع hash & (MAX_ENTRIES - 1);
    }

    مضمن size_t get_File_سائيز()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_سائيز = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        أرجع file_سائيز;
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
    ٻٽو_پروگرام_File(ثابت std::string& fileName, ثابت حرف* ذريعو_Signature_2)
        : fileName_(fileName), ذريعو_Signature(ذريعو_Signature_2), ذريعو_Signature_سائيز_2(ذريعو_Signature ? لڙي_ڊيگهه(ذريعو_Signature) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(ذريعو_Signature != عدم);
        CV_Assert(ذريعو_Signature_سائيز_2 > 0);
        memset(داخلا_Offsets, 0, حجم(داخلا_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        إذا(f.is_open() && get_File_سائيز() > 0)
        {
            منطقي isValid = خطأ;
            حاول
            {
                uint32_t file_ذريعو_Signature_سائيز = readUInt32();
                إذا (file_ذريعو_Signature_سائيز == ذريعو_Signature_سائيز_2)
                {
                    cv::Auto_بفر<حرف> file_ذريعو_Signature(file_ذريعو_Signature_سائيز + 1);
                    f.اقرأ(file_ذريعو_Signature.معلومات(), file_ذريعو_Signature_سائيز);
                    إذا (f.eof())
                    {
                        CV_LOG_ERROR(عدم, "Unexpected EOF");
                    }
                    وإلا إذا (memcmp(ذريعو_Signature_2, file_ذريعو_Signature.معلومات(), file_ذريعو_Signature_سائيز) == 0)
                    {
                        isValid = سچ;
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

    منطقي اقرأ(ثابت std::string& ڪنجي, std::vector<حرف>& buf)
    {
        إذا (!f.is_open())
            أرجع خطأ;

        size_t file_سائيز = get_File_سائيز();
        إذا (file_سائيز == 0)
        {
            CV_LOG_ERROR(عدم, "Invalid file (empty): " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ذريعو_Signature_سائيز = readUInt32();
        CV_Assert(file_ذريعو_Signature_سائيز > 0);
        seekReadRelative(file_ذريعو_Signature_سائيز);

        uint32_t عدد_جو_Entries = readUInt32();
        CV_Assert(عدد_جو_Entries > 0);
        إذا (عدد_جو_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        f.اقرأ((حرف*)&داخلا_Offsets[0], حجم(داخلا_Offsets));
        CV_Assert(!f.fail());

        uint32_t داخلا_انگ = getHash(ڪنجي);

        uint32_t داخلا_وٿي = داخلا_Offsets[داخلا_انگ];
        File_داخلا داخلا_3;
        طالما (داخلا_وٿي > 0)
        {
            seekReadAbsolute(داخلا_وٿي);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&داخلا_3, حجم(داخلا_3));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_ڪنجي(داخلا_3.ڪنجي_سائيز + 1);
            إذا (ڪنجي.size() == داخلا_3.ڪنجي_سائيز)
            {
                إذا (داخلا_3.ڪنجي_سائيز > 0)
                {
                    f.اقرأ(file_ڪنجي.معلومات(), داخلا_3.ڪنجي_سائيز);
                    CV_Assert(!f.fail());
                }
                إذا (memcmp(file_ڪنجي.معلومات(), ڪنجي.c_str(), داخلا_3.ڪنجي_سائيز) == 0)
                {
                    buf.resize(داخلا_3.معلومات_سائيز);
                    f.اقرأ(&buf[0], داخلا_3.معلومات_سائيز);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(عدم, 0, "Read...");
                    أرجع سچ;
                }
            }
            إذا (داخلا_3.ايندڙ_داخلا_File_وٿي == 0)
                اكسر;
            داخلا_وٿي = داخلا_3.ايندڙ_داخلا_File_وٿي;
        }
        أرجع خطأ;
    }

    منطقي اكتب(ثابت std::string& ڪنجي, std::vector<حرف>& buf)
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

        size_t file_سائيز = get_File_سائيز();
        إذا (file_سائيز == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)ذريعو_Signature_سائيز_2);
            f.اكتب(ذريعو_Signature, ذريعو_Signature_سائيز_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(داخلا_Offsets, 0, حجم(داخلا_Offsets));
            f.اكتب((حرف*)داخلا_Offsets, حجم(داخلا_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_سائيز = get_File_سائيز();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ذريعو_Signature_سائيز = readUInt32();
        CV_Assert(file_ذريعو_Signature_سائيز == ذريعو_Signature_سائيز_2);
        seekReadRelative(file_ذريعو_Signature_سائيز);

        uint32_t عدد_جو_Entries = readUInt32();
        CV_Assert(عدد_جو_Entries > 0);
        إذا (عدد_جو_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        size_t جدول_Entries_وٿي = (size_t)f.tellg();
        f.اقرأ((حرف*)&داخلا_Offsets[0], حجم(داخلا_Offsets));
        CV_Assert(!f.fail());

        uint32_t داخلا_انگ = getHash(ڪنجي);

        uint32_t داخلا_وٿي = داخلا_Offsets[داخلا_انگ];
        File_داخلا داخلا_3;
        طالما (داخلا_وٿي > 0)
        {
            seekReadAbsolute(داخلا_وٿي);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&داخلا_3, حجم(داخلا_3));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_ڪنجي(داخلا_3.ڪنجي_سائيز + 1);
            إذا (ڪنجي.size() == داخلا_3.ڪنجي_سائيز)
            {
                إذا (داخلا_3.ڪنجي_سائيز > 0)
                {
                    f.اقرأ(file_ڪنجي.معلومات(), داخلا_3.ڪنجي_سائيز);
                    CV_Assert(!f.fail());
                }
                إذا (0 == memcmp(file_ڪنجي.معلومات(), ڪنجي.c_str(), داخلا_3.ڪنجي_سائيز))
                {
                    // duplicate
                    CV_LOG_VERBOSE(عدم, 0, "Duplicate key ignored: " << fileName_);
                    أرجع خطأ;
                }
            }
            إذا (داخلا_3.ايندڙ_داخلا_File_وٿي == 0)
                اكسر;
            داخلا_وٿي = داخلا_3.ايندڙ_داخلا_File_وٿي;
        }
        seekReadAbsolute(0);
        إذا (داخلا_وٿي > 0)
        {
            seekWriteAbsolute(داخلا_وٿي);
            داخلا_3.ايندڙ_داخلا_File_وٿي = (uint32_t)file_سائيز;
            f.اكتب((حرف*)&داخلا_3, حجم(داخلا_3));
            CV_Assert(!f.fail());
        }
        وإلا
        {
            داخلا_Offsets[داخلا_انگ] = (uint32_t)file_سائيز;
            seekWriteAbsolute(جدول_Entries_وٿي);
            f.اكتب((حرف*)داخلا_Offsets, حجم(داخلا_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_سائيز);
        داخلا_3.ايندڙ_داخلا_File_وٿي = 0;
        داخلا_3.معلومات_سائيز = (uint32_t)buf.size();
        داخلا_3.ڪنجي_سائيز = (uint32_t)ڪنجي.size();
        f.اكتب((حرف*)&داخلا_3, حجم(داخلا_3));
        CV_Assert(!f.fail());
        f.اكتب(ڪنجي.c_str(), داخلا_3.ڪنجي_سائيز);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], داخلا_3.معلومات_سائيز);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(عدم, 0, "Write... (" << buf.size() << " bytes)");
        أرجع سچ;
    }
};
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT



بنية OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحيح device_;  // device index in context
    ocl::قطار قطار_3;
    صحيح useOpenCL_;

محمي:
    Impl() = ميٽيو;

    فراغ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي مليو = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            إذا (dhandle == deviceID)
            {
                device_ = i;
                مليو = سچ;
                اكسر;
            }
        }
        CV_Assert(مليو && "OpenCL device can't work with passed OpenCL context");
    }

    فراغ _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي مليو = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            إذا (d.getImpl() == device.getImpl())
            {
                device_ = i;
                مليو = سچ;
                اكسر;
            }
        }
        CV_Assert(مليو && "OpenCL device can't work with passed OpenCL context");
    }

عام:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_اڻ_استعمال_ٿيل(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        قطار_3 = قطار(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device, ثابت ocl::قطار& قطار_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        قطار_3 = قطار_2;
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        قطار_3 = قطار(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت صحيح device, ثابت ocl::قطار& قطار_2)
        : context_(context)
        , device_(device)
        , قطار_3(قطار_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ثابت Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , قطار_3(other.قطار_3)
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
                    useOpenCL_ = d.موجود();
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

    فراغ سيٽ_Use_Open_CL(منطقي flag)
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
            cv::Auto_تالو lock(getInitializationMutex());
            إذا (!initialized)
            {
                CV_LOG_INFO(عدم, "OpenCL: creating new execution context...");
                حاول
                {
                    Context c = ocl::Context::create(std::string());
                    إذا (c.ndevices())
                    {
                        صحيح deviceId = 0;
                        خودڪار& d = c.device(deviceId);
                        إذا (d.موجود())
                        {
                            خودڪار q = ocl::قطار(c, d);
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
                initialized = سچ;
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
قطار& OpenCLExecutionContext::get_قطار() ثابت
{
    CV_Assert(p);
    أرجع p->قطار_3;
}

منطقي OpenCLExecutionContext::useOpenCL() ثابت
{
    إذا (p)
        أرجع p->useOpenCL();
    أرجع خطأ;
}
فراغ OpenCLExecutionContext::سيٽ_Use_Open_CL(منطقي flag)
{
    CV_Assert(p);
    p->سيٽ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    OpenCLExecutionContext& c = معلومات.oclExecutionContext;
    إذا (!معلومات.oclExecutionContextInitialized)
    {
        معلومات.oclExecutionContextInitialized = سچ;
        إذا (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    أرجع c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    OpenCLExecutionContext& c = معلومات.oclExecutionContext;
    أرجع c;
}

فراغ OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    معلومات.oclExecutionContext = *هذا;
    معلومات.oclExecutionContextInitialized = سچ;
    معلومات.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت قطار q(getContext(), getDevice());
    أرجع clone_With_New_قطار(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار(ثابت ocl::قطار& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != عدم);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    أرجع c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::قطار& قطار_2)
{
    CV_TRACE_FUNCTION();
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, قطار_2);
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
        std::string env_رستو = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        إذا (!env_رستو.empty())
        {
            إذا (env_رستو == "disabled")
            {
                g_isOpenCLAvailable = خطأ;
                g_isOpenCLInitialized = سچ;
                أرجع خطأ;
            }
        }

        cv::Auto_تالو lock(getInitializationMutex());
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
        g_isOpenCLInitialized = سچ;
    }
    أرجع g_isOpenCLAvailable;
}

منطقي useOpenCL()
{
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    إذا (معلومات.useOpenCL < 0)
    {
        حاول
        {
            معلومات.useOpenCL = 0;
            إذا (haveOpenCL())
            {
                خودڪار c = OpenCLExecutionContext::getCurrent();
                معلومات.useOpenCL = c.useOpenCL();
            }
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    أرجع معلومات.useOpenCL > 0;
}

منطقي isOpenCLActivated()
{
    إذا (!g_isOpenCLAvailable)
        أرجع خطأ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    أرجع useOpenCL();
}

فراغ سيٽ_Use_Open_CL(منطقي flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    خودڪار& c = OpenCLExecutionContext::getCurrentRef();
    إذا (!c.empty())
    {
        c.سيٽ_Use_Open_CL(flag);
        معلومات.useOpenCL = c.useOpenCL();
    }
    وإلا
    {
        إذا (!flag)
            معلومات.useOpenCL = 0;
        وإلا
            معلومات.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#جيڪڏهن_وصف_ٿيل HAVE_CLAMDBLAS

صنف AmdBlasHelper
{
عام:
    ساكن AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, جديد AmdBlasHelper())
    }

    منطقي is_موجود() ثابت
    {
        أرجع g_is_Amd_Blas_موجود;
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
            Auto_تالو lock(getInitializationMutex());

            إذا (!g_isAmdBlasInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        g_is_Amd_Blas_موجود = clblas_تياري() == clblasSuccess;
                    }
                    امسك (...)
                    {
                        g_is_Amd_Blas_موجود = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Blas_موجود = خطأ;

                g_isAmdBlasInitialized = سچ;
            }
        }
    }

خاص:
    ساكن منطقي g_isAmdBlasInitialized;
    ساكن منطقي g_is_Amd_Blas_موجود;
};

منطقي AmdBlasHelper::g_is_Amd_Blas_موجود = خطأ;
منطقي AmdBlasHelper::g_isAmdBlasInitialized = خطأ;

منطقي haveAmdBlas()
{
    أرجع AmdBlasHelper::getInstance().is_موجود();
}

#وإلا

منطقي haveAmdBlas()
{
    أرجع خطأ;
}

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_CLAMDFFT

صنف AmdFftHelper
{
عام:
    ساكن AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, جديد AmdFftHelper())
    }

    منطقي is_موجود() ثابت
    {
        أرجع g_is_Amd_Fft_موجود;
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
            Auto_تالو lock(getInitializationMutex());

            إذا (!g_isAmdFftInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_تياري_معلومات(&تياري_معلومات) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_نسخو(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_موجود = سچ;
                    }
                    امسك (ثابت Exception &)
                    {
                        g_is_Amd_Fft_موجود = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Fft_موجود = خطأ;

                g_isAmdFftInitialized = سچ;
            }
        }
    }

خاص:
    ساكن clfft_تياري_معلومات تياري_معلومات;
    ساكن منطقي g_isAmdFftInitialized;
    ساكن منطقي g_is_Amd_Fft_موجود;
};

clfft_تياري_معلومات AmdFftHelper::تياري_معلومات;
منطقي AmdFftHelper::g_is_Amd_Fft_موجود = خطأ;
منطقي AmdFftHelper::g_isAmdFftInitialized = خطأ;

منطقي haveAmdFft()
{
    أرجع AmdFftHelper::getInstance().is_موجود();
}

#وإلا

منطقي haveAmdFft()
{
    أرجع خطأ;
}

#جيڪڏهن_پورو

منطقي haveSVM()
{
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
    أرجع سچ;
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ پڄاڻي_2()
{
    قطار::getDefault().پڄاڻي_2();
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

            initialized = سچ;
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

Platform& Platform::عامل = (ثابت Platform& pl)
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
    pl.p = خالي_اشارو;
}

Platform& Platform::عامل = (Platform&& pl) CV_NOEXCEPT
{
    إذا (هذا != &pl) {
        إذا(p)
            p->release();
        p = pl.p;
        pl.p = خالي_اشارو;
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
ساكن فراغ parseOpenCLVersion(ثابت String &نسخو, صحيح &major, صحيح &minor)
{
    major = minor = 0;
    إذا (10 >= نسخو.length())
        أرجع;
    ثابت حرف *pstr = نسخو.c_str();
    إذا (0 != لڙي_ڀيٽ(pstr, "OpenCL ", 7))
        أرجع;
    size_t ppos = نسخو.find('.', 7);
    إذا (String::npos == ppos)
        أرجع;
    String عارضي_2 = نسخو.substr(7, ppos - 7);
    major = atoi(عارضي_2.c_str());
    عارضي_2 = نسخو.substr(ppos + 1);
    minor = atoi(عارضي_2.c_str());
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
        نسخو_3 = getStrProp(CL_DEVICE_نسخو);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_يادگيري = getBoolProp(CL_DEVICE_HOST_UNIFIED_يادگيري);
        maxComputeUnits_ = getProp<cl_uint, صحيح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_گروهه_سائيز_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_گروهه_سائيز);
        type_ = getProp<cl_device_type, صحيح>(CL_DEVICE_TYPE);
        driver_نسخو_2 = getStrProp(CL_DRIVER_نسخو);
        پتو_بٽ_2 = getProp<cl_uint, صحيح>(CL_DEVICE_پتو_بٽ);

        String device_نسخو = getStrProp(CL_DEVICE_نسخو);
        parseOpenCLVersion(device_نسخو, device_نسخو_Major_2, device_نسخو_Minor_2);

        size_t pos = 0;
        طالما (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            إذا (pos2 == String::npos)
                pos2 = extensions_.size();
            إذا (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_سيٽ.insert(extensionName);
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
        وإلا إذا (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || لڙي_ڳولا(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وإلا إذا (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وإلا
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_گروهه_سائيز = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        إذا (CV_OPENCL_DEVICE_MAX_WORK_گروهه_سائيز > 0)
        {
            ثابت size_t new_max_Work_گروهه_سائيز = std::min(max_Work_گروهه_سائيز_2, CV_OPENCL_DEVICE_MAX_WORK_گروهه_سائيز);
            إذا (new_max_Work_گروهه_سائيز != max_Work_گروهه_سائيز_2)
                CV_LOG_WARNING(عدم, "OpenCL: using workgroup size: " << new_max_Work_گروهه_سائيز << " (was " << max_Work_گروهه_سائيز_2 << ")");
            max_Work_گروهه_سائيز_2 = new_max_Work_گروهه_سائيز;
        }
#إذا 0
        إذا (isExtensionSupported("cl_khr_spir"))
        {
#جيڪڏهن_وصف_نه_ٿيل CL_DEVICE_SPIR_VERSIONS
#وصف_ڪريو CL_DEVICE_SPIR_VERSIONS                     0x40E0
#جيڪڏهن_پورو
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::ٻاهر << spir_versions << std::سٽ_پڄاڻي;
        }
#جيڪڏهن_پورو
    }

    ~Impl()
    {
#جيڪڏهن_وصف_ٿيل _WIN32
        إذا (!cv::__termination)
#جيڪڏهن_پورو
        {
            إذا (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    سانچو<قسم_نالو _TpCL, قسم_نالو Tp_ٻاهريون>
    Tp_ٻاهريون getProp(cl_device_info prop) ثابت
    {
        _TpCL عارضي_2=_TpCL();
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(عارضي_2), &عارضي_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضي_2) ? Tp_ٻاهريون(عارضي_2) : Tp_ٻاهريون();
    }

    منطقي getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool عارضي_2 = CL_FALSE;
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(عارضي_2), &عارضي_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضي_2) ? عارضي_2 != 0 : خطأ;
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
        أرجع extensions_سيٽ.ڳڻپ_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String نسخو_3;
    std::string extensions_;
    صحيح doubleFPConfig_;
    منطقي khr_fp64_support_;
    صحيح halfFPConfig_;
    منطقي khr_fp16_support_;
    منطقي host_Unified_يادگيري;
    صحيح maxComputeUnits_;
    size_t max_Work_گروهه_سائيز_2;
    صحيح type_;
    صحيح پتو_بٽ_2;
    صحيح device_نسخو_Major_2;
    صحيح device_نسخو_Minor_2;
    String driver_نسخو_2;
    String vendorName_;
    صحيح vendorID_;
    منطقي intelSubgroupsSupport_;

    std::سيٽ<std::string> extensions_سيٽ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(فراغ* d)
{
    p = 0;
    سيٽ(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    إذا(p)
        p->addref();
}

Device& Device::عامل = (ثابت Device& d)
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
    d.p = خالي_اشارو;
}

Device& Device::عامل = (Device&& d) CV_NOEXCEPT
{
    إذا (هذا != &d) {
        إذا(p)
            p->release();
        p = d.p;
        d.p = خالي_اشارو;
    }
    أرجع *هذا;
}

Device::~Device()
{
    إذا(p)
        p->release();
}

فراغ Device::سيٽ(فراغ* d)
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

String Device::نسخو() ثابت
{ أرجع p ? p->نسخو_3 : String(); }

String Device::vendorName() ثابت
{ أرجع p ? p->vendorName_ : String(); }

صحيح Device::vendorID() ثابت
{ أرجع p ? p->vendorID_ : 0; }

String Device::Open_CL_C_نسخو() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_OPENCL_C_نسخو) : String(); }

String Device::OpenCLVersion() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_نسخو) : String(); }

صحيح Device::device_نسخو_Major() ثابت
{ أرجع p ? p->device_نسخو_Major_2 : 0; }

صحيح Device::device_نسخو_Minor() ثابت
{ أرجع p ? p->device_نسخو_Minor_2 : 0; }

String Device::driver_نسخو() ثابت
{ أرجع p ? p->driver_نسخو_2 : String(); }

صحيح Device::type() ثابت
{ أرجع p ? p->type_ : 0; }

صحيح Device::پتو_بٽ() ثابت
{ أرجع p ? p->پتو_بٽ_2 : 0; }

منطقي Device::موجود() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_موجود) : خطأ; }

منطقي Device::compiler_موجود() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_COMPILER_موجود) : خطأ; }

منطقي Device::linker_موجود() ثابت
#جيڪڏهن_وصف_ٿيل CL_نسخو_1_2
{ أرجع p ? p->getBoolProp(CL_DEVICE_LINKER_موجود) : خطأ; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جيڪڏهن_پورو

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

size_t Device::global_Mem_Cache_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_سائيز) : 0; }

صحيح Device::globalMemCacheType() ثابت
{ أرجع p ? p->getProp<cl_device_mem_cache_type, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحيح Device::global_Mem_Cache_سٽ_سائيز() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_سائيز) : 0; }

size_t Device::global_Mem_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_سائيز) : 0; }

size_t Device::local_Mem_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_سائيز) : 0; }

صحيح Device::localMemType() ثابت
{ أرجع p ? p->getProp<cl_device_local_mem_type, صحيح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقي Device::host_Unified_يادگيري_2() ثابت
{ أرجع p ? p->host_Unified_يادگيري : خطأ; }

منطقي Device::imageSupport() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : خطأ; }

منطقي Device::image_From_بفر_Support() ثابت
{
    أرجع p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : خطأ;
}

uint Device::imagePitchAlignment() ثابت
{
#جيڪڏهن_وصف_ٿيل CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
}

uint Device::image_بنياد_پتو_Alignment() ثابت
{
#جيڪڏهن_وصف_ٿيل CL_DEVICE_IMAGE_بنياد_پتو_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_بنياد_پتو_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
}

size_t Device::image2_DMax_ويڪر() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ويڪر) : 0; }

size_t Device::image2_DMax_اوچائي() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_اوچائي) : 0; }

size_t Device::image3_DMax_ويڪر() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ويڪر) : 0; }

size_t Device::image3_DMax_اوچائي() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_اوچائي) : 0; }

size_t Device::image3_DMax_اونهائي() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_اونهائي) : 0; }

size_t Device::image_Max_بفر_سائيز() ثابت
#جيڪڏهن_وصف_ٿيل CL_نسخو_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_بفر_سائيز) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جيڪڏهن_پورو

size_t Device::image_Max_Array_سائيز() ثابت
#جيڪڏهن_وصف_ٿيل CL_نسخو_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_سائيز) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جيڪڏهن_پورو

منطقي Device::intelSubgroupsSupport() ثابت
{ أرجع p ? p->intelSubgroupsSupport_ : خطأ; }

صحيح Device::maxClockFrequency() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحيح Device::maxComputeUnits() ثابت
{ أرجع p ? p->maxComputeUnits_ : 0; }

صحيح Device::max_Constant_دليلون() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CONSTANT_دليلون) : 0; }

size_t Device::max_Constant_بفر_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_بفر_سائيز) : 0; }

size_t Device::max_Mem_Alloc_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_سائيز) : 0; }

size_t Device::max_Parameter_سائيز() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_سائيز) : 0; }

صحيح Device::max_Read_Image_دليلون() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_READ_IMAGE_دليلون) : 0; }

صحيح Device::max_Write_Image_دليلون() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WRITE_IMAGE_دليلون) : 0; }

صحيح Device::maxSamplers() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_گروهه_سائيز() ثابت
{ أرجع p ? p->max_Work_گروهه_سائيز_2 : 0; }

صحيح Device::max_Work_شي_Dims() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WORK_شي_DIMENSIONS) : 0; }

فراغ Device::max_Work_شي_Sizes(size_t* sizes) ثابت
{
    إذا(p)
    {
        ثابت صحيح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_شي_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحيح Device::mem_بنياد_Addr_Align() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MEM_بنياد_ADDR_ALIGN) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_CHAR) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_SHORT) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_INT) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_LONG) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_FLOAT) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_DOUBLE) : 0; }

صحيح Device::native_ويڪٽر_ويڪر_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ويڪٽر_ويڪر_HALF) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_CHAR) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_SHORT) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_INT) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_LONG) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_FLOAT) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_DOUBLE) : 0; }

صحيح Device::preferred_ويڪٽر_ويڪر_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ويڪٽر_ويڪر_HALF) : 0; }

size_t Device::printf_بفر_سائيز() ثابت
#جيڪڏهن_وصف_ٿيل CL_نسخو_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_بفر_سائيز) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جيڪڏهن_پورو


size_t Device::profilingTimerResolution() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    خودڪار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        أرجع c.getDevice();
    }

    ساكن Device dummy;
    أرجع dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

سانچو <قسم_نالو Functor, قسم_نالو ObjectType>
مضمن cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, عدم, &required);
    إذا (err != CL_SUCCESS)
        أرجع err;

    param.clear();
    إذا (required > 0)
    {
        Auto_بفر<حرف> buf(required + 1);
        حرف* ptr = buf.معلومات(); // cleanup is not needed
        err = f(obj, name, required, ptr, عدم);
        إذا (err != CL_SUCCESS)
            أرجع err;
        param = ptr;
    }

    أرجع CL_SUCCESS;
}

ساكن فراغ ورهايو(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    إذا (s.size() == 0)
        أرجع;
    std::istringstream ss(s);
    std::string شي;
    طالما (!ss.eof())
    {
        std::getline(ss, شي, delim);
        elems.push_back(شي);
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
    ورهايو(configurationStr, ':', parts);
    إذا (parts.size() > 3)
    {
        CV_LOG_ERROR(عدم, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        أرجع خطأ;
    }
    إذا (parts.size() > 2)
        deviceNameOrID = parts[2];
    إذا (parts.size() > 1)
    {
        ورهايو(parts[1], '|', deviceTypes);
    }
    إذا (parts.size() > 0)
    {
        platform = parts[0];
    }
    أرجع سچ;
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
        isID = سچ;
        لكل (size_t i = 0; i < deviceName.length(); i++)
        {
            إذا (!انگ_آهي(deviceName[i]))
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
        cl_uint انگ_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &انگ_Platforms));

        إذا (انگ_Platforms == 0)
            أرجع عدم;
        platforms.resize((size_t)انگ_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(انگ_Platforms, &platforms[0], &انگ_Platforms));
        platforms.resize(انگ_Platforms);
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
            اذهب not_مليو;
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
        std::string عارضي_Str_Device_Type = deviceTypes[t];
        std::transform(عارضي_Str_Device_Type.begin(), عارضي_Str_Device_Type.end(), عارضي_Str_Device_Type.begin(), details::char_tolower);

        إذا (عارضي_Str_Device_Type == "gpu" || عارضي_Str_Device_Type == "dgpu" || عارضي_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        وإلا إذا (عارضي_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        وإلا إذا (عارضي_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وإلا إذا (عارضي_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        وإلا
        {
            CV_LOG_ERROR(عدم, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            اذهب not_مليو;
        }

        std::vector<cl_device_id> devices;
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint ڳڻپ_2 = 0;
            cl_int حالت = clGetDeviceIDs(*currentPlatform, deviceType, 0, عدم, &ڳڻپ_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_مليو))
            {
                CV_OCL_DBG_CHECK_نتيجو(حالت, "clGetDeviceIDs get count");
            }
            إذا (ڳڻپ_2 == 0)
                استمر;
            size_t بنياد = devices.size();
            devices.resize(بنياد + ڳڻپ_2);
            حالت = clGetDeviceIDs(*currentPlatform, deviceType, ڳڻپ_2, &devices[بنياد], &ڳڻپ_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_مليو))
            {
                CV_OCL_DBG_CHECK_نتيجو(حالت, "clGetDeviceIDs get IDs");
            }
        }

        لكل (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : سچ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = سچ;
            إذا(عارضي_Str_Device_Type == "dgpu" || عارضي_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_يادگيري, حجم(isIGPU), &isIGPU, عدم));
                useGPU = عارضي_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            إذا ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                أرجع devices[i];
            }
        }
    }

not_مليو:
    إذا (configuration.empty())
        أرجع عدم; // suppress messages on stderr

    std::ostringstream نياپو;
    نياپو << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::سٽ_پڄاڻي
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::سٽ_پڄاڻي
        << "    Device types:";
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
        نياپو << ' ' << deviceTypes[t];

    نياپو << std::سٽ_پڄاڻي << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(عدم, نياپو.str());
    أرجع عدم;
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
نطاق svm {

تعداد AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_بفر = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_بفر = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_بفر_نقاب = 3 << 16,
        OPENCL_SVM_بفر_MAP = 4 << 16
};

ساكن منطقي checkForceSVMUmatUsage()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", خطأ);
        initialized = سچ;
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
        initialized = سچ;
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
        initialized = سچ;
    }
    أرجع force;
}
// see SVMCapabilities
ساكن بنانشان صحيح get_SVMCapabilities_نقاب()
{
    ساكن منطقي initialized = خطأ;
    ساكن بنانشان صحيح نقاب = 0;
    إذا (!initialized)
    {
        ثابت std::string env_قدر = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        إذا (env_قدر.empty())
        {
            أرجع ~0U; // all bits 1
        }
        نقاب = atoi(env_قدر.c_str());
        initialized = سچ;
    }
    أرجع نقاب;
}
} // namespace
#جيڪڏهن_پورو

ساكن size_t get_پروگرام_ڳڻپ_حد_بندي()
{
    ساكن منطقي initialized = خطأ;
    ساكن size_t ڳڻپ_2 = 0;
    إذا (!initialized)
    {
        ڳڻپ_2 = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = سچ;
    }
    أرجع ڳڻپ_2;
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
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        , svmInitialized(خطأ)
#جيڪڏهن_پورو
    {
        إذا (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_تالو lock(cv::getInitializationMutex());
        خودڪار& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = هذا;
    }

    ~Impl()
    {
#جيڪڏهن_وصف_ٿيل _WIN32
        إذا (!cv::__termination)
#جيڪڏهن_پورو
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
            cv::Auto_تالو lock(cv::getInitializationMutex());
            خودڪار& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = عدم;
        }
    }

    فراغ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_انگ_DEVICES, حجم(ndevices), &ndevices, عدم));
        CV_Assert(ndevices > 0);

        cv::Auto_بفر<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_سائيز = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_سائيز));
        CV_CheckEQ(devices_ret_سائيز, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        لكل (بنانشان i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    فراغ __init_buffer_pools();  // w/o synchronization
    فراغ init_بفر_pools() ثابت
    {
        إذا (!بفر_Pool_2)
        {
            cv::Auto_تالو lock(cv::getInitializationMutex());
            إذا (!بفر_Pool_2)
            {
                const_cast<Impl*>(هذا)->__init_buffer_pools();
            }
        }
    }
عام:
    ساكن Impl* ڳوليو_Context(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_تالو lock(cv::getInitializationMutex());
        خودڪار& container = getGlobalContainer();
        إذا (configuration.empty() && !container.empty())
            أرجع container[0];
        لكل (خودڪار it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            إذا (i && i->configuration == configuration)
            {
                أرجع i;
            }
        }
        أرجع عدم;
    }

    ساكن Impl* ڳوليو_Or_Create_Context(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        إذا (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            إذا (!c.empty())
                configuration = c;
        }
        Impl* impl = ڳوليو_Context(configuration);
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
            ميٽيو impl;
            أرجع عدم;
        }
        امسك (...)
        {
            ميٽيو impl;
            ارم;
        }
    }

    ساكن Impl* ڳوليو_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (فراغ*)h);
        Impl* impl = ڳوليو_Context(configuration);
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
            ميٽيو impl;
            ارم;
        }
    }

    ساكن Impl* ڳوليو_Or_Create_Context(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (فراغ*)d);
        Impl* impl = ڳوليو_Context(configuration);
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
            ميٽيو impl;
            ارم;
        }
    }

    فراغ سيٽ_Default()
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
        cl_int حالت;

        handle = clCreateContext(prop, nd, &d, 0, 0, &حالت);
        CV_OCL_DBG_CHECK_نتيجو(حالت, "clCreateContext");

        منطقي ok = handle != 0 && حالت == CL_SUCCESS;
        إذا( ok )
        {
            devices.resize(nd);
            devices[0].سيٽ(d);
        }
        وإلا
            handle = عدم;
    }

    پروگرام_2 getProg(ثابت پروگرام_ذريعو& src, ثابت String& buildflags, String& errmsg);

    فراغ unloadProg(پروگرام_2& prog)
    {
        cv::Auto_تالو lock(پروگرام_cache_mutex);
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

    std::string& get_اڳياڙي_String()
    {
        إذا (اڳياڙي.empty())
        {
            cv::Auto_تالو lock(پروگرام_cache_mutex);
            إذا (اڳياڙي.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحيح بٽ_2 = d.پتو_بٽ();
                إذا (بٽ_2 > 0 && بٽ_2 != 64)
                    اڳياڙي = cv::format("%d-bit--", بٽ_2);
                اڳياڙي += d.vendorName() + "--" + d.name() + "--" + d.driver_نسخو();
                // sanitize chars
                لكل (size_t i = 0; i < اڳياڙي.size(); i++)
                {
                    حرف c = اڳياڙي[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        اڳياڙي[i] = '_';
                    }
                }
            }
        }
        أرجع اڳياڙي;
    }

    std::string& get_اڳياڙي_بنياد()
    {
        إذا (اڳياڙي_بنياد.empty())
        {
            cv::Auto_تالو lock(پروگرام_cache_mutex);
            إذا (اڳياڙي_بنياد.empty())
            {
                ثابت Device& d = devices[0];
                صحيح بٽ_2 = d.پتو_بٽ();
                إذا (بٽ_2 > 0 && بٽ_2 != 64)
                    اڳياڙي_بنياد = cv::format("%d-bit--", بٽ_2);
                اڳياڙي_بنياد += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                لكل (size_t i = 0; i < اڳياڙي_بنياد.size(); i++)
                {
                    حرف c = اڳياڙي_بنياد[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        اڳياڙي_بنياد[i] = '_';
                    }
                }
            }
        }
        أرجع اڳياڙي_بنياد;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحيح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string اڳياڙي;
    std::string اڳياڙي_بنياد;

    cv::Mutex پروگرام_cache_mutex;
    تعريفنوع std::map<std::string, پروگرام_2> phash_t;
    phash_t phash;
    تعريفنوع std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> بفر_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> بفر_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_بفر_Pool() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_2);
        أرجع *بفر_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_بفر_Pool_Host_Ptr() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_Host_Ptr);
        أرجع *بفر_Pool_Host_Ptr.get();
    }

    std::map<std::type_اشاريو, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    فراغ سيٽ_User_Context(std::type_اشاريو typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::Auto_تالو lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_اشاريو typeId) {
        cv::Auto_تالو lock(userContextMutex);
        خودڪار it = userContextStorage.find(typeId);
        إذا (it != userContextStorage.end())
            أرجع it->second;
        وإلا
            أرجع خالي_اشارو;
    }

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
    منطقي svmInitialized;
    منطقي svm_موجود;
    منطقي svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    فراغ svmInit()
    {
        CV_Assert(handle != عدم);
        ثابت Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((فراغ)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int حالت = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, حجم(deviceCaps), &deviceCaps, عدم);
        إذا (حالت != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", حالت);
            اذهب noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (صحيح)deviceCaps);
        CV_Assert(((فراغ)0, CL_DEVICE_SVM_COARSE_GRAIN_بفر == CL_DEVICE_SVM_COARSE_GRAIN_بفر_AMD)); // Check assumption
        svmCapabilities.قدر_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_FINE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.قدر_2 &= svm::get_SVMCapabilities_نقاب();
        إذا (svmCapabilities.قدر_2 == 0)
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
                cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
                إذا (CL_SUCCESS != cl_قطار_اندر_وجهو_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_پڄاڻي(q);
                حاول
                {
                    ((صحيح*)ptr)[0] = 100;
                }
                امسك (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = سچ;
                }
                إذا (CL_SUCCESS != cl_قطار_اندر_وجهو_SVMUnmap(q, ptr, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_پڄاڻي(q);
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
            svmFunctions.fn_cl_سيٽ_Kernel_Arg_SVMPointer = cl_سيٽ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMemcpy = cl_قطار_اندر_وجهو_SVMMemcpy;
            svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMem_Fill = cl_قطار_اندر_وجهو_SVMMem_Fill;
            svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMap = cl_قطار_اندر_وجهو_SVMMap;
            svmFunctions.fn_cl_قطار_اندر_وجهو_SVMUnmap = cl_قطار_اندر_وجهو_SVMUnmap;
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
                CV_OCL_CHECK(حالت = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, حجم(cl_platform_id), &p, عدم));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_سيٽ_Kernel_Arg_SVMPointer = (cl_سيٽ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMemcpy = (cl_قطار_اندر_وجهو_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMem_Fill = (cl_قطار_اندر_وجهو_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_قطار_اندر_وجهو_SVMMap = (cl_قطار_اندر_وجهو_SVMMap_AMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_قطار_اندر_وجهو_SVMUnmap = (cl_قطار_اندر_وجهو_SVMUnmap_AMD_fn)cl_Get_Extension_Function_پتو_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                اذهب noSVM;
            }
        }

        svm_موجود = سچ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = سچ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        أرجع;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_موجود = خطأ;
        svmEnabled = خطأ;
        svmCapabilities.قدر_2 = 0;
        svmInitialized = سچ;
        svmFunctions.fn_clSVMAlloc = عدم;
        أرجع;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> بفر_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_بفر_Pool_SVM() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_SVM);
        أرجع *بفر_Pool_SVM.get();
    }
#جيڪڏهن_پورو

    دوست صنف پروگرام_2;
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
    p = Impl::ڳوليو_Or_Create_Context(std::string());
    إذا (p && p->handle)
        أرجع سچ;
    release();
    أرجع خطأ;
}

// deprecated
منطقي Context::create(صحيح dtype)
{
    إذا( !haveOpenCL() )
        أرجع خطأ;
    release();
    إذا (dtype == CL_DEVICE_TYPE_DEFAULT || (بنانشان)dtype == (بنانشان)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::ڳوليو_Or_Create_Context("");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::ڳوليو_Or_Create_Context(":GPU:");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::ڳوليو_Or_Create_Context(":CPU:");
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

Context& Context::عامل = (ثابت Context& c)
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
    c.p = خالي_اشارو;
}

Context& Context::عامل = (Context&& c) CV_NOEXCEPT
{
    إذا (هذا != &c) {
        إذا(p)
            p->release();
        p = c.p;
        c.p = خالي_اشارو;
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
    خودڪار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خودڪار& ctx = c.getContext();
        أرجع ctx;
    }

    ساكن Context dummy;
    أرجع dummy;
}

پروگرام_2 Context::getProg(ثابت پروگرام_ذريعو& prog,
                         ثابت String& buildopts, String& errmsg)
{
    أرجع p ? p->getProg(prog, buildopts, errmsg) : پروگرام_2();
}

فراغ Context::unloadProg(پروگرام_2& prog)
{
    إذا (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(فراغ* context)
{
    Context ctx;
    ctx.p = Impl::ڳوليو_Or_Create_Context((cl_context)context);
    أرجع ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::ڳوليو_Or_Create_Context(device);
    أرجع ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::ڳوليو_Or_Create_Context(configuration);
    أرجع ctx;
}

فراغ* Context::getOpenCLContextProperty(صحيح propertyId) ثابت
{
    إذا (p == عدم)
        أرجع خالي_اشارو;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, عدم, &size));
    std::vector<cl_context_properties> prop(size / حجم(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.معلومات(), عدم));
    لكل (size_t i = 0; i < prop.size(); i += 2)
    {
        إذا (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(عدم, "OpenCL: found context property=" << propertyId << ") => " << (فراغ*)prop[i + 1]);
            أرجع (فراغ*)prop[i + 1];
        }
    }
    أرجع خالي_اشارو;
}

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
منطقي Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmEnabled;
}
فراغ Context::سيٽ_Use_SVM(منطقي enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    إذا (enabled && !i->svm_موجود)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#وإلا
منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سيٽ_Use_SVM(منطقي enabled) { CV_Assert(!enabled); }
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
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
        أرجع سچ;
    إذا (checkDisableSVMUMatUsage())
        أرجع خطأ;
    إذا ((usageFlags & USAGE_ALLOCATE_SHARED_يادگيري) != 0)
        أرجع سچ;
    أرجع خطأ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#جيڪڏهن_پورو // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

فراغ Context::سيٽ_User_Context(std::type_اشاريو typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->سيٽ_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاريو typeId)
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
    Auto_بفر<حرف> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.معلومات(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.معلومات();
}

/*
// Attaches OpenCL context to OpenCV
*/
فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID)
{
    خودڪار ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
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

    منطقي platform_موجود = خطأ;

    // check if external platformName contained in list of available platforms in OpenCV
    لكل (بنانشان صحيح i = 0; i < cnt; i++)
    {
        String موجود_Platform_Name;
        get_platform_name(platforms[i], موجود_Platform_Name);
        // external platform is found in the list of available platforms
        إذا (platformName == موجود_Platform_Name)
        {
            platform_موجود = سچ;
            اكسر;
        }
    }

    إذا (!platform_موجود)
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

    خودڪار clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

بنية قطار::Impl
{
    مضمن فراغ __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_قطار = خطأ;
    }

    Impl(cl_command_قطار q)
    {
        __init();
        handle = q;

        cl_command_قطار_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_PROPERTIES, حجم(cl_command_قطار_properties), &props, عدم));
        is_Profiling_قطار = !!(props & CL_قطار_PROFILING_ENABLE);
    }

    Impl(cl_command_قطار q, منطقي is_Profiling_قطار_2)
    {
        __init();
        handle = q;
        is_Profiling_قطار = is_Profiling_قطار_2;
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
        cl_command_قطار_properties props = withProfiling ? CL_قطار_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_قطار(ch, dh, props, &retval), retval);
        is_Profiling_قطار = withProfiling;
    }

    ~Impl()
    {
#جيڪڏهن_وصف_ٿيل _WIN32
        إذا (!cv::__termination)
#جيڪڏهن_پورو
        {
            إذا(handle)
            {
                CV_OCL_DBG_CHECK(cl_پڄاڻي(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_قطار(handle));
                handle = عدم;
            }
        }
    }

    ثابت cv::ocl::قطار& get_Profiling_قطار(ثابت cv::ocl::قطار& self)
    {
        إذا (is_Profiling_قطار)
            أرجع self;

        إذا (profiling_قطار.ptr())
            أرجع profiling_قطار;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_CONTEXT, حجم(cl_context), &ctx, عدم));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_DEVICE, حجم(cl_device_id), &device, عدم));

        cl_int نتيجو = CL_SUCCESS;
        cl_command_قطار_properties props = CL_قطار_PROFILING_ENABLE;
        cl_command_قطار q = cl_Create_Command_قطار(ctx, device, props, &نتيجو);
        CV_OCL_DBG_CHECK_نتيجو(نتيجو, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        قطار قطار_2;
        قطار_2.p = جديد Impl(q, سچ);
        profiling_قطار = قطار_2;

        أرجع profiling_قطار;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_قطار handle;
    منطقي is_Profiling_قطار;
    cv::ocl::قطار profiling_قطار;
};

قطار::قطار() CV_NOEXCEPT
{
    p = 0;
}

قطار::قطار(ثابت Context& c, ثابت Device& d)
{
    p = 0;
    create(c, d);
}

قطار::قطار(ثابت قطار& q)
{
    p = q.p;
    إذا(p)
        p->addref();
}

قطار& قطار::عامل = (ثابت قطار& q)
{
    Impl* newp = (Impl*)q.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

قطار::قطار(قطار&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = خالي_اشارو;
}

قطار& قطار::عامل = (قطار&& q) CV_NOEXCEPT
{
    إذا (هذا != &q) {
        إذا(p)
            p->release();
        p = q.p;
        q.p = خالي_اشارو;
    }
    أرجع *هذا;
}

قطار::~قطار()
{
    إذا(p)
        p->release();
}

منطقي قطار::create(ثابت Context& c, ثابت Device& d)
{
    إذا(p)
        p->release();
    p = جديد Impl(c, d);
    أرجع p->handle != 0;
}

فراغ قطار::پڄاڻي_2()
{
    إذا(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_پڄاڻي(p->handle));
    }
}

ثابت قطار& قطار::get_Profiling_قطار() ثابت
{
    CV_Assert(p);
    أرجع p->get_Profiling_قطار(*هذا);
}

فراغ* قطار::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

قطار& قطار::getDefault()
{
    خودڪار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خودڪار& q = c.get_قطار();
        أرجع q;
    }
    ساكن قطار dummy;
    أرجع dummy;
}

ساكن cl_command_قطار get_قطار(ثابت قطار& q)
{
    cl_command_قطار qq = (cl_command_قطار)q.ptr();
    إذا(!qq)
        qq = (cl_command_قطار)قطار::getDefault().ptr();
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
    أرجع KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.ڪل()*m.elem_سائيز());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

بنية Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت پروگرام_2& prog) :
        refcount(1), handle(عدم), is_اندريون_ترقي(خطأ), is_Async_لاڳيتو(خطأ), nu(0)
    {
        cl_پروگرام ph = (cl_پروگرام)prog.ptr();
        cl_int retval = 0;
        name = kname;
        إذا (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_عارضي_Dst_UMats = خطأ;
        have_عارضي_Src_UMats = خطأ;
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
                    u[i]->flags |= UMat_معلومات::ASYNC_CLEANUP;
                    حاول
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    امسك(ثابت std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(عدم, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = سچ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_عارضي_Dst_UMats = خطأ;
        have_عارضي_Src_UMats = خطأ;
        CV_Assert(!exceptionOccurred);
    }

    فراغ جوڙيو_UMat(ثابت UMat& m, منطقي dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        إذا(dst && m.u->عارضي_UMat())
            have_عارضي_Dst_UMats = سچ;
        إذا(m.u->original_UMat_معلومات == عدم && m.u->عارضي_UMat())
            have_عارضي_Src_UMats = سچ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    فراغ رجسٽر_Image_دليل(صحيح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        إذا (arg < (صحيح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_اندريون_ترقي, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    فراغ finit(cl_event e)
    {
        CV_اڻ_استعمال_ٿيل(e);
        is_اندريون_ترقي = خطأ;
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

    منطقي لاڳيتو(صحيح dims, size_t _globalsize[], size_t _localsize[],
            منطقي متزامن, int64* وقت_NS, ثابت قطار& q);

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
    UMat_معلومات* u[MAX_ARRS];
    منطقي is_اندريون_ترقي;
    منطقي is_Async_لاڳيتو;  // true if kernel was scheduled in async mode
    صحيح nu;
    std::vector<Image2D> shadow_images;
    منطقي have_عارضي_Dst_UMats;
    منطقي have_عارضي_Src_UMats;
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

Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_ذريعو& src,
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

Kernel& Kernel::عامل = (ثابت Kernel& k)
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
    k.p = خالي_اشارو;
}

Kernel& Kernel::عامل = (Kernel&& k) CV_NOEXCEPT
{
    إذا (هذا != &k) {
        إذا(p)
            p->release();
        p = k.p;
        k.p = خالي_اشارو;
    }
    أرجع *هذا;
}

Kernel::~Kernel()
{
    إذا(p)
        p->release();
}

منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_2& prog)
{
    إذا(p)
        p->release();
    p = جديد Impl(kname, prog);
    إذا(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#جيڪڏهن_وصف_ٿيل CV_OPENCL_لاڳيتو_ASSERT // check kernel compilation fails
    CV_Assert(p);
#جيڪڏهن_پورو
    أرجع p != 0;
}

منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_ذريعو& src,
                    ثابت String& buildopts, String* errmsg)
{
    إذا(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    إذا( !errmsg ) errmsg = &tempmsg;
    ثابت پروگرام_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
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

ساكن cv::String dump_قدر(size_t sz, ثابت فراغ* p)
{
    إذا (!p)
        أرجع "NULL";
    إذا (sz == 2)
        أرجع cv::format("%d / %uu / 0x%04x", *(ننڍو*)p, *(بنانشان ننڍو*)p, *(ننڍو*)p);
    إذا (sz == 4)
        أرجع cv::format("%d / %uu / 0x%08x / %g", *(صحيح*)p, *(صحيح*)p, *(صحيح*)p, *(عائم*)p);
    إذا (sz == 8)
        أرجع cv::format("%lld / %lluu / 0x%16llx / %g", *(ڊگهو ڊگهو*)p, *(ڊگهو ڊگهو*)p, *(ڊگهو ڊگهو*)p, *(مزدوج*)p);
    أرجع cv::format("%p", p);
}

صحيح Kernel::سيٽ(صحيح i, ثابت فراغ* value, size_t sz)
{
    إذا (!p || !p->handle)
        أرجع -1;
    إذا (i < 0)
        أرجع i;
    إذا( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحيح)i, (صحيح)sz, dump_قدر(sz, value).c_str()).c_str());
    إذا (retval != CL_SUCCESS)
        أرجع -1;
    أرجع i+1;
}

صحيح Kernel::سيٽ(صحيح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحيح res = سيٽ(i, &h, حجم(h));
    إذا (res >= 0)
        p->رجسٽر_Image_دليل(i, image2D);
    أرجع res;
}

صحيح Kernel::سيٽ(صحيح i, ثابت UMat& m)
{
    أرجع سيٽ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساكن منطقي isCudaBacked(ثابت UMat_معلومات* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    أرجع u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحيح Kernel::سيٽ(صحيح i, ثابت KernelArg& arg)
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
    cl_int حالت = 0;
    إذا( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        منطقي ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        إذا (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)عدم;
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحيح)i).c_str());
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

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_معلومات_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_معلومات_Ptr);
#إذا 1 // TODO
            حالت = svmFns->fn_cl_سيٽ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_معلومات_Ptr);
#وإلا
            حالت = svmFns->fn_cl_سيٽ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_معلومات_Ptr);
#جيڪڏهن_پورو
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحيح)i, (فراغ*)svm_معلومات_Ptr).c_str());
        }
        وإلا
#جيڪڏهن_پورو
        {
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحيح)i, (فراغ*)h).c_str());
        }

        إذا (ptronly)
        {
            i++;
        }
        وإلا إذا( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u2d.قدم), &u2d.قدم);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u2d.قدم).c_str());
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u2d.offset).c_str());
            i += 3;

            إذا( !(arg.flags & KernelArg::NO_سائيز) )
            {
                صحيح کانن_ڳڻپ = u2d.کانن_ڳڻپ*arg.wscale/arg.iwscale;
                حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u2d.صفون), &u2d.صفون);
                CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u2d.صفون).c_str());
                حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(کانن_ڳڻپ), &کانن_ڳڻپ);
                CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)کانن_ڳڻپ).c_str());
                i += 2;
            }
        }
        وإلا
        {
            UMat3D u3d(*arg.m);
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.slicestep).c_str());
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.قدم), &u3d.قدم);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u3d.قدم).c_str());
            حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+3), (صحيح)u3d.offset).c_str());
            i += 4;
            إذا( !(arg.flags & KernelArg::NO_سائيز) )
            {
                صحيح کانن_ڳڻپ = u3d.کانن_ڳڻپ*arg.wscale/arg.iwscale;
                حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u3d.slices).c_str());
                حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.صفون), &u3d.صفون);
                CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.صفون).c_str());
                حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.کانن_ڳڻپ), &کانن_ڳڻپ);
                CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)کانن_ڳڻپ).c_str());
                i += 3;
            }
        }
        p->جوڙيو_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        أرجع i;
    }
    حالت = cl_سيٽ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_نتيجو(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحيح)i, (صحيح)arg.sz, (فراغ*)arg.obj).c_str());
    أرجع i+1;
}

منطقي Kernel::لاڳيتو(صحيح dims, size_t _globalsize[], size_t _localsize[],
                 منطقي متزامن, ثابت قطار& q)
{
    إذا (!p)
        أرجع خطأ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t ڪل = 1;
    CV_Assert(_globalsize != عدم);
    لكل (صحيح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحيح)(i>0)) : 1;
        CV_Assert( val > 0 );
        ڪل *= _globalsize[i];
        إذا (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_مٿي(_globalsize[i], (بنانشان صحيح)val) * val;
    }
    CV_Assert(ڪل > 0);

    أرجع p->لاڳيتو(dims, globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::لاڳيتو_2(صحيح dims, size_t _globalsize[], size_t _localsize[],
                  منطقي متزامن, ثابت قطار& q)
{
    CV_Assert(p);
    أرجع p->لاڳيتو(dims, _globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::Impl::لاڳيتو(صحيح dims, size_t globalsize[], size_t localsize[],
        منطقي متزامن, int64* وقت_NS, ثابت قطار& q)
{
    CV_INSTRUMENT_REGION_OPENCL_لاڳيتو(name.c_str());

    إذا (!handle)
    {
        CV_LOG_ERROR(عدم, "OpenCL kernel has zero handle: " << name);
        أرجع خطأ;
    }

    إذا (is_Async_لاڳيتو)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_لاڳيتو = !متزامن;

    إذا (is_اندريون_ترقي)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#إذا CV_OPENCL_SYNC_لاڳيتو_KERNELS
    متزامن = سچ;
#جيڪڏهن_پورو

    cl_command_قطار qq = get_قطار(q);
    إذا (have_عارضي_Dst_UMats)
        متزامن = سچ;
    إذا (have_عارضي_Src_UMats)
        متزامن = سچ;
    إذا (وقت_NS)
        متزامن = سچ;
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_اندر_وجهو_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           عدم, globalsize, localsize, 0, 0,
                                           (متزامن && !وقت_NS) ? 0 : &asyncEvent);
#إذا !CV_OPENCL_SHOW_لاڳيتو_KERNELS
    إذا (retval != CL_SUCCESS)
#جيڪڏهن_پورو
    {
        cv::String نياپو = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحيح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        متزامن ? "true" : "false"
                        );
        إذا (retval != CL_SUCCESS)
        {
            نياپو = CV_OCL_API_ERROR_نياپو(retval, نياپو.c_str());
        }
#إذا CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_نتيجو(retval, نياپو.c_str());
#وإلا
        لکو("%s\n", نياپو.c_str());
        fflush(stdout);
#جيڪڏهن_پورو
    }
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پڄاڻي(qq));
        إذا (وقت_NS)
        {
            إذا (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong شروعات_وقت, stop_وقت;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_شروعات, حجم(شروعات_وقت), &شروعات_وقت, عدم));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_پڇاڙي, حجم(stop_وقت), &stop_وقت, عدم));
                *وقت_NS = (int64)(stop_وقت - شروعات_وقت);
            }
            وإلا
            {
                *وقت_NS = -1;
            }
        }
        cleanupUMats();
    }
    وإلا
    {
        addref();
        is_اندريون_ترقي = سچ;
        CV_OCL_CHECK(cl_سيٽ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, هذا));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

منطقي Kernel::لاڳيتو_Task(منطقي متزامن, ثابت قطار& q)
{
    إذا(!p || !p->handle || p->is_اندريون_ترقي)
        أرجع خطأ;

    cl_command_قطار qq = get_قطار(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_اندر_وجهو_Task(qq, p->handle, 0, 0, متزامن ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), متزامن ? "true" : "false").c_str());
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پڄاڻي(qq));
        p->cleanupUMats();
    }
    وإلا
    {
        p->addref();
        p->is_اندريون_ترقي = سچ;
        CV_OCL_CHECK(cl_سيٽ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

int64 Kernel::لاڳيتو_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q_)
{
    CV_Assert(p && p->handle && !p->is_اندريون_ترقي);
    قطار q = q_.ptr() ? q_ : قطار::getDefault();
    CV_Assert(q.ptr());
    q.پڄاڻي_2(); // call clFinish() on base queue
    قطار profiling_قطار_2 = q.get_Profiling_قطار();
    int64 وقت_Ns = -1;
    منطقي res = p->لاڳيتو(dims, globalsize, localsize, سچ, &وقت_Ns, profiling_قطار_2);
    أرجع res ? وقت_Ns : -1;
}

size_t Kernel::work_گروهه_سائيز() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروهه_Info(p->handle, dev, CL_KERNEL_WORK_گروهه_سائيز, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتيجو(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_گروهه_سائيز_گڻو() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروهه_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_گروهه_سائيز_گڻو, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتيجو(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

منطقي Kernel::compile_Work_گروهه_سائيز(size_t wsz[]) ثابت
{
    إذا(!p || !p->handle || !wsz)
        أرجع 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروهه_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_گروهه_سائيز, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_نتيجو(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS;
}

size_t Kernel::local_Mem_سائيز() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروهه_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_سائيز, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتيجو(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    أرجع حالت == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

بنية پروگرام_ذريعو::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    تعداد قسم_4 {
        پروگرام_ذريعو_ڪوڊ = 0,
        پروگرام_BINARIES,
        پروگرام_SPIR,
        پروگرام_SPIRV
    } قسم_5;

    Impl(ثابت String& src)
    {
        init(پروگرام_ذريعو_ڪوڊ, cv::String(), cv::String());
        init_From_ذريعو(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& ڪوڊ_Str, ثابت String& ڪوڊ_Hash)
    {
        init(پروگرام_ذريعو_ڪوڊ, module, name);
        init_From_ذريعو(ڪوڊ_Str, ڪوڊ_Hash);
    }

    /// reset fields
    فراغ init(تعداد قسم_4 قسم, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        قسم_5 = قسم;
        module_ = module;
        name_ = name;

        ذريعو_Addr = عدم;
        ذريعو_سائيز = 0;
        isHashUpdated = خطأ;
    }

    فراغ init_From_ذريعو(ثابت String& ڪوڊ_Str, ثابت String& ڪوڊ_Hash)
    {
        ڪوڊ_Str_2 = ڪوڊ_Str;
        ذريعو_Hash = ڪوڊ_Hash;
        إذا (ذريعو_Hash.empty())
        {
            updateHash();
        }
        وإلا
        {
            isHashUpdated = سچ;
        }
    }

    فراغ updateHash(ثابت حرف* hashStr = عدم)
    {
        إذا (hashStr)
        {
            ذريعو_Hash = cv::String(hashStr);
            isHashUpdated = سچ;
            أرجع;
        }
        uint64 hash = 0;
        بدل (قسم_5)
        {
        حالة پروگرام_ذريعو_ڪوڊ:
            إذا (ذريعو_Addr)
            {
                CV_Assert(ڪوڊ_Str_2.empty());
                hash = crc64(ذريعو_Addr, ذريعو_سائيز); // static storage
            }
            وإلا
            {
                CV_Assert(!ڪوڊ_Str_2.empty());
                hash = crc64((uchar*)ڪوڊ_Str_2.c_str(), ڪوڊ_Str_2.size());
            }
            اكسر;
        حالة پروگرام_BINARIES:
        حالة پروگرام_SPIR:
        حالة پروگرام_SPIRV:
            hash = crc64(ذريعو_Addr, ذريعو_سائيز);
            اكسر;
        افتراضي:
            CV_Error(Error::Sts_اندروني, "Internal error");
        }
        ذريعو_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = سچ;
    }

    Impl(تعداد قسم_4 قسم,
            ثابت String& module, ثابت String& name,
            ثابت بنانشان حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(قسم, module, name);

        ذريعو_Addr = binary;
        ذريعو_سائيز = size;

        buildOptions_ = buildOptions;
    }

    ساكن پروگرام_ذريعو from_ذريعو_With_Static_Lifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* ذريعو_ڪوڊ_Static_Str, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ذريعو نتيجو;
        نتيجو.p = جديد Impl(پروگرام_ذريعو_ڪوڊ, module, name,
                (ثابت بنانشان حرف*)ذريعو_ڪوڊ_Static_Str, لڙي_ڊيگهه(ذريعو_ڪوڊ_Static_Str), buildOptions);
        نتيجو.p->updateHash(hashStaticStr);
        أرجع نتيجو;
    }

    ساكن پروگرام_ذريعو from_ٻٽو(ثابت String& module, ثابت String& name,
            ثابت بنانشان حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ذريعو نتيجو;
        نتيجو.p = جديد Impl(پروگرام_BINARIES, module, name, binary, size, buildOptions);
        أرجع نتيجو;
    }

    ساكن پروگرام_ذريعو fromSPIR(ثابت String& module, ثابت String& name,
            ثابت بنانشان حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ذريعو نتيجو;
        نتيجو.p = جديد Impl(پروگرام_SPIR, module, name, binary, size, buildOptions);
        أرجع نتيجو;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String ڪوڊ_Str_2; // PROGRAM_SOURCE_CODE only

    ثابت بنانشان حرف* ذريعو_Addr;
    size_t ذريعو_سائيز;

    cv::String buildOptions_;

    String ذريعو_Hash;
    منطقي isHashUpdated;

    دوست بنية پروگرام_2::Impl;
    دوست بنية internal::پروگرام_داخلا;
    دوست بنية Context::Impl;
};


پروگرام_ذريعو::پروگرام_ذريعو() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_ذريعو::پروگرام_ذريعو(ثابت String& module, ثابت String& name, ثابت String& ڪوڊ_Str, ثابت String& ڪوڊ_Hash)
{
    p = جديد Impl(module, name, ڪوڊ_Str, ڪوڊ_Hash);
}

پروگرام_ذريعو::پروگرام_ذريعو(ثابت حرف* prog)
{
    p = جديد Impl(prog);
}

پروگرام_ذريعو::پروگرام_ذريعو(ثابت String& prog)
{
    p = جديد Impl(prog);
}

پروگرام_ذريعو::~پروگرام_ذريعو()
{
    إذا(p)
        p->release();
}

پروگرام_ذريعو::پروگرام_ذريعو(ثابت پروگرام_ذريعو& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

پروگرام_ذريعو& پروگرام_ذريعو::عامل = (ثابت پروگرام_ذريعو& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

پروگرام_ذريعو::پروگرام_ذريعو(پروگرام_ذريعو&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = خالي_اشارو;
}

پروگرام_ذريعو& پروگرام_ذريعو::عامل = (پروگرام_ذريعو&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = خالي_اشارو;
    }
    أرجع *هذا;
}

ثابت String& پروگرام_ذريعو::ذريعو() ثابت
{
    CV_Assert(p);
    CV_Assert(p->قسم_5 == Impl::پروگرام_ذريعو_ڪوڊ);
    CV_Assert(p->ذريعو_Addr == عدم); // method returns reference - can't construct temporary object
    أرجع p->ڪوڊ_Str_2;
}

پروگرام_ذريعو::hash_t پروگرام_ذريعو::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

پروگرام_ذريعو پروگرام_ذريعو::from_ٻٽو(ثابت String& module, ثابت String& name,
        ثابت بنانشان حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_ٻٽو(module, name, binary, size, buildOptions);
}

پروگرام_ذريعو پروگرام_ذريعو::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت بنانشان حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_ٻٽو(module, name, binary, size, buildOptions);
}


internal::پروگرام_داخلا::عامل پروگرام_ذريعو&() ثابت
{
    إذا (هذا->p_پروگرام_ذريعو == عدم)
    {
        cv::Auto_تالو lock(cv::getInitializationMutex());
        إذا (هذا->p_پروگرام_ذريعو == عدم)
        {
            پروگرام_ذريعو ps = پروگرام_ذريعو::Impl::from_ذريعو_With_Static_Lifetime(هذا->module, هذا->name, هذا->پروگرام_ڪوڊ, هذا->پروگرام_Hash, cv::String());
            پروگرام_ذريعو* ptr = جديد پروگرام_ذريعو(ps);
            const_cast<پروگرام_داخلا*>(هذا)->p_پروگرام_ذريعو = ptr;
        }
    }
    أرجع *هذا->p_پروگرام_ذريعو;
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

بنية پروگرام_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت پروگرام_ذريعو& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(عدم),
         buildflags(_buildflags)
    {
        ثابت پروگرام_ذريعو::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        ذريعو_Module = src_->module_;
        ذريعو_Name = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        إذا (ctx.ptr() == عدم || device.ptr() == عدم)
            أرجع;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        إذا (src.getImpl()->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_ذريعو_ڪوڊ)
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
        CV_LOG_INFO(عدم, "OpenCL program '" << ذريعو_Module << "/" << ذريعو_Name << "' options:" << buildflags);
#جيڪڏهن_پورو
        compile(ctx, src_, errmsg);
#إذا CV_OPENCL_SHOW_BUILD_KERNELS
        إذا (handle)
        {
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            cl_int نتيجو = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نالا, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            CV_OCL_DBG_CHECK_نتيجو(نتيجو, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", ذريعو_Module.c_str(), ذريعو_Name.c_str()).c_str());
            إذا (نتيجو == CL_SUCCESS && retsz < حجم(kernels_بفر))
            {
                kernels_بفر[retsz] = 0;
                CV_LOG_INFO(عدم, "OpenCL program '" << ذريعو_Module << "/" << ذريعو_Name << "' kernels: '" << kernels_بفر << "'");
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, "OpenCL program '" << ذريعو_Module << "/" << ذريعو_Name << "' can't retrieve kernel names!");
            }
        }
#جيڪڏهن_پورو
    }

    منطقي compile(ثابت Context& ctx, ثابت پروگرام_ذريعو::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        إذا (src_->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_BINARIES)
        {
            CV_LOG_VERBOSE(عدم, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_ٻٽو(ctx, src_->ذريعو_Addr, src_->ذريعو_سائيز, errmsg);
            أرجع isLoaded;
        }
        أرجع compileWithCache(ctx, src_, errmsg);
    }

    منطقي compileWithCache(ثابت Context& ctx, ثابت پروگرام_ذريعو::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 != پروگرام_ذريعو::Impl::پروگرام_BINARIES);

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string بنياد_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_اڳياڙي_String(),
                ctx.getImpl()->get_اڳياڙي_بنياد()
        );
        ثابت String& hash_str = src_->ذريعو_Hash;
        cv::String fname;
        إذا (!بنياد_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(بنياد_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::File_تالو> file_تالو = config.cache_تالو; // can be empty
        إذا (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            حاول
            {
                std::vector<حرف> ٻٽو_Buf;
                منطقي res = خطأ;
                {
                    cv::utils::optional_shared_تالو_guard<cv::utils::fs::File_تالو> تالو_fs(file_تالو.get());
                    ٻٽو_پروگرام_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, ٻٽو_Buf);
                }
                إذا (res)
                {
                    CV_Assert(!ٻٽو_Buf.empty());
                    CV_LOG_VERBOSE(عدم, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقي isLoaded = create_From_ٻٽو(ctx, ٻٽو_Buf, errmsg);
                    إذا (isLoaded)
                        أرجع سچ;
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_اڻ_استعمال_ٿيل(e);
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname << std::سٽ_پڄاڻي << e.what());
            }
            امسك (...)
            {
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == عدم);
        إذا (src_->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_ذريعو_ڪوڊ)
        {
            إذا (!buildFromSources(ctx, src_, errmsg))
            {
                أرجع خطأ;
            }
        }
        وإلا إذا (src_->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            إذا ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(عدم, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_ٻٽو(ctx, src_->ذريعو_Addr, src_->ذريعو_سائيز, errmsg);
            إذا (!isLoaded)
                أرجع خطأ;
        }
        وإلا إذا (src_->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وإلا
        {
            CV_Error(Error::Sts_اندروني, "Internal error");
        }
        CV_Assert(handle != عدم);
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        إذا (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            حاول
            {
                std::vector<حرف> ٻٽو_Buf;
                get_پروگرام_ٻٽو(ٻٽو_Buf);
                {
                    cv::utils::optional_تالو_guard<cv::utils::fs::File_تالو> تالو_fs(file_تالو.get());
                    ٻٽو_پروگرام_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, ٻٽو_Buf);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname << std::سٽ_پڄاڻي << e.what());
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS
        إذا (CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS_قدر)
        {
            std::vector<حرف> ٻٽو_Buf;
            get_پروگرام_ٻٽو(ٻٽو_Buf);
            إذا (!ٻٽو_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                handle = عدم;
                create_From_ٻٽو(ctx, ٻٽو_Buf, errmsg);
            }
        }
#جيڪڏهن_پورو
        أرجع handle != عدم;
    }

    فراغ dumpBuildLog_(cl_int نتيجو, ثابت cl_device_id* deviceList, String& errmsg)
    {
        Auto_بفر<حرف, 4096> بفر; بفر[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                                  CL_پروگرام_BUILD_LOG, 0, 0, &retsz);
        إذا (log_retval == CL_SUCCESS && retsz > 1)
        {
            بفر.resize(retsz + 16);
            log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                               CL_پروگرام_BUILD_LOG, retsz+1, بفر.معلومات(), &retsz);
            إذا (log_retval == CL_SUCCESS)
            {
                إذا (retsz < بفر.size())
                    بفر[retsz] = 0;
                وإلا
                    بفر[بفر.size() - 1] = 0;
            }
            وإلا
            {
                بفر[0] = 0;
            }
        }

        errmsg = String(بفر.معلومات());
        لکو("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                ذريعو_Module.c_str(), ذريعو_Name.c_str(),
                نتيجو, getOpenCLErrorString(نتيجو),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    منطقي buildFromSources(ثابت Context& ctx, ثابت پروگرام_ذريعو::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 == پروگرام_ذريعو::Impl::پروگرام_ذريعو_ڪوڊ);
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                ذريعو_Module.c_str(), ذريعو_Name.c_str(),
                src_->ذريعو_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(عدم, 0, "Compile... " << ذريعو_Module.c_str() << "/" << ذريعو_Name.c_str());

        ثابت حرف* srcptr = src_->ذريعو_Addr ? ((ثابت حرف*)src_->ذريعو_Addr) : src_->ڪوڊ_Str_2.c_str();
        size_t srclen = src_->ذريعو_Addr ? src_->ذريعو_سائيز : src_->ڪوڊ_Str_2.size();
        CV_Assert(srcptr != عدم);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_پروگرام_With_ذريعو((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_نتيجو(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        إذا (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_بفر<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.معلومات();
            لكل (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_پروگرام(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_نتيجو(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#إذا !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            إذا (retval != CL_SUCCESS)
#جيڪڏهن_پورو
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                إذا (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = عدم;
                }
                إذا (retval != CL_SUCCESS &&
                    ذريعو_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#إذا CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS
            إذا (handle && CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS_قدر)
            {
                CV_LOG_INFO(عدم, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_بفر[4096] = {0};
                cl_int نتيجو = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نالا, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
                إذا (retsz < حجم(kernels_بفر))
                    kernels_بفر[retsz] = 0;
                وإلا
                    kernels_بفر[0] = 0;
                CV_LOG_INFO(عدم, نتيجو << ": Kernels='" << kernels_بفر << "'");
            }
#جيڪڏهن_پورو
        }
        أرجع handle != عدم;
    }

    فراغ get_پروگرام_ٻٽو(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_ٻٽو_SIZES, حجم(sz), &sz, عدم));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_BINARIES, حجم(ptr), &ptr, عدم));
    }

    منطقي create_From_ٻٽو(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        أرجع create_From_ٻٽو(ctx, (ثابت بنانشان حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقي create_From_ٻٽو(ثابت Context& ctx, ثابت بنانشان حرف* ٻٽو_Addr, ثابت size_t ٻٽو_سائيز, String& errmsg)
    {
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(عدم, 0, "Load from binary... (" << ٻٽو_سائيز << " bytes)");

        CV_Assert(ٻٽو_سائيز > 0);

        size_t ndevices = (صحيح)ctx.ndevices();
        Auto_بفر<cl_device_id> devices_(ndevices);
        Auto_بفر<ثابت uchar*> ٻٽو_Ptrs(ndevices);
        Auto_بفر<size_t> ٻٽو_Sizes(ndevices);

        cl_device_id* devices = devices_.معلومات();
        ثابت uchar** ٻٽو_Ptrs_2 = ٻٽو_Ptrs.معلومات();
        size_t* ٻٽو_Sizes_2 = ٻٽو_Sizes.معلومات();
        لكل (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ٻٽو_Ptrs_2[i] = ٻٽو_Addr;
            ٻٽو_Sizes_2[i] = ٻٽو_سائيز;
        }

        cl_int نتيجو = 0;
        handle = cl_Create_پروگرام_With_ٻٽو((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.معلومات(),
                                           ٻٽو_Sizes_2, ٻٽو_Ptrs_2, عدم, &نتيجو);
        إذا (نتيجو != CL_SUCCESS)
        {
            CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_نياپو(نتيجو, "clCreateProgramWithBinary"));
            إذا (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                handle = عدم;
            }
        }
        إذا (!handle)
        {
            أرجع خطأ;
        }
        // call clBuildProgram()
        {
            نتيجو = cl_Build_پروگرام(handle, (cl_uint)ndevices, devices_.معلومات(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_نتيجو(نتيجو, cv::format("clBuildProgram(binary: %s/%s)", ذريعو_Module.c_str(), ذريعو_Name.c_str()).c_str());
            إذا (نتيجو != CL_SUCCESS)
            {
                dumpBuildLog_(نتيجو, devices, errmsg);
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = عدم;
                }
                أرجع خطأ;
            }
        }
        // check build status
        {
            cl_build_حالت build_حالت = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(نتيجو = cl_Get_پروگرام_Build_Info(handle, devices[0], CL_پروگرام_BUILD_حالت,
                    حجم(build_حالت), &build_حالت, &retsz));
            إذا (نتيجو == CL_SUCCESS)
            {
                إذا (build_حالت == CL_BUILD_SUCCESS)
                {
                    أرجع سچ;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "clGetProgramBuildInfo() returns " << build_حالت);
                    أرجع خطأ;
                }
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_نياپو(نتيجو, "clGetProgramBuildInfo()"));
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = عدم;
                }
            }
        }
#إذا CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS
        إذا (handle && CV_OPENCL_VALIDATE_ٻٽو_PROGRAMS_قدر)
        {
            CV_LOG_INFO(عدم, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            نتيجو = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نالا, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            إذا (retsz < حجم(kernels_بفر))
                kernels_بفر[retsz] = 0;
            وإلا
                kernels_بفر[0] = 0;
            CV_LOG_INFO(عدم, نتيجو << ": Kernels='" << kernels_بفر << "'");
        }
#جيڪڏهن_پورو
        أرجع handle != عدم;
    }

    ~Impl()
    {
        إذا( handle )
        {
#جيڪڏهن_وصف_ٿيل _WIN32
            إذا (!cv::__termination)
#جيڪڏهن_پورو
            {
                cl_Release_پروگرام(handle);
            }
            handle = عدم;
        }
    }

    cl_پروگرام handle;

    String buildflags;
    String ذريعو_Module;
    String ذريعو_Name;
};


پروگرام_2::پروگرام_2() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_2::پروگرام_2(ثابت پروگرام_ذريعو& src,
        ثابت String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

پروگرام_2::پروگرام_2(ثابت پروگرام_2& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

پروگرام_2& پروگرام_2::عامل = (ثابت پروگرام_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

پروگرام_2::پروگرام_2(پروگرام_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = خالي_اشارو;
}

پروگرام_2& پروگرام_2::عامل = (پروگرام_2&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = خالي_اشارو;
    }
    أرجع *هذا;
}

پروگرام_2::~پروگرام_2()
{
    إذا(p)
        p->release();
}

منطقي پروگرام_2::create(ثابت پروگرام_ذريعو& src,
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

فراغ* پروگرام_2::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

#جيڪڏهن_وصف_نه_ٿيل OPENCV_REMOVE_DEPRECATED_API
ثابت پروگرام_ذريعو& پروگرام_2::ذريعو() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروگرام_2::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_اڻ_استعمال_ٿيل(bin); CV_اڻ_استعمال_ٿيل(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروگرام_2::اكتب(String& bin) ثابت
{
    CV_اڻ_استعمال_ٿيل(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String پروگرام_2::get_اڳياڙي() ثابت
{
    إذا(!p)
        أرجع String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_اڳياڙي_String().c_str(), p->buildflags.c_str());
}

String پروگرام_2::get_اڳياڙي(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_اڳياڙي_String().c_str(), buildflags.c_str());
}
#جيڪڏهن_پورو // OPENCV_REMOVE_DEPRECATED_API

فراغ پروگرام_2::get_ٻٽو(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_پروگرام_ٻٽو(binary);
}

پروگرام_2 Context::Impl::getProg(ثابت پروگرام_ذريعو& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t حد_بندي = get_پروگرام_ڳڻپ_حد_بندي();
    ثابت پروگرام_ذريعو::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String ڪنجي = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->ذريعو_Hash.c_str(),
            get_اڳياڙي_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_تالو lock(پروگرام_cache_mutex);
        phash_t::iterator it = phash.find(ڪنجي);
        إذا (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), ڪنجي);
            إذا (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(ڪنجي);
            }
            أرجع it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            إذا (حد_بندي > 0 && sz >= حد_بندي)
            {
                ساكن منطقي warningFlag = خطأ;
                إذا (!warningFlag)
                {
                    لکو("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = سچ;
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
    پروگرام_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_تالو lock(پروگرام_cache_mutex);
        phash.insert(std::pair<std::string, پروگرام_2>(ڪنجي, prog));
        cacheList.push_front(ڪنجي);
    }
    أرجع prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

سانچو<قسم_نالو T>
صنف OpenCLBufferPool
{
محمي:
    ~OpenCLBufferPool() { }
عام:
    مجازي T allocate(size_t size) = 0;
    مجازي فراغ release(T بفر) = 0;
};

سانچو <قسم_نالو Derived, قسم_نالو بفر_داخلا, قسم_نالو T>
صنف Open_CLBuffer_Pool_بنياد_Impl : عام بفر_Pool_Controller, عام OpenCLBufferPool<T>
{
خاص:
    مضمن Derived& derived() { أرجع *static_cast<Derived*>(هذا); }
محمي:
    Mutex mutex_;

    size_t current_Reserved_سائيز;
    size_t max_Reserved_سائيز;

    std::list<بفر_داخلا> allocatedEntries_; // Allocated and used entries
    std::list<بفر_داخلا> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقي ڳوليو_And_Remove_داخلا_From_Allocated_List(CV_ٻاهريون بفر_داخلا& داخلا_3, T بفر)
    {
        قسم_نالو std::list<بفر_داخلا>::iterator i = allocatedEntries_.begin();
        لكل (; i != allocatedEntries_.end(); ++i)
        {
            بفر_داخلا& e = *i;
            إذا (e.cl_بفر_2 == بفر)
            {
                داخلا_3 = e;
                allocatedEntries_.erase(i);
                أرجع سچ;
            }
        }
        أرجع خطأ;
    }

    // synchronized
    منطقي ڳوليو_And_Remove_داخلا_From_Reserved_List(CV_ٻاهريون بفر_داخلا& داخلا_3, ثابت size_t size)
    {
        إذا (reservedEntries_.empty())
            أرجع خطأ;
        قسم_نالو std::list<بفر_داخلا>::iterator i = reservedEntries_.begin();
        قسم_نالو std::list<بفر_داخلا>::iterator نتيجو_pos = reservedEntries_.end();
        بفر_داخلا نتيجو;
        size_t minDiff = (size_t)(-1);
        لكل (; i != reservedEntries_.end(); ++i)
        {
            بفر_داخلا& e = *i;
            إذا (e.صلاحيت >= size)
            {
                size_t diff = e.صلاحيت - size;
                إذا (diff < std::max((size_t)4096, size / 8) && (نتيجو_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    نتيجو_pos = i;
                    نتيجو = e;
                    إذا (diff == 0)
                        اكسر;
                }
            }
        }
        إذا (نتيجو_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(نتيجو_pos);
            داخلا_3 = نتيجو;
            current_Reserved_سائيز -= داخلا_3.صلاحيت;
            allocatedEntries_.push_back(داخلا_3);
            أرجع سچ;
        }
        أرجع خطأ;
    }

    // synchronized
    فراغ check_سائيز_جو_Reserved_Entries()
    {
        طالما (current_Reserved_سائيز > max_Reserved_سائيز)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت بفر_داخلا& داخلا_3 = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_سائيز >= داخلا_3.صلاحيت);
            current_Reserved_سائيز -= داخلا_3.صلاحيت;
            derived().release_بفر_داخلا(داخلا_3);
            reservedEntries_.pop_back();
        }
    }

    مضمن size_t ورهاست_Granularity(size_t size)
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
    Open_CLBuffer_Pool_بنياد_Impl()
        : current_Reserved_سائيز(0),
          max_Reserved_سائيز(0)
    {
        // nothing
    }
    مجازي ~Open_CLBuffer_Pool_بنياد_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عام:
    مجازي T allocate(size_t size) CV_OVERRIDE
    {
        Auto_تالو locker(mutex_);
        بفر_داخلا داخلا_3;
        إذا (max_Reserved_سائيز > 0 && ڳوليو_And_Remove_داخلا_From_Reserved_List(داخلا_3, size))
        {
            CV_DbgAssert(size <= داخلا_3.صلاحيت);
            LOG_بفر_POOL("Reuse reserved buffer: %p\n", داخلا_3.cl_بفر_2);
        }
        وإلا
        {
            derived().allocate_بفر_داخلا(داخلا_3, size);
        }
        أرجع داخلا_3.cl_بفر_2;
    }
    مجازي فراغ release(T بفر) CV_OVERRIDE
    {
        Auto_تالو locker(mutex_);
        بفر_داخلا داخلا_3;
        CV_Assert(ڳوليو_And_Remove_داخلا_From_Allocated_List(داخلا_3, بفر));
        إذا (max_Reserved_سائيز == 0 || داخلا_3.صلاحيت > max_Reserved_سائيز / 8)
        {
            derived().release_بفر_داخلا(داخلا_3);
        }
        وإلا
        {
            reservedEntries_.push_front(داخلا_3);
            current_Reserved_سائيز += داخلا_3.صلاحيت;
            check_سائيز_جو_Reserved_Entries();
        }
    }

    مجازي size_t get_Reserved_سائيز() ثابت CV_OVERRIDE { أرجع current_Reserved_سائيز; }
    مجازي size_t get_Max_Reserved_سائيز() ثابت CV_OVERRIDE { أرجع max_Reserved_سائيز; }
    مجازي فراغ سيٽ_Max_Reserved_سائيز(size_t size) CV_OVERRIDE
    {
        Auto_تالو locker(mutex_);
        size_t old_Max_Reserved_سائيز = max_Reserved_سائيز;
        max_Reserved_سائيز = size;
        إذا (max_Reserved_سائيز < old_Max_Reserved_سائيز)
        {
            قسم_نالو std::list<بفر_داخلا>::iterator i = reservedEntries_.begin();
            لكل (; i != reservedEntries_.end();)
            {
                ثابت بفر_داخلا& داخلا_3 = *i;
                إذا (داخلا_3.صلاحيت > max_Reserved_سائيز / 8)
                {
                    CV_DbgAssert(current_Reserved_سائيز >= داخلا_3.صلاحيت);
                    current_Reserved_سائيز -= داخلا_3.صلاحيت;
                    derived().release_بفر_داخلا(داخلا_3);
                    i = reservedEntries_.erase(i);
                    استمر;
                }
                ++i;
            }
            check_سائيز_جو_Reserved_Entries();
        }
    }
    مجازي فراغ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_تالو locker(mutex_);
        قسم_نالو std::list<بفر_داخلا>::const_iterator i = reservedEntries_.begin();
        لكل (; i != reservedEntries_.end(); ++i)
        {
            ثابت بفر_داخلا& داخلا_3 = *i;
            derived().release_بفر_داخلا(داخلا_3);
        }
        reservedEntries_.clear();
        current_Reserved_سائيز = 0;
    }
};

بنية CLBuffer_داخلا
{
    cl_mem cl_بفر_2;
    size_t صلاحيت;
    CLBuffer_داخلا() : cl_بفر_2((cl_mem)عدم), صلاحيت(0) { }
};

صنف OpenCLBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_بنياد_Impl<OpenCLBufferPoolImpl, CLBuffer_داخلا, cl_mem>
{
عام:
    تعريفنوع بنية CLBuffer_داخلا بفر_داخلا;
محمي:
    صحيح createFlags_;
عام:
    OpenCLBufferPoolImpl(صحيح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    فراغ allocate_بفر_داخلا(بفر_داخلا& داخلا_3, size_t size)
    {
        CV_DbgAssert(داخلا_3.cl_بفر_2 == عدم);
        داخلا_3.صلاحيت = align_سائيز(size, (صحيح)ورهاست_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        داخلا_3.cl_بفر_2 = cl_Create_بفر((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, داخلا_3.صلاحيت, 0, &retval);
        CV_OCL_CHECK_نتيجو(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (ڊگهو ڊگهو صحيح)داخلا_3.صلاحيت, (فراغ*)داخلا_3.cl_بفر_2).c_str());
        CV_Assert(داخلا_3.cl_بفر_2 != عدم);
        إذا(retval == CL_SUCCESS)
        {
            CV_IMPL_جوڙيو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (ڊگهو ڊگهو)داخلا_3.صلاحيت, (ڊگهو ڊگهو)داخلا_3.صلاحيت, داخلا_3.cl_بفر_2);
        allocatedEntries_.push_back(داخلا_3);
    }

    فراغ release_بفر_داخلا(ثابت بفر_داخلا& داخلا_3)
    {
        CV_Assert(داخلا_3.صلاحيت != 0);
        CV_Assert(داخلا_3.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                داخلا_3.cl_بفر_2, (ڊگهو ڊگهو)داخلا_3.صلاحيت, (ڊگهو ڊگهو)داخلا_3.صلاحيت);
        CV_OCL_DBG_CHECK(clReleaseMemObject(داخلا_3.cl_بفر_2));
    }
};

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
بنية CLSVMBuffer_داخلا
{
    فراغ* cl_بفر_2;
    size_t صلاحيت;
    CLSVMBuffer_داخلا() : cl_بفر_2(عدم), صلاحيت(0) { }
};
صنف OpenCLSVMBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_بنياد_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_داخلا, فراغ*>
{
عام:
    تعريفنوع بنية CLSVMBuffer_داخلا بفر_داخلا;
عام:
    OpenCLSVMBufferPoolImpl()
    {
    }

    فراغ allocate_بفر_داخلا(بفر_داخلا& داخلا_3, size_t size)
    {
        CV_DbgAssert(داخلا_3.cl_بفر_2 == عدم);
        داخلا_3.صلاحيت = align_سائيز(size, (صحيح)ورهاست_Granularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحيح)داخلا_3.صلاحيت);
        فراغ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, داخلا_3.صلاحيت, 0);
        CV_Assert(buf);

        داخلا_3.cl_بفر_2 = buf;
        {
            CV_IMPL_جوڙيو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (ڊگهو ڊگهو)داخلا_3.صلاحيت, (ڊگهو ڊگهو)داخلا_3.صلاحيت, داخلا_3.cl_بفر_2);
        allocatedEntries_.push_back(داخلا_3);
    }

    فراغ release_بفر_داخلا(ثابت بفر_داخلا& داخلا_3)
    {
        CV_Assert(داخلا_3.صلاحيت != 0);
        CV_Assert(داخلا_3.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                داخلا_3.cl_بفر_2, (ڊگهو ڊگهو)داخلا_3.صلاحيت, (ڊگهو ڊگهو)داخلا_3.صلاحيت);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  داخلا_3.cl_بفر_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), داخلا_3.cl_بفر_2);
    }
};
#جيڪڏهن_پورو



سانچو <منطقي readAccess, منطقي writeAccess>
صنف Aligned_معلومات_Ptr
{
محمي:
    ثابت size_t سائيز_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عام:
    Aligned_معلومات_Ptr(uchar* ptr, size_t size, size_t alignment)
        : سائيز_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        إذا (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[سائيز_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                يادگيري_نقل(ptr_, originPtr_, سائيز_3);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_معلومات_Ptr()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                يادگيري_نقل(originPtr_, ptr_, سائيز_3);
            }
            ميٽيو[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_معلومات_Ptr(ثابت Aligned_معلومات_Ptr&); // disabled
    Aligned_معلومات_Ptr& عامل=(ثابت Aligned_معلومات_Ptr&); // disabled
};

سانچو <منطقي readAccess, منطقي writeAccess>
صنف Aligned_معلومات_Ptr2_D
{
محمي:
    ثابت size_t سائيز_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t صفون_2;
    size_t کانن_ڳڻپ_2;
    size_t قدم_2;

عام:
    Aligned_معلومات_Ptr2_D(uchar* ptr, size_t صفون, size_t کانن_ڳڻپ, size_t قدم, size_t alignment, size_t extrabytes=0)
        : سائيز_3(صفون*قدم), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم), صفون_2(صفون), کانن_ڳڻپ_2(کانن_ڳڻپ), قدم_2(قدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != عدم);
        إذا (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[سائيز_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                لكل (size_t i = 0; i < صفون_2; i++)
                    يادگيري_نقل(ptr_ + i*قدم_2, originPtr_ + i*قدم_2, کانن_ڳڻپ_2);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_معلومات_Ptr2_D()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                لكل (size_t i = 0; i < صفون_2; i++)
                    يادگيري_نقل(originPtr_ + i*قدم_2, ptr_ + i*قدم_2, کانن_ڳڻپ_2);
            }
            ميٽيو[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_معلومات_Ptr2_D(ثابت Aligned_معلومات_Ptr2_D&); // disabled
    Aligned_معلومات_Ptr2_D& عامل=(ثابت Aligned_معلومات_Ptr2_D&); // disabled
};

#جيڪڏهن_وصف_نه_ٿيل CV_OPENCL_معلومات_PTR_ALIGNMENT
#وصف_ڪريو CV_OPENCL_معلومات_PTR_ALIGNMENT 16
#جيڪڏهن_پورو


فراغ Context::Impl::__init_buffer_pools()
{
    بفر_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& بفر_Pool_3 = *بفر_Pool_2.get();
    بفر_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& بفر_Pool_Host_Ptr_2 = *بفر_Pool_Host_Ptr.get();

    size_t default_Pool_سائيز = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_سائيز = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_سائيز);
    بفر_Pool_3.سيٽ_Max_Reserved_سائيز(pool_سائيز);
    size_t pool_سائيز_Host_Ptr = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_سائيز);
    بفر_Pool_Host_Ptr_2.سيٽ_Max_Reserved_سائيز(pool_سائيز_Host_Ptr);

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
    بفر_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& بفر_Pool_SVM_2 = *بفر_Pool_SVM.get();
    size_t pool_سائيز_SVM = utils::get_Configuration_Parameter_سائيز_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_سائيز);
    بفر_Pool_SVM_2.سيٽ_Max_Reserved_سائيز(pool_سائيز_SVM);
#جيڪڏهن_پورو

    CV_LOG_INFO(عدم, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_سائيز << " poolSizeHostPtr=" << pool_سائيز_Host_Ptr);
}

صنف OpenCLAllocator CV_FINAL : عام MatAllocator
{
عام:
    تعداد AllocatorFlags
    {
        ALLOCATOR_FLAGS_بفر_POOL_استعمال_ٿيل = 1 << 0,
        ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_استعمال_ٿيل = 1 << 1,
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_بفر_POOL_SVM_استعمال_ٿيل = 1 << 2,
#جيڪڏهن_پورو
        ALLOCATOR_FLAGS_EXTERNAL_بفر = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_قطار();
    }

    UMat_معلومات* defaultAllocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* معلومات, size_t* قدم,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMat_معلومات* u = matStdAllocator->allocate(dims, sizes, type, معلومات, قدم, flags, usageFlags);
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

    فراغ get_بهترين_Flags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحيح& createFlags, UMat_معلومات::يادگيري_Flag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        إذا ((usageFlags & USAGE_ALLOCATE_HOST_يادگيري) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        إذا (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_يادگيري_2()
#جيڪڏهن_وصف_نه_ٿيل __APPLE__
                || dev.isIntel()
#جيڪڏهن_پورو
                )
            )
        )
            flags0 = static_cast<UMat_معلومات::يادگيري_Flag>(0);
        وإلا
            flags0 = UMat_معلومات::COPY_ON_MAP;
    }

    UMat_معلومات* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* معلومات, size_t* قدم, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!useOpenCL())
            أرجع defaultAllocate(dims, sizes, type, معلومات, قدم, flags, usageFlags);

        flush_Cleanup_قطار();

        CV_Assert(معلومات == 0);
        size_t ڪل = CV_ELEM_سائيز(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( قدم )
                قدم[i] = ڪل;
            ڪل *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        إذا (!ctx.getImpl())
            أرجع defaultAllocate(dims, sizes, type, معلومات, قدم, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحيح createFlags = 0;
        UMat_معلومات::يادگيري_Flag flags0 = static_cast<UMat_معلومات::يادگيري_Flag>(0);
        get_بهترين_Flags(ctx, flags, usageFlags, createFlags, flags0);

        فراغ* handle = عدم;
        صحيح allocatorFlags = 0;

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        إذا (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_SVM_استعمال_ٿيل;
            handle = ctxImpl.get_بفر_Pool_SVM().allocate(ڪل);

            // this property is constant, so single buffer pool can be used here
            منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
            allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
        }
        وإلا
#جيڪڏهن_پورو
        إذا (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_استعمال_ٿيل;
            handle = ctxImpl.get_بفر_Pool().allocate(ڪل);
        }
        وإلا إذا (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_استعمال_ٿيل;
            handle = ctxImpl.get_بفر_Pool_Host_Ptr().allocate(ڪل);
        }
        وإلا
        {
            CV_Assert(handle != عدم); // Unsupported, throw
        }

        إذا (!handle)
            أرجع defaultAllocate(dims, sizes, type, معلومات, قدم, flags, usageFlags);

        UMat_معلومات* u = جديد UMat_معلومات(هذا);
        u->معلومات = 0;
        u->size = ڪل;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<فراغ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->عارضي_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(سچ);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع u;
    }

    منطقي allocate(UMat_معلومات* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع خطأ;

        flush_Cleanup_قطار();

        UMat_معلومات_Auto_تالو lock(u);

        إذا(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحيح createFlags = 0;
            UMat_معلومات::يادگيري_Flag flags0 = static_cast<UMat_معلومات::يادگيري_Flag>(0);
            get_بهترين_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقي copyOnMap = (flags0 & UMat_معلومات::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحيح allocatorFlags = 0;
            UMat_معلومات::يادگيري_Flag عارضي_UMat_Flags = static_cast<UMat_معلومات::يادگيري_Flag>(0);
            فراغ* handle = عدم;
            cl_int retval = CL_SUCCESS;

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقي useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            إذا (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                عارضي_UMat_Flags = UMat_معلومات::عارضي_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (صحيح)u->size, handle);
            }
            وإلا إذا (useSVM && (svmCaps.is_Support_Fine_Grain_بفر() || svmCaps.is_Support_Coarse_Grain_بفر()))
            {
                إذا (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (صحيح)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_قطار q = عدم;
                    إذا (!is_Fine_Grain_بفر)
                    {
                        q = (cl_command_قطار)قطار::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحيح)u->size);
                        cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMap()");

                    }
                    يادگيري_نقل(handle, u->origdata, u->size);
                    إذا (!is_Fine_Grain_بفر)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                    }

                    عارضي_UMat_Flags = UMat_معلومات::عارضي_UMAT | UMat_معلومات::عارضي_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر
                                                : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
                }
            }
            وإلا
#جيڪڏهن_پورو
            {
                إذا( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                عارضي_UMat_Flags = UMat_معلومات::عارضي_UMAT;
                إذا (
                #جيڪڏهن_وصف_ٿيل __APPLE__
                    !copyOnMap &&
                #جيڪڏهن_پورو
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحيح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_معلومات && u->original_UMat_معلومات->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (ڊگهو ڊگهو صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                }
                إذا((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (ڊگهو ڊگهو صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                    عارضي_UMat_Flags |= UMat_معلومات::عارضي_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clCreateBuffer() => %p", (فراغ*)handle).c_str());
            إذا(!handle || retval != CL_SUCCESS)
                أرجع خطأ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = هذا;
            u->flags |= عارضي_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(سچ);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع سچ;
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

    فراغ deallocate(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        إذا (!!(u->flags & UMat_معلومات::ASYNC_CLEANUP))
            جوڙيو_منزل_Cleanup_قطار(u);
        وإلا
            deallocate_(u);
    }

    فراغ deallocate_(UMat_معلومات* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        إذا ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_بفر) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#جيڪڏهن_وصف_ٿيل _WIN32
        إذا (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            أرجع;             // avoid any OpenCL calls
#جيڪڏهن_پورو
        إذا(u->عارضي_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            إذا (u->hostCopyObsolete())
            {
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    إذا( u->عارضي_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر);
                        منطقي is_Fine_Grain_بفر = (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر;
                        cl_command_قطار q = عدم;
                        إذا (!is_Fine_Grain_بفر)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0));
                            q = (cl_command_قطار)قطار::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMap()");
                        }
                        cl_پڄاڻي(q);
                        يادگيري_نقل(u->origdata, u->handle, u->size);
                        إذا (!is_Fine_Grain_بفر)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, u->handle, 0, عدم, عدم);
                            CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                        }
                    }
                    وإلا
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وإلا
#جيڪڏهن_پورو
                {
                    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
                    إذا( u->عارضي_Copied_UMat() )
                    {
                        Aligned_معلومات_Ptr<خطأ, سچ> alignedPtr(u->origdata, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وإلا
                    {
                        cl_int retval = 0;
                        إذا (u->عارضي_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_قطار(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            فراغ* معلومات = cl_قطار_اندر_وجهو_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (ڊگهو ڊگهو صحيح)u->size, معلومات).c_str());
                            CV_Assert(u->origdata == معلومات && "Details: https://github.com/opencv/opencv/issues/6293");
                            إذا (u->original_UMat_معلومات)
                            {
                                CV_Assert(u->original_UMat_معلومات->معلومات == معلومات);
                            }
                            retval = cl_قطار_اندر_وجهو_Unmap_Mem_Object(q, (cl_mem)u->handle, معلومات, 0, 0, 0);
                            CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, معلومات, (ڊگهو ڊگهو صحيح)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_پڄاڻي(q));
                        }
                    }
                }
                u->markHostCopyObsolete(خطأ);
            }
            وإلا
            {
                // nothing
            }
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                إذا( u->عارضي_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وإلا
#جيڪڏهن_پورو
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clReleaseMemObject(ptr=%p)", (فراغ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = عدم;
            إذا(u->معلومات && u->copyOnMap() && u->معلومات != u->origdata)
                fastFree(u->معلومات);
            u->معلومات = u->origdata;
            u->currAllocator->deallocate(u);
            u = عدم;
        }
        وإلا
        {
            CV_Assert(u->origdata == عدم);
            إذا(u->معلومات && u->copyOnMap() && u->معلومات != u->origdata)
            {
                fastFree(u->معلومات);
                u->معلومات = 0;
                u->markHostCopyObsolete(سچ);
            }
            إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_استعمال_ٿيل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool().release((cl_mem)u->handle);
            }
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_استعمال_ٿيل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_SVM_استعمال_ٿيل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                وإلا إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, u->handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_SVM().release((فراغ*)u->handle);
            }
#جيڪڏهن_پورو
            وإلا
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            ميٽيو u;
            u = عدم;
        }
        CV_Assert(u == عدم);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    فراغ map(UMat_معلومات* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(سچ);

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        {
            إذا( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_بفر_MAP;
                        }
                    }
                    cl_پڄاڻي(q);
                    u->معلومات = (uchar*)u->handle;
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(سچ);
                    أرجع;
                }
#جيڪڏهن_پورو

                cl_int retval = CL_SUCCESS;
                إذا (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->معلومات = (uchar*)cl_قطار_اندر_وجهو_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_نتيجو(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (ڊگهو ڊگهو صحيح)u->size, u->معلومات).c_str());
                }
                إذا (u->معلومات && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(سچ);
                    أرجع;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_معلومات::COPY_ON_MAP;
            }

            إذا(!u->معلومات)
            {
                u->معلومات = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(سچ);
            }
        }

        إذا (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_معلومات_Ptr<خطأ, سچ> alignedPtr(u->معلومات, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#جيڪڏهن_پورو
            cl_int retval = cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (ڊگهو ڊگهو صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(خطأ);
        }
    }

    فراغ unmap(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;


        CV_Assert(u->handle != 0);

        UMat_معلومات_Auto_تالو autolock(u);

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
        cl_int retval = 0;
        إذا( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->معلومات != عدم);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, u->handle,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                        cl_پڄاڻي(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_بفر_MAP;
                    }
                }
                إذا (u->refcount == 0)
                    u->معلومات = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(سچ);
                أرجع;
            }
#جيڪڏهن_پورو
            إذا (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_قطار_اندر_وجهو_Unmap_Mem_Object(q, (cl_mem)u->handle, u->معلومات, 0, 0, 0);
                CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, u->معلومات, (ڊگهو ڊگهو صحيح)u->size).c_str());
                إذا (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_پڄاڻي(q));
                }
                u->markDeviceMemMapped(خطأ);
                u->معلومات = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(سچ);
            }
        }
        وإلا إذا( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_معلومات_Ptr<سچ, خطأ> alignedPtr(u->معلومات, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#جيڪڏهن_پورو
            retval = cl_قطار_اندر_وجهو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (ڊگهو ڊگهو صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(خطأ);
            u->markHostCopyObsolete(سچ);
        }
    }

    منطقي checkContinuous(صحيح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& ڪل, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقي iscontinuous = سچ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        ڪل = sz[dims-1];
        لكل( صحيح i = dims-2; i >= 0; i-- )
        {
            إذا( i >= 0 && (ڪل != srcstep[i] || ڪل != dststep[i]) )
                iscontinuous = خطأ;
            ڪل *= sz[i];
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

    فراغ download(UMat_معلومات* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;
        UMat_معلومات_Auto_تالو autolock(u);

        إذا( u->معلومات && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            أرجع;
        }
        CV_Assert( u->handle != 0 );

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        size_t ڪل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            ڪل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->معلومات == عدم || u->معلومات == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMap()");
            }
            cl_پڄاڻي(q);
            إذا( iscontinuous )
            {
                يادگيري_نقل(dstptr, (uchar*)u->handle + srcrawofs, ڪل);
            }
            وإلا
            {
                // This code is from MatAllocator::download()
                صحيح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)وڌ_کان_وڌ_عدد );
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
                    يادگيري_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                cl_پڄاڻي(q);
            }
        }
        وإلا
#جيڪڏهن_پورو
        {
            إذا( iscontinuous )
            {
                Aligned_معلومات_Ptr<خطأ, سچ> alignedPtr((uchar*)dstptr, ڪل, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, ڪل, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_معلومات_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_معلومات_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_معلومات_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                ڪل = align_سائيز(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                ڪل = std::min(ڪل, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, ڪل, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    يادگيري_نقل( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وإلا
            {
                Aligned_معلومات_Ptr2_D<خطأ, سچ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_معلومات_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    فراغ upload(UMat_معلومات* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->عارضي_UMat());

        size_t ڪل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            ڪل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_معلومات_Auto_تالو autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        إذا( u->معلومات && (u->hostCopyObsolete() < u->deviceCopyObsolete() || ڪل == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(خطأ);
            u->markDeviceCopyObsolete(سچ);
            أرجع;
        }

        CV_Assert( u->handle != 0 );
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->معلومات == عدم || u->معلومات == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMap()");
            }
            cl_پڄاڻي(q);
            إذا( iscontinuous )
            {
                يادگيري_نقل((uchar*)u->handle + dstrawofs, srcptr, ڪل);
            }
            وإلا
            {
                // This code is from MatAllocator::upload()
                صحيح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)وڌ_کان_وڌ_عدد );
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
                    يادگيري_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMUnmap()");
                cl_پڄاڻي(q);
            }
        }
        وإلا
#جيڪڏهن_پورو
        {
            إذا( iscontinuous )
            {
                Aligned_معلومات_Ptr<سچ, خطأ> alignedPtr((uchar*)srcptr, ڪل, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                cl_int retval = cl_قطار_اندر_وجهو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, ڪل, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (فراغ*)u->handle, (ڊگهو ڊگهو صحيح)dstrawofs, (ڊگهو ڊگهو صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_معلومات_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_معلومات_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_معلومات_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                ڪل = align_سائيز(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                ڪل = std::min(ڪل, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ڪل, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    يادگيري_نقل( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ڪل, ptr, 0, 0, 0));
            }
            وإلا
            {
                Aligned_معلومات_Ptr2_D<سچ, خطأ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_معلومات_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Write_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(سچ);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#جيڪڏهن_پورو
        {
            u->markHostCopyObsolete(سچ);
        }
        u->markDeviceCopyObsolete(خطأ);
    }

    فراغ copy(UMat_معلومات* src, UMat_معلومات* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي _sync) ثابت CV_OVERRIDE
    {
        إذا(!src || !dst)
            أرجع;

        size_t ڪل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            ڪل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_معلومات_Auto_تالو src_autolock(src, dst);

        إذا( !src->handle || (src->معلومات && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->معلومات + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            أرجع;
        }
        إذا( !dst->handle || (dst->معلومات && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->معلومات + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(خطأ);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
            إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وإلا
#جيڪڏهن_پورو
            {
                dst->markDeviceCopyObsolete(سچ);
            }
            أرجع;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                إذا( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحيح)ڪل);
                    cl_int حالت = svmFns->fn_cl_قطار_اندر_وجهو_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            ڪل, 0, عدم, عدم);
                    CV_OCL_CHECK_نتيجو(حالت, "clEnqueueSVMMemcpy()");
                }
                وإلا
                {
                    cl_پڄاڻي(q);
                    // This code is from MatAllocator::download()/upload()
                    صحيح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)وڌ_کان_وڌ_عدد );
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
                        يادگيري_نقل(ptrs[1], ptrs[0], planesz);
                }
            }
            وإلا
            {
                إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->معلومات + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                وإلا
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->معلومات + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        وإلا
#جيڪڏهن_پورو
        {
            إذا( iscontinuous )
            {
                retval = cl_قطار_اندر_وجهو_Copy_بفر(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, ڪل, 0, 0, 0);
                CV_OCL_CHECK_نتيجو(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (فراغ*)src->handle, (فراغ*)dst->handle, (ڊگهو ڊگهو صحيح)srcrawofs, (ڊگهو ڊگهو صحيح)dstrawofs, (ڊگهو ڊگهو صحيح)ڪل).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_معلومات_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_معلومات_Ptr2_D<خطأ, خطأ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_معلومات_PTR_ALIGNMENT, padding*2);
                Aligned_معلومات_Ptr2_D<خطأ, خطأ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_معلومات_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_ڪل = align_سائيز(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_ڪل = std::min(src_ڪل, src->size - new_srcrawofs);
                size_t dst_ڪل = align_سائيز(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_ڪل = std::min(dst_ڪل, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_ڪل, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Read_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_ڪل, dstptr, 0, 0, 0));

                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    يادگيري_نقل( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_اندر_وجهو_Write_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_ڪل, dstptr, 0, 0, 0));
            }
            وإلا
            {
                CV_OCL_CHECK(retval = cl_قطار_اندر_وجهو_Copy_بفر_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        إذا (retval == CL_SUCCESS)
        {
            CV_IMPL_جوڙيو(CV_IMPL_OCL)
        }

#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#جيڪڏهن_پورو
        {
            dst->markHostCopyObsolete(سچ);
        }
        dst->markDeviceCopyObsolete(خطأ);

        إذا( _sync )
        {
            CV_OCL_DBG_CHECK(cl_پڄاڻي(q));
        }
    }

    بفر_Pool_Controller* get_بفر_Pool_Controller(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        إذا (ctx.empty())
            أرجع عدم;
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL_SVM
        إذا ((svm::checkForceSVMUmatUsage() && (id == عدم || strcmp(id, "OCL") == 0)) || (id != عدم && strcmp(id, "SVM") == 0))
        {
            أرجع &ctx.getImpl()->get_بفر_Pool_SVM();
        }
#جيڪڏهن_پورو
        إذا (id != عدم && strcmp(id, "HOST_ALLOC") == 0)
        {
            أرجع &ctx.getImpl()->get_بفر_Pool_Host_Ptr();
        }
        إذا (id != عدم && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        أرجع &ctx.getImpl()->get_بفر_Pool();
    }

    MatAllocator* matStdAllocator;

    بدلجندڙ cv::Mutex cleanup_قطار_Mutex;
    بدلجندڙ std::deque<UMat_معلومات*> cleanup_قطار;

    فراغ flush_Cleanup_قطار() ثابت
    {
        // TSAN false positive: see #27638
        إذا (!cleanup_قطار.empty())
        {
            std::deque<UMat_معلومات*> q;
            {
                cv::Auto_تالو lock(cleanup_قطار_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_قطار);
            }
            لكل (std::deque<UMat_معلومات*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    فراغ جوڙيو_منزل_Cleanup_قطار(UMat_معلومات* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_تالو lock(cleanup_قطار_Mutex);
            cleanup_قطار.push_back(u);
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
فراغ سيٽ_سائيز( UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* قدمن_2,
              منطقي auto_قدمن = خطأ );
فراغ finalizeHdr(UMat& m);

} // namespace cv


نطاق cv { نطاق ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t قدم, صحيح صفون, صحيح کانن_ڳڻپ, صحيح type, UMat& dst)
{
    صحيح d = 2;
    صحيح sizes[] = { صفون, کانن_ڳڻپ };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_نقاب) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    سيٽ_سائيز(dst, d, sizes, 0, سچ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_بفر;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_بفر == mem_type);

    size_t ڪل = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_سائيز, حجم(size_t), &ڪل, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحيح)قدم >= کانن_ڳڻپ * CV_ELEM_سائيز(type));
    CV_Assert(ڪل >= صفون * قدم);

    // attach clBuffer to UMatData
    dst.u = جديد UMat_معلومات(getOpenCLAllocator());
    dst.u->معلومات            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_بفر;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_معلومات::يادگيري_Flag>(0);
    dst.u->handle          = cl_mem_بفر;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = ڪل;

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

    صحيح اونهائي = CV_8U;
    بدل (fmt.image_channel_معلومات_type)
    {
    حالة CL_UNORM_INT8:
    حالة CL_UNSIGNED_INT8:
        اونهائي = CV_8U;
        اكسر;

    حالة CL_SNORM_INT8:
    حالة CL_SIGNED_INT8:
        اونهائي = CV_8S;
        اكسر;

    حالة CL_UNORM_INT16:
    حالة CL_UNSIGNED_INT16:
        اونهائي = CV_16U;
        اكسر;

    حالة CL_SNORM_INT16:
    حالة CL_SIGNED_INT16:
        اونهائي = CV_16S;
        اكسر;

    حالة CL_SIGNED_INT32:
        اونهائي = CV_32S;
        اكسر;

    حالة CL_FLOAT:
        اونهائي = CV_32F;
        اكسر;

    حالة CL_HALF_FLOAT:
        اونهائي = CV_16F;
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    صحيح type = CV_8UC1;
    بدل (fmt.image_channel_ترتيب)
    {
    حالة CL_R:
    حالة CL_A:
    حالة CL_INTENSITY:
    حالة CL_LUMINANCE:
        type = CV_MAKE_TYPE(اونهائي, 1);
        اكسر;

    حالة CL_RG:
    حالة CL_RA:
        type = CV_MAKE_TYPE(اونهائي, 2);
        اكسر;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالة CL_RGBA:
    حالة CL_BGRA:
    حالة CL_ARGB:
        type = CV_MAKE_TYPE(اونهائي, 4);
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        اكسر;
    }

    size_t قدم = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_صف_PITCH, حجم(size_t), &قدم, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ويڪر, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_اوچائي, حجم(size_t), &h, 0));

    dst.create((صحيح)h, (صحيح)w, type);

    cl_mem cl_بفر = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_قطار_اندر_وجهو_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم));

    CV_OCL_CHECK(cl_پڄاڻي(q));

    أرجع;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساكن فراغ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint انگ_Devices = 0;
    cl_int حالت = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, عدم, &انگ_Devices);
    إذا (حالت != CL_DEVICE_NOT_مليو) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_نتيجو(حالت,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &انگ_Devices).c_str());
    }

    إذا (انگ_Devices == 0)
    {
        devices.clear();
        أرجع;
    }

    devices.resize((size_t)انگ_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, انگ_Devices, &devices[0], &انگ_Devices));
}

بنية PlatformInfo::Impl
{
    Impl(فراغ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        نسخو_3 = getStrProp(CL_PLATFORM_نسخو);
        parseOpenCLVersion(نسخو_3, نسخو_Major, نسخو_Minor);
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

    String نسخو_3;
    صحيح نسخو_Major;
    صحيح نسخو_Minor;
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

PlatformInfo& PlatformInfo::عامل =(ثابت PlatformInfo& i)
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
    i.p = خالي_اشارو;
}

PlatformInfo& PlatformInfo::عامل = (PlatformInfo&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا(p)
            p->release();
        p = i.p;
        i.p = خالي_اشارو;
    }
    أرجع *هذا;
}

صحيح PlatformInfo::device_عدد() ثابت
{
    أرجع p ? (صحيح)p->devices.size() : 0;
}

فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت
{
    CV_Assert(p && d < (صحيح)p->devices.size() );
    إذا(p)
        device.سيٽ(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::نسخو() ثابت
{
    أرجع p ? p->نسخو_3 : String();
}

صحيح PlatformInfo::نسخو_Major_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخو_Major;
}

صحيح PlatformInfo::نسخو_Minor_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخو_Minor;
}

ساكن فراغ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint انگ_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &انگ_Platforms));

    إذا (انگ_Platforms == 0)
    {
        platforms.clear();
        أرجع;
    }

    platforms.resize((size_t)انگ_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(انگ_Platforms, &platforms[0], &انگ_Platforms));
}

فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    لكل (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((فراغ*)&platforms[i]) );
}

ثابت حرف* type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_اونهائي_MAX*16]=
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
    صحيح cn = CV_MAT_CN(type), اونهائي = CV_MAT_اونهائي(type);
    ثابت حرف* نتيجو = cn > 16 ? خالي_اشارو : tab[اونهائي*16 + cn-1];
    CV_Assert(نتيجو);
    أرجع نتيجو;
}

ثابت حرف* memop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_اونهائي_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), اونهائي = CV_MAT_اونهائي(type);
    ثابت حرف* نتيجو = cn > 16 ? خالي_اشارو : tab[اونهائي*16 + cn-1];
    CV_Assert(نتيجو);
    أرجع نتيجو;
}

ثابت حرف* vecop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_اونهائي_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), اونهائي = CV_MAT_اونهائي(type);
    ثابت حرف* نتيجو = cn > 16 ? 0 : tab[اونهائي*16 + cn-1];
    CV_Assert(نتيجو);
    أرجع نتيجو;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    أرجع convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf, size_t buf_سائيز_2)
{
    إذا( sdepth == ddepth )
        أرجع "noconvert";
    ثابت حرف *typestr = type_منزل_Str(CV_MAKETYPE(ddepth, cn));
    إذا( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_سائيز_2, "convert_%s", typestr);
    }
    وإلا إذا( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_سائيز_2, "convert_%s_sat_rte", typestr);
    وإلا
        snprintf(buf, buf_سائيز_2, "convert_%s_sat", typestr);

    أرجع buf;
}

ثابت حرف* getOpenCLErrorString(صحيح error_ڪوڊ)
{
#وصف_ڪريو CV_OCL_ڪوڊ(id) حالة id: أرجع #id
#وصف_ڪريو CV_OCL_ڪوڊ_2(id, name) حالة id: أرجع #name
    بدل (error_ڪوڊ)
    {
    CV_OCL_ڪوڊ(CL_SUCCESS);
    CV_OCL_ڪوڊ(CL_DEVICE_NOT_مليو);
    CV_OCL_ڪوڊ(CL_DEVICE_NOT_موجود);
    CV_OCL_ڪوڊ(CL_COMPILER_NOT_موجود);
    CV_OCL_ڪوڊ(CL_MEM_OBJECT_ورهاست_FAILURE);
    CV_OCL_ڪوڊ(CL_ٻاهريون_جو_RESOURCES);
    CV_OCL_ڪوڊ(CL_ٻاهريون_جو_HOST_يادگيري);
    CV_OCL_ڪوڊ(CL_PROFILING_INFO_NOT_موجود);
    CV_OCL_ڪوڊ(CL_MEM_COPY_OVERLAP);
    CV_OCL_ڪوڊ(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_ڪوڊ(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_ڪوڊ(CL_BUILD_پروگرام_FAILURE);
    CV_OCL_ڪوڊ(CL_MAP_FAILURE);
    CV_OCL_ڪوڊ(CL_MISALIGNED_SUB_بفر_وٿي);
    CV_OCL_ڪوڊ(CL_EXEC_حالت_ERROR_FOR_EVENTS_اندريون_WAIT_LIST);
    CV_OCL_ڪوڊ(CL_COMPILE_پروگرام_FAILURE);
    CV_OCL_ڪوڊ(CL_LINKER_NOT_موجود);
    CV_OCL_ڪوڊ(CL_LINK_پروگرام_FAILURE);
    CV_OCL_ڪوڊ(CL_DEVICE_حصا_ڪريو_FAILED);
    CV_OCL_ڪوڊ(CL_KERNEL_ARG_INFO_NOT_موجود);
    CV_OCL_ڪوڊ(CL_INVALID_قدر);
    CV_OCL_ڪوڊ(CL_INVALID_DEVICE_TYPE);
    CV_OCL_ڪوڊ(CL_INVALID_PLATFORM);
    CV_OCL_ڪوڊ(CL_INVALID_DEVICE);
    CV_OCL_ڪوڊ(CL_INVALID_CONTEXT);
    CV_OCL_ڪوڊ(CL_INVALID_قطار_PROPERTIES);
    CV_OCL_ڪوڊ(CL_INVALID_COMMAND_قطار);
    CV_OCL_ڪوڊ(CL_INVALID_HOST_PTR);
    CV_OCL_ڪوڊ(CL_INVALID_MEM_OBJECT);
    CV_OCL_ڪوڊ(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_ڪوڊ(CL_INVALID_IMAGE_سائيز);
    CV_OCL_ڪوڊ(CL_INVALID_SAMPLER);
    CV_OCL_ڪوڊ(CL_INVALID_ٻٽو);
    CV_OCL_ڪوڊ(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_ڪوڊ(CL_INVALID_پروگرام);
    CV_OCL_ڪوڊ(CL_INVALID_پروگرام_EXECUTABLE);
    CV_OCL_ڪوڊ(CL_INVALID_KERNEL_NAME);
    CV_OCL_ڪوڊ(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_ڪوڊ(CL_INVALID_KERNEL);
    CV_OCL_ڪوڊ(CL_INVALID_ARG_اشاريو);
    CV_OCL_ڪوڊ(CL_INVALID_ARG_قدر);
    CV_OCL_ڪوڊ(CL_INVALID_ARG_سائيز);
    CV_OCL_ڪوڊ(CL_INVALID_KERNEL_دليلون);
    CV_OCL_ڪوڊ(CL_INVALID_WORK_DIMENSION);
    CV_OCL_ڪوڊ(CL_INVALID_WORK_گروهه_سائيز);
    CV_OCL_ڪوڊ(CL_INVALID_WORK_شي_سائيز);
    CV_OCL_ڪوڊ(CL_INVALID_GLOBAL_وٿي);
    CV_OCL_ڪوڊ(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_ڪوڊ(CL_INVALID_EVENT);
    CV_OCL_ڪوڊ(CL_INVALID_OPERATION);
    CV_OCL_ڪوڊ(CL_INVALID_GL_OBJECT);
    CV_OCL_ڪوڊ(CL_INVALID_بفر_سائيز);
    CV_OCL_ڪوڊ(CL_INVALID_MIP_LEVEL);
    CV_OCL_ڪوڊ(CL_INVALID_GLOBAL_WORK_سائيز);
    // OpenCL 1.1
    CV_OCL_ڪوڊ(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_ڪوڊ(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_ڪوڊ(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_ڪوڊ(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_ڪوڊ(CL_INVALID_DEVICE_حصا_ڪريو_ڳڻپ);
    // OpenCL 2.0
    CV_OCL_ڪوڊ_2(-69, CL_INVALID_PIPE_سائيز);
    CV_OCL_ڪوڊ_2(-70, CL_INVALID_DEVICE_قطار);
    // Extensions
    CV_OCL_ڪوڊ_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_ڪوڊ_2(-1001, CL_PLATFORM_NOT_مليو_KHR);
    CV_OCL_ڪوڊ_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_ڪوڊ_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_ڪوڊ_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_ڪوڊ_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    افتراضي: أرجع "Unknown OpenCL error";
    }
#وصف_رد CV_OCL_ڪوڊ
#وصف_رد CV_OCL_ڪوڊ_2
}

سانچو <قسم_نالو T>
ساكن std::string ker_منزل_Str(ثابت Mat & k)
{
    صحيح ويڪر = k.کانن_ڳڻپ - 1, اونهائي = k.اونهائي();
    ثابت T * ثابت معلومات = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    إذا (اونهائي <= CV_8S)
    {
        لكل (صحيح i = 0; i < ويڪر; ++i)
            stream << "DIG(" << (صحيح)معلومات[i] << ")";
        stream << "DIG(" << (صحيح)معلومات[ويڪر] << ")";
    }
    وإلا إذا (اونهائي == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < ويڪر; ++i)
            stream << "DIG(" << معلومات[i] << "f)";
        stream << "DIG(" << معلومات[ويڪر] << "f)";
    }
    وإلا إذا (اونهائي == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < ويڪر; ++i)
            stream << "DIG(" << (عائم)معلومات[i] << "h)";
        stream << "DIG(" << (عائم)معلومات[ويڪر] << "h)";
    }
    وإلا
    {
        لكل (صحيح i = 0; i < ويڪر; ++i)
            stream << "DIG(" << معلومات[i] << ")";
        stream << "DIG(" << معلومات[ويڪر] << ")";
    }

    أرجع stream.str();
}

String kernel_منزل_Str(داخلا_Array _kernel, صحيح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحيح اونهائي = kernel.اونهائي();
    إذا (ddepth < 0)
        ddepth = اونهائي;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    إذا (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    إذا (ddepth != اونهائي)
        kernel.convert_منزل(kernel, ddepth);

    تعريفنوع std::string (* func_t)(ثابت Mat &);
    ساكن ثابت func_t funcs[CV_اونهائي_MAX] = { ker_منزل_Str<uchar>, ker_منزل_Str<حرف>, ker_منزل_Str<ushort>, ker_منزل_Str<ننڍو>,
                                                ker_منزل_Str<صحيح>, ker_منزل_Str<عائم>, ker_منزل_Str<مزدوج>, ker_منزل_Str<hfloat>,
                                                0, ker_منزل_Str<منطقي>, ker_منزل_Str<uint64_t>, ker_منزل_Str<int64_t>, ker_منزل_Str<بنانشان> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    أرجع cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#وصف_ڪريو PROCESS_SRC(src) \
    افعل \
    { \
        إذا (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            سائيز csize = src.size(); \
            صحيح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_اونهائي(ctype), \
                ckercn = ويڪٽر_Widths[cdepth], cwidth = ccn * csize.ويڪر; \
            إذا (cwidth < ckercn || ckercn <= 0) \
                أرجع 1; \
            کانن_ڳڻپ.push_back(cwidth); \
            إذا (strat == OCL_ويڪٽر_OWN && ctype != ref_type) \
                أرجع 1; \
            offsets.push_back(src.offset()); \
            قدمن.push_back(src.قدم()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    طالما ((فراغ)0, 0)

صحيح predict_Optimal_ويڪٽر_ويڪر(داخلا_Array src1, داخلا_Array src2, داخلا_Array src3,
                              داخلا_Array src4, داخلا_Array src5, داخلا_Array src6,
                              داخلا_Array src7, داخلا_Array src8, داخلا_Array src9,
                              Ocl_ويڪٽر_Strategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحيح ويڪٽر_Widths[CV_اونهائي_MAX] = {
        d.preferred_ويڪٽر_ويڪر_Char(), d.preferred_ويڪٽر_ويڪر_Char(),   // CV_8U,  CV_8S
        d.preferred_ويڪٽر_ويڪر_Short(), d.preferred_ويڪٽر_ويڪر_Short(), // CV_16U, CV_16S
        d.preferred_ويڪٽر_ويڪر_Int(), d.preferred_ويڪٽر_ويڪر_Float(),   // CV_32S, CV_32F
        d.preferred_ويڪٽر_ويڪر_Double(), d.preferred_ويڪٽر_ويڪر_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_ويڪٽر_ويڪر_Char(),                                 // CV_Bool
        d.preferred_ويڪٽر_ويڪر_Long(), d.preferred_ويڪٽر_ويڪر_Long(),   // CV_64U, CV_64S
        d.preferred_ويڪٽر_ويڪر_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    إذا (ويڪٽر_Widths[0] == 1)
    {
        // it's heuristic
        ويڪٽر_Widths[CV_8U] = ويڪٽر_Widths[CV_8S] = 4;
        ويڪٽر_Widths[CV_16U] = ويڪٽر_Widths[CV_16S] = ويڪٽر_Widths[CV_16F] = 2;
        ويڪٽر_Widths[CV_32S] = ويڪٽر_Widths[CV_32F] = ويڪٽر_Widths[CV_64F] = 1;
    }

    أرجع check_Optimal_ويڪٽر_ويڪر(ويڪٽر_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحيح check_Optimal_ويڪٽر_ويڪر([[maybe_unused]] ثابت صحيح *ويڪٽر_Widths,
                            [[maybe_unused]] داخلا_Array src1,
                            [[maybe_unused]] داخلا_Array src2,
                            [[maybe_unused]] داخلا_Array src3,
                            [[maybe_unused]] داخلا_Array src4,
                            [[maybe_unused]] داخلا_Array src5,
                            [[maybe_unused]] داخلا_Array src6,
                            [[maybe_unused]] داخلا_Array src7,
                            [[maybe_unused]] داخلا_Array src8,
                            [[maybe_unused]] داخلا_Array src9,
                            [[maybe_unused]] Ocl_ويڪٽر_Strategy strat)
{
#جيڪڏهن_وصف_ٿيل __APPLE__
    أرجع 1;
#وإلا
    CV_Assert(ويڪٽر_Widths);

    صحيح ref_type = src1.type();

    std::vector<size_t> offsets, قدمن, کانن_ڳڻپ;
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
        طالما (offsets[i] % dividers[i] != 0 || قدمن[i] % dividers[i] != 0 || کانن_ڳڻپ[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحيح kercn = *std::min_element(kercns.begin(), kercns.end());

    أرجع kercn;
#جيڪڏهن_پورو
}

صحيح predict_Optimal_ويڪٽر_ويڪر_Max(داخلا_Array src1, داخلا_Array src2, داخلا_Array src3,
                                 داخلا_Array src4, داخلا_Array src5, داخلا_Array src6,
                                 داخلا_Array src7, داخلا_Array src8, داخلا_Array src9)
{
    أرجع predict_Optimal_ويڪٽر_ويڪر(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_ويڪٽر_MAX);
}

#وصف_رد PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
فراغ build_Options_جوڙيو_ميٽرڪس_Description(String& buildOptions, ثابت String& name, داخلا_Array _m)
{
    إذا (!buildOptions.empty())
        buildOptions += " ";
    صحيح type = _m.type(), اونهائي = CV_MAT_اونهائي(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_منزل_Str(type),
            name.c_str(), ocl::type_منزل_Str(CV_MAKE_TYPE(اونهائي, 1)),
            name.c_str(), (صحيح)CV_MAT_CN(type),
            name.c_str(), (صحيح)CV_ELEM_سائيز(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحيح)اونهائي
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

    ساكن cl_image_format getImageFormat(صحيح اونهائي, صحيح cn, منطقي norm)
    {
        cl_image_format format;
        ساكن ثابت صحيح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساكن ثابت صحيح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساكن ثابت صحيح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحيح channelType = norm ? channelTypesNorm[اونهائي] : channelTypes[اونهائي];
        صحيح channel_ترتيب = channelOrders[cn];
        format.image_channel_معلومات_type = (cl_channel_type)channelType;
        format.image_channel_ترتيب = (cl_channel_ترتيب)channel_ترتيب;
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
        cl_uint انگ_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, انگ_Formats,
                                                عدم, &انگ_Formats);
        CV_OCL_DBG_CHECK_نتيجو(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        إذا (انگ_Formats > 0)
        {
            Auto_بفر<cl_image_format> formats(انگ_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, انگ_Formats,
                                             formats.معلومات(), عدم);
            CV_OCL_DBG_CHECK_نتيجو(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            لكل (cl_uint i = 0; i < انگ_Formats; ++i)
            {
                إذا (!memcmp(&formats[i], &format, حجم(format)))
                {
                    أرجع سچ;
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

        صحيح err, اونهائي = src.اونهائي(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(اونهائي, cn, norm);

        إذا (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        إذا (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_قطار قطار_2 = (cl_command_قطار)قطار::getDefault().ptr();

#جيڪڏهن_وصف_ٿيل CL_نسخو_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحيح minor = d.device_نسخو_Minor(), major = d.device_نسخو_Major();
        CV_Assert(!alias || canCreateAlias(src));
        إذا (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_ويڪر      = src.کانن_ڳڻپ;
            desc.image_اوچائي     = src.صفون;
            desc.image_اونهائي      = 0;
            desc.image_array_سائيز = 1;
            desc.image_صف_pitch  = alias ? src.قدم[0] : 0;
            desc.image_ٽڪرو_pitch = 0;
            desc.بفر           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.انگ_mip_سطحون   = 0;
            desc.انگ_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, عدم, &err);
        }
        وإلا
#جيڪڏهن_پورو
        {
            CV_SUPPRESS_DEPRECATED_شروعات
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.کانن_ڳڻپ, src.صفون, 0, عدم, &err);
            CV_SUPPRESS_DEPRECATED_پڇاڙي
        }
        CV_OCL_DBG_CHECK_نتيجو(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.کانن_ڳڻپ), static_cast<size_t>(src.صفون), 1 };

        cl_mem dev_معلومات;
        إذا (!alias && !src.isContinuous())
        {
            dev_معلومات = cl_Create_بفر(context, CL_MEM_READ_ONLY, src.کانن_ڳڻپ * src.صفون * src.elem_سائيز(), عدم, &err);
            CV_OCL_CHECK_نتيجو(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (ڊگهو ڊگهو صحيح)(src.کانن_ڳڻپ * src.صفون * src.elem_سائيز()), (فراغ*)dev_معلومات
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.کانن_ڳڻپ) * src.elem_سائيز(), static_cast<size_t>(src.صفون), 1};
            CV_OCL_CHECK(cl_قطار_اندر_وجهو_Copy_بفر_Rect(قطار_2, (cl_mem)src.handle(ACCESS_READ), dev_معلومات, origin, origin,
                roi, src.قدم, 0, src.کانن_ڳڻپ * src.elem_سائيز(), 0, 0, عدم, عدم));
            CV_OCL_DBG_CHECK(clFlush(قطار_2));
        }
        وإلا
        {
            dev_معلومات = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_معلومات != عدم);

        إذا (!alias)
        {
            CV_OCL_CHECK(cl_قطار_اندر_وجهو_Copy_بفر_منزل_Image(قطار_2, dev_معلومات, handle, 0, origin, region, 0, عدم, 0));
            إذا (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(قطار_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_معلومات));
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
    إذا (d.image_From_بفر_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        إذا (pitchAlign && !(m.قدم % (pitchAlign * m.elem_سائيز())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            إذا (!m.u->عارضي_UMat())
            {
                ret = سچ;
            }
        }
    }
    أرجع ret;
}

منطقي Image2D::isFormatSupported(صحيح اونهائي, صحيح cn, منطقي norm)
{
    cl_image_format format = Impl::getImageFormat(اونهائي, cn, norm);

    أرجع Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    إذا (p)
        p->addref();
}

Image2D & Image2D::عامل = (ثابت Image2D & i)
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
    i.p = خالي_اشارو;
}

Image2D& Image2D::عامل = (Image2D&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا (p)
            p->release();
        p = i.p;
        i.p = خالي_اشارو;
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
        initialized = سچ;
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
        initialized = سچ;
    }
    أرجع value;
}

منطقي internal::isCLBuffer(UMat& u)
{
    إذا (isCudaBacked(u.u))
        أرجع خطأ;
    فراغ* h = u.handle(ACCESS_RW);
    إذا (!h)
        أرجع سچ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#إذا 1
    إذا ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        أرجع خطأ;
#وإلا
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, حجم(type), &type, عدم);
    إذا (ret != CL_SUCCESS || type != CL_MEM_OBJECT_بفر)
        أرجع خطأ;
#جيڪڏهن_پورو
    أرجع سچ;
}

بنية Timer::Impl
{
    ثابت قطار قطار_2;

    Impl(ثابت قطار& q)
        : قطار_2(q)
    {
    }

    ~Impl(){}

    فراغ شروعات()
    {
        CV_OCL_DBG_CHECK(cl_پڄاڻي((cl_command_قطار)قطار_2.ptr()));
        timer.شروعات();
    }

    فراغ stop()
    {
        CV_OCL_DBG_CHECK(cl_پڄاڻي((cl_command_قطار)قطار_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        أرجع (uint64)(timer.get_وقت_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت قطار& q) : p(جديد Impl(q)) { }
Timer::~Timer() { ميٽيو p; }

فراغ Timer::شروعات()
{
    CV_Assert(p);
    p->شروعات();
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

#جيڪڏهن_پورو // HAVE_OPENCL
