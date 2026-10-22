// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#که_نه_تعریف_شوی OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#تعریف_کړه OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

نطاق cv { نطاق parallel {

خارجي صحيح عدد_تارونه;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#که_نه_تعریف_شوی BUILD_PLUGIN

#که_تعریف_شوی HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#د_که_پای

#که_تعریف_شوی HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#د_که_پای

#د_که_پای  // BUILD_PLUGIN

}}  // namespace

#د_که_پای // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
