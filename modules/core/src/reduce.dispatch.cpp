// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#شامل "precomp.hpp"

#شامل "reduce.simd.hpp"
#شامل "reduce.simd_declarations.hpp"

فضاینام cv {

تعریفنوع تهی (*ReduceSumFunc)(ثابت Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(صحیح sdepth, صحیح ddepth);
ReduceSumFunc getReduceRSumFunc(صحیح sdepth, صحیح ddepth);
ReduceSumFunc getReduceRSum2Func(صحیح sdepth, صحیح ddepth);

ReduceSumFunc getReduceCSumFunc(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(صحیح sdepth, صحیح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
