// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#اگر_غیر_تعریف_شدہ OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#تعریف_کرو OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#شامل "opencv2/core/parallel/parallel_backend.hpp"

نامخلا cv { نامخلا parallel {

خارجی صحیح عدد_دھاگے;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#اگر_غیر_تعریف_شدہ BUILD_PLUGIN

#اگر_تعریف_شدہ HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#اگر_ختم

#اگر_تعریف_شدہ HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#اگر_ختم

#اگر_ختم  // BUILD_PLUGIN

}}  // namespace

#اگر_ختم // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
