// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_PARALLEL_REGISTRY_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_PARALLEL_REGISTRY_HPP

#ಅನ್ತರ್ಭಾವ "factory_parallel.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ರಚನೆ ParallelBackendInfo
{
    ಪೂರ್ಣಾಂಕ ಆದ್ಯತೆ;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

ಸ್ಥಿರಾಂಕ std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#ಆದರೆ_ಅಂತ್ಯ // OPENCV_CORE_PARALLEL_REGISTRY_HPP
