// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ "reduce.simd.hpp"
#અન્તર્ભાવ "reduce.simd_declarations.hpp"

નામાકાશ cv {

પ્રકારવ્યાખ્યા શૂન્ય (*ReduceSumFunc)(અચલ Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
ReduceSumFunc getReduceRSum2Func(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);

ReduceSumFunc getReduceCSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
