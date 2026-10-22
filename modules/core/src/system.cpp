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

#اگر_تعریف_شدہ __QNX__
    #شامل <unistd.h>
    #شامل <sys/neutrino.h>
    #شامل <sys/syspage.h>
#اگر_تعریف_شدہ __aarch64__
    #شامل <aarch64/syspage.h>
#اگر_ختم
#اگر_ختم

#شامل <opencv2/core/utils/configuration.private.hpp>
#شامل <opencv2/core/utils/trace.private.hpp>

#شامل <opencv2/core/utils/logger.hpp>

#شامل <opencv2/core/utils/tls.hpp>
#شامل <opencv2/core/utils/instrumentation.hpp>

#شامل <opencv2/core/utils/filesystem.private.hpp>

#شامل <opencv2/core/utils/fp_control_utils.hpp>
#شامل <opencv2/core/utils/fp_control.private.hpp>

نامخلا cv {

ساکن خالی _initSystem()
{
#اگر_تعریف_شدہ __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ساکن std::ios_base::Init s_iostream_initializer;
#اگر_ختم
}

ساکن Mutex* __initialization_mutex = تہی;
Mutex& getInitializationMutex()
{
    اگر (__initialization_mutex == تہی)
    {
        (خالی)_initSystem();
        __initialization_mutex = نیا Mutex();
    }
    واپس *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ساکن منطقی param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#اگر defined(_DEBUG) || defined(__ANDROID__)
    سچ
#ورنہ
    جھوٹ
#اگر_ختم
);

خالی* alloc_Singleton_بفر(size_t size) { واپس fastMalloc(size); }
خالی* alloc_Singleton_New_بفر(size_t size) { واپس حافظہ_دو(size); }


} // namespace cv

#اگر_غیر_تعریف_شدہ CV_ERROR_سیٹ_TERMINATE_HANDLER  // build config option
# اگر defined(_WIN32)
#   define CV_ERROR_سیٹ_TERMINATE_HANDLER 1
# endif
#اگر_ختم
#اگر defined(CV_ERROR_سیٹ_TERMINATE_HANDLER) && !CV_ERROR_سیٹ_TERMINATE_HANDLER
# undef CV_ERROR_سیٹ_TERMINATE_HANDLER
#اگر_ختم

#اگر_تعریف_شدہ _MSC_VER
# اگر _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#اگر_ختم

#اگر_تعریف_شدہ CV_ERROR_سیٹ_TERMINATE_HANDLER
#شامل <cstdlib>        // std::abort
#اگر_ختم

#اگر defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#اگر defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
استعمال Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ثابت uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ورنہ
#  include <elf.h>
#اگر_ختم
#اگر defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#اگر_ختم
#اگر_ختم

#اگر defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#اگر_ختم


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
#اگر_ختم

#اگر defined __loongarch64
#شامل "sys/auxv.h"
#تعریف_کرو LA_HWCAP_LSX   (1<<4)
#تعریف_کرو LA_HWCAP_LASX  (1<<5)
#اگر_ختم

#اگر defined _WIN32 || defined WINCE
#اگر_غیر_تعریف_شدہ _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #تعریف_کرو _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#اگر_ختم
#شامل <windows.h>
#شامل <combaseapi.h>
#اگر (_WIN32_WINNT >= 0x0602)
  #شامل <synchapi.h>
#اگر_ختم
#اگر ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #شامل <fibersapi.h>
  #تعریف_کرو CV_USE_FLS
#اگر_ختم
#تعریف_منسوخ small
#تعریف_منسوخ min
#تعریف_منسوخ max
#تعریف_منسوخ مطلق_قدر
#شامل <tchar.h>

#اگر_تعریف_شدہ WINRT
#شامل <wrl/client.h>
#اگر_غیر_تعریف_شدہ __cplusplus_winrt
#شامل <windows.storage.h>
#ہدایت comment(lib, "runtimeobject.lib")
#اگر_ختم // WINRT

std::wstring Get_عارضی_راستہ_Win_RT()
{
#اگر_تعریف_شدہ __cplusplus_winrt
    واپس std::wstring(Windows::Storage::Application_ڈیٹا::Current->TemporaryFolder->راستہ_2->ڈیٹا_4());
#ورنہ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ڈیٹا_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ڈیٹا> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_شے> storageitemRef;
    HSTRING str;
    HSTRING_سرخی hstr_سر;
    std::wstring wstr;
    اگر (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_ڈیٹا,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_ڈیٹا), &hstr_سر, &str)))
        واپس wstr;
    اگر (FAILED(RoGetActivationFactory(str, IID_PPV_دلائل(appdataFactory.Release_And_Get_پتہ_کا()))))
        واپس wstr;
    اگر (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_پتہ_کا())))
        واپس wstr;
    اگر (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_پتہ_کا())))
        واپس wstr;
    اگر (FAILED(storagefolderRef.As(&storageitemRef)))
        واپس wstr;
    str = تہی;
    اگر (FAILED(storageitemRef->get_راستہ(&str)))
        واپس wstr;
    wstr = Windows_Get_String_Raw_بفر(str, تہی);
    WindowsDeleteString(str);
    واپس wstr;
#اگر_ختم
}

std::wstring Get_عارضی_File_Name_Win_RT(std::wstring سابقہ)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* نقاب = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], حجم(guidStr)/حجم(wchar_t), نقاب,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    واپس سابقہ.append(std::wstring(guidStr));
}

#اگر_ختم
#ورنہ
#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT
#شامل <pthread.h>
#اگر_ختم
#شامل <sys/time.h>
#شامل <time.h>

#اگر defined __MACH__ && defined __APPLE__
#شامل <mach/mach.h>
#شامل <mach/mach_time.h>
#شامل <sys/sysctl.h>
#اگر_ختم

#اگر_ختم

#اگر_تعریف_شدہ _OPENMP
#شامل "omp.h"
#اگر_ختم

#اگر defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#شامل <unistd.h>
#شامل <stdio.h>
#شامل <sys/types.h>
#اگر defined __ANDROID__
#شامل <sys/sysconf.h>
#اگر_ختم
#اگر_ختم

#اگر_تعریف_شدہ __ANDROID__
# include <android/log.h>
#اگر_ختم

#اگر_تعریف_شدہ DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#اگر_ختم
#اگر_غیر_تعریف_شدہ CV_CPUID_X86
  #اگر defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #اگر _MSC_VER >= 1400  // MSVS 2005
      #شامل <intrin.h>  // __cpuidex()
      #تعریف_کرو CV_CPUID_X86 __cpuidex
    #ورنہ
      #غلطی "Required MSVS 2005+"
    #اگر_ختم
  #وگرنہ_اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ساکن خالی cv_cpuid(صحیح* cpuid_ڈیٹا, صحیح reg_eax, صحیح reg_ecx)
    {
        صحیح __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#اگر !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#وگرنہ_اگر defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ورنہ
#غلطی "Configuration error"
#اگر_ختم
        cpuid_ڈیٹا[0] = __eax; cpuid_ڈیٹا[1] = __ebx; cpuid_ڈیٹا[2] = __ecx; cpuid_ڈیٹا[3] = __edx;
    }
    #تعریف_کرو CV_CPUID_X86 cv_cpuid
  #اگر_ختم
#اگر_ختم

#شامل <chrono>

نامخلا cv
{

Exception::Exception() { رمز = Error::StsOk; سطر = 0; }

Exception::Exception(Error::رمز_3 رمز_4, ثابت String& _err, ثابت String& _func, ثابت String& _file, صحیح سطر_3)
: رمز(رمز_4), err(_err), func(_func), file(_file), سطر(سطر_3)
{
    format_پیغام();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ثابت حرف* Exception::what() ثابت CV_NOEXCEPT { واپس پیغام.c_str(); }

خالی Exception::format_پیغام()
{
    size_t pos = err.find('\n');
    منطقی multiline = pos != cv::String::npos;
    اگر (multiline)
    {
        std::stringstream ss;
        size_t پچھلا_pos = 0;
        جب (pos != cv::String::npos)
        {
           ss << "> " << err.substr(پچھلا_pos, pos - پچھلا_pos) << std::سطر_ختم;
           پچھلا_pos = pos + 1;
           pos = err.find('\n', پچھلا_pos);
        }
        ss << "> " << err.substr(پچھلا_pos);
        اگر (err[err.size() - 1] != '\n')
            ss << std::سطر_ختم;
        err = ss.str();
    }
    اگر (func.size() > 0)
    {
        اگر (multiline)
            پیغام = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_نسخہ, file.c_str(), سطر, رمز, رمز_پیغام(), func.c_str(), err.c_str());
        ورنہ
            پیغام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_نسخہ, file.c_str(), سطر, رمز, رمز_پیغام(), err.c_str(), func.c_str());
    }
    ورنہ
    {
        پیغام = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_نسخہ, file.c_str(), سطر, رمز, رمز_پیغام(), err.c_str(), multiline ? "" : "\n");
    }
}

ثابت حرف * Exception::رمز_پیغام() ثابت
{
    انتخاب (رمز)
    {
    حالت cv::Error::StsOk :                  واپس "No Error";
    حالت cv::Error::Sts_عقب_Trace :           واپس "Backtrace";
    حالت cv::Error::StsError :               واپس "Unspecified error";
    حالت cv::Error::Sts_اندرونی :            واپس "Internal error";
    حالت cv::Error::StsNoMem :               واپس "Insufficient memory";
    حالت cv::Error::StsBadArg :              واپس "Bad argument";
    حالت cv::Error::StsNoConv :              واپس "Iterations do not converge";
    حالت cv::Error::StsAutoTrace :           واپس "Autotrace call";
    حالت cv::Error::Sts_Bad_سائز :             واپس "Incorrect size of input array";
    حالت cv::Error::StsNullPtr :             واپس "Null pointer";
    حالت cv::Error::StsDivByZero :           واپس "Division by zero occurred";
    حالت cv::Error::Bad_قدم :                واپس "Image step is wrong";
    حالت cv::Error::StsInplaceNotSupported : واپس "Inplace operation is not supported";
    حالت cv::Error::Sts_Object_Not_ملا :      واپس "Requested object was not found";
    حالت cv::Error::Bad_گہرائی :               واپس "Input image depth is not supported by function";
    حالت cv::Error::StsUnmatchedFormats :    واپس "Formats of input arguments do not match";
    حالت cv::Error::StsUnmatchedSizes :      واپس "Sizes of input arguments do not match";
    حالت cv::Error::Sts_برآمد_کا_Range :          واپس "One of the arguments\' values is out of range";
    حالت cv::Error::StsUnsupportedFormat :   واپس "Unsupported format or combination of formats";
    حالت cv::Error::BadCOI :                 واپس "Input COI is not supported";
    حالت cv::Error::Bad_عدد_Channels :         واپس "Bad number of channels";
    حالت cv::Error::StsBadFlag :             واپس "Bad flag (parameter or structure field)";
    حالت cv::Error::Sts_Bad_نقطہ :            واپس "Bad parameter of type CvPoint";
    حالت cv::Error::Sts_Bad_نقاب :             واپس "Bad type of mask argument";
    حالت cv::Error::StsParseError :          واپس "Parsing error";
    حالت cv::Error::StsNotImplemented :      واپس "The function/feature is not implemented";
    حالت cv::Error::Sts_Bad_Mem_بلاک :         واپس "Memory block has been corrupted";
    حالت cv::Error::StsAssert :              واپس "Assertion failed";
    حالت cv::Error::GpuNotSupported :        واپس "No CUDA support";
    حالت cv::Error::GpuApiCallError :        واپس "Gpu API call";
    حالت cv::Error::OpenGlNotSupported :     واپس "No OpenGL support";
    حالت cv::Error::OpenGlApiCallError :     واپس "OpenGL API call";
    حالت cv::Error::OpenCLApiCallError :     واپس "OpenCL API call error";
    حالت cv::Error::OpenCLDoubleNotSupported:واپس "OpenCL device does not support double";
    حالت cv::Error::OpenCLInitError :        واپس "OpenCL initialization error";
    حالت cv::Error::OpenCLNoAMDBlasFft :     واپس "Missing OpenCL AMD BLAS FFT";
    طےشدہ:
    {
        ساکن حرف buf[256] {0};
        snprintf(buf, حجم(buf), "Unknown %s code %d", رمز >= 0 ? "status":"error", رمز);
        واپس buf;
    }
    };

}

ساکن ثابت حرف* g_hw_Feature_نام[CV_HARDWARE_MAX_FEATURE] = { تہی };

ساکن ثابت حرف* getHWFeatureName(صحیح id)
{
    واپس (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_نام[id] : تہی;
}
ساکن ثابت حرف* get_HWFeature_Name_محفوظ(صحیح id)
{
    ثابت حرف* name = getHWFeatureName(id);
    واپس name ? name : "Unknown feature";
}

ساخت HWFeatures
{
    شمار { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(منطقی تسلسل_initialize = جھوٹ)
    {
        اگر (تسلسل_initialize)
            initialize();
    }

    ساکن خالی initialize_نام()
    {
        برائے (صحیح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_نام[i] = 0;
        }
        g_hw_Feature_نام[CPU_MMX] = "MMX";
        g_hw_Feature_نام[CPU_SSE] = "SSE";
        g_hw_Feature_نام[CPU_SSE2] = "SSE2";
        g_hw_Feature_نام[CPU_SSE3] = "SSE3";
        g_hw_Feature_نام[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_نام[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_نام[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_نام[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_نام[CPU_FP16] = "FP16";
        g_hw_Feature_نام[CPU_AVX] = "AVX";
        g_hw_Feature_نام[CPU_AVX2] = "AVX2";
        g_hw_Feature_نام[CPU_FMA3] = "FMA3";

        g_hw_Feature_نام[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_نام[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_نام[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_نام[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_نام[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_نام[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_نام[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_نام[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_نام[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_نام[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_نام[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_نام[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_نام[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_نام[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_نام[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_نام[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_نام[CPU_NEON] = "NEON";
        g_hw_Feature_نام[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_نام[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_نام[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_نام[CPU_SVE] = "SVE";

        g_hw_Feature_نام[CPU_VSX] = "VSX";
        g_hw_Feature_نام[CPU_VSX3] = "VSX3";

        g_hw_Feature_نام[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_نام[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_نام[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_نام[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_نام[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_نام[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_نام[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_نام[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_نام[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_نام[CPU_RVV] = "RVV";
        g_hw_Feature_نام[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_نام[CPU_LSX]  = "LSX";
        g_hw_Feature_نام[CPU_LASX] = "LASX";
    }

    خالی initialize(خالی)
    {
        اگر (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_نام();

    #اگر_تعریف_شدہ CV_CPUID_X86
        صحیح cpuid_ڈیٹا[4] = { 0, 0, 0, 0 };
        صحیح cpuid_ڈیٹا_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_ڈیٹا, 1, 0/*غیر_مستعمل*/);

        صحیح x86_family = (cpuid_ڈیٹا[0] >> 8) & 15;
        اگر( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_ڈیٹا[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_ڈیٹا[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_ڈیٹا[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_ڈیٹا[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_ڈیٹا[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_ڈیٹا[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_ڈیٹا[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_ڈیٹا[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_ڈیٹا[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_ڈیٹا[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_ڈیٹا[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_ڈیٹا_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_ڈیٹا_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_ڈیٹا_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_ڈیٹا_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_ڈیٹا_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_ڈیٹا_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_ڈیٹا_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_ڈیٹا_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_ڈیٹا_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_ڈیٹا_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_ڈیٹا_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_ڈیٹا_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_ڈیٹا_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_ڈیٹا_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_ڈیٹا_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_ڈیٹا_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_ڈیٹا_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            صحیح cpuid_ڈیٹا_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_ڈیٹا_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_ڈیٹا_ex1[0] & (1<<4))  != 0;

            منطقی have_AVX_OS_support = سچ;
            منطقی have_AVX512_OS_support = سچ;
            اگر (!(cpuid_ڈیٹا[2] & (1<<27)))
                have_AVX_OS_support = جھوٹ; // OS uses XSAVE_XRSTORE and CPU support AVX
            ورنہ
            {
                صحیح xcr0 = 0;
            #اگر_تعریف_شدہ XCR_XFEATURE_ENABLED_نقاب // requires immintrin.h
                xcr0 = (صحیح)_xgetbv(XCR_XFEATURE_ENABLED_نقاب);
            #وگرنہ_اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #اگر_ختم
                اگر ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = جھوٹ; // YMM registers
                اگر ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = جھوٹ; // ZMM registers
            }

            اگر (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = جھوٹ;
                have[CV_CPU_FP16] = جھوٹ;
                have[CV_CPU_AVX2] = جھوٹ;
                have[CV_CPU_FMA3] = جھوٹ;
                have[CV_CPU_AVX_VNNI] = جھوٹ;
            }
            اگر (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = جھوٹ;
                have[CV_CPU_AVX_512BW] = جھوٹ;
                have[CV_CPU_AVX_512CD] = جھوٹ;
                have[CV_CPU_AVX_512DQ] = جھوٹ;
                have[CV_CPU_AVX_512ER] = جھوٹ;
                have[CV_CPU_AVX_512IFMA] = جھوٹ;
                have[CV_CPU_AVX_512PF] = جھوٹ;
                have[CV_CPU_AVX_512VBMI] = جھوٹ;
                have[CV_CPU_AVX_512VL] = جھوٹ;
                have[CV_CPU_AVX_512VBMI2] = جھوٹ;
                have[CV_CPU_AVX_512VNNI] = جھوٹ;
                have[CV_CPU_AVX_512BITALG] = جھوٹ;
                have[CV_CPU_AVX_512VPOPCNTDQ] = جھوٹ;
                have[CV_CPU_AVX_5124VNNIW] = جھوٹ;
                have[CV_CPU_AVX_5124FMAPS] = جھوٹ;
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
            ورنہ
            {
                have[CV_CPU_AVX512_KNL] = جھوٹ;
                have[CV_CPU_AVX512_KNM] = جھوٹ;
                have[CV_CPU_AVX512_SKX] = جھوٹ;
                have[CV_CPU_AVX512_CNL] = جھوٹ;
                have[CV_CPU_AVX512_CLX] = جھوٹ;
                have[CV_CPU_AVX512_ICL] = جھوٹ;
            }
        }
    #اگر_ختم // CV_CPUID_X86

    #اگر defined __ANDROID__ || defined __linux__ || defined __QNX__
    #اگر_تعریف_شدہ __aarch64__
        have[CV_CPU_NEON] = سچ;
        have[CV_CPU_FP16] = سچ;
        صحیح cpufile = open("/proc/self/auxv", O_RDONLY);

        اگر (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ثابت size_t سائز_auxv_t = حجم(auxv);

            جب ((size_t)اقرأ(cpufile, &auxv, سائز_auxv_t) == سائز_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                اگر (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#اگر defined(AT_HWCAP2)
                ورنہ اگر (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#اگر_ختم
            }

            close(cpufile);
        }
    #وگرنہ_اگر defined __arm__ && defined __ANDROID__
      #اگر defined HAVE_CPUFEATURES
        CV_LOG_INFO(تہی, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(تہی, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (طویل طویل)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ورنہ
        CV_LOG_INFO(تہی, "cpufeatures library is not available for CPU detection");
        #اگر CV_NEON
        CV_LOG_INFO(تہی, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = سچ;
        #ورنہ
        CV_LOG_INFO(تہی, "- NEON instructions is NOT enabled via build flags");
        #اگر_ختم
        #اگر CV_FP16
        CV_LOG_INFO(تہی, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = سچ;
        #ورنہ
        CV_LOG_INFO(تہی, "- FP16 instructions is NOT enabled via build flags");
        #اگر_ختم
      #اگر_ختم
    #وگرنہ_اگر defined __arm__
        صحیح cpufile = open("/proc/self/auxv", O_RDONLY);

        اگر (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ثابت size_t سائز_auxv_t = حجم(auxv);

            جب ((size_t)اقرأ(cpufile, &auxv, سائز_auxv_t) == سائز_auxv_t)
            {
                اگر (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    توڑو;
                }
            }

            close(cpufile);
        }
    #اگر_ختم
    #وگرنہ_اگر (defined __APPLE__)
    #اگر defined __ARM_NEON
        have[CV_CPU_NEON] = سچ;
    #اگر_ختم
    #اگر (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = سچ;
    #اگر_ختم
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    صحیح has_feat_dotprod = 0;
    size_t has_feat_dotprod_سائز = حجم(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_سائز, تہی, 0);
    اگر (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = سچ;
    }
    صحیح has_feat_bf16 = 0;
    size_t has_feat_bf16_سائز = حجم(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_سائز, تہی, 0);
    اگر (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = سچ;
    }
    #وگرنہ_اگر (defined __clang__)
    #اگر defined __ARM_NEON
        have[CV_CPU_NEON] = سچ;
        #اگر (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = سچ;
        #اگر_ختم
    #اگر_ختم
    #اگر_ختم
    #اگر defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = سچ;
    #اگر_ختم
    #اگر defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = سچ;
    #اگر_ختم
    #اگر_تعریف_شدہ __riscv_vector
        have[CV_CPU_RISCVV] = سچ;
    #اگر_ختم
    #اگر_تعریف_شدہ __mips_msa
        have[CV_CPU_MSA] = سچ;
    #اگر_ختم

    #اگر (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        غیرعلامتی صحیح hwcap = getauxval(AT_HWCAP);
        اگر (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            اگر (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = سچ;
            } ورنہ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #وگرنہ_اگر (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        غیرعلامتی طویل hwcap = 0, hwcap2 = 0;
        اگر (elf_aux_info(AT_HWCAP, &hwcap, حجم(hwcap)) != 0)
            hwcap = 0;
        اگر (hwcap & PPC_FEATURE_HAS_VSX) {
            اگر (elf_aux_info(AT_HWCAP2, &hwcap2, حجم(hwcap2)) != 0)
                hwcap2 = 0;
            اگر (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = سچ;
            } ورنہ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ورنہ
        // TODO: AIX
        #اگر CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = سچ;
        #اگر_ختم
        #اگر CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = سچ;
        #اگر_ختم
    #اگر_ختم

    #اگر defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = سچ;
        #اگر (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = سچ;
        #اگر_ختم
        #اگر defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = سچ;
        #اگر_ختم
    #اگر_ختم

    #اگر defined __loongarch64 && defined __linux__
        صحیح flag = (صحیح)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #اگر_ختم

        منطقی skip_baseline_check = جھوٹ;
        اگر (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = سچ;
        }
        صحیح baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        اگر (!checkFeatures(baseline_features, حجم(baseline_features) / حجم(baseline_features[0]))
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

    منطقی checkFeatures(ثابت صحیح* features, صحیح گنتی_2, منطقی dump = جھوٹ)
    {
        منطقی نتیجہ = سچ;
        برائے (صحیح i = 0; i < گنتی_2; i++)
        {
            صحیح feature = features[i];
            اگر (feature)
            {
                اگر (have[feature])
                {
                    اگر (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_محفوظ(feature));
                }
                ورنہ
                {
                    نتیجہ = جھوٹ;
                    اگر (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_محفوظ(feature));
                }
            }
        }
        واپس نتیجہ;
    }

    ساکن اندرونی منطقی is_علامت_Separator(حرف c)
    {
        واپس c == ',' || c == ';';
    }

    خالی readSettings(ثابت صحیح* baseline_features, صحیح baseline_گنتی)
    {
        منطقی dump = سچ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        اگر (!disabled_features.empty())
        {
            ثابت حرف* آغاز = disabled_features.c_str();
            برائے (;;)
            {
                جب (آغاز[0] != 0 && is_علامت_Separator(آغاز[0]))
                {
                    آغاز++;
                }
                اگر (آغاز[0] == 0)
                    توڑو;
                ثابت حرف* end = آغاز;
                جب (end[0] != 0 && !is_علامت_Separator(end[0]))
                {
                    end++;
                }
                اگر (end == آغاز)
                    جاری;
                cv::String feature(آغاز, end);
                آغاز = end;

                CV_Assert(feature.size() > 0);

                منطقی ملا = جھوٹ;
                برائے (صحیح i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    اگر (!g_hw_Feature_نام[i]) جاری;
                    size_t len = لڑی_لمبائی(g_hw_Feature_نام[i]);
                    اگر (len != feature.size()) جاری;
                    اگر (feature.compare(g_hw_Feature_نام[i]) == 0)
                    {
                        منطقی isBaseline = جھوٹ;
                        برائے (صحیح k = 0; k < baseline_گنتی; k++)
                        {
                            اگر (baseline_features[k] == i)
                            {
                                isBaseline = سچ;
                                توڑو;
                            }
                        }
                        اگر (isBaseline)
                        {
                            اگر (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_محفوظ(i));
                        }
                        اگر (!have[i])
                        {
                            اگر (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_محفوظ(i));
                        }
                        have[i] = جھوٹ;

                        ملا = سچ;
                        توڑو;
                    }
                }
                اگر (!ملا)
                {
                    اگر (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    منطقی have[MAX_FEATURE+1]{};
};

ساکن HWFeatures  featuresEnabled(سچ), featuresDisabled = HWFeatures(جھوٹ);
ساکن HWFeatures* currentFeatures = &featuresEnabled;

منطقی checkHardwareSupport(صحیح feature)
{
    اگر (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        واپس جھوٹ;
    واپس currentFeatures->have[feature];
}

String getHardwareFeatureName(صحیح feature)
{
    ثابت حرف* name = getHWFeatureName(feature);
    واپس name ? String(name) : String();
}

std::string get_CPUFeatures_سطر()
{
    ثابت صحیح features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ثابت صحیح sz = حجم(features) / حجم(features[0]);
    std::string نتیجہ;
    std::string سابقہ;
    برائے (صحیح i = 1; i < sz; ++i)
    {
        اگر (features[i] == 0)
        {
            سابقہ = "*";
            جاری;
        }
        اگر (i != 1) نتیجہ.append(" ");
        نتیجہ.append(سابقہ);
        نتیجہ.append(get_HWFeature_Name_محفوظ(features[i]));
        اگر (!checkHardwareSupport(features[i])) نتیجہ.append("?");
    }
    واپس نتیجہ;
}

متلون منطقی useOptimizedFlag = سچ;

خالی سیٹ_Use_Optimized( منطقی flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::سیٹ_Use_IPP(flag);
#اگر_تعریف_شدہ HAVE_OPENCL
    ocl::سیٹ_Use_Open_CL(flag);
#اگر_ختم
}

منطقی useOptimized(خالی)
{
    واپس useOptimizedFlag;
}

int64 get_Tick_گنتی(خالی)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    واپس (int64)now.time_since_epoch().گنتی_2();
}

دوہرا getTickFrequency(خالی)
{
    استعمال clock_period_t = std::chrono::steady_clock::duration::period;
    دوہرا clock_freq = clock_period_t::den / clock_period_t::num;
    واپس clock_freq;
}

#اگر defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#اگر defined(__i386__)

int64 get_CPUTick_گنتی(خالی)
{
    int64 x;
    __asm__ متلون (".byte 0x0f, 0x31" : "=A" (x));
    واپس x;
}
#وگرنہ_اگر defined(__x86_64__)

int64 get_CPUTick_گنتی(خالی)
{
    غیرعلامتی hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    واپس (int64)lo | ((int64)hi << 32);
}

#وگرنہ_اگر defined(__ppc__)

int64 get_CPUTick_گنتی(خالی)
{
    غیرعلامتی upper, lower, tmp;
    __asm__ متلون(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    واپس lower | ((int64)upper << 32);
}

#ورنہ

#غلطی "RDTSC not defined"

#اگر_ختم

#وگرنہ_اگر defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_گنتی(خالی)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ورنہ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_گنتی(خالی)
{
    واپس get_Tick_گنتی();
}
//#endif

#اگر_ختم


نامخلا internal {

جماعت Timestamp
{
عوامی:
    ثابت int64 zero_Tick_گنتی;
    ثابت دوہرا ns_اندر_ticks;

    Timestamp()
        : zero_Tick_گنتی(get_Tick_گنتی())
        , ns_اندر_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_گنتی();
        واپس (int64)((t - zero_Tick_گنتی) * ns_اندر_ticks);
    }

    ساکن Timestamp& getInstance()
    {
        ساکن Timestamp g_timestamp;
        واپس g_timestamp;
    }
};

جماعت InitTimestamp {
عوامی:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ساکن InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    واپس internal::Timestamp::getInstance().getTimestamp();
}


ثابت String& getBuildInformation()
{
    ساکن String build_info =
#شامل "version_string.inc"
    ;
    واپس build_info;
}

String get_نسخہ_String() { واپس String(CV_نسخہ); }

صحیح get_نسخہ_Major() { واپس CV_نسخہ_MAJOR; }

صحیح get_نسخہ_Minor() { واپس CV_نسخہ_MINOR; }

صحیح get_نسخہ_Revision() { واپس CV_نسخہ_REVISION; }

String format( ثابت حرف* fmt, ... )
{
    Auto_بفر<حرف, 1024> buf;

    برائے ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        صحیح bsize = static_cast<صحیح>(buf.size());
        صحیح len = cv_vsnprintf(buf.ڈیٹا(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        اگر (len >= bsize)
        {
            buf.resize(len + 1);
            جاری;
        }
        buf[bsize - 1] = 0;
        واپس String(buf.ڈیٹا(), len);
    }
}

String tempfile( ثابت حرف* suffix )
{
#اگر OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string عارضی_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#اگر defined _WIN32
#اگر_تعریف_شدہ WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring عارضی_dir_rt = Get_عارضی_راستہ_Win_RT();

    std::wstring عارضی_file = Get_عارضی_File_Name_Win_RT(L"ocv");
    اگر (عارضی_file.empty())
        واپس String();

    عارضی_file = عارضی_dir_rt.append(std::wstring(L"\\")).append(عارضی_file);
    DeleteFileW(عارضی_file.c_str());

    حرف aname[MAX_راستہ];
    size_t copied = wcstombs(aname, عارضی_file.c_str(), MAX_راستہ);
    CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#وگرنہ_اگر defined(_WIN32_WCE)
    ثابت خودکار k_Max_راستہ_سائز = MAX_راستہ+1;
    wchar_t عارضی_dir_ce[k_Max_راستہ_سائز] = {0};
    wchar_t عارضی_file[k_Max_راستہ_سائز] = {0};

    ::Get_عارضی_راستہ_W(k_Max_راستہ_سائز, عارضی_dir_ce);

    اگر(0 != ::Get_عارضی_File_Name_W(عارضی_dir_ce, L"ocv", 0, عارضی_file)) {
        DeleteFileW(عارضی_file);
        حرف aname[MAX_راستہ];
        size_t copied = wcstombs(aname, عارضی_file, MAX_راستہ);
        CV_Assert((copied != MAX_راستہ) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ورنہ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t عارضی_dir2_w[MAX_راستہ] = { 0 };

    اگر (عارضی_dir.empty())
    {
        ::Get_عارضی_راستہ_W(حجم(عارضی_dir2_w)/حجم(wchar_t), عارضی_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        صحیح len = Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, عارضی_dir2_w, -1, تہی, 0, تہی, تہی);
        اگر (len > 0)
        {
            std::vector<حرف> utf8_buf(len);
            Wide_Char_مقصد_Multi_Byte(CP_UTF8, 0, عارضی_dir2_w, -1, utf8_buf.ڈیٹا(), len, تہی, تہی);
            عارضی_dir = std::string(utf8_buf.ڈیٹا());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    اگر (FAILED(hr))
        واپس String();
    حرف guidStr[40];
    ثابت حرف* نقاب = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, حجم(guidStr), نقاب,
            g.Data1, g.Data2, g.Data3, (غیرعلامتی صحیح)g.Data4[0], (غیرعلامتی صحیح)g.Data4[1],
            (غیرعلامتی صحیح)g.Data4[2], (غیرعلامتی صحیح)g.Data4[3], (غیرعلامتی صحیح)g.Data4[4],
            (غیرعلامتی صحیح)g.Data4[5], (غیرعلامتی صحیح)g.Data4[6], (غیرعلامتی صحیح)g.Data4[7]);

    fname = عارضی_dir;
    اگر (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#اگر_ختم
# ورنہ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    حرف defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ورنہ
    حرف defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    اگر (عارضی_dir.empty())
        fname = defaultTemplate;
    ورنہ
    {
        fname = عارضی_dir;
        حرف ech = fname[fname.size() - 1];
        اگر(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ثابت صحیح fd = mkstemp((حرف*)fname.c_str());
    اگر (fd == -1) واپس String();

    close(fd);
    remove(fname.c_str());
# endif

    اگر (suffix)
    {
        اگر (suffix[0] != '.')
            واپس fname + "." + suffix;
        ورنہ
            واپس fname + suffix;
    }
    واپس fname;
#ورنہ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_غیر_مستعمل(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#اگر_ختم // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ساکن ErrorCallback customErrorCallback = 0;
ساکن خالی* custom_Error_Callback_ڈیٹا = 0;
ساکن منطقی breakOnError = جھوٹ;

منطقی سیٹ_Break_On_Error(منطقی value)
{
    منطقی پچھلا_Val = breakOnError;
    breakOnError = value;
    واپس پچھلا_Val;
}

صحیح cv_snprintf(حرف* buf, صحیح len, ثابت حرف* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    صحیح res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    واپس res;
}

صحیح cv_vsnprintf(حرف* buf, صحیح len, ثابت حرف* fmt, va_list دلائل)
{
#اگر defined _MSC_VER
    اگر (len <= 0) واپس len == 0 ? 1024 : -1;
    صحیح res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, دلائل);
    // ensure null terminating on VS
    اگر (res >= 0 && res < len)
    {
        buf[res] = 0;
        واپس res;
    }
    ورنہ
    {
        buf[len - 1] = 0; // truncate happened
        واپس res >= len ? res : (len * 2);
    }
#ورنہ
    واپس vsnprintf(buf, len, fmt, دلائل);
#اگر_ختم
}

ساکن خالی dumpException(ثابت Exception& exc)
{
    ثابت حرف* errorStr = exc.رمز_پیغام();
    حرف buf[1 << 12];

    cv_snprintf(buf, حجم(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_نسخہ,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.سطر);
#اگر_تعریف_شدہ __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ورنہ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#اگر_ختم
}

#اگر_تعریف_شدہ CV_ERROR_سیٹ_TERMINATE_HANDLER
ساکن منطقی cv_terminate_handler_installed = جھوٹ;
ساکن std::terminate_handler cv_old_terminate_handler;
ساکن cv::Exception cv_terminate_handler_exception;
ساکن منطقی param_تیاری_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", سچ);
ساکن خالی cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    اگر (جھوٹ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#اگر_ختم

#اگر_تعریف_شدہ __GNUC__
# اگر defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#اگر_ختم

خالی error( ثابت Exception& exc )
{
#اگر_تعریف_شدہ CV_ERROR_سیٹ_TERMINATE_HANDLER
    {
        cv::Auto_تالا lock(getInitializationMutex());
        اگر (!cv_terminate_handler_installed)
        {
            اگر (param_تیاری_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = سچ;
        }
        cv_terminate_handler_exception = exc;
    }
#اگر_ختم

    اگر (customErrorCallback != 0)
        customErrorCallback(exc.رمز, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.سطر, custom_Error_Callback_ڈیٹا);
    ورنہ اگر (param_dumpErrors)
    {
        dumpException(exc);
    }

    اگر(breakOnError)
    {
        std::terminate();
    }

    پھینکو exc;
#اگر_تعریف_شدہ __GNUC__
# اگر !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#اگر_ختم
}

خالی error(Error::رمز_3 رمز_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحیح سطر_3)
{
    error(cv::Exception(رمز_4, _err, _func, _file, سطر_3));
#اگر_تعریف_شدہ __GNUC__
# اگر !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#اگر_ختم
}

#اگر_تعریف_شدہ __GNUC__
# اگر defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#اگر_ختم


ErrorCallback
redirectError( ErrorCallback errCallback, خالی* userdata, خالی** پچھلا_Userdata)
{
    اگر( پچھلا_Userdata )
        *پچھلا_Userdata = custom_Error_Callback_ڈیٹا;

    ErrorCallback پچھلا_Callback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_ڈیٹا = userdata;

    واپس پچھلا_Callback;
}

خالی terminate(Error::رمز_3 رمز_4, ثابت String& _err, ثابت حرف* _func, ثابت حرف* _file, صحیح سطر_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(رمز_4, _err, _func, _file, سطر_3));
    std::terminate();
}

}

نامخلا cv {
منطقی __termination = جھوٹ;


//////////////////////////////// thread-local storage ////////////////////////////////

نامخلا details {

#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT

#اگر_تعریف_شدہ _WIN32
#اگر_تعریف_شدہ _MSC_VER
#ہدایت warning(disable:4505) // unreferenced local function has been removed
#اگر_ختم
#اگر_غیر_تعریف_شدہ TLS_برآمد_کا_INDEXES
#تعریف_کرو TLS_برآمد_کا_INDEXES ((DWORD)0xFFFFFFFF)
#اگر_ختم
#اگر_ختم

// TLS platform abstraction layer
جماعت TlsAbstraction
{
عوامی:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    خالی* get_ڈیٹا() ثابت;
    خالی سیٹ_ڈیٹا(خالی *p_ڈیٹا);

    خالی releaseSystemResources();

نجی:

#اگر_تعریف_شدہ _WIN32
#اگر_غیر_تعریف_شدہ WINRT
    DWORD tls_کلید;
    منطقی disposed;
#اگر_ختم
#ورنہ // _WIN32
    pthread_key_t  tls_کلید;
    std::atomic<منطقی> disposed;
#اگر_ختم
};

جماعت TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
عوامی:
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
ساکن TlsAbstraction* getTlsAbstraction()
{
    ساکن TlsAbstraction *g_tls = نیا TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ساکن TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    واپس g_tls;
}


#اگر_تعریف_شدہ _WIN32
#اگر_تعریف_شدہ WINRT
ساکن __declspec( thread ) خالی* tls_ڈیٹا = تہی; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
خالی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
}
خالی* TlsAbstraction::get_ڈیٹا() ثابت
{
    واپس tls_ڈیٹا;
}
خالی TlsAbstraction::سیٹ_ڈیٹا(خالی *p_ڈیٹا)
{
    tls_ڈیٹا = p_ڈیٹا;
}
#ورنہ //WINRT
#اگر_تعریف_شدہ CV_USE_FLS
ساکن خالی NTAPI opencv_fls_destructor(خالی* p_ڈیٹا);
#اگر_ختم // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(جھوٹ)
{
#اگر_غیر_تعریف_شدہ CV_USE_FLS
    tls_کلید = TlsAlloc();
#ورنہ // CV_USE_FLS
    tls_کلید = FlsAlloc(opencv_fls_destructor);
#اگر_ختم // CV_USE_FLS
    CV_Assert(tls_کلید != TLS_برآمد_کا_INDEXES);
}
خالی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
    disposed = سچ;
#اگر_غیر_تعریف_شدہ CV_USE_FLS
    TlsFree(tls_کلید);
#ورنہ // CV_USE_FLS
    FlsFree(tls_کلید);
#اگر_ختم // CV_USE_FLS
    tls_کلید = TLS_برآمد_کا_INDEXES;
}
خالی* TlsAbstraction::get_ڈیٹا() ثابت
{
    اگر (disposed)
        واپس تہی;
#اگر_غیر_تعریف_شدہ CV_USE_FLS
    واپس Tls_Get_قدر(tls_کلید);
#ورنہ // CV_USE_FLS
    واپس Fls_Get_قدر(tls_کلید);
#اگر_ختم // CV_USE_FLS
}
خالی TlsAbstraction::سیٹ_ڈیٹا(خالی *p_ڈیٹا)
{
    اگر (disposed)
        واپس;  // no-op
#اگر_غیر_تعریف_شدہ CV_USE_FLS
    CV_Assert(Tls_سیٹ_قدر(tls_کلید, p_ڈیٹا) == TRUE);
#ورنہ // CV_USE_FLS
    CV_Assert(Fls_سیٹ_قدر(tls_کلید, p_ڈیٹا) == TRUE);
#اگر_ختم // CV_USE_FLS
}
#اگر_ختم // WINRT
#ورنہ // _WIN32
ساکن خالی opencv_tls_destructor(خالی* p_ڈیٹا);
TlsAbstraction::TlsAbstraction()
    : disposed(جھوٹ)
{
    CV_Assert(pthread_key_create(&tls_کلید, opencv_tls_destructor) == 0);
}
خالی TlsAbstraction::releaseSystemResources()
{
    cv::__termination = سچ;  // DllMain is missing in static builds
    disposed = سچ;
    اگر (pthread_key_delete(tls_کلید) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
خالی* TlsAbstraction::get_ڈیٹا() ثابت
{
    اگر (disposed)
        واپس تہی;
    واپس pthread_getspecific(tls_کلید);
}
خالی TlsAbstraction::سیٹ_ڈیٹا(خالی *p_ڈیٹا)
{
    اگر (disposed)
        واپس;  // no-op
    CV_Assert(pthread_setspecific(tls_کلید, p_ڈیٹا) == 0);
}
#اگر_ختم

// Per-thread data structure
ساخت Thread_ڈیٹا
{
    Thread_ڈیٹا()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<خالی*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ساکن منطقی g_isTlsStorageInitialized = جھوٹ;

// Main TLS storage class
جماعت TlsStorage
{
عوامی:
    TlsStorage() :
        tls_Slots_سائز(0)
    {
        (خالی)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        دھاگے.reserve(32);
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

    خالی releaseThread(خالی* tls_قدر = تہی)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        اگر (تہی == tls)
            واپس;  // TLS singleton is not available (terminated)
        Thread_ڈیٹا *pTD = tls_قدر == تہی ? (Thread_ڈیٹا*)tls->get_ڈیٹا() : (Thread_ڈیٹا*)tls_قدر;
        اگر (pTD == تہی)
            واپس;  // no OpenCV TLS data for this thread
        Auto_تالا guard(mtxGlobalAccess);
        برائے (size_t i = 0; i < دھاگے.size(); i++)
        {
            اگر (pTD == دھاگے[i])
            {
                دھاگے[i] = تہی;
                اگر (tls_قدر == تہی)
                    tls->سیٹ_ڈیٹا(0);
                std::vector<خالی*>& thread_slots = pTD->slots;
                برائے (size_t خانہ_Idx = 0; خانہ_Idx < thread_slots.size(); خانہ_Idx++)
                {
                    خالی* p_ڈیٹا = thread_slots[خانہ_Idx];
                    thread_slots[خانہ_Idx] = تہی;
                    اگر (!p_ڈیٹا)
                        جاری;
                    TLSDataContainer* container = tlsSlots[خانہ_Idx].container;
                    اگر (container)
                        container->delete_ڈیٹا_Instance(p_ڈیٹا);
                    ورنہ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (صحیح)خانہ_Idx);
                        fflush(stderr);
                    }
                }
                حذف pTD;
                واپس;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (خالی*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_خانہ(TLSDataContainer* container)
    {
        Auto_تالا guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائز == tlsSlots.size());

        // Find unused slots
        برائے(size_t خانہ = 0; خانہ < tls_Slots_سائز; خانہ++)
        {
            اگر (tlsSlots[خانہ].container == تہی)
            {
                tlsSlots[خانہ].container = container;
                واپس خانہ;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_خانہ_Info(container)); tls_Slots_سائز++;
        واپس tls_Slots_سائز - 1;
    }

    // Release TLS storage index and pass associated data to caller
    خالی release_خانہ(size_t خانہ_Idx, std::vector<خالی*> &ڈیٹا_Vec, منطقی keep_خانہ = جھوٹ)
    {
        Auto_تالا guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائز == tlsSlots.size());
        CV_Assert(tls_Slots_سائز > خانہ_Idx);

        برائے(size_t i = 0; i < دھاگے.size(); i++)
        {
            اگر(دھاگے[i])
            {
                std::vector<خالی*>& thread_slots = دھاگے[i]->slots;
                اگر (thread_slots.size() > خانہ_Idx && thread_slots[خانہ_Idx])
                {
                    ڈیٹا_Vec.push_back(thread_slots[خانہ_Idx]);
                    thread_slots[خانہ_Idx] = تہی;
                }
            }
        }

        اگر (!keep_خانہ)
        {
            tlsSlots[خانہ_Idx].container = تہی;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    خالی* get_ڈیٹا(size_t خانہ_Idx) ثابت
    {
#اگر_غیر_تعریف_شدہ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_سائز > خانہ_Idx);
#اگر_ختم

        TlsAbstraction* tls = getTlsAbstraction();
        اگر (تہی == tls)
            واپس تہی;  // TLS singleton is not available (terminated)

        Thread_ڈیٹا* thread_ڈیٹا = (Thread_ڈیٹا*)tls->get_ڈیٹا();
        اگر(thread_ڈیٹا && thread_ڈیٹا->slots.size() > خانہ_Idx)
            واپس thread_ڈیٹا->slots[خانہ_Idx];

        واپس تہی;
    }

    // Gather data from threads by TLS storage index
    خالی gather(size_t خانہ_Idx, std::vector<خالی*> &ڈیٹا_Vec)
    {
        Auto_تالا guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_سائز == tlsSlots.size());
        CV_Assert(tls_Slots_سائز > خانہ_Idx);

        برائے(size_t i = 0; i < دھاگے.size(); i++)
        {
            اگر(دھاگے[i])
            {
                std::vector<خالی*>& thread_slots = دھاگے[i]->slots;
                اگر (thread_slots.size() > خانہ_Idx && thread_slots[خانہ_Idx])
                    ڈیٹا_Vec.push_back(thread_slots[خانہ_Idx]);
            }
        }
    }

    // Set data to storage index
    خالی سیٹ_ڈیٹا(size_t خانہ_Idx, خالی* p_ڈیٹا)
    {
#اگر_غیر_تعریف_شدہ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_سائز > خانہ_Idx);
#اگر_ختم

        TlsAbstraction* tls = getTlsAbstraction();
        اگر (تہی == tls)
            واپس;  // TLS singleton is not available (terminated)

        Thread_ڈیٹا* thread_ڈیٹا = (Thread_ڈیٹا*)tls->get_ڈیٹا();
        اگر(!thread_ڈیٹا)
        {
            thread_ڈیٹا = نیا Thread_ڈیٹا;
            tls->سیٹ_ڈیٹا((خالی*)thread_ڈیٹا);
            {
                Auto_تالا guard(mtxGlobalAccess);

                منطقی ملا = جھوٹ;
                // Find unused slots
                برائے(size_t خانہ = 0; خانہ < دھاگے.size(); خانہ++)
                {
                    اگر (دھاگے[خانہ] == تہی)
                    {
                        thread_ڈیٹا->idx = (صحیح)خانہ;
                        دھاگے[خانہ] = thread_ڈیٹا;
                        ملا = سچ;
                        توڑو;
                    }
                }

                اگر (!ملا)
                {
                    // Create new slot
                    thread_ڈیٹا->idx = دھاگے.size();
                    دھاگے.push_back(thread_ڈیٹا);
                }
            }
        }

        اگر(خانہ_Idx >= thread_ڈیٹا->slots.size())
        {
            Auto_تالا guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_ڈیٹا->slots.resize(خانہ_Idx + 1, تہی);
        }
        thread_ڈیٹا->slots[خانہ_Idx] = p_ڈیٹا;
    }

نجی:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_سائز;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ساخت Tls_خانہ_Info
    {
        Tls_خانہ_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ساخت Tls_خانہ_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_ڈیٹا*> دھاگے; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ساکن TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, نیا TlsStorage())
}

#اگر_غیر_تعریف_شدہ _WIN32  // pthread key destructor
ساکن خالی opencv_tls_destructor(خالی* p_ڈیٹا)
{
    اگر (!g_isTlsStorageInitialized)
        واپس;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ڈیٹا);
}
#ورنہ // _WIN32
#اگر_تعریف_شدہ CV_USE_FLS
ساکن خالی WINAPI opencv_fls_destructor(خالی* p_ڈیٹا)
{
    // Empiric detection of ExitProcess call
    DWORD رمز = STILL_فعال/*259*/;
    BOOL res = Get_Exit_رمز_Process(GetCurrentProcess(), &رمز);
    اگر (res && رمز != STILL_فعال)
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

    اگر (!g_isTlsStorageInitialized)
        واپس;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ڈیٹا);
}
#اگر_ختم // CV_USE_FLS
#اگر_ختم // _WIN32

ساکن TlsStorage* ثابت g_force_initialization_کا_Tls_Storage
#اگر defined __GNUC__
    __attribute__((غیر_مستعمل))
#اگر_ختم
    = &getTlsStorage();


#ورنہ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
جماعت TlsStorage
{
عوامی:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        برائے (size_t خانہ_Idx = 0; خانہ_Idx < slots.size(); خانہ_Idx++)
        {
            خانہ_Info& s = slots[خانہ_Idx];
            TLSDataContainer* container = s.container;
            اگر (container && s.ڈیٹا)
            {
                container->delete_ڈیٹا_Instance(s.ڈیٹا);  // Can't use from SlotInfo destructor
                s.ڈیٹا = خالی_اشاریہ;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_خانہ(TLSDataContainer* container)
    {
        size_t slots_سائز = slots.size();
        برائے (size_t خانہ = 0; خانہ < slots_سائز; خانہ++)
        {
            خانہ_Info& s = slots[خانہ];
            اگر (s.container == تہی)
            {
                CV_Assert(!s.ڈیٹا);
                s.container = container;
                واپس خانہ;
            }
        }

        // create new slot
        slots.push_back(خانہ_Info(container));
        واپس slots_سائز;
    }

    // Release TLS storage index and pass associated data to caller
    خالی release_خانہ(size_t خانہ_Idx, std::vector<خالی*> &ڈیٹا_Vec, منطقی keep_خانہ = جھوٹ)
    {
        CV_Assert(خانہ_Idx < slots.size());
        خانہ_Info& s = slots[خانہ_Idx];
        خالی* ڈیٹا = s.ڈیٹا;
        اگر (ڈیٹا)
        {
            ڈیٹا_Vec.push_back(ڈیٹا);
            s.ڈیٹا = خالی_اشاریہ;
        }
        اگر (!keep_خانہ)
        {
            s.container = تہی;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    خالی* get_ڈیٹا(size_t خانہ_Idx) ثابت
    {
        CV_Assert(خانہ_Idx < slots.size());
        ثابت خانہ_Info& s = slots[خانہ_Idx];
        واپس s.ڈیٹا;
    }

    // Gather data from threads by TLS storage index
    خالی gather(size_t خانہ_Idx, std::vector<خالی*> &ڈیٹا_Vec)
    {
        CV_Assert(خانہ_Idx < slots.size());
        خانہ_Info& s = slots[خانہ_Idx];
        خالی* ڈیٹا = s.ڈیٹا;
        اگر (ڈیٹا)
            ڈیٹا_Vec.push_back(ڈیٹا);
        واپس;
    }

    // Set data to storage index
    خالی سیٹ_ڈیٹا(size_t خانہ_Idx, خالی* p_ڈیٹا)
    {
        CV_Assert(خانہ_Idx < slots.size());
        خانہ_Info& s = slots[خانہ_Idx];
        s.ڈیٹا = p_ڈیٹا;
    }

نجی:
    ساخت خانہ_Info
    {
        خانہ_Info(TLSDataContainer* _container) : container(_container), ڈیٹا(خالی_اشاریہ) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        خالی* ڈیٹا;
    };
    std::vector<ساخت خانہ_Info> slots;
};

ساکن TlsStorage& getTlsStorage()
{
    ساکن TlsStorage g_storage;  // no threading
    واپس g_storage;
}

#اگر_ختم  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
استعمال نامخلا details;

خالی releaseTlsStorageThread()
{
#اگر_غیر_تعریف_شدہ OPENCV_DISABLE_THREAD_SUPPORT
    اگر (!g_isTlsStorageInitialized)
        واپس;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#اگر_ختم
}

TLSDataContainer::TLSDataContainer()
{
    کلید_2 = (صحیح)getTlsStorage().reserve_خانہ(یہ); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(کلید_2 == -1); // Key must be released in child object
}

خالی TLSDataContainer::gather_ڈیٹا(std::vector<خالی*> &ڈیٹا) ثابت
{
    getTlsStorage().gather(کلید_2, ڈیٹا);
}

خالی TLSDataContainer::detach_ڈیٹا(std::vector<خالی*> &ڈیٹا)
{
    getTlsStorage().release_خانہ(کلید_2, ڈیٹا, سچ);
}

خالی TLSDataContainer::release()
{
    اگر (کلید_2 == -1)
        واپس;  // already released
    std::vector<خالی*> ڈیٹا; ڈیٹا.reserve(32);
    getTlsStorage().release_خانہ(کلید_2, ڈیٹا, جھوٹ); // Release key and get stored data for proper destruction
    کلید_2 = -1;
    برائے(size_t i = 0; i < ڈیٹا.size(); i++)  // Delete all associated data
        delete_ڈیٹا_Instance(ڈیٹا[i]);
}

خالی TLSDataContainer::cleanup()
{
    std::vector<خالی*> ڈیٹا; ڈیٹا.reserve(32);
    getTlsStorage().release_خانہ(کلید_2, ڈیٹا, سچ); // Extract stored data with removal from TLS tables
    برائے(size_t i = 0; i < ڈیٹا.size(); i++)  // Delete all associated data
        delete_ڈیٹا_Instance(ڈیٹا[i]);
}

خالی* TLSDataContainer::get_ڈیٹا() ثابت
{
    CV_Assert(کلید_2 != -1 && "Can't fetch data from terminated TLS container.");
    خالی* p_ڈیٹا = getTlsStorage().get_ڈیٹا(کلید_2); // Check if data was already allocated
    اگر(!p_ڈیٹا)
    {
        // Create new data instance and save it to TLS storage
        p_ڈیٹا = create_ڈیٹا_Instance();
        کوشش
        {
            getTlsStorage().سیٹ_ڈیٹا(کلید_2, p_ڈیٹا);
        }
        پکڑو (...)
        {
            delete_ڈیٹا_Instance(p_ڈیٹا);
            پھینکو;
        }
    }
    واپس p_ڈیٹا;
}

ساکن TLSData<CoreTLSData>& get_Core_Tls_ڈیٹا_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, نیا TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_ڈیٹا()
{
    واپس get_Core_Tls_ڈیٹا_TLS().getRef();
}

#اگر defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#اگر_تعریف_شدہ WINRT
    #ہدایت warning(disable:4447) // Disable warning 'main' signature found without threading model
#اگر_ختم

خارجی "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

خارجی "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    اگر (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        اگر (lpReserved != تہی) // called after ExitProcess() call
        {
            cv::__termination = سچ;
        }
        ورنہ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    واپس TRUE;
}
#اگر_ختم


نامخلا {

#اگر_تعریف_شدہ OPENCV_WITH_ITT
منطقی overrideThreadName()
{
    ساکن منطقی param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", جھوٹ);
    واپس param;
}
#اگر_ختم

ساکن صحیح g_thread_عدد = 0;
جماعت ThreadID {
عوامی:
    ثابت صحیح id;
    ThreadID() :
        id(CV_XADD(&g_thread_عدد, 1))
    {
#اگر_تعریف_شدہ OPENCV_WITH_ITT
        اگر (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#اگر_ختم
    }
};

ساکن TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, نیا TLSData<ThreadID>());
}

} // namespace
صحیح utils::getThreadID() { واپس getThreadIDTLS().get()->id; }


جماعت ParseError
{
    std::string bad_قدر;
عوامی:
    ParseError(ثابت std::string &bad_قدر_2) :bad_قدر(bad_قدر_2) {}
    std::string مقصد_String(ثابت std::string &param) ثابت
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_قدر;
        واپس out.str();
    }
};

سانچہ <قسم_نام T>
T parseOption(ثابت std::string &);

سانچہ<>
اندرونی منطقی parseOption(ثابت std::string & value)
{
    اگر (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        واپس سچ;
    }
    اگر (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        واپس جھوٹ;
    }
    پھینکو ParseError(value);
}

سانچہ<>
اندرونی size_t parseOption(ثابت std::string &value)
{
    size_t pos = 0;
    برائے (; pos < value.size(); pos++)
    {
        اگر (!ہندسہ_ہے(value[pos]))
            توڑو;
    }
    cv::String قدر_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(قدر_Str);
    اگر (suffixStr.length() == 0)
        واپس v;
    ورنہ اگر (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        واپس v * 1024 * 1024;
    ورنہ اگر (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        واپس v * 1024;
    پھینکو ParseError(value);
}

سانچہ<>
اندرونی cv::String parseOption(ثابت std::string &value)
{
    واپس value;
}

سانچہ<>
اندرونی utils::Paths parseOption(ثابت std::string &value)
{
    utils::Paths نتیجہ;
#اگر_تعریف_شدہ _WIN32
    ثابت حرف sep = ';';
#ورنہ
    ثابت حرف sep = ':';
#اگر_ختم
    size_t آغاز_pos = 0;
    جب (آغاز_pos != std::string::npos)
    {
        ثابت size_t pos = value.find(sep, آغاز_pos);
        ثابت std::string one_piece(value, آغاز_pos, pos == std::string::npos ? pos : pos - آغاز_pos);
        اگر (!one_piece.empty())
            نتیجہ.push_back(one_piece);
        آغاز_pos = pos == std::string::npos ? pos : pos + 1;
    }
    واپس نتیجہ;
}

ساکن اندرونی ثابت حرف * envRead(ثابت حرف * name)
{
#اگر_تعریف_شدہ NO_GETENV
    CV_غیر_مستعمل(name);
    واپس تہی;
#ورنہ
    واپس getenv(name);
#اگر_ختم
}

سانچہ<قسم_نام T>
اندرونی T اقرأ(ثابت std::string & k, ثابت T & default_قدر_2)
{
    کوشش
    {
        ثابت حرف * res = envRead(k.c_str());
        اگر (res)
            واپس parseOption<T>(std::string(res));
    }
    پکڑو (ثابت ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.مقصد_String(k));
    }
    واپس default_قدر_2;
}

منطقی utils::getConfigurationParameterBool(ثابت حرف* name, منطقی default_قدر_2)
{
    واپس اقرأ<منطقی>(name, default_قدر_2);
}

size_t utils::get_Configuration_Parameter_سائز_T(ثابت حرف* name, size_t default_قدر_2)
{
    واپس اقرأ<size_t>(name, default_قدر_2);
}

std::string utils::getConfigurationParameterString(ثابت حرف* name, ثابت std::string & default_قدر_2)
{
    واپس اقرأ<cv::String>(name, default_قدر_2);
}

utils::Paths utils::getConfigurationParameterPaths(ثابت حرف* name, ثابت utils::Paths &default_قدر_2)
{
    واپس اقرأ<utils::Paths>(name, default_قدر_2);
}


#اگر_تعریف_شدہ CV_COLLECT_IMPL_ڈیٹا
ImplCollector& get_Impl_ڈیٹا()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, نیا ImplCollector())
}

خالی سیٹ_Impl(صحیح flags)
{
    cv::Auto_تالا lock(get_Impl_ڈیٹا().mutex);

    get_Impl_ڈیٹا().implFlags = flags;
    get_Impl_ڈیٹا().impl_رمز.clear();
    get_Impl_ڈیٹا().implFun.clear();
}

خالی جوڑو_Impl(صحیح flag, ثابت حرف* func)
{
    cv::Auto_تالا lock(get_Impl_ڈیٹا().mutex);

    get_Impl_ڈیٹا().implFlags |= flag;
    اگر(func) // use lazy collection if name was not specified
    {
        size_t اشاریہ = get_Impl_ڈیٹا().impl_رمز.size();
        اگر(!اشاریہ || (get_Impl_ڈیٹا().impl_رمز[اشاریہ-1] != flag || get_Impl_ڈیٹا().implFun[اشاریہ-1].compare(func))) // avoid duplicates
        {
            get_Impl_ڈیٹا().impl_رمز.push_back(flag);
            get_Impl_ڈیٹا().implFun.push_back(func);
        }
    }
}

صحیح getImpl(std::vector<صحیح> &impl, std::vector<String> &funName)
{
    cv::Auto_تالا lock(get_Impl_ڈیٹا().mutex);

    impl    = get_Impl_ڈیٹا().impl_رمز;
    funName = get_Impl_ڈیٹا().implFun;
    واپس get_Impl_ڈیٹا().implFlags; // return actual flags for lazy collection
}

منطقی useCollection()
{
    واپس get_Impl_ڈیٹا().useCollection;
}

خالی سیٹ_Use_Collection(منطقی flag)
{
    cv::Auto_تالا lock(get_Impl_ڈیٹا().mutex);

    get_Impl_ڈیٹا().useCollection = flag;
}
#اگر_ختم

نامخلا instr
{
منطقی useInstrumentation()
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    واپس getInstrumentStruct().useInstr;
#ورنہ
    واپس جھوٹ;
#اگر_ختم
}

خالی سیٹ_Use_Instrumentation(منطقی flag)
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ورنہ
    CV_غیر_مستعمل(flag);
#اگر_ختم
}

Instr_گرہ* getTrace()
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    واپس &getInstrumentStruct().جڑ_گرہ_2;
#ورنہ
    واپس تہی;
#اگر_ختم
}

خالی resetTrace()
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    getInstrumentStruct().جڑ_گرہ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_گرہ = &getInstrumentStruct().جڑ_گرہ_2;
#اگر_ختم
}

خالی سیٹ_Flags(FLAGS منوال_Flags)
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = منوال_Flags;
#ورنہ
    CV_غیر_مستعمل(منوال_Flags);
#اگر_ختم
}
FLAGS getFlags()
{
#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
    واپس (FLAGS)getInstrumentStruct().flags;
#ورنہ
    واپس (FLAGS)0;
#اگر_ختم
}

گرہ_ڈیٹا::گرہ_ڈیٹا(ثابت حرف* funName, ثابت حرف* fileName, صحیح سطر_عدد, خالی* ret_پتہ, منطقی alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_سطر_عدد       = سطر_عدد;
    m_ret_پتہ    = ret_پتہ;
    m_alwaysExpand  = alwaysExpand;

    m_دھاگے    = 1;
    m_شمار    = 0;
    m_ticks_کل = 0;

    m_funError  = جھوٹ;
}
گرہ_ڈیٹا::گرہ_ڈیٹا(گرہ_ڈیٹا &ref)
{
    *یہ = ref;
}
گرہ_ڈیٹا& گرہ_ڈیٹا::عامل=(ثابت گرہ_ڈیٹا &دائیں)
{
    یہ->m_funName      = دائیں.m_funName;
    یہ->m_instrType    = دائیں.m_instrType;
    یہ->m_implType     = دائیں.m_implType;
    یہ->m_fileName     = دائیں.m_fileName;
    یہ->m_سطر_عدد      = دائیں.m_سطر_عدد;
    یہ->m_ret_پتہ   = دائیں.m_ret_پتہ;
    یہ->m_alwaysExpand = دائیں.m_alwaysExpand;

    یہ->m_دھاگے     = دائیں.m_دھاگے;
    یہ->m_شمار     = دائیں.m_شمار;
    یہ->m_ticks_کل  = دائیں.m_ticks_کل;

    یہ->m_funError    = دائیں.m_funError;

    واپس *یہ;
}
گرہ_ڈیٹا::~گرہ_ڈیٹا()
{
}
منطقی عامل==(ثابت گرہ_ڈیٹا& بائیں, ثابت گرہ_ڈیٹا& دائیں)
{
    اگر(بائیں.m_سطر_عدد == دائیں.m_سطر_عدد && بائیں.m_funName == دائیں.m_funName && بائیں.m_fileName == دائیں.m_fileName)
    {
        اگر(بائیں.m_ret_پتہ == دائیں.m_ret_پتہ || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_نام || بائیں.m_alwaysExpand))
            واپس سچ;
    }
    واپس جھوٹ;
}

#اگر_تعریف_شدہ ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ساکن InstrStruct instr;
    واپس instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    واپس *getInstrumentStruct().tlsStruct.get();
}

Instr_گرہ* get_Current_گرہ()
{
    واپس getInstrumentTLSStruct().p_Current_گرہ;
}

IntrumentationRegion::IntrumentationRegion(ثابت حرف* funName, ثابت حرف* fileName, صحیح سطر_عدد, خالی *ret_پتہ, منطقی alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = جھوٹ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    اگر(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        اگر(!pTLS->p_Current_گرہ)
        {
            m_disabled = سچ;
            واپس;
        }

        صحیح گہرائی = pTLS->p_Current_گرہ->get_گہرائی();
        اگر(pStruct->max_گہرائی && pStruct->max_گہرائی <= گہرائی)
        {
            m_disabled = سچ;
            واپس;
        }

        گرہ_ڈیٹا payload(funName, fileName, سطر_عدد, ret_پتہ, alwaysExpand, instrType, implType);
        Node<گرہ_ڈیٹا>* p_بچہ = تہی;

        اگر(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_تالا guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_بچہ = pTLS->p_Current_گرہ->ڈھونڈو_بچہ(payload);
            اگر(!p_بچہ)
            {
                p_بچہ = نیا Node<گرہ_ڈیٹا>(payload);
                pTLS->p_Current_گرہ->جوڑو_بچہ(p_بچہ);
            }
        }
        ورنہ
        {
            p_بچہ = pTLS->p_Current_گرہ->ڈھونڈو_بچہ(payload);
            اگر(!p_بچہ)
            {
                m_disabled = سچ;
                واپس;
            }
        }
        pTLS->p_Current_گرہ = p_بچہ;

        m_regionTicks = get_Tick_گنتی();
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

            اگر (pTLS->p_Current_گرہ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_گرہ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_گرہ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::اختتام_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_گنتی() - m_regionTicks);
            {
                cv::Auto_تالا guard(pStruct->mutex_گنتی); // Concurrent ticks accumulation
                pTLS->p_Current_گرہ->m_payload.m_شمار++;
                pTLS->p_Current_گرہ->m_payload.m_ticks_کل += ticks;
                pTLS->p_Current_گرہ->m_payload.m_tls.get()->m_ticks_کل += ticks;
            }

            pTLS->p_Current_گرہ = pTLS->p_Current_گرہ->m_p_والد;
        }
    }
}
#اگر_ختم
}

نامخلا ipp
{

#اگر_تعریف_شدہ HAVE_IPP
ساخت IPPInitSingleton
{
عوامی:
    IPPInitSingleton()
    {
        useIPP         = سچ;

#اگر defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ورنہ
        useIPP_NE      = جھوٹ;
#اگر_ختم
        ipp_حالت      = 0;
        funcname       = تہی;
        filename       = تہی;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_چوٹی_Features = 0;
        pIppLibInfo    = تہی;

        ipp_حالت = ippGetCpuFeatures(&cpuFeatures, تہی);
        اگر(ipp_حالت < 0)
        {
            CV_LOG_ERROR(تہی, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = جھوٹ;
            واپس;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        اگر(!env.empty())
        {
#اگر IPP_نسخہ_X100 >= 201900
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#وگرنہ_اگر IPP_نسخہ_X100 >= 201703
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#وگرنہ_اگر IPP_نسخہ_X100 >= 201700
            ثابت Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ورنہ
            ثابت Ipp64u minorFeatures = 0;
#اگر_ختم

            env = مقصد_زیریں_Case(env);
            اگر(env.substr(0, 2) == "ne")
            {
                useIPP_NE = سچ;
                env = env.substr(3, env.size());
            }

            اگر(env == "disabled")
            {
                CV_LOG_WARNING(تہی, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = جھوٹ;
            }
            ورنہ اگر(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ورنہ اگر(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#اگر IPP_نسخہ_X100 >= 201700
#اگر defined (_M_AMD64) || defined (__x86_64__)
            ورنہ اگر(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#اگر_ختم
#اگر_ختم
            ورنہ
                CV_LOG_ERROR(تہی, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        اگر(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        اگر(!(
#اگر IPP_نسخہ_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#اگر_ختم
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = جھوٹ;
            واپس;
        }

        اگر(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ورنہ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#اگر IPP_نسخہ_X100 >= 201700
        اگر(ippFeatures&ippCPUID_AVX512F)
        {
            اگر((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_چوٹی_Features = ippCPUID_AVX512_SKX;
            ورنہ اگر((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_چوٹی_Features = ippCPUID_AVX512_KNL;
            ورنہ
                ipp_چوٹی_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ورنہ
#اگر_ختم
        اگر(ippFeatures&ippCPUID_AVX2)
            ipp_چوٹی_Features = ippCPUID_AVX2;
        ورنہ اگر(ippFeatures&ippCPUID_SSE42)
            ipp_چوٹی_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_نسخہ();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        اگر (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_چوٹی_Features = ippCPUID_SSE42;
        }
    }

عوامی:
    منطقی        useIPP;
    منطقی        useIPP_NE;

    صحیح         ipp_حالت;  // 0 - all is ok, -1 - IPP functions failed
    ثابت حرف *funcname;
    ثابت حرف *filename;
    صحیح         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_چوٹی_Features;
    ثابت Ipp_Library_نسخہ *pIppLibInfo;
};

ساکن IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, نیا IPPInitSingleton())
}
#اگر_ختم

غیرعلامتی طویل طویل getIppFeatures()
{
#اگر_تعریف_شدہ HAVE_IPP
    واپس getIPPSingleton().ippFeatures;
#ورنہ
    واپس 0;
#اگر_ختم
}

#اگر_تعریف_شدہ HAVE_IPP
غیرعلامتی طویل طویل get_Ipp_چوٹی_Features()
{
    واپس getIPPSingleton().ipp_چوٹی_Features;
}
#اگر_ختم

خالی سیٹ_Ipp_حالت(صحیح حالت_2, ثابت حرف * ثابت _funcname, ثابت حرف * ثابت _filename, صحیح سطر_3)
{
#اگر_تعریف_شدہ HAVE_IPP
    getIPPSingleton().ipp_حالت = حالت_2;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = سطر_3;
#ورنہ
    CV_غیر_مستعمل(حالت_2); CV_غیر_مستعمل(_funcname); CV_غیر_مستعمل(_filename); CV_غیر_مستعمل(سطر_3);
#اگر_ختم
}

صحیح get_Ipp_حالت()
{
#اگر_تعریف_شدہ HAVE_IPP
    واپس getIPPSingleton().ipp_حالت;
#ورنہ
    واپس 0;
#اگر_ختم
}

String getIppErrorLocation()
{
#اگر_تعریف_شدہ HAVE_IPP
    واپس format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ورنہ
    واپس String();
#اگر_ختم
}

String get_Ipp_نسخہ()
{
#اگر_تعریف_شدہ HAVE_IPP
    ثابت Ipp_Library_نسخہ *pInfo = getIPPSingleton().pIppLibInfo;
    اگر(pInfo)
        واپس format("%s %s %s", pInfo->Name, pInfo->نسخہ_4, pInfo->BuildDate);
    ورنہ
        واپس String("error");
#ورنہ
    واپس String("disabled");
#اگر_ختم
}

منطقی useIPP()
{
#اگر_تعریف_شدہ HAVE_IPP
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    اگر (ڈیٹا.useIPP < 0)
    {
        ڈیٹا.useIPP = getIPPSingleton().useIPP;
    }
    واپس (ڈیٹا.useIPP > 0);
#ورنہ
    واپس جھوٹ;
#اگر_ختم
}

خالی سیٹ_Use_IPP(منطقی flag)
{
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
#اگر_تعریف_شدہ HAVE_IPP
    ڈیٹا.useIPP = (getIPPSingleton().useIPP)?flag:جھوٹ;
#ورنہ
    CV_غیر_مستعمل(flag);
    ڈیٹا.useIPP = جھوٹ;
#اگر_ختم
}

منطقی useIPP_NotExact()
{
#اگر_تعریف_شدہ HAVE_IPP
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
    اگر (ڈیٹا.useIPP_NE < 0)
    {
        ڈیٹا.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    واپس (ڈیٹا.useIPP_NE > 0);
#ورنہ
    واپس جھوٹ;
#اگر_ختم
}

خالی سیٹ_Use_IPP_Not_Exact(منطقی flag)
{
    CoreTLSData& ڈیٹا = get_Core_Tls_ڈیٹا();
#اگر_تعریف_شدہ HAVE_IPP
    ڈیٹا.useIPP_NE = flag;
#ورنہ
    CV_غیر_مستعمل(flag);
    ڈیٹا.useIPP_NE = جھوٹ;
#اگر_ختم
}

} // namespace ipp


نامخلا details {

#اگر OPENCV_IMPL_FP_HINTS_X86
#اگر_غیر_تعریف_شدہ _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#تعریف_کرو _MM_DENORMALS_ZERO_ON 0x0040
#اگر_ختم
#اگر_غیر_تعریف_شدہ MM_DENORMALS_ZERO_نقاب  // requires pmmintrin.h (SSE3)
#تعریف_کرو MM_DENORMALS_ZERO_نقاب 0x0040
#اگر_ختم
#اگر_ختم

خالی سیٹ_FPDenormals_Ignore_Hint(منطقی ignore, CV_برآمد FPDenormals_منوال_State& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    غیرعلامتی نقاب = MM_FLUSH_ZERO_نقاب;
    غیرعلامتی value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    اگر (featuresEnabled.have[CPU_SSE3])
    {
        نقاب |= MM_DENORMALS_ZERO_نقاب;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ثابت غیرعلامتی old_flags = _mm_getcsr();
    ثابت غیرعلامتی old_قدر = old_flags & نقاب;
    غیرعلامتی flags = (old_flags & ~نقاب) | value;
    CV_LOG_DEBUG(تہی, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)نقاب;
    state.reserved[1] = (uint32_t)old_قدر;
    _mm_setcsr(flags);
#ورنہ
    CV_غیر_مستعمل(ignore); CV_غیر_مستعمل(state);
#اگر_ختم
}

صحیح saveFPDenormalsState(CV_برآمد FPDenormals_منوال_State& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    غیرعلامتی نقاب = MM_FLUSH_ZERO_نقاب;
    اگر (featuresEnabled.have[CPU_SSE3])
    {
        نقاب |= MM_DENORMALS_ZERO_نقاب;
    }
    ثابت غیرعلامتی old_flags = _mm_getcsr();
    ثابت غیرعلامتی old_قدر = old_flags & نقاب;
    // save state
    state.reserved[0] = (uint32_t)نقاب;
    state.reserved[1] = (uint32_t)old_قدر;
    واپس 2;
#ورنہ
    CV_غیر_مستعمل(state);
    واپس 0;
#اگر_ختم
}

منطقی restoreFPDenormalsState(ثابت FPDenormals_منوال_State& state)
{
#اگر OPENCV_IMPL_FP_HINTS_X86
    ثابت غیرعلامتی نقاب = (غیرعلامتی)state.reserved[0];
    CV_DbgAssert(نقاب != 0); // invalid state (ensure that state is properly saved earlier)
    ثابت غیرعلامتی value = (غیرعلامتی)state.reserved[1];
    CV_DbgCheck((صحیح)value, value == (value & نقاب), "invalid SSE FP state");
    ثابت غیرعلامتی old_flags = _mm_getcsr();
    غیرعلامتی flags = (old_flags & ~نقاب) | value;
    CV_LOG_DEBUG(تہی, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    واپس سچ;
#ورنہ
    CV_غیر_مستعمل(state);
    واپس جھوٹ;
#اگر_ختم
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#اگر_تعریف_شدہ OPENCV_ALGO_HINT_DEFAULT
    واپس OPENCV_ALGO_HINT_DEFAULT;
#ورنہ
    واپس ALGO_HINT_ACCURATE;
#اگر_ختم
};

} // namespace cv

/* End of file. */
