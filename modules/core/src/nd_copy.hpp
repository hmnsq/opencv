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

نطاق cv { نطاق nd {

تعداد { MAX_VIEW_DIMS = CV_MAX_DIM };

بنية View
{
    uchar* data = nullptr;
    صحيح dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    صحيح size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(ثابت Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(ثابت View& v, ثابت صحيح* order);

فراغ flip(View& v, صحيح axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
فراغ slice(View& v, صحيح axis, صحيح start, صحيح step, صحيح كدي_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
فراغ copy(ثابت View& src, ثابت View& dst);

// n independent copies executed in one parallel pass.
فراغ copyBatch(ثابت View* src, ثابت View* dst, صحيح n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
تعريفنوع فراغ (*TransposeFunc)(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
