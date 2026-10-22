// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#અન્તર્ભાવ "../precomp.hpp"

#ifdef HAVE_OPENMP

#અન્તર્ભાવ "parallel.hpp"
#અન્તર્ભાવ "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

નામાકાશ cv { નામાકાશ parallel {

સ્થિર
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    સ્થિર std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    પાછા g_instance;
}

#ifndef BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    પાછા getInstance();
}
#endif

}}  // namespace

#ifdef BUILD_PLUGIN

#define ABI_VERSION 0
#define API_VERSION 0
#અન્તર્ભાવ "plugin_parallel_api.hpp"

સ્થિર
CvResult cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    પ્રયાસ
    {
        જો (!handle)
            પાછા CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        પાછા CV_ERROR_OK;
    }
    પકડો (...)
    {
        પાછા CV_ERROR_FAIL;
    }
}

સ્થિર અચલ OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        કદ(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

અચલ OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(પૂર્ણાંક requested_abi_version, પૂર્ણાંક requested_api_version, શૂન્ય* /*reserved=NULL*/) CV_NOEXCEPT
{
    જો (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        પાછા &plugin_api;
    પાછા ખાલી;
}

#endif  // BUILD_PLUGIN

#endif  // HAVE_TBB
