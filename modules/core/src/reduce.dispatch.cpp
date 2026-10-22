// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ "reduce.simd.hpp"
#அந்தர்பாவ "reduce.simd_declarations.hpp"

நாமாகாஶ cv {

வகைவரை வெறுமை (*ReduceSumFunc)(மாறிலி Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(முழு sdepth, முழு ddepth);
ReduceSumFunc getReduceRSumFunc(முழு sdepth, முழு ddepth);
ReduceSumFunc getReduceRSum2Func(முழு sdepth, முழு ddepth);

ReduceSumFunc getReduceCSumFunc(முழு sdepth, முழு ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(முழு sdepth, முழு ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(முழு sdepth, முழு ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
