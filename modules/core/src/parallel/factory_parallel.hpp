// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_PARALLEL_FACTORY_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_PARALLEL_FACTORY_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ವರ್ಗ IParallelBackendFactory
{
ಸಾರ್ವಜನಿಕ:
    ವಾಸ್ತವಿಕ ~IParallelBackendFactory() {}
    ವಾಸ್ತವಿಕ std::shared_ptr<cv::parallel::ParallelForAPI> create() ಸ್ಥಿರಾಂಕ = 0;
};


ವರ್ಗ StaticBackendFactory CV_FINAL: ಸಾರ್ವಜನಿಕ IParallelBackendFactory
{
ಸಂರಕ್ಷಿತ:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ಶೂನ್ಯ)> create_fn_;

ಸಾರ್ವಜನಿಕ:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ಶೂನ್ಯ)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ಸ್ಥಿರಾಂಕ CV_OVERRIDE
    {
        ಹಿಂತಿರುಗಿ create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ಸ್ಥಿರಾಂಕ std::string& ತಳ_Name);

}}  // namespace

#ಆದರೆ_ಅಂತ್ಯ  // OPENCV_CORE_PARALLEL_FACTORY_HPP
