// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব "lut.simd.hpp"
#অন্তর্ভাব "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

নামাকাশ cv {

নামাকাশ {

প্রকারনির্দেশ শূন্য (*LUT8uFunc)( ধ্রুব uchar*, ধ্রুব uchar*, uchar*, পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক );
প্রকারনির্দেশ শূন্য (*LUT16uFunc)( ধ্রুব uchar*, ধ্রুব ushort*, ushort*, পূর্ণাঙ্ক, পূর্ণাঙ্ক, পূর্ণাঙ্ক );

স্থির LUT8uFunc resolveLUT8uFunc()
{
#যদি CV_TRY_AVX512_ICL
    যদি (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        প্রত্যাগম opt_AVX512_ICL::LUT8u_;
#endif
    প্রত্যাগম cpu_baseline::LUT8u_;
}

স্থির LUT16uFunc resolveLUT16uFunc()
{
#যদি CV_TRY_AVX512_ICL
    যদি (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        প্রত্যাগম opt_AVX512_ICL::LUT16u_;
#endif
    প্রত্যাগম cpu_baseline::LUT16u_;
}

} // namespace

শূন্য LUT8u_dispatch( ধ্রুব uchar* src, ধ্রুব uchar* lut, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn );

শূন্য LUT8u_dispatch( ধ্রুব uchar* src, ধ্রুব uchar* lut, uchar* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn )
{
    CV_INSTRUMENT_REGION();
    স্থির LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

শূন্য LUT16u_dispatch( ধ্রুব uchar* src, ধ্রুব ushort* lut, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn );

শূন্য LUT16u_dispatch( ধ্রুব uchar* src, ধ্রুব ushort* lut, ushort* dst, পূর্ণাঙ্ক len, পূর্ণাঙ্ক cn, পূর্ণাঙ্ক lutcn )
{
    CV_INSTRUMENT_REGION();
    স্থির LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
