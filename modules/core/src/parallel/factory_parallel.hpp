// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अगर_अघोषित OPENCV_CORE_PARALLEL_FACTORY_HPP
#घोषणा OPENCV_CORE_PARALLEL_FACTORY_HPP

#समावेश "opencv2/core/parallel/parallel_backend.hpp"

namespace cv { namespace parallel {

श्रेणी IParallelBackendFactory
{
खुला:
    भव ~IParallelBackendFactory() {}
    भव std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिर = 0;
};


श्रेणी StaticBackendFactory CV_FINAL: खुला IParallelBackendFactory
{
रक्षित:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(व्योम)> create_fn_;

खुला:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(व्योम)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() स्थिर CV_OVERRIDE
    {
        वापस create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(स्थिर std::string& baseName);

}}  // namespace

#पूर्ण_अगर  // OPENCV_CORE_PARALLEL_FACTORY_HPP
