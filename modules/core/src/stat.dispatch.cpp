// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "stat.simd.hpp"
#అన్తర్భావ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv { నామాకాశ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
స్థిర NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
స్థిర NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, పూర్ణాంకం n)
{
    స్థిర స్థిరరాశి NormHammingFunc fn = getNormHammingFunc();
    తిరిగి fn(a, n);
}

పూర్ణాంకం normHamming(స్థిరరాశి uchar* a, స్థిరరాశి uchar* b, పూర్ణాంకం n)
{
    స్థిర స్థిరరాశి NormHammingDiffFunc fn = getNormHammingDiffFunc();
    తిరిగి fn(a, b, n);
}

}} //cv::hal
