// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ "reduce.simd.hpp"
#അന്തര്ഭാവ "reduce.simd_declarations.hpp"

നാമാകാശ cv {

തരനിർവചനം ശൂന്യം (*ReduceSumFunc)(സ്ഥിരം Mat& src, Mat& dst);
ReduceSumFunc getReduceCSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
ReduceSumFunc getReduceRSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
ReduceSumFunc getReduceRSum2Func(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);

ReduceSumFunc getReduceCSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceCSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSumFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSumFunc, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

ReduceSumFunc getReduceRSum2Func(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth)
{
    CV_INSTRUMENT_REGION();
    CV_CPU_DISPATCH(getReduceRSum2Func, (sdepth, ddepth),
        CV_CPU_DISPATCH_MODES_ALL);
}

} // namespace cv
