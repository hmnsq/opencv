// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_PARALLEL_FACTORY_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_PARALLEL_FACTORY_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/parallel/parallel_backend.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

ਵਰਗ IParallelBackendFactory
{
ਜਨਤਕ:
    ਆਭਾਸੀ ~IParallelBackendFactory() {}
    ਆਭਾਸੀ std::shared_ptr<cv::parallel::ParallelForAPI> create() ਅਚਲ = 0;
};


ਵਰਗ StaticBackendFactory CV_FINAL: ਜਨਤਕ IParallelBackendFactory
{
ਸੁਰੱਖਿਅਤ:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ਖਾਲੀ)> create_fn_;

ਜਨਤਕ:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(ਖਾਲੀ)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() ਅਚਲ CV_OVERRIDE
    {
        ਵਾਪਸ create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(ਅਚਲ std::string& ਅਧਾਰ_Name);

}}  // namespace

#ਜੇ_ਅੰਤ  // OPENCV_CORE_PARALLEL_FACTORY_HPP
