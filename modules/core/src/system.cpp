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
    𐤀𐤌 (__initialization_mutex == عدم)
    {
        (فراغ)_initSystem();
        __initialization_mutex = جديد Mutex();
    }
    أرجع *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ساكن منطقي param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#𐤀𐤌 defined(_DEBUG) || defined(__ANDROID__)
    true
#وإلا
    خطأ
#endif
);

فراغ* allocSingletonBuffer(size_t size) { أرجع fastMalloc(size); }
فراغ* allocSingletonNewBuffer(size_t size) { أرجع malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# 𐤀𐤌 defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#𐤀𐤌 defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# 𐤀𐤌 _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#ضمن <cstdlib>        // std::abort
#endif

#𐤀𐤌 defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#𐤀𐤌 defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#وإلا
#  include <elf.h>
#endif
#𐤀𐤌 defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#𐤀𐤌 defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#𐤀𐤌 ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#𐤀𐤌 defined __loongarch64
#ضمن "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#𐤀𐤌 defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#ضمن <windows.h>
#ضمن <combaseapi.h>
#𐤀𐤌 (_WIN32_WINNT >= 0x0602)
  #ضمن <synchapi.h>
#endif
#𐤀𐤌 ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
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
    أرجع std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#وإلا
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    𐤀𐤌 (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        أرجع wstr;
    𐤀𐤌 (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        أرجع wstr;
    𐤀𐤌 (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        أرجع wstr;
    𐤀𐤌 (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        أرجع wstr;
    𐤀𐤌 (FAILED(storagefolderRef.As(&storageitemRef)))
        أرجع wstr;
    str = عدم;
    𐤀𐤌 (FAILED(storageitemRef->get_Path(&str)))
        أرجع wstr;
    wstr = WindowsGetStringRawBuffer(str, عدم);
    WindowsDeleteString(str);
    أرجع wstr;
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

    أرجع prefix.append(std::wstring(guidStr));
}

#endif
#وإلا
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#ضمن <pthread.h>
#endif
#ضمن <sys/time.h>
#ضمن <time.h>

#𐤀𐤌 defined __MACH__ && defined __APPLE__
#ضمن <mach/mach.h>
#ضمن <mach/mach_time.h>
#ضمن <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#ضمن "omp.h"
#endif

#𐤀𐤌 defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#𐤀𐤌 defined __ANDROID__
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
  #𐤀𐤌 defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #𐤀𐤌 _MSC_VER >= 1400  // MSVS 2005
      #ضمن <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #وإلا
      #error "Required MSVS 2005+"
    #endif
  #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ساكن فراغ cv_cpuid(صحيح* cpuid_data, صحيح reg_eax, صحيح reg_ecx)
    {
        صحيح __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#𐤀𐤌 !defined(__PIC__) \
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
#وإلا
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
ثابت حرف* Exception::what() ثابت CV_NOEXCEPT { أرجع msg.c_str(); }

فراغ Exception::formatMessage()
{
    size_t pos = err.find('\n');
    منطقي multiline = pos != cv::String::npos;
    𐤀𐤌 (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        طالما (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        𐤀𐤌 (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    𐤀𐤌 (func.size() > 0)
    {
        𐤀𐤌 (multiline)
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), line, code, codeMessage(), func.c_str(), err.c_str());
        وإلا
            msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), func.c_str());
    }
    وإلا
    {
        msg = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), line, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت حرف * Exception::codeMessage() ثابت
{
    بدل (code)
    {
    حالة cv::Error::StsOk :                  أرجع "No Error";
    حالة cv::Error::StsBackTrace :           أرجع "Backtrace";
    حالة cv::Error::StsError :               أرجع "Unspecified error";
    حالة cv::Error::StsInternal :            أرجع "Internal error";
    حالة cv::Error::StsNoMem :               أرجع "Insufficient memory";
    حالة cv::Error::StsBadArg :              أرجع "Bad argument";
    حالة cv::Error::StsNoConv :              أرجع "Iterations do not converge";
    حالة cv::Error::StsAutoTrace :           أرجع "Autotrace call";
    حالة cv::Error::StsBadSize :             أرجع "Incorrect size of input array";
    حالة cv::Error::StsNullPtr :             أرجع "Null pointer";
    حالة cv::Error::StsDivByZero :           أرجع "Division by zero occurred";
    حالة cv::Error::BadStep :                أرجع "Image step is wrong";
    حالة cv::Error::StsInplaceNotSupported : أرجع "Inplace operation is not supported";
    حالة cv::Error::StsObjectNotFound :      أرجع "Requested object was not found";
    حالة cv::Error::BadDepth :               أرجع "Input image depth is not supported by function";
    حالة cv::Error::StsUnmatchedFormats :    أرجع "Formats of input arguments do not match";
    حالة cv::Error::StsUnmatchedSizes :      أرجع "Sizes of input arguments do not match";
    حالة cv::Error::StsOutOfRange :          أرجع "One of the arguments\' values is out of range";
    حالة cv::Error::StsUnsupportedFormat :   أرجع "Unsupported format or combination of formats";
    حالة cv::Error::BadCOI :                 أرجع "Input COI is not supported";
    حالة cv::Error::BadNumChannels :         أرجع "Bad number of channels";
    حالة cv::Error::StsBadFlag :             أرجع "Bad flag (parameter or structure field)";
    حالة cv::Error::StsBadPoint :            أرجع "Bad parameter of type CvPoint";
    حالة cv::Error::StsBadMask :             أرجع "Bad type of mask argument";
    حالة cv::Error::StsParseError :          أرجع "Parsing error";
    حالة cv::Error::StsNotImplemented :      أرجع "The function/feature is not implemented";
    حالة cv::Error::StsBadMemBlock :         أرجع "Memory block has been corrupted";
    حالة cv::Error::StsAssert :              أرجع "Assertion failed";
    حالة cv::Error::GpuNotSupported :        أرجع "No CUDA support";
    حالة cv::Error::GpuApiCallError :        أرجع "Gpu API call";
    حالة cv::Error::OpenGlNotSupported :     أرجع "No OpenGL support";
    حالة cv::Error::OpenGlApiCallError :     أرجع "OpenGL API call";
    حالة cv::Error::OpenCLApiCallError :     أرجع "OpenCL API call error";
    حالة cv::Error::OpenCLDoubleNotSupported:أرجع "OpenCL device does not support double";
    حالة cv::Error::OpenCLInitError :        أرجع "OpenCL initialization error";
    حالة cv::Error::OpenCLNoAMDBlasFft :     أرجع "Missing OpenCL AMD BLAS FFT";
    افتراضي:
    {
        ساكن حرف buf[256] {0};
        snprintf(buf, حجم(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        أرجع buf;
    }
    };

}

ساكن ثابت حرف* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { عدم };

ساكن ثابت حرف* getHWFeatureName(صحيح id)
{
    أرجع (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : عدم;
}
ساكن ثابت حرف* getHWFeatureNameSafe(صحيح id)
{
    ثابت حرف* name = getHWFeatureName(id);
    أرجع name ? name : "Unknown feature";
}

بنية HWFeatures
{
    تعداد { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقي run_initialize = خطأ)
    {
        𐤀𐤌 (run_initialize)
            initialize();
    }

    ساكن فراغ initializeNames()
    {
        لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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
        𐤀𐤌 (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
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
        𐤀𐤌( x86_family >= 6 )
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

            منطقي have_AVX_OS_support = true;
            منطقي have_AVX512_OS_support = true;
            𐤀𐤌 (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = خطأ; // OS uses XSAVE_XRSTORE and CPU support AVX
            وإلا
            {
                صحيح xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (صحيح)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                𐤀𐤌 ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = خطأ; // YMM registers
                𐤀𐤌 ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = خطأ; // ZMM registers
            }

            𐤀𐤌 (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = خطأ;
                have[CV_CPU_FP16] = خطأ;
                have[CV_CPU_AVX2] = خطأ;
                have[CV_CPU_FMA3] = خطأ;
                have[CV_CPU_AVX_VNNI] = خطأ;
            }
            𐤀𐤌 (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = خطأ;
                have[CV_CPU_AVX_512BW] = خطأ;
                have[CV_CPU_AVX_512CD] = خطأ;
                have[CV_CPU_AVX_512DQ] = خطأ;
                have[CV_CPU_AVX_512ER] = خطأ;
                have[CV_CPU_AVX_512IFMA] = خطأ;
                have[CV_CPU_AVX_512PF] = خطأ;
                have[CV_CPU_AVX_512VBMI] = خطأ;
                have[CV_CPU_AVX_512VL] = خطأ;
                have[CV_CPU_AVX_512VBMI2] = خطأ;
                have[CV_CPU_AVX_512VNNI] = خطأ;
                have[CV_CPU_AVX_512BITALG] = خطأ;
                have[CV_CPU_AVX_512VPOPCNTDQ] = خطأ;
                have[CV_CPU_AVX_5124VNNIW] = خطأ;
                have[CV_CPU_AVX_5124FMAPS] = خطأ;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            𐤀𐤌 (have[CV_CPU_AVX512_COMMON])
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
            وإلا
            {
                have[CV_CPU_AVX512_KNL] = خطأ;
                have[CV_CPU_AVX512_KNM] = خطأ;
                have[CV_CPU_AVX512_SKX] = خطأ;
                have[CV_CPU_AVX512_CNL] = خطأ;
                have[CV_CPU_AVX512_CLX] = خطأ;
                have[CV_CPU_AVX512_ICL] = خطأ;
            }
        }
    #endif // CV_CPUID_X86

    #𐤀𐤌 defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = true;
        have[CV_CPU_FP16] = true;
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        𐤀𐤌 (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t size_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                𐤀𐤌 (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#𐤀𐤌 defined(AT_HWCAP2)
                وإلا 𐤀𐤌 (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #وإلاإذا defined __arm__ && defined __ANDROID__
      #𐤀𐤌 defined HAVE_CPUFEATURES
        CV_LOG_INFO(عدم, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(عدم, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #وإلا
        CV_LOG_INFO(عدم, "cpufeatures library is not available for CPU detection");
        #𐤀𐤌 CV_NEON
        CV_LOG_INFO(عدم, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = true;
        #وإلا
        CV_LOG_INFO(عدم, "- NEON instructions is NOT enabled via build flags");
        #endif
        #𐤀𐤌 CV_FP16
        CV_LOG_INFO(عدم, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = true;
        #وإلا
        CV_LOG_INFO(عدم, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #وإلاإذا defined __arm__
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        𐤀𐤌 (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t size_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                𐤀𐤌 (auxv.a_type == AT_HWCAP)
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
    #𐤀𐤌 defined __ARM_NEON
        have[CV_CPU_NEON] = true;
    #endif
    #𐤀𐤌 (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = true;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحيح has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = حجم(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, عدم, 0);
    𐤀𐤌 (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = true;
    }
    صحيح has_feat_bf16 = 0;
    size_t has_feat_bf16_size = حجم(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, عدم, 0);
    𐤀𐤌 (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = true;
    }
    #وإلاإذا (defined __clang__)
    #𐤀𐤌 defined __ARM_NEON
        have[CV_CPU_NEON] = true;
        #𐤀𐤌 (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = true;
        #endif
    #endif
    #endif
    #𐤀𐤌 defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = true;
    #endif
    #𐤀𐤌 defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = true;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = true;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = true;
    #endif

    #𐤀𐤌 (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned صحيح hwcap = getauxval(AT_HWCAP);
        𐤀𐤌 (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            𐤀𐤌 (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلاإذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        𐤀𐤌 (elf_aux_info(AT_HWCAP, &hwcap, حجم(hwcap)) != 0)
            hwcap = 0;
        𐤀𐤌 (hwcap & PPC_FEATURE_HAS_VSX) {
            𐤀𐤌 (elf_aux_info(AT_HWCAP2, &hwcap2, حجم(hwcap2)) != 0)
                hwcap2 = 0;
            𐤀𐤌 (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلا
        // TODO: AIX
        #𐤀𐤌 CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = true;
        #endif
        #𐤀𐤌 CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = true;
        #endif
    #endif

    #𐤀𐤌 defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = true;
        #𐤀𐤌 (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = true;
        #endif
        #𐤀𐤌 defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = true;
        #endif
    #endif

    #𐤀𐤌 defined __loongarch64 && defined __linux__
        صحيح flag = (صحيح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        منطقي skip_baseline_check = خطأ;
        𐤀𐤌 (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = true;
        }
        صحيح baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        𐤀𐤌 (!checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]))
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
            checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]), true);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]));
    }

    منطقي checkFeatures(ثابت صحيح* features, صحيح count, منطقي dump = خطأ)
    {
        منطقي result = true;
        لكل (صحيح i = 0; i < count; i++)
        {
            صحيح feature = features[i];
            𐤀𐤌 (feature)
            {
                𐤀𐤌 (have[feature])
                {
                    𐤀𐤌 (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                وإلا
                {
                    result = خطأ;
                    𐤀𐤌 (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        أرجع result;
    }

    ساكن مضمن منطقي isSymbolSeparator(حرف c)
    {
        أرجع c == ',' || c == ';';
    }

    فراغ readSettings(ثابت صحيح* baseline_features, صحيح baseline_count)
    {
        منطقي dump = true;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        𐤀𐤌 (!disabled_features.empty())
        {
            ثابت حرف* start = disabled_features.c_str();
            لكل (;;)
            {
                طالما (start[0] != 0 && isSymbolSeparator(start[0]))
                {
                    start++;
                }
                𐤀𐤌 (start[0] == 0)
                    اكسر;
                ثابت حرف* end = start;
                طالما (end[0] != 0 && !isSymbolSeparator(end[0]))
                {
                    end++;
                }
                𐤀𐤌 (end == start)
                    استمر;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                منطقي found = خطأ;
                لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    𐤀𐤌 (!g_hwFeatureNames[i]) استمر;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    𐤀𐤌 (len != feature.size()) استمر;
                    𐤀𐤌 (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        منطقي isBaseline = خطأ;
                        لكل (صحيح k = 0; k < baseline_count; k++)
                        {
                            𐤀𐤌 (baseline_features[k] == i)
                            {
                                isBaseline = true;
                                اكسر;
                            }
                        }
                        𐤀𐤌 (isBaseline)
                        {
                            𐤀𐤌 (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        𐤀𐤌 (!have[i])
                        {
                            𐤀𐤌 (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = خطأ;

                        found = true;
                        اكسر;
                    }
                }
                𐤀𐤌 (!found)
                {
                    𐤀𐤌 (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقي have[MAX_FEATURE+1]{};
};

ساكن HWFeatures  featuresEnabled(true), featuresDisabled = HWFeatures(خطأ);
ساكن HWFeatures* currentFeatures = &featuresEnabled;

منطقي checkHardwareSupport(صحيح feature)
{
    𐤀𐤌 (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        أرجع خطأ;
    أرجع currentFeatures->have[feature];
}

String getHardwareFeatureName(صحيح feature)
{
    ثابت حرف* name = getHWFeatureName(feature);
    أرجع name ? String(name) : String();
}

std::string getCPUFeaturesLine()
{
    ثابت صحيح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحيح sz = حجم(features) / حجم(features[0]);
    std::string result;
    std::string prefix;
    لكل (صحيح i = 1; i < sz; ++i)
    {
        𐤀𐤌 (features[i] == 0)
        {
            prefix = "*";
            استمر;
        }
        𐤀𐤌 (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        𐤀𐤌 (!checkHardwareSupport(features[i])) result.append("?");
    }
    أرجع result;
}

متطاير منطقي useOptimizedFlag = true;

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
    أرجع useOptimizedFlag;
}

int64 getTickCount(فراغ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    أرجع (int64)now.time_since_epoch().count();
}

مزدوج getTickFrequency(فراغ)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    مزدوج clock_freq = clock_period_t::den / clock_period_t::num;
    أرجع clock_freq;
}

#𐤀𐤌 defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#𐤀𐤌 defined(__i386__)

int64 getCPUTickCount(فراغ)
{
    int64 x;
    __asm__ متطاير (".byte 0x0f, 0x31" : "=A" (x));
    أرجع x;
}
#وإلاإذا defined(__x86_64__)

int64 getCPUTickCount(فراغ)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    أرجع (int64)lo | ((int64)hi << 32);
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
    أرجع lower | ((int64)upper << 32);
}

#وإلا

#error "RDTSC not defined"

#endif

#وإلاإذا defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 getCPUTickCount(فراغ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#وإلا

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 getCPUTickCount(فراغ)
{
    أرجع getTickCount();
}
//#endif

#endif


نطاق internal {

صنف Timestamp
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
        أرجع (int64)((t - zeroTickCount) * ns_in_ticks);
    }

    ساكن Timestamp& getInstance()
    {
        ساكن Timestamp g_timestamp;
        أرجع g_timestamp;
    }
};

صنف InitTimestamp {
عام:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ساكن InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    أرجع internal::Timestamp::getInstance().getTimestamp();
}


ثابت String& getBuildInformation()
{
    ساكن String build_info =
#ضمن "version_string.inc"
    ;
    أرجع build_info;
}

String getVersionString() { أرجع String(CV_VERSION); }

صحيح getVersionMajor() { أرجع CV_VERSION_MAJOR; }

صحيح getVersionMinor() { أرجع CV_VERSION_MINOR; }

صحيح getVersionRevision() { أرجع CV_VERSION_REVISION; }

String format( ثابت حرف* fmt, ... )
{
    AutoBuffer<حرف, 1024> buf;

    لكل ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحيح bsize = static_cast<صحيح>(buf.size());
        صحيح len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        𐤀𐤌 (len >= bsize)
        {
            buf.resize(len + 1);
            استمر;
        }
        buf[bsize - 1] = 0;
        أرجع String(buf.data(), len);
    }
}

String tempfile( ثابت حرف* suffix )
{
#𐤀𐤌 OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#𐤀𐤌 defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    𐤀𐤌 (temp_file.empty())
        أرجع String();

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

    𐤀𐤌(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        حرف aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#وإلا
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    𐤀𐤌 (temp_dir.empty())
    {
        ::GetTempPathW(حجم(temp_dir2_w)/حجم(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحيح len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, عدم, 0, عدم, عدم);
        𐤀𐤌 (len > 0)
        {
            std::vector<حرف> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, عدم, عدم);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    𐤀𐤌 (FAILED(hr))
        أرجع String();
    حرف guidStr[40];
    ثابت حرف* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, حجم(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned صحيح)g.Data4[0], (unsigned صحيح)g.Data4[1],
            (unsigned صحيح)g.Data4[2], (unsigned صحيح)g.Data4[3], (unsigned صحيح)g.Data4[4],
            (unsigned صحيح)g.Data4[5], (unsigned صحيح)g.Data4[6], (unsigned صحيح)g.Data4[7]);

    fname = temp_dir;
    𐤀𐤌 (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# وإلا
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    حرف defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  وإلا
    حرف defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    𐤀𐤌 (temp_dir.empty())
        fname = defaultTemplate;
    وإلا
    {
        fname = temp_dir;
        حرف ech = fname[fname.size() - 1];
        𐤀𐤌(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ثابت صحيح fd = mkstemp((حرف*)fname.c_str());
    𐤀𐤌 (fd == -1) أرجع String();

    close(fd);
    remove(fname.c_str());
# endif

    𐤀𐤌 (suffix)
    {
        𐤀𐤌 (suffix[0] != '.')
            أرجع fname + "." + suffix;
        وإلا
            أرجع fname + suffix;
    }
    أرجع fname;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ساكن ErrorCallback customErrorCallback = 0;
ساكن فراغ* customErrorCallbackData = 0;
ساكن منطقي breakOnError = خطأ;

منطقي setBreakOnError(منطقي value)
{
    منطقي prevVal = breakOnError;
    breakOnError = value;
    أرجع prevVal;
}

صحيح cv_snprintf(حرف* buf, صحيح len, ثابت حرف* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    صحيح res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    أرجع res;
}

صحيح cv_vsnprintf(حرف* buf, صحيح len, ثابت حرف* fmt, va_list args)
{
#𐤀𐤌 defined _MSC_VER
    𐤀𐤌 (len <= 0) أرجع len == 0 ? 1024 : -1;
    صحيح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    𐤀𐤌 (res >= 0 && res < len)
    {
        buf[res] = 0;
        أرجع res;
    }
    وإلا
    {
        buf[len - 1] = 0; // truncate happened
        أرجع res >= len ? res : (len * 2);
    }
#وإلا
    أرجع vsnprintf(buf, len, fmt, args);
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
#وإلا
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
ساكن منطقي cv_terminate_handler_installed = خطأ;
ساكن std::terminate_handler cv_old_terminate_handler;
ساكن cv::Exception cv_terminate_handler_exception;
ساكن منطقي param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", true);
ساكن فراغ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    𐤀𐤌 (خطأ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# 𐤀𐤌 defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

فراغ error( ثابت Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        𐤀𐤌 (!cv_terminate_handler_installed)
        {
            𐤀𐤌 (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = true;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    𐤀𐤌 (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.line, customErrorCallbackData);
    وإلا 𐤀𐤌 (param_dumpErrors)
    {
        dumpException(exc);
    }

    𐤀𐤌(breakOnError)
    {
        std::terminate();
    }

    ارم exc;
#ifdef __GNUC__
# 𐤀𐤌 !defined __clang__ && !defined __APPLE__
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
# 𐤀𐤌 !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# 𐤀𐤌 defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, فراغ* userdata, فراغ** prevUserdata)
{
    𐤀𐤌( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    أرجع prevCallback;
}

فراغ terminate(Error::Code _code, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح _line) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, _line));
    std::terminate();
}

}

نطاق cv {
منطقي __termination = خطأ;


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
صنف TlsAbstraction
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
#وإلا // _WIN32
    pthread_key_t  tlsKey;
    std::atomic<منطقي> disposed;
#endif
};

صنف TlsAbstractionReleaseGuard
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
    أرجع g_tls;
}


#ifdef _WIN32
#ifdef WINRT
ساكن __declspec( thread ) فراغ* tlsData = عدم; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
}
فراغ* TlsAbstraction::getData() ثابت
{
    أرجع tlsData;
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    tlsData = pData;
}
#وإلا //WINRT
#ifdef CV_USE_FLS
ساكن فراغ NTAPI opencv_fls_destructor(فراغ* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
#ifndef CV_USE_FLS
    tlsKey = TlsAlloc();
#وإلا // CV_USE_FLS
    tlsKey = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tlsKey != TLS_OUT_OF_INDEXES);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
#ifndef CV_USE_FLS
    TlsFree(tlsKey);
#وإلا // CV_USE_FLS
    FlsFree(tlsKey);
#endif // CV_USE_FLS
    tlsKey = TLS_OUT_OF_INDEXES;
}
فراغ* TlsAbstraction::getData() ثابت
{
    𐤀𐤌 (disposed)
        أرجع عدم;
#ifndef CV_USE_FLS
    أرجع TlsGetValue(tlsKey);
#وإلا // CV_USE_FLS
    أرجع FlsGetValue(tlsKey);
#endif // CV_USE_FLS
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    𐤀𐤌 (disposed)
        أرجع;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(TlsSetValue(tlsKey, pData) == TRUE);
#وإلا // CV_USE_FLS
    CV_Assert(FlsSetValue(tlsKey, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#وإلا // _WIN32
ساكن فراغ opencv_tls_destructor(فراغ* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
    CV_Assert(pthread_key_create(&tlsKey, opencv_tls_destructor) == 0);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
    𐤀𐤌 (pthread_key_delete(tlsKey) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
فراغ* TlsAbstraction::getData() ثابت
{
    𐤀𐤌 (disposed)
        أرجع عدم;
    أرجع pthread_getspecific(tlsKey);
}
فراغ TlsAbstraction::setData(فراغ *pData)
{
    𐤀𐤌 (disposed)
        أرجع;  // no-op
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


ساكن منطقي g_isTlsStorageInitialized = خطأ;

// Main TLS storage class
صنف TlsStorage
{
عام:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (فراغ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
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

    فراغ releaseThread(فراغ* tlsValue = عدم)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        𐤀𐤌 (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tlsValue == عدم ? (ThreadData*)tls->getData() : (ThreadData*)tlsValue;
        𐤀𐤌 (pTD == عدم)
            أرجع;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        لكل (size_t i = 0; i < threads.size(); i++)
        {
            𐤀𐤌 (pTD == threads[i])
            {
                threads[i] = عدم;
                𐤀𐤌 (tlsValue == عدم)
                    tls->setData(0);
                std::vector<فراغ*>& thread_slots = pTD->slots;
                لكل (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    فراغ* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = عدم;
                    𐤀𐤌 (!pData)
                        استمر;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    𐤀𐤌 (container)
                        container->deleteDataInstance(pData);
                    وإلا
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحيح)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                أرجع;
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
        لكل(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            𐤀𐤌 (tlsSlots[slot].container == عدم)
            {
                tlsSlots[slot].container = container;
                أرجع slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        أرجع tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ releaseSlot(size_t slotIdx, std::vector<فراغ*> &dataVec, منطقي keepSlot = خطأ)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        لكل(size_t i = 0; i < threads.size(); i++)
        {
            𐤀𐤌(threads[i])
            {
                std::vector<فراغ*>& thread_slots = threads[i]->slots;
                𐤀𐤌 (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = عدم;
                }
            }
        }

        𐤀𐤌 (!keepSlot)
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
        𐤀𐤌 (عدم == tls)
            أرجع عدم;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        𐤀𐤌(threadData && threadData->slots.size() > slotIdx)
            أرجع threadData->slots[slotIdx];

        أرجع عدم;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t slotIdx, std::vector<فراغ*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        لكل(size_t i = 0; i < threads.size(); i++)
        {
            𐤀𐤌(threads[i])
            {
                std::vector<فراغ*>& thread_slots = threads[i]->slots;
                𐤀𐤌 (thread_slots.size() > slotIdx && thread_slots[slotIdx])
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
        𐤀𐤌 (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        𐤀𐤌(!threadData)
        {
            threadData = جديد ThreadData;
            tls->setData((فراغ*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                منطقي found = خطأ;
                // Find unused slots
                لكل(size_t slot = 0; slot < threads.size(); slot++)
                {
                    𐤀𐤌 (threads[slot] == عدم)
                    {
                        threadData->idx = (صحيح)slot;
                        threads[slot] = threadData;
                        found = true;
                        اكسر;
                    }
                }

                𐤀𐤌 (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        𐤀𐤌(slotIdx >= threadData->slots.size())
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
    𐤀𐤌 (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#وإلا // _WIN32
#ifdef CV_USE_FLS
ساكن فراغ WINAPI opencv_fls_destructor(فراغ* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    𐤀𐤌 (res && code != STILL_ACTIVE)
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

    𐤀𐤌 (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

ساكن TlsStorage* ثابت g_force_initialization_of_TlsStorage
#𐤀𐤌 defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#وإلا  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
صنف TlsStorage
{
عام:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        لكل (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            𐤀𐤌 (container && s.data)
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
        لكل (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            𐤀𐤌 (s.container == عدم)
            {
                CV_Assert(!s.data);
                s.container = container;
                أرجع slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        أرجع slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ releaseSlot(size_t slotIdx, std::vector<فراغ*> &dataVec, منطقي keepSlot = خطأ)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        فراغ* data = s.data;
        𐤀𐤌 (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        𐤀𐤌 (!keepSlot)
        {
            s.container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* getData(size_t slotIdx) ثابت
    {
        CV_Assert(slotIdx < slots.size());
        ثابت SlotInfo& s = slots[slotIdx];
        أرجع s.data;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t slotIdx, std::vector<فراغ*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        فراغ* data = s.data;
        𐤀𐤌 (data)
            dataVec.push_back(data);
        أرجع;
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
    أرجع g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using نطاق details;

فراغ releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    𐤀𐤌 (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
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
    getTlsStorage().releaseSlot(key_, data, true);
}

فراغ TLSDataContainer::release()
{
    𐤀𐤌 (key_ == -1)
        أرجع;  // already released
    std::vector<فراغ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, خطأ); // Release key and get stored data for proper destruction
    key_ = -1;
    لكل(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

فراغ TLSDataContainer::cleanup()
{
    std::vector<فراغ*> data; data.reserve(32);
    getTlsStorage().releaseSlot(key_, data, true); // Extract stored data with removal from TLS tables
    لكل(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

فراغ* TLSDataContainer::getData() ثابت
{
    CV_Assert(key_ != -1 && "Can't fetch data from terminated TLS container.");
    فراغ* pData = getTlsStorage().getData(key_); // Check if data was already allocated
    𐤀𐤌(!pData)
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
    أرجع pData;
}

ساكن TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, جديد TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    أرجع getCoreTlsDataTLS().getRef();
}

#𐤀𐤌 defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    𐤀𐤌 (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        𐤀𐤌 (lpReserved != عدم) // called after ExitProcess() call
        {
            cv::__termination = true;
        }
        وإلا
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    أرجع TRUE;
}
#endif


نطاق {

#ifdef OPENCV_WITH_ITT
منطقي overrideThreadName()
{
    ساكن منطقي param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", خطأ);
    أرجع param;
}
#endif

ساكن صحيح g_threadNum = 0;
صنف ThreadID {
عام:
    ثابت صحيح id;
    ThreadID() :
        id(CV_XADD(&g_threadNum, 1))
    {
#ifdef OPENCV_WITH_ITT
        𐤀𐤌 (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

ساكن TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, جديد TLSData<ThreadID>());
}

} // namespace
صحيح utils::getThreadID() { أرجع getThreadIDTLS().get()->id; }


صنف ParseError
{
    std::string bad_value;
عام:
    ParseError(ثابت std::string &bad_value_) :bad_value(bad_value_) {}
    std::string toString(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_value;
        أرجع out.str();
    }
};

template <typename T>
T parseOption(ثابت std::string &);

template<>
مضمن منطقي parseOption(ثابت std::string & value)
{
    𐤀𐤌 (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        أرجع true;
    }
    𐤀𐤌 (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        أرجع خطأ;
    }
    ارم ParseError(value);
}

template<>
مضمن size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    لكل (; pos < value.size(); pos++)
    {
        𐤀𐤌 (!isdigit(value[pos]))
            اكسر;
    }
    cv::String valueStr = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(valueStr);
    𐤀𐤌 (suffixStr.length() == 0)
        أرجع v;
    وإلا 𐤀𐤌 (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        أرجع v * 1024 * 1024;
    وإلا 𐤀𐤌 (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        أرجع v * 1024;
    ارم ParseError(value);
}

template<>
مضمن cv::String parseOption(ثابت std::string &value)
{
    أرجع value;
}

template<>
مضمن utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    ثابت حرف sep = ';';
#وإلا
    ثابت حرف sep = ':';
#endif
    size_t start_pos = 0;
    طالما (start_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, start_pos);
        ثابت std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        𐤀𐤌 (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    أرجع result;
}

ساكن مضمن ثابت حرف * envRead(ثابت حرف * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    أرجع عدم;
#وإلا
    أرجع getenv(name);
#endif
}

template<typename T>
مضمن T اقرأ(ثابت std::string & k, ثابت T & defaultValue)
{
    حاول
    {
        ثابت حرف * res = envRead(k.c_str());
        𐤀𐤌 (res)
            أرجع parseOption<T>(std::string(res));
    }
    امسك (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    أرجع defaultValue;
}

منطقي utils::getConfigurationParameterBool(ثابت حرف* name, منطقي defaultValue)
{
    أرجع اقرأ<منطقي>(name, defaultValue);
}

size_t utils::getConfigurationParameterSizeT(ثابت حرف* name, size_t defaultValue)
{
    أرجع اقرأ<size_t>(name, defaultValue);
}

std::string utils::getConfigurationParameterString(ثابت حرف* name, ثابت std::string & defaultValue)
{
    أرجع اقرأ<cv::String>(name, defaultValue);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت حرف* name, ثابت utils::Paths &defaultValue)
{
    أرجع اقرأ<utils::Paths>(name, defaultValue);
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
    𐤀𐤌(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        𐤀𐤌(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
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
    أرجع getImplData().implFlags; // return actual flags for lazy collection
}

منطقي useCollection()
{
    أرجع getImplData().useCollection;
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
    أرجع getInstrumentStruct().useInstr;
#وإلا
    أرجع خطأ;
#endif
}

فراغ setUseInstrumentation(منطقي flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#وإلا
    CV_UNUSED(flag);
#endif
}

InstrNode* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    أرجع &getInstrumentStruct().rootNode;
#وإلا
    أرجع عدم;
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
#وإلا
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    أرجع (FLAGS)getInstrumentStruct().flags;
#وإلا
    أرجع (FLAGS)0;
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

    m_funError  = خطأ;
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

    أرجع *هذا;
}
NodeData::~NodeData()
{
}
منطقي operator==(ثابت NodeData& left, ثابت NodeData& right)
{
    𐤀𐤌(left.m_lineNum == right.m_lineNum && left.m_funName == right.m_funName && left.m_fileName == right.m_fileName)
    {
        𐤀𐤌(left.m_retAddress == right.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || left.m_alwaysExpand))
            أرجع true;
    }
    أرجع خطأ;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ساكن InstrStruct instr;
    أرجع instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    أرجع *getInstrumentStruct().tlsStruct.get();
}

InstrNode* getCurrentNode()
{
    أرجع getInstrumentTLSStruct().pCurrentNode;
}

IntrumentationRegion::IntrumentationRegion(ثابت حرف* funName, ثابت حرف* fileName, صحيح lineNum, فراغ *retAddress, منطقي alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = خطأ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    𐤀𐤌(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        𐤀𐤌(!pTLS->pCurrentNode)
        {
            m_disabled = true;
            أرجع;
        }

        صحيح depth = pTLS->pCurrentNode->getDepth();
        𐤀𐤌(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = true;
            أرجع;
        }

        NodeData payload(funName, fileName, lineNum, retAddress, alwaysExpand, instrType, implType);
        Node<NodeData>* pChild = عدم;

        𐤀𐤌(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->pCurrentNode->findChild(payload);
            𐤀𐤌(!pChild)
            {
                pChild = جديد Node<NodeData>(payload);
                pTLS->pCurrentNode->addChild(pChild);
            }
        }
        وإلا
        {
            pChild = pTLS->pCurrentNode->findChild(payload);
            𐤀𐤌(!pChild)
            {
                m_disabled = true;
                أرجع;
            }
        }
        pTLS->pCurrentNode = pChild;

        m_regionTicks = getTickCount();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    𐤀𐤌(pStruct->useInstr)
    {
        𐤀𐤌(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            𐤀𐤌 (pTLS->pCurrentNode->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
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
        useIPP         = true;

#𐤀𐤌 defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#وإلا
        useIPP_NE      = خطأ;
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
        𐤀𐤌(ippStatus < 0)
        {
            CV_LOG_ERROR(عدم, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = خطأ;
            أرجع;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        𐤀𐤌(!env.empty())
        {
#𐤀𐤌 IPP_VERSION_X100 >= 201900
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
#وإلا
            ثابت Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            𐤀𐤌(env.substr(0, 2) == "ne")
            {
                useIPP_NE = true;
                env = env.substr(3, env.size());
            }

            𐤀𐤌(env == "disabled")
            {
                CV_LOG_WARNING(عدم, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = خطأ;
            }
            وإلا 𐤀𐤌(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            وإلا 𐤀𐤌(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#𐤀𐤌 IPP_VERSION_X100 >= 201700
#𐤀𐤌 defined (_M_AMD64) || defined (__x86_64__)
            وإلا 𐤀𐤌(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            وإلا
                CV_LOG_ERROR(عدم, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        𐤀𐤌(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        𐤀𐤌(!(
#𐤀𐤌 IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = خطأ;
            أرجع;
        }

        𐤀𐤌(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        وإلا
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#𐤀𐤌 IPP_VERSION_X100 >= 201700
        𐤀𐤌(ippFeatures&ippCPUID_AVX512F)
        {
            𐤀𐤌((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ippTopFeatures = ippCPUID_AVX512_SKX;
            وإلا 𐤀𐤌((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ippTopFeatures = ippCPUID_AVX512_KNL;
            وإلا
                ippTopFeatures = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        وإلا
#endif
        𐤀𐤌(ippFeatures&ippCPUID_AVX2)
            ippTopFeatures = ippCPUID_AVX2;
        وإلا 𐤀𐤌(ippFeatures&ippCPUID_SSE42)
            ippTopFeatures = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        𐤀𐤌 (ippName.find("SSE4.2") != std::string::npos)
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
    أرجع getIPPSingleton().ippFeatures;
#وإلا
    أرجع 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long getIppTopFeatures()
{
    أرجع getIPPSingleton().ippTopFeatures;
}
#endif

فراغ setIppStatus(صحيح status, ثابت حرف * ثابت _funcname, ثابت حرف * ثابت _filename, صحيح _line)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = _line;
#وإلا
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(_line);
#endif
}

صحيح getIppStatus()
{
#ifdef HAVE_IPP
    أرجع getIPPSingleton().ippStatus;
#وإلا
    أرجع 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    أرجع format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#وإلا
    أرجع String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ثابت IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    𐤀𐤌(pInfo)
        أرجع format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    وإلا
        أرجع String("error");
#وإلا
    أرجع String("disabled");
#endif
}

منطقي useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    𐤀𐤌 (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    أرجع (data.useIPP > 0);
#وإلا
    أرجع خطأ;
#endif
}

فراغ setUseIPP(منطقي flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:خطأ;
#وإلا
    CV_UNUSED(flag);
    data.useIPP = خطأ;
#endif
}

منطقي useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    𐤀𐤌 (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    أرجع (data.useIPP_NE > 0);
#وإلا
    أرجع خطأ;
#endif
}

فراغ setUseIPP_NotExact(منطقي flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#وإلا
    CV_UNUSED(flag);
    data.useIPP_NE = خطأ;
#endif
}

} // namespace ipp


نطاق details {

#𐤀𐤌 OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

فراغ setFPDenormalsIgnoreHint(منطقي ignore, CV_OUT FPDenormalsModeState& state)
{
#𐤀𐤌 OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    𐤀𐤌 (featuresEnabled.have[CPU_SSE3])
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
#وإلا
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

صحيح saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#𐤀𐤌 OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    𐤀𐤌 (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ثابت unsigned old_flags = _mm_getcsr();
    ثابت unsigned old_value = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_value;
    أرجع 2;
#وإلا
    CV_UNUSED(state);
    أرجع 0;
#endif
}

منطقي restoreFPDenormalsState(ثابت FPDenormalsModeState& state)
{
#𐤀𐤌 OPENCV_IMPL_FP_HINTS_X86
    ثابت unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((صحيح)value, value == (value & mask), "invalid SSE FP state");
    ثابت unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(عدم, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    أرجع true;
#وإلا
    CV_UNUSED(state);
    أرجع خطأ;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    أرجع OPENCV_ALGO_HINT_DEFAULT;
#وإلا
    أرجع ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
