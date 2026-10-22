// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "precomp.hpp"

#include "lut.simd.hpp"
#include "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace {

typedef void (*LUT8uFunc)( const uchar*, const uchar*, uchar*, số_nguyên, số_nguyên, số_nguyên );
typedef void (*LUT16uFunc)( const uchar*, const ushort*, ushort*, số_nguyên, số_nguyên, số_nguyên );

static LUT8uFunc resolveLUT8uFunc()
{
#nếu CV_TRY_AVX512_ICL
    nếu (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        trả_về opt_AVX512_ICL::LUT8u_;
#endif
    trả_về cpu_baseline::LUT8u_;
}

static LUT16uFunc resolveLUT16uFunc()
{
#nếu CV_TRY_AVX512_ICL
    nếu (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        trả_về opt_AVX512_ICL::LUT16u_;
#endif
    trả_về cpu_baseline::LUT16u_;
}

} // namespace

void LUT8u_dispatch( const uchar* src, const uchar* lut, uchar* dst, số_nguyên len, số_nguyên cn, số_nguyên lutcn );

void LUT8u_dispatch( const uchar* src, const uchar* lut, uchar* dst, số_nguyên len, số_nguyên cn, số_nguyên lutcn )
{
    CV_INSTRUMENT_REGION();
    static LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

void LUT16u_dispatch( const uchar* src, const ushort* lut, ushort* dst, số_nguyên len, số_nguyên cn, số_nguyên lutcn );

void LUT16u_dispatch( const uchar* src, const ushort* lut, ushort* dst, số_nguyên len, số_nguyên cn, số_nguyên lutcn )
{
    CV_INSTRUMENT_REGION();
    static LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
