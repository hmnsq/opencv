// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#నిర్వచితం_కాకపోతే PARALLEL_PLUGIN_API_HPP
#నిర్వచించు PARALLEL_PLUGIN_API_HPP

#అన్తర్భావ <opencv2/core/cvdef.h>
#అన్తర్భావ <opencv2/core/llapi/llapi.h>

#అన్తర్భావ "opencv2/core/parallel/parallel_backend.hpp"

#ఒకవేళ !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#నిర్వచించు API_సంస్కరణ 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#నిర్వచించు ABI_సంస్కరణ 0 // preview

#లేకపోతే // !defined(BUILD_PLUGIN)

#ఒకవేళ !defined(ABI_సంస్కరణ) || !defined(API_సంస్కరణ)
#దోషం "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#అయితే_ముగింపు

#అయితే_ముగింపు // !defined(BUILD_PLUGIN)

రకనిర్వచనం cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

నిర్మాణం OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_ఫలితం (CV_API_CALL *getInstance)(CV_బహిర్గతం CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

రకనిర్వచనం నిర్మాణం OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_శీర్షిక api_శీర్షిక;
    నిర్మాణం OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#ఒకవేళ ABI_సంస్కరణ == 0 && API_సంస్కరణ == 0
రకనిర్వచనం OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#లేకపోతే
#దోషం "Not supported configuration: check ABI_VERSION/API_VERSION"
#అయితే_ముగింపు

#నిర్వచితమైతే BUILD_PLUGIN
బాహ్య "C" {

CV_PLUGIN_EXPORTS
స్థిరరాశి OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (పూర్ణాంకం requested_abi_సంస్కరణ, పూర్ణాంకం requested_api_సంస్కరణ, శూన్య* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#లేకపోతే  // BUILD_PLUGIN
రకనిర్వచనం స్థిరరాశి OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (పూర్ణాంకం requested_abi_సంస్కరణ, పూర్ణాంకం requested_api_సంస్కరణ, శూన్య* reserved /*NULL*/);
#అయితే_ముగింపు  // BUILD_PLUGIN

#అయితే_ముగింపు // PARALLEL_PLUGIN_API_HPP
