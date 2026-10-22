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

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/logger.defines.hpp>
#تعریف_منسوخ CV_LOG_STRIP_LEVEL
#تعریف_کرو CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#شامل <opencv2/core/utils/logger.hpp>
#شامل <opencv2/core/utils/configuration.private.hpp>

#تعریف_کرو CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(تہی, 0, "alloc.cpp: " << __VA_ARGS__)
#شامل "opencv2/core/utils/allocator_stats.impl.hpp"
#تعریف_منسوخ CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#اگر_تعریف_شدہ HAVE_POSIX_MEMALIGN
#شامل <stdlib.h>
#وگرنہ_اگر defined HAVE_MALLOC_H
#شامل <malloc.h>
#اگر_ختم

#اگر_تعریف_شدہ OPENCV_ALLOC_ENABLE_STATISTICS
#تعریف_کرو OPENCV_ALLOC_STATISTICS_حد_بندی 4096  // don't track buffers less than N bytes
#شامل <map>
#اگر_ختم

نامخلا cv {

ساکن خالی* برآمد_کا_حافظہ_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (غیرعلامتی طویل طویل)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

ساکن cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    واپس allocator_stats;
}

#اگر defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
ساکن منطقی read_حافظہ_Alignment_Parameter()
{
    منطقی value = سچ;
#اگر defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_حافظہ_SANITIZER) \
    && !defined(FUZZING_BUILD_منوال_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = جھوٹ;
    }
#اگر_ختم
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    واپس value;
}

#اگر defined _MSC_VER
#ہدایت warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
ساکن __forceinline
#ورنہ
ساکن اندرونی
#اگر_ختم
منطقی is_Aligned_تخصیص_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    ساکن منطقی useMemalign = read_حافظہ_Alignment_Parameter();
    واپس useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
ساکن ثابت منطقی g_force_initialization_memalign_flag
#اگر defined __GNUC__
    __attribute__((غیر_مستعمل))
#اگر_ختم
    = is_Aligned_تخصیص_Enabled();
#اگر_ختم

#اگر_تعریف_شدہ OPENCV_ALLOC_ENABLE_STATISTICS
ساکن اندرونی
خالی* fastMalloc_(size_t size)
#ورنہ
خالی* fastMalloc(size_t size)
#اگر_ختم
{
#اگر_تعریف_شدہ HAVE_POSIX_MEMALIGN
    اگر (is_Aligned_تخصیص_Enabled())
    {
        خالی* ptr = تہی;
        اگر(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = تہی;
        اگر(!ptr)
            واپس برآمد_کا_حافظہ_Error(size);
        واپس ptr;
    }
#وگرنہ_اگر defined HAVE_MEMALIGN
    اگر (is_Aligned_تخصیص_Enabled())
    {
        خالی* ptr = memalign(CV_MALLOC_ALIGN, size);
        اگر(!ptr)
            واپس برآمد_کا_حافظہ_Error(size);
        واپس ptr;
    }
#وگرنہ_اگر defined HAVE_WIN32_ALIGNED_MALLOC
    اگر (is_Aligned_تخصیص_Enabled())
    {
        خالی* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        اگر(!ptr)
            واپس برآمد_کا_حافظہ_Error(size);
        واپس ptr;
    }
#اگر_ختم
    uchar* udata = (uchar*)حافظہ_دو(size + حجم(خالی*) + CV_MALLOC_ALIGN);
    اگر(!udata)
        واپس برآمد_کا_حافظہ_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    واپس adata;
}

#اگر_تعریف_شدہ OPENCV_ALLOC_ENABLE_STATISTICS
ساکن اندرونی
خالی fastFree_(خالی* ptr)
#ورنہ
خالی fastFree(خالی* ptr)
#اگر_ختم
{
#اگر defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    اگر (is_Aligned_تخصیص_Enabled())
    {
        آزاد(ptr);
        واپس;
    }
#وگرنہ_اگر defined HAVE_WIN32_ALIGNED_MALLOC
    اگر (is_Aligned_تخصیص_Enabled())
    {
        _aligned_free(ptr);
        واپس;
    }
#اگر_ختم
    اگر(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(حجم(خالی*)+CV_MALLOC_ALIGN));
        آزاد(udata);
    }
}

#اگر_تعریف_شدہ OPENCV_ALLOC_ENABLE_STATISTICS

ساکن
Mutex& get_تخصیص_Statistics_Mutex()
{
    ساکن Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    واپس *p_alloc_mutex;
}

ساکن std::map<خالی*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

خالی* fastMalloc(size_t size)
{
    خالی* res = fastMalloc_(size);
    اگر (res && size >= OPENCV_ALLOC_STATISTICS_حد_بندی)
    {
        cv::Auto_تالا lock(get_تخصیص_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    واپس res;
}

خالی fastFree(خالی* ptr)
{
    {
        cv::Auto_تالا lock(get_تخصیص_Statistics_Mutex());
        std::map<خالی*, size_t>::iterator i = allocated_buffers.find(ptr);
        اگر (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#اگر_ختم // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
