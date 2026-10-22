// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#समावेश "precomp.hpp"

#समावेश "lut.simd.hpp"
#समावेश "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace {

प्रकार व्योम (*LUT8uFunc)( स्थिर uchar*, स्थिर uchar*, uchar*, पूर्णांक, पूर्णांक, पूर्णांक );
प्रकार व्योम (*LUT16uFunc)( स्थिर uchar*, स्थिर ushort*, ushort*, पूर्णांक, पूर्णांक, पूर्णांक );

अटल LUT8uFunc resolveLUT8uFunc()
{
#अगर CV_TRY_AVX512_ICL
    अगर (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        वापस opt_AVX512_ICL::LUT8u_;
#पूर्ण_अगर
    वापस cpu_baseline::LUT8u_;
}

अटल LUT16uFunc resolveLUT16uFunc()
{
#अगर CV_TRY_AVX512_ICL
    अगर (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        वापस opt_AVX512_ICL::LUT16u_;
#पूर्ण_अगर
    वापस cpu_baseline::LUT16u_;
}

} // namespace

व्योम LUT8u_dispatch( स्थिर uchar* src, स्थिर uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

व्योम LUT8u_dispatch( स्थिर uchar* src, स्थिर uchar* lut, uchar* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    CV_INSTRUMENT_REGION();
    अटल LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

व्योम LUT16u_dispatch( स्थिर uchar* src, स्थिर ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn );

व्योम LUT16u_dispatch( स्थिर uchar* src, स्थिर ushort* lut, ushort* dst, पूर्णांक len, पूर्णांक cn, पूर्णांक lutcn )
{
    CV_INSTRUMENT_REGION();
    अटल LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
