// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_PARALLEL_IMPL_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_PARALLEL_IMPL_HPP

ਨਾਮਾਕਾਸ਼ cv {

ਬਿਨਾਨਿਸ਼ਾਨੀ default_ਸੰਖਿਆ_ਦਾ_ਧਾਗੇ();

ਖਾਲੀ parallel_for_pthreads(ਅਚਲ Range& range, ਅਚਲ ParallelLoopBody& body, ਦੁੱਗਣਾ nstripes);
size_t parallel_pthreads_get_ਧਾਗੇ_ਅੰਕ();
ਖਾਲੀ parallel_pthreads_ਸੈੱਟ_ਧਾਗੇ_ਅੰਕ(ਪੂਰਨ num);

}

#ਜੇ_ਅੰਤ // OPENCV_CORE_PARALLEL_IMPL_HPP
