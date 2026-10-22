// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अगर_अघोषित OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#घोषणा OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#समावेश "opencv2/core/parallel/parallel_backend.hpp"

namespace cv { namespace parallel {

बाह्य पूर्णांक अंक_Threads;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#अगर_अघोषित BUILD_PLUGIN

#अगर_घोषित HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#पूर्ण_अगर

#अगर_घोषित HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#पूर्ण_अगर

#पूर्ण_अगर  // BUILD_PLUGIN

}}  // namespace

#पूर्ण_अगर // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
