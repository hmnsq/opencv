// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "reduce.simd.hpp"
#अन्तर्भाव "reduce.simd_declarations.hpp"

नामाकाश cv {

प्रकारनिर्देश शून्य (*Reduce_योग_Func)(ध्रुव Mat& src, Mat& dst);
Reduce_योग_Func getReduceCSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
Reduce_योग_Func getReduceRSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
Reduce_योग_Func getReduceRSum2Func(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);

Reduce_योग_Func getReduceCSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_योग_Func getReduceRSumFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_योग_Func getReduceRSum2Func(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
