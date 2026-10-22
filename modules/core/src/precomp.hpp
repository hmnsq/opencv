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
#شامل "opencv2/core/utility.hpp"
#وگرنه  // BUILD_PLUGIN

#شامل "opencv2/opencv_modules.hpp"
#شامل "cvconfig.h"

#شامل "opencv2/core/utility.hpp"
#شامل "opencv2/core/cuda.hpp"
#شامل "opencv2/core/opengl.hpp"
#شامل "opencv2/core/va_intel.hpp"

#شامل "opencv2/core/private.hpp"
#شامل "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#شامل "opencv2/core/ocl.hpp"
#endif

#شامل <ctype.h>
#شامل <float.h>
#شامل <limits.h>
#شامل <math.h>
#شامل <stdarg.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

#شامل <algorithm>
#شامل <cmath>
#شامل <cstdlib>
#شامل <limits>
#شامل <float.h>
#شامل <cstring>
#شامل <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#شامل "opencv2/core/hal/hal.hpp"
#شامل "opencv2/core/hal/intrin.hpp"
#شامل "opencv2/core/neon_utils.hpp"
#شامل "opencv2/core/vsx_utils.hpp"
#شامل "hal_replacement.hpp"


فضاینام cv
{

// -128.f ... 255.f
بیرونی ثابت اعشاری g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

بیرونی ثابت ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

بیرونی ثابت uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (أكد(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpNop
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a) ثابت { بازگشت saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpSqr
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a) ثابت { بازگشت saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpAdd
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a, ثابت T2 b) ثابت { بازگشت saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpAddSqr
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a, ثابت T2 b) ثابت { بازگشت saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpSub
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a, ثابت T2 b) ثابت { بازگشت saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ساختار OpRSub
{
    تعریفنوع T1 type1;
    تعریفنوع T2 type2;
    تعریفنوع T3 rtype;
    T3 operator ()(ثابت T1 a, ثابت T2 b) ثابت { بازگشت saturate_cast<T3>(b - a); }
};

template<typename T> ساختار OpMin
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator ()(ثابت T a, ثابت T b) ثابت { بازگشت std::min(a, b); }
};

template<typename T> ساختار OpMax
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator ()(ثابت T a, ثابت T b) ثابت { بازگشت std::max(a, b); }
};

template<typename T> ساختار OpAbsDiff
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator()(T a, T b) ثابت { بازگشت a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> ساختار OpAbsDiff<اعشاری>
{
    تعریفنوع اعشاری type1;
    تعریفنوع اعشاری type2;
    تعریفنوع اعشاری rtype;
    اعشاری operator()(اعشاری a, اعشاری b) ثابت { بازگشت std::abs(a - b); }
};
template<> ساختار OpAbsDiff<دوگانه>
{
    تعریفنوع دوگانه type1;
    تعریفنوع دوگانه type2;
    تعریفنوع دوگانه rtype;
    دوگانه operator()(دوگانه a, دوگانه b) ثابت { بازگشت std::abs(a - b); }
};

template<typename T> ساختار OpAnd
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator()( T a, T b ) ثابت { بازگشت a & b; }
};

template<typename T> ساختار OpOr
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator()( T a, T b ) ثابت { بازگشت a | b; }
};

template<typename T> ساختار OpXor
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator()( T a, T b ) ثابت { بازگشت a ^ b; }
};

template<typename T> ساختار OpNot
{
    تعریفنوع T type1;
    تعریفنوع T type2;
    تعریفنوع T rtype;
    T operator()( T a, T ) ثابت { بازگشت ~a; }
};

template<> درونخطی uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ثابت
{ بازگشت CV_FAST_CAST_8U(a + b); }

template<> درونخطی uchar OpSub<uchar>::operator ()(uchar a, uchar b) ثابت
{ بازگشت CV_FAST_CAST_8U(a - b); }

template<> درونخطی short OpAbsDiff<short>::operator ()(short a, short b) ثابت
{ بازگشت saturate_cast<short>(std::abs(a - b)); }

template<> درونخطی schar OpAbsDiff<schar>::operator ()(schar a, schar b) ثابت
{ بازگشت saturate_cast<schar>(std::abs(a - b)); }

template<> درونخطی uchar OpMin<uchar>::operator ()(uchar a, uchar b) ثابت { بازگشت CV_MIN_8U(a, b); }

template<> درونخطی uchar OpMax<uchar>::operator ()(uchar a, uchar b) ثابت { بازگشت CV_MAX_8U(a, b); }

تعریفنوع تهی (*UnaryFunc)(ثابت uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       تهی*);

تعریفنوع تهی (*دودویی_Func)(ثابت uchar* src1, size_t step1,
                       ثابت uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       تهی*);

تعریفنوع تهی (*دودویی_Func_C)(ثابت تهی* src1, size_t step1,
                       ثابت تهی* src2, size_t step2,
                       تهی* dst, size_t step, صحیح width, صحیح height,
                       تهی*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS دودویی_Func getConvertFunc(صحیح sdepth, صحیح ddepth);
CV_EXPORTS دودویی_Func getConvertScaleFunc(صحیح sdepth, صحیح ddepth);
دودویی_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, صحیح widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, صحیح widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, صحیح widthScale=1);

تهی setSize( Mat& m, صحیح _dims, ثابت صحیح* _sz, ثابت size_t* _steps, منطقی autoSteps=نادرست );
تهی finalizeHdr(Mat& m);
صحیح updateContinuityFlag(صحیح flags, صحیح dims, ثابت صحیح* size, ثابت size_t* step);

ساختار NoVec
{
    size_t operator()(ثابت تهی*, ثابت تهی*, تهی*, size_t) ثابت { بازگشت 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

شمارش { BLOCK_SIZE = 1024 };

#اگر defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#وگرنه
#define ARITHM_USE_IPP 0
#endif

درونخطی منطقی checkScalar(ثابت Mat& sc, صحیح atype, درون_داد_Array_2::KindFlag sckind, درون_داد_Array_2::KindFlag akind)
{
    اگر( sc.dims > 2 || !sc.isContinuous() )
        بازگشت نادرست;
    Size sz = sc.size();
    اگر(sz.width != 1 && sz.height != 1)
        بازگشت نادرست;
    صحیح cn = CV_MAT_CN(atype);
    اگر( akind == درون_داد_Array_2::MATX && sckind != درون_داد_Array_2::MATX )
        بازگشت نادرست;
    بازگشت sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

درونخطی منطقی checkScalar(درون_داد_Array sc, صحیح atype, درون_داد_Array_2::KindFlag sckind, درون_داد_Array_2::KindFlag akind)
{
    اگر( sc.dims() > 2 || !sc.isContinuous() )
        بازگشت نادرست;
    Size sz = sc.size();
    اگر(sz.width != 1 && sz.height != 1)
        بازگشت نادرست;
    صحیح cn = CV_MAT_CN(atype);
    اگر( akind == درون_داد_Array_2::MATX && sckind != درون_داد_Array_2::MATX )
        بازگشت نادرست;
    بازگشت sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
درونخطی منطقی sameShapeAsArg(ثابت درون_داد_Array_2& sc, ثابت درون_داد_Array_2& arr)
{
    بازگشت arr.channels() == 1 && sc.sameSize(arr);
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
درونخطی منطقی isScalarArg(ثابت درون_داد_Array_2& sc, ثابت درون_داد_Array_2& arr)
{
    ثابت درون_داد_Array_2::KindFlag kind = sc.kind();
    ثابت صحیح cn = arr.channels();
    اگر (kind == درون_داد_Array_2::MATX)
    {
        Size sz = sc.getSz();
        صحیح scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        اگر (scn0 != sz.width + sz.height - 1)
            بازگشت نادرست;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        بازگشت scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    اگر (kind == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        بازگشت m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    اگر (kind == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        بازگشت m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    بازگشت نادرست;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
درونخطی منطقی isScalarLikeMat(ثابت درون_داد_Array_2& sc, صحیح cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    صحیح rows, cols, nval;
    منطقی continuous;
    ثابت درون_داد_Array_2::KindFlag kind = sc.kind();
    اگر (kind == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        اگر ((m.rows != 1 && m.cols != 1) || m.dims > 2) بازگشت نادرست;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    وگرنه اگر (kind == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& m = *(ثابت UMat*)sc.getObj();
        اگر ((m.rows != 1 && m.cols != 1) || m.dims > 2) بازگشت نادرست;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    وگرنه
        بازگشت نادرست;
    اگر (!continuous || nval > 4 || (nval != 1 && nval != cn))
        بازگشت نادرست;
    بازگشت (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
شمارش { EW_SCALAR_BUF_SIZE = 4 * اندازه(دوگانه) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
درونخطی صحیح scalarArgElems(ثابت درون_داد_Array_2& sc, ثابت uchar*& p, صحیح& d, uchar* scbuf)
{
    ثابت درون_داد_Array_2::KindFlag kind = sc.kind();
    اگر (kind == درون_داد_Array_2::MAT)
    {
        ثابت Mat& m = *(ثابت Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        بازگشت (صحیح)m.total() * m.channels();
    }
    اگر (kind == درون_داد_Array_2::UMAT)
    {
        ثابت UMat& u = *(ثابت UMat*)sc.getObj();
        d = u.depth();
        صحیح n = (صحیح)u.total() * u.channels();
        CV_Assert(n * (صحیح)CV_ELEM_SIZE1(d) <= (صحیح)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        بازگشت n;
    }
    p = (ثابت uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    بازگشت sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
درونخطی دوگانه elemToDouble(صحیح d, ثابت uchar* p)
{
    انتخاب (d)
    {
    حالت CV_8U:   بازگشت *(ثابت uchar*)p;
    حالت CV_8S:   بازگشت *(ثابت schar*)p;
    حالت CV_16U:  بازگشت *(ثابت ushort*)p;
    حالت CV_16S:  بازگشت *(ثابت short*)p;
    حالت CV_32U:  بازگشت *(ثابت unsigned*)p;
    حالت CV_32S:  بازگشت *(ثابت صحیح*)p;
    حالت CV_64U:  بازگشت (دوگانه)*(ثابت uint64_t*)p;
    حالت CV_64S:  بازگشت (دوگانه)*(ثابت int64_t*)p;
    حالت CV_16F:  بازگشت (اعشاری)*(ثابت hfloat*)p;
    حالت CV_16BF: بازگشت (اعشاری)*(ثابت bfloat*)p;
    حالت CV_32F:  بازگشت *(ثابت اعشاری*)p;
    حالت CV_64F:  بازگشت *(ثابت دوگانه*)p;
    پیشفرض:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
درونخطی صحیح readScalarArg(ثابت درون_داد_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ثابت uchar* p; صحیح d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    صحیح n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    برای (صحیح i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    بازگشت n;
}

تهی convertAndUnrollScalar( ثابت Mat& sc, صحیح buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
ساختار ImplCollector
{
    ImplCollector()
    {
        useCollection   = نادرست;
        implFlags       = 0;
    }
    منطقی useCollection; // enable/disable impl data collection

    صحیح implFlags;
    std::vector<صحیح>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

ساختار CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(نادرست), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    منطقی oclExecutionContextInitialized;
    صحیح useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    صحیح useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    صحیح useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#اگر defined(BUILD_SHARED_LIBS)
#اگر defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#وگرنهاگر defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#وگرنه
#define CL_RUNTIME_EXPORT
#endif
#وگرنه
#define CL_RUNTIME_EXPORT
#endif

بیرونی CV_EXPORTS
منطقی __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_مقدار) \
    ایستا TYPE* ثابت instance = INITIALIZER; \
    بازگشت RET_مقدار;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS تهی releaseTlsStorageThread();

صحیح cv_snprintf(نویسه* buf, صحیح len, ثابت نویسه* fmt, ...);
صحیح cv_vsnprintf(نویسه* buf, صحیح len, ثابت نویسه* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
