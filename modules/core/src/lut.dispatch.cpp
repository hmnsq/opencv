// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ "lut.simd.hpp"
#அந்தர்பாவ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

நாமாகாஶ cv {

நாமாகாஶ {

வகைவரை வெறுமை (*LUT8uFunc)( மாறிலி uchar*, மாறிலி uchar*, uchar*, முழு, முழு, முழு );
வகைவரை வெறுமை (*LUT16uFunc)( மாறிலி uchar*, மாறிலி ushort*, ushort*, முழு, முழு, முழு );

நிலையான LUT8uFunc resolveLUT8uFunc()
{
#எனில் CV_TRY_AVX512_ICL
    எனில் (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        திரும்பு opt_AVX512_ICL::LUT8u_;
#endif
    திரும்பு cpu_baseline::LUT8u_;
}

நிலையான LUT16uFunc resolveLUT16uFunc()
{
#எனில் CV_TRY_AVX512_ICL
    எனில் (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        திரும்பு opt_AVX512_ICL::LUT16u_;
#endif
    திரும்பு cpu_baseline::LUT16u_;
}

} // namespace

வெறுமை LUT8u_dispatch( மாறிலி uchar* src, மாறிலி uchar* lut, uchar* dst, முழு len, முழு cn, முழு lutcn );

வெறுமை LUT8u_dispatch( மாறிலி uchar* src, மாறிலி uchar* lut, uchar* dst, முழு len, முழு cn, முழு lutcn )
{
    CV_INSTRUMENT_REGION();
    நிலையான LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

வெறுமை LUT16u_dispatch( மாறிலி uchar* src, மாறிலி ushort* lut, ushort* dst, முழு len, முழு cn, முழு lutcn );

வெறுமை LUT16u_dispatch( மாறிலி uchar* src, மாறிலி ushort* lut, ushort* dst, முழு len, முழு cn, முழு lutcn )
{
    CV_INSTRUMENT_REGION();
    நிலையான LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
