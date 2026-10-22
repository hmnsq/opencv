// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#که_نه_تعریف_شوی PARALLEL_PLUGIN_API_HPP
#تعریف_کړه PARALLEL_PLUGIN_API_HPP

#ضمن <opencv2/core/cvdef.h>
#ضمن <opencv2/core/llapi/llapi.h>

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

#إذا !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#تعریف_کړه API_نسخه 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#تعریف_کړه ABI_نسخه 0 // preview

#وإلا // !defined(BUILD_PLUGIN)

#إذا !defined(ABI_نسخه) || !defined(API_نسخه)
#تېروتنه "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#د_که_پای

#د_که_پای // !defined(BUILD_PLUGIN)

تعريفنوع cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_پایله (CV_API_CALL *getInstance)(CV_بهر CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

تعريفنوع بنية OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_سرلیک api_سرلیک;
    بنية OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#إذا ABI_نسخه == 0 && API_نسخه == 0
تعريفنوع OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#وإلا
#تېروتنه "Not supported configuration: check ABI_VERSION/API_VERSION"
#د_که_پای

#که_تعریف_شوی BUILD_PLUGIN
خارجي "C" {

CV_PLUGIN_EXPORTS
ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (صحيح requested_abi_نسخه, صحيح requested_api_نسخه, فراغ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#وإلا  // BUILD_PLUGIN
تعريفنوع ثابت OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (صحيح requested_abi_نسخه, صحيح requested_api_نسخه, فراغ* reserved /*NULL*/);
#د_که_پای  // BUILD_PLUGIN

#د_که_پای // PARALLEL_PLUGIN_API_HPP
