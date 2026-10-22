// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

नामाकाश cv {

अचिन्हित default_संख्या_Of_Threads();

शून्य parallel_for_pthreads(स्थिरांक Range& range, स्थिरांक ParallelLoopBody& body, द्विगुण nstripes);
माप_प्रकार parallel_pthreads_get_threads_अंक();
शून्य parallel_pthreads_set_threads_अंक(पूर्णांक num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
