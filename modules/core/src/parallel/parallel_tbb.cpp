// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#অন্তর্ভাব "../precomp.hpp"

#অন্তর্ভাব "factory_parallel.hpp"

#ifdef HAVE_TBB

#অন্তর্ভাব "parallel.hpp"
#অন্তর্ভাব "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

নামাকাশ cv { নামাকাশ parallel {

স্থির
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    স্থির std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    ফেরত g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    ফেরত getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#অন্তর্ভাব "plugin_parallel_api.hpp"

স্থির
Cv_ফলাফল cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    চেষ্টা
    {
        যদি (!handle)
            ফেরত CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        ফেরত CV_ERROR_OK;
    }
    ধরো (...)
    {
        ফেরত CV_ERROR_FAIL;
    }
}

স্থির ধ্রুবক OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        আকার(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_VERSION) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ধ্রুবক OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(পূর্ণসংখ্যা requested_abi_version, পূর্ণসংখ্যা requested_api_version, শূন্য* /*reserved=NULL*/) CV_NOEXCEPT
{
    যদি (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        ফেরত &plugin_api;
    ফেরত ফাঁকা;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
