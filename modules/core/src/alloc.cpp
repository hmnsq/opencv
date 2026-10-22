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

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(रिक्त, 0, "alloc.cpp: " << __VA_ARGS__)
#अन्तर्भाव "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ifdef HAVE_POSIX_MEMALIGN
#अन्तर्भाव <stdlib.h>
#नोचेद्यदि defined HAVE_MALLOC_H
#अन्तर्भाव <malloc.h>
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
#define OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#अन्तर्भाव <map>
#endif

नामाकाश cv {

स्थिर शून्य* OutOfMemoryError(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (unsigned long long)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

स्थिर cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    पटिनिवत्त allocator_stats;
}

#सचे defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
स्थिर तार्किक readMemoryAlignmentParameter()
{
    तार्किक value = सच्च;
#सचे defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = मिच्छा;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    पटिनिवत्त value;
}

#सचे defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
स्थिर __forceinline
#अञ्ञथा
स्थिर अन्तःस्थ
#endif
तार्किक isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    स्थिर तार्किक useMemalign = readMemoryAlignmentParameter();
    पटिनिवत्त useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
स्थिर ध्रुव तार्किक g_force_initialization_memalign_flag
#सचे defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य* fastMalloc_(size_t size)
#अञ्ञथा
शून्य* fastMalloc(size_t size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    सचे (isAlignedAllocationEnabled())
    {
        शून्य* ptr = रिक्त;
        सचे(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = रिक्त;
        सचे(!ptr)
            पटिनिवत्त OutOfMemoryError(size);
        पटिनिवत्त ptr;
    }
#नोचेद्यदि defined HAVE_MEMALIGN
    सचे (isAlignedAllocationEnabled())
    {
        शून्य* ptr = memalign(CV_MALLOC_ALIGN, size);
        सचे(!ptr)
            पटिनिवत्त OutOfMemoryError(size);
        पटिनिवत्त ptr;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    सचे (isAlignedAllocationEnabled())
    {
        शून्य* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        सचे(!ptr)
            पटिनिवत्त OutOfMemoryError(size);
        पटिनिवत्त ptr;
    }
#endif
    uchar* udata = (uchar*)malloc(size + परिमाण(शून्य*) + CV_MALLOC_ALIGN);
    सचे(!udata)
        पटिनिवत्त OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    पटिनिवत्त adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य fastFree_(शून्य* ptr)
#अञ्ञथा
शून्य fastFree(शून्य* ptr)
#endif
{
#सचे defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    सचे (isAlignedAllocationEnabled())
    {
        मुक्त(ptr);
        पटिनिवत्त;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    सचे (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        पटिनिवत्त;
    }
#endif
    सचे(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(परिमाण(शून्य*)+CV_MALLOC_ALIGN));
        मुक्त(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

स्थिर
Mutex& getAllocationStatisticsMutex()
{
    स्थिर Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    पटिनिवत्त *p_alloc_mutex;
}

स्थिर std::map<शून्य*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

शून्य* fastMalloc(size_t size)
{
    शून्य* res = fastMalloc_(size);
    सचे (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    पटिनिवत्त res;
}

शून्य fastFree(शून्य* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<शून्य*, size_t>::iterator i = allocated_buffers.find(ptr);
        सचे (i != allocated_buffers.end())
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
