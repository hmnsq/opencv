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

#اگر_غیر_تعریف_شدہ HAVE_OPENCL
#شامل "ocl_disabled.impl.hpp"
#ورنہ // HAVE_OPENCL

#شامل <list>
#شامل <map>
#شامل <deque>
#شامل <set>
#شامل <string>
#شامل <sstream>
#شامل <fstream>
#اگر !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#شامل <inttypes.h>
#اگر_ختم

#شامل <opencv2/core/utils/configuration.private.hpp>

#شامل <opencv2/core/utils/logger.defines.hpp>
#تعریف_منسوخ CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#شامل <opencv2/core/utils/logger.hpp>

#شامل "opencv2/core/ocl_genbase.hpp"
#شامل "opencl_kernels_core.hpp"

#شامل "opencv2/core/utils/lock.private.hpp"
#شامل "opencv2/core/utils/filesystem.hpp"
#شامل "opencv2/core/utils/filesystem.private.hpp"

#تعریف_کرو CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(تہی, 0, "OpenCL allocator: " << __VA_ARGS__)
#شامل "opencv2/core/utils/allocator_stats.impl.hpp"
#تعریف_منسوخ CV__ALLOCATOR_STATS_LOG

#تعریف_کرو CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#تعریف_کرو CV_OPENCL_SHOW_BUILD_OPTIONS             0
#تعریف_کرو CV_OPENCL_SHOW_BUILD_KERNELS             0

#تعریف_کرو CV_OPENCL_SHOW_تسلسل_KERNELS               0
#تعریف_کرو CV_OPENCL_SYNC_تسلسل_KERNELS               0
#تعریف_کرو CV_OPENCL_TRACE_CHECK                    0

#تعریف_کرو CV_OPENCL_VALIDATE_ثنائی_PROGRAMS       1

#تعریف_کرو CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#تعریف_کرو CV_OPENCL_SHOW_SVM_LOG                   0

#شامل "opencv2/core/bufferpool.hpp"
#اگر_غیر_تعریف_شدہ LOG_بفر_POOL
# اگر 0
#   define LOG_بفر_POOL لکھو
# ورنہ
#   define LOG_بفر_POOL(...)
# endif
#اگر_ختم

#اگر CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#تعریف_کرو CV_OPENCL_SVM_TRACE_P لکھو("line %d (ocl.cpp): ", __LINE__); لکھو
#ورنہ
#تعریف_کرو CV_OPENCL_SVM_TRACE_P(...)
#اگر_ختم

#اگر CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#تعریف_کرو CV_OPENCL_SVM_TRACE_ERROR_P لکھو("Error on line %d (ocl.cpp): ", __LINE__); لکھو
#ورنہ
#تعریف_کرو CV_OPENCL_SVM_TRACE_ERROR_P(...)
#اگر_ختم

#شامل "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#شامل "opencv2/core/opencl/runtime/opencl_core.hpp"

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
#شامل "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#شامل "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#شامل "opencv2/core/opencl/opencl_svm.hpp"
#اگر_ختم

#شامل "umatrix.hpp"

نامخلا cv { نامخلا ocl {

#تعریف_کرو IMPLEMENT_REFCOUNTABLE() \
    خالی addref() { CV_XADD(&refcount, 1); } \
    خالی release() { اگر( CV_XADD(&refcount, -1) == 1 && !cv::__termination) حذف یہ; } \
    صحیح refcount

ساکن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    واپس opencl_allocator_stats;
}

#اگر_غیر_تعریف_شدہ _DEBUG
ساکن منطقی isRaiseError()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی value = جھوٹ;
    اگر (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", جھوٹ);
        initialized = سچ;
    }
    واپس value;
}
#اگر_ختم

ساکن خالی onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", جھوٹ);
    اگر (value)
    {
        fprintf(stderr, "Abort on OpenCL kernel build failure!\n");
        abort();
    }
}

#اگر CV_OPENCL_TRACE_CHECK
ساکن اندرونی
خالی traceOpenCLCheck(cl_int حالت_2, ثابت حرف* message)
{
    std::خارج << "OpenCV(OpenCL:" << حالت_2 << "): " << message << std::سطر_ختم << std::flush;
}
#تعریف_کرو CV_OCL_TRACE_CHECK_نتیجہ(حالت_2, message) traceOpenCLCheck(حالت_2, message)
#ورنہ
#تعریف_کرو CV_OCL_TRACE_CHECK_نتیجہ(حالت_2, message) /* nothing */
#اگر_ختم

#تعریف_کرو CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_نتیجہ), check_نتیجہ, پیغام)

#تعریف_کرو CV_OCL_CHECK_نتیجہ(check_نتیجہ, پیغام) \
    کرو { \
        CV_OCL_TRACE_CHECK_نتیجہ(check_نتیجہ, پیغام); \
        اگر (check_نتیجہ != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام_2 = CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام_2); \
        } \
    } جب (0)

#تعریف_کرو CV_OCL_CHECK_(اظہاریہ, check_نتیجہ) کرو { اظہاریہ; CV_OCL_CHECK_نتیجہ(check_نتیجہ, #expr); } جب (0)

#تعریف_کرو CV_OCL_CHECK(اظہاریہ) کرو { cl_int __cl_result = (اظہاریہ); CV_OCL_CHECK_نتیجہ(__cl_result, #expr); } جب (0)

#اگر_تعریف_شدہ _DEBUG
#تعریف_کرو CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, پیغام) CV_OCL_CHECK_نتیجہ(check_نتیجہ, پیغام)
#تعریف_کرو CV_OCL_DBG_CHECK(اظہاریہ) CV_OCL_CHECK(اظہاریہ)
#تعریف_کرو CV_OCL_DBG_CHECK_(اظہاریہ, check_نتیجہ) CV_OCL_CHECK_(اظہاریہ, check_نتیجہ)
#ورنہ
#تعریف_کرو CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, پیغام) \
    کرو { \
        CV_OCL_TRACE_CHECK_نتیجہ(check_نتیجہ, پیغام); \
        اگر (check_نتیجہ != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام_2 = CV_OCL_API_ERROR_پیغام(check_نتیجہ, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام_2); \
        } \
    } جب (0)
#تعریف_کرو CV_OCL_DBG_CHECK_(اظہاریہ, check_نتیجہ) کرو { اظہاریہ; CV_OCL_DBG_CHECK_نتیجہ(check_نتیجہ, #expr); } جب (0)
#تعریف_کرو CV_OCL_DBG_CHECK(اظہاریہ) کرو { cl_int __cl_result = (اظہاریہ); CV_OCL_DBG_CHECK_نتیجہ(__cl_result, #expr); } جب (0)
#اگر_ختم


ساکن ثابت منطقی CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", سچ);
ساکن ثابت منطقی CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", سچ);
ساکن ثابت منطقی CV_OPENCL_CACHE_تالا_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", سچ);
ساکن ثابت منطقی CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", سچ);

#اگر CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
ساکن ثابت منطقی CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", جھوٹ);
#اگر_ختم

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساکن ثابت منطقی CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#اگر 1 //def __APPLE__
        سچ
#ورنہ
        جھوٹ
#اگر_ختم
);

ساکن String getBuildExtraOptions()
{
    ساکن String param_buildExtraOptions;
    ساکن منطقی initialized = جھوٹ;
    اگر (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = سچ;
        اگر (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(تہی, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    واپس param_buildExtraOptions;
}

ساکن ثابت منطقی CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", سچ);
ساکن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


ساخت UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحیح)m.offset;
        قدم = (صحیح)m.قدم;
        صفیں = m.صفیں;
        ستون_تعداد = m.ستون_تعداد;
    }
    صحیح offset;
    صحیح قدم;
    صحیح صفیں;
    صحیح ستون_تعداد;
};

ساخت UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحیح)m.offset;
        قدم = (صحیح)m.قدم.p[1];
        slicestep = (صحیح)m.قدم.p[0];
        slices = (صحیح)m.size.p[0];
        صفیں = m.size.p[1];
        ستون_تعداد = m.size.p[2];
    }
    صحیح offset;
    صحیح slicestep;
    صحیح قدم;
    صحیح slices;
    صحیح صفیں;
    صحیح ستون_تعداد;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ساکن uint64 crc64( ثابت uchar* ڈیٹا, size_t size, uint64 crc0=0 )
{
    ساکن uint64 table[256];
    ساکن منطقی initialized = جھوٹ;

    اگر( !initialized )
    {
        برائے( صحیح i = 0; i < 256; i++ )
        {
            uint64 c = i;
            برائے( صحیح j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = سچ;
    }

    uint64 چکری_جانچ = ~crc0;
    برائے( size_t idx = 0; idx < size; idx++ )
        چکری_جانچ = table[(uchar)چکری_جانچ ^ ڈیٹا[idx]] ^ (چکری_جانچ >> 8);

    واپس ~چکری_جانچ;
}

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
ساخت OpenCLBinaryCacheConfigurator
{
    cv::String cache_راستہ;
    cv::String cache_تالا_filename;
    cv::Ptr<utils::fs::File_تالا> cache_تالا;

    قسمتعریف std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(تہی, "Initializing OpenCL cache configuration...");
        اگر (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(تہی, "OpenCL cache is disabled");
            واپس;
        }
        cache_راستہ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        اگر (cache_راستہ.empty())
        {
            CV_LOG_INFO(تہی, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        کرو
        {
            کوشش
            {
                اگر (cache_راستہ.empty())
                    توڑو;
                اگر (cache_راستہ == "disabled")
                    توڑو;
                اگر (!utils::fs::createDirectories(cache_راستہ))
                {
                    CV_LOG_DEBUG(تہی, "Can't use OpenCL cache directory: " << cache_راستہ);
                    clear();
                    توڑو;
                }

                اگر (CV_OPENCL_CACHE_تالا_ENABLE)
                {
                    cache_تالا_filename = cache_راستہ + ".lock";
                    اگر (!utils::fs::exists(cache_تالا_filename))
                    {
                        CV_LOG_DEBUG(تہی, "Creating lock file... (" << cache_تالا_filename << ")");
                        std::ofstream تالا_filename(cache_تالا_filename.c_str(), std::ios::out);
                        اگر (!تالا_filename.is_open())
                        {
                            CV_LOG_WARNING(تہی, "Can't create lock file for OpenCL program cache: " << cache_تالا_filename);
                            توڑو;
                        }
                    }

                    کوشش
                    {
                        cache_تالا = makePtr<utils::fs::File_تالا>(cache_تالا_filename.c_str());
                        CV_LOG_VERBOSE(تہی, 0, "Checking cache lock... (" << cache_تالا_filename << ")");
                        {
                            utils::shared_تالا_guard<utils::fs::File_تالا> lock(*cache_تالا);
                        }
                        CV_LOG_VERBOSE(تہی, 0, "Checking cache lock... Done!");
                    }
                    پکڑو (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(تہی, "Can't create OpenCL program cache lock: " << cache_تالا_filename << std::سطر_ختم << e.what());
                    }
                    پکڑو (...)
                    {
                        CV_LOG_WARNING(تہی, "Can't create OpenCL program cache lock: " << cache_تالا_filename);
                    }
                }
                ورنہ
                {
                    اگر (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(تہی, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    ورنہ
                    {
                        CV_LOG_INFO(تہی, "OpenCL cache lock is disabled");
                    }
                }
            }
            پکڑو (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(تہی, "Can't prepare OpenCL program cache: " << cache_راستہ << std::سطر_ختم << e.what());
                clear();
            }
        } جب (0);
        اگر (!cache_راستہ.empty())
        {
            اگر (cache_تالا.empty() && CV_OPENCL_CACHE_تالا_ENABLE)
            {
                CV_LOG_WARNING(تہی, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            ورنہ
            {
                CV_LOG_INFO(تہی, "Successfully initialized OpenCL cache directory: " << cache_راستہ);
            }
        }
    }

    خالی clear()
    {
        cache_راستہ.clear();
        cache_تالا_filename.clear();
        cache_تالا.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_سابقہ,
            ثابت std::string& cleanup_سابقہ)
    {
        اگر (cache_راستہ.empty())
            واپس std::string();

        Auto_تالا lock(mutex_prepared_contexts_);

        ContextCacheType::iterator ملا_it = prepared_contexts_.find(ctx_سابقہ);
        اگر (ملا_it != prepared_contexts_.end())
            واپس ملا_it->second;

        CV_LOG_INFO(تہی, "Preparing OpenCL cache configuration for context: " << ctx_سابقہ);

        std::string ہدف_directory = cache_راستہ + ctx_سابقہ + "/";
        منطقی نتیجہ = utils::fs::isDirectory(ہدف_directory);
        اگر (!نتیجہ)
        {
            کوشش
            {
                CV_LOG_VERBOSE(تہی, 0, "Creating directory: " << ہدف_directory);
                اگر (utils::fs::createDirectories(ہدف_directory))
                {
                    نتیجہ = سچ;
                }
                ورنہ
                {
                    CV_LOG_WARNING(تہی, "Can't create directory: " << ہدف_directory);
                }
            }
            پکڑو (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(تہی, "Can't create OpenCL program cache directory for context: " << ہدف_directory << std::سطر_ختم << e.what());
            }
        }
        ہدف_directory = نتیجہ ? ہدف_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_سابقہ, ہدف_directory));

        اگر (نتیجہ && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_سابقہ.empty())
        {
            کوشش
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_راستہ, cleanup_سابقہ + "*", entries, جھوٹ, سچ);
                std::vector<String> remove_entries;
                برائے (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    اگر (0 == name.find(cleanup_سابقہ))
                    {
                        اگر (0 == name.find(ctx_سابقہ))
                            جاری; // skip current
                        remove_entries.push_back(name);
                    }
                }
                اگر (!remove_entries.empty())
                {
                    CV_LOG_WARNING(تہی, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(تہی, "Trying to remove these directories...");
                    برائے (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(تہی, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(تہی, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    برائے (size_t i = 0; i < remove_entries.size(); i++)
                    {
                        ثابت String& name = remove_entries[i];
                        cv::String راستہ = utils::fs::join(cache_راستہ, name);
                        کوشش
                        {
                            utils::fs::remove_all(راستہ);
                            CV_LOG_WARNING(تہی, "Removed: " << راستہ);
                        }
                        پکڑو (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(تہی, "Exception during removal of obsolete OpenCL cache directory: " << راستہ << std::سطر_ختم << e.what());
                        }
                    }
                }
            }
            پکڑو (...)
            {
                CV_LOG_WARNING(تہی, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(تہی, 1, "  Result: " << (ہدف_directory.empty() ? std::string("Failed") : ہدف_directory));
        واپس ہدف_directory;
    }

    ساکن OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, نیا OpenCLBinaryCacheConfigurator());
    }
};
جماعت ثنائی_پروگرام_File
{
    شمار { MAX_ENTRIES = 64 };

    قسمتعریف غیرعلامتی صحیح uint32_t;

    ساخت CV_DECL_ALIGNED(4) File_سرخی
    {
        uint32_t ماخذ_Signature_سائز;
        //char sourceSignature[];
    };

    ساخت CV_DECL_ALIGNED(4) File_جدول
    {
        uint32_t نمبر_کا_Entries;
        //uint32_t firstEntryOffset[];
    };

    ساخت CV_DECL_ALIGNED(4) File_اندراج
    {
        uint32_t اگلا_اندراج_File_انحراف; // 0 for the last entry in chain
        uint32_t کلید_سائز;
        uint32_t ڈیٹا_سائز;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت ماخذ_Signature;
    ثابت size_t ماخذ_Signature_سائز_2;

    std::fstream f;

    uint32_t اندراج_Offsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        واپس hash & (MAX_ENTRIES - 1);
    }

    اندرونی size_t get_File_سائز()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_سائز = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        واپس file_سائز;
    }
    اندرونی uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.اقرأ((حرف*)&res, حجم(uint32_t));
        CV_Assert(!f.fail());
        واپس res;
    }
    اندرونی خالی writeUInt32(ثابت uint32_t value)
    {
        uint32_t v = value;
        f.اكتب((حرف*)&v, حجم(uint32_t));
        CV_Assert(!f.fail());
    }

    اندرونی خالی seekReadAbsolute(size_t pos)
    {
        f.seekg(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }
    اندرونی خالی seekReadRelative(size_t pos)
    {
        f.seekg(pos, std::fstream::cur);
        CV_Assert(!f.fail());
    }

    اندرونی خالی seekWriteAbsolute(size_t pos)
    {
        f.seekp(pos, std::fstream::beg);
        CV_Assert(!f.fail());
    }

    خالی clearFile()
    {
        f.close();
        اگر (0 != remove(fileName_.c_str()))
            CV_LOG_ERROR(تہی, "Can't remove: " << fileName_);
        واپس;
    }

عوامی:
    ثنائی_پروگرام_File(ثابت std::string& fileName, ثابت حرف* ماخذ_Signature_2)
        : fileName_(fileName), ماخذ_Signature(ماخذ_Signature_2), ماخذ_Signature_سائز_2(ماخذ_Signature ? لڑی_لمبائی(ماخذ_Signature) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(ماخذ_Signature != تہی);
        CV_Assert(ماخذ_Signature_سائز_2 > 0);
        memset(اندراج_Offsets, 0, حجم(اندراج_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        اگر(f.is_open() && get_File_سائز() > 0)
        {
            منطقی isValid = جھوٹ;
            کوشش
            {
                uint32_t file_ماخذ_Signature_سائز = readUInt32();
                اگر (file_ماخذ_Signature_سائز == ماخذ_Signature_سائز_2)
                {
                    cv::Auto_بفر<حرف> file_ماخذ_Signature(file_ماخذ_Signature_سائز + 1);
                    f.اقرأ(file_ماخذ_Signature.ڈیٹا(), file_ماخذ_Signature_سائز);
                    اگر (f.eof())
                    {
                        CV_LOG_ERROR(تہی, "Unexpected EOF");
                    }
                    ورنہ اگر (memcmp(ماخذ_Signature_2, file_ماخذ_Signature.ڈیٹا(), file_ماخذ_Signature_سائز) == 0)
                    {
                        isValid = سچ;
                    }
                }
                اگر (!isValid)
                {
                    CV_LOG_ERROR(تہی, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            پکڑو (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(تہی, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            پکڑو (...)
            {
                CV_LOG_ERROR(تہی, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            اگر (!isValid)
            {
                clearFile();
            }
            ورنہ
            {
                seekReadAbsolute(0);
            }
        }
    }

    منطقی اقرأ(ثابت std::string& کلید, std::vector<حرف>& buf)
    {
        اگر (!f.is_open())
            واپس جھوٹ;

        size_t file_سائز = get_File_سائز();
        اگر (file_سائز == 0)
        {
            CV_LOG_ERROR(تہی, "Invalid file (empty): " << fileName_);
            clearFile();
            واپس جھوٹ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ماخذ_Signature_سائز = readUInt32();
        CV_Assert(file_ماخذ_Signature_سائز > 0);
        seekReadRelative(file_ماخذ_Signature_سائز);

        uint32_t نمبر_کا_Entries = readUInt32();
        CV_Assert(نمبر_کا_Entries > 0);
        اگر (نمبر_کا_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(تہی, "Invalid file: " << fileName_);
            clearFile();
            واپس جھوٹ;
        }
        f.اقرأ((حرف*)&اندراج_Offsets[0], حجم(اندراج_Offsets));
        CV_Assert(!f.fail());

        uint32_t اندراج_عدد = getHash(کلید);

        uint32_t اندراج_انحراف = اندراج_Offsets[اندراج_عدد];
        File_اندراج اندراج_2;
        جب (اندراج_انحراف > 0)
        {
            seekReadAbsolute(اندراج_انحراف);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&اندراج_2, حجم(اندراج_2));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلید(اندراج_2.کلید_سائز + 1);
            اگر (کلید.size() == اندراج_2.کلید_سائز)
            {
                اگر (اندراج_2.کلید_سائز > 0)
                {
                    f.اقرأ(file_کلید.ڈیٹا(), اندراج_2.کلید_سائز);
                    CV_Assert(!f.fail());
                }
                اگر (memcmp(file_کلید.ڈیٹا(), کلید.c_str(), اندراج_2.کلید_سائز) == 0)
                {
                    buf.resize(اندراج_2.ڈیٹا_سائز);
                    f.اقرأ(&buf[0], اندراج_2.ڈیٹا_سائز);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(تہی, 0, "Read...");
                    واپس سچ;
                }
            }
            اگر (اندراج_2.اگلا_اندراج_File_انحراف == 0)
                توڑو;
            اندراج_انحراف = اندراج_2.اگلا_اندراج_File_انحراف;
        }
        واپس جھوٹ;
    }

    منطقی اكتب(ثابت std::string& کلید, std::vector<حرف>& buf)
    {
        اگر (!f.is_open())
        {
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            اگر (!f.is_open())
            {
                f.open(fileName_.c_str(), std::ios::out|std::ios::binary);
                اگر (!f.is_open())
                {
                    CV_LOG_ERROR(تہی, "Can't create file: " << fileName_);
                    واپس جھوٹ;
                }
            }
        }

        size_t file_سائز = get_File_سائز();
        اگر (file_سائز == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)ماخذ_Signature_سائز_2);
            f.اكتب(ماخذ_Signature, ماخذ_Signature_سائز_2);
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
            file_سائز = get_File_سائز();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_ماخذ_Signature_سائز = readUInt32();
        CV_Assert(file_ماخذ_Signature_سائز == ماخذ_Signature_سائز_2);
        seekReadRelative(file_ماخذ_Signature_سائز);

        uint32_t نمبر_کا_Entries = readUInt32();
        CV_Assert(نمبر_کا_Entries > 0);
        اگر (نمبر_کا_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(تہی, "Invalid file: " << fileName_);
            clearFile();
            واپس جھوٹ;
        }
        size_t جدول_Entries_انحراف = (size_t)f.tellg();
        f.اقرأ((حرف*)&اندراج_Offsets[0], حجم(اندراج_Offsets));
        CV_Assert(!f.fail());

        uint32_t اندراج_عدد = getHash(کلید);

        uint32_t اندراج_انحراف = اندراج_Offsets[اندراج_عدد];
        File_اندراج اندراج_2;
        جب (اندراج_انحراف > 0)
        {
            seekReadAbsolute(اندراج_انحراف);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&اندراج_2, حجم(اندراج_2));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلید(اندراج_2.کلید_سائز + 1);
            اگر (کلید.size() == اندراج_2.کلید_سائز)
            {
                اگر (اندراج_2.کلید_سائز > 0)
                {
                    f.اقرأ(file_کلید.ڈیٹا(), اندراج_2.کلید_سائز);
                    CV_Assert(!f.fail());
                }
                اگر (0 == memcmp(file_کلید.ڈیٹا(), کلید.c_str(), اندراج_2.کلید_سائز))
                {
                    // duplicate
                    CV_LOG_VERBOSE(تہی, 0, "Duplicate key ignored: " << fileName_);
                    واپس جھوٹ;
                }
            }
            اگر (اندراج_2.اگلا_اندراج_File_انحراف == 0)
                توڑو;
            اندراج_انحراف = اندراج_2.اگلا_اندراج_File_انحراف;
        }
        seekReadAbsolute(0);
        اگر (اندراج_انحراف > 0)
        {
            seekWriteAbsolute(اندراج_انحراف);
            اندراج_2.اگلا_اندراج_File_انحراف = (uint32_t)file_سائز;
            f.اكتب((حرف*)&اندراج_2, حجم(اندراج_2));
            CV_Assert(!f.fail());
        }
        ورنہ
        {
            اندراج_Offsets[اندراج_عدد] = (uint32_t)file_سائز;
            seekWriteAbsolute(جدول_Entries_انحراف);
            f.اكتب((حرف*)اندراج_Offsets, حجم(اندراج_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_سائز);
        اندراج_2.اگلا_اندراج_File_انحراف = 0;
        اندراج_2.ڈیٹا_سائز = (uint32_t)buf.size();
        اندراج_2.کلید_سائز = (uint32_t)کلید.size();
        f.اكتب((حرف*)&اندراج_2, حجم(اندراج_2));
        CV_Assert(!f.fail());
        f.اكتب(کلید.c_str(), اندراج_2.کلید_سائز);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], اندراج_2.ڈیٹا_سائز);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(تہی, 0, "Write... (" << buf.size() << " bytes)");
        واپس سچ;
    }
};
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT



ساخت OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحیح device_;  // device index in context
    ocl::قطار قطار_3;
    صحیح useOpenCL_;

محفوظ:
    Impl() = حذف;

    خالی _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحیح ndevices = (صحیح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقی ملا = جھوٹ;
        برائے (صحیح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            اگر (dhandle == deviceID)
            {
                device_ = i;
                ملا = سچ;
                توڑو;
            }
        }
        CV_Assert(ملا && "OpenCL device can't work with passed OpenCL context");
    }

    خالی _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحیح ndevices = (صحیح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقی ملا = جھوٹ;
        برائے (صحیح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            اگر (d.getImpl() == device.getImpl())
            {
                device_ = i;
                ملا = سچ;
                توڑو;
            }
        }
        CV_Assert(ملا && "OpenCL device can't work with passed OpenCL context");
    }

عوامی:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_غیر_مستعمل(platformID);
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

    Impl(ثابت ocl::Context& context, ثابت صحیح device, ثابت ocl::قطار& قطار_2)
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

    اندرونی منطقی useOpenCL() ثابت { واپس const_cast<Impl*>(یہ)->useOpenCL(); }
    منطقی useOpenCL()
    {
        اگر (useOpenCL_ < 0)
        {
            کوشش
            {
                useOpenCL_ = 0;
                اگر (!context_.empty() && context_.ndevices() > 0)
                {
                    ثابت Device& d = context_.device(device_);
                    useOpenCL_ = d.دستیاب();
                }
            }
            پکڑو (ثابت cv::Exception&)
            {
                // nothing
            }
            اگر (!useOpenCL_)
                CV_LOG_INFO(تہی, "OpenCL: can't use OpenCL execution context");
        }
        واپس useOpenCL_ > 0;
    }

    خالی سیٹ_Use_Open_CL(منطقی flag)
    {
        اگر (!flag)
            useOpenCL_ = 0;
        ورنہ
            useOpenCL_ = -1;
    }

    ساکن ثابت std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(تہی, "OpenCL: initializing thread execution context");

        ساکن منطقی initialized = جھوٹ;
        ساکن std::shared_ptr<Impl> g_primaryExecutionContext;

        اگر (!initialized)
        {
            cv::Auto_تالا lock(getInitializationMutex());
            اگر (!initialized)
            {
                CV_LOG_INFO(تہی, "OpenCL: creating new execution context...");
                کوشش
                {
                    Context c = ocl::Context::create(std::string());
                    اگر (c.ndevices())
                    {
                        صحیح deviceId = 0;
                        خودکار& d = c.device(deviceId);
                        اگر (d.دستیاب())
                        {
                            خودکار q = ocl::قطار(c, d);
                            اگر (!q.ptr())
                            {
                                CV_LOG_ERROR(تہی, "OpenCL: Can't create default OpenCL queue");
                            }
                            ورنہ
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(تہی, "OpenCL: device=" << d.name());
                            }
                        }
                        ورنہ
                        {
                            CV_LOG_ERROR(تہی, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    ورنہ
                    {
                        CV_LOG_INFO(تہی, "OpenCL: context is not available/disabled");
                    }
                }
                پکڑو (ثابت std::exception& e)
                {
                    CV_LOG_INFO(تہی, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                پکڑو (...)
                {
                    CV_LOG_WARNING(تہی, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = سچ;
            }
        }
        واپس g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() ثابت
{
    CV_Assert(p);
    واپس p->context_;
}
Device& OpenCLExecutionContext::getDevice() ثابت
{
    CV_Assert(p);
    واپس p->context_.device(p->device_);
}
قطار& OpenCLExecutionContext::get_قطار() ثابت
{
    CV_Assert(p);
    واپس p->قطار_3;
}

منطقی OpenCLExecutionContext::useOpenCL() ثابت
{
    اگر (p)
        واپس p->useOpenCL();
    واپس جھوٹ;
}
خالی OpenCLExecutionContext::سیٹ_Use_Open_CL(منطقی flag)
{
    CV_Assert(p);
    p->سیٹ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    OpenCLExecutionContext& c = ڈیٹا.oclExecutionContext;
    اگر (!ڈیٹا.oclExecutionContextInitialized)
    {
        ڈیٹا.oclExecutionContextInitialized = سچ;
        اگر (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    واپس c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    OpenCLExecutionContext& c = ڈیٹا.oclExecutionContext;
    واپس c;
}

خالی OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    ڈیٹا.oclExecutionContext = *یہ;
    ڈیٹا.oclExecutionContextInitialized = سچ;
    ڈیٹا.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت قطار q(getContext(), getDevice());
    واپس clone_With_New_قطار(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_قطار(ثابت ocl::قطار& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != تہی);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    واپس c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::قطار& قطار_2)
{
    CV_TRACE_FUNCTION();
    اگر (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, قطار_2);
    واپس ctx;

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
    واپس ctx;

}

خالی OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
ساکن منطقی g_isOpenCLInitialized = جھوٹ;
ساکن منطقی g_isOpenCLAvailable = جھوٹ;

منطقی haveOpenCL()
{
    CV_TRACE_FUNCTION();

    اگر (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string env_راستہ = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        اگر (!env_راستہ.empty())
        {
            اگر (env_راستہ == "disabled")
            {
                g_isOpenCLAvailable = جھوٹ;
                g_isOpenCLInitialized = سچ;
                واپس جھوٹ;
            }
        }

        cv::Auto_تالا lock(getInitializationMutex());
        CV_LOG_INFO(تہی, "Initialize OpenCL runtime...");
        کوشش
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, تہی, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(تہی, "OpenCL: found " << n << " platforms");
        }
        پکڑو (...)
        {
            g_isOpenCLAvailable = جھوٹ;
        }
        g_isOpenCLInitialized = سچ;
    }
    واپس g_isOpenCLAvailable;
}

منطقی useOpenCL()
{
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    اگر (ڈیٹا.useOpenCL < 0)
    {
        کوشش
        {
            ڈیٹا.useOpenCL = 0;
            اگر (haveOpenCL())
            {
                خودکار c = OpenCLExecutionContext::getCurrent();
                ڈیٹا.useOpenCL = c.useOpenCL();
            }
        }
        پکڑو (...)
        {
            CV_LOG_INFO(تہی, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    واپس ڈیٹا.useOpenCL > 0;
}

منطقی isOpenCLActivated()
{
    اگر (!g_isOpenCLAvailable)
        واپس جھوٹ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    واپس useOpenCL();
}

خالی سیٹ_Use_Open_CL(منطقی flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    خودکار& c = OpenCLExecutionContext::getCurrentRef();
    اگر (!c.empty())
    {
        c.سیٹ_Use_Open_CL(flag);
        ڈیٹا.useOpenCL = c.useOpenCL();
    }
    ورنہ
    {
        اگر (!flag)
            ڈیٹا.useOpenCL = 0;
        ورنہ
            ڈیٹا.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#اگر_تعریف_شدہ HAVE_CLAMDBLAS

جماعت AmdBlasHelper
{
عوامی:
    ساکن AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, نیا AmdBlasHelper())
    }

    منطقی is_دستیاب() ثابت
    {
        واپس g_is_Amd_Blas_دستیاب;
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

محفوظ:
    AmdBlasHelper()
    {
        اگر (!g_isAmdBlasInitialized)
        {
            Auto_تالا lock(getInitializationMutex());

            اگر (!g_isAmdBlasInitialized)
            {
                اگر (haveOpenCL())
                {
                    کوشش
                    {
                        g_is_Amd_Blas_دستیاب = clblas_تیاری() == clblasSuccess;
                    }
                    پکڑو (...)
                    {
                        g_is_Amd_Blas_دستیاب = جھوٹ;
                    }
                }
                ورنہ
                    g_is_Amd_Blas_دستیاب = جھوٹ;

                g_isAmdBlasInitialized = سچ;
            }
        }
    }

نجی:
    ساکن منطقی g_isAmdBlasInitialized;
    ساکن منطقی g_is_Amd_Blas_دستیاب;
};

منطقی AmdBlasHelper::g_is_Amd_Blas_دستیاب = جھوٹ;
منطقی AmdBlasHelper::g_isAmdBlasInitialized = جھوٹ;

منطقی haveAmdBlas()
{
    واپس AmdBlasHelper::getInstance().is_دستیاب();
}

#ورنہ

منطقی haveAmdBlas()
{
    واپس جھوٹ;
}

#اگر_ختم

#اگر_تعریف_شدہ HAVE_CLAMDFFT

جماعت AmdFftHelper
{
عوامی:
    ساکن AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, نیا AmdFftHelper())
    }

    منطقی is_دستیاب() ثابت
    {
        واپس g_is_Amd_Fft_دستیاب;
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

محفوظ:
    AmdFftHelper()
    {
        اگر (!g_isAmdFftInitialized)
        {
            Auto_تالا lock(getInitializationMutex());

            اگر (!g_isAmdFftInitialized)
            {
                اگر (haveOpenCL())
                {
                    کوشش
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_تیاری_ڈیٹا(&تیاری_ڈیٹا) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_نسخہ(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_دستیاب = سچ;
                    }
                    پکڑو (ثابت Exception &)
                    {
                        g_is_Amd_Fft_دستیاب = جھوٹ;
                    }
                }
                ورنہ
                    g_is_Amd_Fft_دستیاب = جھوٹ;

                g_isAmdFftInitialized = سچ;
            }
        }
    }

نجی:
    ساکن clfft_تیاری_ڈیٹا تیاری_ڈیٹا;
    ساکن منطقی g_isAmdFftInitialized;
    ساکن منطقی g_is_Amd_Fft_دستیاب;
};

clfft_تیاری_ڈیٹا AmdFftHelper::تیاری_ڈیٹا;
منطقی AmdFftHelper::g_is_Amd_Fft_دستیاب = جھوٹ;
منطقی AmdFftHelper::g_isAmdFftInitialized = جھوٹ;

منطقی haveAmdFft()
{
    واپس AmdFftHelper::getInstance().is_دستیاب();
}

#ورنہ

منطقی haveAmdFft()
{
    واپس جھوٹ;
}

#اگر_ختم

منطقی haveSVM()
{
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
    واپس سچ;
#ورنہ
    واپس جھوٹ;
#اگر_ختم
}

خالی اختتام_2()
{
    قطار::getDefault().اختتام_2();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

ساخت Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = جھوٹ;
    }

    ~Impl() {}

    خالی init()
    {
        اگر( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            اگر( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            اگر( handle != 0 )
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

Platform& Platform::عامل = (ثابت Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = خالی_اشاریہ;
}

Platform& Platform::عامل = (Platform&& pl) CV_NOEXCEPT
{
    اگر (یہ != &pl) {
        اگر(p)
            p->release();
        p = pl.p;
        pl.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

خالی* Platform::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(تہی, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    ساکن Platform p;
    اگر( !p.p )
    {
        p.p = نیا Impl;
        p.p->init();
    }
    واپس p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
ساکن خالی parseOpenCLVersion(ثابت String &نسخہ, صحیح &major, صحیح &minor)
{
    major = minor = 0;
    اگر (10 >= نسخہ.length())
        واپس;
    ثابت حرف *pstr = نسخہ.c_str();
    اگر (0 != لڑی_موازنہ(pstr, "OpenCL ", 7))
        واپس;
    size_t ppos = نسخہ.find('.', 7);
    اگر (String::npos == ppos)
        واپس;
    String عارضی_2 = نسخہ.substr(7, ppos - 7);
    major = atoi(عارضی_2.c_str());
    عارضی_2 = نسخہ.substr(ppos + 1);
    minor = atoi(عارضی_2.c_str());
}

ساخت Device::Impl
{
    Impl(خالی* d)
        : refcount(1)
        , handle(0)
    {
        کوشش
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        پکڑو (...)
        {
            پھینکو;
        }
    }

    خالی _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        نسخہ_3 = getStrProp(CL_DEVICE_نسخہ);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحیح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحیح>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_حافظہ = getBoolProp(CL_DEVICE_HOST_UNIFIED_حافظہ);
        maxComputeUnits_ = getProp<cl_uint, صحیح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_گروہ_سائز_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_گروہ_سائز);
        type_ = getProp<cl_device_type, صحیح>(CL_DEVICE_TYPE);
        driver_نسخہ_2 = getStrProp(CL_DRIVER_نسخہ);
        پتہ_بٹ_2 = getProp<cl_uint, صحیح>(CL_DEVICE_پتہ_بٹ);

        String device_نسخہ = getStrProp(CL_DEVICE_نسخہ);
        parseOpenCLVersion(device_نسخہ, device_نسخہ_Major_2, device_نسخہ_Minor_2);

        size_t pos = 0;
        جب (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            اگر (pos2 == String::npos)
                pos2 = extensions_.size();
            اگر (pos2 > pos)
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
        اگر (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        ورنہ اگر (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || لڑی_تلاش(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        ورنہ اگر (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        ورنہ
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_گروہ_سائز = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        اگر (CV_OPENCL_DEVICE_MAX_WORK_گروہ_سائز > 0)
        {
            ثابت size_t new_max_Work_گروہ_سائز = std::min(max_Work_گروہ_سائز_2, CV_OPENCL_DEVICE_MAX_WORK_گروہ_سائز);
            اگر (new_max_Work_گروہ_سائز != max_Work_گروہ_سائز_2)
                CV_LOG_WARNING(تہی, "OpenCL: using workgroup size: " << new_max_Work_گروہ_سائز << " (was " << max_Work_گروہ_سائز_2 << ")");
            max_Work_گروہ_سائز_2 = new_max_Work_گروہ_سائز;
        }
#اگر 0
        اگر (isExtensionSupported("cl_khr_spir"))
        {
#اگر_غیر_تعریف_شدہ CL_DEVICE_SPIR_VERSIONS
#تعریف_کرو CL_DEVICE_SPIR_VERSIONS                     0x40E0
#اگر_ختم
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::خارج << spir_versions << std::سطر_ختم;
        }
#اگر_ختم
    }

    ~Impl()
    {
#اگر_تعریف_شدہ _WIN32
        اگر (!cv::__termination)
#اگر_ختم
        {
            اگر (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    سانچہ<قسم_نام _TpCL, قسم_نام Tp_برآمد>
    Tp_برآمد getProp(cl_device_info prop) ثابت
    {
        _TpCL عارضی_2=_TpCL();
        size_t sz = 0;

        واپس clGetDeviceInfo(handle, prop, حجم(عارضی_2), &عارضی_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضی_2) ? Tp_برآمد(عارضی_2) : Tp_برآمد();
    }

    منطقی getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool عارضی_2 = CL_FALSE;
        size_t sz = 0;

        واپس clGetDeviceInfo(handle, prop, حجم(عارضی_2), &عارضی_2, &sz) == CL_SUCCESS &&
            sz == حجم(عارضی_2) ? عارضی_2 != 0 : جھوٹ;
    }

    String getStrProp(cl_device_info prop) ثابت
    {
        حرف buf[4096];
        size_t sz=0;
        واپس clGetDeviceInfo(handle, prop, حجم(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < حجم(buf) ? String(buf) : String();
    }

    منطقی isExtensionSupported(ثابت std::string& extensionName) ثابت
    {
        واپس extensions_سیٹ.گنتی_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String نسخہ_3;
    std::string extensions_;
    صحیح doubleFPConfig_;
    منطقی khr_fp64_support_;
    صحیح halfFPConfig_;
    منطقی khr_fp16_support_;
    منطقی host_Unified_حافظہ;
    صحیح maxComputeUnits_;
    size_t max_Work_گروہ_سائز_2;
    صحیح type_;
    صحیح پتہ_بٹ_2;
    صحیح device_نسخہ_Major_2;
    صحیح device_نسخہ_Minor_2;
    String driver_نسخہ_2;
    String vendorName_;
    صحیح vendorID_;
    منطقی intelSubgroupsSupport_;

    std::سیٹ<std::string> extensions_سیٹ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(خالی* d)
{
    p = 0;
    سیٹ(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    اگر(p)
        p->addref();
}

Device& Device::عامل = (ثابت Device& d)
{
    Impl* newp = (Impl*)d.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = خالی_اشاریہ;
}

Device& Device::عامل = (Device&& d) CV_NOEXCEPT
{
    اگر (یہ != &d) {
        اگر(p)
            p->release();
        p = d.p;
        d.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

Device::~Device()
{
    اگر(p)
        p->release();
}

خالی Device::سیٹ(خالی* d)
{
    اگر(p)
        p->release();
    p = نیا Impl(d);
    اگر (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(خالی* d)
{
    Device device(d);
    واپس device;
}

خالی* Device::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

String Device::name() ثابت
{ واپس p ? p->name_ : String(); }

String Device::extensions() ثابت
{ واپس p ? String(p->extensions_) : String(); }

منطقی Device::isExtensionSupported(ثابت String& extensionName) ثابت
{ واپس p ? p->isExtensionSupported(extensionName) : جھوٹ; }

String Device::نسخہ() ثابت
{ واپس p ? p->نسخہ_3 : String(); }

String Device::vendorName() ثابت
{ واپس p ? p->vendorName_ : String(); }

صحیح Device::vendorID() ثابت
{ واپس p ? p->vendorID_ : 0; }

String Device::Open_CL_C_نسخہ() ثابت
{ واپس p ? p->getStrProp(CL_DEVICE_OPENCL_C_نسخہ) : String(); }

String Device::OpenCLVersion() ثابت
{ واپس p ? p->getStrProp(CL_DEVICE_نسخہ) : String(); }

صحیح Device::device_نسخہ_Major() ثابت
{ واپس p ? p->device_نسخہ_Major_2 : 0; }

صحیح Device::device_نسخہ_Minor() ثابت
{ واپس p ? p->device_نسخہ_Minor_2 : 0; }

String Device::driver_نسخہ() ثابت
{ واپس p ? p->driver_نسخہ_2 : String(); }

صحیح Device::type() ثابت
{ واپس p ? p->type_ : 0; }

صحیح Device::پتہ_بٹ() ثابت
{ واپس p ? p->پتہ_بٹ_2 : 0; }

منطقی Device::دستیاب() ثابت
{ واپس p ? p->getBoolProp(CL_DEVICE_دستیاب) : جھوٹ; }

منطقی Device::compiler_دستیاب() ثابت
{ واپس p ? p->getBoolProp(CL_DEVICE_COMPILER_دستیاب) : جھوٹ; }

منطقی Device::linker_دستیاب() ثابت
#اگر_تعریف_شدہ CL_نسخہ_1_2
{ واپس p ? p->getBoolProp(CL_DEVICE_LINKER_دستیاب) : جھوٹ; }
#ورنہ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#اگر_ختم

صحیح Device::doubleFPConfig() ثابت
{ واپس p ? p->doubleFPConfig_ : 0; }

صحیح Device::singleFPConfig() ثابت
{ واپس p ? p->getProp<cl_device_fp_config, صحیح>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

صحیح Device::halfFPConfig() ثابت
{ واپس p ? p->halfFPConfig_ : 0; }

منطقی Device::hasFP64() ثابت
{ واپس p ? p->khr_fp64_support_ : جھوٹ; }
منطقی Device::hasFP16() ثابت
{ واپس p ? p->khr_fp16_support_ : جھوٹ; }

منطقی Device::endianLittle() ثابت
{ واپس p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : جھوٹ; }

منطقی Device::errorCorrectionSupport() ثابت
{ واپس p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : جھوٹ; }

صحیح Device::executionCapabilities() ثابت
{ واپس p ? p->getProp<cl_device_exec_capabilities, صحیح>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

size_t Device::global_Mem_Cache_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_سائز) : 0; }

صحیح Device::globalMemCacheType() ثابت
{ واپس p ? p->getProp<cl_device_mem_cache_type, صحیح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحیح Device::global_Mem_Cache_سطر_سائز() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_سائز) : 0; }

size_t Device::global_Mem_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_سائز) : 0; }

size_t Device::local_Mem_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_سائز) : 0; }

صحیح Device::localMemType() ثابت
{ واپس p ? p->getProp<cl_device_local_mem_type, صحیح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقی Device::host_Unified_حافظہ_2() ثابت
{ واپس p ? p->host_Unified_حافظہ : جھوٹ; }

منطقی Device::imageSupport() ثابت
{ واپس p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : جھوٹ; }

منطقی Device::image_From_بفر_Support() ثابت
{
    واپس p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : جھوٹ;
}

uint Device::imagePitchAlignment() ثابت
{
#اگر_تعریف_شدہ CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    واپس p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#ورنہ
    واپس 0;
#اگر_ختم
}

uint Device::image_بنیاد_پتہ_Alignment() ثابت
{
#اگر_تعریف_شدہ CL_DEVICE_IMAGE_بنیاد_پتہ_ALIGNMENT
    واپس p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_بنیاد_پتہ_ALIGNMENT) : 0;
#ورنہ
    واپس 0;
#اگر_ختم
}

size_t Device::image2_DMax_چوڑائی() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_چوڑائی) : 0; }

size_t Device::image2_DMax_اونچائی() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_اونچائی) : 0; }

size_t Device::image3_DMax_چوڑائی() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_چوڑائی) : 0; }

size_t Device::image3_DMax_اونچائی() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_اونچائی) : 0; }

size_t Device::image3_DMax_گہرائی() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_گہرائی) : 0; }

size_t Device::image_Max_بفر_سائز() ثابت
#اگر_تعریف_شدہ CL_نسخہ_1_2
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_بفر_سائز) : 0; }
#ورنہ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#اگر_ختم

size_t Device::image_Max_Array_سائز() ثابت
#اگر_تعریف_شدہ CL_نسخہ_1_2
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_سائز) : 0; }
#ورنہ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#اگر_ختم

منطقی Device::intelSubgroupsSupport() ثابت
{ واپس p ? p->intelSubgroupsSupport_ : جھوٹ; }

صحیح Device::maxClockFrequency() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحیح Device::maxComputeUnits() ثابت
{ واپس p ? p->maxComputeUnits_ : 0; }

صحیح Device::max_Constant_دلائل() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_CONSTANT_دلائل) : 0; }

size_t Device::max_Constant_بفر_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_بفر_سائز) : 0; }

size_t Device::max_Mem_Alloc_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_سائز) : 0; }

size_t Device::max_Parameter_سائز() ثابت
{ واپس p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_سائز) : 0; }

صحیح Device::max_Read_Image_دلائل() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_READ_IMAGE_دلائل) : 0; }

صحیح Device::max_Write_Image_دلائل() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_WRITE_IMAGE_دلائل) : 0; }

صحیح Device::maxSamplers() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_گروہ_سائز() ثابت
{ واپس p ? p->max_Work_گروہ_سائز_2 : 0; }

صحیح Device::max_Work_شے_Dims() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MAX_WORK_شے_DIMENSIONS) : 0; }

خالی Device::max_Work_شے_Sizes(size_t* sizes) ثابت
{
    اگر(p)
    {
        ثابت صحیح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_شے_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحیح Device::mem_بنیاد_Addr_Align() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_MEM_بنیاد_ADDR_ALIGN) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Char() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_CHAR) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Short() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_SHORT) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Int() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_INT) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Long() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_LONG) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Float() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_FLOAT) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Double() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_DOUBLE) : 0; }

صحیح Device::native_سمتیہ_چوڑائی_Half() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_NATIVE_سمتیہ_چوڑائی_HALF) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Char() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_CHAR) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Short() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_SHORT) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Int() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_INT) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Long() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_LONG) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Float() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_FLOAT) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Double() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_DOUBLE) : 0; }

صحیح Device::preferred_سمتیہ_چوڑائی_Half() ثابت
{ واپس p ? p->getProp<cl_uint, صحیح>(CL_DEVICE_PREFERRED_سمتیہ_چوڑائی_HALF) : 0; }

size_t Device::printf_بفر_سائز() ثابت
#اگر_تعریف_شدہ CL_نسخہ_1_2
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_بفر_سائز) : 0; }
#ورنہ
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#اگر_ختم


size_t Device::profilingTimerResolution() ثابت
{ واپس p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    خودکار& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        واپس c.getDevice();
    }

    ساکن Device dummy;
    واپس dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

سانچہ <قسم_نام Functor, قسم_نام ObjectType>
اندرونی cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, تہی, &required);
    اگر (err != CL_SUCCESS)
        واپس err;

    param.clear();
    اگر (required > 0)
    {
        Auto_بفر<حرف> buf(required + 1);
        حرف* ptr = buf.ڈیٹا(); // cleanup is not needed
        err = f(obj, name, required, ptr, تہی);
        اگر (err != CL_SUCCESS)
            واپس err;
        param = ptr;
    }

    واپس CL_SUCCESS;
}

ساکن خالی بانٹو(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    اگر (s.size() == 0)
        واپس;
    std::istringstream ss(s);
    std::string شے;
    جب (!ss.eof())
    {
        std::getline(ss, شے, delim);
        elems.push_back(شے);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
ساکن منطقی parseOpenCLDeviceConfiguration(ثابت std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    بانٹو(configurationStr, ':', parts);
    اگر (parts.size() > 3)
    {
        CV_LOG_ERROR(تہی, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        واپس جھوٹ;
    }
    اگر (parts.size() > 2)
        deviceNameOrID = parts[2];
    اگر (parts.size() > 1)
    {
        بانٹو(parts[1], '|', deviceTypes);
    }
    اگر (parts.size() > 0)
    {
        platform = parts[0];
    }
    واپس سچ;
}

ساکن cl_device_id selectOpenCLDevice(ثابت std::string & configuration_ = std::string())
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
        واپس تہی;

    منطقی isID = جھوٹ;
    صحیح deviceID = -1;
    اگر (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = سچ;
        برائے (size_t i = 0; i < deviceName.length(); i++)
        {
            اگر (!ہندسہ_ہے(deviceName[i]))
            {
                isID = جھوٹ;
                توڑو;
            }
        }
        اگر (isID)
        {
            deviceID = atoi(deviceName.c_str());
            اگر (deviceID < 0)
                واپس تہی;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint عدد_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, تہی, &عدد_Platforms));

        اگر (عدد_Platforms == 0)
            واپس تہی;
        platforms.resize((size_t)عدد_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(عدد_Platforms, &platforms[0], &عدد_Platforms));
        platforms.resize(عدد_Platforms);
    }

    اگر (platform.length() > 0)
    {
        برائے (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            اگر (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            ورنہ
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        اگر (platforms.size() == 0)
        {
            CV_LOG_ERROR(تہی, "OpenCL: Can't find OpenCL platform by name: " << platform);
            جاؤ not_ملا;
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
        ورنہ
            deviceTypes.push_back("ALL");
    }
    برائے (size_t t = 0; t < deviceTypes.size(); t++)
    {
        صحیح deviceType = 0;
        std::string عارضی_Str_Device_Type = deviceTypes[t];
        std::transform(عارضی_Str_Device_Type.begin(), عارضی_Str_Device_Type.end(), عارضی_Str_Device_Type.begin(), details::char_tolower);

        اگر (عارضی_Str_Device_Type == "gpu" || عارضی_Str_Device_Type == "dgpu" || عارضی_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        ورنہ اگر (عارضی_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        ورنہ اگر (عارضی_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        ورنہ اگر (عارضی_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        ورنہ
        {
            CV_LOG_ERROR(تہی, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            جاؤ not_ملا;
        }

        std::vector<cl_device_id> devices;
        برائے (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint گنتی_2 = 0;
            cl_int حالت_2 = clGetDeviceIDs(*currentPlatform, deviceType, 0, تہی, &گنتی_2);
            اگر (!(حالت_2 == CL_SUCCESS || حالت_2 == CL_DEVICE_NOT_ملا))
            {
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, "clGetDeviceIDs get count");
            }
            اگر (گنتی_2 == 0)
                جاری;
            size_t بنیاد = devices.size();
            devices.resize(بنیاد + گنتی_2);
            حالت_2 = clGetDeviceIDs(*currentPlatform, deviceType, گنتی_2, &devices[بنیاد], &گنتی_2);
            اگر (!(حالت_2 == CL_SUCCESS || حالت_2 == CL_DEVICE_NOT_ملا))
            {
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, "clGetDeviceIDs get IDs");
            }
        }

        برائے (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : سچ) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = سچ;
            اگر(عارضی_Str_Device_Type == "dgpu" || عارضی_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_حافظہ, حجم(isIGPU), &isIGPU, تہی));
                useGPU = عارضی_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            اگر ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                واپس devices[i];
            }
        }
    }

not_ملا:
    اگر (configuration.empty())
        واپس تہی; // suppress messages on stderr

    std::ostringstream پیغام;
    پیغام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::سطر_ختم
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::سطر_ختم
        << "    Device types:";
    برائے (size_t t = 0; t < deviceTypes.size(); t++)
        پیغام << ' ' << deviceTypes[t];

    پیغام << std::سطر_ختم << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(تہی, پیغام.str());
    واپس تہی;
}

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
نامخلا svm {

شمار AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_بفر = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_بفر = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_بفر_نقاب = 3 << 16,
        OPENCL_SVM_بفر_MAP = 4 << 16
};

ساکن منطقی checkForceSVMUmatUsage()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی force = جھوٹ;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", جھوٹ);
        initialized = سچ;
    }
    واپس force;
}
ساکن منطقی checkDisableSVMUMatUsage()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی force = جھوٹ;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", جھوٹ);
        initialized = سچ;
    }
    واپس force;
}
ساکن منطقی checkDisableSVM()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی force = جھوٹ;
    اگر (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", جھوٹ);
        initialized = سچ;
    }
    واپس force;
}
// see SVMCapabilities
ساکن غیرعلامتی صحیح get_SVMCapabilities_نقاب()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن غیرعلامتی صحیح نقاب = 0;
    اگر (!initialized)
    {
        ثابت std::string env_قدر = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        اگر (env_قدر.empty())
        {
            واپس ~0U; // all bits 1
        }
        نقاب = atoi(env_قدر.c_str());
        initialized = سچ;
    }
    واپس نقاب;
}
} // namespace
#اگر_ختم

ساکن size_t get_پروگرام_گنتی_حد_بندی()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن size_t گنتی_2 = 0;
    اگر (!initialized)
    {
        گنتی_2 = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = سچ;
    }
    واپس گنتی_2;
}

ساکن صحیح g_contextId = 0;

جماعت OpenCLBufferPoolImpl;
جماعت OpenCLSVMBufferPoolImpl;

ساخت Context::Impl
{
    ساکن Context::Impl* get(Context& context) { واپس context.getImpl(); }

    قسمتعریف std::deque<Context::Impl*> container_t;
    ساکن container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        ساکن container_t* g_contexts = نیا container_t();
        واپس *g_contexts;
    }

محفوظ:
    Impl(ثابت std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        , svmInitialized(جھوٹ)
#اگر_ختم
    {
        اگر (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_تالا lock(cv::getInitializationMutex());
        خودکار& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = یہ;
    }

    ~Impl()
    {
#اگر_تعریف_شدہ _WIN32
        اگر (!cv::__termination)
#اگر_ختم
        {
            اگر (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = تہی;
            }
            devices.clear();
        }

        userContextStorage.clear();

        {
            cv::Auto_تالا lock(cv::getInitializationMutex());
            خودکار& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = تہی;
        }
    }

    خالی init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_عدد_DEVICES, حجم(ndevices), &ndevices, تہی));
        CV_Assert(ndevices > 0);

        cv::Auto_بفر<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_سائز = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_سائز));
        CV_CheckEQ(devices_ret_سائز, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        برائے (غیرعلامتی i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    خالی __init_buffer_pools();  // w/o synchronization
    خالی init_بفر_pools() ثابت
    {
        اگر (!بفر_Pool_2)
        {
            cv::Auto_تالا lock(cv::getInitializationMutex());
            اگر (!بفر_Pool_2)
            {
                const_cast<Impl*>(یہ)->__init_buffer_pools();
            }
        }
    }
عوامی:
    ساکن Impl* ڈھونڈو_Context(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_تالا lock(cv::getInitializationMutex());
        خودکار& container = getGlobalContainer();
        اگر (configuration.empty() && !container.empty())
            واپس container[0];
        برائے (خودکار it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            اگر (i && i->configuration == configuration)
            {
                واپس i;
            }
        }
        واپس تہی;
    }

    ساکن Impl* ڈھونڈو_Or_Create_Context(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        اگر (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            اگر (!c.empty())
                configuration = c;
        }
        Impl* impl = ڈھونڈو_Context(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(تہی, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            واپس impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        اگر (d == تہی)
            واپس تہی;

        impl = نیا Impl(configuration);
        کوشش
        {
            impl->createFromDevice(d);
            اگر (impl->handle)
                واپس impl;
            حذف impl;
            واپس تہی;
        }
        پکڑو (...)
        {
            حذف impl;
            پھینکو;
        }
    }

    ساکن Impl* ڈھونڈو_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (خالی*)h);
        Impl* impl = ڈھونڈو_Context(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(تہی, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            واپس impl;
        }

        impl = نیا Impl(configuration);
        کوشش
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            واپس impl;
        }
        پکڑو (...)
        {
            حذف impl;
            پھینکو;
        }
    }

    ساکن Impl* ڈھونڈو_Or_Create_Context(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (خالی*)d);
        Impl* impl = ڈھونڈو_Context(configuration);
        اگر (impl)
        {
            CV_LOG_INFO(تہی, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            واپس impl;
        }

        impl = نیا Impl(configuration);
        کوشش
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            واپس impl;
        }
        پکڑو (...)
        {
            حذف impl;
            پھینکو;
        }
    }

    خالی سیٹ_Default()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        اگر (d == تہی)
            واپس;

        createFromDevice(d);
    }

    خالی createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == تہی);

        cl_platform_id pl = تہی;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, حجم(cl_platform_id), &pl, تہی));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int حالت_2;

        handle = clCreateContext(prop, nd, &d, 0, 0, &حالت_2);
        CV_OCL_DBG_CHECK_نتیجہ(حالت_2, "clCreateContext");

        منطقی ok = handle != 0 && حالت_2 == CL_SUCCESS;
        اگر( ok )
        {
            devices.resize(nd);
            devices[0].سیٹ(d);
        }
        ورنہ
            handle = تہی;
    }

    پروگرام_2 getProg(ثابت پروگرام_ماخذ& src, ثابت String& buildflags, String& errmsg);

    خالی unloadProg(پروگرام_2& prog)
    {
        cv::Auto_تالا lock(پروگرام_cache_mutex);
        برائے (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              اگر (it != phash.end())
              {
                  اگر (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      واپس;
                  }
              }
        }
    }

    std::string& get_سابقہ_String()
    {
        اگر (سابقہ.empty())
        {
            cv::Auto_تالا lock(پروگرام_cache_mutex);
            اگر (سابقہ.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحیح بٹ_2 = d.پتہ_بٹ();
                اگر (بٹ_2 > 0 && بٹ_2 != 64)
                    سابقہ = cv::format("%d-bit--", بٹ_2);
                سابقہ += d.vendorName() + "--" + d.name() + "--" + d.driver_نسخہ();
                // sanitize chars
                برائے (size_t i = 0; i < سابقہ.size(); i++)
                {
                    حرف c = سابقہ[i];
                    اگر (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        سابقہ[i] = '_';
                    }
                }
            }
        }
        واپس سابقہ;
    }

    std::string& get_سابقہ_بنیاد()
    {
        اگر (سابقہ_بنیاد.empty())
        {
            cv::Auto_تالا lock(پروگرام_cache_mutex);
            اگر (سابقہ_بنیاد.empty())
            {
                ثابت Device& d = devices[0];
                صحیح بٹ_2 = d.پتہ_بٹ();
                اگر (بٹ_2 > 0 && بٹ_2 != 64)
                    سابقہ_بنیاد = cv::format("%d-bit--", بٹ_2);
                سابقہ_بنیاد += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                برائے (size_t i = 0; i < سابقہ_بنیاد.size(); i++)
                {
                    حرف c = سابقہ_بنیاد[i];
                    اگر (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        سابقہ_بنیاد[i] = '_';
                    }
                }
            }
        }
        واپس سابقہ_بنیاد;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحیح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string سابقہ;
    std::string سابقہ_بنیاد;

    cv::Mutex پروگرام_cache_mutex;
    قسمتعریف std::map<std::string, پروگرام_2> phash_t;
    phash_t phash;
    قسمتعریف std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> بفر_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> بفر_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_بفر_Pool() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_2);
        واپس *بفر_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_بفر_Pool_Host_Ptr() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_Host_Ptr);
        واپس *بفر_Pool_Host_Ptr.get();
    }

    std::map<std::type_اشاریہ, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    خالی سیٹ_User_Context(std::type_اشاریہ typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::Auto_تالا lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_اشاریہ typeId) {
        cv::Auto_تالا lock(userContextMutex);
        خودکار it = userContextStorage.find(typeId);
        اگر (it != userContextStorage.end())
            واپس it->second;
        ورنہ
            واپس خالی_اشاریہ;
    }

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
    منطقی svmInitialized;
    منطقی svm_دستیاب;
    منطقی svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    خالی svmInit()
    {
        CV_Assert(handle != تہی);
        ثابت Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((خالی)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int حالت_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, حجم(deviceCaps), &deviceCaps, تہی);
        اگر (حالت_2 != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", حالت_2);
            جاؤ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (صحیح)deviceCaps);
        CV_Assert(((خالی)0, CL_DEVICE_SVM_COARSE_GRAIN_بفر == CL_DEVICE_SVM_COARSE_GRAIN_بفر_AMD)); // Check assumption
        svmCapabilities.قدر_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_FINE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.قدر_2 &= svm::get_SVMCapabilities_نقاب();
        اگر (svmCapabilities.قدر_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            جاؤ noSVM;
        }
        کوشش
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            خالی* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            اگر (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            کوشش
            {
                منطقی error = جھوٹ;
                cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
                اگر (CL_SUCCESS != cl_قطار_میں_ڈالو_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, تہی, تہی))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_اختتام(q);
                کوشش
                {
                    ((صحیح*)ptr)[0] = 100;
                }
                پکڑو (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = سچ;
                }
                اگر (CL_SUCCESS != cl_قطار_میں_ڈالو_SVMUnmap(q, ptr, 0, تہی, تہی))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_اختتام(q);
                اگر (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            پکڑو (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                پھینکو;
            }
            clSVMFree(handle, ptr);
            svmFunctions.fn_clSVMAlloc = clSVMAlloc;
            svmFunctions.fn_clSVMFree = clSVMFree;
            svmFunctions.fn_cl_سیٹ_Kernel_Arg_SVMPointer = cl_سیٹ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMemcpy = cl_قطار_میں_ڈالو_SVMMemcpy;
            svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMem_Fill = cl_قطار_میں_ڈالو_SVMMem_Fill;
            svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMap = cl_قطار_میں_ڈالو_SVMMap;
            svmFunctions.fn_cl_قطار_میں_ڈالو_SVMUnmap = cl_قطار_میں_ڈالو_SVMUnmap;
        }
        پکڑو (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            کوشش
            {
                // Try HSA extension
                String extensions = device.extensions();
                اگر (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    جاؤ noSVM;
                }
                cl_platform_id p = تہی;
                CV_OCL_CHECK(حالت_2 = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, حجم(cl_platform_id), &p, تہی));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_سیٹ_Kernel_Arg_SVMPointer = (cl_سیٹ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMemcpy = (cl_قطار_میں_ڈالو_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMem_Fill = (cl_قطار_میں_ڈالو_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_قطار_میں_ڈالو_SVMMap = (cl_قطار_میں_ڈالو_SVMMap_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_قطار_میں_ڈالو_SVMUnmap = (cl_قطار_میں_ڈالو_SVMUnmap_AMD_fn)cl_Get_Extension_Function_پتہ_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            پکڑو (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                جاؤ noSVM;
            }
        }

        svm_دستیاب = سچ;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = سچ;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        واپس;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_دستیاب = جھوٹ;
        svmEnabled = جھوٹ;
        svmCapabilities.قدر_2 = 0;
        svmInitialized = سچ;
        svmFunctions.fn_clSVMAlloc = تہی;
        واپس;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> بفر_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_بفر_Pool_SVM() ثابت
    {
        init_بفر_pools();
        CV_DbgAssert(بفر_Pool_SVM);
        واپس *بفر_Pool_SVM.get();
    }
#اگر_ختم

    دوست جماعت پروگرام_2;
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

خالی Context::release()
{
    اگر (p)
    {
        p->release();
        p = تہی;
    }
}

منطقی Context::create()
{
    release();
    اگر (!haveOpenCL())
        واپس جھوٹ;
    p = Impl::ڈھونڈو_Or_Create_Context(std::string());
    اگر (p && p->handle)
        واپس سچ;
    release();
    واپس جھوٹ;
}

// deprecated
منطقی Context::create(صحیح dtype)
{
    اگر( !haveOpenCL() )
        واپس جھوٹ;
    release();
    اگر (dtype == CL_DEVICE_TYPE_DEFAULT || (غیرعلامتی)dtype == (غیرعلامتی)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::ڈھونڈو_Or_Create_Context("");
    }
    ورنہ اگر (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::ڈھونڈو_Or_Create_Context(":GPU:");
    }
    ورنہ اگر (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::ڈھونڈو_Or_Create_Context(":CPU:");
    }
    ورنہ
    {
        CV_LOG_ERROR(تہی, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    اگر (p && !p->handle)
    {
        release();
    }
    واپس p != 0;
}

Context::Context(ثابت Context& c)
{
    p = (Impl*)c.p;
    اگر(p)
        p->addref();
}

Context& Context::عامل = (ثابت Context& c)
{
    Impl* newp = (Impl*)c.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = خالی_اشاریہ;
}

Context& Context::عامل = (Context&& c) CV_NOEXCEPT
{
    اگر (یہ != &c) {
        اگر(p)
            p->release();
        p = c.p;
        c.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

خالی* Context::ptr() ثابت
{
    واپس p == تہی ? تہی : p->handle;
}

size_t Context::ndevices() ثابت
{
    واپس p ? p->devices.size() : 0;
}

Device& Context::device(size_t idx) ثابت
{
    ساکن Device dummy;
    واپس !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    خودکار& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        خودکار& ctx = c.getContext();
        واپس ctx;
    }

    ساکن Context dummy;
    واپس dummy;
}

پروگرام_2 Context::getProg(ثابت پروگرام_ماخذ& prog,
                         ثابت String& buildopts, String& errmsg)
{
    واپس p ? p->getProg(prog, buildopts, errmsg) : پروگرام_2();
}

خالی Context::unloadProg(پروگرام_2& prog)
{
    اگر (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(خالی* context)
{
    Context ctx;
    ctx.p = Impl::ڈھونڈو_Or_Create_Context((cl_context)context);
    واپس ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::ڈھونڈو_Or_Create_Context(device);
    واپس ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::ڈھونڈو_Or_Create_Context(configuration);
    واپس ctx;
}

خالی* Context::getOpenCLContextProperty(صحیح propertyId) ثابت
{
    اگر (p == تہی)
        واپس خالی_اشاریہ;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, تہی, &size));
    std::vector<cl_context_properties> prop(size / حجم(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.ڈیٹا(), تہی));
    برائے (size_t i = 0; i < prop.size(); i += 2)
    {
        اگر (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(تہی, "OpenCL: found context property=" << propertyId << ") => " << (خالی*)prop[i + 1]);
            واپس (خالی*)prop[i + 1];
        }
    }
    واپس خالی_اشاریہ;
}

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
منطقی Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    واپس i->svmEnabled;
}
خالی Context::سیٹ_Use_SVM(منطقی enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    اگر (enabled && !i->svm_دستیاب)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#ورنہ
منطقی Context::useSVM() ثابت { واپس جھوٹ; }
خالی Context::سیٹ_Use_SVM(منطقی enabled) { CV_Assert(!enabled); }
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
نامخلا svm {

ثابت SVMCapabilities getSVMCapabilitites(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    اگر (!i->svmInitialized)
        i->svmInit();
    واپس i->svmCapabilities;
}

CV_EXPORTS ثابت SVMFunctions* getSVMFunctions(ثابت ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != تہی);
    واپس &i->svmFunctions;
}

CV_EXPORTS منطقی useSVM(UMatUsageFlags usageFlags)
{
    اگر (checkForceSVMUmatUsage())
        واپس سچ;
    اگر (checkDisableSVMUMatUsage())
        واپس جھوٹ;
    اگر ((usageFlags & USAGE_ALLOCATE_SHARED_حافظہ) != 0)
        واپس سچ;
    واپس جھوٹ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#اگر_ختم // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

خالی Context::سیٹ_User_Context(std::type_اشاریہ typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->سیٹ_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_اشاریہ typeId)
{
    CV_Assert(p);
    واپس p->getUserContext(typeId);
}

ساکن خالی get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    size_t sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_بفر<حرف> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.ڈیٹا(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.ڈیٹا();
}

/*
// Attaches OpenCL context to OpenCV
*/
خالی attachContext(ثابت String& platformName, خالی* platformID, خالی* context, خالی* deviceID)
{
    خودکار ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        ثابت std::string& platformName, خالی* platformID, خالی* context, خالی* deviceID
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

    منطقی platform_دستیاب = جھوٹ;

    // check if external platformName contained in list of available platforms in OpenCV
    برائے (غیرعلامتی صحیح i = 0; i < cnt; i++)
    {
        String دستیاب_Platform_Name;
        get_platform_name(platforms[i], دستیاب_Platform_Name);
        // external platform is found in the list of available platforms
        اگر (platformName == دستیاب_Platform_Name)
        {
            platform_دستیاب = سچ;
            توڑو;
        }
    }

    اگر (!platform_دستیاب)
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
    واپس ctx;
}

خالی initializeContextFromHandle(Context& ctx, خالی* _platform, خالی* _context, خالی* _device)
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

ساخت قطار::Impl
{
    اندرونی خالی __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_قطار = جھوٹ;
    }

    Impl(cl_command_قطار q)
    {
        __init();
        handle = q;

        cl_command_قطار_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_PROPERTIES, حجم(cl_command_قطار_properties), &props, تہی));
        is_Profiling_قطار = !!(props & CL_قطار_PROFILING_ENABLE);
    }

    Impl(cl_command_قطار q, منطقی is_Profiling_قطار_2)
    {
        __init();
        handle = q;
        is_Profiling_قطار = is_Profiling_قطار_2;
    }

    Impl(ثابت Context& c, ثابت Device& d, منطقی withProfiling = جھوٹ)
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
        cl_command_قطار_properties props = withProfiling ? CL_قطار_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_قطار(ch, dh, props, &retval), retval);
        is_Profiling_قطار = withProfiling;
    }

    ~Impl()
    {
#اگر_تعریف_شدہ _WIN32
        اگر (!cv::__termination)
#اگر_ختم
        {
            اگر(handle)
            {
                CV_OCL_DBG_CHECK(cl_اختتام(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_قطار(handle));
                handle = تہی;
            }
        }
    }

    ثابت cv::ocl::قطار& get_Profiling_قطار(ثابت cv::ocl::قطار& self)
    {
        اگر (is_Profiling_قطار)
            واپس self;

        اگر (profiling_قطار.ptr())
            واپس profiling_قطار;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_CONTEXT, حجم(cl_context), &ctx, تہی));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_قطار_Info(handle, CL_قطار_DEVICE, حجم(cl_device_id), &device, تہی));

        cl_int نتیجہ = CL_SUCCESS;
        cl_command_قطار_properties props = CL_قطار_PROFILING_ENABLE;
        cl_command_قطار q = cl_Create_Command_قطار(ctx, device, props, &نتیجہ);
        CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        قطار قطار_2;
        قطار_2.p = نیا Impl(q, سچ);
        profiling_قطار = قطار_2;

        واپس profiling_قطار;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_قطار handle;
    منطقی is_Profiling_قطار;
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
    اگر(p)
        p->addref();
}

قطار& قطار::عامل = (ثابت قطار& q)
{
    Impl* newp = (Impl*)q.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

قطار::قطار(قطار&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = خالی_اشاریہ;
}

قطار& قطار::عامل = (قطار&& q) CV_NOEXCEPT
{
    اگر (یہ != &q) {
        اگر(p)
            p->release();
        p = q.p;
        q.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

قطار::~قطار()
{
    اگر(p)
        p->release();
}

منطقی قطار::create(ثابت Context& c, ثابت Device& d)
{
    اگر(p)
        p->release();
    p = نیا Impl(c, d);
    واپس p->handle != 0;
}

خالی قطار::اختتام_2()
{
    اگر(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_اختتام(p->handle));
    }
}

ثابت قطار& قطار::get_Profiling_قطار() ثابت
{
    CV_Assert(p);
    واپس p->get_Profiling_قطار(*یہ);
}

خالی* قطار::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

قطار& قطار::getDefault()
{
    خودکار& c = OpenCLExecutionContext::getCurrent();
    اگر (!c.empty())
    {
        خودکار& q = c.get_قطار();
        واپس q;
    }
    ساکن قطار dummy;
    واپس dummy;
}

ساکن cl_command_قطار get_قطار(ثابت قطار& q)
{
    cl_command_قطار qq = (cl_command_قطار)q.ptr();
    اگر(!qq)
        qq = (cl_command_قطار)قطار::getDefault().ptr();
    واپس qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(صحیح _flags, UMat* _m, صحیح _wscale, صحیح _iwscale, ثابت خالی* _obj, size_t _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != تہی);
}

KernelArg KernelArg::Constant(ثابت Mat& m)
{
    CV_Assert(m.isContinuous());
    واپس KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.کل()*m.elem_سائز());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

ساخت Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت پروگرام_2& prog) :
        refcount(1), handle(تہی), is_اندر_پیش_رفت(جھوٹ), is_Async_تسلسل(جھوٹ), nu(0)
    {
        cl_پروگرام ph = (cl_پروگرام)prog.ptr();
        cl_int retval = 0;
        name = kname;
        اگر (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        برائے( صحیح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_عارضی_Dst_UMats = جھوٹ;
        have_عارضی_Src_UMats = جھوٹ;
    }

    خالی cleanupUMats()
    {
        منطقی exceptionOccurred = جھوٹ;
        برائے( صحیح i = 0; i < MAX_ARRS; i++ )
        {
            اگر( u[i] )
            {
                اگر( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_ڈیٹا::ASYNC_CLEANUP;
                    کوشش
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    پکڑو(ثابت std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(تہی, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = سچ;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_عارضی_Dst_UMats = جھوٹ;
        have_عارضی_Src_UMats = جھوٹ;
        CV_Assert(!exceptionOccurred);
    }

    خالی جوڑو_UMat(ثابت UMat& m, منطقی dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        اگر(dst && m.u->عارضی_UMat())
            have_عارضی_Dst_UMats = سچ;
        اگر(m.u->original_UMat_ڈیٹا == تہی && m.u->عارضی_UMat())
            have_عارضی_Src_UMats = سچ;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    خالی رجسٹر_Image_دلیل(صحیح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        اگر (arg < (صحیح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_اندر_پیش_رفت, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    خالی finit(cl_event e)
    {
        CV_غیر_مستعمل(e);
        is_اندر_پیش_رفت = جھوٹ;
        کوشش
        {
            cleanupUMats();
        }
        پکڑو(...)
        {
            release();
            پھینکو;
        }
        release();
    }

    منطقی تسلسل(صحیح dims, size_t _globalsize[], size_t _localsize[],
            منطقی ہمآہنگ, int64* وقت_NS, ثابت قطار& q);

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
    شمار { MAX_ARRS = 16 };
    UMat_ڈیٹا* u[MAX_ARRS];
    منطقی is_اندر_پیش_رفت;
    منطقی is_Async_تسلسل;  // true if kernel was scheduled in async mode
    صحیح nu;
    std::vector<Image2D> shadow_images;
    منطقی have_عارضی_Dst_UMats;
    منطقی have_عارضی_Src_UMats;
};

}} // namespace cv::ocl

خارجی "C" {

ساکن خالی CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, خالی *p)
{
    کوشش
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    پکڑو (ثابت cv::Exception& exc)
    {
        CV_LOG_ERROR(تہی, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    پکڑو (ثابت std::exception& exc)
    {
        CV_LOG_ERROR(تہی, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    پکڑو (...)
    {
        CV_LOG_ERROR(تہی, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

نامخلا cv { نامخلا ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت حرف* kname, ثابت پروگرام_ماخذ& src,
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

Kernel& Kernel::عامل = (ثابت Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = خالی_اشاریہ;
}

Kernel& Kernel::عامل = (Kernel&& k) CV_NOEXCEPT
{
    اگر (یہ != &k) {
        اگر(p)
            p->release();
        p = k.p;
        k.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

Kernel::~Kernel()
{
    اگر(p)
        p->release();
}

منطقی Kernel::create(ثابت حرف* kname, ثابت پروگرام_2& prog)
{
    اگر(p)
        p->release();
    p = نیا Impl(kname, prog);
    اگر(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#اگر_تعریف_شدہ CV_OPENCL_تسلسل_ASSERT // check kernel compilation fails
    CV_Assert(p);
#اگر_ختم
    واپس p != 0;
}

منطقی Kernel::create(ثابت حرف* kname, ثابت پروگرام_ماخذ& src,
                    ثابت String& buildopts, String* errmsg)
{
    اگر(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    اگر( !errmsg ) errmsg = &tempmsg;
    ثابت پروگرام_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    واپس create(kname, prog);
}

خالی* Kernel::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

منطقی Kernel::empty() ثابت
{
    واپس ptr() == 0;
}

ساکن cv::String dump_قدر(size_t sz, ثابت خالی* p)
{
    اگر (!p)
        واپس "NULL";
    اگر (sz == 2)
        واپس cv::format("%d / %uu / 0x%04x", *(مختصر*)p, *(غیرعلامتی مختصر*)p, *(مختصر*)p);
    اگر (sz == 4)
        واپس cv::format("%d / %uu / 0x%08x / %g", *(صحیح*)p, *(صحیح*)p, *(صحیح*)p, *(اعشاری*)p);
    اگر (sz == 8)
        واپس cv::format("%lld / %lluu / 0x%16llx / %g", *(طویل طویل*)p, *(طویل طویل*)p, *(طویل طویل*)p, *(دوہرا*)p);
    واپس cv::format("%p", p);
}

صحیح Kernel::سیٹ(صحیح i, ثابت خالی* value, size_t sz)
{
    اگر (!p || !p->handle)
        واپس -1;
    اگر (i < 0)
        واپس i;
    اگر( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحیح)i, (صحیح)sz, dump_قدر(sz, value).c_str()).c_str());
    اگر (retval != CL_SUCCESS)
        واپس -1;
    واپس i+1;
}

صحیح Kernel::سیٹ(صحیح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحیح res = سیٹ(i, &h, حجم(h));
    اگر (res >= 0)
        p->رجسٹر_Image_دلیل(i, image2D);
    واپس res;
}

صحیح Kernel::سیٹ(صحیح i, ثابت UMat& m)
{
    واپس سیٹ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساکن منطقی isCudaBacked(ثابت UMat_ڈیٹا* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    واپس u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحیح Kernel::سیٹ(صحیح i, ثابت KernelArg& arg)
{
    اگر( !p || !p->handle )
        واپس -1;
    اگر (i < 0)
    {
        CV_LOG_ERROR(تہی, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (صحیح)i));
        واپس i;
    }
    اگر( i == 0 )
        p->cleanupUMats();
    cl_int حالت_2 = 0;
    اگر( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        منطقی ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        اگر (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)تہی;
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحیح)i).c_str());
            واپس i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        اگر (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(تہی, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (صحیح)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            واپس -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        اگر (!h)
        {
            CV_LOG_ERROR(تہی, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (صحیح)i, (صحیح)arg.flags, arg.m));
            p->release();
            p = 0;
            واپس -1;
        }

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_ڈیٹا_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_ڈیٹا_Ptr);
#اگر 1 // TODO
            حالت_2 = svmFns->fn_cl_سیٹ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_ڈیٹا_Ptr);
#ورنہ
            حالت_2 = svmFns->fn_cl_سیٹ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_ڈیٹا_Ptr);
#اگر_ختم
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحیح)i, (خالی*)svm_ڈیٹا_Ptr).c_str());
        }
        ورنہ
#اگر_ختم
        {
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحیح)i, (خالی*)h).c_str());
        }

        اگر (ptronly)
        {
            i++;
        }
        ورنہ اگر( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u2d.قدم), &u2d.قدم);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u2d.قدم).c_str());
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)u2d.offset).c_str());
            i += 3;

            اگر( !(arg.flags & KernelArg::NO_سائز) )
            {
                صحیح ستون_تعداد = u2d.ستون_تعداد*arg.wscale/arg.iwscale;
                حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u2d.صفیں), &u2d.صفیں);
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحیح)i, (صحیح)u2d.صفیں).c_str());
                حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(ستون_تعداد), &ستون_تعداد);
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)ستون_تعداد).c_str());
                i += 2;
            }
        }
        ورنہ
        {
            UMat3D u3d(*arg.m);
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u3d.slicestep).c_str());
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.قدم), &u3d.قدم);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)u3d.قدم).c_str());
            حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحیح)(i+3), (صحیح)u3d.offset).c_str());
            i += 4;
            اگر( !(arg.flags & KernelArg::NO_سائز) )
            {
                صحیح ستون_تعداد = u3d.ستون_تعداد*arg.wscale/arg.iwscale;
                حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحیح)i, (صحیح)u3d.slices).c_str());
                حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.صفیں), &u3d.صفیں);
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحیح)(i+1), (صحیح)u3d.صفیں).c_str());
                حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.ستون_تعداد), &ستون_تعداد);
                CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحیح)(i+2), (صحیح)ستون_تعداد).c_str());
                i += 3;
            }
        }
        p->جوڑو_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        واپس i;
    }
    حالت_2 = cl_سیٹ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_نتیجہ(حالت_2, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحیح)i, (صحیح)arg.sz, (خالی*)arg.obj).c_str());
    واپس i+1;
}

منطقی Kernel::تسلسل(صحیح dims, size_t _globalsize[], size_t _localsize[],
                 منطقی ہمآہنگ, ثابت قطار& q)
{
    اگر (!p)
        واپس جھوٹ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t کل = 1;
    CV_Assert(_globalsize != تہی);
    برائے (صحیح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحیح)(i>0)) : 1;
        CV_Assert( val > 0 );
        کل *= _globalsize[i];
        اگر (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_اوپر(_globalsize[i], (غیرعلامتی صحیح)val) * val;
    }
    CV_Assert(کل > 0);

    واپس p->تسلسل(dims, globalsize, _localsize, ہمآہنگ, تہی, q);
}


منطقی Kernel::تسلسل_2(صحیح dims, size_t _globalsize[], size_t _localsize[],
                  منطقی ہمآہنگ, ثابت قطار& q)
{
    CV_Assert(p);
    واپس p->تسلسل(dims, _globalsize, _localsize, ہمآہنگ, تہی, q);
}


منطقی Kernel::Impl::تسلسل(صحیح dims, size_t globalsize[], size_t localsize[],
        منطقی ہمآہنگ, int64* وقت_NS, ثابت قطار& q)
{
    CV_INSTRUMENT_REGION_OPENCL_تسلسل(name.c_str());

    اگر (!handle)
    {
        CV_LOG_ERROR(تہی, "OpenCL kernel has zero handle: " << name);
        واپس جھوٹ;
    }

    اگر (is_Async_تسلسل)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_تسلسل = !ہمآہنگ;

    اگر (is_اندر_پیش_رفت)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#اگر CV_OPENCL_SYNC_تسلسل_KERNELS
    ہمآہنگ = سچ;
#اگر_ختم

    cl_command_قطار qq = get_قطار(q);
    اگر (have_عارضی_Dst_UMats)
        ہمآہنگ = سچ;
    اگر (have_عارضی_Src_UMats)
        ہمآہنگ = سچ;
    اگر (وقت_NS)
        ہمآہنگ = سچ;
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_میں_ڈالو_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           تہی, globalsize, localsize, 0, 0,
                                           (ہمآہنگ && !وقت_NS) ? 0 : &asyncEvent);
#اگر !CV_OPENCL_SHOW_تسلسل_KERNELS
    اگر (retval != CL_SUCCESS)
#اگر_ختم
    {
        cv::String پیغام = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحیح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        ہمآہنگ ? "true" : "false"
                        );
        اگر (retval != CL_SUCCESS)
        {
            پیغام = CV_OCL_API_ERROR_پیغام(retval, پیغام.c_str());
        }
#اگر CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_نتیجہ(retval, پیغام.c_str());
#ورنہ
        لکھو("%s\n", پیغام.c_str());
        fflush(stdout);
#اگر_ختم
    }
    اگر (ہمآہنگ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_اختتام(qq));
        اگر (وقت_NS)
        {
            اگر (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong آغاز_وقت, stop_وقت;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_آغاز, حجم(آغاز_وقت), &آغاز_وقت, تہی));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_انتہا, حجم(stop_وقت), &stop_وقت, تہی));
                *وقت_NS = (int64)(stop_وقت - آغاز_وقت);
            }
            ورنہ
            {
                *وقت_NS = -1;
            }
        }
        cleanupUMats();
    }
    ورنہ
    {
        addref();
        is_اندر_پیش_رفت = سچ;
        CV_OCL_CHECK(cl_سیٹ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, یہ));
    }
    اگر (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    واپس retval == CL_SUCCESS;
}

منطقی Kernel::تسلسل_Task(منطقی ہمآہنگ, ثابت قطار& q)
{
    اگر(!p || !p->handle || p->is_اندر_پیش_رفت)
        واپس جھوٹ;

    cl_command_قطار qq = get_قطار(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_قطار_میں_ڈالو_Task(qq, p->handle, 0, 0, ہمآہنگ ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), ہمآہنگ ? "true" : "false").c_str());
    اگر (ہمآہنگ || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_اختتام(qq));
        p->cleanupUMats();
    }
    ورنہ
    {
        p->addref();
        p->is_اندر_پیش_رفت = سچ;
        CV_OCL_CHECK(cl_سیٹ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    اگر (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    واپس retval == CL_SUCCESS;
}

int64 Kernel::تسلسل_Profiling(صحیح dims, size_t globalsize[], size_t localsize[], ثابت قطار& q_)
{
    CV_Assert(p && p->handle && !p->is_اندر_پیش_رفت);
    قطار q = q_.ptr() ? q_ : قطار::getDefault();
    CV_Assert(q.ptr());
    q.اختتام_2(); // call clFinish() on base queue
    قطار profiling_قطار_2 = q.get_Profiling_قطار();
    int64 وقت_Ns = -1;
    منطقی res = p->تسلسل(dims, globalsize, localsize, سچ, &وقت_Ns, profiling_قطار_2);
    واپس res ? وقت_Ns : -1;
}

size_t Kernel::work_گروہ_سائز() ثابت
{
    اگر(!p || !p->handle)
        واپس 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت_2 = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_WORK_گروہ_سائز, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت_2, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    واپس حالت_2 == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_گروہ_سائز_مضرب() ثابت
{
    اگر(!p || !p->handle)
        واپس 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت_2 = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_گروہ_سائز_مضرب, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت_2, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    واپس حالت_2 == CL_SUCCESS ? val : 0;
}

منطقی Kernel::compile_Work_گروہ_سائز(size_t wsz[]) ثابت
{
    اگر(!p || !p->handle || !wsz)
        واپس 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت_2 = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_گروہ_سائز, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت_2, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    واپس حالت_2 == CL_SUCCESS;
}

size_t Kernel::local_Mem_سائز() ثابت
{
    اگر(!p || !p->handle)
        واپس 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت_2 = cl_Get_Kernel_Work_گروہ_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_سائز, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجہ(حالت_2, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    واپس حالت_2 == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

ساخت پروگرام_ماخذ::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    شمار قسم_4 {
        پروگرام_ماخذ_رمز = 0,
        پروگرام_BINARIES,
        پروگرام_SPIR,
        پروگرام_SPIRV
    } قسم_5;

    Impl(ثابت String& src)
    {
        init(پروگرام_ماخذ_رمز, cv::String(), cv::String());
        init_From_ماخذ(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& رمز_Str, ثابت String& رمز_Hash)
    {
        init(پروگرام_ماخذ_رمز, module, name);
        init_From_ماخذ(رمز_Str, رمز_Hash);
    }

    /// reset fields
    خالی init(شمار قسم_4 قسم, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        قسم_5 = قسم;
        module_ = module;
        name_ = name;

        ماخذ_Addr = تہی;
        ماخذ_سائز = 0;
        isHashUpdated = جھوٹ;
    }

    خالی init_From_ماخذ(ثابت String& رمز_Str, ثابت String& رمز_Hash)
    {
        رمز_Str_2 = رمز_Str;
        ماخذ_Hash = رمز_Hash;
        اگر (ماخذ_Hash.empty())
        {
            updateHash();
        }
        ورنہ
        {
            isHashUpdated = سچ;
        }
    }

    خالی updateHash(ثابت حرف* hashStr = تہی)
    {
        اگر (hashStr)
        {
            ماخذ_Hash = cv::String(hashStr);
            isHashUpdated = سچ;
            واپس;
        }
        uint64 hash = 0;
        انتخاب (قسم_5)
        {
        حالت پروگرام_ماخذ_رمز:
            اگر (ماخذ_Addr)
            {
                CV_Assert(رمز_Str_2.empty());
                hash = crc64(ماخذ_Addr, ماخذ_سائز); // static storage
            }
            ورنہ
            {
                CV_Assert(!رمز_Str_2.empty());
                hash = crc64((uchar*)رمز_Str_2.c_str(), رمز_Str_2.size());
            }
            توڑو;
        حالت پروگرام_BINARIES:
        حالت پروگرام_SPIR:
        حالت پروگرام_SPIRV:
            hash = crc64(ماخذ_Addr, ماخذ_سائز);
            توڑو;
        طےشدہ:
            CV_Error(Error::Sts_اندرونی, "Internal error");
        }
        ماخذ_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = سچ;
    }

    Impl(شمار قسم_4 قسم,
            ثابت String& module, ثابت String& name,
            ثابت غیرعلامتی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(قسم, module, name);

        ماخذ_Addr = binary;
        ماخذ_سائز = size;

        buildOptions_ = buildOptions;
    }

    ساکن پروگرام_ماخذ from_ماخذ_With_Static_Lifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* ماخذ_رمز_Static_Str, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ماخذ نتیجہ;
        نتیجہ.p = نیا Impl(پروگرام_ماخذ_رمز, module, name,
                (ثابت غیرعلامتی حرف*)ماخذ_رمز_Static_Str, لڑی_لمبائی(ماخذ_رمز_Static_Str), buildOptions);
        نتیجہ.p->updateHash(hashStaticStr);
        واپس نتیجہ;
    }

    ساکن پروگرام_ماخذ from_ثنائی(ثابت String& module, ثابت String& name,
            ثابت غیرعلامتی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ماخذ نتیجہ;
        نتیجہ.p = نیا Impl(پروگرام_BINARIES, module, name, binary, size, buildOptions);
        واپس نتیجہ;
    }

    ساکن پروگرام_ماخذ fromSPIR(ثابت String& module, ثابت String& name,
            ثابت غیرعلامتی حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروگرام_ماخذ نتیجہ;
        نتیجہ.p = نیا Impl(پروگرام_SPIR, module, name, binary, size, buildOptions);
        واپس نتیجہ;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String رمز_Str_2; // PROGRAM_SOURCE_CODE only

    ثابت غیرعلامتی حرف* ماخذ_Addr;
    size_t ماخذ_سائز;

    cv::String buildOptions_;

    String ماخذ_Hash;
    منطقی isHashUpdated;

    دوست ساخت پروگرام_2::Impl;
    دوست ساخت internal::پروگرام_اندراج;
    دوست ساخت Context::Impl;
};


پروگرام_ماخذ::پروگرام_ماخذ() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_ماخذ::پروگرام_ماخذ(ثابت String& module, ثابت String& name, ثابت String& رمز_Str, ثابت String& رمز_Hash)
{
    p = نیا Impl(module, name, رمز_Str, رمز_Hash);
}

پروگرام_ماخذ::پروگرام_ماخذ(ثابت حرف* prog)
{
    p = نیا Impl(prog);
}

پروگرام_ماخذ::پروگرام_ماخذ(ثابت String& prog)
{
    p = نیا Impl(prog);
}

پروگرام_ماخذ::~پروگرام_ماخذ()
{
    اگر(p)
        p->release();
}

پروگرام_ماخذ::پروگرام_ماخذ(ثابت پروگرام_ماخذ& prog)
{
    p = prog.p;
    اگر(p)
        p->addref();
}

پروگرام_ماخذ& پروگرام_ماخذ::عامل = (ثابت پروگرام_ماخذ& prog)
{
    Impl* newp = (Impl*)prog.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

پروگرام_ماخذ::پروگرام_ماخذ(پروگرام_ماخذ&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = خالی_اشاریہ;
}

پروگرام_ماخذ& پروگرام_ماخذ::عامل = (پروگرام_ماخذ&& prog) CV_NOEXCEPT
{
    اگر (یہ != &prog) {
        اگر(p)
            p->release();
        p = prog.p;
        prog.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

ثابت String& پروگرام_ماخذ::ماخذ() ثابت
{
    CV_Assert(p);
    CV_Assert(p->قسم_5 == Impl::پروگرام_ماخذ_رمز);
    CV_Assert(p->ماخذ_Addr == تہی); // method returns reference - can't construct temporary object
    واپس p->رمز_Str_2;
}

پروگرام_ماخذ::hash_t پروگرام_ماخذ::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

پروگرام_ماخذ پروگرام_ماخذ::from_ثنائی(ثابت String& module, ثابت String& name,
        ثابت غیرعلامتی حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    واپس Impl::from_ثنائی(module, name, binary, size, buildOptions);
}

پروگرام_ماخذ پروگرام_ماخذ::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت غیرعلامتی حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    واپس Impl::from_ثنائی(module, name, binary, size, buildOptions);
}


internal::پروگرام_اندراج::عامل پروگرام_ماخذ&() ثابت
{
    اگر (یہ->p_پروگرام_ماخذ == تہی)
    {
        cv::Auto_تالا lock(cv::getInitializationMutex());
        اگر (یہ->p_پروگرام_ماخذ == تہی)
        {
            پروگرام_ماخذ ps = پروگرام_ماخذ::Impl::from_ماخذ_With_Static_Lifetime(یہ->module, یہ->name, یہ->پروگرام_رمز, یہ->پروگرام_Hash, cv::String());
            پروگرام_ماخذ* ptr = نیا پروگرام_ماخذ(ps);
            const_cast<پروگرام_اندراج*>(یہ)->p_پروگرام_ماخذ = ptr;
        }
    }
    واپس *یہ->p_پروگرام_ماخذ;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

ساکن
cv::String joinBuildOptions(ثابت cv::String& a, ثابت cv::String& b)
{
    اگر (b.empty())
        واپس a;
    اگر (a.empty())
        واپس b;
    اگر (b[0] == ' ')
        واپس a + b;
    واپس a + (cv::String(" ") + b);
}

ساخت پروگرام_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت پروگرام_ماخذ& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(تہی),
         buildflags(_buildflags)
    {
        ثابت پروگرام_ماخذ::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        ماخذ_Module = src_->module_;
        ماخذ_Name = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        اگر (ctx.ptr() == تہی || device.ptr() == تہی)
            واپس;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        اگر (src.getImpl()->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_ماخذ_رمز)
        {
            اگر (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            ورنہ اگر (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            ثابت String param_buildExtraOptions = getBuildExtraOptions();
            اگر (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#اگر CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(تہی, "OpenCL program '" << ماخذ_Module << "/" << ماخذ_Name << "' options:" << buildflags);
#اگر_ختم
        compile(ctx, src_, errmsg);
#اگر CV_OPENCL_SHOW_BUILD_KERNELS
        اگر (handle)
        {
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            cl_int نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نام, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", ماخذ_Module.c_str(), ماخذ_Name.c_str()).c_str());
            اگر (نتیجہ == CL_SUCCESS && retsz < حجم(kernels_بفر))
            {
                kernels_بفر[retsz] = 0;
                CV_LOG_INFO(تہی, "OpenCL program '" << ماخذ_Module << "/" << ماخذ_Name << "' kernels: '" << kernels_بفر << "'");
            }
            ورنہ
            {
                CV_LOG_ERROR(تہی, "OpenCL program '" << ماخذ_Module << "/" << ماخذ_Name << "' can't retrieve kernel names!");
            }
        }
#اگر_ختم
    }

    منطقی compile(ثابت Context& ctx, ثابت پروگرام_ماخذ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        اگر (src_->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_BINARIES)
        {
            CV_LOG_VERBOSE(تہی, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقی isLoaded = create_From_ثنائی(ctx, src_->ماخذ_Addr, src_->ماخذ_سائز, errmsg);
            واپس isLoaded;
        }
        واپس compileWithCache(ctx, src_, errmsg);
    }

    منطقی compileWithCache(ثابت Context& ctx, ثابت پروگرام_ماخذ::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 != پروگرام_ماخذ::Impl::پروگرام_BINARIES);

#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string بنیاد_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_سابقہ_String(),
                ctx.getImpl()->get_سابقہ_بنیاد()
        );
        ثابت String& hash_str = src_->ماخذ_Hash;
        cv::String fname;
        اگر (!بنیاد_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(بنیاد_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::File_تالا> file_تالا = config.cache_تالا; // can be empty
        اگر (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            کوشش
            {
                std::vector<حرف> ثنائی_Buf;
                منطقی res = جھوٹ;
                {
                    cv::utils::optional_shared_تالا_guard<cv::utils::fs::File_تالا> تالا_fs(file_تالا.get());
                    ثنائی_پروگرام_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, ثنائی_Buf);
                }
                اگر (res)
                {
                    CV_Assert(!ثنائی_Buf.empty());
                    CV_LOG_VERBOSE(تہی, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقی isLoaded = create_From_ثنائی(ctx, ثنائی_Buf, errmsg);
                    اگر (isLoaded)
                        واپس سچ;
                }
            }
            پکڑو (ثابت cv::Exception& e)
            {
                CV_غیر_مستعمل(e);
                CV_LOG_VERBOSE(تہی, 0, "Can't load OpenCL binary: " + fname << std::سطر_ختم << e.what());
            }
            پکڑو (...)
            {
                CV_LOG_VERBOSE(تہی, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == تہی);
        اگر (src_->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_ماخذ_رمز)
        {
            اگر (!buildFromSources(ctx, src_, errmsg))
            {
                واپس جھوٹ;
            }
        }
        ورنہ اگر (src_->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            اگر ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(تہی, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقی isLoaded = create_From_ثنائی(ctx, src_->ماخذ_Addr, src_->ماخذ_سائز, errmsg);
            اگر (!isLoaded)
                واپس جھوٹ;
        }
        ورنہ اگر (src_->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        ورنہ
        {
            CV_Error(Error::Sts_اندرونی, "Internal error");
        }
        CV_Assert(handle != تہی);
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
        اگر (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            کوشش
            {
                std::vector<حرف> ثنائی_Buf;
                get_پروگرام_ثنائی(ثنائی_Buf);
                {
                    cv::utils::optional_تالا_guard<cv::utils::fs::File_تالا> تالا_fs(file_تالا.get());
                    ثنائی_پروگرام_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, ثنائی_Buf);
                }
            }
            پکڑو (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(تہی, "Can't save OpenCL binary into cache: " + fname << std::سطر_ختم << e.what());
            }
            پکڑو (...)
            {
                CV_LOG_WARNING(تہی, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
#اگر CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
        اگر (CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
        {
            std::vector<حرف> ثنائی_Buf;
            get_پروگرام_ثنائی(ثنائی_Buf);
            اگر (!ثنائی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                handle = تہی;
                create_From_ثنائی(ctx, ثنائی_Buf, errmsg);
            }
        }
#اگر_ختم
        واپس handle != تہی;
    }

    خالی dumpBuildLog_(cl_int نتیجہ, ثابت cl_device_id* deviceList, String& errmsg)
    {
        Auto_بفر<حرف, 4096> بفر; بفر[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                                  CL_پروگرام_BUILD_LOG, 0, 0, &retsz);
        اگر (log_retval == CL_SUCCESS && retsz > 1)
        {
            بفر.resize(retsz + 16);
            log_retval = cl_Get_پروگرام_Build_Info(handle, deviceList[0],
                                               CL_پروگرام_BUILD_LOG, retsz+1, بفر.ڈیٹا(), &retsz);
            اگر (log_retval == CL_SUCCESS)
            {
                اگر (retsz < بفر.size())
                    بفر[retsz] = 0;
                ورنہ
                    بفر[بفر.size() - 1] = 0;
            }
            ورنہ
            {
                بفر[0] = 0;
            }
        }

        errmsg = String(بفر.ڈیٹا());
        لکھو("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                ماخذ_Module.c_str(), ماخذ_Name.c_str(),
                نتیجہ, getOpenCLErrorString(نتیجہ),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    منطقی buildFromSources(ثابت Context& ctx, ثابت پروگرام_ماخذ::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->قسم_5 == پروگرام_ماخذ::Impl::پروگرام_ماخذ_رمز);
        CV_Assert(handle == تہی);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                ماخذ_Module.c_str(), ماخذ_Name.c_str(),
                src_->ماخذ_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(تہی, 0, "Compile... " << ماخذ_Module.c_str() << "/" << ماخذ_Name.c_str());

        ثابت حرف* srcptr = src_->ماخذ_Addr ? ((ثابت حرف*)src_->ماخذ_Addr) : src_->رمز_Str_2.c_str();
        size_t srclen = src_->ماخذ_Addr ? src_->ماخذ_سائز : src_->رمز_Str_2.size();
        CV_Assert(srcptr != تہی);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_پروگرام_With_ماخذ((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_نتیجہ(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        اگر (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_بفر<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.ڈیٹا();
            برائے (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_پروگرام(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_نتیجہ(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#اگر !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            اگر (retval != CL_SUCCESS)
#اگر_ختم
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                اگر (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = تہی;
                }
                اگر (retval != CL_SUCCESS &&
                    ماخذ_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#اگر CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
            اگر (handle && CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
            {
                CV_LOG_INFO(تہی, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_بفر[4096] = {0};
                cl_int نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نام, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
                اگر (retsz < حجم(kernels_بفر))
                    kernels_بفر[retsz] = 0;
                ورنہ
                    kernels_بفر[0] = 0;
                CV_LOG_INFO(تہی, نتیجہ << ": Kernels='" << kernels_بفر << "'");
            }
#اگر_ختم
        }
        واپس handle != تہی;
    }

    خالی get_پروگرام_ثنائی(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_ثنائی_SIZES, حجم(sz), &sz, تہی));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_پروگرام_Info(handle, CL_پروگرام_BINARIES, حجم(ptr), &ptr, تہی));
    }

    منطقی create_From_ثنائی(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        واپس create_From_ثنائی(ctx, (ثابت غیرعلامتی حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقی create_From_ثنائی(ثابت Context& ctx, ثابت غیرعلامتی حرف* ثنائی_Addr, ثابت size_t ثنائی_سائز, String& errmsg)
    {
        CV_Assert(handle == تہی);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(تہی, 0, "Load from binary... (" << ثنائی_سائز << " bytes)");

        CV_Assert(ثنائی_سائز > 0);

        size_t ndevices = (صحیح)ctx.ndevices();
        Auto_بفر<cl_device_id> devices_(ndevices);
        Auto_بفر<ثابت uchar*> ثنائی_Ptrs(ndevices);
        Auto_بفر<size_t> ثنائی_Sizes(ndevices);

        cl_device_id* devices = devices_.ڈیٹا();
        ثابت uchar** ثنائی_Ptrs_2 = ثنائی_Ptrs.ڈیٹا();
        size_t* ثنائی_Sizes_2 = ثنائی_Sizes.ڈیٹا();
        برائے (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            ثنائی_Ptrs_2[i] = ثنائی_Addr;
            ثنائی_Sizes_2[i] = ثنائی_سائز;
        }

        cl_int نتیجہ = 0;
        handle = cl_Create_پروگرام_With_ثنائی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.ڈیٹا(),
                                           ثنائی_Sizes_2, ثنائی_Ptrs_2, تہی, &نتیجہ);
        اگر (نتیجہ != CL_SUCCESS)
        {
            CV_LOG_ERROR(تہی, CV_OCL_API_ERROR_پیغام(نتیجہ, "clCreateProgramWithBinary"));
            اگر (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                handle = تہی;
            }
        }
        اگر (!handle)
        {
            واپس جھوٹ;
        }
        // call clBuildProgram()
        {
            نتیجہ = cl_Build_پروگرام(handle, (cl_uint)ndevices, devices_.ڈیٹا(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_نتیجہ(نتیجہ, cv::format("clBuildProgram(binary: %s/%s)", ماخذ_Module.c_str(), ماخذ_Name.c_str()).c_str());
            اگر (نتیجہ != CL_SUCCESS)
            {
                dumpBuildLog_(نتیجہ, devices, errmsg);
                اگر (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = تہی;
                }
                واپس جھوٹ;
            }
        }
        // check build status
        {
            cl_build_حالت build_حالت = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(نتیجہ = cl_Get_پروگرام_Build_Info(handle, devices[0], CL_پروگرام_BUILD_حالت,
                    حجم(build_حالت), &build_حالت, &retsz));
            اگر (نتیجہ == CL_SUCCESS)
            {
                اگر (build_حالت == CL_BUILD_SUCCESS)
                {
                    واپس سچ;
                }
                ورنہ
                {
                    CV_LOG_WARNING(تہی, "clGetProgramBuildInfo() returns " << build_حالت);
                    واپس جھوٹ;
                }
            }
            ورنہ
            {
                CV_LOG_ERROR(تہی, CV_OCL_API_ERROR_پیغام(نتیجہ, "clGetProgramBuildInfo()"));
                اگر (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروگرام(handle));
                    handle = تہی;
                }
            }
        }
#اگر CV_OPENCL_VALIDATE_ثنائی_PROGRAMS
        اگر (handle && CV_OPENCL_VALIDATE_ثنائی_PROGRAMS_قدر)
        {
            CV_LOG_INFO(تہی, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            نتیجہ = cl_Get_پروگرام_Info(handle, CL_پروگرام_KERNEL_نام, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            اگر (retsz < حجم(kernels_بفر))
                kernels_بفر[retsz] = 0;
            ورنہ
                kernels_بفر[0] = 0;
            CV_LOG_INFO(تہی, نتیجہ << ": Kernels='" << kernels_بفر << "'");
        }
#اگر_ختم
        واپس handle != تہی;
    }

    ~Impl()
    {
        اگر( handle )
        {
#اگر_تعریف_شدہ _WIN32
            اگر (!cv::__termination)
#اگر_ختم
            {
                cl_Release_پروگرام(handle);
            }
            handle = تہی;
        }
    }

    cl_پروگرام handle;

    String buildflags;
    String ماخذ_Module;
    String ماخذ_Name;
};


پروگرام_2::پروگرام_2() CV_NOEXCEPT
{
    p = 0;
}

پروگرام_2::پروگرام_2(ثابت پروگرام_ماخذ& src,
        ثابت String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

پروگرام_2::پروگرام_2(ثابت پروگرام_2& prog)
{
    p = prog.p;
    اگر(p)
        p->addref();
}

پروگرام_2& پروگرام_2::عامل = (ثابت پروگرام_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    اگر(newp)
        newp->addref();
    اگر(p)
        p->release();
    p = newp;
    واپس *یہ;
}

پروگرام_2::پروگرام_2(پروگرام_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = خالی_اشاریہ;
}

پروگرام_2& پروگرام_2::عامل = (پروگرام_2&& prog) CV_NOEXCEPT
{
    اگر (یہ != &prog) {
        اگر(p)
            p->release();
        p = prog.p;
        prog.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

پروگرام_2::~پروگرام_2()
{
    اگر(p)
        p->release();
}

منطقی پروگرام_2::create(ثابت پروگرام_ماخذ& src,
            ثابت String& buildflags, String& errmsg)
{
    اگر(p)
    {
        p->release();
        p = تہی;
    }
    p = نیا Impl(src, buildflags, errmsg);
    اگر(!p->handle)
    {
        p->release();
        p = 0;
    }
    واپس p != 0;
}

خالی* پروگرام_2::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

#اگر_غیر_تعریف_شدہ OPENCV_REMOVE_DEPRECATED_API
ثابت پروگرام_ماخذ& پروگرام_2::ماخذ() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقی پروگرام_2::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_غیر_مستعمل(bin); CV_غیر_مستعمل(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقی پروگرام_2::اكتب(String& bin) ثابت
{
    CV_غیر_مستعمل(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String پروگرام_2::get_سابقہ() ثابت
{
    اگر(!p)
        واپس String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    واپس cv::format("opencl=%s\nbuildflags=%s", ctx_->get_سابقہ_String().c_str(), p->buildflags.c_str());
}

String پروگرام_2::get_سابقہ(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        واپس cv::format("opencl=%s\nbuildflags=%s", ctx_->get_سابقہ_String().c_str(), buildflags.c_str());
}
#اگر_ختم // OPENCV_REMOVE_DEPRECATED_API

خالی پروگرام_2::get_ثنائی(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_پروگرام_ثنائی(binary);
}

پروگرام_2 Context::Impl::getProg(ثابت پروگرام_ماخذ& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t حد_بندی = get_پروگرام_گنتی_حد_بندی();
    ثابت پروگرام_ماخذ::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلید = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->ماخذ_Hash.c_str(),
            get_سابقہ_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_تالا lock(پروگرام_cache_mutex);
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
            واپس it->second;
        }
        { // cleanup program cache
            size_t sz = phash.size();
            اگر (حد_بندی > 0 && sz >= حد_بندی)
            {
                ساکن منطقی warningFlag = جھوٹ;
                اگر (!warningFlag)
                {
                    لکھو("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = سچ;
                }
                جب (!cacheList.empty())
                {
                    size_t c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    اگر (c != 0)
                        توڑو;
                }
            }
        }
    }
    پروگرام_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_تالا lock(پروگرام_cache_mutex);
        phash.insert(std::pair<std::string, پروگرام_2>(کلید, prog));
        cacheList.push_front(کلید);
    }
    واپس prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

سانچہ<قسم_نام T>
جماعت OpenCLBufferPool
{
محفوظ:
    ~OpenCLBufferPool() { }
عوامی:
    مجازی T allocate(size_t size) = 0;
    مجازی خالی release(T بفر) = 0;
};

سانچہ <قسم_نام Derived, قسم_نام بفر_اندراج, قسم_نام T>
جماعت Open_CLBuffer_Pool_بنیاد_Impl : عوامی بفر_Pool_Controller, عوامی OpenCLBufferPool<T>
{
نجی:
    اندرونی Derived& derived() { واپس *static_cast<Derived*>(یہ); }
محفوظ:
    Mutex mutex_;

    size_t current_Reserved_سائز;
    size_t max_Reserved_سائز;

    std::list<بفر_اندراج> allocatedEntries_; // Allocated and used entries
    std::list<بفر_اندراج> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقی ڈھونڈو_And_Remove_اندراج_From_Allocated_List(CV_برآمد بفر_اندراج& اندراج_2, T بفر)
    {
        قسم_نام std::list<بفر_اندراج>::iterator i = allocatedEntries_.begin();
        برائے (; i != allocatedEntries_.end(); ++i)
        {
            بفر_اندراج& e = *i;
            اگر (e.cl_بفر_2 == بفر)
            {
                اندراج_2 = e;
                allocatedEntries_.erase(i);
                واپس سچ;
            }
        }
        واپس جھوٹ;
    }

    // synchronized
    منطقی ڈھونڈو_And_Remove_اندراج_From_Reserved_List(CV_برآمد بفر_اندراج& اندراج_2, ثابت size_t size)
    {
        اگر (reservedEntries_.empty())
            واپس جھوٹ;
        قسم_نام std::list<بفر_اندراج>::iterator i = reservedEntries_.begin();
        قسم_نام std::list<بفر_اندراج>::iterator نتیجہ_pos = reservedEntries_.end();
        بفر_اندراج نتیجہ;
        size_t minDiff = (size_t)(-1);
        برائے (; i != reservedEntries_.end(); ++i)
        {
            بفر_اندراج& e = *i;
            اگر (e.استعداد >= size)
            {
                size_t diff = e.استعداد - size;
                اگر (diff < std::max((size_t)4096, size / 8) && (نتیجہ_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    نتیجہ_pos = i;
                    نتیجہ = e;
                    اگر (diff == 0)
                        توڑو;
                }
            }
        }
        اگر (نتیجہ_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(نتیجہ_pos);
            اندراج_2 = نتیجہ;
            current_Reserved_سائز -= اندراج_2.استعداد;
            allocatedEntries_.push_back(اندراج_2);
            واپس سچ;
        }
        واپس جھوٹ;
    }

    // synchronized
    خالی check_سائز_کا_Reserved_Entries()
    {
        جب (current_Reserved_سائز > max_Reserved_سائز)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت بفر_اندراج& اندراج_2 = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_سائز >= اندراج_2.استعداد);
            current_Reserved_سائز -= اندراج_2.استعداد;
            derived().release_بفر_اندراج(اندراج_2);
            reservedEntries_.pop_back();
        }
    }

    اندرونی size_t تخصیص_Granularity(size_t size)
    {
        // heuristic values
        اگر (size < 1024*1024)
            واپس 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        ورنہ اگر (size < 16*1024*1024)
            واپس 64*1024;
        ورنہ
            واپس 1024*1024;
    }

عوامی:
    Open_CLBuffer_Pool_بنیاد_Impl()
        : current_Reserved_سائز(0),
          max_Reserved_سائز(0)
    {
        // nothing
    }
    مجازی ~Open_CLBuffer_Pool_بنیاد_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عوامی:
    مجازی T allocate(size_t size) CV_OVERRIDE
    {
        Auto_تالا locker(mutex_);
        بفر_اندراج اندراج_2;
        اگر (max_Reserved_سائز > 0 && ڈھونڈو_And_Remove_اندراج_From_Reserved_List(اندراج_2, size))
        {
            CV_DbgAssert(size <= اندراج_2.استعداد);
            LOG_بفر_POOL("Reuse reserved buffer: %p\n", اندراج_2.cl_بفر_2);
        }
        ورنہ
        {
            derived().allocate_بفر_اندراج(اندراج_2, size);
        }
        واپس اندراج_2.cl_بفر_2;
    }
    مجازی خالی release(T بفر) CV_OVERRIDE
    {
        Auto_تالا locker(mutex_);
        بفر_اندراج اندراج_2;
        CV_Assert(ڈھونڈو_And_Remove_اندراج_From_Allocated_List(اندراج_2, بفر));
        اگر (max_Reserved_سائز == 0 || اندراج_2.استعداد > max_Reserved_سائز / 8)
        {
            derived().release_بفر_اندراج(اندراج_2);
        }
        ورنہ
        {
            reservedEntries_.push_front(اندراج_2);
            current_Reserved_سائز += اندراج_2.استعداد;
            check_سائز_کا_Reserved_Entries();
        }
    }

    مجازی size_t get_Reserved_سائز() ثابت CV_OVERRIDE { واپس current_Reserved_سائز; }
    مجازی size_t get_Max_Reserved_سائز() ثابت CV_OVERRIDE { واپس max_Reserved_سائز; }
    مجازی خالی سیٹ_Max_Reserved_سائز(size_t size) CV_OVERRIDE
    {
        Auto_تالا locker(mutex_);
        size_t old_Max_Reserved_سائز = max_Reserved_سائز;
        max_Reserved_سائز = size;
        اگر (max_Reserved_سائز < old_Max_Reserved_سائز)
        {
            قسم_نام std::list<بفر_اندراج>::iterator i = reservedEntries_.begin();
            برائے (; i != reservedEntries_.end();)
            {
                ثابت بفر_اندراج& اندراج_2 = *i;
                اگر (اندراج_2.استعداد > max_Reserved_سائز / 8)
                {
                    CV_DbgAssert(current_Reserved_سائز >= اندراج_2.استعداد);
                    current_Reserved_سائز -= اندراج_2.استعداد;
                    derived().release_بفر_اندراج(اندراج_2);
                    i = reservedEntries_.erase(i);
                    جاری;
                }
                ++i;
            }
            check_سائز_کا_Reserved_Entries();
        }
    }
    مجازی خالی freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_تالا locker(mutex_);
        قسم_نام std::list<بفر_اندراج>::const_iterator i = reservedEntries_.begin();
        برائے (; i != reservedEntries_.end(); ++i)
        {
            ثابت بفر_اندراج& اندراج_2 = *i;
            derived().release_بفر_اندراج(اندراج_2);
        }
        reservedEntries_.clear();
        current_Reserved_سائز = 0;
    }
};

ساخت CLBuffer_اندراج
{
    cl_mem cl_بفر_2;
    size_t استعداد;
    CLBuffer_اندراج() : cl_بفر_2((cl_mem)تہی), استعداد(0) { }
};

جماعت OpenCLBufferPoolImpl CV_FINAL : عوامی Open_CLBuffer_Pool_بنیاد_Impl<OpenCLBufferPoolImpl, CLBuffer_اندراج, cl_mem>
{
عوامی:
    قسمتعریف ساخت CLBuffer_اندراج بفر_اندراج;
محفوظ:
    صحیح createFlags_;
عوامی:
    OpenCLBufferPoolImpl(صحیح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    خالی allocate_بفر_اندراج(بفر_اندراج& اندراج_2, size_t size)
    {
        CV_DbgAssert(اندراج_2.cl_بفر_2 == تہی);
        اندراج_2.استعداد = align_سائز(size, (صحیح)تخصیص_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        اندراج_2.cl_بفر_2 = cl_Create_بفر((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, اندراج_2.استعداد, 0, &retval);
        CV_OCL_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (طویل طویل صحیح)اندراج_2.استعداد, (خالی*)اندراج_2.cl_بفر_2).c_str());
        CV_Assert(اندراج_2.cl_بفر_2 != تہی);
        اگر(retval == CL_SUCCESS)
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (طویل طویل)اندراج_2.استعداد, (طویل طویل)اندراج_2.استعداد, اندراج_2.cl_بفر_2);
        allocatedEntries_.push_back(اندراج_2);
    }

    خالی release_بفر_اندراج(ثابت بفر_اندراج& اندراج_2)
    {
        CV_Assert(اندراج_2.استعداد != 0);
        CV_Assert(اندراج_2.cl_بفر_2 != تہی);
        LOG_بفر_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                اندراج_2.cl_بفر_2, (طویل طویل)اندراج_2.استعداد, (طویل طویل)اندراج_2.استعداد);
        CV_OCL_DBG_CHECK(clReleaseMemObject(اندراج_2.cl_بفر_2));
    }
};

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
ساخت CLSVMBuffer_اندراج
{
    خالی* cl_بفر_2;
    size_t استعداد;
    CLSVMBuffer_اندراج() : cl_بفر_2(تہی), استعداد(0) { }
};
جماعت OpenCLSVMBufferPoolImpl CV_FINAL : عوامی Open_CLBuffer_Pool_بنیاد_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_اندراج, خالی*>
{
عوامی:
    قسمتعریف ساخت CLSVMBuffer_اندراج بفر_اندراج;
عوامی:
    OpenCLSVMBufferPoolImpl()
    {
    }

    خالی allocate_بفر_اندراج(بفر_اندراج& اندراج_2, size_t size)
    {
        CV_DbgAssert(اندراج_2.cl_بفر_2 == تہی);
        اندراج_2.استعداد = align_سائز(size, (صحیح)تخصیص_Granularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقی is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحیح)اندراج_2.استعداد);
        خالی *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, اندراج_2.استعداد, 0);
        CV_Assert(buf);

        اندراج_2.cl_بفر_2 = buf;
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (طویل طویل)اندراج_2.استعداد, (طویل طویل)اندراج_2.استعداد, اندراج_2.cl_بفر_2);
        allocatedEntries_.push_back(اندراج_2);
    }

    خالی release_بفر_اندراج(ثابت بفر_اندراج& اندراج_2)
    {
        CV_Assert(اندراج_2.استعداد != 0);
        CV_Assert(اندراج_2.cl_بفر_2 != تہی);
        LOG_بفر_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                اندراج_2.cl_بفر_2, (طویل طویل)اندراج_2.استعداد, (طویل طویل)اندراج_2.استعداد);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  اندراج_2.cl_بفر_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), اندراج_2.cl_بفر_2);
    }
};
#اگر_ختم



سانچہ <منطقی readAccess, منطقی writeAccess>
جماعت Aligned_ڈیٹا_Ptr
{
محفوظ:
    ثابت size_t سائز_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عوامی:
    Aligned_ڈیٹا_Ptr(uchar* ptr, size_t size, size_t alignment)
        : سائز_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(تہی)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        اگر (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = نیا uchar[سائز_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            اگر (readAccess)
            {
                حافظہ_نقل(ptr_, originPtr_, سائز_3);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        واپس ptr_;
    }

    ~Aligned_ڈیٹا_Ptr()
    {
        اگر (allocatedPtr_)
        {
            اگر (writeAccess)
            {
                حافظہ_نقل(originPtr_, ptr_, سائز_3);
            }
            حذف[] allocatedPtr_;
            allocatedPtr_ = تہی;
        }
        ptr_ = تہی;
    }
نجی:
    Aligned_ڈیٹا_Ptr(ثابت Aligned_ڈیٹا_Ptr&); // disabled
    Aligned_ڈیٹا_Ptr& عامل=(ثابت Aligned_ڈیٹا_Ptr&); // disabled
};

سانچہ <منطقی readAccess, منطقی writeAccess>
جماعت Aligned_ڈیٹا_Ptr2_D
{
محفوظ:
    ثابت size_t سائز_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t صفیں_2;
    size_t ستون_تعداد_2;
    size_t قدم_2;

عوامی:
    Aligned_ڈیٹا_Ptr2_D(uchar* ptr, size_t صفیں, size_t ستون_تعداد, size_t قدم, size_t alignment, size_t extrabytes=0)
        : سائز_3(صفیں*قدم), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(تہی), صفیں_2(صفیں), ستون_تعداد_2(ستون_تعداد), قدم_2(قدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != تہی);
        اگر (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = نیا uchar[سائز_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            اگر (readAccess)
            {
                برائے (size_t i = 0; i < صفیں_2; i++)
                    حافظہ_نقل(ptr_ + i*قدم_2, originPtr_ + i*قدم_2, ستون_تعداد_2);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        واپس ptr_;
    }

    ~Aligned_ڈیٹا_Ptr2_D()
    {
        اگر (allocatedPtr_)
        {
            اگر (writeAccess)
            {
                برائے (size_t i = 0; i < صفیں_2; i++)
                    حافظہ_نقل(originPtr_ + i*قدم_2, ptr_ + i*قدم_2, ستون_تعداد_2);
            }
            حذف[] allocatedPtr_;
            allocatedPtr_ = تہی;
        }
        ptr_ = تہی;
    }
نجی:
    Aligned_ڈیٹا_Ptr2_D(ثابت Aligned_ڈیٹا_Ptr2_D&); // disabled
    Aligned_ڈیٹا_Ptr2_D& عامل=(ثابت Aligned_ڈیٹا_Ptr2_D&); // disabled
};

#اگر_غیر_تعریف_شدہ CV_OPENCL_ڈیٹا_PTR_ALIGNMENT
#تعریف_کرو CV_OPENCL_ڈیٹا_PTR_ALIGNMENT 16
#اگر_ختم


خالی Context::Impl::__init_buffer_pools()
{
    بفر_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& بفر_Pool_3 = *بفر_Pool_2.get();
    بفر_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& بفر_Pool_Host_Ptr_2 = *بفر_Pool_Host_Ptr.get();

    size_t default_Pool_سائز = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_سائز = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_سائز);
    بفر_Pool_3.سیٹ_Max_Reserved_سائز(pool_سائز);
    size_t pool_سائز_Host_Ptr = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_سائز);
    بفر_Pool_Host_Ptr_2.سیٹ_Max_Reserved_سائز(pool_سائز_Host_Ptr);

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
    بفر_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& بفر_Pool_SVM_2 = *بفر_Pool_SVM.get();
    size_t pool_سائز_SVM = utils::get_Configuration_Parameter_سائز_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_سائز);
    بفر_Pool_SVM_2.سیٹ_Max_Reserved_سائز(pool_سائز_SVM);
#اگر_ختم

    CV_LOG_INFO(تہی, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_سائز << " poolSizeHostPtr=" << pool_سائز_Host_Ptr);
}

جماعت OpenCLAllocator CV_FINAL : عوامی MatAllocator
{
عوامی:
    شمار AllocatorFlags
    {
        ALLOCATOR_FLAGS_بفر_POOL_مستعمل = 1 << 0,
        ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_مستعمل = 1 << 1,
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_بفر_POOL_SVM_مستعمل = 1 << 2,
#اگر_ختم
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

    UMat_ڈیٹا* defaultAllocate(صحیح dims, ثابت صحیح* sizes, صحیح type, خالی* ڈیٹا, size_t* قدم,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMat_ڈیٹا* u = matStdAllocator->allocate(dims, sizes, type, ڈیٹا, قدم, flags, usageFlags);
        واپس u;
    }

    ساکن منطقی isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        ساکن منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", جھوٹ);
        واپس value;
    }
    ساکن منطقی isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        ساکن منطقی value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", جھوٹ);
        واپس value;
    }

    خالی get_بہترین_Flags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحیح& createFlags, UMat_ڈیٹا::حافظہ_Flag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        اگر ((usageFlags & USAGE_ALLOCATE_HOST_حافظہ) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        اگر (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_حافظہ_2()
#اگر_غیر_تعریف_شدہ __APPLE__
                || dev.isIntel()
#اگر_ختم
                )
            )
        )
            flags0 = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
        ورنہ
            flags0 = UMat_ڈیٹا::COPY_ON_MAP;
    }

    UMat_ڈیٹا* allocate(صحیح dims, ثابت صحیح* sizes, صحیح type,
                       خالی* ڈیٹا, size_t* قدم, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        اگر(!useOpenCL())
            واپس defaultAllocate(dims, sizes, type, ڈیٹا, قدم, flags, usageFlags);

        flush_Cleanup_قطار();

        CV_Assert(ڈیٹا == 0);
        size_t کل = CV_ELEM_سائز(type);
        برائے( صحیح i = dims-1; i >= 0; i-- )
        {
            اگر( قدم )
                قدم[i] = کل;
            کل *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        اگر (!ctx.getImpl())
            واپس defaultAllocate(dims, sizes, type, ڈیٹا, قدم, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحیح createFlags = 0;
        UMat_ڈیٹا::حافظہ_Flag flags0 = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
        get_بہترین_Flags(ctx, flags, usageFlags, createFlags, flags0);

        خالی* handle = تہی;
        صحیح allocatorFlags = 0;

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        اگر (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_SVM_مستعمل;
            handle = ctxImpl.get_بفر_Pool_SVM().allocate(کل);

            // this property is constant, so single buffer pool can be used here
            منطقی is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
            allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
        }
        ورنہ
#اگر_ختم
        اگر (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_مستعمل;
            handle = ctxImpl.get_بفر_Pool().allocate(کل);
        }
        ورنہ اگر (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_مستعمل;
            handle = ctxImpl.get_بفر_Pool_Host_Ptr().allocate(کل);
        }
        ورنہ
        {
            CV_Assert(handle != تہی); // Unsupported, throw
        }

        اگر (!handle)
            واپس defaultAllocate(dims, sizes, type, ڈیٹا, قدم, flags, usageFlags);

        UMat_ڈیٹا* u = نیا UMat_ڈیٹا(یہ);
        u->ڈیٹا = 0;
        u->size = کل;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<خالی>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->عارضی_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(سچ);
        opencl_allocator_stats.onAllocate(u->size);
        واپس u;
    }

    منطقی allocate(UMat_ڈیٹا* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس جھوٹ;

        flush_Cleanup_قطار();

        UMat_ڈیٹا_Auto_تالا lock(u);

        اگر(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحیح createFlags = 0;
            UMat_ڈیٹا::حافظہ_Flag flags0 = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
            get_بہترین_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقی copyOnMap = (flags0 & UMat_ڈیٹا::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحیح allocatorFlags = 0;
            UMat_ڈیٹا::حافظہ_Flag عارضی_UMat_Flags = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
            خالی* handle = تہی;
            cl_int retval = CL_SUCCESS;

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقی useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            اگر (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                عارضی_UMat_Flags = UMat_ڈیٹا::عارضی_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (صحیح)u->size, handle);
            }
            ورنہ اگر (useSVM && (svmCaps.is_Support_Fine_Grain_بفر() || svmCaps.is_Support_Coarse_Grain_بفر()))
            {
                اگر (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    منطقی is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (صحیح)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_قطار q = تہی;
                    اگر (!is_Fine_Grain_بفر)
                    {
                        q = (cl_command_قطار)قطار::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحیح)u->size);
                        cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, تہی, تہی);
                        CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMap()");

                    }
                    حافظہ_نقل(handle, u->origdata, u->size);
                    اگر (!is_Fine_Grain_بفر)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, handle, 0, تہی, تہی);
                        CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                    }

                    عارضی_UMat_Flags = UMat_ڈیٹا::عارضی_UMAT | UMat_ڈیٹا::عارضی_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر
                                                : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
                }
            }
            ورنہ
#اگر_ختم
            {
                اگر( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                عارضی_UMat_Flags = UMat_ڈیٹا::عارضی_UMAT;
                اگر (
                #اگر_تعریف_شدہ __APPLE__
                    !copyOnMap &&
                #اگر_ختم
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحیح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_ڈیٹا && u->original_UMat_ڈیٹا->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (طویل طویل صحیح)u->size, u->origdata, (خالی*)handle).c_str());
                }
                اگر((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (طویل طویل صحیح)u->size, u->origdata, (خالی*)handle).c_str());
                    عارضی_UMat_Flags |= UMat_ڈیٹا::عارضی_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clCreateBuffer() => %p", (خالی*)handle).c_str());
            اگر(!handle || retval != CL_SUCCESS)
                واپس جھوٹ;
            u->handle = handle;
            u->پچھلا_Allocator = u->currAllocator;
            u->currAllocator = یہ;
            u->flags |= عارضی_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(سچ);
        opencl_allocator_stats.onAllocate(u->size);
        واپس سچ;
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

    خالی deallocate(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        اگر (!!(u->flags & UMat_ڈیٹا::ASYNC_CLEANUP))
            جوڑو_مقصد_Cleanup_قطار(u);
        ورنہ
            deallocate_(u);
    }

    خالی deallocate_(UMat_ڈیٹا* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        اگر ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_بفر) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#اگر_تعریف_شدہ _WIN32
        اگر (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            واپس;             // avoid any OpenCL calls
#اگر_ختم
        اگر(u->عارضی_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            اگر (u->hostCopyObsolete())
            {
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    اگر( u->عارضی_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر);
                        منطقی is_Fine_Grain_بفر = (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر;
                        cl_command_قطار q = تہی;
                        اگر (!is_Fine_Grain_بفر)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0));
                            q = (cl_command_قطار)قطار::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                            cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, تہی, تہی);
                            CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMap()");
                        }
                        cl_اختتام(q);
                        حافظہ_نقل(u->origdata, u->handle, u->size);
                        اگر (!is_Fine_Grain_بفر)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, u->handle, 0, تہی, تہی);
                            CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                        }
                    }
                    ورنہ
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                ورنہ
#اگر_ختم
                {
                    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
                    اگر( u->عارضی_Copied_UMat() )
                    {
                        Aligned_ڈیٹا_Ptr<جھوٹ, سچ> alignedPtr(u->origdata, u->size, CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    ورنہ
                    {
                        cl_int retval = 0;
                        اگر (u->عارضی_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_قطار(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            خالی* ڈیٹا = cl_قطار_میں_ڈالو_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (خالی*)u->handle, (طویل طویل صحیح)u->size, ڈیٹا).c_str());
                            CV_Assert(u->origdata == ڈیٹا && "Details: https://github.com/opencv/opencv/issues/6293");
                            اگر (u->original_UMat_ڈیٹا)
                            {
                                CV_Assert(u->original_UMat_ڈیٹا->ڈیٹا == ڈیٹا);
                            }
                            retval = cl_قطار_میں_ڈالو_Unmap_Mem_Object(q, (cl_mem)u->handle, ڈیٹا, 0, 0, 0);
                            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (خالی*)u->handle, ڈیٹا, (طویل طویل صحیح)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_اختتام(q));
                        }
                    }
                }
                u->markHostCopyObsolete(جھوٹ);
            }
            ورنہ
            {
                // nothing
            }
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                اگر( u->عارضی_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            ورنہ
#اگر_ختم
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clReleaseMemObject(ptr=%p)", (خالی*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            u->currAllocator = u->پچھلا_Allocator;
            u->پچھلا_Allocator = تہی;
            اگر(u->ڈیٹا && u->copyOnMap() && u->ڈیٹا != u->origdata)
                fastFree(u->ڈیٹا);
            u->ڈیٹا = u->origdata;
            u->currAllocator->deallocate(u);
            u = تہی;
        }
        ورنہ
        {
            CV_Assert(u->origdata == تہی);
            اگر(u->ڈیٹا && u->copyOnMap() && u->ڈیٹا != u->origdata)
            {
                fastFree(u->ڈیٹا);
                u->ڈیٹا = 0;
                u->markHostCopyObsolete(سچ);
            }
            اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_مستعمل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool().release((cl_mem)u->handle);
            }
            ورنہ اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_مستعمل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            ورنہ اگر (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_SVM_مستعمل)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                ورنہ اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

                    اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, u->handle, 0, تہی, تہی);
                        CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_SVM().release((خالی*)u->handle);
            }
#اگر_ختم
            ورنہ
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(سچ);
            حذف u;
            u = تہی;
        }
        CV_Assert(u == تہی);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    خالی map(UMat_ڈیٹا* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        اگر (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(سچ);

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        {
            اگر( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                            cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, تہی, تہی);
                            CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_بفر_MAP;
                        }
                    }
                    cl_اختتام(q);
                    u->ڈیٹا = (uchar*)u->handle;
                    u->markHostCopyObsolete(جھوٹ);
                    u->markDeviceMemMapped(سچ);
                    واپس;
                }
#اگر_ختم

                cl_int retval = CL_SUCCESS;
                اگر (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->ڈیٹا = (uchar*)cl_قطار_میں_ڈالو_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_نتیجہ(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (خالی*)u->handle, (طویل طویل صحیح)u->size, u->ڈیٹا).c_str());
                }
                اگر (u->ڈیٹا && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(جھوٹ);
                    u->markDeviceMemMapped(سچ);
                    واپس;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_ڈیٹا::COPY_ON_MAP;
            }

            اگر(!u->ڈیٹا)
            {
                u->ڈیٹا = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(سچ);
            }
        }

        اگر (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_ڈیٹا_Ptr<جھوٹ, سچ> alignedPtr(u->ڈیٹا, u->size, CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#اگر_ختم
            cl_int retval = cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (خالی*)u->handle, (طویل طویل صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(جھوٹ);
        }
    }

    خالی unmap(UMat_ڈیٹا* u) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس;


        CV_Assert(u->handle != 0);

        UMat_ڈیٹا_Auto_تالا autolock(u);

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
        cl_int retval = 0;
        اگر( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->ڈیٹا != تہی);
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, u->handle,
                                0, تہی, تہی);
                        CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                        cl_اختتام(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_بفر_MAP;
                    }
                }
                اگر (u->refcount == 0)
                    u->ڈیٹا = 0;
                u->markDeviceCopyObsolete(جھوٹ);
                u->markHostCopyObsolete(سچ);
                واپس;
            }
#اگر_ختم
            اگر (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_قطار_میں_ڈالو_Unmap_Mem_Object(q, (cl_mem)u->handle, u->ڈیٹا, 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (خالی*)u->handle, u->ڈیٹا, (طویل طویل صحیح)u->size).c_str());
                اگر (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_اختتام(q));
                }
                u->markDeviceMemMapped(جھوٹ);
                u->ڈیٹا = 0;
                u->markDeviceCopyObsolete(جھوٹ);
                u->markHostCopyObsolete(سچ);
            }
        }
        ورنہ اگر( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_ڈیٹا_Ptr<سچ, جھوٹ> alignedPtr(u->ڈیٹا, u->size, CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == 0);
#اگر_ختم
            retval = cl_قطار_میں_ڈالو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (خالی*)u->handle, (طویل طویل صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(جھوٹ);
            u->markHostCopyObsolete(سچ);
        }
    }

    منطقی checkContinuous(صحیح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& کل, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقی iscontinuous = سچ;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        کل = sz[dims-1];
        برائے( صحیح i = dims-2; i >= 0; i-- )
        {
            اگر( i >= 0 && (کل != srcstep[i] || کل != dststep[i]) )
                iscontinuous = جھوٹ;
            کل *= sz[i];
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
            ورنہ
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
        واپس iscontinuous;
    }

    خالی download(UMat_ڈیٹا* u, خالی* dstptr, صحیح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس;
        UMat_ڈیٹا_Auto_تالا autolock(u);

        اگر( u->ڈیٹا && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            واپس;
        }
        CV_Assert( u->handle != 0 );

        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->ڈیٹا == تہی || u->ڈیٹا == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, تہی, تہی);
                CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMap()");
            }
            cl_اختتام(q);
            اگر( iscontinuous )
            {
                حافظہ_نقل(dstptr, (uchar*)u->handle + srcrawofs, کل);
            }
            ورنہ
            {
                // This code is from MatAllocator::download()
                صحیح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                برائے( صحیح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
                    اگر( sz[i] == 0 )
                    واپس;
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

                برائے( j = 0; j < it.nplanes; j++, ++it )
                    حافظہ_نقل(ptrs[1], ptrs[0], planesz);
            }
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, u->handle,
                        0, تہی, تہی);
                CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                cl_اختتام(q);
            }
        }
        ورنہ
#اگر_ختم
        {
            اگر( iscontinuous )
            {
                Aligned_ڈیٹا_Ptr<جھوٹ, سچ> alignedPtr((uchar*)dstptr, کل, CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, کل, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            ورنہ اگر (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_ڈیٹا_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_ڈیٹا_Ptr2_D<جھوٹ, جھوٹ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_ڈیٹا_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                کل = align_سائز(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                کل = std::min(کل, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, کل, ptr, 0, 0, 0));
                برائے( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            ورنہ
            {
                Aligned_ڈیٹا_Ptr2_D<جھوٹ, سچ> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    خالی upload(UMat_ڈیٹا* u, ثابت خالی* srcptr, صحیح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        اگر(!u)
            واپس;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->عارضی_UMat());

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ڈیٹا_Auto_تالا autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        اگر( u->ڈیٹا && (u->hostCopyObsolete() < u->deviceCopyObsolete() || کل == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(جھوٹ);
            u->markDeviceCopyObsolete(سچ);
            واپس;
        }

        CV_Assert( u->handle != 0 );
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            CV_DbgAssert(u->ڈیٹا == تہی || u->ڈیٹا == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحیح)u->size);
                cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, تہی, تہی);
                CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMap()");
            }
            cl_اختتام(q);
            اگر( iscontinuous )
            {
                حافظہ_نقل((uchar*)u->handle + dstrawofs, srcptr, کل);
            }
            ورنہ
            {
                // This code is from MatAllocator::upload()
                صحیح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                برائے( صحیح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
                    اگر( sz[i] == 0 )
                    واپس;
                    اگر( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (صحیح)sz[i];
                }

                Mat src(dims, isz, CV_8U, (خالی*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                ثابت Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                size_t j, planesz = it.size;

                برائے( j = 0; j < it.nplanes; j++, ++it )
                    حافظہ_نقل(ptrs[1], ptrs[0], planesz);
            }
            اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMUnmap(q, u->handle,
                        0, تہی, تہی);
                CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMUnmap()");
                cl_اختتام(q);
            }
        }
        ورنہ
#اگر_ختم
        {
            اگر( iscontinuous )
            {
                Aligned_ڈیٹا_Ptr<سچ, جھوٹ> alignedPtr((uchar*)srcptr, کل, CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
                cl_int retval = cl_قطار_میں_ڈالو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, کل, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (خالی*)u->handle, (طویل طویل صحیح)dstrawofs, (طویل طویل صحیح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            ورنہ اگر (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_ڈیٹا_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_ڈیٹا_Ptr2_D<جھوٹ, جھوٹ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_ڈیٹا_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                کل = align_سائز(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                کل = std::min(کل, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, کل, ptr, 0, 0, 0));
                برائے( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, کل, ptr, 0, 0, 0));
            }
            ورنہ
            {
                Aligned_ڈیٹا_Ptr2_D<سچ, جھوٹ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_ڈیٹا_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Write_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(سچ);
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ورنہ
#اگر_ختم
        {
            u->markHostCopyObsolete(سچ);
        }
        u->markDeviceCopyObsolete(جھوٹ);
    }

    خالی copy(UMat_ڈیٹا* src, UMat_ڈیٹا* dst, صحیح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقی _sync) ثابت CV_OVERRIDE
    {
        اگر(!src || !dst)
            واپس;

        size_t کل = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقی iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            کل, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_ڈیٹا_Auto_تالا src_autolock(src, dst);

        اگر( !src->handle || (src->ڈیٹا && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->ڈیٹا + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            واپس;
        }
        اگر( !dst->handle || (dst->ڈیٹا && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->ڈیٹا + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(جھوٹ);
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
            اگر ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            ورنہ
#اگر_ختم
            {
                dst->markDeviceCopyObsolete(سچ);
            }
            واپس;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
        {
            اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                اگر( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحیح)کل);
                    cl_int حالت_2 = svmFns->fn_cl_قطار_میں_ڈالو_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            کل, 0, تہی, تہی);
                    CV_OCL_CHECK_نتیجہ(حالت_2, "clEnqueueSVMMemcpy()");
                }
                ورنہ
                {
                    cl_اختتام(q);
                    // This code is from MatAllocator::download()/upload()
                    صحیح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    برائے( صحیح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)زیادہ_سے_زیادہ_عدد );
                        اگر( sz[i] == 0 )
                        واپس;
                        اگر( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (صحیح)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    برائے( صحیح i = 0; i < dims; i++ )
                    {
                        اگر( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    ثابت Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    size_t j, planesz = it.size;

                    برائے( j = 0; j < it.nplanes; j++, ++it )
                        حافظہ_نقل(ptrs[1], ptrs[0], planesz);
                }
            }
            ورنہ
            {
                اگر ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->ڈیٹا + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                ورنہ
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->ڈیٹا + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        ورنہ
#اگر_ختم
        {
            اگر( iscontinuous )
            {
                retval = cl_قطار_میں_ڈالو_Copy_بفر(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, کل, 0, 0, 0);
                CV_OCL_CHECK_نتیجہ(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (خالی*)src->handle, (خالی*)dst->handle, (طویل طویل صحیح)srcrawofs, (طویل طویل صحیح)dstrawofs, (طویل طویل صحیح)کل).c_str());
            }
            ورنہ اگر (CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_ڈیٹا_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_ڈیٹا_Ptr2_D<جھوٹ, جھوٹ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_ڈیٹا_PTR_ALIGNMENT, padding*2);
                Aligned_ڈیٹا_Ptr2_D<جھوٹ, جھوٹ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_ڈیٹا_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_کل = align_سائز(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_کل = std::min(src_کل, src->size - new_srcrawofs);
                size_t dst_کل = align_سائز(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_کل = std::min(dst_کل, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_کل, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Read_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_کل, dstptr, 0, 0, 0));

                برائے( size_t i = 0; i < new_sz[1]; i++ )
                    حافظہ_نقل( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_قطار_میں_ڈالو_Write_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_کل, dstptr, 0, 0, 0));
            }
            ورنہ
            {
                CV_OCL_CHECK(retval = cl_قطار_میں_ڈالو_Copy_بفر_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        اگر (retval == CL_SUCCESS)
        {
            CV_IMPL_جوڑو(CV_IMPL_OCL)
        }

#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_نقاب) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        ورنہ
#اگر_ختم
        {
            dst->markHostCopyObsolete(سچ);
        }
        dst->markDeviceCopyObsolete(جھوٹ);

        اگر( _sync )
        {
            CV_OCL_DBG_CHECK(cl_اختتام(q));
        }
    }

    بفر_Pool_Controller* get_بفر_Pool_Controller(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        اگر (ctx.empty())
            واپس تہی;
#اگر_تعریف_شدہ HAVE_OPENCL_SVM
        اگر ((svm::checkForceSVMUmatUsage() && (id == تہی || strcmp(id, "OCL") == 0)) || (id != تہی && strcmp(id, "SVM") == 0))
        {
            واپس &ctx.getImpl()->get_بفر_Pool_SVM();
        }
#اگر_ختم
        اگر (id != تہی && strcmp(id, "HOST_ALLOC") == 0)
        {
            واپس &ctx.getImpl()->get_بفر_Pool_Host_Ptr();
        }
        اگر (id != تہی && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        واپس &ctx.getImpl()->get_بفر_Pool();
    }

    MatAllocator* matStdAllocator;

    تغیرپذیر cv::Mutex cleanup_قطار_Mutex;
    تغیرپذیر std::deque<UMat_ڈیٹا*> cleanup_قطار;

    خالی flush_Cleanup_قطار() ثابت
    {
        // TSAN false positive: see #27638
        اگر (!cleanup_قطار.empty())
        {
            std::deque<UMat_ڈیٹا*> q;
            {
                cv::Auto_تالا lock(cleanup_قطار_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_قطار);
            }
            برائے (std::deque<UMat_ڈیٹا*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    خالی جوڑو_مقصد_Cleanup_قطار(UMat_ڈیٹا* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_تالا lock(cleanup_قطار_Mutex);
            cleanup_قطار.push_back(u);
        }
    }
};

ساکن OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    ساکن OpenCLAllocator* g_allocator = نیا OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    واپس g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


نامخلا cv {

// three funcs below are implemented in umatrix.cpp
خالی سیٹ_سائز( UMat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* اقدام_2,
              منطقی auto_اقدام = جھوٹ );
خالی finalizeHdr(UMat& m);

} // namespace cv


نامخلا cv { نامخلا ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
خالی convert_From_بفر(خالی* cl_mem_بفر, size_t قدم, صحیح صفیں, صحیح ستون_تعداد, صحیح type, UMat& dst)
{
    صحیح d = 2;
    صحیح sizes[] = { صفیں, ستون_تعداد };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_نقاب) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    سیٹ_سائز(dst, d, sizes, 0, سچ);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_بفر;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_بفر == mem_type);

    size_t کل = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_سائز, حجم(size_t), &کل, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحیح)قدم >= ستون_تعداد * CV_ELEM_سائز(type));
    CV_Assert(کل >= صفیں * قدم);

    // attach clBuffer to UMatData
    dst.u = نیا UMat_ڈیٹا(getOpenCLAllocator());
    dst.u->ڈیٹا            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_بفر;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_ڈیٹا::حافظہ_Flag>(0);
    dst.u->handle          = cl_mem_بفر;
    dst.u->origdata        = 0;
    dst.u->پچھلا_Allocator   = 0;
    dst.u->size            = کل;

    finalizeHdr(dst);
    dst.addref();

    واپس;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
خالی convertFromImage(خالی* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, حجم(cl_image_format), &fmt, 0));

    صحیح گہرائی = CV_8U;
    انتخاب (fmt.image_channel_ڈیٹا_type)
    {
    حالت CL_UNORM_INT8:
    حالت CL_UNSIGNED_INT8:
        گہرائی = CV_8U;
        توڑو;

    حالت CL_SNORM_INT8:
    حالت CL_SIGNED_INT8:
        گہرائی = CV_8S;
        توڑو;

    حالت CL_UNORM_INT16:
    حالت CL_UNSIGNED_INT16:
        گہرائی = CV_16U;
        توڑو;

    حالت CL_SNORM_INT16:
    حالت CL_SIGNED_INT16:
        گہرائی = CV_16S;
        توڑو;

    حالت CL_SIGNED_INT32:
        گہرائی = CV_32S;
        توڑو;

    حالت CL_FLOAT:
        گہرائی = CV_32F;
        توڑو;

    حالت CL_HALF_FLOAT:
        گہرائی = CV_16F;
        توڑو;

    طےشدہ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    صحیح type = CV_8UC1;
    انتخاب (fmt.image_channel_ترتیب)
    {
    حالت CL_R:
    حالت CL_A:
    حالت CL_INTENSITY:
    حالت CL_LUMINANCE:
        type = CV_MAKE_TYPE(گہرائی, 1);
        توڑو;

    حالت CL_RG:
    حالت CL_RA:
        type = CV_MAKE_TYPE(گہرائی, 2);
        توڑو;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالت CL_RGBA:
    حالت CL_BGRA:
    حالت CL_ARGB:
        type = CV_MAKE_TYPE(گہرائی, 4);
        توڑو;

    طےشدہ:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        توڑو;
    }

    size_t قدم = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_صف_PITCH, حجم(size_t), &قدم, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_چوڑائی, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_اونچائی, حجم(size_t), &h, 0));

    dst.create((صحیح)h, (صحیح)w, type);

    cl_mem cl_بفر = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, تہی, تہی));

    CV_OCL_CHECK(cl_اختتام(q));

    واپس;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساکن خالی getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint عدد_Devices = 0;
    cl_int حالت_2 = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, تہی, &عدد_Devices);
    اگر (حالت_2 != CL_DEVICE_NOT_ملا) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_نتیجہ(حالت_2,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &عدد_Devices).c_str());
    }

    اگر (عدد_Devices == 0)
    {
        devices.clear();
        واپس;
    }

    devices.resize((size_t)عدد_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, عدد_Devices, &devices[0], &عدد_Devices));
}

ساخت PlatformInfo::Impl
{
    Impl(خالی* id)
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
        واپس clGetPlatformInfo(handle, prop, حجم(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < حجم(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String نسخہ_3;
    صحیح نسخہ_Major;
    صحیح نسخہ_Minor;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(خالی* platform_id)
{
    p = نیا Impl(platform_id);
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

PlatformInfo& PlatformInfo::عامل =(ثابت PlatformInfo& i)
{
    اگر (i.p != p)
    {
        اگر (i.p)
            i.p->addref();
        اگر (p)
            p->release();
        p = i.p;
    }
    واپس *یہ;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = خالی_اشاریہ;
}

PlatformInfo& PlatformInfo::عامل = (PlatformInfo&& i) CV_NOEXCEPT
{
    اگر (یہ != &i) {
        اگر(p)
            p->release();
        p = i.p;
        i.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

صحیح PlatformInfo::device_نمبر() ثابت
{
    واپس p ? (صحیح)p->devices.size() : 0;
}

خالی PlatformInfo::getDevice(Device& device, صحیح d) ثابت
{
    CV_Assert(p && d < (صحیح)p->devices.size() );
    اگر(p)
        device.سیٹ(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    واپس p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    واپس p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::نسخہ() ثابت
{
    واپس p ? p->نسخہ_3 : String();
}

صحیح PlatformInfo::نسخہ_Major_2() ثابت
{
    CV_Assert(p);
    واپس p->نسخہ_Major;
}

صحیح PlatformInfo::نسخہ_Minor_2() ثابت
{
    CV_Assert(p);
    واپس p->نسخہ_Minor;
}

ساکن خالی getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint عدد_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, تہی, &عدد_Platforms));

    اگر (عدد_Platforms == 0)
    {
        platforms.clear();
        واپس;
    }

    platforms.resize((size_t)عدد_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(عدد_Platforms, &platforms[0], &عدد_Platforms));
}

خالی getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    برائے (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((خالی*)&platforms[i]) );
}

ثابت حرف* type_مقصد_Str(صحیح type)
{
    ساکن ثابت حرف* tab[CV_گہرائی_MAX*16]=
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
    صحیح cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? خالی_اشاریہ : tab[گہرائی*16 + cn-1];
    CV_Assert(نتیجہ);
    واپس نتیجہ;
}

ثابت حرف* memop_Type_مقصد_Str(صحیح type)
{
    ساکن ثابت حرف* tab[CV_گہرائی_MAX*16] =
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
    صحیح cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? خالی_اشاریہ : tab[گہرائی*16 + cn-1];
    CV_Assert(نتیجہ);
    واپس نتیجہ;
}

ثابت حرف* vecop_Type_مقصد_Str(صحیح type)
{
    ساکن ثابت حرف* tab[CV_گہرائی_MAX*16] =
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
    صحیح cn = CV_MAT_CN(type), گہرائی = CV_MAT_گہرائی(type);
    ثابت حرف* نتیجہ = cn > 16 ? 0 : tab[گہرائی*16 + cn-1];
    CV_Assert(نتیجہ);
    واپس نتیجہ;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت حرف* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, حرف* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    واپس convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت حرف* convertTypeStr(صحیح sdepth, صحیح ddepth, صحیح cn, حرف* buf, size_t buf_سائز_2)
{
    اگر( sdepth == ddepth )
        واپس "noconvert";
    ثابت حرف *typestr = type_مقصد_Str(CV_MAKETYPE(ddepth, cn));
    اگر( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_سائز_2, "convert_%s", typestr);
    }
    ورنہ اگر( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_سائز_2, "convert_%s_sat_rte", typestr);
    ورنہ
        snprintf(buf, buf_سائز_2, "convert_%s_sat", typestr);

    واپس buf;
}

ثابت حرف* getOpenCLErrorString(صحیح error_رمز)
{
#تعریف_کرو CV_OCL_رمز(id) حالت id: واپس #id
#تعریف_کرو CV_OCL_رمز_2(id, name) حالت id: واپس #name
    انتخاب (error_رمز)
    {
    CV_OCL_رمز(CL_SUCCESS);
    CV_OCL_رمز(CL_DEVICE_NOT_ملا);
    CV_OCL_رمز(CL_DEVICE_NOT_دستیاب);
    CV_OCL_رمز(CL_COMPILER_NOT_دستیاب);
    CV_OCL_رمز(CL_MEM_OBJECT_تخصیص_FAILURE);
    CV_OCL_رمز(CL_برآمد_کا_RESOURCES);
    CV_OCL_رمز(CL_برآمد_کا_HOST_حافظہ);
    CV_OCL_رمز(CL_PROFILING_INFO_NOT_دستیاب);
    CV_OCL_رمز(CL_MEM_COPY_OVERLAP);
    CV_OCL_رمز(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_رمز(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_رمز(CL_BUILD_پروگرام_FAILURE);
    CV_OCL_رمز(CL_MAP_FAILURE);
    CV_OCL_رمز(CL_MISALIGNED_SUB_بفر_انحراف);
    CV_OCL_رمز(CL_EXEC_حالت_ERROR_FOR_EVENTS_اندر_WAIT_LIST);
    CV_OCL_رمز(CL_COMPILE_پروگرام_FAILURE);
    CV_OCL_رمز(CL_LINKER_NOT_دستیاب);
    CV_OCL_رمز(CL_LINK_پروگرام_FAILURE);
    CV_OCL_رمز(CL_DEVICE_تقسیم_FAILED);
    CV_OCL_رمز(CL_KERNEL_ARG_INFO_NOT_دستیاب);
    CV_OCL_رمز(CL_INVALID_قدر);
    CV_OCL_رمز(CL_INVALID_DEVICE_TYPE);
    CV_OCL_رمز(CL_INVALID_PLATFORM);
    CV_OCL_رمز(CL_INVALID_DEVICE);
    CV_OCL_رمز(CL_INVALID_CONTEXT);
    CV_OCL_رمز(CL_INVALID_قطار_PROPERTIES);
    CV_OCL_رمز(CL_INVALID_COMMAND_قطار);
    CV_OCL_رمز(CL_INVALID_HOST_PTR);
    CV_OCL_رمز(CL_INVALID_MEM_OBJECT);
    CV_OCL_رمز(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_رمز(CL_INVALID_IMAGE_سائز);
    CV_OCL_رمز(CL_INVALID_SAMPLER);
    CV_OCL_رمز(CL_INVALID_ثنائی);
    CV_OCL_رمز(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_رمز(CL_INVALID_پروگرام);
    CV_OCL_رمز(CL_INVALID_پروگرام_EXECUTABLE);
    CV_OCL_رمز(CL_INVALID_KERNEL_NAME);
    CV_OCL_رمز(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_رمز(CL_INVALID_KERNEL);
    CV_OCL_رمز(CL_INVALID_ARG_اشاریہ);
    CV_OCL_رمز(CL_INVALID_ARG_قدر);
    CV_OCL_رمز(CL_INVALID_ARG_سائز);
    CV_OCL_رمز(CL_INVALID_KERNEL_دلائل);
    CV_OCL_رمز(CL_INVALID_WORK_DIMENSION);
    CV_OCL_رمز(CL_INVALID_WORK_گروہ_سائز);
    CV_OCL_رمز(CL_INVALID_WORK_شے_سائز);
    CV_OCL_رمز(CL_INVALID_GLOBAL_انحراف);
    CV_OCL_رمز(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_رمز(CL_INVALID_EVENT);
    CV_OCL_رمز(CL_INVALID_OPERATION);
    CV_OCL_رمز(CL_INVALID_GL_OBJECT);
    CV_OCL_رمز(CL_INVALID_بفر_سائز);
    CV_OCL_رمز(CL_INVALID_MIP_LEVEL);
    CV_OCL_رمز(CL_INVALID_GLOBAL_WORK_سائز);
    // OpenCL 1.1
    CV_OCL_رمز(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_رمز(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_رمز(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_رمز(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_رمز(CL_INVALID_DEVICE_تقسیم_گنتی);
    // OpenCL 2.0
    CV_OCL_رمز_2(-69, CL_INVALID_PIPE_سائز);
    CV_OCL_رمز_2(-70, CL_INVALID_DEVICE_قطار);
    // Extensions
    CV_OCL_رمز_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_رمز_2(-1001, CL_PLATFORM_NOT_ملا_KHR);
    CV_OCL_رمز_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_رمز_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_رمز_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_رمز_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    طےشدہ: واپس "Unknown OpenCL error";
    }
#تعریف_منسوخ CV_OCL_رمز
#تعریف_منسوخ CV_OCL_رمز_2
}

سانچہ <قسم_نام T>
ساکن std::string ker_مقصد_Str(ثابت Mat & k)
{
    صحیح چوڑائی = k.ستون_تعداد - 1, گہرائی = k.گہرائی();
    ثابت T * ثابت ڈیٹا = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    اگر (گہرائی <= CV_8S)
    {
        برائے (صحیح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << (صحیح)ڈیٹا[i] << ")";
        stream << "DIG(" << (صحیح)ڈیٹا[چوڑائی] << ")";
    }
    ورنہ اگر (گہرائی == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        برائے (صحیح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << ڈیٹا[i] << "f)";
        stream << "DIG(" << ڈیٹا[چوڑائی] << "f)";
    }
    ورنہ اگر (گہرائی == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        برائے (صحیح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << (اعشاری)ڈیٹا[i] << "h)";
        stream << "DIG(" << (اعشاری)ڈیٹا[چوڑائی] << "h)";
    }
    ورنہ
    {
        برائے (صحیح i = 0; i < چوڑائی; ++i)
            stream << "DIG(" << ڈیٹا[i] << ")";
        stream << "DIG(" << ڈیٹا[چوڑائی] << ")";
    }

    واپس stream.str();
}

String kernel_مقصد_Str(داخلہ_Array _kernel, صحیح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحیح گہرائی = kernel.گہرائی();
    اگر (ddepth < 0)
        ddepth = گہرائی;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    اگر (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    اگر (ddepth != گہرائی)
        kernel.convert_مقصد(kernel, ddepth);

    قسمتعریف std::string (* func_t)(ثابت Mat &);
    ساکن ثابت func_t funcs[CV_گہرائی_MAX] = { ker_مقصد_Str<uchar>, ker_مقصد_Str<حرف>, ker_مقصد_Str<ushort>, ker_مقصد_Str<مختصر>,
                                                ker_مقصد_Str<صحیح>, ker_مقصد_Str<اعشاری>, ker_مقصد_Str<دوہرا>, ker_مقصد_Str<hfloat>,
                                                0, ker_مقصد_Str<منطقی>, ker_مقصد_Str<uint64_t>, ker_مقصد_Str<int64_t>, ker_مقصد_Str<غیرعلامتی> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    واپس cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#تعریف_کرو PROCESS_SRC(src) \
    کرو \
    { \
        اگر (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            سائز csize = src.size(); \
            صحیح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_گہرائی(ctype), \
                ckercn = سمتیہ_Widths[cdepth], cwidth = ccn * csize.چوڑائی; \
            اگر (cwidth < ckercn || ckercn <= 0) \
                واپس 1; \
            ستون_تعداد.push_back(cwidth); \
            اگر (strat == OCL_سمتیہ_OWN && ctype != ref_type) \
                واپس 1; \
            offsets.push_back(src.offset()); \
            اقدام.push_back(src.قدم()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    جب ((خالی)0, 0)

صحیح predict_Optimal_سمتیہ_چوڑائی(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
                              داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
                              داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9,
                              Ocl_سمتیہ_Strategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحیح سمتیہ_Widths[CV_گہرائی_MAX] = {
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
    اگر (سمتیہ_Widths[0] == 1)
    {
        // it's heuristic
        سمتیہ_Widths[CV_8U] = سمتیہ_Widths[CV_8S] = 4;
        سمتیہ_Widths[CV_16U] = سمتیہ_Widths[CV_16S] = سمتیہ_Widths[CV_16F] = 2;
        سمتیہ_Widths[CV_32S] = سمتیہ_Widths[CV_32F] = سمتیہ_Widths[CV_64F] = 1;
    }

    واپس check_Optimal_سمتیہ_چوڑائی(سمتیہ_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحیح check_Optimal_سمتیہ_چوڑائی([[maybe_unused]] ثابت صحیح *سمتیہ_Widths,
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
#اگر_تعریف_شدہ __APPLE__
    واپس 1;
#ورنہ
    CV_Assert(سمتیہ_Widths);

    صحیح ref_type = src1.type();

    std::vector<size_t> offsets, اقدام, ستون_تعداد;
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

    برائے (size_t i = 0; i < size; ++i)
        جب (offsets[i] % dividers[i] != 0 || اقدام[i] % dividers[i] != 0 || ستون_تعداد[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحیح kercn = *std::min_element(kercns.begin(), kercns.end());

    واپس kercn;
#اگر_ختم
}

صحیح predict_Optimal_سمتیہ_چوڑائی_Max(داخلہ_Array src1, داخلہ_Array src2, داخلہ_Array src3,
                                 داخلہ_Array src4, داخلہ_Array src5, داخلہ_Array src6,
                                 داخلہ_Array src7, داخلہ_Array src8, داخلہ_Array src9)
{
    واپس predict_Optimal_سمتیہ_چوڑائی(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_سمتیہ_MAX);
}

#تعریف_منسوخ PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
خالی build_Options_جوڑو_میٹرکس_Description(String& buildOptions, ثابت String& name, داخلہ_Array _m)
{
    اگر (!buildOptions.empty())
        buildOptions += " ";
    صحیح type = _m.type(), گہرائی = CV_MAT_گہرائی(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_مقصد_Str(type),
            name.c_str(), ocl::type_مقصد_Str(CV_MAKE_TYPE(گہرائی, 1)),
            name.c_str(), (صحیح)CV_MAT_CN(type),
            name.c_str(), (صحیح)CV_ELEM_سائز(type),
            name.c_str(), (صحیح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحیح)گہرائی
            );
}


ساخت Image2D::Impl
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

    ساکن cl_image_format getImageFormat(صحیح گہرائی, صحیح cn, منطقی norm)
    {
        cl_image_format format;
        ساکن ثابت صحیح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساکن ثابت صحیح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساکن ثابت صحیح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحیح channelType = norm ? channelTypesNorm[گہرائی] : channelTypes[گہرائی];
        صحیح channel_ترتیب = channelOrders[cn];
        format.image_channel_ڈیٹا_type = (cl_channel_type)channelType;
        format.image_channel_ترتیب = (cl_channel_ترتیب)channel_ترتیب;
        واپس format;
    }

    ساکن منطقی isFormatSupported(cl_image_format format)
    {
        اگر (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        اگر (!context)
            واپس جھوٹ;

        // Figure out how many formats are supported by this context.
        cl_uint عدد_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, عدد_Formats,
                                                تہی, &عدد_Formats);
        CV_OCL_DBG_CHECK_نتیجہ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        اگر (عدد_Formats > 0)
        {
            Auto_بفر<cl_image_format> formats(عدد_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, عدد_Formats,
                                             formats.ڈیٹا(), تہی);
            CV_OCL_DBG_CHECK_نتیجہ(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            برائے (cl_uint i = 0; i < عدد_Formats; ++i)
            {
                اگر (!memcmp(&formats[i], &format, حجم(format)))
                {
                    واپس سچ;
                }
            }
        }
        واپس جھوٹ;
    }

    خالی init(ثابت UMat &src, منطقی norm, منطقی alias)
    {
        اگر (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        صحیح err, گہرائی = src.گہرائی(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(گہرائی, cn, norm);

        اگر (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        اگر (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_قطار قطار_2 = (cl_command_قطار)قطار::getDefault().ptr();

#اگر_تعریف_شدہ CL_نسخہ_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحیح minor = d.device_نسخہ_Minor(), major = d.device_نسخہ_Major();
        CV_Assert(!alias || canCreateAlias(src));
        اگر (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_چوڑائی      = src.ستون_تعداد;
            desc.image_اونچائی     = src.صفیں;
            desc.image_گہرائی      = 0;
            desc.image_array_سائز = 1;
            desc.image_صف_pitch  = alias ? src.قدم[0] : 0;
            desc.image_قاش_pitch = 0;
            desc.بفر           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.عدد_mip_سطحیں   = 0;
            desc.عدد_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, تہی, &err);
        }
        ورنہ
#اگر_ختم
        {
            CV_SUPPRESS_DEPRECATED_آغاز
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.ستون_تعداد, src.صفیں, 0, تہی, &err);
            CV_SUPPRESS_DEPRECATED_انتہا
        }
        CV_OCL_DBG_CHECK_نتیجہ(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.ستون_تعداد), static_cast<size_t>(src.صفیں), 1 };

        cl_mem dev_ڈیٹا;
        اگر (!alias && !src.isContinuous())
        {
            dev_ڈیٹا = cl_Create_بفر(context, CL_MEM_READ_ONLY, src.ستون_تعداد * src.صفیں * src.elem_سائز(), تہی, &err);
            CV_OCL_CHECK_نتیجہ(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (طویل طویل صحیح)(src.ستون_تعداد * src.صفیں * src.elem_سائز()), (خالی*)dev_ڈیٹا
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.ستون_تعداد) * src.elem_سائز(), static_cast<size_t>(src.صفیں), 1};
            CV_OCL_CHECK(cl_قطار_میں_ڈالو_Copy_بفر_Rect(قطار_2, (cl_mem)src.handle(ACCESS_READ), dev_ڈیٹا, origin, origin,
                roi, src.قدم, 0, src.ستون_تعداد * src.elem_سائز(), 0, 0, تہی, تہی));
            CV_OCL_DBG_CHECK(clFlush(قطار_2));
        }
        ورنہ
        {
            dev_ڈیٹا = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_ڈیٹا != تہی);

        اگر (!alias)
        {
            CV_OCL_CHECK(cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(قطار_2, dev_ڈیٹا, handle, 0, origin, region, 0, تہی, 0));
            اگر (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(قطار_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_ڈیٹا));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = تہی;
}

Image2D::Image2D(ثابت UMat &src, منطقی norm, منطقی alias)
{
    p = نیا Impl(src, norm, alias);
}

منطقی Image2D::canCreateAlias(ثابت UMat &m)
{
    منطقی ret = جھوٹ;
    ثابت Device & d = ocl::Device::getDefault();
    اگر (d.image_From_بفر_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        اگر (pitchAlign && !(m.قدم % (pitchAlign * m.elem_سائز())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            اگر (!m.u->عارضی_UMat())
            {
                ret = سچ;
            }
        }
    }
    واپس ret;
}

منطقی Image2D::isFormatSupported(صحیح گہرائی, صحیح cn, منطقی norm)
{
    cl_image_format format = Impl::getImageFormat(گہرائی, cn, norm);

    واپس Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    اگر (p)
        p->addref();
}

Image2D & Image2D::عامل = (ثابت Image2D & i)
{
    اگر (i.p != p)
    {
        اگر (i.p)
            i.p->addref();
        اگر (p)
            p->release();
        p = i.p;
    }
    واپس *یہ;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = خالی_اشاریہ;
}

Image2D& Image2D::عامل = (Image2D&& i) CV_NOEXCEPT
{
    اگر (یہ != &i) {
        اگر (p)
            p->release();
        p = i.p;
        i.p = خالی_اشاریہ;
    }
    واپس *یہ;
}

Image2D::~Image2D()
{
    اگر (p)
        p->release();
}

خالی* Image2D::ptr() ثابت
{
    واپس p ? p->handle : 0;
}

منطقی internal::isOpenCLForced()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی value = جھوٹ;
    اگر (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", جھوٹ);
        initialized = سچ;
    }
    واپس value;
}

منطقی internal::isPerformanceCheckBypassed()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن منطقی value = جھوٹ;
    اگر (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", جھوٹ);
        initialized = سچ;
    }
    واپس value;
}

منطقی internal::isCLBuffer(UMat& u)
{
    اگر (isCudaBacked(u.u))
        واپس جھوٹ;
    خالی* h = u.handle(ACCESS_RW);
    اگر (!h)
        واپس سچ;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#اگر 1
    اگر ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        واپس جھوٹ;
#ورنہ
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, حجم(type), &type, تہی);
    اگر (ret != CL_SUCCESS || type != CL_MEM_OBJECT_بفر)
        واپس جھوٹ;
#اگر_ختم
    واپس سچ;
}

ساخت Timer::Impl
{
    ثابت قطار قطار_2;

    Impl(ثابت قطار& q)
        : قطار_2(q)
    {
    }

    ~Impl(){}

    خالی آغاز()
    {
        CV_OCL_DBG_CHECK(cl_اختتام((cl_command_قطار)قطار_2.ptr()));
        timer.آغاز();
    }

    خالی stop()
    {
        CV_OCL_DBG_CHECK(cl_اختتام((cl_command_قطار)قطار_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        واپس (uint64)(timer.get_وقت_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت قطار& q) : p(نیا Impl(q)) { }
Timer::~Timer() { حذف p; }

خالی Timer::آغاز()
{
    CV_Assert(p);
    p->آغاز();
}

خالی Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() ثابت
{
    CV_Assert(p);
    واپس p->durationNS();
}

}} // namespace

#اگر_ختم // HAVE_OPENCL
