// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ "factory_parallel.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_TBB

#ಅನ್ತರ್ಭಾವ "parallel.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ಸ್ಥಿರ
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ಸ್ಥಿರ std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    ಹಿಂತಿರುಗಿ g_instance;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    ಹಿಂತಿರುಗಿ getInstance();
}
#ಆದರೆ_ಅಂತ್ಯ

}}  // namespace

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BUILD_PLUGIN

#ವ್ಯಾಖ್ಯಾನಿಸು ABI_ಆವೃತ್ತಿ 0
#ವ್ಯಾಖ್ಯಾನಿಸು API_ಆವೃತ್ತಿ 0
#ಅನ್ತರ್ಭಾವ "plugin_parallel_api.hpp"

ಸ್ಥಿರ
Cv_ಫಲಿತಾಂಶ cv_getInstance(CV_ಹೊರಹೋಗು CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ಪ್ರಯತ್ನಿಸು
    {
        ಆದರೆ (!handle)
            ಹಿಂತಿರುಗಿ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ಹಿಂತಿರುಗಿ CV_ERROR_OK;
    }
    ಹಿಡಿ (...)
    {
        ಹಿಂತಿರುಗಿ CV_ERROR_FAIL;
    }
}

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        ಗಾತ್ರ(OpenCV_Core_Parallel_Plugin_API), ABI_ಆವೃತ್ತಿ, API_ಆವೃತ್ತಿ,
        CV_ಆವೃತ್ತಿ_MAJOR, CV_ಆವೃತ್ತಿ_MINOR, CV_ಆವೃತ್ತಿ_REVISION, CV_ಆವೃತ್ತಿ_ಸ್ಥಿತಿ,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_ಆವೃತ್ತಿ) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ಸ್ಥಿರಾಂಕ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(ಪೂರ್ಣಾಂಕ requested_abi_ಆವೃತ್ತಿ, ಪೂರ್ಣಾಂಕ requested_api_ಆವೃತ್ತಿ, ಶೂನ್ಯ* /*reserved=NULL*/) CV_NOEXCEPT
{
    ಆದರೆ (requested_abi_ಆವೃತ್ತಿ == ABI_ಆವೃತ್ತಿ && requested_api_ಆವೃತ್ತಿ <= API_ಆವೃತ್ತಿ)
        ಹಿಂತಿರುಗಿ &plugin_api;
    ಹಿಂತಿರುಗಿ ಖಾಲಿ;
}

#ಆದರೆ_ಅಂತ್ಯ  // BUILD_PLUGIN

#ಆದರೆ_ಅಂತ್ಯ  // HAVE_TBB
