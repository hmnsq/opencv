// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ضمن "precomp.hpp"

#ضمن "lut.simd.hpp"
#ضمن "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

نطاق cv {

نطاق {

تعريفنوع فراغ (*LUT8uFunc)( ثابت uchar*, ثابت uchar*, uchar*, صحيح, صحيح, صحيح );
تعريفنوع فراغ (*LUT16uFunc)( ثابت uchar*, ثابت ushort*, ushort*, صحيح, صحيح, صحيح );

ساكن LUT8uFunc resolveLUT8uFunc()
{
#إذا CV_TRY_AVX512_ICL
    إذا (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        أرجع opt_AVX512_ICL::LUT8u_;
#endif
    أرجع cpu_baseline::LUT8u_;
}

ساكن LUT16uFunc resolveLUT16uFunc()
{
#إذا CV_TRY_AVX512_ICL
    إذا (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        أرجع opt_AVX512_ICL::LUT16u_;
#endif
    أرجع cpu_baseline::LUT16u_;
}

} // namespace

فراغ LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn );

فراغ LUT8u_dispatch( ثابت uchar* src, ثابت uchar* lut, uchar* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    CV_INSTRUMENT_REGION();
    ساكن LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

فراغ LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn );

فراغ LUT16u_dispatch( ثابت uchar* src, ثابت ushort* lut, ushort* dst, صحيح len, صحيح cn, صحيح lutcn )
{
    CV_INSTRUMENT_REGION();
    ساكن LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
