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

#அந்தர்பாவ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#அந்தர்பாவ <atomic>
#அந்தர்பாவ <exception>
#அந்தர்பாவ <iostream>
#அந்தர்பாவ <ostream>

#ifdef __QNX__
    #அந்தர்பாவ <unistd.h>
    #அந்தர்பாவ <sys/neutrino.h>
    #அந்தர்பாவ <sys/syspage.h>
#ifdef __aarch64__
    #அந்தர்பாவ <aarch64/syspage.h>
#endif
#endif

#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>
#அந்தர்பாவ <opencv2/core/utils/trace.private.hpp>

#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#அந்தர்பாவ <opencv2/core/utils/tls.hpp>
#அந்தர்பாவ <opencv2/core/utils/instrumentation.hpp>

#அந்தர்பாவ <opencv2/core/utils/filesystem.private.hpp>

#அந்தர்பாவ <opencv2/core/utils/fp_control_utils.hpp>
#அந்தர்பாவ <opencv2/core/utils/fp_control.private.hpp>

நாமாகாஶ cv {

நிலையான வெறுமை _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    நிலையான std::ios_base::Init s_iostream_initializer;
#endif
}

நிலையான Mutex* __initialization_mutex = வெற்று;
Mutex& getInitializationMutex()
{
    எனில் (__initialization_mutex == வெற்று)
    {
        (வெறுமை)_initSystem();
        __initialization_mutex = புதிய Mutex();
    }
    திரும்பு *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

நிலையான தருக்க param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#எனில் defined(_DEBUG) || defined(__ANDROID__)
    உண்மை
#இல்லையெனில்
    பொய்
#endif
);

வெறுமை* allocSingletonBuffer(size_t size) { திரும்பு fastMalloc(size); }
வெறுமை* allocSingletonNewBuffer(size_t size) { திரும்பு malloc(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# எனில் defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#எனில் defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# எனில் _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#அந்தர்பாவ <cstdlib>        // std::abort
#endif

#எனில் defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#எனில் defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
மாறிலி uint64_t AT_HWCAP = NT_GNU_HWCAP;
#இல்லையெனில்
#  include <elf.h>
#endif
#எனில் defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#எனில் defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#எனில் ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#எனில் defined __loongarch64
#அந்தர்பாவ "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#எனில் defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#அந்தர்பாவ <windows.h>
#அந்தர்பாவ <combaseapi.h>
#எனில் (_WIN32_WINNT >= 0x0602)
  #அந்தர்பாவ <synchapi.h>
#endif
#எனில் ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #அந்தர்பாவ <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#அந்தர்பாவ <tchar.h>

#ifdef WINRT
#அந்தர்பாவ <wrl/client.h>
#ifndef __cplusplus_winrt
#அந்தர்பாவ <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    திரும்பு std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#இல்லையெனில்
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    எனில் (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        திரும்பு wstr;
    எனில் (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        திரும்பு wstr;
    எனில் (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        திரும்பு wstr;
    எனில் (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        திரும்பு wstr;
    எனில் (FAILED(storagefolderRef.As(&storageitemRef)))
        திரும்பு wstr;
    str = வெற்று;
    எனில் (FAILED(storageitemRef->get_Path(&str)))
        திரும்பு wstr;
    wstr = WindowsGetStringRawBuffer(str, வெற்று);
    WindowsDeleteString(str);
    திரும்பு wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], அளவு(guidStr)/அளவு(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    திரும்பு prefix.append(std::wstring(guidStr));
}

#endif
#இல்லையெனில்
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#அந்தர்பாவ <pthread.h>
#endif
#அந்தர்பாவ <sys/time.h>
#அந்தர்பாவ <time.h>

#எனில் defined __MACH__ && defined __APPLE__
#அந்தர்பாவ <mach/mach.h>
#அந்தர்பாவ <mach/mach_time.h>
#அந்தர்பாவ <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#அந்தர்பாவ "omp.h"
#endif

#எனில் defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <sys/types.h>
#எனில் defined __ANDROID__
#அந்தர்பாவ <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #எனில் defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #எனில் _MSC_VER >= 1400  // MSVS 2005
      #அந்தர்பாவ <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #இல்லையெனில்
      #error "Required MSVS 2005+"
    #endif
  #அல்லதெனில் defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    நிலையான வெறுமை cv_cpuid(முழு* cpuid_data, முழு reg_eax, முழு reg_ecx)
    {
        முழு __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#எனில் !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#அல்லதெனில் defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#இல்லையெனில்
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#அந்தர்பாவ <chrono>

நாமாகாஶ cv
{

Exception::Exception() { code = Error::StsOk; வரி = 0; }

Exception::Exception(Error::Code _code, மாறிலி String& _err, மாறிலி String& _func, மாறிலி String& _file, முழு வரி_3)
: code(_code), err(_err), func(_func), file(_file), வரி(வரி_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
மாறிலி எழுத்து* Exception::what() மாறிலி CV_NOEXCEPT { திரும்பு செய்தி.c_str(); }

வெறுமை Exception::formatMessage()
{
    size_t pos = err.find('\n');
    தருக்க multiline = pos != cv::String::npos;
    எனில் (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        போது (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        எனில் (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    எனில் (func.size() > 0)
    {
        எனில் (multiline)
            செய்தி = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), வரி, code, codeMessage(), func.c_str(), err.c_str());
        இல்லையெனில்
            செய்தி = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), வரி, code, codeMessage(), err.c_str(), func.c_str());
    }
    இல்லையெனில்
    {
        செய்தி = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), வரி, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

மாறிலி எழுத்து * Exception::codeMessage() மாறிலி
{
    தேர்வு (code)
    {
    நிலை cv::Error::StsOk :                  திரும்பு "No Error";
    நிலை cv::Error::StsBackTrace :           திரும்பு "Backtrace";
    நிலை cv::Error::StsError :               திரும்பு "Unspecified error";
    நிலை cv::Error::StsInternal :            திரும்பு "Internal error";
    நிலை cv::Error::StsNoMem :               திரும்பு "Insufficient memory";
    நிலை cv::Error::StsBadArg :              திரும்பு "Bad argument";
    நிலை cv::Error::StsNoConv :              திரும்பு "Iterations do not converge";
    நிலை cv::Error::StsAutoTrace :           திரும்பு "Autotrace call";
    நிலை cv::Error::StsBadSize :             திரும்பு "Incorrect size of input array";
    நிலை cv::Error::StsNullPtr :             திரும்பு "Null pointer";
    நிலை cv::Error::StsDivByZero :           திரும்பு "Division by zero occurred";
    நிலை cv::Error::BadStep :                திரும்பு "Image step is wrong";
    நிலை cv::Error::StsInplaceNotSupported : திரும்பு "Inplace operation is not supported";
    நிலை cv::Error::StsObjectNotFound :      திரும்பு "Requested object was not found";
    நிலை cv::Error::BadDepth :               திரும்பு "Input image depth is not supported by function";
    நிலை cv::Error::StsUnmatchedFormats :    திரும்பு "Formats of input arguments do not match";
    நிலை cv::Error::StsUnmatchedSizes :      திரும்பு "Sizes of input arguments do not match";
    நிலை cv::Error::StsOutOfRange :          திரும்பு "One of the arguments\' values is out of range";
    நிலை cv::Error::StsUnsupportedFormat :   திரும்பு "Unsupported format or combination of formats";
    நிலை cv::Error::BadCOI :                 திரும்பு "Input COI is not supported";
    நிலை cv::Error::Bad_இலக்கம்_Channels :         திரும்பு "Bad number of channels";
    நிலை cv::Error::StsBadFlag :             திரும்பு "Bad flag (parameter or structure field)";
    நிலை cv::Error::StsBadPoint :            திரும்பு "Bad parameter of type CvPoint";
    நிலை cv::Error::StsBadMask :             திரும்பு "Bad type of mask argument";
    நிலை cv::Error::StsParseError :          திரும்பு "Parsing error";
    நிலை cv::Error::StsNotImplemented :      திரும்பு "The function/feature is not implemented";
    நிலை cv::Error::StsBadMemBlock :         திரும்பு "Memory block has been corrupted";
    நிலை cv::Error::StsAssert :              திரும்பு "Assertion failed";
    நிலை cv::Error::GpuNotSupported :        திரும்பு "No CUDA support";
    நிலை cv::Error::GpuApiCallError :        திரும்பு "Gpu API call";
    நிலை cv::Error::OpenGlNotSupported :     திரும்பு "No OpenGL support";
    நிலை cv::Error::OpenGlApiCallError :     திரும்பு "OpenGL API call";
    நிலை cv::Error::OpenCLApiCallError :     திரும்பு "OpenCL API call error";
    நிலை cv::Error::OpenCLDoubleNotSupported:திரும்பு "OpenCL device does not support double";
    நிலை cv::Error::OpenCLInitError :        திரும்பு "OpenCL initialization error";
    நிலை cv::Error::OpenCLNoAMDBlasFft :     திரும்பு "Missing OpenCL AMD BLAS FFT";
    இயல்பு:
    {
        நிலையான எழுத்து buf[256] {0};
        snprintf(buf, அளவு(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        திரும்பு buf;
    }
    };

}

நிலையான மாறிலி எழுத்து* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { வெற்று };

நிலையான மாறிலி எழுத்து* getHWFeatureName(முழு id)
{
    திரும்பு (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : வெற்று;
}
நிலையான மாறிலி எழுத்து* getHWFeatureNameSafe(முழு id)
{
    மாறிலி எழுத்து* name = getHWFeatureName(id);
    திரும்பு name ? name : "Unknown feature";
}

கட்டமைப்பு HWFeatures
{
    எண்ணிக்கை { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(தருக்க run_initialize = பொய்)
    {
        எனில் (run_initialize)
            initialize();
    }

    நிலையான வெறுமை initializeNames()
    {
        க்கு (முழு i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    வெறுமை initialize(வெறுமை)
    {
        எனில் (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        முழு cpuid_data[4] = { 0, 0, 0, 0 };
        முழு cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        முழு x86_family = (cpuid_data[0] >> 8) & 15;
        எனில்( x86_family >= 6 )
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
            முழு cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            தருக்க have_AVX_OS_support = உண்மை;
            தருக்க have_AVX512_OS_support = உண்மை;
            எனில் (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = பொய்; // OS uses XSAVE_XRSTORE and CPU support AVX
            இல்லையெனில்
            {
                முழு xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (முழு)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #அல்லதெனில் defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                எனில் ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = பொய்; // YMM registers
                எனில் ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = பொய்; // ZMM registers
            }

            எனில் (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = பொய்;
                have[CV_CPU_FP16] = பொய்;
                have[CV_CPU_AVX2] = பொய்;
                have[CV_CPU_FMA3] = பொய்;
                have[CV_CPU_AVX_VNNI] = பொய்;
            }
            எனில் (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = பொய்;
                have[CV_CPU_AVX_512BW] = பொய்;
                have[CV_CPU_AVX_512CD] = பொய்;
                have[CV_CPU_AVX_512DQ] = பொய்;
                have[CV_CPU_AVX_512ER] = பொய்;
                have[CV_CPU_AVX_512IFMA] = பொய்;
                have[CV_CPU_AVX_512PF] = பொய்;
                have[CV_CPU_AVX_512VBMI] = பொய்;
                have[CV_CPU_AVX_512VL] = பொய்;
                have[CV_CPU_AVX_512VBMI2] = பொய்;
                have[CV_CPU_AVX_512VNNI] = பொய்;
                have[CV_CPU_AVX_512BITALG] = பொய்;
                have[CV_CPU_AVX_512VPOPCNTDQ] = பொய்;
                have[CV_CPU_AVX_5124VNNIW] = பொய்;
                have[CV_CPU_AVX_5124FMAPS] = பொய்;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            எனில் (have[CV_CPU_AVX512_COMMON])
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
            இல்லையெனில்
            {
                have[CV_CPU_AVX512_KNL] = பொய்;
                have[CV_CPU_AVX512_KNM] = பொய்;
                have[CV_CPU_AVX512_SKX] = பொய்;
                have[CV_CPU_AVX512_CNL] = பொய்;
                have[CV_CPU_AVX512_CLX] = பொய்;
                have[CV_CPU_AVX512_ICL] = பொய்;
            }
        }
    #endif // CV_CPUID_X86

    #எனில் defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = உண்மை;
        have[CV_CPU_FP16] = உண்மை;
        முழு cpufile = open("/proc/self/auxv", O_RDONLY);

        எனில் (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            மாறிலி size_t size_auxv_t = அளவு(auxv);

            போது ((size_t)பட(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                எனில் (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#எனில் defined(AT_HWCAP2)
                இல்லையெனில் எனில் (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            close(cpufile);
        }
    #அல்லதெனில் defined __arm__ && defined __ANDROID__
      #எனில் defined HAVE_CPUFEATURES
        CV_LOG_INFO(வெற்று, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(வெற்று, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #இல்லையெனில்
        CV_LOG_INFO(வெற்று, "cpufeatures library is not available for CPU detection");
        #எனில் CV_NEON
        CV_LOG_INFO(வெற்று, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = உண்மை;
        #இல்லையெனில்
        CV_LOG_INFO(வெற்று, "- NEON instructions is NOT enabled via build flags");
        #endif
        #எனில் CV_FP16
        CV_LOG_INFO(வெற்று, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = உண்மை;
        #இல்லையெனில்
        CV_LOG_INFO(வெற்று, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #அல்லதெனில் defined __arm__
        முழு cpufile = open("/proc/self/auxv", O_RDONLY);

        எனில் (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            மாறிலி size_t size_auxv_t = அளவு(auxv);

            போது ((size_t)பட(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                எனில் (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    நிறுத்து;
                }
            }

            close(cpufile);
        }
    #endif
    #அல்லதெனில் (defined __APPLE__)
    #எனில் defined __ARM_NEON
        have[CV_CPU_NEON] = உண்மை;
    #endif
    #எனில் (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = உண்மை;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    முழு has_feat_dotprod = 0;
    size_t has_feat_dotprod_size = அளவு(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, வெற்று, 0);
    எனில் (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = உண்மை;
    }
    முழு has_feat_bf16 = 0;
    size_t has_feat_bf16_size = அளவு(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, வெற்று, 0);
    எனில் (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = உண்மை;
    }
    #அல்லதெனில் (defined __clang__)
    #எனில் defined __ARM_NEON
        have[CV_CPU_NEON] = உண்மை;
        #எனில் (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = உண்மை;
        #endif
    #endif
    #endif
    #எனில் defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = உண்மை;
    #endif
    #எனில் defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = உண்மை;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = உண்மை;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = உண்மை;
    #endif

    #எனில் (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned முழு hwcap = getauxval(AT_HWCAP);
        எனில் (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            எனில் (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = உண்மை;
            } இல்லையெனில் {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #அல்லதெனில் (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        எனில் (elf_aux_info(AT_HWCAP, &hwcap, அளவு(hwcap)) != 0)
            hwcap = 0;
        எனில் (hwcap & PPC_FEATURE_HAS_VSX) {
            எனில் (elf_aux_info(AT_HWCAP2, &hwcap2, அளவு(hwcap2)) != 0)
                hwcap2 = 0;
            எனில் (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = உண்மை;
            } இல்லையெனில் {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #இல்லையெனில்
        // TODO: AIX
        #எனில் CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = உண்மை;
        #endif
        #எனில் CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = உண்மை;
        #endif
    #endif

    #எனில் defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = உண்மை;
        #எனில் (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = உண்மை;
        #endif
        #எனில் defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = உண்மை;
        #endif
    #endif

    #எனில் defined __loongarch64 && defined __linux__
        முழு flag = (முழு)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        தருக்க skip_baseline_check = பொய்;
        எனில் (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = உண்மை;
        }
        முழு baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        எனில் (!checkFeatures(baseline_features, அளவு(baseline_features) / அளவு(baseline_features[0]))
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
            checkFeatures(baseline_features, அளவு(baseline_features) / அளவு(baseline_features[0]), உண்மை);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, அளவு(baseline_features) / அளவு(baseline_features[0]));
    }

    தருக்க checkFeatures(மாறிலி முழு* features, முழு மொத்தம்_2, தருக்க dump = பொய்)
    {
        தருக்க result = உண்மை;
        க்கு (முழு i = 0; i < மொத்தம்_2; i++)
        {
            முழு feature = features[i];
            எனில் (feature)
            {
                எனில் (have[feature])
                {
                    எனில் (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                இல்லையெனில்
                {
                    result = பொய்;
                    எனில் (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        திரும்பு result;
    }

    நிலையான உள்வரி தருக்க is_குறியீடு_Separator(எழுத்து c)
    {
        திரும்பு c == ',' || c == ';';
    }

    வெறுமை readSettings(மாறிலி முழு* baseline_features, முழு baseline_மொத்தம்)
    {
        தருக்க dump = உண்மை;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        எனில் (!disabled_features.empty())
        {
            மாறிலி எழுத்து* start = disabled_features.c_str();
            க்கு (;;)
            {
                போது (start[0] != 0 && is_குறியீடு_Separator(start[0]))
                {
                    start++;
                }
                எனில் (start[0] == 0)
                    நிறுத்து;
                மாறிலி எழுத்து* end = start;
                போது (end[0] != 0 && !is_குறியீடு_Separator(end[0]))
                {
                    end++;
                }
                எனில் (end == start)
                    தொடர்;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                தருக்க found = பொய்;
                க்கு (முழு i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    எனில் (!g_hwFeatureNames[i]) தொடர்;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    எனில் (len != feature.size()) தொடர்;
                    எனில் (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        தருக்க isBaseline = பொய்;
                        க்கு (முழு k = 0; k < baseline_மொத்தம்; k++)
                        {
                            எனில் (baseline_features[k] == i)
                            {
                                isBaseline = உண்மை;
                                நிறுத்து;
                            }
                        }
                        எனில் (isBaseline)
                        {
                            எனில் (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        எனில் (!have[i])
                        {
                            எனில் (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = பொய்;

                        found = உண்மை;
                        நிறுத்து;
                    }
                }
                எனில் (!found)
                {
                    எனில் (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    தருக்க have[MAX_FEATURE+1]{};
};

நிலையான HWFeatures  featuresEnabled(உண்மை), featuresDisabled = HWFeatures(பொய்);
நிலையான HWFeatures* currentFeatures = &featuresEnabled;

தருக்க checkHardwareSupport(முழு feature)
{
    எனில் (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        திரும்பு பொய்;
    திரும்பு currentFeatures->have[feature];
}

String getHardwareFeatureName(முழு feature)
{
    மாறிலி எழுத்து* name = getHWFeatureName(feature);
    திரும்பு name ? String(name) : String();
}

std::string get_CPUFeatures_வரி()
{
    மாறிலி முழு features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    மாறிலி முழு sz = அளவு(features) / அளவு(features[0]);
    std::string result;
    std::string prefix;
    க்கு (முழு i = 1; i < sz; ++i)
    {
        எனில் (features[i] == 0)
        {
            prefix = "*";
            தொடர்;
        }
        எனில் (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        எனில் (!checkHardwareSupport(features[i])) result.append("?");
    }
    திரும்பு result;
}

நிலையற்ற தருக்க useOptimizedFlag = உண்மை;

வெறுமை setUseOptimized( தருக்க flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

தருக்க useOptimized(வெறுமை)
{
    திரும்பு useOptimizedFlag;
}

int64 get_Tick_மொத்தம்(வெறுமை)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    திரும்பு (int64)now.time_since_epoch().மொத்தம்_2();
}

இரட்டை getTickFrequency(வெறுமை)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    இரட்டை clock_freq = clock_period_t::den / clock_period_t::num;
    திரும்பு clock_freq;
}

#எனில் defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#எனில் defined(__i386__)

int64 get_CPUTick_மொத்தம்(வெறுமை)
{
    int64 x;
    __asm__ நிலையற்ற (".byte 0x0f, 0x31" : "=A" (x));
    திரும்பு x;
}
#அல்லதெனில் defined(__x86_64__)

int64 get_CPUTick_மொத்தம்(வெறுமை)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    திரும்பு (int64)lo | ((int64)hi << 32);
}

#அல்லதெனில் defined(__ppc__)

int64 get_CPUTick_மொத்தம்(வெறுமை)
{
    unsigned upper, lower, tmp;
    __asm__ நிலையற்ற(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    திரும்பு lower | ((int64)upper << 32);
}

#இல்லையெனில்

#error "RDTSC not defined"

#endif

#அல்லதெனில் defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_மொத்தம்(வெறுமை)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#இல்லையெனில்

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_மொத்தம்(வெறுமை)
{
    திரும்பு get_Tick_மொத்தம்();
}
//#endif

#endif


நாமாகாஶ internal {

வகுப்பு Timestamp
{
பொது:
    மாறிலி int64 zero_Tick_மொத்தம்;
    மாறிலி இரட்டை ns_in_ticks;

    Timestamp()
        : zero_Tick_மொத்தம்(get_Tick_மொத்தம்())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_மொத்தம்();
        திரும்பு (int64)((t - zero_Tick_மொத்தம்) * ns_in_ticks);
    }

    நிலையான Timestamp& getInstance()
    {
        நிலையான Timestamp g_timestamp;
        திரும்பு g_timestamp;
    }
};

வகுப்பு InitTimestamp {
பொது:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
நிலையான InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    திரும்பு internal::Timestamp::getInstance().getTimestamp();
}


மாறிலி String& getBuildInformation()
{
    நிலையான String build_info =
#அந்தர்பாவ "version_string.inc"
    ;
    திரும்பு build_info;
}

String getVersionString() { திரும்பு String(CV_VERSION); }

முழு getVersionMajor() { திரும்பு CV_VERSION_MAJOR; }

முழு getVersionMinor() { திரும்பு CV_VERSION_MINOR; }

முழு getVersionRevision() { திரும்பு CV_VERSION_REVISION; }

String format( மாறிலி எழுத்து* fmt, ... )
{
    AutoBuffer<எழுத்து, 1024> buf;

    க்கு ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        முழு bsize = static_cast<முழு>(buf.size());
        முழு len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        எனில் (len >= bsize)
        {
            buf.resize(len + 1);
            தொடர்;
        }
        buf[bsize - 1] = 0;
        திரும்பு String(buf.data(), len);
    }
}

String tempfile( மாறிலி எழுத்து* suffix )
{
#எனில் OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#எனில் defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    எனில் (temp_file.empty())
        திரும்பு String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    எழுத்து aname[MAX_PATH];
    size_t copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#அல்லதெனில் defined(_WIN32_WCE)
    மாறிலி auto kMaxPathSize = MAX_PATH+1;
    wchar_t temp_dir_ce[kMaxPathSize] = {0};
    wchar_t temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    எனில்(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        எழுத்து aname[MAX_PATH];
        size_t copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#இல்லையெனில்
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t temp_dir2_w[MAX_PATH] = { 0 };

    எனில் (temp_dir.empty())
    {
        ::GetTempPathW(அளவு(temp_dir2_w)/அளவு(wchar_t), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        முழு len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, வெற்று, 0, வெற்று, வெற்று);
        எனில் (len > 0)
        {
            std::vector<எழுத்து> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, வெற்று, வெற்று);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    எனில் (FAILED(hr))
        திரும்பு String();
    எழுத்து guidStr[40];
    மாறிலி எழுத்து* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, அளவு(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned முழு)g.Data4[0], (unsigned முழு)g.Data4[1],
            (unsigned முழு)g.Data4[2], (unsigned முழு)g.Data4[3], (unsigned முழு)g.Data4[4],
            (unsigned முழு)g.Data4[5], (unsigned முழு)g.Data4[6], (unsigned முழு)g.Data4[7]);

    fname = temp_dir;
    எனில் (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# இல்லையெனில்
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    எழுத்து defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  இல்லையெனில்
    எழுத்து defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    எனில் (temp_dir.empty())
        fname = defaultTemplate;
    இல்லையெனில்
    {
        fname = temp_dir;
        எழுத்து ech = fname[fname.size() - 1];
        எனில்(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    மாறிலி முழு fd = mkstemp((எழுத்து*)fname.c_str());
    எனில் (fd == -1) திரும்பு String();

    close(fd);
    remove(fname.c_str());
# endif

    எனில் (suffix)
    {
        எனில் (suffix[0] != '.')
            திரும்பு fname + "." + suffix;
        இல்லையெனில்
            திரும்பு fname + suffix;
    }
    திரும்பு fname;
#இல்லையெனில் // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

நிலையான ErrorCallback customErrorCallback = 0;
நிலையான வெறுமை* customErrorCallbackData = 0;
நிலையான தருக்க breakOnError = பொய்;

தருக்க setBreakOnError(தருக்க value)
{
    தருக்க prevVal = breakOnError;
    breakOnError = value;
    திரும்பு prevVal;
}

முழு cv_snprintf(எழுத்து* buf, முழு len, மாறிலி எழுத்து* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    முழு res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    திரும்பு res;
}

முழு cv_vsnprintf(எழுத்து* buf, முழு len, மாறிலி எழுத்து* fmt, va_list args)
{
#எனில் defined _MSC_VER
    எனில் (len <= 0) திரும்பு len == 0 ? 1024 : -1;
    முழு res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    எனில் (res >= 0 && res < len)
    {
        buf[res] = 0;
        திரும்பு res;
    }
    இல்லையெனில்
    {
        buf[len - 1] = 0; // truncate happened
        திரும்பு res >= len ? res : (len * 2);
    }
#இல்லையெனில்
    திரும்பு vsnprintf(buf, len, fmt, args);
#endif
}

நிலையான வெறுமை dumpException(மாறிலி Exception& exc)
{
    மாறிலி எழுத்து* errorStr = exc.codeMessage();
    எழுத்து buf[1 << 12];

    cv_snprintf(buf, அளவு(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.வரி);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#இல்லையெனில்
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
நிலையான தருக்க cv_terminate_handler_installed = பொய்;
நிலையான std::terminate_handler cv_old_terminate_handler;
நிலையான cv::Exception cv_terminate_handler_exception;
நிலையான தருக்க param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", உண்மை);
நிலையான வெறுமை cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    எனில் (பொய் /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#endif

#ifdef __GNUC__
# எனில் defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

வெறுமை error( மாறிலி Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        எனில் (!cv_terminate_handler_installed)
        {
            எனில் (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = உண்மை;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    எனில் (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.வரி, customErrorCallbackData);
    இல்லையெனில் எனில் (param_dumpErrors)
    {
        dumpException(exc);
    }

    எனில்(breakOnError)
    {
        std::terminate();
    }

    எறி exc;
#ifdef __GNUC__
# எனில் !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

வெறுமை error(Error::Code _code, மாறிலி String& _err, மாறிலி எழுத்து* _func, மாறிலி எழுத்து* _file, முழு வரி_3)
{
    error(cv::Exception(_code, _err, _func, _file, வரி_3));
#ifdef __GNUC__
# எனில் !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# எனில் defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, வெறுமை* userdata, வெறுமை** prevUserdata)
{
    எனில்( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    திரும்பு prevCallback;
}

வெறுமை terminate(Error::Code _code, மாறிலி String& _err, மாறிலி எழுத்து* _func, மாறிலி எழுத்து* _file, முழு வரி_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, வரி_3));
    std::terminate();
}

}

நாமாகாஶ cv {
தருக்க __termination = பொய்;


//////////////////////////////// thread-local storage ////////////////////////////////

நாமாகாஶ details {

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
வகுப்பு TlsAbstraction
{
பொது:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    வெறுமை* getData() மாறிலி;
    வெறுமை setData(வெறுமை *pData);

    வெறுமை releaseSystemResources();

தனிப்பட்ட:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_சாவி;
    தருக்க disposed;
#endif
#இல்லையெனில் // _WIN32
    pthread_key_t  tls_சாவி;
    std::atomic<தருக்க> disposed;
#endif
};

வகுப்பு TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
பொது:
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
நிலையான TlsAbstraction* getTlsAbstraction()
{
    நிலையான TlsAbstraction *g_tls = புதிய TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    நிலையான TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    திரும்பு g_tls;
}


#ifdef _WIN32
#ifdef WINRT
நிலையான __declspec( thread ) வெறுமை* tlsData = வெற்று; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
வெறுமை TlsAbstraction::releaseSystemResources()
{
    cv::__termination = உண்மை;  // DllMain is missing in static builds
}
வெறுமை* TlsAbstraction::getData() மாறிலி
{
    திரும்பு tlsData;
}
வெறுமை TlsAbstraction::setData(வெறுமை *pData)
{
    tlsData = pData;
}
#இல்லையெனில் //WINRT
#ifdef CV_USE_FLS
நிலையான வெறுமை NTAPI opencv_fls_destructor(வெறுமை* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(பொய்)
{
#ifndef CV_USE_FLS
    tls_சாவி = TlsAlloc();
#இல்லையெனில் // CV_USE_FLS
    tls_சாவி = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_சாவி != TLS_OUT_OF_INDEXES);
}
வெறுமை TlsAbstraction::releaseSystemResources()
{
    cv::__termination = உண்மை;  // DllMain is missing in static builds
    disposed = உண்மை;
#ifndef CV_USE_FLS
    TlsFree(tls_சாவி);
#இல்லையெனில் // CV_USE_FLS
    FlsFree(tls_சாவி);
#endif // CV_USE_FLS
    tls_சாவி = TLS_OUT_OF_INDEXES;
}
வெறுமை* TlsAbstraction::getData() மாறிலி
{
    எனில் (disposed)
        திரும்பு வெற்று;
#ifndef CV_USE_FLS
    திரும்பு Tls_Get_மதிப்பு(tls_சாவி);
#இல்லையெனில் // CV_USE_FLS
    திரும்பு Fls_Get_மதிப்பு(tls_சாவி);
#endif // CV_USE_FLS
}
வெறுமை TlsAbstraction::setData(வெறுமை *pData)
{
    எனில் (disposed)
        திரும்பு;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_மதிப்பு(tls_சாவி, pData) == TRUE);
#இல்லையெனில் // CV_USE_FLS
    CV_Assert(Fls_Set_மதிப்பு(tls_சாவி, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#இல்லையெனில் // _WIN32
நிலையான வெறுமை opencv_tls_destructor(வெறுமை* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(பொய்)
{
    CV_Assert(pthread_key_create(&tls_சாவி, opencv_tls_destructor) == 0);
}
வெறுமை TlsAbstraction::releaseSystemResources()
{
    cv::__termination = உண்மை;  // DllMain is missing in static builds
    disposed = உண்மை;
    எனில் (pthread_key_delete(tls_சாவி) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
வெறுமை* TlsAbstraction::getData() மாறிலி
{
    எனில் (disposed)
        திரும்பு வெற்று;
    திரும்பு pthread_getspecific(tls_சாவி);
}
வெறுமை TlsAbstraction::setData(வெறுமை *pData)
{
    எனில் (disposed)
        திரும்பு;  // no-op
    CV_Assert(pthread_setspecific(tls_சாவி, pData) == 0);
}
#endif

// Per-thread data structure
கட்டமைப்பு ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<வெறுமை*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


நிலையான தருக்க g_isTlsStorageInitialized = பொய்;

// Main TLS storage class
வகுப்பு TlsStorage
{
பொது:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (வெறுமை)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = உண்மை;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    வெறுமை releaseThread(வெறுமை* tls_மதிப்பு = வெற்று)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        எனில் (வெற்று == tls)
            திரும்பு;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_மதிப்பு == வெற்று ? (ThreadData*)tls->getData() : (ThreadData*)tls_மதிப்பு;
        எனில் (pTD == வெற்று)
            திரும்பு;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        க்கு (size_t i = 0; i < threads.size(); i++)
        {
            எனில் (pTD == threads[i])
            {
                threads[i] = வெற்று;
                எனில் (tls_மதிப்பு == வெற்று)
                    tls->setData(0);
                std::vector<வெறுமை*>& thread_slots = pTD->slots;
                க்கு (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    வெறுமை* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = வெற்று;
                    எனில் (!pData)
                        தொடர்;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    எனில் (container)
                        container->deleteDataInstance(pData);
                    இல்லையெனில்
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (முழு)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                திரும்பு;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (வெறுமை*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        க்கு(size_t slot = 0; slot < tlsSlotsSize; slot++)
        {
            எனில் (tlsSlots[slot].container == வெற்று)
            {
                tlsSlots[slot].container = container;
                திரும்பு slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        திரும்பு tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    வெறுமை releaseSlot(size_t slotIdx, std::vector<வெறுமை*> &dataVec, தருக்க keepSlot = பொய்)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        க்கு(size_t i = 0; i < threads.size(); i++)
        {
            எனில்(threads[i])
            {
                std::vector<வெறுமை*>& thread_slots = threads[i]->slots;
                எனில் (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = வெற்று;
                }
            }
        }

        எனில் (!keepSlot)
        {
            tlsSlots[slotIdx].container = வெற்று;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    வெறுமை* getData(size_t slotIdx) மாறிலி
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        எனில் (வெற்று == tls)
            திரும்பு வெற்று;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        எனில்(threadData && threadData->slots.size() > slotIdx)
            திரும்பு threadData->slots[slotIdx];

        திரும்பு வெற்று;
    }

    // Gather data from threads by TLS storage index
    வெறுமை gather(size_t slotIdx, std::vector<வெறுமை*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        க்கு(size_t i = 0; i < threads.size(); i++)
        {
            எனில்(threads[i])
            {
                std::vector<வெறுமை*>& thread_slots = threads[i]->slots;
                எனில் (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    வெறுமை setData(size_t slotIdx, வெறுமை* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        எனில் (வெற்று == tls)
            திரும்பு;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        எனில்(!threadData)
        {
            threadData = புதிய ThreadData;
            tls->setData((வெறுமை*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                தருக்க found = பொய்;
                // Find unused slots
                க்கு(size_t slot = 0; slot < threads.size(); slot++)
                {
                    எனில் (threads[slot] == வெற்று)
                    {
                        threadData->idx = (முழு)slot;
                        threads[slot] = threadData;
                        found = உண்மை;
                        நிறுத்து;
                    }
                }

                எனில் (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        எனில்(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, வெற்று);
        }
        threadData->slots[slotIdx] = pData;
    }

தனிப்பட்ட:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    கட்டமைப்பு TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<கட்டமைப்பு TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
நிலையான TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, புதிய TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
நிலையான வெறுமை opencv_tls_destructor(வெறுமை* pData)
{
    எனில் (!g_isTlsStorageInitialized)
        திரும்பு;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#இல்லையெனில் // _WIN32
#ifdef CV_USE_FLS
நிலையான வெறுமை WINAPI opencv_fls_destructor(வெறுமை* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    எனில் (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = உண்மை;
    }

    எனில் (!g_isTlsStorageInitialized)
        திரும்பு;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

நிலையான TlsStorage* மாறிலி g_force_initialization_of_TlsStorage
#எனில் defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#இல்லையெனில்  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
வகுப்பு TlsStorage
{
பொது:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        க்கு (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            எனில் (container && s.data)
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
        க்கு (size_t slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            எனில் (s.container == வெற்று)
            {
                CV_Assert(!s.data);
                s.container = container;
                திரும்பு slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        திரும்பு slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    வெறுமை releaseSlot(size_t slotIdx, std::vector<வெறுமை*> &dataVec, தருக்க keepSlot = பொய்)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        வெறுமை* data = s.data;
        எனில் (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        எனில் (!keepSlot)
        {
            s.container = வெற்று;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    வெறுமை* getData(size_t slotIdx) மாறிலி
    {
        CV_Assert(slotIdx < slots.size());
        மாறிலி SlotInfo& s = slots[slotIdx];
        திரும்பு s.data;
    }

    // Gather data from threads by TLS storage index
    வெறுமை gather(size_t slotIdx, std::vector<வெறுமை*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        வெறுமை* data = s.data;
        எனில் (data)
            dataVec.push_back(data);
        திரும்பு;
    }

    // Set data to storage index
    வெறுமை setData(size_t slotIdx, வெறுமை* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

தனிப்பட்ட:
    கட்டமைப்பு SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        வெறுமை* data;
    };
    std::vector<கட்டமைப்பு SlotInfo> slots;
};

நிலையான TlsStorage& getTlsStorage()
{
    நிலையான TlsStorage g_storage;  // no threading
    திரும்பு g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using நாமாகாஶ details;

வெறுமை releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    எனில் (!g_isTlsStorageInitialized)
        திரும்பு;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    சாவி_2 = (முழு)getTlsStorage().reserveSlot(இது); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(சாவி_2 == -1); // Key must be released in child object
}

வெறுமை TLSDataContainer::gatherData(std::vector<வெறுமை*> &data) மாறிலி
{
    getTlsStorage().gather(சாவி_2, data);
}

வெறுமை TLSDataContainer::detachData(std::vector<வெறுமை*> &data)
{
    getTlsStorage().releaseSlot(சாவி_2, data, உண்மை);
}

வெறுமை TLSDataContainer::release()
{
    எனில் (சாவி_2 == -1)
        திரும்பு;  // already released
    std::vector<வெறுமை*> data; data.reserve(32);
    getTlsStorage().releaseSlot(சாவி_2, data, பொய்); // Release key and get stored data for proper destruction
    சாவி_2 = -1;
    க்கு(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

வெறுமை TLSDataContainer::cleanup()
{
    std::vector<வெறுமை*> data; data.reserve(32);
    getTlsStorage().releaseSlot(சாவி_2, data, உண்மை); // Extract stored data with removal from TLS tables
    க்கு(size_t i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

வெறுமை* TLSDataContainer::getData() மாறிலி
{
    CV_Assert(சாவி_2 != -1 && "Can't fetch data from terminated TLS container.");
    வெறுமை* pData = getTlsStorage().getData(சாவி_2); // Check if data was already allocated
    எனில்(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        முயல்
        {
            getTlsStorage().setData(சாவி_2, pData);
        }
        பிடி (...)
        {
            deleteDataInstance(pData);
            எறி;
        }
    }
    திரும்பு pData;
}

நிலையான TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, புதிய TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    திரும்பு getCoreTlsDataTLS().getRef();
}

#எனில் defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

வெளி "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

வெளி "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    எனில் (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        எனில் (lpReserved != வெற்று) // called after ExitProcess() call
        {
            cv::__termination = உண்மை;
        }
        இல்லையெனில்
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    திரும்பு TRUE;
}
#endif


நாமாகாஶ {

#ifdef OPENCV_WITH_ITT
தருக்க overrideThreadName()
{
    நிலையான தருக்க param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", பொய்);
    திரும்பு param;
}
#endif

நிலையான முழு g_thread_இலக்கம் = 0;
வகுப்பு ThreadID {
பொது:
    மாறிலி முழு id;
    ThreadID() :
        id(CV_XADD(&g_thread_இலக்கம், 1))
    {
#ifdef OPENCV_WITH_ITT
        எனில் (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

நிலையான TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, புதிய TLSData<ThreadID>());
}

} // namespace
முழு utils::getThreadID() { திரும்பு getThreadIDTLS().get()->id; }


வகுப்பு ParseError
{
    std::string bad_மதிப்பு;
பொது:
    ParseError(மாறிலி std::string &bad_மதிப்பு_2) :bad_மதிப்பு(bad_மதிப்பு_2) {}
    std::string toString(மாறிலி std::string &param) மாறிலி
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_மதிப்பு;
        திரும்பு out.str();
    }
};

template <typename T>
T parseOption(மாறிலி std::string &);

template<>
உள்வரி தருக்க parseOption(மாறிலி std::string & value)
{
    எனில் (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        திரும்பு உண்மை;
    }
    எனில் (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        திரும்பு பொய்;
    }
    எறி ParseError(value);
}

template<>
உள்வரி size_t parseOption(மாறிலி std::string &value)
{
    size_t pos = 0;
    க்கு (; pos < value.size(); pos++)
    {
        எனில் (!isdigit(value[pos]))
            நிறுத்து;
    }
    cv::String மதிப்பு_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(மதிப்பு_Str);
    எனில் (suffixStr.length() == 0)
        திரும்பு v;
    இல்லையெனில் எனில் (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        திரும்பு v * 1024 * 1024;
    இல்லையெனில் எனில் (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        திரும்பு v * 1024;
    எறி ParseError(value);
}

template<>
உள்வரி cv::String parseOption(மாறிலி std::string &value)
{
    திரும்பு value;
}

template<>
உள்வரி utils::Paths parseOption(மாறிலி std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    மாறிலி எழுத்து sep = ';';
#இல்லையெனில்
    மாறிலி எழுத்து sep = ':';
#endif
    size_t start_pos = 0;
    போது (start_pos != std::string::npos)
    {
        மாறிலி size_t pos = value.find(sep, start_pos);
        மாறிலி std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        எனில் (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    திரும்பு result;
}

நிலையான உள்வரி மாறிலி எழுத்து * envRead(மாறிலி எழுத்து * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    திரும்பு வெற்று;
#இல்லையெனில்
    திரும்பு getenv(name);
#endif
}

template<typename T>
உள்வரி T பட(மாறிலி std::string & k, மாறிலி T & default_மதிப்பு_2)
{
    முயல்
    {
        மாறிலி எழுத்து * res = envRead(k.c_str());
        எனில் (res)
            திரும்பு parseOption<T>(std::string(res));
    }
    பிடி (மாறிலி ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    திரும்பு default_மதிப்பு_2;
}

தருக்க utils::getConfigurationParameterBool(மாறிலி எழுத்து* name, தருக்க default_மதிப்பு_2)
{
    திரும்பு பட<தருக்க>(name, default_மதிப்பு_2);
}

size_t utils::getConfigurationParameterSizeT(மாறிலி எழுத்து* name, size_t default_மதிப்பு_2)
{
    திரும்பு பட<size_t>(name, default_மதிப்பு_2);
}

std::string utils::getConfigurationParameterString(மாறிலி எழுத்து* name, மாறிலி std::string & default_மதிப்பு_2)
{
    திரும்பு பட<cv::String>(name, default_மதிப்பு_2);
}

utils::Paths utils::getConfigurationParameterPaths(மாறிலி எழுத்து* name, மாறிலி utils::Paths &default_மதிப்பு_2)
{
    திரும்பு பட<utils::Paths>(name, default_மதிப்பு_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, புதிய ImplCollector())
}

வெறுமை setImpl(முழு flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

வெறுமை addImpl(முழு flag, மாறிலி எழுத்து* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    எனில்(func) // use lazy collection if name was not specified
    {
        size_t index = getImplData().implCode.size();
        எனில்(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

முழு getImpl(std::vector<முழு> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    திரும்பு getImplData().implFlags; // return actual flags for lazy collection
}

தருக்க useCollection()
{
    திரும்பு getImplData().useCollection;
}

வெறுமை setUseCollection(தருக்க flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

நாமாகாஶ instr
{
தருக்க useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    திரும்பு getInstrumentStruct().useInstr;
#இல்லையெனில்
    திரும்பு பொய்;
#endif
}

வெறுமை setUseInstrumentation(தருக்க flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#இல்லையெனில்
    CV_UNUSED(flag);
#endif
}

Instr_முனை* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    திரும்பு &getInstrumentStruct().வேர்_முனை_2;
#இல்லையெனில்
    திரும்பு வெற்று;
#endif
}

வெறுமை resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().வேர்_முனை_2.removeChilds();
    getInstrumentTLSStruct().p_Current_முனை = &getInstrumentStruct().வேர்_முனை_2;
#endif
}

வெறுமை setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#இல்லையெனில்
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    திரும்பு (FLAGS)getInstrumentStruct().flags;
#இல்லையெனில்
    திரும்பு (FLAGS)0;
#endif
}

முனை_Data::முனை_Data(மாறிலி எழுத்து* funName, மாறிலி எழுத்து* fileName, முழு வரி_இலக்கம், வெறுமை* retAddress, தருக்க alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_வரி_இலக்கம்       = வரி_இலக்கம்;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = பொய்;
}
முனை_Data::முனை_Data(முனை_Data &ref)
{
    *இது = ref;
}
முனை_Data& முனை_Data::operator=(மாறிலி முனை_Data &வலது)
{
    இது->m_funName      = வலது.m_funName;
    இது->m_instrType    = வலது.m_instrType;
    இது->m_implType     = வலது.m_implType;
    இது->m_fileName     = வலது.m_fileName;
    இது->m_வரி_இலக்கம்      = வலது.m_வரி_இலக்கம்;
    இது->m_retAddress   = வலது.m_retAddress;
    இது->m_alwaysExpand = வலது.m_alwaysExpand;

    இது->m_threads     = வலது.m_threads;
    இது->m_counter     = வலது.m_counter;
    இது->m_ticksTotal  = வலது.m_ticksTotal;

    இது->m_funError    = வலது.m_funError;

    திரும்பு *இது;
}
முனை_Data::~முனை_Data()
{
}
தருக்க operator==(மாறிலி முனை_Data& இடது, மாறிலி முனை_Data& வலது)
{
    எனில்(இடது.m_வரி_இலக்கம் == வலது.m_வரி_இலக்கம் && இடது.m_funName == வலது.m_funName && இடது.m_fileName == வலது.m_fileName)
    {
        எனில்(இடது.m_retAddress == வலது.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || இடது.m_alwaysExpand))
            திரும்பு உண்மை;
    }
    திரும்பு பொய்;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    நிலையான InstrStruct instr;
    திரும்பு instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    திரும்பு *getInstrumentStruct().tlsStruct.get();
}

Instr_முனை* get_Current_முனை()
{
    திரும்பு getInstrumentTLSStruct().p_Current_முனை;
}

IntrumentationRegion::IntrumentationRegion(மாறிலி எழுத்து* funName, மாறிலி எழுத்து* fileName, முழு வரி_இலக்கம், வெறுமை *retAddress, தருக்க alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = பொய்;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    எனில்(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        எனில்(!pTLS->p_Current_முனை)
        {
            m_disabled = உண்மை;
            திரும்பு;
        }

        முழு depth = pTLS->p_Current_முனை->getDepth();
        எனில்(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = உண்மை;
            திரும்பு;
        }

        முனை_Data payload(funName, fileName, வரி_இலக்கம், retAddress, alwaysExpand, instrType, implType);
        Node<முனை_Data>* pChild = வெற்று;

        எனில்(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_முனை->findChild(payload);
            எனில்(!pChild)
            {
                pChild = புதிய Node<முனை_Data>(payload);
                pTLS->p_Current_முனை->addChild(pChild);
            }
        }
        இல்லையெனில்
        {
            pChild = pTLS->p_Current_முனை->findChild(payload);
            எனில்(!pChild)
            {
                m_disabled = உண்மை;
                திரும்பு;
            }
        }
        pTLS->p_Current_முனை = pChild;

        m_regionTicks = get_Tick_மொத்தம்();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    எனில்(pStruct->useInstr)
    {
        எனில்(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            எனில் (pTLS->p_Current_முனை->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_முனை->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_முனை->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_மொத்தம்() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_மொத்தம்); // Concurrent ticks accumulation
                pTLS->p_Current_முனை->m_payload.m_counter++;
                pTLS->p_Current_முனை->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_முனை->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_முனை = pTLS->p_Current_முனை->m_pParent;
        }
    }
}
#endif
}

நாமாகாஶ ipp
{

#ifdef HAVE_IPP
கட்டமைப்பு IPPInitSingleton
{
பொது:
    IPPInitSingleton()
    {
        useIPP         = உண்மை;

#எனில் defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#இல்லையெனில்
        useIPP_NE      = பொய்;
#endif
        ippStatus      = 0;
        funcname       = வெற்று;
        filename       = வெற்று;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_உச்சி_Features = 0;
        pIppLibInfo    = வெற்று;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, வெற்று);
        எனில்(ippStatus < 0)
        {
            CV_LOG_ERROR(வெற்று, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = பொய்;
            திரும்பு;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        எனில்(!env.empty())
        {
#எனில் IPP_VERSION_X100 >= 201900
            மாறிலி Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#அல்லதெனில் IPP_VERSION_X100 >= 201703
            மாறிலி Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#அல்லதெனில் IPP_VERSION_X100 >= 201700
            மாறிலி Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#இல்லையெனில்
            மாறிலி Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            எனில்(env.substr(0, 2) == "ne")
            {
                useIPP_NE = உண்மை;
                env = env.substr(3, env.size());
            }

            எனில்(env == "disabled")
            {
                CV_LOG_WARNING(வெற்று, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = பொய்;
            }
            இல்லையெனில் எனில்(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            இல்லையெனில் எனில்(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#எனில் IPP_VERSION_X100 >= 201700
#எனில் defined (_M_AMD64) || defined (__x86_64__)
            இல்லையெனில் எனில்(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            இல்லையெனில்
                CV_LOG_ERROR(வெற்று, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        எனில்(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        எனில்(!(
#எனில் IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = பொய்;
            திரும்பு;
        }

        எனில்(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        இல்லையெனில்
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#எனில் IPP_VERSION_X100 >= 201700
        எனில்(ippFeatures&ippCPUID_AVX512F)
        {
            எனில்((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_உச்சி_Features = ippCPUID_AVX512_SKX;
            இல்லையெனில் எனில்((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_உச்சி_Features = ippCPUID_AVX512_KNL;
            இல்லையெனில்
                ipp_உச்சி_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        இல்லையெனில்
#endif
        எனில்(ippFeatures&ippCPUID_AVX2)
            ipp_உச்சி_Features = ippCPUID_AVX2;
        இல்லையெனில் எனில்(ippFeatures&ippCPUID_SSE42)
            ipp_உச்சி_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        எனில் (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_உச்சி_Features = ippCPUID_SSE42;
        }
    }

பொது:
    தருக்க        useIPP;
    தருக்க        useIPP_NE;

    முழு         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    மாறிலி எழுத்து *funcname;
    மாறிலி எழுத்து *filename;
    முழு         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_உச்சி_Features;
    மாறிலி IppLibraryVersion *pIppLibInfo;
};

நிலையான IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, புதிய IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    திரும்பு getIPPSingleton().ippFeatures;
#இல்லையெனில்
    திரும்பு 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_உச்சி_Features()
{
    திரும்பு getIPPSingleton().ipp_உச்சி_Features;
}
#endif

வெறுமை setIppStatus(முழு status, மாறிலி எழுத்து * மாறிலி _funcname, மாறிலி எழுத்து * மாறிலி _filename, முழு வரி_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = வரி_3;
#இல்லையெனில்
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(வரி_3);
#endif
}

முழு getIppStatus()
{
#ifdef HAVE_IPP
    திரும்பு getIPPSingleton().ippStatus;
#இல்லையெனில்
    திரும்பு 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    திரும்பு format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#இல்லையெனில்
    திரும்பு String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    மாறிலி IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    எனில்(pInfo)
        திரும்பு format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    இல்லையெனில்
        திரும்பு String("error");
#இல்லையெனில்
    திரும்பு String("disabled");
#endif
}

தருக்க useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    எனில் (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    திரும்பு (data.useIPP > 0);
#இல்லையெனில்
    திரும்பு பொய்;
#endif
}

வெறுமை setUseIPP(தருக்க flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:பொய்;
#இல்லையெனில்
    CV_UNUSED(flag);
    data.useIPP = பொய்;
#endif
}

தருக்க useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    எனில் (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    திரும்பு (data.useIPP_NE > 0);
#இல்லையெனில்
    திரும்பு பொய்;
#endif
}

வெறுமை setUseIPP_NotExact(தருக்க flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#இல்லையெனில்
    CV_UNUSED(flag);
    data.useIPP_NE = பொய்;
#endif
}

} // namespace ipp


நாமாகாஶ details {

#எனில் OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

வெறுமை setFPDenormalsIgnoreHint(தருக்க ignore, CV_OUT FPDenormalsModeState& state)
{
#எனில் OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    எனில் (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    மாறிலி unsigned old_flags = _mm_getcsr();
    மாறிலி unsigned old_மதிப்பு = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(வெற்று, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_மதிப்பு;
    _mm_setcsr(flags);
#இல்லையெனில்
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

முழு saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#எனில் OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    எனில் (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    மாறிலி unsigned old_flags = _mm_getcsr();
    மாறிலி unsigned old_மதிப்பு = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_மதிப்பு;
    திரும்பு 2;
#இல்லையெனில்
    CV_UNUSED(state);
    திரும்பு 0;
#endif
}

தருக்க restoreFPDenormalsState(மாறிலி FPDenormalsModeState& state)
{
#எனில் OPENCV_IMPL_FP_HINTS_X86
    மாறிலி unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    மாறிலி unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((முழு)value, value == (value & mask), "invalid SSE FP state");
    மாறிலி unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(வெற்று, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    திரும்பு உண்மை;
#இல்லையெனில்
    CV_UNUSED(state);
    திரும்பு பொய்;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    திரும்பு OPENCV_ALGO_HINT_DEFAULT;
#இல்லையெனில்
    திரும்பு ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
