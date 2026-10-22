// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_PARALLEL_FACTORY_HPP
#define OPENCV_CORE_PARALLEL_FACTORY_HPP

#ଅନ୍ତର୍ଭାଵ "opencv2/core/parallel/parallel_backend.hpp"

ନାମାକାଶ cv { ନାମାକାଶ parallel {

ଶ୍ରେଣୀ IParallelBackendFactory
{
ସର୍ବସାଧାରଣ:
    ଆଭାସୀ ~IParallelBackendFactory() {}
    ଆଭାସୀ std::shared_ptr<cv::parallel::ParallelForAPI> create() ଅଚଳ = 0;
};


ଶ୍ରେଣୀ StaticBackendFactory CV_FINAL: ସର୍ବସାଧାରଣ IParallelBackendFactory
{
ସୁରକ୍ଷିତ:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ଶୂନ୍ୟ)> create_fn_;

ସର୍ବସାଧାରଣ:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ଶୂନ୍ୟ)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ଅଚଳ CV_OVERRIDE
    {
        ଫେରନ୍ତୁ create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ଅଚଳ std::string& baseName);

}}  // namespace

#endif  // OPENCV_CORE_PARALLEL_FACTORY_HPP
