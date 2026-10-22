// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ "lut.simd.hpp"
#അന്തര്ഭാവ "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

നാമാകാശ cv {

നാമാകാശ {

തരനിർവചനം ശൂന്യം (*LUT8uFunc)( സ്ഥിരം uchar*, സ്ഥിരം uchar*, uchar*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ );
തരനിർവചനം ശൂന്യം (*LUT16uFunc)( സ്ഥിരം uchar*, സ്ഥിരം ushort*, ushort*, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ, പൂർണ്ണസംഖ്യ );

സ്ഥിതം LUT8uFunc resolveLUT8uFunc()
{
#എങ്കിൽ CV_TRY_AVX512_ICL
    എങ്കിൽ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        തിരികെ opt_AVX512_ICL::LUT8u_;
#endif
    തിരികെ cpu_baseline::LUT8u_;
}

സ്ഥിതം LUT16uFunc resolveLUT16uFunc()
{
#എങ്കിൽ CV_TRY_AVX512_ICL
    എങ്കിൽ (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        തിരികെ opt_AVX512_ICL::LUT16u_;
#endif
    തിരികെ cpu_baseline::LUT16u_;
}

} // namespace

ശൂന്യം LUT8u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം uchar* lut, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn );

ശൂന്യം LUT8u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം uchar* lut, uchar* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn )
{
    CV_INSTRUMENT_REGION();
    സ്ഥിതം LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

ശൂന്യം LUT16u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം ushort* lut, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn );

ശൂന്യം LUT16u_dispatch( സ്ഥിരം uchar* src, സ്ഥിരം ushort* lut, ushort* dst, പൂർണ്ണസംഖ്യ len, പൂർണ്ണസംഖ്യ cn, പൂർണ്ണസംഖ്യ lutcn )
{
    CV_INSTRUMENT_REGION();
    സ്ഥിതം LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
