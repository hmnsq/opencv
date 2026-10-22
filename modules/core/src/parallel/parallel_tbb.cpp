// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ಅನ್ತರ್ಭಾವ "../precomp.hpp"

#ಅನ್ತರ್ಭಾವ "factory_parallel.hpp"

#ifdef HAVE_TBB

#ಅನ್ತರ್ಭಾವ "parallel.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ಸ್ಥಿರ
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ಸ್ಥಿರ std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    ಪ್ರತ್ಯಾಗಮ g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    ಪ್ರತ್ಯಾಗಮ getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#ಅನ್ತರ್ಭಾವ "plugin_parallel_api.hpp"

ಸ್ಥಿರ
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ಯತ್ನ
    {
        ಯದಿ (!handle)
            ಪ್ರತ್ಯಾಗಮ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ಪ್ರತ್ಯಾಗಮ CV_ERROR_OK;
    }
    ಗೃಹಾಣ (...)
    {
        ಪ್ರತ್ಯಾಗಮ CV_ERROR_FAIL;
    }
}

ಸ್ಥಿರ ಧ್ರುವ OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        ಪರಿಮಾಣ(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_VERSION) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ಧ್ರುವ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(ಪೂರ್ಣಾಙ್ಕ requested_abi_version, ಪೂರ್ಣಾಙ್ಕ requested_api_version, ಶೂನ್ಯ* /*reserved=NULL*/) CV_NOEXCEPT
{
    ಯದಿ (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        ಪ್ರತ್ಯಾಗಮ &plugin_api;
    ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
