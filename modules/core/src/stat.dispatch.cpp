// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "precomp.hpp"

#ضمن "stat.simd.hpp"
#ضمن "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

نطاق cv { نطاق hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
ساكن NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
ساكن NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

صحيح normHamming(ثابت uchar* a, صحيح n)
{
    ساكن ثابت NormHammingFunc fn = getNormHammingFunc();
    أرجع fn(a, n);
}

صحيح normHamming(ثابت uchar* a, ثابت uchar* b, صحيح n)
{
    ساكن ثابت NormHammingDiffFunc fn = getNormHammingDiffFunc();
    أرجع fn(a, b, n);
}

}} //cv::hal
