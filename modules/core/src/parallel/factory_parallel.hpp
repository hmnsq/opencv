// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_PARALLEL_FACTORY_HPP
#define OPENCV_CORE_PARALLEL_FACTORY_HPP

#অন্তর্ভাব "opencv2/core/parallel/parallel_backend.hpp"

নামাকাশ cv { নামাকাশ parallel {

শ্রেণী IParallelBackendFactory
{
সর্বজনীন:
    আভাসী ~IParallelBackendFactory() {}
    আভাসী std::shared_ptr<cv::parallel::ParallelForAPI> create() ধ্রুবক = 0;
};


শ্রেণী StaticBackendFactory CV_FINAL: সর্বজনীন IParallelBackendFactory
{
সুরক্ষিত:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(শূন্য)> create_fn_;

সর্বজনীন:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(শূন্য)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ধ্রুবক CV_OVERRIDE
    {
        ফেরত create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ধ্রুবক std::string& baseName);

}}  // namespace

#endif  // OPENCV_CORE_PARALLEL_FACTORY_HPP
