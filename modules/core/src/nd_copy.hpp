// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#अगर_अघोषित OPENCV_CORE_ND_COPY_HPP
#घोषणा OPENCV_CORE_ND_COPY_HPP

namespace cv { namespace nd {

क्रमागत { MAX_VIEW_DIMS = CV_MAX_DIM };

काष्ठा View
{
    uchar* तथ्य = nullptr;
    पूर्णांक dims = 0;
    माप_प्रकार esz = 0;                    // element size in bytes, channels included
    पूर्णांक size[MAX_VIEW_DIMS];
    सूचक_भेद_प्रकार step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(स्थिर Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(स्थिर View& v, स्थिर पूर्णांक* order);

व्योम flip(View& v, पूर्णांक axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
व्योम slice(View& v, पूर्णांक axis, पूर्णांक start, पूर्णांक step, पूर्णांक गिनती_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
व्योम copy(स्थिर View& src, स्थिर View& dst);

// n independent copies executed in one parallel pass.
व्योम copyBatch(स्थिर View* src, स्थिर View* dst, पूर्णांक n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
प्रकार व्योम (*TransposeFunc)(स्थिर uchar* src, माप_प्रकार sstep, uchar* dst, माप_प्रकार dstep, आकार sz);
TransposeFunc getTransposeFunc(माप_प्रकार esz);

}} // namespace cv::nd

#पूर्ण_अगर
