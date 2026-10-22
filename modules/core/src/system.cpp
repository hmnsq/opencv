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

#ifdef __QNX__
    #अन्तर्भाव <unistd.h>
    #अन्तर्भाव <sys/neutrino.h>
    #अन्तर्भाव <sys/syspage.h>
#ifdef __aarch64__
    #अन्तर्भाव <aarch64/syspage.h>
#endif
#endif

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
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    स्थिर std::ios_base::Init s_iostream_initializer;
#endif
}

स्थिर Mutex* __initialization_mutex = रिक्त;
Mutex& getInitializationMutex()
{
    जइ (__initialization_mutex == रिक्त)
    {
        (शून्य)_initSystem();
        __initialization_mutex = नूतन Mutex();
    }
    प्रत्यागम *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

स्थिर तार्किक param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#जइ defined(_DEBUG) || defined(__ANDROID__)
    सच्च
#अण्णहा
    मिच्छ
#endif
);

शून्य* allocSingletonBuffer(size_t size) { प्रत्यागम fastMalloc(size); }
शून्य* allocSingletonNewBuffer(size_t size) { प्रत्यागम malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# जइ defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#जइ defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# जइ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#अन्तर्भाव <cstdlib>        // std::abort
#endif

#जइ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#जइ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ध्रुव uint64_t AT_HWCAP = NT_GNU_HWCAP;
#अण्णहा
#  include <elf.h>
#endif
#जइ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#जइ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#जइ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#endif

#जइ defined __loongarch64
#अन्तर्भाव "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#जइ defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#अन्तर्भाव <windows.h>
#अन्तर्भाव <combaseapi.h>
#जइ (_WIN32_WINNT >= 0x0602)
  #अन्तर्भाव <synchapi.h>
#endif
#जइ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #अन्तर्भाव <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#अन्तर्भाव <tchar.h>

#ifdef WINRT
#अन्तर्भाव <wrl/client.h>
#ifndef __cplusplus_winrt
#अन्तर्भाव <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    प्रत्यागम std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#अण्णहा
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    जइ (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        प्रत्यागम wstr;
    जइ (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        प्रत्यागम wstr;
    जइ (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        प्रत्यागम wstr;
    जइ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        प्रत्यागम wstr;
    जइ (FAILED(storagefolderRef.As(&storageitemRef)))
        प्रत्यागम wstr;
    str = रिक्त;
    जइ (FAILED(storageitemRef->get_Path(&str)))
        प्रत्यागम wstr;
    wstr = WindowsGetStringRawBuffer(str, रिक्त);
    WindowsDeleteString(str);
    प्रत्यागम wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], परिमाण(guidStr)/परिमाण(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    प्रत्यागम prefix.append(std::wstring(guidStr));
}

#endif
#अण्णहा
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#अन्तर्भाव <pthread.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>

#जइ defined __MACH__ && defined __APPLE__
#अन्तर्भाव <mach/mach.h>
#अन्तर्भाव <mach/mach_time.h>
#अन्तर्भाव <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#अन्तर्भाव "omp.h"
#endif

#जइ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#जइ defined __ANDROID__
#अन्तर्भाव <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #जइ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #जइ _MSC_VER >= 1400  // MSVS 2005
      #अन्तर्भाव <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #अण्णहा
      #error "Required MSVS 2005+"
    #endif
  #नोचेद्यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    स्थिर शून्य cv_cpuid(पूर्णाङ्क* cpuid_data, पूर्णाङ्क reg_eax, पूर्णाङ्क reg_ecx)
    {
        पूर्णाङ्क __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#जइ !defined(__PIC__) \
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
#अण्णहा
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#अन्तर्भाव <chrono>

नामाकाश cv
{

Exception::Exception() { code = Error::StsOk; पङ्क्तिः = 0; }

Exception::Exception(Error::Code _code, ध्रुव String& _err, ध्रुव String& _func, ध्रुव String& _file, पूर्णाङ्क पङ्क्तिः_3)
: code(_code), err(_err), func(_func), file(_file), पङ्क्तिः(पङ्क्तिः_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ध्रुव वर्ण* Exception::what() ध्रुव CV_NOEXCEPT { प्रत्यागम सन्देशः.c_str(); }

शून्य Exception::formatMessage()
{
    size_t pos = err.find('\n');
    तार्किक multiline = pos != cv::String::npos;
    जइ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        जाव (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        जइ (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    जइ (func.size() > 0)
    {
        जइ (multiline)
            सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), पङ्क्तिः, code, codeMessage(), func.c_str(), err.c_str());
        अण्णहा
            सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), पङ्क्तिः, code, codeMessage(), err.c_str(), func.c_str());
    }
    अण्णहा
    {
        सन्देशः = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), पङ्क्तिः, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ध्रुव वर्ण * Exception::codeMessage() ध्रुव
{
    विकल्प (code)
    {
    पक्ष cv::Error::StsOk :                  प्रत्यागम "No Error";
    पक्ष cv::Error::StsBackTrace :           प्रत्यागम "Backtrace";
    पक्ष cv::Error::StsError :               प्रत्यागम "Unspecified error";
    पक्ष cv::Error::StsInternal :            प्रत्यागम "Internal error";
    पक्ष cv::Error::StsNoMem :               प्रत्यागम "Insufficient memory";
    पक्ष cv::Error::StsBadArg :              प्रत्यागम "Bad argument";
    पक्ष cv::Error::StsNoConv :              प्रत्यागम "Iterations do not converge";
    पक्ष cv::Error::StsAutoTrace :           प्रत्यागम "Autotrace call";
    पक्ष cv::Error::StsBadSize :             प्रत्यागम "Incorrect size of input array";
    पक्ष cv::Error::StsNullPtr :             प्रत्यागम "Null pointer";
    पक्ष cv::Error::StsDivByZero :           प्रत्यागम "Division by zero occurred";
    पक्ष cv::Error::BadStep :                प्रत्यागम "Image step is wrong";
    पक्ष cv::Error::StsInplaceNotSupported : प्रत्यागम "Inplace operation is not supported";
    पक्ष cv::Error::StsObjectNotFound :      प्रत्यागम "Requested object was not found";
    पक्ष cv::Error::BadDepth :               प्रत्यागम "Input image depth is not supported by function";
    पक्ष cv::Error::StsUnmatchedFormats :    प्रत्यागम "Formats of input arguments do not match";
    पक्ष cv::Error::StsUnmatchedSizes :      प्रत्यागम "Sizes of input arguments do not match";
    पक्ष cv::Error::StsOutOfRange :          प्रत्यागम "One of the arguments\' values is out of range";
    पक्ष cv::Error::StsUnsupportedFormat :   प्रत्यागम "Unsupported format or combination of formats";
    पक्ष cv::Error::BadCOI :                 प्रत्यागम "Input COI is not supported";
    पक्ष cv::Error::Bad_अङ्कः_Channels :         प्रत्यागम "Bad number of channels";
    पक्ष cv::Error::StsBadFlag :             प्रत्यागम "Bad flag (parameter or structure field)";
    पक्ष cv::Error::StsBadPoint :            प्रत्यागम "Bad parameter of type CvPoint";
    पक्ष cv::Error::StsBadMask :             प्रत्यागम "Bad type of mask argument";
    पक्ष cv::Error::StsParseError :          प्रत्यागम "Parsing error";
    पक्ष cv::Error::StsNotImplemented :      प्रत्यागम "The function/feature is not implemented";
    पक्ष cv::Error::StsBadMemBlock :         प्रत्यागम "Memory block has been corrupted";
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
        snprintf(buf, परिमाण(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        प्रत्यागम buf;
    }
    };

}

स्थिर ध्रुव वर्ण* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { रिक्त };

स्थिर ध्रुव वर्ण* getHWFeatureName(पूर्णाङ्क id)
{
    प्रत्यागम (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : रिक्त;
}
स्थिर ध्रुव वर्ण* getHWFeatureNameSafe(पूर्णाङ्क id)
{
    ध्रुव वर्ण* name = getHWFeatureName(id);
    प्रत्यागम name ? name : "Unknown feature";
}

संरचना HWFeatures
{
    गणना { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(तार्किक run_initialize = मिच्छ)
    {
        जइ (run_initialize)
            initialize();
    }

    स्थिर शून्य initializeNames()
    {
        प्रति (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hwFeatureNames[i] = 0;
        }
        g_hwFeatureNames[CPU_MMX] = "MMX";
        g_hwFeatureNames[CPU_SSE] = "SSE";
        g_hwFeatureNames[CPU_SSE2] = "SSE2";
        g_hwFeatureNames[CPU_SSE3] = "SSE3";
        g_hwFeatureNames[CPU_SSSE3] = "SSSE3";
        g_hwFeatureNames[CPU_SSE4_1] = "SSE4.1";
        g_hwFeatureNames[CPU_SSE4_2] = "SSE4.2";
        g_hwFeatureNames[CPU_POPCNT] = "POPCNT";
        g_hwFeatureNames[CPU_FP16] = "FP16";
        g_hwFeatureNames[CPU_AVX] = "AVX";
        g_hwFeatureNames[CPU_AVX2] = "AVX2";
        g_hwFeatureNames[CPU_FMA3] = "FMA3";

        g_hwFeatureNames[CPU_AVX_512F] = "AVX512F";
        g_hwFeatureNames[CPU_AVX_512BW] = "AVX512BW";
        g_hwFeatureNames[CPU_AVX_512CD] = "AVX512CD";
        g_hwFeatureNames[CPU_AVX_512DQ] = "AVX512DQ";
        g_hwFeatureNames[CPU_AVX_512ER] = "AVX512ER";
        g_hwFeatureNames[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hwFeatureNames[CPU_AVX_512PF] = "AVX512PF";
        g_hwFeatureNames[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hwFeatureNames[CPU_AVX_512VL] = "AVX512VL";
        g_hwFeatureNames[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hwFeatureNames[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hwFeatureNames[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hwFeatureNames[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hwFeatureNames[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hwFeatureNames[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hwFeatureNames[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hwFeatureNames[CPU_NEON] = "NEON";
        g_hwFeatureNames[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hwFeatureNames[CPU_NEON_FP16] = "NEON_FP16";
        g_hwFeatureNames[CPU_NEON_BF16] = "NEON_BF16";
        g_hwFeatureNames[CPU_SVE] = "SVE";

        g_hwFeatureNames[CPU_VSX] = "VSX";
        g_hwFeatureNames[CPU_VSX3] = "VSX3";

        g_hwFeatureNames[CPU_MSA] = "CPU_MSA";
        g_hwFeatureNames[CPU_RISCVV] = "RISCVV";

        g_hwFeatureNames[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hwFeatureNames[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hwFeatureNames[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hwFeatureNames[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hwFeatureNames[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hwFeatureNames[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hwFeatureNames[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hwFeatureNames[CPU_RVV] = "RVV";
        g_hwFeatureNames[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hwFeatureNames[CPU_LSX]  = "LSX";
        g_hwFeatureNames[CPU_LASX] = "LASX";
    }

    शून्य initialize(शून्य)
    {
        जइ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        पूर्णाङ्क cpuid_data[4] = { 0, 0, 0, 0 };
        पूर्णाङ्क cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        पूर्णाङ्क x86_family = (cpuid_data[0] >> 8) & 15;
        जइ( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_data[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_data[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_data[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_data[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_data[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_data[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_data[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_data[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_data[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_data[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_data[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_data_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_data_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_data_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_data_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_data_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_data_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_data_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_data_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_data_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_data_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_data_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_data_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_data_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_data_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_data_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_data_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_data_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            पूर्णाङ्क cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            तार्किक have_AVX_OS_support = सच्च;
            तार्किक have_AVX512_OS_support = सच्च;
            जइ (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = मिच्छ; // OS uses XSAVE_XRSTORE and CPU support AVX
            अण्णहा
            {
                पूर्णाङ्क xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (पूर्णाङ्क)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #नोचेद्यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                जइ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = मिच्छ; // YMM registers
                जइ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = मिच्छ; // ZMM registers
            }

            जइ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = मिच्छ;
                have[CV_CPU_FP16] = मिच्छ;
                have[CV_CPU_AVX2] = मिच्छ;
                have[CV_CPU_FMA3] = मिच्छ;
                have[CV_CPU_AVX_VNNI] = मिच्छ;
            }
            जइ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = मिच्छ;
                have[CV_CPU_AVX_512BW] = मिच्छ;
                have[CV_CPU_AVX_512CD] = मिच्छ;
                have[CV_CPU_AVX_512DQ] = मिच्छ;
                have[CV_CPU_AVX_512ER] = मिच्छ;
                have[CV_CPU_AVX_512IFMA] = मिच्छ;
                have[CV_CPU_AVX_512PF] = मिच्छ;
                have[CV_CPU_AVX_512VBMI] = मिच्छ;
                have[CV_CPU_AVX_512VL] = मिच्छ;
                have[CV_CPU_AVX_512VBMI2] = मिच्छ;
                have[CV_CPU_AVX_512VNNI] = मिच्छ;
                have[CV_CPU_AVX_512BITALG] = मिच्छ;
                have[CV_CPU_AVX_512VPOPCNTDQ] = मिच्छ;
                have[CV_CPU_AVX_5124VNNIW] = मिच्छ;
                have[CV_CPU_AVX_5124FMAPS] = मिच्छ;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            जइ (have[CV_CPU_AVX512_COMMON])
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
            अण्णहा
            {
                have[CV_CPU_AVX512_KNL] = मिच्छ;
                have[CV_CPU_AVX512_KNM] = मिच्छ;
                have[CV_CPU_AVX512_SKX] = मिच्छ;
                have[CV_CPU_AVX512_CNL] = मिच्छ;
                have[CV_CPU_AVX512_CLX] = मिच्छ;
                have[CV_CPU_AVX512_ICL] = मिच्छ;
            }
        }
    #endif // CV_CPUID_X86

    #जइ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = सच्च;
        have[CV_CPU_FP16] = सच्च;
        पूर्णाङ्क cpufile = open("/proc/self/auxv", O_RDONLY);

        जइ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ध्रुव size_t size_auxv_t = परिमाण(auxv);

            जाव ((size_t)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                जइ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#जइ defined(AT_HWCAP2)
                अण्णहा जइ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #नोचेद्यदि defined __arm__ && defined __ANDROID__
      #जइ defined HAVE_CPUFEATURES
        CV_LOG_INFO(रिक्त, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(रिक्त, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #अण्णहा
        CV_LOG_INFO(रिक्त, "cpufeatures library is not available for CPU detection");
        #जइ CV_NEON
        CV_LOG_INFO(रिक्त, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = सच्च;
        #अण्णहा
        CV_LOG_INFO(रिक्त, "- NEON instructions is NOT enabled via build flags");
        #endif
        #जइ CV_FP16
        CV_LOG_INFO(रिक्त, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = सच्च;
        #अण्णहा
        CV_LOG_INFO(रिक्त, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #नोचेद्यदि defined __arm__
        पूर्णाङ्क cpufile = open("/proc/self/auxv", O_RDONLY);

        जइ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ध्रुव size_t size_auxv_t = परिमाण(auxv);

            जाव ((size_t)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                जइ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    भङ्ग;
                }
            }

            close(cpufile);
        }
    #endif
    #नोचेद्यदि (defined __APPLE__)
    #जइ defined __ARM_NEON
        have[CV_CPU_NEON] = सच्च;
    #endif
    #जइ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = सच्च;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    पूर्णाङ्क has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = परिमाण(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, रिक्त, 0);
    जइ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = सच्च;
    }
    पूर्णाङ्क has_feat_bf16 = 0;
    size_t has_feat_bf16_size = परिमाण(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, रिक्त, 0);
    जइ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = सच्च;
    }
    #नोचेद्यदि (defined __clang__)
    #जइ defined __ARM_NEON
        have[CV_CPU_NEON] = सच्च;
        #जइ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = सच्च;
        #endif
    #endif
    #endif
    #जइ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = सच्च;
    #endif
    #जइ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = सच्च;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = सच्च;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = सच्च;
    #endif

    #जइ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned पूर्णाङ्क hwcap = getauxval(AT_HWCAP);
        जइ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            जइ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सच्च;
            } अण्णहा {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #नोचेद्यदि (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        जइ (elf_aux_info(AT_HWCAP, &hwcap, परिमाण(hwcap)) != 0)
            hwcap = 0;
        जइ (hwcap & PPC_FEATURE_HAS_VSX) {
            जइ (elf_aux_info(AT_HWCAP2, &hwcap2, परिमाण(hwcap2)) != 0)
                hwcap2 = 0;
            जइ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सच्च;
            } अण्णहा {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अण्णहा
        // TODO: AIX
        #जइ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = सच्च;
        #endif
        #जइ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = सच्च;
        #endif
    #endif

    #जइ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = सच्च;
        #जइ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = सच्च;
        #endif
        #जइ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = सच्च;
        #endif
    #endif

    #जइ defined __loongarch64 && defined __linux__
        पूर्णाङ्क flag = (पूर्णाङ्क)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        तार्किक skip_baseline_check = मिच्छ;
        जइ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = सच्च;
        }
        पूर्णाङ्क baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        जइ (!checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]))
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
            checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]), सच्च);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]));
    }

    तार्किक checkFeatures(ध्रुव पूर्णाङ्क* features, पूर्णाङ्क गणनम्_2, तार्किक dump = मिच्छ)
    {
        तार्किक result = सच्च;
        प्रति (पूर्णाङ्क i = 0; i < गणनम्_2; i++)
        {
            पूर्णाङ्क feature = features[i];
            जइ (feature)
            {
                जइ (have[feature])
                {
                    जइ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                अण्णहा
                {
                    result = मिच्छ;
                    जइ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        प्रत्यागम result;
    }

    स्थिर अन्तःस्थ तार्किक is_चिह्नम्_Separator(वर्ण c)
    {
        प्रत्यागम c == ',' || c == ';';
    }

    शून्य readSettings(ध्रुव पूर्णाङ्क* baseline_features, पूर्णाङ्क baseline_गणनम्)
    {
        तार्किक dump = सच्च;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        जइ (!disabled_features.empty())
        {
            ध्रुव वर्ण* start = disabled_features.c_str();
            प्रति (;;)
            {
                जाव (start[0] != 0 && is_चिह्नम्_Separator(start[0]))
                {
                    start++;
                }
                जइ (start[0] == 0)
                    भङ्ग;
                ध्रुव वर्ण* end = start;
                जाव (end[0] != 0 && !is_चिह्नम्_Separator(end[0]))
                {
                    end++;
                }
                जइ (end == start)
                    अनुवर्त;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                तार्किक found = मिच्छ;
                प्रति (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    जइ (!g_hwFeatureNames[i]) अनुवर्त;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    जइ (len != feature.size()) अनुवर्त;
                    जइ (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        तार्किक isBaseline = मिच्छ;
                        प्रति (पूर्णाङ्क k = 0; k < baseline_गणनम्; k++)
                        {
                            जइ (baseline_features[k] == i)
                            {
                                isBaseline = सच्च;
                                भङ्ग;
                            }
                        }
                        जइ (isBaseline)
                        {
                            जइ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        जइ (!have[i])
                        {
                            जइ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = मिच्छ;

                        found = सच्च;
                        भङ्ग;
                    }
                }
                जइ (!found)
                {
                    जइ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    तार्किक have[MAX_FEATURE+1]{};
};

स्थिर HWFeatures  featuresEnabled(सच्च), featuresDisabled = HWFeatures(मिच्छ);
स्थिर HWFeatures* currentFeatures = &featuresEnabled;

तार्किक checkHardwareSupport(पूर्णाङ्क feature)
{
    जइ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        प्रत्यागम मिच्छ;
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
    std::string result;
    std::string prefix;
    प्रति (पूर्णाङ्क i = 1; i < sz; ++i)
    {
        जइ (features[i] == 0)
        {
            prefix = "*";
            अनुवर्त;
        }
        जइ (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        जइ (!checkHardwareSupport(features[i])) result.append("?");
    }
    प्रत्यागम result;
}

चञ्चल तार्किक useOptimizedFlag = सच्च;

शून्य setUseOptimized( तार्किक flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
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
    using clock_period_t = std::chrono::steady_clock::duration::period;
    द्विगुण clock_freq = clock_period_t::den / clock_period_t::num;
    प्रत्यागम clock_freq;
}

#जइ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#जइ defined(__i386__)

int64 get_CPUTick_गणनम्(शून्य)
{
    int64 x;
    __asm__ चञ्चल (".byte 0x0f, 0x31" : "=A" (x));
    प्रत्यागम x;
}
#नोचेद्यदि defined(__x86_64__)

int64 get_CPUTick_गणनम्(शून्य)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    प्रत्यागम (int64)lo | ((int64)hi << 32);
}

#नोचेद्यदि defined(__ppc__)

int64 get_CPUTick_गणनम्(शून्य)
{
    unsigned upper, lower, tmp;
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

#अण्णहा

#error "RDTSC not defined"

#endif

#नोचेद्यदि defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_गणनम्(शून्य)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#अण्णहा

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

#endif


नामाकाश internal {

वर्ग Timestamp
{
सार्वजनिक:
    ध्रुव int64 zero_Tick_गणनम्;
    ध्रुव द्विगुण ns_in_ticks;

    Timestamp()
        : zero_Tick_गणनम्(get_Tick_गणनम्())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_गणनम्();
        प्रत्यागम (int64)((t - zero_Tick_गणनम्) * ns_in_ticks);
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

String getVersionString() { प्रत्यागम String(CV_VERSION); }

पूर्णाङ्क getVersionMajor() { प्रत्यागम CV_VERSION_MAJOR; }

पूर्णाङ्क getVersionMinor() { प्रत्यागम CV_VERSION_MINOR; }

पूर्णाङ्क getVersionRevision() { प्रत्यागम CV_VERSION_REVISION; }

String format( ध्रुव वर्ण* fmt, ... )
{
    AutoBuffer<वर्ण, 1024> buf;

    प्रति ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        पूर्णाङ्क bsize = static_cast<पूर्णाङ्क>(buf.size());
        पूर्णाङ्क len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        जइ (len >= bsize)
        {
            buf.resize(len + 1);
            अनुवर्त;
        }
        buf[bsize - 1] = 0;
        प्रत्यागम String(buf.data(), len);
    }
}

String tempfile( ध्रुव वर्ण* suffix )
{
#जइ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#जइ defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    जइ (temp_file.empty())
        प्रत्यागम String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    वर्ण aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#नोचेद्यदि defined(_WIN32_WCE)
    ध्रुव auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    जइ(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        वर्ण aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#अण्णहा
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    जइ (temp_dir.empty())
    {
        ::GetTempPathW(परिमाण(temp_dir2_w)/परिमाण(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        पूर्णाङ्क len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, रिक्त, 0, रिक्त, रिक्त);
        जइ (len > 0)
        {
            std::vector<वर्ण> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, रिक्त, रिक्त);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    जइ (FAILED(hr))
        प्रत्यागम String();
    वर्ण guidStr[40];
    ध्रुव वर्ण* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, परिमाण(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned पूर्णाङ्क)g.Data4[0], (unsigned पूर्णाङ्क)g.Data4[1],
            (unsigned पूर्णाङ्क)g.Data4[2], (unsigned पूर्णाङ्क)g.Data4[3], (unsigned पूर्णाङ्क)g.Data4[4],
            (unsigned पूर्णाङ्क)g.Data4[5], (unsigned पूर्णाङ्क)g.Data4[6], (unsigned पूर्णाङ्क)g.Data4[7]);

    fname = temp_dir;
    जइ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# अण्णहा
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    वर्ण defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  अण्णहा
    वर्ण defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    जइ (temp_dir.empty())
        fname = defaultTemplate;
    अण्णहा
    {
        fname = temp_dir;
        वर्ण ech = fname[fname.size() - 1];
        जइ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ध्रुव पूर्णाङ्क fd = mkstemp((वर्ण*)fname.c_str());
    जइ (fd == -1) प्रत्यागम String();

    close(fd);
    remove(fname.c_str());
# endif

    जइ (suffix)
    {
        जइ (suffix[0] != '.')
            प्रत्यागम fname + "." + suffix;
        अण्णहा
            प्रत्यागम fname + suffix;
    }
    प्रत्यागम fname;
#अण्णहा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

स्थिर ErrorCallback customErrorCallback = 0;
स्थिर शून्य* customErrorCallbackData = 0;
स्थिर तार्किक breakOnError = मिच्छ;

तार्किक setBreakOnError(तार्किक value)
{
    तार्किक prevVal = breakOnError;
    breakOnError = value;
    प्रत्यागम prevVal;
}

पूर्णाङ्क cv_snprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    पूर्णाङ्क res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    प्रत्यागम res;
}

पूर्णाङ्क cv_vsnprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, va_list args)
{
#जइ defined _MSC_VER
    जइ (len <= 0) प्रत्यागम len == 0 ? 1024 : -1;
    पूर्णाङ्क res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    जइ (res >= 0 && res < len)
    {
        buf[res] = 0;
        प्रत्यागम res;
    }
    अण्णहा
    {
        buf[len - 1] = 0; // truncate happened
        प्रत्यागम res >= len ? res : (len * 2);
    }
#अण्णहा
    प्रत्यागम vsnprintf(buf, len, fmt, args);
#endif
}

स्थिर शून्य dumpException(ध्रुव Exception& exc)
{
    ध्रुव वर्ण* errorStr = exc.codeMessage();
    वर्ण buf[1 << 12];

    cv_snprintf(buf, परिमाण(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.पङ्क्तिः);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#अण्णहा
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
स्थिर तार्किक cv_terminate_handler_installed = मिच्छ;
स्थिर std::terminate_handler cv_old_terminate_handler;
स्थिर cv::Exception cv_terminate_handler_exception;
स्थिर तार्किक param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", सच्च);
स्थिर शून्य cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    जइ (मिच्छ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# जइ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

शून्य error( ध्रुव Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        जइ (!cv_terminate_handler_installed)
        {
            जइ (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = सच्च;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    जइ (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.पङ्क्तिः, customErrorCallbackData);
    अण्णहा जइ (param_dumpErrors)
    {
        dumpException(exc);
    }

    जइ(breakOnError)
    {
        std::terminate();
    }

    क्षिप exc;
#ifdef __GNUC__
# जइ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

शून्य error(Error::Code _code, ध्रुव String& _err, ध्रुव वर्ण* _func, ध्रुव वर्ण* _file, पूर्णाङ्क पङ्क्तिः_3)
{
    error(cv::Exception(_code, _err, _func, _file, पङ्क्तिः_3));
#ifdef __GNUC__
# जइ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# जइ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, शून्य* userdata, शून्य** prevUserdata)
{
    जइ( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    प्रत्यागम prevCallback;
}

शून्य terminate(Error::Code _code, ध्रुव String& _err, ध्रुव वर्ण* _func, ध्रुव वर्ण* _file, पूर्णाङ्क पङ्क्तिः_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, पङ्क्तिः_3));
    std::terminate();
}

}

नामाकाश cv {
तार्किक __termination = मिच्छ;


//////////////////////////////// thread-local storage ////////////////////////////////

नामाकाश details {

#ifndef OPENCV_DISABLE_THREAD_SUPPORT

#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning(disable:4505) // unreferenced local function has been removed
#endif
#ifndef TLS_OUT_OF_INDEXES
#define TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
#endif
#endif

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

    शून्य* getData() ध्रुव;
    शून्य setData(शून्य *pData);

    शून्य releaseSystemResources();

गुप्त:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_कुञ्चिका;
    तार्किक disposed;
#endif
#अण्णहा // _WIN32
    pthread_key_t  tls_कुञ्चिका;
    std::atomic<तार्किक> disposed;
#endif
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


#ifdef _WIN32
#ifdef WINRT
स्थिर __declspec( thread ) शून्य* tlsData = रिक्त; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सच्च;  // DllMain is missing in static builds
}
शून्य* TlsAbstraction::getData() ध्रुव
{
    प्रत्यागम tlsData;
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    tlsData = pData;
}
#अण्णहा //WINRT
#ifdef CV_USE_FLS
स्थिर शून्य NTAPI opencv_fls_destructor(शून्य* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(मिच्छ)
{
#ifndef CV_USE_FLS
    tls_कुञ्चिका = TlsAlloc();
#अण्णहा // CV_USE_FLS
    tls_कुञ्चिका = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_कुञ्चिका != TLS_OUT_OF_INDEXES);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सच्च;  // DllMain is missing in static builds
    disposed = सच्च;
#ifndef CV_USE_FLS
    TlsFree(tls_कुञ्चिका);
#अण्णहा // CV_USE_FLS
    FlsFree(tls_कुञ्चिका);
#endif // CV_USE_FLS
    tls_कुञ्चिका = TLS_OUT_OF_INDEXES;
}
शून्य* TlsAbstraction::getData() ध्रुव
{
    जइ (disposed)
        प्रत्यागम रिक्त;
#ifndef CV_USE_FLS
    प्रत्यागम Tls_Get_मानम्(tls_कुञ्चिका);
#अण्णहा // CV_USE_FLS
    प्रत्यागम Fls_Get_मानम्(tls_कुञ्चिका);
#endif // CV_USE_FLS
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    जइ (disposed)
        प्रत्यागम;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_मानम्(tls_कुञ्चिका, pData) == TRUE);
#अण्णहा // CV_USE_FLS
    CV_Assert(Fls_Set_मानम्(tls_कुञ्चिका, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#अण्णहा // _WIN32
स्थिर शून्य opencv_tls_destructor(शून्य* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(मिच्छ)
{
    CV_Assert(pthread_key_create(&tls_कुञ्चिका, opencv_tls_destructor) == 0);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सच्च;  // DllMain is missing in static builds
    disposed = सच्च;
    जइ (pthread_key_delete(tls_कुञ्चिका) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
शून्य* TlsAbstraction::getData() ध्रुव
{
    जइ (disposed)
        प्रत्यागम रिक्त;
    प्रत्यागम pthread_getspecific(tls_कुञ्चिका);
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    जइ (disposed)
        प्रत्यागम;  // no-op
    CV_Assert(pthread_setspecific(tls_कुञ्चिका, pData) == 0);
}
#endif

// Per-thread data structure
संरचना ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<शून्य*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


स्थिर तार्किक g_isTlsStorageInitialized = मिच्छ;

// Main TLS storage class
वर्ग TlsStorage
{
सार्वजनिक:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (शून्य)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = सच्च;
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
        जइ (रिक्त == tls)
            प्रत्यागम;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_मानम् == रिक्त ? (ThreadData*)tls->getData() : (ThreadData*)tls_मानम्;
        जइ (pTD == रिक्त)
            प्रत्यागम;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        प्रति (size_t i = 0; i < threads.size(); i++)
        {
            जइ (pTD == threads[i])
            {
                threads[i] = रिक्त;
                जइ (tls_मानम् == रिक्त)
                    tls->setData(0);
                std::vector<शून्य*>& thread_slots = pTD->slots;
                प्रति (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    शून्य* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = रिक्त;
                    जइ (!pData)
                        अनुवर्त;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    जइ (container)
                        container->deleteDataInstance(pData);
                    अण्णहा
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (पूर्णाङ्क)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                प्रत्यागम;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (शून्य*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        प्रति(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            जइ (tlsSlots[slot].container == रिक्त)
            {
                tlsSlots[slot].container = container;
                प्रत्यागम slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        प्रत्यागम tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(size_t slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = मिच्छ)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        प्रति(size_t i = 0; i < threads.size(); i++)
        {
            जइ(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                जइ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = रिक्त;
                }
            }
        }

        जइ (!keepSlot)
        {
            tlsSlots[slotIdx].container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(size_t slotIdx) ध्रुव
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        जइ (रिक्त == tls)
            प्रत्यागम रिक्त;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        जइ(threadData && threadData->slots.size() > slotIdx)
            प्रत्यागम threadData->slots[slotIdx];

        प्रत्यागम रिक्त;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(size_t slotIdx, std::vector<शून्य*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        प्रति(size_t i = 0; i < threads.size(); i++)
        {
            जइ(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                जइ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    शून्य setData(size_t slotIdx, शून्य* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        जइ (रिक्त == tls)
            प्रत्यागम;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        जइ(!threadData)
        {
            threadData = नूतन ThreadData;
            tls->setData((शून्य*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                तार्किक found = मिच्छ;
                // Find unused slots
                प्रति(size_t slot = 0; slot < threads.size(); slot++)
                {
                    जइ (threads[slot] == रिक्त)
                    {
                        threadData->idx = (पूर्णाङ्क)slot;
                        threads[slot] = threadData;
                        found = सच्च;
                        भङ्ग;
                    }
                }

                जइ (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        जइ(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, रिक्त);
        }
        threadData->slots[slotIdx] = pData;
    }

गुप्त:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    संरचना TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<संरचना TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
स्थिर TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, नूतन TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
स्थिर शून्य opencv_tls_destructor(शून्य* pData)
{
    जइ (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#अण्णहा // _WIN32
#ifdef CV_USE_FLS
स्थिर शून्य WINAPI opencv_fls_destructor(शून्य* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    जइ (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = सच्च;
    }

    जइ (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

स्थिर TlsStorage* ध्रुव g_force_initialization_of_TlsStorage
#जइ defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#अण्णहा  // OPENCV_DISABLE_THREAD_SUPPORT

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
            जइ (container && s.data)
            {
                container->deleteDataInstance(s.data);  // Can't use from SlotInfo destructor
                s.data = nullptr;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        size_t slotsSize = slots.size();
        प्रति (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            जइ (s.container == रिक्त)
            {
                CV_Assert(!s.data);
                s.container = container;
                प्रत्यागम slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        प्रत्यागम slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(size_t slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = मिच्छ)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        जइ (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        जइ (!keepSlot)
        {
            s.container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(size_t slotIdx) ध्रुव
    {
        CV_Assert(slotIdx < slots.size());
        ध्रुव SlotInfo& s = slots[slotIdx];
        प्रत्यागम s.data;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(size_t slotIdx, std::vector<शून्य*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        जइ (data)
            dataVec.push_back(data);
        प्रत्यागम;
    }

    // Set data to storage index
    शून्य setData(size_t slotIdx, शून्य* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

गुप्त:
    संरचना SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        शून्य* data;
    };
    std::vector<संरचना SlotInfo> slots;
};

स्थिर TlsStorage& getTlsStorage()
{
    स्थिर TlsStorage g_storage;  // no threading
    प्रत्यागम g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using नामाकाश details;

शून्य releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    जइ (!g_isTlsStorageInitialized)
        प्रत्यागम;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    कुञ्चिका_2 = (पूर्णाङ्क)getTlsStorage().reserveSlot(एतत्); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(कुञ्चिका_2 == -1); // Key must be released in child object
}

शून्य TLSDataContainer::gatherData(std::vector<शून्य*> &data) ध्रुव
{
    getTlsStorage().gather(कुञ्चिका_2, data);
}

शून्य TLSDataContainer::detachData(std::vector<शून्य*> &data)
{
    getTlsStorage().releaseSlot(कुञ्चिका_2, data, सच्च);
}

शून्य TLSDataContainer::release()
{
    जइ (कुञ्चिका_2 == -1)
        प्रत्यागम;  // already released
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(कुञ्चिका_2, data, मिच्छ); // Release key and get stored data for proper destruction
    कुञ्चिका_2 = -1;
    प्रति(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य TLSDataContainer::cleanup()
{
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(कुञ्चिका_2, data, सच्च); // Extract stored data with removal from TLS tables
    प्रति(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य* TLSDataContainer::getData() ध्रुव
{
    CV_Assert(कुञ्चिका_2 != -1 && "Can't fetch data from terminated TLS container.");
    शून्य* pData = getTlsStorage().getData(कुञ्चिका_2); // Check if data was already allocated
    जइ(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        यत्न
        {
            getTlsStorage().setData(कुञ्चिका_2, pData);
        }
        गृहाण (...)
        {
            deleteDataInstance(pData);
            क्षिप;
        }
    }
    प्रत्यागम pData;
}

स्थिर TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, नूतन TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    प्रत्यागम getCoreTlsDataTLS().getRef();
}

#जइ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    जइ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        जइ (lpReserved != रिक्त) // called after ExitProcess() call
        {
            cv::__termination = सच्च;
        }
        अण्णहा
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    प्रत्यागम TRUE;
}
#endif


नामाकाश {

#ifdef OPENCV_WITH_ITT
तार्किक overrideThreadName()
{
    स्थिर तार्किक param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", मिच्छ);
    प्रत्यागम param;
}
#endif

स्थिर पूर्णाङ्क g_thread_अङ्कः = 0;
वर्ग ThreadID {
सार्वजनिक:
    ध्रुव पूर्णाङ्क id;
    ThreadID() :
        id(CV_XADD(&g_thread_अङ्कः, 1))
    {
#ifdef OPENCV_WITH_ITT
        जइ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
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
    std::string toString(ध्रुव std::string &param) ध्रुव
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_मानम्;
        प्रत्यागम out.str();
    }
};

template <typename T>
T parseOption(ध्रुव std::string &);

template<>
अन्तःस्थ तार्किक parseOption(ध्रुव std::string & value)
{
    जइ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        प्रत्यागम सच्च;
    }
    जइ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        प्रत्यागम मिच्छ;
    }
    क्षिप ParseError(value);
}

template<>
अन्तःस्थ size_t parseOption(ध्रुव std::string &value)
{
    size_t pos = 0;
    प्रति (; pos < value.size(); pos++)
    {
        जइ (!isdigit(value[pos]))
            भङ्ग;
    }
    cv::String मानम्_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(मानम्_Str);
    जइ (suffixStr.length() == 0)
        प्रत्यागम v;
    अण्णहा जइ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        प्रत्यागम v * 1024 * 1024;
    अण्णहा जइ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        प्रत्यागम v * 1024;
    क्षिप ParseError(value);
}

template<>
अन्तःस्थ cv::String parseOption(ध्रुव std::string &value)
{
    प्रत्यागम value;
}

template<>
अन्तःस्थ utils::Paths parseOption(ध्रुव std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ध्रुव वर्ण sep = ';';
#अण्णहा
    ध्रुव वर्ण sep = ':';
#endif
    size_t start_pos = 0;
    जाव (start_pos != std::string::npos)
    {
        ध्रुव size_t pos = value.find(sep, start_pos);
        ध्रुव std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        जइ (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    प्रत्यागम result;
}

स्थिर अन्तःस्थ ध्रुव वर्ण * envRead(ध्रुव वर्ण * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    प्रत्यागम रिक्त;
#अण्णहा
    प्रत्यागम getenv(name);
#endif
}

template<typename T>
अन्तःस्थ T पठ(ध्रुव std::string & k, ध्रुव T & default_मानम्_2)
{
    यत्न
    {
        ध्रुव वर्ण * res = envRead(k.c_str());
        जइ (res)
            प्रत्यागम parseOption<T>(std::string(res));
    }
    गृहाण (ध्रुव ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    प्रत्यागम default_मानम्_2;
}

तार्किक utils::getConfigurationParameterBool(ध्रुव वर्ण* name, तार्किक default_मानम्_2)
{
    प्रत्यागम पठ<तार्किक>(name, default_मानम्_2);
}

size_t utils::getConfigurationParameterSizeT(ध्रुव वर्ण* name, size_t default_मानम्_2)
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


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, नूतन ImplCollector())
}

शून्य setImpl(पूर्णाङ्क flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

शून्य addImpl(पूर्णाङ्क flag, ध्रुव वर्ण* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    जइ(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        जइ(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

पूर्णाङ्क getImpl(std::vector<पूर्णाङ्क> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    प्रत्यागम getImplData().implFlags; // return actual flags for lazy collection
}

तार्किक useCollection()
{
    प्रत्यागम getImplData().useCollection;
}

शून्य setUseCollection(तार्किक flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

नामाकाश instr
{
तार्किक useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    प्रत्यागम getInstrumentStruct().useInstr;
#अण्णहा
    प्रत्यागम मिच्छ;
#endif
}

शून्य setUseInstrumentation(तार्किक flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#अण्णहा
    CV_UNUSED(flag);
#endif
}

Instr_ग्रन्थिः* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    प्रत्यागम &getInstrumentStruct().मूलम्_ग्रन्थिः_2;
#अण्णहा
    प्रत्यागम रिक्त;
#endif
}

शून्य resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().मूलम्_ग्रन्थिः_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ग्रन्थिः = &getInstrumentStruct().मूलम्_ग्रन्थिः_2;
#endif
}

शून्य setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#अण्णहा
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    प्रत्यागम (FLAGS)getInstrumentStruct().flags;
#अण्णहा
    प्रत्यागम (FLAGS)0;
#endif
}

ग्रन्थिः_Data::ग्रन्थिः_Data(ध्रुव वर्ण* funName, ध्रुव वर्ण* fileName, पूर्णाङ्क पङ्क्तिः_अङ्कः, शून्य* retAddress, तार्किक alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_पङ्क्तिः_अङ्कः       = पङ्क्तिः_अङ्कः;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = मिच्छ;
}
ग्रन्थिः_Data::ग्रन्थिः_Data(ग्रन्थिः_Data &ref)
{
    *एतत् = ref;
}
ग्रन्थिः_Data& ग्रन्थिः_Data::operator=(ध्रुव ग्रन्थिः_Data &दक्षिणः)
{
    एतत्->m_funName      = दक्षिणः.m_funName;
    एतत्->m_instrType    = दक्षिणः.m_instrType;
    एतत्->m_implType     = दक्षिणः.m_implType;
    एतत्->m_fileName     = दक्षिणः.m_fileName;
    एतत्->m_पङ्क्तिः_अङ्कः      = दक्षिणः.m_पङ्क्तिः_अङ्कः;
    एतत्->m_retAddress   = दक्षिणः.m_retAddress;
    एतत्->m_alwaysExpand = दक्षिणः.m_alwaysExpand;

    एतत्->m_threads     = दक्षिणः.m_threads;
    एतत्->m_counter     = दक्षिणः.m_counter;
    एतत्->m_ticksTotal  = दक्षिणः.m_ticksTotal;

    एतत्->m_funError    = दक्षिणः.m_funError;

    प्रत्यागम *एतत्;
}
ग्रन्थिः_Data::~ग्रन्थिः_Data()
{
}
तार्किक operator==(ध्रुव ग्रन्थिः_Data& वामः, ध्रुव ग्रन्थिः_Data& दक्षिणः)
{
    जइ(वामः.m_पङ्क्तिः_अङ्कः == दक्षिणः.m_पङ्क्तिः_अङ्कः && वामः.m_funName == दक्षिणः.m_funName && वामः.m_fileName == दक्षिणः.m_fileName)
    {
        जइ(वामः.m_retAddress == दक्षिणः.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || वामः.m_alwaysExpand))
            प्रत्यागम सच्च;
    }
    प्रत्यागम मिच्छ;
}

#ifdef ENABLE_INSTRUMENTATION
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

IntrumentationRegion::IntrumentationRegion(ध्रुव वर्ण* funName, ध्रुव वर्ण* fileName, पूर्णाङ्क पङ्क्तिः_अङ्कः, शून्य *retAddress, तार्किक alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = मिच्छ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    जइ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        जइ(!pTLS->p_Current_ग्रन्थिः)
        {
            m_disabled = सच्च;
            प्रत्यागम;
        }

        पूर्णाङ्क depth = pTLS->p_Current_ग्रन्थिः->getDepth();
        जइ(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = सच्च;
            प्रत्यागम;
        }

        ग्रन्थिः_Data payload(funName, fileName, पङ्क्तिः_अङ्कः, retAddress, alwaysExpand, instrType, implType);
        Node<ग्रन्थिः_Data>* pChild = रिक्त;

        जइ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_ग्रन्थिः->findChild(payload);
            जइ(!pChild)
            {
                pChild = नूतन Node<ग्रन्थिः_Data>(payload);
                pTLS->p_Current_ग्रन्थिः->addChild(pChild);
            }
        }
        अण्णहा
        {
            pChild = pTLS->p_Current_ग्रन्थिः->findChild(payload);
            जइ(!pChild)
            {
                m_disabled = सच्च;
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
    जइ(pStruct->useInstr)
    {
        जइ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            जइ (pTLS->p_Current_ग्रन्थिः->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ग्रन्थिः->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ग्रन्थिः->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_गणनम्() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_गणनम्); // Concurrent ticks accumulation
                pTLS->p_Current_ग्रन्थिः->m_payload.m_counter++;
                pTLS->p_Current_ग्रन्थिः->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_ग्रन्थिः->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_ग्रन्थिः = pTLS->p_Current_ग्रन्थिः->m_pParent;
        }
    }
}
#endif
}

नामाकाश ipp
{

#ifdef HAVE_IPP
संरचना IPPInitSingleton
{
सार्वजनिक:
    IPPInitSingleton()
    {
        useIPP         = सच्च;

#जइ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#अण्णहा
        useIPP_NE      = मिच्छ;
#endif
        ippStatus      = 0;
        funcname       = रिक्त;
        filename       = रिक्त;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_शिखरम्_Features = 0;
        pIppLibInfo    = रिक्त;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, रिक्त);
        जइ(ippStatus < 0)
        {
            CV_LOG_ERROR(रिक्त, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = मिच्छ;
            प्रत्यागम;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        जइ(!env.empty())
        {
#जइ IPP_VERSION_X100 >= 201900
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#नोचेद्यदि IPP_VERSION_X100 >= 201703
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#नोचेद्यदि IPP_VERSION_X100 >= 201700
            ध्रुव Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अण्णहा
            ध्रुव Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            जइ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = सच्च;
                env = env.substr(3, env.size());
            }

            जइ(env == "disabled")
            {
                CV_LOG_WARNING(रिक्त, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = मिच्छ;
            }
            अण्णहा जइ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            अण्णहा जइ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#जइ IPP_VERSION_X100 >= 201700
#जइ defined (_M_AMD64) || defined (__x86_64__)
            अण्णहा जइ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            अण्णहा
                CV_LOG_ERROR(रिक्त, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        जइ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        जइ(!(
#जइ IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = मिच्छ;
            प्रत्यागम;
        }

        जइ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        अण्णहा
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#जइ IPP_VERSION_X100 >= 201700
        जइ(ippFeatures&ippCPUID_AVX512F)
        {
            जइ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_शिखरम्_Features = ippCPUID_AVX512_SKX;
            अण्णहा जइ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_शिखरम्_Features = ippCPUID_AVX512_KNL;
            अण्णहा
                ipp_शिखरम्_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        अण्णहा
#endif
        जइ(ippFeatures&ippCPUID_AVX2)
            ipp_शिखरम्_Features = ippCPUID_AVX2;
        अण्णहा जइ(ippFeatures&ippCPUID_SSE42)
            ipp_शिखरम्_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        जइ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_शिखरम्_Features = ippCPUID_SSE42;
        }
    }

सार्वजनिक:
    तार्किक        useIPP;
    तार्किक        useIPP_NE;

    पूर्णाङ्क         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ध्रुव वर्ण *funcname;
    ध्रुव वर्ण *filename;
    पूर्णाङ्क         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_शिखरम्_Features;
    ध्रुव IppLibraryVersion *pIppLibInfo;
};

स्थिर IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, नूतन IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    प्रत्यागम getIPPSingleton().ippFeatures;
#अण्णहा
    प्रत्यागम 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_शिखरम्_Features()
{
    प्रत्यागम getIPPSingleton().ipp_शिखरम्_Features;
}
#endif

शून्य setIppStatus(पूर्णाङ्क status, ध्रुव वर्ण * ध्रुव _funcname, ध्रुव वर्ण * ध्रुव _filename, पूर्णाङ्क पङ्क्तिः_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = पङ्क्तिः_3;
#अण्णहा
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(पङ्क्तिः_3);
#endif
}

पूर्णाङ्क getIppStatus()
{
#ifdef HAVE_IPP
    प्रत्यागम getIPPSingleton().ippStatus;
#अण्णहा
    प्रत्यागम 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    प्रत्यागम format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#अण्णहा
    प्रत्यागम String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ध्रुव IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    जइ(pInfo)
        प्रत्यागम format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    अण्णहा
        प्रत्यागम String("error");
#अण्णहा
    प्रत्यागम String("disabled");
#endif
}

तार्किक useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    जइ (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    प्रत्यागम (data.useIPP > 0);
#अण्णहा
    प्रत्यागम मिच्छ;
#endif
}

शून्य setUseIPP(तार्किक flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:मिच्छ;
#अण्णहा
    CV_UNUSED(flag);
    data.useIPP = मिच्छ;
#endif
}

तार्किक useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    जइ (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    प्रत्यागम (data.useIPP_NE > 0);
#अण्णहा
    प्रत्यागम मिच्छ;
#endif
}

शून्य setUseIPP_NotExact(तार्किक flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#अण्णहा
    CV_UNUSED(flag);
    data.useIPP_NE = मिच्छ;
#endif
}

} // namespace ipp


नामाकाश details {

#जइ OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

शून्य setFPDenormalsIgnoreHint(तार्किक ignore, CV_OUT FPDenormalsModeState& state)
{
#जइ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    जइ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ध्रुव unsigned old_flags = _mm_getcsr();
    ध्रुव unsigned old_मानम् = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मानम्;
    _mm_setcsr(flags);
#अण्णहा
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

पूर्णाङ्क saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#जइ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    जइ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ध्रुव unsigned old_flags = _mm_getcsr();
    ध्रुव unsigned old_मानम् = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मानम्;
    प्रत्यागम 2;
#अण्णहा
    CV_UNUSED(state);
    प्रत्यागम 0;
#endif
}

तार्किक restoreFPDenormalsState(ध्रुव FPDenormalsModeState& state)
{
#जइ OPENCV_IMPL_FP_HINTS_X86
    ध्रुव unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ध्रुव unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((पूर्णाङ्क)value, value == (value & mask), "invalid SSE FP state");
    ध्रुव unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    प्रत्यागम सच्च;
#अण्णहा
    CV_UNUSED(state);
    प्रत्यागम मिच्छ;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    प्रत्यागम OPENCV_ALGO_HINT_DEFAULT;
#अण्णहा
    प्रत्यागम ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
