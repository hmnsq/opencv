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

#اگر_تعریف_نشده HAVE_OPENCL
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
#پایان_اگر

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#لغو_تعریف CV_LOG_STRIP_LEVEL
#تعریف_کن CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/ocl_genbase.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "opencv2/core/utils/lock.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#تعریف_کن CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "OpenCL allocator: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#لغو_تعریف CV__ALLOCATOR_STATS_LOG

#تعریف_کن CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#تعریف_کن CV_OPENCL_SHOW_BUILD_OPTIONS             0
#تعریف_کن CV_OPENCL_SHOW_BUILD_KERNELS             0

#تعریف_کن CV_OPENCL_SHOW_پیاپی_KERNELS               0
#تعریف_کن CV_OPENCL_SYNC_پیاپی_KERNELS               0
#تعریف_کن CV_OPENCL_TRACE_CHECK                    0

#تعریف_کن CV_OPENCL_VALIDATE_دودویی_PROGRAMS       1

#تعریف_کن CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#تعریف_کن CV_OPENCL_SHOW_SVM_LOG                   0

#ضمن "opencv2/core/bufferpool.hpp"
#اگر_تعریف_نشده LOG_میانگیر_POOL
# إذا 0
#   define LOG_میانگیر_POOL چاپ_کن
# وإلا
#   define LOG_میانگیر_POOL(...)
# endif
#پایان_اگر

#إذا CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#تعریف_کن CV_OPENCL_SVM_TRACE_P چاپ_کن("line %d (ocl.cpp): ", __LINE__); چاپ_کن
#وإلا
#تعریف_کن CV_OPENCL_SVM_TRACE_P(...)
#پایان_اگر

#إذا CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#تعریف_کن CV_OPENCL_SVM_TRACE_ERROR_P چاپ_کن("Error on line %d (ocl.cpp): ", __LINE__); چاپ_کن
#وإلا
#تعریف_کن CV_OPENCL_SVM_TRACE_ERROR_P(...)
#پایان_اگر

#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"

#اگر_تعریف_شده HAVE_OPENCL_SVM
#ضمن "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ضمن "opencv2/core/opencl/opencl_svm.hpp"
#پایان_اگر

#ضمن "umatrix.hpp"

نطاق cv { نطاق ocl {

#تعریف_کن IMPLEMENT_REFCOUNTABLE() \
    فراغ addref() { CV_XADD(&refcount, 1); } \
    فراغ release() { إذا( CV_XADD(&refcount, -1) == 1 && !cv::__termination) حذف هذا; } \
    صحيح refcount

ساكن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    أرجع opencl_allocator_stats;
}

#اگر_تعریف_نشده _DEBUG
ساكن منطقي isRaiseError()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", خطأ);
        initialized = درست;
    }
    أرجع value;
}
#پایان_اگر

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
فراغ traceOpenCLCheck(cl_int وضعیت, ثابت حرف* message)
{
    std::خروجی << "OpenCV(OpenCL:" << وضعیت << "): " << message << std::پایان_سطر << std::flush;
}
#تعریف_کن CV_OCL_TRACE_CHECK_نتیجه(وضعیت, message) traceOpenCLCheck(وضعیت, message)
#وإلا
#تعریف_کن CV_OCL_TRACE_CHECK_نتیجه(وضعیت, message) /* nothing */
#پایان_اگر

#تعریف_کن CV_OCL_API_ERROR_پیام(check_نتیجه, پیام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_نتیجه), check_نتیجه, پیام)

#تعریف_کن CV_OCL_CHECK_نتیجه(check_نتیجه, پیام) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتیجه(check_نتیجه, پیام); \
        إذا (check_نتیجه != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پیام), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پیام_2 = CV_OCL_API_ERROR_پیام(check_نتیجه, پیام); \
            CV_Error(Error::OpenCLApiCallError, error_پیام_2); \
        } \
    } طالما (0)

#تعریف_کن CV_OCL_CHECK_(عبارت, check_نتیجه) افعل { عبارت; CV_OCL_CHECK_نتیجه(check_نتیجه, #expr); } طالما (0)

#تعریف_کن CV_OCL_CHECK(عبارت) افعل { cl_int __cl_result = (عبارت); CV_OCL_CHECK_نتیجه(__cl_result, #expr); } طالما (0)

#اگر_تعریف_شده _DEBUG
#تعریف_کن CV_OCL_DBG_CHECK_نتیجه(check_نتیجه, پیام) CV_OCL_CHECK_نتیجه(check_نتیجه, پیام)
#تعریف_کن CV_OCL_DBG_CHECK(عبارت) CV_OCL_CHECK(عبارت)
#تعریف_کن CV_OCL_DBG_CHECK_(عبارت, check_نتیجه) CV_OCL_CHECK_(عبارت, check_نتیجه)
#وإلا
#تعریف_کن CV_OCL_DBG_CHECK_نتیجه(check_نتیجه, پیام) \
    افعل { \
        CV_OCL_TRACE_CHECK_نتیجه(check_نتیجه, پیام); \
        إذا (check_نتیجه != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پیام), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پیام_2 = CV_OCL_API_ERROR_پیام(check_نتیجه, پیام); \
            CV_Error(Error::OpenCLApiCallError, error_پیام_2); \
        } \
    } طالما (0)
#تعریف_کن CV_OCL_DBG_CHECK_(عبارت, check_نتیجه) افعل { عبارت; CV_OCL_DBG_CHECK_نتیجه(check_نتیجه, #expr); } طالما (0)
#تعریف_کن CV_OCL_DBG_CHECK(عبارت) افعل { cl_int __cl_result = (عبارت); CV_OCL_DBG_CHECK_نتیجه(__cl_result, #expr); } طالما (0)
#پایان_اگر


ساكن ثابت منطقي CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", درست);
ساكن ثابت منطقي CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", درست);
ساكن ثابت منطقي CV_OPENCL_CACHE_قفل_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", درست);
ساكن ثابت منطقي CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", درست);

#إذا CV_OPENCL_VALIDATE_دودویی_PROGRAMS
ساكن ثابت منطقي CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", خطأ);
#پایان_اگر

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساكن ثابت منطقي CV_OPENCL_DISABLE_میانگیر_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#إذا 1 //def __APPLE__
        درست
#وإلا
        خطأ
#پایان_اگر
);

ساكن String getBuildExtraOptions()
{
    ساكن String param_buildExtraOptions;
    ساكن منطقي initialized = خطأ;
    إذا (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = درست;
        إذا (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(عدم, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    أرجع param_buildExtraOptions;
}

ساكن ثابت منطقي CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", درست);
ساكن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


بنية UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        گام = (صحيح)m.گام;
        ردیفها = m.ردیفها;
        تعداد_ستون = m.تعداد_ستون;
    }
    صحيح offset;
    صحيح گام;
    صحيح ردیفها;
    صحيح تعداد_ستون;
};

بنية UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        گام = (صحيح)m.گام.p[1];
        slicestep = (صحيح)m.گام.p[0];
        slices = (صحيح)m.size.p[0];
        ردیفها = m.size.p[1];
        تعداد_ستون = m.size.p[2];
    }
    صحيح offset;
    صحيح slicestep;
    صحيح گام;
    صحيح slices;
    صحيح ردیفها;
    صحيح تعداد_ستون;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
ساكن uint64 crc64( ثابت uchar* داده, size_t size, uint64 crc0=0 )
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
        initialized = درست;
    }

    uint64 بررسی_چرخهای = ~crc0;
    لكل( size_t idx = 0; idx < size; idx++ )
        بررسی_چرخهای = table[(uchar)بررسی_چرخهای ^ داده[idx]] ^ (بررسی_چرخهای >> 8);

    أرجع ~بررسی_چرخهای;
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
بنية OpenCLBinaryCacheConfigurator
{
    cv::String cache_مسیر;
    cv::String cache_قفل_filename;
    cv::Ptr<utils::fs::File_قفل> cache_قفل;

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
        cache_مسیر = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        إذا (cache_مسیر.empty())
        {
            CV_LOG_INFO(عدم, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        افعل
        {
            حاول
            {
                إذا (cache_مسیر.empty())
                    اكسر;
                إذا (cache_مسیر == "disabled")
                    اكسر;
                إذا (!utils::fs::createDirectories(cache_مسیر))
                {
                    CV_LOG_DEBUG(عدم, "Can't use OpenCL cache directory: " << cache_مسیر);
                    clear();
                    اكسر;
                }

                إذا (CV_OPENCL_CACHE_قفل_ENABLE)
                {
                    cache_قفل_filename = cache_مسیر + ".lock";
                    إذا (!utils::fs::exists(cache_قفل_filename))
                    {
                        CV_LOG_DEBUG(عدم, "Creating lock file... (" << cache_قفل_filename << ")");
                        std::ofstream قفل_filename(cache_قفل_filename.c_str(), std::ios::out);
                        إذا (!قفل_filename.is_open())
                        {
                            CV_LOG_WARNING(عدم, "Can't create lock file for OpenCL program cache: " << cache_قفل_filename);
                            اكسر;
                        }
                    }

                    حاول
                    {
                        cache_قفل = makePtr<utils::fs::File_قفل>(cache_قفل_filename.c_str());
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... (" << cache_قفل_filename << ")");
                        {
                            utils::shared_قفل_guard<utils::fs::File_قفل> lock(*cache_قفل);
                        }
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... Done!");
                    }
                    امسك (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_قفل_filename << std::پایان_سطر << e.what());
                    }
                    امسك (...)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_قفل_filename);
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
                CV_LOG_WARNING(عدم, "Can't prepare OpenCL program cache: " << cache_مسیر << std::پایان_سطر << e.what());
                clear();
            }
        } طالما (0);
        إذا (!cache_مسیر.empty())
        {
            إذا (cache_قفل.empty() && CV_OPENCL_CACHE_قفل_ENABLE)
            {
                CV_LOG_WARNING(عدم, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Successfully initialized OpenCL cache directory: " << cache_مسیر);
            }
        }
    }

    فراغ clear()
    {
        cache_مسیر.clear();
        cache_قفل_filename.clear();
        cache_قفل.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_پیشوند,
            ثابت std::string& cleanup_پیشوند)
    {
        إذا (cache_مسیر.empty())
            أرجع std::string();

        Auto_قفل lock(mutex_prepared_contexts_);

        ContextCacheType::iterator یافت_it = prepared_contexts_.find(ctx_پیشوند);
        إذا (یافت_it != prepared_contexts_.end())
            أرجع یافت_it->second;

        CV_LOG_INFO(عدم, "Preparing OpenCL cache configuration for context: " << ctx_پیشوند);

        std::string هدف_directory = cache_مسیر + ctx_پیشوند + "/";
        منطقي نتیجه = utils::fs::isDirectory(هدف_directory);
        إذا (!نتیجه)
        {
            حاول
            {
                CV_LOG_VERBOSE(عدم, 0, "Creating directory: " << هدف_directory);
                إذا (utils::fs::createDirectories(هدف_directory))
                {
                    نتیجه = درست;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "Can't create directory: " << هدف_directory);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCL program cache directory for context: " << هدف_directory << std::پایان_سطر << e.what());
            }
        }
        هدف_directory = نتیجه ? هدف_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_پیشوند, هدف_directory));

        إذا (نتیجه && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_پیشوند.empty())
        {
            حاول
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_مسیر, cleanup_پیشوند + "*", entries, خطأ, درست);
                std::vector<String> remove_entries;
                لكل (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    إذا (0 == name.find(cleanup_پیشوند))
                    {
                        إذا (0 == name.find(ctx_پیشوند))
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
                        cv::String مسیر = utils::fs::join(cache_مسیر, name);
                        حاول
                        {
                            utils::fs::remove_all(مسیر);
                            CV_LOG_WARNING(عدم, "Removed: " << مسیر);
                        }
                        امسك (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(عدم, "Exception during removal of obsolete OpenCL cache directory: " << مسیر << std::پایان_سطر << e.what());
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
صنف دودویی_برنامه_File
{
    تعداد { MAX_ENTRIES = 64 };

    تعريفنوع بیعلامت صحيح uint32_t;

    بنية CV_DECL_ALIGNED(4) File_سرآیند
    {
        uint32_t منبع_Signature_اندازه;
        //char sourceSignature[];
    };

    بنية CV_DECL_ALIGNED(4) File_جدول
    {
        uint32_t عدد_از_Entries;
        //uint32_t firstEntryOffset[];
    };

    بنية CV_DECL_ALIGNED(4) File_مدخل
    {
        uint32_t بعدی_مدخل_File_جابه_جایی; // 0 for the last entry in chain
        uint32_t کلید_اندازه;
        uint32_t داده_اندازه;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت منبع_Signature;
    ثابت size_t منبع_Signature_اندازه_2;

    std::fstream f;

    uint32_t مدخل_Offsets[MAX_ENTRIES];

    uint32_t getHash(ثابت std::string& options)
    {
        uint64 hash = crc64((ثابت uchar*)options.c_str(), options.size(), 0);
        أرجع hash & (MAX_ENTRIES - 1);
    }

    مضمن size_t get_File_اندازه()
    {
        size_t pos = (size_t)f.tellg();
        f.seekg(0, std::fstream::end);
        size_t file_اندازه = (size_t)f.tellg();
        f.seekg(pos, std::fstream::beg);
        أرجع file_اندازه;
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
    دودویی_برنامه_File(ثابت std::string& fileName, ثابت حرف* منبع_Signature_2)
        : fileName_(fileName), منبع_Signature(منبع_Signature_2), منبع_Signature_اندازه_2(منبع_Signature ? طول_رشته(منبع_Signature) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(منبع_Signature != عدم);
        CV_Assert(منبع_Signature_اندازه_2 > 0);
        memset(مدخل_Offsets, 0, حجم(مدخل_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        إذا(f.is_open() && get_File_اندازه() > 0)
        {
            منطقي isValid = خطأ;
            حاول
            {
                uint32_t file_منبع_Signature_اندازه = readUInt32();
                إذا (file_منبع_Signature_اندازه == منبع_Signature_اندازه_2)
                {
                    cv::Auto_میانگیر<حرف> file_منبع_Signature(file_منبع_Signature_اندازه + 1);
                    f.اقرأ(file_منبع_Signature.داده(), file_منبع_Signature_اندازه);
                    إذا (f.eof())
                    {
                        CV_LOG_ERROR(عدم, "Unexpected EOF");
                    }
                    وإلا إذا (memcmp(منبع_Signature_2, file_منبع_Signature.داده(), file_منبع_Signature_اندازه) == 0)
                    {
                        isValid = درست;
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

        size_t file_اندازه = get_File_اندازه();
        إذا (file_اندازه == 0)
        {
            CV_LOG_ERROR(عدم, "Invalid file (empty): " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_منبع_Signature_اندازه = readUInt32();
        CV_Assert(file_منبع_Signature_اندازه > 0);
        seekReadRelative(file_منبع_Signature_اندازه);

        uint32_t عدد_از_Entries = readUInt32();
        CV_Assert(عدد_از_Entries > 0);
        إذا (عدد_از_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        f.اقرأ((حرف*)&مدخل_Offsets[0], حجم(مدخل_Offsets));
        CV_Assert(!f.fail());

        uint32_t مدخل_رقم = getHash(کلید);

        uint32_t مدخل_جابه_جایی = مدخل_Offsets[مدخل_رقم];
        File_مدخل مدخل;
        طالما (مدخل_جابه_جایی > 0)
        {
            seekReadAbsolute(مدخل_جابه_جایی);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&مدخل, حجم(مدخل));
            CV_Assert(!f.fail());
            cv::Auto_میانگیر<حرف> file_کلید(مدخل.کلید_اندازه + 1);
            إذا (کلید.size() == مدخل.کلید_اندازه)
            {
                إذا (مدخل.کلید_اندازه > 0)
                {
                    f.اقرأ(file_کلید.داده(), مدخل.کلید_اندازه);
                    CV_Assert(!f.fail());
                }
                إذا (memcmp(file_کلید.داده(), کلید.c_str(), مدخل.کلید_اندازه) == 0)
                {
                    buf.resize(مدخل.داده_اندازه);
                    f.اقرأ(&buf[0], مدخل.داده_اندازه);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(عدم, 0, "Read...");
                    أرجع درست;
                }
            }
            إذا (مدخل.بعدی_مدخل_File_جابه_جایی == 0)
                اكسر;
            مدخل_جابه_جایی = مدخل.بعدی_مدخل_File_جابه_جایی;
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

        size_t file_اندازه = get_File_اندازه();
        إذا (file_اندازه == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)منبع_Signature_اندازه_2);
            f.اكتب(منبع_Signature, منبع_Signature_اندازه_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(مدخل_Offsets, 0, حجم(مدخل_Offsets));
            f.اكتب((حرف*)مدخل_Offsets, حجم(مدخل_Offsets));
            CV_Assert(!f.fail());
            f.flush();
            CV_Assert(!f.fail());
            f.close();
            f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
            CV_Assert(f.is_open());
            file_اندازه = get_File_اندازه();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_منبع_Signature_اندازه = readUInt32();
        CV_Assert(file_منبع_Signature_اندازه == منبع_Signature_اندازه_2);
        seekReadRelative(file_منبع_Signature_اندازه);

        uint32_t عدد_از_Entries = readUInt32();
        CV_Assert(عدد_از_Entries > 0);
        إذا (عدد_از_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        size_t جدول_Entries_جابه_جایی = (size_t)f.tellg();
        f.اقرأ((حرف*)&مدخل_Offsets[0], حجم(مدخل_Offsets));
        CV_Assert(!f.fail());

        uint32_t مدخل_رقم = getHash(کلید);

        uint32_t مدخل_جابه_جایی = مدخل_Offsets[مدخل_رقم];
        File_مدخل مدخل;
        طالما (مدخل_جابه_جایی > 0)
        {
            seekReadAbsolute(مدخل_جابه_جایی);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&مدخل, حجم(مدخل));
            CV_Assert(!f.fail());
            cv::Auto_میانگیر<حرف> file_کلید(مدخل.کلید_اندازه + 1);
            إذا (کلید.size() == مدخل.کلید_اندازه)
            {
                إذا (مدخل.کلید_اندازه > 0)
                {
                    f.اقرأ(file_کلید.داده(), مدخل.کلید_اندازه);
                    CV_Assert(!f.fail());
                }
                إذا (0 == memcmp(file_کلید.داده(), کلید.c_str(), مدخل.کلید_اندازه))
                {
                    // duplicate
                    CV_LOG_VERBOSE(عدم, 0, "Duplicate key ignored: " << fileName_);
                    أرجع خطأ;
                }
            }
            إذا (مدخل.بعدی_مدخل_File_جابه_جایی == 0)
                اكسر;
            مدخل_جابه_جایی = مدخل.بعدی_مدخل_File_جابه_جایی;
        }
        seekReadAbsolute(0);
        إذا (مدخل_جابه_جایی > 0)
        {
            seekWriteAbsolute(مدخل_جابه_جایی);
            مدخل.بعدی_مدخل_File_جابه_جایی = (uint32_t)file_اندازه;
            f.اكتب((حرف*)&مدخل, حجم(مدخل));
            CV_Assert(!f.fail());
        }
        وإلا
        {
            مدخل_Offsets[مدخل_رقم] = (uint32_t)file_اندازه;
            seekWriteAbsolute(جدول_Entries_جابه_جایی);
            f.اكتب((حرف*)مدخل_Offsets, حجم(مدخل_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_اندازه);
        مدخل.بعدی_مدخل_File_جابه_جایی = 0;
        مدخل.داده_اندازه = (uint32_t)buf.size();
        مدخل.کلید_اندازه = (uint32_t)کلید.size();
        f.اكتب((حرف*)&مدخل, حجم(مدخل));
        CV_Assert(!f.fail());
        f.اكتب(کلید.c_str(), مدخل.کلید_اندازه);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], مدخل.داده_اندازه);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(عدم, 0, "Write... (" << buf.size() << " bytes)");
        أرجع درست;
    }
};
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT



بنية OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحيح device_;  // device index in context
    ocl::صف صف_3;
    صحيح useOpenCL_;

محمي:
    Impl() = حذف;

    فراغ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي یافت = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            إذا (dhandle == deviceID)
            {
                device_ = i;
                یافت = درست;
                اكسر;
            }
        }
        CV_Assert(یافت && "OpenCL device can't work with passed OpenCL context");
    }

    فراغ _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي یافت = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            إذا (d.getImpl() == device.getImpl())
            {
                device_ = i;
                یافت = درست;
                اكسر;
            }
        }
        CV_Assert(یافت && "OpenCL device can't work with passed OpenCL context");
    }

عام:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_استفادهنشده(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        صف_3 = صف(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device, ثابت ocl::صف& صف_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        صف_3 = صف_2;
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        صف_3 = صف(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت صحيح device, ثابت ocl::صف& صف_2)
        : context_(context)
        , device_(device)
        , صف_3(صف_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ثابت Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , صف_3(other.صف_3)
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

    فراغ مجموعه_Use_Open_CL(منطقي flag)
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
            cv::Auto_قفل lock(getInitializationMutex());
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
                        إذا (d.موجود())
                        {
                            خودکار q = ocl::صف(c, d);
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
                initialized = درست;
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
صف& OpenCLExecutionContext::get_صف() ثابت
{
    CV_Assert(p);
    أرجع p->صف_3;
}

منطقي OpenCLExecutionContext::useOpenCL() ثابت
{
    إذا (p)
        أرجع p->useOpenCL();
    أرجع خطأ;
}
فراغ OpenCLExecutionContext::مجموعه_Use_Open_CL(منطقي flag)
{
    CV_Assert(p);
    p->مجموعه_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& داده = get_Core_Tls_داده();
    OpenCLExecutionContext& c = داده.oclExecutionContext;
    إذا (!داده.oclExecutionContextInitialized)
    {
        داده.oclExecutionContextInitialized = درست;
        إذا (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    أرجع c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& داده = get_Core_Tls_داده();
    OpenCLExecutionContext& c = داده.oclExecutionContext;
    أرجع c;
}

فراغ OpenCLExecutionContext::bind() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& داده = get_Core_Tls_داده();
    داده.oclExecutionContext = *هذا;
    داده.oclExecutionContextInitialized = درست;
    داده.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_صف() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت صف q(getContext(), getDevice());
    أرجع clone_With_New_صف(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_صف(ثابت ocl::صف& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != عدم);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    أرجع c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::صف& صف_2)
{
    CV_TRACE_FUNCTION();
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, صف_2);
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
        std::string env_مسیر = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        إذا (!env_مسیر.empty())
        {
            إذا (env_مسیر == "disabled")
            {
                g_isOpenCLAvailable = خطأ;
                g_isOpenCLInitialized = درست;
                أرجع خطأ;
            }
        }

        cv::Auto_قفل lock(getInitializationMutex());
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
        g_isOpenCLInitialized = درست;
    }
    أرجع g_isOpenCLAvailable;
}

منطقي useOpenCL()
{
    CoreTLSData& داده = get_Core_Tls_داده();
    إذا (داده.useOpenCL < 0)
    {
        حاول
        {
            داده.useOpenCL = 0;
            إذا (haveOpenCL())
            {
                خودکار c = OpenCLExecutionContext::getCurrent();
                داده.useOpenCL = c.useOpenCL();
            }
        }
        امسك (...)
        {
            CV_LOG_INFO(عدم, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    أرجع داده.useOpenCL > 0;
}

منطقي isOpenCLActivated()
{
    إذا (!g_isOpenCLAvailable)
        أرجع خطأ; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    أرجع useOpenCL();
}

فراغ مجموعه_Use_Open_CL(منطقي flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& داده = get_Core_Tls_داده();
    خودکار& c = OpenCLExecutionContext::getCurrentRef();
    إذا (!c.empty())
    {
        c.مجموعه_Use_Open_CL(flag);
        داده.useOpenCL = c.useOpenCL();
    }
    وإلا
    {
        إذا (!flag)
            داده.useOpenCL = 0;
        وإلا
            داده.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#اگر_تعریف_شده HAVE_CLAMDBLAS

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
            Auto_قفل lock(getInitializationMutex());

            إذا (!g_isAmdBlasInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        g_is_Amd_Blas_موجود = clblas_راهاندازی() == clblasSuccess;
                    }
                    امسك (...)
                    {
                        g_is_Amd_Blas_موجود = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Blas_موجود = خطأ;

                g_isAmdBlasInitialized = درست;
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

#پایان_اگر

#اگر_تعریف_شده HAVE_CLAMDFFT

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
            Auto_قفل lock(getInitializationMutex());

            إذا (!g_isAmdFftInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_راهاندازی_داده(&راهاندازی_داده) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_نسخه(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_موجود = درست;
                    }
                    امسك (ثابت Exception &)
                    {
                        g_is_Amd_Fft_موجود = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Fft_موجود = خطأ;

                g_isAmdFftInitialized = درست;
            }
        }
    }

خاص:
    ساكن clfft_راهاندازی_داده راهاندازی_داده;
    ساكن منطقي g_isAmdFftInitialized;
    ساكن منطقي g_is_Amd_Fft_موجود;
};

clfft_راهاندازی_داده AmdFftHelper::راهاندازی_داده;
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

#پایان_اگر

منطقي haveSVM()
{
#اگر_تعریف_شده HAVE_OPENCL_SVM
    أرجع درست;
#وإلا
    أرجع خطأ;
#پایان_اگر
}

فراغ پایان_2()
{
    صف::getDefault().پایان_2();
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

            initialized = درست;
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

Platform& Platform::عملگر = (ثابت Platform& pl)
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
    pl.p = اشاره_تهی;
}

Platform& Platform::عملگر = (Platform&& pl) CV_NOEXCEPT
{
    إذا (هذا != &pl) {
        إذا(p)
            p->release();
        p = pl.p;
        pl.p = اشاره_تهی;
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
ساكن فراغ parseOpenCLVersion(ثابت String &نسخه, صحيح &major, صحيح &minor)
{
    major = minor = 0;
    إذا (10 >= نسخه.length())
        أرجع;
    ثابت حرف *pstr = نسخه.c_str();
    إذا (0 != مقایسه_رشته(pstr, "OpenCL ", 7))
        أرجع;
    size_t ppos = نسخه.find('.', 7);
    إذا (String::npos == ppos)
        أرجع;
    String موقت_2 = نسخه.substr(7, ppos - 7);
    major = atoi(موقت_2.c_str());
    موقت_2 = نسخه.substr(ppos + 1);
    minor = atoi(موقت_2.c_str());
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
        نسخه_3 = getStrProp(CL_DEVICE_نسخه);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, صحيح>(CL_DEVICE_HALF_FP_CONFIG);
        host_Unified_حافظه = getBoolProp(CL_DEVICE_HOST_UNIFIED_حافظه);
        maxComputeUnits_ = getProp<cl_uint, صحيح>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_گروه_اندازه_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_گروه_اندازه);
        type_ = getProp<cl_device_type, صحيح>(CL_DEVICE_TYPE);
        driver_نسخه_2 = getStrProp(CL_DRIVER_نسخه);
        آدرس_بیتها_2 = getProp<cl_uint, صحيح>(CL_DEVICE_آدرس_بیتها);

        String device_نسخه = getStrProp(CL_DEVICE_نسخه);
        parseOpenCLVersion(device_نسخه, device_نسخه_Major_2, device_نسخه_Minor_2);

        size_t pos = 0;
        طالما (pos < extensions_.size())
        {
            size_t pos2 = extensions_.find(' ', pos);
            إذا (pos2 == String::npos)
                pos2 = extensions_.size();
            إذا (pos2 > pos)
            {
                std::string extensionName = extensions_.substr(pos, pos2 - pos);
                extensions_مجموعه.insert(extensionName);
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
        وإلا إذا (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || جستجوی_رشته(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وإلا إذا (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وإلا
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_گروه_اندازه = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        إذا (CV_OPENCL_DEVICE_MAX_WORK_گروه_اندازه > 0)
        {
            ثابت size_t new_max_Work_گروه_اندازه = std::min(max_Work_گروه_اندازه_2, CV_OPENCL_DEVICE_MAX_WORK_گروه_اندازه);
            إذا (new_max_Work_گروه_اندازه != max_Work_گروه_اندازه_2)
                CV_LOG_WARNING(عدم, "OpenCL: using workgroup size: " << new_max_Work_گروه_اندازه << " (was " << max_Work_گروه_اندازه_2 << ")");
            max_Work_گروه_اندازه_2 = new_max_Work_گروه_اندازه;
        }
#إذا 0
        إذا (isExtensionSupported("cl_khr_spir"))
        {
#اگر_تعریف_نشده CL_DEVICE_SPIR_VERSIONS
#تعریف_کن CL_DEVICE_SPIR_VERSIONS                     0x40E0
#پایان_اگر
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::خروجی << spir_versions << std::پایان_سطر;
        }
#پایان_اگر
    }

    ~Impl()
    {
#اگر_تعریف_شده _WIN32
        إذا (!cv::__termination)
#پایان_اگر
        {
            إذا (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    قالب<نام_نوع _TpCL, نام_نوع Tp_بیرون>
    Tp_بیرون getProp(cl_device_info prop) ثابت
    {
        _TpCL موقت_2=_TpCL();
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(موقت_2), &موقت_2, &sz) == CL_SUCCESS &&
            sz == حجم(موقت_2) ? Tp_بیرون(موقت_2) : Tp_بیرون();
    }

    منطقي getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool موقت_2 = CL_FALSE;
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(موقت_2), &موقت_2, &sz) == CL_SUCCESS &&
            sz == حجم(موقت_2) ? موقت_2 != 0 : خطأ;
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
        أرجع extensions_مجموعه.شمار_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String نسخه_3;
    std::string extensions_;
    صحيح doubleFPConfig_;
    منطقي khr_fp64_support_;
    صحيح halfFPConfig_;
    منطقي khr_fp16_support_;
    منطقي host_Unified_حافظه;
    صحيح maxComputeUnits_;
    size_t max_Work_گروه_اندازه_2;
    صحيح type_;
    صحيح آدرس_بیتها_2;
    صحيح device_نسخه_Major_2;
    صحيح device_نسخه_Minor_2;
    String driver_نسخه_2;
    String vendorName_;
    صحيح vendorID_;
    منطقي intelSubgroupsSupport_;

    std::مجموعه<std::string> extensions_مجموعه;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(فراغ* d)
{
    p = 0;
    مجموعه(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    إذا(p)
        p->addref();
}

Device& Device::عملگر = (ثابت Device& d)
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
    d.p = اشاره_تهی;
}

Device& Device::عملگر = (Device&& d) CV_NOEXCEPT
{
    إذا (هذا != &d) {
        إذا(p)
            p->release();
        p = d.p;
        d.p = اشاره_تهی;
    }
    أرجع *هذا;
}

Device::~Device()
{
    إذا(p)
        p->release();
}

فراغ Device::مجموعه(فراغ* d)
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

String Device::نسخه() ثابت
{ أرجع p ? p->نسخه_3 : String(); }

String Device::vendorName() ثابت
{ أرجع p ? p->vendorName_ : String(); }

صحيح Device::vendorID() ثابت
{ أرجع p ? p->vendorID_ : 0; }

String Device::Open_CL_C_نسخه() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_OPENCL_C_نسخه) : String(); }

String Device::OpenCLVersion() ثابت
{ أرجع p ? p->getStrProp(CL_DEVICE_نسخه) : String(); }

صحيح Device::device_نسخه_Major() ثابت
{ أرجع p ? p->device_نسخه_Major_2 : 0; }

صحيح Device::device_نسخه_Minor() ثابت
{ أرجع p ? p->device_نسخه_Minor_2 : 0; }

String Device::driver_نسخه() ثابت
{ أرجع p ? p->driver_نسخه_2 : String(); }

صحيح Device::type() ثابت
{ أرجع p ? p->type_ : 0; }

صحيح Device::آدرس_بیتها() ثابت
{ أرجع p ? p->آدرس_بیتها_2 : 0; }

منطقي Device::موجود() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_موجود) : خطأ; }

منطقي Device::compiler_موجود() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_COMPILER_موجود) : خطأ; }

منطقي Device::linker_موجود() ثابت
#اگر_تعریف_شده CL_نسخه_1_2
{ أرجع p ? p->getBoolProp(CL_DEVICE_LINKER_موجود) : خطأ; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#پایان_اگر

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

size_t Device::global_Mem_Cache_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_CACHE_اندازه) : 0; }

صحيح Device::globalMemCacheType() ثابت
{ أرجع p ? p->getProp<cl_device_mem_cache_type, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

صحيح Device::global_Mem_Cache_سطر_اندازه() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_GLOBAL_MEM_CACHELINE_اندازه) : 0; }

size_t Device::global_Mem_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_GLOBAL_MEM_اندازه) : 0; }

size_t Device::local_Mem_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_LOCAL_MEM_اندازه) : 0; }

صحيح Device::localMemType() ثابت
{ أرجع p ? p->getProp<cl_device_local_mem_type, صحيح>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

منطقي Device::host_Unified_حافظه_2() ثابت
{ أرجع p ? p->host_Unified_حافظه : خطأ; }

منطقي Device::imageSupport() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : خطأ; }

منطقي Device::image_From_میانگیر_Support() ثابت
{
    أرجع p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : خطأ;
}

uint Device::imagePitchAlignment() ثابت
{
#اگر_تعریف_شده CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#پایان_اگر
}

uint Device::image_پایه_آدرس_Alignment() ثابت
{
#اگر_تعریف_شده CL_DEVICE_IMAGE_پایه_آدرس_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_پایه_آدرس_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#پایان_اگر
}

size_t Device::image2_DMax_عرض() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_عرض) : 0; }

size_t Device::image2_DMax_ارتفاع() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_ارتفاع) : 0; }

size_t Device::image3_DMax_عرض() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_عرض) : 0; }

size_t Device::image3_DMax_ارتفاع() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ارتفاع) : 0; }

size_t Device::image3_DMax_عمق() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_عمق) : 0; }

size_t Device::image_Max_میانگیر_اندازه() ثابت
#اگر_تعریف_شده CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_میانگیر_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#پایان_اگر

size_t Device::image_Max_Array_اندازه() ثابت
#اگر_تعریف_شده CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#پایان_اگر

منطقي Device::intelSubgroupsSupport() ثابت
{ أرجع p ? p->intelSubgroupsSupport_ : خطأ; }

صحيح Device::maxClockFrequency() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحيح Device::maxComputeUnits() ثابت
{ أرجع p ? p->maxComputeUnits_ : 0; }

صحيح Device::max_Constant_آرگومانها() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CONSTANT_آرگومانها) : 0; }

size_t Device::max_Constant_میانگیر_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_میانگیر_اندازه) : 0; }

size_t Device::max_Mem_Alloc_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_اندازه) : 0; }

size_t Device::max_Parameter_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_اندازه) : 0; }

صحيح Device::max_Read_Image_آرگومانها() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_READ_IMAGE_آرگومانها) : 0; }

صحيح Device::max_Write_Image_آرگومانها() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WRITE_IMAGE_آرگومانها) : 0; }

صحيح Device::maxSamplers() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_گروه_اندازه() ثابت
{ أرجع p ? p->max_Work_گروه_اندازه_2 : 0; }

صحيح Device::max_Work_قلم_Dims() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WORK_قلم_DIMENSIONS) : 0; }

فراغ Device::max_Work_قلم_Sizes(size_t* sizes) ثابت
{
    إذا(p)
    {
        ثابت صحيح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_قلم_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحيح Device::mem_پایه_Addr_Align() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MEM_پایه_ADDR_ALIGN) : 0; }

صحيح Device::native_وکتور_عرض_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_CHAR) : 0; }

صحيح Device::native_وکتور_عرض_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_SHORT) : 0; }

صحيح Device::native_وکتور_عرض_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_INT) : 0; }

صحيح Device::native_وکتور_عرض_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_LONG) : 0; }

صحيح Device::native_وکتور_عرض_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_FLOAT) : 0; }

صحيح Device::native_وکتور_عرض_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_DOUBLE) : 0; }

صحيح Device::native_وکتور_عرض_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_وکتور_عرض_HALF) : 0; }

صحيح Device::preferred_وکتور_عرض_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_CHAR) : 0; }

صحيح Device::preferred_وکتور_عرض_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_SHORT) : 0; }

صحيح Device::preferred_وکتور_عرض_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_INT) : 0; }

صحيح Device::preferred_وکتور_عرض_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_LONG) : 0; }

صحيح Device::preferred_وکتور_عرض_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_FLOAT) : 0; }

صحيح Device::preferred_وکتور_عرض_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_DOUBLE) : 0; }

صحيح Device::preferred_وکتور_عرض_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_وکتور_عرض_HALF) : 0; }

size_t Device::printf_میانگیر_اندازه() ثابت
#اگر_تعریف_شده CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_میانگیر_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#پایان_اگر


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

قالب <نام_نوع Functor, نام_نوع ObjectType>
مضمن cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::size_t required;
    cl_int err = f(obj, name, 0, عدم, &required);
    إذا (err != CL_SUCCESS)
        أرجع err;

    param.clear();
    إذا (required > 0)
    {
        Auto_میانگیر<حرف> buf(required + 1);
        حرف* ptr = buf.داده(); // cleanup is not needed
        err = f(obj, name, required, ptr, عدم);
        إذا (err != CL_SUCCESS)
            أرجع err;
        param = ptr;
    }

    أرجع CL_SUCCESS;
}

ساكن فراغ تقسیم(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    إذا (s.size() == 0)
        أرجع;
    std::istringstream ss(s);
    std::string قلم;
    طالما (!ss.eof())
    {
        std::getline(ss, قلم, delim);
        elems.push_back(قلم);
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
    تقسیم(configurationStr, ':', parts);
    إذا (parts.size() > 3)
    {
        CV_LOG_ERROR(عدم, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        أرجع خطأ;
    }
    إذا (parts.size() > 2)
        deviceNameOrID = parts[2];
    إذا (parts.size() > 1)
    {
        تقسیم(parts[1], '|', deviceTypes);
    }
    إذا (parts.size() > 0)
    {
        platform = parts[0];
    }
    أرجع درست;
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
        isID = درست;
        لكل (size_t i = 0; i < deviceName.length(); i++)
        {
            إذا (!آیا_رقم(deviceName[i]))
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
        cl_uint رقم_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &رقم_Platforms));

        إذا (رقم_Platforms == 0)
            أرجع عدم;
        platforms.resize((size_t)رقم_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(رقم_Platforms, &platforms[0], &رقم_Platforms));
        platforms.resize(رقم_Platforms);
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
            اذهب not_یافت;
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
        std::string موقت_Str_Device_Type = deviceTypes[t];
        std::transform(موقت_Str_Device_Type.begin(), موقت_Str_Device_Type.end(), موقت_Str_Device_Type.begin(), details::char_tolower);

        إذا (موقت_Str_Device_Type == "gpu" || موقت_Str_Device_Type == "dgpu" || موقت_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        وإلا إذا (موقت_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        وإلا إذا (موقت_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وإلا إذا (موقت_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        وإلا
        {
            CV_LOG_ERROR(عدم, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            اذهب not_یافت;
        }

        std::vector<cl_device_id> devices;
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint شمار_2 = 0;
            cl_int وضعیت = clGetDeviceIDs(*currentPlatform, deviceType, 0, عدم, &شمار_2);
            إذا (!(وضعیت == CL_SUCCESS || وضعیت == CL_DEVICE_NOT_یافت))
            {
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, "clGetDeviceIDs get count");
            }
            إذا (شمار_2 == 0)
                استمر;
            size_t پایه = devices.size();
            devices.resize(پایه + شمار_2);
            وضعیت = clGetDeviceIDs(*currentPlatform, deviceType, شمار_2, &devices[پایه], &شمار_2);
            إذا (!(وضعیت == CL_SUCCESS || وضعیت == CL_DEVICE_NOT_یافت))
            {
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, "clGetDeviceIDs get IDs");
            }
        }

        لكل (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : درست) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = درست;
            إذا(موقت_Str_Device_Type == "dgpu" || موقت_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_حافظه, حجم(isIGPU), &isIGPU, عدم));
                useGPU = موقت_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            إذا ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                أرجع devices[i];
            }
        }
    }

not_یافت:
    إذا (configuration.empty())
        أرجع عدم; // suppress messages on stderr

    std::ostringstream پیام;
    پیام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::پایان_سطر
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::پایان_سطر
        << "    Device types:";
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
        پیام << ' ' << deviceTypes[t];

    پیام << std::پایان_سطر << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(عدم, پیام.str());
    أرجع عدم;
}

#اگر_تعریف_شده HAVE_OPENCL_SVM
نطاق svm {

تعداد AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_میانگیر = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_میانگیر = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_میانگیر_ماسک = 3 << 16,
        OPENCL_SVM_میانگیر_MAP = 4 << 16
};

ساكن منطقي checkForceSVMUmatUsage()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", خطأ);
        initialized = درست;
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
        initialized = درست;
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
        initialized = درست;
    }
    أرجع force;
}
// see SVMCapabilities
ساكن بیعلامت صحيح get_SVMCapabilities_ماسک()
{
    ساكن منطقي initialized = خطأ;
    ساكن بیعلامت صحيح ماسک = 0;
    إذا (!initialized)
    {
        ثابت std::string env_مقدار = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        إذا (env_مقدار.empty())
        {
            أرجع ~0U; // all bits 1
        }
        ماسک = atoi(env_مقدار.c_str());
        initialized = درست;
    }
    أرجع ماسک;
}
} // namespace
#پایان_اگر

ساكن size_t get_برنامه_شمار_حد()
{
    ساكن منطقي initialized = خطأ;
    ساكن size_t شمار_2 = 0;
    إذا (!initialized)
    {
        شمار_2 = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = درست;
    }
    أرجع شمار_2;
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
#اگر_تعریف_شده HAVE_OPENCL_SVM
        , svmInitialized(خطأ)
#پایان_اگر
    {
        إذا (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_قفل lock(cv::getInitializationMutex());
        خودکار& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = هذا;
    }

    ~Impl()
    {
#اگر_تعریف_شده _WIN32
        إذا (!cv::__termination)
#پایان_اگر
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
            cv::Auto_قفل lock(cv::getInitializationMutex());
            خودکار& container = getGlobalContainer();
            CV_CheckLT((size_t)contextId, container.size(), "");
            container[contextId] = عدم;
        }
    }

    فراغ init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_رقم_DEVICES, حجم(ndevices), &ndevices, عدم));
        CV_Assert(ndevices > 0);

        cv::Auto_میانگیر<cl_device_id> cl_devices(ndevices);
        size_t devices_ret_اندازه = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_اندازه));
        CV_CheckEQ(devices_ret_اندازه, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        لكل (بیعلامت i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    فراغ __init_buffer_pools();  // w/o synchronization
    فراغ init_میانگیر_pools() ثابت
    {
        إذا (!میانگیر_Pool_2)
        {
            cv::Auto_قفل lock(cv::getInitializationMutex());
            إذا (!میانگیر_Pool_2)
            {
                const_cast<Impl*>(هذا)->__init_buffer_pools();
            }
        }
    }
عام:
    ساكن Impl* پیدا_کن_Context(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_قفل lock(cv::getInitializationMutex());
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

    ساكن Impl* پیدا_کن_Or_Create_Context(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        إذا (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            إذا (!c.empty())
                configuration = c;
        }
        Impl* impl = پیدا_کن_Context(configuration);
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
            حذف impl;
            أرجع عدم;
        }
        امسك (...)
        {
            حذف impl;
            ارم;
        }
    }

    ساكن Impl* پیدا_کن_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (فراغ*)h);
        Impl* impl = پیدا_کن_Context(configuration);
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
            حذف impl;
            ارم;
        }
    }

    ساكن Impl* پیدا_کن_Or_Create_Context(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (فراغ*)d);
        Impl* impl = پیدا_کن_Context(configuration);
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
            حذف impl;
            ارم;
        }
    }

    فراغ مجموعه_Default()
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
        cl_int وضعیت;

        handle = clCreateContext(prop, nd, &d, 0, 0, &وضعیت);
        CV_OCL_DBG_CHECK_نتیجه(وضعیت, "clCreateContext");

        منطقي ok = handle != 0 && وضعیت == CL_SUCCESS;
        إذا( ok )
        {
            devices.resize(nd);
            devices[0].مجموعه(d);
        }
        وإلا
            handle = عدم;
    }

    برنامه_2 getProg(ثابت برنامه_منبع& src, ثابت String& buildflags, String& errmsg);

    فراغ unloadProg(برنامه_2& prog)
    {
        cv::Auto_قفل lock(برنامه_cache_mutex);
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

    std::string& get_پیشوند_String()
    {
        إذا (پیشوند.empty())
        {
            cv::Auto_قفل lock(برنامه_cache_mutex);
            إذا (پیشوند.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحيح بیتها_2 = d.آدرس_بیتها();
                إذا (بیتها_2 > 0 && بیتها_2 != 64)
                    پیشوند = cv::format("%d-bit--", بیتها_2);
                پیشوند += d.vendorName() + "--" + d.name() + "--" + d.driver_نسخه();
                // sanitize chars
                لكل (size_t i = 0; i < پیشوند.size(); i++)
                {
                    حرف c = پیشوند[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        پیشوند[i] = '_';
                    }
                }
            }
        }
        أرجع پیشوند;
    }

    std::string& get_پیشوند_پایه()
    {
        إذا (پیشوند_پایه.empty())
        {
            cv::Auto_قفل lock(برنامه_cache_mutex);
            إذا (پیشوند_پایه.empty())
            {
                ثابت Device& d = devices[0];
                صحيح بیتها_2 = d.آدرس_بیتها();
                إذا (بیتها_2 > 0 && بیتها_2 != 64)
                    پیشوند_پایه = cv::format("%d-bit--", بیتها_2);
                پیشوند_پایه += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                لكل (size_t i = 0; i < پیشوند_پایه.size(); i++)
                {
                    حرف c = پیشوند_پایه[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        پیشوند_پایه[i] = '_';
                    }
                }
            }
        }
        أرجع پیشوند_پایه;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحيح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string پیشوند;
    std::string پیشوند_پایه;

    cv::Mutex برنامه_cache_mutex;
    تعريفنوع std::map<std::string, برنامه_2> phash_t;
    phash_t phash;
    تعريفنوع std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> میانگیر_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> میانگیر_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_میانگیر_Pool() ثابت
    {
        init_میانگیر_pools();
        CV_DbgAssert(میانگیر_Pool_2);
        أرجع *میانگیر_Pool_2.get();
    }
    OpenCLBufferPoolImpl& get_میانگیر_Pool_Host_Ptr() ثابت
    {
        init_میانگیر_pools();
        CV_DbgAssert(میانگیر_Pool_Host_Ptr);
        أرجع *میانگیر_Pool_Host_Ptr.get();
    }

    std::map<std::type_شاخص, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    فراغ مجموعه_User_Context(std::type_شاخص typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::Auto_قفل lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_شاخص typeId) {
        cv::Auto_قفل lock(userContextMutex);
        خودکار it = userContextStorage.find(typeId);
        إذا (it != userContextStorage.end())
            أرجع it->second;
        وإلا
            أرجع اشاره_تهی;
    }

#اگر_تعریف_شده HAVE_OPENCL_SVM
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
        cl_int وضعیت = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, حجم(deviceCaps), &deviceCaps, عدم);
        إذا (وضعیت != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", وضعیت);
            اذهب noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (صحيح)deviceCaps);
        CV_Assert(((فراغ)0, CL_DEVICE_SVM_COARSE_GRAIN_میانگیر == CL_DEVICE_SVM_COARSE_GRAIN_میانگیر_AMD)); // Check assumption
        svmCapabilities.مقدار_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_میانگیر) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_میانگیر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_میانگیر) ? svm::SVMCapabilities::SVM_FINE_GRAIN_میانگیر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.مقدار_2 &= svm::get_SVMCapabilities_ماسک();
        إذا (svmCapabilities.مقدار_2 == 0)
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
                cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();
                إذا (CL_SUCCESS != cl_افزودن_به_صف_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_پایان(q);
                حاول
                {
                    ((صحيح*)ptr)[0] = 100;
                }
                امسك (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = درست;
                }
                إذا (CL_SUCCESS != cl_افزودن_به_صف_SVMUnmap(q, ptr, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_پایان(q);
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
            svmFunctions.fn_cl_مجموعه_Kernel_Arg_SVMPointer = cl_مجموعه_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_افزودن_به_صف_SVMMemcpy = cl_افزودن_به_صف_SVMMemcpy;
            svmFunctions.fn_cl_افزودن_به_صف_SVMMem_Fill = cl_افزودن_به_صف_SVMMem_Fill;
            svmFunctions.fn_cl_افزودن_به_صف_SVMMap = cl_افزودن_به_صف_SVMMap;
            svmFunctions.fn_cl_افزودن_به_صف_SVMUnmap = cl_افزودن_به_صف_SVMUnmap;
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
                CV_OCL_CHECK(وضعیت = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, حجم(cl_platform_id), &p, عدم));
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_مجموعه_Kernel_Arg_SVMPointer = (cl_مجموعه_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_افزودن_به_صف_SVMMemcpy = (cl_افزودن_به_صف_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_افزودن_به_صف_SVMMem_Fill = (cl_افزودن_به_صف_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_افزودن_به_صف_SVMMap = (cl_افزودن_به_صف_SVMMap_AMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_افزودن_به_صف_SVMUnmap = (cl_افزودن_به_صف_SVMUnmap_AMD_fn)cl_Get_Extension_Function_آدرس_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                اذهب noSVM;
            }
        }

        svm_موجود = درست;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = درست;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        أرجع;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_موجود = خطأ;
        svmEnabled = خطأ;
        svmCapabilities.مقدار_2 = 0;
        svmInitialized = درست;
        svmFunctions.fn_clSVMAlloc = عدم;
        أرجع;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> میانگیر_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_میانگیر_Pool_SVM() ثابت
    {
        init_میانگیر_pools();
        CV_DbgAssert(میانگیر_Pool_SVM);
        أرجع *میانگیر_Pool_SVM.get();
    }
#پایان_اگر

    دوست صنف برنامه_2;
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
    p = Impl::پیدا_کن_Or_Create_Context(std::string());
    إذا (p && p->handle)
        أرجع درست;
    release();
    أرجع خطأ;
}

// deprecated
منطقي Context::create(صحيح dtype)
{
    إذا( !haveOpenCL() )
        أرجع خطأ;
    release();
    إذا (dtype == CL_DEVICE_TYPE_DEFAULT || (بیعلامت)dtype == (بیعلامت)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::پیدا_کن_Or_Create_Context("");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::پیدا_کن_Or_Create_Context(":GPU:");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::پیدا_کن_Or_Create_Context(":CPU:");
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

Context& Context::عملگر = (ثابت Context& c)
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
    c.p = اشاره_تهی;
}

Context& Context::عملگر = (Context&& c) CV_NOEXCEPT
{
    إذا (هذا != &c) {
        إذا(p)
            p->release();
        p = c.p;
        c.p = اشاره_تهی;
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

برنامه_2 Context::getProg(ثابت برنامه_منبع& prog,
                         ثابت String& buildopts, String& errmsg)
{
    أرجع p ? p->getProg(prog, buildopts, errmsg) : برنامه_2();
}

فراغ Context::unloadProg(برنامه_2& prog)
{
    إذا (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(فراغ* context)
{
    Context ctx;
    ctx.p = Impl::پیدا_کن_Or_Create_Context((cl_context)context);
    أرجع ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::پیدا_کن_Or_Create_Context(device);
    أرجع ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::پیدا_کن_Or_Create_Context(configuration);
    أرجع ctx;
}

فراغ* Context::getOpenCLContextProperty(صحيح propertyId) ثابت
{
    إذا (p == عدم)
        أرجع اشاره_تهی;
    ::size_t size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, عدم, &size));
    std::vector<cl_context_properties> prop(size / حجم(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.داده(), عدم));
    لكل (size_t i = 0; i < prop.size(); i += 2)
    {
        إذا (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(عدم, "OpenCL: found context property=" << propertyId << ") => " << (فراغ*)prop[i + 1]);
            أرجع (فراغ*)prop[i + 1];
        }
    }
    أرجع اشاره_تهی;
}

#اگر_تعریف_شده HAVE_OPENCL_SVM
منطقي Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmEnabled;
}
فراغ Context::مجموعه_Use_SVM(منطقي enabled)
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
فراغ Context::مجموعه_Use_SVM(منطقي enabled) { CV_Assert(!enabled); }
#پایان_اگر

#اگر_تعریف_شده HAVE_OPENCL_SVM
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
        أرجع درست;
    إذا (checkDisableSVMUMatUsage())
        أرجع خطأ;
    إذا ((usageFlags & USAGE_ALLOCATE_SHARED_حافظه) != 0)
        أرجع درست;
    أرجع خطأ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#پایان_اگر // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

فراغ Context::مجموعه_User_Context(std::type_شاخص typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->مجموعه_User_Context(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_شاخص typeId)
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
    Auto_میانگیر<حرف> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.داده(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.داده();
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

    منطقي platform_موجود = خطأ;

    // check if external platformName contained in list of available platforms in OpenCV
    لكل (بیعلامت صحيح i = 0; i < cnt; i++)
    {
        String موجود_Platform_Name;
        get_platform_name(platforms[i], موجود_Platform_Name);
        // external platform is found in the list of available platforms
        إذا (platformName == موجود_Platform_Name)
        {
            platform_موجود = درست;
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

    خودکار clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

بنية صف::Impl
{
    مضمن فراغ __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_صف = خطأ;
    }

    Impl(cl_command_صف q)
    {
        __init();
        handle = q;

        cl_command_صف_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_صف_Info(handle, CL_صف_PROPERTIES, حجم(cl_command_صف_properties), &props, عدم));
        is_Profiling_صف = !!(props & CL_صف_PROFILING_ENABLE);
    }

    Impl(cl_command_صف q, منطقي is_Profiling_صف_2)
    {
        __init();
        handle = q;
        is_Profiling_صف = is_Profiling_صف_2;
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
        cl_command_صف_properties props = withProfiling ? CL_صف_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_صف(ch, dh, props, &retval), retval);
        is_Profiling_صف = withProfiling;
    }

    ~Impl()
    {
#اگر_تعریف_شده _WIN32
        إذا (!cv::__termination)
#پایان_اگر
        {
            إذا(handle)
            {
                CV_OCL_DBG_CHECK(cl_پایان(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_صف(handle));
                handle = عدم;
            }
        }
    }

    ثابت cv::ocl::صف& get_Profiling_صف(ثابت cv::ocl::صف& self)
    {
        إذا (is_Profiling_صف)
            أرجع self;

        إذا (profiling_صف.ptr())
            أرجع profiling_صف;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_صف_Info(handle, CL_صف_CONTEXT, حجم(cl_context), &ctx, عدم));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_صف_Info(handle, CL_صف_DEVICE, حجم(cl_device_id), &device, عدم));

        cl_int نتیجه = CL_SUCCESS;
        cl_command_صف_properties props = CL_صف_PROFILING_ENABLE;
        cl_command_صف q = cl_Create_Command_صف(ctx, device, props, &نتیجه);
        CV_OCL_DBG_CHECK_نتیجه(نتیجه, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        صف صف_2;
        صف_2.p = جديد Impl(q, درست);
        profiling_صف = صف_2;

        أرجع profiling_صف;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_صف handle;
    منطقي is_Profiling_صف;
    cv::ocl::صف profiling_صف;
};

صف::صف() CV_NOEXCEPT
{
    p = 0;
}

صف::صف(ثابت Context& c, ثابت Device& d)
{
    p = 0;
    create(c, d);
}

صف::صف(ثابت صف& q)
{
    p = q.p;
    إذا(p)
        p->addref();
}

صف& صف::عملگر = (ثابت صف& q)
{
    Impl* newp = (Impl*)q.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

صف::صف(صف&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = اشاره_تهی;
}

صف& صف::عملگر = (صف&& q) CV_NOEXCEPT
{
    إذا (هذا != &q) {
        إذا(p)
            p->release();
        p = q.p;
        q.p = اشاره_تهی;
    }
    أرجع *هذا;
}

صف::~صف()
{
    إذا(p)
        p->release();
}

منطقي صف::create(ثابت Context& c, ثابت Device& d)
{
    إذا(p)
        p->release();
    p = جديد Impl(c, d);
    أرجع p->handle != 0;
}

فراغ صف::پایان_2()
{
    إذا(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_پایان(p->handle));
    }
}

ثابت صف& صف::get_Profiling_صف() ثابت
{
    CV_Assert(p);
    أرجع p->get_Profiling_صف(*هذا);
}

فراغ* صف::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

صف& صف::getDefault()
{
    خودکار& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خودکار& q = c.get_صف();
        أرجع q;
    }
    ساكن صف dummy;
    أرجع dummy;
}

ساكن cl_command_صف get_صف(ثابت صف& q)
{
    cl_command_صف qq = (cl_command_صف)q.ptr();
    إذا(!qq)
        qq = (cl_command_صف)صف::getDefault().ptr();
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
    أرجع KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.مجموع()*m.elem_اندازه());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

بنية Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت برنامه_2& prog) :
        refcount(1), handle(عدم), is_درون_پیشرفت(خطأ), is_Async_پیاپی(خطأ), nu(0)
    {
        cl_برنامه ph = (cl_برنامه)prog.ptr();
        cl_int retval = 0;
        name = kname;
        إذا (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_موقت_Dst_UMats = خطأ;
        have_موقت_Src_UMats = خطأ;
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
                    u[i]->flags |= UMat_داده::ASYNC_CLEANUP;
                    حاول
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    امسك(ثابت std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(عدم, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = درست;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_موقت_Dst_UMats = خطأ;
        have_موقت_Src_UMats = خطأ;
        CV_Assert(!exceptionOccurred);
    }

    فراغ جمع_کن_UMat(ثابت UMat& m, منطقي dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        إذا(dst && m.u->موقت_UMat())
            have_موقت_Dst_UMats = درست;
        إذا(m.u->original_UMat_داده == عدم && m.u->موقت_UMat())
            have_موقت_Src_UMats = درست;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    فراغ ثبات_Image_آرگومان(صحيح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        إذا (arg < (صحيح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_درون_پیشرفت, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    فراغ finit(cl_event e)
    {
        CV_استفادهنشده(e);
        is_درون_پیشرفت = خطأ;
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

    منطقي پیاپی(صحيح dims, size_t _globalsize[], size_t _localsize[],
            منطقي متزامن, int64* زمان_NS, ثابت صف& q);

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
    UMat_داده* u[MAX_ARRS];
    منطقي is_درون_پیشرفت;
    منطقي is_Async_پیاپی;  // true if kernel was scheduled in async mode
    صحيح nu;
    std::vector<Image2D> shadow_images;
    منطقي have_موقت_Dst_UMats;
    منطقي have_موقت_Src_UMats;
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

Kernel::Kernel(ثابت حرف* kname, ثابت برنامه_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت حرف* kname, ثابت برنامه_منبع& src,
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

Kernel& Kernel::عملگر = (ثابت Kernel& k)
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
    k.p = اشاره_تهی;
}

Kernel& Kernel::عملگر = (Kernel&& k) CV_NOEXCEPT
{
    إذا (هذا != &k) {
        إذا(p)
            p->release();
        p = k.p;
        k.p = اشاره_تهی;
    }
    أرجع *هذا;
}

Kernel::~Kernel()
{
    إذا(p)
        p->release();
}

منطقي Kernel::create(ثابت حرف* kname, ثابت برنامه_2& prog)
{
    إذا(p)
        p->release();
    p = جديد Impl(kname, prog);
    إذا(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#اگر_تعریف_شده CV_OPENCL_پیاپی_ASSERT // check kernel compilation fails
    CV_Assert(p);
#پایان_اگر
    أرجع p != 0;
}

منطقي Kernel::create(ثابت حرف* kname, ثابت برنامه_منبع& src,
                    ثابت String& buildopts, String* errmsg)
{
    إذا(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    إذا( !errmsg ) errmsg = &tempmsg;
    ثابت برنامه_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
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

ساكن cv::String dump_مقدار(size_t sz, ثابت فراغ* p)
{
    إذا (!p)
        أرجع "NULL";
    إذا (sz == 2)
        أرجع cv::format("%d / %uu / 0x%04x", *(کوتاه*)p, *(بیعلامت کوتاه*)p, *(کوتاه*)p);
    إذا (sz == 4)
        أرجع cv::format("%d / %uu / 0x%08x / %g", *(صحيح*)p, *(صحيح*)p, *(صحيح*)p, *(عائم*)p);
    إذا (sz == 8)
        أرجع cv::format("%lld / %lluu / 0x%16llx / %g", *(دراز دراز*)p, *(دراز دراز*)p, *(دراز دراز*)p, *(مزدوج*)p);
    أرجع cv::format("%p", p);
}

صحيح Kernel::مجموعه(صحيح i, ثابت فراغ* value, size_t sz)
{
    إذا (!p || !p->handle)
        أرجع -1;
    إذا (i < 0)
        أرجع i;
    إذا( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحيح)i, (صحيح)sz, dump_مقدار(sz, value).c_str()).c_str());
    إذا (retval != CL_SUCCESS)
        أرجع -1;
    أرجع i+1;
}

صحيح Kernel::مجموعه(صحيح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحيح res = مجموعه(i, &h, حجم(h));
    إذا (res >= 0)
        p->ثبات_Image_آرگومان(i, image2D);
    أرجع res;
}

صحيح Kernel::مجموعه(صحيح i, ثابت UMat& m)
{
    أرجع مجموعه(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساكن منطقي isCudaBacked(ثابت UMat_داده* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    أرجع u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحيح Kernel::مجموعه(صحيح i, ثابت KernelArg& arg)
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
    cl_int وضعیت = 0;
    إذا( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        منطقي ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        إذا (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)عدم;
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحيح)i).c_str());
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

#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_داده_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_داده_Ptr);
#إذا 1 // TODO
            وضعیت = svmFns->fn_cl_مجموعه_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_داده_Ptr);
#وإلا
            وضعیت = svmFns->fn_cl_مجموعه_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_داده_Ptr);
#پایان_اگر
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحيح)i, (فراغ*)svm_داده_Ptr).c_str());
        }
        وإلا
#پایان_اگر
        {
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحيح)i, (فراغ*)h).c_str());
        }

        إذا (ptronly)
        {
            i++;
        }
        وإلا إذا( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u2d.گام), &u2d.گام);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u2d.گام).c_str());
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u2d.offset).c_str());
            i += 3;

            إذا( !(arg.flags & KernelArg::NO_اندازه) )
            {
                صحيح تعداد_ستون = u2d.تعداد_ستون*arg.wscale/arg.iwscale;
                وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, حجم(u2d.ردیفها), &u2d.ردیفها);
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u2d.ردیفها).c_str());
                وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(تعداد_ستون), &تعداد_ستون);
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)تعداد_ستون).c_str());
                i += 2;
            }
        }
        وإلا
        {
            UMat3D u3d(*arg.m);
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.slicestep).c_str());
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.گام), &u3d.گام);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u3d.گام).c_str());
            وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+3), (صحيح)u3d.offset).c_str());
            i += 4;
            إذا( !(arg.flags & KernelArg::NO_اندازه) )
            {
                صحيح تعداد_ستون = u3d.تعداد_ستون*arg.wscale/arg.iwscale;
                وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u3d.slices).c_str());
                وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.ردیفها), &u3d.ردیفها);
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.ردیفها).c_str());
                وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.تعداد_ستون), &تعداد_ستون);
                CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)تعداد_ستون).c_str());
                i += 3;
            }
        }
        p->جمع_کن_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        أرجع i;
    }
    وضعیت = cl_مجموعه_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_نتیجه(وضعیت, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحيح)i, (صحيح)arg.sz, (فراغ*)arg.obj).c_str());
    أرجع i+1;
}

منطقي Kernel::پیاپی(صحيح dims, size_t _globalsize[], size_t _localsize[],
                 منطقي متزامن, ثابت صف& q)
{
    إذا (!p)
        أرجع خطأ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t مجموع = 1;
    CV_Assert(_globalsize != عدم);
    لكل (صحيح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحيح)(i>0)) : 1;
        CV_Assert( val > 0 );
        مجموع *= _globalsize[i];
        إذا (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_به_بالا(_globalsize[i], (بیعلامت صحيح)val) * val;
    }
    CV_Assert(مجموع > 0);

    أرجع p->پیاپی(dims, globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::پیاپی_2(صحيح dims, size_t _globalsize[], size_t _localsize[],
                  منطقي متزامن, ثابت صف& q)
{
    CV_Assert(p);
    أرجع p->پیاپی(dims, _globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::Impl::پیاپی(صحيح dims, size_t globalsize[], size_t localsize[],
        منطقي متزامن, int64* زمان_NS, ثابت صف& q)
{
    CV_INSTRUMENT_REGION_OPENCL_پیاپی(name.c_str());

    إذا (!handle)
    {
        CV_LOG_ERROR(عدم, "OpenCL kernel has zero handle: " << name);
        أرجع خطأ;
    }

    إذا (is_Async_پیاپی)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_پیاپی = !متزامن;

    إذا (is_درون_پیشرفت)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#إذا CV_OPENCL_SYNC_پیاپی_KERNELS
    متزامن = درست;
#پایان_اگر

    cl_command_صف qq = get_صف(q);
    إذا (have_موقت_Dst_UMats)
        متزامن = درست;
    إذا (have_موقت_Src_UMats)
        متزامن = درست;
    إذا (زمان_NS)
        متزامن = درست;
    cl_event asyncEvent = 0;
    cl_int retval = cl_افزودن_به_صف_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           عدم, globalsize, localsize, 0, 0,
                                           (متزامن && !زمان_NS) ? 0 : &asyncEvent);
#إذا !CV_OPENCL_SHOW_پیاپی_KERNELS
    إذا (retval != CL_SUCCESS)
#پایان_اگر
    {
        cv::String پیام = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (صحيح)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        متزامن ? "true" : "false"
                        );
        إذا (retval != CL_SUCCESS)
        {
            پیام = CV_OCL_API_ERROR_پیام(retval, پیام.c_str());
        }
#إذا CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_نتیجه(retval, پیام.c_str());
#وإلا
        چاپ_کن("%s\n", پیام.c_str());
        fflush(stdout);
#پایان_اگر
    }
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پایان(qq));
        إذا (زمان_NS)
        {
            إذا (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong شروع_زمان, stop_زمان;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_شروع, حجم(شروع_زمان), &شروع_زمان, عدم));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_انتها, حجم(stop_زمان), &stop_زمان, عدم));
                *زمان_NS = (int64)(stop_زمان - شروع_زمان);
            }
            وإلا
            {
                *زمان_NS = -1;
            }
        }
        cleanupUMats();
    }
    وإلا
    {
        addref();
        is_درون_پیشرفت = درست;
        CV_OCL_CHECK(cl_مجموعه_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, هذا));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

منطقي Kernel::پیاپی_Task(منطقي متزامن, ثابت صف& q)
{
    إذا(!p || !p->handle || p->is_درون_پیشرفت)
        أرجع خطأ;

    cl_command_صف qq = get_صف(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_افزودن_به_صف_Task(qq, p->handle, 0, 0, متزامن ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), متزامن ? "true" : "false").c_str());
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پایان(qq));
        p->cleanupUMats();
    }
    وإلا
    {
        p->addref();
        p->is_درون_پیشرفت = درست;
        CV_OCL_CHECK(cl_مجموعه_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

int64 Kernel::پیاپی_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت صف& q_)
{
    CV_Assert(p && p->handle && !p->is_درون_پیشرفت);
    صف q = q_.ptr() ? q_ : صف::getDefault();
    CV_Assert(q.ptr());
    q.پایان_2(); // call clFinish() on base queue
    صف profiling_صف_2 = q.get_Profiling_صف();
    int64 زمان_Ns = -1;
    منطقي res = p->پیاپی(dims, globalsize, localsize, درست, &زمان_Ns, profiling_صف_2);
    أرجع res ? زمان_Ns : -1;
}

size_t Kernel::work_گروه_اندازه() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int وضعیت = cl_Get_Kernel_Work_گروه_Info(p->handle, dev, CL_KERNEL_WORK_گروه_اندازه, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجه(وضعیت, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    أرجع وضعیت == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_گروه_اندازه_مضرب() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int وضعیت = cl_Get_Kernel_Work_گروه_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_گروه_اندازه_مضرب, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجه(وضعیت, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    أرجع وضعیت == CL_SUCCESS ? val : 0;
}

منطقي Kernel::compile_Work_گروه_اندازه(size_t wsz[]) ثابت
{
    إذا(!p || !p->handle || !wsz)
        أرجع 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int وضعیت = cl_Get_Kernel_Work_گروه_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_گروه_اندازه, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_نتیجه(وضعیت, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    أرجع وضعیت == CL_SUCCESS;
}

size_t Kernel::local_Mem_اندازه() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int وضعیت = cl_Get_Kernel_Work_گروه_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_اندازه, حجم(val), &val, &retsz);
    CV_OCL_CHECK_نتیجه(وضعیت, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    أرجع وضعیت == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

بنية برنامه_منبع::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    تعداد نوع_4 {
        برنامه_منبع_کد = 0,
        برنامه_BINARIES,
        برنامه_SPIR,
        برنامه_SPIRV
    } نوع_5;

    Impl(ثابت String& src)
    {
        init(برنامه_منبع_کد, cv::String(), cv::String());
        init_From_منبع(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& کد_Str, ثابت String& کد_Hash)
    {
        init(برنامه_منبع_کد, module, name);
        init_From_منبع(کد_Str, کد_Hash);
    }

    /// reset fields
    فراغ init(تعداد نوع_4 نوع, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        نوع_5 = نوع;
        module_ = module;
        name_ = name;

        منبع_Addr = عدم;
        منبع_اندازه = 0;
        isHashUpdated = خطأ;
    }

    فراغ init_From_منبع(ثابت String& کد_Str, ثابت String& کد_Hash)
    {
        کد_Str_2 = کد_Str;
        منبع_Hash = کد_Hash;
        إذا (منبع_Hash.empty())
        {
            updateHash();
        }
        وإلا
        {
            isHashUpdated = درست;
        }
    }

    فراغ updateHash(ثابت حرف* hashStr = عدم)
    {
        إذا (hashStr)
        {
            منبع_Hash = cv::String(hashStr);
            isHashUpdated = درست;
            أرجع;
        }
        uint64 hash = 0;
        بدل (نوع_5)
        {
        حالة برنامه_منبع_کد:
            إذا (منبع_Addr)
            {
                CV_Assert(کد_Str_2.empty());
                hash = crc64(منبع_Addr, منبع_اندازه); // static storage
            }
            وإلا
            {
                CV_Assert(!کد_Str_2.empty());
                hash = crc64((uchar*)کد_Str_2.c_str(), کد_Str_2.size());
            }
            اكسر;
        حالة برنامه_BINARIES:
        حالة برنامه_SPIR:
        حالة برنامه_SPIRV:
            hash = crc64(منبع_Addr, منبع_اندازه);
            اكسر;
        افتراضي:
            CV_Error(Error::Sts_داخلی, "Internal error");
        }
        منبع_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = درست;
    }

    Impl(تعداد نوع_4 نوع,
            ثابت String& module, ثابت String& name,
            ثابت بیعلامت حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(نوع, module, name);

        منبع_Addr = binary;
        منبع_اندازه = size;

        buildOptions_ = buildOptions;
    }

    ساكن برنامه_منبع from_منبع_With_Static_Lifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* منبع_کد_Static_Str, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        برنامه_منبع نتیجه;
        نتیجه.p = جديد Impl(برنامه_منبع_کد, module, name,
                (ثابت بیعلامت حرف*)منبع_کد_Static_Str, طول_رشته(منبع_کد_Static_Str), buildOptions);
        نتیجه.p->updateHash(hashStaticStr);
        أرجع نتیجه;
    }

    ساكن برنامه_منبع from_دودویی(ثابت String& module, ثابت String& name,
            ثابت بیعلامت حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        برنامه_منبع نتیجه;
        نتیجه.p = جديد Impl(برنامه_BINARIES, module, name, binary, size, buildOptions);
        أرجع نتیجه;
    }

    ساكن برنامه_منبع fromSPIR(ثابت String& module, ثابت String& name,
            ثابت بیعلامت حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        برنامه_منبع نتیجه;
        نتیجه.p = جديد Impl(برنامه_SPIR, module, name, binary, size, buildOptions);
        أرجع نتیجه;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String کد_Str_2; // PROGRAM_SOURCE_CODE only

    ثابت بیعلامت حرف* منبع_Addr;
    size_t منبع_اندازه;

    cv::String buildOptions_;

    String منبع_Hash;
    منطقي isHashUpdated;

    دوست بنية برنامه_2::Impl;
    دوست بنية internal::برنامه_مدخل;
    دوست بنية Context::Impl;
};


برنامه_منبع::برنامه_منبع() CV_NOEXCEPT
{
    p = 0;
}

برنامه_منبع::برنامه_منبع(ثابت String& module, ثابت String& name, ثابت String& کد_Str, ثابت String& کد_Hash)
{
    p = جديد Impl(module, name, کد_Str, کد_Hash);
}

برنامه_منبع::برنامه_منبع(ثابت حرف* prog)
{
    p = جديد Impl(prog);
}

برنامه_منبع::برنامه_منبع(ثابت String& prog)
{
    p = جديد Impl(prog);
}

برنامه_منبع::~برنامه_منبع()
{
    إذا(p)
        p->release();
}

برنامه_منبع::برنامه_منبع(ثابت برنامه_منبع& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

برنامه_منبع& برنامه_منبع::عملگر = (ثابت برنامه_منبع& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

برنامه_منبع::برنامه_منبع(برنامه_منبع&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = اشاره_تهی;
}

برنامه_منبع& برنامه_منبع::عملگر = (برنامه_منبع&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = اشاره_تهی;
    }
    أرجع *هذا;
}

ثابت String& برنامه_منبع::منبع() ثابت
{
    CV_Assert(p);
    CV_Assert(p->نوع_5 == Impl::برنامه_منبع_کد);
    CV_Assert(p->منبع_Addr == عدم); // method returns reference - can't construct temporary object
    أرجع p->کد_Str_2;
}

برنامه_منبع::hash_t برنامه_منبع::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

برنامه_منبع برنامه_منبع::from_دودویی(ثابت String& module, ثابت String& name,
        ثابت بیعلامت حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دودویی(module, name, binary, size, buildOptions);
}

برنامه_منبع برنامه_منبع::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت بیعلامت حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دودویی(module, name, binary, size, buildOptions);
}


internal::برنامه_مدخل::عملگر برنامه_منبع&() ثابت
{
    إذا (هذا->p_برنامه_منبع == عدم)
    {
        cv::Auto_قفل lock(cv::getInitializationMutex());
        إذا (هذا->p_برنامه_منبع == عدم)
        {
            برنامه_منبع ps = برنامه_منبع::Impl::from_منبع_With_Static_Lifetime(هذا->module, هذا->name, هذا->برنامه_کد, هذا->برنامه_Hash, cv::String());
            برنامه_منبع* ptr = جديد برنامه_منبع(ps);
            const_cast<برنامه_مدخل*>(هذا)->p_برنامه_منبع = ptr;
        }
    }
    أرجع *هذا->p_برنامه_منبع;
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

بنية برنامه_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت برنامه_منبع& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(عدم),
         buildflags(_buildflags)
    {
        ثابت برنامه_منبع::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        منبع_Module = src_->module_;
        منبع_Name = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        إذا (ctx.ptr() == عدم || device.ptr() == عدم)
            أرجع;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        إذا (src.getImpl()->نوع_5 == برنامه_منبع::Impl::برنامه_منبع_کد)
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
        CV_LOG_INFO(عدم, "OpenCL program '" << منبع_Module << "/" << منبع_Name << "' options:" << buildflags);
#پایان_اگر
        compile(ctx, src_, errmsg);
#إذا CV_OPENCL_SHOW_BUILD_KERNELS
        إذا (handle)
        {
            size_t retsz = 0;
            حرف kernels_میانگیر[4096] = {0};
            cl_int نتیجه = cl_Get_برنامه_Info(handle, CL_برنامه_KERNEL_نامها, حجم(kernels_میانگیر), &kernels_میانگیر[0], &retsz);
            CV_OCL_DBG_CHECK_نتیجه(نتیجه, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", منبع_Module.c_str(), منبع_Name.c_str()).c_str());
            إذا (نتیجه == CL_SUCCESS && retsz < حجم(kernels_میانگیر))
            {
                kernels_میانگیر[retsz] = 0;
                CV_LOG_INFO(عدم, "OpenCL program '" << منبع_Module << "/" << منبع_Name << "' kernels: '" << kernels_میانگیر << "'");
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, "OpenCL program '" << منبع_Module << "/" << منبع_Name << "' can't retrieve kernel names!");
            }
        }
#پایان_اگر
    }

    منطقي compile(ثابت Context& ctx, ثابت برنامه_منبع::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        إذا (src_->نوع_5 == برنامه_منبع::Impl::برنامه_BINARIES)
        {
            CV_LOG_VERBOSE(عدم, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دودویی(ctx, src_->منبع_Addr, src_->منبع_اندازه, errmsg);
            أرجع isLoaded;
        }
        أرجع compileWithCache(ctx, src_, errmsg);
    }

    منطقي compileWithCache(ثابت Context& ctx, ثابت برنامه_منبع::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->نوع_5 != برنامه_منبع::Impl::برنامه_BINARIES);

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string پایه_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_پیشوند_String(),
                ctx.getImpl()->get_پیشوند_پایه()
        );
        ثابت String& hash_str = src_->منبع_Hash;
        cv::String fname;
        إذا (!پایه_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(پایه_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::File_قفل> file_قفل = config.cache_قفل; // can be empty
        إذا (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            حاول
            {
                std::vector<حرف> دودویی_Buf;
                منطقي res = خطأ;
                {
                    cv::utils::optional_shared_قفل_guard<cv::utils::fs::File_قفل> قفل_fs(file_قفل.get());
                    دودویی_برنامه_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, دودویی_Buf);
                }
                إذا (res)
                {
                    CV_Assert(!دودویی_Buf.empty());
                    CV_LOG_VERBOSE(عدم, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقي isLoaded = create_From_دودویی(ctx, دودویی_Buf, errmsg);
                    إذا (isLoaded)
                        أرجع درست;
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_استفادهنشده(e);
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname << std::پایان_سطر << e.what());
            }
            امسك (...)
            {
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == عدم);
        إذا (src_->نوع_5 == برنامه_منبع::Impl::برنامه_منبع_کد)
        {
            إذا (!buildFromSources(ctx, src_, errmsg))
            {
                أرجع خطأ;
            }
        }
        وإلا إذا (src_->نوع_5 == برنامه_منبع::Impl::برنامه_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            إذا ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(عدم, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دودویی(ctx, src_->منبع_Addr, src_->منبع_اندازه, errmsg);
            إذا (!isLoaded)
                أرجع خطأ;
        }
        وإلا إذا (src_->نوع_5 == برنامه_منبع::Impl::برنامه_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وإلا
        {
            CV_Error(Error::Sts_داخلی, "Internal error");
        }
        CV_Assert(handle != عدم);
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        إذا (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            حاول
            {
                std::vector<حرف> دودویی_Buf;
                get_برنامه_دودویی(دودویی_Buf);
                {
                    cv::utils::optional_قفل_guard<cv::utils::fs::File_قفل> قفل_fs(file_قفل.get());
                    دودویی_برنامه_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, دودویی_Buf);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname << std::پایان_سطر << e.what());
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا CV_OPENCL_VALIDATE_دودویی_PROGRAMS
        إذا (CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
        {
            std::vector<حرف> دودویی_Buf;
            get_برنامه_دودویی(دودویی_Buf);
            إذا (!دودویی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_برنامه(handle));
                handle = عدم;
                create_From_دودویی(ctx, دودویی_Buf, errmsg);
            }
        }
#پایان_اگر
        أرجع handle != عدم;
    }

    فراغ dumpBuildLog_(cl_int نتیجه, ثابت cl_device_id* deviceList, String& errmsg)
    {
        Auto_میانگیر<حرف, 4096> میانگیر; میانگیر[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_برنامه_Build_Info(handle, deviceList[0],
                                                  CL_برنامه_BUILD_LOG, 0, 0, &retsz);
        إذا (log_retval == CL_SUCCESS && retsz > 1)
        {
            میانگیر.resize(retsz + 16);
            log_retval = cl_Get_برنامه_Build_Info(handle, deviceList[0],
                                               CL_برنامه_BUILD_LOG, retsz+1, میانگیر.داده(), &retsz);
            إذا (log_retval == CL_SUCCESS)
            {
                إذا (retsz < میانگیر.size())
                    میانگیر[retsz] = 0;
                وإلا
                    میانگیر[میانگیر.size() - 1] = 0;
            }
            وإلا
            {
                میانگیر[0] = 0;
            }
        }

        errmsg = String(میانگیر.داده());
        چاپ_کن("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                منبع_Module.c_str(), منبع_Name.c_str(),
                نتیجه, getOpenCLErrorString(نتیجه),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    منطقي buildFromSources(ثابت Context& ctx, ثابت برنامه_منبع::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->نوع_5 == برنامه_منبع::Impl::برنامه_منبع_کد);
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                منبع_Module.c_str(), منبع_Name.c_str(),
                src_->منبع_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(عدم, 0, "Compile... " << منبع_Module.c_str() << "/" << منبع_Name.c_str());

        ثابت حرف* srcptr = src_->منبع_Addr ? ((ثابت حرف*)src_->منبع_Addr) : src_->کد_Str_2.c_str();
        size_t srclen = src_->منبع_Addr ? src_->منبع_اندازه : src_->کد_Str_2.size();
        CV_Assert(srcptr != عدم);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_برنامه_With_منبع((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_نتیجه(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        إذا (handle && retval == CL_SUCCESS)
        {
            size_t n = ctx.ndevices();
            Auto_میانگیر<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.داده();
            لكل (size_t i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = cl_Build_برنامه(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_نتیجه(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#إذا !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            إذا (retval != CL_SUCCESS)
#پایان_اگر
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                إذا (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_برنامه(handle));
                    handle = عدم;
                }
                إذا (retval != CL_SUCCESS &&
                    منبع_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#إذا CV_OPENCL_VALIDATE_دودویی_PROGRAMS
            إذا (handle && CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
            {
                CV_LOG_INFO(عدم, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_میانگیر[4096] = {0};
                cl_int نتیجه = cl_Get_برنامه_Info(handle, CL_برنامه_KERNEL_نامها, حجم(kernels_میانگیر), &kernels_میانگیر[0], &retsz);
                إذا (retsz < حجم(kernels_میانگیر))
                    kernels_میانگیر[retsz] = 0;
                وإلا
                    kernels_میانگیر[0] = 0;
                CV_LOG_INFO(عدم, نتیجه << ": Kernels='" << kernels_میانگیر << "'");
            }
#پایان_اگر
        }
        أرجع handle != عدم;
    }

    فراغ get_برنامه_دودویی(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_برنامه_Info(handle, CL_برنامه_دودویی_SIZES, حجم(sz), &sz, عدم));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_برنامه_Info(handle, CL_برنامه_BINARIES, حجم(ptr), &ptr, عدم));
    }

    منطقي create_From_دودویی(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        أرجع create_From_دودویی(ctx, (ثابت بیعلامت حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقي create_From_دودویی(ثابت Context& ctx, ثابت بیعلامت حرف* دودویی_Addr, ثابت size_t دودویی_اندازه, String& errmsg)
    {
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(عدم, 0, "Load from binary... (" << دودویی_اندازه << " bytes)");

        CV_Assert(دودویی_اندازه > 0);

        size_t ndevices = (صحيح)ctx.ndevices();
        Auto_میانگیر<cl_device_id> devices_(ndevices);
        Auto_میانگیر<ثابت uchar*> دودویی_Ptrs(ndevices);
        Auto_میانگیر<size_t> دودویی_Sizes(ndevices);

        cl_device_id* devices = devices_.داده();
        ثابت uchar** دودویی_Ptrs_2 = دودویی_Ptrs.داده();
        size_t* دودویی_Sizes_2 = دودویی_Sizes.داده();
        لكل (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            دودویی_Ptrs_2[i] = دودویی_Addr;
            دودویی_Sizes_2[i] = دودویی_اندازه;
        }

        cl_int نتیجه = 0;
        handle = cl_Create_برنامه_With_دودویی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.داده(),
                                           دودویی_Sizes_2, دودویی_Ptrs_2, عدم, &نتیجه);
        إذا (نتیجه != CL_SUCCESS)
        {
            CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیام(نتیجه, "clCreateProgramWithBinary"));
            إذا (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_برنامه(handle));
                handle = عدم;
            }
        }
        إذا (!handle)
        {
            أرجع خطأ;
        }
        // call clBuildProgram()
        {
            نتیجه = cl_Build_برنامه(handle, (cl_uint)ndevices, devices_.داده(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_نتیجه(نتیجه, cv::format("clBuildProgram(binary: %s/%s)", منبع_Module.c_str(), منبع_Name.c_str()).c_str());
            إذا (نتیجه != CL_SUCCESS)
            {
                dumpBuildLog_(نتیجه, devices, errmsg);
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_برنامه(handle));
                    handle = عدم;
                }
                أرجع خطأ;
            }
        }
        // check build status
        {
            cl_build_وضعیت build_وضعیت = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(نتیجه = cl_Get_برنامه_Build_Info(handle, devices[0], CL_برنامه_BUILD_وضعیت,
                    حجم(build_وضعیت), &build_وضعیت, &retsz));
            إذا (نتیجه == CL_SUCCESS)
            {
                إذا (build_وضعیت == CL_BUILD_SUCCESS)
                {
                    أرجع درست;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "clGetProgramBuildInfo() returns " << build_وضعیت);
                    أرجع خطأ;
                }
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیام(نتیجه, "clGetProgramBuildInfo()"));
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_برنامه(handle));
                    handle = عدم;
                }
            }
        }
#إذا CV_OPENCL_VALIDATE_دودویی_PROGRAMS
        إذا (handle && CV_OPENCL_VALIDATE_دودویی_PROGRAMS_مقدار)
        {
            CV_LOG_INFO(عدم, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_میانگیر[4096] = {0};
            نتیجه = cl_Get_برنامه_Info(handle, CL_برنامه_KERNEL_نامها, حجم(kernels_میانگیر), &kernels_میانگیر[0], &retsz);
            إذا (retsz < حجم(kernels_میانگیر))
                kernels_میانگیر[retsz] = 0;
            وإلا
                kernels_میانگیر[0] = 0;
            CV_LOG_INFO(عدم, نتیجه << ": Kernels='" << kernels_میانگیر << "'");
        }
#پایان_اگر
        أرجع handle != عدم;
    }

    ~Impl()
    {
        إذا( handle )
        {
#اگر_تعریف_شده _WIN32
            إذا (!cv::__termination)
#پایان_اگر
            {
                cl_Release_برنامه(handle);
            }
            handle = عدم;
        }
    }

    cl_برنامه handle;

    String buildflags;
    String منبع_Module;
    String منبع_Name;
};


برنامه_2::برنامه_2() CV_NOEXCEPT
{
    p = 0;
}

برنامه_2::برنامه_2(ثابت برنامه_منبع& src,
        ثابت String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

برنامه_2::برنامه_2(ثابت برنامه_2& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

برنامه_2& برنامه_2::عملگر = (ثابت برنامه_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

برنامه_2::برنامه_2(برنامه_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = اشاره_تهی;
}

برنامه_2& برنامه_2::عملگر = (برنامه_2&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = اشاره_تهی;
    }
    أرجع *هذا;
}

برنامه_2::~برنامه_2()
{
    إذا(p)
        p->release();
}

منطقي برنامه_2::create(ثابت برنامه_منبع& src,
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

فراغ* برنامه_2::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

#اگر_تعریف_نشده OPENCV_REMOVE_DEPRECATED_API
ثابت برنامه_منبع& برنامه_2::منبع() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي برنامه_2::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_استفادهنشده(bin); CV_استفادهنشده(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي برنامه_2::اكتب(String& bin) ثابت
{
    CV_استفادهنشده(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String برنامه_2::get_پیشوند() ثابت
{
    إذا(!p)
        أرجع String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_پیشوند_String().c_str(), p->buildflags.c_str());
}

String برنامه_2::get_پیشوند(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_پیشوند_String().c_str(), buildflags.c_str());
}
#پایان_اگر // OPENCV_REMOVE_DEPRECATED_API

فراغ برنامه_2::get_دودویی(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_برنامه_دودویی(binary);
}

برنامه_2 Context::Impl::getProg(ثابت برنامه_منبع& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t حد = get_برنامه_شمار_حد();
    ثابت برنامه_منبع::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلید = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->منبع_Hash.c_str(),
            get_پیشوند_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_قفل lock(برنامه_cache_mutex);
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
            إذا (حد > 0 && sz >= حد)
            {
                ساكن منطقي warningFlag = خطأ;
                إذا (!warningFlag)
                {
                    چاپ_کن("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = درست;
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
    برنامه_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_قفل lock(برنامه_cache_mutex);
        phash.insert(std::pair<std::string, برنامه_2>(کلید, prog));
        cacheList.push_front(کلید);
    }
    أرجع prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

قالب<نام_نوع T>
صنف OpenCLBufferPool
{
محمي:
    ~OpenCLBufferPool() { }
عام:
    مجازی T allocate(size_t size) = 0;
    مجازی فراغ release(T میانگیر) = 0;
};

قالب <نام_نوع Derived, نام_نوع میانگیر_مدخل, نام_نوع T>
صنف Open_CLBuffer_Pool_پایه_Impl : عام میانگیر_Pool_Controller, عام OpenCLBufferPool<T>
{
خاص:
    مضمن Derived& derived() { أرجع *static_cast<Derived*>(هذا); }
محمي:
    Mutex mutex_;

    size_t current_Reserved_اندازه;
    size_t max_Reserved_اندازه;

    std::list<میانگیر_مدخل> allocatedEntries_; // Allocated and used entries
    std::list<میانگیر_مدخل> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقي پیدا_کن_And_Remove_مدخل_From_Allocated_List(CV_بیرون میانگیر_مدخل& مدخل, T میانگیر)
    {
        نام_نوع std::list<میانگیر_مدخل>::iterator i = allocatedEntries_.begin();
        لكل (; i != allocatedEntries_.end(); ++i)
        {
            میانگیر_مدخل& e = *i;
            إذا (e.cl_میانگیر_2 == میانگیر)
            {
                مدخل = e;
                allocatedEntries_.erase(i);
                أرجع درست;
            }
        }
        أرجع خطأ;
    }

    // synchronized
    منطقي پیدا_کن_And_Remove_مدخل_From_Reserved_List(CV_بیرون میانگیر_مدخل& مدخل, ثابت size_t size)
    {
        إذا (reservedEntries_.empty())
            أرجع خطأ;
        نام_نوع std::list<میانگیر_مدخل>::iterator i = reservedEntries_.begin();
        نام_نوع std::list<میانگیر_مدخل>::iterator نتیجه_pos = reservedEntries_.end();
        میانگیر_مدخل نتیجه;
        size_t minDiff = (size_t)(-1);
        لكل (; i != reservedEntries_.end(); ++i)
        {
            میانگیر_مدخل& e = *i;
            إذا (e.ظرفیت >= size)
            {
                size_t diff = e.ظرفیت - size;
                إذا (diff < std::max((size_t)4096, size / 8) && (نتیجه_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    نتیجه_pos = i;
                    نتیجه = e;
                    إذا (diff == 0)
                        اكسر;
                }
            }
        }
        إذا (نتیجه_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(نتیجه_pos);
            مدخل = نتیجه;
            current_Reserved_اندازه -= مدخل.ظرفیت;
            allocatedEntries_.push_back(مدخل);
            أرجع درست;
        }
        أرجع خطأ;
    }

    // synchronized
    فراغ check_اندازه_از_Reserved_Entries()
    {
        طالما (current_Reserved_اندازه > max_Reserved_اندازه)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت میانگیر_مدخل& مدخل = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_اندازه >= مدخل.ظرفیت);
            current_Reserved_اندازه -= مدخل.ظرفیت;
            derived().release_میانگیر_مدخل(مدخل);
            reservedEntries_.pop_back();
        }
    }

    مضمن size_t تخصیص_Granularity(size_t size)
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
    Open_CLBuffer_Pool_پایه_Impl()
        : current_Reserved_اندازه(0),
          max_Reserved_اندازه(0)
    {
        // nothing
    }
    مجازی ~Open_CLBuffer_Pool_پایه_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عام:
    مجازی T allocate(size_t size) CV_OVERRIDE
    {
        Auto_قفل locker(mutex_);
        میانگیر_مدخل مدخل;
        إذا (max_Reserved_اندازه > 0 && پیدا_کن_And_Remove_مدخل_From_Reserved_List(مدخل, size))
        {
            CV_DbgAssert(size <= مدخل.ظرفیت);
            LOG_میانگیر_POOL("Reuse reserved buffer: %p\n", مدخل.cl_میانگیر_2);
        }
        وإلا
        {
            derived().allocate_میانگیر_مدخل(مدخل, size);
        }
        أرجع مدخل.cl_میانگیر_2;
    }
    مجازی فراغ release(T میانگیر) CV_OVERRIDE
    {
        Auto_قفل locker(mutex_);
        میانگیر_مدخل مدخل;
        CV_Assert(پیدا_کن_And_Remove_مدخل_From_Allocated_List(مدخل, میانگیر));
        إذا (max_Reserved_اندازه == 0 || مدخل.ظرفیت > max_Reserved_اندازه / 8)
        {
            derived().release_میانگیر_مدخل(مدخل);
        }
        وإلا
        {
            reservedEntries_.push_front(مدخل);
            current_Reserved_اندازه += مدخل.ظرفیت;
            check_اندازه_از_Reserved_Entries();
        }
    }

    مجازی size_t get_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع current_Reserved_اندازه; }
    مجازی size_t get_Max_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع max_Reserved_اندازه; }
    مجازی فراغ مجموعه_Max_Reserved_اندازه(size_t size) CV_OVERRIDE
    {
        Auto_قفل locker(mutex_);
        size_t old_Max_Reserved_اندازه = max_Reserved_اندازه;
        max_Reserved_اندازه = size;
        إذا (max_Reserved_اندازه < old_Max_Reserved_اندازه)
        {
            نام_نوع std::list<میانگیر_مدخل>::iterator i = reservedEntries_.begin();
            لكل (; i != reservedEntries_.end();)
            {
                ثابت میانگیر_مدخل& مدخل = *i;
                إذا (مدخل.ظرفیت > max_Reserved_اندازه / 8)
                {
                    CV_DbgAssert(current_Reserved_اندازه >= مدخل.ظرفیت);
                    current_Reserved_اندازه -= مدخل.ظرفیت;
                    derived().release_میانگیر_مدخل(مدخل);
                    i = reservedEntries_.erase(i);
                    استمر;
                }
                ++i;
            }
            check_اندازه_از_Reserved_Entries();
        }
    }
    مجازی فراغ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_قفل locker(mutex_);
        نام_نوع std::list<میانگیر_مدخل>::const_iterator i = reservedEntries_.begin();
        لكل (; i != reservedEntries_.end(); ++i)
        {
            ثابت میانگیر_مدخل& مدخل = *i;
            derived().release_میانگیر_مدخل(مدخل);
        }
        reservedEntries_.clear();
        current_Reserved_اندازه = 0;
    }
};

بنية CLBuffer_مدخل
{
    cl_mem cl_میانگیر_2;
    size_t ظرفیت;
    CLBuffer_مدخل() : cl_میانگیر_2((cl_mem)عدم), ظرفیت(0) { }
};

صنف OpenCLBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_پایه_Impl<OpenCLBufferPoolImpl, CLBuffer_مدخل, cl_mem>
{
عام:
    تعريفنوع بنية CLBuffer_مدخل میانگیر_مدخل;
محمي:
    صحيح createFlags_;
عام:
    OpenCLBufferPoolImpl(صحيح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    فراغ allocate_میانگیر_مدخل(میانگیر_مدخل& مدخل, size_t size)
    {
        CV_DbgAssert(مدخل.cl_میانگیر_2 == عدم);
        مدخل.ظرفیت = align_اندازه(size, (صحيح)تخصیص_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        مدخل.cl_میانگیر_2 = cl_Create_میانگیر((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, مدخل.ظرفیت, 0, &retval);
        CV_OCL_CHECK_نتیجه(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (دراز دراز صحيح)مدخل.ظرفیت, (فراغ*)مدخل.cl_میانگیر_2).c_str());
        CV_Assert(مدخل.cl_میانگیر_2 != عدم);
        إذا(retval == CL_SUCCESS)
        {
            CV_IMPL_جمع_کن(CV_IMPL_OCL);
        }
        LOG_میانگیر_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (دراز دراز)مدخل.ظرفیت, (دراز دراز)مدخل.ظرفیت, مدخل.cl_میانگیر_2);
        allocatedEntries_.push_back(مدخل);
    }

    فراغ release_میانگیر_مدخل(ثابت میانگیر_مدخل& مدخل)
    {
        CV_Assert(مدخل.ظرفیت != 0);
        CV_Assert(مدخل.cl_میانگیر_2 != عدم);
        LOG_میانگیر_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                مدخل.cl_میانگیر_2, (دراز دراز)مدخل.ظرفیت, (دراز دراز)مدخل.ظرفیت);
        CV_OCL_DBG_CHECK(clReleaseMemObject(مدخل.cl_میانگیر_2));
    }
};

#اگر_تعریف_شده HAVE_OPENCL_SVM
بنية CLSVMBuffer_مدخل
{
    فراغ* cl_میانگیر_2;
    size_t ظرفیت;
    CLSVMBuffer_مدخل() : cl_میانگیر_2(عدم), ظرفیت(0) { }
};
صنف OpenCLSVMBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_پایه_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_مدخل, فراغ*>
{
عام:
    تعريفنوع بنية CLSVMBuffer_مدخل میانگیر_مدخل;
عام:
    OpenCLSVMBufferPoolImpl()
    {
    }

    فراغ allocate_میانگیر_مدخل(میانگیر_مدخل& مدخل, size_t size)
    {
        CV_DbgAssert(مدخل.cl_میانگیر_2 == عدم);
        مدخل.ظرفیت = align_اندازه(size, (صحيح)تخصیص_Granularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقي is_Fine_Grain_میانگیر = svmCaps.is_Support_Fine_Grain_میانگیر();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_میانگیر ? CL_MEM_SVM_FINE_GRAIN_میانگیر : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحيح)مدخل.ظرفیت);
        فراغ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, مدخل.ظرفیت, 0);
        CV_Assert(buf);

        مدخل.cl_میانگیر_2 = buf;
        {
            CV_IMPL_جمع_کن(CV_IMPL_OCL);
        }
        LOG_میانگیر_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (دراز دراز)مدخل.ظرفیت, (دراز دراز)مدخل.ظرفیت, مدخل.cl_میانگیر_2);
        allocatedEntries_.push_back(مدخل);
    }

    فراغ release_میانگیر_مدخل(ثابت میانگیر_مدخل& مدخل)
    {
        CV_Assert(مدخل.ظرفیت != 0);
        CV_Assert(مدخل.cl_میانگیر_2 != عدم);
        LOG_میانگیر_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                مدخل.cl_میانگیر_2, (دراز دراز)مدخل.ظرفیت, (دراز دراز)مدخل.ظرفیت);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  مدخل.cl_میانگیر_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), مدخل.cl_میانگیر_2);
    }
};
#پایان_اگر



قالب <منطقي readAccess, منطقي writeAccess>
صنف Aligned_داده_Ptr
{
محمي:
    ثابت size_t اندازه_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عام:
    Aligned_داده_Ptr(uchar* ptr, size_t size, size_t alignment)
        : اندازه_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        إذا (((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[اندازه_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                کپی_حافظه(ptr_, originPtr_, اندازه_3);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_داده_Ptr()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                کپی_حافظه(originPtr_, ptr_, اندازه_3);
            }
            حذف[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_داده_Ptr(ثابت Aligned_داده_Ptr&); // disabled
    Aligned_داده_Ptr& عملگر=(ثابت Aligned_داده_Ptr&); // disabled
};

قالب <منطقي readAccess, منطقي writeAccess>
صنف Aligned_داده_Ptr2_D
{
محمي:
    ثابت size_t اندازه_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t ردیفها_2;
    size_t تعداد_ستون_2;
    size_t گام_2;

عام:
    Aligned_داده_Ptr2_D(uchar* ptr, size_t ردیفها, size_t تعداد_ستون, size_t گام, size_t alignment, size_t extrabytes=0)
        : اندازه_3(ردیفها*گام), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم), ردیفها_2(ردیفها), تعداد_ستون_2(تعداد_ستون), گام_2(گام)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != عدم);
        إذا (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[اندازه_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                لكل (size_t i = 0; i < ردیفها_2; i++)
                    کپی_حافظه(ptr_ + i*گام_2, originPtr_ + i*گام_2, تعداد_ستون_2);
            }
        }
    }

    uchar* getAlignedPtr() ثابت
    {
        CV_DbgAssert(((size_t)ptr_ & (alignment_ - 1)) == 0);
        أرجع ptr_;
    }

    ~Aligned_داده_Ptr2_D()
    {
        إذا (allocatedPtr_)
        {
            إذا (writeAccess)
            {
                لكل (size_t i = 0; i < ردیفها_2; i++)
                    کپی_حافظه(originPtr_ + i*گام_2, ptr_ + i*گام_2, تعداد_ستون_2);
            }
            حذف[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_داده_Ptr2_D(ثابت Aligned_داده_Ptr2_D&); // disabled
    Aligned_داده_Ptr2_D& عملگر=(ثابت Aligned_داده_Ptr2_D&); // disabled
};

#اگر_تعریف_نشده CV_OPENCL_داده_PTR_ALIGNMENT
#تعریف_کن CV_OPENCL_داده_PTR_ALIGNMENT 16
#پایان_اگر


فراغ Context::Impl::__init_buffer_pools()
{
    میانگیر_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& میانگیر_Pool_3 = *میانگیر_Pool_2.get();
    میانگیر_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& میانگیر_Pool_Host_Ptr_2 = *میانگیر_Pool_Host_Ptr.get();

    size_t default_Pool_اندازه = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_اندازه = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    میانگیر_Pool_3.مجموعه_Max_Reserved_اندازه(pool_اندازه);
    size_t pool_اندازه_Host_Ptr = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    میانگیر_Pool_Host_Ptr_2.مجموعه_Max_Reserved_اندازه(pool_اندازه_Host_Ptr);

#اگر_تعریف_شده HAVE_OPENCL_SVM
    میانگیر_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& میانگیر_Pool_SVM_2 = *میانگیر_Pool_SVM.get();
    size_t pool_اندازه_SVM = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    میانگیر_Pool_SVM_2.مجموعه_Max_Reserved_اندازه(pool_اندازه_SVM);
#پایان_اگر

    CV_LOG_INFO(عدم, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_اندازه << " poolSizeHostPtr=" << pool_اندازه_Host_Ptr);
}

صنف OpenCLAllocator CV_FINAL : عام MatAllocator
{
عام:
    تعداد AllocatorFlags
    {
        ALLOCATOR_FLAGS_میانگیر_POOL_استفاده_شده = 1 << 0,
        ALLOCATOR_FLAGS_میانگیر_POOL_HOST_PTR_استفاده_شده = 1 << 1,
#اگر_تعریف_شده HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_میانگیر_POOL_SVM_استفاده_شده = 1 << 2,
#پایان_اگر
        ALLOCATOR_FLAGS_EXTERNAL_میانگیر = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_صف();
    }

    UMat_داده* defaultAllocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* داده, size_t* گام,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMat_داده* u = matStdAllocator->allocate(dims, sizes, type, داده, گام, flags, usageFlags);
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

    فراغ get_بهترین_Flags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحيح& createFlags, UMat_داده::حافظه_Flag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        إذا ((usageFlags & USAGE_ALLOCATE_HOST_حافظه) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        إذا (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_حافظه_2()
#اگر_تعریف_نشده __APPLE__
                || dev.isIntel()
#پایان_اگر
                )
            )
        )
            flags0 = static_cast<UMat_داده::حافظه_Flag>(0);
        وإلا
            flags0 = UMat_داده::COPY_ON_MAP;
    }

    UMat_داده* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* داده, size_t* گام, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!useOpenCL())
            أرجع defaultAllocate(dims, sizes, type, داده, گام, flags, usageFlags);

        flush_Cleanup_صف();

        CV_Assert(داده == 0);
        size_t مجموع = CV_ELEM_اندازه(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( گام )
                گام[i] = مجموع;
            مجموع *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        إذا (!ctx.getImpl())
            أرجع defaultAllocate(dims, sizes, type, داده, گام, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحيح createFlags = 0;
        UMat_داده::حافظه_Flag flags0 = static_cast<UMat_داده::حافظه_Flag>(0);
        get_بهترین_Flags(ctx, flags, usageFlags, createFlags, flags0);

        فراغ* handle = عدم;
        صحيح allocatorFlags = 0;

#اگر_تعریف_شده HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        إذا (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_میانگیر_POOL_SVM_استفاده_شده;
            handle = ctxImpl.get_میانگیر_Pool_SVM().allocate(مجموع);

            // this property is constant, so single buffer pool can be used here
            منطقي is_Fine_Grain_میانگیر = svmCaps.is_Support_Fine_Grain_میانگیر();
            allocatorFlags |= is_Fine_Grain_میانگیر ? svm::OPENCL_SVM_FINE_GRAIN_میانگیر : svm::OPENCL_SVM_COARSE_GRAIN_میانگیر;
        }
        وإلا
#پایان_اگر
        إذا (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_میانگیر_POOL_استفاده_شده;
            handle = ctxImpl.get_میانگیر_Pool().allocate(مجموع);
        }
        وإلا إذا (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_میانگیر_POOL_HOST_PTR_استفاده_شده;
            handle = ctxImpl.get_میانگیر_Pool_Host_Ptr().allocate(مجموع);
        }
        وإلا
        {
            CV_Assert(handle != عدم); // Unsupported, throw
        }

        إذا (!handle)
            أرجع defaultAllocate(dims, sizes, type, داده, گام, flags, usageFlags);

        UMat_داده* u = جديد UMat_داده(هذا);
        u->داده = 0;
        u->size = مجموع;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<فراغ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->موقت_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(درست);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع u;
    }

    منطقي allocate(UMat_داده* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع خطأ;

        flush_Cleanup_صف();

        UMat_داده_Auto_قفل lock(u);

        إذا(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحيح createFlags = 0;
            UMat_داده::حافظه_Flag flags0 = static_cast<UMat_داده::حافظه_Flag>(0);
            get_بهترین_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقي copyOnMap = (flags0 & UMat_داده::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحيح allocatorFlags = 0;
            UMat_داده::حافظه_Flag موقت_UMat_Flags = static_cast<UMat_داده::حافظه_Flag>(0);
            فراغ* handle = عدم;
            cl_int retval = CL_SUCCESS;

#اگر_تعریف_شده HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقي useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            إذا (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                موقت_UMat_Flags = UMat_داده::موقت_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (صحيح)u->size, handle);
            }
            وإلا إذا (useSVM && (svmCaps.is_Support_Fine_Grain_میانگیر() || svmCaps.is_Support_Coarse_Grain_میانگیر()))
            {
                إذا (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    منطقي is_Fine_Grain_میانگیر = svmCaps.is_Support_Fine_Grain_میانگیر();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_میانگیر ? CL_MEM_SVM_FINE_GRAIN_میانگیر : 0);

                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (صحيح)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_صف q = عدم;
                    إذا (!is_Fine_Grain_میانگیر)
                    {
                        q = (cl_command_صف)صف::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحيح)u->size);
                        cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMap()");

                    }
                    کپی_حافظه(handle, u->origdata, u->size);
                    إذا (!is_Fine_Grain_میانگیر)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                    }

                    موقت_UMat_Flags = UMat_داده::موقت_UMAT | UMat_داده::موقت_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_میانگیر ? svm::OPENCL_SVM_FINE_GRAIN_میانگیر
                                                : svm::OPENCL_SVM_COARSE_GRAIN_میانگیر;
                }
            }
            وإلا
#پایان_اگر
            {
                إذا( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                موقت_UMat_Flags = UMat_داده::موقت_UMAT;
                إذا (
                #اگر_تعریف_شده __APPLE__
                    !copyOnMap &&
                #پایان_اگر
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (صحيح)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_داده && u->original_UMat_داده->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_میانگیر(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (دراز دراز صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                }
                إذا((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_میانگیر(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (دراز دراز صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                    موقت_UMat_Flags |= UMat_داده::موقت_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clCreateBuffer() => %p", (فراغ*)handle).c_str());
            إذا(!handle || retval != CL_SUCCESS)
                أرجع خطأ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = هذا;
            u->flags |= موقت_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(درست);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع درست;
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

    فراغ deallocate(UMat_داده* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        إذا (!!(u->flags & UMat_داده::ASYNC_CLEANUP))
            جمع_کن_مقصد_Cleanup_صف(u);
        وإلا
            deallocate_(u);
    }

    فراغ deallocate_(UMat_داده* u) ثابت
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        إذا ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_میانگیر) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#اگر_تعریف_شده _WIN32
        إذا (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            أرجع;             // avoid any OpenCL calls
#پایان_اگر
        إذا(u->موقت_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            إذا (u->hostCopyObsolete())
            {
#اگر_تعریف_شده HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    إذا( u->موقت_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر);
                        منطقي is_Fine_Grain_میانگیر = (u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر;
                        cl_command_صف q = عدم;
                        إذا (!is_Fine_Grain_میانگیر)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) == 0));
                            q = (cl_command_صف)صف::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMap()");
                        }
                        cl_پایان(q);
                        کپی_حافظه(u->origdata, u->handle, u->size);
                        إذا (!is_Fine_Grain_میانگیر)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, u->handle, 0, عدم, عدم);
                            CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                        }
                    }
                    وإلا
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وإلا
#پایان_اگر
                {
                    cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();
                    إذا( u->موقت_Copied_UMat() )
                    {
                        Aligned_داده_Ptr<خطأ, درست> alignedPtr(u->origdata, u->size, CV_OPENCL_داده_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وإلا
                    {
                        cl_int retval = 0;
                        إذا (u->موقت_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_صف(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            فراغ* داده = cl_افزودن_به_صف_Map_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (دراز دراز صحيح)u->size, داده).c_str());
                            CV_Assert(u->origdata == داده && "Details: https://github.com/opencv/opencv/issues/6293");
                            إذا (u->original_UMat_داده)
                            {
                                CV_Assert(u->original_UMat_داده->داده == داده);
                            }
                            retval = cl_افزودن_به_صف_Unmap_Mem_Object(q, (cl_mem)u->handle, داده, 0, 0, 0);
                            CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, داده, (دراز دراز صحيح)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_پایان(q));
                        }
                    }
                }
                u->markHostCopyObsolete(خطأ);
            }
            وإلا
            {
                // nothing
            }
#اگر_تعریف_شده HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
            {
                إذا( u->موقت_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وإلا
#پایان_اگر
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clReleaseMemObject(ptr=%p)", (فراغ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(درست);
            u->currAllocator = u->prevAllocator;
            u->prevAllocator = عدم;
            إذا(u->داده && u->copyOnMap() && u->داده != u->origdata)
                fastFree(u->داده);
            u->داده = u->origdata;
            u->currAllocator->deallocate(u);
            u = عدم;
        }
        وإلا
        {
            CV_Assert(u->origdata == عدم);
            إذا(u->داده && u->copyOnMap() && u->داده != u->origdata)
            {
                fastFree(u->داده);
                u->داده = 0;
                u->markHostCopyObsolete(درست);
            }
            إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_میانگیر_POOL_استفاده_شده)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_میانگیر_Pool().release((cl_mem)u->handle);
            }
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_میانگیر_POOL_HOST_PTR_استفاده_شده)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_میانگیر_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#اگر_تعریف_شده HAVE_OPENCL_SVM
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_میانگیر_POOL_SVM_استفاده_شده)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                وإلا إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, u->handle, 0, عدم, عدم);
                        CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_میانگیر_Pool_SVM().release((فراغ*)u->handle);
            }
#پایان_اگر
            وإلا
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(درست);
            حذف u;
            u = عدم;
        }
        CV_Assert(u == عدم);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    فراغ map(UMat_داده* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(درست);

        cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

        {
            إذا( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#اگر_تعریف_شده HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
                {
                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_میانگیر_MAP;
                        }
                    }
                    cl_پایان(q);
                    u->داده = (uchar*)u->handle;
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(درست);
                    أرجع;
                }
#پایان_اگر

                cl_int retval = CL_SUCCESS;
                إذا (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->داده = (uchar*)cl_افزودن_به_صف_Map_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_نتیجه(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (دراز دراز صحيح)u->size, u->داده).c_str());
                }
                إذا (u->داده && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(درست);
                    أرجع;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_داده::COPY_ON_MAP;
            }

            إذا(!u->داده)
            {
                u->داده = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(درست);
            }
        }

        إذا (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_داده_Ptr<خطأ, درست> alignedPtr(u->داده, u->size, CV_OPENCL_داده_PTR_ALIGNMENT);
#اگر_تعریف_شده HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == 0);
#پایان_اگر
            cl_int retval = cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (دراز دراز صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(خطأ);
        }
    }

    فراغ unmap(UMat_داده* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;


        CV_Assert(u->handle != 0);

        UMat_داده_Auto_قفل autolock(u);

        cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();
        cl_int retval = 0;
        إذا( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->داده != عدم);
#اگر_تعریف_شده HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
            {
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, u->handle,
                                0, عدم, عدم);
                        CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                        cl_پایان(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_میانگیر_MAP;
                    }
                }
                إذا (u->refcount == 0)
                    u->داده = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(درست);
                أرجع;
            }
#پایان_اگر
            إذا (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_افزودن_به_صف_Unmap_Mem_Object(q, (cl_mem)u->handle, u->داده, 0, 0, 0);
                CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, u->داده, (دراز دراز صحيح)u->size).c_str());
                إذا (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_پایان(q));
                }
                u->markDeviceMemMapped(خطأ);
                u->داده = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(درست);
            }
        }
        وإلا إذا( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_داده_Ptr<درست, خطأ> alignedPtr(u->داده, u->size, CV_OPENCL_داده_PTR_ALIGNMENT);
#اگر_تعریف_شده HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == 0);
#پایان_اگر
            retval = cl_افزودن_به_صف_Write_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (دراز دراز صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(خطأ);
            u->markHostCopyObsolete(درست);
        }
    }

    منطقي checkContinuous(صحيح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& مجموع, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقي iscontinuous = درست;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        مجموع = sz[dims-1];
        لكل( صحيح i = dims-2; i >= 0; i-- )
        {
            إذا( i >= 0 && (مجموع != srcstep[i] || مجموع != dststep[i]) )
                iscontinuous = خطأ;
            مجموع *= sz[i];
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

    فراغ download(UMat_داده* u, فراغ* dstptr, صحيح dims, ثابت size_t sz[],
                  ثابت size_t srcofs[], ثابت size_t srcstep[],
                  ثابت size_t dststep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;
        UMat_داده_Auto_قفل autolock(u);

        إذا( u->داده && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            أرجع;
        }
        CV_Assert( u->handle != 0 );

        cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

        size_t مجموع = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            مجموع, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
        {
            CV_DbgAssert(u->داده == عدم || u->داده == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMap()");
            }
            cl_پایان(q);
            إذا( iscontinuous )
            {
                کپی_حافظه(dstptr, (uchar*)u->handle + srcrawofs, مجموع);
            }
            وإلا
            {
                // This code is from MatAllocator::download()
                صحيح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
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
                    کپی_حافظه(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                cl_پایان(q);
            }
        }
        وإلا
#پایان_اگر
        {
            إذا( iscontinuous )
            {
                Aligned_داده_Ptr<خطأ, درست> alignedPtr((uchar*)dstptr, مجموع, CV_OPENCL_داده_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, مجموع, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            وإلا إذا (CV_OPENCL_DISABLE_میانگیر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_داده_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_داده_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_داده_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                مجموع = align_اندازه(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                مجموع = std::min(مجموع, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, مجموع, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    کپی_حافظه( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وإلا
            {
                Aligned_داده_Ptr2_D<خطأ, درست> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_داده_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    فراغ upload(UMat_داده* u, ثابت فراغ* srcptr, صحيح dims, ثابت size_t sz[],
                ثابت size_t dstofs[], ثابت size_t dststep[],
                ثابت size_t srcstep[]) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->موقت_UMat());

        size_t مجموع = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            مجموع, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_داده_Auto_قفل autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        إذا( u->داده && (u->hostCopyObsolete() < u->deviceCopyObsolete() || مجموع == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(خطأ);
            u->markDeviceCopyObsolete(درست);
            أرجع;
        }

        CV_Assert( u->handle != 0 );
        cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
        {
            CV_DbgAssert(u->داده == عدم || u->داده == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMap()");
            }
            cl_پایان(q);
            إذا( iscontinuous )
            {
                کپی_حافظه((uchar*)u->handle + dstrawofs, srcptr, مجموع);
            }
            وإلا
            {
                // This code is from MatAllocator::upload()
                صحيح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
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
                    کپی_حافظه(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_میانگیر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMUnmap()");
                cl_پایان(q);
            }
        }
        وإلا
#پایان_اگر
        {
            إذا( iscontinuous )
            {
                Aligned_داده_Ptr<درست, خطأ> alignedPtr((uchar*)srcptr, مجموع, CV_OPENCL_داده_PTR_ALIGNMENT);
                cl_int retval = cl_افزودن_به_صف_Write_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, مجموع, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (فراغ*)u->handle, (دراز دراز صحيح)dstrawofs, (دراز دراز صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_میانگیر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_داده_PTR_ALIGNMENT;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_داده_Ptr2_D<خطأ, خطأ> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_داده_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                مجموع = align_اندازه(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                مجموع = std::min(مجموع, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, مجموع, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    کپی_حافظه( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_افزودن_به_صف_Write_میانگیر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, مجموع, ptr, 0, 0, 0));
            }
            وإلا
            {
                Aligned_داده_Ptr2_D<درست, خطأ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_داده_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_افزودن_به_صف_Write_میانگیر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(درست);
#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#پایان_اگر
        {
            u->markHostCopyObsolete(درست);
        }
        u->markDeviceCopyObsolete(خطأ);
    }

    فراغ copy(UMat_داده* src, UMat_داده* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي _sync) ثابت CV_OVERRIDE
    {
        إذا(!src || !dst)
            أرجع;

        size_t مجموع = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            مجموع, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_داده_Auto_قفل src_autolock(src, dst);

        إذا( !src->handle || (src->داده && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->داده + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            أرجع;
        }
        إذا( !dst->handle || (dst->داده && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->داده + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(خطأ);
#اگر_تعریف_شده HAVE_OPENCL_SVM
            إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وإلا
#پایان_اگر
            {
                dst->markDeviceCopyObsolete(درست);
            }
            أرجع;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
        {
            إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                إذا( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحيح)مجموع);
                    cl_int وضعیت = svmFns->fn_cl_افزودن_به_صف_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            مجموع, 0, عدم, عدم);
                    CV_OCL_CHECK_نتیجه(وضعیت, "clEnqueueSVMMemcpy()");
                }
                وإلا
                {
                    cl_پایان(q);
                    // This code is from MatAllocator::download()/upload()
                    صحيح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)بیشترین_صحیح );
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
                        کپی_حافظه(ptrs[1], ptrs[0], planesz);
                }
            }
            وإلا
            {
                إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->داده + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                وإلا
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->داده + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        وإلا
#پایان_اگر
        {
            إذا( iscontinuous )
            {
                retval = cl_افزودن_به_صف_Copy_میانگیر(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, مجموع, 0, 0, 0);
                CV_OCL_CHECK_نتیجه(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (فراغ*)src->handle, (فراغ*)dst->handle, (دراز دراز صحيح)srcrawofs, (دراز دراز صحيح)dstrawofs, (دراز دراز صحيح)مجموع).c_str());
            }
            وإلا إذا (CV_OPENCL_DISABLE_میانگیر_RECT_OPERATIONS)
            {
                ثابت size_t padding = CV_OPENCL_داده_PTR_ALIGNMENT;
                size_t new_srcrawofs = srcrawofs & ~(padding-1);
                size_t srcmembuf_ofs = srcrawofs - new_srcrawofs;
                size_t new_dstrawofs = dstrawofs & ~(padding-1);
                size_t dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_داده_Ptr2_D<خطأ, خطأ> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_داده_PTR_ALIGNMENT, padding*2);
                Aligned_داده_Ptr2_D<خطأ, خطأ> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_داده_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                size_t src_مجموع = align_اندازه(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_مجموع = std::min(src_مجموع, src->size - new_srcrawofs);
                size_t dst_مجموع = align_اندازه(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_مجموع = std::min(dst_مجموع, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_مجموع, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_افزودن_به_صف_Read_میانگیر(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_مجموع, dstptr, 0, 0, 0));

                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    کپی_حافظه( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_افزودن_به_صف_Write_میانگیر(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_مجموع, dstptr, 0, 0, 0));
            }
            وإلا
            {
                CV_OCL_CHECK(retval = cl_افزودن_به_صف_Copy_میانگیر_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        إذا (retval == CL_SUCCESS)
        {
            CV_IMPL_جمع_کن(CV_IMPL_OCL)
        }

#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_میانگیر ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_میانگیر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#پایان_اگر
        {
            dst->markHostCopyObsolete(درست);
        }
        dst->markDeviceCopyObsolete(خطأ);

        إذا( _sync )
        {
            CV_OCL_DBG_CHECK(cl_پایان(q));
        }
    }

    میانگیر_Pool_Controller* get_میانگیر_Pool_Controller(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        إذا (ctx.empty())
            أرجع عدم;
#اگر_تعریف_شده HAVE_OPENCL_SVM
        إذا ((svm::checkForceSVMUmatUsage() && (id == عدم || strcmp(id, "OCL") == 0)) || (id != عدم && strcmp(id, "SVM") == 0))
        {
            أرجع &ctx.getImpl()->get_میانگیر_Pool_SVM();
        }
#پایان_اگر
        إذا (id != عدم && strcmp(id, "HOST_ALLOC") == 0)
        {
            أرجع &ctx.getImpl()->get_میانگیر_Pool_Host_Ptr();
        }
        إذا (id != عدم && strcmp(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        أرجع &ctx.getImpl()->get_میانگیر_Pool();
    }

    MatAllocator* matStdAllocator;

    تغییرپذیر cv::Mutex cleanup_صف_Mutex;
    تغییرپذیر std::deque<UMat_داده*> cleanup_صف;

    فراغ flush_Cleanup_صف() ثابت
    {
        // TSAN false positive: see #27638
        إذا (!cleanup_صف.empty())
        {
            std::deque<UMat_داده*> q;
            {
                cv::Auto_قفل lock(cleanup_صف_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_صف);
            }
            لكل (std::deque<UMat_داده*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    فراغ جمع_کن_مقصد_Cleanup_صف(UMat_داده* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_قفل lock(cleanup_صف_Mutex);
            cleanup_صف.push_back(u);
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
فراغ مجموعه_اندازه( UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* گامها_2,
              منطقي auto_گامها = خطأ );
فراغ finalizeHdr(UMat& m);

} // namespace cv


نطاق cv { نطاق ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
فراغ convert_From_میانگیر(فراغ* cl_mem_میانگیر, size_t گام, صحيح ردیفها, صحيح تعداد_ستون, صحيح type, UMat& dst)
{
    صحيح d = 2;
    صحيح sizes[] = { ردیفها, تعداد_ستون };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_ماسک) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    مجموعه_اندازه(dst, d, sizes, 0, درست);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_میانگیر;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_میانگیر == mem_type);

    size_t مجموع = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_اندازه, حجم(size_t), &مجموع, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحيح)گام >= تعداد_ستون * CV_ELEM_اندازه(type));
    CV_Assert(مجموع >= ردیفها * گام);

    // attach clBuffer to UMatData
    dst.u = جديد UMat_داده(getOpenCLAllocator());
    dst.u->داده            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_میانگیر;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_داده::حافظه_Flag>(0);
    dst.u->handle          = cl_mem_میانگیر;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = مجموع;

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

    صحيح عمق = CV_8U;
    بدل (fmt.image_channel_داده_type)
    {
    حالة CL_UNORM_INT8:
    حالة CL_UNSIGNED_INT8:
        عمق = CV_8U;
        اكسر;

    حالة CL_SNORM_INT8:
    حالة CL_SIGNED_INT8:
        عمق = CV_8S;
        اكسر;

    حالة CL_UNORM_INT16:
    حالة CL_UNSIGNED_INT16:
        عمق = CV_16U;
        اكسر;

    حالة CL_SNORM_INT16:
    حالة CL_SIGNED_INT16:
        عمق = CV_16S;
        اكسر;

    حالة CL_SIGNED_INT32:
        عمق = CV_32S;
        اكسر;

    حالة CL_FLOAT:
        عمق = CV_32F;
        اكسر;

    حالة CL_HALF_FLOAT:
        عمق = CV_16F;
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
        type = CV_MAKE_TYPE(عمق, 1);
        اكسر;

    حالة CL_RG:
    حالة CL_RA:
        type = CV_MAKE_TYPE(عمق, 2);
        اكسر;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالة CL_RGBA:
    حالة CL_BGRA:
    حالة CL_ARGB:
        type = CV_MAKE_TYPE(عمق, 4);
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        اكسر;
    }

    size_t گام = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ردیف_PITCH, حجم(size_t), &گام, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_عرض, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ارتفاع, حجم(size_t), &h, 0));

    dst.create((صحيح)h, (صحيح)w, type);

    cl_mem cl_میانگیر = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_صف q = (cl_command_صف)صف::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_افزودن_به_صف_Copy_Image_مقصد_میانگیر(q, clImage, cl_میانگیر, src_origin, region, offset, 0, عدم, عدم));

    CV_OCL_CHECK(cl_پایان(q));

    أرجع;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساكن فراغ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint رقم_Devices = 0;
    cl_int وضعیت = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, عدم, &رقم_Devices);
    إذا (وضعیت != CL_DEVICE_NOT_یافت) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_نتیجه(وضعیت,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &رقم_Devices).c_str());
    }

    إذا (رقم_Devices == 0)
    {
        devices.clear();
        أرجع;
    }

    devices.resize((size_t)رقم_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, رقم_Devices, &devices[0], &رقم_Devices));
}

بنية PlatformInfo::Impl
{
    Impl(فراغ* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        نسخه_3 = getStrProp(CL_PLATFORM_نسخه);
        parseOpenCLVersion(نسخه_3, نسخه_Major, نسخه_Minor);
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

    String نسخه_3;
    صحيح نسخه_Major;
    صحيح نسخه_Minor;
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

PlatformInfo& PlatformInfo::عملگر =(ثابت PlatformInfo& i)
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
    i.p = اشاره_تهی;
}

PlatformInfo& PlatformInfo::عملگر = (PlatformInfo&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا(p)
            p->release();
        p = i.p;
        i.p = اشاره_تهی;
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
        device.مجموعه(p->devices[d]);
}

String PlatformInfo::name() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() ثابت
{
    أرجع p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::نسخه() ثابت
{
    أرجع p ? p->نسخه_3 : String();
}

صحيح PlatformInfo::نسخه_Major_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخه_Major;
}

صحيح PlatformInfo::نسخه_Minor_2() ثابت
{
    CV_Assert(p);
    أرجع p->نسخه_Minor;
}

ساكن فراغ getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint رقم_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, عدم, &رقم_Platforms));

    إذا (رقم_Platforms == 0)
    {
        platforms.clear();
        أرجع;
    }

    platforms.resize((size_t)رقم_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(رقم_Platforms, &platforms[0], &رقم_Platforms));
}

فراغ getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    لكل (size_t i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((فراغ*)&platforms[i]) );
}

ثابت حرف* type_مقصد_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_عمق_MAX*16]=
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
    صحيح cn = CV_MAT_CN(type), عمق = CV_MAT_عمق(type);
    ثابت حرف* نتیجه = cn > 16 ? اشاره_تهی : tab[عمق*16 + cn-1];
    CV_Assert(نتیجه);
    أرجع نتیجه;
}

ثابت حرف* memop_Type_مقصد_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_عمق_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), عمق = CV_MAT_عمق(type);
    ثابت حرف* نتیجه = cn > 16 ? اشاره_تهی : tab[عمق*16 + cn-1];
    CV_Assert(نتیجه);
    أرجع نتیجه;
}

ثابت حرف* vecop_Type_مقصد_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_عمق_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), عمق = CV_MAT_عمق(type);
    ثابت حرف* نتیجه = cn > 16 ? 0 : tab[عمق*16 + cn-1];
    CV_Assert(نتیجه);
    أرجع نتیجه;
}

// Deprecated due to size of buf buffer being unknowable.
ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr size_t buf_max = 50;

    أرجع convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

ثابت حرف* convertTypeStr(صحيح sdepth, صحيح ddepth, صحيح cn, حرف* buf, size_t buf_اندازه_2)
{
    إذا( sdepth == ddepth )
        أرجع "noconvert";
    ثابت حرف *typestr = type_مقصد_Str(CV_MAKETYPE(ddepth, cn));
    إذا( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_اندازه_2, "convert_%s", typestr);
    }
    وإلا إذا( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_اندازه_2, "convert_%s_sat_rte", typestr);
    وإلا
        snprintf(buf, buf_اندازه_2, "convert_%s_sat", typestr);

    أرجع buf;
}

ثابت حرف* getOpenCLErrorString(صحيح error_کد)
{
#تعریف_کن CV_OCL_کد(id) حالة id: أرجع #id
#تعریف_کن CV_OCL_کد_2(id, name) حالة id: أرجع #name
    بدل (error_کد)
    {
    CV_OCL_کد(CL_SUCCESS);
    CV_OCL_کد(CL_DEVICE_NOT_یافت);
    CV_OCL_کد(CL_DEVICE_NOT_موجود);
    CV_OCL_کد(CL_COMPILER_NOT_موجود);
    CV_OCL_کد(CL_MEM_OBJECT_تخصیص_FAILURE);
    CV_OCL_کد(CL_بیرون_از_RESOURCES);
    CV_OCL_کد(CL_بیرون_از_HOST_حافظه);
    CV_OCL_کد(CL_PROFILING_INFO_NOT_موجود);
    CV_OCL_کد(CL_MEM_COPY_OVERLAP);
    CV_OCL_کد(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_کد(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_کد(CL_BUILD_برنامه_FAILURE);
    CV_OCL_کد(CL_MAP_FAILURE);
    CV_OCL_کد(CL_MISALIGNED_SUB_میانگیر_جابه_جایی);
    CV_OCL_کد(CL_EXEC_وضعیت_ERROR_FOR_EVENTS_درون_WAIT_LIST);
    CV_OCL_کد(CL_COMPILE_برنامه_FAILURE);
    CV_OCL_کد(CL_LINKER_NOT_موجود);
    CV_OCL_کد(CL_LINK_برنامه_FAILURE);
    CV_OCL_کد(CL_DEVICE_افراز_FAILED);
    CV_OCL_کد(CL_KERNEL_ARG_INFO_NOT_موجود);
    CV_OCL_کد(CL_INVALID_مقدار);
    CV_OCL_کد(CL_INVALID_DEVICE_TYPE);
    CV_OCL_کد(CL_INVALID_PLATFORM);
    CV_OCL_کد(CL_INVALID_DEVICE);
    CV_OCL_کد(CL_INVALID_CONTEXT);
    CV_OCL_کد(CL_INVALID_صف_PROPERTIES);
    CV_OCL_کد(CL_INVALID_COMMAND_صف);
    CV_OCL_کد(CL_INVALID_HOST_PTR);
    CV_OCL_کد(CL_INVALID_MEM_OBJECT);
    CV_OCL_کد(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_کد(CL_INVALID_IMAGE_اندازه);
    CV_OCL_کد(CL_INVALID_SAMPLER);
    CV_OCL_کد(CL_INVALID_دودویی);
    CV_OCL_کد(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_کد(CL_INVALID_برنامه);
    CV_OCL_کد(CL_INVALID_برنامه_EXECUTABLE);
    CV_OCL_کد(CL_INVALID_KERNEL_NAME);
    CV_OCL_کد(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_کد(CL_INVALID_KERNEL);
    CV_OCL_کد(CL_INVALID_ARG_شاخص);
    CV_OCL_کد(CL_INVALID_ARG_مقدار);
    CV_OCL_کد(CL_INVALID_ARG_اندازه);
    CV_OCL_کد(CL_INVALID_KERNEL_آرگومانها);
    CV_OCL_کد(CL_INVALID_WORK_DIMENSION);
    CV_OCL_کد(CL_INVALID_WORK_گروه_اندازه);
    CV_OCL_کد(CL_INVALID_WORK_قلم_اندازه);
    CV_OCL_کد(CL_INVALID_GLOBAL_جابه_جایی);
    CV_OCL_کد(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_کد(CL_INVALID_EVENT);
    CV_OCL_کد(CL_INVALID_OPERATION);
    CV_OCL_کد(CL_INVALID_GL_OBJECT);
    CV_OCL_کد(CL_INVALID_میانگیر_اندازه);
    CV_OCL_کد(CL_INVALID_MIP_LEVEL);
    CV_OCL_کد(CL_INVALID_GLOBAL_WORK_اندازه);
    // OpenCL 1.1
    CV_OCL_کد(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_کد(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_کد(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_کد(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_کد(CL_INVALID_DEVICE_افراز_شمار);
    // OpenCL 2.0
    CV_OCL_کد_2(-69, CL_INVALID_PIPE_اندازه);
    CV_OCL_کد_2(-70, CL_INVALID_DEVICE_صف);
    // Extensions
    CV_OCL_کد_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_کد_2(-1001, CL_PLATFORM_NOT_یافت_KHR);
    CV_OCL_کد_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_کد_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_کد_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_کد_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    افتراضي: أرجع "Unknown OpenCL error";
    }
#لغو_تعریف CV_OCL_کد
#لغو_تعریف CV_OCL_کد_2
}

قالب <نام_نوع T>
ساكن std::string ker_مقصد_Str(ثابت Mat & k)
{
    صحيح عرض = k.تعداد_ستون - 1, عمق = k.عمق();
    ثابت T * ثابت داده = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    إذا (عمق <= CV_8S)
    {
        لكل (صحيح i = 0; i < عرض; ++i)
            stream << "DIG(" << (صحيح)داده[i] << ")";
        stream << "DIG(" << (صحيح)داده[عرض] << ")";
    }
    وإلا إذا (عمق == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < عرض; ++i)
            stream << "DIG(" << داده[i] << "f)";
        stream << "DIG(" << داده[عرض] << "f)";
    }
    وإلا إذا (عمق == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < عرض; ++i)
            stream << "DIG(" << (عائم)داده[i] << "h)";
        stream << "DIG(" << (عائم)داده[عرض] << "h)";
    }
    وإلا
    {
        لكل (صحيح i = 0; i < عرض; ++i)
            stream << "DIG(" << داده[i] << ")";
        stream << "DIG(" << داده[عرض] << ")";
    }

    أرجع stream.str();
}

String kernel_مقصد_Str(درون_داد_Array _kernel, صحيح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحيح عمق = kernel.عمق();
    إذا (ddepth < 0)
        ddepth = عمق;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    إذا (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    إذا (ddepth != عمق)
        kernel.convert_مقصد(kernel, ddepth);

    تعريفنوع std::string (* func_t)(ثابت Mat &);
    ساكن ثابت func_t funcs[CV_عمق_MAX] = { ker_مقصد_Str<uchar>, ker_مقصد_Str<حرف>, ker_مقصد_Str<ushort>, ker_مقصد_Str<کوتاه>,
                                                ker_مقصد_Str<صحيح>, ker_مقصد_Str<عائم>, ker_مقصد_Str<مزدوج>, ker_مقصد_Str<hfloat>,
                                                0, ker_مقصد_Str<منطقي>, ker_مقصد_Str<uint64_t>, ker_مقصد_Str<int64_t>, ker_مقصد_Str<بیعلامت> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    أرجع cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#تعریف_کن PROCESS_SRC(src) \
    افعل \
    { \
        إذا (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            اندازه csize = src.size(); \
            صحيح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_عمق(ctype), \
                ckercn = وکتور_Widths[cdepth], cwidth = ccn * csize.عرض; \
            إذا (cwidth < ckercn || ckercn <= 0) \
                أرجع 1; \
            تعداد_ستون.push_back(cwidth); \
            إذا (strat == OCL_وکتور_OWN && ctype != ref_type) \
                أرجع 1; \
            offsets.push_back(src.offset()); \
            گامها.push_back(src.گام()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    طالما ((فراغ)0, 0)

صحيح predict_Optimal_وکتور_عرض(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
                              درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
                              درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9,
                              Ocl_وکتور_Strategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحيح وکتور_Widths[CV_عمق_MAX] = {
        d.preferred_وکتور_عرض_Char(), d.preferred_وکتور_عرض_Char(),   // CV_8U,  CV_8S
        d.preferred_وکتور_عرض_Short(), d.preferred_وکتور_عرض_Short(), // CV_16U, CV_16S
        d.preferred_وکتور_عرض_Int(), d.preferred_وکتور_عرض_Float(),   // CV_32S, CV_32F
        d.preferred_وکتور_عرض_Double(), d.preferred_وکتور_عرض_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_وکتور_عرض_Char(),                                 // CV_Bool
        d.preferred_وکتور_عرض_Long(), d.preferred_وکتور_عرض_Long(),   // CV_64U, CV_64S
        d.preferred_وکتور_عرض_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    إذا (وکتور_Widths[0] == 1)
    {
        // it's heuristic
        وکتور_Widths[CV_8U] = وکتور_Widths[CV_8S] = 4;
        وکتور_Widths[CV_16U] = وکتور_Widths[CV_16S] = وکتور_Widths[CV_16F] = 2;
        وکتور_Widths[CV_32S] = وکتور_Widths[CV_32F] = وکتور_Widths[CV_64F] = 1;
    }

    أرجع check_Optimal_وکتور_عرض(وکتور_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحيح check_Optimal_وکتور_عرض([[maybe_unused]] ثابت صحيح *وکتور_Widths,
                            [[maybe_unused]] درون_داد_Array src1,
                            [[maybe_unused]] درون_داد_Array src2,
                            [[maybe_unused]] درون_داد_Array src3,
                            [[maybe_unused]] درون_داد_Array src4,
                            [[maybe_unused]] درون_داد_Array src5,
                            [[maybe_unused]] درون_داد_Array src6,
                            [[maybe_unused]] درون_داد_Array src7,
                            [[maybe_unused]] درون_داد_Array src8,
                            [[maybe_unused]] درون_داد_Array src9,
                            [[maybe_unused]] Ocl_وکتور_Strategy strat)
{
#اگر_تعریف_شده __APPLE__
    أرجع 1;
#وإلا
    CV_Assert(وکتور_Widths);

    صحيح ref_type = src1.type();

    std::vector<size_t> offsets, گامها, تعداد_ستون;
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
        طالما (offsets[i] % dividers[i] != 0 || گامها[i] % dividers[i] != 0 || تعداد_ستون[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحيح kercn = *std::min_element(kercns.begin(), kercns.end());

    أرجع kercn;
#پایان_اگر
}

صحيح predict_Optimal_وکتور_عرض_Max(درون_داد_Array src1, درون_داد_Array src2, درون_داد_Array src3,
                                 درون_داد_Array src4, درون_داد_Array src5, درون_داد_Array src6,
                                 درون_داد_Array src7, درون_داد_Array src8, درون_داد_Array src9)
{
    أرجع predict_Optimal_وکتور_عرض(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_وکتور_MAX);
}

#لغو_تعریف PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
فراغ build_Options_جمع_کن_ماتریس_Description(String& buildOptions, ثابت String& name, درون_داد_Array _m)
{
    إذا (!buildOptions.empty())
        buildOptions += " ";
    صحيح type = _m.type(), عمق = CV_MAT_عمق(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_مقصد_Str(type),
            name.c_str(), ocl::type_مقصد_Str(CV_MAKE_TYPE(عمق, 1)),
            name.c_str(), (صحيح)CV_MAT_CN(type),
            name.c_str(), (صحيح)CV_ELEM_اندازه(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحيح)عمق
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

    ساكن cl_image_format getImageFormat(صحيح عمق, صحيح cn, منطقي norm)
    {
        cl_image_format format;
        ساكن ثابت صحيح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساكن ثابت صحيح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساكن ثابت صحيح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحيح channelType = norm ? channelTypesNorm[عمق] : channelTypes[عمق];
        صحيح channel_ترتیب = channelOrders[cn];
        format.image_channel_داده_type = (cl_channel_type)channelType;
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
        cl_uint رقم_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, رقم_Formats,
                                                عدم, &رقم_Formats);
        CV_OCL_DBG_CHECK_نتیجه(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        إذا (رقم_Formats > 0)
        {
            Auto_میانگیر<cl_image_format> formats(رقم_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, رقم_Formats,
                                             formats.داده(), عدم);
            CV_OCL_DBG_CHECK_نتیجه(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            لكل (cl_uint i = 0; i < رقم_Formats; ++i)
            {
                إذا (!memcmp(&formats[i], &format, حجم(format)))
                {
                    أرجع درست;
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

        صحيح err, عمق = src.عمق(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(عمق, cn, norm);

        إذا (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        إذا (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_صف صف_2 = (cl_command_صف)صف::getDefault().ptr();

#اگر_تعریف_شده CL_نسخه_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحيح minor = d.device_نسخه_Minor(), major = d.device_نسخه_Major();
        CV_Assert(!alias || canCreateAlias(src));
        إذا (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_عرض      = src.تعداد_ستون;
            desc.image_ارتفاع     = src.ردیفها;
            desc.image_عمق      = 0;
            desc.image_array_اندازه = 1;
            desc.image_ردیف_pitch  = alias ? src.گام[0] : 0;
            desc.image_برش_pitch = 0;
            desc.میانگیر           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.رقم_mip_سطوح   = 0;
            desc.رقم_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, عدم, &err);
        }
        وإلا
#پایان_اگر
        {
            CV_SUPPRESS_DEPRECATED_شروع
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.تعداد_ستون, src.ردیفها, 0, عدم, &err);
            CV_SUPPRESS_DEPRECATED_انتها
        }
        CV_OCL_DBG_CHECK_نتیجه(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.تعداد_ستون), static_cast<size_t>(src.ردیفها), 1 };

        cl_mem dev_داده;
        إذا (!alias && !src.isContinuous())
        {
            dev_داده = cl_Create_میانگیر(context, CL_MEM_READ_ONLY, src.تعداد_ستون * src.ردیفها * src.elem_اندازه(), عدم, &err);
            CV_OCL_CHECK_نتیجه(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (دراز دراز صحيح)(src.تعداد_ستون * src.ردیفها * src.elem_اندازه()), (فراغ*)dev_داده
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.تعداد_ستون) * src.elem_اندازه(), static_cast<size_t>(src.ردیفها), 1};
            CV_OCL_CHECK(cl_افزودن_به_صف_Copy_میانگیر_Rect(صف_2, (cl_mem)src.handle(ACCESS_READ), dev_داده, origin, origin,
                roi, src.گام, 0, src.تعداد_ستون * src.elem_اندازه(), 0, 0, عدم, عدم));
            CV_OCL_DBG_CHECK(clFlush(صف_2));
        }
        وإلا
        {
            dev_داده = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_داده != عدم);

        إذا (!alias)
        {
            CV_OCL_CHECK(cl_افزودن_به_صف_Copy_میانگیر_مقصد_Image(صف_2, dev_داده, handle, 0, origin, region, 0, عدم, 0));
            إذا (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(صف_2));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_داده));
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
    إذا (d.image_From_میانگیر_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        إذا (pitchAlign && !(m.گام % (pitchAlign * m.elem_اندازه())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            إذا (!m.u->موقت_UMat())
            {
                ret = درست;
            }
        }
    }
    أرجع ret;
}

منطقي Image2D::isFormatSupported(صحيح عمق, صحيح cn, منطقي norm)
{
    cl_image_format format = Impl::getImageFormat(عمق, cn, norm);

    أرجع Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    إذا (p)
        p->addref();
}

Image2D & Image2D::عملگر = (ثابت Image2D & i)
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
    i.p = اشاره_تهی;
}

Image2D& Image2D::عملگر = (Image2D&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا (p)
            p->release();
        p = i.p;
        i.p = اشاره_تهی;
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
        initialized = درست;
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
        initialized = درست;
    }
    أرجع value;
}

منطقي internal::isCLBuffer(UMat& u)
{
    إذا (isCudaBacked(u.u))
        أرجع خطأ;
    فراغ* h = u.handle(ACCESS_RW);
    إذا (!h)
        أرجع درست;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#إذا 1
    إذا ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        أرجع خطأ;
#وإلا
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, حجم(type), &type, عدم);
    إذا (ret != CL_SUCCESS || type != CL_MEM_OBJECT_میانگیر)
        أرجع خطأ;
#پایان_اگر
    أرجع درست;
}

بنية Timer::Impl
{
    ثابت صف صف_2;

    Impl(ثابت صف& q)
        : صف_2(q)
    {
    }

    ~Impl(){}

    فراغ شروع()
    {
        CV_OCL_DBG_CHECK(cl_پایان((cl_command_صف)صف_2.ptr()));
        timer.شروع();
    }

    فراغ stop()
    {
        CV_OCL_DBG_CHECK(cl_پایان((cl_command_صف)صف_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        أرجع (uint64)(timer.get_زمان_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت صف& q) : p(جديد Impl(q)) { }
Timer::~Timer() { حذف p; }

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

#پایان_اگر // HAVE_OPENCL
