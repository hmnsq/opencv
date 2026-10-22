// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#include "precomp.hpp"

#include "reduce.simd.hpp"
#include "reduce.simd_declarations.hpp"

namespace cv {

typedef void (*ReduceSumFunc)(const Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth);
ReduceSumFunc getReduceRSumFunc(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth);
ReduceSumFunc getReduceRSum2Func(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth);

ReduceSumFunc getReduceCSumFunc(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(ចំនួនគត់ sdepth, ចំនួនគត់ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
