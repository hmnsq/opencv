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

namespace cv { namespace nd {

ga_nanaa { MAX_VIEW_DIMS = CV_MAX_DIM };

sa_nracanaa View
{
    uchar* data = nullptr;
    ຈຳນວນເຕັມ dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    ຈຳນວນເຕັມ size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(const Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(const View& v, const ຈຳນວນເຕັມ* order);

void flip(View& v, ຈຳນວນເຕັມ axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
void slice(View& v, ຈຳນວນເຕັມ axis, ຈຳນວນເຕັມ start, ຈຳນວນເຕັມ step, ຈຳນວນເຕັມ count);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
void copy(const View& src, const View& dst);

// n independent copies executed in one parallel pass.
void copyBatch(const View* src, const View* dst, ຈຳນວນເຕັມ n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
typedef void (*TransposeFunc)(const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
