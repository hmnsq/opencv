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

#جے_بنا_تعریف HAVE_OPENCL
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
#جے_انت

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_رد CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/ocl_genbase.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "opencv2/core/utils/lock.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#تعریف_کرو CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "OpenCL allocator: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#تعریف_رد CV__ALLOCATOR_STATS_LOG

#تعریف_کرو CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#تعریف_کرو CV_OPENCL_SHOW_BUILD_OPTIONS             0
#تعریف_کرو CV_OPENCL_SHOW_BUILD_KERNELS             0

#تعریف_کرو CV_OPENCL_SHOW_لگاتار_KERNELS               0
#تعریف_کرو CV_OPENCL_SYNC_لگاتار_KERNELS               0
#تعریف_کرو CV_OPENCL_TRACE_CHECK                    0

#تعریف_کرو CV_OPENCL_VALIDATE_ثنائی_PROGRAMS       1

#تعریف_کرو CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#تعریف_کرو CV_OPENCL_SHOW_SVM_LOG                   0

#ضمن "opencv2/core/bufferpool.hpp"
#جے_بنا_تعریف LOG_بفر_POOL
# إذا 0
#   define LOG_بفر_POOL لکھو
# وإلا
#   define LOG_بفر_POOL(...)
# endif
#جے_انت

#إذا CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#تعریف_کرو CV_OPENCL_SVM_TRACE_P لکھو("line %d (ocl.cpp): ", __LINE__); لکھو
#وإلا
#تعریف_کرو CV_OPENCL_SVM_TRACE_P(...)
#جے_انت

#إذا CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#تعریف_کرو CV_OPENCL_SVM_TRACE_ERROR_P لکھو("Error on line %d (ocl.cpp): ", __LINE__); لکھو
#وإلا
#تعریف_کرو CV_OPENCL_SVM_TRACE_ERROR_P(...)
#جے_انت

#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"

#جے_تعریف_شدہ HAVE_OPENCL_SVM
#ضمن "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ضمن "opencv2/core/opencl/opencl_svm.hpp"
#جے_انت

#ضمن "umatrix.hpp"

نطاق cv { نطاق ocl {

#تعریف_کرو IMPLEMENT_REFCOUNTABLE() \
    فراغ addref() { CV_XADD(&refcount, 1); } \
    فراغ release() { إذا( CV_XADD(&refcount, -1) == 1 && !cv::__termination) مٹاؤ هذا; } \
    صحيح refcount

ساكن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    أرجع opencl_allocator_stats;
}

#جے_بنا_تعریف _DEBUG
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
#جے_انت

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
    std::باہر << "OpenCV(OpenCL:" << حالت << "): " << message << std::سطر_انت << std::flush;
}
#تعریف_کرو CV_OCL_TRACE_CHECK_نتیجہ(حالت, message) traceOpenCLCheck(حالت, message)
#وإلا
#تعریف_کرو CV_OCL_TRACE_CHECK_نتیجہ(حالت, message) /* nothing */
#جے_انت

#تعریف_کرو CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_نتیجہ), check_نتیجہ, پیغام)

#تعریف_کرو CV_OCL_CHECK_نتیجہ(check_نتیجہ, پیغام) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتیجہ(check_نتیجہ, پیغام); \
        إذا (check_نتیجہ != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام = CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام); \
        } \
    } طالما (0)

#تعریف_کرو CV_OCL_CHECK_(اظہاریہ, check_نتیجہ) افعل { اظہاریہ; CV_OCL_CHECK_نتیجہ(check_نتیجہ, #expr); } طالما (0)

#تعریف_کرو CV_OCL_CHECK(اظہاریہ) افعل { cl_int __cl_result = (اظہاریہ); CV_OCL_CHECK_نتیجہ(__cl_result, #expr); } طالما (0)

#جے_تعریف_شدہ _DEBUG
#تعریف_کرو CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, پیغام) CV_OCL_CHECK_نتیجہ(check_نتیجہ, پیغام)
#تعریف_کرو CV_OCL_DBG_CHECK(اظہاریہ) CV_OCL_CHECK(اظہاریہ)
#تعریف_کرو CV_OCL_DBG_CHECK_(اظہاریہ, check_نتیجہ) CV_OCL_CHECK_(اظہاریہ, check_نتیجہ)
#وإلا
#تعریف_کرو CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, پیغام) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتیجہ(check_نتیجہ, پیغام); \
        إذا (check_نتیجہ != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام = CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام); \
        } \
    } طالما (0)
#تعریف_کرو CV_OCL_DBG_CHECK_(اظہاریہ, check_نتیجہ) افعل { اظہاریہ; CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, #expr); } طالما (0)
#تعریف_کرو CV_OCL_DBG_CHECK(اظہاریہ) افعل { cl_int __cl_result = (اظہاریہ); CV_OCL_DBG_CHECK_نتیجہ(__cl_result, #expr); } طالما (0)
#جے_انت


ساكن ثابت منطقي CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_جندرا_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", سچ);
ساكن ثابت منطقي CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", سچ);

#إذا CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
ساكن ثابت منطقي CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", خطأ);
#جے_انت

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساكن ثابت منطقي CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#إذا 1 //def __APPLE__
        سچ
#وإلا
        خطأ
#جے_انت
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
ساكن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


بنية UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        قدم = (صحيح)m.قدم;
        صفاں = m.صفاں;
        خانے_گنتی = m.خانے_گنتی;
    }
    صحيح offset;
    صحيح قدم;
    صحيح صفاں;
    صحيح خانے_گنتی;
};

بنية UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        قدم = (صحيح)m.قدم.p[1];
        slicestep = (صحيح)m.قدم.p[0];
        slices = (صحيح)m.size.p[0];
        صفاں = m.size.p[1];
        خانے_گنتی = m.size.p[2];
    }
    صحيح offset;
    صحيح slicestep;
    صحيح قدم;
    صحيح slices;
    صحيح صفاں;
    صحيح خانے_گنتی;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ساكن uint64 crc64( ثابت uchar* انکڑے, size_t size, uint64 crc0=0 )
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

    uint64 چکری_جانچ = ~crc0;
    لكل( size_t idx = 0; idx < size; idx++ )
        چکری_جانچ = table[(uchar)چکری_جانچ ^ انکڑے[idx]] ^ (چکری_جانچ >> 8);

    أرجع ~چکری_جانچ;
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
بنية OpenCLBinaryCacheConfigurator
{
    cv::String cache_رستہ;
    cv::String cache_جندرا_filename;
    cv::Ptr<utils::fs::File_جندرا> cache_جندرا;

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
        cache_رستہ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        إذا (cache_رستہ.empty())
        {
            CV_LOG_INFO(عدم, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        افعل
        {
            حاول
            {
                إذا (cache_رستہ.empty())
                    اكسر;
                إذا (cache_رستہ == "disabled")
                    اكسر;
                إذا (!utils::fs::createDirectories(cache_رستہ))
                {
                    CV_LOG_DEBUG(عدم, "Can't use OpenCL cache directory: " << cache_رستہ);
                    clear();
                    اكسر;
                }

                إذا (CV_OPENCL_CACHE_جندرا_ENABLE)
                {
                    cache_جندرا_filename = cache_رستہ + ".lock";
                    إذا (!utils::fs::exists(cache_جندرا_filename))
                    {
                        CV_LOG_DEBUG(عدم, "Creating lock file... (" << cache_جندرا_filename << ")");
                        std::ofstream جندرا_filename(cache_جندرا_filename.c_str(), std::ios::out);
                        إذا (!جندرا_filename.is_open())
                        {
                            CV_LOG_WARNING(عدم, "Can't create lock file for OpenCL program cache: " << cache_جندرا_filename);
                            اكسر;
                        }
                    }

                    حاول
                    {
                        cache_جندرا = makePtr<utils::fs::File_جندرا>(cache_جندرا_filename.c_str());
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... (" << cache_جندرا_filename << ")");
                        {
                            utils::shared_جندرا_guard<utils::fs::File_جندرا> lock(*cache_جندرا);
                        }
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... Done!");
                    }
                    امسك (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_جندرا_filename << std::سطر_انت << e.what());
                    }
                    امسك (...)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_جندرا_filename);
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
                CV_LOG_WARNING(عدم, "Can't prepare OpenCL program cache: " << cache_رستہ << std::سطر_انت << e.what());
                clear();
            }
        } طالما (0);
        إذا (!cache_رستہ.empty())
        {
            إذا (cache_جندرا.empty() && CV_OPENCL_CACHE_جندرا_ENABLE)
            {
                CV_LOG_WARNING(عدم, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Successfully initialized OpenCL cache directory: " << cache_رستہ);
            }
        }
    }

    فراغ clear()
    {
        cache_رستہ.clear();
        cache_جندرا_filename.clear();
        cache_جندرا.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_اگیتر,
            ثابت std::string& cleanup_اگیتر)
    {
        إذا (cache_رستہ.empty())
            أرجع std::string();

        Auto_جندرا lock(mutex_prepared_contexts_);

        ContextCacheType::iterator لبھیا_it = prepared_contexts_.find(ctx_اگیتر);
        إذا (لبھیا_it != prepared_contexts_.end())
            أرجع لبھیا_it->second;

        CV_LOG_INFO(عدم, "Preparing OpenCL cache configuration for context: " << ctx_اگیتر);

        std::string نشانہ_directory = cache_رستہ + ctx_اگیتر + "/";
        منطقي نتیجہ = utils::fs::isDirectory(نشانہ_directory);
        إذا (!نتیجہ)
        {
            حاول
            {
                CV_LOG_VERBOSE(عدم, 0, "Creating directory: " << نشانہ_directory);
                إذا (utils::fs::createDirectories(نشانہ_directory))
                {
                    نتیجہ = سچ;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "Can't create directory: " << نشانہ_directory);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCL program cache directory for context: " << نشانہ_directory << std::سطر_انت << e.what());
            }
        }
        نشانہ_directory = نتیجہ ? نشانہ_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_اگیتر, نشانہ_directory));

        إذا (نتیجہ && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_اگیتر.empty())
        {
            حاول
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_رستہ, cleanup_اگیتر + "*", entries, خطأ, سچ);
                std::vector<String> remove_entries;
                لكل (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    إذا (0 == name.find(cleanup_اگیتر))
                    {
                        إذا (0 == name.find(ctx_اگیتر))
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
                        cv::String رستہ = utils::fs::join(cache_رستہ, name);
                        حاول
                        {
                            utils::fs::remove_all(رستہ);
                            CV_LOG_WARNING(عدم, "Removed: " << رستہ);
                        }
                        امسك (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(عدم, "Exception during removal of obsolete OpenCL cache directory: " << رستہ << std::سطر_انت << e.what());
                        }
                    }
                }
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(عدم, 1, "  Result: " << (نشانہ_directory.empty() ? std::string("Failed") : نشانہ_directory));
        أرجع نشانہ_directory;
    }

    ساكن OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, جديد OpenCLBinaryCacheConfigurator());
    }
};
صنف ثنائی_پروگرام_File
{
    تعداد { MAX_ENTRIES = 64 };

    تعريفنوع بنانشانی صحيح uint32_t;

    بنية CV_DECL_ALIGNED(4) File_سرخی
    {
        uint32_t سروت_Signature_ناپ;
        //char sourceSignature[];
    };

    بنية CV_DECL_ALIGNED(4) File_جدول
    {
        uint32_t نمبر_دا_Entries;
        //uint32_t firstEntryOffset[];
    };

    بنية CV_DECL_ALIGNED(4) File_اندراج
    {
        uint32_t اگلا_اندراج_File_وتھ; // 0 for the last entry in chain
        uint32_t کلید_ناپ;
        uint32_t انکڑے_ناپ;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت سروت_Signature;
    ثابت size_t سروت_Signature_ناپ_2;

    std::fstream f;

    uint32_t اندراج_Offsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        أرجع hash & (MAX_ENTRIES - 1);
    }

    مضمن size_t get_File_ناپ()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_ناپ = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        أرجع file_ناپ;
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
    ثنائی_پروگرام_File(ثابت std::string& fileName, ثابت حرف* سروت_Signature_2)
        : fileName_(fileName), سروت_Signature(سروت_Signature_2), سروت_Signature_ناپ_2(سروت_Signature ? لڑی_لمبائی(سروت_Signature) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(سروت_Signature != عدم);
        CV_Assert(سروت_Signature_ناپ_2 > 0);
        memset(اندراج_Offsets, 0, حجم(اندراج_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        إذا(f.is_open() && get_File_ناپ() > 0)
        {
            منطقي isValid = خطأ;
            حاول
            {
                uint32_t file_سروت_Signature_ناپ = readUInt32();
                إذا (file_سروت_Signature_ناپ == سروت_Signature_ناپ_2)
                {
                    cv::Auto_بفر<حرف> file_سروت_Signature(file_سروت_Signature_ناپ + 1);
                    f.اقرأ(file_سروت_Signature.انکڑے(), file_سروت_Signature_ناپ);
                    إذا (f.eof())
                    {
                        CV_LOG_ERROR(عدم, "Unexpected EOF");
                    }
                    وإلا إذا (memcmp(سروت_Signature_2, file_سروت_Signature.انکڑے(), file_سروت_Signature_ناپ) == 0)
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

    منطقي اقرأ(ثابت std::string& کلید, std::vector<حرف>& buf)
    {
        إذا (!f.is_open())
            أرجع خطأ;

        size_t file_ناپ = get_File_ناپ();
        إذا (file_ناپ == 0)
        {
            CV_LOG_ERROR(عدم, "Invalid file (empty): " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_سروت_Signature_ناپ = readUInt32();
        CV_Assert(file_سروت_Signature_ناپ > 0);
        seekReadRelative(file_سروت_Signature_ناپ);

        uint32_t نمبر_دا_Entries = readUInt32();
        CV_Assert(نمبر_دا_Entries > 0);
        إذا (نمبر_دا_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        f.اقرأ((حرف*)&اندراج_Offsets[0], حجم(اندراج_Offsets));
        CV_Assert(!f.fail());

        uint32_t اندراج_عدد = getHash(کلید);

        uint32_t اندراج_وتھ = اندراج_Offsets[اندراج_عدد];
        File_اندراج اندراج;
        طالما (اندراج_وتھ > 0)
        {
            seekReadAbsolute(اندراج_وتھ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&اندراج, حجم(اندراج));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلید(اندراج.کلید_ناپ + 1);
            إذا (کلید.size() == اندراج.کلید_ناپ)
            {
                إذا (اندراج.کلید_ناپ > 0)
                {
                    f.اقرأ(file_کلید.انکڑے(), اندراج.کلید_ناپ);
                    CV_Assert(!f.fail());
                }
                إذا (memcmp(file_کلید.انکڑے(), کلید.c_str(), اندراج.کلید_ناپ) == 0)
                {
                    buf.resize(اندراج.انکڑے_ناپ);
                    f.اقرأ(&buf[0], اندراج.انکڑے_ناپ);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(عدم, 0, "Read...");
                    أرجع سچ;
                }
            }
            إذا (اندراج.اگلا_اندراج_File_وتھ == 0)
                اكسر;
            اندراج_وتھ = اندراج.اگلا_اندراج_File_وتھ;
        }
        أرجع خطأ;
    }

    منطقي اكتب(ثابت std::string& کلید, std::vector<حرف>& buf)
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

        size_t file_ناپ = get_File_ناپ();
        إذا (file_ناپ == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)سروت_Signature_ناپ_2);
            f.اكتب(سروت_Signature, سروت_Signature_ناپ_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(اندراج_Offsets, 0, حجم(اندراج_Offsets));
            f.اكتب((حرف*)اندراج_Offsets, حجم(اندراج_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_ناپ = get_File_ناپ();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_سروت_Signature_ناپ = readUInt32();
        CV_Assert(file_سروت_Signature_ناپ == سروت_Signature_ناپ_2);
        seekReadRelative(file_سروت_Signature_ناپ);

        uint32_t نمبر_دا_Entries = readUInt32();
        CV_Assert(نمبر_دا_Entries > 0);
        إذا (نمبر_دا_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        size_t جدول_Entries_وتھ = (size_t)f.tellg();
        f.اقرأ((حرف*)&اندراج_Offsets[0], حجم(اندراج_Offsets));
        CV_Assert(!f.fail());

        uint32_t اندراج_عدد = getHash(کلید);

        uint32_t اندراج_وتھ = اندراج_Offsets[اندراج_عدد];
        File_اندراج اندراج;
        طالما (اندراج_وتھ > 0)
        {
            seekReadAbsolute(اندراج_وتھ);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&اندراج, حجم(اندراج));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلید(اندراج.کلید_ناپ + 1);
            إذا (کلید.size() == اندراج.کلید_ناپ)
            {
                إذا (اندراج.کلید_ناپ > 0)
                {
                    f.اقرأ(file_کلید.انکڑے(), اندراج.کلید_ناپ);
                    CV_Assert(!f.fail());
                }
                إذا (0 == memcmp(file_کلید.انکڑے(), کلید.c_str(), اندراج.کلید_ناپ))
                {
                    // duplicate
                    CV_LOG_VERBOSE(عدم, 0, "Duplicate key ignored: " << fileName_);
                    أرجع خطأ;
                }
            }
            إذا (اندراج.اگلا_اندراج_File_وتھ == 0)
                اكسر;
            اندراج_وتھ = اندراج.اگلا_اندراج_File_وتھ;
        }
        seekReadAbsolute(0);
        إذا (اندراج_وتھ > 0)
        {
            seekWriteAbsolute(اندراج_وتھ);
            اندراج.اگلا_اندراج_File_وتھ = (uint32_t)file_ناپ;
            f.اكتب((حرف*)&اندراج, حجم(اندراج));
            CV_Assert(!f.fail());
        }
        وإلا
        {
            اندراج_Offsets[اندراج_عدد] = (uint32_t)file_ناپ;
            seekWriteAbsolute(جدول_Entries_وتھ);
            f.اكتب((حرف*)اندراج_Offsets, حجم(اندراج_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_ناپ);
        اندراج.اگلا_اندراج_File_وتھ = 0;
        اندراج.انکڑے_ناپ = (uint32_t)buf.size();
        اندراج.کلید_ناپ = (uint32_t)کلید.size();
        f.اكتب((حرف*)&اندراج, حجم(اندراج));
        CV_Assert(!f.fail());
        f.اكتب(کلید.c_str(), اندراج.کلید_ناپ);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], اندراج.انکڑے_ناپ);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(عدم, 0, "Write... (" << buf.size() << " bytes)");
        أرجع سچ;
    }
};
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT



بنية OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحيح device_;  // device index in context
    ocl::قطار قطار_3;
    صحيح useOpenCL_;

محمي:
    Impl() = مٹاؤ;

    فراغ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي لبھیا = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            إذا (dhandle == deviceID)
            {
                device_ = i;
                لبھیا = سچ;
                اكسر;
            }
        }
        CV_Assert(لبھیا && "OpenCL device can't work with passed OpenCL context");
    }

    فراغ _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي لبھیا = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            إذا (d.getImpl() == device.getImpl())
            {
                device_ = i;
                لبھیا = سچ;
                اكسر;
            }
        }
        CV_Assert(لبھیا && "OpenCL device can't work with passed OpenCL context");
    }

عام:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_بن_ورتیا(platformID);
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
                    useOpenCL_ = d.دستیاب();
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

    فراغ سیٹ_Use_Open_CL(منطقي flag)
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
            cv::Auto_جندرا lock(getInitializationMutex());
            إذا (!initialized)
            {
                CV_LOG_INFO(عدم, "OpenCL: creating new execution context...");
                حاول
                {
                    Context c = ocl::Context::create(std::string());
                    إذا (c.ndevices())
                    {
                        صحيح deviceId = 0;
                        خودکار& d = c.device(deviceId);
                        إذا (d.دستیاب())
                        {
                            خودکار q = ocl::قطار(c, d);
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
فراغ OpenCLExecutionContext::سیٹ_Use_Open_CL(منطقي flag)
{
    CV_Assert(p);
    p->سیٹ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& انکڑے = get_Core_Tls_انکڑے();
    OpenCLExecutionContext& c = انکڑے.oclExecutionContext;
    إذا (!انکڑے.oclExecutionContextInitialized)
    {
        انکڑے.oclExecutionContextInitialized = سچ;
        إذا (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    أرجع c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& انکڑے = get_Core_Tls_انکڑے();
    OpenCLExecutionContext& c = انکڑے.oclExecutionContext;
    أرجع c;
}

فراغ OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& انکڑے = get_Core_Tls_انکڑے();
    انکڑے.oclExecutionContext = *هذا;
    انکڑے.oclExecutionContextInitialized = سچ;
    انکڑے.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
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
        std::string env_رستہ = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        إذا (!env_رستہ.empty())
        {
            إذا (env_رستہ == "disabled")
            {
                g_isOpenCLAvailable = خطأ;
                g_isOpenCLInitialized = سچ;
                أرجع خطأ;
            }
        }

        cv::Auto_جندرا lock(getInitializationMutex());
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
    CoreTLSData& انکڑے = get_Core_Tls_انکڑے();
    إذا (انکڑے.useOpenCL < 0)
    {
        حاول
        {
            انکڑے.useOpenCL = 0;
            إذا (haveOpenCL())
            {
                خودکار c = OpenCLExecutionContext::getCurrent();
                انکڑے.useOpenCL = c.useOpenCL();
            }
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    أرجع انکڑے.useOpenCL > 0;
}

منطقي isOpenCLActivated()
{
    إذا (!g_isOpenCLAvailable)
        أرجع خطأ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    أرجع useOpenCL();
}

فراغ سیٹ_Use_Open_CL(منطقي flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& انکڑے = get_Core_Tls_انکڑے();
    خودکار& c = OpenCLExecutionContext::getCurrentRef();
    إذا (!c.empty())
    {
        c.سیٹ_Use_Open_CL(flag);
        انکڑے.useOpenCL = c.useOpenCL();
    }
    وإلا
    {
        إذا (!flag)
            انکڑے.useOpenCL = 0;
        وإلا
            انکڑے.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#جے_تعریف_شدہ HAVE_CLAMDBLAS

صنف AmdBlasHelper
{
عام:
    ساكن AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, جديد AmdBlasHelper())
    }

    منطقي is_دستیاب() ثابت
    {
        أرجع g_is_Amd_Blas_دستیاب;
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
            Auto_جندرا lock(getInitializationMutex());

            إذا (!g_isAmdBlasInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        g_is_Amd_Blas_دستیاب = clblas_تیاری() == clblasSuccess;
                    }
                    امسك (...)
                    {
                        g_is_Amd_Blas_دستیاب = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Blas_دستیاب = خطأ;

                g_isAmdBlasInitialized = سچ;
            }
        }
    }

خاص:
    ساكن منطقي g_isAmdBlasInitialized;
    ساكن منطقي g_is_Amd_Blas_دستیاب;
};

منطقي AmdBlasHelper::g_is_Amd_Blas_دستیاب = خطأ;
منطقي AmdBlasHelper::g_isAmdBlasInitialized = خطأ;

منطقي haveAmdBlas()
{
    أرجع AmdBlasHelper::getInstance().is_دستیاب();
}

#وإلا

منطقي haveAmdBlas()
{
    أرجع خطأ;
}

#جے_انت

#جے_تعریف_شدہ HAVE_CLAMDFFT

صنف AmdFftHelper
{
عام:
    ساكن AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, جديد AmdFftHelper())
    }

    منطقي is_دستیاب() ثابت
    {
        أرجع g_is_Amd_Fft_دستیاب;
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
            Auto_جندرا lock(getInitializationMutex());

            إذا (!g_isAmdFftInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_تیاری_انکڑے(&تیاری_انکڑے) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_نسخہ(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_دستیاب = سچ;
                    }
                    امسك (ثابت Exception &)
                    {
                        g_is_Amd_Fft_دستیاب = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Fft_دستیاب = خطأ;

                g_isAmdFftInitialized = سچ;
            }
        }
    }

خاص:
    ساكن clfft_تیاری_انکڑے تیاری_انکڑے;
    ساكن منطقي g_isAmdFftInitialized;
    ساكن منطقي g_is_Amd_Fft_دستیاب;
};

clfft_تیاری_انکڑے AmdFftHelper::تیاری_انکڑے;
منطقي AmdFftHelper::g_is_Amd_Fft_دستیاب = خطأ;
منطقي AmdFftHelper::g_isAmdFftInitialized = خطأ;

منطقي haveAmdFft()
{
    أرجع AmdFftHelper::getInstance().is_دستیاب();
}

#وإلا

منطقي haveAmdFft()
{
    أرجع خطأ;
}

#جے_انت

منطقي haveSVM()
{
#جے_تعریف_شدہ HAVE_OPENCL_SVM
    أرجع سچ;
#وإلا
    أرجع خطأ;
#جے_انت
}

فراغ انت_2()
{
    قطار::getDefault().انت_2();
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
    pl.p = خالی_سوچک;
}

Platform& Platform::عامل = (Platform&& pl) CV_NOEXCEPT
{
    إذا (هذا != &pl) {
        إذا(p)
            p->release();
        p = pl.p;
        pl.p = خالی_سوچک;
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
ساكن فراغ parseOpenCLVersion(ثابت String &نسخہ, صحيح &major, صحيح &minor)
{
    major = minor = 0;
    إذا (10 >= نسخہ.length())
        أرجع;
    ثابت حرف *pstr = نسخہ.c_str();
    إذا (0 != لڑی_مقابلہ(pstr, "OpenCL ", 7))
        أرجع;
    size_t ppos = نسخہ.find('.', 7);
    إذا (String::npos == ppos)
        أرجع;
    String عارضی_2 = نسخہ.substr(7, ppos - 7);
    major = atoi(عارضی_2.c_str());
    عارضی_2 = نسخہ.substr(ppos + 1);
    minor = atoi(عارضی_2.c_str());
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
        نسخہ_3 = getStrProp(CL_DEVICE_نسخہ);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_حافظہ = getBoolProp(CL_DEVICE_HOST_UNIFIED_حافظہ);
        maxComputeUnits_ = getProp<cl_uint, صحيح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_گروہ_ناپ_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_گروہ_ناپ);
        type_ = getProp<cl_device_type, صحيح>(CL_DEVICE_TYPE);
        driver_نسخہ_2 = getStrProp(CL_DRIVER_نسخہ);
        پتہ_بٹ_2 = getProp<cl_uint, صحيح>(CL_DEVICE_پتہ_بٹ);

        String device_نسخہ = getStrProp(CL_DEVICE_نسخہ);
        parseOpenCLVersion(device_نسخہ, device_نسخہ_Major_2, device_نسخہ_Minor_2);

        size_t pos = 0;
        طالما (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            إذا (pos2 == String::npos)
                pos2 = extensions_.size();
            إذا (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_سیٹ.insert(extensionName);
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
        وإلا إذا (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || لڑی_کھوج(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وإلا إذا (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وإلا
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_گروہ_ناپ = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        إذا (CV_OPENCL_DEVICE_MAX_WORK_گروہ_ناپ > 0)
        {
            ثابت size_t new_max_Work_گروہ_ناپ = std::min(max_Work_گروہ_ناپ_2, CV_OPENCL_DEVICE_MAX_WORK_گروہ_ناپ);
            إذا (new_max_Work_گروہ_ناپ != max_Work_گروہ_ناپ_2)
                CV_LOG_WARNING(عدم, "OpenCL: using workgroup size: " << new_max_Work_گروہ_ناپ << " (was " << max_Work_گروہ_ناپ_2 << ")");
            max_Work_گروہ_ناپ_2 = new_max_Work_گروہ_ناپ;
        }
#إذا 0
        إذا (isExtensionSupported("cl_khr_spir"))
        {
#جے_بنا_تعریف CL_DEVICE_SPIR_VERSIONS
#تعریف_کرو CL_DEVICE_SPIR_VERSIONS                     0x40E0
#جے_انت
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::باہر << spir_versions << std::سطر_انت;
        }
#جے_انت
    }

    ~Impl()
    {
#جے_تعریف_شدہ _WIN32
        إذا (!cv::__termination)
#جے_انت
        {
            إذا (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    سانچا<قسم_ناں _TpCL, قسم_ناں Tp_باہرلا>
    Tp_باہرلا getProp(cl_device_info prop) ثابت
    {
        _TpCL عارضی_2=_TpCL();
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(عارضی_2), &عارضی_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضی_2) ? Tp_باہرلا(عارضی_2) : Tp_باہرلا();
    }

    منطقي getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool عارضی_2 = CL_FALSE;
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(عارضی_2), &عارضی_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضی_2) ? عارضی_2 != 0 : خطأ;
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
        أرجع extensions_سیٹ.گنتی_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String نسخہ_3;
    std::string extensions_;
    صحيح doubleFPConfig_;
    منطقي khr_fp64_support_;
    صحيح halfFPConfig_;
    منطقي khr_fp16_support_;
    منطقي host_Unified_حافظہ;
    صحيح maxComputeUnits_;
    size_t max_Work_گروہ_ناپ_2;
    صحيح type_;
    صحيح پتہ_بٹ_2;
    صحيح device_نسخہ_Major_2;
    صحيح device_نسخہ_Minor_2;
    String driver_نسخہ_2;
    String vendorName_;
    صحيح vendorID_;
    منطقي intelSubgroupsSupport_;

    std::سیٹ<std::string> extensions_سیٹ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(فراغ* d)
{
    p = 0;
    سیٹ(d);
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
    d.p = خالی_سوچک;
}

Device& Device::عامل = (Device&& d) CV_NOEXCEPT
{
    إذا (هذا != &d) {
        إذا(p)
            p->release();
        p = d.p;
        d.p = خالی_سوچک;
    }
    أرجع *هذا;
}

Device::~Device()
{
    إذا(p)
        p->release();
}

فراغ Device::سیٹ(فراغ* d)
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

String Device::نسخہ() ثابت
{ أرجع p ? p->نسخہ_3 : String(); }

String Device::vendorName() ثابت
{ أرجع p ? p->vendorName_ : String(); }

صحيح Device::vendorID() ثابت
{ أرجع p ? p->vendorID_ : 0; }

String Device::Open_CL_C_نسخہ() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_OPENCL_C_نسخہ) : String(); }

String Device::OpenCLVersion() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_نسخہ) : String(); }

صحيح Device::device_نسخہ_Major() ثابت
{ أرجع p ? p->device_نسخہ_Major_2 : 0; }

صحيح Device::device_نسخہ_Minor() ثابت
{ أرجع p ? p->device_نسخہ_Minor_2 : 0; }

String Device::driver_نسخہ() ثابت
{ أرجع p ? p->driver_نسخہ_2 : String(); }

صحيح Device::type() ثابت
{ أرجع p ? p->type_ : 0; }

صحيح Device::پتہ_بٹ() ثابت
{ أرجع p ? p->پتہ_بٹ_2 : 0; }

منطقي Device::دستیاب() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_دستیاب) : خطأ; }

منطقي Device::compiler_دستیاب() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_COMPILER_دستیاب) : خطأ; }

منطقي Device::linker_دستیاب() ثابت
#جے_تعریف_شدہ CL_نسخہ_1_2
{ أرجع p ? p->getBoolProp(CL_DEVICE_LINKER_دستیاب) : خطأ; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جے_انت

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

size_t Device::global_Mem_Cache_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_ناپ) : 0; }

صحيح Device::globalMemCacheType() ثابت
{ أرجع p ? p->getProp<cl_device_mem_cache_type, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحيح Device::global_Mem_Cache_سطر_ناپ() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_ناپ) : 0; }

size_t Device::global_Mem_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_ناپ) : 0; }

size_t Device::local_Mem_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_ناپ) : 0; }

صحيح Device::localMemType() ثابت
{ أرجع p ? p->getProp<cl_device_local_mem_type, صحيح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقي Device::host_Unified_حافظہ_2() ثابت
{ أرجع p ? p->host_Unified_حافظہ : خطأ; }

منطقي Device::imageSupport() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : خطأ; }

منطقي Device::image_From_بفر_Support() ثابت
{
    أرجع p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : خطأ;
}

uint Device::imagePitchAlignment() ثابت
{
#جے_تعریف_شدہ CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#جے_انت
}

uint Device::image_ادھار_پتہ_Alignment() ثابت
{
#جے_تعریف_شدہ CL_DEVICE_IMAGE_ادھار_پتہ_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_ادھار_پتہ_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#جے_انت
}

size_t Device::image2_DMax_چوڑائی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_چوڑائی) : 0; }

size_t Device::image2_DMax_اچائی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_اچائی) : 0; }

size_t Device::image3_DMax_چوڑائی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_چوڑائی) : 0; }

size_t Device::image3_DMax_اچائی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_اچائی) : 0; }

size_t Device::image3_DMax_ڈونگھائی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ڈونگھائی) : 0; }

size_t Device::image_Max_بفر_ناپ() ثابت
#جے_تعریف_شدہ CL_نسخہ_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_بفر_ناپ) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جے_انت

size_t Device::image_Max_Array_ناپ() ثابت
#جے_تعریف_شدہ CL_نسخہ_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_ناپ) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جے_انت

منطقي Device::intelSubgroupsSupport() ثابت
{ أرجع p ? p->intelSubgroupsSupport_ : خطأ; }

صحيح Device::maxClockFrequency() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحيح Device::maxComputeUnits() ثابت
{ أرجع p ? p->maxComputeUnits_ : 0; }

صحيح Device::max_Constant_دلیلاں() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CONSTANT_دلیلاں) : 0; }

size_t Device::max_Constant_بفر_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_بفر_ناپ) : 0; }

size_t Device::max_Mem_Alloc_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_ناپ) : 0; }

size_t Device::max_Parameter_ناپ() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_ناپ) : 0; }

صحيح Device::max_Read_Image_دلیلاں() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_READ_IMAGE_دلیلاں) : 0; }

صحيح Device::max_Write_Image_دلیلاں() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WRITE_IMAGE_دلیلاں) : 0; }

صحيح Device::maxSamplers() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_گروہ_ناپ() ثابت
{ أرجع p ? p->max_Work_گروہ_ناپ_2 : 0; }

صحيح Device::max_Work_شے_Dims() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WORK_شے_DIMENSIONS) : 0; }

فراغ Device::max_Work_شے_Sizes(size_t* sizes) ثابت
{
    إذا(p)
    {
        ثابت صحيح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_شے_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحيح Device::mem_ادھار_Addr_Align() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MEM_ادھار_ADDR_ALIGN) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_CHAR) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_SHORT) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_INT) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_LONG) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_FLOAT) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_DOUBLE) : 0; }

صحيح Device::native_سمتیہ_چوڑائی_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_HALF) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_CHAR) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_SHORT) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_INT) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_LONG) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_FLOAT) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_DOUBLE) : 0; }

صحيح Device::preferred_سمتیہ_چوڑائی_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_HALF) : 0; }

size_t Device::printf_بفر_ناپ() ثابت
#جے_تعریف_شدہ CL_نسخہ_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_بفر_ناپ) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#جے_انت


size_t Device::profilingTimerResolution() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    خودکار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        أرجع c.getDevice();
    }

    ساكن Device dummy;
    أرجع dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

سانچا <قسم_ناں Functor, قسم_ناں ObjectType>
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
        حرف* ptr = buf.انکڑے(); // cleanup is not needed
        err = f(obj, name, required, ptr, عدم);
        إذا (err != CL_SUCCESS)
            أرجع err;
        param = ptr;
    }

    أرجع CL_SUCCESS;
}

ساكن فراغ ونڈو(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    إذا (s.size() == 0)
        أرجع;
    std::istringstream ss(s);
    std::string شے;
    طالما (!ss.eof())
    {
        std::getline(ss, شے, delim);
        elems.push_back(شے);
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
    ونڈو(configurationStr, ':', parts);
    إذا (parts.size() > 3)
    {
        CV_LOG_ERROR(عدم, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        أرجع خطأ;
    }
    إذا (parts.size() > 2)
        deviceNameOrID = parts[2];
    إذا (parts.size() > 1)
    {
        ونڈو(parts[1], '|', deviceTypes);
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
            إذا (!ہندسہ_اے(deviceName[i]))
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
        cl_uint عدد_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &عدد_Platforms));

        إذا (عدد_Platforms == 0)
            أرجع عدم;
        platforms.resize((size_t)عدد_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(عدد_Platforms, &platforms[0], &عدد_Platforms));
        platforms.resize(عدد_Platforms);
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
            اذهب not_لبھیا;
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
        std::string عارضی_Str_Device_Type = deviceTypes[t];
        std::transform(عارضی_Str_Device_Type.begin(), عارضی_Str_Device_Type.end(), عارضی_Str_Device_Type.begin(), details::char_tolower);

        إذا (عارضی_Str_Device_Type == "gpu" || عارضی_Str_Device_Type == "dgpu" || عارضی_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        وإلا إذا (عارضی_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        وإلا إذا (عارضی_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وإلا إذا (عارضی_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        وإلا
        {
            CV_LOG_ERROR(عدم, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            اذهب not_لبھیا;
        }

        std::vector<cl_device_id> devices;
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint گنتی_2 = 0;
            cl_int حالت = clGetDeviceIDs(*currentPlatform, deviceType, 0, عدم, &گنتی_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_لبھیا))
            {
                CV_OCL_DBG_CHECK_نتیجہ(حالت, "clGetDeviceIDs get count");
            }
            إذا (گنتی_2 == 0)
                استمر;
            size_t ادھار = devices.size();
            devices.resize(ادھار + گنتی_2);
            حالت = clGetDeviceIDs(*currentPlatform, deviceType, گنتی_2, &devices[ادھار], &گنتی_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_لبھیا))
            {
                CV_OCL_DBG_CHECK_نتیجہ(حالت, "clGetDeviceIDs get IDs");
            }
        }

        لكل (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : سچ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = سچ;
            إذا(عارضی_Str_Device_Type == "dgpu" || عارضی_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_حافظہ, حجم(isIGPU), &isIGPU, عدم));
                useGPU = عارضی_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            إذا ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                أرجع devices[i];
            }
        }
    }

not_لبھیا:
    إذا (configuration.empty())
        أرجع عدم; // suppress messages on stderr

    std::ostringstream پیغام;
    پیغام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::سطر_انت
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::سطر_انت
        << "    Device types:";
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
        پیغام << ' ' << deviceTypes[t];

    پیغام << std::سطر_انت << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(عدم, پیغام.str());
    أرجع عدم;
}

#جے_تعریف_شدہ HAVE_OPENCL_SVM
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
ساكن بنانشانی صحيح get_SVMCapabilities_نقاب()
{
    ساكن منطقي initialized = خطأ;
    ساكن بنانشانی صحيح نقاب = 0;
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
#جے_انت

ساكن size_t get_پروگرام_گنتی_حد_بندی()
{
    ساكن منطقي initialized = خطأ;
    ساكن size_t گنتی_2 = 0;
    إذا (!initialized)
    {
        گنتی_2 = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = سچ;
    }
    أرجع گنتی_2;
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
#جے_تعریف_شدہ HAVE_OPENCL_SVM
        , svmInitialized(خطأ)
#جے_انت
    {
        إذا (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_جندرا lock(cv::getInitializationMutex());
        خودکار& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = هذا;
    }

    ~Impl()
    {
#جے_تعریف_شدہ _WIN32
        إذا (!cv::__termination)
#جے_انت
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
            cv::Auto_جندرا lock(cv::getInitializationMutex());
            خودکار& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = عدم;
        }
    }

    فراغ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_عدد_DEVICES, حجم(ndevices), &ndevices, عدم));
        CV_Assert(ndevices > 0);

        cv::Auto_بفر<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_ناپ = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_ناپ));
        CV_CheckEQ(devices_ret_ناپ, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        لكل (بنانشانی i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    فراغ __init_buffer_pools();  // w/o synchronization
    فراغ init_بفر_pools() ثابت
    {
        إذا (!بفر_Pool_2)
        {
            cv::Auto_جندرا lock(cv::getInitializationMutex());
            إذا (!بفر_Pool_2)
            {
                const_cast<Impl*>(هذا)->__init_buffer_pools();
            }
        }
    }
عام:
    ساكن Impl* لبھو_Context(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_جندرا lock(cv::getInitializationMutex());
        خودکار& container = getGlobalContainer();
        إذا (configuration.empty() && !container.empty())
            أرجع container[0];
        لكل (خودکار it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            إذا (i && i->configuration == configuration)
            {
                أرجع i;
            }
        }
        أرجع عدم;
    }

    ساكن Impl* لبھو_Or_Create_Context(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        إذا (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            إذا (!c.empty())
                configuration = c;
        }
        Impl* impl = لبھو_Context(configuration);
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
            مٹاؤ impl;
            أرجع عدم;
        }
        امسك (...)
        {
            مٹاؤ impl;
            ارم;
        }
    }

    ساكن Impl* لبھو_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (فراغ*)h);
        Impl* impl = لبھو_Context(configuration);
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
            مٹاؤ impl;
            ارم;
        }
    }

    ساكن Impl* لبھو_Or_Create_Context(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (فراغ*)d);
        Impl* impl = لبھو_Context(configuration);
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
            مٹاؤ impl;
            ارم;
        }
    }

    فراغ سیٹ_Default()
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
        CV_OCL_DBG_CHECK_نتیجہ(حالت, "clCreateContext");

        منطقي ok = handle != 0 && حالت == CL_SUCCESS;
        إذا( ok )
        {
            devices.resize(nd);
            devices[0].سیٹ(d);
        }
        وإلا
            handle = عدم;
    }

    پروگرام_2 getProg(ثابت پروگرام_سروت& src, ثابت String& buildflags, String& errmsg);

    فراغ unloadProg(پروگرام_2& prog)
    {
        cv::Auto_جندرا lock(پروگرام_cache_mutex);
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

    std::string& get_اگیتر_String()
    {
        إذا (اگیتر.empty())
        {
            cv::Auto_جندرا lock(پروگرام_cache_mutex);
            إذا (اگیتر.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحيح بٹ_2 = d.پتہ_بٹ();
                إذا (بٹ_2 > 0 && بٹ_2 != 64)
                    اگیتر = cv::format("%d-bit--", بٹ_2);
                اگیتر += d.vendorName() + "--" + d.name() + "--" + d.driver_نسخہ();
                // sanitize chars
                لكل (size_t i = 0; i < اگیتر.size(); i++)
                {
                    حرف c = اگیتر[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        اگیتر[i] = '_';
                    }
                }
            }
        }
        أرجع اگیتر;
    }

    std::string& get_اگیتر_ادھار()
    {
        إذا (اگیتر_ادھار.empty())
        {
            cv::Auto_جندرا lock(پروگرام_cache_mutex);
            إذا (اگیتر_ادھار.empty())
            {
                ثابت Device& d = devices[0];
                صحيح بٹ_2 = d.پتہ_بٹ();
                إذا (بٹ_2 > 0 && بٹ_2 != 64)
                    اگیتر_ادھار = cv::format("%d-bit--", بٹ_2);
                اگیتر_ادھار += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                لكل (size_t i = 0; i < اگیتر_ادھار.size(); i++)
                {
                    حرف c = اگیتر_ادھار[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        اگیتر_ادھار[i] = '_';
                    }
                }
            }
        }
        أرجع اگیتر_ادھار;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحيح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string اگیتر;
    std::string اگیتر_ادھار;

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

    std::map<std::type_اشاریہ, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    فراغ سیٹ_User_Context(std::type_اشاریہ typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::Auto_جندرا lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_اشاریہ typeId) {
        cv::Auto_جندرا lock(userContextMutex);
        خودکار it = userContextStorage.find(typeId);
        إذا (it != userContextStorage.end())
            أرجع it->second;
        وإلا
            أرجع خالی_سوچک;
    }

#جے_تعریف_شدہ HAVE_OPENCL_SVM
    منطقي svmInitialized;
    منطقي svm_دستیاب;
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
                إذا (CL_SUCCESS != cl_قطار_وچ_پاؤ_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_انت(q);
                حاول
                {
                    ((صحيح*)ptr)[0] = 100;
                }
                امسك (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = سچ;
                }
                إذا (CL_SUCCESS != cl_قطار_وچ_پاؤ_SVMUnmap(q, ptr, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_انت(q);
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
            svmFunctions.fn_cl_سیٹ_Kernel_Arg_SVMPointer = cl_سیٹ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMemcpy = cl_قطار_وچ_پاؤ_SVMMemcpy;
            svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMem_Fill = cl_قطار_وچ_پاؤ_SVMMem_Fill;
            svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMap = cl_قطار_وچ_پاؤ_SVMMap;
            svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMUnmap = cl_قطار_وچ_پاؤ_SVMUnmap;
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
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_سیٹ_Kernel_Arg_SVMPointer = (cl_سیٹ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMemcpy = (cl_قطار_وچ_پاؤ_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMem_Fill = (cl_قطار_وچ_پاؤ_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMMap = (cl_قطار_وچ_پاؤ_SVMMap_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_قطار_وچ_پاؤ_SVMUnmap = (cl_قطار_وچ_پاؤ_SVMUnmap_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                اذهب noSVM;
            }
        }

        svm_دستیاب = سچ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = سچ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        أرجع;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_دستیاب = خطأ;
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
#جے_انت

    یار صنف پروگرام_2;
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
    p = Impl::لبھو_Or_Create_Context(std::string());
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
    إذا (dtype == CL_DEVICE_TYPE_DEFAULT || (بنانشانی)dtype == (بنانشانی)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::لبھو_Or_Create_Context("");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::لبھو_Or_Create_Context(":GPU:");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::لبھو_Or_Create_Context(":CPU:");
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
    c.p = خالی_سوچک;
}

Context& Context::عامل = (Context&& c) CV_NOEXCEPT
{
    إذا (هذا != &c) {
        إذا(p)
            p->release();
        p = c.p;
        c.p = خالی_سوچک;
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
    خودکار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خودکار& ctx = c.getContext();
        أرجع ctx;
    }

    ساكن Context dummy;
    أرجع dummy;
}

پروگرام_2 Context::getProg(ثابت پروگرام_سروت& prog,
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
    ctx.p = Impl::لبھو_Or_Create_Context((cl_context)context);
    أرجع ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::لبھو_Or_Create_Context(device);
    أرجع ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::لبھو_Or_Create_Context(configuration);
    أرجع ctx;
}

فراغ* Context::getOpenCLContextProperty(صحيح propertyId) ثابت
{
    إذا (p == عدم)
        أرجع خالی_سوچک;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, عدم, &size));
    std::vector<cl_context_properties> prop(size / حجم(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.انکڑے(), عدم));
    لكل (size_t i = 0; i < prop.size(); i += 2)
    {
        إذا (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(عدم, "OpenCL: found context property=" << propertyId << ") => " << (فراغ*)prop[i + 1]);
            أرجع (فراغ*)prop[i + 1];
        }
    }
    أرجع خالی_سوچک;
}

#جے_تعریف_شدہ HAVE_OPENCL_SVM
منطقي Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmEnabled;
}
فراغ Context::سیٹ_Use_SVM(منطقي enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    إذا (enabled && !i->svm_دستیاب)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#وإلا
منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سیٹ_Use_SVM(منطقي enabled) { CV_Assert(!enabled); }
#جے_انت

#جے_تعریف_شدہ HAVE_OPENCL_SVM
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
    إذا ((usageFlags & USAGE_ALLOCATE_SHARED_حافظہ) != 0)
        أرجع سچ;
    أرجع خطأ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#جے_انت // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

فراغ Context::سیٹ_User_Context(std::type_اشاریہ typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->سیٹ_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاریہ typeId)
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
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.انکڑے(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.انکڑے();
}

/*
// Attaches OpenCL context to OpenCV
*/
فراغ attachContext(ثابت String& platformName, فراغ* platformID, فراغ* context, فراغ* deviceID)
{
    خودکار ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
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

    منطقي platform_دستیاب = خطأ;

    // check if external platformName contained in list of available platforms in OpenCV
    لكل (بنانشانی صحيح i = 0; i < cnt; i++)
    {
        String دستیاب_Platform_Name;
        get_platform_name(platforms[i], دستیاب_Platform_Name);
        // external platform is found in the list of available platforms
        إذا (platformName == دستیاب_Platform_Name)
        {
            platform_دستیاب = سچ;
            اكسر;
        }
    }

    إذا (!platform_دستیاب)
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

    خودکار clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
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
#جے_تعریف_شدہ _WIN32
        إذا (!cv::__termination)
#جے_انت
        {
            إذا(handle)
            {
                CV_OCL_DBG_CHECK(cl_انت(handle));
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

        cl_int نتیجہ = CL_SUCCESS;
        cl_command_قطار_properties props = CL_قطار_PROFILING_ENABLE;
        cl_command_قطار q = cl_Create_Command_قطار(ctx, device, props, &نتیجہ);
        CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

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
    q.p = خالی_سوچک;
}

قطار& قطار::عامل = (قطار&& q) CV_NOEXCEPT
{
    إذا (هذا != &q) {
        إذا(p)
            p->release();
        p = q.p;
        q.p = خالی_سوچک;
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

فراغ قطار::انت_2()
{
    إذا(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_انت(p->handle));
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
    خودکار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خودکار& q = c.get_قطار();
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
    أرجع KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.کل()*m.elem_ناپ());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

بنية Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت پروگرام_2& prog) :
        refcount(1), handle(عدم), is_وچ_ترقی(خطأ), is_Async_لگاتار(خطأ), nu(0)
    {
        cl_پروگرام ph = (cl_پروگرام)prog.ptr();
        cl_int retval = 0;
        name = kname;
        إذا (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_عارضی_Dst_UMats = خطأ;
        have_عارضی_Src_UMats = خطأ;
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
                    u[i]->flags |= UMat_انکڑے::ASYNC_CLEANUP;
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
        have_عارضی_Dst_UMats = خطأ;
        have_عارضی_Src_UMats = خطأ;
        CV_Assert(!exceptionOccurred);
    }

    فراغ جوڑو_UMat(ثابت UMat& m, منطقي dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        إذا(dst && m.u->عارضی_UMat())
            have_عارضی_Dst_UMats = سچ;
        إذا(m.u->original_UMat_انکڑے == عدم && m.u->عارضی_UMat())
            have_عارضی_Src_UMats = سچ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    فراغ رجسٹر_Image_دلیل(صحيح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        إذا (arg < (صحيح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_وچ_ترقی, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    فراغ finit(cl_event e)
    {
        CV_بن_ورتیا(e);
        is_وچ_ترقی = خطأ;
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

    منطقي لگاتار(صحيح dims, size_t _globalsize[], size_t _localsize[],
            منطقي متزامن, int64* ویلا_NS, ثابت قطار& q);

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
    UMat_انکڑے* u[MAX_ARRS];
    منطقي is_وچ_ترقی;
    منطقي is_Async_لگاتار;  // true if kernel was scheduled in async mode
    صحيح nu;
    std::vector<Image2D> shadow_images;
    منطقي have_عارضی_Dst_UMats;
    منطقي have_عارضی_Src_UMats;
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

Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_سروت& src,
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
    k.p = خالی_سوچک;
}

Kernel& Kernel::عامل = (Kernel&& k) CV_NOEXCEPT
{
    إذا (هذا != &k) {
        إذا(p)
            p->release();
        p = k.p;
        k.p = خالی_سوچک;
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
#جے_تعریف_شدہ CV_OPENCL_لگاتار_ASSERT // check kernel compilation fails
    CV_Assert(p);
#جے_انت
    أرجع p != 0;
}

منطقي Kernel::create(ثابت حرف* kname, ثابت پروگرام_سروت& src,
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
        أرجع cv::format("%d / %uu / 0x%04x", *(چھوٹا*)p, *(بنانشانی چھوٹا*)p, *(چھوٹا*)p);
    إذا (sz == 4)
        أرجع cv::format("%d / %uu / 0x%08x / %g", *(صحيح*)p, *(صحيح*)p, *(صحيح*)p, *(عائم*)p);
    إذا (sz == 8)
        أرجع cv::format("%lld / %lluu / 0x%16llx / %g", *(لما لما*)p, *(لما لما*)p, *(لما لما*)p, *(مزدوج*)p);
    أرجع cv::format("%p", p);
}

صحيح Kernel::سیٹ(صحيح i, ثابت فراغ* value, size_t sz)
{
    إذا (!p || !p->handle)
        أرجع -1;
    إذا (i < 0)
        أرجع i;
    إذا( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحيح)i, (صحيح)sz, dump_قدر(sz, value).c_str()).c_str());
    إذا (retval != CL_SUCCESS)
        أرجع -1;
    أرجع i+1;
}

صحيح Kernel::سیٹ(صحيح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحيح res = سیٹ(i, &h, حجم(h));
    إذا (res >= 0)
        p->رجسٹر_Image_دلیل(i, image2D);
    أرجع res;
}

صحيح Kernel::سیٹ(صحيح i, ثابت UMat& m)
{
    أرجع سیٹ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساكن منطقي isCudaBacked(ثابت UMat_انکڑے* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    أرجع u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحيح Kernel::سیٹ(صحيح i, ثابت KernelArg& arg)
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
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحيح)i).c_str());
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

#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_انکڑے_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_انکڑے_Ptr);
#إذا 1 // TODO
            حالت = svmFns->fn_cl_سیٹ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_انکڑے_Ptr);
#وإلا
            حالت = svmFns->fn_cl_سیٹ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_انکڑے_Ptr);
#جے_انت
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحيح)i, (فراغ*)svm_انکڑے_Ptr).c_str());
        }
        وإلا
#جے_انت
        {
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحيح)i, (فراغ*)h).c_str());
        }

        إذا (ptronly)
        {
            i++;
        }
        وإلا إذا( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u2d.قدم), &u2d.قدم);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u2d.قدم).c_str());
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u2d.offset).c_str());
            i += 3;

            إذا( !(arg.flags & KernelArg::NO_ناپ) )
            {
                صحيح خانے_گنتی = u2d.خانے_گنتی*arg.wscale/arg.iwscale;
                حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u2d.صفاں), &u2d.صفاں);
                CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u2d.صفاں).c_str());
                حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(خانے_گنتی), &خانے_گنتی);
                CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)خانے_گنتی).c_str());
                i += 2;
            }
        }
        وإلا
        {
            UMat3D u3d(*arg.m);
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.slicestep).c_str());
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.قدم), &u3d.قدم);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u3d.قدم).c_str());
            حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+3), (صحيح)u3d.offset).c_str());
            i += 4;
            إذا( !(arg.flags & KernelArg::NO_ناپ) )
            {
                صحيح خانے_گنتی = u3d.خانے_گنتی*arg.wscale/arg.iwscale;
                حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u3d.slices).c_str());
                حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.صفاں), &u3d.صفاں);
                CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.صفاں).c_str());
                حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.خانے_گنتی), &خانے_گنتی);
                CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)خانے_گنتی).c_str());
                i += 3;
            }
        }
        p->جوڑو_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        أرجع i;
    }
    حالت = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_نتیجہ(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحيح)i, (صحيح)arg.sz, (فراغ*)arg.obj).c_str());
    أرجع i+1;
}

منطقي Kernel::لگاتار(صحيح dims, size_t _globalsize[], size_t _localsize[],
                 منطقي متزامن, ثابت قطار& q)
{
    إذا (!p)
        أرجع خطأ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t کل = 1;
    CV_Assert(_globalsize != عدم);
    لكل (صحيح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحيح)(i>0)) : 1;
        CV_Assert( val > 0 );
        کل *= _globalsize[i];
        إذا (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_اتے(_globalsize[i], (بنانشانی صحيح)val) * val;
    }
    CV_Assert(کل > 0);

    أرجع p->لگاتار(dims, globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::لگاتار_2(صحيح dims, size_t _globalsize[], size_t _localsize[],
                  منطقي متزامن, ثابت قطار& q)
{
    CV_Assert(p);
    أرجع p->لگاتار(dims, _globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::Impl::لگاتار(صحيح dims, size_t globalsize[], size_t localsize[],
        منطقي متزامن, int64* ویلا_NS, ثابت قطار& q)
{
    CV_INSTRUMENT_REGION_OPENCL_لگاتار(name.c_str());

    إذا (!handle)
    {
        CV_LOG_ERROR(عدم, "OpenCL kernel has zero handle: " << name);
        أرجع خطأ;
    }

    إذا (is_Async_لگاتار)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_لگاتار = !متزامن;

    إذا (is_وچ_ترقی)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#إذا CV_OPENCL_SYNC_لگاتار_KERNELS
    متزامن = سچ;
#جے_انت

    cl_command_قطار qq = get_قطار(q);
    إذا (have_عارضی_Dst_UMats)
        متزامن = سچ;
    إذا (have_عارضی_Src_UMats)
        متزامن = سچ;
    إذا (ویلا_NS)
        متزامن = سچ;
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_وچ_پاؤ_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           عدم, globalsize, localsize, 0, 0,
                                           (متزامن && !ویلا_NS) ? 0 : &asyncEvent);
#إذا !CV_OPENCL_SHOW_لگاتار_KERNELS
    إذا (retval != CL_SUCCESS)
#جے_انت
    {
        cv::String پیغام = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحيح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        متزامن ? "true" : "false"
                        );
        إذا (retval != CL_SUCCESS)
        {
            پیغام = CV_OCL_API_ERROR_پیغام(retval, پیغام.c_str());
        }
#إذا CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_نتیجہ(retval, پیغام.c_str());
#وإلا
        لکھو("%s\n", پیغام.c_str());
        fflush(stdout);
#جے_انت
    }
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_انت(qq));
        إذا (ویلا_NS)
        {
            إذا (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong شروع_ویلا, stop_ویلا;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_شروع, حجم(شروع_ویلا), &شروع_ویلا, عدم));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_سرا, حجم(stop_ویلا), &stop_ویلا, عدم));
                *ویلا_NS = (int64)(stop_ویلا - شروع_ویلا);
            }
            وإلا
            {
                *ویلا_NS = -1;
            }
        }
        cleanupUMats();
    }
    وإلا
    {
        addref();
        is_وچ_ترقی = سچ;
        CV_OCL_CHECK(cl_سیٹ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, هذا));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

منطقي Kernel::لگاتار_Task(منطقي متزامن, ثابت قطار& q)
{
    إذا(!p || !p->handle || p->is_وچ_ترقی)
        أرجع خطأ;

    cl_command_قطار qq = get_قطار(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_وچ_پاؤ_Task(qq, p->handle, 0, 0, متزامن ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), متزامن ? "true" : "false").c_str());
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_انت(qq));
        p->cleanupUMats();
    }
    وإلا
    {
        p->addref();
        p->is_وچ_ترقی = سچ;
        CV_OCL_CHECK(cl_سیٹ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

int64 Kernel::لگاتار_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q_)
{
    CV_Assert(p && p->handle && !p->is_وچ_ترقی);
    قطار q = q_.ptr() ? q_ : قطار::getDefault();
    CV_Assert(q.ptr());
    q.انت_2(); // call clFinish() on base queue
    قطار profiling_قطار_2 = q.get_Profiling_قطار();
    int64 ویلا_Ns = -1;
    منطقي res = p->لگاتار(dims, globalsize, localsize, سچ, &ویلا_Ns, profiling_قطار_2);
    أرجع res ? ویلا_Ns : -1;
}

size_t Kernel::work_گروہ_ناپ() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_WORK_گروہ_ناپ, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_گروہ_ناپ_مضرب() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_گروہ_ناپ_مضرب, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

منطقي Kernel::compile_Work_گروہ_ناپ(size_t wsz[]) ثابت
{
    إذا(!p || !p->handle || !wsz)
        أرجع 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_گروہ_ناپ, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS;
}

size_t Kernel::local_Mem_ناپ() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_ناپ, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    أرجع حالت == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

بنية پروگرام_سروت::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    تعداد قسم_4 {
        پروگرام_سروت_کوڈ = 0,
        پروگرام_BINARIES,
        پروگرام_SPIR,
        پروگرام_SPIRV
    } قسم_5;

    Impl(ثابت String& src)
    {
        init(پروگرام_سروت_کوڈ, cv::String(), cv::String());
        init_From_سروت(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& کوڈ_Str, ثابت String& کوڈ_Hash)
    {
        init(پروگرام_سروت_کوڈ, module, name);
        init_From_سروت(کوڈ_Str, کوڈ_Hash);
    }

    /// reset fields
    فراغ init(تعداد قسم_4 قسم, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        قسم_5 = قسم;
        module_ = module;
        name_ = name;

        سروت_Addr = عدم;
        سروت_ناپ = 0;
        isHashUpdated = خطأ;
    }

    فراغ init_From_سروت(ثابت String& کوڈ_Str, ثابت String& کوڈ_Hash)
    {
        کوڈ_Str_2 = کوڈ_Str;
        سروت_Hash = کوڈ_Hash;
        إذا (سروت_Hash.empty())
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
            سروت_Hash = cv::String(hashStr);
            isHashUpdated = سچ;
            أرجع;
        }
        uint64 hash = 0;
        بدل (قسم_5)
        {
        حالة پروگرام_سروت_کوڈ:
            إذا (سروت_Addr)
            {
                CV_Assert(کوڈ_Str_2.empty());
                hash = crc64(سروت_Addr, سروت_ناپ); // static storage
            }
            وإلا
            {
                CV_Assert(!کوڈ_Str_2.empty());
                hash = crc64((uchar*)کوڈ_Str_2.c_str(), کوڈ_Str_2.size());
            }
            اكسر;
        حالة پروگرام_BINARIES:
        حالة پروگرام_SPIR:
        حالة پروگرام_SPIRV:
            hash = crc64(سروت_Addr, سروت_ناپ);
            اكسر;
        افتراضي:
            CV_Error(Error::Sts_اندرونی, "Internal error");
        }
        سروت_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = سچ;
    }

    Impl(تعداد قسم_4 قسم,
            ثابت String& module, ثابت String& name,
            ثابت بنانشانی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(قسم, module, name);

        سروت_Addr = binary;
        سروت_ناپ = size;

        buildOptions_ = buildOptions;
    }

    ساكن پروگرام_سروت from_سروت_With_Static_Lifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* سروت_کوڈ_Static_Str, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        پروگرام_سروت نتیجہ;
        نتیجہ.p = جديد Impl(پروگرام_سروت_کوڈ, module, name,
                (ثابت بنانشانی حرف*)سروت_کوڈ_Static_Str, لڑی_لمبائی(سروت_کوڈ_Static_Str), buildOptions);
        نتیجہ.p->updateHash(hashStaticStr);
        أرجع نتیجہ;
    }

    ساكن پروگرام_سروت from_ثنائی(ثابت String& module, ثابت String& name,
            ثابت بنانشانی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_سروت نتیجہ;
        نتیجہ.p = جديد Impl(پروگرام_BINARIES, module, name, binary, size, buildOptions);
        أرجع نتیجہ;
    }

    ساكن پروگرام_سروت fromSPIR(ثابت String& module, ثابت String& name,
            ثابت بنانشانی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_سروت نتیجہ;
        نتیجہ.p = جديد Impl(پروگرام_SPIR, module, name, binary, size, buildOptions);
        أرجع نتیجہ;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String کوڈ_Str_2; // PROGRAM_SOURCE_CODE only

    ثابت بنانشانی حرف* سروت_Addr;
    size_t سروت_ناپ;

    cv::String buildOptions_;

    String سروت_Hash;
    منطقي isHashUpdated;

    یار بنية پروگرام_2::Impl;
    یار بنية internal::پروگرام_اندراج;
    یار بنية Context::Impl;
};


پروگرام_سروت::پروگرام_سروت() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_سروت::پروگرام_سروت(ثابت String& module, ثابت String& name, ثابت String& کوڈ_Str, ثابت String& کوڈ_Hash)
{
    p = جديد Impl(module, name, کوڈ_Str, کوڈ_Hash);
}

پروگرام_سروت::پروگرام_سروت(ثابت حرف* prog)
{
    p = جديد Impl(prog);
}

پروگرام_سروت::پروگرام_سروت(ثابت String& prog)
{
    p = جديد Impl(prog);
}

پروگرام_سروت::~پروگرام_سروت()
{
    إذا(p)
        p->release();
}

پروگرام_سروت::پروگرام_سروت(ثابت پروگرام_سروت& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

پروگرام_سروت& پروگرام_سروت::عامل = (ثابت پروگرام_سروت& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

پروگرام_سروت::پروگرام_سروت(پروگرام_سروت&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = خالی_سوچک;
}

پروگرام_سروت& پروگرام_سروت::عامل = (پروگرام_سروت&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = خالی_سوچک;
    }
    أرجع *هذا;
}

ثابت String& پروگرام_سروت::سروت() ثابت
{
    CV_Assert(p);
    CV_Assert(p->قسم_5 == Impl::پروگرام_سروت_کوڈ);
    CV_Assert(p->سروت_Addr == عدم); // method returns reference - can't construct temporary object
    أرجع p->کوڈ_Str_2;
}

پروگرام_سروت::hash_t پروگرام_سروت::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

پروگرام_سروت پروگرام_سروت::from_ثنائی(ثابت String& module, ثابت String& name,
        ثابت بنانشانی حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_ثنائی(module, name, binary, size, buildOptions);
}

پروگرام_سروت پروگرام_سروت::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت بنانشانی حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_ثنائی(module, name, binary, size, buildOptions);
}


internal::پروگرام_اندراج::عامل پروگرام_سروت&() ثابت
{
    إذا (هذا->p_پروگرام_سروت == عدم)
    {
        cv::Auto_جندرا lock(cv::getInitializationMutex());
        إذا (هذا->p_پروگرام_سروت == عدم)
        {
            پروگرام_سروت ps = پروگرام_سروت::Impl::from_سروت_With_Static_Lifetime(هذا->module, هذا->name, هذا->پروگرام_کوڈ, هذا->پروگرام_Hash, cv::String());
            پروگرام_سروت* ptr = جديد پروگرام_سروت(ps);
            const_cast<پروگرام_اندراج*>(هذا)->p_پروگرام_سروت = ptr;
        }
    }
    أرجع *هذا->p_پروگرام_سروت;
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

    Impl(ثابت پروگرام_سروت& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(عدم),
         buildflags(_buildflags)
    {
        ثابت پروگرام_سروت::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        سروت_Module = src_->module_;
        سروت_Name = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        إذا (ctx.ptr() == عدم || device.ptr() == عدم)
            أرجع;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        إذا (src.getImpl()->قسم_5 == پروگرام_سروت::Impl::پروگرام_سروت_کوڈ)
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
        CV_LOG_INFO(عدم, "OpenCL program '" << سروت_Module << "/" << سروت_Name << "' options:" << buildflags);
#جے_انت
        compile(ctx, src_, errmsg);
#إذا CV_OPENCL_SHOW_BUILD_KERNELS
        إذا (handle)
        {
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            cl_int نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_ناں, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", سروت_Module.c_str(), سروت_Name.c_str()).c_str());
            إذا (نتیجہ == CL_SUCCESS && retsz < حجم(kernels_بفر))
            {
                kernels_بفر[retsz] = 0;
                CV_LOG_INFO(عدم, "OpenCL program '" << سروت_Module << "/" << سروت_Name << "' kernels: '" << kernels_بفر << "'");
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, "OpenCL program '" << سروت_Module << "/" << سروت_Name << "' can't retrieve kernel names!");
            }
        }
#جے_انت
    }

    منطقي compile(ثابت Context& ctx, ثابت پروگرام_سروت::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        إذا (src_->قسم_5 == پروگرام_سروت::Impl::پروگرام_BINARIES)
        {
            CV_LOG_VERBOSE(عدم, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_ثنائی(ctx, src_->سروت_Addr, src_->سروت_ناپ, errmsg);
            أرجع isLoaded;
        }
        أرجع compileWithCache(ctx, src_, errmsg);
    }

    منطقي compileWithCache(ثابت Context& ctx, ثابت پروگرام_سروت::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 != پروگرام_سروت::Impl::پروگرام_BINARIES);

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string ادھار_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_اگیتر_String(),
                ctx.getImpl()->get_اگیتر_ادھار()
        );
        ثابت String& hash_str = src_->سروت_Hash;
        cv::String fname;
        إذا (!ادھار_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(ادھار_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::File_جندرا> file_جندرا = config.cache_جندرا; // can be empty
        إذا (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            حاول
            {
                std::vector<حرف> ثنائی_Buf;
                منطقي res = خطأ;
                {
                    cv::utils::optional_shared_جندرا_guard<cv::utils::fs::File_جندرا> جندرا_fs(file_جندرا.get());
                    ثنائی_پروگرام_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, ثنائی_Buf);
                }
                إذا (res)
                {
                    CV_Assert(!ثنائی_Buf.empty());
                    CV_LOG_VERBOSE(عدم, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقي isLoaded = create_From_ثنائی(ctx, ثنائی_Buf, errmsg);
                    إذا (isLoaded)
                        أرجع سچ;
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_بن_ورتیا(e);
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname << std::سطر_انت << e.what());
            }
            امسك (...)
            {
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == عدم);
        إذا (src_->قسم_5 == پروگرام_سروت::Impl::پروگرام_سروت_کوڈ)
        {
            إذا (!buildFromSources(ctx, src_, errmsg))
            {
                أرجع خطأ;
            }
        }
        وإلا إذا (src_->قسم_5 == پروگرام_سروت::Impl::پروگرام_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            إذا ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(عدم, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_ثنائی(ctx, src_->سروت_Addr, src_->سروت_ناپ, errmsg);
            إذا (!isLoaded)
                أرجع خطأ;
        }
        وإلا إذا (src_->قسم_5 == پروگرام_سروت::Impl::پروگرام_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وإلا
        {
            CV_Error(Error::Sts_اندرونی, "Internal error");
        }
        CV_Assert(handle != عدم);
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        إذا (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            حاول
            {
                std::vector<حرف> ثنائی_Buf;
                get_پروگرام_ثنائی(ثنائی_Buf);
                {
                    cv::utils::optional_جندرا_guard<cv::utils::fs::File_جندرا> جندرا_fs(file_جندرا.get());
                    ثنائی_پروگرام_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, ثنائی_Buf);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname << std::سطر_انت << e.what());
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#جے_انت // OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
        إذا (CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
        {
            std::vector<حرف> ثنائی_Buf;
            get_پروگرام_ثنائی(ثنائی_Buf);
            إذا (!ثنائی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                handle = عدم;
                create_From_ثنائی(ctx, ثنائی_Buf, errmsg);
            }
        }
#جے_انت
        أرجع handle != عدم;
    }

    فراغ dumpBuildLog_(cl_int نتیجہ, ثابت cl_device_id* deviceList, String& errmsg)
    {
        Auto_بفر<حرف, 4096> بفر; بفر[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                                  CL_پروگرام_BUILD_LOG, 0, 0, &retsz);
        إذا (log_retval == CL_SUCCESS && retsz > 1)
        {
            بفر.resize(retsz + 16);
            log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                               CL_پروگرام_BUILD_LOG, retsz+1, بفر.انکڑے(), &retsz);
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

        errmsg = String(بفر.انکڑے());
        لکھو("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                سروت_Module.c_str(), سروت_Name.c_str(),
                نتیجہ, getOpenCLErrorString(نتیجہ),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    منطقي buildFromSources(ثابت Context& ctx, ثابت پروگرام_سروت::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 == پروگرام_سروت::Impl::پروگرام_سروت_کوڈ);
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                سروت_Module.c_str(), سروت_Name.c_str(),
                src_->سروت_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(عدم, 0, "Compile... " << سروت_Module.c_str() << "/" << سروت_Name.c_str());

        ثابت حرف* srcptr = src_->سروت_Addr ? ((ثابت حرف*)src_->سروت_Addr) : src_->کوڈ_Str_2.c_str();
        size_t srclen = src_->سروت_Addr ? src_->سروت_ناپ : src_->کوڈ_Str_2.size();
        CV_Assert(srcptr != عدم);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_پروگرام_With_سروت((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_نتیجہ(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        إذا (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_بفر<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.انکڑے();
            لكل (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_پروگرام(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_نتیجہ(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#إذا !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            إذا (retval != CL_SUCCESS)
#جے_انت
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
                    سروت_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#إذا CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
            إذا (handle && CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
            {
                CV_LOG_INFO(عدم, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_بفر[4096] = {0};
                cl_int نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_ناں, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
                إذا (retsz < حجم(kernels_بفر))
                    kernels_بفر[retsz] = 0;
                وإلا
                    kernels_بفر[0] = 0;
                CV_LOG_INFO(عدم, نتیجہ << ": Kernels='" << kernels_بفر << "'");
            }
#جے_انت
        }
        أرجع handle != عدم;
    }

    فراغ get_پروگرام_ثنائی(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_ثنائی_SIZES, حجم(sz), &sz, عدم));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_BINARIES, حجم(ptr), &ptr, عدم));
    }

    منطقي create_From_ثنائی(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        أرجع create_From_ثنائی(ctx, (ثابت بنانشانی حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقي create_From_ثنائی(ثابت Context& ctx, ثابت بنانشانی حرف* ثنائی_Addr, ثابت size_t ثنائی_ناپ, String& errmsg)
    {
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(عدم, 0, "Load from binary... (" << ثنائی_ناپ << " bytes)");

        CV_Assert(ثنائی_ناپ > 0);

        size_t ndevices = (صحيح)ctx.ndevices();
        Auto_بفر<cl_device_id> devices_(ndevices);
        Auto_بفر<ثابت uchar*> ثنائی_Ptrs(ndevices);
        Auto_بفر<size_t> ثنائی_Sizes(ndevices);

        cl_device_id* devices = devices_.انکڑے();
        ثابت uchar** ثنائی_Ptrs_2 = ثنائی_Ptrs.انکڑے();
        size_t* ثنائی_Sizes_2 = ثنائی_Sizes.انکڑے();
        لكل (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ثنائی_Ptrs_2[i] = ثنائی_Addr;
            ثنائی_Sizes_2[i] = ثنائی_ناپ;
        }

        cl_int نتیجہ = 0;
        handle = cl_Create_پروگرام_With_ثنائی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.انکڑے(),
                                           ثنائی_Sizes_2, ثنائی_Ptrs_2, عدم, &نتیجہ);
        إذا (نتیجہ != CL_SUCCESS)
        {
            CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیغام(نتیجہ, "clCreateProgramWithBinary"));
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
            نتیجہ = cl_Build_پروگرام(handle, (cl_uint)ndevices, devices_.انکڑے(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, cv::format("clBuildProgram(binary: %s/%s)", سروت_Module.c_str(), سروت_Name.c_str()).c_str());
            إذا (نتیجہ != CL_SUCCESS)
            {
                dumpBuildLog_(نتیجہ, devices, errmsg);
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
            CV_OCL_DBG_CHECK(نتیجہ = cl_Get_پروگرام_Build_Info(handle, devices[0], CL_پروگرام_BUILD_حالت,
                    حجم(build_حالت), &build_حالت, &retsz));
            إذا (نتیجہ == CL_SUCCESS)
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
                CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیغام(نتیجہ, "clGetProgramBuildInfo()"));
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = عدم;
                }
            }
        }
#إذا CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
        إذا (handle && CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
        {
            CV_LOG_INFO(عدم, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_ناں, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            إذا (retsz < حجم(kernels_بفر))
                kernels_بفر[retsz] = 0;
            وإلا
                kernels_بفر[0] = 0;
            CV_LOG_INFO(عدم, نتیجہ << ": Kernels='" << kernels_بفر << "'");
        }
#جے_انت
        أرجع handle != عدم;
    }

    ~Impl()
    {
        إذا( handle )
        {
#جے_تعریف_شدہ _WIN32
            إذا (!cv::__termination)
#جے_انت
            {
                cl_Release_پروگرام(handle);
            }
            handle = عدم;
        }
    }

    cl_پروگرام handle;

    String buildflags;
    String سروت_Module;
    String سروت_Name;
};


پروگرام_2::پروگرام_2() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_2::پروگرام_2(ثابت پروگرام_سروت& src,
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
    prog.p = خالی_سوچک;
}

پروگرام_2& پروگرام_2::عامل = (پروگرام_2&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = خالی_سوچک;
    }
    أرجع *هذا;
}

پروگرام_2::~پروگرام_2()
{
    إذا(p)
        p->release();
}

منطقي پروگرام_2::create(ثابت پروگرام_سروت& src,
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

#جے_بنا_تعریف OPENCV_REMOVE_DEPRECATED_API
ثابت پروگرام_سروت& پروگرام_2::سروت() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروگرام_2::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_بن_ورتیا(bin); CV_بن_ورتیا(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروگرام_2::اكتب(String& bin) ثابت
{
    CV_بن_ورتیا(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String پروگرام_2::get_اگیتر() ثابت
{
    إذا(!p)
        أرجع String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_اگیتر_String().c_str(), p->buildflags.c_str());
}

String پروگرام_2::get_اگیتر(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_اگیتر_String().c_str(), buildflags.c_str());
}
#جے_انت // OPENCV_REMOVE_DEPRECATED_API

فراغ پروگرام_2::get_ثنائی(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_پروگرام_ثنائی(binary);
}

پروگرام_2 Context::Impl::getProg(ثابت پروگرام_سروت& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t حد_بندی = get_پروگرام_گنتی_حد_بندی();
    ثابت پروگرام_سروت::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلید = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->سروت_Hash.c_str(),
            get_اگیتر_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_جندرا lock(پروگرام_cache_mutex);
        phash_t::iterator it = phash.find(کلید);
        إذا (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), کلید);
            إذا (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(کلید);
            }
            أرجع it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            إذا (حد_بندی > 0 && sz >= حد_بندی)
            {
                ساكن منطقي warningFlag = خطأ;
                إذا (!warningFlag)
                {
                    لکھو("\nWARNING: OpenCV-OpenCL:\n"
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
        cv::Auto_جندرا lock(پروگرام_cache_mutex);
        phash.insert(std::pair<std::string, پروگرام_2>(کلید, prog));
        cacheList.push_front(کلید);
    }
    أرجع prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

سانچا<قسم_ناں T>
صنف OpenCLBufferPool
{
محمي:
    ~OpenCLBufferPool() { }
عام:
    مجازی T allocate(size_t size) = 0;
    مجازی فراغ release(T بفر) = 0;
};

سانچا <قسم_ناں Derived, قسم_ناں بفر_اندراج, قسم_ناں T>
صنف Open_CLBuffer_Pool_ادھار_Impl : عام بفر_Pool_Controller, عام OpenCLBufferPool<T>
{
خاص:
    مضمن Derived& derived() { أرجع *static_cast<Derived*>(هذا); }
محمي:
    Mutex mutex_;

    size_t current_Reserved_ناپ;
    size_t max_Reserved_ناپ;

    std::list<بفر_اندراج> allocatedEntries_; // Allocated and used entries
    std::list<بفر_اندراج> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقي لبھو_And_Remove_اندراج_From_Allocated_List(CV_باہرلا بفر_اندراج& اندراج, T بفر)
    {
        قسم_ناں std::list<بفر_اندراج>::iterator i = allocatedEntries_.begin();
        لكل (; i != allocatedEntries_.end(); ++i)
        {
            بفر_اندراج& e = *i;
            إذا (e.cl_بفر_2 == بفر)
            {
                اندراج = e;
                allocatedEntries_.erase(i);
                أرجع سچ;
            }
        }
        أرجع خطأ;
    }

    // synchronized
    منطقي لبھو_And_Remove_اندراج_From_Reserved_List(CV_باہرلا بفر_اندراج& اندراج, ثابت size_t size)
    {
        إذا (reservedEntries_.empty())
            أرجع خطأ;
        قسم_ناں std::list<بفر_اندراج>::iterator i = reservedEntries_.begin();
        قسم_ناں std::list<بفر_اندراج>::iterator نتیجہ_pos = reservedEntries_.end();
        بفر_اندراج نتیجہ;
        size_t minDiff = (size_t)(-1);
        لكل (; i != reservedEntries_.end(); ++i)
        {
            بفر_اندراج& e = *i;
            إذا (e.سمرتھا >= size)
            {
                size_t diff = e.سمرتھا - size;
                إذا (diff < std::max((size_t)4096, size / 8) && (نتیجہ_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    نتیجہ_pos = i;
                    نتیجہ = e;
                    إذا (diff == 0)
                        اكسر;
                }
            }
        }
        إذا (نتیجہ_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(نتیجہ_pos);
            اندراج = نتیجہ;
            current_Reserved_ناپ -= اندراج.سمرتھا;
            allocatedEntries_.push_back(اندراج);
            أرجع سچ;
        }
        أرجع خطأ;
    }

    // synchronized
    فراغ check_ناپ_دا_Reserved_Entries()
    {
        طالما (current_Reserved_ناپ > max_Reserved_ناپ)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت بفر_اندراج& اندراج = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_ناپ >= اندراج.سمرتھا);
            current_Reserved_ناپ -= اندراج.سمرتھا;
            derived().release_بفر_اندراج(اندراج);
            reservedEntries_.pop_back();
        }
    }

    مضمن size_t ونڈ_Granularity(size_t size)
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
    Open_CLBuffer_Pool_ادھار_Impl()
        : current_Reserved_ناپ(0),
          max_Reserved_ناپ(0)
    {
        // nothing
    }
    مجازی ~Open_CLBuffer_Pool_ادھار_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عام:
    مجازی T allocate(size_t size) CV_OVERRIDE
    {
        Auto_جندرا locker(mutex_);
        بفر_اندراج اندراج;
        إذا (max_Reserved_ناپ > 0 && لبھو_And_Remove_اندراج_From_Reserved_List(اندراج, size))
        {
            CV_DbgAssert(size <= اندراج.سمرتھا);
            LOG_بفر_POOL("Reuse reserved buffer: %p\n", اندراج.cl_بفر_2);
        }
        وإلا
        {
            derived().allocate_بفر_اندراج(اندراج, size);
        }
        أرجع اندراج.cl_بفر_2;
    }
    مجازی فراغ release(T بفر) CV_OVERRIDE
    {
        Auto_جندرا locker(mutex_);
        بفر_اندراج اندراج;
        CV_Assert(لبھو_And_Remove_اندراج_From_Allocated_List(اندراج, بفر));
        إذا (max_Reserved_ناپ == 0 || اندراج.سمرتھا > max_Reserved_ناپ / 8)
        {
            derived().release_بفر_اندراج(اندراج);
        }
        وإلا
        {
            reservedEntries_.push_front(اندراج);
            current_Reserved_ناپ += اندراج.سمرتھا;
            check_ناپ_دا_Reserved_Entries();
        }
    }

    مجازی size_t get_Reserved_ناپ() ثابت CV_OVERRIDE { أرجع current_Reserved_ناپ; }
    مجازی size_t get_Max_Reserved_ناپ() ثابت CV_OVERRIDE { أرجع max_Reserved_ناپ; }
    مجازی فراغ سیٹ_Max_Reserved_ناپ(size_t size) CV_OVERRIDE
    {
        Auto_جندرا locker(mutex_);
        size_t old_Max_Reserved_ناپ = max_Reserved_ناپ;
        max_Reserved_ناپ = size;
        إذا (max_Reserved_ناپ < old_Max_Reserved_ناپ)
        {
            قسم_ناں std::list<بفر_اندراج>::iterator i = reservedEntries_.begin();
            لكل (; i != reservedEntries_.end();)
            {
                ثابت بفر_اندراج& اندراج = *i;
                إذا (اندراج.سمرتھا > max_Reserved_ناپ / 8)
                {
                    CV_DbgAssert(current_Reserved_ناپ >= اندراج.سمرتھا);
                    current_Reserved_ناپ -= اندراج.سمرتھا;
                    derived().release_بفر_اندراج(اندراج);
                    i = reservedEntries_.erase(i);
                    استمر;
                }
                ++i;
            }
            check_ناپ_دا_Reserved_Entries();
        }
    }
    مجازی فراغ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_جندرا locker(mutex_);
        قسم_ناں std::list<بفر_اندراج>::const_iterator i = reservedEntries_.begin();
        لكل (; i != reservedEntries_.end(); ++i)
        {
            ثابت بفر_اندراج& اندراج = *i;
            derived().release_بفر_اندراج(اندراج);
        }
        reservedEntries_.clear();
        current_Reserved_ناپ = 0;
    }
};

بنية CLBuffer_اندراج
{
    cl_mem cl_بفر_2;
    size_t سمرتھا;
    CLBuffer_اندراج() : cl_بفر_2((cl_mem)عدم), سمرتھا(0) { }
};

صنف OpenCLBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_ادھار_Impl<OpenCLBufferPoolImpl, CLBuffer_اندراج, cl_mem>
{
عام:
    تعريفنوع بنية CLBuffer_اندراج بفر_اندراج;
محمي:
    صحيح createFlags_;
عام:
    OpenCLBufferPoolImpl(صحيح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    فراغ allocate_بفر_اندراج(بفر_اندراج& اندراج, size_t size)
    {
        CV_DbgAssert(اندراج.cl_بفر_2 == عدم);
        اندراج.سمرتھا = align_ناپ(size, (صحيح)ونڈ_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        اندراج.cl_بفر_2 = cl_Create_بفر((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, اندراج.سمرتھا, 0, &retval);
        CV_OCL_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (لما لما صحيح)اندراج.سمرتھا, (فراغ*)اندراج.cl_بفر_2).c_str());
        CV_Assert(اندراج.cl_بفر_2 != عدم);
        إذا(retval == CL_SUCCESS)
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (لما لما)اندراج.سمرتھا, (لما لما)اندراج.سمرتھا, اندراج.cl_بفر_2);
        allocatedEntries_.push_back(اندراج);
    }

    فراغ release_بفر_اندراج(ثابت بفر_اندراج& اندراج)
    {
        CV_Assert(اندراج.سمرتھا != 0);
        CV_Assert(اندراج.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                اندراج.cl_بفر_2, (لما لما)اندراج.سمرتھا, (لما لما)اندراج.سمرتھا);
        CV_OCL_DBG_CHECK(clReleaseMemObject(اندراج.cl_بفر_2));
    }
};

#جے_تعریف_شدہ HAVE_OPENCL_SVM
بنية CLSVMBuffer_اندراج
{
    فراغ* cl_بفر_2;
    size_t سمرتھا;
    CLSVMBuffer_اندراج() : cl_بفر_2(عدم), سمرتھا(0) { }
};
صنف OpenCLSVMBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_ادھار_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_اندراج, فراغ*>
{
عام:
    تعريفنوع بنية CLSVMBuffer_اندراج بفر_اندراج;
عام:
    OpenCLSVMBufferPoolImpl()
    {
    }

    فراغ allocate_بفر_اندراج(بفر_اندراج& اندراج, size_t size)
    {
        CV_DbgAssert(اندراج.cl_بفر_2 == عدم);
        اندراج.سمرتھا = align_ناپ(size, (صحيح)ونڈ_Granularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحيح)اندراج.سمرتھا);
        فراغ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, اندراج.سمرتھا, 0);
        CV_Assert(buf);

        اندراج.cl_بفر_2 = buf;
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (لما لما)اندراج.سمرتھا, (لما لما)اندراج.سمرتھا, اندراج.cl_بفر_2);
        allocatedEntries_.push_back(اندراج);
    }

    فراغ release_بفر_اندراج(ثابت بفر_اندراج& اندراج)
    {
        CV_Assert(اندراج.سمرتھا != 0);
        CV_Assert(اندراج.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                اندراج.cl_بفر_2, (لما لما)اندراج.سمرتھا, (لما لما)اندراج.سمرتھا);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  اندراج.cl_بفر_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), اندراج.cl_بفر_2);
    }
};
#جے_انت



سانچا <منطقي readAccess, منطقي writeAccess>
صنف Aligned_انکڑے_Ptr
{
محمي:
    ثابت size_t ناپ_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عام:
    Aligned_انکڑے_Ptr(uchar* ptr, size_t size, size_t alignment)
        : ناپ_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        إذا (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[ناپ_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                حافظہ_نقل(ptr_, originPtr_, ناپ_3);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_انکڑے_Ptr()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                حافظہ_نقل(originPtr_, ptr_, ناپ_3);
            }
            مٹاؤ[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_انکڑے_Ptr(ثابت Aligned_انکڑے_Ptr&); // disabled
    Aligned_انکڑے_Ptr& عامل=(ثابت Aligned_انکڑے_Ptr&); // disabled
};

سانچا <منطقي readAccess, منطقي writeAccess>
صنف Aligned_انکڑے_Ptr2_D
{
محمي:
    ثابت size_t ناپ_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t صفاں_2;
    size_t خانے_گنتی_2;
    size_t قدم_2;

عام:
    Aligned_انکڑے_Ptr2_D(uchar* ptr, size_t صفاں, size_t خانے_گنتی, size_t قدم, size_t alignment, size_t extrabytes=0)
        : ناپ_3(صفاں*قدم), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم), صفاں_2(صفاں), خانے_گنتی_2(خانے_گنتی), قدم_2(قدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != عدم);
        إذا (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[ناپ_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                لكل (size_t i = 0; i < صفاں_2; i++)
                    حافظہ_نقل(ptr_ + i*قدم_2, originPtr_ + i*قدم_2, خانے_گنتی_2);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_انکڑے_Ptr2_D()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                لكل (size_t i = 0; i < صفاں_2; i++)
                    حافظہ_نقل(originPtr_ + i*قدم_2, ptr_ + i*قدم_2, خانے_گنتی_2);
            }
            مٹاؤ[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_انکڑے_Ptr2_D(ثابت Aligned_انکڑے_Ptr2_D&); // disabled
    Aligned_انکڑے_Ptr2_D& عامل=(ثابت Aligned_انکڑے_Ptr2_D&); // disabled
};

#جے_بنا_تعریف CV_OPENCL_انکڑے_PTR_ALIGNMENT
#تعریف_کرو CV_OPENCL_انکڑے_PTR_ALIGNMENT 16
#جے_انت


فراغ Context::Impl::__init_buffer_pools()
{
    بفر_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& بفر_Pool_3 = *بفر_Pool_2.get();
    بفر_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& بفر_Pool_Host_Ptr_2 = *بفر_Pool_Host_Ptr.get();

    size_t default_Pool_ناپ = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_ناپ = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_ناپ);
    بفر_Pool_3.سیٹ_Max_Reserved_ناپ(pool_ناپ);
    size_t pool_ناپ_Host_Ptr = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_ناپ);
    بفر_Pool_Host_Ptr_2.سیٹ_Max_Reserved_ناپ(pool_ناپ_Host_Ptr);

#جے_تعریف_شدہ HAVE_OPENCL_SVM
    بفر_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& بفر_Pool_SVM_2 = *بفر_Pool_SVM.get();
    size_t pool_ناپ_SVM = utils::get_Configuration_Parameter_ناپ_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_ناپ);
    بفر_Pool_SVM_2.سیٹ_Max_Reserved_ناپ(pool_ناپ_SVM);
#جے_انت

    CV_LOG_INFO(عدم, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_ناپ << " poolSizeHostPtr=" << pool_ناپ_Host_Ptr);
}

صنف OpenCLAllocator CV_FINAL : عام MatAllocator
{
عام:
    تعداد AllocatorFlags
    {
        ALLOCATOR_FLAGS_بفر_POOL_ورتیا = 1 << 0,
        ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_ورتیا = 1 << 1,
#جے_تعریف_شدہ HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_بفر_POOL_SVM_ورتیا = 1 << 2,
#جے_انت
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

    UMat_انکڑے* defaultAllocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* انکڑے, size_t* قدم,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMat_انکڑے* u = matStdAllocator->allocate(dims, sizes, type, انکڑے, قدم, flags, usageFlags);
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

    فراغ get_بہترین_Flags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحيح& createFlags, UMat_انکڑے::حافظہ_Flag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        إذا ((usageFlags & USAGE_ALLOCATE_HOST_حافظہ) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        إذا (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_حافظہ_2()
#جے_بنا_تعریف __APPLE__
                || dev.isIntel()
#جے_انت
                )
            )
        )
            flags0 = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
        وإلا
            flags0 = UMat_انکڑے::COPY_ON_MAP;
    }

    UMat_انکڑے* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* انکڑے, size_t* قدم, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!useOpenCL())
            أرجع defaultAllocate(dims, sizes, type, انکڑے, قدم, flags, usageFlags);

        flush_Cleanup_قطار();

        CV_Assert(انکڑے == 0);
        size_t کل = CV_ELEM_ناپ(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( قدم )
                قدم[i] = کل;
            کل *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        إذا (!ctx.getImpl())
            أرجع defaultAllocate(dims, sizes, type, انکڑے, قدم, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحيح createFlags = 0;
        UMat_انکڑے::حافظہ_Flag flags0 = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
        get_بہترین_Flags(ctx, flags, usageFlags, createFlags, flags0);

        فراغ* handle = عدم;
        صحيح allocatorFlags = 0;

#جے_تعریف_شدہ HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        إذا (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_SVM_ورتیا;
            handle = ctxImpl.get_بفر_Pool_SVM().allocate(کل);

            // this property is constant, so single buffer pool can be used here
            منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
            allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
        }
        وإلا
#جے_انت
        إذا (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_ورتیا;
            handle = ctxImpl.get_بفر_Pool().allocate(کل);
        }
        وإلا إذا (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_ورتیا;
            handle = ctxImpl.get_بفر_Pool_Host_Ptr().allocate(کل);
        }
        وإلا
        {
            CV_Assert(handle != عدم); // Unsupported, throw
        }

        إذا (!handle)
            أرجع defaultAllocate(dims, sizes, type, انکڑے, قدم, flags, usageFlags);

        UMat_انکڑے* u = جديد UMat_انکڑے(هذا);
        u->انکڑے = 0;
        u->size = کل;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<فراغ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->عارضی_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(سچ);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع u;
    }

    منطقي allocate(UMat_انکڑے* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع خطأ;

        flush_Cleanup_قطار();

        UMat_انکڑے_Auto_جندرا lock(u);

        إذا(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحيح createFlags = 0;
            UMat_انکڑے::حافظہ_Flag flags0 = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
            get_بہترین_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقي copyOnMap = (flags0 & UMat_انکڑے::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحيح allocatorFlags = 0;
            UMat_انکڑے::حافظہ_Flag عارضی_UMat_Flags = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
            فراغ* handle = عدم;
            cl_int retval = CL_SUCCESS;

#جے_تعریف_شدہ HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقي useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            إذا (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                عارضی_UMat_Flags = UMat_انکڑے::عارضی_UMAT;
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
                        cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMap()");

                    }
                    حافظہ_نقل(handle, u->origdata, u->size);
                    إذا (!is_Fine_Grain_بفر)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                    }

                    عارضی_UMat_Flags = UMat_انکڑے::عارضی_UMAT | UMat_انکڑے::عارضی_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر
                                                : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
                }
            }
            وإلا
#جے_انت
            {
                إذا( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                عارضی_UMat_Flags = UMat_انکڑے::عارضی_UMAT;
                إذا (
                #جے_تعریف_شدہ __APPLE__
                    !copyOnMap &&
                #جے_انت
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحيح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_انکڑے && u->original_UMat_انکڑے->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (لما لما صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                }
                إذا((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (لما لما صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                    عارضی_UMat_Flags |= UMat_انکڑے::عارضی_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer() => %p", (فراغ*)handle).c_str());
            إذا(!handle || retval != CL_SUCCESS)
                أرجع خطأ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = هذا;
            u->flags |= عارضی_UMat_Flags | flags0;
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

    فراغ deallocate(UMat_انکڑے* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        إذا (!!(u->flags & UMat_انکڑے::ASYNC_CLEANUP))
            جوڑو_منزل_Cleanup_قطار(u);
        وإلا
            deallocate_(u);
    }

    فراغ deallocate_(UMat_انکڑے* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        إذا ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_بفر) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#جے_تعریف_شدہ _WIN32
        إذا (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            أرجع;             // avoid any OpenCL calls
#جے_انت
        إذا(u->عارضی_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            إذا (u->hostCopyObsolete())
            {
#جے_تعریف_شدہ HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    إذا( u->عارضی_Copied_UMat() )
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
                            cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMap()");
                        }
                        cl_انت(q);
                        حافظہ_نقل(u->origdata, u->handle, u->size);
                        إذا (!is_Fine_Grain_بفر)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, u->handle, 0, عدم, عدم);
                            CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                        }
                    }
                    وإلا
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وإلا
#جے_انت
                {
                    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
                    إذا( u->عارضی_Copied_UMat() )
                    {
                        Aligned_انکڑے_Ptr<خطأ, سچ> alignedPtr(u->origdata, u->size, CV_OPENCL_انکڑے_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وإلا
                    {
                        cl_int retval = 0;
                        إذا (u->عارضی_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_قطار(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            فراغ* انکڑے = cl_قطار_وچ_پاؤ_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (لما لما صحيح)u->size, انکڑے).c_str());
                            CV_Assert(u->origdata == انکڑے && "Details: https://github.com/opencv/opencv/issues/6293");
                            إذا (u->original_UMat_انکڑے)
                            {
                                CV_Assert(u->original_UMat_انکڑے->انکڑے == انکڑے);
                            }
                            retval = cl_قطار_وچ_پاؤ_Unmap_Mem_Object(q, (cl_mem)u->handle, انکڑے, 0, 0, 0);
                            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, انکڑے, (لما لما صحيح)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_انت(q));
                        }
                    }
                }
                u->markHostCopyObsolete(خطأ);
            }
            وإلا
            {
                // nothing
            }
#جے_تعریف_شدہ HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                إذا( u->عارضی_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وإلا
#جے_انت
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clReleaseMemObject(ptr=%p)", (فراغ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = عدم;
            إذا(u->انکڑے && u->copyOnMap() && u->انکڑے != u->origdata)
                fastFree(u->انکڑے);
            u->انکڑے = u->origdata;
            u->currAllocator->deallocate(u);
            u = عدم;
        }
        وإلا
        {
            CV_Assert(u->origdata == عدم);
            إذا(u->انکڑے && u->copyOnMap() && u->انکڑے != u->origdata)
            {
                fastFree(u->انکڑے);
                u->انکڑے = 0;
                u->markHostCopyObsolete(سچ);
            }
            إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_ورتیا)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool().release((cl_mem)u->handle);
            }
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_ورتیا)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#جے_تعریف_شدہ HAVE_OPENCL_SVM
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_SVM_ورتیا)
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
                        cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, u->handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_SVM().release((فراغ*)u->handle);
            }
#جے_انت
            وإلا
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            مٹاؤ u;
            u = عدم;
        }
        CV_Assert(u == عدم);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    فراغ map(UMat_انکڑے* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
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
#جے_تعریف_شدہ HAVE_OPENCL_SVM
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
                            cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_بفر_MAP;
                        }
                    }
                    cl_انت(q);
                    u->انکڑے = (uchar*)u->handle;
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(سچ);
                    أرجع;
                }
#جے_انت

                cl_int retval = CL_SUCCESS;
                إذا (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->انکڑے = (uchar*)cl_قطار_وچ_پاؤ_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (لما لما صحيح)u->size, u->انکڑے).c_str());
                }
                إذا (u->انکڑے && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(سچ);
                    أرجع;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_انکڑے::COPY_ON_MAP;
            }

            إذا(!u->انکڑے)
            {
                u->انکڑے = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(سچ);
            }
        }

        إذا (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_انکڑے_Ptr<خطأ, سچ> alignedPtr(u->انکڑے, u->size, CV_OPENCL_انکڑے_PTR_ALIGNMENT);
#جے_تعریف_شدہ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#جے_انت
            cl_int retval = cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (لما لما صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(خطأ);
        }
    }

    فراغ unmap(UMat_انکڑے* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;


        CV_Assert(u->handle != 0);

        UMat_انکڑے_Auto_جندرا autolock(u);

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
        cl_int retval = 0;
        إذا( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->انکڑے != عدم);
#جے_تعریف_شدہ HAVE_OPENCL_SVM
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
                        cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, u->handle,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                        cl_انت(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_بفر_MAP;
                    }
                }
                إذا (u->refcount == 0)
                    u->انکڑے = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(سچ);
                أرجع;
            }
#جے_انت
            إذا (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_قطار_وچ_پاؤ_Unmap_Mem_Object(q, (cl_mem)u->handle, u->انکڑے, 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, u->انکڑے, (لما لما صحيح)u->size).c_str());
                إذا (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_انت(q));
                }
                u->markDeviceMemMapped(خطأ);
                u->انکڑے = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(سچ);
            }
        }
        وإلا إذا( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_انکڑے_Ptr<سچ, خطأ> alignedPtr(u->انکڑے, u->size, CV_OPENCL_انکڑے_PTR_ALIGNMENT);
#جے_تعریف_شدہ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#جے_انت
            retval = cl_قطار_وچ_پاؤ_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (لما لما صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(خطأ);
            u->markHostCopyObsolete(سچ);
        }
    }

    منطقي checkContinuous(صحيح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& کل, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقي iscontinuous = سچ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        کل = sz[dims-1];
        لكل( صحيح i = dims-2; i >= 0; i-- )
        {
            إذا( i >= 0 && (کل != srcstep[i] || کل != dststep[i]) )
                iscontinuous = خطأ;
            کل *= sz[i];
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

    فراغ download(UMat_انکڑے* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;
        UMat_انکڑے_Auto_جندرا autolock(u);

        إذا( u->انکڑے && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            أرجع;
        }
        CV_Assert( u->handle != 0 );

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->انکڑے == عدم || u->انکڑے == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMap()");
            }
            cl_انت(q);
            إذا( iscontinuous )
            {
                حافظہ_نقل(dstptr, (uchar*)u->handle + srcrawofs, کل);
            }
            وإلا
            {
                // This code is from MatAllocator::download()
                صحيح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
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
                    حافظہ_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                cl_انت(q);
            }
        }
        وإلا
#جے_انت
        {
            إذا( iscontinuous )
            {
                Aligned_انکڑے_Ptr<خطأ, سچ> alignedPtr((uchar*)dstptr, کل, CV_OPENCL_انکڑے_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, کل, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_انکڑے_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_انکڑے_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_انکڑے_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                کل = align_ناپ(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                کل = std::min(کل, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, کل, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وإلا
            {
                Aligned_انکڑے_Ptr2_D<خطأ, سچ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_انکڑے_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    فراغ upload(UMat_انکڑے* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->عارضی_UMat());

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_انکڑے_Auto_جندرا autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        إذا( u->انکڑے && (u->hostCopyObsolete() < u->deviceCopyObsolete() || کل == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(خطأ);
            u->markDeviceCopyObsolete(سچ);
            أرجع;
        }

        CV_Assert( u->handle != 0 );
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->انکڑے == عدم || u->انکڑے == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMap()");
            }
            cl_انت(q);
            إذا( iscontinuous )
            {
                حافظہ_نقل((uchar*)u->handle + dstrawofs, srcptr, کل);
            }
            وإلا
            {
                // This code is from MatAllocator::upload()
                صحيح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
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
                    حافظہ_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMUnmap()");
                cl_انت(q);
            }
        }
        وإلا
#جے_انت
        {
            إذا( iscontinuous )
            {
                Aligned_انکڑے_Ptr<سچ, خطأ> alignedPtr((uchar*)srcptr, کل, CV_OPENCL_انکڑے_PTR_ALIGNMENT);
                cl_int retval = cl_قطار_وچ_پاؤ_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, کل, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (فراغ*)u->handle, (لما لما صحيح)dstrawofs, (لما لما صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_انکڑے_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_انکڑے_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_انکڑے_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                کل = align_ناپ(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                کل = std::min(کل, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, کل, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, کل, ptr, 0, 0, 0));
            }
            وإلا
            {
                Aligned_انکڑے_Ptr2_D<سچ, خطأ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_انکڑے_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Write_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(سچ);
#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#جے_انت
        {
            u->markHostCopyObsolete(سچ);
        }
        u->markDeviceCopyObsolete(خطأ);
    }

    فراغ copy(UMat_انکڑے* src, UMat_انکڑے* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي _sync) ثابت CV_OVERRIDE
    {
        إذا(!src || !dst)
            أرجع;

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_انکڑے_Auto_جندرا src_autolock(src, dst);

        إذا( !src->handle || (src->انکڑے && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->انکڑے + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            أرجع;
        }
        إذا( !dst->handle || (dst->انکڑے && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->انکڑے + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(خطأ);
#جے_تعریف_شدہ HAVE_OPENCL_SVM
            إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وإلا
#جے_انت
            {
                dst->markDeviceCopyObsolete(سچ);
            }
            أرجع;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#جے_تعریف_شدہ HAVE_OPENCL_SVM
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
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحيح)کل);
                    cl_int حالت = svmFns->fn_cl_قطار_وچ_پاؤ_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            کل, 0, عدم, عدم);
                    CV_OCL_CHECK_نتیجہ(حالت, "clEnqueueSVMMemcpy()");
                }
                وإلا
                {
                    cl_انت(q);
                    // This code is from MatAllocator::download()/upload()
                    صحيح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)ودھ_توں_ودھ_عدد );
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
                        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
                }
            }
            وإلا
            {
                إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->انکڑے + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                وإلا
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->انکڑے + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        وإلا
#جے_انت
        {
            إذا( iscontinuous )
            {
                retval = cl_قطار_وچ_پاؤ_Copy_بفر(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, کل, 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (فراغ*)src->handle, (فراغ*)dst->handle, (لما لما صحيح)srcrawofs, (لما لما صحيح)dstrawofs, (لما لما صحيح)کل).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_انکڑے_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_انکڑے_Ptr2_D<خطأ, خطأ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_انکڑے_PTR_ALIGNMENT, padding*2);
                Aligned_انکڑے_Ptr2_D<خطأ, خطأ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_انکڑے_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_کل = align_ناپ(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_کل = std::min(src_کل, src->size - new_srcrawofs);
                size_t dst_کل = align_ناپ(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_کل = std::min(dst_کل, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_کل, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Read_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_کل, dstptr, 0, 0, 0));

                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Write_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_کل, dstptr, 0, 0, 0));
            }
            وإلا
            {
                CV_OCL_CHECK(retval = cl_قطار_وچ_پاؤ_Copy_بفر_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        إذا (retval == CL_SUCCESS)
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL)
        }

#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#جے_انت
        {
            dst->markHostCopyObsolete(سچ);
        }
        dst->markDeviceCopyObsolete(خطأ);

        إذا( _sync )
        {
            CV_OCL_DBG_CHECK(cl_انت(q));
        }
    }

    بفر_Pool_Controller* get_بفر_Pool_Controller(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        إذا (ctx.empty())
            أرجع عدم;
#جے_تعریف_شدہ HAVE_OPENCL_SVM
        إذا ((svm::checkForceSVMUmatUsage() && (id == عدم || strcmp(id, "OCL") == 0)) || (id != عدم && strcmp(id, "SVM") == 0))
        {
            أرجع &ctx.getImpl()->get_بفر_Pool_SVM();
        }
#جے_انت
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

    بدلنجوگ cv::Mutex cleanup_قطار_Mutex;
    بدلنجوگ std::deque<UMat_انکڑے*> cleanup_قطار;

    فراغ flush_Cleanup_قطار() ثابت
    {
        // TSAN false positive: see #27638
        إذا (!cleanup_قطار.empty())
        {
            std::deque<UMat_انکڑے*> q;
            {
                cv::Auto_جندرا lock(cleanup_قطار_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_قطار);
            }
            لكل (std::deque<UMat_انکڑے*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    فراغ جوڑو_منزل_Cleanup_قطار(UMat_انکڑے* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_جندرا lock(cleanup_قطار_Mutex);
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
فراغ سیٹ_ناپ( UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* قدماں_2,
              منطقي auto_قدماں = خطأ );
فراغ finalizeHdr(UMat& m);

} // namespace cv


نطاق cv { نطاق ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t قدم, صحيح صفاں, صحيح خانے_گنتی, صحيح type, UMat& dst)
{
    صحيح d = 2;
    صحيح sizes[] = { صفاں, خانے_گنتی };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_نقاب) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    سیٹ_ناپ(dst, d, sizes, 0, سچ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_بفر;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_بفر == mem_type);

    size_t کل = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_ناپ, حجم(size_t), &کل, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحيح)قدم >= خانے_گنتی * CV_ELEM_ناپ(type));
    CV_Assert(کل >= صفاں * قدم);

    // attach clBuffer to UMatData
    dst.u = جديد UMat_انکڑے(getOpenCLAllocator());
    dst.u->انکڑے            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_بفر;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_انکڑے::حافظہ_Flag>(0);
    dst.u->handle          = cl_mem_بفر;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = کل;

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

    صحيح ڈونگھائی = CV_8U;
    بدل (fmt.image_channel_انکڑے_type)
    {
    حالة CL_UNORM_INT8:
    حالة CL_UNSIGNED_INT8:
        ڈونگھائی = CV_8U;
        اكسر;

    حالة CL_SNORM_INT8:
    حالة CL_SIGNED_INT8:
        ڈونگھائی = CV_8S;
        اكسر;

    حالة CL_UNORM_INT16:
    حالة CL_UNSIGNED_INT16:
        ڈونگھائی = CV_16U;
        اكسر;

    حالة CL_SNORM_INT16:
    حالة CL_SIGNED_INT16:
        ڈونگھائی = CV_16S;
        اكسر;

    حالة CL_SIGNED_INT32:
        ڈونگھائی = CV_32S;
        اكسر;

    حالة CL_FLOAT:
        ڈونگھائی = CV_32F;
        اكسر;

    حالة CL_HALF_FLOAT:
        ڈونگھائی = CV_16F;
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    صحيح type = CV_8UC1;
    بدل (fmt.image_channel_ترتیب)
    {
    حالة CL_R:
    حالة CL_A:
    حالة CL_INTENSITY:
    حالة CL_LUMINANCE:
        type = CV_MAKE_TYPE(ڈونگھائی, 1);
        اكسر;

    حالة CL_RG:
    حالة CL_RA:
        type = CV_MAKE_TYPE(ڈونگھائی, 2);
        اكسر;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالة CL_RGBA:
    حالة CL_BGRA:
    حالة CL_ARGB:
        type = CV_MAKE_TYPE(ڈونگھائی, 4);
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        اكسر;
    }

    size_t قدم = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_صف_PITCH, حجم(size_t), &قدم, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_چوڑائی, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_اچائی, حجم(size_t), &h, 0));

    dst.create((صحيح)h, (صحيح)w, type);

    cl_mem cl_بفر = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم));

    CV_OCL_CHECK(cl_انت(q));

    أرجع;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساكن فراغ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint عدد_Devices = 0;
    cl_int حالت = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, عدم, &عدد_Devices);
    إذا (حالت != CL_DEVICE_NOT_لبھیا) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_نتیجہ(حالت,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &عدد_Devices).c_str());
    }

    إذا (عدد_Devices == 0)
    {
        devices.clear();
        أرجع;
    }

    devices.resize((size_t)عدد_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, عدد_Devices, &devices[0], &عدد_Devices));
}

بنية PlatformInfo::Impl
{
    Impl(فراغ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        نسخہ_3 = getStrProp(CL_PLATFORM_نسخہ);
        parseOpenCLVersion(نسخہ_3, نسخہ_Major, نسخہ_Minor);
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

    String نسخہ_3;
    صحيح نسخہ_Major;
    صحيح نسخہ_Minor;
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
    i.p = خالی_سوچک;
}

PlatformInfo& PlatformInfo::عامل = (PlatformInfo&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا(p)
            p->release();
        p = i.p;
        i.p = خالی_سوچک;
    }
    أرجع *هذا;
}

صحيح PlatformInfo::device_نمبر() ثابت
{
    أرجع p ? (صحيح)p->devices.size() : 0;
}

فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت
{
    CV_Assert(p && d < (صحيح)p->devices.size() );
    إذا(p)
        device.سیٹ(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::نسخہ() ثابت
{
    أرجع p ? p->نسخہ_3 : String();
}

صحيح PlatformInfo::نسخہ_Major_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخہ_Major;
}

صحيح PlatformInfo::نسخہ_Minor_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخہ_Minor;
}

ساكن فراغ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint عدد_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &عدد_Platforms));

    إذا (عدد_Platforms == 0)
    {
        platforms.clear();
        أرجع;
    }

    platforms.resize((size_t)عدد_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(عدد_Platforms, &platforms[0], &عدد_Platforms));
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
    ساكن ثابت حرف* tab[CV_ڈونگھائی_MAX*16]=
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
    صحيح cn = CV_MAT_CN(type), ڈونگھائی = CV_MAT_ڈونگھائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? خالی_سوچک : tab[ڈونگھائی*16 + cn-1];
    CV_Assert(نتیجہ);
    أرجع نتیجہ;
}

ثابت حرف* memop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_ڈونگھائی_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), ڈونگھائی = CV_MAT_ڈونگھائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? خالی_سوچک : tab[ڈونگھائی*16 + cn-1];
    CV_Assert(نتیجہ);
    أرجع نتیجہ;
}

ثابت حرف* vecop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_ڈونگھائی_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), ڈونگھائی = CV_MAT_ڈونگھائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? 0 : tab[ڈونگھائی*16 + cn-1];
    CV_Assert(نتیجہ);
    أرجع نتیجہ;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    أرجع convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf, size_t buf_ناپ_2)
{
    إذا( sdepth == ddepth )
        أرجع "noconvert";
    ثابت حرف *typestr = type_منزل_Str(CV_MAKETYPE(ddepth, cn));
    إذا( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_ناپ_2, "convert_%s", typestr);
    }
    وإلا إذا( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_ناپ_2, "convert_%s_sat_rte", typestr);
    وإلا
        snprintf(buf, buf_ناپ_2, "convert_%s_sat", typestr);

    أرجع buf;
}

ثابت حرف* getOpenCLErrorString(صحيح error_کوڈ)
{
#تعریف_کرو CV_OCL_کوڈ(id) حالة id: أرجع #id
#تعریف_کرو CV_OCL_کوڈ_2(id, name) حالة id: أرجع #name
    بدل (error_کوڈ)
    {
    CV_OCL_کوڈ(CL_SUCCESS);
    CV_OCL_کوڈ(CL_DEVICE_NOT_لبھیا);
    CV_OCL_کوڈ(CL_DEVICE_NOT_دستیاب);
    CV_OCL_کوڈ(CL_COMPILER_NOT_دستیاب);
    CV_OCL_کوڈ(CL_MEM_OBJECT_ونڈ_FAILURE);
    CV_OCL_کوڈ(CL_باہرلا_دا_RESOURCES);
    CV_OCL_کوڈ(CL_باہرلا_دا_HOST_حافظہ);
    CV_OCL_کوڈ(CL_PROFILING_INFO_NOT_دستیاب);
    CV_OCL_کوڈ(CL_MEM_COPY_OVERLAP);
    CV_OCL_کوڈ(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_کوڈ(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_کوڈ(CL_BUILD_پروگرام_FAILURE);
    CV_OCL_کوڈ(CL_MAP_FAILURE);
    CV_OCL_کوڈ(CL_MISALIGNED_SUB_بفر_وتھ);
    CV_OCL_کوڈ(CL_EXEC_حالت_ERROR_FOR_EVENTS_وچ_WAIT_LIST);
    CV_OCL_کوڈ(CL_COMPILE_پروگرام_FAILURE);
    CV_OCL_کوڈ(CL_LINKER_NOT_دستیاب);
    CV_OCL_کوڈ(CL_LINK_پروگرام_FAILURE);
    CV_OCL_کوڈ(CL_DEVICE_حصے_کرو_FAILED);
    CV_OCL_کوڈ(CL_KERNEL_ARG_INFO_NOT_دستیاب);
    CV_OCL_کوڈ(CL_INVALID_قدر);
    CV_OCL_کوڈ(CL_INVALID_DEVICE_TYPE);
    CV_OCL_کوڈ(CL_INVALID_PLATFORM);
    CV_OCL_کوڈ(CL_INVALID_DEVICE);
    CV_OCL_کوڈ(CL_INVALID_CONTEXT);
    CV_OCL_کوڈ(CL_INVALID_قطار_PROPERTIES);
    CV_OCL_کوڈ(CL_INVALID_COMMAND_قطار);
    CV_OCL_کوڈ(CL_INVALID_HOST_PTR);
    CV_OCL_کوڈ(CL_INVALID_MEM_OBJECT);
    CV_OCL_کوڈ(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_کوڈ(CL_INVALID_IMAGE_ناپ);
    CV_OCL_کوڈ(CL_INVALID_SAMPLER);
    CV_OCL_کوڈ(CL_INVALID_ثنائی);
    CV_OCL_کوڈ(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_کوڈ(CL_INVALID_پروگرام);
    CV_OCL_کوڈ(CL_INVALID_پروگرام_EXECUTABLE);
    CV_OCL_کوڈ(CL_INVALID_KERNEL_NAME);
    CV_OCL_کوڈ(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_کوڈ(CL_INVALID_KERNEL);
    CV_OCL_کوڈ(CL_INVALID_ARG_اشاریہ);
    CV_OCL_کوڈ(CL_INVALID_ARG_قدر);
    CV_OCL_کوڈ(CL_INVALID_ARG_ناپ);
    CV_OCL_کوڈ(CL_INVALID_KERNEL_دلیلاں);
    CV_OCL_کوڈ(CL_INVALID_WORK_DIMENSION);
    CV_OCL_کوڈ(CL_INVALID_WORK_گروہ_ناپ);
    CV_OCL_کوڈ(CL_INVALID_WORK_شے_ناپ);
    CV_OCL_کوڈ(CL_INVALID_GLOBAL_وتھ);
    CV_OCL_کوڈ(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_کوڈ(CL_INVALID_EVENT);
    CV_OCL_کوڈ(CL_INVALID_OPERATION);
    CV_OCL_کوڈ(CL_INVALID_GL_OBJECT);
    CV_OCL_کوڈ(CL_INVALID_بفر_ناپ);
    CV_OCL_کوڈ(CL_INVALID_MIP_LEVEL);
    CV_OCL_کوڈ(CL_INVALID_GLOBAL_WORK_ناپ);
    // OpenCL 1.1
    CV_OCL_کوڈ(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_کوڈ(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_کوڈ(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_کوڈ(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_کوڈ(CL_INVALID_DEVICE_حصے_کرو_گنتی);
    // OpenCL 2.0
    CV_OCL_کوڈ_2(-69, CL_INVALID_PIPE_ناپ);
    CV_OCL_کوڈ_2(-70, CL_INVALID_DEVICE_قطار);
    // Extensions
    CV_OCL_کوڈ_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_کوڈ_2(-1001, CL_PLATFORM_NOT_لبھیا_KHR);
    CV_OCL_کوڈ_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_کوڈ_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_کوڈ_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_کوڈ_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    افتراضي: أرجع "Unknown OpenCL error";
    }
#تعریف_رد CV_OCL_کوڈ
#تعریف_رد CV_OCL_کوڈ_2
}

سانچا <قسم_ناں T>
ساكن std::string ker_منزل_Str(ثابت Mat & k)
{
    صحيح چوڑائی = k.خانے_گنتی - 1, ڈونگھائی = k.ڈونگھائی();
    ثابت T * ثابت انکڑے = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    إذا (ڈونگھائی <= CV_8S)
    {
        لكل (صحيح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << (صحيح)انکڑے[i] << ")";
        stream << "DIG(" << (صحيح)انکڑے[چوڑائی] << ")";
    }
    وإلا إذا (ڈونگھائی == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << انکڑے[i] << "f)";
        stream << "DIG(" << انکڑے[چوڑائی] << "f)";
    }
    وإلا إذا (ڈونگھائی == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << (عائم)انکڑے[i] << "h)";
        stream << "DIG(" << (عائم)انکڑے[چوڑائی] << "h)";
    }
    وإلا
    {
        لكل (صحيح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << انکڑے[i] << ")";
        stream << "DIG(" << انکڑے[چوڑائی] << ")";
    }

    أرجع stream.str();
}

String kernel_منزل_Str(داخلہ_Array _kernel, صحيح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحيح ڈونگھائی = kernel.ڈونگھائی();
    إذا (ddepth < 0)
        ddepth = ڈونگھائی;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    إذا (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    إذا (ddepth != ڈونگھائی)
        kernel.convert_منزل(kernel, ddepth);

    تعريفنوع std::string (* func_t)(ثابت Mat &);
    ساكن ثابت func_t funcs[CV_ڈونگھائی_MAX] = { ker_منزل_Str<uchar>, ker_منزل_Str<حرف>, ker_منزل_Str<ushort>, ker_منزل_Str<چھوٹا>,
                                                ker_منزل_Str<صحيح>, ker_منزل_Str<عائم>, ker_منزل_Str<مزدوج>, ker_منزل_Str<hfloat>,
                                                0, ker_منزل_Str<منطقي>, ker_منزل_Str<uint64_t>, ker_منزل_Str<int64_t>, ker_منزل_Str<بنانشانی> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    أرجع cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#تعریف_کرو PROCESS_SRC(src) \
    افعل \
    { \
        إذا (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            ناپ csize = src.size(); \
            صحيح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_ڈونگھائی(ctype), \
                ckercn = سمتیہ_Widths[cdepth], cwidth = ccn * csize.چوڑائی; \
            إذا (cwidth < ckercn || ckercn <= 0) \
                أرجع 1; \
            خانے_گنتی.push_back(cwidth); \
            إذا (strat == OCL_سمتیہ_OWN && ctype != ref_type) \
                أرجع 1; \
            offsets.push_back(src.offset()); \
            قدماں.push_back(src.قدم()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    طالما ((فراغ)0, 0)

صحيح predict_Optimal_سمتیہ_چوڑائی(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
                              داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
                              داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
                              Ocl_سمتیہ_Strategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحيح سمتیہ_Widths[CV_ڈونگھائی_MAX] = {
        d.preferred_سمتیہ_چوڑائی_Char(), d.preferred_سمتیہ_چوڑائی_Char(),   // CV_8U,  CV_8S
        d.preferred_سمتیہ_چوڑائی_Short(), d.preferred_سمتیہ_چوڑائی_Short(), // CV_16U, CV_16S
        d.preferred_سمتیہ_چوڑائی_Int(), d.preferred_سمتیہ_چوڑائی_Float(),   // CV_32S, CV_32F
        d.preferred_سمتیہ_چوڑائی_Double(), d.preferred_سمتیہ_چوڑائی_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_سمتیہ_چوڑائی_Char(),                                 // CV_Bool
        d.preferred_سمتیہ_چوڑائی_Long(), d.preferred_سمتیہ_چوڑائی_Long(),   // CV_64U, CV_64S
        d.preferred_سمتیہ_چوڑائی_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    إذا (سمتیہ_Widths[0] == 1)
    {
        // it's heuristic
        سمتیہ_Widths[CV_8U] = سمتیہ_Widths[CV_8S] = 4;
        سمتیہ_Widths[CV_16U] = سمتیہ_Widths[CV_16S] = سمتیہ_Widths[CV_16F] = 2;
        سمتیہ_Widths[CV_32S] = سمتیہ_Widths[CV_32F] = سمتیہ_Widths[CV_64F] = 1;
    }

    أرجع check_Optimal_سمتیہ_چوڑائی(سمتیہ_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحيح check_Optimal_سمتیہ_چوڑائی([[maybe_unused]] ثابت صحيح *سمتیہ_Widths,
                            [[maybe_unused]] داخلہ_Array src1,
                            [[maybe_unused]] داخلہ_Array src2,
                            [[maybe_unused]] داخلہ_Array src3,
                            [[maybe_unused]] داخلہ_Array src4,
                            [[maybe_unused]] داخلہ_Array src5,
                            [[maybe_unused]] داخلہ_Array src6,
                            [[maybe_unused]] داخلہ_Array src7,
                            [[maybe_unused]] داخلہ_Array src8,
                            [[maybe_unused]] داخلہ_Array src9,
                            [[maybe_unused]] Ocl_سمتیہ_Strategy strat)
{
#جے_تعریف_شدہ __APPLE__
    أرجع 1;
#وإلا
    CV_Assert(سمتیہ_Widths);

    صحيح ref_type = src1.type();

    std::vector<size_t> offsets, قدماں, خانے_گنتی;
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
        طالما (offsets[i] % dividers[i] != 0 || قدماں[i] % dividers[i] != 0 || خانے_گنتی[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحيح kercn = *std::min_element(kercns.begin(), kercns.end());

    أرجع kercn;
#جے_انت
}

صحيح predict_Optimal_سمتیہ_چوڑائی_Max(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
                                 داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
                                 داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9)
{
    أرجع predict_Optimal_سمتیہ_چوڑائی(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_سمتیہ_MAX);
}

#تعریف_رد PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
فراغ build_Options_جوڑو_میٹرکس_Description(String& buildOptions, ثابت String& name, داخلہ_Array _m)
{
    إذا (!buildOptions.empty())
        buildOptions += " ";
    صحيح type = _m.type(), ڈونگھائی = CV_MAT_ڈونگھائی(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_منزل_Str(type),
            name.c_str(), ocl::type_منزل_Str(CV_MAKE_TYPE(ڈونگھائی, 1)),
            name.c_str(), (صحيح)CV_MAT_CN(type),
            name.c_str(), (صحيح)CV_ELEM_ناپ(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحيح)ڈونگھائی
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

    ساكن cl_image_format getImageFormat(صحيح ڈونگھائی, صحيح cn, منطقي norm)
    {
        cl_image_format format;
        ساكن ثابت صحيح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساكن ثابت صحيح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساكن ثابت صحيح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحيح channelType = norm ? channelTypesNorm[ڈونگھائی] : channelTypes[ڈونگھائی];
        صحيح channel_ترتیب = channelOrders[cn];
        format.image_channel_انکڑے_type = (cl_channel_type)channelType;
        format.image_channel_ترتیب = (cl_channel_ترتیب)channel_ترتیب;
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
        cl_uint عدد_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, عدد_Formats,
                                                عدم, &عدد_Formats);
        CV_OCL_DBG_CHECK_نتیجہ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        إذا (عدد_Formats > 0)
        {
            Auto_بفر<cl_image_format> formats(عدد_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, عدد_Formats,
                                             formats.انکڑے(), عدم);
            CV_OCL_DBG_CHECK_نتیجہ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            لكل (cl_uint i = 0; i < عدد_Formats; ++i)
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

        صحيح err, ڈونگھائی = src.ڈونگھائی(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(ڈونگھائی, cn, norm);

        إذا (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        إذا (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_قطار قطار_2 = (cl_command_قطار)قطار::getDefault().ptr();

#جے_تعریف_شدہ CL_نسخہ_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحيح minor = d.device_نسخہ_Minor(), major = d.device_نسخہ_Major();
        CV_Assert(!alias || canCreateAlias(src));
        إذا (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_چوڑائی      = src.خانے_گنتی;
            desc.image_اچائی     = src.صفاں;
            desc.image_ڈونگھائی      = 0;
            desc.image_array_ناپ = 1;
            desc.image_صف_pitch  = alias ? src.قدم[0] : 0;
            desc.image_ٹکڑا_pitch = 0;
            desc.بفر           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.عدد_mip_پدھراں   = 0;
            desc.عدد_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, عدم, &err);
        }
        وإلا
#جے_انت
        {
            CV_SUPPRESS_DEPRECATED_شروع
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.خانے_گنتی, src.صفاں, 0, عدم, &err);
            CV_SUPPRESS_DEPRECATED_سرا
        }
        CV_OCL_DBG_CHECK_نتیجہ(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.خانے_گنتی), static_cast<size_t>(src.صفاں), 1 };

        cl_mem dev_انکڑے;
        إذا (!alias && !src.isContinuous())
        {
            dev_انکڑے = cl_Create_بفر(context, CL_MEM_READ_ONLY, src.خانے_گنتی * src.صفاں * src.elem_ناپ(), عدم, &err);
            CV_OCL_CHECK_نتیجہ(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (لما لما صحيح)(src.خانے_گنتی * src.صفاں * src.elem_ناپ()), (فراغ*)dev_انکڑے
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.خانے_گنتی) * src.elem_ناپ(), static_cast<size_t>(src.صفاں), 1};
            CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Copy_بفر_Rect(قطار_2, (cl_mem)src.handle(ACCESS_READ), dev_انکڑے, origin, origin,
                roi, src.قدم, 0, src.خانے_گنتی * src.elem_ناپ(), 0, 0, عدم, عدم));
            CV_OCL_DBG_CHECK(clFlush(قطار_2));
        }
        وإلا
        {
            dev_انکڑے = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_انکڑے != عدم);

        إذا (!alias)
        {
            CV_OCL_CHECK(cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(قطار_2, dev_انکڑے, handle, 0, origin, region, 0, عدم, 0));
            إذا (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(قطار_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_انکڑے));
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
        إذا (pitchAlign && !(m.قدم % (pitchAlign * m.elem_ناپ())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            إذا (!m.u->عارضی_UMat())
            {
                ret = سچ;
            }
        }
    }
    أرجع ret;
}

منطقي Image2D::isFormatSupported(صحيح ڈونگھائی, صحيح cn, منطقي norm)
{
    cl_image_format format = Impl::getImageFormat(ڈونگھائی, cn, norm);

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
    i.p = خالی_سوچک;
}

Image2D& Image2D::عامل = (Image2D&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا (p)
            p->release();
        p = i.p;
        i.p = خالی_سوچک;
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
#جے_انت
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

    فراغ شروع()
    {
        CV_OCL_DBG_CHECK(cl_انت((cl_command_قطار)قطار_2.ptr()));
        timer.شروع();
    }

    فراغ stop()
    {
        CV_OCL_DBG_CHECK(cl_انت((cl_command_قطار)قطار_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        أرجع (uint64)(timer.get_ویلا_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت قطار& q) : p(جديد Impl(q)) { }
Timer::~Timer() { مٹاؤ p; }

فراغ Timer::شروع()
{
    CV_Assert(p);
    p->شروع();
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

#جے_انت // HAVE_OPENCL
