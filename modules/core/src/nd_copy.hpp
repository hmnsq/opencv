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

നാമാകാശ cv { നാമാകാശ nd {

എണ്ണൽ { MAX_VIEW_DIMS = CV_MAX_DIM };

ഘടന View
{
    uchar* data = nullptr;
    പൂർണ്ണസംഖ്യ dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    പൂർണ്ണസംഖ്യ size[MAX_VIEW_DIMS];
    ptrdiff_t step[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View viewOf(സ്ഥിരം Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View permute(സ്ഥിരം View& v, സ്ഥിരം പൂർണ്ണസംഖ്യ* order);

ശൂന്യം flip(View& v, പൂർണ്ണസംഖ്യ axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
ശൂന്യം slice(View& v, പൂർണ്ണസംഖ്യ axis, പൂർണ്ണസംഖ്യ start, പൂർണ്ണസംഖ്യ step, പൂർണ്ണസംഖ്യ എണ്ണം_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
ശൂന്യം copy(സ്ഥിരം View& src, സ്ഥിരം View& dst);

// n independent copies executed in one parallel pass.
ശൂന്യം copyBatch(സ്ഥിരം View* src, സ്ഥിരം View* dst, പൂർണ്ണസംഖ്യ n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
തരനിർവചനം ശൂന്യം (*TransposeFunc)(സ്ഥിരം uchar* src, size_t sstep, uchar* dst, size_t dstep, Size sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#endif
