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
#அந்தர்பாவ "opencv2/core/utility.hpp"
#இல்லையெனில்  // BUILD_PLUGIN

#அந்தர்பாவ "opencv2/opencv_modules.hpp"
#அந்தர்பாவ "cvconfig.h"

#அந்தர்பாவ "opencv2/core/utility.hpp"
#அந்தர்பாவ "opencv2/core/cuda.hpp"
#அந்தர்பாவ "opencv2/core/opengl.hpp"
#அந்தர்பாவ "opencv2/core/va_intel.hpp"

#அந்தர்பாவ "opencv2/core/private.hpp"
#அந்தர்பாவ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#அந்தர்பாவ "opencv2/core/ocl.hpp"
#endif

#அந்தர்பாவ <ctype.h>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <math.h>
#அந்தர்பாவ <stdarg.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

#அந்தர்பாவ <algorithm>
#அந்தர்பாவ <cmath>
#அந்தர்பாவ <cstdlib>
#அந்தர்பாவ <limits>
#அந்தர்பாவ <float.h>
#அந்தர்பாவ <cstring>
#அந்தர்பாவ <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#அந்தர்பாவ "opencv2/core/hal/hal.hpp"
#அந்தர்பாவ "opencv2/core/hal/intrin.hpp"
#அந்தர்பாவ "opencv2/core/neon_utils.hpp"
#அந்தர்பாவ "opencv2/core/vsx_utils.hpp"
#அந்தர்பாவ "hal_replacement.hpp"


நாமாகாஶ cv
{

// -128.f ... 255.f
வெளி மாறிலி தசம g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

வெளி மாறிலி ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

வெளி மாறிலி uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (ப்ரதிஜ்ஞா(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpNop
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a) மாறிலி { திரும்பு saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpSqr
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a) மாறிலி { திரும்பு saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpAdd
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a, மாறிலி T2 b) மாறிலி { திரும்பு saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpAddSqr
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a, மாறிலி T2 b) மாறிலி { திரும்பு saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpSub
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a, மாறிலி T2 b) மாறிலி { திரும்பு saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> கட்டமைப்பு OpRSub
{
    வகைவரை T1 type1;
    வகைவரை T2 type2;
    வகைவரை T3 rtype;
    T3 operator ()(மாறிலி T1 a, மாறிலி T2 b) மாறிலி { திரும்பு saturate_cast<T3>(b - a); }
};

template<typename T> கட்டமைப்பு OpMin
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator ()(மாறிலி T a, மாறிலி T b) மாறிலி { திரும்பு std::min(a, b); }
};

template<typename T> கட்டமைப்பு OpMax
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator ()(மாறிலி T a, மாறிலி T b) மாறிலி { திரும்பு std::max(a, b); }
};

template<typename T> கட்டமைப்பு OpAbsDiff
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator()(T a, T b) மாறிலி { திரும்பு a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> கட்டமைப்பு OpAbsDiff<தசம>
{
    வகைவரை தசம type1;
    வகைவரை தசம type2;
    வகைவரை தசம rtype;
    தசம operator()(தசம a, தசம b) மாறிலி { திரும்பு std::abs(a - b); }
};
template<> கட்டமைப்பு OpAbsDiff<இரட்டை>
{
    வகைவரை இரட்டை type1;
    வகைவரை இரட்டை type2;
    வகைவரை இரட்டை rtype;
    இரட்டை operator()(இரட்டை a, இரட்டை b) மாறிலி { திரும்பு std::abs(a - b); }
};

template<typename T> கட்டமைப்பு OpAnd
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator()( T a, T b ) மாறிலி { திரும்பு a & b; }
};

template<typename T> கட்டமைப்பு OpOr
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator()( T a, T b ) மாறிலி { திரும்பு a | b; }
};

template<typename T> கட்டமைப்பு OpXor
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator()( T a, T b ) மாறிலி { திரும்பு a ^ b; }
};

template<typename T> கட்டமைப்பு OpNot
{
    வகைவரை T type1;
    வகைவரை T type2;
    வகைவரை T rtype;
    T operator()( T a, T ) மாறிலி { திரும்பு ~a; }
};

template<> உள்வரி uchar OpAdd<uchar>::operator ()(uchar a, uchar b) மாறிலி
{ திரும்பு CV_FAST_CAST_8U(a + b); }

template<> உள்வரி uchar OpSub<uchar>::operator ()(uchar a, uchar b) மாறிலி
{ திரும்பு CV_FAST_CAST_8U(a - b); }

template<> உள்வரி short OpAbsDiff<short>::operator ()(short a, short b) மாறிலி
{ திரும்பு saturate_cast<short>(std::abs(a - b)); }

template<> உள்வரி schar OpAbsDiff<schar>::operator ()(schar a, schar b) மாறிலி
{ திரும்பு saturate_cast<schar>(std::abs(a - b)); }

template<> உள்வரி uchar OpMin<uchar>::operator ()(uchar a, uchar b) மாறிலி { திரும்பு CV_MIN_8U(a, b); }

template<> உள்வரி uchar OpMax<uchar>::operator ()(uchar a, uchar b) மாறிலி { திரும்பு CV_MAX_8U(a, b); }

வகைவரை வெறுமை (*UnaryFunc)(மாறிலி uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       வெறுமை*);

வகைவரை வெறுமை (*இருமம்_Func)(மாறிலி uchar* src1, size_t step1,
                       மாறிலி uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       வெறுமை*);

வகைவரை வெறுமை (*இருமம்_Func_C)(மாறிலி வெறுமை* src1, size_t step1,
                       மாறிலி வெறுமை* src2, size_t step2,
                       வெறுமை* dst, size_t step, முழு width, முழு height,
                       வெறுமை*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS இருமம்_Func getConvertFunc(முழு sdepth, முழு ddepth);
CV_EXPORTS இருமம்_Func getConvertScaleFunc(முழு sdepth, முழு ddepth);
இருமம்_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, முழு widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, முழு widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, முழு widthScale=1);

வெறுமை setSize( Mat& m, முழு _dims, மாறிலி முழு* _sz, மாறிலி size_t* _steps, தருக்க autoSteps=பொய் );
வெறுமை finalizeHdr(Mat& m);
முழு updateContinuityFlag(முழு flags, முழு dims, மாறிலி முழு* size, மாறிலி size_t* step);

கட்டமைப்பு NoVec
{
    size_t operator()(மாறிலி வெறுமை*, மாறிலி வெறுமை*, வெறுமை*, size_t) மாறிலி { திரும்பு 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

எண்ணிக்கை { BLOCK_SIZE = 1024 };

#எனில் defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#இல்லையெனில்
#define ARITHM_USE_IPP 0
#endif

உள்வரி தருக்க checkScalar(மாறிலி Mat& sc, முழு atype, உள்ளீடு_Array_2::KindFlag sckind, உள்ளீடு_Array_2::KindFlag akind)
{
    எனில்( sc.dims > 2 || !sc.isContinuous() )
        திரும்பு பொய்;
    Size sz = sc.size();
    எனில்(sz.width != 1 && sz.height != 1)
        திரும்பு பொய்;
    முழு cn = CV_MAT_CN(atype);
    எனில்( akind == உள்ளீடு_Array_2::MATX && sckind != உள்ளீடு_Array_2::MATX )
        திரும்பு பொய்;
    திரும்பு sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

உள்வரி தருக்க checkScalar(உள்ளீடு_Array sc, முழு atype, உள்ளீடு_Array_2::KindFlag sckind, உள்ளீடு_Array_2::KindFlag akind)
{
    எனில்( sc.dims() > 2 || !sc.isContinuous() )
        திரும்பு பொய்;
    Size sz = sc.size();
    எனில்(sz.width != 1 && sz.height != 1)
        திரும்பு பொய்;
    முழு cn = CV_MAT_CN(atype);
    எனில்( akind == உள்ளீடு_Array_2::MATX && sckind != உள்ளீடு_Array_2::MATX )
        திரும்பு பொய்;
    திரும்பு sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
உள்வரி தருக்க sameShapeAsArg(மாறிலி உள்ளீடு_Array_2& sc, மாறிலி உள்ளீடு_Array_2& arr)
{
    திரும்பு arr.channels() == 1 && sc.sameSize(arr);
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
உள்வரி தருக்க isScalarArg(மாறிலி உள்ளீடு_Array_2& sc, மாறிலி உள்ளீடு_Array_2& arr)
{
    மாறிலி உள்ளீடு_Array_2::KindFlag kind = sc.kind();
    மாறிலி முழு cn = arr.channels();
    எனில் (kind == உள்ளீடு_Array_2::MATX)
    {
        Size sz = sc.getSz();
        முழு scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        எனில் (scn0 != sz.width + sz.height - 1)
            திரும்பு பொய்;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        திரும்பு scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    எனில் (kind == உள்ளீடு_Array_2::MAT)
    {
        மாறிலி Mat& m = *(மாறிலி Mat*)sc.getObj();
        திரும்பு m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    எனில் (kind == உள்ளீடு_Array_2::UMAT)
    {
        மாறிலி UMat& m = *(மாறிலி UMat*)sc.getObj();
        திரும்பு m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    திரும்பு பொய்;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
உள்வரி தருக்க isScalarLikeMat(மாறிலி உள்ளீடு_Array_2& sc, முழு cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    முழு rows, cols, nval;
    தருக்க continuous;
    மாறிலி உள்ளீடு_Array_2::KindFlag kind = sc.kind();
    எனில் (kind == உள்ளீடு_Array_2::MAT)
    {
        மாறிலி Mat& m = *(மாறிலி Mat*)sc.getObj();
        எனில் ((m.rows != 1 && m.cols != 1) || m.dims > 2) திரும்பு பொய்;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    இல்லையெனில் எனில் (kind == உள்ளீடு_Array_2::UMAT)
    {
        மாறிலி UMat& m = *(மாறிலி UMat*)sc.getObj();
        எனில் ((m.rows != 1 && m.cols != 1) || m.dims > 2) திரும்பு பொய்;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    இல்லையெனில்
        திரும்பு பொய்;
    எனில் (!continuous || nval > 4 || (nval != 1 && nval != cn))
        திரும்பு பொய்;
    திரும்பு (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
எண்ணிக்கை { EW_SCALAR_BUF_SIZE = 4 * அளவு(இரட்டை) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
உள்வரி முழு scalarArgElems(மாறிலி உள்ளீடு_Array_2& sc, மாறிலி uchar*& p, முழு& d, uchar* scbuf)
{
    மாறிலி உள்ளீடு_Array_2::KindFlag kind = sc.kind();
    எனில் (kind == உள்ளீடு_Array_2::MAT)
    {
        மாறிலி Mat& m = *(மாறிலி Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        திரும்பு (முழு)m.total() * m.channels();
    }
    எனில் (kind == உள்ளீடு_Array_2::UMAT)
    {
        மாறிலி UMat& u = *(மாறிலி UMat*)sc.getObj();
        d = u.depth();
        முழு n = (முழு)u.total() * u.channels();
        CV_Assert(n * (முழு)CV_ELEM_SIZE1(d) <= (முழு)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        திரும்பு n;
    }
    p = (மாறிலி uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    திரும்பு sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
உள்வரி இரட்டை elemToDouble(முழு d, மாறிலி uchar* p)
{
    தேர்வு (d)
    {
    நிலை CV_8U:   திரும்பு *(மாறிலி uchar*)p;
    நிலை CV_8S:   திரும்பு *(மாறிலி schar*)p;
    நிலை CV_16U:  திரும்பு *(மாறிலி ushort*)p;
    நிலை CV_16S:  திரும்பு *(மாறிலி short*)p;
    நிலை CV_32U:  திரும்பு *(மாறிலி unsigned*)p;
    நிலை CV_32S:  திரும்பு *(மாறிலி முழு*)p;
    நிலை CV_64U:  திரும்பு (இரட்டை)*(மாறிலி uint64_t*)p;
    நிலை CV_64S:  திரும்பு (இரட்டை)*(மாறிலி int64_t*)p;
    நிலை CV_16F:  திரும்பு (தசம)*(மாறிலி hfloat*)p;
    நிலை CV_16BF: திரும்பு (தசம)*(மாறிலி bfloat*)p;
    நிலை CV_32F:  திரும்பு *(மாறிலி தசம*)p;
    நிலை CV_64F:  திரும்பு *(மாறிலி இரட்டை*)p;
    இயல்பு:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
உள்வரி முழு readScalarArg(மாறிலி உள்ளீடு_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    மாறிலி uchar* p; முழு d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    முழு n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    க்கு (முழு i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    திரும்பு n;
}

வெறுமை convertAndUnrollScalar( மாறிலி Mat& sc, முழு buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
கட்டமைப்பு ImplCollector
{
    ImplCollector()
    {
        useCollection   = பொய்;
        implFlags       = 0;
    }
    தருக்க useCollection; // enable/disable impl data collection

    முழு implFlags;
    std::vector<முழு>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

கட்டமைப்பு CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(பொய்), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    தருக்க oclExecutionContextInitialized;
    முழு useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    முழு useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    முழு useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#எனில் defined(BUILD_SHARED_LIBS)
#எனில் defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#அல்லதெனில் defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#இல்லையெனில்
#define CL_RUNTIME_EXPORT
#endif
#இல்லையெனில்
#define CL_RUNTIME_EXPORT
#endif

வெளி CV_EXPORTS
தருக்க __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_மதிப்பு) \
    நிலையான TYPE* மாறிலி instance = INITIALIZER; \
    திரும்பு RET_மதிப்பு;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS வெறுமை releaseTlsStorageThread();

முழு cv_snprintf(எழுத்து* buf, முழு len, மாறிலி எழுத்து* fmt, ...);
முழு cv_vsnprintf(எழுத்து* buf, முழு len, மாறிலி எழுத்து* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
