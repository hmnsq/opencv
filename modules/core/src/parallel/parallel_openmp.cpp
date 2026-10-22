// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#অন্তর্ভাব "../precomp.hpp"

#ifdef HAVE_OPENMP

#অন্তর্ভাব "parallel.hpp"
#অন্তর্ভাব "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

নামাকাশ cv { নামাকাশ parallel {

স্থির
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    স্থির std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    প্রত্যাগম g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    প্রত্যাগম getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#অন্তর্ভাব "plugin_parallel_api.hpp"

স্থির
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    যত্ন
    {
        যদি (!handle)
            প্রত্যাগম CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        প্রত্যাগম CV_ERROR_OK;
    }
    গৃহাণ (...)
    {
        প্রত্যাগম CV_ERROR_FAIL;
    }
}

স্থির ধ্রুব OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        পরিমাণ(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ধ্রুব OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(পূর্ণাঙ্ক requested_abi_version, পূর্ণাঙ্ক requested_api_version, শূন্য* /*reserved=NULL*/) CV_NOEXCEPT
{
    যদি (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        প্রত্যাগম &plugin_api;
    প্রত্যাগম রিক্ত;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
