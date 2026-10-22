// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अगर_अघोषित PARALLEL_PLUGIN_API_HPP
#घोषणा PARALLEL_PLUGIN_API_HPP

#समावेश <opencv2/core/cvdef.h>
#समावेश <opencv2/core/llapi/llapi.h>

#समावेश "opencv2/core/parallel/parallel_backend.hpp"

#अगर !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#घोषणा API_VERSION 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#घोषणा ABI_VERSION 0 // preview

#अन्यथा // !defined(BUILD_PLUGIN)

#अगर !defined(ABI_VERSION) || !defined(API_VERSION)
#त्रुटि "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#पूर्ण_अगर

#पूर्ण_अगर // !defined(BUILD_PLUGIN)

प्रकार cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

काष्ठा OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_परिणाम (CV_API_CALL *getInstance)(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

प्रकार काष्ठा OpenCV_Core_Parallel_Plugin_API_v0
{
    OpenCV_API_Header api_header;
    काष्ठा OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#अगर ABI_VERSION == 0 && API_VERSION == 0
प्रकार OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#अन्यथा
#त्रुटि "Not supported configuration: check ABI_VERSION/API_VERSION"
#पूर्ण_अगर

#अगर_घोषित BUILD_PLUGIN
बाह्य "C" {

CV_PLUGIN_EXPORTS
स्थिर OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (पूर्णांक requested_abi_version, पूर्णांक requested_api_version, व्योम* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#अन्यथा  // BUILD_PLUGIN
प्रकार स्थिर OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (पूर्णांक requested_abi_version, पूर्णांक requested_api_version, व्योम* reserved /*NULL*/);
#पूर्ण_अगर  // BUILD_PLUGIN

#पूर्ण_अगर // PARALLEL_PLUGIN_API_HPP
