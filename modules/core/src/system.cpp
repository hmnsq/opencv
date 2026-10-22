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

#شامل "precomp.hpp"
//#include "opencv2/core/core_c.h"
#شامل <atomic>
#شامل <exception>
#شامل <iostream>
#شامل <ostream>

#ifdef __QNX__
    #شامل <unistd.h>
    #شامل <sys/neutrino.h>
    #شامل <sys/syspage.h>
#ifdef __aarch64__
    #شامل <aarch64/syspage.h>
#endif
#endif

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/trace.private.hpp>

#شامل <opencv2/core/utils/logger.hpp>

#شامل <opencv2/core/utils/tls.hpp>
#شامل <opencv2/core/utils/instrumentation.hpp>

#شامل <opencv2/core/utils/filesystem.private.hpp>

#شامل <opencv2/core/utils/fp_control_utils.hpp>
#شامل <opencv2/core/utils/fp_control.private.hpp>

فضاینام cv {

ایستا تهی _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ایستا std::ios_base::Init s_iostream_initializer;
#endif
}

ایستا Mutex* __initialization_mutex = پوچ;
Mutex& getInitializationMutex()
{
    اگر (__initialization_mutex == پوچ)
    {
        (تهی)_initSystem();
        __initialization_mutex = جدید Mutex();
    }
    بازگشت *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ایستا منطقی param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#اگر defined(_DEBUG) || defined(__ANDROID__)
    درست
#وگرنه
    نادرست
#endif
);

تهی* allocSingletonBuffer(size_t size) { بازگشت fastMalloc(size); }
تهی* allocSingletonNewBuffer(size_t size) { بازگشت malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# اگر defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#اگر defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# اگر _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#شامل <cstdlib>        // std::abort
#endif

#اگر defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#اگر defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#وگرنه
#  include <elf.h>
#endif
#اگر defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#اگر defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#اگر ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#اگر defined __loongarch64
#شامل "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#اگر defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#شامل <windows.h>
#شامل <combaseapi.h>
#اگر (_WIN32_WINNT >= 0x0602)
  #شامل <synchapi.h>
#endif
#اگر ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #شامل <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#شامل <tchar.h>

#ifdef WINRT
#شامل <wrl/client.h>
#ifndef __cplusplus_winrt
#شامل <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    بازگشت std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#وگرنه
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    اگر (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        بازگشت wstr;
    اگر (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        بازگشت wstr;
    اگر (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        بازگشت wstr;
    اگر (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        بازگشت wstr;
    اگر (FAILED(storagefolderRef.As(&storageitemRef)))
        بازگشت wstr;
    str = پوچ;
    اگر (FAILED(storageitemRef->get_Path(&str)))
        بازگشت wstr;
    wstr = WindowsGetStringRawBuffer(str, پوچ);
    WindowsDeleteString(str);
    بازگشت wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], اندازه(guidStr)/اندازه(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    بازگشت prefix.append(std::wstring(guidStr));
}

#endif
#وگرنه
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#شامل <pthread.h>
#endif
#شامل <sys/time.h>
#شامل <time.h>

#اگر defined __MACH__ && defined __APPLE__
#شامل <mach/mach.h>
#شامل <mach/mach_time.h>
#شامل <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#شامل "omp.h"
#endif

#اگر defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#اگر defined __ANDROID__
#شامل <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #اگر defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #اگر _MSC_VER >= 1400  // MSVS 2005
      #شامل <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #وگرنه
      #error "Required MSVS 2005+"
    #endif
  #وگرنهاگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ایستا تهی cv_cpuid(صحیح* cpuid_data, صحیح reg_eax, صحیح reg_ecx)
    {
        صحیح __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#اگر !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#وگرنهاگر defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#وگرنه
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#شامل <chrono>

فضاینام cv
{

Exception::Exception() { code = Error::StsOk; سطر = 0; }

Exception::Exception(Error::Code _code, ثابت String& _err, ثابت String& _func, ثابت String& _file, صحیح سطر_3)
: code(_code), err(_err), func(_func), file(_file), سطر(سطر_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ثابت نویسه* Exception::what() ثابت CV_NOEXCEPT { بازگشت پیام.c_str(); }

تهی Exception::formatMessage()
{
    size_t pos = err.find('\n');
    منطقی multiline = pos != cv::String::npos;
    اگر (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        وقتی (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        اگر (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    اگر (func.size() > 0)
    {
        اگر (multiline)
            پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), سطر, code, codeMessage(), func.c_str(), err.c_str());
        وگرنه
            پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), سطر, code, codeMessage(), err.c_str(), func.c_str());
    }
    وگرنه
    {
        پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), سطر, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت نویسه * Exception::codeMessage() ثابت
{
    انتخاب (code)
    {
    حالت cv::Error::StsOk :                  بازگشت "No Error";
    حالت cv::Error::StsBackTrace :           بازگشت "Backtrace";
    حالت cv::Error::StsError :               بازگشت "Unspecified error";
    حالت cv::Error::StsInternal :            بازگشت "Internal error";
    حالت cv::Error::StsNoMem :               بازگشت "Insufficient memory";
    حالت cv::Error::StsBadArg :              بازگشت "Bad argument";
    حالت cv::Error::StsNoConv :              بازگشت "Iterations do not converge";
    حالت cv::Error::StsAutoTrace :           بازگشت "Autotrace call";
    حالت cv::Error::StsBadSize :             بازگشت "Incorrect size of input array";
    حالت cv::Error::StsNullPtr :             بازگشت "Null pointer";
    حالت cv::Error::StsDivByZero :           بازگشت "Division by zero occurred";
    حالت cv::Error::BadStep :                بازگشت "Image step is wrong";
    حالت cv::Error::StsInplaceNotSupported : بازگشت "Inplace operation is not supported";
    حالت cv::Error::StsObjectNotFound :      بازگشت "Requested object was not found";
    حالت cv::Error::BadDepth :               بازگشت "Input image depth is not supported by function";
    حالت cv::Error::StsUnmatchedFormats :    بازگشت "Formats of input arguments do not match";
    حالت cv::Error::StsUnmatchedSizes :      بازگشت "Sizes of input arguments do not match";
    حالت cv::Error::StsOutOfRange :          بازگشت "One of the arguments\' values is out of range";
    حالت cv::Error::StsUnsupportedFormat :   بازگشت "Unsupported format or combination of formats";
    حالت cv::Error::BadCOI :                 بازگشت "Input COI is not supported";
    حالت cv::Error::Bad_رقم_Channels :         بازگشت "Bad number of channels";
    حالت cv::Error::StsBadFlag :             بازگشت "Bad flag (parameter or structure field)";
    حالت cv::Error::StsBadPoint :            بازگشت "Bad parameter of type CvPoint";
    حالت cv::Error::StsBadMask :             بازگشت "Bad type of mask argument";
    حالت cv::Error::StsParseError :          بازگشت "Parsing error";
    حالت cv::Error::StsNotImplemented :      بازگشت "The function/feature is not implemented";
    حالت cv::Error::StsBadMemBlock :         بازگشت "Memory block has been corrupted";
    حالت cv::Error::StsAssert :              بازگشت "Assertion failed";
    حالت cv::Error::GpuNotSupported :        بازگشت "No CUDA support";
    حالت cv::Error::GpuApiCallError :        بازگشت "Gpu API call";
    حالت cv::Error::OpenGlNotSupported :     بازگشت "No OpenGL support";
    حالت cv::Error::OpenGlApiCallError :     بازگشت "OpenGL API call";
    حالت cv::Error::OpenCLApiCallError :     بازگشت "OpenCL API call error";
    حالت cv::Error::OpenCLDoubleNotSupported:بازگشت "OpenCL device does not support double";
    حالت cv::Error::OpenCLInitError :        بازگشت "OpenCL initialization error";
    حالت cv::Error::OpenCLNoAMDBlasFft :     بازگشت "Missing OpenCL AMD BLAS FFT";
    پیشفرض:
    {
        ایستا نویسه buf[256] {0};
        snprintf(buf, اندازه(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        بازگشت buf;
    }
    };

}

ایستا ثابت نویسه* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { پوچ };

ایستا ثابت نویسه* getHWFeatureName(صحیح id)
{
    بازگشت (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : پوچ;
}
ایستا ثابت نویسه* getHWFeatureNameSafe(صحیح id)
{
    ثابت نویسه* name = getHWFeatureName(id);
    بازگشت name ? name : "Unknown feature";
}

ساختار HWFeatures
{
    شمارش { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقی run_initialize = نادرست)
    {
        اگر (run_initialize)
            initialize();
    }

    ایستا تهی initializeNames()
    {
        برای (صحیح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    تهی initialize(تهی)
    {
        اگر (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        صحیح cpuid_data[4] = { 0, 0, 0, 0 };
        صحیح cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        صحیح x86_family = (cpuid_data[0] >> 8) & 15;
        اگر( x86_family >= 6 )
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
            صحیح cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            منطقی have_AVX_OS_support = درست;
            منطقی have_AVX512_OS_support = درست;
            اگر (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = نادرست; // OS uses XSAVE_XRSTORE and CPU support AVX
            وگرنه
            {
                صحیح xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (صحیح)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #وگرنهاگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                اگر ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = نادرست; // YMM registers
                اگر ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = نادرست; // ZMM registers
            }

            اگر (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = نادرست;
                have[CV_CPU_FP16] = نادرست;
                have[CV_CPU_AVX2] = نادرست;
                have[CV_CPU_FMA3] = نادرست;
                have[CV_CPU_AVX_VNNI] = نادرست;
            }
            اگر (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = نادرست;
                have[CV_CPU_AVX_512BW] = نادرست;
                have[CV_CPU_AVX_512CD] = نادرست;
                have[CV_CPU_AVX_512DQ] = نادرست;
                have[CV_CPU_AVX_512ER] = نادرست;
                have[CV_CPU_AVX_512IFMA] = نادرست;
                have[CV_CPU_AVX_512PF] = نادرست;
                have[CV_CPU_AVX_512VBMI] = نادرست;
                have[CV_CPU_AVX_512VL] = نادرست;
                have[CV_CPU_AVX_512VBMI2] = نادرست;
                have[CV_CPU_AVX_512VNNI] = نادرست;
                have[CV_CPU_AVX_512BITALG] = نادرست;
                have[CV_CPU_AVX_512VPOPCNTDQ] = نادرست;
                have[CV_CPU_AVX_5124VNNIW] = نادرست;
                have[CV_CPU_AVX_5124FMAPS] = نادرست;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            اگر (have[CV_CPU_AVX512_COMMON])
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
            وگرنه
            {
                have[CV_CPU_AVX512_KNL] = نادرست;
                have[CV_CPU_AVX512_KNM] = نادرست;
                have[CV_CPU_AVX512_SKX] = نادرست;
                have[CV_CPU_AVX512_CNL] = نادرست;
                have[CV_CPU_AVX512_CLX] = نادرست;
                have[CV_CPU_AVX512_ICL] = نادرست;
            }
        }
    #endif // CV_CPUID_X86

    #اگر defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = درست;
        have[CV_CPU_FP16] = درست;
        صحیح cpufile = open("/proc/self/auxv", O_RDONLY);

        اگر (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t size_auxv_t = اندازه(auxv);

            وقتی ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                اگر (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#اگر defined(AT_HWCAP2)
                وگرنه اگر (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #وگرنهاگر defined __arm__ && defined __ANDROID__
      #اگر defined HAVE_CPUFEATURES
        CV_LOG_INFO(پوچ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(پوچ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #وگرنه
        CV_LOG_INFO(پوچ, "cpufeatures library is not available for CPU detection");
        #اگر CV_NEON
        CV_LOG_INFO(پوچ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = درست;
        #وگرنه
        CV_LOG_INFO(پوچ, "- NEON instructions is NOT enabled via build flags");
        #endif
        #اگر CV_FP16
        CV_LOG_INFO(پوچ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = درست;
        #وگرنه
        CV_LOG_INFO(پوچ, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #وگرنهاگر defined __arm__
        صحیح cpufile = open("/proc/self/auxv", O_RDONLY);

        اگر (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t size_auxv_t = اندازه(auxv);

            وقتی ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                اگر (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    بشکن;
                }
            }

            close(cpufile);
        }
    #endif
    #وگرنهاگر (defined __APPLE__)
    #اگر defined __ARM_NEON
        have[CV_CPU_NEON] = درست;
    #endif
    #اگر (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = درست;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحیح has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = اندازه(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, پوچ, 0);
    اگر (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = درست;
    }
    صحیح has_feat_bf16 = 0;
    size_t has_feat_bf16_size = اندازه(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, پوچ, 0);
    اگر (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = درست;
    }
    #وگرنهاگر (defined __clang__)
    #اگر defined __ARM_NEON
        have[CV_CPU_NEON] = درست;
        #اگر (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = درست;
        #endif
    #endif
    #endif
    #اگر defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = درست;
    #endif
    #اگر defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = درست;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = درست;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = درست;
    #endif

    #اگر (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned صحیح hwcap = getauxval(AT_HWCAP);
        اگر (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            اگر (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = درست;
            } وگرنه {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وگرنهاگر (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        اگر (elf_aux_info(AT_HWCAP, &hwcap, اندازه(hwcap)) != 0)
            hwcap = 0;
        اگر (hwcap & PPC_FEATURE_HAS_VSX) {
            اگر (elf_aux_info(AT_HWCAP2, &hwcap2, اندازه(hwcap2)) != 0)
                hwcap2 = 0;
            اگر (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = درست;
            } وگرنه {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وگرنه
        // TODO: AIX
        #اگر CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = درست;
        #endif
        #اگر CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = درست;
        #endif
    #endif

    #اگر defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = درست;
        #اگر (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = درست;
        #endif
        #اگر defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = درست;
        #endif
    #endif

    #اگر defined __loongarch64 && defined __linux__
        صحیح flag = (صحیح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        منطقی skip_baseline_check = نادرست;
        اگر (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = درست;
        }
        صحیح baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        اگر (!checkFeatures(baseline_features, اندازه(baseline_features) / اندازه(baseline_features[0]))
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
            checkFeatures(baseline_features, اندازه(baseline_features) / اندازه(baseline_features[0]), درست);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, اندازه(baseline_features) / اندازه(baseline_features[0]));
    }

    منطقی checkFeatures(ثابت صحیح* features, صحیح شمار_2, منطقی dump = نادرست)
    {
        منطقی result = درست;
        برای (صحیح i = 0; i < شمار_2; i++)
        {
            صحیح feature = features[i];
            اگر (feature)
            {
                اگر (have[feature])
                {
                    اگر (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                وگرنه
                {
                    result = نادرست;
                    اگر (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        بازگشت result;
    }

    ایستا درونخطی منطقی is_نماد_Separator(نویسه c)
    {
        بازگشت c == ',' || c == ';';
    }

    تهی readSettings(ثابت صحیح* baseline_features, صحیح baseline_شمار)
    {
        منطقی dump = درست;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        اگر (!disabled_features.empty())
        {
            ثابت نویسه* start = disabled_features.c_str();
            برای (;;)
            {
                وقتی (start[0] != 0 && is_نماد_Separator(start[0]))
                {
                    start++;
                }
                اگر (start[0] == 0)
                    بشکن;
                ثابت نویسه* end = start;
                وقتی (end[0] != 0 && !is_نماد_Separator(end[0]))
                {
                    end++;
                }
                اگر (end == start)
                    ادامه;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                منطقی found = نادرست;
                برای (صحیح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    اگر (!g_hwFeatureNames[i]) ادامه;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    اگر (len != feature.size()) ادامه;
                    اگر (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        منطقی isBaseline = نادرست;
                        برای (صحیح k = 0; k < baseline_شمار; k++)
                        {
                            اگر (baseline_features[k] == i)
                            {
                                isBaseline = درست;
                                بشکن;
                            }
                        }
                        اگر (isBaseline)
                        {
                            اگر (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        اگر (!have[i])
                        {
                            اگر (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = نادرست;

                        found = درست;
                        بشکن;
                    }
                }
                اگر (!found)
                {
                    اگر (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقی have[MAX_FEATURE+1]{};
};

ایستا HWFeatures  featuresEnabled(درست), featuresDisabled = HWFeatures(نادرست);
ایستا HWFeatures* currentFeatures = &featuresEnabled;

منطقی checkHardwareSupport(صحیح feature)
{
    اگر (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        بازگشت نادرست;
    بازگشت currentFeatures->have[feature];
}

String getHardwareFeatureName(صحیح feature)
{
    ثابت نویسه* name = getHWFeatureName(feature);
    بازگشت name ? String(name) : String();
}

std::string get_CPUFeatures_سطر()
{
    ثابت صحیح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحیح sz = اندازه(features) / اندازه(features[0]);
    std::string result;
    std::string prefix;
    برای (صحیح i = 1; i < sz; ++i)
    {
        اگر (features[i] == 0)
        {
            prefix = "*";
            ادامه;
        }
        اگر (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        اگر (!checkHardwareSupport(features[i])) result.append("?");
    }
    بازگشت result;
}

فرار منطقی useOptimizedFlag = درست;

تهی setUseOptimized( منطقی flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

منطقی useOptimized(تهی)
{
    بازگشت useOptimizedFlag;
}

int64 get_Tick_شمار(تهی)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    بازگشت (int64)now.time_since_epoch().شمار_2();
}

دوگانه getTickFrequency(تهی)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    دوگانه clock_freq = clock_period_t::den / clock_period_t::num;
    بازگشت clock_freq;
}

#اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#اگر defined(__i386__)

int64 get_CPUTick_شمار(تهی)
{
    int64 x;
    __asm__ فرار (".byte 0x0f, 0x31" : "=A" (x));
    بازگشت x;
}
#وگرنهاگر defined(__x86_64__)

int64 get_CPUTick_شمار(تهی)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    بازگشت (int64)lo | ((int64)hi << 32);
}

#وگرنهاگر defined(__ppc__)

int64 get_CPUTick_شمار(تهی)
{
    unsigned upper, lower, tmp;
    __asm__ فرار(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    بازگشت lower | ((int64)upper << 32);
}

#وگرنه

#error "RDTSC not defined"

#endif

#وگرنهاگر defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_شمار(تهی)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#وگرنه

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_شمار(تهی)
{
    بازگشت get_Tick_شمار();
}
//#endif

#endif


فضاینام internal {

رده Timestamp
{
عمومی:
    ثابت int64 zero_Tick_شمار;
    ثابت دوگانه ns_in_ticks;

    Timestamp()
        : zero_Tick_شمار(get_Tick_شمار())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_شمار();
        بازگشت (int64)((t - zero_Tick_شمار) * ns_in_ticks);
    }

    ایستا Timestamp& getInstance()
    {
        ایستا Timestamp g_timestamp;
        بازگشت g_timestamp;
    }
};

رده InitTimestamp {
عمومی:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ایستا InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    بازگشت internal::Timestamp::getInstance().getTimestamp();
}


ثابت String& getBuildInformation()
{
    ایستا String build_info =
#شامل "version_string.inc"
    ;
    بازگشت build_info;
}

String getVersionString() { بازگشت String(CV_VERSION); }

صحیح getVersionMajor() { بازگشت CV_VERSION_MAJOR; }

صحیح getVersionMinor() { بازگشت CV_VERSION_MINOR; }

صحیح getVersionRevision() { بازگشت CV_VERSION_REVISION; }

String format( ثابت نویسه* fmt, ... )
{
    AutoBuffer<نویسه, 1024> buf;

    برای ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحیح bsize = static_cast<صحیح>(buf.size());
        صحیح len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        اگر (len >= bsize)
        {
            buf.resize(len + 1);
            ادامه;
        }
        buf[bsize - 1] = 0;
        بازگشت String(buf.data(), len);
    }
}

String tempfile( ثابت نویسه* suffix )
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#اگر defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    اگر (temp_file.empty())
        بازگشت String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    نویسه aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#وگرنهاگر defined(_WIN32_WCE)
    ثابت auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    اگر(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        نویسه aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#وگرنه
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    اگر (temp_dir.empty())
    {
        ::GetTempPathW(اندازه(temp_dir2_w)/اندازه(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحیح len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, پوچ, 0, پوچ, پوچ);
        اگر (len > 0)
        {
            std::vector<نویسه> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, پوچ, پوچ);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    اگر (FAILED(hr))
        بازگشت String();
    نویسه guidStr[40];
    ثابت نویسه* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, اندازه(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned صحیح)g.Data4[0], (unsigned صحیح)g.Data4[1],
            (unsigned صحیح)g.Data4[2], (unsigned صحیح)g.Data4[3], (unsigned صحیح)g.Data4[4],
            (unsigned صحیح)g.Data4[5], (unsigned صحیح)g.Data4[6], (unsigned صحیح)g.Data4[7]);

    fname = temp_dir;
    اگر (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# وگرنه
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    نویسه defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  وگرنه
    نویسه defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    اگر (temp_dir.empty())
        fname = defaultTemplate;
    وگرنه
    {
        fname = temp_dir;
        نویسه ech = fname[fname.size() - 1];
        اگر(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ثابت صحیح fd = mkstemp((نویسه*)fname.c_str());
    اگر (fd == -1) بازگشت String();

    close(fd);
    remove(fname.c_str());
# endif

    اگر (suffix)
    {
        اگر (suffix[0] != '.')
            بازگشت fname + "." + suffix;
        وگرنه
            بازگشت fname + suffix;
    }
    بازگشت fname;
#وگرنه // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ایستا ErrorCallback customErrorCallback = 0;
ایستا تهی* customErrorCallbackData = 0;
ایستا منطقی breakOnError = نادرست;

منطقی setBreakOnError(منطقی value)
{
    منطقی prevVal = breakOnError;
    breakOnError = value;
    بازگشت prevVal;
}

صحیح cv_snprintf(نویسه* buf, صحیح len, ثابت نویسه* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    صحیح res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    بازگشت res;
}

صحیح cv_vsnprintf(نویسه* buf, صحیح len, ثابت نویسه* fmt, va_list args)
{
#اگر defined _MSC_VER
    اگر (len <= 0) بازگشت len == 0 ? 1024 : -1;
    صحیح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    اگر (res >= 0 && res < len)
    {
        buf[res] = 0;
        بازگشت res;
    }
    وگرنه
    {
        buf[len - 1] = 0; // truncate happened
        بازگشت res >= len ? res : (len * 2);
    }
#وگرنه
    بازگشت vsnprintf(buf, len, fmt, args);
#endif
}

ایستا تهی dumpException(ثابت Exception& exc)
{
    ثابت نویسه* errorStr = exc.codeMessage();
    نویسه buf[1 << 12];

    cv_snprintf(buf, اندازه(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.سطر);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#وگرنه
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
ایستا منطقی cv_terminate_handler_installed = نادرست;
ایستا std::terminate_handler cv_old_terminate_handler;
ایستا cv::Exception cv_terminate_handler_exception;
ایستا منطقی param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", درست);
ایستا تهی cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    اگر (نادرست /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# اگر defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

تهی error( ثابت Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        اگر (!cv_terminate_handler_installed)
        {
            اگر (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = درست;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    اگر (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.سطر, customErrorCallbackData);
    وگرنه اگر (param_dumpErrors)
    {
        dumpException(exc);
    }

    اگر(breakOnError)
    {
        std::terminate();
    }

    پرتابکن exc;
#ifdef __GNUC__
# اگر !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

تهی error(Error::Code _code, ثابت String& _err, ثابت نویسه* _func, ثابت نویسه* _file, صحیح سطر_3)
{
    error(cv::Exception(_code, _err, _func, _file, سطر_3));
#ifdef __GNUC__
# اگر !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# اگر defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, تهی* userdata, تهی** prevUserdata)
{
    اگر( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    بازگشت prevCallback;
}

تهی terminate(Error::Code _code, ثابت String& _err, ثابت نویسه* _func, ثابت نویسه* _file, صحیح سطر_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, سطر_3));
    std::terminate();
}

}

فضاینام cv {
منطقی __termination = نادرست;


//////////////////////////////// thread-local storage ////////////////////////////////

فضاینام details {

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
رده TlsAbstraction
{
عمومی:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    تهی* getData() ثابت;
    تهی setData(تهی *pData);

    تهی releaseSystemResources();

خصوصی:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_کلید;
    منطقی disposed;
#endif
#وگرنه // _WIN32
    pthread_key_t  tls_کلید;
    std::atomic<منطقی> disposed;
#endif
};

رده TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
عمومی:
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
ایستا TlsAbstraction* getTlsAbstraction()
{
    ایستا TlsAbstraction *g_tls = جدید TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ایستا TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    بازگشت g_tls;
}


#ifdef _WIN32
#ifdef WINRT
ایستا __declspec( thread ) تهی* tlsData = پوچ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
تهی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
}
تهی* TlsAbstraction::getData() ثابت
{
    بازگشت tlsData;
}
تهی TlsAbstraction::setData(تهی *pData)
{
    tlsData = pData;
}
#وگرنه //WINRT
#ifdef CV_USE_FLS
ایستا تهی NTAPI opencv_fls_destructor(تهی* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(نادرست)
{
#ifndef CV_USE_FLS
    tls_کلید = TlsAlloc();
#وگرنه // CV_USE_FLS
    tls_کلید = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_کلید != TLS_OUT_OF_INDEXES);
}
تهی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
    disposed = درست;
#ifndef CV_USE_FLS
    TlsFree(tls_کلید);
#وگرنه // CV_USE_FLS
    FlsFree(tls_کلید);
#endif // CV_USE_FLS
    tls_کلید = TLS_OUT_OF_INDEXES;
}
تهی* TlsAbstraction::getData() ثابت
{
    اگر (disposed)
        بازگشت پوچ;
#ifndef CV_USE_FLS
    بازگشت Tls_Get_مقدار(tls_کلید);
#وگرنه // CV_USE_FLS
    بازگشت Fls_Get_مقدار(tls_کلید);
#endif // CV_USE_FLS
}
تهی TlsAbstraction::setData(تهی *pData)
{
    اگر (disposed)
        بازگشت;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_مقدار(tls_کلید, pData) == TRUE);
#وگرنه // CV_USE_FLS
    CV_Assert(Fls_Set_مقدار(tls_کلید, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#وگرنه // _WIN32
ایستا تهی opencv_tls_destructor(تهی* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(نادرست)
{
    CV_Assert(pthread_key_create(&tls_کلید, opencv_tls_destructor) == 0);
}
تهی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
    disposed = درست;
    اگر (pthread_key_delete(tls_کلید) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
تهی* TlsAbstraction::getData() ثابت
{
    اگر (disposed)
        بازگشت پوچ;
    بازگشت pthread_getspecific(tls_کلید);
}
تهی TlsAbstraction::setData(تهی *pData)
{
    اگر (disposed)
        بازگشت;  // no-op
    CV_Assert(pthread_setspecific(tls_کلید, pData) == 0);
}
#endif

// Per-thread data structure
ساختار ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<تهی*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ایستا منطقی g_isTlsStorageInitialized = نادرست;

// Main TLS storage class
رده TlsStorage
{
عمومی:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (تهی)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = درست;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    تهی releaseThread(تهی* tls_مقدار = پوچ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        اگر (پوچ == tls)
            بازگشت;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_مقدار == پوچ ? (ThreadData*)tls->getData() : (ThreadData*)tls_مقدار;
        اگر (pTD == پوچ)
            بازگشت;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        برای (size_t i = 0; i < threads.size(); i++)
        {
            اگر (pTD == threads[i])
            {
                threads[i] = پوچ;
                اگر (tls_مقدار == پوچ)
                    tls->setData(0);
                std::vector<تهی*>& thread_slots = pTD->slots;
                برای (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    تهی* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = پوچ;
                    اگر (!pData)
                        ادامه;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    اگر (container)
                        container->deleteDataInstance(pData);
                    وگرنه
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحیح)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                بازگشت;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (تهی*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        برای(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            اگر (tlsSlots[slot].container == پوچ)
            {
                tlsSlots[slot].container = container;
                بازگشت slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        بازگشت tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    تهی releaseSlot(size_t slotIdx, std::vector<تهی*> &dataVec, منطقی keepSlot = نادرست)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        برای(size_t i = 0; i < threads.size(); i++)
        {
            اگر(threads[i])
            {
                std::vector<تهی*>& thread_slots = threads[i]->slots;
                اگر (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = پوچ;
                }
            }
        }

        اگر (!keepSlot)
        {
            tlsSlots[slotIdx].container = پوچ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    تهی* getData(size_t slotIdx) ثابت
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        اگر (پوچ == tls)
            بازگشت پوچ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        اگر(threadData && threadData->slots.size() > slotIdx)
            بازگشت threadData->slots[slotIdx];

        بازگشت پوچ;
    }

    // Gather data from threads by TLS storage index
    تهی gather(size_t slotIdx, std::vector<تهی*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        برای(size_t i = 0; i < threads.size(); i++)
        {
            اگر(threads[i])
            {
                std::vector<تهی*>& thread_slots = threads[i]->slots;
                اگر (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    تهی setData(size_t slotIdx, تهی* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        اگر (پوچ == tls)
            بازگشت;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        اگر(!threadData)
        {
            threadData = جدید ThreadData;
            tls->setData((تهی*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                منطقی found = نادرست;
                // Find unused slots
                برای(size_t slot = 0; slot < threads.size(); slot++)
                {
                    اگر (threads[slot] == پوچ)
                    {
                        threadData->idx = (صحیح)slot;
                        threads[slot] = threadData;
                        found = درست;
                        بشکن;
                    }
                }

                اگر (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        اگر(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, پوچ);
        }
        threadData->slots[slotIdx] = pData;
    }

خصوصی:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ساختار TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ساختار TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ایستا TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, جدید TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
ایستا تهی opencv_tls_destructor(تهی* pData)
{
    اگر (!g_isTlsStorageInitialized)
        بازگشت;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#وگرنه // _WIN32
#ifdef CV_USE_FLS
ایستا تهی WINAPI opencv_fls_destructor(تهی* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    اگر (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = درست;
    }

    اگر (!g_isTlsStorageInitialized)
        بازگشت;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

ایستا TlsStorage* ثابت g_force_initialization_of_TlsStorage
#اگر defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#وگرنه  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
رده TlsStorage
{
عمومی:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        برای (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            اگر (container && s.data)
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
        برای (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            اگر (s.container == پوچ)
            {
                CV_Assert(!s.data);
                s.container = container;
                بازگشت slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        بازگشت slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    تهی releaseSlot(size_t slotIdx, std::vector<تهی*> &dataVec, منطقی keepSlot = نادرست)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        تهی* data = s.data;
        اگر (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        اگر (!keepSlot)
        {
            s.container = پوچ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    تهی* getData(size_t slotIdx) ثابت
    {
        CV_Assert(slotIdx < slots.size());
        ثابت SlotInfo& s = slots[slotIdx];
        بازگشت s.data;
    }

    // Gather data from threads by TLS storage index
    تهی gather(size_t slotIdx, std::vector<تهی*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        تهی* data = s.data;
        اگر (data)
            dataVec.push_back(data);
        بازگشت;
    }

    // Set data to storage index
    تهی setData(size_t slotIdx, تهی* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

خصوصی:
    ساختار SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        تهی* data;
    };
    std::vector<ساختار SlotInfo> slots;
};

ایستا TlsStorage& getTlsStorage()
{
    ایستا TlsStorage g_storage;  // no threading
    بازگشت g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using فضاینام details;

تهی releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    اگر (!g_isTlsStorageInitialized)
        بازگشت;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    کلید_2 = (صحیح)getTlsStorage().reserveSlot(این); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(کلید_2 == -1); // Key must be released in child object
}

تهی TLSDataContainer::gatherData(std::vector<تهی*> &data) ثابت
{
    getTlsStorage().gather(کلید_2, data);
}

تهی TLSDataContainer::detachData(std::vector<تهی*> &data)
{
    getTlsStorage().releaseSlot(کلید_2, data, درست);
}

تهی TLSDataContainer::release()
{
    اگر (کلید_2 == -1)
        بازگشت;  // already released
    std::vector<تهی*> data; data.reserve(32);
    getTlsStorage().releaseSlot(کلید_2, data, نادرست); // Release key and get stored data for proper destruction
    کلید_2 = -1;
    برای(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

تهی TLSDataContainer::cleanup()
{
    std::vector<تهی*> data; data.reserve(32);
    getTlsStorage().releaseSlot(کلید_2, data, درست); // Extract stored data with removal from TLS tables
    برای(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

تهی* TLSDataContainer::getData() ثابت
{
    CV_Assert(کلید_2 != -1 && "Can't fetch data from terminated TLS container.");
    تهی* pData = getTlsStorage().getData(کلید_2); // Check if data was already allocated
    اگر(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        تلاش
        {
            getTlsStorage().setData(کلید_2, pData);
        }
        بگیر (...)
        {
            deleteDataInstance(pData);
            پرتابکن;
        }
    }
    بازگشت pData;
}

ایستا TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, جدید TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    بازگشت getCoreTlsDataTLS().getRef();
}

#اگر defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

بیرونی "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

بیرونی "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    اگر (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        اگر (lpReserved != پوچ) // called after ExitProcess() call
        {
            cv::__termination = درست;
        }
        وگرنه
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    بازگشت TRUE;
}
#endif


فضاینام {

#ifdef OPENCV_WITH_ITT
منطقی overrideThreadName()
{
    ایستا منطقی param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", نادرست);
    بازگشت param;
}
#endif

ایستا صحیح g_thread_رقم = 0;
رده ThreadID {
عمومی:
    ثابت صحیح id;
    ThreadID() :
        id(CV_XADD(&g_thread_رقم, 1))
    {
#ifdef OPENCV_WITH_ITT
        اگر (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

ایستا TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, جدید TLSData<ThreadID>());
}

} // namespace
صحیح utils::getThreadID() { بازگشت getThreadIDTLS().get()->id; }


رده ParseError
{
    std::string bad_مقدار;
عمومی:
    ParseError(ثابت std::string &bad_مقدار_2) :bad_مقدار(bad_مقدار_2) {}
    std::string toString(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_مقدار;
        بازگشت out.str();
    }
};

template <typename T>
T parseOption(ثابت std::string &);

template<>
درونخطی منطقی parseOption(ثابت std::string & value)
{
    اگر (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        بازگشت درست;
    }
    اگر (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        بازگشت نادرست;
    }
    پرتابکن ParseError(value);
}

template<>
درونخطی size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    برای (; pos < value.size(); pos++)
    {
        اگر (!isdigit(value[pos]))
            بشکن;
    }
    cv::String مقدار_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(مقدار_Str);
    اگر (suffixStr.length() == 0)
        بازگشت v;
    وگرنه اگر (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        بازگشت v * 1024 * 1024;
    وگرنه اگر (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        بازگشت v * 1024;
    پرتابکن ParseError(value);
}

template<>
درونخطی cv::String parseOption(ثابت std::string &value)
{
    بازگشت value;
}

template<>
درونخطی utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ثابت نویسه sep = ';';
#وگرنه
    ثابت نویسه sep = ':';
#endif
    size_t start_pos = 0;
    وقتی (start_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, start_pos);
        ثابت std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        اگر (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    بازگشت result;
}

ایستا درونخطی ثابت نویسه * envRead(ثابت نویسه * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    بازگشت پوچ;
#وگرنه
    بازگشت getenv(name);
#endif
}

template<typename T>
درونخطی T اقرأ(ثابت std::string & k, ثابت T & default_مقدار_2)
{
    تلاش
    {
        ثابت نویسه * res = envRead(k.c_str());
        اگر (res)
            بازگشت parseOption<T>(std::string(res));
    }
    بگیر (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    بازگشت default_مقدار_2;
}

منطقی utils::getConfigurationParameterBool(ثابت نویسه* name, منطقی default_مقدار_2)
{
    بازگشت اقرأ<منطقی>(name, default_مقدار_2);
}

size_t utils::getConfigurationParameterSizeT(ثابت نویسه* name, size_t default_مقدار_2)
{
    بازگشت اقرأ<size_t>(name, default_مقدار_2);
}

std::string utils::getConfigurationParameterString(ثابت نویسه* name, ثابت std::string & default_مقدار_2)
{
    بازگشت اقرأ<cv::String>(name, default_مقدار_2);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت نویسه* name, ثابت utils::Paths &default_مقدار_2)
{
    بازگشت اقرأ<utils::Paths>(name, default_مقدار_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, جدید ImplCollector())
}

تهی setImpl(صحیح flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

تهی addImpl(صحیح flag, ثابت نویسه* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    اگر(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        اگر(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

صحیح getImpl(std::vector<صحیح> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    بازگشت getImplData().implFlags; // return actual flags for lazy collection
}

منطقی useCollection()
{
    بازگشت getImplData().useCollection;
}

تهی setUseCollection(منطقی flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

فضاینام instr
{
منطقی useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    بازگشت getInstrumentStruct().useInstr;
#وگرنه
    بازگشت نادرست;
#endif
}

تهی setUseInstrumentation(منطقی flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#وگرنه
    CV_UNUSED(flag);
#endif
}

Instr_گره* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    بازگشت &getInstrumentStruct().ریشه_گره_2;
#وگرنه
    بازگشت پوچ;
#endif
}

تهی resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().ریشه_گره_2.removeChilds();
    getInstrumentTLSStruct().p_Current_گره = &getInstrumentStruct().ریشه_گره_2;
#endif
}

تهی setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#وگرنه
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    بازگشت (FLAGS)getInstrumentStruct().flags;
#وگرنه
    بازگشت (FLAGS)0;
#endif
}

گره_Data::گره_Data(ثابت نویسه* funName, ثابت نویسه* fileName, صحیح سطر_رقم, تهی* retAddress, منطقی alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_سطر_رقم       = سطر_رقم;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = نادرست;
}
گره_Data::گره_Data(گره_Data &ref)
{
    *این = ref;
}
گره_Data& گره_Data::operator=(ثابت گره_Data &راست)
{
    این->m_funName      = راست.m_funName;
    این->m_instrType    = راست.m_instrType;
    این->m_implType     = راست.m_implType;
    این->m_fileName     = راست.m_fileName;
    این->m_سطر_رقم      = راست.m_سطر_رقم;
    این->m_retAddress   = راست.m_retAddress;
    این->m_alwaysExpand = راست.m_alwaysExpand;

    این->m_threads     = راست.m_threads;
    این->m_counter     = راست.m_counter;
    این->m_ticksTotal  = راست.m_ticksTotal;

    این->m_funError    = راست.m_funError;

    بازگشت *این;
}
گره_Data::~گره_Data()
{
}
منطقی operator==(ثابت گره_Data& چپ, ثابت گره_Data& راست)
{
    اگر(چپ.m_سطر_رقم == راست.m_سطر_رقم && چپ.m_funName == راست.m_funName && چپ.m_fileName == راست.m_fileName)
    {
        اگر(چپ.m_retAddress == راست.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || چپ.m_alwaysExpand))
            بازگشت درست;
    }
    بازگشت نادرست;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ایستا InstrStruct instr;
    بازگشت instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    بازگشت *getInstrumentStruct().tlsStruct.get();
}

Instr_گره* get_Current_گره()
{
    بازگشت getInstrumentTLSStruct().p_Current_گره;
}

IntrumentationRegion::IntrumentationRegion(ثابت نویسه* funName, ثابت نویسه* fileName, صحیح سطر_رقم, تهی *retAddress, منطقی alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = نادرست;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    اگر(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        اگر(!pTLS->p_Current_گره)
        {
            m_disabled = درست;
            بازگشت;
        }

        صحیح depth = pTLS->p_Current_گره->getDepth();
        اگر(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = درست;
            بازگشت;
        }

        گره_Data payload(funName, fileName, سطر_رقم, retAddress, alwaysExpand, instrType, implType);
        Node<گره_Data>* pChild = پوچ;

        اگر(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_گره->findChild(payload);
            اگر(!pChild)
            {
                pChild = جدید Node<گره_Data>(payload);
                pTLS->p_Current_گره->addChild(pChild);
            }
        }
        وگرنه
        {
            pChild = pTLS->p_Current_گره->findChild(payload);
            اگر(!pChild)
            {
                m_disabled = درست;
                بازگشت;
            }
        }
        pTLS->p_Current_گره = pChild;

        m_regionTicks = get_Tick_شمار();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    اگر(pStruct->useInstr)
    {
        اگر(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            اگر (pTLS->p_Current_گره->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_گره->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_گره->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_شمار() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_شمار); // Concurrent ticks accumulation
                pTLS->p_Current_گره->m_payload.m_counter++;
                pTLS->p_Current_گره->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_گره->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_گره = pTLS->p_Current_گره->m_pParent;
        }
    }
}
#endif
}

فضاینام ipp
{

#ifdef HAVE_IPP
ساختار IPPInitSingleton
{
عمومی:
    IPPInitSingleton()
    {
        useIPP         = درست;

#اگر defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#وگرنه
        useIPP_NE      = نادرست;
#endif
        ippStatus      = 0;
        funcname       = پوچ;
        filename       = پوچ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_بالاترین_Features = 0;
        pIppLibInfo    = پوچ;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, پوچ);
        اگر(ippStatus < 0)
        {
            CV_LOG_ERROR(پوچ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = نادرست;
            بازگشت;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        اگر(!env.empty())
        {
#اگر IPP_VERSION_X100 >= 201900
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#وگرنهاگر IPP_VERSION_X100 >= 201703
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وگرنهاگر IPP_VERSION_X100 >= 201700
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وگرنه
            ثابت Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            اگر(env.substr(0, 2) == "ne")
            {
                useIPP_NE = درست;
                env = env.substr(3, env.size());
            }

            اگر(env == "disabled")
            {
                CV_LOG_WARNING(پوچ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = نادرست;
            }
            وگرنه اگر(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            وگرنه اگر(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#اگر IPP_VERSION_X100 >= 201700
#اگر defined (_M_AMD64) || defined (__x86_64__)
            وگرنه اگر(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            وگرنه
                CV_LOG_ERROR(پوچ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        اگر(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        اگر(!(
#اگر IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = نادرست;
            بازگشت;
        }

        اگر(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        وگرنه
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#اگر IPP_VERSION_X100 >= 201700
        اگر(ippFeatures&ippCPUID_AVX512F)
        {
            اگر((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_بالاترین_Features = ippCPUID_AVX512_SKX;
            وگرنه اگر((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_بالاترین_Features = ippCPUID_AVX512_KNL;
            وگرنه
                ipp_بالاترین_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        وگرنه
#endif
        اگر(ippFeatures&ippCPUID_AVX2)
            ipp_بالاترین_Features = ippCPUID_AVX2;
        وگرنه اگر(ippFeatures&ippCPUID_SSE42)
            ipp_بالاترین_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        اگر (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_بالاترین_Features = ippCPUID_SSE42;
        }
    }

عمومی:
    منطقی        useIPP;
    منطقی        useIPP_NE;

    صحیح         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ثابت نویسه *funcname;
    ثابت نویسه *filename;
    صحیح         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_بالاترین_Features;
    ثابت IppLibraryVersion *pIppLibInfo;
};

ایستا IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, جدید IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    بازگشت getIPPSingleton().ippFeatures;
#وگرنه
    بازگشت 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_بالاترین_Features()
{
    بازگشت getIPPSingleton().ipp_بالاترین_Features;
}
#endif

تهی setIppStatus(صحیح status, ثابت نویسه * ثابت _funcname, ثابت نویسه * ثابت _filename, صحیح سطر_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = سطر_3;
#وگرنه
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(سطر_3);
#endif
}

صحیح getIppStatus()
{
#ifdef HAVE_IPP
    بازگشت getIPPSingleton().ippStatus;
#وگرنه
    بازگشت 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    بازگشت format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#وگرنه
    بازگشت String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ثابت IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    اگر(pInfo)
        بازگشت format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    وگرنه
        بازگشت String("error");
#وگرنه
    بازگشت String("disabled");
#endif
}

منطقی useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    اگر (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    بازگشت (data.useIPP > 0);
#وگرنه
    بازگشت نادرست;
#endif
}

تهی setUseIPP(منطقی flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:نادرست;
#وگرنه
    CV_UNUSED(flag);
    data.useIPP = نادرست;
#endif
}

منطقی useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    اگر (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    بازگشت (data.useIPP_NE > 0);
#وگرنه
    بازگشت نادرست;
#endif
}

تهی setUseIPP_NotExact(منطقی flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#وگرنه
    CV_UNUSED(flag);
    data.useIPP_NE = نادرست;
#endif
}

} // namespace ipp


فضاینام details {

#اگر OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

تهی setFPDenormalsIgnoreHint(منطقی ignore, CV_OUT FPDenormalsModeState& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    اگر (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ثابت unsigned old_flags = _mm_getcsr();
    ثابت unsigned old_مقدار = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(پوچ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_مقدار;
    _mm_setcsr(flags);
#وگرنه
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

صحیح saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    اگر (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ثابت unsigned old_flags = _mm_getcsr();
    ثابت unsigned old_مقدار = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_مقدار;
    بازگشت 2;
#وگرنه
    CV_UNUSED(state);
    بازگشت 0;
#endif
}

منطقی restoreFPDenormalsState(ثابت FPDenormalsModeState& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    ثابت unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((صحیح)value, value == (value & mask), "invalid SSE FP state");
    ثابت unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(پوچ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    بازگشت درست;
#وگرنه
    CV_UNUSED(state);
    بازگشت نادرست;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    بازگشت OPENCV_ALGO_HINT_DEFAULT;
#وگرنه
    بازگشت ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
