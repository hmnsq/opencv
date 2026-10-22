// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_PARALLEL_FACTORY_HPP
#define OPENCV_CORE_PARALLEL_FACTORY_HPP

#அந்தர்பாவ "opencv2/core/parallel/parallel_backend.hpp"

நாமாகாஶ cv { நாமாகாஶ parallel {

வகுப்பு IParallelBackendFactory
{
பொது:
    மெய்நிகர் ~IParallelBackendFactory() {}
    மெய்நிகர் std::shared_ptr<cv::parallel::ParallelForAPI> create() மாறிலி = 0;
};


வகுப்பு StaticBackendFactory CV_FINAL: பொது IParallelBackendFactory
{
பாதுகாக்கப்பட்ட:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(வெறுமை)> create_fn_;

பொது:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(வெறுமை)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() மாறிலி CV_OVERRIDE
    {
        திரும்பு create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(மாறிலி std::string& baseName);

}}  // namespace

#endif  // OPENCV_CORE_PARALLEL_FACTORY_HPP
