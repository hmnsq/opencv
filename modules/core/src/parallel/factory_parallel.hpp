// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_تعریف_نشده OPENCV_CORE_PARALLEL_FACTORY_HPP
#تعریف_کن OPENCV_CORE_PARALLEL_FACTORY_HPP

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

نطاق cv { نطاق parallel {

صنف IParallelBackendFactory
{
عام:
    مجازی ~IParallelBackendFactory() {}
    مجازی std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت = 0;
};


صنف StaticBackendFactory CV_FINAL: عام IParallelBackendFactory
{
محمي:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(فراغ)> create_fn_;

عام:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(فراغ)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        أرجع create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& پایه_Name);

}}  // namespace

#پایان_اگر  // OPENCV_CORE_PARALLEL_FACTORY_HPP
