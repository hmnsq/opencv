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

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/logger.defines.hpp>
#నిర్వచనం_రద్దు CV_LOG_STRIP_LEVEL
#నిర్వచించు CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#అన్తర్భావ <opencv2/core/utils/logger.hpp>
#అన్తర్భావ <opencv2/core/utils/configuration.private.hpp>

#నిర్వచించు CV__ALLOCATOR_STATS_LOG(...) CV_LOG_VERBOSE(ఖాళీ, 0, "alloc.cpp: " << __VA_ARGS__)
#అన్తర్భావ "opencv2/core/utils/allocator_stats.impl.hpp"
#నిర్వచనం_రద్దు CV__ALLOCATOR_STATS_LOG

//#define OPENCV_ALLOC_ENABLE_STATISTICS


#నిర్వచితమైతే HAVE_POSIX_MEMALIGN
#అన్తర్భావ <stdlib.h>
#లేదావేళ defined HAVE_MALLOC_H
#అన్తర్భావ <malloc.h>
#అయితే_ముగింపు

#నిర్వచితమైతే OPENCV_ALLOC_ENABLE_STATISTICS
#నిర్వచించు OPENCV_ALLOC_STATISTICS_హద్దు 4096  // don't track buffers less than N bytes
#అన్తర్భావ <map>
#అయితే_ముగింపు

నామాకాశ cv {

స్థిర శూన్య* బహిర్గతం_యొక్క_జ్ఞాపకం_Error(size_t size)
{
    CV_Error_(cv::Error::StsNoMem, ("Failed to allocate %llu bytes", (గుర్తులేని దీర్ఘ దీర్ఘ)size));
}

CV_EXPORTS cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics();

స్థిర cv::utils::AllocatorStatistics allocator_stats;

cv::utils::AllocatorStatisticsInterface& getAllocatorStatistics()
{
    తిరిగి allocator_stats;
}

#ఒకవేళ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN || defined HAVE_WIN32_ALIGNED_MALLOC
స్థిర తార్కిక read_జ్ఞాపకం_Alignment_Parameter()
{
    తార్కిక value = నిజం;
#ఒకవేళ defined(__GLIBC__) && defined(__linux__) \
    && !defined(CV_STATIC_ANALYSIS) \
    && !defined(OPENCV_ENABLE_జ్ఞాపకం_SANITIZER) \
    && !defined(FUZZING_BUILD_బహుళకం_UNSAFE_FOR_PRODUCTION)  /* oss-fuzz */ \
    && !defined(_WIN32)  /* MinGW? */
    {
        // https://github.com/opencv/opencv/issues/15526
        value = అబద్ధం;
    }
#అయితే_ముగింపు
    value = cv::utils::getConfigurationParameterBool("OPENCV_ENABLE_MEMALIGN", value);  // should not call fastMalloc() internally
    // TODO add checks for valgrind, ASAN if value == false
    తిరిగి value;
}

#ఒకవేళ defined _MSC_VER
#సూచన warning(suppress:4714)  // preventive: const marked as __forceinline not inlined
స్థిర __forceinline
#లేకపోతే
స్థిర అంతఃవరుస
#అయితే_ముగింపు
తార్కిక is_Aligned_కేటాయింపు_Enabled()
{
    // use construct on first use idiom https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use
    // details: https://github.com/opencv/opencv/issues/15691
    స్థిర తార్కిక useMemalign = read_జ్ఞాపకం_Alignment_Parameter();
    తిరిగి useMemalign;
}

// need for this static const is disputed; retaining as it doesn't cause harm
స్థిర స్థిరరాశి తార్కిక g_force_initialization_memalign_flag
#ఒకవేళ defined __GNUC__
    __attribute__((వాడని))
#అయితే_ముగింపు
    = is_Aligned_కేటాయింపు_Enabled();
#అయితే_ముగింపు

#నిర్వచితమైతే OPENCV_ALLOC_ENABLE_STATISTICS
స్థిర అంతఃవరుస
శూన్య* fastMalloc_(size_t size)
#లేకపోతే
శూన్య* fastMalloc(size_t size)
#అయితే_ముగింపు
{
#నిర్వచితమైతే HAVE_POSIX_MEMALIGN
    ఒకవేళ (is_Aligned_కేటాయింపు_Enabled())
    {
        శూన్య* ptr = ఖాళీ;
        ఒకవేళ(posix_memalign(&ptr, CV_MALLOC_ALIGN, size))
            ptr = ఖాళీ;
        ఒకవేళ(!ptr)
            తిరిగి బహిర్గతం_యొక్క_జ్ఞాపకం_Error(size);
        తిరిగి ptr;
    }
#లేదావేళ defined HAVE_MEMALIGN
    ఒకవేళ (is_Aligned_కేటాయింపు_Enabled())
    {
        శూన్య* ptr = memalign(CV_MALLOC_ALIGN, size);
        ఒకవేళ(!ptr)
            తిరిగి బహిర్గతం_యొక్క_జ్ఞాపకం_Error(size);
        తిరిగి ptr;
    }
#లేదావేళ defined HAVE_WIN32_ALIGNED_MALLOC
    ఒకవేళ (is_Aligned_కేటాయింపు_Enabled())
    {
        శూన్య* ptr = _aligned_malloc(size, CV_MALLOC_ALIGN);
        ఒకవేళ(!ptr)
            తిరిగి బహిర్గతం_యొక్క_జ్ఞాపకం_Error(size);
        తిరిగి ptr;
    }
#అయితే_ముగింపు
    uchar* udata = (uchar*)జ్ఞాపకం_ఇవ్వు(size + పరిమాణం(శూన్య*) + CV_MALLOC_ALIGN);
    ఒకవేళ(!udata)
        తిరిగి బహిర్గతం_యొక్క_జ్ఞాపకం_Error(size);
    uchar** adata = alignPtr((uchar**)udata + 1, CV_MALLOC_ALIGN);
    adata[-1] = udata;
    తిరిగి adata;
}

#నిర్వచితమైతే OPENCV_ALLOC_ENABLE_STATISTICS
స్థిర అంతఃవరుస
శూన్య fastFree_(శూన్య* ptr)
#లేకపోతే
శూన్య fastFree(శూన్య* ptr)
#అయితే_ముగింపు
{
#ఒకవేళ defined HAVE_POSIX_MEMALIGN || defined HAVE_MEMALIGN
    ఒకవేళ (is_Aligned_కేటాయింపు_Enabled())
    {
        విడుదల(ptr);
        తిరిగి;
    }
#లేదావేళ defined HAVE_WIN32_ALIGNED_MALLOC
    ఒకవేళ (is_Aligned_కేటాయింపు_Enabled())
    {
        _aligned_free(ptr);
        తిరిగి;
    }
#అయితే_ముగింపు
    ఒకవేళ(ptr)
    {
        uchar* udata = ((uchar**)ptr)[-1];
        CV_DbgAssert(udata < (uchar*)ptr &&
               ((uchar*)ptr - udata) <= (ptrdiff_t)(పరిమాణం(శూన్య*)+CV_MALLOC_ALIGN));
        విడుదల(udata);
    }
}

#నిర్వచితమైతే OPENCV_ALLOC_ENABLE_STATISTICS

స్థిర
Mutex& get_కేటాయింపు_Statistics_Mutex()
{
    స్థిర Mutex* p_alloc_mutex = allocSingletonNew<Mutex>();
    CV_Assert(p_alloc_mutex);
    తిరిగి *p_alloc_mutex;
}

స్థిర std::map<శూన్య*, size_t> allocated_buffers;  // guarded by getAllocationStatisticsMutex()

శూన్య* fastMalloc(size_t size)
{
    శూన్య* res = fastMalloc_(size);
    ఒకవేళ (res && size >= OPENCV_ALLOC_STATISTICS_హద్దు)
    {
        cv::Auto_తాళం lock(get_కేటాయింపు_Statistics_Mutex());
        allocated_buffers.insert(std::make_pair(res, size));
        allocator_stats.onAllocate(size);
    }
    తిరిగి res;
}

శూన్య fastFree(శూన్య* ptr)
{
    {
        cv::Auto_తాళం lock(get_కేటాయింపు_Statistics_Mutex());
        std::map<శూన్య*, size_t>::iterator i = allocated_buffers.find(ptr);
        ఒకవేళ (i != allocated_buffers.end())
        {
            size_t size = i->second;
            allocator_stats.onFree(size);
            allocated_buffers.erase(i);
        }
    }
    fastFree_(ptr);
}

#అయితే_ముగింపు // OPENCV_ALLOC_ENABLE_STATISTICS

} // namespace
/* End of file. */
