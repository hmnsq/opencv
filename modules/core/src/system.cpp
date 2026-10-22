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
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2015, Itseez Inc., all rights reserved.
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
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#अन्तर्भाव "precomp.hpp"
//#include "opencv2/core/core_c.h"
#अन्तर्भाव <atomic>
#अन्तर्भाव <exception>
#अन्तर्भाव <iostream>
#अन्तर्भाव <ostream>

#यदि_परिभाषितम् __QNX__
    #अन्तर्भाव <unistd.h>
    #अन्तर्भाव <sys/neutrino.h>
    #अन्तर्भाव <sys/syspage.h>
#यदि_परिभाषितम् __aarch64__
    #अन्तर्भाव <aarch64/syspage.h>
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/trace.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव <opencv2/core/utils/tls.hpp>
#अन्तर्भाव <opencv2/core/utils/instrumentation.hpp>

#अन्तर्भाव <opencv2/core/utils/filesystem.private.hpp>

#अन्तर्भाव <opencv2/core/utils/fp_control_utils.hpp>
#अन्तर्भाव <opencv2/core/utils/fp_control.private.hpp>

नामाकाश cv {

स्थिर शून्य _initSystem()
{
#यदि_परिभाषितम् __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    स्थिर std::ios_base::Init s_iostream_initializer;
#यदि_अन्तः
}

स्थिर Mutex* __initialization_mutex = रिक्त;
Mutex& getInitializationMutex()
{
    यदि (__initialization_mutex == रिक्त)
    {
        (शून्य)_initSystem();
        __initialization_mutex = नूतन Mutex();
    }
    प्रत्यागम *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

स्थिर तार्किक param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#यदि defined(_DEBUG) || defined(__ANDROID__)
    सत्य
#अन्यथा
    असत्य
#यदि_अन्तः
);

शून्य* alloc_Singleton_बफर(size_t size) { प्रत्यागम fastMalloc(size); }
शून्य* alloc_Singleton_New_बफर(size_t size) { प्रत्यागम स्मृतिं_देहि(size); }


} // namespace cv

#यदि_अपरिभाषितम् CV_ERROR_समुच्चयः_TERMINATE_HANDLER  // build config option
# यदि defined(_WIN32)
#   define CV_ERROR_समुच्चयः_TERMINATE_HANDLER 1
# endif
#यदि_अन्तः
#यदि defined(CV_ERROR_समुच्चयः_TERMINATE_HANDLER) && !CV_ERROR_समुच्चयः_TERMINATE_HANDLER
# undef CV_ERROR_समुच्चयः_TERMINATE_HANDLER
#यदि_अन्तः

#यदि_परिभाषितम् _MSC_VER
# यदि _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#यदि_अन्तः

#यदि_परिभाषितम् CV_ERROR_समुच्चयः_TERMINATE_HANDLER
#अन्तर्भाव <cstdlib>        // std::abort
#यदि_अन्तः

#यदि defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#यदि defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
उपयुज्य Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ध्रुव uint64_t AT_HWCAP = NT_GNU_HWCAP;
#अन्यथा
#  include <elf.h>
#यदि_अन्तः
#यदि defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#यदि_अन्तः
#यदि_अन्तः

#यदि defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#यदि_अन्तः


#यदि ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
# include "sys/auxv.h"
# ifndef AT_HWCAP2
#   define AT_HWCAP2 26
# endif
# ifndef PPC_FEATURE2_ARCH_2_07
#   define PPC_FEATURE2_ARCH_2_07 0x80000000
# endif
# ifndef PPC_FEATURE2_ARCH_3_00
#   define PPC_FEATURE2_ARCH_3_00 0x00800000
# endif
# ifndef PPC_FEATURE_HAS_VSX
#   define PPC_FEATURE_HAS_VSX 0x00000080
# endif
#यदि_अन्तः

#यदि defined __loongarch64
#अन्तर्भाव "sys/auxv.h"
#परिभाषय LA_HWCAP_LSX   (1<<4)
#परिभाषय LA_HWCAP_LASX  (1<<5)
#यदि_अन्तः

#यदि defined _WIN32 || defined WINCE
#यदि_अपरिभाषितम् _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #परिभाषय _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#यदि_अन्तः
#अन्तर्भाव <windows.h>
#अन्तर्भाव <combaseapi.h>
#यदि (_WIN32_WINNT >= 0x0602)
  #अन्तर्भाव <synchapi.h>
#यदि_अन्तः
#यदि ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #अन्तर्भाव <fibersapi.h>
  #परिभाषय CV_USE_FLS
#यदि_अन्तः
#अपरिभाषय small
#अपरिभाषय min
#अपरिभाषय max
#अपरिभाषय निरपेक्षम्
#अन्तर्भाव <tchar.h>

#यदि_परिभाषितम् WINRT
#अन्तर्भाव <wrl/client.h>
#यदि_अपरिभाषितम् __cplusplus_winrt
#अन्तर्भाव <windows.storage.h>
#निर्देशः comment(lib, "runtimeobject.lib")
#यदि_अन्तः // WINRT

std::wstring Get_अस्थायी_मार्गः_Win_RT()
{
#यदि_परिभाषितम् __cplusplus_winrt
    प्रत्यागम std::wstring(Windows::Storage::Application_दत्त::Current->TemporaryFolder->मार्गः_2->दत्त_4());
#अन्यथा
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_दत्त_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_दत्त> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_शीर्षकम् hstr_शीर्ष;
    std::wstring wstr;
    यदि (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_दत्त,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_दत्त), &hstr_शीर्ष, &str)))
        प्रत्यागम wstr;
    यदि (FAILED(RoGetActivationFactory(str, IID_PPV_तर्काः(appdataFactory.Release_And_Get_स्थानाङ्कः_सम्बन्धि()))))
        प्रत्यागम wstr;
    यदि (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_स्थानाङ्कः_सम्बन्धि())))
        प्रत्यागम wstr;
    यदि (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_स्थानाङ्कः_सम्बन्धि())))
        प्रत्यागम wstr;
    यदि (FAILED(storagefolderRef.As(&storageitemRef)))
        प्रत्यागम wstr;
    str = रिक्त;
    यदि (FAILED(storageitemRef->get_मार्गः(&str)))
        प्रत्यागम wstr;
    wstr = Windows_Get_String_Raw_बफर(str, रिक्त);
    WindowsDeleteString(str);
    प्रत्यागम wstr;
#यदि_अन्तः
}

std::wstring Get_अस्थायी_File_Name_Win_RT(std::wstring उपसर्गः)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* आवरणम् = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], परिमाण(guidStr)/परिमाण(wchar_t), आवरणम्,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    प्रत्यागम उपसर्गः.append(std::wstring(guidStr));
}

#यदि_अन्तः
#अन्यथा
#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT
#अन्तर्भाव <pthread.h>
#यदि_अन्तः
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>

#यदि defined __MACH__ && defined __APPLE__
#अन्तर्भाव <mach/mach.h>
#अन्तर्भाव <mach/mach_time.h>
#अन्तर्भाव <sys/sysctl.h>
#यदि_अन्तः

#यदि_अन्तः

#यदि_परिभाषितम् _OPENMP
#अन्तर्भाव "omp.h"
#यदि_अन्तः

#यदि defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#यदि defined __ANDROID__
#अन्तर्भाव <sys/sysconf.h>
#यदि_अन्तः
#यदि_अन्तः

#यदि_परिभाषितम् __ANDROID__
# include <android/log.h>
#यदि_अन्तः

#यदि_परिभाषितम् DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#यदि_अन्तः
#यदि_अपरिभाषितम् CV_CPUID_X86
  #यदि defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #यदि _MSC_VER >= 1400  // MSVS 2005
      #अन्तर्भाव <intrin.h>  // __cpuidex()
      #परिभाषय CV_CPUID_X86 __cpuidex
    #अन्यथा
      #दोषः "Required MSVS 2005+"
    #यदि_अन्तः
  #नोचेद्यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    स्थिर शून्य cv_cpuid(पूर्णाङ्क* cpuid_दत्त, पूर्णाङ्क reg_eax, पूर्णाङ्क reg_ecx)
    {
        पूर्णाङ्क __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#यदि !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#नोचेद्यदि defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#अन्यथा
#दोषः "Configuration error"
#यदि_अन्तः
        cpuid_दत्त[0] = __eax; cpuid_दत्त[1] = __ebx; cpuid_दत्त[2] = __ecx; cpuid_दत्त[3] = __edx;
    }
    #परिभाषय CV_CPUID_X86 cv_cpuid
  #यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव <chrono>

नामाकाश cv
{

Exception::Exception() { संहिता = Error::StsOk; पङ्क्तिः = 0; }

Exception::Exception(Error::संहिता_3 संहिता_4, ध्रुव String& _err, ध्रुव String& _func, ध्रुव String& _file, पूर्णाङ्क पङ्क्तिः_3)
: संहिता(संहिता_4), err(_err), func(_func), file(_file), पङ्क्तिः(पङ्क्तिः_3)
{
    format_सन्देशः();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ध्रुव वर्ण* Exception::what() ध्रुव CV_NOEXCEPT { प्रत्यागम सन्देशः.c_str(); }

शून्य Exception::format_सन्देशः()
{
    size_t pos = err.find('\n');
    तार्किक multiline = pos != cv::String::npos;
    यदि (multiline)
    {
        std::stringstream ss;
        size_t पूर्व_pos = 0;
        यावत् (pos != cv::String::npos)
        {
           ss << "> " << err.substr(पूर्व_pos, pos - पूर्व_pos) << std::पङ्क्त्यन्तः;
           पूर्व_pos = pos + 1;
           pos = err.find('\n', पूर्व_pos);
        }
        ss << "> " << err.substr(पूर्व_pos);
        यदि (err[err.size() - 1] != '\n')
            ss << std::पङ्क्त्यन्तः;
        err = ss.str();
    }
    यदि (func.size() > 0)
    {
        यदि (multiline)
            सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_संस्करणम्, file.c_str(), पङ्क्तिः, संहिता, संहिता_सन्देशः(), func.c_str(), err.c_str());
        अन्यथा
            सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_संस्करणम्, file.c_str(), पङ्क्तिः, संहिता, संहिता_सन्देशः(), err.c_str(), func.c_str());
    }
    अन्यथा
    {
        सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_संस्करणम्, file.c_str(), पङ्क्तिः, संहिता, संहिता_सन्देशः(), err.c_str(), multiline ? "" : "\n");
    }
}

ध्रुव वर्ण * Exception::संहिता_सन्देशः() ध्रुव
{
    विकल्प (संहिता)
    {
    पक्ष cv::Error::StsOk :                  प्रत्यागम "No Error";
    पक्ष cv::Error::Sts_पृष्ठभागः_Trace :           प्रत्यागम "Backtrace";
    पक्ष cv::Error::StsError :               प्रत्यागम "Unspecified error";
    पक्ष cv::Error::Sts_आन्तरिकम् :            प्रत्यागम "Internal error";
    पक्ष cv::Error::StsNoMem :               प्रत्यागम "Insufficient memory";
    पक्ष cv::Error::StsBadArg :              प्रत्यागम "Bad argument";
    पक्ष cv::Error::StsNoConv :              प्रत्यागम "Iterations do not converge";
    पक्ष cv::Error::StsAutoTrace :           प्रत्यागम "Autotrace call";
    पक्ष cv::Error::Sts_Bad_आकार :             प्रत्यागम "Incorrect size of input array";
    पक्ष cv::Error::StsNullPtr :             प्रत्यागम "Null pointer";
    पक्ष cv::Error::StsDivByZero :           प्रत्यागम "Division by zero occurred";
    पक्ष cv::Error::Bad_चरणः :                प्रत्यागम "Image step is wrong";
    पक्ष cv::Error::StsInplaceNotSupported : प्रत्यागम "Inplace operation is not supported";
    पक्ष cv::Error::Sts_Object_Not_प्राप्तम् :      प्रत्यागम "Requested object was not found";
    पक्ष cv::Error::Bad_गभीरता :               प्रत्यागम "Input image depth is not supported by function";
    पक्ष cv::Error::StsUnmatchedFormats :    प्रत्यागम "Formats of input arguments do not match";
    पक्ष cv::Error::StsUnmatchedSizes :      प्रत्यागम "Sizes of input arguments do not match";
    पक्ष cv::Error::Sts_बहिर्गमनम्_सम्बन्धि_Range :          प्रत्यागम "One of the arguments\' values is out of range";
    पक्ष cv::Error::StsUnsupportedFormat :   प्रत्यागम "Unsupported format or combination of formats";
    पक्ष cv::Error::BadCOI :                 प्रत्यागम "Input COI is not supported";
    पक्ष cv::Error::Bad_अङ्कः_Channels :         प्रत्यागम "Bad number of channels";
    पक्ष cv::Error::StsBadFlag :             प्रत्यागम "Bad flag (parameter or structure field)";
    पक्ष cv::Error::StsBadPoint :            प्रत्यागम "Bad parameter of type CvPoint";
    पक्ष cv::Error::Sts_Bad_आवरणम् :             प्रत्यागम "Bad type of mask argument";
    पक्ष cv::Error::StsParseError :          प्रत्यागम "Parsing error";
    पक्ष cv::Error::StsNotImplemented :      प्रत्यागम "The function/feature is not implemented";
    पक्ष cv::Error::Sts_Bad_Mem_खण्डम् :         प्रत्यागम "Memory block has been corrupted";
    पक्ष cv::Error::StsAssert :              प्रत्यागम "Assertion failed";
    पक्ष cv::Error::GpuNotSupported :        प्रत्यागम "No CUDA support";
    पक्ष cv::Error::GpuApiCallError :        प्रत्यागम "Gpu API call";
    पक्ष cv::Error::OpenGlNotSupported :     प्रत्यागम "No OpenGL support";
    पक्ष cv::Error::OpenGlApiCallError :     प्रत्यागम "OpenGL API call";
    पक्ष cv::Error::OpenCLApiCallError :     प्रत्यागम "OpenCL API call error";
    पक्ष cv::Error::OpenCLDoubleNotSupported:प्रत्यागम "OpenCL device does not support double";
    पक्ष cv::Error::OpenCLInitError :        प्रत्यागम "OpenCL initialization error";
    पक्ष cv::Error::OpenCLNoAMDBlasFft :     प्रत्यागम "Missing OpenCL AMD BLAS FFT";
    स्वभाव:
    {
        स्थिर वर्ण buf[256] {0};
        snprintf(buf, परिमाण(buf), "Unknown %s code %d", संहिता >= 0 ? "status":"error", संहिता);
        प्रत्यागम buf;
    }
    };

}

स्थिर ध्रुव वर्ण* g_hw_Feature_नामानि[CV_HARDWARE_MAX_FEATURE] = { रिक्त };

स्थिर ध्रुव वर्ण* getHWFeatureName(पूर्णाङ्क id)
{
    प्रत्यागम (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_नामानि[id] : रिक्त;
}
स्थिर ध्रुव वर्ण* getHWFeatureNameSafe(पूर्णाङ्क id)
{
    ध्रुव वर्ण* name = getHWFeatureName(id);
    प्रत्यागम name ? name : "Unknown feature";
}

संरचना HWFeatures
{
    गणना { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(तार्किक सततम्_initialize = असत्य)
    {
        यदि (सततम्_initialize)
            initialize();
    }

    स्थिर शून्य initialize_नामानि()
    {
        प्रति (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_नामानि[i] = 0;
        }
        g_hw_Feature_नामानि[CPU_MMX] = "MMX";
        g_hw_Feature_नामानि[CPU_SSE] = "SSE";
        g_hw_Feature_नामानि[CPU_SSE2] = "SSE2";
        g_hw_Feature_नामानि[CPU_SSE3] = "SSE3";
        g_hw_Feature_नामानि[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_नामानि[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_नामानि[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_नामानि[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_नामानि[CPU_FP16] = "FP16";
        g_hw_Feature_नामानि[CPU_AVX] = "AVX";
        g_hw_Feature_नामानि[CPU_AVX2] = "AVX2";
        g_hw_Feature_नामानि[CPU_FMA3] = "FMA3";

        g_hw_Feature_नामानि[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_नामानि[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_नामानि[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_नामानि[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_नामानि[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_नामानि[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_नामानि[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_नामानि[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_नामानि[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_नामानि[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_नामानि[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_नामानि[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_नामानि[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_नामानि[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_नामानि[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_नामानि[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_नामानि[CPU_NEON] = "NEON";
        g_hw_Feature_नामानि[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_नामानि[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_नामानि[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_नामानि[CPU_SVE] = "SVE";

        g_hw_Feature_नामानि[CPU_VSX] = "VSX";
        g_hw_Feature_नामानि[CPU_VSX3] = "VSX3";

        g_hw_Feature_नामानि[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_नामानि[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_नामानि[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_नामानि[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_नामानि[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_नामानि[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_नामानि[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_नामानि[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_नामानि[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_नामानि[CPU_RVV] = "RVV";
        g_hw_Feature_नामानि[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_नामानि[CPU_LSX]  = "LSX";
        g_hw_Feature_नामानि[CPU_LASX] = "LASX";
    }

    शून्य initialize(शून्य)
    {
        यदि (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_नामानि();

    #यदि_परिभाषितम् CV_CPUID_X86
        पूर्णाङ्क cpuid_दत्त[4] = { 0, 0, 0, 0 };
        पूर्णाङ्क cpuid_दत्त_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_दत्त, 1, 0/*अप्रयुक्तम्*/);

        पूर्णाङ्क x86_family = (cpuid_दत्त[0] >> 8) & 15;
        यदि( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_दत्त[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_दत्त[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_दत्त[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_दत्त[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_दत्त[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_दत्त[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_दत्त[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_दत्त[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_दत्त[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_दत्त[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_दत्त[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_दत्त_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_दत्त_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_दत्त_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_दत्त_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_दत्त_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_दत्त_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_दत्त_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_दत्त_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_दत्त_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_दत्त_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_दत्त_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_दत्त_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_दत्त_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_दत्त_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_दत्त_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_दत्त_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_दत्त_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            पूर्णाङ्क cpuid_दत्त_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_दत्त_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_दत्त_ex1[0] & (1<<4))  != 0;

            तार्किक have_AVX_OS_support = सत्य;
            तार्किक have_AVX512_OS_support = सत्य;
            यदि (!(cpuid_दत्त[2] & (1<<27)))
                have_AVX_OS_support = असत्य; // OS uses XSAVE_XRSTORE and CPU support AVX
            अन्यथा
            {
                पूर्णाङ्क xcr0 = 0;
            #यदि_परिभाषितम् XCR_XFEATURE_ENABLED_आवरणम् // requires immintrin.h
                xcr0 = (पूर्णाङ्क)_xgetbv(XCR_XFEATURE_ENABLED_आवरणम्);
            #नोचेद्यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #यदि_अन्तः
                यदि ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = असत्य; // YMM registers
                यदि ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = असत्य; // ZMM registers
            }

            यदि (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = असत्य;
                have[CV_CPU_FP16] = असत्य;
                have[CV_CPU_AVX2] = असत्य;
                have[CV_CPU_FMA3] = असत्य;
                have[CV_CPU_AVX_VNNI] = असत्य;
            }
            यदि (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = असत्य;
                have[CV_CPU_AVX_512BW] = असत्य;
                have[CV_CPU_AVX_512CD] = असत्य;
                have[CV_CPU_AVX_512DQ] = असत्य;
                have[CV_CPU_AVX_512ER] = असत्य;
                have[CV_CPU_AVX_512IFMA] = असत्य;
                have[CV_CPU_AVX_512PF] = असत्य;
                have[CV_CPU_AVX_512VBMI] = असत्य;
                have[CV_CPU_AVX_512VL] = असत्य;
                have[CV_CPU_AVX_512VBMI2] = असत्य;
                have[CV_CPU_AVX_512VNNI] = असत्य;
                have[CV_CPU_AVX_512BITALG] = असत्य;
                have[CV_CPU_AVX_512VPOPCNTDQ] = असत्य;
                have[CV_CPU_AVX_5124VNNIW] = असत्य;
                have[CV_CPU_AVX_5124FMAPS] = असत्य;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            यदि (have[CV_CPU_AVX512_COMMON])
            {
                have[CV_CPU_AVX512_KNL] = have[CV_CPU_AVX_512ER]  && have[CV_CPU_AVX_512PF];
                have[CV_CPU_AVX512_KNM] = have[CV_CPU_AVX512_KNL] && have[CV_CPU_AVX_5124FMAPS] &&
                                          have[CV_CPU_AVX_5124VNNIW] && have[CV_CPU_AVX_512VPOPCNTDQ];
                have[CV_CPU_AVX512_SKX] = have[CV_CPU_AVX_512BW] && have[CV_CPU_AVX_512DQ] && have[CV_CPU_AVX_512VL];
                have[CV_CPU_AVX512_CNL] = have[CV_CPU_AVX512_SKX] && have[CV_CPU_AVX_512IFMA] && have[CV_CPU_AVX_512VBMI];
                have[CV_CPU_AVX512_CLX] = have[CV_CPU_AVX512_SKX] && have[CV_CPU_AVX_512VNNI];
                have[CV_CPU_AVX512_ICL] = have[CV_CPU_AVX512_SKX] &&
                                          have[CV_CPU_AVX_512IFMA] && have[CV_CPU_AVX_512VBMI] &&
                                          have[CV_CPU_AVX_512VNNI] &&
                                          have[CV_CPU_AVX_512VBMI2] && have[CV_CPU_AVX_512BITALG] && have[CV_CPU_AVX_512VPOPCNTDQ];
            }
            अन्यथा
            {
                have[CV_CPU_AVX512_KNL] = असत्य;
                have[CV_CPU_AVX512_KNM] = असत्य;
                have[CV_CPU_AVX512_SKX] = असत्य;
                have[CV_CPU_AVX512_CNL] = असत्य;
                have[CV_CPU_AVX512_CLX] = असत्य;
                have[CV_CPU_AVX512_ICL] = असत्य;
            }
        }
    #यदि_अन्तः // CV_CPUID_X86

    #यदि defined __ANDROID__ || defined __linux__ || defined __QNX__
    #यदि_परिभाषितम् __aarch64__
        have[CV_CPU_NEON] = सत्य;
        have[CV_CPU_FP16] = सत्य;
        पूर्णाङ्क cpufile = उद्घाटय("/proc/self/auxv", O_RDONLY);

        यदि (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ध्रुव size_t आकार_auxv_t = परिमाण(auxv);

            यावत् ((size_t)पठ(cpufile, &auxv, आकार_auxv_t) == आकार_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                यदि (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#यदि defined(AT_HWCAP2)
                अन्यथा यदि (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#यदि_अन्तः
            }

            close(cpufile);
        }
    #नोचेद्यदि defined __arm__ && defined __ANDROID__
      #यदि defined HAVE_CPUFEATURES
        CV_LOG_INFO(रिक्त, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(रिक्त, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (दीर्घ दीर्घ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #अन्यथा
        CV_LOG_INFO(रिक्त, "cpufeatures library is not available for CPU detection");
        #यदि CV_NEON
        CV_LOG_INFO(रिक्त, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- NEON instructions is NOT enabled via build flags");
        #यदि_अन्तः
        #यदि CV_FP16
        CV_LOG_INFO(रिक्त, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- FP16 instructions is NOT enabled via build flags");
        #यदि_अन्तः
      #यदि_अन्तः
    #नोचेद्यदि defined __arm__
        पूर्णाङ्क cpufile = उद्घाटय("/proc/self/auxv", O_RDONLY);

        यदि (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ध्रुव size_t आकार_auxv_t = परिमाण(auxv);

            यावत् ((size_t)पठ(cpufile, &auxv, आकार_auxv_t) == आकार_auxv_t)
            {
                यदि (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    भङ्ग;
                }
            }

            close(cpufile);
        }
    #यदि_अन्तः
    #नोचेद्यदि (defined __APPLE__)
    #यदि defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
    #यदि_अन्तः
    #यदि (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = सत्य;
    #यदि_अन्तः
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    पूर्णाङ्क has_feat_dotprod = 0;
    size_t has_feat_dotprod_आकार = परिमाण(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_आकार, रिक्त, 0);
    यदि (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = सत्य;
    }
    पूर्णाङ्क has_feat_bf16 = 0;
    size_t has_feat_bf16_आकार = परिमाण(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_आकार, रिक्त, 0);
    यदि (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = सत्य;
    }
    #नोचेद्यदि (defined __clang__)
    #यदि defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
        #यदि (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = सत्य;
        #यदि_अन्तः
    #यदि_अन्तः
    #यदि_अन्तः
    #यदि defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = सत्य;
    #यदि_अन्तः
    #यदि defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = सत्य;
    #यदि_अन्तः
    #यदि_परिभाषितम् __riscv_vector
        have[CV_CPU_RISCVV] = सत्य;
    #यदि_अन्तः
    #यदि_परिभाषितम् __mips_msa
        have[CV_CPU_MSA] = सत्य;
    #यदि_अन्तः

    #यदि (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        अचिह्नित पूर्णाङ्क hwcap = getauxval(AT_HWCAP);
        यदि (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            यदि (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #नोचेद्यदि (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        अचिह्नित दीर्घ hwcap = 0, hwcap2 = 0;
        यदि (elf_aux_info(AT_HWCAP, &hwcap, परिमाण(hwcap)) != 0)
            hwcap = 0;
        यदि (hwcap & PPC_FEATURE_HAS_VSX) {
            यदि (elf_aux_info(AT_HWCAP2, &hwcap2, परिमाण(hwcap2)) != 0)
                hwcap2 = 0;
            यदि (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अन्यथा
        // TODO: AIX
        #यदि CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = सत्य;
        #यदि_अन्तः
        #यदि CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = सत्य;
        #यदि_अन्तः
    #यदि_अन्तः

    #यदि defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = सत्य;
        #यदि (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = सत्य;
        #यदि_अन्तः
        #यदि defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = सत्य;
        #यदि_अन्तः
    #यदि_अन्तः

    #यदि defined __loongarch64 && defined __linux__
        पूर्णाङ्क flag = (पूर्णाङ्क)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #यदि_अन्तः

        तार्किक skip_baseline_check = असत्य;
        यदि (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = सत्य;
        }
        पूर्णाङ्क baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        यदि (!checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]))
            && !skip_baseline_check)
        {
            fprintf(stderr, "\n"
                    "******************************************************************\n"
                    "* FATAL ERROR:                                                   *\n"
                    "* This OpenCV build doesn't support current CPU/HW configuration *\n"
                    "*                                                                *\n"
                    "* Use OPENCV_DUMP_CONFIG=1 environment variable for details      *\n"
                    "******************************************************************\n");
            fprintf(stderr, "\nRequired baseline features:\n");
            checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]), सत्य);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]));
    }

    तार्किक checkFeatures(ध्रुव पूर्णाङ्क* features, पूर्णाङ्क गणनम्_2, तार्किक dump = असत्य)
    {
        तार्किक परिणाम = सत्य;
        प्रति (पूर्णाङ्क i = 0; i < गणनम्_2; i++)
        {
            पूर्णाङ्क feature = features[i];
            यदि (feature)
            {
                यदि (have[feature])
                {
                    यदि (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                अन्यथा
                {
                    परिणाम = असत्य;
                    यदि (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        प्रत्यागम परिणाम;
    }

    स्थिर अन्तःस्थ तार्किक is_चिह्नम्_Separator(वर्ण c)
    {
        प्रत्यागम c == ',' || c == ';';
    }

    शून्य readSettings(ध्रुव पूर्णाङ्क* baseline_features, पूर्णाङ्क baseline_गणनम्)
    {
        तार्किक dump = सत्य;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        यदि (!disabled_features.empty())
        {
            ध्रुव वर्ण* start = disabled_features.c_str();
            प्रति (;;)
            {
                यावत् (start[0] != 0 && is_चिह्नम्_Separator(start[0]))
                {
                    start++;
                }
                यदि (start[0] == 0)
                    भङ्ग;
                ध्रुव वर्ण* end = start;
                यावत् (end[0] != 0 && !is_चिह्नम्_Separator(end[0]))
                {
                    end++;
                }
                यदि (end == start)
                    अनुवर्त;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                तार्किक प्राप्तम् = असत्य;
                प्रति (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    यदि (!g_hw_Feature_नामानि[i]) अनुवर्त;
                    size_t len = वाक्यदैर्घ्यम्(g_hw_Feature_नामानि[i]);
                    यदि (len != feature.size()) अनुवर्त;
                    यदि (feature.compare(g_hw_Feature_नामानि[i]) == 0)
                    {
                        तार्किक isBaseline = असत्य;
                        प्रति (पूर्णाङ्क k = 0; k < baseline_गणनम्; k++)
                        {
                            यदि (baseline_features[k] == i)
                            {
                                isBaseline = सत्य;
                                भङ्ग;
                            }
                        }
                        यदि (isBaseline)
                        {
                            यदि (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        यदि (!have[i])
                        {
                            यदि (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = असत्य;

                        प्राप्तम् = सत्य;
                        भङ्ग;
                    }
                }
                यदि (!प्राप्तम्)
                {
                    यदि (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    तार्किक have[MAX_FEATURE+1]{};
};

स्थिर HWFeatures  featuresEnabled(सत्य), featuresDisabled = HWFeatures(असत्य);
स्थिर HWFeatures* currentFeatures = &featuresEnabled;

तार्किक checkHardwareSupport(पूर्णाङ्क feature)
{
    यदि (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        प्रत्यागम असत्य;
    प्रत्यागम currentFeatures->have[feature];
}

String getHardwareFeatureName(पूर्णाङ्क feature)
{
    ध्रुव वर्ण* name = getHWFeatureName(feature);
    प्रत्यागम name ? String(name) : String();
}

std::string get_CPUFeatures_पङ्क्तिः()
{
    ध्रुव पूर्णाङ्क features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ध्रुव पूर्णाङ्क sz = परिमाण(features) / परिमाण(features[0]);
    std::string परिणाम;
    std::string उपसर्गः;
    प्रति (पूर्णाङ्क i = 1; i < sz; ++i)
    {
        यदि (features[i] == 0)
        {
            उपसर्गः = "*";
            अनुवर्त;
        }
        यदि (i != 1) परिणाम.append(" ");
        परिणाम.append(उपसर्गः);
        परिणाम.append(getHWFeatureNameSafe(features[i]));
        यदि (!checkHardwareSupport(features[i])) परिणाम.append("?");
    }
    प्रत्यागम परिणाम;
}

चञ्चल तार्किक useOptimizedFlag = सत्य;

शून्य समुच्चयः_Use_Optimized( तार्किक flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::समुच्चयः_Use_IPP(flag);
#यदि_परिभाषितम् HAVE_OPENCL
    ocl::समुच्चयः_Use_Open_CL(flag);
#यदि_अन्तः
}

तार्किक useOptimized(शून्य)
{
    प्रत्यागम useOptimizedFlag;
}

int64 get_Tick_गणनम्(शून्य)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    प्रत्यागम (int64)now.time_since_epoch().गणनम्_2();
}

द्विगुण getTickFrequency(शून्य)
{
    उपयुज्य clock_period_t = std::chrono::steady_clock::duration::period;
    द्विगुण clock_freq = clock_period_t::den / clock_period_t::num;
    प्रत्यागम clock_freq;
}

#यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#यदि defined(__i386__)

int64 get_CPUTick_गणनम्(शून्य)
{
    int64 x;
    __asm__ चञ्चल (".byte 0x0f, 0x31" : "=A" (x));
    प्रत्यागम x;
}
#नोचेद्यदि defined(__x86_64__)

int64 get_CPUTick_गणनम्(शून्य)
{
    अचिह्नित hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    प्रत्यागम (int64)lo | ((int64)hi << 32);
}

#नोचेद्यदि defined(__ppc__)

int64 get_CPUTick_गणनम्(शून्य)
{
    अचिह्नित upper, lower, tmp;
    __asm__ चञ्चल(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    प्रत्यागम lower | ((int64)upper << 32);
}

#अन्यथा

#दोषः "RDTSC not defined"

#यदि_अन्तः

#नोचेद्यदि defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_गणनम्(शून्य)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#अन्यथा

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_गणनम्(शून्य)
{
    प्रत्यागम get_Tick_गणनम्();
}
//#endif

#यदि_अन्तः


नामाकाश internal {

वर्ग Timestamp
{
सार्वजनिक:
    ध्रुव int64 zero_Tick_गणनम्;
    ध्रुव द्विगुण ns_अन्तर्_ticks;

    Timestamp()
        : zero_Tick_गणनम्(get_Tick_गणनम्())
        , ns_अन्तर्_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_गणनम्();
        प्रत्यागम (int64)((t - zero_Tick_गणनम्) * ns_अन्तर्_ticks);
    }

    स्थिर Timestamp& getInstance()
    {
        स्थिर Timestamp g_timestamp;
        प्रत्यागम g_timestamp;
    }
};

वर्ग InitTimestamp {
सार्वजनिक:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
स्थिर InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    प्रत्यागम internal::Timestamp::getInstance().getTimestamp();
}


ध्रुव String& getBuildInformation()
{
    स्थिर String build_info =
#अन्तर्भाव "version_string.inc"
    ;
    प्रत्यागम build_info;
}

String get_संस्करणम्_String() { प्रत्यागम String(CV_संस्करणम्); }

पूर्णाङ्क get_संस्करणम्_Major() { प्रत्यागम CV_संस्करणम्_MAJOR; }

पूर्णाङ्क get_संस्करणम्_Minor() { प्रत्यागम CV_संस्करणम्_MINOR; }

पूर्णाङ्क get_संस्करणम्_Revision() { प्रत्यागम CV_संस्करणम्_REVISION; }

String format( ध्रुव वर्ण* fmt, ... )
{
    Auto_बफर<वर्ण, 1024> buf;

    प्रति ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        पूर्णाङ्क bsize = static_cast<पूर्णाङ्क>(buf.size());
        पूर्णाङ्क len = cv_vsnprintf(buf.दत्त(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        यदि (len >= bsize)
        {
            buf.resize(len + 1);
            अनुवर्त;
        }
        buf[bsize - 1] = 0;
        प्रत्यागम String(buf.दत्त(), len);
    }
}

String tempfile( ध्रुव वर्ण* suffix )
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string अस्थायी_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#यदि defined _WIN32
#यदि_परिभाषितम् WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring अस्थायी_dir_rt = Get_अस्थायी_मार्गः_Win_RT();

    std::wstring अस्थायी_file = Get_अस्थायी_File_Name_Win_RT(L"ocv");
    यदि (अस्थायी_file.empty())
        प्रत्यागम String();

    अस्थायी_file = अस्थायी_dir_rt.append(std::wstring(L"\\")).append(अस्थायी_file);
    DeleteFileW(अस्थायी_file.c_str());

    वर्ण aname[MAX_मार्गः];
    size_t copied = wcstombs(aname, अस्थायी_file.c_str(), MAX_मार्गः);
    CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#नोचेद्यदि defined(_WIN32_WCE)
    ध्रुव स्वचालित k_Max_मार्गः_आकार = MAX_मार्गः+1;
    wchar_t अस्थायी_dir_ce[k_Max_मार्गः_आकार] = {0};
    wchar_t अस्थायी_file[k_Max_मार्गः_आकार] = {0};

    ::Get_अस्थायी_मार्गः_W(k_Max_मार्गः_आकार, अस्थायी_dir_ce);

    यदि(0 != ::Get_अस्थायी_File_Name_W(अस्थायी_dir_ce, L"ocv", 0, अस्थायी_file)) {
        DeleteFileW(अस्थायी_file);
        वर्ण aname[MAX_मार्गः];
        size_t copied = wcstombs(aname, अस्थायी_file, MAX_मार्गः);
        CV_Assert((copied != MAX_मार्गः) && (copied != (size_t)-1));
        fname = String(aname);
    }
#अन्यथा
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t अस्थायी_dir2_w[MAX_मार्गः] = { 0 };

    यदि (अस्थायी_dir.empty())
    {
        ::Get_अस्थायी_मार्गः_W(परिमाण(अस्थायी_dir2_w)/परिमाण(wchar_t), अस्थायी_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        पूर्णाङ्क len = Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, अस्थायी_dir2_w, -1, रिक्त, 0, रिक्त, रिक्त);
        यदि (len > 0)
        {
            std::vector<वर्ण> utf8_buf(len);
            Wide_Char_गन्तव्यम्_Multi_Byte(CP_UTF8, 0, अस्थायी_dir2_w, -1, utf8_buf.दत्त(), len, रिक्त, रिक्त);
            अस्थायी_dir = std::string(utf8_buf.दत्त());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    यदि (FAILED(hr))
        प्रत्यागम String();
    वर्ण guidStr[40];
    ध्रुव वर्ण* आवरणम् = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, परिमाण(guidStr), आवरणम्,
            g.Data1, g.Data2, g.Data3, (अचिह्नित पूर्णाङ्क)g.Data4[0], (अचिह्नित पूर्णाङ्क)g.Data4[1],
            (अचिह्नित पूर्णाङ्क)g.Data4[2], (अचिह्नित पूर्णाङ्क)g.Data4[3], (अचिह्नित पूर्णाङ्क)g.Data4[4],
            (अचिह्नित पूर्णाङ्क)g.Data4[5], (अचिह्नित पूर्णाङ्क)g.Data4[6], (अचिह्नित पूर्णाङ्क)g.Data4[7]);

    fname = अस्थायी_dir;
    यदि (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#यदि_अन्तः
# अन्यथा
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    वर्ण defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  अन्यथा
    वर्ण defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    यदि (अस्थायी_dir.empty())
        fname = defaultTemplate;
    अन्यथा
    {
        fname = अस्थायी_dir;
        वर्ण ech = fname[fname.size() - 1];
        यदि(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ध्रुव पूर्णाङ्क fd = mkstemp((वर्ण*)fname.c_str());
    यदि (fd == -1) प्रत्यागम String();

    close(fd);
    remove(fname.c_str());
# endif

    यदि (suffix)
    {
        यदि (suffix[0] != '.')
            प्रत्यागम fname + "." + suffix;
        अन्यथा
            प्रत्यागम fname + suffix;
    }
    प्रत्यागम fname;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_अप्रयुक्तम्(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#यदि_अन्तः // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

स्थिर ErrorCallback customErrorCallback = 0;
स्थिर शून्य* custom_Error_Callback_दत्त = 0;
स्थिर तार्किक breakOnError = असत्य;

तार्किक समुच्चयः_Break_On_Error(तार्किक value)
{
    तार्किक पूर्व_Val = breakOnError;
    breakOnError = value;
    प्रत्यागम पूर्व_Val;
}

पूर्णाङ्क cv_snprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    पूर्णाङ्क res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    प्रत्यागम res;
}

पूर्णाङ्क cv_vsnprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, va_list तर्काः)
{
#यदि defined _MSC_VER
    यदि (len <= 0) प्रत्यागम len == 0 ? 1024 : -1;
    पूर्णाङ्क res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, तर्काः);
    // ensure null terminating on VS
    यदि (res >= 0 && res < len)
    {
        buf[res] = 0;
        प्रत्यागम res;
    }
    अन्यथा
    {
        buf[len - 1] = 0; // truncate happened
        प्रत्यागम res >= len ? res : (len * 2);
    }
#अन्यथा
    प्रत्यागम vsnprintf(buf, len, fmt, तर्काः);
#यदि_अन्तः
}

स्थिर शून्य dumpException(ध्रुव Exception& exc)
{
    ध्रुव वर्ण* errorStr = exc.संहिता_सन्देशः();
    वर्ण buf[1 << 12];

    cv_snprintf(buf, परिमाण(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_संस्करणम्,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.पङ्क्तिः);
#यदि_परिभाषितम् __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#अन्यथा
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#यदि_अन्तः
}

#यदि_परिभाषितम् CV_ERROR_समुच्चयः_TERMINATE_HANDLER
स्थिर तार्किक cv_terminate_handler_installed = असत्य;
स्थिर std::terminate_handler cv_old_terminate_handler;
स्थिर cv::Exception cv_terminate_handler_exception;
स्थिर तार्किक param_सज्जता_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", सत्य);
स्थिर शून्य cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    यदि (असत्य /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#यदि_अन्तः

#यदि_परिभाषितम् __GNUC__
# यदि defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#यदि_अन्तः

शून्य error( ध्रुव Exception& exc )
{
#यदि_परिभाषितम् CV_ERROR_समुच्चयः_TERMINATE_HANDLER
    {
        cv::Auto_तालकम् lock(getInitializationMutex());
        यदि (!cv_terminate_handler_installed)
        {
            यदि (param_सज्जता_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = सत्य;
        }
        cv_terminate_handler_exception = exc;
    }
#यदि_अन्तः

    यदि (customErrorCallback != 0)
        customErrorCallback(exc.संहिता, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.पङ्क्तिः, custom_Error_Callback_दत्त);
    अन्यथा यदि (param_dumpErrors)
    {
        dumpException(exc);
    }

    यदि(breakOnError)
    {
        std::terminate();
    }

    क्षिप exc;
#यदि_परिभाषितम् __GNUC__
# यदि !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#यदि_अन्तः
}

शून्य error(Error::संहिता_3 संहिता_4, ध्रुव String& _err, ध्रुव वर्ण* _func, ध्रुव वर्ण* _file, पूर्णाङ्क पङ्क्तिः_3)
{
    error(cv::Exception(संहिता_4, _err, _func, _file, पङ्क्तिः_3));
#यदि_परिभाषितम् __GNUC__
# यदि !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#यदि_अन्तः
}

#यदि_परिभाषितम् __GNUC__
# यदि defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#यदि_अन्तः


ErrorCallback
redirectError( ErrorCallback errCallback, शून्य* userdata, शून्य** पूर्व_Userdata)
{
    यदि( पूर्व_Userdata )
        *पूर्व_Userdata = custom_Error_Callback_दत्त;

    ErrorCallback पूर्व_Callback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_दत्त = userdata;

    प्रत्यागम पूर्व_Callback;
}

शून्य terminate(Error::संहिता_3 संहिता_4, ध्रुव String& _err, ध्रुव वर्ण* _func, ध्रुव वर्ण* _file, पूर्णाङ्क पङ्क्तिः_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(संहिता_4, _err, _func, _file, पङ्क्तिः_3));
    std::terminate();
}

}

नामाकाश cv {
तार्किक __termination = असत्य;


//////////////////////////////// thread-local storage ////////////////////////////////

नामाकाश details {

#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT

#यदि_परिभाषितम् _WIN32
#यदि_परिभाषितम् _MSC_VER
#निर्देशः warning(disable:4505) // unreferenced local function has been removed
#यदि_अन्तः
#यदि_अपरिभाषितम् TLS_बहिर्गमनम्_सम्बन्धि_INDEXES
#परिभाषय TLS_बहिर्गमनम्_सम्बन्धि_INDEXES ((DWORD)0xFFFFFFFF)
#यदि_अन्तः
#यदि_अन्तः

// TLS platform abstraction layer
वर्ग TlsAbstraction
{
सार्वजनिक:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    शून्य* get_दत्त() ध्रुव;
    शून्य समुच्चयः_दत्त(शून्य *p_दत्त);

    शून्य releaseSystemResources();

गुप्त:

#यदि_परिभाषितम् _WIN32
#यदि_अपरिभाषितम् WINRT
    DWORD tls_कुञ्चिका;
    तार्किक disposed;
#यदि_अन्तः
#अन्यथा // _WIN32
    pthread_key_t  tls_कुञ्चिका;
    std::atomic<तार्किक> disposed;
#यदि_अन्तः
};

वर्ग TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
सार्वजनिक:
    TlsAbstractionReleaseGuard(TlsAbstraction& tls) : tls_(tls)
    {
        /* nothing */
    }
    ~TlsAbstractionReleaseGuard()
    {
        tls_.releaseSystemResources();
    }
};

// TODO use reference
स्थिर TlsAbstraction* getTlsAbstraction()
{
    स्थिर TlsAbstraction *g_tls = नूतन TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    स्थिर TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    प्रत्यागम g_tls;
}


#यदि_परिभाषितम् _WIN32
#यदि_परिभाषितम् WINRT
स्थिर __declspec( thread ) शून्य* tls_दत्त = रिक्त; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
}
शून्य* TlsAbstraction::get_दत्त() ध्रुव
{
    प्रत्यागम tls_दत्त;
}
शून्य TlsAbstraction::समुच्चयः_दत्त(शून्य *p_दत्त)
{
    tls_दत्त = p_दत्त;
}
#अन्यथा //WINRT
#यदि_परिभाषितम् CV_USE_FLS
स्थिर शून्य NTAPI opencv_fls_destructor(शून्य* p_दत्त);
#यदि_अन्तः // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(असत्य)
{
#यदि_अपरिभाषितम् CV_USE_FLS
    tls_कुञ्चिका = TlsAlloc();
#अन्यथा // CV_USE_FLS
    tls_कुञ्चिका = FlsAlloc(opencv_fls_destructor);
#यदि_अन्तः // CV_USE_FLS
    CV_Assert(tls_कुञ्चिका != TLS_बहिर्गमनम्_सम्बन्धि_INDEXES);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
#यदि_अपरिभाषितम् CV_USE_FLS
    TlsFree(tls_कुञ्चिका);
#अन्यथा // CV_USE_FLS
    FlsFree(tls_कुञ्चिका);
#यदि_अन्तः // CV_USE_FLS
    tls_कुञ्चिका = TLS_बहिर्गमनम्_सम्बन्धि_INDEXES;
}
शून्य* TlsAbstraction::get_दत्त() ध्रुव
{
    यदि (disposed)
        प्रत्यागम रिक्त;
#यदि_अपरिभाषितम् CV_USE_FLS
    प्रत्यागम Tls_Get_मानम्(tls_कुञ्चिका);
#अन्यथा // CV_USE_FLS
    प्रत्यागम Fls_Get_मानम्(tls_कुञ्चिका);
#यदि_अन्तः // CV_USE_FLS
}
शून्य TlsAbstraction::समुच्चयः_दत्त(शून्य *p_दत्त)
{
    यदि (disposed)
        प्रत्यागम;  // no-op
#यदि_अपरिभाषितम् CV_USE_FLS
    CV_Assert(Tls_समुच्चयः_मानम्(tls_कुञ्चिका, p_दत्त) == TRUE);
#अन्यथा // CV_USE_FLS
    CV_Assert(Fls_समुच्चयः_मानम्(tls_कुञ्चिका, p_दत्त) == TRUE);
#यदि_अन्तः // CV_USE_FLS
}
#यदि_अन्तः // WINRT
#अन्यथा // _WIN32
स्थिर शून्य opencv_tls_destructor(शून्य* p_दत्त);
TlsAbstraction::TlsAbstraction()
    : disposed(असत्य)
{
    CV_Assert(pthread_key_create(&tls_कुञ्चिका, opencv_tls_destructor) == 0);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
    यदि (pthread_key_delete(tls_कुञ्चिका) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
शून्य* TlsAbstraction::get_दत्त() ध्रुव
{
    यदि (disposed)
        प्रत्यागम रिक्त;
    प्रत्यागम pthread_getspecific(tls_कुञ्चिका);
}
शून्य TlsAbstraction::समुच्चयः_दत्त(शून्य *p_दत्त)
{
    यदि (disposed)
        प्रत्यागम;  // no-op
    CV_Assert(pthread_setspecific(tls_कुञ्चिका, p_दत्त) == 0);
}
#यदि_अन्तः

// Per-thread data structure
संरचना Thread_दत्त
{
    Thread_दत्त()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<शून्य*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


स्थिर तार्किक g_isTlsStorageInitialized = असत्य;

// Main TLS storage class
वर्ग TlsStorage
{
सार्वजनिक:
    TlsStorage() :
        tls_Slots_आकार(0)
    {
        (शून्य)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        तन्तवः.reserve(32);
        g_isTlsStorageInitialized = सत्य;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    शून्य releaseThread(शून्य* tls_मानम् = रिक्त)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            प्रत्यागम;  // TLS singleton is not available (terminated)
        Thread_दत्त *pTD = tls_मानम् == रिक्त ? (Thread_दत्त*)tls->get_दत्त() : (Thread_दत्त*)tls_मानम्;
        यदि (pTD == रिक्त)
            प्रत्यागम;  // no OpenCV TLS data for this thread
        Auto_तालकम् guard(mtxGlobalAccess);
        प्रति (size_t i = 0; i < तन्तवः.size(); i++)
        {
            यदि (pTD == तन्तवः[i])
            {
                तन्तवः[i] = रिक्त;
                यदि (tls_मानम् == रिक्त)
                    tls->समुच्चयः_दत्त(0);
                std::vector<शून्य*>& thread_slots = pTD->slots;
                प्रति (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    शून्य* p_दत्त = thread_slots[slotIdx];
                    thread_slots[slotIdx] = रिक्त;
                    यदि (!p_दत्त)
                        अनुवर्त;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    यदि (container)
                        container->delete_दत्त_Instance(p_दत्त);
                    अन्यथा
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (पूर्णाङ्क)slotIdx);
                        fflush(stderr);
                    }
                }
                विलोपय pTD;
                प्रत्यागम;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (शून्य*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        Auto_तालकम् guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());

        // Find unused slots
        प्रति(size_t slot = 0; slot < tls_Slots_आकार; slot++)
        {
            यदि (tlsSlots[slot].container == रिक्त)
            {
                tlsSlots[slot].container = container;
                प्रत्यागम slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tls_Slots_आकार++;
        प्रत्यागम tls_Slots_आकार - 1;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(size_t slotIdx, std::vector<शून्य*> &दत्त_Vec, तार्किक keepSlot = असत्य)
    {
        Auto_तालकम् guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());
        CV_Assert(tls_Slots_आकार > slotIdx);

        प्रति(size_t i = 0; i < तन्तवः.size(); i++)
        {
            यदि(तन्तवः[i])
            {
                std::vector<शून्य*>& thread_slots = तन्तवः[i]->slots;
                यदि (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    दत्त_Vec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = रिक्त;
                }
            }
        }

        यदि (!keepSlot)
        {
            tlsSlots[slotIdx].container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* get_दत्त(size_t slotIdx) ध्रुव
    {
#यदि_अपरिभाषितम् CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_आकार > slotIdx);
#यदि_अन्तः

        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            प्रत्यागम रिक्त;  // TLS singleton is not available (terminated)

        Thread_दत्त* thread_दत्त = (Thread_दत्त*)tls->get_दत्त();
        यदि(thread_दत्त && thread_दत्त->slots.size() > slotIdx)
            प्रत्यागम thread_दत्त->slots[slotIdx];

        प्रत्यागम रिक्त;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(size_t slotIdx, std::vector<शून्य*> &दत्त_Vec)
    {
        Auto_तालकम् guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());
        CV_Assert(tls_Slots_आकार > slotIdx);

        प्रति(size_t i = 0; i < तन्तवः.size(); i++)
        {
            यदि(तन्तवः[i])
            {
                std::vector<शून्य*>& thread_slots = तन्तवः[i]->slots;
                यदि (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    दत्त_Vec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    शून्य समुच्चयः_दत्त(size_t slotIdx, शून्य* p_दत्त)
    {
#यदि_अपरिभाषितम् CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_आकार > slotIdx);
#यदि_अन्तः

        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            प्रत्यागम;  // TLS singleton is not available (terminated)

        Thread_दत्त* thread_दत्त = (Thread_दत्त*)tls->get_दत्त();
        यदि(!thread_दत्त)
        {
            thread_दत्त = नूतन Thread_दत्त;
            tls->समुच्चयः_दत्त((शून्य*)thread_दत्त);
            {
                Auto_तालकम् guard(mtxGlobalAccess);

                तार्किक प्राप्तम् = असत्य;
                // Find unused slots
                प्रति(size_t slot = 0; slot < तन्तवः.size(); slot++)
                {
                    यदि (तन्तवः[slot] == रिक्त)
                    {
                        thread_दत्त->idx = (पूर्णाङ्क)slot;
                        तन्तवः[slot] = thread_दत्त;
                        प्राप्तम् = सत्य;
                        भङ्ग;
                    }
                }

                यदि (!प्राप्तम्)
                {
                    // Create new slot
                    thread_दत्त->idx = तन्तवः.size();
                    तन्तवः.push_back(thread_दत्त);
                }
            }
        }

        यदि(slotIdx >= thread_दत्त->slots.size())
        {
            Auto_तालकम् guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_दत्त->slots.resize(slotIdx + 1, रिक्त);
        }
        thread_दत्त->slots[slotIdx] = p_दत्त;
    }

गुप्त:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_आकार;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    संरचना TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<संरचना TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<Thread_दत्त*> तन्तवः; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
स्थिर TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, नूतन TlsStorage())
}

#यदि_अपरिभाषितम् _WIN32  // pthread key destructor
स्थिर शून्य opencv_tls_destructor(शून्य* p_दत्त)
{
    यदि (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_दत्त);
}
#अन्यथा // _WIN32
#यदि_परिभाषितम् CV_USE_FLS
स्थिर शून्य WINAPI opencv_fls_destructor(शून्य* p_दत्त)
{
    // Empiric detection of ExitProcess call
    DWORD संहिता = STILL_सक्रिय/*259*/;
    BOOL res = Get_Exit_संहिता_Process(GetCurrentProcess(), &संहिता);
    यदि (res && संहिता != STILL_सक्रिय)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = सत्य;
    }

    यदि (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_दत्त);
}
#यदि_अन्तः // CV_USE_FLS
#यदि_अन्तः // _WIN32

स्थिर TlsStorage* ध्रुव g_force_initialization_सम्बन्धि_Tls_Storage
#यदि defined __GNUC__
    __attribute__((अप्रयुक्तम्))
#यदि_अन्तः
    = &getTlsStorage();


#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
वर्ग TlsStorage
{
सार्वजनिक:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        प्रति (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            यदि (container && s.दत्त)
            {
                container->delete_दत्त_Instance(s.दत्त);  // Can't use from SlotInfo destructor
                s.दत्त = शून्यसूचकः;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        size_t slots_आकार = slots.size();
        प्रति (size_t slot = 0; slot < slots_आकार; slot++)
        {
            SlotInfo& s = slots[slot];
            यदि (s.container == रिक्त)
            {
                CV_Assert(!s.दत्त);
                s.container = container;
                प्रत्यागम slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        प्रत्यागम slots_आकार;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(size_t slotIdx, std::vector<शून्य*> &दत्त_Vec, तार्किक keepSlot = असत्य)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* दत्त = s.दत्त;
        यदि (दत्त)
        {
            दत्त_Vec.push_back(दत्त);
            s.दत्त = शून्यसूचकः;
        }
        यदि (!keepSlot)
        {
            s.container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* get_दत्त(size_t slotIdx) ध्रुव
    {
        CV_Assert(slotIdx < slots.size());
        ध्रुव SlotInfo& s = slots[slotIdx];
        प्रत्यागम s.दत्त;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(size_t slotIdx, std::vector<शून्य*> &दत्त_Vec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* दत्त = s.दत्त;
        यदि (दत्त)
            दत्त_Vec.push_back(दत्त);
        प्रत्यागम;
    }

    // Set data to storage index
    शून्य समुच्चयः_दत्त(size_t slotIdx, शून्य* p_दत्त)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.दत्त = p_दत्त;
    }

गुप्त:
    संरचना SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), दत्त(शून्यसूचकः) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        शून्य* दत्त;
    };
    std::vector<संरचना SlotInfo> slots;
};

स्थिर TlsStorage& getTlsStorage()
{
    स्थिर TlsStorage g_storage;  // no threading
    प्रत्यागम g_storage;
}

#यदि_अन्तः  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
उपयुज्य नामाकाश details;

शून्य releaseTlsStorageThread()
{
#यदि_अपरिभाषितम् OPENCV_DISABLE_THREAD_SUPPORT
    यदि (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#यदि_अन्तः
}

TLSDataContainer::TLSDataContainer()
{
    कुञ्चिका_2 = (पूर्णाङ्क)getTlsStorage().reserveSlot(एतत्); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(कुञ्चिका_2 == -1); // Key must be released in child object
}

शून्य TLSDataContainer::gather_दत्त(std::vector<शून्य*> &दत्त) ध्रुव
{
    getTlsStorage().gather(कुञ्चिका_2, दत्त);
}

शून्य TLSDataContainer::detach_दत्त(std::vector<शून्य*> &दत्त)
{
    getTlsStorage().releaseSlot(कुञ्चिका_2, दत्त, सत्य);
}

शून्य TLSDataContainer::release()
{
    यदि (कुञ्चिका_2 == -1)
        प्रत्यागम;  // already released
    std::vector<शून्य*> दत्त; दत्त.reserve(32);
    getTlsStorage().releaseSlot(कुञ्चिका_2, दत्त, असत्य); // Release key and get stored data for proper destruction
    कुञ्चिका_2 = -1;
    प्रति(size_t i = 0; i < दत्त.size(); i++)  // Delete all associated data
        delete_दत्त_Instance(दत्त[i]);
}

शून्य TLSDataContainer::cleanup()
{
    std::vector<शून्य*> दत्त; दत्त.reserve(32);
    getTlsStorage().releaseSlot(कुञ्चिका_2, दत्त, सत्य); // Extract stored data with removal from TLS tables
    प्रति(size_t i = 0; i < दत्त.size(); i++)  // Delete all associated data
        delete_दत्त_Instance(दत्त[i]);
}

शून्य* TLSDataContainer::get_दत्त() ध्रुव
{
    CV_Assert(कुञ्चिका_2 != -1 && "Can't fetch data from terminated TLS container.");
    शून्य* p_दत्त = getTlsStorage().get_दत्त(कुञ्चिका_2); // Check if data was already allocated
    यदि(!p_दत्त)
    {
        // Create new data instance and save it to TLS storage
        p_दत्त = create_दत्त_Instance();
        यत्न
        {
            getTlsStorage().समुच्चयः_दत्त(कुञ्चिका_2, p_दत्त);
        }
        गृहाण (...)
        {
            delete_दत्त_Instance(p_दत्त);
            क्षिप;
        }
    }
    प्रत्यागम p_दत्त;
}

स्थिर TLSData<CoreTLSData>& get_Core_Tls_दत्त_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, नूतन TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_दत्त()
{
    प्रत्यागम get_Core_Tls_दत्त_TLS().getRef();
}

#यदि defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#यदि_परिभाषितम् WINRT
    #निर्देशः warning(disable:4447) // Disable warning 'main' signature found without threading model
#यदि_अन्तः

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    यदि (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        यदि (lpReserved != रिक्त) // called after ExitProcess() call
        {
            cv::__termination = सत्य;
        }
        अन्यथा
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    प्रत्यागम TRUE;
}
#यदि_अन्तः


नामाकाश {

#यदि_परिभाषितम् OPENCV_WITH_ITT
तार्किक overrideThreadName()
{
    स्थिर तार्किक param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", असत्य);
    प्रत्यागम param;
}
#यदि_अन्तः

स्थिर पूर्णाङ्क g_thread_अङ्कः = 0;
वर्ग ThreadID {
सार्वजनिक:
    ध्रुव पूर्णाङ्क id;
    ThreadID() :
        id(CV_XADD(&g_thread_अङ्कः, 1))
    {
#यदि_परिभाषितम् OPENCV_WITH_ITT
        यदि (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#यदि_अन्तः
    }
};

स्थिर TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, नूतन TLSData<ThreadID>());
}

} // namespace
पूर्णाङ्क utils::getThreadID() { प्रत्यागम getThreadIDTLS().get()->id; }


वर्ग ParseError
{
    std::string bad_मानम्;
सार्वजनिक:
    ParseError(ध्रुव std::string &bad_मानम्_2) :bad_मानम्(bad_मानम्_2) {}
    std::string गन्तव्यम्_String(ध्रुव std::string &param) ध्रुव
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_मानम्;
        प्रत्यागम out.str();
    }
};

प्रतिरूपम् <प्रकारनाम T>
T parseOption(ध्रुव std::string &);

प्रतिरूपम्<>
अन्तःस्थ तार्किक parseOption(ध्रुव std::string & value)
{
    यदि (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        प्रत्यागम सत्य;
    }
    यदि (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        प्रत्यागम असत्य;
    }
    क्षिप ParseError(value);
}

प्रतिरूपम्<>
अन्तःस्थ size_t parseOption(ध्रुव std::string &value)
{
    size_t pos = 0;
    प्रति (; pos < value.size(); pos++)
    {
        यदि (!अङ्कः_वा(value[pos]))
            भङ्ग;
    }
    cv::String मानम्_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(मानम्_Str);
    यदि (suffixStr.length() == 0)
        प्रत्यागम v;
    अन्यथा यदि (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        प्रत्यागम v * 1024 * 1024;
    अन्यथा यदि (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        प्रत्यागम v * 1024;
    क्षिप ParseError(value);
}

प्रतिरूपम्<>
अन्तःस्थ cv::String parseOption(ध्रुव std::string &value)
{
    प्रत्यागम value;
}

प्रतिरूपम्<>
अन्तःस्थ utils::Paths parseOption(ध्रुव std::string &value)
{
    utils::Paths परिणाम;
#यदि_परिभाषितम् _WIN32
    ध्रुव वर्ण sep = ';';
#अन्यथा
    ध्रुव वर्ण sep = ':';
#यदि_अन्तः
    size_t start_pos = 0;
    यावत् (start_pos != std::string::npos)
    {
        ध्रुव size_t pos = value.find(sep, start_pos);
        ध्रुव std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        यदि (!one_piece.empty())
            परिणाम.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    प्रत्यागम परिणाम;
}

स्थिर अन्तःस्थ ध्रुव वर्ण * envRead(ध्रुव वर्ण * name)
{
#यदि_परिभाषितम् NO_GETENV
    CV_अप्रयुक्तम्(name);
    प्रत्यागम रिक्त;
#अन्यथा
    प्रत्यागम getenv(name);
#यदि_अन्तः
}

प्रतिरूपम्<प्रकारनाम T>
अन्तःस्थ T पठ(ध्रुव std::string & k, ध्रुव T & default_मानम्_2)
{
    यत्न
    {
        ध्रुव वर्ण * res = envRead(k.c_str());
        यदि (res)
            प्रत्यागम parseOption<T>(std::string(res));
    }
    गृहाण (ध्रुव ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.गन्तव्यम्_String(k));
    }
    प्रत्यागम default_मानम्_2;
}

तार्किक utils::getConfigurationParameterBool(ध्रुव वर्ण* name, तार्किक default_मानम्_2)
{
    प्रत्यागम पठ<तार्किक>(name, default_मानम्_2);
}

size_t utils::get_Configuration_Parameter_आकार_T(ध्रुव वर्ण* name, size_t default_मानम्_2)
{
    प्रत्यागम पठ<size_t>(name, default_मानम्_2);
}

std::string utils::getConfigurationParameterString(ध्रुव वर्ण* name, ध्रुव std::string & default_मानम्_2)
{
    प्रत्यागम पठ<cv::String>(name, default_मानम्_2);
}

utils::Paths utils::getConfigurationParameterPaths(ध्रुव वर्ण* name, ध्रुव utils::Paths &default_मानम्_2)
{
    प्रत्यागम पठ<utils::Paths>(name, default_मानम्_2);
}


#यदि_परिभाषितम् CV_COLLECT_IMPL_दत्त
ImplCollector& get_Impl_दत्त()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, नूतन ImplCollector())
}

शून्य समुच्चयः_Impl(पूर्णाङ्क flags)
{
    cv::Auto_तालकम् lock(get_Impl_दत्त().mutex);

    get_Impl_दत्त().implFlags = flags;
    get_Impl_दत्त().impl_संहिता.clear();
    get_Impl_दत्त().implFun.clear();
}

शून्य सङ्कलय_Impl(पूर्णाङ्क flag, ध्रुव वर्ण* func)
{
    cv::Auto_तालकम् lock(get_Impl_दत्त().mutex);

    get_Impl_दत्त().implFlags |= flag;
    यदि(func) // use lazy collection if name was not specified
    {
        size_t सूचकांक = get_Impl_दत्त().impl_संहिता.size();
        यदि(!सूचकांक || (get_Impl_दत्त().impl_संहिता[सूचकांक-1] != flag || get_Impl_दत्त().implFun[सूचकांक-1].compare(func))) // avoid duplicates
        {
            get_Impl_दत्त().impl_संहिता.push_back(flag);
            get_Impl_दत्त().implFun.push_back(func);
        }
    }
}

पूर्णाङ्क getImpl(std::vector<पूर्णाङ्क> &impl, std::vector<String> &funName)
{
    cv::Auto_तालकम् lock(get_Impl_दत्त().mutex);

    impl    = get_Impl_दत्त().impl_संहिता;
    funName = get_Impl_दत्त().implFun;
    प्रत्यागम get_Impl_दत्त().implFlags; // return actual flags for lazy collection
}

तार्किक useCollection()
{
    प्रत्यागम get_Impl_दत्त().useCollection;
}

शून्य समुच्चयः_Use_Collection(तार्किक flag)
{
    cv::Auto_तालकम् lock(get_Impl_दत्त().mutex);

    get_Impl_दत्त().useCollection = flag;
}
#यदि_अन्तः

नामाकाश instr
{
तार्किक useInstrumentation()
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    प्रत्यागम getInstrumentStruct().useInstr;
#अन्यथा
    प्रत्यागम असत्य;
#यदि_अन्तः
}

शून्य समुच्चयः_Use_Instrumentation(तार्किक flag)
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#अन्यथा
    CV_अप्रयुक्तम्(flag);
#यदि_अन्तः
}

Instr_ग्रन्थिः* getTrace()
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    प्रत्यागम &getInstrumentStruct().मूलम्_ग्रन्थिः_2;
#अन्यथा
    प्रत्यागम रिक्त;
#यदि_अन्तः
}

शून्य resetTrace()
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    getInstrumentStruct().मूलम्_ग्रन्थिः_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ग्रन्थिः = &getInstrumentStruct().मूलम्_ग्रन्थिः_2;
#यदि_अन्तः
}

शून्य समुच्चयः_Flags(FLAGS बहुलकम्_Flags)
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = बहुलकम्_Flags;
#अन्यथा
    CV_अप्रयुक्तम्(बहुलकम्_Flags);
#यदि_अन्तः
}
FLAGS getFlags()
{
#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
    प्रत्यागम (FLAGS)getInstrumentStruct().flags;
#अन्यथा
    प्रत्यागम (FLAGS)0;
#यदि_अन्तः
}

ग्रन्थिः_दत्त::ग्रन्थिः_दत्त(ध्रुव वर्ण* funName, ध्रुव वर्ण* fileName, पूर्णाङ्क पङ्क्तिः_अङ्कः, शून्य* ret_स्थानाङ्कः, तार्किक alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_पङ्क्तिः_अङ्कः       = पङ्क्तिः_अङ्कः;
    m_ret_स्थानाङ्कः    = ret_स्थानाङ्कः;
    m_alwaysExpand  = alwaysExpand;

    m_तन्तवः    = 1;
    m_गणक    = 0;
    m_ticks_योग = 0;

    m_funError  = असत्य;
}
ग्रन्थिः_दत्त::ग्रन्थिः_दत्त(ग्रन्थिः_दत्त &ref)
{
    *एतत् = ref;
}
ग्रन्थिः_दत्त& ग्रन्थिः_दत्त::संक्रियकः=(ध्रुव ग्रन्थिः_दत्त &दक्षिणतः)
{
    एतत्->m_funName      = दक्षिणतः.m_funName;
    एतत्->m_instrType    = दक्षिणतः.m_instrType;
    एतत्->m_implType     = दक्षिणतः.m_implType;
    एतत्->m_fileName     = दक्षिणतः.m_fileName;
    एतत्->m_पङ्क्तिः_अङ्कः      = दक्षिणतः.m_पङ्क्तिः_अङ्कः;
    एतत्->m_ret_स्थानाङ्कः   = दक्षिणतः.m_ret_स्थानाङ्कः;
    एतत्->m_alwaysExpand = दक्षिणतः.m_alwaysExpand;

    एतत्->m_तन्तवः     = दक्षिणतः.m_तन्तवः;
    एतत्->m_गणक     = दक्षिणतः.m_गणक;
    एतत्->m_ticks_योग  = दक्षिणतः.m_ticks_योग;

    एतत्->m_funError    = दक्षिणतः.m_funError;

    प्रत्यागम *एतत्;
}
ग्रन्थिः_दत्त::~ग्रन्थिः_दत्त()
{
}
तार्किक संक्रियकः==(ध्रुव ग्रन्थिः_दत्त& वामतः, ध्रुव ग्रन्थिः_दत्त& दक्षिणतः)
{
    यदि(वामतः.m_पङ्क्तिः_अङ्कः == दक्षिणतः.m_पङ्क्तिः_अङ्कः && वामतः.m_funName == दक्षिणतः.m_funName && वामतः.m_fileName == दक्षिणतः.m_fileName)
    {
        यदि(वामतः.m_ret_स्थानाङ्कः == दक्षिणतः.m_ret_स्थानाङ्कः || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_नामानि || वामतः.m_alwaysExpand))
            प्रत्यागम सत्य;
    }
    प्रत्यागम असत्य;
}

#यदि_परिभाषितम् ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    स्थिर InstrStruct instr;
    प्रत्यागम instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    प्रत्यागम *getInstrumentStruct().tlsStruct.get();
}

Instr_ग्रन्थिः* get_Current_ग्रन्थिः()
{
    प्रत्यागम getInstrumentTLSStruct().p_Current_ग्रन्थिः;
}

IntrumentationRegion::IntrumentationRegion(ध्रुव वर्ण* funName, ध्रुव वर्ण* fileName, पूर्णाङ्क पङ्क्तिः_अङ्कः, शून्य *ret_स्थानाङ्कः, तार्किक alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = असत्य;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    यदि(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        यदि(!pTLS->p_Current_ग्रन्थिः)
        {
            m_disabled = सत्य;
            प्रत्यागम;
        }

        पूर्णाङ्क गभीरता = pTLS->p_Current_ग्रन्थिः->get_गभीरता();
        यदि(pStruct->max_गभीरता && pStruct->max_गभीरता <= गभीरता)
        {
            m_disabled = सत्य;
            प्रत्यागम;
        }

        ग्रन्थिः_दत्त payload(funName, fileName, पङ्क्तिः_अङ्कः, ret_स्थानाङ्कः, alwaysExpand, instrType, implType);
        Node<ग्रन्थिः_दत्त>* pChild = रिक्त;

        यदि(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_तालकम् guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_ग्रन्थिः->findChild(payload);
            यदि(!pChild)
            {
                pChild = नूतन Node<ग्रन्थिः_दत्त>(payload);
                pTLS->p_Current_ग्रन्थिः->सङ्कलय_Child(pChild);
            }
        }
        अन्यथा
        {
            pChild = pTLS->p_Current_ग्रन्थिः->findChild(payload);
            यदि(!pChild)
            {
                m_disabled = सत्य;
                प्रत्यागम;
            }
        }
        pTLS->p_Current_ग्रन्थिः = pChild;

        m_regionTicks = get_Tick_गणनम्();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    यदि(pStruct->useInstr)
    {
        यदि(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            यदि (pTLS->p_Current_ग्रन्थिः->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ग्रन्थिः->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ग्रन्थिः->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_गणनम्() - m_regionTicks);
            {
                cv::Auto_तालकम् guard(pStruct->mutex_गणनम्); // Concurrent ticks accumulation
                pTLS->p_Current_ग्रन्थिः->m_payload.m_गणक++;
                pTLS->p_Current_ग्रन्थिः->m_payload.m_ticks_योग += ticks;
                pTLS->p_Current_ग्रन्थिः->m_payload.m_tls.get()->m_ticks_योग += ticks;
            }

            pTLS->p_Current_ग्रन्थिः = pTLS->p_Current_ग्रन्थिः->m_pParent;
        }
    }
}
#यदि_अन्तः
}

नामाकाश ipp
{

#यदि_परिभाषितम् HAVE_IPP
संरचना IPPInitSingleton
{
सार्वजनिक:
    IPPInitSingleton()
    {
        useIPP         = सत्य;

#यदि defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#अन्यथा
        useIPP_NE      = असत्य;
#यदि_अन्तः
        ipp_स्थितिः      = 0;
        funcname       = रिक्त;
        filename       = रिक्त;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_शिखरम्_Features = 0;
        pIppLibInfo    = रिक्त;

        ipp_स्थितिः = ippGetCpuFeatures(&cpuFeatures, रिक्त);
        यदि(ipp_स्थितिः < 0)
        {
            CV_LOG_ERROR(रिक्त, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = असत्य;
            प्रत्यागम;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        यदि(!env.empty())
        {
#यदि IPP_संस्करणम्_X100 >= 201900
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#नोचेद्यदि IPP_संस्करणम्_X100 >= 201703
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#नोचेद्यदि IPP_संस्करणम्_X100 >= 201700
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अन्यथा
            ध्रुव Ipp64u minorFeatures = 0;
#यदि_अन्तः

            env = गन्तव्यम्_अधस्तनम्_Case(env);
            यदि(env.substr(0, 2) == "ne")
            {
                useIPP_NE = सत्य;
                env = env.substr(3, env.size());
            }

            यदि(env == "disabled")
            {
                CV_LOG_WARNING(रिक्त, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = असत्य;
            }
            अन्यथा यदि(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            अन्यथा यदि(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#यदि IPP_संस्करणम्_X100 >= 201700
#यदि defined (_M_AMD64) || defined (__x86_64__)
            अन्यथा यदि(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#यदि_अन्तः
#यदि_अन्तः
            अन्यथा
                CV_LOG_ERROR(रिक्त, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        यदि(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        यदि(!(
#यदि IPP_संस्करणम्_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#यदि_अन्तः
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = असत्य;
            प्रत्यागम;
        }

        यदि(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        अन्यथा
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#यदि IPP_संस्करणम्_X100 >= 201700
        यदि(ippFeatures&ippCPUID_AVX512F)
        {
            यदि((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_शिखरम्_Features = ippCPUID_AVX512_SKX;
            अन्यथा यदि((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_शिखरम्_Features = ippCPUID_AVX512_KNL;
            अन्यथा
                ipp_शिखरम्_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        अन्यथा
#यदि_अन्तः
        यदि(ippFeatures&ippCPUID_AVX2)
            ipp_शिखरम्_Features = ippCPUID_AVX2;
        अन्यथा यदि(ippFeatures&ippCPUID_SSE42)
            ipp_शिखरम्_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_संस्करणम्();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        यदि (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_शिखरम्_Features = ippCPUID_SSE42;
        }
    }

सार्वजनिक:
    तार्किक        useIPP;
    तार्किक        useIPP_NE;

    पूर्णाङ्क         ipp_स्थितिः;  // 0 - all is ok, -1 - IPP functions failed
    ध्रुव वर्ण *funcname;
    ध्रुव वर्ण *filename;
    पूर्णाङ्क         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_शिखरम्_Features;
    ध्रुव Ipp_Library_संस्करणम् *pIppLibInfo;
};

स्थिर IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, नूतन IPPInitSingleton())
}
#यदि_अन्तः

अचिह्नित दीर्घ दीर्घ getIppFeatures()
{
#यदि_परिभाषितम् HAVE_IPP
    प्रत्यागम getIPPSingleton().ippFeatures;
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः
}

#यदि_परिभाषितम् HAVE_IPP
अचिह्नित दीर्घ दीर्घ get_Ipp_शिखरम्_Features()
{
    प्रत्यागम getIPPSingleton().ipp_शिखरम्_Features;
}
#यदि_अन्तः

शून्य समुच्चयः_Ipp_स्थितिः(पूर्णाङ्क स्थितिः, ध्रुव वर्ण * ध्रुव _funcname, ध्रुव वर्ण * ध्रुव _filename, पूर्णाङ्क पङ्क्तिः_3)
{
#यदि_परिभाषितम् HAVE_IPP
    getIPPSingleton().ipp_स्थितिः = स्थितिः;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = पङ्क्तिः_3;
#अन्यथा
    CV_अप्रयुक्तम्(स्थितिः); CV_अप्रयुक्तम्(_funcname); CV_अप्रयुक्तम्(_filename); CV_अप्रयुक्तम्(पङ्क्तिः_3);
#यदि_अन्तः
}

पूर्णाङ्क get_Ipp_स्थितिः()
{
#यदि_परिभाषितम् HAVE_IPP
    प्रत्यागम getIPPSingleton().ipp_स्थितिः;
#अन्यथा
    प्रत्यागम 0;
#यदि_अन्तः
}

String getIppErrorLocation()
{
#यदि_परिभाषितम् HAVE_IPP
    प्रत्यागम format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#अन्यथा
    प्रत्यागम String();
#यदि_अन्तः
}

String get_Ipp_संस्करणम्()
{
#यदि_परिभाषितम् HAVE_IPP
    ध्रुव Ipp_Library_संस्करणम् *pInfo = getIPPSingleton().pIppLibInfo;
    यदि(pInfo)
        प्रत्यागम format("%s %s %s", pInfo->Name, pInfo->संस्करणम्_4, pInfo->BuildDate);
    अन्यथा
        प्रत्यागम String("error");
#अन्यथा
    प्रत्यागम String("disabled");
#यदि_अन्तः
}

तार्किक useIPP()
{
#यदि_परिभाषितम् HAVE_IPP
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    यदि (दत्त.useIPP < 0)
    {
        दत्त.useIPP = getIPPSingleton().useIPP;
    }
    प्रत्यागम (दत्त.useIPP > 0);
#अन्यथा
    प्रत्यागम असत्य;
#यदि_अन्तः
}

शून्य समुच्चयः_Use_IPP(तार्किक flag)
{
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
#यदि_परिभाषितम् HAVE_IPP
    दत्त.useIPP = (getIPPSingleton().useIPP)?flag:असत्य;
#अन्यथा
    CV_अप्रयुक्तम्(flag);
    दत्त.useIPP = असत्य;
#यदि_अन्तः
}

तार्किक useIPP_NotExact()
{
#यदि_परिभाषितम् HAVE_IPP
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
    यदि (दत्त.useIPP_NE < 0)
    {
        दत्त.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    प्रत्यागम (दत्त.useIPP_NE > 0);
#अन्यथा
    प्रत्यागम असत्य;
#यदि_अन्तः
}

शून्य समुच्चयः_Use_IPP_Not_Exact(तार्किक flag)
{
    CoreTLSData& दत्त = get_Core_Tls_दत्त();
#यदि_परिभाषितम् HAVE_IPP
    दत्त.useIPP_NE = flag;
#अन्यथा
    CV_अप्रयुक्तम्(flag);
    दत्त.useIPP_NE = असत्य;
#यदि_अन्तः
}

} // namespace ipp


नामाकाश details {

#यदि OPENCV_IMPL_FP_HINTS_X86
#यदि_अपरिभाषितम् _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#परिभाषय _MM_DENORMALS_ZERO_ON 0x0040
#यदि_अन्तः
#यदि_अपरिभाषितम् MM_DENORMALS_ZERO_आवरणम्  // requires pmmintrin.h (SSE3)
#परिभाषय MM_DENORMALS_ZERO_आवरणम् 0x0040
#यदि_अन्तः
#यदि_अन्तः

शून्य समुच्चयः_FPDenormals_Ignore_Hint(तार्किक ignore, CV_बहिर्गमनम् FPDenormals_बहुलकम्_State& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    अचिह्नित आवरणम् = MM_FLUSH_ZERO_आवरणम्;
    अचिह्नित value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    यदि (featuresEnabled.have[CPU_SSE3])
    {
        आवरणम् |= MM_DENORMALS_ZERO_आवरणम्;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ध्रुव अचिह्नित old_flags = _mm_getcsr();
    ध्रुव अचिह्नित old_मानम् = old_flags & आवरणम्;
    अचिह्नित flags = (old_flags & ~आवरणम्) | value;
    CV_LOG_DEBUG(रिक्त, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)आवरणम्;
    state.reserved[1] = (uint32_t)old_मानम्;
    _mm_setcsr(flags);
#अन्यथा
    CV_अप्रयुक्तम्(ignore); CV_अप्रयुक्तम्(state);
#यदि_अन्तः
}

पूर्णाङ्क saveFPDenormalsState(CV_बहिर्गमनम् FPDenormals_बहुलकम्_State& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    अचिह्नित आवरणम् = MM_FLUSH_ZERO_आवरणम्;
    यदि (featuresEnabled.have[CPU_SSE3])
    {
        आवरणम् |= MM_DENORMALS_ZERO_आवरणम्;
    }
    ध्रुव अचिह्नित old_flags = _mm_getcsr();
    ध्रुव अचिह्नित old_मानम् = old_flags & आवरणम्;
    // save state
    state.reserved[0] = (uint32_t)आवरणम्;
    state.reserved[1] = (uint32_t)old_मानम्;
    प्रत्यागम 2;
#अन्यथा
    CV_अप्रयुक्तम्(state);
    प्रत्यागम 0;
#यदि_अन्तः
}

तार्किक restoreFPDenormalsState(ध्रुव FPDenormals_बहुलकम्_State& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    ध्रुव अचिह्नित आवरणम् = (अचिह्नित)state.reserved[0];
    CV_DbgAssert(आवरणम् != 0); // invalid state (ensure that state is properly saved earlier)
    ध्रुव अचिह्नित value = (अचिह्नित)state.reserved[1];
    CV_DbgCheck((पूर्णाङ्क)value, value == (value & आवरणम्), "invalid SSE FP state");
    ध्रुव अचिह्नित old_flags = _mm_getcsr();
    अचिह्नित flags = (old_flags & ~आवरणम्) | value;
    CV_LOG_DEBUG(रिक्त, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    प्रत्यागम सत्य;
#अन्यथा
    CV_अप्रयुक्तम्(state);
    प्रत्यागम असत्य;
#यदि_अन्तः
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#यदि_परिभाषितम् OPENCV_ALGO_HINT_DEFAULT
    प्रत्यागम OPENCV_ALGO_HINT_DEFAULT;
#अन्यथा
    प्रत्यागम ALGO_HINT_ACCURATE;
#यदि_अन्तः
};

} // namespace cv

/* End of file. */
