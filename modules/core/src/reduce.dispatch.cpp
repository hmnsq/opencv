// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "reduce.simd.hpp"
#అన్తర్భావ "reduce.simd_declarations.hpp"

నామాకాశ cv {

రకనిర్వచనం శూన్య (*Reduce_కూడిక_Func)(స్థిరరాశి Mat& src, Mat& dst);
Reduce_కూడిక_Func getReduceCSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);
Reduce_కూడిక_Func getReduceRSum2Func(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth);

Reduce_కూడిక_Func getReduceCSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_కూడిక_Func getReduceRSumFunc(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_కూడిక_Func getReduceRSum2Func(పూర్ణాంకం sdepth, పూర్ణాంకం ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
