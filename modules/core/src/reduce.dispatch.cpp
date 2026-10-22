// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "reduce.simd.hpp"
#अन्तर्भाव "reduce.simd_declarations.hpp"

नामाकाश cv {

प्रकारव्याख्या शून्य (*ReduceSumFunc)(स्थिरांक Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
ReduceSumFunc getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth);
ReduceSumFunc getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth);

ReduceSumFunc getReduceCSumFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(पूर्णांक sdepth, पूर्णांक ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
