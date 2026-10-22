// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "lut.simd.hpp"
#अन्तर्भाव "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

नामाकाश {

प्रकारनिर्देश शून्य (*LUT8uFunc)( अचर uchar*, अचर uchar*, uchar*, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क );
प्रकारनिर्देश शून्य (*LUT16uFunc)( अचर uchar*, अचर ushort*, ushort*, पूर्णाङ्क, पूर्णाङ्क, पूर्णाङ्क );

स्थिर LUT8uFunc resolveLUT8uFunc()
{
#यदि CV_TRY_AVX512_ICL
    यदि (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        फर्काऊ opt_AVX512_ICL::LUT8u_;
#endif
    फर्काऊ cpu_baseline::LUT8u_;
}

स्थिर LUT16uFunc resolveLUT16uFunc()
{
#यदि CV_TRY_AVX512_ICL
    यदि (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        फर्काऊ opt_AVX512_ICL::LUT16u_;
#endif
    फर्काऊ cpu_baseline::LUT16u_;
}

} // namespace

शून्य LUT8u_dispatch( अचर uchar* src, अचर uchar* lut, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );

शून्य LUT8u_dispatch( अचर uchar* src, अचर uchar* lut, uchar* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    CV_INSTRUMENT_REGION();
    स्थिर LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

शून्य LUT16u_dispatch( अचर uchar* src, अचर ushort* lut, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn );

शून्य LUT16u_dispatch( अचर uchar* src, अचर ushort* lut, ushort* dst, पूर्णाङ्क len, पूर्णाङ्क cn, पूर्णाङ्क lutcn )
{
    CV_INSTRUMENT_REGION();
    स्थिर LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
