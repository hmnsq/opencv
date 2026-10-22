// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "lut.simd.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

ਨਾਮਾਕਾਸ਼ cv {

ਨਾਮਾਕਾਸ਼ {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*LUT8uFunc)( ਅਚਲ uchar*, ਅਚਲ uchar*, uchar*, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ );
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਖਾਲੀ (*LUT16uFunc)( ਅਚਲ uchar*, ਅਚਲ ushort*, ushort*, ਪੂਰਨ, ਪੂਰਨ, ਪੂਰਨ );

ਸਥਿਰ LUT8uFunc resolveLUT8uFunc()
{
#ਜੇ CV_TRY_AVX512_ICL
    ਜੇ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ਵਾਪਸ opt_AVX512_ICL::LUT8u_;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ cpu_baseline::LUT8u_;
}

ਸਥਿਰ LUT16uFunc resolveLUT16uFunc()
{
#ਜੇ CV_TRY_AVX512_ICL
    ਜੇ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ਵਾਪਸ opt_AVX512_ICL::LUT16u_;
#ਜੇ_ਅੰਤ
    ਵਾਪਸ cpu_baseline::LUT16u_;
}

} // namespace

ਖਾਲੀ LUT8u_dispatch( ਅਚਲ uchar* src, ਅਚਲ uchar* lut, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );

ਖਾਲੀ LUT8u_dispatch( ਅਚਲ uchar* src, ਅਚਲ uchar* lut, uchar* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    CV_INSTRUMENT_REGION();
    ਸਥਿਰ LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

ਖਾਲੀ LUT16u_dispatch( ਅਚਲ uchar* src, ਅਚਲ ushort* lut, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn );

ਖਾਲੀ LUT16u_dispatch( ਅਚਲ uchar* src, ਅਚਲ ushort* lut, ushort* dst, ਪੂਰਨ len, ਪੂਰਨ cn, ਪੂਰਨ lutcn )
{
    CV_INSTRUMENT_REGION();
    ਸਥਿਰ LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
