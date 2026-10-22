// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PARALLEL_PLUGIN_API_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು PARALLEL_PLUGIN_API_HPP

#ಅನ್ತರ್ಭಾವ <opencv2/core/cvdef.h>
#ಅನ್ತರ್ಭಾವ <opencv2/core/llapi/llapi.h>

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"

#ಆದರೆ !defined(BUILD_PLUGIN)

/// increased for backward-compatible changes, e.g. add new function
/// Caller API <= Plugin API -> plugin is fully compatible
/// Caller API > Plugin API -> plugin is not fully compatible, caller should use extra checks to use plugins with older API
#ವ್ಯಾಖ್ಯಾನಿಸು API_ಆವೃತ್ತಿ 0 // preview

/// increased for incompatible changes, e.g. remove function argument
/// Caller ABI == Plugin ABI -> plugin is compatible
/// Caller ABI > Plugin ABI -> plugin is not compatible, caller should use shim code to use old ABI plugins (caller may know how lower ABI works, so it is possible)
/// Caller ABI < Plugin ABI -> plugin can't be used (plugin should provide interface with lower ABI to handle that)
#ವ್ಯಾಖ್ಯಾನಿಸು ABI_ಆವೃತ್ತಿ 0 // preview

#ಇಲ್ಲದಿದ್ದರೆ // !defined(BUILD_PLUGIN)

#ಆದರೆ !defined(ABI_ಆವೃತ್ತಿ) || !defined(API_ಆವೃತ್ತಿ)
#ದೋಷ "Plugin must define ABI_VERSION and API_VERSION before including parallel_plugin_api.hpp"
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ // !defined(BUILD_PLUGIN)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ cv::parallel::ParallelForAPI* CvPluginParallelBackendAPI;

ರಚನೆ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries
{
    /** @brief Get parallel backend API instance

    @param[out] handle pointer on backend API handle

    @note API-CALL 1, API-Version == 0
     */
    Cv_ಫಲಿತಾಂಶ (CV_API_CALL *getInstance)(CV_ಹೊರಹೋಗು CvPluginParallelBackendAPI* handle) CV_NOEXCEPT;
}; // OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ OpenCV_Core_Parallel_Plugin_API_v0
{
    Open_CV_API_ಶೀರ್ಷಿಕೆ api_ಶೀರ್ಷಿಕೆ;
    ರಚನೆ OpenCV_Core_Parallel_Plugin_API_v0_0_api_entries v0;
} OpenCV_Core_Parallel_Plugin_API_v0;

#ಆದರೆ ABI_ಆವೃತ್ತಿ == 0 && API_ಆವೃತ್ತಿ == 0
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ OpenCV_Core_Parallel_Plugin_API_v0 OpenCV_Core_Parallel_Plugin_API;
#ಇಲ್ಲದಿದ್ದರೆ
#ದೋಷ "Not supported configuration: check ABI_VERSION/API_VERSION"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BUILD_PLUGIN
ಬಾಹ್ಯ "C" {

CV_PLUGIN_EXPORTS
ಸ್ಥಿರಾಂಕ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0
        (ಪೂರ್ಣಾಂಕ requested_abi_ಆವೃತ್ತಿ, ಪೂರ್ಣಾಂಕ requested_api_ಆವೃತ್ತಿ, ಶೂನ್ಯ* reserved /*NULL*/) CV_NOEXCEPT;

}  // extern "C"
#ಇಲ್ಲದಿದ್ದರೆ  // BUILD_PLUGIN
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಸ್ಥಿರಾಂಕ OpenCV_Core_Parallel_Plugin_API* (CV_API_CALL *FN_opencv_core_parallel_plugin_init_t)
        (ಪೂರ್ಣಾಂಕ requested_abi_ಆವೃತ್ತಿ, ಪೂರ್ಣಾಂಕ requested_api_ಆವೃತ್ತಿ, ಶೂನ್ಯ* reserved /*NULL*/);
#ಆದರೆ_ಅಂತ್ಯ  // BUILD_PLUGIN

#ಆದರೆ_ಅಂತ್ಯ // PARALLEL_PLUGIN_API_HPP
