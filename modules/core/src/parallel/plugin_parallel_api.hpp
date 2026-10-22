// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ PARALLEL_PLUGIN_API_HPP
#تعریف_کرو PARALLEL_PLUGIN_API_HPP

#شامل <opencv2/core/cvdef.h>
#شامل <opencv2/core/llapi/llapi.h>

#شامل "opencv2/core/parallel/parallel_backend.hpp"

#اگر !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#تعریف_کرو API_نسخہ 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#تعریف_کرو ABI_نسخہ 0 // preview

#ورنہ // !defined(BUILD_PLUGIN)

#اگر !defined(ABI_نسخہ) || !defined(API_نسخہ)
#غلطی "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#اگر_ختم

#اگر_ختم // !defined(BUILD_PLUGIN)

قسمتعریف cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

ساخت OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_نتیجہ (CV_API_CALL *getInstance)(CV_برآمد CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

قسمتعریف ساخت OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_سرخی api_سرخی;
    ساخت OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#اگر ABI_نسخہ == 0 && API_نسخہ == 0
قسمتعریف OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#ورنہ
#غلطی "Not supported configuration: check ABI_VERSION/API_VERSION"
#اگر_ختم

#اگر_تعریف_شدہ BUILD_PLUGIN
خارجی "C" {

CV_PLUGIN_EXPORTS
ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (صحیح requested_abi_نسخہ, صحیح requested_api_نسخہ, خالی* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#ورنہ  // BUILD_PLUGIN
قسمتعریف ثابت OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (صحیح requested_abi_نسخہ, صحیح requested_api_نسخہ, خالی* reserved /*NULL*/);
#اگر_ختم  // BUILD_PLUGIN

#اگر_ختم // PARALLEL_PLUGIN_API_HPP
