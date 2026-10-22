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

#جيڪڏهن_وصف_ٿيل __QNX__
    #ضمن <unistd.h>
    #ضمن <sys/neutrino.h>
    #ضمن <sys/syspage.h>
#جيڪڏهن_وصف_ٿيل __aarch64__
    #ضمن <aarch64/syspage.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

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
#جيڪڏهن_وصف_ٿيل __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ساكن std::ios_base::Init s_iostream_initializer;
#جيڪڏهن_پورو
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
    سچ
#وإلا
    خطأ
#جيڪڏهن_پورو
);

فراغ* alloc_Singleton_بفر(size_t size) { أرجع fastMalloc(size); }
فراغ* alloc_Singleton_New_بفر(size_t size) { أرجع يادگيري_ڏيو(size); }


} // namespace cv

#جيڪڏهن_وصف_نه_ٿيل CV_ERROR_سيٽ_TERMINATE_HANDLER  // build config option
# إذا defined(_WIN32)
#   define CV_ERROR_سيٽ_TERMINATE_HANDLER 1
# endif
#جيڪڏهن_پورو
#إذا defined(CV_ERROR_سيٽ_TERMINATE_HANDLER) && !CV_ERROR_سيٽ_TERMINATE_HANDLER
# undef CV_ERROR_سيٽ_TERMINATE_HANDLER
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _MSC_VER
# إذا _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل CV_ERROR_سيٽ_TERMINATE_HANDLER
#ضمن <cstdlib>        // std::abort
#جيڪڏهن_پورو

#إذا defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#إذا defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
استعمال Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#وإلا
#  include <elf.h>
#جيڪڏهن_پورو
#إذا defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#جيڪڏهن_پورو


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
#جيڪڏهن_پورو

#إذا defined __loongarch64
#ضمن "sys/auxv.h"
#وصف_ڪريو LA_HWCAP_LSX   (1<<4)
#وصف_ڪريو LA_HWCAP_LASX  (1<<5)
#جيڪڏهن_پورو

#إذا defined _WIN32 || defined WINCE
#جيڪڏهن_وصف_نه_ٿيل _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #وصف_ڪريو _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#جيڪڏهن_پورو
#ضمن <windows.h>
#ضمن <combaseapi.h>
#إذا (_WIN32_WINNT >= 0x0602)
  #ضمن <synchapi.h>
#جيڪڏهن_پورو
#إذا ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ضمن <fibersapi.h>
  #وصف_ڪريو CV_USE_FLS
#جيڪڏهن_پورو
#وصف_رد small
#وصف_رد min
#وصف_رد max
#وصف_رد مطلق_قدر
#ضمن <tchar.h>

#جيڪڏهن_وصف_ٿيل WINRT
#ضمن <wrl/client.h>
#جيڪڏهن_وصف_نه_ٿيل __cplusplus_winrt
#ضمن <windows.storage.h>
#هدايت comment(lib, "runtimeobject.lib")
#جيڪڏهن_پورو // WINRT

std::wstring Get_عارضي_رستو_Win_RT()
{
#جيڪڏهن_وصف_ٿيل __cplusplus_winrt
    أرجع std::wstring(Windows::Storage::Application_معلومات::Current->TemporaryFolder->رستو_2->معلومات_4());
#وإلا
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_معلومات_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_معلومات> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_شي> storageitemRef;
    HSTRING str;
    HSTRING_سرو hstr_سر;
    std::wstring wstr;
    إذا (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_معلومات,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_معلومات), &hstr_سر, &str)))
        أرجع wstr;
    إذا (FAILED(RoGetActivationFactory(str, IID_PPV_دليلون(appdataFactory.Release_And_Get_پتو_جو()))))
        أرجع wstr;
    إذا (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_پتو_جو())))
        أرجع wstr;
    إذا (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_پتو_جو())))
        أرجع wstr;
    إذا (FAILED(storagefolderRef.As(&storageitemRef)))
        أرجع wstr;
    str = عدم;
    إذا (FAILED(storageitemRef->get_رستو(&str)))
        أرجع wstr;
    wstr = Windows_Get_String_Raw_بفر(str, عدم);
    WindowsDeleteString(str);
    أرجع wstr;
#جيڪڏهن_پورو
}

std::wstring Get_عارضي_File_Name_Win_RT(std::wstring اڳياڙي)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* نقاب = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], حجم(guidStr)/حجم(wchar_t), نقاب,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    أرجع اڳياڙي.append(std::wstring(guidStr));
}

#جيڪڏهن_پورو
#وإلا
#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT
#ضمن <pthread.h>
#جيڪڏهن_پورو
#ضمن <sys/time.h>
#ضمن <time.h>

#إذا defined __MACH__ && defined __APPLE__
#ضمن <mach/mach.h>
#ضمن <mach/mach_time.h>
#ضمن <sys/sysctl.h>
#جيڪڏهن_پورو

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل _OPENMP
#ضمن "omp.h"
#جيڪڏهن_پورو

#إذا defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ضمن <unistd.h>
#ضمن <stdio.h>
#ضمن <sys/types.h>
#إذا defined __ANDROID__
#ضمن <sys/sysconf.h>
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __ANDROID__
# include <android/log.h>
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل CV_CPUID_X86
  #إذا defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #إذا _MSC_VER >= 1400  // MSVS 2005
      #ضمن <intrin.h>  // __cpuidex()
      #وصف_ڪريو CV_CPUID_X86 __cpuidex
    #وإلا
      #غلطي "Required MSVS 2005+"
    #جيڪڏهن_پورو
  #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ساكن فراغ cv_cpuid(صحيح* cpuid_معلومات, صحيح reg_eax, صحيح reg_ecx)
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
#غلطي "Configuration error"
#جيڪڏهن_پورو
        cpuid_معلومات[0] = __eax; cpuid_معلومات[1] = __ebx; cpuid_معلومات[2] = __ecx; cpuid_معلومات[3] = __edx;
    }
    #وصف_ڪريو CV_CPUID_X86 cv_cpuid
  #جيڪڏهن_پورو
#جيڪڏهن_پورو

#ضمن <chrono>

نطاق cv
{

Exception::Exception() { ڪوڊ = Error::StsOk; سٽ = 0; }

Exception::Exception(Error::ڪوڊ_3 ڪوڊ_4, ثابت String& _err, ثابت String& _func, ثابت String& _file, صحيح سٽ_3)
: ڪوڊ(ڪوڊ_4), err(_err), func(_func), file(_file), سٽ(سٽ_3)
{
    format_پيغام();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ثابت حرف* Exception::what() ثابت CV_NOEXCEPT { أرجع نياپو.c_str(); }

فراغ Exception::format_پيغام()
{
    size_t pos = err.find('\n');
    منطقي multiline = pos != cv::String::npos;
    إذا (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        طالما (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::سٽ_پڄاڻي;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        إذا (err[err.size() - 1] != '\n')
            ss << std::سٽ_پڄاڻي;
        err = ss.str();
    }
    إذا (func.size() > 0)
    {
        إذا (multiline)
            نياپو = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_نسخو, file.c_str(), سٽ, ڪوڊ, ڪوڊ_پيغام(), func.c_str(), err.c_str());
        وإلا
            نياپو = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_نسخو, file.c_str(), سٽ, ڪوڊ, ڪوڊ_پيغام(), err.c_str(), func.c_str());
    }
    وإلا
    {
        نياپو = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_نسخو, file.c_str(), سٽ, ڪوڊ, ڪوڊ_پيغام(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت حرف * Exception::ڪوڊ_پيغام() ثابت
{
    بدل (ڪوڊ)
    {
    حالة cv::Error::StsOk :                  أرجع "No Error";
    حالة cv::Error::Sts_پٺ_Trace :           أرجع "Backtrace";
    حالة cv::Error::StsError :               أرجع "Unspecified error";
    حالة cv::Error::Sts_اندروني :            أرجع "Internal error";
    حالة cv::Error::StsNoMem :               أرجع "Insufficient memory";
    حالة cv::Error::StsBadArg :              أرجع "Bad argument";
    حالة cv::Error::StsNoConv :              أرجع "Iterations do not converge";
    حالة cv::Error::StsAutoTrace :           أرجع "Autotrace call";
    حالة cv::Error::Sts_Bad_سائيز :             أرجع "Incorrect size of input array";
    حالة cv::Error::StsNullPtr :             أرجع "Null pointer";
    حالة cv::Error::StsDivByZero :           أرجع "Division by zero occurred";
    حالة cv::Error::Bad_قدم :                أرجع "Image step is wrong";
    حالة cv::Error::StsInplaceNotSupported : أرجع "Inplace operation is not supported";
    حالة cv::Error::Sts_Object_Not_مليو :      أرجع "Requested object was not found";
    حالة cv::Error::Bad_اونهائي :               أرجع "Input image depth is not supported by function";
    حالة cv::Error::StsUnmatchedFormats :    أرجع "Formats of input arguments do not match";
    حالة cv::Error::StsUnmatchedSizes :      أرجع "Sizes of input arguments do not match";
    حالة cv::Error::Sts_ٻاهريون_جو_Range :          أرجع "One of the arguments\' values is out of range";
    حالة cv::Error::StsUnsupportedFormat :   أرجع "Unsupported format or combination of formats";
    حالة cv::Error::BadCOI :                 أرجع "Input COI is not supported";
    حالة cv::Error::Bad_انگ_Channels :         أرجع "Bad number of channels";
    حالة cv::Error::StsBadFlag :             أرجع "Bad flag (parameter or structure field)";
    حالة cv::Error::Sts_Bad_نقطو :            أرجع "Bad parameter of type CvPoint";
    حالة cv::Error::Sts_Bad_نقاب :             أرجع "Bad type of mask argument";
    حالة cv::Error::StsParseError :          أرجع "Parsing error";
    حالة cv::Error::StsNotImplemented :      أرجع "The function/feature is not implemented";
    حالة cv::Error::Sts_Bad_Mem_بلاڪ :         أرجع "Memory block has been corrupted";
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
        snprintf(buf, حجم(buf), "Unknown %s code %d", ڪوڊ >= 0 ? "status":"error", ڪوڊ);
        أرجع buf;
    }
    };

}

ساكن ثابت حرف* g_hw_Feature_نالا[CV_HARDWARE_MAX_FEATURE] = { عدم };

ساكن ثابت حرف* getHWFeatureName(صحيح id)
{
    أرجع (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_نالا[id] : عدم;
}
ساكن ثابت حرف* get_HWFeature_Name_محفوظ(صحيح id)
{
    ثابت حرف* name = getHWFeatureName(id);
    أرجع name ? name : "Unknown feature";
}

بنية HWFeatures
{
    تعداد { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقي لاڳيتو_initialize = خطأ)
    {
        إذا (لاڳيتو_initialize)
            initialize();
    }

    ساكن فراغ initialize_نالا()
    {
        لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_نالا[i] = 0;
        }
        g_hw_Feature_نالا[CPU_MMX] = "MMX";
        g_hw_Feature_نالا[CPU_SSE] = "SSE";
        g_hw_Feature_نالا[CPU_SSE2] = "SSE2";
        g_hw_Feature_نالا[CPU_SSE3] = "SSE3";
        g_hw_Feature_نالا[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_نالا[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_نالا[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_نالا[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_نالا[CPU_FP16] = "FP16";
        g_hw_Feature_نالا[CPU_AVX] = "AVX";
        g_hw_Feature_نالا[CPU_AVX2] = "AVX2";
        g_hw_Feature_نالا[CPU_FMA3] = "FMA3";

        g_hw_Feature_نالا[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_نالا[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_نالا[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_نالا[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_نالا[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_نالا[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_نالا[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_نالا[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_نالا[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_نالا[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_نالا[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_نالا[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_نالا[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_نالا[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_نالا[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_نالا[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_نالا[CPU_NEON] = "NEON";
        g_hw_Feature_نالا[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_نالا[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_نالا[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_نالا[CPU_SVE] = "SVE";

        g_hw_Feature_نالا[CPU_VSX] = "VSX";
        g_hw_Feature_نالا[CPU_VSX3] = "VSX3";

        g_hw_Feature_نالا[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_نالا[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_نالا[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_نالا[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_نالا[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_نالا[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_نالا[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_نالا[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_نالا[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_نالا[CPU_RVV] = "RVV";
        g_hw_Feature_نالا[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_نالا[CPU_LSX]  = "LSX";
        g_hw_Feature_نالا[CPU_LASX] = "LASX";
    }

    فراغ initialize(فراغ)
    {
        إذا (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_نالا();

    #جيڪڏهن_وصف_ٿيل CV_CPUID_X86
        صحيح cpuid_معلومات[4] = { 0, 0, 0, 0 };
        صحيح cpuid_معلومات_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_معلومات, 1, 0/*اڻ_استعمال_ٿيل*/);

        صحيح x86_family = (cpuid_معلومات[0] >> 8) & 15;
        إذا( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_معلومات[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_معلومات[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_معلومات[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_معلومات[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_معلومات[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_معلومات[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_معلومات[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_معلومات[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_معلومات[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_معلومات[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_معلومات[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_معلومات_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_معلومات_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_معلومات_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_معلومات_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_معلومات_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_معلومات_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_معلومات_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_معلومات_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_معلومات_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_معلومات_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_معلومات_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_معلومات_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_معلومات_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_معلومات_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_معلومات_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_معلومات_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_معلومات_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            صحيح cpuid_معلومات_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_معلومات_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_معلومات_ex1[0] & (1<<4))  != 0;

            منطقي have_AVX_OS_support = سچ;
            منطقي have_AVX512_OS_support = سچ;
            إذا (!(cpuid_معلومات[2] & (1<<27)))
                have_AVX_OS_support = خطأ; // OS uses XSAVE_XRSTORE and CPU support AVX
            وإلا
            {
                صحيح xcr0 = 0;
            #جيڪڏهن_وصف_ٿيل XCR_XFEATURE_ENABLED_نقاب // requires immintrin.h
                xcr0 = (صحيح)_xgetbv(XCR_XFEATURE_ENABLED_نقاب);
            #وإلاإذا defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #جيڪڏهن_پورو
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
    #جيڪڏهن_پورو // CV_CPUID_X86

    #إذا defined __ANDROID__ || defined __linux__ || defined __QNX__
    #جيڪڏهن_وصف_ٿيل __aarch64__
        have[CV_CPU_NEON] = سچ;
        have[CV_CPU_FP16] = سچ;
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        إذا (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t سائيز_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, سائيز_auxv_t) == سائيز_auxv_t)
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
#جيڪڏهن_پورو
            }

            close(cpufile);
        }
    #وإلاإذا defined __arm__ && defined __ANDROID__
      #إذا defined HAVE_CPUFEATURES
        CV_LOG_INFO(عدم, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(عدم, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (ڊگهو ڊگهو)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #وإلا
        CV_LOG_INFO(عدم, "cpufeatures library is not available for CPU detection");
        #إذا CV_NEON
        CV_LOG_INFO(عدم, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = سچ;
        #وإلا
        CV_LOG_INFO(عدم, "- NEON instructions is NOT enabled via build flags");
        #جيڪڏهن_پورو
        #إذا CV_FP16
        CV_LOG_INFO(عدم, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = سچ;
        #وإلا
        CV_LOG_INFO(عدم, "- FP16 instructions is NOT enabled via build flags");
        #جيڪڏهن_پورو
      #جيڪڏهن_پورو
    #وإلاإذا defined __arm__
        صحيح cpufile = open("/proc/self/auxv", O_RDONLY);

        إذا (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t سائيز_auxv_t = حجم(auxv);

            طالما ((size_t)اقرأ(cpufile, &auxv, سائيز_auxv_t) == سائيز_auxv_t)
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
    #جيڪڏهن_پورو
    #وإلاإذا (defined __APPLE__)
    #إذا defined __ARM_NEON
        have[CV_CPU_NEON] = سچ;
    #جيڪڏهن_پورو
    #إذا (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = سچ;
    #جيڪڏهن_پورو
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحيح has_feat_dotprod = 0;
    size_t has_feat_dotprod_سائيز = حجم(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_سائيز, عدم, 0);
    إذا (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = سچ;
    }
    صحيح has_feat_bf16 = 0;
    size_t has_feat_bf16_سائيز = حجم(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_سائيز, عدم, 0);
    إذا (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = سچ;
    }
    #وإلاإذا (defined __clang__)
    #إذا defined __ARM_NEON
        have[CV_CPU_NEON] = سچ;
        #إذا (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = سچ;
        #جيڪڏهن_پورو
    #جيڪڏهن_پورو
    #جيڪڏهن_پورو
    #إذا defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = سچ;
    #جيڪڏهن_پورو
    #إذا defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = سچ;
    #جيڪڏهن_پورو
    #جيڪڏهن_وصف_ٿيل __riscv_vector
        have[CV_CPU_RISCVV] = سچ;
    #جيڪڏهن_پورو
    #جيڪڏهن_وصف_ٿيل __mips_msa
        have[CV_CPU_MSA] = سچ;
    #جيڪڏهن_پورو

    #إذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        بنانشان صحيح hwcap = getauxval(AT_HWCAP);
        إذا (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            إذا (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = سچ;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلاإذا (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        بنانشان ڊگهو hwcap = 0, hwcap2 = 0;
        إذا (elf_aux_info(AT_HWCAP, &hwcap, حجم(hwcap)) != 0)
            hwcap = 0;
        إذا (hwcap & PPC_FEATURE_HAS_VSX) {
            إذا (elf_aux_info(AT_HWCAP2, &hwcap2, حجم(hwcap2)) != 0)
                hwcap2 = 0;
            إذا (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = سچ;
            } وإلا {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وإلا
        // TODO: AIX
        #إذا CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = سچ;
        #جيڪڏهن_پورو
        #إذا CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = سچ;
        #جيڪڏهن_پورو
    #جيڪڏهن_پورو

    #إذا defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = سچ;
        #إذا (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = سچ;
        #جيڪڏهن_پورو
        #إذا defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = سچ;
        #جيڪڏهن_پورو
    #جيڪڏهن_پورو

    #إذا defined __loongarch64 && defined __linux__
        صحيح flag = (صحيح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #جيڪڏهن_پورو

        منطقي skip_baseline_check = خطأ;
        إذا (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = سچ;
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
            checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]), سچ);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]));
    }

    منطقي checkFeatures(ثابت صحيح* features, صحيح ڳڻپ_2, منطقي dump = خطأ)
    {
        منطقي نتيجو = سچ;
        لكل (صحيح i = 0; i < ڳڻپ_2; i++)
        {
            صحيح feature = features[i];
            إذا (feature)
            {
                إذا (have[feature])
                {
                    إذا (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_محفوظ(feature));
                }
                وإلا
                {
                    نتيجو = خطأ;
                    إذا (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_محفوظ(feature));
                }
            }
        }
        أرجع نتيجو;
    }

    ساكن مضمن منطقي is_نشان_Separator(حرف c)
    {
        أرجع c == ',' || c == ';';
    }

    فراغ readSettings(ثابت صحيح* baseline_features, صحيح baseline_ڳڻپ)
    {
        منطقي dump = سچ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        إذا (!disabled_features.empty())
        {
            ثابت حرف* شروعات = disabled_features.c_str();
            لكل (;;)
            {
                طالما (شروعات[0] != 0 && is_نشان_Separator(شروعات[0]))
                {
                    شروعات++;
                }
                إذا (شروعات[0] == 0)
                    اكسر;
                ثابت حرف* end = شروعات;
                طالما (end[0] != 0 && !is_نشان_Separator(end[0]))
                {
                    end++;
                }
                إذا (end == شروعات)
                    استمر;
                cv::String feature(شروعات, end);
                شروعات = end;

                CV_Assert(feature.size() > 0);

                منطقي مليو = خطأ;
                لكل (صحيح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    إذا (!g_hw_Feature_نالا[i]) استمر;
                    size_t len = لڙي_ڊيگهه(g_hw_Feature_نالا[i]);
                    إذا (len != feature.size()) استمر;
                    إذا (feature.compare(g_hw_Feature_نالا[i]) == 0)
                    {
                        منطقي isBaseline = خطأ;
                        لكل (صحيح k = 0; k < baseline_ڳڻپ; k++)
                        {
                            إذا (baseline_features[k] == i)
                            {
                                isBaseline = سچ;
                                اكسر;
                            }
                        }
                        إذا (isBaseline)
                        {
                            إذا (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_محفوظ(i));
                        }
                        إذا (!have[i])
                        {
                            إذا (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_محفوظ(i));
                        }
                        have[i] = خطأ;

                        مليو = سچ;
                        اكسر;
                    }
                }
                إذا (!مليو)
                {
                    إذا (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقي have[MAX_FEATURE+1]{};
};

ساكن HWFeatures  featuresEnabled(سچ), featuresDisabled = HWFeatures(خطأ);
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

std::string get_CPUFeatures_سٽ()
{
    ثابت صحيح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحيح sz = حجم(features) / حجم(features[0]);
    std::string نتيجو;
    std::string اڳياڙي;
    لكل (صحيح i = 1; i < sz; ++i)
    {
        إذا (features[i] == 0)
        {
            اڳياڙي = "*";
            استمر;
        }
        إذا (i != 1) نتيجو.append(" ");
        نتيجو.append(اڳياڙي);
        نتيجو.append(get_HWFeature_Name_محفوظ(features[i]));
        إذا (!checkHardwareSupport(features[i])) نتيجو.append("?");
    }
    أرجع نتيجو;
}

متطاير منطقي useOptimizedFlag = سچ;

فراغ سيٽ_Use_Optimized( منطقي flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::سيٽ_Use_IPP(flag);
#جيڪڏهن_وصف_ٿيل HAVE_OPENCL
    ocl::سيٽ_Use_Open_CL(flag);
#جيڪڏهن_پورو
}

منطقي useOptimized(فراغ)
{
    أرجع useOptimizedFlag;
}

int64 get_Tick_ڳڻپ(فراغ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    أرجع (int64)now.time_since_epoch().ڳڻپ_2();
}

مزدوج getTickFrequency(فراغ)
{
    استعمال clock_period_t = std::chrono::steady_clock::duration::period;
    مزدوج clock_freq = clock_period_t::den / clock_period_t::num;
    أرجع clock_freq;
}

#إذا defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#إذا defined(__i386__)

int64 get_CPUTick_ڳڻپ(فراغ)
{
    int64 x;
    __asm__ متطاير (".byte 0x0f, 0x31" : "=A" (x));
    أرجع x;
}
#وإلاإذا defined(__x86_64__)

int64 get_CPUTick_ڳڻپ(فراغ)
{
    بنانشان hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    أرجع (int64)lo | ((int64)hi << 32);
}

#وإلاإذا defined(__ppc__)

int64 get_CPUTick_ڳڻپ(فراغ)
{
    بنانشان upper, lower, tmp;
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

#غلطي "RDTSC not defined"

#جيڪڏهن_پورو

#وإلاإذا defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ڳڻپ(فراغ)
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
int64 get_CPUTick_ڳڻپ(فراغ)
{
    أرجع get_Tick_ڳڻپ();
}
//#endif

#جيڪڏهن_پورو


نطاق internal {

صنف Timestamp
{
عام:
    ثابت int64 zero_Tick_ڳڻپ;
    ثابت مزدوج ns_اندريون_ticks;

    Timestamp()
        : zero_Tick_ڳڻپ(get_Tick_ڳڻپ())
        , ns_اندريون_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ڳڻپ();
        أرجع (int64)((t - zero_Tick_ڳڻپ) * ns_اندريون_ticks);
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

String get_نسخو_String() { أرجع String(CV_نسخو); }

صحيح get_نسخو_Major() { أرجع CV_نسخو_MAJOR; }

صحيح get_نسخو_Minor() { أرجع CV_نسخو_MINOR; }

صحيح get_نسخو_Revision() { أرجع CV_نسخو_REVISION; }

String format( ثابت حرف* fmt, ... )
{
    Auto_بفر<حرف, 1024> buf;

    لكل ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحيح bsize = static_cast<صحيح>(buf.size());
        صحيح len = cv_vsnprintf(buf.معلومات(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        إذا (len >= bsize)
        {
            buf.resize(len + 1);
            استمر;
        }
        buf[bsize - 1] = 0;
        أرجع String(buf.معلومات(), len);
    }
}

String tempfile( ثابت حرف* suffix )
{
#إذا OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string عارضي_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#إذا defined _WIN32
#جيڪڏهن_وصف_ٿيل WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring عارضي_dir_rt = Get_عارضي_رستو_Win_RT();

    std::wstring عارضي_file = Get_عارضي_File_Name_Win_RT(L"ocv");
    إذا (عارضي_file.empty())
        أرجع String();

    عارضي_file = عارضي_dir_rt.append(std::wstring(L"\\")).append(عارضي_file);
    DeleteFileW(عارضي_file.c_str());

    حرف aname[MAX_رستو];
    size_t copied = wcstombs(aname, عارضي_file.c_str(), MAX_رستو);
    CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#وإلاإذا defined(_WIN32_WCE)
    ثابت خودڪار k_Max_رستو_سائيز = MAX_رستو+1;
    wchar_t عارضي_dir_ce[k_Max_رستو_سائيز] = {0};
    wchar_t عارضي_file[k_Max_رستو_سائيز] = {0};

    ::Get_عارضي_رستو_W(k_Max_رستو_سائيز, عارضي_dir_ce);

    إذا(0 != ::Get_عارضي_File_Name_W(عارضي_dir_ce, L"ocv", 0, عارضي_file)) {
        DeleteFileW(عارضي_file);
        حرف aname[MAX_رستو];
        size_t copied = wcstombs(aname, عارضي_file, MAX_رستو);
        CV_Assert((copied != MAX_رستو) && (copied != (size_t)-1));
        fname = String(aname);
    }
#وإلا
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t عارضي_dir2_w[MAX_رستو] = { 0 };

    إذا (عارضي_dir.empty())
    {
        ::Get_عارضي_رستو_W(حجم(عارضي_dir2_w)/حجم(wchar_t), عارضي_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحيح len = Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, عارضي_dir2_w, -1, عدم, 0, عدم, عدم);
        إذا (len > 0)
        {
            std::vector<حرف> utf8_buf(len);
            Wide_Char_منزل_Multi_Byte(CP_UTF8, 0, عارضي_dir2_w, -1, utf8_buf.معلومات(), len, عدم, عدم);
            عارضي_dir = std::string(utf8_buf.معلومات());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    إذا (FAILED(hr))
        أرجع String();
    حرف guidStr[40];
    ثابت حرف* نقاب = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, حجم(guidStr), نقاب,
            g.Data1, g.Data2, g.Data3, (بنانشان صحيح)g.Data4[0], (بنانشان صحيح)g.Data4[1],
            (بنانشان صحيح)g.Data4[2], (بنانشان صحيح)g.Data4[3], (بنانشان صحيح)g.Data4[4],
            (بنانشان صحيح)g.Data4[5], (بنانشان صحيح)g.Data4[6], (بنانشان صحيح)g.Data4[7]);

    fname = عارضي_dir;
    إذا (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#جيڪڏهن_پورو
# وإلا
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    حرف defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  وإلا
    حرف defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    إذا (عارضي_dir.empty())
        fname = defaultTemplate;
    وإلا
    {
        fname = عارضي_dir;
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
    CV_اڻ_استعمال_ٿيل(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#جيڪڏهن_پورو // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ساكن ErrorCallback customErrorCallback = 0;
ساكن فراغ* custom_Error_Callback_معلومات = 0;
ساكن منطقي breakOnError = خطأ;

منطقي سيٽ_Break_On_Error(منطقي value)
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

صحيح cv_vsnprintf(حرف* buf, صحيح len, ثابت حرف* fmt, va_list دليلون)
{
#إذا defined _MSC_VER
    إذا (len <= 0) أرجع len == 0 ? 1024 : -1;
    صحيح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, دليلون);
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
    أرجع vsnprintf(buf, len, fmt, دليلون);
#جيڪڏهن_پورو
}

ساكن فراغ dumpException(ثابت Exception& exc)
{
    ثابت حرف* errorStr = exc.ڪوڊ_پيغام();
    حرف buf[1 << 12];

    cv_snprintf(buf, حجم(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_نسخو,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.سٽ);
#جيڪڏهن_وصف_ٿيل __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#وإلا
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل CV_ERROR_سيٽ_TERMINATE_HANDLER
ساكن منطقي cv_terminate_handler_installed = خطأ;
ساكن std::terminate_handler cv_old_terminate_handler;
ساكن cv::Exception cv_terminate_handler_exception;
ساكن منطقي param_تياري_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", سچ);
ساكن فراغ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    إذا (خطأ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل __GNUC__
# إذا defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#جيڪڏهن_پورو

فراغ error( ثابت Exception& exc )
{
#جيڪڏهن_وصف_ٿيل CV_ERROR_سيٽ_TERMINATE_HANDLER
    {
        cv::Auto_تالو lock(getInitializationMutex());
        إذا (!cv_terminate_handler_installed)
        {
            إذا (param_تياري_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = سچ;
        }
        cv_terminate_handler_exception = exc;
    }
#جيڪڏهن_پورو

    إذا (customErrorCallback != 0)
        customErrorCallback(exc.ڪوڊ, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.سٽ, custom_Error_Callback_معلومات);
    وإلا إذا (param_dumpErrors)
    {
        dumpException(exc);
    }

    إذا(breakOnError)
    {
        std::terminate();
    }

    ارم exc;
#جيڪڏهن_وصف_ٿيل __GNUC__
# إذا !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#جيڪڏهن_پورو
}

فراغ error(Error::ڪوڊ_3 ڪوڊ_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح سٽ_3)
{
    error(cv::Exception(ڪوڊ_4, _err, _func, _file, سٽ_3));
#جيڪڏهن_وصف_ٿيل __GNUC__
# إذا !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل __GNUC__
# إذا defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#جيڪڏهن_پورو


ErrorCallback
redirectError( ErrorCallback errCallback, فراغ* userdata, فراغ** prevUserdata)
{
    إذا( prevUserdata )
        *prevUserdata = custom_Error_Callback_معلومات;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_معلومات = userdata;

    أرجع prevCallback;
}

فراغ terminate(Error::ڪوڊ_3 ڪوڊ_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحيح سٽ_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(ڪوڊ_4, _err, _func, _file, سٽ_3));
    std::terminate();
}

}

نطاق cv {
منطقي __termination = خطأ;


//////////////////////////////// thread-local storage ////////////////////////////////

نطاق details {

#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT

#جيڪڏهن_وصف_ٿيل _WIN32
#جيڪڏهن_وصف_ٿيل _MSC_VER
#هدايت warning(disable:4505) // unreferenced local function has been removed
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل TLS_ٻاهريون_جو_INDEXES
#وصف_ڪريو TLS_ٻاهريون_جو_INDEXES ((DWORD)0xFFFFFFFF)
#جيڪڏهن_پورو
#جيڪڏهن_پورو

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

    فراغ* get_معلومات() ثابت;
    فراغ سيٽ_معلومات(فراغ *p_معلومات);

    فراغ releaseSystemResources();

خاص:

#جيڪڏهن_وصف_ٿيل _WIN32
#جيڪڏهن_وصف_نه_ٿيل WINRT
    DWORD tls_ڪنجي;
    منطقي disposed;
#جيڪڏهن_پورو
#وإلا // _WIN32
    pthread_key_t  tls_ڪنجي;
    std::atomic<منطقي> disposed;
#جيڪڏهن_پورو
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


#جيڪڏهن_وصف_ٿيل _WIN32
#جيڪڏهن_وصف_ٿيل WINRT
ساكن __declspec( thread ) فراغ* tls_معلومات = عدم; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
}
فراغ* TlsAbstraction::get_معلومات() ثابت
{
    أرجع tls_معلومات;
}
فراغ TlsAbstraction::سيٽ_معلومات(فراغ *p_معلومات)
{
    tls_معلومات = p_معلومات;
}
#وإلا //WINRT
#جيڪڏهن_وصف_ٿيل CV_USE_FLS
ساكن فراغ NTAPI opencv_fls_destructor(فراغ* p_معلومات);
#جيڪڏهن_پورو // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
#جيڪڏهن_وصف_نه_ٿيل CV_USE_FLS
    tls_ڪنجي = TlsAlloc();
#وإلا // CV_USE_FLS
    tls_ڪنجي = FlsAlloc(opencv_fls_destructor);
#جيڪڏهن_پورو // CV_USE_FLS
    CV_Assert(tls_ڪنجي != TLS_ٻاهريون_جو_INDEXES);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
    disposed = سچ;
#جيڪڏهن_وصف_نه_ٿيل CV_USE_FLS
    TlsFree(tls_ڪنجي);
#وإلا // CV_USE_FLS
    FlsFree(tls_ڪنجي);
#جيڪڏهن_پورو // CV_USE_FLS
    tls_ڪنجي = TLS_ٻاهريون_جو_INDEXES;
}
فراغ* TlsAbstraction::get_معلومات() ثابت
{
    إذا (disposed)
        أرجع عدم;
#جيڪڏهن_وصف_نه_ٿيل CV_USE_FLS
    أرجع Tls_Get_قدر(tls_ڪنجي);
#وإلا // CV_USE_FLS
    أرجع Fls_Get_قدر(tls_ڪنجي);
#جيڪڏهن_پورو // CV_USE_FLS
}
فراغ TlsAbstraction::سيٽ_معلومات(فراغ *p_معلومات)
{
    إذا (disposed)
        أرجع;  // no-op
#جيڪڏهن_وصف_نه_ٿيل CV_USE_FLS
    CV_Assert(Tls_سيٽ_قدر(tls_ڪنجي, p_معلومات) == TRUE);
#وإلا // CV_USE_FLS
    CV_Assert(Fls_سيٽ_قدر(tls_ڪنجي, p_معلومات) == TRUE);
#جيڪڏهن_پورو // CV_USE_FLS
}
#جيڪڏهن_پورو // WINRT
#وإلا // _WIN32
ساكن فراغ opencv_tls_destructor(فراغ* p_معلومات);
TlsAbstraction::TlsAbstraction()
    : disposed(خطأ)
{
    CV_Assert(pthread_key_create(&tls_ڪنجي, opencv_tls_destructor) == 0);
}
فراغ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
    disposed = سچ;
    إذا (pthread_key_delete(tls_ڪنجي) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
فراغ* TlsAbstraction::get_معلومات() ثابت
{
    إذا (disposed)
        أرجع عدم;
    أرجع pthread_getspecific(tls_ڪنجي);
}
فراغ TlsAbstraction::سيٽ_معلومات(فراغ *p_معلومات)
{
    إذا (disposed)
        أرجع;  // no-op
    CV_Assert(pthread_setspecific(tls_ڪنجي, p_معلومات) == 0);
}
#جيڪڏهن_پورو

// Per-thread data structure
بنية Thread_معلومات
{
    Thread_معلومات()
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
        tls_Slots_سائيز(0)
    {
        (فراغ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        ڌاڳا.reserve(32);
        g_isTlsStorageInitialized = سچ;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    فراغ releaseThread(فراغ* tls_قدر = عدم)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)
        Thread_معلومات *pTD = tls_قدر == عدم ? (Thread_معلومات*)tls->get_معلومات() : (Thread_معلومات*)tls_قدر;
        إذا (pTD == عدم)
            أرجع;  // no OpenCV TLS data for this thread
        Auto_تالو guard(mtxGlobalAccess);
        لكل (size_t i = 0; i < ڌاڳا.size(); i++)
        {
            إذا (pTD == ڌاڳا[i])
            {
                ڌاڳا[i] = عدم;
                إذا (tls_قدر == عدم)
                    tls->سيٽ_معلومات(0);
                std::vector<فراغ*>& thread_slots = pTD->slots;
                لكل (size_t جڳهه_Idx = 0; جڳهه_Idx < thread_slots.size(); جڳهه_Idx++)
                {
                    فراغ* p_معلومات = thread_slots[جڳهه_Idx];
                    thread_slots[جڳهه_Idx] = عدم;
                    إذا (!p_معلومات)
                        استمر;
                    TLSDataContainer* container = tlsSlots[جڳهه_Idx].container;
                    إذا (container)
                        container->delete_معلومات_Instance(p_معلومات);
                    وإلا
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحيح)جڳهه_Idx);
                        fflush(stderr);
                    }
                }
                ميٽيو pTD;
                أرجع;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (فراغ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_جڳهه(TLSDataContainer* container)
    {
        Auto_تالو guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائيز == tlsSlots.size());

        // Find unused slots
        لكل(size_t جڳهه = 0; جڳهه < tls_Slots_سائيز; جڳهه++)
        {
            إذا (tlsSlots[جڳهه].container == عدم)
            {
                tlsSlots[جڳهه].container = container;
                أرجع جڳهه;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_جڳهه_Info(container)); tls_Slots_سائيز++;
        أرجع tls_Slots_سائيز - 1;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ release_جڳهه(size_t جڳهه_Idx, std::vector<فراغ*> &معلومات_Vec, منطقي keep_جڳهه = خطأ)
    {
        Auto_تالو guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائيز == tlsSlots.size());
        CV_Assert(tls_Slots_سائيز > جڳهه_Idx);

        لكل(size_t i = 0; i < ڌاڳا.size(); i++)
        {
            إذا(ڌاڳا[i])
            {
                std::vector<فراغ*>& thread_slots = ڌاڳا[i]->slots;
                إذا (thread_slots.size() > جڳهه_Idx && thread_slots[جڳهه_Idx])
                {
                    معلومات_Vec.push_back(thread_slots[جڳهه_Idx]);
                    thread_slots[جڳهه_Idx] = عدم;
                }
            }
        }

        إذا (!keep_جڳهه)
        {
            tlsSlots[جڳهه_Idx].container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* get_معلومات(size_t جڳهه_Idx) ثابت
    {
#جيڪڏهن_وصف_نه_ٿيل CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_سائيز > جڳهه_Idx);
#جيڪڏهن_پورو

        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع عدم;  // TLS singleton is not available (terminated)

        Thread_معلومات* thread_معلومات = (Thread_معلومات*)tls->get_معلومات();
        إذا(thread_معلومات && thread_معلومات->slots.size() > جڳهه_Idx)
            أرجع thread_معلومات->slots[جڳهه_Idx];

        أرجع عدم;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t جڳهه_Idx, std::vector<فراغ*> &معلومات_Vec)
    {
        Auto_تالو guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائيز == tlsSlots.size());
        CV_Assert(tls_Slots_سائيز > جڳهه_Idx);

        لكل(size_t i = 0; i < ڌاڳا.size(); i++)
        {
            إذا(ڌاڳا[i])
            {
                std::vector<فراغ*>& thread_slots = ڌاڳا[i]->slots;
                إذا (thread_slots.size() > جڳهه_Idx && thread_slots[جڳهه_Idx])
                    معلومات_Vec.push_back(thread_slots[جڳهه_Idx]);
            }
        }
    }

    // Set data to storage index
    فراغ سيٽ_معلومات(size_t جڳهه_Idx, فراغ* p_معلومات)
    {
#جيڪڏهن_وصف_نه_ٿيل CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_سائيز > جڳهه_Idx);
#جيڪڏهن_پورو

        TlsAbstraction* tls = getTlsAbstraction();
        إذا (عدم == tls)
            أرجع;  // TLS singleton is not available (terminated)

        Thread_معلومات* thread_معلومات = (Thread_معلومات*)tls->get_معلومات();
        إذا(!thread_معلومات)
        {
            thread_معلومات = جديد Thread_معلومات;
            tls->سيٽ_معلومات((فراغ*)thread_معلومات);
            {
                Auto_تالو guard(mtxGlobalAccess);

                منطقي مليو = خطأ;
                // Find unused slots
                لكل(size_t جڳهه = 0; جڳهه < ڌاڳا.size(); جڳهه++)
                {
                    إذا (ڌاڳا[جڳهه] == عدم)
                    {
                        thread_معلومات->idx = (صحيح)جڳهه;
                        ڌاڳا[جڳهه] = thread_معلومات;
                        مليو = سچ;
                        اكسر;
                    }
                }

                إذا (!مليو)
                {
                    // Create new slot
                    thread_معلومات->idx = ڌاڳا.size();
                    ڌاڳا.push_back(thread_معلومات);
                }
            }
        }

        إذا(جڳهه_Idx >= thread_معلومات->slots.size())
        {
            Auto_تالو guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_معلومات->slots.resize(جڳهه_Idx + 1, عدم);
        }
        thread_معلومات->slots[جڳهه_Idx] = p_معلومات;
    }

خاص:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_سائيز;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    بنية Tls_جڳهه_Info
    {
        Tls_جڳهه_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<بنية Tls_جڳهه_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_معلومات*> ڌاڳا; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ساكن TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, جديد TlsStorage())
}

#جيڪڏهن_وصف_نه_ٿيل _WIN32  // pthread key destructor
ساكن فراغ opencv_tls_destructor(فراغ* p_معلومات)
{
    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_معلومات);
}
#وإلا // _WIN32
#جيڪڏهن_وصف_ٿيل CV_USE_FLS
ساكن فراغ WINAPI opencv_fls_destructor(فراغ* p_معلومات)
{
    // Empiric detection of ExitProcess call
    DWORD ڪوڊ = STILL_سرگرم/*259*/;
    BOOL res = Get_Exit_ڪوڊ_Process(GetCurrentProcess(), &ڪوڊ);
    إذا (res && ڪوڊ != STILL_سرگرم)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = سچ;
    }

    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_معلومات);
}
#جيڪڏهن_پورو // CV_USE_FLS
#جيڪڏهن_پورو // _WIN32

ساكن TlsStorage* ثابت g_force_initialization_جو_Tls_Storage
#إذا defined __GNUC__
    __attribute__((اڻ_استعمال_ٿيل))
#جيڪڏهن_پورو
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
        لكل (size_t جڳهه_Idx = 0; جڳهه_Idx < slots.size(); جڳهه_Idx++)
        {
            جڳهه_Info& s = slots[جڳهه_Idx];
            TLSDataContainer* container = s.container;
            إذا (container && s.معلومات)
            {
                container->delete_معلومات_Instance(s.معلومات);  // Can't use from SlotInfo destructor
                s.معلومات = خالي_اشارو;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_جڳهه(TLSDataContainer* container)
    {
        size_t slots_سائيز = slots.size();
        لكل (size_t جڳهه = 0; جڳهه < slots_سائيز; جڳهه++)
        {
            جڳهه_Info& s = slots[جڳهه];
            إذا (s.container == عدم)
            {
                CV_Assert(!s.معلومات);
                s.container = container;
                أرجع جڳهه;
            }
        }

        // create new slot
        slots.push_back(جڳهه_Info(container));
        أرجع slots_سائيز;
    }

    // Release TLS storage index and pass associated data to caller
    فراغ release_جڳهه(size_t جڳهه_Idx, std::vector<فراغ*> &معلومات_Vec, منطقي keep_جڳهه = خطأ)
    {
        CV_Assert(جڳهه_Idx < slots.size());
        جڳهه_Info& s = slots[جڳهه_Idx];
        فراغ* معلومات = s.معلومات;
        إذا (معلومات)
        {
            معلومات_Vec.push_back(معلومات);
            s.معلومات = خالي_اشارو;
        }
        إذا (!keep_جڳهه)
        {
            s.container = عدم;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    فراغ* get_معلومات(size_t جڳهه_Idx) ثابت
    {
        CV_Assert(جڳهه_Idx < slots.size());
        ثابت جڳهه_Info& s = slots[جڳهه_Idx];
        أرجع s.معلومات;
    }

    // Gather data from threads by TLS storage index
    فراغ gather(size_t جڳهه_Idx, std::vector<فراغ*> &معلومات_Vec)
    {
        CV_Assert(جڳهه_Idx < slots.size());
        جڳهه_Info& s = slots[جڳهه_Idx];
        فراغ* معلومات = s.معلومات;
        إذا (معلومات)
            معلومات_Vec.push_back(معلومات);
        أرجع;
    }

    // Set data to storage index
    فراغ سيٽ_معلومات(size_t جڳهه_Idx, فراغ* p_معلومات)
    {
        CV_Assert(جڳهه_Idx < slots.size());
        جڳهه_Info& s = slots[جڳهه_Idx];
        s.معلومات = p_معلومات;
    }

خاص:
    بنية جڳهه_Info
    {
        جڳهه_Info(TLSDataContainer* _container) : container(_container), معلومات(خالي_اشارو) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        فراغ* معلومات;
    };
    std::vector<بنية جڳهه_Info> slots;
};

ساكن TlsStorage& getTlsStorage()
{
    ساكن TlsStorage g_storage;  // no threading
    أرجع g_storage;
}

#جيڪڏهن_پورو  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
استعمال نطاق details;

فراغ releaseTlsStorageThread()
{
#جيڪڏهن_وصف_نه_ٿيل OPENCV_DISABLE_THREAD_SUPPORT
    إذا (!g_isTlsStorageInitialized)
        أرجع;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#جيڪڏهن_پورو
}

TLSDataContainer::TLSDataContainer()
{
    ڪنجي_2 = (صحيح)getTlsStorage().reserve_جڳهه(هذا); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ڪنجي_2 == -1); // Key must be released in child object
}

فراغ TLSDataContainer::gather_معلومات(std::vector<فراغ*> &معلومات) ثابت
{
    getTlsStorage().gather(ڪنجي_2, معلومات);
}

فراغ TLSDataContainer::detach_معلومات(std::vector<فراغ*> &معلومات)
{
    getTlsStorage().release_جڳهه(ڪنجي_2, معلومات, سچ);
}

فراغ TLSDataContainer::release()
{
    إذا (ڪنجي_2 == -1)
        أرجع;  // already released
    std::vector<فراغ*> معلومات; معلومات.reserve(32);
    getTlsStorage().release_جڳهه(ڪنجي_2, معلومات, خطأ); // Release key and get stored data for proper destruction
    ڪنجي_2 = -1;
    لكل(size_t i = 0; i < معلومات.size(); i++)  // Delete all associated data
        delete_معلومات_Instance(معلومات[i]);
}

فراغ TLSDataContainer::cleanup()
{
    std::vector<فراغ*> معلومات; معلومات.reserve(32);
    getTlsStorage().release_جڳهه(ڪنجي_2, معلومات, سچ); // Extract stored data with removal from TLS tables
    لكل(size_t i = 0; i < معلومات.size(); i++)  // Delete all associated data
        delete_معلومات_Instance(معلومات[i]);
}

فراغ* TLSDataContainer::get_معلومات() ثابت
{
    CV_Assert(ڪنجي_2 != -1 && "Can't fetch data from terminated TLS container.");
    فراغ* p_معلومات = getTlsStorage().get_معلومات(ڪنجي_2); // Check if data was already allocated
    إذا(!p_معلومات)
    {
        // Create new data instance and save it to TLS storage
        p_معلومات = create_معلومات_Instance();
        حاول
        {
            getTlsStorage().سيٽ_معلومات(ڪنجي_2, p_معلومات);
        }
        امسك (...)
        {
            delete_معلومات_Instance(p_معلومات);
            ارم;
        }
    }
    أرجع p_معلومات;
}

ساكن TLSData<CoreTLSData>& get_Core_Tls_معلومات_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, جديد TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_معلومات()
{
    أرجع get_Core_Tls_معلومات_TLS().getRef();
}

#إذا defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#جيڪڏهن_وصف_ٿيل WINRT
    #هدايت warning(disable:4447) // Disable warning 'main' signature found without threading model
#جيڪڏهن_پورو

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

خارجي "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    إذا (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        إذا (lpReserved != عدم) // called after ExitProcess() call
        {
            cv::__termination = سچ;
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
#جيڪڏهن_پورو


نطاق {

#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
منطقي overrideThreadName()
{
    ساكن منطقي param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", خطأ);
    أرجع param;
}
#جيڪڏهن_پورو

ساكن صحيح g_thread_انگ = 0;
صنف ThreadID {
عام:
    ثابت صحيح id;
    ThreadID() :
        id(CV_XADD(&g_thread_انگ, 1))
    {
#جيڪڏهن_وصف_ٿيل OPENCV_WITH_ITT
        إذا (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#جيڪڏهن_پورو
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
    std::string bad_قدر;
عام:
    ParseError(ثابت std::string &bad_قدر_2) :bad_قدر(bad_قدر_2) {}
    std::string منزل_String(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_قدر;
        أرجع out.str();
    }
};

سانچو <قسم_نالو T>
T parseOption(ثابت std::string &);

سانچو<>
مضمن منطقي parseOption(ثابت std::string & value)
{
    إذا (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        أرجع سچ;
    }
    إذا (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        أرجع خطأ;
    }
    ارم ParseError(value);
}

سانچو<>
مضمن size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    لكل (; pos < value.size(); pos++)
    {
        إذا (!انگ_آهي(value[pos]))
            اكسر;
    }
    cv::String قدر_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(قدر_Str);
    إذا (suffixStr.length() == 0)
        أرجع v;
    وإلا إذا (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        أرجع v * 1024 * 1024;
    وإلا إذا (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        أرجع v * 1024;
    ارم ParseError(value);
}

سانچو<>
مضمن cv::String parseOption(ثابت std::string &value)
{
    أرجع value;
}

سانچو<>
مضمن utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths نتيجو;
#جيڪڏهن_وصف_ٿيل _WIN32
    ثابت حرف sep = ';';
#وإلا
    ثابت حرف sep = ':';
#جيڪڏهن_پورو
    size_t شروعات_pos = 0;
    طالما (شروعات_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, شروعات_pos);
        ثابت std::string one_piece(value, شروعات_pos, pos == std::string::npos ? pos : pos - شروعات_pos);
        إذا (!one_piece.empty())
            نتيجو.push_back(one_piece);
        شروعات_pos = pos == std::string::npos ? pos : pos + 1;
    }
    أرجع نتيجو;
}

ساكن مضمن ثابت حرف * envRead(ثابت حرف * name)
{
#جيڪڏهن_وصف_ٿيل NO_GETENV
    CV_اڻ_استعمال_ٿيل(name);
    أرجع عدم;
#وإلا
    أرجع getenv(name);
#جيڪڏهن_پورو
}

سانچو<قسم_نالو T>
مضمن T اقرأ(ثابت std::string & k, ثابت T & default_قدر_2)
{
    حاول
    {
        ثابت حرف * res = envRead(k.c_str());
        إذا (res)
            أرجع parseOption<T>(std::string(res));
    }
    امسك (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.منزل_String(k));
    }
    أرجع default_قدر_2;
}

منطقي utils::getConfigurationParameterBool(ثابت حرف* name, منطقي default_قدر_2)
{
    أرجع اقرأ<منطقي>(name, default_قدر_2);
}

size_t utils::get_Configuration_Parameter_سائيز_T(ثابت حرف* name, size_t default_قدر_2)
{
    أرجع اقرأ<size_t>(name, default_قدر_2);
}

std::string utils::getConfigurationParameterString(ثابت حرف* name, ثابت std::string & default_قدر_2)
{
    أرجع اقرأ<cv::String>(name, default_قدر_2);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت حرف* name, ثابت utils::Paths &default_قدر_2)
{
    أرجع اقرأ<utils::Paths>(name, default_قدر_2);
}


#جيڪڏهن_وصف_ٿيل CV_COLLECT_IMPL_معلومات
ImplCollector& get_Impl_معلومات()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, جديد ImplCollector())
}

فراغ سيٽ_Impl(صحيح flags)
{
    cv::Auto_تالو lock(get_Impl_معلومات().mutex);

    get_Impl_معلومات().implFlags = flags;
    get_Impl_معلومات().impl_ڪوڊ.clear();
    get_Impl_معلومات().implFun.clear();
}

فراغ جوڙيو_Impl(صحيح flag, ثابت حرف* func)
{
    cv::Auto_تالو lock(get_Impl_معلومات().mutex);

    get_Impl_معلومات().implFlags |= flag;
    إذا(func) // use lazy collection if name was not specified
    {
        size_t اشاريو = get_Impl_معلومات().impl_ڪوڊ.size();
        إذا(!اشاريو || (get_Impl_معلومات().impl_ڪوڊ[اشاريو-1] != flag || get_Impl_معلومات().implFun[اشاريو-1].compare(func))) // avoid duplicates
        {
            get_Impl_معلومات().impl_ڪوڊ.push_back(flag);
            get_Impl_معلومات().implFun.push_back(func);
        }
    }
}

صحيح getImpl(std::vector<صحيح> &impl, std::vector<String> &funName)
{
    cv::Auto_تالو lock(get_Impl_معلومات().mutex);

    impl    = get_Impl_معلومات().impl_ڪوڊ;
    funName = get_Impl_معلومات().implFun;
    أرجع get_Impl_معلومات().implFlags; // return actual flags for lazy collection
}

منطقي useCollection()
{
    أرجع get_Impl_معلومات().useCollection;
}

فراغ سيٽ_Use_Collection(منطقي flag)
{
    cv::Auto_تالو lock(get_Impl_معلومات().mutex);

    get_Impl_معلومات().useCollection = flag;
}
#جيڪڏهن_پورو

نطاق instr
{
منطقي useInstrumentation()
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    أرجع getInstrumentStruct().useInstr;
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ سيٽ_Use_Instrumentation(منطقي flag)
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#وإلا
    CV_اڻ_استعمال_ٿيل(flag);
#جيڪڏهن_پورو
}

Instr_ڳنڍ* getTrace()
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    أرجع &getInstrumentStruct().پاڙ_ڳنڍ_2;
#وإلا
    أرجع عدم;
#جيڪڏهن_پورو
}

فراغ resetTrace()
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    getInstrumentStruct().پاڙ_ڳنڍ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ڳنڍ = &getInstrumentStruct().پاڙ_ڳنڍ_2;
#جيڪڏهن_پورو
}

فراغ سيٽ_Flags(FLAGS گهڻائي_Flags)
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = گهڻائي_Flags;
#وإلا
    CV_اڻ_استعمال_ٿيل(گهڻائي_Flags);
#جيڪڏهن_پورو
}
FLAGS getFlags()
{
#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
    أرجع (FLAGS)getInstrumentStruct().flags;
#وإلا
    أرجع (FLAGS)0;
#جيڪڏهن_پورو
}

ڳنڍ_معلومات::ڳنڍ_معلومات(ثابت حرف* funName, ثابت حرف* fileName, صحيح سٽ_انگ, فراغ* ret_پتو, منطقي alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_سٽ_انگ       = سٽ_انگ;
    m_ret_پتو    = ret_پتو;
    m_alwaysExpand  = alwaysExpand;

    m_ڌاڳا    = 1;
    m_counter    = 0;
    m_ticks_ڪل = 0;

    m_funError  = خطأ;
}
ڳنڍ_معلومات::ڳنڍ_معلومات(ڳنڍ_معلومات &ref)
{
    *هذا = ref;
}
ڳنڍ_معلومات& ڳنڍ_معلومات::عامل=(ثابت ڳنڍ_معلومات &ساڄي)
{
    هذا->m_funName      = ساڄي.m_funName;
    هذا->m_instrType    = ساڄي.m_instrType;
    هذا->m_implType     = ساڄي.m_implType;
    هذا->m_fileName     = ساڄي.m_fileName;
    هذا->m_سٽ_انگ      = ساڄي.m_سٽ_انگ;
    هذا->m_ret_پتو   = ساڄي.m_ret_پتو;
    هذا->m_alwaysExpand = ساڄي.m_alwaysExpand;

    هذا->m_ڌاڳا     = ساڄي.m_ڌاڳا;
    هذا->m_counter     = ساڄي.m_counter;
    هذا->m_ticks_ڪل  = ساڄي.m_ticks_ڪل;

    هذا->m_funError    = ساڄي.m_funError;

    أرجع *هذا;
}
ڳنڍ_معلومات::~ڳنڍ_معلومات()
{
}
منطقي عامل==(ثابت ڳنڍ_معلومات& کاٻي, ثابت ڳنڍ_معلومات& ساڄي)
{
    إذا(کاٻي.m_سٽ_انگ == ساڄي.m_سٽ_انگ && کاٻي.m_funName == ساڄي.m_funName && کاٻي.m_fileName == ساڄي.m_fileName)
    {
        إذا(کاٻي.m_ret_پتو == ساڄي.m_ret_پتو || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_نالا || کاٻي.m_alwaysExpand))
            أرجع سچ;
    }
    أرجع خطأ;
}

#جيڪڏهن_وصف_ٿيل ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ساكن InstrStruct instr;
    أرجع instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    أرجع *getInstrumentStruct().tlsStruct.get();
}

Instr_ڳنڍ* get_Current_ڳنڍ()
{
    أرجع getInstrumentTLSStruct().p_Current_ڳنڍ;
}

IntrumentationRegion::IntrumentationRegion(ثابت حرف* funName, ثابت حرف* fileName, صحيح سٽ_انگ, فراغ *ret_پتو, منطقي alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = خطأ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    إذا(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        إذا(!pTLS->p_Current_ڳنڍ)
        {
            m_disabled = سچ;
            أرجع;
        }

        صحيح اونهائي = pTLS->p_Current_ڳنڍ->get_اونهائي();
        إذا(pStruct->max_اونهائي && pStruct->max_اونهائي <= اونهائي)
        {
            m_disabled = سچ;
            أرجع;
        }

        ڳنڍ_معلومات payload(funName, fileName, سٽ_انگ, ret_پتو, alwaysExpand, instrType, implType);
        Node<ڳنڍ_معلومات>* p_ٻار = عدم;

        إذا(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_تالو guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_ٻار = pTLS->p_Current_ڳنڍ->ڳوليو_ٻار(payload);
            إذا(!p_ٻار)
            {
                p_ٻار = جديد Node<ڳنڍ_معلومات>(payload);
                pTLS->p_Current_ڳنڍ->جوڙيو_ٻار(p_ٻار);
            }
        }
        وإلا
        {
            p_ٻار = pTLS->p_Current_ڳنڍ->ڳوليو_ٻار(payload);
            إذا(!p_ٻار)
            {
                m_disabled = سچ;
                أرجع;
            }
        }
        pTLS->p_Current_ڳنڍ = p_ٻار;

        m_regionTicks = get_Tick_ڳڻپ();
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

            إذا (pTLS->p_Current_ڳنڍ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ڳنڍ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ڳنڍ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::پڄاڻي_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ڳڻپ() - m_regionTicks);
            {
                cv::Auto_تالو guard(pStruct->mutex_ڳڻپ); // Concurrent ticks accumulation
                pTLS->p_Current_ڳنڍ->m_payload.m_counter++;
                pTLS->p_Current_ڳنڍ->m_payload.m_ticks_ڪل += ticks;
                pTLS->p_Current_ڳنڍ->m_payload.m_tls.get()->m_ticks_ڪل += ticks;
            }

            pTLS->p_Current_ڳنڍ = pTLS->p_Current_ڳنڍ->m_p_والد;
        }
    }
}
#جيڪڏهن_پورو
}

نطاق ipp
{

#جيڪڏهن_وصف_ٿيل HAVE_IPP
بنية IPPInitSingleton
{
عام:
    IPPInitSingleton()
    {
        useIPP         = سچ;

#إذا defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#وإلا
        useIPP_NE      = خطأ;
#جيڪڏهن_پورو
        ipp_حالت      = 0;
        funcname       = عدم;
        filename       = عدم;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_چوٽي_Features = 0;
        pIppLibInfo    = عدم;

        ipp_حالت = ippGetCpuFeatures(&cpuFeatures, عدم);
        إذا(ipp_حالت < 0)
        {
            CV_LOG_ERROR(عدم, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = خطأ;
            أرجع;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        إذا(!env.empty())
        {
#إذا IPP_نسخو_X100 >= 201900
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#وإلاإذا IPP_نسخو_X100 >= 201703
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وإلاإذا IPP_نسخو_X100 >= 201700
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وإلا
            ثابت Ipp64u minorFeatures = 0;
#جيڪڏهن_پورو

            env = منزل_هيٺاهون_Case(env);
            إذا(env.substr(0, 2) == "ne")
            {
                useIPP_NE = سچ;
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
#إذا IPP_نسخو_X100 >= 201700
#إذا defined (_M_AMD64) || defined (__x86_64__)
            وإلا إذا(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#جيڪڏهن_پورو
#جيڪڏهن_پورو
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
#إذا IPP_نسخو_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#جيڪڏهن_پورو
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
#إذا IPP_نسخو_X100 >= 201700
        إذا(ippFeatures&ippCPUID_AVX512F)
        {
            إذا((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_چوٽي_Features = ippCPUID_AVX512_SKX;
            وإلا إذا((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_چوٽي_Features = ippCPUID_AVX512_KNL;
            وإلا
                ipp_چوٽي_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        وإلا
#جيڪڏهن_پورو
        إذا(ippFeatures&ippCPUID_AVX2)
            ipp_چوٽي_Features = ippCPUID_AVX2;
        وإلا إذا(ippFeatures&ippCPUID_SSE42)
            ipp_چوٽي_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_نسخو();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        إذا (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_چوٽي_Features = ippCPUID_SSE42;
        }
    }

عام:
    منطقي        useIPP;
    منطقي        useIPP_NE;

    صحيح         ipp_حالت;  // 0 - all is ok, -1 - IPP functions failed
    ثابت حرف *funcname;
    ثابت حرف *filename;
    صحيح         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_چوٽي_Features;
    ثابت Ipp_Library_نسخو *pIppLibInfo;
};

ساكن IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, جديد IPPInitSingleton())
}
#جيڪڏهن_پورو

بنانشان ڊگهو ڊگهو getIppFeatures()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    أرجع getIPPSingleton().ippFeatures;
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
}

#جيڪڏهن_وصف_ٿيل HAVE_IPP
بنانشان ڊگهو ڊگهو get_Ipp_چوٽي_Features()
{
    أرجع getIPPSingleton().ipp_چوٽي_Features;
}
#جيڪڏهن_پورو

فراغ سيٽ_Ipp_حالت(صحيح حالت, ثابت حرف * ثابت _funcname, ثابت حرف * ثابت _filename, صحيح سٽ_3)
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    getIPPSingleton().ipp_حالت = حالت;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = سٽ_3;
#وإلا
    CV_اڻ_استعمال_ٿيل(حالت); CV_اڻ_استعمال_ٿيل(_funcname); CV_اڻ_استعمال_ٿيل(_filename); CV_اڻ_استعمال_ٿيل(سٽ_3);
#جيڪڏهن_پورو
}

صحيح get_Ipp_حالت()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    أرجع getIPPSingleton().ipp_حالت;
#وإلا
    أرجع 0;
#جيڪڏهن_پورو
}

String getIppErrorLocation()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    أرجع format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#وإلا
    أرجع String();
#جيڪڏهن_پورو
}

String get_Ipp_نسخو()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    ثابت Ipp_Library_نسخو *pInfo = getIPPSingleton().pIppLibInfo;
    إذا(pInfo)
        أرجع format("%s %s %s", pInfo->Name, pInfo->نسخو_4, pInfo->BuildDate);
    وإلا
        أرجع String("error");
#وإلا
    أرجع String("disabled");
#جيڪڏهن_پورو
}

منطقي useIPP()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    إذا (معلومات.useIPP < 0)
    {
        معلومات.useIPP = getIPPSingleton().useIPP;
    }
    أرجع (معلومات.useIPP > 0);
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ سيٽ_Use_IPP(منطقي flag)
{
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    معلومات.useIPP = (getIPPSingleton().useIPP)?flag:خطأ;
#وإلا
    CV_اڻ_استعمال_ٿيل(flag);
    معلومات.useIPP = خطأ;
#جيڪڏهن_پورو
}

منطقي useIPP_NotExact()
{
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
    إذا (معلومات.useIPP_NE < 0)
    {
        معلومات.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    أرجع (معلومات.useIPP_NE > 0);
#وإلا
    أرجع خطأ;
#جيڪڏهن_پورو
}

فراغ سيٽ_Use_IPP_Not_Exact(منطقي flag)
{
    CoreTLSData& معلومات = get_Core_Tls_معلومات();
#جيڪڏهن_وصف_ٿيل HAVE_IPP
    معلومات.useIPP_NE = flag;
#وإلا
    CV_اڻ_استعمال_ٿيل(flag);
    معلومات.useIPP_NE = خطأ;
#جيڪڏهن_پورو
}

} // namespace ipp


نطاق details {

#إذا OPENCV_IMPL_FP_HINTS_X86
#جيڪڏهن_وصف_نه_ٿيل _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#وصف_ڪريو _MM_DENORMALS_ZERO_ON 0x0040
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل MM_DENORMALS_ZERO_نقاب  // requires pmmintrin.h (SSE3)
#وصف_ڪريو MM_DENORMALS_ZERO_نقاب 0x0040
#جيڪڏهن_پورو
#جيڪڏهن_پورو

فراغ سيٽ_FPDenormals_Ignore_Hint(منطقي ignore, CV_ٻاهريون FPDenormals_گهڻائي_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    بنانشان نقاب = MM_FLUSH_ZERO_نقاب;
    بنانشان value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    إذا (featuresEnabled.have[CPU_SSE3])
    {
        نقاب |= MM_DENORMALS_ZERO_نقاب;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ثابت بنانشان old_flags = _mm_getcsr();
    ثابت بنانشان old_قدر = old_flags & نقاب;
    بنانشان flags = (old_flags & ~نقاب) | value;
    CV_LOG_DEBUG(عدم, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)نقاب;
    state.reserved[1] = (uint32_t)old_قدر;
    _mm_setcsr(flags);
#وإلا
    CV_اڻ_استعمال_ٿيل(ignore); CV_اڻ_استعمال_ٿيل(state);
#جيڪڏهن_پورو
}

صحيح saveFPDenormalsState(CV_ٻاهريون FPDenormals_گهڻائي_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    بنانشان نقاب = MM_FLUSH_ZERO_نقاب;
    إذا (featuresEnabled.have[CPU_SSE3])
    {
        نقاب |= MM_DENORMALS_ZERO_نقاب;
    }
    ثابت بنانشان old_flags = _mm_getcsr();
    ثابت بنانشان old_قدر = old_flags & نقاب;
    // save state
    state.reserved[0] = (uint32_t)نقاب;
    state.reserved[1] = (uint32_t)old_قدر;
    أرجع 2;
#وإلا
    CV_اڻ_استعمال_ٿيل(state);
    أرجع 0;
#جيڪڏهن_پورو
}

منطقي restoreFPDenormalsState(ثابت FPDenormals_گهڻائي_State& state)
{
#إذا OPENCV_IMPL_FP_HINTS_X86
    ثابت بنانشان نقاب = (بنانشان)state.reserved[0];
    CV_DbgAssert(نقاب != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت بنانشان value = (بنانشان)state.reserved[1];
    CV_DbgCheck((صحيح)value, value == (value & نقاب), "invalid SSE FP state");
    ثابت بنانشان old_flags = _mm_getcsr();
    بنانشان flags = (old_flags & ~نقاب) | value;
    CV_LOG_DEBUG(عدم, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    أرجع سچ;
#وإلا
    CV_اڻ_استعمال_ٿيل(state);
    أرجع خطأ;
#جيڪڏهن_پورو
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#جيڪڏهن_وصف_ٿيل OPENCV_ALGO_HINT_DEFAULT
    أرجع OPENCV_ALGO_HINT_DEFAULT;
#وإلا
    أرجع ALGO_HINT_ACCURATE;
#جيڪڏهن_پورو
};

} // namespace cv

/* End of file. */
