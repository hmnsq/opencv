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
    ᱨᱩᱣᱟᱹᱲ g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    ᱨᱩᱣᱟᱹᱲ getInstance();
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
        ᱡᱩᱫᱤ (!handle)
            ᱨᱩᱣᱟᱹᱲ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ᱨᱩᱣᱟᱹᱲ CV_ERROR_OK;
    }
    gr_haa_na (...)
    {
        ᱨᱩᱣᱟᱹᱲ CV_ERROR_FAIL;
    }
}

static const OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        parimaa_na(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

const OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(puur_naa_nka requested_abi_version, puur_naa_nka requested_api_version, void* /*reserved=NULL*/) CV_NOEXCEPT
{
    ᱡᱩᱫᱤ (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        ᱨᱩᱣᱟᱹᱲ &plugin_api;
    ᱨᱩᱣᱟᱹᱲ NULL;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
