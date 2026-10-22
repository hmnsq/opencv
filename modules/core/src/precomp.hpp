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
#અન્તર્ભાવ "opencv2/core/utility.hpp"
#નહીંતર  // BUILD_PLUGIN

#અન્તર્ભાવ "opencv2/opencv_modules.hpp"
#અન્તર્ભાવ "cvconfig.h"

#અન્તર્ભાવ "opencv2/core/utility.hpp"
#અન્તર્ભાવ "opencv2/core/cuda.hpp"
#અન્તર્ભાવ "opencv2/core/opengl.hpp"
#અન્તર્ભાવ "opencv2/core/va_intel.hpp"

#અન્તર્ભાવ "opencv2/core/private.hpp"
#અન્તર્ભાવ "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#અન્તર્ભાવ "opencv2/core/ocl.hpp"
#endif

#અન્તર્ભાવ <ctype.h>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <math.h>
#અન્તર્ભાવ <stdarg.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

#અન્તર્ભાવ <algorithm>
#અન્તર્ભાવ <cmath>
#અન્તર્ભાવ <cstdlib>
#અન્તર્ભાવ <limits>
#અન્તર્ભાવ <float.h>
#અન્તર્ભાવ <cstring>
#અન્તર્ભાવ <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#અન્તર્ભાવ "opencv2/core/hal/hal.hpp"
#અન્તર્ભાવ "opencv2/core/hal/intrin.hpp"
#અન્તર્ભાવ "opencv2/core/neon_utils.hpp"
#અન્તર્ભાવ "opencv2/core/vsx_utils.hpp"
#અન્તર્ભાવ "hal_replacement.hpp"


નામાકાશ cv
{

// -128.f ... 255.f
બાહ્ય અચલ દશાંશ g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

બાહ્ય અચલ ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

બાહ્ય અચલ uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (પ્રતિજ્ઞા(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpNop
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a) અચલ { પાછા saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpSqr
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a) અચલ { પાછા saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpAdd
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a, અચલ T2 b) અચલ { પાછા saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpAddSqr
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a, અચલ T2 b) અચલ { પાછા saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpSub
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a, અચલ T2 b) અચલ { પાછા saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> સંરચના OpRSub
{
    પ્રકારવ્યાખ્યા T1 type1;
    પ્રકારવ્યાખ્યા T2 type2;
    પ્રકારવ્યાખ્યા T3 rtype;
    T3 operator ()(અચલ T1 a, અચલ T2 b) અચલ { પાછા saturate_cast<T3>(b - a); }
};

template<typename T> સંરચના OpMin
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator ()(અચલ T a, અચલ T b) અચલ { પાછા std::min(a, b); }
};

template<typename T> સંરચના OpMax
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator ()(અચલ T a, અચલ T b) અચલ { પાછા std::max(a, b); }
};

template<typename T> સંરચના OpAbsDiff
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator()(T a, T b) અચલ { પાછા a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> સંરચના OpAbsDiff<દશાંશ>
{
    પ્રકારવ્યાખ્યા દશાંશ type1;
    પ્રકારવ્યાખ્યા દશાંશ type2;
    પ્રકારવ્યાખ્યા દશાંશ rtype;
    દશાંશ operator()(દશાંશ a, દશાંશ b) અચલ { પાછા std::abs(a - b); }
};
template<> સંરચના OpAbsDiff<દ્વિગુણ>
{
    પ્રકારવ્યાખ્યા દ્વિગુણ type1;
    પ્રકારવ્યાખ્યા દ્વિગુણ type2;
    પ્રકારવ્યાખ્યા દ્વિગુણ rtype;
    દ્વિગુણ operator()(દ્વિગુણ a, દ્વિગુણ b) અચલ { પાછા std::abs(a - b); }
};

template<typename T> સંરચના OpAnd
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator()( T a, T b ) અચલ { પાછા a & b; }
};

template<typename T> સંરચના OpOr
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator()( T a, T b ) અચલ { પાછા a | b; }
};

template<typename T> સંરચના OpXor
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator()( T a, T b ) અચલ { પાછા a ^ b; }
};

template<typename T> સંરચના OpNot
{
    પ્રકારવ્યાખ્યા T type1;
    પ્રકારવ્યાખ્યા T type2;
    પ્રકારવ્યાખ્યા T rtype;
    T operator()( T a, T ) અચલ { પાછા ~a; }
};

template<> અંતઃરેખ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) અચલ
{ પાછા CV_FAST_CAST_8U(a + b); }

template<> અંતઃરેખ uchar OpSub<uchar>::operator ()(uchar a, uchar b) અચલ
{ પાછા CV_FAST_CAST_8U(a - b); }

template<> અંતઃરેખ short OpAbsDiff<short>::operator ()(short a, short b) અચલ
{ પાછા saturate_cast<short>(std::abs(a - b)); }

template<> અંતઃરેખ schar OpAbsDiff<schar>::operator ()(schar a, schar b) અચલ
{ પાછા saturate_cast<schar>(std::abs(a - b)); }

template<> અંતઃરેખ uchar OpMin<uchar>::operator ()(uchar a, uchar b) અચલ { પાછા CV_MIN_8U(a, b); }

template<> અંતઃરેખ uchar OpMax<uchar>::operator ()(uchar a, uchar b) અચલ { પાછા CV_MAX_8U(a, b); }

પ્રકારવ્યાખ્યા શૂન્ય (*UnaryFunc)(અચલ uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       શૂન્ય*);

પ્રકારવ્યાખ્યા શૂન્ય (*દ્વિઅંકી_Func)(અચલ uchar* src1, size_t step1,
                       અચલ uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       શૂન્ય*);

પ્રકારવ્યાખ્યા શૂન્ય (*દ્વિઅંકી_Func_C)(અચલ શૂન્ય* src1, size_t step1,
                       અચલ શૂન્ય* src2, size_t step2,
                       શૂન્ય* dst, size_t step, પૂર્ણાંક width, પૂર્ણાંક height,
                       શૂન્ય*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS દ્વિઅંકી_Func getConvertFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
CV_EXPORTS દ્વિઅંકી_Func getConvertScaleFunc(પૂર્ણાંક sdepth, પૂર્ણાંક ddepth);
દ્વિઅંકી_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, પૂર્ણાંક widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, પૂર્ણાંક widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, પૂર્ણાંક widthScale=1);

શૂન્ય setSize( Mat& m, પૂર્ણાંક _dims, અચલ પૂર્ણાંક* _sz, અચલ size_t* _steps, તાર્કિક autoSteps=ખોટું );
શૂન્ય finalizeHdr(Mat& m);
પૂર્ણાંક updateContinuityFlag(પૂર્ણાંક flags, પૂર્ણાંક dims, અચલ પૂર્ણાંક* size, અચલ size_t* step);

સંરચના NoVec
{
    size_t operator()(અચલ શૂન્ય*, અચલ શૂન્ય*, શૂન્ય*, size_t) અચલ { પાછા 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

ગણના { BLOCK_SIZE = 1024 };

#જો defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#નહીંતર
#define ARITHM_USE_IPP 0
#endif

અંતઃરેખ તાર્કિક checkScalar(અચલ Mat& sc, પૂર્ણાંક atype, આગત_Array_2::KindFlag sckind, આગત_Array_2::KindFlag akind)
{
    જો( sc.dims > 2 || !sc.isContinuous() )
        પાછા ખોટું;
    Size sz = sc.size();
    જો(sz.width != 1 && sz.height != 1)
        પાછા ખોટું;
    પૂર્ણાંક cn = CV_MAT_CN(atype);
    જો( akind == આગત_Array_2::MATX && sckind != આગત_Array_2::MATX )
        પાછા ખોટું;
    પાછા sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

અંતઃરેખ તાર્કિક checkScalar(આગત_Array sc, પૂર્ણાંક atype, આગત_Array_2::KindFlag sckind, આગત_Array_2::KindFlag akind)
{
    જો( sc.dims() > 2 || !sc.isContinuous() )
        પાછા ખોટું;
    Size sz = sc.size();
    જો(sz.width != 1 && sz.height != 1)
        પાછા ખોટું;
    પૂર્ણાંક cn = CV_MAT_CN(atype);
    જો( akind == આગત_Array_2::MATX && sckind != આગત_Array_2::MATX )
        પાછા ખોટું;
    પાછા sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
અંતઃરેખ તાર્કિક sameShapeAsArg(અચલ આગત_Array_2& sc, અચલ આગત_Array_2& arr)
{
    પાછા arr.channels() == 1 && sc.sameSize(arr);
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
અંતઃરેખ તાર્કિક isScalarArg(અચલ આગત_Array_2& sc, અચલ આગત_Array_2& arr)
{
    અચલ આગત_Array_2::KindFlag kind = sc.kind();
    અચલ પૂર્ણાંક cn = arr.channels();
    જો (kind == આગત_Array_2::MATX)
    {
        Size sz = sc.getSz();
        પૂર્ણાંક scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        જો (scn0 != sz.width + sz.height - 1)
            પાછા ખોટું;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        પાછા scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    જો (kind == આગત_Array_2::MAT)
    {
        અચલ Mat& m = *(અચલ Mat*)sc.getObj();
        પાછા m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    જો (kind == આગત_Array_2::UMAT)
    {
        અચલ UMat& m = *(અચલ UMat*)sc.getObj();
        પાછા m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    પાછા ખોટું;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
અંતઃરેખ તાર્કિક isScalarLikeMat(અચલ આગત_Array_2& sc, પૂર્ણાંક cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    પૂર્ણાંક rows, cols, nval;
    તાર્કિક continuous;
    અચલ આગત_Array_2::KindFlag kind = sc.kind();
    જો (kind == આગત_Array_2::MAT)
    {
        અચલ Mat& m = *(અચલ Mat*)sc.getObj();
        જો ((m.rows != 1 && m.cols != 1) || m.dims > 2) પાછા ખોટું;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    નહીંતર જો (kind == આગત_Array_2::UMAT)
    {
        અચલ UMat& m = *(અચલ UMat*)sc.getObj();
        જો ((m.rows != 1 && m.cols != 1) || m.dims > 2) પાછા ખોટું;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    નહીંતર
        પાછા ખોટું;
    જો (!continuous || nval > 4 || (nval != 1 && nval != cn))
        પાછા ખોટું;
    પાછા (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
ગણના { EW_SCALAR_BUF_SIZE = 4 * કદ(દ્વિગુણ) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
અંતઃરેખ પૂર્ણાંક scalarArgElems(અચલ આગત_Array_2& sc, અચલ uchar*& p, પૂર્ણાંક& d, uchar* scbuf)
{
    અચલ આગત_Array_2::KindFlag kind = sc.kind();
    જો (kind == આગત_Array_2::MAT)
    {
        અચલ Mat& m = *(અચલ Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        પાછા (પૂર્ણાંક)m.total() * m.channels();
    }
    જો (kind == આગત_Array_2::UMAT)
    {
        અચલ UMat& u = *(અચલ UMat*)sc.getObj();
        d = u.depth();
        પૂર્ણાંક n = (પૂર્ણાંક)u.total() * u.channels();
        CV_Assert(n * (પૂર્ણાંક)CV_ELEM_SIZE1(d) <= (પૂર્ણાંક)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        પાછા n;
    }
    p = (અચલ uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    પાછા sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
અંતઃરેખ દ્વિગુણ elemToDouble(પૂર્ણાંક d, અચલ uchar* p)
{
    પસંદગી (d)
    {
    સ્થિતિ CV_8U:   પાછા *(અચલ uchar*)p;
    સ્થિતિ CV_8S:   પાછા *(અચલ schar*)p;
    સ્થિતિ CV_16U:  પાછા *(અચલ ushort*)p;
    સ્થિતિ CV_16S:  પાછા *(અચલ short*)p;
    સ્થિતિ CV_32U:  પાછા *(અચલ unsigned*)p;
    સ્થિતિ CV_32S:  પાછા *(અચલ પૂર્ણાંક*)p;
    સ્થિતિ CV_64U:  પાછા (દ્વિગુણ)*(અચલ uint64_t*)p;
    સ્થિતિ CV_64S:  પાછા (દ્વિગુણ)*(અચલ int64_t*)p;
    સ્થિતિ CV_16F:  પાછા (દશાંશ)*(અચલ hfloat*)p;
    સ્થિતિ CV_16BF: પાછા (દશાંશ)*(અચલ bfloat*)p;
    સ્થિતિ CV_32F:  પાછા *(અચલ દશાંશ*)p;
    સ્થિતિ CV_64F:  પાછા *(અચલ દ્વિગુણ*)p;
    મૂળભૂત:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
અંતઃરેખ પૂર્ણાંક readScalarArg(અચલ આગત_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    અચલ uchar* p; પૂર્ણાંક d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    પૂર્ણાંક n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    માટે (પૂર્ણાંક i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    પાછા n;
}

શૂન્ય convertAndUnrollScalar( અચલ Mat& sc, પૂર્ણાંક buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
સંરચના ImplCollector
{
    ImplCollector()
    {
        useCollection   = ખોટું;
        implFlags       = 0;
    }
    તાર્કિક useCollection; // enable/disable impl data collection

    પૂર્ણાંક implFlags;
    std::vector<પૂર્ણાંક>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

સંરચના CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(ખોટું), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    તાર્કિક oclExecutionContextInitialized;
    પૂર્ણાંક useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    પૂર્ણાંક useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    પૂર્ણાંક useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#જો defined(BUILD_SHARED_LIBS)
#જો defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#અથવાજો defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#નહીંતર
#define CL_RUNTIME_EXPORT
#endif
#નહીંતર
#define CL_RUNTIME_EXPORT
#endif

બાહ્ય CV_EXPORTS
તાર્કિક __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_કિંમત) \
    સ્થિર TYPE* અચલ instance = INITIALIZER; \
    પાછા RET_કિંમત;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS શૂન્ય releaseTlsStorageThread();

પૂર્ણાંક cv_snprintf(અક્ષર* buf, પૂર્ણાંક len, અચલ અક્ષર* fmt, ...);
પૂર્ણાંક cv_vsnprintf(અક્ષર* buf, પૂર્ણાંક len, અચલ અક્ષર* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
