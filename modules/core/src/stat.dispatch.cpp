// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ "stat.simd.hpp"
#അന്തര്ഭാവ "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv { നാമാകാശ hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
സ്ഥിതം NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
സ്ഥിതം NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, പൂർണ്ണസംഖ്യ n)
{
    സ്ഥിതം സ്ഥിരം NormHammingFunc fn = getNormHammingFunc();
    തിരികെ fn(a, n);
}

പൂർണ്ണസംഖ്യ normHamming(സ്ഥിരം uchar* a, സ്ഥിരം uchar* b, പൂർണ്ണസംഖ്യ n)
{
    സ്ഥിതം സ്ഥിരം NormHammingDiffFunc fn = getNormHammingDiffFunc();
    തിരികെ fn(a, b, n);
}

}} //cv::hal
