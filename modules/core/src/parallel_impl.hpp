// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

ನಾಮಾಕಾಶ cv {

unsigned default_ಸಂಖ್ಯೆ_Of_Threads();

ಶೂನ್ಯ parallel_for_pthreads(ಧ್ರುವ Range& range, ಧ್ರುವ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes);
size_t parallel_pthreads_get_threads_ಅಂಕೆ();
ಶೂನ್ಯ parallel_pthreads_set_threads_ಅಂಕೆ(ಪೂರ್ಣಾಙ್ಕ num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
