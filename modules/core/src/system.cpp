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

#അന്തര്ഭാവ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#അന്തര്ഭാവ <atomic>
#അന്തര്ഭാവ <exception>
#അന്തര്ഭാവ <iostream>
#അന്തര്ഭാവ <ostream>

#ifdef __QNX__
    #അന്തര്ഭാവ <unistd.h>
    #അന്തര്ഭാവ <sys/neutrino.h>
    #അന്തര്ഭാവ <sys/syspage.h>
#ifdef __aarch64__
    #അന്തര്ഭാവ <aarch64/syspage.h>
#endif
#endif

#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/trace.private.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/tls.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/instrumentation.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/filesystem.private.hpp>

#അന്തര്ഭാവ <opencv2/core/utils/fp_control_utils.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/fp_control.private.hpp>

നാമാകാശ cv {

സ്ഥിതം ശൂന്യം _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    സ്ഥിതം std::ios_base::Init s_iostream_initializer;
#endif
}

സ്ഥിതം Mutex* __initialization_mutex = ശൂന്യ;
Mutex& getInitializationMutex()
{
    എങ്കിൽ (__initialization_mutex == ശൂന്യ)
    {
        (ശൂന്യം)_initSystem();
        __initialization_mutex = പുതിയ Mutex();
    }
    തിരികെ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

സ്ഥിതം തർക്കം param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#എങ്കിൽ defined(_DEBUG) || defined(__ANDROID__)
    സത്യം
#അല്ലെങ്കിൽ
    അസത്യം
#endif
);

ശൂന്യം* allocSingletonBuffer(size_t size) { തിരികെ fastMalloc(size); }
ശൂന്യം* allocSingletonNewBuffer(size_t size) { തിരികെ malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# എങ്കിൽ defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#എങ്കിൽ defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# എങ്കിൽ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#അന്തര്ഭാവ <cstdlib>        // std::abort
#endif

#എങ്കിൽ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#എങ്കിൽ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
സ്ഥിരം uint64_t AT_HWCAP = NT_GNU_HWCAP;
#അല്ലെങ്കിൽ
#  include <elf.h>
#endif
#എങ്കിൽ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#എങ്കിൽ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#എങ്കിൽ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#എങ്കിൽ defined __loongarch64
#അന്തര്ഭാവ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#എങ്കിൽ defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#അന്തര്ഭാവ <windows.h>
#അന്തര്ഭാവ <combaseapi.h>
#എങ്കിൽ (_WIN32_WINNT >= 0x0602)
  #അന്തര്ഭാവ <synchapi.h>
#endif
#എങ്കിൽ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #അന്തര്ഭാവ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#അന്തര്ഭാവ <tchar.h>

#ifdef WINRT
#അന്തര്ഭാവ <wrl/client.h>
#ifndef __cplusplus_winrt
#അന്തര്ഭാവ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    തിരികെ std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#അല്ലെങ്കിൽ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    എങ്കിൽ (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        തിരികെ wstr;
    എങ്കിൽ (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        തിരികെ wstr;
    എങ്കിൽ (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        തിരികെ wstr;
    എങ്കിൽ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        തിരികെ wstr;
    എങ്കിൽ (FAILED(storagefolderRef.As(&storageitemRef)))
        തിരികെ wstr;
    str = ശൂന്യ;
    എങ്കിൽ (FAILED(storageitemRef->get_Path(&str)))
        തിരികെ wstr;
    wstr = WindowsGetStringRawBuffer(str, ശൂന്യ);
    WindowsDeleteString(str);
    തിരികെ wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], വലുപ്പം(guidStr)/വലുപ്പം(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    തിരികെ prefix.append(std::wstring(guidStr));
}

#endif
#അല്ലെങ്കിൽ
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#അന്തര്ഭാവ <pthread.h>
#endif
#അന്തര്ഭാവ <sys/time.h>
#അന്തര്ഭാവ <time.h>

#എങ്കിൽ defined __MACH__ && defined __APPLE__
#അന്തര്ഭാവ <mach/mach.h>
#അന്തര്ഭാവ <mach/mach_time.h>
#അന്തര്ഭാവ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#അന്തര്ഭാവ "omp.h"
#endif

#എങ്കിൽ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <sys/types.h>
#എങ്കിൽ defined __ANDROID__
#അന്തര്ഭാവ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #എങ്കിൽ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #എങ്കിൽ _MSC_VER >= 1400  // MSVS 2005
      #അന്തര്ഭാവ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #അല്ലെങ്കിൽ
      #error "Required MSVS 2005+"
    #endif
  #അല്ലെങ്കിലെങ്കിൽ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    സ്ഥിതം ശൂന്യം cv_cpuid(പൂർണ്ണസംഖ്യ* cpuid_data, പൂർണ്ണസംഖ്യ reg_eax, പൂർണ്ണസംഖ്യ reg_ecx)
    {
        പൂർണ്ണസംഖ്യ __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#എങ്കിൽ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#അല്ലെങ്കിലെങ്കിൽ defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#അല്ലെങ്കിൽ
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#അന്തര്ഭാവ <chrono>

നാമാകാശ cv
{

Exception::Exception() { code = Error::StsOk; വരി = 0; }

Exception::Exception(Error::Code _code, സ്ഥിരം String& _err, സ്ഥിരം String& _func, സ്ഥിരം String& _file, പൂർണ്ണസംഖ്യ വരി_3)
: code(_code), err(_err), func(_func), file(_file), വരി(വരി_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
സ്ഥിരം അക്ഷരം* Exception::what() സ്ഥിരം CV_NOEXCEPT { തിരികെ സന്ദേശം.c_str(); }

ശൂന്യം Exception::formatMessage()
{
    size_t pos = err.find('\n');
    തർക്കം multiline = pos != cv::String::npos;
    എങ്കിൽ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ആകുമ്പോൾ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        എങ്കിൽ (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    എങ്കിൽ (func.size() > 0)
    {
        എങ്കിൽ (multiline)
            സന്ദേശം = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), വരി, code, codeMessage(), func.c_str(), err.c_str());
        അല്ലെങ്കിൽ
            സന്ദേശം = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), വരി, code, codeMessage(), err.c_str(), func.c_str());
    }
    അല്ലെങ്കിൽ
    {
        സന്ദേശം = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), വരി, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

സ്ഥിരം അക്ഷരം * Exception::codeMessage() സ്ഥിരം
{
    തിരഞ്ഞെടുപ്പ് (code)
    {
    അവസ്ഥ cv::Error::StsOk :                  തിരികെ "No Error";
    അവസ്ഥ cv::Error::StsBackTrace :           തിരികെ "Backtrace";
    അവസ്ഥ cv::Error::StsError :               തിരികെ "Unspecified error";
    അവസ്ഥ cv::Error::StsInternal :            തിരികെ "Internal error";
    അവസ്ഥ cv::Error::StsNoMem :               തിരികെ "Insufficient memory";
    അവസ്ഥ cv::Error::StsBadArg :              തിരികെ "Bad argument";
    അവസ്ഥ cv::Error::StsNoConv :              തിരികെ "Iterations do not converge";
    അവസ്ഥ cv::Error::StsAutoTrace :           തിരികെ "Autotrace call";
    അവസ്ഥ cv::Error::StsBadSize :             തിരികെ "Incorrect size of input array";
    അവസ്ഥ cv::Error::StsNullPtr :             തിരികെ "Null pointer";
    അവസ്ഥ cv::Error::StsDivByZero :           തിരികെ "Division by zero occurred";
    അവസ്ഥ cv::Error::BadStep :                തിരികെ "Image step is wrong";
    അവസ്ഥ cv::Error::StsInplaceNotSupported : തിരികെ "Inplace operation is not supported";
    അവസ്ഥ cv::Error::StsObjectNotFound :      തിരികെ "Requested object was not found";
    അവസ്ഥ cv::Error::BadDepth :               തിരികെ "Input image depth is not supported by function";
    അവസ്ഥ cv::Error::StsUnmatchedFormats :    തിരികെ "Formats of input arguments do not match";
    അവസ്ഥ cv::Error::StsUnmatchedSizes :      തിരികെ "Sizes of input arguments do not match";
    അവസ്ഥ cv::Error::StsOutOfRange :          തിരികെ "One of the arguments\' values is out of range";
    അവസ്ഥ cv::Error::StsUnsupportedFormat :   തിരികെ "Unsupported format or combination of formats";
    അവസ്ഥ cv::Error::BadCOI :                 തിരികെ "Input COI is not supported";
    അവസ്ഥ cv::Error::Bad_അക്കം_Channels :         തിരികെ "Bad number of channels";
    അവസ്ഥ cv::Error::StsBadFlag :             തിരികെ "Bad flag (parameter or structure field)";
    അവസ്ഥ cv::Error::StsBadPoint :            തിരികെ "Bad parameter of type CvPoint";
    അവസ്ഥ cv::Error::StsBadMask :             തിരികെ "Bad type of mask argument";
    അവസ്ഥ cv::Error::StsParseError :          തിരികെ "Parsing error";
    അവസ്ഥ cv::Error::StsNotImplemented :      തിരികെ "The function/feature is not implemented";
    അവസ്ഥ cv::Error::StsBadMemBlock :         തിരികെ "Memory block has been corrupted";
    അവസ്ഥ cv::Error::StsAssert :              തിരികെ "Assertion failed";
    അവസ്ഥ cv::Error::GpuNotSupported :        തിരികെ "No CUDA support";
    അവസ്ഥ cv::Error::GpuApiCallError :        തിരികെ "Gpu API call";
    അവസ്ഥ cv::Error::OpenGlNotSupported :     തിരികെ "No OpenGL support";
    അവസ്ഥ cv::Error::OpenGlApiCallError :     തിരികെ "OpenGL API call";
    അവസ്ഥ cv::Error::OpenCLApiCallError :     തിരികെ "OpenCL API call error";
    അവസ്ഥ cv::Error::OpenCLDoubleNotSupported:തിരികെ "OpenCL device does not support double";
    അവസ്ഥ cv::Error::OpenCLInitError :        തിരികെ "OpenCL initialization error";
    അവസ്ഥ cv::Error::OpenCLNoAMDBlasFft :     തിരികെ "Missing OpenCL AMD BLAS FFT";
    സ്ഥിരസ്ഥിതി:
    {
        സ്ഥിതം അക്ഷരം buf[256] {0};
        snprintf(buf, വലുപ്പം(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        തിരികെ buf;
    }
    };

}

സ്ഥിതം സ്ഥിരം അക്ഷരം* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { ശൂന്യ };

സ്ഥിതം സ്ഥിരം അക്ഷരം* getHWFeatureName(പൂർണ്ണസംഖ്യ id)
{
    തിരികെ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : ശൂന്യ;
}
സ്ഥിതം സ്ഥിരം അക്ഷരം* getHWFeatureNameSafe(പൂർണ്ണസംഖ്യ id)
{
    സ്ഥിരം അക്ഷരം* name = getHWFeatureName(id);
    തിരികെ name ? name : "Unknown feature";
}

ഘടന HWFeatures
{
    എണ്ണൽ { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(തർക്കം run_initialize = അസത്യം)
    {
        എങ്കിൽ (run_initialize)
            initialize();
    }

    സ്ഥിതം ശൂന്യം initializeNames()
    {
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    ശൂന്യം initialize(ശൂന്യം)
    {
        എങ്കിൽ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        പൂർണ്ണസംഖ്യ cpuid_data[4] = { 0, 0, 0, 0 };
        പൂർണ്ണസംഖ്യ cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        പൂർണ്ണസംഖ്യ x86_family = (cpuid_data[0] >> 8) & 15;
        എങ്കിൽ( x86_family >= 6 )
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
            പൂർണ്ണസംഖ്യ cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            തർക്കം have_AVX_OS_support = സത്യം;
            തർക്കം have_AVX512_OS_support = സത്യം;
            എങ്കിൽ (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = അസത്യം; // OS uses XSAVE_XRSTORE and CPU support AVX
            അല്ലെങ്കിൽ
            {
                പൂർണ്ണസംഖ്യ xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (പൂർണ്ണസംഖ്യ)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #അല്ലെങ്കിലെങ്കിൽ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                എങ്കിൽ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = അസത്യം; // YMM registers
                എങ്കിൽ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = അസത്യം; // ZMM registers
            }

            എങ്കിൽ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = അസത്യം;
                have[CV_CPU_FP16] = അസത്യം;
                have[CV_CPU_AVX2] = അസത്യം;
                have[CV_CPU_FMA3] = അസത്യം;
                have[CV_CPU_AVX_VNNI] = അസത്യം;
            }
            എങ്കിൽ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = അസത്യം;
                have[CV_CPU_AVX_512BW] = അസത്യം;
                have[CV_CPU_AVX_512CD] = അസത്യം;
                have[CV_CPU_AVX_512DQ] = അസത്യം;
                have[CV_CPU_AVX_512ER] = അസത്യം;
                have[CV_CPU_AVX_512IFMA] = അസത്യം;
                have[CV_CPU_AVX_512PF] = അസത്യം;
                have[CV_CPU_AVX_512VBMI] = അസത്യം;
                have[CV_CPU_AVX_512VL] = അസത്യം;
                have[CV_CPU_AVX_512VBMI2] = അസത്യം;
                have[CV_CPU_AVX_512VNNI] = അസത്യം;
                have[CV_CPU_AVX_512BITALG] = അസത്യം;
                have[CV_CPU_AVX_512VPOPCNTDQ] = അസത്യം;
                have[CV_CPU_AVX_5124VNNIW] = അസത്യം;
                have[CV_CPU_AVX_5124FMAPS] = അസത്യം;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            എങ്കിൽ (have[CV_CPU_AVX512_COMMON])
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
            അല്ലെങ്കിൽ
            {
                have[CV_CPU_AVX512_KNL] = അസത്യം;
                have[CV_CPU_AVX512_KNM] = അസത്യം;
                have[CV_CPU_AVX512_SKX] = അസത്യം;
                have[CV_CPU_AVX512_CNL] = അസത്യം;
                have[CV_CPU_AVX512_CLX] = അസത്യം;
                have[CV_CPU_AVX512_ICL] = അസത്യം;
            }
        }
    #endif // CV_CPUID_X86

    #എങ്കിൽ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = സത്യം;
        have[CV_CPU_FP16] = സത്യം;
        പൂർണ്ണസംഖ്യ cpufile = open("/proc/self/auxv", O_RDONLY);

        എങ്കിൽ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            സ്ഥിരം size_t size_auxv_t = വലുപ്പം(auxv);

            ആകുമ്പോൾ ((size_t)പഠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                എങ്കിൽ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#എങ്കിൽ defined(AT_HWCAP2)
                അല്ലെങ്കിൽ എങ്കിൽ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #അല്ലെങ്കിലെങ്കിൽ defined __arm__ && defined __ANDROID__
      #എങ്കിൽ defined HAVE_CPUFEATURES
        CV_LOG_INFO(ശൂന്യ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ശൂന്യ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #അല്ലെങ്കിൽ
        CV_LOG_INFO(ശൂന്യ, "cpufeatures library is not available for CPU detection");
        #എങ്കിൽ CV_NEON
        CV_LOG_INFO(ശൂന്യ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = സത്യം;
        #അല്ലെങ്കിൽ
        CV_LOG_INFO(ശൂന്യ, "- NEON instructions is NOT enabled via build flags");
        #endif
        #എങ്കിൽ CV_FP16
        CV_LOG_INFO(ശൂന്യ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = സത്യം;
        #അല്ലെങ്കിൽ
        CV_LOG_INFO(ശൂന്യ, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #അല്ലെങ്കിലെങ്കിൽ defined __arm__
        പൂർണ്ണസംഖ്യ cpufile = open("/proc/self/auxv", O_RDONLY);

        എങ്കിൽ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            സ്ഥിരം size_t size_auxv_t = വലുപ്പം(auxv);

            ആകുമ്പോൾ ((size_t)പഠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                എങ്കിൽ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    നിർത്തുക;
                }
            }

            close(cpufile);
        }
    #endif
    #അല്ലെങ്കിലെങ്കിൽ (defined __APPLE__)
    #എങ്കിൽ defined __ARM_NEON
        have[CV_CPU_NEON] = സത്യം;
    #endif
    #എങ്കിൽ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = സത്യം;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    പൂർണ്ണസംഖ്യ has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = വലുപ്പം(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, ശൂന്യ, 0);
    എങ്കിൽ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = സത്യം;
    }
    പൂർണ്ണസംഖ്യ has_feat_bf16 = 0;
    size_t has_feat_bf16_size = വലുപ്പം(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, ശൂന്യ, 0);
    എങ്കിൽ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = സത്യം;
    }
    #അല്ലെങ്കിലെങ്കിൽ (defined __clang__)
    #എങ്കിൽ defined __ARM_NEON
        have[CV_CPU_NEON] = സത്യം;
        #എങ്കിൽ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = സത്യം;
        #endif
    #endif
    #endif
    #എങ്കിൽ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = സത്യം;
    #endif
    #എങ്കിൽ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = സത്യം;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = സത്യം;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = സത്യം;
    #endif

    #എങ്കിൽ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned പൂർണ്ണസംഖ്യ hwcap = getauxval(AT_HWCAP);
        എങ്കിൽ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            എങ്കിൽ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = സത്യം;
            } അല്ലെങ്കിൽ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #അല്ലെങ്കിലെങ്കിൽ (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        എങ്കിൽ (elf_aux_info(AT_HWCAP, &hwcap, വലുപ്പം(hwcap)) != 0)
            hwcap = 0;
        എങ്കിൽ (hwcap & PPC_FEATURE_HAS_VSX) {
            എങ്കിൽ (elf_aux_info(AT_HWCAP2, &hwcap2, വലുപ്പം(hwcap2)) != 0)
                hwcap2 = 0;
            എങ്കിൽ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = സത്യം;
            } അല്ലെങ്കിൽ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #അല്ലെങ്കിൽ
        // TODO: AIX
        #എങ്കിൽ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = സത്യം;
        #endif
        #എങ്കിൽ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = സത്യം;
        #endif
    #endif

    #എങ്കിൽ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = സത്യം;
        #എങ്കിൽ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = സത്യം;
        #endif
        #എങ്കിൽ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = സത്യം;
        #endif
    #endif

    #എങ്കിൽ defined __loongarch64 && defined __linux__
        പൂർണ്ണസംഖ്യ flag = (പൂർണ്ണസംഖ്യ)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        തർക്കം skip_baseline_check = അസത്യം;
        എങ്കിൽ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = സത്യം;
        }
        പൂർണ്ണസംഖ്യ baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        എങ്കിൽ (!checkFeatures(baseline_features, വലുപ്പം(baseline_features) / വലുപ്പം(baseline_features[0]))
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
            checkFeatures(baseline_features, വലുപ്പം(baseline_features) / വലുപ്പം(baseline_features[0]), സത്യം);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, വലുപ്പം(baseline_features) / വലുപ്പം(baseline_features[0]));
    }

    തർക്കം checkFeatures(സ്ഥിരം പൂർണ്ണസംഖ്യ* features, പൂർണ്ണസംഖ്യ എണ്ണം_2, തർക്കം dump = അസത്യം)
    {
        തർക്കം result = സത്യം;
        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < എണ്ണം_2; i++)
        {
            പൂർണ്ണസംഖ്യ feature = features[i];
            എങ്കിൽ (feature)
            {
                എങ്കിൽ (have[feature])
                {
                    എങ്കിൽ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                അല്ലെങ്കിൽ
                {
                    result = അസത്യം;
                    എങ്കിൽ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        തിരികെ result;
    }

    സ്ഥിതം വരിയിൽ തർക്കം is_ചിഹ്നം_Separator(അക്ഷരം c)
    {
        തിരികെ c == ',' || c == ';';
    }

    ശൂന്യം readSettings(സ്ഥിരം പൂർണ്ണസംഖ്യ* baseline_features, പൂർണ്ണസംഖ്യ baseline_എണ്ണം)
    {
        തർക്കം dump = സത്യം;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        എങ്കിൽ (!disabled_features.empty())
        {
            സ്ഥിരം അക്ഷരം* start = disabled_features.c_str();
            വേണ്ടി (;;)
            {
                ആകുമ്പോൾ (start[0] != 0 && is_ചിഹ്നം_Separator(start[0]))
                {
                    start++;
                }
                എങ്കിൽ (start[0] == 0)
                    നിർത്തുക;
                സ്ഥിരം അക്ഷരം* end = start;
                ആകുമ്പോൾ (end[0] != 0 && !is_ചിഹ്നം_Separator(end[0]))
                {
                    end++;
                }
                എങ്കിൽ (end == start)
                    തുടരുക;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                തർക്കം found = അസത്യം;
                വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    എങ്കിൽ (!g_hwFeatureNames[i]) തുടരുക;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    എങ്കിൽ (len != feature.size()) തുടരുക;
                    എങ്കിൽ (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        തർക്കം isBaseline = അസത്യം;
                        വേണ്ടി (പൂർണ്ണസംഖ്യ k = 0; k < baseline_എണ്ണം; k++)
                        {
                            എങ്കിൽ (baseline_features[k] == i)
                            {
                                isBaseline = സത്യം;
                                നിർത്തുക;
                            }
                        }
                        എങ്കിൽ (isBaseline)
                        {
                            എങ്കിൽ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        എങ്കിൽ (!have[i])
                        {
                            എങ്കിൽ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = അസത്യം;

                        found = സത്യം;
                        നിർത്തുക;
                    }
                }
                എങ്കിൽ (!found)
                {
                    എങ്കിൽ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    തർക്കം have[MAX_FEATURE+1]{};
};

സ്ഥിതം HWFeatures  featuresEnabled(സത്യം), featuresDisabled = HWFeatures(അസത്യം);
സ്ഥിതം HWFeatures* currentFeatures = &featuresEnabled;

തർക്കം checkHardwareSupport(പൂർണ്ണസംഖ്യ feature)
{
    എങ്കിൽ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        തിരികെ അസത്യം;
    തിരികെ currentFeatures->have[feature];
}

String getHardwareFeatureName(പൂർണ്ണസംഖ്യ feature)
{
    സ്ഥിരം അക്ഷരം* name = getHWFeatureName(feature);
    തിരികെ name ? String(name) : String();
}

std::string get_CPUFeatures_വരി()
{
    സ്ഥിരം പൂർണ്ണസംഖ്യ features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    സ്ഥിരം പൂർണ്ണസംഖ്യ sz = വലുപ്പം(features) / വലുപ്പം(features[0]);
    std::string result;
    std::string prefix;
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 1; i < sz; ++i)
    {
        എങ്കിൽ (features[i] == 0)
        {
            prefix = "*";
            തുടരുക;
        }
        എങ്കിൽ (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        എങ്കിൽ (!checkHardwareSupport(features[i])) result.append("?");
    }
    തിരികെ result;
}

അസ്ഥിര തർക്കം useOptimizedFlag = സത്യം;

ശൂന്യം setUseOptimized( തർക്കം flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

തർക്കം useOptimized(ശൂന്യം)
{
    തിരികെ useOptimizedFlag;
}

int64 get_Tick_എണ്ണം(ശൂന്യം)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    തിരികെ (int64)now.time_since_epoch().എണ്ണം_2();
}

ഇരട്ട getTickFrequency(ശൂന്യം)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    ഇരട്ട clock_freq = clock_period_t::den / clock_period_t::num;
    തിരികെ clock_freq;
}

#എങ്കിൽ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#എങ്കിൽ defined(__i386__)

int64 get_CPUTick_എണ്ണം(ശൂന്യം)
{
    int64 x;
    __asm__ അസ്ഥിര (".byte 0x0f, 0x31" : "=A" (x));
    തിരികെ x;
}
#അല്ലെങ്കിലെങ്കിൽ defined(__x86_64__)

int64 get_CPUTick_എണ്ണം(ശൂന്യം)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    തിരികെ (int64)lo | ((int64)hi << 32);
}

#അല്ലെങ്കിലെങ്കിൽ defined(__ppc__)

int64 get_CPUTick_എണ്ണം(ശൂന്യം)
{
    unsigned upper, lower, tmp;
    __asm__ അസ്ഥിര(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    തിരികെ lower | ((int64)upper << 32);
}

#അല്ലെങ്കിൽ

#error "RDTSC not defined"

#endif

#അല്ലെങ്കിലെങ്കിൽ defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_എണ്ണം(ശൂന്യം)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#അല്ലെങ്കിൽ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_എണ്ണം(ശൂന്യം)
{
    തിരികെ get_Tick_എണ്ണം();
}
//#endif

#endif


നാമാകാശ internal {

വർഗ്ഗം Timestamp
{
പൊതു:
    സ്ഥിരം int64 zero_Tick_എണ്ണം;
    സ്ഥിരം ഇരട്ട ns_in_ticks;

    Timestamp()
        : zero_Tick_എണ്ണം(get_Tick_എണ്ണം())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_എണ്ണം();
        തിരികെ (int64)((t - zero_Tick_എണ്ണം) * ns_in_ticks);
    }

    സ്ഥിതം Timestamp& getInstance()
    {
        സ്ഥിതം Timestamp g_timestamp;
        തിരികെ g_timestamp;
    }
};

വർഗ്ഗം InitTimestamp {
പൊതു:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
സ്ഥിതം InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    തിരികെ internal::Timestamp::getInstance().getTimestamp();
}


സ്ഥിരം String& getBuildInformation()
{
    സ്ഥിതം String build_info =
#അന്തര്ഭാവ "version_string.inc"
    ;
    തിരികെ build_info;
}

String getVersionString() { തിരികെ String(CV_VERSION); }

പൂർണ്ണസംഖ്യ getVersionMajor() { തിരികെ CV_VERSION_MAJOR; }

പൂർണ്ണസംഖ്യ getVersionMinor() { തിരികെ CV_VERSION_MINOR; }

പൂർണ്ണസംഖ്യ getVersionRevision() { തിരികെ CV_VERSION_REVISION; }

String format( സ്ഥിരം അക്ഷരം* fmt, ... )
{
    AutoBuffer<അക്ഷരം, 1024> buf;

    വേണ്ടി ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        പൂർണ്ണസംഖ്യ bsize = static_cast<പൂർണ്ണസംഖ്യ>(buf.size());
        പൂർണ്ണസംഖ്യ len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        എങ്കിൽ (len >= bsize)
        {
            buf.resize(len + 1);
            തുടരുക;
        }
        buf[bsize - 1] = 0;
        തിരികെ String(buf.data(), len);
    }
}

String tempfile( സ്ഥിരം അക്ഷരം* suffix )
{
#എങ്കിൽ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#എങ്കിൽ defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    എങ്കിൽ (temp_file.empty())
        തിരികെ String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    അക്ഷരം aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#അല്ലെങ്കിലെങ്കിൽ defined(_WIN32_WCE)
    സ്ഥിരം auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    എങ്കിൽ(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        അക്ഷരം aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#അല്ലെങ്കിൽ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    എങ്കിൽ (temp_dir.empty())
    {
        ::GetTempPathW(വലുപ്പം(temp_dir2_w)/വലുപ്പം(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        പൂർണ്ണസംഖ്യ len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, ശൂന്യ, 0, ശൂന്യ, ശൂന്യ);
        എങ്കിൽ (len > 0)
        {
            std::vector<അക്ഷരം> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, ശൂന്യ, ശൂന്യ);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    എങ്കിൽ (FAILED(hr))
        തിരികെ String();
    അക്ഷരം guidStr[40];
    സ്ഥിരം അക്ഷരം* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, വലുപ്പം(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned പൂർണ്ണസംഖ്യ)g.Data4[0], (unsigned പൂർണ്ണസംഖ്യ)g.Data4[1],
            (unsigned പൂർണ്ണസംഖ്യ)g.Data4[2], (unsigned പൂർണ്ണസംഖ്യ)g.Data4[3], (unsigned പൂർണ്ണസംഖ്യ)g.Data4[4],
            (unsigned പൂർണ്ണസംഖ്യ)g.Data4[5], (unsigned പൂർണ്ണസംഖ്യ)g.Data4[6], (unsigned പൂർണ്ണസംഖ്യ)g.Data4[7]);

    fname = temp_dir;
    എങ്കിൽ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# അല്ലെങ്കിൽ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    അക്ഷരം defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  അല്ലെങ്കിൽ
    അക്ഷരം defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    എങ്കിൽ (temp_dir.empty())
        fname = defaultTemplate;
    അല്ലെങ്കിൽ
    {
        fname = temp_dir;
        അക്ഷരം ech = fname[fname.size() - 1];
        എങ്കിൽ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    സ്ഥിരം പൂർണ്ണസംഖ്യ fd = mkstemp((അക്ഷരം*)fname.c_str());
    എങ്കിൽ (fd == -1) തിരികെ String();

    close(fd);
    remove(fname.c_str());
# endif

    എങ്കിൽ (suffix)
    {
        എങ്കിൽ (suffix[0] != '.')
            തിരികെ fname + "." + suffix;
        അല്ലെങ്കിൽ
            തിരികെ fname + suffix;
    }
    തിരികെ fname;
#അല്ലെങ്കിൽ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

സ്ഥിതം ErrorCallback customErrorCallback = 0;
സ്ഥിതം ശൂന്യം* customErrorCallbackData = 0;
സ്ഥിതം തർക്കം breakOnError = അസത്യം;

തർക്കം setBreakOnError(തർക്കം value)
{
    തർക്കം prevVal = breakOnError;
    breakOnError = value;
    തിരികെ prevVal;
}

പൂർണ്ണസംഖ്യ cv_snprintf(അക്ഷരം* buf, പൂർണ്ണസംഖ്യ len, സ്ഥിരം അക്ഷരം* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    പൂർണ്ണസംഖ്യ res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    തിരികെ res;
}

പൂർണ്ണസംഖ്യ cv_vsnprintf(അക്ഷരം* buf, പൂർണ്ണസംഖ്യ len, സ്ഥിരം അക്ഷരം* fmt, va_list args)
{
#എങ്കിൽ defined _MSC_VER
    എങ്കിൽ (len <= 0) തിരികെ len == 0 ? 1024 : -1;
    പൂർണ്ണസംഖ്യ res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    എങ്കിൽ (res >= 0 && res < len)
    {
        buf[res] = 0;
        തിരികെ res;
    }
    അല്ലെങ്കിൽ
    {
        buf[len - 1] = 0; // truncate happened
        തിരികെ res >= len ? res : (len * 2);
    }
#അല്ലെങ്കിൽ
    തിരികെ vsnprintf(buf, len, fmt, args);
#endif
}

സ്ഥിതം ശൂന്യം dumpException(സ്ഥിരം Exception& exc)
{
    സ്ഥിരം അക്ഷരം* errorStr = exc.codeMessage();
    അക്ഷരം buf[1 << 12];

    cv_snprintf(buf, വലുപ്പം(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.വരി);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#അല്ലെങ്കിൽ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
സ്ഥിതം തർക്കം cv_terminate_handler_installed = അസത്യം;
സ്ഥിതം std::terminate_handler cv_old_terminate_handler;
സ്ഥിതം cv::Exception cv_terminate_handler_exception;
സ്ഥിതം തർക്കം param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", സത്യം);
സ്ഥിതം ശൂന്യം cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    എങ്കിൽ (അസത്യം /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# എങ്കിൽ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

ശൂന്യം error( സ്ഥിരം Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        എങ്കിൽ (!cv_terminate_handler_installed)
        {
            എങ്കിൽ (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = സത്യം;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    എങ്കിൽ (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.വരി, customErrorCallbackData);
    അല്ലെങ്കിൽ എങ്കിൽ (param_dumpErrors)
    {
        dumpException(exc);
    }

    എങ്കിൽ(breakOnError)
    {
        std::terminate();
    }

    എറിയുക exc;
#ifdef __GNUC__
# എങ്കിൽ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

ശൂന്യം error(Error::Code _code, സ്ഥിരം String& _err, സ്ഥിരം അക്ഷരം* _func, സ്ഥിരം അക്ഷരം* _file, പൂർണ്ണസംഖ്യ വരി_3)
{
    error(cv::Exception(_code, _err, _func, _file, വരി_3));
#ifdef __GNUC__
# എങ്കിൽ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# എങ്കിൽ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, ശൂന്യം* userdata, ശൂന്യം** prevUserdata)
{
    എങ്കിൽ( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    തിരികെ prevCallback;
}

ശൂന്യം terminate(Error::Code _code, സ്ഥിരം String& _err, സ്ഥിരം അക്ഷരം* _func, സ്ഥിരം അക്ഷരം* _file, പൂർണ്ണസംഖ്യ വരി_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, വരി_3));
    std::terminate();
}

}

നാമാകാശ cv {
തർക്കം __termination = അസത്യം;


//////////////////////////////// thread-local storage ////////////////////////////////

നാമാകാശ details {

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
വർഗ്ഗം TlsAbstraction
{
പൊതു:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    ശൂന്യം* getData() സ്ഥിരം;
    ശൂന്യം setData(ശൂന്യം *pData);

    ശൂന്യം releaseSystemResources();

സ്വകാര്യ:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_താക്കോൽ;
    തർക്കം disposed;
#endif
#അല്ലെങ്കിൽ // _WIN32
    pthread_key_t  tls_താക്കോൽ;
    std::atomic<തർക്കം> disposed;
#endif
};

വർഗ്ഗം TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
പൊതു:
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
സ്ഥിതം TlsAbstraction* getTlsAbstraction()
{
    സ്ഥിതം TlsAbstraction *g_tls = പുതിയ TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    സ്ഥിതം TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    തിരികെ g_tls;
}


#ifdef _WIN32
#ifdef WINRT
സ്ഥിതം __declspec( thread ) ശൂന്യം* tlsData = ശൂന്യ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
ശൂന്യം TlsAbstraction::releaseSystemResources()
{
    cv::__termination = സത്യം;  // DllMain is missing in static builds
}
ശൂന്യം* TlsAbstraction::getData() സ്ഥിരം
{
    തിരികെ tlsData;
}
ശൂന്യം TlsAbstraction::setData(ശൂന്യം *pData)
{
    tlsData = pData;
}
#അല്ലെങ്കിൽ //WINRT
#ifdef CV_USE_FLS
സ്ഥിതം ശൂന്യം NTAPI opencv_fls_destructor(ശൂന്യം* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(അസത്യം)
{
#ifndef CV_USE_FLS
    tls_താക്കോൽ = TlsAlloc();
#അല്ലെങ്കിൽ // CV_USE_FLS
    tls_താക്കോൽ = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_താക്കോൽ != TLS_OUT_OF_INDEXES);
}
ശൂന്യം TlsAbstraction::releaseSystemResources()
{
    cv::__termination = സത്യം;  // DllMain is missing in static builds
    disposed = സത്യം;
#ifndef CV_USE_FLS
    TlsFree(tls_താക്കോൽ);
#അല്ലെങ്കിൽ // CV_USE_FLS
    FlsFree(tls_താക്കോൽ);
#endif // CV_USE_FLS
    tls_താക്കോൽ = TLS_OUT_OF_INDEXES;
}
ശൂന്യം* TlsAbstraction::getData() സ്ഥിരം
{
    എങ്കിൽ (disposed)
        തിരികെ ശൂന്യ;
#ifndef CV_USE_FLS
    തിരികെ Tls_Get_മൂല്യം(tls_താക്കോൽ);
#അല്ലെങ്കിൽ // CV_USE_FLS
    തിരികെ Fls_Get_മൂല്യം(tls_താക്കോൽ);
#endif // CV_USE_FLS
}
ശൂന്യം TlsAbstraction::setData(ശൂന്യം *pData)
{
    എങ്കിൽ (disposed)
        തിരികെ;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_മൂല്യം(tls_താക്കോൽ, pData) == TRUE);
#അല്ലെങ്കിൽ // CV_USE_FLS
    CV_Assert(Fls_Set_മൂല്യം(tls_താക്കോൽ, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#അല്ലെങ്കിൽ // _WIN32
സ്ഥിതം ശൂന്യം opencv_tls_destructor(ശൂന്യം* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(അസത്യം)
{
    CV_Assert(pthread_key_create(&tls_താക്കോൽ, opencv_tls_destructor) == 0);
}
ശൂന്യം TlsAbstraction::releaseSystemResources()
{
    cv::__termination = സത്യം;  // DllMain is missing in static builds
    disposed = സത്യം;
    എങ്കിൽ (pthread_key_delete(tls_താക്കോൽ) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
ശൂന്യം* TlsAbstraction::getData() സ്ഥിരം
{
    എങ്കിൽ (disposed)
        തിരികെ ശൂന്യ;
    തിരികെ pthread_getspecific(tls_താക്കോൽ);
}
ശൂന്യം TlsAbstraction::setData(ശൂന്യം *pData)
{
    എങ്കിൽ (disposed)
        തിരികെ;  // no-op
    CV_Assert(pthread_setspecific(tls_താക്കോൽ, pData) == 0);
}
#endif

// Per-thread data structure
ഘടന ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<ശൂന്യം*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


സ്ഥിതം തർക്കം g_isTlsStorageInitialized = അസത്യം;

// Main TLS storage class
വർഗ്ഗം TlsStorage
{
പൊതു:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (ശൂന്യം)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = സത്യം;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    ശൂന്യം releaseThread(ശൂന്യം* tls_മൂല്യം = ശൂന്യ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        എങ്കിൽ (ശൂന്യ == tls)
            തിരികെ;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_മൂല്യം == ശൂന്യ ? (ThreadData*)tls->getData() : (ThreadData*)tls_മൂല്യം;
        എങ്കിൽ (pTD == ശൂന്യ)
            തിരികെ;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        വേണ്ടി (size_t i = 0; i < threads.size(); i++)
        {
            എങ്കിൽ (pTD == threads[i])
            {
                threads[i] = ശൂന്യ;
                എങ്കിൽ (tls_മൂല്യം == ശൂന്യ)
                    tls->setData(0);
                std::vector<ശൂന്യം*>& thread_slots = pTD->slots;
                വേണ്ടി (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    ശൂന്യം* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = ശൂന്യ;
                    എങ്കിൽ (!pData)
                        തുടരുക;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    എങ്കിൽ (container)
                        container->deleteDataInstance(pData);
                    അല്ലെങ്കിൽ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (പൂർണ്ണസംഖ്യ)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                തിരികെ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (ശൂന്യം*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        വേണ്ടി(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            എങ്കിൽ (tlsSlots[slot].container == ശൂന്യ)
            {
                tlsSlots[slot].container = container;
                തിരികെ slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        തിരികെ tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    ശൂന്യം releaseSlot(size_t slotIdx, std::vector<ശൂന്യം*> &dataVec, തർക്കം keepSlot = അസത്യം)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        വേണ്ടി(size_t i = 0; i < threads.size(); i++)
        {
            എങ്കിൽ(threads[i])
            {
                std::vector<ശൂന്യം*>& thread_slots = threads[i]->slots;
                എങ്കിൽ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = ശൂന്യ;
                }
            }
        }

        എങ്കിൽ (!keepSlot)
        {
            tlsSlots[slotIdx].container = ശൂന്യ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ശൂന്യം* getData(size_t slotIdx) സ്ഥിരം
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        എങ്കിൽ (ശൂന്യ == tls)
            തിരികെ ശൂന്യ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        എങ്കിൽ(threadData && threadData->slots.size() > slotIdx)
            തിരികെ threadData->slots[slotIdx];

        തിരികെ ശൂന്യ;
    }

    // Gather data from threads by TLS storage index
    ശൂന്യം gather(size_t slotIdx, std::vector<ശൂന്യം*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        വേണ്ടി(size_t i = 0; i < threads.size(); i++)
        {
            എങ്കിൽ(threads[i])
            {
                std::vector<ശൂന്യം*>& thread_slots = threads[i]->slots;
                എങ്കിൽ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    ശൂന്യം setData(size_t slotIdx, ശൂന്യം* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        എങ്കിൽ (ശൂന്യ == tls)
            തിരികെ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        എങ്കിൽ(!threadData)
        {
            threadData = പുതിയ ThreadData;
            tls->setData((ശൂന്യം*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                തർക്കം found = അസത്യം;
                // Find unused slots
                വേണ്ടി(size_t slot = 0; slot < threads.size(); slot++)
                {
                    എങ്കിൽ (threads[slot] == ശൂന്യ)
                    {
                        threadData->idx = (പൂർണ്ണസംഖ്യ)slot;
                        threads[slot] = threadData;
                        found = സത്യം;
                        നിർത്തുക;
                    }
                }

                എങ്കിൽ (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        എങ്കിൽ(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, ശൂന്യ);
        }
        threadData->slots[slotIdx] = pData;
    }

സ്വകാര്യ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ഘടന TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ഘടന TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
സ്ഥിതം TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, പുതിയ TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
സ്ഥിതം ശൂന്യം opencv_tls_destructor(ശൂന്യം* pData)
{
    എങ്കിൽ (!g_isTlsStorageInitialized)
        തിരികെ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#അല്ലെങ്കിൽ // _WIN32
#ifdef CV_USE_FLS
സ്ഥിതം ശൂന്യം WINAPI opencv_fls_destructor(ശൂന്യം* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    എങ്കിൽ (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = സത്യം;
    }

    എങ്കിൽ (!g_isTlsStorageInitialized)
        തിരികെ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

സ്ഥിതം TlsStorage* സ്ഥിരം g_force_initialization_of_TlsStorage
#എങ്കിൽ defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#അല്ലെങ്കിൽ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
വർഗ്ഗം TlsStorage
{
പൊതു:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        വേണ്ടി (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            എങ്കിൽ (container && s.data)
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
        വേണ്ടി (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            എങ്കിൽ (s.container == ശൂന്യ)
            {
                CV_Assert(!s.data);
                s.container = container;
                തിരികെ slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        തിരികെ slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    ശൂന്യം releaseSlot(size_t slotIdx, std::vector<ശൂന്യം*> &dataVec, തർക്കം keepSlot = അസത്യം)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ശൂന്യം* data = s.data;
        എങ്കിൽ (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        എങ്കിൽ (!keepSlot)
        {
            s.container = ശൂന്യ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ശൂന്യം* getData(size_t slotIdx) സ്ഥിരം
    {
        CV_Assert(slotIdx < slots.size());
        സ്ഥിരം SlotInfo& s = slots[slotIdx];
        തിരികെ s.data;
    }

    // Gather data from threads by TLS storage index
    ശൂന്യം gather(size_t slotIdx, std::vector<ശൂന്യം*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ശൂന്യം* data = s.data;
        എങ്കിൽ (data)
            dataVec.push_back(data);
        തിരികെ;
    }

    // Set data to storage index
    ശൂന്യം setData(size_t slotIdx, ശൂന്യം* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

സ്വകാര്യ:
    ഘടന SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        ശൂന്യം* data;
    };
    std::vector<ഘടന SlotInfo> slots;
};

സ്ഥിതം TlsStorage& getTlsStorage()
{
    സ്ഥിതം TlsStorage g_storage;  // no threading
    തിരികെ g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using നാമാകാശ details;

ശൂന്യം releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    എങ്കിൽ (!g_isTlsStorageInitialized)
        തിരികെ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    താക്കോൽ_2 = (പൂർണ്ണസംഖ്യ)getTlsStorage().reserveSlot(ഇത്); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(താക്കോൽ_2 == -1); // Key must be released in child object
}

ശൂന്യം TLSDataContainer::gatherData(std::vector<ശൂന്യം*> &data) സ്ഥിരം
{
    getTlsStorage().gather(താക്കോൽ_2, data);
}

ശൂന്യം TLSDataContainer::detachData(std::vector<ശൂന്യം*> &data)
{
    getTlsStorage().releaseSlot(താക്കോൽ_2, data, സത്യം);
}

ശൂന്യം TLSDataContainer::release()
{
    എങ്കിൽ (താക്കോൽ_2 == -1)
        തിരികെ;  // already released
    std::vector<ശൂന്യം*> data; data.reserve(32);
    getTlsStorage().releaseSlot(താക്കോൽ_2, data, അസത്യം); // Release key and get stored data for proper destruction
    താക്കോൽ_2 = -1;
    വേണ്ടി(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ശൂന്യം TLSDataContainer::cleanup()
{
    std::vector<ശൂന്യം*> data; data.reserve(32);
    getTlsStorage().releaseSlot(താക്കോൽ_2, data, സത്യം); // Extract stored data with removal from TLS tables
    വേണ്ടി(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ശൂന്യം* TLSDataContainer::getData() സ്ഥിരം
{
    CV_Assert(താക്കോൽ_2 != -1 && "Can't fetch data from terminated TLS container.");
    ശൂന്യം* pData = getTlsStorage().getData(താക്കോൽ_2); // Check if data was already allocated
    എങ്കിൽ(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        ശ്രമിക്കുക
        {
            getTlsStorage().setData(താക്കോൽ_2, pData);
        }
        പിടിക്കുക (...)
        {
            deleteDataInstance(pData);
            എറിയുക;
        }
    }
    തിരികെ pData;
}

സ്ഥിതം TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, പുതിയ TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    തിരികെ getCoreTlsDataTLS().getRef();
}

#എങ്കിൽ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

ബാഹ്യ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

ബാഹ്യ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    എങ്കിൽ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        എങ്കിൽ (lpReserved != ശൂന്യ) // called after ExitProcess() call
        {
            cv::__termination = സത്യം;
        }
        അല്ലെങ്കിൽ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    തിരികെ TRUE;
}
#endif


നാമാകാശ {

#ifdef OPENCV_WITH_ITT
തർക്കം overrideThreadName()
{
    സ്ഥിതം തർക്കം param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", അസത്യം);
    തിരികെ param;
}
#endif

സ്ഥിതം പൂർണ്ണസംഖ്യ g_thread_അക്കം = 0;
വർഗ്ഗം ThreadID {
പൊതു:
    സ്ഥിരം പൂർണ്ണസംഖ്യ id;
    ThreadID() :
        id(CV_XADD(&g_thread_അക്കം, 1))
    {
#ifdef OPENCV_WITH_ITT
        എങ്കിൽ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

സ്ഥിതം TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, പുതിയ TLSData<ThreadID>());
}

} // namespace
പൂർണ്ണസംഖ്യ utils::getThreadID() { തിരികെ getThreadIDTLS().get()->id; }


വർഗ്ഗം ParseError
{
    std::string bad_മൂല്യം;
പൊതു:
    ParseError(സ്ഥിരം std::string &bad_മൂല്യം_2) :bad_മൂല്യം(bad_മൂല്യം_2) {}
    std::string toString(സ്ഥിരം std::string &param) സ്ഥിരം
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_മൂല്യം;
        തിരികെ out.str();
    }
};

template <typename T>
T parseOption(സ്ഥിരം std::string &);

template<>
വരിയിൽ തർക്കം parseOption(സ്ഥിരം std::string & value)
{
    എങ്കിൽ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        തിരികെ സത്യം;
    }
    എങ്കിൽ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        തിരികെ അസത്യം;
    }
    എറിയുക ParseError(value);
}

template<>
വരിയിൽ size_t parseOption(സ്ഥിരം std::string &value)
{
    size_t pos = 0;
    വേണ്ടി (; pos < value.size(); pos++)
    {
        എങ്കിൽ (!isdigit(value[pos]))
            നിർത്തുക;
    }
    cv::String മൂല്യം_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(മൂല്യം_Str);
    എങ്കിൽ (suffixStr.length() == 0)
        തിരികെ v;
    അല്ലെങ്കിൽ എങ്കിൽ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        തിരികെ v * 1024 * 1024;
    അല്ലെങ്കിൽ എങ്കിൽ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        തിരികെ v * 1024;
    എറിയുക ParseError(value);
}

template<>
വരിയിൽ cv::String parseOption(സ്ഥിരം std::string &value)
{
    തിരികെ value;
}

template<>
വരിയിൽ utils::Paths parseOption(സ്ഥിരം std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    സ്ഥിരം അക്ഷരം sep = ';';
#അല്ലെങ്കിൽ
    സ്ഥിരം അക്ഷരം sep = ':';
#endif
    size_t start_pos = 0;
    ആകുമ്പോൾ (start_pos != std::string::npos)
    {
        സ്ഥിരം size_t pos = value.find(sep, start_pos);
        സ്ഥിരം std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        എങ്കിൽ (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    തിരികെ result;
}

സ്ഥിതം വരിയിൽ സ്ഥിരം അക്ഷരം * envRead(സ്ഥിരം അക്ഷരം * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    തിരികെ ശൂന്യ;
#അല്ലെങ്കിൽ
    തിരികെ getenv(name);
#endif
}

template<typename T>
വരിയിൽ T പഠ(സ്ഥിരം std::string & k, സ്ഥിരം T & default_മൂല്യം_2)
{
    ശ്രമിക്കുക
    {
        സ്ഥിരം അക്ഷരം * res = envRead(k.c_str());
        എങ്കിൽ (res)
            തിരികെ parseOption<T>(std::string(res));
    }
    പിടിക്കുക (സ്ഥിരം ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    തിരികെ default_മൂല്യം_2;
}

തർക്കം utils::getConfigurationParameterBool(സ്ഥിരം അക്ഷരം* name, തർക്കം default_മൂല്യം_2)
{
    തിരികെ പഠ<തർക്കം>(name, default_മൂല്യം_2);
}

size_t utils::getConfigurationParameterSizeT(സ്ഥിരം അക്ഷരം* name, size_t default_മൂല്യം_2)
{
    തിരികെ പഠ<size_t>(name, default_മൂല്യം_2);
}

std::string utils::getConfigurationParameterString(സ്ഥിരം അക്ഷരം* name, സ്ഥിരം std::string & default_മൂല്യം_2)
{
    തിരികെ പഠ<cv::String>(name, default_മൂല്യം_2);
}

utils::Paths utils::getConfigurationParameterPaths(സ്ഥിരം അക്ഷരം* name, സ്ഥിരം utils::Paths &default_മൂല്യം_2)
{
    തിരികെ പഠ<utils::Paths>(name, default_മൂല്യം_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, പുതിയ ImplCollector())
}

ശൂന്യം setImpl(പൂർണ്ണസംഖ്യ flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

ശൂന്യം addImpl(പൂർണ്ണസംഖ്യ flag, സ്ഥിരം അക്ഷരം* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    എങ്കിൽ(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        എങ്കിൽ(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

പൂർണ്ണസംഖ്യ getImpl(std::vector<പൂർണ്ണസംഖ്യ> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    തിരികെ getImplData().implFlags; // return actual flags for lazy collection
}

തർക്കം useCollection()
{
    തിരികെ getImplData().useCollection;
}

ശൂന്യം setUseCollection(തർക്കം flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

നാമാകാശ instr
{
തർക്കം useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    തിരികെ getInstrumentStruct().useInstr;
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം setUseInstrumentation(തർക്കം flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#അല്ലെങ്കിൽ
    CV_UNUSED(flag);
#endif
}

Instr_കണ്ണി* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    തിരികെ &getInstrumentStruct().വേര്_കണ്ണി_2;
#അല്ലെങ്കിൽ
    തിരികെ ശൂന്യ;
#endif
}

ശൂന്യം resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().വേര്_കണ്ണി_2.removeChilds();
    getInstrumentTLSStruct().p_Current_കണ്ണി = &getInstrumentStruct().വേര്_കണ്ണി_2;
#endif
}

ശൂന്യം setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#അല്ലെങ്കിൽ
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    തിരികെ (FLAGS)getInstrumentStruct().flags;
#അല്ലെങ്കിൽ
    തിരികെ (FLAGS)0;
#endif
}

കണ്ണി_Data::കണ്ണി_Data(സ്ഥിരം അക്ഷരം* funName, സ്ഥിരം അക്ഷരം* fileName, പൂർണ്ണസംഖ്യ വരി_അക്കം, ശൂന്യം* retAddress, തർക്കം alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_വരി_അക്കം       = വരി_അക്കം;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = അസത്യം;
}
കണ്ണി_Data::കണ്ണി_Data(കണ്ണി_Data &ref)
{
    *ഇത് = ref;
}
കണ്ണി_Data& കണ്ണി_Data::operator=(സ്ഥിരം കണ്ണി_Data &വലത്)
{
    ഇത്->m_funName      = വലത്.m_funName;
    ഇത്->m_instrType    = വലത്.m_instrType;
    ഇത്->m_implType     = വലത്.m_implType;
    ഇത്->m_fileName     = വലത്.m_fileName;
    ഇത്->m_വരി_അക്കം      = വലത്.m_വരി_അക്കം;
    ഇത്->m_retAddress   = വലത്.m_retAddress;
    ഇത്->m_alwaysExpand = വലത്.m_alwaysExpand;

    ഇത്->m_threads     = വലത്.m_threads;
    ഇത്->m_counter     = വലത്.m_counter;
    ഇത്->m_ticksTotal  = വലത്.m_ticksTotal;

    ഇത്->m_funError    = വലത്.m_funError;

    തിരികെ *ഇത്;
}
കണ്ണി_Data::~കണ്ണി_Data()
{
}
തർക്കം operator==(സ്ഥിരം കണ്ണി_Data& ഇടത്, സ്ഥിരം കണ്ണി_Data& വലത്)
{
    എങ്കിൽ(ഇടത്.m_വരി_അക്കം == വലത്.m_വരി_അക്കം && ഇടത്.m_funName == വലത്.m_funName && ഇടത്.m_fileName == വലത്.m_fileName)
    {
        എങ്കിൽ(ഇടത്.m_retAddress == വലത്.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || ഇടത്.m_alwaysExpand))
            തിരികെ സത്യം;
    }
    തിരികെ അസത്യം;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    സ്ഥിതം InstrStruct instr;
    തിരികെ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    തിരികെ *getInstrumentStruct().tlsStruct.get();
}

Instr_കണ്ണി* get_Current_കണ്ണി()
{
    തിരികെ getInstrumentTLSStruct().p_Current_കണ്ണി;
}

IntrumentationRegion::IntrumentationRegion(സ്ഥിരം അക്ഷരം* funName, സ്ഥിരം അക്ഷരം* fileName, പൂർണ്ണസംഖ്യ വരി_അക്കം, ശൂന്യം *retAddress, തർക്കം alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = അസത്യം;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    എങ്കിൽ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        എങ്കിൽ(!pTLS->p_Current_കണ്ണി)
        {
            m_disabled = സത്യം;
            തിരികെ;
        }

        പൂർണ്ണസംഖ്യ depth = pTLS->p_Current_കണ്ണി->getDepth();
        എങ്കിൽ(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = സത്യം;
            തിരികെ;
        }

        കണ്ണി_Data payload(funName, fileName, വരി_അക്കം, retAddress, alwaysExpand, instrType, implType);
        Node<കണ്ണി_Data>* pChild = ശൂന്യ;

        എങ്കിൽ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_കണ്ണി->findChild(payload);
            എങ്കിൽ(!pChild)
            {
                pChild = പുതിയ Node<കണ്ണി_Data>(payload);
                pTLS->p_Current_കണ്ണി->addChild(pChild);
            }
        }
        അല്ലെങ്കിൽ
        {
            pChild = pTLS->p_Current_കണ്ണി->findChild(payload);
            എങ്കിൽ(!pChild)
            {
                m_disabled = സത്യം;
                തിരികെ;
            }
        }
        pTLS->p_Current_കണ്ണി = pChild;

        m_regionTicks = get_Tick_എണ്ണം();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    എങ്കിൽ(pStruct->useInstr)
    {
        എങ്കിൽ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            എങ്കിൽ (pTLS->p_Current_കണ്ണി->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_കണ്ണി->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_കണ്ണി->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_എണ്ണം() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_എണ്ണം); // Concurrent ticks accumulation
                pTLS->p_Current_കണ്ണി->m_payload.m_counter++;
                pTLS->p_Current_കണ്ണി->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_കണ്ണി->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_കണ്ണി = pTLS->p_Current_കണ്ണി->m_pParent;
        }
    }
}
#endif
}

നാമാകാശ ipp
{

#ifdef HAVE_IPP
ഘടന IPPInitSingleton
{
പൊതു:
    IPPInitSingleton()
    {
        useIPP         = സത്യം;

#എങ്കിൽ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#അല്ലെങ്കിൽ
        useIPP_NE      = അസത്യം;
#endif
        ippStatus      = 0;
        funcname       = ശൂന്യ;
        filename       = ശൂന്യ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_മുകൾ_Features = 0;
        pIppLibInfo    = ശൂന്യ;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, ശൂന്യ);
        എങ്കിൽ(ippStatus < 0)
        {
            CV_LOG_ERROR(ശൂന്യ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = അസത്യം;
            തിരികെ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        എങ്കിൽ(!env.empty())
        {
#എങ്കിൽ IPP_VERSION_X100 >= 201900
            സ്ഥിരം Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#അല്ലെങ്കിലെങ്കിൽ IPP_VERSION_X100 >= 201703
            സ്ഥിരം Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#അല്ലെങ്കിലെങ്കിൽ IPP_VERSION_X100 >= 201700
            സ്ഥിരം Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#അല്ലെങ്കിൽ
            സ്ഥിരം Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            എങ്കിൽ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = സത്യം;
                env = env.substr(3, env.size());
            }

            എങ്കിൽ(env == "disabled")
            {
                CV_LOG_WARNING(ശൂന്യ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = അസത്യം;
            }
            അല്ലെങ്കിൽ എങ്കിൽ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            അല്ലെങ്കിൽ എങ്കിൽ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#എങ്കിൽ IPP_VERSION_X100 >= 201700
#എങ്കിൽ defined (_M_AMD64) || defined (__x86_64__)
            അല്ലെങ്കിൽ എങ്കിൽ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            അല്ലെങ്കിൽ
                CV_LOG_ERROR(ശൂന്യ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        എങ്കിൽ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        എങ്കിൽ(!(
#എങ്കിൽ IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = അസത്യം;
            തിരികെ;
        }

        എങ്കിൽ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        അല്ലെങ്കിൽ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#എങ്കിൽ IPP_VERSION_X100 >= 201700
        എങ്കിൽ(ippFeatures&ippCPUID_AVX512F)
        {
            എങ്കിൽ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_മുകൾ_Features = ippCPUID_AVX512_SKX;
            അല്ലെങ്കിൽ എങ്കിൽ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_മുകൾ_Features = ippCPUID_AVX512_KNL;
            അല്ലെങ്കിൽ
                ipp_മുകൾ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        അല്ലെങ്കിൽ
#endif
        എങ്കിൽ(ippFeatures&ippCPUID_AVX2)
            ipp_മുകൾ_Features = ippCPUID_AVX2;
        അല്ലെങ്കിൽ എങ്കിൽ(ippFeatures&ippCPUID_SSE42)
            ipp_മുകൾ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        എങ്കിൽ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_മുകൾ_Features = ippCPUID_SSE42;
        }
    }

പൊതു:
    തർക്കം        useIPP;
    തർക്കം        useIPP_NE;

    പൂർണ്ണസംഖ്യ         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    സ്ഥിരം അക്ഷരം *funcname;
    സ്ഥിരം അക്ഷരം *filename;
    പൂർണ്ണസംഖ്യ         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_മുകൾ_Features;
    സ്ഥിരം IppLibraryVersion *pIppLibInfo;
};

സ്ഥിതം IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, പുതിയ IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    തിരികെ getIPPSingleton().ippFeatures;
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_മുകൾ_Features()
{
    തിരികെ getIPPSingleton().ipp_മുകൾ_Features;
}
#endif

ശൂന്യം setIppStatus(പൂർണ്ണസംഖ്യ status, സ്ഥിരം അക്ഷരം * സ്ഥിരം _funcname, സ്ഥിരം അക്ഷരം * സ്ഥിരം _filename, പൂർണ്ണസംഖ്യ വരി_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = വരി_3;
#അല്ലെങ്കിൽ
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(വരി_3);
#endif
}

പൂർണ്ണസംഖ്യ getIppStatus()
{
#ifdef HAVE_IPP
    തിരികെ getIPPSingleton().ippStatus;
#അല്ലെങ്കിൽ
    തിരികെ 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    തിരികെ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#അല്ലെങ്കിൽ
    തിരികെ String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    സ്ഥിരം IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    എങ്കിൽ(pInfo)
        തിരികെ format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    അല്ലെങ്കിൽ
        തിരികെ String("error");
#അല്ലെങ്കിൽ
    തിരികെ String("disabled");
#endif
}

തർക്കം useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    എങ്കിൽ (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    തിരികെ (data.useIPP > 0);
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം setUseIPP(തർക്കം flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:അസത്യം;
#അല്ലെങ്കിൽ
    CV_UNUSED(flag);
    data.useIPP = അസത്യം;
#endif
}

തർക്കം useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    എങ്കിൽ (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    തിരികെ (data.useIPP_NE > 0);
#അല്ലെങ്കിൽ
    തിരികെ അസത്യം;
#endif
}

ശൂന്യം setUseIPP_NotExact(തർക്കം flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#അല്ലെങ്കിൽ
    CV_UNUSED(flag);
    data.useIPP_NE = അസത്യം;
#endif
}

} // namespace ipp


നാമാകാശ details {

#എങ്കിൽ OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

ശൂന്യം setFPDenormalsIgnoreHint(തർക്കം ignore, CV_OUT FPDenormalsModeState& state)
{
#എങ്കിൽ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    എങ്കിൽ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    സ്ഥിരം unsigned old_flags = _mm_getcsr();
    സ്ഥിരം unsigned old_മൂല്യം = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ശൂന്യ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_മൂല്യം;
    _mm_setcsr(flags);
#അല്ലെങ്കിൽ
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

പൂർണ്ണസംഖ്യ saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#എങ്കിൽ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    എങ്കിൽ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    സ്ഥിരം unsigned old_flags = _mm_getcsr();
    സ്ഥിരം unsigned old_മൂല്യം = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_മൂല്യം;
    തിരികെ 2;
#അല്ലെങ്കിൽ
    CV_UNUSED(state);
    തിരികെ 0;
#endif
}

തർക്കം restoreFPDenormalsState(സ്ഥിരം FPDenormalsModeState& state)
{
#എങ്കിൽ OPENCV_IMPL_FP_HINTS_X86
    സ്ഥിരം unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    സ്ഥിരം unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((പൂർണ്ണസംഖ്യ)value, value == (value & mask), "invalid SSE FP state");
    സ്ഥിരം unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ശൂന്യ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    തിരികെ സത്യം;
#അല്ലെങ്കിൽ
    CV_UNUSED(state);
    തിരികെ അസത്യം;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    തിരികെ OPENCV_ALGO_HINT_DEFAULT;
#അല്ലെങ്കിൽ
    തിരികെ ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
