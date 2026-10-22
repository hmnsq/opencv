// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
// Copyright (C) 2026, BigVision LLC, all rights reserved.
// Third party copyrights are property of their respective owners.

// Strided n-dimensional copy shared by the n-dimensional transpose, flip, concat, split, repeat
// and slice.
// Each of them only describes a source and a destination view; channels stay inside the element.

#నిర్వచితం_కాకపోతే OPENCV_CORE_ND_COPY_HPP
#నిర్వచించు OPENCV_CORE_ND_COPY_HPP

నామాకాశ cv { నామాకాశ nd {

గణన { MAX_VIEW_DIMS = CV_MAX_DIM };

నిర్మాణం View
{
    uchar* దత్తాంశం = శూన్యసూచిక;
    పూర్ణాంకం dims = 0;
    size_t esz = 0;                    // element size in bytes, channels included
    పూర్ణాంకం size[MAX_VIEW_DIMS];
    ptrdiff_t అడుగు[MAX_VIEW_DIMS];     // in bytes; 0 = broadcast, negative = reversed
};

View view_యొక్క(స్థిరరాశి Mat& m);

// The i-th axis of the result is the order[i]-th axis of v.
View క్రమమార్పు(స్థిరరాశి View& v, స్థిరరాశి పూర్ణాంకం* క్రమం);

శూన్య flip(View& v, పూర్ణాంకం axis);

// Keep `count` elements along the axis, starting at `start`; `step` may be negative.
శూన్య ముక్క_2(View& v, పూర్ణాంకం axis, పూర్ణాంకం ప్రారంభం, పూర్ణాంకం అడుగు, పూర్ణాంకం లెక్క_2);

// The views must have the same dims, sizes and esz. Overlapping views are allowed.
శూన్య copy(స్థిరరాశి View& src, స్థిరరాశి View& dst);

// n independent copies executed in one parallel pass.
శూన్య copyBatch(స్థిరరాశి View* src, స్థిరరాశి View* dst, పూర్ణాంకం n);

// cv::transpose kernel for the element size, or nullptr; defined in matrix_transform.cpp.
రకనిర్వచనం శూన్య (*TransposeFunc)(స్థిరరాశి uchar* src, size_t sstep, uchar* dst, size_t dstep, కొలత sz);
TransposeFunc getTransposeFunc(size_t esz);

}} // namespace cv::nd

#అయితే_ముగింపు
