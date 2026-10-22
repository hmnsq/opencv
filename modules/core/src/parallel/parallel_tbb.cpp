// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ਅਨ੍ਤਰ੍ਭਾਵ "../precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "factory_parallel.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_TBB

#ਅਨ੍ਤਰ੍ਭਾਵ "parallel.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

ਸਥਿਰ
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ਸਥਿਰ std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    ਵਾਪਸ g_instance;
}

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    ਵਾਪਸ getInstance();
}
#ਜੇ_ਅੰਤ

}}  // namespace

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ BUILD_PLUGIN

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ABI_ਸੰਸਕਰਣ 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ API_ਸੰਸਕਰਣ 0
#ਅਨ੍ਤਰ੍ਭਾਵ "plugin_parallel_api.hpp"

ਸਥਿਰ
Cv_ਨਤੀਜਾ cv_getInstance(CV_ਬਾਹਰਲਾ CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ਕੋਸ਼ਿਸ਼
    {
        ਜੇ (!handle)
            ਵਾਪਸ CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ਵਾਪਸ CV_ERROR_OK;
    }
    ਫੜੋ (...)
    {
        ਵਾਪਸ CV_ERROR_FAIL;
    }
}

ਸਥਿਰ ਅਚਲ OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        ਆਕਾਰ(OpenCV_Core_Parallel_Plugin_API), ABI_ਸੰਸਕਰਣ, API_ਸੰਸਕਰਣ,
        CV_ਸੰਸਕਰਣ_MAJOR, CV_ਸੰਸਕਰਣ_MINOR, CV_ਸੰਸਕਰਣ_REVISION, CV_ਸੰਸਕਰਣ_ਸਥਿਤੀ,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_ਸੰਸਕਰਣ) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ਅਚਲ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(ਪੂਰਨ requested_abi_ਸੰਸਕਰਣ, ਪੂਰਨ requested_api_ਸੰਸਕਰਣ, ਖਾਲੀ* /*reserved=NULL*/) CV_NOEXCEPT
{
    ਜੇ (requested_abi_ਸੰਸਕਰਣ == ABI_ਸੰਸਕਰਣ && requested_api_ਸੰਸਕਰਣ <= API_ਸੰਸਕਰਣ)
        ਵਾਪਸ &plugin_api;
    ਵਾਪਸ ਨਿਰਾ;
}

#ਜੇ_ਅੰਤ  // BUILD_PLUGIN

#ਜੇ_ਅੰਤ  // HAVE_TBB
