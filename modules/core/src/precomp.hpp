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

#ifndef __OPENCV_PRECOMP_H__
#define __OPENCV_PRECOMP_H__

#ifdef _MSC_VER
#  define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#  define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

#ifdef BUILD_PLUGIN
#ಅನ್ತರ್ಭಾವ "opencv2/core/utility.hpp"
#ಅನ್ಯಥಾ  // BUILD_PLUGIN

#ಅನ್ತರ್ಭಾವ "opencv2/opencv_modules.hpp"
#ಅನ್ತರ್ಭಾವ "cvconfig.h"

#ಅನ್ತರ್ಭಾವ "opencv2/core/utility.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/cuda.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/opengl.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/va_intel.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core/private.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl.hpp"
#endif

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

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/hal.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/hal/intrin.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/neon_utils.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/vsx_utils.hpp"
#ಅನ್ತರ್ಭಾವ "hal_replacement.hpp"


ನಾಮಾಕಾಶ cv
{

// -128.f ... 255.f
ಬಾಹ್ಯ ಧ್ರುವ ಭಿನ್ನ g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

ಬಾಹ್ಯ ಧ್ರುವ ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

ಬಾಹ್ಯ ಧ್ರುವ uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (ಪ್ರತಿಜ್ಞಾ(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpNop
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpSqr
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpAdd
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a, ಧ್ರುವ T2 b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpAddSqr
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a, ಧ್ರುವ T2 b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpSub
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a, ಧ್ರುವ T2 b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ಸಂರಚನಾ OpRSub
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T1 type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T2 type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T3 rtype;
    T3 operator ()(ಧ್ರುವ T1 a, ಧ್ರುವ T2 b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ saturate_cast<T3>(b - a); }
};

template<typename T> ಸಂರಚನಾ OpMin
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator ()(ಧ್ರುವ T a, ಧ್ರುವ T b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::min(a, b); }
};

template<typename T> ಸಂರಚನಾ OpMax
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator ()(ಧ್ರುವ T a, ಧ್ರುವ T b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::max(a, b); }
};

template<typename T> ಸಂರಚನಾ OpAbsDiff
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator()(T a, T b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> ಸಂರಚನಾ OpAbsDiff<ಭಿನ್ನ>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ ಭಿನ್ನ rtype;
    ಭಿನ್ನ operator()(ಭಿನ್ನ a, ಭಿನ್ನ b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::abs(a - b); }
};
template<> ಸಂರಚನಾ OpAbsDiff<ದ್ವಿಗುಣ>
{
    ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ ದ್ವಿಗುಣ rtype;
    ದ್ವಿಗುಣ operator()(ದ್ವಿಗುಣ a, ದ್ವಿಗುಣ b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ std::abs(a - b); }
};

template<typename T> ಸಂರಚನಾ OpAnd
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator()( T a, T b ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ a & b; }
};

template<typename T> ಸಂರಚನಾ OpOr
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator()( T a, T b ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ a | b; }
};

template<typename T> ಸಂರಚನಾ OpXor
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator()( T a, T b ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ a ^ b; }
};

template<typename T> ಸಂರಚನಾ OpNot
{
    ಪ್ರಕಾರನಿರ್ದೇಶ T type1;
    ಪ್ರಕಾರನಿರ್ದೇಶ T type2;
    ಪ್ರಕಾರನಿರ್ದೇಶ T rtype;
    T operator()( T a, T ) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ ~a; }
};

template<> ಅನ್ತಃಸ್ಥ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ CV_FAST_CAST_8U(a + b); }

template<> ಅನ್ತಃಸ್ಥ uchar OpSub<uchar>::operator ()(uchar a, uchar b) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ CV_FAST_CAST_8U(a - b); }

template<> ಅನ್ತಃಸ್ಥ short OpAbsDiff<short>::operator ()(short a, short b) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ saturate_cast<short>(std::abs(a - b)); }

template<> ಅನ್ತಃಸ್ಥ schar OpAbsDiff<schar>::operator ()(schar a, schar b) ಧ್ರುವ
{ ಪ್ರತ್ಯಾಗಮ saturate_cast<schar>(std::abs(a - b)); }

template<> ಅನ್ತಃಸ್ಥ uchar OpMin<uchar>::operator ()(uchar a, uchar b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ CV_MIN_8U(a, b); }

template<> ಅನ್ತಃಸ್ಥ uchar OpMax<uchar>::operator ()(uchar a, uchar b) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ CV_MAX_8U(a, b); }

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*UnaryFunc)(ಧ್ರುವ uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       ಶೂನ್ಯ*);

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ದ್ವಿಮಾನ_Func)(ಧ್ರುವ uchar* src1, size_t step1,
                       ಧ್ರುವ uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       ಶೂನ್ಯ*);

ಪ್ರಕಾರನಿರ್ದೇಶ ಶೂನ್ಯ (*ದ್ವಿಮಾನ_Func_C)(ಧ್ರುವ ಶೂನ್ಯ* src1, size_t step1,
                       ಧ್ರುವ ಶೂನ್ಯ* src2, size_t step2,
                       ಶೂನ್ಯ* dst, size_t step, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height,
                       ಶೂನ್ಯ*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ದ್ವಿಮಾನ_Func getConvertFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth);
CV_EXPORTS ದ್ವಿಮಾನ_Func getConvertScaleFunc(ಪೂರ್ಣಾಙ್ಕ sdepth, ಪೂರ್ಣಾಙ್ಕ ddepth);
ದ್ವಿಮಾನ_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, ಪೂರ್ಣಾಙ್ಕ widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, ಪೂರ್ಣಾಙ್ಕ widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ಪೂರ್ಣಾಙ್ಕ widthScale=1);

ಶೂನ್ಯ setSize( Mat& m, ಪೂರ್ಣಾಙ್ಕ _dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* _sz, ಧ್ರುವ size_t* _steps, ತಾರ್ಕಿಕ autoSteps=ಅಸತ್ಯ );
ಶೂನ್ಯ finalizeHdr(Mat& m);
ಪೂರ್ಣಾಙ್ಕ updateContinuityFlag(ಪೂರ್ಣಾಙ್ಕ flags, ಪೂರ್ಣಾಙ್ಕ dims, ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ* size, ಧ್ರುವ size_t* step);

ಸಂರಚನಾ NoVec
{
    size_t operator()(ಧ್ರುವ ಶೂನ್ಯ*, ಧ್ರುವ ಶೂನ್ಯ*, ಶೂನ್ಯ*, size_t) ಧ್ರುವ { ಪ್ರತ್ಯಾಗಮ 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

ಗಣನಾ { BLOCK_SIZE = 1024 };

#ಯದಿ defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#ಅನ್ಯಥಾ
#define ARITHM_USE_IPP 0
#endif

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ checkScalar(ಧ್ರುವ Mat& sc, ಪೂರ್ಣಾಙ್ಕ atype, ಒಳಹರಿವು_Array_2::KindFlag sckind, ಒಳಹರಿವು_Array_2::KindFlag akind)
{
    ಯದಿ( sc.dims > 2 || !sc.isContinuous() )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    Size sz = sc.size();
    ಯದಿ(sz.width != 1 && sz.height != 1)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(atype);
    ಯದಿ( akind == ಒಳಹರಿವು_Array_2::MATX && sckind != ಒಳಹರಿವು_Array_2::MATX )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ checkScalar(ಒಳಹರಿವು_Array sc, ಪೂರ್ಣಾಙ್ಕ atype, ಒಳಹರಿವು_Array_2::KindFlag sckind, ಒಳಹರಿವು_Array_2::KindFlag akind)
{
    ಯದಿ( sc.dims() > 2 || !sc.isContinuous() )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    Size sz = sc.size();
    ಯದಿ(sz.width != 1 && sz.height != 1)
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪೂರ್ಣಾಙ್ಕ cn = CV_MAT_CN(atype);
    ಯದಿ( akind == ಒಳಹರಿವು_Array_2::MATX && sckind != ಒಳಹರಿವು_Array_2::MATX )
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ sameShapeAsArg(ಧ್ರುವ ಒಳಹರಿವು_Array_2& sc, ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr)
{
    ಪ್ರತ್ಯಾಗಮ arr.channels() == 1 && sc.sameSize(arr);
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
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isScalarArg(ಧ್ರುವ ಒಳಹರಿವು_Array_2& sc, ಧ್ರುವ ಒಳಹರಿವು_Array_2& arr)
{
    ಧ್ರುವ ಒಳಹರಿವು_Array_2::KindFlag kind = sc.kind();
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ cn = arr.channels();
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::MATX)
    {
        Size sz = sc.getSz();
        ಪೂರ್ಣಾಙ್ಕ scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        ಯದಿ (scn0 != sz.width + sz.height - 1)
            ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ಪ್ರತ್ಯಾಗಮ scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)sc.getObj();
        ಪ್ರತ್ಯಾಗಮ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಧ್ರುವ UMat& m = *(ಧ್ರುವ UMat*)sc.getObj();
        ಪ್ರತ್ಯಾಗಮ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
ಅನ್ತಃಸ್ಥ ತಾರ್ಕಿಕ isScalarLikeMat(ಧ್ರುವ ಒಳಹರಿವು_Array_2& sc, ಪೂರ್ಣಾಙ್ಕ cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    ಪೂರ್ಣಾಙ್ಕ rows, cols, nval;
    ತಾರ್ಕಿಕ continuous;
    ಧ್ರುವ ಒಳಹರಿವು_Array_2::KindFlag kind = sc.kind();
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)sc.getObj();
        ಯದಿ ((m.rows != 1 && m.cols != 1) || m.dims > 2) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ಅನ್ಯಥಾ ಯದಿ (kind == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಧ್ರುವ UMat& m = *(ಧ್ರುವ UMat*)sc.getObj();
        ಯದಿ ((m.rows != 1 && m.cols != 1) || m.dims > 2) ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ಅನ್ಯಥಾ
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಯದಿ (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;
    ಪ್ರತ್ಯಾಗಮ (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
ಗಣನಾ { EW_SCALAR_BUF_SIZE = 4 * ಪರಿಮಾಣ(ದ್ವಿಗುಣ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ scalarArgElems(ಧ್ರುವ ಒಳಹರಿವು_Array_2& sc, ಧ್ರುವ uchar*& p, ಪೂರ್ಣಾಙ್ಕ& d, uchar* scbuf)
{
    ಧ್ರುವ ಒಳಹರಿವು_Array_2::KindFlag kind = sc.kind();
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::MAT)
    {
        ಧ್ರುವ Mat& m = *(ಧ್ರುವ Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        ಪ್ರತ್ಯಾಗಮ (ಪೂರ್ಣಾಙ್ಕ)m.total() * m.channels();
    }
    ಯದಿ (kind == ಒಳಹರಿವು_Array_2::UMAT)
    {
        ಧ್ರುವ UMat& u = *(ಧ್ರುವ UMat*)sc.getObj();
        d = u.depth();
        ಪೂರ್ಣಾಙ್ಕ n = (ಪೂರ್ಣಾಙ್ಕ)u.total() * u.channels();
        CV_Assert(n * (ಪೂರ್ಣಾಙ್ಕ)CV_ELEM_SIZE1(d) <= (ಪೂರ್ಣಾಙ್ಕ)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ಪ್ರತ್ಯಾಗಮ n;
    }
    p = (ಧ್ರುವ uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    ಪ್ರತ್ಯಾಗಮ sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
ಅನ್ತಃಸ್ಥ ದ್ವಿಗುಣ elemToDouble(ಪೂರ್ಣಾಙ್ಕ d, ಧ್ರುವ uchar* p)
{
    ವಿಕಲ್ಪ (d)
    {
    ಪಕ್ಷ CV_8U:   ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ uchar*)p;
    ಪಕ್ಷ CV_8S:   ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ schar*)p;
    ಪಕ್ಷ CV_16U:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ ushort*)p;
    ಪಕ್ಷ CV_16S:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ short*)p;
    ಪಕ್ಷ CV_32U:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ unsigned*)p;
    ಪಕ್ಷ CV_32S:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ*)p;
    ಪಕ್ಷ CV_64U:  ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ)*(ಧ್ರುವ uint64_t*)p;
    ಪಕ್ಷ CV_64S:  ಪ್ರತ್ಯಾಗಮ (ದ್ವಿಗುಣ)*(ಧ್ರುವ int64_t*)p;
    ಪಕ್ಷ CV_16F:  ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)*(ಧ್ರುವ hfloat*)p;
    ಪಕ್ಷ CV_16BF: ಪ್ರತ್ಯಾಗಮ (ಭಿನ್ನ)*(ಧ್ರುವ bfloat*)p;
    ಪಕ್ಷ CV_32F:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ ಭಿನ್ನ*)p;
    ಪಕ್ಷ CV_64F:  ಪ್ರತ್ಯಾಗಮ *(ಧ್ರುವ ದ್ವಿಗುಣ*)p;
    ಸ್ವಭಾವ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
ಅನ್ತಃಸ್ಥ ಪೂರ್ಣಾಙ್ಕ readScalarArg(ಧ್ರುವ ಒಳಹರಿವು_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ಧ್ರುವ uchar* p; ಪೂರ್ಣಾಙ್ಕ d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    ಪೂರ್ಣಾಙ್ಕ n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    ಪ್ರತ್ಯಾಗಮ n;
}

ಶೂನ್ಯ convertAndUnrollScalar( ಧ್ರುವ Mat& sc, ಪೂರ್ಣಾಙ್ಕ buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
ಸಂರಚನಾ ImplCollector
{
    ImplCollector()
    {
        useCollection   = ಅಸತ್ಯ;
        implFlags       = 0;
    }
    ತಾರ್ಕಿಕ useCollection; // enable/disable impl data collection

    ಪೂರ್ಣಾಙ್ಕ implFlags;
    std::vector<ಪೂರ್ಣಾಙ್ಕ>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

ಸಂರಚನಾ CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(ಅಸತ್ಯ), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    ತಾರ್ಕಿಕ oclExecutionContextInitialized;
    ಪೂರ್ಣಾಙ್ಕ useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    ಪೂರ್ಣಾಙ್ಕ useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    ಪೂರ್ಣಾಙ್ಕ useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#ಯದಿ defined(BUILD_SHARED_LIBS)
#ಯದಿ defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#ನೋಚೇದ್ಯದಿ defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#ಅನ್ಯಥಾ
#define CL_RUNTIME_EXPORT
#endif
#ಅನ್ಯಥಾ
#define CL_RUNTIME_EXPORT
#endif

ಬಾಹ್ಯ CV_EXPORTS
ತಾರ್ಕಿಕ __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_ಮೌಲ್ಯ) \
    ಸ್ಥಿರ TYPE* ಧ್ರುವ instance = INITIALIZER; \
    ಪ್ರತ್ಯಾಗಮ RET_ಮೌಲ್ಯ;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS ಶೂನ್ಯ releaseTlsStorageThread();

ಪೂರ್ಣಾಙ್ಕ cv_snprintf(ವರ್ಣ* buf, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ವರ್ಣ* fmt, ...);
ಪೂರ್ಣಾಙ್ಕ cv_vsnprintf(ವರ್ಣ* buf, ಪೂರ್ಣಾಙ್ಕ len, ಧ್ರುವ ವರ್ಣ* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
