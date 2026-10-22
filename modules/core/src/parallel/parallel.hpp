// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/parallel/parallel_backend.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ parallel {

ਬਾਹਰੀ ਪੂਰਨ ਅੰਕ_ਧਾਗੇ;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BUILD_PLUGIN

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ  // BUILD_PLUGIN

}}  // namespace

#ਜੇ_ਅੰਤ // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
