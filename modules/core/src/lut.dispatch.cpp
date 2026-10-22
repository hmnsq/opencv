// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "lut.simd.hpp"
#అన్తర్భావ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

నామాకాశ cv {

నామాకాశ {

రకనిర్వచనం శూన్య (*LUT8uFunc)( స్థిరరాశి uchar*, స్థిరరాశి uchar*, uchar*, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం );
రకనిర్వచనం శూన్య (*LUT16uFunc)( స్థిరరాశి uchar*, స్థిరరాశి ushort*, ushort*, పూర్ణాంకం, పూర్ణాంకం, పూర్ణాంకం );

స్థిర LUT8uFunc resolveLUT8uFunc()
{
#ఒకవేళ CV_TRY_AVX512_ICL
    ఒకవేళ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        తిరిగి opt_AVX512_ICL::LUT8u_;
#అయితే_ముగింపు
    తిరిగి cpu_baseline::LUT8u_;
}

స్థిర LUT16uFunc resolveLUT16uFunc()
{
#ఒకవేళ CV_TRY_AVX512_ICL
    ఒకవేళ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        తిరిగి opt_AVX512_ICL::LUT16u_;
#అయితే_ముగింపు
    తిరిగి cpu_baseline::LUT16u_;
}

} // namespace

శూన్య LUT8u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి uchar* lut, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );

శూన్య LUT8u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి uchar* lut, uchar* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    CV_INSTRUMENT_REGION();
    స్థిర LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

శూన్య LUT16u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి ushort* lut, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn );

శూన్య LUT16u_dispatch( స్థిరరాశి uchar* src, స్థిరరాశి ushort* lut, ushort* dst, పూర్ణాంకం len, పూర్ణాంకం cn, పూర్ణాంకం lutcn )
{
    CV_INSTRUMENT_REGION();
    స్థిర LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
