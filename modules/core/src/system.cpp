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

#include "precomp.hpp"
//#include "opencv2/core/core_c.h"
#include <atomic>
#include <exception>
#include <iostream>
#include <ostream>

#ifdef __QNX__
    #include <unistd.h>
    #include <sys/neutrino.h>
    #include <sys/syspage.h>
#ifdef __aarch64__
    #include <aarch64/syspage.h>
#endif
#endif

#include <opencv2/core/utils/configuration.private.hpp>
#include <opencv2/core/utils/trace.private.hpp>

#include <opencv2/core/utils/logger.hpp>

#include <opencv2/core/utils/tls.hpp>
#include <opencv2/core/utils/instrumentation.hpp>

#include <opencv2/core/utils/filesystem.private.hpp>

#include <opencv2/core/utils/fp_control_utils.hpp>
#include <opencv2/core/utils/fp_control.private.hpp>

namespace cv {

static void _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    static std::ios_base::Init s_iostream_initializer;
#endif
}

static Mutex* __initialization_mutex = NULL;
Mutex& getInitializationMutex()
{
    ᤀᤡ᤺ (__initialization_mutex == NULL)
    {
        (void)_initSystem();
        __initialization_mutex = new Mutex();
    }
    ᤀᤡᤴ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

static bool param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ᤀᤡ᤺ defined(_DEBUG) || defined(__ANDROID__)
    true
#ᤀᤡ᤺ᤆᤡ
    false
#endif
);

void* allocSingletonBuffer(size_t size) { ᤀᤡᤴ fastMalloc(size); }
void* allocSingletonNewBuffer(size_t size) { ᤀᤡᤴ malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# ᤀᤡ᤺ defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#ᤀᤡ᤺ defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# ᤀᤡ᤺ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#include <cstdlib>        // std::abort
#endif

#ᤀᤡ᤺ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ᤀᤡ᤺ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
const uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ᤀᤡ᤺ᤆᤡ
#  include <elf.h>
#endif
#ᤀᤡ᤺ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#ᤀᤡ᤺ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#ᤀᤡ᤺ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#ᤀᤡ᤺ defined __loongarch64
#include "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#ᤀᤡ᤺ defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#include <windows.h>
#include <combaseapi.h>
#ᤀᤡ᤺ (_WIN32_WINNT >= 0x0602)
  #include <synchapi.h>
#endif
#ᤀᤡ᤺ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #include <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#include <tchar.h>

#ifdef WINRT
#include <wrl/client.h>
#ifndef __cplusplus_winrt
#include <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    ᤀᤡᤴ std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#ᤀᤡ᤺ᤆᤡ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    ᤀᤡ᤺ (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        ᤀᤡᤴ wstr;
    ᤀᤡ᤺ (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        ᤀᤡᤴ wstr;
    ᤀᤡ᤺ (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        ᤀᤡᤴ wstr;
    ᤀᤡ᤺ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        ᤀᤡᤴ wstr;
    ᤀᤡ᤺ (FAILED(storagefolderRef.As(&storageitemRef)))
        ᤀᤡᤴ wstr;
    str = NULL;
    ᤀᤡ᤺ (FAILED(storageitemRef->get_Path(&str)))
        ᤀᤡᤴ wstr;
    wstr = WindowsGetStringRawBuffer(str, NULL);
    WindowsDeleteString(str);
    ᤀᤡᤴ wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], parimaa_na(guidStr)/parimaa_na(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ᤀᤡᤴ prefix.append(std::wstring(guidStr));
}

#endif
#ᤀᤡ᤺ᤆᤡ
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#include <pthread.h>
#endif
#include <sys/time.h>
#include <time.h>

#ᤀᤡ᤺ defined __MACH__ && defined __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#include "omp.h"
#endif

#ᤀᤡ᤺ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#ᤀᤡ᤺ defined __ANDROID__
#include <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #ᤀᤡ᤺ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ᤀᤡ᤺ _MSC_VER >= 1400  // MSVS 2005
      #include <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #ᤀᤡ᤺ᤆᤡ
      #error "Required MSVS 2005+"
    #endif
  #elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    static void cv_cpuid(puur_naa_nka* cpuid_data, puur_naa_nka reg_eax, puur_naa_nka reg_ecx)
    {
        puur_naa_nka __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ᤀᤡ᤺ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#elif defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ᤀᤡ᤺ᤆᤡ
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#include <chrono>

namespace cv
{

Exception::Exception() { code = Error::StsOk; line = 0; }

Exception::Exception(Error::Code _code, const String& _err, const String& _func, const String& _file, puur_naa_nka _line)
: code(_code), err(_err), func(_func), file(_file), line(_line)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
const var_na* Exception::what() const CV_NOEXCEPT { ᤀᤡᤴ msg.c_str(); }

void Exception::formatMessage()
{
    size_t pos = err.find('\n');
    bool multiline = pos != cv::String::npos;
    ᤀᤡ᤺ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ᤀᤠ᤺ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ᤀᤡ᤺ (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    ᤀᤡ᤺ (func.size() > 0)
    {
        ᤀᤡ᤺ (multiline)
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), line, code, codeMessage(), func.c_str(), err.c_str());
        ᤀᤡ᤺ᤆᤡ
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), func.c_str());
    }
    ᤀᤡ᤺ᤆᤡ
    {
        msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

const var_na * Exception::codeMessage() const
{
    switch (code)
    {
    pak_sa cv::Error::StsOk :                  ᤀᤡᤴ "No Error";
    pak_sa cv::Error::StsBackTrace :           ᤀᤡᤴ "Backtrace";
    pak_sa cv::Error::StsError :               ᤀᤡᤴ "Unspecified error";
    pak_sa cv::Error::StsInternal :            ᤀᤡᤴ "Internal error";
    pak_sa cv::Error::StsNoMem :               ᤀᤡᤴ "Insufficient memory";
    pak_sa cv::Error::StsBadArg :              ᤀᤡᤴ "Bad argument";
    pak_sa cv::Error::StsNoConv :              ᤀᤡᤴ "Iterations do not converge";
    pak_sa cv::Error::StsAutoTrace :           ᤀᤡᤴ "Autotrace call";
    pak_sa cv::Error::StsBadSize :             ᤀᤡᤴ "Incorrect size of input array";
    pak_sa cv::Error::StsNullPtr :             ᤀᤡᤴ "Null pointer";
    pak_sa cv::Error::StsDivByZero :           ᤀᤡᤴ "Division by zero occurred";
    pak_sa cv::Error::BadStep :                ᤀᤡᤴ "Image step is wrong";
    pak_sa cv::Error::StsInplaceNotSupported : ᤀᤡᤴ "Inplace operation is not supported";
    pak_sa cv::Error::StsObjectNotFound :      ᤀᤡᤴ "Requested object was not found";
    pak_sa cv::Error::BadDepth :               ᤀᤡᤴ "Input image depth is not supported by function";
    pak_sa cv::Error::StsUnmatchedFormats :    ᤀᤡᤴ "Formats of input arguments do not match";
    pak_sa cv::Error::StsUnmatchedSizes :      ᤀᤡᤴ "Sizes of input arguments do not match";
    pak_sa cv::Error::StsOutOfRange :          ᤀᤡᤴ "One of the arguments\' values is out of range";
    pak_sa cv::Error::StsUnsupportedFormat :   ᤀᤡᤴ "Unsupported format or combination of formats";
    pak_sa cv::Error::BadCOI :                 ᤀᤡᤴ "Input COI is not supported";
    pak_sa cv::Error::BadNumChannels :         ᤀᤡᤴ "Bad number of channels";
    pak_sa cv::Error::StsBadFlag :             ᤀᤡᤴ "Bad flag (parameter or structure field)";
    pak_sa cv::Error::StsBadPoint :            ᤀᤡᤴ "Bad parameter of type CvPoint";
    pak_sa cv::Error::StsBadMask :             ᤀᤡᤴ "Bad type of mask argument";
    pak_sa cv::Error::StsParseError :          ᤀᤡᤴ "Parsing error";
    pak_sa cv::Error::StsNotImplemented :      ᤀᤡᤴ "The function/feature is not implemented";
    pak_sa cv::Error::StsBadMemBlock :         ᤀᤡᤴ "Memory block has been corrupted";
    pak_sa cv::Error::StsAssert :              ᤀᤡᤴ "Assertion failed";
    pak_sa cv::Error::GpuNotSupported :        ᤀᤡᤴ "No CUDA support";
    pak_sa cv::Error::GpuApiCallError :        ᤀᤡᤴ "Gpu API call";
    pak_sa cv::Error::OpenGlNotSupported :     ᤀᤡᤴ "No OpenGL support";
    pak_sa cv::Error::OpenGlApiCallError :     ᤀᤡᤴ "OpenGL API call";
    pak_sa cv::Error::OpenCLApiCallError :     ᤀᤡᤴ "OpenCL API call error";
    pak_sa cv::Error::OpenCLDoubleNotSupported:ᤀᤡᤴ "OpenCL device does not support double";
    pak_sa cv::Error::OpenCLInitError :        ᤀᤡᤴ "OpenCL initialization error";
    pak_sa cv::Error::OpenCLNoAMDBlasFft :     ᤀᤡᤴ "Missing OpenCL AMD BLAS FFT";
    default:
    {
        static var_na buf[256] {0};
        snprintf(buf, parimaa_na(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        ᤀᤡᤴ buf;
    }
    };

}

static const var_na* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { NULL };

static const var_na* getHWFeatureName(puur_naa_nka id)
{
    ᤀᤡᤴ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : NULL;
}
static const var_na* getHWFeatureNameSafe(puur_naa_nka id)
{
    const var_na* name = getHWFeatureName(id);
    ᤀᤡᤴ name ? name : "Unknown feature";
}

sa_nracanaa HWFeatures
{
    ga_nanaa { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(bool run_initialize = false)
    {
        ᤀᤡ᤺ (run_initialize)
            initialize();
    }

    static void initializeNames()
    {
        for (puur_naa_nka i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    void initialize(void)
    {
        ᤀᤡ᤺ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        puur_naa_nka cpuid_data[4] = { 0, 0, 0, 0 };
        puur_naa_nka cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        puur_naa_nka x86_family = (cpuid_data[0] >> 8) & 15;
        ᤀᤡ᤺( x86_family >= 6 )
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
            puur_naa_nka cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            bool have_AVX_OS_support = true;
            bool have_AVX512_OS_support = true;
            ᤀᤡ᤺ (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = false; // OS uses XSAVE_XRSTORE and CPU support AVX
            ᤀᤡ᤺ᤆᤡ
            {
                puur_naa_nka xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (puur_naa_nka)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                ᤀᤡ᤺ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = false; // YMM registers
                ᤀᤡ᤺ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = false; // ZMM registers
            }

            ᤀᤡ᤺ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = false;
                have[CV_CPU_FP16] = false;
                have[CV_CPU_AVX2] = false;
                have[CV_CPU_FMA3] = false;
                have[CV_CPU_AVX_VNNI] = false;
            }
            ᤀᤡ᤺ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = false;
                have[CV_CPU_AVX_512BW] = false;
                have[CV_CPU_AVX_512CD] = false;
                have[CV_CPU_AVX_512DQ] = false;
                have[CV_CPU_AVX_512ER] = false;
                have[CV_CPU_AVX_512IFMA] = false;
                have[CV_CPU_AVX_512PF] = false;
                have[CV_CPU_AVX_512VBMI] = false;
                have[CV_CPU_AVX_512VL] = false;
                have[CV_CPU_AVX_512VBMI2] = false;
                have[CV_CPU_AVX_512VNNI] = false;
                have[CV_CPU_AVX_512BITALG] = false;
                have[CV_CPU_AVX_512VPOPCNTDQ] = false;
                have[CV_CPU_AVX_5124VNNIW] = false;
                have[CV_CPU_AVX_5124FMAPS] = false;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ᤀᤡ᤺ (have[CV_CPU_AVX512_COMMON])
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
            ᤀᤡ᤺ᤆᤡ
            {
                have[CV_CPU_AVX512_KNL] = false;
                have[CV_CPU_AVX512_KNM] = false;
                have[CV_CPU_AVX512_SKX] = false;
                have[CV_CPU_AVX512_CNL] = false;
                have[CV_CPU_AVX512_CLX] = false;
                have[CV_CPU_AVX512_ICL] = false;
            }
        }
    #endif // CV_CPUID_X86

    #ᤀᤡ᤺ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = true;
        have[CV_CPU_FP16] = true;
        puur_naa_nka cpufile = open("/proc/self/auxv", O_RDONLY);

        ᤀᤡ᤺ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            const size_t size_auxv_t = parimaa_na(auxv);

            ᤀᤠ᤺ ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ᤀᤡ᤺ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ᤀᤡ᤺ defined(AT_HWCAP2)
                ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #elif defined __arm__ && defined __ANDROID__
      #ᤀᤡ᤺ defined HAVE_CPUFEATURES
        CV_LOG_INFO(NULL, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(NULL, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ᤀᤡ᤺ᤆᤡ
        CV_LOG_INFO(NULL, "cpufeatures library is not available for CPU detection");
        #ᤀᤡ᤺ CV_NEON
        CV_LOG_INFO(NULL, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = true;
        #ᤀᤡ᤺ᤆᤡ
        CV_LOG_INFO(NULL, "- NEON instructions is NOT enabled via build flags");
        #endif
        #ᤀᤡ᤺ CV_FP16
        CV_LOG_INFO(NULL, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = true;
        #ᤀᤡ᤺ᤆᤡ
        CV_LOG_INFO(NULL, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #elif defined __arm__
        puur_naa_nka cpufile = open("/proc/self/auxv", O_RDONLY);

        ᤀᤡ᤺ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            const size_t size_auxv_t = parimaa_na(auxv);

            ᤀᤠ᤺ ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                ᤀᤡ᤺ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    bha_nga;
                }
            }

            close(cpufile);
        }
    #endif
    #elif (defined __APPLE__)
    #ᤀᤡ᤺ defined __ARM_NEON
        have[CV_CPU_NEON] = true;
    #endif
    #ᤀᤡ᤺ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = true;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    puur_naa_nka has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = parimaa_na(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, NULL, 0);
    ᤀᤡ᤺ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = true;
    }
    puur_naa_nka has_feat_bf16 = 0;
    size_t has_feat_bf16_size = parimaa_na(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, NULL, 0);
    ᤀᤡ᤺ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = true;
    }
    #elif (defined __clang__)
    #ᤀᤡ᤺ defined __ARM_NEON
        have[CV_CPU_NEON] = true;
        #ᤀᤡ᤺ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = true;
        #endif
    #endif
    #endif
    #ᤀᤡ᤺ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = true;
    #endif
    #ᤀᤡ᤺ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = true;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = true;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = true;
    #endif

    #ᤀᤡ᤺ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned puur_naa_nka hwcap = getauxval(AT_HWCAP);
        ᤀᤡ᤺ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ᤀᤡ᤺ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } ᤀᤡ᤺ᤆᤡ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #elif (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        ᤀᤡ᤺ (elf_aux_info(AT_HWCAP, &hwcap, parimaa_na(hwcap)) != 0)
            hwcap = 0;
        ᤀᤡ᤺ (hwcap & PPC_FEATURE_HAS_VSX) {
            ᤀᤡ᤺ (elf_aux_info(AT_HWCAP2, &hwcap2, parimaa_na(hwcap2)) != 0)
                hwcap2 = 0;
            ᤀᤡ᤺ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } ᤀᤡ᤺ᤆᤡ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ᤀᤡ᤺ᤆᤡ
        // TODO: AIX
        #ᤀᤡ᤺ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = true;
        #endif
        #ᤀᤡ᤺ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = true;
        #endif
    #endif

    #ᤀᤡ᤺ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = true;
        #ᤀᤡ᤺ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = true;
        #endif
        #ᤀᤡ᤺ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = true;
        #endif
    #endif

    #ᤀᤡ᤺ defined __loongarch64 && defined __linux__
        puur_naa_nka flag = (puur_naa_nka)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        bool skip_baseline_check = false;
        ᤀᤡ᤺ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = true;
        }
        puur_naa_nka baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ᤀᤡ᤺ (!checkFeatures(baseline_features, parimaa_na(baseline_features) / parimaa_na(baseline_features[0]))
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
            checkFeatures(baseline_features, parimaa_na(baseline_features) / parimaa_na(baseline_features[0]), true);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, parimaa_na(baseline_features) / parimaa_na(baseline_features[0]));
    }

    bool checkFeatures(const puur_naa_nka* features, puur_naa_nka count, bool dump = false)
    {
        bool result = true;
        for (puur_naa_nka i = 0; i < count; i++)
        {
            puur_naa_nka feature = features[i];
            ᤀᤡ᤺ (feature)
            {
                ᤀᤡ᤺ (have[feature])
                {
                    ᤀᤡ᤺ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                ᤀᤡ᤺ᤆᤡ
                {
                    result = false;
                    ᤀᤡ᤺ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        ᤀᤡᤴ result;
    }

    static anta_hstha bool isSymbolSeparator(var_na c)
    {
        ᤀᤡᤴ c == ',' || c == ';';
    }

    void readSettings(const puur_naa_nka* baseline_features, puur_naa_nka baseline_count)
    {
        bool dump = true;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ᤀᤡ᤺ (!disabled_features.empty())
        {
            const var_na* start = disabled_features.c_str();
            for (;;)
            {
                ᤀᤠ᤺ (start[0] != 0 && isSymbolSeparator(start[0]))
                {
                    start++;
                }
                ᤀᤡ᤺ (start[0] == 0)
                    bha_nga;
                const var_na* end = start;
                ᤀᤠ᤺ (end[0] != 0 && !isSymbolSeparator(end[0]))
                {
                    end++;
                }
                ᤀᤡ᤺ (end == start)
                    continue;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                bool found = false;
                for (puur_naa_nka i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ᤀᤡ᤺ (!g_hwFeatureNames[i]) continue;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    ᤀᤡ᤺ (len != feature.size()) continue;
                    ᤀᤡ᤺ (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        bool isBaseline = false;
                        for (puur_naa_nka k = 0; k < baseline_count; k++)
                        {
                            ᤀᤡ᤺ (baseline_features[k] == i)
                            {
                                isBaseline = true;
                                bha_nga;
                            }
                        }
                        ᤀᤡ᤺ (isBaseline)
                        {
                            ᤀᤡ᤺ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        ᤀᤡ᤺ (!have[i])
                        {
                            ᤀᤡ᤺ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = false;

                        found = true;
                        bha_nga;
                    }
                }
                ᤀᤡ᤺ (!found)
                {
                    ᤀᤡ᤺ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    bool have[MAX_FEATURE+1]{};
};

static HWFeatures  featuresEnabled(true), featuresDisabled = HWFeatures(false);
static HWFeatures* currentFeatures = &featuresEnabled;

bool checkHardwareSupport(puur_naa_nka feature)
{
    ᤀᤡ᤺ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ᤀᤡᤴ false;
    ᤀᤡᤴ currentFeatures->have[feature];
}

String getHardwareFeatureName(puur_naa_nka feature)
{
    const var_na* name = getHWFeatureName(feature);
    ᤀᤡᤴ name ? String(name) : String();
}

std::string getCPUFeaturesLine()
{
    const puur_naa_nka features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    const puur_naa_nka sz = parimaa_na(features) / parimaa_na(features[0]);
    std::string result;
    std::string prefix;
    for (puur_naa_nka i = 1; i < sz; ++i)
    {
        ᤀᤡ᤺ (features[i] == 0)
        {
            prefix = "*";
            continue;
        }
        ᤀᤡ᤺ (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        ᤀᤡ᤺ (!checkHardwareSupport(features[i])) result.append("?");
    }
    ᤀᤡᤴ result;
}

ca_ncala bool useOptimizedFlag = true;

void setUseOptimized( bool flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

bool useOptimized(void)
{
    ᤀᤡᤴ useOptimizedFlag;
}

int64 getTickCount(void)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ᤀᤡᤴ (int64)now.time_since_epoch().count();
}

dviguu_na getTickFrequency(void)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    dviguu_na clock_freq = clock_period_t::den / clock_period_t::num;
    ᤀᤡᤴ clock_freq;
}

#ᤀᤡ᤺ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ᤀᤡ᤺ defined(__i386__)

int64 getCPUTickCount(void)
{
    int64 x;
    __asm__ ca_ncala (".byte 0x0f, 0x31" : "=A" (x));
    ᤀᤡᤴ x;
}
#elif defined(__x86_64__)

int64 getCPUTickCount(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ᤀᤡᤴ (int64)lo | ((int64)hi << 32);
}

#elif defined(__ppc__)

int64 getCPUTickCount(void)
{
    unsigned upper, lower, tmp;
    __asm__ ca_ncala(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ᤀᤡᤴ lower | ((int64)upper << 32);
}

#ᤀᤡ᤺ᤆᤡ

#error "RDTSC not defined"

#endif

#elif defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 getCPUTickCount(void)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ᤀᤡ᤺ᤆᤡ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 getCPUTickCount(void)
{
    ᤀᤡᤴ getTickCount();
}
//#endif

#endif


namespace internal {

class Timestamp
{
public:
    const int64 zeroTickCount;
    const dviguu_na ns_in_ticks;

    Timestamp()
        : zeroTickCount(getTickCount())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = getTickCount();
        ᤀᤡᤴ (int64)((t - zeroTickCount) * ns_in_ticks);
    }

    static Timestamp& getInstance()
    {
        static Timestamp g_timestamp;
        ᤀᤡᤴ g_timestamp;
    }
};

class InitTimestamp {
public:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
static InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ᤀᤡᤴ internal::Timestamp::getInstance().getTimestamp();
}


const String& getBuildInformation()
{
    static String build_info =
#include "version_string.inc"
    ;
    ᤀᤡᤴ build_info;
}

String getVersionString() { ᤀᤡᤴ String(CV_VERSION); }

puur_naa_nka getVersionMajor() { ᤀᤡᤴ CV_VERSION_MAJOR; }

puur_naa_nka getVersionMinor() { ᤀᤡᤴ CV_VERSION_MINOR; }

puur_naa_nka getVersionRevision() { ᤀᤡᤴ CV_VERSION_REVISION; }

String format( const var_na* fmt, ... )
{
    AutoBuffer<var_na, 1024> buf;

    for ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        puur_naa_nka bsize = static_cast<puur_naa_nka>(buf.size());
        puur_naa_nka len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ᤀᤡ᤺ (len >= bsize)
        {
            buf.resize(len + 1);
            continue;
        }
        buf[bsize - 1] = 0;
        ᤀᤡᤴ String(buf.data(), len);
    }
}

String tempfile( const var_na* suffix )
{
#ᤀᤡ᤺ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ᤀᤡ᤺ defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    ᤀᤡ᤺ (temp_file.empty())
        ᤀᤡᤴ String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    var_na aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#elif defined(_WIN32_WCE)
    const auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    ᤀᤡ᤺(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        var_na aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ᤀᤡ᤺ᤆᤡ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    ᤀᤡ᤺ (temp_dir.empty())
    {
        ::GetTempPathW(parimaa_na(temp_dir2_w)/parimaa_na(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        puur_naa_nka len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, NULL, 0, NULL, NULL);
        ᤀᤡ᤺ (len > 0)
        {
            std::vector<var_na> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, NULL, NULL);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ᤀᤡ᤺ (FAILED(hr))
        ᤀᤡᤴ String();
    var_na guidStr[40];
    const var_na* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, parimaa_na(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned puur_naa_nka)g.Data4[0], (unsigned puur_naa_nka)g.Data4[1],
            (unsigned puur_naa_nka)g.Data4[2], (unsigned puur_naa_nka)g.Data4[3], (unsigned puur_naa_nka)g.Data4[4],
            (unsigned puur_naa_nka)g.Data4[5], (unsigned puur_naa_nka)g.Data4[6], (unsigned puur_naa_nka)g.Data4[7]);

    fname = temp_dir;
    ᤀᤡ᤺ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# ᤀᤡ᤺ᤆᤡ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    var_na defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ᤀᤡ᤺ᤆᤡ
    var_na defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ᤀᤡ᤺ (temp_dir.empty())
        fname = defaultTemplate;
    ᤀᤡ᤺ᤆᤡ
    {
        fname = temp_dir;
        var_na ech = fname[fname.size() - 1];
        ᤀᤡ᤺(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    const puur_naa_nka fd = mkstemp((var_na*)fname.c_str());
    ᤀᤡ᤺ (fd == -1) ᤀᤡᤴ String();

    close(fd);
    remove(fname.c_str());
# endif

    ᤀᤡ᤺ (suffix)
    {
        ᤀᤡ᤺ (suffix[0] != '.')
            ᤀᤡᤴ fname + "." + suffix;
        ᤀᤡ᤺ᤆᤡ
            ᤀᤡᤴ fname + suffix;
    }
    ᤀᤡᤴ fname;
#ᤀᤡ᤺ᤆᤡ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

static ErrorCallback customErrorCallback = 0;
static void* customErrorCallbackData = 0;
static bool breakOnError = false;

bool setBreakOnError(bool value)
{
    bool prevVal = breakOnError;
    breakOnError = value;
    ᤀᤡᤴ prevVal;
}

puur_naa_nka cv_snprintf(var_na* buf, puur_naa_nka len, const var_na* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    puur_naa_nka res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ᤀᤡᤴ res;
}

puur_naa_nka cv_vsnprintf(var_na* buf, puur_naa_nka len, const var_na* fmt, va_list args)
{
#ᤀᤡ᤺ defined _MSC_VER
    ᤀᤡ᤺ (len <= 0) ᤀᤡᤴ len == 0 ? 1024 : -1;
    puur_naa_nka res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    ᤀᤡ᤺ (res >= 0 && res < len)
    {
        buf[res] = 0;
        ᤀᤡᤴ res;
    }
    ᤀᤡ᤺ᤆᤡ
    {
        buf[len - 1] = 0; // truncate happened
        ᤀᤡᤴ res >= len ? res : (len * 2);
    }
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ vsnprintf(buf, len, fmt, args);
#endif
}

static void dumpException(const Exception& exc)
{
    const var_na* errorStr = exc.codeMessage();
    var_na buf[1 << 12];

    cv_snprintf(buf, parimaa_na(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.line);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ᤀᤡ᤺ᤆᤡ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
static bool cv_terminate_handler_installed = false;
static std::terminate_handler cv_old_terminate_handler;
static cv::Exception cv_terminate_handler_exception;
static bool param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", true);
static void cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ᤀᤡ᤺ (false /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# ᤀᤡ᤺ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

void error( const Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        ᤀᤡ᤺ (!cv_terminate_handler_installed)
        {
            ᤀᤡ᤺ (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = true;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    ᤀᤡ᤺ (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.line, customErrorCallbackData);
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (param_dumpErrors)
    {
        dumpException(exc);
    }

    ᤀᤡ᤺(breakOnError)
    {
        std::terminate();
    }

    k_sipa exc;
#ifdef __GNUC__
# ᤀᤡ᤺ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

void error(Error::Code _code, const String& _err, const var_na* _func, const var_na* _file, puur_naa_nka _line)
{
    error(cv::Exception(_code, _err, _func, _file, _line));
#ifdef __GNUC__
# ᤀᤡ᤺ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# ᤀᤡ᤺ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, void* userdata, void** prevUserdata)
{
    ᤀᤡ᤺( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    ᤀᤡᤴ prevCallback;
}

void terminate(Error::Code _code, const String& _err, const var_na* _func, const var_na* _file, puur_naa_nka _line) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, _line));
    std::terminate();
}

}

namespace cv {
bool __termination = false;


//////////////////////////////// thread-local storage ////////////////////////////////

namespace details {

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
class TlsAbstraction
{
public:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    void* getData() const;
    void setData(void *pData);

    void releaseSystemResources();

private:

#ifdef _WIN32
#ifndef WINRT
    DWORD tlsKey;
    bool disposed;
#endif
#ᤀᤡ᤺ᤆᤡ // _WIN32
    pthread_key_t  tlsKey;
    std::atomic<bool> disposed;
#endif
};

class TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
public:
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
static TlsAbstraction* getTlsAbstraction()
{
    static TlsAbstraction *g_tls = new TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    static TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ᤀᤡᤴ g_tls;
}


#ifdef _WIN32
#ifdef WINRT
static __declspec( thread ) void* tlsData = NULL; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
void TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
}
void* TlsAbstraction::getData() const
{
    ᤀᤡᤴ tlsData;
}
void TlsAbstraction::setData(void *pData)
{
    tlsData = pData;
}
#ᤀᤡ᤺ᤆᤡ //WINRT
#ifdef CV_USE_FLS
static void NTAPI opencv_fls_destructor(void* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(false)
{
#ifndef CV_USE_FLS
    tlsKey = TlsAlloc();
#ᤀᤡ᤺ᤆᤡ // CV_USE_FLS
    tlsKey = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tlsKey != TLS_OUT_OF_INDEXES);
}
void TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
#ifndef CV_USE_FLS
    TlsFree(tlsKey);
#ᤀᤡ᤺ᤆᤡ // CV_USE_FLS
    FlsFree(tlsKey);
#endif // CV_USE_FLS
    tlsKey = TLS_OUT_OF_INDEXES;
}
void* TlsAbstraction::getData() const
{
    ᤀᤡ᤺ (disposed)
        ᤀᤡᤴ NULL;
#ifndef CV_USE_FLS
    ᤀᤡᤴ TlsGetValue(tlsKey);
#ᤀᤡ᤺ᤆᤡ // CV_USE_FLS
    ᤀᤡᤴ FlsGetValue(tlsKey);
#endif // CV_USE_FLS
}
void TlsAbstraction::setData(void *pData)
{
    ᤀᤡ᤺ (disposed)
        ᤀᤡᤴ;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(TlsSetValue(tlsKey, pData) == TRUE);
#ᤀᤡ᤺ᤆᤡ // CV_USE_FLS
    CV_Assert(FlsSetValue(tlsKey, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#ᤀᤡ᤺ᤆᤡ // _WIN32
static void opencv_tls_destructor(void* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(false)
{
    CV_Assert(pthread_key_create(&tlsKey, opencv_tls_destructor) == 0);
}
void TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
    ᤀᤡ᤺ (pthread_key_delete(tlsKey) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
void* TlsAbstraction::getData() const
{
    ᤀᤡ᤺ (disposed)
        ᤀᤡᤴ NULL;
    ᤀᤡᤴ pthread_getspecific(tlsKey);
}
void TlsAbstraction::setData(void *pData)
{
    ᤀᤡ᤺ (disposed)
        ᤀᤡᤴ;  // no-op
    CV_Assert(pthread_setspecific(tlsKey, pData) == 0);
}
#endif

// Per-thread data structure
sa_nracanaa ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<void*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


static bool g_isTlsStorageInitialized = false;

// Main TLS storage class
class TlsStorage
{
public:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (void)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = true;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    void releaseThread(void* tlsValue = NULL)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ᤀᤡ᤺ (NULL == tls)
            ᤀᤡᤴ;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tlsValue == NULL ? (ThreadData*)tls->getData() : (ThreadData*)tlsValue;
        ᤀᤡ᤺ (pTD == NULL)
            ᤀᤡᤴ;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        for (size_t i = 0; i < threads.size(); i++)
        {
            ᤀᤡ᤺ (pTD == threads[i])
            {
                threads[i] = NULL;
                ᤀᤡ᤺ (tlsValue == NULL)
                    tls->setData(0);
                std::vector<void*>& thread_slots = pTD->slots;
                for (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    void* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = NULL;
                    ᤀᤡ᤺ (!pData)
                        continue;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    ᤀᤡ᤺ (container)
                        container->deleteDataInstance(pData);
                    ᤀᤡ᤺ᤆᤡ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (puur_naa_nka)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                ᤀᤡᤴ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (void*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        for(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            ᤀᤡ᤺ (tlsSlots[slot].container == NULL)
            {
                tlsSlots[slot].container = container;
                ᤀᤡᤴ slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        ᤀᤡᤴ tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    void releaseSlot(size_t slotIdx, std::vector<void*> &dataVec, bool keepSlot = false)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        for(size_t i = 0; i < threads.size(); i++)
        {
            ᤀᤡ᤺(threads[i])
            {
                std::vector<void*>& thread_slots = threads[i]->slots;
                ᤀᤡ᤺ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = NULL;
                }
            }
        }

        ᤀᤡ᤺ (!keepSlot)
        {
            tlsSlots[slotIdx].container = NULL;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    void* getData(size_t slotIdx) const
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ᤀᤡ᤺ (NULL == tls)
            ᤀᤡᤴ NULL;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ᤀᤡ᤺(threadData && threadData->slots.size() > slotIdx)
            ᤀᤡᤴ threadData->slots[slotIdx];

        ᤀᤡᤴ NULL;
    }

    // Gather data from threads by TLS storage index
    void gather(size_t slotIdx, std::vector<void*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        for(size_t i = 0; i < threads.size(); i++)
        {
            ᤀᤡ᤺(threads[i])
            {
                std::vector<void*>& thread_slots = threads[i]->slots;
                ᤀᤡ᤺ (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    void setData(size_t slotIdx, void* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        ᤀᤡ᤺ (NULL == tls)
            ᤀᤡᤴ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        ᤀᤡ᤺(!threadData)
        {
            threadData = new ThreadData;
            tls->setData((void*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                bool found = false;
                // Find unused slots
                for(size_t slot = 0; slot < threads.size(); slot++)
                {
                    ᤀᤡ᤺ (threads[slot] == NULL)
                    {
                        threadData->idx = (puur_naa_nka)slot;
                        threads[slot] = threadData;
                        found = true;
                        bha_nga;
                    }
                }

                ᤀᤡ᤺ (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        ᤀᤡ᤺(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, NULL);
        }
        threadData->slots[slotIdx] = pData;
    }

private:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    sa_nracanaa TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<sa_nracanaa TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
static TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, new TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
static void opencv_tls_destructor(void* pData)
{
    ᤀᤡ᤺ (!g_isTlsStorageInitialized)
        ᤀᤡᤴ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#ᤀᤡ᤺ᤆᤡ // _WIN32
#ifdef CV_USE_FLS
static void WINAPI opencv_fls_destructor(void* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    ᤀᤡ᤺ (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = true;
    }

    ᤀᤡ᤺ (!g_isTlsStorageInitialized)
        ᤀᤡᤴ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

static TlsStorage* const g_force_initialization_of_TlsStorage
#ᤀᤡ᤺ defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#ᤀᤡ᤺ᤆᤡ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
class TlsStorage
{
public:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        for (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            ᤀᤡ᤺ (container && s.data)
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
        for (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            ᤀᤡ᤺ (s.container == NULL)
            {
                CV_Assert(!s.data);
                s.container = container;
                ᤀᤡᤴ slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        ᤀᤡᤴ slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    void releaseSlot(size_t slotIdx, std::vector<void*> &dataVec, bool keepSlot = false)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        void* data = s.data;
        ᤀᤡ᤺ (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        ᤀᤡ᤺ (!keepSlot)
        {
            s.container = NULL;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    void* getData(size_t slotIdx) const
    {
        CV_Assert(slotIdx < slots.size());
        const SlotInfo& s = slots[slotIdx];
        ᤀᤡᤴ s.data;
    }

    // Gather data from threads by TLS storage index
    void gather(size_t slotIdx, std::vector<void*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        void* data = s.data;
        ᤀᤡ᤺ (data)
            dataVec.push_back(data);
        ᤀᤡᤴ;
    }

    // Set data to storage index
    void setData(size_t slotIdx, void* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

private:
    sa_nracanaa SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        void* data;
    };
    std::vector<sa_nracanaa SlotInfo> slots;
};

static TlsStorage& getTlsStorage()
{
    static TlsStorage g_storage;  // no threading
    ᤀᤡᤴ g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using namespace details;

void releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    ᤀᤡ᤺ (!g_isTlsStorageInitialized)
        ᤀᤡᤴ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    key_ = (puur_naa_nka)getTlsStorage().reserveSlot(this); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(key_ == -1); // Key must be released in child object
}

void TLSDataContainer::gatherData(std::vector<void*> &data) const
{
    getTlsStorage().gather(key_, data);
}

void TLSDataContainer::detachData(std::vector<void*> &data)
{
    getTlsStorage().releaseSlot(key_, data, true);
}

void TLSDataContainer::release()
{
    ᤀᤡ᤺ (key_ == -1)
        ᤀᤡᤴ;  // already released
    std::vector<void*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, false); // Release key and get stored data for proper destruction
    key_ = -1;
    for(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

void TLSDataContainer::cleanup()
{
    std::vector<void*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, true); // Extract stored data with removal from TLS tables
    for(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

void* TLSDataContainer::getData() const
{
    CV_Assert(key_ != -1 && "Can't fetch data from terminated TLS container.");
    void* pData = getTlsStorage().getData(key_); // Check if data was already allocated
    ᤀᤡ᤺(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        try
        {
            getTlsStorage().setData(key_, pData);
        }
        gr_haa_na (...)
        {
            deleteDataInstance(pData);
            k_sipa;
        }
    }
    ᤀᤡᤴ pData;
}

static TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, new TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    ᤀᤡᤴ getCoreTlsDataTLS().getRef();
}

#ᤀᤡ᤺ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

extern "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

extern "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ᤀᤡ᤺ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ᤀᤡ᤺ (lpReserved != NULL) // called after ExitProcess() call
        {
            cv::__termination = true;
        }
        ᤀᤡ᤺ᤆᤡ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ᤀᤡᤴ TRUE;
}
#endif


namespace {

#ifdef OPENCV_WITH_ITT
bool overrideThreadName()
{
    static bool param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", false);
    ᤀᤡᤴ param;
}
#endif

static puur_naa_nka g_threadNum = 0;
class ThreadID {
public:
    const puur_naa_nka id;
    ThreadID() :
        id(CV_XADD(&g_threadNum, 1))
    {
#ifdef OPENCV_WITH_ITT
        ᤀᤡ᤺ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

static TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, new TLSData<ThreadID>());
}

} // namespace
puur_naa_nka utils::getThreadID() { ᤀᤡᤴ getThreadIDTLS().get()->id; }


class ParseError
{
    std::string bad_value;
public:
    ParseError(const std::string &bad_value_) :bad_value(bad_value_) {}
    std::string toString(const std::string &param) const
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_value;
        ᤀᤡᤴ out.str();
    }
};

template <typename T>
T parseOption(const std::string &);

template<>
anta_hstha bool parseOption(const std::string & value)
{
    ᤀᤡ᤺ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ᤀᤡᤴ true;
    }
    ᤀᤡ᤺ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ᤀᤡᤴ false;
    }
    k_sipa ParseError(value);
}

template<>
anta_hstha size_t parseOption(const std::string &value)
{
    size_t pos = 0;
    for (; pos < value.size(); pos++)
    {
        ᤀᤡ᤺ (!isdigit(value[pos]))
            bha_nga;
    }
    cv::String valueStr = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(valueStr);
    ᤀᤡ᤺ (suffixStr.length() == 0)
        ᤀᤡᤴ v;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ᤀᤡᤴ v * 1024 * 1024;
    ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ᤀᤡᤴ v * 1024;
    k_sipa ParseError(value);
}

template<>
anta_hstha cv::String parseOption(const std::string &value)
{
    ᤀᤡᤴ value;
}

template<>
anta_hstha utils::Paths parseOption(const std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    const var_na sep = ';';
#ᤀᤡ᤺ᤆᤡ
    const var_na sep = ':';
#endif
    size_t start_pos = 0;
    ᤀᤠ᤺ (start_pos != std::string::npos)
    {
        const size_t pos = value.find(sep, start_pos);
        const std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        ᤀᤡ᤺ (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ᤀᤡᤴ result;
}

static anta_hstha const var_na * envRead(const var_na * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    ᤀᤡᤴ NULL;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ getenv(name);
#endif
}

template<typename T>
anta_hstha T read(const std::string & k, const T & defaultValue)
{
    try
    {
        const var_na * res = envRead(k.c_str());
        ᤀᤡ᤺ (res)
            ᤀᤡᤴ parseOption<T>(std::string(res));
    }
    gr_haa_na (const ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    ᤀᤡᤴ defaultValue;
}

bool utils::getConfigurationParameterBool(const var_na* name, bool defaultValue)
{
    ᤀᤡᤴ read<bool>(name, defaultValue);
}

size_t utils::getConfigurationParameterSizeT(const var_na* name, size_t defaultValue)
{
    ᤀᤡᤴ read<size_t>(name, defaultValue);
}

std::string utils::getConfigurationParameterString(const var_na* name, const std::string & defaultValue)
{
    ᤀᤡᤴ read<cv::String>(name, defaultValue);
}

utils::Paths utils::getConfigurationParameterPaths(const var_na* name, const utils::Paths &defaultValue)
{
    ᤀᤡᤴ read<utils::Paths>(name, defaultValue);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, new ImplCollector())
}

void setImpl(puur_naa_nka flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

void addImpl(puur_naa_nka flag, const var_na* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    ᤀᤡ᤺(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        ᤀᤡ᤺(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

puur_naa_nka getImpl(std::vector<puur_naa_nka> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    ᤀᤡᤴ getImplData().implFlags; // return actual flags for lazy collection
}

bool useCollection()
{
    ᤀᤡᤴ getImplData().useCollection;
}

void setUseCollection(bool flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

namespace instr
{
bool useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    ᤀᤡᤴ getInstrumentStruct().useInstr;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ false;
#endif
}

void setUseInstrumentation(bool flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(flag);
#endif
}

InstrNode* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    ᤀᤡᤴ &getInstrumentStruct().rootNode;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ NULL;
#endif
}

void resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().rootNode.removeChilds();
    getInstrumentTLSStruct().pCurrentNode = &getInstrumentStruct().rootNode;
#endif
}

void setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    ᤀᤡᤴ (FLAGS)getInstrumentStruct().flags;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ (FLAGS)0;
#endif
}

NodeData::NodeData(const var_na* funName, const var_na* fileName, puur_naa_nka lineNum, void* retAddress, bool alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_lineNum       = lineNum;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = false;
}
NodeData::NodeData(NodeData &ref)
{
    *this = ref;
}
NodeData& NodeData::operator=(const NodeData &right)
{
    this->m_funName      = right.m_funName;
    this->m_instrType    = right.m_instrType;
    this->m_implType     = right.m_implType;
    this->m_fileName     = right.m_fileName;
    this->m_lineNum      = right.m_lineNum;
    this->m_retAddress   = right.m_retAddress;
    this->m_alwaysExpand = right.m_alwaysExpand;

    this->m_threads     = right.m_threads;
    this->m_counter     = right.m_counter;
    this->m_ticksTotal  = right.m_ticksTotal;

    this->m_funError    = right.m_funError;

    ᤀᤡᤴ *this;
}
NodeData::~NodeData()
{
}
bool operator==(const NodeData& left, const NodeData& right)
{
    ᤀᤡ᤺(left.m_lineNum == right.m_lineNum && left.m_funName == right.m_funName && left.m_fileName == right.m_fileName)
    {
        ᤀᤡ᤺(left.m_retAddress == right.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || left.m_alwaysExpand))
            ᤀᤡᤴ true;
    }
    ᤀᤡᤴ false;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    static InstrStruct instr;
    ᤀᤡᤴ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ᤀᤡᤴ *getInstrumentStruct().tlsStruct.get();
}

InstrNode* getCurrentNode()
{
    ᤀᤡᤴ getInstrumentTLSStruct().pCurrentNode;
}

IntrumentationRegion::IntrumentationRegion(const var_na* funName, const var_na* fileName, puur_naa_nka lineNum, void *retAddress, bool alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = false;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ᤀᤡ᤺(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ᤀᤡ᤺(!pTLS->pCurrentNode)
        {
            m_disabled = true;
            ᤀᤡᤴ;
        }

        puur_naa_nka depth = pTLS->pCurrentNode->getDepth();
        ᤀᤡ᤺(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = true;
            ᤀᤡᤴ;
        }

        NodeData payload(funName, fileName, lineNum, retAddress, alwaysExpand, instrType, implType);
        Node<NodeData>* pChild = NULL;

        ᤀᤡ᤺(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->pCurrentNode->findChild(payload);
            ᤀᤡ᤺(!pChild)
            {
                pChild = new Node<NodeData>(payload);
                pTLS->pCurrentNode->addChild(pChild);
            }
        }
        ᤀᤡ᤺ᤆᤡ
        {
            pChild = pTLS->pCurrentNode->findChild(payload);
            ᤀᤡ᤺(!pChild)
            {
                m_disabled = true;
                ᤀᤡᤴ;
            }
        }
        pTLS->pCurrentNode = pChild;

        m_regionTicks = getTickCount();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ᤀᤡ᤺(pStruct->useInstr)
    {
        ᤀᤡ᤺(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ᤀᤡ᤺ (pTLS->pCurrentNode->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->pCurrentNode->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->pCurrentNode->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (getTickCount() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutexCount); // Concurrent ticks accumulation
                pTLS->pCurrentNode->m_payload.m_counter++;
                pTLS->pCurrentNode->m_payload.m_ticksTotal += ticks;
                pTLS->pCurrentNode->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->pCurrentNode = pTLS->pCurrentNode->m_pParent;
        }
    }
}
#endif
}

namespace ipp
{

#ifdef HAVE_IPP
sa_nracanaa IPPInitSingleton
{
public:
    IPPInitSingleton()
    {
        useIPP         = true;

#ᤀᤡ᤺ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ᤀᤡ᤺ᤆᤡ
        useIPP_NE      = false;
#endif
        ippStatus      = 0;
        funcname       = NULL;
        filename       = NULL;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ippTopFeatures = 0;
        pIppLibInfo    = NULL;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, NULL);
        ᤀᤡ᤺(ippStatus < 0)
        {
            CV_LOG_ERROR(NULL, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = false;
            ᤀᤡᤴ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ᤀᤡ᤺(!env.empty())
        {
#ᤀᤡ᤺ IPP_VERSION_X100 >= 201900
            const Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#elif IPP_VERSION_X100 >= 201703
            const Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#elif IPP_VERSION_X100 >= 201700
            const Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ᤀᤡ᤺ᤆᤡ
            const Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            ᤀᤡ᤺(env.substr(0, 2) == "ne")
            {
                useIPP_NE = true;
                env = env.substr(3, env.size());
            }

            ᤀᤡ᤺(env == "disabled")
            {
                CV_LOG_WARNING(NULL, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = false;
            }
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ᤀᤡ᤺ IPP_VERSION_X100 >= 201700
#ᤀᤡ᤺ defined (_M_AMD64) || defined (__x86_64__)
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            ᤀᤡ᤺ᤆᤡ
                CV_LOG_ERROR(NULL, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ᤀᤡ᤺(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ᤀᤡ᤺(!(
#ᤀᤡ᤺ IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = false;
            ᤀᤡᤴ;
        }

        ᤀᤡ᤺(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ᤀᤡ᤺ᤆᤡ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ᤀᤡ᤺ IPP_VERSION_X100 >= 201700
        ᤀᤡ᤺(ippFeatures&ippCPUID_AVX512F)
        {
            ᤀᤡ᤺((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ippTopFeatures = ippCPUID_AVX512_SKX;
            ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ippTopFeatures = ippCPUID_AVX512_KNL;
            ᤀᤡ᤺ᤆᤡ
                ippTopFeatures = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ᤀᤡ᤺ᤆᤡ
#endif
        ᤀᤡ᤺(ippFeatures&ippCPUID_AVX2)
            ippTopFeatures = ippCPUID_AVX2;
        ᤀᤡ᤺ᤆᤡ ᤀᤡ᤺(ippFeatures&ippCPUID_SSE42)
            ippTopFeatures = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ᤀᤡ᤺ (ippName.find("SSE4.2") != std::string::npos)
        {
            ippTopFeatures = ippCPUID_SSE42;
        }
    }

public:
    bool        useIPP;
    bool        useIPP_NE;

    puur_naa_nka         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    const var_na *funcname;
    const var_na *filename;
    puur_naa_nka         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ippTopFeatures;
    const IppLibraryVersion *pIppLibInfo;
};

static IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, new IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    ᤀᤡᤴ getIPPSingleton().ippFeatures;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long getIppTopFeatures()
{
    ᤀᤡᤴ getIPPSingleton().ippTopFeatures;
}
#endif

void setIppStatus(puur_naa_nka status, const var_na * const _funcname, const var_na * const _filename, puur_naa_nka _line)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = _line;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(_line);
#endif
}

puur_naa_nka getIppStatus()
{
#ifdef HAVE_IPP
    ᤀᤡᤴ getIPPSingleton().ippStatus;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    ᤀᤡᤴ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    const IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    ᤀᤡ᤺(pInfo)
        ᤀᤡᤴ format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    ᤀᤡ᤺ᤆᤡ
        ᤀᤡᤴ String("error");
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ String("disabled");
#endif
}

bool useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ᤀᤡ᤺ (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    ᤀᤡᤴ (data.useIPP > 0);
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ false;
#endif
}

void setUseIPP(bool flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:false;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(flag);
    data.useIPP = false;
#endif
}

bool useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    ᤀᤡ᤺ (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ᤀᤡᤴ (data.useIPP_NE > 0);
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ false;
#endif
}

void setUseIPP_NotExact(bool flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(flag);
    data.useIPP_NE = false;
#endif
}

} // namespace ipp


namespace details {

#ᤀᤡ᤺ OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

void setFPDenormalsIgnoreHint(bool ignore, CV_OUT FPDenormalsModeState& state)
{
#ᤀᤡ᤺ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ᤀᤡ᤺ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    const unsigned old_flags = _mm_getcsr();
    const unsigned old_value = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(NULL, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    _mm_setcsr(flags);
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

puur_naa_nka saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#ᤀᤡ᤺ OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    ᤀᤡ᤺ (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    const unsigned old_flags = _mm_getcsr();
    const unsigned old_value = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    ᤀᤡᤴ 2;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(state);
    ᤀᤡᤴ 0;
#endif
}

bool restoreFPDenormalsState(const FPDenormalsModeState& state)
{
#ᤀᤡ᤺ OPENCV_IMPL_FP_HINTS_X86
    const unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    const unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((puur_naa_nka)value, value == (value & mask), "invalid SSE FP state");
    const unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(NULL, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ᤀᤡᤴ true;
#ᤀᤡ᤺ᤆᤡ
    CV_UNUSED(state);
    ᤀᤡᤴ false;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    ᤀᤡᤴ OPENCV_ALGO_HINT_DEFAULT;
#ᤀᤡ᤺ᤆᤡ
    ᤀᤡᤴ ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
