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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#ಅನ್ತರ್ಭಾವ <atomic>
#ಅನ್ತರ್ಭಾವ <exception>
#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ <ostream>

#ifdef __QNX__
    #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಅನ್ತರ್ಭಾವ <sys/neutrino.h>
    #ಅನ್ತರ್ಭಾವ <sys/syspage.h>
#ifdef __aarch64__
    #ಅನ್ತರ್ಭಾವ <aarch64/syspage.h>
#endif
#endif

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/tls.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/instrumentation.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/filesystem.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control_utils.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control.private.hpp>

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಶೂನ್ಯ _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ಸ್ಥಿರ std::ios_base::Init s_iostream_initializer;
#endif
}

ಸ್ಥಿರ Mutex* __initialization_mutex = ರಿಕ್ತ;
Mutex& getInitializationMutex()
{
    ಯದಿ (__initialization_mutex == ರಿಕ್ತ)
    {
        (ಶೂನ್ಯ)_initSystem();
        __initialization_mutex = ನೂತನ Mutex();
    }
    ಪ್ರತ್ಯಾಗಮ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ಸ್ಥಿರ ತಾರ್ಕಿಕ param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ಯದಿ defined(_DEBUG) || defined(__ANDROID__)
    ಸತ್ಯ
#ಅನ್ಯಥಾ
    ಅಸತ್ಯ
#endif
);

ಶೂನ್ಯ* allocSingletonBuffer(size_t size) { ಪ್ರತ್ಯಾಗಮ fastMalloc(size); }
ಶೂನ್ಯ* allocSingletonNewBuffer(size_t size) { ಪ್ರತ್ಯಾಗಮ malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# ಯದಿ defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#ಯದಿ defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# ಯದಿ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#ಅನ್ತರ್ಭಾವ <cstdlib>        // std::abort
#endif

#ಯದಿ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ಯದಿ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ಧ್ರುವ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ಅನ್ಯಥಾ
#  include <elf.h>
#endif
#ಯದಿ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#ಯದಿ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#ಯದಿ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#ಯದಿ defined __loongarch64
#ಅನ್ತರ್ಭಾವ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#ಯದಿ defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <combaseapi.h>
#ಯದಿ (_WIN32_WINNT >= 0x0602)
  #ಅನ್ತರ್ಭಾವ <synchapi.h>
#endif
#ಯದಿ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ಅನ್ತರ್ಭಾವ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#ಅನ್ತರ್ಭಾವ <tchar.h>

#ifdef WINRT
#ಅನ್ತರ್ಭಾವ <wrl/client.h>
#ifndef __cplusplus_winrt
#ಅನ್ತರ್ಭಾವ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    ಪ್ರತ್ಯಾಗಮ std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#ಅನ್ಯಥಾ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    ಯದಿ (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        ಪ್ರತ್ಯಾಗಮ wstr;
    ಯದಿ (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        ಪ್ರತ್ಯಾಗಮ wstr;
    ಯದಿ (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        ಪ್ರತ್ಯಾಗಮ wstr;
    ಯದಿ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        ಪ್ರತ್ಯಾಗಮ wstr;
    ಯದಿ (FAILED(storagefolderRef.As(&storageitemRef)))
        ಪ್ರತ್ಯಾಗಮ wstr;
    str = ರಿಕ್ತ;
    ಯದಿ (FAILED(storageitemRef->get_Path(&str)))
        ಪ್ರತ್ಯಾಗಮ wstr;
    wstr = WindowsGetStringRawBuffer(str, ರಿಕ್ತ);
    WindowsDeleteString(str);
    ಪ್ರತ್ಯಾಗಮ wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], ಪರಿಮಾಣ(guidStr)/ಪರಿಮಾಣ(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ಪ್ರತ್ಯಾಗಮ prefix.append(std::wstring(guidStr));
}

#endif
#ಅನ್ಯಥಾ
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#ಅನ್ತರ್ಭಾವ <pthread.h>
#endif
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಯದಿ defined __MACH__ && defined __APPLE__
#ಅನ್ತರ್ಭಾವ <mach/mach.h>
#ಅನ್ತರ್ಭಾವ <mach/mach_time.h>
#ಅನ್ತರ್ಭಾವ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#ಅನ್ತರ್ಭಾವ "omp.h"
#endif

#ಯದಿ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಯದಿ defined __ANDROID__
#ಅನ್ತರ್ಭಾವ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #ಯದಿ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ಯದಿ _MSC_VER >= 1400  // MSVS 2005
      #ಅನ್ತರ್ಭಾವ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #ಅನ್ಯಥಾ
      #error "Required MSVS 2005+"
    #endif
  #ನೋಚೇದ್ಯದಿ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ಸ್ಥಿರ ಶೂನ್ಯ cv_cpuid(ಪೂರ್ಣಾಙ್ಕ* cpuid_data, ಪೂರ್ಣಾಙ್ಕ reg_eax, ಪೂರ್ಣಾಙ್ಕ reg_ecx)
    {
        ಪೂರ್ಣಾಙ್ಕ __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ಯದಿ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ನೋಚೇದ್ಯದಿ defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ಅನ್ಯಥಾ
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#ಅನ್ತರ್ಭಾವ <chrono>

ನಾಮಾಕಾಶ cv
{

Exception::Exception() { code = Error::StsOk; ಸಾಲು = 0; }

Exception::Exception(Error::Code _code, ಧ್ರುವ String& _err, ಧ್ರುವ String& _func, ಧ್ರುವ String& _file, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_3)
: code(_code), err(_err), func(_func), file(_file), ಸಾಲು(ಸಾಲು_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ಧ್ರುವ ವರ್ಣ* Exception::what() ಧ್ರುವ CV_NOEXCEPT { ಪ್ರತ್ಯಾಗಮ ಸಂದೇಶ.c_str(); }

ಶೂನ್ಯ Exception::formatMessage()
{
    size_t pos = err.find('\n');
    ತಾರ್ಕಿಕ multiline = pos != cv::String::npos;
    ಯದಿ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ಯಾವತ್ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ಯದಿ (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    ಯದಿ (func.size() > 0)
    {
        ಯದಿ (multiline)
            ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), ಸಾಲು, code, codeMessage(), func.c_str(), err.c_str());
        ಅನ್ಯಥಾ
            ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), ಸಾಲು, code, codeMessage(), err.c_str(), func.c_str());
    }
    ಅನ್ಯಥಾ
    {
        ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), ಸಾಲು, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ಧ್ರುವ ವರ್ಣ * Exception::codeMessage() ಧ್ರುವ
{
    ವಿಕಲ್ಪ (code)
    {
    ಪಕ್ಷ cv::Error::StsOk :                  ಪ್ರತ್ಯಾಗಮ "No Error";
    ಪಕ್ಷ cv::Error::StsBackTrace :           ಪ್ರತ್ಯಾಗಮ "Backtrace";
    ಪಕ್ಷ cv::Error::StsError :               ಪ್ರತ್ಯಾಗಮ "Unspecified error";
    ಪಕ್ಷ cv::Error::StsInternal :            ಪ್ರತ್ಯಾಗಮ "Internal error";
    ಪಕ್ಷ cv::Error::StsNoMem :               ಪ್ರತ್ಯಾಗಮ "Insufficient memory";
    ಪಕ್ಷ cv::Error::StsBadArg :              ಪ್ರತ್ಯಾಗಮ "Bad argument";
    ಪಕ್ಷ cv::Error::StsNoConv :              ಪ್ರತ್ಯಾಗಮ "Iterations do not converge";
    ಪಕ್ಷ cv::Error::StsAutoTrace :           ಪ್ರತ್ಯಾಗಮ "Autotrace call";
    ಪಕ್ಷ cv::Error::StsBadSize :             ಪ್ರತ್ಯಾಗಮ "Incorrect size of input array";
    ಪಕ್ಷ cv::Error::StsNullPtr :             ಪ್ರತ್ಯಾಗಮ "Null pointer";
    ಪಕ್ಷ cv::Error::StsDivByZero :           ಪ್ರತ್ಯಾಗಮ "Division by zero occurred";
    ಪಕ್ಷ cv::Error::BadStep :                ಪ್ರತ್ಯಾಗಮ "Image step is wrong";
    ಪಕ್ಷ cv::Error::StsInplaceNotSupported : ಪ್ರತ್ಯಾಗಮ "Inplace operation is not supported";
    ಪಕ್ಷ cv::Error::StsObjectNotFound :      ಪ್ರತ್ಯಾಗಮ "Requested object was not found";
    ಪಕ್ಷ cv::Error::BadDepth :               ಪ್ರತ್ಯಾಗಮ "Input image depth is not supported by function";
    ಪಕ್ಷ cv::Error::StsUnmatchedFormats :    ಪ್ರತ್ಯಾಗಮ "Formats of input arguments do not match";
    ಪಕ್ಷ cv::Error::StsUnmatchedSizes :      ಪ್ರತ್ಯಾಗಮ "Sizes of input arguments do not match";
    ಪಕ್ಷ cv::Error::StsOutOfRange :          ಪ್ರತ್ಯಾಗಮ "One of the arguments\' values is out of range";
    ಪಕ್ಷ cv::Error::StsUnsupportedFormat :   ಪ್ರತ್ಯಾಗಮ "Unsupported format or combination of formats";
    ಪಕ್ಷ cv::Error::BadCOI :                 ಪ್ರತ್ಯಾಗಮ "Input COI is not supported";
    ಪಕ್ಷ cv::Error::Bad_ಅಂಕೆ_Channels :         ಪ್ರತ್ಯಾಗಮ "Bad number of channels";
    ಪಕ್ಷ cv::Error::StsBadFlag :             ಪ್ರತ್ಯಾಗಮ "Bad flag (parameter or structure field)";
    ಪಕ್ಷ cv::Error::StsBadPoint :            ಪ್ರತ್ಯಾಗಮ "Bad parameter of type CvPoint";
    ಪಕ್ಷ cv::Error::StsBadMask :             ಪ್ರತ್ಯಾಗಮ "Bad type of mask argument";
    ಪಕ್ಷ cv::Error::StsParseError :          ಪ್ರತ್ಯಾಗಮ "Parsing error";
    ಪಕ್ಷ cv::Error::StsNotImplemented :      ಪ್ರತ್ಯಾಗಮ "The function/feature is not implemented";
    ಪಕ್ಷ cv::Error::StsBadMemBlock :         ಪ್ರತ್ಯಾಗಮ "Memory block has been corrupted";
    ಪಕ್ಷ cv::Error::StsAssert :              ಪ್ರತ್ಯಾಗಮ "Assertion failed";
    ಪಕ್ಷ cv::Error::GpuNotSupported :        ಪ್ರತ್ಯಾಗಮ "No CUDA support";
    ಪಕ್ಷ cv::Error::GpuApiCallError :        ಪ್ರತ್ಯಾಗಮ "Gpu API call";
    ಪಕ್ಷ cv::Error::OpenGlNotSupported :     ಪ್ರತ್ಯಾಗಮ "No OpenGL support";
    ಪಕ್ಷ cv::Error::OpenGlApiCallError :     ಪ್ರತ್ಯಾಗಮ "OpenGL API call";
    ಪಕ್ಷ cv::Error::OpenCLApiCallError :     ಪ್ರತ್ಯಾಗಮ "OpenCL API call error";
    ಪಕ್ಷ cv::Error::OpenCLDoubleNotSupported:ಪ್ರತ್ಯಾಗಮ "OpenCL device does not support double";
    ಪಕ್ಷ cv::Error::OpenCLInitError :        ಪ್ರತ್ಯಾಗಮ "OpenCL initialization error";
    ಪಕ್ಷ cv::Error::OpenCLNoAMDBlasFft :     ಪ್ರತ್ಯಾಗಮ "Missing OpenCL AMD BLAS FFT";
    ಸ್ವಭಾವ:
    {
        ಸ್ಥಿರ ವರ್ಣ buf[256] {0};
        snprintf(buf, ಪರಿಮಾಣ(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        ಪ್ರತ್ಯಾಗಮ buf;
    }
    };

}

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { ರಿಕ್ತ };

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* getHWFeatureName(ಪೂರ್ಣಾಙ್ಕ id)
{
    ಪ್ರತ್ಯಾಗಮ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : ರಿಕ್ತ;
}
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* getHWFeatureNameSafe(ಪೂರ್ಣಾಙ್ಕ id)
{
    ಧ್ರುವ ವರ್ಣ* name = getHWFeatureName(id);
    ಪ್ರತ್ಯಾಗಮ name ? name : "Unknown feature";
}

ಸಂರಚನಾ HWFeatures
{
    ಗಣನಾ { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(ತಾರ್ಕಿಕ run_initialize = ಅಸತ್ಯ)
    {
        ಯದಿ (run_initialize)
            initialize();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ initializeNames()
    {
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    ಶೂನ್ಯ initialize(ಶೂನ್ಯ)
    {
        ಯದಿ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        ಪೂರ್ಣಾಙ್ಕ cpuid_data[4] = { 0, 0, 0, 0 };
        ಪೂರ್ಣಾಙ್ಕ cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        ಪೂರ್ಣಾಙ್ಕ x86_family = (cpuid_data[0] >> 8) & 15;
        ಯದಿ( x86_family >= 6 )
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
            ಪೂರ್ಣಾಙ್ಕ cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            ತಾರ್ಕಿಕ have_AVX_OS_support = ಸತ್ಯ;
            ತಾರ್ಕಿಕ have_AVX512_OS_support = ಸತ್ಯ;
            ಯದಿ (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = ಅಸತ್ಯ; // OS uses XSAVE_XRSTORE and CPU support AVX
            ಅನ್ಯಥಾ
            {
                ಪೂರ್ಣಾಙ್ಕ xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (ಪೂರ್ಣಾಙ್ಕ)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #ನೋಚೇದ್ಯದಿ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                ಯದಿ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = ಅಸತ್ಯ; // YMM registers
                ಯದಿ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = ಅಸತ್ಯ; // ZMM registers
            }

            ಯದಿ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = ಅಸತ್ಯ;
                have[CV_CPU_FP16] = ಅಸತ್ಯ;
                have[CV_CPU_AVX2] = ಅಸತ್ಯ;
                have[CV_CPU_FMA3] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_VNNI] = ಅಸತ್ಯ;
            }
            ಯದಿ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512BW] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512CD] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512DQ] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512ER] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512IFMA] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512PF] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512VBMI] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512VL] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512VBMI2] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512VNNI] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512BITALG] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_512VPOPCNTDQ] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_5124VNNIW] = ಅಸತ್ಯ;
                have[CV_CPU_AVX_5124FMAPS] = ಅಸತ್ಯ;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ಯದಿ (have[CV_CPU_AVX512_COMMON])
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
            ಅನ್ಯಥಾ
            {
                have[CV_CPU_AVX512_KNL] = ಅಸತ್ಯ;
                have[CV_CPU_AVX512_KNM] = ಅಸತ್ಯ;
                have[CV_CPU_AVX512_SKX] = ಅಸತ್ಯ;
                have[CV_CPU_AVX512_CNL] = ಅಸತ್ಯ;
                have[CV_CPU_AVX512_CLX] = ಅಸತ್ಯ;
                have[CV_CPU_AVX512_ICL] = ಅಸತ್ಯ;
            }
        }
    #endif // CV_CPUID_X86

    #ಯದಿ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = ಸತ್ಯ;
        have[CV_CPU_FP16] = ಸತ್ಯ;
        ಪೂರ್ಣಾಙ್ಕ cpufile = open("/proc/self/auxv", O_RDONLY);

        ಯದಿ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ಧ್ರುವ size_t size_auxv_t = ಪರಿಮಾಣ(auxv);

            ಯಾವತ್ ((size_t)ಪಠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ಯದಿ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ಯದಿ defined(AT_HWCAP2)
                ಅನ್ಯಥಾ ಯದಿ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #ನೋಚೇದ್ಯದಿ defined __arm__ && defined __ANDROID__
      #ಯದಿ defined HAVE_CPUFEATURES
        CV_LOG_INFO(ರಿಕ್ತ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ರಿಕ್ತ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ಅನ್ಯಥಾ
        CV_LOG_INFO(ರಿಕ್ತ, "cpufeatures library is not available for CPU detection");
        #ಯದಿ CV_NEON
        CV_LOG_INFO(ರಿಕ್ತ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = ಸತ್ಯ;
        #ಅನ್ಯಥಾ
        CV_LOG_INFO(ರಿಕ್ತ, "- NEON instructions is NOT enabled via build flags");
        #endif
        #ಯದಿ CV_FP16
        CV_LOG_INFO(ರಿಕ್ತ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = ಸತ್ಯ;
        #ಅನ್ಯಥಾ
        CV_LOG_INFO(ರಿಕ್ತ, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #ನೋಚೇದ್ಯದಿ defined __arm__
        ಪೂರ್ಣಾಙ್ಕ cpufile = open("/proc/self/auxv", O_RDONLY);

        ಯದಿ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ಧ್ರುವ size_t size_auxv_t = ಪರಿಮಾಣ(auxv);

            ಯಾವತ್ ((size_t)ಪಠ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                ಯದಿ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ಭಙ್ಗ;
                }
            }

            close(cpufile);
        }
    #endif
    #ನೋಚೇದ್ಯದಿ (defined __APPLE__)
    #ಯದಿ defined __ARM_NEON
        have[CV_CPU_NEON] = ಸತ್ಯ;
    #endif
    #ಯದಿ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = ಸತ್ಯ;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    ಪೂರ್ಣಾಙ್ಕ has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = ಪರಿಮಾಣ(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, ರಿಕ್ತ, 0);
    ಯದಿ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = ಸತ್ಯ;
    }
    ಪೂರ್ಣಾಙ್ಕ has_feat_bf16 = 0;
    size_t has_feat_bf16_size = ಪರಿಮಾಣ(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, ರಿಕ್ತ, 0);
    ಯದಿ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = ಸತ್ಯ;
    }
    #ನೋಚೇದ್ಯದಿ (defined __clang__)
    #ಯದಿ defined __ARM_NEON
        have[CV_CPU_NEON] = ಸತ್ಯ;
        #ಯದಿ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = ಸತ್ಯ;
        #endif
    #endif
    #endif
    #ಯದಿ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = ಸತ್ಯ;
    #endif
    #ಯದಿ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = ಸತ್ಯ;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = ಸತ್ಯ;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = ಸತ್ಯ;
    #endif

    #ಯದಿ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned ಪೂರ್ಣಾಙ್ಕ hwcap = getauxval(AT_HWCAP);
        ಯದಿ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ಯದಿ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ಸತ್ಯ;
            } ಅನ್ಯಥಾ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ನೋಚೇದ್ಯದಿ (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        ಯದಿ (elf_aux_info(AT_HWCAP, &hwcap, ಪರಿಮಾಣ(hwcap)) != 0)
            hwcap = 0;
        ಯದಿ (hwcap & PPC_FEATURE_HAS_VSX) {
            ಯದಿ (elf_aux_info(AT_HWCAP2, &hwcap2, ಪರಿಮಾಣ(hwcap2)) != 0)
                hwcap2 = 0;
            ಯದಿ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ಸತ್ಯ;
            } ಅನ್ಯಥಾ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ಅನ್ಯಥಾ
        // TODO: AIX
        #ಯದಿ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = ಸತ್ಯ;
        #endif
        #ಯದಿ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = ಸತ್ಯ;
        #endif
    #endif

    #ಯದಿ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = ಸತ್ಯ;
        #ಯದಿ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = ಸತ್ಯ;
        #endif
        #ಯದಿ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = ಸತ್ಯ;
        #endif
    #endif

    #ಯದಿ defined __loongarch64 && defined __linux__
        ಪೂರ್ಣಾಙ್ಕ flag = (ಪೂರ್ಣಾಙ್ಕ)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        ತಾರ್ಕಿಕ skip_baseline_check = ಅಸತ್ಯ;
        ಯದಿ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = ಸತ್ಯ;
        }
        ಪೂರ್ಣಾಙ್ಕ baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ಯದಿ (!checkFeatures(baseline_features, ಪರಿಮಾಣ(baseline_features) / ಪರಿಮಾಣ(baseline_features[0]))
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
            checkFeatures(baseline_features, ಪರಿಮಾಣ(baseline_features) / ಪರಿಮಾಣ(baseline_features[0]), ಸತ್ಯ);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, ಪರಿಮಾಣ(baseline_features) / ಪರಿಮಾಣ(baseline_features[0]));
    }

    ತಾರ್ಕಿಕ checkFeatures(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* features, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2, ತಾರ್ಕಿಕ dump = ಅಸತ್ಯ)
    {
        ತಾರ್ಕಿಕ result = ಸತ್ಯ;
        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < ಎಣಿಕೆ_2; i++)
        {
            ಪೂರ್ಣಾಙ್ಕ feature = features[i];
            ಯದಿ (feature)
            {
                ಯದಿ (have[feature])
                {
                    ಯದಿ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                ಅನ್ಯಥಾ
                {
                    result = ಅಸತ್ಯ;
                    ಯದಿ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        ಪ್ರತ್ಯಾಗಮ result;
    }

    ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ is_ಚಿಹ್ನೆ_Separator(ವರ್ಣ c)
    {
        ಪ್ರತ್ಯಾಗಮ c == ',' || c == ';';
    }

    ಶೂನ್ಯ readSettings(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* baseline_features, ಪೂರ್ಣಾಙ್ಕ baseline_ಎಣಿಕೆ)
    {
        ತಾರ್ಕಿಕ dump = ಸತ್ಯ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ಯದಿ (!disabled_features.empty())
        {
            ಧ್ರುವ ವರ್ಣ* start = disabled_features.c_str();
            ಪ್ರತಿ (;;)
            {
                ಯಾವತ್ (start[0] != 0 && is_ಚಿಹ್ನೆ_Separator(start[0]))
                {
                    start++;
                }
                ಯದಿ (start[0] == 0)
                    ಭಙ್ಗ;
                ಧ್ರುವ ವರ್ಣ* end = start;
                ಯಾವತ್ (end[0] != 0 && !is_ಚಿಹ್ನೆ_Separator(end[0]))
                {
                    end++;
                }
                ಯದಿ (end == start)
                    ಅನುವರ್ತ;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
                ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ಯದಿ (!g_hwFeatureNames[i]) ಅನುವರ್ತ;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    ಯದಿ (len != feature.size()) ಅನುವರ್ತ;
                    ಯದಿ (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        ತಾರ್ಕಿಕ isBaseline = ಅಸತ್ಯ;
                        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ k = 0; k < baseline_ಎಣಿಕೆ; k++)
                        {
                            ಯದಿ (baseline_features[k] == i)
                            {
                                isBaseline = ಸತ್ಯ;
                                ಭಙ್ಗ;
                            }
                        }
                        ಯದಿ (isBaseline)
                        {
                            ಯದಿ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        ಯದಿ (!have[i])
                        {
                            ಯದಿ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = ಅಸತ್ಯ;

                        found = ಸತ್ಯ;
                        ಭಙ್ಗ;
                    }
                }
                ಯದಿ (!found)
                {
                    ಯದಿ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    ತಾರ್ಕಿಕ have[MAX_FEATURE+1]{};
};

ಸ್ಥಿರ HWFeatures  featuresEnabled(ಸತ್ಯ), featuresDisabled = HWFeatures(ಅಸತ್ಯ);
ಸ್ಥಿರ HWFeatures* currentFeatures = &featuresEnabled;

ತಾರ್ಕಿಕ checkHardwareSupport(ಪೂರ್ಣಾಙ್ಕ feature)
{
    ಯದಿ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ currentFeatures->have[feature];
}

String getHardwareFeatureName(ಪೂರ್ಣಾಙ್ಕ feature)
{
    ಧ್ರುವ ವರ್ಣ* name = getHWFeatureName(feature);
    ಪ್ರತ್ಯಾಗಮ name ? String(name) : String();
}

std::string get_CPUFeatures_ಸಾಲು()
{
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ sz = ಪರಿಮಾಣ(features) / ಪರಿಮಾಣ(features[0]);
    std::string result;
    std::string prefix;
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 1; i < sz; ++i)
    {
        ಯದಿ (features[i] == 0)
        {
            prefix = "*";
            ಅನುವರ್ತ;
        }
        ಯದಿ (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        ಯದಿ (!checkHardwareSupport(features[i])) result.append("?");
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

ಚಞ್ಚಲ ತಾರ್ಕಿಕ useOptimizedFlag = ಸತ್ಯ;

ಶೂನ್ಯ setUseOptimized( ತಾರ್ಕಿಕ flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

ತಾರ್ಕಿಕ useOptimized(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ useOptimizedFlag;
}

int64 get_Tick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ಪ್ರತ್ಯಾಗಮ (int64)now.time_since_epoch().ಎಣಿಕೆ_2();
}

ದ್ವಿಗುಣ getTickFrequency(ಶೂನ್ಯ)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    ದ್ವಿಗುಣ clock_freq = clock_period_t::den / clock_period_t::num;
    ಪ್ರತ್ಯಾಗಮ clock_freq;
}

#ಯದಿ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ಯದಿ defined(__i386__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    int64 x;
    __asm__ ಚಞ್ಚಲ (".byte 0x0f, 0x31" : "=A" (x));
    ಪ್ರತ್ಯಾಗಮ x;
}
#ನೋಚೇದ್ಯದಿ defined(__x86_64__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ಪ್ರತ್ಯಾಗಮ (int64)lo | ((int64)hi << 32);
}

#ನೋಚೇದ್ಯದಿ defined(__ppc__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    unsigned upper, lower, tmp;
    __asm__ ಚಞ್ಚಲ(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ಪ್ರತ್ಯಾಗಮ lower | ((int64)upper << 32);
}

#ಅನ್ಯಥಾ

#error "RDTSC not defined"

#endif

#ನೋಚೇದ್ಯದಿ defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ಅನ್ಯಥಾ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಪ್ರತ್ಯಾಗಮ get_Tick_ಎಣಿಕೆ();
}
//#endif

#endif


ನಾಮಾಕಾಶ internal {

ವರ್ಗ Timestamp
{
ಸಾರ್ವಜನಿಕ:
    ಧ್ರುವ int64 zero_Tick_ಎಣಿಕೆ;
    ಧ್ರುವ ದ್ವಿಗುಣ ns_in_ticks;

    Timestamp()
        : zero_Tick_ಎಣಿಕೆ(get_Tick_ಎಣಿಕೆ())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ಎಣಿಕೆ();
        ಪ್ರತ್ಯಾಗಮ (int64)((t - zero_Tick_ಎಣಿಕೆ) * ns_in_ticks);
    }

    ಸ್ಥಿರ Timestamp& getInstance()
    {
        ಸ್ಥಿರ Timestamp g_timestamp;
        ಪ್ರತ್ಯಾಗಮ g_timestamp;
    }
};

ವರ್ಗ InitTimestamp {
ಸಾರ್ವಜನಿಕ:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ಸ್ಥಿರ InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ಪ್ರತ್ಯಾಗಮ internal::Timestamp::getInstance().getTimestamp();
}


ಧ್ರುವ String& getBuildInformation()
{
    ಸ್ಥಿರ String build_info =
#ಅನ್ತರ್ಭಾವ "version_string.inc"
    ;
    ಪ್ರತ್ಯಾಗಮ build_info;
}

String getVersionString() { ಪ್ರತ್ಯಾಗಮ String(CV_VERSION); }

ಪೂರ್ಣಾಙ್ಕ getVersionMajor() { ಪ್ರತ್ಯಾಗಮ CV_VERSION_MAJOR; }

ಪೂರ್ಣಾಙ್ಕ getVersionMinor() { ಪ್ರತ್ಯಾಗಮ CV_VERSION_MINOR; }

ಪೂರ್ಣಾಙ್ಕ getVersionRevision() { ಪ್ರತ್ಯಾಗಮ CV_VERSION_REVISION; }

String format( ಧ್ರುವ ವರ್ಣ* fmt, ... )
{
    AutoBuffer<ವರ್ಣ, 1024> buf;

    ಪ್ರತಿ ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        ಪೂರ್ಣಾಙ್ಕ bsize = static_cast<ಪೂರ್ಣಾಙ್ಕ>(buf.size());
        ಪೂರ್ಣಾಙ್ಕ len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ಯದಿ (len >= bsize)
        {
            buf.resize(len + 1);
            ಅನುವರ್ತ;
        }
        buf[bsize - 1] = 0;
        ಪ್ರತ್ಯಾಗಮ String(buf.data(), len);
    }
}

String tempfile( ಧ್ರುವ ವರ್ಣ* suffix )
{
#ಯದಿ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ಯದಿ defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    ಯದಿ (temp_file.empty())
        ಪ್ರತ್ಯಾಗಮ String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    ವರ್ಣ aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#ನೋಚೇದ್ಯದಿ defined(_WIN32_WCE)
    ಧ್ರುವ auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    ಯದಿ(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        ವರ್ಣ aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ಅನ್ಯಥಾ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    ಯದಿ (temp_dir.empty())
    {
        ::GetTempPathW(ಪರಿಮಾಣ(temp_dir2_w)/ಪರಿಮಾಣ(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        ಪೂರ್ಣಾಙ್ಕ len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, ರಿಕ್ತ, 0, ರಿಕ್ತ, ರಿಕ್ತ);
        ಯದಿ (len > 0)
        {
            std::vector<ವರ್ಣ> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, ರಿಕ್ತ, ರಿಕ್ತ);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ಯದಿ (FAILED(hr))
        ಪ್ರತ್ಯಾಗಮ String();
    ವರ್ಣ guidStr[40];
    ಧ್ರುವ ವರ್ಣ* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, ಪರಿಮಾಣ(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[0], (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[1],
            (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[2], (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[3], (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[4],
            (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[5], (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[6], (unsigned ಪೂರ್ಣಾಙ್ಕ)g.Data4[7]);

    fname = temp_dir;
    ಯದಿ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# ಅನ್ಯಥಾ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    ವರ್ಣ defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ಅನ್ಯಥಾ
    ವರ್ಣ defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ಯದಿ (temp_dir.empty())
        fname = defaultTemplate;
    ಅನ್ಯಥಾ
    {
        fname = temp_dir;
        ವರ್ಣ ech = fname[fname.size() - 1];
        ಯದಿ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ fd = mkstemp((ವರ್ಣ*)fname.c_str());
    ಯದಿ (fd == -1) ಪ್ರತ್ಯಾಗಮ String();

    close(fd);
    remove(fname.c_str());
# endif

    ಯದಿ (suffix)
    {
        ಯದಿ (suffix[0] != '.')
            ಪ್ರತ್ಯಾಗಮ fname + "." + suffix;
        ಅನ್ಯಥಾ
            ಪ್ರತ್ಯಾಗಮ fname + suffix;
    }
    ಪ್ರತ್ಯಾಗಮ fname;
#ಅನ್ಯಥಾ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ಸ್ಥಿರ ErrorCallback customErrorCallback = 0;
ಸ್ಥಿರ ಶೂನ್ಯ* customErrorCallbackData = 0;
ಸ್ಥಿರ ತಾರ್ಕಿಕ breakOnError = ಅಸತ್ಯ;

ತಾರ್ಕಿಕ setBreakOnError(ತಾರ್ಕಿಕ value)
{
    ತಾರ್ಕಿಕ prevVal = breakOnError;
    breakOnError = value;
    ಪ್ರತ್ಯಾಗಮ prevVal;
}

ಪೂರ್ಣಾಙ್ಕ cv_snprintf(ವರ್ಣ* buf, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ವರ್ಣ* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    ಪೂರ್ಣಾಙ್ಕ res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ಪ್ರತ್ಯಾಗಮ res;
}

ಪೂರ್ಣಾಙ್ಕ cv_vsnprintf(ವರ್ಣ* buf, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ವರ್ಣ* fmt, va_list args)
{
#ಯದಿ defined _MSC_VER
    ಯದಿ (len <= 0) ಪ್ರತ್ಯಾಗಮ len == 0 ? 1024 : -1;
    ಪೂರ್ಣಾಙ್ಕ res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    ಯದಿ (res >= 0 && res < len)
    {
        buf[res] = 0;
        ಪ್ರತ್ಯಾಗಮ res;
    }
    ಅನ್ಯಥಾ
    {
        buf[len - 1] = 0; // truncate happened
        ಪ್ರತ್ಯಾಗಮ res >= len ? res : (len * 2);
    }
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ vsnprintf(buf, len, fmt, args);
#endif
}

ಸ್ಥಿರ ಶೂನ್ಯ dumpException(ಧ್ರುವ Exception& exc)
{
    ಧ್ರುವ ವರ್ಣ* errorStr = exc.codeMessage();
    ವರ್ಣ buf[1 << 12];

    cv_snprintf(buf, ಪರಿಮಾಣ(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.ಸಾಲು);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ಅನ್ಯಥಾ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
ಸ್ಥಿರ ತಾರ್ಕಿಕ cv_terminate_handler_installed = ಅಸತ್ಯ;
ಸ್ಥಿರ std::terminate_handler cv_old_terminate_handler;
ಸ್ಥಿರ cv::Exception cv_terminate_handler_exception;
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", ಸತ್ಯ);
ಸ್ಥಿರ ಶೂನ್ಯ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ಯದಿ (ಅಸತ್ಯ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# ಯದಿ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

ಶೂನ್ಯ error( ಧ್ರುವ Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        ಯದಿ (!cv_terminate_handler_installed)
        {
            ಯದಿ (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = ಸತ್ಯ;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    ಯದಿ (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.ಸಾಲು, customErrorCallbackData);
    ಅನ್ಯಥಾ ಯದಿ (param_dumpErrors)
    {
        dumpException(exc);
    }

    ಯದಿ(breakOnError)
    {
        std::terminate();
    }

    ಕ್ಷಿಪ exc;
#ifdef __GNUC__
# ಯದಿ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

ಶೂನ್ಯ error(Error::Code _code, ಧ್ರುವ String& _err, ಧ್ರುವ ವರ್ಣ* _func, ಧ್ರುವ ವರ್ಣ* _file, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_3)
{
    error(cv::Exception(_code, _err, _func, _file, ಸಾಲು_3));
#ifdef __GNUC__
# ಯದಿ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# ಯದಿ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, ಶೂನ್ಯ* userdata, ಶೂನ್ಯ** prevUserdata)
{
    ಯದಿ( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    ಪ್ರತ್ಯಾಗಮ prevCallback;
}

ಶೂನ್ಯ terminate(Error::Code _code, ಧ್ರುವ String& _err, ಧ್ರುವ ವರ್ಣ* _func, ಧ್ರುವ ವರ್ಣ* _file, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, ಸಾಲು_3));
    std::terminate();
}

}

ನಾಮಾಕಾಶ cv {
ತಾರ್ಕಿಕ __termination = ಅಸತ್ಯ;


//////////////////////////////// thread-local storage ////////////////////////////////

ನಾಮಾಕಾಶ details {

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
ವರ್ಗ TlsAbstraction
{
ಸಾರ್ವಜನಿಕ:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    ಶೂನ್ಯ* getData() ಧ್ರುವ;
    ಶೂನ್ಯ setData(ಶೂನ್ಯ *pData);

    ಶೂನ್ಯ releaseSystemResources();

ಗುಪ್ತ:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_ಕೀಲಿ;
    ತಾರ್ಕಿಕ disposed;
#endif
#ಅನ್ಯಥಾ // _WIN32
    pthread_key_t  tls_ಕೀಲಿ;
    std::atomic<ತಾರ್ಕಿಕ> disposed;
#endif
};

ವರ್ಗ TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
ಸಾರ್ವಜನಿಕ:
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
ಸ್ಥಿರ TlsAbstraction* getTlsAbstraction()
{
    ಸ್ಥಿರ TlsAbstraction *g_tls = ನೂತನ TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ಸ್ಥಿರ TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ಪ್ರತ್ಯಾಗಮ g_tls;
}


#ifdef _WIN32
#ifdef WINRT
ಸ್ಥಿರ __declspec( thread ) ಶೂನ್ಯ* tlsData = ರಿಕ್ತ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ಸತ್ಯ;  // DllMain is missing in static builds
}
ಶೂನ್ಯ* TlsAbstraction::getData() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ tlsData;
}
ಶೂನ್ಯ TlsAbstraction::setData(ಶೂನ್ಯ *pData)
{
    tlsData = pData;
}
#ಅನ್ಯಥಾ //WINRT
#ifdef CV_USE_FLS
ಸ್ಥಿರ ಶೂನ್ಯ NTAPI opencv_fls_destructor(ಶೂನ್ಯ* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(ಅಸತ್ಯ)
{
#ifndef CV_USE_FLS
    tls_ಕೀಲಿ = TlsAlloc();
#ಅನ್ಯಥಾ // CV_USE_FLS
    tls_ಕೀಲಿ = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_ಕೀಲಿ != TLS_OUT_OF_INDEXES);
}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ಸತ್ಯ;  // DllMain is missing in static builds
    disposed = ಸತ್ಯ;
#ifndef CV_USE_FLS
    TlsFree(tls_ಕೀಲಿ);
#ಅನ್ಯಥಾ // CV_USE_FLS
    FlsFree(tls_ಕೀಲಿ);
#endif // CV_USE_FLS
    tls_ಕೀಲಿ = TLS_OUT_OF_INDEXES;
}
ಶೂನ್ಯ* TlsAbstraction::getData() ಧ್ರುವ
{
    ಯದಿ (disposed)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#ifndef CV_USE_FLS
    ಪ್ರತ್ಯಾಗಮ Tls_Get_ಮೌಲ್ಯ(tls_ಕೀಲಿ);
#ಅನ್ಯಥಾ // CV_USE_FLS
    ಪ್ರತ್ಯಾಗಮ Fls_Get_ಮೌಲ್ಯ(tls_ಕೀಲಿ);
#endif // CV_USE_FLS
}
ಶೂನ್ಯ TlsAbstraction::setData(ಶೂನ್ಯ *pData)
{
    ಯದಿ (disposed)
        ಪ್ರತ್ಯಾಗಮ;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_ಮೌಲ್ಯ(tls_ಕೀಲಿ, pData) == TRUE);
#ಅನ್ಯಥಾ // CV_USE_FLS
    CV_Assert(Fls_Set_ಮೌಲ್ಯ(tls_ಕೀಲಿ, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#ಅನ್ಯಥಾ // _WIN32
ಸ್ಥಿರ ಶೂನ್ಯ opencv_tls_destructor(ಶೂನ್ಯ* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(ಅಸತ್ಯ)
{
    CV_Assert(pthread_key_create(&tls_ಕೀಲಿ, opencv_tls_destructor) == 0);
}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ಸತ್ಯ;  // DllMain is missing in static builds
    disposed = ಸತ್ಯ;
    ಯದಿ (pthread_key_delete(tls_ಕೀಲಿ) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
ಶೂನ್ಯ* TlsAbstraction::getData() ಧ್ರುವ
{
    ಯದಿ (disposed)
        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    ಪ್ರತ್ಯಾಗಮ pthread_getspecific(tls_ಕೀಲಿ);
}
ಶೂನ್ಯ TlsAbstraction::setData(ಶೂನ್ಯ *pData)
{
    ಯದಿ (disposed)
        ಪ್ರತ್ಯಾಗಮ;  // no-op
    CV_Assert(pthread_setspecific(tls_ಕೀಲಿ, pData) == 0);
}
#endif

// Per-thread data structure
ಸಂರಚನಾ ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<ಶೂನ್ಯ*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isTlsStorageInitialized = ಅಸತ್ಯ;

// Main TLS storage class
ವರ್ಗ TlsStorage
{
ಸಾರ್ವಜನಿಕ:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (ಶೂನ್ಯ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = ಸತ್ಯ;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    ಶೂನ್ಯ releaseThread(ಶೂನ್ಯ* tls_ಮೌಲ್ಯ = ರಿಕ್ತ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ಯದಿ (ರಿಕ್ತ == tls)
            ಪ್ರತ್ಯಾಗಮ;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_ಮೌಲ್ಯ == ರಿಕ್ತ ? (ThreadData*)tls->getData() : (ThreadData*)tls_ಮೌಲ್ಯ;
        ಯದಿ (pTD == ರಿಕ್ತ)
            ಪ್ರತ್ಯಾಗಮ;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        ಪ್ರತಿ (size_t i = 0; i < threads.size(); i++)
        {
            ಯದಿ (pTD == threads[i])
            {
                threads[i] = ರಿಕ್ತ;
                ಯದಿ (tls_ಮೌಲ್ಯ == ರಿಕ್ತ)
                    tls->setData(0);
                std::vector<ಶೂನ್ಯ*>& thread_slots = pTD->slots;
                ಪ್ರತಿ (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    ಶೂನ್ಯ* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = ರಿಕ್ತ;
                    ಯದಿ (!pData)
                        ಅನುವರ್ತ;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    ಯದಿ (container)
                        container->deleteDataInstance(pData);
                    ಅನ್ಯಥಾ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (ಪೂರ್ಣಾಙ್ಕ)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (ಶೂನ್ಯ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        ಪ್ರತಿ(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            ಯದಿ (tlsSlots[slot].container == ರಿಕ್ತ)
            {
                tlsSlots[slot].container = container;
                ಪ್ರತ್ಯಾಗಮ slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        ಪ್ರತ್ಯಾಗಮ tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    ಶೂನ್ಯ releaseSlot(size_t slotIdx, std::vector<ಶೂನ್ಯ*> &dataVec, ತಾರ್ಕಿಕ keepSlot = ಅಸತ್ಯ)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        ಪ್ರತಿ(size_t i = 0; i < threads.size(); i++)
        {
            ಯದಿ(threads[i])
            {
                std::vector<ಶೂನ್ಯ*>& thread_slots = threads[i]->slots;
                ಯದಿ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = ರಿಕ್ತ;
                }
            }
        }

        ಯದಿ (!keepSlot)
        {
            tlsSlots[slotIdx].container = ರಿಕ್ತ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ಶೂನ್ಯ* getData(size_t slotIdx) ಧ್ರುವ
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ಯದಿ (ರಿಕ್ತ == tls)
            ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ಯದಿ(threadData && threadData->slots.size() > slotIdx)
            ಪ್ರತ್ಯಾಗಮ threadData->slots[slotIdx];

        ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
    }

    // Gather data from threads by TLS storage index
    ಶೂನ್ಯ gather(size_t slotIdx, std::vector<ಶೂನ್ಯ*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        ಪ್ರತಿ(size_t i = 0; i < threads.size(); i++)
        {
            ಯದಿ(threads[i])
            {
                std::vector<ಶೂನ್ಯ*>& thread_slots = threads[i]->slots;
                ಯದಿ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    ಶೂನ್ಯ setData(size_t slotIdx, ಶೂನ್ಯ* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ಯದಿ (ರಿಕ್ತ == tls)
            ಪ್ರತ್ಯಾಗಮ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ಯದಿ(!threadData)
        {
            threadData = ನೂತನ ThreadData;
            tls->setData((ಶೂನ್ಯ*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                ತಾರ್ಕಿಕ found = ಅಸತ್ಯ;
                // Find unused slots
                ಪ್ರತಿ(size_t slot = 0; slot < threads.size(); slot++)
                {
                    ಯದಿ (threads[slot] == ರಿಕ್ತ)
                    {
                        threadData->idx = (ಪೂರ್ಣಾಙ್ಕ)slot;
                        threads[slot] = threadData;
                        found = ಸತ್ಯ;
                        ಭಙ್ಗ;
                    }
                }

                ಯದಿ (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        ಯದಿ(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, ರಿಕ್ತ);
        }
        threadData->slots[slotIdx] = pData;
    }

ಗುಪ್ತ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ಸಂರಚನಾ TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ಸಂರಚನಾ TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ಸ್ಥಿರ TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, ನೂತನ TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
ಸ್ಥಿರ ಶೂನ್ಯ opencv_tls_destructor(ಶೂನ್ಯ* pData)
{
    ಯದಿ (!g_isTlsStorageInitialized)
        ಪ್ರತ್ಯಾಗಮ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#ಅನ್ಯಥಾ // _WIN32
#ifdef CV_USE_FLS
ಸ್ಥಿರ ಶೂನ್ಯ WINAPI opencv_fls_destructor(ಶೂನ್ಯ* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    ಯದಿ (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = ಸತ್ಯ;
    }

    ಯದಿ (!g_isTlsStorageInitialized)
        ಪ್ರತ್ಯಾಗಮ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

ಸ್ಥಿರ TlsStorage* ಧ್ರುವ g_force_initialization_of_TlsStorage
#ಯದಿ defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#ಅನ್ಯಥಾ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
ವರ್ಗ TlsStorage
{
ಸಾರ್ವಜನಿಕ:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        ಪ್ರತಿ (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            ಯದಿ (container && s.data)
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
        ಪ್ರತಿ (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            ಯದಿ (s.container == ರಿಕ್ತ)
            {
                CV_Assert(!s.data);
                s.container = container;
                ಪ್ರತ್ಯಾಗಮ slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        ಪ್ರತ್ಯಾಗಮ slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    ಶೂನ್ಯ releaseSlot(size_t slotIdx, std::vector<ಶೂನ್ಯ*> &dataVec, ತಾರ್ಕಿಕ keepSlot = ಅಸತ್ಯ)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ಶೂನ್ಯ* data = s.data;
        ಯದಿ (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        ಯದಿ (!keepSlot)
        {
            s.container = ರಿಕ್ತ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ಶೂನ್ಯ* getData(size_t slotIdx) ಧ್ರುವ
    {
        CV_Assert(slotIdx < slots.size());
        ಧ್ರುವ SlotInfo& s = slots[slotIdx];
        ಪ್ರತ್ಯಾಗಮ s.data;
    }

    // Gather data from threads by TLS storage index
    ಶೂನ್ಯ gather(size_t slotIdx, std::vector<ಶೂನ್ಯ*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        ಶೂನ್ಯ* data = s.data;
        ಯದಿ (data)
            dataVec.push_back(data);
        ಪ್ರತ್ಯಾಗಮ;
    }

    // Set data to storage index
    ಶೂನ್ಯ setData(size_t slotIdx, ಶೂನ್ಯ* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

ಗುಪ್ತ:
    ಸಂರಚನಾ SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        ಶೂನ್ಯ* data;
    };
    std::vector<ಸಂರಚನಾ SlotInfo> slots;
};

ಸ್ಥಿರ TlsStorage& getTlsStorage()
{
    ಸ್ಥಿರ TlsStorage g_storage;  // no threading
    ಪ್ರತ್ಯಾಗಮ g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using ನಾಮಾಕಾಶ details;

ಶೂನ್ಯ releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    ಯದಿ (!g_isTlsStorageInitialized)
        ಪ್ರತ್ಯಾಗಮ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    ಕೀಲಿ_2 = (ಪೂರ್ಣಾಙ್ಕ)getTlsStorage().reserveSlot(ಏತತ್); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ಕೀಲಿ_2 == -1); // Key must be released in child object
}

ಶೂನ್ಯ TLSDataContainer::gatherData(std::vector<ಶೂನ್ಯ*> &data) ಧ್ರುವ
{
    getTlsStorage().gather(ಕೀಲಿ_2, data);
}

ಶೂನ್ಯ TLSDataContainer::detachData(std::vector<ಶೂನ್ಯ*> &data)
{
    getTlsStorage().releaseSlot(ಕೀಲಿ_2, data, ಸತ್ಯ);
}

ಶೂನ್ಯ TLSDataContainer::release()
{
    ಯದಿ (ಕೀಲಿ_2 == -1)
        ಪ್ರತ್ಯಾಗಮ;  // already released
    std::vector<ಶೂನ್ಯ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ಕೀಲಿ_2, data, ಅಸತ್ಯ); // Release key and get stored data for proper destruction
    ಕೀಲಿ_2 = -1;
    ಪ್ರತಿ(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ಶೂನ್ಯ TLSDataContainer::cleanup()
{
    std::vector<ಶೂನ್ಯ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(ಕೀಲಿ_2, data, ಸತ್ಯ); // Extract stored data with removal from TLS tables
    ಪ್ರತಿ(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

ಶೂನ್ಯ* TLSDataContainer::getData() ಧ್ರುವ
{
    CV_Assert(ಕೀಲಿ_2 != -1 && "Can't fetch data from terminated TLS container.");
    ಶೂನ್ಯ* pData = getTlsStorage().getData(ಕೀಲಿ_2); // Check if data was already allocated
    ಯದಿ(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        ಯತ್ನ
        {
            getTlsStorage().setData(ಕೀಲಿ_2, pData);
        }
        ಗೃಹಾಣ (...)
        {
            deleteDataInstance(pData);
            ಕ್ಷಿಪ;
        }
    }
    ಪ್ರತ್ಯಾಗಮ pData;
}

ಸ್ಥಿರ TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, ನೂತನ TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    ಪ್ರತ್ಯಾಗಮ getCoreTlsDataTLS().getRef();
}

#ಯದಿ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

ಬಾಹ್ಯ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

ಬಾಹ್ಯ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ಯದಿ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ಯದಿ (lpReserved != ರಿಕ್ತ) // called after ExitProcess() call
        {
            cv::__termination = ಸತ್ಯ;
        }
        ಅನ್ಯಥಾ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ಪ್ರತ್ಯಾಗಮ TRUE;
}
#endif


ನಾಮಾಕಾಶ {

#ifdef OPENCV_WITH_ITT
ತಾರ್ಕಿಕ overrideThreadName()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", ಅಸತ್ಯ);
    ಪ್ರತ್ಯಾಗಮ param;
}
#endif

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ g_thread_ಅಂಕೆ = 0;
ವರ್ಗ ThreadID {
ಸಾರ್ವಜನಿಕ:
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ id;
    ThreadID() :
        id(CV_XADD(&g_thread_ಅಂಕೆ, 1))
    {
#ifdef OPENCV_WITH_ITT
        ಯದಿ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

ಸ್ಥಿರ TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, ನೂತನ TLSData<ThreadID>());
}

} // namespace
ಪೂರ್ಣಾಙ್ಕ utils::getThreadID() { ಪ್ರತ್ಯಾಗಮ getThreadIDTLS().get()->id; }


ವರ್ಗ ParseError
{
    std::string bad_ಮೌಲ್ಯ;
ಸಾರ್ವಜನಿಕ:
    ParseError(ಧ್ರುವ std::string &bad_ಮೌಲ್ಯ_2) :bad_ಮೌಲ್ಯ(bad_ಮೌಲ್ಯ_2) {}
    std::string toString(ಧ್ರುವ std::string &param) ಧ್ರುವ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_ಮೌಲ್ಯ;
        ಪ್ರತ್ಯಾಗಮ out.str();
    }
};

template <typename T>
T parseOption(ಧ್ರುವ std::string &);

template<>
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ parseOption(ಧ್ರುವ std::string & value)
{
    ಯದಿ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಯದಿ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    }
    ಕ್ಷಿಪ ParseError(value);
}

template<>
ಅನ್ತಃಸ್ಥ size_t parseOption(ಧ್ರುವ std::string &value)
{
    size_t pos = 0;
    ಪ್ರತಿ (; pos < value.size(); pos++)
    {
        ಯದಿ (!isdigit(value[pos]))
            ಭಙ್ಗ;
    }
    cv::String ಮೌಲ್ಯ_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(ಮೌಲ್ಯ_Str);
    ಯದಿ (suffixStr.length() == 0)
        ಪ್ರತ್ಯಾಗಮ v;
    ಅನ್ಯಥಾ ಯದಿ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ಪ್ರತ್ಯಾಗಮ v * 1024 * 1024;
    ಅನ್ಯಥಾ ಯದಿ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ಪ್ರತ್ಯಾಗಮ v * 1024;
    ಕ್ಷಿಪ ParseError(value);
}

template<>
ಅನ್ತಃಸ್ಥ cv::String parseOption(ಧ್ರುವ std::string &value)
{
    ಪ್ರತ್ಯಾಗಮ value;
}

template<>
ಅನ್ತಃಸ್ಥ utils::Paths parseOption(ಧ್ರುವ std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ಧ್ರುವ ವರ್ಣ sep = ';';
#ಅನ್ಯಥಾ
    ಧ್ರುವ ವರ್ಣ sep = ':';
#endif
    size_t start_pos = 0;
    ಯಾವತ್ (start_pos != std::string::npos)
    {
        ಧ್ರುವ size_t pos = value.find(sep, start_pos);
        ಧ್ರುವ std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        ಯದಿ (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ಪ್ರತ್ಯಾಗಮ result;
}

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಧ್ರುವ ವರ್ಣ * envRead(ಧ್ರುವ ವರ್ಣ * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ getenv(name);
#endif
}

template<typename T>
ಅನ್ತಃಸ್ಥ T ಪಠ(ಧ್ರುವ std::string & k, ಧ್ರುವ T & default_ಮೌಲ್ಯ_2)
{
    ಯತ್ನ
    {
        ಧ್ರುವ ವರ್ಣ * res = envRead(k.c_str());
        ಯದಿ (res)
            ಪ್ರತ್ಯಾಗಮ parseOption<T>(std::string(res));
    }
    ಗೃಹಾಣ (ಧ್ರುವ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    ಪ್ರತ್ಯಾಗಮ default_ಮೌಲ್ಯ_2;
}

ತಾರ್ಕಿಕ utils::getConfigurationParameterBool(ಧ್ರುವ ವರ್ಣ* name, ತಾರ್ಕಿಕ default_ಮೌಲ್ಯ_2)
{
    ಪ್ರತ್ಯಾಗಮ ಪಠ<ತಾರ್ಕಿಕ>(name, default_ಮೌಲ್ಯ_2);
}

size_t utils::getConfigurationParameterSizeT(ಧ್ರುವ ವರ್ಣ* name, size_t default_ಮೌಲ್ಯ_2)
{
    ಪ್ರತ್ಯಾಗಮ ಪಠ<size_t>(name, default_ಮೌಲ್ಯ_2);
}

std::string utils::getConfigurationParameterString(ಧ್ರುವ ವರ್ಣ* name, ಧ್ರುವ std::string & default_ಮೌಲ್ಯ_2)
{
    ಪ್ರತ್ಯಾಗಮ ಪಠ<cv::String>(name, default_ಮೌಲ್ಯ_2);
}

utils::Paths utils::getConfigurationParameterPaths(ಧ್ರುವ ವರ್ಣ* name, ಧ್ರುವ utils::Paths &default_ಮೌಲ್ಯ_2)
{
    ಪ್ರತ್ಯಾಗಮ ಪಠ<utils::Paths>(name, default_ಮೌಲ್ಯ_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, ನೂತನ ImplCollector())
}

ಶೂನ್ಯ setImpl(ಪೂರ್ಣಾಙ್ಕ flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

ಶೂನ್ಯ addImpl(ಪೂರ್ಣಾಙ್ಕ flag, ಧ್ರುವ ವರ್ಣ* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    ಯದಿ(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        ಯದಿ(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

ಪೂರ್ಣಾಙ್ಕ getImpl(std::vector<ಪೂರ್ಣಾಙ್ಕ> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    ಪ್ರತ್ಯಾಗಮ getImplData().implFlags; // return actual flags for lazy collection
}

ತಾರ್ಕಿಕ useCollection()
{
    ಪ್ರತ್ಯಾಗಮ getImplData().useCollection;
}

ಶೂನ್ಯ setUseCollection(ತಾರ್ಕಿಕ flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

ನಾಮಾಕಾಶ instr
{
ತಾರ್ಕಿಕ useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    ಪ್ರತ್ಯಾಗಮ getInstrumentStruct().useInstr;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ setUseInstrumentation(ತಾರ್ಕಿಕ flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ಅನ್ಯಥಾ
    CV_UNUSED(flag);
#endif
}

Instr_ಗಂಟು* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    ಪ್ರತ್ಯಾಗಮ &getInstrumentStruct().ಬೇರು_ಗಂಟು_2;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#endif
}

ಶೂನ್ಯ resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().ಬೇರು_ಗಂಟು_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ಗಂಟು = &getInstrumentStruct().ಬೇರು_ಗಂಟು_2;
#endif
}

ಶೂನ್ಯ setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#ಅನ್ಯಥಾ
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    ಪ್ರತ್ಯಾಗಮ (FLAGS)getInstrumentStruct().flags;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ (FLAGS)0;
#endif
}

ಗಂಟು_Data::ಗಂಟು_Data(ಧ್ರುವ ವರ್ಣ* funName, ಧ್ರುವ ವರ್ಣ* fileName, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_ಅಂಕೆ, ಶೂನ್ಯ* retAddress, ತಾರ್ಕಿಕ alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_ಸಾಲು_ಅಂಕೆ       = ಸಾಲು_ಅಂಕೆ;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = ಅಸತ್ಯ;
}
ಗಂಟು_Data::ಗಂಟು_Data(ಗಂಟು_Data &ref)
{
    *ಏತತ್ = ref;
}
ಗಂಟು_Data& ಗಂಟು_Data::operator=(ಧ್ರುವ ಗಂಟು_Data &ಬಲ)
{
    ಏತತ್->m_funName      = ಬಲ.m_funName;
    ಏತತ್->m_instrType    = ಬಲ.m_instrType;
    ಏತತ್->m_implType     = ಬಲ.m_implType;
    ಏತತ್->m_fileName     = ಬಲ.m_fileName;
    ಏತತ್->m_ಸಾಲು_ಅಂಕೆ      = ಬಲ.m_ಸಾಲು_ಅಂಕೆ;
    ಏತತ್->m_retAddress   = ಬಲ.m_retAddress;
    ಏತತ್->m_alwaysExpand = ಬಲ.m_alwaysExpand;

    ಏತತ್->m_threads     = ಬಲ.m_threads;
    ಏತತ್->m_counter     = ಬಲ.m_counter;
    ಏತತ್->m_ticksTotal  = ಬಲ.m_ticksTotal;

    ಏತತ್->m_funError    = ಬಲ.m_funError;

    ಪ್ರತ್ಯಾಗಮ *ಏತತ್;
}
ಗಂಟು_Data::~ಗಂಟು_Data()
{
}
ತಾರ್ಕಿಕ operator==(ಧ್ರುವ ಗಂಟು_Data& ಎಡ, ಧ್ರುವ ಗಂಟು_Data& ಬಲ)
{
    ಯದಿ(ಎಡ.m_ಸಾಲು_ಅಂಕೆ == ಬಲ.m_ಸಾಲು_ಅಂಕೆ && ಎಡ.m_funName == ಬಲ.m_funName && ಎಡ.m_fileName == ಬಲ.m_fileName)
    {
        ಯದಿ(ಎಡ.m_retAddress == ಬಲ.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || ಎಡ.m_alwaysExpand))
            ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ಸ್ಥಿರ InstrStruct instr;
    ಪ್ರತ್ಯಾಗಮ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ಪ್ರತ್ಯಾಗಮ *getInstrumentStruct().tlsStruct.get();
}

Instr_ಗಂಟು* get_Current_ಗಂಟು()
{
    ಪ್ರತ್ಯಾಗಮ getInstrumentTLSStruct().p_Current_ಗಂಟು;
}

IntrumentationRegion::IntrumentationRegion(ಧ್ರುವ ವರ್ಣ* funName, ಧ್ರುವ ವರ್ಣ* fileName, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_ಅಂಕೆ, ಶೂನ್ಯ *retAddress, ತಾರ್ಕಿಕ alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = ಅಸತ್ಯ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ಯದಿ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ಯದಿ(!pTLS->p_Current_ಗಂಟು)
        {
            m_disabled = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಪೂರ್ಣಾಙ್ಕ depth = pTLS->p_Current_ಗಂಟು->getDepth();
        ಯದಿ(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = ಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಗಂಟು_Data payload(funName, fileName, ಸಾಲು_ಅಂಕೆ, retAddress, alwaysExpand, instrType, implType);
        Node<ಗಂಟು_Data>* pChild = ರಿಕ್ತ;

        ಯದಿ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_ಗಂಟು->findChild(payload);
            ಯದಿ(!pChild)
            {
                pChild = ನೂತನ Node<ಗಂಟು_Data>(payload);
                pTLS->p_Current_ಗಂಟು->addChild(pChild);
            }
        }
        ಅನ್ಯಥಾ
        {
            pChild = pTLS->p_Current_ಗಂಟು->findChild(payload);
            ಯದಿ(!pChild)
            {
                m_disabled = ಸತ್ಯ;
                ಪ್ರತ್ಯಾಗಮ;
            }
        }
        pTLS->p_Current_ಗಂಟು = pChild;

        m_regionTicks = get_Tick_ಎಣಿಕೆ();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ಯದಿ(pStruct->useInstr)
    {
        ಯದಿ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ಯದಿ (pTLS->p_Current_ಗಂಟು->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ಗಂಟು->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ಗಂಟು->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ಎಣಿಕೆ() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_ಎಣಿಕೆ); // Concurrent ticks accumulation
                pTLS->p_Current_ಗಂಟು->m_payload.m_counter++;
                pTLS->p_Current_ಗಂಟು->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_ಗಂಟು->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_ಗಂಟು = pTLS->p_Current_ಗಂಟು->m_pParent;
        }
    }
}
#endif
}

ನಾಮಾಕಾಶ ipp
{

#ifdef HAVE_IPP
ಸಂರಚನಾ IPPInitSingleton
{
ಸಾರ್ವಜನಿಕ:
    IPPInitSingleton()
    {
        useIPP         = ಸತ್ಯ;

#ಯದಿ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ಅನ್ಯಥಾ
        useIPP_NE      = ಅಸತ್ಯ;
#endif
        ippStatus      = 0;
        funcname       = ರಿಕ್ತ;
        filename       = ರಿಕ್ತ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_ತುದಿ_Features = 0;
        pIppLibInfo    = ರಿಕ್ತ;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, ರಿಕ್ತ);
        ಯದಿ(ippStatus < 0)
        {
            CV_LOG_ERROR(ರಿಕ್ತ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = ಅಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ಯದಿ(!env.empty())
        {
#ಯದಿ IPP_VERSION_X100 >= 201900
            ಧ್ರುವ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#ನೋಚೇದ್ಯದಿ IPP_VERSION_X100 >= 201703
            ಧ್ರುವ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ನೋಚೇದ್ಯದಿ IPP_VERSION_X100 >= 201700
            ಧ್ರುವ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ಅನ್ಯಥಾ
            ಧ್ರುವ Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            ಯದಿ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = ಸತ್ಯ;
                env = env.substr(3, env.size());
            }

            ಯದಿ(env == "disabled")
            {
                CV_LOG_WARNING(ರಿಕ್ತ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = ಅಸತ್ಯ;
            }
            ಅನ್ಯಥಾ ಯದಿ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ಅನ್ಯಥಾ ಯದಿ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ಯದಿ IPP_VERSION_X100 >= 201700
#ಯದಿ defined (_M_AMD64) || defined (__x86_64__)
            ಅನ್ಯಥಾ ಯದಿ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            ಅನ್ಯಥಾ
                CV_LOG_ERROR(ರಿಕ್ತ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ಯದಿ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ಯದಿ(!(
#ಯದಿ IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = ಅಸತ್ಯ;
            ಪ್ರತ್ಯಾಗಮ;
        }

        ಯದಿ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ಅನ್ಯಥಾ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ಯದಿ IPP_VERSION_X100 >= 201700
        ಯದಿ(ippFeatures&ippCPUID_AVX512F)
        {
            ಯದಿ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_ತುದಿ_Features = ippCPUID_AVX512_SKX;
            ಅನ್ಯಥಾ ಯದಿ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_ತುದಿ_Features = ippCPUID_AVX512_KNL;
            ಅನ್ಯಥಾ
                ipp_ತುದಿ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ಅನ್ಯಥಾ
#endif
        ಯದಿ(ippFeatures&ippCPUID_AVX2)
            ipp_ತುದಿ_Features = ippCPUID_AVX2;
        ಅನ್ಯಥಾ ಯದಿ(ippFeatures&ippCPUID_SSE42)
            ipp_ತುದಿ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ಯದಿ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_ತುದಿ_Features = ippCPUID_SSE42;
        }
    }

ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ        useIPP;
    ತಾರ್ಕಿಕ        useIPP_NE;

    ಪೂರ್ಣಾಙ್ಕ         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ಧ್ರುವ ವರ್ಣ *funcname;
    ಧ್ರುವ ವರ್ಣ *filename;
    ಪೂರ್ಣಾಙ್ಕ         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_ತುದಿ_Features;
    ಧ್ರುವ IppLibraryVersion *pIppLibInfo;
};

ಸ್ಥಿರ IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, ನೂತನ IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    ಪ್ರತ್ಯಾಗಮ getIPPSingleton().ippFeatures;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_ತುದಿ_Features()
{
    ಪ್ರತ್ಯಾಗಮ getIPPSingleton().ipp_ತುದಿ_Features;
}
#endif

ಶೂನ್ಯ setIppStatus(ಪೂರ್ಣಾಙ್ಕ status, ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ _funcname, ಧ್ರುವ ವರ್ಣ * ಧ್ರುವ _filename, ಪೂರ್ಣಾಙ್ಕ ಸಾಲು_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = ಸಾಲು_3;
#ಅನ್ಯಥಾ
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(ಸಾಲು_3);
#endif
}

ಪೂರ್ಣಾಙ್ಕ getIppStatus()
{
#ifdef HAVE_IPP
    ಪ್ರತ್ಯಾಗಮ getIPPSingleton().ippStatus;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    ಪ್ರತ್ಯಾಗಮ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ಧ್ರುವ IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    ಯದಿ(pInfo)
        ಪ್ರತ್ಯಾಗಮ format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ String("error");
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ String("disabled");
#endif
}

ತಾರ್ಕಿಕ useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ಯದಿ (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    ಪ್ರತ್ಯಾಗಮ (data.useIPP > 0);
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ setUseIPP(ತಾರ್ಕಿಕ flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:ಅಸತ್ಯ;
#ಅನ್ಯಥಾ
    CV_UNUSED(flag);
    data.useIPP = ಅಸತ್ಯ;
#endif
}

ತಾರ್ಕಿಕ useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ಯದಿ (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ಪ್ರತ್ಯಾಗಮ (data.useIPP_NE > 0);
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

ಶೂನ್ಯ setUseIPP_NotExact(ತಾರ್ಕಿಕ flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#ಅನ್ಯಥಾ
    CV_UNUSED(flag);
    data.useIPP_NE = ಅಸತ್ಯ;
#endif
}

} // namespace ipp


ನಾಮಾಕಾಶ details {

#ಯದಿ OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

ಶೂನ್ಯ setFPDenormalsIgnoreHint(ತಾರ್ಕಿಕ ignore, CV_OUT FPDenormalsModeState& state)
{
#ಯದಿ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ಯದಿ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ಧ್ರುವ unsigned old_flags = _mm_getcsr();
    ಧ್ರುವ unsigned old_ಮೌಲ್ಯ = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ರಿಕ್ತ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_ಮೌಲ್ಯ;
    _mm_setcsr(flags);
#ಅನ್ಯಥಾ
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

ಪೂರ್ಣಾಙ್ಕ saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#ಯದಿ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    ಯದಿ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ಧ್ರುವ unsigned old_flags = _mm_getcsr();
    ಧ್ರುವ unsigned old_ಮೌಲ್ಯ = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_ಮೌಲ್ಯ;
    ಪ್ರತ್ಯಾಗಮ 2;
#ಅನ್ಯಥಾ
    CV_UNUSED(state);
    ಪ್ರತ್ಯಾಗಮ 0;
#endif
}

ತಾರ್ಕಿಕ restoreFPDenormalsState(ಧ್ರುವ FPDenormalsModeState& state)
{
#ಯದಿ OPENCV_IMPL_FP_HINTS_X86
    ಧ್ರುವ unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ಧ್ರುವ unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((ಪೂರ್ಣಾಙ್ಕ)value, value == (value & mask), "invalid SSE FP state");
    ಧ್ರುವ unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ರಿಕ್ತ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ಪ್ರತ್ಯಾಗಮ ಸತ್ಯ;
#ಅನ್ಯಥಾ
    CV_UNUSED(state);
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    ಪ್ರತ್ಯಾಗಮ OPENCV_ALGO_HINT_DEFAULT;
#ಅನ್ಯಥಾ
    ಪ್ರತ್ಯಾಗಮ ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
