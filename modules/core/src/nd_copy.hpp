// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_CORE_ND_COPY_HPP
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ OPENCV_CORE_ND_COPY_HPP

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ nd {

ਗਿਣਤੀ { MAX_VIEW_DIMS = CV_MAX_DIM };

ਬਣਤਰ View
{
    uchar* ਅੰਕੜੇ = ਖਾਲੀ_ਸੂਚਕ;
    ਪੂਰਨ dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    ਪੂਰਨ size[MAX_VIEW_DIMS];
    ptrdiff_t ਕਦਮ[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View view_ਦਾ(ਅਚਲ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View ਕ੍ਰਮ_ਬਦਲੋ(ਅਚਲ View& v, ਅਚਲ ਪੂਰਨ* ਕ੍ਰਮ);

ਖਾਲੀ flip(View& v, ਪੂਰਨ axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
ਖਾਲੀ ਟੁਕੜਾ_2(View& v, ਪੂਰਨ axis, ਪੂਰਨ ਸ਼ੁਰੂ, ਪੂਰਨ ਕਦਮ, ਪੂਰਨ ਗਿਣਤੀ_3);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
ਖਾਲੀ copy(ਅਚਲ View& src, ਅਚਲ View& dst);

// n independent copies executed in one parallel pass.
ਖਾਲੀ copyBatch(ਅਚਲ View* src, ਅਚਲ View* dst, ਪੂਰਨ n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*TransposeFunc)(ਅਚਲ uchar* src, size_t sstep, uchar* dst, size_t dstep, ਮਾਪ sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#ਜੇ_ਅੰਤ
