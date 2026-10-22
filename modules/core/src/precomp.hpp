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
#അന്തര്ഭാവ "opencv2/core/utility.hpp"
#അല്ലെങ്കിൽ  // BUILD_PLUGIN

#അന്തര്ഭാവ "opencv2/opencv_modules.hpp"
#അന്തര്ഭാവ "cvconfig.h"

#അന്തര്ഭാവ "opencv2/core/utility.hpp"
#അന്തര്ഭാവ "opencv2/core/cuda.hpp"
#അന്തര്ഭാവ "opencv2/core/opengl.hpp"
#അന്തര്ഭാവ "opencv2/core/va_intel.hpp"

#അന്തര്ഭാവ "opencv2/core/private.hpp"
#അന്തര്ഭാവ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#അന്തര്ഭാവ "opencv2/core/ocl.hpp"
#endif

#അന്തര്ഭാവ <ctype.h>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <math.h>
#അന്തര്ഭാവ <stdarg.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

#അന്തര്ഭാവ <algorithm>
#അന്തര്ഭാവ <cmath>
#അന്തര്ഭാവ <cstdlib>
#അന്തര്ഭാവ <limits>
#അന്തര്ഭാവ <float.h>
#അന്തര്ഭാവ <cstring>
#അന്തര്ഭാവ <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#അന്തര്ഭാവ "opencv2/core/hal/hal.hpp"
#അന്തര്ഭാവ "opencv2/core/hal/intrin.hpp"
#അന്തര്ഭാവ "opencv2/core/neon_utils.hpp"
#അന്തര്ഭാവ "opencv2/core/vsx_utils.hpp"
#അന്തര്ഭാവ "hal_replacement.hpp"


നാമാകാശ cv
{

// -128.f ... 255.f
ബാഹ്യ സ്ഥിരം ദശാംശം g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

ബാഹ്യ സ്ഥിരം ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

ബാഹ്യ സ്ഥിരം uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (പ്രതിജ്ഞാ(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpNop
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a) സ്ഥിരം { തിരികെ saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpSqr
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a) സ്ഥിരം { തിരികെ saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpAdd
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a, സ്ഥിരം T2 b) സ്ഥിരം { തിരികെ saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpAddSqr
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a, സ്ഥിരം T2 b) സ്ഥിരം { തിരികെ saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpSub
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a, സ്ഥിരം T2 b) സ്ഥിരം { തിരികെ saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> ഘടന OpRSub
{
    തരനിർവചനം T1 type1;
    തരനിർവചനം T2 type2;
    തരനിർവചനം T3 rtype;
    T3 operator ()(സ്ഥിരം T1 a, സ്ഥിരം T2 b) സ്ഥിരം { തിരികെ saturate_cast<T3>(b - a); }
};

template<typename T> ഘടന OpMin
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator ()(സ്ഥിരം T a, സ്ഥിരം T b) സ്ഥിരം { തിരികെ std::min(a, b); }
};

template<typename T> ഘടന OpMax
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator ()(സ്ഥിരം T a, സ്ഥിരം T b) സ്ഥിരം { തിരികെ std::max(a, b); }
};

template<typename T> ഘടന OpAbsDiff
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator()(T a, T b) സ്ഥിരം { തിരികെ a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> ഘടന OpAbsDiff<ദശാംശം>
{
    തരനിർവചനം ദശാംശം type1;
    തരനിർവചനം ദശാംശം type2;
    തരനിർവചനം ദശാംശം rtype;
    ദശാംശം operator()(ദശാംശം a, ദശാംശം b) സ്ഥിരം { തിരികെ std::abs(a - b); }
};
template<> ഘടന OpAbsDiff<ഇരട്ട>
{
    തരനിർവചനം ഇരട്ട type1;
    തരനിർവചനം ഇരട്ട type2;
    തരനിർവചനം ഇരട്ട rtype;
    ഇരട്ട operator()(ഇരട്ട a, ഇരട്ട b) സ്ഥിരം { തിരികെ std::abs(a - b); }
};

template<typename T> ഘടന OpAnd
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator()( T a, T b ) സ്ഥിരം { തിരികെ a & b; }
};

template<typename T> ഘടന OpOr
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator()( T a, T b ) സ്ഥിരം { തിരികെ a | b; }
};

template<typename T> ഘടന OpXor
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator()( T a, T b ) സ്ഥിരം { തിരികെ a ^ b; }
};

template<typename T> ഘടന OpNot
{
    തരനിർവചനം T type1;
    തരനിർവചനം T type2;
    തരനിർവചനം T rtype;
    T operator()( T a, T ) സ്ഥിരം { തിരികെ ~a; }
};

template<> വരിയിൽ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) സ്ഥിരം
{ തിരികെ CV_FAST_CAST_8U(a + b); }

template<> വരിയിൽ uchar OpSub<uchar>::operator ()(uchar a, uchar b) സ്ഥിരം
{ തിരികെ CV_FAST_CAST_8U(a - b); }

template<> വരിയിൽ short OpAbsDiff<short>::operator ()(short a, short b) സ്ഥിരം
{ തിരികെ saturate_cast<short>(std::abs(a - b)); }

template<> വരിയിൽ schar OpAbsDiff<schar>::operator ()(schar a, schar b) സ്ഥിരം
{ തിരികെ saturate_cast<schar>(std::abs(a - b)); }

template<> വരിയിൽ uchar OpMin<uchar>::operator ()(uchar a, uchar b) സ്ഥിരം { തിരികെ CV_MIN_8U(a, b); }

template<> വരിയിൽ uchar OpMax<uchar>::operator ()(uchar a, uchar b) സ്ഥിരം { തിരികെ CV_MAX_8U(a, b); }

തരനിർവചനം ശൂന്യം (*UnaryFunc)(സ്ഥിരം uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       ശൂന്യം*);

തരനിർവചനം ശൂന്യം (*ദ്വിമാന_Func)(സ്ഥിരം uchar* src1, size_t step1,
                       സ്ഥിരം uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       ശൂന്യം*);

തരനിർവചനം ശൂന്യം (*ദ്വിമാന_Func_C)(സ്ഥിരം ശൂന്യം* src1, size_t step1,
                       സ്ഥിരം ശൂന്യം* src2, size_t step2,
                       ശൂന്യം* dst, size_t step, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height,
                       ശൂന്യം*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS ദ്വിമാന_Func getConvertFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
CV_EXPORTS ദ്വിമാന_Func getConvertScaleFunc(പൂർണ്ണസംഖ്യ sdepth, പൂർണ്ണസംഖ്യ ddepth);
ദ്വിമാന_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, പൂർണ്ണസംഖ്യ widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, പൂർണ്ണസംഖ്യ widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, പൂർണ്ണസംഖ്യ widthScale=1);

ശൂന്യം setSize( Mat& m, പൂർണ്ണസംഖ്യ _dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* _sz, സ്ഥിരം size_t* _steps, തർക്കം autoSteps=അസത്യം );
ശൂന്യം finalizeHdr(Mat& m);
പൂർണ്ണസംഖ്യ updateContinuityFlag(പൂർണ്ണസംഖ്യ flags, പൂർണ്ണസംഖ്യ dims, സ്ഥിരം പൂർണ്ണസംഖ്യ* size, സ്ഥിരം size_t* step);

ഘടന NoVec
{
    size_t operator()(സ്ഥിരം ശൂന്യം*, സ്ഥിരം ശൂന്യം*, ശൂന്യം*, size_t) സ്ഥിരം { തിരികെ 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

എണ്ണൽ { BLOCK_SIZE = 1024 };

#എങ്കിൽ defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#അല്ലെങ്കിൽ
#define ARITHM_USE_IPP 0
#endif

വരിയിൽ തർക്കം checkScalar(സ്ഥിരം Mat& sc, പൂർണ്ണസംഖ്യ atype, നിവേശം_Array_2::KindFlag sckind, നിവേശം_Array_2::KindFlag akind)
{
    എങ്കിൽ( sc.dims > 2 || !sc.isContinuous() )
        തിരികെ അസത്യം;
    Size sz = sc.size();
    എങ്കിൽ(sz.width != 1 && sz.height != 1)
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(atype);
    എങ്കിൽ( akind == നിവേശം_Array_2::MATX && sckind != നിവേശം_Array_2::MATX )
        തിരികെ അസത്യം;
    തിരികെ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

വരിയിൽ തർക്കം checkScalar(നിവേശം_Array sc, പൂർണ്ണസംഖ്യ atype, നിവേശം_Array_2::KindFlag sckind, നിവേശം_Array_2::KindFlag akind)
{
    എങ്കിൽ( sc.dims() > 2 || !sc.isContinuous() )
        തിരികെ അസത്യം;
    Size sz = sc.size();
    എങ്കിൽ(sz.width != 1 && sz.height != 1)
        തിരികെ അസത്യം;
    പൂർണ്ണസംഖ്യ cn = CV_MAT_CN(atype);
    എങ്കിൽ( akind == നിവേശം_Array_2::MATX && sckind != നിവേശം_Array_2::MATX )
        തിരികെ അസത്യം;
    തിരികെ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
വരിയിൽ തർക്കം sameShapeAsArg(സ്ഥിരം നിവേശം_Array_2& sc, സ്ഥിരം നിവേശം_Array_2& arr)
{
    തിരികെ arr.channels() == 1 && sc.sameSize(arr);
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
വരിയിൽ തർക്കം isScalarArg(സ്ഥിരം നിവേശം_Array_2& sc, സ്ഥിരം നിവേശം_Array_2& arr)
{
    സ്ഥിരം നിവേശം_Array_2::KindFlag kind = sc.kind();
    സ്ഥിരം പൂർണ്ണസംഖ്യ cn = arr.channels();
    എങ്കിൽ (kind == നിവേശം_Array_2::MATX)
    {
        Size sz = sc.getSz();
        പൂർണ്ണസംഖ്യ scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        എങ്കിൽ (scn0 != sz.width + sz.height - 1)
            തിരികെ അസത്യം;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        തിരികെ scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    എങ്കിൽ (kind == നിവേശം_Array_2::MAT)
    {
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)sc.getObj();
        തിരികെ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    എങ്കിൽ (kind == നിവേശം_Array_2::UMAT)
    {
        സ്ഥിരം UMat& m = *(സ്ഥിരം UMat*)sc.getObj();
        തിരികെ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    തിരികെ അസത്യം;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
വരിയിൽ തർക്കം isScalarLikeMat(സ്ഥിരം നിവേശം_Array_2& sc, പൂർണ്ണസംഖ്യ cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    പൂർണ്ണസംഖ്യ rows, cols, nval;
    തർക്കം continuous;
    സ്ഥിരം നിവേശം_Array_2::KindFlag kind = sc.kind();
    എങ്കിൽ (kind == നിവേശം_Array_2::MAT)
    {
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)sc.getObj();
        എങ്കിൽ ((m.rows != 1 && m.cols != 1) || m.dims > 2) തിരികെ അസത്യം;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    അല്ലെങ്കിൽ എങ്കിൽ (kind == നിവേശം_Array_2::UMAT)
    {
        സ്ഥിരം UMat& m = *(സ്ഥിരം UMat*)sc.getObj();
        എങ്കിൽ ((m.rows != 1 && m.cols != 1) || m.dims > 2) തിരികെ അസത്യം;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    അല്ലെങ്കിൽ
        തിരികെ അസത്യം;
    എങ്കിൽ (!continuous || nval > 4 || (nval != 1 && nval != cn))
        തിരികെ അസത്യം;
    തിരികെ (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
എണ്ണൽ { EW_SCALAR_BUF_SIZE = 4 * വലുപ്പം(ഇരട്ട) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
വരിയിൽ പൂർണ്ണസംഖ്യ scalarArgElems(സ്ഥിരം നിവേശം_Array_2& sc, സ്ഥിരം uchar*& p, പൂർണ്ണസംഖ്യ& d, uchar* scbuf)
{
    സ്ഥിരം നിവേശം_Array_2::KindFlag kind = sc.kind();
    എങ്കിൽ (kind == നിവേശം_Array_2::MAT)
    {
        സ്ഥിരം Mat& m = *(സ്ഥിരം Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        തിരികെ (പൂർണ്ണസംഖ്യ)m.total() * m.channels();
    }
    എങ്കിൽ (kind == നിവേശം_Array_2::UMAT)
    {
        സ്ഥിരം UMat& u = *(സ്ഥിരം UMat*)sc.getObj();
        d = u.depth();
        പൂർണ്ണസംഖ്യ n = (പൂർണ്ണസംഖ്യ)u.total() * u.channels();
        CV_Assert(n * (പൂർണ്ണസംഖ്യ)CV_ELEM_SIZE1(d) <= (പൂർണ്ണസംഖ്യ)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        തിരികെ n;
    }
    p = (സ്ഥിരം uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    തിരികെ sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
വരിയിൽ ഇരട്ട elemToDouble(പൂർണ്ണസംഖ്യ d, സ്ഥിരം uchar* p)
{
    തിരഞ്ഞെടുപ്പ് (d)
    {
    അവസ്ഥ CV_8U:   തിരികെ *(സ്ഥിരം uchar*)p;
    അവസ്ഥ CV_8S:   തിരികെ *(സ്ഥിരം schar*)p;
    അവസ്ഥ CV_16U:  തിരികെ *(സ്ഥിരം ushort*)p;
    അവസ്ഥ CV_16S:  തിരികെ *(സ്ഥിരം short*)p;
    അവസ്ഥ CV_32U:  തിരികെ *(സ്ഥിരം unsigned*)p;
    അവസ്ഥ CV_32S:  തിരികെ *(സ്ഥിരം പൂർണ്ണസംഖ്യ*)p;
    അവസ്ഥ CV_64U:  തിരികെ (ഇരട്ട)*(സ്ഥിരം uint64_t*)p;
    അവസ്ഥ CV_64S:  തിരികെ (ഇരട്ട)*(സ്ഥിരം int64_t*)p;
    അവസ്ഥ CV_16F:  തിരികെ (ദശാംശം)*(സ്ഥിരം hfloat*)p;
    അവസ്ഥ CV_16BF: തിരികെ (ദശാംശം)*(സ്ഥിരം bfloat*)p;
    അവസ്ഥ CV_32F:  തിരികെ *(സ്ഥിരം ദശാംശം*)p;
    അവസ്ഥ CV_64F:  തിരികെ *(സ്ഥിരം ഇരട്ട*)p;
    സ്ഥിരസ്ഥിതി:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
വരിയിൽ പൂർണ്ണസംഖ്യ readScalarArg(സ്ഥിരം നിവേശം_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    സ്ഥിരം uchar* p; പൂർണ്ണസംഖ്യ d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    പൂർണ്ണസംഖ്യ n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    തിരികെ n;
}

ശൂന്യം convertAndUnrollScalar( സ്ഥിരം Mat& sc, പൂർണ്ണസംഖ്യ buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
ഘടന ImplCollector
{
    ImplCollector()
    {
        useCollection   = അസത്യം;
        implFlags       = 0;
    }
    തർക്കം useCollection; // enable/disable impl data collection

    പൂർണ്ണസംഖ്യ implFlags;
    std::vector<പൂർണ്ണസംഖ്യ>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

ഘടന CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(അസത്യം), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    തർക്കം oclExecutionContextInitialized;
    പൂർണ്ണസംഖ്യ useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    പൂർണ്ണസംഖ്യ useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    പൂർണ്ണസംഖ്യ useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#എങ്കിൽ defined(BUILD_SHARED_LIBS)
#എങ്കിൽ defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#അല്ലെങ്കിലെങ്കിൽ defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#അല്ലെങ്കിൽ
#define CL_RUNTIME_EXPORT
#endif
#അല്ലെങ്കിൽ
#define CL_RUNTIME_EXPORT
#endif

ബാഹ്യ CV_EXPORTS
തർക്കം __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_മൂല്യം) \
    സ്ഥിതം TYPE* സ്ഥിരം instance = INITIALIZER; \
    തിരികെ RET_മൂല്യം;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS ശൂന്യം releaseTlsStorageThread();

പൂർണ്ണസംഖ്യ cv_snprintf(അക്ഷരം* buf, പൂർണ്ണസംഖ്യ len, സ്ഥിരം അക്ഷരം* fmt, ...);
പൂർണ്ണസംഖ്യ cv_vsnprintf(അക്ഷരം* buf, പൂർണ്ണസംഖ്യ len, സ്ഥിരം അക്ഷരം* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
