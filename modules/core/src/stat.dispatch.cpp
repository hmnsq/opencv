// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ "stat.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv { ନାମାକାଶ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
ସ୍ଥିର NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
ସ୍ଥିର NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ସ୍ଥିର ଅଚଳ NormHammingFunc fn = getNormHammingFunc();
    ଫେରନ୍ତୁ fn(a, n);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା normHamming(ଅଚଳ uchar* a, ଅଚଳ uchar* b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ସ୍ଥିର ଅଚଳ NormHammingDiffFunc fn = getNormHammingDiffFunc();
    ଫେରନ୍ତୁ fn(a, b, n);
}

}} //cv::hal
