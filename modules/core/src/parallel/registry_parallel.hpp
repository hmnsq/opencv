// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#నిర్వచితం_కాకపోతే OPENCV_CORE_PARALLEL_REGISTRY_HPP
#నిర్వచించు OPENCV_CORE_PARALLEL_REGISTRY_HPP

#అన్తర్భావ "factory_parallel.hpp"

నామాకాశ cv { నామాకాశ parallel {

నిర్మాణం ParallelBackendInfo
{
    పూర్ణాంకం ప్రాధాన్యత;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

స్థిరరాశి std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#అయితే_ముగింపు // OPENCV_CORE_PARALLEL_REGISTRY_HPP
