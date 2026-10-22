// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "../precomp.hpp"

#ifdef HAVE_OPENMP

#include "parallel.hpp"
#include "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

namespace cv { namespace parallel {

static
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    static std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    повернути g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    повернути getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#include "plugin_parallel_api.hpp"

static
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    try
    {
        якщо (!handle)
            повернути CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        повернути CV_ERROR_OK;
    }
    catch (...)
    {
        повернути CV_ERROR_FAIL;
    }
}

static const OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        sizeof(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

const OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(ціле requested_abi_version, ціле requested_api_version, void* /*reserved=NULL*/) CV_NOEXCEPT
{
    якщо (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        повернути &plugin_api;
    повернути NULL;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
