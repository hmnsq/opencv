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
#अपरिभाषय CV_LOG_STRIP_LEVEL
#परिभाषय CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#अन्तर्भाव <opencv2/core/utils/logger.hpp>
#अन्तर्भाव <opencv2/core/utils/configuration.private.hpp>

#परिभाषय CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(रिक्त, 0, "alloc.cpp: " << __VA_ARGS__)
#अन्तर्भाव "opencv2/core/utils/allocator_stats.impl.hpp"
#अपरिभाषय CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#यदि_परिभाषितम् HAVE_POSIX_MEMALIGN
#अन्तर्भाव <stdlib.h>
#नोचेद्यदि defined HAVE_MALLOC_H
#अन्तर्भाव <malloc.h>
#यदि_अन्तः

#यदि_परिभाषितम् OPENCV_ALLOC_ENABLE_STATISTICS
#परिभाषय OPENCV_ALLOC_STATISTICS_सीमा 4096  // don't track buffers less than N bytes
#अन्तर्भाव <map>
#यदि_अन्तः

नामाकाश cv {

स्थिर शून्य* बहिर्गमनम्_सम्बन्धि_स्मृतिः_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (अचिह्नित दीर्घ दीर्घ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

स्थिर cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    प्रत्यागम allocator_stats;
}

#यदि defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
स्थिर तार्किक read_स्मृतिः_Alignment_Parameter()
{
    तार्किक value = सत्य;
#यदि defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_स्मृतिः_SANITIZER) \
    && !defined(FUZZING_BUILD_बहुलकम्_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = असत्य;
    }
#यदि_अन्तः
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    प्रत्यागम value;
}

#यदि defined _MSC_VER
#निर्देशः warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
स्थिर __forceinline
#अन्यथा
स्थिर अन्तःस्थ
#यदि_अन्तः
तार्किक is_Aligned_आवण्टनम्_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    स्थिर तार्किक useMemalign = read_स्मृतिः_Alignment_Parameter();
    प्रत्यागम useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
स्थिर ध्रुव तार्किक g_force_initialization_memalign_flag
#यदि defined __GNUC__
    __attribute__((अप्रयुक्तम्))
#यदि_अन्तः
    = is_Aligned_आवण्टनम्_Enabled();
#यदि_अन्तः

#यदि_परिभाषितम् OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य* fastMalloc_(size_t size)
#अन्यथा
शून्य* fastMalloc(size_t size)
#यदि_अन्तः
{
#यदि_परिभाषितम् HAVE_POSIX_MEMALIGN
    यदि (is_Aligned_आवण्टनम्_Enabled())
    {
        शून्य* ptr = रिक्त;
        यदि(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = रिक्त;
        यदि(!ptr)
            प्रत्यागम बहिर्गमनम्_सम्बन्धि_स्मृतिः_Error(size);
        प्रत्यागम ptr;
    }
#नोचेद्यदि defined HAVE_MEMALIGN
    यदि (is_Aligned_आवण्टनम्_Enabled())
    {
        शून्य* ptr = memalign(CV_MALLOC_ALIGN, size);
        यदि(!ptr)
            प्रत्यागम बहिर्गमनम्_सम्बन्धि_स्मृतिः_Error(size);
        प्रत्यागम ptr;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    यदि (is_Aligned_आवण्टनम्_Enabled())
    {
        शून्य* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        यदि(!ptr)
            प्रत्यागम बहिर्गमनम्_सम्बन्धि_स्मृतिः_Error(size);
        प्रत्यागम ptr;
    }
#यदि_अन्तः
    uchar* udata = (uchar*)स्मृतिं_देहि(size + परिमाण(शून्य*) + CV_MALLOC_ALIGN);
    यदि(!udata)
        प्रत्यागम बहिर्गमनम्_सम्बन्धि_स्मृतिः_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    प्रत्यागम adata;
}

#यदि_परिभाषितम् OPENCV_ALLOC_ENABLE_STATISTICS
स्थिर अन्तःस्थ
शून्य fastFree_(शून्य* ptr)
#अन्यथा
शून्य fastFree(शून्य* ptr)
#यदि_अन्तः
{
#यदि defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    यदि (is_Aligned_आवण्टनम्_Enabled())
    {
        मुक्त(ptr);
        प्रत्यागम;
    }
#नोचेद्यदि defined HAVE_WIN32_ALIGNED_MALLOC
    यदि (is_Aligned_आवण्टनम्_Enabled())
    {
        _aligned_free(ptr);
        प्रत्यागम;
    }
#यदि_अन्तः
    यदि(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(परिमाण(शून्य*)+CV_MALLOC_ALIGN));
        मुक्त(udata);
    }
}

#यदि_परिभाषितम् OPENCV_ALLOC_ENABLE_STATISTICS

स्थिर
Mutex& get_आवण्टनम्_Statistics_Mutex()
{
    स्थिर Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    प्रत्यागम *p_alloc_mutex;
}

स्थिर std::map<शून्य*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

शून्य* fastMalloc(size_t size)
{
    शून्य* res = fastMalloc_(size);
    यदि (res && size >= OPENCV_ALLOC_STATISTICS_सीमा)
    {
        cv::Auto_तालकम् lock(get_आवण्टनम्_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    प्रत्यागम res;
}

शून्य fastFree(शून्य* ptr)
{
    {
        cv::Auto_तालकम् lock(get_आवण्टनम्_Statistics_Mutex());
        std::map<शून्य*, size_t>::iterator i = allocated_buffers.find(ptr);
        यदि (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#यदि_अन्तः // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
