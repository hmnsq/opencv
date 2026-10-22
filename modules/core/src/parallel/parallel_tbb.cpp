// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अन्तर्भाव "../precomp.hpp"

#अन्तर्भाव "factory_parallel.hpp"

#ifdef HAVE_TBB

#अन्तर्भाव "parallel.hpp"
#अन्तर्भाव "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

नामाकाश cv { नामाकाश parallel {

स्थिर
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    स्थिर std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    पटिनिवत्त g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    पटिनिवत्त getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#अन्तर्भाव "plugin_parallel_api.hpp"

स्थिर
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    यत्न
    {
        सचे (!handle)
            पटिनिवत्त CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        पटिनिवत्त CV_ERROR_OK;
    }
    गृहाण (...)
    {
        पटिनिवत्त CV_ERROR_FAIL;
    }
}

स्थिर ध्रुव OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        परिमाण(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_VERSION) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ध्रुव OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(पूर्णाङ्क requested_abi_version, पूर्णाङ्क requested_api_version, शून्य* /*reserved=NULL*/) CV_NOEXCEPT
{
    सचे (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        पटिनिवत्त &plugin_api;
    पटिनिवत्त रिक्त;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
