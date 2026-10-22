// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html
// Copyright (C) 2026, Advanced Micro Devices, Inc., all rights reserved.

#include "precomp.hpp"

#include "lut.simd.hpp"
#include "lut.simd_declarations.hpp" // defines CV_CPU_DISPATCH_MODES_ALL=AVX512_ICL,...,BASELINE based on CMakeLists.txt content

namespace cv {

namespace {

typedef void (*LUT8uFunc)( const uchar*, const uchar*, uchar*, 整数, 整数, 整数 );
typedef void (*LUT16uFunc)( const uchar*, const ushort*, ushort*, 整数, 整数, 整数 );

static LUT8uFunc resolveLUT8uFunc()
{
#もし CV_TRY_AVX512_ICL
    もし (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        戻る opt_AVX512_ICL::LUT8u_;
#endif
    戻る cpu_baseline::LUT8u_;
}

static LUT16uFunc resolveLUT16uFunc()
{
#もし CV_TRY_AVX512_ICL
    もし (cv::checkHardwareSupport(CV_CPU_AVX512_ICL))
        戻る opt_AVX512_ICL::LUT16u_;
#endif
    戻る cpu_baseline::LUT16u_;
}

} // namespace

void LUT8u_dispatch( const uchar* src, const uchar* lut, uchar* dst, 整数 len, 整数 cn, 整数 lutcn );

void LUT8u_dispatch( const uchar* src, const uchar* lut, uchar* dst, 整数 len, 整数 cn, 整数 lutcn )
{
    CV_INSTRUMENT_REGION();
    static LUT8uFunc fn = resolveLUT8uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

void LUT16u_dispatch( const uchar* src, const ushort* lut, ushort* dst, 整数 len, 整数 cn, 整数 lutcn );

void LUT16u_dispatch( const uchar* src, const ushort* lut, ushort* dst, 整数 len, 整数 cn, 整数 lutcn )
{
    CV_INSTRUMENT_REGION();
    static LUT16uFunc fn = resolveLUT16uFunc();
    fn(src, lut, dst, len, cn, lutcn);
}

} // namespace cv
