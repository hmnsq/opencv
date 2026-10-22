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

#অন্তর্ভাৱ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#অন্তর্ভাৱ <atomic>
#অন্তর্ভাৱ <exception>
#অন্তর্ভাৱ <iostream>
#অন্তর্ভাৱ <ostream>

#ifdef __QNX__
    #অন্তর্ভাৱ <unistd.h>
    #অন্তর্ভাৱ <sys/neutrino.h>
    #অন্তর্ভাৱ <sys/syspage.h>
#ifdef __aarch64__
    #অন্তর্ভাৱ <aarch64/syspage.h>
#endif
#endif

#অন্তর্ভাৱ <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/trace.private.hpp>

#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>

#অন্তর্ভাৱ <opencv2/core/utils/tls.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/instrumentation.hpp>

#অন্তর্ভাৱ <opencv2/core/utils/filesystem.private.hpp>

#অন্তর্ভাৱ <opencv2/core/utils/fp_control_utils.hpp>
#অন্তর্ভাৱ <opencv2/core/utils/fp_control.private.hpp>

নামাকাশ cv {

স্থির শূন্য _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    স্থির std::ios_base::Init s_iostream_initializer;
#endif
}

স্থির Mutex* __initialization_mutex = রিক্ত;
Mutex& getInitializationMutex()
{
    যদি (__initialization_mutex == রিক্ত)
    {
        (শূন্য)_initSystem();
        __initialization_mutex = নূতন Mutex();
    }
    ঘূৰাই *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

স্থির তার্কিক param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#যদি defined(_DEBUG) || defined(__ANDROID__)
    সঁচা
#অন্যথা
    মিছা
#endif
);

শূন্য* allocSingletonBuffer(size_t size) { ঘূৰাই fastMalloc(size); }
শূন্য* allocSingletonNewBuffer(size_t size) { ঘূৰাই malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# যদি defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#যদি defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# যদি _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#অন্তর্ভাৱ <cstdlib>        // std::abort
#endif

#যদি defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#যদি defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ধ্রুৱ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#অন্যথা
#  include <elf.h>
#endif
#যদি defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#যদি defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#যদি ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#যদি defined __loongarch64
#অন্তর্ভাৱ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#যদি defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#অন্তর্ভাৱ <windows.h>
#অন্তর্ভাৱ <combaseapi.h>
#যদি (_WIN32_WINNT >= 0x0602)
  #অন্তর্ভাৱ <synchapi.h>
#endif
#যদি ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #অন্তর্ভাৱ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#অন্তর্ভাৱ <tchar.h>

#ifdef WINRT
#অন্তর্ভাৱ <wrl/client.h>
#ifndef __cplusplus_winrt
#অন্তর্ভাৱ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    ঘূৰাই std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#অন্যথা
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    যদি (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        ঘূৰাই wstr;
    যদি (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        ঘূৰাই wstr;
    যদি (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        ঘূৰাই wstr;
    যদি (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        ঘূৰাই wstr;
    যদি (FAILED(storagefolderRef.As(&storageitemRef)))
        ঘূৰাই wstr;
    str = রিক্ত;
    যদি (FAILED(storageitemRef->get_Path(&str)))
        ঘূৰাই wstr;
    wstr = WindowsGetStringRawBuffer(str, রিক্ত);
    WindowsDeleteString(str);
    ঘূৰাই wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], পরিমাণ(guidStr)/পরিমাণ(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ঘূৰাই prefix.append(std::wstring(guidStr));
}

#endif
#অন্যথা
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#অন্তর্ভাৱ <pthread.h>
#endif
#অন্তর্ভাৱ <sys/time.h>
#অন্তর্ভাৱ <time.h>

#যদি defined __MACH__ && defined __APPLE__
#অন্তর্ভাৱ <mach/mach.h>
#অন্তর্ভাৱ <mach/mach_time.h>
#অন্তর্ভাৱ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#অন্তর্ভাৱ "omp.h"
#endif

#যদি defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <sys/types.h>
#যদি defined __ANDROID__
#অন্তর্ভাৱ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #যদি defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #যদি _MSC_VER >= 1400  // MSVS 2005
      #অন্তর্ভাৱ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #অন্যথা
      #error "Required MSVS 2005+"
    #endif
  #নোচেদ্যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    স্থির শূন্য cv_cpuid(পূর্ণাঙ্ক* cpuid_data, পূর্ণাঙ্ক reg_eax, পূর্ণাঙ্ক reg_ecx)
    {
        পূর্ণাঙ্ক __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#যদি !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#নোচেদ্যদি defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#অন্যথা
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#অন্তর্ভাৱ <chrono>

নামাকাশ cv
{

Exception::Exception() { code = Error::StsOk; সাৰি = 0; }

Exception::Exception(Error::Code _code, ধ্রুৱ String& _err, ধ্রুৱ String& _func, ধ্রুৱ String& _file, পূর্ণাঙ্ক সাৰি_3)
: code(_code), err(_err), func(_func), file(_file), সাৰি(সাৰি_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ধ্রুৱ ৱর্ণ* Exception::what() ধ্রুৱ CV_NOEXCEPT { ঘূৰাই বাৰ্তা.c_str(); }

শূন্য Exception::formatMessage()
{
    size_t pos = err.find('\n');
    তার্কিক multiline = pos != cv::String::npos;
    যদি (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        যেতিয়ালৈ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        যদি (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    যদি (func.size() > 0)
    {
        যদি (multiline)
            বাৰ্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), সাৰি, code, codeMessage(), func.c_str(), err.c_str());
        অন্যথা
            বাৰ্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), সাৰি, code, codeMessage(), err.c_str(), func.c_str());
    }
    অন্যথা
    {
        বাৰ্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), সাৰি, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ধ্রুৱ ৱর্ণ * Exception::codeMessage() ধ্রুৱ
{
    ৱিকল্প (code)
    {
    পক্ষ cv::Error::StsOk :                  ঘূৰাই "No Error";
    পক্ষ cv::Error::StsBackTrace :           ঘূৰাই "Backtrace";
    পক্ষ cv::Error::StsError :               ঘূৰাই "Unspecified error";
    পক্ষ cv::Error::StsInternal :            ঘূৰাই "Internal error";
    পক্ষ cv::Error::StsNoMem :               ঘূৰাই "Insufficient memory";
    পক্ষ cv::Error::StsBadArg :              ঘূৰাই "Bad argument";
    পক্ষ cv::Error::StsNoConv :              ঘূৰাই "Iterations do not converge";
    পক্ষ cv::Error::StsAutoTrace :           ঘূৰাই "Autotrace call";
    পক্ষ cv::Error::StsBadSize :             ঘূৰাই "Incorrect size of input array";
    পক্ষ cv::Error::StsNullPtr :             ঘূৰাই "Null pointer";
    পক্ষ cv::Error::StsDivByZero :           ঘূৰাই "Division by zero occurred";
    পক্ষ cv::Error::BadStep :                ঘূৰাই "Image step is wrong";
    পক্ষ cv::Error::StsInplaceNotSupported : ঘূৰাই "Inplace operation is not supported";
    পক্ষ cv::Error::StsObjectNotFound :      ঘূৰাই "Requested object was not found";
    পক্ষ cv::Error::BadDepth :               ঘূৰাই "Input image depth is not supported by function";
    পক্ষ cv::Error::StsUnmatchedFormats :    ঘূৰাই "Formats of input arguments do not match";
    পক্ষ cv::Error::StsUnmatchedSizes :      ঘূৰাই "Sizes of input arguments do not match";
    পক্ষ cv::Error::StsOutOfRange :          ঘূৰাই "One of the arguments\' values is out of range";
    পক্ষ cv::Error::StsUnsupportedFormat :   ঘূৰাই "Unsupported format or combination of formats";
    পক্ষ cv::Error::BadCOI :                 ঘূৰাই "Input COI is not supported";
    পক্ষ cv::Error::Bad_অঙ্ক_Channels :         ঘূৰাই "Bad number of channels";
    পক্ষ cv::Error::StsBadFlag :             ঘূৰাই "Bad flag (parameter or structure field)";
    পক্ষ cv::Error::StsBadPoint :            ঘূৰাই "Bad parameter of type CvPoint";
    পক্ষ cv::Error::StsBadMask :             ঘূৰাই "Bad type of mask argument";
    পক্ষ cv::Error::StsParseError :          ঘূৰাই "Parsing error";
    পক্ষ cv::Error::StsNotImplemented :      ঘূৰাই "The function/feature is not implemented";
    পক্ষ cv::Error::StsBadMemBlock :         ঘূৰাই "Memory block has been corrupted";
    পক্ষ cv::Error::StsAssert :              ঘূৰাই "Assertion failed";
    পক্ষ cv::Error::GpuNotSupported :        ঘূৰাই "No CUDA support";
    পক্ষ cv::Error::GpuApiCallError :        ঘূৰাই "Gpu API call";
    পক্ষ cv::Error::OpenGlNotSupported :     ঘূৰাই "No OpenGL support";
    পক্ষ cv::Error::OpenGlApiCallError :     ঘূৰাই "OpenGL API call";
    পক্ষ cv::Error::OpenCLApiCallError :     ঘূৰাই "OpenCL API call error";
    পক্ষ cv::Error::OpenCLDoubleNotSupported:ঘূৰাই "OpenCL device does not support double";
    পক্ষ cv::Error::OpenCLInitError :        ঘূৰাই "OpenCL initialization error";
    পক্ষ cv::Error::OpenCLNoAMDBlasFft :     ঘূৰাই "Missing OpenCL AMD BLAS FFT";
    স্ৱভাৱ:
    {
        স্থির ৱর্ণ buf[256] {0};
        snprintf(buf, পরিমাণ(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        ঘূৰাই buf;
    }
    };

}

স্থির ধ্রুৱ ৱর্ণ* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { রিক্ত };

স্থির ধ্রুৱ ৱর্ণ* getHWFeatureName(পূর্ণাঙ্ক id)
{
    ঘূৰাই (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : রিক্ত;
}
স্থির ধ্রুৱ ৱর্ণ* getHWFeatureNameSafe(পূর্ণাঙ্ক id)
{
    ধ্রুৱ ৱর্ণ* name = getHWFeatureName(id);
    ঘূৰাই name ? name : "Unknown feature";
}

সংরচনা HWFeatures
{
    গণনা { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(তার্কিক run_initialize = মিছা)
    {
        যদি (run_initialize)
            initialize();
    }

    স্থির শূন্য initializeNames()
    {
        বাবে (পূর্ণাঙ্ক i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    শূন্য initialize(শূন্য)
    {
        যদি (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        পূর্ণাঙ্ক cpuid_data[4] = { 0, 0, 0, 0 };
        পূর্ণাঙ্ক cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        পূর্ণাঙ্ক x86_family = (cpuid_data[0] >> 8) & 15;
        যদি( x86_family >= 6 )
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
            পূর্ণাঙ্ক cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            তার্কিক have_AVX_OS_support = সঁচা;
            তার্কিক have_AVX512_OS_support = সঁচা;
            যদি (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = মিছা; // OS uses XSAVE_XRSTORE and CPU support AVX
            অন্যথা
            {
                পূর্ণাঙ্ক xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (পূর্ণাঙ্ক)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #নোচেদ্যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                যদি ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = মিছা; // YMM registers
                যদি ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = মিছা; // ZMM registers
            }

            যদি (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = মিছা;
                have[CV_CPU_FP16] = মিছা;
                have[CV_CPU_AVX2] = মিছা;
                have[CV_CPU_FMA3] = মিছা;
                have[CV_CPU_AVX_VNNI] = মিছা;
            }
            যদি (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = মিছা;
                have[CV_CPU_AVX_512BW] = মিছা;
                have[CV_CPU_AVX_512CD] = মিছা;
                have[CV_CPU_AVX_512DQ] = মিছা;
                have[CV_CPU_AVX_512ER] = মিছা;
                have[CV_CPU_AVX_512IFMA] = মিছা;
                have[CV_CPU_AVX_512PF] = মিছা;
                have[CV_CPU_AVX_512VBMI] = মিছা;
                have[CV_CPU_AVX_512VL] = মিছা;
                have[CV_CPU_AVX_512VBMI2] = মিছা;
                have[CV_CPU_AVX_512VNNI] = মিছা;
                have[CV_CPU_AVX_512BITALG] = মিছা;
                have[CV_CPU_AVX_512VPOPCNTDQ] = মিছা;
                have[CV_CPU_AVX_5124VNNIW] = মিছা;
                have[CV_CPU_AVX_5124FMAPS] = মিছা;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            যদি (have[CV_CPU_AVX512_COMMON])
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
            অন্যথা
            {
                have[CV_CPU_AVX512_KNL] = মিছা;
                have[CV_CPU_AVX512_KNM] = মিছা;
                have[CV_CPU_AVX512_SKX] = মিছা;
                have[CV_CPU_AVX512_CNL] = মিছা;
                have[CV_CPU_AVX512_CLX] = মিছা;
                have[CV_CPU_AVX512_ICL] = মিছা;
            }
        }
    #endif // CV_CPUID_X86

    #যদি defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = সঁচা;
        have[CV_CPU_FP16] = সঁচা;
        পূর্ণাঙ্ক cpufile = open("/proc/self/auxv", O_RDONLY);

        যদি (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ধ্রুৱ size_t size_auxv_t = পরিমাণ(auxv);

            যেতিয়ালৈ ((size_t)পঠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                যদি (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#যদি defined(AT_HWCAP2)
                অন্যথা যদি (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #নোচেদ্যদি defined __arm__ && defined __ANDROID__
      #যদি defined HAVE_CPUFEATURES
        CV_LOG_INFO(রিক্ত, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(রিক্ত, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #অন্যথা
        CV_LOG_INFO(রিক্ত, "cpufeatures library is not available for CPU detection");
        #যদি CV_NEON
        CV_LOG_INFO(রিক্ত, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = সঁচা;
        #অন্যথা
        CV_LOG_INFO(রিক্ত, "- NEON instructions is NOT enabled via build flags");
        #endif
        #যদি CV_FP16
        CV_LOG_INFO(রিক্ত, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = সঁচা;
        #অন্যথা
        CV_LOG_INFO(রিক্ত, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #নোচেদ্যদি defined __arm__
        পূর্ণাঙ্ক cpufile = open("/proc/self/auxv", O_RDONLY);

        যদি (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ধ্রুৱ size_t size_auxv_t = পরিমাণ(auxv);

            যেতিয়ালৈ ((size_t)পঠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                যদি (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ভঙ্গ;
                }
            }

            close(cpufile);
        }
    #endif
    #নোচেদ্যদি (defined __APPLE__)
    #যদি defined __ARM_NEON
        have[CV_CPU_NEON] = সঁচা;
    #endif
    #যদি (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = সঁচা;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    পূর্ণাঙ্ক has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = পরিমাণ(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, রিক্ত, 0);
    যদি (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = সঁচা;
    }
    পূর্ণাঙ্ক has_feat_bf16 = 0;
    size_t has_feat_bf16_size = পরিমাণ(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, রিক্ত, 0);
    যদি (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = সঁচা;
    }
    #নোচেদ্যদি (defined __clang__)
    #যদি defined __ARM_NEON
        have[CV_CPU_NEON] = সঁচা;
        #যদি (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = সঁচা;
        #endif
    #endif
    #endif
    #যদি defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = সঁচা;
    #endif
    #যদি defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = সঁচা;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = সঁচা;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = সঁচা;
    #endif

    #যদি (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned পূর্ণাঙ্ক hwcap = getauxval(AT_HWCAP);
        যদি (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            যদি (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = সঁচা;
            } অন্যথা {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #নোচেদ্যদি (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        যদি (elf_aux_info(AT_HWCAP, &hwcap, পরিমাণ(hwcap)) != 0)
            hwcap = 0;
        যদি (hwcap & PPC_FEATURE_HAS_VSX) {
            যদি (elf_aux_info(AT_HWCAP2, &hwcap2, পরিমাণ(hwcap2)) != 0)
                hwcap2 = 0;
            যদি (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = সঁচা;
            } অন্যথা {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #অন্যথা
        // TODO: AIX
        #যদি CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = সঁচা;
        #endif
        #যদি CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = সঁচা;
        #endif
    #endif

    #যদি defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = সঁচা;
        #যদি (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = সঁচা;
        #endif
        #যদি defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = সঁচা;
        #endif
    #endif

    #যদি defined __loongarch64 && defined __linux__
        পূর্ণাঙ্ক flag = (পূর্ণাঙ্ক)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        তার্কিক skip_baseline_check = মিছা;
        যদি (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = সঁচা;
        }
        পূর্ণাঙ্ক baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        যদি (!checkFeatures(baseline_features, পরিমাণ(baseline_features) / পরিমাণ(baseline_features[0]))
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
            checkFeatures(baseline_features, পরিমাণ(baseline_features) / পরিমাণ(baseline_features[0]), সঁচা);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, পরিমাণ(baseline_features) / পরিমাণ(baseline_features[0]));
    }

    তার্কিক checkFeatures(ধ্রুৱ পূর্ণাঙ্ক* features, পূর্ণাঙ্ক গুনতি_2, তার্কিক dump = মিছা)
    {
        তার্কিক result = সঁচা;
        বাবে (পূর্ণাঙ্ক i = 0; i < গুনতি_2; i++)
        {
            পূর্ণাঙ্ক feature = features[i];
            যদি (feature)
            {
                যদি (have[feature])
                {
                    যদি (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                অন্যথা
                {
                    result = মিছা;
                    যদি (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        ঘূৰাই result;
    }

    স্থির অন্তঃস্থ তার্কিক is_প্ৰতীক_Separator(ৱর্ণ c)
    {
        ঘূৰাই c == ',' || c == ';';
    }

    শূন্য readSettings(ধ্রুৱ পূর্ণাঙ্ক* baseline_features, পূর্ণাঙ্ক baseline_গুনতি)
    {
        তার্কিক dump = সঁচা;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        যদি (!disabled_features.empty())
        {
            ধ্রুৱ ৱর্ণ* start = disabled_features.c_str();
            বাবে (;;)
            {
                যেতিয়ালৈ (start[0] != 0 && is_প্ৰতীক_Separator(start[0]))
                {
                    start++;
                }
                যদি (start[0] == 0)
                    ভঙ্গ;
                ধ্রুৱ ৱর্ণ* end = start;
                যেতিয়ালৈ (end[0] != 0 && !is_প্ৰতীক_Separator(end[0]))
                {
                    end++;
                }
                যদি (end == start)
                    অনুৱর্ত;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                তার্কিক found = মিছা;
                বাবে (পূর্ণাঙ্ক i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    যদি (!g_hwFeatureNames[i]) অনুৱর্ত;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    যদি (len != feature.size()) অনুৱর্ত;
                    যদি (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        তার্কিক isBaseline = মিছা;
                        বাবে (পূর্ণাঙ্ক k = 0; k < baseline_গুনতি; k++)
                        {
                            যদি (baseline_features[k] == i)
                            {
                                isBaseline = সঁচা;
                                ভঙ্গ;
                            }
                        }
                        যদি (isBaseline)
                        {
                            যদি (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        যদি (!have[i])
                        {
                            যদি (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = মিছা;

                        found = সঁচা;
                        ভঙ্গ;
                    }
                }
                যদি (!found)
                {
                    যদি (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    তার্কিক have[MAX_FEATURE+1]{};
};

স্থির HWFeatures  featuresEnabled(সঁচা), featuresDisabled = HWFeatures(মিছা);
স্থির HWFeatures* currentFeatures = &featuresEnabled;

তার্কিক checkHardwareSupport(পূর্ণাঙ্ক feature)
{
    যদি (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ঘূৰাই মিছা;
    ঘূৰাই currentFeatures->have[feature];
}

String getHardwareFeatureName(পূর্ণাঙ্ক feature)
{
    ধ্রুৱ ৱর্ণ* name = getHWFeatureName(feature);
    ঘূৰাই name ? String(name) : String();
}

std::string get_CPUFeatures_সাৰি()
{
    ধ্রুৱ পূর্ণাঙ্ক features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ধ্রুৱ পূর্ণাঙ্ক sz = পরিমাণ(features) / পরিমাণ(features[0]);
    std::string result;
    std::string prefix;
    বাবে (পূর্ণাঙ্ক i = 1; i < sz; ++i)
    {
        যদি (features[i] == 0)
        {
            prefix = "*";
            অনুৱর্ত;
        }
        যদি (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        যদি (!checkHardwareSupport(features[i])) result.append("?");
    }
    ঘূৰাই result;
}

চঞ্চল তার্কিক useOptimizedFlag = সঁচা;

শূন্য setUseOptimized( তার্কিক flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

তার্কিক useOptimized(শূন্য)
{
    ঘূৰাই useOptimizedFlag;
}

int64 get_Tick_গুনতি(শূন্য)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ঘূৰাই (int64)now.time_since_epoch().গুনতি_2();
}

দ্ৱিগুণ getTickFrequency(শূন্য)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    দ্ৱিগুণ clock_freq = clock_period_t::den / clock_period_t::num;
    ঘূৰাই clock_freq;
}

#যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#যদি defined(__i386__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    int64 x;
    __asm__ চঞ্চল (".byte 0x0f, 0x31" : "=A" (x));
    ঘূৰাই x;
}
#নোচেদ্যদি defined(__x86_64__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ঘূৰাই (int64)lo | ((int64)hi << 32);
}

#নোচেদ্যদি defined(__ppc__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    unsigned upper, lower, tmp;
    __asm__ চঞ্চল(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ঘূৰাই lower | ((int64)upper << 32);
}

#অন্যথা

#error "RDTSC not defined"

#endif

#নোচেদ্যদি defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_গুনতি(শূন্য)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#অন্যথা

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_গুনতি(শূন্য)
{
    ঘূৰাই get_Tick_গুনতি();
}
//#endif

#endif


নামাকাশ internal {

শ্ৰেণী Timestamp
{
সার্ৱজনিক:
    ধ্রুৱ int64 zero_Tick_গুনতি;
    ধ্রুৱ দ্ৱিগুণ ns_in_ticks;

    Timestamp()
        : zero_Tick_গুনতি(get_Tick_গুনতি())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_গুনতি();
        ঘূৰাই (int64)((t - zero_Tick_গুনতি) * ns_in_ticks);
    }

    স্থির Timestamp& getInstance()
    {
        স্থির Timestamp g_timestamp;
        ঘূৰাই g_timestamp;
    }
};

শ্ৰেণী InitTimestamp {
সার্ৱজনিক:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
স্থির InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ঘূৰাই internal::Timestamp::getInstance().getTimestamp();
}


ধ্রুৱ String& getBuildInformation()
{
    স্থির String build_info =
#অন্তর্ভাৱ "version_string.inc"
    ;
    ঘূৰাই build_info;
}

String getVersionString() { ঘূৰাই String(CV_VERSION); }

পূর্ণাঙ্ক getVersionMajor() { ঘূৰাই CV_VERSION_MAJOR; }

পূর্ণাঙ্ক getVersionMinor() { ঘূৰাই CV_VERSION_MINOR; }

পূর্ণাঙ্ক getVersionRevision() { ঘূৰাই CV_VERSION_REVISION; }

String format( ধ্রুৱ ৱর্ণ* fmt, ... )
{
    AutoBuffer<ৱর্ণ, 1024> buf;

    বাবে ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        পূর্ণাঙ্ক bsize = static_cast<পূর্ণাঙ্ক>(buf.size());
        পূর্ণাঙ্ক len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        যদি (len >= bsize)
        {
            buf.resize(len + 1);
            অনুৱর্ত;
        }
        buf[bsize - 1] = 0;
        ঘূৰাই String(buf.data(), len);
    }
}

String tempfile( ধ্রুৱ ৱর্ণ* suffix )
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#যদি defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    যদি (temp_file.empty())
        ঘূৰাই String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    ৱর্ণ aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#নোচেদ্যদি defined(_WIN32_WCE)
    ধ্রুৱ auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    যদি(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        ৱর্ণ aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#অন্যথা
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    যদি (temp_dir.empty())
    {
        ::GetTempPathW(পরিমাণ(temp_dir2_w)/পরিমাণ(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        পূর্ণাঙ্ক len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, রিক্ত, 0, রিক্ত, রিক্ত);
        যদি (len > 0)
        {
            std::vector<ৱর্ণ> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, রিক্ত, রিক্ত);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    যদি (FAILED(hr))
        ঘূৰাই String();
    ৱর্ণ guidStr[40];
    ধ্রুৱ ৱর্ণ* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, পরিমাণ(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned পূর্ণাঙ্ক)g.Data4[0], (unsigned পূর্ণাঙ্ক)g.Data4[1],
            (unsigned পূর্ণাঙ্ক)g.Data4[2], (unsigned পূর্ণাঙ্ক)g.Data4[3], (unsigned পূর্ণাঙ্ক)g.Data4[4],
            (unsigned পূর্ণাঙ্ক)g.Data4[5], (unsigned পূর্ণাঙ্ক)g.Data4[6], (unsigned পূর্ণাঙ্ক)g.Data4[7]);

    fname = temp_dir;
    যদি (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# অন্যথা
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    ৱর্ণ defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  অন্যথা
    ৱর্ণ defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    যদি (temp_dir.empty())
        fname = defaultTemplate;
    অন্যথা
    {
        fname = temp_dir;
        ৱর্ণ ech = fname[fname.size() - 1];
        যদি(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ধ্রুৱ পূর্ণাঙ্ক fd = mkstemp((ৱর্ণ*)fname.c_str());
    যদি (fd == -1) ঘূৰাই String();

    close(fd);
    remove(fname.c_str());
# endif

    যদি (suffix)
    {
        যদি (suffix[0] != '.')
            ঘূৰাই fname + "." + suffix;
        অন্যথা
            ঘূৰাই fname + suffix;
    }
    ঘূৰাই fname;
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

স্থির ErrorCallback customErrorCallback = 0;
স্থির শূন্য* customErrorCallbackData = 0;
স্থির তার্কিক breakOnError = মিছা;

তার্কিক setBreakOnError(তার্কিক value)
{
    তার্কিক prevVal = breakOnError;
    breakOnError = value;
    ঘূৰাই prevVal;
}

পূর্ণাঙ্ক cv_snprintf(ৱর্ণ* buf, পূর্ণাঙ্ক len, ধ্রুৱ ৱর্ণ* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    পূর্ণাঙ্ক res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ঘূৰাই res;
}

পূর্ণাঙ্ক cv_vsnprintf(ৱর্ণ* buf, পূর্ণাঙ্ক len, ধ্রুৱ ৱর্ণ* fmt, va_list args)
{
#যদি defined _MSC_VER
    যদি (len <= 0) ঘূৰাই len == 0 ? 1024 : -1;
    পূর্ণাঙ্ক res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    যদি (res >= 0 && res < len)
    {
        buf[res] = 0;
        ঘূৰাই res;
    }
    অন্যথা
    {
        buf[len - 1] = 0; // truncate happened
        ঘূৰাই res >= len ? res : (len * 2);
    }
#অন্যথা
    ঘূৰাই vsnprintf(buf, len, fmt, args);
#endif
}

স্থির শূন্য dumpException(ধ্রুৱ Exception& exc)
{
    ধ্রুৱ ৱর্ণ* errorStr = exc.codeMessage();
    ৱর্ণ buf[1 << 12];

    cv_snprintf(buf, পরিমাণ(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.সাৰি);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#অন্যথা
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
স্থির তার্কিক cv_terminate_handler_installed = মিছা;
স্থির std::terminate_handler cv_old_terminate_handler;
স্থির cv::Exception cv_terminate_handler_exception;
স্থির তার্কিক param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", সঁচা);
স্থির শূন্য cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    যদি (মিছা /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# যদি defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

শূন্য error( ধ্রুৱ Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        যদি (!cv_terminate_handler_installed)
        {
            যদি (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = সঁচা;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    যদি (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.সাৰি, customErrorCallbackData);
    অন্যথা যদি (param_dumpErrors)
    {
        dumpException(exc);
    }

    যদি(breakOnError)
    {
        std::terminate();
    }

    ক্ষিপ exc;
#ifdef __GNUC__
# যদি !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

শূন্য error(Error::Code _code, ধ্রুৱ String& _err, ধ্রুৱ ৱর্ণ* _func, ধ্রুৱ ৱর্ণ* _file, পূর্ণাঙ্ক সাৰি_3)
{
    error(cv::Exception(_code, _err, _func, _file, সাৰি_3));
#ifdef __GNUC__
# যদি !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# যদি defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, শূন্য* userdata, শূন্য** prevUserdata)
{
    যদি( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    ঘূৰাই prevCallback;
}

শূন্য terminate(Error::Code _code, ধ্রুৱ String& _err, ধ্রুৱ ৱর্ণ* _func, ধ্রুৱ ৱর্ণ* _file, পূর্ণাঙ্ক সাৰি_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, সাৰি_3));
    std::terminate();
}

}

নামাকাশ cv {
তার্কিক __termination = মিছা;


//////////////////////////////// thread-local storage ////////////////////////////////

নামাকাশ details {

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
শ্ৰেণী TlsAbstraction
{
সার্ৱজনিক:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    শূন্য* getData() ধ্রুৱ;
    শূন্য setData(শূন্য *pData);

    শূন্য releaseSystemResources();

গুপ্ত:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_চাবি;
    তার্কিক disposed;
#endif
#অন্যথা // _WIN32
    pthread_key_t  tls_চাবি;
    std::atomic<তার্কিক> disposed;
#endif
};

শ্ৰেণী TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
সার্ৱজনিক:
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
স্থির TlsAbstraction* getTlsAbstraction()
{
    স্থির TlsAbstraction *g_tls = নূতন TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    স্থির TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ঘূৰাই g_tls;
}


#ifdef _WIN32
#ifdef WINRT
স্থির __declspec( thread ) শূন্য* tlsData = রিক্ত; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
শূন্য TlsAbstraction::releaseSystemResources()
{
    cv::__termination = সঁচা;  // DllMain is missing in static builds
}
শূন্য* TlsAbstraction::getData() ধ্রুৱ
{
    ঘূৰাই tlsData;
}
শূন্য TlsAbstraction::setData(শূন্য *pData)
{
    tlsData = pData;
}
#অন্যথা //WINRT
#ifdef CV_USE_FLS
স্থির শূন্য NTAPI opencv_fls_destructor(শূন্য* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(মিছা)
{
#ifndef CV_USE_FLS
    tls_চাবি = TlsAlloc();
#অন্যথা // CV_USE_FLS
    tls_চাবি = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_চাবি != TLS_OUT_OF_INDEXES);
}
শূন্য TlsAbstraction::releaseSystemResources()
{
    cv::__termination = সঁচা;  // DllMain is missing in static builds
    disposed = সঁচা;
#ifndef CV_USE_FLS
    TlsFree(tls_চাবি);
#অন্যথা // CV_USE_FLS
    FlsFree(tls_চাবি);
#endif // CV_USE_FLS
    tls_চাবি = TLS_OUT_OF_INDEXES;
}
শূন্য* TlsAbstraction::getData() ধ্রুৱ
{
    যদি (disposed)
        ঘূৰাই রিক্ত;
#ifndef CV_USE_FLS
    ঘূৰাই Tls_Get_মান(tls_চাবি);
#অন্যথা // CV_USE_FLS
    ঘূৰাই Fls_Get_মান(tls_চাবি);
#endif // CV_USE_FLS
}
শূন্য TlsAbstraction::setData(শূন্য *pData)
{
    যদি (disposed)
        ঘূৰাই;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_মান(tls_চাবি, pData) == TRUE);
#অন্যথা // CV_USE_FLS
    CV_Assert(Fls_Set_মান(tls_চাবি, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#অন্যথা // _WIN32
স্থির শূন্য opencv_tls_destructor(শূন্য* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(মিছা)
{
    CV_Assert(pthread_key_create(&tls_চাবি, opencv_tls_destructor) == 0);
}
শূন্য TlsAbstraction::releaseSystemResources()
{
    cv::__termination = সঁচা;  // DllMain is missing in static builds
    disposed = সঁচা;
    যদি (pthread_key_delete(tls_চাবি) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
শূন্য* TlsAbstraction::getData() ধ্রুৱ
{
    যদি (disposed)
        ঘূৰাই রিক্ত;
    ঘূৰাই pthread_getspecific(tls_চাবি);
}
শূন্য TlsAbstraction::setData(শূন্য *pData)
{
    যদি (disposed)
        ঘূৰাই;  // no-op
    CV_Assert(pthread_setspecific(tls_চাবি, pData) == 0);
}
#endif

// Per-thread data structure
সংরচনা ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<শূন্য*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


স্থির তার্কিক g_isTlsStorageInitialized = মিছা;

// Main TLS storage class
শ্ৰেণী TlsStorage
{
সার্ৱজনিক:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (শূন্য)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = সঁচা;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    শূন্য releaseThread(শূন্য* tls_মান = রিক্ত)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        যদি (রিক্ত == tls)
            ঘূৰাই;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_মান == রিক্ত ? (ThreadData*)tls->getData() : (ThreadData*)tls_মান;
        যদি (pTD == রিক্ত)
            ঘূৰাই;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        বাবে (size_t i = 0; i < threads.size(); i++)
        {
            যদি (pTD == threads[i])
            {
                threads[i] = রিক্ত;
                যদি (tls_মান == রিক্ত)
                    tls->setData(0);
                std::vector<শূন্য*>& thread_slots = pTD->slots;
                বাবে (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    শূন্য* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = রিক্ত;
                    যদি (!pData)
                        অনুৱর্ত;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    যদি (container)
                        container->deleteDataInstance(pData);
                    অন্যথা
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (পূর্ণাঙ্ক)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                ঘূৰাই;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (শূন্য*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        বাবে(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            যদি (tlsSlots[slot].container == রিক্ত)
            {
                tlsSlots[slot].container = container;
                ঘূৰাই slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        ঘূৰাই tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    শূন্য releaseSlot(size_t slotIdx, std::vector<শূন্য*> &dataVec, তার্কিক keepSlot = মিছা)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        বাবে(size_t i = 0; i < threads.size(); i++)
        {
            যদি(threads[i])
            {
                std::vector<শূন্য*>& thread_slots = threads[i]->slots;
                যদি (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = রিক্ত;
                }
            }
        }

        যদি (!keepSlot)
        {
            tlsSlots[slotIdx].container = রিক্ত;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    শূন্য* getData(size_t slotIdx) ধ্রুৱ
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        যদি (রিক্ত == tls)
            ঘূৰাই রিক্ত;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        যদি(threadData && threadData->slots.size() > slotIdx)
            ঘূৰাই threadData->slots[slotIdx];

        ঘূৰাই রিক্ত;
    }

    // Gather data from threads by TLS storage index
    শূন্য gather(size_t slotIdx, std::vector<শূন্য*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        বাবে(size_t i = 0; i < threads.size(); i++)
        {
            যদি(threads[i])
            {
                std::vector<শূন্য*>& thread_slots = threads[i]->slots;
                যদি (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    শূন্য setData(size_t slotIdx, শূন্য* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        যদি (রিক্ত == tls)
            ঘূৰাই;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        যদি(!threadData)
        {
            threadData = নূতন ThreadData;
            tls->setData((শূন্য*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                তার্কিক found = মিছা;
                // Find unused slots
                বাবে(size_t slot = 0; slot < threads.size(); slot++)
                {
                    যদি (threads[slot] == রিক্ত)
                    {
                        threadData->idx = (পূর্ণাঙ্ক)slot;
                        threads[slot] = threadData;
                        found = সঁচা;
                        ভঙ্গ;
                    }
                }

                যদি (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        যদি(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, রিক্ত);
        }
        threadData->slots[slotIdx] = pData;
    }

গুপ্ত:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    সংরচনা TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<সংরচনা TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
স্থির TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, নূতন TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
স্থির শূন্য opencv_tls_destructor(শূন্য* pData)
{
    যদি (!g_isTlsStorageInitialized)
        ঘূৰাই;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#অন্যথা // _WIN32
#ifdef CV_USE_FLS
স্থির শূন্য WINAPI opencv_fls_destructor(শূন্য* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    যদি (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = সঁচা;
    }

    যদি (!g_isTlsStorageInitialized)
        ঘূৰাই;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

স্থির TlsStorage* ধ্রুৱ g_force_initialization_of_TlsStorage
#যদি defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#অন্যথা  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
শ্ৰেণী TlsStorage
{
সার্ৱজনিক:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        বাবে (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            যদি (container && s.data)
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
        বাবে (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            যদি (s.container == রিক্ত)
            {
                CV_Assert(!s.data);
                s.container = container;
                ঘূৰাই slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        ঘূৰাই slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    শূন্য releaseSlot(size_t slotIdx, std::vector<শূন্য*> &dataVec, তার্কিক keepSlot = মিছা)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        শূন্য* data = s.data;
        যদি (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        যদি (!keepSlot)
        {
            s.container = রিক্ত;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    শূন্য* getData(size_t slotIdx) ধ্রুৱ
    {
        CV_Assert(slotIdx < slots.size());
        ধ্রুৱ SlotInfo& s = slots[slotIdx];
        ঘূৰাই s.data;
    }

    // Gather data from threads by TLS storage index
    শূন্য gather(size_t slotIdx, std::vector<শূন্য*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        শূন্য* data = s.data;
        যদি (data)
            dataVec.push_back(data);
        ঘূৰাই;
    }

    // Set data to storage index
    শূন্য setData(size_t slotIdx, শূন্য* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

গুপ্ত:
    সংরচনা SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        শূন্য* data;
    };
    std::vector<সংরচনা SlotInfo> slots;
};

স্থির TlsStorage& getTlsStorage()
{
    স্থির TlsStorage g_storage;  // no threading
    ঘূৰাই g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using নামাকাশ details;

শূন্য releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    যদি (!g_isTlsStorageInitialized)
        ঘূৰাই;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    চাবি_2 = (পূর্ণাঙ্ক)getTlsStorage().reserveSlot(এতত্); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(চাবি_2 == -1); // Key must be released in child object
}

শূন্য TLSDataContainer::gatherData(std::vector<শূন্য*> &data) ধ্রুৱ
{
    getTlsStorage().gather(চাবি_2, data);
}

শূন্য TLSDataContainer::detachData(std::vector<শূন্য*> &data)
{
    getTlsStorage().releaseSlot(চাবি_2, data, সঁচা);
}

শূন্য TLSDataContainer::release()
{
    যদি (চাবি_2 == -1)
        ঘূৰাই;  // already released
    std::vector<শূন্য*> data; data.reserve(32);
    getTlsStorage().releaseSlot(চাবি_2, data, মিছা); // Release key and get stored data for proper destruction
    চাবি_2 = -1;
    বাবে(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

শূন্য TLSDataContainer::cleanup()
{
    std::vector<শূন্য*> data; data.reserve(32);
    getTlsStorage().releaseSlot(চাবি_2, data, সঁচা); // Extract stored data with removal from TLS tables
    বাবে(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

শূন্য* TLSDataContainer::getData() ধ্রুৱ
{
    CV_Assert(চাবি_2 != -1 && "Can't fetch data from terminated TLS container.");
    শূন্য* pData = getTlsStorage().getData(চাবি_2); // Check if data was already allocated
    যদি(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        যত্ন
        {
            getTlsStorage().setData(চাবি_2, pData);
        }
        গৃহাণ (...)
        {
            deleteDataInstance(pData);
            ক্ষিপ;
        }
    }
    ঘূৰাই pData;
}

স্থির TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, নূতন TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    ঘূৰাই getCoreTlsDataTLS().getRef();
}

#যদি defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

বাহ্য "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

বাহ্য "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    যদি (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        যদি (lpReserved != রিক্ত) // called after ExitProcess() call
        {
            cv::__termination = সঁচা;
        }
        অন্যথা
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ঘূৰাই TRUE;
}
#endif


নামাকাশ {

#ifdef OPENCV_WITH_ITT
তার্কিক overrideThreadName()
{
    স্থির তার্কিক param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", মিছা);
    ঘূৰাই param;
}
#endif

স্থির পূর্ণাঙ্ক g_thread_অঙ্ক = 0;
শ্ৰেণী ThreadID {
সার্ৱজনিক:
    ধ্রুৱ পূর্ণাঙ্ক id;
    ThreadID() :
        id(CV_XADD(&g_thread_অঙ্ক, 1))
    {
#ifdef OPENCV_WITH_ITT
        যদি (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

স্থির TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, নূতন TLSData<ThreadID>());
}

} // namespace
পূর্ণাঙ্ক utils::getThreadID() { ঘূৰাই getThreadIDTLS().get()->id; }


শ্ৰেণী ParseError
{
    std::string bad_মান;
সার্ৱজনিক:
    ParseError(ধ্রুৱ std::string &bad_মান_2) :bad_মান(bad_মান_2) {}
    std::string toString(ধ্রুৱ std::string &param) ধ্রুৱ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_মান;
        ঘূৰাই out.str();
    }
};

template <typename T>
T parseOption(ধ্রুৱ std::string &);

template<>
অন্তঃস্থ তার্কিক parseOption(ধ্রুৱ std::string & value)
{
    যদি (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ঘূৰাই সঁচা;
    }
    যদি (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ঘূৰাই মিছা;
    }
    ক্ষিপ ParseError(value);
}

template<>
অন্তঃস্থ size_t parseOption(ধ্রুৱ std::string &value)
{
    size_t pos = 0;
    বাবে (; pos < value.size(); pos++)
    {
        যদি (!isdigit(value[pos]))
            ভঙ্গ;
    }
    cv::String মান_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(মান_Str);
    যদি (suffixStr.length() == 0)
        ঘূৰাই v;
    অন্যথা যদি (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ঘূৰাই v * 1024 * 1024;
    অন্যথা যদি (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ঘূৰাই v * 1024;
    ক্ষিপ ParseError(value);
}

template<>
অন্তঃস্থ cv::String parseOption(ধ্রুৱ std::string &value)
{
    ঘূৰাই value;
}

template<>
অন্তঃস্থ utils::Paths parseOption(ধ্রুৱ std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ধ্রুৱ ৱর্ণ sep = ';';
#অন্যথা
    ধ্রুৱ ৱর্ণ sep = ':';
#endif
    size_t start_pos = 0;
    যেতিয়ালৈ (start_pos != std::string::npos)
    {
        ধ্রুৱ size_t pos = value.find(sep, start_pos);
        ধ্রুৱ std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        যদি (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ঘূৰাই result;
}

স্থির অন্তঃস্থ ধ্রুৱ ৱর্ণ * envRead(ধ্রুৱ ৱর্ণ * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    ঘূৰাই রিক্ত;
#অন্যথা
    ঘূৰাই getenv(name);
#endif
}

template<typename T>
অন্তঃস্থ T পঠ(ধ্রুৱ std::string & k, ধ্রুৱ T & default_মান_2)
{
    যত্ন
    {
        ধ্রুৱ ৱর্ণ * res = envRead(k.c_str());
        যদি (res)
            ঘূৰাই parseOption<T>(std::string(res));
    }
    গৃহাণ (ধ্রুৱ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    ঘূৰাই default_মান_2;
}

তার্কিক utils::getConfigurationParameterBool(ধ্রুৱ ৱর্ণ* name, তার্কিক default_মান_2)
{
    ঘূৰাই পঠ<তার্কিক>(name, default_মান_2);
}

size_t utils::getConfigurationParameterSizeT(ধ্রুৱ ৱর্ণ* name, size_t default_মান_2)
{
    ঘূৰাই পঠ<size_t>(name, default_মান_2);
}

std::string utils::getConfigurationParameterString(ধ্রুৱ ৱর্ণ* name, ধ্রুৱ std::string & default_মান_2)
{
    ঘূৰাই পঠ<cv::String>(name, default_মান_2);
}

utils::Paths utils::getConfigurationParameterPaths(ধ্রুৱ ৱর্ণ* name, ধ্রুৱ utils::Paths &default_মান_2)
{
    ঘূৰাই পঠ<utils::Paths>(name, default_মান_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, নূতন ImplCollector())
}

শূন্য setImpl(পূর্ণাঙ্ক flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

শূন্য addImpl(পূর্ণাঙ্ক flag, ধ্রুৱ ৱর্ণ* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    যদি(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        যদি(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

পূর্ণাঙ্ক getImpl(std::vector<পূর্ণাঙ্ক> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    ঘূৰাই getImplData().implFlags; // return actual flags for lazy collection
}

তার্কিক useCollection()
{
    ঘূৰাই getImplData().useCollection;
}

শূন্য setUseCollection(তার্কিক flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

নামাকাশ instr
{
তার্কিক useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    ঘূৰাই getInstrumentStruct().useInstr;
#অন্যথা
    ঘূৰাই মিছা;
#endif
}

শূন্য setUseInstrumentation(তার্কিক flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#অন্যথা
    CV_UNUSED(flag);
#endif
}

Instr_গ্ৰন্থি* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    ঘূৰাই &getInstrumentStruct().মূল_গ্ৰন্থি_2;
#অন্যথা
    ঘূৰাই রিক্ত;
#endif
}

শূন্য resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().মূল_গ্ৰন্থি_2.removeChilds();
    getInstrumentTLSStruct().p_Current_গ্ৰন্থি = &getInstrumentStruct().মূল_গ্ৰন্থি_2;
#endif
}

শূন্য setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#অন্যথা
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    ঘূৰাই (FLAGS)getInstrumentStruct().flags;
#অন্যথা
    ঘূৰাই (FLAGS)0;
#endif
}

গ্ৰন্থি_Data::গ্ৰন্থি_Data(ধ্রুৱ ৱর্ণ* funName, ধ্রুৱ ৱর্ণ* fileName, পূর্ণাঙ্ক সাৰি_অঙ্ক, শূন্য* retAddress, তার্কিক alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_সাৰি_অঙ্ক       = সাৰি_অঙ্ক;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = মিছা;
}
গ্ৰন্থি_Data::গ্ৰন্থি_Data(গ্ৰন্থি_Data &ref)
{
    *এতত্ = ref;
}
গ্ৰন্থি_Data& গ্ৰন্থি_Data::operator=(ধ্রুৱ গ্ৰন্থি_Data &ডান)
{
    এতত্->m_funName      = ডান.m_funName;
    এতত্->m_instrType    = ডান.m_instrType;
    এতত্->m_implType     = ডান.m_implType;
    এতত্->m_fileName     = ডান.m_fileName;
    এতত্->m_সাৰি_অঙ্ক      = ডান.m_সাৰি_অঙ্ক;
    এতত্->m_retAddress   = ডান.m_retAddress;
    এতত্->m_alwaysExpand = ডান.m_alwaysExpand;

    এতত্->m_threads     = ডান.m_threads;
    এতত্->m_counter     = ডান.m_counter;
    এতত্->m_ticksTotal  = ডান.m_ticksTotal;

    এতত্->m_funError    = ডান.m_funError;

    ঘূৰাই *এতত্;
}
গ্ৰন্থি_Data::~গ্ৰন্থি_Data()
{
}
তার্কিক operator==(ধ্রুৱ গ্ৰন্থি_Data& বাম, ধ্রুৱ গ্ৰন্থি_Data& ডান)
{
    যদি(বাম.m_সাৰি_অঙ্ক == ডান.m_সাৰি_অঙ্ক && বাম.m_funName == ডান.m_funName && বাম.m_fileName == ডান.m_fileName)
    {
        যদি(বাম.m_retAddress == ডান.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || বাম.m_alwaysExpand))
            ঘূৰাই সঁচা;
    }
    ঘূৰাই মিছা;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    স্থির InstrStruct instr;
    ঘূৰাই instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ঘূৰাই *getInstrumentStruct().tlsStruct.get();
}

Instr_গ্ৰন্থি* get_Current_গ্ৰন্থি()
{
    ঘূৰাই getInstrumentTLSStruct().p_Current_গ্ৰন্থি;
}

IntrumentationRegion::IntrumentationRegion(ধ্রুৱ ৱর্ণ* funName, ধ্রুৱ ৱর্ণ* fileName, পূর্ণাঙ্ক সাৰি_অঙ্ক, শূন্য *retAddress, তার্কিক alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = মিছা;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    যদি(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        যদি(!pTLS->p_Current_গ্ৰন্থি)
        {
            m_disabled = সঁচা;
            ঘূৰাই;
        }

        পূর্ণাঙ্ক depth = pTLS->p_Current_গ্ৰন্থি->getDepth();
        যদি(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = সঁচা;
            ঘূৰাই;
        }

        গ্ৰন্থি_Data payload(funName, fileName, সাৰি_অঙ্ক, retAddress, alwaysExpand, instrType, implType);
        Node<গ্ৰন্থি_Data>* pChild = রিক্ত;

        যদি(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_গ্ৰন্থি->findChild(payload);
            যদি(!pChild)
            {
                pChild = নূতন Node<গ্ৰন্থি_Data>(payload);
                pTLS->p_Current_গ্ৰন্থি->addChild(pChild);
            }
        }
        অন্যথা
        {
            pChild = pTLS->p_Current_গ্ৰন্থি->findChild(payload);
            যদি(!pChild)
            {
                m_disabled = সঁচা;
                ঘূৰাই;
            }
        }
        pTLS->p_Current_গ্ৰন্থি = pChild;

        m_regionTicks = get_Tick_গুনতি();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    যদি(pStruct->useInstr)
    {
        যদি(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            যদি (pTLS->p_Current_গ্ৰন্থি->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_গ্ৰন্থি->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_গ্ৰন্থি->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_গুনতি() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_গুনতি); // Concurrent ticks accumulation
                pTLS->p_Current_গ্ৰন্থি->m_payload.m_counter++;
                pTLS->p_Current_গ্ৰন্থি->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_গ্ৰন্থি->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_গ্ৰন্থি = pTLS->p_Current_গ্ৰন্থি->m_pParent;
        }
    }
}
#endif
}

নামাকাশ ipp
{

#ifdef HAVE_IPP
সংরচনা IPPInitSingleton
{
সার্ৱজনিক:
    IPPInitSingleton()
    {
        useIPP         = সঁচা;

#যদি defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#অন্যথা
        useIPP_NE      = মিছা;
#endif
        ippStatus      = 0;
        funcname       = রিক্ত;
        filename       = রিক্ত;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_শীৰ্ষ_Features = 0;
        pIppLibInfo    = রিক্ত;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, রিক্ত);
        যদি(ippStatus < 0)
        {
            CV_LOG_ERROR(রিক্ত, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = মিছা;
            ঘূৰাই;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        যদি(!env.empty())
        {
#যদি IPP_VERSION_X100 >= 201900
            ধ্রুৱ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#নোচেদ্যদি IPP_VERSION_X100 >= 201703
            ধ্রুৱ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#নোচেদ্যদি IPP_VERSION_X100 >= 201700
            ধ্রুৱ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#অন্যথা
            ধ্রুৱ Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            যদি(env.substr(0, 2) == "ne")
            {
                useIPP_NE = সঁচা;
                env = env.substr(3, env.size());
            }

            যদি(env == "disabled")
            {
                CV_LOG_WARNING(রিক্ত, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = মিছা;
            }
            অন্যথা যদি(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            অন্যথা যদি(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#যদি IPP_VERSION_X100 >= 201700
#যদি defined (_M_AMD64) || defined (__x86_64__)
            অন্যথা যদি(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            অন্যথা
                CV_LOG_ERROR(রিক্ত, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        যদি(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        যদি(!(
#যদি IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = মিছা;
            ঘূৰাই;
        }

        যদি(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        অন্যথা
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#যদি IPP_VERSION_X100 >= 201700
        যদি(ippFeatures&ippCPUID_AVX512F)
        {
            যদি((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_শীৰ্ষ_Features = ippCPUID_AVX512_SKX;
            অন্যথা যদি((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_শীৰ্ষ_Features = ippCPUID_AVX512_KNL;
            অন্যথা
                ipp_শীৰ্ষ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        অন্যথা
#endif
        যদি(ippFeatures&ippCPUID_AVX2)
            ipp_শীৰ্ষ_Features = ippCPUID_AVX2;
        অন্যথা যদি(ippFeatures&ippCPUID_SSE42)
            ipp_শীৰ্ষ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        যদি (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_শীৰ্ষ_Features = ippCPUID_SSE42;
        }
    }

সার্ৱজনিক:
    তার্কিক        useIPP;
    তার্কিক        useIPP_NE;

    পূর্ণাঙ্ক         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ধ্রুৱ ৱর্ণ *funcname;
    ধ্রুৱ ৱর্ণ *filename;
    পূর্ণাঙ্ক         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_শীৰ্ষ_Features;
    ধ্রুৱ IppLibraryVersion *pIppLibInfo;
};

স্থির IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, নূতন IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    ঘূৰাই getIPPSingleton().ippFeatures;
#অন্যথা
    ঘূৰাই 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_শীৰ্ষ_Features()
{
    ঘূৰাই getIPPSingleton().ipp_শীৰ্ষ_Features;
}
#endif

শূন্য setIppStatus(পূর্ণাঙ্ক status, ধ্রুৱ ৱর্ণ * ধ্রুৱ _funcname, ধ্রুৱ ৱর্ণ * ধ্রুৱ _filename, পূর্ণাঙ্ক সাৰি_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = সাৰি_3;
#অন্যথা
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(সাৰি_3);
#endif
}

পূর্ণাঙ্ক getIppStatus()
{
#ifdef HAVE_IPP
    ঘূৰাই getIPPSingleton().ippStatus;
#অন্যথা
    ঘূৰাই 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    ঘূৰাই format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#অন্যথা
    ঘূৰাই String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ধ্রুৱ IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    যদি(pInfo)
        ঘূৰাই format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    অন্যথা
        ঘূৰাই String("error");
#অন্যথা
    ঘূৰাই String("disabled");
#endif
}

তার্কিক useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    যদি (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    ঘূৰাই (data.useIPP > 0);
#অন্যথা
    ঘূৰাই মিছা;
#endif
}

শূন্য setUseIPP(তার্কিক flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:মিছা;
#অন্যথা
    CV_UNUSED(flag);
    data.useIPP = মিছা;
#endif
}

তার্কিক useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    যদি (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ঘূৰাই (data.useIPP_NE > 0);
#অন্যথা
    ঘূৰাই মিছা;
#endif
}

শূন্য setUseIPP_NotExact(তার্কিক flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#অন্যথা
    CV_UNUSED(flag);
    data.useIPP_NE = মিছা;
#endif
}

} // namespace ipp


নামাকাশ details {

#যদি OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

শূন্য setFPDenormalsIgnoreHint(তার্কিক ignore, CV_OUT FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    যদি (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ধ্রুৱ unsigned old_flags = _mm_getcsr();
    ধ্রুৱ unsigned old_মান = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(রিক্ত, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_মান;
    _mm_setcsr(flags);
#অন্যথা
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

পূর্ণাঙ্ক saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    যদি (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ধ্রুৱ unsigned old_flags = _mm_getcsr();
    ধ্রুৱ unsigned old_মান = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_মান;
    ঘূৰাই 2;
#অন্যথা
    CV_UNUSED(state);
    ঘূৰাই 0;
#endif
}

তার্কিক restoreFPDenormalsState(ধ্রুৱ FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    ধ্রুৱ unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ধ্রুৱ unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((পূর্ণাঙ্ক)value, value == (value & mask), "invalid SSE FP state");
    ধ্রুৱ unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(রিক্ত, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ঘূৰাই সঁচা;
#অন্যথা
    CV_UNUSED(state);
    ঘূৰাই মিছা;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    ঘূৰাই OPENCV_ALGO_HINT_DEFAULT;
#অন্যথা
    ঘূৰাই ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
