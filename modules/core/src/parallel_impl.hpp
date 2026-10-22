// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_PARALLEL_IMPL_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_PARALLEL_IMPL_HPP

ನಾಮಾಕಾಶ cv {

ಚಿಹ್ನೆರಹಿತ default_ಸಂಖ್ಯೆ_ದ_ಎಳೆಗಳು();

ಶೂನ್ಯ parallel_for_pthreads(ಸ್ಥಿರಾಂಕ Range& range, ಸ್ಥಿರಾಂಕ ParallelLoopBody& body, ದ್ವಿಗುಣ nstripes);
size_t parallel_pthreads_get_ಎಳೆಗಳು_ಅಂಕೆ();
ಶೂನ್ಯ parallel_pthreads_ಗಣ_ಎಳೆಗಳು_ಅಂಕೆ(ಪೂರ್ಣಾಂಕ num);

}

#ಆದರೆ_ಅಂತ್ಯ // OPENCV_CORE_PARALLEL_IMPL_HPP
