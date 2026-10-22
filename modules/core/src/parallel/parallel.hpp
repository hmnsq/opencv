// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#اگر_تعریف_نشده OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#تعریف_کن OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

نطاق cv { نطاق parallel {

خارجي صحيح رقم_نخها;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#اگر_تعریف_نشده BUILD_PLUGIN

#اگر_تعریف_شده HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#پایان_اگر

#اگر_تعریف_شده HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#پایان_اگر

#پایان_اگر  // BUILD_PLUGIN

}}  // namespace

#پایان_اگر // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
