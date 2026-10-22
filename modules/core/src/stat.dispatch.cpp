// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "stat.simd.hpp"
#ಅನ್ತರ್ಭಾವ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
ಸ್ಥಿರ NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
ಸ್ಥಿರ NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಸ್ಥಿರ ಧ್ರುವ NormHammingFunc fn = getNormHammingFunc();
    ಪ್ರತ್ಯಾಗಮ fn(a, n);
}

ಪೂರ್ಣಾಙ್ಕ normHamming(ಧ್ರುವ uchar* a, ಧ್ರುವ uchar* b, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಸ್ಥಿರ ಧ್ರುವ NormHammingDiffFunc fn = getNormHammingDiffFunc();
    ಪ್ರತ್ಯಾಗಮ fn(a, b, n);
}

}} //cv::hal
