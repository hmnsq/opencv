// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#جے_بنا_تعریف OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#تعریف_کرو OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ضمن "opencv2/core/parallel/parallel_backend.hpp"

نطاق cv { نطاق parallel {

خارجي صحيح عدد_دھاگے;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#جے_بنا_تعریف BUILD_PLUGIN

#جے_تعریف_شدہ HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#جے_انت

#جے_تعریف_شدہ HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#جے_انت

#جے_انت  // BUILD_PLUGIN

}}  // namespace

#جے_انت // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
