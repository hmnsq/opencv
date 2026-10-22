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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.defines.hpp>
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV_LOG_STRIP_LEVEL
#ವ್ಯಾಖ್ಯಾನಿಸು CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/configuration.private.hpp>

#ವ್ಯಾಖ್ಯಾನಿಸು CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ಖಾಲಿ, 0, "alloc.cpp: " << __VA_ARGS__)
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/allocator_stats.impl.hpp"
#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_POSIX_MEMALIGN
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅಥವಾಆದರೆ defined HAVE_MALLOC_H
#ಅನ್ತರ್ಭಾವ <malloc.h>
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_ALLOC_ENABLE_STATISTICS
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_ALLOC_STATISTICS_ಎಲ್ಲೆ 4096  // don't track buffers less than N bytes
#ಅನ್ತರ್ಭಾವ <map>
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv {

ಸ್ಥಿರ ಶೂನ್ಯ* ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

ಸ್ಥಿರ cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    ಹಿಂತಿರುಗಿ allocator_stats;
}

#ಆದರೆ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
ಸ್ಥಿರ ತಾರ್ಕಿಕ read_ಸ್ಮರಣೆ_Alignment_Parameter()
{
    ತಾರ್ಕಿಕ value = ನಿಜ;
#ಆದರೆ defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_ಸ್ಮರಣೆ_SANITIZER) \
    && !defined(FUZZING_BUILD_ಬಹುಲಕ_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = ಸುಳ್ಳು;
    }
#ಆದರೆ_ಅಂತ್ಯ
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    ಹಿಂತಿರುಗಿ value;
}

#ಆದರೆ defined _MSC_VER
#ಸೂಚನೆ warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
ಸ್ಥಿರ __forceinline
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ
ತಾರ್ಕಿಕ is_Aligned_ಹಂಚಿಕೆ_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    ಸ್ಥಿರ ತಾರ್ಕಿಕ useMemalign = read_ಸ್ಮರಣೆ_Alignment_Parameter();
    ಹಿಂತಿರುಗಿ useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ತಾರ್ಕಿಕ g_force_initialization_memalign_flag
#ಆದರೆ defined __GNUC__
    __attribute__((ಬಳಸದ))
#ಆದರೆ_ಅಂತ್ಯ
    = is_Aligned_ಹಂಚಿಕೆ_Enabled();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_ALLOC_ENABLE_STATISTICS
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ* fastMalloc_(size_t size)
#ಇಲ್ಲದಿದ್ದರೆ
ಶೂನ್ಯ* fastMalloc(size_t size)
#ಆದರೆ_ಅಂತ್ಯ
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_POSIX_MEMALIGN
    ಆದರೆ (is_Aligned_ಹಂಚಿಕೆ_Enabled())
    {
        ಶೂನ್ಯ* ptr = ಖಾಲಿ;
        ಆದರೆ(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ಖಾಲಿ;
        ಆದರೆ(!ptr)
            ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ_Error(size);
        ಹಿಂತಿರುಗಿ ptr;
    }
#ಅಥವಾಆದರೆ defined HAVE_MEMALIGN
    ಆದರೆ (is_Aligned_ಹಂಚಿಕೆ_Enabled())
    {
        ಶೂನ್ಯ* ptr = memalign(CV_MALLOC_ALIGN, size);
        ಆದರೆ(!ptr)
            ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ_Error(size);
        ಹಿಂತಿರುಗಿ ptr;
    }
#ಅಥವಾಆದರೆ defined HAVE_WIN32_ALIGNED_MALLOC
    ಆದರೆ (is_Aligned_ಹಂಚಿಕೆ_Enabled())
    {
        ಶೂನ್ಯ* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        ಆದರೆ(!ptr)
            ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ_Error(size);
        ಹಿಂತಿರುಗಿ ptr;
    }
#ಆದರೆ_ಅಂತ್ಯ
    uchar* udata = (uchar*)ಸ್ಮರಣೆ_ಕೊಡು(size + ಗಾತ್ರ(ಶೂನ್ಯ*) + CV_MALLOC_ALIGN);
    ಆದರೆ(!udata)
        ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು_ದ_ಸ್ಮರಣೆ_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    ಹಿಂತಿರುಗಿ adata;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_ALLOC_ENABLE_STATISTICS
ಸ್ಥಿರ ಸಾಲೊಳಗೆ
ಶೂನ್ಯ fastFree_(ಶೂನ್ಯ* ptr)
#ಇಲ್ಲದಿದ್ದರೆ
ಶೂನ್ಯ fastFree(ಶೂನ್ಯ* ptr)
#ಆದರೆ_ಅಂತ್ಯ
{
#ಆದರೆ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    ಆದರೆ (is_Aligned_ಹಂಚಿಕೆ_Enabled())
    {
        ಬಿಡುಗಡೆ(ptr);
        ಹಿಂತಿರುಗಿ;
    }
#ಅಥವಾಆದರೆ defined HAVE_WIN32_ALIGNED_MALLOC
    ಆದರೆ (is_Aligned_ಹಂಚಿಕೆ_Enabled())
    {
        _aligned_free(ptr);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    ಆದರೆ(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(ಗಾತ್ರ(ಶೂನ್ಯ*)+CV_MALLOC_ALIGN));
        ಬಿಡುಗಡೆ(udata);
    }
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV_ALLOC_ENABLE_STATISTICS

ಸ್ಥಿರ
Mutex& get_ಹಂಚಿಕೆ_Statistics_Mutex()
{
    ಸ್ಥಿರ Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    ಹಿಂತಿರುಗಿ *p_alloc_mutex;
}

ಸ್ಥಿರ std::map<ಶೂನ್ಯ*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

ಶೂನ್ಯ* fastMalloc(size_t size)
{
    ಶೂನ್ಯ* res = fastMalloc_(size);
    ಆದರೆ (res && size >= OPENCV_ALLOC_STATISTICS_ಎಲ್ಲೆ)
    {
        cv::Auto_ಬೀಗ lock(get_ಹಂಚಿಕೆ_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    ಹಿಂತಿರುಗಿ res;
}

ಶೂನ್ಯ fastFree(ಶೂನ್ಯ* ptr)
{
    {
        cv::Auto_ಬೀಗ lock(get_ಹಂಚಿಕೆ_Statistics_Mutex());
        std::map<ಶೂನ್ಯ*, size_t>::iterator i = allocated_buffers.find(ptr);
        ಆದರೆ (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#ಆದರೆ_ಅಂತ್ಯ // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
