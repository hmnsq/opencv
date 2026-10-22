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

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/configuration.private.hpp>

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ଖାଲି, 0, "alloc.cpp: " << __VA_ARGS__)
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ifdef HAVE_POSIX_MEMALIGN
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ନଚେତଯଦି defined HAVE_MALLOC_H
#ଅନ୍ତର୍ଭାଵ <malloc.h>
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
#define OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#ଅନ୍ତର୍ଭାଵ <map>
#endif

ନାମାକାଶ cv {

ସ୍ଥିର ଶୂନ୍ୟ* OutOfMemoryError(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (unsigned long long)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

ସ୍ଥିର cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    ଫେରନ୍ତୁ allocator_stats;
}

#ଯଦି defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
ସ୍ଥିର ତାର୍କିକ readMemoryAlignmentParameter()
{
    ତାର୍କିକ value = ସତ୍ୟ;
#ଯଦି defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = ମିଥ୍ୟା;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    ଫେରନ୍ତୁ value;
}

#ଯଦି defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
ସ୍ଥିର __forceinline
#ଅନ୍ୟଥା
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
#endif
ତାର୍କିକ isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    ସ୍ଥିର ତାର୍କିକ useMemalign = readMemoryAlignmentParameter();
    ଫେରନ୍ତୁ useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
ସ୍ଥିର ଅଚଳ ତାର୍କିକ g_force_initialization_memalign_flag
#ଯଦି defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ* fastMalloc_(size_t size)
#ଅନ୍ୟଥା
ଶୂନ୍ୟ* fastMalloc(size_t size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    ଯଦି (isAlignedAllocationEnabled())
    {
        ଶୂନ୍ୟ* ptr = ଖାଲି;
        ଯଦି(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ଖାଲି;
        ଯଦି(!ptr)
            ଫେରନ୍ତୁ OutOfMemoryError(size);
        ଫେରନ୍ତୁ ptr;
    }
#ନଚେତଯଦି defined HAVE_MEMALIGN
    ଯଦି (isAlignedAllocationEnabled())
    {
        ଶୂନ୍ୟ* ptr = memalign(CV_MALLOC_ALIGN, size);
        ଯଦି(!ptr)
            ଫେରନ୍ତୁ OutOfMemoryError(size);
        ଫେରନ୍ତୁ ptr;
    }
#ନଚେତଯଦି defined HAVE_WIN32_ALIGNED_MALLOC
    ଯଦି (isAlignedAllocationEnabled())
    {
        ଶୂନ୍ୟ* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        ଯଦି(!ptr)
            ଫେରନ୍ତୁ OutOfMemoryError(size);
        ଫେରନ୍ତୁ ptr;
    }
#endif
    uchar* udata = (uchar*)malloc(size + ଆକାର(ଶୂନ୍ୟ*) + CV_MALLOC_ALIGN);
    ଯଦି(!udata)
        ଫେରନ୍ତୁ OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    ଫେରନ୍ତୁ adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
ସ୍ଥିର ଧାଡ଼ିଭିତରେ
ଶୂନ୍ୟ fastFree_(ଶୂନ୍ୟ* ptr)
#ଅନ୍ୟଥା
ଶୂନ୍ୟ fastFree(ଶୂନ୍ୟ* ptr)
#endif
{
#ଯଦି defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    ଯଦି (isAlignedAllocationEnabled())
    {
        ମୁକ୍ତ(ptr);
        ଫେରନ୍ତୁ;
    }
#ନଚେତଯଦି defined HAVE_WIN32_ALIGNED_MALLOC
    ଯଦି (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        ଫେରନ୍ତୁ;
    }
#endif
    ଯଦି(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(ଆକାର(ଶୂନ୍ୟ*)+CV_MALLOC_ALIGN));
        ମୁକ୍ତ(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

ସ୍ଥିର
Mutex& getAllocationStatisticsMutex()
{
    ସ୍ଥିର Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    ଫେରନ୍ତୁ *p_alloc_mutex;
}

ସ୍ଥିର std::map<ଶୂନ୍ୟ*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

ଶୂନ୍ୟ* fastMalloc(size_t size)
{
    ଶୂନ୍ୟ* res = fastMalloc_(size);
    ଯଦି (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    ଫେରନ୍ତୁ res;
}

ଶୂନ୍ୟ fastFree(ଶୂନ୍ୟ* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<ଶୂନ୍ୟ*, size_t>::iterator i = allocated_buffers.find(ptr);
        ଯଦି (i != allocated_buffers.end())
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
