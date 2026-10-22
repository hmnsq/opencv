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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <atomic>
#ਅਨ੍ਤਰ੍ਭਾਵ <exception>
#ਅਨ੍ਤਰ੍ਭਾਵ <iostream>
#ਅਨ੍ਤਰ੍ਭਾਵ <ostream>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __QNX__
    #ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <sys/neutrino.h>
    #ਅਨ੍ਤਰ੍ਭਾਵ <sys/syspage.h>
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __aarch64__
    #ਅਨ੍ਤਰ੍ਭਾਵ <aarch64/syspage.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/trace.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/tls.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/instrumentation.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/filesystem.private.hpp>

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/fp_control_utils.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/fp_control.private.hpp>

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਖਾਲੀ _initSystem()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ਸਥਿਰ std::ios_base::Init s_iostream_initializer;
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ Mutex* __initialization_mutex = ਨਿਰਾ;
Mutex& getInitializationMutex()
{
    ਜੇ (__initialization_mutex == ਨਿਰਾ)
    {
        (ਖਾਲੀ)_initSystem();
        __initialization_mutex = ਨਵਾਂ Mutex();
    }
    ਵਾਪਸ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ਸਥਿਰ ਤਾਰਕਿਕ param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ਜੇ defined(_DEBUG) || defined(__ANDROID__)
    ਸੱਚ
#ਨਹੀਂਤਾਂ
    ਝੂਠ
#ਜੇ_ਅੰਤ
);

ਖਾਲੀ* alloc_Singleton_ਬਫਰ(size_t size) { ਵਾਪਸ fastMalloc(size); }
ਖਾਲੀ* alloc_Singleton_New_ਬਫਰ(size_t size) { ਵਾਪਸ ਸਮ੍ਰਿਤੀ_ਦਿਓ(size); }


} // namespace cv

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER  // build config option
# ਜੇ defined(_WIN32)
#   define CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER 1
# endif
#ਜੇ_ਅੰਤ
#ਜੇ defined(CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER) && !CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER
# undef CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
# ਜੇ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER
#ਅਨ੍ਤਰ੍ਭਾਵ <cstdlib>        // std::abort
#ਜੇ_ਅੰਤ

#ਜੇ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ਜੇ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
ਵਰਤੋਂ Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ਅਚਲ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ਨਹੀਂਤਾਂ
#  include <elf.h>
#ਜੇ_ਅੰਤ
#ਜੇ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#ਜੇ_ਅੰਤ


#ਜੇ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#ਜੇ_ਅੰਤ

#ਜੇ defined __loongarch64
#ਅਨ੍ਤਰ੍ਭਾਵ "sys/auxv.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LA_HWCAP_LSX   (1<<4)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LA_HWCAP_LASX  (1<<5)
#ਜੇ_ਅੰਤ

#ਜੇ defined _WIN32 || defined WINCE
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <windows.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <combaseapi.h>
#ਜੇ (_WIN32_WINNT >= 0x0602)
  #ਅਨ੍ਤਰ੍ਭਾਵ <synchapi.h>
#ਜੇ_ਅੰਤ
#ਜੇ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ਅਨ੍ਤਰ੍ਭਾਵ <fibersapi.h>
  #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_USE_FLS
#ਜੇ_ਅੰਤ
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ small
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ min
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ max
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ ਨਿਰਪੇਖ
#ਅਨ੍ਤਰ੍ਭਾਵ <tchar.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
#ਅਨ੍ਤਰ੍ਭਾਵ <wrl/client.h>
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ __cplusplus_winrt
#ਅਨ੍ਤਰ੍ਭਾਵ <windows.storage.h>
#ਹਦਾਇਤ comment(lib, "runtimeobject.lib")
#ਜੇ_ਅੰਤ // WINRT

std::wstring Get_ਆਰਜ਼ੀ_ਰਸਤਾ_Win_RT()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __cplusplus_winrt
    ਵਾਪਸ std::wstring(Windows::Storage::Application_ਅੰਕੜੇ::Current->TemporaryFolder->ਰਸਤਾ_2->ਅੰਕੜੇ_4());
#ਨਹੀਂਤਾਂ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ਅੰਕੜੇ_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ਅੰਕੜੇ> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_ਵਸਤੂ> storageitemRef;
    HSTRING str;
    HSTRING_ਸਿਰਲੇਖ hstr_ਸਿਰ;
    std::wstring wstr;
    ਜੇ (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_ਅੰਕੜੇ,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_ਅੰਕੜੇ), &hstr_ਸਿਰ, &str)))
        ਵਾਪਸ wstr;
    ਜੇ (FAILED(RoGetActivationFactory(str, IID_PPV_ਦਲੀਲਾਂ(appdataFactory.Release_And_Get_ਪਤਾ_ਦਾ()))))
        ਵਾਪਸ wstr;
    ਜੇ (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_ਪਤਾ_ਦਾ())))
        ਵਾਪਸ wstr;
    ਜੇ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_ਪਤਾ_ਦਾ())))
        ਵਾਪਸ wstr;
    ਜੇ (FAILED(storagefolderRef.As(&storageitemRef)))
        ਵਾਪਸ wstr;
    str = ਨਿਰਾ;
    ਜੇ (FAILED(storageitemRef->get_ਰਸਤਾ(&str)))
        ਵਾਪਸ wstr;
    wstr = Windows_Get_String_Raw_ਬਫਰ(str, ਨਿਰਾ);
    WindowsDeleteString(str);
    ਵਾਪਸ wstr;
#ਜੇ_ਅੰਤ
}

std::wstring Get_ਆਰਜ਼ੀ_File_Name_Win_RT(std::wstring ਅਗੇਤਰ)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* ਨਕਾਬ = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], ਆਕਾਰ(guidStr)/ਆਕਾਰ(wchar_t), ਨਕਾਬ,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ਵਾਪਸ ਅਗੇਤਰ.append(std::wstring(guidStr));
}

#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT
#ਅਨ੍ਤਰ੍ਭਾਵ <pthread.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

#ਜੇ defined __MACH__ && defined __APPLE__
#ਅਨ੍ਤਰ੍ਭਾਵ <mach/mach.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <mach/mach_time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysctl.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _OPENMP
#ਅਨ੍ਤਰ੍ਭਾਵ "omp.h"
#ਜੇ_ਅੰਤ

#ਜੇ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ defined __ANDROID__
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysconf.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
# include <android/log.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_CPUID_X86
  #ਜੇ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ਜੇ _MSC_VER >= 1400  // MSVS 2005
      #ਅਨ੍ਤਰ੍ਭਾਵ <intrin.h>  // __cpuidex()
      #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_CPUID_X86 __cpuidex
    #ਨਹੀਂਤਾਂ
      #ਗਲਤੀ "Required MSVS 2005+"
    #ਜੇ_ਅੰਤ
  #ਜਾਂਜੇ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ਸਥਿਰ ਖਾਲੀ cv_cpuid(ਪੂਰਨ* cpuid_ਅੰਕੜੇ, ਪੂਰਨ reg_eax, ਪੂਰਨ reg_ecx)
    {
        ਪੂਰਨ __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ਜੇ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ਜਾਂਜੇ defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ਨਹੀਂਤਾਂ
#ਗਲਤੀ "Configuration error"
#ਜੇ_ਅੰਤ
        cpuid_ਅੰਕੜੇ[0] = __eax; cpuid_ਅੰਕੜੇ[1] = __ebx; cpuid_ਅੰਕੜੇ[2] = __ecx; cpuid_ਅੰਕੜੇ[3] = __edx;
    }
    #ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_CPUID_X86 cv_cpuid
  #ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <chrono>

ਨਾਮਾਕਾਸ਼ cv
{

Exception::Exception() { ਕੋਡ = Error::StsOk; ਸਤਰ = 0; }

Exception::Exception(Error::ਕੋਡ_3 ਕੋਡ_4, ਅਚਲ String& _err, ਅਚਲ String& _func, ਅਚਲ String& _file, ਪੂਰਨ ਸਤਰ_3)
: ਕੋਡ(ਕੋਡ_4), err(_err), func(_func), file(_file), ਸਤਰ(ਸਤਰ_3)
{
    format_ਸੁਨੇਹਾ();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ਅਚਲ ਅੱਖਰ* Exception::what() ਅਚਲ CV_NOEXCEPT { ਵਾਪਸ ਸੁਨੇਹਾ.c_str(); }

ਖਾਲੀ Exception::format_ਸੁਨੇਹਾ()
{
    size_t pos = err.find('\n');
    ਤਾਰਕਿਕ multiline = pos != cv::String::npos;
    ਜੇ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ਜਦੋਂ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::ਸਤਰ_ਅੰਤ;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ਜੇ (err[err.size() - 1] != '\n')
            ss << std::ਸਤਰ_ਅੰਤ;
        err = ss.str();
    }
    ਜੇ (func.size() > 0)
    {
        ਜੇ (multiline)
            ਸੁਨੇਹਾ = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_ਸੰਸਕਰਣ, file.c_str(), ਸਤਰ, ਕੋਡ, ਕੋਡ_ਸੁਨੇਹਾ(), func.c_str(), err.c_str());
        ਨਹੀਂਤਾਂ
            ਸੁਨੇਹਾ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_ਸੰਸਕਰਣ, file.c_str(), ਸਤਰ, ਕੋਡ, ਕੋਡ_ਸੁਨੇਹਾ(), err.c_str(), func.c_str());
    }
    ਨਹੀਂਤਾਂ
    {
        ਸੁਨੇਹਾ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_ਸੰਸਕਰਣ, file.c_str(), ਸਤਰ, ਕੋਡ, ਕੋਡ_ਸੁਨੇਹਾ(), err.c_str(), multiline ? "" : "\n");
    }
}

ਅਚਲ ਅੱਖਰ * Exception::ਕੋਡ_ਸੁਨੇਹਾ() ਅਚਲ
{
    ਚੋਣ (ਕੋਡ)
    {
    ਹਾਲਤ cv::Error::StsOk :                  ਵਾਪਸ "No Error";
    ਹਾਲਤ cv::Error::Sts_ਪਿੱਠ_Trace :           ਵਾਪਸ "Backtrace";
    ਹਾਲਤ cv::Error::StsError :               ਵਾਪਸ "Unspecified error";
    ਹਾਲਤ cv::Error::Sts_ਅੰਦਰੂਨੀ :            ਵਾਪਸ "Internal error";
    ਹਾਲਤ cv::Error::StsNoMem :               ਵਾਪਸ "Insufficient memory";
    ਹਾਲਤ cv::Error::StsBadArg :              ਵਾਪਸ "Bad argument";
    ਹਾਲਤ cv::Error::StsNoConv :              ਵਾਪਸ "Iterations do not converge";
    ਹਾਲਤ cv::Error::StsAutoTrace :           ਵਾਪਸ "Autotrace call";
    ਹਾਲਤ cv::Error::Sts_Bad_ਮਾਪ :             ਵਾਪਸ "Incorrect size of input array";
    ਹਾਲਤ cv::Error::StsNullPtr :             ਵਾਪਸ "Null pointer";
    ਹਾਲਤ cv::Error::StsDivByZero :           ਵਾਪਸ "Division by zero occurred";
    ਹਾਲਤ cv::Error::Bad_ਕਦਮ :                ਵਾਪਸ "Image step is wrong";
    ਹਾਲਤ cv::Error::StsInplaceNotSupported : ਵਾਪਸ "Inplace operation is not supported";
    ਹਾਲਤ cv::Error::Sts_Object_Not_ਮਿਲਿਆ :      ਵਾਪਸ "Requested object was not found";
    ਹਾਲਤ cv::Error::Bad_ਡੂੰਘਾਈ :               ਵਾਪਸ "Input image depth is not supported by function";
    ਹਾਲਤ cv::Error::StsUnmatchedFormats :    ਵਾਪਸ "Formats of input arguments do not match";
    ਹਾਲਤ cv::Error::StsUnmatchedSizes :      ਵਾਪਸ "Sizes of input arguments do not match";
    ਹਾਲਤ cv::Error::Sts_ਬਾਹਰਲਾ_ਦਾ_Range :          ਵਾਪਸ "One of the arguments\' values is out of range";
    ਹਾਲਤ cv::Error::StsUnsupportedFormat :   ਵਾਪਸ "Unsupported format or combination of formats";
    ਹਾਲਤ cv::Error::BadCOI :                 ਵਾਪਸ "Input COI is not supported";
    ਹਾਲਤ cv::Error::Bad_ਅੰਕ_Channels :         ਵਾਪਸ "Bad number of channels";
    ਹਾਲਤ cv::Error::StsBadFlag :             ਵਾਪਸ "Bad flag (parameter or structure field)";
    ਹਾਲਤ cv::Error::Sts_Bad_ਬਿੰਦੂ :            ਵਾਪਸ "Bad parameter of type CvPoint";
    ਹਾਲਤ cv::Error::Sts_Bad_ਨਕਾਬ :             ਵਾਪਸ "Bad type of mask argument";
    ਹਾਲਤ cv::Error::StsParseError :          ਵਾਪਸ "Parsing error";
    ਹਾਲਤ cv::Error::StsNotImplemented :      ਵਾਪਸ "The function/feature is not implemented";
    ਹਾਲਤ cv::Error::Sts_Bad_Mem_ਖੰਡ :         ਵਾਪਸ "Memory block has been corrupted";
    ਹਾਲਤ cv::Error::StsAssert :              ਵਾਪਸ "Assertion failed";
    ਹਾਲਤ cv::Error::GpuNotSupported :        ਵਾਪਸ "No CUDA support";
    ਹਾਲਤ cv::Error::GpuApiCallError :        ਵਾਪਸ "Gpu API call";
    ਹਾਲਤ cv::Error::OpenGlNotSupported :     ਵਾਪਸ "No OpenGL support";
    ਹਾਲਤ cv::Error::OpenGlApiCallError :     ਵਾਪਸ "OpenGL API call";
    ਹਾਲਤ cv::Error::OpenCLApiCallError :     ਵਾਪਸ "OpenCL API call error";
    ਹਾਲਤ cv::Error::OpenCLDoubleNotSupported:ਵਾਪਸ "OpenCL device does not support double";
    ਹਾਲਤ cv::Error::OpenCLInitError :        ਵਾਪਸ "OpenCL initialization error";
    ਹਾਲਤ cv::Error::OpenCLNoAMDBlasFft :     ਵਾਪਸ "Missing OpenCL AMD BLAS FFT";
    ਮੂਲ:
    {
        ਸਥਿਰ ਅੱਖਰ buf[256] {0};
        snprintf(buf, ਆਕਾਰ(buf), "Unknown %s code %d", ਕੋਡ >= 0 ? "status":"error", ਕੋਡ);
        ਵਾਪਸ buf;
    }
    };

}

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* g_hw_Feature_ਨਾਂ[CV_HARDWARE_MAX_FEATURE] = { ਨਿਰਾ };

ਸਥਿਰ ਅਚਲ ਅੱਖਰ* getHWFeatureName(ਪੂਰਨ id)
{
    ਵਾਪਸ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_ਨਾਂ[id] : ਨਿਰਾ;
}
ਸਥਿਰ ਅਚਲ ਅੱਖਰ* get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(ਪੂਰਨ id)
{
    ਅਚਲ ਅੱਖਰ* name = getHWFeatureName(id);
    ਵਾਪਸ name ? name : "Unknown feature";
}

ਬਣਤਰ HWFeatures
{
    ਗਿਣਤੀ { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(ਤਾਰਕਿਕ ਲੜੀਵਾਰ_initialize = ਝੂਠ)
    {
        ਜੇ (ਲੜੀਵਾਰ_initialize)
            initialize();
    }

    ਸਥਿਰ ਖਾਲੀ initialize_ਨਾਂ()
    {
        ਲਈ (ਪੂਰਨ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_ਨਾਂ[i] = 0;
        }
        g_hw_Feature_ਨਾਂ[CPU_MMX] = "MMX";
        g_hw_Feature_ਨਾਂ[CPU_SSE] = "SSE";
        g_hw_Feature_ਨਾਂ[CPU_SSE2] = "SSE2";
        g_hw_Feature_ਨਾਂ[CPU_SSE3] = "SSE3";
        g_hw_Feature_ਨਾਂ[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_ਨਾਂ[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_ਨਾਂ[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_ਨਾਂ[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_ਨਾਂ[CPU_FP16] = "FP16";
        g_hw_Feature_ਨਾਂ[CPU_AVX] = "AVX";
        g_hw_Feature_ਨਾਂ[CPU_AVX2] = "AVX2";
        g_hw_Feature_ਨਾਂ[CPU_FMA3] = "FMA3";

        g_hw_Feature_ਨਾਂ[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_ਨਾਂ[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_ਨਾਂ[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_ਨਾਂ[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_ਨਾਂ[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_ਨਾਂ[CPU_NEON] = "NEON";
        g_hw_Feature_ਨਾਂ[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_ਨਾਂ[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_ਨਾਂ[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_ਨਾਂ[CPU_SVE] = "SVE";

        g_hw_Feature_ਨਾਂ[CPU_VSX] = "VSX";
        g_hw_Feature_ਨਾਂ[CPU_VSX3] = "VSX3";

        g_hw_Feature_ਨਾਂ[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_ਨਾਂ[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_ਨਾਂ[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_ਨਾਂ[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_ਨਾਂ[CPU_RVV] = "RVV";
        g_hw_Feature_ਨਾਂ[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_ਨਾਂ[CPU_LSX]  = "LSX";
        g_hw_Feature_ਨਾਂ[CPU_LASX] = "LASX";
    }

    ਖਾਲੀ initialize(ਖਾਲੀ)
    {
        ਜੇ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_ਨਾਂ();

    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_CPUID_X86
        ਪੂਰਨ cpuid_ਅੰਕੜੇ[4] = { 0, 0, 0, 0 };
        ਪੂਰਨ cpuid_ਅੰਕੜੇ_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_ਅੰਕੜੇ, 1, 0/*ਅਣਵਰਤਿਆ*/);

        ਪੂਰਨ x86_family = (cpuid_ਅੰਕੜੇ[0] >> 8) & 15;
        ਜੇ( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_ਅੰਕੜੇ[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_ਅੰਕੜੇ[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_ਅੰਕੜੇ[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_ਅੰਕੜੇ[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_ਅੰਕੜੇ[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_ਅੰਕੜੇ[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_ਅੰਕੜੇ[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_ਅੰਕੜੇ[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_ਅੰਕੜੇ[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_ਅੰਕੜੇ[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_ਅੰਕੜੇ[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_ਅੰਕੜੇ_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_ਅੰਕੜੇ_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_ਅੰਕੜੇ_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_ਅੰਕੜੇ_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_ਅੰਕੜੇ_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_ਅੰਕੜੇ_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_ਅੰਕੜੇ_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_ਅੰਕੜੇ_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_ਅੰਕੜੇ_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            ਪੂਰਨ cpuid_ਅੰਕੜੇ_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_ਅੰਕੜੇ_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_ਅੰਕੜੇ_ex1[0] & (1<<4))  != 0;

            ਤਾਰਕਿਕ have_AVX_OS_support = ਸੱਚ;
            ਤਾਰਕਿਕ have_AVX512_OS_support = ਸੱਚ;
            ਜੇ (!(cpuid_ਅੰਕੜੇ[2] & (1<<27)))
                have_AVX_OS_support = ਝੂਠ; // OS uses XSAVE_XRSTORE and CPU support AVX
            ਨਹੀਂਤਾਂ
            {
                ਪੂਰਨ xcr0 = 0;
            #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ XCR_XFEATURE_ENABLED_ਨਕਾਬ // requires immintrin.h
                xcr0 = (ਪੂਰਨ)_xgetbv(XCR_XFEATURE_ENABLED_ਨਕਾਬ);
            #ਜਾਂਜੇ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #ਜੇ_ਅੰਤ
                ਜੇ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = ਝੂਠ; // YMM registers
                ਜੇ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = ਝੂਠ; // ZMM registers
            }

            ਜੇ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = ਝੂਠ;
                have[CV_CPU_FP16] = ਝੂਠ;
                have[CV_CPU_AVX2] = ਝੂਠ;
                have[CV_CPU_FMA3] = ਝੂਠ;
                have[CV_CPU_AVX_VNNI] = ਝੂਠ;
            }
            ਜੇ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = ਝੂਠ;
                have[CV_CPU_AVX_512BW] = ਝੂਠ;
                have[CV_CPU_AVX_512CD] = ਝੂਠ;
                have[CV_CPU_AVX_512DQ] = ਝੂਠ;
                have[CV_CPU_AVX_512ER] = ਝੂਠ;
                have[CV_CPU_AVX_512IFMA] = ਝੂਠ;
                have[CV_CPU_AVX_512PF] = ਝੂਠ;
                have[CV_CPU_AVX_512VBMI] = ਝੂਠ;
                have[CV_CPU_AVX_512VL] = ਝੂਠ;
                have[CV_CPU_AVX_512VBMI2] = ਝੂਠ;
                have[CV_CPU_AVX_512VNNI] = ਝੂਠ;
                have[CV_CPU_AVX_512BITALG] = ਝੂਠ;
                have[CV_CPU_AVX_512VPOPCNTDQ] = ਝੂਠ;
                have[CV_CPU_AVX_5124VNNIW] = ਝੂਠ;
                have[CV_CPU_AVX_5124FMAPS] = ਝੂਠ;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ਜੇ (have[CV_CPU_AVX512_COMMON])
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
            ਨਹੀਂਤਾਂ
            {
                have[CV_CPU_AVX512_KNL] = ਝੂਠ;
                have[CV_CPU_AVX512_KNM] = ਝੂਠ;
                have[CV_CPU_AVX512_SKX] = ਝੂਠ;
                have[CV_CPU_AVX512_CNL] = ਝੂਠ;
                have[CV_CPU_AVX512_CLX] = ਝੂਠ;
                have[CV_CPU_AVX512_ICL] = ਝੂਠ;
            }
        }
    #ਜੇ_ਅੰਤ // CV_CPUID_X86

    #ਜੇ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __aarch64__
        have[CV_CPU_NEON] = ਸੱਚ;
        have[CV_CPU_FP16] = ਸੱਚ;
        ਪੂਰਨ cpufile = open("/proc/self/auxv", O_RDONLY);

        ਜੇ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ਅਚਲ size_t ਮਾਪ_auxv_t = ਆਕਾਰ(auxv);

            ਜਦੋਂ ((size_t)ਪਠ(cpufile, &auxv, ਮਾਪ_auxv_t) == ਮਾਪ_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ਜੇ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ਜੇ defined(AT_HWCAP2)
                ਨਹੀਂਤਾਂ ਜੇ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#ਜੇ_ਅੰਤ
            }

            close(cpufile);
        }
    #ਜਾਂਜੇ defined __arm__ && defined __ANDROID__
      #ਜੇ defined HAVE_CPUFEATURES
        CV_LOG_INFO(ਨਿਰਾ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ਨਿਰਾ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (ਲੰਮਾ ਲੰਮਾ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ਨਹੀਂਤਾਂ
        CV_LOG_INFO(ਨਿਰਾ, "cpufeatures library is not available for CPU detection");
        #ਜੇ CV_NEON
        CV_LOG_INFO(ਨਿਰਾ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = ਸੱਚ;
        #ਨਹੀਂਤਾਂ
        CV_LOG_INFO(ਨਿਰਾ, "- NEON instructions is NOT enabled via build flags");
        #ਜੇ_ਅੰਤ
        #ਜੇ CV_FP16
        CV_LOG_INFO(ਨਿਰਾ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = ਸੱਚ;
        #ਨਹੀਂਤਾਂ
        CV_LOG_INFO(ਨਿਰਾ, "- FP16 instructions is NOT enabled via build flags");
        #ਜੇ_ਅੰਤ
      #ਜੇ_ਅੰਤ
    #ਜਾਂਜੇ defined __arm__
        ਪੂਰਨ cpufile = open("/proc/self/auxv", O_RDONLY);

        ਜੇ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ਅਚਲ size_t ਮਾਪ_auxv_t = ਆਕਾਰ(auxv);

            ਜਦੋਂ ((size_t)ਪਠ(cpufile, &auxv, ਮਾਪ_auxv_t) == ਮਾਪ_auxv_t)
            {
                ਜੇ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ਰੋਕੋ;
                }
            }

            close(cpufile);
        }
    #ਜੇ_ਅੰਤ
    #ਜਾਂਜੇ (defined __APPLE__)
    #ਜੇ defined __ARM_NEON
        have[CV_CPU_NEON] = ਸੱਚ;
    #ਜੇ_ਅੰਤ
    #ਜੇ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = ਸੱਚ;
    #ਜੇ_ਅੰਤ
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    ਪੂਰਨ has_feat_dotprod = 0;
    size_t has_feat_dotprod_ਮਾਪ = ਆਕਾਰ(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_ਮਾਪ, ਨਿਰਾ, 0);
    ਜੇ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = ਸੱਚ;
    }
    ਪੂਰਨ has_feat_bf16 = 0;
    size_t has_feat_bf16_ਮਾਪ = ਆਕਾਰ(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_ਮਾਪ, ਨਿਰਾ, 0);
    ਜੇ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = ਸੱਚ;
    }
    #ਜਾਂਜੇ (defined __clang__)
    #ਜੇ defined __ARM_NEON
        have[CV_CPU_NEON] = ਸੱਚ;
        #ਜੇ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = ਸੱਚ;
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ
    #ਜੇ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = ਸੱਚ;
    #ਜੇ_ਅੰਤ
    #ਜੇ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = ਸੱਚ;
    #ਜੇ_ਅੰਤ
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __riscv_vector
        have[CV_CPU_RISCVV] = ਸੱਚ;
    #ਜੇ_ਅੰਤ
    #ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __mips_msa
        have[CV_CPU_MSA] = ਸੱਚ;
    #ਜੇ_ਅੰਤ

    #ਜੇ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ hwcap = getauxval(AT_HWCAP);
        ਜੇ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ਜੇ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ਸੱਚ;
            } ਨਹੀਂਤਾਂ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ਜਾਂਜੇ (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ hwcap = 0, hwcap2 = 0;
        ਜੇ (elf_aux_info(AT_HWCAP, &hwcap, ਆਕਾਰ(hwcap)) != 0)
            hwcap = 0;
        ਜੇ (hwcap & PPC_FEATURE_HAS_VSX) {
            ਜੇ (elf_aux_info(AT_HWCAP2, &hwcap2, ਆਕਾਰ(hwcap2)) != 0)
                hwcap2 = 0;
            ਜੇ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ਸੱਚ;
            } ਨਹੀਂਤਾਂ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ਨਹੀਂਤਾਂ
        // TODO: AIX
        #ਜੇ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = ਸੱਚ;
        #ਜੇ_ਅੰਤ
        #ਜੇ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = ਸੱਚ;
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ

    #ਜੇ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = ਸੱਚ;
        #ਜੇ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = ਸੱਚ;
        #ਜੇ_ਅੰਤ
        #ਜੇ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = ਸੱਚ;
        #ਜੇ_ਅੰਤ
    #ਜੇ_ਅੰਤ

    #ਜੇ defined __loongarch64 && defined __linux__
        ਪੂਰਨ flag = (ਪੂਰਨ)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #ਜੇ_ਅੰਤ

        ਤਾਰਕਿਕ skip_baseline_check = ਝੂਠ;
        ਜੇ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = ਸੱਚ;
        }
        ਪੂਰਨ baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ਜੇ (!checkFeatures(baseline_features, ਆਕਾਰ(baseline_features) / ਆਕਾਰ(baseline_features[0]))
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
            checkFeatures(baseline_features, ਆਕਾਰ(baseline_features) / ਆਕਾਰ(baseline_features[0]), ਸੱਚ);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, ਆਕਾਰ(baseline_features) / ਆਕਾਰ(baseline_features[0]));
    }

    ਤਾਰਕਿਕ checkFeatures(ਅਚਲ ਪੂਰਨ* features, ਪੂਰਨ ਗਿਣਤੀ_3, ਤਾਰਕਿਕ dump = ਝੂਠ)
    {
        ਤਾਰਕਿਕ ਨਤੀਜਾ = ਸੱਚ;
        ਲਈ (ਪੂਰਨ i = 0; i < ਗਿਣਤੀ_3; i++)
        {
            ਪੂਰਨ feature = features[i];
            ਜੇ (feature)
            {
                ਜੇ (have[feature])
                {
                    ਜੇ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(feature));
                }
                ਨਹੀਂਤਾਂ
                {
                    ਨਤੀਜਾ = ਝੂਠ;
                    ਜੇ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(feature));
                }
            }
        }
        ਵਾਪਸ ਨਤੀਜਾ;
    }

    ਸਥਿਰ ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ is_ਚਿੰਨ੍ਹ_Separator(ਅੱਖਰ c)
    {
        ਵਾਪਸ c == ',' || c == ';';
    }

    ਖਾਲੀ readSettings(ਅਚਲ ਪੂਰਨ* baseline_features, ਪੂਰਨ baseline_ਗਿਣਤੀ)
    {
        ਤਾਰਕਿਕ dump = ਸੱਚ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ਜੇ (!disabled_features.empty())
        {
            ਅਚਲ ਅੱਖਰ* ਸ਼ੁਰੂ = disabled_features.c_str();
            ਲਈ (;;)
            {
                ਜਦੋਂ (ਸ਼ੁਰੂ[0] != 0 && is_ਚਿੰਨ੍ਹ_Separator(ਸ਼ੁਰੂ[0]))
                {
                    ਸ਼ੁਰੂ++;
                }
                ਜੇ (ਸ਼ੁਰੂ[0] == 0)
                    ਰੋਕੋ;
                ਅਚਲ ਅੱਖਰ* end = ਸ਼ੁਰੂ;
                ਜਦੋਂ (end[0] != 0 && !is_ਚਿੰਨ੍ਹ_Separator(end[0]))
                {
                    end++;
                }
                ਜੇ (end == ਸ਼ੁਰੂ)
                    ਜਾਰੀ;
                cv::String feature(ਸ਼ੁਰੂ, end);
                ਸ਼ੁਰੂ = end;

                CV_Assert(feature.size() > 0);

                ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
                ਲਈ (ਪੂਰਨ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ਜੇ (!g_hw_Feature_ਨਾਂ[i]) ਜਾਰੀ;
                    size_t len = ਲੜੀ_ਲੰਬਾਈ(g_hw_Feature_ਨਾਂ[i]);
                    ਜੇ (len != feature.size()) ਜਾਰੀ;
                    ਜੇ (feature.compare(g_hw_Feature_ਨਾਂ[i]) == 0)
                    {
                        ਤਾਰਕਿਕ isBaseline = ਝੂਠ;
                        ਲਈ (ਪੂਰਨ k = 0; k < baseline_ਗਿਣਤੀ; k++)
                        {
                            ਜੇ (baseline_features[k] == i)
                            {
                                isBaseline = ਸੱਚ;
                                ਰੋਕੋ;
                            }
                        }
                        ਜੇ (isBaseline)
                        {
                            ਜੇ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(i));
                        }
                        ਜੇ (!have[i])
                        {
                            ਜੇ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(i));
                        }
                        have[i] = ਝੂਠ;

                        ਮਿਲਿਆ = ਸੱਚ;
                        ਰੋਕੋ;
                    }
                }
                ਜੇ (!ਮਿਲਿਆ)
                {
                    ਜੇ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    ਤਾਰਕਿਕ have[MAX_FEATURE+1]{};
};

ਸਥਿਰ HWFeatures  featuresEnabled(ਸੱਚ), featuresDisabled = HWFeatures(ਝੂਠ);
ਸਥਿਰ HWFeatures* currentFeatures = &featuresEnabled;

ਤਾਰਕਿਕ checkHardwareSupport(ਪੂਰਨ feature)
{
    ਜੇ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ਵਾਪਸ ਝੂਠ;
    ਵਾਪਸ currentFeatures->have[feature];
}

String getHardwareFeatureName(ਪੂਰਨ feature)
{
    ਅਚਲ ਅੱਖਰ* name = getHWFeatureName(feature);
    ਵਾਪਸ name ? String(name) : String();
}

std::string get_CPUFeatures_ਸਤਰ()
{
    ਅਚਲ ਪੂਰਨ features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ਅਚਲ ਪੂਰਨ sz = ਆਕਾਰ(features) / ਆਕਾਰ(features[0]);
    std::string ਨਤੀਜਾ;
    std::string ਅਗੇਤਰ;
    ਲਈ (ਪੂਰਨ i = 1; i < sz; ++i)
    {
        ਜੇ (features[i] == 0)
        {
            ਅਗੇਤਰ = "*";
            ਜਾਰੀ;
        }
        ਜੇ (i != 1) ਨਤੀਜਾ.append(" ");
        ਨਤੀਜਾ.append(ਅਗੇਤਰ);
        ਨਤੀਜਾ.append(get_HWFeature_Name_ਸੁਰੱਖਿਅਤ(features[i]));
        ਜੇ (!checkHardwareSupport(features[i])) ਨਤੀਜਾ.append("?");
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਅਸਥਿਰ ਤਾਰਕਿਕ useOptimizedFlag = ਸੱਚ;

ਖਾਲੀ ਸੈੱਟ_Use_Optimized( ਤਾਰਕਿਕ flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::ਸੈੱਟ_Use_IPP(flag);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
    ocl::ਸੈੱਟ_Use_Open_CL(flag);
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ useOptimized(ਖਾਲੀ)
{
    ਵਾਪਸ useOptimizedFlag;
}

int64 get_Tick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ਵਾਪਸ (int64)now.time_since_epoch().ਗਿਣਤੀ_3();
}

ਦੁੱਗਣਾ getTickFrequency(ਖਾਲੀ)
{
    ਵਰਤੋਂ clock_period_t = std::chrono::steady_clock::duration::period;
    ਦੁੱਗਣਾ clock_freq = clock_period_t::den / clock_period_t::num;
    ਵਾਪਸ clock_freq;
}

#ਜੇ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ਜੇ defined(__i386__)

int64 get_CPUTick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    int64 x;
    __asm__ ਅਸਥਿਰ (".byte 0x0f, 0x31" : "=A" (x));
    ਵਾਪਸ x;
}
#ਜਾਂਜੇ defined(__x86_64__)

int64 get_CPUTick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ਵਾਪਸ (int64)lo | ((int64)hi << 32);
}

#ਜਾਂਜੇ defined(__ppc__)

int64 get_CPUTick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    ਬਿਨਾਨਿਸ਼ਾਨੀ upper, lower, tmp;
    __asm__ ਅਸਥਿਰ(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ਵਾਪਸ lower | ((int64)upper << 32);
}

#ਨਹੀਂਤਾਂ

#ਗਲਤੀ "RDTSC not defined"

#ਜੇ_ਅੰਤ

#ਜਾਂਜੇ defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ਨਹੀਂਤਾਂ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_ਗਿਣਤੀ(ਖਾਲੀ)
{
    ਵਾਪਸ get_Tick_ਗਿਣਤੀ();
}
//#endif

#ਜੇ_ਅੰਤ


ਨਾਮਾਕਾਸ਼ internal {

ਵਰਗ Timestamp
{
ਜਨਤਕ:
    ਅਚਲ int64 zero_Tick_ਗਿਣਤੀ;
    ਅਚਲ ਦੁੱਗਣਾ ns_ਵਿੱਚ_ticks;

    Timestamp()
        : zero_Tick_ਗਿਣਤੀ(get_Tick_ਗਿਣਤੀ())
        , ns_ਵਿੱਚ_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ਗਿਣਤੀ();
        ਵਾਪਸ (int64)((t - zero_Tick_ਗਿਣਤੀ) * ns_ਵਿੱਚ_ticks);
    }

    ਸਥਿਰ Timestamp& getInstance()
    {
        ਸਥਿਰ Timestamp g_timestamp;
        ਵਾਪਸ g_timestamp;
    }
};

ਵਰਗ InitTimestamp {
ਜਨਤਕ:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ਸਥਿਰ InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ਵਾਪਸ internal::Timestamp::getInstance().getTimestamp();
}


ਅਚਲ String& getBuildInformation()
{
    ਸਥਿਰ String build_info =
#ਅਨ੍ਤਰ੍ਭਾਵ "version_string.inc"
    ;
    ਵਾਪਸ build_info;
}

String get_ਸੰਸਕਰਣ_String() { ਵਾਪਸ String(CV_ਸੰਸਕਰਣ); }

ਪੂਰਨ get_ਸੰਸਕਰਣ_Major() { ਵਾਪਸ CV_ਸੰਸਕਰਣ_MAJOR; }

ਪੂਰਨ get_ਸੰਸਕਰਣ_Minor() { ਵਾਪਸ CV_ਸੰਸਕਰਣ_MINOR; }

ਪੂਰਨ get_ਸੰਸਕਰਣ_Revision() { ਵਾਪਸ CV_ਸੰਸਕਰਣ_REVISION; }

String format( ਅਚਲ ਅੱਖਰ* fmt, ... )
{
    Auto_ਬਫਰ<ਅੱਖਰ, 1024> buf;

    ਲਈ ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        ਪੂਰਨ bsize = static_cast<ਪੂਰਨ>(buf.size());
        ਪੂਰਨ len = cv_vsnprintf(buf.ਅੰਕੜੇ(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ਜੇ (len >= bsize)
        {
            buf.resize(len + 1);
            ਜਾਰੀ;
        }
        buf[bsize - 1] = 0;
        ਵਾਪਸ String(buf.ਅੰਕੜੇ(), len);
    }
}

String tempfile( ਅਚਲ ਅੱਖਰ* suffix )
{
#ਜੇ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string ਆਰਜ਼ੀ_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ਜੇ defined _WIN32
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring ਆਰਜ਼ੀ_dir_rt = Get_ਆਰਜ਼ੀ_ਰਸਤਾ_Win_RT();

    std::wstring ਆਰਜ਼ੀ_file = Get_ਆਰਜ਼ੀ_File_Name_Win_RT(L"ocv");
    ਜੇ (ਆਰਜ਼ੀ_file.empty())
        ਵਾਪਸ String();

    ਆਰਜ਼ੀ_file = ਆਰਜ਼ੀ_dir_rt.append(std::wstring(L"\\")).append(ਆਰਜ਼ੀ_file);
    DeleteFileW(ਆਰਜ਼ੀ_file.c_str());

    ਅੱਖਰ aname[MAX_ਰਸਤਾ];
    size_t copied = wcstombs(aname, ਆਰਜ਼ੀ_file.c_str(), MAX_ਰਸਤਾ);
    CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#ਜਾਂਜੇ defined(_WIN32_WCE)
    ਅਚਲ ਸਵੈਚਾਲਿਤ k_Max_ਰਸਤਾ_ਮਾਪ = MAX_ਰਸਤਾ+1;
    wchar_t ਆਰਜ਼ੀ_dir_ce[k_Max_ਰਸਤਾ_ਮਾਪ] = {0};
    wchar_t ਆਰਜ਼ੀ_file[k_Max_ਰਸਤਾ_ਮਾਪ] = {0};

    ::Get_ਆਰਜ਼ੀ_ਰਸਤਾ_W(k_Max_ਰਸਤਾ_ਮਾਪ, ਆਰਜ਼ੀ_dir_ce);

    ਜੇ(0 != ::Get_ਆਰਜ਼ੀ_File_Name_W(ਆਰਜ਼ੀ_dir_ce, L"ocv", 0, ਆਰਜ਼ੀ_file)) {
        DeleteFileW(ਆਰਜ਼ੀ_file);
        ਅੱਖਰ aname[MAX_ਰਸਤਾ];
        size_t copied = wcstombs(aname, ਆਰਜ਼ੀ_file, MAX_ਰਸਤਾ);
        CV_Assert((copied != MAX_ਰਸਤਾ) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ਨਹੀਂਤਾਂ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t ਆਰਜ਼ੀ_dir2_w[MAX_ਰਸਤਾ] = { 0 };

    ਜੇ (ਆਰਜ਼ੀ_dir.empty())
    {
        ::Get_ਆਰਜ਼ੀ_ਰਸਤਾ_W(ਆਕਾਰ(ਆਰਜ਼ੀ_dir2_w)/ਆਕਾਰ(wchar_t), ਆਰਜ਼ੀ_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        ਪੂਰਨ len = Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, ਆਰਜ਼ੀ_dir2_w, -1, ਨਿਰਾ, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (len > 0)
        {
            std::vector<ਅੱਖਰ> utf8_buf(len);
            Wide_Char_ਮੰਜ਼ਿਲ_Multi_Byte(CP_UTF8, 0, ਆਰਜ਼ੀ_dir2_w, -1, utf8_buf.ਅੰਕੜੇ(), len, ਨਿਰਾ, ਨਿਰਾ);
            ਆਰਜ਼ੀ_dir = std::string(utf8_buf.ਅੰਕੜੇ());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ਜੇ (FAILED(hr))
        ਵਾਪਸ String();
    ਅੱਖਰ guidStr[40];
    ਅਚਲ ਅੱਖਰ* ਨਕਾਬ = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, ਆਕਾਰ(guidStr), ਨਕਾਬ,
            g.Data1, g.Data2, g.Data3, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[0], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[1],
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[2], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[3], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[4],
            (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[5], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[6], (ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ)g.Data4[7]);

    fname = ਆਰਜ਼ੀ_dir;
    ਜੇ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#ਜੇ_ਅੰਤ
# ਨਹੀਂਤਾਂ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    ਅੱਖਰ defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ਨਹੀਂਤਾਂ
    ਅੱਖਰ defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ਜੇ (ਆਰਜ਼ੀ_dir.empty())
        fname = defaultTemplate;
    ਨਹੀਂਤਾਂ
    {
        fname = ਆਰਜ਼ੀ_dir;
        ਅੱਖਰ ech = fname[fname.size() - 1];
        ਜੇ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ਅਚਲ ਪੂਰਨ fd = mkstemp((ਅੱਖਰ*)fname.c_str());
    ਜੇ (fd == -1) ਵਾਪਸ String();

    close(fd);
    remove(fname.c_str());
# endif

    ਜੇ (suffix)
    {
        ਜੇ (suffix[0] != '.')
            ਵਾਪਸ fname + "." + suffix;
        ਨਹੀਂਤਾਂ
            ਵਾਪਸ fname + suffix;
    }
    ਵਾਪਸ fname;
#ਨਹੀਂਤਾਂ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ਅਣਵਰਤਿਆ(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ਜੇ_ਅੰਤ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ਸਥਿਰ ErrorCallback customErrorCallback = 0;
ਸਥਿਰ ਖਾਲੀ* custom_Error_Callback_ਅੰਕੜੇ = 0;
ਸਥਿਰ ਤਾਰਕਿਕ breakOnError = ਝੂਠ;

ਤਾਰਕਿਕ ਸੈੱਟ_Break_On_Error(ਤਾਰਕਿਕ value)
{
    ਤਾਰਕਿਕ prevVal = breakOnError;
    breakOnError = value;
    ਵਾਪਸ prevVal;
}

ਪੂਰਨ cv_snprintf(ਅੱਖਰ* buf, ਪੂਰਨ len, ਅਚਲ ਅੱਖਰ* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    ਪੂਰਨ res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ਵਾਪਸ res;
}

ਪੂਰਨ cv_vsnprintf(ਅੱਖਰ* buf, ਪੂਰਨ len, ਅਚਲ ਅੱਖਰ* fmt, va_list ਦਲੀਲਾਂ)
{
#ਜੇ defined _MSC_VER
    ਜੇ (len <= 0) ਵਾਪਸ len == 0 ? 1024 : -1;
    ਪੂਰਨ res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, ਦਲੀਲਾਂ);
    // ensure null terminating on VS
    ਜੇ (res >= 0 && res < len)
    {
        buf[res] = 0;
        ਵਾਪਸ res;
    }
    ਨਹੀਂਤਾਂ
    {
        buf[len - 1] = 0; // truncate happened
        ਵਾਪਸ res >= len ? res : (len * 2);
    }
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ vsnprintf(buf, len, fmt, ਦਲੀਲਾਂ);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ ਖਾਲੀ dumpException(ਅਚਲ Exception& exc)
{
    ਅਚਲ ਅੱਖਰ* errorStr = exc.ਕੋਡ_ਸੁਨੇਹਾ();
    ਅੱਖਰ buf[1 << 12];

    cv_snprintf(buf, ਆਕਾਰ(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_ਸੰਸਕਰਣ,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.ਸਤਰ);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ਨਹੀਂਤਾਂ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER
ਸਥਿਰ ਤਾਰਕਿਕ cv_terminate_handler_installed = ਝੂਠ;
ਸਥਿਰ std::terminate_handler cv_old_terminate_handler;
ਸਥਿਰ cv::Exception cv_terminate_handler_exception;
ਸਥਿਰ ਤਾਰਕਿਕ param_ਤਿਆਰੀ_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", ਸੱਚ);
ਸਥਿਰ ਖਾਲੀ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ਜੇ (ਝੂਠ /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
# ਜੇ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#ਜੇ_ਅੰਤ

ਖਾਲੀ error( ਅਚਲ Exception& exc )
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_ERROR_ਸੈੱਟ_TERMINATE_HANDLER
    {
        cv::Auto_ਤਾਲਾ lock(getInitializationMutex());
        ਜੇ (!cv_terminate_handler_installed)
        {
            ਜੇ (param_ਤਿਆਰੀ_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = ਸੱਚ;
        }
        cv_terminate_handler_exception = exc;
    }
#ਜੇ_ਅੰਤ

    ਜੇ (customErrorCallback != 0)
        customErrorCallback(exc.ਕੋਡ, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.ਸਤਰ, custom_Error_Callback_ਅੰਕੜੇ);
    ਨਹੀਂਤਾਂ ਜੇ (param_dumpErrors)
    {
        dumpException(exc);
    }

    ਜੇ(breakOnError)
    {
        std::terminate();
    }

    ਸੁੱਟੋ exc;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
# ਜੇ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ error(Error::ਕੋਡ_3 ਕੋਡ_4, ਅਚਲ String& _err, ਅਚਲ ਅੱਖਰ* _func, ਅਚਲ ਅੱਖਰ* _file, ਪੂਰਨ ਸਤਰ_3)
{
    error(cv::Exception(ਕੋਡ_4, _err, _func, _file, ਸਤਰ_3));
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
# ਜੇ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
# ਜੇ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#ਜੇ_ਅੰਤ


ErrorCallback
redirectError( ErrorCallback errCallback, ਖਾਲੀ* userdata, ਖਾਲੀ** prevUserdata)
{
    ਜੇ( prevUserdata )
        *prevUserdata = custom_Error_Callback_ਅੰਕੜੇ;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_ਅੰਕੜੇ = userdata;

    ਵਾਪਸ prevCallback;
}

ਖਾਲੀ terminate(Error::ਕੋਡ_3 ਕੋਡ_4, ਅਚਲ String& _err, ਅਚਲ ਅੱਖਰ* _func, ਅਚਲ ਅੱਖਰ* _file, ਪੂਰਨ ਸਤਰ_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(ਕੋਡ_4, _err, _func, _file, ਸਤਰ_3));
    std::terminate();
}

}

ਨਾਮਾਕਾਸ਼ cv {
ਤਾਰਕਿਕ __termination = ਝੂਠ;


//////////////////////////////// thread-local storage ////////////////////////////////

ਨਾਮਾਕਾਸ਼ details {

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _MSC_VER
#ਹਦਾਇਤ warning(disable:4505) // unreferenced local function has been removed
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ TLS_ਬਾਹਰਲਾ_ਦਾ_INDEXES
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TLS_ਬਾਹਰਲਾ_ਦਾ_INDEXES ((DWORD)0xFFFFFFFF)
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

// TLS platform abstraction layer
ਵਰਗ TlsAbstraction
{
ਜਨਤਕ:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    ਖਾਲੀ* get_ਅੰਕੜੇ() ਅਚਲ;
    ਖਾਲੀ ਸੈੱਟ_ਅੰਕੜੇ(ਖਾਲੀ *p_ਅੰਕੜੇ);

    ਖਾਲੀ releaseSystemResources();

ਨਿਜੀ:

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ WINRT
    DWORD tls_ਕੁੰਜੀ;
    ਤਾਰਕਿਕ disposed;
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ // _WIN32
    pthread_key_t  tls_ਕੁੰਜੀ;
    std::atomic<ਤਾਰਕਿਕ> disposed;
#ਜੇ_ਅੰਤ
};

ਵਰਗ TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
ਜਨਤਕ:
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
ਸਥਿਰ TlsAbstraction* getTlsAbstraction()
{
    ਸਥਿਰ TlsAbstraction *g_tls = ਨਵਾਂ TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ਸਥਿਰ TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ਵਾਪਸ g_tls;
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
ਸਥਿਰ __declspec( thread ) ਖਾਲੀ* tls_ਅੰਕੜੇ = ਨਿਰਾ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
ਖਾਲੀ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ਸੱਚ;  // DllMain is missing in static builds
}
ਖਾਲੀ* TlsAbstraction::get_ਅੰਕੜੇ() ਅਚਲ
{
    ਵਾਪਸ tls_ਅੰਕੜੇ;
}
ਖਾਲੀ TlsAbstraction::ਸੈੱਟ_ਅੰਕੜੇ(ਖਾਲੀ *p_ਅੰਕੜੇ)
{
    tls_ਅੰਕੜੇ = p_ਅੰਕੜੇ;
}
#ਨਹੀਂਤਾਂ //WINRT
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
ਸਥਿਰ ਖਾਲੀ NTAPI opencv_fls_destructor(ਖਾਲੀ* p_ਅੰਕੜੇ);
#ਜੇ_ਅੰਤ // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(ਝੂਠ)
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
    tls_ਕੁੰਜੀ = TlsAlloc();
#ਨਹੀਂਤਾਂ // CV_USE_FLS
    tls_ਕੁੰਜੀ = FlsAlloc(opencv_fls_destructor);
#ਜੇ_ਅੰਤ // CV_USE_FLS
    CV_Assert(tls_ਕੁੰਜੀ != TLS_ਬਾਹਰਲਾ_ਦਾ_INDEXES);
}
ਖਾਲੀ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ਸੱਚ;  // DllMain is missing in static builds
    disposed = ਸੱਚ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
    TlsFree(tls_ਕੁੰਜੀ);
#ਨਹੀਂਤਾਂ // CV_USE_FLS
    FlsFree(tls_ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ // CV_USE_FLS
    tls_ਕੁੰਜੀ = TLS_ਬਾਹਰਲਾ_ਦਾ_INDEXES;
}
ਖਾਲੀ* TlsAbstraction::get_ਅੰਕੜੇ() ਅਚਲ
{
    ਜੇ (disposed)
        ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
    ਵਾਪਸ Tls_Get_ਮੁੱਲ(tls_ਕੁੰਜੀ);
#ਨਹੀਂਤਾਂ // CV_USE_FLS
    ਵਾਪਸ Fls_Get_ਮੁੱਲ(tls_ਕੁੰਜੀ);
#ਜੇ_ਅੰਤ // CV_USE_FLS
}
ਖਾਲੀ TlsAbstraction::ਸੈੱਟ_ਅੰਕੜੇ(ਖਾਲੀ *p_ਅੰਕੜੇ)
{
    ਜੇ (disposed)
        ਵਾਪਸ;  // no-op
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
    CV_Assert(Tls_ਸੈੱਟ_ਮੁੱਲ(tls_ਕੁੰਜੀ, p_ਅੰਕੜੇ) == TRUE);
#ਨਹੀਂਤਾਂ // CV_USE_FLS
    CV_Assert(Fls_ਸੈੱਟ_ਮੁੱਲ(tls_ਕੁੰਜੀ, p_ਅੰਕੜੇ) == TRUE);
#ਜੇ_ਅੰਤ // CV_USE_FLS
}
#ਜੇ_ਅੰਤ // WINRT
#ਨਹੀਂਤਾਂ // _WIN32
ਸਥਿਰ ਖਾਲੀ opencv_tls_destructor(ਖਾਲੀ* p_ਅੰਕੜੇ);
TlsAbstraction::TlsAbstraction()
    : disposed(ਝੂਠ)
{
    CV_Assert(pthread_key_create(&tls_ਕੁੰਜੀ, opencv_tls_destructor) == 0);
}
ਖਾਲੀ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ਸੱਚ;  // DllMain is missing in static builds
    disposed = ਸੱਚ;
    ਜੇ (pthread_key_delete(tls_ਕੁੰਜੀ) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
ਖਾਲੀ* TlsAbstraction::get_ਅੰਕੜੇ() ਅਚਲ
{
    ਜੇ (disposed)
        ਵਾਪਸ ਨਿਰਾ;
    ਵਾਪਸ pthread_getspecific(tls_ਕੁੰਜੀ);
}
ਖਾਲੀ TlsAbstraction::ਸੈੱਟ_ਅੰਕੜੇ(ਖਾਲੀ *p_ਅੰਕੜੇ)
{
    ਜੇ (disposed)
        ਵਾਪਸ;  // no-op
    CV_Assert(pthread_setspecific(tls_ਕੁੰਜੀ, p_ਅੰਕੜੇ) == 0);
}
#ਜੇ_ਅੰਤ

// Per-thread data structure
ਬਣਤਰ Thread_ਅੰਕੜੇ
{
    Thread_ਅੰਕੜੇ()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<ਖਾਲੀ*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ਸਥਿਰ ਤਾਰਕਿਕ g_isTlsStorageInitialized = ਝੂਠ;

// Main TLS storage class
ਵਰਗ TlsStorage
{
ਜਨਤਕ:
    TlsStorage() :
        tls_Slots_ਮਾਪ(0)
    {
        (ਖਾਲੀ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        ਧਾਗੇ.reserve(32);
        g_isTlsStorageInitialized = ਸੱਚ;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    ਖਾਲੀ releaseThread(ਖਾਲੀ* tls_ਮੁੱਲ = ਨਿਰਾ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ਜੇ (ਨਿਰਾ == tls)
            ਵਾਪਸ;  // TLS singleton is not available (terminated)
        Thread_ਅੰਕੜੇ *pTD = tls_ਮੁੱਲ == ਨਿਰਾ ? (Thread_ਅੰਕੜੇ*)tls->get_ਅੰਕੜੇ() : (Thread_ਅੰਕੜੇ*)tls_ਮੁੱਲ;
        ਜੇ (pTD == ਨਿਰਾ)
            ਵਾਪਸ;  // no OpenCV TLS data for this thread
        Auto_ਤਾਲਾ guard(mtxGlobalAccess);
        ਲਈ (size_t i = 0; i < ਧਾਗੇ.size(); i++)
        {
            ਜੇ (pTD == ਧਾਗੇ[i])
            {
                ਧਾਗੇ[i] = ਨਿਰਾ;
                ਜੇ (tls_ਮੁੱਲ == ਨਿਰਾ)
                    tls->ਸੈੱਟ_ਅੰਕੜੇ(0);
                std::vector<ਖਾਲੀ*>& thread_slots = pTD->slots;
                ਲਈ (size_t ਥਾਂ_Idx = 0; ਥਾਂ_Idx < thread_slots.size(); ਥਾਂ_Idx++)
                {
                    ਖਾਲੀ* p_ਅੰਕੜੇ = thread_slots[ਥਾਂ_Idx];
                    thread_slots[ਥਾਂ_Idx] = ਨਿਰਾ;
                    ਜੇ (!p_ਅੰਕੜੇ)
                        ਜਾਰੀ;
                    TLSDataContainer* container = tlsSlots[ਥਾਂ_Idx].container;
                    ਜੇ (container)
                        container->delete_ਅੰਕੜੇ_Instance(p_ਅੰਕੜੇ);
                    ਨਹੀਂਤਾਂ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (ਪੂਰਨ)ਥਾਂ_Idx);
                        fflush(stderr);
                    }
                }
                ਮਿਟਾਓ pTD;
                ਵਾਪਸ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (ਖਾਲੀ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_ਥਾਂ(TLSDataContainer* container)
    {
        Auto_ਤਾਲਾ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ਮਾਪ == tlsSlots.size());

        // Find unused slots
        ਲਈ(size_t ਥਾਂ = 0; ਥਾਂ < tls_Slots_ਮਾਪ; ਥਾਂ++)
        {
            ਜੇ (tlsSlots[ਥਾਂ].container == ਨਿਰਾ)
            {
                tlsSlots[ਥਾਂ].container = container;
                ਵਾਪਸ ਥਾਂ;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_ਥਾਂ_Info(container)); tls_Slots_ਮਾਪ++;
        ਵਾਪਸ tls_Slots_ਮਾਪ - 1;
    }

    // Release TLS storage index and pass associated data to caller
    ਖਾਲੀ release_ਥਾਂ(size_t ਥਾਂ_Idx, std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ_Vec, ਤਾਰਕਿਕ keep_ਥਾਂ = ਝੂਠ)
    {
        Auto_ਤਾਲਾ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ਮਾਪ == tlsSlots.size());
        CV_Assert(tls_Slots_ਮਾਪ > ਥਾਂ_Idx);

        ਲਈ(size_t i = 0; i < ਧਾਗੇ.size(); i++)
        {
            ਜੇ(ਧਾਗੇ[i])
            {
                std::vector<ਖਾਲੀ*>& thread_slots = ਧਾਗੇ[i]->slots;
                ਜੇ (thread_slots.size() > ਥਾਂ_Idx && thread_slots[ਥਾਂ_Idx])
                {
                    ਅੰਕੜੇ_Vec.push_back(thread_slots[ਥਾਂ_Idx]);
                    thread_slots[ਥਾਂ_Idx] = ਨਿਰਾ;
                }
            }
        }

        ਜੇ (!keep_ਥਾਂ)
        {
            tlsSlots[ਥਾਂ_Idx].container = ਨਿਰਾ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ਖਾਲੀ* get_ਅੰਕੜੇ(size_t ਥਾਂ_Idx) ਅਚਲ
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_ਮਾਪ > ਥਾਂ_Idx);
#ਜੇ_ਅੰਤ

        TlsAbstraction* tls = getTlsAbstraction();
        ਜੇ (ਨਿਰਾ == tls)
            ਵਾਪਸ ਨਿਰਾ;  // TLS singleton is not available (terminated)

        Thread_ਅੰਕੜੇ* thread_ਅੰਕੜੇ = (Thread_ਅੰਕੜੇ*)tls->get_ਅੰਕੜੇ();
        ਜੇ(thread_ਅੰਕੜੇ && thread_ਅੰਕੜੇ->slots.size() > ਥਾਂ_Idx)
            ਵਾਪਸ thread_ਅੰਕੜੇ->slots[ਥਾਂ_Idx];

        ਵਾਪਸ ਨਿਰਾ;
    }

    // Gather data from threads by TLS storage index
    ਖਾਲੀ gather(size_t ਥਾਂ_Idx, std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ_Vec)
    {
        Auto_ਤਾਲਾ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ਮਾਪ == tlsSlots.size());
        CV_Assert(tls_Slots_ਮਾਪ > ਥਾਂ_Idx);

        ਲਈ(size_t i = 0; i < ਧਾਗੇ.size(); i++)
        {
            ਜੇ(ਧਾਗੇ[i])
            {
                std::vector<ਖਾਲੀ*>& thread_slots = ਧਾਗੇ[i]->slots;
                ਜੇ (thread_slots.size() > ਥਾਂ_Idx && thread_slots[ਥਾਂ_Idx])
                    ਅੰਕੜੇ_Vec.push_back(thread_slots[ਥਾਂ_Idx]);
            }
        }
    }

    // Set data to storage index
    ਖਾਲੀ ਸੈੱਟ_ਅੰਕੜੇ(size_t ਥਾਂ_Idx, ਖਾਲੀ* p_ਅੰਕੜੇ)
    {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_ਮਾਪ > ਥਾਂ_Idx);
#ਜੇ_ਅੰਤ

        TlsAbstraction* tls = getTlsAbstraction();
        ਜੇ (ਨਿਰਾ == tls)
            ਵਾਪਸ;  // TLS singleton is not available (terminated)

        Thread_ਅੰਕੜੇ* thread_ਅੰਕੜੇ = (Thread_ਅੰਕੜੇ*)tls->get_ਅੰਕੜੇ();
        ਜੇ(!thread_ਅੰਕੜੇ)
        {
            thread_ਅੰਕੜੇ = ਨਵਾਂ Thread_ਅੰਕੜੇ;
            tls->ਸੈੱਟ_ਅੰਕੜੇ((ਖਾਲੀ*)thread_ਅੰਕੜੇ);
            {
                Auto_ਤਾਲਾ guard(mtxGlobalAccess);

                ਤਾਰਕਿਕ ਮਿਲਿਆ = ਝੂਠ;
                // Find unused slots
                ਲਈ(size_t ਥਾਂ = 0; ਥਾਂ < ਧਾਗੇ.size(); ਥਾਂ++)
                {
                    ਜੇ (ਧਾਗੇ[ਥਾਂ] == ਨਿਰਾ)
                    {
                        thread_ਅੰਕੜੇ->idx = (ਪੂਰਨ)ਥਾਂ;
                        ਧਾਗੇ[ਥਾਂ] = thread_ਅੰਕੜੇ;
                        ਮਿਲਿਆ = ਸੱਚ;
                        ਰੋਕੋ;
                    }
                }

                ਜੇ (!ਮਿਲਿਆ)
                {
                    // Create new slot
                    thread_ਅੰਕੜੇ->idx = ਧਾਗੇ.size();
                    ਧਾਗੇ.push_back(thread_ਅੰਕੜੇ);
                }
            }
        }

        ਜੇ(ਥਾਂ_Idx >= thread_ਅੰਕੜੇ->slots.size())
        {
            Auto_ਤਾਲਾ guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_ਅੰਕੜੇ->slots.resize(ਥਾਂ_Idx + 1, ਨਿਰਾ);
        }
        thread_ਅੰਕੜੇ->slots[ਥਾਂ_Idx] = p_ਅੰਕੜੇ;
    }

ਨਿਜੀ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_ਮਾਪ;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ਬਣਤਰ Tls_ਥਾਂ_Info
    {
        Tls_ਥਾਂ_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ਬਣਤਰ Tls_ਥਾਂ_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_ਅੰਕੜੇ*> ਧਾਗੇ; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ਸਥਿਰ TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, ਨਵਾਂ TlsStorage())
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _WIN32  // pthread key destructor
ਸਥਿਰ ਖਾਲੀ opencv_tls_destructor(ਖਾਲੀ* p_ਅੰਕੜੇ)
{
    ਜੇ (!g_isTlsStorageInitialized)
        ਵਾਪਸ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ਅੰਕੜੇ);
}
#ਨਹੀਂਤਾਂ // _WIN32
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_USE_FLS
ਸਥਿਰ ਖਾਲੀ WINAPI opencv_fls_destructor(ਖਾਲੀ* p_ਅੰਕੜੇ)
{
    // Empiric detection of ExitProcess call
    DWORD ਕੋਡ = STILL_ਸਰਗਰਮ/*259*/;
    BOOL res = Get_Exit_ਕੋਡ_Process(GetCurrentProcess(), &ਕੋਡ);
    ਜੇ (res && ਕੋਡ != STILL_ਸਰਗਰਮ)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = ਸੱਚ;
    }

    ਜੇ (!g_isTlsStorageInitialized)
        ਵਾਪਸ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ਅੰਕੜੇ);
}
#ਜੇ_ਅੰਤ // CV_USE_FLS
#ਜੇ_ਅੰਤ // _WIN32

ਸਥਿਰ TlsStorage* ਅਚਲ g_force_initialization_ਦਾ_Tls_Storage
#ਜੇ defined __GNUC__
    __attribute__((ਅਣਵਰਤਿਆ))
#ਜੇ_ਅੰਤ
    = &getTlsStorage();


#ਨਹੀਂਤਾਂ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
ਵਰਗ TlsStorage
{
ਜਨਤਕ:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        ਲਈ (size_t ਥਾਂ_Idx = 0; ਥਾਂ_Idx < slots.size(); ਥਾਂ_Idx++)
        {
            ਥਾਂ_Info& s = slots[ਥਾਂ_Idx];
            TLSDataContainer* container = s.container;
            ਜੇ (container && s.ਅੰਕੜੇ)
            {
                container->delete_ਅੰਕੜੇ_Instance(s.ਅੰਕੜੇ);  // Can't use from SlotInfo destructor
                s.ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_ਥਾਂ(TLSDataContainer* container)
    {
        size_t slots_ਮਾਪ = slots.size();
        ਲਈ (size_t ਥਾਂ = 0; ਥਾਂ < slots_ਮਾਪ; ਥਾਂ++)
        {
            ਥਾਂ_Info& s = slots[ਥਾਂ];
            ਜੇ (s.container == ਨਿਰਾ)
            {
                CV_Assert(!s.ਅੰਕੜੇ);
                s.container = container;
                ਵਾਪਸ ਥਾਂ;
            }
        }

        // create new slot
        slots.push_back(ਥਾਂ_Info(container));
        ਵਾਪਸ slots_ਮਾਪ;
    }

    // Release TLS storage index and pass associated data to caller
    ਖਾਲੀ release_ਥਾਂ(size_t ਥਾਂ_Idx, std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ_Vec, ਤਾਰਕਿਕ keep_ਥਾਂ = ਝੂਠ)
    {
        CV_Assert(ਥਾਂ_Idx < slots.size());
        ਥਾਂ_Info& s = slots[ਥਾਂ_Idx];
        ਖਾਲੀ* ਅੰਕੜੇ = s.ਅੰਕੜੇ;
        ਜੇ (ਅੰਕੜੇ)
        {
            ਅੰਕੜੇ_Vec.push_back(ਅੰਕੜੇ);
            s.ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
        }
        ਜੇ (!keep_ਥਾਂ)
        {
            s.container = ਨਿਰਾ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ਖਾਲੀ* get_ਅੰਕੜੇ(size_t ਥਾਂ_Idx) ਅਚਲ
    {
        CV_Assert(ਥਾਂ_Idx < slots.size());
        ਅਚਲ ਥਾਂ_Info& s = slots[ਥਾਂ_Idx];
        ਵਾਪਸ s.ਅੰਕੜੇ;
    }

    // Gather data from threads by TLS storage index
    ਖਾਲੀ gather(size_t ਥਾਂ_Idx, std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ_Vec)
    {
        CV_Assert(ਥਾਂ_Idx < slots.size());
        ਥਾਂ_Info& s = slots[ਥਾਂ_Idx];
        ਖਾਲੀ* ਅੰਕੜੇ = s.ਅੰਕੜੇ;
        ਜੇ (ਅੰਕੜੇ)
            ਅੰਕੜੇ_Vec.push_back(ਅੰਕੜੇ);
        ਵਾਪਸ;
    }

    // Set data to storage index
    ਖਾਲੀ ਸੈੱਟ_ਅੰਕੜੇ(size_t ਥਾਂ_Idx, ਖਾਲੀ* p_ਅੰਕੜੇ)
    {
        CV_Assert(ਥਾਂ_Idx < slots.size());
        ਥਾਂ_Info& s = slots[ਥਾਂ_Idx];
        s.ਅੰਕੜੇ = p_ਅੰਕੜੇ;
    }

ਨਿਜੀ:
    ਬਣਤਰ ਥਾਂ_Info
    {
        ਥਾਂ_Info(TLSDataContainer* _container) : container(_container), ਅੰਕੜੇ(ਖਾਲੀ_ਸੂਚਕ) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        ਖਾਲੀ* ਅੰਕੜੇ;
    };
    std::vector<ਬਣਤਰ ਥਾਂ_Info> slots;
};

ਸਥਿਰ TlsStorage& getTlsStorage()
{
    ਸਥਿਰ TlsStorage g_storage;  // no threading
    ਵਾਪਸ g_storage;
}

#ਜੇ_ਅੰਤ  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ details;

ਖਾਲੀ releaseTlsStorageThread()
{
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_DISABLE_THREAD_SUPPORT
    ਜੇ (!g_isTlsStorageInitialized)
        ਵਾਪਸ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#ਜੇ_ਅੰਤ
}

TLSDataContainer::TLSDataContainer()
{
    ਕੁੰਜੀ_2 = (ਪੂਰਨ)getTlsStorage().reserve_ਥਾਂ(ਇਹ); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ਕੁੰਜੀ_2 == -1); // Key must be released in child object
}

ਖਾਲੀ TLSDataContainer::gather_ਅੰਕੜੇ(std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ) ਅਚਲ
{
    getTlsStorage().gather(ਕੁੰਜੀ_2, ਅੰਕੜੇ);
}

ਖਾਲੀ TLSDataContainer::detach_ਅੰਕੜੇ(std::vector<ਖਾਲੀ*> &ਅੰਕੜੇ)
{
    getTlsStorage().release_ਥਾਂ(ਕੁੰਜੀ_2, ਅੰਕੜੇ, ਸੱਚ);
}

ਖਾਲੀ TLSDataContainer::release()
{
    ਜੇ (ਕੁੰਜੀ_2 == -1)
        ਵਾਪਸ;  // already released
    std::vector<ਖਾਲੀ*> ਅੰਕੜੇ; ਅੰਕੜੇ.reserve(32);
    getTlsStorage().release_ਥਾਂ(ਕੁੰਜੀ_2, ਅੰਕੜੇ, ਝੂਠ); // Release key and get stored data for proper destruction
    ਕੁੰਜੀ_2 = -1;
    ਲਈ(size_t i = 0; i < ਅੰਕੜੇ.size(); i++)  // Delete all associated data
        delete_ਅੰਕੜੇ_Instance(ਅੰਕੜੇ[i]);
}

ਖਾਲੀ TLSDataContainer::cleanup()
{
    std::vector<ਖਾਲੀ*> ਅੰਕੜੇ; ਅੰਕੜੇ.reserve(32);
    getTlsStorage().release_ਥਾਂ(ਕੁੰਜੀ_2, ਅੰਕੜੇ, ਸੱਚ); // Extract stored data with removal from TLS tables
    ਲਈ(size_t i = 0; i < ਅੰਕੜੇ.size(); i++)  // Delete all associated data
        delete_ਅੰਕੜੇ_Instance(ਅੰਕੜੇ[i]);
}

ਖਾਲੀ* TLSDataContainer::get_ਅੰਕੜੇ() ਅਚਲ
{
    CV_Assert(ਕੁੰਜੀ_2 != -1 && "Can't fetch data from terminated TLS container.");
    ਖਾਲੀ* p_ਅੰਕੜੇ = getTlsStorage().get_ਅੰਕੜੇ(ਕੁੰਜੀ_2); // Check if data was already allocated
    ਜੇ(!p_ਅੰਕੜੇ)
    {
        // Create new data instance and save it to TLS storage
        p_ਅੰਕੜੇ = create_ਅੰਕੜੇ_Instance();
        ਕੋਸ਼ਿਸ਼
        {
            getTlsStorage().ਸੈੱਟ_ਅੰਕੜੇ(ਕੁੰਜੀ_2, p_ਅੰਕੜੇ);
        }
        ਫੜੋ (...)
        {
            delete_ਅੰਕੜੇ_Instance(p_ਅੰਕੜੇ);
            ਸੁੱਟੋ;
        }
    }
    ਵਾਪਸ p_ਅੰਕੜੇ;
}

ਸਥਿਰ TLSData<CoreTLSData>& get_Core_Tls_ਅੰਕੜੇ_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, ਨਵਾਂ TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_ਅੰਕੜੇ()
{
    ਵਾਪਸ get_Core_Tls_ਅੰਕੜੇ_TLS().getRef();
}

#ਜੇ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ WINRT
    #ਹਦਾਇਤ warning(disable:4447) // Disable warning 'main' signature found without threading model
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

ਬਾਹਰੀ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ਜੇ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ਜੇ (lpReserved != ਨਿਰਾ) // called after ExitProcess() call
        {
            cv::__termination = ਸੱਚ;
        }
        ਨਹੀਂਤਾਂ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ਵਾਪਸ TRUE;
}
#ਜੇ_ਅੰਤ


ਨਾਮਾਕਾਸ਼ {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
ਤਾਰਕਿਕ overrideThreadName()
{
    ਸਥਿਰ ਤਾਰਕਿਕ param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", ਝੂਠ);
    ਵਾਪਸ param;
}
#ਜੇ_ਅੰਤ

ਸਥਿਰ ਪੂਰਨ g_thread_ਅੰਕ = 0;
ਵਰਗ ThreadID {
ਜਨਤਕ:
    ਅਚਲ ਪੂਰਨ id;
    ThreadID() :
        id(CV_XADD(&g_thread_ਅੰਕ, 1))
    {
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_WITH_ITT
        ਜੇ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#ਜੇ_ਅੰਤ
    }
};

ਸਥਿਰ TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, ਨਵਾਂ TLSData<ThreadID>());
}

} // namespace
ਪੂਰਨ utils::getThreadID() { ਵਾਪਸ getThreadIDTLS().get()->id; }


ਵਰਗ ParseError
{
    std::string bad_ਮੁੱਲ;
ਜਨਤਕ:
    ParseError(ਅਚਲ std::string &bad_ਮੁੱਲ_2) :bad_ਮੁੱਲ(bad_ਮੁੱਲ_2) {}
    std::string ਮੰਜ਼ਿਲ_String(ਅਚਲ std::string &param) ਅਚਲ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_ਮੁੱਲ;
        ਵਾਪਸ out.str();
    }
};

ਸਾਂਚਾ <ਕਿਸਮ_ਨਾਂ T>
T parseOption(ਅਚਲ std::string &);

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ ਤਾਰਕਿਕ parseOption(ਅਚਲ std::string & value)
{
    ਜੇ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ਵਾਪਸ ਸੱਚ;
    }
    ਜੇ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ਵਾਪਸ ਝੂਠ;
    }
    ਸੁੱਟੋ ParseError(value);
}

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ size_t parseOption(ਅਚਲ std::string &value)
{
    size_t pos = 0;
    ਲਈ (; pos < value.size(); pos++)
    {
        ਜੇ (!ਅੰਕ_ਹੈ(value[pos]))
            ਰੋਕੋ;
    }
    cv::String ਮੁੱਲ_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(ਮੁੱਲ_Str);
    ਜੇ (suffixStr.length() == 0)
        ਵਾਪਸ v;
    ਨਹੀਂਤਾਂ ਜੇ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ਵਾਪਸ v * 1024 * 1024;
    ਨਹੀਂਤਾਂ ਜੇ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ਵਾਪਸ v * 1024;
    ਸੁੱਟੋ ParseError(value);
}

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ cv::String parseOption(ਅਚਲ std::string &value)
{
    ਵਾਪਸ value;
}

ਸਾਂਚਾ<>
ਸਤਰਅੰਦਰ utils::Paths parseOption(ਅਚਲ std::string &value)
{
    utils::Paths ਨਤੀਜਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ _WIN32
    ਅਚਲ ਅੱਖਰ sep = ';';
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਅੱਖਰ sep = ':';
#ਜੇ_ਅੰਤ
    size_t ਸ਼ੁਰੂ_pos = 0;
    ਜਦੋਂ (ਸ਼ੁਰੂ_pos != std::string::npos)
    {
        ਅਚਲ size_t pos = value.find(sep, ਸ਼ੁਰੂ_pos);
        ਅਚਲ std::string one_piece(value, ਸ਼ੁਰੂ_pos, pos == std::string::npos ? pos : pos - ਸ਼ੁਰੂ_pos);
        ਜੇ (!one_piece.empty())
            ਨਤੀਜਾ.push_back(one_piece);
        ਸ਼ੁਰੂ_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ਵਾਪਸ ਨਤੀਜਾ;
}

ਸਥਿਰ ਸਤਰਅੰਦਰ ਅਚਲ ਅੱਖਰ * envRead(ਅਚਲ ਅੱਖਰ * name)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_GETENV
    CV_ਅਣਵਰਤਿਆ(name);
    ਵਾਪਸ ਨਿਰਾ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ getenv(name);
#ਜੇ_ਅੰਤ
}

ਸਾਂਚਾ<ਕਿਸਮ_ਨਾਂ T>
ਸਤਰਅੰਦਰ T ਪਠ(ਅਚਲ std::string & k, ਅਚਲ T & default_ਮੁੱਲ_2)
{
    ਕੋਸ਼ਿਸ਼
    {
        ਅਚਲ ਅੱਖਰ * res = envRead(k.c_str());
        ਜੇ (res)
            ਵਾਪਸ parseOption<T>(std::string(res));
    }
    ਫੜੋ (ਅਚਲ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.ਮੰਜ਼ਿਲ_String(k));
    }
    ਵਾਪਸ default_ਮੁੱਲ_2;
}

ਤਾਰਕਿਕ utils::getConfigurationParameterBool(ਅਚਲ ਅੱਖਰ* name, ਤਾਰਕਿਕ default_ਮੁੱਲ_2)
{
    ਵਾਪਸ ਪਠ<ਤਾਰਕਿਕ>(name, default_ਮੁੱਲ_2);
}

size_t utils::get_Configuration_Parameter_ਮਾਪ_T(ਅਚਲ ਅੱਖਰ* name, size_t default_ਮੁੱਲ_2)
{
    ਵਾਪਸ ਪਠ<size_t>(name, default_ਮੁੱਲ_2);
}

std::string utils::getConfigurationParameterString(ਅਚਲ ਅੱਖਰ* name, ਅਚਲ std::string & default_ਮੁੱਲ_2)
{
    ਵਾਪਸ ਪਠ<cv::String>(name, default_ਮੁੱਲ_2);
}

utils::Paths utils::getConfigurationParameterPaths(ਅਚਲ ਅੱਖਰ* name, ਅਚਲ utils::Paths &default_ਮੁੱਲ_2)
{
    ਵਾਪਸ ਪਠ<utils::Paths>(name, default_ਮੁੱਲ_2);
}


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CV_COLLECT_IMPL_ਅੰਕੜੇ
ImplCollector& get_Impl_ਅੰਕੜੇ()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, ਨਵਾਂ ImplCollector())
}

ਖਾਲੀ ਸੈੱਟ_Impl(ਪੂਰਨ flags)
{
    cv::Auto_ਤਾਲਾ lock(get_Impl_ਅੰਕੜੇ().mutex);

    get_Impl_ਅੰਕੜੇ().implFlags = flags;
    get_Impl_ਅੰਕੜੇ().impl_ਕੋਡ.clear();
    get_Impl_ਅੰਕੜੇ().implFun.clear();
}

ਖਾਲੀ ਜੋੜੋ_Impl(ਪੂਰਨ flag, ਅਚਲ ਅੱਖਰ* func)
{
    cv::Auto_ਤਾਲਾ lock(get_Impl_ਅੰਕੜੇ().mutex);

    get_Impl_ਅੰਕੜੇ().implFlags |= flag;
    ਜੇ(func) // use lazy collection if name was not specified
    {
        size_t ਸੂਚਕਾਂਕ = get_Impl_ਅੰਕੜੇ().impl_ਕੋਡ.size();
        ਜੇ(!ਸੂਚਕਾਂਕ || (get_Impl_ਅੰਕੜੇ().impl_ਕੋਡ[ਸੂਚਕਾਂਕ-1] != flag || get_Impl_ਅੰਕੜੇ().implFun[ਸੂਚਕਾਂਕ-1].compare(func))) // avoid duplicates
        {
            get_Impl_ਅੰਕੜੇ().impl_ਕੋਡ.push_back(flag);
            get_Impl_ਅੰਕੜੇ().implFun.push_back(func);
        }
    }
}

ਪੂਰਨ getImpl(std::vector<ਪੂਰਨ> &impl, std::vector<String> &funName)
{
    cv::Auto_ਤਾਲਾ lock(get_Impl_ਅੰਕੜੇ().mutex);

    impl    = get_Impl_ਅੰਕੜੇ().impl_ਕੋਡ;
    funName = get_Impl_ਅੰਕੜੇ().implFun;
    ਵਾਪਸ get_Impl_ਅੰਕੜੇ().implFlags; // return actual flags for lazy collection
}

ਤਾਰਕਿਕ useCollection()
{
    ਵਾਪਸ get_Impl_ਅੰਕੜੇ().useCollection;
}

ਖਾਲੀ ਸੈੱਟ_Use_Collection(ਤਾਰਕਿਕ flag)
{
    cv::Auto_ਤਾਲਾ lock(get_Impl_ਅੰਕੜੇ().mutex);

    get_Impl_ਅੰਕੜੇ().useCollection = flag;
}
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ instr
{
ਤਾਰਕਿਕ useInstrumentation()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    ਵਾਪਸ getInstrumentStruct().useInstr;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਸੈੱਟ_Use_Instrumentation(ਤਾਰਕਿਕ flag)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(flag);
#ਜੇ_ਅੰਤ
}

Instr_ਗੰਢ* getTrace()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    ਵਾਪਸ &getInstrumentStruct().ਜੜ੍ਹ_ਗੰਢ_2;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ resetTrace()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    getInstrumentStruct().ਜੜ੍ਹ_ਗੰਢ_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ਗੰਢ = &getInstrumentStruct().ਜੜ੍ਹ_ਗੰਢ_2;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਸੈੱਟ_Flags(FLAGS ਬਹੁਲਕ_Flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = ਬਹੁਲਕ_Flags;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ਬਹੁਲਕ_Flags);
#ਜੇ_ਅੰਤ
}
FLAGS getFlags()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
    ਵਾਪਸ (FLAGS)getInstrumentStruct().flags;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ (FLAGS)0;
#ਜੇ_ਅੰਤ
}

ਗੰਢ_ਅੰਕੜੇ::ਗੰਢ_ਅੰਕੜੇ(ਅਚਲ ਅੱਖਰ* funName, ਅਚਲ ਅੱਖਰ* fileName, ਪੂਰਨ ਸਤਰ_ਅੰਕ, ਖਾਲੀ* ret_ਪਤਾ, ਤਾਰਕਿਕ alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_ਸਤਰ_ਅੰਕ       = ਸਤਰ_ਅੰਕ;
    m_ret_ਪਤਾ    = ret_ਪਤਾ;
    m_alwaysExpand  = alwaysExpand;

    m_ਧਾਗੇ    = 1;
    m_counter    = 0;
    m_ticks_ਕੁੱਲ = 0;

    m_funError  = ਝੂਠ;
}
ਗੰਢ_ਅੰਕੜੇ::ਗੰਢ_ਅੰਕੜੇ(ਗੰਢ_ਅੰਕੜੇ &ref)
{
    *ਇਹ = ref;
}
ਗੰਢ_ਅੰਕੜੇ& ਗੰਢ_ਅੰਕੜੇ::ਸੰਚਾਲਕ=(ਅਚਲ ਗੰਢ_ਅੰਕੜੇ &ਸੱਜੇ)
{
    ਇਹ->m_funName      = ਸੱਜੇ.m_funName;
    ਇਹ->m_instrType    = ਸੱਜੇ.m_instrType;
    ਇਹ->m_implType     = ਸੱਜੇ.m_implType;
    ਇਹ->m_fileName     = ਸੱਜੇ.m_fileName;
    ਇਹ->m_ਸਤਰ_ਅੰਕ      = ਸੱਜੇ.m_ਸਤਰ_ਅੰਕ;
    ਇਹ->m_ret_ਪਤਾ   = ਸੱਜੇ.m_ret_ਪਤਾ;
    ਇਹ->m_alwaysExpand = ਸੱਜੇ.m_alwaysExpand;

    ਇਹ->m_ਧਾਗੇ     = ਸੱਜੇ.m_ਧਾਗੇ;
    ਇਹ->m_counter     = ਸੱਜੇ.m_counter;
    ਇਹ->m_ticks_ਕੁੱਲ  = ਸੱਜੇ.m_ticks_ਕੁੱਲ;

    ਇਹ->m_funError    = ਸੱਜੇ.m_funError;

    ਵਾਪਸ *ਇਹ;
}
ਗੰਢ_ਅੰਕੜੇ::~ਗੰਢ_ਅੰਕੜੇ()
{
}
ਤਾਰਕਿਕ ਸੰਚਾਲਕ==(ਅਚਲ ਗੰਢ_ਅੰਕੜੇ& ਖੱਬੇ, ਅਚਲ ਗੰਢ_ਅੰਕੜੇ& ਸੱਜੇ)
{
    ਜੇ(ਖੱਬੇ.m_ਸਤਰ_ਅੰਕ == ਸੱਜੇ.m_ਸਤਰ_ਅੰਕ && ਖੱਬੇ.m_funName == ਸੱਜੇ.m_funName && ਖੱਬੇ.m_fileName == ਸੱਜੇ.m_fileName)
    {
        ਜੇ(ਖੱਬੇ.m_ret_ਪਤਾ == ਸੱਜੇ.m_ret_ਪਤਾ || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_ਨਾਂ || ਖੱਬੇ.m_alwaysExpand))
            ਵਾਪਸ ਸੱਚ;
    }
    ਵਾਪਸ ਝੂਠ;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ਸਥਿਰ InstrStruct instr;
    ਵਾਪਸ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ਵਾਪਸ *getInstrumentStruct().tlsStruct.get();
}

Instr_ਗੰਢ* get_Current_ਗੰਢ()
{
    ਵਾਪਸ getInstrumentTLSStruct().p_Current_ਗੰਢ;
}

IntrumentationRegion::IntrumentationRegion(ਅਚਲ ਅੱਖਰ* funName, ਅਚਲ ਅੱਖਰ* fileName, ਪੂਰਨ ਸਤਰ_ਅੰਕ, ਖਾਲੀ *ret_ਪਤਾ, ਤਾਰਕਿਕ alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = ਝੂਠ;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ਜੇ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ਜੇ(!pTLS->p_Current_ਗੰਢ)
        {
            m_disabled = ਸੱਚ;
            ਵਾਪਸ;
        }

        ਪੂਰਨ ਡੂੰਘਾਈ = pTLS->p_Current_ਗੰਢ->get_ਡੂੰਘਾਈ();
        ਜੇ(pStruct->max_ਡੂੰਘਾਈ && pStruct->max_ਡੂੰਘਾਈ <= ਡੂੰਘਾਈ)
        {
            m_disabled = ਸੱਚ;
            ਵਾਪਸ;
        }

        ਗੰਢ_ਅੰਕੜੇ payload(funName, fileName, ਸਤਰ_ਅੰਕ, ret_ਪਤਾ, alwaysExpand, instrType, implType);
        Node<ਗੰਢ_ਅੰਕੜੇ>* p_ਬੱਚਾ = ਨਿਰਾ;

        ਜੇ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_ਤਾਲਾ guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_ਬੱਚਾ = pTLS->p_Current_ਗੰਢ->ਲੱਭੋ_ਬੱਚਾ(payload);
            ਜੇ(!p_ਬੱਚਾ)
            {
                p_ਬੱਚਾ = ਨਵਾਂ Node<ਗੰਢ_ਅੰਕੜੇ>(payload);
                pTLS->p_Current_ਗੰਢ->ਜੋੜੋ_ਬੱਚਾ(p_ਬੱਚਾ);
            }
        }
        ਨਹੀਂਤਾਂ
        {
            p_ਬੱਚਾ = pTLS->p_Current_ਗੰਢ->ਲੱਭੋ_ਬੱਚਾ(payload);
            ਜੇ(!p_ਬੱਚਾ)
            {
                m_disabled = ਸੱਚ;
                ਵਾਪਸ;
            }
        }
        pTLS->p_Current_ਗੰਢ = p_ਬੱਚਾ;

        m_regionTicks = get_Tick_ਗਿਣਤੀ();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ਜੇ(pStruct->useInstr)
    {
        ਜੇ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ਜੇ (pTLS->p_Current_ਗੰਢ->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ਗੰਢ->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ਗੰਢ->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::ਅੰਤ_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ਗਿਣਤੀ() - m_regionTicks);
            {
                cv::Auto_ਤਾਲਾ guard(pStruct->mutex_ਗਿਣਤੀ); // Concurrent ticks accumulation
                pTLS->p_Current_ਗੰਢ->m_payload.m_counter++;
                pTLS->p_Current_ਗੰਢ->m_payload.m_ticks_ਕੁੱਲ += ticks;
                pTLS->p_Current_ਗੰਢ->m_payload.m_tls.get()->m_ticks_ਕੁੱਲ += ticks;
            }

            pTLS->p_Current_ਗੰਢ = pTLS->p_Current_ਗੰਢ->m_p_ਮਾਪਾ;
        }
    }
}
#ਜੇ_ਅੰਤ
}

ਨਾਮਾਕਾਸ਼ ipp
{

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਬਣਤਰ IPPInitSingleton
{
ਜਨਤਕ:
    IPPInitSingleton()
    {
        useIPP         = ਸੱਚ;

#ਜੇ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ਨਹੀਂਤਾਂ
        useIPP_NE      = ਝੂਠ;
#ਜੇ_ਅੰਤ
        ipp_ਸਥਿਤੀ      = 0;
        funcname       = ਨਿਰਾ;
        filename       = ਨਿਰਾ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_ਸਿਖਰ_Features = 0;
        pIppLibInfo    = ਨਿਰਾ;

        ipp_ਸਥਿਤੀ = ippGetCpuFeatures(&cpuFeatures, ਨਿਰਾ);
        ਜੇ(ipp_ਸਥਿਤੀ < 0)
        {
            CV_LOG_ERROR(ਨਿਰਾ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = ਝੂਠ;
            ਵਾਪਸ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ਜੇ(!env.empty())
        {
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201900
            ਅਚਲ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#ਜਾਂਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201703
            ਅਚਲ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ਜਾਂਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201700
            ਅਚਲ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ਨਹੀਂਤਾਂ
            ਅਚਲ Ipp64u minorFeatures = 0;
#ਜੇ_ਅੰਤ

            env = ਮੰਜ਼ਿਲ_ਹੇਠਲਾ_Case(env);
            ਜੇ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = ਸੱਚ;
                env = env.substr(3, env.size());
            }

            ਜੇ(env == "disabled")
            {
                CV_LOG_WARNING(ਨਿਰਾ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = ਝੂਠ;
            }
            ਨਹੀਂਤਾਂ ਜੇ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ਨਹੀਂਤਾਂ ਜੇ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201700
#ਜੇ defined (_M_AMD64) || defined (__x86_64__)
            ਨਹੀਂਤਾਂ ਜੇ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
            ਨਹੀਂਤਾਂ
                CV_LOG_ERROR(ਨਿਰਾ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ਜੇ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ਜੇ(!(
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#ਜੇ_ਅੰਤ
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = ਝੂਠ;
            ਵਾਪਸ;
        }

        ਜੇ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ਨਹੀਂਤਾਂ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ਜੇ IPP_ਸੰਸਕਰਣ_X100 >= 201700
        ਜੇ(ippFeatures&ippCPUID_AVX512F)
        {
            ਜੇ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_ਸਿਖਰ_Features = ippCPUID_AVX512_SKX;
            ਨਹੀਂਤਾਂ ਜੇ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_ਸਿਖਰ_Features = ippCPUID_AVX512_KNL;
            ਨਹੀਂਤਾਂ
                ipp_ਸਿਖਰ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
        ਜੇ(ippFeatures&ippCPUID_AVX2)
            ipp_ਸਿਖਰ_Features = ippCPUID_AVX2;
        ਨਹੀਂਤਾਂ ਜੇ(ippFeatures&ippCPUID_SSE42)
            ipp_ਸਿਖਰ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_ਸੰਸਕਰਣ();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ਜੇ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_ਸਿਖਰ_Features = ippCPUID_SSE42;
        }
    }

ਜਨਤਕ:
    ਤਾਰਕਿਕ        useIPP;
    ਤਾਰਕਿਕ        useIPP_NE;

    ਪੂਰਨ         ipp_ਸਥਿਤੀ;  // 0 - all is ok, -1 - IPP functions failed
    ਅਚਲ ਅੱਖਰ *funcname;
    ਅਚਲ ਅੱਖਰ *filename;
    ਪੂਰਨ         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_ਸਿਖਰ_Features;
    ਅਚਲ Ipp_Library_ਸੰਸਕਰਣ *pIppLibInfo;
};

ਸਥਿਰ IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, ਨਵਾਂ IPPInitSingleton())
}
#ਜੇ_ਅੰਤ

ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ getIppFeatures()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਵਾਪਸ getIPPSingleton().ippFeatures;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ get_Ipp_ਸਿਖਰ_Features()
{
    ਵਾਪਸ getIPPSingleton().ipp_ਸਿਖਰ_Features;
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਸੈੱਟ_Ipp_ਸਥਿਤੀ(ਪੂਰਨ ਸਥਿਤੀ, ਅਚਲ ਅੱਖਰ * ਅਚਲ _funcname, ਅਚਲ ਅੱਖਰ * ਅਚਲ _filename, ਪੂਰਨ ਸਤਰ_3)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    getIPPSingleton().ipp_ਸਥਿਤੀ = ਸਥਿਤੀ;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = ਸਤਰ_3;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ਸਥਿਤੀ); CV_ਅਣਵਰਤਿਆ(_funcname); CV_ਅਣਵਰਤਿਆ(_filename); CV_ਅਣਵਰਤਿਆ(ਸਤਰ_3);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ get_Ipp_ਸਥਿਤੀ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਵਾਪਸ getIPPSingleton().ipp_ਸਥਿਤੀ;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

String getIppErrorLocation()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਵਾਪਸ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ String();
#ਜੇ_ਅੰਤ
}

String get_Ipp_ਸੰਸਕਰਣ()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਅਚਲ Ipp_Library_ਸੰਸਕਰਣ *pInfo = getIPPSingleton().pIppLibInfo;
    ਜੇ(pInfo)
        ਵਾਪਸ format("%s %s %s", pInfo->Name, pInfo->ਸੰਸਕਰਣ_4, pInfo->BuildDate);
    ਨਹੀਂਤਾਂ
        ਵਾਪਸ String("error");
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ String("disabled");
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ useIPP()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    ਜੇ (ਅੰਕੜੇ.useIPP < 0)
    {
        ਅੰਕੜੇ.useIPP = getIPPSingleton().useIPP;
    }
    ਵਾਪਸ (ਅੰਕੜੇ.useIPP > 0);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਸੈੱਟ_Use_IPP(ਤਾਰਕਿਕ flag)
{
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਅੰਕੜੇ.useIPP = (getIPPSingleton().useIPP)?flag:ਝੂਠ;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(flag);
    ਅੰਕੜੇ.useIPP = ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ useIPP_NotExact()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
    ਜੇ (ਅੰਕੜੇ.useIPP_NE < 0)
    {
        ਅੰਕੜੇ.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ਵਾਪਸ (ਅੰਕੜੇ.useIPP_NE > 0);
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ ਸੈੱਟ_Use_IPP_Not_Exact(ਤਾਰਕਿਕ flag)
{
    CoreTLSData& ਅੰਕੜੇ = get_Core_Tls_ਅੰਕੜੇ();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_IPP
    ਅੰਕੜੇ.useIPP_NE = flag;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(flag);
    ਅੰਕੜੇ.useIPP_NE = ਝੂਠ;
#ਜੇ_ਅੰਤ
}

} // namespace ipp


ਨਾਮਾਕਾਸ਼ details {

#ਜੇ OPENCV_IMPL_FP_HINTS_X86
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _MM_DENORMALS_ZERO_ON 0x0040
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MM_DENORMALS_ZERO_ਨਕਾਬ  // requires pmmintrin.h (SSE3)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MM_DENORMALS_ZERO_ਨਕਾਬ 0x0040
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

ਖਾਲੀ ਸੈੱਟ_FPDenormals_Ignore_Hint(ਤਾਰਕਿਕ ignore, CV_ਬਾਹਰਲਾ FPDenormals_ਬਹੁਲਕ_State& state)
{
#ਜੇ OPENCV_IMPL_FP_HINTS_X86
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = MM_FLUSH_ZERO_ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ਜੇ (featuresEnabled.have[CPU_SSE3])
    {
        ਨਕਾਬ |= MM_DENORMALS_ZERO_ਨਕਾਬ;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ old_flags = _mm_getcsr();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਮੁੱਲ = old_flags & ਨਕਾਬ;
    ਬਿਨਾਨਿਸ਼ਾਨੀ flags = (old_flags & ~ਨਕਾਬ) | value;
    CV_LOG_DEBUG(ਨਿਰਾ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)ਨਕਾਬ;
    state.reserved[1] = (uint32_t)old_ਮੁੱਲ;
    _mm_setcsr(flags);
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(ignore); CV_ਅਣਵਰਤਿਆ(state);
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ saveFPDenormalsState(CV_ਬਾਹਰਲਾ FPDenormals_ਬਹੁਲਕ_State& state)
{
#ਜੇ OPENCV_IMPL_FP_HINTS_X86
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = MM_FLUSH_ZERO_ਨਕਾਬ;
    ਜੇ (featuresEnabled.have[CPU_SSE3])
    {
        ਨਕਾਬ |= MM_DENORMALS_ZERO_ਨਕਾਬ;
    }
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ old_flags = _mm_getcsr();
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ old_ਮੁੱਲ = old_flags & ਨਕਾਬ;
    // save state
    state.reserved[0] = (uint32_t)ਨਕਾਬ;
    state.reserved[1] = (uint32_t)old_ਮੁੱਲ;
    ਵਾਪਸ 2;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(state);
    ਵਾਪਸ 0;
#ਜੇ_ਅੰਤ
}

ਤਾਰਕਿਕ restoreFPDenormalsState(ਅਚਲ FPDenormals_ਬਹੁਲਕ_State& state)
{
#ਜੇ OPENCV_IMPL_FP_HINTS_X86
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਨਕਾਬ = (ਬਿਨਾਨਿਸ਼ਾਨੀ)state.reserved[0];
    CV_DbgAssert(ਨਕਾਬ != 0); // invalid state (ensure that state is properly saved earlier)
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ value = (ਬਿਨਾਨਿਸ਼ਾਨੀ)state.reserved[1];
    CV_DbgCheck((ਪੂਰਨ)value, value == (value & ਨਕਾਬ), "invalid SSE FP state");
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ old_flags = _mm_getcsr();
    ਬਿਨਾਨਿਸ਼ਾਨੀ flags = (old_flags & ~ਨਕਾਬ) | value;
    CV_LOG_DEBUG(ਨਿਰਾ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ਵਾਪਸ ਸੱਚ;
#ਨਹੀਂਤਾਂ
    CV_ਅਣਵਰਤਿਆ(state);
    ਵਾਪਸ ਝੂਠ;
#ਜੇ_ਅੰਤ
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_ALGO_HINT_DEFAULT
    ਵਾਪਸ OPENCV_ALGO_HINT_DEFAULT;
#ਨਹੀਂਤਾਂ
    ਵਾਪਸ ALGO_HINT_ACCURATE;
#ਜੇ_ਅੰਤ
};

} // namespace cv

/* End of file. */
