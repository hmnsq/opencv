// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
//
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#समावेश "precomp.hpp"

#समावेश "stat.simd.hpp"
#समावेश "stat.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX2,...,BASELINE based on CMakeLists.txt content

namespace cv { namespace hal {

// Resolve the SIMD implementation ONCE via a cached function pointer. normHamming
// is called per-descriptor in hot matcher loops on short (32/64-byte) descriptors,
// where a per-call dispatch chain + CV_INSTRUMENT_REGION dominate the cost.
अटल NormHammingFunc getNormHammingFunc() {
    CV_CPU_DISPATCH(getNormHammingFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}
अटल NormHammingDiffFunc getNormHammingDiffFunc() {
    CV_CPU_DISPATCH(getNormHammingDiffFunc, (), CV_CPU_DISPATCH_MODES_ALL);
}

पूर्णांक normHamming(स्थिर uchar* a, पूर्णांक n)
{
    अटल स्थिर NormHammingFunc fn = getNormHammingFunc();
    वापस fn(a, n);
}

पूर्णांक normHamming(स्थिर uchar* a, स्थिर uchar* b, पूर्णांक n)
{
    अटल स्थिर NormHammingDiffFunc fn = getNormHammingDiffFunc();
    वापस fn(a, b, n);
}

}} //cv::hal
