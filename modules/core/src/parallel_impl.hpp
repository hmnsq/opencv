// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#جے_بنا_تعریف OPENCV_CORE_PARALLEL_IMPL_HPP
#تعریف_کرو OPENCV_CORE_PARALLEL_IMPL_HPP

نطاق cv {

بنانشانی default_نمبر_دا_دھاگے();

فراغ parallel_for_pthreads(ثابت Range& range, ثابت ParallelLoopBody& body, مزدوج nstripes);
size_t parallel_pthreads_get_دھاگے_عدد();
فراغ parallel_pthreads_سیٹ_دھاگے_عدد(صحيح num);

}

#جے_انت // OPENCV_CORE_PARALLEL_IMPL_HPP
