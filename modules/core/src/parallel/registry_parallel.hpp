// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_PARALLEL_REGISTRY_HPP
#تعریف_کرو OPENCV_CORE_PARALLEL_REGISTRY_HPP

#شامل "factory_parallel.hpp"

نامخلا cv { نامخلا parallel {

ساخت ParallelBackendInfo
{
    صحیح ترجیح;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

ثابت std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#اگر_ختم // OPENCV_CORE_PARALLEL_REGISTRY_HPP
