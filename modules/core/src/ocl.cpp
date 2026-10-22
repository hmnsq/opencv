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

#که_نه_تعریف_شوی HAVE_OPENCL
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
#د_که_پای

#ضمن <opencv2/core/utils/configuration.private.hpp>

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_لغوه CV_LOG_STRIP_LEVEL
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ضمن <opencv2/core/utils/logger.hpp>

#ضمن "opencv2/core/ocl_genbase.hpp"
#ضمن "opencl_kernels_core.hpp"

#ضمن "opencv2/core/utils/lock.private.hpp"
#ضمن "opencv2/core/utils/filesystem.hpp"
#ضمن "opencv2/core/utils/filesystem.private.hpp"

#تعریف_کړه CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "OpenCL allocator: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#تعریف_لغوه CV__ALLOCATOR_STATS_LOG

#تعریف_کړه CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#تعریف_کړه CV_OPENCL_SHOW_BUILD_OPTIONS             0
#تعریف_کړه CV_OPENCL_SHOW_BUILD_KERNELS             0

#تعریف_کړه CV_OPENCL_SHOW_پرلپسې_KERNELS               0
#تعریف_کړه CV_OPENCL_SYNC_پرلپسې_KERNELS               0
#تعریف_کړه CV_OPENCL_TRACE_CHECK                    0

#تعریف_کړه CV_OPENCL_VALIDATE_دوګونی_PROGRAMS       1

#تعریف_کړه CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#تعریف_کړه CV_OPENCL_SHOW_SVM_LOG                   0

#ضمن "opencv2/core/bufferpool.hpp"
#که_نه_تعریف_شوی LOG_بفر_POOL
# إذا 0
#   define LOG_بفر_POOL چاپ_کړه
# وإلا
#   define LOG_بفر_POOL(...)
# endif
#د_که_پای

#إذا CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#تعریف_کړه CV_OPENCL_SVM_TRACE_P چاپ_کړه("line %d (ocl.cpp): ", __LINE__); چاپ_کړه
#وإلا
#تعریف_کړه CV_OPENCL_SVM_TRACE_P(...)
#د_که_پای

#إذا CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#تعریف_کړه CV_OPENCL_SVM_TRACE_ERROR_P چاپ_کړه("Error on line %d (ocl.cpp): ", __LINE__); چاپ_کړه
#وإلا
#تعریف_کړه CV_OPENCL_SVM_TRACE_ERROR_P(...)
#د_که_پای

#ضمن "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#ضمن "opencv2/core/opencl/runtime/opencl_core.hpp"

#که_تعریف_شوی HAVE_OPENCL_SVM
#ضمن "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#ضمن "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#ضمن "opencv2/core/opencl/opencl_svm.hpp"
#د_که_پای

#ضمن "umatrix.hpp"

نطاق cv { نطاق ocl {

#تعریف_کړه IMPLEMENT_REFCOUNTABLE() \
    فراغ addref() { CV_XADD(&refcount, 1); } \
    فراغ release() { إذا( CV_XADD(&refcount, -1) == 1 && !cv::__termination) ړنګول هذا; } \
    صحيح refcount

ساكن cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    أرجع opencl_allocator_stats;
}

#که_نه_تعریف_شوی _DEBUG
ساكن منطقي isRaiseError()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي value = خطأ;
    إذا (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", خطأ);
        initialized = ریښتیا;
    }
    أرجع value;
}
#د_که_پای

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
    std::وتنه << "OpenCV(OpenCL:" << حالت << "): " << message << std::کرښه_پای << std::flush;
}
#تعریف_کړه CV_OCL_TRACE_CHECK_پایله(حالت, message) traceOpenCLCheck(حالت, message)
#وإلا
#تعریف_کړه CV_OCL_TRACE_CHECK_پایله(حالت, message) /* nothing */
#د_که_پای

#تعریف_کړه CV_OCL_API_ERROR_پیغام(check_پایله, پیغام) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_پایله), check_پایله, پیغام)

#تعریف_کړه CV_OCL_CHECK_پایله(check_پایله, پیغام) \
    افعل { \
        CV_OCL_TRACE_CHECK_پایله(check_پایله, پیغام); \
        إذا (check_پایله != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام_2 = CV_OCL_API_ERROR_پیغام(check_پایله, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام_2); \
        } \
    } طالما (0)

#تعریف_کړه CV_OCL_CHECK_(بیان, check_پایله) افعل { بیان; CV_OCL_CHECK_پایله(check_پایله, #expr); } طالما (0)

#تعریف_کړه CV_OCL_CHECK(بیان) افعل { cl_int __cl_result = (بیان); CV_OCL_CHECK_پایله(__cl_result, #expr); } طالما (0)

#که_تعریف_شوی _DEBUG
#تعریف_کړه CV_OCL_DBG_CHECK_پایله(check_پایله, پیغام) CV_OCL_CHECK_پایله(check_پایله, پیغام)
#تعریف_کړه CV_OCL_DBG_CHECK(بیان) CV_OCL_CHECK(بیان)
#تعریف_کړه CV_OCL_DBG_CHECK_(بیان, check_پایله) CV_OCL_CHECK_(بیان, check_پایله)
#وإلا
#تعریف_کړه CV_OCL_DBG_CHECK_پایله(check_پایله, پیغام) \
    افعل { \
        CV_OCL_TRACE_CHECK_پایله(check_پایله, پیغام); \
        إذا (check_پایله != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(پیغام), ثابت حرف*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_پیغام_2 = CV_OCL_API_ERROR_پیغام(check_پایله, پیغام); \
            CV_Error(Error::OpenCLApiCallError, error_پیغام_2); \
        } \
    } طالما (0)
#تعریف_کړه CV_OCL_DBG_CHECK_(بیان, check_پایله) افعل { بیان; CV_OCL_DBG_CHECK_پایله(check_پایله, #expr); } طالما (0)
#تعریف_کړه CV_OCL_DBG_CHECK(بیان) افعل { cl_int __cl_result = (بیان); CV_OCL_DBG_CHECK_پایله(__cl_result, #expr); } طالما (0)
#د_که_پای


ساكن ثابت منطقي CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", ریښتیا);
ساكن ثابت منطقي CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", ریښتیا);
ساكن ثابت منطقي CV_OPENCL_CACHE_قلف_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", ریښتیا);
ساكن ثابت منطقي CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", ریښتیا);

#إذا CV_OPENCL_VALIDATE_دوګونی_PROGRAMS
ساكن ثابت منطقي CV_OPENCL_VALIDATE_دوګونی_PROGRAMS_ارزښت = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", خطأ);
#د_که_پای

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
ساكن ثابت منطقي CV_OPENCL_DISABLE_بفر_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#إذا 1 //def __APPLE__
        ریښتیا
#وإلا
        خطأ
#د_که_پای
);

ساكن String getBuildExtraOptions()
{
    ساكن String param_buildExtraOptions;
    ساكن منطقي initialized = خطأ;
    إذا (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = ریښتیا;
        إذا (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(عدم, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    أرجع param_buildExtraOptions;
}

ساكن ثابت منطقي CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", ریښتیا);
ساكن ثابت size_t CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


بنية UMat2D
{
    UMat2D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        ګام = (صحيح)m.ګام;
        لیکې = m.لیکې;
        ستون_شمېر = m.ستون_شمېر;
    }
    صحيح offset;
    صحيح ګام;
    صحيح لیکې;
    صحيح ستون_شمېر;
};

بنية UMat3D
{
    UMat3D(ثابت UMat& m)
    {
        offset = (صحيح)m.offset;
        ګام = (صحيح)m.ګام.p[1];
        slicestep = (صحيح)m.ګام.p[0];
        slices = (صحيح)m.size.p[0];
        لیکې = m.size.p[1];
        ستون_شمېر = m.size.p[2];
    }
    صحيح offset;
    صحيح slicestep;
    صحيح ګام;
    صحيح slices;
    صحيح لیکې;
    صحيح ستون_شمېر;
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
        initialized = ریښتیا;
    }

    uint64 دوراني_ازموینه = ~crc0;
    لكل( size_t idx = 0; idx < size; idx++ )
        دوراني_ازموینه = table[(uchar)دوراني_ازموینه ^ معلومات[idx]] ^ (دوراني_ازموینه >> 8);

    أرجع ~دوراني_ازموینه;
}

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
بنية OpenCLBinaryCacheConfigurator
{
    cv::String cache_لار;
    cv::String cache_قلف_filename;
    cv::Ptr<utils::fs::File_قلف> cache_قلف;

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
        cache_لار = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        إذا (cache_لار.empty())
        {
            CV_LOG_INFO(عدم, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        افعل
        {
            حاول
            {
                إذا (cache_لار.empty())
                    اكسر;
                إذا (cache_لار == "disabled")
                    اكسر;
                إذا (!utils::fs::createDirectories(cache_لار))
                {
                    CV_LOG_DEBUG(عدم, "Can't use OpenCL cache directory: " << cache_لار);
                    clear();
                    اكسر;
                }

                إذا (CV_OPENCL_CACHE_قلف_ENABLE)
                {
                    cache_قلف_filename = cache_لار + ".lock";
                    إذا (!utils::fs::exists(cache_قلف_filename))
                    {
                        CV_LOG_DEBUG(عدم, "Creating lock file... (" << cache_قلف_filename << ")");
                        std::ofstream قلف_filename(cache_قلف_filename.c_str(), std::ios::out);
                        إذا (!قلف_filename.is_open())
                        {
                            CV_LOG_WARNING(عدم, "Can't create lock file for OpenCL program cache: " << cache_قلف_filename);
                            اكسر;
                        }
                    }

                    حاول
                    {
                        cache_قلف = makePtr<utils::fs::File_قلف>(cache_قلف_filename.c_str());
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... (" << cache_قلف_filename << ")");
                        {
                            utils::shared_قلف_guard<utils::fs::File_قلف> lock(*cache_قلف);
                        }
                        CV_LOG_VERBOSE(عدم, 0, "Checking cache lock... Done!");
                    }
                    امسك (ثابت cv::Exception& e)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_قلف_filename << std::کرښه_پای << e.what());
                    }
                    امسك (...)
                    {
                        CV_LOG_WARNING(عدم, "Can't create OpenCL program cache lock: " << cache_قلف_filename);
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
                CV_LOG_WARNING(عدم, "Can't prepare OpenCL program cache: " << cache_لار << std::کرښه_پای << e.what());
                clear();
            }
        } طالما (0);
        إذا (!cache_لار.empty())
        {
            إذا (cache_قلف.empty() && CV_OPENCL_CACHE_قلف_ENABLE)
            {
                CV_LOG_WARNING(عدم, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            وإلا
            {
                CV_LOG_INFO(عدم, "Successfully initialized OpenCL cache directory: " << cache_لار);
            }
        }
    }

    فراغ clear()
    {
        cache_لار.clear();
        cache_قلف_filename.clear();
        cache_قلف.release();
    }

    std::string prepareCacheDirectoryForContext(ثابت std::string& ctx_مختاړی,
            ثابت std::string& cleanup_مختاړی)
    {
        إذا (cache_لار.empty())
            أرجع std::string();

        Auto_قلف lock(mutex_prepared_contexts_);

        ContextCacheType::iterator وموندل_it = prepared_contexts_.find(ctx_مختاړی);
        إذا (وموندل_it != prepared_contexts_.end())
            أرجع وموندل_it->second;

        CV_LOG_INFO(عدم, "Preparing OpenCL cache configuration for context: " << ctx_مختاړی);

        std::string موخه_directory = cache_لار + ctx_مختاړی + "/";
        منطقي پایله = utils::fs::isDirectory(موخه_directory);
        إذا (!پایله)
        {
            حاول
            {
                CV_LOG_VERBOSE(عدم, 0, "Creating directory: " << موخه_directory);
                إذا (utils::fs::createDirectories(موخه_directory))
                {
                    پایله = ریښتیا;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "Can't create directory: " << موخه_directory);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_ERROR(عدم, "Can't create OpenCL program cache directory for context: " << موخه_directory << std::کرښه_پای << e.what());
            }
        }
        موخه_directory = پایله ? موخه_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_مختاړی, موخه_directory));

        إذا (پایله && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_مختاړی.empty())
        {
            حاول
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_لار, cleanup_مختاړی + "*", entries, خطأ, ریښتیا);
                std::vector<String> remove_entries;
                لكل (size_t i = 0; i < entries.size(); i++)
                {
                    ثابت String& name = entries[i];
                    إذا (0 == name.find(cleanup_مختاړی))
                    {
                        إذا (0 == name.find(ctx_مختاړی))
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
                        cv::String لار = utils::fs::join(cache_لار, name);
                        حاول
                        {
                            utils::fs::remove_all(لار);
                            CV_LOG_WARNING(عدم, "Removed: " << لار);
                        }
                        امسك (ثابت cv::Exception& e)
                        {
                            CV_LOG_ERROR(عدم, "Exception during removal of obsolete OpenCL cache directory: " << لار << std::کرښه_پای << e.what());
                        }
                    }
                }
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(عدم, 1, "  Result: " << (موخه_directory.empty() ? std::string("Failed") : موخه_directory));
        أرجع موخه_directory;
    }

    ساكن OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, جديد OpenCLBinaryCacheConfigurator());
    }
};
صنف دوګونی_پروګرام_File
{
    تعداد { MAX_ENTRIES = 64 };

    تعريفنوع بینښه صحيح uint32_t;

    بنية CV_DECL_ALIGNED(4) File_سرلیک
    {
        uint32_t سرچینه_Signature_اندازه;
        //char sourceSignature[];
    };

    بنية CV_DECL_ALIGNED(4) File_جدول
    {
        uint32_t شمېره_د_Entries;
        //uint32_t firstEntryOffset[];
    };

    بنية CV_DECL_ALIGNED(4) File_ثبت
    {
        uint32_t بل_ثبت_File_دننه_واټن; // 0 for the last entry in chain
        uint32_t کلی_اندازه;
        uint32_t معلومات_اندازه;
        //char key[];
        //char data[];
    };

    ثابت std::string fileName_;
    ثابت حرف* ثابت سرچینه_Signature;
    ثابت size_t سرچینه_Signature_اندازه_2;

    std::fstream f;

    uint32_t ثبت_Offsets[MAX_ENTRIES];

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
    دوګونی_پروګرام_File(ثابت std::string& fileName, ثابت حرف* سرچینه_Signature_2)
        : fileName_(fileName), سرچینه_Signature(سرچینه_Signature_2), سرچینه_Signature_اندازه_2(سرچینه_Signature ? مزي_اوږدوالی(سرچینه_Signature) : 0)
    {
        CV_StaticAssert(حجم(uint32_t) == 4, "");
        CV_Assert(سرچینه_Signature != عدم);
        CV_Assert(سرچینه_Signature_اندازه_2 > 0);
        memset(ثبت_Offsets, 0, حجم(ثبت_Offsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.open(fileName_.c_str(), std::ios::in|std::ios::out|std::ios::binary);
        إذا(f.is_open() && get_File_اندازه() > 0)
        {
            منطقي isValid = خطأ;
            حاول
            {
                uint32_t file_سرچینه_Signature_اندازه = readUInt32();
                إذا (file_سرچینه_Signature_اندازه == سرچینه_Signature_اندازه_2)
                {
                    cv::Auto_بفر<حرف> file_سرچینه_Signature(file_سرچینه_Signature_اندازه + 1);
                    f.اقرأ(file_سرچینه_Signature.معلومات(), file_سرچینه_Signature_اندازه);
                    إذا (f.eof())
                    {
                        CV_LOG_ERROR(عدم, "Unexpected EOF");
                    }
                    وإلا إذا (memcmp(سرچینه_Signature_2, file_سرچینه_Signature.معلومات(), file_سرچینه_Signature_اندازه) == 0)
                    {
                        isValid = ریښتیا;
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

    منطقي اقرأ(ثابت std::string& کلی, std::vector<حرف>& buf)
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
        uint32_t file_سرچینه_Signature_اندازه = readUInt32();
        CV_Assert(file_سرچینه_Signature_اندازه > 0);
        seekReadRelative(file_سرچینه_Signature_اندازه);

        uint32_t شمېره_د_Entries = readUInt32();
        CV_Assert(شمېره_د_Entries > 0);
        إذا (شمېره_د_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        f.اقرأ((حرف*)&ثبت_Offsets[0], حجم(ثبت_Offsets));
        CV_Assert(!f.fail());

        uint32_t ثبت_عدد = getHash(کلی);

        uint32_t ثبت_دننه_واټن = ثبت_Offsets[ثبت_عدد];
        File_ثبت ثبت;
        طالما (ثبت_دننه_واټن > 0)
        {
            seekReadAbsolute(ثبت_دننه_واټن);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&ثبت, حجم(ثبت));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلی(ثبت.کلی_اندازه + 1);
            إذا (کلی.size() == ثبت.کلی_اندازه)
            {
                إذا (ثبت.کلی_اندازه > 0)
                {
                    f.اقرأ(file_کلی.معلومات(), ثبت.کلی_اندازه);
                    CV_Assert(!f.fail());
                }
                إذا (memcmp(file_کلی.معلومات(), کلی.c_str(), ثبت.کلی_اندازه) == 0)
                {
                    buf.resize(ثبت.معلومات_اندازه);
                    f.اقرأ(&buf[0], ثبت.معلومات_اندازه);
                    CV_Assert(!f.fail());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(عدم, 0, "Read...");
                    أرجع ریښتیا;
                }
            }
            إذا (ثبت.بل_ثبت_File_دننه_واټن == 0)
                اكسر;
            ثبت_دننه_واټن = ثبت.بل_ثبت_File_دننه_واټن;
        }
        أرجع خطأ;
    }

    منطقي اكتب(ثابت std::string& کلی, std::vector<حرف>& buf)
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
            writeUInt32((uint32_t)سرچینه_Signature_اندازه_2);
            f.اكتب(سرچینه_Signature, سرچینه_Signature_اندازه_2);
            CV_Assert(!f.fail());

            writeUInt32(MAX_ENTRIES);
            memset(ثبت_Offsets, 0, حجم(ثبت_Offsets));
            f.اكتب((حرف*)ثبت_Offsets, حجم(ثبت_Offsets));
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
        uint32_t file_سرچینه_Signature_اندازه = readUInt32();
        CV_Assert(file_سرچینه_Signature_اندازه == سرچینه_Signature_اندازه_2);
        seekReadRelative(file_سرچینه_Signature_اندازه);

        uint32_t شمېره_د_Entries = readUInt32();
        CV_Assert(شمېره_د_Entries > 0);
        إذا (شمېره_د_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(عدم, "Invalid file: " << fileName_);
            clearFile();
            أرجع خطأ;
        }
        size_t جدول_Entries_دننه_واټن = (size_t)f.tellg();
        f.اقرأ((حرف*)&ثبت_Offsets[0], حجم(ثبت_Offsets));
        CV_Assert(!f.fail());

        uint32_t ثبت_عدد = getHash(کلی);

        uint32_t ثبت_دننه_واټن = ثبت_Offsets[ثبت_عدد];
        File_ثبت ثبت;
        طالما (ثبت_دننه_واټن > 0)
        {
            seekReadAbsolute(ثبت_دننه_واټن);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.اقرأ((حرف*)&ثبت, حجم(ثبت));
            CV_Assert(!f.fail());
            cv::Auto_بفر<حرف> file_کلی(ثبت.کلی_اندازه + 1);
            إذا (کلی.size() == ثبت.کلی_اندازه)
            {
                إذا (ثبت.کلی_اندازه > 0)
                {
                    f.اقرأ(file_کلی.معلومات(), ثبت.کلی_اندازه);
                    CV_Assert(!f.fail());
                }
                إذا (0 == memcmp(file_کلی.معلومات(), کلی.c_str(), ثبت.کلی_اندازه))
                {
                    // duplicate
                    CV_LOG_VERBOSE(عدم, 0, "Duplicate key ignored: " << fileName_);
                    أرجع خطأ;
                }
            }
            إذا (ثبت.بل_ثبت_File_دننه_واټن == 0)
                اكسر;
            ثبت_دننه_واټن = ثبت.بل_ثبت_File_دننه_واټن;
        }
        seekReadAbsolute(0);
        إذا (ثبت_دننه_واټن > 0)
        {
            seekWriteAbsolute(ثبت_دننه_واټن);
            ثبت.بل_ثبت_File_دننه_واټن = (uint32_t)file_اندازه;
            f.اكتب((حرف*)&ثبت, حجم(ثبت));
            CV_Assert(!f.fail());
        }
        وإلا
        {
            ثبت_Offsets[ثبت_عدد] = (uint32_t)file_اندازه;
            seekWriteAbsolute(جدول_Entries_دننه_واټن);
            f.اكتب((حرف*)ثبت_Offsets, حجم(ثبت_Offsets));
            CV_Assert(!f.fail());
        }
        seekWriteAbsolute(file_اندازه);
        ثبت.بل_ثبت_File_دننه_واټن = 0;
        ثبت.معلومات_اندازه = (uint32_t)buf.size();
        ثبت.کلی_اندازه = (uint32_t)کلی.size();
        f.اكتب((حرف*)&ثبت, حجم(ثبت));
        CV_Assert(!f.fail());
        f.اكتب(کلی.c_str(), ثبت.کلی_اندازه);
        CV_Assert(!f.fail());
        f.اكتب(&buf[0], ثبت.معلومات_اندازه);
        CV_Assert(!f.fail());
        f.flush();
        CV_Assert(!f.fail());
        CV_LOG_VERBOSE(عدم, 0, "Write... (" << buf.size() << " bytes)");
        أرجع ریښتیا;
    }
};
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT



بنية OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    صحيح device_;  // device index in context
    ocl::کتار کتار_3;
    صحيح useOpenCL_;

محمي:
    Impl() = ړنګول;

    فراغ _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي وموندل = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            إذا (dhandle == deviceID)
            {
                device_ = i;
                وموندل = ریښتیا;
                اكسر;
            }
        }
        CV_Assert(وموندل && "OpenCL device can't work with passed OpenCL context");
    }

    فراغ _init_device(ثابت ocl::Device& device)
    {
        CV_Assert(device.ptr());
        صحيح ndevices = (صحيح)context_.ndevices();
        CV_Assert(ndevices > 0);
        منطقي وموندل = خطأ;
        لكل (صحيح i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            إذا (d.getImpl() == device.getImpl())
            {
                device_ = i;
                وموندل = ریښتیا;
                اكسر;
            }
        }
        CV_Assert(وموندل && "OpenCL device can't work with passed OpenCL context");
    }

عام:
    Impl(cl_platform_id platformID, cl_context context, cl_device_id deviceID)
        : device_(0), useOpenCL_(-1)
    {
        CV_ناکارول_شوی(platformID);
        CV_Assert(context);
        CV_Assert(deviceID);

        context_ = Context::fromHandle(context);
        _init_device(deviceID);
        کتار_3 = کتار(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device, ثابت ocl::کتار& کتار_2)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        کتار_3 = کتار_2;
    }

    Impl(ثابت ocl::Context& context, ثابت ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        کتار_3 = کتار(context_, context_.device(device_));
    }

    Impl(ثابت ocl::Context& context, ثابت صحيح device, ثابت ocl::کتار& کتار_2)
        : context_(context)
        , device_(device)
        , کتار_3(کتار_2)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(ثابت Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , کتار_3(other.کتار_3)
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
                    useOpenCL_ = d.شته();
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

    فراغ سېټ_Use_Open_CL(منطقي flag)
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
            cv::Auto_قلف lock(getInitializationMutex());
            إذا (!initialized)
            {
                CV_LOG_INFO(عدم, "OpenCL: creating new execution context...");
                حاول
                {
                    Context c = ocl::Context::create(std::string());
                    إذا (c.ndevices())
                    {
                        صحيح deviceId = 0;
                        خپلکاری& d = c.device(deviceId);
                        إذا (d.شته())
                        {
                            خپلکاری q = ocl::کتار(c, d);
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
                initialized = ریښتیا;
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
کتار& OpenCLExecutionContext::get_کتار() ثابت
{
    CV_Assert(p);
    أرجع p->کتار_3;
}

منطقي OpenCLExecutionContext::useOpenCL() ثابت
{
    إذا (p)
        أرجع p->useOpenCL();
    أرجع خطأ;
}
فراغ OpenCLExecutionContext::سېټ_Use_Open_CL(منطقي flag)
{
    CV_Assert(p);
    p->سېټ_Use_Open_CL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    OpenCLExecutionContext& c = معلومات.oclExecutionContext;
    إذا (!معلومات.oclExecutionContextInitialized)
    {
        معلومات.oclExecutionContextInitialized = ریښتیا;
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
    معلومات.oclExecutionContextInitialized = ریښتیا;
    معلومات.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_کتار() ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    ثابت کتار q(getContext(), getDevice());
    أرجع clone_With_New_کتار(q);
}

OpenCLExecutionContext OpenCLExecutionContext::clone_With_New_کتار(ثابت ocl::کتار& q) ثابت
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != عدم);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    أرجع c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(ثابت Context& context, ثابت Device& device, ثابت ocl::کتار& کتار_2)
{
    CV_TRACE_FUNCTION();
    إذا (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, کتار_2);
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
        std::string env_لار = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        إذا (!env_لار.empty())
        {
            إذا (env_لار == "disabled")
            {
                g_isOpenCLAvailable = خطأ;
                g_isOpenCLInitialized = ریښتیا;
                أرجع خطأ;
            }
        }

        cv::Auto_قلف lock(getInitializationMutex());
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
        g_isOpenCLInitialized = ریښتیا;
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
                خپلکاری c = OpenCLExecutionContext::getCurrent();
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

فراغ سېټ_Use_Open_CL(منطقي flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    خپلکاری& c = OpenCLExecutionContext::getCurrentRef();
    إذا (!c.empty())
    {
        c.سېټ_Use_Open_CL(flag);
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



#که_تعریف_شوی HAVE_CLAMDBLAS

صنف AmdBlasHelper
{
عام:
    ساكن AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, جديد AmdBlasHelper())
    }

    منطقي is_شته() ثابت
    {
        أرجع g_is_Amd_Blas_شته;
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
            Auto_قلف lock(getInitializationMutex());

            إذا (!g_isAmdBlasInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        g_is_Amd_Blas_شته = clblas_تیاری() == clblasSuccess;
                    }
                    امسك (...)
                    {
                        g_is_Amd_Blas_شته = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Blas_شته = خطأ;

                g_isAmdBlasInitialized = ریښتیا;
            }
        }
    }

خاص:
    ساكن منطقي g_isAmdBlasInitialized;
    ساكن منطقي g_is_Amd_Blas_شته;
};

منطقي AmdBlasHelper::g_is_Amd_Blas_شته = خطأ;
منطقي AmdBlasHelper::g_isAmdBlasInitialized = خطأ;

منطقي haveAmdBlas()
{
    أرجع AmdBlasHelper::getInstance().is_شته();
}

#وإلا

منطقي haveAmdBlas()
{
    أرجع خطأ;
}

#د_که_پای

#که_تعریف_شوی HAVE_CLAMDFFT

صنف AmdFftHelper
{
عام:
    ساكن AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, جديد AmdFftHelper())
    }

    منطقي is_شته() ثابت
    {
        أرجع g_is_Amd_Fft_شته;
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
            Auto_قلف lock(getInitializationMutex());

            إذا (!g_isAmdFftInitialized)
            {
                إذا (haveOpenCL())
                {
                    حاول
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_تیاری_معلومات(&تیاری_معلومات) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfft_Get_نسخه(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_is_Amd_Fft_شته = ریښتیا;
                    }
                    امسك (ثابت Exception &)
                    {
                        g_is_Amd_Fft_شته = خطأ;
                    }
                }
                وإلا
                    g_is_Amd_Fft_شته = خطأ;

                g_isAmdFftInitialized = ریښتیا;
            }
        }
    }

خاص:
    ساكن clfft_تیاری_معلومات تیاری_معلومات;
    ساكن منطقي g_isAmdFftInitialized;
    ساكن منطقي g_is_Amd_Fft_شته;
};

clfft_تیاری_معلومات AmdFftHelper::تیاری_معلومات;
منطقي AmdFftHelper::g_is_Amd_Fft_شته = خطأ;
منطقي AmdFftHelper::g_isAmdFftInitialized = خطأ;

منطقي haveAmdFft()
{
    أرجع AmdFftHelper::getInstance().is_شته();
}

#وإلا

منطقي haveAmdFft()
{
    أرجع خطأ;
}

#د_که_پای

منطقي haveSVM()
{
#که_تعریف_شوی HAVE_OPENCL_SVM
    أرجع ریښتیا;
#وإلا
    أرجع خطأ;
#د_که_پای
}

فراغ پای_2()
{
    کتار::getDefault().پای_2();
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

            initialized = ریښتیا;
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

Platform& Platform::چلوونکی = (ثابت Platform& pl)
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
    pl.p = تش_اشاره;
}

Platform& Platform::چلوونکی = (Platform&& pl) CV_NOEXCEPT
{
    إذا (هذا != &pl) {
        إذا(p)
            p->release();
        p = pl.p;
        pl.p = تش_اشاره;
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
    إذا (0 != مزي_پرتله(pstr, "OpenCL ", 7))
        أرجع;
    size_t ppos = نسخه.find('.', 7);
    إذا (String::npos == ppos)
        أرجع;
    String لنډمهاله_2 = نسخه.substr(7, ppos - 7);
    major = atoi(لنډمهاله_2.c_str());
    لنډمهاله_2 = نسخه.substr(ppos + 1);
    minor = atoi(لنډمهاله_2.c_str());
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
        max_Work_ډله_اندازه_2 = getProp<size_t, size_t>(CL_DEVICE_MAX_WORK_ډله_اندازه);
        type_ = getProp<cl_device_type, صحيح>(CL_DEVICE_TYPE);
        driver_نسخه_2 = getStrProp(CL_DRIVER_نسخه);
        پته_بټونه_2 = getProp<cl_uint, صحيح>(CL_DEVICE_پته_بټونه);

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
                extensions_سېټ.insert(extensionName);
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
        وإلا إذا (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || مزي_پلټنه(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        وإلا إذا (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        وإلا
            vendorID_ = UNKNOWN_VENDOR;

        ثابت size_t CV_OPENCL_DEVICE_MAX_WORK_ډله_اندازه = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        إذا (CV_OPENCL_DEVICE_MAX_WORK_ډله_اندازه > 0)
        {
            ثابت size_t new_max_Work_ډله_اندازه = std::min(max_Work_ډله_اندازه_2, CV_OPENCL_DEVICE_MAX_WORK_ډله_اندازه);
            إذا (new_max_Work_ډله_اندازه != max_Work_ډله_اندازه_2)
                CV_LOG_WARNING(عدم, "OpenCL: using workgroup size: " << new_max_Work_ډله_اندازه << " (was " << max_Work_ډله_اندازه_2 << ")");
            max_Work_ډله_اندازه_2 = new_max_Work_ډله_اندازه;
        }
#إذا 0
        إذا (isExtensionSupported("cl_khr_spir"))
        {
#که_نه_تعریف_شوی CL_DEVICE_SPIR_VERSIONS
#تعریف_کړه CL_DEVICE_SPIR_VERSIONS                     0x40E0
#د_که_پای
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::وتنه << spir_versions << std::کرښه_پای;
        }
#د_که_پای
    }

    ~Impl()
    {
#که_تعریف_شوی _WIN32
        إذا (!cv::__termination)
#د_که_پای
        {
            إذا (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    کينډۍ<ډول_نوم _TpCL, ډول_نوم Tp_بهر>
    Tp_بهر getProp(cl_device_info prop) ثابت
    {
        _TpCL لنډمهاله_2=_TpCL();
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(لنډمهاله_2), &لنډمهاله_2, &sz) == CL_SUCCESS &&
            sz == حجم(لنډمهاله_2) ? Tp_بهر(لنډمهاله_2) : Tp_بهر();
    }

    منطقي getBoolProp(cl_device_info prop) ثابت
    {
        cl_bool لنډمهاله_2 = CL_FALSE;
        size_t sz = 0;

        أرجع clGetDeviceInfo(handle, prop, حجم(لنډمهاله_2), &لنډمهاله_2, &sz) == CL_SUCCESS &&
            sz == حجم(لنډمهاله_2) ? لنډمهاله_2 != 0 : خطأ;
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
        أرجع extensions_سېټ.شمېر_2(extensionName) > 0;
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
    size_t max_Work_ډله_اندازه_2;
    صحيح type_;
    صحيح پته_بټونه_2;
    صحيح device_نسخه_Major_2;
    صحيح device_نسخه_Minor_2;
    String driver_نسخه_2;
    String vendorName_;
    صحيح vendorID_;
    منطقي intelSubgroupsSupport_;

    std::سېټ<std::string> extensions_سېټ;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(فراغ* d)
{
    p = 0;
    سېټ(d);
}

Device::Device(ثابت Device& d)
{
    p = d.p;
    إذا(p)
        p->addref();
}

Device& Device::چلوونکی = (ثابت Device& d)
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
    d.p = تش_اشاره;
}

Device& Device::چلوونکی = (Device&& d) CV_NOEXCEPT
{
    إذا (هذا != &d) {
        إذا(p)
            p->release();
        p = d.p;
        d.p = تش_اشاره;
    }
    أرجع *هذا;
}

Device::~Device()
{
    إذا(p)
        p->release();
}

فراغ Device::سېټ(فراغ* d)
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

صحيح Device::پته_بټونه() ثابت
{ أرجع p ? p->پته_بټونه_2 : 0; }

منطقي Device::شته() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_شته) : خطأ; }

منطقي Device::compiler_شته() ثابت
{ أرجع p ? p->getBoolProp(CL_DEVICE_COMPILER_شته) : خطأ; }

منطقي Device::linker_شته() ثابت
#که_تعریف_شوی CL_نسخه_1_2
{ أرجع p ? p->getBoolProp(CL_DEVICE_LINKER_شته) : خطأ; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#د_که_پای

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

صحيح Device::global_Mem_Cache_کرښه_اندازه() ثابت
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

منطقي Device::image_From_بفر_Support() ثابت
{
    أرجع p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : خطأ;
}

uint Device::imagePitchAlignment() ثابت
{
#که_تعریف_شوی CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#د_که_پای
}

uint Device::image_بنسټ_پته_Alignment() ثابت
{
#که_تعریف_شوی CL_DEVICE_IMAGE_بنسټ_پته_ALIGNMENT
    أرجع p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_بنسټ_پته_ALIGNMENT) : 0;
#وإلا
    أرجع 0;
#د_که_پای
}

size_t Device::image2_DMax_پلنوالی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_پلنوالی) : 0; }

size_t Device::image2_DMax_لوړوالی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE2_D_MAX_لوړوالی) : 0; }

size_t Device::image3_DMax_پلنوالی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_پلنوالی) : 0; }

size_t Device::image3_DMax_لوړوالی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_لوړوالی) : 0; }

size_t Device::image3_DMax_ژوروالی() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE3_D_MAX_ژوروالی) : 0; }

size_t Device::image_Max_بفر_اندازه() ثابت
#که_تعریف_شوی CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_بفر_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#د_که_پای

size_t Device::image_Max_Array_اندازه() ثابت
#که_تعریف_شوی CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_IMAGE_MAX_ARRAY_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#د_که_پای

منطقي Device::intelSubgroupsSupport() ثابت
{ أرجع p ? p->intelSubgroupsSupport_ : خطأ; }

صحيح Device::maxClockFrequency() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

صحيح Device::maxComputeUnits() ثابت
{ أرجع p ? p->maxComputeUnits_ : 0; }

صحيح Device::max_Constant_دلیلونه() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_CONSTANT_دلیلونه) : 0; }

size_t Device::max_Constant_بفر_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_CONSTANT_بفر_اندازه) : 0; }

size_t Device::max_Mem_Alloc_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_MEM_ALLOC_اندازه) : 0; }

size_t Device::max_Parameter_اندازه() ثابت
{ أرجع p ? p->getProp<cl_ulong, size_t>(CL_DEVICE_MAX_PARAMETER_اندازه) : 0; }

صحيح Device::max_Read_Image_دلیلونه() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_READ_IMAGE_دلیلونه) : 0; }

صحيح Device::max_Write_Image_دلیلونه() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WRITE_IMAGE_دلیلونه) : 0; }

صحيح Device::maxSamplers() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_SAMPLERS) : 0; }

size_t Device::max_Work_ډله_اندازه() ثابت
{ أرجع p ? p->max_Work_ډله_اندازه_2 : 0; }

صحيح Device::max_Work_توکی_Dims() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MAX_WORK_توکی_DIMENSIONS) : 0; }

فراغ Device::max_Work_توکی_Sizes(size_t* sizes) ثابت
{
    إذا(p)
    {
        ثابت صحيح MAX_DIMS = 32;
        size_t retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_توکی_SIZES,
                MAX_DIMS*حجم(sizes[0]), &sizes[0], &retsz));
    }
}

صحيح Device::mem_بنسټ_Addr_Align() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_MEM_بنسټ_ADDR_ALIGN) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_CHAR) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_SHORT) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_INT) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_LONG) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_FLOAT) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_DOUBLE) : 0; }

صحيح Device::native_ویکټور_پلنوالی_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_NATIVE_ویکټور_پلنوالی_HALF) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Char() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_CHAR) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Short() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_SHORT) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Int() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_INT) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Long() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_LONG) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Float() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_FLOAT) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Double() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_DOUBLE) : 0; }

صحيح Device::preferred_ویکټور_پلنوالی_Half() ثابت
{ أرجع p ? p->getProp<cl_uint, صحيح>(CL_DEVICE_PREFERRED_ویکټور_پلنوالی_HALF) : 0; }

size_t Device::printf_بفر_اندازه() ثابت
#که_تعریف_شوی CL_نسخه_1_2
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PRINTF_بفر_اندازه) : 0; }
#وإلا
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#د_که_پای


size_t Device::profilingTimerResolution() ثابت
{ أرجع p ? p->getProp<size_t, size_t>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

ثابت Device& Device::getDefault()
{
    خپلکاری& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        أرجع c.getDevice();
    }

    ساكن Device dummy;
    أرجع dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

کينډۍ <ډول_نوم Functor, ډول_نوم ObjectType>
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

ساكن فراغ وېشل(ثابت std::string &s, حرف delim, std::vector<std::string> &elems)
{
    elems.clear();
    إذا (s.size() == 0)
        أرجع;
    std::istringstream ss(s);
    std::string توکی;
    طالما (!ss.eof())
    {
        std::getline(ss, توکی, delim);
        elems.push_back(توکی);
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
    وېشل(configurationStr, ':', parts);
    إذا (parts.size() > 3)
    {
        CV_LOG_ERROR(عدم, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        أرجع خطأ;
    }
    إذا (parts.size() > 2)
        deviceNameOrID = parts[2];
    إذا (parts.size() > 1)
    {
        وېشل(parts[1], '|', deviceTypes);
    }
    إذا (parts.size() > 0)
    {
        platform = parts[0];
    }
    أرجع ریښتیا;
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
        isID = ریښتیا;
        لكل (size_t i = 0; i < deviceName.length(); i++)
        {
            إذا (!که_عدد(deviceName[i]))
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
            اذهب not_وموندل;
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
        std::string لنډمهاله_Str_Device_Type = deviceTypes[t];
        std::transform(لنډمهاله_Str_Device_Type.begin(), لنډمهاله_Str_Device_Type.end(), لنډمهاله_Str_Device_Type.begin(), details::char_tolower);

        إذا (لنډمهاله_Str_Device_Type == "gpu" || لنډمهاله_Str_Device_Type == "dgpu" || لنډمهاله_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        وإلا إذا (لنډمهاله_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        وإلا إذا (لنډمهاله_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        وإلا إذا (لنډمهاله_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        وإلا
        {
            CV_LOG_ERROR(عدم, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            اذهب not_وموندل;
        }

        std::vector<cl_device_id> devices;
        لكل (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint شمېر_2 = 0;
            cl_int حالت = clGetDeviceIDs(*currentPlatform, deviceType, 0, عدم, &شمېر_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_وموندل))
            {
                CV_OCL_DBG_CHECK_پایله(حالت, "clGetDeviceIDs get count");
            }
            إذا (شمېر_2 == 0)
                استمر;
            size_t بنسټ = devices.size();
            devices.resize(بنسټ + شمېر_2);
            حالت = clGetDeviceIDs(*currentPlatform, deviceType, شمېر_2, &devices[بنسټ], &شمېر_2);
            إذا (!(حالت == CL_SUCCESS || حالت == CL_DEVICE_NOT_وموندل))
            {
                CV_OCL_DBG_CHECK_پایله(حالت, "clGetDeviceIDs get IDs");
            }
        }

        لكل (size_t i = (isID ? deviceID : 0);
             (isID ? (i == (size_t)deviceID) : ریښتیا) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = ریښتیا;
            إذا(لنډمهاله_Str_Device_Type == "dgpu" || لنډمهاله_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_حافظه, حجم(isIGPU), &isIGPU, عدم));
                useGPU = لنډمهاله_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            إذا ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                أرجع devices[i];
            }
        }
    }

not_وموندل:
    إذا (configuration.empty())
        أرجع عدم; // suppress messages on stderr

    std::ostringstream پیغام;
    پیغام << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::کرښه_پای
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::کرښه_پای
        << "    Device types:";
    لكل (size_t t = 0; t < deviceTypes.size(); t++)
        پیغام << ' ' << deviceTypes[t];

    پیغام << std::کرښه_پای << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(عدم, پیغام.str());
    أرجع عدم;
}

#که_تعریف_شوی HAVE_OPENCL_SVM
نطاق svm {

تعداد AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_بفر = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_بفر = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_بفر_ماسک = 3 << 16,
        OPENCL_SVM_بفر_MAP = 4 << 16
};

ساكن منطقي checkForceSVMUmatUsage()
{
    ساكن منطقي initialized = خطأ;
    ساكن منطقي force = خطأ;
    إذا (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", خطأ);
        initialized = ریښتیا;
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
        initialized = ریښتیا;
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
        initialized = ریښتیا;
    }
    أرجع force;
}
// see SVMCapabilities
ساكن بینښه صحيح get_SVMCapabilities_ماسک()
{
    ساكن منطقي initialized = خطأ;
    ساكن بینښه صحيح ماسک = 0;
    إذا (!initialized)
    {
        ثابت std::string env_ارزښت = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        إذا (env_ارزښت.empty())
        {
            أرجع ~0U; // all bits 1
        }
        ماسک = atoi(env_ارزښت.c_str());
        initialized = ریښتیا;
    }
    أرجع ماسک;
}
} // namespace
#د_که_پای

ساكن size_t get_پروګرام_شمېر_حد()
{
    ساكن منطقي initialized = خطأ;
    ساكن size_t شمېر_2 = 0;
    إذا (!initialized)
    {
        شمېر_2 = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = ریښتیا;
    }
    أرجع شمېر_2;
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
#که_تعریف_شوی HAVE_OPENCL_SVM
        , svmInitialized(خطأ)
#د_که_پای
    {
        إذا (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::Auto_قلف lock(cv::getInitializationMutex());
        خپلکاری& container = getGlobalContainer();
        container.resize(std::max(container.size(), (size_t)contextId + 1));
        container[contextId] = هذا;
    }

    ~Impl()
    {
#که_تعریف_شوی _WIN32
        إذا (!cv::__termination)
#د_که_پای
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
            cv::Auto_قلف lock(cv::getInitializationMutex());
            خپلکاری& container = getGlobalContainer();
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
        size_t devices_ret_اندازه = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * حجم(cl_device_id), &cl_devices[0], &devices_ret_اندازه));
        CV_CheckEQ(devices_ret_اندازه, cl_devices.size() * حجم(cl_device_id), "");

        devices.clear();
        لكل (بینښه i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    فراغ __init_buffer_pools();  // w/o synchronization
    فراغ init_بفر_pools() ثابت
    {
        إذا (!بفر_Pool_2)
        {
            cv::Auto_قلف lock(cv::getInitializationMutex());
            إذا (!بفر_Pool_2)
            {
                const_cast<Impl*>(هذا)->__init_buffer_pools();
            }
        }
    }
عام:
    ساكن Impl* پیدا_کړه_Context(ثابت std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::Auto_قلف lock(cv::getInitializationMutex());
        خپلکاری& container = getGlobalContainer();
        إذا (configuration.empty() && !container.empty())
            أرجع container[0];
        لكل (خپلکاری it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            إذا (i && i->configuration == configuration)
            {
                أرجع i;
            }
        }
        أرجع عدم;
    }

    ساكن Impl* پیدا_کړه_Or_Create_Context(ثابت std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        إذا (configuration_.empty())
        {
            ثابت std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            إذا (!c.empty())
                configuration = c;
        }
        Impl* impl = پیدا_کړه_Context(configuration);
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
            ړنګول impl;
            أرجع عدم;
        }
        امسك (...)
        {
            ړنګول impl;
            ارم;
        }
    }

    ساكن Impl* پیدا_کړه_Or_Create_Context(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (فراغ*)h);
        Impl* impl = پیدا_کړه_Context(configuration);
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
            ړنګول impl;
            ارم;
        }
    }

    ساكن Impl* پیدا_کړه_Or_Create_Context(ثابت ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (فراغ*)d);
        Impl* impl = پیدا_کړه_Context(configuration);
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
            ړنګول impl;
            ارم;
        }
    }

    فراغ سېټ_Default()
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
        CV_OCL_DBG_CHECK_پایله(حالت, "clCreateContext");

        منطقي ok = handle != 0 && حالت == CL_SUCCESS;
        إذا( ok )
        {
            devices.resize(nd);
            devices[0].سېټ(d);
        }
        وإلا
            handle = عدم;
    }

    پروګرام_2 getProg(ثابت پروګرام_سرچینه& src, ثابت String& buildflags, String& errmsg);

    فراغ unloadProg(پروګرام_2& prog)
    {
        cv::Auto_قلف lock(پروګرام_cache_mutex);
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

    std::string& get_مختاړی_String()
    {
        إذا (مختاړی.empty())
        {
            cv::Auto_قلف lock(پروګرام_cache_mutex);
            إذا (مختاړی.empty())
            {
                CV_Assert(!devices.empty());
                ثابت Device& d = devices[0];
                صحيح بټونه_2 = d.پته_بټونه();
                إذا (بټونه_2 > 0 && بټونه_2 != 64)
                    مختاړی = cv::format("%d-bit--", بټونه_2);
                مختاړی += d.vendorName() + "--" + d.name() + "--" + d.driver_نسخه();
                // sanitize chars
                لكل (size_t i = 0; i < مختاړی.size(); i++)
                {
                    حرف c = مختاړی[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        مختاړی[i] = '_';
                    }
                }
            }
        }
        أرجع مختاړی;
    }

    std::string& get_مختاړی_بنسټ()
    {
        إذا (مختاړی_بنسټ.empty())
        {
            cv::Auto_قلف lock(پروګرام_cache_mutex);
            إذا (مختاړی_بنسټ.empty())
            {
                ثابت Device& d = devices[0];
                صحيح بټونه_2 = d.پته_بټونه();
                إذا (بټونه_2 > 0 && بټونه_2 != 64)
                    مختاړی_بنسټ = cv::format("%d-bit--", بټونه_2);
                مختاړی_بنسټ += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                لكل (size_t i = 0; i < مختاړی_بنسټ.size(); i++)
                {
                    حرف c = مختاړی_بنسټ[i];
                    إذا (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        مختاړی_بنسټ[i] = '_';
                    }
                }
            }
        }
        أرجع مختاړی_بنسټ;
    }

    IMPLEMENT_REFCOUNTABLE();

    ثابت صحيح contextId;  // global unique ID
    ثابت std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string مختاړی;
    std::string مختاړی_بنسټ;

    cv::Mutex پروګرام_cache_mutex;
    تعريفنوع std::map<std::string, پروګرام_2> phash_t;
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

    std::map<std::type_شاخص, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    فراغ سېټ_User_Context(std::type_شاخص typeId, ثابت std::shared_ptr<UserContext>& userContext) {
        cv::Auto_قلف lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_شاخص typeId) {
        cv::Auto_قلف lock(userContextMutex);
        خپلکاری it = userContextStorage.find(typeId);
        إذا (it != userContextStorage.end())
            أرجع it->second;
        وإلا
            أرجع تش_اشاره;
    }

#که_تعریف_شوی HAVE_OPENCL_SVM
    منطقي svmInitialized;
    منطقي svm_شته;
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
        svmCapabilities.ارزښت_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_بفر) ? svm::SVMCapabilities::SVM_FINE_GRAIN_بفر : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.ارزښت_2 &= svm::get_SVMCapabilities_ماسک();
        إذا (svmCapabilities.ارزښت_2 == 0)
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
                cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();
                إذا (CL_SUCCESS != cl_کتار_ته_ورزیاتول_SVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                cl_پای(q);
                حاول
                {
                    ((صحيح*)ptr)[0] = 100;
                }
                امسك (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = ریښتیا;
                }
                إذا (CL_SUCCESS != cl_کتار_ته_ورزیاتول_SVMUnmap(q, ptr, 0, عدم, عدم))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                cl_پای(q);
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
            svmFunctions.fn_cl_سېټ_Kernel_Arg_SVMPointer = cl_سېټ_Kernel_Arg_SVMPointer;
            //svmFunctions.fn_clSetKernelExecInfo = clSetKernelExecInfo;
            //svmFunctions.fn_clEnqueueSVMFree = clEnqueueSVMFree;
            svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMemcpy = cl_کتار_ته_ورزیاتول_SVMMemcpy;
            svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMem_Fill = cl_کتار_ته_ورزیاتول_SVMMem_Fill;
            svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMap = cl_کتار_ته_ورزیاتول_SVMMap;
            svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMUnmap = cl_کتار_ته_ورزیاتول_SVMUnmap;
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
                svmFunctions.fn_clSVMAlloc = (clSVMAllocAMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clSVMAllocAMD");
                svmFunctions.fn_clSVMFree = (clSVMFreeAMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clSVMFreeAMD");
                svmFunctions.fn_cl_سېټ_Kernel_Arg_SVMPointer = (cl_سېټ_Kernel_Arg_SVMPointer_AMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clSetKernelArgSVMPointerAMD");
                //svmFunctions.fn_clSetKernelExecInfo = (clSetKernelExecInfoAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clSetKernelExecInfoAMD");
                //svmFunctions.fn_clEnqueueSVMFree = (clEnqueueSVMFreeAMD_fn)clGetExtensionFunctionAddressForPlatform(p, "clEnqueueSVMFreeAMD");
                svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMemcpy = (cl_کتار_ته_ورزیاتول_SVMMemcpy_AMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clEnqueueSVMMemcpyAMD");
                svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMem_Fill = (cl_کتار_ته_ورزیاتول_SVMMem_Fill_AMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clEnqueueSVMMemFillAMD");
                svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMMap = (cl_کتار_ته_ورزیاتول_SVMMap_AMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clEnqueueSVMMapAMD");
                svmFunctions.fn_cl_کتار_ته_ورزیاتول_SVMUnmap = (cl_کتار_ته_ورزیاتول_SVMUnmap_AMD_fn)cl_Get_Extension_Function_پته_For_Platform(p, "clEnqueueSVMUnmapAMD");
                CV_Assert(svmFunctions.isValid());
            }
            امسك (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                اذهب noSVM;
            }
        }

        svm_شته = ریښتیا;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = ریښتیا;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        أرجع;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svm_شته = خطأ;
        svmEnabled = خطأ;
        svmCapabilities.ارزښت_2 = 0;
        svmInitialized = ریښتیا;
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
#د_که_پای

    ملګری صنف پروګرام_2;
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
    p = Impl::پیدا_کړه_Or_Create_Context(std::string());
    إذا (p && p->handle)
        أرجع ریښتیا;
    release();
    أرجع خطأ;
}

// deprecated
منطقي Context::create(صحيح dtype)
{
    إذا( !haveOpenCL() )
        أرجع خطأ;
    release();
    إذا (dtype == CL_DEVICE_TYPE_DEFAULT || (بینښه)dtype == (بینښه)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::پیدا_کړه_Or_Create_Context("");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::پیدا_کړه_Or_Create_Context(":GPU:");
    }
    وإلا إذا (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::پیدا_کړه_Or_Create_Context(":CPU:");
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

Context& Context::چلوونکی = (ثابت Context& c)
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
    c.p = تش_اشاره;
}

Context& Context::چلوونکی = (Context&& c) CV_NOEXCEPT
{
    إذا (هذا != &c) {
        إذا(p)
            p->release();
        p = c.p;
        c.p = تش_اشاره;
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
    خپلکاری& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خپلکاری& ctx = c.getContext();
        أرجع ctx;
    }

    ساكن Context dummy;
    أرجع dummy;
}

پروګرام_2 Context::getProg(ثابت پروګرام_سرچینه& prog,
                         ثابت String& buildopts, String& errmsg)
{
    أرجع p ? p->getProg(prog, buildopts, errmsg) : پروګرام_2();
}

فراغ Context::unloadProg(پروګرام_2& prog)
{
    إذا (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(فراغ* context)
{
    Context ctx;
    ctx.p = Impl::پیدا_کړه_Or_Create_Context((cl_context)context);
    أرجع ctx;
}

/* static */
Context Context::fromDevice(ثابت ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::پیدا_کړه_Or_Create_Context(device);
    أرجع ctx;
}

/* static */
Context Context::create(ثابت std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::پیدا_کړه_Or_Create_Context(configuration);
    أرجع ctx;
}

فراغ* Context::getOpenCLContextProperty(صحيح propertyId) ثابت
{
    إذا (p == عدم)
        أرجع تش_اشاره;
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
    أرجع تش_اشاره;
}

#که_تعریف_شوی HAVE_OPENCL_SVM
منطقي Context::useSVM() ثابت
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    أرجع i->svmEnabled;
}
فراغ Context::سېټ_Use_SVM(منطقي enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    إذا (!i->svmInitialized)
        i->svmInit();
    إذا (enabled && !i->svm_شته)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#وإلا
منطقي Context::useSVM() ثابت { أرجع خطأ; }
فراغ Context::سېټ_Use_SVM(منطقي enabled) { CV_Assert(!enabled); }
#د_که_پای

#که_تعریف_شوی HAVE_OPENCL_SVM
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
        أرجع ریښتیا;
    إذا (checkDisableSVMUMatUsage())
        أرجع خطأ;
    إذا ((usageFlags & USAGE_ALLOCATE_SHARED_حافظه) != 0)
        أرجع ریښتیا;
    أرجع خطأ; // don't use SVM by default
}

} // namespace cv::ocl::svm
#د_که_پای // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

فراغ Context::سېټ_User_Context(std::type_شاخص typeId, ثابت std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->سېټ_User_Context(typeId, userContext);
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
    خپلکاری ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
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

    منطقي platform_شته = خطأ;

    // check if external platformName contained in list of available platforms in OpenCV
    لكل (بینښه صحيح i = 0; i < cnt; i++)
    {
        String شته_Platform_Name;
        get_platform_name(platforms[i], شته_Platform_Name);
        // external platform is found in the list of available platforms
        إذا (platformName == شته_Platform_Name)
        {
            platform_شته = ریښتیا;
            اكسر;
        }
    }

    إذا (!platform_شته)
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

    خپلکاری clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

بنية کتار::Impl
{
    مضمن فراغ __init()
    {
        refcount = 1;
        handle = 0;
        is_Profiling_کتار = خطأ;
    }

    Impl(cl_command_کتار q)
    {
        __init();
        handle = q;

        cl_command_کتار_properties props = 0;
        CV_OCL_CHECK(cl_Get_Command_کتار_Info(handle, CL_کتار_PROPERTIES, حجم(cl_command_کتار_properties), &props, عدم));
        is_Profiling_کتار = !!(props & CL_کتار_PROFILING_ENABLE);
    }

    Impl(cl_command_کتار q, منطقي is_Profiling_کتار_2)
    {
        __init();
        handle = q;
        is_Profiling_کتار = is_Profiling_کتار_2;
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
        cl_command_کتار_properties props = withProfiling ? CL_کتار_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = cl_Create_Command_کتار(ch, dh, props, &retval), retval);
        is_Profiling_کتار = withProfiling;
    }

    ~Impl()
    {
#که_تعریف_شوی _WIN32
        إذا (!cv::__termination)
#د_که_پای
        {
            إذا(handle)
            {
                CV_OCL_DBG_CHECK(cl_پای(handle));
                CV_OCL_DBG_CHECK(cl_Release_Command_کتار(handle));
                handle = عدم;
            }
        }
    }

    ثابت cv::ocl::کتار& get_Profiling_کتار(ثابت cv::ocl::کتار& self)
    {
        إذا (is_Profiling_کتار)
            أرجع self;

        إذا (profiling_کتار.ptr())
            أرجع profiling_کتار;

        cl_context ctx = 0;
        CV_OCL_CHECK(cl_Get_Command_کتار_Info(handle, CL_کتار_CONTEXT, حجم(cl_context), &ctx, عدم));

        cl_device_id device = 0;
        CV_OCL_CHECK(cl_Get_Command_کتار_Info(handle, CL_کتار_DEVICE, حجم(cl_device_id), &device, عدم));

        cl_int پایله = CL_SUCCESS;
        cl_command_کتار_properties props = CL_کتار_PROFILING_ENABLE;
        cl_command_کتار q = cl_Create_Command_کتار(ctx, device, props, &پایله);
        CV_OCL_DBG_CHECK_پایله(پایله, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        کتار کتار_2;
        کتار_2.p = جديد Impl(q, ریښتیا);
        profiling_کتار = کتار_2;

        أرجع profiling_کتار;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_کتار handle;
    منطقي is_Profiling_کتار;
    cv::ocl::کتار profiling_کتار;
};

کتار::کتار() CV_NOEXCEPT
{
    p = 0;
}

کتار::کتار(ثابت Context& c, ثابت Device& d)
{
    p = 0;
    create(c, d);
}

کتار::کتار(ثابت کتار& q)
{
    p = q.p;
    إذا(p)
        p->addref();
}

کتار& کتار::چلوونکی = (ثابت کتار& q)
{
    Impl* newp = (Impl*)q.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

کتار::کتار(کتار&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = تش_اشاره;
}

کتار& کتار::چلوونکی = (کتار&& q) CV_NOEXCEPT
{
    إذا (هذا != &q) {
        إذا(p)
            p->release();
        p = q.p;
        q.p = تش_اشاره;
    }
    أرجع *هذا;
}

کتار::~کتار()
{
    إذا(p)
        p->release();
}

منطقي کتار::create(ثابت Context& c, ثابت Device& d)
{
    إذا(p)
        p->release();
    p = جديد Impl(c, d);
    أرجع p->handle != 0;
}

فراغ کتار::پای_2()
{
    إذا(p && p->handle)
    {
        CV_OCL_DBG_CHECK(cl_پای(p->handle));
    }
}

ثابت کتار& کتار::get_Profiling_کتار() ثابت
{
    CV_Assert(p);
    أرجع p->get_Profiling_کتار(*هذا);
}

فراغ* کتار::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

کتار& کتار::getDefault()
{
    خپلکاری& c = OpenCLExecutionContext::getCurrent();
    إذا (!c.empty())
    {
        خپلکاری& q = c.get_کتار();
        أرجع q;
    }
    ساكن کتار dummy;
    أرجع dummy;
}

ساكن cl_command_کتار get_کتار(ثابت کتار& q)
{
    cl_command_کتار qq = (cl_command_کتار)q.ptr();
    إذا(!qq)
        qq = (cl_command_کتار)کتار::getDefault().ptr();
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
    أرجع KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.ټول()*m.elem_اندازه());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

بنية Kernel::Impl
{
    Impl(ثابت حرف* kname, ثابت پروګرام_2& prog) :
        refcount(1), handle(عدم), is_دننه_پرمختګ(خطأ), is_Async_پرلپسې(خطأ), nu(0)
    {
        cl_پروګرام ph = (cl_پروګرام)prog.ptr();
        cl_int retval = 0;
        name = kname;
        إذا (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_پایله(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        لكل( صحيح i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_لنډمهاله_Dst_UMats = خطأ;
        have_لنډمهاله_Src_UMats = خطأ;
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
                        exceptionOccurred = ریښتیا;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_لنډمهاله_Dst_UMats = خطأ;
        have_لنډمهاله_Src_UMats = خطأ;
        CV_Assert(!exceptionOccurred);
    }

    فراغ جمع_کړه_UMat(ثابت UMat& m, منطقي dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        إذا(dst && m.u->لنډمهاله_UMat())
            have_لنډمهاله_Dst_UMats = ریښتیا;
        إذا(m.u->original_UMat_معلومات == عدم && m.u->لنډمهاله_UMat())
            have_لنډمهاله_Src_UMats = ریښتیا;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    فراغ ثبتوونکی_Image_دلیل(صحيح arg, ثابت Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        إذا (arg < (صحيح)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !is_دننه_پرمختګ, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (size_t)arg + 1));
        shadow_images[arg] = image;
    }

    فراغ finit(cl_event e)
    {
        CV_ناکارول_شوی(e);
        is_دننه_پرمختګ = خطأ;
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

    منطقي پرلپسې(صحيح dims, size_t _globalsize[], size_t _localsize[],
            منطقي متزامن, int64* وخت_NS, ثابت کتار& q);

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
    منطقي is_دننه_پرمختګ;
    منطقي is_Async_پرلپسې;  // true if kernel was scheduled in async mode
    صحيح nu;
    std::vector<Image2D> shadow_images;
    منطقي have_لنډمهاله_Dst_UMats;
    منطقي have_لنډمهاله_Src_UMats;
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

Kernel::Kernel(ثابت حرف* kname, ثابت پروګرام_2& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(ثابت حرف* kname, ثابت پروګرام_سرچینه& src,
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

Kernel& Kernel::چلوونکی = (ثابت Kernel& k)
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
    k.p = تش_اشاره;
}

Kernel& Kernel::چلوونکی = (Kernel&& k) CV_NOEXCEPT
{
    إذا (هذا != &k) {
        إذا(p)
            p->release();
        p = k.p;
        k.p = تش_اشاره;
    }
    أرجع *هذا;
}

Kernel::~Kernel()
{
    إذا(p)
        p->release();
}

منطقي Kernel::create(ثابت حرف* kname, ثابت پروګرام_2& prog)
{
    إذا(p)
        p->release();
    p = جديد Impl(kname, prog);
    إذا(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#که_تعریف_شوی CV_OPENCL_پرلپسې_ASSERT // check kernel compilation fails
    CV_Assert(p);
#د_که_پای
    أرجع p != 0;
}

منطقي Kernel::create(ثابت حرف* kname, ثابت پروګرام_سرچینه& src,
                    ثابت String& buildopts, String* errmsg)
{
    إذا(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    إذا( !errmsg ) errmsg = &tempmsg;
    ثابت پروګرام_2 prog = Context::getDefault().getProg(src, buildopts, *errmsg);
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

ساكن cv::String dump_ارزښت(size_t sz, ثابت فراغ* p)
{
    إذا (!p)
        أرجع "NULL";
    إذا (sz == 2)
        أرجع cv::format("%d / %uu / 0x%04x", *(لنډ*)p, *(بینښه لنډ*)p, *(لنډ*)p);
    إذا (sz == 4)
        أرجع cv::format("%d / %uu / 0x%08x / %g", *(صحيح*)p, *(صحيح*)p, *(صحيح*)p, *(عائم*)p);
    إذا (sz == 8)
        أرجع cv::format("%lld / %lluu / 0x%16llx / %g", *(اوږد اوږد*)p, *(اوږد اوږد*)p, *(اوږد اوږد*)p, *(مزدوج*)p);
    أرجع cv::format("%p", p);
}

صحيح Kernel::سېټ(صحيح i, ثابت فراغ* value, size_t sz)
{
    إذا (!p || !p->handle)
        أرجع -1;
    إذا (i < 0)
        أرجع i;
    إذا( i == 0 )
        p->cleanupUMats();

    cl_int retval = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_پایله(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (صحيح)i, (صحيح)sz, dump_ارزښت(sz, value).c_str()).c_str());
    إذا (retval != CL_SUCCESS)
        أرجع -1;
    أرجع i+1;
}

صحيح Kernel::سېټ(صحيح i, ثابت Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    صحيح res = سېټ(i, &h, حجم(h));
    إذا (res >= 0)
        p->ثبتوونکی_Image_دلیل(i, image2D);
    أرجع res;
}

صحيح Kernel::سېټ(صحيح i, ثابت UMat& m)
{
    أرجع سېټ(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
ساكن منطقي isCudaBacked(ثابت UMat_معلومات* u)
{
    ثابت MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    أرجع u && cudaAllocator && u->currAllocator == cudaAllocator;
}

صحيح Kernel::سېټ(صحيح i, ثابت KernelArg& arg)
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
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h_null), &h_null);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (صحيح)i).c_str());
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

#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
        {
            ثابت Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_معلومات_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_معلومات_Ptr);
#إذا 1 // TODO
            حالت = svmFns->fn_cl_سېټ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, svm_معلومات_Ptr);
#وإلا
            حالت = svmFns->fn_cl_سېټ_Kernel_Arg_SVMPointer(p->handle, (cl_uint)i, &svm_معلومات_Ptr);
#د_که_پای
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (صحيح)i, (فراغ*)svm_معلومات_Ptr).c_str());
        }
        وإلا
#د_که_پای
        {
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, حجم(h), &h);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (صحيح)i, (فراغ*)h).c_str());
        }

        إذا (ptronly)
        {
            i++;
        }
        وإلا إذا( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u2d.ګام), &u2d.ګام);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u2d.ګام).c_str());
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u2d.offset).c_str());
            i += 3;

            إذا( !(arg.flags & KernelArg::NO_اندازه) )
            {
                صحيح ستون_شمېر = u2d.ستون_شمېر*arg.wscale/arg.iwscale;
                حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u2d.لیکې), &u2d.لیکې);
                CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u2d.لیکې).c_str());
                حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(ستون_شمېر), &ستون_شمېر);
                CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)ستون_شمېر).c_str());
                i += 2;
            }
        }
        وإلا
        {
            UMat3D u3d(*arg.m);
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.slicestep).c_str());
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.ګام), &u3d.ګام);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)u3d.ګام).c_str());
            حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+3), حجم(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (صحيح)(i+3), (صحيح)u3d.offset).c_str());
            i += 4;
            إذا( !(arg.flags & KernelArg::NO_اندازه) )
            {
                صحيح ستون_شمېر = u3d.ستون_شمېر*arg.wscale/arg.iwscale;
                حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, حجم(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (صحيح)i, (صحيح)u3d.slices).c_str());
                حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+1), حجم(u3d.لیکې), &u3d.لیکې);
                CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (صحيح)(i+1), (صحيح)u3d.لیکې).c_str());
                حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)(i+2), حجم(u3d.ستون_شمېر), &ستون_شمېر);
                CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (صحيح)(i+2), (صحيح)ستون_شمېر).c_str());
                i += 3;
            }
        }
        p->جمع_کړه_UMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        أرجع i;
    }
    حالت = cl_سېټ_Kernel_Arg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_پایله(حالت, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (صحيح)i, (صحيح)arg.sz, (فراغ*)arg.obj).c_str());
    أرجع i+1;
}

منطقي Kernel::پرلپسې(صحيح dims, size_t _globalsize[], size_t _localsize[],
                 منطقي متزامن, ثابت کتار& q)
{
    إذا (!p)
        أرجع خطأ;

    size_t globalsize[CV_MAX_DIM] = {1,1,1};
    size_t ټول = 1;
    CV_Assert(_globalsize != عدم);
    لكل (صحيح i = 0; i < dims; i++)
    {
        size_t val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(صحيح)(i>0)) : 1;
        CV_Assert( val > 0 );
        ټول *= _globalsize[i];
        إذا (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = div_پورته(_globalsize[i], (بینښه صحيح)val) * val;
    }
    CV_Assert(ټول > 0);

    أرجع p->پرلپسې(dims, globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::پرلپسې_2(صحيح dims, size_t _globalsize[], size_t _localsize[],
                  منطقي متزامن, ثابت کتار& q)
{
    CV_Assert(p);
    أرجع p->پرلپسې(dims, _globalsize, _localsize, متزامن, عدم, q);
}


منطقي Kernel::Impl::پرلپسې(صحيح dims, size_t globalsize[], size_t localsize[],
        منطقي متزامن, int64* وخت_NS, ثابت کتار& q)
{
    CV_INSTRUMENT_REGION_OPENCL_پرلپسې(name.c_str());

    إذا (!handle)
    {
        CV_LOG_ERROR(عدم, "OpenCL kernel has zero handle: " << name);
        أرجع خطأ;
    }

    إذا (is_Async_پرلپسې)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    is_Async_پرلپسې = !متزامن;

    إذا (is_دننه_پرمختګ)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#إذا CV_OPENCL_SYNC_پرلپسې_KERNELS
    متزامن = ریښتیا;
#د_که_پای

    cl_command_کتار qq = get_کتار(q);
    إذا (have_لنډمهاله_Dst_UMats)
        متزامن = ریښتیا;
    إذا (have_لنډمهاله_Src_UMats)
        متزامن = ریښتیا;
    إذا (وخت_NS)
        متزامن = ریښتیا;
    cl_event asyncEvent = 0;
    cl_int retval = cl_کتار_ته_ورزیاتول_NDRange_Kernel(qq, handle, (cl_uint)dims,
                                           عدم, globalsize, localsize, 0, 0,
                                           (متزامن && !وخت_NS) ? 0 : &asyncEvent);
#إذا !CV_OPENCL_SHOW_پرلپسې_KERNELS
    إذا (retval != CL_SUCCESS)
#د_که_پای
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
        CV_OCL_TRACE_CHECK_پایله(retval, پیغام.c_str());
#وإلا
        چاپ_کړه("%s\n", پیغام.c_str());
        fflush(stdout);
#د_که_پای
    }
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پای(qq));
        إذا (وخت_NS)
        {
            إذا (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong پیل_وخت, stop_وخت;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_پیل, حجم(پیل_وخت), &پیل_وخت, عدم));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_پای_ټکی, حجم(stop_وخت), &stop_وخت, عدم));
                *وخت_NS = (int64)(stop_وخت - پیل_وخت);
            }
            وإلا
            {
                *وخت_NS = -1;
            }
        }
        cleanupUMats();
    }
    وإلا
    {
        addref();
        is_دننه_پرمختګ = ریښتیا;
        CV_OCL_CHECK(cl_سېټ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, هذا));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

منطقي Kernel::پرلپسې_Task(منطقي متزامن, ثابت کتار& q)
{
    إذا(!p || !p->handle || p->is_دننه_پرمختګ)
        أرجع خطأ;

    cl_command_کتار qq = get_کتار(q);
    cl_event asyncEvent = 0;
    cl_int retval = cl_کتار_ته_ورزیاتول_Task(qq, p->handle, 0, 0, متزامن ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_پایله(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), متزامن ? "true" : "false").c_str());
    إذا (متزامن || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(cl_پای(qq));
        p->cleanupUMats();
    }
    وإلا
    {
        p->addref();
        p->is_دننه_پرمختګ = ریښتیا;
        CV_OCL_CHECK(cl_سېټ_Event_Callback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    إذا (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    أرجع retval == CL_SUCCESS;
}

int64 Kernel::پرلپسې_Profiling(صحيح dims, size_t globalsize[], size_t localsize[], ثابت کتار& q_)
{
    CV_Assert(p && p->handle && !p->is_دننه_پرمختګ);
    کتار q = q_.ptr() ? q_ : کتار::getDefault();
    CV_Assert(q.ptr());
    q.پای_2(); // call clFinish() on base queue
    کتار profiling_کتار_2 = q.get_Profiling_کتار();
    int64 وخت_Ns = -1;
    منطقي res = p->پرلپسې(dims, globalsize, localsize, ریښتیا, &وخت_Ns, profiling_کتار_2);
    أرجع res ? وخت_Ns : -1;
}

size_t Kernel::work_ډله_اندازه() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_ډله_Info(p->handle, dev, CL_KERNEL_WORK_ډله_اندازه, حجم(val), &val, &retsz);
    CV_OCL_CHECK_پایله(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

size_t Kernel::prefered_Work_ډله_اندازه_مضرب() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_ډله_Info(p->handle, dev, CL_KERNEL_PREFERRED_WORK_ډله_اندازه_مضرب, حجم(val), &val, &retsz);
    CV_OCL_CHECK_پایله(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    أرجع حالت == CL_SUCCESS ? val : 0;
}

منطقي Kernel::compile_Work_ډله_اندازه(size_t wsz[]) ثابت
{
    إذا(!p || !p->handle || !wsz)
        أرجع 0;
    size_t retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_ډله_Info(p->handle, dev, CL_KERNEL_COMPILE_WORK_ډله_اندازه, حجم(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_پایله(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    أرجع حالت == CL_SUCCESS;
}

size_t Kernel::local_Mem_اندازه() ثابت
{
    إذا(!p || !p->handle)
        أرجع 0;
    size_t retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int حالت = cl_Get_Kernel_Work_ډله_Info(p->handle, dev, CL_KERNEL_LOCAL_MEM_اندازه, حجم(val), &val, &retsz);
    CV_OCL_CHECK_پایله(حالت, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    أرجع حالت == CL_SUCCESS ? (size_t)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

بنية پروګرام_سرچینه::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    تعداد ډول_4 {
        پروګرام_سرچینه_کوډ = 0,
        پروګرام_BINARIES,
        پروګرام_SPIR,
        پروګرام_SPIRV
    } ډول_5;

    Impl(ثابت String& src)
    {
        init(پروګرام_سرچینه_کوډ, cv::String(), cv::String());
        init_From_سرچینه(src, cv::String());
    }
    Impl(ثابت String& module, ثابت String& name, ثابت String& کوډ_Str, ثابت String& کوډ_Hash)
    {
        init(پروګرام_سرچینه_کوډ, module, name);
        init_From_سرچینه(کوډ_Str, کوډ_Hash);
    }

    /// reset fields
    فراغ init(تعداد ډول_4 ډول, ثابت String& module, ثابت String& name)
    {
        refcount = 1;
        ډول_5 = ډول;
        module_ = module;
        name_ = name;

        سرچینه_Addr = عدم;
        سرچینه_اندازه = 0;
        isHashUpdated = خطأ;
    }

    فراغ init_From_سرچینه(ثابت String& کوډ_Str, ثابت String& کوډ_Hash)
    {
        کوډ_Str_2 = کوډ_Str;
        سرچینه_Hash = کوډ_Hash;
        إذا (سرچینه_Hash.empty())
        {
            updateHash();
        }
        وإلا
        {
            isHashUpdated = ریښتیا;
        }
    }

    فراغ updateHash(ثابت حرف* hashStr = عدم)
    {
        إذا (hashStr)
        {
            سرچینه_Hash = cv::String(hashStr);
            isHashUpdated = ریښتیا;
            أرجع;
        }
        uint64 hash = 0;
        بدل (ډول_5)
        {
        حالة پروګرام_سرچینه_کوډ:
            إذا (سرچینه_Addr)
            {
                CV_Assert(کوډ_Str_2.empty());
                hash = crc64(سرچینه_Addr, سرچینه_اندازه); // static storage
            }
            وإلا
            {
                CV_Assert(!کوډ_Str_2.empty());
                hash = crc64((uchar*)کوډ_Str_2.c_str(), کوډ_Str_2.size());
            }
            اكسر;
        حالة پروګرام_BINARIES:
        حالة پروګرام_SPIR:
        حالة پروګرام_SPIRV:
            hash = crc64(سرچینه_Addr, سرچینه_اندازه);
            اكسر;
        افتراضي:
            CV_Error(Error::Sts_دننی, "Internal error");
        }
        سرچینه_Hash = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = ریښتیا;
    }

    Impl(تعداد ډول_4 ډول,
            ثابت String& module, ثابت String& name,
            ثابت بینښه حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions = cv::String())
    {
        init(ډول, module, name);

        سرچینه_Addr = binary;
        سرچینه_اندازه = size;

        buildOptions_ = buildOptions;
    }

    ساكن پروګرام_سرچینه from_سرچینه_With_Static_Lifetime(ثابت String& module, ثابت String& name,
            ثابت حرف* سرچینه_کوډ_Static_Str, ثابت حرف* hashStaticStr,
            ثابت cv::String& buildOptions)
    {
        پروګرام_سرچینه پایله;
        پایله.p = جديد Impl(پروګرام_سرچینه_کوډ, module, name,
                (ثابت بینښه حرف*)سرچینه_کوډ_Static_Str, مزي_اوږدوالی(سرچینه_کوډ_Static_Str), buildOptions);
        پایله.p->updateHash(hashStaticStr);
        أرجع پایله;
    }

    ساكن پروګرام_سرچینه from_دوګونی(ثابت String& module, ثابت String& name,
            ثابت بینښه حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروګرام_سرچینه پایله;
        پایله.p = جديد Impl(پروګرام_BINARIES, module, name, binary, size, buildOptions);
        أرجع پایله;
    }

    ساكن پروګرام_سرچینه fromSPIR(ثابت String& module, ثابت String& name,
            ثابت بینښه حرف* binary, ثابت size_t size,
            ثابت cv::String& buildOptions)
    {
        پروګرام_سرچینه پایله;
        پایله.p = جديد Impl(پروګرام_SPIR, module, name, binary, size, buildOptions);
        أرجع پایله;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String کوډ_Str_2; // PROGRAM_SOURCE_CODE only

    ثابت بینښه حرف* سرچینه_Addr;
    size_t سرچینه_اندازه;

    cv::String buildOptions_;

    String سرچینه_Hash;
    منطقي isHashUpdated;

    ملګری بنية پروګرام_2::Impl;
    ملګری بنية internal::پروګرام_ثبت;
    ملګری بنية Context::Impl;
};


پروګرام_سرچینه::پروګرام_سرچینه() CV_NOEXCEPT
{
    p = 0;
}

پروګرام_سرچینه::پروګرام_سرچینه(ثابت String& module, ثابت String& name, ثابت String& کوډ_Str, ثابت String& کوډ_Hash)
{
    p = جديد Impl(module, name, کوډ_Str, کوډ_Hash);
}

پروګرام_سرچینه::پروګرام_سرچینه(ثابت حرف* prog)
{
    p = جديد Impl(prog);
}

پروګرام_سرچینه::پروګرام_سرچینه(ثابت String& prog)
{
    p = جديد Impl(prog);
}

پروګرام_سرچینه::~پروګرام_سرچینه()
{
    إذا(p)
        p->release();
}

پروګرام_سرچینه::پروګرام_سرچینه(ثابت پروګرام_سرچینه& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

پروګرام_سرچینه& پروګرام_سرچینه::چلوونکی = (ثابت پروګرام_سرچینه& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

پروګرام_سرچینه::پروګرام_سرچینه(پروګرام_سرچینه&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = تش_اشاره;
}

پروګرام_سرچینه& پروګرام_سرچینه::چلوونکی = (پروګرام_سرچینه&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = تش_اشاره;
    }
    أرجع *هذا;
}

ثابت String& پروګرام_سرچینه::سرچینه() ثابت
{
    CV_Assert(p);
    CV_Assert(p->ډول_5 == Impl::پروګرام_سرچینه_کوډ);
    CV_Assert(p->سرچینه_Addr == عدم); // method returns reference - can't construct temporary object
    أرجع p->کوډ_Str_2;
}

پروګرام_سرچینه::hash_t پروګرام_سرچینه::hash() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

پروګرام_سرچینه پروګرام_سرچینه::from_دوګونی(ثابت String& module, ثابت String& name,
        ثابت بینښه حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دوګونی(module, name, binary, size, buildOptions);
}

پروګرام_سرچینه پروګرام_سرچینه::fromSPIR(ثابت String& module, ثابت String& name,
        ثابت بینښه حرف* binary, ثابت size_t size,
        ثابت cv::String& buildOptions)
{
    CV_Assert(binary);
    CV_Assert(size > 0);
    أرجع Impl::from_دوګونی(module, name, binary, size, buildOptions);
}


internal::پروګرام_ثبت::چلوونکی پروګرام_سرچینه&() ثابت
{
    إذا (هذا->p_پروګرام_سرچینه == عدم)
    {
        cv::Auto_قلف lock(cv::getInitializationMutex());
        إذا (هذا->p_پروګرام_سرچینه == عدم)
        {
            پروګرام_سرچینه ps = پروګرام_سرچینه::Impl::from_سرچینه_With_Static_Lifetime(هذا->module, هذا->name, هذا->پروګرام_کوډ, هذا->پروګرام_Hash, cv::String());
            پروګرام_سرچینه* ptr = جديد پروګرام_سرچینه(ps);
            const_cast<پروګرام_ثبت*>(هذا)->p_پروګرام_سرچینه = ptr;
        }
    }
    أرجع *هذا->p_پروګرام_سرچینه;
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

بنية پروګرام_2::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(ثابت پروګرام_سرچینه& src,
         ثابت String& _buildflags, String& errmsg) :
         refcount(1),
         handle(عدم),
         buildflags(_buildflags)
    {
        ثابت پروګرام_سرچینه::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        سرچینه_Module = src_->module_;
        سرچینه_Name = src_->name_;
        ثابت Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        إذا (ctx.ptr() == عدم || device.ptr() == عدم)
            أرجع;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        إذا (src.getImpl()->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_سرچینه_کوډ)
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
        CV_LOG_INFO(عدم, "OpenCL program '" << سرچینه_Module << "/" << سرچینه_Name << "' options:" << buildflags);
#د_که_پای
        compile(ctx, src_, errmsg);
#إذا CV_OPENCL_SHOW_BUILD_KERNELS
        إذا (handle)
        {
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            cl_int پایله = cl_Get_پروګرام_Info(handle, CL_پروګرام_KERNEL_نومونه, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            CV_OCL_DBG_CHECK_پایله(پایله, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", سرچینه_Module.c_str(), سرچینه_Name.c_str()).c_str());
            إذا (پایله == CL_SUCCESS && retsz < حجم(kernels_بفر))
            {
                kernels_بفر[retsz] = 0;
                CV_LOG_INFO(عدم, "OpenCL program '" << سرچینه_Module << "/" << سرچینه_Name << "' kernels: '" << kernels_بفر << "'");
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, "OpenCL program '" << سرچینه_Module << "/" << سرچینه_Name << "' can't retrieve kernel names!");
            }
        }
#د_که_پای
    }

    منطقي compile(ثابت Context& ctx, ثابت پروګرام_سرچینه::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        إذا (src_->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_BINARIES)
        {
            CV_LOG_VERBOSE(عدم, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دوګونی(ctx, src_->سرچینه_Addr, src_->سرچینه_اندازه, errmsg);
            أرجع isLoaded;
        }
        أرجع compileWithCache(ctx, src_, errmsg);
    }

    منطقي compileWithCache(ثابت Context& ctx, ثابت پروګرام_سرچینه::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->ډول_5 != پروګرام_سرچینه::Impl::پروګرام_BINARIES);

#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        ثابت std::string بنسټ_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->get_مختاړی_String(),
                ctx.getImpl()->get_مختاړی_بنسټ()
        );
        ثابت String& hash_str = src_->سرچینه_Hash;
        cv::String fname;
        إذا (!بنسټ_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(بنسټ_dir, fname);
        }
        ثابت cv::Ptr<utils::fs::File_قلف> file_قلف = config.cache_قلف; // can be empty
        إذا (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            حاول
            {
                std::vector<حرف> دوګونی_Buf;
                منطقي res = خطأ;
                {
                    cv::utils::optional_shared_قلف_guard<cv::utils::fs::File_قلف> قلف_fs(file_قلف.get());
                    دوګونی_پروګرام_File file(fname, hash_str.c_str());
                    res = file.اقرأ(buildflags, دوګونی_Buf);
                }
                إذا (res)
                {
                    CV_Assert(!دوګونی_Buf.empty());
                    CV_LOG_VERBOSE(عدم, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    منطقي isLoaded = create_From_دوګونی(ctx, دوګونی_Buf, errmsg);
                    إذا (isLoaded)
                        أرجع ریښتیا;
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_ناکارول_شوی(e);
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname << std::کرښه_پای << e.what());
            }
            امسك (...)
            {
                CV_LOG_VERBOSE(عدم, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == عدم);
        إذا (src_->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_سرچینه_کوډ)
        {
            إذا (!buildFromSources(ctx, src_, errmsg))
            {
                أرجع خطأ;
            }
        }
        وإلا إذا (src_->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            إذا ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(عدم, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            منطقي isLoaded = create_From_دوګونی(ctx, src_->سرچینه_Addr, src_->سرچینه_اندازه, errmsg);
            إذا (!isLoaded)
                أرجع خطأ;
        }
        وإلا إذا (src_->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        وإلا
        {
            CV_Error(Error::Sts_دننی, "Internal error");
        }
        CV_Assert(handle != عدم);
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
        إذا (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            حاول
            {
                std::vector<حرف> دوګونی_Buf;
                get_پروګرام_دوګونی(دوګونی_Buf);
                {
                    cv::utils::optional_قلف_guard<cv::utils::fs::File_قلف> قلف_fs(file_قلف.get());
                    دوګونی_پروګرام_File file(fname, hash_str.c_str());
                    file.اكتب(buildflags, دوګونی_Buf);
                }
            }
            امسك (ثابت cv::Exception& e)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname << std::کرښه_پای << e.what());
            }
            امسك (...)
            {
                CV_LOG_WARNING(عدم, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#د_که_پای // OPENCV_HAVE_FILESYSTEM_SUPPORT
#إذا CV_OPENCL_VALIDATE_دوګونی_PROGRAMS
        إذا (CV_OPENCL_VALIDATE_دوګونی_PROGRAMS_ارزښت)
        {
            std::vector<حرف> دوګونی_Buf;
            get_پروګرام_دوګونی(دوګونی_Buf);
            إذا (!دوګونی_Buf.empty())
            {
                CV_OCL_DBG_CHECK(cl_Release_پروګرام(handle));
                handle = عدم;
                create_From_دوګونی(ctx, دوګونی_Buf, errmsg);
            }
        }
#د_که_پای
        أرجع handle != عدم;
    }

    فراغ dumpBuildLog_(cl_int پایله, ثابت cl_device_id* deviceList, String& errmsg)
    {
        Auto_بفر<حرف, 4096> بفر; بفر[0] = 0;

        size_t retsz = 0;
        cl_int log_retval = cl_Get_پروګرام_Build_Info(handle, deviceList[0],
                                                  CL_پروګرام_BUILD_LOG, 0, 0, &retsz);
        إذا (log_retval == CL_SUCCESS && retsz > 1)
        {
            بفر.resize(retsz + 16);
            log_retval = cl_Get_پروګرام_Build_Info(handle, deviceList[0],
                                               CL_پروګرام_BUILD_LOG, retsz+1, بفر.معلومات(), &retsz);
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
        چاپ_کړه("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                سرچینه_Module.c_str(), سرچینه_Name.c_str(),
                پایله, getOpenCLErrorString(پایله),
                buildflags.c_str(), errmsg.c_str());
        fflush(stdout);
    }

    منطقي buildFromSources(ثابت Context& ctx, ثابت پروګرام_سرچینه::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->ډول_5 == پروګرام_سرچینه::Impl::پروګرام_سرچینه_کوډ);
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                سرچینه_Module.c_str(), سرچینه_Name.c_str(),
                src_->سرچینه_Hash.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(عدم, 0, "Compile... " << سرچینه_Module.c_str() << "/" << سرچینه_Name.c_str());

        ثابت حرف* srcptr = src_->سرچینه_Addr ? ((ثابت حرف*)src_->سرچینه_Addr) : src_->کوډ_Str_2.c_str();
        size_t srclen = src_->سرچینه_Addr ? src_->سرچینه_اندازه : src_->کوډ_Str_2.size();
        CV_Assert(srcptr != عدم);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = cl_Create_پروګرام_With_سرچینه((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_پایله(retval, "clCreateProgramWithSource");
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

            retval = cl_Build_پروګرام(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_پایله(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#إذا !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            إذا (retval != CL_SUCCESS)
#د_که_پای
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                إذا (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروګرام(handle));
                    handle = عدم;
                }
                إذا (retval != CL_SUCCESS &&
                    سرچینه_Name != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#إذا CV_OPENCL_VALIDATE_دوګونی_PROGRAMS
            إذا (handle && CV_OPENCL_VALIDATE_دوګونی_PROGRAMS_ارزښت)
            {
                CV_LOG_INFO(عدم, "OpenCL: query kernel names (build from sources)...");
                size_t retsz = 0;
                حرف kernels_بفر[4096] = {0};
                cl_int پایله = cl_Get_پروګرام_Info(handle, CL_پروګرام_KERNEL_نومونه, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
                إذا (retsz < حجم(kernels_بفر))
                    kernels_بفر[retsz] = 0;
                وإلا
                    kernels_بفر[0] = 0;
                CV_LOG_INFO(عدم, پایله << ": Kernels='" << kernels_بفر << "'");
            }
#د_که_پای
        }
        أرجع handle != عدم;
    }

    فراغ get_پروګرام_دوګونی(std::vector<حرف>& buf)
    {
        CV_Assert(handle);
        size_t sz = 0;
        CV_OCL_CHECK(cl_Get_پروګرام_Info(handle, CL_پروګرام_دوګونی_SIZES, حجم(sz), &sz, عدم));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(cl_Get_پروګرام_Info(handle, CL_پروګرام_BINARIES, حجم(ptr), &ptr, عدم));
    }

    منطقي create_From_دوګونی(ثابت Context& ctx, ثابت std::vector<حرف>& buf, String& errmsg)
    {
        أرجع create_From_دوګونی(ctx, (ثابت بینښه حرف*)&buf[0], buf.size(), errmsg);
    }

    منطقي create_From_دوګونی(ثابت Context& ctx, ثابت بینښه حرف* دوګونی_Addr, ثابت size_t دوګونی_اندازه, String& errmsg)
    {
        CV_Assert(handle == عدم);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(عدم, 0, "Load from binary... (" << دوګونی_اندازه << " bytes)");

        CV_Assert(دوګونی_اندازه > 0);

        size_t ndevices = (صحيح)ctx.ndevices();
        Auto_بفر<cl_device_id> devices_(ndevices);
        Auto_بفر<ثابت uchar*> دوګونی_Ptrs(ndevices);
        Auto_بفر<size_t> دوګونی_Sizes(ndevices);

        cl_device_id* devices = devices_.معلومات();
        ثابت uchar** دوګونی_Ptrs_2 = دوګونی_Ptrs.معلومات();
        size_t* دوګونی_Sizes_2 = دوګونی_Sizes.معلومات();
        لكل (size_t i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            دوګونی_Ptrs_2[i] = دوګونی_Addr;
            دوګونی_Sizes_2[i] = دوګونی_اندازه;
        }

        cl_int پایله = 0;
        handle = cl_Create_پروګرام_With_دوګونی((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.معلومات(),
                                           دوګونی_Sizes_2, دوګونی_Ptrs_2, عدم, &پایله);
        إذا (پایله != CL_SUCCESS)
        {
            CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیغام(پایله, "clCreateProgramWithBinary"));
            إذا (handle)
            {
                CV_OCL_DBG_CHECK(cl_Release_پروګرام(handle));
                handle = عدم;
            }
        }
        إذا (!handle)
        {
            أرجع خطأ;
        }
        // call clBuildProgram()
        {
            پایله = cl_Build_پروګرام(handle, (cl_uint)ndevices, devices_.معلومات(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_پایله(پایله, cv::format("clBuildProgram(binary: %s/%s)", سرچینه_Module.c_str(), سرچینه_Name.c_str()).c_str());
            إذا (پایله != CL_SUCCESS)
            {
                dumpBuildLog_(پایله, devices, errmsg);
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروګرام(handle));
                    handle = عدم;
                }
                أرجع خطأ;
            }
        }
        // check build status
        {
            cl_build_حالت build_حالت = CL_BUILD_NONE;
            size_t retsz = 0;
            CV_OCL_DBG_CHECK(پایله = cl_Get_پروګرام_Build_Info(handle, devices[0], CL_پروګرام_BUILD_حالت,
                    حجم(build_حالت), &build_حالت, &retsz));
            إذا (پایله == CL_SUCCESS)
            {
                إذا (build_حالت == CL_BUILD_SUCCESS)
                {
                    أرجع ریښتیا;
                }
                وإلا
                {
                    CV_LOG_WARNING(عدم, "clGetProgramBuildInfo() returns " << build_حالت);
                    أرجع خطأ;
                }
            }
            وإلا
            {
                CV_LOG_ERROR(عدم, CV_OCL_API_ERROR_پیغام(پایله, "clGetProgramBuildInfo()"));
                إذا (handle)
                {
                    CV_OCL_DBG_CHECK(cl_Release_پروګرام(handle));
                    handle = عدم;
                }
            }
        }
#إذا CV_OPENCL_VALIDATE_دوګونی_PROGRAMS
        إذا (handle && CV_OPENCL_VALIDATE_دوګونی_PROGRAMS_ارزښت)
        {
            CV_LOG_INFO(عدم, "OpenCL: query kernel names (binary)...");
            size_t retsz = 0;
            حرف kernels_بفر[4096] = {0};
            پایله = cl_Get_پروګرام_Info(handle, CL_پروګرام_KERNEL_نومونه, حجم(kernels_بفر), &kernels_بفر[0], &retsz);
            إذا (retsz < حجم(kernels_بفر))
                kernels_بفر[retsz] = 0;
            وإلا
                kernels_بفر[0] = 0;
            CV_LOG_INFO(عدم, پایله << ": Kernels='" << kernels_بفر << "'");
        }
#د_که_پای
        أرجع handle != عدم;
    }

    ~Impl()
    {
        إذا( handle )
        {
#که_تعریف_شوی _WIN32
            إذا (!cv::__termination)
#د_که_پای
            {
                cl_Release_پروګرام(handle);
            }
            handle = عدم;
        }
    }

    cl_پروګرام handle;

    String buildflags;
    String سرچینه_Module;
    String سرچینه_Name;
};


پروګرام_2::پروګرام_2() CV_NOEXCEPT
{
    p = 0;
}

پروګرام_2::پروګرام_2(ثابت پروګرام_سرچینه& src,
        ثابت String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

پروګرام_2::پروګرام_2(ثابت پروګرام_2& prog)
{
    p = prog.p;
    إذا(p)
        p->addref();
}

پروګرام_2& پروګرام_2::چلوونکی = (ثابت پروګرام_2& prog)
{
    Impl* newp = (Impl*)prog.p;
    إذا(newp)
        newp->addref();
    إذا(p)
        p->release();
    p = newp;
    أرجع *هذا;
}

پروګرام_2::پروګرام_2(پروګرام_2&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = تش_اشاره;
}

پروګرام_2& پروګرام_2::چلوونکی = (پروګرام_2&& prog) CV_NOEXCEPT
{
    إذا (هذا != &prog) {
        إذا(p)
            p->release();
        p = prog.p;
        prog.p = تش_اشاره;
    }
    أرجع *هذا;
}

پروګرام_2::~پروګرام_2()
{
    إذا(p)
        p->release();
}

منطقي پروګرام_2::create(ثابت پروګرام_سرچینه& src,
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

فراغ* پروګرام_2::ptr() ثابت
{
    أرجع p ? p->handle : 0;
}

#که_نه_تعریف_شوی OPENCV_REMOVE_DEPRECATED_API
ثابت پروګرام_سرچینه& پروګرام_2::سرچینه() ثابت
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروګرام_2::اقرأ(ثابت String& bin, ثابت String& buildflags)
{
    CV_ناکارول_شوی(bin); CV_ناکارول_شوی(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

منطقي پروګرام_2::اكتب(String& bin) ثابت
{
    CV_ناکارول_شوی(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String پروګرام_2::get_مختاړی() ثابت
{
    إذا(!p)
        أرجع String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_مختاړی_String().c_str(), p->buildflags.c_str());
}

String پروګرام_2::get_مختاړی(ثابت String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        أرجع cv::format("opencl=%s\nbuildflags=%s", ctx_->get_مختاړی_String().c_str(), buildflags.c_str());
}
#د_که_پای // OPENCV_REMOVE_DEPRECATED_API

فراغ پروګرام_2::get_دوګونی(std::vector<حرف>& binary) ثابت
{
    CV_Assert(p && "Empty program");
    p->get_پروګرام_دوګونی(binary);
}

پروګرام_2 Context::Impl::getProg(ثابت پروګرام_سرچینه& src,
                               ثابت String& buildflags, String& errmsg)
{
    size_t حد = get_پروګرام_شمېر_حد();
    ثابت پروګرام_سرچینه::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String کلی = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->سرچینه_Hash.c_str(),
            get_مختاړی_String().c_str(),
            buildflags.c_str());
    {
        cv::Auto_قلف lock(پروګرام_cache_mutex);
        phash_t::iterator it = phash.find(کلی);
        إذا (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), کلی);
            إذا (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(کلی);
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
                    چاپ_کړه("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = ریښتیا;
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
    پروګرام_2 prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::Auto_قلف lock(پروګرام_cache_mutex);
        phash.insert(std::pair<std::string, پروګرام_2>(کلی, prog));
        cacheList.push_front(کلی);
    }
    أرجع prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

کينډۍ<ډول_نوم T>
صنف OpenCLBufferPool
{
محمي:
    ~OpenCLBufferPool() { }
عام:
    مجازي T allocate(size_t size) = 0;
    مجازي فراغ release(T بفر) = 0;
};

کينډۍ <ډول_نوم Derived, ډول_نوم بفر_ثبت, ډول_نوم T>
صنف Open_CLBuffer_Pool_بنسټ_Impl : عام بفر_Pool_Controller, عام OpenCLBufferPool<T>
{
خاص:
    مضمن Derived& derived() { أرجع *static_cast<Derived*>(هذا); }
محمي:
    Mutex mutex_;

    size_t current_Reserved_اندازه;
    size_t max_Reserved_اندازه;

    std::list<بفر_ثبت> allocatedEntries_; // Allocated and used entries
    std::list<بفر_ثبت> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    منطقي پیدا_کړه_And_Remove_ثبت_From_Allocated_List(CV_بهر بفر_ثبت& ثبت, T بفر)
    {
        ډول_نوم std::list<بفر_ثبت>::iterator i = allocatedEntries_.begin();
        لكل (; i != allocatedEntries_.end(); ++i)
        {
            بفر_ثبت& e = *i;
            إذا (e.cl_بفر_2 == بفر)
            {
                ثبت = e;
                allocatedEntries_.erase(i);
                أرجع ریښتیا;
            }
        }
        أرجع خطأ;
    }

    // synchronized
    منطقي پیدا_کړه_And_Remove_ثبت_From_Reserved_List(CV_بهر بفر_ثبت& ثبت, ثابت size_t size)
    {
        إذا (reservedEntries_.empty())
            أرجع خطأ;
        ډول_نوم std::list<بفر_ثبت>::iterator i = reservedEntries_.begin();
        ډول_نوم std::list<بفر_ثبت>::iterator پایله_pos = reservedEntries_.end();
        بفر_ثبت پایله;
        size_t minDiff = (size_t)(-1);
        لكل (; i != reservedEntries_.end(); ++i)
        {
            بفر_ثبت& e = *i;
            إذا (e.ظرفیت >= size)
            {
                size_t diff = e.ظرفیت - size;
                إذا (diff < std::max((size_t)4096, size / 8) && (پایله_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    پایله_pos = i;
                    پایله = e;
                    إذا (diff == 0)
                        اكسر;
                }
            }
        }
        إذا (پایله_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(پایله_pos);
            ثبت = پایله;
            current_Reserved_اندازه -= ثبت.ظرفیت;
            allocatedEntries_.push_back(ثبت);
            أرجع ریښتیا;
        }
        أرجع خطأ;
    }

    // synchronized
    فراغ check_اندازه_د_Reserved_Entries()
    {
        طالما (current_Reserved_اندازه > max_Reserved_اندازه)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            ثابت بفر_ثبت& ثبت = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_اندازه >= ثبت.ظرفیت);
            current_Reserved_اندازه -= ثبت.ظرفیت;
            derived().release_بفر_ثبت(ثبت);
            reservedEntries_.pop_back();
        }
    }

    مضمن size_t ویش_Granularity(size_t size)
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
    Open_CLBuffer_Pool_بنسټ_Impl()
        : current_Reserved_اندازه(0),
          max_Reserved_اندازه(0)
    {
        // nothing
    }
    مجازي ~Open_CLBuffer_Pool_بنسټ_Impl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
عام:
    مجازي T allocate(size_t size) CV_OVERRIDE
    {
        Auto_قلف locker(mutex_);
        بفر_ثبت ثبت;
        إذا (max_Reserved_اندازه > 0 && پیدا_کړه_And_Remove_ثبت_From_Reserved_List(ثبت, size))
        {
            CV_DbgAssert(size <= ثبت.ظرفیت);
            LOG_بفر_POOL("Reuse reserved buffer: %p\n", ثبت.cl_بفر_2);
        }
        وإلا
        {
            derived().allocate_بفر_ثبت(ثبت, size);
        }
        أرجع ثبت.cl_بفر_2;
    }
    مجازي فراغ release(T بفر) CV_OVERRIDE
    {
        Auto_قلف locker(mutex_);
        بفر_ثبت ثبت;
        CV_Assert(پیدا_کړه_And_Remove_ثبت_From_Allocated_List(ثبت, بفر));
        إذا (max_Reserved_اندازه == 0 || ثبت.ظرفیت > max_Reserved_اندازه / 8)
        {
            derived().release_بفر_ثبت(ثبت);
        }
        وإلا
        {
            reservedEntries_.push_front(ثبت);
            current_Reserved_اندازه += ثبت.ظرفیت;
            check_اندازه_د_Reserved_Entries();
        }
    }

    مجازي size_t get_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع current_Reserved_اندازه; }
    مجازي size_t get_Max_Reserved_اندازه() ثابت CV_OVERRIDE { أرجع max_Reserved_اندازه; }
    مجازي فراغ سېټ_Max_Reserved_اندازه(size_t size) CV_OVERRIDE
    {
        Auto_قلف locker(mutex_);
        size_t old_Max_Reserved_اندازه = max_Reserved_اندازه;
        max_Reserved_اندازه = size;
        إذا (max_Reserved_اندازه < old_Max_Reserved_اندازه)
        {
            ډول_نوم std::list<بفر_ثبت>::iterator i = reservedEntries_.begin();
            لكل (; i != reservedEntries_.end();)
            {
                ثابت بفر_ثبت& ثبت = *i;
                إذا (ثبت.ظرفیت > max_Reserved_اندازه / 8)
                {
                    CV_DbgAssert(current_Reserved_اندازه >= ثبت.ظرفیت);
                    current_Reserved_اندازه -= ثبت.ظرفیت;
                    derived().release_بفر_ثبت(ثبت);
                    i = reservedEntries_.erase(i);
                    استمر;
                }
                ++i;
            }
            check_اندازه_د_Reserved_Entries();
        }
    }
    مجازي فراغ freeAllReservedBuffers() CV_OVERRIDE
    {
        Auto_قلف locker(mutex_);
        ډول_نوم std::list<بفر_ثبت>::const_iterator i = reservedEntries_.begin();
        لكل (; i != reservedEntries_.end(); ++i)
        {
            ثابت بفر_ثبت& ثبت = *i;
            derived().release_بفر_ثبت(ثبت);
        }
        reservedEntries_.clear();
        current_Reserved_اندازه = 0;
    }
};

بنية CLBuffer_ثبت
{
    cl_mem cl_بفر_2;
    size_t ظرفیت;
    CLBuffer_ثبت() : cl_بفر_2((cl_mem)عدم), ظرفیت(0) { }
};

صنف OpenCLBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_بنسټ_Impl<OpenCLBufferPoolImpl, CLBuffer_ثبت, cl_mem>
{
عام:
    تعريفنوع بنية CLBuffer_ثبت بفر_ثبت;
محمي:
    صحيح createFlags_;
عام:
    OpenCLBufferPoolImpl(صحيح createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    فراغ allocate_بفر_ثبت(بفر_ثبت& ثبت, size_t size)
    {
        CV_DbgAssert(ثبت.cl_بفر_2 == عدم);
        ثبت.ظرفیت = align_اندازه(size, (صحيح)ویش_Granularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        ثبت.cl_بفر_2 = cl_Create_بفر((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, ثبت.ظرفیت, 0, &retval);
        CV_OCL_CHECK_پایله(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (اوږد اوږد صحيح)ثبت.ظرفیت, (فراغ*)ثبت.cl_بفر_2).c_str());
        CV_Assert(ثبت.cl_بفر_2 != عدم);
        إذا(retval == CL_SUCCESS)
        {
            CV_IMPL_جمع_کړه(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (اوږد اوږد)ثبت.ظرفیت, (اوږد اوږد)ثبت.ظرفیت, ثبت.cl_بفر_2);
        allocatedEntries_.push_back(ثبت);
    }

    فراغ release_بفر_ثبت(ثابت بفر_ثبت& ثبت)
    {
        CV_Assert(ثبت.ظرفیت != 0);
        CV_Assert(ثبت.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                ثبت.cl_بفر_2, (اوږد اوږد)ثبت.ظرفیت, (اوږد اوږد)ثبت.ظرفیت);
        CV_OCL_DBG_CHECK(clReleaseMemObject(ثبت.cl_بفر_2));
    }
};

#که_تعریف_شوی HAVE_OPENCL_SVM
بنية CLSVMBuffer_ثبت
{
    فراغ* cl_بفر_2;
    size_t ظرفیت;
    CLSVMBuffer_ثبت() : cl_بفر_2(عدم), ظرفیت(0) { }
};
صنف OpenCLSVMBufferPoolImpl CV_FINAL : عام Open_CLBuffer_Pool_بنسټ_Impl<OpenCLSVMBufferPoolImpl, CLSVMBuffer_ثبت, فراغ*>
{
عام:
    تعريفنوع بنية CLSVMBuffer_ثبت بفر_ثبت;
عام:
    OpenCLSVMBufferPoolImpl()
    {
    }

    فراغ allocate_بفر_ثبت(بفر_ثبت& ثبت, size_t size)
    {
        CV_DbgAssert(ثبت.cl_بفر_2 == عدم);
        ثبت.ظرفیت = align_اندازه(size, (صحيح)ویش_Granularity(size));

        Context& ctx = Context::getDefault();
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_بفر ? CL_MEM_SVM_FINE_GRAIN_بفر : 0);

        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (صحيح)ثبت.ظرفیت);
        فراغ *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, ثبت.ظرفیت, 0);
        CV_Assert(buf);

        ثبت.cl_بفر_2 = buf;
        {
            CV_IMPL_جمع_کړه(CV_IMPL_OCL);
        }
        LOG_بفر_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (اوږد اوږد)ثبت.ظرفیت, (اوږد اوږد)ثبت.ظرفیت, ثبت.cl_بفر_2);
        allocatedEntries_.push_back(ثبت);
    }

    فراغ release_بفر_ثبت(ثابت بفر_ثبت& ثبت)
    {
        CV_Assert(ثبت.ظرفیت != 0);
        CV_Assert(ثبت.cl_بفر_2 != عدم);
        LOG_بفر_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                ثبت.cl_بفر_2, (اوږد اوږد)ثبت.ظرفیت, (اوږد اوږد)ثبت.ظرفیت);
        Context& ctx = Context::getDefault();
        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  ثبت.cl_بفر_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), ثبت.cl_بفر_2);
    }
};
#د_که_پای



کينډۍ <منطقي readAccess, منطقي writeAccess>
صنف Aligned_معلومات_Ptr
{
محمي:
    ثابت size_t اندازه_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

عام:
    Aligned_معلومات_Ptr(uchar* ptr, size_t size, size_t alignment)
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
                حافظه_نقل(ptr_, originPtr_, اندازه_3);
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
                حافظه_نقل(originPtr_, ptr_, اندازه_3);
            }
            ړنګول[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_معلومات_Ptr(ثابت Aligned_معلومات_Ptr&); // disabled
    Aligned_معلومات_Ptr& چلوونکی=(ثابت Aligned_معلومات_Ptr&); // disabled
};

کينډۍ <منطقي readAccess, منطقي writeAccess>
صنف Aligned_معلومات_Ptr2_D
{
محمي:
    ثابت size_t اندازه_3;
    uchar* ثابت originPtr_;
    ثابت size_t alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    size_t لیکې_2;
    size_t ستون_شمېر_2;
    size_t ګام_2;

عام:
    Aligned_معلومات_Ptr2_D(uchar* ptr, size_t لیکې, size_t ستون_شمېر, size_t ګام, size_t alignment, size_t extrabytes=0)
        : اندازه_3(لیکې*ګام), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(عدم), لیکې_2(لیکې), ستون_شمېر_2(ستون_شمېر), ګام_2(ګام)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != عدم);
        إذا (ptr == 0 || ((size_t)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = جديد uchar[اندازه_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            إذا (readAccess)
            {
                لكل (size_t i = 0; i < لیکې_2; i++)
                    حافظه_نقل(ptr_ + i*ګام_2, originPtr_ + i*ګام_2, ستون_شمېر_2);
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
                لكل (size_t i = 0; i < لیکې_2; i++)
                    حافظه_نقل(originPtr_ + i*ګام_2, ptr_ + i*ګام_2, ستون_شمېر_2);
            }
            ړنګول[] allocatedPtr_;
            allocatedPtr_ = عدم;
        }
        ptr_ = عدم;
    }
خاص:
    Aligned_معلومات_Ptr2_D(ثابت Aligned_معلومات_Ptr2_D&); // disabled
    Aligned_معلومات_Ptr2_D& چلوونکی=(ثابت Aligned_معلومات_Ptr2_D&); // disabled
};

#که_نه_تعریف_شوی CV_OPENCL_معلومات_PTR_ALIGNMENT
#تعریف_کړه CV_OPENCL_معلومات_PTR_ALIGNMENT 16
#د_که_پای


فراغ Context::Impl::__init_buffer_pools()
{
    بفر_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& بفر_Pool_3 = *بفر_Pool_2.get();
    بفر_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& بفر_Pool_Host_Ptr_2 = *بفر_Pool_Host_Ptr.get();

    size_t default_Pool_اندازه = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    size_t pool_اندازه = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    بفر_Pool_3.سېټ_Max_Reserved_اندازه(pool_اندازه);
    size_t pool_اندازه_Host_Ptr = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    بفر_Pool_Host_Ptr_2.سېټ_Max_Reserved_اندازه(pool_اندازه_Host_Ptr);

#که_تعریف_شوی HAVE_OPENCL_SVM
    بفر_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& بفر_Pool_SVM_2 = *بفر_Pool_SVM.get();
    size_t pool_اندازه_SVM = utils::get_Configuration_Parameter_اندازه_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_اندازه);
    بفر_Pool_SVM_2.سېټ_Max_Reserved_اندازه(pool_اندازه_SVM);
#د_که_پای

    CV_LOG_INFO(عدم, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_اندازه << " poolSizeHostPtr=" << pool_اندازه_Host_Ptr);
}

صنف OpenCLAllocator CV_FINAL : عام MatAllocator
{
عام:
    تعداد AllocatorFlags
    {
        ALLOCATOR_FLAGS_بفر_POOL_کارول_شوی = 1 << 0,
        ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_کارول_شوی = 1 << 1,
#که_تعریف_شوی HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_بفر_POOL_SVM_کارول_شوی = 1 << 2,
#د_که_پای
        ALLOCATOR_FLAGS_EXTERNAL_بفر = 1 << 3  // convertFromBuffer()
    };

    OpenCLAllocator()
    {
        matStdAllocator = Mat::getDefaultAllocator();
    }
    ~OpenCLAllocator()
    {
        flush_Cleanup_کتار();
    }

    UMat_معلومات* defaultAllocate(صحيح dims, ثابت صحيح* sizes, صحيح type, فراغ* معلومات, size_t* ګام,
            AccessFlag flags, UMatUsageFlags usageFlags) ثابت
    {
        UMat_معلومات* u = matStdAllocator->allocate(dims, sizes, type, معلومات, ګام, flags, usageFlags);
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

    فراغ get_غوره_Flags(ثابت Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, صحيح& createFlags, UMat_معلومات::حافظه_Flag& flags0) ثابت
    {
        ثابت Device& dev = ctx.device(0);
        createFlags = 0;
        إذا ((usageFlags & USAGE_ALLOCATE_HOST_حافظه) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        إذا (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.host_Unified_حافظه_2()
#که_نه_تعریف_شوی __APPLE__
                || dev.isIntel()
#د_که_پای
                )
            )
        )
            flags0 = static_cast<UMat_معلومات::حافظه_Flag>(0);
        وإلا
            flags0 = UMat_معلومات::COPY_ON_MAP;
    }

    UMat_معلومات* allocate(صحيح dims, ثابت صحيح* sizes, صحيح type,
                       فراغ* معلومات, size_t* ګام, AccessFlag flags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!useOpenCL())
            أرجع defaultAllocate(dims, sizes, type, معلومات, ګام, flags, usageFlags);

        flush_Cleanup_کتار();

        CV_Assert(معلومات == 0);
        size_t ټول = CV_ELEM_اندازه(type);
        لكل( صحيح i = dims-1; i >= 0; i-- )
        {
            إذا( ګام )
                ګام[i] = ټول;
            ټول *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        إذا (!ctx.getImpl())
            أرجع defaultAllocate(dims, sizes, type, معلومات, ګام, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        صحيح createFlags = 0;
        UMat_معلومات::حافظه_Flag flags0 = static_cast<UMat_معلومات::حافظه_Flag>(0);
        get_غوره_Flags(ctx, flags, usageFlags, createFlags, flags0);

        فراغ* handle = عدم;
        صحيح allocatorFlags = 0;

#که_تعریف_شوی HAVE_OPENCL_SVM
        ثابت svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        إذا (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_SVM_کارول_شوی;
            handle = ctxImpl.get_بفر_Pool_SVM().allocate(ټول);

            // this property is constant, so single buffer pool can be used here
            منطقي is_Fine_Grain_بفر = svmCaps.is_Support_Fine_Grain_بفر();
            allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
        }
        وإلا
#د_که_پای
        إذا (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_کارول_شوی;
            handle = ctxImpl.get_بفر_Pool().allocate(ټول);
        }
        وإلا إذا (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_کارول_شوی;
            handle = ctxImpl.get_بفر_Pool_Host_Ptr().allocate(ټول);
        }
        وإلا
        {
            CV_Assert(handle != عدم); // Unsupported, throw
        }

        إذا (!handle)
            أرجع defaultAllocate(dims, sizes, type, معلومات, ګام, flags, usageFlags);

        UMat_معلومات* u = جديد UMat_معلومات(هذا);
        u->معلومات = 0;
        u->size = ټول;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<فراغ>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->لنډمهاله_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(ریښتیا);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع u;
    }

    منطقي allocate(UMat_معلومات* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع خطأ;

        flush_Cleanup_کتار();

        UMat_معلومات_Auto_قلف lock(u);

        إذا(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            صحيح createFlags = 0;
            UMat_معلومات::حافظه_Flag flags0 = static_cast<UMat_معلومات::حافظه_Flag>(0);
            get_غوره_Flags(ctx, accessFlags, usageFlags, createFlags, flags0);

            منطقي copyOnMap = (flags0 & UMat_معلومات::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            صحيح allocatorFlags = 0;
            UMat_معلومات::حافظه_Flag لنډمهاله_UMat_Flags = static_cast<UMat_معلومات::حافظه_Flag>(0);
            فراغ* handle = عدم;
            cl_int retval = CL_SUCCESS;

#که_تعریف_شوی HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            منطقي useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            إذا (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                لنډمهاله_UMat_Flags = UMat_معلومات::لنډمهاله_UMAT;
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

                    cl_command_کتار q = عدم;
                    إذا (!is_Fine_Grain_بفر)
                    {
                        q = (cl_command_کتار)کتار::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (صحيح)u->size);
                        cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, عدم, عدم);
                        CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMap()");

                    }
                    حافظه_نقل(handle, u->origdata, u->size);
                    إذا (!is_Fine_Grain_بفر)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, handle, 0, عدم, عدم);
                        CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                    }

                    لنډمهاله_UMat_Flags = UMat_معلومات::لنډمهاله_UMAT | UMat_معلومات::لنډمهاله_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_بفر ? svm::OPENCL_SVM_FINE_GRAIN_بفر
                                                : svm::OPENCL_SVM_COARSE_GRAIN_بفر;
                }
            }
            وإلا
#د_که_پای
            {
                إذا( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                لنډمهاله_UMat_Flags = UMat_معلومات::لنډمهاله_UMAT;
                إذا (
                #که_تعریف_شوی __APPLE__
                    !copyOnMap &&
                #د_که_پای
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
                    CV_OCL_DBG_CHECK_پایله(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (اوږد اوږد صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                }
                إذا((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_بفر(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_پایله(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (اوږد اوږد صحيح)u->size, u->origdata, (فراغ*)handle).c_str());
                    لنډمهاله_UMat_Flags |= UMat_معلومات::لنډمهاله_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_پایله(retval, cv::format("clCreateBuffer() => %p", (فراغ*)handle).c_str());
            إذا(!handle || retval != CL_SUCCESS)
                أرجع خطأ;
            u->handle = handle;
            u->prevAllocator = u->currAllocator;
            u->currAllocator = هذا;
            u->flags |= لنډمهاله_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(ریښتیا);
        opencl_allocator_stats.onAllocate(u->size);
        أرجع ریښتیا;
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
            جمع_کړه_منزل_Cleanup_کتار(u);
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

#که_تعریف_شوی _WIN32
        إذا (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            أرجع;             // avoid any OpenCL calls
#د_که_پای
        إذا(u->لنډمهاله_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            إذا (u->hostCopyObsolete())
            {
#که_تعریف_شوی HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    إذا( u->لنډمهاله_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر);
                        منطقي is_Fine_Grain_بفر = (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر;
                        cl_command_کتار q = عدم;
                        إذا (!is_Fine_Grain_بفر)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0));
                            q = (cl_command_کتار)کتار::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMap()");
                        }
                        cl_پای(q);
                        حافظه_نقل(u->origdata, u->handle, u->size);
                        إذا (!is_Fine_Grain_بفر)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, u->handle, 0, عدم, عدم);
                            CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                        }
                    }
                    وإلا
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                وإلا
#د_که_پای
                {
                    cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();
                    إذا( u->لنډمهاله_Copied_UMat() )
                    {
                        Aligned_معلومات_Ptr<خطأ, ریښتیا> alignedPtr(u->origdata, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    وإلا
                    {
                        cl_int retval = 0;
                        إذا (u->لنډمهاله_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flush_Cleanup_کتار(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            فراغ* معلومات = cl_کتار_ته_ورزیاتول_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (اوږد اوږد صحيح)u->size, معلومات).c_str());
                            CV_Assert(u->origdata == معلومات && "Details: https://github.com/opencv/opencv/issues/6293");
                            إذا (u->original_UMat_معلومات)
                            {
                                CV_Assert(u->original_UMat_معلومات->معلومات == معلومات);
                            }
                            retval = cl_کتار_ته_ورزیاتول_Unmap_Mem_Object(q, (cl_mem)u->handle, معلومات, 0, 0, 0);
                            CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, معلومات, (اوږد اوږد صحيح)u->size).c_str());
                            CV_OCL_DBG_CHECK(cl_پای(q));
                        }
                    }
                }
                u->markHostCopyObsolete(خطأ);
            }
            وإلا
            {
                // nothing
            }
#که_تعریف_شوی HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
            {
                إذا( u->لنډمهاله_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            وإلا
#د_که_پای
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_پایله(retval, cv::format("clReleaseMemObject(ptr=%p)", (فراغ*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ریښتیا);
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
                u->markHostCopyObsolete(ریښتیا);
            }
            إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_کارول_شوی)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool().release((cl_mem)u->handle);
            }
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_HOST_PTR_کارول_شوی)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#که_تعریف_شوی HAVE_OPENCL_SVM
            وإلا إذا (u->allocatorFlags_ & ALLOCATOR_FLAGS_بفر_POOL_SVM_کارول_شوی)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.get();
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                وإلا إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, u->handle, 0, عدم, عدم);
                        CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_بفر_Pool_SVM().release((فراغ*)u->handle);
            }
#د_که_پای
            وإلا
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(ریښتیا);
            ړنګول u;
            u = عدم;
        }
        CV_Assert(u == عدم);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    فراغ map(UMat_معلومات* u, AccessFlag accessFlags) ثابت CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        إذا (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(ریښتیا);

        cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

        {
            إذا( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#که_تعریف_شوی HAVE_OPENCL_SVM
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
                {
                    إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                    {
                        Context& ctx = Context::getDefault();
                        ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                            cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, عدم, عدم);
                            CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_بفر_MAP;
                        }
                    }
                    cl_پای(q);
                    u->معلومات = (uchar*)u->handle;
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(ریښتیا);
                    أرجع;
                }
#د_که_پای

                cl_int retval = CL_SUCCESS;
                إذا (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->معلومات = (uchar*)cl_کتار_ته_ورزیاتول_Map_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_پایله(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (فراغ*)u->handle, (اوږد اوږد صحيح)u->size, u->معلومات).c_str());
                }
                إذا (u->معلومات && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(خطأ);
                    u->markDeviceMemMapped(ریښتیا);
                    أرجع;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_معلومات::COPY_ON_MAP;
            }

            إذا(!u->معلومات)
            {
                u->معلومات = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(ریښتیا);
            }
        }

        إذا (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_معلومات_Ptr<خطأ, ریښتیا> alignedPtr(u->معلومات, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
#که_تعریف_شوی HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == 0);
#د_که_پای
            cl_int retval = cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (اوږد اوږد صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(خطأ);
        }
    }

    فراغ unmap(UMat_معلومات* u) ثابت CV_OVERRIDE
    {
        إذا(!u)
            أرجع;


        CV_Assert(u->handle != 0);

        UMat_معلومات_Auto_قلف autolock(u);

        cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();
        cl_int retval = 0;
        إذا( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->معلومات != عدم);
#که_تعریف_شوی HAVE_OPENCL_SVM
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
            {
                إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
                {
                    Context& ctx = Context::getDefault();
                    ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, u->handle,
                                0, عدم, عدم);
                        CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                        cl_پای(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_بفر_MAP;
                    }
                }
                إذا (u->refcount == 0)
                    u->معلومات = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(ریښتیا);
                أرجع;
            }
#د_که_پای
            إذا (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = cl_کتار_ته_ورزیاتول_Unmap_Mem_Object(q, (cl_mem)u->handle, u->معلومات, 0, 0, 0);
                CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (فراغ*)u->handle, u->معلومات, (اوږد اوږد صحيح)u->size).c_str());
                إذا (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(cl_پای(q));
                }
                u->markDeviceMemMapped(خطأ);
                u->معلومات = 0;
                u->markDeviceCopyObsolete(خطأ);
                u->markHostCopyObsolete(ریښتیا);
            }
        }
        وإلا إذا( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_معلومات_Ptr<ریښتیا, خطأ> alignedPtr(u->معلومات, u->size, CV_OPENCL_معلومات_PTR_ALIGNMENT);
#که_تعریف_شوی HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == 0);
#د_که_پای
            retval = cl_کتار_ته_ورزیاتول_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (فراغ*)u->handle, (اوږد اوږد صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(خطأ);
            u->markHostCopyObsolete(ریښتیا);
        }
    }

    منطقي checkContinuous(صحيح dims, ثابت size_t sz[],
                         ثابت size_t srcofs[], ثابت size_t srcstep[],
                         ثابت size_t dstofs[], ثابت size_t dststep[],
                         size_t& ټول, size_t new_sz[],
                         size_t& srcrawofs, size_t new_srcofs[], size_t new_srcstep[],
                         size_t& dstrawofs, size_t new_dstofs[], size_t new_dststep[]) ثابت
    {
        منطقي iscontinuous = ریښتیا;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        ټول = sz[dims-1];
        لكل( صحيح i = dims-2; i >= 0; i-- )
        {
            إذا( i >= 0 && (ټول != srcstep[i] || ټول != dststep[i]) )
                iscontinuous = خطأ;
            ټول *= sz[i];
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
        UMat_معلومات_Auto_قلف autolock(u);

        إذا( u->معلومات && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            أرجع;
        }
        CV_Assert( u->handle != 0 );

        cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

        size_t ټول = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            ټول, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
        {
            CV_DbgAssert(u->معلومات == عدم || u->معلومات == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMap()");
            }
            cl_پای(q);
            إذا( iscontinuous )
            {
                حافظه_نقل(dstptr, (uchar*)u->handle + srcrawofs, ټول);
            }
            وإلا
            {
                // This code is from MatAllocator::download()
                صحيح isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)لوی_عدد );
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
                    حافظه_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                cl_پای(q);
            }
        }
        وإلا
#د_که_پای
        {
            إذا( iscontinuous )
            {
                Aligned_معلومات_Ptr<خطأ, ریښتیا> alignedPtr((uchar*)dstptr, ټول, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, ټول, alignedPtr.getAlignedPtr(), 0, 0, 0));
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
                ټول = align_اندازه(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                ټول = std::min(ټول, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, ټول, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظه_نقل( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            وإلا
            {
                Aligned_معلومات_Ptr2_D<خطأ, ریښتیا> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_معلومات_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
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
        CV_Assert(u->refcount == 0 || u->لنډمهاله_UMat());

        size_t ټول = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            ټول, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_معلومات_Auto_قلف autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        إذا( u->معلومات && (u->hostCopyObsolete() < u->deviceCopyObsolete() || ټول == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(خطأ);
            u->markDeviceCopyObsolete(ریښتیا);
            أرجع;
        }

        CV_Assert( u->handle != 0 );
        cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
        {
            CV_DbgAssert(u->معلومات == عدم || u->معلومات == u->handle);
            Context& ctx = Context::getDefault();
            ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_MAP) == 0);
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (صحيح)u->size);
                cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, عدم, عدم);
                CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMap()");
            }
            cl_پای(q);
            إذا( iscontinuous )
            {
                حافظه_نقل((uchar*)u->handle + dstrawofs, srcptr, ټول);
            }
            وإلا
            {
                // This code is from MatAllocator::upload()
                صحيح isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                لكل( صحيح i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (size_t)لوی_عدد );
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
                    حافظه_نقل(ptrs[1], ptrs[0], planesz);
            }
            إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_COARSE_GRAIN_بفر)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMUnmap(q, u->handle,
                        0, عدم, عدم);
                CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMUnmap()");
                cl_پای(q);
            }
        }
        وإلا
#د_که_پای
        {
            إذا( iscontinuous )
            {
                Aligned_معلومات_Ptr<ریښتیا, خطأ> alignedPtr((uchar*)srcptr, ټول, CV_OPENCL_معلومات_PTR_ALIGNMENT);
                cl_int retval = cl_کتار_ته_ورزیاتول_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, ټول, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (فراغ*)u->handle, (اوږد اوږد صحيح)dstrawofs, (اوږد اوږد صحيح)u->size, alignedPtr.getAlignedPtr()).c_str());
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
                ټول = align_اندازه(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                ټول = std::min(ټول, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ټول, ptr, 0, 0, 0));
                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظه_نقل( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Write_بفر(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, ټول, ptr, 0, 0, 0));
            }
            وإلا
            {
                Aligned_معلومات_Ptr2_D<ریښتیا, خطأ> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_معلومات_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Write_بفر_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(ریښتیا);
#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#د_که_پای
        {
            u->markHostCopyObsolete(ریښتیا);
        }
        u->markDeviceCopyObsolete(خطأ);
    }

    فراغ copy(UMat_معلومات* src, UMat_معلومات* dst, صحيح dims, ثابت size_t sz[],
              ثابت size_t srcofs[], ثابت size_t srcstep[],
              ثابت size_t dstofs[], ثابت size_t dststep[], منطقي _sync) ثابت CV_OVERRIDE
    {
        إذا(!src || !dst)
            أرجع;

        size_t ټول = 0, new_sz[] = {0, 0, 0};
        size_t srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        size_t dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        منطقي iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            ټول, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_معلومات_Auto_قلف src_autolock(src, dst);

        إذا( !src->handle || (src->معلومات && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->معلومات + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            أرجع;
        }
        إذا( !dst->handle || (dst->معلومات && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->معلومات + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(خطأ);
#که_تعریف_شوی HAVE_OPENCL_SVM
            إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            وإلا
#د_که_پای
            {
                dst->markDeviceCopyObsolete(ریښتیا);
            }
            أرجع;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
        {
            إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
            {
                Context& ctx = Context::getDefault();
                ثابت svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                إذا( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (صحيح)ټول);
                    cl_int حالت = svmFns->fn_cl_کتار_ته_ورزیاتول_SVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            ټول, 0, عدم, عدم);
                    CV_OCL_CHECK_پایله(حالت, "clEnqueueSVMMemcpy()");
                }
                وإلا
                {
                    cl_پای(q);
                    // This code is from MatAllocator::download()/upload()
                    صحيح isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    لكل( صحيح i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (size_t)لوی_عدد );
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
                        حافظه_نقل(ptrs[1], ptrs[0], planesz);
                }
            }
            وإلا
            {
                إذا ((src->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) != 0)
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
#د_که_پای
        {
            إذا( iscontinuous )
            {
                retval = cl_کتار_ته_ورزیاتول_Copy_بفر(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, ټول, 0, 0, 0);
                CV_OCL_CHECK_پایله(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (فراغ*)src->handle, (فراغ*)dst->handle, (اوږد اوږد صحيح)srcrawofs, (اوږد اوږد صحيح)dstrawofs, (اوږد اوږد صحيح)ټول).c_str());
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

                size_t src_ټول = align_اندازه(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_ټول = std::min(src_ټول, src->size - new_srcrawofs);
                size_t dst_ټول = align_اندازه(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_ټول = std::min(dst_ټول, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_ټول, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Read_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_ټول, dstptr, 0, 0, 0));

                لكل( size_t i = 0; i < new_sz[1]; i++ )
                    حافظه_نقل( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Write_بفر(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_ټول, dstptr, 0, 0, 0));
            }
            وإلا
            {
                CV_OCL_CHECK(retval = cl_کتار_ته_ورزیاتول_Copy_بفر_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        إذا (retval == CL_SUCCESS)
        {
            CV_IMPL_جمع_کړه(CV_IMPL_OCL)
        }

#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_بفر ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_بفر_ماسک) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        وإلا
#د_که_پای
        {
            dst->markHostCopyObsolete(ریښتیا);
        }
        dst->markDeviceCopyObsolete(خطأ);

        إذا( _sync )
        {
            CV_OCL_DBG_CHECK(cl_پای(q));
        }
    }

    بفر_Pool_Controller* get_بفر_Pool_Controller(ثابت حرف* id) ثابت CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        إذا (ctx.empty())
            أرجع عدم;
#که_تعریف_شوی HAVE_OPENCL_SVM
        إذا ((svm::checkForceSVMUmatUsage() && (id == عدم || strcmp(id, "OCL") == 0)) || (id != عدم && strcmp(id, "SVM") == 0))
        {
            أرجع &ctx.getImpl()->get_بفر_Pool_SVM();
        }
#د_که_پای
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

    بدلیدونکی cv::Mutex cleanup_کتار_Mutex;
    بدلیدونکی std::deque<UMat_معلومات*> cleanup_کتار;

    فراغ flush_Cleanup_کتار() ثابت
    {
        // TSAN false positive: see #27638
        إذا (!cleanup_کتار.empty())
        {
            std::deque<UMat_معلومات*> q;
            {
                cv::Auto_قلف lock(cleanup_کتار_Mutex);
                // TSAN false positive: see #27638
                q.swap(cleanup_کتار);
            }
            لكل (std::deque<UMat_معلومات*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    فراغ جمع_کړه_منزل_Cleanup_کتار(UMat_معلومات* u) ثابت
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::Auto_قلف lock(cleanup_کتار_Mutex);
            cleanup_کتار.push_back(u);
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
فراغ سېټ_اندازه( UMat& m, صحيح _dims, ثابت صحيح* _sz, ثابت size_t* ګامونه_2,
              منطقي auto_ګامونه = خطأ );
فراغ finalizeHdr(UMat& m);

} // namespace cv


نطاق cv { نطاق ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
فراغ convert_From_بفر(فراغ* cl_mem_بفر, size_t ګام, صحيح لیکې, صحيح ستون_شمېر, صحيح type, UMat& dst)
{
    صحيح d = 2;
    صحيح sizes[] = { لیکې, ستون_شمېر };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_ماسک) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    سېټ_اندازه(dst, d, sizes, 0, ریښتیا);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_بفر;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, حجم(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_بفر == mem_type);

    size_t ټول = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_اندازه, حجم(size_t), &ټول, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((صحيح)ګام >= ستون_شمېر * CV_ELEM_اندازه(type));
    CV_Assert(ټول >= لیکې * ګام);

    // attach clBuffer to UMatData
    dst.u = جديد UMat_معلومات(getOpenCLAllocator());
    dst.u->معلومات            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_بفر;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_معلومات::حافظه_Flag>(0);
    dst.u->handle          = cl_mem_بفر;
    dst.u->origdata        = 0;
    dst.u->prevAllocator   = 0;
    dst.u->size            = ټول;

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

    صحيح ژوروالی = CV_8U;
    بدل (fmt.image_channel_معلومات_type)
    {
    حالة CL_UNORM_INT8:
    حالة CL_UNSIGNED_INT8:
        ژوروالی = CV_8U;
        اكسر;

    حالة CL_SNORM_INT8:
    حالة CL_SIGNED_INT8:
        ژوروالی = CV_8S;
        اكسر;

    حالة CL_UNORM_INT16:
    حالة CL_UNSIGNED_INT16:
        ژوروالی = CV_16U;
        اكسر;

    حالة CL_SNORM_INT16:
    حالة CL_SIGNED_INT16:
        ژوروالی = CV_16S;
        اكسر;

    حالة CL_SIGNED_INT32:
        ژوروالی = CV_32S;
        اكسر;

    حالة CL_FLOAT:
        ژوروالی = CV_32F;
        اكسر;

    حالة CL_HALF_FLOAT:
        ژوروالی = CV_16F;
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
        type = CV_MAKE_TYPE(ژوروالی, 1);
        اكسر;

    حالة CL_RG:
    حالة CL_RA:
        type = CV_MAKE_TYPE(ژوروالی, 2);
        اكسر;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    حالة CL_RGBA:
    حالة CL_BGRA:
    حالة CL_ARGB:
        type = CV_MAKE_TYPE(ژوروالی, 4);
        اكسر;

    افتراضي:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        اكسر;
    }

    size_t ګام = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_لیکه_PITCH, حجم(size_t), &ګام, 0));

    size_t w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_پلنوالی, حجم(size_t), &w, 0));

    size_t h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_لوړوالی, حجم(size_t), &h, 0));

    dst.create((صحيح)h, (صحيح)w, type);

    cl_mem cl_بفر = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_کتار q = (cl_command_کتار)کتار::getDefault().ptr();

    size_t offset = 0;
    size_t src_origin[3] = { 0, 0, 0 };
    size_t region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم));

    CV_OCL_CHECK(cl_پای(q));

    أرجع;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

ساكن فراغ getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint عدد_Devices = 0;
    cl_int حالت = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, عدم, &عدد_Devices);
    إذا (حالت != CL_DEVICE_NOT_وموندل) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_پایله(حالت,
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

PlatformInfo& PlatformInfo::چلوونکی =(ثابت PlatformInfo& i)
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
    i.p = تش_اشاره;
}

PlatformInfo& PlatformInfo::چلوونکی = (PlatformInfo&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا(p)
            p->release();
        p = i.p;
        i.p = تش_اشاره;
    }
    أرجع *هذا;
}

صحيح PlatformInfo::device_شمېره() ثابت
{
    أرجع p ? (صحيح)p->devices.size() : 0;
}

فراغ PlatformInfo::getDevice(Device& device, صحيح d) ثابت
{
    CV_Assert(p && d < (صحيح)p->devices.size() );
    إذا(p)
        device.سېټ(p->devices[d]);
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
    ساكن ثابت حرف* tab[CV_ژوروالی_MAX*16]=
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
    صحيح cn = CV_MAT_CN(type), ژوروالی = CV_MAT_ژوروالی(type);
    ثابت حرف* پایله = cn > 16 ? تش_اشاره : tab[ژوروالی*16 + cn-1];
    CV_Assert(پایله);
    أرجع پایله;
}

ثابت حرف* memop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_ژوروالی_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), ژوروالی = CV_MAT_ژوروالی(type);
    ثابت حرف* پایله = cn > 16 ? تش_اشاره : tab[ژوروالی*16 + cn-1];
    CV_Assert(پایله);
    أرجع پایله;
}

ثابت حرف* vecop_Type_منزل_Str(صحيح type)
{
    ساكن ثابت حرف* tab[CV_ژوروالی_MAX*16] =
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
    صحيح cn = CV_MAT_CN(type), ژوروالی = CV_MAT_ژوروالی(type);
    ثابت حرف* پایله = cn > 16 ? 0 : tab[ژوروالی*16 + cn-1];
    CV_Assert(پایله);
    أرجع پایله;
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
    ثابت حرف *typestr = type_منزل_Str(CV_MAKETYPE(ddepth, cn));
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

ثابت حرف* getOpenCLErrorString(صحيح error_کوډ)
{
#تعریف_کړه CV_OCL_کوډ(id) حالة id: أرجع #id
#تعریف_کړه CV_OCL_کوډ_2(id, name) حالة id: أرجع #name
    بدل (error_کوډ)
    {
    CV_OCL_کوډ(CL_SUCCESS);
    CV_OCL_کوډ(CL_DEVICE_NOT_وموندل);
    CV_OCL_کوډ(CL_DEVICE_NOT_شته);
    CV_OCL_کوډ(CL_COMPILER_NOT_شته);
    CV_OCL_کوډ(CL_MEM_OBJECT_ویش_FAILURE);
    CV_OCL_کوډ(CL_بهر_د_RESOURCES);
    CV_OCL_کوډ(CL_بهر_د_HOST_حافظه);
    CV_OCL_کوډ(CL_PROFILING_INFO_NOT_شته);
    CV_OCL_کوډ(CL_MEM_COPY_OVERLAP);
    CV_OCL_کوډ(CL_IMAGE_FORMAT_MISMATCH);
    CV_OCL_کوډ(CL_IMAGE_FORMAT_NOT_SUPPORTED);
    CV_OCL_کوډ(CL_BUILD_پروګرام_FAILURE);
    CV_OCL_کوډ(CL_MAP_FAILURE);
    CV_OCL_کوډ(CL_MISALIGNED_SUB_بفر_دننه_واټن);
    CV_OCL_کوډ(CL_EXEC_حالت_ERROR_FOR_EVENTS_دننه_WAIT_LIST);
    CV_OCL_کوډ(CL_COMPILE_پروګرام_FAILURE);
    CV_OCL_کوډ(CL_LINKER_NOT_شته);
    CV_OCL_کوډ(CL_LINK_پروګرام_FAILURE);
    CV_OCL_کوډ(CL_DEVICE_برخه_کول_FAILED);
    CV_OCL_کوډ(CL_KERNEL_ARG_INFO_NOT_شته);
    CV_OCL_کوډ(CL_INVALID_ارزښت);
    CV_OCL_کوډ(CL_INVALID_DEVICE_TYPE);
    CV_OCL_کوډ(CL_INVALID_PLATFORM);
    CV_OCL_کوډ(CL_INVALID_DEVICE);
    CV_OCL_کوډ(CL_INVALID_CONTEXT);
    CV_OCL_کوډ(CL_INVALID_کتار_PROPERTIES);
    CV_OCL_کوډ(CL_INVALID_COMMAND_کتار);
    CV_OCL_کوډ(CL_INVALID_HOST_PTR);
    CV_OCL_کوډ(CL_INVALID_MEM_OBJECT);
    CV_OCL_کوډ(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_کوډ(CL_INVALID_IMAGE_اندازه);
    CV_OCL_کوډ(CL_INVALID_SAMPLER);
    CV_OCL_کوډ(CL_INVALID_دوګونی);
    CV_OCL_کوډ(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_کوډ(CL_INVALID_پروګرام);
    CV_OCL_کوډ(CL_INVALID_پروګرام_EXECUTABLE);
    CV_OCL_کوډ(CL_INVALID_KERNEL_NAME);
    CV_OCL_کوډ(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_کوډ(CL_INVALID_KERNEL);
    CV_OCL_کوډ(CL_INVALID_ARG_شاخص);
    CV_OCL_کوډ(CL_INVALID_ARG_ارزښت);
    CV_OCL_کوډ(CL_INVALID_ARG_اندازه);
    CV_OCL_کوډ(CL_INVALID_KERNEL_دلیلونه);
    CV_OCL_کوډ(CL_INVALID_WORK_DIMENSION);
    CV_OCL_کوډ(CL_INVALID_WORK_ډله_اندازه);
    CV_OCL_کوډ(CL_INVALID_WORK_توکی_اندازه);
    CV_OCL_کوډ(CL_INVALID_GLOBAL_دننه_واټن);
    CV_OCL_کوډ(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_کوډ(CL_INVALID_EVENT);
    CV_OCL_کوډ(CL_INVALID_OPERATION);
    CV_OCL_کوډ(CL_INVALID_GL_OBJECT);
    CV_OCL_کوډ(CL_INVALID_بفر_اندازه);
    CV_OCL_کوډ(CL_INVALID_MIP_LEVEL);
    CV_OCL_کوډ(CL_INVALID_GLOBAL_WORK_اندازه);
    // OpenCL 1.1
    CV_OCL_کوډ(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_کوډ(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_کوډ(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_کوډ(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_کوډ(CL_INVALID_DEVICE_برخه_کول_شمېر);
    // OpenCL 2.0
    CV_OCL_کوډ_2(-69, CL_INVALID_PIPE_اندازه);
    CV_OCL_کوډ_2(-70, CL_INVALID_DEVICE_کتار);
    // Extensions
    CV_OCL_کوډ_2(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_کوډ_2(-1001, CL_PLATFORM_NOT_وموندل_KHR);
    CV_OCL_کوډ_2(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_کوډ_2(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_کوډ_2(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_کوډ_2(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    افتراضي: أرجع "Unknown OpenCL error";
    }
#تعریف_لغوه CV_OCL_کوډ
#تعریف_لغوه CV_OCL_کوډ_2
}

کينډۍ <ډول_نوم T>
ساكن std::string ker_منزل_Str(ثابت Mat & k)
{
    صحيح پلنوالی = k.ستون_شمېر - 1, ژوروالی = k.ژوروالی();
    ثابت T * ثابت معلومات = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    إذا (ژوروالی <= CV_8S)
    {
        لكل (صحيح i = 0; i < پلنوالی; ++i)
            stream << "DIG(" << (صحيح)معلومات[i] << ")";
        stream << "DIG(" << (صحيح)معلومات[پلنوالی] << ")";
    }
    وإلا إذا (ژوروالی == CV_32F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < پلنوالی; ++i)
            stream << "DIG(" << معلومات[i] << "f)";
        stream << "DIG(" << معلومات[پلنوالی] << "f)";
    }
    وإلا إذا (ژوروالی == CV_16F)
    {
        stream.setf(std::ios_base::showpoint);
        لكل (صحيح i = 0; i < پلنوالی; ++i)
            stream << "DIG(" << (عائم)معلومات[i] << "h)";
        stream << "DIG(" << (عائم)معلومات[پلنوالی] << "h)";
    }
    وإلا
    {
        لكل (صحيح i = 0; i < پلنوالی; ++i)
            stream << "DIG(" << معلومات[i] << ")";
        stream << "DIG(" << معلومات[پلنوالی] << ")";
    }

    أرجع stream.str();
}

String kernel_منزل_Str(ننوت_Array _kernel, صحيح ddepth, ثابت حرف * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    صحيح ژوروالی = kernel.ژوروالی();
    إذا (ddepth < 0)
        ddepth = ژوروالی;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    إذا (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    إذا (ddepth != ژوروالی)
        kernel.convert_منزل(kernel, ddepth);

    تعريفنوع std::string (* func_t)(ثابت Mat &);
    ساكن ثابت func_t funcs[CV_ژوروالی_MAX] = { ker_منزل_Str<uchar>, ker_منزل_Str<حرف>, ker_منزل_Str<ushort>, ker_منزل_Str<لنډ>,
                                                ker_منزل_Str<صحيح>, ker_منزل_Str<عائم>, ker_منزل_Str<مزدوج>, ker_منزل_Str<hfloat>,
                                                0, ker_منزل_Str<منطقي>, ker_منزل_Str<uint64_t>, ker_منزل_Str<int64_t>, ker_منزل_Str<بینښه> };
    ثابت func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    أرجع cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#تعریف_کړه PROCESS_SRC(src) \
    افعل \
    { \
        إذا (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            اندازه csize = src.size(); \
            صحيح ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_ژوروالی(ctype), \
                ckercn = ویکټور_Widths[cdepth], cwidth = ccn * csize.پلنوالی; \
            إذا (cwidth < ckercn || ckercn <= 0) \
                أرجع 1; \
            ستون_شمېر.push_back(cwidth); \
            إذا (strat == OCL_ویکټور_OWN && ctype != ref_type) \
                أرجع 1; \
            offsets.push_back(src.offset()); \
            ګامونه.push_back(src.ګام()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    طالما ((فراغ)0, 0)

صحيح predict_Optimal_ویکټور_پلنوالی(ننوت_Array src1, ننوت_Array src2, ننوت_Array src3,
                              ننوت_Array src4, ننوت_Array src5, ننوت_Array src6,
                              ننوت_Array src7, ننوت_Array src8, ننوت_Array src9,
                              Ocl_ویکټور_Strategy strat)
{
    ثابت ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    صحيح ویکټور_Widths[CV_ژوروالی_MAX] = {
        d.preferred_ویکټور_پلنوالی_Char(), d.preferred_ویکټور_پلنوالی_Char(),   // CV_8U,  CV_8S
        d.preferred_ویکټور_پلنوالی_Short(), d.preferred_ویکټور_پلنوالی_Short(), // CV_16U, CV_16S
        d.preferred_ویکټور_پلنوالی_Int(), d.preferred_ویکټور_پلنوالی_Float(),   // CV_32S, CV_32F
        d.preferred_ویکټور_پلنوالی_Double(), d.preferred_ویکټور_پلنوالی_Half(), // CV_64F, CV_16F
        1,                                                            // CV_16BF
        d.preferred_ویکټور_پلنوالی_Char(),                                 // CV_Bool
        d.preferred_ویکټور_پلنوالی_Long(), d.preferred_ویکټور_پلنوالی_Long(),   // CV_64U, CV_64S
        d.preferred_ویکټور_پلنوالی_Int()                                   // CV_32U
    };

    // if the device says don't use vectors
    إذا (ویکټور_Widths[0] == 1)
    {
        // it's heuristic
        ویکټور_Widths[CV_8U] = ویکټور_Widths[CV_8S] = 4;
        ویکټور_Widths[CV_16U] = ویکټور_Widths[CV_16S] = ویکټور_Widths[CV_16F] = 2;
        ویکټور_Widths[CV_32S] = ویکټور_Widths[CV_32F] = ویکټور_Widths[CV_64F] = 1;
    }

    أرجع check_Optimal_ویکټور_پلنوالی(ویکټور_Widths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

صحيح check_Optimal_ویکټور_پلنوالی([[maybe_unused]] ثابت صحيح *ویکټور_Widths,
                            [[maybe_unused]] ننوت_Array src1,
                            [[maybe_unused]] ننوت_Array src2,
                            [[maybe_unused]] ننوت_Array src3,
                            [[maybe_unused]] ننوت_Array src4,
                            [[maybe_unused]] ننوت_Array src5,
                            [[maybe_unused]] ننوت_Array src6,
                            [[maybe_unused]] ننوت_Array src7,
                            [[maybe_unused]] ننوت_Array src8,
                            [[maybe_unused]] ننوت_Array src9,
                            [[maybe_unused]] Ocl_ویکټور_Strategy strat)
{
#که_تعریف_شوی __APPLE__
    أرجع 1;
#وإلا
    CV_Assert(ویکټور_Widths);

    صحيح ref_type = src1.type();

    std::vector<size_t> offsets, ګامونه, ستون_شمېر;
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
        طالما (offsets[i] % dividers[i] != 0 || ګامونه[i] % dividers[i] != 0 || ستون_شمېر[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    صحيح kercn = *std::min_element(kercns.begin(), kercns.end());

    أرجع kercn;
#د_که_پای
}

صحيح predict_Optimal_ویکټور_پلنوالی_Max(ننوت_Array src1, ننوت_Array src2, ننوت_Array src3,
                                 ننوت_Array src4, ننوت_Array src5, ننوت_Array src6,
                                 ننوت_Array src7, ننوت_Array src8, ننوت_Array src9)
{
    أرجع predict_Optimal_ویکټور_پلنوالی(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_ویکټور_MAX);
}

#تعریف_لغوه PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
فراغ build_Options_جمع_کړه_ماتریکس_Description(String& buildOptions, ثابت String& name, ننوت_Array _m)
{
    إذا (!buildOptions.empty())
        buildOptions += " ";
    صحيح type = _m.type(), ژوروالی = CV_MAT_ژوروالی(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::type_منزل_Str(type),
            name.c_str(), ocl::type_منزل_Str(CV_MAKE_TYPE(ژوروالی, 1)),
            name.c_str(), (صحيح)CV_MAT_CN(type),
            name.c_str(), (صحيح)CV_ELEM_اندازه(type),
            name.c_str(), (صحيح)CV_ELEM_SIZE1(type),
            name.c_str(), (صحيح)ژوروالی
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

    ساكن cl_image_format getImageFormat(صحيح ژوروالی, صحيح cn, منطقي norm)
    {
        cl_image_format format;
        ساكن ثابت صحيح channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        ساكن ثابت صحيح channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        ساكن ثابت صحيح channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        صحيح channelType = norm ? channelTypesNorm[ژوروالی] : channelTypes[ژوروالی];
        صحيح channel_ترتیب = channelOrders[cn];
        format.image_channel_معلومات_type = (cl_channel_type)channelType;
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
        CV_OCL_DBG_CHECK_پایله(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        إذا (عدد_Formats > 0)
        {
            Auto_بفر<cl_image_format> formats(عدد_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, عدد_Formats,
                                             formats.معلومات(), عدم);
            CV_OCL_DBG_CHECK_پایله(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            لكل (cl_uint i = 0; i < عدد_Formats; ++i)
            {
                إذا (!memcmp(&formats[i], &format, حجم(format)))
                {
                    أرجع ریښتیا;
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

        صحيح err, ژوروالی = src.ژوروالی(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(ژوروالی, cn, norm);

        إذا (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        إذا (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_کتار کتار_2 = (cl_command_کتار)کتار::getDefault().ptr();

#که_تعریف_شوی CL_نسخه_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        ثابت Device & d = ocl::Device::getDefault();
        صحيح minor = d.device_نسخه_Minor(), major = d.device_نسخه_Major();
        CV_Assert(!alias || canCreateAlias(src));
        إذا (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_پلنوالی      = src.ستون_شمېر;
            desc.image_لوړوالی     = src.لیکې;
            desc.image_ژوروالی      = 0;
            desc.image_array_اندازه = 1;
            desc.image_لیکه_pitch  = alias ? src.ګام[0] : 0;
            desc.image_ټوټه_pitch = 0;
            desc.بفر           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.عدد_mip_پوړونه   = 0;
            desc.عدد_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, عدم, &err);
        }
        وإلا
#د_که_پای
        {
            CV_SUPPRESS_DEPRECATED_پیل
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.ستون_شمېر, src.لیکې, 0, عدم, &err);
            CV_SUPPRESS_DEPRECATED_پای_ټکی
        }
        CV_OCL_DBG_CHECK_پایله(err, "clCreateImage()");

        size_t origin[] = { 0, 0, 0 };
        size_t region[] = { static_cast<size_t>(src.ستون_شمېر), static_cast<size_t>(src.لیکې), 1 };

        cl_mem dev_معلومات;
        إذا (!alias && !src.isContinuous())
        {
            dev_معلومات = cl_Create_بفر(context, CL_MEM_READ_ONLY, src.ستون_شمېر * src.لیکې * src.elem_اندازه(), عدم, &err);
            CV_OCL_CHECK_پایله(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (اوږد اوږد صحيح)(src.ستون_شمېر * src.لیکې * src.elem_اندازه()), (فراغ*)dev_معلومات
                ).c_str());

            ثابت size_t roi[3] = {static_cast<size_t>(src.ستون_شمېر) * src.elem_اندازه(), static_cast<size_t>(src.لیکې), 1};
            CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Copy_بفر_Rect(کتار_2, (cl_mem)src.handle(ACCESS_READ), dev_معلومات, origin, origin,
                roi, src.ګام, 0, src.ستون_شمېر * src.elem_اندازه(), 0, 0, عدم, عدم));
            CV_OCL_DBG_CHECK(clFlush(کتار_2));
        }
        وإلا
        {
            dev_معلومات = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_معلومات != عدم);

        إذا (!alias)
        {
            CV_OCL_CHECK(cl_کتار_ته_ورزیاتول_Copy_بفر_منزل_Image(کتار_2, dev_معلومات, handle, 0, origin, region, 0, عدم, 0));
            إذا (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(کتار_2));
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
        إذا (pitchAlign && !(m.ګام % (pitchAlign * m.elem_اندازه())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            إذا (!m.u->لنډمهاله_UMat())
            {
                ret = ریښتیا;
            }
        }
    }
    أرجع ret;
}

منطقي Image2D::isFormatSupported(صحيح ژوروالی, صحيح cn, منطقي norm)
{
    cl_image_format format = Impl::getImageFormat(ژوروالی, cn, norm);

    أرجع Impl::isFormatSupported(format);
}

Image2D::Image2D(ثابت Image2D & i)
{
    p = i.p;
    إذا (p)
        p->addref();
}

Image2D & Image2D::چلوونکی = (ثابت Image2D & i)
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
    i.p = تش_اشاره;
}

Image2D& Image2D::چلوونکی = (Image2D&& i) CV_NOEXCEPT
{
    إذا (هذا != &i) {
        إذا (p)
            p->release();
        p = i.p;
        i.p = تش_اشاره;
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
        initialized = ریښتیا;
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
        initialized = ریښتیا;
    }
    أرجع value;
}

منطقي internal::isCLBuffer(UMat& u)
{
    إذا (isCudaBacked(u.u))
        أرجع خطأ;
    فراغ* h = u.handle(ACCESS_RW);
    إذا (!h)
        أرجع ریښتیا;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#إذا 1
    إذا ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        أرجع خطأ;
#وإلا
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, حجم(type), &type, عدم);
    إذا (ret != CL_SUCCESS || type != CL_MEM_OBJECT_بفر)
        أرجع خطأ;
#د_که_پای
    أرجع ریښتیا;
}

بنية Timer::Impl
{
    ثابت کتار کتار_2;

    Impl(ثابت کتار& q)
        : کتار_2(q)
    {
    }

    ~Impl(){}

    فراغ پیل()
    {
        CV_OCL_DBG_CHECK(cl_پای((cl_command_کتار)کتار_2.ptr()));
        timer.پیل();
    }

    فراغ stop()
    {
        CV_OCL_DBG_CHECK(cl_پای((cl_command_کتار)کتار_2.ptr()));
        timer.stop();
    }

    uint64 durationNS() ثابت
    {
        أرجع (uint64)(timer.get_وخت_Sec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(ثابت کتار& q) : p(جديد Impl(q)) { }
Timer::~Timer() { ړنګول p; }

فراغ Timer::پیل()
{
    CV_Assert(p);
    p->پیل();
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

#د_که_پای // HAVE_OPENCL
