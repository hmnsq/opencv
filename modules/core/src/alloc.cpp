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

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/logger.defines.hpp>
#تعریف_لغوه CV_LOG_STRIP_LEVEL
#تعریف_کړه CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ضمن <opencv2/core/utils/logger.hpp>
#ضمن <opencv2/core/utils/configuration.private.hpp>

#تعریف_کړه CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(عدم, 0, "alloc.cpp: " << __VA_ARGS__)
#ضمن "opencv2/core/utils/allocator_stats.impl.hpp"
#تعریف_لغوه CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#که_تعریف_شوی HAVE_POSIX_MEMALIGN
#ضمن <stdlib.h>
#وإلاإذا defined HAVE_MALLOC_H
#ضمن <malloc.h>
#د_که_پای

#که_تعریف_شوی OPENCV_ALLOC_ENABLE_STATISTICS
#تعریف_کړه OPENCV_ALLOC_STATISTICS_حد 4096  // don't track buffers less than N bytes
#ضمن <map>
#د_که_پای

نطاق cv {

ساكن فراغ* بهر_د_حافظه_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (بینښه اوږد اوږد)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

ساكن cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    أرجع allocator_stats;
}

#إذا defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
ساكن منطقي read_حافظه_Alignment_Parameter()
{
    منطقي value = ریښتیا;
#إذا defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_حافظه_SANITIZER) \
    && !defined(FUZZING_BUILD_ډېرپېښ_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = خطأ;
    }
#د_که_پای
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    أرجع value;
}

#إذا defined _MSC_VER
#لارښوونه warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
ساكن __forceinline
#وإلا
ساكن مضمن
#د_که_پای
منطقي is_Aligned_ویش_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    ساكن منطقي useMemalign = read_حافظه_Alignment_Parameter();
    أرجع useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
ساكن ثابت منطقي g_force_initialization_memalign_flag
#إذا defined __GNUC__
    __attribute__((ناکارول_شوی))
#د_که_پای
    = is_Aligned_ویش_Enabled();
#د_که_پای

#که_تعریف_شوی OPENCV_ALLOC_ENABLE_STATISTICS
ساكن مضمن
فراغ* fastMalloc_(size_t size)
#وإلا
فراغ* fastMalloc(size_t size)
#د_که_پای
{
#که_تعریف_شوی HAVE_POSIX_MEMALIGN
    إذا (is_Aligned_ویش_Enabled())
    {
        فراغ* ptr = عدم;
        إذا(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = عدم;
        إذا(!ptr)
            أرجع بهر_د_حافظه_Error(size);
        أرجع ptr;
    }
#وإلاإذا defined HAVE_MEMALIGN
    إذا (is_Aligned_ویش_Enabled())
    {
        فراغ* ptr = memalign(CV_MALLOC_ALIGN, size);
        إذا(!ptr)
            أرجع بهر_د_حافظه_Error(size);
        أرجع ptr;
    }
#وإلاإذا defined HAVE_WIN32_ALIGNED_MALLOC
    إذا (is_Aligned_ویش_Enabled())
    {
        فراغ* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        إذا(!ptr)
            أرجع بهر_د_حافظه_Error(size);
        أرجع ptr;
    }
#د_که_پای
    uchar* udata = (uchar*)حافظه_ورکړه(size + حجم(فراغ*) + CV_MALLOC_ALIGN);
    إذا(!udata)
        أرجع بهر_د_حافظه_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    أرجع adata;
}

#که_تعریف_شوی OPENCV_ALLOC_ENABLE_STATISTICS
ساكن مضمن
فراغ fastFree_(فراغ* ptr)
#وإلا
فراغ fastFree(فراغ* ptr)
#د_که_پای
{
#إذا defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    إذا (is_Aligned_ویش_Enabled())
    {
        حرر(ptr);
        أرجع;
    }
#وإلاإذا defined HAVE_WIN32_ALIGNED_MALLOC
    إذا (is_Aligned_ویش_Enabled())
    {
        _aligned_free(ptr);
        أرجع;
    }
#د_که_پای
    إذا(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(حجم(فراغ*)+CV_MALLOC_ALIGN));
        حرر(udata);
    }
}

#که_تعریف_شوی OPENCV_ALLOC_ENABLE_STATISTICS

ساكن
Mutex& get_ویش_Statistics_Mutex()
{
    ساكن Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    أرجع *p_alloc_mutex;
}

ساكن std::map<فراغ*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

فراغ* fastMalloc(size_t size)
{
    فراغ* res = fastMalloc_(size);
    إذا (res && size >= OPENCV_ALLOC_STATISTICS_حد)
    {
        cv::Auto_قلف lock(get_ویش_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    أرجع res;
}

فراغ fastFree(فراغ* ptr)
{
    {
        cv::Auto_قلف lock(get_ویش_Statistics_Mutex());
        std::map<فراغ*, size_t>::iterator i = allocated_buffers.find(ptr);
        إذا (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#د_که_پای // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
