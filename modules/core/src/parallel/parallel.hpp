// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#وصف_ڪريو OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

نطاق cv { نطاق parallel {

خارجي صحيح انگ_ڌاڳا;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#جيڪڏهن_وصف_نه_ٿيل BUILD_PLUGIN

#جيڪڏهن_وصف_ٿيل HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#جيڪڏهن_پورو

#جيڪڏهن_وصف_ٿيل HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#جيڪڏهن_پورو

#جيڪڏهن_پورو  // BUILD_PLUGIN

}}  // namespace

#جيڪڏهن_پورو // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
