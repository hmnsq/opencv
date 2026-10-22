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

#अन्तर्भाव "precomp.hpp"
//#include "opencv2/core/core_c.h"
#अन्तर्भाव <atomic>
#अन्तर्भाव <exception>
#अन्तर्भाव <iostream>
#अन्तर्भाव <ostream>

#ifdef __QNX__
    #अन्तर्भाव <unistd.h>
    #अन्तर्भाव <sys/neutrino.h>
    #अन्तर्भाव <sys/syspage.h>
#ifdef __aarch64__
    #अन्तर्भाव <aarch64/syspage.h>
#endif
#endif

#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>
#अन्तर्भाव <opencv2/core/utils/trace.private.hpp>

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#अन्तर्भाव <opencv2/core/utils/tls.hpp>
#अन्तर्भाव <opencv2/core/utils/instrumentation.hpp>

#अन्तर्भाव <opencv2/core/utils/filesystem.private.hpp>

#अन्तर्भाव <opencv2/core/utils/fp_control_utils.hpp>
#अन्तर्भाव <opencv2/core/utils/fp_control.private.hpp>

नामाकाश cv {

स्थिर शून्य _initSystem()
{
#ifdef __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    स्थिर std::ios_base::Init s_iostream_initializer;
#endif
}

स्थिर Mutex* __initialization_mutex = रिक्त;
Mutex& getInitializationMutex()
{
    यदि (__initialization_mutex == रिक्त)
    {
        (शून्य)_initSystem();
        __initialization_mutex = नूतन Mutex();
    }
    फर्काऊ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

स्थिर तार्किक param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#यदि defined(_DEBUG) || defined(__ANDROID__)
    सत्य
#अन्यथा
    असत्य
#endif
);

शून्य* allocSingletonBuffer(माप_प्रकार size) { फर्काऊ fastMalloc(size); }
शून्य* allocSingletonNewBuffer(माप_प्रकार size) { फर्काऊ दो_स्मृति(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# यदि defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#यदि defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# यदि _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#अन्तर्भाव <cstdlib>        // std::abort
#endif

#यदि defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#यदि defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
अचर uint64_t AT_HWCAP = NT_GNU_HWCAP;
#अन्यथा
#  include <elf.h>
#endif
#यदि defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#यदि defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#यदि ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#यदि defined __loongarch64
#अन्तर्भाव "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#यदि defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#अन्तर्भाव <windows.h>
#अन्तर्भाव <combaseapi.h>
#यदि (_WIN32_WINNT >= 0x0602)
  #अन्तर्भाव <synchapi.h>
#endif
#यदि ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #अन्तर्भाव <fibersapi.h>
  #define CV_USE_FLS
#endif
#undef small
#undef min
#undef max
#undef असल
#अन्तर्भाव <tchar.h>

#ifdef WINRT
#अन्तर्भाव <wrl/client.h>
#ifndef __cplusplus_winrt
#अन्तर्भाव <windows.storage.h>
#pragma comment(lib, "runtimeobject.lib")
#endif // WINRT

std::wstring GetTempPathWinRT()
{
#ifdef __cplusplus_winrt
    फर्काऊ std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#अन्यथा
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    यदि (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        फर्काऊ wstr;
    यदि (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        फर्काऊ wstr;
    यदि (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        फर्काऊ wstr;
    यदि (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        फर्काऊ wstr;
    यदि (FAILED(storagefolderRef.As(&storageitemRef)))
        फर्काऊ wstr;
    str = रिक्त;
    यदि (FAILED(storageitemRef->get_Path(&str)))
        फर्काऊ wstr;
    wstr = WindowsGetStringRawBuffer(str, रिक्त);
    WindowsDeleteString(str);
    फर्काऊ wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    ब_अक्षर_प्रकार guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    ब_अक्षर_प्रकार* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], परिमाण(guidStr)/परिमाण(ब_अक्षर_प्रकार), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    फर्काऊ prefix.append(std::wstring(guidStr));
}

#endif
#अन्यथा
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#अन्तर्भाव <pthread.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>

#यदि defined __MACH__ && defined __APPLE__
#अन्तर्भाव <mach/mach.h>
#अन्तर्भाव <mach/mach_time.h>
#अन्तर्भाव <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#अन्तर्भाव "omp.h"
#endif

#यदि defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#यदि defined __ANDROID__
#अन्तर्भाव <sys/sysconf.h>
#endif
#endif

#ifdef __ANDROID__
# include <android/log.h>
#endif

#ifdef DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#endif
#ifndef CV_CPUID_X86
  #यदि defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #यदि _MSC_VER >= 1400  // MSVS 2005
      #अन्तर्भाव <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #अन्यथा
      #error "Required MSVS 2005+"
    #endif
  #अथवायदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    स्थिर शून्य cv_cpuid(पूर्णाङ्क* cpuid_data, पूर्णाङ्क reg_eax, पूर्णाङ्क reg_ecx)
    {
        पूर्णाङ्क __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#यदि !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#अथवायदि defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#अन्यथा
#error "Configuration error"
#endif
        cpuid_data[0] = __eax; cpuid_data[1] = __ebx; cpuid_data[2] = __ecx; cpuid_data[3] = __edx;
    }
    #define CV_CPUID_X86 cv_cpuid
  #endif
#endif

#अन्तर्भाव <chrono>

नामाकाश cv
{

Exception::Exception() { code = Error::StsOk; पङ्क्ति = 0; }

Exception::Exception(Error::Code _code, अचर String& _err, अचर String& _func, अचर String& _file, पूर्णाङ्क पङ्क्ति_3)
: code(_code), err(_err), func(_func), file(_file), पङ्क्ति(पङ्क्ति_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
अचर वर्ण* Exception::what() अचर CV_NOEXCEPT { फर्काऊ सन्देश.c_str(); }

शून्य Exception::formatMessage()
{
    माप_प्रकार pos = err.find('\n');
    तार्किक multiline = pos != cv::String::npos;
    यदि (multiline)
    {
        std::stringstream ss;
        माप_प्रकार prev_pos = 0;
        जबसम्म (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        यदि (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    यदि (func.size() > 0)
    {
        यदि (multiline)
            सन्देश = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), पङ्क्ति, code, codeMessage(), func.c_str(), err.c_str());
        अन्यथा
            सन्देश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), पङ्क्ति, code, codeMessage(), err.c_str(), func.c_str());
    }
    अन्यथा
    {
        सन्देश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), पङ्क्ति, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

अचर वर्ण * Exception::codeMessage() अचर
{
    विकल्प (code)
    {
    पक्ष cv::Error::StsOk :                  फर्काऊ "No Error";
    पक्ष cv::Error::StsBackTrace :           फर्काऊ "Backtrace";
    पक्ष cv::Error::StsError :               फर्काऊ "Unspecified error";
    पक्ष cv::Error::StsInternal :            फर्काऊ "Internal error";
    पक्ष cv::Error::StsNoMem :               फर्काऊ "Insufficient memory";
    पक्ष cv::Error::StsBadArg :              फर्काऊ "Bad argument";
    पक्ष cv::Error::StsNoConv :              फर्काऊ "Iterations do not converge";
    पक्ष cv::Error::StsAutoTrace :           फर्काऊ "Autotrace call";
    पक्ष cv::Error::StsBadSize :             फर्काऊ "Incorrect size of input array";
    पक्ष cv::Error::StsNullPtr :             फर्काऊ "Null pointer";
    पक्ष cv::Error::StsDivByZero :           फर्काऊ "Division by zero occurred";
    पक्ष cv::Error::BadStep :                फर्काऊ "Image step is wrong";
    पक्ष cv::Error::StsInplaceNotSupported : फर्काऊ "Inplace operation is not supported";
    पक्ष cv::Error::StsObjectNotFound :      फर्काऊ "Requested object was not found";
    पक्ष cv::Error::BadDepth :               फर्काऊ "Input image depth is not supported by function";
    पक्ष cv::Error::StsUnmatchedFormats :    फर्काऊ "Formats of input arguments do not match";
    पक्ष cv::Error::StsUnmatchedSizes :      फर्काऊ "Sizes of input arguments do not match";
    पक्ष cv::Error::StsOutOfRange :          फर्काऊ "One of the arguments\' values is out of range";
    पक्ष cv::Error::StsUnsupportedFormat :   फर्काऊ "Unsupported format or combination of formats";
    पक्ष cv::Error::BadCOI :                 फर्काऊ "Input COI is not supported";
    पक्ष cv::Error::Bad_अङ्क_Channels :         फर्काऊ "Bad number of channels";
    पक्ष cv::Error::StsBadFlag :             फर्काऊ "Bad flag (parameter or structure field)";
    पक्ष cv::Error::StsBadPoint :            फर्काऊ "Bad parameter of type CvPoint";
    पक्ष cv::Error::StsBadMask :             फर्काऊ "Bad type of mask argument";
    पक्ष cv::Error::StsParseError :          फर्काऊ "Parsing error";
    पक्ष cv::Error::StsNotImplemented :      फर्काऊ "The function/feature is not implemented";
    पक्ष cv::Error::StsBadMemBlock :         फर्काऊ "Memory block has been corrupted";
    पक्ष cv::Error::StsAssert :              फर्काऊ "Assertion failed";
    पक्ष cv::Error::GpuNotSupported :        फर्काऊ "No CUDA support";
    पक्ष cv::Error::GpuApiCallError :        फर्काऊ "Gpu API call";
    पक्ष cv::Error::OpenGlNotSupported :     फर्काऊ "No OpenGL support";
    पक्ष cv::Error::OpenGlApiCallError :     फर्काऊ "OpenGL API call";
    पक्ष cv::Error::OpenCLApiCallError :     फर्काऊ "OpenCL API call error";
    पक्ष cv::Error::OpenCLDoubleNotSupported:फर्काऊ "OpenCL device does not support double";
    पक्ष cv::Error::OpenCLInitError :        फर्काऊ "OpenCL initialization error";
    पक्ष cv::Error::OpenCLNoAMDBlasFft :     फर्काऊ "Missing OpenCL AMD BLAS FFT";
    स्वभाव:
    {
        स्थिर वर्ण buf[256] {0};
        snprintf(buf, परिमाण(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        फर्काऊ buf;
    }
    };

}

स्थिर अचर वर्ण* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { रिक्त };

स्थिर अचर वर्ण* getHWFeatureName(पूर्णाङ्क id)
{
    फर्काऊ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : रिक्त;
}
स्थिर अचर वर्ण* getHWFeatureNameSafe(पूर्णाङ्क id)
{
    अचर वर्ण* name = getHWFeatureName(id);
    फर्काऊ name ? name : "Unknown feature";
}

संरचना HWFeatures
{
    गणना { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(तार्किक run_initialize = असत्य)
    {
        यदि (run_initialize)
            initialize();
    }

    स्थिर शून्य initializeNames()
    {
        लागि (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    शून्य initialize(शून्य)
    {
        यदि (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            ख_लिखो(मानक_त्रुटि, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        पूर्णाङ्क cpuid_data[4] = { 0, 0, 0, 0 };
        पूर्णाङ्क cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        पूर्णाङ्क x86_family = (cpuid_data[0] >> 8) & 15;
        यदि( x86_family >= 6 )
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
            पूर्णाङ्क cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            तार्किक have_AVX_OS_support = सत्य;
            तार्किक have_AVX512_OS_support = सत्य;
            यदि (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = असत्य; // OS uses XSAVE_XRSTORE and CPU support AVX
            अन्यथा
            {
                पूर्णाङ्क xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (पूर्णाङ्क)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #अथवायदि defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                यदि ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = असत्य; // YMM registers
                यदि ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = असत्य; // ZMM registers
            }

            यदि (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = असत्य;
                have[CV_CPU_FP16] = असत्य;
                have[CV_CPU_AVX2] = असत्य;
                have[CV_CPU_FMA3] = असत्य;
                have[CV_CPU_AVX_VNNI] = असत्य;
            }
            यदि (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = असत्य;
                have[CV_CPU_AVX_512BW] = असत्य;
                have[CV_CPU_AVX_512CD] = असत्य;
                have[CV_CPU_AVX_512DQ] = असत्य;
                have[CV_CPU_AVX_512ER] = असत्य;
                have[CV_CPU_AVX_512IFMA] = असत्य;
                have[CV_CPU_AVX_512PF] = असत्य;
                have[CV_CPU_AVX_512VBMI] = असत्य;
                have[CV_CPU_AVX_512VL] = असत्य;
                have[CV_CPU_AVX_512VBMI2] = असत्य;
                have[CV_CPU_AVX_512VNNI] = असत्य;
                have[CV_CPU_AVX_512BITALG] = असत्य;
                have[CV_CPU_AVX_512VPOPCNTDQ] = असत्य;
                have[CV_CPU_AVX_5124VNNIW] = असत्य;
                have[CV_CPU_AVX_5124FMAPS] = असत्य;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            यदि (have[CV_CPU_AVX512_COMMON])
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
            अन्यथा
            {
                have[CV_CPU_AVX512_KNL] = असत्य;
                have[CV_CPU_AVX512_KNM] = असत्य;
                have[CV_CPU_AVX512_SKX] = असत्य;
                have[CV_CPU_AVX512_CNL] = असत्य;
                have[CV_CPU_AVX512_CLX] = असत्य;
                have[CV_CPU_AVX512_ICL] = असत्य;
            }
        }
    #endif // CV_CPUID_X86

    #यदि defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = सत्य;
        have[CV_CPU_FP16] = सत्य;
        पूर्णाङ्क cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        यदि (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            अचर माप_प्रकार size_auxv_t = परिमाण(auxv);

            जबसम्म ((माप_प्रकार)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                यदि (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#यदि defined(AT_HWCAP2)
                अन्यथा यदि (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            बंद(cpufile);
        }
    #अथवायदि defined __arm__ && defined __ANDROID__
      #यदि defined HAVE_CPUFEATURES
        CV_LOG_INFO(रिक्त, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(रिक्त, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (दीर्घ दीर्घ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #अन्यथा
        CV_LOG_INFO(रिक्त, "cpufeatures library is not available for CPU detection");
        #यदि CV_NEON
        CV_LOG_INFO(रिक्त, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- NEON instructions is NOT enabled via build flags");
        #endif
        #यदि CV_FP16
        CV_LOG_INFO(रिक्त, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #अथवायदि defined __arm__
        पूर्णाङ्क cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        यदि (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            अचर माप_प्रकार size_auxv_t = परिमाण(auxv);

            जबसम्म ((माप_प्रकार)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                यदि (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    रोक;
                }
            }

            बंद(cpufile);
        }
    #endif
    #अथवायदि (defined __APPLE__)
    #यदि defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
    #endif
    #यदि (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = सत्य;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    पूर्णाङ्क has_feat_dotprod = 0;
    माप_प्रकार has_feat_dotprod_size = परिमाण(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, रिक्त, 0);
    यदि (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = सत्य;
    }
    पूर्णाङ्क has_feat_bf16 = 0;
    माप_प्रकार has_feat_bf16_size = परिमाण(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, रिक्त, 0);
    यदि (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = सत्य;
    }
    #अथवायदि (defined __clang__)
    #यदि defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
        #यदि (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = सत्य;
        #endif
    #endif
    #endif
    #यदि defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = सत्य;
    #endif
    #यदि defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = सत्य;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = सत्य;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = सत्य;
    #endif

    #यदि (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        अचिन्हित पूर्णाङ्क hwcap = getauxval(AT_HWCAP);
        यदि (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            यदि (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अथवायदि (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        अचिन्हित दीर्घ hwcap = 0, hwcap2 = 0;
        यदि (elf_aux_info(AT_HWCAP, &hwcap, परिमाण(hwcap)) != 0)
            hwcap = 0;
        यदि (hwcap & PPC_FEATURE_HAS_VSX) {
            यदि (elf_aux_info(AT_HWCAP2, &hwcap2, परिमाण(hwcap2)) != 0)
                hwcap2 = 0;
            यदि (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अन्यथा
        // TODO: AIX
        #यदि CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = सत्य;
        #endif
        #यदि CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = सत्य;
        #endif
    #endif

    #यदि defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = सत्य;
        #यदि (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = सत्य;
        #endif
        #यदि defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = सत्य;
        #endif
    #endif

    #यदि defined __loongarch64 && defined __linux__
        पूर्णाङ्क flag = (पूर्णाङ्क)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        तार्किक skip_baseline_check = असत्य;
        यदि (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = सत्य;
        }
        पूर्णाङ्क baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        यदि (!checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]))
            && !skip_baseline_check)
        {
            ख_लिखो(मानक_त्रुटि, "\n"
                    "******************************************************************\n"
                    "* FATAL ERROR:                                                   *\n"
                    "* This OpenCV build doesn't support current CPU/HW configuration *\n"
                    "*                                                                *\n"
                    "* Use OPENCV_DUMP_CONFIG=1 environment variable for details      *\n"
                    "******************************************************************\n");
            ख_लिखो(मानक_त्रुटि, "\nRequired baseline features:\n");
            checkFeatures(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]), सत्य);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, परिमाण(baseline_features) / परिमाण(baseline_features[0]));
    }

    तार्किक checkFeatures(अचर पूर्णाङ्क* features, पूर्णाङ्क गन्ती_2, तार्किक dump = असत्य)
    {
        तार्किक result = सत्य;
        लागि (पूर्णाङ्क i = 0; i < गन्ती_2; i++)
        {
            पूर्णाङ्क feature = features[i];
            यदि (feature)
            {
                यदि (have[feature])
                {
                    यदि (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                अन्यथा
                {
                    result = असत्य;
                    यदि (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        फर्काऊ result;
    }

    स्थिर अन्तःस्थ तार्किक is_चिह्न_Separator(वर्ण c)
    {
        फर्काऊ c == ',' || c == ';';
    }

    शून्य readSettings(अचर पूर्णाङ्क* baseline_features, पूर्णाङ्क baseline_गन्ती)
    {
        तार्किक dump = सत्य;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        यदि (!disabled_features.empty())
        {
            अचर वर्ण* start = disabled_features.c_str();
            लागि (;;)
            {
                जबसम्म (start[0] != 0 && is_चिह्न_Separator(start[0]))
                {
                    start++;
                }
                यदि (start[0] == 0)
                    रोक;
                अचर वर्ण* end = start;
                जबसम्म (end[0] != 0 && !is_चिह्न_Separator(end[0]))
                {
                    end++;
                }
                यदि (end == start)
                    जारी;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                तार्किक found = असत्य;
                लागि (पूर्णाङ्क i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    यदि (!g_hwFeatureNames[i]) जारी;
                    माप_प्रकार len = म_माप(g_hwFeatureNames[i]);
                    यदि (len != feature.size()) जारी;
                    यदि (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        तार्किक isBaseline = असत्य;
                        लागि (पूर्णाङ्क k = 0; k < baseline_गन्ती; k++)
                        {
                            यदि (baseline_features[k] == i)
                            {
                                isBaseline = सत्य;
                                रोक;
                            }
                        }
                        यदि (isBaseline)
                        {
                            यदि (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        यदि (!have[i])
                        {
                            यदि (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = असत्य;

                        found = सत्य;
                        रोक;
                    }
                }
                यदि (!found)
                {
                    यदि (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    तार्किक have[MAX_FEATURE+1]{};
};

स्थिर HWFeatures  featuresEnabled(सत्य), featuresDisabled = HWFeatures(असत्य);
स्थिर HWFeatures* currentFeatures = &featuresEnabled;

तार्किक checkHardwareSupport(पूर्णाङ्क feature)
{
    यदि (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        फर्काऊ असत्य;
    फर्काऊ currentFeatures->have[feature];
}

String getHardwareFeatureName(पूर्णाङ्क feature)
{
    अचर वर्ण* name = getHWFeatureName(feature);
    फर्काऊ name ? String(name) : String();
}

std::string get_CPUFeatures_पङ्क्ति()
{
    अचर पूर्णाङ्क features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    अचर पूर्णाङ्क sz = परिमाण(features) / परिमाण(features[0]);
    std::string result;
    std::string prefix;
    लागि (पूर्णाङ्क i = 1; i < sz; ++i)
    {
        यदि (features[i] == 0)
        {
            prefix = "*";
            जारी;
        }
        यदि (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        यदि (!checkHardwareSupport(features[i])) result.append("?");
    }
    फर्काऊ result;
}

चञ्चल तार्किक useOptimizedFlag = सत्य;

शून्य setUseOptimized( तार्किक flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#ifdef HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#endif
}

तार्किक useOptimized(शून्य)
{
    फर्काऊ useOptimizedFlag;
}

int64 get_Tick_गन्ती(शून्य)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    फर्काऊ (int64)now.time_since_epoch().गन्ती_2();
}

द्विगुण getTickFrequency(शून्य)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    द्विगुण clock_freq = clock_period_t::den / clock_period_t::num;
    फर्काऊ clock_freq;
}

#यदि defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#यदि defined(__i386__)

int64 get_CPUTick_गन्ती(शून्य)
{
    int64 x;
    __asm__ चञ्चल (".byte 0x0f, 0x31" : "=A" (x));
    फर्काऊ x;
}
#अथवायदि defined(__x86_64__)

int64 get_CPUTick_गन्ती(शून्य)
{
    अचिन्हित hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    फर्काऊ (int64)lo | ((int64)hi << 32);
}

#अथवायदि defined(__ppc__)

int64 get_CPUTick_गन्ती(शून्य)
{
    अचिन्हित upper, lower, tmp;
    __asm__ चञ्चल(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    फर्काऊ lower | ((int64)upper << 32);
}

#अन्यथा

#error "RDTSC not defined"

#endif

#अथवायदि defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_गन्ती(शून्य)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#अन्यथा

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_गन्ती(शून्य)
{
    फर्काऊ get_Tick_गन्ती();
}
//#endif

#endif


नामाकाश internal {

वर्ग Timestamp
{
सार्वजनिक:
    अचर int64 zero_Tick_गन्ती;
    अचर द्विगुण ns_in_ticks;

    Timestamp()
        : zero_Tick_गन्ती(get_Tick_गन्ती())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_गन्ती();
        फर्काऊ (int64)((t - zero_Tick_गन्ती) * ns_in_ticks);
    }

    स्थिर Timestamp& getInstance()
    {
        स्थिर Timestamp g_timestamp;
        फर्काऊ g_timestamp;
    }
};

वर्ग InitTimestamp {
सार्वजनिक:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
स्थिर InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    फर्काऊ internal::Timestamp::getInstance().getTimestamp();
}


अचर String& getBuildInformation()
{
    स्थिर String build_info =
#अन्तर्भाव "version_string.inc"
    ;
    फर्काऊ build_info;
}

String getVersionString() { फर्काऊ String(CV_VERSION); }

पूर्णाङ्क getVersionMajor() { फर्काऊ CV_VERSION_MAJOR; }

पूर्णाङ्क getVersionMinor() { फर्काऊ CV_VERSION_MINOR; }

पूर्णाङ्क getVersionRevision() { फर्काऊ CV_VERSION_REVISION; }

String format( अचर वर्ण* fmt, ... )
{
    AutoBuffer<वर्ण, 1024> buf;

    लागि ( ; ; )
    {
        बहु_सूची va;
        बहु_शुरू(va, fmt);
        पूर्णाङ्क bsize = static_cast<पूर्णाङ्क>(buf.size());
        पूर्णाङ्क len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        बहु_पूर्ण(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        यदि (len >= bsize)
        {
            buf.resize(len + 1);
            जारी;
        }
        buf[bsize - 1] = 0;
        फर्काऊ String(buf.data(), len);
    }
}

String tempfile( अचर वर्ण* suffix )
{
#यदि OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#यदि defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    यदि (temp_file.empty())
        फर्काऊ String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    वर्ण aname[MAX_PATH];
    माप_प्रकार copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
    fname = String(aname);
    RoUninitialize();
#अथवायदि defined(_WIN32_WCE)
    अचर स्वतः kMaxPathSize = MAX_PATH+1;
    ब_अक्षर_प्रकार temp_dir_ce[kMaxPathSize] = {0};
    ब_अक्षर_प्रकार temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    यदि(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        वर्ण aname[MAX_PATH];
        माप_प्रकार copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        fname = String(aname);
    }
#अन्यथा
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    ब_अक्षर_प्रकार temp_dir2_w[MAX_PATH] = { 0 };

    यदि (temp_dir.empty())
    {
        ::GetTempPathW(परिमाण(temp_dir2_w)/परिमाण(ब_अक्षर_प्रकार), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        पूर्णाङ्क len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, रिक्त, 0, रिक्त, रिक्त);
        यदि (len > 0)
        {
            std::vector<वर्ण> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, रिक्त, रिक्त);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    यदि (FAILED(hr))
        फर्काऊ String();
    वर्ण guidStr[40];
    अचर वर्ण* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, परिमाण(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (अचिन्हित पूर्णाङ्क)g.Data4[0], (अचिन्हित पूर्णाङ्क)g.Data4[1],
            (अचिन्हित पूर्णाङ्क)g.Data4[2], (अचिन्हित पूर्णाङ्क)g.Data4[3], (अचिन्हित पूर्णाङ्क)g.Data4[4],
            (अचिन्हित पूर्णाङ्क)g.Data4[5], (अचिन्हित पूर्णाङ्क)g.Data4[6], (अचिन्हित पूर्णाङ्क)g.Data4[7]);

    fname = temp_dir;
    यदि (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# अन्यथा
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    वर्ण defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  अन्यथा
    वर्ण defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    यदि (temp_dir.empty())
        fname = defaultTemplate;
    अन्यथा
    {
        fname = temp_dir;
        वर्ण ech = fname[fname.size() - 1];
        यदि(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    अचर पूर्णाङ्क fd = mkstemp((वर्ण*)fname.c_str());
    यदि (fd == -1) फर्काऊ String();

    बंद(fd);
    हटाओ(fname.c_str());
# endif

    यदि (suffix)
    {
        यदि (suffix[0] != '.')
            फर्काऊ fname + "." + suffix;
        अन्यथा
            फर्काऊ fname + suffix;
    }
    फर्काऊ fname;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#endif // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

स्थिर ErrorCallback customErrorCallback = 0;
स्थिर शून्य* customErrorCallbackData = 0;
स्थिर तार्किक breakOnError = असत्य;

तार्किक setBreakOnError(तार्किक value)
{
    तार्किक prevVal = breakOnError;
    breakOnError = value;
    फर्काऊ prevVal;
}

पूर्णाङ्क cv_snprintf(वर्ण* buf, पूर्णाङ्क len, अचर वर्ण* fmt, ...)
{
    बहु_सूची va;
    बहु_शुरू(va, fmt);
    पूर्णाङ्क res = cv_vsnprintf(buf, len, fmt, va);
    बहु_पूर्ण(va);
    फर्काऊ res;
}

पूर्णाङ्क cv_vsnprintf(वर्ण* buf, पूर्णाङ्क len, अचर वर्ण* fmt, बहु_सूची args)
{
#यदि defined _MSC_VER
    यदि (len <= 0) फर्काऊ len == 0 ? 1024 : -1;
    पूर्णाङ्क res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    यदि (res >= 0 && res < len)
    {
        buf[res] = 0;
        फर्काऊ res;
    }
    अन्यथा
    {
        buf[len - 1] = 0; // truncate happened
        फर्काऊ res >= len ? res : (len * 2);
    }
#अन्यथा
    फर्काऊ vsnprintf(buf, len, fmt, args);
#endif
}

स्थिर शून्य dumpException(अचर Exception& exc)
{
    अचर वर्ण* errorStr = exc.codeMessage();
    वर्ण buf[1 << 12];

    cv_snprintf(buf, परिमाण(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.पङ्क्ति);
#ifdef __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#अन्यथा
    ख_साफ(मानक_निकास); ख_साफ(मानक_त्रुटि);
    ख_लिखो(मानक_त्रुटि, "%s\n", buf);
    ख_साफ(मानक_त्रुटि);
#endif
}

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
स्थिर तार्किक cv_terminate_handler_installed = असत्य;
स्थिर std::terminate_handler cv_old_terminate_handler;
स्थिर cv::Exception cv_terminate_handler_exception;
स्थिर तार्किक param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", सत्य);
स्थिर शून्य cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    यदि (असत्य /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    पात();
}

#endif

#ifdef __GNUC__
# यदि defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

शून्य error( अचर Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        यदि (!cv_terminate_handler_installed)
        {
            यदि (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = सत्य;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    यदि (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.पङ्क्ति, customErrorCallbackData);
    अन्यथा यदि (param_dumpErrors)
    {
        dumpException(exc);
    }

    यदि(breakOnError)
    {
        std::terminate();
    }

    क्षिप exc;
#ifdef __GNUC__
# यदि !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

शून्य error(Error::Code _code, अचर String& _err, अचर वर्ण* _func, अचर वर्ण* _file, पूर्णाङ्क पङ्क्ति_3)
{
    error(cv::Exception(_code, _err, _func, _file, पङ्क्ति_3));
#ifdef __GNUC__
# यदि !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# यदि defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, शून्य* userdata, शून्य** prevUserdata)
{
    यदि( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    फर्काऊ prevCallback;
}

शून्य terminate(Error::Code _code, अचर String& _err, अचर वर्ण* _func, अचर वर्ण* _file, पूर्णाङ्क पङ्क्ति_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, पङ्क्ति_3));
    std::terminate();
}

}

नामाकाश cv {
तार्किक __termination = असत्य;


//////////////////////////////// thread-local storage ////////////////////////////////

नामाकाश details {

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
वर्ग TlsAbstraction
{
सार्वजनिक:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        ख_साफ(मानक_त्रुटि);
    }

    शून्य* getData() अचर;
    शून्य setData(शून्य *pData);

    शून्य releaseSystemResources();

गुप्त:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_साँचो;
    तार्किक disposed;
#endif
#अन्यथा // _WIN32
    pthread_key_t  tls_साँचो;
    std::atomic<तार्किक> disposed;
#endif
};

वर्ग TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
सार्वजनिक:
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
स्थिर TlsAbstraction* getTlsAbstraction()
{
    स्थिर TlsAbstraction *g_tls = नूतन TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    स्थिर TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    फर्काऊ g_tls;
}


#ifdef _WIN32
#ifdef WINRT
स्थिर __declspec( thread ) शून्य* tlsData = रिक्त; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
}
शून्य* TlsAbstraction::getData() अचर
{
    फर्काऊ tlsData;
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    tlsData = pData;
}
#अन्यथा //WINRT
#ifdef CV_USE_FLS
स्थिर शून्य NTAPI opencv_fls_destructor(शून्य* pData);
#endif // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(असत्य)
{
#ifndef CV_USE_FLS
    tls_साँचो = TlsAlloc();
#अन्यथा // CV_USE_FLS
    tls_साँचो = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_साँचो != TLS_OUT_OF_INDEXES);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
#ifndef CV_USE_FLS
    TlsFree(tls_साँचो);
#अन्यथा // CV_USE_FLS
    FlsFree(tls_साँचो);
#endif // CV_USE_FLS
    tls_साँचो = TLS_OUT_OF_INDEXES;
}
शून्य* TlsAbstraction::getData() अचर
{
    यदि (disposed)
        फर्काऊ रिक्त;
#ifndef CV_USE_FLS
    फर्काऊ Tls_Get_मान(tls_साँचो);
#अन्यथा // CV_USE_FLS
    फर्काऊ Fls_Get_मान(tls_साँचो);
#endif // CV_USE_FLS
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    यदि (disposed)
        फर्काऊ;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_मान(tls_साँचो, pData) == TRUE);
#अन्यथा // CV_USE_FLS
    CV_Assert(Fls_Set_मान(tls_साँचो, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#अन्यथा // _WIN32
स्थिर शून्य opencv_tls_destructor(शून्य* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(असत्य)
{
    CV_Assert(pthread_key_create(&tls_साँचो, opencv_tls_destructor) == 0);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
    यदि (pthread_key_delete(tls_साँचो) != 0)
    {
        // Don't use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        ख_साफ(मानक_त्रुटि);
    }
}
शून्य* TlsAbstraction::getData() अचर
{
    यदि (disposed)
        फर्काऊ रिक्त;
    फर्काऊ pthread_getspecific(tls_साँचो);
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    यदि (disposed)
        फर्काऊ;  // no-op
    CV_Assert(pthread_setspecific(tls_साँचो, pData) == 0);
}
#endif

// Per-thread data structure
संरचना ThreadData
{
    ThreadData()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<शून्य*> slots; // Data array for a thread
    माप_प्रकार idx;               // Thread index in TLS storage. This is not OS thread ID!
};


स्थिर तार्किक g_isTlsStorageInitialized = असत्य;

// Main TLS storage class
वर्ग TlsStorage
{
सार्वजनिक:
    TlsStorage() :
        tlsSlotsSize(0)
    {
        (शून्य)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = सत्य;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        ख_साफ(मानक_त्रुटि);
    }

    शून्य releaseThread(शून्य* tls_मान = रिक्त)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            फर्काऊ;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_मान == रिक्त ? (ThreadData*)tls->getData() : (ThreadData*)tls_मान;
        यदि (pTD == रिक्त)
            फर्काऊ;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        लागि (माप_प्रकार i = 0; i < threads.size(); i++)
        {
            यदि (pTD == threads[i])
            {
                threads[i] = रिक्त;
                यदि (tls_मान == रिक्त)
                    tls->setData(0);
                std::vector<शून्य*>& thread_slots = pTD->slots;
                लागि (माप_प्रकार slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    शून्य* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = रिक्त;
                    यदि (!pData)
                        जारी;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    यदि (container)
                        container->deleteDataInstance(pData);
                    अन्यथा
                    {
                        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (पूर्णाङ्क)slotIdx);
                        ख_साफ(मानक_त्रुटि);
                    }
                }
                delete pTD;
                फर्काऊ;
            }
        }
        ख_लिखो(मानक_त्रुटि, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (शून्य*)pTD); ख_साफ(मानक_त्रुटि);
    }

    // Reserve TLS storage index
    माप_प्रकार reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());

        // Find unused slots
        लागि(माप_प्रकार slot = 0; slot < tlsSlotsSize; slot++)
        {
            यदि (tlsSlots[slot].container == रिक्त)
            {
                tlsSlots[slot].container = container;
                फर्काऊ slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        फर्काऊ tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = असत्य)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        लागि(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            यदि(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                यदि (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = रिक्त;
                }
            }
        }

        यदि (!keepSlot)
        {
            tlsSlots[slotIdx].container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(माप_प्रकार slotIdx) अचर
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            फर्काऊ रिक्त;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        यदि(threadData && threadData->slots.size() > slotIdx)
            फर्काऊ threadData->slots[slotIdx];

        फर्काऊ रिक्त;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        लागि(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            यदि(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                यदि (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    dataVec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    शून्य setData(माप_प्रकार slotIdx, शून्य* pData)
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        यदि (रिक्त == tls)
            फर्काऊ;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        यदि(!threadData)
        {
            threadData = नूतन ThreadData;
            tls->setData((शून्य*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                तार्किक found = असत्य;
                // Find unused slots
                लागि(माप_प्रकार slot = 0; slot < threads.size(); slot++)
                {
                    यदि (threads[slot] == रिक्त)
                    {
                        threadData->idx = (पूर्णाङ्क)slot;
                        threads[slot] = threadData;
                        found = सत्य;
                        रोक;
                    }
                }

                यदि (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        यदि(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, रिक्त);
        }
        threadData->slots[slotIdx] = pData;
    }

गुप्त:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    माप_प्रकार tlsSlotsSize;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    संरचना TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<संरचना TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<ThreadData*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
स्थिर TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, नूतन TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
स्थिर शून्य opencv_tls_destructor(शून्य* pData)
{
    यदि (!g_isTlsStorageInitialized)
        फर्काऊ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#अन्यथा // _WIN32
#ifdef CV_USE_FLS
स्थिर शून्य WINAPI opencv_fls_destructor(शून्य* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    यदि (res && code != STILL_ACTIVE)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = सत्य;
    }

    यदि (!g_isTlsStorageInitialized)
        फर्काऊ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

स्थिर TlsStorage* अचर g_force_initialization_of_TlsStorage
#यदि defined __GNUC__
    __attribute__((unused))
#endif
    = &getTlsStorage();


#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
वर्ग TlsStorage
{
सार्वजनिक:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        लागि (माप_प्रकार slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            यदि (container && s.data)
            {
                container->deleteDataInstance(s.data);  // Can't use from SlotInfo destructor
                s.data = nullptr;
            }
        }
    }

    // Reserve TLS storage index
    माप_प्रकार reserveSlot(TLSDataContainer* container)
    {
        माप_प्रकार slotsSize = slots.size();
        लागि (माप_प्रकार slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            यदि (s.container == रिक्त)
            {
                CV_Assert(!s.data);
                s.container = container;
                फर्काऊ slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        फर्काऊ slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = असत्य)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        यदि (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        यदि (!keepSlot)
        {
            s.container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(माप_प्रकार slotIdx) अचर
    {
        CV_Assert(slotIdx < slots.size());
        अचर SlotInfo& s = slots[slotIdx];
        फर्काऊ s.data;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        यदि (data)
            dataVec.push_back(data);
        फर्काऊ;
    }

    // Set data to storage index
    शून्य setData(माप_प्रकार slotIdx, शून्य* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

गुप्त:
    संरचना SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), data(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        शून्य* data;
    };
    std::vector<संरचना SlotInfo> slots;
};

स्थिर TlsStorage& getTlsStorage()
{
    स्थिर TlsStorage g_storage;  // no threading
    फर्काऊ g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using नामाकाश details;

शून्य releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    यदि (!g_isTlsStorageInitialized)
        फर्काऊ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    साँचो_2 = (पूर्णाङ्क)getTlsStorage().reserveSlot(एतत्); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(साँचो_2 == -1); // Key must be released in child object
}

शून्य TLSDataContainer::gatherData(std::vector<शून्य*> &data) अचर
{
    getTlsStorage().gather(साँचो_2, data);
}

शून्य TLSDataContainer::detachData(std::vector<शून्य*> &data)
{
    getTlsStorage().releaseSlot(साँचो_2, data, सत्य);
}

शून्य TLSDataContainer::release()
{
    यदि (साँचो_2 == -1)
        फर्काऊ;  // already released
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(साँचो_2, data, असत्य); // Release key and get stored data for proper destruction
    साँचो_2 = -1;
    लागि(माप_प्रकार i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य TLSDataContainer::cleanup()
{
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(साँचो_2, data, सत्य); // Extract stored data with removal from TLS tables
    लागि(माप_प्रकार i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य* TLSDataContainer::getData() अचर
{
    CV_Assert(साँचो_2 != -1 && "Can't fetch data from terminated TLS container.");
    शून्य* pData = getTlsStorage().getData(साँचो_2); // Check if data was already allocated
    यदि(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        यत्न
        {
            getTlsStorage().setData(साँचो_2, pData);
        }
        गृहाण (...)
        {
            deleteDataInstance(pData);
            क्षिप;
        }
    }
    फर्काऊ pData;
}

स्थिर TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, नूतन TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    फर्काऊ getCoreTlsDataTLS().getRef();
}

#यदि defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    यदि (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        यदि (lpReserved != रिक्त) // called after ExitProcess() call
        {
            cv::__termination = सत्य;
        }
        अन्यथा
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    फर्काऊ TRUE;
}
#endif


नामाकाश {

#ifdef OPENCV_WITH_ITT
तार्किक overrideThreadName()
{
    स्थिर तार्किक param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", असत्य);
    फर्काऊ param;
}
#endif

स्थिर पूर्णाङ्क g_thread_अङ्क = 0;
वर्ग ThreadID {
सार्वजनिक:
    अचर पूर्णाङ्क id;
    ThreadID() :
        id(CV_XADD(&g_thread_अङ्क, 1))
    {
#ifdef OPENCV_WITH_ITT
        यदि (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

स्थिर TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, नूतन TLSData<ThreadID>());
}

} // namespace
पूर्णाङ्क utils::getThreadID() { फर्काऊ getThreadIDTLS().get()->id; }


वर्ग ParseError
{
    std::string bad_मान;
सार्वजनिक:
    ParseError(अचर std::string &bad_मान_2) :bad_मान(bad_मान_2) {}
    std::string toString(अचर std::string &param) अचर
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_मान;
        फर्काऊ out.str();
    }
};

ढाँचा <typename T>
T parseOption(अचर std::string &);

ढाँचा<>
अन्तःस्थ तार्किक parseOption(अचर std::string & value)
{
    यदि (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        फर्काऊ सत्य;
    }
    यदि (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        फर्काऊ असत्य;
    }
    क्षिप ParseError(value);
}

ढाँचा<>
अन्तःस्थ माप_प्रकार parseOption(अचर std::string &value)
{
    माप_प्रकार pos = 0;
    लागि (; pos < value.size(); pos++)
    {
        यदि (!है_अंक(value[pos]))
            रोक;
    }
    cv::String मान_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    माप_प्रकार v = (माप_प्रकार)std::stoull(मान_Str);
    यदि (suffixStr.length() == 0)
        फर्काऊ v;
    अन्यथा यदि (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        फर्काऊ v * 1024 * 1024;
    अन्यथा यदि (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        फर्काऊ v * 1024;
    क्षिप ParseError(value);
}

ढाँचा<>
अन्तःस्थ cv::String parseOption(अचर std::string &value)
{
    फर्काऊ value;
}

ढाँचा<>
अन्तःस्थ utils::Paths parseOption(अचर std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    अचर वर्ण sep = ';';
#अन्यथा
    अचर वर्ण sep = ':';
#endif
    माप_प्रकार start_pos = 0;
    जबसम्म (start_pos != std::string::npos)
    {
        अचर माप_प्रकार pos = value.find(sep, start_pos);
        अचर std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        यदि (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    फर्काऊ result;
}

स्थिर अन्तःस्थ अचर वर्ण * envRead(अचर वर्ण * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    फर्काऊ रिक्त;
#अन्यथा
    फर्काऊ दो_पर्या(name);
#endif
}

ढाँचा<typename T>
अन्तःस्थ T पठ(अचर std::string & k, अचर T & default_मान_2)
{
    यत्न
    {
        अचर वर्ण * res = envRead(k.c_str());
        यदि (res)
            फर्काऊ parseOption<T>(std::string(res));
    }
    गृहाण (अचर ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    फर्काऊ default_मान_2;
}

तार्किक utils::getConfigurationParameterBool(अचर वर्ण* name, तार्किक default_मान_2)
{
    फर्काऊ पठ<तार्किक>(name, default_मान_2);
}

माप_प्रकार utils::getConfigurationParameterSizeT(अचर वर्ण* name, माप_प्रकार default_मान_2)
{
    फर्काऊ पठ<माप_प्रकार>(name, default_मान_2);
}

std::string utils::getConfigurationParameterString(अचर वर्ण* name, अचर std::string & default_मान_2)
{
    फर्काऊ पठ<cv::String>(name, default_मान_2);
}

utils::Paths utils::getConfigurationParameterPaths(अचर वर्ण* name, अचर utils::Paths &default_मान_2)
{
    फर्काऊ पठ<utils::Paths>(name, default_मान_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, नूतन ImplCollector())
}

शून्य setImpl(पूर्णाङ्क flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

शून्य addImpl(पूर्णाङ्क flag, अचर वर्ण* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    यदि(func) // use lazy collection if name was not specified
    {
        माप_प्रकार index = getImplData().implCode.size();
        यदि(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

पूर्णाङ्क getImpl(std::vector<पूर्णाङ्क> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    फर्काऊ getImplData().implFlags; // return actual flags for lazy collection
}

तार्किक useCollection()
{
    फर्काऊ getImplData().useCollection;
}

शून्य setUseCollection(तार्किक flag)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().useCollection = flag;
}
#endif

नामाकाश instr
{
तार्किक useInstrumentation()
{
#ifdef ENABLE_INSTRUMENTATION
    फर्काऊ getInstrumentStruct().useInstr;
#अन्यथा
    फर्काऊ असत्य;
#endif
}

शून्य setUseInstrumentation(तार्किक flag)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#अन्यथा
    CV_UNUSED(flag);
#endif
}

Instr_गाँठो* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    फर्काऊ &getInstrumentStruct().जरा_गाँठो_2;
#अन्यथा
    फर्काऊ रिक्त;
#endif
}

शून्य resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().जरा_गाँठो_2.removeChilds();
    getInstrumentTLSStruct().p_Current_गाँठो = &getInstrumentStruct().जरा_गाँठो_2;
#endif
}

शून्य setFlags(FLAGS modeFlags)
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#अन्यथा
    CV_UNUSED(modeFlags);
#endif
}
FLAGS getFlags()
{
#ifdef ENABLE_INSTRUMENTATION
    फर्काऊ (FLAGS)getInstrumentStruct().flags;
#अन्यथा
    फर्काऊ (FLAGS)0;
#endif
}

गाँठो_Data::गाँठो_Data(अचर वर्ण* funName, अचर वर्ण* fileName, पूर्णाङ्क पङ्क्ति_अङ्क, शून्य* retAddress, तार्किक alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_पङ्क्ति_अङ्क       = पङ्क्ति_अङ्क;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = असत्य;
}
गाँठो_Data::गाँठो_Data(गाँठो_Data &ref)
{
    *एतत् = ref;
}
गाँठो_Data& गाँठो_Data::चालक=(अचर गाँठो_Data &दायाँ)
{
    एतत्->m_funName      = दायाँ.m_funName;
    एतत्->m_instrType    = दायाँ.m_instrType;
    एतत्->m_implType     = दायाँ.m_implType;
    एतत्->m_fileName     = दायाँ.m_fileName;
    एतत्->m_पङ्क्ति_अङ्क      = दायाँ.m_पङ्क्ति_अङ्क;
    एतत्->m_retAddress   = दायाँ.m_retAddress;
    एतत्->m_alwaysExpand = दायाँ.m_alwaysExpand;

    एतत्->m_threads     = दायाँ.m_threads;
    एतत्->m_counter     = दायाँ.m_counter;
    एतत्->m_ticksTotal  = दायाँ.m_ticksTotal;

    एतत्->m_funError    = दायाँ.m_funError;

    फर्काऊ *एतत्;
}
गाँठो_Data::~गाँठो_Data()
{
}
तार्किक चालक==(अचर गाँठो_Data& बायाँ, अचर गाँठो_Data& दायाँ)
{
    यदि(बायाँ.m_पङ्क्ति_अङ्क == दायाँ.m_पङ्क्ति_अङ्क && बायाँ.m_funName == दायाँ.m_funName && बायाँ.m_fileName == दायाँ.m_fileName)
    {
        यदि(बायाँ.m_retAddress == दायाँ.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || बायाँ.m_alwaysExpand))
            फर्काऊ सत्य;
    }
    फर्काऊ असत्य;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    स्थिर InstrStruct instr;
    फर्काऊ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    फर्काऊ *getInstrumentStruct().tlsStruct.get();
}

Instr_गाँठो* get_Current_गाँठो()
{
    फर्काऊ getInstrumentTLSStruct().p_Current_गाँठो;
}

IntrumentationRegion::IntrumentationRegion(अचर वर्ण* funName, अचर वर्ण* fileName, पूर्णाङ्क पङ्क्ति_अङ्क, शून्य *retAddress, तार्किक alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = असत्य;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    यदि(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        यदि(!pTLS->p_Current_गाँठो)
        {
            m_disabled = सत्य;
            फर्काऊ;
        }

        पूर्णाङ्क depth = pTLS->p_Current_गाँठो->getDepth();
        यदि(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = सत्य;
            फर्काऊ;
        }

        गाँठो_Data payload(funName, fileName, पङ्क्ति_अङ्क, retAddress, alwaysExpand, instrType, implType);
        Node<गाँठो_Data>* pChild = रिक्त;

        यदि(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_गाँठो->findChild(payload);
            यदि(!pChild)
            {
                pChild = नूतन Node<गाँठो_Data>(payload);
                pTLS->p_Current_गाँठो->addChild(pChild);
            }
        }
        अन्यथा
        {
            pChild = pTLS->p_Current_गाँठो->findChild(payload);
            यदि(!pChild)
            {
                m_disabled = सत्य;
                फर्काऊ;
            }
        }
        pTLS->p_Current_गाँठो = pChild;

        m_regionTicks = get_Tick_गन्ती();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    यदि(pStruct->useInstr)
    {
        यदि(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            यदि (pTLS->p_Current_गाँठो->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_गाँठो->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_गाँठो->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_गन्ती() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_गन्ती); // Concurrent ticks accumulation
                pTLS->p_Current_गाँठो->m_payload.m_counter++;
                pTLS->p_Current_गाँठो->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_गाँठो->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_गाँठो = pTLS->p_Current_गाँठो->m_pParent;
        }
    }
}
#endif
}

नामाकाश ipp
{

#ifdef HAVE_IPP
संरचना IPPInitSingleton
{
सार्वजनिक:
    IPPInitSingleton()
    {
        useIPP         = सत्य;

#यदि defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#अन्यथा
        useIPP_NE      = असत्य;
#endif
        ippStatus      = 0;
        funcname       = रिक्त;
        filename       = रिक्त;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_शीर्ष_Features = 0;
        pIppLibInfo    = रिक्त;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, रिक्त);
        यदि(ippStatus < 0)
        {
            CV_LOG_ERROR(रिक्त, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = असत्य;
            फर्काऊ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        यदि(!env.empty())
        {
#यदि IPP_VERSION_X100 >= 201900
            अचर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#अथवायदि IPP_VERSION_X100 >= 201703
            अचर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अथवायदि IPP_VERSION_X100 >= 201700
            अचर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अन्यथा
            अचर Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            यदि(env.substr(0, 2) == "ne")
            {
                useIPP_NE = सत्य;
                env = env.substr(3, env.size());
            }

            यदि(env == "disabled")
            {
                CV_LOG_WARNING(रिक्त, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = असत्य;
            }
            अन्यथा यदि(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            अन्यथा यदि(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#यदि IPP_VERSION_X100 >= 201700
#यदि defined (_M_AMD64) || defined (__x86_64__)
            अन्यथा यदि(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            अन्यथा
                CV_LOG_ERROR(रिक्त, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        यदि(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        यदि(!(
#यदि IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = असत्य;
            फर्काऊ;
        }

        यदि(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        अन्यथा
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#यदि IPP_VERSION_X100 >= 201700
        यदि(ippFeatures&ippCPUID_AVX512F)
        {
            यदि((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_शीर्ष_Features = ippCPUID_AVX512_SKX;
            अन्यथा यदि((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_शीर्ष_Features = ippCPUID_AVX512_KNL;
            अन्यथा
                ipp_शीर्ष_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        अन्यथा
#endif
        यदि(ippFeatures&ippCPUID_AVX2)
            ipp_शीर्ष_Features = ippCPUID_AVX2;
        अन्यथा यदि(ippFeatures&ippCPUID_SSE42)
            ipp_शीर्ष_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        यदि (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_शीर्ष_Features = ippCPUID_SSE42;
        }
    }

सार्वजनिक:
    तार्किक        useIPP;
    तार्किक        useIPP_NE;

    पूर्णाङ्क         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    अचर वर्ण *funcname;
    अचर वर्ण *filename;
    पूर्णाङ्क         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_शीर्ष_Features;
    अचर IppLibraryVersion *pIppLibInfo;
};

स्थिर IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, नूतन IPPInitSingleton())
}
#endif

अचिन्हित दीर्घ दीर्घ getIppFeatures()
{
#ifdef HAVE_IPP
    फर्काऊ getIPPSingleton().ippFeatures;
#अन्यथा
    फर्काऊ 0;
#endif
}

#ifdef HAVE_IPP
अचिन्हित दीर्घ दीर्घ get_Ipp_शीर्ष_Features()
{
    फर्काऊ getIPPSingleton().ipp_शीर्ष_Features;
}
#endif

शून्य setIppStatus(पूर्णाङ्क status, अचर वर्ण * अचर _funcname, अचर वर्ण * अचर _filename, पूर्णाङ्क पङ्क्ति_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = पङ्क्ति_3;
#अन्यथा
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(पङ्क्ति_3);
#endif
}

पूर्णाङ्क getIppStatus()
{
#ifdef HAVE_IPP
    फर्काऊ getIPPSingleton().ippStatus;
#अन्यथा
    फर्काऊ 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    फर्काऊ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#अन्यथा
    फर्काऊ String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    अचर IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    यदि(pInfo)
        फर्काऊ format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    अन्यथा
        फर्काऊ String("error");
#अन्यथा
    फर्काऊ String("disabled");
#endif
}

तार्किक useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    यदि (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    फर्काऊ (data.useIPP > 0);
#अन्यथा
    फर्काऊ असत्य;
#endif
}

शून्य setUseIPP(तार्किक flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP = (getIPPSingleton().useIPP)?flag:असत्य;
#अन्यथा
    CV_UNUSED(flag);
    data.useIPP = असत्य;
#endif
}

तार्किक useIPP_NotExact()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    यदि (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    फर्काऊ (data.useIPP_NE > 0);
#अन्यथा
    फर्काऊ असत्य;
#endif
}

शून्य setUseIPP_NotExact(तार्किक flag)
{
    CoreTLSData& data = getCoreTlsData();
#ifdef HAVE_IPP
    data.useIPP_NE = flag;
#अन्यथा
    CV_UNUSED(flag);
    data.useIPP_NE = असत्य;
#endif
}

} // namespace ipp


नामाकाश details {

#यदि OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

शून्य setFPDenormalsIgnoreHint(तार्किक ignore, CV_OUT FPDenormalsModeState& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    अचिन्हित value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    यदि (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    अचर अचिन्हित old_flags = _mm_getcsr();
    अचर अचिन्हित old_मान = old_flags & mask;
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मान;
    _mm_setcsr(flags);
#अन्यथा
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

पूर्णाङ्क saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    यदि (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    अचर अचिन्हित old_flags = _mm_getcsr();
    अचर अचिन्हित old_मान = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मान;
    फर्काऊ 2;
#अन्यथा
    CV_UNUSED(state);
    फर्काऊ 0;
#endif
}

तार्किक restoreFPDenormalsState(अचर FPDenormalsModeState& state)
{
#यदि OPENCV_IMPL_FP_HINTS_X86
    अचर अचिन्हित mask = (अचिन्हित)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    अचर अचिन्हित value = (अचिन्हित)state.reserved[1];
    CV_DbgCheck((पूर्णाङ्क)value, value == (value & mask), "invalid SSE FP state");
    अचर अचिन्हित old_flags = _mm_getcsr();
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    फर्काऊ सत्य;
#अन्यथा
    CV_UNUSED(state);
    फर्काऊ असत्य;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    फर्काऊ OPENCV_ALGO_HINT_DEFAULT;
#अन्यथा
    फर्काऊ ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
