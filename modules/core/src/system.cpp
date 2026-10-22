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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#ಅನ್ತರ್ಭಾವ <atomic>
#ಅನ್ತರ್ಭಾವ <exception>
#ಅನ್ತರ್ಭಾವ <iostream>
#ಅನ್ತರ್ಭಾವ <ostream>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __QNX__
    #ಅನ್ತರ್ಭಾವ <unistd.h>
    #ಅನ್ತರ್ಭಾವ <sys/neutrino.h>
    #ಅನ್ತರ್ಭಾವ <sys/syspage.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __aarch64__
    #ಅನ್ತರ್ಭಾವ <aarch64/syspage.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/trace.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/tls.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/instrumentation.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/filesystem.private.hpp>

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control_utils.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/fp_control.private.hpp>

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಶೂನ್ಯ _initSystem()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    ಸ್ಥಿರ std::ios_base::Init s_iostream_initializer;
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ Mutex* __initialization_mutex = ಖಾಲಿ;
Mutex& getInitializationMutex()
{
    ಆದರೆ (__initialization_mutex == ಖಾಲಿ)
    {
        (ಶೂನ್ಯ)_initSystem();
        __initialization_mutex = ಹೊಸ Mutex();
    }
    ಹಿಂತಿರುಗಿ *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

ಸ್ಥಿರ ತಾರ್ಕಿಕ param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ಆದರೆ defined(_DEBUG) || defined(__ANDROID__)
    ನಿಜ
#ಇಲ್ಲದಿದ್ದರೆ
    ಸುಳ್ಳು
#ಆದರೆ_ಅಂತ್ಯ
);

ಶೂನ್ಯ* alloc_Singleton_ಬಫರ್(size_t size) { ಹಿಂತಿರುಗಿ fastMalloc(size); }
ಶೂನ್ಯ* alloc_Singleton_New_ಬಫರ್(size_t size) { ಹಿಂತಿರುಗಿ ಸ್ಮರಣೆ_ಕೊಡು(size); }


} // namespace cv

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_ERROR_ಗಣ_TERMINATE_HANDLER  // build config option
# ಆದರೆ defined(_WIN32)
#   define CV_ERROR_ಗಣ_TERMINATE_HANDLER 1
# endif
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined(CV_ERROR_ಗಣ_TERMINATE_HANDLER) && !CV_ERROR_ಗಣ_TERMINATE_HANDLER
# undef CV_ERROR_ಗಣ_TERMINATE_HANDLER
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
# ಆದರೆ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_ERROR_ಗಣ_TERMINATE_HANDLER
#ಅನ್ತರ್ಭಾವ <cstdlib>        // std::abort
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ಆದರೆ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
ಬಳಸಿ Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
ಸ್ಥಿರಾಂಕ uint64_t AT_HWCAP = NT_GNU_HWCAP;
#ಇಲ್ಲದಿದ್ದರೆ
#  include <elf.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#ಆದರೆ_ಅಂತ್ಯ


#ಆದರೆ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __loongarch64
#ಅನ್ತರ್ಭಾವ "sys/auxv.h"
#ವ್ಯಾಖ್ಯಾನಿಸು LA_HWCAP_LSX   (1<<4)
#ವ್ಯಾಖ್ಯಾನಿಸು LA_HWCAP_LASX  (1<<5)
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined _WIN32 || defined WINCE
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #ವ್ಯಾಖ್ಯಾನಿಸು _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <windows.h>
#ಅನ್ತರ್ಭಾವ <combaseapi.h>
#ಆದರೆ (_WIN32_WINNT >= 0x0602)
  #ಅನ್ತರ್ಭಾವ <synchapi.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #ಅನ್ತರ್ಭಾವ <fibersapi.h>
  #ವ್ಯಾಖ್ಯಾನಿಸು CV_USE_FLS
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು small
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು min
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು max
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು ನಿರಪೇಕ್ಷ
#ಅನ್ತರ್ಭಾವ <tchar.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
#ಅನ್ತರ್ಭಾವ <wrl/client.h>
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __cplusplus_winrt
#ಅನ್ತರ್ಭಾವ <windows.storage.h>
#ಸೂಚನೆ comment(lib, "runtimeobject.lib")
#ಆದರೆ_ಅಂತ್ಯ // WINRT

std::wstring Get_ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ_Win_RT()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __cplusplus_winrt
    ಹಿಂತಿರುಗಿ std::wstring(Windows::Storage::Application_ದತ್ತಾಂಶ::Current->TemporaryFolder->ಮಾರ್ಗ_2->ದತ್ತಾಂಶ_4());
#ಇಲ್ಲದಿದ್ದರೆ
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ದತ್ತಾಂಶ_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_ದತ್ತಾಂಶ> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_ಅಂಶ> storageitemRef;
    HSTRING str;
    HSTRING_ಶೀರ್ಷಿಕೆ hstr_ತಲೆ;
    std::wstring wstr;
    ಆದರೆ (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_ದತ್ತಾಂಶ,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_ದತ್ತಾಂಶ), &hstr_ತಲೆ, &str)))
        ಹಿಂತಿರುಗಿ wstr;
    ಆದರೆ (FAILED(RoGetActivationFactory(str, IID_PPV_ವಾದಗಳು(appdataFactory.Release_And_Get_ವಿಳಾಸ_ದ()))))
        ಹಿಂತಿರುಗಿ wstr;
    ಆದರೆ (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_ವಿಳಾಸ_ದ())))
        ಹಿಂತಿರುಗಿ wstr;
    ಆದರೆ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_ವಿಳಾಸ_ದ())))
        ಹಿಂತಿರುಗಿ wstr;
    ಆದರೆ (FAILED(storagefolderRef.As(&storageitemRef)))
        ಹಿಂತಿರುಗಿ wstr;
    str = ಖಾಲಿ;
    ಆದರೆ (FAILED(storageitemRef->get_ಮಾರ್ಗ(&str)))
        ಹಿಂತಿರುಗಿ wstr;
    wstr = Windows_Get_String_Raw_ಬಫರ್(str, ಖಾಲಿ);
    WindowsDeleteString(str);
    ಹಿಂತಿರುಗಿ wstr;
#ಆದರೆ_ಅಂತ್ಯ
}

std::wstring Get_ತಾತ್ಕಾಲಿಕ_File_Name_Win_RT(std::wstring ಪೂರ್ವಪ್ರತ್ಯಯ)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* ಮುಖವಾಡ = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], ಗಾತ್ರ(guidStr)/ಗಾತ್ರ(wchar_t), ಮುಖವಾಡ,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    ಹಿಂತಿರುಗಿ ಪೂರ್ವಪ್ರತ್ಯಯ.append(std::wstring(guidStr));
}

#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT
#ಅನ್ತರ್ಭಾವ <pthread.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <sys/time.h>
#ಅನ್ತರ್ಭಾವ <time.h>

#ಆದರೆ defined __MACH__ && defined __APPLE__
#ಅನ್ತರ್ಭಾವ <mach/mach.h>
#ಅನ್ತರ್ಭಾವ <mach/mach_time.h>
#ಅನ್ತರ್ಭಾವ <sys/sysctl.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _OPENMP
#ಅನ್ತರ್ಭಾವ "omp.h"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ defined __ANDROID__
#ಅನ್ತರ್ಭಾವ <sys/sysconf.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
# include <android/log.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_CPUID_X86
  #ಆದರೆ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ಆದರೆ _MSC_VER >= 1400  // MSVS 2005
      #ಅನ್ತರ್ಭಾವ <intrin.h>  // __cpuidex()
      #ವ್ಯಾಖ್ಯಾನಿಸು CV_CPUID_X86 __cpuidex
    #ಇಲ್ಲದಿದ್ದರೆ
      #ದೋಷ "Required MSVS 2005+"
    #ಆದರೆ_ಅಂತ್ಯ
  #ಅಥವಾಆದರೆ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    ಸ್ಥಿರ ಶೂನ್ಯ cv_cpuid(ಪೂರ್ಣಾಂಕ* cpuid_ದತ್ತಾಂಶ, ಪೂರ್ಣಾಂಕ reg_eax, ಪೂರ್ಣಾಂಕ reg_ecx)
    {
        ಪೂರ್ಣಾಂಕ __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ಆದರೆ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ಅಥವಾಆದರೆ defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#ಇಲ್ಲದಿದ್ದರೆ
#ದೋಷ "Configuration error"
#ಆದರೆ_ಅಂತ್ಯ
        cpuid_ದತ್ತಾಂಶ[0] = __eax; cpuid_ದತ್ತಾಂಶ[1] = __ebx; cpuid_ದತ್ತಾಂಶ[2] = __ecx; cpuid_ದತ್ತಾಂಶ[3] = __edx;
    }
    #ವ್ಯಾಖ್ಯಾನಿಸು CV_CPUID_X86 cv_cpuid
  #ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <chrono>

ನಾಮಾಕಾಶ cv
{

Exception::Exception() { ಸಂಕೇತ_2 = Error::StsOk; ಸಾಲು = 0; }

Exception::Exception(Error::ಸಂಕೇತ_4 ಸಂಕೇತ_5, ಸ್ಥಿರಾಂಕ String& _err, ಸ್ಥಿರಾಂಕ String& _func, ಸ್ಥಿರಾಂಕ String& _file, ಪೂರ್ಣಾಂಕ ಸಾಲು_3)
: ಸಂಕೇತ_2(ಸಂಕೇತ_5), err(_err), func(_func), file(_file), ಸಾಲು(ಸಾಲು_3)
{
    format_ಸಂದೇಶ();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* Exception::what() ಸ್ಥಿರಾಂಕ CV_NOEXCEPT { ಹಿಂತಿರುಗಿ ಸಂದೇಶ.c_str(); }

ಶೂನ್ಯ Exception::format_ಸಂದೇಶ()
{
    size_t pos = err.find('\n');
    ತಾರ್ಕಿಕ multiline = pos != cv::String::npos;
    ಆದರೆ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        ಆಗ (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::ಸಾಲು_ಅಂತ್ಯ;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ಆದರೆ (err[err.size() - 1] != '\n')
            ss << std::ಸಾಲು_ಅಂತ್ಯ;
        err = ss.str();
    }
    ಆದರೆ (func.size() > 0)
    {
        ಆದರೆ (multiline)
            ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_ಆವೃತ್ತಿ, file.c_str(), ಸಾಲು, ಸಂಕೇತ_2, ಸಂಕೇತ_ಸಂದೇಶ(), func.c_str(), err.c_str());
        ಇಲ್ಲದಿದ್ದರೆ
            ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_ಆವೃತ್ತಿ, file.c_str(), ಸಾಲು, ಸಂಕೇತ_2, ಸಂಕೇತ_ಸಂದೇಶ(), err.c_str(), func.c_str());
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        ಸಂದೇಶ = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_ಆವೃತ್ತಿ, file.c_str(), ಸಾಲು, ಸಂಕೇತ_2, ಸಂಕೇತ_ಸಂದೇಶ(), err.c_str(), multiline ? "" : "\n");
    }
}

ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * Exception::ಸಂಕೇತ_ಸಂದೇಶ() ಸ್ಥಿರಾಂಕ
{
    ಆಯ್ಕೆ (ಸಂಕೇತ_2)
    {
    ಸ್ಥಿತಿ cv::Error::StsOk :                  ಹಿಂತಿರುಗಿ "No Error";
    ಸ್ಥಿತಿ cv::Error::Sts_ಹಿಂಭಾಗ_Trace :           ಹಿಂತಿರುಗಿ "Backtrace";
    ಸ್ಥಿತಿ cv::Error::StsError :               ಹಿಂತಿರುಗಿ "Unspecified error";
    ಸ್ಥಿತಿ cv::Error::Sts_ಆಂತರಿಕ :            ಹಿಂತಿರುಗಿ "Internal error";
    ಸ್ಥಿತಿ cv::Error::StsNoMem :               ಹಿಂತಿರುಗಿ "Insufficient memory";
    ಸ್ಥಿತಿ cv::Error::StsBadArg :              ಹಿಂತಿರುಗಿ "Bad argument";
    ಸ್ಥಿತಿ cv::Error::StsNoConv :              ಹಿಂತಿರುಗಿ "Iterations do not converge";
    ಸ್ಥಿತಿ cv::Error::StsAutoTrace :           ಹಿಂತಿರುಗಿ "Autotrace call";
    ಸ್ಥಿತಿ cv::Error::Sts_Bad_ಗಾತ್ರ :             ಹಿಂತಿರುಗಿ "Incorrect size of input array";
    ಸ್ಥಿತಿ cv::Error::StsNullPtr :             ಹಿಂತಿರುಗಿ "Null pointer";
    ಸ್ಥಿತಿ cv::Error::StsDivByZero :           ಹಿಂತಿರುಗಿ "Division by zero occurred";
    ಸ್ಥಿತಿ cv::Error::Bad_ಹೆಜ್ಜೆ :                ಹಿಂತಿರುಗಿ "Image step is wrong";
    ಸ್ಥಿತಿ cv::Error::StsInplaceNotSupported : ಹಿಂತಿರುಗಿ "Inplace operation is not supported";
    ಸ್ಥಿತಿ cv::Error::Sts_Object_Not_ಸಿಕ್ಕಿತು :      ಹಿಂತಿರುಗಿ "Requested object was not found";
    ಸ್ಥಿತಿ cv::Error::Bad_ಆಳ :               ಹಿಂತಿರುಗಿ "Input image depth is not supported by function";
    ಸ್ಥಿತಿ cv::Error::StsUnmatchedFormats :    ಹಿಂತಿರುಗಿ "Formats of input arguments do not match";
    ಸ್ಥಿತಿ cv::Error::StsUnmatchedSizes :      ಹಿಂತಿರುಗಿ "Sizes of input arguments do not match";
    ಸ್ಥಿತಿ cv::Error::Sts_ಹೊರಹೋಗು_ದ_Range :          ಹಿಂತಿರುಗಿ "One of the arguments\' values is out of range";
    ಸ್ಥಿತಿ cv::Error::StsUnsupportedFormat :   ಹಿಂತಿರುಗಿ "Unsupported format or combination of formats";
    ಸ್ಥಿತಿ cv::Error::BadCOI :                 ಹಿಂತಿರುಗಿ "Input COI is not supported";
    ಸ್ಥಿತಿ cv::Error::Bad_ಅಂಕೆ_Channels :         ಹಿಂತಿರುಗಿ "Bad number of channels";
    ಸ್ಥಿತಿ cv::Error::StsBadFlag :             ಹಿಂತಿರುಗಿ "Bad flag (parameter or structure field)";
    ಸ್ಥಿತಿ cv::Error::Sts_Bad_ಬಿಂದು :            ಹಿಂತಿರುಗಿ "Bad parameter of type CvPoint";
    ಸ್ಥಿತಿ cv::Error::Sts_Bad_ಮುಖವಾಡ :             ಹಿಂತಿರುಗಿ "Bad type of mask argument";
    ಸ್ಥಿತಿ cv::Error::StsParseError :          ಹಿಂತಿರುಗಿ "Parsing error";
    ಸ್ಥಿತಿ cv::Error::StsNotImplemented :      ಹಿಂತಿರುಗಿ "The function/feature is not implemented";
    ಸ್ಥಿತಿ cv::Error::Sts_Bad_Mem_ಖಂಡ :         ಹಿಂತಿರುಗಿ "Memory block has been corrupted";
    ಸ್ಥಿತಿ cv::Error::StsAssert :              ಹಿಂತಿರುಗಿ "Assertion failed";
    ಸ್ಥಿತಿ cv::Error::GpuNotSupported :        ಹಿಂತಿರುಗಿ "No CUDA support";
    ಸ್ಥಿತಿ cv::Error::GpuApiCallError :        ಹಿಂತಿರುಗಿ "Gpu API call";
    ಸ್ಥಿತಿ cv::Error::OpenGlNotSupported :     ಹಿಂತಿರುಗಿ "No OpenGL support";
    ಸ್ಥಿತಿ cv::Error::OpenGlApiCallError :     ಹಿಂತಿರುಗಿ "OpenGL API call";
    ಸ್ಥಿತಿ cv::Error::OpenCLApiCallError :     ಹಿಂತಿರುಗಿ "OpenCL API call error";
    ಸ್ಥಿತಿ cv::Error::OpenCLDoubleNotSupported:ಹಿಂತಿರುಗಿ "OpenCL device does not support double";
    ಸ್ಥಿತಿ cv::Error::OpenCLInitError :        ಹಿಂತಿರುಗಿ "OpenCL initialization error";
    ಸ್ಥಿತಿ cv::Error::OpenCLNoAMDBlasFft :     ಹಿಂತಿರುಗಿ "Missing OpenCL AMD BLAS FFT";
    ಪೂರ್ವನಿಯೋಜಿತ:
    {
        ಸ್ಥಿರ ಅಕ್ಷರ buf[256] {0};
        snprintf(buf, ಗಾತ್ರ(buf), "Unknown %s code %d", ಸಂಕೇತ_2 >= 0 ? "status":"error", ಸಂಕೇತ_2);
        ಹಿಂತಿರುಗಿ buf;
    }
    };

}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* g_hw_Feature_ಹೆಸರುಗಳು[CV_HARDWARE_MAX_FEATURE] = { ಖಾಲಿ };

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* getHWFeatureName(ಪೂರ್ಣಾಂಕ id)
{
    ಹಿಂತಿರುಗಿ (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_ಹೆಸರುಗಳು[id] : ಖಾಲಿ;
}
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* get_HWFeature_Name_ಸುರಕ್ಷಿತ(ಪೂರ್ಣಾಂಕ id)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name = getHWFeatureName(id);
    ಹಿಂತಿರುಗಿ name ? name : "Unknown feature";
}

ರಚನೆ HWFeatures
{
    ಎಣಿಕೆ { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(ತಾರ್ಕಿಕ ಸತತ_initialize = ಸುಳ್ಳು)
    {
        ಆದರೆ (ಸತತ_initialize)
            initialize();
    }

    ಸ್ಥಿರ ಶೂನ್ಯ initialize_ಹೆಸರುಗಳು()
    {
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_ಹೆಸರುಗಳು[i] = 0;
        }
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_MMX] = "MMX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSE] = "SSE";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSE2] = "SSE2";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSE3] = "SSE3";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_FP16] = "FP16";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX] = "AVX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX2] = "AVX2";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_FMA3] = "FMA3";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_NEON] = "NEON";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_SVE] = "SVE";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_VSX] = "VSX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_VSX3] = "VSX3";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_RVV] = "RVV";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_ಹೆಸರುಗಳು[CPU_LSX]  = "LSX";
        g_hw_Feature_ಹೆಸರುಗಳು[CPU_LASX] = "LASX";
    }

    ಶೂನ್ಯ initialize(ಶೂನ್ಯ)
    {
        ಆದರೆ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_ಹೆಸರುಗಳು();

    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_CPUID_X86
        ಪೂರ್ಣಾಂಕ cpuid_ದತ್ತಾಂಶ[4] = { 0, 0, 0, 0 };
        ಪೂರ್ಣಾಂಕ cpuid_ದತ್ತಾಂಶ_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_ದತ್ತಾಂಶ, 1, 0/*ಬಳಸದ*/);

        ಪೂರ್ಣಾಂಕ x86_family = (cpuid_ದತ್ತಾಂಶ[0] >> 8) & 15;
        ಆದರೆ( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_ದತ್ತಾಂಶ[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_ದತ್ತಾಂಶ[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_ದತ್ತಾಂಶ[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_ದತ್ತಾಂಶ[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_ದತ್ತಾಂಶ[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_ದತ್ತಾಂಶ[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_ದತ್ತಾಂಶ[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_ದತ್ತಾಂಶ[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_ದತ್ತಾಂಶ[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_ದತ್ತಾಂಶ[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_ದತ್ತಾಂಶ[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_ದತ್ತಾಂಶ_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_ದತ್ತಾಂಶ_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_ದತ್ತಾಂಶ_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_ದತ್ತಾಂಶ_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_ದತ್ತಾಂಶ_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_ದತ್ತಾಂಶ_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_ದತ್ತಾಂಶ_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_ದತ್ತಾಂಶ_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_ದತ್ತಾಂಶ_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            ಪೂರ್ಣಾಂಕ cpuid_ದತ್ತಾಂಶ_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_ದತ್ತಾಂಶ_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_ದತ್ತಾಂಶ_ex1[0] & (1<<4))  != 0;

            ತಾರ್ಕಿಕ have_AVX_OS_support = ನಿಜ;
            ತಾರ್ಕಿಕ have_AVX512_OS_support = ನಿಜ;
            ಆದರೆ (!(cpuid_ದತ್ತಾಂಶ[2] & (1<<27)))
                have_AVX_OS_support = ಸುಳ್ಳು; // OS uses XSAVE_XRSTORE and CPU support AVX
            ಇಲ್ಲದಿದ್ದರೆ
            {
                ಪೂರ್ಣಾಂಕ xcr0 = 0;
            #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ XCR_XFEATURE_ENABLED_ಮುಖವಾಡ // requires immintrin.h
                xcr0 = (ಪೂರ್ಣಾಂಕ)_xgetbv(XCR_XFEATURE_ENABLED_ಮುಖವಾಡ);
            #ಅಥವಾಆದರೆ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #ಆದರೆ_ಅಂತ್ಯ
                ಆದರೆ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = ಸುಳ್ಳು; // YMM registers
                ಆದರೆ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = ಸುಳ್ಳು; // ZMM registers
            }

            ಆದರೆ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = ಸುಳ್ಳು;
                have[CV_CPU_FP16] = ಸುಳ್ಳು;
                have[CV_CPU_AVX2] = ಸುಳ್ಳು;
                have[CV_CPU_FMA3] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_VNNI] = ಸುಳ್ಳು;
            }
            ಆದರೆ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512BW] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512CD] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512DQ] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512ER] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512IFMA] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512PF] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512VBMI] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512VL] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512VBMI2] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512VNNI] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512BITALG] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_512VPOPCNTDQ] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_5124VNNIW] = ಸುಳ್ಳು;
                have[CV_CPU_AVX_5124FMAPS] = ಸುಳ್ಳು;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ಆದರೆ (have[CV_CPU_AVX512_COMMON])
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
            ಇಲ್ಲದಿದ್ದರೆ
            {
                have[CV_CPU_AVX512_KNL] = ಸುಳ್ಳು;
                have[CV_CPU_AVX512_KNM] = ಸುಳ್ಳು;
                have[CV_CPU_AVX512_SKX] = ಸುಳ್ಳು;
                have[CV_CPU_AVX512_CNL] = ಸುಳ್ಳು;
                have[CV_CPU_AVX512_CLX] = ಸುಳ್ಳು;
                have[CV_CPU_AVX512_ICL] = ಸುಳ್ಳು;
            }
        }
    #ಆದರೆ_ಅಂತ್ಯ // CV_CPUID_X86

    #ಆದರೆ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __aarch64__
        have[CV_CPU_NEON] = ನಿಜ;
        have[CV_CPU_FP16] = ನಿಜ;
        ಪೂರ್ಣಾಂಕ cpufile = open("/proc/self/auxv", O_RDONLY);

        ಆದರೆ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            ಸ್ಥಿರಾಂಕ size_t ಗಾತ್ರ_auxv_t = ಗಾತ್ರ(auxv);

            ಆಗ ((size_t)ಪಠ(cpufile, &auxv, ಗಾತ್ರ_auxv_t) == ಗಾತ್ರ_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ಆದರೆ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ಆದರೆ defined(AT_HWCAP2)
                ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#ಆದರೆ_ಅಂತ್ಯ
            }

            close(cpufile);
        }
    #ಅಥವಾಆದರೆ defined __arm__ && defined __ANDROID__
      #ಆದರೆ defined HAVE_CPUFEATURES
        CV_LOG_INFO(ಖಾಲಿ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ಖಾಲಿ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (ದೀರ್ಘ ದೀರ್ಘ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #ಇಲ್ಲದಿದ್ದರೆ
        CV_LOG_INFO(ಖಾಲಿ, "cpufeatures library is not available for CPU detection");
        #ಆದರೆ CV_NEON
        CV_LOG_INFO(ಖಾಲಿ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = ನಿಜ;
        #ಇಲ್ಲದಿದ್ದರೆ
        CV_LOG_INFO(ಖಾಲಿ, "- NEON instructions is NOT enabled via build flags");
        #ಆದರೆ_ಅಂತ್ಯ
        #ಆದರೆ CV_FP16
        CV_LOG_INFO(ಖಾಲಿ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = ನಿಜ;
        #ಇಲ್ಲದಿದ್ದರೆ
        CV_LOG_INFO(ಖಾಲಿ, "- FP16 instructions is NOT enabled via build flags");
        #ಆದರೆ_ಅಂತ್ಯ
      #ಆದರೆ_ಅಂತ್ಯ
    #ಅಥವಾಆದರೆ defined __arm__
        ಪೂರ್ಣಾಂಕ cpufile = open("/proc/self/auxv", O_RDONLY);

        ಆದರೆ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            ಸ್ಥಿರಾಂಕ size_t ಗಾತ್ರ_auxv_t = ಗಾತ್ರ(auxv);

            ಆಗ ((size_t)ಪಠ(cpufile, &auxv, ಗಾತ್ರ_auxv_t) == ಗಾತ್ರ_auxv_t)
            {
                ಆದರೆ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ನಿಲ್ಲಿಸು;
                }
            }

            close(cpufile);
        }
    #ಆದರೆ_ಅಂತ್ಯ
    #ಅಥವಾಆದರೆ (defined __APPLE__)
    #ಆದರೆ defined __ARM_NEON
        have[CV_CPU_NEON] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    ಪೂರ್ಣಾಂಕ has_feat_dotprod = 0;
    size_t has_feat_dotprod_ಗಾತ್ರ = ಗಾತ್ರ(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_ಗಾತ್ರ, ಖಾಲಿ, 0);
    ಆದರೆ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = ನಿಜ;
    }
    ಪೂರ್ಣಾಂಕ has_feat_bf16 = 0;
    size_t has_feat_bf16_ಗಾತ್ರ = ಗಾತ್ರ(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_ಗಾತ್ರ, ಖಾಲಿ, 0);
    ಆದರೆ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = ನಿಜ;
    }
    #ಅಥವಾಆದರೆ (defined __clang__)
    #ಆದರೆ defined __ARM_NEON
        have[CV_CPU_NEON] = ನಿಜ;
        #ಆದರೆ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = ನಿಜ;
        #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __riscv_vector
        have[CV_CPU_RISCVV] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ
    #ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __mips_msa
        have[CV_CPU_MSA] = ನಿಜ;
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ hwcap = getauxval(AT_HWCAP);
        ಆದರೆ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ಆದರೆ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ನಿಜ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ಅಥವಾಆದರೆ (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ hwcap = 0, hwcap2 = 0;
        ಆದರೆ (elf_aux_info(AT_HWCAP, &hwcap, ಗಾತ್ರ(hwcap)) != 0)
            hwcap = 0;
        ಆದರೆ (hwcap & PPC_FEATURE_HAS_VSX) {
            ಆದರೆ (elf_aux_info(AT_HWCAP2, &hwcap2, ಗಾತ್ರ(hwcap2)) != 0)
                hwcap2 = 0;
            ಆದರೆ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = ನಿಜ;
            } ಇಲ್ಲದಿದ್ದರೆ {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #ಇಲ್ಲದಿದ್ದರೆ
        // TODO: AIX
        #ಆದರೆ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = ನಿಜ;
        #ಆದರೆ_ಅಂತ್ಯ
        #ಆದರೆ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = ನಿಜ;
        #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = ನಿಜ;
        #ಆದರೆ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = ನಿಜ;
        #ಆದರೆ_ಅಂತ್ಯ
        #ಆದರೆ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = ನಿಜ;
        #ಆದರೆ_ಅಂತ್ಯ
    #ಆದರೆ_ಅಂತ್ಯ

    #ಆದರೆ defined __loongarch64 && defined __linux__
        ಪೂರ್ಣಾಂಕ flag = (ಪೂರ್ಣಾಂಕ)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #ಆದರೆ_ಅಂತ್ಯ

        ತಾರ್ಕಿಕ skip_baseline_check = ಸುಳ್ಳು;
        ಆದರೆ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = ನಿಜ;
        }
        ಪೂರ್ಣಾಂಕ baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ಆದರೆ (!checkFeatures(baseline_features, ಗಾತ್ರ(baseline_features) / ಗಾತ್ರ(baseline_features[0]))
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
            checkFeatures(baseline_features, ಗಾತ್ರ(baseline_features) / ಗಾತ್ರ(baseline_features[0]), ನಿಜ);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, ಗಾತ್ರ(baseline_features) / ಗಾತ್ರ(baseline_features[0]));
    }

    ತಾರ್ಕಿಕ checkFeatures(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* features, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3, ತಾರ್ಕಿಕ dump = ಸುಳ್ಳು)
    {
        ತಾರ್ಕಿಕ ಫಲಿತಾಂಶ = ನಿಜ;
        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < ಎಣಿಕೆ_3; i++)
        {
            ಪೂರ್ಣಾಂಕ feature = features[i];
            ಆದರೆ (feature)
            {
                ಆದರೆ (have[feature])
                {
                    ಆದರೆ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_ಸುರಕ್ಷಿತ(feature));
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಫಲಿತಾಂಶ = ಸುಳ್ಳು;
                    ಆದರೆ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_ಸುರಕ್ಷಿತ(feature));
                }
            }
        }
        ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
    }

    ಸ್ಥಿರ ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ is_ಚಿಹ್ನೆ_Separator(ಅಕ್ಷರ c)
    {
        ಹಿಂತಿರುಗಿ c == ',' || c == ';';
    }

    ಶೂನ್ಯ readSettings(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* baseline_features, ಪೂರ್ಣಾಂಕ baseline_ಎಣಿಕೆ)
    {
        ತಾರ್ಕಿಕ dump = ನಿಜ;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ಆದರೆ (!disabled_features.empty())
        {
            ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಆರಂಭ = disabled_features.c_str();
            ಗಾಗಿ (;;)
            {
                ಆಗ (ಆರಂಭ[0] != 0 && is_ಚಿಹ್ನೆ_Separator(ಆರಂಭ[0]))
                {
                    ಆರಂಭ++;
                }
                ಆದರೆ (ಆರಂಭ[0] == 0)
                    ನಿಲ್ಲಿಸು;
                ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* end = ಆರಂಭ;
                ಆಗ (end[0] != 0 && !is_ಚಿಹ್ನೆ_Separator(end[0]))
                {
                    end++;
                }
                ಆದರೆ (end == ಆರಂಭ)
                    ಮುಂದುವರಿ;
                cv::String feature(ಆರಂಭ, end);
                ಆರಂಭ = end;

                CV_Assert(feature.size() > 0);

                ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
                ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ಆದರೆ (!g_hw_Feature_ಹೆಸರುಗಳು[i]) ಮುಂದುವರಿ;
                    size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(g_hw_Feature_ಹೆಸರುಗಳು[i]);
                    ಆದರೆ (len != feature.size()) ಮುಂದುವರಿ;
                    ಆದರೆ (feature.compare(g_hw_Feature_ಹೆಸರುಗಳು[i]) == 0)
                    {
                        ತಾರ್ಕಿಕ isBaseline = ಸುಳ್ಳು;
                        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ k = 0; k < baseline_ಎಣಿಕೆ; k++)
                        {
                            ಆದರೆ (baseline_features[k] == i)
                            {
                                isBaseline = ನಿಜ;
                                ನಿಲ್ಲಿಸು;
                            }
                        }
                        ಆದರೆ (isBaseline)
                        {
                            ಆದರೆ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_ಸುರಕ್ಷಿತ(i));
                        }
                        ಆದರೆ (!have[i])
                        {
                            ಆದರೆ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_ಸುರಕ್ಷಿತ(i));
                        }
                        have[i] = ಸುಳ್ಳು;

                        ಸಿಕ್ಕಿತು = ನಿಜ;
                        ನಿಲ್ಲಿಸು;
                    }
                }
                ಆದರೆ (!ಸಿಕ್ಕಿತು)
                {
                    ಆದರೆ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    ತಾರ್ಕಿಕ have[MAX_FEATURE+1]{};
};

ಸ್ಥಿರ HWFeatures  featuresEnabled(ನಿಜ), featuresDisabled = HWFeatures(ಸುಳ್ಳು);
ಸ್ಥಿರ HWFeatures* currentFeatures = &featuresEnabled;

ತಾರ್ಕಿಕ checkHardwareSupport(ಪೂರ್ಣಾಂಕ feature)
{
    ಆದರೆ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ currentFeatures->have[feature];
}

String getHardwareFeatureName(ಪೂರ್ಣಾಂಕ feature)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name = getHWFeatureName(feature);
    ಹಿಂತಿರುಗಿ name ? String(name) : String();
}

std::string get_CPUFeatures_ಸಾಲು()
{
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ sz = ಗಾತ್ರ(features) / ಗಾತ್ರ(features[0]);
    std::string ಫಲಿತಾಂಶ;
    std::string ಪೂರ್ವಪ್ರತ್ಯಯ;
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 1; i < sz; ++i)
    {
        ಆದರೆ (features[i] == 0)
        {
            ಪೂರ್ವಪ್ರತ್ಯಯ = "*";
            ಮುಂದುವರಿ;
        }
        ಆದರೆ (i != 1) ಫಲಿತಾಂಶ.append(" ");
        ಫಲಿತಾಂಶ.append(ಪೂರ್ವಪ್ರತ್ಯಯ);
        ಫಲಿತಾಂಶ.append(get_HWFeature_Name_ಸುರಕ್ಷಿತ(features[i]));
        ಆದರೆ (!checkHardwareSupport(features[i])) ಫಲಿತಾಂಶ.append("?");
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಅಸ್ಥಿರ ತಾರ್ಕಿಕ useOptimizedFlag = ನಿಜ;

ಶೂನ್ಯ ಗಣ_Use_Optimized( ತಾರ್ಕಿಕ flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::ಗಣ_Use_IPP(flag);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
    ocl::ಗಣ_Use_Open_CL(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ useOptimized(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ useOptimizedFlag;
}

int64 get_Tick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    ಹಿಂತಿರುಗಿ (int64)now.time_since_epoch().ಎಣಿಕೆ_3();
}

ದ್ವಿಗುಣ getTickFrequency(ಶೂನ್ಯ)
{
    ಬಳಸಿ clock_period_t = std::chrono::steady_clock::duration::period;
    ದ್ವಿಗುಣ clock_freq = clock_period_t::den / clock_period_t::num;
    ಹಿಂತಿರುಗಿ clock_freq;
}

#ಆದರೆ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ಆದರೆ defined(__i386__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    int64 x;
    __asm__ ಅಸ್ಥಿರ (".byte 0x0f, 0x31" : "=A" (x));
    ಹಿಂತಿರುಗಿ x;
}
#ಅಥವಾಆದರೆ defined(__x86_64__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಚಿಹ್ನೆರಹಿತ hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    ಹಿಂತಿರುಗಿ (int64)lo | ((int64)hi << 32);
}

#ಅಥವಾಆದರೆ defined(__ppc__)

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಚಿಹ್ನೆರಹಿತ upper, lower, tmp;
    __asm__ ಅಸ್ಥಿರ(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    ಹಿಂತಿರುಗಿ lower | ((int64)upper << 32);
}

#ಇಲ್ಲದಿದ್ದರೆ

#ದೋಷ "RDTSC not defined"

#ಆದರೆ_ಅಂತ್ಯ

#ಅಥವಾಆದರೆ defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#ಇಲ್ಲದಿದ್ದರೆ

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_ಎಣಿಕೆ(ಶೂನ್ಯ)
{
    ಹಿಂತಿರುಗಿ get_Tick_ಎಣಿಕೆ();
}
//#endif

#ಆದರೆ_ಅಂತ್ಯ


ನಾಮಾಕಾಶ internal {

ವರ್ಗ Timestamp
{
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರಾಂಕ int64 zero_Tick_ಎಣಿಕೆ;
    ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ ns_ಒಳ_ticks;

    Timestamp()
        : zero_Tick_ಎಣಿಕೆ(get_Tick_ಎಣಿಕೆ())
        , ns_ಒಳ_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_ಎಣಿಕೆ();
        ಹಿಂತಿರುಗಿ (int64)((t - zero_Tick_ಎಣಿಕೆ) * ns_ಒಳ_ticks);
    }

    ಸ್ಥಿರ Timestamp& getInstance()
    {
        ಸ್ಥಿರ Timestamp g_timestamp;
        ಹಿಂತಿರುಗಿ g_timestamp;
    }
};

ವರ್ಗ InitTimestamp {
ಸಾರ್ವಜನಿಕ:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
ಸ್ಥಿರ InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    ಹಿಂತಿರುಗಿ internal::Timestamp::getInstance().getTimestamp();
}


ಸ್ಥಿರಾಂಕ String& getBuildInformation()
{
    ಸ್ಥಿರ String build_info =
#ಅನ್ತರ್ಭಾವ "version_string.inc"
    ;
    ಹಿಂತಿರುಗಿ build_info;
}

String get_ಆವೃತ್ತಿ_String() { ಹಿಂತಿರುಗಿ String(CV_ಆವೃತ್ತಿ); }

ಪೂರ್ಣಾಂಕ get_ಆವೃತ್ತಿ_Major() { ಹಿಂತಿರುಗಿ CV_ಆವೃತ್ತಿ_MAJOR; }

ಪೂರ್ಣಾಂಕ get_ಆವೃತ್ತಿ_Minor() { ಹಿಂತಿರುಗಿ CV_ಆವೃತ್ತಿ_MINOR; }

ಪೂರ್ಣಾಂಕ get_ಆವೃತ್ತಿ_Revision() { ಹಿಂತಿರುಗಿ CV_ಆವೃತ್ತಿ_REVISION; }

String format( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt, ... )
{
    Auto_ಬಫರ್<ಅಕ್ಷರ, 1024> buf;

    ಗಾಗಿ ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        ಪೂರ್ಣಾಂಕ bsize = static_cast<ಪೂರ್ಣಾಂಕ>(buf.size());
        ಪೂರ್ಣಾಂಕ len = cv_vsnprintf(buf.ದತ್ತಾಂಶ(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ಆದರೆ (len >= bsize)
        {
            buf.resize(len + 1);
            ಮುಂದುವರಿ;
        }
        buf[bsize - 1] = 0;
        ಹಿಂತಿರುಗಿ String(buf.ದತ್ತಾಂಶ(), len);
    }
}

String tempfile( ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* suffix )
{
#ಆದರೆ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string ತಾತ್ಕಾಲಿಕ_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ಆದರೆ defined _WIN32
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring ತಾತ್ಕಾಲಿಕ_dir_rt = Get_ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ_Win_RT();

    std::wstring ತಾತ್ಕಾಲಿಕ_file = Get_ತಾತ್ಕಾಲಿಕ_File_Name_Win_RT(L"ocv");
    ಆದರೆ (ತಾತ್ಕಾಲಿಕ_file.empty())
        ಹಿಂತಿರುಗಿ String();

    ತಾತ್ಕಾಲಿಕ_file = ತಾತ್ಕಾಲಿಕ_dir_rt.append(std::wstring(L"\\")).append(ತಾತ್ಕಾಲಿಕ_file);
    DeleteFileW(ತಾತ್ಕಾಲಿಕ_file.c_str());

    ಅಕ್ಷರ aname[MAX_ಮಾರ್ಗ];
    size_t copied = wcstombs(aname, ತಾತ್ಕಾಲಿಕ_file.c_str(), MAX_ಮಾರ್ಗ);
    CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#ಅಥವಾಆದರೆ defined(_WIN32_WCE)
    ಸ್ಥಿರಾಂಕ ಸ್ವಯಂಚಾಲಿತ k_Max_ಮಾರ್ಗ_ಗಾತ್ರ = MAX_ಮಾರ್ಗ+1;
    wchar_t ತಾತ್ಕಾಲಿಕ_dir_ce[k_Max_ಮಾರ್ಗ_ಗಾತ್ರ] = {0};
    wchar_t ತಾತ್ಕಾಲಿಕ_file[k_Max_ಮಾರ್ಗ_ಗಾತ್ರ] = {0};

    ::Get_ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ_W(k_Max_ಮಾರ್ಗ_ಗಾತ್ರ, ತಾತ್ಕಾಲಿಕ_dir_ce);

    ಆದರೆ(0 != ::Get_ತಾತ್ಕಾಲಿಕ_File_Name_W(ತಾತ್ಕಾಲಿಕ_dir_ce, L"ocv", 0, ತಾತ್ಕಾಲಿಕ_file)) {
        DeleteFileW(ತಾತ್ಕಾಲಿಕ_file);
        ಅಕ್ಷರ aname[MAX_ಮಾರ್ಗ];
        size_t copied = wcstombs(aname, ತಾತ್ಕಾಲಿಕ_file, MAX_ಮಾರ್ಗ);
        CV_Assert((copied != MAX_ಮಾರ್ಗ) && (copied != (size_t)-1));
        fname = String(aname);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t ತಾತ್ಕಾಲಿಕ_dir2_w[MAX_ಮಾರ್ಗ] = { 0 };

    ಆದರೆ (ತಾತ್ಕಾಲಿಕ_dir.empty())
    {
        ::Get_ತಾತ್ಕಾಲಿಕ_ಮಾರ್ಗ_W(ಗಾತ್ರ(ತಾತ್ಕಾಲಿಕ_dir2_w)/ಗಾತ್ರ(wchar_t), ತಾತ್ಕಾಲಿಕ_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        ಪೂರ್ಣಾಂಕ len = Wide_Char_ಗಮ್ಯ_Multi_Byte(CP_UTF8, 0, ತಾತ್ಕಾಲಿಕ_dir2_w, -1, ಖಾಲಿ, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (len > 0)
        {
            std::vector<ಅಕ್ಷರ> utf8_buf(len);
            Wide_Char_ಗಮ್ಯ_Multi_Byte(CP_UTF8, 0, ತಾತ್ಕಾಲಿಕ_dir2_w, -1, utf8_buf.ದತ್ತಾಂಶ(), len, ಖಾಲಿ, ಖಾಲಿ);
            ತಾತ್ಕಾಲಿಕ_dir = std::string(utf8_buf.ದತ್ತಾಂಶ());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ಆದರೆ (FAILED(hr))
        ಹಿಂತಿರುಗಿ String();
    ಅಕ್ಷರ guidStr[40];
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಮುಖವಾಡ = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, ಗಾತ್ರ(guidStr), ಮುಖವಾಡ,
            g.Data1, g.Data2, g.Data3, (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[0], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[1],
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[2], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[3], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[4],
            (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[5], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[6], (ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ)g.Data4[7]);

    fname = ತಾತ್ಕಾಲಿಕ_dir;
    ಆದರೆ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#ಆದರೆ_ಅಂತ್ಯ
# ಇಲ್ಲದಿದ್ದರೆ
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    ಅಕ್ಷರ defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  ಇಲ್ಲದಿದ್ದರೆ
    ಅಕ್ಷರ defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ಆದರೆ (ತಾತ್ಕಾಲಿಕ_dir.empty())
        fname = defaultTemplate;
    ಇಲ್ಲದಿದ್ದರೆ
    {
        fname = ತಾತ್ಕಾಲಿಕ_dir;
        ಅಕ್ಷರ ech = fname[fname.size() - 1];
        ಆದರೆ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ fd = mkstemp((ಅಕ್ಷರ*)fname.c_str());
    ಆದರೆ (fd == -1) ಹಿಂತಿರುಗಿ String();

    close(fd);
    remove(fname.c_str());
# endif

    ಆದರೆ (suffix)
    {
        ಆದರೆ (suffix[0] != '.')
            ಹಿಂತಿರುಗಿ fname + "." + suffix;
        ಇಲ್ಲದಿದ್ದರೆ
            ಹಿಂತಿರುಗಿ fname + suffix;
    }
    ಹಿಂತಿರುಗಿ fname;
#ಇಲ್ಲದಿದ್ದರೆ // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_ಬಳಸದ(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#ಆದರೆ_ಅಂತ್ಯ // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

ಸ್ಥಿರ ErrorCallback customErrorCallback = 0;
ಸ್ಥಿರ ಶೂನ್ಯ* custom_Error_Callback_ದತ್ತಾಂಶ = 0;
ಸ್ಥಿರ ತಾರ್ಕಿಕ breakOnError = ಸುಳ್ಳು;

ತಾರ್ಕಿಕ ಗಣ_Break_On_Error(ತಾರ್ಕಿಕ value)
{
    ತಾರ್ಕಿಕ prevVal = breakOnError;
    breakOnError = value;
    ಹಿಂತಿರುಗಿ prevVal;
}

ಪೂರ್ಣಾಂಕ cv_snprintf(ಅಕ್ಷರ* buf, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    ಪೂರ್ಣಾಂಕ res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    ಹಿಂತಿರುಗಿ res;
}

ಪೂರ್ಣಾಂಕ cv_vsnprintf(ಅಕ್ಷರ* buf, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt, va_list ವಾದಗಳು)
{
#ಆದರೆ defined _MSC_VER
    ಆದರೆ (len <= 0) ಹಿಂತಿರುಗಿ len == 0 ? 1024 : -1;
    ಪೂರ್ಣಾಂಕ res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, ವಾದಗಳು);
    // ensure null terminating on VS
    ಆದರೆ (res >= 0 && res < len)
    {
        buf[res] = 0;
        ಹಿಂತಿರುಗಿ res;
    }
    ಇಲ್ಲದಿದ್ದರೆ
    {
        buf[len - 1] = 0; // truncate happened
        ಹಿಂತಿರುಗಿ res >= len ? res : (len * 2);
    }
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ vsnprintf(buf, len, fmt, ವಾದಗಳು);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ ಶೂನ್ಯ dumpException(ಸ್ಥಿರಾಂಕ Exception& exc)
{
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* errorStr = exc.ಸಂಕೇತ_ಸಂದೇಶ();
    ಅಕ್ಷರ buf[1 << 12];

    cv_snprintf(buf, ಗಾತ್ರ(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_ಆವೃತ್ತಿ,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.ಸಾಲು);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#ಇಲ್ಲದಿದ್ದರೆ
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_ERROR_ಗಣ_TERMINATE_HANDLER
ಸ್ಥಿರ ತಾರ್ಕಿಕ cv_terminate_handler_installed = ಸುಳ್ಳು;
ಸ್ಥಿರ std::terminate_handler cv_old_terminate_handler;
ಸ್ಥಿರ cv::Exception cv_terminate_handler_exception;
ಸ್ಥಿರ ತಾರ್ಕಿಕ param_ಸಿದ್ಧತೆ_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", ನಿಜ);
ಸ್ಥಿರ ಶೂನ್ಯ cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ಆದರೆ (ಸುಳ್ಳು /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
# ಆದರೆ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ error( ಸ್ಥಿರಾಂಕ Exception& exc )
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_ERROR_ಗಣ_TERMINATE_HANDLER
    {
        cv::Auto_ಬೀಗ lock(getInitializationMutex());
        ಆದರೆ (!cv_terminate_handler_installed)
        {
            ಆದರೆ (param_ಸಿದ್ಧತೆ_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = ನಿಜ;
        }
        cv_terminate_handler_exception = exc;
    }
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ (customErrorCallback != 0)
        customErrorCallback(exc.ಸಂಕೇತ_2, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.ಸಾಲು, custom_Error_Callback_ದತ್ತಾಂಶ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (param_dumpErrors)
    {
        dumpException(exc);
    }

    ಆದರೆ(breakOnError)
    {
        std::terminate();
    }

    ಎಸೆ exc;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
# ಆದರೆ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ error(Error::ಸಂಕೇತ_4 ಸಂಕೇತ_5, ಸ್ಥಿರಾಂಕ String& _err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _func, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _file, ಪೂರ್ಣಾಂಕ ಸಾಲು_3)
{
    error(cv::Exception(ಸಂಕೇತ_5, _err, _func, _file, ಸಾಲು_3));
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
# ಆದರೆ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
# ಆದರೆ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#ಆದರೆ_ಅಂತ್ಯ


ErrorCallback
redirectError( ErrorCallback errCallback, ಶೂನ್ಯ* userdata, ಶೂನ್ಯ** prevUserdata)
{
    ಆದರೆ( prevUserdata )
        *prevUserdata = custom_Error_Callback_ದತ್ತಾಂಶ;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_ದತ್ತಾಂಶ = userdata;

    ಹಿಂತಿರುಗಿ prevCallback;
}

ಶೂನ್ಯ terminate(Error::ಸಂಕೇತ_4 ಸಂಕೇತ_5, ಸ್ಥಿರಾಂಕ String& _err, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _func, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* _file, ಪೂರ್ಣಾಂಕ ಸಾಲು_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(ಸಂಕೇತ_5, _err, _func, _file, ಸಾಲು_3));
    std::terminate();
}

}

ನಾಮಾಕಾಶ cv {
ತಾರ್ಕಿಕ __termination = ಸುಳ್ಳು;


//////////////////////////////// thread-local storage ////////////////////////////////

ನಾಮಾಕಾಶ details {

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#ಸೂಚನೆ warning(disable:4505) // unreferenced local function has been removed
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ TLS_ಹೊರಹೋಗು_ದ_INDEXES
#ವ್ಯಾಖ್ಯಾನಿಸು TLS_ಹೊರಹೋಗು_ದ_INDEXES ((DWORD)0xFFFFFFFF)
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

// TLS platform abstraction layer
ವರ್ಗ TlsAbstraction
{
ಸಾರ್ವಜನಿಕ:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    ಶೂನ್ಯ* get_ದತ್ತಾಂಶ() ಸ್ಥಿರಾಂಕ;
    ಶೂನ್ಯ ಗಣ_ದತ್ತಾಂಶ(ಶೂನ್ಯ *p_ದತ್ತಾಂಶ);

    ಶೂನ್ಯ releaseSystemResources();

ಖಾಸಗಿ:

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ WINRT
    DWORD tls_ಕೀಲಿ;
    ತಾರ್ಕಿಕ disposed;
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ // _WIN32
    pthread_key_t  tls_ಕೀಲಿ;
    std::atomic<ತಾರ್ಕಿಕ> disposed;
#ಆದರೆ_ಅಂತ್ಯ
};

ವರ್ಗ TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
ಸಾರ್ವಜನಿಕ:
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
ಸ್ಥಿರ TlsAbstraction* getTlsAbstraction()
{
    ಸ್ಥಿರ TlsAbstraction *g_tls = ಹೊಸ TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    ಸ್ಥಿರ TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    ಹಿಂತಿರುಗಿ g_tls;
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
ಸ್ಥಿರ __declspec( thread ) ಶೂನ್ಯ* tls_ದತ್ತಾಂಶ = ಖಾಲಿ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ನಿಜ;  // DllMain is missing in static builds
}
ಶೂನ್ಯ* TlsAbstraction::get_ದತ್ತಾಂಶ() ಸ್ಥಿರಾಂಕ
{
    ಹಿಂತಿರುಗಿ tls_ದತ್ತಾಂಶ;
}
ಶೂನ್ಯ TlsAbstraction::ಗಣ_ದತ್ತಾಂಶ(ಶೂನ್ಯ *p_ದತ್ತಾಂಶ)
{
    tls_ದತ್ತಾಂಶ = p_ದತ್ತಾಂಶ;
}
#ಇಲ್ಲದಿದ್ದರೆ //WINRT
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_USE_FLS
ಸ್ಥಿರ ಶೂನ್ಯ NTAPI opencv_fls_destructor(ಶೂನ್ಯ* p_ದತ್ತಾಂಶ);
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(ಸುಳ್ಳು)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_USE_FLS
    tls_ಕೀಲಿ = TlsAlloc();
#ಇಲ್ಲದಿದ್ದರೆ // CV_USE_FLS
    tls_ಕೀಲಿ = FlsAlloc(opencv_fls_destructor);
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
    CV_Assert(tls_ಕೀಲಿ != TLS_ಹೊರಹೋಗು_ದ_INDEXES);
}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ನಿಜ;  // DllMain is missing in static builds
    disposed = ನಿಜ;
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_USE_FLS
    TlsFree(tls_ಕೀಲಿ);
#ಇಲ್ಲದಿದ್ದರೆ // CV_USE_FLS
    FlsFree(tls_ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
    tls_ಕೀಲಿ = TLS_ಹೊರಹೋಗು_ದ_INDEXES;
}
ಶೂನ್ಯ* TlsAbstraction::get_ದತ್ತಾಂಶ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (disposed)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_USE_FLS
    ಹಿಂತಿರುಗಿ Tls_Get_ಮೌಲ್ಯ(tls_ಕೀಲಿ);
#ಇಲ್ಲದಿದ್ದರೆ // CV_USE_FLS
    ಹಿಂತಿರುಗಿ Fls_Get_ಮೌಲ್ಯ(tls_ಕೀಲಿ);
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
}
ಶೂನ್ಯ TlsAbstraction::ಗಣ_ದತ್ತಾಂಶ(ಶೂನ್ಯ *p_ದತ್ತಾಂಶ)
{
    ಆದರೆ (disposed)
        ಹಿಂತಿರುಗಿ;  // no-op
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_USE_FLS
    CV_Assert(Tls_ಗಣ_ಮೌಲ್ಯ(tls_ಕೀಲಿ, p_ದತ್ತಾಂಶ) == TRUE);
#ಇಲ್ಲದಿದ್ದರೆ // CV_USE_FLS
    CV_Assert(Fls_ಗಣ_ಮೌಲ್ಯ(tls_ಕೀಲಿ, p_ದತ್ತಾಂಶ) == TRUE);
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
}
#ಆದರೆ_ಅಂತ್ಯ // WINRT
#ಇಲ್ಲದಿದ್ದರೆ // _WIN32
ಸ್ಥಿರ ಶೂನ್ಯ opencv_tls_destructor(ಶೂನ್ಯ* p_ದತ್ತಾಂಶ);
TlsAbstraction::TlsAbstraction()
    : disposed(ಸುಳ್ಳು)
{
    CV_Assert(pthread_key_create(&tls_ಕೀಲಿ, opencv_tls_destructor) == 0);
}
ಶೂನ್ಯ TlsAbstraction::releaseSystemResources()
{
    cv::__termination = ನಿಜ;  // DllMain is missing in static builds
    disposed = ನಿಜ;
    ಆದರೆ (pthread_key_delete(tls_ಕೀಲಿ) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
ಶೂನ್ಯ* TlsAbstraction::get_ದತ್ತಾಂಶ() ಸ್ಥಿರಾಂಕ
{
    ಆದರೆ (disposed)
        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    ಹಿಂತಿರುಗಿ pthread_getspecific(tls_ಕೀಲಿ);
}
ಶೂನ್ಯ TlsAbstraction::ಗಣ_ದತ್ತಾಂಶ(ಶೂನ್ಯ *p_ದತ್ತಾಂಶ)
{
    ಆದರೆ (disposed)
        ಹಿಂತಿರುಗಿ;  // no-op
    CV_Assert(pthread_setspecific(tls_ಕೀಲಿ, p_ದತ್ತಾಂಶ) == 0);
}
#ಆದರೆ_ಅಂತ್ಯ

// Per-thread data structure
ರಚನೆ Thread_ದತ್ತಾಂಶ
{
    Thread_ದತ್ತಾಂಶ()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<ಶೂನ್ಯ*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


ಸ್ಥಿರ ತಾರ್ಕಿಕ g_isTlsStorageInitialized = ಸುಳ್ಳು;

// Main TLS storage class
ವರ್ಗ TlsStorage
{
ಸಾರ್ವಜನಿಕ:
    TlsStorage() :
        tls_Slots_ಗಾತ್ರ(0)
    {
        (ಶೂನ್ಯ)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        ಎಳೆಗಳು.reserve(32);
        g_isTlsStorageInitialized = ನಿಜ;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    ಶೂನ್ಯ releaseThread(ಶೂನ್ಯ* tls_ಮೌಲ್ಯ = ಖಾಲಿ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ಆದರೆ (ಖಾಲಿ == tls)
            ಹಿಂತಿರುಗಿ;  // TLS singleton is not available (terminated)
        Thread_ದತ್ತಾಂಶ *pTD = tls_ಮೌಲ್ಯ == ಖಾಲಿ ? (Thread_ದತ್ತಾಂಶ*)tls->get_ದತ್ತಾಂಶ() : (Thread_ದತ್ತಾಂಶ*)tls_ಮೌಲ್ಯ;
        ಆದರೆ (pTD == ಖಾಲಿ)
            ಹಿಂತಿರುಗಿ;  // no OpenCV TLS data for this thread
        Auto_ಬೀಗ guard(mtxGlobalAccess);
        ಗಾಗಿ (size_t i = 0; i < ಎಳೆಗಳು.size(); i++)
        {
            ಆದರೆ (pTD == ಎಳೆಗಳು[i])
            {
                ಎಳೆಗಳು[i] = ಖಾಲಿ;
                ಆದರೆ (tls_ಮೌಲ್ಯ == ಖಾಲಿ)
                    tls->ಗಣ_ದತ್ತಾಂಶ(0);
                std::vector<ಶೂನ್ಯ*>& thread_slots = pTD->slots;
                ಗಾಗಿ (size_t ಸ್ಥಾನ_Idx = 0; ಸ್ಥಾನ_Idx < thread_slots.size(); ಸ್ಥಾನ_Idx++)
                {
                    ಶೂನ್ಯ* p_ದತ್ತಾಂಶ = thread_slots[ಸ್ಥಾನ_Idx];
                    thread_slots[ಸ್ಥಾನ_Idx] = ಖಾಲಿ;
                    ಆದರೆ (!p_ದತ್ತಾಂಶ)
                        ಮುಂದುವರಿ;
                    TLSDataContainer* container = tlsSlots[ಸ್ಥಾನ_Idx].container;
                    ಆದರೆ (container)
                        container->delete_ದತ್ತಾಂಶ_Instance(p_ದತ್ತಾಂಶ);
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನ_Idx);
                        fflush(stderr);
                    }
                }
                ಅಳಿಸು pTD;
                ಹಿಂತಿರುಗಿ;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (ಶೂನ್ಯ*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_ಸ್ಥಾನ(TLSDataContainer* container)
    {
        Auto_ಬೀಗ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ಗಾತ್ರ == tlsSlots.size());

        // Find unused slots
        ಗಾಗಿ(size_t ಸ್ಥಾನ = 0; ಸ್ಥಾನ < tls_Slots_ಗಾತ್ರ; ಸ್ಥಾನ++)
        {
            ಆದರೆ (tlsSlots[ಸ್ಥಾನ].container == ಖಾಲಿ)
            {
                tlsSlots[ಸ್ಥಾನ].container = container;
                ಹಿಂತಿರುಗಿ ಸ್ಥಾನ;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_ಸ್ಥಾನ_Info(container)); tls_Slots_ಗಾತ್ರ++;
        ಹಿಂತಿರುಗಿ tls_Slots_ಗಾತ್ರ - 1;
    }

    // Release TLS storage index and pass associated data to caller
    ಶೂನ್ಯ release_ಸ್ಥಾನ(size_t ಸ್ಥಾನ_Idx, std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ_Vec, ತಾರ್ಕಿಕ keep_ಸ್ಥಾನ = ಸುಳ್ಳು)
    {
        Auto_ಬೀಗ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ಗಾತ್ರ == tlsSlots.size());
        CV_Assert(tls_Slots_ಗಾತ್ರ > ಸ್ಥಾನ_Idx);

        ಗಾಗಿ(size_t i = 0; i < ಎಳೆಗಳು.size(); i++)
        {
            ಆದರೆ(ಎಳೆಗಳು[i])
            {
                std::vector<ಶೂನ್ಯ*>& thread_slots = ಎಳೆಗಳು[i]->slots;
                ಆದರೆ (thread_slots.size() > ಸ್ಥಾನ_Idx && thread_slots[ಸ್ಥಾನ_Idx])
                {
                    ದತ್ತಾಂಶ_Vec.push_back(thread_slots[ಸ್ಥಾನ_Idx]);
                    thread_slots[ಸ್ಥಾನ_Idx] = ಖಾಲಿ;
                }
            }
        }

        ಆದರೆ (!keep_ಸ್ಥಾನ)
        {
            tlsSlots[ಸ್ಥಾನ_Idx].container = ಖಾಲಿ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ಶೂನ್ಯ* get_ದತ್ತಾಂಶ(size_t ಸ್ಥಾನ_Idx) ಸ್ಥಿರಾಂಕ
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_ಗಾತ್ರ > ಸ್ಥಾನ_Idx);
#ಆದರೆ_ಅಂತ್ಯ

        TlsAbstraction* tls = getTlsAbstraction();
        ಆದರೆ (ಖಾಲಿ == tls)
            ಹಿಂತಿರುಗಿ ಖಾಲಿ;  // TLS singleton is not available (terminated)

        Thread_ದತ್ತಾಂಶ* thread_ದತ್ತಾಂಶ = (Thread_ದತ್ತಾಂಶ*)tls->get_ದತ್ತಾಂಶ();
        ಆದರೆ(thread_ದತ್ತಾಂಶ && thread_ದತ್ತಾಂಶ->slots.size() > ಸ್ಥಾನ_Idx)
            ಹಿಂತಿರುಗಿ thread_ದತ್ತಾಂಶ->slots[ಸ್ಥಾನ_Idx];

        ಹಿಂತಿರುಗಿ ಖಾಲಿ;
    }

    // Gather data from threads by TLS storage index
    ಶೂನ್ಯ gather(size_t ಸ್ಥಾನ_Idx, std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ_Vec)
    {
        Auto_ಬೀಗ guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_ಗಾತ್ರ == tlsSlots.size());
        CV_Assert(tls_Slots_ಗಾತ್ರ > ಸ್ಥಾನ_Idx);

        ಗಾಗಿ(size_t i = 0; i < ಎಳೆಗಳು.size(); i++)
        {
            ಆದರೆ(ಎಳೆಗಳು[i])
            {
                std::vector<ಶೂನ್ಯ*>& thread_slots = ಎಳೆಗಳು[i]->slots;
                ಆದರೆ (thread_slots.size() > ಸ್ಥಾನ_Idx && thread_slots[ಸ್ಥಾನ_Idx])
                    ದತ್ತಾಂಶ_Vec.push_back(thread_slots[ಸ್ಥಾನ_Idx]);
            }
        }
    }

    // Set data to storage index
    ಶೂನ್ಯ ಗಣ_ದತ್ತಾಂಶ(size_t ಸ್ಥಾನ_Idx, ಶೂನ್ಯ* p_ದತ್ತಾಂಶ)
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_ಗಾತ್ರ > ಸ್ಥಾನ_Idx);
#ಆದರೆ_ಅಂತ್ಯ

        TlsAbstraction* tls = getTlsAbstraction();
        ಆದರೆ (ಖಾಲಿ == tls)
            ಹಿಂತಿರುಗಿ;  // TLS singleton is not available (terminated)

        Thread_ದತ್ತಾಂಶ* thread_ದತ್ತಾಂಶ = (Thread_ದತ್ತಾಂಶ*)tls->get_ದತ್ತಾಂಶ();
        ಆದರೆ(!thread_ದತ್ತಾಂಶ)
        {
            thread_ದತ್ತಾಂಶ = ಹೊಸ Thread_ದತ್ತಾಂಶ;
            tls->ಗಣ_ದತ್ತಾಂಶ((ಶೂನ್ಯ*)thread_ದತ್ತಾಂಶ);
            {
                Auto_ಬೀಗ guard(mtxGlobalAccess);

                ತಾರ್ಕಿಕ ಸಿಕ್ಕಿತು = ಸುಳ್ಳು;
                // Find unused slots
                ಗಾಗಿ(size_t ಸ್ಥಾನ = 0; ಸ್ಥಾನ < ಎಳೆಗಳು.size(); ಸ್ಥಾನ++)
                {
                    ಆದರೆ (ಎಳೆಗಳು[ಸ್ಥಾನ] == ಖಾಲಿ)
                    {
                        thread_ದತ್ತಾಂಶ->idx = (ಪೂರ್ಣಾಂಕ)ಸ್ಥಾನ;
                        ಎಳೆಗಳು[ಸ್ಥಾನ] = thread_ದತ್ತಾಂಶ;
                        ಸಿಕ್ಕಿತು = ನಿಜ;
                        ನಿಲ್ಲಿಸು;
                    }
                }

                ಆದರೆ (!ಸಿಕ್ಕಿತು)
                {
                    // Create new slot
                    thread_ದತ್ತಾಂಶ->idx = ಎಳೆಗಳು.size();
                    ಎಳೆಗಳು.push_back(thread_ದತ್ತಾಂಶ);
                }
            }
        }

        ಆದರೆ(ಸ್ಥಾನ_Idx >= thread_ದತ್ತಾಂಶ->slots.size())
        {
            Auto_ಬೀಗ guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_ದತ್ತಾಂಶ->slots.resize(ಸ್ಥಾನ_Idx + 1, ಖಾಲಿ);
        }
        thread_ದತ್ತಾಂಶ->slots[ಸ್ಥಾನ_Idx] = p_ದತ್ತಾಂಶ;
    }

ಖಾಸಗಿ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_ಗಾತ್ರ;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    ರಚನೆ Tls_ಸ್ಥಾನ_Info
    {
        Tls_ಸ್ಥಾನ_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<ರಚನೆ Tls_ಸ್ಥಾನ_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_ದತ್ತಾಂಶ*> ಎಳೆಗಳು; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
ಸ್ಥಿರ TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, ಹೊಸ TlsStorage())
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _WIN32  // pthread key destructor
ಸ್ಥಿರ ಶೂನ್ಯ opencv_tls_destructor(ಶೂನ್ಯ* p_ದತ್ತಾಂಶ)
{
    ಆದರೆ (!g_isTlsStorageInitialized)
        ಹಿಂತಿರುಗಿ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ದತ್ತಾಂಶ);
}
#ಇಲ್ಲದಿದ್ದರೆ // _WIN32
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_USE_FLS
ಸ್ಥಿರ ಶೂನ್ಯ WINAPI opencv_fls_destructor(ಶೂನ್ಯ* p_ದತ್ತಾಂಶ)
{
    // Empiric detection of ExitProcess call
    DWORD ಸಂಕೇತ_2 = STILL_ಸಕ್ರಿಯ/*259*/;
    BOOL res = Get_Exit_ಸಂಕೇತ_Process(GetCurrentProcess(), &ಸಂಕೇತ_2);
    ಆದರೆ (res && ಸಂಕೇತ_2 != STILL_ಸಕ್ರಿಯ)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = ನಿಜ;
    }

    ಆದರೆ (!g_isTlsStorageInitialized)
        ಹಿಂತಿರುಗಿ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_ದತ್ತಾಂಶ);
}
#ಆದರೆ_ಅಂತ್ಯ // CV_USE_FLS
#ಆದರೆ_ಅಂತ್ಯ // _WIN32

ಸ್ಥಿರ TlsStorage* ಸ್ಥಿರಾಂಕ g_force_initialization_ದ_Tls_Storage
#ಆದರೆ defined __GNUC__
    __attribute__((ಬಳಸದ))
#ಆದರೆ_ಅಂತ್ಯ
    = &getTlsStorage();


#ಇಲ್ಲದಿದ್ದರೆ  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
ವರ್ಗ TlsStorage
{
ಸಾರ್ವಜನಿಕ:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        ಗಾಗಿ (size_t ಸ್ಥಾನ_Idx = 0; ಸ್ಥಾನ_Idx < slots.size(); ಸ್ಥಾನ_Idx++)
        {
            ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ_Idx];
            TLSDataContainer* container = s.container;
            ಆದರೆ (container && s.ದತ್ತಾಂಶ)
            {
                container->delete_ದತ್ತಾಂಶ_Instance(s.ದತ್ತಾಂಶ);  // Can't use from SlotInfo destructor
                s.ದತ್ತಾಂಶ = ಶೂನ್ಯಸೂಚಕ;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_ಸ್ಥಾನ(TLSDataContainer* container)
    {
        size_t slots_ಗಾತ್ರ = slots.size();
        ಗಾಗಿ (size_t ಸ್ಥಾನ = 0; ಸ್ಥಾನ < slots_ಗಾತ್ರ; ಸ್ಥಾನ++)
        {
            ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ];
            ಆದರೆ (s.container == ಖಾಲಿ)
            {
                CV_Assert(!s.ದತ್ತಾಂಶ);
                s.container = container;
                ಹಿಂತಿರುಗಿ ಸ್ಥಾನ;
            }
        }

        // create new slot
        slots.push_back(ಸ್ಥಾನ_Info(container));
        ಹಿಂತಿರುಗಿ slots_ಗಾತ್ರ;
    }

    // Release TLS storage index and pass associated data to caller
    ಶೂನ್ಯ release_ಸ್ಥಾನ(size_t ಸ್ಥಾನ_Idx, std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ_Vec, ತಾರ್ಕಿಕ keep_ಸ್ಥಾನ = ಸುಳ್ಳು)
    {
        CV_Assert(ಸ್ಥಾನ_Idx < slots.size());
        ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ_Idx];
        ಶೂನ್ಯ* ದತ್ತಾಂಶ = s.ದತ್ತಾಂಶ;
        ಆದರೆ (ದತ್ತಾಂಶ)
        {
            ದತ್ತಾಂಶ_Vec.push_back(ದತ್ತಾಂಶ);
            s.ದತ್ತಾಂಶ = ಶೂನ್ಯಸೂಚಕ;
        }
        ಆದರೆ (!keep_ಸ್ಥಾನ)
        {
            s.container = ಖಾಲಿ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    ಶೂನ್ಯ* get_ದತ್ತಾಂಶ(size_t ಸ್ಥಾನ_Idx) ಸ್ಥಿರಾಂಕ
    {
        CV_Assert(ಸ್ಥಾನ_Idx < slots.size());
        ಸ್ಥಿರಾಂಕ ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ_Idx];
        ಹಿಂತಿರುಗಿ s.ದತ್ತಾಂಶ;
    }

    // Gather data from threads by TLS storage index
    ಶೂನ್ಯ gather(size_t ಸ್ಥಾನ_Idx, std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ_Vec)
    {
        CV_Assert(ಸ್ಥಾನ_Idx < slots.size());
        ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ_Idx];
        ಶೂನ್ಯ* ದತ್ತಾಂಶ = s.ದತ್ತಾಂಶ;
        ಆದರೆ (ದತ್ತಾಂಶ)
            ದತ್ತಾಂಶ_Vec.push_back(ದತ್ತಾಂಶ);
        ಹಿಂತಿರುಗಿ;
    }

    // Set data to storage index
    ಶೂನ್ಯ ಗಣ_ದತ್ತಾಂಶ(size_t ಸ್ಥಾನ_Idx, ಶೂನ್ಯ* p_ದತ್ತಾಂಶ)
    {
        CV_Assert(ಸ್ಥಾನ_Idx < slots.size());
        ಸ್ಥಾನ_Info& s = slots[ಸ್ಥಾನ_Idx];
        s.ದತ್ತಾಂಶ = p_ದತ್ತಾಂಶ;
    }

ಖಾಸಗಿ:
    ರಚನೆ ಸ್ಥಾನ_Info
    {
        ಸ್ಥಾನ_Info(TLSDataContainer* _container) : container(_container), ದತ್ತಾಂಶ(ಶೂನ್ಯಸೂಚಕ) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        ಶೂನ್ಯ* ದತ್ತಾಂಶ;
    };
    std::vector<ರಚನೆ ಸ್ಥಾನ_Info> slots;
};

ಸ್ಥಿರ TlsStorage& getTlsStorage()
{
    ಸ್ಥಿರ TlsStorage g_storage;  // no threading
    ಹಿಂತಿರುಗಿ g_storage;
}

#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
ಬಳಸಿ ನಾಮಾಕಾಶ details;

ಶೂನ್ಯ releaseTlsStorageThread()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_DISABLE_THREAD_SUPPORT
    ಆದರೆ (!g_isTlsStorageInitialized)
        ಹಿಂತಿರುಗಿ;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#ಆದರೆ_ಅಂತ್ಯ
}

TLSDataContainer::TLSDataContainer()
{
    ಕೀಲಿ_2 = (ಪೂರ್ಣಾಂಕ)getTlsStorage().reserve_ಸ್ಥಾನ(ಇದು); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(ಕೀಲಿ_2 == -1); // Key must be released in child object
}

ಶೂನ್ಯ TLSDataContainer::gather_ದತ್ತಾಂಶ(std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ) ಸ್ಥಿರಾಂಕ
{
    getTlsStorage().gather(ಕೀಲಿ_2, ದತ್ತಾಂಶ);
}

ಶೂನ್ಯ TLSDataContainer::detach_ದತ್ತಾಂಶ(std::vector<ಶೂನ್ಯ*> &ದತ್ತಾಂಶ)
{
    getTlsStorage().release_ಸ್ಥಾನ(ಕೀಲಿ_2, ದತ್ತಾಂಶ, ನಿಜ);
}

ಶೂನ್ಯ TLSDataContainer::release()
{
    ಆದರೆ (ಕೀಲಿ_2 == -1)
        ಹಿಂತಿರುಗಿ;  // already released
    std::vector<ಶೂನ್ಯ*> ದತ್ತಾಂಶ; ದತ್ತಾಂಶ.reserve(32);
    getTlsStorage().release_ಸ್ಥಾನ(ಕೀಲಿ_2, ದತ್ತಾಂಶ, ಸುಳ್ಳು); // Release key and get stored data for proper destruction
    ಕೀಲಿ_2 = -1;
    ಗಾಗಿ(size_t i = 0; i < ದತ್ತಾಂಶ.size(); i++)  // Delete all associated data
        delete_ದತ್ತಾಂಶ_Instance(ದತ್ತಾಂಶ[i]);
}

ಶೂನ್ಯ TLSDataContainer::cleanup()
{
    std::vector<ಶೂನ್ಯ*> ದತ್ತಾಂಶ; ದತ್ತಾಂಶ.reserve(32);
    getTlsStorage().release_ಸ್ಥಾನ(ಕೀಲಿ_2, ದತ್ತಾಂಶ, ನಿಜ); // Extract stored data with removal from TLS tables
    ಗಾಗಿ(size_t i = 0; i < ದತ್ತಾಂಶ.size(); i++)  // Delete all associated data
        delete_ದತ್ತಾಂಶ_Instance(ದತ್ತಾಂಶ[i]);
}

ಶೂನ್ಯ* TLSDataContainer::get_ದತ್ತಾಂಶ() ಸ್ಥಿರಾಂಕ
{
    CV_Assert(ಕೀಲಿ_2 != -1 && "Can't fetch data from terminated TLS container.");
    ಶೂನ್ಯ* p_ದತ್ತಾಂಶ = getTlsStorage().get_ದತ್ತಾಂಶ(ಕೀಲಿ_2); // Check if data was already allocated
    ಆದರೆ(!p_ದತ್ತಾಂಶ)
    {
        // Create new data instance and save it to TLS storage
        p_ದತ್ತಾಂಶ = create_ದತ್ತಾಂಶ_Instance();
        ಪ್ರಯತ್ನಿಸು
        {
            getTlsStorage().ಗಣ_ದತ್ತಾಂಶ(ಕೀಲಿ_2, p_ದತ್ತಾಂಶ);
        }
        ಹಿಡಿ (...)
        {
            delete_ದತ್ತಾಂಶ_Instance(p_ದತ್ತಾಂಶ);
            ಎಸೆ;
        }
    }
    ಹಿಂತಿರುಗಿ p_ದತ್ತಾಂಶ;
}

ಸ್ಥಿರ TLSData<CoreTLSData>& get_Core_Tls_ದತ್ತಾಂಶ_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, ಹೊಸ TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_ದತ್ತಾಂಶ()
{
    ಹಿಂತಿರುಗಿ get_Core_Tls_ದತ್ತಾಂಶ_TLS().getRef();
}

#ಆದರೆ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ WINRT
    #ಸೂಚನೆ warning(disable:4447) // Disable warning 'main' signature found without threading model
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

ಬಾಹ್ಯ "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ಆದರೆ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ಆದರೆ (lpReserved != ಖಾಲಿ) // called after ExitProcess() call
        {
            cv::__termination = ನಿಜ;
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    ಹಿಂತಿರುಗಿ TRUE;
}
#ಆದರೆ_ಅಂತ್ಯ


ನಾಮಾಕಾಶ {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
ತಾರ್ಕಿಕ overrideThreadName()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", ಸುಳ್ಳು);
    ಹಿಂತಿರುಗಿ param;
}
#ಆದರೆ_ಅಂತ್ಯ

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ g_thread_ಅಂಕೆ = 0;
ವರ್ಗ ThreadID {
ಸಾರ್ವಜನಿಕ:
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ id;
    ThreadID() :
        id(CV_XADD(&g_thread_ಅಂಕೆ, 1))
    {
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_WITH_ITT
        ಆದರೆ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#ಆದರೆ_ಅಂತ್ಯ
    }
};

ಸ್ಥಿರ TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, ಹೊಸ TLSData<ThreadID>());
}

} // namespace
ಪೂರ್ಣಾಂಕ utils::getThreadID() { ಹಿಂತಿರುಗಿ getThreadIDTLS().get()->id; }


ವರ್ಗ ParseError
{
    std::string bad_ಮೌಲ್ಯ;
ಸಾರ್ವಜನಿಕ:
    ParseError(ಸ್ಥಿರಾಂಕ std::string &bad_ಮೌಲ್ಯ_2) :bad_ಮೌಲ್ಯ(bad_ಮೌಲ್ಯ_2) {}
    std::string ಗಮ್ಯ_String(ಸ್ಥಿರಾಂಕ std::string &param) ಸ್ಥಿರಾಂಕ
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_ಮೌಲ್ಯ;
        ಹಿಂತಿರುಗಿ out.str();
    }
};

ಮಾದರಿ <ಬಗೆ_ಹೆಸರು T>
T parseOption(ಸ್ಥಿರಾಂಕ std::string &);

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ parseOption(ಸ್ಥಿರಾಂಕ std::string & value)
{
    ಆದರೆ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಆದರೆ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    }
    ಎಸೆ ParseError(value);
}

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ size_t parseOption(ಸ್ಥಿರಾಂಕ std::string &value)
{
    size_t pos = 0;
    ಗಾಗಿ (; pos < value.size(); pos++)
    {
        ಆದರೆ (!ಅಂಕಿಯೇ(value[pos]))
            ನಿಲ್ಲಿಸು;
    }
    cv::String ಮೌಲ್ಯ_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(ಮೌಲ್ಯ_Str);
    ಆದರೆ (suffixStr.length() == 0)
        ಹಿಂತಿರುಗಿ v;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        ಹಿಂತಿರುಗಿ v * 1024 * 1024;
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        ಹಿಂತಿರುಗಿ v * 1024;
    ಎಸೆ ParseError(value);
}

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ cv::String parseOption(ಸ್ಥಿರಾಂಕ std::string &value)
{
    ಹಿಂತಿರುಗಿ value;
}

ಮಾದರಿ<>
ಸಾಲೊಳಗೆ utils::Paths parseOption(ಸ್ಥಿರಾಂಕ std::string &value)
{
    utils::Paths ಫಲಿತಾಂಶ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _WIN32
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ sep = ';';
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ sep = ':';
#ಆದರೆ_ಅಂತ್ಯ
    size_t ಆರಂಭ_pos = 0;
    ಆಗ (ಆರಂಭ_pos != std::string::npos)
    {
        ಸ್ಥಿರಾಂಕ size_t pos = value.find(sep, ಆರಂಭ_pos);
        ಸ್ಥಿರಾಂಕ std::string one_piece(value, ಆರಂಭ_pos, pos == std::string::npos ? pos : pos - ಆರಂಭ_pos);
        ಆದರೆ (!one_piece.empty())
            ಫಲಿತಾಂಶ.push_back(one_piece);
        ಆರಂಭ_pos = pos == std::string::npos ? pos : pos + 1;
    }
    ಹಿಂತಿರುಗಿ ಫಲಿತಾಂಶ;
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * envRead(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * name)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_GETENV
    CV_ಬಳಸದ(name);
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ getenv(name);
#ಆದರೆ_ಅಂತ್ಯ
}

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T>
ಸಾಲೊಳಗೆ T ಪಠ(ಸ್ಥಿರಾಂಕ std::string & k, ಸ್ಥಿರಾಂಕ T & default_ಮೌಲ್ಯ_2)
{
    ಪ್ರಯತ್ನಿಸು
    {
        ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * res = envRead(k.c_str());
        ಆದರೆ (res)
            ಹಿಂತಿರುಗಿ parseOption<T>(std::string(res));
    }
    ಹಿಡಿ (ಸ್ಥಿರಾಂಕ ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.ಗಮ್ಯ_String(k));
    }
    ಹಿಂತಿರುಗಿ default_ಮೌಲ್ಯ_2;
}

ತಾರ್ಕಿಕ utils::getConfigurationParameterBool(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, ತಾರ್ಕಿಕ default_ಮೌಲ್ಯ_2)
{
    ಹಿಂತಿರುಗಿ ಪಠ<ತಾರ್ಕಿಕ>(name, default_ಮೌಲ್ಯ_2);
}

size_t utils::get_Configuration_Parameter_ಗಾತ್ರ_T(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, size_t default_ಮೌಲ್ಯ_2)
{
    ಹಿಂತಿರುಗಿ ಪಠ<size_t>(name, default_ಮೌಲ್ಯ_2);
}

std::string utils::getConfigurationParameterString(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, ಸ್ಥಿರಾಂಕ std::string & default_ಮೌಲ್ಯ_2)
{
    ಹಿಂತಿರುಗಿ ಪಠ<cv::String>(name, default_ಮೌಲ್ಯ_2);
}

utils::Paths utils::getConfigurationParameterPaths(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* name, ಸ್ಥಿರಾಂಕ utils::Paths &default_ಮೌಲ್ಯ_2)
{
    ಹಿಂತಿರುಗಿ ಪಠ<utils::Paths>(name, default_ಮೌಲ್ಯ_2);
}


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_COLLECT_IMPL_ದತ್ತಾಂಶ
ImplCollector& get_Impl_ದತ್ತಾಂಶ()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, ಹೊಸ ImplCollector())
}

ಶೂನ್ಯ ಗಣ_Impl(ಪೂರ್ಣಾಂಕ flags)
{
    cv::Auto_ಬೀಗ lock(get_Impl_ದತ್ತಾಂಶ().mutex);

    get_Impl_ದತ್ತಾಂಶ().implFlags = flags;
    get_Impl_ದತ್ತಾಂಶ().impl_ಸಂಕೇತ.clear();
    get_Impl_ದತ್ತಾಂಶ().implFun.clear();
}

ಶೂನ್ಯ ಕೂಡಿಸು_Impl(ಪೂರ್ಣಾಂಕ flag, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* func)
{
    cv::Auto_ಬೀಗ lock(get_Impl_ದತ್ತಾಂಶ().mutex);

    get_Impl_ದತ್ತಾಂಶ().implFlags |= flag;
    ಆದರೆ(func) // use lazy collection if name was not specified
    {
        size_t ಸೂಚ್ಯಂಕ = get_Impl_ದತ್ತಾಂಶ().impl_ಸಂಕೇತ.size();
        ಆದರೆ(!ಸೂಚ್ಯಂಕ || (get_Impl_ದತ್ತಾಂಶ().impl_ಸಂಕೇತ[ಸೂಚ್ಯಂಕ-1] != flag || get_Impl_ದತ್ತಾಂಶ().implFun[ಸೂಚ್ಯಂಕ-1].compare(func))) // avoid duplicates
        {
            get_Impl_ದತ್ತಾಂಶ().impl_ಸಂಕೇತ.push_back(flag);
            get_Impl_ದತ್ತಾಂಶ().implFun.push_back(func);
        }
    }
}

ಪೂರ್ಣಾಂಕ getImpl(std::vector<ಪೂರ್ಣಾಂಕ> &impl, std::vector<String> &funName)
{
    cv::Auto_ಬೀಗ lock(get_Impl_ದತ್ತಾಂಶ().mutex);

    impl    = get_Impl_ದತ್ತಾಂಶ().impl_ಸಂಕೇತ;
    funName = get_Impl_ದತ್ತಾಂಶ().implFun;
    ಹಿಂತಿರುಗಿ get_Impl_ದತ್ತಾಂಶ().implFlags; // return actual flags for lazy collection
}

ತಾರ್ಕಿಕ useCollection()
{
    ಹಿಂತಿರುಗಿ get_Impl_ದತ್ತಾಂಶ().useCollection;
}

ಶೂನ್ಯ ಗಣ_Use_Collection(ತಾರ್ಕಿಕ flag)
{
    cv::Auto_ಬೀಗ lock(get_Impl_ದತ್ತಾಂಶ().mutex);

    get_Impl_ದತ್ತಾಂಶ().useCollection = flag;
}
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ instr
{
ತಾರ್ಕಿಕ useInstrumentation()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    ಹಿಂತಿರುಗಿ getInstrumentStruct().useInstr;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಗಣ_Use_Instrumentation(ತಾರ್ಕಿಕ flag)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(flag);
#ಆದರೆ_ಅಂತ್ಯ
}

Instr_ಗಂಟು* getTrace()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    ಹಿಂತಿರುಗಿ &getInstrumentStruct().ಬೇರು_ಗಂಟು_2;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ resetTrace()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    getInstrumentStruct().ಬೇರು_ಗಂಟು_2.removeChilds();
    getInstrumentTLSStruct().p_Current_ಗಂಟು = &getInstrumentStruct().ಬೇರು_ಗಂಟು_2;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಗಣ_Flags(FLAGS ಬಹುಲಕ_Flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = ಬಹುಲಕ_Flags;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ಬಹುಲಕ_Flags);
#ಆದರೆ_ಅಂತ್ಯ
}
FLAGS getFlags()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
    ಹಿಂತಿರುಗಿ (FLAGS)getInstrumentStruct().flags;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ (FLAGS)0;
#ಆದರೆ_ಅಂತ್ಯ
}

ಗಂಟು_ದತ್ತಾಂಶ::ಗಂಟು_ದತ್ತಾಂಶ(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* funName, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fileName, ಪೂರ್ಣಾಂಕ ಸಾಲು_ಅಂಕೆ, ಶೂನ್ಯ* ret_ವಿಳಾಸ, ತಾರ್ಕಿಕ alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_ಸಾಲು_ಅಂಕೆ       = ಸಾಲು_ಅಂಕೆ;
    m_ret_ವಿಳಾಸ    = ret_ವಿಳಾಸ;
    m_alwaysExpand  = alwaysExpand;

    m_ಎಳೆಗಳು    = 1;
    m_counter    = 0;
    m_ticks_ಒಟ್ಟು = 0;

    m_funError  = ಸುಳ್ಳು;
}
ಗಂಟು_ದತ್ತಾಂಶ::ಗಂಟು_ದತ್ತಾಂಶ(ಗಂಟು_ದತ್ತಾಂಶ &ref)
{
    *ಇದು = ref;
}
ಗಂಟು_ದತ್ತಾಂಶ& ಗಂಟು_ದತ್ತಾಂಶ::ಕ್ರಿಯಾಕಾರಕ=(ಸ್ಥಿರಾಂಕ ಗಂಟು_ದತ್ತಾಂಶ &ಬಲ)
{
    ಇದು->m_funName      = ಬಲ.m_funName;
    ಇದು->m_instrType    = ಬಲ.m_instrType;
    ಇದು->m_implType     = ಬಲ.m_implType;
    ಇದು->m_fileName     = ಬಲ.m_fileName;
    ಇದು->m_ಸಾಲು_ಅಂಕೆ      = ಬಲ.m_ಸಾಲು_ಅಂಕೆ;
    ಇದು->m_ret_ವಿಳಾಸ   = ಬಲ.m_ret_ವಿಳಾಸ;
    ಇದು->m_alwaysExpand = ಬಲ.m_alwaysExpand;

    ಇದು->m_ಎಳೆಗಳು     = ಬಲ.m_ಎಳೆಗಳು;
    ಇದು->m_counter     = ಬಲ.m_counter;
    ಇದು->m_ticks_ಒಟ್ಟು  = ಬಲ.m_ticks_ಒಟ್ಟು;

    ಇದು->m_funError    = ಬಲ.m_funError;

    ಹಿಂತಿರುಗಿ *ಇದು;
}
ಗಂಟು_ದತ್ತಾಂಶ::~ಗಂಟು_ದತ್ತಾಂಶ()
{
}
ತಾರ್ಕಿಕ ಕ್ರಿಯಾಕಾರಕ==(ಸ್ಥಿರಾಂಕ ಗಂಟು_ದತ್ತಾಂಶ& ಎಡ, ಸ್ಥಿರಾಂಕ ಗಂಟು_ದತ್ತಾಂಶ& ಬಲ)
{
    ಆದರೆ(ಎಡ.m_ಸಾಲು_ಅಂಕೆ == ಬಲ.m_ಸಾಲು_ಅಂಕೆ && ಎಡ.m_funName == ಬಲ.m_funName && ಎಡ.m_fileName == ಬಲ.m_fileName)
    {
        ಆದರೆ(ಎಡ.m_ret_ವಿಳಾಸ == ಬಲ.m_ret_ವಿಳಾಸ || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_ಹೆಸರುಗಳು || ಎಡ.m_alwaysExpand))
            ಹಿಂತಿರುಗಿ ನಿಜ;
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    ಸ್ಥಿರ InstrStruct instr;
    ಹಿಂತಿರುಗಿ instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    ಹಿಂತಿರುಗಿ *getInstrumentStruct().tlsStruct.get();
}

Instr_ಗಂಟು* get_Current_ಗಂಟು()
{
    ಹಿಂತಿರುಗಿ getInstrumentTLSStruct().p_Current_ಗಂಟು;
}

IntrumentationRegion::IntrumentationRegion(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* funName, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fileName, ಪೂರ್ಣಾಂಕ ಸಾಲು_ಅಂಕೆ, ಶೂನ್ಯ *ret_ವಿಳಾಸ, ತಾರ್ಕಿಕ alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = ಸುಳ್ಳು;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ಆದರೆ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ಆದರೆ(!pTLS->p_Current_ಗಂಟು)
        {
            m_disabled = ನಿಜ;
            ಹಿಂತಿರುಗಿ;
        }

        ಪೂರ್ಣಾಂಕ ಆಳ = pTLS->p_Current_ಗಂಟು->get_ಆಳ();
        ಆದರೆ(pStruct->max_ಆಳ && pStruct->max_ಆಳ <= ಆಳ)
        {
            m_disabled = ನಿಜ;
            ಹಿಂತಿರುಗಿ;
        }

        ಗಂಟು_ದತ್ತಾಂಶ payload(funName, fileName, ಸಾಲು_ಅಂಕೆ, ret_ವಿಳಾಸ, alwaysExpand, instrType, implType);
        Node<ಗಂಟು_ದತ್ತಾಂಶ>* p_ಮಗು = ಖಾಲಿ;

        ಆದರೆ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_ಬೀಗ guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_ಮಗು = pTLS->p_Current_ಗಂಟು->ಕಂಡುಹಿಡಿ_ಮಗು(payload);
            ಆದರೆ(!p_ಮಗು)
            {
                p_ಮಗು = ಹೊಸ Node<ಗಂಟು_ದತ್ತಾಂಶ>(payload);
                pTLS->p_Current_ಗಂಟು->ಕೂಡಿಸು_ಮಗು(p_ಮಗು);
            }
        }
        ಇಲ್ಲದಿದ್ದರೆ
        {
            p_ಮಗು = pTLS->p_Current_ಗಂಟು->ಕಂಡುಹಿಡಿ_ಮಗು(payload);
            ಆದರೆ(!p_ಮಗು)
            {
                m_disabled = ನಿಜ;
                ಹಿಂತಿರುಗಿ;
            }
        }
        pTLS->p_Current_ಗಂಟು = p_ಮಗು;

        m_regionTicks = get_Tick_ಎಣಿಕೆ();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ಆದರೆ(pStruct->useInstr)
    {
        ಆದರೆ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ಆದರೆ (pTLS->p_Current_ಗಂಟು->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_ಗಂಟು->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_ಗಂಟು->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::ಮುಕ್ತಾಯ_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_ಎಣಿಕೆ() - m_regionTicks);
            {
                cv::Auto_ಬೀಗ guard(pStruct->mutex_ಎಣಿಕೆ); // Concurrent ticks accumulation
                pTLS->p_Current_ಗಂಟು->m_payload.m_counter++;
                pTLS->p_Current_ಗಂಟು->m_payload.m_ticks_ಒಟ್ಟು += ticks;
                pTLS->p_Current_ಗಂಟು->m_payload.m_tls.get()->m_ticks_ಒಟ್ಟು += ticks;
            }

            pTLS->p_Current_ಗಂಟು = pTLS->p_Current_ಗಂಟು->m_p_ಪೋಷಕ;
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ
}

ನಾಮಾಕಾಶ ipp
{

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ರಚನೆ IPPInitSingleton
{
ಸಾರ್ವಜನಿಕ:
    IPPInitSingleton()
    {
        useIPP         = ನಿಜ;

#ಆದರೆ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#ಇಲ್ಲದಿದ್ದರೆ
        useIPP_NE      = ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
        ipp_ಸ್ಥಿತಿ      = 0;
        funcname       = ಖಾಲಿ;
        filename       = ಖಾಲಿ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_ತುದಿ_Features = 0;
        pIppLibInfo    = ಖಾಲಿ;

        ipp_ಸ್ಥಿತಿ = ippGetCpuFeatures(&cpuFeatures, ಖಾಲಿ);
        ಆದರೆ(ipp_ಸ್ಥಿತಿ < 0)
        {
            CV_LOG_ERROR(ಖಾಲಿ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ಆದರೆ(!env.empty())
        {
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201900
            ಸ್ಥಿರಾಂಕ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#ಅಥವಾಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201703
            ಸ್ಥಿರಾಂಕ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ಅಥವಾಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201700
            ಸ್ಥಿರಾಂಕ Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#ಇಲ್ಲದಿದ್ದರೆ
            ಸ್ಥಿರಾಂಕ Ipp64u minorFeatures = 0;
#ಆದರೆ_ಅಂತ್ಯ

            env = ಗಮ್ಯ_ಅಧೋ_Case(env);
            ಆದರೆ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = ನಿಜ;
                env = env.substr(3, env.size());
            }

            ಆದರೆ(env == "disabled")
            {
                CV_LOG_WARNING(ಖಾಲಿ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = ಸುಳ್ಳು;
            }
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201700
#ಆದರೆ defined (_M_AMD64) || defined (__x86_64__)
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
            ಇಲ್ಲದಿದ್ದರೆ
                CV_LOG_ERROR(ಖಾಲಿ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ಆದರೆ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ಆದರೆ(!(
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#ಆದರೆ_ಅಂತ್ಯ
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = ಸುಳ್ಳು;
            ಹಿಂತಿರುಗಿ;
        }

        ಆದರೆ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        ಇಲ್ಲದಿದ್ದರೆ
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ಆದರೆ IPP_ಆವೃತ್ತಿ_X100 >= 201700
        ಆದರೆ(ippFeatures&ippCPUID_AVX512F)
        {
            ಆದರೆ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_ತುದಿ_Features = ippCPUID_AVX512_SKX;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_ತುದಿ_Features = ippCPUID_AVX512_KNL;
            ಇಲ್ಲದಿದ್ದರೆ
                ipp_ತುದಿ_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
        ಆದರೆ(ippFeatures&ippCPUID_AVX2)
            ipp_ತುದಿ_Features = ippCPUID_AVX2;
        ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ippFeatures&ippCPUID_SSE42)
            ipp_ತುದಿ_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_ಆವೃತ್ತಿ();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ಆದರೆ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_ತುದಿ_Features = ippCPUID_SSE42;
        }
    }

ಸಾರ್ವಜನಿಕ:
    ತಾರ್ಕಿಕ        useIPP;
    ತಾರ್ಕಿಕ        useIPP_NE;

    ಪೂರ್ಣಾಂಕ         ipp_ಸ್ಥಿತಿ;  // 0 - all is ok, -1 - IPP functions failed
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *funcname;
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *filename;
    ಪೂರ್ಣಾಂಕ         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_ತುದಿ_Features;
    ಸ್ಥಿರಾಂಕ Ipp_Library_ಆವೃತ್ತಿ *pIppLibInfo;
};

ಸ್ಥಿರ IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, ಹೊಸ IPPInitSingleton())
}
#ಆದರೆ_ಅಂತ್ಯ

ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ getIppFeatures()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ಹಿಂತಿರುಗಿ getIPPSingleton().ippFeatures;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ get_Ipp_ತುದಿ_Features()
{
    ಹಿಂತಿರುಗಿ getIPPSingleton().ipp_ತುದಿ_Features;
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ಗಣ_Ipp_ಸ್ಥಿತಿ(ಪೂರ್ಣಾಂಕ ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ _funcname, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ * ಸ್ಥಿರಾಂಕ _filename, ಪೂರ್ಣಾಂಕ ಸಾಲು_3)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    getIPPSingleton().ipp_ಸ್ಥಿತಿ = ಸ್ಥಿತಿ_2;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = ಸಾಲು_3;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ಸ್ಥಿತಿ_2); CV_ಬಳಸದ(_funcname); CV_ಬಳಸದ(_filename); CV_ಬಳಸದ(ಸಾಲು_3);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ get_Ipp_ಸ್ಥಿತಿ()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ಹಿಂತಿರುಗಿ getIPPSingleton().ipp_ಸ್ಥಿತಿ;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

String getIppErrorLocation()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ಹಿಂತಿರುಗಿ format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ String();
#ಆದರೆ_ಅಂತ್ಯ
}

String get_Ipp_ಆವೃತ್ತಿ()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ಸ್ಥಿರಾಂಕ Ipp_Library_ಆವೃತ್ತಿ *pInfo = getIPPSingleton().pIppLibInfo;
    ಆದರೆ(pInfo)
        ಹಿಂತಿರುಗಿ format("%s %s %s", pInfo->Name, pInfo->ಆವೃತ್ತಿ_4, pInfo->BuildDate);
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ String("error");
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ String("disabled");
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ useIPP()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    ಆದರೆ (ದತ್ತಾಂಶ.useIPP < 0)
    {
        ದತ್ತಾಂಶ.useIPP = getIPPSingleton().useIPP;
    }
    ಹಿಂತಿರುಗಿ (ದತ್ತಾಂಶ.useIPP > 0);
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಗಣ_Use_IPP(ತಾರ್ಕಿಕ flag)
{
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ದತ್ತಾಂಶ.useIPP = (getIPPSingleton().useIPP)?flag:ಸುಳ್ಳು;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(flag);
    ದತ್ತಾಂಶ.useIPP = ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ useIPP_NotExact()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
    ಆದರೆ (ದತ್ತಾಂಶ.useIPP_NE < 0)
    {
        ದತ್ತಾಂಶ.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    ಹಿಂತಿರುಗಿ (ದತ್ತಾಂಶ.useIPP_NE > 0);
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ ಗಣ_Use_IPP_Not_Exact(ತಾರ್ಕಿಕ flag)
{
    CoreTLSData& ದತ್ತಾಂಶ = get_Core_Tls_ದತ್ತಾಂಶ();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_IPP
    ದತ್ತಾಂಶ.useIPP_NE = flag;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(flag);
    ದತ್ತಾಂಶ.useIPP_NE = ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

} // namespace ipp


ನಾಮಾಕಾಶ details {

#ಆದರೆ OPENCV_IMPL_FP_HINTS_X86
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#ವ್ಯಾಖ್ಯಾನಿಸು _MM_DENORMALS_ZERO_ON 0x0040
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MM_DENORMALS_ZERO_ಮುಖವಾಡ  // requires pmmintrin.h (SSE3)
#ವ್ಯಾಖ್ಯಾನಿಸು MM_DENORMALS_ZERO_ಮುಖವಾಡ 0x0040
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ ಗಣ_FPDenormals_Ignore_Hint(ತಾರ್ಕಿಕ ignore, CV_ಹೊರಹೋಗು FPDenormals_ಬಹುಲಕ_State& state)
{
#ಆದರೆ OPENCV_IMPL_FP_HINTS_X86
    ಚಿಹ್ನೆರಹಿತ ಮುಖವಾಡ = MM_FLUSH_ZERO_ಮುಖವಾಡ;
    ಚಿಹ್ನೆರಹಿತ value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ಆದರೆ (featuresEnabled.have[CPU_SSE3])
    {
        ಮುಖವಾಡ |= MM_DENORMALS_ZERO_ಮುಖವಾಡ;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ old_flags = _mm_getcsr();
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ old_ಮೌಲ್ಯ = old_flags & ಮುಖವಾಡ;
    ಚಿಹ್ನೆರಹಿತ flags = (old_flags & ~ಮುಖವಾಡ) | value;
    CV_LOG_DEBUG(ಖಾಲಿ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)ಮುಖವಾಡ;
    state.reserved[1] = (uint32_t)old_ಮೌಲ್ಯ;
    _mm_setcsr(flags);
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(ignore); CV_ಬಳಸದ(state);
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ saveFPDenormalsState(CV_ಹೊರಹೋಗು FPDenormals_ಬಹುಲಕ_State& state)
{
#ಆದರೆ OPENCV_IMPL_FP_HINTS_X86
    ಚಿಹ್ನೆರಹಿತ ಮುಖವಾಡ = MM_FLUSH_ZERO_ಮುಖವಾಡ;
    ಆದರೆ (featuresEnabled.have[CPU_SSE3])
    {
        ಮುಖವಾಡ |= MM_DENORMALS_ZERO_ಮುಖವಾಡ;
    }
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ old_flags = _mm_getcsr();
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ old_ಮೌಲ್ಯ = old_flags & ಮುಖವಾಡ;
    // save state
    state.reserved[0] = (uint32_t)ಮುಖವಾಡ;
    state.reserved[1] = (uint32_t)old_ಮೌಲ್ಯ;
    ಹಿಂತಿರುಗಿ 2;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(state);
    ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}

ತಾರ್ಕಿಕ restoreFPDenormalsState(ಸ್ಥಿರಾಂಕ FPDenormals_ಬಹುಲಕ_State& state)
{
#ಆದರೆ OPENCV_IMPL_FP_HINTS_X86
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಮುಖವಾಡ = (ಚಿಹ್ನೆರಹಿತ)state.reserved[0];
    CV_DbgAssert(ಮುಖವಾಡ != 0); // invalid state (ensure that state is properly saved earlier)
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ value = (ಚಿಹ್ನೆರಹಿತ)state.reserved[1];
    CV_DbgCheck((ಪೂರ್ಣಾಂಕ)value, value == (value & ಮುಖವಾಡ), "invalid SSE FP state");
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ old_flags = _mm_getcsr();
    ಚಿಹ್ನೆರಹಿತ flags = (old_flags & ~ಮುಖವಾಡ) | value;
    CV_LOG_DEBUG(ಖಾಲಿ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    ಹಿಂತಿರುಗಿ ನಿಜ;
#ಇಲ್ಲದಿದ್ದರೆ
    CV_ಬಳಸದ(state);
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
#ಆದರೆ_ಅಂತ್ಯ
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_ALGO_HINT_DEFAULT
    ಹಿಂತಿರುಗಿ OPENCV_ALGO_HINT_DEFAULT;
#ಇಲ್ಲದಿದ್ದರೆ
    ಹಿಂತಿರುಗಿ ALGO_HINT_ACCURATE;
#ಆದರೆ_ಅಂತ್ಯ
};

} // namespace cv

/* End of file. */
