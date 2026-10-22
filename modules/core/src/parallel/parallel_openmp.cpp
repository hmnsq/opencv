// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#समावेश "../precomp.hpp"

#अगर_घोषित HAVE_OPENMP

#समावेश "parallel.hpp"
#समावेश "opencv2/core/parallel/backend/parallel_for.openmp.hpp"

namespace cv { namespace parallel {

अटल
std::shared_ptr<cv::parallel::openmp::ParallelForBackend>& getInstance()
{
    अटल std::shared_ptr<cv::parallel::openmp::ParallelForBackend> g_instance = std::make_shared<cv::parallel::openmp::ParallelForBackend>();
    वापस g_instance;
}

#अगर_अघोषित BUILD_PLUGIN
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP()
{
    वापस getInstance();
}
#पूर्ण_अगर

}}  // namespace

#अगर_घोषित BUILD_PLUGIN

#घोषणा ABI_VERSION 0
#घोषणा API_VERSION 0
#समावेश "plugin_parallel_api.hpp"

अटल
Cv_परिणाम cv_getInstance(CV_OUT CvPluginParallelBackendAPI* handle) CV_NOEXCEPT
{
    प्रयास
    {
        अगर (!handle)
            वापस CV_ERROR_FAIL;
        *handle = cv::parallel::getInstance().लो();
        वापस CV_ERROR_OK;
    }
    पकड़ो (...)
    {
        वापस CV_ERROR_FAIL;
    }
}

अटल स्थिर OpenCV_Core_Parallel_Plugin_API plugin_api =
{
    {
        माप(OpenCV_Core_Parallel_Plugin_API), ABI_VERSION, API_VERSION,
        CV_VERSION_MAJOR, CV_VERSION_MINOR, CV_VERSION_REVISION, CV_VERSION_STATUS,
        "OpenMP (" CVAUX_STR(_OPENMP) ") OpenCV parallel plugin"
    },
    {
        /*  1*/cv_getInstance
    }
};

स्थिर OpenCV_Core_Parallel_Plugin_API* CV_API_CALL opencv_core_parallel_plugin_init_v0(पूर्णांक requested_abi_version, पूर्णांक requested_api_version, व्योम* /*reserved=NULL*/) CV_NOEXCEPT
{
    अगर (requested_abi_version == ABI_VERSION && requested_api_version <= API_VERSION)
        वापस &plugin_api;
    वापस शून्य;
}

#पूर्ण_अगर  // BUILD_PLUGIN

#पूर्ण_अगर  // HAVE_TBB
