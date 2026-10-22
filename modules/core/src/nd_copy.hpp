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

નામાકાશ cv { નામાકાશ nd {

ગણના { MAX_VIEW_DIMS = CV_MAX_DIM };

સંરચના View
{
    uchar* data = nullptr;
    પૂર્ણાંક dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    પૂર્ણાંક size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(અચલ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(અચલ View& v, અચલ પૂર્ણાંક* order);

શૂન્ય flip(View& v, પૂર્ણાંક axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
શૂન્ય slice(View& v, પૂર્ણાંક axis, પૂર્ણાંક start, પૂર્ણાંક step, પૂર્ણાંક ગણતરી_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
શૂન્ય copy(અચલ View& src, અચલ View& dst);

// n independent copies executed in one parallel pass.
શૂન્ય copyBatch(અચલ View* src, અચલ View* dst, પૂર્ણાંક n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
પ્રકારવ્યાખ્યા શૂન્ય (*TransposeFunc)(અચલ uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
