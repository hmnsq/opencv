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

#অন্তর্ভাব "precomp.hpp"
//#include "opencv2/core/core_c.h"
#অন্তর্ভাব <atomic>
#অন্তর্ভাব <exception>
#অন্তর্ভাব <iostream>
#অন্তর্ভাব <ostream>

#ifdef __QNX__
    #অন্তর্ভাব <unistd.h>
    #অন্তর্ভাব <sys/neutrino.h>
    #অন্তর্ভাব <sys/syspage.h>
#ifdef __aarch64__
    #অন্তর্ভাব <aarch64/syspage.h>
#endif
#endif

#অন্তর্ভাব <opencv2/core/utils/configuration.private.hpp>
#অন্তর্ভাব <opencv2/core/utils/trace.private.hpp>

#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#অন্তর্ভাব <opencv2/core/utils/tls.hpp>
#অন্তর্ভাব <opencv2/core/utils/instrumentation.hpp>

#অন্তর্ভাব <opencv2/core/utils/filesystem.private.hpp>

#অন্তর্ভাব <opencv2/core/utils/fp_control_utils.hpp>
#অন্তর্ভাব <opencv2/core/utils/fp_control.private.hpp>

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

স্থির Mutex* __initialization_mutex = ফাঁকা;
Mutex& getInitializationMutex()
{
    যদি (__initialization_mutex == ফাঁকা)
    {
        (শূন্য)_initSystem();
        __initialization_mutex = নতুন Mutex();
    }
    ফেরত *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

স্থির যৌক্তিক param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#যদি defined(_DEBUG) || defined(__ANDROID__)
    সত্য
#অন্যথা
    মিথ্যা
#endif
);

শূন্য* alloc_Singleton_বাফার(size_t size) { ফেরত fastMalloc(size); }
শূন্য* alloc_Singleton_New_বাফার(size_t size) { ফেরত malloc(size); }


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
#অন্তর্ভাব <cstdlib>        // std::abort
#endif

#যদি defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#যদি defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ধ্রুবক uint64_t AT_HWCAP = NT_GNU_HWCAP;
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
#অন্তর্ভাব "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#যদি defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#অন্তর্ভাব <windows.h>
#অন্তর্ভাব <combaseapi.h>
#যদি (_WIN32_WINNT >= 0x0602)
  #অন্তর্ভাব <synchapi.h>
#endif
#যদি ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #অন্তর্ভাব <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef abs
#অন্তর্ভাব <tchar.h>

#ifdef WINRT
#অন্তর্ভাব <wrl/client.h>
#ifndef __cplusplus_winrt
#অন্তর্ভাব <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring Get_অস্থায়ী_Path_Win_RT()
{
#ifdef __cplusplus_winrt
    ফেরত std::wstring(Windows::Storage::Application_তথ্য::Current->TemporaryFolder->Path->তথ্য_4());
#অন্যথা
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_তথ্য_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_তথ্য> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstr_শীর্ষ;
    std::wstring wstr;
    যদি (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_তথ্য,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_তথ্য), &hstr_শীর্ষ, &str)))
        ফেরত wstr;
    যদি (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        ফেরত wstr;
    যদি (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        ফেরত wstr;
    যদি (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        ফেরত wstr;
    যদি (FAILED(storagefolderRef.As(&storageitemRef)))
        ফেরত wstr;
    str = ফাঁকা;
    যদি (FAILED(storageitemRef->get_Path(&str)))
        ফেরত wstr;
    wstr = Windows_Get_String_Raw_বাফার(str, ফাঁকা);
    WindowsDeleteString(str);
    ফেরত wstr;
#endif
}

std::wstring Get_অস্থায়ী_File_Name_Win_RT(std::wstring prefix)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], আকার(guidStr)/আকার(wchar_t), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ফেরত prefix.append(std::wstring(guidStr));
}

#endif
#অন্যথা
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#অন্তর্ভাব <pthread.h>
#endif
#অন্তর্ভাব <sys/time.h>
#অন্তর্ভাব <time.h>

#যদি defined __MACH__ && defined __APPLE__
#অন্তর্ভাব <mach/mach.h>
#অন্তর্ভাব <mach/mach_time.h>
#অন্তর্ভাব <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#অন্তর্ভাব "omp.h"
#endif

#যদি defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <sys/types.h>
#যদি defined __ANDROID__
#অন্তর্ভাব <sys/sysconf.h>
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
      #অন্তর্ভাব <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #অন্যথা
      #error "Required MSVS 2005+"
    #endif
  #নতুবা_যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    স্থির শূন্য cv_cpuid(পূর্ণসংখ্যা* cpuid_তথ্য, পূর্ণসংখ্যা reg_eax, পূর্ণসংখ্যা reg_ecx)
    {
        পূর্ণসংখ্যা __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#যদি !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#নতুবা_যদি defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#অন্যথা
#error "Configuration error"
#endif
        cpuid_তথ্য[0] = __eax; cpuid_তথ্য[1] = __ebx; cpuid_তথ্য[2] = __ecx; cpuid_তথ্য[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#অন্তর্ভাব <chrono>

নামাকাশ cv
{

Exception::Exception() { code = Error::StsOk; সারি = 0; }

Exception::Exception(Error::Code _code, ধ্রুবক String& _err, ধ্রুবক String& _func, ধ্রুবক String& _file, পূর্ণসংখ্যা সারি_3)
: code(_code), err(_err), func(_func), file(_file), সারি(সারি_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ধ্রুবক অক্ষর* Exception::what() ধ্রুবক CV_NOEXCEPT { ফেরত বার্তা.c_str(); }

শূন্য Exception::formatMessage()
{
    size_t pos = err.find('\n');
    যৌক্তিক multiline = pos != cv::String::npos;
    যদি (multiline)
    {
        std::stringstream ss;
        size_t পূর্ব_pos = 0;
        যখন (pos != cv::String::npos)
        {
           ss << "> " << err.substr(পূর্ব_pos, pos - পূর্ব_pos) << std::endl;
           পূর্ব_pos = pos + 1;
           pos = err.find('\n', পূর্ব_pos);
        }
        ss << "> " << err.substr(পূর্ব_pos);
        যদি (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    যদি (func.size() > 0)
    {
        যদি (multiline)
            বার্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), সারি, code, codeMessage(), func.c_str(), err.c_str());
        অন্যথা
            বার্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), সারি, code, codeMessage(), err.c_str(), func.c_str());
    }
    অন্যথা
    {
        বার্তা = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), সারি, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

ধ্রুবক অক্ষর * Exception::codeMessage() ধ্রুবক
{
    নির্বাচন (code)
    {
    ক্ষেত্র cv::Error::StsOk :                  ফেরত "No Error";
    ক্ষেত্র cv::Error::StsBackTrace :           ফেরত "Backtrace";
    ক্ষেত্র cv::Error::StsError :               ফেরত "Unspecified error";
    ক্ষেত্র cv::Error::StsInternal :            ফেরত "Internal error";
    ক্ষেত্র cv::Error::StsNoMem :               ফেরত "Insufficient memory";
    ক্ষেত্র cv::Error::StsBadArg :              ফেরত "Bad argument";
    ক্ষেত্র cv::Error::StsNoConv :              ফেরত "Iterations do not converge";
    ক্ষেত্র cv::Error::StsAutoTrace :           ফেরত "Autotrace call";
    ক্ষেত্র cv::Error::Sts_Bad_আকার :             ফেরত "Incorrect size of input array";
    ক্ষেত্র cv::Error::StsNullPtr :             ফেরত "Null pointer";
    ক্ষেত্র cv::Error::StsDivByZero :           ফেরত "Division by zero occurred";
    ক্ষেত্র cv::Error::BadStep :                ফেরত "Image step is wrong";
    ক্ষেত্র cv::Error::StsInplaceNotSupported : ফেরত "Inplace operation is not supported";
    ক্ষেত্র cv::Error::StsObjectNotFound :      ফেরত "Requested object was not found";
    ক্ষেত্র cv::Error::BadDepth :               ফেরত "Input image depth is not supported by function";
    ক্ষেত্র cv::Error::StsUnmatchedFormats :    ফেরত "Formats of input arguments do not match";
    ক্ষেত্র cv::Error::StsUnmatchedSizes :      ফেরত "Sizes of input arguments do not match";
    ক্ষেত্র cv::Error::StsOutOfRange :          ফেরত "One of the arguments\' values is out of range";
    ক্ষেত্র cv::Error::StsUnsupportedFormat :   ফেরত "Unsupported format or combination of formats";
    ক্ষেত্র cv::Error::BadCOI :                 ফেরত "Input COI is not supported";
    ক্ষেত্র cv::Error::Bad_অঙ্ক_Channels :         ফেরত "Bad number of channels";
    ক্ষেত্র cv::Error::StsBadFlag :             ফেরত "Bad flag (parameter or structure field)";
    ক্ষেত্র cv::Error::StsBadPoint :            ফেরত "Bad parameter of type CvPoint";
    ক্ষেত্র cv::Error::StsBadMask :             ফেরত "Bad type of mask argument";
    ক্ষেত্র cv::Error::StsParseError :          ফেরত "Parsing error";
    ক্ষেত্র cv::Error::StsNotImplemented :      ফেরত "The function/feature is not implemented";
    ক্ষেত্র cv::Error::StsBadMemBlock :         ফেরত "Memory block has been corrupted";
    ক্ষেত্র cv::Error::StsAssert :              ফেরত "Assertion failed";
    ক্ষেত্র cv::Error::GpuNotSupported :        ফেরত "No CUDA support";
    ক্ষেত্র cv::Error::GpuApiCallError :        ফেরত "Gpu API call";
    ক্ষেত্র cv::Error::OpenGlNotSupported :     ফেরত "No OpenGL support";
    ক্ষেত্র cv::Error::OpenGlApiCallError :     ফেরত "OpenGL API call";
    ক্ষেত্র cv::Error::OpenCLApiCallError :     ফেরত "OpenCL API call error";
    ক্ষেত্র cv::Error::OpenCLDoubleNotSupported:ফেরত "OpenCL device does not support double";
    ক্ষেত্র cv::Error::OpenCLInitError :        ফেরত "OpenCL initialization error";
    ক্ষেত্র cv::Error::OpenCLNoAMDBlasFft :     ফেরত "Missing OpenCL AMD BLAS FFT";
    পূর্বনির্ধারিত:
    {
        স্থির অক্ষর buf[256] {0};
        snprintf(buf, আকার(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        ফেরত buf;
    }
    };

}

স্থির ধ্রুবক অক্ষর* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { ফাঁকা };

স্থির ধ্রুবক অক্ষর* getHWFeatureName(পূর্ণসংখ্যা id)
{
    ফেরত (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : ফাঁকা;
}
স্থির ধ্রুবক অক্ষর* getHWFeatureNameSafe(পূর্ণসংখ্যা id)
{
    ধ্রুবক অক্ষর* name = getHWFeatureName(id);
    ফেরত name ? name : "Unknown feature";
}

কাঠামো HWFeatures
{
    গণনা { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(যৌক্তিক run_initialize = মিথ্যা)
    {
        যদি (run_initialize)
            initialize();
    }

    স্থির শূন্য initializeNames()
    {
        জন্য (পূর্ণসংখ্যা i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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
        পূর্ণসংখ্যা cpuid_তথ্য[4] = { 0, 0, 0, 0 };
        পূর্ণসংখ্যা cpuid_তথ্য_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_তথ্য, 1, 0/*unused*/);

        পূর্ণসংখ্যা x86_family = (cpuid_তথ্য[0] >> 8) & 15;
        যদি( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_তথ্য[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_তথ্য[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_তথ্য[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_তথ্য[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_তথ্য[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_তথ্য[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_তথ্য[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_তথ্য[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_তথ্য[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_তথ্য[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_তথ্য[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_তথ্য_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_তথ্য_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_তথ্য_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_তথ্য_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_তথ্য_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_তথ্য_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_তথ্য_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_তথ্য_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_তথ্য_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_তথ্য_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_তথ্য_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_তথ্য_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_তথ্য_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_তথ্য_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_তথ্য_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_তথ্য_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_তথ্য_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            পূর্ণসংখ্যা cpuid_তথ্য_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_তথ্য_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_তথ্য_ex1[0] & (1<<4))  != 0;

            যৌক্তিক have_AVX_OS_support = সত্য;
            যৌক্তিক have_AVX512_OS_support = সত্য;
            যদি (!(cpuid_তথ্য[2] & (1<<27)))
                have_AVX_OS_support = মিথ্যা; // OS uses XSAVE_XRSTORE and CPU support AVX
            অন্যথা
            {
                পূর্ণসংখ্যা xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (পূর্ণসংখ্যা)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #নতুবা_যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                যদি ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = মিথ্যা; // YMM registers
                যদি ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = মিথ্যা; // ZMM registers
            }

            যদি (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = মিথ্যা;
                have[CV_CPU_FP16] = মিথ্যা;
                have[CV_CPU_AVX2] = মিথ্যা;
                have[CV_CPU_FMA3] = মিথ্যা;
                have[CV_CPU_AVX_VNNI] = মিথ্যা;
            }
            যদি (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = মিথ্যা;
                have[CV_CPU_AVX_512BW] = মিথ্যা;
                have[CV_CPU_AVX_512CD] = মিথ্যা;
                have[CV_CPU_AVX_512DQ] = মিথ্যা;
                have[CV_CPU_AVX_512ER] = মিথ্যা;
                have[CV_CPU_AVX_512IFMA] = মিথ্যা;
                have[CV_CPU_AVX_512PF] = মিথ্যা;
                have[CV_CPU_AVX_512VBMI] = মিথ্যা;
                have[CV_CPU_AVX_512VL] = মিথ্যা;
                have[CV_CPU_AVX_512VBMI2] = মিথ্যা;
                have[CV_CPU_AVX_512VNNI] = মিথ্যা;
                have[CV_CPU_AVX_512BITALG] = মিথ্যা;
                have[CV_CPU_AVX_512VPOPCNTDQ] = মিথ্যা;
                have[CV_CPU_AVX_5124VNNIW] = মিথ্যা;
                have[CV_CPU_AVX_5124FMAPS] = মিথ্যা;
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
                have[CV_CPU_AVX512_KNL] = মিথ্যা;
                have[CV_CPU_AVX512_KNM] = মিথ্যা;
                have[CV_CPU_AVX512_SKX] = মিথ্যা;
                have[CV_CPU_AVX512_CNL] = মিথ্যা;
                have[CV_CPU_AVX512_CLX] = মিথ্যা;
                have[CV_CPU_AVX512_ICL] = মিথ্যা;
            }
        }
    #endif // CV_CPUID_X86

    #যদি defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = সত্য;
        have[CV_CPU_FP16] = সত্য;
        পূর্ণসংখ্যা cpufile = open("/proc/self/auxv", O_RDONLY);

        যদি (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ধ্রুবক size_t আকার_auxv_t = আকার(auxv);

            যখন ((size_t)পঠ(cpufile, &auxv, আকার_auxv_t) == আকার_auxv_t)
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
    #নতুবা_যদি defined __arm__ && defined __ANDROID__
      #যদি defined HAVE_CPUFEATURES
        CV_LOG_INFO(ফাঁকা, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ফাঁকা, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (long long)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #অন্যথা
        CV_LOG_INFO(ফাঁকা, "cpufeatures library is not available for CPU detection");
        #যদি CV_NEON
        CV_LOG_INFO(ফাঁকা, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = সত্য;
        #অন্যথা
        CV_LOG_INFO(ফাঁকা, "- NEON instructions is NOT enabled via build flags");
        #endif
        #যদি CV_FP16
        CV_LOG_INFO(ফাঁকা, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = সত্য;
        #অন্যথা
        CV_LOG_INFO(ফাঁকা, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #নতুবা_যদি defined __arm__
        পূর্ণসংখ্যা cpufile = open("/proc/self/auxv", O_RDONLY);

        যদি (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ধ্রুবক size_t আকার_auxv_t = আকার(auxv);

            যখন ((size_t)পঠ(cpufile, &auxv, আকার_auxv_t) == আকার_auxv_t)
            {
                যদি (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    বিরতি;
                }
            }

            close(cpufile);
        }
    #endif
    #নতুবা_যদি (defined __APPLE__)
    #যদি defined __ARM_NEON
        have[CV_CPU_NEON] = সত্য;
    #endif
    #যদি (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = সত্য;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    পূর্ণসংখ্যা has_feat_dotprod = 0;
    size_t has_feat_dotprod_আকার = আকার(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_আকার, ফাঁকা, 0);
    যদি (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = সত্য;
    }
    পূর্ণসংখ্যা has_feat_bf16 = 0;
    size_t has_feat_bf16_আকার = আকার(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_আকার, ফাঁকা, 0);
    যদি (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = সত্য;
    }
    #নতুবা_যদি (defined __clang__)
    #যদি defined __ARM_NEON
        have[CV_CPU_NEON] = সত্য;
        #যদি (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = সত্য;
        #endif
    #endif
    #endif
    #যদি defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = সত্য;
    #endif
    #যদি defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = সত্য;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = সত্য;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = সত্য;
    #endif

    #যদি (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        unsigned পূর্ণসংখ্যা hwcap = getauxval(AT_HWCAP);
        যদি (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            যদি (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = সত্য;
            } অন্যথা {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #নতুবা_যদি (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        unsigned long hwcap = 0, hwcap2 = 0;
        যদি (elf_aux_info(AT_HWCAP, &hwcap, আকার(hwcap)) != 0)
            hwcap = 0;
        যদি (hwcap & PPC_FEATURE_HAS_VSX) {
            যদি (elf_aux_info(AT_HWCAP2, &hwcap2, আকার(hwcap2)) != 0)
                hwcap2 = 0;
            যদি (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = সত্য;
            } অন্যথা {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #অন্যথা
        // TODO: AIX
        #যদি CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = সত্য;
        #endif
        #যদি CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = সত্য;
        #endif
    #endif

    #যদি defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = সত্য;
        #যদি (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = সত্য;
        #endif
        #যদি defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = সত্য;
        #endif
    #endif

    #যদি defined __loongarch64 && defined __linux__
        পূর্ণসংখ্যা flag = (পূর্ণসংখ্যা)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        যৌক্তিক skip_baseline_check = মিথ্যা;
        যদি (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = সত্য;
        }
        পূর্ণসংখ্যা baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        যদি (!checkFeatures(baseline_features, আকার(baseline_features) / আকার(baseline_features[0]))
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
            checkFeatures(baseline_features, আকার(baseline_features) / আকার(baseline_features[0]), সত্য);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, আকার(baseline_features) / আকার(baseline_features[0]));
    }

    যৌক্তিক checkFeatures(ধ্রুবক পূর্ণসংখ্যা* features, পূর্ণসংখ্যা গুনতি_2, যৌক্তিক dump = মিথ্যা)
    {
        যৌক্তিক ফলাফল = সত্য;
        জন্য (পূর্ণসংখ্যা i = 0; i < গুনতি_2; i++)
        {
            পূর্ণসংখ্যা feature = features[i];
            যদি (feature)
            {
                যদি (have[feature])
                {
                    যদি (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                অন্যথা
                {
                    ফলাফল = মিথ্যা;
                    যদি (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        ফেরত ফলাফল;
    }

    স্থির অন্তঃরেখ যৌক্তিক is_প্রতীক_Separator(অক্ষর c)
    {
        ফেরত c == ',' || c == ';';
    }

    শূন্য readSettings(ধ্রুবক পূর্ণসংখ্যা* baseline_features, পূর্ণসংখ্যা baseline_গুনতি)
    {
        যৌক্তিক dump = সত্য;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        যদি (!disabled_features.empty())
        {
            ধ্রুবক অক্ষর* start = disabled_features.c_str();
            জন্য (;;)
            {
                যখন (start[0] != 0 && is_প্রতীক_Separator(start[0]))
                {
                    start++;
                }
                যদি (start[0] == 0)
                    বিরতি;
                ধ্রুবক অক্ষর* end = start;
                যখন (end[0] != 0 && !is_প্রতীক_Separator(end[0]))
                {
                    end++;
                }
                যদি (end == start)
                    চালু;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                যৌক্তিক found = মিথ্যা;
                জন্য (পূর্ণসংখ্যা i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    যদি (!g_hwFeatureNames[i]) চালু;
                    size_t len = strlen(g_hwFeatureNames[i]);
                    যদি (len != feature.size()) চালু;
                    যদি (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        যৌক্তিক isBaseline = মিথ্যা;
                        জন্য (পূর্ণসংখ্যা k = 0; k < baseline_গুনতি; k++)
                        {
                            যদি (baseline_features[k] == i)
                            {
                                isBaseline = সত্য;
                                বিরতি;
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
                        have[i] = মিথ্যা;

                        found = সত্য;
                        বিরতি;
                    }
                }
                যদি (!found)
                {
                    যদি (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    যৌক্তিক have[MAX_FEATURE+1]{};
};

স্থির HWFeatures  featuresEnabled(সত্য), featuresDisabled = HWFeatures(মিথ্যা);
স্থির HWFeatures* currentFeatures = &featuresEnabled;

যৌক্তিক checkHardwareSupport(পূর্ণসংখ্যা feature)
{
    যদি (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ফেরত মিথ্যা;
    ফেরত currentFeatures->have[feature];
}

String getHardwareFeatureName(পূর্ণসংখ্যা feature)
{
    ধ্রুবক অক্ষর* name = getHWFeatureName(feature);
    ফেরত name ? String(name) : String();
}

std::string get_CPUFeatures_সারি()
{
    ধ্রুবক পূর্ণসংখ্যা features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ধ্রুবক পূর্ণসংখ্যা sz = আকার(features) / আকার(features[0]);
    std::string ফলাফল;
    std::string prefix;
    জন্য (পূর্ণসংখ্যা i = 1; i < sz; ++i)
    {
        যদি (features[i] == 0)
        {
            prefix = "*";
            চালু;
        }
        যদি (i != 1) ফলাফল.append(" ");
        ফলাফল.append(prefix);
        ফলাফল.append(getHWFeatureNameSafe(features[i]));
        যদি (!checkHardwareSupport(features[i])) ফলাফল.append("?");
    }
    ফেরত ফলাফল;
}

অস্থির যৌক্তিক useOptimizedFlag = সত্য;

শূন্য setUseOptimized( যৌক্তিক flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

যৌক্তিক useOptimized(শূন্য)
{
    ফেরত useOptimizedFlag;
}

int64 get_Tick_গুনতি(শূন্য)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ফেরত (int64)now.time_since_epoch().গুনতি_2();
}

দ্বিগুণ getTickFrequency(শূন্য)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    দ্বিগুণ clock_freq = clock_period_t::den / clock_period_t::num;
    ফেরত clock_freq;
}

#যদি defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#যদি defined(__i386__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    int64 x;
    __asm__ অস্থির (".byte 0x0f, 0x31" : "=A" (x));
    ফেরত x;
}
#নতুবা_যদি defined(__x86_64__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ফেরত (int64)lo | ((int64)hi << 32);
}

#নতুবা_যদি defined(__ppc__)

int64 get_CPUTick_গুনতি(শূন্য)
{
    unsigned upper, lower, tmp;
    __asm__ অস্থির(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ফেরত lower | ((int64)upper << 32);
}

#অন্যথা

#error "RDTSC not defined"

#endif

#নতুবা_যদি defined _MSC_VER && defined _WIN32 && defined _M_IX86

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
    ফেরত get_Tick_গুনতি();
}
//#endif

#endif


নামাকাশ internal {

শ্রেণী Timestamp
{
সর্বজনীন:
    ধ্রুবক int64 zero_Tick_গুনতি;
    ধ্রুবক দ্বিগুণ ns_in_ticks;

    Timestamp()
        : zero_Tick_গুনতি(get_Tick_গুনতি())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_গুনতি();
        ফেরত (int64)((t - zero_Tick_গুনতি) * ns_in_ticks);
    }

    স্থির Timestamp& getInstance()
    {
        স্থির Timestamp g_timestamp;
        ফেরত g_timestamp;
    }
};

শ্রেণী InitTimestamp {
সর্বজনীন:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
স্থির InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ফেরত internal::Timestamp::getInstance().getTimestamp();
}


ধ্রুবক String& getBuildInformation()
{
    স্থির String build_info =
#অন্তর্ভাব "version_string.inc"
    ;
    ফেরত build_info;
}

String getVersionString() { ফেরত String(CV_VERSION); }

পূর্ণসংখ্যা getVersionMajor() { ফেরত CV_VERSION_MAJOR; }

পূর্ণসংখ্যা getVersionMinor() { ফেরত CV_VERSION_MINOR; }

পূর্ণসংখ্যা getVersionRevision() { ফেরত CV_VERSION_REVISION; }

String format( ধ্রুবক অক্ষর* fmt, ... )
{
    Auto_বাফার<অক্ষর, 1024> buf;

    জন্য ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        পূর্ণসংখ্যা bsize = static_cast<পূর্ণসংখ্যা>(buf.size());
        পূর্ণসংখ্যা len = cv_vsnprintf(buf.তথ্য(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        যদি (len >= bsize)
        {
            buf.resize(len + 1);
            চালু;
        }
        buf[bsize - 1] = 0;
        ফেরত String(buf.তথ্য(), len);
    }
}

String tempfile( ধ্রুবক অক্ষর* suffix )
{
#যদি OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string অস্থায়ী_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#যদি defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring অস্থায়ী_dir_rt = Get_অস্থায়ী_Path_Win_RT();

    std::wstring অস্থায়ী_file = Get_অস্থায়ী_File_Name_Win_RT(L"ocv");
    যদি (অস্থায়ী_file.empty())
        ফেরত String();

    অস্থায়ী_file = অস্থায়ী_dir_rt.append(std::wstring(L"\\")).append(অস্থায়ী_file);
    DeleteFileW(অস্থায়ী_file.c_str());

    অক্ষর aname[MAX_PATH];
    size_t copied = wcstombs(aname, অস্থায়ী_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#নতুবা_যদি defined(_WIN32_WCE)
    ধ্রুবক auto k_Max_Path_আকার = MAX_PATH+1;
    wchar_t অস্থায়ী_dir_ce[k_Max_Path_আকার] = {0};
    wchar_t অস্থায়ী_file[k_Max_Path_আকার] = {0};

    ::Get_অস্থায়ী_Path_W(k_Max_Path_আকার, অস্থায়ী_dir_ce);

    যদি(0 != ::Get_অস্থায়ী_File_Name_W(অস্থায়ী_dir_ce, L"ocv", 0, অস্থায়ী_file)) {
        DeleteFileW(অস্থায়ী_file);
        অক্ষর aname[MAX_PATH];
        size_t copied = wcstombs(aname, অস্থায়ী_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (size_t)-1));
        fname = String(aname);
    }
#অন্যথা
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t অস্থায়ী_dir2_w[MAX_PATH] = { 0 };

    যদি (অস্থায়ী_dir.empty())
    {
        ::Get_অস্থায়ী_Path_W(আকার(অস্থায়ী_dir2_w)/আকার(wchar_t), অস্থায়ী_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        পূর্ণসংখ্যা len = WideCharToMultiByte(CP_UTF8, 0, অস্থায়ী_dir2_w, -1, ফাঁকা, 0, ফাঁকা, ফাঁকা);
        যদি (len > 0)
        {
            std::vector<অক্ষর> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, অস্থায়ী_dir2_w, -1, utf8_buf.তথ্য(), len, ফাঁকা, ফাঁকা);
            অস্থায়ী_dir = std::string(utf8_buf.তথ্য());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    যদি (FAILED(hr))
        ফেরত String();
    অক্ষর guidStr[40];
    ধ্রুবক অক্ষর* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, আকার(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (unsigned পূর্ণসংখ্যা)g.Data4[0], (unsigned পূর্ণসংখ্যা)g.Data4[1],
            (unsigned পূর্ণসংখ্যা)g.Data4[2], (unsigned পূর্ণসংখ্যা)g.Data4[3], (unsigned পূর্ণসংখ্যা)g.Data4[4],
            (unsigned পূর্ণসংখ্যা)g.Data4[5], (unsigned পূর্ণসংখ্যা)g.Data4[6], (unsigned পূর্ণসংখ্যা)g.Data4[7]);

    fname = অস্থায়ী_dir;
    যদি (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# অন্যথা
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    অক্ষর defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  অন্যথা
    অক্ষর defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    যদি (অস্থায়ী_dir.empty())
        fname = defaultTemplate;
    অন্যথা
    {
        fname = অস্থায়ী_dir;
        অক্ষর ech = fname[fname.size() - 1];
        যদি(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ধ্রুবক পূর্ণসংখ্যা fd = mkstemp((অক্ষর*)fname.c_str());
    যদি (fd == -1) ফেরত String();

    close(fd);
    remove(fname.c_str());
# endif

    যদি (suffix)
    {
        যদি (suffix[0] != '.')
            ফেরত fname + "." + suffix;
        অন্যথা
            ফেরত fname + suffix;
    }
    ফেরত fname;
#অন্যথা // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

স্থির ErrorCallback customErrorCallback = 0;
স্থির শূন্য* custom_Error_Callback_তথ্য = 0;
স্থির যৌক্তিক breakOnError = মিথ্যা;

যৌক্তিক setBreakOnError(যৌক্তিক value)
{
    যৌক্তিক পূর্ব_Val = breakOnError;
    breakOnError = value;
    ফেরত পূর্ব_Val;
}

পূর্ণসংখ্যা cv_snprintf(অক্ষর* buf, পূর্ণসংখ্যা len, ধ্রুবক অক্ষর* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    পূর্ণসংখ্যা res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ফেরত res;
}

পূর্ণসংখ্যা cv_vsnprintf(অক্ষর* buf, পূর্ণসংখ্যা len, ধ্রুবক অক্ষর* fmt, va_list args)
{
#যদি defined _MSC_VER
    যদি (len <= 0) ফেরত len == 0 ? 1024 : -1;
    পূর্ণসংখ্যা res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    যদি (res >= 0 && res < len)
    {
        buf[res] = 0;
        ফেরত res;
    }
    অন্যথা
    {
        buf[len - 1] = 0; // truncate happened
        ফেরত res >= len ? res : (len * 2);
    }
#অন্যথা
    ফেরত vsnprintf(buf, len, fmt, args);
#endif
}

স্থির শূন্য dumpException(ধ্রুবক Exception& exc)
{
    ধ্রুবক অক্ষর* errorStr = exc.codeMessage();
    অক্ষর buf[1 << 12];

    cv_snprintf(buf, আকার(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.সারি);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#অন্যথা
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
স্থির যৌক্তিক cv_terminate_handler_installed = মিথ্যা;
স্থির std::terminate_handler cv_old_terminate_handler;
স্থির cv::Exception cv_terminate_handler_exception;
স্থির যৌক্তিক param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", সত্য);
স্থির শূন্য cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    যদি (মিথ্যা /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
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

শূন্য error( ধ্রুবক Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        যদি (!cv_terminate_handler_installed)
        {
            যদি (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = সত্য;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    যদি (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.সারি, custom_Error_Callback_তথ্য);
    অন্যথা যদি (param_dumpErrors)
    {
        dumpException(exc);
    }

    যদি(breakOnError)
    {
        std::terminate();
    }

    ছোড়ো exc;
#ifdef __GNUC__
# যদি !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

শূন্য error(Error::Code _code, ধ্রুবক String& _err, ধ্রুবক অক্ষর* _func, ধ্রুবক অক্ষর* _file, পূর্ণসংখ্যা সারি_3)
{
    error(cv::Exception(_code, _err, _func, _file, সারি_3));
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
redirectError( ErrorCallback errCallback, শূন্য* userdata, শূন্য** পূর্ব_Userdata)
{
    যদি( পূর্ব_Userdata )
        *পূর্ব_Userdata = custom_Error_Callback_তথ্য;

    ErrorCallback পূর্ব_Callback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_তথ্য = userdata;

    ফেরত পূর্ব_Callback;
}

শূন্য terminate(Error::Code _code, ধ্রুবক String& _err, ধ্রুবক অক্ষর* _func, ধ্রুবক অক্ষর* _file, পূর্ণসংখ্যা সারি_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, সারি_3));
    std::terminate();
}

}

নামাকাশ cv {
যৌক্তিক __termination = মিথ্যা;


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
শ্রেণী TlsAbstraction
{
সর্বজনীন:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    শূন্য* get_তথ্য() ধ্রুবক;
    শূন্য set_তথ্য(শূন্য *p_তথ্য);

    শূন্য releaseSystemResources();

ব্যক্তিগত:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_চাবি;
    যৌক্তিক disposed;
#endif
#অন্যথা // _WIN32
    pthread_key_t  tls_চাবি;
    std::atomic<যৌক্তিক> disposed;
#endif
};

শ্রেণী TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
সর্বজনীন:
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
    স্থির TlsAbstraction *g_tls = নতুন TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    স্থির TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ফেরত g_tls;
}


#ifdef _WIN32
#ifdef WINRT
স্থির __declspec( thread ) শূন্য* tls_তথ্য = ফাঁকা; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
শূন্য TlsAbstraction::releaseSystemResources()
{
    cv::__termination = সত্য;  // DllMain is missing in static builds
}
শূন্য* TlsAbstraction::get_তথ্য() ধ্রুবক
{
    ফেরত tls_তথ্য;
}
শূন্য TlsAbstraction::set_তথ্য(শূন্য *p_তথ্য)
{
    tls_তথ্য = p_তথ্য;
}
#অন্যথা //WINRT
#ifdef CV_USE_FLS
স্থির শূন্য NTAPI opencv_fls_destructor(শূন্য* p_তথ্য);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(মিথ্যা)
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
    cv::__termination = সত্য;  // DllMain is missing in static builds
    disposed = সত্য;
#ifndef CV_USE_FLS
    TlsFree(tls_চাবি);
#অন্যথা // CV_USE_FLS
    FlsFree(tls_চাবি);
#endif // CV_USE_FLS
    tls_চাবি = TLS_OUT_OF_INDEXES;
}
শূন্য* TlsAbstraction::get_তথ্য() ধ্রুবক
{
    যদি (disposed)
        ফেরত ফাঁকা;
#ifndef CV_USE_FLS
    ফেরত Tls_Get_মান(tls_চাবি);
#অন্যথা // CV_USE_FLS
    ফেরত Fls_Get_মান(tls_চাবি);
#endif // CV_USE_FLS
}
শূন্য TlsAbstraction::set_তথ্য(শূন্য *p_তথ্য)
{
    যদি (disposed)
        ফেরত;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_মান(tls_চাবি, p_তথ্য) == TRUE);
#অন্যথা // CV_USE_FLS
    CV_Assert(Fls_Set_মান(tls_চাবি, p_তথ্য) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#অন্যথা // _WIN32
স্থির শূন্য opencv_tls_destructor(শূন্য* p_তথ্য);
TlsAbstraction::TlsAbstraction()
    : disposed(মিথ্যা)
{
    CV_Assert(pthread_key_create(&tls_চাবি, opencv_tls_destructor) == 0);
}
শূন্য TlsAbstraction::releaseSystemResources()
{
    cv::__termination = সত্য;  // DllMain is missing in static builds
    disposed = সত্য;
    যদি (pthread_key_delete(tls_চাবি) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
শূন্য* TlsAbstraction::get_তথ্য() ধ্রুবক
{
    যদি (disposed)
        ফেরত ফাঁকা;
    ফেরত pthread_getspecific(tls_চাবি);
}
শূন্য TlsAbstraction::set_তথ্য(শূন্য *p_তথ্য)
{
    যদি (disposed)
        ফেরত;  // no-op
    CV_Assert(pthread_setspecific(tls_চাবি, p_তথ্য) == 0);
}
#endif

// Per-thread data structure
কাঠামো Thread_তথ্য
{
    Thread_তথ্য()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<শূন্য*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


স্থির যৌক্তিক g_isTlsStorageInitialized = মিথ্যা;

// Main TLS storage class
শ্রেণী TlsStorage
{
সর্বজনীন:
    TlsStorage() :
        tls_Slots_আকার(0)
    {
        (শূন্য)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = সত্য;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    শূন্য releaseThread(শূন্য* tls_মান = ফাঁকা)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        যদি (ফাঁকা == tls)
            ফেরত;  // TLS singleton is not available (terminated)
        Thread_তথ্য *pTD = tls_মান == ফাঁকা ? (Thread_তথ্য*)tls->get_তথ্য() : (Thread_তথ্য*)tls_মান;
        যদি (pTD == ফাঁকা)
            ফেরত;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        জন্য (size_t i = 0; i < threads.size(); i++)
        {
            যদি (pTD == threads[i])
            {
                threads[i] = ফাঁকা;
                যদি (tls_মান == ফাঁকা)
                    tls->set_তথ্য(0);
                std::vector<শূন্য*>& thread_slots = pTD->slots;
                জন্য (size_t slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    শূন্য* p_তথ্য = thread_slots[slotIdx];
                    thread_slots[slotIdx] = ফাঁকা;
                    যদি (!p_তথ্য)
                        চালু;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    যদি (container)
                        container->delete_তথ্য_Instance(p_তথ্য);
                    অন্যথা
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (পূর্ণসংখ্যা)slotIdx);
                        fflush(stderr);
                    }
                }
                delete pTD;
                ফেরত;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (শূন্য*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_আকার == tlsSlots.size());

        // Find unused slots
        জন্য(size_t slot = 0; slot < tls_Slots_আকার; slot++)
        {
            যদি (tlsSlots[slot].container == ফাঁকা)
            {
                tlsSlots[slot].container = container;
                ফেরত slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tls_Slots_আকার++;
        ফেরত tls_Slots_আকার - 1;
    }

    // Release TLS storage index and pass associated data to caller
    শূন্য releaseSlot(size_t slotIdx, std::vector<শূন্য*> &তথ্য_Vec, যৌক্তিক keepSlot = মিথ্যা)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_আকার == tlsSlots.size());
        CV_Assert(tls_Slots_আকার > slotIdx);

        জন্য(size_t i = 0; i < threads.size(); i++)
        {
            যদি(threads[i])
            {
                std::vector<শূন্য*>& thread_slots = threads[i]->slots;
                যদি (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    তথ্য_Vec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = ফাঁকা;
                }
            }
        }

        যদি (!keepSlot)
        {
            tlsSlots[slotIdx].container = ফাঁকা;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    শূন্য* get_তথ্য(size_t slotIdx) ধ্রুবক
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_আকার > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        যদি (ফাঁকা == tls)
            ফেরত ফাঁকা;  // TLS singleton is not available (terminated)

        Thread_তথ্য* thread_তথ্য = (Thread_তথ্য*)tls->get_তথ্য();
        যদি(thread_তথ্য && thread_তথ্য->slots.size() > slotIdx)
            ফেরত thread_তথ্য->slots[slotIdx];

        ফেরত ফাঁকা;
    }

    // Gather data from threads by TLS storage index
    শূন্য gather(size_t slotIdx, std::vector<শূন্য*> &তথ্য_Vec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_আকার == tlsSlots.size());
        CV_Assert(tls_Slots_আকার > slotIdx);

        জন্য(size_t i = 0; i < threads.size(); i++)
        {
            যদি(threads[i])
            {
                std::vector<শূন্য*>& thread_slots = threads[i]->slots;
                যদি (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    তথ্য_Vec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    শূন্য set_তথ্য(size_t slotIdx, শূন্য* p_তথ্য)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_আকার > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        যদি (ফাঁকা == tls)
            ফেরত;  // TLS singleton is not available (terminated)

        Thread_তথ্য* thread_তথ্য = (Thread_তথ্য*)tls->get_তথ্য();
        যদি(!thread_তথ্য)
        {
            thread_তথ্য = নতুন Thread_তথ্য;
            tls->set_তথ্য((শূন্য*)thread_তথ্য);
            {
                AutoLock guard(mtxGlobalAccess);

                যৌক্তিক found = মিথ্যা;
                // Find unused slots
                জন্য(size_t slot = 0; slot < threads.size(); slot++)
                {
                    যদি (threads[slot] == ফাঁকা)
                    {
                        thread_তথ্য->idx = (পূর্ণসংখ্যা)slot;
                        threads[slot] = thread_তথ্য;
                        found = সত্য;
                        বিরতি;
                    }
                }

                যদি (!found)
                {
                    // Create new slot
                    thread_তথ্য->idx = threads.size();
                    threads.push_back(thread_তথ্য);
                }
            }
        }

        যদি(slotIdx >= thread_তথ্য->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_তথ্য->slots.resize(slotIdx + 1, ফাঁকা);
        }
        thread_তথ্য->slots[slotIdx] = p_তথ্য;
    }

ব্যক্তিগত:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_আকার;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    কাঠামো TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<কাঠামো TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<Thread_তথ্য*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
স্থির TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, নতুন TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
স্থির শূন্য opencv_tls_destructor(শূন্য* p_তথ্য)
{
    যদি (!g_isTlsStorageInitialized)
        ফেরত;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_তথ্য);
}
#অন্যথা // _WIN32
#ifdef CV_USE_FLS
স্থির শূন্য WINAPI opencv_fls_destructor(শূন্য* p_তথ্য)
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
        cv::__termination = সত্য;
    }

    যদি (!g_isTlsStorageInitialized)
        ফেরত;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_তথ্য);
}
#endif // CV_USE_FLS
#endif // _WIN32

স্থির TlsStorage* ধ্রুবক g_force_initialization_of_TlsStorage
#যদি defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#অন্যথা  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
শ্রেণী TlsStorage
{
সর্বজনীন:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        জন্য (size_t slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            যদি (container && s.তথ্য)
            {
                container->delete_তথ্য_Instance(s.তথ্য);  // Can't use from SlotInfo destructor
                s.তথ্য = nullptr;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserveSlot(TLSDataContainer* container)
    {
        size_t slots_আকার = slots.size();
        জন্য (size_t slot = 0; slot < slots_আকার; slot++)
        {
            SlotInfo& s = slots[slot];
            যদি (s.container == ফাঁকা)
            {
                CV_Assert(!s.তথ্য);
                s.container = container;
                ফেরত slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        ফেরত slots_আকার;
    }

    // Release TLS storage index and pass associated data to caller
    শূন্য releaseSlot(size_t slotIdx, std::vector<শূন্য*> &তথ্য_Vec, যৌক্তিক keepSlot = মিথ্যা)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        শূন্য* তথ্য = s.তথ্য;
        যদি (তথ্য)
        {
            তথ্য_Vec.push_back(তথ্য);
            s.তথ্য = nullptr;
        }
        যদি (!keepSlot)
        {
            s.container = ফাঁকা;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    শূন্য* get_তথ্য(size_t slotIdx) ধ্রুবক
    {
        CV_Assert(slotIdx < slots.size());
        ধ্রুবক SlotInfo& s = slots[slotIdx];
        ফেরত s.তথ্য;
    }

    // Gather data from threads by TLS storage index
    শূন্য gather(size_t slotIdx, std::vector<শূন্য*> &তথ্য_Vec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        শূন্য* তথ্য = s.তথ্য;
        যদি (তথ্য)
            তথ্য_Vec.push_back(তথ্য);
        ফেরত;
    }

    // Set data to storage index
    শূন্য set_তথ্য(size_t slotIdx, শূন্য* p_তথ্য)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.তথ্য = p_তথ্য;
    }

ব্যক্তিগত:
    কাঠামো SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), তথ্য(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        শূন্য* তথ্য;
    };
    std::vector<কাঠামো SlotInfo> slots;
};

স্থির TlsStorage& getTlsStorage()
{
    স্থির TlsStorage g_storage;  // no threading
    ফেরত g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using নামাকাশ details;

শূন্য releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    যদি (!g_isTlsStorageInitialized)
        ফেরত;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    চাবি_2 = (পূর্ণসংখ্যা)getTlsStorage().reserveSlot(এই); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(চাবি_2 == -1); // Key must be released in child object
}

শূন্য TLSDataContainer::gather_তথ্য(std::vector<শূন্য*> &তথ্য) ধ্রুবক
{
    getTlsStorage().gather(চাবি_2, তথ্য);
}

শূন্য TLSDataContainer::detach_তথ্য(std::vector<শূন্য*> &তথ্য)
{
    getTlsStorage().releaseSlot(চাবি_2, তথ্য, সত্য);
}

শূন্য TLSDataContainer::release()
{
    যদি (চাবি_2 == -1)
        ফেরত;  // already released
    std::vector<শূন্য*> তথ্য; তথ্য.reserve(32);
    getTlsStorage().releaseSlot(চাবি_2, তথ্য, মিথ্যা); // Release key and get stored data for proper destruction
    চাবি_2 = -1;
    জন্য(size_t i = 0; i < তথ্য.size(); i++)  // Delete all associated data
        delete_তথ্য_Instance(তথ্য[i]);
}

শূন্য TLSDataContainer::cleanup()
{
    std::vector<শূন্য*> তথ্য; তথ্য.reserve(32);
    getTlsStorage().releaseSlot(চাবি_2, তথ্য, সত্য); // Extract stored data with removal from TLS tables
    জন্য(size_t i = 0; i < তথ্য.size(); i++)  // Delete all associated data
        delete_তথ্য_Instance(তথ্য[i]);
}

শূন্য* TLSDataContainer::get_তথ্য() ধ্রুবক
{
    CV_Assert(চাবি_2 != -1 && "Can't fetch data from terminated TLS container.");
    শূন্য* p_তথ্য = getTlsStorage().get_তথ্য(চাবি_2); // Check if data was already allocated
    যদি(!p_তথ্য)
    {
        // Create new data instance and save it to TLS storage
        p_তথ্য = create_তথ্য_Instance();
        চেষ্টা
        {
            getTlsStorage().set_তথ্য(চাবি_2, p_তথ্য);
        }
        ধরো (...)
        {
            delete_তথ্য_Instance(p_তথ্য);
            ছোড়ো;
        }
    }
    ফেরত p_তথ্য;
}

স্থির TLSData<CoreTLSData>& get_Core_Tls_তথ্য_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, নতুন TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_তথ্য()
{
    ফেরত get_Core_Tls_তথ্য_TLS().getRef();
}

#যদি defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

বাহ্যিক "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

বাহ্যিক "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    যদি (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        যদি (lpReserved != ফাঁকা) // called after ExitProcess() call
        {
            cv::__termination = সত্য;
        }
        অন্যথা
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ফেরত TRUE;
}
#endif


নামাকাশ {

#ifdef OPENCV_WITH_ITT
যৌক্তিক overrideThreadName()
{
    স্থির যৌক্তিক param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", মিথ্যা);
    ফেরত param;
}
#endif

স্থির পূর্ণসংখ্যা g_thread_অঙ্ক = 0;
শ্রেণী ThreadID {
সর্বজনীন:
    ধ্রুবক পূর্ণসংখ্যা id;
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
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, নতুন TLSData<ThreadID>());
}

} // namespace
পূর্ণসংখ্যা utils::getThreadID() { ফেরত getThreadIDTLS().get()->id; }


শ্রেণী ParseError
{
    std::string bad_মান;
সর্বজনীন:
    ParseError(ধ্রুবক std::string &bad_মান_2) :bad_মান(bad_মান_2) {}
    std::string toString(ধ্রুবক std::string &param) ধ্রুবক
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_মান;
        ফেরত out.str();
    }
};

template <typename T>
T parseOption(ধ্রুবক std::string &);

template<>
অন্তঃরেখ যৌক্তিক parseOption(ধ্রুবক std::string & value)
{
    যদি (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ফেরত সত্য;
    }
    যদি (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ফেরত মিথ্যা;
    }
    ছোড়ো ParseError(value);
}

template<>
অন্তঃরেখ size_t parseOption(ধ্রুবক std::string &value)
{
    size_t pos = 0;
    জন্য (; pos < value.size(); pos++)
    {
        যদি (!isdigit(value[pos]))
            বিরতি;
    }
    cv::String মান_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(মান_Str);
    যদি (suffixStr.length() == 0)
        ফেরত v;
    অন্যথা যদি (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ফেরত v * 1024 * 1024;
    অন্যথা যদি (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ফেরত v * 1024;
    ছোড়ো ParseError(value);
}

template<>
অন্তঃরেখ cv::String parseOption(ধ্রুবক std::string &value)
{
    ফেরত value;
}

template<>
অন্তঃরেখ utils::Paths parseOption(ধ্রুবক std::string &value)
{
    utils::Paths ফলাফল;
#ifdef _WIN32
    ধ্রুবক অক্ষর sep = ';';
#অন্যথা
    ধ্রুবক অক্ষর sep = ':';
#endif
    size_t start_pos = 0;
    যখন (start_pos != std::string::npos)
    {
        ধ্রুবক size_t pos = value.find(sep, start_pos);
        ধ্রুবক std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        যদি (!one_piece.empty())
            ফলাফল.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ফেরত ফলাফল;
}

স্থির অন্তঃরেখ ধ্রুবক অক্ষর * envRead(ধ্রুবক অক্ষর * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    ফেরত ফাঁকা;
#অন্যথা
    ফেরত getenv(name);
#endif
}

template<typename T>
অন্তঃরেখ T পঠ(ধ্রুবক std::string & k, ধ্রুবক T & default_মান_2)
{
    চেষ্টা
    {
        ধ্রুবক অক্ষর * res = envRead(k.c_str());
        যদি (res)
            ফেরত parseOption<T>(std::string(res));
    }
    ধরো (ধ্রুবক ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    ফেরত default_মান_2;
}

যৌক্তিক utils::getConfigurationParameterBool(ধ্রুবক অক্ষর* name, যৌক্তিক default_মান_2)
{
    ফেরত পঠ<যৌক্তিক>(name, default_মান_2);
}

size_t utils::get_Configuration_Parameter_আকার_T(ধ্রুবক অক্ষর* name, size_t default_মান_2)
{
    ফেরত পঠ<size_t>(name, default_মান_2);
}

std::string utils::getConfigurationParameterString(ধ্রুবক অক্ষর* name, ধ্রুবক std::string & default_মান_2)
{
    ফেরত পঠ<cv::String>(name, default_মান_2);
}

utils::Paths utils::getConfigurationParameterPaths(ধ্রুবক অক্ষর* name, ধ্রুবক utils::Paths &default_মান_2)
{
    ফেরত পঠ<utils::Paths>(name, default_মান_2);
}


#ifdef CV_COLLECT_IMPL_তথ্য
ImplCollector& get_Impl_তথ্য()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, নতুন ImplCollector())
}

শূন্য setImpl(পূর্ণসংখ্যা flags)
{
    cv::AutoLock lock(get_Impl_তথ্য().mutex);

    get_Impl_তথ্য().implFlags = flags;
    get_Impl_তথ্য().implCode.clear();
    get_Impl_তথ্য().implFun.clear();
}

শূন্য addImpl(পূর্ণসংখ্যা flag, ধ্রুবক অক্ষর* func)
{
    cv::AutoLock lock(get_Impl_তথ্য().mutex);

    get_Impl_তথ্য().implFlags |= flag;
    যদি(func) // use lazy collection if name was not specified
    {
        size_t সূচক = get_Impl_তথ্য().implCode.size();
        যদি(!সূচক || (get_Impl_তথ্য().implCode[সূচক-1] != flag || get_Impl_তথ্য().implFun[সূচক-1].compare(func))) // avoid duplicates
        {
            get_Impl_তথ্য().implCode.push_back(flag);
            get_Impl_তথ্য().implFun.push_back(func);
        }
    }
}

পূর্ণসংখ্যা getImpl(std::vector<পূর্ণসংখ্যা> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(get_Impl_তথ্য().mutex);

    impl    = get_Impl_তথ্য().implCode;
    funName = get_Impl_তথ্য().implFun;
    ফেরত get_Impl_তথ্য().implFlags; // return actual flags for lazy collection
}

যৌক্তিক useCollection()
{
    ফেরত get_Impl_তথ্য().useCollection;
}

শূন্য setUseCollection(যৌক্তিক flag)
{
    cv::AutoLock lock(get_Impl_তথ্য().mutex);

    get_Impl_তথ্য().useCollection = flag;
}
#endif

নামাকাশ instr
{
যৌক্তিক useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    ফেরত getInstrumentStruct().useInstr;
#অন্যথা
    ফেরত মিথ্যা;
#endif
}

শূন্য setUseInstrumentation(যৌক্তিক flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#অন্যথা
    CV_UNUSED(flag);
#endif
}

Instr_গ্রন্থি* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    ফেরত &getInstrumentStruct().মূল_গ্রন্থি_2;
#অন্যথা
    ফেরত ফাঁকা;
#endif
}

শূন্য resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().মূল_গ্রন্থি_2.removeChilds();
    getInstrumentTLSStruct().p_Current_গ্রন্থি = &getInstrumentStruct().মূল_গ্রন্থি_2;
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
    ফেরত (FLAGS)getInstrumentStruct().flags;
#অন্যথা
    ফেরত (FLAGS)0;
#endif
}

গ্রন্থি_তথ্য::গ্রন্থি_তথ্য(ধ্রুবক অক্ষর* funName, ধ্রুবক অক্ষর* fileName, পূর্ণসংখ্যা সারি_অঙ্ক, শূন্য* retAddress, যৌক্তিক alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_সারি_অঙ্ক       = সারি_অঙ্ক;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_গণক    = 0;
    m_ticks_যোগ = 0;

    m_funError  = মিথ্যা;
}
গ্রন্থি_তথ্য::গ্রন্থি_তথ্য(গ্রন্থি_তথ্য &ref)
{
    *এই = ref;
}
গ্রন্থি_তথ্য& গ্রন্থি_তথ্য::operator=(ধ্রুবক গ্রন্থি_তথ্য &ডান)
{
    এই->m_funName      = ডান.m_funName;
    এই->m_instrType    = ডান.m_instrType;
    এই->m_implType     = ডান.m_implType;
    এই->m_fileName     = ডান.m_fileName;
    এই->m_সারি_অঙ্ক      = ডান.m_সারি_অঙ্ক;
    এই->m_retAddress   = ডান.m_retAddress;
    এই->m_alwaysExpand = ডান.m_alwaysExpand;

    এই->m_threads     = ডান.m_threads;
    এই->m_গণক     = ডান.m_গণক;
    এই->m_ticks_যোগ  = ডান.m_ticks_যোগ;

    এই->m_funError    = ডান.m_funError;

    ফেরত *এই;
}
গ্রন্থি_তথ্য::~গ্রন্থি_তথ্য()
{
}
যৌক্তিক operator==(ধ্রুবক গ্রন্থি_তথ্য& বাম, ধ্রুবক গ্রন্থি_তথ্য& ডান)
{
    যদি(বাম.m_সারি_অঙ্ক == ডান.m_সারি_অঙ্ক && বাম.m_funName == ডান.m_funName && বাম.m_fileName == ডান.m_fileName)
    {
        যদি(বাম.m_retAddress == ডান.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || বাম.m_alwaysExpand))
            ফেরত সত্য;
    }
    ফেরত মিথ্যা;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    স্থির InstrStruct instr;
    ফেরত instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ফেরত *getInstrumentStruct().tlsStruct.get();
}

Instr_গ্রন্থি* get_Current_গ্রন্থি()
{
    ফেরত getInstrumentTLSStruct().p_Current_গ্রন্থি;
}

IntrumentationRegion::IntrumentationRegion(ধ্রুবক অক্ষর* funName, ধ্রুবক অক্ষর* fileName, পূর্ণসংখ্যা সারি_অঙ্ক, শূন্য *retAddress, যৌক্তিক alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = মিথ্যা;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    যদি(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        যদি(!pTLS->p_Current_গ্রন্থি)
        {
            m_disabled = সত্য;
            ফেরত;
        }

        পূর্ণসংখ্যা depth = pTLS->p_Current_গ্রন্থি->getDepth();
        যদি(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = সত্য;
            ফেরত;
        }

        গ্রন্থি_তথ্য payload(funName, fileName, সারি_অঙ্ক, retAddress, alwaysExpand, instrType, implType);
        Node<গ্রন্থি_তথ্য>* pChild = ফাঁকা;

        যদি(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_গ্রন্থি->findChild(payload);
            যদি(!pChild)
            {
                pChild = নতুন Node<গ্রন্থি_তথ্য>(payload);
                pTLS->p_Current_গ্রন্থি->addChild(pChild);
            }
        }
        অন্যথা
        {
            pChild = pTLS->p_Current_গ্রন্থি->findChild(payload);
            যদি(!pChild)
            {
                m_disabled = সত্য;
                ফেরত;
            }
        }
        pTLS->p_Current_গ্রন্থি = pChild;

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

            যদি (pTLS->p_Current_গ্রন্থি->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_গ্রন্থি->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_গ্রন্থি->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_গুনতি() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_গুনতি); // Concurrent ticks accumulation
                pTLS->p_Current_গ্রন্থি->m_payload.m_গণক++;
                pTLS->p_Current_গ্রন্থি->m_payload.m_ticks_যোগ += ticks;
                pTLS->p_Current_গ্রন্থি->m_payload.m_tls.get()->m_ticks_যোগ += ticks;
            }

            pTLS->p_Current_গ্রন্থি = pTLS->p_Current_গ্রন্থি->m_pParent;
        }
    }
}
#endif
}

নামাকাশ ipp
{

#ifdef HAVE_IPP
কাঠামো IPPInitSingleton
{
সর্বজনীন:
    IPPInitSingleton()
    {
        useIPP         = সত্য;

#যদি defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#অন্যথা
        useIPP_NE      = মিথ্যা;
#endif
        ippStatus      = 0;
        funcname       = ফাঁকা;
        filename       = ফাঁকা;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_শীর্ষ_Features = 0;
        pIppLibInfo    = ফাঁকা;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, ফাঁকা);
        যদি(ippStatus < 0)
        {
            CV_LOG_ERROR(ফাঁকা, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = মিথ্যা;
            ফেরত;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        যদি(!env.empty())
        {
#যদি IPP_VERSION_X100 >= 201900
            ধ্রুবক Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#নতুবা_যদি IPP_VERSION_X100 >= 201703
            ধ্রুবক Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#নতুবা_যদি IPP_VERSION_X100 >= 201700
            ধ্রুবক Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#অন্যথা
            ধ্রুবক Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            যদি(env.substr(0, 2) == "ne")
            {
                useIPP_NE = সত্য;
                env = env.substr(3, env.size());
            }

            যদি(env == "disabled")
            {
                CV_LOG_WARNING(ফাঁকা, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = মিথ্যা;
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
                CV_LOG_ERROR(ফাঁকা, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

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
            useIPP = মিথ্যা;
            ফেরত;
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
                ipp_শীর্ষ_Features = ippCPUID_AVX512_SKX;
            অন্যথা যদি((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_শীর্ষ_Features = ippCPUID_AVX512_KNL;
            অন্যথা
                ipp_শীর্ষ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        অন্যথা
#endif
        যদি(ippFeatures&ippCPUID_AVX2)
            ipp_শীর্ষ_Features = ippCPUID_AVX2;
        অন্যথা যদি(ippFeatures&ippCPUID_SSE42)
            ipp_শীর্ষ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        যদি (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_শীর্ষ_Features = ippCPUID_SSE42;
        }
    }

সর্বজনীন:
    যৌক্তিক        useIPP;
    যৌক্তিক        useIPP_NE;

    পূর্ণসংখ্যা         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    ধ্রুবক অক্ষর *funcname;
    ধ্রুবক অক্ষর *filename;
    পূর্ণসংখ্যা         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_শীর্ষ_Features;
    ধ্রুবক IppLibraryVersion *pIppLibInfo;
};

স্থির IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, নতুন IPPInitSingleton())
}
#endif

unsigned long long getIppFeatures()
{
#ifdef HAVE_IPP
    ফেরত getIPPSingleton().ippFeatures;
#অন্যথা
    ফেরত 0;
#endif
}

#ifdef HAVE_IPP
unsigned long long get_Ipp_শীর্ষ_Features()
{
    ফেরত getIPPSingleton().ipp_শীর্ষ_Features;
}
#endif

শূন্য setIppStatus(পূর্ণসংখ্যা status, ধ্রুবক অক্ষর * ধ্রুবক _funcname, ধ্রুবক অক্ষর * ধ্রুবক _filename, পূর্ণসংখ্যা সারি_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = সারি_3;
#অন্যথা
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(সারি_3);
#endif
}

পূর্ণসংখ্যা getIppStatus()
{
#ifdef HAVE_IPP
    ফেরত getIPPSingleton().ippStatus;
#অন্যথা
    ফেরত 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    ফেরত format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#অন্যথা
    ফেরত String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    ধ্রুবক IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    যদি(pInfo)
        ফেরত format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    অন্যথা
        ফেরত String("error");
#অন্যথা
    ফেরত String("disabled");
#endif
}

যৌক্তিক useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    যদি (তথ্য.useIPP < 0)
    {
        তথ্য.useIPP = getIPPSingleton().useIPP;
    }
    ফেরত (তথ্য.useIPP > 0);
#অন্যথা
    ফেরত মিথ্যা;
#endif
}

শূন্য setUseIPP(যৌক্তিক flag)
{
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
#ifdef HAVE_IPP
    তথ্য.useIPP = (getIPPSingleton().useIPP)?flag:মিথ্যা;
#অন্যথা
    CV_UNUSED(flag);
    তথ্য.useIPP = মিথ্যা;
#endif
}

যৌক্তিক useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
    যদি (তথ্য.useIPP_NE < 0)
    {
        তথ্য.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ফেরত (তথ্য.useIPP_NE > 0);
#অন্যথা
    ফেরত মিথ্যা;
#endif
}

শূন্য setUseIPP_NotExact(যৌক্তিক flag)
{
    CoreTLSData& তথ্য = get_Core_Tls_তথ্য();
#ifdef HAVE_IPP
    তথ্য.useIPP_NE = flag;
#অন্যথা
    CV_UNUSED(flag);
    তথ্য.useIPP_NE = মিথ্যা;
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

শূন্য setFPDenormalsIgnoreHint(যৌক্তিক ignore, CV_OUT FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    unsigned value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    যদি (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ধ্রুবক unsigned old_flags = _mm_getcsr();
    ধ্রুবক unsigned old_মান = old_flags & mask;
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ফাঁকা, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_মান;
    _mm_setcsr(flags);
#অন্যথা
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

পূর্ণসংখ্যা saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    unsigned mask = _MM_FLUSH_ZERO_MASK;
    যদি (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    ধ্রুবক unsigned old_flags = _mm_getcsr();
    ধ্রুবক unsigned old_মান = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_মান;
    ফেরত 2;
#অন্যথা
    CV_UNUSED(state);
    ফেরত 0;
#endif
}

যৌক্তিক restoreFPDenormalsState(ধ্রুবক FPDenormalsModeState& state)
{
#যদি OPENCV_IMPL_FP_HINTS_X86
    ধ্রুবক unsigned mask = (unsigned)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    ধ্রুবক unsigned value = (unsigned)state.reserved[1];
    CV_DbgCheck((পূর্ণসংখ্যা)value, value == (value & mask), "invalid SSE FP state");
    ধ্রুবক unsigned old_flags = _mm_getcsr();
    unsigned flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(ফাঁকা, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ফেরত সত্য;
#অন্যথা
    CV_UNUSED(state);
    ফেরত মিথ্যা;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    ফেরত OPENCV_ALGO_HINT_DEFAULT;
#অন্যথা
    ফেরত ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
