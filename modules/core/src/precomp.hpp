/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                           License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ __OPENCV_PRECOMP_H__
#ವ್ಯಾಖ್ಯಾನಿಸು __OPENCV_PRECOMP_H__

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ _MSC_VER
#  define SILENCE_CXX17_C_ಶೀರ್ಷಿಕೆ_DEPRECATION_WARNING
#  define SILENCE_CXX17_CODECVT_ಶೀರ್ಷಿಕೆ_DEPRECATION_WARNING
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ BUILD_PLUGIN
#ಅನ್ತರ್ಭಾವ "opencv2/core/utility.hpp"
#ಇಲ್ಲದಿದ್ದರೆ  // BUILD_PLUGIN

#ಅನ್ತರ್ಭಾವ "opencv2/opencv_modules.hpp"
#ಅನ್ತರ್ಭಾವ "cvconfig.h"

#ಅನ್ತರ್ಭಾವ "opencv2/core/utility.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/cuda.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opengl.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/va_intel.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/private.cuda.hpp"
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl.hpp"
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ <ctype.h>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <math.h>
#ಅನ್ತರ್ಭಾವ <stdarg.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

#ಅನ್ತರ್ಭಾವ <algorithm>
#ಅನ್ತರ್ಭಾವ <cmath>
#ಅನ್ತರ್ಭಾವ <cstdlib>
#ಅನ್ತರ್ಭಾವ <limits>
#ಅನ್ತರ್ಭಾವ <float.h>
#ಅನ್ತರ್ಭಾವ <cstring>
#ಅನ್ತರ್ಭಾವ <cassert>

#ವ್ಯಾಖ್ಯಾನಿಸು USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#ವ್ಯಾಖ್ಯಾನಿಸು USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#ವ್ಯಾಖ್ಯಾನಿಸು USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#ವ್ಯಾಖ್ಯಾನಿಸು USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/hal.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/neon_utils.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/vsx_utils.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"


ನಾಮಾಕಾಶ cv
{

// -128.f ... 255.f
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ g_8x32fTab[];
#ವ್ಯಾಖ್ಯಾನಿಸು CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ushort g_8x16uSqrTab[];
#ವ್ಯಾಖ್ಯಾನಿಸು CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ uchar g_Saturate8u[];
#ವ್ಯಾಖ್ಯಾನಿಸು CV_FAST_CAST_8U(t)   (ಪ್ರತಿಜ್ಞಾ(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#ವ್ಯಾಖ್ಯಾನಿಸು CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#ವ್ಯಾಖ್ಯಾನಿಸು CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ OpNop
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(a); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ OpSqr
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ Op_ಕೂಡಿಸು
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a, ಸ್ಥಿರಾಂಕ T2 b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(a + b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ Op_ಕೂಡಿಸು_Sqr
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a, ಸ್ಥಿರಾಂಕ T2 b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ OpSub
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a, ಸ್ಥಿರಾಂಕ T2 b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(a - b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T1, ಬಗೆ_ಹೆಸರು T2=T1, ಬಗೆ_ಹೆಸರು T3=T1> ರಚನೆ OpRSub
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T1 type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T2 type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T3 rtype;
    T3 ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T1 a, ಸ್ಥಿರಾಂಕ T2 b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ saturate_cast<T3>(b - a); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpMin
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T a, ಸ್ಥಿರಾಂಕ T b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ std::min(a, b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpMax
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ ()(ಸ್ಥಿರಾಂಕ T a, ಸ್ಥಿರಾಂಕ T b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ std::max(a, b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpAbsDiff
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ()(T a, T b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
ಮಾದರಿ<> ರಚನೆ OpAbsDiff<ದಶಮಾಂಶ>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದಶಮಾಂಶ rtype;
    ದಶಮಾಂಶ ಕ್ರಿಯಾಕಾರಕ()(ದಶಮಾಂಶ a, ದಶಮಾಂಶ b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ std::ನಿರಪೇಕ್ಷ(a - b); }
};
ಮಾದರಿ<> ರಚನೆ OpAbsDiff<ದ್ವಿಗುಣ>
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ದ್ವಿಗುಣ rtype;
    ದ್ವಿಗುಣ ಕ್ರಿಯಾಕಾರಕ()(ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ std::ನಿರಪೇಕ್ಷ(a - b); }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpAnd
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ()( T a, T b ) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ a & b; }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpOr
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ()( T a, T b ) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ a | b; }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpXor
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ()( T a, T b ) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ a ^ b; }
};

ಮಾದರಿ<ಬಗೆ_ಹೆಸರು T> ರಚನೆ OpNot
{
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type1;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T type2;
    ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ T rtype;
    T ಕ್ರಿಯಾಕಾರಕ()( T a, T ) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ ~a; }
};

ಮಾದರಿ<> ಸಾಲೊಳಗೆ uchar Op_ಕೂಡಿಸು<uchar>::ಕ್ರಿಯಾಕಾರಕ ()(uchar a, uchar b) ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ CV_FAST_CAST_8U(a + b); }

ಮಾದರಿ<> ಸಾಲೊಳಗೆ uchar OpSub<uchar>::ಕ್ರಿಯಾಕಾರಕ ()(uchar a, uchar b) ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ CV_FAST_CAST_8U(a - b); }

ಮಾದರಿ<> ಸಾಲೊಳಗೆ ಗಿಡ್ಡ OpAbsDiff<ಗಿಡ್ಡ>::ಕ್ರಿಯಾಕಾರಕ ()(ಗಿಡ್ಡ a, ಗಿಡ್ಡ b) ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ saturate_cast<ಗಿಡ್ಡ>(std::ನಿರಪೇಕ್ಷ(a - b)); }

ಮಾದರಿ<> ಸಾಲೊಳಗೆ schar OpAbsDiff<schar>::ಕ್ರಿಯಾಕಾರಕ ()(schar a, schar b) ಸ್ಥಿರಾಂಕ
{ ಹಿಂತಿರುಗಿ saturate_cast<schar>(std::ನಿರಪೇಕ್ಷ(a - b)); }

ಮಾದರಿ<> ಸಾಲೊಳಗೆ uchar OpMin<uchar>::ಕ್ರಿಯಾಕಾರಕ ()(uchar a, uchar b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ CV_MIN_8U(a, b); }

ಮಾದರಿ<> ಸಾಲೊಳಗೆ uchar OpMax<uchar>::ಕ್ರಿಯಾಕಾರಕ ()(uchar a, uchar b) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ CV_MAX_8U(a, b); }

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*UnaryFunc)(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1,
                       uchar* dst, size_t ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 sz,
                       ಶೂನ್ಯ*);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ದ್ವಿಮಾನ_Func)(ಸ್ಥಿರಾಂಕ uchar* src1, size_t step1,
                       ಸ್ಥಿರಾಂಕ uchar* src2, size_t step2,
                       uchar* dst, size_t ಹೆಜ್ಜೆ, ಗಾತ್ರ_2 sz,
                       ಶೂನ್ಯ*);

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಶೂನ್ಯ (*ದ್ವಿಮಾನ_Func_C)(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src1, size_t step1,
                       ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ* src2, size_t step2,
                       ಶೂನ್ಯ* dst, size_t ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ,
                       ಶೂನ್ಯ*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
CV_EXPORTS ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಂಕ sdepth, ಪೂರ್ಣಾಂಕ ddepth);
ದ್ವಿಮಾನ_Func get_Copy_ಮುಖವಾಡ_Func(size_t esz);

/* default memory block for sparse array elements */
#ವ್ಯಾಖ್ಯಾನಿಸು  CV_SPARSE_MAT_ಖಂಡ     (1<<12)

/* initial hash table size */
#ವ್ಯಾಖ್ಯಾನಿಸು  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#ವ್ಯಾಖ್ಯಾನಿಸು  CV_SPARSE_HASH_ಅನುಪಾತ    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale=1);
ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, Mat& m2, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale=1);
ಗಾತ್ರ_2 getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ಪೂರ್ಣಾಂಕ ಅಗಲ_Scale=1);

ಶೂನ್ಯ ಗಣ_ಗಾತ್ರ( Mat& m, ಪೂರ್ಣಾಂಕ _dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* _sz, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆಗಳು_2, ತಾರ್ಕಿಕ auto_ಹೆಜ್ಜೆಗಳು=ಸುಳ್ಳು );
ಶೂನ್ಯ finalizeHdr(Mat& m);
ಪೂರ್ಣಾಂಕ updateContinuityFlag(ಪೂರ್ಣಾಂಕ flags, ಪೂರ್ಣಾಂಕ dims, ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ* size, ಸ್ಥಿರಾಂಕ size_t* ಹೆಜ್ಜೆ);

ರಚನೆ NoVec
{
    size_t ಕ್ರಿಯಾಕಾರಕ()(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ*, ಶೂನ್ಯ*, size_t) ಸ್ಥಿರಾಂಕ { ಹಿಂತಿರುಗಿ 0; }
};

#ವ್ಯಾಖ್ಯಾನಿಸು CV_ವಿಭಜಿಸು_ವಿಲೀನ_MAX_ಖಂಡ_ಗಾತ್ರ(cn) ((ಗರಿಷ್ಠ_ಪೂರ್ಣಾಂಕ/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

ಎಣಿಕೆ { ಖಂಡ_ಗಾತ್ರ = 1024 };

#ಆದರೆ defined HAVE_IPP && (IPP_ಆವೃತ್ತಿ_X100 >= 700)
#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_USE_IPP 1
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ARITHM_USE_IPP 0
#ಆದರೆ_ಅಂತ್ಯ

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ checkScalar(ಸ್ಥಿರಾಂಕ Mat& sc, ಪೂರ್ಣಾಂಕ atype, ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag sckind, ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag akind)
{
    ಆದರೆ( sc.dims > 2 || !sc.isContinuous() )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾತ್ರ_2 sz = sc.size();
    ಆದರೆ(sz.ಅಗಲ != 1 && sz.ಎತ್ತರ != 1)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(atype);
    ಆದರೆ( akind == ಒಳಹರಿವು_Array_2::MATX && sckind != ಒಳಹರಿವು_Array_2::MATX )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ sz == ಗಾತ್ರ_2(1, 1) || sz == ಗಾತ್ರ_2(1, cn) || sz == ಗಾತ್ರ_2(cn, 1) ||
           (sz == ಗಾತ್ರ_2(1, 4) && sc.type() == CV_64F && cn <= 4);
}

ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ checkScalar(ಒಳಹರಿವು_Array sc, ಪೂರ್ಣಾಂಕ atype, ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag sckind, ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag akind)
{
    ಆದರೆ( sc.dims() > 2 || !sc.isContinuous() )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಗಾತ್ರ_2 sz = sc.size();
    ಆದರೆ(sz.ಅಗಲ != 1 && sz.ಎತ್ತರ != 1)
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಪೂರ್ಣಾಂಕ cn = CV_MAT_CN(atype);
    ಆದರೆ( akind == ಒಳಹರಿವು_Array_2::MATX && sckind != ಒಳಹರಿವು_Array_2::MATX )
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ sz == ಗಾತ್ರ_2(1, 1) || sz == ಗಾತ್ರ_2(1, cn) || sz == ಗಾತ್ರ_2(cn, 1) ||
           (sz == ಗಾತ್ರ_2(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ sameShapeAsArg(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& sc, ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr)
{
    ಹಿಂತಿರುಗಿ arr.channels() == 1 && sc.same_ಗಾತ್ರ(arr);
}

// New element-wise engine scalar handling. A genuine number / Scalar / Vec / Matx operand to an
// arithmetic op arrives via _InputArray::MATX (its data is inline in the caller's object;
// getObj() points straight at it). In addition, the EXACT Scalar materialization that the
// python/java bindings and operator-(Mat, Matx) produce - a 2-D 4x1 CV_64F single-channel Mat or
// UMat - is a scalar UNCONDITIONALLY: by coincidence it can be broadcast-COMPATIBLE with the array
// (a 4-row array, a 1-D array make (4,1) legal numpy-wise), and the 4.x per-channel-scalar
// semantics must win there for binding users. Any other real Mat/UMat rides normal broadcasting
// (but see isScalarLikeMat below for the shape-incompatible compat fallback).
// `arr` is the OTHER operand: its channel count decides what a MATX scalar may broadcast over, its
// shape whether the bindings-style column below is a scalar at all.
ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isScalarArg(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& sc, ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& arr)
{
    ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag ಬಗೆ = sc.ಬಗೆ();
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ cn = arr.channels();
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::MATX)
    {
        ಗಾತ್ರ_2 sz = sc.getSz();
        ಪೂರ್ಣಾಂಕ scn0 = sz.ಅಗಲ * sz.ಎತ್ತರ;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        ಆದರೆ (scn0 != sz.ಅಗಲ + sz.ಎತ್ತರ - 1)
            ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ಹಿಂತಿರುಗಿ scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)sc.getObj();
        ಹಿಂತಿರುಗಿ m.ಅಡ್ಡಸಾಲುಗಳು == 4 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಸ್ಥಿರಾಂಕ UMat& m = *(ಸ್ಥಿರಾಂಕ UMat*)sc.getObj();
        ಹಿಂತಿರುಗಿ m.ಅಡ್ಡಸಾಲುಗಳು == 4 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
ಸಾಲೊಳಗೆ ತಾರ್ಕಿಕ isScalarLikeMat(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& sc, ಪೂರ್ಣಾಂಕ cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, nval;
    ತಾರ್ಕಿಕ continuous;
    ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag ಬಗೆ = sc.ಬಗೆ();
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)sc.getObj();
        ಆದರೆ ((m.ಅಡ್ಡಸಾಲುಗಳು != 1 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != 1) || m.dims > 2) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; nval = ಅಡ್ಡಸಾಲುಗಳು * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * m.channels(); continuous = m.isContinuous();
    }
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಸ್ಥಿರಾಂಕ UMat& m = *(ಸ್ಥಿರಾಂಕ UMat*)sc.getObj();
        ಆದರೆ ((m.ಅಡ್ಡಸಾಲುಗಳು != 1 && m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ != 1) || m.dims > 2) ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
        ಅಡ್ಡಸಾಲುಗಳು = m.ಅಡ್ಡಸಾಲುಗಳು; ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; nval = ಅಡ್ಡಸಾಲುಗಳು * ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ * m.channels(); continuous = m.isContinuous();
    }
    ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಆದರೆ (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;
    ಹಿಂತಿರುಗಿ (ಅಡ್ಡಸಾಲುಗಳು == 1 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) || (ಅಡ್ಡಸಾಲುಗಳು == cn && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == 1) || (ಅಡ್ಡಸಾಲುಗಳು == 1 && ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
ಎಣಿಕೆ { EW_SCALAR_BUF_ಗಾತ್ರ = 4 * ಗಾತ್ರ(ದ್ವಿಗುಣ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ scalarArgElems(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& sc, ಸ್ಥಿರಾಂಕ uchar*& p, ಪೂರ್ಣಾಂಕ& d, uchar* scbuf)
{
    ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2::ಬಗೆ_Flag ಬಗೆ = sc.ಬಗೆ();
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಸ್ಥಿರಾಂಕ Mat& m = *(ಸ್ಥಿರಾಂಕ Mat*)sc.getObj();
        p = m.ದತ್ತಾಂಶ;
        d = m.ಆಳ();
        ಹಿಂತಿರುಗಿ (ಪೂರ್ಣಾಂಕ)m.ಒಟ್ಟು() * m.channels();
    }
    ಆದರೆ (ಬಗೆ == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಸ್ಥಿರಾಂಕ UMat& u = *(ಸ್ಥಿರಾಂಕ UMat*)sc.getObj();
        d = u.ಆಳ();
        ಪೂರ್ಣಾಂಕ n = (ಪೂರ್ಣಾಂಕ)u.ಒಟ್ಟು() * u.channels();
        CV_Assert(n * (ಪೂರ್ಣಾಂಕ)CV_ELEM_SIZE1(d) <= (ಪೂರ್ಣಾಂಕ)EW_SCALAR_BUF_ಗಾತ್ರ);
        Mat ಶೀರ್ಷಿಕೆ(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copy_ಗಮ್ಯ(ಶೀರ್ಷಿಕೆ);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ಹಿಂತಿರುಗಿ n;
    }
    p = (ಸ್ಥಿರಾಂಕ uchar*)sc.getObj();
    d = sc.ಆಳ();
    ಗಾತ್ರ_2 sz = sc.getSz();
    ಹಿಂತಿರುಗಿ sz.ಅಗಲ * sz.ಎತ್ತರ;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
ಸಾಲೊಳಗೆ ದ್ವಿಗುಣ elem_ಗಮ್ಯ_Double(ಪೂರ್ಣಾಂಕ d, ಸ್ಥಿರಾಂಕ uchar* p)
{
    ಆಯ್ಕೆ (d)
    {
    ಸ್ಥಿತಿ CV_8U:   ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ uchar*)p;
    ಸ್ಥಿತಿ CV_8S:   ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ schar*)p;
    ಸ್ಥಿತಿ CV_16U:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ushort*)p;
    ಸ್ಥಿತಿ CV_16S:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ಗಿಡ್ಡ*)p;
    ಸ್ಥಿತಿ CV_32U:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ*)p;
    ಸ್ಥಿತಿ CV_32S:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ*)p;
    ಸ್ಥಿತಿ CV_64U:  ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)*(ಸ್ಥಿರಾಂಕ uint64_t*)p;
    ಸ್ಥಿತಿ CV_64S:  ಹಿಂತಿರುಗಿ (ದ್ವಿಗುಣ)*(ಸ್ಥಿರಾಂಕ int64_t*)p;
    ಸ್ಥಿತಿ CV_16F:  ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)*(ಸ್ಥಿರಾಂಕ hfloat*)p;
    ಸ್ಥಿತಿ CV_16BF: ಹಿಂತಿರುಗಿ (ದಶಮಾಂಶ)*(ಸ್ಥಿರಾಂಕ bfloat*)p;
    ಸ್ಥಿತಿ CV_32F:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ*)p;
    ಸ್ಥಿತಿ CV_64F:  ಹಿಂತಿರುಗಿ *(ಸ್ಥಿರಾಂಕ ದ್ವಿಗುಣ*)p;
    ಪೂರ್ವನಿಯೋಜಿತ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ readScalarArg(ಸ್ಥಿರಾಂಕ ಒಳಹರಿವು_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ಸ್ಥಿರಾಂಕ uchar* p; ಪೂರ್ಣಾಂಕ d; uchar scbuf[EW_SCALAR_BUF_ಗಾತ್ರ];
    ಪೂರ್ಣಾಂಕ n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < n; i++)
        out[i] = elem_ಗಮ್ಯ_Double(d, p + (size_t)i * esz);
    ಹಿಂತಿರುಗಿ n;
}

ಶೂನ್ಯ convertAndUnrollScalar( ಸ್ಥಿರಾಂಕ Mat& sc, ಪೂರ್ಣಾಂಕ buftype, uchar* scbuf, size_t blocksize );

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CV_COLLECT_IMPL_ದತ್ತಾಂಶ
ರಚನೆ ImplCollector
{
    ImplCollector()
    {
        useCollection   = ಸುಳ್ಳು;
        implFlags       = 0;
    }
    ತಾರ್ಕಿಕ useCollection; // enable/disable impl data collection

    ಪೂರ್ಣಾಂಕ implFlags;
    std::vector<ಪೂರ್ಣಾಂಕ>    impl_ಸಂಕೇತ;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(ಸುಳ್ಳು), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    ತಾರ್ಕಿಕ oclExecutionContextInitialized;
    ಪೂರ್ಣಾಂಕ useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    ಪೂರ್ಣಾಂಕ useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    ಪೂರ್ಣಾಂಕ useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_ದತ್ತಾಂಶ();

#ಆದರೆ defined(BUILD_SHARED_LIBS)
#ಆದರೆ defined _WIN32 || defined WINCE
#ವ್ಯಾಖ್ಯಾನಿಸು CL_RUNTIME_EXPORT __declspec(dllexport)
#ಅಥವಾಆದರೆ defined __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CL_RUNTIME_EXPORT
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು CL_RUNTIME_EXPORT
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ CV_EXPORTS
ತಾರ್ಕಿಕ __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#ವ್ಯಾಖ್ಯಾನಿಸು CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_ಮೌಲ್ಯ) \
    ಸ್ಥಿರ TYPE* ಸ್ಥಿರಾಂಕ instance = INITIALIZER; \
    ಹಿಂತಿರುಗಿ RET_ಮೌಲ್ಯ;

#ವ್ಯಾಖ್ಯಾನಿಸು CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#ವ್ಯಾಖ್ಯಾನಿಸು CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS ಶೂನ್ಯ releaseTlsStorageThread();

ಪೂರ್ಣಾಂಕ cv_snprintf(ಅಕ್ಷರ* buf, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt, ...);
ಪೂರ್ಣಾಂಕ cv_vsnprintf(ಅಕ್ಷರ* buf, ಪೂರ್ಣಾಂಕ len, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* fmt, va_list ವಾದಗಳು);
}

#ಆದರೆ_ಅಂತ್ಯ  // BUILD_PLUGIN
#ಆದರೆ_ಅಂತ್ಯ  // __OPENCV_PRECOMP_H__
