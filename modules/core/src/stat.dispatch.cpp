// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ "stat.simd.hpp"
#அந்தர்பாவ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv { நாமாகாஶ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
நிலையான NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
நிலையான NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

முழு normHamming(மாறிலி uchar* a, முழு n)
{
    நிலையான மாறிலி NormHammingFunc fn = getNormHammingFunc();
    திரும்பு fn(a, n);
}

முழு normHamming(மாறிலி uchar* a, மாறிலி uchar* b, முழு n)
{
    நிலையான மாறிலி NormHammingDiffFunc fn = getNormHammingDiffFunc();
    திரும்பு fn(a, b, n);
}

}} //cv::hal
