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

কাঠামো View
{
    uchar* তথ্য = nullptr;
    পূর্ণসংখ্যা dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    পূর্ণসংখ্যা size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ধ্রুবক Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ধ্রুবক View& v, ধ্রুবক পূর্ণসংখ্যা* order);

শূন্য flip(View& v, পূর্ণসংখ্যা axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
শূন্য slice(View& v, পূর্ণসংখ্যা axis, পূর্ণসংখ্যা start, পূর্ণসংখ্যা step, পূর্ণসংখ্যা গুনতি_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
শূন্য copy(ধ্রুবক View& src, ধ্রুবক View& dst);

// n independent copies executed in one parallel pass.
শূন্য copyBatch(ধ্রুবক View* src, ধ্রুবক View* dst, পূর্ণসংখ্যা n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
প্রকারলেখ শূন্য (*TransposeFunc)(ধ্রুবক uchar* src, size_t sstep, uchar* dst, size_t dstep, আকার_2 sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
