// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#यदि_अपरिभाषितम् OPENCV_CORE_PARALLEL_IMPL_HPP
#परिभाषय OPENCV_CORE_PARALLEL_IMPL_HPP

नामाकाश cv {

अचिह्नित default_सङ्ख्या_सम्बन्धि_तन्तवः();

शून्य parallel_for_pthreads(ध्रुव Range& range, ध्रुव ParallelLoopBody& body, द्विगुण nstripes);
size_t parallel_pthreads_get_तन्तवः_अङ्कः();
शून्य parallel_pthreads_समुच्चयः_तन्तवः_अङ्कः(पूर्णाङ्क num);

}

#यदि_अन्तः // OPENCV_CORE_PARALLEL_IMPL_HPP
