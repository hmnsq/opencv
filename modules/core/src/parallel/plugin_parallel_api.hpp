// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جے_بنا_تعریف PARALLEL_PLUGIN_API_HPP
#تعریف_کرو PARALLEL_PLUGIN_API_HPP

#ضمن <opencv2/core/cvdef.h>
#ضمن <opencv2/core/llapi/llapi.h>

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

#إذا !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#تعریف_کرو API_نسخہ 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#تعریف_کرو ABI_نسخہ 0 // preview

#وإلا // !defined(BUILD_PLUGIN)

#إذا !defined(ABI_نسخہ) || !defined(API_نسخہ)
#غلطی "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#جے_انت

#جے_انت // !defined(BUILD_PLUGIN)

تعريفنوع cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_نتیجہ (CV_API_CALL *getInstance)(CV_باہرلا CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

تعريفنوع بنية OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_سرخی api_سرخی;
    بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#إذا ABI_نسخہ == 0 && API_نسخہ == 0
تعريفنوع OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#وإلا
#غلطی "Not supported configuration: check ABI_VERSION/API_VERSION"
#جے_انت

#جے_تعریف_شدہ BUILD_PLUGIN
خارجي "C" {

CV_PLUGIN_EXPORTS
ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (صحيح requested_abi_نسخہ, صحيح requested_api_نسخہ, فراغ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#وإلا  // BUILD_PLUGIN
تعريفنوع ثابت OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (صحيح requested_abi_نسخہ, صحيح requested_api_نسخہ, فراغ* reserved /*NULL*/);
#جے_انت  // BUILD_PLUGIN

#جے_انت // PARALLEL_PLUGIN_API_HPP
