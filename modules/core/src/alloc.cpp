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

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#அந்தர்பாவ <opencv2/core/utils/logger.hpp>
#அந்தர்பாவ <opencv2/core/utils/configuration.private.hpp>

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(வெற்று, 0, "alloc.cpp: " << __VA_ARGS__)
#அந்தர்பாவ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ifdef HAVE_POSIX_MEMALIGN
#அந்தர்பாவ <stdlib.h>
#அல்லதெனில் defined HAVE_MALLOC_H
#அந்தர்பாவ <malloc.h>
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
#define OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#அந்தர்பாவ <map>
#endif

நாமாகாஶ cv {

நிலையான வெறுமை* OutOfMemoryError(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (unsigned long long)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

நிலையான cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    திரும்பு allocator_stats;
}

#எனில் defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
நிலையான தருக்க readMemoryAlignmentParameter()
{
    தருக்க value = உண்மை;
#எனில் defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = பொய்;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    திரும்பு value;
}

#எனில் defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
நிலையான __forceinline
#இல்லையெனில்
நிலையான உள்வரி
#endif
தருக்க isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    நிலையான தருக்க useMemalign = readMemoryAlignmentParameter();
    திரும்பு useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
நிலையான மாறிலி தருக்க g_force_initialization_memalign_flag
#எனில் defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
நிலையான உள்வரி
வெறுமை* fastMalloc_(size_t size)
#இல்லையெனில்
வெறுமை* fastMalloc(size_t size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    எனில் (isAlignedAllocationEnabled())
    {
        வெறுமை* ptr = வெற்று;
        எனில்(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = வெற்று;
        எனில்(!ptr)
            திரும்பு OutOfMemoryError(size);
        திரும்பு ptr;
    }
#அல்லதெனில் defined HAVE_MEMALIGN
    எனில் (isAlignedAllocationEnabled())
    {
        வெறுமை* ptr = memalign(CV_MALLOC_ALIGN, size);
        எனில்(!ptr)
            திரும்பு OutOfMemoryError(size);
        திரும்பு ptr;
    }
#அல்லதெனில் defined HAVE_WIN32_ALIGNED_MALLOC
    எனில் (isAlignedAllocationEnabled())
    {
        வெறுமை* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        எனில்(!ptr)
            திரும்பு OutOfMemoryError(size);
        திரும்பு ptr;
    }
#endif
    uchar* udata = (uchar*)malloc(size + அளவு(வெறுமை*) + CV_MALLOC_ALIGN);
    எனில்(!udata)
        திரும்பு OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    திரும்பு adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
நிலையான உள்வரி
வெறுமை fastFree_(வெறுமை* ptr)
#இல்லையெனில்
வெறுமை fastFree(வெறுமை* ptr)
#endif
{
#எனில் defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    எனில் (isAlignedAllocationEnabled())
    {
        விடுவி(ptr);
        திரும்பு;
    }
#அல்லதெனில் defined HAVE_WIN32_ALIGNED_MALLOC
    எனில் (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        திரும்பு;
    }
#endif
    எனில்(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(அளவு(வெறுமை*)+CV_MALLOC_ALIGN));
        விடுவி(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

நிலையான
Mutex& getAllocationStatisticsMutex()
{
    நிலையான Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    திரும்பு *p_alloc_mutex;
}

நிலையான std::map<வெறுமை*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

வெறுமை* fastMalloc(size_t size)
{
    வெறுமை* res = fastMalloc_(size);
    எனில் (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    திரும்பு res;
}

வெறுமை fastFree(வெறுமை* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<வெறுமை*, size_t>::iterator i = allocated_buffers.find(ptr);
        எனில் (i != allocated_buffers.end())
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
