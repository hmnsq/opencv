// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ضمن "precomp.hpp"

#ضمن "reduce.simd.hpp"
#ضمن "reduce.simd_declarations.hpp"

نطاق cv {

تعريفنوع فراغ (*Reduce_جوڙ_Func)(ثابت Mat& src, Mat& dst);
Reduce_جوڙ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڙ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth);
Reduce_جوڙ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth);

Reduce_جوڙ_Func getReduceCSumFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_جوڙ_Func getReduceRSumFunc(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_جوڙ_Func getReduceRSum2Func(صحيح sdepth, صحيح ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
