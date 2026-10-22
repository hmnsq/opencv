// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "reduce.simd.hpp"
#অন্তর্ভাব "reduce.simd_declarations.hpp"

নামাকাশ cv {

প্রকারলেখ শূন্য (*Reduce_যোগ_Func)(ধ্রুবক Mat& src, Mat& dst);
Reduce_যোগ_Func getReduceCSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
Reduce_যোগ_Func getReduceRSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
Reduce_যোগ_Func getReduceRSum2Func(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);

Reduce_যোগ_Func getReduceCSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_যোগ_Func getReduceRSumFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_যোগ_Func getReduceRSum2Func(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
