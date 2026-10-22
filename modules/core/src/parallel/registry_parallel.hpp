// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef OPENCV_CORE_PARALLEL_REGISTRY_HPP
#define OPENCV_CORE_PARALLEL_REGISTRY_HPP

#अन्तर्भाव "factory_parallel.hpp"

नामाकाश cv { नामाकाश parallel {

संरचना ParallelBackendInfo
{
    पूर्णाङ्क priority;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

ध्रुव std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#endif // OPENCV_CORE_PARALLEL_REGISTRY_HPP
