// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ifndef OPENCV_CORE_PARALLEL_IMPL_HPP
#define OPENCV_CORE_PARALLEL_IMPL_HPP

നാമാകാശ cv {

unsigned default_സംഖ്യ_Of_Threads();

ശൂന്യം parallel_for_pthreads(സ്ഥിരം Range& range, സ്ഥിരം ParallelLoopBody& body, ഇരട്ട nstripes);
size_t parallel_pthreads_get_threads_അക്കം();
ശൂന്യം parallel_pthreads_set_threads_അക്കം(പൂർണ്ണസംഖ്യ num);

}

#endif // OPENCV_CORE_PARALLEL_IMPL_HPP
