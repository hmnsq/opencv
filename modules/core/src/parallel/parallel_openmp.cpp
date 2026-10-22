// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#அந்தர்பாவ "../precomp.hpp"

#ifdef HAVE_OPENMP

#அந்தர்பாவ "parallel.hpp"
#அந்தர்பாவ "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

நாமாகாஶ cv { நாமாகாஶ parallel {

நிலையான
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    நிலையான std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    திரும்பு g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    திரும்பு getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#அந்தர்பாவ "plugin_parallel_api.hpp"

நிலையான
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    முயல்
    {
        எனில் (!handle)
            திரும்பு CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        திரும்பு CV_ERROR_OK;
    }
    பிடி (...)
    {
        திரும்பு CV_ERROR_FAIL;
    }
}

நிலையான மாறிலி OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        அளவு(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

மாறிலி OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(முழு requested_abi_version, முழு requested_api_version, வெறுமை* /*reserved=NULL*/) CV_NOEXCEPT
{
    எனில் (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        திரும்பு &plugin_api;
    திரும்பு வெற்று;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
