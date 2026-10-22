// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#अगर_अघोषित OPENCV_CORE_PARALLEL_REGISTRY_HPP
#घोषणा OPENCV_CORE_PARALLEL_REGISTRY_HPP

#समावेश "factory_parallel.hpp"

namespace cv { namespace parallel {

काष्ठा ParallelBackendInfo
{
    पूर्णांक priority;     // 1000-<index*10> - default builtin priority
                      // 0 - disabled (OPENCV_PARALLEL_PRIORITY_<name> = 0)
                      // >10000 - prioritized list (OPENCV_PARALLEL_PRIORITY_LIST)
    std::string name;
    std::shared_ptr<IParallelBackendFactory> backendFactory;
};

स्थिर std::vector<ParallelBackendInfo>& getParallelBackendsInfo();

}} // namespace

#पूर्ण_अगर // OPENCV_CORE_PARALLEL_REGISTRY_HPP
