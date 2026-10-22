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
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utility.hpp"
#ଅନ୍ୟଥା  // BUILD_PLUGIN

#ଅନ୍ତର୍ଭାଵ "opencv2/opencv_modules.hpp"
#ଅନ୍ତର୍ଭାଵ "cvconfig.h"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utility.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/cuda.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/opengl.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/va_intel.hpp"

#ଅନ୍ତର୍ଭାଵ "opencv2/core/private.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#ଅନ୍ତର୍ଭାଵ "opencv2/core/ocl.hpp"
#endif

#ଅନ୍ତର୍ଭାଵ <ctype.h>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <math.h>
#ଅନ୍ତର୍ଭାଵ <stdarg.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

#ଅନ୍ତର୍ଭାଵ <algorithm>
#ଅନ୍ତର୍ଭାଵ <cmath>
#ଅନ୍ତର୍ଭାଵ <cstdlib>
#ଅନ୍ତର୍ଭାଵ <limits>
#ଅନ୍ତର୍ଭାଵ <float.h>
#ଅନ୍ତର୍ଭାଵ <cstring>
#ଅନ୍ତର୍ଭାଵ <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/hal.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/hal/intrin.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/neon_utils.hpp"
#ଅନ୍ତର୍ଭାଵ "opencv2/core/vsx_utils.hpp"
#ଅନ୍ତର୍ଭାଵ "hal_replacement.hpp"


ନାମାକାଶ cv
{

// -128.f ... 255.f
ବାହ୍ୟ ଅଚଳ ଦଶମିକ g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

ବାହ୍ୟ ଅଚଳ ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

ବାହ୍ୟ ଅଚଳ uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (ପ୍ରତିଜ୍ଞା(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpNop
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpSqr
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpAdd
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a, ଅଚଳ T2 b) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpAddSqr
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a, ଅଚଳ T2 b) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpSub
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a, ଅଚଳ T2 b) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ସଂରଚନା OpRSub
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T1 type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T2 type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T3 rtype;
    T3 operator ()(ଅଚଳ T1 a, ଅଚଳ T2 b) ଅଚଳ { ଫେରନ୍ତୁ saturate_cast<T3>(b - a); }
};

template<typename T> ସଂରଚନା OpMin
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator ()(ଅଚଳ T a, ଅଚଳ T b) ଅଚଳ { ଫେରନ୍ତୁ std::min(a, b); }
};

template<typename T> ସଂରଚନା OpMax
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator ()(ଅଚଳ T a, ଅଚଳ T b) ଅଚଳ { ଫେରନ୍ତୁ std::max(a, b); }
};

template<typename T> ସଂରଚନା OpAbsDiff
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator()(T a, T b) ଅଚଳ { ଫେରନ୍ତୁ a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> ସଂରଚନା OpAbsDiff<ଦଶମିକ>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦଶମିକ rtype;
    ଦଶମିକ operator()(ଦଶମିକ a, ଦଶମିକ b) ଅଚଳ { ଫେରନ୍ତୁ std::abs(a - b); }
};
template<> ସଂରଚନା OpAbsDiff<ଦ୍ୱିଗୁଣ>
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା ଦ୍ୱିଗୁଣ rtype;
    ଦ୍ୱିଗୁଣ operator()(ଦ୍ୱିଗୁଣ a, ଦ୍ୱିଗୁଣ b) ଅଚଳ { ଫେରନ୍ତୁ std::abs(a - b); }
};

template<typename T> ସଂରଚନା OpAnd
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator()( T a, T b ) ଅଚଳ { ଫେରନ୍ତୁ a & b; }
};

template<typename T> ସଂରଚନା OpOr
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator()( T a, T b ) ଅଚଳ { ଫେରନ୍ତୁ a | b; }
};

template<typename T> ସଂରଚନା OpXor
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator()( T a, T b ) ଅଚଳ { ଫେରନ୍ତୁ a ^ b; }
};

template<typename T> ସଂରଚନା OpNot
{
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type1;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T type2;
    ପ୍ରକାରବ୍ୟାଖ୍ୟା T rtype;
    T operator()( T a, T ) ଅଚଳ { ଫେରନ୍ତୁ ~a; }
};

template<> ଧାଡ଼ିଭିତରେ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ଅଚଳ
{ ଫେରନ୍ତୁ CV_FAST_CAST_8U(a + b); }

template<> ଧାଡ଼ିଭିତରେ uchar OpSub<uchar>::operator ()(uchar a, uchar b) ଅଚଳ
{ ଫେରନ୍ତୁ CV_FAST_CAST_8U(a - b); }

template<> ଧାଡ଼ିଭିତରେ short OpAbsDiff<short>::operator ()(short a, short b) ଅଚଳ
{ ଫେରନ୍ତୁ saturate_cast<short>(std::abs(a - b)); }

template<> ଧାଡ଼ିଭିତରେ schar OpAbsDiff<schar>::operator ()(schar a, schar b) ଅଚଳ
{ ଫେରନ୍ତୁ saturate_cast<schar>(std::abs(a - b)); }

template<> ଧାଡ଼ିଭିତରେ uchar OpMin<uchar>::operator ()(uchar a, uchar b) ଅଚଳ { ଫେରନ୍ତୁ CV_MIN_8U(a, b); }

template<> ଧାଡ଼ିଭିତରେ uchar OpMax<uchar>::operator ()(uchar a, uchar b) ଅଚଳ { ଫେରନ୍ତୁ CV_MAX_8U(a, b); }

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*UnaryFunc)(ଅଚଳ uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       ଶୂନ୍ୟ*);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ଦ୍ୱିମିକ_Func)(ଅଚଳ uchar* src1, size_t step1,
                       ଅଚଳ uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       ଶୂନ୍ୟ*);

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଶୂନ୍ୟ (*ଦ୍ୱିମିକ_Func_C)(ଅଚଳ ଶୂନ୍ୟ* src1, size_t step1,
                       ଅଚଳ ଶୂନ୍ୟ* src2, size_t step2,
                       ଶୂନ୍ୟ* dst, size_t step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height,
                       ଶୂନ୍ୟ*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ଦ୍ୱିମିକ_Func getConvertFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);
CV_EXPORTS ଦ୍ୱିମିକ_Func getConvertScaleFunc(ପୂର୍ଣ୍ଣସଂଖ୍ୟା sdepth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ddepth);
ଦ୍ୱିମିକ_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, ପୂର୍ଣ୍ଣସଂଖ୍ୟା widthScale=1);

ଶୂନ୍ୟ setSize( Mat& m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା _dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* _sz, ଅଚଳ size_t* _steps, ତାର୍କିକ autoSteps=ମିଥ୍ୟା );
ଶୂନ୍ୟ finalizeHdr(Mat& m);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା updateContinuityFlag(ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dims, ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା* size, ଅଚଳ size_t* step);

ସଂରଚନା NoVec
{
    size_t operator()(ଅଚଳ ଶୂନ୍ୟ*, ଅଚଳ ଶୂନ୍ୟ*, ଶୂନ୍ୟ*, size_t) ଅଚଳ { ଫେରନ୍ତୁ 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

ଗଣନା { BLOCK_SIZE = 1024 };

#ଯଦି defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#ଅନ୍ୟଥା
#define ARITHM_USE_IPP 0
#endif

ଧାଡ଼ିଭିତରେ ତାର୍କିକ checkScalar(ଅଚଳ Mat& sc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype, ନିବେଶ_Array_2::KindFlag sckind, ନିବେଶ_Array_2::KindFlag akind)
{
    ଯଦି( sc.dims > 2 || !sc.isContinuous() )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    Size sz = sc.size();
    ଯଦି(sz.width != 1 && sz.height != 1)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(atype);
    ଯଦି( akind == ନିବେଶ_Array_2::MATX && sckind != ନିବେଶ_Array_2::MATX )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

ଧାଡ଼ିଭିତରେ ତାର୍କିକ checkScalar(ନିବେଶ_Array sc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା atype, ନିବେଶ_Array_2::KindFlag sckind, ନିବେଶ_Array_2::KindFlag akind)
{
    ଯଦି( sc.dims() > 2 || !sc.isContinuous() )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    Size sz = sc.size();
    ଯଦି(sz.width != 1 && sz.height != 1)
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = CV_MAT_CN(atype);
    ଯଦି( akind == ନିବେଶ_Array_2::MATX && sckind != ନିବେଶ_Array_2::MATX )
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
ଧାଡ଼ିଭିତରେ ତାର୍କିକ sameShapeAsArg(ଅଚଳ ନିବେଶ_Array_2& sc, ଅଚଳ ନିବେଶ_Array_2& arr)
{
    ଫେରନ୍ତୁ arr.channels() == 1 && sc.sameSize(arr);
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
ଧାଡ଼ିଭିତରେ ତାର୍କିକ isScalarArg(ଅଚଳ ନିବେଶ_Array_2& sc, ଅଚଳ ନିବେଶ_Array_2& arr)
{
    ଅଚଳ ନିବେଶ_Array_2::KindFlag kind = sc.kind();
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn = arr.channels();
    ଯଦି (kind == ନିବେଶ_Array_2::MATX)
    {
        Size sz = sc.getSz();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        ଯଦି (scn0 != sz.width + sz.height - 1)
            ଫେରନ୍ତୁ ମିଥ୍ୟା;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ଫେରନ୍ତୁ scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    ଯଦି (kind == ନିବେଶ_Array_2::MAT)
    {
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)sc.getObj();
        ଫେରନ୍ତୁ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ଯଦି (kind == ନିବେଶ_Array_2::UMAT)
    {
        ଅଚଳ UMat& m = *(ଅଚଳ UMat*)sc.getObj();
        ଫେରନ୍ତୁ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ଫେରନ୍ତୁ ମିଥ୍ୟା;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
ଧାଡ଼ିଭିତରେ ତାର୍କିକ isScalarLikeMat(ଅଚଳ ନିବେଶ_Array_2& sc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, cols, nval;
    ତାର୍କିକ continuous;
    ଅଚଳ ନିବେଶ_Array_2::KindFlag kind = sc.kind();
    ଯଦି (kind == ନିବେଶ_Array_2::MAT)
    {
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)sc.getObj();
        ଯଦି ((m.rows != 1 && m.cols != 1) || m.dims > 2) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ଅନ୍ୟଥା ଯଦି (kind == ନିବେଶ_Array_2::UMAT)
    {
        ଅଚଳ UMat& m = *(ଅଚଳ UMat*)sc.getObj();
        ଯଦି ((m.rows != 1 && m.cols != 1) || m.dims > 2) ଫେରନ୍ତୁ ମିଥ୍ୟା;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ଅନ୍ୟଥା
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଯଦି (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ଫେରନ୍ତୁ ମିଥ୍ୟା;
    ଫେରନ୍ତୁ (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
ଗଣନା { EW_SCALAR_BUF_SIZE = 4 * ଆକାର(ଦ୍ୱିଗୁଣ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା scalarArgElems(ଅଚଳ ନିବେଶ_Array_2& sc, ଅଚଳ uchar*& p, ପୂର୍ଣ୍ଣସଂଖ୍ୟା& d, uchar* scbuf)
{
    ଅଚଳ ନିବେଶ_Array_2::KindFlag kind = sc.kind();
    ଯଦି (kind == ନିବେଶ_Array_2::MAT)
    {
        ଅଚଳ Mat& m = *(ଅଚଳ Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        ଫେରନ୍ତୁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)m.total() * m.channels();
    }
    ଯଦି (kind == ନିବେଶ_Array_2::UMAT)
    {
        ଅଚଳ UMat& u = *(ଅଚଳ UMat*)sc.getObj();
        d = u.depth();
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u.total() * u.channels();
        CV_Assert(n * (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)CV_ELEM_SIZE1(d) <= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ଫେରନ୍ତୁ n;
    }
    p = (ଅଚଳ uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    ଫେରନ୍ତୁ sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
ଧାଡ଼ିଭିତରେ ଦ୍ୱିଗୁଣ elemToDouble(ପୂର୍ଣ୍ଣସଂଖ୍ୟା d, ଅଚଳ uchar* p)
{
    ବାଛ (d)
    {
    ସ୍ଥିତି CV_8U:   ଫେରନ୍ତୁ *(ଅଚଳ uchar*)p;
    ସ୍ଥିତି CV_8S:   ଫେରନ୍ତୁ *(ଅଚଳ schar*)p;
    ସ୍ଥିତି CV_16U:  ଫେରନ୍ତୁ *(ଅଚଳ ushort*)p;
    ସ୍ଥିତି CV_16S:  ଫେରନ୍ତୁ *(ଅଚଳ short*)p;
    ସ୍ଥିତି CV_32U:  ଫେରନ୍ତୁ *(ଅଚଳ unsigned*)p;
    ସ୍ଥିତି CV_32S:  ଫେରନ୍ତୁ *(ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା*)p;
    ସ୍ଥିତି CV_64U:  ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)*(ଅଚଳ uint64_t*)p;
    ସ୍ଥିତି CV_64S:  ଫେରନ୍ତୁ (ଦ୍ୱିଗୁଣ)*(ଅଚଳ int64_t*)p;
    ସ୍ଥିତି CV_16F:  ଫେରନ୍ତୁ (ଦଶମିକ)*(ଅଚଳ hfloat*)p;
    ସ୍ଥିତି CV_16BF: ଫେରନ୍ତୁ (ଦଶମିକ)*(ଅଚଳ bfloat*)p;
    ସ୍ଥିତି CV_32F:  ଫେରନ୍ତୁ *(ଅଚଳ ଦଶମିକ*)p;
    ସ୍ଥିତି CV_64F:  ଫେରନ୍ତୁ *(ଅଚଳ ଦ୍ୱିଗୁଣ*)p;
    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା readScalarArg(ଅଚଳ ନିବେଶ_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ଅଚଳ uchar* p; ପୂର୍ଣ୍ଣସଂଖ୍ୟା d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    ଫେରନ୍ତୁ n;
}

ଶୂନ୍ୟ convertAndUnrollScalar( ଅଚଳ Mat& sc, ପୂର୍ଣ୍ଣସଂଖ୍ୟା buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
ସଂରଚନା ImplCollector
{
    ImplCollector()
    {
        useCollection   = ମିଥ୍ୟା;
        implFlags       = 0;
    }
    ତାର୍କିକ useCollection; // enable/disable impl data collection

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା implFlags;
    std::vector<ପୂର୍ଣ୍ଣସଂଖ୍ୟା>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

ସଂରଚନା CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(ମିଥ୍ୟା), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    ତାର୍କିକ oclExecutionContextInitialized;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#ଯଦି defined(BUILD_SHARED_LIBS)
#ଯଦି defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#ନଚେତଯଦି defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#ଅନ୍ୟଥା
#define CL_RUNTIME_EXPORT
#endif
#ଅନ୍ୟଥା
#define CL_RUNTIME_EXPORT
#endif

ବାହ୍ୟ CV_EXPORTS
ତାର୍କିକ __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_ମୂଲ୍ୟ) \
    ସ୍ଥିର TYPE* ଅଚଳ instance = INITIALIZER; \
    ଫେରନ୍ତୁ RET_ମୂଲ୍ୟ;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS ଶୂନ୍ୟ releaseTlsStorageThread();

ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv_snprintf(ଅକ୍ଷର* buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଅକ୍ଷର* fmt, ...);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା cv_vsnprintf(ଅକ୍ଷର* buf, ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ଅଚଳ ଅକ୍ଷର* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
