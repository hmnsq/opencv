// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "stat.simd.hpp"
#অন্তর্ভাব "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv { নামাকাশ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
স্থির NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
স্থির NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, পূর্ণাঙ্ক n)
{
    স্থির ধ্রুব NormHammingFunc fn = getNormHammingFunc();
    প্রত্যাগম fn(a, n);
}

পূর্ণাঙ্ক normHamming(ধ্রুব uchar* a, ধ্রুব uchar* b, পূর্ণাঙ্ক n)
{
    স্থির ধ্রুব NormHammingDiffFunc fn = getNormHammingDiffFunc();
    প্রত্যাগম fn(a, b, n);
}

}} //cv::hal
