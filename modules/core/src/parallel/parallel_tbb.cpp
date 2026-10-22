// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ضمن "../precomp.hpp"

#ضمن "factory_parallel.hpp"

#جيڪڏهن_وصف_ٿيل HAVE_TBB

#ضمن "parallel.hpp"
#ضمن "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

نطاق cv { نطاق parallel {

ساكن
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    ساكن std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    أرجع g_instance;
}

#جيڪڏهن_وصف_نه_ٿيل BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    أرجع getInstance();
}
#جيڪڏهن_پورو

}}  // namespace

#جيڪڏهن_وصف_ٿيل BUILD_PLUGIN

#وصف_ڪريو ABI_نسخو 0
#وصف_ڪريو API_نسخو 0
#ضمن "plugin_parallel_api.hpp"

ساكن
Cv_نتيجو cv_getInstance(CV_ٻاهريون CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
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
        حجم(OpenCV_Core_Parallel_Plugin_API), ABI_نسخو, API_نسخو,
        CV_نسخو_MAJOR, CV_نسخو_MINOR, CV_نسخو_REVISION, CV_نسخو_حالت,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_نسخو) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

ثابت OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(صحيح requested_abi_نسخو, صحيح requested_api_نسخو, فراغ* /*reserved=NULL*/) CV_NOEXCEPT
{
    إذا (requested_abi_نسخو == ABI_نسخو && requested_api_نسخو <= API_نسخو)
        أرجع &plugin_api;
    أرجع عدم;
}

#جيڪڏهن_پورو  // BUILD_PLUGIN

#جيڪڏهن_پورو  // HAVE_TBB
