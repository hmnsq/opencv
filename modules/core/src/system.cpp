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

#అన్తర్భావ "precomp.hpp"
//#include "opencv2/core/core_c.h"
#అన్తర్భావ <atomic>
#అన్తర్భావ <exception>
#అన్తర్భావ <iostream>
#అన్తర్భావ <ostream>

#నిర్వచితమైతే __QNX__
    #అన్తర్భావ <unistd.h>
    #అన్తర్భావ <sys/neutrino.h>
    #అన్తర్భావ <sys/syspage.h>
#నిర్వచితమైతే __aarch64__
    #అన్తర్భావ <aarch64/syspage.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>
#అన్తర్భావ <opencv2/core/utils/trace.private.hpp>

#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#అన్తర్భావ <opencv2/core/utils/tls.hpp>
#అన్తర్భావ <opencv2/core/utils/instrumentation.hpp>

#అన్తర్భావ <opencv2/core/utils/filesystem.private.hpp>

#అన్తర్భావ <opencv2/core/utils/fp_control_utils.hpp>
#అన్తర్భావ <opencv2/core/utils/fp_control.private.hpp>

నామాకాశ cv {

స్థిర శూన్య _initSystem()
{
#నిర్వచితమైతే __ANDROID__
    // https://github.com/opencv/opencv/issues/14906
    // "ios_base::Init" object is not a part of Android's "iostream" header (in case of clang toolchain, NDK 20).
    // Ref1: https://en.cppreference.com/w/cpp/io/ios_base/Init
    //       The header <iostream> behaves as if it defines (directly or indirectly) an instance of std::ios_base::Init with static storage duration
    // Ref2: https://github.com/gcc-mirror/gcc/blob/gcc-8-branch/libstdc%2B%2B-v3/include/std/iostream#L73-L74
    స్థిర std::ios_base::Init s_iostream_initializer;
#అయితే_ముగింపు
}

స్థిర Mutex* __initialization_mutex = ఖాళీ;
Mutex& getInitializationMutex()
{
    ఒకవేళ (__initialization_mutex == ఖాళీ)
    {
        (శూన్య)_initSystem();
        __initialization_mutex = కొత్త Mutex();
    }
    తిరిగి *__initialization_mutex;
}
// force initialization (single-threaded environment)
Mutex* __initialization_mutex_initializer = &getInitializationMutex();

స్థిర తార్కిక param_dumpErrors = utils::getConfigurationParameterBool("OPENCV_DUMP_ERRORS",
#ఒకవేళ defined(_DEBUG) || defined(__ANDROID__)
    నిజం
#లేకపోతే
    అబద్ధం
#అయితే_ముగింపు
);

శూన్య* alloc_Singleton_బఫరు(size_t size) { తిరిగి fastMalloc(size); }
శూన్య* alloc_Singleton_New_బఫరు(size_t size) { తిరిగి జ్ఞాపకం_ఇవ్వు(size); }


} // namespace cv

#నిర్వచితం_కాకపోతే CV_ERROR_సమితి_TERMINATE_HANDLER  // build config option
# ఒకవేళ defined(_WIN32)
#   define CV_ERROR_సమితి_TERMINATE_HANDLER 1
# endif
#అయితే_ముగింపు
#ఒకవేళ defined(CV_ERROR_సమితి_TERMINATE_HANDLER) && !CV_ERROR_సమితి_TERMINATE_HANDLER
# undef CV_ERROR_సమితి_TERMINATE_HANDLER
#అయితే_ముగింపు

#నిర్వచితమైతే _MSC_VER
# ఒకవేళ _MSC_VER >= 1700
#  pragma warning(disable:4447) // Disable warning 'main' signature found without threading model
# endif
#అయితే_ముగింపు

#నిర్వచితమైతే CV_ERROR_సమితి_TERMINATE_HANDLER
#అన్తర్భావ <cstdlib>        // std::abort
#అయితే_ముగింపు

#ఒకవేళ defined __ANDROID__ || defined __unix__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __HAIKU__ || defined __Fuchsia__ || defined __QNX__
#  include <unistd.h>
#  include <fcntl.h>
#ఒకవేళ defined __QNX__
#  include <sys/elf.h>
#  include <sys/auxv.h>
ఉపయోగించి Elf64_auxv_t = auxv64_t;
#  include <elfdefinitions.h>
స్థిరరాశి uint64_t AT_HWCAP = NT_GNU_HWCAP;
#లేకపోతే
#  include <elf.h>
#అయితే_ముగింపు
#ఒకవేళ defined __ANDROID__ || defined __linux__
#  include <linux/auxvec.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined __ANDROID__ && defined HAVE_CPUFEATURES
#  include <cpu-features.h>
#అయితే_ముగింపు


#ఒకవేళ ((defined __ppc64__ || defined __PPC64__) && (defined HAVE_GETAUXVAL || defined HAVE_ELF_AUX_INFO))
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
#అయితే_ముగింపు

#ఒకవేళ defined __loongarch64
#అన్తర్భావ "sys/auxv.h"
#నిర్వచించు LA_HWCAP_LSX   (1<<4)
#నిర్వచించు LA_HWCAP_LASX  (1<<5)
#అయితే_ముగింపు

#ఒకవేళ defined _WIN32 || defined WINCE
#నిర్వచితం_కాకపోతే _WIN32_WINNT           // This is needed for the declaration of TryEnterCriticalSection in winbase.h with Visual Studio 2005 (and older?)
  #నిర్వచించు _WIN32_WINNT 0x0400  // http://msdn.microsoft.com/en-us/library/ms686857(VS.85).aspx
#అయితే_ముగింపు
#అన్తర్భావ <windows.h>
#అన్తర్భావ <combaseapi.h>
#ఒకవేళ (_WIN32_WINNT >= 0x0602)
  #అన్తర్భావ <synchapi.h>
#అయితే_ముగింపు
#ఒకవేళ ((_WIN32_WINNT >= 0x0600) && !defined(CV_DISABLE_FLS)) || defined(CV_FORCE_FLS)
  #అన్తర్భావ <fibersapi.h>
  #నిర్వచించు CV_USE_FLS
#అయితే_ముగింపు
#నిర్వచనం_రద్దు small
#నిర్వచనం_రద్దు min
#నిర్వచనం_రద్దు max
#నిర్వచనం_రద్దు నిరపేక్ష
#అన్తర్భావ <tchar.h>

#నిర్వచితమైతే WINRT
#అన్తర్భావ <wrl/client.h>
#నిర్వచితం_కాకపోతే __cplusplus_winrt
#అన్తర్భావ <windows.storage.h>
#సూచన comment(lib, "runtimeobject.lib")
#అయితే_ముగింపు // WINRT

std::wstring Get_తాత్కాలిక_మార్గం_Win_RT()
{
#నిర్వచితమైతే __cplusplus_winrt
    తిరిగి std::wstring(Windows::Storage::Application_దత్తాంశం::Current->TemporaryFolder->మార్గం_2->దత్తాంశం_4());
#లేకపోతే
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_దత్తాంశం_Statics> appdataFactory;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IApplication_దత్తాంశం> appdataRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorageFolder> storagefolderRef;
    Microsoft::WRL::ComPtr<ABI::Windows::Storage::IStorage_అంశం> storageitemRef;
    HSTRING str;
    HSTRING_శీర్షిక hstr_తల;
    std::wstring wstr;
    ఒకవేళ (FAILED(WindowsCreateStringReference(Runtime_Class_Windows_Storage_Application_దత్తాంశం,
                                            (UINT32)wcslen(Runtime_Class_Windows_Storage_Application_దత్తాంశం), &hstr_తల, &str)))
        తిరిగి wstr;
    ఒకవేళ (FAILED(RoGetActivationFactory(str, IID_PPV_వాదనలు(appdataFactory.Release_And_Get_చిరునామా_యొక్క()))))
        తిరిగి wstr;
    ఒకవేళ (FAILED(appdataFactory->get_Current(appdataRef.Release_And_Get_చిరునామా_యొక్క())))
        తిరిగి wstr;
    ఒకవేళ (FAILED(appdataRef->get_TemporaryFolder(storagefolderRef.Release_And_Get_చిరునామా_యొక్క())))
        తిరిగి wstr;
    ఒకవేళ (FAILED(storagefolderRef.As(&storageitemRef)))
        తిరిగి wstr;
    str = ఖాళీ;
    ఒకవేళ (FAILED(storageitemRef->get_మార్గం(&str)))
        తిరిగి wstr;
    wstr = Windows_Get_String_Raw_బఫరు(str, ఖాళీ);
    WindowsDeleteString(str);
    తిరిగి wstr;
#అయితే_ముగింపు
}

std::wstring Get_తాత్కాలిక_File_Name_Win_RT(std::wstring ఉపసర్గ)
{
    wchar_t guidStr[40];
    GUID g;
    CoCreateGuid(&g);
    wchar_t* ముసుగు = L"%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    swprintf(&guidStr[0], పరిమాణం(guidStr)/పరిమాణం(wchar_t), ముసుగు,
             g.Data1, g.Data2, g.Data3, UINT(g.Data4[0]), UINT(g.Data4[1]),
             UINT(g.Data4[2]), UINT(g.Data4[3]), UINT(g.Data4[4]),
             UINT(g.Data4[5]), UINT(g.Data4[6]), UINT(g.Data4[7]));

    తిరిగి ఉపసర్గ.append(std::wstring(guidStr));
}

#అయితే_ముగింపు
#లేకపోతే
#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT
#అన్తర్భావ <pthread.h>
#అయితే_ముగింపు
#అన్తర్భావ <sys/time.h>
#అన్తర్భావ <time.h>

#ఒకవేళ defined __MACH__ && defined __APPLE__
#అన్తర్భావ <mach/mach.h>
#అన్తర్భావ <mach/mach_time.h>
#అన్తర్భావ <sys/sysctl.h>
#అయితే_ముగింపు

#అయితే_ముగింపు

#నిర్వచితమైతే _OPENMP
#అన్తర్భావ "omp.h"
#అయితే_ముగింపు

#ఒకవేళ defined __unix__ || defined __APPLE__ || defined __EMSCRIPTEN__ || defined __FreeBSD__ || defined __OpenBSD__ || defined __GLIBC__ || defined __HAIKU__
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ defined __ANDROID__
#అన్తర్భావ <sys/sysconf.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే __ANDROID__
# include <android/log.h>
#అయితే_ముగింపు

#నిర్వచితమైతే DECLARE_CV_CPUID_X86
DECLARE_CV_CPUID_X86
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే CV_CPUID_X86
  #ఒకవేళ defined _MSC_VER && (defined _M_IX86 || defined _M_X64)
    #ఒకవేళ _MSC_VER >= 1400  // MSVS 2005
      #అన్తర్భావ <intrin.h>  // __cpuidex()
      #నిర్వచించు CV_CPUID_X86 __cpuidex
    #లేకపోతే
      #దోషం "Required MSVS 2005+"
    #అయితే_ముగింపు
  #లేదావేళ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
    స్థిర శూన్య cv_cpuid(పూర్ణాంకం* cpuid_దత్తాంశం, పూర్ణాంకం reg_eax, పూర్ణాంకం reg_ecx)
    {
        పూర్ణాంకం __eax = reg_eax, __ebx = 0, __ecx = reg_ecx, __edx = 0;
// tested with available compilers (-fPIC -O2 -m32/-m64): https://godbolt.org/
#ఒకవేళ !defined(__PIC__) \
    || defined(__x86_64__) || __GNUC__ >= 5 \
    || defined(__clang__) || defined(__INTEL_COMPILER)
        __asm__("cpuid\n\t"
                : "+a" (__eax), "=b" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#లేదావేళ defined(__i386__)  // ebx may be reserved as the PIC register
        __asm__("xchg{l}\t{%%}ebx, %1\n\t"
                "cpuid\n\t"
                "xchg{l}\t{%%}ebx, %1\n\t"
                : "+a" (__eax), "=&r" (__ebx), "+c" (__ecx), "=d" (__edx)
        );
#లేకపోతే
#దోషం "Configuration error"
#అయితే_ముగింపు
        cpuid_దత్తాంశం[0] = __eax; cpuid_దత్తాంశం[1] = __ebx; cpuid_దత్తాంశం[2] = __ecx; cpuid_దత్తాంశం[3] = __edx;
    }
    #నిర్వచించు CV_CPUID_X86 cv_cpuid
  #అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ <chrono>

నామాకాశ cv
{

Exception::Exception() { సంకేతం_2 = Error::StsOk; పంక్తి = 0; }

Exception::Exception(Error::సంకేతం_4 సంకేతం_5, స్థిరరాశి String& _err, స్థిరరాశి String& _func, స్థిరరాశి String& _file, పూర్ణాంకం పంక్తి_3)
: సంకేతం_2(సంకేతం_5), err(_err), func(_func), file(_file), పంక్తి(పంక్తి_3)
{
    format_సందేశం();
}

Exception::~Exception() CV_NOEXCEPT {}

/*!
 \return the error description and the context as a text string.
 */
స్థిరరాశి అక్షరం* Exception::what() స్థిరరాశి CV_NOEXCEPT { తిరిగి సందేశం.c_str(); }

శూన్య Exception::format_సందేశం()
{
    size_t pos = err.find('\n');
    తార్కిక multiline = pos != cv::String::npos;
    ఒకవేళ (multiline)
    {
        std::stringstream ss;
        size_t prev_pos = 0;
        అయితే (pos != cv::String::npos)
        {
           ss << "> " << err.substr(prev_pos, pos - prev_pos) << std::పంక్తి_ముగింపు;
           prev_pos = pos + 1;
           pos = err.find('\n', prev_pos);
        }
        ss << "> " << err.substr(prev_pos);
        ఒకవేళ (err[err.size() - 1] != '\n')
            ss << std::పంక్తి_ముగింపు;
        err = ss.str();
    }
    ఒకవేళ (func.size() > 0)
    {
        ఒకవేళ (multiline)
            సందేశం = format("OpenCV(%s) %s:%d: error: (%d:%s) in function '%s'\n%s", CV_సంస్కరణ, file.c_str(), పంక్తి, సంకేతం_2, సంకేతం_సందేశం(), func.c_str(), err.c_str());
        లేకపోతే
            సందేశం = format("OpenCV(%s) %s:%d: error: (%d:%s) %s in function '%s'\n", CV_సంస్కరణ, file.c_str(), పంక్తి, సంకేతం_2, సంకేతం_సందేశం(), err.c_str(), func.c_str());
    }
    లేకపోతే
    {
        సందేశం = format("OpenCV(%s) %s:%d: error: (%d:%s) %s%s", CV_సంస్కరణ, file.c_str(), పంక్తి, సంకేతం_2, సంకేతం_సందేశం(), err.c_str(), multiline ? "" : "\n");
    }
}

స్థిరరాశి అక్షరం * Exception::సంకేతం_సందేశం() స్థిరరాశి
{
    ఎంపిక (సంకేతం_2)
    {
    స్థితి cv::Error::StsOk :                  తిరిగి "No Error";
    స్థితి cv::Error::Sts_వెనుక_Trace :           తిరిగి "Backtrace";
    స్థితి cv::Error::StsError :               తిరిగి "Unspecified error";
    స్థితి cv::Error::Sts_అంతర్గత :            తిరిగి "Internal error";
    స్థితి cv::Error::StsNoMem :               తిరిగి "Insufficient memory";
    స్థితి cv::Error::StsBadArg :              తిరిగి "Bad argument";
    స్థితి cv::Error::StsNoConv :              తిరిగి "Iterations do not converge";
    స్థితి cv::Error::StsAutoTrace :           తిరిగి "Autotrace call";
    స్థితి cv::Error::Sts_Bad_కొలత :             తిరిగి "Incorrect size of input array";
    స్థితి cv::Error::StsNullPtr :             తిరిగి "Null pointer";
    స్థితి cv::Error::StsDivByZero :           తిరిగి "Division by zero occurred";
    స్థితి cv::Error::Bad_అడుగు :                తిరిగి "Image step is wrong";
    స్థితి cv::Error::StsInplaceNotSupported : తిరిగి "Inplace operation is not supported";
    స్థితి cv::Error::Sts_Object_Not_దొరికింది :      తిరిగి "Requested object was not found";
    స్థితి cv::Error::Bad_లోతు :               తిరిగి "Input image depth is not supported by function";
    స్థితి cv::Error::StsUnmatchedFormats :    తిరిగి "Formats of input arguments do not match";
    స్థితి cv::Error::StsUnmatchedSizes :      తిరిగి "Sizes of input arguments do not match";
    స్థితి cv::Error::Sts_బహిర్గతం_యొక్క_Range :          తిరిగి "One of the arguments\' values is out of range";
    స్థితి cv::Error::StsUnsupportedFormat :   తిరిగి "Unsupported format or combination of formats";
    స్థితి cv::Error::BadCOI :                 తిరిగి "Input COI is not supported";
    స్థితి cv::Error::Bad_అంకె_Channels :         తిరిగి "Bad number of channels";
    స్థితి cv::Error::StsBadFlag :             తిరిగి "Bad flag (parameter or structure field)";
    స్థితి cv::Error::Sts_Bad_బిందువు :            తిరిగి "Bad parameter of type CvPoint";
    స్థితి cv::Error::Sts_Bad_ముసుగు :             తిరిగి "Bad type of mask argument";
    స్థితి cv::Error::StsParseError :          తిరిగి "Parsing error";
    స్థితి cv::Error::StsNotImplemented :      తిరిగి "The function/feature is not implemented";
    స్థితి cv::Error::Sts_Bad_Mem_ఖండం :         తిరిగి "Memory block has been corrupted";
    స్థితి cv::Error::StsAssert :              తిరిగి "Assertion failed";
    స్థితి cv::Error::GpuNotSupported :        తిరిగి "No CUDA support";
    స్థితి cv::Error::GpuApiCallError :        తిరిగి "Gpu API call";
    స్థితి cv::Error::OpenGlNotSupported :     తిరిగి "No OpenGL support";
    స్థితి cv::Error::OpenGlApiCallError :     తిరిగి "OpenGL API call";
    స్థితి cv::Error::OpenCLApiCallError :     తిరిగి "OpenCL API call error";
    స్థితి cv::Error::OpenCLDoubleNotSupported:తిరిగి "OpenCL device does not support double";
    స్థితి cv::Error::OpenCLInitError :        తిరిగి "OpenCL initialization error";
    స్థితి cv::Error::OpenCLNoAMDBlasFft :     తిరిగి "Missing OpenCL AMD BLAS FFT";
    అప్రమేయ:
    {
        స్థిర అక్షరం buf[256] {0};
        snprintf(buf, పరిమాణం(buf), "Unknown %s code %d", సంకేతం_2 >= 0 ? "status":"error", సంకేతం_2);
        తిరిగి buf;
    }
    };

}

స్థిర స్థిరరాశి అక్షరం* g_hw_Feature_పేర్లు[CV_HARDWARE_MAX_FEATURE] = { ఖాళీ };

స్థిర స్థిరరాశి అక్షరం* getHWFeatureName(పూర్ణాంకం id)
{
    తిరిగి (0 <= id && id < CV_HARDWARE_MAX_FEATURE) ? g_hw_Feature_పేర్లు[id] : ఖాళీ;
}
స్థిర స్థిరరాశి అక్షరం* get_HWFeature_Name_సురక్షిత(పూర్ణాంకం id)
{
    స్థిరరాశి అక్షరం* name = getHWFeatureName(id);
    తిరిగి name ? name : "Unknown feature";
}

నిర్మాణం HWFeatures
{
    గణన { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };

    HWFeatures(తార్కిక నిరంతరం_initialize = అబద్ధం)
    {
        ఒకవేళ (నిరంతరం_initialize)
            initialize();
    }

    స్థిర శూన్య initialize_పేర్లు()
    {
        కొరకు (పూర్ణాంకం i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
        {
            g_hw_Feature_పేర్లు[i] = 0;
        }
        g_hw_Feature_పేర్లు[CPU_MMX] = "MMX";
        g_hw_Feature_పేర్లు[CPU_SSE] = "SSE";
        g_hw_Feature_పేర్లు[CPU_SSE2] = "SSE2";
        g_hw_Feature_పేర్లు[CPU_SSE3] = "SSE3";
        g_hw_Feature_పేర్లు[CPU_SSSE3] = "SSSE3";
        g_hw_Feature_పేర్లు[CPU_SSE4_1] = "SSE4.1";
        g_hw_Feature_పేర్లు[CPU_SSE4_2] = "SSE4.2";
        g_hw_Feature_పేర్లు[CPU_POPCNT] = "POPCNT";
        g_hw_Feature_పేర్లు[CPU_FP16] = "FP16";
        g_hw_Feature_పేర్లు[CPU_AVX] = "AVX";
        g_hw_Feature_పేర్లు[CPU_AVX2] = "AVX2";
        g_hw_Feature_పేర్లు[CPU_FMA3] = "FMA3";

        g_hw_Feature_పేర్లు[CPU_AVX_512F] = "AVX512F";
        g_hw_Feature_పేర్లు[CPU_AVX_512BW] = "AVX512BW";
        g_hw_Feature_పేర్లు[CPU_AVX_512CD] = "AVX512CD";
        g_hw_Feature_పేర్లు[CPU_AVX_512DQ] = "AVX512DQ";
        g_hw_Feature_పేర్లు[CPU_AVX_512ER] = "AVX512ER";
        g_hw_Feature_పేర్లు[CPU_AVX_512IFMA] = "AVX512IFMA";
        g_hw_Feature_పేర్లు[CPU_AVX_512PF] = "AVX512PF";
        g_hw_Feature_పేర్లు[CPU_AVX_512VBMI] = "AVX512VBMI";
        g_hw_Feature_పేర్లు[CPU_AVX_512VL] = "AVX512VL";
        g_hw_Feature_పేర్లు[CPU_AVX_512VBMI2] = "AVX512VBMI2";
        g_hw_Feature_పేర్లు[CPU_AVX_512VNNI] = "AVX512VNNI";
        g_hw_Feature_పేర్లు[CPU_AVX_512BITALG] = "AVX512BITALG";
        g_hw_Feature_పేర్లు[CPU_AVX_512VPOPCNTDQ] = "AVX512VPOPCNTDQ";
        g_hw_Feature_పేర్లు[CPU_AVX_5124VNNIW] = "AVX5124VNNIW";
        g_hw_Feature_పేర్లు[CPU_AVX_5124FMAPS] = "AVX5124FMAPS";
        g_hw_Feature_పేర్లు[CPU_AVX_VNNI] = "AVX_VNNI";

        g_hw_Feature_పేర్లు[CPU_NEON] = "NEON";
        g_hw_Feature_పేర్లు[CPU_NEON_DOTPROD] = "NEON_DOTPROD";
        g_hw_Feature_పేర్లు[CPU_NEON_FP16] = "NEON_FP16";
        g_hw_Feature_పేర్లు[CPU_NEON_BF16] = "NEON_BF16";
        g_hw_Feature_పేర్లు[CPU_SVE] = "SVE";

        g_hw_Feature_పేర్లు[CPU_VSX] = "VSX";
        g_hw_Feature_పేర్లు[CPU_VSX3] = "VSX3";

        g_hw_Feature_పేర్లు[CPU_MSA] = "CPU_MSA";
        g_hw_Feature_పేర్లు[CPU_RISCVV] = "RISCVV";

        g_hw_Feature_పేర్లు[CPU_AVX512_COMMON] = "AVX512-COMMON";
        g_hw_Feature_పేర్లు[CPU_AVX512_SKX] = "AVX512-SKX";
        g_hw_Feature_పేర్లు[CPU_AVX512_KNL] = "AVX512-KNL";
        g_hw_Feature_పేర్లు[CPU_AVX512_KNM] = "AVX512-KNM";
        g_hw_Feature_పేర్లు[CPU_AVX512_CNL] = "AVX512-CNL";
        g_hw_Feature_పేర్లు[CPU_AVX512_CLX] = "AVX512-CLX";
        g_hw_Feature_పేర్లు[CPU_AVX512_ICL] = "AVX512-ICL";

        g_hw_Feature_పేర్లు[CPU_RVV] = "RVV";
        g_hw_Feature_పేర్లు[CPU_RVV_ZVFH] = "RVV_ZVFH";

        g_hw_Feature_పేర్లు[CPU_LSX]  = "LSX";
        g_hw_Feature_పేర్లు[CPU_LASX] = "LASX";
    }

    శూన్య initialize(శూన్య)
    {
        ఒకవేళ (utils::getConfigurationParameterBool("OPENCV_DUMP_CONFIG"))
        {
            fprintf(stderr, "\nOpenCV build configuration is:\n%s\n",
                cv::getBuildInformation().c_str());
        }

        initialize_పేర్లు();

    #నిర్వచితమైతే CV_CPUID_X86
        పూర్ణాంకం cpuid_దత్తాంశం[4] = { 0, 0, 0, 0 };
        పూర్ణాంకం cpuid_దత్తాంశం_ex[4] = { 0, 0, 0, 0 };

        CV_CPUID_X86(cpuid_దత్తాంశం, 1, 0/*వాడని*/);

        పూర్ణాంకం x86_family = (cpuid_దత్తాంశం[0] >> 8) & 15;
        ఒకవేళ( x86_family >= 6 )
        {
            have[CV_CPU_MMX]    = (cpuid_దత్తాంశం[3] & (1<<23)) != 0;
            have[CV_CPU_SSE]    = (cpuid_దత్తాంశం[3] & (1<<25)) != 0;
            have[CV_CPU_SSE2]   = (cpuid_దత్తాంశం[3] & (1<<26)) != 0;
            have[CV_CPU_SSE3]   = (cpuid_దత్తాంశం[2] & (1<<0)) != 0;
            have[CV_CPU_SSSE3]  = (cpuid_దత్తాంశం[2] & (1<<9)) != 0;
            have[CV_CPU_FMA3]   = (cpuid_దత్తాంశం[2] & (1<<12)) != 0;
            have[CV_CPU_SSE4_1] = (cpuid_దత్తాంశం[2] & (1<<19)) != 0;
            have[CV_CPU_SSE4_2] = (cpuid_దత్తాంశం[2] & (1<<20)) != 0;
            have[CV_CPU_POPCNT] = (cpuid_దత్తాంశం[2] & (1<<23)) != 0;
            have[CV_CPU_AVX]    = (cpuid_దత్తాంశం[2] & (1<<28)) != 0;
            have[CV_CPU_FP16]   = (cpuid_దత్తాంశం[2] & (1<<29)) != 0;

            // make the second call to the cpuid command in order to get
            // information about extended features like AVX2
            CV_CPUID_X86(cpuid_దత్తాంశం_ex, 7, 0);

            have[CV_CPU_AVX2]   = (cpuid_దత్తాంశం_ex[1] & (1<<5)) != 0;

            have[CV_CPU_AVX_512F]         = (cpuid_దత్తాంశం_ex[1] & (1<<16)) != 0;
            have[CV_CPU_AVX_512DQ]        = (cpuid_దత్తాంశం_ex[1] & (1<<17)) != 0;
            have[CV_CPU_AVX_512IFMA]      = (cpuid_దత్తాంశం_ex[1] & (1<<21)) != 0;
            have[CV_CPU_AVX_512PF]        = (cpuid_దత్తాంశం_ex[1] & (1<<26)) != 0;
            have[CV_CPU_AVX_512ER]        = (cpuid_దత్తాంశం_ex[1] & (1<<27)) != 0;
            have[CV_CPU_AVX_512CD]        = (cpuid_దత్తాంశం_ex[1] & (1<<28)) != 0;
            have[CV_CPU_AVX_512BW]        = (cpuid_దత్తాంశం_ex[1] & (1<<30)) != 0;
            have[CV_CPU_AVX_512VL]        = (cpuid_దత్తాంశం_ex[1] & (1<<31)) != 0;
            have[CV_CPU_AVX_512VBMI]      = (cpuid_దత్తాంశం_ex[2] & (1<<1))  != 0;
            have[CV_CPU_AVX_512VBMI2]     = (cpuid_దత్తాంశం_ex[2] & (1<<6))  != 0;
            have[CV_CPU_AVX_512VNNI]      = (cpuid_దత్తాంశం_ex[2] & (1<<11)) != 0;
            have[CV_CPU_AVX_512BITALG]    = (cpuid_దత్తాంశం_ex[2] & (1<<12)) != 0;
            have[CV_CPU_AVX_512VPOPCNTDQ] = (cpuid_దత్తాంశం_ex[2] & (1<<14)) != 0;
            have[CV_CPU_AVX_5124VNNIW]    = (cpuid_దత్తాంశం_ex[3] & (1<<2))  != 0;
            have[CV_CPU_AVX_5124FMAPS]    = (cpuid_దత్తాంశం_ex[3] & (1<<3))  != 0;

            // CPUID leaf 7, subleaf 1 for AVX-VNNI
            పూర్ణాంకం cpuid_దత్తాంశం_ex1[4] = { 0, 0, 0, 0 };
            CV_CPUID_X86(cpuid_దత్తాంశం_ex1, 7, 1);
            have[CV_CPU_AVX_VNNI]         = (cpuid_దత్తాంశం_ex1[0] & (1<<4))  != 0;

            తార్కిక have_AVX_OS_support = నిజం;
            తార్కిక have_AVX512_OS_support = నిజం;
            ఒకవేళ (!(cpuid_దత్తాంశం[2] & (1<<27)))
                have_AVX_OS_support = అబద్ధం; // OS uses XSAVE_XRSTORE and CPU support AVX
            లేకపోతే
            {
                పూర్ణాంకం xcr0 = 0;
            #నిర్వచితమైతే XCR_XFEATURE_ENABLED_ముసుగు // requires immintrin.h
                xcr0 = (పూర్ణాంకం)_xgetbv(XCR_XFEATURE_ENABLED_ముసుగు);
            #లేదావేళ defined __GNUC__ && (defined __i386__ || defined __x86_64__)
                __asm__ ("xgetbv\n\t" : "=a" (xcr0) : "c" (0) : "%edx" );
            #అయితే_ముగింపు
                ఒకవేళ ((xcr0 & 0x6) != 0x6)
                    have_AVX_OS_support = అబద్ధం; // YMM registers
                ఒకవేళ ((xcr0 & 0xe6) != 0xe6)
                    have_AVX512_OS_support = అబద్ధం; // ZMM registers
            }

            ఒకవేళ (!have_AVX_OS_support)
            {
                have[CV_CPU_AVX] = అబద్ధం;
                have[CV_CPU_FP16] = అబద్ధం;
                have[CV_CPU_AVX2] = అబద్ధం;
                have[CV_CPU_FMA3] = అబద్ధం;
                have[CV_CPU_AVX_VNNI] = అబద్ధం;
            }
            ఒకవేళ (!have_AVX_OS_support || !have_AVX512_OS_support)
            {
                have[CV_CPU_AVX_512F] = అబద్ధం;
                have[CV_CPU_AVX_512BW] = అబద్ధం;
                have[CV_CPU_AVX_512CD] = అబద్ధం;
                have[CV_CPU_AVX_512DQ] = అబద్ధం;
                have[CV_CPU_AVX_512ER] = అబద్ధం;
                have[CV_CPU_AVX_512IFMA] = అబద్ధం;
                have[CV_CPU_AVX_512PF] = అబద్ధం;
                have[CV_CPU_AVX_512VBMI] = అబద్ధం;
                have[CV_CPU_AVX_512VL] = అబద్ధం;
                have[CV_CPU_AVX_512VBMI2] = అబద్ధం;
                have[CV_CPU_AVX_512VNNI] = అబద్ధం;
                have[CV_CPU_AVX_512BITALG] = అబద్ధం;
                have[CV_CPU_AVX_512VPOPCNTDQ] = అబద్ధం;
                have[CV_CPU_AVX_5124VNNIW] = అబద్ధం;
                have[CV_CPU_AVX_5124FMAPS] = అబద్ధం;
            }

            have[CV_CPU_AVX512_COMMON] = have[CV_CPU_AVX_512F] && have[CV_CPU_AVX_512CD];
            ఒకవేళ (have[CV_CPU_AVX512_COMMON])
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
            లేకపోతే
            {
                have[CV_CPU_AVX512_KNL] = అబద్ధం;
                have[CV_CPU_AVX512_KNM] = అబద్ధం;
                have[CV_CPU_AVX512_SKX] = అబద్ధం;
                have[CV_CPU_AVX512_CNL] = అబద్ధం;
                have[CV_CPU_AVX512_CLX] = అబద్ధం;
                have[CV_CPU_AVX512_ICL] = అబద్ధం;
            }
        }
    #అయితే_ముగింపు // CV_CPUID_X86

    #ఒకవేళ defined __ANDROID__ || defined __linux__ || defined __QNX__
    #నిర్వచితమైతే __aarch64__
        have[CV_CPU_NEON] = నిజం;
        have[CV_CPU_FP16] = నిజం;
        పూర్ణాంకం cpufile = open("/proc/self/auxv", O_RDONLY);

        ఒకవేళ (cpufile >= 0)
        {
            Elf64_auxv_t auxv;
            స్థిరరాశి size_t కొలత_auxv_t = పరిమాణం(auxv);

            అయితే ((size_t)పఠ(cpufile, &auxv, కొలత_auxv_t) == కొలత_auxv_t)
            {
                // see https://elixir.bootlin.com/linux/latest/source/arch/arm64/include/uapi/asm/hwcap.h
                ఒకవేళ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON_DOTPROD] = (auxv.a_un.a_val & (1 << 20)) != 0; // HWCAP_ASIMDDP
                    have[CV_CPU_NEON_FP16] = (auxv.a_un.a_val & (1 << 10)) != 0; // HWCAP_ASIMDHP
                    have[CV_CPU_SVE] = (auxv.a_un.a_val & (1 << 22)) != 0; // HWCAP_SVE
                }
#ఒకవేళ defined(AT_HWCAP2)
                లేకపోతే ఒకవేళ (auxv.a_type == AT_HWCAP2)
                {
                    have[CV_CPU_NEON_BF16] = (auxv.a_un.a_val & (1 << 14)) != 0; // HWCAP2_BF16
                }
#అయితే_ముగింపు
            }

            close(cpufile);
        }
    #లేదావేళ defined __arm__ && defined __ANDROID__
      #ఒకవేళ defined HAVE_CPUFEATURES
        CV_LOG_INFO(ఖాళీ, "calling android_getCpuFeatures() ...");
        uint64_t features = android_getCpuFeatures();
        CV_LOG_INFO(ఖాళీ, cv::format("calling android_getCpuFeatures() ... Done (%llx)", (దీర్ఘ దీర్ఘ)features));
        have[CV_CPU_NEON] = (features & ANDROID_CPU_ARM_FEATURE_NEON) != 0;
        have[CV_CPU_FP16] = (features & ANDROID_CPU_ARM_FEATURE_VFP_FP16) != 0;
      #లేకపోతే
        CV_LOG_INFO(ఖాళీ, "cpufeatures library is not available for CPU detection");
        #ఒకవేళ CV_NEON
        CV_LOG_INFO(ఖాళీ, "- NEON instructions is enabled via build flags");
        have[CV_CPU_NEON] = నిజం;
        #లేకపోతే
        CV_LOG_INFO(ఖాళీ, "- NEON instructions is NOT enabled via build flags");
        #అయితే_ముగింపు
        #ఒకవేళ CV_FP16
        CV_LOG_INFO(ఖాళీ, "- FP16 instructions is enabled via build flags");
        have[CV_CPU_FP16] = నిజం;
        #లేకపోతే
        CV_LOG_INFO(ఖాళీ, "- FP16 instructions is NOT enabled via build flags");
        #అయితే_ముగింపు
      #అయితే_ముగింపు
    #లేదావేళ defined __arm__
        పూర్ణాంకం cpufile = open("/proc/self/auxv", O_RDONLY);

        ఒకవేళ (cpufile >= 0)
        {
            Elf32_auxv_t auxv;
            స్థిరరాశి size_t కొలత_auxv_t = పరిమాణం(auxv);

            అయితే ((size_t)పఠ(cpufile, &auxv, కొలత_auxv_t) == కొలత_auxv_t)
            {
                ఒకవేళ (auxv.a_type == AT_HWCAP)
                {
                    have[CV_CPU_NEON] = (auxv.a_un.a_val & 4096) != 0;
                    have[CV_CPU_FP16] = (auxv.a_un.a_val & 2) != 0;
                    ఆపు;
                }
            }

            close(cpufile);
        }
    #అయితే_ముగింపు
    #లేదావేళ (defined __APPLE__)
    #ఒకవేళ defined __ARM_NEON
        have[CV_CPU_NEON] = నిజం;
    #అయితే_ముగింపు
    #ఒకవేళ (defined __ARM_FP  && (((__ARM_FP & 0x2) != 0) && defined __ARM_NEON))
        have[CV_CPU_FP16] = have[CV_CPU_NEON_FP16] = నిజం;
    #అయితే_ముగింపు
    // system.cpp may be compiled w/o special -march=armv8...+dotprod, -march=armv8...+bf16 etc.,
    // so we check for the features in any case, no mater what are the compile flags.
    // We check the real hardware capabilities here.
    పూర్ణాంకం has_feat_dotprod = 0;
    size_t has_feat_dotprod_కొలత = పరిమాణం(has_feat_dotprod);
    sysctlbyname("hw.optional.arm.FEAT_DotProd", &has_feat_dotprod, &has_feat_dotprod_కొలత, ఖాళీ, 0);
    ఒకవేళ (has_feat_dotprod) {
        have[CV_CPU_NEON_DOTPROD] = నిజం;
    }
    పూర్ణాంకం has_feat_bf16 = 0;
    size_t has_feat_bf16_కొలత = పరిమాణం(has_feat_bf16);
    sysctlbyname("hw.optional.arm.FEAT_BF16", &has_feat_bf16, &has_feat_bf16_కొలత, ఖాళీ, 0);
    ఒకవేళ (has_feat_bf16) {
        have[CV_CPU_NEON_BF16] = నిజం;
    }
    #లేదావేళ (defined __clang__)
    #ఒకవేళ defined __ARM_NEON
        have[CV_CPU_NEON] = నిజం;
        #ఒకవేళ (defined __ARM_FP  && ((__ARM_FP & 0x2) != 0))
        have[CV_CPU_FP16] = నిజం;
        #అయితే_ముగింపు
    #అయితే_ముగింపు
    #అయితే_ముగింపు
    #ఒకవేళ defined _ARM_ && (defined(_WIN32_WCE) && _WIN32_WCE >= 0x800)
        have[CV_CPU_NEON] = నిజం;
    #అయితే_ముగింపు
    #ఒకవేళ defined _M_ARM64 || defined _M_ARM64EC
        have[CV_CPU_NEON] = నిజం;
    #అయితే_ముగింపు
    #నిర్వచితమైతే __riscv_vector
        have[CV_CPU_RISCVV] = నిజం;
    #అయితే_ముగింపు
    #నిర్వచితమైతే __mips_msa
        have[CV_CPU_MSA] = నిజం;
    #అయితే_ముగింపు

    #ఒకవేళ (defined __ppc64__ || defined __PPC64__) && defined HAVE_GETAUXVAL
        గుర్తులేని పూర్ణాంకం hwcap = getauxval(AT_HWCAP);
        ఒకవేళ (hwcap & PPC_FEATURE_HAS_VSX) {
            hwcap = getauxval(AT_HWCAP2);
            ఒకవేళ (hwcap & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = నిజం;
            } లేకపోతే {
                have[CV_CPU_VSX] = (hwcap & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #లేదావేళ (defined __ppc64__ || defined __PPC64__) && defined HAVE_ELF_AUX_INFO
        గుర్తులేని దీర్ఘ hwcap = 0, hwcap2 = 0;
        ఒకవేళ (elf_aux_info(AT_HWCAP, &hwcap, పరిమాణం(hwcap)) != 0)
            hwcap = 0;
        ఒకవేళ (hwcap & PPC_FEATURE_HAS_VSX) {
            ఒకవేళ (elf_aux_info(AT_HWCAP2, &hwcap2, పరిమాణం(hwcap2)) != 0)
                hwcap2 = 0;
            ఒకవేళ (hwcap2 & PPC_FEATURE2_ARCH_3_00) {
                have[CV_CPU_VSX] = have[CV_CPU_VSX3] = నిజం;
            } లేకపోతే {
                have[CV_CPU_VSX] = (hwcap2 & PPC_FEATURE2_ARCH_2_07) != 0;
            }
        }
    #లేకపోతే
        // TODO: AIX
        #ఒకవేళ CV_VSX || defined _ARCH_PWR8 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX] = నిజం;
        #అయితే_ముగింపు
        #ఒకవేళ CV_VSX3 || defined __POWER9_VECTOR__
            have[CV_CPU_VSX3] = నిజం;
        #అయితే_ముగింపు
    #అయితే_ముగింపు

    #ఒకవేళ defined __riscv && defined __riscv_vector
        have[CV_CPU_RVV] = నిజం;
        #ఒకవేళ (defined __riscv_zvfh && __riscv_zvfh) || (defined __riscv_zvfhmin && __riscv_zvfhmin)
            have[CV_CPU_FP16] = నిజం;
        #అయితే_ముగింపు
        #ఒకవేళ defined __riscv_zvfh && __riscv_zvfh
            have[CV_CPU_RVV_ZVFH] = నిజం;
        #అయితే_ముగింపు
    #అయితే_ముగింపు

    #ఒకవేళ defined __loongarch64 && defined __linux__
        పూర్ణాంకం flag = (పూర్ణాంకం)getauxval(AT_HWCAP);

        have[CV_CPU_LSX] = (flag & LA_HWCAP_LSX) != 0;
        have[CV_CPU_LASX] = (flag & LA_HWCAP_LASX) != 0;
    #అయితే_ముగింపు

        తార్కిక skip_baseline_check = అబద్ధం;
        ఒకవేళ (utils::getConfigurationParameterBool("OPENCV_SKIP_CPU_BASELINE_CHECK"))
        {
            skip_baseline_check = నిజం;
        }
        పూర్ణాంకం baseline_features[] = { CV_CPU_BASELINE_FEATURES };
        ఒకవేళ (!checkFeatures(baseline_features, పరిమాణం(baseline_features) / పరిమాణం(baseline_features[0]))
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
            checkFeatures(baseline_features, పరిమాణం(baseline_features) / పరిమాణం(baseline_features[0]), నిజం);
            CV_Error(cv::Error::StsAssert, "Missing support for required CPU baseline features. Check OpenCV build configuration and required CPU/HW setup.");
        }

        readSettings(baseline_features, పరిమాణం(baseline_features) / పరిమాణం(baseline_features[0]));
    }

    తార్కిక checkFeatures(స్థిరరాశి పూర్ణాంకం* features, పూర్ణాంకం లెక్క_2, తార్కిక dump = అబద్ధం)
    {
        తార్కిక ఫలితం = నిజం;
        కొరకు (పూర్ణాంకం i = 0; i < లెక్క_2; i++)
        {
            పూర్ణాంకం feature = features[i];
            ఒకవేళ (feature)
            {
                ఒకవేళ (have[feature])
                {
                    ఒకవేళ (dump) fprintf(stderr, "    ID=%3d (%s) - OK\n", feature, get_HWFeature_Name_సురక్షిత(feature));
                }
                లేకపోతే
                {
                    ఫలితం = అబద్ధం;
                    ఒకవేళ (dump) fprintf(stderr, "    ID=%3d (%s) - NOT AVAILABLE\n", feature, get_HWFeature_Name_సురక్షిత(feature));
                }
            }
        }
        తిరిగి ఫలితం;
    }

    స్థిర అంతఃవరుస తార్కిక is_చిహ్నం_Separator(అక్షరం c)
    {
        తిరిగి c == ',' || c == ';';
    }

    శూన్య readSettings(స్థిరరాశి పూర్ణాంకం* baseline_features, పూర్ణాంకం baseline_లెక్క)
    {
        తార్కిక dump = నిజం;
        std::string disabled_features = utils::getConfigurationParameterString("OPENCV_CPU_DISABLE");
        ఒకవేళ (!disabled_features.empty())
        {
            స్థిరరాశి అక్షరం* ప్రారంభం = disabled_features.c_str();
            కొరకు (;;)
            {
                అయితే (ప్రారంభం[0] != 0 && is_చిహ్నం_Separator(ప్రారంభం[0]))
                {
                    ప్రారంభం++;
                }
                ఒకవేళ (ప్రారంభం[0] == 0)
                    ఆపు;
                స్థిరరాశి అక్షరం* end = ప్రారంభం;
                అయితే (end[0] != 0 && !is_చిహ్నం_Separator(end[0]))
                {
                    end++;
                }
                ఒకవేళ (end == ప్రారంభం)
                    కొనసాగు;
                cv::String feature(ప్రారంభం, end);
                ప్రారంభం = end;

                CV_Assert(feature.size() > 0);

                తార్కిక దొరికింది = అబద్ధం;
                కొరకు (పూర్ణాంకం i = 0; i < CV_HARDWARE_MAX_FEATURE; i++)
                {
                    ఒకవేళ (!g_hw_Feature_పేర్లు[i]) కొనసాగు;
                    size_t len = అక్షరశ్రేణి_పొడవు(g_hw_Feature_పేర్లు[i]);
                    ఒకవేళ (len != feature.size()) కొనసాగు;
                    ఒకవేళ (feature.compare(g_hw_Feature_పేర్లు[i]) == 0)
                    {
                        తార్కిక isBaseline = అబద్ధం;
                        కొరకు (పూర్ణాంకం k = 0; k < baseline_లెక్క; k++)
                        {
                            ఒకవేళ (baseline_features[k] == i)
                            {
                                isBaseline = నిజం;
                                ఆపు;
                            }
                        }
                        ఒకవేళ (isBaseline)
                        {
                            ఒకవేళ (dump) fprintf(stderr, "OPENCV: Trying to disable baseline CPU feature: '%s'."
                                                      "This has very limited effect, because code optimizations for this feature are executed unconditionally "
                                                      "in the most cases.\n", get_HWFeature_Name_సురక్షిత(i));
                        }
                        ఒకవేళ (!have[i])
                        {
                            ఒకవేళ (dump) fprintf(stderr, "OPENCV: Trying to disable unavailable CPU feature on the current platform: '%s'.\n",
                                get_HWFeature_Name_సురక్షిత(i));
                        }
                        have[i] = అబద్ధం;

                        దొరికింది = నిజం;
                        ఆపు;
                    }
                }
                ఒకవేళ (!దొరికింది)
                {
                    ఒకవేళ (dump) fprintf(stderr, "OPENCV: Trying to disable unknown CPU feature: '%s'.\n", feature.c_str());
                }
            }
        }
    }

    తార్కిక have[MAX_FEATURE+1]{};
};

స్థిర HWFeatures  featuresEnabled(నిజం), featuresDisabled = HWFeatures(అబద్ధం);
స్థిర HWFeatures* currentFeatures = &featuresEnabled;

తార్కిక checkHardwareSupport(పూర్ణాంకం feature)
{
    ఒకవేళ (feature < 0 || feature >= CV_HARDWARE_MAX_FEATURE)
        తిరిగి అబద్ధం;
    తిరిగి currentFeatures->have[feature];
}

String getHardwareFeatureName(పూర్ణాంకం feature)
{
    స్థిరరాశి అక్షరం* name = getHWFeatureName(feature);
    తిరిగి name ? String(name) : String();
}

std::string get_CPUFeatures_పంక్తి()
{
    స్థిరరాశి పూర్ణాంకం features[] = { CV_CPU_BASELINE_FEATURES, CV_CPU_DISPATCH_FEATURES };
    స్థిరరాశి పూర్ణాంకం sz = పరిమాణం(features) / పరిమాణం(features[0]);
    std::string ఫలితం;
    std::string ఉపసర్గ;
    కొరకు (పూర్ణాంకం i = 1; i < sz; ++i)
    {
        ఒకవేళ (features[i] == 0)
        {
            ఉపసర్గ = "*";
            కొనసాగు;
        }
        ఒకవేళ (i != 1) ఫలితం.append(" ");
        ఫలితం.append(ఉపసర్గ);
        ఫలితం.append(get_HWFeature_Name_సురక్షిత(features[i]));
        ఒకవేళ (!checkHardwareSupport(features[i])) ఫలితం.append("?");
    }
    తిరిగి ఫలితం;
}

అస్థిర తార్కిక useOptimizedFlag = నిజం;

శూన్య సమితి_Use_Optimized( తార్కిక flag )
{
    useOptimizedFlag = flag;
    currentFeatures = flag ? &featuresEnabled : &featuresDisabled;

    ipp::సమితి_Use_IPP(flag);
#నిర్వచితమైతే HAVE_OPENCL
    ocl::సమితి_Use_Open_CL(flag);
#అయితే_ముగింపు
}

తార్కిక useOptimized(శూన్య)
{
    తిరిగి useOptimizedFlag;
}

int64 get_Tick_లెక్క(శూన్య)
{
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    తిరిగి (int64)now.time_since_epoch().లెక్క_2();
}

ద్విగుణ getTickFrequency(శూన్య)
{
    ఉపయోగించి clock_period_t = std::chrono::steady_clock::duration::period;
    ద్విగుణ clock_freq = clock_period_t::den / clock_period_t::num;
    తిరిగి clock_freq;
}

#ఒకవేళ defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#ఒకవేళ defined(__i386__)

int64 get_CPUTick_లెక్క(శూన్య)
{
    int64 x;
    __asm__ అస్థిర (".byte 0x0f, 0x31" : "=A" (x));
    తిరిగి x;
}
#లేదావేళ defined(__x86_64__)

int64 get_CPUTick_లెక్క(శూన్య)
{
    గుర్తులేని hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    తిరిగి (int64)lo | ((int64)hi << 32);
}

#లేదావేళ defined(__ppc__)

int64 get_CPUTick_లెక్క(శూన్య)
{
    గుర్తులేని upper, lower, tmp;
    __asm__ అస్థిర(
                     "0:                  \n"
                     "\tmftbu   %0           \n"
                     "\tmftb    %1           \n"
                     "\tmftbu   %2           \n"
                     "\tcmpw    %2,%0        \n"
                     "\tbne     0b         \n"
                     : "=r"(upper),"=r"(lower),"=r"(tmp)
                     );
    తిరిగి lower | ((int64)upper << 32);
}

#లేకపోతే

#దోషం "RDTSC not defined"

#అయితే_ముగింపు

#లేదావేళ defined _MSC_VER && defined _WIN32 && defined _M_IX86

int64 get_CPUTick_లెక్క(శూన్య)
{
    __asm _emit 0x0f;
    __asm _emit 0x31;
}

#లేకపోతే

//#ifdef HAVE_IPP
//int64 getCPUTickCount(void)
//{
//    return ippGetCpuClocks();
//}
//#else
int64 get_CPUTick_లెక్క(శూన్య)
{
    తిరిగి get_Tick_లెక్క();
}
//#endif

#అయితే_ముగింపు


నామాకాశ internal {

తరగతి Timestamp
{
బహిరంగ:
    స్థిరరాశి int64 zero_Tick_లెక్క;
    స్థిరరాశి ద్విగుణ ns_లోపల_ticks;

    Timestamp()
        : zero_Tick_లెక్క(get_Tick_లెక్క())
        , ns_లోపల_ticks(1e9 / getTickFrequency())
    {
        // nothing
    }

    int64 getTimestamp()
    {
        int64 t = get_Tick_లెక్క();
        తిరిగి (int64)((t - zero_Tick_లెక్క) * ns_లోపల_ticks);
    }

    స్థిర Timestamp& getInstance()
    {
        స్థిర Timestamp g_timestamp;
        తిరిగి g_timestamp;
    }
};

తరగతి InitTimestamp {
బహిరంగ:
    InitTimestamp() {
        Timestamp::getInstance();
    }
};
స్థిర InitTimestamp g_initialize_timestamp;  // force zero timestamp initialization

}  // namespace

int64 getTimestampNS()
{
    తిరిగి internal::Timestamp::getInstance().getTimestamp();
}


స్థిరరాశి String& getBuildInformation()
{
    స్థిర String build_info =
#అన్తర్భావ "version_string.inc"
    ;
    తిరిగి build_info;
}

String get_సంస్కరణ_String() { తిరిగి String(CV_సంస్కరణ); }

పూర్ణాంకం get_సంస్కరణ_Major() { తిరిగి CV_సంస్కరణ_MAJOR; }

పూర్ణాంకం get_సంస్కరణ_Minor() { తిరిగి CV_సంస్కరణ_MINOR; }

పూర్ణాంకం get_సంస్కరణ_Revision() { తిరిగి CV_సంస్కరణ_REVISION; }

String format( స్థిరరాశి అక్షరం* fmt, ... )
{
    Auto_బఫరు<అక్షరం, 1024> buf;

    కొరకు ( ; ; )
    {
        va_list va;
        va_start(va, fmt);
        పూర్ణాంకం bsize = static_cast<పూర్ణాంకం>(buf.size());
        పూర్ణాంకం len = cv_vsnprintf(buf.దత్తాంశం(), bsize, fmt, va);
        va_end(va);

        CV_Assert(len >= 0 && "Check format string for errors");
        ఒకవేళ (len >= bsize)
        {
            buf.resize(len + 1);
            కొనసాగు;
        }
        buf[bsize - 1] = 0;
        తిరిగి String(buf.దత్తాంశం(), len);
    }
}

String tempfile( స్థిరరాశి అక్షరం* suffix )
{
#ఒకవేళ OPENCV_HAVE_FILESYSTEM_SUPPORT
    String fname;

    std::string తాత్కాలిక_dir = utils::getConfigurationParameterString("OPENCV_TEMP_PATH");

#ఒకవేళ defined _WIN32
#నిర్వచితమైతే WINRT
    RoInitialize(RO_INIT_MULTITHREADED);
    std::wstring తాత్కాలిక_dir_rt = Get_తాత్కాలిక_మార్గం_Win_RT();

    std::wstring తాత్కాలిక_file = Get_తాత్కాలిక_File_Name_Win_RT(L"ocv");
    ఒకవేళ (తాత్కాలిక_file.empty())
        తిరిగి String();

    తాత్కాలిక_file = తాత్కాలిక_dir_rt.append(std::wstring(L"\\")).append(తాత్కాలిక_file);
    DeleteFileW(తాత్కాలిక_file.c_str());

    అక్షరం aname[MAX_మార్గం];
    size_t copied = wcstombs(aname, తాత్కాలిక_file.c_str(), MAX_మార్గం);
    CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
    fname = String(aname);
    RoUninitialize();
#లేదావేళ defined(_WIN32_WCE)
    స్థిరరాశి స్వయంచాలక k_Max_మార్గం_కొలత = MAX_మార్గం+1;
    wchar_t తాత్కాలిక_dir_ce[k_Max_మార్గం_కొలత] = {0};
    wchar_t తాత్కాలిక_file[k_Max_మార్గం_కొలత] = {0};

    ::Get_తాత్కాలిక_మార్గం_W(k_Max_మార్గం_కొలత, తాత్కాలిక_dir_ce);

    ఒకవేళ(0 != ::Get_తాత్కాలిక_File_Name_W(తాత్కాలిక_dir_ce, L"ocv", 0, తాత్కాలిక_file)) {
        DeleteFileW(తాత్కాలిక_file);
        అక్షరం aname[MAX_మార్గం];
        size_t copied = wcstombs(aname, తాత్కాలిక_file, MAX_మార్గం);
        CV_Assert((copied != MAX_మార్గం) && (copied != (size_t)-1));
        fname = String(aname);
    }
#లేకపోతే
    // Use GUID-based naming to avoid race condition with GetTempFileNameA
    // See issue #19648
    wchar_t తాత్కాలిక_dir2_w[MAX_మార్గం] = { 0 };

    ఒకవేళ (తాత్కాలిక_dir.empty())
    {
        ::Get_తాత్కాలిక_మార్గం_W(పరిమాణం(తాత్కాలిక_dir2_w)/పరిమాణం(wchar_t), తాత్కాలిక_dir2_w);
        // Convert from UTF-16 to UTF-8 to support Unicode paths
        పూర్ణాంకం len = Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, తాత్కాలిక_dir2_w, -1, ఖాళీ, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (len > 0)
        {
            std::vector<అక్షరం> utf8_buf(len);
            Wide_Char_గమ్యం_Multi_Byte(CP_UTF8, 0, తాత్కాలిక_dir2_w, -1, utf8_buf.దత్తాంశం(), len, ఖాళీ, ఖాళీ);
            తాత్కాలిక_dir = std::string(utf8_buf.దత్తాంశం());
        }
    }

    GUID g;
    HRESULT hr = CoCreateGuid(&g);
    ఒకవేళ (FAILED(hr))
        తిరిగి String();
    అక్షరం guidStr[40];
    స్థిరరాశి అక్షరం* ముసుగు = "%08x_%04x_%04x_%02x%02x_%02x%02x%02x%02x%02x%02x";
    snprintf(guidStr, పరిమాణం(guidStr), ముసుగు,
            g.Data1, g.Data2, g.Data3, (గుర్తులేని పూర్ణాంకం)g.Data4[0], (గుర్తులేని పూర్ణాంకం)g.Data4[1],
            (గుర్తులేని పూర్ణాంకం)g.Data4[2], (గుర్తులేని పూర్ణాంకం)g.Data4[3], (గుర్తులేని పూర్ణాంకం)g.Data4[4],
            (గుర్తులేని పూర్ణాంకం)g.Data4[5], (గుర్తులేని పూర్ణాంకం)g.Data4[6], (గుర్తులేని పూర్ణాంకం)g.Data4[7]);

    fname = తాత్కాలిక_dir;
    ఒకవేళ (!fname.empty() && fname[fname.size()-1] != '\\' && fname[fname.size()-1] != '/')
        fname += "\\";
    fname = fname + "ocv" + guidStr;
#అయితే_ముగింపు
# లేకపోతే
#  ifdef __ANDROID__
    //char defaultTemplate[] = "/mnt/sdcard/__opencv_temp.XXXXXX";
    అక్షరం defaultTemplate[] = "/data/local/tmp/__opencv_temp.XXXXXX";
#  లేకపోతే
    అక్షరం defaultTemplate[] = "/tmp/__opencv_temp.XXXXXX";
#  endif

    ఒకవేళ (తాత్కాలిక_dir.empty())
        fname = defaultTemplate;
    లేకపోతే
    {
        fname = తాత్కాలిక_dir;
        అక్షరం ech = fname[fname.size() - 1];
        ఒకవేళ(ech != '/' && ech != '\\')
            fname = fname + "/";
        fname = fname + "__opencv_temp.XXXXXX";
    }

    స్థిరరాశి పూర్ణాంకం fd = mkstemp((అక్షరం*)fname.c_str());
    ఒకవేళ (fd == -1) తిరిగి String();

    close(fd);
    remove(fname.c_str());
# endif

    ఒకవేళ (suffix)
    {
        ఒకవేళ (suffix[0] != '.')
            తిరిగి fname + "." + suffix;
        లేకపోతే
            తిరిగి fname + suffix;
    }
    తిరిగి fname;
#లేకపోతే // OPENCV_HAVE_FILESYSTEM_SUPPORT
    CV_వాడని(suffix);
    CV_Error(Error::StsNotImplemented, "File system support is disabled in this OpenCV build!");
#అయితే_ముగింపు // OPENCV_HAVE_FILESYSTEM_SUPPORT
}

స్థిర ErrorCallback customErrorCallback = 0;
స్థిర శూన్య* custom_Error_Callback_దత్తాంశం = 0;
స్థిర తార్కిక breakOnError = అబద్ధం;

తార్కిక సమితి_Break_On_Error(తార్కిక value)
{
    తార్కిక prevVal = breakOnError;
    breakOnError = value;
    తిరిగి prevVal;
}

పూర్ణాంకం cv_snprintf(అక్షరం* buf, పూర్ణాంకం len, స్థిరరాశి అక్షరం* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    పూర్ణాంకం res = cv_vsnprintf(buf, len, fmt, va);
    va_end(va);
    తిరిగి res;
}

పూర్ణాంకం cv_vsnprintf(అక్షరం* buf, పూర్ణాంకం len, స్థిరరాశి అక్షరం* fmt, va_list వాదనలు)
{
#ఒకవేళ defined _MSC_VER
    ఒకవేళ (len <= 0) తిరిగి len == 0 ? 1024 : -1;
    పూర్ణాంకం res = _vsnprintf_s(buf, len, _TRUNCATE, fmt, వాదనలు);
    // ensure null terminating on VS
    ఒకవేళ (res >= 0 && res < len)
    {
        buf[res] = 0;
        తిరిగి res;
    }
    లేకపోతే
    {
        buf[len - 1] = 0; // truncate happened
        తిరిగి res >= len ? res : (len * 2);
    }
#లేకపోతే
    తిరిగి vsnprintf(buf, len, fmt, వాదనలు);
#అయితే_ముగింపు
}

స్థిర శూన్య dumpException(స్థిరరాశి Exception& exc)
{
    స్థిరరాశి అక్షరం* errorStr = exc.సంకేతం_సందేశం();
    అక్షరం buf[1 << 12];

    cv_snprintf(buf, పరిమాణం(buf),
        "OpenCV(%s) Error: %s (%s) in %s, file %s, line %d",
        CV_సంస్కరణ,
        errorStr, exc.err.c_str(), exc.func.size() > 0 ?
        exc.func.c_str() : "unknown function", exc.file.c_str(), exc.పంక్తి);
#నిర్వచితమైతే __ANDROID__
    __android_log_print(ANDROID_LOG_ERROR, "cv::error()", "%s", buf);
#లేకపోతే
    fflush(stdout); fflush(stderr);
    fprintf(stderr, "%s\n", buf);
    fflush(stderr);
#అయితే_ముగింపు
}

#నిర్వచితమైతే CV_ERROR_సమితి_TERMINATE_HANDLER
స్థిర తార్కిక cv_terminate_handler_installed = అబద్ధం;
స్థిర std::terminate_handler cv_old_terminate_handler;
స్థిర cv::Exception cv_terminate_handler_exception;
స్థిర తార్కిక param_అమరిక_Terminate_Handler = utils::getConfigurationParameterBool("OPENCV_SETUP_TERMINATE_HANDLER", నిజం);
స్థిర శూన్య cv_terminate_handler() {
    std::cerr << "OpenCV: terminate handler is called! The last OpenCV error is:\n";
    dumpException(cv_terminate_handler_exception);
    ఒకవేళ (అబద్ధం /*cv_old_terminate_handler*/)  // buggy behavior is observed with doubled "abort/retry/ignore" windows
        cv_old_terminate_handler();
    abort();
}

#అయితే_ముగింపు

#నిర్వచితమైతే __GNUC__
# ఒకవేళ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Winvalid-noreturn"
# endif
#అయితే_ముగింపు

శూన్య error( స్థిరరాశి Exception& exc )
{
#నిర్వచితమైతే CV_ERROR_సమితి_TERMINATE_HANDLER
    {
        cv::Auto_తాళం lock(getInitializationMutex());
        ఒకవేళ (!cv_terminate_handler_installed)
        {
            ఒకవేళ (param_అమరిక_Terminate_Handler)
                cv_old_terminate_handler = std::set_terminate(cv_terminate_handler);
            cv_terminate_handler_installed = నిజం;
        }
        cv_terminate_handler_exception = exc;
    }
#అయితే_ముగింపు

    ఒకవేళ (customErrorCallback != 0)
        customErrorCallback(exc.సంకేతం_2, exc.func.c_str(), exc.err.c_str(),
                            exc.file.c_str(), exc.పంక్తి, custom_Error_Callback_దత్తాంశం);
    లేకపోతే ఒకవేళ (param_dumpErrors)
    {
        dumpException(exc);
    }

    ఒకవేళ(breakOnError)
    {
        std::terminate();
    }

    విసురు exc;
#నిర్వచితమైతే __GNUC__
# ఒకవేళ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#అయితే_ముగింపు
}

శూన్య error(Error::సంకేతం_4 సంకేతం_5, స్థిరరాశి String& _err, స్థిరరాశి అక్షరం* _func, స్థిరరాశి అక్షరం* _file, పూర్ణాంకం పంక్తి_3)
{
    error(cv::Exception(సంకేతం_5, _err, _func, _file, పంక్తి_3));
#నిర్వచితమైతే __GNUC__
# ఒకవేళ !defined __clang__ && !defined __APPLE__
    // this suppresses this warning: "noreturn" function does return [enabled by default]
    __builtin_trap();
    // or use infinite loop: for (;;) {}
# endif
#అయితే_ముగింపు
}

#నిర్వచితమైతే __GNUC__
# ఒకవేళ defined __clang__ || defined __APPLE__
#   pragma GCC diagnostic pop
# endif
#అయితే_ముగింపు


ErrorCallback
redirectError( ErrorCallback errCallback, శూన్య* userdata, శూన్య** prevUserdata)
{
    ఒకవేళ( prevUserdata )
        *prevUserdata = custom_Error_Callback_దత్తాంశం;

    ErrorCallback prevCallback = customErrorCallback;

    customErrorCallback     = errCallback;
    custom_Error_Callback_దత్తాంశం = userdata;

    తిరిగి prevCallback;
}

శూన్య terminate(Error::సంకేతం_4 సంకేతం_5, స్థిరరాశి String& _err, స్థిరరాశి అక్షరం* _func, స్థిరరాశి అక్షరం* _file, పూర్ణాంకం పంక్తి_3) CV_NOEXCEPT
{
    dumpException(cv::Exception(సంకేతం_5, _err, _func, _file, పంక్తి_3));
    std::terminate();
}

}

నామాకాశ cv {
తార్కిక __termination = అబద్ధం;


//////////////////////////////// thread-local storage ////////////////////////////////

నామాకాశ details {

#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT

#నిర్వచితమైతే _WIN32
#నిర్వచితమైతే _MSC_VER
#సూచన warning(disable:4505) // unreferenced local function has been removed
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే TLS_బహిర్గతం_యొక్క_INDEXES
#నిర్వచించు TLS_బహిర్గతం_యొక్క_INDEXES ((DWORD)0xFFFFFFFF)
#అయితే_ముగింపు
#అయితే_ముగింపు

// TLS platform abstraction layer
తరగతి TlsAbstraction
{
బహిరంగ:
    TlsAbstraction();
    ~TlsAbstraction()
    {
        // TlsAbstraction singleton should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // NB: Do NOT use logging here
        fprintf(stderr, "OpenCV FATAL: TlsAbstraction::~TlsAbstraction() call is not expected\n");
        fflush(stderr);
    }

    శూన్య* get_దత్తాంశం() స్థిరరాశి;
    శూన్య సమితి_దత్తాంశం(శూన్య *p_దత్తాంశం);

    శూన్య releaseSystemResources();

అంతరంగ:

#నిర్వచితమైతే _WIN32
#నిర్వచితం_కాకపోతే WINRT
    DWORD tls_కీలకం;
    తార్కిక disposed;
#అయితే_ముగింపు
#లేకపోతే // _WIN32
    pthread_key_t  tls_కీలకం;
    std::atomic<తార్కిక> disposed;
#అయితే_ముగింపు
};

తరగతి TlsAbstractionReleaseGuard
{
    TlsAbstraction& tls_;
బహిరంగ:
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
స్థిర TlsAbstraction* getTlsAbstraction()
{
    స్థిర TlsAbstraction *g_tls = కొత్త TlsAbstraction();  // memory leak is intended here to avoid disposing of TLS container
    స్థిర TlsAbstractionReleaseGuard g_tlsReleaseGuard(*g_tls);
    తిరిగి g_tls;
}


#నిర్వచితమైతే _WIN32
#నిర్వచితమైతే WINRT
స్థిర __declspec( thread ) శూన్య* tls_దత్తాంశం = ఖాళీ; // using C++11 thread attribute for local thread data
TlsAbstraction::TlsAbstraction() {}
శూన్య TlsAbstraction::releaseSystemResources()
{
    cv::__termination = నిజం;  // DllMain is missing in static builds
}
శూన్య* TlsAbstraction::get_దత్తాంశం() స్థిరరాశి
{
    తిరిగి tls_దత్తాంశం;
}
శూన్య TlsAbstraction::సమితి_దత్తాంశం(శూన్య *p_దత్తాంశం)
{
    tls_దత్తాంశం = p_దత్తాంశం;
}
#లేకపోతే //WINRT
#నిర్వచితమైతే CV_USE_FLS
స్థిర శూన్య NTAPI opencv_fls_destructor(శూన్య* p_దత్తాంశం);
#అయితే_ముగింపు // CV_USE_FLS
TlsAbstraction::TlsAbstraction()
    : disposed(అబద్ధం)
{
#నిర్వచితం_కాకపోతే CV_USE_FLS
    tls_కీలకం = TlsAlloc();
#లేకపోతే // CV_USE_FLS
    tls_కీలకం = FlsAlloc(opencv_fls_destructor);
#అయితే_ముగింపు // CV_USE_FLS
    CV_Assert(tls_కీలకం != TLS_బహిర్గతం_యొక్క_INDEXES);
}
శూన్య TlsAbstraction::releaseSystemResources()
{
    cv::__termination = నిజం;  // DllMain is missing in static builds
    disposed = నిజం;
#నిర్వచితం_కాకపోతే CV_USE_FLS
    TlsFree(tls_కీలకం);
#లేకపోతే // CV_USE_FLS
    FlsFree(tls_కీలకం);
#అయితే_ముగింపు // CV_USE_FLS
    tls_కీలకం = TLS_బహిర్గతం_యొక్క_INDEXES;
}
శూన్య* TlsAbstraction::get_దత్తాంశం() స్థిరరాశి
{
    ఒకవేళ (disposed)
        తిరిగి ఖాళీ;
#నిర్వచితం_కాకపోతే CV_USE_FLS
    తిరిగి Tls_Get_విలువ(tls_కీలకం);
#లేకపోతే // CV_USE_FLS
    తిరిగి Fls_Get_విలువ(tls_కీలకం);
#అయితే_ముగింపు // CV_USE_FLS
}
శూన్య TlsAbstraction::సమితి_దత్తాంశం(శూన్య *p_దత్తాంశం)
{
    ఒకవేళ (disposed)
        తిరిగి;  // no-op
#నిర్వచితం_కాకపోతే CV_USE_FLS
    CV_Assert(Tls_సమితి_విలువ(tls_కీలకం, p_దత్తాంశం) == TRUE);
#లేకపోతే // CV_USE_FLS
    CV_Assert(Fls_సమితి_విలువ(tls_కీలకం, p_దత్తాంశం) == TRUE);
#అయితే_ముగింపు // CV_USE_FLS
}
#అయితే_ముగింపు // WINRT
#లేకపోతే // _WIN32
స్థిర శూన్య opencv_tls_destructor(శూన్య* p_దత్తాంశం);
TlsAbstraction::TlsAbstraction()
    : disposed(అబద్ధం)
{
    CV_Assert(pthread_key_create(&tls_కీలకం, opencv_tls_destructor) == 0);
}
శూన్య TlsAbstraction::releaseSystemResources()
{
    cv::__termination = నిజం;  // DllMain is missing in static builds
    disposed = నిజం;
    ఒకవేళ (pthread_key_delete(tls_కీలకం) != 0)
    {
        // Don't use logging here
        fprintf(stderr, "OpenCV ERROR: TlsAbstraction::~TlsAbstraction(): pthread_key_delete() call failed\n");
        fflush(stderr);
    }
}
శూన్య* TlsAbstraction::get_దత్తాంశం() స్థిరరాశి
{
    ఒకవేళ (disposed)
        తిరిగి ఖాళీ;
    తిరిగి pthread_getspecific(tls_కీలకం);
}
శూన్య TlsAbstraction::సమితి_దత్తాంశం(శూన్య *p_దత్తాంశం)
{
    ఒకవేళ (disposed)
        తిరిగి;  // no-op
    CV_Assert(pthread_setspecific(tls_కీలకం, p_దత్తాంశం) == 0);
}
#అయితే_ముగింపు

// Per-thread data structure
నిర్మాణం Thread_దత్తాంశం
{
    Thread_దత్తాంశం()
    {
        idx = 0;
        slots.reserve(32);
    }

    std::vector<శూన్య*> slots; // Data array for a thread
    size_t idx;               // Thread index in TLS storage. This is not OS thread ID!
};


స్థిర తార్కిక g_isTlsStorageInitialized = అబద్ధం;

// Main TLS storage class
తరగతి TlsStorage
{
బహిరంగ:
    TlsStorage() :
        tls_Slots_కొలత(0)
    {
        (శూన్య)getTlsAbstraction();  // ensure singeton initialization (for correct order of atexit calls)
        tlsSlots.reserve(32);
        దారాలు.reserve(32);
        g_isTlsStorageInitialized = నిజం;
    }
    ~TlsStorage()
    {
        // TlsStorage object should not be released
        // There is no reliable way to avoid problems caused by static initialization order fiasco
        // Don't use logging here
        fprintf(stderr, "OpenCV FATAL: TlsStorage::~TlsStorage() call is not expected\n");
        fflush(stderr);
    }

    శూన్య releaseThread(శూన్య* tls_విలువ = ఖాళీ)
    {
        TlsAbstraction* tls = getTlsAbstraction();
        ఒకవేళ (ఖాళీ == tls)
            తిరిగి;  // TLS singleton is not available (terminated)
        Thread_దత్తాంశం *pTD = tls_విలువ == ఖాళీ ? (Thread_దత్తాంశం*)tls->get_దత్తాంశం() : (Thread_దత్తాంశం*)tls_విలువ;
        ఒకవేళ (pTD == ఖాళీ)
            తిరిగి;  // no OpenCV TLS data for this thread
        Auto_తాళం guard(mtxGlobalAccess);
        కొరకు (size_t i = 0; i < దారాలు.size(); i++)
        {
            ఒకవేళ (pTD == దారాలు[i])
            {
                దారాలు[i] = ఖాళీ;
                ఒకవేళ (tls_విలువ == ఖాళీ)
                    tls->సమితి_దత్తాంశం(0);
                std::vector<శూన్య*>& thread_slots = pTD->slots;
                కొరకు (size_t స్థానం_Idx = 0; స్థానం_Idx < thread_slots.size(); స్థానం_Idx++)
                {
                    శూన్య* p_దత్తాంశం = thread_slots[స్థానం_Idx];
                    thread_slots[స్థానం_Idx] = ఖాళీ;
                    ఒకవేళ (!p_దత్తాంశం)
                        కొనసాగు;
                    TLSDataContainer* container = tlsSlots[స్థానం_Idx].container;
                    ఒకవేళ (container)
                        container->delete_దత్తాంశం_Instance(p_దత్తాంశం);
                    లేకపోతే
                    {
                        fprintf(stderr, "OpenCV ERROR: TLS: container for slotIdx=%d is NULL. Can't release thread data\n", (పూర్ణాంకం)స్థానం_Idx);
                        fflush(stderr);
                    }
                }
                తొలగించు pTD;
                తిరిగి;
            }
        }
        fprintf(stderr, "OpenCV WARNING: TLS: Can't release thread TLS data (unknown pointer or data race): %p\n", (శూన్య*)pTD); fflush(stderr);
    }

    // Reserve TLS storage index
    size_t reserve_స్థానం(TLSDataContainer* container)
    {
        Auto_తాళం guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_కొలత == tlsSlots.size());

        // Find unused slots
        కొరకు(size_t స్థానం = 0; స్థానం < tls_Slots_కొలత; స్థానం++)
        {
            ఒకవేళ (tlsSlots[స్థానం].container == ఖాళీ)
            {
                tlsSlots[స్థానం].container = container;
                తిరిగి స్థానం;
            }
        }

        // Create new slot
        tlsSlots.push_back(Tls_స్థానం_Info(container)); tls_Slots_కొలత++;
        తిరిగి tls_Slots_కొలత - 1;
    }

    // Release TLS storage index and pass associated data to caller
    శూన్య release_స్థానం(size_t స్థానం_Idx, std::vector<శూన్య*> &దత్తాంశం_Vec, తార్కిక keep_స్థానం = అబద్ధం)
    {
        Auto_తాళం guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_కొలత == tlsSlots.size());
        CV_Assert(tls_Slots_కొలత > స్థానం_Idx);

        కొరకు(size_t i = 0; i < దారాలు.size(); i++)
        {
            ఒకవేళ(దారాలు[i])
            {
                std::vector<శూన్య*>& thread_slots = దారాలు[i]->slots;
                ఒకవేళ (thread_slots.size() > స్థానం_Idx && thread_slots[స్థానం_Idx])
                {
                    దత్తాంశం_Vec.push_back(thread_slots[స్థానం_Idx]);
                    thread_slots[స్థానం_Idx] = ఖాళీ;
                }
            }
        }

        ఒకవేళ (!keep_స్థానం)
        {
            tlsSlots[స్థానం_Idx].container = ఖాళీ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    శూన్య* get_దత్తాంశం(size_t స్థానం_Idx) స్థిరరాశి
    {
#నిర్వచితం_కాకపోతే CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_కొలత > స్థానం_Idx);
#అయితే_ముగింపు

        TlsAbstraction* tls = getTlsAbstraction();
        ఒకవేళ (ఖాళీ == tls)
            తిరిగి ఖాళీ;  // TLS singleton is not available (terminated)

        Thread_దత్తాంశం* thread_దత్తాంశం = (Thread_దత్తాంశం*)tls->get_దత్తాంశం();
        ఒకవేళ(thread_దత్తాంశం && thread_దత్తాంశం->slots.size() > స్థానం_Idx)
            తిరిగి thread_దత్తాంశం->slots[స్థానం_Idx];

        తిరిగి ఖాళీ;
    }

    // Gather data from threads by TLS storage index
    శూన్య gather(size_t స్థానం_Idx, std::vector<శూన్య*> &దత్తాంశం_Vec)
    {
        Auto_తాళం guard(mtxGlobalAccess);
        CV_Assert(tls_Slots_కొలత == tlsSlots.size());
        CV_Assert(tls_Slots_కొలత > స్థానం_Idx);

        కొరకు(size_t i = 0; i < దారాలు.size(); i++)
        {
            ఒకవేళ(దారాలు[i])
            {
                std::vector<శూన్య*>& thread_slots = దారాలు[i]->slots;
                ఒకవేళ (thread_slots.size() > స్థానం_Idx && thread_slots[స్థానం_Idx])
                    దత్తాంశం_Vec.push_back(thread_slots[స్థానం_Idx]);
            }
        }
    }

    // Set data to storage index
    శూన్య సమితి_దత్తాంశం(size_t స్థానం_Idx, శూన్య* p_దత్తాంశం)
    {
#నిర్వచితం_కాకపోతే CV_THREAD_SANITIZER
        CV_Assert(tls_Slots_కొలత > స్థానం_Idx);
#అయితే_ముగింపు

        TlsAbstraction* tls = getTlsAbstraction();
        ఒకవేళ (ఖాళీ == tls)
            తిరిగి;  // TLS singleton is not available (terminated)

        Thread_దత్తాంశం* thread_దత్తాంశం = (Thread_దత్తాంశం*)tls->get_దత్తాంశం();
        ఒకవేళ(!thread_దత్తాంశం)
        {
            thread_దత్తాంశం = కొత్త Thread_దత్తాంశం;
            tls->సమితి_దత్తాంశం((శూన్య*)thread_దత్తాంశం);
            {
                Auto_తాళం guard(mtxGlobalAccess);

                తార్కిక దొరికింది = అబద్ధం;
                // Find unused slots
                కొరకు(size_t స్థానం = 0; స్థానం < దారాలు.size(); స్థానం++)
                {
                    ఒకవేళ (దారాలు[స్థానం] == ఖాళీ)
                    {
                        thread_దత్తాంశం->idx = (పూర్ణాంకం)స్థానం;
                        దారాలు[స్థానం] = thread_దత్తాంశం;
                        దొరికింది = నిజం;
                        ఆపు;
                    }
                }

                ఒకవేళ (!దొరికింది)
                {
                    // Create new slot
                    thread_దత్తాంశం->idx = దారాలు.size();
                    దారాలు.push_back(thread_దత్తాంశం);
                }
            }
        }

        ఒకవేళ(స్థానం_Idx >= thread_దత్తాంశం->slots.size())
        {
            Auto_తాళం guard(mtxGlobalAccess); // keep synchronization with gather() calls
            thread_దత్తాంశం->slots.resize(స్థానం_Idx + 1, ఖాళీ);
        }
        thread_దత్తాంశం->slots[స్థానం_Idx] = p_దత్తాంశం;
    }

అంతరంగ:
    Mutex  mtxGlobalAccess;           // Shared objects operation guard
    size_t tls_Slots_కొలత;              // equal to tlsSlots.size() in synchronized sections
                                      // without synchronization this counter doesn't decrease - it is used for slotIdx sanity checks

    నిర్మాణం Tls_స్థానం_Info
    {
        Tls_స్థానం_Info(TLSDataContainer* _container) : container(_container) {}
        TLSDataContainer* container;  // attached container (to dispose data of terminated threads)
    };
    std::vector<నిర్మాణం Tls_స్థానం_Info> tlsSlots;  // TLS keys state
    std::vector<Thread_దత్తాంశం*> దారాలు; // Array for all allocated data. Thread data pointers are placed here to allow data cleanup
};

// Create global TLS storage object
స్థిర TlsStorage &getTlsStorage()
{
    CV_SINGLETON_LAZY_INIT_REF(TlsStorage, కొత్త TlsStorage())
}

#నిర్వచితం_కాకపోతే _WIN32  // pthread key destructor
స్థిర శూన్య opencv_tls_destructor(శూన్య* p_దత్తాంశం)
{
    ఒకవేళ (!g_isTlsStorageInitialized)
        తిరిగి;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_దత్తాంశం);
}
#లేకపోతే // _WIN32
#నిర్వచితమైతే CV_USE_FLS
స్థిర శూన్య WINAPI opencv_fls_destructor(శూన్య* p_దత్తాంశం)
{
    // Empiric detection of ExitProcess call
    DWORD సంకేతం_2 = STILL_చురుకైన/*259*/;
    BOOL res = Get_Exit_సంకేతం_Process(GetCurrentProcess(), &సంకేతం_2);
    ఒకవేళ (res && సంకేతం_2 != STILL_చురుకైన)
    {
        // Looks like we are in ExitProcess() call
        // This is FLS specific only because their callback is called before DllMain.
        // TLS doesn't have similar problem, DllMain() is called first which mark __termination properly.
        // Note: this workaround conflicts with ExitProcess() steps order described in documentation, however it works:
        // 3. ... called with DLL_PROCESS_DETACH
        // 7. The termination status of the process changes from STILL_ACTIVE to the exit value of the process.
        // (ref: https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-exitprocess)
        cv::__termination = నిజం;
    }

    ఒకవేళ (!g_isTlsStorageInitialized)
        తిరిగి;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread(p_దత్తాంశం);
}
#అయితే_ముగింపు // CV_USE_FLS
#అయితే_ముగింపు // _WIN32

స్థిర TlsStorage* స్థిరరాశి g_force_initialization_యొక్క_Tls_Storage
#ఒకవేళ defined __GNUC__
    __attribute__((వాడని))
#అయితే_ముగింపు
    = &getTlsStorage();


#లేకపోతే  // OPENCV_DISABLE_THREAD_SUPPORT

// no threading (OPENCV_DISABLE_THREAD_SUPPORT=ON)
తరగతి TlsStorage
{
బహిరంగ:
    TlsStorage()
    {
        slots.reserve(32);
    }
    ~TlsStorage()
    {
        కొరకు (size_t స్థానం_Idx = 0; స్థానం_Idx < slots.size(); స్థానం_Idx++)
        {
            స్థానం_Info& s = slots[స్థానం_Idx];
            TLSDataContainer* container = s.container;
            ఒకవేళ (container && s.దత్తాంశం)
            {
                container->delete_దత్తాంశం_Instance(s.దత్తాంశం);  // Can't use from SlotInfo destructor
                s.దత్తాంశం = శూన్యసూచిక;
            }
        }
    }

    // Reserve TLS storage index
    size_t reserve_స్థానం(TLSDataContainer* container)
    {
        size_t slots_కొలత = slots.size();
        కొరకు (size_t స్థానం = 0; స్థానం < slots_కొలత; స్థానం++)
        {
            స్థానం_Info& s = slots[స్థానం];
            ఒకవేళ (s.container == ఖాళీ)
            {
                CV_Assert(!s.దత్తాంశం);
                s.container = container;
                తిరిగి స్థానం;
            }
        }

        // create new slot
        slots.push_back(స్థానం_Info(container));
        తిరిగి slots_కొలత;
    }

    // Release TLS storage index and pass associated data to caller
    శూన్య release_స్థానం(size_t స్థానం_Idx, std::vector<శూన్య*> &దత్తాంశం_Vec, తార్కిక keep_స్థానం = అబద్ధం)
    {
        CV_Assert(స్థానం_Idx < slots.size());
        స్థానం_Info& s = slots[స్థానం_Idx];
        శూన్య* దత్తాంశం = s.దత్తాంశం;
        ఒకవేళ (దత్తాంశం)
        {
            దత్తాంశం_Vec.push_back(దత్తాంశం);
            s.దత్తాంశం = శూన్యసూచిక;
        }
        ఒకవేళ (!keep_స్థానం)
        {
            s.container = ఖాళీ;  // mark slot as free (see reserveSlot() implementation)
        }
    }

    // Get data by TLS storage index
    శూన్య* get_దత్తాంశం(size_t స్థానం_Idx) స్థిరరాశి
    {
        CV_Assert(స్థానం_Idx < slots.size());
        స్థిరరాశి స్థానం_Info& s = slots[స్థానం_Idx];
        తిరిగి s.దత్తాంశం;
    }

    // Gather data from threads by TLS storage index
    శూన్య gather(size_t స్థానం_Idx, std::vector<శూన్య*> &దత్తాంశం_Vec)
    {
        CV_Assert(స్థానం_Idx < slots.size());
        స్థానం_Info& s = slots[స్థానం_Idx];
        శూన్య* దత్తాంశం = s.దత్తాంశం;
        ఒకవేళ (దత్తాంశం)
            దత్తాంశం_Vec.push_back(దత్తాంశం);
        తిరిగి;
    }

    // Set data to storage index
    శూన్య సమితి_దత్తాంశం(size_t స్థానం_Idx, శూన్య* p_దత్తాంశం)
    {
        CV_Assert(స్థానం_Idx < slots.size());
        స్థానం_Info& s = slots[స్థానం_Idx];
        s.దత్తాంశం = p_దత్తాంశం;
    }

అంతరంగ:
    నిర్మాణం స్థానం_Info
    {
        స్థానం_Info(TLSDataContainer* _container) : container(_container), దత్తాంశం(శూన్యసూచిక) {}
        TLSDataContainer* container;  // attached container (to dispose data)
        శూన్య* దత్తాంశం;
    };
    std::vector<నిర్మాణం స్థానం_Info> slots;
};

స్థిర TlsStorage& getTlsStorage()
{
    స్థిర TlsStorage g_storage;  // no threading
    తిరిగి g_storage;
}

#అయితే_ముగింపు  // OPENCV_DISABLE_THREAD_SUPPORT

} // namespace details
ఉపయోగించి నామాకాశ details;

శూన్య releaseTlsStorageThread()
{
#నిర్వచితం_కాకపోతే OPENCV_DISABLE_THREAD_SUPPORT
    ఒకవేళ (!g_isTlsStorageInitialized)
        తిరిగి;  // nothing to release, so prefer to avoid creation of new global structures
    getTlsStorage().releaseThread();
#అయితే_ముగింపు
}

TLSDataContainer::TLSDataContainer()
{
    కీలకం_2 = (పూర్ణాంకం)getTlsStorage().reserve_స్థానం(ఇది); // Reserve key from TLS storage
}

TLSDataContainer::~TLSDataContainer()
{
    CV_Assert(కీలకం_2 == -1); // Key must be released in child object
}

శూన్య TLSDataContainer::gather_దత్తాంశం(std::vector<శూన్య*> &దత్తాంశం) స్థిరరాశి
{
    getTlsStorage().gather(కీలకం_2, దత్తాంశం);
}

శూన్య TLSDataContainer::detach_దత్తాంశం(std::vector<శూన్య*> &దత్తాంశం)
{
    getTlsStorage().release_స్థానం(కీలకం_2, దత్తాంశం, నిజం);
}

శూన్య TLSDataContainer::release()
{
    ఒకవేళ (కీలకం_2 == -1)
        తిరిగి;  // already released
    std::vector<శూన్య*> దత్తాంశం; దత్తాంశం.reserve(32);
    getTlsStorage().release_స్థానం(కీలకం_2, దత్తాంశం, అబద్ధం); // Release key and get stored data for proper destruction
    కీలకం_2 = -1;
    కొరకు(size_t i = 0; i < దత్తాంశం.size(); i++)  // Delete all associated data
        delete_దత్తాంశం_Instance(దత్తాంశం[i]);
}

శూన్య TLSDataContainer::cleanup()
{
    std::vector<శూన్య*> దత్తాంశం; దత్తాంశం.reserve(32);
    getTlsStorage().release_స్థానం(కీలకం_2, దత్తాంశం, నిజం); // Extract stored data with removal from TLS tables
    కొరకు(size_t i = 0; i < దత్తాంశం.size(); i++)  // Delete all associated data
        delete_దత్తాంశం_Instance(దత్తాంశం[i]);
}

శూన్య* TLSDataContainer::get_దత్తాంశం() స్థిరరాశి
{
    CV_Assert(కీలకం_2 != -1 && "Can't fetch data from terminated TLS container.");
    శూన్య* p_దత్తాంశం = getTlsStorage().get_దత్తాంశం(కీలకం_2); // Check if data was already allocated
    ఒకవేళ(!p_దత్తాంశం)
    {
        // Create new data instance and save it to TLS storage
        p_దత్తాంశం = create_దత్తాంశం_Instance();
        ప్రయత్నించు
        {
            getTlsStorage().సమితి_దత్తాంశం(కీలకం_2, p_దత్తాంశం);
        }
        పట్టుకో (...)
        {
            delete_దత్తాంశం_Instance(p_దత్తాంశం);
            విసురు;
        }
    }
    తిరిగి p_దత్తాంశం;
}

స్థిర TLSData<CoreTLSData>& get_Core_Tls_దత్తాంశం_TLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<CoreTLSData>, కొత్త TLSData<CoreTLSData>())
}

CoreTLSData& get_Core_Tls_దత్తాంశం()
{
    తిరిగి get_Core_Tls_దత్తాంశం_TLS().getRef();
}

#ఒకవేళ defined CVAPI_EXPORTS && defined _WIN32 && !defined WINCE
#నిర్వచితమైతే WINRT
    #సూచన warning(disable:4447) // Disable warning 'main' signature found without threading model
#అయితే_ముగింపు

బాహ్య "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved);

బాహ్య "C"
BOOL WINAPI DllMain(HINSTANCE, DWORD fdwReason, LPVOID lpReserved)
{
    ఒకవేళ (fdwReason == DLL_THREAD_DETACH || fdwReason == DLL_PROCESS_DETACH)
    {
        ఒకవేళ (lpReserved != ఖాళీ) // called after ExitProcess() call
        {
            cv::__termination = నిజం;
        }
        లేకపోతే
        {
            // Not allowed to free resources if lpReserved is non-null
            // http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583.aspx
            releaseTlsStorageThread();
        }
    }
    తిరిగి TRUE;
}
#అయితే_ముగింపు


నామాకాశ {

#నిర్వచితమైతే OPENCV_WITH_ITT
తార్కిక overrideThreadName()
{
    స్థిర తార్కిక param = utils::getConfigurationParameterBool("OPENCV_TRACE_ITT_SET_THREAD_NAME", అబద్ధం);
    తిరిగి param;
}
#అయితే_ముగింపు

స్థిర పూర్ణాంకం g_thread_అంకె = 0;
తరగతి ThreadID {
బహిరంగ:
    స్థిరరాశి పూర్ణాంకం id;
    ThreadID() :
        id(CV_XADD(&g_thread_అంకె, 1))
    {
#నిర్వచితమైతే OPENCV_WITH_ITT
        ఒకవేళ (overrideThreadName())
            __itt_thread_set_name(cv::format("OpenCVThread-%03d", id).c_str());
#అయితే_ముగింపు
    }
};

స్థిర TLSData<ThreadID>& getThreadIDTLS()
{
    CV_SINGLETON_LAZY_INIT_REF(TLSData<ThreadID>, కొత్త TLSData<ThreadID>());
}

} // namespace
పూర్ణాంకం utils::getThreadID() { తిరిగి getThreadIDTLS().get()->id; }


తరగతి ParseError
{
    std::string bad_విలువ;
బహిరంగ:
    ParseError(స్థిరరాశి std::string &bad_విలువ_2) :bad_విలువ(bad_విలువ_2) {}
    std::string గమ్యం_String(స్థిరరాశి std::string &param) స్థిరరాశి
    {
        std::ostringstream out;
        out << "Invalid value for parameter " << param << ": " << bad_విలువ;
        తిరిగి out.str();
    }
};

మూస <రకం_పేరు T>
T parseOption(స్థిరరాశి std::string &);

మూస<>
అంతఃవరుస తార్కిక parseOption(స్థిరరాశి std::string & value)
{
    ఒకవేళ (value == "1" || value == "True" || value == "true" || value == "TRUE")
    {
        తిరిగి నిజం;
    }
    ఒకవేళ (value == "0" || value == "False" || value == "false" || value == "FALSE")
    {
        తిరిగి అబద్ధం;
    }
    విసురు ParseError(value);
}

మూస<>
అంతఃవరుస size_t parseOption(స్థిరరాశి std::string &value)
{
    size_t pos = 0;
    కొరకు (; pos < value.size(); pos++)
    {
        ఒకవేళ (!అంకెనా(value[pos]))
            ఆపు;
    }
    cv::String విలువ_Str = value.substr(0, pos);
    cv::String suffixStr = value.substr(pos, value.length() - pos);
    size_t v = (size_t)std::stoull(విలువ_Str);
    ఒకవేళ (suffixStr.length() == 0)
        తిరిగి v;
    లేకపోతే ఒకవేళ (suffixStr == "MB" || suffixStr == "Mb" || suffixStr == "mb")
        తిరిగి v * 1024 * 1024;
    లేకపోతే ఒకవేళ (suffixStr == "KB" || suffixStr == "Kb" || suffixStr == "kb")
        తిరిగి v * 1024;
    విసురు ParseError(value);
}

మూస<>
అంతఃవరుస cv::String parseOption(స్థిరరాశి std::string &value)
{
    తిరిగి value;
}

మూస<>
అంతఃవరుస utils::Paths parseOption(స్థిరరాశి std::string &value)
{
    utils::Paths ఫలితం;
#నిర్వచితమైతే _WIN32
    స్థిరరాశి అక్షరం sep = ';';
#లేకపోతే
    స్థిరరాశి అక్షరం sep = ':';
#అయితే_ముగింపు
    size_t ప్రారంభం_pos = 0;
    అయితే (ప్రారంభం_pos != std::string::npos)
    {
        స్థిరరాశి size_t pos = value.find(sep, ప్రారంభం_pos);
        స్థిరరాశి std::string one_piece(value, ప్రారంభం_pos, pos == std::string::npos ? pos : pos - ప్రారంభం_pos);
        ఒకవేళ (!one_piece.empty())
            ఫలితం.push_back(one_piece);
        ప్రారంభం_pos = pos == std::string::npos ? pos : pos + 1;
    }
    తిరిగి ఫలితం;
}

స్థిర అంతఃవరుస స్థిరరాశి అక్షరం * envRead(స్థిరరాశి అక్షరం * name)
{
#నిర్వచితమైతే NO_GETENV
    CV_వాడని(name);
    తిరిగి ఖాళీ;
#లేకపోతే
    తిరిగి getenv(name);
#అయితే_ముగింపు
}

మూస<రకం_పేరు T>
అంతఃవరుస T పఠ(స్థిరరాశి std::string & k, స్థిరరాశి T & default_విలువ_2)
{
    ప్రయత్నించు
    {
        స్థిరరాశి అక్షరం * res = envRead(k.c_str());
        ఒకవేళ (res)
            తిరిగి parseOption<T>(std::string(res));
    }
    పట్టుకో (స్థిరరాశి ParseError &err)
    {
        CV_Error(cv::Error::StsBadArg, err.గమ్యం_String(k));
    }
    తిరిగి default_విలువ_2;
}

తార్కిక utils::getConfigurationParameterBool(స్థిరరాశి అక్షరం* name, తార్కిక default_విలువ_2)
{
    తిరిగి పఠ<తార్కిక>(name, default_విలువ_2);
}

size_t utils::get_Configuration_Parameter_కొలత_T(స్థిరరాశి అక్షరం* name, size_t default_విలువ_2)
{
    తిరిగి పఠ<size_t>(name, default_విలువ_2);
}

std::string utils::getConfigurationParameterString(స్థిరరాశి అక్షరం* name, స్థిరరాశి std::string & default_విలువ_2)
{
    తిరిగి పఠ<cv::String>(name, default_విలువ_2);
}

utils::Paths utils::getConfigurationParameterPaths(స్థిరరాశి అక్షరం* name, స్థిరరాశి utils::Paths &default_విలువ_2)
{
    తిరిగి పఠ<utils::Paths>(name, default_విలువ_2);
}


#నిర్వచితమైతే CV_COLLECT_IMPL_దత్తాంశం
ImplCollector& get_Impl_దత్తాంశం()
{
    CV_SINGLETON_LAZY_INIT_REF(ImplCollector, కొత్త ImplCollector())
}

శూన్య సమితి_Impl(పూర్ణాంకం flags)
{
    cv::Auto_తాళం lock(get_Impl_దత్తాంశం().mutex);

    get_Impl_దత్తాంశం().implFlags = flags;
    get_Impl_దత్తాంశం().impl_సంకేతం.clear();
    get_Impl_దత్తాంశం().implFun.clear();
}

శూన్య కూడు_Impl(పూర్ణాంకం flag, స్థిరరాశి అక్షరం* func)
{
    cv::Auto_తాళం lock(get_Impl_దత్తాంశం().mutex);

    get_Impl_దత్తాంశం().implFlags |= flag;
    ఒకవేళ(func) // use lazy collection if name was not specified
    {
        size_t సూచీ = get_Impl_దత్తాంశం().impl_సంకేతం.size();
        ఒకవేళ(!సూచీ || (get_Impl_దత్తాంశం().impl_సంకేతం[సూచీ-1] != flag || get_Impl_దత్తాంశం().implFun[సూచీ-1].compare(func))) // avoid duplicates
        {
            get_Impl_దత్తాంశం().impl_సంకేతం.push_back(flag);
            get_Impl_దత్తాంశం().implFun.push_back(func);
        }
    }
}

పూర్ణాంకం getImpl(std::vector<పూర్ణాంకం> &impl, std::vector<String> &funName)
{
    cv::Auto_తాళం lock(get_Impl_దత్తాంశం().mutex);

    impl    = get_Impl_దత్తాంశం().impl_సంకేతం;
    funName = get_Impl_దత్తాంశం().implFun;
    తిరిగి get_Impl_దత్తాంశం().implFlags; // return actual flags for lazy collection
}

తార్కిక useCollection()
{
    తిరిగి get_Impl_దత్తాంశం().useCollection;
}

శూన్య సమితి_Use_Collection(తార్కిక flag)
{
    cv::Auto_తాళం lock(get_Impl_దత్తాంశం().mutex);

    get_Impl_దత్తాంశం().useCollection = flag;
}
#అయితే_ముగింపు

నామాకాశ instr
{
తార్కిక useInstrumentation()
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    తిరిగి getInstrumentStruct().useInstr;
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య సమితి_Use_Instrumentation(తార్కిక flag)
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    getInstrumentStruct().useInstr = flag;
#లేకపోతే
    CV_వాడని(flag);
#అయితే_ముగింపు
}

Instr_కణుపు* getTrace()
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    తిరిగి &getInstrumentStruct().మూలం_కణుపు_2;
#లేకపోతే
    తిరిగి ఖాళీ;
#అయితే_ముగింపు
}

శూన్య resetTrace()
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    getInstrumentStruct().మూలం_కణుపు_2.removeChilds();
    getInstrumentTLSStruct().p_Current_కణుపు = &getInstrumentStruct().మూలం_కణుపు_2;
#అయితే_ముగింపు
}

శూన్య సమితి_Flags(FLAGS బహుళకం_Flags)
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    getInstrumentStruct().flags = బహుళకం_Flags;
#లేకపోతే
    CV_వాడని(బహుళకం_Flags);
#అయితే_ముగింపు
}
FLAGS getFlags()
{
#నిర్వచితమైతే ENABLE_INSTRUMENTATION
    తిరిగి (FLAGS)getInstrumentStruct().flags;
#లేకపోతే
    తిరిగి (FLAGS)0;
#అయితే_ముగింపు
}

కణుపు_దత్తాంశం::కణుపు_దత్తాంశం(స్థిరరాశి అక్షరం* funName, స్థిరరాశి అక్షరం* fileName, పూర్ణాంకం పంక్తి_అంకె, శూన్య* ret_చిరునామా, తార్కిక alwaysExpand, cv::instr::TYPE instrType, cv::instr::IMPL implType)
{
    m_funName       = funName ? cv::String(funName) : cv::String();  // std::string doesn't accept NULL
    m_instrType     = instrType;
    m_implType      = implType;
    m_fileName      = fileName;
    m_పంక్తి_అంకె       = పంక్తి_అంకె;
    m_ret_చిరునామా    = ret_చిరునామా;
    m_alwaysExpand  = alwaysExpand;

    m_దారాలు    = 1;
    m_counter    = 0;
    m_ticks_మొత్తం = 0;

    m_funError  = అబద్ధం;
}
కణుపు_దత్తాంశం::కణుపు_దత్తాంశం(కణుపు_దత్తాంశం &ref)
{
    *ఇది = ref;
}
కణుపు_దత్తాంశం& కణుపు_దత్తాంశం::క్రియాకారకం=(స్థిరరాశి కణుపు_దత్తాంశం &కుడి)
{
    ఇది->m_funName      = కుడి.m_funName;
    ఇది->m_instrType    = కుడి.m_instrType;
    ఇది->m_implType     = కుడి.m_implType;
    ఇది->m_fileName     = కుడి.m_fileName;
    ఇది->m_పంక్తి_అంకె      = కుడి.m_పంక్తి_అంకె;
    ఇది->m_ret_చిరునామా   = కుడి.m_ret_చిరునామా;
    ఇది->m_alwaysExpand = కుడి.m_alwaysExpand;

    ఇది->m_దారాలు     = కుడి.m_దారాలు;
    ఇది->m_counter     = కుడి.m_counter;
    ఇది->m_ticks_మొత్తం  = కుడి.m_ticks_మొత్తం;

    ఇది->m_funError    = కుడి.m_funError;

    తిరిగి *ఇది;
}
కణుపు_దత్తాంశం::~కణుపు_దత్తాంశం()
{
}
తార్కిక క్రియాకారకం==(స్థిరరాశి కణుపు_దత్తాంశం& ఎడమ, స్థిరరాశి కణుపు_దత్తాంశం& కుడి)
{
    ఒకవేళ(ఎడమ.m_పంక్తి_అంకె == కుడి.m_పంక్తి_అంకె && ఎడమ.m_funName == కుడి.m_funName && ఎడమ.m_fileName == కుడి.m_fileName)
    {
        ఒకవేళ(ఎడమ.m_ret_చిరునామా == కుడి.m_ret_చిరునామా || !(cv::instr::getFlags()&cv::instr::FLAGS_EXPAND_SAME_పేర్లు || ఎడమ.m_alwaysExpand))
            తిరిగి నిజం;
    }
    తిరిగి అబద్ధం;
}

#నిర్వచితమైతే ENABLE_INSTRUMENTATION
InstrStruct& getInstrumentStruct()
{
    స్థిర InstrStruct instr;
    తిరిగి instr;
}

InstrTLSStruct& getInstrumentTLSStruct()
{
    తిరిగి *getInstrumentStruct().tlsStruct.get();
}

Instr_కణుపు* get_Current_కణుపు()
{
    తిరిగి getInstrumentTLSStruct().p_Current_కణుపు;
}

IntrumentationRegion::IntrumentationRegion(స్థిరరాశి అక్షరం* funName, స్థిరరాశి అక్షరం* fileName, పూర్ణాంకం పంక్తి_అంకె, శూన్య *ret_చిరునామా, తార్కిక alwaysExpand, TYPE instrType, IMPL implType)
{
    m_disabled    = అబద్ధం;
    m_regionTicks = 0;

    InstrStruct *pStruct = &getInstrumentStruct();
    ఒకవేళ(pStruct->useInstr)
    {
        InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

        // Disable in case of failure
        ఒకవేళ(!pTLS->p_Current_కణుపు)
        {
            m_disabled = నిజం;
            తిరిగి;
        }

        పూర్ణాంకం లోతు = pTLS->p_Current_కణుపు->get_లోతు();
        ఒకవేళ(pStruct->max_లోతు && pStruct->max_లోతు <= లోతు)
        {
            m_disabled = నిజం;
            తిరిగి;
        }

        కణుపు_దత్తాంశం payload(funName, fileName, పంక్తి_అంకె, ret_చిరునామా, alwaysExpand, instrType, implType);
        Node<కణుపు_దత్తాంశం>* p_బిడ్డ = ఖాళీ;

        ఒకవేళ(pStruct->flags&FLAGS_MAPPING)
        {
            // Critical section
            cv::Auto_తాళం guard(pStruct->mutexCreate); // Guard from concurrent child creation
            p_బిడ్డ = pTLS->p_Current_కణుపు->కనుగొను_బిడ్డ(payload);
            ఒకవేళ(!p_బిడ్డ)
            {
                p_బిడ్డ = కొత్త Node<కణుపు_దత్తాంశం>(payload);
                pTLS->p_Current_కణుపు->కూడు_బిడ్డ(p_బిడ్డ);
            }
        }
        లేకపోతే
        {
            p_బిడ్డ = pTLS->p_Current_కణుపు->కనుగొను_బిడ్డ(payload);
            ఒకవేళ(!p_బిడ్డ)
            {
                m_disabled = నిజం;
                తిరిగి;
            }
        }
        pTLS->p_Current_కణుపు = p_బిడ్డ;

        m_regionTicks = get_Tick_లెక్క();
    }
}

IntrumentationRegion::~IntrumentationRegion()
{
    InstrStruct *pStruct = &getInstrumentStruct();
    ఒకవేళ(pStruct->useInstr)
    {
        ఒకవేళ(!m_disabled)
        {
            InstrTLSStruct *pTLS = &getInstrumentTLSStruct();

            ఒకవేళ (pTLS->p_Current_కణుపు->m_payload.m_implType == cv::instr::IMPL_OPENCL &&
                (pTLS->p_Current_కణుపు->m_payload.m_instrType == cv::instr::TYPE_FUN ||
                    pTLS->p_Current_కణుపు->m_payload.m_instrType == cv::instr::TYPE_WRAPPER))
            {
                cv::ocl::ముగింపు_2(); // TODO Support "async" OpenCL instrumentation
            }

            uint64 ticks = (get_Tick_లెక్క() - m_regionTicks);
            {
                cv::Auto_తాళం guard(pStruct->mutex_లెక్క); // Concurrent ticks accumulation
                pTLS->p_Current_కణుపు->m_payload.m_counter++;
                pTLS->p_Current_కణుపు->m_payload.m_ticks_మొత్తం += ticks;
                pTLS->p_Current_కణుపు->m_payload.m_tls.get()->m_ticks_మొత్తం += ticks;
            }

            pTLS->p_Current_కణుపు = pTLS->p_Current_కణుపు->m_p_మాతృ;
        }
    }
}
#అయితే_ముగింపు
}

నామాకాశ ipp
{

#నిర్వచితమైతే HAVE_IPP
నిర్మాణం IPPInitSingleton
{
బహిరంగ:
    IPPInitSingleton()
    {
        useIPP         = నిజం;

#ఒకవేళ defined(OPENCV_ALGO_HINT_DEFAULT)
        useIPP_NE      = OPENCV_ALGO_HINT_DEFAULT == cv::ALGO_HINT_APPROX;
#లేకపోతే
        useIPP_NE      = అబద్ధం;
#అయితే_ముగింపు
        ipp_స్థితి      = 0;
        funcname       = ఖాళీ;
        filename       = ఖాళీ;
        linen          = 0;
        cpuFeatures    = 0;
        ippFeatures    = 0;
        ipp_శిఖరం_Features = 0;
        pIppLibInfo    = ఖాళీ;

        ipp_స్థితి = ippGetCpuFeatures(&cpuFeatures, ఖాళీ);
        ఒకవేళ(ipp_స్థితి < 0)
        {
            CV_LOG_ERROR(ఖాళీ, "ERROR: IPP cannot detect CPU features, IPP was disabled");
            useIPP = అబద్ధం;
            తిరిగి;
        }
        ippFeatures = cpuFeatures;

        std::string env = utils::getConfigurationParameterString("OPENCV_IPP");
        ఒకవేళ(!env.empty())
        {
#ఒకవేళ IPP_సంస్కరణ_X100 >= 201900
            స్థిరరాశి Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI|ippCPUID_AVX512_4FMADDPS|
                ippCPUID_AVX512_4VNNIW|ippCPUID_AVX512IFMA;
#లేదావేళ IPP_సంస్కరణ_X100 >= 201703
            స్థిరరాశి Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_MPX|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#లేదావేళ IPP_సంస్కరణ_X100 >= 201700
            స్థిరరాశి Ipp64u minorFeatures = ippCPUID_MOVBE|ippCPUID_AES|ippCPUID_CLMUL|ippCPUID_ABR|ippCPUID_RDRAND|ippCPUID_F16C|
                ippCPUID_ADCOX|ippCPUID_RDSEED|ippCPUID_PREFETCHW|ippCPUID_SHA|ippCPUID_AVX512CD|ippCPUID_AVX512ER|
                ippCPUID_AVX512PF|ippCPUID_AVX512BW|ippCPUID_AVX512DQ|ippCPUID_AVX512VL|ippCPUID_AVX512VBMI;
#లేకపోతే
            స్థిరరాశి Ipp64u minorFeatures = 0;
#అయితే_ముగింపు

            env = గమ్యం_క్రింది_Case(env);
            ఒకవేళ(env.substr(0, 2) == "ne")
            {
                useIPP_NE = నిజం;
                env = env.substr(3, env.size());
            }

            ఒకవేళ(env == "disabled")
            {
                CV_LOG_WARNING(ఖాళీ, "WARNING: IPP was disabled by OPENCV_IPP environment variable");
                useIPP = అబద్ధం;
            }
            లేకపోతే ఒకవేళ(env == "sse42")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42;
            లేకపోతే ఒకవేళ(env == "avx2")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2;
#ఒకవేళ IPP_సంస్కరణ_X100 >= 201700
#ఒకవేళ defined (_M_AMD64) || defined (__x86_64__)
            లేకపోతే ఒకవేళ(env == "avx512")
                ippFeatures = minorFeatures|ippCPUID_SSE2|ippCPUID_SSE3|ippCPUID_SSSE3|ippCPUID_SSE41|ippCPUID_SSE42|ippCPUID_AVX|ippCPUID_AVX2|ippCPUID_AVX512F;
#అయితే_ముగింపు
#అయితే_ముగింపు
            లేకపోతే
                CV_LOG_ERROR(ఖాళీ, "ERROR: Improper value of OPENCV_IPP: " << env.c_str() << ". Correct values are: disabled, sse42, avx2, avx512 (Intel64 only)");

            // Trim unsupported features
            ippFeatures &= cpuFeatures;
        }

        // Disable AVX1 since we don't track regressions for it. SSE42 will be used instead
        ఒకవేళ(cpuFeatures&ippCPUID_AVX && !(cpuFeatures&ippCPUID_AVX2))
            ippFeatures &= ~((Ipp64u)ippCPUID_AVX);

        // IPP integrations in OpenCV support only SSE4.2, AVX2 and AVX-512 optimizations.
        ఒకవేళ(!(
#ఒకవేళ IPP_సంస్కరణ_X100 >= 201700
            cpuFeatures&ippCPUID_AVX512F ||
#అయితే_ముగింపు
            cpuFeatures&ippCPUID_AVX2 ||
            cpuFeatures&ippCPUID_SSE42
            ))
        {
            useIPP = అబద్ధం;
            తిరిగి;
        }

        ఒకవేళ(ippFeatures == cpuFeatures)
            IPP_INITIALIZER(0)
        లేకపోతే
            IPP_INITIALIZER(ippFeatures)
        ippFeatures = ippGetEnabledCpuFeatures();

        // Detect top level optimizations to make comparison easier for optimizations dependent conditions
#ఒకవేళ IPP_సంస్కరణ_X100 >= 201700
        ఒకవేళ(ippFeatures&ippCPUID_AVX512F)
        {
            ఒకవేళ((ippFeatures&ippCPUID_AVX512_SKX) == ippCPUID_AVX512_SKX)
                ipp_శిఖరం_Features = ippCPUID_AVX512_SKX;
            లేకపోతే ఒకవేళ((ippFeatures&ippCPUID_AVX512_KNL) == ippCPUID_AVX512_KNL)
                ipp_శిఖరం_Features = ippCPUID_AVX512_KNL;
            లేకపోతే
                ipp_శిఖరం_Features = ippCPUID_AVX512F; // Unknown AVX512 configuration
        }
        లేకపోతే
#అయితే_ముగింపు
        ఒకవేళ(ippFeatures&ippCPUID_AVX2)
            ipp_శిఖరం_Features = ippCPUID_AVX2;
        లేకపోతే ఒకవేళ(ippFeatures&ippCPUID_SSE42)
            ipp_శిఖరం_Features = ippCPUID_SSE42;

        pIppLibInfo = ippi_Get_Lib_సంస్కరణ();

        // workaround: https://github.com/opencv/opencv/issues/12959
        std::string ippName(pIppLibInfo->Name ? pIppLibInfo->Name : "");
        ఒకవేళ (ippName.find("SSE4.2") != std::string::npos)
        {
            ipp_శిఖరం_Features = ippCPUID_SSE42;
        }
    }

బహిరంగ:
    తార్కిక        useIPP;
    తార్కిక        useIPP_NE;

    పూర్ణాంకం         ipp_స్థితి;  // 0 - all is ok, -1 - IPP functions failed
    స్థిరరాశి అక్షరం *funcname;
    స్థిరరాశి అక్షరం *filename;
    పూర్ణాంకం         linen;
    Ipp64u      ippFeatures;
    Ipp64u      cpuFeatures;
    Ipp64u      ipp_శిఖరం_Features;
    స్థిరరాశి Ipp_Library_సంస్కరణ *pIppLibInfo;
};

స్థిర IPPInitSingleton& getIPPSingleton()
{
    CV_SINGLETON_LAZY_INIT_REF(IPPInitSingleton, కొత్త IPPInitSingleton())
}
#అయితే_ముగింపు

గుర్తులేని దీర్ఘ దీర్ఘ getIppFeatures()
{
#నిర్వచితమైతే HAVE_IPP
    తిరిగి getIPPSingleton().ippFeatures;
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
}

#నిర్వచితమైతే HAVE_IPP
గుర్తులేని దీర్ఘ దీర్ఘ get_Ipp_శిఖరం_Features()
{
    తిరిగి getIPPSingleton().ipp_శిఖరం_Features;
}
#అయితే_ముగింపు

శూన్య సమితి_Ipp_స్థితి(పూర్ణాంకం స్థితి_2, స్థిరరాశి అక్షరం * స్థిరరాశి _funcname, స్థిరరాశి అక్షరం * స్థిరరాశి _filename, పూర్ణాంకం పంక్తి_3)
{
#నిర్వచితమైతే HAVE_IPP
    getIPPSingleton().ipp_స్థితి = స్థితి_2;
    getIPPSingleton().funcname = _funcname;
    getIPPSingleton().filename = _filename;
    getIPPSingleton().linen = పంక్తి_3;
#లేకపోతే
    CV_వాడని(స్థితి_2); CV_వాడని(_funcname); CV_వాడని(_filename); CV_వాడని(పంక్తి_3);
#అయితే_ముగింపు
}

పూర్ణాంకం get_Ipp_స్థితి()
{
#నిర్వచితమైతే HAVE_IPP
    తిరిగి getIPPSingleton().ipp_స్థితి;
#లేకపోతే
    తిరిగి 0;
#అయితే_ముగింపు
}

String getIppErrorLocation()
{
#నిర్వచితమైతే HAVE_IPP
    తిరిగి format("%s:%d %s", getIPPSingleton().filename ? getIPPSingleton().filename : "", getIPPSingleton().linen, getIPPSingleton().funcname ? getIPPSingleton().funcname : "");
#లేకపోతే
    తిరిగి String();
#అయితే_ముగింపు
}

String get_Ipp_సంస్కరణ()
{
#నిర్వచితమైతే HAVE_IPP
    స్థిరరాశి Ipp_Library_సంస్కరణ *pInfo = getIPPSingleton().pIppLibInfo;
    ఒకవేళ(pInfo)
        తిరిగి format("%s %s %s", pInfo->Name, pInfo->సంస్కరణ_4, pInfo->BuildDate);
    లేకపోతే
        తిరిగి String("error");
#లేకపోతే
    తిరిగి String("disabled");
#అయితే_ముగింపు
}

తార్కిక useIPP()
{
#నిర్వచితమైతే HAVE_IPP
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    ఒకవేళ (దత్తాంశం.useIPP < 0)
    {
        దత్తాంశం.useIPP = getIPPSingleton().useIPP;
    }
    తిరిగి (దత్తాంశం.useIPP > 0);
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య సమితి_Use_IPP(తార్కిక flag)
{
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
#నిర్వచితమైతే HAVE_IPP
    దత్తాంశం.useIPP = (getIPPSingleton().useIPP)?flag:అబద్ధం;
#లేకపోతే
    CV_వాడని(flag);
    దత్తాంశం.useIPP = అబద్ధం;
#అయితే_ముగింపు
}

తార్కిక useIPP_NotExact()
{
#నిర్వచితమైతే HAVE_IPP
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
    ఒకవేళ (దత్తాంశం.useIPP_NE < 0)
    {
        దత్తాంశం.useIPP_NE = getIPPSingleton().useIPP_NE;
    }
    తిరిగి (దత్తాంశం.useIPP_NE > 0);
#లేకపోతే
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

శూన్య సమితి_Use_IPP_Not_Exact(తార్కిక flag)
{
    CoreTLSData& దత్తాంశం = get_Core_Tls_దత్తాంశం();
#నిర్వచితమైతే HAVE_IPP
    దత్తాంశం.useIPP_NE = flag;
#లేకపోతే
    CV_వాడని(flag);
    దత్తాంశం.useIPP_NE = అబద్ధం;
#అయితే_ముగింపు
}

} // namespace ipp


నామాకాశ details {

#ఒకవేళ OPENCV_IMPL_FP_HINTS_X86
#నిర్వచితం_కాకపోతే _MM_DENORMALS_ZERO_ON  // requires pmmintrin.h (SSE3)
#నిర్వచించు _MM_DENORMALS_ZERO_ON 0x0040
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే MM_DENORMALS_ZERO_ముసుగు  // requires pmmintrin.h (SSE3)
#నిర్వచించు MM_DENORMALS_ZERO_ముసుగు 0x0040
#అయితే_ముగింపు
#అయితే_ముగింపు

శూన్య సమితి_FPDenormals_Ignore_Hint(తార్కిక ignore, CV_బహిర్గతం FPDenormals_బహుళకం_State& state)
{
#ఒకవేళ OPENCV_IMPL_FP_HINTS_X86
    గుర్తులేని ముసుగు = MM_FLUSH_ZERO_ముసుగు;
    గుర్తులేని value = ignore ? _MM_FLUSH_ZERO_ON : 0;
    ఒకవేళ (featuresEnabled.have[CPU_SSE3])
    {
        ముసుగు |= MM_DENORMALS_ZERO_ముసుగు;
        value |= ignore ? _MM_DENORMALS_ZERO_ON : 0;
    }
    స్థిరరాశి గుర్తులేని old_flags = _mm_getcsr();
    స్థిరరాశి గుర్తులేని old_విలువ = old_flags & ముసుగు;
    గుర్తులేని flags = (old_flags & ~ముసుగు) | value;
    CV_LOG_DEBUG(ఖాళీ, "core: update FP mxcsr flags = " << cv::format("0x%08x", flags));
    // save state
    state.reserved[0] = (uint32_t)ముసుగు;
    state.reserved[1] = (uint32_t)old_విలువ;
    _mm_setcsr(flags);
#లేకపోతే
    CV_వాడని(ignore); CV_వాడని(state);
#అయితే_ముగింపు
}

పూర్ణాంకం saveFPDenormalsState(CV_బహిర్గతం FPDenormals_బహుళకం_State& state)
{
#ఒకవేళ OPENCV_IMPL_FP_HINTS_X86
    గుర్తులేని ముసుగు = MM_FLUSH_ZERO_ముసుగు;
    ఒకవేళ (featuresEnabled.have[CPU_SSE3])
    {
        ముసుగు |= MM_DENORMALS_ZERO_ముసుగు;
    }
    స్థిరరాశి గుర్తులేని old_flags = _mm_getcsr();
    స్థిరరాశి గుర్తులేని old_విలువ = old_flags & ముసుగు;
    // save state
    state.reserved[0] = (uint32_t)ముసుగు;
    state.reserved[1] = (uint32_t)old_విలువ;
    తిరిగి 2;
#లేకపోతే
    CV_వాడని(state);
    తిరిగి 0;
#అయితే_ముగింపు
}

తార్కిక restoreFPDenormalsState(స్థిరరాశి FPDenormals_బహుళకం_State& state)
{
#ఒకవేళ OPENCV_IMPL_FP_HINTS_X86
    స్థిరరాశి గుర్తులేని ముసుగు = (గుర్తులేని)state.reserved[0];
    CV_DbgAssert(ముసుగు != 0); // invalid state (ensure that state is properly saved earlier)
    స్థిరరాశి గుర్తులేని value = (గుర్తులేని)state.reserved[1];
    CV_DbgCheck((పూర్ణాంకం)value, value == (value & ముసుగు), "invalid SSE FP state");
    స్థిరరాశి గుర్తులేని old_flags = _mm_getcsr();
    గుర్తులేని flags = (old_flags & ~ముసుగు) | value;
    CV_LOG_DEBUG(ఖాళీ, "core: restore FP mxcsr flags = " << cv::format("0x%08x", flags));
    _mm_setcsr(flags);
    తిరిగి నిజం;
#లేకపోతే
    CV_వాడని(state);
    తిరిగి అబద్ధం;
#అయితే_ముగింపు
}

}  // namespace details

AlgorithmHint getDefaultAlgorithmHint()
{
#నిర్వచితమైతే OPENCV_ALGO_HINT_DEFAULT
    తిరిగి OPENCV_ALGO_HINT_DEFAULT;
#లేకపోతే
    తిరిగి ALGO_HINT_ACCURATE;
#అయితే_ముగింపు
};

} // namespace cv

/* End of file. */
