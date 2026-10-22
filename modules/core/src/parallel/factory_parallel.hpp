// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#నిర్వచితం_కాకపోతే OPENCV_CORE_PARALLEL_FACTORY_HPP
#నిర్వచించు OPENCV_CORE_PARALLEL_FACTORY_HPP

#అన్తర్భావ "opencv2/core/parallel/parallel_backend.hpp"

నామాకాశ cv { నామాకాశ parallel {

తరగతి IParallelBackendFactory
{
బహిరంగ:
    మాయా ~IParallelBackendFactory() {}
    మాయా std::shared_ptr<cv::parallel::ParallelForAPI> create() స్థిరరాశి = 0;
};


తరగతి StaticBackendFactory CV_FINAL: బహిరంగ IParallelBackendFactory
{
రక్షిత:
    std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(శూన్య)> create_fn_;

బహిరంగ:
    StaticBackendFactory(std::function<std::shared_ptr<cv::parallel::ParallelForAPI>(శూన్య)>&& create_fn)
        : create_fn_(create_fn)
    {
        // nothing
    }

    ~StaticBackendFactory() CV_OVERRIDE {}

    std::shared_ptr<cv::parallel::ParallelForAPI> create() స్థిరరాశి CV_OVERRIDE
    {
        తిరిగి create_fn_();
    }
};

//
// PluginBackendFactory is implemented in plugin_wrapper.cpp
//

std::shared_ptr<IParallelBackendFactory> createPluginParallelBackendFactory(స్థిరరాశి std::string& పునాది_Name);

}}  // namespace

#అయితే_ముగింపు  // OPENCV_CORE_PARALLEL_FACTORY_HPP
