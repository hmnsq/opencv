// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#അന്തര്ഭാവ "../precomp.hpp"

#ifdef HAVE_OPENMP

#അന്തര്ഭാവ "parallel.hpp"
#അന്തര്ഭാവ "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

നാമാകാശ cv { നാമാകാശ parallel {

സ്ഥിതം
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    സ്ഥിതം std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    തിരികെ g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    തിരികെ getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#അന്തര്ഭാവ "plugin_parallel_api.hpp"

സ്ഥിതം
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ശ്രമിക്കുക
    {
        എങ്കിൽ (!handle)
            തിരികെ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        തിരികെ CV_ERROR_OK;
    }
    പിടിക്കുക (...)
    {
        തിരികെ CV_ERROR_FAIL;
    }
}

സ്ഥിതം സ്ഥിരം OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        വലുപ്പം(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

സ്ഥിരം OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(പൂർണ്ണസംഖ്യ requested_abi_version, പൂർണ്ണസംഖ്യ requested_api_version, ശൂന്യം* /*reserved=NULL*/) CV_NOEXCEPT
{
    എങ്കിൽ (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        തിരികെ &plugin_api;
    തിരികെ ശൂന്യ;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
