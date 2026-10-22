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
#অন্তর্ভাৱ "opencv2/core/utility.hpp"
#অন্যথা  // BUILD_PLUGIN

#অন্তর্ভাৱ "opencv2/opencv_modules.hpp"
#অন্তর্ভাৱ "cvconfig.h"

#অন্তর্ভাৱ "opencv2/core/utility.hpp"
#অন্তর্ভাৱ "opencv2/core/cuda.hpp"
#অন্তর্ভাৱ "opencv2/core/opengl.hpp"
#অন্তর্ভাৱ "opencv2/core/va_intel.hpp"

#অন্তর্ভাৱ "opencv2/core/private.hpp"
#অন্তর্ভাৱ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#অন্তর্ভাৱ "opencv2/core/ocl.hpp"
#endif

#অন্তর্ভাৱ <ctype.h>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <limits.h>
#অন্তর্ভাৱ <math.h>
#অন্তর্ভাৱ <stdarg.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

#অন্তর্ভাৱ <algorithm>
#অন্তর্ভাৱ <cmath>
#অন্তর্ভাৱ <cstdlib>
#অন্তর্ভাৱ <limits>
#অন্তর্ভাৱ <float.h>
#অন্তর্ভাৱ <cstring>
#অন্তর্ভাৱ <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#অন্তর্ভাৱ "opencv2/core/hal/hal.hpp"
#অন্তর্ভাৱ "opencv2/core/hal/intrin.hpp"
#অন্তর্ভাৱ "opencv2/core/neon_utils.hpp"
#অন্তর্ভাৱ "opencv2/core/vsx_utils.hpp"
#অন্তর্ভাৱ "hal_replacement.hpp"


নামাকাশ cv
{

// -128.f ... 255.f
বাহ্য ধ্রুৱ ভিন্ন g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

বাহ্য ধ্রুৱ ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

বাহ্য ধ্রুৱ uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (প্রতিজ্ঞা(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpNop
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpSqr
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpAdd
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a, ধ্রুৱ T2 b) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpAddSqr
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a, ধ্রুৱ T2 b) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpSub
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a, ধ্রুৱ T2 b) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> সংরচনা OpRSub
{
    প্রকারনির্দেশ T1 type1;
    প্রকারনির্দেশ T2 type2;
    প্রকারনির্দেশ T3 rtype;
    T3 operator ()(ধ্রুৱ T1 a, ধ্রুৱ T2 b) ধ্রুৱ { ঘূৰাই saturate_cast<T3>(b - a); }
};

template<typename T> সংরচনা OpMin
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator ()(ধ্রুৱ T a, ধ্রুৱ T b) ধ্রুৱ { ঘূৰাই std::min(a, b); }
};

template<typename T> সংরচনা OpMax
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator ()(ধ্রুৱ T a, ধ্রুৱ T b) ধ্রুৱ { ঘূৰাই std::max(a, b); }
};

template<typename T> সংরচনা OpAbsDiff
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator()(T a, T b) ধ্রুৱ { ঘূৰাই a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> সংরচনা OpAbsDiff<ভিন্ন>
{
    প্রকারনির্দেশ ভিন্ন type1;
    প্রকারনির্দেশ ভিন্ন type2;
    প্রকারনির্দেশ ভিন্ন rtype;
    ভিন্ন operator()(ভিন্ন a, ভিন্ন b) ধ্রুৱ { ঘূৰাই std::abs(a - b); }
};
template<> সংরচনা OpAbsDiff<দ্ৱিগুণ>
{
    প্রকারনির্দেশ দ্ৱিগুণ type1;
    প্রকারনির্দেশ দ্ৱিগুণ type2;
    প্রকারনির্দেশ দ্ৱিগুণ rtype;
    দ্ৱিগুণ operator()(দ্ৱিগুণ a, দ্ৱিগুণ b) ধ্রুৱ { ঘূৰাই std::abs(a - b); }
};

template<typename T> সংরচনা OpAnd
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator()( T a, T b ) ধ্রুৱ { ঘূৰাই a & b; }
};

template<typename T> সংরচনা OpOr
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator()( T a, T b ) ধ্রুৱ { ঘূৰাই a | b; }
};

template<typename T> সংরচনা OpXor
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator()( T a, T b ) ধ্রুৱ { ঘূৰাই a ^ b; }
};

template<typename T> সংরচনা OpNot
{
    প্রকারনির্দেশ T type1;
    প্রকারনির্দেশ T type2;
    প্রকারনির্দেশ T rtype;
    T operator()( T a, T ) ধ্রুৱ { ঘূৰাই ~a; }
};

template<> অন্তঃস্থ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ধ্রুৱ
{ ঘূৰাই CV_FAST_CAST_8U(a + b); }

template<> অন্তঃস্থ uchar OpSub<uchar>::operator ()(uchar a, uchar b) ধ্রুৱ
{ ঘূৰাই CV_FAST_CAST_8U(a - b); }

template<> অন্তঃস্থ short OpAbsDiff<short>::operator ()(short a, short b) ধ্রুৱ
{ ঘূৰাই saturate_cast<short>(std::abs(a - b)); }

template<> অন্তঃস্থ schar OpAbsDiff<schar>::operator ()(schar a, schar b) ধ্রুৱ
{ ঘূৰাই saturate_cast<schar>(std::abs(a - b)); }

template<> অন্তঃস্থ uchar OpMin<uchar>::operator ()(uchar a, uchar b) ধ্রুৱ { ঘূৰাই CV_MIN_8U(a, b); }

template<> অন্তঃস্থ uchar OpMax<uchar>::operator ()(uchar a, uchar b) ধ্রুৱ { ঘূৰাই CV_MAX_8U(a, b); }

প্রকারনির্দেশ শূন্য (*UnaryFunc)(ধ্রুৱ uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       শূন্য*);

প্রকারনির্দেশ শূন্য (*দ্বিমিক_Func)(ধ্রুৱ uchar* src1, size_t step1,
                       ধ্রুৱ uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       শূন্য*);

প্রকারনির্দেশ শূন্য (*দ্বিমিক_Func_C)(ধ্রুৱ শূন্য* src1, size_t step1,
                       ধ্রুৱ শূন্য* src2, size_t step2,
                       শূন্য* dst, size_t step, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height,
                       শূন্য*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS দ্বিমিক_Func getConvertFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
CV_EXPORTS দ্বিমিক_Func getConvertScaleFunc(পূর্ণাঙ্ক sdepth, পূর্ণাঙ্ক ddepth);
দ্বিমিক_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, পূর্ণাঙ্ক widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, পূর্ণাঙ্ক widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, পূর্ণাঙ্ক widthScale=1);

শূন্য setSize( Mat& m, পূর্ণাঙ্ক _dims, ধ্রুৱ পূর্ণাঙ্ক* _sz, ধ্রুৱ size_t* _steps, তার্কিক autoSteps=মিছা );
শূন্য finalizeHdr(Mat& m);
পূর্ণাঙ্ক updateContinuityFlag(পূর্ণাঙ্ক flags, পূর্ণাঙ্ক dims, ধ্রুৱ পূর্ণাঙ্ক* size, ধ্রুৱ size_t* step);

সংরচনা NoVec
{
    size_t operator()(ধ্রুৱ শূন্য*, ধ্রুৱ শূন্য*, শূন্য*, size_t) ধ্রুৱ { ঘূৰাই 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

গণনা { BLOCK_SIZE = 1024 };

#যদি defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#অন্যথা
#define ARITHM_USE_IPP 0
#endif

অন্তঃস্থ তার্কিক checkScalar(ধ্রুৱ Mat& sc, পূর্ণাঙ্ক atype, প্ৰবেশ_Array_2::KindFlag sckind, প্ৰবেশ_Array_2::KindFlag akind)
{
    যদি( sc.dims > 2 || !sc.isContinuous() )
        ঘূৰাই মিছা;
    Size sz = sc.size();
    যদি(sz.width != 1 && sz.height != 1)
        ঘূৰাই মিছা;
    পূর্ণাঙ্ক cn = CV_MAT_CN(atype);
    যদি( akind == প্ৰবেশ_Array_2::MATX && sckind != প্ৰবেশ_Array_2::MATX )
        ঘূৰাই মিছা;
    ঘূৰাই sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

অন্তঃস্থ তার্কিক checkScalar(প্ৰবেশ_Array sc, পূর্ণাঙ্ক atype, প্ৰবেশ_Array_2::KindFlag sckind, প্ৰবেশ_Array_2::KindFlag akind)
{
    যদি( sc.dims() > 2 || !sc.isContinuous() )
        ঘূৰাই মিছা;
    Size sz = sc.size();
    যদি(sz.width != 1 && sz.height != 1)
        ঘূৰাই মিছা;
    পূর্ণাঙ্ক cn = CV_MAT_CN(atype);
    যদি( akind == প্ৰবেশ_Array_2::MATX && sckind != প্ৰবেশ_Array_2::MATX )
        ঘূৰাই মিছা;
    ঘূৰাই sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
অন্তঃস্থ তার্কিক sameShapeAsArg(ধ্রুৱ প্ৰবেশ_Array_2& sc, ধ্রুৱ প্ৰবেশ_Array_2& arr)
{
    ঘূৰাই arr.channels() == 1 && sc.sameSize(arr);
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
অন্তঃস্থ তার্কিক isScalarArg(ধ্রুৱ প্ৰবেশ_Array_2& sc, ধ্রুৱ প্ৰবেশ_Array_2& arr)
{
    ধ্রুৱ প্ৰবেশ_Array_2::KindFlag kind = sc.kind();
    ধ্রুৱ পূর্ণাঙ্ক cn = arr.channels();
    যদি (kind == প্ৰবেশ_Array_2::MATX)
    {
        Size sz = sc.getSz();
        পূর্ণাঙ্ক scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        যদি (scn0 != sz.width + sz.height - 1)
            ঘূৰাই মিছা;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ঘূৰাই scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    যদি (kind == প্ৰবেশ_Array_2::MAT)
    {
        ধ্রুৱ Mat& m = *(ধ্রুৱ Mat*)sc.getObj();
        ঘূৰাই m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    যদি (kind == প্ৰবেশ_Array_2::UMAT)
    {
        ধ্রুৱ UMat& m = *(ধ্রুৱ UMat*)sc.getObj();
        ঘূৰাই m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ঘূৰাই মিছা;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
অন্তঃস্থ তার্কিক isScalarLikeMat(ধ্রুৱ প্ৰবেশ_Array_2& sc, পূর্ণাঙ্ক cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    পূর্ণাঙ্ক rows, cols, nval;
    তার্কিক continuous;
    ধ্রুৱ প্ৰবেশ_Array_2::KindFlag kind = sc.kind();
    যদি (kind == প্ৰবেশ_Array_2::MAT)
    {
        ধ্রুৱ Mat& m = *(ধ্রুৱ Mat*)sc.getObj();
        যদি ((m.rows != 1 && m.cols != 1) || m.dims > 2) ঘূৰাই মিছা;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    অন্যথা যদি (kind == প্ৰবেশ_Array_2::UMAT)
    {
        ধ্রুৱ UMat& m = *(ধ্রুৱ UMat*)sc.getObj();
        যদি ((m.rows != 1 && m.cols != 1) || m.dims > 2) ঘূৰাই মিছা;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    অন্যথা
        ঘূৰাই মিছা;
    যদি (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ঘূৰাই মিছা;
    ঘূৰাই (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
গণনা { EW_SCALAR_BUF_SIZE = 4 * পরিমাণ(দ্ৱিগুণ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
অন্তঃস্থ পূর্ণাঙ্ক scalarArgElems(ধ্রুৱ প্ৰবেশ_Array_2& sc, ধ্রুৱ uchar*& p, পূর্ণাঙ্ক& d, uchar* scbuf)
{
    ধ্রুৱ প্ৰবেশ_Array_2::KindFlag kind = sc.kind();
    যদি (kind == প্ৰবেশ_Array_2::MAT)
    {
        ধ্রুৱ Mat& m = *(ধ্রুৱ Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        ঘূৰাই (পূর্ণাঙ্ক)m.total() * m.channels();
    }
    যদি (kind == প্ৰবেশ_Array_2::UMAT)
    {
        ধ্রুৱ UMat& u = *(ধ্রুৱ UMat*)sc.getObj();
        d = u.depth();
        পূর্ণাঙ্ক n = (পূর্ণাঙ্ক)u.total() * u.channels();
        CV_Assert(n * (পূর্ণাঙ্ক)CV_ELEM_SIZE1(d) <= (পূর্ণাঙ্ক)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ঘূৰাই n;
    }
    p = (ধ্রুৱ uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    ঘূৰাই sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
অন্তঃস্থ দ্ৱিগুণ elemToDouble(পূর্ণাঙ্ক d, ধ্রুৱ uchar* p)
{
    ৱিকল্প (d)
    {
    পক্ষ CV_8U:   ঘূৰাই *(ধ্রুৱ uchar*)p;
    পক্ষ CV_8S:   ঘূৰাই *(ধ্রুৱ schar*)p;
    পক্ষ CV_16U:  ঘূৰাই *(ধ্রুৱ ushort*)p;
    পক্ষ CV_16S:  ঘূৰাই *(ধ্রুৱ short*)p;
    পক্ষ CV_32U:  ঘূৰাই *(ধ্রুৱ unsigned*)p;
    পক্ষ CV_32S:  ঘূৰাই *(ধ্রুৱ পূর্ণাঙ্ক*)p;
    পক্ষ CV_64U:  ঘূৰাই (দ্ৱিগুণ)*(ধ্রুৱ uint64_t*)p;
    পক্ষ CV_64S:  ঘূৰাই (দ্ৱিগুণ)*(ধ্রুৱ int64_t*)p;
    পক্ষ CV_16F:  ঘূৰাই (ভিন্ন)*(ধ্রুৱ hfloat*)p;
    পক্ষ CV_16BF: ঘূৰাই (ভিন্ন)*(ধ্রুৱ bfloat*)p;
    পক্ষ CV_32F:  ঘূৰাই *(ধ্রুৱ ভিন্ন*)p;
    পক্ষ CV_64F:  ঘূৰাই *(ধ্রুৱ দ্ৱিগুণ*)p;
    স্ৱভাৱ:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
অন্তঃস্থ পূর্ণাঙ্ক readScalarArg(ধ্রুৱ প্ৰবেশ_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ধ্রুৱ uchar* p; পূর্ণাঙ্ক d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    পূর্ণাঙ্ক n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    বাবে (পূর্ণাঙ্ক i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    ঘূৰাই n;
}

শূন্য convertAndUnrollScalar( ধ্রুৱ Mat& sc, পূর্ণাঙ্ক buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
সংরচনা ImplCollector
{
    ImplCollector()
    {
        useCollection   = মিছা;
        implFlags       = 0;
    }
    তার্কিক useCollection; // enable/disable impl data collection

    পূর্ণাঙ্ক implFlags;
    std::vector<পূর্ণাঙ্ক>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

সংরচনা CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(মিছা), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    তার্কিক oclExecutionContextInitialized;
    পূর্ণাঙ্ক useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    পূর্ণাঙ্ক useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    পূর্ণাঙ্ক useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#যদি defined(BUILD_SHARED_LIBS)
#যদি defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#নোচেদ্যদি defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#অন্যথা
#define CL_RUNTIME_EXPORT
#endif
#অন্যথা
#define CL_RUNTIME_EXPORT
#endif

বাহ্য CV_EXPORTS
তার্কিক __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_মান) \
    স্থির TYPE* ধ্রুৱ instance = INITIALIZER; \
    ঘূৰাই RET_মান;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS শূন্য releaseTlsStorageThread();

পূর্ণাঙ্ক cv_snprintf(ৱর্ণ* buf, পূর্ণাঙ্ক len, ধ্রুৱ ৱর্ণ* fmt, ...);
পূর্ণাঙ্ক cv_vsnprintf(ৱর্ণ* buf, পূর্ণাঙ্ক len, ধ্রুৱ ৱর্ণ* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
