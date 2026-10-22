// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

નામાકાશ cv {

unsigned default_સંખ્યા_Of_Threads();

શૂન્ય parallel_for_pthreads(અચલ Range& range, અચલ ParallelLoopBody& body, દ્વિગુણ nstripes);
size_t parallel_pthreads_get_threads_અંક();
શૂન્ય parallel_pthreads_set_threads_અંક(પૂર્ણાંક num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
