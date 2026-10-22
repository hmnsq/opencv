// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "reduce.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "reduce.simd_declarations.hpp"

ਨਾਮਾਕਾਸ਼ cv {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*Reduce_ਜੋੜ_Func)(ਅਚਲ Mat& src, Mat& dst);
Reduce_ਜੋੜ_Func getReduceCSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);
Reduce_ਜੋੜ_Func getReduceRSum2Func(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth);

Reduce_ਜੋੜ_Func getReduceCSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_ਜੋੜ_Func getReduceRSumFunc(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

Reduce_ਜੋੜ_Func getReduceRSum2Func(ਪੂਰਨ sdepth, ਪੂਰਨ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
