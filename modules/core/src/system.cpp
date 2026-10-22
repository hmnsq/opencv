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

#اگر_تعریف_شده __QNX__
    #ضمن <unistd.h>
    #ضمن <sys/neutrino.h>
    #ضمن <sys/syspage.h>
#اگر_تعریف_شده __aarch64__
    #ضمن <aarch64/syspage.h>
#پایان_اگر
#پایان_اگر

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
#اگر_تعریف_شده __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ساكن std::ios_base::Init s_iostream_initializer;
#پایان_اگر
}

ساكن Mutex* __initialization_mutex = عدم;
Mutex& getInitializationMutex()
{
    إذا (__initialization_mutex == عدم)
    {
        (فراغ)_initSystem();
        __initialization_mutex = جديد Mutex();
    }
    أرجع *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ساكن منطقي param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#إذا defined(_DEBUG) || defined(__ANDROID__)
    درست
#وإلا
    خطأ
#پایان_اگر
);

فراغ* alloc_Singleton_میانگیر(size_t size) { أرجع fastMalloc(size); }
فراغ* alloc_Singleton_New_میانگیر(size_t size) { أرجع حافظه_بده(size); }


} // namespace cv

#اگر_تعریف_نشده CV_ERROR_مجموعه_TERMINATE_HANDLER  // build config option
# إذا defined(_WIN32)
#   define CV_ERROR_مجموعه_TERMINATE_HANDLER 1
# endif
#پایان_اگر
#إذا defined(CV_ERROR_مجموعه_TERMINATE_HANDLER) && !CV_ERROR_مجموعه_TERMINATE_HANDLER
# undef CV_ERROR_مجموعه_TERMINATE_HANDLER
#پایان_اگر

#اگر_تعریف_شده _MSC_VER
# إذا _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#پایان_اگر

#اگر_تعریف_شده CV_ERROR_مجموعه_TERMINATE_HANDLER
#ضمن <cstdlib>        // std::abort
#پایان_اگر

#إذا defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#إذا defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
استفاده Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#وإلا
#  include <elf.h>
#پایان_اگر
#إذا defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#پایان_اگر
#پایان_اگر

#إذا defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#پایان_اگر


#إذا ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#پایان_اگر

#إذا defined __loongarch64
#ضمن "sys/auxv.h"
#تعریف_کن LA_HWCAP_LSX   (1<<4)
#تعریف_کن LA_HWCAP_LASX  (1<<5)
#پایان_اگر

#إذا defined _WIN32 || defined WINCE
#اگر_تعریف_نشده _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #تعریف_کن _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#پایان_اگر
#ضمن <windows.h>
#ضمن <combaseapi.h>
#إذا (_WIN32_WINNT >= 0x0602)
  #ضمن <synchapi.h>
#پایان_اگر
#إذا ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ضمن <fibersapi.h>
  #تعریف_کن CV_USE_FLS
#پایان_اگر
#لغو_تعریف small
#لغو_تعریف min
#لغو_تعریف max
#لغو_تعریف قدر_مطلق
#ضمن <tchar.h>

#اگر_تعریف_شده WINRT
#ضمن <wrl/client.h>
#اگر_تعریف_نشده __cplusplus_winrt
#ضمن <windows.storage.h>
#رهنمود comment(lib, "runtimeobject.lib")
#پایان_اگر // WINRT

std::wstring Get_موقت_مسیر_Win_RT()
{
#اگر_تعریف_شده __cplusplus_winrt
    أرجع std::wstring(Windows::Storage::Application_داده::Current->TemporaryFolder->مسیر_2->داده_4());
#وإلا
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_داده_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_داده> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_قلم> storageitemRef;
    HSTRING str;
    HSTRING_سرآیند hstr_سر;
    std::wstring wstr;
    إذا (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_داده,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_داده), &hstr_سر, &str)))
        أرجع wstr;
    إذا (FAILED(RoGetActivationFactory(str, IID_PPV_آرگومانها(appdataFactory.Release_And_Get_آدرس_از()))))
        أرجع wstr;
    إذا (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_آدرس_از())))
        أرجع wstr;
    إذا (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_آدرس_از())))
        أرجع wstr;
    إذا (FAILED(storagefolderRef.As(&storageitemRef)))
        أرجع wstr;
    str = عدم;
    إذا (FAILED(storageitemRef->get_مسیر(&str)))
        أرجع wstr;
    wstr = Windows_Get_String_Raw_میانگیر(str, عدم);
    WindowsDeleteString(str);
    أرجع wstr;
#پایان_اگر
}

std::wstring Get_موقت_File_Name_Win_RT(std::wstring پیشوند)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* ماسک = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], حجم(guidStr)/حجم(wchar_t), ماسک,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    أرجع پیشوند.append(std::wstring(guidStr));
}

#پایان_اگر
#وإلا
#اگر_تعریف_نشده OPENCV_DISABLE_THREAD_SUPPORT
#ضمن <pthread.h>
#پایان_اگر
#ضمن <sys/time.h>
#ضمن <time.h>

#إذا defined __MACH__ && defined __APPLE__
#ضمن <mach/mach.h>
#ضمن <mach/mach_time.h>
#ضمن <sys/sysctl.h>
#پایان_اگر

#پایان_اگر

#اگر_تعریف_شده _OPENMP
#ضمن "omp.h"
#پایان_اگر

#إذا defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#إذا defined __ANDROID__
#ضمن <sys/sysconf.h>
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده __ANDROID__
# include <android/log.h>
#پایان_اگر

#اگر_تعریف_شده DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#پایان_اگر
#اگر_تعریف_نشده CV_CPUID_X86
  #إذا defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #إذا _MSC_VER >= 1400  // MSVS 2005
      #ضمن <intrin.h>  // __cpuidex()
      #تعریف_کن CV_CPUID_X86 __cpuidex
    #وإلا
      #خطا "Required MSVS 2005+"
    #پایان_اگر
  #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ساكن فراغ cv_cpuid(صحيح* cpuid_داده, صحيح reg_eax, صحيح reg_ecx)
    {
        صحيح __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#إذا !defined(__PIC__) \
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
#خطا "Configuration error"
#پایان_اگر
        cpuid_داده[0] = __eax; cpuid_داده[1] = __ebx; cpuid_داده[2] = __ecx; cpuid_داده[3] = __edx;
    }
    #تعریف_کن CV_CPUID_X86 cv_cpuid
  #پایان_اگر
#پایان_اگر

#ضمن <chrono>

نطاق cv
{

Exception::Exception() { کد = Error::StsOk; سطر = 0; }

Exception::Exception(Error::کد_3 کد_4, ثابت String& _err, ثابت String& _func, ثابت String& _file, صحيح سطر_3)
: کد(کد_4), err(_err), func(_func), file(_file), سطر(سطر_3)
{
    format_پیام();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ثابت حرف* Exception::what() ثابت CV_NOEXCEPT { أرجع پیام.c_str(); }

فراغ Exception::format_پیام()
{
    size_t pos = err.find('\n');
    منطقي multiline = pos != cv::String::npos;
    إذا (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        طالما (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::پایان_سطر;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        إذا (err[err.size() - 1] != '\n')
            ss << std::پایان_سطر;
        err = ss.str();
    }
    إذا (func.size() > 0)
    {
        إذا (multiline)
            پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_نسخه, file.c_str(), سطر, کد, کد_پیام(), func.c_str(), err.c_str());
        وإلا
            پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_نسخه, file.c_str(), سطر, کد, کد_پیام(), err.c_str(), func.c_str());
    }
    وإلا
    {
        پیام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_نسخه, file.c_str(), سطر, کد, کد_پیام(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت حرف * Exception::کد_پیام() ثابت
{
    بدل (کد)
    {
    حالة cv::Error::StsOk :                  أرجع "No Error";
    حالة cv::Error::Sts_پشت_Trace :           أرجع "Backtrace";
    حالة cv::Error::StsError :               أرجع "Unspecified error";
    حالة cv::Error::Sts_داخلی :            أرجع "Internal error";
    حالة cv::Error::StsNoMem :               أرجع "Insufficient memory";
    حالة cv::Error::StsBadArg :              أرجع "Bad argument";
    حالة cv::Error::StsNoConv :              أرجع "Iterations do not converge";
    حالة cv::Error::StsAutoTrace :           أرجع "Autotrace call";
    حالة cv::Error::Sts_Bad_اندازه :             أرجع "Incorrect size of input array";
    حالة cv::Error::StsNullPtr :             أرجع "Null pointer";
    حالة cv::Error::StsDivByZero :           أرجع "Division by zero occurred";
    حالة cv::Error::Bad_گام :                أرجع "Image step is wrong";
    حالة cv::Error::StsInplaceNotSupported : أرجع "Inplace operation is not supported";
    حالة cv::Error::Sts_Object_Not_یافت :      أرجع "Requested object was not found";
    حالة cv::Error::Bad_عمق :               أرجع "Input image depth is not supported by function";
    حالة cv::Error::StsUnmatchedFormats :    أرجع "Formats of input arguments do not match";
    حالة cv::Error::StsUnmatchedSizes :      أرجع "Sizes of input arguments do not match";
    حالة cv::Error::Sts_بیرون_از_Range :          أرجع "One of the arguments\' values is out of range";
    حالة cv::Error::StsUnsupportedFormat :   أرجع "Unsupported format or combination of formats";
    حالة cv::Error::BadCOI :                 أرجع "Input COI is not supported";
    حالة cv::Error::Bad_رقم_Channels :         أرجع "Bad number of channels";
    حالة cv::Error::StsBadFlag :             أرجع "Bad flag (parameter or structure field)";
    حالة cv::Error::Sts_Bad_نقطه :            أرجع "Bad parameter of type CvPoint";
    حالة cv::Error::Sts_Bad_ماسک :             أرجع "Bad type of mask argument";
    حالة cv::Error::StsParseError :          أرجع "Parsing error";
    حالة cv::Error::StsNotImplemented :      أرجع "The function/feature is not implemented";
    حالة cv::Error::Sts_Bad_Mem_بلوک :         أرجع "Memory block has been corrupted";
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
        snprintf(buf, حجم(buf), "Unknown %s code %d", کد >= 0 ? "status":"error", کد);
        أرجع buf;
    }
    };

}

ساكن ثابت حرف* g_hw_Feature_نامها[CV_HARDWARE_MAX_FEATURE] = { عدم };

ساكن ثابت حرف* getHWFeatureName(صحيح id)
{
    أرجع (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_نامها[id] : عدم;
}
ساكن ثابت حرف* get_HWFeature_Name_امن(صحيح id)
{
    ثابت حرف* name = getHWFeatureName(id);
    أرجع name ? name : "Unknown feature";
}

بنية HWFeatures
{
    تعداد { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقي پیاپی_initialize = خطأ)
    {
        إذا (پیاپی_initialize)
            initialize();
    }

    ساكن فراغ initialize_نامها()
    {
        لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_نامها[i] = 0;
        }
        g_hw_Feature_نامها[CPU_MMX] = "MMX";
        g_hw_Feature_نامها[CPU_SSE] = "SSE";
        g_hw_Feature_نامها[CPU_SSE2] = "SSE2";
        g_hw_Feature_نامها[CPU_SSE3] = "SSE3";
        g_hw_Feature_نامها[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_نامها[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_نامها[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_نامها[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_نامها[CPU_FP16] = "FP16";
        g_hw_Feature_نامها[CPU_AVX] = "AVX";
        g_hw_Feature_نامها[CPU_AVX2] = "AVX2";
        g_hw_Feature_نامها[CPU_FMA3] = "FMA3";

        g_hw_Feature_نامها[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_نامها[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_نامها[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_نامها[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_نامها[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_نامها[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_نامها[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_نامها[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_نامها[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_نامها[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_نامها[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_نامها[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_نامها[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_نامها[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_نامها[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_نامها[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_نامها[CPU_NEON] = "NEON";
        g_hw_Feature_نامها[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_نامها[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_نامها[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_نامها[CPU_SVE] = "SVE";

        g_hw_Feature_نامها[CPU_VSX] = "VSX";
        g_hw_Feature_نامها[CPU_VSX3] = "VSX3";

        g_hw_Feature_نامها[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_نامها[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_نامها[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_نامها[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_نامها[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_نامها[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_نامها[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_نامها[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_نامها[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_نامها[CPU_RVV] = "RVV";
        g_hw_Feature_نامها[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_نامها[CPU_LSX]  = "LSX";
        g_hw_Feature_نامها[CPU_LASX] = "LASX";
    }

    فراغ initialize(فراغ)
    {
        إذا (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_نامها();

    #اگر_تعریف_شده CV_CPUID_X86
        صحيح cpuid_داده[4] = { 0, 0, 0, 0 };
        صحيح cpuid_داده_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_داده, 1, 0/*استفادهنشده*/);

        صحيح x86_family = (cpuid_داده[0] >> 8) & 15;
        إذا( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_داده[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_داده[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_داده[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_داده[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_داده[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_داده[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_داده[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_داده[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_داده[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_داده[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_داده[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_داده_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_داده_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_داده_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_داده_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_داده_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_داده_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_داده_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_داده_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_داده_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_داده_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_داده_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_داده_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_داده_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_داده_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_داده_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_داده_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_داده_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            صحيح cpuid_داده_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_داده_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_داده_ex1[0] & (1<<4))  != 0;

            منطقي have_AVX_OS_support = درست;
            منطقي have_AVX512_OS_support = درست;
            إذا (!(cpuid_داده[2] & (1<<27)))
                have_AVX_OS_support = خطأ; // OS uses XSAVE_XRSTORE and CPU support AVX
            وإلا
            {
                صحيح xcr0 = 0;
            #اگر_تعریف_شده XCR_XFEATURE_ENABLED_ماسک // requires immintrin.h
                xcr0 = (صحيح)_xgetbv(XCR_XFEATURE_ENABLED_ماسک);
            #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #پایان_اگر
                إذا ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = خطأ; // YMM registers
                إذا ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = خطأ; // ZMM registers
            }

            إذا (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = خطأ;
                have[CV_CPU_FP16] = خطأ;
                have[CV_CPU_AVX2] = خطأ;
                have[CV_CPU_FMA3] = خطأ;
                have[CV_CPU_AVX_VNNI] = خطأ;
            }
            إذا (!have_AVX_OS_support || !have_AVX512_OS_support)
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
            إذا (have[CV_CPU_AVX512_COMMON])
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
    #پایان_اگر // CV_CPUID_X86

    #إذا defined __ANDROID__ || defined __linux__ || defined __QNX__
    #اگر_تعریف_شده __aarch64__
        have[CV_CPU_NEON] = درست;
        have[CV_CPU_FP16] = درست;
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        إذا (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t اندازه_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, اندازه_auxv_t) == اندازه_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                إذا (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#إذا defined(AT_HWCAP2)
                وإلا إذا (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#پایان_اگر
            }

            close(cpufile);
        }
    #وإلاإذا defined __arm__ && defined __ANDROID__
      #إذا defined HAVE_CPUFEATURES
        CV_LOG_INFO(عدم, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(عدم, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (دراز دراز)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #وإلا
        CV_LOG_INFO(عدم, "cpufeatures library is not available for CPU detection");
        #إذا CV_NEON
        CV_LOG_INFO(عدم, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = درست;
        #وإلا
        CV_LOG_INFO(عدم, "- NEON instructions is NOT enabled via build flags");
        #پایان_اگر
        #إذا CV_FP16
        CV_LOG_INFO(عدم, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = درست;
        #وإلا
        CV_LOG_INFO(عدم, "- FP16 instructions is NOT enabled via build flags");
        #پایان_اگر
      #پایان_اگر
    #وإلاإذا defined __arm__
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        إذا (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t اندازه_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, اندازه_auxv_t) == اندازه_auxv_t)
            {
                إذا (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    اكسر;
                }
            }

            close(cpufile);
        }
    #پایان_اگر
    #وإلاإذا (defined __APPLE__)
    #إذا defined __ARM_NEON
        have[CV_CPU_NEON] = درست;
    #پایان_اگر
    #إذا (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = درست;
    #پایان_اگر
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحيح has_feat_dotprod = 0;
    size_t has_feat_dotprod_اندازه = حجم(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_اندازه, عدم, 0);
    إذا (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = درست;
    }
    صحيح has_feat_bf16 = 0;
    size_t has_feat_bf16_اندازه = حجم(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_اندازه, عدم, 0);
    إذا (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = درست;
    }
    #وإلاإذا (defined __clang__)
    #إذا defined __ARM_NEON
        have[CV_CPU_NEON] = درست;
        #إذا (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = درست;
        #پایان_اگر
    #پایان_اگر
    #پایان_اگر
    #إذا defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = درست;
    #پایان_اگر
    #إذا defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = درست;
    #پایان_اگر
    #اگر_تعریف_شده __riscv_vector
        have[CV_CPU_RISCVV] = درست;
    #پایان_اگر
    #اگر_تعریف_شده __mips_msa
        have[CV_CPU_MSA] = درست;
    #پایان_اگر

    #إذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        بیعلامت صحيح hwcap = getauxval(AT_HWCAP);
        إذا (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            إذا (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = درست;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلاإذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        بیعلامت دراز hwcap = 0, hwcap2 = 0;
        إذا (elf_aux_info(AT_HWCAP, &hwcap, حجم(hwcap)) != 0)
            hwcap = 0;
        إذا (hwcap & PPC_FEATURE_HAS_VSX) {
            إذا (elf_aux_info(AT_HWCAP2, &hwcap2, حجم(hwcap2)) != 0)
                hwcap2 = 0;
            إذا (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = درست;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلا
        // TODO: AIX
        #إذا CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = درست;
        #پایان_اگر
        #إذا CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = درست;
        #پایان_اگر
    #پایان_اگر

    #إذا defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = درست;
        #إذا (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = درست;
        #پایان_اگر
        #إذا defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = درست;
        #پایان_اگر
    #پایان_اگر

    #إذا defined __loongarch64 && defined __linux__
        صحيح flag = (صحيح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #پایان_اگر

        منطقي skip_baseline_check = خطأ;
        إذا (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = درست;
        }
        صحيح baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        إذا (!checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]))
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
            checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]), درست);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]));
    }

    منطقي checkFeatures(ثابت صحيح* features, صحيح شمار_2, منطقي dump = خطأ)
    {
        منطقي نتیجه = درست;
        لكل (صحيح i = 0; i < شمار_2; i++)
        {
            صحيح feature = features[i];
            إذا (feature)
            {
                إذا (have[feature])
                {
                    إذا (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_امن(feature));
                }
                وإلا
                {
                    نتیجه = خطأ;
                    إذا (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_امن(feature));
                }
            }
        }
        أرجع نتیجه;
    }

    ساكن مضمن منطقي is_نماد_Separator(حرف c)
    {
        أرجع c == ',' || c == ';';
    }

    فراغ readSettings(ثابت صحيح* baseline_features, صحيح baseline_شمار)
    {
        منطقي dump = درست;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        إذا (!disabled_features.empty())
        {
            ثابت حرف* شروع = disabled_features.c_str();
            لكل (;;)
            {
                طالما (شروع[0] != 0 && is_نماد_Separator(شروع[0]))
                {
                    شروع++;
                }
                إذا (شروع[0] == 0)
                    اكسر;
                ثابت حرف* end = شروع;
                طالما (end[0] != 0 && !is_نماد_Separator(end[0]))
                {
                    end++;
                }
                إذا (end == شروع)
                    استمر;
                cv::String feature(شروع, end);
                شروع = end;

                CV_Assert(feature.size() > 0);

                منطقي یافت = خطأ;
                لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    إذا (!g_hw_Feature_نامها[i]) استمر;
                    size_t len = طول_رشته(g_hw_Feature_نامها[i]);
                    إذا (len != feature.size()) استمر;
                    إذا (feature.compare(g_hw_Feature_نامها[i]) == 0)
                    {
                        منطقي isBaseline = خطأ;
                        لكل (صحيح k = 0; k < baseline_شمار; k++)
                        {
                            إذا (baseline_features[k] == i)
                            {
                                isBaseline = درست;
                                اكسر;
                            }
                        }
                        إذا (isBaseline)
                        {
                            إذا (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_امن(i));
                        }
                        إذا (!have[i])
                        {
                            إذا (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_امن(i));
                        }
                        have[i] = خطأ;

                        یافت = درست;
                        اكسر;
                    }
                }
                إذا (!یافت)
                {
                    إذا (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقي have[MAX_FEATURE+1]{};
};

ساكن HWFeatures  featuresEnabled(درست), featuresDisabled = HWFeatures(خطأ);
ساكن HWFeatures* currentFeatures = &featuresEnabled;

منطقي checkHardwareSupport(صحيح feature)
{
    إذا (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        أرجع خطأ;
    أرجع currentFeatures->have[feature];
}

String getHardwareFeatureName(صحيح feature)
{
    ثابت حرف* name = getHWFeatureName(feature);
    أرجع name ? String(name) : String();
}

std::string get_CPUFeatures_سطر()
{
    ثابت صحيح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحيح sz = حجم(features) / حجم(features[0]);
    std::string نتیجه;
    std::string پیشوند;
    لكل (صحيح i = 1; i < sz; ++i)
    {
        إذا (features[i] == 0)
        {
            پیشوند = "*";
            استمر;
        }
        إذا (i != 1) نتیجه.append(" ");
        نتیجه.append(پیشوند);
        نتیجه.append(get_HWFeature_Name_امن(features[i]));
        إذا (!checkHardwareSupport(features[i])) نتیجه.append("?");
    }
    أرجع نتیجه;
}

متطاير منطقي useOptimizedFlag = درست;

فراغ مجموعه_Use_Optimized( منطقي flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::مجموعه_Use_IPP(flag);
#اگر_تعریف_شده HAVE_OPENCL
    ocl::مجموعه_Use_Open_CL(flag);
#پایان_اگر
}

منطقي useOptimized(فراغ)
{
    أرجع useOptimizedFlag;
}

int64 get_Tick_شمار(فراغ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    أرجع (int64)now.time_since_epoch().شمار_2();
}

مزدوج getTickFrequency(فراغ)
{
    استفاده clock_period_t = std::chrono::steady_clock::duration::period;
    مزدوج clock_freq = clock_period_t::den / clock_period_t::num;
    أرجع clock_freq;
}

#إذا defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#إذا defined(__i386__)

int64 get_CPUTick_شمار(فراغ)
{
    int64 x;
    __asm__ متطاير (".byte 0x0f, 0x31" : "=A" (x));
    أرجع x;
}
#وإلاإذا defined(__x86_64__)

int64 get_CPUTick_شمار(فراغ)
{
    بیعلامت hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    أرجع (int64)lo | ((int64)hi << 32);
}

#وإلاإذا defined(__ppc__)

int64 get_CPUTick_شمار(فراغ)
{
    بیعلامت upper, lower, tmp;
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

#خطا "RDTSC not defined"

#پایان_اگر

#وإلاإذا defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_شمار(فراغ)
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
int64 get_CPUTick_شمار(فراغ)
{
    أرجع get_Tick_شمار();
}
//#endif

#پایان_اگر


نطاق internal {

صنف Timestamp
{
عام:
    ثابت int64 zero_Tick_شمار;
    ثابت مزدوج ns_درون_ticks;

    Timestamp()
        : zero_Tick_شمار(get_Tick_شمار())
        , ns_درون_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_شمار();
        أرجع (int64)((t - zero_Tick_شمار) * ns_درون_ticks);
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

String get_نسخه_String() { أرجع String(CV_نسخه); }

صحيح get_نسخه_Major() { أرجع CV_نسخه_MAJOR; }

صحيح get_نسخه_Minor() { أرجع CV_نسخه_MINOR; }

صحيح get_نسخه_Revision() { أرجع CV_نسخه_REVISION; }

String format( ثابت حرف* fmt, ... )
{
    Auto_میانگیر<حرف, 1024> buf;

    لكل ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحيح bsize = static_cast<صحيح>(buf.size());
        صحيح len = cv_vsnprintf(buf.داده(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        إذا (len >= bsize)
        {
            buf.resize(len + 1);
            استمر;
        }
        buf[bsize - 1] = 0;
        أرجع String(buf.داده(), len);
    }
}

String tempfile( ثابت حرف* suffix )
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string موقت_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#إذا defined _WIN32
#اگر_تعریف_شده WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring موقت_dir_rt = Get_موقت_مسیر_Win_RT();

    std::wstring موقت_file = Get_موقت_File_Name_Win_RT(L"ocv");
    إذا (موقت_file.empty())
        أرجع String();

    موقت_file = موقت_dir_rt.append(std::wstring(L"\\")).append(موقت_file);
    DeleteFileW(موقت_file.c_str());

    حرف aname[MAX_مسیر];
    size_t copied = wcstombs(aname, موقت_file.c_str(), MAX_مسیر);
    CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#وإلاإذا defined(_WIN32_WCE)
    ثابت خودکار k_Max_مسیر_اندازه = MAX_مسیر+1;
    wchar_t موقت_dir_ce[k_Max_مسیر_اندازه] = {0};
    wchar_t موقت_file[k_Max_مسیر_اندازه] = {0};

    ::Get_موقت_مسیر_W(k_Max_مسیر_اندازه, موقت_dir_ce);

    إذا(0 != ::Get_موقت_File_Name_W(موقت_dir_ce, L"ocv", 0, موقت_file)) {
        DeleteFileW(موقت_file);
        حرف aname[MAX_مسیر];
        size_t copied = wcstombs(aname, موقت_file, MAX_مسیر);
        CV_Assert((copied != MAX_مسیر) && (copied != (size_t)-1));
        fname = String(aname);
    }
#وإلا
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t موقت_dir2_w[MAX_مسیر] = { 0 };

    إذا (موقت_dir.empty())
    {
        ::Get_موقت_مسیر_W(حجم(موقت_dir2_w)/حجم(wchar_t), موقت_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحيح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, موقت_dir2_w, -1, عدم, 0, عدم, عدم);
        إذا (len > 0)
        {
            std::vector<حرف> utf8_buf(len);
            Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, موقت_dir2_w, -1, utf8_buf.داده(), len, عدم, عدم);
            موقت_dir = std::string(utf8_buf.داده());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    إذا (FAILED(hr))
        أرجع String();
    حرف guidStr[40];
    ثابت حرف* ماسک = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, حجم(guidStr), ماسک,
            g.Data1, g.Data2, g.Data3, (بیعلامت صحيح)g.Data4[0], (بیعلامت صحيح)g.Data4[1],
            (بیعلامت صحيح)g.Data4[2], (بیعلامت صحيح)g.Data4[3], (بیعلامت صحيح)g.Data4[4],
            (بیعلامت صحيح)g.Data4[5], (بیعلامت صحيح)g.Data4[6], (بیعلامت صحيح)g.Data4[7]);

    fname = موقت_dir;
    إذا (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#پایان_اگر
# وإلا
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    حرف defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  وإلا
    حرف defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    إذا (موقت_dir.empty())
        fname = defaultTemplate;
    وإلا
    {
        fname = موقت_dir;
        حرف ech = fname[fname.size() - 1];
        إذا(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ثابت صحيح fd = mkstemp((حرف*)fname.c_str());
    إذا (fd == -1) أرجع String();

    close(fd);
    remove(fname.c_str());
# endif

    إذا (suffix)
    {
        إذا (suffix[0] != '.')
            أرجع fname + "." + suffix;
        وإلا
            أرجع fname + suffix;
    }
    أرجع fname;
#وإلا // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_استفادهنشده(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#پایان_اگر // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ساكن ErrorCallback customErrorCallback = 0;
ساكن فراغ* custom_Error_Callback_داده = 0;
ساكن منطقي breakOnError = خطأ;

منطقي مجموعه_Break_On_Error(منطقي value)
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

صحيح cv_vsnprintf(حرف* buf, صحيح len, ثابت حرف* fmt, va_list آرگومانها)
{
#إذا defined _MSC_VER
    إذا (len <= 0) أرجع len == 0 ? 1024 : -1;
    صحيح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, آرگومانها);
    // ensure null terminating on VS
    إذا (res >= 0 && res < len)
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
    أرجع vsnprintf(buf, len, fmt, آرگومانها);
#پایان_اگر
}

ساكن فراغ dumpException(ثابت Exception& exc)
{
    ثابت حرف* errorStr = exc.کد_پیام();
    حرف buf[1 << 12];

    cv_snprintf(buf, حجم(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_نسخه,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.سطر);
#اگر_تعریف_شده __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#وإلا
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#پایان_اگر
}

#اگر_تعریف_شده CV_ERROR_مجموعه_TERMINATE_HANDLER
ساكن منطقي cv_terminate_handler_installed = خطأ;
ساكن std::terminate_handler cv_old_terminate_handler;
ساكن cv::Exception cv_terminate_handler_exception;
ساكن منطقي param_راهاندازی_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", درست);
ساكن فراغ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    إذا (خطأ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#پایان_اگر

#اگر_تعریف_شده __GNUC__
# إذا defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#پایان_اگر

فراغ error( ثابت Exception& exc )
{
#اگر_تعریف_شده CV_ERROR_مجموعه_TERMINATE_HANDLER
    {
        cv::Auto_قفل lock(getInitializationMutex());
        إذا (!cv_terminate_handler_installed)
        {
            إذا (param_راهاندازی_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = درست;
        }
        cv_terminate_handler_exception = exc;
    }
#پایان_اگر

    إذا (customErrorCallback != 0)
        customErrorCallback(exc.کد, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.سطر, custom_Error_Callback_داده);
    وإلا إذا (param_dumpErrors)
    {
        dumpException(exc);
    }

    إذا(breakOnError)
    {
        std::terminate();
    }

    ارم exc;
#اگر_تعریف_شده __GNUC__
# إذا !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#پایان_اگر
}

فراغ error(Error::کد_3 کد_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح سطر_3)
{
    error(cv::Exception(کد_4, _err, _func, _file, سطر_3));
#اگر_تعریف_شده __GNUC__
# إذا !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#پایان_اگر
}

#اگر_تعریف_شده __GNUC__
# إذا defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#پایان_اگر


ErrorCallback
redirectError( ErrorCallback errCallback, فراغ* userdata, فراغ** prevUserdata)
{
    إذا( prevUserdata )
        *prevUserdata = custom_Error_Callback_داده;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_داده = userdata;

    أرجع prevCallback;
}

فراغ terminate(Error::کد_3 کد_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح سطر_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(کد_4, _err, _func, _file, سطر_3));
    std::terminate();
}

}

نطاق cv {
منطقي __termination = خطأ;


//////////////////////////////// thread-local storage ////////////////////////////////

نطاق details {

#اگر_تعریف_نشده OPENCV_DISABLE_THREAD_SUPPORT

#اگر_تعریف_شده _WIN32
#اگر_تعریف_شده _MSC_VER
#رهنمود warning(disable:4505) // unreferenced local function has been removed
#پایان_اگر
#اگر_تعریف_نشده TLS_بیرون_از_INDEXES
#تعریف_کن TLS_بیرون_از_INDEXES ((DWORD)0xFFFFFFFF)
#پایان_اگر
#پایان_اگر

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

    فراغ* get_داده() ثابت;
    فراغ مجموعه_داده(فراغ *p_داده);

    فراغ releaseSystemResources();

خاص:

#اگر_تعریف_شده _WIN32
#اگر_تعریف_نشده WINRT
    DWORD tls_کلید;
    منطقي disposed;
#پایان_اگر
#وإلا // _WIN32
    pthread_key_t  tls_کلید;
    std::atomic<منطقي> disposed;
#پایان_اگر
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


#اگر_تعریف_شده _WIN32
#اگر_تعریف_شده WINRT
ساكن __declspec( thread ) فراغ* tls_داده = عدم; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
}
فراغ* TlsAbstraction::get_داده() ثابت
{
    أرجع tls_داده;
}
فراغ TlsAbstraction::مجموعه_داده(فراغ *p_داده)
{
    tls_داده = p_داده;
}
#وإلا //WINRT
#اگر_تعریف_شده CV_USE_FLS
ساكن فراغ NTAPI opencv_fls_destructor(فراغ* p_داده);
#پایان_اگر // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
#اگر_تعریف_نشده CV_USE_FLS
    tls_کلید = TlsAlloc();
#وإلا // CV_USE_FLS
    tls_کلید = FlsAlloc(opencv_fls_destructor);
#پایان_اگر // CV_USE_FLS
    CV_Assert(tls_کلید != TLS_بیرون_از_INDEXES);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
    disposed = درست;
#اگر_تعریف_نشده CV_USE_FLS
    TlsFree(tls_کلید);
#وإلا // CV_USE_FLS
    FlsFree(tls_کلید);
#پایان_اگر // CV_USE_FLS
    tls_کلید = TLS_بیرون_از_INDEXES;
}
فراغ* TlsAbstraction::get_داده() ثابت
{
    إذا (disposed)
        أرجع عدم;
#اگر_تعریف_نشده CV_USE_FLS
    أرجع Tls_Get_مقدار(tls_کلید);
#وإلا // CV_USE_FLS
    أرجع Fls_Get_مقدار(tls_کلید);
#پایان_اگر // CV_USE_FLS
}
فراغ TlsAbstraction::مجموعه_داده(فراغ *p_داده)
{
    إذا (disposed)
        أرجع;  // no-op
#اگر_تعریف_نشده CV_USE_FLS
    CV_Assert(Tls_مجموعه_مقدار(tls_کلید, p_داده) == TRUE);
#وإلا // CV_USE_FLS
    CV_Assert(Fls_مجموعه_مقدار(tls_کلید, p_داده) == TRUE);
#پایان_اگر // CV_USE_FLS
}
#پایان_اگر // WINRT
#وإلا // _WIN32
ساكن فراغ opencv_tls_destructor(فراغ* p_داده);
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
    CV_Assert(pthread_key_create(&tls_کلید, opencv_tls_destructor) == 0);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = درست;  // DllMain is missing in static builds
    disposed = درست;
    إذا (pthread_key_delete(tls_کلید) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
فراغ* TlsAbstraction::get_داده() ثابت
{
    إذا (disposed)
        أرجع عدم;
    أرجع pthread_getspecific(tls_کلید);
}
فراغ TlsAbstraction::مجموعه_داده(فراغ *p_داده)
{
    إذا (disposed)
        أرجع;  // no-op
    CV_Assert(pthread_setspecific(tls_کلید, p_داده) == 0);
}
#پایان_اگر

// Per-thread data structure
بنية Thread_داده
{
    Thread_داده()
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
        tls_Slots_اندازه(0)
    {
        (فراغ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        نخها.reserve(32);
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

    فراغ releaseThread(فراغ* tls_مقدار = عدم)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)
        Thread_داده *pTD = tls_مقدار == عدم ? (Thread_داده*)tls->get_داده() : (Thread_داده*)tls_مقدار;
        إذا (pTD == عدم)
            أرجع;  // no OpenCV TLS data for this thread
        Auto_قفل guard(mtxGlobalAccess);
        لكل (size_t i = 0; i < نخها.size(); i++)
        {
            إذا (pTD == نخها[i])
            {
                نخها[i] = عدم;
                إذا (tls_مقدار == عدم)
                    tls->مجموعه_داده(0);
                std::vector<فراغ*>& thread_slots = pTD->slots;
                لكل (size_t جایگاه_Idx = 0; جایگاه_Idx < thread_slots.size(); جایگاه_Idx++)
                {
                    فراغ* p_داده = thread_slots[جایگاه_Idx];
                    thread_slots[جایگاه_Idx] = عدم;
                    إذا (!p_داده)
                        استمر;
                    TLSDataContainer* container = tlsSlots[جایگاه_Idx].container;
                    إذا (container)
                        container->delete_داده_Instance(p_داده);
                    وإلا
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحيح)جایگاه_Idx);
                        fflush(stderr);
                    }
                }
                حذف pTD;
                أرجع;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (فراغ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_جایگاه(TLSDataContainer* container)
    {
        Auto_قفل guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_اندازه == tlsSlots.size());

        // Find unused slots
        لكل(size_t جایگاه = 0; جایگاه < tls_Slots_اندازه; جایگاه++)
        {
            إذا (tlsSlots[جایگاه].container == عدم)
            {
                tlsSlots[جایگاه].container = container;
                أرجع جایگاه;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_جایگاه_Info(container)); tls_Slots_اندازه++;
        أرجع tls_Slots_اندازه - 1;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ release_جایگاه(size_t جایگاه_Idx, std::vector<فراغ*> &داده_Vec, منطقي keep_جایگاه = خطأ)
    {
        Auto_قفل guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_اندازه == tlsSlots.size());
        CV_Assert(tls_Slots_اندازه > جایگاه_Idx);

        لكل(size_t i = 0; i < نخها.size(); i++)
        {
            إذا(نخها[i])
            {
                std::vector<فراغ*>& thread_slots = نخها[i]->slots;
                إذا (thread_slots.size() > جایگاه_Idx && thread_slots[جایگاه_Idx])
                {
                    داده_Vec.push_back(thread_slots[جایگاه_Idx]);
                    thread_slots[جایگاه_Idx] = عدم;
                }
            }
        }

        إذا (!keep_جایگاه)
        {
            tlsSlots[جایگاه_Idx].container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* get_داده(size_t جایگاه_Idx) ثابت
    {
#اگر_تعریف_نشده CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_اندازه > جایگاه_Idx);
#پایان_اگر

        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع عدم;  // TLS singleton is not available (terminated)

        Thread_داده* thread_داده = (Thread_داده*)tls->get_داده();
        إذا(thread_داده && thread_داده->slots.size() > جایگاه_Idx)
            أرجع thread_داده->slots[جایگاه_Idx];

        أرجع عدم;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t جایگاه_Idx, std::vector<فراغ*> &داده_Vec)
    {
        Auto_قفل guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_اندازه == tlsSlots.size());
        CV_Assert(tls_Slots_اندازه > جایگاه_Idx);

        لكل(size_t i = 0; i < نخها.size(); i++)
        {
            إذا(نخها[i])
            {
                std::vector<فراغ*>& thread_slots = نخها[i]->slots;
                إذا (thread_slots.size() > جایگاه_Idx && thread_slots[جایگاه_Idx])
                    داده_Vec.push_back(thread_slots[جایگاه_Idx]);
            }
        }
    }

    // Set data to storage index
    فراغ مجموعه_داده(size_t جایگاه_Idx, فراغ* p_داده)
    {
#اگر_تعریف_نشده CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_اندازه > جایگاه_Idx);
#پایان_اگر

        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)

        Thread_داده* thread_داده = (Thread_داده*)tls->get_داده();
        إذا(!thread_داده)
        {
            thread_داده = جديد Thread_داده;
            tls->مجموعه_داده((فراغ*)thread_داده);
            {
                Auto_قفل guard(mtxGlobalAccess);

                منطقي یافت = خطأ;
                // Find unused slots
                لكل(size_t جایگاه = 0; جایگاه < نخها.size(); جایگاه++)
                {
                    إذا (نخها[جایگاه] == عدم)
                    {
                        thread_داده->idx = (صحيح)جایگاه;
                        نخها[جایگاه] = thread_داده;
                        یافت = درست;
                        اكسر;
                    }
                }

                إذا (!یافت)
                {
                    // Create new slot
                    thread_داده->idx = نخها.size();
                    نخها.push_back(thread_داده);
                }
            }
        }

        إذا(جایگاه_Idx >= thread_داده->slots.size())
        {
            Auto_قفل guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_داده->slots.resize(جایگاه_Idx + 1, عدم);
        }
        thread_داده->slots[جایگاه_Idx] = p_داده;
    }

خاص:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_اندازه;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    بنية Tls_جایگاه_Info
    {
        Tls_جایگاه_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<بنية Tls_جایگاه_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_داده*> نخها; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ساكن TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, جديد TlsStorage())
}

#اگر_تعریف_نشده _WIN32  // pthread key destructor
ساكن فراغ opencv_tls_destructor(فراغ* p_داده)
{
    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_داده);
}
#وإلا // _WIN32
#اگر_تعریف_شده CV_USE_FLS
ساكن فراغ WINAPI opencv_fls_destructor(فراغ* p_داده)
{
    // Empiric detection of ExitProcess call
    DWORD کد = STILL_فعال/*259*/;
    BOOL res = Get_Exit_کد_Process(GetCurrentProcess(), &کد);
    إذا (res && کد != STILL_فعال)
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

    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_داده);
}
#پایان_اگر // CV_USE_FLS
#پایان_اگر // _WIN32

ساكن TlsStorage* ثابت g_force_initialization_از_Tls_Storage
#إذا defined __GNUC__
    __attribute__((استفادهنشده))
#پایان_اگر
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
        لكل (size_t جایگاه_Idx = 0; جایگاه_Idx < slots.size(); جایگاه_Idx++)
        {
            جایگاه_Info& s = slots[جایگاه_Idx];
            TLSDataContainer* container = s.container;
            إذا (container && s.داده)
            {
                container->delete_داده_Instance(s.داده);  // Can't use from SlotInfo destructor
                s.داده = اشاره_تهی;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_جایگاه(TLSDataContainer* container)
    {
        size_t slots_اندازه = slots.size();
        لكل (size_t جایگاه = 0; جایگاه < slots_اندازه; جایگاه++)
        {
            جایگاه_Info& s = slots[جایگاه];
            إذا (s.container == عدم)
            {
                CV_Assert(!s.داده);
                s.container = container;
                أرجع جایگاه;
            }
        }

        // create new slot
        slots.push_back(جایگاه_Info(container));
        أرجع slots_اندازه;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ release_جایگاه(size_t جایگاه_Idx, std::vector<فراغ*> &داده_Vec, منطقي keep_جایگاه = خطأ)
    {
        CV_Assert(جایگاه_Idx < slots.size());
        جایگاه_Info& s = slots[جایگاه_Idx];
        فراغ* داده = s.داده;
        إذا (داده)
        {
            داده_Vec.push_back(داده);
            s.داده = اشاره_تهی;
        }
        إذا (!keep_جایگاه)
        {
            s.container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* get_داده(size_t جایگاه_Idx) ثابت
    {
        CV_Assert(جایگاه_Idx < slots.size());
        ثابت جایگاه_Info& s = slots[جایگاه_Idx];
        أرجع s.داده;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t جایگاه_Idx, std::vector<فراغ*> &داده_Vec)
    {
        CV_Assert(جایگاه_Idx < slots.size());
        جایگاه_Info& s = slots[جایگاه_Idx];
        فراغ* داده = s.داده;
        إذا (داده)
            داده_Vec.push_back(داده);
        أرجع;
    }

    // Set data to storage index
    فراغ مجموعه_داده(size_t جایگاه_Idx, فراغ* p_داده)
    {
        CV_Assert(جایگاه_Idx < slots.size());
        جایگاه_Info& s = slots[جایگاه_Idx];
        s.داده = p_داده;
    }

خاص:
    بنية جایگاه_Info
    {
        جایگاه_Info(TLSDataContainer* _container) : container(_container), داده(اشاره_تهی) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        فراغ* داده;
    };
    std::vector<بنية جایگاه_Info> slots;
};

ساكن TlsStorage& getTlsStorage()
{
    ساكن TlsStorage g_storage;  // no threading
    أرجع g_storage;
}

#پایان_اگر  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
استفاده نطاق details;

فراغ releaseTlsStorageThread()
{
#اگر_تعریف_نشده OPENCV_DISABLE_THREAD_SUPPORT
    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#پایان_اگر
}

TLSDataContainer::TLSDataContainer()
{
    کلید_2 = (صحيح)getTlsStorage().reserve_جایگاه(هذا); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(کلید_2 == -1); // Key must be released in child object
}

فراغ TLSDataContainer::gather_داده(std::vector<فراغ*> &داده) ثابت
{
    getTlsStorage().gather(کلید_2, داده);
}

فراغ TLSDataContainer::detach_داده(std::vector<فراغ*> &داده)
{
    getTlsStorage().release_جایگاه(کلید_2, داده, درست);
}

فراغ TLSDataContainer::release()
{
    إذا (کلید_2 == -1)
        أرجع;  // already released
    std::vector<فراغ*> داده; داده.reserve(32);
    getTlsStorage().release_جایگاه(کلید_2, داده, خطأ); // Release key and get stored data for proper destruction
    کلید_2 = -1;
    لكل(size_t i = 0; i < داده.size(); i++)  // Delete all associated data
        delete_داده_Instance(داده[i]);
}

فراغ TLSDataContainer::cleanup()
{
    std::vector<فراغ*> داده; داده.reserve(32);
    getTlsStorage().release_جایگاه(کلید_2, داده, درست); // Extract stored data with removal from TLS tables
    لكل(size_t i = 0; i < داده.size(); i++)  // Delete all associated data
        delete_داده_Instance(داده[i]);
}

فراغ* TLSDataContainer::get_داده() ثابت
{
    CV_Assert(کلید_2 != -1 && "Can't fetch data from terminated TLS container.");
    فراغ* p_داده = getTlsStorage().get_داده(کلید_2); // Check if data was already allocated
    إذا(!p_داده)
    {
        // Create new data instance and save it to TLS storage
        p_داده = create_داده_Instance();
        حاول
        {
            getTlsStorage().مجموعه_داده(کلید_2, p_داده);
        }
        امسك (...)
        {
            delete_داده_Instance(p_داده);
            ارم;
        }
    }
    أرجع p_داده;
}

ساكن TLSData<CoreTLSData>& get_Core_Tls_داده_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, جديد TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_داده()
{
    أرجع get_Core_Tls_داده_TLS().getRef();
}

#إذا defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#اگر_تعریف_شده WINRT
    #رهنمود warning(disable:4447) // Disable warning 'main' signature found without threading model
#پایان_اگر

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    إذا (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        إذا (lpReserved != عدم) // called after ExitProcess() call
        {
            cv::__termination = درست;
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
#پایان_اگر


نطاق {

#اگر_تعریف_شده OPENCV_WITH_ITT
منطقي overrideThreadName()
{
    ساكن منطقي param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", خطأ);
    أرجع param;
}
#پایان_اگر

ساكن صحيح g_thread_رقم = 0;
صنف ThreadID {
عام:
    ثابت صحيح id;
    ThreadID() :
        id(CV_XADD(&g_thread_رقم, 1))
    {
#اگر_تعریف_شده OPENCV_WITH_ITT
        إذا (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#پایان_اگر
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
    std::string bad_مقدار;
عام:
    ParseError(ثابت std::string &bad_مقدار_2) :bad_مقدار(bad_مقدار_2) {}
    std::string مقصد_String(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_مقدار;
        أرجع out.str();
    }
};

قالب <نام_نوع T>
T parseOption(ثابت std::string &);

قالب<>
مضمن منطقي parseOption(ثابت std::string & value)
{
    إذا (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        أرجع درست;
    }
    إذا (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        أرجع خطأ;
    }
    ارم ParseError(value);
}

قالب<>
مضمن size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    لكل (; pos < value.size(); pos++)
    {
        إذا (!آیا_رقم(value[pos]))
            اكسر;
    }
    cv::String مقدار_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(مقدار_Str);
    إذا (suffixStr.length() == 0)
        أرجع v;
    وإلا إذا (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        أرجع v * 1024 * 1024;
    وإلا إذا (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        أرجع v * 1024;
    ارم ParseError(value);
}

قالب<>
مضمن cv::String parseOption(ثابت std::string &value)
{
    أرجع value;
}

قالب<>
مضمن utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths نتیجه;
#اگر_تعریف_شده _WIN32
    ثابت حرف sep = ';';
#وإلا
    ثابت حرف sep = ':';
#پایان_اگر
    size_t شروع_pos = 0;
    طالما (شروع_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, شروع_pos);
        ثابت std::string one_piece(value, شروع_pos, pos == std::string::npos ? pos : pos - شروع_pos);
        إذا (!one_piece.empty())
            نتیجه.push_back(one_piece);
        شروع_pos = pos == std::string::npos ? pos : pos + 1;
    }
    أرجع نتیجه;
}

ساكن مضمن ثابت حرف * envRead(ثابت حرف * name)
{
#اگر_تعریف_شده NO_GETENV
    CV_استفادهنشده(name);
    أرجع عدم;
#وإلا
    أرجع getenv(name);
#پایان_اگر
}

قالب<نام_نوع T>
مضمن T اقرأ(ثابت std::string & k, ثابت T & default_مقدار_2)
{
    حاول
    {
        ثابت حرف * res = envRead(k.c_str());
        إذا (res)
            أرجع parseOption<T>(std::string(res));
    }
    امسك (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.مقصد_String(k));
    }
    أرجع default_مقدار_2;
}

منطقي utils::getConfigurationParameterBool(ثابت حرف* name, منطقي default_مقدار_2)
{
    أرجع اقرأ<منطقي>(name, default_مقدار_2);
}

size_t utils::get_Configuration_Parameter_اندازه_T(ثابت حرف* name, size_t default_مقدار_2)
{
    أرجع اقرأ<size_t>(name, default_مقدار_2);
}

std::string utils::getConfigurationParameterString(ثابت حرف* name, ثابت std::string & default_مقدار_2)
{
    أرجع اقرأ<cv::String>(name, default_مقدار_2);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت حرف* name, ثابت utils::Paths &default_مقدار_2)
{
    أرجع اقرأ<utils::Paths>(name, default_مقدار_2);
}


#اگر_تعریف_شده CV_COLLECT_IMPL_داده
ImplCollector& get_Impl_داده()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, جديد ImplCollector())
}

فراغ مجموعه_Impl(صحيح flags)
{
    cv::Auto_قفل lock(get_Impl_داده().mutex);

    get_Impl_داده().implFlags = flags;
    get_Impl_داده().impl_کد.clear();
    get_Impl_داده().implFun.clear();
}

فراغ جمع_کن_Impl(صحيح flag, ثابت حرف* func)
{
    cv::Auto_قفل lock(get_Impl_داده().mutex);

    get_Impl_داده().implFlags |= flag;
    إذا(func) // use lazy collection if name was not specified
    {
        size_t شاخص = get_Impl_داده().impl_کد.size();
        إذا(!شاخص || (get_Impl_داده().impl_کد[شاخص-1] != flag || get_Impl_داده().implFun[شاخص-1].compare(func))) // avoid duplicates
        {
            get_Impl_داده().impl_کد.push_back(flag);
            get_Impl_داده().implFun.push_back(func);
        }
    }
}

صحيح getImpl(std::vector<صحيح> &impl, std::vector<String> &funName)
{
    cv::Auto_قفل lock(get_Impl_داده().mutex);

    impl    = get_Impl_داده().impl_کد;
    funName = get_Impl_داده().implFun;
    أرجع get_Impl_داده().implFlags; // return actual flags for lazy collection
}

منطقي useCollection()
{
    أرجع get_Impl_داده().useCollection;
}

فراغ مجموعه_Use_Collection(منطقي flag)
{
    cv::Auto_قفل lock(get_Impl_داده().mutex);

    get_Impl_داده().useCollection = flag;
}
#پایان_اگر

نطاق instr
{
منطقي useInstrumentation()
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    أرجع getInstrumentStruct().useInstr;
#وإلا
    أرجع خطأ;
#پایان_اگر
}

فراغ مجموعه_Use_Instrumentation(منطقي flag)
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#وإلا
    CV_استفادهنشده(flag);
#پایان_اگر
}

Instr_گره* getTrace()
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    أرجع &getInstrumentStruct().ریشه_گره_2;
#وإلا
    أرجع عدم;
#پایان_اگر
}

فراغ resetTrace()
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    getInstrumentStruct().ریشه_گره_2.removeChilds();
    getInstrumentTLSStruct().p_Current_گره = &getInstrumentStruct().ریشه_گره_2;
#پایان_اگر
}

فراغ مجموعه_Flags(FLAGS نما_Flags)
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = نما_Flags;
#وإلا
    CV_استفادهنشده(نما_Flags);
#پایان_اگر
}
FLAGS getFlags()
{
#اگر_تعریف_شده ENABLE_INSTRUMENTATION
    أرجع (FLAGS)getInstrumentStruct().flags;
#وإلا
    أرجع (FLAGS)0;
#پایان_اگر
}

گره_داده::گره_داده(ثابت حرف* funName, ثابت حرف* fileName, صحيح سطر_رقم, فراغ* ret_آدرس, منطقي alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_سطر_رقم       = سطر_رقم;
    m_ret_آدرس    = ret_آدرس;
    m_alwaysExpand  = alwaysExpand;

    m_نخها    = 1;
    m_counter    = 0;
    m_ticks_مجموع = 0;

    m_funError  = خطأ;
}
گره_داده::گره_داده(گره_داده &ref)
{
    *هذا = ref;
}
گره_داده& گره_داده::عملگر=(ثابت گره_داده &راست)
{
    هذا->m_funName      = راست.m_funName;
    هذا->m_instrType    = راست.m_instrType;
    هذا->m_implType     = راست.m_implType;
    هذا->m_fileName     = راست.m_fileName;
    هذا->m_سطر_رقم      = راست.m_سطر_رقم;
    هذا->m_ret_آدرس   = راست.m_ret_آدرس;
    هذا->m_alwaysExpand = راست.m_alwaysExpand;

    هذا->m_نخها     = راست.m_نخها;
    هذا->m_counter     = راست.m_counter;
    هذا->m_ticks_مجموع  = راست.m_ticks_مجموع;

    هذا->m_funError    = راست.m_funError;

    أرجع *هذا;
}
گره_داده::~گره_داده()
{
}
منطقي عملگر==(ثابت گره_داده& چپ, ثابت گره_داده& راست)
{
    إذا(چپ.m_سطر_رقم == راست.m_سطر_رقم && چپ.m_funName == راست.m_funName && چپ.m_fileName == راست.m_fileName)
    {
        إذا(چپ.m_ret_آدرس == راست.m_ret_آدرس || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_نامها || چپ.m_alwaysExpand))
            أرجع درست;
    }
    أرجع خطأ;
}

#اگر_تعریف_شده ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ساكن InstrStruct instr;
    أرجع instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    أرجع *getInstrumentStruct().tlsStruct.get();
}

Instr_گره* get_Current_گره()
{
    أرجع getInstrumentTLSStruct().p_Current_گره;
}

IntrumentationRegion::IntrumentationRegion(ثابت حرف* funName, ثابت حرف* fileName, صحيح سطر_رقم, فراغ *ret_آدرس, منطقي alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = خطأ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    إذا(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        إذا(!pTLS->p_Current_گره)
        {
            m_disabled = درست;
            أرجع;
        }

        صحيح عمق = pTLS->p_Current_گره->get_عمق();
        إذا(pStruct->max_عمق && pStruct->max_عمق <= عمق)
        {
            m_disabled = درست;
            أرجع;
        }

        گره_داده payload(funName, fileName, سطر_رقم, ret_آدرس, alwaysExpand, instrType, implType);
        Node<گره_داده>* p_فرزند = عدم;

        إذا(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_قفل guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_فرزند = pTLS->p_Current_گره->پیدا_کن_فرزند(payload);
            إذا(!p_فرزند)
            {
                p_فرزند = جديد Node<گره_داده>(payload);
                pTLS->p_Current_گره->جمع_کن_فرزند(p_فرزند);
            }
        }
        وإلا
        {
            p_فرزند = pTLS->p_Current_گره->پیدا_کن_فرزند(payload);
            إذا(!p_فرزند)
            {
                m_disabled = درست;
                أرجع;
            }
        }
        pTLS->p_Current_گره = p_فرزند;

        m_regionTicks = get_Tick_شمار();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    إذا(pStruct->useInstr)
    {
        إذا(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            إذا (pTLS->p_Current_گره->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_گره->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_گره->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::پایان_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_شمار() - m_regionTicks);
            {
                cv::Auto_قفل guard(pStruct->mutex_شمار); // Concurrent ticks accumulation
                pTLS->p_Current_گره->m_payload.m_counter++;
                pTLS->p_Current_گره->m_payload.m_ticks_مجموع += ticks;
                pTLS->p_Current_گره->m_payload.m_tls.get()->m_ticks_مجموع += ticks;
            }

            pTLS->p_Current_گره = pTLS->p_Current_گره->m_p_والد;
        }
    }
}
#پایان_اگر
}

نطاق ipp
{

#اگر_تعریف_شده HAVE_IPP
بنية IPPInitSingleton
{
عام:
    IPPInitSingleton()
    {
        useIPP         = درست;

#إذا defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#وإلا
        useIPP_NE      = خطأ;
#پایان_اگر
        ipp_وضعیت      = 0;
        funcname       = عدم;
        filename       = عدم;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_بالاترین_Features = 0;
        pIppLibInfo    = عدم;

        ipp_وضعیت = ippGetCpuFeatures(&cpuFeatures, عدم);
        إذا(ipp_وضعیت < 0)
        {
            CV_LOG_ERROR(عدم, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = خطأ;
            أرجع;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        إذا(!env.empty())
        {
#إذا IPP_نسخه_X100 >= 201900
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#وإلاإذا IPP_نسخه_X100 >= 201703
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وإلاإذا IPP_نسخه_X100 >= 201700
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وإلا
            ثابت Ipp64u minorFeatures = 0;
#پایان_اگر

            env = مقصد_پایینی_Case(env);
            إذا(env.substr(0, 2) == "ne")
            {
                useIPP_NE = درست;
                env = env.substr(3, env.size());
            }

            إذا(env == "disabled")
            {
                CV_LOG_WARNING(عدم, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = خطأ;
            }
            وإلا إذا(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            وإلا إذا(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#إذا IPP_نسخه_X100 >= 201700
#إذا defined (_M_AMD64) || defined (__x86_64__)
            وإلا إذا(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#پایان_اگر
#پایان_اگر
            وإلا
                CV_LOG_ERROR(عدم, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        إذا(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        إذا(!(
#إذا IPP_نسخه_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#پایان_اگر
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = خطأ;
            أرجع;
        }

        إذا(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        وإلا
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#إذا IPP_نسخه_X100 >= 201700
        إذا(ippFeatures&ippCPUID_AVX512F)
        {
            إذا((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_بالاترین_Features = ippCPUID_AVX512_SKX;
            وإلا إذا((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_بالاترین_Features = ippCPUID_AVX512_KNL;
            وإلا
                ipp_بالاترین_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        وإلا
#پایان_اگر
        إذا(ippFeatures&ippCPUID_AVX2)
            ipp_بالاترین_Features = ippCPUID_AVX2;
        وإلا إذا(ippFeatures&ippCPUID_SSE42)
            ipp_بالاترین_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_نسخه();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        إذا (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_بالاترین_Features = ippCPUID_SSE42;
        }
    }

عام:
    منطقي        useIPP;
    منطقي        useIPP_NE;

    صحيح         ipp_وضعیت;  // 0 - all is ok, -1 - IPP functions failed
    ثابت حرف *funcname;
    ثابت حرف *filename;
    صحيح         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_بالاترین_Features;
    ثابت Ipp_Library_نسخه *pIppLibInfo;
};

ساكن IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, جديد IPPInitSingleton())
}
#پایان_اگر

بیعلامت دراز دراز getIppFeatures()
{
#اگر_تعریف_شده HAVE_IPP
    أرجع getIPPSingleton().ippFeatures;
#وإلا
    أرجع 0;
#پایان_اگر
}

#اگر_تعریف_شده HAVE_IPP
بیعلامت دراز دراز get_Ipp_بالاترین_Features()
{
    أرجع getIPPSingleton().ipp_بالاترین_Features;
}
#پایان_اگر

فراغ مجموعه_Ipp_وضعیت(صحيح وضعیت, ثابت حرف * ثابت _funcname, ثابت حرف * ثابت _filename, صحيح سطر_3)
{
#اگر_تعریف_شده HAVE_IPP
    getIPPSingleton().ipp_وضعیت = وضعیت;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = سطر_3;
#وإلا
    CV_استفادهنشده(وضعیت); CV_استفادهنشده(_funcname); CV_استفادهنشده(_filename); CV_استفادهنشده(سطر_3);
#پایان_اگر
}

صحيح get_Ipp_وضعیت()
{
#اگر_تعریف_شده HAVE_IPP
    أرجع getIPPSingleton().ipp_وضعیت;
#وإلا
    أرجع 0;
#پایان_اگر
}

String getIppErrorLocation()
{
#اگر_تعریف_شده HAVE_IPP
    أرجع format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#وإلا
    أرجع String();
#پایان_اگر
}

String get_Ipp_نسخه()
{
#اگر_تعریف_شده HAVE_IPP
    ثابت Ipp_Library_نسخه *pInfo = getIPPSingleton().pIppLibInfo;
    إذا(pInfo)
        أرجع format("%s %s %s", pInfo->Name, pInfo->نسخه_4, pInfo->BuildDate);
    وإلا
        أرجع String("error");
#وإلا
    أرجع String("disabled");
#پایان_اگر
}

منطقي useIPP()
{
#اگر_تعریف_شده HAVE_IPP
    CoreTLSData& داده = get_Core_Tls_داده();
    إذا (داده.useIPP < 0)
    {
        داده.useIPP = getIPPSingleton().useIPP;
    }
    أرجع (داده.useIPP > 0);
#وإلا
    أرجع خطأ;
#پایان_اگر
}

فراغ مجموعه_Use_IPP(منطقي flag)
{
    CoreTLSData& داده = get_Core_Tls_داده();
#اگر_تعریف_شده HAVE_IPP
    داده.useIPP = (getIPPSingleton().useIPP)?flag:خطأ;
#وإلا
    CV_استفادهنشده(flag);
    داده.useIPP = خطأ;
#پایان_اگر
}

منطقي useIPP_NotExact()
{
#اگر_تعریف_شده HAVE_IPP
    CoreTLSData& داده = get_Core_Tls_داده();
    إذا (داده.useIPP_NE < 0)
    {
        داده.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    أرجع (داده.useIPP_NE > 0);
#وإلا
    أرجع خطأ;
#پایان_اگر
}

فراغ مجموعه_Use_IPP_Not_Exact(منطقي flag)
{
    CoreTLSData& داده = get_Core_Tls_داده();
#اگر_تعریف_شده HAVE_IPP
    داده.useIPP_NE = flag;
#وإلا
    CV_استفادهنشده(flag);
    داده.useIPP_NE = خطأ;
#پایان_اگر
}

} // namespace ipp


نطاق details {

#إذا OPENCV_IMPL_FP_HINTS_X86
#اگر_تعریف_نشده _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#تعریف_کن _MM_DENORMALS_ZERO_ON 0x0040
#پایان_اگر
#اگر_تعریف_نشده MM_DENORMALS_ZERO_ماسک  // requires pmmintrin.h (SSE3)
#تعریف_کن MM_DENORMALS_ZERO_ماسک 0x0040
#پایان_اگر
#پایان_اگر

فراغ مجموعه_FPDenormals_Ignore_Hint(منطقي ignore, CV_بیرون FPDenormals_نما_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    بیعلامت ماسک = MM_FLUSH_ZERO_ماسک;
    بیعلامت value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    إذا (featuresEnabled.have[CPU_SSE3])
    {
        ماسک |= MM_DENORMALS_ZERO_ماسک;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ثابت بیعلامت old_flags = _mm_getcsr();
    ثابت بیعلامت old_مقدار = old_flags & ماسک;
    بیعلامت flags = (old_flags & ~ماسک) | value;
    CV_LOG_DEBUG(عدم, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)ماسک;
    state.reserved[1] = (uint32_t)old_مقدار;
    _mm_setcsr(flags);
#وإلا
    CV_استفادهنشده(ignore); CV_استفادهنشده(state);
#پایان_اگر
}

صحيح saveFPDenormalsState(CV_بیرون FPDenormals_نما_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    بیعلامت ماسک = MM_FLUSH_ZERO_ماسک;
    إذا (featuresEnabled.have[CPU_SSE3])
    {
        ماسک |= MM_DENORMALS_ZERO_ماسک;
    }
    ثابت بیعلامت old_flags = _mm_getcsr();
    ثابت بیعلامت old_مقدار = old_flags & ماسک;
    // save state
    state.reserved[0] = (uint32_t)ماسک;
    state.reserved[1] = (uint32_t)old_مقدار;
    أرجع 2;
#وإلا
    CV_استفادهنشده(state);
    أرجع 0;
#پایان_اگر
}

منطقي restoreFPDenormalsState(ثابت FPDenormals_نما_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    ثابت بیعلامت ماسک = (بیعلامت)state.reserved[0];
    CV_DbgAssert(ماسک != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت بیعلامت value = (بیعلامت)state.reserved[1];
    CV_DbgCheck((صحيح)value, value == (value & ماسک), "invalid SSE FP state");
    ثابت بیعلامت old_flags = _mm_getcsr();
    بیعلامت flags = (old_flags & ~ماسک) | value;
    CV_LOG_DEBUG(عدم, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    أرجع درست;
#وإلا
    CV_استفادهنشده(state);
    أرجع خطأ;
#پایان_اگر
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#اگر_تعریف_شده OPENCV_ALGO_HINT_DEFAULT
    أرجع OPENCV_ALGO_HINT_DEFAULT;
#وإلا
    أرجع ALGO_HINT_ACCURATE;
#پایان_اگر
};

} // namespace cv

/* End of file. */
