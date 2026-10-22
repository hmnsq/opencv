// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"

#ifdef HAVE_OPENMP

#ضمن "parallel.hpp"
#ضمن "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

نطاق cv { نطاق parallel {

ساكن
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    ساكن std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    أرجع g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    أرجع getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#ضمن "plugin_parallel_api.hpp"

ساكن
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    حاول
    {
        𐡄𐡍 (!handle)
            أرجع CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        أرجع CV_ERROR_OK;
    }
    امسك (...)
    {
        أرجع CV_ERROR_FAIL;
    }
}

ساكن ثابت OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        حجم(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحيح requested_abi_version, صحيح requested_api_version, فراغ* /*reserved=NULL*/) CV_NOEXCEPT
{
    𐡄𐡍 (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        أرجع &plugin_api;
    أرجع عدم;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
