// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

ନାମାକାଶ cv {

unsigned default_ସଂଖ୍ୟା_Of_Threads();

ଶୂନ୍ୟ parallel_for_pthreads(ଅଚଳ Range& range, ଅଚଳ ParallelLoopBody& body, ଦ୍ୱିଗୁଣ nstripes);
size_t parallel_pthreads_get_threads_ଅଙ୍କ();
ଶୂନ୍ୟ parallel_pthreads_set_threads_ଅଙ୍କ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
