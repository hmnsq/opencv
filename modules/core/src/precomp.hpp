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
#অন্তর্ভাব "opencv2/core/utility.hpp"
#অন্যথা  // BUILD_PLUGIN

#অন্তর্ভাব "opencv2/opencv_modules.hpp"
#অন্তর্ভাব "cvconfig.h"

#অন্তর্ভাব "opencv2/core/utility.hpp"
#অন্তর্ভাব "opencv2/core/cuda.hpp"
#অন্তর্ভাব "opencv2/core/opengl.hpp"
#অন্তর্ভাব "opencv2/core/va_intel.hpp"

#অন্তর্ভাব "opencv2/core/private.hpp"
#অন্তর্ভাব "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#অন্তর্ভাব "opencv2/core/ocl.hpp"
#endif

#অন্তর্ভাব <ctype.h>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <math.h>
#অন্তর্ভাব <stdarg.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

#অন্তর্ভাব <algorithm>
#অন্তর্ভাব <cmath>
#অন্তর্ভাব <cstdlib>
#অন্তর্ভাব <limits>
#অন্তর্ভাব <float.h>
#অন্তর্ভাব <cstring>
#অন্তর্ভাব <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#অন্তর্ভাব "opencv2/core/hal/hal.hpp"
#অন্তর্ভাব "opencv2/core/hal/intrin.hpp"
#অন্তর্ভাব "opencv2/core/neon_utils.hpp"
#অন্তর্ভাব "opencv2/core/vsx_utils.hpp"
#অন্তর্ভাব "hal_replacement.hpp"


নামাকাশ cv
{

// -128.f ... 255.f
বাহ্যিক ধ্রুবক দশমিক g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

বাহ্যিক ধ্রুবক ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

বাহ্যিক ধ্রুবক uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (প্রতিজ্ঞা(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpNop
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a) ধ্রুবক { ফেরত saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpSqr
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a) ধ্রুবক { ফেরত saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpAdd
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a, ধ্রুবক T2 b) ধ্রুবক { ফেরত saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpAddSqr
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a, ধ্রুবক T2 b) ধ্রুবক { ফেরত saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpSub
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a, ধ্রুবক T2 b) ধ্রুবক { ফেরত saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> কাঠামো OpRSub
{
    প্রকারলেখ T1 type1;
    প্রকারলেখ T2 type2;
    প্রকারলেখ T3 rtype;
    T3 operator ()(ধ্রুবক T1 a, ধ্রুবক T2 b) ধ্রুবক { ফেরত saturate_cast<T3>(b - a); }
};

template<typename T> কাঠামো OpMin
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator ()(ধ্রুবক T a, ধ্রুবক T b) ধ্রুবক { ফেরত std::min(a, b); }
};

template<typename T> কাঠামো OpMax
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator ()(ধ্রুবক T a, ধ্রুবক T b) ধ্রুবক { ফেরত std::max(a, b); }
};

template<typename T> কাঠামো OpAbsDiff
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator()(T a, T b) ধ্রুবক { ফেরত a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> কাঠামো OpAbsDiff<দশমিক>
{
    প্রকারলেখ দশমিক type1;
    প্রকারলেখ দশমিক type2;
    প্রকারলেখ দশমিক rtype;
    দশমিক operator()(দশমিক a, দশমিক b) ধ্রুবক { ফেরত std::abs(a - b); }
};
template<> কাঠামো OpAbsDiff<দ্বিগুণ>
{
    প্রকারলেখ দ্বিগুণ type1;
    প্রকারলেখ দ্বিগুণ type2;
    প্রকারলেখ দ্বিগুণ rtype;
    দ্বিগুণ operator()(দ্বিগুণ a, দ্বিগুণ b) ধ্রুবক { ফেরত std::abs(a - b); }
};

template<typename T> কাঠামো OpAnd
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator()( T a, T b ) ধ্রুবক { ফেরত a & b; }
};

template<typename T> কাঠামো OpOr
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator()( T a, T b ) ধ্রুবক { ফেরত a | b; }
};

template<typename T> কাঠামো OpXor
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator()( T a, T b ) ধ্রুবক { ফেরত a ^ b; }
};

template<typename T> কাঠামো OpNot
{
    প্রকারলেখ T type1;
    প্রকারলেখ T type2;
    প্রকারলেখ T rtype;
    T operator()( T a, T ) ধ্রুবক { ফেরত ~a; }
};

template<> অন্তঃরেখ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ধ্রুবক
{ ফেরত CV_FAST_CAST_8U(a + b); }

template<> অন্তঃরেখ uchar OpSub<uchar>::operator ()(uchar a, uchar b) ধ্রুবক
{ ফেরত CV_FAST_CAST_8U(a - b); }

template<> অন্তঃরেখ short OpAbsDiff<short>::operator ()(short a, short b) ধ্রুবক
{ ফেরত saturate_cast<short>(std::abs(a - b)); }

template<> অন্তঃরেখ schar OpAbsDiff<schar>::operator ()(schar a, schar b) ধ্রুবক
{ ফেরত saturate_cast<schar>(std::abs(a - b)); }

template<> অন্তঃরেখ uchar OpMin<uchar>::operator ()(uchar a, uchar b) ধ্রুবক { ফেরত CV_MIN_8U(a, b); }

template<> অন্তঃরেখ uchar OpMax<uchar>::operator ()(uchar a, uchar b) ধ্রুবক { ফেরত CV_MAX_8U(a, b); }

প্রকারলেখ শূন্য (*UnaryFunc)(ধ্রুবক uchar* src1, size_t step1,
                       uchar* dst, size_t step, আকার_2 sz,
                       শূন্য*);

প্রকারলেখ শূন্য (*দ্বিমিক_Func)(ধ্রুবক uchar* src1, size_t step1,
                       ধ্রুবক uchar* src2, size_t step2,
                       uchar* dst, size_t step, আকার_2 sz,
                       শূন্য*);

প্রকারলেখ শূন্য (*দ্বিমিক_Func_C)(ধ্রুবক শূন্য* src1, size_t step1,
                       ধ্রুবক শূন্য* src2, size_t step2,
                       শূন্য* dst, size_t step, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height,
                       শূন্য*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS দ্বিমিক_Func getConvertFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
CV_EXPORTS দ্বিমিক_Func getConvertScaleFunc(পূর্ণসংখ্যা sdepth, পূর্ণসংখ্যা ddepth);
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
আকার_2 getContinuousSize2D(Mat& m1, পূর্ণসংখ্যা widthScale=1);
আকার_2 getContinuousSize2D(Mat& m1, Mat& m2, পূর্ণসংখ্যা widthScale=1);
আকার_2 getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, পূর্ণসংখ্যা widthScale=1);

শূন্য set_আকার( Mat& m, পূর্ণসংখ্যা _dims, ধ্রুবক পূর্ণসংখ্যা* _sz, ধ্রুবক size_t* _steps, যৌক্তিক autoSteps=মিথ্যা );
শূন্য finalizeHdr(Mat& m);
পূর্ণসংখ্যা updateContinuityFlag(পূর্ণসংখ্যা flags, পূর্ণসংখ্যা dims, ধ্রুবক পূর্ণসংখ্যা* size, ধ্রুবক size_t* step);

কাঠামো NoVec
{
    size_t operator()(ধ্রুবক শূন্য*, ধ্রুবক শূন্য*, শূন্য*, size_t) ধ্রুবক { ফেরত 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_আকার(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

গণনা { BLOCK_আকার = 1024 };

#যদি defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#অন্যথা
#define ARITHM_USE_IPP 0
#endif

অন্তঃরেখ যৌক্তিক checkScalar(ধ্রুবক Mat& sc, পূর্ণসংখ্যা atype, প্রবেশ_Array_2::KindFlag sckind, প্রবেশ_Array_2::KindFlag akind)
{
    যদি( sc.dims > 2 || !sc.isContinuous() )
        ফেরত মিথ্যা;
    আকার_2 sz = sc.size();
    যদি(sz.width != 1 && sz.height != 1)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা cn = CV_MAT_CN(atype);
    যদি( akind == প্রবেশ_Array_2::MATX && sckind != প্রবেশ_Array_2::MATX )
        ফেরত মিথ্যা;
    ফেরত sz == আকার_2(1, 1) || sz == আকার_2(1, cn) || sz == আকার_2(cn, 1) ||
           (sz == আকার_2(1, 4) && sc.type() == CV_64F && cn <= 4);
}

অন্তঃরেখ যৌক্তিক checkScalar(প্রবেশ_Array sc, পূর্ণসংখ্যা atype, প্রবেশ_Array_2::KindFlag sckind, প্রবেশ_Array_2::KindFlag akind)
{
    যদি( sc.dims() > 2 || !sc.isContinuous() )
        ফেরত মিথ্যা;
    আকার_2 sz = sc.size();
    যদি(sz.width != 1 && sz.height != 1)
        ফেরত মিথ্যা;
    পূর্ণসংখ্যা cn = CV_MAT_CN(atype);
    যদি( akind == প্রবেশ_Array_2::MATX && sckind != প্রবেশ_Array_2::MATX )
        ফেরত মিথ্যা;
    ফেরত sz == আকার_2(1, 1) || sz == আকার_2(1, cn) || sz == আকার_2(cn, 1) ||
           (sz == আকার_2(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
অন্তঃরেখ যৌক্তিক sameShapeAsArg(ধ্রুবক প্রবেশ_Array_2& sc, ধ্রুবক প্রবেশ_Array_2& arr)
{
    ফেরত arr.channels() == 1 && sc.same_আকার(arr);
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
অন্তঃরেখ যৌক্তিক isScalarArg(ধ্রুবক প্রবেশ_Array_2& sc, ধ্রুবক প্রবেশ_Array_2& arr)
{
    ধ্রুবক প্রবেশ_Array_2::KindFlag kind = sc.kind();
    ধ্রুবক পূর্ণসংখ্যা cn = arr.channels();
    যদি (kind == প্রবেশ_Array_2::MATX)
    {
        আকার_2 sz = sc.getSz();
        পূর্ণসংখ্যা scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        যদি (scn0 != sz.width + sz.height - 1)
            ফেরত মিথ্যা;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ফেরত scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    যদি (kind == প্রবেশ_Array_2::MAT)
    {
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)sc.getObj();
        ফেরত m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    যদি (kind == প্রবেশ_Array_2::UMAT)
    {
        ধ্রুবক UMat& m = *(ধ্রুবক UMat*)sc.getObj();
        ফেরত m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ফেরত মিথ্যা;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
অন্তঃরেখ যৌক্তিক isScalarLikeMat(ধ্রুবক প্রবেশ_Array_2& sc, পূর্ণসংখ্যা cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    পূর্ণসংখ্যা rows, cols, nval;
    যৌক্তিক continuous;
    ধ্রুবক প্রবেশ_Array_2::KindFlag kind = sc.kind();
    যদি (kind == প্রবেশ_Array_2::MAT)
    {
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)sc.getObj();
        যদি ((m.rows != 1 && m.cols != 1) || m.dims > 2) ফেরত মিথ্যা;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    অন্যথা যদি (kind == প্রবেশ_Array_2::UMAT)
    {
        ধ্রুবক UMat& m = *(ধ্রুবক UMat*)sc.getObj();
        যদি ((m.rows != 1 && m.cols != 1) || m.dims > 2) ফেরত মিথ্যা;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    অন্যথা
        ফেরত মিথ্যা;
    যদি (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ফেরত মিথ্যা;
    ফেরত (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
গণনা { EW_SCALAR_BUF_আকার = 4 * আকার(দ্বিগুণ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
অন্তঃরেখ পূর্ণসংখ্যা scalarArgElems(ধ্রুবক প্রবেশ_Array_2& sc, ধ্রুবক uchar*& p, পূর্ণসংখ্যা& d, uchar* scbuf)
{
    ধ্রুবক প্রবেশ_Array_2::KindFlag kind = sc.kind();
    যদি (kind == প্রবেশ_Array_2::MAT)
    {
        ধ্রুবক Mat& m = *(ধ্রুবক Mat*)sc.getObj();
        p = m.তথ্য;
        d = m.depth();
        ফেরত (পূর্ণসংখ্যা)m.যোগ() * m.channels();
    }
    যদি (kind == প্রবেশ_Array_2::UMAT)
    {
        ধ্রুবক UMat& u = *(ধ্রুবক UMat*)sc.getObj();
        d = u.depth();
        পূর্ণসংখ্যা n = (পূর্ণসংখ্যা)u.যোগ() * u.channels();
        CV_Assert(n * (পূর্ণসংখ্যা)CV_ELEM_SIZE1(d) <= (পূর্ণসংখ্যা)EW_SCALAR_BUF_আকার);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ফেরত n;
    }
    p = (ধ্রুবক uchar*)sc.getObj();
    d = sc.depth();
    আকার_2 sz = sc.getSz();
    ফেরত sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
অন্তঃরেখ দ্বিগুণ elemToDouble(পূর্ণসংখ্যা d, ধ্রুবক uchar* p)
{
    নির্বাচন (d)
    {
    ক্ষেত্র CV_8U:   ফেরত *(ধ্রুবক uchar*)p;
    ক্ষেত্র CV_8S:   ফেরত *(ধ্রুবক schar*)p;
    ক্ষেত্র CV_16U:  ফেরত *(ধ্রুবক ushort*)p;
    ক্ষেত্র CV_16S:  ফেরত *(ধ্রুবক short*)p;
    ক্ষেত্র CV_32U:  ফেরত *(ধ্রুবক unsigned*)p;
    ক্ষেত্র CV_32S:  ফেরত *(ধ্রুবক পূর্ণসংখ্যা*)p;
    ক্ষেত্র CV_64U:  ফেরত (দ্বিগুণ)*(ধ্রুবক uint64_t*)p;
    ক্ষেত্র CV_64S:  ফেরত (দ্বিগুণ)*(ধ্রুবক int64_t*)p;
    ক্ষেত্র CV_16F:  ফেরত (দশমিক)*(ধ্রুবক hfloat*)p;
    ক্ষেত্র CV_16BF: ফেরত (দশমিক)*(ধ্রুবক bfloat*)p;
    ক্ষেত্র CV_32F:  ফেরত *(ধ্রুবক দশমিক*)p;
    ক্ষেত্র CV_64F:  ফেরত *(ধ্রুবক দ্বিগুণ*)p;
    পূর্বনির্ধারিত:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
অন্তঃরেখ পূর্ণসংখ্যা readScalarArg(ধ্রুবক প্রবেশ_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ধ্রুবক uchar* p; পূর্ণসংখ্যা d; uchar scbuf[EW_SCALAR_BUF_আকার];
    পূর্ণসংখ্যা n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    জন্য (পূর্ণসংখ্যা i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    ফেরত n;
}

শূন্য convertAndUnrollScalar( ধ্রুবক Mat& sc, পূর্ণসংখ্যা buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_তথ্য
কাঠামো ImplCollector
{
    ImplCollector()
    {
        useCollection   = মিথ্যা;
        implFlags       = 0;
    }
    যৌক্তিক useCollection; // enable/disable impl data collection

    পূর্ণসংখ্যা implFlags;
    std::vector<পূর্ণসংখ্যা>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

কাঠামো CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(মিথ্যা), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    যৌক্তিক oclExecutionContextInitialized;
    পূর্ণসংখ্যা useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    পূর্ণসংখ্যা useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    পূর্ণসংখ্যা useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_তথ্য();

#যদি defined(BUILD_SHARED_LIBS)
#যদি defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#নতুবা_যদি defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#অন্যথা
#define CL_RUNTIME_EXPORT
#endif
#অন্যথা
#define CL_RUNTIME_EXPORT
#endif

বাহ্যিক CV_EXPORTS
যৌক্তিক __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_মান) \
    স্থির TYPE* ধ্রুবক instance = INITIALIZER; \
    ফেরত RET_মান;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS শূন্য releaseTlsStorageThread();

পূর্ণসংখ্যা cv_snprintf(অক্ষর* buf, পূর্ণসংখ্যা len, ধ্রুবক অক্ষর* fmt, ...);
পূর্ণসংখ্যা cv_vsnprintf(অক্ষর* buf, পূর্ণসংখ্যা len, ধ্রুবক অক্ষর* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
