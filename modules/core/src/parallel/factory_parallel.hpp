// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_PARALLEL_FACTORY_HPP
#تعریف_کرو OPENCV_CORE_PARALLEL_FACTORY_HPP

#شامل "opencv2/core/parallel/parallel_backend.hpp"

نامخلا cv { نامخلا parallel {

جماعت IParallelBackendFactory
{
عوامی:
    مجازی ~IParallelBackendFactory() {}
    مجازی std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت = 0;
};


جماعت StaticBackendFactory CV_FINAL: عوامی IParallelBackendFactory
{
محفوظ:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(خالی)> create_fn_;

عوامی:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(خالی)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ثابت CV_OVERRIDE
    {
        واپس create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ثابت std::string& بنیاد_Name);

}}  // namespace

#اگر_ختم  // OPENCV_CORE_PARALLEL_FACTORY_HPP
