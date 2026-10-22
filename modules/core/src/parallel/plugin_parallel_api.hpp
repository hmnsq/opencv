// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ PARALLEL_PLUGIN_API_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PARALLEL_PLUGIN_API_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/cvdef.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/llapi/llapi.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/parallel/parallel_backend.hpp"

#ਜੇ !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ API_ਸੰਸਕਰਣ 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ABI_ਸੰਸਕਰਣ 0 // preview

#ਨਹੀਂਤਾਂ // !defined(BUILD_PLUGIN)

#ਜੇ !defined(ABI_ਸੰਸਕਰਣ) || !defined(API_ਸੰਸਕਰਣ)
#ਗਲਤੀ "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ // !defined(BUILD_PLUGIN)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

ਬਣਤਰ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_ਨਤੀਜਾ (CV_API_CALL *getInstance)(CV_ਬਾਹਰਲਾ CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_ਸਿਰਲੇਖ api_ਸਿਰਲੇਖ;
    ਬਣਤਰ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#ਜੇ ABI_ਸੰਸਕਰਣ == 0 && API_ਸੰਸਕਰਣ == 0
ਕਿਸਮਪਰਿਭਾਸ਼ਾ OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#ਨਹੀਂਤਾਂ
#ਗਲਤੀ "Not supported configuration: check ABI_VERSION/API_VERSION"
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BUILD_PLUGIN
ਬਾਹਰੀ "C" {

CV_PLUGIN_EXPORTS
ਅਚਲ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (ਪੂਰਨ requested_abi_ਸੰਸਕਰਣ, ਪੂਰਨ requested_api_ਸੰਸਕਰਣ, ਖਾਲੀ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#ਨਹੀਂਤਾਂ  // BUILD_PLUGIN
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅਚਲ OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (ਪੂਰਨ requested_abi_ਸੰਸਕਰਣ, ਪੂਰਨ requested_api_ਸੰਸਕਰਣ, ਖਾਲੀ* reserved /*NULL*/);
#ਜੇ_ਅੰਤ  // BUILD_PLUGIN

#ਜੇ_ਅੰਤ // PARALLEL_PLUGIN_API_HPP
