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

#ضمن "precomp.hpp"
//#include "opencv2/core/core_c.h"
#ضمن <atomic>
#ضمن <exception>
#ضمن <iostream>
#ضمن <ostream>

#ifdef __QNX__
    #ضمن <unistd.h>
    #ضمن <sys/neutrino.h>
    #ضمن <sys/syspage.h>
#ifdef __aarch64__
    #ضمن <aarch64/syspage.h>
#endif
#endif

#ضمن <opencv2/core/utils/configuration.private.hpp>
#ضمن <opencv2/core/utils/trace.private.hpp>

#ضمن <opencv2/core/utils/logger.hpp>

#ضمن <opencv2/core/utils/tls.hpp>
#ضمن <opencv2/core/utils/instrumentation.hpp>

#ضمن <opencv2/core/utils/filesystem.private.hpp>

#ضمن <opencv2/core/utils/fp_control_utils.hpp>
#ضمن <opencv2/core/utils/fp_control.private.hpp>

نطاق cv {

ساكن فراغ _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ساكن std::ios_base::Init s_iostream_initializer;
#endif
}

ساكن Mutex* __initialization_mutex = عدم;
Mutex& getInitializationMutex()
{
    אם (__initialization_mutex == عدم)
    {
        (فراغ)_initSystem();
        __initialization_mutex = جديد Mutex();
    }
    החזר *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ساكن منطقي param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#אם defined(_DEBUG) || defined(__ANDROID__)
    אמת
#אחרת
    שקר
#endif
);

فراغ* allocSingletonBuffer(size_t size) { החזר fastMalloc(size); }
فراغ* allocSingletonNewBuffer(size_t size) { החזר malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# אם defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#אם defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# אם _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#ضمن <cstdlib>        // std::abort
#endif

#אם defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#אם defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#אחרת
#  include <elf.h>
#endif
#אם defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#אם defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#אם ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#אם defined __loongarch64
#ضمن "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#אם defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#ضمن <windows.h>
#ضمن <combaseapi.h>
#אם (_WIN32_WINNT >= 0x0602)
  #ضمن <synchapi.h>
#endif
#אם ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ضمن <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#ضمن <tchar.h>

#ifdef WINRT
#ضمن <wrl/client.h>
#ifndef __cplusplus_winrt
#ضمن <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    החזר std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#אחרת
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    אם (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        החזר wstr;
    אם (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        החזר wstr;
    אם (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        החזר wstr;
    אם (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        החזר wstr;
    אם (FAILED(storagefolderRef.As(&storageitemRef)))
        החזר wstr;
    str = عدم;
    אם (FAILED(storageitemRef->get_Path(&str)))
        החזר wstr;
    wstr = WindowsGetStringRawBuffer(str, عدم);
    WindowsDeleteString(str);
    החזר wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], حجم(guidStr)/حجم(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    החזר prefix.append(std::wstring(guidStr));
}

#endif
#אחרת
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#ضمن <pthread.h>
#endif
#ضمن <sys/time.h>
#ضمن <time.h>

#אם defined __MACH__ && defined __APPLE__
#ضمن <mach/mach.h>
#ضمن <mach/mach_time.h>
#ضمن <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#ضمن "omp.h"
#endif

#אם defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#אם defined __ANDROID__
#ضمن <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #אם defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #אם _MSC_VER >= 1400  // MSVS 2005
      #ضمن <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #אחרת
      #error "Required MSVS 2005+"
    #endif
  #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ساكن فراغ cv_cpuid(صحيح* cpuid_data, صحيح reg_eax, صحيح reg_ecx)
    {
        صحيح __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#אם !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#وإلاإذا defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#אחרת
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#ضمن <chrono>

نطاق cv
{

Exception::Exception() { code = Error::StsOk; line = 0; }

Exception::Exception(Error::Code _code, ثابت String& _err, ثابت String& _func, ثابت String& _file, صحيح _line)
: code(_code), err(_err), func(_func), file(_file), line(_line)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ثابت حرف* Exception::what() ثابت CV_NOEXCEPT { החזר msg.c_str(); }

فراغ Exception::formatMessage()
{
    size_t pos = err.find('\n');
    منطقي multiline = pos != cv::String::npos;
    אם (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        כלעוד (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        אם (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    אם (func.size() > 0)
    {
        אם (multiline)
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), line, code, codeMessage(), func.c_str(), err.c_str());
        אחרת
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), func.c_str());
    }
    אחרת
    {
        msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت حرف * Exception::codeMessage() ثابت
{
    بدل (code)
    {
    حالة cv::Error::StsOk :                  החזר "No Error";
    حالة cv::Error::StsBackTrace :           החזר "Backtrace";
    حالة cv::Error::StsError :               החזר "Unspecified error";
    حالة cv::Error::StsInternal :            החזר "Internal error";
    حالة cv::Error::StsNoMem :               החזר "Insufficient memory";
    حالة cv::Error::StsBadArg :              החזר "Bad argument";
    حالة cv::Error::StsNoConv :              החזר "Iterations do not converge";
    حالة cv::Error::StsAutoTrace :           החזר "Autotrace call";
    حالة cv::Error::StsBadSize :             החזר "Incorrect size of input array";
    حالة cv::Error::StsNullPtr :             החזר "Null pointer";
    حالة cv::Error::StsDivByZero :           החזר "Division by zero occurred";
    حالة cv::Error::BadStep :                החזר "Image step is wrong";
    حالة cv::Error::StsInplaceNotSupported : החזר "Inplace operation is not supported";
    حالة cv::Error::StsObjectNotFound :      החזר "Requested object was not found";
    حالة cv::Error::BadDepth :               החזר "Input image depth is not supported by function";
    حالة cv::Error::StsUnmatchedFormats :    החזר "Formats of input arguments do not match";
    حالة cv::Error::StsUnmatchedSizes :      החזר "Sizes of input arguments do not match";
    حالة cv::Error::StsOutOfRange :          החזר "One of the arguments\' values is out of range";
    حالة cv::Error::StsUnsupportedFormat :   החזר "Unsupported format or combination of formats";
    حالة cv::Error::BadCOI :                 החזר "Input COI is not supported";
    حالة cv::Error::BadNumChannels :         החזר "Bad number of channels";
    حالة cv::Error::StsBadFlag :             החזר "Bad flag (parameter or structure field)";
    حالة cv::Error::StsBadPoint :            החזר "Bad parameter of type CvPoint";
    حالة cv::Error::StsBadMask :             החזר "Bad type of mask argument";
    حالة cv::Error::StsParseError :          החזר "Parsing error";
    حالة cv::Error::StsNotImplemented :      החזר "The function/feature is not implemented";
    حالة cv::Error::StsBadMemBlock :         החזר "Memory block has been corrupted";
    حالة cv::Error::StsAssert :              החזר "Assertion failed";
    حالة cv::Error::GpuNotSupported :        החזר "No CUDA support";
    حالة cv::Error::GpuApiCallError :        החזר "Gpu API call";
    حالة cv::Error::OpenGlNotSupported :     החזר "No OpenGL support";
    حالة cv::Error::OpenGlApiCallError :     החזר "OpenGL API call";
    حالة cv::Error::OpenCLApiCallError :     החזר "OpenCL API call error";
    حالة cv::Error::OpenCLDoubleNotSupported:החזר "OpenCL device does not support double";
    حالة cv::Error::OpenCLInitError :        החזר "OpenCL initialization error";
    حالة cv::Error::OpenCLNoAMDBlasFft :     החזר "Missing OpenCL AMD BLAS FFT";
    افتراضي:
    {
        ساكن حرف buf[256] {0};
        snprintf(buf, حجم(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        החזר buf;
    }
    };

}

ساكن ثابت حرف* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { عدم };

ساكن ثابت حرف* getHWFeatureName(صحيح id)
{
    החזר (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : عدم;
}
ساكن ثابت حرف* getHWFeatureNameSafe(صحيح id)
{
    ثابت حرف* name = getHWFeatureName(id);
    החזר name ? name : "Unknown feature";
}

بنية HWFeatures
{
    تعداد { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقي run_initialize = שקר)
    {
        אם (run_initialize)
            initialize();
    }

    ساكن فراغ initializeNames()
    {
        עבור (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    فراغ initialize(فراغ)
    {
        אם (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        صحيح cpuid_data[4] = { 0, 0, 0, 0 };
        صحيح cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        صحيح x86_family = (cpuid_data[0] >> 8) & 15;
        אם( x86_family >= 6 )
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
            صحيح cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            منطقي have_AVX_OS_support = אמת;
            منطقي have_AVX512_OS_support = אמת;
            אם (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = שקר; // OS uses XSAVE_XRSTORE and CPU support AVX
            אחרת
            {
                صحيح xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (صحيح)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                אם ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = שקר; // YMM registers
                אם ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = שקר; // ZMM registers
            }

            אם (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = שקר;
                have[CV_CPU_FP16] = שקר;
                have[CV_CPU_AVX2] = שקר;
                have[CV_CPU_FMA3] = שקר;
                have[CV_CPU_AVX_VNNI] = שקר;
            }
            אם (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = שקר;
                have[CV_CPU_AVX_512BW] = שקר;
                have[CV_CPU_AVX_512CD] = שקר;
                have[CV_CPU_AVX_512DQ] = שקר;
                have[CV_CPU_AVX_512ER] = שקר;
                have[CV_CPU_AVX_512IFMA] = שקר;
                have[CV_CPU_AVX_512PF] = שקר;
                have[CV_CPU_AVX_512VBMI] = שקר;
                have[CV_CPU_AVX_512VL] = שקר;
                have[CV_CPU_AVX_512VBMI2] = שקר;
                have[CV_CPU_AVX_512VNNI] = שקר;
                have[CV_CPU_AVX_512BITALG] = שקר;
                have[CV_CPU_AVX_512VPOPCNTDQ] = שקר;
                have[CV_CPU_AVX_5124VNNIW] = שקר;
                have[CV_CPU_AVX_5124FMAPS] = שקר;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            אם (have[CV_CPU_AVX512_COMMON])
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
            אחרת
            {
                have[CV_CPU_AVX512_KNL] = שקר;
                have[CV_CPU_AVX512_KNM] = שקר;
                have[CV_CPU_AVX512_SKX] = שקר;
                have[CV_CPU_AVX512_CNL] = שקר;
                have[CV_CPU_AVX512_CLX] = שקר;
                have[CV_CPU_AVX512_ICL] = שקר;
            }
        }
    #endif // CV_CPUID_X86

    #אם defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = אמת;
        have[CV_CPU_FP16] = אמת;
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        אם (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t size_auxv_t = حجم(auxv);

            כלעוד ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                אם (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#אם defined(AT_HWCAP2)
                אחרת אם (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #وإلاإذا defined __arm__ && defined __ANDROID__
      #אם defined HAVE_CPUFEATURES
        CV_LOG_INFO(عدم, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(عدم, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #אחרת
        CV_LOG_INFO(عدم, "cpufeatures library is not available for CPU detection");
        #אם CV_NEON
        CV_LOG_INFO(عدم, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = אמת;
        #אחרת
        CV_LOG_INFO(عدم, "- NEON instructions is NOT enabled via build flags");
        #endif
        #אם CV_FP16
        CV_LOG_INFO(عدم, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = אמת;
        #אחרת
        CV_LOG_INFO(عدم, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #وإلاإذا defined __arm__
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        אם (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t size_auxv_t = حجم(auxv);

            כלעוד ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                אם (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    اكسر;
                }
            }

            close(cpufile);
        }
    #endif
    #وإلاإذا (defined __APPLE__)
    #אם defined __ARM_NEON
        have[CV_CPU_NEON] = אמת;
    #endif
    #אם (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = אמת;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحيح has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = حجم(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, عدم, 0);
    אם (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = אמת;
    }
    صحيح has_feat_bf16 = 0;
    size_t has_feat_bf16_size = حجم(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, عدم, 0);
    אם (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = אמת;
    }
    #وإلاإذا (defined __clang__)
    #אם defined __ARM_NEON
        have[CV_CPU_NEON] = אמת;
        #אם (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = אמת;
        #endif
    #endif
    #endif
    #אם defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = אמת;
    #endif
    #אם defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = אמת;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = אמת;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = אמת;
    #endif

    #אם (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned صحيح hwcap = getauxval(AT_HWCAP);
        אם (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            אם (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = אמת;
            } אחרת {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلاإذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        אם (elf_aux_info(AT_HWCAP, &hwcap, حجم(hwcap)) != 0)
            hwcap = 0;
        אם (hwcap & PPC_FEATURE_HAS_VSX) {
            אם (elf_aux_info(AT_HWCAP2, &hwcap2, حجم(hwcap2)) != 0)
                hwcap2 = 0;
            אם (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = אמת;
            } אחרת {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #אחרת
        // TODO: AIX
        #אם CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = אמת;
        #endif
        #אם CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = אמת;
        #endif
    #endif

    #אם defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = אמת;
        #אם (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = אמת;
        #endif
        #אם defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = אמת;
        #endif
    #endif

    #אם defined __loongarch64 && defined __linux__
        صحيح flag = (صحيح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        منطقي skip_baseline_check = שקר;
        אם (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = אמת;
        }
        صحيح baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        אם (!checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]))
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
            checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]), אמת);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]));
    }

    منطقي checkFeatures(ثابت صحيح* features, صحيح count, منطقي dump = שקר)
    {
        منطقي result = אמת;
        עבור (صحيح i = 0; i < count; i++)
        {
            صحيح feature = features[i];
            אם (feature)
            {
                אם (have[feature])
                {
                    אם (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                אחרת
                {
                    result = שקר;
                    אם (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        החזר result;
    }

    ساكن مضمن منطقي isSymbolSeparator(حرف c)
    {
        החזר c == ',' || c == ';';
    }

    فراغ readSettings(ثابت صحيح* baseline_features, صحيح baseline_count)
    {
        منطقي dump = אמת;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        אם (!disabled_features.empty())
        {
            ثابت حرف* start = disabled_features.c_str();
            עבור (;;)
            {
                כלעוד (start[0] != 0 && isSymbolSeparator(start[0]))
                {
                    start++;
                }
                אם (start[0] == 0)
                    اكسر;
                ثابت حرف* end = start;
                כלעוד (end[0] != 0 && !isSymbolSeparator(end[0]))
                {
                    end++;
                }
                אם (end == start)
                    استمر;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                منطقي found = שקר;
                עבור (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    אם (!g_hwFeatureNames[i]) استمر;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    אם (len != feature.size()) استمر;
                    אם (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        منطقي isBaseline = שקר;
                        עבור (صحيح k = 0; k < baseline_count; k++)
                        {
                            אם (baseline_features[k] == i)
                            {
                                isBaseline = אמת;
                                اكسر;
                            }
                        }
                        אם (isBaseline)
                        {
                            אם (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        אם (!have[i])
                        {
                            אם (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = שקר;

                        found = אמת;
                        اكسر;
                    }
                }
                אם (!found)
                {
                    אם (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقي have[MAX_FEATURE+1]{};
};

ساكن HWFeatures  featuresEnabled(אמת), featuresDisabled = HWFeatures(שקר);
ساكن HWFeatures* currentFeatures = &featuresEnabled;

منطقي checkHardwareSupport(صحيح feature)
{
    אם (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        החזר שקר;
    החזר currentFeatures->have[feature];
}

String getHardwareFeatureName(صحيح feature)
{
    ثابت حرف* name = getHWFeatureName(feature);
    החזר name ? String(name) : String();
}

std::string getCPUFeaturesLine()
{
    ثابت صحيح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحيح sz = حجم(features) / حجم(features[0]);
    std::string result;
    std::string prefix;
    עבור (صحيح i = 1; i < sz; ++i)
    {
        אם (features[i] == 0)
        {
            prefix = "*";
            استمر;
        }
        אם (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        אם (!checkHardwareSupport(features[i])) result.append("?");
    }
    החזר result;
}

متطاير منطقي useOptimizedFlag = אמת;

فراغ setUseOptimized( منطقي flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

منطقي useOptimized(فراغ)
{
    החזר useOptimizedFlag;
}

int64 getTickCount(فراغ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    החזר (int64)now.time_since_epoch().count();
}

مزدوج getTickFrequency(فراغ)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    مزدوج clock_freq = clock_period_t::den / clock_period_t::num;
    החזר clock_freq;
}

#אם defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#אם defined(__i386__)

int64 getCPUTickCount(فراغ)
{
    int64 x;
    __asm__ متطاير (".byte 0x0f, 0x31" : "=A" (x));
    החזר x;
}
#وإلاإذا defined(__x86_64__)

int64 getCPUTickCount(فراغ)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    החזר (int64)lo | ((int64)hi << 32);
}

#وإلاإذا defined(__ppc__)

int64 getCPUTickCount(فراغ)
{
    unsigned upper, lower, tmp;
    __asm__ متطاير(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    החזר lower | ((int64)upper << 32);
}

#אחרת

#error "RDTSC not defined"

#endif

#وإلاإذا defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 getCPUTickCount(فراغ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#אחרת

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 getCPUTickCount(فراغ)
{
    החזר getTickCount();
}
//#endif

#endif


نطاق internal {

מחלקה Timestamp
{
عام:
    ثابت int64 zeroTickCount;
    ثابت مزدوج ns_in_ticks;

    Timestamp()
        : zeroTickCount(getTickCount())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = getTickCount();
        החזר (int64)((t - zeroTickCount) * ns_in_ticks);
    }

    ساكن Timestamp& getInstance()
    {
        ساكن Timestamp g_timestamp;
        החזר g_timestamp;
    }
};

מחלקה InitTimestamp {
عام:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ساكن InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    החזר internal::Timestamp::getInstance().getTimestamp();
}


ثابت String& getBuildInformation()
{
    ساكن String build_info =
#ضمن "version_string.inc"
    ;
    החזר build_info;
}

String getVersionString() { החזר String(CV_VERSION); }

صحيح getVersionMajor() { החזר CV_VERSION_MAJOR; }

صحيح getVersionMinor() { החזר CV_VERSION_MINOR; }

صحيح getVersionRevision() { החזר CV_VERSION_REVISION; }

String format( ثابت حرف* fmt, ... )
{
    AutoBuffer<حرف, 1024> buf;

    עבור ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحيح bsize = static_cast<صحيح>(buf.size());
        صحيح len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        אם (len >= bsize)
        {
            buf.resize(len + 1);
            استمر;
        }
        buf[bsize - 1] = 0;
        החזר String(buf.data(), len);
    }
}

String tempfile( ثابت حرف* suffix )
{
#אם OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#אם defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    אם (temp_file.empty())
        החזר String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    حرف aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#وإلاإذا defined(_WIN32_WCE)
    ثابت auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    אם(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        حرف aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#אחרת
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    אם (temp_dir.empty())
    {
        ::GetTempPathW(حجم(temp_dir2_w)/حجم(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحيح len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, عدم, 0, عدم, عدم);
        אם (len > 0)
        {
            std::vector<حرف> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, عدم, عدم);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    אם (FAILED(hr))
        החזר String();
    حرف guidStr[40];
    ثابت حرف* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, حجم(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned صحيح)g.Data4[0], (unsigned صحيح)g.Data4[1],
            (unsigned صحيح)g.Data4[2], (unsigned صحيح)g.Data4[3], (unsigned صحيح)g.Data4[4],
            (unsigned صحيح)g.Data4[5], (unsigned صحيح)g.Data4[6], (unsigned صحيح)g.Data4[7]);

    fname = temp_dir;
    אם (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# אחרת
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    حرف defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  אחרת
    حرف defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    אם (temp_dir.empty())
        fname = defaultTemplate;
    אחרת
    {
        fname = temp_dir;
        حرف ech = fname[fname.size() - 1];
        אם(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ثابت صحيح fd = mkstemp((حرف*)fname.c_str());
    אם (fd == -1) החזר String();

    close(fd);
    remove(fname.c_str());
# endif

    אם (suffix)
    {
        אם (suffix[0] != '.')
            החזר fname + "." + suffix;
        אחרת
            החזר fname + suffix;
    }
    החזר fname;
#אחרת // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ساكن ErrorCallback customErrorCallback = 0;
ساكن فراغ* customErrorCallbackData = 0;
ساكن منطقي breakOnError = שקר;

منطقي setBreakOnError(منطقي value)
{
    منطقي prevVal = breakOnError;
    breakOnError = value;
    החזר prevVal;
}

صحيح cv_snprintf(حرف* buf, صحيح len, ثابت حرف* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    صحيح res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    החזר res;
}

صحيح cv_vsnprintf(حرف* buf, صحيح len, ثابت حرف* fmt, va_list args)
{
#אם defined _MSC_VER
    אם (len <= 0) החזר len == 0 ? 1024 : -1;
    صحيح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    אם (res >= 0 && res < len)
    {
        buf[res] = 0;
        החזר res;
    }
    אחרת
    {
        buf[len - 1] = 0; // truncate happened
        החזר res >= len ? res : (len * 2);
    }
#אחרת
    החזר vsnprintf(buf, len, fmt, args);
#endif
}

ساكن فراغ dumpException(ثابت Exception& exc)
{
    ثابت حرف* errorStr = exc.codeMessage();
    حرف buf[1 << 12];

    cv_snprintf(buf, حجم(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.line);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#אחרת
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
ساكن منطقي cv_terminate_handler_installed = שקר;
ساكن std::terminate_handler cv_old_terminate_handler;
ساكن cv::Exception cv_terminate_handler_exception;
ساكن منطقي param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", אמת);
ساكن فراغ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    אם (שקר /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# אם defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

فراغ error( ثابت Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        אם (!cv_terminate_handler_installed)
        {
            אם (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = אמת;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    אם (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.line, customErrorCallbackData);
    אחרת אם (param_dumpErrors)
    {
        dumpException(exc);
    }

    אם(breakOnError)
    {
        std::terminate();
    }

    ارم exc;
#ifdef __GNUC__
# אם !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

فراغ error(Error::Code _code, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح _line)
{
    error(cv::Exception(_code, _err, _func, _file, _line));
#ifdef __GNUC__
# אם !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# אם defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, فراغ* userdata, فراغ** prevUserdata)
{
    אם( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    החזר prevCallback;
}

فراغ terminate(Error::Code _code, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح _line) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, _line));
    std::terminate();
}

}

نطاق cv {
منطقي __termination = שקר;


//////////////////////////////// thread-local storage ////////////////////////////////

نطاق details {

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
מחלקה TlsAbstraction
{
عام:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    فراغ* getData() ثابت;
    فراغ setData(فراغ *pData);

    فراغ releaseSystemResources();

خاص:

#ifdef _WIN32
#ifndef WINRT
    DWORD tlsKey;
    منطقي disposed;
#endif
#אחרת // _WIN32
    pthread_key_t  tlsKey;
    std::atomic<منطقي> disposed;
#endif
};

מחלקה TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
عام:
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
ساكن TlsAbstraction* getTlsAbstraction()
{
    ساكن TlsAbstraction *g_tls = جديد TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ساكن TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    החזר g_tls;
}


#ifdef _WIN32
#ifdef WINRT
ساكن __declspec( thread ) فراغ* tlsData = عدم; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = אמת;  // DllMain is missing in static builds
}
فراغ* TlsAbstraction::getData() ثابت
{
    החזר tlsData;
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    tlsData = pData;
}
#אחרת //WINRT
#ifdef CV_USE_FLS
ساكن فراغ NTAPI opencv_fls_destructor(فراغ* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(שקר)
{
#ifndef CV_USE_FLS
    tlsKey = TlsAlloc();
#אחרת // CV_USE_FLS
    tlsKey = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tlsKey != TLS_OUT_OF_INDEXES);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = אמת;  // DllMain is missing in static builds
    disposed = אמת;
#ifndef CV_USE_FLS
    TlsFree(tlsKey);
#אחרת // CV_USE_FLS
    FlsFree(tlsKey);
#endif // CV_USE_FLS
    tlsKey = TLS_OUT_OF_INDEXES;
}
فراغ* TlsAbstraction::getData() ثابت
{
    אם (disposed)
        החזר عدم;
#ifndef CV_USE_FLS
    החזר TlsGetValue(tlsKey);
#אחרת // CV_USE_FLS
    החזר FlsGetValue(tlsKey);
#endif // CV_USE_FLS
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    אם (disposed)
        החזר;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(TlsSetValue(tlsKey, pData) == TRUE);
#אחרת // CV_USE_FLS
    CV_Assert(FlsSetValue(tlsKey, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#אחרת // _WIN32
ساكن فراغ opencv_tls_destructor(فراغ* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(שקר)
{
    CV_Assert(pthread_key_create(&tlsKey, opencv_tls_destructor) == 0);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = אמת;  // DllMain is missing in static builds
    disposed = אמת;
    אם (pthread_key_delete(tlsKey) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
فراغ* TlsAbstraction::getData() ثابت
{
    אם (disposed)
        החזר عدم;
    החזר pthread_getspecific(tlsKey);
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    אם (disposed)
        החזר;  // no-op
    CV_Assert(pthread_setspecific(tlsKey, pData) == 0);
}
#endif

// Per-thread data structure
بنية ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<فراغ*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ساكن منطقي g_isTlsStorageInitialized = שקר;

// Main TLS storage class
מחלקה TlsStorage
{
عام:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (فراغ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = אמת;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    فراغ releaseThread(فراغ* tlsValue = عدم)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        אם (عدم == tls)
            החזר;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tlsValue == عدم ? (ThreadData*)tls->getData() : (ThreadData*)tlsValue;
        אם (pTD == عدم)
            החזר;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        עבור (size_t i = 0; i < threads.size(); i++)
        {
            אם (pTD == threads[i])
            {
                threads[i] = عدم;
                אם (tlsValue == عدم)
                    tls->setData(0);
                std::vector<فراغ*>& thread_slots = pTD->slots;
                עבור (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    فراغ* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = عدم;
                    אם (!pData)
                        استمر;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    אם (container)
                        container->deleteDataInstance(pData);
                    אחרת
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحيح)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                החזר;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (فراغ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        עבור(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            אם (tlsSlots[slot].container == عدم)
            {
                tlsSlots[slot].container = container;
                החזר slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        החזר tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ releaseSlot(size_t slotIdx, std::vector<فراغ*> &dataVec, منطقي keepSlot = שקר)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        עבור(size_t i = 0; i < threads.size(); i++)
        {
            אם(threads[i])
            {
                std::vector<فراغ*>& thread_slots = threads[i]->slots;
                אם (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = عدم;
                }
            }
        }

        אם (!keepSlot)
        {
            tlsSlots[slotIdx].container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* getData(size_t slotIdx) ثابت
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        אם (عدم == tls)
            החזר عدم;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        אם(threadData && threadData->slots.size() > slotIdx)
            החזר threadData->slots[slotIdx];

        החזר عدم;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t slotIdx, std::vector<فراغ*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        עבור(size_t i = 0; i < threads.size(); i++)
        {
            אם(threads[i])
            {
                std::vector<فراغ*>& thread_slots = threads[i]->slots;
                אם (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    فراغ setData(size_t slotIdx, فراغ* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        אם (عدم == tls)
            החזר;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        אם(!threadData)
        {
            threadData = جديد ThreadData;
            tls->setData((فراغ*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                منطقي found = שקר;
                // Find unused slots
                עבור(size_t slot = 0; slot < threads.size(); slot++)
                {
                    אם (threads[slot] == عدم)
                    {
                        threadData->idx = (صحيح)slot;
                        threads[slot] = threadData;
                        found = אמת;
                        اكسر;
                    }
                }

                אם (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        אם(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, عدم);
        }
        threadData->slots[slotIdx] = pData;
    }

خاص:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    بنية TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<بنية TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ساكن TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, جديد TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
ساكن فراغ opencv_tls_destructor(فراغ* pData)
{
    אם (!g_isTlsStorageInitialized)
        החזר;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#אחרת // _WIN32
#ifdef CV_USE_FLS
ساكن فراغ WINAPI opencv_fls_destructor(فراغ* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    אם (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = אמת;
    }

    אם (!g_isTlsStorageInitialized)
        החזר;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

ساكن TlsStorage* ثابت g_force_initialization_of_TlsStorage
#אם defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#אחרת  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
מחלקה TlsStorage
{
عام:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        עבור (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            אם (container && s.data)
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
        עבור (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            אם (s.container == عدم)
            {
                CV_Assert(!s.data);
                s.container = container;
                החזר slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        החזר slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ releaseSlot(size_t slotIdx, std::vector<فراغ*> &dataVec, منطقي keepSlot = שקר)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        فراغ* data = s.data;
        אם (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        אם (!keepSlot)
        {
            s.container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* getData(size_t slotIdx) ثابت
    {
        CV_Assert(slotIdx < slots.size());
        ثابت SlotInfo& s = slots[slotIdx];
        החזר s.data;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t slotIdx, std::vector<فراغ*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        فراغ* data = s.data;
        אם (data)
            dataVec.push_back(data);
        החזר;
    }

    // Set data to storage index
    فراغ setData(size_t slotIdx, فراغ* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

خاص:
    بنية SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        فراغ* data;
    };
    std::vector<بنية SlotInfo> slots;
};

ساكن TlsStorage& getTlsStorage()
{
    ساكن TlsStorage g_storage;  // no threading
    החזר g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using نطاق details;

فراغ releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    אם (!g_isTlsStorageInitialized)
        החזר;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    key_ = (صحيح)getTlsStorage().reserveSlot(هذا); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(key_ == -1); // Key must be released in child object
}

فراغ TLSDataContainer::gatherData(std::vector<فراغ*> &data) ثابت
{
    getTlsStorage().gather(key_, data);
}

فراغ TLSDataContainer::detachData(std::vector<فراغ*> &data)
{
    getTlsStorage().releaseSlot(key_, data, אמת);
}

فراغ TLSDataContainer::release()
{
    אם (key_ == -1)
        החזר;  // already released
    std::vector<فراغ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, שקר); // Release key and get stored data for proper destruction
    key_ = -1;
    עבור(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

فراغ TLSDataContainer::cleanup()
{
    std::vector<فراغ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, אמת); // Extract stored data with removal from TLS tables
    עבור(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

فراغ* TLSDataContainer::getData() ثابت
{
    CV_Assert(key_ != -1 && "Can't fetch data from terminated TLS container.");
    فراغ* pData = getTlsStorage().getData(key_); // Check if data was already allocated
    אם(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        حاول
        {
            getTlsStorage().setData(key_, pData);
        }
        امسك (...)
        {
            deleteDataInstance(pData);
            ارم;
        }
    }
    החזר pData;
}

ساكن TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, جديد TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    החזר getCoreTlsDataTLS().getRef();
}

#אם defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    אם (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        אם (lpReserved != عدم) // called after ExitProcess() call
        {
            cv::__termination = אמת;
        }
        אחרת
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    החזר TRUE;
}
#endif


نطاق {

#ifdef OPENCV_WITH_ITT
منطقي overrideThreadName()
{
    ساكن منطقي param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", שקר);
    החזר param;
}
#endif

ساكن صحيح g_threadNum = 0;
מחלקה ThreadID {
عام:
    ثابت صحيح id;
    ThreadID() :
        id(CV_XADD(&g_threadNum, 1))
    {
#ifdef OPENCV_WITH_ITT
        אם (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

ساكن TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, جديد TLSData<ThreadID>());
}

} // namespace
صحيح utils::getThreadID() { החזר getThreadIDTLS().get()->id; }


מחלקה ParseError
{
    std::string bad_value;
عام:
    ParseError(ثابت std::string &bad_value_) :bad_value(bad_value_) {}
    std::string toString(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_value;
        החזר out.str();
    }
};

template <typename T>
T parseOption(ثابت std::string &);

template<>
مضمن منطقي parseOption(ثابت std::string & value)
{
    אם (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        החזר אמת;
    }
    אם (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        החזר שקר;
    }
    ارم ParseError(value);
}

template<>
مضمن size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    עבור (; pos < value.size(); pos++)
    {
        אם (!isdigit(value[pos]))
            اكسر;
    }
    cv::String valueStr = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(valueStr);
    אם (suffixStr.length() == 0)
        החזר v;
    אחרת אם (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        החזר v * 1024 * 1024;
    אחרת אם (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        החזר v * 1024;
    ارم ParseError(value);
}

template<>
مضمن cv::String parseOption(ثابت std::string &value)
{
    החזר value;
}

template<>
مضمن utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ثابت حرف sep = ';';
#אחרת
    ثابت حرف sep = ':';
#endif
    size_t start_pos = 0;
    כלעוד (start_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, start_pos);
        ثابت std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        אם (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    החזר result;
}

ساكن مضمن ثابت حرف * envRead(ثابت حرف * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    החזר عدم;
#אחרת
    החזר getenv(name);
#endif
}

template<typename T>
مضمن T اقرأ(ثابت std::string & k, ثابت T & defaultValue)
{
    حاول
    {
        ثابت حرف * res = envRead(k.c_str());
        אם (res)
            החזר parseOption<T>(std::string(res));
    }
    امسك (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    החזר defaultValue;
}

منطقي utils::getConfigurationParameterBool(ثابت حرف* name, منطقي defaultValue)
{
    החזר اقرأ<منطقي>(name, defaultValue);
}

size_t utils::getConfigurationParameterSizeT(ثابت حرف* name, size_t defaultValue)
{
    החזר اقرأ<size_t>(name, defaultValue);
}

std::string utils::getConfigurationParameterString(ثابت حرف* name, ثابت std::string & defaultValue)
{
    החזר اقرأ<cv::String>(name, defaultValue);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت حرف* name, ثابت utils::Paths &defaultValue)
{
    החזר اقرأ<utils::Paths>(name, defaultValue);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, جديد ImplCollector())
}

فراغ setImpl(صحيح flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

فراغ addImpl(صحيح flag, ثابت حرف* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    אם(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        אם(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

صحيح getImpl(std::vector<صحيح> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    החזר getImplData().implFlags; // return actual flags for lazy collection
}

منطقي useCollection()
{
    החזר getImplData().useCollection;
}

فراغ setUseCollection(منطقي flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

نطاق instr
{
منطقي useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    החזר getInstrumentStruct().useInstr;
#אחרת
    החזר שקר;
#endif
}

فراغ setUseInstrumentation(منطقي flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#אחרת
    CV_UNUSED(flag);
#endif
}

InstrNode* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    החזר &getInstrumentStruct().rootNode;
#אחרת
    החזר عدم;
#endif
}

فراغ resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().rootNode.removeChilds();
    getInstrumentTLSStruct().pCurrentNode = &getInstrumentStruct().rootNode;
#endif
}

فراغ setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#אחרת
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    החזר (FLAGS)getInstrumentStruct().flags;
#אחרת
    החזר (FLAGS)0;
#endif
}

NodeData::NodeData(ثابت حرف* funName, ثابت حرف* fileName, صحيح lineNum, فراغ* retAddress, منطقي alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
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

    m_funError  = שקר;
}
NodeData::NodeData(NodeData &ref)
{
    *هذا = ref;
}
NodeData& NodeData::operator=(ثابت NodeData &right)
{
    هذا->m_funName      = right.m_funName;
    هذا->m_instrType    = right.m_instrType;
    هذا->m_implType     = right.m_implType;
    هذا->m_fileName     = right.m_fileName;
    هذا->m_lineNum      = right.m_lineNum;
    هذا->m_retAddress   = right.m_retAddress;
    هذا->m_alwaysExpand = right.m_alwaysExpand;

    هذا->m_threads     = right.m_threads;
    هذا->m_counter     = right.m_counter;
    هذا->m_ticksTotal  = right.m_ticksTotal;

    هذا->m_funError    = right.m_funError;

    החזר *هذا;
}
NodeData::~NodeData()
{
}
منطقي operator==(ثابت NodeData& left, ثابت NodeData& right)
{
    אם(left.m_lineNum == right.m_lineNum && left.m_funName == right.m_funName && left.m_fileName == right.m_fileName)
    {
        אם(left.m_retAddress == right.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || left.m_alwaysExpand))
            החזר אמת;
    }
    החזר שקר;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ساكن InstrStruct instr;
    החזר instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    החזר *getInstrumentStruct().tlsStruct.get();
}

InstrNode* getCurrentNode()
{
    החזר getInstrumentTLSStruct().pCurrentNode;
}

IntrumentationRegion::IntrumentationRegion(ثابت حرف* funName, ثابت حرف* fileName, صحيح lineNum, فراغ *retAddress, منطقي alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = שקר;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    אם(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        אם(!pTLS->pCurrentNode)
        {
            m_disabled = אמת;
            החזר;
        }

        صحيح depth = pTLS->pCurrentNode->getDepth();
        אם(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = אמת;
            החזר;
        }

        NodeData payload(funName, fileName, lineNum, retAddress, alwaysExpand, instrType, implType);
        Node<NodeData>* pChild = عدم;

        אם(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->pCurrentNode->findChild(payload);
            אם(!pChild)
            {
                pChild = جديد Node<NodeData>(payload);
                pTLS->pCurrentNode->addChild(pChild);
            }
        }
        אחרת
        {
            pChild = pTLS->pCurrentNode->findChild(payload);
            אם(!pChild)
            {
                m_disabled = אמת;
                החזר;
            }
        }
        pTLS->pCurrentNode = pChild;

        m_regionTicks = getTickCount();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    אם(pStruct->useInstr)
    {
        אם(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            אם (pTLS->pCurrentNode->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
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

نطاق ipp
{

#ifdef HAVE_IPP
بنية IPPInitSingleton
{
عام:
    IPPInitSingleton()
    {
        useIPP         = אמת;

#אם defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#אחרת
        useIPP_NE      = שקר;
#endif
        ippStatus      = 0;
        funcname       = عدم;
        filename       = عدم;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ippTopFeatures = 0;
        pIppLibInfo    = عدم;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, عدم);
        אם(ippStatus < 0)
        {
            CV_LOG_ERROR(عدم, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = שקר;
            החזר;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        אם(!env.empty())
        {
#אם IPP_VERSION_X100 >= 201900
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#وإلاإذا IPP_VERSION_X100 >= 201703
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وإلاإذا IPP_VERSION_X100 >= 201700
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#אחרת
            ثابت Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            אם(env.substr(0, 2) == "ne")
            {
                useIPP_NE = אמת;
                env = env.substr(3, env.size());
            }

            אם(env == "disabled")
            {
                CV_LOG_WARNING(عدم, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = שקר;
            }
            אחרת אם(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            אחרת אם(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#אם IPP_VERSION_X100 >= 201700
#אם defined (_M_AMD64) || defined (__x86_64__)
            אחרת אם(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            אחרת
                CV_LOG_ERROR(عدم, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        אם(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        אם(!(
#אם IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = שקר;
            החזר;
        }

        אם(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        אחרת
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#אם IPP_VERSION_X100 >= 201700
        אם(ippFeatures&ippCPUID_AVX512F)
        {
            אם((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ippTopFeatures = ippCPUID_AVX512_SKX;
            אחרת אם((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ippTopFeatures = ippCPUID_AVX512_KNL;
            אחרת
                ippTopFeatures = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        אחרת
#endif
        אם(ippFeatures&ippCPUID_AVX2)
            ippTopFeatures = ippCPUID_AVX2;
        אחרת אם(ippFeatures&ippCPUID_SSE42)
            ippTopFeatures = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        אם (ippName.find("SSE4.2") != std::string::npos)
        {
            ippTopFeatures = ippCPUID_SSE42;
        }
    }

عام:
    منطقي        useIPP;
    منطقي        useIPP_NE;

    صحيح         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ثابت حرف *funcname;
    ثابت حرف *filename;
    صحيح         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ippTopFeatures;
    ثابت IppLibraryVersion *pIppLibInfo;
};

ساكن IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, جديد IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    החזר getIPPSingleton().ippFeatures;
#אחרת
    החזר 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long getIppTopFeatures()
{
    החזר getIPPSingleton().ippTopFeatures;
}
#endif

فراغ setIppStatus(صحيح status, ثابت حرف * ثابت _funcname, ثابت حرف * ثابت _filename, صحيح _line)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = _line;
#אחרת
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(_line);
#endif
}

صحيح getIppStatus()
{
#ifdef HAVE_IPP
    החזר getIPPSingleton().ippStatus;
#אחרת
    החזר 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    החזר format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#אחרת
    החזר String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ثابت IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    אם(pInfo)
        החזר format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    אחרת
        החזר String("error");
#אחרת
    החזר String("disabled");
#endif
}

منطقي useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    אם (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    החזר (data.useIPP > 0);
#אחרת
    החזר שקר;
#endif
}

فراغ setUseIPP(منطقي flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:שקר;
#אחרת
    CV_UNUSED(flag);
    data.useIPP = שקר;
#endif
}

منطقي useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    אם (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    החזר (data.useIPP_NE > 0);
#אחרת
    החזר שקר;
#endif
}

فراغ setUseIPP_NotExact(منطقي flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#אחרת
    CV_UNUSED(flag);
    data.useIPP_NE = שקר;
#endif
}

} // namespace ipp


نطاق details {

#אם OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

فراغ setFPDenormalsIgnoreHint(منطقي ignore, CV_OUT FPDenormalsModeState& state)
{
#אם OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    אם (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ثابت unsigned old_flags = _mm_getcsr();
    ثابت unsigned old_value = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(عدم, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    _mm_setcsr(flags);
#אחרת
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

صحيح saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#אם OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    אם (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ثابت unsigned old_flags = _mm_getcsr();
    ثابت unsigned old_value = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    החזר 2;
#אחרת
    CV_UNUSED(state);
    החזר 0;
#endif
}

منطقي restoreFPDenormalsState(ثابت FPDenormalsModeState& state)
{
#אם OPENCV_IMPL_FP_HINTS_X86
    ثابت unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((صحيح)value, value == (value & mask), "invalid SSE FP state");
    ثابت unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(عدم, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    החזר אמת;
#אחרת
    CV_UNUSED(state);
    החזר שקר;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    החזר OPENCV_ALGO_HINT_DEFAULT;
#אחרת
    החזר ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
