// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "stat.simd.hpp"
#अन्तर्भाव "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

नामाकाश cv { नामाकाश hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
स्थिर NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
स्थिर NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

पूर्णाङ्क normHamming(ध्रुव uchar* a, पूर्णाङ्क n)
{
    स्थिर ध्रुव NormHammingFunc fn = getNormHammingFunc();
    पटिनिवत्त fn(a, n);
}

पूर्णाङ्क normHamming(ध्रुव uchar* a, ध्रुव uchar* b, पूर्णाङ्क n)
{
    स्थिर ध्रुव NormHammingDiffFunc fn = getNormHammingDiffFunc();
    पटिनिवत्त fn(a, b, n);
}

}} //cv::hal
