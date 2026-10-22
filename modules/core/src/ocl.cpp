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

#समावेश "precomp.hpp"

#अगर_अघोषित HAVE_OPENCL
#समावेश "ocl_disabled.impl.hpp"
#अन्यथा // HAVE_OPENCL

#समावेश <list>
#समावेश <map>
#समावेश <deque>
#समावेश <set>
#समावेश <string>
#समावेश <sstream>
#समावेश <fstream>
#अगर !(defined _MSC_VER) || (defined _MSC_VER && _MSC_VER > 1700)
#समावेश <inttypes.h>
#पूर्ण_अगर

#समावेश <opencv2/core/utils/configuration.private.hpp>

#समावेश <opencv2/core/utils/logger.defines.hpp>
#अघोषित CV_LOG_STRIP_LEVEL
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#समावेश <opencv2/core/utils/logger.hpp>

#समावेश "opencv2/core/ocl_genbase.hpp"
#समावेश "opencl_kernels_core.hpp"

#समावेश "opencv2/core/utils/lock.private.hpp"
#समावेश "opencv2/core/utils/filesystem.hpp"
#समावेश "opencv2/core/utils/filesystem.private.hpp"

#घोषणा CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(शून्य, 0, "OpenCL allocator: " << __VA_ARGS__)
#समावेश "opencv2/core/utils/allocator_stats.impl.hpp"
#अघोषित CV__ALLOCATOR_STATS_LOG

#घोषणा CV_OPENCL_ALWAYS_SHOW_BUILD_LOG          0
#घोषणा CV_OPENCL_SHOW_BUILD_OPTIONS             0
#घोषणा CV_OPENCL_SHOW_BUILD_KERNELS             0

#घोषणा CV_OPENCL_SHOW_RUN_KERNELS               0
#घोषणा CV_OPENCL_SYNC_RUN_KERNELS               0
#घोषणा CV_OPENCL_TRACE_CHECK                    0

#घोषणा CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS       1

#घोषणा CV_OPENCL_SHOW_SVM_ERROR_LOG             1
#घोषणा CV_OPENCL_SHOW_SVM_LOG                   0

#समावेश "opencv2/core/bufferpool.hpp"
#अगर_अघोषित LOG_बफर_POOL
# अगर 0
#   define LOG_बफर_POOL म_लिखो
# अन्यथा
#   define LOG_बफर_POOL(...)
# endif
#पूर्ण_अगर

#अगर CV_OPENCL_SHOW_SVM_LOG
// TODO add timestamp logging
#घोषणा CV_OPENCL_SVM_TRACE_P म_लिखो("line %d (ocl.cpp): ", __LINE__); म_लिखो
#अन्यथा
#घोषणा CV_OPENCL_SVM_TRACE_P(...)
#पूर्ण_अगर

#अगर CV_OPENCL_SHOW_SVM_ERROR_LOG
// TODO add timestamp logging
#घोषणा CV_OPENCL_SVM_TRACE_ERROR_P म_लिखो("Error on line %d (ocl.cpp): ", __LINE__); म_लिखो
#अन्यथा
#घोषणा CV_OPENCL_SVM_TRACE_ERROR_P(...)
#पूर्ण_अगर

#समावेश "opencv2/core/opencl/runtime/opencl_clblas.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_clfft.hpp"

#समावेश "opencv2/core/opencl/runtime/opencl_core.hpp"

#अगर_घोषित HAVE_OPENCL_SVM
#समावेश "opencv2/core/opencl/runtime/opencl_svm_20.hpp"
#समावेश "opencv2/core/opencl/runtime/opencl_svm_hsa_extension.hpp"
#समावेश "opencv2/core/opencl/opencl_svm.hpp"
#पूर्ण_अगर

#समावेश "umatrix.hpp"

namespace cv { namespace ocl {

#घोषणा IMPLEMENT_REFCOUNTABLE() \
    व्योम addref() { CV_XADD(&refcount, 1); } \
    व्योम release() { अगर( CV_XADD(&refcount, -1) == 1 && !cv::__termination) मिटाओ यह; } \
    पूर्णांक refcount

अटल cv::utils::AllocatorStatistics opencl_allocator_stats;

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics();
cv::utils::AllocatorStatisticsInterface& getOpenCLAllocatorStatistics()
{
    वापस opencl_allocator_stats;
}

#अगर_अघोषित _DEBUG
अटल bool isRaiseError()
{
    अटल bool initialized = false;
    अटल bool value = false;
    अगर (!initialized)
    {
        value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_RAISE_ERROR", false);
        initialized = true;
    }
    वापस value;
}
#पूर्ण_अगर

अटल व्योम onOpenCLKernelBuildError()
{
    // NB: no need to cache this value
    bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_ABORT_ON_BUILD_ERROR", false);
    अगर (value)
    {
        ख_लिखो(मानक_त्रुटि, "Abort on OpenCL kernel build failure!\n");
        पात();
    }
}

#अगर CV_OPENCL_TRACE_CHECK
अटल अंतरभूत
व्योम traceOpenCLCheck(cl_int status, स्थिर अक्षर* message)
{
    std::अ_बाहर << "OpenCV(OpenCL:" << status << "): " << message << std::प_पूर्ण << std::सफाई;
}
#घोषणा CV_OCL_TRACE_CHECK_परिणाम(status, message) traceOpenCLCheck(status, message)
#अन्यथा
#घोषणा CV_OCL_TRACE_CHECK_परिणाम(status, message) /* nothing */
#पूर्ण_अगर

#घोषणा CV_OCL_API_ERROR_संदेश(check_परिणाम, संदेश) \
    cv::format("OpenCL error %s (%d) during call: %s", getOpenCLErrorString(check_परिणाम), check_परिणाम, संदेश)

#घोषणा CV_OCL_CHECK_परिणाम(check_परिणाम, संदेश) \
    करो { \
        CV_OCL_TRACE_CHECK_परिणाम(check_परिणाम, संदेश); \
        अगर (check_परिणाम != CL_SUCCESS) \
        { \
            static_assert(std::is_convertible<decltype(संदेश), स्थिर अक्षर*>::value, "msg of CV_OCL_CHECK_RESULT must be const char*"); \
            cv::String error_संदेश = CV_OCL_API_ERROR_संदेश(check_परिणाम, संदेश); \
            CV_Error(Error::OpenCLApiCallError, error_संदेश); \
        } \
    } जबतक (0)

#घोषणा CV_OCL_CHECK_(व्यंजक, check_परिणाम) करो { व्यंजक; CV_OCL_CHECK_परिणाम(check_परिणाम, #expr); } जबतक (0)

#घोषणा CV_OCL_CHECK(व्यंजक) करो { cl_int __cl_result = (व्यंजक); CV_OCL_CHECK_परिणाम(__cl_result, #expr); } जबतक (0)

#अगर_घोषित _DEBUG
#घोषणा CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, संदेश) CV_OCL_CHECK_परिणाम(check_परिणाम, संदेश)
#घोषणा CV_OCL_DBG_CHECK(व्यंजक) CV_OCL_CHECK(व्यंजक)
#घोषणा CV_OCL_DBG_CHECK_(व्यंजक, check_परिणाम) CV_OCL_CHECK_(व्यंजक, check_परिणाम)
#अन्यथा
#घोषणा CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, संदेश) \
    करो { \
        CV_OCL_TRACE_CHECK_परिणाम(check_परिणाम, संदेश); \
        अगर (check_परिणाम != CL_SUCCESS && isRaiseError()) \
        { \
            static_assert(std::is_convertible<decltype(संदेश), स्थिर अक्षर*>::value, "msg of CV_OCL_DBG_CHECK_RESULT must be const char*"); \
            cv::String error_संदेश = CV_OCL_API_ERROR_संदेश(check_परिणाम, संदेश); \
            CV_Error(Error::OpenCLApiCallError, error_संदेश); \
        } \
    } जबतक (0)
#घोषणा CV_OCL_DBG_CHECK_(व्यंजक, check_परिणाम) करो { व्यंजक; CV_OCL_DBG_CHECK_परिणाम(check_परिणाम, #expr); } जबतक (0)
#घोषणा CV_OCL_DBG_CHECK(व्यंजक) करो { cl_int __cl_result = (व्यंजक); CV_OCL_DBG_CHECK_परिणाम(__cl_result, #expr); } जबतक (0)
#पूर्ण_अगर


अटल स्थिर bool CV_OPENCL_CACHE_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_ENABLE", true);
अटल स्थिर bool CV_OPENCL_CACHE_WRITE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_WRITE", true);
अटल स्थिर bool CV_OPENCL_CACHE_LOCK_ENABLE = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_LOCK_ENABLE", true);
अटल स्थिर bool CV_OPENCL_CACHE_CLEANUP = utils::getConfigurationParameterBool("OPENCV_OPENCL_CACHE_CLEANUP", true);

#अगर CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS
अटल स्थिर bool CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS_मूल्य = utils::getConfigurationParameterBool("OPENCV_OPENCL_VALIDATE_BINARY_PROGRAMS", false);
#पूर्ण_अगर

// Option to disable calls clEnqueueReadBufferRect / clEnqueueWriteBufferRect / clEnqueueCopyBufferRect
अटल स्थिर bool CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS = utils::getConfigurationParameterBool("OPENCV_OPENCL_DISABLE_BUFFER_RECT_OPERATIONS",
#अगर 1 //def __APPLE__
        true
#अन्यथा
        false
#पूर्ण_अगर
);

अटल String getBuildExtraOptions()
{
    अटल String param_buildExtraOptions;
    अटल bool initialized = false;
    अगर (!initialized)
    {
        param_buildExtraOptions = utils::getConfigurationParameterString("OPENCV_OPENCL_BUILD_EXTRA_OPTIONS", "");
        initialized = true;
        अगर (!param_buildExtraOptions.empty())
            CV_LOG_WARNING(शून्य, "OpenCL: using extra build options: '" << param_buildExtraOptions << "'");
    }
    वापस param_buildExtraOptions;
}

अटल स्थिर bool CV_OPENCL_ENABLE_MEM_USE_HOST_PTR = utils::getConfigurationParameterBool("OPENCV_OPENCL_ENABLE_MEM_USE_HOST_PTR", true);
अटल स्थिर माप_प्रकार CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR", 4);


काष्ठा UMat2D
{
    UMat2D(स्थिर UMat& m)
    {
        offset = (पूर्णांक)m.offset;
        step = (पूर्णांक)m.step;
        rows = m.rows;
        cols = m.cols;
    }
    पूर्णांक offset;
    पूर्णांक step;
    पूर्णांक rows;
    पूर्णांक cols;
};

काष्ठा UMat3D
{
    UMat3D(स्थिर UMat& m)
    {
        offset = (पूर्णांक)m.offset;
        step = (पूर्णांक)m.step.p[1];
        slicestep = (पूर्णांक)m.step.p[0];
        slices = (पूर्णांक)m.size.p[0];
        rows = m.size.p[1];
        cols = m.size.p[2];
    }
    पूर्णांक offset;
    पूर्णांक slicestep;
    पूर्णांक step;
    पूर्णांक slices;
    पूर्णांक rows;
    पूर्णांक cols;
};

// Computes 64-bit "cyclic redundancy check" sum, as specified in ECMA-182
अटल uint64 crc64( स्थिर uchar* तथ्य, माप_प्रकार size, uint64 crc0=0 )
{
    अटल uint64 table[256];
    अटल bool initialized = false;

    अगर( !initialized )
    {
        क्रम( पूर्णांक i = 0; i < 256; i++ )
        {
            uint64 c = i;
            क्रम( पूर्णांक j = 0; j < 8; j++ )
                c = ((c & 1) ? CV_BIG_UINT(0xc96c5795d7870f42) : 0) ^ (c >> 1);
            table[i] = c;
        }
        initialized = true;
    }

    uint64 crc = ~crc0;
    क्रम( माप_प्रकार idx = 0; idx < size; idx++ )
        crc = table[(uchar)crc ^ तथ्य[idx]] ^ (crc >> 8);

    वापस ~crc;
}

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
काष्ठा OpenCLBinaryCacheConfigurator
{
    cv::String cache_path_;
    cv::String cache_lock_filename_;
    cv::Ptr<utils::fs::FileLock> cache_lock_;

    प्रकार std::map<std::string, std::string> ContextCacheType;
    ContextCacheType prepared_contexts_;
    Mutex mutex_prepared_contexts_;

    OpenCLBinaryCacheConfigurator()
    {
        CV_LOG_DEBUG(शून्य, "Initializing OpenCL cache configuration...");
        अगर (!CV_OPENCL_CACHE_ENABLE)
        {
            CV_LOG_INFO(शून्य, "OpenCL cache is disabled");
            वापस;
        }
        cache_path_ = utils::fs::getCacheDirectory("opencl_cache", "OPENCV_OPENCL_CACHE_DIR");
        अगर (cache_path_.empty())
        {
            CV_LOG_INFO(शून्य, "Specify OPENCV_OPENCL_CACHE_DIR configuration parameter to enable OpenCL cache");
        }
        करो
        {
            प्रयास
            {
                अगर (cache_path_.empty())
                    अवरोध;
                अगर (cache_path_ == "disabled")
                    अवरोध;
                अगर (!utils::fs::createDirectories(cache_path_))
                {
                    CV_LOG_DEBUG(शून्य, "Can't use OpenCL cache directory: " << cache_path_);
                    सफा();
                    अवरोध;
                }

                अगर (CV_OPENCL_CACHE_LOCK_ENABLE)
                {
                    cache_lock_filename_ = cache_path_ + ".lock";
                    अगर (!utils::fs::exists(cache_lock_filename_))
                    {
                        CV_LOG_DEBUG(शून्य, "Creating lock file... (" << cache_lock_filename_ << ")");
                        std::बख_प्रवाह lock_filename(cache_lock_filename_.c_str(), std::पनप::out);
                        अगर (!lock_filename.is_open())
                        {
                            CV_LOG_WARNING(शून्य, "Can't create lock file for OpenCL program cache: " << cache_lock_filename_);
                            अवरोध;
                        }
                    }

                    प्रयास
                    {
                        cache_lock_ = makePtr<utils::fs::FileLock>(cache_lock_filename_.c_str());
                        CV_LOG_VERBOSE(शून्य, 0, "Checking cache lock... (" << cache_lock_filename_ << ")");
                        {
                            utils::shared_lock_guard<utils::fs::FileLock> lock(*cache_lock_);
                        }
                        CV_LOG_VERBOSE(शून्य, 0, "Checking cache lock... Done!");
                    }
                    पकड़ो (स्थिर cv::Exception& e)
                    {
                        CV_LOG_WARNING(शून्य, "Can't create OpenCL program cache lock: " << cache_lock_filename_ << std::प_पूर्ण << e.what());
                    }
                    पकड़ो (...)
                    {
                        CV_LOG_WARNING(शून्य, "Can't create OpenCL program cache lock: " << cache_lock_filename_);
                    }
                }
                अन्यथा
                {
                    अगर (CV_OPENCL_CACHE_WRITE)
                    {
                        CV_LOG_WARNING(शून्य, "OpenCL cache lock is disabled while cache write is allowed "
                                "(not safe for multiprocess environment)");
                    }
                    अन्यथा
                    {
                        CV_LOG_INFO(शून्य, "OpenCL cache lock is disabled");
                    }
                }
            }
            पकड़ो (स्थिर cv::Exception& e)
            {
                CV_LOG_WARNING(शून्य, "Can't prepare OpenCL program cache: " << cache_path_ << std::प_पूर्ण << e.what());
                सफा();
            }
        } जबतक (0);
        अगर (!cache_path_.empty())
        {
            अगर (cache_lock_.empty() && CV_OPENCL_CACHE_LOCK_ENABLE)
            {
                CV_LOG_WARNING(शून्य, "Initialized OpenCL cache directory, but interprocess synchronization lock is not available. "
                        "Consider to disable OpenCL cache: OPENCV_OPENCL_CACHE_DIR=disabled");
            }
            अन्यथा
            {
                CV_LOG_INFO(शून्य, "Successfully initialized OpenCL cache directory: " << cache_path_);
            }
        }
    }

    व्योम सफा()
    {
        cache_path_.सफा();
        cache_lock_filename_.सफा();
        cache_lock_.release();
    }

    std::string prepareCacheDirectoryForContext(स्थिर std::string& ctx_prefix,
            स्थिर std::string& cleanup_prefix)
    {
        अगर (cache_path_.empty())
            वापस std::string();

        AutoLock lock(mutex_prepared_contexts_);

        ContextCacheType::iterator found_it = prepared_contexts_.find(ctx_prefix);
        अगर (found_it != prepared_contexts_.end())
            वापस found_it->second;

        CV_LOG_INFO(शून्य, "Preparing OpenCL cache configuration for context: " << ctx_prefix);

        std::string target_directory = cache_path_ + ctx_prefix + "/";
        bool परिणाम = utils::fs::isDirectory(target_directory);
        अगर (!परिणाम)
        {
            प्रयास
            {
                CV_LOG_VERBOSE(शून्य, 0, "Creating directory: " << target_directory);
                अगर (utils::fs::createDirectories(target_directory))
                {
                    परिणाम = true;
                }
                अन्यथा
                {
                    CV_LOG_WARNING(शून्य, "Can't create directory: " << target_directory);
                }
            }
            पकड़ो (स्थिर cv::Exception& e)
            {
                CV_LOG_ERROR(शून्य, "Can't create OpenCL program cache directory for context: " << target_directory << std::प_पूर्ण << e.what());
            }
        }
        target_directory = परिणाम ? target_directory : std::string();
        prepared_contexts_.insert(std::pair<std::string, std::string>(ctx_prefix, target_directory));

        अगर (परिणाम && CV_OPENCL_CACHE_CLEANUP && CV_OPENCL_CACHE_WRITE && !cleanup_prefix.empty())
        {
            प्रयास
            {
                std::vector<String> entries;
                utils::fs::glob_relative(cache_path_, cleanup_prefix + "*", entries, false, true);
                std::vector<String> remove_entries;
                क्रम (माप_प्रकार i = 0; i < entries.size(); i++)
                {
                    स्थिर String& name = entries[i];
                    अगर (0 == name.find(cleanup_prefix))
                    {
                        अगर (0 == name.find(ctx_prefix))
                            जारी; // skip current
                        remove_entries.push_back(name);
                    }
                }
                अगर (!remove_entries.empty())
                {
                    CV_LOG_WARNING(शून्य, (remove_entries.size() == 1
                            ? "Detected OpenCL cache directory for other version of OpenCL device."
                            : "Detected OpenCL cache directories for other versions of OpenCL device.")
                            << " We assume that these directories are obsolete after OpenCL runtime/drivers upgrade.");
                    CV_LOG_WARNING(शून्य, "Trying to remove these directories...");
                    क्रम (माप_प्रकार i = 0; i < remove_entries.size(); i++)
                    {
                        CV_LOG_WARNING(शून्य, "- " << remove_entries[i]);
                    }
                    CV_LOG_WARNING(शून्य, "Note: You can disable this behavior via this option: OPENCV_OPENCL_CACHE_CLEANUP=0");

                    क्रम (माप_प्रकार i = 0; i < remove_entries.size(); i++)
                    {
                        स्थिर String& name = remove_entries[i];
                        cv::String path = utils::fs::join(cache_path_, name);
                        प्रयास
                        {
                            utils::fs::remove_all(path);
                            CV_LOG_WARNING(शून्य, "Removed: " << path);
                        }
                        पकड़ो (स्थिर cv::Exception& e)
                        {
                            CV_LOG_ERROR(शून्य, "Exception during removal of obsolete OpenCL cache directory: " << path << std::प_पूर्ण << e.what());
                        }
                    }
                }
            }
            पकड़ो (...)
            {
                CV_LOG_WARNING(शून्य, "Can't check for obsolete OpenCL cache directories");
            }
        }

        CV_LOG_VERBOSE(शून्य, 1, "  Result: " << (target_directory.empty() ? std::string("Failed") : target_directory));
        वापस target_directory;
    }

    अटल OpenCLBinaryCacheConfigurator& getSingletonInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(OpenCLBinaryCacheConfigurator, नया OpenCLBinaryCacheConfigurator());
    }
};
श्रेणी द्विआधारी_Program_File
{
    क्रमागत { MAX_ENTRIES = 64 };

    प्रकार अचिन्हित पूर्णांक uint32_t;

    काष्ठा CV_DECL_ALIGNED(4) FileHeader
    {
        uint32_t source_Signature_आकार;
        //char sourceSignature[];
    };

    काष्ठा CV_DECL_ALIGNED(4) FileTable
    {
        uint32_t संख्या_Of_Entries;
        //uint32_t firstEntryOffset[];
    };

    काष्ठा CV_DECL_ALIGNED(4) FileEntry
    {
        uint32_t अग्र_Entry_File_Offset; // 0 for the last entry in chain
        uint32_t कुंजी_आकार;
        uint32_t तथ्य_आकार;
        //char key[];
        //char data[];
    };

    स्थिर std::string fileName_;
    स्थिर अक्षर* स्थिर sourceSignature_;
    स्थिर माप_प्रकार source_Signature_आकार_2;

    std::ख_प्रवाह f;

    uint32_t entryOffsets[MAX_ENTRIES];

    uint32_t getHash(स्थिर std::string& options)
    {
        uint64 hash = crc64((स्थिर uchar*)options.c_str(), options.size(), 0);
        वापस hash & (MAX_ENTRIES - 1);
    }

    अंतरभूत माप_प्रकार get_File_आकार()
    {
        माप_प्रकार pos = (माप_प्रकार)f.tellg();
        f.ग_स्थान(0, std::ख_प्रवाह::end);
        माप_प्रकार file_आकार = (माप_प्रकार)f.tellg();
        f.ग_स्थान(pos, std::ख_प्रवाह::beg);
        वापस file_आकार;
    }
    अंतरभूत uint32_t readUInt32()
    {
        uint32_t res = 0;
        f.पढ़ो((अक्षर*)&res, माप(uint32_t));
        CV_Assert(!f.हार());
        वापस res;
    }
    अंतरभूत व्योम writeUInt32(स्थिर uint32_t value)
    {
        uint32_t v = value;
        f.ग_लिखो((अक्षर*)&v, माप(uint32_t));
        CV_Assert(!f.हार());
    }

    अंतरभूत व्योम seekReadAbsolute(माप_प्रकार pos)
    {
        f.ग_स्थान(pos, std::ख_प्रवाह::beg);
        CV_Assert(!f.हार());
    }
    अंतरभूत व्योम seekReadRelative(माप_प्रकार pos)
    {
        f.ग_स्थान(pos, std::ख_प्रवाह::cur);
        CV_Assert(!f.हार());
    }

    अंतरभूत व्योम seekWriteAbsolute(माप_प्रकार pos)
    {
        f.प_स्थान(pos, std::ख_प्रवाह::beg);
        CV_Assert(!f.हार());
    }

    व्योम clearFile()
    {
        f.बंद();
        अगर (0 != हटाओ(fileName_.c_str()))
            CV_LOG_ERROR(शून्य, "Can't remove: " << fileName_);
        वापस;
    }

खुला:
    द्विआधारी_Program_File(स्थिर std::string& fileName, स्थिर अक्षर* sourceSignature)
        : fileName_(fileName), sourceSignature_(sourceSignature), source_Signature_आकार_2(sourceSignature_ ? म_माप(sourceSignature_) : 0)
    {
        CV_StaticAssert(माप(uint32_t) == 4, "");
        CV_Assert(sourceSignature_ != शून्य);
        CV_Assert(source_Signature_आकार_2 > 0);
        स_रखो(entryOffsets, 0, माप(entryOffsets));

        f.rdbuf()->pubsetbuf(0, 0); // disable buffering
        f.खोलो(fileName_.c_str(), std::पनप::in|std::पनप::out|std::पनप::द्विग);
        अगर(f.is_open() && get_File_आकार() > 0)
        {
            bool isValid = false;
            प्रयास
            {
                uint32_t file_Source_Signature_आकार = readUInt32();
                अगर (file_Source_Signature_आकार == source_Signature_आकार_2)
                {
                    cv::Auto_बफर<अक्षर> fileSourceSignature(file_Source_Signature_आकार + 1);
                    f.पढ़ो(fileSourceSignature.तथ्य(), file_Source_Signature_आकार);
                    अगर (f.खप())
                    {
                        CV_LOG_ERROR(शून्य, "Unexpected EOF");
                    }
                    अन्यथा अगर (स_भेद(sourceSignature, fileSourceSignature.तथ्य(), file_Source_Signature_आकार) == 0)
                    {
                        isValid = true;
                    }
                }
                अगर (!isValid)
                {
                    CV_LOG_ERROR(शून्य, "Source code signature/hash mismatch (program source code has been changed/updated)");
                }
            }
            पकड़ो (स्थिर cv::Exception& e)
            {
                CV_LOG_ERROR(शून्य, "Can't open binary program file: " << fileName << " : " << e.what());
            }
            पकड़ो (...)
            {
                CV_LOG_ERROR(शून्य, "Can't open binary program file: " << fileName << " : Unknown error");
            }
            अगर (!isValid)
            {
                clearFile();
            }
            अन्यथा
            {
                seekReadAbsolute(0);
            }
        }
    }

    bool पढ़ो(स्थिर std::string& कुंजी, std::vector<अक्षर>& buf)
    {
        अगर (!f.is_open())
            वापस false;

        माप_प्रकार file_आकार = get_File_आकार();
        अगर (file_आकार == 0)
        {
            CV_LOG_ERROR(शून्य, "Invalid file (empty): " << fileName_);
            clearFile();
            वापस false;
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_आकार = readUInt32();
        CV_Assert(file_Source_Signature_आकार > 0);
        seekReadRelative(file_Source_Signature_आकार);

        uint32_t संख्या_Of_Entries = readUInt32();
        CV_Assert(संख्या_Of_Entries > 0);
        अगर (संख्या_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(शून्य, "Invalid file: " << fileName_);
            clearFile();
            वापस false;
        }
        f.पढ़ो((अक्षर*)&entryOffsets[0], माप(entryOffsets));
        CV_Assert(!f.हार());

        uint32_t entry_अंक = getHash(कुंजी);

        uint32_t entryOffset = entryOffsets[entry_अंक];
        FileEntry entry;
        जबतक (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पढ़ो((अक्षर*)&entry, माप(entry));
            CV_Assert(!f.हार());
            cv::Auto_बफर<अक्षर> file_कुंजी(entry.कुंजी_आकार + 1);
            अगर (कुंजी.size() == entry.कुंजी_आकार)
            {
                अगर (entry.कुंजी_आकार > 0)
                {
                    f.पढ़ो(file_कुंजी.तथ्य(), entry.कुंजी_आकार);
                    CV_Assert(!f.हार());
                }
                अगर (स_भेद(file_कुंजी.तथ्य(), कुंजी.c_str(), entry.कुंजी_आकार) == 0)
                {
                    buf.resize(entry.तथ्य_आकार);
                    f.पढ़ो(&buf[0], entry.तथ्य_आकार);
                    CV_Assert(!f.हार());
                    seekReadAbsolute(0);
                    CV_LOG_VERBOSE(शून्य, 0, "Read...");
                    वापस true;
                }
            }
            अगर (entry.अग्र_Entry_File_Offset == 0)
                अवरोध;
            entryOffset = entry.अग्र_Entry_File_Offset;
        }
        वापस false;
    }

    bool ग_लिखो(स्थिर std::string& कुंजी, std::vector<अक्षर>& buf)
    {
        अगर (!f.is_open())
        {
            f.खोलो(fileName_.c_str(), std::पनप::in|std::पनप::out|std::पनप::द्विग);
            अगर (!f.is_open())
            {
                f.खोलो(fileName_.c_str(), std::पनप::out|std::पनप::द्विग);
                अगर (!f.is_open())
                {
                    CV_LOG_ERROR(शून्य, "Can't create file: " << fileName_);
                    वापस false;
                }
            }
        }

        माप_प्रकार file_आकार = get_File_आकार();
        अगर (file_आकार == 0)
        {
            // Write header
            seekWriteAbsolute(0);
            writeUInt32((uint32_t)source_Signature_आकार_2);
            f.ग_लिखो(sourceSignature_, source_Signature_आकार_2);
            CV_Assert(!f.हार());

            writeUInt32(MAX_ENTRIES);
            स_रखो(entryOffsets, 0, माप(entryOffsets));
            f.ग_लिखो((अक्षर*)entryOffsets, माप(entryOffsets));
            CV_Assert(!f.हार());
            f.सफाई();
            CV_Assert(!f.हार());
            f.बंद();
            f.खोलो(fileName_.c_str(), std::पनप::in|std::पनप::out|std::पनप::द्विग);
            CV_Assert(f.is_open());
            file_आकार = get_File_आकार();
        }
        seekReadAbsolute(0);

        // bypass FileHeader
        uint32_t file_Source_Signature_आकार = readUInt32();
        CV_Assert(file_Source_Signature_आकार == source_Signature_आकार_2);
        seekReadRelative(file_Source_Signature_आकार);

        uint32_t संख्या_Of_Entries = readUInt32();
        CV_Assert(संख्या_Of_Entries > 0);
        अगर (संख्या_Of_Entries != MAX_ENTRIES)
        {
            CV_LOG_ERROR(शून्य, "Invalid file: " << fileName_);
            clearFile();
            वापस false;
        }
        माप_प्रकार tableEntriesOffset = (माप_प्रकार)f.tellg();
        f.पढ़ो((अक्षर*)&entryOffsets[0], माप(entryOffsets));
        CV_Assert(!f.हार());

        uint32_t entry_अंक = getHash(कुंजी);

        uint32_t entryOffset = entryOffsets[entry_अंक];
        FileEntry entry;
        जबतक (entryOffset > 0)
        {
            seekReadAbsolute(entryOffset);
            //CV_StaticAssert(sizeof(entry) == sizeof(uint32_t) * 3, "");
            f.पढ़ो((अक्षर*)&entry, माप(entry));
            CV_Assert(!f.हार());
            cv::Auto_बफर<अक्षर> file_कुंजी(entry.कुंजी_आकार + 1);
            अगर (कुंजी.size() == entry.कुंजी_आकार)
            {
                अगर (entry.कुंजी_आकार > 0)
                {
                    f.पढ़ो(file_कुंजी.तथ्य(), entry.कुंजी_आकार);
                    CV_Assert(!f.हार());
                }
                अगर (0 == स_भेद(file_कुंजी.तथ्य(), कुंजी.c_str(), entry.कुंजी_आकार))
                {
                    // duplicate
                    CV_LOG_VERBOSE(शून्य, 0, "Duplicate key ignored: " << fileName_);
                    वापस false;
                }
            }
            अगर (entry.अग्र_Entry_File_Offset == 0)
                अवरोध;
            entryOffset = entry.अग्र_Entry_File_Offset;
        }
        seekReadAbsolute(0);
        अगर (entryOffset > 0)
        {
            seekWriteAbsolute(entryOffset);
            entry.अग्र_Entry_File_Offset = (uint32_t)file_आकार;
            f.ग_लिखो((अक्षर*)&entry, माप(entry));
            CV_Assert(!f.हार());
        }
        अन्यथा
        {
            entryOffsets[entry_अंक] = (uint32_t)file_आकार;
            seekWriteAbsolute(tableEntriesOffset);
            f.ग_लिखो((अक्षर*)entryOffsets, माप(entryOffsets));
            CV_Assert(!f.हार());
        }
        seekWriteAbsolute(file_आकार);
        entry.अग्र_Entry_File_Offset = 0;
        entry.तथ्य_आकार = (uint32_t)buf.size();
        entry.कुंजी_आकार = (uint32_t)कुंजी.size();
        f.ग_लिखो((अक्षर*)&entry, माप(entry));
        CV_Assert(!f.हार());
        f.ग_लिखो(कुंजी.c_str(), entry.कुंजी_आकार);
        CV_Assert(!f.हार());
        f.ग_लिखो(&buf[0], entry.तथ्य_आकार);
        CV_Assert(!f.हार());
        f.सफाई();
        CV_Assert(!f.हार());
        CV_LOG_VERBOSE(शून्य, 0, "Write... (" << buf.size() << " bytes)");
        वापस true;
    }
};
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT



काष्ठा OpenCLExecutionContext::Impl
{
    ocl::Context context_;
    पूर्णांक device_;  // device index in context
    ocl::Queue queue_;
    पूर्णांक useOpenCL_;

रक्षित:
    Impl() = मिटाओ;

    व्योम _init_device(cl_device_id deviceID)
    {
        CV_Assert(deviceID);
        पूर्णांक ndevices = (पूर्णांक)context_.ndevices();
        CV_Assert(ndevices > 0);
        bool found = false;
        क्रम (पूर्णांक i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            cl_device_id dhandle = (cl_device_id)d.ptr();
            अगर (dhandle == deviceID)
            {
                device_ = i;
                found = true;
                अवरोध;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

    व्योम _init_device(स्थिर ocl::Device& device)
    {
        CV_Assert(device.ptr());
        पूर्णांक ndevices = (पूर्णांक)context_.ndevices();
        CV_Assert(ndevices > 0);
        bool found = false;
        क्रम (पूर्णांक i = 0; i < ndevices; i++)
        {
            ocl::Device d = context_.device(i);
            अगर (d.getImpl() == device.getImpl())
            {
                device_ = i;
                found = true;
                अवरोध;
            }
        }
        CV_Assert(found && "OpenCL device can't work with passed OpenCL context");
    }

खुला:
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

    Impl(स्थिर ocl::Context& context, स्थिर ocl::Device& device, स्थिर ocl::Queue& queue)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = queue;
    }

    Impl(स्थिर ocl::Context& context, स्थिर ocl::Device& device)
        : device_(0), useOpenCL_(-1)
    {
        CV_Assert(context.ptr());
        CV_Assert(device.ptr());

        context_ = context;
        _init_device(device);
        queue_ = Queue(context_, context_.device(device_));
    }

    Impl(स्थिर ocl::Context& context, स्थिर पूर्णांक device, स्थिर ocl::Queue& queue)
        : context_(context)
        , device_(device)
        , queue_(queue)
        , useOpenCL_(-1)
    {
        // nothing
    }
    Impl(स्थिर Impl& other)
        : context_(other.context_)
        , device_(other.device_)
        , queue_(other.queue_)
        , useOpenCL_(-1)
    {
        // nothing
    }

    अंतरभूत bool useOpenCL() स्थिर { वापस const_cast<Impl*>(यह)->useOpenCL(); }
    bool useOpenCL()
    {
        अगर (useOpenCL_ < 0)
        {
            प्रयास
            {
                useOpenCL_ = 0;
                अगर (!context_.empty() && context_.ndevices() > 0)
                {
                    स्थिर Device& d = context_.device(device_);
                    useOpenCL_ = d.available();
                }
            }
            पकड़ो (स्थिर cv::Exception&)
            {
                // nothing
            }
            अगर (!useOpenCL_)
                CV_LOG_INFO(शून्य, "OpenCL: can't use OpenCL execution context");
        }
        वापस useOpenCL_ > 0;
    }

    व्योम setUseOpenCL(bool flag)
    {
        अगर (!flag)
            useOpenCL_ = 0;
        अन्यथा
            useOpenCL_ = -1;
    }

    अटल स्थिर std::shared_ptr<Impl>& getInitializedExecutionContext()
    {
        CV_TRACE_FUNCTION();

        CV_LOG_INFO(शून्य, "OpenCL: initializing thread execution context");

        अटल bool initialized = false;
        अटल std::shared_ptr<Impl> g_primaryExecutionContext;

        अगर (!initialized)
        {
            cv::AutoLock lock(getInitializationMutex());
            अगर (!initialized)
            {
                CV_LOG_INFO(शून्य, "OpenCL: creating new execution context...");
                प्रयास
                {
                    Context c = ocl::Context::create(std::string());
                    अगर (c.ndevices())
                    {
                        पूर्णांक deviceId = 0;
                        स्वतः& d = c.device(deviceId);
                        अगर (d.available())
                        {
                            स्वतः q = ocl::Queue(c, d);
                            अगर (!q.ptr())
                            {
                                CV_LOG_ERROR(शून्य, "OpenCL: Can't create default OpenCL queue");
                            }
                            अन्यथा
                            {
                                g_primaryExecutionContext = std::make_shared<Impl>(c, deviceId, q);
                                CV_LOG_INFO(शून्य, "OpenCL: device=" << d.name());
                            }
                        }
                        अन्यथा
                        {
                            CV_LOG_ERROR(शून्य, "OpenCL: OpenCL device is not available (CL_DEVICE_AVAILABLE returns false)");
                        }
                    }
                    अन्यथा
                    {
                        CV_LOG_INFO(शून्य, "OpenCL: context is not available/disabled");
                    }
                }
                पकड़ो (स्थिर std::exception& e)
                {
                    CV_LOG_INFO(शून्य, "OpenCL: Can't initialize OpenCL context/device/queue: " << e.what());
                }
                पकड़ो (...)
                {
                    CV_LOG_WARNING(शून्य, "OpenCL: Can't initialize OpenCL context/device/queue: unknown C++ exception");
                }
                initialized = true;
            }
        }
        वापस g_primaryExecutionContext;
    }
};

Context& OpenCLExecutionContext::getContext() स्थिर
{
    CV_Assert(p);
    वापस p->context_;
}
Device& OpenCLExecutionContext::getDevice() स्थिर
{
    CV_Assert(p);
    वापस p->context_.device(p->device_);
}
Queue& OpenCLExecutionContext::getQueue() स्थिर
{
    CV_Assert(p);
    वापस p->queue_;
}

bool OpenCLExecutionContext::useOpenCL() स्थिर
{
    अगर (p)
        वापस p->useOpenCL();
    वापस false;
}
व्योम OpenCLExecutionContext::setUseOpenCL(bool flag)
{
    CV_Assert(p);
    p->setUseOpenCL(flag);
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrent()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    OpenCLExecutionContext& c = तथ्य.oclExecutionContext;
    अगर (!तथ्य.oclExecutionContextInitialized)
    {
        तथ्य.oclExecutionContextInitialized = true;
        अगर (c.empty() && haveOpenCL())
            c.p = Impl::getInitializedExecutionContext();
    }
    वापस c;
}

/* static */
OpenCLExecutionContext& OpenCLExecutionContext::getCurrentRef()
{
    CV_TRACE_FUNCTION();
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    OpenCLExecutionContext& c = तथ्य.oclExecutionContext;
    वापस c;
}

व्योम OpenCLExecutionContext::bind() स्थिर
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    तथ्य.oclExecutionContext = *यह;
    तथ्य.oclExecutionContextInitialized = true;
    तथ्य.useOpenCL = p->useOpenCL_;  // propagate "-1", avoid call useOpenCL()
}


OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue() स्थिर
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    स्थिर Queue q(getContext(), getDevice());
    वापस cloneWithNewQueue(q);
}

OpenCLExecutionContext OpenCLExecutionContext::cloneWithNewQueue(स्थिर ocl::Queue& q) स्थिर
{
    CV_TRACE_FUNCTION();
    CV_Assert(p);
    CV_Assert(q.ptr() != शून्य);
    OpenCLExecutionContext c;
    c.p = std::make_shared<Impl>(p->context_, p->device_, q);
    वापस c;
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(स्थिर Context& context, स्थिर Device& device, स्थिर ocl::Queue& queue)
{
    CV_TRACE_FUNCTION();
    अगर (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device, queue);
    वापस ctx;

}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(स्थिर Context& context, स्थिर Device& device)
{
    CV_TRACE_FUNCTION();
    अगर (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    CV_Assert(!context.empty());
    CV_Assert(context.ptr());
    CV_Assert(!device.empty());
    CV_Assert(device.ptr());
    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>(context, device);
    वापस ctx;

}

व्योम OpenCLExecutionContext::release()
{
    CV_TRACE_FUNCTION();
    p.reset();
}



// true if we have initialized OpenCL subsystem with available platforms
अटल bool g_isOpenCLInitialized = false;
अटल bool g_isOpenCLAvailable = false;

bool haveOpenCL()
{
    CV_TRACE_FUNCTION();

    अगर (!g_isOpenCLInitialized)
    {
        CV_TRACE_REGION("Init_OpenCL_Runtime");
        std::string envPath = utils::getConfigurationParameterString("OPENCV_OPENCL_RUNTIME");
        अगर (!envPath.empty())
        {
            अगर (envPath == "disabled")
            {
                g_isOpenCLAvailable = false;
                g_isOpenCLInitialized = true;
                वापस false;
            }
        }

        cv::AutoLock lock(getInitializationMutex());
        CV_LOG_INFO(शून्य, "Initialize OpenCL runtime...");
        प्रयास
        {
            cl_uint n = 0;
            g_isOpenCLAvailable = ::clGetPlatformIDs(0, शून्य, &n) == CL_SUCCESS;
            g_isOpenCLAvailable &= n > 0;
            CV_LOG_INFO(शून्य, "OpenCL: found " << n << " platforms");
        }
        पकड़ो (...)
        {
            g_isOpenCLAvailable = false;
        }
        g_isOpenCLInitialized = true;
    }
    वापस g_isOpenCLAvailable;
}

bool useOpenCL()
{
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    अगर (तथ्य.useOpenCL < 0)
    {
        प्रयास
        {
            तथ्य.useOpenCL = 0;
            अगर (haveOpenCL())
            {
                स्वतः c = OpenCLExecutionContext::getCurrent();
                तथ्य.useOpenCL = c.useOpenCL();
            }
        }
        पकड़ो (...)
        {
            CV_LOG_INFO(शून्य, "OpenCL: can't initialize thread OpenCL execution context");
        }
    }
    वापस तथ्य.useOpenCL > 0;
}

bool isOpenCLActivated()
{
    अगर (!g_isOpenCLAvailable)
        वापस false; // prevent unnecessary OpenCL activation via useOpenCL()->haveOpenCL() calls
    वापस useOpenCL();
}

व्योम setUseOpenCL(bool flag)
{
    CV_TRACE_FUNCTION();

    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    स्वतः& c = OpenCLExecutionContext::getCurrentRef();
    अगर (!c.empty())
    {
        c.setUseOpenCL(flag);
        तथ्य.useOpenCL = c.useOpenCL();
    }
    अन्यथा
    {
        अगर (!flag)
            तथ्य.useOpenCL = 0;
        अन्यथा
            तथ्य.useOpenCL = -1; // enabled by default (if context is not initialized)
    }
}



#अगर_घोषित HAVE_CLAMDBLAS

श्रेणी AmdBlasHelper
{
खुला:
    अटल AmdBlasHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdBlasHelper, नया AmdBlasHelper())
    }

    bool isAvailable() स्थिर
    {
        वापस g_isAmdBlasAvailable;
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
        अगर (!g_isAmdBlasInitialized)
        {
            AutoLock lock(getInitializationMutex());

            अगर (!g_isAmdBlasInitialized)
            {
                अगर (haveOpenCL())
                {
                    प्रयास
                    {
                        g_isAmdBlasAvailable = clblasSetup() == clblasSuccess;
                    }
                    पकड़ो (...)
                    {
                        g_isAmdBlasAvailable = false;
                    }
                }
                अन्यथा
                    g_isAmdBlasAvailable = false;

                g_isAmdBlasInitialized = true;
            }
        }
    }

निजी:
    अटल bool g_isAmdBlasInitialized;
    अटल bool g_isAmdBlasAvailable;
};

bool AmdBlasHelper::g_isAmdBlasAvailable = false;
bool AmdBlasHelper::g_isAmdBlasInitialized = false;

bool haveAmdBlas()
{
    वापस AmdBlasHelper::getInstance().isAvailable();
}

#अन्यथा

bool haveAmdBlas()
{
    वापस false;
}

#पूर्ण_अगर

#अगर_घोषित HAVE_CLAMDFFT

श्रेणी AmdFftHelper
{
खुला:
    अटल AmdFftHelper & getInstance()
    {
        CV_SINGLETON_LAZY_INIT_REF(AmdFftHelper, नया AmdFftHelper())
    }

    bool isAvailable() स्थिर
    {
        वापस g_isAmdFftAvailable;
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
        अगर (!g_isAmdFftInitialized)
        {
            AutoLock lock(getInitializationMutex());

            अगर (!g_isAmdFftInitialized)
            {
                अगर (haveOpenCL())
                {
                    प्रयास
                    {
                        cl_uint major, minor, patch;
                        CV_Assert(clfft_Init_Setup_तथ्य(&setup_तथ्य) == CLFFT_SUCCESS);

                        // it throws exception in case AmdFft binaries are not found
                        CV_Assert(clfftGetVersion(&major, &minor, &patch) == CLFFT_SUCCESS);
                        g_isAmdFftAvailable = true;
                    }
                    पकड़ो (स्थिर Exception &)
                    {
                        g_isAmdFftAvailable = false;
                    }
                }
                अन्यथा
                    g_isAmdFftAvailable = false;

                g_isAmdFftInitialized = true;
            }
        }
    }

निजी:
    अटल clfft_Setup_तथ्य setup_तथ्य;
    अटल bool g_isAmdFftInitialized;
    अटल bool g_isAmdFftAvailable;
};

clfft_Setup_तथ्य AmdFftHelper::setup_तथ्य;
bool AmdFftHelper::g_isAmdFftAvailable = false;
bool AmdFftHelper::g_isAmdFftInitialized = false;

bool haveAmdFft()
{
    वापस AmdFftHelper::getInstance().isAvailable();
}

#अन्यथा

bool haveAmdFft()
{
    वापस false;
}

#पूर्ण_अगर

bool haveSVM()
{
#अगर_घोषित HAVE_OPENCL_SVM
    वापस true;
#अन्यथा
    वापस false;
#पूर्ण_अगर
}

व्योम finish()
{
    Queue::getDefault().finish();
}

/////////////////////////////////////////// Platform /////////////////////////////////////////////

काष्ठा Platform::Impl
{
    Impl()
    {
        refcount = 1;
        handle = 0;
        initialized = false;
    }

    ~Impl() {}

    व्योम init()
    {
        अगर( !initialized )
        {
            //cl_uint num_entries
            cl_uint n = 0;
            अगर( clGetPlatformIDs(1, &handle, &n) != CL_SUCCESS || n == 0 )
                handle = 0;
            अगर( handle != 0 )
            {
                अक्षर buf[1000];
                माप_प्रकार len = 0;
                CV_OCL_DBG_CHECK(clGetPlatformInfo(handle, CL_PLATFORM_VENDOR, माप(buf), buf, &len));
                buf[len] = '\0';
                vendor = String(buf);
            }

            initialized = true;
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_platform_id handle;
    String vendor;
    bool initialized;
};

Platform::Platform() CV_NOEXCEPT
{
    p = 0;
}

Platform::~Platform()
{
    अगर(p)
        p->release();
}

Platform::Platform(स्थिर Platform& pl)
{
    p = (Impl*)pl.p;
    अगर(p)
        p->addref();
}

Platform& Platform::चालक = (स्थिर Platform& pl)
{
    Impl* newp = (Impl*)pl.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Platform::Platform(Platform&& pl) CV_NOEXCEPT
{
    p = pl.p;
    pl.p = nullptr;
}

Platform& Platform::चालक = (Platform&& pl) CV_NOEXCEPT
{
    अगर (यह != &pl) {
        अगर(p)
            p->release();
        p = pl.p;
        pl.p = nullptr;
    }
    वापस *यह;
}

व्योम* Platform::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

Platform& Platform::getDefault()
{
    CV_LOG_ONCE_WARNING(शून्य, "OpenCL: Platform::getDefault() is deprecated and will be removed. Use cv::ocl::getPlatfomsInfo() for enumeration of available platforms");
    अटल Platform p;
    अगर( !p.p )
    {
        p.p = नया Impl;
        p.p->init();
    }
    वापस p;
}

/////////////////////////////////////// Device ////////////////////////////////////////////

// Version has format:
//   OpenCL<space><major_version.minor_version><space><vendor-specific information>
// by specification
//   http://www.khronos.org/registry/cl/sdk/1.1/docs/man/xhtml/clGetDeviceInfo.html
//   http://www.khronos.org/registry/cl/sdk/1.2/docs/man/xhtml/clGetDeviceInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/clGetPlatformInfo.html
//   https://www.khronos.org/registry/OpenCL/sdk/1.2/docs/man/xhtml/clGetPlatformInfo.html
अटल व्योम parseOpenCLVersion(स्थिर String &version, पूर्णांक &major, पूर्णांक &minor)
{
    major = minor = 0;
    अगर (10 >= version.length())
        वापस;
    स्थिर अक्षर *pstr = version.c_str();
    अगर (0 != म_भेदन(pstr, "OpenCL ", 7))
        वापस;
    माप_प्रकार ppos = version.find('.', 7);
    अगर (String::npos == ppos)
        वापस;
    String अस्थायी_2 = version.substr(7, ppos - 7);
    major = म_से_प(अस्थायी_2.c_str());
    अस्थायी_2 = version.substr(ppos + 1);
    minor = म_से_प(अस्थायी_2.c_str());
}

काष्ठा Device::Impl
{
    Impl(व्योम* d)
        : refcount(1)
        , handle(0)
    {
        प्रयास
        {
            cl_device_id device = (cl_device_id)d;
            _init(device);
            CV_OCL_CHECK(clRetainDevice(device));  // increment reference counter on success only
        }
        पकड़ो (...)
        {
            फेंको;
        }
    }

    व्योम _init(cl_device_id d)
    {
        handle = (cl_device_id)d;

        name_ = getStrProp(CL_DEVICE_NAME);
        version_ = getStrProp(CL_DEVICE_VERSION);
        extensions_ = getStrProp(CL_DEVICE_EXTENSIONS);
        doubleFPConfig_ = getProp<cl_device_fp_config, पूर्णांक>(CL_DEVICE_DOUBLE_FP_CONFIG);
        halfFPConfig_ = getProp<cl_device_fp_config, पूर्णांक>(CL_DEVICE_HALF_FP_CONFIG);
        hostUnifiedMemory_ = getBoolProp(CL_DEVICE_HOST_UNIFIED_MEMORY);
        maxComputeUnits_ = getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_COMPUTE_UNITS);
        max_Work_Group_आकार_2 = getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_MAX_WORK_GROUP_आकार);
        type_ = getProp<cl_device_type, पूर्णांक>(CL_DEVICE_TYPE);
        driverVersion_ = getStrProp(CL_DRIVER_VERSION);
        addressBits_ = getProp<cl_uint, पूर्णांक>(CL_DEVICE_ADDRESS_BITS);

        String deviceVersion_ = getStrProp(CL_DEVICE_VERSION);
        parseOpenCLVersion(deviceVersion_, deviceVersionMajor_, deviceVersionMinor_);

        माप_प्रकार pos = 0;
        जबतक (pos < extensions_.size())
        {
            माप_प्रकार pos2 = extensions_.find(' ', pos);
            अगर (pos2 == String::npos)
                pos2 = extensions_.size();
            अगर (pos2 > pos)
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
        अगर (vendorName_ == "Advanced Micro Devices, Inc." ||
            vendorName_ == "AMD")
            vendorID_ = VENDOR_AMD;
        अन्यथा अगर (vendorName_ == "Intel(R) Corporation" || vendorName_ == "Intel" || vendorName_ == "Intel Inc." || म_माला(name_.c_str(), "Iris") != 0)
            vendorID_ = VENDOR_INTEL;
        अन्यथा अगर (vendorName_ == "NVIDIA Corporation")
            vendorID_ = VENDOR_NVIDIA;
        अन्यथा
            vendorID_ = UNKNOWN_VENDOR;

        स्थिर माप_प्रकार CV_OPENCL_DEVICE_MAX_WORK_GROUP_आकार = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_DEVICE_MAX_WORK_GROUP_SIZE", 0);
        अगर (CV_OPENCL_DEVICE_MAX_WORK_GROUP_आकार > 0)
        {
            स्थिर माप_प्रकार new_max_Work_Group_आकार = std::min(max_Work_Group_आकार_2, CV_OPENCL_DEVICE_MAX_WORK_GROUP_आकार);
            अगर (new_max_Work_Group_आकार != max_Work_Group_आकार_2)
                CV_LOG_WARNING(शून्य, "OpenCL: using workgroup size: " << new_max_Work_Group_आकार << " (was " << max_Work_Group_आकार_2 << ")");
            max_Work_Group_आकार_2 = new_max_Work_Group_आकार;
        }
#अगर 0
        अगर (isExtensionSupported("cl_khr_spir"))
        {
#अगर_अघोषित CL_DEVICE_SPIR_VERSIONS
#घोषणा CL_DEVICE_SPIR_VERSIONS                     0x40E0
#पूर्ण_अगर
            cv::String spir_versions = getStrProp(CL_DEVICE_SPIR_VERSIONS);
            std::अ_बाहर << spir_versions << std::प_पूर्ण;
        }
#पूर्ण_अगर
    }

    ~Impl()
    {
#अगर_घोषित _WIN32
        अगर (!cv::__termination)
#पूर्ण_अगर
        {
            अगर (handle)
            {
                CV_OCL_CHECK(clReleaseDevice(handle));
                handle = 0;
            }
        }
    }

    ढाँचा<typename _TpCL, typename _TpOut>
    _TpOut getProp(cl_device_info prop) स्थिर
    {
        _TpCL अस्थायी_2=_TpCL();
        माप_प्रकार sz = 0;

        वापस clGetDeviceInfo(handle, prop, माप(अस्थायी_2), &अस्थायी_2, &sz) == CL_SUCCESS &&
            sz == माप(अस्थायी_2) ? _TpOut(अस्थायी_2) : _TpOut();
    }

    bool getBoolProp(cl_device_info prop) स्थिर
    {
        cl_bool अस्थायी_2 = CL_FALSE;
        माप_प्रकार sz = 0;

        वापस clGetDeviceInfo(handle, prop, माप(अस्थायी_2), &अस्थायी_2, &sz) == CL_SUCCESS &&
            sz == माप(अस्थायी_2) ? अस्थायी_2 != 0 : false;
    }

    String getStrProp(cl_device_info prop) स्थिर
    {
        अक्षर buf[4096];
        माप_प्रकार sz=0;
        वापस clGetDeviceInfo(handle, prop, माप(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < माप(buf) ? String(buf) : String();
    }

    bool isExtensionSupported(स्थिर std::string& extensionName) स्थिर
    {
        वापस extensions_set_.गिनती_2(extensionName) > 0;
    }


    IMPLEMENT_REFCOUNTABLE();

    cl_device_id handle;

    String name_;
    String version_;
    std::string extensions_;
    पूर्णांक doubleFPConfig_;
    bool khr_fp64_support_;
    पूर्णांक halfFPConfig_;
    bool khr_fp16_support_;
    bool hostUnifiedMemory_;
    पूर्णांक maxComputeUnits_;
    माप_प्रकार max_Work_Group_आकार_2;
    पूर्णांक type_;
    पूर्णांक addressBits_;
    पूर्णांक deviceVersionMajor_;
    पूर्णांक deviceVersionMinor_;
    String driverVersion_;
    String vendorName_;
    पूर्णांक vendorID_;
    bool intelSubgroupsSupport_;

    std::set<std::string> extensions_set_;
};


Device::Device() CV_NOEXCEPT
{
    p = 0;
}

Device::Device(व्योम* d)
{
    p = 0;
    set(d);
}

Device::Device(स्थिर Device& d)
{
    p = d.p;
    अगर(p)
        p->addref();
}

Device& Device::चालक = (स्थिर Device& d)
{
    Impl* newp = (Impl*)d.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Device::Device(Device&& d) CV_NOEXCEPT
{
    p = d.p;
    d.p = nullptr;
}

Device& Device::चालक = (Device&& d) CV_NOEXCEPT
{
    अगर (यह != &d) {
        अगर(p)
            p->release();
        p = d.p;
        d.p = nullptr;
    }
    वापस *यह;
}

Device::~Device()
{
    अगर(p)
        p->release();
}

व्योम Device::set(व्योम* d)
{
    अगर(p)
        p->release();
    p = नया Impl(d);
    अगर (p->handle)
    {
        CV_OCL_CHECK(clReleaseDevice((cl_device_id)d));
    }
}

Device Device::fromHandle(व्योम* d)
{
    Device device(d);
    वापस device;
}

व्योम* Device::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

String Device::name() स्थिर
{ वापस p ? p->name_ : String(); }

String Device::extensions() स्थिर
{ वापस p ? String(p->extensions_) : String(); }

bool Device::isExtensionSupported(स्थिर String& extensionName) स्थिर
{ वापस p ? p->isExtensionSupported(extensionName) : false; }

String Device::version() स्थिर
{ वापस p ? p->version_ : String(); }

String Device::vendorName() स्थिर
{ वापस p ? p->vendorName_ : String(); }

पूर्णांक Device::vendorID() स्थिर
{ वापस p ? p->vendorID_ : 0; }

String Device::OpenCL_C_Version() स्थिर
{ वापस p ? p->getStrProp(CL_DEVICE_OPENCL_C_VERSION) : String(); }

String Device::OpenCLVersion() स्थिर
{ वापस p ? p->getStrProp(CL_DEVICE_VERSION) : String(); }

पूर्णांक Device::deviceVersionMajor() स्थिर
{ वापस p ? p->deviceVersionMajor_ : 0; }

पूर्णांक Device::deviceVersionMinor() स्थिर
{ वापस p ? p->deviceVersionMinor_ : 0; }

String Device::driverVersion() स्थिर
{ वापस p ? p->driverVersion_ : String(); }

पूर्णांक Device::type() स्थिर
{ वापस p ? p->type_ : 0; }

पूर्णांक Device::addressBits() स्थिर
{ वापस p ? p->addressBits_ : 0; }

bool Device::available() स्थिर
{ वापस p ? p->getBoolProp(CL_DEVICE_AVAILABLE) : false; }

bool Device::compilerAvailable() स्थिर
{ वापस p ? p->getBoolProp(CL_DEVICE_COMPILER_AVAILABLE) : false; }

bool Device::linkerAvailable() स्थिर
#अगर_घोषित CL_VERSION_1_2
{ वापस p ? p->getBoolProp(CL_DEVICE_LINKER_AVAILABLE) : false; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#पूर्ण_अगर

पूर्णांक Device::doubleFPConfig() स्थिर
{ वापस p ? p->doubleFPConfig_ : 0; }

पूर्णांक Device::singleFPConfig() स्थिर
{ वापस p ? p->getProp<cl_device_fp_config, पूर्णांक>(CL_DEVICE_SINGLE_FP_CONFIG) : 0; }

पूर्णांक Device::halfFPConfig() स्थिर
{ वापस p ? p->halfFPConfig_ : 0; }

bool Device::hasFP64() स्थिर
{ वापस p ? p->khr_fp64_support_ : false; }
bool Device::hasFP16() स्थिर
{ वापस p ? p->khr_fp16_support_ : false; }

bool Device::endianLittle() स्थिर
{ वापस p ? p->getBoolProp(CL_DEVICE_ENDIAN_LITTLE) : false; }

bool Device::errorCorrectionSupport() स्थिर
{ वापस p ? p->getBoolProp(CL_DEVICE_ERROR_CORRECTION_SUPPORT) : false; }

पूर्णांक Device::executionCapabilities() स्थिर
{ वापस p ? p->getProp<cl_device_exec_capabilities, पूर्णांक>(CL_DEVICE_EXECUTION_CAPABILITIES) : 0; }

माप_प्रकार Device::global_Mem_Cache_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_GLOBAL_MEM_CACHE_आकार) : 0; }

पूर्णांक Device::globalMemCacheType() स्थिर
{ वापस p ? p->getProp<cl_device_mem_cache_type, पूर्णांक>(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE) : 0; }

पूर्णांक Device::global_Mem_Cache_पंक्ति_आकार() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_GLOBAL_MEM_CACHELINE_आकार) : 0; }

माप_प्रकार Device::global_Mem_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_GLOBAL_MEM_आकार) : 0; }

माप_प्रकार Device::local_Mem_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_LOCAL_MEM_आकार) : 0; }

पूर्णांक Device::localMemType() स्थिर
{ वापस p ? p->getProp<cl_device_local_mem_type, पूर्णांक>(CL_DEVICE_LOCAL_MEM_TYPE) : 0; }

bool Device::hostUnifiedMemory() स्थिर
{ वापस p ? p->hostUnifiedMemory_ : false; }

bool Device::imageSupport() स्थिर
{ वापस p ? p->getBoolProp(CL_DEVICE_IMAGE_SUPPORT) : false; }

bool Device::image_From_बफर_Support() स्थिर
{
    वापस p ? p->isExtensionSupported("cl_khr_image2d_from_buffer") : false;
}

uint Device::imagePitchAlignment() स्थिर
{
#अगर_घोषित CL_DEVICE_IMAGE_PITCH_ALIGNMENT
    वापस p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_PITCH_ALIGNMENT) : 0;
#अन्यथा
    वापस 0;
#पूर्ण_अगर
}

uint Device::imageBaseAddressAlignment() स्थिर
{
#अगर_घोषित CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT
    वापस p ? p->getProp<cl_uint, uint>(CL_DEVICE_IMAGE_BASE_ADDRESS_ALIGNMENT) : 0;
#अन्यथा
    वापस 0;
#पूर्ण_अगर
}

माप_प्रकार Device::image2DMaxWidth() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE2D_MAX_WIDTH) : 0; }

माप_प्रकार Device::image2DMaxHeight() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE2D_MAX_HEIGHT) : 0; }

माप_प्रकार Device::image3DMaxWidth() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE3D_MAX_WIDTH) : 0; }

माप_प्रकार Device::image3DMaxHeight() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE3D_MAX_HEIGHT) : 0; }

माप_प्रकार Device::image3DMaxDepth() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE3D_MAX_DEPTH) : 0; }

माप_प्रकार Device::image_Max_बफर_आकार() स्थिर
#अगर_घोषित CL_VERSION_1_2
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE_MAX_बफर_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#पूर्ण_अगर

माप_प्रकार Device::image_Max_Array_आकार() स्थिर
#अगर_घोषित CL_VERSION_1_2
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_IMAGE_MAX_ARRAY_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#पूर्ण_अगर

bool Device::intelSubgroupsSupport() स्थिर
{ वापस p ? p->intelSubgroupsSupport_ : false; }

पूर्णांक Device::maxClockFrequency() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_CLOCK_FREQUENCY) : 0; }

पूर्णांक Device::maxComputeUnits() स्थिर
{ वापस p ? p->maxComputeUnits_ : 0; }

पूर्णांक Device::maxConstantArgs() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_CONSTANT_ARGS) : 0; }

माप_प्रकार Device::max_Constant_बफर_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_MAX_CONSTANT_बफर_आकार) : 0; }

माप_प्रकार Device::max_Mem_Alloc_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_MAX_MEM_ALLOC_आकार) : 0; }

माप_प्रकार Device::max_Parameter_आकार() स्थिर
{ वापस p ? p->getProp<cl_ulong, माप_प्रकार>(CL_DEVICE_MAX_PARAMETER_आकार) : 0; }

पूर्णांक Device::maxReadImageArgs() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_READ_IMAGE_ARGS) : 0; }

पूर्णांक Device::maxWriteImageArgs() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_WRITE_IMAGE_ARGS) : 0; }

पूर्णांक Device::maxSamplers() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_SAMPLERS) : 0; }

माप_प्रकार Device::max_Work_Group_आकार() स्थिर
{ वापस p ? p->max_Work_Group_आकार_2 : 0; }

पूर्णांक Device::maxWorkItemDims() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS) : 0; }

व्योम Device::maxWorkItemSizes(माप_प्रकार* sizes) स्थिर
{
    अगर(p)
    {
        स्थिर पूर्णांक MAX_DIMS = 32;
        माप_प्रकार retsz = 0;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(p->handle, CL_DEVICE_MAX_WORK_ITEM_SIZES,
                MAX_DIMS*माप(sizes[0]), &sizes[0], &retsz));
    }
}

पूर्णांक Device::memBaseAddrAlign() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_MEM_BASE_ADDR_ALIGN) : 0; }

पूर्णांक Device::nativeVectorWidthChar() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_CHAR) : 0; }

पूर्णांक Device::nativeVectorWidthShort() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_SHORT) : 0; }

पूर्णांक Device::nativeVectorWidthInt() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_INT) : 0; }

पूर्णांक Device::nativeVectorWidthLong() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_LONG) : 0; }

पूर्णांक Device::nativeVectorWidthFloat() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_FLOAT) : 0; }

पूर्णांक Device::nativeVectorWidthDouble() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_DOUBLE) : 0; }

पूर्णांक Device::nativeVectorWidthHalf() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_NATIVE_VECTOR_WIDTH_HALF) : 0; }

पूर्णांक Device::preferredVectorWidthChar() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR) : 0; }

पूर्णांक Device::preferredVectorWidthShort() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT) : 0; }

पूर्णांक Device::preferredVectorWidthInt() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT) : 0; }

पूर्णांक Device::preferredVectorWidthLong() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG) : 0; }

पूर्णांक Device::preferredVectorWidthFloat() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT) : 0; }

पूर्णांक Device::preferredVectorWidthDouble() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE) : 0; }

पूर्णांक Device::preferredVectorWidthHalf() स्थिर
{ वापस p ? p->getProp<cl_uint, पूर्णांक>(CL_DEVICE_PREFERRED_VECTOR_WIDTH_HALF) : 0; }

माप_प्रकार Device::printf_बफर_आकार() स्थिर
#अगर_घोषित CL_VERSION_1_2
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_PRINTF_बफर_आकार) : 0; }
#अन्यथा
{ CV_REQUIRE_OPENCL_1_2_ERROR; }
#पूर्ण_अगर


माप_प्रकार Device::profilingTimerResolution() स्थिर
{ वापस p ? p->getProp<माप_प्रकार, माप_प्रकार>(CL_DEVICE_PROFILING_TIMER_RESOLUTION) : 0; }

स्थिर Device& Device::getDefault()
{
    स्वतः& c = OpenCLExecutionContext::getCurrent();
    अगर (!c.empty())
    {
        वापस c.getDevice();
    }

    अटल Device dummy;
    वापस dummy;
}

////////////////////////////////////// Context ///////////////////////////////////////////////////

ढाँचा <typename Functor, typename ObjectType>
अंतरभूत cl_int getStringInfo(Functor f, ObjectType obj, cl_uint name, std::string& param)
{
    ::माप_प्रकार required;
    cl_int err = f(obj, name, 0, शून्य, &required);
    अगर (err != CL_SUCCESS)
        वापस err;

    param.सफा();
    अगर (required > 0)
    {
        Auto_बफर<अक्षर> buf(required + 1);
        अक्षर* ptr = buf.तथ्य(); // cleanup is not needed
        err = f(obj, name, required, ptr, शून्य);
        अगर (err != CL_SUCCESS)
            वापस err;
        param = ptr;
    }

    वापस CL_SUCCESS;
}

अटल व्योम split(स्थिर std::string &s, अक्षर delim, std::vector<std::string> &elems)
{
    elems.सफा();
    अगर (s.size() == 0)
        वापस;
    std::istringstream ss(s);
    std::string item;
    जबतक (!ss.खप())
    {
        std::लो_पंक्ति(ss, item, delim);
        elems.push_back(item);
    }
}

// Layout: <Platform>:<CPU|GPU|ACCELERATOR|nothing=GPU/CPU>:<deviceName>
// Sample: AMD:GPU:
// Sample: AMD:GPU:Tahiti
// Sample: :GPU|CPU: = '' = ':' = '::'
अटल bool parseOpenCLDeviceConfiguration(स्थिर std::string& configurationStr,
        std::string& platform, std::vector<std::string>& deviceTypes, std::string& deviceNameOrID)
{
    std::vector<std::string> parts;
    split(configurationStr, ':', parts);
    अगर (parts.size() > 3)
    {
        CV_LOG_ERROR(शून्य, "OpenCL: Invalid configuration string for OpenCL device: " << configurationStr);
        वापस false;
    }
    अगर (parts.size() > 2)
        deviceNameOrID = parts[2];
    अगर (parts.size() > 1)
    {
        split(parts[1], '|', deviceTypes);
    }
    अगर (parts.size() > 0)
    {
        platform = parts[0];
    }
    वापस true;
}

अटल cl_device_id selectOpenCLDevice(स्थिर std::string & configuration_ = std::string())
{
    std::string platform, deviceName;
    std::vector<std::string> deviceTypes;

    std::string configuration(configuration_);
    अगर (configuration.empty())
        configuration = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");

    अगर (!configuration.empty() &&
            (configuration == "disabled" ||
             !parseOpenCLDeviceConfiguration(configuration, platform, deviceTypes, deviceName)
            ))
        वापस शून्य;

    bool isID = false;
    पूर्णांक deviceID = -1;
    अगर (deviceName.length() == 1)
    // We limit ID range to 0..9, because we want to write:
    // - '2500' to mean i5-2500
    // - '8350' to mean AMD FX-8350
    // - '650' to mean GeForce 650
    // To extend ID range change condition to '> 0'
    {
        isID = true;
        क्रम (माप_प्रकार i = 0; i < deviceName.length(); i++)
        {
            अगर (!है_अंक(deviceName[i]))
            {
                isID = false;
                अवरोध;
            }
        }
        अगर (isID)
        {
            deviceID = म_से_प(deviceName.c_str());
            अगर (deviceID < 0)
                वापस शून्य;
        }
    }

    std::vector<cl_platform_id> platforms;
    {
        cl_uint अंक_Platforms = 0;
        CV_OCL_DBG_CHECK(clGetPlatformIDs(0, शून्य, &अंक_Platforms));

        अगर (अंक_Platforms == 0)
            वापस शून्य;
        platforms.resize((माप_प्रकार)अंक_Platforms);
        CV_OCL_DBG_CHECK(clGetPlatformIDs(अंक_Platforms, &platforms[0], &अंक_Platforms));
        platforms.resize(अंक_Platforms);
    }

    अगर (platform.length() > 0)
    {
        क्रम (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end();)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetPlatformInfo, *currentPlatform, CL_PLATFORM_NAME, name));
            अगर (name.find(platform) != std::string::npos)
            {
                ++currentPlatform;
            }
            अन्यथा
            {
                currentPlatform = platforms.erase(currentPlatform);
            }
        }
        अगर (platforms.size() == 0)
        {
            CV_LOG_ERROR(शून्य, "OpenCL: Can't find OpenCL platform by name: " << platform);
            जाओ not_found;
        }
    }
    अगर (deviceTypes.size() == 0)
    {
        अगर (!isID)
        {
            deviceTypes.push_back("GPU");
            अगर (!configuration.empty())
                deviceTypes.push_back("CPU");
        }
        अन्यथा
            deviceTypes.push_back("ALL");
    }
    क्रम (माप_प्रकार t = 0; t < deviceTypes.size(); t++)
    {
        पूर्णांक deviceType = 0;
        std::string अस्थायी_Str_Device_Type = deviceTypes[t];
        std::transform(अस्थायी_Str_Device_Type.begin(), अस्थायी_Str_Device_Type.end(), अस्थायी_Str_Device_Type.begin(), details::char_tolower);

        अगर (अस्थायी_Str_Device_Type == "gpu" || अस्थायी_Str_Device_Type == "dgpu" || अस्थायी_Str_Device_Type == "igpu")
            deviceType = Device::TYPE_GPU;
        अन्यथा अगर (अस्थायी_Str_Device_Type == "cpu")
            deviceType = Device::TYPE_CPU;
        अन्यथा अगर (अस्थायी_Str_Device_Type == "accelerator")
            deviceType = Device::TYPE_ACCELERATOR;
        अन्यथा अगर (अस्थायी_Str_Device_Type == "all")
            deviceType = Device::TYPE_ALL;
        अन्यथा
        {
            CV_LOG_ERROR(शून्य, "OpenCL: Unsupported device type for OpenCL device (GPU, CPU, ACCELERATOR): " << deviceTypes[t]);
            जाओ not_found;
        }

        std::vector<cl_device_id> devices;
        क्रम (std::vector<cl_platform_id>::iterator currentPlatform = platforms.begin(); currentPlatform != platforms.end(); ++currentPlatform)
        {
            cl_uint गिनती_2 = 0;
            cl_int status = clGetDeviceIDs(*currentPlatform, deviceType, 0, शून्य, &गिनती_2);
            अगर (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_परिणाम(status, "clGetDeviceIDs get count");
            }
            अगर (गिनती_2 == 0)
                जारी;
            माप_प्रकार base = devices.size();
            devices.resize(base + गिनती_2);
            status = clGetDeviceIDs(*currentPlatform, deviceType, गिनती_2, &devices[base], &गिनती_2);
            अगर (!(status == CL_SUCCESS || status == CL_DEVICE_NOT_FOUND))
            {
                CV_OCL_DBG_CHECK_परिणाम(status, "clGetDeviceIDs get IDs");
            }
        }

        क्रम (माप_प्रकार i = (isID ? deviceID : 0);
             (isID ? (i == (माप_प्रकार)deviceID) : true) && (i < devices.size());
             i++)
        {
            std::string name;
            CV_OCL_DBG_CHECK(getStringInfo(clGetDeviceInfo, devices[i], CL_DEVICE_NAME, name));
            cl_bool useGPU = true;
            अगर(अस्थायी_Str_Device_Type == "dgpu" || अस्थायी_Str_Device_Type == "igpu")
            {
                cl_bool isIGPU = CL_FALSE;
                CV_OCL_DBG_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_HOST_UNIFIED_MEMORY, माप(isIGPU), &isIGPU, शून्य));
                useGPU = अस्थायी_Str_Device_Type == "dgpu" ? !isIGPU : isIGPU;
            }
            अगर ( (isID || name.find(deviceName) != std::string::npos) && useGPU)
            {
                // TODO check for OpenCL 1.1
                वापस devices[i];
            }
        }
    }

not_found:
    अगर (configuration.empty())
        वापस शून्य; // suppress messages on stderr

    std::ostringstream संदेश;
    संदेश << "ERROR: Requested OpenCL device not found, check configuration: '" << configuration << "'" << std::प_पूर्ण
        << "    Platform: " << (platform.length() == 0 ? "any" : platform) << std::प_पूर्ण
        << "    Device types:";
    क्रम (माप_प्रकार t = 0; t < deviceTypes.size(); t++)
        संदेश << ' ' << deviceTypes[t];

    संदेश << std::प_पूर्ण << "    Device name: " << (deviceName.length() == 0 ? "any" : deviceName);

    CV_LOG_ERROR(शून्य, संदेश.str());
    वापस शून्य;
}

#अगर_घोषित HAVE_OPENCL_SVM
namespace svm {

क्रमागत AllocatorFlags { // don't use first 16 bits
        OPENCL_SVM_COARSE_GRAIN_बफर = 1 << 16, // clSVMAlloc + SVM map/unmap
        OPENCL_SVM_FINE_GRAIN_बफर = 2 << 16, // clSVMAlloc
        OPENCL_SVM_FINE_GRAIN_SYSTEM = 3 << 16, // direct access
        OPENCL_SVM_बफर_MASK = 3 << 16,
        OPENCL_SVM_बफर_MAP = 4 << 16
};

अटल bool checkForceSVMUmatUsage()
{
    अटल bool initialized = false;
    अटल bool force = false;
    अगर (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_FORCE_UMAT_USAGE", false);
        initialized = true;
    }
    वापस force;
}
अटल bool checkDisableSVMUMatUsage()
{
    अटल bool initialized = false;
    अटल bool force = false;
    अगर (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE_UMAT_USAGE", false);
        initialized = true;
    }
    वापस force;
}
अटल bool checkDisableSVM()
{
    अटल bool initialized = false;
    अटल bool force = false;
    अगर (!initialized)
    {
        force = utils::getConfigurationParameterBool("OPENCV_OPENCL_SVM_DISABLE", false);
        initialized = true;
    }
    वापस force;
}
// see SVMCapabilities
अटल अचिन्हित पूर्णांक getSVMCapabilitiesMask()
{
    अटल bool initialized = false;
    अटल अचिन्हित पूर्णांक mask = 0;
    अगर (!initialized)
    {
        स्थिर std::string env_मूल्य = utils::getConfigurationParameterString("OPENCV_OPENCL_SVM_CAPABILITIES_MASK");
        अगर (env_मूल्य.empty())
        {
            वापस ~0U; // all bits 1
        }
        mask = म_से_प(env_मूल्य.c_str());
        initialized = true;
    }
    वापस mask;
}
} // namespace
#पूर्ण_अगर

अटल माप_प्रकार get_Program_गिनती_Limit()
{
    अटल bool initialized = false;
    अटल माप_प्रकार गिनती_2 = 0;
    अगर (!initialized)
    {
        गिनती_2 = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_PROGRAM_CACHE", 0);
        initialized = true;
    }
    वापस गिनती_2;
}

अटल पूर्णांक g_contextId = 0;

श्रेणी OpenCLBufferPoolImpl;
श्रेणी OpenCLSVMBufferPoolImpl;

काष्ठा Context::Impl
{
    अटल Context::Impl* लो(Context& context) { वापस context.getImpl(); }

    प्रकार std::deque<Context::Impl*> container_t;
    अटल container_t& getGlobalContainer()
    {
        // never delete this container (Impl lifetime is greater due to TLS storage)
        अटल container_t* g_contexts = नया container_t();
        वापस *g_contexts;
    }

रक्षित:
    Impl(स्थिर std::string& configuration_)
        : refcount(1)
        , contextId(CV_XADD(&g_contextId, 1))
        , configuration(configuration_)
        , handle(0)
#अगर_घोषित HAVE_OPENCL_SVM
        , svmInitialized(false)
#पूर्ण_अगर
    {
        अगर (!haveOpenCL())
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

        cv::AutoLock lock(cv::getInitializationMutex());
        स्वतः& container = getGlobalContainer();
        container.resize(std::max(container.size(), (माप_प्रकार)contextId + 1));
        container[contextId] = यह;
    }

    ~Impl()
    {
#अगर_घोषित _WIN32
        अगर (!cv::__termination)
#पूर्ण_अगर
        {
            अगर (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseContext(handle));
                handle = शून्य;
            }
            devices.सफा();
        }

        userContextStorage.सफा();

        {
            cv::AutoLock lock(cv::getInitializationMutex());
            स्वतः& container = getGlobalContainer();
            CV_CheckLT((माप_प्रकार)contextId, container.size(), "");
            container[contextId] = शून्य;
        }
    }

    व्योम init_device_list()
    {
        CV_Assert(handle);

        cl_uint ndevices = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_अंक_DEVICES, माप(ndevices), &ndevices, शून्य));
        CV_Assert(ndevices > 0);

        cv::Auto_बफर<cl_device_id> cl_devices(ndevices);
        माप_प्रकार devices_ret_आकार = 0;
        CV_OCL_CHECK(clGetContextInfo(handle, CL_CONTEXT_DEVICES, cl_devices.size() * माप(cl_device_id), &cl_devices[0], &devices_ret_आकार));
        CV_CheckEQ(devices_ret_आकार, cl_devices.size() * माप(cl_device_id), "");

        devices.सफा();
        क्रम (अचिन्हित i = 0; i < ndevices; i++)
        {
            devices.emplace_back(Device::fromHandle(cl_devices[i]));
        }
    }

    व्योम __init_buffer_pools();  // w/o synchronization
    व्योम init_बफर_pools() स्थिर
    {
        अगर (!बफर_Pool_2)
        {
            cv::AutoLock lock(cv::getInitializationMutex());
            अगर (!बफर_Pool_2)
            {
                const_cast<Impl*>(यह)->__init_buffer_pools();
            }
        }
    }
खुला:
    अटल Impl* findContext(स्थिर std::string& configuration)
    {
        CV_TRACE_FUNCTION();
        cv::AutoLock lock(cv::getInitializationMutex());
        स्वतः& container = getGlobalContainer();
        अगर (configuration.empty() && !container.empty())
            वापस container[0];
        क्रम (स्वतः it = container.begin(); it != container.end(); ++it)
        {
            Impl* i = *it;
            अगर (i && i->configuration == configuration)
            {
                वापस i;
            }
        }
        वापस शून्य;
    }

    अटल Impl* findOrCreateContext(स्थिर std::string& configuration_)
    {
        CV_TRACE_FUNCTION();
        std::string configuration = configuration_;
        अगर (configuration_.empty())
        {
            स्थिर std::string c = utils::getConfigurationParameterString("OPENCV_OPENCL_DEVICE");
            अगर (!c.empty())
                configuration = c;
        }
        Impl* impl = findContext(configuration);
        अगर (impl)
        {
            CV_LOG_INFO(शून्य, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            वापस impl;
        }

        cl_device_id d = selectOpenCLDevice(configuration);
        अगर (d == शून्य)
            वापस शून्य;

        impl = नया Impl(configuration);
        प्रयास
        {
            impl->createFromDevice(d);
            अगर (impl->handle)
                वापस impl;
            मिटाओ impl;
            वापस शून्य;
        }
        पकड़ो (...)
        {
            मिटाओ impl;
            फेंको;
        }
    }

    अटल Impl* findOrCreateContext(cl_context h)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(h);

        std::string configuration = cv::format("@ctx-%p", (व्योम*)h);
        Impl* impl = findContext(configuration);
        अगर (impl)
        {
            CV_LOG_INFO(शून्य, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            वापस impl;
        }

        impl = नया Impl(configuration);
        प्रयास
        {
            CV_OCL_CHECK(clRetainContext(h));
            impl->handle = h;
            impl->init_device_list();
            वापस impl;
        }
        पकड़ो (...)
        {
            मिटाओ impl;
            फेंको;
        }
    }

    अटल Impl* findOrCreateContext(स्थिर ocl::Device& device)
    {
        CV_TRACE_FUNCTION();

        CV_Assert(!device.empty());
        cl_device_id d = (cl_device_id)device.ptr();
        CV_Assert(d);

        std::string configuration = cv::format("@dev-%p", (व्योम*)d);
        Impl* impl = findContext(configuration);
        अगर (impl)
        {
            CV_LOG_INFO(शून्य, "OpenCL: reuse context@" << impl->contextId << " for configuration: " << configuration)
            impl->addref();
            वापस impl;
        }

        impl = नया Impl(configuration);
        प्रयास
        {
            impl->createFromDevice(d);
            CV_Assert(impl->handle);
            वापस impl;
        }
        पकड़ो (...)
        {
            मिटाओ impl;
            फेंको;
        }
    }

    व्योम setDefault()
    {
        CV_TRACE_FUNCTION();
        cl_device_id d = selectOpenCLDevice();

        अगर (d == शून्य)
            वापस;

        createFromDevice(d);
    }

    व्योम createFromDevice(cl_device_id d)
    {
        CV_TRACE_FUNCTION();
        CV_Assert(handle == शून्य);

        cl_platform_id pl = शून्य;
        CV_OCL_DBG_CHECK(clGetDeviceInfo(d, CL_DEVICE_PLATFORM, माप(cl_platform_id), &pl, शून्य));

        cl_context_properties prop[] =
        {
            CL_CONTEXT_PLATFORM, (cl_context_properties)pl,
            0
        };

        // !!! in the current implementation force the number of devices to 1 !!!
        cl_uint nd = 1;
        cl_int status;

        handle = clCreateContext(prop, nd, &d, 0, 0, &status);
        CV_OCL_DBG_CHECK_परिणाम(status, "clCreateContext");

        bool ok = handle != 0 && status == CL_SUCCESS;
        अगर( ok )
        {
            devices.resize(nd);
            devices[0].set(d);
        }
        अन्यथा
            handle = शून्य;
    }

    Program getProg(स्थिर ProgramSource& src, स्थिर String& buildflags, String& errmsg);

    व्योम unloadProg(Program& prog)
    {
        cv::AutoLock lock(program_cache_mutex);
        क्रम (CacheList::iterator i = cacheList.begin(); i != cacheList.end(); ++i)
        {
              phash_t::iterator it = phash.find(*i);
              अगर (it != phash.end())
              {
                  अगर (it->second.ptr() == prog.ptr())
                  {
                      phash.erase(*i);
                      cacheList.erase(i);
                      वापस;
                  }
              }
        }
    }

    std::string& getPrefixString()
    {
        अगर (prefix.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            अगर (prefix.empty())
            {
                CV_Assert(!devices.empty());
                स्थिर Device& d = devices[0];
                पूर्णांक bits = d.addressBits();
                अगर (bits > 0 && bits != 64)
                    prefix = cv::format("%d-bit--", bits);
                prefix += d.vendorName() + "--" + d.name() + "--" + d.driverVersion();
                // sanitize chars
                क्रम (माप_प्रकार i = 0; i < prefix.size(); i++)
                {
                    अक्षर c = prefix[i];
                    अगर (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix[i] = '_';
                    }
                }
            }
        }
        वापस prefix;
    }

    std::string& getPrefixBase()
    {
        अगर (prefix_base.empty())
        {
            cv::AutoLock lock(program_cache_mutex);
            अगर (prefix_base.empty())
            {
                स्थिर Device& d = devices[0];
                पूर्णांक bits = d.addressBits();
                अगर (bits > 0 && bits != 64)
                    prefix_base = cv::format("%d-bit--", bits);
                prefix_base += d.vendorName() + "--" + d.name() + "--";
                // sanitize chars
                क्रम (माप_प्रकार i = 0; i < prefix_base.size(); i++)
                {
                    अक्षर c = prefix_base[i];
                    अगर (!((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-'))
                    {
                        prefix_base[i] = '_';
                    }
                }
            }
        }
        वापस prefix_base;
    }

    IMPLEMENT_REFCOUNTABLE();

    स्थिर पूर्णांक contextId;  // global unique ID
    स्थिर std::string configuration;

    cl_context handle;
    std::vector<Device> devices;

    std::string prefix;
    std::string prefix_base;

    cv::Mutex program_cache_mutex;
    प्रकार std::map<std::string, Program> phash_t;
    phash_t phash;
    प्रकार std::list<cv::String> CacheList;
    CacheList cacheList;

    std::shared_ptr<OpenCLBufferPoolImpl> बफर_Pool_2;
    std::shared_ptr<OpenCLBufferPoolImpl> बफर_Pool_Host_Ptr;
    OpenCLBufferPoolImpl& get_बफर_Pool() स्थिर
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_2);
        वापस *बफर_Pool_2.लो();
    }
    OpenCLBufferPoolImpl& get_बफर_Pool_Host_Ptr() स्थिर
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_Host_Ptr);
        वापस *बफर_Pool_Host_Ptr.लो();
    }

    std::map<std::type_सूचकांक, std::shared_ptr<UserContext>> userContextStorage;
    cv::Mutex userContextMutex;
    व्योम setUserContext(std::type_सूचकांक typeId, स्थिर std::shared_ptr<UserContext>& userContext) {
        cv::AutoLock lock(userContextMutex);
        userContextStorage[typeId] = userContext;
    }
    std::shared_ptr<UserContext> getUserContext(std::type_सूचकांक typeId) {
        cv::AutoLock lock(userContextMutex);
        स्वतः it = userContextStorage.find(typeId);
        अगर (it != userContextStorage.end())
            वापस it->second;
        अन्यथा
            वापस nullptr;
    }

#अगर_घोषित HAVE_OPENCL_SVM
    bool svmInitialized;
    bool svmAvailable;
    bool svmEnabled;
    svm::SVMCapabilities svmCapabilities;
    svm::SVMFunctions svmFunctions;

    व्योम svmInit()
    {
        CV_Assert(handle != शून्य);
        स्थिर Device& device = devices[0];
        cl_device_svm_capabilities deviceCaps = 0;
        CV_Assert(((व्योम)0, CL_DEVICE_SVM_CAPABILITIES == CL_DEVICE_SVM_CAPABILITIES_AMD)); // Check assumption
        cl_int status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_SVM_CAPABILITIES, माप(deviceCaps), &deviceCaps, शून्य);
        अगर (status != CL_SUCCESS)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("CL_DEVICE_SVM_CAPABILITIES via clGetDeviceInfo failed: %d\n", status);
            जाओ noSVM;
        }
        CV_OPENCL_SVM_TRACE_P("CL_DEVICE_SVM_CAPABILITIES returned: 0x%x\n", (पूर्णांक)deviceCaps);
        CV_Assert(((व्योम)0, CL_DEVICE_SVM_COARSE_GRAIN_बफर == CL_DEVICE_SVM_COARSE_GRAIN_बफर_AMD)); // Check assumption
        svmCapabilities.मूल्य_2 =
                ((deviceCaps & CL_DEVICE_SVM_COARSE_GRAIN_बफर) ? svm::SVMCapabilities::SVM_COARSE_GRAIN_बफर : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_बफर) ? svm::SVMCapabilities::SVM_FINE_GRAIN_बफर : 0) |
                ((deviceCaps & CL_DEVICE_SVM_FINE_GRAIN_SYSTEM) ? svm::SVMCapabilities::SVM_FINE_GRAIN_SYSTEM : 0) |
                ((deviceCaps & CL_DEVICE_SVM_ATOMICS) ? svm::SVMCapabilities::SVM_ATOMICS : 0);
        svmCapabilities.मूल्य_2 &= svm::getSVMCapabilitiesMask();
        अगर (svmCapabilities.मूल्य_2 == 0)
        {
            CV_OPENCL_SVM_TRACE_ERROR_P("svmCapabilities is empty\n");
            जाओ noSVM;
        }
        प्रयास
        {
            // Try OpenCL 2.0
            CV_OPENCL_SVM_TRACE_P("Try SVM from OpenCL 2.0 ...\n");
            व्योम* ptr = clSVMAlloc(handle, CL_MEM_READ_WRITE, 100, 0);
            अगर (!ptr)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("clSVMAlloc returned NULL...\n");
                CV_Error(Error::StsBadArg, "clSVMAlloc returned NULL");
            }
            प्रयास
            {
                bool error = false;
                cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                अगर (CL_SUCCESS != clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE, ptr, 100, 0, शून्य, शून्य))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMMap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMMap FAILED");
                }
                clFinish(q);
                प्रयास
                {
                    ((पूर्णांक*)ptr)[0] = 100;
                }
                पकड़ो (...)
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("SVM buffer access test FAILED\n");
                    error = true;
                }
                अगर (CL_SUCCESS != clEnqueueSVMUnmap(q, ptr, 0, शून्य, शून्य))
                {
                    CV_OPENCL_SVM_TRACE_ERROR_P("clEnqueueSVMUnmap failed...\n");
                    CV_Error(Error::StsBadArg, "clEnqueueSVMUnmap FAILED");
                }
                clFinish(q);
                अगर (error)
                {
                    CV_Error(Error::StsBadArg, "OpenCL SVM buffer access test was FAILED");
                }
            }
            पकड़ो (...)
            {
                CV_OPENCL_SVM_TRACE_ERROR_P("OpenCL SVM buffer access test was FAILED\n");
                clSVMFree(handle, ptr);
                फेंको;
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
        पकड़ो (...)
        {
            CV_OPENCL_SVM_TRACE_P("clSVMAlloc failed, trying HSA extension...\n");
            प्रयास
            {
                // Try HSA extension
                String extensions = device.extensions();
                अगर (extensions.find("cl_amd_svm") == String::npos)
                {
                    CV_OPENCL_SVM_TRACE_P("Device extension doesn't have cl_amd_svm: %s\n", extensions.c_str());
                    जाओ noSVM;
                }
                cl_platform_id p = शून्य;
                CV_OCL_CHECK(status = clGetDeviceInfo((cl_device_id)device.ptr(), CL_DEVICE_PLATFORM, माप(cl_platform_id), &p, शून्य));
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
            पकड़ो (...)
            {
                CV_OPENCL_SVM_TRACE_P("Something is totally wrong\n");
                जाओ noSVM;
            }
        }

        svmAvailable = true;
        svmEnabled = !svm::checkDisableSVM();
        svmInitialized = true;
        CV_OPENCL_SVM_TRACE_P("OpenCV OpenCL SVM support initialized\n");
        वापस;
    noSVM:
        CV_OPENCL_SVM_TRACE_P("OpenCL SVM is not detected\n");
        svmAvailable = false;
        svmEnabled = false;
        svmCapabilities.मूल्य_2 = 0;
        svmInitialized = true;
        svmFunctions.fn_clSVMAlloc = शून्य;
        वापस;
    }

    std::shared_ptr<OpenCLSVMBufferPoolImpl> बफर_Pool_SVM;

    OpenCLSVMBufferPoolImpl& get_बफर_Pool_SVM() स्थिर
    {
        init_बफर_pools();
        CV_DbgAssert(बफर_Pool_SVM);
        वापस *बफर_Pool_SVM.लो();
    }
#पूर्ण_अगर

    मित्र श्रेणी Program;
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
Context::Context(पूर्णांक dtype)
{
    p = 0;
    create(dtype);
}

व्योम Context::release()
{
    अगर (p)
    {
        p->release();
        p = शून्य;
    }
}

bool Context::create()
{
    release();
    अगर (!haveOpenCL())
        वापस false;
    p = Impl::findOrCreateContext(std::string());
    अगर (p && p->handle)
        वापस true;
    release();
    वापस false;
}

// deprecated
bool Context::create(पूर्णांक dtype)
{
    अगर( !haveOpenCL() )
        वापस false;
    release();
    अगर (dtype == CL_DEVICE_TYPE_DEFAULT || (अचिन्हित)dtype == (अचिन्हित)CL_DEVICE_TYPE_ALL)
    {
        p = Impl::findOrCreateContext("");
    }
    अन्यथा अगर (dtype == CL_DEVICE_TYPE_GPU)
    {
        p = Impl::findOrCreateContext(":GPU:");
    }
    अन्यथा अगर (dtype == CL_DEVICE_TYPE_CPU)
    {
        p = Impl::findOrCreateContext(":CPU:");
    }
    अन्यथा
    {
        CV_LOG_ERROR(शून्य, "OpenCL: Can't recognize OpenCV device type=" << dtype);
    }
    अगर (p && !p->handle)
    {
        release();
    }
    वापस p != 0;
}

Context::Context(स्थिर Context& c)
{
    p = (Impl*)c.p;
    अगर(p)
        p->addref();
}

Context& Context::चालक = (स्थिर Context& c)
{
    Impl* newp = (Impl*)c.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Context::Context(Context&& c) CV_NOEXCEPT
{
    p = c.p;
    c.p = nullptr;
}

Context& Context::चालक = (Context&& c) CV_NOEXCEPT
{
    अगर (यह != &c) {
        अगर(p)
            p->release();
        p = c.p;
        c.p = nullptr;
    }
    वापस *यह;
}

व्योम* Context::ptr() स्थिर
{
    वापस p == शून्य ? शून्य : p->handle;
}

माप_प्रकार Context::ndevices() स्थिर
{
    वापस p ? p->devices.size() : 0;
}

Device& Context::device(माप_प्रकार idx) स्थिर
{
    अटल Device dummy;
    वापस !p || idx >= p->devices.size() ? dummy : p->devices[idx];
}

Context& Context::getDefault()
{
    स्वतः& c = OpenCLExecutionContext::getCurrent();
    अगर (!c.empty())
    {
        स्वतः& ctx = c.getContext();
        वापस ctx;
    }

    अटल Context dummy;
    वापस dummy;
}

Program Context::getProg(स्थिर ProgramSource& prog,
                         स्थिर String& buildopts, String& errmsg)
{
    वापस p ? p->getProg(prog, buildopts, errmsg) : Program();
}

व्योम Context::unloadProg(Program& prog)
{
    अगर (p)
        p->unloadProg(prog);
}

/* static */
Context Context::fromHandle(व्योम* context)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext((cl_context)context);
    वापस ctx;
}

/* static */
Context Context::fromDevice(स्थिर ocl::Device& device)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(device);
    वापस ctx;
}

/* static */
Context Context::create(स्थिर std::string& configuration)
{
    Context ctx;
    ctx.p = Impl::findOrCreateContext(configuration);
    वापस ctx;
}

व्योम* Context::getOpenCLContextProperty(पूर्णांक propertyId) स्थिर
{
    अगर (p == शून्य)
        वापस nullptr;
    ::माप_प्रकार size = 0;
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, 0, शून्य, &size));
    std::vector<cl_context_properties> prop(size / माप(cl_context_properties), (cl_context_properties)0);
    CV_OCL_CHECK(clGetContextInfo(p->handle, CL_CONTEXT_PROPERTIES, size, prop.तथ्य(), शून्य));
    क्रम (माप_प्रकार i = 0; i < prop.size(); i += 2)
    {
        अगर (prop[i] == (cl_context_properties)propertyId)
        {
            CV_LOG_DEBUG(शून्य, "OpenCL: found context property=" << propertyId << ") => " << (व्योम*)prop[i + 1]);
            वापस (व्योम*)prop[i + 1];
        }
    }
    वापस nullptr;
}

#अगर_घोषित HAVE_OPENCL_SVM
bool Context::useSVM() स्थिर
{
    Context::Impl* i = p;
    CV_Assert(i);
    अगर (!i->svmInitialized)
        i->svmInit();
    वापस i->svmEnabled;
}
व्योम Context::setUseSVM(bool enabled)
{
    Context::Impl* i = p;
    CV_Assert(i);
    अगर (!i->svmInitialized)
        i->svmInit();
    अगर (enabled && !i->svmAvailable)
    {
        CV_Error(Error::StsError, "OpenCL Shared Virtual Memory (SVM) is not supported by OpenCL device");
    }
    i->svmEnabled = enabled;
}
#अन्यथा
bool Context::useSVM() स्थिर { वापस false; }
व्योम Context::setUseSVM(bool enabled) { CV_Assert(!enabled); }
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENCL_SVM
namespace svm {

स्थिर SVMCapabilities getSVMCapabilitites(स्थिर ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    अगर (!i->svmInitialized)
        i->svmInit();
    वापस i->svmCapabilities;
}

CV_EXPORTS स्थिर SVMFunctions* getSVMFunctions(स्थिर ocl::Context& context)
{
    Context::Impl* i = context.getImpl();
    CV_Assert(i);
    CV_Assert(i->svmInitialized); // getSVMCapabilitites() must be called first
    CV_Assert(i->svmFunctions.fn_clSVMAlloc != शून्य);
    वापस &i->svmFunctions;
}

CV_EXPORTS bool useSVM(UMatUsageFlags usageFlags)
{
    अगर (checkForceSVMUmatUsage())
        वापस true;
    अगर (checkDisableSVMUMatUsage())
        वापस false;
    अगर ((usageFlags & USAGE_ALLOCATE_SHARED_MEMORY) != 0)
        वापस true;
    वापस false; // don't use SVM by default
}

} // namespace cv::ocl::svm
#पूर्ण_अगर // HAVE_OPENCL_SVM

Context::UserContext::~UserContext()
{
}

व्योम Context::setUserContext(std::type_सूचकांक typeId, स्थिर std::shared_ptr<Context::UserContext>& userContext)
{
    CV_Assert(p);
    p->setUserContext(typeId, userContext);
}

std::shared_ptr<Context::UserContext> Context::getUserContext(std::type_सूचकांक typeId)
{
    CV_Assert(p);
    वापस p->getUserContext(typeId);
}

अटल व्योम get_platform_name(cl_platform_id id, String& name)
{
    // get platform name string length
    माप_प्रकार sz = 0;
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, 0, 0, &sz));

    // get platform name string
    Auto_बफर<अक्षर> buf(sz + 1);
    CV_OCL_CHECK(clGetPlatformInfo(id, CL_PLATFORM_NAME, sz, buf.तथ्य(), 0));

    // just in case, ensure trailing zero for ASCIIZ string
    buf[sz] = 0;

    name = buf.तथ्य();
}

/*
// Attaches OpenCL context to OpenCV
*/
व्योम attachContext(स्थिर String& platformName, व्योम* platformID, व्योम* context, व्योम* deviceID)
{
    स्वतः ctx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    ctx.bind();
}

/* static */
OpenCLExecutionContext OpenCLExecutionContext::create(
        स्थिर std::string& platformName, व्योम* platformID, व्योम* context, व्योम* deviceID
)
{
    अगर (!haveOpenCL())
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL runtime is not available!");

    cl_uint cnt = 0;
    CV_OCL_CHECK(clGetPlatformIDs(0, 0, &cnt));

    अगर (cnt == 0)
        CV_Error(cv::Error::OpenCLApiCallError, "No OpenCL platform available!");

    std::vector<cl_platform_id> platforms(cnt);

    CV_OCL_CHECK(clGetPlatformIDs(cnt, &platforms[0], 0));

    bool platformAvailable = false;

    // check if external platformName contained in list of available platforms in OpenCV
    क्रम (अचिन्हित पूर्णांक i = 0; i < cnt; i++)
    {
        String availablePlatformName;
        get_platform_name(platforms[i], availablePlatformName);
        // external platform is found in the list of available platforms
        अगर (platformName == availablePlatformName)
        {
            platformAvailable = true;
            अवरोध;
        }
    }

    अगर (!platformAvailable)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    // check if platformID corresponds to platformName
    String actualPlatformName;
    get_platform_name((cl_platform_id)platformID, actualPlatformName);
    अगर (platformName != actualPlatformName)
        CV_Error(cv::Error::OpenCLApiCallError, "No matched platforms available!");

    OpenCLExecutionContext ctx;
    ctx.p = std::make_shared<OpenCLExecutionContext::Impl>((cl_platform_id)platformID, (cl_context)context, (cl_device_id)deviceID);
    CV_OCL_CHECK(clReleaseContext((cl_context)context));
    CV_OCL_CHECK(clReleaseDevice((cl_device_id)deviceID));
    वापस ctx;
}

व्योम initializeContextFromHandle(Context& ctx, व्योम* _platform, व्योम* _context, व्योम* _device)
{
    // internal call, less checks
    cl_platform_id platformID = (cl_platform_id)_platform;
    cl_context context = (cl_context)_context;
    cl_device_id deviceID = (cl_device_id)_device;

    std::string platformName = PlatformInfo(&platformID).name();

    स्वतः clExecCtx = OpenCLExecutionContext::create(platformName, platformID, context, deviceID);
    CV_Assert(!clExecCtx.empty());
    ctx = clExecCtx.getContext();
}

/////////////////////////////////////////// Queue /////////////////////////////////////////////

काष्ठा Queue::Impl
{
    अंतरभूत व्योम __init()
    {
        refcount = 1;
        handle = 0;
        isProfilingQueue_ = false;
    }

    Impl(cl_command_queue q)
    {
        __init();
        handle = q;

        cl_command_queue_properties props = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_PROPERTIES, माप(cl_command_queue_properties), &props, शून्य));
        isProfilingQueue_ = !!(props & CL_QUEUE_PROFILING_ENABLE);
    }

    Impl(cl_command_queue q, bool isProfilingQueue)
    {
        __init();
        handle = q;
        isProfilingQueue_ = isProfilingQueue;
    }

    Impl(स्थिर Context& c, स्थिर Device& d, bool withProfiling = false)
    {
        __init();

        स्थिर Context* pc = &c;
        cl_context ch = (cl_context)pc->ptr();
        अगर( !ch )
        {
            pc = &Context::getDefault();
            ch = (cl_context)pc->ptr();
        }
        cl_device_id dh = (cl_device_id)d.ptr();
        अगर( !dh )
            dh = (cl_device_id)pc->device(0).ptr();
        cl_int retval = 0;
        cl_command_queue_properties props = withProfiling ? CL_QUEUE_PROFILING_ENABLE : 0;
        CV_OCL_DBG_CHECK_(handle = clCreateCommandQueue(ch, dh, props, &retval), retval);
        isProfilingQueue_ = withProfiling;
    }

    ~Impl()
    {
#अगर_घोषित _WIN32
        अगर (!cv::__termination)
#पूर्ण_अगर
        {
            अगर(handle)
            {
                CV_OCL_DBG_CHECK(clFinish(handle));
                CV_OCL_DBG_CHECK(clReleaseCommandQueue(handle));
                handle = शून्य;
            }
        }
    }

    स्थिर cv::ocl::Queue& getProfilingQueue(स्थिर cv::ocl::Queue& self)
    {
        अगर (isProfilingQueue_)
            वापस self;

        अगर (profiling_queue_.ptr())
            वापस profiling_queue_;

        cl_context ctx = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_CONTEXT, माप(cl_context), &ctx, शून्य));

        cl_device_id device = 0;
        CV_OCL_CHECK(clGetCommandQueueInfo(handle, CL_QUEUE_DEVICE, माप(cl_device_id), &device, शून्य));

        cl_int परिणाम = CL_SUCCESS;
        cl_command_queue_properties props = CL_QUEUE_PROFILING_ENABLE;
        cl_command_queue q = clCreateCommandQueue(ctx, device, props, &परिणाम);
        CV_OCL_DBG_CHECK_परिणाम(परिणाम, "clCreateCommandQueue(with CL_QUEUE_PROFILING_ENABLE)");

        Queue queue;
        queue.p = नया Impl(q, true);
        profiling_queue_ = queue;

        वापस profiling_queue_;
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_command_queue handle;
    bool isProfilingQueue_;
    cv::ocl::Queue profiling_queue_;
};

Queue::Queue() CV_NOEXCEPT
{
    p = 0;
}

Queue::Queue(स्थिर Context& c, स्थिर Device& d)
{
    p = 0;
    create(c, d);
}

Queue::Queue(स्थिर Queue& q)
{
    p = q.p;
    अगर(p)
        p->addref();
}

Queue& Queue::चालक = (स्थिर Queue& q)
{
    Impl* newp = (Impl*)q.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Queue::Queue(Queue&& q) CV_NOEXCEPT
{
    p = q.p;
    q.p = nullptr;
}

Queue& Queue::चालक = (Queue&& q) CV_NOEXCEPT
{
    अगर (यह != &q) {
        अगर(p)
            p->release();
        p = q.p;
        q.p = nullptr;
    }
    वापस *यह;
}

Queue::~Queue()
{
    अगर(p)
        p->release();
}

bool Queue::create(स्थिर Context& c, स्थिर Device& d)
{
    अगर(p)
        p->release();
    p = नया Impl(c, d);
    वापस p->handle != 0;
}

व्योम Queue::finish()
{
    अगर(p && p->handle)
    {
        CV_OCL_DBG_CHECK(clFinish(p->handle));
    }
}

स्थिर Queue& Queue::getProfilingQueue() स्थिर
{
    CV_Assert(p);
    वापस p->getProfilingQueue(*यह);
}

व्योम* Queue::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

Queue& Queue::getDefault()
{
    स्वतः& c = OpenCLExecutionContext::getCurrent();
    अगर (!c.empty())
    {
        स्वतः& q = c.getQueue();
        वापस q;
    }
    अटल Queue dummy;
    वापस dummy;
}

अटल cl_command_queue getQueue(स्थिर Queue& q)
{
    cl_command_queue qq = (cl_command_queue)q.ptr();
    अगर(!qq)
        qq = (cl_command_queue)Queue::getDefault().ptr();
    वापस qq;
}

/////////////////////////////////////////// KernelArg /////////////////////////////////////////////

KernelArg::KernelArg() CV_NOEXCEPT
    : flags(0), m(0), obj(0), sz(0), wscale(1), iwscale(1)
{
}

KernelArg::KernelArg(पूर्णांक _flags, UMat* _m, पूर्णांक _wscale, पूर्णांक _iwscale, स्थिर व्योम* _obj, माप_प्रकार _sz)
    : flags(_flags), m(_m), obj(_obj), sz(_sz), wscale(_wscale), iwscale(_iwscale)
{
    CV_Assert(_flags == LOCAL || _flags == CONSTANT || _m != शून्य);
}

KernelArg KernelArg::Constant(स्थिर Mat& m)
{
    CV_Assert(m.isContinuous());
    वापस KernelArg(CONSTANT, 0, 0, 0, m.ptr(), m.योग()*m.elem_आकार());
}

/////////////////////////////////////////// Kernel /////////////////////////////////////////////

काष्ठा Kernel::Impl
{
    Impl(स्थिर अक्षर* kname, स्थिर Program& prog) :
        refcount(1), handle(शून्य), isInProgress(false), isAsyncRun(false), nu(0)
    {
        cl_program ph = (cl_program)prog.ptr();
        cl_int retval = 0;
        name = kname;
        अगर (ph)
        {
            handle = clCreateKernel(ph, kname, &retval);
            CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateKernel('%s')", kname).c_str());
        }
        क्रम( पूर्णांक i = 0; i < MAX_ARRS; i++ )
            u[i] = 0;
        have_अस्थायी_Dst_UMats = false;
        have_अस्थायी_Src_UMats = false;
    }

    व्योम cleanupUMats()
    {
        bool exceptionOccurred = false;
        क्रम( पूर्णांक i = 0; i < MAX_ARRS; i++ )
        {
            अगर( u[i] )
            {
                अगर( CV_XADD(&u[i]->urefcount, -1) == 1 )
                {
                    u[i]->flags |= UMat_तथ्य::ASYNC_CLEANUP;
                    प्रयास
                    {
                        u[i]->currAllocator->deallocate(u[i]);
                    }
                    पकड़ो(स्थिर std::exception& exc)
                    {
                        // limited by legacy before C++11, therefore log and
                        // remember some exception occurred to throw below
                        CV_LOG_ERROR(शून्य, "OCL: Unexpected C++ exception in OpenCL Kernel::Impl::cleanupUMats(): " << exc.what());
                        exceptionOccurred = true;
                    }
                }
                u[i] = 0;
            }
        }
        nu = 0;
        have_अस्थायी_Dst_UMats = false;
        have_अस्थायी_Src_UMats = false;
        CV_Assert(!exceptionOccurred);
    }

    व्योम addUMat(स्थिर UMat& m, bool dst)
    {
        // TSAN false positive: see #27638
        CV_Assert(nu < MAX_ARRS && m.u && m.u->urefcount > 0);
        u[nu] = m.u;
        // TSAN false positive: see #27638
        CV_XADD(&m.u->urefcount, 1);
        nu++;
        अगर(dst && m.u->अस्थायी_UMat())
            have_अस्थायी_Dst_UMats = true;
        अगर(m.u->original_UMat_तथ्य == शून्य && m.u->अस्थायी_UMat())
            have_अस्थायी_Src_UMats = true;  // UMat is created on RAW memory (without proper lifetime management, even from Mat)
    }

    /// Preserve image lifetime (while it is specified as Kernel argument)
    व्योम registerImageArgument(पूर्णांक arg, स्थिर Image2D& image)
    {
        CV_CheckGE(arg, 0, "");
        अगर (arg < (पूर्णांक)shadow_images.size() && shadow_images[arg].ptr() != image.ptr())  // TODO future: replace ptr => impl (more strong check)
        {
            CV_Check(arg, !isInProgress, "ocl::Kernel: clearing of pending Image2D arguments is not allowed");
        }
        shadow_images.reserve(MAX_ARRS);
        shadow_images.resize(std::max(shadow_images.size(), (माप_प्रकार)arg + 1));
        shadow_images[arg] = image;
    }

    व्योम finit(cl_event e)
    {
        CV_UNUSED(e);
        isInProgress = false;
        प्रयास
        {
            cleanupUMats();
        }
        पकड़ो(...)
        {
            release();
            फेंको;
        }
        release();
    }

    bool run(पूर्णांक dims, माप_प्रकार _globalsize[], माप_प्रकार _localsize[],
            bool sync, int64* timeNS, स्थिर Queue& q);

    ~Impl()
    {
        अगर(handle)
        {
            CV_OCL_DBG_CHECK(clReleaseKernel(handle));
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cv::String name;
    cl_kernel handle;
    क्रमागत { MAX_ARRS = 16 };
    UMat_तथ्य* u[MAX_ARRS];
    bool isInProgress;
    bool isAsyncRun;  // true if kernel was scheduled in async mode
    पूर्णांक nu;
    std::vector<Image2D> shadow_images;
    bool have_अस्थायी_Dst_UMats;
    bool have_अस्थायी_Src_UMats;
};

}} // namespace cv::ocl

बाह्य "C" {

अटल व्योम CL_CALLBACK oclCleanupCallback(cl_event e, cl_int, व्योम *p)
{
    प्रयास
    {
        ((cv::ocl::Kernel::Impl*)p)->finit(e);
    }
    पकड़ो (स्थिर cv::Exception& exc)
    {
        CV_LOG_ERROR(शून्य, "OCL: Unexpected OpenCV exception in OpenCL callback: " << exc.what());
    }
    पकड़ो (स्थिर std::exception& exc)
    {
        CV_LOG_ERROR(शून्य, "OCL: Unexpected C++ exception in OpenCL callback: " << exc.what());
    }
    पकड़ो (...)
    {
        CV_LOG_ERROR(शून्य, "OCL: Unexpected unknown C++ exception in OpenCL callback");
    }
}

}

namespace cv { namespace ocl {

Kernel::Kernel() CV_NOEXCEPT
{
    p = 0;
}

Kernel::Kernel(स्थिर अक्षर* kname, स्थिर Program& prog)
{
    p = 0;
    create(kname, prog);
}

Kernel::Kernel(स्थिर अक्षर* kname, स्थिर ProgramSource& src,
               स्थिर String& buildopts, String* errmsg)
{
    p = 0;
    create(kname, src, buildopts, errmsg);
}

Kernel::Kernel(स्थिर Kernel& k)
{
    p = k.p;
    अगर(p)
        p->addref();
}

Kernel& Kernel::चालक = (स्थिर Kernel& k)
{
    Impl* newp = (Impl*)k.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Kernel::Kernel(Kernel&& k) CV_NOEXCEPT
{
    p = k.p;
    k.p = nullptr;
}

Kernel& Kernel::चालक = (Kernel&& k) CV_NOEXCEPT
{
    अगर (यह != &k) {
        अगर(p)
            p->release();
        p = k.p;
        k.p = nullptr;
    }
    वापस *यह;
}

Kernel::~Kernel()
{
    अगर(p)
        p->release();
}

bool Kernel::create(स्थिर अक्षर* kname, स्थिर Program& prog)
{
    अगर(p)
        p->release();
    p = नया Impl(kname, prog);
    अगर(p->handle == 0)
    {
        p->release();
        p = 0;
    }
#अगर_घोषित CV_OPENCL_RUN_ASSERT // check kernel compilation fails
    CV_Assert(p);
#पूर्ण_अगर
    वापस p != 0;
}

bool Kernel::create(स्थिर अक्षर* kname, स्थिर ProgramSource& src,
                    स्थिर String& buildopts, String* errmsg)
{
    अगर(p)
    {
        p->release();
        p = 0;
    }
    String tempmsg;
    अगर( !errmsg ) errmsg = &tempmsg;
    स्थिर Program prog = Context::getDefault().getProg(src, buildopts, *errmsg);
    वापस create(kname, prog);
}

व्योम* Kernel::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

bool Kernel::empty() स्थिर
{
    वापस ptr() == 0;
}

अटल cv::String dump_मूल्य(माप_प्रकार sz, स्थिर व्योम* p)
{
    अगर (!p)
        वापस "NULL";
    अगर (sz == 2)
        वापस cv::format("%d / %uu / 0x%04x", *(लघु*)p, *(अचिन्हित लघु*)p, *(लघु*)p);
    अगर (sz == 4)
        वापस cv::format("%d / %uu / 0x%08x / %g", *(पूर्णांक*)p, *(पूर्णांक*)p, *(पूर्णांक*)p, *(भग्न*)p);
    अगर (sz == 8)
        वापस cv::format("%lld / %lluu / 0x%16llx / %g", *(दीर्घ दीर्घ*)p, *(दीर्घ दीर्घ*)p, *(दीर्घ दीर्घ*)p, *(द्विगुन*)p);
    वापस cv::format("%p", p);
}

पूर्णांक Kernel::set(पूर्णांक i, स्थिर व्योम* value, माप_प्रकार sz)
{
    अगर (!p || !p->handle)
        वापस -1;
    अगर (i < 0)
        वापस i;
    अगर( i == 0 )
        p->cleanupUMats();

    cl_int retval = clSetKernelArg(p->handle, (cl_uint)i, sz, value);
    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, value=%s)", p->name.c_str(), (पूर्णांक)i, (पूर्णांक)sz, dump_मूल्य(sz, value).c_str()).c_str());
    अगर (retval != CL_SUCCESS)
        वापस -1;
    वापस i+1;
}

पूर्णांक Kernel::set(पूर्णांक i, स्थिर Image2D& image2D)
{
    cl_mem h = (cl_mem)image2D.ptr();
    पूर्णांक res = set(i, &h, माप(h));
    अगर (res >= 0)
        p->registerImageArgument(i, image2D);
    वापस res;
}

पूर्णांक Kernel::set(पूर्णांक i, स्थिर UMat& m)
{
    वापस set(i, KernelArg(KernelArg::READ_WRITE, (UMat*)&m));
}

// a CUDA-backed UMat carries a device pointer in u->handle, not a cl_mem
अटल bool isCudaBacked(स्थिर UMat_तथ्य* u)
{
    स्थिर MatAllocator* cudaAllocator = cv::cuda::getCudaAllocator();
    वापस u && cudaAllocator && u->currAllocator == cudaAllocator;
}

पूर्णांक Kernel::set(पूर्णांक i, स्थिर KernelArg& arg)
{
    अगर( !p || !p->handle )
        वापस -1;
    अगर (i < 0)
    {
        CV_LOG_ERROR(शून्य, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): negative arg_index",
                p->name.c_str(), (पूर्णांक)i));
        वापस i;
    }
    अगर( i == 0 )
        p->cleanupUMats();
    cl_int status = 0;
    अगर( arg.m )
    {
        AccessFlag accessFlags = ((arg.flags & KernelArg::READ_ONLY) ? ACCESS_READ : static_cast<AccessFlag>(0)) |
                                 ((arg.flags & KernelArg::WRITE_ONLY) ? ACCESS_WRITE : static_cast<AccessFlag>(0));
        bool ptronly = (arg.flags & KernelArg::PTR_ONLY) != 0;
        अगर (ptronly && arg.m->empty())
        {
            cl_mem h_null = (cl_mem)शून्य;
            status = clSetKernelArg(p->handle, (cl_uint)i, माप(h_null), &h_null);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=NULL)", p->name.c_str(), (पूर्णांक)i).c_str());
            वापस i + 1;
        }
        // a CUDA-backed UMat has no cl_mem; decline so the caller falls back to CPU
        अगर (isCudaBacked(arg.m->u))
        {
            CV_LOG_DEBUG(शून्य, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d): UMat is backed by the CUDA allocator, declining",
                    p->name.c_str(), (पूर्णांक)i));
            p->cleanupUMats();
            p->release();
            p = 0;
            वापस -1;
        }
        cl_mem h = (cl_mem)arg.m->handle(accessFlags);

        अगर (!h)
        {
            CV_LOG_ERROR(शून्य, cv::format("OpenCL: Kernel(%s)::set(arg_index=%d, flags=%d): can't create cl_mem handle for passed UMat buffer (addr=%p)",
                    p->name.c_str(), (पूर्णांक)i, (पूर्णांक)arg.flags, arg.m));
            p->release();
            p = 0;
            वापस -1;
        }

#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((arg.m->u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
        {
            स्थिर Context& ctx = Context::getDefault();
            स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            uchar*& svm_तथ्य_Ptr = (uchar*&)arg.m->u->handle;
            CV_OPENCL_SVM_TRACE_P("clSetKernelArgSVMPointer: %p\n", svm_तथ्य_Ptr);
#अगर 1 // TODO
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, svm_तथ्य_Ptr);
#अन्यथा
            status = svmFns->fn_clSetKernelArgSVMPointer(p->handle, (cl_uint)i, &svm_तथ्य_Ptr);
#पूर्ण_अगर
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArgSVMPointer('%s', arg_index=%d, ptr=%p)", p->name.c_str(), (पूर्णांक)i, (व्योम*)svm_तथ्य_Ptr).c_str());
        }
        अन्यथा
#पूर्ण_अगर
        {
            status = clSetKernelArg(p->handle, (cl_uint)i, माप(h), &h);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, cl_mem=%p)", p->name.c_str(), (पूर्णांक)i, (व्योम*)h).c_str());
        }

        अगर (ptronly)
        {
            i++;
        }
        अन्यथा अगर( arg.m->dims <= 2 )
        {
            UMat2D u2d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), माप(u2d.step), &u2d.step);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णांक)(i+1), (पूर्णांक)u2d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), माप(u2d.offset), &u2d.offset);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णांक)(i+2), (पूर्णांक)u2d.offset).c_str());
            i += 3;

            अगर( !(arg.flags & KernelArg::NO_आकार) )
            {
                पूर्णांक cols = u2d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, माप(u2d.rows), &u2d.rows);
                CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णांक)i, (पूर्णांक)u2d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), माप(cols), &cols);
                CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णांक)(i+1), (पूर्णांक)cols).c_str());
                i += 2;
            }
        }
        अन्यथा
        {
            UMat3D u3d(*arg.m);
            status = clSetKernelArg(p->handle, (cl_uint)(i+1), माप(u3d.slicestep), &u3d.slicestep);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, slicestep_value=%d)", p->name.c_str(), (पूर्णांक)(i+1), (पूर्णांक)u3d.slicestep).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+2), माप(u3d.step), &u3d.step);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, step_value=%d)", p->name.c_str(), (पूर्णांक)(i+2), (पूर्णांक)u3d.step).c_str());
            status = clSetKernelArg(p->handle, (cl_uint)(i+3), माप(u3d.offset), &u3d.offset);
            CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, offset_value=%d)", p->name.c_str(), (पूर्णांक)(i+3), (पूर्णांक)u3d.offset).c_str());
            i += 4;
            अगर( !(arg.flags & KernelArg::NO_आकार) )
            {
                पूर्णांक cols = u3d.cols*arg.wscale/arg.iwscale;
                status = clSetKernelArg(p->handle, (cl_uint)i, माप(u3d.slices), &u3d.slices);
                CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, slices_value=%d)", p->name.c_str(), (पूर्णांक)i, (पूर्णांक)u3d.slices).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+1), माप(u3d.rows), &u3d.rows);
                CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, rows_value=%d)", p->name.c_str(), (पूर्णांक)(i+1), (पूर्णांक)u3d.rows).c_str());
                status = clSetKernelArg(p->handle, (cl_uint)(i+2), माप(u3d.cols), &cols);
                CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, cols_value=%d)", p->name.c_str(), (पूर्णांक)(i+2), (पूर्णांक)cols).c_str());
                i += 3;
            }
        }
        p->addUMat(*arg.m, !!(accessFlags & ACCESS_WRITE));
        वापस i;
    }
    status = clSetKernelArg(p->handle, (cl_uint)i, arg.sz, arg.obj);
    CV_OCL_DBG_CHECK_परिणाम(status, cv::format("clSetKernelArg('%s', arg_index=%d, size=%d, obj=%p)", p->name.c_str(), (पूर्णांक)i, (पूर्णांक)arg.sz, (व्योम*)arg.obj).c_str());
    वापस i+1;
}

bool Kernel::run(पूर्णांक dims, माप_प्रकार _globalsize[], माप_प्रकार _localsize[],
                 bool sync, स्थिर Queue& q)
{
    अगर (!p)
        वापस false;

    माप_प्रकार globalsize[CV_MAX_DIM] = {1,1,1};
    माप_प्रकार योग = 1;
    CV_Assert(_globalsize != शून्य);
    क्रम (पूर्णांक i = 0; i < dims; i++)
    {
        माप_प्रकार val = _localsize ? _localsize[i] :
            dims == 1 ? 64 : dims == 2 ? (i == 0 ? 256 : 8) : dims == 3 ? (8>>(पूर्णांक)(i>0)) : 1;
        CV_Assert( val > 0 );
        योग *= _globalsize[i];
        अगर (_globalsize[i] == 1 && !_localsize)
            val = 1;
        globalsize[i] = divUp(_globalsize[i], (अचिन्हित पूर्णांक)val) * val;
    }
    CV_Assert(योग > 0);

    वापस p->run(dims, globalsize, _localsize, sync, शून्य, q);
}


bool Kernel::run_(पूर्णांक dims, माप_प्रकार _globalsize[], माप_प्रकार _localsize[],
                  bool sync, स्थिर Queue& q)
{
    CV_Assert(p);
    वापस p->run(dims, _globalsize, _localsize, sync, शून्य, q);
}


bool Kernel::Impl::run(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[],
        bool sync, int64* timeNS, स्थिर Queue& q)
{
    CV_INSTRUMENT_REGION_OPENCL_RUN(name.c_str());

    अगर (!handle)
    {
        CV_LOG_ERROR(शून्य, "OpenCL kernel has zero handle: " << name);
        वापस false;
    }

    अगर (isAsyncRun)
    {
        CV_Error_(Error::StsError, ("OpenCL kernel can't be reused in async mode: %s", name.c_str()));
    }
    isAsyncRun = !sync;

    अगर (isInProgress)
    {
        CV_Error_(Error::StsError, ("Previous OpenCL kernel launch is not finished: %s", name.c_str()));
    }

#अगर CV_OPENCL_SYNC_RUN_KERNELS
    sync = true;
#पूर्ण_अगर

    cl_command_queue qq = getQueue(q);
    अगर (have_अस्थायी_Dst_UMats)
        sync = true;
    अगर (have_अस्थायी_Src_UMats)
        sync = true;
    अगर (timeNS)
        sync = true;
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueNDRangeKernel(qq, handle, (cl_uint)dims,
                                           शून्य, globalsize, localsize, 0, 0,
                                           (sync && !timeNS) ? 0 : &asyncEvent);
#अगर !CV_OPENCL_SHOW_RUN_KERNELS
    अगर (retval != CL_SUCCESS)
#पूर्ण_अगर
    {
        cv::String संदेश = cv::format("clEnqueueNDRangeKernel('%s', dims=%d, globalsize=%zux%zux%zu, localsize=%s) sync=%s", name.c_str(), (पूर्णांक)dims,
                        globalsize[0], (dims > 1 ? globalsize[1] : 1), (dims > 2 ? globalsize[2] : 1),
                        (localsize ? cv::format("%zux%zux%zu", localsize[0], (dims > 1 ? localsize[1] : 1), (dims > 2 ? localsize[2] : 1)) : cv::String("NULL")).c_str(),
                        sync ? "true" : "false"
                        );
        अगर (retval != CL_SUCCESS)
        {
            संदेश = CV_OCL_API_ERROR_संदेश(retval, संदेश.c_str());
        }
#अगर CV_OPENCL_TRACE_CHECK
        CV_OCL_TRACE_CHECK_परिणाम(retval, संदेश.c_str());
#अन्यथा
        म_लिखो("%s\n", संदेश.c_str());
        ख_साफ(मानक_निकास);
#पूर्ण_अगर
    }
    अगर (sync || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        अगर (timeNS)
        {
            अगर (retval == CL_SUCCESS)
            {
                CV_OCL_DBG_CHECK(clWaitForEvents(1, &asyncEvent));
                cl_ulong startTime, stopTime;
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_START, माप(startTime), &startTime, शून्य));
                CV_OCL_CHECK(clGetEventProfilingInfo(asyncEvent, CL_PROFILING_COMMAND_END, माप(stopTime), &stopTime, शून्य));
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
        isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, यह));
    }
    अगर (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    वापस retval == CL_SUCCESS;
}

bool Kernel::runTask(bool sync, स्थिर Queue& q)
{
    अगर(!p || !p->handle || p->isInProgress)
        वापस false;

    cl_command_queue qq = getQueue(q);
    cl_event asyncEvent = 0;
    cl_int retval = clEnqueueTask(qq, p->handle, 0, 0, sync ? 0 : &asyncEvent);
    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clEnqueueTask('%s') sync=%s", p->name.c_str(), sync ? "true" : "false").c_str());
    अगर (sync || retval != CL_SUCCESS)
    {
        CV_OCL_DBG_CHECK(clFinish(qq));
        p->cleanupUMats();
    }
    अन्यथा
    {
        p->addref();
        p->isInProgress = true;
        CV_OCL_CHECK(clSetEventCallback(asyncEvent, CL_COMPLETE, oclCleanupCallback, p));
    }
    अगर (asyncEvent)
        CV_OCL_DBG_CHECK(clReleaseEvent(asyncEvent));
    वापस retval == CL_SUCCESS;
}

int64 Kernel::runProfiling(पूर्णांक dims, माप_प्रकार globalsize[], माप_प्रकार localsize[], स्थिर Queue& q_)
{
    CV_Assert(p && p->handle && !p->isInProgress);
    Queue q = q_.ptr() ? q_ : Queue::getDefault();
    CV_Assert(q.ptr());
    q.finish(); // call clFinish() on base queue
    Queue profilingQueue = q.getProfilingQueue();
    int64 timeNs = -1;
    bool res = p->run(dims, globalsize, localsize, true, &timeNs, profilingQueue);
    वापस res ? timeNs : -1;
}

माप_प्रकार Kernel::work_Group_आकार() स्थिर
{
    अगर(!p || !p->handle)
        वापस 0;
    माप_प्रकार val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_WORK_GROUP_आकार, माप(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(status, "clGetKernelWorkGroupInfo(CL_KERNEL_WORK_GROUP_SIZE)");
    वापस status == CL_SUCCESS ? val : 0;
}

माप_प्रकार Kernel::prefered_Work_Group_आकार_Multiple() स्थिर
{
    अगर(!p || !p->handle)
        वापस 0;
    माप_प्रकार val = 0, retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_PREFERRED_WORK_GROUP_आकार_MULTIPLE, माप(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(status, "clGetKernelWorkGroupInfo(CL_KERNEL_PREFERRED_WORK_GROUP_SIZE_MULTIPLE)");
    वापस status == CL_SUCCESS ? val : 0;
}

bool Kernel::compile_Work_Group_आकार(माप_प्रकार wsz[]) स्थिर
{
    अगर(!p || !p->handle || !wsz)
        वापस 0;
    माप_प्रकार retsz = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_COMPILE_WORK_GROUP_आकार, माप(wsz[0])*3, wsz, &retsz);
    CV_OCL_CHECK_परिणाम(status, "clGetKernelWorkGroupInfo(CL_KERNEL_COMPILE_WORK_GROUP_SIZE)");
    वापस status == CL_SUCCESS;
}

माप_प्रकार Kernel::local_Mem_आकार() स्थिर
{
    अगर(!p || !p->handle)
        वापस 0;
    माप_प्रकार retsz = 0;
    cl_ulong val = 0;
    cl_device_id dev = (cl_device_id)Device::getDefault().ptr();
    cl_int status = clGetKernelWorkGroupInfo(p->handle, dev, CL_KERNEL_LOCAL_MEM_आकार, माप(val), &val, &retsz);
    CV_OCL_CHECK_परिणाम(status, "clGetKernelWorkGroupInfo(CL_KERNEL_LOCAL_MEM_SIZE)");
    वापस status == CL_SUCCESS ? (माप_प्रकार)val : 0;
}



///////////////////////////////////////// ProgramSource ///////////////////////////////////////////////

काष्ठा ProgramSource::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    क्रमागत KIND {
        PROGRAM_SOURCE_CODE = 0,
        PROGRAM_BINARIES,
        PROGRAM_SPIR,
        PROGRAM_SPIRV
    } kind_;

    Impl(स्थिर String& src)
    {
        init(PROGRAM_SOURCE_CODE, cv::String(), cv::String());
        initFromSource(src, cv::String());
    }
    Impl(स्थिर String& module, स्थिर String& name, स्थिर String& codeStr, स्थिर String& codeHash)
    {
        init(PROGRAM_SOURCE_CODE, module, name);
        initFromSource(codeStr, codeHash);
    }

    /// reset fields
    व्योम init(क्रमागत KIND kind, स्थिर String& module, स्थिर String& name)
    {
        refcount = 1;
        kind_ = kind;
        module_ = module;
        name_ = name;

        sourceAddr_ = शून्य;
        source_आकार = 0;
        isHashUpdated = false;
    }

    व्योम initFromSource(स्थिर String& codeStr, स्थिर String& codeHash)
    {
        codeStr_ = codeStr;
        sourceHash_ = codeHash;
        अगर (sourceHash_.empty())
        {
            updateHash();
        }
        अन्यथा
        {
            isHashUpdated = true;
        }
    }

    व्योम updateHash(स्थिर अक्षर* hashStr = शून्य)
    {
        अगर (hashStr)
        {
            sourceHash_ = cv::String(hashStr);
            isHashUpdated = true;
            वापस;
        }
        uint64 hash = 0;
        चयन (kind_)
        {
        हाल PROGRAM_SOURCE_CODE:
            अगर (sourceAddr_)
            {
                CV_Assert(codeStr_.empty());
                hash = crc64(sourceAddr_, source_आकार); // static storage
            }
            अन्यथा
            {
                CV_Assert(!codeStr_.empty());
                hash = crc64((uchar*)codeStr_.c_str(), codeStr_.size());
            }
            अवरोध;
        हाल PROGRAM_BINARIES:
        हाल PROGRAM_SPIR:
        हाल PROGRAM_SPIRV:
            hash = crc64(sourceAddr_, source_आकार);
            अवरोध;
        शेष:
            CV_Error(Error::StsInternal, "Internal error");
        }
        sourceHash_ = cv::format("%08jx", (uintmax_t)hash);
        isHashUpdated = true;
    }

    Impl(क्रमागत KIND kind,
            स्थिर String& module, स्थिर String& name,
            स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size,
            स्थिर cv::String& buildOptions = cv::String())
    {
        init(kind, module, name);

        sourceAddr_ = द्विग;
        source_आकार = size;

        buildOptions_ = buildOptions;
    }

    अटल ProgramSource fromSourceWithStaticLifetime(स्थिर String& module, स्थिर String& name,
            स्थिर अक्षर* sourceCodeStaticStr, स्थिर अक्षर* hashStaticStr,
            स्थिर cv::String& buildOptions)
    {
        ProgramSource परिणाम;
        परिणाम.p = नया Impl(PROGRAM_SOURCE_CODE, module, name,
                (स्थिर अचिन्हित अक्षर*)sourceCodeStaticStr, म_माप(sourceCodeStaticStr), buildOptions);
        परिणाम.p->updateHash(hashStaticStr);
        वापस परिणाम;
    }

    अटल ProgramSource from_द्विआधारी(स्थिर String& module, स्थिर String& name,
            स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size,
            स्थिर cv::String& buildOptions)
    {
        ProgramSource परिणाम;
        परिणाम.p = नया Impl(PROGRAM_BINARIES, module, name, द्विग, size, buildOptions);
        वापस परिणाम;
    }

    अटल ProgramSource fromSPIR(स्थिर String& module, स्थिर String& name,
            स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size,
            स्थिर cv::String& buildOptions)
    {
        ProgramSource परिणाम;
        परिणाम.p = नया Impl(PROGRAM_SPIR, module, name, द्विग, size, buildOptions);
        वापस परिणाम;
    }

    String module_;
    String name_;

    // TODO std::vector<ProgramSource> includes_;
    String codeStr_; // PROGRAM_SOURCE_CODE only

    स्थिर अचिन्हित अक्षर* sourceAddr_;
    माप_प्रकार source_आकार;

    cv::String buildOptions_;

    String sourceHash_;
    bool isHashUpdated;

    मित्र काष्ठा Program::Impl;
    मित्र काष्ठा internal::ProgramEntry;
    मित्र काष्ठा Context::Impl;
};


ProgramSource::ProgramSource() CV_NOEXCEPT
{
    p = 0;
}

ProgramSource::ProgramSource(स्थिर String& module, स्थिर String& name, स्थिर String& codeStr, स्थिर String& codeHash)
{
    p = नया Impl(module, name, codeStr, codeHash);
}

ProgramSource::ProgramSource(स्थिर अक्षर* prog)
{
    p = नया Impl(prog);
}

ProgramSource::ProgramSource(स्थिर String& prog)
{
    p = नया Impl(prog);
}

ProgramSource::~ProgramSource()
{
    अगर(p)
        p->release();
}

ProgramSource::ProgramSource(स्थिर ProgramSource& prog)
{
    p = prog.p;
    अगर(p)
        p->addref();
}

ProgramSource& ProgramSource::चालक = (स्थिर ProgramSource& prog)
{
    Impl* newp = (Impl*)prog.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

ProgramSource::ProgramSource(ProgramSource&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

ProgramSource& ProgramSource::चालक = (ProgramSource&& prog) CV_NOEXCEPT
{
    अगर (यह != &prog) {
        अगर(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    वापस *यह;
}

स्थिर String& ProgramSource::source() स्थिर
{
    CV_Assert(p);
    CV_Assert(p->kind_ == Impl::PROGRAM_SOURCE_CODE);
    CV_Assert(p->sourceAddr_ == शून्य); // method returns reference - can't construct temporary object
    वापस p->codeStr_;
}

ProgramSource::hash_t ProgramSource::hash() स्थिर
{
    CV_Error(Error::StsNotImplemented, "Removed method: ProgramSource::hash()");
}

ProgramSource ProgramSource::from_द्विआधारी(स्थिर String& module, स्थिर String& name,
        स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size,
        स्थिर cv::String& buildOptions)
{
    CV_Assert(द्विग);
    CV_Assert(size > 0);
    वापस Impl::from_द्विआधारी(module, name, द्विग, size, buildOptions);
}

ProgramSource ProgramSource::fromSPIR(स्थिर String& module, स्थिर String& name,
        स्थिर अचिन्हित अक्षर* द्विग, स्थिर माप_प्रकार size,
        स्थिर cv::String& buildOptions)
{
    CV_Assert(द्विग);
    CV_Assert(size > 0);
    वापस Impl::from_द्विआधारी(module, name, द्विग, size, buildOptions);
}


internal::ProgramEntry::चालक ProgramSource&() स्थिर
{
    अगर (यह->pProgramSource == शून्य)
    {
        cv::AutoLock lock(cv::getInitializationMutex());
        अगर (यह->pProgramSource == शून्य)
        {
            ProgramSource ps = ProgramSource::Impl::fromSourceWithStaticLifetime(यह->module, यह->name, यह->programCode, यह->programHash, cv::String());
            ProgramSource* ptr = नया ProgramSource(ps);
            const_cast<ProgramEntry*>(यह)->pProgramSource = ptr;
        }
    }
    वापस *यह->pProgramSource;
}



/////////////////////////////////////////// Program /////////////////////////////////////////////

अटल
cv::String joinBuildOptions(स्थिर cv::String& a, स्थिर cv::String& b)
{
    अगर (b.empty())
        वापस a;
    अगर (a.empty())
        वापस b;
    अगर (b[0] == ' ')
        वापस a + b;
    वापस a + (cv::String(" ") + b);
}

काष्ठा Program::Impl
{
    IMPLEMENT_REFCOUNTABLE();

    Impl(स्थिर ProgramSource& src,
         स्थिर String& _buildflags, String& errmsg) :
         refcount(1),
         handle(शून्य),
         buildflags(_buildflags)
    {
        स्थिर ProgramSource::Impl* src_ = src.getImpl();
        CV_Assert(src_);
        sourceModule_ = src_->module_;
        sourceName_ = src_->name_;
        स्थिर Context ctx = Context::getDefault();
        Device device = ctx.device(0);
        अगर (ctx.ptr() == शून्य || device.ptr() == शून्य)
            वापस;
        buildflags = joinBuildOptions(buildflags, src_->buildOptions_);
        अगर (src.getImpl()->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            अगर (device.isAMD())
                buildflags = joinBuildOptions(buildflags, " -D AMD_DEVICE");
            अन्यथा अगर (device.isIntel())
                buildflags = joinBuildOptions(buildflags, " -D INTEL_DEVICE");
            स्थिर String param_buildExtraOptions = getBuildExtraOptions();
            अगर (!param_buildExtraOptions.empty())
                buildflags = joinBuildOptions(buildflags, param_buildExtraOptions);
        }
#अगर CV_OPENCL_SHOW_BUILD_OPTIONS
        CV_LOG_INFO(शून्य, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' options:" << buildflags);
#पूर्ण_अगर
        compile(ctx, src_, errmsg);
#अगर CV_OPENCL_SHOW_BUILD_KERNELS
        अगर (handle)
        {
            माप_प्रकार retsz = 0;
            अक्षर kernels_बफर[4096] = {0};
            cl_int परिणाम = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, माप(kernels_बफर), &kernels_बफर[0], &retsz);
            CV_OCL_DBG_CHECK_परिणाम(परिणाम, cv::format("clGetProgramInfo(CL_PROGRAM_KERNEL_NAMES: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            अगर (परिणाम == CL_SUCCESS && retsz < माप(kernels_बफर))
            {
                kernels_बफर[retsz] = 0;
                CV_LOG_INFO(शून्य, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' kernels: '" << kernels_बफर << "'");
            }
            अन्यथा
            {
                CV_LOG_ERROR(शून्य, "OpenCL program '" << sourceModule_ << "/" << sourceName_ << "' can't retrieve kernel names!");
            }
        }
#पूर्ण_अगर
    }

    bool compile(स्थिर Context& ctx, स्थिर ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);

        // We don't cache OpenCL binaries
        अगर (src_->kind_ == ProgramSource::Impl::PROGRAM_BINARIES)
        {
            CV_LOG_VERBOSE(शून्य, 0, "Load program binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            bool isLoaded = create_From_द्विआधारी(ctx, src_->sourceAddr_, src_->source_आकार, errmsg);
            वापस isLoaded;
        }
        वापस compileWithCache(ctx, src_, errmsg);
    }

    bool compileWithCache(स्थिर Context& ctx, स्थिर ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(ctx.getImpl());
        CV_Assert(src_);
        CV_Assert(src_->kind_ != ProgramSource::Impl::PROGRAM_BINARIES);

#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
        OpenCLBinaryCacheConfigurator& config = OpenCLBinaryCacheConfigurator::getSingletonInstance();
        स्थिर std::string base_dir = config.prepareCacheDirectoryForContext(
                ctx.getImpl()->getPrefixString(),
                ctx.getImpl()->getPrefixBase()
        );
        स्थिर String& hash_str = src_->sourceHash_;
        cv::String fname;
        अगर (!base_dir.empty() && !src_->module_.empty() && !src_->name_.empty())
        {
            CV_Assert(!hash_str.empty());
            fname = src_->module_ + "--" + src_->name_ + "_" + hash_str + ".bin";
            fname = utils::fs::join(base_dir, fname);
        }
        स्थिर cv::Ptr<utils::fs::FileLock> fileLock = config.cache_lock_; // can be empty
        अगर (!fname.empty() && CV_OPENCL_CACHE_ENABLE)
        {
            प्रयास
            {
                std::vector<अक्षर> द्विआधारी_Buf;
                bool res = false;
                {
                    cv::utils::optional_shared_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.लो());
                    द्विआधारी_Program_File file(fname, hash_str.c_str());
                    res = file.पढ़ो(buildflags, द्विआधारी_Buf);
                }
                अगर (res)
                {
                    CV_Assert(!द्विआधारी_Buf.empty());
                    CV_LOG_VERBOSE(शून्य, 0, "Load program binary from cache: " << src_->module_.c_str() << "/" << src_->name_.c_str());
                    bool isLoaded = create_From_द्विआधारी(ctx, द्विआधारी_Buf, errmsg);
                    अगर (isLoaded)
                        वापस true;
                }
            }
            पकड़ो (स्थिर cv::Exception& e)
            {
                CV_UNUSED(e);
                CV_LOG_VERBOSE(शून्य, 0, "Can't load OpenCL binary: " + fname << std::प_पूर्ण << e.what());
            }
            पकड़ो (...)
            {
                CV_LOG_VERBOSE(शून्य, 0, "Can't load OpenCL binary: " + fname);
            }
        }
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
        CV_Assert(handle == शून्य);
        अगर (src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE)
        {
            अगर (!buildFromSources(ctx, src_, errmsg))
            {
                वापस false;
            }
        }
        अन्यथा अगर (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIR)
        {
            buildflags = joinBuildOptions(buildflags, " -x spir");
            अगर ((cv::String(" ") + buildflags).find(" -spir-std=") == cv::String::npos)
            {
                buildflags = joinBuildOptions(buildflags, " -spir-std=1.2");
            }
            CV_LOG_VERBOSE(शून्य, 0, "Load program SPIR binary... " << src_->module_.c_str() << "/" << src_->name_.c_str());
            bool isLoaded = create_From_द्विआधारी(ctx, src_->sourceAddr_, src_->source_आकार, errmsg);
            अगर (!isLoaded)
                वापस false;
        }
        अन्यथा अगर (src_->kind_ == ProgramSource::Impl::PROGRAM_SPIRV)
        {
            CV_Error(Error::StsNotImplemented, "OpenCL: SPIR-V is not supported");
        }
        अन्यथा
        {
            CV_Error(Error::StsInternal, "Internal error");
        }
        CV_Assert(handle != शून्य);
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
        अगर (!fname.empty() && CV_OPENCL_CACHE_WRITE)
        {
            प्रयास
            {
                std::vector<अक्षर> द्विआधारी_Buf;
                get_Program_द्विआधारी(द्विआधारी_Buf);
                {
                    cv::utils::optional_lock_guard<cv::utils::fs::FileLock> lock_fs(fileLock.लो());
                    द्विआधारी_Program_File file(fname, hash_str.c_str());
                    file.ग_लिखो(buildflags, द्विआधारी_Buf);
                }
            }
            पकड़ो (स्थिर cv::Exception& e)
            {
                CV_LOG_WARNING(शून्य, "Can't save OpenCL binary into cache: " + fname << std::प_पूर्ण << e.what());
            }
            पकड़ो (...)
            {
                CV_LOG_WARNING(शून्य, "Can't save OpenCL binary into cache: " + fname);
            }
        }
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
#अगर CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS
        अगर (CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS_मूल्य)
        {
            std::vector<अक्षर> द्विआधारी_Buf;
            get_Program_द्विआधारी(द्विआधारी_Buf);
            अगर (!द्विआधारी_Buf.empty())
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = शून्य;
                create_From_द्विआधारी(ctx, द्विआधारी_Buf, errmsg);
            }
        }
#पूर्ण_अगर
        वापस handle != शून्य;
    }

    व्योम dumpBuildLog_(cl_int परिणाम, स्थिर cl_device_id* deviceList, String& errmsg)
    {
        Auto_बफर<अक्षर, 4096> बफर; बफर[0] = 0;

        माप_प्रकार retsz = 0;
        cl_int log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                                  CL_PROGRAM_BUILD_LOG, 0, 0, &retsz);
        अगर (log_retval == CL_SUCCESS && retsz > 1)
        {
            बफर.resize(retsz + 16);
            log_retval = clGetProgramBuildInfo(handle, deviceList[0],
                                               CL_PROGRAM_BUILD_LOG, retsz+1, बफर.तथ्य(), &retsz);
            अगर (log_retval == CL_SUCCESS)
            {
                अगर (retsz < बफर.size())
                    बफर[retsz] = 0;
                अन्यथा
                    बफर[बफर.size() - 1] = 0;
            }
            अन्यथा
            {
                बफर[0] = 0;
            }
        }

        errmsg = String(बफर.तथ्य());
        म_लिखो("OpenCL program build log: %s/%s\nStatus %d: %s\n%s\n%s\n",
                sourceModule_.c_str(), sourceName_.c_str(),
                परिणाम, getOpenCLErrorString(परिणाम),
                buildflags.c_str(), errmsg.c_str());
        ख_साफ(मानक_निकास);
    }

    bool buildFromSources(स्थिर Context& ctx, स्थिर ProgramSource::Impl* src_, String& errmsg)
    {
        CV_Assert(src_);
        CV_Assert(src_->kind_ == ProgramSource::Impl::PROGRAM_SOURCE_CODE);
        CV_Assert(handle == शून्य);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE(cv::format("Build OpenCL program: %s/%s %s options: %s",
                sourceModule_.c_str(), sourceName_.c_str(),
                src_->sourceHash_.c_str(), buildflags.c_str()).c_str());

        CV_LOG_VERBOSE(शून्य, 0, "Compile... " << sourceModule_.c_str() << "/" << sourceName_.c_str());

        स्थिर अक्षर* srcptr = src_->sourceAddr_ ? ((स्थिर अक्षर*)src_->sourceAddr_) : src_->codeStr_.c_str();
        माप_प्रकार srclen = src_->sourceAddr_ ? src_->source_आकार : src_->codeStr_.size();
        CV_Assert(srcptr != शून्य);
        CV_Assert(srclen > 0);

        cl_int retval = 0;

        handle = clCreateProgramWithSource((cl_context)ctx.ptr(), 1, &srcptr, &srclen, &retval);
        CV_OCL_DBG_CHECK_परिणाम(retval, "clCreateProgramWithSource");
        CV_Assert(handle || retval != CL_SUCCESS);
        अगर (handle && retval == CL_SUCCESS)
        {
            माप_प्रकार n = ctx.ndevices();
            Auto_बफर<cl_device_id, 4> deviceListBuf(n + 1);
            cl_device_id* deviceList = deviceListBuf.तथ्य();
            क्रम (माप_प्रकार i = 0; i < n; i++)
            {
                deviceList[i] = (cl_device_id)(ctx.device(i).ptr());
            }

            retval = clBuildProgram(handle, (cl_uint)n, deviceList, buildflags.c_str(), 0, 0);
            CV_OCL_TRACE_CHECK_परिणाम(/*don't throw: retval*/CL_SUCCESS, cv::format("clBuildProgram(source: %s)", buildflags.c_str()).c_str());
#अगर !CV_OPENCL_ALWAYS_SHOW_BUILD_LOG
            अगर (retval != CL_SUCCESS)
#पूर्ण_अगर
            {
                dumpBuildLog_(retval, deviceList, errmsg);

                // don't remove "retval != CL_SUCCESS" condition here:
                // it would break CV_OPENCL_ALWAYS_SHOW_BUILD_LOG mode
                अगर (retval != CL_SUCCESS && handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = शून्य;
                }
                अगर (retval != CL_SUCCESS &&
                    sourceName_ != "dummy"  // used for testing of compilation flags
                )
                {
                    onOpenCLKernelBuildError();
                }
            }
#अगर CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS
            अगर (handle && CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS_मूल्य)
            {
                CV_LOG_INFO(शून्य, "OpenCL: query kernel names (build from sources)...");
                माप_प्रकार retsz = 0;
                अक्षर kernels_बफर[4096] = {0};
                cl_int परिणाम = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, माप(kernels_बफर), &kernels_बफर[0], &retsz);
                अगर (retsz < माप(kernels_बफर))
                    kernels_बफर[retsz] = 0;
                अन्यथा
                    kernels_बफर[0] = 0;
                CV_LOG_INFO(शून्य, परिणाम << ": Kernels='" << kernels_बफर << "'");
            }
#पूर्ण_अगर
        }
        वापस handle != शून्य;
    }

    व्योम get_Program_द्विआधारी(std::vector<अक्षर>& buf)
    {
        CV_Assert(handle);
        माप_प्रकार sz = 0;
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_द्विआधारी_SIZES, माप(sz), &sz, शून्य));
        buf.resize(sz);
        uchar* ptr = (uchar*)&buf[0];
        CV_OCL_CHECK(clGetProgramInfo(handle, CL_PROGRAM_BINARIES, माप(ptr), &ptr, शून्य));
    }

    bool create_From_द्विआधारी(स्थिर Context& ctx, स्थिर std::vector<अक्षर>& buf, String& errmsg)
    {
        वापस create_From_द्विआधारी(ctx, (स्थिर अचिन्हित अक्षर*)&buf[0], buf.size(), errmsg);
    }

    bool create_From_द्विआधारी(स्थिर Context& ctx, स्थिर अचिन्हित अक्षर* द्विआधारी_Addr, स्थिर माप_प्रकार द्विआधारी_आकार, String& errmsg)
    {
        CV_Assert(handle == शून्य);
        CV_INSTRUMENT_REGION_OPENCL_COMPILE("Load OpenCL program");
        CV_LOG_VERBOSE(शून्य, 0, "Load from binary... (" << द्विआधारी_आकार << " bytes)");

        CV_Assert(द्विआधारी_आकार > 0);

        माप_प्रकार ndevices = (पूर्णांक)ctx.ndevices();
        Auto_बफर<cl_device_id> devices_(ndevices);
        Auto_बफर<स्थिर uchar*> द्विआधारी_Ptrs(ndevices);
        Auto_बफर<माप_प्रकार> द्विआधारी_Sizes(ndevices);

        cl_device_id* devices = devices_.तथ्य();
        स्थिर uchar** द्विआधारी_Ptrs_2 = द्विआधारी_Ptrs.तथ्य();
        माप_प्रकार* द्विआधारी_Sizes_2 = द्विआधारी_Sizes.तथ्य();
        क्रम (माप_प्रकार i = 0; i < ndevices; i++)
        {
            devices[i] = (cl_device_id)ctx.device(i).ptr();
            द्विआधारी_Ptrs_2[i] = द्विआधारी_Addr;
            द्विआधारी_Sizes_2[i] = द्विआधारी_आकार;
        }

        cl_int परिणाम = 0;
        handle = cl_Create_Program_With_द्विआधारी((cl_context)ctx.ptr(), (cl_uint)ndevices, devices_.तथ्य(),
                                           द्विआधारी_Sizes_2, द्विआधारी_Ptrs_2, शून्य, &परिणाम);
        अगर (परिणाम != CL_SUCCESS)
        {
            CV_LOG_ERROR(शून्य, CV_OCL_API_ERROR_संदेश(परिणाम, "clCreateProgramWithBinary"));
            अगर (handle)
            {
                CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                handle = शून्य;
            }
        }
        अगर (!handle)
        {
            वापस false;
        }
        // call clBuildProgram()
        {
            परिणाम = clBuildProgram(handle, (cl_uint)ndevices, devices_.तथ्य(), buildflags.c_str(), 0, 0);
            CV_OCL_DBG_CHECK_परिणाम(परिणाम, cv::format("clBuildProgram(binary: %s/%s)", sourceModule_.c_str(), sourceName_.c_str()).c_str());
            अगर (परिणाम != CL_SUCCESS)
            {
                dumpBuildLog_(परिणाम, devices, errmsg);
                अगर (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = शून्य;
                }
                वापस false;
            }
        }
        // check build status
        {
            cl_build_status build_status = CL_BUILD_NONE;
            माप_प्रकार retsz = 0;
            CV_OCL_DBG_CHECK(परिणाम = clGetProgramBuildInfo(handle, devices[0], CL_PROGRAM_BUILD_STATUS,
                    माप(build_status), &build_status, &retsz));
            अगर (परिणाम == CL_SUCCESS)
            {
                अगर (build_status == CL_BUILD_SUCCESS)
                {
                    वापस true;
                }
                अन्यथा
                {
                    CV_LOG_WARNING(शून्य, "clGetProgramBuildInfo() returns " << build_status);
                    वापस false;
                }
            }
            अन्यथा
            {
                CV_LOG_ERROR(शून्य, CV_OCL_API_ERROR_संदेश(परिणाम, "clGetProgramBuildInfo()"));
                अगर (handle)
                {
                    CV_OCL_DBG_CHECK(clReleaseProgram(handle));
                    handle = शून्य;
                }
            }
        }
#अगर CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS
        अगर (handle && CV_OPENCL_VALIDATE_द्विआधारी_PROGRAMS_मूल्य)
        {
            CV_LOG_INFO(शून्य, "OpenCL: query kernel names (binary)...");
            माप_प्रकार retsz = 0;
            अक्षर kernels_बफर[4096] = {0};
            परिणाम = clGetProgramInfo(handle, CL_PROGRAM_KERNEL_NAMES, माप(kernels_बफर), &kernels_बफर[0], &retsz);
            अगर (retsz < माप(kernels_बफर))
                kernels_बफर[retsz] = 0;
            अन्यथा
                kernels_बफर[0] = 0;
            CV_LOG_INFO(शून्य, परिणाम << ": Kernels='" << kernels_बफर << "'");
        }
#पूर्ण_अगर
        वापस handle != शून्य;
    }

    ~Impl()
    {
        अगर( handle )
        {
#अगर_घोषित _WIN32
            अगर (!cv::__termination)
#पूर्ण_अगर
            {
                clReleaseProgram(handle);
            }
            handle = शून्य;
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

Program::Program(स्थिर ProgramSource& src,
        स्थिर String& buildflags, String& errmsg)
{
    p = 0;
    create(src, buildflags, errmsg);
}

Program::Program(स्थिर Program& prog)
{
    p = prog.p;
    अगर(p)
        p->addref();
}

Program& Program::चालक = (स्थिर Program& prog)
{
    Impl* newp = (Impl*)prog.p;
    अगर(newp)
        newp->addref();
    अगर(p)
        p->release();
    p = newp;
    वापस *यह;
}

Program::Program(Program&& prog) CV_NOEXCEPT
{
    p = prog.p;
    prog.p = nullptr;
}

Program& Program::चालक = (Program&& prog) CV_NOEXCEPT
{
    अगर (यह != &prog) {
        अगर(p)
            p->release();
        p = prog.p;
        prog.p = nullptr;
    }
    वापस *यह;
}

Program::~Program()
{
    अगर(p)
        p->release();
}

bool Program::create(स्थिर ProgramSource& src,
            स्थिर String& buildflags, String& errmsg)
{
    अगर(p)
    {
        p->release();
        p = शून्य;
    }
    p = नया Impl(src, buildflags, errmsg);
    अगर(!p->handle)
    {
        p->release();
        p = 0;
    }
    वापस p != 0;
}

व्योम* Program::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

#अगर_अघोषित OPENCV_REMOVE_DEPRECATED_API
स्थिर ProgramSource& Program::source() स्थिर
{
    CV_Error(Error::StsNotImplemented, "Removed API");
}

bool Program::पढ़ो(स्थिर String& bin, स्थिर String& buildflags)
{
    CV_UNUSED(bin); CV_UNUSED(buildflags);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

bool Program::ग_लिखो(String& bin) स्थिर
{
    CV_UNUSED(bin);
    CV_Error(Error::StsNotImplemented, "Removed API");
}

String Program::getPrefix() स्थिर
{
    अगर(!p)
        वापस String();
    Context::Impl* ctx_ = Context::getDefault().getImpl();
    CV_Assert(ctx_);
    वापस cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), p->buildflags.c_str());
}

String Program::getPrefix(स्थिर String& buildflags)
{
        Context::Impl* ctx_ = Context::getDefault().getImpl();
        CV_Assert(ctx_);
        वापस cv::format("opencl=%s\nbuildflags=%s", ctx_->getPrefixString().c_str(), buildflags.c_str());
}
#पूर्ण_अगर // OPENCV_REMOVE_DEPRECATED_API

व्योम Program::get_द्विआधारी(std::vector<अक्षर>& द्विग) स्थिर
{
    CV_Assert(p && "Empty program");
    p->get_Program_द्विआधारी(द्विग);
}

Program Context::Impl::getProg(स्थिर ProgramSource& src,
                               स्थिर String& buildflags, String& errmsg)
{
    माप_प्रकार limit = get_Program_गिनती_Limit();
    स्थिर ProgramSource::Impl* src_ = src.getImpl();
    CV_Assert(src_);
    String कुंजी = cv::format("module=%s name=%s codehash=%s\nopencl=%s\nbuildflags=%s",
            src_->module_.c_str(), src_->name_.c_str(), src_->sourceHash_.c_str(),
            getPrefixString().c_str(),
            buildflags.c_str());
    {
        cv::AutoLock lock(program_cache_mutex);
        phash_t::iterator it = phash.find(कुंजी);
        अगर (it != phash.end())
        {
            // TODO LRU cache
            CacheList::iterator i = std::find(cacheList.begin(), cacheList.end(), कुंजी);
            अगर (i != cacheList.end() && i != cacheList.begin())
            {
                cacheList.erase(i);
                cacheList.push_front(कुंजी);
            }
            वापस it->second;
        }
        { // cleanup program cache
            माप_प्रकार sz = phash.size();
            अगर (limit > 0 && sz >= limit)
            {
                अटल bool warningFlag = false;
                अगर (!warningFlag)
                {
                    म_लिखो("\nWARNING: OpenCV-OpenCL:\n"
                        "    In-memory cache for OpenCL programs is full, older programs will be unloaded.\n"
                        "    You can change cache size via OPENCV_OPENCL_PROGRAM_CACHE environment variable\n\n");
                    warningFlag = true;
                }
                जबतक (!cacheList.empty())
                {
                    माप_प्रकार c = phash.erase(cacheList.back());
                    cacheList.pop_back();
                    अगर (c != 0)
                        अवरोध;
                }
            }
        }
    }
    Program prog(src, buildflags, errmsg);
    // Cache result of build failures too (to prevent unnecessary compiler invocations)
    {
        cv::AutoLock lock(program_cache_mutex);
        phash.insert(std::pair<std::string, Program>(कुंजी, prog));
        cacheList.push_front(कुंजी);
    }
    वापस prog;
}


//////////////////////////////////////////// OpenCLAllocator //////////////////////////////////////////////////

ढाँचा<typename T>
श्रेणी OpenCLBufferPool
{
रक्षित:
    ~OpenCLBufferPool() { }
खुला:
    भव T allocate(माप_प्रकार size) = 0;
    भव व्योम release(T बफर) = 0;
};

ढाँचा <typename Derived, typename बफर_Entry, typename T>
श्रेणी OpenCLBufferPoolBaseImpl : खुला बफर_Pool_Controller, खुला OpenCLBufferPool<T>
{
निजी:
    अंतरभूत Derived& derived() { वापस *static_cast<Derived*>(यह); }
रक्षित:
    Mutex mutex_;

    माप_प्रकार current_Reserved_आकार;
    माप_प्रकार max_Reserved_आकार;

    std::list<बफर_Entry> allocatedEntries_; // Allocated and used entries
    std::list<बफर_Entry> reservedEntries_; // LRU order. Allocated, but not used entries

    // synchronized
    bool _findAndRemoveEntryFromAllocatedList(CV_OUT बफर_Entry& entry, T बफर)
    {
        typename std::list<बफर_Entry>::iterator i = allocatedEntries_.begin();
        क्रम (; i != allocatedEntries_.end(); ++i)
        {
            बफर_Entry& e = *i;
            अगर (e.cl_बफर_2 == बफर)
            {
                entry = e;
                allocatedEntries_.erase(i);
                वापस true;
            }
        }
        वापस false;
    }

    // synchronized
    bool _findAndRemoveEntryFromReservedList(CV_OUT बफर_Entry& entry, स्थिर माप_प्रकार size)
    {
        अगर (reservedEntries_.empty())
            वापस false;
        typename std::list<बफर_Entry>::iterator i = reservedEntries_.begin();
        typename std::list<बफर_Entry>::iterator परिणाम_pos = reservedEntries_.end();
        बफर_Entry परिणाम;
        माप_प्रकार minDiff = (माप_प्रकार)(-1);
        क्रम (; i != reservedEntries_.end(); ++i)
        {
            बफर_Entry& e = *i;
            अगर (e.capacity_ >= size)
            {
                माप_प्रकार diff = e.capacity_ - size;
                अगर (diff < std::max((माप_प्रकार)4096, size / 8) && (परिणाम_pos == reservedEntries_.end() || diff < minDiff))
                {
                    minDiff = diff;
                    परिणाम_pos = i;
                    परिणाम = e;
                    अगर (diff == 0)
                        अवरोध;
                }
            }
        }
        अगर (परिणाम_pos != reservedEntries_.end())
        {
            //CV_DbgAssert(result == *result_pos);
            reservedEntries_.erase(परिणाम_pos);
            entry = परिणाम;
            current_Reserved_आकार -= entry.capacity_;
            allocatedEntries_.push_back(entry);
            वापस true;
        }
        वापस false;
    }

    // synchronized
    व्योम check_आकार_Of_Reserved_Entries()
    {
        जबतक (current_Reserved_आकार > max_Reserved_आकार)
        {
            CV_DbgAssert(!reservedEntries_.empty());
            स्थिर बफर_Entry& entry = reservedEntries_.back();
            CV_DbgAssert(current_Reserved_आकार >= entry.capacity_);
            current_Reserved_आकार -= entry.capacity_;
            derived().release_बफर_Entry(entry);
            reservedEntries_.pop_back();
        }
    }

    अंतरभूत माप_प्रकार _allocationGranularity(माप_प्रकार size)
    {
        // heuristic values
        अगर (size < 1024*1024)
            वापस 4096;  // don't work with buffers smaller than 4Kb (hidden allocation overhead issue)
        अन्यथा अगर (size < 16*1024*1024)
            वापस 64*1024;
        अन्यथा
            वापस 1024*1024;
    }

खुला:
    OpenCLBufferPoolBaseImpl()
        : current_Reserved_आकार(0),
          max_Reserved_आकार(0)
    {
        // nothing
    }
    भव ~OpenCLBufferPoolBaseImpl()
    {
        freeAllReservedBuffers();
        CV_Assert(reservedEntries_.empty());
    }
खुला:
    भव T allocate(माप_प्रकार size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        बफर_Entry entry;
        अगर (max_Reserved_आकार > 0 && _findAndRemoveEntryFromReservedList(entry, size))
        {
            CV_DbgAssert(size <= entry.capacity_);
            LOG_बफर_POOL("Reuse reserved buffer: %p\n", entry.cl_बफर_2);
        }
        अन्यथा
        {
            derived().allocate_बफर_Entry(entry, size);
        }
        वापस entry.cl_बफर_2;
    }
    भव व्योम release(T बफर) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        बफर_Entry entry;
        CV_Assert(_findAndRemoveEntryFromAllocatedList(entry, बफर));
        अगर (max_Reserved_आकार == 0 || entry.capacity_ > max_Reserved_आकार / 8)
        {
            derived().release_बफर_Entry(entry);
        }
        अन्यथा
        {
            reservedEntries_.push_front(entry);
            current_Reserved_आकार += entry.capacity_;
            check_आकार_Of_Reserved_Entries();
        }
    }

    भव माप_प्रकार get_Reserved_आकार() स्थिर CV_OVERRIDE { वापस current_Reserved_आकार; }
    भव माप_प्रकार get_Max_Reserved_आकार() स्थिर CV_OVERRIDE { वापस max_Reserved_आकार; }
    भव व्योम set_Max_Reserved_आकार(माप_प्रकार size) CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        माप_प्रकार old_Max_Reserved_आकार = max_Reserved_आकार;
        max_Reserved_आकार = size;
        अगर (max_Reserved_आकार < old_Max_Reserved_आकार)
        {
            typename std::list<बफर_Entry>::iterator i = reservedEntries_.begin();
            क्रम (; i != reservedEntries_.end();)
            {
                स्थिर बफर_Entry& entry = *i;
                अगर (entry.capacity_ > max_Reserved_आकार / 8)
                {
                    CV_DbgAssert(current_Reserved_आकार >= entry.capacity_);
                    current_Reserved_आकार -= entry.capacity_;
                    derived().release_बफर_Entry(entry);
                    i = reservedEntries_.erase(i);
                    जारी;
                }
                ++i;
            }
            check_आकार_Of_Reserved_Entries();
        }
    }
    भव व्योम freeAllReservedBuffers() CV_OVERRIDE
    {
        AutoLock locker(mutex_);
        typename std::list<बफर_Entry>::const_iterator i = reservedEntries_.begin();
        क्रम (; i != reservedEntries_.end(); ++i)
        {
            स्थिर बफर_Entry& entry = *i;
            derived().release_बफर_Entry(entry);
        }
        reservedEntries_.सफा();
        current_Reserved_आकार = 0;
    }
};

काष्ठा CLBufferEntry
{
    cl_mem cl_बफर_2;
    माप_प्रकार capacity_;
    CLBufferEntry() : cl_बफर_2((cl_mem)शून्य), capacity_(0) { }
};

श्रेणी OpenCLBufferPoolImpl CV_FINAL : खुला OpenCLBufferPoolBaseImpl<OpenCLBufferPoolImpl, CLBufferEntry, cl_mem>
{
खुला:
    प्रकार काष्ठा CLBufferEntry बफर_Entry;
रक्षित:
    पूर्णांक createFlags_;
खुला:
    OpenCLBufferPoolImpl(पूर्णांक createFlags = 0)
        : createFlags_(createFlags)
    {
    }

    व्योम allocate_बफर_Entry(बफर_Entry& entry, माप_प्रकार size)
    {
        CV_DbgAssert(entry.cl_बफर_2 == शून्य);
        entry.capacity_ = align_आकार(size, (पूर्णांक)_allocationGranularity(size));
        Context& ctx = Context::getDefault();
        cl_int retval = CL_SUCCESS;
        entry.cl_बफर_2 = cl_Create_बफर((cl_context)ctx.ptr(), CL_MEM_READ_WRITE|createFlags_, entry.capacity_, 0, &retval);
        CV_OCL_CHECK_परिणाम(retval, cv::format("clCreateBuffer(capacity=%lld) => %p", (दीर्घ दीर्घ पूर्णांक)entry.capacity_, (व्योम*)entry.cl_बफर_2).c_str());
        CV_Assert(entry.cl_बफर_2 != शून्य);
        अगर(retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_बफर_POOL("OpenCL allocate %lld (0x%llx) bytes: %p\n",
                (दीर्घ दीर्घ)entry.capacity_, (दीर्घ दीर्घ)entry.capacity_, entry.cl_बफर_2);
        allocatedEntries_.push_back(entry);
    }

    व्योम release_बफर_Entry(स्थिर बफर_Entry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.cl_बफर_2 != शून्य);
        LOG_बफर_POOL("OpenCL release buffer: %p, %lld (0x%llx) bytes\n",
                entry.cl_बफर_2, (दीर्घ दीर्घ)entry.capacity_, (दीर्घ दीर्घ)entry.capacity_);
        CV_OCL_DBG_CHECK(clReleaseMemObject(entry.cl_बफर_2));
    }
};

#अगर_घोषित HAVE_OPENCL_SVM
काष्ठा CLSVMBufferEntry
{
    व्योम* cl_बफर_2;
    माप_प्रकार capacity_;
    CLSVMBufferEntry() : cl_बफर_2(शून्य), capacity_(0) { }
};
श्रेणी OpenCLSVMBufferPoolImpl CV_FINAL : खुला OpenCLBufferPoolBaseImpl<OpenCLSVMBufferPoolImpl, CLSVMBufferEntry, व्योम*>
{
खुला:
    प्रकार काष्ठा CLSVMBufferEntry बफर_Entry;
खुला:
    OpenCLSVMBufferPoolImpl()
    {
    }

    व्योम allocate_बफर_Entry(बफर_Entry& entry, माप_प्रकार size)
    {
        CV_DbgAssert(entry.cl_बफर_2 == शून्य);
        entry.capacity_ = align_आकार(size, (पूर्णांक)_allocationGranularity(size));

        Context& ctx = Context::getDefault();
        स्थिर svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        bool is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();
        cl_svm_mem_flags memFlags = CL_MEM_READ_WRITE |
                (is_Fine_Grain_बफर ? CL_MEM_SVM_FINE_GRAIN_बफर : 0);

        स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());

        CV_OPENCL_SVM_TRACE_P("clSVMAlloc: %d\n", (पूर्णांक)entry.capacity_);
        व्योम *buf = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, entry.capacity_, 0);
        CV_Assert(buf);

        entry.cl_बफर_2 = buf;
        {
            CV_IMPL_ADD(CV_IMPL_OCL);
        }
        LOG_बफर_POOL("OpenCL SVM allocate %lld (0x%llx) bytes: %p\n",
                (दीर्घ दीर्घ)entry.capacity_, (दीर्घ दीर्घ)entry.capacity_, entry.cl_बफर_2);
        allocatedEntries_.push_back(entry);
    }

    व्योम release_बफर_Entry(स्थिर बफर_Entry& entry)
    {
        CV_Assert(entry.capacity_ != 0);
        CV_Assert(entry.cl_बफर_2 != शून्य);
        LOG_बफर_POOL("OpenCL release SVM buffer: %p, %lld (0x%llx) bytes\n",
                entry.cl_बफर_2, (दीर्घ दीर्घ)entry.capacity_, (दीर्घ दीर्घ)entry.capacity_);
        Context& ctx = Context::getDefault();
        स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
        CV_DbgAssert(svmFns->isValid());
        CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n",  entry.cl_बफर_2);
        svmFns->fn_clSVMFree((cl_context)ctx.ptr(), entry.cl_बफर_2);
    }
};
#पूर्ण_अगर



ढाँचा <bool readAccess, bool writeAccess>
श्रेणी Aligned_तथ्य_Ptr
{
रक्षित:
    स्थिर माप_प्रकार आकार_3;
    uchar* स्थिर originPtr_;
    स्थिर माप_प्रकार alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;

खुला:
    Aligned_तथ्य_Ptr(uchar* ptr, माप_प्रकार size, माप_प्रकार alignment)
        : आकार_3(size), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(शून्य)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr);
        अगर (((माप_प्रकार)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नया uchar[आकार_3 + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            अगर (readAccess)
            {
                स_नकल(ptr_, originPtr_, आकार_3);
            }
        }
    }

    uchar* getAlignedPtr() स्थिर
    {
        CV_DbgAssert(((माप_प्रकार)ptr_ & (alignment_ - 1)) == 0);
        वापस ptr_;
    }

    ~Aligned_तथ्य_Ptr()
    {
        अगर (allocatedPtr_)
        {
            अगर (writeAccess)
            {
                स_नकल(originPtr_, ptr_, आकार_3);
            }
            मिटाओ[] allocatedPtr_;
            allocatedPtr_ = शून्य;
        }
        ptr_ = शून्य;
    }
निजी:
    Aligned_तथ्य_Ptr(स्थिर Aligned_तथ्य_Ptr&); // disabled
    Aligned_तथ्य_Ptr& चालक=(स्थिर Aligned_तथ्य_Ptr&); // disabled
};

ढाँचा <bool readAccess, bool writeAccess>
श्रेणी Aligned_तथ्य_Ptr2_D
{
रक्षित:
    स्थिर माप_प्रकार आकार_3;
    uchar* स्थिर originPtr_;
    स्थिर माप_प्रकार alignment_;
    uchar* ptr_;
    uchar* allocatedPtr_;
    माप_प्रकार rows_;
    माप_प्रकार cols_;
    माप_प्रकार step_;

खुला:
    Aligned_तथ्य_Ptr2_D(uchar* ptr, माप_प्रकार rows, माप_प्रकार cols, माप_प्रकार step, माप_प्रकार alignment, माप_प्रकार extrabytes=0)
        : आकार_3(rows*step), originPtr_(ptr), alignment_(alignment), ptr_(ptr), allocatedPtr_(शून्य), rows_(rows), cols_(cols), step_(step)
    {
        CV_DbgAssert((alignment & (alignment - 1)) == 0); // check for 2^n
        CV_DbgAssert(!readAccess || ptr != शून्य);
        अगर (ptr == 0 || ((माप_प्रकार)ptr_ & (alignment - 1)) != 0)
        {
            allocatedPtr_ = नया uchar[आकार_3 + extrabytes + alignment - 1];
            ptr_ = (uchar*)(((uintptr_t)allocatedPtr_ + (alignment - 1)) & ~(alignment - 1));
            अगर (readAccess)
            {
                क्रम (माप_प्रकार i = 0; i < rows_; i++)
                    स_नकल(ptr_ + i*step_, originPtr_ + i*step_, cols_);
            }
        }
    }

    uchar* getAlignedPtr() स्थिर
    {
        CV_DbgAssert(((माप_प्रकार)ptr_ & (alignment_ - 1)) == 0);
        वापस ptr_;
    }

    ~Aligned_तथ्य_Ptr2_D()
    {
        अगर (allocatedPtr_)
        {
            अगर (writeAccess)
            {
                क्रम (माप_प्रकार i = 0; i < rows_; i++)
                    स_नकल(originPtr_ + i*step_, ptr_ + i*step_, cols_);
            }
            मिटाओ[] allocatedPtr_;
            allocatedPtr_ = शून्य;
        }
        ptr_ = शून्य;
    }
निजी:
    Aligned_तथ्य_Ptr2_D(स्थिर Aligned_तथ्य_Ptr2_D&); // disabled
    Aligned_तथ्य_Ptr2_D& चालक=(स्थिर Aligned_तथ्य_Ptr2_D&); // disabled
};

#अगर_अघोषित CV_OPENCL_तथ्य_PTR_ALIGNMENT
#घोषणा CV_OPENCL_तथ्य_PTR_ALIGNMENT 16
#पूर्ण_अगर


व्योम Context::Impl::__init_buffer_pools()
{
    बफर_Pool_2 = std::make_shared<OpenCLBufferPoolImpl>(0);
    OpenCLBufferPoolImpl& बफर_Pool_3 = *बफर_Pool_2.लो();
    बफर_Pool_Host_Ptr = std::make_shared<OpenCLBufferPoolImpl>(CL_MEM_ALLOC_HOST_PTR);
    OpenCLBufferPoolImpl& बफर_Pool_Host_Ptr_2 = *बफर_Pool_Host_Ptr.लो();

    माप_प्रकार default_Pool_आकार = ocl::Device::getDefault().isIntel() ? 1 << 27 : 0;
    माप_प्रकार pool_आकार = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_3.set_Max_Reserved_आकार(pool_आकार);
    माप_प्रकार pool_आकार_Host_Ptr = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_HOST_PTR_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_Host_Ptr_2.set_Max_Reserved_आकार(pool_आकार_Host_Ptr);

#अगर_घोषित HAVE_OPENCL_SVM
    बफर_Pool_SVM = std::make_shared<OpenCLSVMBufferPoolImpl>();
    OpenCLSVMBufferPoolImpl& बफर_Pool_SVM_2 = *बफर_Pool_SVM.लो();
    माप_प्रकार pool_आकार_SVM = utils::get_Configuration_Parameter_आकार_T("OPENCV_OPENCL_SVM_BUFFERPOOL_LIMIT", default_Pool_आकार);
    बफर_Pool_SVM_2.set_Max_Reserved_आकार(pool_आकार_SVM);
#पूर्ण_अगर

    CV_LOG_INFO(शून्य, "OpenCL: Initializing buffer pool for context@" << contextId << " with max capacity: poolSize=" << pool_आकार << " poolSizeHostPtr=" << pool_आकार_Host_Ptr);
}

श्रेणी OpenCLAllocator CV_FINAL : खुला MatAllocator
{
खुला:
    क्रमागत AllocatorFlags
    {
        ALLOCATOR_FLAGS_बफर_POOL_USED = 1 << 0,
        ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_USED = 1 << 1,
#अगर_घोषित HAVE_OPENCL_SVM
        ALLOCATOR_FLAGS_बफर_POOL_SVM_USED = 1 << 2,
#पूर्ण_अगर
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

    UMat_तथ्य* defaultAllocate(पूर्णांक dims, स्थिर पूर्णांक* sizes, पूर्णांक type, व्योम* तथ्य, माप_प्रकार* step,
            AccessFlag flags, UMatUsageFlags usageFlags) स्थिर
    {
        UMat_तथ्य* u = matStdAllocator->allocate(dims, sizes, type, तथ्य, step, flags, usageFlags);
        वापस u;
    }

    अटल bool isOpenCLMapForced()  // force clEnqueueMapBuffer / clEnqueueUnmapMemObject OpenCL API
    {
        अटल bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_MAPPING", false);
        वापस value;
    }
    अटल bool isOpenCLCopyingForced()  // force clEnqueueReadBuffer[Rect] / clEnqueueWriteBuffer[Rect] OpenCL API
    {
        अटल bool value = cv::utils::getConfigurationParameterBool("OPENCV_OPENCL_BUFFER_FORCE_COPYING", false);
        वापस value;
    }

    व्योम getBestFlags(स्थिर Context& ctx, AccessFlag /*flags*/, UMatUsageFlags usageFlags, पूर्णांक& createFlags, UMat_तथ्य::MemoryFlag& flags0) स्थिर
    {
        स्थिर Device& dev = ctx.device(0);
        createFlags = 0;
        अगर ((usageFlags & USAGE_ALLOCATE_HOST_MEMORY) != 0)
            createFlags |= CL_MEM_ALLOC_HOST_PTR;

        अगर (!isOpenCLCopyingForced() &&
            (isOpenCLMapForced() ||
                (dev.hostUnifiedMemory()
#अगर_अघोषित __APPLE__
                || dev.isIntel()
#पूर्ण_अगर
                )
            )
        )
            flags0 = static_cast<UMat_तथ्य::MemoryFlag>(0);
        अन्यथा
            flags0 = UMat_तथ्य::COPY_ON_MAP;
    }

    UMat_तथ्य* allocate(पूर्णांक dims, स्थिर पूर्णांक* sizes, पूर्णांक type,
                       व्योम* तथ्य, माप_प्रकार* step, AccessFlag flags, UMatUsageFlags usageFlags) स्थिर CV_OVERRIDE
    {
        अगर(!useOpenCL())
            वापस defaultAllocate(dims, sizes, type, तथ्य, step, flags, usageFlags);

        flushCleanupQueue();

        CV_Assert(तथ्य == 0);
        माप_प्रकार योग = CV_ELEM_आकार(type);
        क्रम( पूर्णांक i = dims-1; i >= 0; i-- )
        {
            अगर( step )
                step[i] = योग;
            योग *= sizes[i];
        }

        Context& ctx = Context::getDefault();
        अगर (!ctx.getImpl())
            वापस defaultAllocate(dims, sizes, type, तथ्य, step, flags, usageFlags);
        Context::Impl& ctxImpl = *ctx.getImpl();

        पूर्णांक createFlags = 0;
        UMat_तथ्य::MemoryFlag flags0 = static_cast<UMat_तथ्य::MemoryFlag>(0);
        getBestFlags(ctx, flags, usageFlags, createFlags, flags0);

        व्योम* handle = शून्य;
        पूर्णांक allocatorFlags = 0;

#अगर_घोषित HAVE_OPENCL_SVM
        स्थिर svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
        अगर (ctx.useSVM() && svm::useSVM(usageFlags) && !svmCaps.isNoSVMSupport())
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_SVM_USED;
            handle = ctxImpl.get_बफर_Pool_SVM().allocate(योग);

            // this property is constant, so single buffer pool can be used here
            bool is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();
            allocatorFlags |= is_Fine_Grain_बफर ? svm::OPENCL_SVM_FINE_GRAIN_बफर : svm::OPENCL_SVM_COARSE_GRAIN_बफर;
        }
        अन्यथा
#पूर्ण_अगर
        अगर (createFlags == 0)
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_USED;
            handle = ctxImpl.get_बफर_Pool().allocate(योग);
        }
        अन्यथा अगर (createFlags == CL_MEM_ALLOC_HOST_PTR)
        {
            allocatorFlags = ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_USED;
            handle = ctxImpl.get_बफर_Pool_Host_Ptr().allocate(योग);
        }
        अन्यथा
        {
            CV_Assert(handle != शून्य); // Unsupported, throw
        }

        अगर (!handle)
            वापस defaultAllocate(dims, sizes, type, तथ्य, step, flags, usageFlags);

        UMat_तथ्य* u = नया UMat_तथ्य(यह);
        u->तथ्य = 0;
        u->size = योग;
        u->handle = handle;
        u->flags = flags0;
        u->allocatorFlags_ = allocatorFlags;
        u->allocatorContext = std::static_pointer_cast<व्योम>(std::make_shared<ocl::Context>(ctx));
        CV_DbgAssert(!u->अस्थायी_UMat()); // for bufferPool.release() consistency in deallocate()
        u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        वापस u;
    }

    bool allocate(UMat_तथ्य* u, AccessFlag accessFlags, UMatUsageFlags usageFlags) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस false;

        flushCleanupQueue();

        UMat_तथ्य_Auto_Lock lock(u);

        अगर(u->handle == 0)
        {
            CV_Assert(u->origdata != 0);
            Context& ctx = Context::getDefault();
            पूर्णांक createFlags = 0;
            UMat_तथ्य::MemoryFlag flags0 = static_cast<UMat_तथ्य::MemoryFlag>(0);
            getBestFlags(ctx, accessFlags, usageFlags, createFlags, flags0);

            bool copyOnMap = (flags0 & UMat_तथ्य::COPY_ON_MAP) != 0;

            cl_context ctx_handle = (cl_context)ctx.ptr();
            पूर्णांक allocatorFlags = 0;
            UMat_तथ्य::MemoryFlag अस्थायी_UMat_Flags = static_cast<UMat_तथ्य::MemoryFlag>(0);
            व्योम* handle = शून्य;
            cl_int retval = CL_SUCCESS;

#अगर_घोषित HAVE_OPENCL_SVM
            svm::SVMCapabilities svmCaps = svm::getSVMCapabilitites(ctx);
            bool useSVM = ctx.useSVM() && svm::useSVM(usageFlags);
            अगर (useSVM && svmCaps.isSupportFineGrainSystem())
            {
                allocatorFlags = svm::OPENCL_SVM_FINE_GRAIN_SYSTEM;
                अस्थायी_UMat_Flags = UMat_तथ्य::अस्थायी_UMAT;
                handle = u->origdata;
                CV_OPENCL_SVM_TRACE_P("Use fine grain system: %d (%p)\n", (पूर्णांक)u->size, handle);
            }
            अन्यथा अगर (useSVM && (svmCaps.is_Support_Fine_Grain_बफर() || svmCaps.is_Support_Coarse_Grain_बफर()))
            {
                अगर (!(accessFlags & ACCESS_FAST)) // memcpy used
                {
                    bool is_Fine_Grain_बफर = svmCaps.is_Support_Fine_Grain_बफर();

                    cl_svm_mem_flags memFlags = createFlags |
                            (is_Fine_Grain_बफर ? CL_MEM_SVM_FINE_GRAIN_बफर : 0);

                    स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMAlloc + copy: %d\n", (पूर्णांक)u->size);
                    handle = svmFns->fn_clSVMAlloc((cl_context)ctx.ptr(), memFlags, u->size, 0);
                    CV_Assert(handle);

                    cl_command_queue q = शून्य;
                    अगर (!is_Fine_Grain_बफर)
                    {
                        q = (cl_command_queue)Queue::getDefault().ptr();
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", handle, (पूर्णांक)u->size);
                        cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_TRUE, CL_MAP_WRITE,
                                handle, u->size,
                                0, शून्य, शून्य);
                        CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMap()");

                    }
                    स_नकल(handle, u->origdata, u->size);
                    अगर (!is_Fine_Grain_बफर)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, handle, 0, शून्य, शून्य);
                        CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                    }

                    अस्थायी_UMat_Flags = UMat_तथ्य::अस्थायी_UMAT | UMat_तथ्य::अस्थायी_COPIED_UMAT;
                    allocatorFlags |= is_Fine_Grain_बफर ? svm::OPENCL_SVM_FINE_GRAIN_बफर
                                                : svm::OPENCL_SVM_COARSE_GRAIN_बफर;
                }
            }
            अन्यथा
#पूर्ण_अगर
            {
                अगर( copyOnMap )
                    accessFlags &= ~ACCESS_FAST;

                अस्थायी_UMat_Flags = UMat_तथ्य::अस्थायी_UMAT;
                अगर (
                #अगर_घोषित __APPLE__
                    !copyOnMap &&
                #पूर्ण_अगर
                    CV_OPENCL_ENABLE_MEM_USE_HOST_PTR
                    // There are OpenCL runtime issues for less aligned data
                    && (CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR != 0
                        && u->origdata == cv::alignPtr(u->origdata, (पूर्णांक)CV_OPENCL_ALIGNMENT_MEM_USE_HOST_PTR))
                    // Avoid sharing of host memory between OpenCL buffers
                    && !(u->original_UMat_तथ्य && u->original_UMat_तथ्य->handle)
                )
                {
                    // Change the host-side origdata[size] to "pinned memory" that enables fast
                    // DMA-transfers over PCIe to the device. Often used with clEnqueueMapBuffer/clEnqueueUnmapMemObject
                    handle = cl_Create_बफर(ctx_handle, CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR),
                                            u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer(CL_MEM_USE_HOST_PTR|(createFlags & ~CL_MEM_ALLOC_HOST_PTR), sz=%lld, origdata=%p) => %p",
                            (दीर्घ दीर्घ पूर्णांक)u->size, u->origdata, (व्योम*)handle).c_str());
                }
                अगर((!handle || retval < 0) && !(accessFlags & ACCESS_FAST))
                {
                    // Allocate device-side memory and immediately copy data from the host-side pointer origdata[size].
                    // If createFlags=CL_MEM_ALLOC_HOST_PTR (aka cv::USAGE_ALLOCATE_HOST_MEMORY), then
                    // additionally allocate a host-side "pinned" duplicate of the origdata that is
                    // managed by OpenCL. This is potentially faster in unaligned/unmanaged scenarios.
                    handle = cl_Create_बफर(ctx_handle, CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags,
                                               u->size, u->origdata, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer(CL_MEM_COPY_HOST_PTR|CL_MEM_READ_WRITE|createFlags, sz=%lld, origdata=%p) => %p",
                            (दीर्घ दीर्घ पूर्णांक)u->size, u->origdata, (व्योम*)handle).c_str());
                    अस्थायी_UMat_Flags |= UMat_तथ्य::अस्थायी_COPIED_UMAT;
                }
            }
            CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clCreateBuffer() => %p", (व्योम*)handle).c_str());
            अगर(!handle || retval != CL_SUCCESS)
                वापस false;
            u->handle = handle;
            u->पूर्व_Allocator = u->currAllocator;
            u->currAllocator = यह;
            u->flags |= अस्थायी_UMat_Flags | flags0;
            u->allocatorFlags_ = allocatorFlags;
        }
        अगर (!!(accessFlags & ACCESS_WRITE))
            u->markHostCopyObsolete(true);
        opencl_allocator_stats.onAllocate(u->size);
        वापस true;
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

    व्योम deallocate(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस;

        // Next two lines: TSAN false positive: see #27638
        CV_Assert(u->urefcount == 0);
        CV_Assert(u->refcount == 0 && "UMat deallocation error: some derived Mat is still alive");

        CV_Assert(u->handle != 0);
        CV_Assert(u->mapcount == 0);

        अगर (!!(u->flags & UMat_तथ्य::ASYNC_CLEANUP))
            addToCleanupQueue(u);
        अन्यथा
            deallocate_(u);
    }

    व्योम deallocate_(UMat_तथ्य* u) स्थिर
    {
        CV_Assert(u);
        CV_Assert(u->handle);
        अगर ((u->allocatorFlags_ & ALLOCATOR_FLAGS_EXTERNAL_बफर) == 0)
        {
            opencl_allocator_stats.onFree(u->size);
        }

#अगर_घोषित _WIN32
        अगर (cv::__termination)  // process is not in consistent state (after ExitProcess call) and terminating
            वापस;             // avoid any OpenCL calls
#पूर्ण_अगर
        अगर(u->अस्थायी_UMat())
        {
            CV_Assert(u->origdata);
//            UMatDataAutoLock lock(u);

            अगर (u->hostCopyObsolete())
            {
#अगर_घोषित HAVE_OPENCL_SVM
                अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
                {
                    Context& ctx = Context::getDefault();
                    स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    अगर( u->अस्थायी_Copied_UMat() )
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                                (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर);
                        bool is_Fine_Grain_बफर = (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर;
                        cl_command_queue q = शून्य;
                        अगर (!is_Fine_Grain_बफर)
                        {
                            CV_DbgAssert(((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0));
                            q = (cl_command_queue)Queue::getDefault().ptr();
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णांक)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                                    u->handle, u->size,
                                    0, शून्य, शून्य);
                            CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMap()");
                        }
                        clFinish(q);
                        स_नकल(u->origdata, u->handle, u->size);
                        अगर (!is_Fine_Grain_बफर)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                            cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, शून्य, शून्य);
                            CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                        }
                    }
                    अन्यथा
                    {
                        CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM);
                        // nothing
                    }
                }
                अन्यथा
#पूर्ण_अगर
                {
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
                    अगर( u->अस्थायी_Copied_UMat() )
                    {
                        Aligned_तथ्य_Ptr<false, true> alignedPtr(u->origdata, u->size, CV_OPENCL_तथ्य_PTR_ALIGNMENT);
                        CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)u->handle, CL_TRUE, 0,
                                            u->size, alignedPtr.getAlignedPtr(), 0, 0, 0));
                    }
                    अन्यथा
                    {
                        cl_int retval = 0;
                        अगर (u->अस्थायी_UMat())
                        {
                            CV_Assert(u->mapcount == 0);
                            flushCleanupQueue(); // workaround for CL_OUT_OF_RESOURCES problem (#9960)
                            व्योम* तथ्य = cl_Enqueue_Map_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                (CL_MAP_READ | CL_MAP_WRITE),
                                0, u->size, 0, 0, 0, &retval);
                            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (व्योम*)u->handle, (दीर्घ दीर्घ पूर्णांक)u->size, तथ्य).c_str());
                            CV_Assert(u->origdata == तथ्य && "Details: https://github.com/opencv/opencv/issues/6293");
                            अगर (u->original_UMat_तथ्य)
                            {
                                CV_Assert(u->original_UMat_तथ्य->तथ्य == तथ्य);
                            }
                            retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, तथ्य, 0, 0, 0);
                            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (व्योम*)u->handle, तथ्य, (दीर्घ दीर्घ पूर्णांक)u->size).c_str());
                            CV_OCL_DBG_CHECK(clFinish(q));
                        }
                    }
                }
                u->markHostCopyObsolete(false);
            }
            अन्यथा
            {
                // nothing
            }
#अगर_घोषित HAVE_OPENCL_SVM
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
            {
                अगर( u->अस्थायी_Copied_UMat() )
                {
                    Context& ctx = Context::getDefault();
                    स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_OPENCL_SVM_TRACE_P("clSVMFree: %p\n", u->handle);
                    svmFns->fn_clSVMFree((cl_context)ctx.ptr(), u->handle);
                }
            }
            अन्यथा
#पूर्ण_अगर
            {
                cl_int retval = clReleaseMemObject((cl_mem)u->handle);
                CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clReleaseMemObject(ptr=%p)", (व्योम*)u->handle).c_str());
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            u->currAllocator = u->पूर्व_Allocator;
            u->पूर्व_Allocator = शून्य;
            अगर(u->तथ्य && u->copyOnMap() && u->तथ्य != u->origdata)
                fastFree(u->तथ्य);
            u->तथ्य = u->origdata;
            u->currAllocator->deallocate(u);
            u = शून्य;
        }
        अन्यथा
        {
            CV_Assert(u->origdata == शून्य);
            अगर(u->तथ्य && u->copyOnMap() && u->तथ्य != u->origdata)
            {
                fastFree(u->तथ्य);
                u->तथ्य = 0;
                u->markHostCopyObsolete(true);
            }
            अगर (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.लो();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool().release((cl_mem)u->handle);
            }
            अन्यथा अगर (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_HOST_PTR_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.लो();
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool_Host_Ptr().release((cl_mem)u->handle);
            }
#अगर_घोषित HAVE_OPENCL_SVM
            अन्यथा अगर (u->allocatorFlags_ & ALLOCATOR_FLAGS_बफर_POOL_SVM_USED)
            {
                std::shared_ptr<ocl::Context> pCtx = std::static_pointer_cast<ocl::Context>(u->allocatorContext);
                CV_Assert(pCtx);
                ocl::Context& ctx = *pCtx.लो();
                अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
                {
                    //nothing
                }
                अन्यथा अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                        (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                {
                    स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());
                    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

                    अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) != 0)
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle, 0, शून्य, शून्य);
                        CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                    }
                }
                CV_Assert(ctx.getImpl());
                ctx.getImpl()->get_बफर_Pool_SVM().release((व्योम*)u->handle);
            }
#पूर्ण_अगर
            अन्यथा
            {
                CV_OCL_DBG_CHECK(clReleaseMemObject((cl_mem)u->handle));
            }
            u->handle = 0;
            u->markDeviceCopyObsolete(true);
            मिटाओ u;
            u = शून्य;
        }
        CV_Assert(u == शून्य);
    }

    // synchronized call (external UMatDataAutoLock, see UMat::getMat)
    व्योम map(UMat_तथ्य* u, AccessFlag accessFlags) स्थिर CV_OVERRIDE
    {
        CV_Assert(u && u->handle);

        अगर (!!(accessFlags & ACCESS_WRITE))
            u->markDeviceCopyObsolete(true);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        {
            अगर( !u->copyOnMap() )
            {
                // TODO
                // because there can be other map requests for the same UMat with different access flags,
                // we use the universal (read-write) access mode.
#अगर_घोषित HAVE_OPENCL_SVM
                अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
                {
                    अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                    {
                        Context& ctx = Context::getDefault();
                        स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                        CV_DbgAssert(svmFns->isValid());

                        अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0)
                        {
                            CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णांक)u->size);
                            cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ | CL_MAP_WRITE,
                                    u->handle, u->size,
                                    0, शून्य, शून्य);
                            CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMap()");
                            u->allocatorFlags_ |= svm::OPENCL_SVM_बफर_MAP;
                        }
                    }
                    clFinish(q);
                    u->तथ्य = (uchar*)u->handle;
                    u->markHostCopyObsolete(false);
                    u->markDeviceMemMapped(true);
                    वापस;
                }
#पूर्ण_अगर

                cl_int retval = CL_SUCCESS;
                अगर (!u->deviceMemMapped())
                {
                    CV_Assert(u->refcount == 1);
                    CV_Assert(u->mapcount++ == 0);
                    u->तथ्य = (uchar*)cl_Enqueue_Map_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                         (CL_MAP_READ | CL_MAP_WRITE),
                                                         0, u->size, 0, 0, 0, &retval);
                    CV_OCL_DBG_CHECK_परिणाम(retval, cv::format("clEnqueueMapBuffer(handle=%p, sz=%lld) => %p", (व्योम*)u->handle, (दीर्घ दीर्घ पूर्णांक)u->size, u->तथ्य).c_str());
                }
                अगर (u->तथ्य && retval == CL_SUCCESS)
                {
                    u->markHostCopyObsolete(false);
                    u->markDeviceMemMapped(true);
                    वापस;
                }

                // TODO Is it really a good idea and was it tested well?
                // if map failed, switch to copy-on-map mode for the particular buffer
                u->flags |= UMat_तथ्य::COPY_ON_MAP;
            }

            अगर(!u->तथ्य)
            {
                u->तथ्य = (uchar*)fastMalloc(u->size);
                u->markHostCopyObsolete(true);
            }
        }

        अगर (!!(accessFlags & ACCESS_READ) && u->hostCopyObsolete())
        {
            Aligned_तथ्य_Ptr<false, true> alignedPtr(u->तथ्य, u->size, CV_OPENCL_तथ्य_PTR_ALIGNMENT);
#अगर_घोषित HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == 0);
#पूर्ण_अगर
            cl_int retval = cl_Enqueue_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueReadBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (व्योम*)u->handle, (दीर्घ दीर्घ पूर्णांक)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markHostCopyObsolete(false);
        }
    }

    व्योम unmap(UMat_तथ्य* u) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस;


        CV_Assert(u->handle != 0);

        UMat_तथ्य_Auto_Lock autolock(u);

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
        cl_int retval = 0;
        अगर( !u->copyOnMap() && u->deviceMemMapped() )
        {
            CV_Assert(u->तथ्य != शून्य);
#अगर_घोषित HAVE_OPENCL_SVM
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
            {
                अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
                {
                    Context& ctx = Context::getDefault();
                    स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                    CV_DbgAssert(svmFns->isValid());

                    CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) != 0);
                    {
                        CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                        cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                                0, शून्य, शून्य);
                        CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                        clFinish(q);
                        u->allocatorFlags_ &= ~svm::OPENCL_SVM_बफर_MAP;
                    }
                }
                अगर (u->refcount == 0)
                    u->तथ्य = 0;
                u->markDeviceCopyObsolete(false);
                u->markHostCopyObsolete(true);
                वापस;
            }
#पूर्ण_अगर
            अगर (u->refcount == 0)
            {
                CV_Assert(u->mapcount-- == 1);
                retval = clEnqueueUnmapMemObject(q, (cl_mem)u->handle, u->तथ्य, 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueUnmapMemObject(handle=%p, data=%p, [sz=%lld])", (व्योम*)u->handle, u->तथ्य, (दीर्घ दीर्घ पूर्णांक)u->size).c_str());
                अगर (Device::getDefault().isAMD())
                {
                    // required for multithreaded applications (see stitching test)
                    CV_OCL_DBG_CHECK(clFinish(q));
                }
                u->markDeviceMemMapped(false);
                u->तथ्य = 0;
                u->markDeviceCopyObsolete(false);
                u->markHostCopyObsolete(true);
            }
        }
        अन्यथा अगर( u->copyOnMap() && u->deviceCopyObsolete() )
        {
            Aligned_तथ्य_Ptr<true, false> alignedPtr(u->तथ्य, u->size, CV_OPENCL_तथ्य_PTR_ALIGNMENT);
#अगर_घोषित HAVE_OPENCL_SVM
            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == 0);
#पूर्ण_अगर
            retval = cl_Enqueue_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                0, u->size, alignedPtr.getAlignedPtr(), 0, 0, 0);
            CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, 0, sz=%lld, data=%p, 0, 0, 0)",
                    (व्योम*)u->handle, (दीर्घ दीर्घ पूर्णांक)u->size, alignedPtr.getAlignedPtr()).c_str());
            u->markDeviceCopyObsolete(false);
            u->markHostCopyObsolete(true);
        }
    }

    bool checkContinuous(पूर्णांक dims, स्थिर माप_प्रकार sz[],
                         स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
                         स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[],
                         माप_प्रकार& योग, माप_प्रकार new_sz[],
                         माप_प्रकार& srcrawofs, माप_प्रकार new_srcofs[], माप_प्रकार new_srcstep[],
                         माप_प्रकार& dstrawofs, माप_प्रकार new_dstofs[], माप_प्रकार new_dststep[]) स्थिर
    {
        bool iscontinuous = true;
        srcrawofs = srcofs ? srcofs[dims-1] : 0;
        dstrawofs = dstofs ? dstofs[dims-1] : 0;
        योग = sz[dims-1];
        क्रम( पूर्णांक i = dims-2; i >= 0; i-- )
        {
            अगर( i >= 0 && (योग != srcstep[i] || योग != dststep[i]) )
                iscontinuous = false;
            योग *= sz[i];
            अगर( srcofs )
                srcrawofs += srcofs[i]*srcstep[i];
            अगर( dstofs )
                dstrawofs += dstofs[i]*dststep[i];
        }

        अगर( !iscontinuous )
        {
            // OpenCL uses {x, y, z} order while OpenCV uses {z, y, x} order.
            अगर( dims == 2 )
            {
                new_sz[0] = sz[1]; new_sz[1] = sz[0]; new_sz[2] = 1;
                // we assume that new_... arrays are initialized by caller
                // with 0's, so there is no else branch
                अगर( srcofs )
                {
                    new_srcofs[0] = srcofs[1];
                    new_srcofs[1] = srcofs[0];
                    new_srcofs[2] = 0;
                }

                अगर( dstofs )
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
                अगर( srcofs )
                {
                    new_srcofs[0] = srcofs[2];
                    new_srcofs[1] = srcofs[1];
                    new_srcofs[2] = srcofs[0];
                }

                अगर( dstofs )
                {
                    new_dstofs[0] = dstofs[2];
                    new_dstofs[1] = dstofs[1];
                    new_dstofs[2] = dstofs[0];
                }

                new_srcstep[0] = srcstep[1]; new_srcstep[1] = srcstep[0];
                new_dststep[0] = dststep[1]; new_dststep[1] = dststep[0];
            }
        }
        वापस iscontinuous;
    }

    व्योम download(UMat_तथ्य* u, व्योम* dstptr, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                  स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
                  स्थिर माप_प्रकार dststep[]) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस;
        UMat_तथ्य_Auto_Lock autolock(u);

        अगर( u->तथ्य && !u->hostCopyObsolete() )
        {
            Mat::getDefaultAllocator()->download(u, dstptr, dims, sz, srcofs, srcstep, dststep);
            वापस;
        }
        CV_Assert( u->handle != 0 );

        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        माप_प्रकार योग = 0, new_sz[] = {0, 0, 0};
        माप_प्रकार srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        माप_प्रकार dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, 0, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
        {
            CV_DbgAssert(u->तथ्य == शून्य || u->तथ्य == u->handle);
            Context& ctx = Context::getDefault();
            स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0);
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णांक)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_READ,
                        u->handle, u->size,
                        0, शून्य, शून्य);
                CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            अगर( iscontinuous )
            {
                स_नकल(dstptr, (uchar*)u->handle + srcrawofs, योग);
            }
            अन्यथा
            {
                // This code is from MatAllocator::download()
                पूर्णांक isz[CV_MAX_DIM];
                uchar* srcptr = (uchar*)u->handle;
                क्रम( पूर्णांक i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
                    अगर( sz[i] == 0 )
                    वापस;
                    अगर( srcofs )
                    srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                    isz[i] = (पूर्णांक)sz[i];
                }

                Mat src(dims, isz, CV_8U, srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                स्थिर Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                माप_प्रकार j, planesz = it.size;

                क्रम( j = 0; j < it.nplanes; j++, ++it )
                    स_नकल(ptrs[1], ptrs[0], planesz);
            }
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, शून्य, शून्य);
                CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अन्यथा
#पूर्ण_अगर
        {
            अगर( iscontinuous )
            {
                Aligned_तथ्य_Ptr<false, true> alignedPtr((uchar*)dstptr, योग, CV_OPENCL_तथ्य_PTR_ALIGNMENT);
                CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    srcrawofs, योग, alignedPtr.getAlignedPtr(), 0, 0, 0));
            }
            अन्यथा अगर (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                स्थिर माप_प्रकार padding = CV_OPENCL_तथ्य_PTR_ALIGNMENT;
                माप_प्रकार new_srcrawofs = srcrawofs & ~(padding-1);
                माप_प्रकार membuf_ofs = srcrawofs - new_srcrawofs;
                Aligned_तथ्य_Ptr2_D<false, false> alignedPtr(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                          CV_OPENCL_तथ्य_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_srcstep[0] >= new_sz[0]);
                योग = align_आकार(new_srcstep[0]*new_sz[1] + membuf_ofs, padding);
                योग = std::min(योग, u->size - new_srcrawofs);
                CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_srcrawofs, योग, ptr, 0, 0, 0));
                क्रम( माप_प्रकार i = 0; i < new_sz[1]; i++ )
                    स_नकल( (uchar*)dstptr + i*new_dststep[0], ptr + i*new_srcstep[0] + membuf_ofs, new_sz[0]);
            }
            अन्यथा
            {
                Aligned_तथ्य_Ptr2_D<false, true> alignedPtr((uchar*)dstptr, new_sz[1], new_sz[0], new_dststep[0], CV_OPENCL_तथ्य_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_Enqueue_Read_बफर_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_srcofs, new_dstofs, new_sz,
                    new_srcstep[0], 0,
                    new_dststep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
    }

    व्योम upload(UMat_तथ्य* u, स्थिर व्योम* srcptr, पूर्णांक dims, स्थिर माप_प्रकार sz[],
                स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[],
                स्थिर माप_प्रकार srcstep[]) स्थिर CV_OVERRIDE
    {
        अगर(!u)
            वापस;

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(u->refcount == 0 || u->अस्थायी_UMat());

        माप_प्रकार योग = 0, new_sz[] = {0, 0, 0};
        माप_प्रकार srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        माप_प्रकार dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, 0, srcstep, dstofs, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_तथ्य_Auto_Lock autolock(u);

        // if there is cached CPU copy of the GPU matrix,
        // we could use it as a destination.
        // we can do it in 2 cases:
        //    1. we overwrite the whole content
        //    2. we overwrite part of the matrix, but the GPU copy is out-of-date
        अगर( u->तथ्य && (u->hostCopyObsolete() < u->deviceCopyObsolete() || योग == u->size))
        {
            Mat::getDefaultAllocator()->upload(u, srcptr, dims, sz, dstofs, dststep, srcstep);
            u->markHostCopyObsolete(false);
            u->markDeviceCopyObsolete(true);
            वापस;
        }

        CV_Assert( u->handle != 0 );
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
        {
            CV_DbgAssert(u->तथ्य == शून्य || u->तथ्य == u->handle);
            Context& ctx = Context::getDefault();
            स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
            CV_DbgAssert(svmFns->isValid());

            CV_DbgAssert((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MAP) == 0);
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMap: %p (%d)\n", u->handle, (पूर्णांक)u->size);
                cl_int status = svmFns->fn_clEnqueueSVMMap(q, CL_FALSE, CL_MAP_WRITE,
                        u->handle, u->size,
                        0, शून्य, शून्य);
                CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMap()");
            }
            clFinish(q);
            अगर( iscontinuous )
            {
                स_नकल((uchar*)u->handle + dstrawofs, srcptr, योग);
            }
            अन्यथा
            {
                // This code is from MatAllocator::upload()
                पूर्णांक isz[CV_MAX_DIM];
                uchar* dstptr = (uchar*)u->handle;
                क्रम( पूर्णांक i = 0; i < dims; i++ )
                {
                    CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
                    अगर( sz[i] == 0 )
                    वापस;
                    अगर( dstofs )
                    dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    isz[i] = (पूर्णांक)sz[i];
                }

                Mat src(dims, isz, CV_8U, (व्योम*)srcptr, srcstep);
                Mat dst(dims, isz, CV_8U, dstptr, dststep);

                स्थिर Mat* arrays[] = { &src, &dst };
                uchar* ptrs[2];
                NAryMatIterator it(arrays, ptrs, 2);
                माप_प्रकार j, planesz = it.size;

                क्रम( j = 0; j < it.nplanes; j++, ++it )
                    स_नकल(ptrs[1], ptrs[0], planesz);
            }
            अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_COARSE_GRAIN_बफर)
            {
                CV_OPENCL_SVM_TRACE_P("clEnqueueSVMUnmap: %p\n", u->handle);
                cl_int status = svmFns->fn_clEnqueueSVMUnmap(q, u->handle,
                        0, शून्य, शून्य);
                CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMUnmap()");
                clFinish(q);
            }
        }
        अन्यथा
#पूर्ण_अगर
        {
            अगर( iscontinuous )
            {
                Aligned_तथ्य_Ptr<true, false> alignedPtr((uchar*)srcptr, योग, CV_OPENCL_तथ्य_PTR_ALIGNMENT);
                cl_int retval = cl_Enqueue_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                    dstrawofs, योग, alignedPtr.getAlignedPtr(), 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueWriteBuffer(q, handle=%p, CL_TRUE, offset=%lld, sz=%lld, data=%p, 0, 0, 0)",
                        (व्योम*)u->handle, (दीर्घ दीर्घ पूर्णांक)dstrawofs, (दीर्घ दीर्घ पूर्णांक)u->size, alignedPtr.getAlignedPtr()).c_str());
            }
            अन्यथा अगर (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                स्थिर माप_प्रकार padding = CV_OPENCL_तथ्य_PTR_ALIGNMENT;
                माप_प्रकार new_dstrawofs = dstrawofs & ~(padding-1);
                माप_प्रकार membuf_ofs = dstrawofs - new_dstrawofs;
                Aligned_तथ्य_Ptr2_D<false, false> alignedPtr(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                          CV_OPENCL_तथ्य_PTR_ALIGNMENT, padding*2);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);
                योग = align_आकार(new_dststep[0]*new_sz[1] + membuf_ofs, padding);
                योग = std::min(योग, u->size - new_dstrawofs);
                /*printf("new_sz0=%d, new_sz1=%d, membuf_ofs=%d, total=%d (%08x), new_dstrawofs=%d (%08x)\n",
                       (int)new_sz[0], (int)new_sz[1], (int)membuf_ofs,
                       (int)total, (int)total, (int)new_dstrawofs, (int)new_dstrawofs);*/
                CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, योग, ptr, 0, 0, 0));
                क्रम( माप_प्रकार i = 0; i < new_sz[1]; i++ )
                    स_नकल( ptr + i*new_dststep[0] + membuf_ofs, (uchar*)srcptr + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_Enqueue_Write_बफर(q, (cl_mem)u->handle, CL_TRUE,
                                                 new_dstrawofs, योग, ptr, 0, 0, 0));
            }
            अन्यथा
            {
                Aligned_तथ्य_Ptr2_D<true, false> alignedPtr((uchar*)srcptr, new_sz[1], new_sz[0], new_srcstep[0], CV_OPENCL_तथ्य_PTR_ALIGNMENT);
                uchar* ptr = alignedPtr.getAlignedPtr();

                CV_OCL_CHECK(cl_Enqueue_Write_बफर_Rect(q, (cl_mem)u->handle, CL_TRUE,
                    new_dstofs, new_srcofs, new_sz,
                    new_dststep[0], 0,
                    new_srcstep[0], 0,
                    ptr, 0, 0, 0));
            }
        }
        u->markHostCopyObsolete(true);
#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                (u->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अन्यथा
#पूर्ण_अगर
        {
            u->markHostCopyObsolete(true);
        }
        u->markDeviceCopyObsolete(false);
    }

    व्योम copy(UMat_तथ्य* src, UMat_तथ्य* dst, पूर्णांक dims, स्थिर माप_प्रकार sz[],
              स्थिर माप_प्रकार srcofs[], स्थिर माप_प्रकार srcstep[],
              स्थिर माप_प्रकार dstofs[], स्थिर माप_प्रकार dststep[], bool _sync) स्थिर CV_OVERRIDE
    {
        अगर(!src || !dst)
            वापस;

        माप_प्रकार योग = 0, new_sz[] = {0, 0, 0};
        माप_प्रकार srcrawofs = 0, new_srcofs[] = {0, 0, 0}, new_srcstep[] = {0, 0, 0};
        माप_प्रकार dstrawofs = 0, new_dstofs[] = {0, 0, 0}, new_dststep[] = {0, 0, 0};

        bool iscontinuous = checkContinuous(dims, sz, srcofs, srcstep, dstofs, dststep,
                                            योग, new_sz,
                                            srcrawofs, new_srcofs, new_srcstep,
                                            dstrawofs, new_dstofs, new_dststep);

        UMat_तथ्य_Auto_Lock src_autolock(src, dst);

        अगर( !src->handle || (src->तथ्य && src->hostCopyObsolete() < src->deviceCopyObsolete()) )
        {
            upload(dst, src->तथ्य + srcrawofs, dims, sz, dstofs, dststep, srcstep);
            वापस;
        }
        अगर( !dst->handle || (dst->तथ्य && dst->hostCopyObsolete() < dst->deviceCopyObsolete()) )
        {
            download(src, dst->तथ्य + dstrawofs, dims, sz, srcofs, srcstep, dststep);
            dst->markHostCopyObsolete(false);
#अगर_घोषित HAVE_OPENCL_SVM
            अगर ((dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
                    (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
            {
                // nothing
            }
            अन्यथा
#पूर्ण_अगर
            {
                dst->markDeviceCopyObsolete(true);
            }
            वापस;
        }

        // there should be no user-visible CPU copies of the UMat which we are going to copy to
        CV_Assert(dst->refcount == 0);
        cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

        cl_int retval = CL_SUCCESS;
#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0 ||
                (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
        {
            अगर ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0 &&
                            (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
            {
                Context& ctx = Context::getDefault();
                स्थिर svm::SVMFunctions* svmFns = svm::getSVMFunctions(ctx);
                CV_DbgAssert(svmFns->isValid());

                अगर( iscontinuous )
                {
                    CV_OPENCL_SVM_TRACE_P("clEnqueueSVMMemcpy: %p <-- %p (%d)\n",
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs, (पूर्णांक)योग);
                    cl_int status = svmFns->fn_clEnqueueSVMMemcpy(q, CL_TRUE,
                            (uchar*)dst->handle + dstrawofs, (uchar*)src->handle + srcrawofs,
                            योग, 0, शून्य, शून्य);
                    CV_OCL_CHECK_परिणाम(status, "clEnqueueSVMMemcpy()");
                }
                अन्यथा
                {
                    clFinish(q);
                    // This code is from MatAllocator::download()/upload()
                    पूर्णांक isz[CV_MAX_DIM];
                    uchar* srcptr = (uchar*)src->handle;
                    क्रम( पूर्णांक i = 0; i < dims; i++ )
                    {
                        CV_Assert( sz[i] <= (माप_प्रकार)पूर्णांक_उच्च );
                        अगर( sz[i] == 0 )
                        वापस;
                        अगर( srcofs )
                        srcptr += srcofs[i]*(i <= dims-2 ? srcstep[i] : 1);
                        isz[i] = (पूर्णांक)sz[i];
                    }
                    Mat m_src(dims, isz, CV_8U, srcptr, srcstep);

                    uchar* dstptr = (uchar*)dst->handle;
                    क्रम( पूर्णांक i = 0; i < dims; i++ )
                    {
                        अगर( dstofs )
                        dstptr += dstofs[i]*(i <= dims-2 ? dststep[i] : 1);
                    }
                    Mat m_dst(dims, isz, CV_8U, dstptr, dststep);

                    स्थिर Mat* arrays[] = { &m_src, &m_dst };
                    uchar* ptrs[2];
                    NAryMatIterator it(arrays, ptrs, 2);
                    माप_प्रकार j, planesz = it.size;

                    क्रम( j = 0; j < it.nplanes; j++, ++it )
                        स_नकल(ptrs[1], ptrs[0], planesz);
                }
            }
            अन्यथा
            {
                अगर ((src->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) != 0)
                {
                    map(src, ACCESS_READ);
                    upload(dst, src->तथ्य + srcrawofs, dims, sz, dstofs, dststep, srcstep);
                    unmap(src);
                }
                अन्यथा
                {
                    map(dst, ACCESS_WRITE);
                    download(src, dst->तथ्य + dstrawofs, dims, sz, srcofs, srcstep, dststep);
                    unmap(dst);
                }
            }
        }
        अन्यथा
#पूर्ण_अगर
        {
            अगर( iscontinuous )
            {
                retval = cl_Enqueue_Copy_बफर(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                               srcrawofs, dstrawofs, योग, 0, 0, 0);
                CV_OCL_CHECK_परिणाम(retval, cv::format("clEnqueueCopyBuffer(q, src=%p, dst=%p, src_offset=%lld, dst_offset=%lld, sz=%lld, 0, 0, 0)",
                        (व्योम*)src->handle, (व्योम*)dst->handle, (दीर्घ दीर्घ पूर्णांक)srcrawofs, (दीर्घ दीर्घ पूर्णांक)dstrawofs, (दीर्घ दीर्घ पूर्णांक)योग).c_str());
            }
            अन्यथा अगर (CV_OPENCL_DISABLE_बफर_RECT_OPERATIONS)
            {
                स्थिर माप_प्रकार padding = CV_OPENCL_तथ्य_PTR_ALIGNMENT;
                माप_प्रकार new_srcrawofs = srcrawofs & ~(padding-1);
                माप_प्रकार srcmembuf_ofs = srcrawofs - new_srcrawofs;
                माप_प्रकार new_dstrawofs = dstrawofs & ~(padding-1);
                माप_प्रकार dstmembuf_ofs = dstrawofs - new_dstrawofs;

                Aligned_तथ्य_Ptr2_D<false, false> srcBuf(0, new_sz[1], new_srcstep[0], new_srcstep[0],
                                                      CV_OPENCL_तथ्य_PTR_ALIGNMENT, padding*2);
                Aligned_तथ्य_Ptr2_D<false, false> dstBuf(0, new_sz[1], new_dststep[0], new_dststep[0],
                                                      CV_OPENCL_तथ्य_PTR_ALIGNMENT, padding*2);
                uchar* srcptr = srcBuf.getAlignedPtr();
                uchar* dstptr = dstBuf.getAlignedPtr();

                CV_Assert(new_dststep[0] >= new_sz[0] && new_srcstep[0] >= new_sz[0]);

                माप_प्रकार src_योग = align_आकार(new_srcstep[0]*new_sz[1] + srcmembuf_ofs, padding);
                src_योग = std::min(src_योग, src->size - new_srcrawofs);
                माप_प्रकार dst_योग = align_आकार(new_dststep[0]*new_sz[1] + dstmembuf_ofs, padding);
                dst_योग = std::min(dst_योग, dst->size - new_dstrawofs);

                CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)src->handle, CL_TRUE,
                                                 new_srcrawofs, src_योग, srcptr, 0, 0, 0));
                CV_OCL_CHECK(cl_Enqueue_Read_बफर(q, (cl_mem)dst->handle, CL_TRUE,
                                                 new_dstrawofs, dst_योग, dstptr, 0, 0, 0));

                क्रम( माप_प्रकार i = 0; i < new_sz[1]; i++ )
                    स_नकल( dstptr + dstmembuf_ofs + i*new_dststep[0],
                            srcptr + srcmembuf_ofs + i*new_srcstep[0], new_sz[0]);
                CV_OCL_CHECK(cl_Enqueue_Write_बफर(q, (cl_mem)dst->handle, CL_TRUE,
                                                  new_dstrawofs, dst_योग, dstptr, 0, 0, 0));
            }
            अन्यथा
            {
                CV_OCL_CHECK(retval = cl_Enqueue_Copy_बफर_Rect(q, (cl_mem)src->handle, (cl_mem)dst->handle,
                                                   new_srcofs, new_dstofs, new_sz,
                                                   new_srcstep[0], 0,
                                                   new_dststep[0], 0,
                                                   0, 0, 0));
            }
        }
        अगर (retval == CL_SUCCESS)
        {
            CV_IMPL_ADD(CV_IMPL_OCL)
        }

#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_बफर ||
            (dst->allocatorFlags_ & svm::OPENCL_SVM_बफर_MASK) == svm::OPENCL_SVM_FINE_GRAIN_SYSTEM)
        {
            // nothing
        }
        अन्यथा
#पूर्ण_अगर
        {
            dst->markHostCopyObsolete(true);
        }
        dst->markDeviceCopyObsolete(false);

        अगर( _sync )
        {
            CV_OCL_DBG_CHECK(clFinish(q));
        }
    }

    बफर_Pool_Controller* get_बफर_Pool_Controller(स्थिर अक्षर* id) स्थिर CV_OVERRIDE
    {
        ocl::Context ctx = Context::getDefault();
        अगर (ctx.empty())
            वापस शून्य;
#अगर_घोषित HAVE_OPENCL_SVM
        अगर ((svm::checkForceSVMUmatUsage() && (id == शून्य || म_भेद(id, "OCL") == 0)) || (id != शून्य && म_भेद(id, "SVM") == 0))
        {
            वापस &ctx.getImpl()->get_बफर_Pool_SVM();
        }
#पूर्ण_अगर
        अगर (id != शून्य && म_भेद(id, "HOST_ALLOC") == 0)
        {
            वापस &ctx.getImpl()->get_बफर_Pool_Host_Ptr();
        }
        अगर (id != शून्य && म_भेद(id, "OCL") != 0)
        {
            CV_Error(cv::Error::StsBadArg, "getBufferPoolController(): unknown BufferPool ID\n");
        }
        वापस &ctx.getImpl()->get_बफर_Pool();
    }

    MatAllocator* matStdAllocator;

    mutable cv::Mutex cleanupQueueMutex;
    mutable std::deque<UMat_तथ्य*> cleanupQueue;

    व्योम flushCleanupQueue() स्थिर
    {
        // TSAN false positive: see #27638
        अगर (!cleanupQueue.empty())
        {
            std::deque<UMat_तथ्य*> q;
            {
                cv::AutoLock lock(cleanupQueueMutex);
                // TSAN false positive: see #27638
                q.swap(cleanupQueue);
            }
            क्रम (std::deque<UMat_तथ्य*>::const_iterator i = q.begin(); i != q.end(); ++i)
            {
                deallocate_(*i);
            }
        }
    }
    व्योम addToCleanupQueue(UMat_तथ्य* u) स्थिर
    {
        //TODO: Validation check: CV_Assert(!u->tempUMat());
        {
            cv::AutoLock lock(cleanupQueueMutex);
            cleanupQueue.push_back(u);
        }
    }
};

अटल OpenCLAllocator* getOpenCLAllocator_() // call once guarantee
{
    अटल OpenCLAllocator* g_allocator = नया OpenCLAllocator(); // avoid destructor call (using of this object is too wide)
    वापस g_allocator;
}
MatAllocator* getOpenCLAllocator()
{
    CV_SINGLETON_LAZY_INIT(MatAllocator, getOpenCLAllocator_())
}

}} // namespace cv::ocl


namespace cv {

// three funcs below are implemented in umatrix.cpp
व्योम set_आकार( UMat& m, पूर्णांक _dims, स्थिर पूर्णांक* _sz, स्थिर माप_प्रकार* _steps,
              bool autoSteps = false );
व्योम finalizeHdr(UMat& m);

} // namespace cv


namespace cv { namespace ocl {

/*
// Convert OpenCL buffer memory to UMat
*/
व्योम convert_From_बफर(व्योम* cl_mem_बफर, माप_प्रकार step, पूर्णांक rows, पूर्णांक cols, पूर्णांक type, UMat& dst)
{
    पूर्णांक d = 2;
    पूर्णांक sizes[] = { rows, cols };

    CV_Assert(0 <= d && d <= CV_MAX_DIM);

    dst.release();

    dst.flags      = (type & Mat::TYPE_MASK) | Mat::MAGIC_VAL;
    dst.usageFlags = USAGE_DEFAULT;

    set_आकार(dst, d, sizes, 0, true);
    dst.offset = 0;

    cl_mem             memobj = (cl_mem)cl_mem_बफर;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_TYPE, माप(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_बफर == mem_type);

    माप_प्रकार योग = 0;
    CV_OCL_CHECK(clGetMemObjectInfo(memobj, CL_MEM_आकार, माप(माप_प्रकार), &योग, 0));

    CV_OCL_CHECK(clRetainMemObject(memobj));

    CV_Assert((पूर्णांक)step >= cols * CV_ELEM_आकार(type));
    CV_Assert(योग >= rows * step);

    // attach clBuffer to UMatData
    dst.u = नया UMat_तथ्य(getOpenCLAllocator());
    dst.u->तथ्य            = 0;
    dst.u->allocatorFlags_ = OpenCLAllocator::ALLOCATOR_FLAGS_EXTERNAL_बफर;  // not allocated from any OpenCV buffer pool
    dst.u->flags           = static_cast<UMat_तथ्य::MemoryFlag>(0);
    dst.u->handle          = cl_mem_बफर;
    dst.u->origdata        = 0;
    dst.u->पूर्व_Allocator   = 0;
    dst.u->size            = योग;

    finalizeHdr(dst);
    dst.addref();

    वापस;
} // convertFromBuffer()


/*
// Convert OpenCL image2d_t memory to UMat
*/
व्योम convertFromImage(व्योम* cl_mem_image, UMat& dst)
{
    cl_mem             clImage = (cl_mem)cl_mem_image;
    cl_mem_object_type mem_type = 0;

    CV_OCL_CHECK(clGetMemObjectInfo(clImage, CL_MEM_TYPE, माप(cl_mem_object_type), &mem_type, 0));

    CV_Assert(CL_MEM_OBJECT_IMAGE2D == mem_type);

    cl_image_format fmt = { 0, 0 };
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_FORMAT, माप(cl_image_format), &fmt, 0));

    पूर्णांक depth = CV_8U;
    चयन (fmt.image_channel_तथ्य_type)
    {
    हाल CL_UNORM_INT8:
    हाल CL_UNSIGNED_INT8:
        depth = CV_8U;
        अवरोध;

    हाल CL_SNORM_INT8:
    हाल CL_SIGNED_INT8:
        depth = CV_8S;
        अवरोध;

    हाल CL_UNORM_INT16:
    हाल CL_UNSIGNED_INT16:
        depth = CV_16U;
        अवरोध;

    हाल CL_SNORM_INT16:
    हाल CL_SIGNED_INT16:
        depth = CV_16S;
        अवरोध;

    हाल CL_SIGNED_INT32:
        depth = CV_32S;
        अवरोध;

    हाल CL_FLOAT:
        depth = CV_32F;
        अवरोध;

    हाल CL_HALF_FLOAT:
        depth = CV_16F;
        अवरोध;

    शेष:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_data_type");
    }

    पूर्णांक type = CV_8UC1;
    चयन (fmt.image_channel_order)
    {
    हाल CL_R:
    हाल CL_A:
    हाल CL_INTENSITY:
    हाल CL_LUMINANCE:
        type = CV_MAKE_TYPE(depth, 1);
        अवरोध;

    हाल CL_RG:
    हाल CL_RA:
        type = CV_MAKE_TYPE(depth, 2);
        अवरोध;

    // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
    // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
    /*case CL_RGB:
        type = CV_MAKE_TYPE(depth, 3);
        break;*/

    हाल CL_RGBA:
    हाल CL_BGRA:
    हाल CL_ARGB:
        type = CV_MAKE_TYPE(depth, 4);
        अवरोध;

    शेष:
        CV_Error(cv::Error::OpenCLApiCallError, "Not supported image_channel_order");
        अवरोध;
    }

    माप_प्रकार step = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_ROW_PITCH, माप(माप_प्रकार), &step, 0));

    माप_प्रकार w = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_WIDTH, माप(माप_प्रकार), &w, 0));

    माप_प्रकार h = 0;
    CV_OCL_CHECK(clGetImageInfo(clImage, CL_IMAGE_HEIGHT, माप(माप_प्रकार), &h, 0));

    dst.create((पूर्णांक)h, (पूर्णांक)w, type);

    cl_mem cl_बफर = (cl_mem)dst.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    माप_प्रकार offset = 0;
    माप_प्रकार src_origin[3] = { 0, 0, 0 };
    माप_प्रकार region[3] = { w, h, 1 };
    CV_OCL_CHECK(cl_Enqueue_Copy_Image_To_बफर(q, clImage, cl_बफर, src_origin, region, offset, 0, शून्य, शून्य));

    CV_OCL_CHECK(clFinish(q));

    वापस;
} // convertFromImage()


///////////////////////////////////////////// Utility functions /////////////////////////////////////////////////

अटल व्योम getDevices(std::vector<cl_device_id>& devices, cl_platform_id platform)
{
    cl_uint अंक_Devices = 0;
    cl_int status = clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, 0, शून्य, &अंक_Devices);
    अगर (status != CL_DEVICE_NOT_FOUND) // Not an error if platform has no devices
    {
        CV_OCL_DBG_CHECK_परिणाम(status,
            cv::format("clGetDeviceIDs(platform, Device::TYPE_ALL, num_entries=0, devices=NULL, numDevices=%p)", &अंक_Devices).c_str());
    }

    अगर (अंक_Devices == 0)
    {
        devices.सफा();
        वापस;
    }

    devices.resize((माप_प्रकार)अंक_Devices);
    CV_OCL_DBG_CHECK(clGetDeviceIDs(platform, (cl_device_type)Device::TYPE_ALL, अंक_Devices, &devices[0], &अंक_Devices));
}

काष्ठा PlatformInfo::Impl
{
    Impl(व्योम* id)
    {
        refcount = 1;
        handle = *(cl_platform_id*)id;
        getDevices(devices, handle);

        version_ = getStrProp(CL_PLATFORM_VERSION);
        parseOpenCLVersion(version_, versionMajor_, versionMinor_);
    }

    String getStrProp(cl_platform_info prop) स्थिर
    {
        अक्षर buf[1024];
        माप_प्रकार sz=0;
        वापस clGetPlatformInfo(handle, prop, माप(buf)-16, buf, &sz) == CL_SUCCESS &&
            sz < माप(buf) ? String(buf) : String();
    }

    IMPLEMENT_REFCOUNTABLE();
    std::vector<cl_device_id> devices;
    cl_platform_id handle;

    String version_;
    पूर्णांक versionMajor_;
    पूर्णांक versionMinor_;
};

PlatformInfo::PlatformInfo() CV_NOEXCEPT
{
    p = 0;
}

PlatformInfo::PlatformInfo(व्योम* platform_id)
{
    p = नया Impl(platform_id);
}

PlatformInfo::~PlatformInfo()
{
    अगर(p)
        p->release();
}

PlatformInfo::PlatformInfo(स्थिर PlatformInfo& i)
{
    अगर (i.p)
        i.p->addref();
    p = i.p;
}

PlatformInfo& PlatformInfo::चालक =(स्थिर PlatformInfo& i)
{
    अगर (i.p != p)
    {
        अगर (i.p)
            i.p->addref();
        अगर (p)
            p->release();
        p = i.p;
    }
    वापस *यह;
}

PlatformInfo::PlatformInfo(PlatformInfo&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

PlatformInfo& PlatformInfo::चालक = (PlatformInfo&& i) CV_NOEXCEPT
{
    अगर (यह != &i) {
        अगर(p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    वापस *यह;
}

पूर्णांक PlatformInfo::device_संख्या() स्थिर
{
    वापस p ? (पूर्णांक)p->devices.size() : 0;
}

व्योम PlatformInfo::getDevice(Device& device, पूर्णांक d) स्थिर
{
    CV_Assert(p && d < (पूर्णांक)p->devices.size() );
    अगर(p)
        device.set(p->devices[d]);
}

String PlatformInfo::name() स्थिर
{
    वापस p ? p->getStrProp(CL_PLATFORM_NAME) : String();
}

String PlatformInfo::vendor() स्थिर
{
    वापस p ? p->getStrProp(CL_PLATFORM_VENDOR) : String();
}

String PlatformInfo::version() स्थिर
{
    वापस p ? p->version_ : String();
}

पूर्णांक PlatformInfo::versionMajor() स्थिर
{
    CV_Assert(p);
    वापस p->versionMajor_;
}

पूर्णांक PlatformInfo::versionMinor() स्थिर
{
    CV_Assert(p);
    वापस p->versionMinor_;
}

अटल व्योम getPlatforms(std::vector<cl_platform_id>& platforms)
{
    cl_uint अंक_Platforms = 0;
    CV_OCL_DBG_CHECK(clGetPlatformIDs(0, शून्य, &अंक_Platforms));

    अगर (अंक_Platforms == 0)
    {
        platforms.सफा();
        वापस;
    }

    platforms.resize((माप_प्रकार)अंक_Platforms);
    CV_OCL_DBG_CHECK(clGetPlatformIDs(अंक_Platforms, &platforms[0], &अंक_Platforms));
}

व्योम getPlatfomsInfo(std::vector<PlatformInfo>& platformsInfo)
{
    std::vector<cl_platform_id> platforms;
    getPlatforms(platforms);

    क्रम (माप_प्रकार i = 0; i < platforms.size(); i++)
        platformsInfo.push_back( PlatformInfo((व्योम*)&platforms[i]) );
}

स्थिर अक्षर* typeToStr(पूर्णांक type)
{
    अटल स्थिर अक्षर* tab[CV_DEPTH_MAX*16]=
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
    पूर्णांक cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    स्थिर अक्षर* परिणाम = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(परिणाम);
    वापस परिणाम;
}

स्थिर अक्षर* memopTypeToStr(पूर्णांक type)
{
    अटल स्थिर अक्षर* tab[CV_DEPTH_MAX*16] =
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
    पूर्णांक cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    स्थिर अक्षर* परिणाम = cn > 16 ? nullptr : tab[depth*16 + cn-1];
    CV_Assert(परिणाम);
    वापस परिणाम;
}

स्थिर अक्षर* vecopTypeToStr(पूर्णांक type)
{
    अटल स्थिर अक्षर* tab[CV_DEPTH_MAX*16] =
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
    पूर्णांक cn = CV_MAT_CN(type), depth = CV_MAT_DEPTH(type);
    स्थिर अक्षर* परिणाम = cn > 16 ? 0 : tab[depth*16 + cn-1];
    CV_Assert(परिणाम);
    वापस परिणाम;
}

// Deprecated due to size of buf buffer being unknowable.
स्थिर अक्षर* convertTypeStr(पूर्णांक sdepth, पूर्णांक ddepth, पूर्णांक cn, अक्षर* buf)
{
    // Since the size of buf is not given, we assume 50 because that's what all callers use.
    constexpr माप_प्रकार buf_max = 50;

    वापस convertTypeStr(sdepth, ddepth, cn, buf, buf_max);
}

स्थिर अक्षर* convertTypeStr(पूर्णांक sdepth, पूर्णांक ddepth, पूर्णांक cn, अक्षर* buf, माप_प्रकार buf_आकार_2)
{
    अगर( sdepth == ddepth )
        वापस "noconvert";
    स्थिर अक्षर *typestr = typeToStr(CV_MAKETYPE(ddepth, cn));
    अगर( CV_IS_FLOAT_TYPE(ddepth) ||
        (ddepth == CV_32S && sdepth < CV_32S) ||
        (ddepth == CV_16S && sdepth <= CV_8S) ||
        (ddepth == CV_16U && sdepth == CV_8U))
    {
        snprintf(buf, buf_आकार_2, "convert_%s", typestr);
    }
    अन्यथा अगर( CV_IS_FLOAT_TYPE(sdepth) )
        snprintf(buf, buf_आकार_2, "convert_%s_sat_rte", typestr);
    अन्यथा
        snprintf(buf, buf_आकार_2, "convert_%s_sat", typestr);

    वापस buf;
}

स्थिर अक्षर* getOpenCLErrorString(पूर्णांक errorCode)
{
#घोषणा CV_OCL_CODE(id) हाल id: वापस #id
#घोषणा CV_OCL_CODE_(id, name) हाल id: वापस #name
    चयन (errorCode)
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
    CV_OCL_CODE(CL_MISALIGNED_SUB_बफर_OFFSET);
    CV_OCL_CODE(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST);
    CV_OCL_CODE(CL_COMPILE_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_LINKER_NOT_AVAILABLE);
    CV_OCL_CODE(CL_LINK_PROGRAM_FAILURE);
    CV_OCL_CODE(CL_DEVICE_PARTITION_FAILED);
    CV_OCL_CODE(CL_KERNEL_ARG_INFO_NOT_AVAILABLE);
    CV_OCL_CODE(CL_INVALID_मूल्य);
    CV_OCL_CODE(CL_INVALID_DEVICE_TYPE);
    CV_OCL_CODE(CL_INVALID_PLATFORM);
    CV_OCL_CODE(CL_INVALID_DEVICE);
    CV_OCL_CODE(CL_INVALID_CONTEXT);
    CV_OCL_CODE(CL_INVALID_QUEUE_PROPERTIES);
    CV_OCL_CODE(CL_INVALID_COMMAND_QUEUE);
    CV_OCL_CODE(CL_INVALID_HOST_PTR);
    CV_OCL_CODE(CL_INVALID_MEM_OBJECT);
    CV_OCL_CODE(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_IMAGE_आकार);
    CV_OCL_CODE(CL_INVALID_SAMPLER);
    CV_OCL_CODE(CL_INVALID_द्विआधारी);
    CV_OCL_CODE(CL_INVALID_BUILD_OPTIONS);
    CV_OCL_CODE(CL_INVALID_PROGRAM);
    CV_OCL_CODE(CL_INVALID_PROGRAM_EXECUTABLE);
    CV_OCL_CODE(CL_INVALID_KERNEL_NAME);
    CV_OCL_CODE(CL_INVALID_KERNEL_DEFINITION);
    CV_OCL_CODE(CL_INVALID_KERNEL);
    CV_OCL_CODE(CL_INVALID_ARG_सूचकांक);
    CV_OCL_CODE(CL_INVALID_ARG_मूल्य);
    CV_OCL_CODE(CL_INVALID_ARG_आकार);
    CV_OCL_CODE(CL_INVALID_KERNEL_ARGS);
    CV_OCL_CODE(CL_INVALID_WORK_DIMENSION);
    CV_OCL_CODE(CL_INVALID_WORK_GROUP_आकार);
    CV_OCL_CODE(CL_INVALID_WORK_ITEM_आकार);
    CV_OCL_CODE(CL_INVALID_GLOBAL_OFFSET);
    CV_OCL_CODE(CL_INVALID_EVENT_WAIT_LIST);
    CV_OCL_CODE(CL_INVALID_EVENT);
    CV_OCL_CODE(CL_INVALID_OPERATION);
    CV_OCL_CODE(CL_INVALID_GL_OBJECT);
    CV_OCL_CODE(CL_INVALID_बफर_आकार);
    CV_OCL_CODE(CL_INVALID_MIP_LEVEL);
    CV_OCL_CODE(CL_INVALID_GLOBAL_WORK_आकार);
    // OpenCL 1.1
    CV_OCL_CODE(CL_INVALID_PROPERTY);
    // OpenCL 1.2
    CV_OCL_CODE(CL_INVALID_IMAGE_DESCRIPTOR);
    CV_OCL_CODE(CL_INVALID_COMPILER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_LINKER_OPTIONS);
    CV_OCL_CODE(CL_INVALID_DEVICE_PARTITION_गिनती);
    // OpenCL 2.0
    CV_OCL_CODE_(-69, CL_INVALID_PIPE_आकार);
    CV_OCL_CODE_(-70, CL_INVALID_DEVICE_QUEUE);
    // Extensions
    CV_OCL_CODE_(-1000, CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR);
    CV_OCL_CODE_(-1001, CL_PLATFORM_NOT_FOUND_KHR);
    CV_OCL_CODE_(-1002, CL_INVALID_D3D10_DEVICE_KHR);
    CV_OCL_CODE_(-1003, CL_INVALID_D3D10_RESOURCE_KHR);
    CV_OCL_CODE_(-1004, CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR);
    CV_OCL_CODE_(-1005, CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR);
    शेष: वापस "Unknown OpenCL error";
    }
#अघोषित CV_OCL_CODE
#अघोषित CV_OCL_CODE_
}

ढाँचा <typename T>
अटल std::string kerToStr(स्थिर Mat & k)
{
    पूर्णांक width = k.cols - 1, depth = k.depth();
    स्थिर T * स्थिर तथ्य = k.ptr<T>();

    std::ostringstream stream;
    stream.precision(10);

    अगर (depth <= CV_8S)
    {
        क्रम (पूर्णांक i = 0; i < width; ++i)
            stream << "DIG(" << (पूर्णांक)तथ्य[i] << ")";
        stream << "DIG(" << (पूर्णांक)तथ्य[width] << ")";
    }
    अन्यथा अगर (depth == CV_32F)
    {
        stream.setf(std::ios_base::लिखो_बिंदु);
        क्रम (पूर्णांक i = 0; i < width; ++i)
            stream << "DIG(" << तथ्य[i] << "f)";
        stream << "DIG(" << तथ्य[width] << "f)";
    }
    अन्यथा अगर (depth == CV_16F)
    {
        stream.setf(std::ios_base::लिखो_बिंदु);
        क्रम (पूर्णांक i = 0; i < width; ++i)
            stream << "DIG(" << (भग्न)तथ्य[i] << "h)";
        stream << "DIG(" << (भग्न)तथ्य[width] << "h)";
    }
    अन्यथा
    {
        क्रम (पूर्णांक i = 0; i < width; ++i)
            stream << "DIG(" << तथ्य[i] << ")";
        stream << "DIG(" << तथ्य[width] << ")";
    }

    वापस stream.str();
}

String kernelToStr(निवेश_Array _kernel, पूर्णांक ddepth, स्थिर अक्षर * name)
{
    Mat kernel = _kernel.getMat().reshape(1, 1);

    पूर्णांक depth = kernel.depth();
    अगर (ddepth < 0)
        ddepth = depth;

    // bf16 and fp8 constants have no OpenCL literal form, output them as regular float
    अगर (ddepth == CV_16BF || ddepth == CV_8F_E4M3FN || ddepth == CV_8F_E4M3FNUZ)
        ddepth = CV_32F;

    अगर (ddepth != depth)
        kernel.convertTo(kernel, ddepth);

    प्रकार std::string (* func_t)(स्थिर Mat &);
    अटल स्थिर func_t funcs[CV_DEPTH_MAX] = { kerToStr<uchar>, kerToStr<अक्षर>, kerToStr<ushort>, kerToStr<लघु>,
                                                kerToStr<पूर्णांक>, kerToStr<भग्न>, kerToStr<द्विगुन>, kerToStr<hfloat>,
                                                0, kerToStr<bool>, kerToStr<uint64_t>, kerToStr<int64_t>, kerToStr<अचिन्हित> };
    स्थिर func_t func = funcs[ddepth];
    CV_Assert(func != 0);

    वापस cv::format(" -D %s=%s", name ? name : "COEFF", func(kernel).c_str());
}

#घोषणा PROCESS_SRC(src) \
    करो \
    { \
        अगर (!src.empty()) \
        { \
            CV_Assert(src.isMat() || src.isUMat()); \
            आकार csize = src.size(); \
            पूर्णांक ctype = src.type(), ccn = CV_MAT_CN(ctype), cdepth = CV_MAT_DEPTH(ctype), \
                ckercn = vectorWidths[cdepth], cwidth = ccn * csize.width; \
            अगर (cwidth < ckercn || ckercn <= 0) \
                वापस 1; \
            cols.push_back(cwidth); \
            अगर (strat == OCL_VECTOR_OWN && ctype != ref_type) \
                वापस 1; \
            offsets.push_back(src.offset()); \
            steps.push_back(src.step()); \
            dividers.push_back(ckercn * CV_ELEM_SIZE1(ctype)); \
            kercns.push_back(ckercn); \
        } \
    } \
    जबतक ((व्योम)0, 0)

पूर्णांक predictOptimalVectorWidth(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
                              निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
                              निवेश_Array src7, निवेश_Array src8, निवेश_Array src9,
                              OclVectorStrategy strat)
{
    स्थिर ocl::Device & d = ocl::Device::getDefault();

    // Indexed by depth: must span CV_DEPTH_MAX. bf16 has no OpenCL vector type.
    पूर्णांक vectorWidths[CV_DEPTH_MAX] = {
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
    अगर (vectorWidths[0] == 1)
    {
        // it's heuristic
        vectorWidths[CV_8U] = vectorWidths[CV_8S] = 4;
        vectorWidths[CV_16U] = vectorWidths[CV_16S] = vectorWidths[CV_16F] = 2;
        vectorWidths[CV_32S] = vectorWidths[CV_32F] = vectorWidths[CV_64F] = 1;
    }

    वापस checkOptimalVectorWidth(vectorWidths, src1, src2, src3, src4, src5, src6, src7, src8, src9, strat);
}

पूर्णांक checkOptimalVectorWidth([[maybe_unused]] स्थिर पूर्णांक *vectorWidths,
                            [[maybe_unused]] निवेश_Array src1,
                            [[maybe_unused]] निवेश_Array src2,
                            [[maybe_unused]] निवेश_Array src3,
                            [[maybe_unused]] निवेश_Array src4,
                            [[maybe_unused]] निवेश_Array src5,
                            [[maybe_unused]] निवेश_Array src6,
                            [[maybe_unused]] निवेश_Array src7,
                            [[maybe_unused]] निवेश_Array src8,
                            [[maybe_unused]] निवेश_Array src9,
                            [[maybe_unused]] OclVectorStrategy strat)
{
#अगर_घोषित __APPLE__
    वापस 1;
#अन्यथा
    CV_Assert(vectorWidths);

    पूर्णांक ref_type = src1.type();

    std::vector<माप_प्रकार> offsets, steps, cols;
    std::vector<पूर्णांक> dividers, kercns;
    PROCESS_SRC(src1);
    PROCESS_SRC(src2);
    PROCESS_SRC(src3);
    PROCESS_SRC(src4);
    PROCESS_SRC(src5);
    PROCESS_SRC(src6);
    PROCESS_SRC(src7);
    PROCESS_SRC(src8);
    PROCESS_SRC(src9);

    माप_प्रकार size = offsets.size();

    क्रम (माप_प्रकार i = 0; i < size; ++i)
        जबतक (offsets[i] % dividers[i] != 0 || steps[i] % dividers[i] != 0 || cols[i] % kercns[i] != 0)
            dividers[i] >>= 1, kercns[i] >>= 1;

    // default strategy
    पूर्णांक kercn = *std::min_element(kercns.begin(), kercns.end());

    वापस kercn;
#पूर्ण_अगर
}

पूर्णांक predictOptimalVectorWidthMax(निवेश_Array src1, निवेश_Array src2, निवेश_Array src3,
                                 निवेश_Array src4, निवेश_Array src5, निवेश_Array src6,
                                 निवेश_Array src7, निवेश_Array src8, निवेश_Array src9)
{
    वापस predictOptimalVectorWidth(src1, src2, src3, src4, src5, src6, src7, src8, src9, OCL_VECTOR_MAX);
}

#अघोषित PROCESS_SRC


// TODO Make this as a method of OpenCL "BuildOptions" class
व्योम buildOptionsAddMatrixDescription(String& buildOptions, स्थिर String& name, निवेश_Array _m)
{
    अगर (!buildOptions.empty())
        buildOptions += " ";
    पूर्णांक type = _m.type(), depth = CV_MAT_DEPTH(type);
    buildOptions += format(
            "-D %s_T=%s -D %s_T1=%s -D %s_CN=%d -D %s_TSIZE=%d -D %s_T1SIZE=%d -D %s_DEPTH=%d",
            name.c_str(), ocl::typeToStr(type),
            name.c_str(), ocl::typeToStr(CV_MAKE_TYPE(depth, 1)),
            name.c_str(), (पूर्णांक)CV_MAT_CN(type),
            name.c_str(), (पूर्णांक)CV_ELEM_आकार(type),
            name.c_str(), (पूर्णांक)CV_ELEM_SIZE1(type),
            name.c_str(), (पूर्णांक)depth
            );
}


काष्ठा Image2D::Impl
{
    Impl(स्थिर UMat &src, bool norm, bool alias)
    {
        handle = 0;
        refcount = 1;
        init(src, norm, alias);
    }

    ~Impl()
    {
        अगर (handle)
            clReleaseMemObject(handle);
    }

    अटल cl_image_format getImageFormat(पूर्णांक depth, पूर्णांक cn, bool norm)
    {
        cl_image_format format;
        अटल स्थिर पूर्णांक channelTypes[] = { CL_UNSIGNED_INT8, CL_SIGNED_INT8, CL_UNSIGNED_INT16,
                                       CL_SIGNED_INT16, CL_SIGNED_INT32, CL_FLOAT, -1, CL_HALF_FLOAT };
        अटल स्थिर पूर्णांक channelTypesNorm[] = { CL_UNORM_INT8, CL_SNORM_INT8, CL_UNORM_INT16,
                                                CL_SNORM_INT16, -1, -1, -1, -1 };
        // CL_RGB has no mappings to OpenCV types because CL_RGB can only be used with
        // CL_UNORM_SHORT_565, CL_UNORM_SHORT_555, or CL_UNORM_INT_101010.
        अटल स्थिर पूर्णांक channelOrders[] = { -1, CL_R, CL_RG, /*CL_RGB*/ -1, CL_RGBA };

        पूर्णांक channelType = norm ? channelTypesNorm[depth] : channelTypes[depth];
        पूर्णांक channelOrder = channelOrders[cn];
        format.image_channel_तथ्य_type = (cl_channel_type)channelType;
        format.image_channel_order = (cl_channel_order)channelOrder;
        वापस format;
    }

    अटल bool isFormatSupported(cl_image_format format)
    {
        अगर (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        cl_context context = (cl_context)Context::getDefault().ptr();
        अगर (!context)
            वापस false;

        // Figure out how many formats are supported by this context.
        cl_uint अंक_Formats = 0;
        cl_int err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                                CL_MEM_OBJECT_IMAGE2D, अंक_Formats,
                                                शून्य, &अंक_Formats);
        CV_OCL_DBG_CHECK_परिणाम(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, NULL)");
        अगर (अंक_Formats > 0)
        {
            Auto_बफर<cl_image_format> formats(अंक_Formats);
            err = clGetSupportedImageFormats(context, CL_MEM_READ_WRITE,
                                             CL_MEM_OBJECT_IMAGE2D, अंक_Formats,
                                             formats.तथ्य(), शून्य);
            CV_OCL_DBG_CHECK_परिणाम(err, "clGetSupportedImageFormats(CL_MEM_OBJECT_IMAGE2D, formats)");
            क्रम (cl_uint i = 0; i < अंक_Formats; ++i)
            {
                अगर (!स_भेद(&formats[i], &format, माप(format)))
                {
                    वापस true;
                }
            }
        }
        वापस false;
    }

    व्योम init(स्थिर UMat &src, bool norm, bool alias)
    {
        अगर (!haveOpenCL())
            CV_Error(Error::OpenCLApiCallError, "OpenCL runtime not found!");

        CV_Assert(!src.empty());
        CV_Assert(ocl::Device::getDefault().imageSupport());

        पूर्णांक err, depth = src.depth(), cn = src.channels();
        CV_Assert(cn <= 4);
        cl_image_format format = getImageFormat(depth, cn, norm);

        अगर (!isFormatSupported(format))
            CV_Error(Error::OpenCLApiCallError, "Image format is not supported");

        अगर (alias && !src.handle(ACCESS_RW))
            CV_Error(Error::OpenCLApiCallError, "Incorrect UMat, handle is null");

        cl_context context = (cl_context)Context::getDefault().ptr();
        cl_command_queue queue = (cl_command_queue)Queue::getDefault().ptr();

#अगर_घोषित CL_VERSION_1_2
        // this enables backwards portability to
        // run on OpenCL 1.1 platform if library binaries are compiled with OpenCL 1.2 support
        स्थिर Device & d = ocl::Device::getDefault();
        पूर्णांक minor = d.deviceVersionMinor(), major = d.deviceVersionMajor();
        CV_Assert(!alias || canCreateAlias(src));
        अगर (1 < major || (1 == major && 2 <= minor))
        {
            cl_image_desc desc;
            desc.image_type       = CL_MEM_OBJECT_IMAGE2D;
            desc.image_width      = src.cols;
            desc.image_height     = src.rows;
            desc.image_depth      = 0;
            desc.image_array_आकार = 1;
            desc.image_row_pitch  = alias ? src.step[0] : 0;
            desc.image_slice_pitch = 0;
            desc.बफर           = alias ? (cl_mem)src.handle(ACCESS_RW) : 0;
            desc.अंक_mip_levels   = 0;
            desc.अंक_samples      = 0;
            handle = clCreateImage(context, CL_MEM_READ_WRITE, &format, &desc, शून्य, &err);
        }
        अन्यथा
#पूर्ण_अगर
        {
            CV_SUPPRESS_DEPRECATED_START
            CV_Assert(!alias);  // This is an OpenCL 1.2 extension
            handle = clCreateImage2D(context, CL_MEM_READ_WRITE, &format, src.cols, src.rows, 0, शून्य, &err);
            CV_SUPPRESS_DEPRECATED_END
        }
        CV_OCL_DBG_CHECK_परिणाम(err, "clCreateImage()");

        माप_प्रकार origin[] = { 0, 0, 0 };
        माप_प्रकार region[] = { static_cast<माप_प्रकार>(src.cols), static_cast<माप_प्रकार>(src.rows), 1 };

        cl_mem dev_तथ्य;
        अगर (!alias && !src.isContinuous())
        {
            dev_तथ्य = cl_Create_बफर(context, CL_MEM_READ_ONLY, src.cols * src.rows * src.elem_आकार(), शून्य, &err);
            CV_OCL_CHECK_परिणाम(err, cv::format("clCreateBuffer(CL_MEM_READ_ONLY, sz=%lld) => %p",
                    (दीर्घ दीर्घ पूर्णांक)(src.cols * src.rows * src.elem_आकार()), (व्योम*)dev_तथ्य
                ).c_str());

            स्थिर माप_प्रकार roi[3] = {static_cast<माप_प्रकार>(src.cols) * src.elem_आकार(), static_cast<माप_प्रकार>(src.rows), 1};
            CV_OCL_CHECK(cl_Enqueue_Copy_बफर_Rect(queue, (cl_mem)src.handle(ACCESS_READ), dev_तथ्य, origin, origin,
                roi, src.step, 0, src.cols * src.elem_आकार(), 0, 0, शून्य, शून्य));
            CV_OCL_DBG_CHECK(clFlush(queue));
        }
        अन्यथा
        {
            dev_तथ्य = (cl_mem)src.handle(ACCESS_READ);
        }
        CV_Assert(dev_तथ्य != शून्य);

        अगर (!alias)
        {
            CV_OCL_CHECK(cl_Enqueue_Copy_बफर_To_Image(queue, dev_तथ्य, handle, 0, origin, region, 0, शून्य, 0));
            अगर (!src.isContinuous())
            {
                CV_OCL_DBG_CHECK(clFlush(queue));
                CV_OCL_DBG_CHECK(clReleaseMemObject(dev_तथ्य));
            }
        }
    }

    IMPLEMENT_REFCOUNTABLE();

    cl_mem handle;
};

Image2D::Image2D() CV_NOEXCEPT
{
    p = शून्य;
}

Image2D::Image2D(स्थिर UMat &src, bool norm, bool alias)
{
    p = नया Impl(src, norm, alias);
}

bool Image2D::canCreateAlias(स्थिर UMat &m)
{
    bool ret = false;
    स्थिर Device & d = ocl::Device::getDefault();
    अगर (d.image_From_बफर_Support() && !m.empty())
    {
        // This is the required pitch alignment in pixels
        uint pitchAlign = d.imagePitchAlignment();
        अगर (pitchAlign && !(m.step % (pitchAlign * m.elem_आकार())))
        {
            // We don't currently handle the case where the buffer was created
            // with CL_MEM_USE_HOST_PTR
            अगर (!m.u->अस्थायी_UMat())
            {
                ret = true;
            }
        }
    }
    वापस ret;
}

bool Image2D::isFormatSupported(पूर्णांक depth, पूर्णांक cn, bool norm)
{
    cl_image_format format = Impl::getImageFormat(depth, cn, norm);

    वापस Impl::isFormatSupported(format);
}

Image2D::Image2D(स्थिर Image2D & i)
{
    p = i.p;
    अगर (p)
        p->addref();
}

Image2D & Image2D::चालक = (स्थिर Image2D & i)
{
    अगर (i.p != p)
    {
        अगर (i.p)
            i.p->addref();
        अगर (p)
            p->release();
        p = i.p;
    }
    वापस *यह;
}

Image2D::Image2D(Image2D&& i) CV_NOEXCEPT
{
    p = i.p;
    i.p = nullptr;
}

Image2D& Image2D::चालक = (Image2D&& i) CV_NOEXCEPT
{
    अगर (यह != &i) {
        अगर (p)
            p->release();
        p = i.p;
        i.p = nullptr;
    }
    वापस *यह;
}

Image2D::~Image2D()
{
    अगर (p)
        p->release();
}

व्योम* Image2D::ptr() स्थिर
{
    वापस p ? p->handle : 0;
}

bool internal::isOpenCLForced()
{
    अटल bool initialized = false;
    अटल bool value = false;
    अगर (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_FORCE", false);
        initialized = true;
    }
    वापस value;
}

bool internal::isPerformanceCheckBypassed()
{
    अटल bool initialized = false;
    अटल bool value = false;
    अगर (!initialized)
    {
        value = utils::getConfigurationParameterBool("OPENCV_OPENCL_PERF_CHECK_BYPASS", false);
        initialized = true;
    }
    वापस value;
}

bool internal::isCLBuffer(UMat& u)
{
    अगर (isCudaBacked(u.u))
        वापस false;
    व्योम* h = u.handle(ACCESS_RW);
    अगर (!h)
        वापस true;
    CV_DbgAssert(u.u->currAllocator == getOpenCLAllocator());
#अगर 1
    अगर ((u.u->allocatorFlags_ & 0xffff0000) != 0) // OpenCL SVM flags are stored here
        वापस false;
#अन्यथा
    cl_mem_object_type type = 0;
    cl_int ret = clGetMemObjectInfo((cl_mem)h, CL_MEM_TYPE, माप(type), &type, शून्य);
    अगर (ret != CL_SUCCESS || type != CL_MEM_OBJECT_बफर)
        वापस false;
#पूर्ण_अगर
    वापस true;
}

काष्ठा Timer::Impl
{
    स्थिर Queue queue;

    Impl(स्थिर Queue& q)
        : queue(q)
    {
    }

    ~Impl(){}

    व्योम start()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.start();
    }

    व्योम stop()
    {
        CV_OCL_DBG_CHECK(clFinish((cl_command_queue)queue.ptr()));
        timer.stop();
    }

    uint64 durationNS() स्थिर
    {
        वापस (uint64)(timer.getTimeSec() * 1e9);
    }

    TickMeter timer;
};

Timer::Timer(स्थिर Queue& q) : p(नया Impl(q)) { }
Timer::~Timer() { मिटाओ p; }

व्योम Timer::start()
{
    CV_Assert(p);
    p->start();
}

व्योम Timer::stop()
{
    CV_Assert(p);
    p->stop();
}

uint64 Timer::durationNS() स्थिर
{
    CV_Assert(p);
    वापस p->durationNS();
}

}} // namespace

#पूर्ण_अगर // HAVE_OPENCL
