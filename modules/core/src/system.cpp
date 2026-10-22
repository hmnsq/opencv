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

#समावेश "precomp.hpp"
//#include "opencv2/core/core_c.h"
#समावेश <atomic>
#समावेश <exception>
#समावेश <iostream>
#समावेश <ostream>

#अगर_घोषित __QNX__
    #समावेश <unistd.h>
    #समावेश <sys/neutrino.h>
    #समावेश <sys/syspage.h>
#अगर_घोषित __aarch64__
    #समावेश <aarch64/syspage.h>
#पूर्ण_अगर
#पूर्ण_अगर

#समावेश <opencv2/core/utils/configuration.private.hpp>
#समावेश <opencv2/core/utils/trace.private.hpp>

#समावेश <opencv2/core/utils/logger.hpp>

#समावेश <opencv2/core/utils/tls.hpp>
#समावेश <opencv2/core/utils/instrumentation.hpp>

#समावेश <opencv2/core/utils/filesystem.private.hpp>

#समावेश <opencv2/core/utils/fp_control_utils.hpp>
#समावेश <opencv2/core/utils/fp_control.private.hpp>

namespace cv {

अटल व्योम _initSystem()
{
#अगर_घोषित __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    अटल std::ios_base::Init s_iostream_initializer;
#पूर्ण_अगर
}

अटल Mutex* __initialization_mutex = शून्य;
Mutex& getInitializationMutex()
{
    अगर (__initialization_mutex == शून्य)
    {
        (व्योम)_initSystem();
        __initialization_mutex = नया Mutex();
    }
    वापस *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

अटल bool param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#अगर defined(_DEBUG) || defined(__ANDROID__)
    true
#अन्यथा
    false
#पूर्ण_अगर
);

व्योम* alloc_Singleton_बफर(माप_प्रकार size) { वापस fastMalloc(size); }
व्योम* alloc_Singleton_New_बफर(माप_प्रकार size) { वापस दो_स्मृति(size); }


} // namespace cv

#अगर_अघोषित CV_ERROR_SET_TERMINATE_HANDLER  // build config option
# अगर defined(_WIN32)
#   define CV_ERROR_SET_TERMINATE_HANDLER 1
# endif
#पूर्ण_अगर
#अगर defined(CV_ERROR_SET_TERMINATE_HANDLER) && !CV_ERROR_SET_TERMINATE_HANDLER
# undef CV_ERROR_SET_TERMINATE_HANDLER
#पूर्ण_अगर

#अगर_घोषित _MSC_VER
# अगर _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#पूर्ण_अगर

#अगर_घोषित CV_ERROR_SET_TERMINATE_HANDLER
#समावेश <cstdlib>        // std::abort
#पूर्ण_अगर

#अगर defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#अगर defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
using Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
स्थिर uint64_t AT_HWCAP = NT_GNU_HWCAP;
#अन्यथा
#  include <elf.h>
#पूर्ण_अगर
#अगर defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#पूर्ण_अगर


#अगर ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#पूर्ण_अगर

#अगर defined __loongarch64
#समावेश "sys/auxv.h"
#घोषणा LA_HWCAP_LSX   (1<<4)
#घोषणा LA_HWCAP_LASX  (1<<5)
#पूर्ण_अगर

#अगर defined _WIN32 || defined WINCE
#अगर_अघोषित _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #घोषणा _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#पूर्ण_अगर
#समावेश <windows.h>
#समावेश <combaseapi.h>
#अगर (_WIN32_WINNT >= 0x0602)
  #समावेश <synchapi.h>
#पूर्ण_अगर
#अगर ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #समावेश <fibersapi.h>
  #घोषणा CV_USE_FLS
#पूर्ण_अगर
#अघोषित small
#अघोषित min
#अघोषित max
#अघोषित असल
#समावेश <tchar.h>

#अगर_घोषित WINRT
#समावेश <wrl/client.h>
#अगर_अघोषित __cplusplus_winrt
#समावेश <windows.storage.h>
#आशय comment(lib, "runtimeobject.lib")
#पूर्ण_अगर // WINRT

std::wstring Get_अस्थायी_Path_Win_RT()
{
#अगर_घोषित __cplusplus_winrt
    वापस std::wstring(Windows::Storage::Application_तथ्य::Current->TemporaryFolder->Path->तथ्य_4());
#अन्यथा
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_तथ्य_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_तथ्य> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageItem> storageitemRef;
    HSTRING str;
    HSTRING_HEADER hstr_शीर्ष;
    std::wstring wstr;
    अगर (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_तथ्य,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_तथ्य), &hstr_शीर्ष, &str)))
        वापस wstr;
    अगर (FAILED(RoGetActivationFactory(str, IID_PPV_ARGS(appdataFactory.ReleaseAndGetAddressOf()))))
        वापस wstr;
    अगर (FAILED(appdataFactory->get_Current(appdataRef.ReleaseAndGetAddressOf())))
        वापस wstr;
    अगर (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.ReleaseAndGetAddressOf())))
        वापस wstr;
    अगर (FAILED(storagefolderRef.As(&storageitemRef)))
        वापस wstr;
    str = शून्य;
    अगर (FAILED(storageitemRef->get_Path(&str)))
        वापस wstr;
    wstr = Windows_Get_String_Raw_बफर(str, शून्य);
    WindowsDeleteString(str);
    वापस wstr;
#पूर्ण_अगर
}

std::wstring Get_अस्थायी_File_Name_Win_RT(std::wstring prefix)
{
    ब_अक्षर_प्रकार guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    ब_अक्षर_प्रकार* mask = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], माप(guidStr)/माप(ब_अक्षर_प्रकार), mask,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    वापस prefix.append(std::wstring(guidStr));
}

#पूर्ण_अगर
#अन्यथा
#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT
#समावेश <pthread.h>
#पूर्ण_अगर
#समावेश <sys/time.h>
#समावेश <time.h>

#अगर defined __MACH__ && defined __APPLE__
#समावेश <mach/mach.h>
#समावेश <mach/mach_time.h>
#समावेश <sys/sysctl.h>
#पूर्ण_अगर

#पूर्ण_अगर

#अगर_घोषित _OPENMP
#समावेश "omp.h"
#पूर्ण_अगर

#अगर defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#समावेश <unistd.h>
#समावेश <stdio.h>
#समावेश <sys/types.h>
#अगर defined __ANDROID__
#समावेश <sys/sysconf.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित __ANDROID__
# include <android/log.h>
#पूर्ण_अगर

#अगर_घोषित DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#पूर्ण_अगर
#अगर_अघोषित CV_CPUID_X86
  #अगर defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #अगर _MSC_VER >= 1400  // MSVS 2005
      #समावेश <intrin.h>  // __cpuidex()
      #घोषणा CV_CPUID_X86 __cpuidex
    #अन्यथा
      #त्रुटि "Required MSVS 2005+"
    #पूर्ण_अगर
  #या_अगर defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    अटल व्योम cv_cpuid(पूर्णांक* cpuid_तथ्य, पूर्णांक reg_eax, पूर्णांक reg_ecx)
    {
        पूर्णांक __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#अगर !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#या_अगर defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#अन्यथा
#त्रुटि "Configuration error"
#पूर्ण_अगर
        cpuid_तथ्य[0] = __eax; cpuid_तथ्य[1] = __ebx; cpuid_तथ्य[2] = __ecx; cpuid_तथ्य[3] = __edx;
    }
    #घोषणा CV_CPUID_X86 cv_cpuid
  #पूर्ण_अगर
#पूर्ण_अगर

#समावेश <chrono>

namespace cv
{

Exception::Exception() { code = Error::StsOk; पंक्ति = 0; }

Exception::Exception(Error::Code _code, स्थिर String& _err, स्थिर String& _func, स्थिर String& _file, पूर्णांक पंक्ति_3)
: code(_code), err(_err), func(_func), file(_file), पंक्ति(पंक्ति_3)
{
    formatMessage();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
स्थिर अक्षर* Exception::what() स्थिर CV_NOEXCEPT { वापस संदेश.c_str(); }

व्योम Exception::formatMessage()
{
    माप_प्रकार pos = err.find('\n');
    bool multiline = pos != cv::String::npos;
    अगर (multiline)
    {
        std::stringstream ss;
        माप_प्रकार पूर्व_pos = 0;
        जबतक (pos != cv::String::npos)
        {
           ss << "> " << err.substr(पूर्व_pos, pos - पूर्व_pos) << std::प_पूर्ण;
           पूर्व_pos = pos + 1;
           pos = err.find('\n', पूर्व_pos);
        }
        ss << "> " << err.substr(पूर्व_pos);
        अगर (err[err.size() - 1] != '\n')
            ss << std::प_पूर्ण;
        err = ss.str();
    }
    अगर (func.size() > 0)
    {
        अगर (multiline)
            संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_VERSION, file.c_str(), पंक्ति, code, codeMessage(), func.c_str(), err.c_str());
        अन्यथा
            संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_VERSION, file.c_str(), पंक्ति, code, codeMessage(), err.c_str(), func.c_str());
    }
    अन्यथा
    {
        संदेश = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_VERSION, file.c_str(), पंक्ति, code, codeMessage(), err.c_str(), multiline ? "" : "\n");
    }
}

स्थिर अक्षर * Exception::codeMessage() स्थिर
{
    चयन (code)
    {
    हाल cv::Error::StsOk :                  वापस "No Error";
    हाल cv::Error::StsBackTrace :           वापस "Backtrace";
    हाल cv::Error::StsError :               वापस "Unspecified error";
    हाल cv::Error::StsInternal :            वापस "Internal error";
    हाल cv::Error::StsNoMem :               वापस "Insufficient memory";
    हाल cv::Error::StsBadArg :              वापस "Bad argument";
    हाल cv::Error::StsNoConv :              वापस "Iterations do not converge";
    हाल cv::Error::StsAutoTrace :           वापस "Autotrace call";
    हाल cv::Error::Sts_Bad_आकार :             वापस "Incorrect size of input array";
    हाल cv::Error::StsNullPtr :             वापस "Null pointer";
    हाल cv::Error::StsDivByZero :           वापस "Division by zero occurred";
    हाल cv::Error::BadStep :                वापस "Image step is wrong";
    हाल cv::Error::StsInplaceNotSupported : वापस "Inplace operation is not supported";
    हाल cv::Error::StsObjectNotFound :      वापस "Requested object was not found";
    हाल cv::Error::BadDepth :               वापस "Input image depth is not supported by function";
    हाल cv::Error::StsUnmatchedFormats :    वापस "Formats of input arguments do not match";
    हाल cv::Error::StsUnmatchedSizes :      वापस "Sizes of input arguments do not match";
    हाल cv::Error::StsOutOfRange :          वापस "One of the arguments\' values is out of range";
    हाल cv::Error::StsUnsupportedFormat :   वापस "Unsupported format or combination of formats";
    हाल cv::Error::BadCOI :                 वापस "Input COI is not supported";
    हाल cv::Error::Bad_अंक_Channels :         वापस "Bad number of channels";
    हाल cv::Error::StsBadFlag :             वापस "Bad flag (parameter or structure field)";
    हाल cv::Error::StsBadPoint :            वापस "Bad parameter of type CvPoint";
    हाल cv::Error::StsBadMask :             वापस "Bad type of mask argument";
    हाल cv::Error::StsParseError :          वापस "Parsing error";
    हाल cv::Error::StsNotImplemented :      वापस "The function/feature is not implemented";
    हाल cv::Error::StsBadMemBlock :         वापस "Memory block has been corrupted";
    हाल cv::Error::StsAssert :              वापस "Assertion failed";
    हाल cv::Error::GpuNotSupported :        वापस "No CUDA support";
    हाल cv::Error::GpuApiCallError :        वापस "Gpu API call";
    हाल cv::Error::OpenGlNotSupported :     वापस "No OpenGL support";
    हाल cv::Error::OpenGlApiCallError :     वापस "OpenGL API call";
    हाल cv::Error::OpenCLApiCallError :     वापस "OpenCL API call error";
    हाल cv::Error::OpenCLDoubleNotSupported:वापस "OpenCL device does not support double";
    हाल cv::Error::OpenCLInitError :        वापस "OpenCL initialization error";
    हाल cv::Error::OpenCLNoAMDBlasFft :     वापस "Missing OpenCL AMD BLAS FFT";
    शेष:
    {
        अटल अक्षर buf[256] {0};
        snprintf(buf, माप(buf), "Unknown %s code %d", code >= 0 ? "status":"error", code);
        वापस buf;
    }
    };

}

अटल स्थिर अक्षर* g_hwFeatureNames[CV_HARDWARE_MAX_FEATURE] = { शून्य };

अटल स्थिर अक्षर* getHWFeatureName(पूर्णांक id)
{
    वापस (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hwFeatureNames[id] : शून्य;
}
अटल स्थिर अक्षर* getHWFeatureNameSafe(पूर्णांक id)
{
    स्थिर अक्षर* name = getHWFeatureName(id);
    वापस name ? name : "Unknown feature";
}

काष्ठा HWFeatures
{
    क्रमागत { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(bool run_initialize = false)
    {
        अगर (run_initialize)
            initialize();
    }

    अटल व्योम initializeNames()
    {
        क्रम (पूर्णांक i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
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

    व्योम initialize(व्योम)
    {
        अगर (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            ख_लिखो(मानक_त्रुटि, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initializeNames();

    #अगर_घोषित CV_CPUID_X86
        पूर्णांक cpuid_तथ्य[4] = { 0, 0, 0, 0 };
        पूर्णांक cpuid_तथ्य_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_तथ्य, 1, 0/*unused*/);

        पूर्णांक x86_family = (cpuid_तथ्य[0] >> 8) & 15;
        अगर( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_तथ्य[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_तथ्य[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_तथ्य[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_तथ्य[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_तथ्य[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_तथ्य[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_तथ्य[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_तथ्य[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_तथ्य[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_तथ्य[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_तथ्य[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_तथ्य_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_तथ्य_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_तथ्य_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_तथ्य_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_तथ्य_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_तथ्य_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_तथ्य_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_तथ्य_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_तथ्य_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_तथ्य_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_तथ्य_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_तथ्य_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_तथ्य_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_तथ्य_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_तथ्य_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_तथ्य_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_तथ्य_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            पूर्णांक cpuid_तथ्य_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_तथ्य_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_तथ्य_ex1[0] & (1<<4))  != 0;

            bool have_AVX_OS_support = true;
            bool have_AVX512_OS_support = true;
            अगर (!(cpuid_तथ्य[2] & (1<<27)))
                have_AVX_OS_support = false; // OS uses XSAVE_XRSTORE and CPU support AVX
            अन्यथा
            {
                पूर्णांक xcr0 = 0;
            #अगर_घोषित _XCR_XFEATURE_ENABLED_MASK // requires immintrin.h
                xcr0 = (पूर्णांक)_xgetbv(_XCR_XFEATURE_ENABLED_MASK);
            #या_अगर defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #पूर्ण_अगर
                अगर ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = false; // YMM registers
                अगर ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = false; // ZMM registers
            }

            अगर (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = false;
                have[CV_CPU_FP16] = false;
                have[CV_CPU_AVX2] = false;
                have[CV_CPU_FMA3] = false;
                have[CV_CPU_AVX_VNNI] = false;
            }
            अगर (!have_AVX_OS_support || !have_AVX512_OS_support)
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
            अगर (have[CV_CPU_AVX512_COMMON])
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
                have[CV_CPU_AVX512_KNL] = false;
                have[CV_CPU_AVX512_KNM] = false;
                have[CV_CPU_AVX512_SKX] = false;
                have[CV_CPU_AVX512_CNL] = false;
                have[CV_CPU_AVX512_CLX] = false;
                have[CV_CPU_AVX512_ICL] = false;
            }
        }
    #पूर्ण_अगर // CV_CPUID_X86

    #अगर defined __ANDROID__ || defined __linux__ || defined __QNX__
    #अगर_घोषित __aarch64__
        have[CV_CPU_NEON] = true;
        have[CV_CPU_FP16] = true;
        पूर्णांक cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        अगर (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            स्थिर माप_प्रकार आकार_auxv_t = माप(auxv);

            जबतक ((माप_प्रकार)पढ़ो(cpufile, &auxv, आकार_auxv_t) == आकार_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                अगर (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#अगर defined(AT_HWCAP2)
                अन्यथा अगर (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#पूर्ण_अगर
            }

            बंद(cpufile);
        }
    #या_अगर defined __arm__ && defined __ANDROID__
      #अगर defined HAVE_CPUFEATURES
        CV_LOG_INFO(शून्य, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(शून्य, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (दीर्घ दीर्घ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #अन्यथा
        CV_LOG_INFO(शून्य, "cpufeatures library is not available for CPU detection");
        #अगर CV_NEON
        CV_LOG_INFO(शून्य, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = true;
        #अन्यथा
        CV_LOG_INFO(शून्य, "- NEON instructions is NOT enabled via build flags");
        #पूर्ण_अगर
        #अगर CV_FP16
        CV_LOG_INFO(शून्य, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = true;
        #अन्यथा
        CV_LOG_INFO(शून्य, "- FP16 instructions is NOT enabled via build flags");
        #पूर्ण_अगर
      #पूर्ण_अगर
    #या_अगर defined __arm__
        पूर्णांक cpufile = खोलो("/proc/self/auxv", O_RDONLY);

        अगर (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            स्थिर माप_प्रकार आकार_auxv_t = माप(auxv);

            जबतक ((माप_प्रकार)पढ़ो(cpufile, &auxv, आकार_auxv_t) == आकार_auxv_t)
            {
                अगर (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    अवरोध;
                }
            }

            बंद(cpufile);
        }
    #पूर्ण_अगर
    #या_अगर (defined __APPLE__)
    #अगर defined __ARM_NEON
        have[CV_CPU_NEON] = true;
    #पूर्ण_अगर
    #अगर (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = true;
    #पूर्ण_अगर
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    पूर्णांक has_feat_dotprod = 0;
    माप_प्रकार has_feat_dotprod_आकार = माप(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_आकार, शून्य, 0);
    अगर (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = true;
    }
    पूर्णांक has_feat_bf16 = 0;
    माप_प्रकार has_feat_bf16_आकार = माप(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_आकार, शून्य, 0);
    अगर (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = true;
    }
    #या_अगर (defined __clang__)
    #अगर defined __ARM_NEON
        have[CV_CPU_NEON] = true;
        #अगर (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = true;
        #पूर्ण_अगर
    #पूर्ण_अगर
    #पूर्ण_अगर
    #अगर defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = true;
    #पूर्ण_अगर
    #अगर defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = true;
    #पूर्ण_अगर
    #अगर_घोषित __riscv_vector
        have[CV_CPU_RISCVV] = true;
    #पूर्ण_अगर
    #अगर_घोषित __mips_msa
        have[CV_CPU_MSA] = true;
    #पूर्ण_अगर

    #अगर (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        अचिन्हित पूर्णांक hwcap = getauxval(AT_HWCAP);
        अगर (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            अगर (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #या_अगर (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        अचिन्हित दीर्घ hwcap = 0, hwcap2 = 0;
        अगर (elf_aux_info(AT_HWCAP, &hwcap, माप(hwcap)) != 0)
            hwcap = 0;
        अगर (hwcap & PPC_FEATURE_HAS_VSX) {
            अगर (elf_aux_info(AT_HWCAP2, &hwcap2, माप(hwcap2)) != 0)
                hwcap2 = 0;
            अगर (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = true;
            } अन्यथा {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #अन्यथा
        // TODO: AIX
        #अगर CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = true;
        #पूर्ण_अगर
        #अगर CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = true;
        #पूर्ण_अगर
    #पूर्ण_अगर

    #अगर defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = true;
        #अगर (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = true;
        #पूर्ण_अगर
        #अगर defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = true;
        #पूर्ण_अगर
    #पूर्ण_अगर

    #अगर defined __loongarch64 && defined __linux__
        पूर्णांक flag = (पूर्णांक)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #पूर्ण_अगर

        bool skip_baseline_check = false;
        अगर (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = true;
        }
        पूर्णांक baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        अगर (!checkFeatures(baseline_features, माप(baseline_features) / माप(baseline_features[0]))
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
            checkFeatures(baseline_features, माप(baseline_features) / माप(baseline_features[0]), true);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, माप(baseline_features) / माप(baseline_features[0]));
    }

    bool checkFeatures(स्थिर पूर्णांक* features, पूर्णांक गिनती_2, bool dump = false)
    {
        bool परिणाम = true;
        क्रम (पूर्णांक i = 0; i < गिनती_2; i++)
        {
            पूर्णांक feature = features[i];
            अगर (feature)
            {
                अगर (have[feature])
                {
                    अगर (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - OK\n", feature, getHWFeatureNameSafe(feature));
                }
                अन्यथा
                {
                    परिणाम = false;
                    अगर (dump) ख_लिखो(मानक_त्रुटि, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, getHWFeatureNameSafe(feature));
                }
            }
        }
        वापस परिणाम;
    }

    अटल अंतरभूत bool is_प्रतीक_Separator(अक्षर c)
    {
        वापस c == ',' || c == ';';
    }

    व्योम readSettings(स्थिर पूर्णांक* baseline_features, पूर्णांक baseline_गिनती)
    {
        bool dump = true;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        अगर (!disabled_features.empty())
        {
            स्थिर अक्षर* start = disabled_features.c_str();
            क्रम (;;)
            {
                जबतक (start[0] != 0 && is_प्रतीक_Separator(start[0]))
                {
                    start++;
                }
                अगर (start[0] == 0)
                    अवरोध;
                स्थिर अक्षर* end = start;
                जबतक (end[0] != 0 && !is_प्रतीक_Separator(end[0]))
                {
                    end++;
                }
                अगर (end == start)
                    जारी;
                cv::String feature(start, end);
                start = end;

                CV_Assert(feature.size() > 0);

                bool found = false;
                क्रम (पूर्णांक i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    अगर (!g_hwFeatureNames[i]) जारी;
                    माप_प्रकार len = म_माप(g_hwFeatureNames[i]);
                    अगर (len != feature.size()) जारी;
                    अगर (feature.compare(g_hwFeatureNames[i]) == 0)
                    {
                        bool isBaseline = false;
                        क्रम (पूर्णांक k = 0; k < baseline_गिनती; k++)
                        {
                            अगर (baseline_features[k] == i)
                            {
                                isBaseline = true;
                                अवरोध;
                            }
                        }
                        अगर (isBaseline)
                        {
                            अगर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", getHWFeatureNameSafe(i));
                        }
                        अगर (!have[i])
                        {
                            अगर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                getHWFeatureNameSafe(i));
                        }
                        have[i] = false;

                        found = true;
                        अवरोध;
                    }
                }
                अगर (!found)
                {
                    अगर (dump) ख_लिखो(मानक_त्रुटि, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    bool have[MAX_FEATURE+1]{};
};

अटल HWFeatures  featuresEnabled(true), featuresDisabled = HWFeatures(false);
अटल HWFeatures* currentFeatures = &featuresEnabled;

bool checkHardwareSupport(पूर्णांक feature)
{
    अगर (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        वापस false;
    वापस currentFeatures->have[feature];
}

String getHardwareFeatureName(पूर्णांक feature)
{
    स्थिर अक्षर* name = getHWFeatureName(feature);
    वापस name ? String(name) : String();
}

std::string get_CPUFeatures_पंक्ति()
{
    स्थिर पूर्णांक features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    स्थिर पूर्णांक sz = माप(features) / माप(features[0]);
    std::string परिणाम;
    std::string prefix;
    क्रम (पूर्णांक i = 1; i < sz; ++i)
    {
        अगर (features[i] == 0)
        {
            prefix = "*";
            जारी;
        }
        अगर (i != 1) परिणाम.append(" ");
        परिणाम.append(prefix);
        परिणाम.append(getHWFeatureNameSafe(features[i]));
        अगर (!checkHardwareSupport(features[i])) परिणाम.append("?");
    }
    वापस परिणाम;
}

अस्थिर bool useOptimizedFlag = true;

व्योम setUseOptimized( bool flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::setUseIPP(flag);
#अगर_घोषित HAVE_OPENCL
    ocl::setUseOpenCL(flag);
#पूर्ण_अगर
}

bool useOptimized(व्योम)
{
    वापस useOptimizedFlag;
}

int64 get_Tick_गिनती(व्योम)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    वापस (int64)now.time_since_epoch().गिनती_2();
}

द्विगुन getTickFrequency(व्योम)
{
    using clock_period_t = std::chrono::steady_clock::duration::period;
    द्विगुन clock_freq = clock_period_t::den / clock_period_t::num;
    वापस clock_freq;
}

#अगर defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#अगर defined(__i386__)

int64 get_CPUTick_गिनती(व्योम)
{
    int64 x;
    __asm__ अस्थिर (".byte 0x0f, 0x31" : "=A" (x));
    वापस x;
}
#या_अगर defined(__x86_64__)

int64 get_CPUTick_गिनती(व्योम)
{
    अचिन्हित hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    वापस (int64)lo | ((int64)hi << 32);
}

#या_अगर defined(__ppc__)

int64 get_CPUTick_गिनती(व्योम)
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
    वापस lower | ((int64)upper << 32);
}

#अन्यथा

#त्रुटि "RDTSC not defined"

#पूर्ण_अगर

#या_अगर defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_गिनती(व्योम)
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
int64 get_CPUTick_गिनती(व्योम)
{
    वापस get_Tick_गिनती();
}
//#endif

#पूर्ण_अगर


namespace internal {

श्रेणी Timestamp
{
खुला:
    स्थिर int64 zero_Tick_गिनती;
    स्थिर द्विगुन ns_in_ticks;

    Timestamp()
        : zero_Tick_गिनती(get_Tick_गिनती())
        , ns_in_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_गिनती();
        वापस (int64)((t - zero_Tick_गिनती) * ns_in_ticks);
    }

    अटल Timestamp& getInstance()
    {
        अटल Timestamp g_timestamp;
        वापस g_timestamp;
    }
};

श्रेणी InitTimestamp {
खुला:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
अटल InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    वापस internal::Timestamp::getInstance().getTimestamp();
}


स्थिर String& getBuildInformation()
{
    अटल String build_info =
#समावेश "version_string.inc"
    ;
    वापस build_info;
}

String getVersionString() { वापस String(CV_VERSION); }

पूर्णांक getVersionMajor() { वापस CV_VERSION_MAJOR; }

पूर्णांक getVersionMinor() { वापस CV_VERSION_MINOR; }

पूर्णांक getVersionRevision() { वापस CV_VERSION_REVISION; }

String format( स्थिर अक्षर* fmt, ... )
{
    Auto_बफर<अक्षर, 1024> buf;

    क्रम ( ; ; )
    {
        बहु_सूची va;
        बहु_शुरू(va, fmt);
        पूर्णांक bsize = static_cast<पूर्णांक>(buf.size());
        पूर्णांक len = cv_vsnprintf(buf.तथ्य(), bsize, fmt, va);
        बहु_पूर्ण(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        अगर (len >= bsize)
        {
            buf.resize(len + 1);
            जारी;
        }
        buf[bsize - 1] = 0;
        वापस String(buf.तथ्य(), len);
    }
}

String tempfile( स्थिर अक्षर* suffix )
{
#अगर OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string अस्थायी_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#अगर defined _WIN32
#अगर_घोषित WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring अस्थायी_dir_rt = Get_अस्थायी_Path_Win_RT();

    std::wstring अस्थायी_file = Get_अस्थायी_File_Name_Win_RT(L"ocv");
    अगर (अस्थायी_file.empty())
        वापस String();

    अस्थायी_file = अस्थायी_dir_rt.append(std::wstring(L"\\")).append(अस्थायी_file);
    DeleteFileW(अस्थायी_file.c_str());

    अक्षर aname[MAX_PATH];
    माप_प्रकार copied = wcstombs(aname, अस्थायी_file.c_str(), MAX_PATH);
    CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
    fname = String(aname);
    RoUninitialize();
#या_अगर defined(_WIN32_WCE)
    स्थिर स्वतः k_Max_Path_आकार = MAX_PATH+1;
    ब_अक्षर_प्रकार अस्थायी_dir_ce[k_Max_Path_आकार] = {0};
    ब_अक्षर_प्रकार अस्थायी_file[k_Max_Path_आकार] = {0};

    ::Get_अस्थायी_Path_W(k_Max_Path_आकार, अस्थायी_dir_ce);

    अगर(0 != ::Get_अस्थायी_File_Name_W(अस्थायी_dir_ce, L"ocv", 0, अस्थायी_file)) {
        DeleteFileW(अस्थायी_file);
        अक्षर aname[MAX_PATH];
        माप_प्रकार copied = wcstombs(aname, अस्थायी_file, MAX_PATH);
        CV_Assert((copied != MAX_PATH) && (copied != (माप_प्रकार)-1));
        fname = String(aname);
    }
#अन्यथा
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    ब_अक्षर_प्रकार अस्थायी_dir2_w[MAX_PATH] = { 0 };

    अगर (अस्थायी_dir.empty())
    {
        ::Get_अस्थायी_Path_W(माप(अस्थायी_dir2_w)/माप(ब_अक्षर_प्रकार), अस्थायी_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        पूर्णांक len = WideCharToMultiByte(CP_UTF8, 0, अस्थायी_dir2_w, -1, शून्य, 0, शून्य, शून्य);
        अगर (len > 0)
        {
            std::vector<अक्षर> utf8_buf(len);
            WideCharToMultiByte(CP_UTF8, 0, अस्थायी_dir2_w, -1, utf8_buf.तथ्य(), len, शून्य, शून्य);
            अस्थायी_dir = std::string(utf8_buf.तथ्य());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    अगर (FAILED(hr))
        वापस String();
    अक्षर guidStr[40];
    स्थिर अक्षर* mask = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, माप(guidStr), mask,
            g.Data1, g.Data2, g.Data3, (अचिन्हित पूर्णांक)g.Data4[0], (अचिन्हित पूर्णांक)g.Data4[1],
            (अचिन्हित पूर्णांक)g.Data4[2], (अचिन्हित पूर्णांक)g.Data4[3], (अचिन्हित पूर्णांक)g.Data4[4],
            (अचिन्हित पूर्णांक)g.Data4[5], (अचिन्हित पूर्णांक)g.Data4[6], (अचिन्हित पूर्णांक)g.Data4[7]);

    fname = अस्थायी_dir;
    अगर (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#पूर्ण_अगर
# अन्यथा
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    अक्षर defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  अन्यथा
    अक्षर defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    अगर (अस्थायी_dir.empty())
        fname = defaultTemplate;
    अन्यथा
    {
        fname = अस्थायी_dir;
        अक्षर ech = fname[fname.size() - 1];
        अगर(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    स्थिर पूर्णांक fd = mkstemp((अक्षर*)fname.c_str());
    अगर (fd == -1) वापस String();

    बंद(fd);
    हटाओ(fname.c_str());
# endif

    अगर (suffix)
    {
        अगर (suffix[0] != '.')
            वापस fname + "." + suffix;
        अन्यथा
            वापस fname + suffix;
    }
    वापस fname;
#अन्यथा // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_UNUSED(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#पूर्ण_अगर // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

अटल ErrorCallback customErrorCallback = 0;
अटल व्योम* custom_Error_Callback_तथ्य = 0;
अटल bool breakOnError = false;

bool setBreakOnError(bool value)
{
    bool पूर्व_Val = breakOnError;
    breakOnError = value;
    वापस पूर्व_Val;
}

पूर्णांक cv_snprintf(अक्षर* buf, पूर्णांक len, स्थिर अक्षर* fmt, ...)
{
    बहु_सूची va;
    बहु_शुरू(va, fmt);
    पूर्णांक res = cv_vsnprintf(buf, len, fmt, va);
    बहु_पूर्ण(va);
    वापस res;
}

पूर्णांक cv_vsnprintf(अक्षर* buf, पूर्णांक len, स्थिर अक्षर* fmt, बहु_सूची args)
{
#अगर defined _MSC_VER
    अगर (len <= 0) वापस len == 0 ? 1024 : -1;
    पूर्णांक res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
    // ensure null terminating on VS
    अगर (res >= 0 && res < len)
    {
        buf[res] = 0;
        वापस res;
    }
    अन्यथा
    {
        buf[len - 1] = 0; // truncate happened
        वापस res >= len ? res : (len * 2);
    }
#अन्यथा
    वापस vsnprintf(buf, len, fmt, args);
#पूर्ण_अगर
}

अटल व्योम dumpException(स्थिर Exception& exc)
{
    स्थिर अक्षर* errorStr = exc.codeMessage();
    अक्षर buf[1 << 12];

    cv_snprintf(buf, माप(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_VERSION,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.पंक्ति);
#अगर_घोषित __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#अन्यथा
    ख_साफ(मानक_निकास); ख_साफ(मानक_त्रुटि);
    ख_लिखो(मानक_त्रुटि, "%s\n", buf);
    ख_साफ(मानक_त्रुटि);
#पूर्ण_अगर
}

#अगर_घोषित CV_ERROR_SET_TERMINATE_HANDLER
अटल bool cv_terminate_handler_installed = false;
अटल std::terminate_handler cv_old_terminate_handler;
अटल cv::Exception cv_terminate_handler_exception;
अटल bool param_setupTerminateHandler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", true);
अटल व्योम cv_terminate_handler() {
    std::अ_त्रुटि << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    अगर (false /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    पात();
}

#पूर्ण_अगर

#अगर_घोषित __GNUC__
# अगर defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#पूर्ण_अगर

व्योम error( स्थिर Exception& exc )
{
#अगर_घोषित CV_ERROR_SET_TERMINATE_HANDLER
    {
        cv::AutoLock lock(getInitializationMutex());
        अगर (!cv_terminate_handler_installed)
        {
            अगर (param_setupTerminateHandler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = true;
        }
        cv_terminate_handler_exception = exc;
    }
#पूर्ण_अगर

    अगर (customErrorCallback != 0)
        customErrorCallback(exc.code, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.पंक्ति, custom_Error_Callback_तथ्य);
    अन्यथा अगर (param_dumpErrors)
    {
        dumpException(exc);
    }

    अगर(breakOnError)
    {
        std::terminate();
    }

    फेंको exc;
#अगर_घोषित __GNUC__
# अगर !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#पूर्ण_अगर
}

व्योम error(Error::Code _code, स्थिर String& _err, स्थिर अक्षर* _func, स्थिर अक्षर* _file, पूर्णांक पंक्ति_3)
{
    error(cv::Exception(_code, _err, _func, _file, पंक्ति_3));
#अगर_घोषित __GNUC__
# अगर !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#पूर्ण_अगर
}

#अगर_घोषित __GNUC__
# अगर defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#पूर्ण_अगर


ErrorCallback
redirectError( ErrorCallback errCallback, व्योम* userdata, व्योम** पूर्व_Userdata)
{
    अगर( पूर्व_Userdata )
        *पूर्व_Userdata = custom_Error_Callback_तथ्य;

    ErrorCallback पूर्व_Callback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_तथ्य = userdata;

    वापस पूर्व_Callback;
}

व्योम terminate(Error::Code _code, स्थिर String& _err, स्थिर अक्षर* _func, स्थिर अक्षर* _file, पूर्णांक पंक्ति_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(_code, _err, _func, _file, पंक्ति_3));
    std::terminate();
}

}

namespace cv {
bool __termination = false;


//////////////////////////////// thread-local storage ////////////////////////////////

namespace details {

#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT

#अगर_घोषित _WIN32
#अगर_घोषित _MSC_VER
#आशय warning(disable:4505) // unreferenced local function has been removed
#पूर्ण_अगर
#अगर_अघोषित TLS_OUT_OF_INDEXES
#घोषणा TLS_OUT_OF_INDEXES ((DWORD)0xFFFFFFFF)
#पूर्ण_अगर
#पूर्ण_अगर

// TLS platform abstraction layer
श्रेणी TlsAbstraction
{
खुला:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        ख_साफ(मानक_त्रुटि);
    }

    व्योम* get_तथ्य() स्थिर;
    व्योम set_तथ्य(व्योम *p_तथ्य);

    व्योम releaseSystemResources();

निजी:

#अगर_घोषित _WIN32
#अगर_अघोषित WINRT
    DWORD tls_कुंजी;
    bool disposed;
#पूर्ण_अगर
#अन्यथा // _WIN32
    pthread_key_t  tls_कुंजी;
    std::atomic<bool> disposed;
#पूर्ण_अगर
};

श्रेणी TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
खुला:
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
अटल TlsAbstraction* getTlsAbstraction()
{
    अटल TlsAbstraction *g_tls = नया TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    अटल TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    वापस g_tls;
}


#अगर_घोषित _WIN32
#अगर_घोषित WINRT
अटल __declspec( thread ) व्योम* tls_तथ्य = शून्य; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
व्योम TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
}
व्योम* TlsAbstraction::get_तथ्य() स्थिर
{
    वापस tls_तथ्य;
}
व्योम TlsAbstraction::set_तथ्य(व्योम *p_तथ्य)
{
    tls_तथ्य = p_तथ्य;
}
#अन्यथा //WINRT
#अगर_घोषित CV_USE_FLS
अटल व्योम NTAPI opencv_fls_destructor(व्योम* p_तथ्य);
#पूर्ण_अगर // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(false)
{
#अगर_अघोषित CV_USE_FLS
    tls_कुंजी = TlsAlloc();
#अन्यथा // CV_USE_FLS
    tls_कुंजी = FlsAlloc(opencv_fls_destructor);
#पूर्ण_अगर // CV_USE_FLS
    CV_Assert(tls_कुंजी != TLS_OUT_OF_INDEXES);
}
व्योम TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
#अगर_अघोषित CV_USE_FLS
    TlsFree(tls_कुंजी);
#अन्यथा // CV_USE_FLS
    FlsFree(tls_कुंजी);
#पूर्ण_अगर // CV_USE_FLS
    tls_कुंजी = TLS_OUT_OF_INDEXES;
}
व्योम* TlsAbstraction::get_तथ्य() स्थिर
{
    अगर (disposed)
        वापस शून्य;
#अगर_अघोषित CV_USE_FLS
    वापस Tls_Get_मूल्य(tls_कुंजी);
#अन्यथा // CV_USE_FLS
    वापस Fls_Get_मूल्य(tls_कुंजी);
#पूर्ण_अगर // CV_USE_FLS
}
व्योम TlsAbstraction::set_तथ्य(व्योम *p_तथ्य)
{
    अगर (disposed)
        वापस;  // no-op
#अगर_अघोषित CV_USE_FLS
    CV_Assert(Tls_Set_मूल्य(tls_कुंजी, p_तथ्य) == TRUE);
#अन्यथा // CV_USE_FLS
    CV_Assert(Fls_Set_मूल्य(tls_कुंजी, p_तथ्य) == TRUE);
#पूर्ण_अगर // CV_USE_FLS
}
#पूर्ण_अगर // WINRT
#अन्यथा // _WIN32
अटल व्योम opencv_tls_destructor(व्योम* p_तथ्य);
TlsAbstraction::TlsAbstraction()
    : disposed(false)
{
    CV_Assert(pthread_key_create(&tls_कुंजी, opencv_tls_destructor) == 0);
}
व्योम TlsAbstraction::releaseSystemResources()
{
    cv::__termination = true;  // DllMain is missing in static builds
    disposed = true;
    अगर (pthread_key_delete(tls_कुंजी) != 0)
    {
        // Don't use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        ख_साफ(मानक_त्रुटि);
    }
}
व्योम* TlsAbstraction::get_तथ्य() स्थिर
{
    अगर (disposed)
        वापस शून्य;
    वापस pthread_getspecific(tls_कुंजी);
}
व्योम TlsAbstraction::set_तथ्य(व्योम *p_तथ्य)
{
    अगर (disposed)
        वापस;  // no-op
    CV_Assert(pthread_setspecific(tls_कुंजी, p_तथ्य) == 0);
}
#पूर्ण_अगर

// Per-thread data structure
काष्ठा Thread_तथ्य
{
    Thread_तथ्य()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<व्योम*> slots; // Data array for a thread
    माप_प्रकार idx;               // Thread index in TLS storage. This is not OS thread ID!
};


अटल bool g_isTlsStorageInitialized = false;

// Main TLS storage class
श्रेणी TlsStorage
{
खुला:
    TlsStorage() :
        tls_Slots_आकार(0)
    {
        (व्योम)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        threads.reserve(32);
        g_isTlsStorageInitialized = true;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        ख_लिखो(मानक_त्रुटि, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        ख_साफ(मानक_त्रुटि);
    }

    व्योम releaseThread(व्योम* tls_मूल्य = शून्य)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        अगर (शून्य == tls)
            वापस;  // TLS singleton is not available (terminated)
        Thread_तथ्य *pTD = tls_मूल्य == शून्य ? (Thread_तथ्य*)tls->get_तथ्य() : (Thread_तथ्य*)tls_मूल्य;
        अगर (pTD == शून्य)
            वापस;  // no OpenCV TLS data for this thread
        AutoLock guard(mtxGlobalAccess);
        क्रम (माप_प्रकार i = 0; i < threads.size(); i++)
        {
            अगर (pTD == threads[i])
            {
                threads[i] = शून्य;
                अगर (tls_मूल्य == शून्य)
                    tls->set_तथ्य(0);
                std::vector<व्योम*>& thread_slots = pTD->slots;
                क्रम (माप_प्रकार slotIdx = 0; slotIdx < thread_slots.size(); slotIdx++)
                {
                    व्योम* p_तथ्य = thread_slots[slotIdx];
                    thread_slots[slotIdx] = शून्य;
                    अगर (!p_तथ्य)
                        जारी;
                    TLSDataContainer* container = tlsSlots[slotIdx].container;
                    अगर (container)
                        container->delete_तथ्य_Instance(p_तथ्य);
                    अन्यथा
                    {
                        ख_लिखो(मानक_त्रुटि, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (पूर्णांक)slotIdx);
                        ख_साफ(मानक_त्रुटि);
                    }
                }
                मिटाओ pTD;
                वापस;
            }
        }
        ख_लिखो(मानक_त्रुटि, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (व्योम*)pTD); ख_साफ(मानक_त्रुटि);
    }

    // Reserve TLS storage index
    माप_प्रकार reserveSlot(TLSDataContainer* container)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());

        // Find unused slots
        क्रम(माप_प्रकार slot = 0; slot < tls_Slots_आकार; slot++)
        {
            अगर (tlsSlots[slot].container == शून्य)
            {
                tlsSlots[slot].container = container;
                वापस slot;
            }
        }

        // Create new slot
        tlsSlots.push_back(TlsSlotInfo(container)); tls_Slots_आकार++;
        वापस tls_Slots_आकार - 1;
    }

    // Release TLS storage index and pass associated data to caller
    व्योम releaseSlot(माप_प्रकार slotIdx, std::vector<व्योम*> &तथ्य_Vec, bool keepSlot = false)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());
        CV_Assert(tls_Slots_आकार > slotIdx);

        क्रम(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            अगर(threads[i])
            {
                std::vector<व्योम*>& thread_slots = threads[i]->slots;
                अगर (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                {
                    तथ्य_Vec.push_back(thread_slots[slotIdx]);
                    thread_slots[slotIdx] = शून्य;
                }
            }
        }

        अगर (!keepSlot)
        {
            tlsSlots[slotIdx].container = शून्य;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    व्योम* get_तथ्य(माप_प्रकार slotIdx) स्थिर
    {
#अगर_अघोषित CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_आकार > slotIdx);
#पूर्ण_अगर

        TlsAbstraction* tls = getTlsAbstraction();
        अगर (शून्य == tls)
            वापस शून्य;  // TLS singleton is not available (terminated)

        Thread_तथ्य* thread_तथ्य = (Thread_तथ्य*)tls->get_तथ्य();
        अगर(thread_तथ्य && thread_तथ्य->slots.size() > slotIdx)
            वापस thread_तथ्य->slots[slotIdx];

        वापस शून्य;
    }

    // Gather data from threads by TLS storage index
    व्योम gather(माप_प्रकार slotIdx, std::vector<व्योम*> &तथ्य_Vec)
    {
        AutoLock guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_आकार == tlsSlots.size());
        CV_Assert(tls_Slots_आकार > slotIdx);

        क्रम(माप_प्रकार i = 0; i < threads.size(); i++)
        {
            अगर(threads[i])
            {
                std::vector<व्योम*>& thread_slots = threads[i]->slots;
                अगर (thread_slots.size() > slotIdx && thread_slots[slotIdx])
                    तथ्य_Vec.push_back(thread_slots[slotIdx]);
            }
        }
    }

    // Set data to storage index
    व्योम set_तथ्य(माप_प्रकार slotIdx, व्योम* p_तथ्य)
    {
#अगर_अघोषित CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_आकार > slotIdx);
#पूर्ण_अगर

        TlsAbstraction* tls = getTlsAbstraction();
        अगर (शून्य == tls)
            वापस;  // TLS singleton is not available (terminated)

        Thread_तथ्य* thread_तथ्य = (Thread_तथ्य*)tls->get_तथ्य();
        अगर(!thread_तथ्य)
        {
            thread_तथ्य = नया Thread_तथ्य;
            tls->set_तथ्य((व्योम*)thread_तथ्य);
            {
                AutoLock guard(mtxGlobalAccess);

                bool found = false;
                // Find unused slots
                क्रम(माप_प्रकार slot = 0; slot < threads.size(); slot++)
                {
                    अगर (threads[slot] == शून्य)
                    {
                        thread_तथ्य->idx = (पूर्णांक)slot;
                        threads[slot] = thread_तथ्य;
                        found = true;
                        अवरोध;
                    }
                }

                अगर (!found)
                {
                    // Create new slot
                    thread_तथ्य->idx = threads.size();
                    threads.push_back(thread_तथ्य);
                }
            }
        }

        अगर(slotIdx >= thread_तथ्य->slots.size())
        {
            AutoLock guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_तथ्य->slots.resize(slotIdx + 1, शून्य);
        }
        thread_तथ्य->slots[slotIdx] = p_तथ्य;
    }

निजी:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    माप_प्रकार tls_Slots_आकार;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    काष्ठा TlsSlotInfo
    {
        TlsSlotInfo(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<काष्ठा TlsSlotInfo> tlsSlots;  // TLS keys state
    std::vector<Thread_तथ्य*> threads; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
अटल TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, नया TlsStorage())
}

#अगर_अघोषित _WIN32  // pthread key destructor
अटल व्योम opencv_tls_destructor(व्योम* p_तथ्य)
{
    अगर (!g_isTlsStorageInitialized)
        वापस;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_तथ्य);
}
#अन्यथा // _WIN32
#अगर_घोषित CV_USE_FLS
अटल व्योम WINAPI opencv_fls_destructor(व्योम* p_तथ्य)
{
    // Empiric detection of ExitProcess call
    DWORD code = STILL_ACTIVE/*259*/;
    BOOL res = GetExitCodeProcess(GetCurrentProcess(), &code);
    अगर (res && code != STILL_ACTIVE)
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

    अगर (!g_isTlsStorageInitialized)
        वापस;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_तथ्य);
}
#पूर्ण_अगर // CV_USE_FLS
#पूर्ण_अगर // _WIN32

अटल TlsStorage* स्थिर g_force_initialization_of_TlsStorage
#अगर defined __GNUC__
    __attribute__((unused))
#पूर्ण_अगर
    = &getTlsStorage();


#अन्यथा  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
श्रेणी TlsStorage
{
खुला:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        क्रम (माप_प्रकार slotIdx = 0; slotIdx < slots.size(); slotIdx++)
        {
            SlotInfo& s = slots[slotIdx];
            TLSDataContainer* container = s.container;
            अगर (container && s.तथ्य)
            {
                container->delete_तथ्य_Instance(s.तथ्य);  // Can't use from SlotInfo destructor
                s.तथ्य = nullptr;
            }
        }
    }

    // Reserve TLS storage index
    माप_प्रकार reserveSlot(TLSDataContainer* container)
    {
        माप_प्रकार slots_आकार = slots.size();
        क्रम (माप_प्रकार slot = 0; slot < slots_आकार; slot++)
        {
            SlotInfo& s = slots[slot];
            अगर (s.container == शून्य)
            {
                CV_Assert(!s.तथ्य);
                s.container = container;
                वापस slot;
            }
        }

        // create new slot
        slots.push_back(SlotInfo(container));
        वापस slots_आकार;
    }

    // Release TLS storage index and pass associated data to caller
    व्योम releaseSlot(माप_प्रकार slotIdx, std::vector<व्योम*> &तथ्य_Vec, bool keepSlot = false)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        व्योम* तथ्य = s.तथ्य;
        अगर (तथ्य)
        {
            तथ्य_Vec.push_back(तथ्य);
            s.तथ्य = nullptr;
        }
        अगर (!keepSlot)
        {
            s.container = शून्य;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    व्योम* get_तथ्य(माप_प्रकार slotIdx) स्थिर
    {
        CV_Assert(slotIdx < slots.size());
        स्थिर SlotInfo& s = slots[slotIdx];
        वापस s.तथ्य;
    }

    // Gather data from threads by TLS storage index
    व्योम gather(माप_प्रकार slotIdx, std::vector<व्योम*> &तथ्य_Vec)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        व्योम* तथ्य = s.तथ्य;
        अगर (तथ्य)
            तथ्य_Vec.push_back(तथ्य);
        वापस;
    }

    // Set data to storage index
    व्योम set_तथ्य(माप_प्रकार slotIdx, व्योम* p_तथ्य)
    {
        CV_Assert(slotIdx < slots.size());
        SlotInfo& s = slots[slotIdx];
        s.तथ्य = p_तथ्य;
    }

निजी:
    काष्ठा SlotInfo
    {
        SlotInfo(TLSDataContainer* _container) : container(_container), तथ्य(nullptr) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        व्योम* तथ्य;
    };
    std::vector<काष्ठा SlotInfo> slots;
};

अटल TlsStorage& getTlsStorage()
{
    अटल TlsStorage g_storage;  // no threading
    वापस g_storage;
}

#पूर्ण_अगर  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
using namespace details;

व्योम releaseTlsStorageThread()
{
#अगर_अघोषित OPENCV_DISABLE_THREAD_SUPPORT
    अगर (!g_isTlsStorageInitialized)
        वापस;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#पूर्ण_अगर
}

TLSDataContainer::TLSDataContainer()
{
    कुंजी_2 = (पूर्णांक)getTlsStorage().reserveSlot(यह); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(कुंजी_2 == -1); // Key must be released in child object
}

व्योम TLSDataContainer::gather_तथ्य(std::vector<व्योम*> &तथ्य) स्थिर
{
    getTlsStorage().gather(कुंजी_2, तथ्य);
}

व्योम TLSDataContainer::detach_तथ्य(std::vector<व्योम*> &तथ्य)
{
    getTlsStorage().releaseSlot(कुंजी_2, तथ्य, true);
}

व्योम TLSDataContainer::release()
{
    अगर (कुंजी_2 == -1)
        वापस;  // already released
    std::vector<व्योम*> तथ्य; तथ्य.reserve(32);
    getTlsStorage().releaseSlot(कुंजी_2, तथ्य, false); // Release key and get stored data for proper destruction
    कुंजी_2 = -1;
    क्रम(माप_प्रकार i = 0; i < तथ्य.size(); i++)  // Delete all associated data
        delete_तथ्य_Instance(तथ्य[i]);
}

व्योम TLSDataContainer::cleanup()
{
    std::vector<व्योम*> तथ्य; तथ्य.reserve(32);
    getTlsStorage().releaseSlot(कुंजी_2, तथ्य, true); // Extract stored data with removal from TLS tables
    क्रम(माप_प्रकार i = 0; i < तथ्य.size(); i++)  // Delete all associated data
        delete_तथ्य_Instance(तथ्य[i]);
}

व्योम* TLSDataContainer::get_तथ्य() स्थिर
{
    CV_Assert(कुंजी_2 != -1 && "Can't fetch data from terminated TLS container.");
    व्योम* p_तथ्य = getTlsStorage().get_तथ्य(कुंजी_2); // Check if data was already allocated
    अगर(!p_तथ्य)
    {
        // Create new data instance and save it to TLS storage
        p_तथ्य = create_तथ्य_Instance();
        प्रयास
        {
            getTlsStorage().set_तथ्य(कुंजी_2, p_तथ्य);
        }
        पकड़ो (...)
        {
            delete_तथ्य_Instance(p_तथ्य);
            फेंको;
        }
    }
    वापस p_तथ्य;
}

अटल TLSData<CoreTLSData>& get_Core_Tls_तथ्य_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, नया TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_तथ्य()
{
    वापस get_Core_Tls_तथ्य_TLS().getRef();
}

#अगर defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#अगर_घोषित WINRT
    #आशय warning(disable:4447) // Disable warning 'main' signature found without threading model
#पूर्ण_अगर

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

बाह्य "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    अगर (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        अगर (lpReserved != शून्य) // called after ExitProcess() call
        {
            cv::__termination = true;
        }
        अन्यथा
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    वापस TRUE;
}
#पूर्ण_अगर


namespace {

#अगर_घोषित OPENCV_WITH_ITT
bool overrideThreadName()
{
    अटल bool param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", false);
    वापस param;
}
#पूर्ण_अगर

अटल पूर्णांक g_thread_अंक = 0;
श्रेणी ThreadID {
खुला:
    स्थिर पूर्णांक id;
    ThreadID() :
        id(CV_XADD(&g_thread_अंक, 1))
    {
#अगर_घोषित OPENCV_WITH_ITT
        अगर (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#पूर्ण_अगर
    }
};

अटल TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, नया TLSData<ThreadID>());
}

} // namespace
पूर्णांक utils::getThreadID() { वापस getThreadIDTLS().लो()->id; }


श्रेणी ParseError
{
    std::string bad_मूल्य;
खुला:
    ParseError(स्थिर std::string &bad_मूल्य_2) :bad_मूल्य(bad_मूल्य_2) {}
    std::string toString(स्थिर std::string &param) स्थिर
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_मूल्य;
        वापस out.str();
    }
};

ढाँचा <typename T>
T parseOption(स्थिर std::string &);

ढाँचा<>
अंतरभूत bool parseOption(स्थिर std::string & value)
{
    अगर (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        वापस true;
    }
    अगर (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        वापस false;
    }
    फेंको ParseError(value);
}

ढाँचा<>
अंतरभूत माप_प्रकार parseOption(स्थिर std::string &value)
{
    माप_प्रकार pos = 0;
    क्रम (; pos < value.size(); pos++)
    {
        अगर (!है_अंक(value[pos]))
            अवरोध;
    }
    cv::String मूल्य_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    माप_प्रकार v = (माप_प्रकार)std::stoull(मूल्य_Str);
    अगर (suffixStr.length() == 0)
        वापस v;
    अन्यथा अगर (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        वापस v * 1024 * 1024;
    अन्यथा अगर (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        वापस v * 1024;
    फेंको ParseError(value);
}

ढाँचा<>
अंतरभूत cv::String parseOption(स्थिर std::string &value)
{
    वापस value;
}

ढाँचा<>
अंतरभूत utils::Paths parseOption(स्थिर std::string &value)
{
    utils::Paths परिणाम;
#अगर_घोषित _WIN32
    स्थिर अक्षर sep = ';';
#अन्यथा
    स्थिर अक्षर sep = ':';
#पूर्ण_अगर
    माप_प्रकार start_pos = 0;
    जबतक (start_pos != std::string::npos)
    {
        स्थिर माप_प्रकार pos = value.find(sep, start_pos);
        स्थिर std::string one_piece(value, start_pos, pos == std::string::npos ? pos : pos - start_pos);
        अगर (!one_piece.empty())
            परिणाम.push_back(one_piece);
        start_pos = pos == std::string::npos ? pos : pos + 1;
    }
    वापस परिणाम;
}

अटल अंतरभूत स्थिर अक्षर * envRead(स्थिर अक्षर * name)
{
#अगर_घोषित NO_GETENV
    CV_UNUSED(name);
    वापस शून्य;
#अन्यथा
    वापस दो_पर्या(name);
#पूर्ण_अगर
}

ढाँचा<typename T>
अंतरभूत T पढ़ो(स्थिर std::string & k, स्थिर T & default_मूल्य_2)
{
    प्रयास
    {
        स्थिर अक्षर * res = envRead(k.c_str());
        अगर (res)
            वापस parseOption<T>(std::string(res));
    }
    पकड़ो (स्थिर ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.toString(k));
    }
    वापस default_मूल्य_2;
}

bool utils::getConfigurationParameterBool(स्थिर अक्षर* name, bool default_मूल्य_2)
{
    वापस पढ़ो<bool>(name, default_मूल्य_2);
}

माप_प्रकार utils::get_Configuration_Parameter_आकार_T(स्थिर अक्षर* name, माप_प्रकार default_मूल्य_2)
{
    वापस पढ़ो<माप_प्रकार>(name, default_मूल्य_2);
}

std::string utils::getConfigurationParameterString(स्थिर अक्षर* name, स्थिर std::string & default_मूल्य_2)
{
    वापस पढ़ो<cv::String>(name, default_मूल्य_2);
}

utils::Paths utils::getConfigurationParameterPaths(स्थिर अक्षर* name, स्थिर utils::Paths &default_मूल्य_2)
{
    वापस पढ़ो<utils::Paths>(name, default_मूल्य_2);
}


#अगर_घोषित CV_COLLECT_IMPL_तथ्य
ImplCollector& get_Impl_तथ्य()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, नया ImplCollector())
}

व्योम setImpl(पूर्णांक flags)
{
    cv::AutoLock lock(get_Impl_तथ्य().mutex);

    get_Impl_तथ्य().implFlags = flags;
    get_Impl_तथ्य().implCode.सफा();
    get_Impl_तथ्य().implFun.सफा();
}

व्योम addImpl(पूर्णांक flag, स्थिर अक्षर* func)
{
    cv::AutoLock lock(get_Impl_तथ्य().mutex);

    get_Impl_तथ्य().implFlags |= flag;
    अगर(func) // use lazy collection if name was not specified
    {
        माप_प्रकार सूचकांक = get_Impl_तथ्य().implCode.size();
        अगर(!सूचकांक || (get_Impl_तथ्य().implCode[सूचकांक-1] != flag || get_Impl_तथ्य().implFun[सूचकांक-1].compare(func))) // avoid duplicates
        {
            get_Impl_तथ्य().implCode.push_back(flag);
            get_Impl_तथ्य().implFun.push_back(func);
        }
    }
}

पूर्णांक getImpl(std::vector<पूर्णांक> &impl, std::vector<String> &funName)
{
    cv::AutoLock lock(get_Impl_तथ्य().mutex);

    impl    = get_Impl_तथ्य().implCode;
    funName = get_Impl_तथ्य().implFun;
    वापस get_Impl_तथ्य().implFlags; // return actual flags for lazy collection
}

bool useCollection()
{
    वापस get_Impl_तथ्य().useCollection;
}

व्योम setUseCollection(bool flag)
{
    cv::AutoLock lock(get_Impl_तथ्य().mutex);

    get_Impl_तथ्य().useCollection = flag;
}
#पूर्ण_अगर

namespace instr
{
bool useInstrumentation()
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    वापस getInstrumentStruct().useInstr;
#अन्यथा
    वापस false;
#पूर्ण_अगर
}

व्योम setUseInstrumentation(bool flag)
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#अन्यथा
    CV_UNUSED(flag);
#पूर्ण_अगर
}

Instr_गाँठ* getTrace()
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    वापस &getInstrumentStruct().मूल_गाँठ_2;
#अन्यथा
    वापस शून्य;
#पूर्ण_अगर
}

व्योम resetTrace()
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    getInstrumentStruct().मूल_गाँठ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_गाँठ = &getInstrumentStruct().मूल_गाँठ_2;
#पूर्ण_अगर
}

व्योम setFlags(FLAGS modeFlags)
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = modeFlags;
#अन्यथा
    CV_UNUSED(modeFlags);
#पूर्ण_अगर
}
FLAGS getFlags()
{
#अगर_घोषित ENABLE_INSTRUMENTATION
    वापस (FLAGS)getInstrumentStruct().flags;
#अन्यथा
    वापस (FLAGS)0;
#पूर्ण_अगर
}

गाँठ_तथ्य::गाँठ_तथ्य(स्थिर अक्षर* funName, स्थिर अक्षर* fileName, पूर्णांक पंक्ति_अंक, व्योम* retAddress, bool alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_पंक्ति_अंक       = पंक्ति_अंक;
    m_retAddress    = retAddress;
    m_alwaysExpand  = alwaysExpand;

    m_threads    = 1;
    m_गणक    = 0;
    m_ticks_योग = 0;

    m_funError  = false;
}
गाँठ_तथ्य::गाँठ_तथ्य(गाँठ_तथ्य &ref)
{
    *यह = ref;
}
गाँठ_तथ्य& गाँठ_तथ्य::चालक=(स्थिर गाँठ_तथ्य &दाँए)
{
    यह->m_funName      = दाँए.m_funName;
    यह->m_instrType    = दाँए.m_instrType;
    यह->m_implType     = दाँए.m_implType;
    यह->m_fileName     = दाँए.m_fileName;
    यह->m_पंक्ति_अंक      = दाँए.m_पंक्ति_अंक;
    यह->m_retAddress   = दाँए.m_retAddress;
    यह->m_alwaysExpand = दाँए.m_alwaysExpand;

    यह->m_threads     = दाँए.m_threads;
    यह->m_गणक     = दाँए.m_गणक;
    यह->m_ticks_योग  = दाँए.m_ticks_योग;

    यह->m_funError    = दाँए.m_funError;

    वापस *यह;
}
गाँठ_तथ्य::~गाँठ_तथ्य()
{
}
bool चालक==(स्थिर गाँठ_तथ्य& बाँए, स्थिर गाँठ_तथ्य& दाँए)
{
    अगर(बाँए.m_पंक्ति_अंक == दाँए.m_पंक्ति_अंक && बाँए.m_funName == दाँए.m_funName && बाँए.m_fileName == दाँए.m_fileName)
    {
        अगर(बाँए.m_retAddress == दाँए.m_retAddress || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_NAMES || बाँए.m_alwaysExpand))
            वापस true;
    }
    वापस false;
}

#अगर_घोषित ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    अटल InstrStruct instr;
    वापस instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    वापस *getInstrumentStruct().tlsStruct.लो();
}

Instr_गाँठ* get_Current_गाँठ()
{
    वापस getInstrumentTLSStruct().p_Current_गाँठ;
}

IntrumentationRegion::IntrumentationRegion(स्थिर अक्षर* funName, स्थिर अक्षर* fileName, पूर्णांक पंक्ति_अंक, व्योम *retAddress, bool alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = false;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    अगर(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        अगर(!pTLS->p_Current_गाँठ)
        {
            m_disabled = true;
            वापस;
        }

        पूर्णांक depth = pTLS->p_Current_गाँठ->getDepth();
        अगर(pStruct->maxDepth && pStruct->maxDepth <= depth)
        {
            m_disabled = true;
            वापस;
        }

        गाँठ_तथ्य payload(funName, fileName, पंक्ति_अंक, retAddress, alwaysExpand, instrType, implType);
        Node<गाँठ_तथ्य>* pChild = शून्य;

        अगर(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::AutoLock guard(pStruct->mutexCreate); // Guard from concurrent child creation
            pChild = pTLS->p_Current_गाँठ->findChild(payload);
            अगर(!pChild)
            {
                pChild = नया Node<गाँठ_तथ्य>(payload);
                pTLS->p_Current_गाँठ->addChild(pChild);
            }
        }
        अन्यथा
        {
            pChild = pTLS->p_Current_गाँठ->findChild(payload);
            अगर(!pChild)
            {
                m_disabled = true;
                वापस;
            }
        }
        pTLS->p_Current_गाँठ = pChild;

        m_regionTicks = get_Tick_गिनती();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    अगर(pStruct->useInstr)
    {
        अगर(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            अगर (pTLS->p_Current_गाँठ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_गाँठ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_गाँठ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::finish(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_गिनती() - m_regionTicks);
            {
                cv::AutoLock guard(pStruct->mutex_गिनती); // Concurrent ticks accumulation
                pTLS->p_Current_गाँठ->m_payload.m_गणक++;
                pTLS->p_Current_गाँठ->m_payload.m_ticks_योग += ticks;
                pTLS->p_Current_गाँठ->m_payload.m_tls.लो()->m_ticks_योग += ticks;
            }

            pTLS->p_Current_गाँठ = pTLS->p_Current_गाँठ->m_pParent;
        }
    }
}
#पूर्ण_अगर
}

namespace ipp
{

#अगर_घोषित HAVE_IPP
काष्ठा IPPInitSingleton
{
खुला:
    IPPInitSingleton()
    {
        useIPP         = true;

#अगर defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#अन्यथा
        useIPP_NE      = false;
#पूर्ण_अगर
        ippStatus      = 0;
        funcname       = शून्य;
        filename       = शून्य;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_शीर्ष_Features = 0;
        pIppLibInfo    = शून्य;

        ippStatus = ippGetCpuFeatures(&cpuFeatures, शून्य);
        अगर(ippStatus < 0)
        {
            CV_LOG_ERROR(शून्य, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = false;
            वापस;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        अगर(!env.empty())
        {
#अगर IPP_VERSION_X100 >= 201900
            स्थिर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#या_अगर IPP_VERSION_X100 >= 201703
            स्थिर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#या_अगर IPP_VERSION_X100 >= 201700
            स्थिर Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#अन्यथा
            स्थिर Ipp64u minorFeatures = 0;
#पूर्ण_अगर

            env = toLowerCase(env);
            अगर(env.substr(0, 2) == "ne")
            {
                useIPP_NE = true;
                env = env.substr(3, env.size());
            }

            अगर(env == "disabled")
            {
                CV_LOG_WARNING(शून्य, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = false;
            }
            अन्यथा अगर(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            अन्यथा अगर(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#अगर IPP_VERSION_X100 >= 201700
#अगर defined (_M_AMD64) || defined (__x86_64__)
            अन्यथा अगर(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#पूर्ण_अगर
#पूर्ण_अगर
            अन्यथा
                CV_LOG_ERROR(शून्य, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        अगर(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        अगर(!(
#अगर IPP_VERSION_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#पूर्ण_अगर
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = false;
            वापस;
        }

        अगर(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        अन्यथा
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#अगर IPP_VERSION_X100 >= 201700
        अगर(ippFeatures&ippCPUID_AVX512F)
        {
            अगर((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_शीर्ष_Features = ippCPUID_AVX512_SKX;
            अन्यथा अगर((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_शीर्ष_Features = ippCPUID_AVX512_KNL;
            अन्यथा
                ipp_शीर्ष_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        अन्यथा
#पूर्ण_अगर
        अगर(ippFeatures&ippCPUID_AVX2)
            ipp_शीर्ष_Features = ippCPUID_AVX2;
        अन्यथा अगर(ippFeatures&ippCPUID_SSE42)
            ipp_शीर्ष_Features = ippCPUID_SSE42;

        pIppLibInfo = ippiGetLibVersion();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        अगर (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_शीर्ष_Features = ippCPUID_SSE42;
        }
    }

खुला:
    bool        useIPP;
    bool        useIPP_NE;

    पूर्णांक         ippStatus;  // 0 - all is ok, -1 - IPP functions failed
    स्थिर अक्षर *funcname;
    स्थिर अक्षर *filename;
    पूर्णांक         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_शीर्ष_Features;
    स्थिर IppLibraryVersion *pIppLibInfo;
};

अटल IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, नया IPPInitSingleton())
}
#पूर्ण_अगर

अचिन्हित दीर्घ दीर्घ getIppFeatures()
{
#अगर_घोषित HAVE_IPP
    वापस getIPPSingleton().ippFeatures;
#अन्यथा
    वापस 0;
#पूर्ण_अगर
}

#अगर_घोषित HAVE_IPP
अचिन्हित दीर्घ दीर्घ get_Ipp_शीर्ष_Features()
{
    वापस getIPPSingleton().ipp_शीर्ष_Features;
}
#पूर्ण_अगर

व्योम setIppStatus(पूर्णांक status, स्थिर अक्षर * स्थिर _funcname, स्थिर अक्षर * स्थिर _filename, पूर्णांक पंक्ति_3)
{
#अगर_घोषित HAVE_IPP
    getIPPSingleton().ippStatus = status;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = पंक्ति_3;
#अन्यथा
    CV_UNUSED(status); CV_UNUSED(_funcname); CV_UNUSED(_filename); CV_UNUSED(पंक्ति_3);
#पूर्ण_अगर
}

पूर्णांक getIppStatus()
{
#अगर_घोषित HAVE_IPP
    वापस getIPPSingleton().ippStatus;
#अन्यथा
    वापस 0;
#पूर्ण_अगर
}

String getIppErrorLocation()
{
#अगर_घोषित HAVE_IPP
    वापस format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#अन्यथा
    वापस String();
#पूर्ण_अगर
}

String getIppVersion()
{
#अगर_घोषित HAVE_IPP
    स्थिर IppLibraryVersion *pInfo = getIPPSingleton().pIppLibInfo;
    अगर(pInfo)
        वापस format("%s %s %s", pInfo->Name, pInfo->Version, pInfo->BuildDate);
    अन्यथा
        वापस String("error");
#अन्यथा
    वापस String("disabled");
#पूर्ण_अगर
}

bool useIPP()
{
#अगर_घोषित HAVE_IPP
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    अगर (तथ्य.useIPP < 0)
    {
        तथ्य.useIPP = getIPPSingleton().useIPP;
    }
    वापस (तथ्य.useIPP > 0);
#अन्यथा
    वापस false;
#पूर्ण_अगर
}

व्योम setUseIPP(bool flag)
{
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
#अगर_घोषित HAVE_IPP
    तथ्य.useIPP = (getIPPSingleton().useIPP)?flag:false;
#अन्यथा
    CV_UNUSED(flag);
    तथ्य.useIPP = false;
#पूर्ण_अगर
}

bool useIPP_NotExact()
{
#अगर_घोषित HAVE_IPP
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
    अगर (तथ्य.useIPP_NE < 0)
    {
        तथ्य.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    वापस (तथ्य.useIPP_NE > 0);
#अन्यथा
    वापस false;
#पूर्ण_अगर
}

व्योम setUseIPP_NotExact(bool flag)
{
    CoreTLSData& तथ्य = get_Core_Tls_तथ्य();
#अगर_घोषित HAVE_IPP
    तथ्य.useIPP_NE = flag;
#अन्यथा
    CV_UNUSED(flag);
    तथ्य.useIPP_NE = false;
#पूर्ण_अगर
}

} // namespace ipp


namespace details {

#अगर OPENCV_IMPL_FP_HINTS_X86
#अगर_अघोषित _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#घोषणा _MM_DENORMALS_ZERO_ON 0x0040
#पूर्ण_अगर
#अगर_अघोषित _MM_DENORMALS_ZERO_MASK  // requires pmmintrin.h (SSE3)
#घोषणा _MM_DENORMALS_ZERO_MASK 0x0040
#पूर्ण_अगर
#पूर्ण_अगर

व्योम setFPDenormalsIgnoreHint(bool ignore, CV_OUT FPDenormalsModeState& state)
{
#अगर OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    अचिन्हित value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    अगर (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    स्थिर अचिन्हित old_flags = _mm_getcsr();
    स्थिर अचिन्हित old_मूल्य = old_flags & mask;
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(शून्य, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मूल्य;
    _mm_setcsr(flags);
#अन्यथा
    CV_UNUSED(ignore); CV_UNUSED(state);
#पूर्ण_अगर
}

पूर्णांक saveFPDenormalsState(CV_OUT FPDenormalsModeState& state)
{
#अगर OPENCV_IMPL_FP_HINTS_X86
    अचिन्हित mask = _MM_FLUSH_ZERO_MASK;
    अगर (featuresEnabled.have[CPU_SSE3])
    {
        mask |= _MM_DENORMALS_ZERO_MASK;
    }
    स्थिर अचिन्हित old_flags = _mm_getcsr();
    स्थिर अचिन्हित old_मूल्य = old_flags & mask;
    // save state
    state.reserved[0] = (uint32_t)mask;
    state.reserved[1] = (uint32_t)old_मूल्य;
    वापस 2;
#अन्यथा
    CV_UNUSED(state);
    वापस 0;
#पूर्ण_अगर
}

bool restoreFPDenormalsState(स्थिर FPDenormalsModeState& state)
{
#अगर OPENCV_IMPL_FP_HINTS_X86
    स्थिर अचिन्हित mask = (अचिन्हित)state.reserved[0];
    CV_DbgAssert(mask != 0); // invalid state (ensure that state is properly saved earlier)
    स्थिर अचिन्हित value = (अचिन्हित)state.reserved[1];
    CV_DbgCheck((पूर्णांक)value, value == (value & mask), "invalid SSE FP state");
    स्थिर अचिन्हित old_flags = _mm_getcsr();
    अचिन्हित flags = (old_flags & ~mask) | value;
    CV_LOG_DEBUG(शून्य, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    वापस true;
#अन्यथा
    CV_UNUSED(state);
    वापस false;
#पूर्ण_अगर
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#अगर_घोषित OPENCV_ALGO_HINT_DEFAULT
    वापस OPENCV_ALGO_HINT_DEFAULT;
#अन्यथा
    वापस ALGO_HINT_ACCURATE;
#पूर्ण_अगर
};

} // namespace cv

/* End of file. */
