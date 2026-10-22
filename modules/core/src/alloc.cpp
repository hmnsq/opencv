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

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>
#അന്തര്ഭാവ <opencv2/core/utils/configuration.private.hpp>

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ശൂന്യ, 0, "alloc.cpp: " << __VA_ARGS__)
#അന്തര്ഭാവ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ifdef HAVE_POSIX_MEMALIGN
#അന്തര്ഭാവ <stdlib.h>
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_MALLOC_H
#അന്തര്ഭാവ <malloc.h>
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
#define OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#അന്തര്ഭാവ <map>
#endif

നാമാകാശ cv {

സ്ഥിതം ശൂന്യം* OutOfMemoryError(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (unsigned long long)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

സ്ഥിതം cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    തിരികെ allocator_stats;
}

#എങ്കിൽ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
സ്ഥിതം തർക്കം readMemoryAlignmentParameter()
{
    തർക്കം value = സത്യം;
#എങ്കിൽ defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = അസത്യം;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    തിരികെ value;
}

#എങ്കിൽ defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
സ്ഥിതം __forceinline
#അല്ലെങ്കിൽ
സ്ഥിതം വരിയിൽ
#endif
തർക്കം isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    സ്ഥിതം തർക്കം useMemalign = readMemoryAlignmentParameter();
    തിരികെ useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
സ്ഥിതം സ്ഥിരം തർക്കം g_force_initialization_memalign_flag
#എങ്കിൽ defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
സ്ഥിതം വരിയിൽ
ശൂന്യം* fastMalloc_(size_t size)
#അല്ലെങ്കിൽ
ശൂന്യം* fastMalloc(size_t size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    എങ്കിൽ (isAlignedAllocationEnabled())
    {
        ശൂന്യം* ptr = ശൂന്യ;
        എങ്കിൽ(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ശൂന്യ;
        എങ്കിൽ(!ptr)
            തിരികെ OutOfMemoryError(size);
        തിരികെ ptr;
    }
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_MEMALIGN
    എങ്കിൽ (isAlignedAllocationEnabled())
    {
        ശൂന്യം* ptr = memalign(CV_MALLOC_ALIGN, size);
        എങ്കിൽ(!ptr)
            തിരികെ OutOfMemoryError(size);
        തിരികെ ptr;
    }
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_WIN32_ALIGNED_MALLOC
    എങ്കിൽ (isAlignedAllocationEnabled())
    {
        ശൂന്യം* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        എങ്കിൽ(!ptr)
            തിരികെ OutOfMemoryError(size);
        തിരികെ ptr;
    }
#endif
    uchar* udata = (uchar*)malloc(size + വലുപ്പം(ശൂന്യം*) + CV_MALLOC_ALIGN);
    എങ്കിൽ(!udata)
        തിരികെ OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    തിരികെ adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
സ്ഥിതം വരിയിൽ
ശൂന്യം fastFree_(ശൂന്യം* ptr)
#അല്ലെങ്കിൽ
ശൂന്യം fastFree(ശൂന്യം* ptr)
#endif
{
#എങ്കിൽ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    എങ്കിൽ (isAlignedAllocationEnabled())
    {
        സ്വതന്ത്രം(ptr);
        തിരികെ;
    }
#അല്ലെങ്കിലെങ്കിൽ defined HAVE_WIN32_ALIGNED_MALLOC
    എങ്കിൽ (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        തിരികെ;
    }
#endif
    എങ്കിൽ(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(വലുപ്പം(ശൂന്യം*)+CV_MALLOC_ALIGN));
        സ്വതന്ത്രം(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

സ്ഥിതം
Mutex& getAllocationStatisticsMutex()
{
    സ്ഥിതം Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    തിരികെ *p_alloc_mutex;
}

സ്ഥിതം std::map<ശൂന്യം*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

ശൂന്യം* fastMalloc(size_t size)
{
    ശൂന്യം* res = fastMalloc_(size);
    എങ്കിൽ (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    തിരികെ res;
}

ശൂന്യം fastFree(ശൂന്യം* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<ശൂന്യം*, size_t>::iterator i = allocated_buffers.find(ptr);
        എങ്കിൽ (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#endif // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
