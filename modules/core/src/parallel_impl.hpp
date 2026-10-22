// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#اگر_تعریف_نشده OPENCV_CORE_PARALLEL_IMPL_HPP
#تعریف_کن OPENCV_CORE_PARALLEL_IMPL_HPP

نطاق cv {

بیعلامت default_عدد_از_نخها();

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);
size_t parallel_pthreads_get_نخها_رقم();
فراغ parallel_pthreads_مجموعه_نخها_رقم(صحيح num);

}

#پایان_اگر // OPENCV_CORE_PARALLEL_IMPL_HPP
