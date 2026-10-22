// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "reduce.simd.hpp"
#অন্তর্ভাব "reduce.simd_declarations.hpp"

নামাকাশ cv {

প্রকারনির্দেশ শূন্য (*ReduceSumFunc)(ধ্রুব Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
ReduceSumFunc getReduceRSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
ReduceSumFunc getReduceRSum2Func(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);

ReduceSumFunc getReduceCSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
