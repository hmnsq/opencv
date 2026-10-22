// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"

#ضمن "factory_parallel.hpp"

#جے_تعریف_شدہ HAVE_TBB

#ضمن "parallel.hpp"
#ضمن "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

نطاق cv { نطاق parallel {

ساكن
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ساكن std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    أرجع g_instance;
}

#جے_بنا_تعریف BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    أرجع getInstance();
}
#جے_انت

}}  // namespace

#جے_تعریف_شدہ BUILD_PLUGIN

#تعریف_کرو ABI_نسخہ 0
#تعریف_کرو API_نسخہ 0
#ضمن "plugin_parallel_api.hpp"

ساكن
Cv_نتیجہ cv_getInstance(CV_باہرلا CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    حاول
    {
        إذا (!handle)
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
        حجم(OpenCV_Core_Parallel_Plugin_API), ABI_نسخہ, API_نسخہ,
        CV_نسخہ_MAJOR, CV_نسخہ_MINOR, CV_نسخہ_REVISION, CV_نسخہ_حالت,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_نسخہ) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحيح requested_abi_نسخہ, صحيح requested_api_نسخہ, فراغ* /*reserved=NULL*/) CV_NOEXCEPT
{
    إذا (requested_abi_نسخہ == ABI_نسخہ && requested_api_نسخہ <= API_نسخہ)
        أرجع &plugin_api;
    أرجع عدم;
}

#جے_انت  // BUILD_PLUGIN

#جے_انت  // HAVE_TBB
