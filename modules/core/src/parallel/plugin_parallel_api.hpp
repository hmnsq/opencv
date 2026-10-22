// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef PARALLEL_PLUGIN_API_HPP
#define PARALLEL_PLUGIN_API_HPP

#ଅନ୍ତର୍ଭାଵ <opencv2/core/cvdef.h>
#ଅନ୍ତର୍ଭାଵ <opencv2/core/llapi/llapi.h>

#ଅନ୍ତର୍ଭାଵ "opencv2/core/parallel/parallel_backend.hpp"

#ଯଦି !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#define API_VERSION 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#define ABI_VERSION 0 // preview

#ଅନ୍ୟଥା // !defined(BUILD_PLUGIN)

#ଯଦି !defined(ABI_VERSION) || !defined(API_VERSION)
#error "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#endif

#endif // !defined(BUILD_PLUGIN)

ପ୍ରକାରବ୍ୟାଖ୍ୟା cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

ସଂରଚନା OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    CvResult (CV_API_CALL *getInstance)(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା OpenCV_Core_Parallel_Plugin_API_v0
{
    OpenCV_API_Header api_header;
    ସଂରଚନା OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#ଯଦି ABI_VERSION == 0 && API_VERSION == 0
ପ୍ରକାରବ୍ୟାଖ୍ୟା OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#ଅନ୍ୟଥା
#error "Not supported configuration: check ABI_VERSION/API_VERSION"
#endif

#ifdef BUILD_PLUGIN
ବାହ୍ୟ "C" {

CV_PLUGIN_EXPORTS
ଅଚଳ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_abi_version, ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_api_version, ଶୂନ୍ୟ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#ଅନ୍ୟଥା  // BUILD_PLUGIN
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅଚଳ OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_abi_version, ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_api_version, ଶୂନ୍ୟ* reserved /*NULL*/);
#endif  // BUILD_PLUGIN

#endif // PARALLEL_PLUGIN_API_HPP
