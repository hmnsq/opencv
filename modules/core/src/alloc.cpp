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

स्थिर शून्य* OutOfMemoryError(माप_प्रकार size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (अचिन्हित दीर्घ दीर्घ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

स्थिर cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    प्रत्यागम allocator_stats;
}

#यदि defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
स्थिर तार्किक readMemoryAlignmentParameter()
{
    तार्किक value = सत्य;
#यदि defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = असत्य;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    प्रत्यागम value;
}

#यदि defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
स्थिर __forceinline
#अन्यथा
स्थिर अन्तःस्थ
#endif
तार्किक isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    स्थिर तार्किक useMemalign = readMemoryAlignmentParameter();
    प्रत्यागम useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
स्थिर ध्रुव तार्किक g_force_initialization_memalign_flag
#यदि defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य* fastMalloc_(माप_प्रकार size)
#अन्यथा
शून्य* fastMalloc(माप_प्रकार size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    यदि (isAlignedAllocationEnabled())
    {
        शून्य* ptr = रिक्त;
        यदि(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = रिक्त;
        यदि(!ptr)
            प्रत्यागम OutOfMemoryError(size);
        प्रत्यागम ptr;
    }
#नोचेद्यदि defined HAVE_MEMALIGN
    यदि (isAlignedAllocationEnabled())
    {
        शून्य* ptr = memalign(CV_MALLOC_ALIGN, size);
        यदि(!ptr)
            प्रत्यागम OutOfMemoryError(size);
        प्रत्यागम ptr;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    यदि (isAlignedAllocationEnabled())
    {
        शून्य* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        यदि(!ptr)
            प्रत्यागम OutOfMemoryError(size);
        प्रत्यागम ptr;
    }
#endif
    uchar* udata = (uchar*)दो_स्मृति(size + परिमाण(शून्य*) + CV_MALLOC_ALIGN);
    यदि(!udata)
        प्रत्यागम OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    प्रत्यागम adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य fastFree_(शून्य* ptr)
#अन्यथा
शून्य fastFree(शून्य* ptr)
#endif
{
#यदि defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    यदि (isAlignedAllocationEnabled())
    {
        मुक्त(ptr);
        प्रत्यागम;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    यदि (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        प्रत्यागम;
    }
#endif
    यदि(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (सूचक_भेद_प्रकार)(परिमाण(शून्य*)+CV_MALLOC_ALIGN));
        मुक्त(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

स्थिर
Mutex& getAllocationStatisticsMutex()
{
    स्थिर Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    प्रत्यागम *p_alloc_mutex;
}

स्थिर std::map<शून्य*, माप_प्रकार> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

शून्य* fastMalloc(माप_प्रकार size)
{
    शून्य* res = fastMalloc_(size);
    यदि (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    प्रत्यागम res;
}

शून्य fastFree(शून्य* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<शून्य*, माप_प्रकार>::iterator i = allocated_buffers.find(ptr);
        यदि (i != allocated_buffers.end())
        {
            माप_प्रकार size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#endif // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
