// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#ಅನ್ತರ್ಭಾವ "opencv2/core/parallel/parallel_backend.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ parallel {

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ಅಂಕೆ_ಎಳೆಗಳು;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BUILD_PLUGIN

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ  // BUILD_PLUGIN

}}  // namespace

#ಆದರೆ_ಅಂತ್ಯ // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
