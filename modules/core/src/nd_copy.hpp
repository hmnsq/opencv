// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#ifndef OPENCV_CORE_ND_COPY_HPP
#define OPENCV_CORE_ND_COPY_HPP

नामाकाश cv { नामाकाश nd {

गणना { MAX_VIEW_DIMS = CV_MAX_DIM };

संरचना View
{
    uchar* data = nullptr;
    पूर्णाङ्क dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    पूर्णाङ्क size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ध्रुव Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ध्रुव View& v, ध्रुव पूर्णाङ्क* order);

शून्य flip(View& v, पूर्णाङ्क axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
शून्य slice(View& v, पूर्णाङ्क axis, पूर्णाङ्क start, पूर्णाङ्क step, पूर्णाङ्क गणनम्_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
शून्य copy(ध्रुव View& src, ध्रुव View& dst);

// n independent copies executed in one parallel pass.
शून्य copyBatch(ध्रुव View* src, ध्रुव View* dst, पूर्णाङ्क n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
प्रकारनिर्देश शून्य (*TransposeFunc)(ध्रुव uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
