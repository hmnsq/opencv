// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#اگر_غیر_تعریف_شدہ OPENCV_CORE_ND_COPY_HPP
#تعریف_کرو OPENCV_CORE_ND_COPY_HPP

نامخلا cv { نامخلا nd {

شمار { MAX_VIEW_DIMS = CV_MAX_DIM };

ساخت View
{
    uchar* ڈیٹا = خالی_اشاریہ;
    صحیح dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    صحیح size[MAX_VIEW_DIMS];
    ptrdiff_t قدم[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View view_کا(ثابت Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View ترتیب_بدلو(ثابت View& v, ثابت صحیح* ترتیب);

خالی flip(View& v, صحیح axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
خالی قاش_2(View& v, صحیح axis, صحیح آغاز, صحیح قدم, صحیح گنتی_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
خالی copy(ثابت View& src, ثابت View& dst);

// n independent copies executed in one parallel pass.
خالی copyBatch(ثابت View* src, ثابت View* dst, صحیح n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
قسمتعریف خالی (*TransposeFunc)(ثابت uchar* src, size_t sstep, uchar* dst, size_t dstep, سائز sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#اگر_ختم
