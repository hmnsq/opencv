// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_PARALLEL_REGISTRY_HPP
#وصف_ڪريو OPENCV_CORE_PARALLEL_REGISTRY_HPP

#ضمن "factory_parallel.hpp"

نطاق cv { نطاق parallel {

بنية ParallelBackendInfo
{
    صحيح ترجيح;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#جيڪڏهن_پورو // OPENCV_CORE_PARALLEL_REGISTRY_HPP
