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

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ nd {

ಗಣನಾ { MAX_VIEW_DIMS = CV_MAX_DIM };

ಸಂರಚನಾ View
{
    uchar* data = nullptr;
    ಪೂರ್ಣಾಙ್ಕ dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    ಪೂರ್ಣಾಙ್ಕ size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ಧ್ರುವ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ಧ್ರುವ View& v, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* order);

ಶೂನ್ಯ flip(View& v, ಪೂರ್ಣಾಙ್ಕ axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
ಶೂನ್ಯ slice(View& v, ಪೂರ್ಣಾಙ್ಕ axis, ಪೂರ್ಣಾಙ್ಕ start, ಪೂರ್ಣಾಙ್ಕ step, ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
ಶೂನ್ಯ copy(ಧ್ರುವ View& src, ಧ್ರುವ View& dst);

// n independent copies executed in one parallel pass.
ಶೂನ್ಯ copyBatch(ಧ್ರುವ View* src, ಧ್ರುವ View* dst, ಪೂರ್ಣಾಙ್ಕ n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*TransposeFunc)(ಧ್ರುವ uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
