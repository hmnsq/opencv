// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_PARALLEL_FACTORY_HPP
#define OPENCV_CORE_PARALLEL_FACTORY_HPP

#അന്തര്ഭാവ "opencv2/core/parallel/parallel_backend.hpp"

നാമാകാശ cv { നാമാകാശ parallel {

വർഗ്ഗം IParallelBackendFactory
{
പൊതു:
    മായാ ~IParallelBackendFactory() {}
    മായാ std::shared_ptr<cv::parallel::ParallelForAPI> create() സ്ഥിരം = 0;
};


വർഗ്ഗം StaticBackendFactory CV_FINAL: പൊതു IParallelBackendFactory
{
സംരക്ഷിത:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ശൂന്യം)> create_fn_;

പൊതു:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ശൂന്യം)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() സ്ഥിരം CV_OVERRIDE
    {
        തിരികെ create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(സ്ഥിരം std::string& baseName);

}}  // namespace

#endif  // OPENCV_CORE_PARALLEL_FACTORY_HPP
