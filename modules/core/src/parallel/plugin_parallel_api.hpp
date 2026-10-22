// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جيڪڏهن_وصف_نه_ٿيل PARALLEL_PLUGIN_API_HPP
#وصف_ڪريو PARALLEL_PLUGIN_API_HPP

#ضمن <opencv2/core/cvdef.h>
#ضمن <opencv2/core/llapi/llapi.h>

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

#إذا !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#وصف_ڪريو API_نسخو 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#وصف_ڪريو ABI_نسخو 0 // preview

#وإلا // !defined(BUILD_PLUGIN)

#إذا !defined(ABI_نسخو) || !defined(API_نسخو)
#غلطي "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#جيڪڏهن_پورو

#جيڪڏهن_پورو // !defined(BUILD_PLUGIN)

تعريفنوع cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_نتيجو (CV_API_CALL *getInstance)(CV_ٻاهريون CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

تعريفنوع بنية OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_سرو api_سرو;
    بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#إذا ABI_نسخو == 0 && API_نسخو == 0
تعريفنوع OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#وإلا
#غلطي "Not supported configuration: check ABI_VERSION/API_VERSION"
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل BUILD_PLUGIN
خارجي "C" {

CV_PLUGIN_EXPORTS
ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (صحيح requested_abi_نسخو, صحيح requested_api_نسخو, فراغ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#وإلا  // BUILD_PLUGIN
تعريفنوع ثابت OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (صحيح requested_abi_نسخو, صحيح requested_api_نسخو, فراغ* reserved /*NULL*/);
#جيڪڏهن_پورو  // BUILD_PLUGIN

#جيڪڏهن_پورو // PARALLEL_PLUGIN_API_HPP
