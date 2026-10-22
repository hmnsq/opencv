// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#अगर_अघोषित OPENCV_CORE_PARALLEL_IMPL_HPP
#घोषणा OPENCV_CORE_PARALLEL_IMPL_HPP

namespace cv {

अचिन्हित default_संख्या_Of_Threads();

व्योम parallel_for_pthreads(स्थिर Range& range, स्थिर ParallelLoopBody& body, द्विगुन nstripes);
माप_प्रकार parallel_pthreads_get_threads_अंक();
व्योम parallel_pthreads_set_threads_अंक(पूर्णांक num);

}

#पूर्ण_अगर // OPENCV_CORE_PARALLEL_IMPL_HPP
