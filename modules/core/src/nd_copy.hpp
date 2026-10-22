// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_CORE_ND_COPY_HPP
#ವ್ಯಾಖ್ಯಾನಿಸು OPENCV_CORE_ND_COPY_HPP

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ nd {

ಎಣಿಕೆ { MAX_VIEW_DIMS = CV_MAX_DIM };

ರಚನೆ View
{
    uchar* ದತ್ತಾಂಶ = ಶೂನ್ಯಸೂಚಕ;
    ಪೂರ್ಣಾಂಕ dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    ಪೂರ್ಣಾಂಕ size[MAX_VIEW_DIMS];
    ptrdiff_t ಹೆಜ್ಜೆ[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View view_ದ(ಸ್ಥಿರಾಂಕ Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View ಕ್ರಮಬದಲಾವಣೆ(ಸ್ಥಿರಾಂಕ View& v, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* ಕ್ರಮ);

ಶೂನ್ಯ flip(View& v, ಪೂರ್ಣಾಂಕ axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
ಶೂನ್ಯ ತುಂಡು_2(View& v, ಪೂರ್ಣಾಂಕ axis, ಪೂರ್ಣಾಂಕ ಆರಂಭ, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_3);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
ಶೂನ್ಯ copy(ಸ್ಥಿರಾಂಕ View& src, ಸ್ಥಿರಾಂಕ View& dst);

// n independent copies executed in one parallel pass.
ಶೂನ್ಯ copyBatch(ಸ್ಥಿರಾಂಕ View* src, ಸ್ಥಿರಾಂಕ View* dst, ಪೂರ್ಣಾಂಕ n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*TransposeFunc)(ಸ್ಥಿರಾಂಕ uchar* src, size_t sstep, uchar* dst, size_t dstep, ಗಾತ್ರ_2 sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#ಆದರೆ_ಅಂತ್ಯ
