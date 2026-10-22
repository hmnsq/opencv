// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

نطاق cv {

unsigned default_نومبور_Of_Threads();

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);
size_t parallel_pthreads_get_threads_اڠک();
فراغ parallel_pthreads_set_threads_اڠک(صحيح num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
