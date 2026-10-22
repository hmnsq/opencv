// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

নামাকাশ cv {

unsigned default_সংখ্যা_Of_Threads();

শূন্য parallel_for_pthreads(ধ্রুৱ Range& range, ধ্রুৱ ParallelLoopBody& body, দ্ৱিগুণ nstripes);
size_t parallel_pthreads_get_threads_অঙ্ক();
শূন্য parallel_pthreads_set_threads_অঙ্ক(পূর্ণাঙ্ক num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
