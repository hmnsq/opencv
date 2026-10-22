// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

नामाकाश cv {

अचिन्हित default_सङ्ख्या_Of_Threads();

शून्य parallel_for_pthreads(अचर Range& range, अचर ParallelLoopBody& body, द्विगुण nstripes);
माप_प्रकार parallel_pthreads_get_threads_अङ्क();
शून्य parallel_pthreads_set_threads_अङ्क(पूर्णाङ्क num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
