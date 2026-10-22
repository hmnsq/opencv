// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "lut.simd.hpp"
#ಅನ್ತರ್ಭಾವ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

ನಾಮಾಕಾಶ cv {

ನಾಮಾಕಾಶ {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*LUT8uFunc)( ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ uchar*, uchar*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ );
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*LUT16uFunc)( ಸ್ಥಿರಾಂಕ uchar*, ಸ್ಥಿರಾಂಕ ushort*, ushort*, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ, ಪೂರ್ಣಾಂಕ );

ಸ್ಥಿರ LUT8uFunc resolveLUT8uFunc()
{
#ಆದರೆ CV_TRY_AVX512_ICL
    ಆದರೆ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ಹಿಂತಿರುಗಿ opt_AVX512_ICL::LUT8u_;
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ cpu_baseline::LUT8u_;
}

ಸ್ಥಿರ LUT16uFunc resolveLUT16uFunc()
{
#ಆದರೆ CV_TRY_AVX512_ICL
    ಆದರೆ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        ಹಿಂತಿರುಗಿ opt_AVX512_ICL::LUT16u_;
#ಆದರೆ_ಅಂತ್ಯ
    ಹಿಂತಿರುಗಿ cpu_baseline::LUT16u_;
}

} // namespace

ಶೂನ್ಯ LUT8u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* lut, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );

ಶೂನ್ಯ LUT8u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ uchar* lut, uchar* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರ LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

ಶೂನ್ಯ LUT16u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ ushort* lut, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn );

ಶೂನ್ಯ LUT16u_dispatch( ಸ್ಥಿರಾಂಕ uchar* src, ಸ್ಥಿರಾಂಕ ushort* lut, ushort* dst, ಪೂರ್ಣಾಂಕ len, ಪೂರ್ಣಾಂಕ cn, ಪೂರ್ಣಾಂಕ lutcn )
{
    CV_INSTRUMENT_REGION();
    ಸ್ಥಿರ LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
