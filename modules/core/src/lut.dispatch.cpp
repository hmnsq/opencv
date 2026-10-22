// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "lut.simd.hpp"
#অন্তর্ভাব "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

নামাকাশ {

প্রকারলেখ শূন্য (*LUT8uFunc)( ধ্রুবক uchar*, ধ্রুবক uchar*, uchar*, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা );
প্রকারলেখ শূন্য (*LUT16uFunc)( ধ্রুবক uchar*, ধ্রুবক ushort*, ushort*, পূর্ণসংখ্যা, পূর্ণসংখ্যা, পূর্ণসংখ্যা );

স্থির LUT8uFunc resolveLUT8uFunc()
{
#যদি CV_TRY_AVX512_ICL
    যদি (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ফেরত opt_AVX512_ICL::LUT8u_;
#endif
    ফেরত cpu_baseline::LUT8u_;
}

স্থির LUT16uFunc resolveLUT16uFunc()
{
#যদি CV_TRY_AVX512_ICL
    যদি (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ফেরত opt_AVX512_ICL::LUT16u_;
#endif
    ফেরত cpu_baseline::LUT16u_;
}

} // namespace

শূন্য LUT8u_dispatch( ধ্রুবক uchar* src, ধ্রুবক uchar* lut, uchar* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn );

শূন্য LUT8u_dispatch( ধ্রুবক uchar* src, ধ্রুবক uchar* lut, uchar* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn )
{
    CV_INSTRUMENT_REGION();
    স্থির LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

শূন্য LUT16u_dispatch( ধ্রুবক uchar* src, ধ্রুবক ushort* lut, ushort* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn );

শূন্য LUT16u_dispatch( ধ্রুবক uchar* src, ধ্রুবক ushort* lut, ushort* dst, পূর্ণসংখ্যা len, পূর্ণসংখ্যা cn, পূর্ণসংখ্যা lutcn )
{
    CV_INSTRUMENT_REGION();
    স্থির LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
