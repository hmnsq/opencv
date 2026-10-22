// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#جيڪڏهن_وصف_نه_ٿيل OPENCV_CORE_PARALLEL_IMPL_HPP
#وصف_ڪريو OPENCV_CORE_PARALLEL_IMPL_HPP

نطاق cv {

بنانشان default_عدد_جو_ڌاڳا();

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);
size_t parallel_pthreads_get_ڌاڳا_انگ();
فراغ parallel_pthreads_سيٽ_ڌاڳا_انگ(صحيح num);

}

#جيڪڏهن_پورو // OPENCV_CORE_PARALLEL_IMPL_HPP
