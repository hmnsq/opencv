// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"

#ضمن "factory_parallel.hpp"

#که_تعریف_شوی HAVE_TBB

#ضمن "parallel.hpp"
#ضمن "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

نطاق cv { نطاق parallel {

ساكن
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ساكن std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    أرجع g_instance;
}

#که_نه_تعریف_شوی BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    أرجع getInstance();
}
#د_که_پای

}}  // namespace

#که_تعریف_شوی BUILD_PLUGIN

#تعریف_کړه ABI_نسخه 0
#تعریف_کړه API_نسخه 0
#ضمن "plugin_parallel_api.hpp"

ساكن
Cv_پایله cv_getInstance(CV_بهر CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
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
        حجم(OpenCV_Core_Parallel_Plugin_API), ABI_نسخه, API_نسخه,
        CV_نسخه_MAJOR, CV_نسخه_MINOR, CV_نسخه_REVISION, CV_نسخه_حالت,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_نسخه) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحيح requested_abi_نسخه, صحيح requested_api_نسخه, فراغ* /*reserved=NULL*/) CV_NOEXCEPT
{
    إذا (requested_abi_نسخه == ABI_نسخه && requested_api_نسخه <= API_نسخه)
        أرجع &plugin_api;
    أرجع عدم;
}

#د_که_پای  // BUILD_PLUGIN

#د_که_پای  // HAVE_TBB
