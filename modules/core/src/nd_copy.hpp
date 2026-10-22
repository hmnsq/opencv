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

ନାମାକାଶ cv { ନାମାକାଶ nd {

ଗଣନା { MAX_VIEW_DIMS = CV_MAX_DIM };

ସଂରଚନା View
{
    uchar* data = nullptr;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ଅଚଳ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ଅଚଳ View& v, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* order);

ଶୂନ୍ୟ flip(View& v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
ଶୂନ୍ୟ slice(View& v, ପୂର୍ଣ୍ଣସଂଖ୍ୟା axis, ପୂର୍ଣ୍ଣସଂଖ୍ୟା start, ପୂର୍ଣ୍ଣସଂଖ୍ୟା step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_3);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
ଶୂନ୍ୟ copy(ଅଚଳ View& src, ଅଚଳ View& dst);

// n independent copies executed in one parallel pass.
ଶୂନ୍ୟ copyBatch(ଅଚଳ View* src, ଅଚଳ View* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*TransposeFunc)(ଅଚଳ uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
