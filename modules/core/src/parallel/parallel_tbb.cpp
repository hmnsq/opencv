// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ଅନ୍ତର୍ଭାଵ "../precomp.hpp"

#ଅନ୍ତର୍ଭାଵ "factory_parallel.hpp"

#ifdef HAVE_TBB

#ଅନ୍ତର୍ଭାଵ "parallel.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

ନାମାକାଶ cv { ନାମାକାଶ parallel {

ସ୍ଥିର
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ସ୍ଥିର std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    ଫେରନ୍ତୁ g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    ଫେରନ୍ତୁ getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#ଅନ୍ତର୍ଭାଵ "plugin_parallel_api.hpp"

ସ୍ଥିର
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ଚେଷ୍ଟା
    {
        ଯଦି (!handle)
            ଫେରନ୍ତୁ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ଫେରନ୍ତୁ CV_ERROR_OK;
    }
    ଧର (...)
    {
        ଫେରନ୍ତୁ CV_ERROR_FAIL;
    }
}

ସ୍ଥିର ଅଚଳ OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        ଆକାର(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_VERSION) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ଅଚଳ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_abi_version, ପୂର୍ଣ୍ଣସଂଖ୍ୟା requested_api_version, ଶୂନ୍ୟ* /*reserved=NULL*/) CV_NOEXCEPT
{
    ଯଦି (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        ଫେରନ୍ତୁ &plugin_api;
    ଫେରନ୍ତୁ ଖାଲି;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
