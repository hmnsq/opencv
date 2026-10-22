// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef PARALLEL_PLUGIN_API_HPP
#define PARALLEL_PLUGIN_API_HPP

#ಅನ್ತರ್ಭಾವ <opencv2/core/cvdef.h>
#ಅನ್ತರ್ಭಾವ <opencv2/core/llapi/llapi.h>

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"

#ಯದಿ !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#define API_VERSION 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#define ABI_VERSION 0 // preview

#ಅನ್ಯಥಾ // !defined(BUILD_PLUGIN)

#ಯದಿ !defined(ABI_VERSION) || !defined(API_VERSION)
#error "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#endif

#endif // !defined(BUILD_PLUGIN)

ಪ್ರಕಾರನಿರ್ದೇಶ cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

ಸಂರಚನಾ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    CvResult (CV_API_CALL *getInstance)(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ OpenCV_Core_Parallel_Plugin_API_v0
{
    OpenCV_API_Header api_header;
    ಸಂರಚನಾ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#ಯದಿ ABI_VERSION == 0 && API_VERSION == 0
ಪ್ರಕಾರನಿರ್ದೇಶ OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#ಅನ್ಯಥಾ
#error "Not supported configuration: check ABI_VERSION/API_VERSION"
#endif

#ifdef BUILD_PLUGIN
ಬಾಹ್ಯ "C" {

CV_PLUGIN_EXPORTS
ಧ್ರುವ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (ಪೂರ್ಣಾಙ್ಕ requested_abi_version, ಪೂರ್ಣಾಙ್ಕ requested_api_version, ಶೂನ್ಯ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#ಅನ್ಯಥಾ  // BUILD_PLUGIN
ಪ್ರಕಾರನಿರ್ದೇಶ ಧ್ರುವ OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (ಪೂರ್ಣಾಙ್ಕ requested_abi_version, ಪೂರ್ಣಾಙ್ಕ requested_api_version, ಶೂನ್ಯ* reserved /*NULL*/);
#endif  // BUILD_PLUGIN

#endif // PARALLEL_PLUGIN_API_HPP
