// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "lut.simd.hpp"
#अन्तर्भाव "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

नामाकाश cv {

नामाकाश {

प्रकारव्याख्या शून्य (*LUT8uFunc)( स्थिरांक uchar*, स्थिरांक uchar*, uchar*, पूर्णांक, पूर्णांक, पूर्णांक );
प्रकारव्याख्या शून्य (*LUT16uFunc)( स्थिरांक uchar*, स्थिरांक ushort*, ushort*, पूर्णांक, पूर्णांक, पूर्णांक );

स्थिर LUT8uFunc resolveLUT8uFunc()
{
#जर CV_TRY_AVX512_ICL
    जर (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        परत opt_AVX512_ICL::LUT8u_;
#endif
    परत cpu_baseline::LUT8u_;
}

स्थिर LUT16uFunc resolveLUT16uFunc()
{
#जर CV_TRY_AVX512_ICL
    जर (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        परत opt_AVX512_ICL::LUT16u_;
#endif
    परत cpu_baseline::LUT16u_;
}

} // namespace

शून्य LUT8u_dispatch( स्थिरांक uchar* src, स्थिरांक uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

शून्य LUT8u_dispatch( स्थिरांक uchar* src, स्थिरांक uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    CV_INSTRUMENT_REGION();
    स्थिर LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

शून्य LUT16u_dispatch( स्थिरांक uchar* src, स्थिरांक ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

शून्य LUT16u_dispatch( स्थिरांक uchar* src, स्थिरांक ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    CV_INSTRUMENT_REGION();
    स्थिर LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
