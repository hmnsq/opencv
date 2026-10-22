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

নামাকাশ cv { নামাকাশ nd {

গণনা { MAX_VIEW_DIMS = CV_MAX_DIM };

সংরচনা View
{
    uchar* data = nullptr;
    পূর্ণাঙ্ক dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    পূর্ণাঙ্ক size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ধ্রুৱ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ধ্রুৱ View& v, ধ্রুৱ পূর্ণাঙ্ক* order);

শূন্য flip(View& v, পূর্ণাঙ্ক axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
শূন্য slice(View& v, পূর্ণাঙ্ক axis, পূর্ণাঙ্ক start, পূর্ণাঙ্ক step, পূর্ণাঙ্ক গুনতি_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
শূন্য copy(ধ্রুৱ View& src, ধ্রুৱ View& dst);

// n independent copies executed in one parallel pass.
শূন্য copyBatch(ধ্রুৱ View* src, ধ্রুৱ View* dst, পূর্ণাঙ্ক n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
প্রকারনির্দেশ শূন্য (*TransposeFunc)(ধ্রুৱ uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
