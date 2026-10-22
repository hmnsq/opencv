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
    如果 (__initialization_mutex == NULL)
    {
        (void)_initSystem();
        __initialization_mutex = new Mutex();
    }
    返回 *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

static bool param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#如果 defined(_DEBUG) || defined(__ANDROID__)
    true
#否则
    false
#endif
);

void* allocSingletonBuffer(size_t size) { 返回 fastMalloc(size); }
void* allocSingletonNewBuffer(size_t size) { 返回 malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# 如果 defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#如果 defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# 如果 _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#include <cstdlib>        // std::abort
#endif

#如果 defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#如果 defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
const uint64_t AT_HWCAP = NT_GNU_HWCAP;
#否则
#  include <elf.h>
#endif
#如果 defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#如果 defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#如果 ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#如果 defined __loongarch64
#include "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#如果 defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#include <windows.h>
#include <combaseapi.h>
#如果 (_WIN32_WINNT >= 0x0602)
  #include <synchapi.h>
#endif
#如果 ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
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
    返回 std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#否则
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    如果 (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        返回 wstr;
    如果 (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        返回 wstr;
    如果 (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        返回 wstr;
    如果 (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        返回 wstr;
    如果 (FAILED(storagefolderRef.As(&storageitemRef)))
        返回 wstr;
    str = NULL;
    如果 (FAILED(storageitemRef->get_Path(&str)))
        返回 wstr;
    wstr = WindowsGetStringRawBuffer(str, NULL);
    WindowsDeleteString(str);
    返回 wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], sizeof(guidStr)/sizeof(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    返回 prefix.append(std::wstring(guidStr));
}

#endif
#否则
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#include <pthread.h>
#endif
#include <sys/time.h>
#include <time.h>

#如果 defined __MACH__ && defined __APPLE__
#include <mach/mach.h>
#include <mach/mach_time.h>
#include <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#include "omp.h"
#endif

#如果 defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#如果 defined __ANDROID__
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
  #如果 defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #如果 _MSC_VER >= 1400  // MSVS 2005
      #include <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #否则
      #error "Required MSVS 2005+"
    #endif
  #elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    static void cv_cpuid(整数* cpuid_data, 整数 reg_eax, 整数 reg_ecx)
    {
        整数 __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#如果 !defined(__PIC__) \
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
#否则
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

Exception::Exception(Error::Code _code, const String& _err, const String& _func, const String& _file, 整数 _line)
: code(_code), err(_err), func(_func), file(_file), line(_line)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
const char* Exception::what() const CV_NOEXCEPT { 返回 msg.c_str(); }

void Exception::formatMessage()
{
    size_t pos = err.find('\n');
    bool multiline = pos != cv::String::npos;
    如果 (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        当 (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        如果 (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    如果 (func.size() > 0)
    {
        如果 (multiline)
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), line, code, codeMessage(), func.c_str(), err.c_str());
        否则
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), func.c_str());
    }
    否则
    {
        msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

const char * Exception::codeMessage() const
{
    switch (code)
    {
    case cv::Error::StsOk :                  返回 "No Error";
    case cv::Error::StsBackTrace :           返回 "Backtrace";
    case cv::Error::StsError :               返回 "Unspecified error";
    case cv::Error::StsInternal :            返回 "Internal error";
    case cv::Error::StsNoMem :               返回 "Insufficient memory";
    case cv::Error::StsBadArg :              返回 "Bad argument";
    case cv::Error::StsNoConv :              返回 "Iterations do not converge";
    case cv::Error::StsAutoTrace :           返回 "Autotrace call";
    case cv::Error::StsBadSize :             返回 "Incorrect size of input array";
    case cv::Error::StsNullPtr :             返回 "Null pointer";
    case cv::Error::StsDivByZero :           返回 "Division by zero occurred";
    case cv::Error::BadStep :                返回 "Image step is wrong";
    case cv::Error::StsInplaceNotSupported : 返回 "Inplace operation is not supported";
    case cv::Error::StsObjectNotFound :      返回 "Requested object was not found";
    case cv::Error::BadDepth :               返回 "Input image depth is not supported by function";
    case cv::Error::StsUnmatchedFormats :    返回 "Formats of input arguments do not match";
    case cv::Error::StsUnmatchedSizes :      返回 "Sizes of input arguments do not match";
    case cv::Error::StsOutOfRange :          返回 "One of the arguments\' values is out of range";
    case cv::Error::StsUnsupportedFormat :   返回 "Unsupported format or combination of formats";
    case cv::Error::BadCOI :                 返回 "Input COI is not supported";
    case cv::Error::BadNumChannels :         返回 "Bad number of channels";
    case cv::Error::StsBadFlag :             返回 "Bad flag (parameter or structure field)";
    case cv::Error::StsBadPoint :            返回 "Bad parameter of type CvPoint";
    case cv::Error::StsBadMask :             返回 "Bad type of mask argument";
    case cv::Error::StsParseError :          返回 "Parsing error";
    case cv::Error::StsNotImplemented :      返回 "The function/feature is not implemented";
    case cv::Error::StsBadMemBlock :         返回 "Memory block has been corrupted";
    case cv::Error::StsAssert :              返回 "Assertion failed";
    case cv::Error::GpuNotSupported :        返回 "No CUDA support";
    case cv::Error::GpuApiCallError :        返回 "Gpu API call";
    case cv::Error::OpenGlNotSupported :     返回 "No OpenGL support";
    case cv::Error::OpenGlApiCallError :     返回 "OpenGL API call";
    case cv::Error::OpenCLApiCallError :     返回 "OpenCL API call error";
    case cv::Error::OpenCLDoubleNotSupported:返回 "OpenCL device does not support double";
    case cv::Error::OpenCLInitError :        返回 "OpenCL initialization error";
    case cv::Error::OpenCLNoAMDBlasFft :     返回 "Missing OpenCL AMD BLAS FFT";
    default:
    {
        static char buf[256] {0};
        snprintf(buf, sizeof(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        返回 buf;
    }
    };

}

static const char* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { NULL };

static const char* getHWFeatureName(整数 id)
{
    返回 (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : NULL;
}
static const char* getHWFeatureNameSafe(整数 id)
{
    const char* name = getHWFeatureName(id);
    返回 name ? name : "Unknown feature";
}

struct HWFeatures
{
    enum { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(bool run_initialize = false)
    {
        如果 (run_initialize)
            initialize();
    }

    static void initializeNames()
    {
        为 (整数 i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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
        如果 (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        整数 cpuid_data[4] = { 0, 0, 0, 0 };
        整数 cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        整数 x86_family = (cpuid_data[0] >> 8) & 15;
        如果( x86_family >= 6 )
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
            整数 cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            bool have_AVX_OS_support = true;
            bool have_AVX512_OS_support = true;
            如果 (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = false; // OS uses XSAVE_XRSTORE and CPU support AVX
            否则
            {
                整数 xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (整数)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #elif defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                如果 ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = false; // YMM registers
                如果 ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = false; // ZMM registers
            }

            如果 (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = false;
                have[CV_CPU_FP16] = false;
                have[CV_CPU_AVX2] = false;
                have[CV_CPU_FMA3] = false;
                have[CV_CPU_AVX_VNNI] = false;
            }
            如果 (!have_AVX_OS_support || !have_AVX512_OS_support)
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
            如果 (have[CV_CPU_AVX512_COMMON])
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
            否则
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

    #如果 defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = true;
        have[CV_CPU_FP16] = true;
        整数 cpufile = open("/proc/self/auxv", O_RDONLY);

        如果 (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            const size_t size_auxv_t = sizeof(auxv);

            当 ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                如果 (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#如果 defined(AT_HWCAP2)
                否则 如果 (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #elif defined __arm__ && defined __ANDROID__
      #如果 defined HAVE_CPUFEATURES
        CV_LOG_INFO(NULL, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(NULL, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #否则
        CV_LOG_INFO(NULL, "cpufeatures library is not available for CPU detection");
        #如果 CV_NEON
        CV_LOG_INFO(NULL, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = true;
        #否则
        CV_LOG_INFO(NULL, "- NEON instructions is NOT enabled via build flags");
        #endif
        #如果 CV_FP16
        CV_LOG_INFO(NULL, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = true;
        #否则
        CV_LOG_INFO(NULL, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #elif defined __arm__
        整数 cpufile = open("/proc/self/auxv", O_RDONLY);

        如果 (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            const size_t size_auxv_t = sizeof(auxv);

            当 ((size_t)read(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                如果 (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    break;
                }
            }

            close(cpufile);
        }
    #endif
    #elif (defined __APPLE__)
    #如果 defined __ARM_NEON
        have[CV_CPU_NEON] = true;
    #endif
    #如果 (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = true;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    整数 has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = sizeof(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, NULL, 0);
    如果 (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = true;
    }
    整数 has_feat_bf16 = 0;
    size_t has_feat_bf16_size = sizeof(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, NULL, 0);
    如果 (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = true;
    }
    #elif (defined __clang__)
    #如果 defined __ARM_NEON
        have[CV_CPU_NEON] = true;
        #如果 (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = true;
        #endif
    #endif
    #endif
    #如果 defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = true;
    #endif
    #如果 defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = true;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = true;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = true;
    #endif

    #如果 (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned 整数 hwcap = getauxval(AT_HWCAP);
        如果 (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            如果 (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } 否则 {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #elif (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        如果 (elf_aux_info(AT_HWCAP, &hwcap, sizeof(hwcap)) != 0)
            hwcap = 0;
        如果 (hwcap & PPC_FEATURE_HAS_VSX) {
            如果 (elf_aux_info(AT_HWCAP2, &hwcap2, sizeof(hwcap2)) != 0)
                hwcap2 = 0;
            如果 (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } 否则 {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #否则
        // TODO: AIX
        #如果 CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = true;
        #endif
        #如果 CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = true;
        #endif
    #endif

    #如果 defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = true;
        #如果 (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = true;
        #endif
        #如果 defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = true;
        #endif
    #endif

    #如果 defined __loongarch64 && defined __linux__
        整数 flag = (整数)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        bool skip_baseline_check = false;
        如果 (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = true;
        }
        整数 baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        如果 (!checkFeatures(baseline_features, sizeof(baseline_features) / sizeof(baseline_features[0]))
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
            checkFeatures(baseline_features, sizeof(baseline_features) / sizeof(baseline_features[0]), true);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, sizeof(baseline_features) / sizeof(baseline_features[0]));
    }

    bool checkFeatures(const 整数* features, 整数 count, bool dump = false)
    {
        bool result = true;
        为 (整数 i = 0; i < count; i++)
        {
            整数 feature = features[i];
            如果 (feature)
            {
                如果 (have[feature])
                {
                    如果 (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                否则
                {
                    result = false;
                    如果 (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        返回 result;
    }

    static inline bool isSymbolSeparator(char c)
    {
        返回 c == ',' || c == ';';
    }

    void readSettings(const 整数* baseline_features, 整数 baseline_count)
    {
        bool dump = true;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        如果 (!disabled_features.empty())
        {
            const char* start = disabled_features.c_str();
            为 (;;)
            {
                当 (start[0] != 0 && isSymbolSeparator(start[0]))
                {
                    start++;
                }
                如果 (start[0] == 0)
                    break;
                const char* end = start;
                当 (end[0] != 0 && !isSymbolSeparator(end[0]))
                {
                    end++;
                }
                如果 (end == start)
                    continue;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                bool found = false;
                为 (整数 i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    如果 (!g_hwFeatureNames[i]) continue;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    如果 (len != feature.size()) continue;
                    如果 (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        bool isBaseline = false;
                        为 (整数 k = 0; k < baseline_count; k++)
                        {
                            如果 (baseline_features[k] == i)
                            {
                                isBaseline = true;
                                break;
                            }
                        }
                        如果 (isBaseline)
                        {
                            如果 (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        如果 (!have[i])
                        {
                            如果 (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = false;

                        found = true;
                        break;
                    }
                }
                如果 (!found)
                {
                    如果 (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    bool have[MAX_FEATURE+1]{};
};

static HWFeatures  featuresEnabled(true), featuresDisabled = HWFeatures(false);
static HWFeatures* currentFeatures = &featuresEnabled;

bool checkHardwareSupport(整数 feature)
{
    如果 (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        返回 false;
    返回 currentFeatures->have[feature];
}

String getHardwareFeatureName(整数 feature)
{
    const char* name = getHWFeatureName(feature);
    返回 name ? String(name) : String();
}

std::string getCPUFeaturesLine()
{
    const 整数 features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    const 整数 sz = sizeof(features) / sizeof(features[0]);
    std::string result;
    std::string prefix;
    为 (整数 i = 1; i < sz; ++i)
    {
        如果 (features[i] == 0)
        {
            prefix = "*";
            continue;
        }
        如果 (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        如果 (!checkHardwareSupport(features[i])) result.append("?");
    }
    返回 result;
}

volatile bool useOptimizedFlag = true;

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
    返回 useOptimizedFlag;
}

int64 getTickCount(void)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    返回 (int64)now.time_since_epoch().count();
}

double getTickFrequency(void)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    double clock_freq = clock_period_t::den / clock_period_t::num;
    返回 clock_freq;
}

#如果 defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#如果 defined(__i386__)

int64 getCPUTickCount(void)
{
    int64 x;
    __asm__ volatile (".byte 0x0f, 0x31" : "=A" (x));
    返回 x;
}
#elif defined(__x86_64__)

int64 getCPUTickCount(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    返回 (int64)lo | ((int64)hi << 32);
}

#elif defined(__ppc__)

int64 getCPUTickCount(void)
{
    unsigned upper, lower, tmp;
    __asm__ volatile(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    返回 lower | ((int64)upper << 32);
}

#否则

#error "RDTSC not defined"

#endif

#elif defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 getCPUTickCount(void)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#否则

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 getCPUTickCount(void)
{
    返回 getTickCount();
}
//#endif

#endif


namespace internal {

class Timestamp
{
public:
    const int64 zeroTickCount;
    const double ns_in_ticks;

    Timestamp()
        : zeroTickCount(getTickCount())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = getTickCount();
        返回 (int64)((t - zeroTickCount) * ns_in_ticks);
    }

    static Timestamp& getInstance()
    {
        static Timestamp g_timestamp;
        返回 g_timestamp;
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
    返回 internal::Timestamp::getInstance().getTimestamp();
}


const String& getBuildInformation()
{
    static String build_info =
#include "version_string.inc"
    ;
    返回 build_info;
}

String getVersionString() { 返回 String(CV_VERSION); }

整数 getVersionMajor() { 返回 CV_VERSION_MAJOR; }

整数 getVersionMinor() { 返回 CV_VERSION_MINOR; }

整数 getVersionRevision() { 返回 CV_VERSION_REVISION; }

String format( const char* fmt, ... )
{
    AutoBuffer<char, 1024> buf;

    为 ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        整数 bsize = static_cast<整数>(buf.size());
        整数 len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        如果 (len >= bsize)
        {
            buf.resize(len + 1);
            continue;
        }
        buf[bsize - 1] = 0;
        返回 String(buf.data(), len);
    }
}

String tempfile( const char* suffix )
{
#如果 OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#如果 defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    如果 (temp_file.empty())
        返回 String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    char aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#elif defined(_WIN32_WCE)
    const auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    如果(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        char aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#否则
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    如果 (temp_dir.empty())
    {
        ::GetTempPathW(sizeof(temp_dir2_w)/sizeof(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        整数 len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, NULL, 0, NULL, NULL);
        如果 (len > 0)
        {
            std::vector<char> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, NULL, NULL);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    如果 (FAILED(hr))
        返回 String();
    char guidStr[40];
    const char* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, sizeof(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned 整数)g.Data4[0], (unsigned 整数)g.Data4[1],
            (unsigned 整数)g.Data4[2], (unsigned 整数)g.Data4[3], (unsigned 整数)g.Data4[4],
            (unsigned 整数)g.Data4[5], (unsigned 整数)g.Data4[6], (unsigned 整数)g.Data4[7]);

    fname = temp_dir;
    如果 (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# 否则
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    char defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  否则
    char defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    如果 (temp_dir.empty())
        fname = defaultTemplate;
    否则
    {
        fname = temp_dir;
        char ech = fname[fname.size() - 1];
        如果(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    const 整数 fd = mkstemp((char*)fname.c_str());
    如果 (fd == -1) 返回 String();

    close(fd);
    remove(fname.c_str());
# endif

    如果 (suffix)
    {
        如果 (suffix[0] != '.')
            返回 fname + "." + suffix;
        否则
            返回 fname + suffix;
    }
    返回 fname;
#否则 // OPENCV_HAVE_FILESYSTEM_SUPPORT
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
    返回 prevVal;
}

整数 cv_snprintf(char* buf, 整数 len, const char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    整数 res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    返回 res;
}

整数 cv_vsnprintf(char* buf, 整数 len, const char* fmt, va_list args)
{
#如果 defined _MSC_VER
    如果 (len <= 0) 返回 len == 0 ? 1024 : -1;
    整数 res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    如果 (res >= 0 && res < len)
    {
        buf[res] = 0;
        返回 res;
    }
    否则
    {
        buf[len - 1] = 0; // truncate happened
        返回 res >= len ? res : (len * 2);
    }
#否则
    返回 vsnprintf(buf, len, fmt, args);
#endif
}

static void dumpException(const Exception& exc)
{
    const char* errorStr = exc.codeMessage();
    char buf[1 << 12];

    cv_snprintf(buf, sizeof(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.line);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#否则
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
    如果 (false /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# 如果 defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

void error( const Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        如果 (!cv_terminate_handler_installed)
        {
            如果 (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = true;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    如果 (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.line, customErrorCallbackData);
    否则 如果 (param_dumpErrors)
    {
        dumpException(exc);
    }

    如果(breakOnError)
    {
        std::terminate();
    }

    throw exc;
#ifdef __GNUC__
# 如果 !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

void error(Error::Code _code, const String& _err, const char* _func, const char* _file, 整数 _line)
{
    error(cv::Exception(_code, _err, _func, _file, _line));
#ifdef __GNUC__
# 如果 !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# 如果 defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, void* userdata, void** prevUserdata)
{
    如果( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    返回 prevCallback;
}

void terminate(Error::Code _code, const String& _err, const char* _func, const char* _file, 整数 _line) CV_NOEXCEPT
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
#否则 // _WIN32
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
    返回 g_tls;
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
    返回 tlsData;
}
void TlsAbstraction::setData(void *pData)
{
    tlsData = pData;
}
#否则 //WINRT
#ifdef CV_USE_FLS
static void NTAPI opencv_fls_destructor(void* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(false)
{
#ifndef CV_USE_FLS
    tlsKey = TlsAlloc();
#否则 // CV_USE_FLS
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
#否则 // CV_USE_FLS
    FlsFree(tlsKey);
#endif // CV_USE_FLS
    tlsKey = TLS_OUT_OF_INDEXES;
}
void* TlsAbstraction::getData() const
{
    如果 (disposed)
        返回 NULL;
#ifndef CV_USE_FLS
    返回 TlsGetValue(tlsKey);
#否则 // CV_USE_FLS
    返回 FlsGetValue(tlsKey);
#endif // CV_USE_FLS
}
void TlsAbstraction::setData(void *pData)
{
    如果 (disposed)
        返回;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(TlsSetValue(tlsKey, pData) == TRUE);
#否则 // CV_USE_FLS
    CV_Assert(FlsSetValue(tlsKey, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#否则 // _WIN32
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
    如果 (pthread_key_delete(tlsKey) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
void* TlsAbstraction::getData() const
{
    如果 (disposed)
        返回 NULL;
    返回 pthread_getspecific(tlsKey);
}
void TlsAbstraction::setData(void *pData)
{
    如果 (disposed)
        返回;  // no-op
    CV_Assert(pthread_setspecific(tlsKey, pData) == 0);
}
#endif

// Per-thread data structure
struct ThreadData
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
        如果 (NULL == tls)
            返回;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tlsValue == NULL ? (ThreadData*)tls->getData() : (ThreadData*)tlsValue;
        如果 (pTD == NULL)
            返回;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        为 (size_t i = 0; i < threads.size(); i++)
        {
            如果 (pTD == threads[i])
            {
                threads[i] = NULL;
                如果 (tlsValue == NULL)
                    tls->setData(0);
                std::vector<void*>& thread_slots = pTD->slots;
                为 (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    void* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = NULL;
                    如果 (!pData)
                        continue;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    如果 (container)
                        container->deleteDataInstance(pData);
                    否则
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (整数)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                返回;
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
        为(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            如果 (tlsSlots[slot].container == NULL)
            {
                tlsSlots[slot].container = container;
                返回 slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        返回 tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    void releaseSlot(size_t slotIdx, std::vector<void*> &dataVec, bool keepSlot = false)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        为(size_t i = 0; i < threads.size(); i++)
        {
            如果(threads[i])
            {
                std::vector<void*>& thread_slots = threads[i]->slots;
                如果 (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = NULL;
                }
            }
        }

        如果 (!keepSlot)
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
        如果 (NULL == tls)
            返回 NULL;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        如果(threadData && threadData->slots.size() > slotIdx)
            返回 threadData->slots[slotIdx];

        返回 NULL;
    }

    // Gather data from threads by TLS storage index
    void gather(size_t slotIdx, std::vector<void*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        为(size_t i = 0; i < threads.size(); i++)
        {
            如果(threads[i])
            {
                std::vector<void*>& thread_slots = threads[i]->slots;
                如果 (thread_slots.size() > slotIdx && thread_slots[slotIdx])
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
        如果 (NULL == tls)
            返回;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        如果(!threadData)
        {
            threadData = new ThreadData;
            tls->setData((void*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                bool found = false;
                // Find unused slots
                为(size_t slot = 0; slot < threads.size(); slot++)
                {
                    如果 (threads[slot] == NULL)
                    {
                        threadData->idx = (整数)slot;
                        threads[slot] = threadData;
                        found = true;
                        break;
                    }
                }

                如果 (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        如果(slotIdx >= threadData->slots.size())
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

    struct TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<struct TlsSlotInfo> tlsSlots;  // TLS keys state
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
    如果 (!g_isTlsStorageInitialized)
        返回;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#否则 // _WIN32
#ifdef CV_USE_FLS
static void WINAPI opencv_fls_destructor(void* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    如果 (res && code != STILL_ACTIVE)
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

    如果 (!g_isTlsStorageInitialized)
        返回;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

static TlsStorage* const g_force_initialization_of_TlsStorage
#如果 defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#否则  // OPENCV_DISABLE_THREAD_SUPPORT

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
        为 (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            如果 (container && s.data)
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
        为 (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            如果 (s.container == NULL)
            {
                CV_Assert(!s.data);
                s.container = container;
                返回 slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        返回 slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    void releaseSlot(size_t slotIdx, std::vector<void*> &dataVec, bool keepSlot = false)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        void* data = s.data;
        如果 (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        如果 (!keepSlot)
        {
            s.container = NULL;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    void* getData(size_t slotIdx) const
    {
        CV_Assert(slotIdx < slots.size());
        const SlotInfo& s = slots[slotIdx];
        返回 s.data;
    }

    // Gather data from threads by TLS storage index
    void gather(size_t slotIdx, std::vector<void*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        void* data = s.data;
        如果 (data)
            dataVec.push_back(data);
        返回;
    }

    // Set data to storage index
    void setData(size_t slotIdx, void* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

private:
    struct SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        void* data;
    };
    std::vector<struct SlotInfo> slots;
};

static TlsStorage& getTlsStorage()
{
    static TlsStorage g_storage;  // no threading
    返回 g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using namespace details;

void releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    如果 (!g_isTlsStorageInitialized)
        返回;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    key_ = (整数)getTlsStorage().reserveSlot(this); // Reserve key from TLS storage
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
    如果 (key_ == -1)
        返回;  // already released
    std::vector<void*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, false); // Release key and get stored data for proper destruction
    key_ = -1;
    为(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

void TLSDataContainer::cleanup()
{
    std::vector<void*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, true); // Extract stored data with removal from TLS tables
    为(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

void* TLSDataContainer::getData() const
{
    CV_Assert(key_ != -1 && "Can't fetch data from terminated TLS container.");
    void* pData = getTlsStorage().getData(key_); // Check if data was already allocated
    如果(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        try
        {
            getTlsStorage().setData(key_, pData);
        }
        catch (...)
        {
            deleteDataInstance(pData);
            throw;
        }
    }
    返回 pData;
}

static TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, new TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    返回 getCoreTlsDataTLS().getRef();
}

#如果 defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

extern "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

extern "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    如果 (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        如果 (lpReserved != NULL) // called after ExitProcess() call
        {
            cv::__termination = true;
        }
        否则
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    返回 TRUE;
}
#endif


namespace {

#ifdef OPENCV_WITH_ITT
bool overrideThreadName()
{
    static bool param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", false);
    返回 param;
}
#endif

static 整数 g_threadNum = 0;
class ThreadID {
public:
    const 整数 id;
    ThreadID() :
        id(CV_XADD(&g_threadNum, 1))
    {
#ifdef OPENCV_WITH_ITT
        如果 (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

static TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, new TLSData<ThreadID>());
}

} // namespace
整数 utils::getThreadID() { 返回 getThreadIDTLS().get()->id; }


class ParseError
{
    std::string bad_value;
public:
    ParseError(const std::string &bad_value_) :bad_value(bad_value_) {}
    std::string toString(const std::string &param) const
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_value;
        返回 out.str();
    }
};

template <typename T>
T parseOption(const std::string &);

template<>
inline bool parseOption(const std::string & value)
{
    如果 (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        返回 true;
    }
    如果 (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        返回 false;
    }
    throw ParseError(value);
}

template<>
inline size_t parseOption(const std::string &value)
{
    size_t pos = 0;
    为 (; pos < value.size(); pos++)
    {
        如果 (!isdigit(value[pos]))
            break;
    }
    cv::String valueStr = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(valueStr);
    如果 (suffixStr.length() == 0)
        返回 v;
    否则 如果 (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        返回 v * 1024 * 1024;
    否则 如果 (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        返回 v * 1024;
    throw ParseError(value);
}

template<>
inline cv::String parseOption(const std::string &value)
{
    返回 value;
}

template<>
inline utils::Paths parseOption(const std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    const char sep = ';';
#否则
    const char sep = ':';
#endif
    size_t start_pos = 0;
    当 (start_pos != std::string::npos)
    {
        const size_t pos = value.find(sep, start_pos);
        const std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        如果 (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    返回 result;
}

static inline const char * envRead(const char * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    返回 NULL;
#否则
    返回 getenv(name);
#endif
}

template<typename T>
inline T read(const std::string & k, const T & defaultValue)
{
    try
    {
        const char * res = envRead(k.c_str());
        如果 (res)
            返回 parseOption<T>(std::string(res));
    }
    catch (const ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    返回 defaultValue;
}

bool utils::getConfigurationParameterBool(const char* name, bool defaultValue)
{
    返回 read<bool>(name, defaultValue);
}

size_t utils::getConfigurationParameterSizeT(const char* name, size_t defaultValue)
{
    返回 read<size_t>(name, defaultValue);
}

std::string utils::getConfigurationParameterString(const char* name, const std::string & defaultValue)
{
    返回 read<cv::String>(name, defaultValue);
}

utils::Paths utils::getConfigurationParameterPaths(const char* name, const utils::Paths &defaultValue)
{
    返回 read<utils::Paths>(name, defaultValue);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, new ImplCollector())
}

void setImpl(整数 flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

void addImpl(整数 flag, const char* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    如果(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        如果(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

整数 getImpl(std::vector<整数> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    返回 getImplData().implFlags; // return actual flags for lazy collection
}

bool useCollection()
{
    返回 getImplData().useCollection;
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
    返回 getInstrumentStruct().useInstr;
#否则
    返回 false;
#endif
}

void setUseInstrumentation(bool flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#否则
    CV_UNUSED(flag);
#endif
}

InstrNode* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    返回 &getInstrumentStruct().rootNode;
#否则
    返回 NULL;
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
#否则
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    返回 (FLAGS)getInstrumentStruct().flags;
#否则
    返回 (FLAGS)0;
#endif
}

NodeData::NodeData(const char* funName, const char* fileName, 整数 lineNum, void* retAddress, bool alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
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

    返回 *this;
}
NodeData::~NodeData()
{
}
bool operator==(const NodeData& left, const NodeData& right)
{
    如果(left.m_lineNum == right.m_lineNum && left.m_funName == right.m_funName && left.m_fileName == right.m_fileName)
    {
        如果(left.m_retAddress == right.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || left.m_alwaysExpand))
            返回 true;
    }
    返回 false;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    static InstrStruct instr;
    返回 instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    返回 *getInstrumentStruct().tlsStruct.get();
}

InstrNode* getCurrentNode()
{
    返回 getInstrumentTLSStruct().pCurrentNode;
}

IntrumentationRegion::IntrumentationRegion(const char* funName, const char* fileName, 整数 lineNum, void *retAddress, bool alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = false;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    如果(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        如果(!pTLS->pCurrentNode)
        {
            m_disabled = true;
            返回;
        }

        整数 depth = pTLS->pCurrentNode->getDepth();
        如果(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = true;
            返回;
        }

        NodeData payload(funName, fileName, lineNum, retAddress, alwaysExpand, instrType, implType);
        Node<NodeData>* pChild = NULL;

        如果(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->pCurrentNode->findChild(payload);
            如果(!pChild)
            {
                pChild = new Node<NodeData>(payload);
                pTLS->pCurrentNode->addChild(pChild);
            }
        }
        否则
        {
            pChild = pTLS->pCurrentNode->findChild(payload);
            如果(!pChild)
            {
                m_disabled = true;
                返回;
            }
        }
        pTLS->pCurrentNode = pChild;

        m_regionTicks = getTickCount();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    如果(pStruct->useInstr)
    {
        如果(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            如果 (pTLS->pCurrentNode->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
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
struct IPPInitSingleton
{
public:
    IPPInitSingleton()
    {
        useIPP         = true;

#如果 defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#否则
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
        如果(ippStatus < 0)
        {
            CV_LOG_ERROR(NULL, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = false;
            返回;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        如果(!env.empty())
        {
#如果 IPP_VERSION_X100 >= 201900
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
#否则
            const Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            如果(env.substr(0, 2) == "ne")
            {
                useIPP_NE = true;
                env = env.substr(3, env.size());
            }

            如果(env == "disabled")
            {
                CV_LOG_WARNING(NULL, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = false;
            }
            否则 如果(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            否则 如果(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#如果 IPP_VERSION_X100 >= 201700
#如果 defined (_M_AMD64) || defined (__x86_64__)
            否则 如果(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            否则
                CV_LOG_ERROR(NULL, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        如果(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        如果(!(
#如果 IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = false;
            返回;
        }

        如果(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        否则
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#如果 IPP_VERSION_X100 >= 201700
        如果(ippFeatures&ippCPUID_AVX512F)
        {
            如果((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ippTopFeatures = ippCPUID_AVX512_SKX;
            否则 如果((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ippTopFeatures = ippCPUID_AVX512_KNL;
            否则
                ippTopFeatures = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        否则
#endif
        如果(ippFeatures&ippCPUID_AVX2)
            ippTopFeatures = ippCPUID_AVX2;
        否则 如果(ippFeatures&ippCPUID_SSE42)
            ippTopFeatures = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        如果 (ippName.find("SSE4.2") != std::string::npos)
        {
            ippTopFeatures = ippCPUID_SSE42;
        }
    }

public:
    bool        useIPP;
    bool        useIPP_NE;

    整数         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    const char *funcname;
    const char *filename;
    整数         linen;
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
    返回 getIPPSingleton().ippFeatures;
#否则
    返回 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long getIppTopFeatures()
{
    返回 getIPPSingleton().ippTopFeatures;
}
#endif

void setIppStatus(整数 status, const char * const _funcname, const char * const _filename, 整数 _line)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = _line;
#否则
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(_line);
#endif
}

整数 getIppStatus()
{
#ifdef HAVE_IPP
    返回 getIPPSingleton().ippStatus;
#否则
    返回 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    返回 format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#否则
    返回 String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    const IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    如果(pInfo)
        返回 format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    否则
        返回 String("error");
#否则
    返回 String("disabled");
#endif
}

bool useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    如果 (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    返回 (data.useIPP > 0);
#否则
    返回 false;
#endif
}

void setUseIPP(bool flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:false;
#否则
    CV_UNUSED(flag);
    data.useIPP = false;
#endif
}

bool useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    如果 (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    返回 (data.useIPP_NE > 0);
#否则
    返回 false;
#endif
}

void setUseIPP_NotExact(bool flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#否则
    CV_UNUSED(flag);
    data.useIPP_NE = false;
#endif
}

} // namespace ipp


namespace details {

#如果 OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

void setFPDenormalsIgnoreHint(bool ignore, CV_OUT FPDenormalsModeState& state)
{
#如果 OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    如果 (featuresEnabled.have[CPU_SSE3])
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
#否则
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

整数 saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#如果 OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    如果 (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    const unsigned old_flags = _mm_getcsr();
    const unsigned old_value = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    返回 2;
#否则
    CV_UNUSED(state);
    返回 0;
#endif
}

bool restoreFPDenormalsState(const FPDenormalsModeState& state)
{
#如果 OPENCV_IMPL_FP_HINTS_X86
    const unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    const unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((整数)value, value == (value & mask), "invalid SSE FP state");
    const unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(NULL, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    返回 true;
#否则
    CV_UNUSED(state);
    返回 false;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    返回 OPENCV_ALGO_HINT_DEFAULT;
#否则
    返回 ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
