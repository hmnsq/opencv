// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#అన్తర్భావ "../precomp.hpp"

#అన్తర్భావ "factory_parallel.hpp"

#నిర్వచితమైతే HAVE_TBB

#అన్తర్భావ "parallel.hpp"
#అన్తర్భావ "opencv2/core/parallel/backend/parallel_for.tbb.hpp"

నామాకాశ cv { నామాకాశ parallel {

స్థిర
std::shared_ptr<cv::parallel::tbb::ParallelForBackend>& getInstance()
{
    స్థిర std::shared_ptr<cv::parallel::tbb::ParallelForBackend> g_instance = std::make_shared<cv::parallel::tbb::ParallelForBackend>();
    తిరిగి g_instance;
}

#నిర్వచితం_కాకపోతే BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB()
{
    తిరిగి getInstance();
}
#అయితే_ముగింపు

}}  // namespace

#నిర్వచితమైతే BUILD_PLUGIN

#నిర్వచించు ABI_సంస్కరణ 0
#నిర్వచించు API_సంస్కరణ 0
#అన్తర్భావ "plugin_parallel_api.hpp"

స్థిర
Cv_ఫలితం cv_getInstance(CV_బహిర్గతం CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    ప్రయత్నించు
    {
        ఒకవేళ (!handle)
            తిరిగి CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().get();
        తిరిగి CV_ERROR_OK;
    }
    పట్టుకో (...)
    {
        తిరిగి CV_ERROR_FAIL;
    }
}

స్థిర స్థిరరాశి OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        పరిమాణం(OpenCV_Core_Parallel_Plugin_API), ABI_సంస్కరణ, API_సంస్కరణ,
        CV_సంస్కరణ_MAJOR, CV_సంస్కరణ_MINOR, CV_సంస్కరణ_REVISION, CV_సంస్కరణ_స్థితి,
        "TBB (interface " CVAUX_STR(TBB_INTERFACE_సంస్కరణ) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

స్థిరరాశి OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(పూర్ణాంకం requested_abi_సంస్కరణ, పూర్ణాంకం requested_api_సంస్కరణ, శూన్య* /*reserved=NULL*/) CV_NOEXCEPT
{
    ఒకవేళ (requested_abi_సంస్కరణ == ABI_సంస్కరణ && requested_api_సంస్కరణ <= API_సంస్కరణ)
        తిరిగి &plugin_api;
    తిరిగి ఖాళీ;
}

#అయితే_ముగింపు  // BUILD_PLUGIN

#అయితే_ముగింపు  // HAVE_TBB
