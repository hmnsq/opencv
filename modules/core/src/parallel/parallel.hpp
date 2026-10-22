// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#यदि_अपरिभाषितम् OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#परिभाषय OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#अन्तर्भाव "opencv2/core/parallel/parallel_backend.hpp"

नामाकाश cv { नामाकाश parallel {

बाह्य पूर्णाङ्क अङ्कः_तन्तवः;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#यदि_अपरिभाषितम् BUILD_PLUGIN

#यदि_परिभाषितम् HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#यदि_अन्तः

#यदि_अन्तः  // BUILD_PLUGIN

}}  // namespace

#यदि_अन्तः // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
