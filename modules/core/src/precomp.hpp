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
#अन्तर्भाव "opencv2/core/utility.hpp"
#अन्यथा  // BUILD_PLUGIN

#अन्तर्भाव "opencv2/opencv_modules.hpp"
#अन्तर्भाव "cvconfig.h"

#अन्तर्भाव "opencv2/core/utility.hpp"
#अन्तर्भाव "opencv2/core/cuda.hpp"
#अन्तर्भाव "opencv2/core/opengl.hpp"
#अन्तर्भाव "opencv2/core/va_intel.hpp"

#अन्तर्भाव "opencv2/core/private.hpp"
#अन्तर्भाव "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#अन्तर्भाव "opencv2/core/ocl.hpp"
#endif

#अन्तर्भाव <ctype.h>
#अन्तर्भाव <float.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <math.h>
#अन्तर्भाव <stdarg.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

#अन्तर्भाव <algorithm>
#अन्तर्भाव <cmath>
#अन्तर्भाव <cstdlib>
#अन्तर्भाव <limits>
#अन्तर्भाव <float.h>
#अन्तर्भाव <cstring>
#अन्तर्भाव <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#अन्तर्भाव "opencv2/core/hal/hal.hpp"
#अन्तर्भाव "opencv2/core/hal/intrin.hpp"
#अन्तर्भाव "opencv2/core/neon_utils.hpp"
#अन्तर्भाव "opencv2/core/vsx_utils.hpp"
#अन्तर्भाव "hal_replacement.hpp"


नामाकाश cv
{

// -128.f ... 255.f
बाह्य ध्रुव भिन्न g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

बाह्य ध्रुव ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

बाह्य ध्रुव uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (प्रतिज्ञा(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpNop
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a) ध्रुव { प्रत्यागम saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpSqr
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a) ध्रुव { प्रत्यागम saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpAdd
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a, ध्रुव T2 b) ध्रुव { प्रत्यागम saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpAddSqr
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a, ध्रुव T2 b) ध्रुव { प्रत्यागम saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpSub
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a, ध्रुव T2 b) ध्रुव { प्रत्यागम saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> संरचना OpRSub
{
    प्रकारनिर्देश T1 type1;
    प्रकारनिर्देश T2 type2;
    प्रकारनिर्देश T3 rtype;
    T3 operator ()(ध्रुव T1 a, ध्रुव T2 b) ध्रुव { प्रत्यागम saturate_cast<T3>(b - a); }
};

template<typename T> संरचना OpMin
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator ()(ध्रुव T a, ध्रुव T b) ध्रुव { प्रत्यागम std::min(a, b); }
};

template<typename T> संरचना OpMax
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator ()(ध्रुव T a, ध्रुव T b) ध्रुव { प्रत्यागम std::max(a, b); }
};

template<typename T> संरचना OpAbsDiff
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator()(T a, T b) ध्रुव { प्रत्यागम a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> संरचना OpAbsDiff<भिन्न>
{
    प्रकारनिर्देश भिन्न type1;
    प्रकारनिर्देश भिन्न type2;
    प्रकारनिर्देश भिन्न rtype;
    भिन्न operator()(भिन्न a, भिन्न b) ध्रुव { प्रत्यागम std::abs(a - b); }
};
template<> संरचना OpAbsDiff<द्विगुण>
{
    प्रकारनिर्देश द्विगुण type1;
    प्रकारनिर्देश द्विगुण type2;
    प्रकारनिर्देश द्विगुण rtype;
    द्विगुण operator()(द्विगुण a, द्विगुण b) ध्रुव { प्रत्यागम std::abs(a - b); }
};

template<typename T> संरचना OpAnd
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator()( T a, T b ) ध्रुव { प्रत्यागम a & b; }
};

template<typename T> संरचना OpOr
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator()( T a, T b ) ध्रुव { प्रत्यागम a | b; }
};

template<typename T> संरचना OpXor
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator()( T a, T b ) ध्रुव { प्रत्यागम a ^ b; }
};

template<typename T> संरचना OpNot
{
    प्रकारनिर्देश T type1;
    प्रकारनिर्देश T type2;
    प्रकारनिर्देश T rtype;
    T operator()( T a, T ) ध्रुव { प्रत्यागम ~a; }
};

template<> अन्तःस्थ uchar OpAdd<uchar>::operator ()(uchar a, uchar b) ध्रुव
{ प्रत्यागम CV_FAST_CAST_8U(a + b); }

template<> अन्तःस्थ uchar OpSub<uchar>::operator ()(uchar a, uchar b) ध्रुव
{ प्रत्यागम CV_FAST_CAST_8U(a - b); }

template<> अन्तःस्थ short OpAbsDiff<short>::operator ()(short a, short b) ध्रुव
{ प्रत्यागम saturate_cast<short>(std::abs(a - b)); }

template<> अन्तःस्थ schar OpAbsDiff<schar>::operator ()(schar a, schar b) ध्रुव
{ प्रत्यागम saturate_cast<schar>(std::abs(a - b)); }

template<> अन्तःस्थ uchar OpMin<uchar>::operator ()(uchar a, uchar b) ध्रुव { प्रत्यागम CV_MIN_8U(a, b); }

template<> अन्तःस्थ uchar OpMax<uchar>::operator ()(uchar a, uchar b) ध्रुव { प्रत्यागम CV_MAX_8U(a, b); }

प्रकारनिर्देश शून्य (*UnaryFunc)(ध्रुव uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       शून्य*);

प्रकारनिर्देश शून्य (*द्विमान_Func)(ध्रुव uchar* src1, size_t step1,
                       ध्रुव uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       शून्य*);

प्रकारनिर्देश शून्य (*द्विमान_Func_C)(ध्रुव शून्य* src1, size_t step1,
                       ध्रुव शून्य* src2, size_t step2,
                       शून्य* dst, size_t step, पूर्णाङ्क width, पूर्णाङ्क height,
                       शून्य*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS द्विमान_Func getConvertFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
CV_EXPORTS द्विमान_Func getConvertScaleFunc(पूर्णाङ्क sdepth, पूर्णाङ्क ddepth);
द्विमान_Func getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, पूर्णाङ्क widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, पूर्णाङ्क widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णाङ्क widthScale=1);

शून्य setSize( Mat& m, पूर्णाङ्क _dims, ध्रुव पूर्णाङ्क* _sz, ध्रुव size_t* _steps, तार्किक autoSteps=असत्य );
शून्य finalizeHdr(Mat& m);
पूर्णाङ्क updateContinuityFlag(पूर्णाङ्क flags, पूर्णाङ्क dims, ध्रुव पूर्णाङ्क* size, ध्रुव size_t* step);

संरचना NoVec
{
    size_t operator()(ध्रुव शून्य*, ध्रुव शून्य*, शून्य*, size_t) ध्रुव { प्रत्यागम 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

गणना { BLOCK_SIZE = 1024 };

#यदि defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#अन्यथा
#define ARITHM_USE_IPP 0
#endif

अन्तःस्थ तार्किक checkScalar(ध्रुव Mat& sc, पूर्णाङ्क atype, आदान_Array_2::KindFlag sckind, आदान_Array_2::KindFlag akind)
{
    यदि( sc.dims > 2 || !sc.isContinuous() )
        प्रत्यागम असत्य;
    Size sz = sc.size();
    यदि(sz.width != 1 && sz.height != 1)
        प्रत्यागम असत्य;
    पूर्णाङ्क cn = CV_MAT_CN(atype);
    यदि( akind == आदान_Array_2::MATX && sckind != आदान_Array_2::MATX )
        प्रत्यागम असत्य;
    प्रत्यागम sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

अन्तःस्थ तार्किक checkScalar(आदान_Array sc, पूर्णाङ्क atype, आदान_Array_2::KindFlag sckind, आदान_Array_2::KindFlag akind)
{
    यदि( sc.dims() > 2 || !sc.isContinuous() )
        प्रत्यागम असत्य;
    Size sz = sc.size();
    यदि(sz.width != 1 && sz.height != 1)
        प्रत्यागम असत्य;
    पूर्णाङ्क cn = CV_MAT_CN(atype);
    यदि( akind == आदान_Array_2::MATX && sckind != आदान_Array_2::MATX )
        प्रत्यागम असत्य;
    प्रत्यागम sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
अन्तःस्थ तार्किक sameShapeAsArg(ध्रुव आदान_Array_2& sc, ध्रुव आदान_Array_2& arr)
{
    प्रत्यागम arr.channels() == 1 && sc.sameSize(arr);
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
अन्तःस्थ तार्किक isScalarArg(ध्रुव आदान_Array_2& sc, ध्रुव आदान_Array_2& arr)
{
    ध्रुव आदान_Array_2::KindFlag kind = sc.kind();
    ध्रुव पूर्णाङ्क cn = arr.channels();
    यदि (kind == आदान_Array_2::MATX)
    {
        Size sz = sc.getSz();
        पूर्णाङ्क scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        यदि (scn0 != sz.width + sz.height - 1)
            प्रत्यागम असत्य;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        प्रत्यागम scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    यदि (kind == आदान_Array_2::MAT)
    {
        ध्रुव Mat& m = *(ध्रुव Mat*)sc.getObj();
        प्रत्यागम m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    यदि (kind == आदान_Array_2::UMAT)
    {
        ध्रुव UMat& m = *(ध्रुव UMat*)sc.getObj();
        प्रत्यागम m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    प्रत्यागम असत्य;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
अन्तःस्थ तार्किक isScalarLikeMat(ध्रुव आदान_Array_2& sc, पूर्णाङ्क cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    पूर्णाङ्क rows, cols, nval;
    तार्किक continuous;
    ध्रुव आदान_Array_2::KindFlag kind = sc.kind();
    यदि (kind == आदान_Array_2::MAT)
    {
        ध्रुव Mat& m = *(ध्रुव Mat*)sc.getObj();
        यदि ((m.rows != 1 && m.cols != 1) || m.dims > 2) प्रत्यागम असत्य;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा यदि (kind == आदान_Array_2::UMAT)
    {
        ध्रुव UMat& m = *(ध्रुव UMat*)sc.getObj();
        यदि ((m.rows != 1 && m.cols != 1) || m.dims > 2) प्रत्यागम असत्य;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा
        प्रत्यागम असत्य;
    यदि (!continuous || nval > 4 || (nval != 1 && nval != cn))
        प्रत्यागम असत्य;
    प्रत्यागम (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
गणना { EW_SCALAR_BUF_SIZE = 4 * परिमाण(द्विगुण) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
अन्तःस्थ पूर्णाङ्क scalarArgElems(ध्रुव आदान_Array_2& sc, ध्रुव uchar*& p, पूर्णाङ्क& d, uchar* scbuf)
{
    ध्रुव आदान_Array_2::KindFlag kind = sc.kind();
    यदि (kind == आदान_Array_2::MAT)
    {
        ध्रुव Mat& m = *(ध्रुव Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        प्रत्यागम (पूर्णाङ्क)m.total() * m.channels();
    }
    यदि (kind == आदान_Array_2::UMAT)
    {
        ध्रुव UMat& u = *(ध्रुव UMat*)sc.getObj();
        d = u.depth();
        पूर्णाङ्क n = (पूर्णाङ्क)u.total() * u.channels();
        CV_Assert(n * (पूर्णाङ्क)CV_ELEM_SIZE1(d) <= (पूर्णाङ्क)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        प्रत्यागम n;
    }
    p = (ध्रुव uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    प्रत्यागम sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
अन्तःस्थ द्विगुण elemToDouble(पूर्णाङ्क d, ध्रुव uchar* p)
{
    विकल्प (d)
    {
    पक्ष CV_8U:   प्रत्यागम *(ध्रुव uchar*)p;
    पक्ष CV_8S:   प्रत्यागम *(ध्रुव schar*)p;
    पक्ष CV_16U:  प्रत्यागम *(ध्रुव ushort*)p;
    पक्ष CV_16S:  प्रत्यागम *(ध्रुव short*)p;
    पक्ष CV_32U:  प्रत्यागम *(ध्रुव unsigned*)p;
    पक्ष CV_32S:  प्रत्यागम *(ध्रुव पूर्णाङ्क*)p;
    पक्ष CV_64U:  प्रत्यागम (द्विगुण)*(ध्रुव uint64_t*)p;
    पक्ष CV_64S:  प्रत्यागम (द्विगुण)*(ध्रुव int64_t*)p;
    पक्ष CV_16F:  प्रत्यागम (भिन्न)*(ध्रुव hfloat*)p;
    पक्ष CV_16BF: प्रत्यागम (भिन्न)*(ध्रुव bfloat*)p;
    पक्ष CV_32F:  प्रत्यागम *(ध्रुव भिन्न*)p;
    पक्ष CV_64F:  प्रत्यागम *(ध्रुव द्विगुण*)p;
    स्वभाव:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
अन्तःस्थ पूर्णाङ्क readScalarArg(ध्रुव आदान_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    ध्रुव uchar* p; पूर्णाङ्क d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    पूर्णाङ्क n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    प्रति (पूर्णाङ्क i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    प्रत्यागम n;
}

शून्य convertAndUnrollScalar( ध्रुव Mat& sc, पूर्णाङ्क buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
संरचना ImplCollector
{
    ImplCollector()
    {
        useCollection   = असत्य;
        implFlags       = 0;
    }
    तार्किक useCollection; // enable/disable impl data collection

    पूर्णाङ्क implFlags;
    std::vector<पूर्णाङ्क>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

संरचना CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(असत्य), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    तार्किक oclExecutionContextInitialized;
    पूर्णाङ्क useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    पूर्णाङ्क useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    पूर्णाङ्क useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#यदि defined(BUILD_SHARED_LIBS)
#यदि defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#नोचेद्यदि defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#अन्यथा
#define CL_RUNTIME_EXPORT
#endif
#अन्यथा
#define CL_RUNTIME_EXPORT
#endif

बाह्य CV_EXPORTS
तार्किक __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_मूल्य) \
    स्थिर TYPE* ध्रुव instance = INITIALIZER; \
    प्रत्यागम RET_मूल्य;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS शून्य releaseTlsStorageThread();

पूर्णाङ्क cv_snprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, ...);
पूर्णाङ्क cv_vsnprintf(वर्ण* buf, पूर्णाङ्क len, ध्रुव वर्ण* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
