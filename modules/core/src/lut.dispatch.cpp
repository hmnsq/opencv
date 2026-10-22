// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ "lut.simd.hpp"
#ଅନ୍ତର୍ଭାଵ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

ନାମାକାଶ cv {

ନାମାକାଶ {

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*LUT8uFunc)( ଅଚଳ uchar*, ଅଚଳ uchar*, uchar*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା );
ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*LUT16uFunc)( ଅଚଳ uchar*, ଅଚଳ ushort*, ushort*, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା, ପୂର୍ଣ୍ଣସଂଖ୍ୟା );

ସ୍ଥିର LUT8uFunc resolveLUT8uFunc()
{
#ଯଦି CV_TRY_AVX512_ICL
    ଯଦି (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ଫେରନ୍ତୁ opt_AVX512_ICL::LUT8u_;
#endif
    ଫେରନ୍ତୁ cpu_baseline::LUT8u_;
}

ସ୍ଥିର LUT16uFunc resolveLUT16uFunc()
{
#ଯଦି CV_TRY_AVX512_ICL
    ଯଦି (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ଫେରନ୍ତୁ opt_AVX512_ICL::LUT16u_;
#endif
    ଫେରନ୍ତୁ cpu_baseline::LUT16u_;
}

} // namespace

ଶୂନ୍ୟ LUT8u_dispatch( ଅଚଳ uchar* src, ଅଚଳ uchar* lut, uchar* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn );

ଶୂନ୍ୟ LUT8u_dispatch( ଅଚଳ uchar* src, ଅଚଳ uchar* lut, uchar* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn )
{
    CV_INSTRUMENT_REGION();
    ସ୍ଥିର LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

ଶୂନ୍ୟ LUT16u_dispatch( ଅଚଳ uchar* src, ଅଚଳ ushort* lut, ushort* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn );

ଶୂନ୍ୟ LUT16u_dispatch( ଅଚଳ uchar* src, ଅଚଳ ushort* lut, ushort* dst, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn, ପୂର୍ଣ୍ଣସଂଖ୍ୟା lutcn )
{
    CV_INSTRUMENT_REGION();
    ସ୍ଥିର LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
