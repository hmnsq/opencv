// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_PARALLEL_REGISTRY_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_PARALLEL_REGISTRY_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "factory_parallel.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

ਬਣਤਰ ParallelBackendInfo
{
    ਪੂਰਨ ਤਰਜੀਹ;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

ਅਚਲ std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#ਜੇ_ਅੰਤ // OPENCV_CORE_PARALLEL_REGISTRY_HPP
