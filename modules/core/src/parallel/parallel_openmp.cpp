// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#شامل "../precomp.hpp"

#اگر_تعریف_شدہ HAVE_OPENMP

#شامل "parallel.hpp"
#شامل "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

نامخلا cv { نامخلا parallel {

ساکن
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    ساکن std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    واپس g_instance;
}

#اگر_غیر_تعریف_شدہ BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    واپس getInstance();
}
#اگر_ختم

}}  // namespace

#اگر_تعریف_شدہ BUILD_PLUGIN

#تعریف_کرو ABI_نسخہ 0
#تعریف_کرو API_نسخہ 0
#شامل "plugin_parallel_api.hpp"

ساکن
Cv_نتیجہ cv_getInstance(CV_برآمد CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    کوشش
    {
        اگر (!handle)
            واپس CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        واپس CV_ERROR_OK;
    }
    پکڑو (...)
    {
        واپس CV_ERROR_FAIL;
    }
}

ساکن ثابت OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        حجم(OpenCV_Core_Parallel_Plugin_API), ABI_نسخہ, API_نسخہ,
        CV_نسخہ_MAJOR, CV_نسخہ_MINOR, CV_نسخہ_REVISION, CV_نسخہ_حالت,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحیح requested_abi_نسخہ, صحیح requested_api_نسخہ, خالی* /*reserved=NULL*/) CV_NOEXCEPT
{
    اگر (requested_abi_نسخہ == ABI_نسخہ && requested_api_نسخہ <= API_نسخہ)
        واپس &plugin_api;
    واپس تہی;
}

#اگر_ختم  // BUILD_PLUGIN

#اگر_ختم  // HAVE_TBB
