// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

நாமாகாஶ cv {

unsigned default_எண்_Of_Threads();

வெறுமை parallel_for_pthreads(மாறிலி Range& range, மாறிலி ParallelLoopBody& body, இரட்டை nstripes);
size_t parallel_pthreads_get_threads_இலக்கம்();
வெறுமை parallel_pthreads_set_threads_இலக்கம்(முழு num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
