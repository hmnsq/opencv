// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "stat.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
ਸਥਿਰ NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
ਸਥਿਰ NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਪੂਰਨ n)
{
    ਸਥਿਰ ਅਚਲ NormHammingFunc fn = getNormHammingFunc();
    ਵਾਪਸ fn(a, n);
}

ਪੂਰਨ normHamming(ਅਚਲ uchar* a, ਅਚਲ uchar* b, ਪੂਰਨ n)
{
    ਸਥਿਰ ਅਚਲ NormHammingDiffFunc fn = getNormHammingDiffFunc();
    ਵਾਪਸ fn(a, b, n);
}

}} //cv::hal
