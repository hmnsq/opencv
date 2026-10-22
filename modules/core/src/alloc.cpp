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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.defines.hpp>
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV_LOG_STRIP_LEVEL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/configuration.private.hpp>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ਨਿਰਾ, 0, "alloc.cpp: " << __VA_ARGS__)
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/allocator_stats.impl.hpp"
#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_POSIX_MEMALIGN
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਜਾਂਜੇ defined HAVE_MALLOC_H
#ਅਨ੍ਤਰ੍ਭਾਵ <malloc.h>
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_ALLOC_ENABLE_STATISTICS
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_ALLOC_STATISTICS_ਹੱਦਬੰਦੀ 4096  // don't track buffers less than N bytes
#ਅਨ੍ਤਰ੍ਭਾਵ <map>
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv {

ਸਥਿਰ ਖਾਲੀ* ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

ਸਥਿਰ cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    ਵਾਪਸ allocator_stats;
}

#ਜੇ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
ਸਥਿਰ ਤਾਰਕਿਕ read_ਸਮ੍ਰਿਤੀ_Alignment_Parameter()
{
    ਤਾਰਕਿਕ value = ਸੱਚ;
#ਜੇ defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_ਸਮ੍ਰਿਤੀ_SANITIZER) \
    && !defined(FUZZING_BUILD_ਬਹੁਲਕ_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = ਝੂਠ;
    }
#ਜੇ_ਅੰਤ
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    ਵਾਪਸ value;
}

#ਜੇ defined _MSC_VER
#ਹਦਾਇਤ warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
ਸਥਿਰ __forceinline
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
ਤਾਰਕਿਕ is_Aligned_ਵੰਡ_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    ਸਥਿਰ ਤਾਰਕਿਕ useMemalign = read_ਸਮ੍ਰਿਤੀ_Alignment_Parameter();
    ਵਾਪਸ useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
ਸਥਿਰ ਅਚਲ ਤਾਰਕਿਕ g_force_initialization_memalign_flag
#ਜੇ defined __GNUC__
    __attribute__((ਅਣਵਰਤਿਆ))
#ਜੇ_ਅੰਤ
    = is_Aligned_ਵੰਡ_Enabled();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_ALLOC_ENABLE_STATISTICS
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ* fastMalloc_(size_t size)
#ਨਹੀਂਤਾਂ
ਖਾਲੀ* fastMalloc(size_t size)
#ਜੇ_ਅੰਤ
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_POSIX_MEMALIGN
    ਜੇ (is_Aligned_ਵੰਡ_Enabled())
    {
        ਖਾਲੀ* ptr = ਨਿਰਾ;
        ਜੇ(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ਨਿਰਾ;
        ਜੇ(!ptr)
            ਵਾਪਸ ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ_Error(size);
        ਵਾਪਸ ptr;
    }
#ਜਾਂਜੇ defined HAVE_MEMALIGN
    ਜੇ (is_Aligned_ਵੰਡ_Enabled())
    {
        ਖਾਲੀ* ptr = memalign(CV_MALLOC_ALIGN, size);
        ਜੇ(!ptr)
            ਵਾਪਸ ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ_Error(size);
        ਵਾਪਸ ptr;
    }
#ਜਾਂਜੇ defined HAVE_WIN32_ALIGNED_MALLOC
    ਜੇ (is_Aligned_ਵੰਡ_Enabled())
    {
        ਖਾਲੀ* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        ਜੇ(!ptr)
            ਵਾਪਸ ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ_Error(size);
        ਵਾਪਸ ptr;
    }
#ਜੇ_ਅੰਤ
    uchar* udata = (uchar*)ਸਮ੍ਰਿਤੀ_ਦਿਓ(size + ਆਕਾਰ(ਖਾਲੀ*) + CV_MALLOC_ALIGN);
    ਜੇ(!udata)
        ਵਾਪਸ ਬਾਹਰਲਾ_ਦਾ_ਸਮ੍ਰਿਤੀ_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    ਵਾਪਸ adata;
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_ALLOC_ENABLE_STATISTICS
ਸਥਿਰ ਸਤਰਅੰਦਰ
ਖਾਲੀ fastFree_(ਖਾਲੀ* ptr)
#ਨਹੀਂਤਾਂ
ਖਾਲੀ fastFree(ਖਾਲੀ* ptr)
#ਜੇ_ਅੰਤ
{
#ਜੇ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    ਜੇ (is_Aligned_ਵੰਡ_Enabled())
    {
        ਮੁਕਤ(ptr);
        ਵਾਪਸ;
    }
#ਜਾਂਜੇ defined HAVE_WIN32_ALIGNED_MALLOC
    ਜੇ (is_Aligned_ਵੰਡ_Enabled())
    {
        _aligned_free(ptr);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    ਜੇ(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(ਆਕਾਰ(ਖਾਲੀ*)+CV_MALLOC_ALIGN));
        ਮੁਕਤ(udata);
    }
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV_ALLOC_ENABLE_STATISTICS

ਸਥਿਰ
Mutex& get_ਵੰਡ_Statistics_Mutex()
{
    ਸਥਿਰ Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    ਵਾਪਸ *p_alloc_mutex;
}

ਸਥਿਰ std::map<ਖਾਲੀ*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

ਖਾਲੀ* fastMalloc(size_t size)
{
    ਖਾਲੀ* res = fastMalloc_(size);
    ਜੇ (res && size >= OPENCV_ALLOC_STATISTICS_ਹੱਦਬੰਦੀ)
    {
        cv::Auto_ਤਾਲਾ lock(get_ਵੰਡ_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    ਵਾਪਸ res;
}

ਖਾਲੀ fastFree(ਖਾਲੀ* ptr)
{
    {
        cv::Auto_ਤਾਲਾ lock(get_ਵੰਡ_Statistics_Mutex());
        std::map<ਖਾਲੀ*, size_t>::iterator i = allocated_buffers.find(ptr);
        ਜੇ (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#ਜੇ_ਅੰਤ // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
