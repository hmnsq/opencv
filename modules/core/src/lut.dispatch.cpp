// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#شامل "precomp.hpp"

#شامل "lut.simd.hpp"
#شامل "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

فضاینام cv {

فضاینام {

تعریفنوع تهی (*LUT8uFunc)( ثابت uchar*, ثابت uchar*, uchar*, صحیح, صحیح, صحیح );
تعریفنوع تهی (*LUT16uFunc)( ثابت uchar*, ثابت ushort*, ushort*, صحیح, صحیح, صحیح );

ایستا LUT8uFunc resolveLUT8uFunc()
{
#اگر CV_TRY_AVX512_ICL
    اگر (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        بازگشت opt_AVX512_ICL::LUT8u_;
#endif
    بازگشت cpu_baseline::LUT8u_;
}

ایستا LUT16uFunc resolveLUT16uFunc()
{
#اگر CV_TRY_AVX512_ICL
    اگر (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        بازگشت opt_AVX512_ICL::LUT16u_;
#endif
    بازگشت cpu_baseline::LUT16u_;
}

} // namespace

تهی LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحیح len, صحیح cn, صحیح lutcn );

تهی LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحیح len, صحیح cn, صحیح lutcn )
{
    CV_INSTRUMENT_REGION();
    ایستا LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

تهی LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحیح len, صحیح cn, صحیح lutcn );

تهی LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحیح len, صحیح cn, صحیح lutcn )
{
    CV_INSTRUMENT_REGION();
    ایستا LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
