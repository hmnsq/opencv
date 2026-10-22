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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#ଅନ୍ତର୍ଭାଵ <atomic>
#ଅନ୍ତର୍ଭାଵ <exception>
#ଅନ୍ତର୍ଭାଵ <iostream>
#ଅନ୍ତର୍ଭାଵ <ostream>

#ifdef __QNX__
    #ଅନ୍ତର୍ଭାଵ <unistd.h>
    #ଅନ୍ତର୍ଭାଵ <sys/neutrino.h>
    #ଅନ୍ତର୍ଭାଵ <sys/syspage.h>
#ifdef __aarch64__
    #ଅନ୍ତର୍ଭାଵ <aarch64/syspage.h>
#endif
#endif

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/trace.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/tls.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/instrumentation.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/filesystem.private.hpp>

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/fp_control_utils.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/fp_control.private.hpp>

ନାମାକାଶ cv {

ସ୍ଥିର ଶୂନ୍ୟ _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ସ୍ଥିର std::ios_base::Init s_iostream_initializer;
#endif
}

ସ୍ଥିର Mutex* __initialization_mutex = ଖାଲି;
Mutex& getInitializationMutex()
{
    ଯଦି (__initialization_mutex == ଖାଲି)
    {
        (ଶୂନ୍ୟ)_initSystem();
        __initialization_mutex = ନୂତନ Mutex();
    }
    ଫେରନ୍ତୁ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ସ୍ଥିର ତାର୍କିକ param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ଯଦି defined(_DEBUG) || defined(__ANDROID__)
    ସତ୍ୟ
#ଅନ୍ୟଥା
    ମିଥ୍ୟା
#endif
);

ଶୂନ୍ୟ* allocSingletonBuffer(size_t size) { ଫେରନ୍ତୁ fastMalloc(size); }
ଶୂନ୍ୟ* allocSingletonNewBuffer(size_t size) { ଫେରନ୍ତୁ malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# ଯଦି defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#ଯଦି defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# ଯଦି _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#ଅନ୍ତର୍ଭାଵ <cstdlib>        // std::abort
#endif

#ଯଦି defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ଯଦି defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ଅଚଳ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ଅନ୍ୟଥା
#  include <elf.h>
#endif
#ଯଦି defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#ଯଦି defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#ଯଦି ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#ଯଦି defined __loongarch64
#ଅନ୍ତର୍ଭାଵ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#ଯଦି defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#ଅନ୍ତର୍ଭାଵ <windows.h>
#ଅନ୍ତର୍ଭାଵ <combaseapi.h>
#ଯଦି (_WIN32_WINNT >= 0x0602)
  #ଅନ୍ତର୍ଭାଵ <synchapi.h>
#endif
#ଯଦି ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ଅନ୍ତର୍ଭାଵ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#ଅନ୍ତର୍ଭାଵ <tchar.h>

#ifdef WINRT
#ଅନ୍ତର୍ଭାଵ <wrl/client.h>
#ifndef __cplusplus_winrt
#ଅନ୍ତର୍ଭାଵ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    ଫେରନ୍ତୁ std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#ଅନ୍ୟଥା
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    ଯଦି (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        ଫେରନ୍ତୁ wstr;
    ଯଦି (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        ଫେରନ୍ତୁ wstr;
    ଯଦି (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        ଫେରନ୍ତୁ wstr;
    ଯଦି (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        ଫେରନ୍ତୁ wstr;
    ଯଦି (FAILED(storagefolderRef.As(&storageitemRef)))
        ଫେରନ୍ତୁ wstr;
    str = ଖାଲି;
    ଯଦି (FAILED(storageitemRef->get_Path(&str)))
        ଫେରନ୍ତୁ wstr;
    wstr = WindowsGetStringRawBuffer(str, ଖାଲି);
    WindowsDeleteString(str);
    ଫେରନ୍ତୁ wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], ଆକାର(guidStr)/ଆକାର(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ଫେରନ୍ତୁ prefix.append(std::wstring(guidStr));
}

#endif
#ଅନ୍ୟଥା
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#ଅନ୍ତର୍ଭାଵ <pthread.h>
#endif
#ଅନ୍ତର୍ଭାଵ <sys/time.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଯଦି defined __MACH__ && defined __APPLE__
#ଅନ୍ତର୍ଭାଵ <mach/mach.h>
#ଅନ୍ତର୍ଭାଵ <mach/mach_time.h>
#ଅନ୍ତର୍ଭାଵ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#ଅନ୍ତର୍ଭାଵ "omp.h"
#endif

#ଯଦି defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଯଦି defined __ANDROID__
#ଅନ୍ତର୍ଭାଵ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #ଯଦି defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ଯଦି _MSC_VER >= 1400  // MSVS 2005
      #ଅନ୍ତର୍ଭାଵ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #ଅନ୍ୟଥା
      #error "Required MSVS 2005+"
    #endif
  #ନଚେତଯଦି defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ସ୍ଥିର ଶୂନ୍ୟ cv_cpuid(ପୂର୍ଣ୍ଣସଂଖ୍ୟା* cpuid_data, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reg_eax, ପୂର୍ଣ୍ଣସଂଖ୍ୟା reg_ecx)
    {
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ଯଦି !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ନଚେତଯଦି defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ଅନ୍ୟଥା
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#ଅନ୍ତର୍ଭାଵ <chrono>

ନାମାକାଶ cv
{

Exception::Exception() { code = Error::StsOk; ଧାଡ଼ି = 0; }

Exception::Exception(Error::Code _code, ଅଚଳ String& _err, ଅଚଳ String& _func, ଅଚଳ String& _file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_3)
: code(_code), err(_err), func(_func), file(_file), ଧାଡ଼ି(ଧାଡ଼ି_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ଅଚଳ ଅକ୍ଷର* Exception::what() ଅଚଳ CV_NOEXCEPT { ଫେରନ୍ତୁ ସନ୍ଦେଶ.c_str(); }

ଶୂନ୍ୟ Exception::formatMessage()
{
    size_t pos = err.find('\n');
    ତାର୍କିକ multiline = pos != cv::String::npos;
    ଯଦି (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ଯେତେବେଳେ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ଯଦି (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    ଯଦି (func.size() > 0)
    {
        ଯଦି (multiline)
            ସନ୍ଦେଶ = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), ଧାଡ଼ି, code, codeMessage(), func.c_str(), err.c_str());
        ଅନ୍ୟଥା
            ସନ୍ଦେଶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), ଧାଡ଼ି, code, codeMessage(), err.c_str(), func.c_str());
    }
    ଅନ୍ୟଥା
    {
        ସନ୍ଦେଶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), ଧାଡ଼ି, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ଅଚଳ ଅକ୍ଷର * Exception::codeMessage() ଅଚଳ
{
    ବାଛ (code)
    {
    ସ୍ଥିତି cv::Error::StsOk :                  ଫେରନ୍ତୁ "No Error";
    ସ୍ଥିତି cv::Error::StsBackTrace :           ଫେରନ୍ତୁ "Backtrace";
    ସ୍ଥିତି cv::Error::StsError :               ଫେରନ୍ତୁ "Unspecified error";
    ସ୍ଥିତି cv::Error::StsInternal :            ଫେରନ୍ତୁ "Internal error";
    ସ୍ଥିତି cv::Error::StsNoMem :               ଫେରନ୍ତୁ "Insufficient memory";
    ସ୍ଥିତି cv::Error::StsBadArg :              ଫେରନ୍ତୁ "Bad argument";
    ସ୍ଥିତି cv::Error::StsNoConv :              ଫେରନ୍ତୁ "Iterations do not converge";
    ସ୍ଥିତି cv::Error::StsAutoTrace :           ଫେରନ୍ତୁ "Autotrace call";
    ସ୍ଥିତି cv::Error::StsBadSize :             ଫେରନ୍ତୁ "Incorrect size of input array";
    ସ୍ଥିତି cv::Error::StsNullPtr :             ଫେରନ୍ତୁ "Null pointer";
    ସ୍ଥିତି cv::Error::StsDivByZero :           ଫେରନ୍ତୁ "Division by zero occurred";
    ସ୍ଥିତି cv::Error::BadStep :                ଫେରନ୍ତୁ "Image step is wrong";
    ସ୍ଥିତି cv::Error::StsInplaceNotSupported : ଫେରନ୍ତୁ "Inplace operation is not supported";
    ସ୍ଥିତି cv::Error::StsObjectNotFound :      ଫେରନ୍ତୁ "Requested object was not found";
    ସ୍ଥିତି cv::Error::BadDepth :               ଫେରନ୍ତୁ "Input image depth is not supported by function";
    ସ୍ଥିତି cv::Error::StsUnmatchedFormats :    ଫେରନ୍ତୁ "Formats of input arguments do not match";
    ସ୍ଥିତି cv::Error::StsUnmatchedSizes :      ଫେରନ୍ତୁ "Sizes of input arguments do not match";
    ସ୍ଥିତି cv::Error::StsOutOfRange :          ଫେରନ୍ତୁ "One of the arguments\' values is out of range";
    ସ୍ଥିତି cv::Error::StsUnsupportedFormat :   ଫେରନ୍ତୁ "Unsupported format or combination of formats";
    ସ୍ଥିତି cv::Error::BadCOI :                 ଫେରନ୍ତୁ "Input COI is not supported";
    ସ୍ଥିତି cv::Error::Bad_ଅଙ୍କ_Channels :         ଫେରନ୍ତୁ "Bad number of channels";
    ସ୍ଥିତି cv::Error::StsBadFlag :             ଫେରନ୍ତୁ "Bad flag (parameter or structure field)";
    ସ୍ଥିତି cv::Error::StsBadPoint :            ଫେରନ୍ତୁ "Bad parameter of type CvPoint";
    ସ୍ଥିତି cv::Error::StsBadMask :             ଫେରନ୍ତୁ "Bad type of mask argument";
    ସ୍ଥିତି cv::Error::StsParseError :          ଫେରନ୍ତୁ "Parsing error";
    ସ୍ଥିତି cv::Error::StsNotImplemented :      ଫେରନ୍ତୁ "The function/feature is not implemented";
    ସ୍ଥିତି cv::Error::StsBadMemBlock :         ଫେରନ୍ତୁ "Memory block has been corrupted";
    ସ୍ଥିତି cv::Error::StsAssert :              ଫେରନ୍ତୁ "Assertion failed";
    ସ୍ଥିତି cv::Error::GpuNotSupported :        ଫେରନ୍ତୁ "No CUDA support";
    ସ୍ଥିତି cv::Error::GpuApiCallError :        ଫେରନ୍ତୁ "Gpu API call";
    ସ୍ଥିତି cv::Error::OpenGlNotSupported :     ଫେରନ୍ତୁ "No OpenGL support";
    ସ୍ଥିତି cv::Error::OpenGlApiCallError :     ଫେରନ୍ତୁ "OpenGL API call";
    ସ୍ଥିତି cv::Error::OpenCLApiCallError :     ଫେରନ୍ତୁ "OpenCL API call error";
    ସ୍ଥିତି cv::Error::OpenCLDoubleNotSupported:ଫେରନ୍ତୁ "OpenCL device does not support double";
    ସ୍ଥିତି cv::Error::OpenCLInitError :        ଫେରନ୍ତୁ "OpenCL initialization error";
    ସ୍ଥିତି cv::Error::OpenCLNoAMDBlasFft :     ଫେରନ୍ତୁ "Missing OpenCL AMD BLAS FFT";
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
    {
        ସ୍ଥିର ଅକ୍ଷର buf[256] {0};
        snprintf(buf, ଆକାର(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        ଫେରନ୍ତୁ buf;
    }
    };

}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { ଖାଲି };

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* getHWFeatureName(ପୂର୍ଣ୍ଣସଂଖ୍ୟା id)
{
    ଫେରନ୍ତୁ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : ଖାଲି;
}
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* getHWFeatureNameSafe(ପୂର୍ଣ୍ଣସଂଖ୍ୟା id)
{
    ଅଚଳ ଅକ୍ଷର* name = getHWFeatureName(id);
    ଫେରନ୍ତୁ name ? name : "Unknown feature";
}

ସଂରଚନା HWFeatures
{
    ଗଣନା { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(ତାର୍କିକ run_initialize = ମିଥ୍ୟା)
    {
        ଯଦି (run_initialize)
            initialize();
    }

    ସ୍ଥିର ଶୂନ୍ୟ initializeNames()
    {
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    ଶୂନ୍ୟ initialize(ଶୂନ୍ୟ)
    {
        ଯଦି (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuid_data[4] = { 0, 0, 0, 0 };
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା x86_family = (cpuid_data[0] >> 8) & 15;
        ଯଦି( x86_family >= 6 )
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
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            ତାର୍କିକ have_AVX_OS_support = ସତ୍ୟ;
            ତାର୍କିକ have_AVX512_OS_support = ସତ୍ୟ;
            ଯଦି (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = ମିଥ୍ୟା; // OS uses XSAVE_XRSTORE and CPU support AVX
            ଅନ୍ୟଥା
            {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #ନଚେତଯଦି defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                ଯଦି ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = ମିଥ୍ୟା; // YMM registers
                ଯଦି ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = ମିଥ୍ୟା; // ZMM registers
            }

            ଯଦି (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = ମିଥ୍ୟା;
                have[CV_CPU_FP16] = ମିଥ୍ୟା;
                have[CV_CPU_AVX2] = ମିଥ୍ୟା;
                have[CV_CPU_FMA3] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_VNNI] = ମିଥ୍ୟା;
            }
            ଯଦି (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512BW] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512CD] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512DQ] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512ER] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512IFMA] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512PF] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512VBMI] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512VL] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512VBMI2] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512VNNI] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512BITALG] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_512VPOPCNTDQ] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_5124VNNIW] = ମିଥ୍ୟା;
                have[CV_CPU_AVX_5124FMAPS] = ମିଥ୍ୟା;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ଯଦି (have[CV_CPU_AVX512_COMMON])
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
            ଅନ୍ୟଥା
            {
                have[CV_CPU_AVX512_KNL] = ମିଥ୍ୟା;
                have[CV_CPU_AVX512_KNM] = ମିଥ୍ୟା;
                have[CV_CPU_AVX512_SKX] = ମିଥ୍ୟା;
                have[CV_CPU_AVX512_CNL] = ମିଥ୍ୟା;
                have[CV_CPU_AVX512_CLX] = ମିଥ୍ୟା;
                have[CV_CPU_AVX512_ICL] = ମିଥ୍ୟା;
            }
        }
    #endif // CV_CPUID_X86

    #ଯଦି defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = ସତ୍ୟ;
        have[CV_CPU_FP16] = ସତ୍ୟ;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpufile = open("/proc/self/auxv", O_RDONLY);

        ଯଦି (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ଅଚଳ size_t size_auxv_t = ଆକାର(auxv);

            ଯେତେବେଳେ ((size_t)ପଠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ଯଦି (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ଯଦି defined(AT_HWCAP2)
                ଅନ୍ୟଥା ଯଦି (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #ନଚେତଯଦି defined __arm__ && defined __ANDROID__
      #ଯଦି defined HAVE_CPUFEATURES
        CV_LOG_INFO(ଖାଲି, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ଖାଲି, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ଅନ୍ୟଥା
        CV_LOG_INFO(ଖାଲି, "cpufeatures library is not available for CPU detection");
        #ଯଦି CV_NEON
        CV_LOG_INFO(ଖାଲି, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = ସତ୍ୟ;
        #ଅନ୍ୟଥା
        CV_LOG_INFO(ଖାଲି, "- NEON instructions is NOT enabled via build flags");
        #endif
        #ଯଦି CV_FP16
        CV_LOG_INFO(ଖାଲି, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = ସତ୍ୟ;
        #ଅନ୍ୟଥା
        CV_LOG_INFO(ଖାଲି, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #ନଚେତଯଦି defined __arm__
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା cpufile = open("/proc/self/auxv", O_RDONLY);

        ଯଦି (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ଅଚଳ size_t size_auxv_t = ଆକାର(auxv);

            ଯେତେବେଳେ ((size_t)ପଠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                ଯଦି (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ଭାଙ୍ଗ;
                }
            }

            close(cpufile);
        }
    #endif
    #ନଚେତଯଦି (defined __APPLE__)
    #ଯଦି defined __ARM_NEON
        have[CV_CPU_NEON] = ସତ୍ୟ;
    #endif
    #ଯଦି (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = ସତ୍ୟ;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = ଆକାର(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, ଖାଲି, 0);
    ଯଦି (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = ସତ୍ୟ;
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା has_feat_bf16 = 0;
    size_t has_feat_bf16_size = ଆକାର(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, ଖାଲି, 0);
    ଯଦି (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = ସତ୍ୟ;
    }
    #ନଚେତଯଦି (defined __clang__)
    #ଯଦି defined __ARM_NEON
        have[CV_CPU_NEON] = ସତ୍ୟ;
        #ଯଦି (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = ସତ୍ୟ;
        #endif
    #endif
    #endif
    #ଯଦି defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = ସତ୍ୟ;
    #endif
    #ଯଦି defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = ସତ୍ୟ;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = ସତ୍ୟ;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = ସତ୍ୟ;
    #endif

    #ଯଦି (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା hwcap = getauxval(AT_HWCAP);
        ଯଦି (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ଯଦି (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ସତ୍ୟ;
            } ଅନ୍ୟଥା {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ନଚେତଯଦି (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        ଯଦି (elf_aux_info(AT_HWCAP, &hwcap, ଆକାର(hwcap)) != 0)
            hwcap = 0;
        ଯଦି (hwcap & PPC_FEATURE_HAS_VSX) {
            ଯଦି (elf_aux_info(AT_HWCAP2, &hwcap2, ଆକାର(hwcap2)) != 0)
                hwcap2 = 0;
            ଯଦି (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ସତ୍ୟ;
            } ଅନ୍ୟଥା {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ଅନ୍ୟଥା
        // TODO: AIX
        #ଯଦି CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = ସତ୍ୟ;
        #endif
        #ଯଦି CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = ସତ୍ୟ;
        #endif
    #endif

    #ଯଦି defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = ସତ୍ୟ;
        #ଯଦି (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = ସତ୍ୟ;
        #endif
        #ଯଦି defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = ସତ୍ୟ;
        #endif
    #endif

    #ଯଦି defined __loongarch64 && defined __linux__
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        ତାର୍କିକ skip_baseline_check = ମିଥ୍ୟା;
        ଯଦି (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = ସତ୍ୟ;
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ଯଦି (!checkFeatures(baseline_features, ଆକାର(baseline_features) / ଆକାର(baseline_features[0]))
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
            checkFeatures(baseline_features, ଆକାର(baseline_features) / ଆକାର(baseline_features[0]), ସତ୍ୟ);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, ଆକାର(baseline_features) / ଆକାର(baseline_features[0]));
    }

    ତାର୍କିକ checkFeatures(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* features, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3, ତାର୍କିକ dump = ମିଥ୍ୟା)
    {
        ତାର୍କିକ result = ସତ୍ୟ;
        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < ଗଣନା_3; i++)
        {
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature = features[i];
            ଯଦି (feature)
            {
                ଯଦି (have[feature])
                {
                    ଯଦି (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                ଅନ୍ୟଥା
                {
                    result = ମିଥ୍ୟା;
                    ଯଦି (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        ଫେରନ୍ତୁ result;
    }

    ସ୍ଥିର ଧାଡ଼ିଭିତରେ ତାର୍କିକ is_ପ୍ରତୀକ_Separator(ଅକ୍ଷର c)
    {
        ଫେରନ୍ତୁ c == ',' || c == ';';
    }

    ଶୂନ୍ୟ readSettings(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* baseline_features, ପୂର୍ଣ୍ଣସଂଖ୍ୟା baseline_ଗଣନା)
    {
        ତାର୍କିକ dump = ସତ୍ୟ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ଯଦି (!disabled_features.empty())
        {
            ଅଚଳ ଅକ୍ଷର* start = disabled_features.c_str();
            ପାଇଁ (;;)
            {
                ଯେତେବେଳେ (start[0] != 0 && is_ପ୍ରତୀକ_Separator(start[0]))
                {
                    start++;
                }
                ଯଦି (start[0] == 0)
                    ଭାଙ୍ଗ;
                ଅଚଳ ଅକ୍ଷର* end = start;
                ଯେତେବେଳେ (end[0] != 0 && !is_ପ୍ରତୀକ_Separator(end[0]))
                {
                    end++;
                }
                ଯଦି (end == start)
                    ଜାରି;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                ତାର୍କିକ found = ମିଥ୍ୟା;
                ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ଯଦି (!g_hwFeatureNames[i]) ଜାରି;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    ଯଦି (len != feature.size()) ଜାରି;
                    ଯଦି (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        ତାର୍କିକ isBaseline = ମିଥ୍ୟା;
                        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 0; k < baseline_ଗଣନା; k++)
                        {
                            ଯଦି (baseline_features[k] == i)
                            {
                                isBaseline = ସତ୍ୟ;
                                ଭାଙ୍ଗ;
                            }
                        }
                        ଯଦି (isBaseline)
                        {
                            ଯଦି (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        ଯଦି (!have[i])
                        {
                            ଯଦି (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = ମିଥ୍ୟା;

                        found = ସତ୍ୟ;
                        ଭାଙ୍ଗ;
                    }
                }
                ଯଦି (!found)
                {
                    ଯଦି (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    ତାର୍କିକ have[MAX_FEATURE+1]{};
};

ସ୍ଥିର HWFeatures  featuresEnabled(ସତ୍ୟ), featuresDisabled = HWFeatures(ମିଥ୍ୟା);
ସ୍ଥିର HWFeatures* currentFeatures = &featuresEnabled;

ତାର୍କିକ checkHardwareSupport(ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature)
{
    ଯଦି (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ currentFeatures->have[feature];
}

String getHardwareFeatureName(ପୂର୍ଣ୍ଣସଂଖ୍ୟା feature)
{
    ଅଚଳ ଅକ୍ଷର* name = getHWFeatureName(feature);
    ଫେରନ୍ତୁ name ? String(name) : String();
}

std::string get_CPUFeatures_ଧାଡ଼ି()
{
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା sz = ଆକାର(features) / ଆକାର(features[0]);
    std::string result;
    std::string prefix;
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 1; i < sz; ++i)
    {
        ଯଦି (features[i] == 0)
        {
            prefix = "*";
            ଜାରି;
        }
        ଯଦି (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        ଯଦି (!checkHardwareSupport(features[i])) result.append("?");
    }
    ଫେରନ୍ତୁ result;
}

ଅସ୍ଥିର ତାର୍କିକ useOptimizedFlag = ସତ୍ୟ;

ଶୂନ୍ୟ setUseOptimized( ତାର୍କିକ flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

ତାର୍କିକ useOptimized(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ useOptimizedFlag;
}

int64 get_Tick_ଗଣନା(ଶୂନ୍ୟ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ଫେରନ୍ତୁ (int64)now.time_since_epoch().ଗଣନା_3();
}

ଦ୍ୱିଗୁଣ getTickFrequency(ଶୂନ୍ୟ)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    ଦ୍ୱିଗୁଣ clock_freq = clock_period_t::den / clock_period_t::num;
    ଫେରନ୍ତୁ clock_freq;
}

#ଯଦି defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ଯଦି defined(__i386__)

int64 get_CPUTick_ଗଣନା(ଶୂନ୍ୟ)
{
    int64 x;
    __asm__ ଅସ୍ଥିର (".byte 0x0f, 0x31" : "=A" (x));
    ଫେରନ୍ତୁ x;
}
#ନଚେତଯଦି defined(__x86_64__)

int64 get_CPUTick_ଗଣନା(ଶୂନ୍ୟ)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ଫେରନ୍ତୁ (int64)lo | ((int64)hi << 32);
}

#ନଚେତଯଦି defined(__ppc__)

int64 get_CPUTick_ଗଣନା(ଶୂନ୍ୟ)
{
    unsigned upper, lower, tmp;
    __asm__ ଅସ୍ଥିର(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ଫେରନ୍ତୁ lower | ((int64)upper << 32);
}

#ଅନ୍ୟଥା

#error "RDTSC not defined"

#endif

#ନଚେତଯଦି defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ଗଣନା(ଶୂନ୍ୟ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ଅନ୍ୟଥା

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_ଗଣନା(ଶୂନ୍ୟ)
{
    ଫେରନ୍ତୁ get_Tick_ଗଣନା();
}
//#endif

#endif


ନାମାକାଶ internal {

ଶ୍ରେଣୀ Timestamp
{
ସର୍ବସାଧାରଣ:
    ଅଚଳ int64 zero_Tick_ଗଣନା;
    ଅଚଳ ଦ୍ୱିଗୁଣ ns_in_ticks;

    Timestamp()
        : zero_Tick_ଗଣନା(get_Tick_ଗଣନା())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ଗଣନା();
        ଫେରନ୍ତୁ (int64)((t - zero_Tick_ଗଣନା) * ns_in_ticks);
    }

    ସ୍ଥିର Timestamp& getInstance()
    {
        ସ୍ଥିର Timestamp g_timestamp;
        ଫେରନ୍ତୁ g_timestamp;
    }
};

ଶ୍ରେଣୀ InitTimestamp {
ସର୍ବସାଧାରଣ:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ସ୍ଥିର InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ଫେରନ୍ତୁ internal::Timestamp::getInstance().getTimestamp();
}


ଅଚଳ String& getBuildInformation()
{
    ସ୍ଥିର String build_info =
#ଅନ୍ତର୍ଭାଵ "version_string.inc"
    ;
    ଫେରନ୍ତୁ build_info;
}

String getVersionString() { ଫେରନ୍ତୁ String(CV_VERSION); }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା getVersionMajor() { ଫେରନ୍ତୁ CV_VERSION_MAJOR; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା getVersionMinor() { ଫେରନ୍ତୁ CV_VERSION_MINOR; }

ପୂର୍ଣ୍ଣସଂଖ୍ୟା getVersionRevision() { ଫେରନ୍ତୁ CV_VERSION_REVISION; }

String format( ଅଚଳ ଅକ୍ଷର* fmt, ... )
{
    AutoBuffer<ଅକ୍ଷର, 1024> buf;

    ପାଇଁ ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା bsize = static_cast<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>(buf.size());
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ଯଦି (len >= bsize)
        {
            buf.resize(len + 1);
            ଜାରି;
        }
        buf[bsize - 1] = 0;
        ଫେରନ୍ତୁ String(buf.data(), len);
    }
}

String tempfile( ଅଚଳ ଅକ୍ଷର* suffix )
{
#ଯଦି OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ଯଦି defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    ଯଦି (temp_file.empty())
        ଫେରନ୍ତୁ String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    ଅକ୍ଷର aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#ନଚେତଯଦି defined(_WIN32_WCE)
    ଅଚଳ auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    ଯଦି(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        ଅକ୍ଷର aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ଅନ୍ୟଥା
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    ଯଦି (temp_dir.empty())
    {
        ::GetTempPathW(ଆକାର(temp_dir2_w)/ଆକାର(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, ଖାଲି, 0, ଖାଲି, ଖାଲି);
        ଯଦି (len > 0)
        {
            std::vector<ଅକ୍ଷର> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, ଖାଲି, ଖାଲି);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ଯଦି (FAILED(hr))
        ଫେରନ୍ତୁ String();
    ଅକ୍ଷର guidStr[40];
    ଅଚଳ ଅକ୍ଷର* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, ଆକାର(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[0], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[1],
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[2], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[3], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[4],
            (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[5], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[6], (unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା)g.Data4[7]);

    fname = temp_dir;
    ଯଦି (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# ଅନ୍ୟଥା
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    ଅକ୍ଷର defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ଅନ୍ୟଥା
    ଅକ୍ଷର defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ଯଦି (temp_dir.empty())
        fname = defaultTemplate;
    ଅନ୍ୟଥା
    {
        fname = temp_dir;
        ଅକ୍ଷର ech = fname[fname.size() - 1];
        ଯଦି(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = mkstemp((ଅକ୍ଷର*)fname.c_str());
    ଯଦି (fd == -1) ଫେରନ୍ତୁ String();

    close(fd);
    remove(fname.c_str());
# endif

    ଯଦି (suffix)
    {
        ଯଦି (suffix[0] != '.')
            ଫେରନ୍ତୁ fname + "." + suffix;
        ଅନ୍ୟଥା
            ଫେରନ୍ତୁ fname + suffix;
    }
    ଫେରନ୍ତୁ fname;
#ଅନ୍ୟଥା // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ସ୍ଥିର ErrorCallback customErrorCallback = 0;
ସ୍ଥିର ଶୂନ୍ୟ* customErrorCallbackData = 0;
ସ୍ଥିର ତାର୍କିକ breakOnError = ମିଥ୍ୟା;

ତାର୍କିକ setBreakOnError(ତାର୍କିକ value)
{
    ତାର୍କିକ prevVal = breakOnError;
    breakOnError = value;
    ଫେରନ୍ତୁ prevVal;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv_snprintf(ଅକ୍ଷର* buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଅକ୍ଷର* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ଫେରନ୍ତୁ res;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv_vsnprintf(ଅକ୍ଷର* buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଅକ୍ଷର* fmt, va_list args)
{
#ଯଦି defined _MSC_VER
    ଯଦି (len <= 0) ଫେରନ୍ତୁ len == 0 ? 1024 : -1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    ଯଦି (res >= 0 && res < len)
    {
        buf[res] = 0;
        ଫେରନ୍ତୁ res;
    }
    ଅନ୍ୟଥା
    {
        buf[len - 1] = 0; // truncate happened
        ଫେରନ୍ତୁ res >= len ? res : (len * 2);
    }
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ vsnprintf(buf, len, fmt, args);
#endif
}

ସ୍ଥିର ଶୂନ୍ୟ dumpException(ଅଚଳ Exception& exc)
{
    ଅଚଳ ଅକ୍ଷର* errorStr = exc.codeMessage();
    ଅକ୍ଷର buf[1 << 12];

    cv_snprintf(buf, ଆକାର(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.ଧାଡ଼ି);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ଅନ୍ୟଥା
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
ସ୍ଥିର ତାର୍କିକ cv_terminate_handler_installed = ମିଥ୍ୟା;
ସ୍ଥିର std::terminate_handler cv_old_terminate_handler;
ସ୍ଥିର cv::Exception cv_terminate_handler_exception;
ସ୍ଥିର ତାର୍କିକ param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", ସତ୍ୟ);
ସ୍ଥିର ଶୂନ୍ୟ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ଯଦି (ମିଥ୍ୟା /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# ଯଦି defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

ଶୂନ୍ୟ error( ଅଚଳ Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        ଯଦି (!cv_terminate_handler_installed)
        {
            ଯଦି (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = ସତ୍ୟ;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    ଯଦି (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.ଧାଡ଼ି, customErrorCallbackData);
    ଅନ୍ୟଥା ଯଦି (param_dumpErrors)
    {
        dumpException(exc);
    }

    ଯଦି(breakOnError)
    {
        std::terminate();
    }

    ଫିଙ୍ଗ exc;
#ifdef __GNUC__
# ଯଦି !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

ଶୂନ୍ୟ error(Error::Code _code, ଅଚଳ String& _err, ଅଚଳ ଅକ୍ଷର* _func, ଅଚଳ ଅକ୍ଷର* _file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_3)
{
    error(cv::Exception(_code, _err, _func, _file, ଧାଡ଼ି_3));
#ifdef __GNUC__
# ଯଦି !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# ଯଦି defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, ଶୂନ୍ୟ* userdata, ଶୂନ୍ୟ** prevUserdata)
{
    ଯଦି( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    ଫେରନ୍ତୁ prevCallback;
}

ଶୂନ୍ୟ terminate(Error::Code _code, ଅଚଳ String& _err, ଅଚଳ ଅକ୍ଷର* _func, ଅଚଳ ଅକ୍ଷର* _file, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, ଧାଡ଼ି_3));
    std::terminate();
}

}

ନାମାକାଶ cv {
ତାର୍କିକ __termination = ମିଥ୍ୟା;


//////////////////////////////// thread-local storage ////////////////////////////////

ନାମାକାଶ details {

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
ଶ୍ରେଣୀ TlsAbstraction
{
ସର୍ବସାଧାରଣ:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    ଶୂନ୍ୟ* getData() ଅଚଳ;
    ଶୂନ୍ୟ setData(ଶୂନ୍ୟ *pData);

    ଶୂନ୍ୟ releaseSystemResources();

ବ୍ୟକ୍ତିଗତ:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_ଚାବି;
    ତାର୍କିକ disposed;
#endif
#ଅନ୍ୟଥା // _WIN32
    pthread_key_t  tls_ଚାବି;
    std::atomic<ତାର୍କିକ> disposed;
#endif
};

ଶ୍ରେଣୀ TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
ସର୍ବସାଧାରଣ:
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
ସ୍ଥିର TlsAbstraction* getTlsAbstraction()
{
    ସ୍ଥିର TlsAbstraction *g_tls = ନୂତନ TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ସ୍ଥିର TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ଫେରନ୍ତୁ g_tls;
}


#ifdef _WIN32
#ifdef WINRT
ସ୍ଥିର __declspec( thread ) ଶୂନ୍ୟ* tlsData = ଖାଲି; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
ଶୂନ୍ୟ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ସତ୍ୟ;  // DllMain is missing in static builds
}
ଶୂନ୍ୟ* TlsAbstraction::getData() ଅଚଳ
{
    ଫେରନ୍ତୁ tlsData;
}
ଶୂନ୍ୟ TlsAbstraction::setData(ଶୂନ୍ୟ *pData)
{
    tlsData = pData;
}
#ଅନ୍ୟଥା //WINRT
#ifdef CV_USE_FLS
ସ୍ଥିର ଶୂନ୍ୟ NTAPI opencv_fls_destructor(ଶୂନ୍ୟ* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(ମିଥ୍ୟା)
{
#ifndef CV_USE_FLS
    tls_ଚାବି = TlsAlloc();
#ଅନ୍ୟଥା // CV_USE_FLS
    tls_ଚାବି = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_ଚାବି != TLS_OUT_OF_INDEXES);
}
ଶୂନ୍ୟ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ସତ୍ୟ;  // DllMain is missing in static builds
    disposed = ସତ୍ୟ;
#ifndef CV_USE_FLS
    TlsFree(tls_ଚାବି);
#ଅନ୍ୟଥା // CV_USE_FLS
    FlsFree(tls_ଚାବି);
#endif // CV_USE_FLS
    tls_ଚାବି = TLS_OUT_OF_INDEXES;
}
ଶୂନ୍ୟ* TlsAbstraction::getData() ଅଚଳ
{
    ଯଦି (disposed)
        ଫେରନ୍ତୁ ଖାଲି;
#ifndef CV_USE_FLS
    ଫେରନ୍ତୁ Tls_Get_ମୂଲ୍ୟ(tls_ଚାବି);
#ଅନ୍ୟଥା // CV_USE_FLS
    ଫେରନ୍ତୁ Fls_Get_ମୂଲ୍ୟ(tls_ଚାବି);
#endif // CV_USE_FLS
}
ଶୂନ୍ୟ TlsAbstraction::setData(ଶୂନ୍ୟ *pData)
{
    ଯଦି (disposed)
        ଫେରନ୍ତୁ;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_ମୂଲ୍ୟ(tls_ଚାବି, pData) == TRUE);
#ଅନ୍ୟଥା // CV_USE_FLS
    CV_Assert(Fls_Set_ମୂଲ୍ୟ(tls_ଚାବି, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#ଅନ୍ୟଥା // _WIN32
ସ୍ଥିର ଶୂନ୍ୟ opencv_tls_destructor(ଶୂନ୍ୟ* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(ମିଥ୍ୟା)
{
    CV_Assert(pthread_key_create(&tls_ଚାବି, opencv_tls_destructor) == 0);
}
ଶୂନ୍ୟ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ସତ୍ୟ;  // DllMain is missing in static builds
    disposed = ସତ୍ୟ;
    ଯଦି (pthread_key_delete(tls_ଚାବି) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
ଶୂନ୍ୟ* TlsAbstraction::getData() ଅଚଳ
{
    ଯଦି (disposed)
        ଫେରନ୍ତୁ ଖାଲି;
    ଫେରନ୍ତୁ pthread_getspecific(tls_ଚାବି);
}
ଶୂନ୍ୟ TlsAbstraction::setData(ଶୂନ୍ୟ *pData)
{
    ଯଦି (disposed)
        ଫେରନ୍ତୁ;  // no-op
    CV_Assert(pthread_setspecific(tls_ଚାବି, pData) == 0);
}
#endif

// Per-thread data structure
ସଂରଚନା ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<ଶୂନ୍ୟ*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ସ୍ଥିର ତାର୍କିକ g_isTlsStorageInitialized = ମିଥ୍ୟା;

// Main TLS storage class
ଶ୍ରେଣୀ TlsStorage
{
ସର୍ବସାଧାରଣ:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (ଶୂନ୍ୟ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = ସତ୍ୟ;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    ଶୂନ୍ୟ releaseThread(ଶୂନ୍ୟ* tls_ମୂଲ୍ୟ = ଖାଲି)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ଯଦି (ଖାଲି == tls)
            ଫେରନ୍ତୁ;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_ମୂଲ୍ୟ == ଖାଲି ? (ThreadData*)tls->getData() : (ThreadData*)tls_ମୂଲ୍ୟ;
        ଯଦି (pTD == ଖାଲି)
            ଫେରନ୍ତୁ;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        ପାଇଁ (size_t i = 0; i < threads.size(); i++)
        {
            ଯଦି (pTD == threads[i])
            {
                threads[i] = ଖାଲି;
                ଯଦି (tls_ମୂଲ୍ୟ == ଖାଲି)
                    tls->setData(0);
                std::vector<ଶୂନ୍ୟ*>& thread_slots = pTD->slots;
                ପାଇଁ (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    ଶୂନ୍ୟ* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = ଖାଲି;
                    ଯଦି (!pData)
                        ଜାରି;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    ଯଦି (container)
                        container->deleteDataInstance(pData);
                    ଅନ୍ୟଥା
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                ଫେରନ୍ତୁ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (ଶୂନ୍ୟ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        ପାଇଁ(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            ଯଦି (tlsSlots[slot].container == ଖାଲି)
            {
                tlsSlots[slot].container = container;
                ଫେରନ୍ତୁ slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        ଫେରନ୍ତୁ tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    ଶୂନ୍ୟ releaseSlot(size_t slotIdx, std::vector<ଶୂନ୍ୟ*> &dataVec, ତାର୍କିକ keepSlot = ମିଥ୍ୟା)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        ପାଇଁ(size_t i = 0; i < threads.size(); i++)
        {
            ଯଦି(threads[i])
            {
                std::vector<ଶୂନ୍ୟ*>& thread_slots = threads[i]->slots;
                ଯଦି (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = ଖାଲି;
                }
            }
        }

        ଯଦି (!keepSlot)
        {
            tlsSlots[slotIdx].container = ଖାଲି;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ଶୂନ୍ୟ* getData(size_t slotIdx) ଅଚଳ
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ଯଦି (ଖାଲି == tls)
            ଫେରନ୍ତୁ ଖାଲି;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ଯଦି(threadData && threadData->slots.size() > slotIdx)
            ଫେରନ୍ତୁ threadData->slots[slotIdx];

        ଫେରନ୍ତୁ ଖାଲି;
    }

    // Gather data from threads by TLS storage index
    ଶୂନ୍ୟ gather(size_t slotIdx, std::vector<ଶୂନ୍ୟ*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        ପାଇଁ(size_t i = 0; i < threads.size(); i++)
        {
            ଯଦି(threads[i])
            {
                std::vector<ଶୂନ୍ୟ*>& thread_slots = threads[i]->slots;
                ଯଦି (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    ଶୂନ୍ୟ setData(size_t slotIdx, ଶୂନ୍ୟ* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ଯଦି (ଖାଲି == tls)
            ଫେରନ୍ତୁ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ଯଦି(!threadData)
        {
            threadData = ନୂତନ ThreadData;
            tls->setData((ଶୂନ୍ୟ*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                ତାର୍କିକ found = ମିଥ୍ୟା;
                // Find unused slots
                ପାଇଁ(size_t slot = 0; slot < threads.size(); slot++)
                {
                    ଯଦି (threads[slot] == ଖାଲି)
                    {
                        threadData->idx = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)slot;
                        threads[slot] = threadData;
                        found = ସତ୍ୟ;
                        ଭାଙ୍ଗ;
                    }
                }

                ଯଦି (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        ଯଦି(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, ଖାଲି);
        }
        threadData->slots[slotIdx] = pData;
    }

ବ୍ୟକ୍ତିଗତ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ସଂରଚନା TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ସଂରଚନା TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ସ୍ଥିର TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, ନୂତନ TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
ସ୍ଥିର ଶୂନ୍ୟ opencv_tls_destructor(ଶୂନ୍ୟ* pData)
{
    ଯଦି (!g_isTlsStorageInitialized)
        ଫେରନ୍ତୁ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#ଅନ୍ୟଥା // _WIN32
#ifdef CV_USE_FLS
ସ୍ଥିର ଶୂନ୍ୟ WINAPI opencv_fls_destructor(ଶୂନ୍ୟ* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    ଯଦି (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = ସତ୍ୟ;
    }

    ଯଦି (!g_isTlsStorageInitialized)
        ଫେରନ୍ତୁ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

ସ୍ଥିର TlsStorage* ଅଚଳ g_force_initialization_of_TlsStorage
#ଯଦି defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#ଅନ୍ୟଥା  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
ଶ୍ରେଣୀ TlsStorage
{
ସର୍ବସାଧାରଣ:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        ପାଇଁ (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            ଯଦି (container && s.data)
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
        ପାଇଁ (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            ଯଦି (s.container == ଖାଲି)
            {
                CV_Assert(!s.data);
                s.container = container;
                ଫେରନ୍ତୁ slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        ଫେରନ୍ତୁ slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    ଶୂନ୍ୟ releaseSlot(size_t slotIdx, std::vector<ଶୂନ୍ୟ*> &dataVec, ତାର୍କିକ keepSlot = ମିଥ୍ୟା)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ଶୂନ୍ୟ* data = s.data;
        ଯଦି (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        ଯଦି (!keepSlot)
        {
            s.container = ଖାଲି;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ଶୂନ୍ୟ* getData(size_t slotIdx) ଅଚଳ
    {
        CV_Assert(slotIdx < slots.size());
        ଅଚଳ SlotInfo& s = slots[slotIdx];
        ଫେରନ୍ତୁ s.data;
    }

    // Gather data from threads by TLS storage index
    ଶୂନ୍ୟ gather(size_t slotIdx, std::vector<ଶୂନ୍ୟ*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ଶୂନ୍ୟ* data = s.data;
        ଯଦି (data)
            dataVec.push_back(data);
        ଫେରନ୍ତୁ;
    }

    // Set data to storage index
    ଶୂନ୍ୟ setData(size_t slotIdx, ଶୂନ୍ୟ* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

ବ୍ୟକ୍ତିଗତ:
    ସଂରଚନା SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        ଶୂନ୍ୟ* data;
    };
    std::vector<ସଂରଚନା SlotInfo> slots;
};

ସ୍ଥିର TlsStorage& getTlsStorage()
{
    ସ୍ଥିର TlsStorage g_storage;  // no threading
    ଫେରନ୍ତୁ g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using ନାମାକାଶ details;

ଶୂନ୍ୟ releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    ଯଦି (!g_isTlsStorageInitialized)
        ଫେରନ୍ତୁ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    ଚାବି_2 = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)getTlsStorage().reserveSlot(ଏହି); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ଚାବି_2 == -1); // Key must be released in child object
}

ଶୂନ୍ୟ TLSDataContainer::gatherData(std::vector<ଶୂନ୍ୟ*> &data) ଅଚଳ
{
    getTlsStorage().gather(ଚାବି_2, data);
}

ଶୂନ୍ୟ TLSDataContainer::detachData(std::vector<ଶୂନ୍ୟ*> &data)
{
    getTlsStorage().releaseSlot(ଚାବି_2, data, ସତ୍ୟ);
}

ଶୂନ୍ୟ TLSDataContainer::release()
{
    ଯଦି (ଚାବି_2 == -1)
        ଫେରନ୍ତୁ;  // already released
    std::vector<ଶୂନ୍ୟ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ଚାବି_2, data, ମିଥ୍ୟା); // Release key and get stored data for proper destruction
    ଚାବି_2 = -1;
    ପାଇଁ(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ଶୂନ୍ୟ TLSDataContainer::cleanup()
{
    std::vector<ଶୂନ୍ୟ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ଚାବି_2, data, ସତ୍ୟ); // Extract stored data with removal from TLS tables
    ପାଇଁ(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ଶୂନ୍ୟ* TLSDataContainer::getData() ଅଚଳ
{
    CV_Assert(ଚାବି_2 != -1 && "Can't fetch data from terminated TLS container.");
    ଶୂନ୍ୟ* pData = getTlsStorage().getData(ଚାବି_2); // Check if data was already allocated
    ଯଦି(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        ଚେଷ୍ଟା
        {
            getTlsStorage().setData(ଚାବି_2, pData);
        }
        ଧର (...)
        {
            deleteDataInstance(pData);
            ଫିଙ୍ଗ;
        }
    }
    ଫେରନ୍ତୁ pData;
}

ସ୍ଥିର TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, ନୂତନ TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    ଫେରନ୍ତୁ getCoreTlsDataTLS().getRef();
}

#ଯଦି defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

ବାହ୍ୟ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

ବାହ୍ୟ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ଯଦି (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ଯଦି (lpReserved != ଖାଲି) // called after ExitProcess() call
        {
            cv::__termination = ସତ୍ୟ;
        }
        ଅନ୍ୟଥା
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ଫେରନ୍ତୁ TRUE;
}
#endif


ନାମାକାଶ {

#ifdef OPENCV_WITH_ITT
ତାର୍କିକ overrideThreadName()
{
    ସ୍ଥିର ତାର୍କିକ param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", ମିଥ୍ୟା);
    ଫେରନ୍ତୁ param;
}
#endif

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା g_thread_ଅଙ୍କ = 0;
ଶ୍ରେଣୀ ThreadID {
ସର୍ବସାଧାରଣ:
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ThreadID() :
        id(CV_XADD(&g_thread_ଅଙ୍କ, 1))
    {
#ifdef OPENCV_WITH_ITT
        ଯଦି (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

ସ୍ଥିର TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, ନୂତନ TLSData<ThreadID>());
}

} // namespace
ପୂର୍ଣ୍ଣସଂଖ୍ୟା utils::getThreadID() { ଫେରନ୍ତୁ getThreadIDTLS().get()->id; }


ଶ୍ରେଣୀ ParseError
{
    std::string bad_ମୂଲ୍ୟ;
ସର୍ବସାଧାରଣ:
    ParseError(ଅଚଳ std::string &bad_ମୂଲ୍ୟ_2) :bad_ମୂଲ୍ୟ(bad_ମୂଲ୍ୟ_2) {}
    std::string toString(ଅଚଳ std::string &param) ଅଚଳ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_ମୂଲ୍ୟ;
        ଫେରନ୍ତୁ out.str();
    }
};

template <typename T>
T parseOption(ଅଚଳ std::string &);

template<>
ଧାଡ଼ିଭିତରେ ତାର୍କିକ parseOption(ଅଚଳ std::string & value)
{
    ଯଦି (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଯଦି (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    }
    ଫିଙ୍ଗ ParseError(value);
}

template<>
ଧାଡ଼ିଭିତରେ size_t parseOption(ଅଚଳ std::string &value)
{
    size_t pos = 0;
    ପାଇଁ (; pos < value.size(); pos++)
    {
        ଯଦି (!isdigit(value[pos]))
            ଭାଙ୍ଗ;
    }
    cv::String ମୂଲ୍ୟ_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(ମୂଲ୍ୟ_Str);
    ଯଦି (suffixStr.length() == 0)
        ଫେରନ୍ତୁ v;
    ଅନ୍ୟଥା ଯଦି (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ଫେରନ୍ତୁ v * 1024 * 1024;
    ଅନ୍ୟଥା ଯଦି (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ଫେରନ୍ତୁ v * 1024;
    ଫିଙ୍ଗ ParseError(value);
}

template<>
ଧାଡ଼ିଭିତରେ cv::String parseOption(ଅଚଳ std::string &value)
{
    ଫେରନ୍ତୁ value;
}

template<>
ଧାଡ଼ିଭିତରେ utils::Paths parseOption(ଅଚଳ std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ଅଚଳ ଅକ୍ଷର sep = ';';
#ଅନ୍ୟଥା
    ଅଚଳ ଅକ୍ଷର sep = ':';
#endif
    size_t start_pos = 0;
    ଯେତେବେଳେ (start_pos != std::string::npos)
    {
        ଅଚଳ size_t pos = value.find(sep, start_pos);
        ଅଚଳ std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        ଯଦି (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ଫେରନ୍ତୁ result;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଅଚଳ ଅକ୍ଷର * envRead(ଅଚଳ ଅକ୍ଷର * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    ଫେରନ୍ତୁ ଖାଲି;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ getenv(name);
#endif
}

template<typename T>
ଧାଡ଼ିଭିତରେ T ପଠ(ଅଚଳ std::string & k, ଅଚଳ T & default_ମୂଲ୍ୟ_2)
{
    ଚେଷ୍ଟା
    {
        ଅଚଳ ଅକ୍ଷର * res = envRead(k.c_str());
        ଯଦି (res)
            ଫେରନ୍ତୁ parseOption<T>(std::string(res));
    }
    ଧର (ଅଚଳ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    ଫେରନ୍ତୁ default_ମୂଲ୍ୟ_2;
}

ତାର୍କିକ utils::getConfigurationParameterBool(ଅଚଳ ଅକ୍ଷର* name, ତାର୍କିକ default_ମୂଲ୍ୟ_2)
{
    ଫେରନ୍ତୁ ପଠ<ତାର୍କିକ>(name, default_ମୂଲ୍ୟ_2);
}

size_t utils::getConfigurationParameterSizeT(ଅଚଳ ଅକ୍ଷର* name, size_t default_ମୂଲ୍ୟ_2)
{
    ଫେରନ୍ତୁ ପଠ<size_t>(name, default_ମୂଲ୍ୟ_2);
}

std::string utils::getConfigurationParameterString(ଅଚଳ ଅକ୍ଷର* name, ଅଚଳ std::string & default_ମୂଲ୍ୟ_2)
{
    ଫେରନ୍ତୁ ପଠ<cv::String>(name, default_ମୂଲ୍ୟ_2);
}

utils::Paths utils::getConfigurationParameterPaths(ଅଚଳ ଅକ୍ଷର* name, ଅଚଳ utils::Paths &default_ମୂଲ୍ୟ_2)
{
    ଫେରନ୍ତୁ ପଠ<utils::Paths>(name, default_ମୂଲ୍ୟ_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, ନୂତନ ImplCollector())
}

ଶୂନ୍ୟ setImpl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

ଶୂନ୍ୟ addImpl(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flag, ଅଚଳ ଅକ୍ଷର* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    ଯଦି(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        ଯଦି(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା getImpl(std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    ଫେରନ୍ତୁ getImplData().implFlags; // return actual flags for lazy collection
}

ତାର୍କିକ useCollection()
{
    ଫେରନ୍ତୁ getImplData().useCollection;
}

ଶୂନ୍ୟ setUseCollection(ତାର୍କିକ flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

ନାମାକାଶ instr
{
ତାର୍କିକ useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    ଫେରନ୍ତୁ getInstrumentStruct().useInstr;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ setUseInstrumentation(ତାର୍କିକ flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ଅନ୍ୟଥା
    CV_UNUSED(flag);
#endif
}

Instr_ଗଣ୍ଠି* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    ଫେରନ୍ତୁ &getInstrumentStruct().ମୂଳ_ଗଣ୍ଠି_2;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ଖାଲି;
#endif
}

ଶୂନ୍ୟ resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().ମୂଳ_ଗଣ୍ଠି_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ଗଣ୍ଠି = &getInstrumentStruct().ମୂଳ_ଗଣ୍ଠି_2;
#endif
}

ଶୂନ୍ୟ setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#ଅନ୍ୟଥା
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    ଫେରନ୍ତୁ (FLAGS)getInstrumentStruct().flags;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ (FLAGS)0;
#endif
}

ଗଣ୍ଠି_Data::ଗଣ୍ଠି_Data(ଅଚଳ ଅକ୍ଷର* funName, ଅଚଳ ଅକ୍ଷର* fileName, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_ଅଙ୍କ, ଶୂନ୍ୟ* retAddress, ତାର୍କିକ alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_ଧାଡ଼ି_ଅଙ୍କ       = ଧାଡ଼ି_ଅଙ୍କ;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = ମିଥ୍ୟା;
}
ଗଣ୍ଠି_Data::ଗଣ୍ଠି_Data(ଗଣ୍ଠି_Data &ref)
{
    *ଏହି = ref;
}
ଗଣ୍ଠି_Data& ଗଣ୍ଠି_Data::operator=(ଅଚଳ ଗଣ୍ଠି_Data &ଡାହାଣ)
{
    ଏହି->m_funName      = ଡାହାଣ.m_funName;
    ଏହି->m_instrType    = ଡାହାଣ.m_instrType;
    ଏହି->m_implType     = ଡାହାଣ.m_implType;
    ଏହି->m_fileName     = ଡାହାଣ.m_fileName;
    ଏହି->m_ଧାଡ଼ି_ଅଙ୍କ      = ଡାହାଣ.m_ଧାଡ଼ି_ଅଙ୍କ;
    ଏହି->m_retAddress   = ଡାହାଣ.m_retAddress;
    ଏହି->m_alwaysExpand = ଡାହାଣ.m_alwaysExpand;

    ଏହି->m_threads     = ଡାହାଣ.m_threads;
    ଏହି->m_counter     = ଡାହାଣ.m_counter;
    ଏହି->m_ticksTotal  = ଡାହାଣ.m_ticksTotal;

    ଏହି->m_funError    = ଡାହାଣ.m_funError;

    ଫେରନ୍ତୁ *ଏହି;
}
ଗଣ୍ଠି_Data::~ଗଣ୍ଠି_Data()
{
}
ତାର୍କିକ operator==(ଅଚଳ ଗଣ୍ଠି_Data& ବାମ, ଅଚଳ ଗଣ୍ଠି_Data& ଡାହାଣ)
{
    ଯଦି(ବାମ.m_ଧାଡ଼ି_ଅଙ୍କ == ଡାହାଣ.m_ଧାଡ଼ି_ଅଙ୍କ && ବାମ.m_funName == ଡାହାଣ.m_funName && ବାମ.m_fileName == ଡାହାଣ.m_fileName)
    {
        ଯଦି(ବାମ.m_retAddress == ଡାହାଣ.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || ବାମ.m_alwaysExpand))
            ଫେରନ୍ତୁ ସତ୍ୟ;
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ସ୍ଥିର InstrStruct instr;
    ଫେରନ୍ତୁ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ଫେରନ୍ତୁ *getInstrumentStruct().tlsStruct.get();
}

Instr_ଗଣ୍ଠି* get_Current_ଗଣ୍ଠି()
{
    ଫେରନ୍ତୁ getInstrumentTLSStruct().p_Current_ଗଣ୍ଠି;
}

IntrumentationRegion::IntrumentationRegion(ଅଚଳ ଅକ୍ଷର* funName, ଅଚଳ ଅକ୍ଷର* fileName, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_ଅଙ୍କ, ଶୂନ୍ୟ *retAddress, ତାର୍କିକ alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = ମିଥ୍ୟା;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ଯଦି(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ଯଦି(!pTLS->p_Current_ଗଣ୍ଠି)
        {
            m_disabled = ସତ୍ୟ;
            ଫେରନ୍ତୁ;
        }

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା depth = pTLS->p_Current_ଗଣ୍ଠି->getDepth();
        ଯଦି(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = ସତ୍ୟ;
            ଫେରନ୍ତୁ;
        }

        ଗଣ୍ଠି_Data payload(funName, fileName, ଧାଡ଼ି_ଅଙ୍କ, retAddress, alwaysExpand, instrType, implType);
        Node<ଗଣ୍ଠି_Data>* pChild = ଖାଲି;

        ଯଦି(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_ଗଣ୍ଠି->findChild(payload);
            ଯଦି(!pChild)
            {
                pChild = ନୂତନ Node<ଗଣ୍ଠି_Data>(payload);
                pTLS->p_Current_ଗଣ୍ଠି->addChild(pChild);
            }
        }
        ଅନ୍ୟଥା
        {
            pChild = pTLS->p_Current_ଗଣ୍ଠି->findChild(payload);
            ଯଦି(!pChild)
            {
                m_disabled = ସତ୍ୟ;
                ଫେରନ୍ତୁ;
            }
        }
        pTLS->p_Current_ଗଣ୍ଠି = pChild;

        m_regionTicks = get_Tick_ଗଣନା();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ଯଦି(pStruct->useInstr)
    {
        ଯଦି(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ଯଦି (pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ଗଣନା() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_ଗଣନା); // Concurrent ticks accumulation
                pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_counter++;
                pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_ଗଣ୍ଠି->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_ଗଣ୍ଠି = pTLS->p_Current_ଗଣ୍ଠି->m_pParent;
        }
    }
}
#endif
}

ନାମାକାଶ ipp
{

#ifdef HAVE_IPP
ସଂରଚନା IPPInitSingleton
{
ସର୍ବସାଧାରଣ:
    IPPInitSingleton()
    {
        useIPP         = ସତ୍ୟ;

#ଯଦି defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ଅନ୍ୟଥା
        useIPP_NE      = ମିଥ୍ୟା;
#endif
        ippStatus      = 0;
        funcname       = ଖାଲି;
        filename       = ଖାଲି;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_ଶୀର୍ଷ_Features = 0;
        pIppLibInfo    = ଖାଲି;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, ଖାଲି);
        ଯଦି(ippStatus < 0)
        {
            CV_LOG_ERROR(ଖାଲି, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = ମିଥ୍ୟା;
            ଫେରନ୍ତୁ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ଯଦି(!env.empty())
        {
#ଯଦି IPP_VERSION_X100 >= 201900
            ଅଚଳ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#ନଚେତଯଦି IPP_VERSION_X100 >= 201703
            ଅଚଳ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ନଚେତଯଦି IPP_VERSION_X100 >= 201700
            ଅଚଳ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ଅନ୍ୟଥା
            ଅଚଳ Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            ଯଦି(env.substr(0, 2) == "ne")
            {
                useIPP_NE = ସତ୍ୟ;
                env = env.substr(3, env.size());
            }

            ଯଦି(env == "disabled")
            {
                CV_LOG_WARNING(ଖାଲି, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = ମିଥ୍ୟା;
            }
            ଅନ୍ୟଥା ଯଦି(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ଅନ୍ୟଥା ଯଦି(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ଯଦି IPP_VERSION_X100 >= 201700
#ଯଦି defined (_M_AMD64) || defined (__x86_64__)
            ଅନ୍ୟଥା ଯଦି(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            ଅନ୍ୟଥା
                CV_LOG_ERROR(ଖାଲି, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ଯଦି(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ଯଦି(!(
#ଯଦି IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = ମିଥ୍ୟା;
            ଫେରନ୍ତୁ;
        }

        ଯଦି(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ଅନ୍ୟଥା
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ଯଦି IPP_VERSION_X100 >= 201700
        ଯଦି(ippFeatures&ippCPUID_AVX512F)
        {
            ଯଦି((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_ଶୀର୍ଷ_Features = ippCPUID_AVX512_SKX;
            ଅନ୍ୟଥା ଯଦି((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_ଶୀର୍ଷ_Features = ippCPUID_AVX512_KNL;
            ଅନ୍ୟଥା
                ipp_ଶୀର୍ଷ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ଅନ୍ୟଥା
#endif
        ଯଦି(ippFeatures&ippCPUID_AVX2)
            ipp_ଶୀର୍ଷ_Features = ippCPUID_AVX2;
        ଅନ୍ୟଥା ଯଦି(ippFeatures&ippCPUID_SSE42)
            ipp_ଶୀର୍ଷ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ଯଦି (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_ଶୀର୍ଷ_Features = ippCPUID_SSE42;
        }
    }

ସର୍ବସାଧାରଣ:
    ତାର୍କିକ        useIPP;
    ତାର୍କିକ        useIPP_NE;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ଅଚଳ ଅକ୍ଷର *funcname;
    ଅଚଳ ଅକ୍ଷର *filename;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_ଶୀର୍ଷ_Features;
    ଅଚଳ IppLibraryVersion *pIppLibInfo;
};

ସ୍ଥିର IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, ନୂତନ IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    ଫେରନ୍ତୁ getIPPSingleton().ippFeatures;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_ଶୀର୍ଷ_Features()
{
    ଫେରନ୍ତୁ getIPPSingleton().ipp_ଶୀର୍ଷ_Features;
}
#endif

ଶୂନ୍ୟ setIppStatus(ପୂର୍ଣ୍ଣସଂଖ୍ୟା status, ଅଚଳ ଅକ୍ଷର * ଅଚଳ _funcname, ଅଚଳ ଅକ୍ଷର * ଅଚଳ _filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଧାଡ଼ି_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = ଧାଡ଼ି_3;
#ଅନ୍ୟଥା
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(ଧାଡ଼ି_3);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା getIppStatus()
{
#ifdef HAVE_IPP
    ଫେରନ୍ତୁ getIPPSingleton().ippStatus;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    ଫେରନ୍ତୁ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ଅଚଳ IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    ଯଦି(pInfo)
        ଫେରନ୍ତୁ format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ String("error");
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ String("disabled");
#endif
}

ତାର୍କିକ useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ଯଦି (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    ଫେରନ୍ତୁ (data.useIPP > 0);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ setUseIPP(ତାର୍କିକ flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:ମିଥ୍ୟା;
#ଅନ୍ୟଥା
    CV_UNUSED(flag);
    data.useIPP = ମିଥ୍ୟା;
#endif
}

ତାର୍କିକ useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ଯଦି (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ଫେରନ୍ତୁ (data.useIPP_NE > 0);
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

ଶୂନ୍ୟ setUseIPP_NotExact(ତାର୍କିକ flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#ଅନ୍ୟଥା
    CV_UNUSED(flag);
    data.useIPP_NE = ମିଥ୍ୟା;
#endif
}

} // namespace ipp


ନାମାକାଶ details {

#ଯଦି OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

ଶୂନ୍ୟ setFPDenormalsIgnoreHint(ତାର୍କିକ ignore, CV_OUT FPDenormalsModeState& state)
{
#ଯଦି OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ଯଦି (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ଅଚଳ unsigned old_flags = _mm_getcsr();
    ଅଚଳ unsigned old_ମୂଲ୍ୟ = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ଖାଲି, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_ମୂଲ୍ୟ;
    _mm_setcsr(flags);
#ଅନ୍ୟଥା
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#ଯଦି OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    ଯଦି (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ଅଚଳ unsigned old_flags = _mm_getcsr();
    ଅଚଳ unsigned old_ମୂଲ୍ୟ = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_ମୂଲ୍ୟ;
    ଫେରନ୍ତୁ 2;
#ଅନ୍ୟଥା
    CV_UNUSED(state);
    ଫେରନ୍ତୁ 0;
#endif
}

ତାର୍କିକ restoreFPDenormalsState(ଅଚଳ FPDenormalsModeState& state)
{
#ଯଦି OPENCV_IMPL_FP_HINTS_X86
    ଅଚଳ unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ଅଚଳ unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)value, value == (value & mask), "invalid SSE FP state");
    ଅଚଳ unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ଖାଲି, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ଫେରନ୍ତୁ ସତ୍ୟ;
#ଅନ୍ୟଥା
    CV_UNUSED(state);
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    ଫେରନ୍ତୁ OPENCV_ALGO_HINT_DEFAULT;
#ଅନ୍ୟଥା
    ଫେରନ୍ତୁ ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
