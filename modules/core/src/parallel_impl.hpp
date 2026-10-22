// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#که_نه_تعریف_شوی OPENCV_CORE_PARALLEL_IMPL_HPP
#تعریف_کړه OPENCV_CORE_PARALLEL_IMPL_HPP

نطاق cv {

بینښه default_شمېره_د_تارونه();

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);
size_t parallel_pthreads_get_تارونه_عدد();
فراغ parallel_pthreads_سېټ_تارونه_عدد(صحيح num);

}

#د_که_پای // OPENCV_CORE_PARALLEL_IMPL_HPP
