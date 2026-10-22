// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "lut.simd.hpp"
#ಅನ್ತರ್ಭಾವ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ನಾಮಾಕಾಶ {

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*LUT8uFunc)( ಧ್ರುವ uchar*, ಧ್ರುವ uchar*, uchar*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ );
ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*LUT16uFunc)( ಧ್ರುವ uchar*, ಧ್ರುವ ushort*, ushort*, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ, ಪೂರ್ಣಾಙ್ಕ );

ಸ್ಥಿರ LUT8uFunc resolveLUT8uFunc()
{
#ಯದಿ CV_TRY_AVX512_ICL
    ಯದಿ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ಪ್ರತ್ಯಾಗಮ opt_AVX512_ICL::LUT8u_;
#endif
    ಪ್ರತ್ಯಾಗಮ cpu_baseline::LUT8u_;
}

ಸ್ಥಿರ LUT16uFunc resolveLUT16uFunc()
{
#ಯದಿ CV_TRY_AVX512_ICL
    ಯದಿ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ಪ್ರತ್ಯಾಗಮ opt_AVX512_ICL::LUT16u_;
#endif
    ಪ್ರತ್ಯಾಗಮ cpu_baseline::LUT16u_;
}

} // namespace

ಶೂನ್ಯ LUT8u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* lut, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn );

ಶೂನ್ಯ LUT8u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ uchar* lut, uchar* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn )
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರ LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

ಶೂನ್ಯ LUT16u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ ushort* lut, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn );

ಶೂನ್ಯ LUT16u_dispatch( ಧ್ರುವ uchar* src, ಧ್ರುವ ushort* lut, ushort* dst, ಪೂರ್ಣಾಙ್ಕ len, ಪೂರ್ಣಾಙ್ಕ cn, ಪೂರ್ಣಾಙ್ಕ lutcn )
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರ LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
