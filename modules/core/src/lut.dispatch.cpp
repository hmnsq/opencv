// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ "lut.simd.hpp"
#અન્તર્ભાવ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

નામાકાશ cv {

નામાકાશ {

પ્રકારવ્યાખ્યા શૂન્ય (*LUT8uFunc)( અચલ uchar*, અચલ uchar*, uchar*, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક );
પ્રકારવ્યાખ્યા શૂન્ય (*LUT16uFunc)( અચલ uchar*, અચલ ushort*, ushort*, પૂર્ણાંક, પૂર્ણાંક, પૂર્ણાંક );

સ્થિર LUT8uFunc resolveLUT8uFunc()
{
#જો CV_TRY_AVX512_ICL
    જો (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        પાછા opt_AVX512_ICL::LUT8u_;
#endif
    પાછા cpu_baseline::LUT8u_;
}

સ્થિર LUT16uFunc resolveLUT16uFunc()
{
#જો CV_TRY_AVX512_ICL
    જો (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        પાછા opt_AVX512_ICL::LUT16u_;
#endif
    પાછા cpu_baseline::LUT16u_;
}

} // namespace

શૂન્ય LUT8u_dispatch( અચલ uchar* src, અચલ uchar* lut, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn );

શૂન્ય LUT8u_dispatch( અચલ uchar* src, અચલ uchar* lut, uchar* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn )
{
    CV_INSTRUMENT_REGION();
    સ્થિર LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

શૂન્ય LUT16u_dispatch( અચલ uchar* src, અચલ ushort* lut, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn );

શૂન્ય LUT16u_dispatch( અચલ uchar* src, અચલ ushort* lut, ushort* dst, પૂર્ણાંક len, પૂર્ણાંક cn, પૂર્ણાંક lutcn )
{
    CV_INSTRUMENT_REGION();
    સ્થિર LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
