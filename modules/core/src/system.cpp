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

#અન્તર્ભાવ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#અન્તર્ભાવ <atomic>
#અન્તર્ભાવ <exception>
#અન્તર્ભાવ <iostream>
#અન્તર્ભાવ <ostream>

#ifdef __QNX__
    #અન્તર્ભાવ <unistd.h>
    #અન્તર્ભાવ <sys/neutrino.h>
    #અન્તર્ભાવ <sys/syspage.h>
#ifdef __aarch64__
    #અન્તર્ભાવ <aarch64/syspage.h>
#endif
#endif

#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>
#અન્તર્ભાવ <opencv2/core/utils/trace.private.hpp>

#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#અન્તર્ભાવ <opencv2/core/utils/tls.hpp>
#અન્તર્ભાવ <opencv2/core/utils/instrumentation.hpp>

#અન્તર્ભાવ <opencv2/core/utils/filesystem.private.hpp>

#અન્તર્ભાવ <opencv2/core/utils/fp_control_utils.hpp>
#અન્તર્ભાવ <opencv2/core/utils/fp_control.private.hpp>

નામાકાશ cv {

સ્થિર શૂન્ય _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    સ્થિર std::ios_base::Init s_iostream_initializer;
#endif
}

સ્થિર Mutex* __initialization_mutex = ખાલી;
Mutex& getInitializationMutex()
{
    જો (__initialization_mutex == ખાલી)
    {
        (શૂન્ય)_initSystem();
        __initialization_mutex = નવું Mutex();
    }
    પાછા *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

સ્થિર તાર્કિક param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#જો defined(_DEBUG) || defined(__ANDROID__)
    સાચું
#નહીંતર
    ખોટું
#endif
);

શૂન્ય* allocSingletonBuffer(size_t size) { પાછા fastMalloc(size); }
શૂન્ય* allocSingletonNewBuffer(size_t size) { પાછા malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# જો defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#જો defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# જો _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#અન્તર્ભાવ <cstdlib>        // std::abort
#endif

#જો defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#જો defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
અચલ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#નહીંતર
#  include <elf.h>
#endif
#જો defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#જો defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#જો ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#જો defined __loongarch64
#અન્તર્ભાવ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#જો defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#અન્તર્ભાવ <windows.h>
#અન્તર્ભાવ <combaseapi.h>
#જો (_WIN32_WINNT >= 0x0602)
  #અન્તર્ભાવ <synchapi.h>
#endif
#જો ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #અન્તર્ભાવ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#અન્તર્ભાવ <tchar.h>

#ifdef WINRT
#અન્તર્ભાવ <wrl/client.h>
#ifndef __cplusplus_winrt
#અન્તર્ભાવ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    પાછા std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#નહીંતર
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    જો (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        પાછા wstr;
    જો (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        પાછા wstr;
    જો (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        પાછા wstr;
    જો (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        પાછા wstr;
    જો (FAILED(storagefolderRef.As(&storageitemRef)))
        પાછા wstr;
    str = ખાલી;
    જો (FAILED(storageitemRef->get_Path(&str)))
        પાછા wstr;
    wstr = WindowsGetStringRawBuffer(str, ખાલી);
    WindowsDeleteString(str);
    પાછા wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], કદ(guidStr)/કદ(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    પાછા prefix.append(std::wstring(guidStr));
}

#endif
#નહીંતર
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#અન્તર્ભાવ <pthread.h>
#endif
#અન્તર્ભાવ <sys/time.h>
#અન્તર્ભાવ <time.h>

#જો defined __MACH__ && defined __APPLE__
#અન્તર્ભાવ <mach/mach.h>
#અન્તર્ભાવ <mach/mach_time.h>
#અન્તર્ભાવ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#અન્તર્ભાવ "omp.h"
#endif

#જો defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <sys/types.h>
#જો defined __ANDROID__
#અન્તર્ભાવ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #જો defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #જો _MSC_VER >= 1400  // MSVS 2005
      #અન્તર્ભાવ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #નહીંતર
      #error "Required MSVS 2005+"
    #endif
  #અથવાજો defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    સ્થિર શૂન્ય cv_cpuid(પૂર્ણાંક* cpuid_data, પૂર્ણાંક reg_eax, પૂર્ણાંક reg_ecx)
    {
        પૂર્ણાંક __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#જો !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#અથવાજો defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#નહીંતર
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#અન્તર્ભાવ <chrono>

નામાકાશ cv
{

Exception::Exception() { code = Error::StsOk; લીટી = 0; }

Exception::Exception(Error::Code _code, અચલ String& _err, અચલ String& _func, અચલ String& _file, પૂર્ણાંક લીટી_3)
: code(_code), err(_err), func(_func), file(_file), લીટી(લીટી_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
અચલ અક્ષર* Exception::what() અચલ CV_NOEXCEPT { પાછા સંદેશ.c_str(); }

શૂન્ય Exception::formatMessage()
{
    size_t pos = err.find('\n');
    તાર્કિક multiline = pos != cv::String::npos;
    જો (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        જ્યારે (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        જો (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    જો (func.size() > 0)
    {
        જો (multiline)
            સંદેશ = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), લીટી, code, codeMessage(), func.c_str(), err.c_str());
        નહીંતર
            સંદેશ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), લીટી, code, codeMessage(), err.c_str(), func.c_str());
    }
    નહીંતર
    {
        સંદેશ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), લીટી, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

અચલ અક્ષર * Exception::codeMessage() અચલ
{
    પસંદગી (code)
    {
    સ્થિતિ cv::Error::StsOk :                  પાછા "No Error";
    સ્થિતિ cv::Error::StsBackTrace :           પાછા "Backtrace";
    સ્થિતિ cv::Error::StsError :               પાછા "Unspecified error";
    સ્થિતિ cv::Error::StsInternal :            પાછા "Internal error";
    સ્થિતિ cv::Error::StsNoMem :               પાછા "Insufficient memory";
    સ્થિતિ cv::Error::StsBadArg :              પાછા "Bad argument";
    સ્થિતિ cv::Error::StsNoConv :              પાછા "Iterations do not converge";
    સ્થિતિ cv::Error::StsAutoTrace :           પાછા "Autotrace call";
    સ્થિતિ cv::Error::StsBadSize :             પાછા "Incorrect size of input array";
    સ્થિતિ cv::Error::StsNullPtr :             પાછા "Null pointer";
    સ્થિતિ cv::Error::StsDivByZero :           પાછા "Division by zero occurred";
    સ્થિતિ cv::Error::BadStep :                પાછા "Image step is wrong";
    સ્થિતિ cv::Error::StsInplaceNotSupported : પાછા "Inplace operation is not supported";
    સ્થિતિ cv::Error::StsObjectNotFound :      પાછા "Requested object was not found";
    સ્થિતિ cv::Error::BadDepth :               પાછા "Input image depth is not supported by function";
    સ્થિતિ cv::Error::StsUnmatchedFormats :    પાછા "Formats of input arguments do not match";
    સ્થિતિ cv::Error::StsUnmatchedSizes :      પાછા "Sizes of input arguments do not match";
    સ્થિતિ cv::Error::StsOutOfRange :          પાછા "One of the arguments\' values is out of range";
    સ્થિતિ cv::Error::StsUnsupportedFormat :   પાછા "Unsupported format or combination of formats";
    સ્થિતિ cv::Error::BadCOI :                 પાછા "Input COI is not supported";
    સ્થિતિ cv::Error::Bad_અંક_Channels :         પાછા "Bad number of channels";
    સ્થિતિ cv::Error::StsBadFlag :             પાછા "Bad flag (parameter or structure field)";
    સ્થિતિ cv::Error::StsBadPoint :            પાછા "Bad parameter of type CvPoint";
    સ્થિતિ cv::Error::StsBadMask :             પાછા "Bad type of mask argument";
    સ્થિતિ cv::Error::StsParseError :          પાછા "Parsing error";
    સ્થિતિ cv::Error::StsNotImplemented :      પાછા "The function/feature is not implemented";
    સ્થિતિ cv::Error::StsBadMemBlock :         પાછા "Memory block has been corrupted";
    સ્થિતિ cv::Error::StsAssert :              પાછા "Assertion failed";
    સ્થિતિ cv::Error::GpuNotSupported :        પાછા "No CUDA support";
    સ્થિતિ cv::Error::GpuApiCallError :        પાછા "Gpu API call";
    સ્થિતિ cv::Error::OpenGlNotSupported :     પાછા "No OpenGL support";
    સ્થિતિ cv::Error::OpenGlApiCallError :     પાછા "OpenGL API call";
    સ્થિતિ cv::Error::OpenCLApiCallError :     પાછા "OpenCL API call error";
    સ્થિતિ cv::Error::OpenCLDoubleNotSupported:પાછા "OpenCL device does not support double";
    સ્થિતિ cv::Error::OpenCLInitError :        પાછા "OpenCL initialization error";
    સ્થિતિ cv::Error::OpenCLNoAMDBlasFft :     પાછા "Missing OpenCL AMD BLAS FFT";
    મૂળભૂત:
    {
        સ્થિર અક્ષર buf[256] {0};
        snprintf(buf, કદ(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        પાછા buf;
    }
    };

}

સ્થિર અચલ અક્ષર* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { ખાલી };

સ્થિર અચલ અક્ષર* getHWFeatureName(પૂર્ણાંક id)
{
    પાછા (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : ખાલી;
}
સ્થિર અચલ અક્ષર* getHWFeatureNameSafe(પૂર્ણાંક id)
{
    અચલ અક્ષર* name = getHWFeatureName(id);
    પાછા name ? name : "Unknown feature";
}

સંરચના HWFeatures
{
    ગણના { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(તાર્કિક run_initialize = ખોટું)
    {
        જો (run_initialize)
            initialize();
    }

    સ્થિર શૂન્ય initializeNames()
    {
        માટે (પૂર્ણાંક i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    શૂન્ય initialize(શૂન્ય)
    {
        જો (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        પૂર્ણાંક cpuid_data[4] = { 0, 0, 0, 0 };
        પૂર્ણાંક cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        પૂર્ણાંક x86_family = (cpuid_data[0] >> 8) & 15;
        જો( x86_family >= 6 )
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
            પૂર્ણાંક cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            તાર્કિક have_AVX_OS_support = સાચું;
            તાર્કિક have_AVX512_OS_support = સાચું;
            જો (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = ખોટું; // OS uses XSAVE_XRSTORE and CPU support AVX
            નહીંતર
            {
                પૂર્ણાંક xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (પૂર્ણાંક)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #અથવાજો defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                જો ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = ખોટું; // YMM registers
                જો ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = ખોટું; // ZMM registers
            }

            જો (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = ખોટું;
                have[CV_CPU_FP16] = ખોટું;
                have[CV_CPU_AVX2] = ખોટું;
                have[CV_CPU_FMA3] = ખોટું;
                have[CV_CPU_AVX_VNNI] = ખોટું;
            }
            જો (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = ખોટું;
                have[CV_CPU_AVX_512BW] = ખોટું;
                have[CV_CPU_AVX_512CD] = ખોટું;
                have[CV_CPU_AVX_512DQ] = ખોટું;
                have[CV_CPU_AVX_512ER] = ખોટું;
                have[CV_CPU_AVX_512IFMA] = ખોટું;
                have[CV_CPU_AVX_512PF] = ખોટું;
                have[CV_CPU_AVX_512VBMI] = ખોટું;
                have[CV_CPU_AVX_512VL] = ખોટું;
                have[CV_CPU_AVX_512VBMI2] = ખોટું;
                have[CV_CPU_AVX_512VNNI] = ખોટું;
                have[CV_CPU_AVX_512BITALG] = ખોટું;
                have[CV_CPU_AVX_512VPOPCNTDQ] = ખોટું;
                have[CV_CPU_AVX_5124VNNIW] = ખોટું;
                have[CV_CPU_AVX_5124FMAPS] = ખોટું;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            જો (have[CV_CPU_AVX512_COMMON])
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
            નહીંતર
            {
                have[CV_CPU_AVX512_KNL] = ખોટું;
                have[CV_CPU_AVX512_KNM] = ખોટું;
                have[CV_CPU_AVX512_SKX] = ખોટું;
                have[CV_CPU_AVX512_CNL] = ખોટું;
                have[CV_CPU_AVX512_CLX] = ખોટું;
                have[CV_CPU_AVX512_ICL] = ખોટું;
            }
        }
    #endif // CV_CPUID_X86

    #જો defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = સાચું;
        have[CV_CPU_FP16] = સાચું;
        પૂર્ણાંક cpufile = open("/proc/self/auxv", O_RDONLY);

        જો (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            અચલ size_t size_auxv_t = કદ(auxv);

            જ્યારે ((size_t)પઠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                જો (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#જો defined(AT_HWCAP2)
                નહીંતર જો (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #અથવાજો defined __arm__ && defined __ANDROID__
      #જો defined HAVE_CPUFEATURES
        CV_LOG_INFO(ખાલી, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ખાલી, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #નહીંતર
        CV_LOG_INFO(ખાલી, "cpufeatures library is not available for CPU detection");
        #જો CV_NEON
        CV_LOG_INFO(ખાલી, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = સાચું;
        #નહીંતર
        CV_LOG_INFO(ખાલી, "- NEON instructions is NOT enabled via build flags");
        #endif
        #જો CV_FP16
        CV_LOG_INFO(ખાલી, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = સાચું;
        #નહીંતર
        CV_LOG_INFO(ખાલી, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #અથવાજો defined __arm__
        પૂર્ણાંક cpufile = open("/proc/self/auxv", O_RDONLY);

        જો (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            અચલ size_t size_auxv_t = કદ(auxv);

            જ્યારે ((size_t)પઠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                જો (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    તોડો;
                }
            }

            close(cpufile);
        }
    #endif
    #અથવાજો (defined __APPLE__)
    #જો defined __ARM_NEON
        have[CV_CPU_NEON] = સાચું;
    #endif
    #જો (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = સાચું;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    પૂર્ણાંક has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = કદ(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, ખાલી, 0);
    જો (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = સાચું;
    }
    પૂર્ણાંક has_feat_bf16 = 0;
    size_t has_feat_bf16_size = કદ(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, ખાલી, 0);
    જો (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = સાચું;
    }
    #અથવાજો (defined __clang__)
    #જો defined __ARM_NEON
        have[CV_CPU_NEON] = સાચું;
        #જો (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = સાચું;
        #endif
    #endif
    #endif
    #જો defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = સાચું;
    #endif
    #જો defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = સાચું;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = સાચું;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = સાચું;
    #endif

    #જો (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned પૂર્ણાંક hwcap = getauxval(AT_HWCAP);
        જો (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            જો (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = સાચું;
            } નહીંતર {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #અથવાજો (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        જો (elf_aux_info(AT_HWCAP, &hwcap, કદ(hwcap)) != 0)
            hwcap = 0;
        જો (hwcap & PPC_FEATURE_HAS_VSX) {
            જો (elf_aux_info(AT_HWCAP2, &hwcap2, કદ(hwcap2)) != 0)
                hwcap2 = 0;
            જો (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = સાચું;
            } નહીંતર {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #નહીંતર
        // TODO: AIX
        #જો CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = સાચું;
        #endif
        #જો CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = સાચું;
        #endif
    #endif

    #જો defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = સાચું;
        #જો (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = સાચું;
        #endif
        #જો defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = સાચું;
        #endif
    #endif

    #જો defined __loongarch64 && defined __linux__
        પૂર્ણાંક flag = (પૂર્ણાંક)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        તાર્કિક skip_baseline_check = ખોટું;
        જો (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = સાચું;
        }
        પૂર્ણાંક baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        જો (!checkFeatures(baseline_features, કદ(baseline_features) / કદ(baseline_features[0]))
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
            checkFeatures(baseline_features, કદ(baseline_features) / કદ(baseline_features[0]), સાચું);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, કદ(baseline_features) / કદ(baseline_features[0]));
    }

    તાર્કિક checkFeatures(અચલ પૂર્ણાંક* features, પૂર્ણાંક ગણતરી_2, તાર્કિક dump = ખોટું)
    {
        તાર્કિક result = સાચું;
        માટે (પૂર્ણાંક i = 0; i < ગણતરી_2; i++)
        {
            પૂર્ણાંક feature = features[i];
            જો (feature)
            {
                જો (have[feature])
                {
                    જો (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                નહીંતર
                {
                    result = ખોટું;
                    જો (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        પાછા result;
    }

    સ્થિર અંતઃરેખ તાર્કિક is_પ્રતીક_Separator(અક્ષર c)
    {
        પાછા c == ',' || c == ';';
    }

    શૂન્ય readSettings(અચલ પૂર્ણાંક* baseline_features, પૂર્ણાંક baseline_ગણતરી)
    {
        તાર્કિક dump = સાચું;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        જો (!disabled_features.empty())
        {
            અચલ અક્ષર* start = disabled_features.c_str();
            માટે (;;)
            {
                જ્યારે (start[0] != 0 && is_પ્રતીક_Separator(start[0]))
                {
                    start++;
                }
                જો (start[0] == 0)
                    તોડો;
                અચલ અક્ષર* end = start;
                જ્યારે (end[0] != 0 && !is_પ્રતીક_Separator(end[0]))
                {
                    end++;
                }
                જો (end == start)
                    ચાલુ;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                તાર્કિક found = ખોટું;
                માટે (પૂર્ણાંક i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    જો (!g_hwFeatureNames[i]) ચાલુ;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    જો (len != feature.size()) ચાલુ;
                    જો (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        તાર્કિક isBaseline = ખોટું;
                        માટે (પૂર્ણાંક k = 0; k < baseline_ગણતરી; k++)
                        {
                            જો (baseline_features[k] == i)
                            {
                                isBaseline = સાચું;
                                તોડો;
                            }
                        }
                        જો (isBaseline)
                        {
                            જો (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        જો (!have[i])
                        {
                            જો (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = ખોટું;

                        found = સાચું;
                        તોડો;
                    }
                }
                જો (!found)
                {
                    જો (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    તાર્કિક have[MAX_FEATURE+1]{};
};

સ્થિર HWFeatures  featuresEnabled(સાચું), featuresDisabled = HWFeatures(ખોટું);
સ્થિર HWFeatures* currentFeatures = &featuresEnabled;

તાર્કિક checkHardwareSupport(પૂર્ણાંક feature)
{
    જો (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        પાછા ખોટું;
    પાછા currentFeatures->have[feature];
}

String getHardwareFeatureName(પૂર્ણાંક feature)
{
    અચલ અક્ષર* name = getHWFeatureName(feature);
    પાછા name ? String(name) : String();
}

std::string get_CPUFeatures_લીટી()
{
    અચલ પૂર્ણાંક features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    અચલ પૂર્ણાંક sz = કદ(features) / કદ(features[0]);
    std::string result;
    std::string prefix;
    માટે (પૂર્ણાંક i = 1; i < sz; ++i)
    {
        જો (features[i] == 0)
        {
            prefix = "*";
            ચાલુ;
        }
        જો (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        જો (!checkHardwareSupport(features[i])) result.append("?");
    }
    પાછા result;
}

અસ્થિર તાર્કિક useOptimizedFlag = સાચું;

શૂન્ય setUseOptimized( તાર્કિક flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

તાર્કિક useOptimized(શૂન્ય)
{
    પાછા useOptimizedFlag;
}

int64 get_Tick_ગણતરી(શૂન્ય)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    પાછા (int64)now.time_since_epoch().ગણતરી_2();
}

દ્વિગુણ getTickFrequency(શૂન્ય)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    દ્વિગુણ clock_freq = clock_period_t::den / clock_period_t::num;
    પાછા clock_freq;
}

#જો defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#જો defined(__i386__)

int64 get_CPUTick_ગણતરી(શૂન્ય)
{
    int64 x;
    __asm__ અસ્થિર (".byte 0x0f, 0x31" : "=A" (x));
    પાછા x;
}
#અથવાજો defined(__x86_64__)

int64 get_CPUTick_ગણતરી(શૂન્ય)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    પાછા (int64)lo | ((int64)hi << 32);
}

#અથવાજો defined(__ppc__)

int64 get_CPUTick_ગણતરી(શૂન્ય)
{
    unsigned upper, lower, tmp;
    __asm__ અસ્થિર(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    પાછા lower | ((int64)upper << 32);
}

#નહીંતર

#error "RDTSC not defined"

#endif

#અથવાજો defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ગણતરી(શૂન્ય)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#નહીંતર

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_ગણતરી(શૂન્ય)
{
    પાછા get_Tick_ગણતરી();
}
//#endif

#endif


નામાકાશ internal {

વર્ગ Timestamp
{
જાહેર:
    અચલ int64 zero_Tick_ગણતરી;
    અચલ દ્વિગુણ ns_in_ticks;

    Timestamp()
        : zero_Tick_ગણતરી(get_Tick_ગણતરી())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ગણતરી();
        પાછા (int64)((t - zero_Tick_ગણતરી) * ns_in_ticks);
    }

    સ્થિર Timestamp& getInstance()
    {
        સ્થિર Timestamp g_timestamp;
        પાછા g_timestamp;
    }
};

વર્ગ InitTimestamp {
જાહેર:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
સ્થિર InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    પાછા internal::Timestamp::getInstance().getTimestamp();
}


અચલ String& getBuildInformation()
{
    સ્થિર String build_info =
#અન્તર્ભાવ "version_string.inc"
    ;
    પાછા build_info;
}

String getVersionString() { પાછા String(CV_VERSION); }

પૂર્ણાંક getVersionMajor() { પાછા CV_VERSION_MAJOR; }

પૂર્ણાંક getVersionMinor() { પાછા CV_VERSION_MINOR; }

પૂર્ણાંક getVersionRevision() { પાછા CV_VERSION_REVISION; }

String format( અચલ અક્ષર* fmt, ... )
{
    AutoBuffer<અક્ષર, 1024> buf;

    માટે ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        પૂર્ણાંક bsize = static_cast<પૂર્ણાંક>(buf.size());
        પૂર્ણાંક len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        જો (len >= bsize)
        {
            buf.resize(len + 1);
            ચાલુ;
        }
        buf[bsize - 1] = 0;
        પાછા String(buf.data(), len);
    }
}

String tempfile( અચલ અક્ષર* suffix )
{
#જો OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#જો defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    જો (temp_file.empty())
        પાછા String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    અક્ષર aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#અથવાજો defined(_WIN32_WCE)
    અચલ auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    જો(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        અક્ષર aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#નહીંતર
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    જો (temp_dir.empty())
    {
        ::GetTempPathW(કદ(temp_dir2_w)/કદ(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        પૂર્ણાંક len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, ખાલી, 0, ખાલી, ખાલી);
        જો (len > 0)
        {
            std::vector<અક્ષર> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, ખાલી, ખાલી);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    જો (FAILED(hr))
        પાછા String();
    અક્ષર guidStr[40];
    અચલ અક્ષર* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, કદ(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned પૂર્ણાંક)g.Data4[0], (unsigned પૂર્ણાંક)g.Data4[1],
            (unsigned પૂર્ણાંક)g.Data4[2], (unsigned પૂર્ણાંક)g.Data4[3], (unsigned પૂર્ણાંક)g.Data4[4],
            (unsigned પૂર્ણાંક)g.Data4[5], (unsigned પૂર્ણાંક)g.Data4[6], (unsigned પૂર્ણાંક)g.Data4[7]);

    fname = temp_dir;
    જો (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# નહીંતર
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    અક્ષર defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  નહીંતર
    અક્ષર defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    જો (temp_dir.empty())
        fname = defaultTemplate;
    નહીંતર
    {
        fname = temp_dir;
        અક્ષર ech = fname[fname.size() - 1];
        જો(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    અચલ પૂર્ણાંક fd = mkstemp((અક્ષર*)fname.c_str());
    જો (fd == -1) પાછા String();

    close(fd);
    remove(fname.c_str());
# endif

    જો (suffix)
    {
        જો (suffix[0] != '.')
            પાછા fname + "." + suffix;
        નહીંતર
            પાછા fname + suffix;
    }
    પાછા fname;
#નહીંતર // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

સ્થિર ErrorCallback customErrorCallback = 0;
સ્થિર શૂન્ય* customErrorCallbackData = 0;
સ્થિર તાર્કિક breakOnError = ખોટું;

તાર્કિક setBreakOnError(તાર્કિક value)
{
    તાર્કિક prevVal = breakOnError;
    breakOnError = value;
    પાછા prevVal;
}

પૂર્ણાંક cv_snprintf(અક્ષર* buf, પૂર્ણાંક len, અચલ અક્ષર* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    પૂર્ણાંક res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    પાછા res;
}

પૂર્ણાંક cv_vsnprintf(અક્ષર* buf, પૂર્ણાંક len, અચલ અક્ષર* fmt, va_list args)
{
#જો defined _MSC_VER
    જો (len <= 0) પાછા len == 0 ? 1024 : -1;
    પૂર્ણાંક res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    જો (res >= 0 && res < len)
    {
        buf[res] = 0;
        પાછા res;
    }
    નહીંતર
    {
        buf[len - 1] = 0; // truncate happened
        પાછા res >= len ? res : (len * 2);
    }
#નહીંતર
    પાછા vsnprintf(buf, len, fmt, args);
#endif
}

સ્થિર શૂન્ય dumpException(અચલ Exception& exc)
{
    અચલ અક્ષર* errorStr = exc.codeMessage();
    અક્ષર buf[1 << 12];

    cv_snprintf(buf, કદ(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.લીટી);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#નહીંતર
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
સ્થિર તાર્કિક cv_terminate_handler_installed = ખોટું;
સ્થિર std::terminate_handler cv_old_terminate_handler;
સ્થિર cv::Exception cv_terminate_handler_exception;
સ્થિર તાર્કિક param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", સાચું);
સ્થિર શૂન્ય cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    જો (ખોટું /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# જો defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

શૂન્ય error( અચલ Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        જો (!cv_terminate_handler_installed)
        {
            જો (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = સાચું;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    જો (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.લીટી, customErrorCallbackData);
    નહીંતર જો (param_dumpErrors)
    {
        dumpException(exc);
    }

    જો(breakOnError)
    {
        std::terminate();
    }

    ફેંકો exc;
#ifdef __GNUC__
# જો !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

શૂન્ય error(Error::Code _code, અચલ String& _err, અચલ અક્ષર* _func, અચલ અક્ષર* _file, પૂર્ણાંક લીટી_3)
{
    error(cv::Exception(_code, _err, _func, _file, લીટી_3));
#ifdef __GNUC__
# જો !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# જો defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, શૂન્ય* userdata, શૂન્ય** prevUserdata)
{
    જો( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    પાછા prevCallback;
}

શૂન્ય terminate(Error::Code _code, અચલ String& _err, અચલ અક્ષર* _func, અચલ અક્ષર* _file, પૂર્ણાંક લીટી_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, લીટી_3));
    std::terminate();
}

}

નામાકાશ cv {
તાર્કિક __termination = ખોટું;


//////////////////////////////// thread-local storage ////////////////////////////////

નામાકાશ details {

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
વર્ગ TlsAbstraction
{
જાહેર:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    શૂન્ય* getData() અચલ;
    શૂન્ય setData(શૂન્ય *pData);

    શૂન્ય releaseSystemResources();

ખાનગી:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_ચાવી;
    તાર્કિક disposed;
#endif
#નહીંતર // _WIN32
    pthread_key_t  tls_ચાવી;
    std::atomic<તાર્કિક> disposed;
#endif
};

વર્ગ TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
જાહેર:
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
સ્થિર TlsAbstraction* getTlsAbstraction()
{
    સ્થિર TlsAbstraction *g_tls = નવું TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    સ્થિર TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    પાછા g_tls;
}


#ifdef _WIN32
#ifdef WINRT
સ્થિર __declspec( thread ) શૂન્ય* tlsData = ખાલી; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
શૂન્ય TlsAbstraction::releaseSystemResources()
{
    cv::__termination = સાચું;  // DllMain is missing in static builds
}
શૂન્ય* TlsAbstraction::getData() અચલ
{
    પાછા tlsData;
}
શૂન્ય TlsAbstraction::setData(શૂન્ય *pData)
{
    tlsData = pData;
}
#નહીંતર //WINRT
#ifdef CV_USE_FLS
સ્થિર શૂન્ય NTAPI opencv_fls_destructor(શૂન્ય* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(ખોટું)
{
#ifndef CV_USE_FLS
    tls_ચાવી = TlsAlloc();
#નહીંતર // CV_USE_FLS
    tls_ચાવી = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_ચાવી != TLS_OUT_OF_INDEXES);
}
શૂન્ય TlsAbstraction::releaseSystemResources()
{
    cv::__termination = સાચું;  // DllMain is missing in static builds
    disposed = સાચું;
#ifndef CV_USE_FLS
    TlsFree(tls_ચાવી);
#નહીંતર // CV_USE_FLS
    FlsFree(tls_ચાવી);
#endif // CV_USE_FLS
    tls_ચાવી = TLS_OUT_OF_INDEXES;
}
શૂન્ય* TlsAbstraction::getData() અચલ
{
    જો (disposed)
        પાછા ખાલી;
#ifndef CV_USE_FLS
    પાછા Tls_Get_કિંમત(tls_ચાવી);
#નહીંતર // CV_USE_FLS
    પાછા Fls_Get_કિંમત(tls_ચાવી);
#endif // CV_USE_FLS
}
શૂન્ય TlsAbstraction::setData(શૂન્ય *pData)
{
    જો (disposed)
        પાછા;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_કિંમત(tls_ચાવી, pData) == TRUE);
#નહીંતર // CV_USE_FLS
    CV_Assert(Fls_Set_કિંમત(tls_ચાવી, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#નહીંતર // _WIN32
સ્થિર શૂન્ય opencv_tls_destructor(શૂન્ય* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(ખોટું)
{
    CV_Assert(pthread_key_create(&tls_ચાવી, opencv_tls_destructor) == 0);
}
શૂન્ય TlsAbstraction::releaseSystemResources()
{
    cv::__termination = સાચું;  // DllMain is missing in static builds
    disposed = સાચું;
    જો (pthread_key_delete(tls_ચાવી) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
શૂન્ય* TlsAbstraction::getData() અચલ
{
    જો (disposed)
        પાછા ખાલી;
    પાછા pthread_getspecific(tls_ચાવી);
}
શૂન્ય TlsAbstraction::setData(શૂન્ય *pData)
{
    જો (disposed)
        પાછા;  // no-op
    CV_Assert(pthread_setspecific(tls_ચાવી, pData) == 0);
}
#endif

// Per-thread data structure
સંરચના ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<શૂન્ય*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


સ્થિર તાર્કિક g_isTlsStorageInitialized = ખોટું;

// Main TLS storage class
વર્ગ TlsStorage
{
જાહેર:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (શૂન્ય)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = સાચું;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    શૂન્ય releaseThread(શૂન્ય* tls_કિંમત = ખાલી)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        જો (ખાલી == tls)
            પાછા;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_કિંમત == ખાલી ? (ThreadData*)tls->getData() : (ThreadData*)tls_કિંમત;
        જો (pTD == ખાલી)
            પાછા;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        માટે (size_t i = 0; i < threads.size(); i++)
        {
            જો (pTD == threads[i])
            {
                threads[i] = ખાલી;
                જો (tls_કિંમત == ખાલી)
                    tls->setData(0);
                std::vector<શૂન્ય*>& thread_slots = pTD->slots;
                માટે (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    શૂન્ય* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = ખાલી;
                    જો (!pData)
                        ચાલુ;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    જો (container)
                        container->deleteDataInstance(pData);
                    નહીંતર
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (પૂર્ણાંક)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                પાછા;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (શૂન્ય*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        માટે(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            જો (tlsSlots[slot].container == ખાલી)
            {
                tlsSlots[slot].container = container;
                પાછા slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        પાછા tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    શૂન્ય releaseSlot(size_t slotIdx, std::vector<શૂન્ય*> &dataVec, તાર્કિક keepSlot = ખોટું)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        માટે(size_t i = 0; i < threads.size(); i++)
        {
            જો(threads[i])
            {
                std::vector<શૂન્ય*>& thread_slots = threads[i]->slots;
                જો (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = ખાલી;
                }
            }
        }

        જો (!keepSlot)
        {
            tlsSlots[slotIdx].container = ખાલી;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    શૂન્ય* getData(size_t slotIdx) અચલ
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        જો (ખાલી == tls)
            પાછા ખાલી;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        જો(threadData && threadData->slots.size() > slotIdx)
            પાછા threadData->slots[slotIdx];

        પાછા ખાલી;
    }

    // Gather data from threads by TLS storage index
    શૂન્ય gather(size_t slotIdx, std::vector<શૂન્ય*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        માટે(size_t i = 0; i < threads.size(); i++)
        {
            જો(threads[i])
            {
                std::vector<શૂન્ય*>& thread_slots = threads[i]->slots;
                જો (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    શૂન્ય setData(size_t slotIdx, શૂન્ય* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        જો (ખાલી == tls)
            પાછા;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        જો(!threadData)
        {
            threadData = નવું ThreadData;
            tls->setData((શૂન્ય*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                તાર્કિક found = ખોટું;
                // Find unused slots
                માટે(size_t slot = 0; slot < threads.size(); slot++)
                {
                    જો (threads[slot] == ખાલી)
                    {
                        threadData->idx = (પૂર્ણાંક)slot;
                        threads[slot] = threadData;
                        found = સાચું;
                        તોડો;
                    }
                }

                જો (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        જો(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, ખાલી);
        }
        threadData->slots[slotIdx] = pData;
    }

ખાનગી:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    સંરચના TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<સંરચના TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
સ્થિર TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, નવું TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
સ્થિર શૂન્ય opencv_tls_destructor(શૂન્ય* pData)
{
    જો (!g_isTlsStorageInitialized)
        પાછા;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#નહીંતર // _WIN32
#ifdef CV_USE_FLS
સ્થિર શૂન્ય WINAPI opencv_fls_destructor(શૂન્ય* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    જો (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = સાચું;
    }

    જો (!g_isTlsStorageInitialized)
        પાછા;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

સ્થિર TlsStorage* અચલ g_force_initialization_of_TlsStorage
#જો defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#નહીંતર  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
વર્ગ TlsStorage
{
જાહેર:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        માટે (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            જો (container && s.data)
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
        માટે (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            જો (s.container == ખાલી)
            {
                CV_Assert(!s.data);
                s.container = container;
                પાછા slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        પાછા slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    શૂન્ય releaseSlot(size_t slotIdx, std::vector<શૂન્ય*> &dataVec, તાર્કિક keepSlot = ખોટું)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        શૂન્ય* data = s.data;
        જો (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        જો (!keepSlot)
        {
            s.container = ખાલી;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    શૂન્ય* getData(size_t slotIdx) અચલ
    {
        CV_Assert(slotIdx < slots.size());
        અચલ SlotInfo& s = slots[slotIdx];
        પાછા s.data;
    }

    // Gather data from threads by TLS storage index
    શૂન્ય gather(size_t slotIdx, std::vector<શૂન્ય*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        શૂન્ય* data = s.data;
        જો (data)
            dataVec.push_back(data);
        પાછા;
    }

    // Set data to storage index
    શૂન્ય setData(size_t slotIdx, શૂન્ય* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

ખાનગી:
    સંરચના SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        શૂન્ય* data;
    };
    std::vector<સંરચના SlotInfo> slots;
};

સ્થિર TlsStorage& getTlsStorage()
{
    સ્થિર TlsStorage g_storage;  // no threading
    પાછા g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using નામાકાશ details;

શૂન્ય releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    જો (!g_isTlsStorageInitialized)
        પાછા;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    ચાવી_2 = (પૂર્ણાંક)getTlsStorage().reserveSlot(આ); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ચાવી_2 == -1); // Key must be released in child object
}

શૂન્ય TLSDataContainer::gatherData(std::vector<શૂન્ય*> &data) અચલ
{
    getTlsStorage().gather(ચાવી_2, data);
}

શૂન્ય TLSDataContainer::detachData(std::vector<શૂન્ય*> &data)
{
    getTlsStorage().releaseSlot(ચાવી_2, data, સાચું);
}

શૂન્ય TLSDataContainer::release()
{
    જો (ચાવી_2 == -1)
        પાછા;  // already released
    std::vector<શૂન્ય*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ચાવી_2, data, ખોટું); // Release key and get stored data for proper destruction
    ચાવી_2 = -1;
    માટે(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

શૂન્ય TLSDataContainer::cleanup()
{
    std::vector<શૂન્ય*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ચાવી_2, data, સાચું); // Extract stored data with removal from TLS tables
    માટે(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

શૂન્ય* TLSDataContainer::getData() અચલ
{
    CV_Assert(ચાવી_2 != -1 && "Can't fetch data from terminated TLS container.");
    શૂન્ય* pData = getTlsStorage().getData(ચાવી_2); // Check if data was already allocated
    જો(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        પ્રયાસ
        {
            getTlsStorage().setData(ચાવી_2, pData);
        }
        પકડો (...)
        {
            deleteDataInstance(pData);
            ફેંકો;
        }
    }
    પાછા pData;
}

સ્થિર TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, નવું TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    પાછા getCoreTlsDataTLS().getRef();
}

#જો defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

બાહ્ય "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

બાહ્ય "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    જો (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        જો (lpReserved != ખાલી) // called after ExitProcess() call
        {
            cv::__termination = સાચું;
        }
        નહીંતર
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    પાછા TRUE;
}
#endif


નામાકાશ {

#ifdef OPENCV_WITH_ITT
તાર્કિક overrideThreadName()
{
    સ્થિર તાર્કિક param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", ખોટું);
    પાછા param;
}
#endif

સ્થિર પૂર્ણાંક g_thread_અંક = 0;
વર્ગ ThreadID {
જાહેર:
    અચલ પૂર્ણાંક id;
    ThreadID() :
        id(CV_XADD(&g_thread_અંક, 1))
    {
#ifdef OPENCV_WITH_ITT
        જો (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

સ્થિર TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, નવું TLSData<ThreadID>());
}

} // namespace
પૂર્ણાંક utils::getThreadID() { પાછા getThreadIDTLS().get()->id; }


વર્ગ ParseError
{
    std::string bad_કિંમત;
જાહેર:
    ParseError(અચલ std::string &bad_કિંમત_2) :bad_કિંમત(bad_કિંમત_2) {}
    std::string toString(અચલ std::string &param) અચલ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_કિંમત;
        પાછા out.str();
    }
};

template <typename T>
T parseOption(અચલ std::string &);

template<>
અંતઃરેખ તાર્કિક parseOption(અચલ std::string & value)
{
    જો (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        પાછા સાચું;
    }
    જો (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        પાછા ખોટું;
    }
    ફેંકો ParseError(value);
}

template<>
અંતઃરેખ size_t parseOption(અચલ std::string &value)
{
    size_t pos = 0;
    માટે (; pos < value.size(); pos++)
    {
        જો (!isdigit(value[pos]))
            તોડો;
    }
    cv::String કિંમત_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(કિંમત_Str);
    જો (suffixStr.length() == 0)
        પાછા v;
    નહીંતર જો (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        પાછા v * 1024 * 1024;
    નહીંતર જો (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        પાછા v * 1024;
    ફેંકો ParseError(value);
}

template<>
અંતઃરેખ cv::String parseOption(અચલ std::string &value)
{
    પાછા value;
}

template<>
અંતઃરેખ utils::Paths parseOption(અચલ std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    અચલ અક્ષર sep = ';';
#નહીંતર
    અચલ અક્ષર sep = ':';
#endif
    size_t start_pos = 0;
    જ્યારે (start_pos != std::string::npos)
    {
        અચલ size_t pos = value.find(sep, start_pos);
        અચલ std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        જો (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    પાછા result;
}

સ્થિર અંતઃરેખ અચલ અક્ષર * envRead(અચલ અક્ષર * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    પાછા ખાલી;
#નહીંતર
    પાછા getenv(name);
#endif
}

template<typename T>
અંતઃરેખ T પઠ(અચલ std::string & k, અચલ T & default_કિંમત_2)
{
    પ્રયાસ
    {
        અચલ અક્ષર * res = envRead(k.c_str());
        જો (res)
            પાછા parseOption<T>(std::string(res));
    }
    પકડો (અચલ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    પાછા default_કિંમત_2;
}

તાર્કિક utils::getConfigurationParameterBool(અચલ અક્ષર* name, તાર્કિક default_કિંમત_2)
{
    પાછા પઠ<તાર્કિક>(name, default_કિંમત_2);
}

size_t utils::getConfigurationParameterSizeT(અચલ અક્ષર* name, size_t default_કિંમત_2)
{
    પાછા પઠ<size_t>(name, default_કિંમત_2);
}

std::string utils::getConfigurationParameterString(અચલ અક્ષર* name, અચલ std::string & default_કિંમત_2)
{
    પાછા પઠ<cv::String>(name, default_કિંમત_2);
}

utils::Paths utils::getConfigurationParameterPaths(અચલ અક્ષર* name, અચલ utils::Paths &default_કિંમત_2)
{
    પાછા પઠ<utils::Paths>(name, default_કિંમત_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, નવું ImplCollector())
}

શૂન્ય setImpl(પૂર્ણાંક flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

શૂન્ય addImpl(પૂર્ણાંક flag, અચલ અક્ષર* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    જો(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        જો(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

પૂર્ણાંક getImpl(std::vector<પૂર્ણાંક> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    પાછા getImplData().implFlags; // return actual flags for lazy collection
}

તાર્કિક useCollection()
{
    પાછા getImplData().useCollection;
}

શૂન્ય setUseCollection(તાર્કિક flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

નામાકાશ instr
{
તાર્કિક useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    પાછા getInstrumentStruct().useInstr;
#નહીંતર
    પાછા ખોટું;
#endif
}

શૂન્ય setUseInstrumentation(તાર્કિક flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#નહીંતર
    CV_UNUSED(flag);
#endif
}

Instr_ગાંઠ* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    પાછા &getInstrumentStruct().મૂળ_ગાંઠ_2;
#નહીંતર
    પાછા ખાલી;
#endif
}

શૂન્ય resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().મૂળ_ગાંઠ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ગાંઠ = &getInstrumentStruct().મૂળ_ગાંઠ_2;
#endif
}

શૂન્ય setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#નહીંતર
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    પાછા (FLAGS)getInstrumentStruct().flags;
#નહીંતર
    પાછા (FLAGS)0;
#endif
}

ગાંઠ_Data::ગાંઠ_Data(અચલ અક્ષર* funName, અચલ અક્ષર* fileName, પૂર્ણાંક લીટી_અંક, શૂન્ય* retAddress, તાર્કિક alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_લીટી_અંક       = લીટી_અંક;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = ખોટું;
}
ગાંઠ_Data::ગાંઠ_Data(ગાંઠ_Data &ref)
{
    *આ = ref;
}
ગાંઠ_Data& ગાંઠ_Data::operator=(અચલ ગાંઠ_Data &જમણું)
{
    આ->m_funName      = જમણું.m_funName;
    આ->m_instrType    = જમણું.m_instrType;
    આ->m_implType     = જમણું.m_implType;
    આ->m_fileName     = જમણું.m_fileName;
    આ->m_લીટી_અંક      = જમણું.m_લીટી_અંક;
    આ->m_retAddress   = જમણું.m_retAddress;
    આ->m_alwaysExpand = જમણું.m_alwaysExpand;

    આ->m_threads     = જમણું.m_threads;
    આ->m_counter     = જમણું.m_counter;
    આ->m_ticksTotal  = જમણું.m_ticksTotal;

    આ->m_funError    = જમણું.m_funError;

    પાછા *આ;
}
ગાંઠ_Data::~ગાંઠ_Data()
{
}
તાર્કિક operator==(અચલ ગાંઠ_Data& ડાબું, અચલ ગાંઠ_Data& જમણું)
{
    જો(ડાબું.m_લીટી_અંક == જમણું.m_લીટી_અંક && ડાબું.m_funName == જમણું.m_funName && ડાબું.m_fileName == જમણું.m_fileName)
    {
        જો(ડાબું.m_retAddress == જમણું.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || ડાબું.m_alwaysExpand))
            પાછા સાચું;
    }
    પાછા ખોટું;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    સ્થિર InstrStruct instr;
    પાછા instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    પાછા *getInstrumentStruct().tlsStruct.get();
}

Instr_ગાંઠ* get_Current_ગાંઠ()
{
    પાછા getInstrumentTLSStruct().p_Current_ગાંઠ;
}

IntrumentationRegion::IntrumentationRegion(અચલ અક્ષર* funName, અચલ અક્ષર* fileName, પૂર્ણાંક લીટી_અંક, શૂન્ય *retAddress, તાર્કિક alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = ખોટું;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    જો(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        જો(!pTLS->p_Current_ગાંઠ)
        {
            m_disabled = સાચું;
            પાછા;
        }

        પૂર્ણાંક depth = pTLS->p_Current_ગાંઠ->getDepth();
        જો(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = સાચું;
            પાછા;
        }

        ગાંઠ_Data payload(funName, fileName, લીટી_અંક, retAddress, alwaysExpand, instrType, implType);
        Node<ગાંઠ_Data>* pChild = ખાલી;

        જો(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_ગાંઠ->findChild(payload);
            જો(!pChild)
            {
                pChild = નવું Node<ગાંઠ_Data>(payload);
                pTLS->p_Current_ગાંઠ->addChild(pChild);
            }
        }
        નહીંતર
        {
            pChild = pTLS->p_Current_ગાંઠ->findChild(payload);
            જો(!pChild)
            {
                m_disabled = સાચું;
                પાછા;
            }
        }
        pTLS->p_Current_ગાંઠ = pChild;

        m_regionTicks = get_Tick_ગણતરી();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    જો(pStruct->useInstr)
    {
        જો(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            જો (pTLS->p_Current_ગાંઠ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ગાંઠ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ગાંઠ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ગણતરી() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_ગણતરી); // Concurrent ticks accumulation
                pTLS->p_Current_ગાંઠ->m_payload.m_counter++;
                pTLS->p_Current_ગાંઠ->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_ગાંઠ->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_ગાંઠ = pTLS->p_Current_ગાંઠ->m_pParent;
        }
    }
}
#endif
}

નામાકાશ ipp
{

#ifdef HAVE_IPP
સંરચના IPPInitSingleton
{
જાહેર:
    IPPInitSingleton()
    {
        useIPP         = સાચું;

#જો defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#નહીંતર
        useIPP_NE      = ખોટું;
#endif
        ippStatus      = 0;
        funcname       = ખાલી;
        filename       = ખાલી;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_ટોચ_Features = 0;
        pIppLibInfo    = ખાલી;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, ખાલી);
        જો(ippStatus < 0)
        {
            CV_LOG_ERROR(ખાલી, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = ખોટું;
            પાછા;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        જો(!env.empty())
        {
#જો IPP_VERSION_X100 >= 201900
            અચલ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#અથવાજો IPP_VERSION_X100 >= 201703
            અચલ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#અથવાજો IPP_VERSION_X100 >= 201700
            અચલ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#નહીંતર
            અચલ Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            જો(env.substr(0, 2) == "ne")
            {
                useIPP_NE = સાચું;
                env = env.substr(3, env.size());
            }

            જો(env == "disabled")
            {
                CV_LOG_WARNING(ખાલી, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = ખોટું;
            }
            નહીંતર જો(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            નહીંતર જો(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#જો IPP_VERSION_X100 >= 201700
#જો defined (_M_AMD64) || defined (__x86_64__)
            નહીંતર જો(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            નહીંતર
                CV_LOG_ERROR(ખાલી, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        જો(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        જો(!(
#જો IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = ખોટું;
            પાછા;
        }

        જો(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        નહીંતર
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#જો IPP_VERSION_X100 >= 201700
        જો(ippFeatures&ippCPUID_AVX512F)
        {
            જો((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_ટોચ_Features = ippCPUID_AVX512_SKX;
            નહીંતર જો((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_ટોચ_Features = ippCPUID_AVX512_KNL;
            નહીંતર
                ipp_ટોચ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        નહીંતર
#endif
        જો(ippFeatures&ippCPUID_AVX2)
            ipp_ટોચ_Features = ippCPUID_AVX2;
        નહીંતર જો(ippFeatures&ippCPUID_SSE42)
            ipp_ટોચ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        જો (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_ટોચ_Features = ippCPUID_SSE42;
        }
    }

જાહેર:
    તાર્કિક        useIPP;
    તાર્કિક        useIPP_NE;

    પૂર્ણાંક         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    અચલ અક્ષર *funcname;
    અચલ અક્ષર *filename;
    પૂર્ણાંક         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_ટોચ_Features;
    અચલ IppLibraryVersion *pIppLibInfo;
};

સ્થિર IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, નવું IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    પાછા getIPPSingleton().ippFeatures;
#નહીંતર
    પાછા 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_ટોચ_Features()
{
    પાછા getIPPSingleton().ipp_ટોચ_Features;
}
#endif

શૂન્ય setIppStatus(પૂર્ણાંક status, અચલ અક્ષર * અચલ _funcname, અચલ અક્ષર * અચલ _filename, પૂર્ણાંક લીટી_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = લીટી_3;
#નહીંતર
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(લીટી_3);
#endif
}

પૂર્ણાંક getIppStatus()
{
#ifdef HAVE_IPP
    પાછા getIPPSingleton().ippStatus;
#નહીંતર
    પાછા 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    પાછા format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#નહીંતર
    પાછા String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    અચલ IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    જો(pInfo)
        પાછા format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    નહીંતર
        પાછા String("error");
#નહીંતર
    પાછા String("disabled");
#endif
}

તાર્કિક useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    જો (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    પાછા (data.useIPP > 0);
#નહીંતર
    પાછા ખોટું;
#endif
}

શૂન્ય setUseIPP(તાર્કિક flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:ખોટું;
#નહીંતર
    CV_UNUSED(flag);
    data.useIPP = ખોટું;
#endif
}

તાર્કિક useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    જો (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    પાછા (data.useIPP_NE > 0);
#નહીંતર
    પાછા ખોટું;
#endif
}

શૂન્ય setUseIPP_NotExact(તાર્કિક flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#નહીંતર
    CV_UNUSED(flag);
    data.useIPP_NE = ખોટું;
#endif
}

} // namespace ipp


નામાકાશ details {

#જો OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

શૂન્ય setFPDenormalsIgnoreHint(તાર્કિક ignore, CV_OUT FPDenormalsModeState& state)
{
#જો OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    જો (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    અચલ unsigned old_flags = _mm_getcsr();
    અચલ unsigned old_કિંમત = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ખાલી, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_કિંમત;
    _mm_setcsr(flags);
#નહીંતર
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

પૂર્ણાંક saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#જો OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    જો (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    અચલ unsigned old_flags = _mm_getcsr();
    અચલ unsigned old_કિંમત = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_કિંમત;
    પાછા 2;
#નહીંતર
    CV_UNUSED(state);
    પાછા 0;
#endif
}

તાર્કિક restoreFPDenormalsState(અચલ FPDenormalsModeState& state)
{
#જો OPENCV_IMPL_FP_HINTS_X86
    અચલ unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    અચલ unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((પૂર્ણાંક)value, value == (value & mask), "invalid SSE FP state");
    અચલ unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ખાલી, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    પાછા સાચું;
#નહીંતર
    CV_UNUSED(state);
    પાછા ખોટું;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    પાછા OPENCV_ALGO_HINT_DEFAULT;
#નહીંતર
    પાછા ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
