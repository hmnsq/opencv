// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "../precomp.hpp"

#ifdef HAVE_OPENMP

#شامل "parallel.hpp"
#شامل "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

فضاینام cv { فضاینام parallel {

ایستا
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    ایستا std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    بازگشت g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    بازگشت getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#شامل "plugin_parallel_api.hpp"

ایستا
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    تلاش
    {
        اگر (!handle)
            بازگشت CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        بازگشت CV_ERROR_OK;
    }
    بگیر (...)
    {
        بازگشت CV_ERROR_FAIL;
    }
}

ایستا ثابت OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        اندازه(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحیح requested_abi_version, صحیح requested_api_version, تهی* /*reserved=NULL*/) CV_NOEXCEPT
{
    اگر (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        بازگشت &plugin_api;
    بازگشت پوچ;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
