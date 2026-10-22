// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#यदि_अपरिभाषितम् PARALLEL_PLUGIN_API_HPP
#परिभाषय PARALLEL_PLUGIN_API_HPP

#अन्तर्भाव <opencv2/core/cvdef.h>
#अन्तर्भाव <opencv2/core/llapi/llapi.h>

#अन्तर्भाव "opencv2/core/parallel/parallel_backend.hpp"

#यदि !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#परिभाषय API_संस्करणम् 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#परिभाषय ABI_संस्करणम् 0 // preview

#अन्यथा // !defined(BUILD_PLUGIN)

#यदि !defined(ABI_संस्करणम्) || !defined(API_संस्करणम्)
#दोषः "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#यदि_अन्तः

#यदि_अन्तः // !defined(BUILD_PLUGIN)

प्रकारनिर्देश cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

संरचना OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_परिणाम (CV_API_CALL *getInstance)(CV_बहिर्गमनम् CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

प्रकारनिर्देश संरचना OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_शीर्षकम् api_शीर्षकम्;
    संरचना OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#यदि ABI_संस्करणम् == 0 && API_संस्करणम् == 0
प्रकारनिर्देश OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#अन्यथा
#दोषः "Not supported configuration: check ABI_VERSION/API_VERSION"
#यदि_अन्तः

#यदि_परिभाषितम् BUILD_PLUGIN
बाह्य "C" {

CV_PLUGIN_EXPORTS
ध्रुव OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (पूर्णाङ्क requested_abi_संस्करणम्, पूर्णाङ्क requested_api_संस्करणम्, शून्य* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#अन्यथा  // BUILD_PLUGIN
प्रकारनिर्देश ध्रुव OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (पूर्णाङ्क requested_abi_संस्करणम्, पूर्णाङ्क requested_api_संस्करणम्, शून्य* reserved /*NULL*/);
#यदि_अन्तः  // BUILD_PLUGIN

#यदि_अन्तः // PARALLEL_PLUGIN_API_HPP
