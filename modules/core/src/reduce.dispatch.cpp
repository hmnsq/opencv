// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#समावेश "precomp.hpp"

#समावेश "reduce.simd.hpp"
#समावेश "reduce.simd_declarations.hpp"

namespace cv {

प्रकार व्योम (*Reduce_योग_Func)(स्थिर Mat& src, Mat& dst);
Reduce_योग_Func getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
Reduce_योग_Func getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
Reduce_योग_Func getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth);

Reduce_योग_Func getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_योग_Func getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_योग_Func getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
