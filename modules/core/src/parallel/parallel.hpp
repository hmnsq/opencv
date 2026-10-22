// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#నిర్వచితం_కాకపోతే OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
#నిర్వచించు OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP

#అన్తర్భావ "opencv2/core/parallel/parallel_backend.hpp"

నామాకాశ cv { నామాకాశ parallel {

బాహ్య పూర్ణాంకం అంకె_దారాలు;

std::shared_ptr<ParallelForAPI>& getCurrentParallelForAPI();

#నిర్వచితం_కాకపోతే BUILD_PLUGIN

#నిర్వచితమైతే HAVE_TBB
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendTBB();
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_OPENMP
std::shared_ptr<cv::parallel::ParallelForAPI> createParallelBackendOpenMP();
#అయితే_ముగింపు

#అయితే_ముగింపు  // BUILD_PLUGIN

}}  // namespace

#అయితే_ముగింపు // OPENCV_CORE_SRC_PARALLEL_PARALLEL_HPP
