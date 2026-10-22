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

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/logger.defines.hpp>
#undef CV_LOG_STRIP_LEVEL
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>
#અન્તર્ભાવ <opencv2/core/utils/configuration.private.hpp>

#define CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ખાલી, 0, "alloc.cpp: " << __VA_ARGS__)
#અન્તર્ભાવ "opencv2/core/utils/allocator_stats.impl.hpp"
#undef CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ifdef HAVE_POSIX_MEMALIGN
#અન્તર્ભાવ <stdlib.h>
#અથવાજો defined HAVE_MALLOC_H
#અન્તર્ભાવ <malloc.h>
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
#define OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#અન્તર્ભાવ <map>
#endif

નામાકાશ cv {

સ્થિર શૂન્ય* OutOfMemoryError(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (unsigned long long)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

સ્થિર cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    પાછા allocator_stats;
}

#જો defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
સ્થિર તાર્કિક readMemoryAlignmentParameter()
{
    તાર્કિક value = સાચું;
#જો defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = ખોટું;
    }
#endif
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    પાછા value;
}

#જો defined _MSC_VER
#pragma warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
સ્થિર __forceinline
#નહીંતર
સ્થિર અંતઃરેખ
#endif
તાર્કિક isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    સ્થિર તાર્કિક useMemalign = readMemoryAlignmentParameter();
    પાછા useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
સ્થિર અચલ તાર્કિક g_force_initialization_memalign_flag
#જો defined __GNUC__
    __attribute__((unused))
#endif
    = isAlignedAllocationEnabled();
#endif

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
સ્થિર અંતઃરેખ
શૂન્ય* fastMalloc_(size_t size)
#નહીંતર
શૂન્ય* fastMalloc(size_t size)
#endif
{
#ifdef HAVE_POSIX_MEMALIGN
    જો (isAlignedAllocationEnabled())
    {
        શૂન્ય* ptr = ખાલી;
        જો(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ખાલી;
        જો(!ptr)
            પાછા OutOfMemoryError(size);
        પાછા ptr;
    }
#અથવાજો defined HAVE_MEMALIGN
    જો (isAlignedAllocationEnabled())
    {
        શૂન્ય* ptr = memalign(CV_MALLOC_ALIGN, size);
        જો(!ptr)
            પાછા OutOfMemoryError(size);
        પાછા ptr;
    }
#અથવાજો defined HAVE_WIN32_ALIGNED_MALLOC
    જો (isAlignedAllocationEnabled())
    {
        શૂન્ય* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        જો(!ptr)
            પાછા OutOfMemoryError(size);
        પાછા ptr;
    }
#endif
    uchar* udata = (uchar*)malloc(size + કદ(શૂન્ય*) + CV_MALLOC_ALIGN);
    જો(!udata)
        પાછા OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    પાછા adata;
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS
સ્થિર અંતઃરેખ
શૂન્ય fastFree_(શૂન્ય* ptr)
#નહીંતર
શૂન્ય fastFree(શૂન્ય* ptr)
#endif
{
#જો defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    જો (isAlignedAllocationEnabled())
    {
        મુક્ત(ptr);
        પાછા;
    }
#અથવાજો defined HAVE_WIN32_ALIGNED_MALLOC
    જો (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        પાછા;
    }
#endif
    જો(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(કદ(શૂન્ય*)+CV_MALLOC_ALIGN));
        મુક્ત(udata);
    }
}

#ifdef OPENCV_ALLOC_ENABLE_STATISTICS

સ્થિર
Mutex& getAllocationStatisticsMutex()
{
    સ્થિર Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    પાછા *p_alloc_mutex;
}

સ્થિર std::map<શૂન્ય*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

શૂન્ય* fastMalloc(size_t size)
{
    શૂન્ય* res = fastMalloc_(size);
    જો (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    પાછા res;
}

શૂન્ય fastFree(શૂન્ય* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<શૂન્ય*, size_t>::iterator i = allocated_buffers.find(ptr);
        જો (i != allocated_buffers.end())
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
