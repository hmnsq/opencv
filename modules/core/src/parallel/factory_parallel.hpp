// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#यदि_अपरिभाषितम् OPENCV_CORE_PARALLEL_FACTORY_HPP
#परिभाषय OPENCV_CORE_PARALLEL_FACTORY_HPP

#अन्तर्भाव "opencv2/core/parallel/parallel_backend.hpp"

नामाकाश cv { नामाकाश parallel {

वर्ग IParallelBackendFactory
{
सार्वजनिक:
    आभास ~IParallelBackendFactory() {}
    आभास std::shared_ptr<cv::parallel::ParallelForAPI> create() ध्रुव = 0;
};


वर्ग StaticBackendFactory CV_FINAL: सार्वजनिक IParallelBackendFactory
{
रक्षित:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(शून्य)> create_fn_;

सार्वजनिक:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(शून्य)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ध्रुव CV_OVERRIDE
    {
        प्रत्यागम create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ध्रुव std::string& आधारः_Name);

}}  // namespace

#यदि_अन्तः  // OPENCV_CORE_PARALLEL_FACTORY_HPP
