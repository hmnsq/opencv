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

#समावेश "precomp.hpp"

#समावेश <opencv2/core/utils/logger.defines.hpp>
#अघोषित CV_LOG_STRIP_LEVEL
#घोषणा CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#समावेश <opencv2/core/utils/logger.hpp>
#समावेश <opencv2/core/utils/configuration.private.hpp>

#घोषणा CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(शून्य, 0, "alloc.cpp: " << __VA_ARGS__)
#समावेश "opencv2/core/utils/allocator_stats.impl.hpp"
#अघोषित CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#अगर_घोषित HAVE_POSIX_MEMALIGN
#समावेश <stdlib.h>
#या_अगर defined HAVE_MALLOC_H
#समावेश <malloc.h>
#पूर्ण_अगर

#अगर_घोषित OPENCV_ALLOC_ENABLE_STATISTICS
#घोषणा OPENCV_ALLOC_STATISTICS_LIMIT 4096  // don't track buffers less than N bytes
#समावेश <map>
#पूर्ण_अगर

namespace cv {

अटल व्योम* OutOfMemoryError(माप_प्रकार size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (अचिन्हित दीर्घ दीर्घ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

अटल cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    वापस allocator_stats;
}

#अगर defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
अटल bool readMemoryAlignmentParameter()
{
    bool value = true;
#अगर defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_MEMORY_SANITIZER) \
    && !defined(FUZZING_BUILD_MODE_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = false;
    }
#पूर्ण_अगर
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    वापस value;
}

#अगर defined _MSC_VER
#आशय warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
अटल __forceinline
#अन्यथा
अटल अंतरभूत
#पूर्ण_अगर
bool isAlignedAllocationEnabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    अटल bool useMemalign = readMemoryAlignmentParameter();
    वापस useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
अटल स्थिर bool g_force_initialization_memalign_flag
#अगर defined __GNUC__
    __attribute__((unused))
#पूर्ण_अगर
    = isAlignedAllocationEnabled();
#पूर्ण_अगर

#अगर_घोषित OPENCV_ALLOC_ENABLE_STATISTICS
अटल अंतरभूत
व्योम* fastMalloc_(माप_प्रकार size)
#अन्यथा
व्योम* fastMalloc(माप_प्रकार size)
#पूर्ण_अगर
{
#अगर_घोषित HAVE_POSIX_MEMALIGN
    अगर (isAlignedAllocationEnabled())
    {
        व्योम* ptr = शून्य;
        अगर(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = शून्य;
        अगर(!ptr)
            वापस OutOfMemoryError(size);
        वापस ptr;
    }
#या_अगर defined HAVE_MEMALIGN
    अगर (isAlignedAllocationEnabled())
    {
        व्योम* ptr = memalign(CV_MALLOC_ALIGN, size);
        अगर(!ptr)
            वापस OutOfMemoryError(size);
        वापस ptr;
    }
#या_अगर defined HAVE_WIN32_ALIGNED_MALLOC
    अगर (isAlignedAllocationEnabled())
    {
        व्योम* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        अगर(!ptr)
            वापस OutOfMemoryError(size);
        वापस ptr;
    }
#पूर्ण_अगर
    uchar* udata = (uchar*)दो_स्मृति(size + माप(व्योम*) + CV_MALLOC_ALIGN);
    अगर(!udata)
        वापस OutOfMemoryError(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    वापस adata;
}

#अगर_घोषित OPENCV_ALLOC_ENABLE_STATISTICS
अटल अंतरभूत
व्योम fastFree_(व्योम* ptr)
#अन्यथा
व्योम fastFree(व्योम* ptr)
#पूर्ण_अगर
{
#अगर defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    अगर (isAlignedAllocationEnabled())
    {
        मुक्त(ptr);
        वापस;
    }
#या_अगर defined HAVE_WIN32_ALIGNED_MALLOC
    अगर (isAlignedAllocationEnabled())
    {
        _aligned_free(ptr);
        वापस;
    }
#पूर्ण_अगर
    अगर(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (सूचक_भेद_प्रकार)(माप(व्योम*)+CV_MALLOC_ALIGN));
        मुक्त(udata);
    }
}

#अगर_घोषित OPENCV_ALLOC_ENABLE_STATISTICS

अटल
Mutex& getAllocationStatisticsMutex()
{
    अटल Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    वापस *p_alloc_mutex;
}

अटल std::map<व्योम*, माप_प्रकार> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

व्योम* fastMalloc(माप_प्रकार size)
{
    व्योम* res = fastMalloc_(size);
    अगर (res && size >= OPENCV_ALLOC_STATISTICS_LIMIT)
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    वापस res;
}

व्योम fastFree(व्योम* ptr)
{
    {
        cv::AutoLock lock(getAllocationStatisticsMutex());
        std::map<व्योम*, माप_प्रकार>::iterator i = allocated_buffers.find(ptr);
        अगर (i != allocated_buffers.end())
        {
            माप_प्रकार size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#पूर्ण_अगर // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
