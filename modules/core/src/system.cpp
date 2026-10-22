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
    जर (__initialization_mutex == रिक्त)
    {
        (शून्य)_initSystem();
        __initialization_mutex = नवीन Mutex();
    }
    परत *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

स्थिर तार्किक param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#जर defined(_DEBUG) || defined(__ANDROID__)
    सत्य
#अन्यथा
    असत्य
#endif
);

शून्य* allocSingletonBuffer(माप_प्रकार size) { परत fastMalloc(size); }
शून्य* allocSingletonNewBuffer(माप_प्रकार size) { परत दो_स्मृति(size); }


} // namespace cv

#ifndef CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# जर defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#endif
#जर defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#endif

#ifdef _MSC_VER
# जर _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#endif

#ifdef CV_ERROR_SET_TERMINATE_HANDLER
#अन्तर्भाव <cstdlib>        // std::abort
#endif

#जर defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#जर defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
स्थिरांक uint64_t AT_HWCAP = NT_GNU_HWCAP;
#अन्यथा
#  include <elf.h>
#endif
#जर defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#endif
#endif

#जर defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#endif


#जर ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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

#जर defined __loongarch64
#अन्तर्भाव "sys/auxv.h"
#define LA_HWCAP_LSX   (1<<4)
#define LA_HWCAP_LASX  (1<<5)
#endif

#जर defined _WIN32 || defined WINCE
#ifndef _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #define _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#endif
#अन्तर्भाव <windows.h>
#अन्तर्भाव <combaseapi.h>
#जर (_WIN32_WINNT >= 0x0602)
  #अन्तर्भाव <synchapi.h>
#endif
#जर ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
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
    परत std::wstring(Windows::Storage::ApplicationData::Current->TemporaryFolder->Path->Data());
#अन्यथा
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationDataStatics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplicationData> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstrHead;
    std::wstring wstr;
    जर (FAILED(WindowsCreateStringReference(RuntimeClass_Windows_Storage_ApplicationData,
                                            (UINT32)wcslen(RuntimeClass_Windows_Storage_ApplicationData), &hstrHead, &str)))
        परत wstr;
    जर (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        परत wstr;
    जर (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        परत wstr;
    जर (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        परत wstr;
    जर (FAILED(storagefolderRef.As(&storageitemRef)))
        परत wstr;
    str = रिक्त;
    जर (FAILED(storageitemRef->get_Path(&str)))
        परत wstr;
    wstr = WindowsGetStringRawBuffer(str, रिक्त);
    WindowsDeleteString(str);
    परत wstr;
#endif
}

std::wstring GetTempFileNameWinRT(std::wstring prefix)
{
    ब_अक्षर_प्रकार guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    ब_अक्षर_प्रकार* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], आकार(guidStr)/आकार(ब_अक्षर_प्रकार), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    परत prefix.append(std::wstring(guidStr));
}

#endif
#अन्यथा
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
#अन्तर्भाव <pthread.h>
#endif
#अन्तर्भाव <sys/time.h>
#अन्तर्भाव <time.h>

#जर defined __MACH__ && defined __APPLE__
#अन्तर्भाव <mach/mach.h>
#अन्तर्भाव <mach/mach_time.h>
#अन्तर्भाव <sys/sysctl.h>
#endif

#endif

#ifdef _OPENMP
#अन्तर्भाव "omp.h"
#endif

#जर defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <sys/types.h>
#जर defined __ANDROID__
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
  #जर defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #जर _MSC_VER >= 1400  // MSVS 2005
      #अन्तर्भाव <intrin.h>  // __cpuidex()
      #define CV_CPUID_X86 __cpuidex
    #अन्यथा
      #error "Required MSVS 2005+"
    #endif
  #किंवाजर defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    स्थिर शून्य cv_cpuid(पूर्णांक* cpuid_data, पूर्णांक reg_eax, पूर्णांक reg_ecx)
    {
        पूर्णांक __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#जर !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#किंवाजर defined(__i386__)  // ebx may be reserved as the PIC register
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

Exception::Exception() { code = Error::StsOk; ओळ = 0; }

Exception::Exception(Error::Code _code, स्थिरांक String& _err, स्थिरांक String& _func, स्थिरांक String& _file, पूर्णांक ओळ_3)
: code(_code), err(_err), func(_func), file(_file), ओळ(ओळ_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
स्थिरांक अक्षर* Exception::what() स्थिरांक CV_NOEXCEPT { परत संदेश.c_str(); }

शून्य Exception::formatMessage()
{
    माप_प्रकार pos = err.find('\n');
    तार्किक multiline = pos != cv::String::npos;
    जर (multiline)
    {
        std::stringstream ss;
        माप_प्रकार prev_pos = 0;
        पर्यंत (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::endl;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        जर (err[err.size() - 1] != '\n')
            ss << std::endl;
        err = ss.str();
    }
    जर (func.size() > 0)
    {
        जर (multiline)
            संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), ओळ, code, codeMessage(), func.c_str(), err.c_str());
        अन्यथा
            संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), ओळ, code, codeMessage(), err.c_str(), func.c_str());
    }
    अन्यथा
    {
        संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), ओळ, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

स्थिरांक अक्षर * Exception::codeMessage() स्थिरांक
{
    निवड (code)
    {
    स्थिती cv::Error::StsOk :                  परत "No Error";
    स्थिती cv::Error::StsBackTrace :           परत "Backtrace";
    स्थिती cv::Error::StsError :               परत "Unspecified error";
    स्थिती cv::Error::StsInternal :            परत "Internal error";
    स्थिती cv::Error::StsNoMem :               परत "Insufficient memory";
    स्थिती cv::Error::StsBadArg :              परत "Bad argument";
    स्थिती cv::Error::StsNoConv :              परत "Iterations do not converge";
    स्थिती cv::Error::StsAutoTrace :           परत "Autotrace call";
    स्थिती cv::Error::StsBadSize :             परत "Incorrect size of input array";
    स्थिती cv::Error::StsNullPtr :             परत "Null pointer";
    स्थिती cv::Error::StsDivByZero :           परत "Division by zero occurred";
    स्थिती cv::Error::BadStep :                परत "Image step is wrong";
    स्थिती cv::Error::StsInplaceNotSupported : परत "Inplace operation is not supported";
    स्थिती cv::Error::StsObjectNotFound :      परत "Requested object was not found";
    स्थिती cv::Error::BadDepth :               परत "Input image depth is not supported by function";
    स्थिती cv::Error::StsUnmatchedFormats :    परत "Formats of input arguments do not match";
    स्थिती cv::Error::StsUnmatchedSizes :      परत "Sizes of input arguments do not match";
    स्थिती cv::Error::StsOutOfRange :          परत "One of the arguments\' values is out of range";
    स्थिती cv::Error::StsUnsupportedFormat :   परत "Unsupported format or combination of formats";
    स्थिती cv::Error::BadCOI :                 परत "Input COI is not supported";
    स्थिती cv::Error::Bad_अंक_Channels :         परत "Bad number of channels";
    स्थिती cv::Error::StsBadFlag :             परत "Bad flag (parameter or structure field)";
    स्थिती cv::Error::StsBadPoint :            परत "Bad parameter of type CvPoint";
    स्थिती cv::Error::StsBadMask :             परत "Bad type of mask argument";
    स्थिती cv::Error::StsParseError :          परत "Parsing error";
    स्थिती cv::Error::StsNotImplemented :      परत "The function/feature is not implemented";
    स्थिती cv::Error::StsBadMemBlock :         परत "Memory block has been corrupted";
    स्थिती cv::Error::StsAssert :              परत "Assertion failed";
    स्थिती cv::Error::GpuNotSupported :        परत "No CUDA support";
    स्थिती cv::Error::GpuApiCallError :        परत "Gpu API call";
    स्थिती cv::Error::OpenGlNotSupported :     परत "No OpenGL support";
    स्थिती cv::Error::OpenGlApiCallError :     परत "OpenGL API call";
    स्थिती cv::Error::OpenCLApiCallError :     परत "OpenCL API call error";
    स्थिती cv::Error::OpenCLDoubleNotSupported:परत "OpenCL device does not support double";
    स्थिती cv::Error::OpenCLInitError :        परत "OpenCL initialization error";
    स्थिती cv::Error::OpenCLNoAMDBlasFft :     परत "Missing OpenCL AMD BLAS FFT";
    पूर्वनिर्धारित:
    {
        स्थिर अक्षर buf[256] {0};
        snprintf(buf, आकार(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        परत buf;
    }
    };

}

स्थिर स्थिरांक अक्षर* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { रिक्त };

स्थिर स्थिरांक अक्षर* getHWFeatureName(पूर्णांक id)
{
    परत (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : रिक्त;
}
स्थिर स्थिरांक अक्षर* getHWFeatureNameSafe(पूर्णांक id)
{
    स्थिरांक अक्षर* name = getHWFeatureName(id);
    परत name ? name : "Unknown feature";
}

संरचना HWFeatures
{
    गणना { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(तार्किक run_initialize = असत्य)
    {
        जर (run_initialize)
            initialize();
    }

    स्थिर शून्य initializeNames()
    {
        साठी (पूर्णांक i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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
        जर (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            ख_लिखो(मानक_त्रुटि, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #ifdef CV_CPUID_X86
        पूर्णांक cpuid_data[4] = { 0, 0, 0, 0 };
        पूर्णांक cpuid_data_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_data, 1, 0/*unused*/);

        पूर्णांक x86_family = (cpuid_data[0] >> 8) & 15;
        जर( x86_family >= 6 )
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
            पूर्णांक cpuid_data_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_data_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_data_ex1[0] & (1<<4))  != 0;

            तार्किक have_AVX_OS_support = सत्य;
            तार्किक have_AVX512_OS_support = सत्य;
            जर (!(cpuid_data[2] & (1<<27)))
                have_AVX_OS_support = असत्य; // OS uses XSAVE_XRSTORE and CPU support AVX
            अन्यथा
            {
                पूर्णांक xcr0 = 0;
            #ifdef _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (पूर्णांक)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #किंवाजर defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #endif
                जर ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = असत्य; // YMM registers
                जर ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = असत्य; // ZMM registers
            }

            जर (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = असत्य;
                have[CV_CPU_FP16] = असत्य;
                have[CV_CPU_AVX2] = असत्य;
                have[CV_CPU_FMA3] = असत्य;
                have[CV_CPU_AVX_VNNI] = असत्य;
            }
            जर (!have_AVX_OS_support || !have_AVX512_OS_support)
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
            जर (have[CV_CPU_AVX512_COMMON])
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

    #जर defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ifdef __aarch64__
        have[CV_CPU_NEON] = सत्य;
        have[CV_CPU_FP16] = सत्य;
        पूर्णांक cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        जर (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            स्थिरांक माप_प्रकार size_auxv_t = आकार(auxv);

            पर्यंत ((माप_प्रकार)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                जर (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#जर defined(AT_HWCAP2)
                अन्यथा जर (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#endif
            }

            बंद(cpufile);
        }
    #किंवाजर defined __arm__ && defined __ANDROID__
      #जर defined HAVE_CPUFEATURES
        CV_LOG_INFO(रिक्त, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(रिक्त, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (दीर्घ दीर्घ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #अन्यथा
        CV_LOG_INFO(रिक्त, "cpufeatures library is not available for CPU detection");
        #जर CV_NEON
        CV_LOG_INFO(रिक्त, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- NEON instructions is NOT enabled via build flags");
        #endif
        #जर CV_FP16
        CV_LOG_INFO(रिक्त, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = सत्य;
        #अन्यथा
        CV_LOG_INFO(रिक्त, "- FP16 instructions is NOT enabled via build flags");
        #endif
      #endif
    #किंवाजर defined __arm__
        पूर्णांक cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        जर (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            स्थिरांक माप_प्रकार size_auxv_t = आकार(auxv);

            पर्यंत ((माप_प्रकार)पठ(cpufile, &auxv, size_auxv_t) == size_auxv_t)
            {
                जर (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    थांबा;
                }
            }

            बंद(cpufile);
        }
    #endif
    #किंवाजर (defined __APPLE__)
    #जर defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
    #endif
    #जर (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = सत्य;
    #endif
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    पूर्णांक has_feat_dotprod = 0;
    माप_प्रकार has_feat_dotprod_size = आकार(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_size, रिक्त, 0);
    जर (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = सत्य;
    }
    पूर्णांक has_feat_bf16 = 0;
    माप_प्रकार has_feat_bf16_size = आकार(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_size, रिक्त, 0);
    जर (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = सत्य;
    }
    #किंवाजर (defined __clang__)
    #जर defined __ARM_NEON
        have[CV_CPU_NEON] = सत्य;
        #जर (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = सत्य;
        #endif
    #endif
    #endif
    #जर defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = सत्य;
    #endif
    #जर defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = सत्य;
    #endif
    #ifdef __riscv_vector
        have[CV_CPU_RISCVV] = सत्य;
    #endif
    #ifdef __mips_msa
        have[CV_CPU_MSA] = सत्य;
    #endif

    #जर (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        अचिन्हित पूर्णांक hwcap = getauxval(AT_HWCAP);
        जर (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            जर (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #किंवाजर (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        अचिन्हित दीर्घ hwcap = 0, hwcap2 = 0;
        जर (elf_aux_info(AT_HWCAP, &hwcap, आकार(hwcap)) != 0)
            hwcap = 0;
        जर (hwcap & PPC_FEATURE_HAS_VSX) {
            जर (elf_aux_info(AT_HWCAP2, &hwcap2, आकार(hwcap2)) != 0)
                hwcap2 = 0;
            जर (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = सत्य;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अन्यथा
        // TODO: AIX
        #जर CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = सत्य;
        #endif
        #जर CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = सत्य;
        #endif
    #endif

    #जर defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = सत्य;
        #जर (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = सत्य;
        #endif
        #जर defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = सत्य;
        #endif
    #endif

    #जर defined __loongarch64 && defined __linux__
        पूर्णांक flag = (पूर्णांक)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #endif

        तार्किक skip_baseline_check = असत्य;
        जर (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = सत्य;
        }
        पूर्णांक baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        जर (!checkFeatures(baseline_features, आकार(baseline_features) / आकार(baseline_features[0]))
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
            checkFeatures(baseline_features, आकार(baseline_features) / आकार(baseline_features[0]), सत्य);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, आकार(baseline_features) / आकार(baseline_features[0]));
    }

    तार्किक checkFeatures(स्थिरांक पूर्णांक* features, पूर्णांक मोजणी_2, तार्किक dump = असत्य)
    {
        तार्किक result = सत्य;
        साठी (पूर्णांक i = 0; i < मोजणी_2; i++)
        {
            पूर्णांक feature = features[i];
            जर (feature)
            {
                जर (have[feature])
                {
                    जर (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                अन्यथा
                {
                    result = असत्य;
                    जर (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        परत result;
    }

    स्थिर ओळीत तार्किक is_चिन्ह_Separator(अक्षर c)
    {
        परत c == ',' || c == ';';
    }

    शून्य readSettings(स्थिरांक पूर्णांक* baseline_features, पूर्णांक baseline_मोजणी)
    {
        तार्किक dump = सत्य;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        जर (!disabled_features.empty())
        {
            स्थिरांक अक्षर* start = disabled_features.c_str();
            साठी (;;)
            {
                पर्यंत (start[0] != 0 && is_चिन्ह_Separator(start[0]))
                {
                    start++;
                }
                जर (start[0] == 0)
                    थांबा;
                स्थिरांक अक्षर* end = start;
                पर्यंत (end[0] != 0 && !is_चिन्ह_Separator(end[0]))
                {
                    end++;
                }
                जर (end == start)
                    सुरूठेवा;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                तार्किक found = असत्य;
                साठी (पूर्णांक i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    जर (!g_hwFeatureNames[i]) सुरूठेवा;
                    माप_प्रकार len = म_माप(g_hwFeatureNames[i]);
                    जर (len != feature.size()) सुरूठेवा;
                    जर (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        तार्किक isBaseline = असत्य;
                        साठी (पूर्णांक k = 0; k < baseline_मोजणी; k++)
                        {
                            जर (baseline_features[k] == i)
                            {
                                isBaseline = सत्य;
                                थांबा;
                            }
                        }
                        जर (isBaseline)
                        {
                            जर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        जर (!have[i])
                        {
                            जर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = असत्य;

                        found = सत्य;
                        थांबा;
                    }
                }
                जर (!found)
                {
                    जर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    तार्किक have[MAX_FEATURE+1]{};
};

स्थिर HWFeatures  featuresEnabled(सत्य), featuresDisabled = HWFeatures(असत्य);
स्थिर HWFeatures* currentFeatures = &featuresEnabled;

तार्किक checkHardwareSupport(पूर्णांक feature)
{
    जर (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        परत असत्य;
    परत currentFeatures->have[feature];
}

String getHardwareFeatureName(पूर्णांक feature)
{
    स्थिरांक अक्षर* name = getHWFeatureName(feature);
    परत name ? String(name) : String();
}

std::string get_CPUFeatures_ओळ()
{
    स्थिरांक पूर्णांक features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    स्थिरांक पूर्णांक sz = आकार(features) / आकार(features[0]);
    std::string result;
    std::string prefix;
    साठी (पूर्णांक i = 1; i < sz; ++i)
    {
        जर (features[i] == 0)
        {
            prefix = "*";
            सुरूठेवा;
        }
        जर (i != 1) result.append(" ");
        result.append(prefix);
        result.append(getHWFeatureNameSafe(features[i]));
        जर (!checkHardwareSupport(features[i])) result.append("?");
    }
    परत result;
}

अस्थिर तार्किक useOptimizedFlag = सत्य;

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
    परत useOptimizedFlag;
}

int64 get_Tick_मोजणी(शून्य)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    परत (int64)now.time_since_epoch().मोजणी_2();
}

द्विगुण getTickFrequency(शून्य)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    द्विगुण clock_freq = clock_period_t::den / clock_period_t::num;
    परत clock_freq;
}

#जर defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#जर defined(__i386__)

int64 get_CPUTick_मोजणी(शून्य)
{
    int64 x;
    __asm__ अस्थिर (".byte 0x0f, 0x31" : "=A" (x));
    परत x;
}
#किंवाजर defined(__x86_64__)

int64 get_CPUTick_मोजणी(शून्य)
{
    अचिन्हित hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    परत (int64)lo | ((int64)hi << 32);
}

#किंवाजर defined(__ppc__)

int64 get_CPUTick_मोजणी(शून्य)
{
    अचिन्हित upper, lower, tmp;
    __asm__ अस्थिर(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    परत lower | ((int64)upper << 32);
}

#अन्यथा

#error "RDTSC not defined"

#endif

#किंवाजर defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_मोजणी(शून्य)
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
int64 get_CPUTick_मोजणी(शून्य)
{
    परत get_Tick_मोजणी();
}
//#endif

#endif


नामाकाश internal {

वर्ग Timestamp
{
सार्वजनिक:
    स्थिरांक int64 zero_Tick_मोजणी;
    स्थिरांक द्विगुण ns_in_ticks;

    Timestamp()
        : zero_Tick_मोजणी(get_Tick_मोजणी())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_मोजणी();
        परत (int64)((t - zero_Tick_मोजणी) * ns_in_ticks);
    }

    स्थिर Timestamp& getInstance()
    {
        स्थिर Timestamp g_timestamp;
        परत g_timestamp;
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
    परत internal::Timestamp::getInstance().getTimestamp();
}


स्थिरांक String& getBuildInformation()
{
    स्थिर String build_info =
#अन्तर्भाव "version_string.inc"
    ;
    परत build_info;
}

String getVersionString() { परत String(CV_VERSION); }

पूर्णांक getVersionMajor() { परत CV_VERSION_MAJOR; }

पूर्णांक getVersionMinor() { परत CV_VERSION_MINOR; }

पूर्णांक getVersionRevision() { परत CV_VERSION_REVISION; }

String format( स्थिरांक अक्षर* fmt, ... )
{
    AutoBuffer<अक्षर, 1024> buf;

    साठी ( ; ; )
    {
        बहु_सूची va;
        बहु_शुरू(va, fmt);
        पूर्णांक bsize = static_cast<पूर्णांक>(buf.size());
        पूर्णांक len = cv_vsnprintf(buf.data(), bsize, fmt, va);
        बहु_पूर्ण(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        जर (len >= bsize)
        {
            buf.resize(len + 1);
            सुरूठेवा;
        }
        buf[bsize - 1] = 0;
        परत String(buf.data(), len);
    }
}

String tempfile( स्थिरांक अक्षर* suffix )
{
#जर OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string temp_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#जर defined _WIN32
#ifdef WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring temp_dir_rt = GetTempPathWinRT();

    std::wstring temp_file = GetTempFileNameWinRT(L"ocv");
    जर (temp_file.empty())
        परत String();

    temp_file = temp_dir_rt.append(std::wstring(L"\\")).append(temp_file);
    DeleteFileW(temp_file.c_str());

    अक्षर aname[MAX_PATH];
    माप_प्रकार copied = wcstombs(aname, temp_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
    fname = String(aname);
    RoUninitialize();
#किंवाजर defined(_WIN32_WCE)
    स्थिरांक स्वतः kMaxPathSize = MAX_PATH+1;
    ब_अक्षर_प्रकार temp_dir_ce[kMaxPathSize] = {0};
    ब_अक्षर_प्रकार temp_file[kMaxPathSize] = {0};

    ::GetTempPathW(kMaxPathSize, temp_dir_ce);

    जर(0 != ::GetTempFileNameW(temp_dir_ce, L"ocv", 0, temp_file)) {
        DeleteFileW(temp_file);
        अक्षर aname[MAX_PATH];
        माप_प्रकार copied = wcstombs(aname, temp_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        fname = String(aname);
    }
#अन्यथा
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    ब_अक्षर_प्रकार temp_dir2_w[MAX_PATH] = { 0 };

    जर (temp_dir.empty())
    {
        ::GetTempPathW(आकार(temp_dir2_w)/आकार(ब_अक्षर_प्रकार), temp_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        पूर्णांक len = WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, रिक्त, 0, रिक्त, रिक्त);
        जर (len > 0)
        {
            std::vector<अक्षर> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, temp_dir2_w, -1, utf8_buf.data(), len, रिक्त, रिक्त);
            temp_dir = std::string(utf8_buf.data());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    जर (FAILED(hr))
        परत String();
    अक्षर guidStr[40];
    स्थिरांक अक्षर* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, आकार(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (अचिन्हित पूर्णांक)g.Data4[0], (अचिन्हित पूर्णांक)g.Data4[1],
            (अचिन्हित पूर्णांक)g.Data4[2], (अचिन्हित पूर्णांक)g.Data4[3], (अचिन्हित पूर्णांक)g.Data4[4],
            (अचिन्हित पूर्णांक)g.Data4[5], (अचिन्हित पूर्णांक)g.Data4[6], (अचिन्हित पूर्णांक)g.Data4[7]);

    fname = temp_dir;
    जर (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#endif
# अन्यथा
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    अक्षर defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  अन्यथा
    अक्षर defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    जर (temp_dir.empty())
        fname = defaultTemplate;
    अन्यथा
    {
        fname = temp_dir;
        अक्षर ech = fname[fname.size() - 1];
        जर(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    स्थिरांक पूर्णांक fd = mkstemp((अक्षर*)fname.c_str());
    जर (fd == -1) परत String();

    बंद(fd);
    हटाओ(fname.c_str());
# endif

    जर (suffix)
    {
        जर (suffix[0] != '.')
            परत fname + "." + suffix;
        अन्यथा
            परत fname + suffix;
    }
    परत fname;
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
    परत prevVal;
}

पूर्णांक cv_snprintf(अक्षर* buf, पूर्णांक len, स्थिरांक अक्षर* fmt, ...)
{
    बहु_सूची va;
    बहु_शुरू(va, fmt);
    पूर्णांक res = cv_vsnprintf(buf, len, fmt, va);
    बहु_पूर्ण(va);
    परत res;
}

पूर्णांक cv_vsnprintf(अक्षर* buf, पूर्णांक len, स्थिरांक अक्षर* fmt, बहु_सूची args)
{
#जर defined _MSC_VER
    जर (len <= 0) परत len == 0 ? 1024 : -1;
    पूर्णांक res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    जर (res >= 0 && res < len)
    {
        buf[res] = 0;
        परत res;
    }
    अन्यथा
    {
        buf[len - 1] = 0; // truncate happened
        परत res >= len ? res : (len * 2);
    }
#अन्यथा
    परत vsnprintf(buf, len, fmt, args);
#endif
}

स्थिर शून्य dumpException(स्थिरांक Exception& exc)
{
    स्थिरांक अक्षर* errorStr = exc.codeMessage();
    अक्षर buf[1 << 12];

    cv_snprintf(buf, आकार(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.ओळ);
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
    जर (असत्य /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    पात();
}

#endif

#ifdef __GNUC__
# जर defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#endif

शून्य error( स्थिरांक Exception& exc )
{
#ifdef CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        जर (!cv_terminate_handler_installed)
        {
            जर (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = सत्य;
        }
        cv_terminate_handler_exception = exc;
    }
#endif

    जर (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.ओळ, customErrorCallbackData);
    अन्यथा जर (param_dumpErrors)
    {
        dumpException(exc);
    }

    जर(breakOnError)
    {
        std::terminate();
    }

    फेका exc;
#ifdef __GNUC__
# जर !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

शून्य error(Error::Code _code, स्थिरांक String& _err, स्थिरांक अक्षर* _func, स्थिरांक अक्षर* _file, पूर्णांक ओळ_3)
{
    error(cv::Exception(_code, _err, _func, _file, ओळ_3));
#ifdef __GNUC__
# जर !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#endif
}

#ifdef __GNUC__
# जर defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#endif


ErrorCallback
redirectError( ErrorCallback errCallback, शून्य* userdata, शून्य** prevUserdata)
{
    जर( prevUserdata )
        *prevUserdata = customErrorCallbackData;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    customErrorCallbackData = userdata;

    परत prevCallback;
}

शून्य terminate(Error::Code _code, स्थिरांक String& _err, स्थिरांक अक्षर* _func, स्थिरांक अक्षर* _file, पूर्णांक ओळ_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, ओळ_3));
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

    शून्य* getData() स्थिरांक;
    शून्य setData(शून्य *pData);

    शून्य releaseSystemResources();

खाजगी:

#ifdef _WIN32
#ifndef WINRT
    DWORD tls_किल्ली;
    तार्किक disposed;
#endif
#अन्यथा // _WIN32
    pthread_key_t  tls_किल्ली;
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
    स्थिर TlsAbstraction *g_tls = नवीन TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    स्थिर TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    परत g_tls;
}


#ifdef _WIN32
#ifdef WINRT
स्थिर __declspec( thread ) शून्य* tlsData = रिक्त; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
}
शून्य* TlsAbstraction::getData() स्थिरांक
{
    परत tlsData;
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
    tls_किल्ली = TlsAlloc();
#अन्यथा // CV_USE_FLS
    tls_किल्ली = FlsAlloc(opencv_fls_destructor);
#endif // CV_USE_FLS
    CV_Assert(tls_किल्ली != TLS_OUT_OF_INDEXES);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
#ifndef CV_USE_FLS
    TlsFree(tls_किल्ली);
#अन्यथा // CV_USE_FLS
    FlsFree(tls_किल्ली);
#endif // CV_USE_FLS
    tls_किल्ली = TLS_OUT_OF_INDEXES;
}
शून्य* TlsAbstraction::getData() स्थिरांक
{
    जर (disposed)
        परत रिक्त;
#ifndef CV_USE_FLS
    परत Tls_Get_मूल्य(tls_किल्ली);
#अन्यथा // CV_USE_FLS
    परत Fls_Get_मूल्य(tls_किल्ली);
#endif // CV_USE_FLS
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    जर (disposed)
        परत;  // no-op
#ifndef CV_USE_FLS
    CV_Assert(Tls_Set_मूल्य(tls_किल्ली, pData) == TRUE);
#अन्यथा // CV_USE_FLS
    CV_Assert(Fls_Set_मूल्य(tls_किल्ली, pData) == TRUE);
#endif // CV_USE_FLS
}
#endif // WINRT
#अन्यथा // _WIN32
स्थिर शून्य opencv_tls_destructor(शून्य* pData);
TlsAbstraction::TlsAbstraction()
    : disposed(असत्य)
{
    CV_Assert(pthread_key_create(&tls_किल्ली, opencv_tls_destructor) == 0);
}
शून्य TlsAbstraction::releaseSystemResources()
{
    cv::__termination = सत्य;  // DllMain is missing in static builds
    disposed = सत्य;
    जर (pthread_key_delete(tls_किल्ली) != 0)
    {
        // Don't use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        ख_साफ(मानक_त्रुटि);
    }
}
शून्य* TlsAbstraction::getData() स्थिरांक
{
    जर (disposed)
        परत रिक्त;
    परत pthread_getspecific(tls_किल्ली);
}
शून्य TlsAbstraction::setData(शून्य *pData)
{
    जर (disposed)
        परत;  // no-op
    CV_Assert(pthread_setspecific(tls_किल्ली, pData) == 0);
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

    शून्य releaseThread(शून्य* tls_मूल्य = रिक्त)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        जर (रिक्त == tls)
            परत;  // TLS singleton is not available (terminated)
        ThreadData *pTD = tls_मूल्य == रिक्त ? (ThreadData*)tls->getData() : (ThreadData*)tls_मूल्य;
        जर (pTD == रिक्त)
            परत;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        साठी (माप_प्रकार i = 0; i < threads.size(); i++)
        {
            जर (pTD == threads[i])
            {
                threads[i] = रिक्त;
                जर (tls_मूल्य == रिक्त)
                    tls->setData(0);
                std::vector<शून्य*>& thread_slots = pTD->slots;
                साठी (माप_प्रकार slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    शून्य* pData = thread_slots[slotIdx];
                    thread_slots[slotIdx] = रिक्त;
                    जर (!pData)
                        सुरूठेवा;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    जर (container)
                        container->deleteDataInstance(pData);
                    अन्यथा
                    {
                        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (पूर्णांक)slotIdx);
                        ख_साफ(मानक_त्रुटि);
                    }
                }
                delete pTD;
                परत;
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
        साठी(माप_प्रकार slot = 0; slot < tlsSlotsSize; slot++)
        {
            जर (tlsSlots[slot].container == रिक्त)
            {
                tlsSlots[slot].container = container;
                परत slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tlsSlotsSize++;
        परत tlsSlotsSize - 1;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = असत्य)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        साठी(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            जर(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                जर (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    dataVec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = रिक्त;
                }
            }
        }

        जर (!keepSlot)
        {
            tlsSlots[slotIdx].container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(माप_प्रकार slotIdx) स्थिरांक
    {
#ifndef CV_THREAD_SANITIZER
        CV_Assert(tlsSlotsSize > slotIdx);
#endif

        TlsAbstraction* tls = getTlsAbstraction();
        जर (रिक्त == tls)
            परत रिक्त;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        जर(threadData && threadData->slots.size() > slotIdx)
            परत threadData->slots[slotIdx];

        परत रिक्त;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tlsSlotsSize == tlsSlots.size());
        CV_Assert(tlsSlotsSize > slotIdx);

        साठी(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            जर(threads[i])
            {
                std::vector<शून्य*>& thread_slots = threads[i]->slots;
                जर (thread_slots.size() > slotIdx && thread_slots[slotIdx])
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
        जर (रिक्त == tls)
            परत;  // TLS singleton is not available (terminated)

        ThreadData* threadData = (ThreadData*)tls->getData();
        जर(!threadData)
        {
            threadData = नवीन ThreadData;
            tls->setData((शून्य*)threadData);
            {
                AutoLock guard(mtxGlobalAccess);

                तार्किक found = असत्य;
                // Find unused slots
                साठी(माप_प्रकार slot = 0; slot < threads.size(); slot++)
                {
                    जर (threads[slot] == रिक्त)
                    {
                        threadData->idx = (पूर्णांक)slot;
                        threads[slot] = threadData;
                        found = सत्य;
                        थांबा;
                    }
                }

                जर (!found)
                {
                    // Create new slot
                    threadData->idx = threads.size();
                    threads.push_back(threadData);
                }
            }
        }

        जर(slotIdx >= threadData->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            threadData->slots.resize(slotIdx + 1, रिक्त);
        }
        threadData->slots[slotIdx] = pData;
    }

खाजगी:
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
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, नवीन TlsStorage())
}

#ifndef _WIN32  // pthread key destructor
स्थिर शून्य opencv_tls_destructor(शून्य* pData)
{
    जर (!g_isTlsStorageInitialized)
        परत;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#अन्यथा // _WIN32
#ifdef CV_USE_FLS
स्थिर शून्य WINAPI opencv_fls_destructor(शून्य* pData)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    जर (res && code != STILL_ACTIVE)
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

    जर (!g_isTlsStorageInitialized)
        परत;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(pData);
}
#endif // CV_USE_FLS
#endif // _WIN32

स्थिर TlsStorage* स्थिरांक g_force_initialization_of_TlsStorage
#जर defined __GNUC__
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
        साठी (माप_प्रकार slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            जर (container && s.data)
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
        साठी (माप_प्रकार slot = 0; slot < slotsSize; slot++)
        {
            SlotInfo& s = slots[slot];
            जर (s.container == रिक्त)
            {
                CV_Assert(!s.data);
                s.container = container;
                परत slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        परत slotsSize;
    }

    // Release TLS storage index and pass associated data to caller
    शून्य releaseSlot(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec, तार्किक keepSlot = असत्य)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        जर (data)
        {
            dataVec.push_back(data);
            s.data = nullptr;
        }
        जर (!keepSlot)
        {
            s.container = रिक्त;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    शून्य* getData(माप_प्रकार slotIdx) स्थिरांक
    {
        CV_Assert(slotIdx < slots.size());
        स्थिरांक SlotInfo& s = slots[slotIdx];
        परत s.data;
    }

    // Gather data from threads by TLS storage index
    शून्य gather(माप_प्रकार slotIdx, std::vector<शून्य*> &dataVec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        शून्य* data = s.data;
        जर (data)
            dataVec.push_back(data);
        परत;
    }

    // Set data to storage index
    शून्य setData(माप_प्रकार slotIdx, शून्य* pData)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.data = pData;
    }

खाजगी:
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
    परत g_storage;
}

#endif  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using नामाकाश details;

शून्य releaseTlsStorageThread()
{
#ifndef OPENCV_DISABLE_THREAD_SUPPORT
    जर (!g_isTlsStorageInitialized)
        परत;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#endif
}

TLSDataContainer::TLSDataContainer()
{
    किल्ली_2 = (पूर्णांक)getTlsStorage().reserveSlot(हे); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(किल्ली_2 == -1); // Key must be released in child object
}

शून्य TLSDataContainer::gatherData(std::vector<शून्य*> &data) स्थिरांक
{
    getTlsStorage().gather(किल्ली_2, data);
}

शून्य TLSDataContainer::detachData(std::vector<शून्य*> &data)
{
    getTlsStorage().releaseSlot(किल्ली_2, data, सत्य);
}

शून्य TLSDataContainer::release()
{
    जर (किल्ली_2 == -1)
        परत;  // already released
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(किल्ली_2, data, असत्य); // Release key and get stored data for proper destruction
    किल्ली_2 = -1;
    साठी(माप_प्रकार i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य TLSDataContainer::cleanup()
{
    std::vector<शून्य*> data; data.reserve(32);
    getTlsStorage().releaseSlot(किल्ली_2, data, सत्य); // Extract stored data with removal from TLS tables
    साठी(माप_प्रकार i = 0; i < data.size(); i++)  // Delete all associated data
        deleteDataInstance(data[i]);
}

शून्य* TLSDataContainer::getData() स्थिरांक
{
    CV_Assert(किल्ली_2 != -1 && "Can't fetch data from terminated TLS container.");
    शून्य* pData = getTlsStorage().getData(किल्ली_2); // Check if data was already allocated
    जर(!pData)
    {
        // Create new data instance and save it to TLS storage
        pData = createDataInstance();
        प्रयत्न
        {
            getTlsStorage().setData(किल्ली_2, pData);
        }
        पकडा (...)
        {
            deleteDataInstance(pData);
            फेका;
        }
    }
    परत pData;
}

स्थिर TLSData<CoreTLSData>& getCoreTlsDataTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, नवीन TLSData<CoreTLSData>())
}

CoreTLSData& getCoreTlsData()
{
    परत getCoreTlsDataTLS().getRef();
}

#जर defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ifdef WINRT
    #pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
#endif

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    जर (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        जर (lpReserved != रिक्त) // called after ExitProcess() call
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
    परत TRUE;
}
#endif


नामाकाश {

#ifdef OPENCV_WITH_ITT
तार्किक overrideThreadName()
{
    स्थिर तार्किक param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", असत्य);
    परत param;
}
#endif

स्थिर पूर्णांक g_thread_अंक = 0;
वर्ग ThreadID {
सार्वजनिक:
    स्थिरांक पूर्णांक id;
    ThreadID() :
        id(CV_XADD(&g_thread_अंक, 1))
    {
#ifdef OPENCV_WITH_ITT
        जर (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#endif
    }
};

स्थिर TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, नवीन TLSData<ThreadID>());
}

} // namespace
पूर्णांक utils::getThreadID() { परत getThreadIDTLS().get()->id; }


वर्ग ParseError
{
    std::string bad_मूल्य;
सार्वजनिक:
    ParseError(स्थिरांक std::string &bad_मूल्य_2) :bad_मूल्य(bad_मूल्य_2) {}
    std::string toString(स्थिरांक std::string &param) स्थिरांक
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_मूल्य;
        परत out.str();
    }
};

ढाँचा <typename T>
T parseOption(स्थिरांक std::string &);

ढाँचा<>
ओळीत तार्किक parseOption(स्थिरांक std::string & value)
{
    जर (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        परत सत्य;
    }
    जर (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        परत असत्य;
    }
    फेका ParseError(value);
}

ढाँचा<>
ओळीत माप_प्रकार parseOption(स्थिरांक std::string &value)
{
    माप_प्रकार pos = 0;
    साठी (; pos < value.size(); pos++)
    {
        जर (!है_अंक(value[pos]))
            थांबा;
    }
    cv::String मूल्य_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    माप_प्रकार v = (माप_प्रकार)std::stoull(मूल्य_Str);
    जर (suffixStr.length() == 0)
        परत v;
    अन्यथा जर (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        परत v * 1024 * 1024;
    अन्यथा जर (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        परत v * 1024;
    फेका ParseError(value);
}

ढाँचा<>
ओळीत cv::String parseOption(स्थिरांक std::string &value)
{
    परत value;
}

ढाँचा<>
ओळीत utils::Paths parseOption(स्थिरांक std::string &value)
{
    utils::Paths result;
#ifdef _WIN32
    स्थिरांक अक्षर sep = ';';
#अन्यथा
    स्थिरांक अक्षर sep = ':';
#endif
    माप_प्रकार start_pos = 0;
    पर्यंत (start_pos != std::string::npos)
    {
        स्थिरांक माप_प्रकार pos = value.find(sep, start_pos);
        स्थिरांक std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        जर (!one_piece.empty())
            result.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    परत result;
}

स्थिर ओळीत स्थिरांक अक्षर * envRead(स्थिरांक अक्षर * name)
{
#ifdef NO_GETENV
    CV_UNUSED(name);
    परत रिक्त;
#अन्यथा
    परत दो_पर्या(name);
#endif
}

ढाँचा<typename T>
ओळीत T पठ(स्थिरांक std::string & k, स्थिरांक T & default_मूल्य_2)
{
    प्रयत्न
    {
        स्थिरांक अक्षर * res = envRead(k.c_str());
        जर (res)
            परत parseOption<T>(std::string(res));
    }
    पकडा (स्थिरांक ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    परत default_मूल्य_2;
}

तार्किक utils::getConfigurationParameterBool(स्थिरांक अक्षर* name, तार्किक default_मूल्य_2)
{
    परत पठ<तार्किक>(name, default_मूल्य_2);
}

माप_प्रकार utils::getConfigurationParameterSizeT(स्थिरांक अक्षर* name, माप_प्रकार default_मूल्य_2)
{
    परत पठ<माप_प्रकार>(name, default_मूल्य_2);
}

std::string utils::getConfigurationParameterString(स्थिरांक अक्षर* name, स्थिरांक std::string & default_मूल्य_2)
{
    परत पठ<cv::String>(name, default_मूल्य_2);
}

utils::Paths utils::getConfigurationParameterPaths(स्थिरांक अक्षर* name, स्थिरांक utils::Paths &default_मूल्य_2)
{
    परत पठ<utils::Paths>(name, default_मूल्य_2);
}


#ifdef CV_COLLECT_IMPL_DATA
ImplCollector& getImplData()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, नवीन ImplCollector())
}

शून्य setImpl(पूर्णांक flags)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags = flags;
    getImplData().implCode.clear();
    getImplData().implFun.clear();
}

शून्य addImpl(पूर्णांक flag, स्थिरांक अक्षर* func)
{
    cv::AutoLock lock(getImplData().mutex);

    getImplData().implFlags |= flag;
    जर(func) // use lazy collection if name was not specified
    {
        माप_प्रकार index = getImplData().implCode.size();
        जर(!index || (getImplData().implCode[index-1] != flag || getImplData().implFun[index-1].compare(func))) // avoid duplicates
        {
            getImplData().implCode.push_back(flag);
            getImplData().implFun.push_back(func);
        }
    }
}

पूर्णांक getImpl(std::vector<पूर्णांक> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(getImplData().mutex);

    impl    = getImplData().implCode;
    funName = getImplData().implFun;
    परत getImplData().implFlags; // return actual flags for lazy collection
}

तार्किक useCollection()
{
    परत getImplData().useCollection;
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
    परत getInstrumentStruct().useInstr;
#अन्यथा
    परत असत्य;
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

Instr_गाठ* getTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    परत &getInstrumentStruct().मूळ_गाठ_2;
#अन्यथा
    परत रिक्त;
#endif
}

शून्य resetTrace()
{
#ifdef ENABLE_INSTRUMENTATION
    getInstrumentStruct().मूळ_गाठ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_गाठ = &getInstrumentStruct().मूळ_गाठ_2;
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
    परत (FLAGS)getInstrumentStruct().flags;
#अन्यथा
    परत (FLAGS)0;
#endif
}

गाठ_Data::गाठ_Data(स्थिरांक अक्षर* funName, स्थिरांक अक्षर* fileName, पूर्णांक ओळ_अंक, शून्य* retAddress, तार्किक alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_ओळ_अंक       = ओळ_अंक;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_counter    = 0;
    m_ticksTotal = 0;

    m_funError  = असत्य;
}
गाठ_Data::गाठ_Data(गाठ_Data &ref)
{
    *हे = ref;
}
गाठ_Data& गाठ_Data::चालक=(स्थिरांक गाठ_Data &उजवा)
{
    हे->m_funName      = उजवा.m_funName;
    हे->m_instrType    = उजवा.m_instrType;
    हे->m_implType     = उजवा.m_implType;
    हे->m_fileName     = उजवा.m_fileName;
    हे->m_ओळ_अंक      = उजवा.m_ओळ_अंक;
    हे->m_retAddress   = उजवा.m_retAddress;
    हे->m_alwaysExpand = उजवा.m_alwaysExpand;

    हे->m_threads     = उजवा.m_threads;
    हे->m_counter     = उजवा.m_counter;
    हे->m_ticksTotal  = उजवा.m_ticksTotal;

    हे->m_funError    = उजवा.m_funError;

    परत *हे;
}
गाठ_Data::~गाठ_Data()
{
}
तार्किक चालक==(स्थिरांक गाठ_Data& डावा, स्थिरांक गाठ_Data& उजवा)
{
    जर(डावा.m_ओळ_अंक == उजवा.m_ओळ_अंक && डावा.m_funName == उजवा.m_funName && डावा.m_fileName == उजवा.m_fileName)
    {
        जर(डावा.m_retAddress == उजवा.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || डावा.m_alwaysExpand))
            परत सत्य;
    }
    परत असत्य;
}

#ifdef ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    स्थिर InstrStruct instr;
    परत instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    परत *getInstrumentStruct().tlsStruct.get();
}

Instr_गाठ* get_Current_गाठ()
{
    परत getInstrumentTLSStruct().p_Current_गाठ;
}

IntrumentationRegion::IntrumentationRegion(स्थिरांक अक्षर* funName, स्थिरांक अक्षर* fileName, पूर्णांक ओळ_अंक, शून्य *retAddress, तार्किक alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = असत्य;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    जर(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        जर(!pTLS->p_Current_गाठ)
        {
            m_disabled = सत्य;
            परत;
        }

        पूर्णांक depth = pTLS->p_Current_गाठ->getDepth();
        जर(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = सत्य;
            परत;
        }

        गाठ_Data payload(funName, fileName, ओळ_अंक, retAddress, alwaysExpand, instrType, implType);
        Node<गाठ_Data>* pChild = रिक्त;

        जर(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_गाठ->findChild(payload);
            जर(!pChild)
            {
                pChild = नवीन Node<गाठ_Data>(payload);
                pTLS->p_Current_गाठ->addChild(pChild);
            }
        }
        अन्यथा
        {
            pChild = pTLS->p_Current_गाठ->findChild(payload);
            जर(!pChild)
            {
                m_disabled = सत्य;
                परत;
            }
        }
        pTLS->p_Current_गाठ = pChild;

        m_regionTicks = get_Tick_मोजणी();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    जर(pStruct->useInstr)
    {
        जर(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            जर (pTLS->p_Current_गाठ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_गाठ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_गाठ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_मोजणी() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_मोजणी); // Concurrent ticks accumulation
                pTLS->p_Current_गाठ->m_payload.m_counter++;
                pTLS->p_Current_गाठ->m_payload.m_ticksTotal += ticks;
                pTLS->p_Current_गाठ->m_payload.m_tls.get()->m_ticksTotal += ticks;
            }

            pTLS->p_Current_गाठ = pTLS->p_Current_गाठ->m_pParent;
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

#जर defined(OPENCV_ALGO_HINT_DEFAULT)
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
        जर(ippStatus < 0)
        {
            CV_LOG_ERROR(रिक्त, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = असत्य;
            परत;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        जर(!env.empty())
        {
#जर IPP_VERSION_X100 >= 201900
            स्थिरांक Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#किंवाजर IPP_VERSION_X100 >= 201703
            स्थिरांक Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#किंवाजर IPP_VERSION_X100 >= 201700
            स्थिरांक Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अन्यथा
            स्थिरांक Ipp64u minorFeatures = 0;
#endif

            env = toLowerCase(env);
            जर(env.substr(0, 2) == "ne")
            {
                useIPP_NE = सत्य;
                env = env.substr(3, env.size());
            }

            जर(env == "disabled")
            {
                CV_LOG_WARNING(रिक्त, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = असत्य;
            }
            अन्यथा जर(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            अन्यथा जर(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#जर IPP_VERSION_X100 >= 201700
#जर defined (_M_AMD64) || defined (__x86_64__)
            अन्यथा जर(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#endif
#endif
            अन्यथा
                CV_LOG_ERROR(रिक्त, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        जर(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        जर(!(
#जर IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#endif
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = असत्य;
            परत;
        }

        जर(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        अन्यथा
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#जर IPP_VERSION_X100 >= 201700
        जर(ippFeatures&ippCPUID_AVX512F)
        {
            जर((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_शीर्ष_Features = ippCPUID_AVX512_SKX;
            अन्यथा जर((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_शीर्ष_Features = ippCPUID_AVX512_KNL;
            अन्यथा
                ipp_शीर्ष_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        अन्यथा
#endif
        जर(ippFeatures&ippCPUID_AVX2)
            ipp_शीर्ष_Features = ippCPUID_AVX2;
        अन्यथा जर(ippFeatures&ippCPUID_SSE42)
            ipp_शीर्ष_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        जर (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_शीर्ष_Features = ippCPUID_SSE42;
        }
    }

सार्वजनिक:
    तार्किक        useIPP;
    तार्किक        useIPP_NE;

    पूर्णांक         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    स्थिरांक अक्षर *funcname;
    स्थिरांक अक्षर *filename;
    पूर्णांक         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_शीर्ष_Features;
    स्थिरांक IppLibraryVersion *pIppLibInfo;
};

स्थिर IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, नवीन IPPInitSingleton())
}
#endif

अचिन्हित दीर्घ दीर्घ getIppFeatures()
{
#ifdef HAVE_IPP
    परत getIPPSingleton().ippFeatures;
#अन्यथा
    परत 0;
#endif
}

#ifdef HAVE_IPP
अचिन्हित दीर्घ दीर्घ get_Ipp_शीर्ष_Features()
{
    परत getIPPSingleton().ipp_शीर्ष_Features;
}
#endif

शून्य setIppStatus(पूर्णांक status, स्थिरांक अक्षर * स्थिरांक _funcname, स्थिरांक अक्षर * स्थिरांक _filename, पूर्णांक ओळ_3)
{
#ifdef HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = ओळ_3;
#अन्यथा
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(ओळ_3);
#endif
}

पूर्णांक getIppStatus()
{
#ifdef HAVE_IPP
    परत getIPPSingleton().ippStatus;
#अन्यथा
    परत 0;
#endif
}

String getIppErrorLocation()
{
#ifdef HAVE_IPP
    परत format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#अन्यथा
    परत String();
#endif
}

String getIppVersion()
{
#ifdef HAVE_IPP
    स्थिरांक IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    जर(pInfo)
        परत format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    अन्यथा
        परत String("error");
#अन्यथा
    परत String("disabled");
#endif
}

तार्किक useIPP()
{
#ifdef HAVE_IPP
    CoreTLSData& data = getCoreTlsData();
    जर (data.useIPP < 0)
    {
        data.useIPP = getIPPSingleton().useIPP;
    }
    परत (data.useIPP > 0);
#अन्यथा
    परत असत्य;
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
    जर (data.useIPP_NE < 0)
    {
        data.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    परत (data.useIPP_NE > 0);
#अन्यथा
    परत असत्य;
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

#जर OPENCV_IMPL_FP_HINTS_X86
#ifndef _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_ON 0x0040
#endif
#ifndef _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#define _MM_DENORMALS_ZERO_MASK 0x0040
#endif
#endif

शून्य setFPDenormalsIgnoreHint(तार्किक ignore, CV_OUT FPDenormalsModeState& state)
{
#जर OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    अचिन्हित value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    जर (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    स्थिरांक अचिन्हित old_flags = _mm_getcsr();
    स्थिरांक अचिन्हित old_मूल्य = old_flags & mask;
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मूल्य;
    _mm_setcsr(flags);
#अन्यथा
    CV_UNUSED(ignore); CV_UNUSED(state);
#endif
}

पूर्णांक saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#जर OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    जर (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    स्थिरांक अचिन्हित old_flags = _mm_getcsr();
    स्थिरांक अचिन्हित old_मूल्य = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मूल्य;
    परत 2;
#अन्यथा
    CV_UNUSED(state);
    परत 0;
#endif
}

तार्किक restoreFPDenormalsState(स्थिरांक FPDenormalsModeState& state)
{
#जर OPENCV_IMPL_FP_HINTS_X86
    स्थिरांक अचिन्हित mask = (अचिन्हित)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    स्थिरांक अचिन्हित value = (अचिन्हित)state.reserved[1];
    CV_DbgCheck((पूर्णांक)value, value == (value & mask), "invalid SSE FP state");
    स्थिरांक अचिन्हित old_flags = _mm_getcsr();
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(रिक्त, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    परत सत्य;
#अन्यथा
    CV_UNUSED(state);
    परत असत्य;
#endif
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ifdef OPENCV_ALGO_HINT_DEFAULT
    परत OPENCV_ALGO_HINT_DEFAULT;
#अन्यथा
    परत ALGO_HINT_ACCURATE;
#endif
};

} // namespace cv

/* End of file. */
