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
बाह्य स्थिरांक दशांश g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

बाह्य स्थिरांक ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

बाह्य स्थिरांक uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (प्रतिज्ञा(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpNop
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a) स्थिरांक { परत saturate_cast<T3>(a); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpSqr
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a) स्थिरांक { परत saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpAdd
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a, स्थिरांक T2 b) स्थिरांक { परत saturate_cast<T3>(a + b); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpAddSqr
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a, स्थिरांक T2 b) स्थिरांक { परत saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpSub
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a, स्थिरांक T2 b) स्थिरांक { परत saturate_cast<T3>(a - b); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> संरचना OpRSub
{
    प्रकारव्याख्या T1 type1;
    प्रकारव्याख्या T2 type2;
    प्रकारव्याख्या T3 rtype;
    T3 चालक ()(स्थिरांक T1 a, स्थिरांक T2 b) स्थिरांक { परत saturate_cast<T3>(b - a); }
};

ढाँचा<typename T> संरचना OpMin
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक ()(स्थिरांक T a, स्थिरांक T b) स्थिरांक { परत std::min(a, b); }
};

ढाँचा<typename T> संरचना OpMax
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक ()(स्थिरांक T a, स्थिरांक T b) स्थिरांक { परत std::max(a, b); }
};

ढाँचा<typename T> संरचना OpAbsDiff
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक()(T a, T b) स्थिरांक { परत a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
ढाँचा<> संरचना OpAbsDiff<दशांश>
{
    प्रकारव्याख्या दशांश type1;
    प्रकारव्याख्या दशांश type2;
    प्रकारव्याख्या दशांश rtype;
    दशांश चालक()(दशांश a, दशांश b) स्थिरांक { परत std::असल(a - b); }
};
ढाँचा<> संरचना OpAbsDiff<द्विगुण>
{
    प्रकारव्याख्या द्विगुण type1;
    प्रकारव्याख्या द्विगुण type2;
    प्रकारव्याख्या द्विगुण rtype;
    द्विगुण चालक()(द्विगुण a, द्विगुण b) स्थिरांक { परत std::असल(a - b); }
};

ढाँचा<typename T> संरचना OpAnd
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक()( T a, T b ) स्थिरांक { परत a & b; }
};

ढाँचा<typename T> संरचना OpOr
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक()( T a, T b ) स्थिरांक { परत a | b; }
};

ढाँचा<typename T> संरचना OpXor
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक()( T a, T b ) स्थिरांक { परत a ^ b; }
};

ढाँचा<typename T> संरचना OpNot
{
    प्रकारव्याख्या T type1;
    प्रकारव्याख्या T type2;
    प्रकारव्याख्या T rtype;
    T चालक()( T a, T ) स्थिरांक { परत ~a; }
};

ढाँचा<> ओळीत uchar OpAdd<uchar>::चालक ()(uchar a, uchar b) स्थिरांक
{ परत CV_FAST_CAST_8U(a + b); }

ढाँचा<> ओळीत uchar OpSub<uchar>::चालक ()(uchar a, uchar b) स्थिरांक
{ परत CV_FAST_CAST_8U(a - b); }

ढाँचा<> ओळीत लघु OpAbsDiff<लघु>::चालक ()(लघु a, लघु b) स्थिरांक
{ परत saturate_cast<लघु>(std::असल(a - b)); }

ढाँचा<> ओळीत schar OpAbsDiff<schar>::चालक ()(schar a, schar b) स्थिरांक
{ परत saturate_cast<schar>(std::असल(a - b)); }

ढाँचा<> ओळीत uchar OpMin<uchar>::चालक ()(uchar a, uchar b) स्थिरांक { परत CV_MIN_8U(a, b); }

ढाँचा<> ओळीत uchar OpMax<uchar>::चालक ()(uchar a, uchar b) स्थिरांक { परत CV_MAX_8U(a, b); }

प्रकारव्याख्या शून्य (*UnaryFunc)(स्थिरांक uchar* src1, माप_प्रकार step1,
                       uchar* dst, माप_प्रकार step, Size sz,
                       शून्य*);

प्रकारव्याख्या शून्य (*द्विमान_Func)(स्थिरांक uchar* src1, माप_प्रकार step1,
                       स्थिरांक uchar* src2, माप_प्रकार step2,
                       uchar* dst, माप_प्रकार step, Size sz,
                       शून्य*);

प्रकारव्याख्या शून्य (*द्विमान_Func_C)(स्थिरांक शून्य* src1, माप_प्रकार step1,
                       स्थिरांक शून्य* src2, माप_प्रकार step2,
                       शून्य* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height,
                       शून्य*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS द्विमान_Func getConvertFunc(पूर्णांक sdepth, पूर्णांक ddepth);
CV_EXPORTS द्विमान_Func getConvertScaleFunc(पूर्णांक sdepth, पूर्णांक ddepth);
द्विमान_Func getCopyMaskFunc(माप_प्रकार esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, पूर्णांक widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, पूर्णांक widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णांक widthScale=1);

शून्य setSize( Mat& m, पूर्णांक _dims, स्थिरांक पूर्णांक* _sz, स्थिरांक माप_प्रकार* _steps, तार्किक autoSteps=असत्य );
शून्य finalizeHdr(Mat& m);
पूर्णांक updateContinuityFlag(पूर्णांक flags, पूर्णांक dims, स्थिरांक पूर्णांक* size, स्थिरांक माप_प्रकार* step);

संरचना NoVec
{
    माप_प्रकार चालक()(स्थिरांक शून्य*, स्थिरांक शून्य*, शून्य*, माप_प्रकार) स्थिरांक { परत 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((पूर्णांक_उच्च/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

गणना { BLOCK_SIZE = 1024 };

#जर defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#अन्यथा
#define ARITHM_USE_IPP 0
#endif

ओळीत तार्किक checkScalar(स्थिरांक Mat& sc, पूर्णांक atype, आदान_Array_2::KindFlag sckind, आदान_Array_2::KindFlag akind)
{
    जर( sc.dims > 2 || !sc.isContinuous() )
        परत असत्य;
    Size sz = sc.size();
    जर(sz.width != 1 && sz.height != 1)
        परत असत्य;
    पूर्णांक cn = CV_MAT_CN(atype);
    जर( akind == आदान_Array_2::MATX && sckind != आदान_Array_2::MATX )
        परत असत्य;
    परत sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

ओळीत तार्किक checkScalar(आदान_Array sc, पूर्णांक atype, आदान_Array_2::KindFlag sckind, आदान_Array_2::KindFlag akind)
{
    जर( sc.dims() > 2 || !sc.isContinuous() )
        परत असत्य;
    Size sz = sc.size();
    जर(sz.width != 1 && sz.height != 1)
        परत असत्य;
    पूर्णांक cn = CV_MAT_CN(atype);
    जर( akind == आदान_Array_2::MATX && sckind != आदान_Array_2::MATX )
        परत असत्य;
    परत sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
ओळीत तार्किक sameShapeAsArg(स्थिरांक आदान_Array_2& sc, स्थिरांक आदान_Array_2& arr)
{
    परत arr.channels() == 1 && sc.sameSize(arr);
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
ओळीत तार्किक isScalarArg(स्थिरांक आदान_Array_2& sc, स्थिरांक आदान_Array_2& arr)
{
    स्थिरांक आदान_Array_2::KindFlag kind = sc.kind();
    स्थिरांक पूर्णांक cn = arr.channels();
    जर (kind == आदान_Array_2::MATX)
    {
        Size sz = sc.getSz();
        पूर्णांक scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        जर (scn0 != sz.width + sz.height - 1)
            परत असत्य;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        परत scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    जर (kind == आदान_Array_2::MAT)
    {
        स्थिरांक Mat& m = *(स्थिरांक Mat*)sc.getObj();
        परत m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    जर (kind == आदान_Array_2::UMAT)
    {
        स्थिरांक UMat& m = *(स्थिरांक UMat*)sc.getObj();
        परत m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    परत असत्य;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
ओळीत तार्किक isScalarLikeMat(स्थिरांक आदान_Array_2& sc, पूर्णांक cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    पूर्णांक rows, cols, nval;
    तार्किक continuous;
    स्थिरांक आदान_Array_2::KindFlag kind = sc.kind();
    जर (kind == आदान_Array_2::MAT)
    {
        स्थिरांक Mat& m = *(स्थिरांक Mat*)sc.getObj();
        जर ((m.rows != 1 && m.cols != 1) || m.dims > 2) परत असत्य;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा जर (kind == आदान_Array_2::UMAT)
    {
        स्थिरांक UMat& m = *(स्थिरांक UMat*)sc.getObj();
        जर ((m.rows != 1 && m.cols != 1) || m.dims > 2) परत असत्य;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा
        परत असत्य;
    जर (!continuous || nval > 4 || (nval != 1 && nval != cn))
        परत असत्य;
    परत (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
गणना { EW_SCALAR_BUF_SIZE = 4 * आकार(द्विगुण) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
ओळीत पूर्णांक scalarArgElems(स्थिरांक आदान_Array_2& sc, स्थिरांक uchar*& p, पूर्णांक& d, uchar* scbuf)
{
    स्थिरांक आदान_Array_2::KindFlag kind = sc.kind();
    जर (kind == आदान_Array_2::MAT)
    {
        स्थिरांक Mat& m = *(स्थिरांक Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        परत (पूर्णांक)m.total() * m.channels();
    }
    जर (kind == आदान_Array_2::UMAT)
    {
        स्थिरांक UMat& u = *(स्थिरांक UMat*)sc.getObj();
        d = u.depth();
        पूर्णांक n = (पूर्णांक)u.total() * u.channels();
        CV_Assert(n * (पूर्णांक)CV_ELEM_SIZE1(d) <= (पूर्णांक)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        परत n;
    }
    p = (स्थिरांक uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    परत sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
ओळीत द्विगुण elemToDouble(पूर्णांक d, स्थिरांक uchar* p)
{
    निवड (d)
    {
    स्थिती CV_8U:   परत *(स्थिरांक uchar*)p;
    स्थिती CV_8S:   परत *(स्थिरांक schar*)p;
    स्थिती CV_16U:  परत *(स्थिरांक ushort*)p;
    स्थिती CV_16S:  परत *(स्थिरांक लघु*)p;
    स्थिती CV_32U:  परत *(स्थिरांक अचिन्हित*)p;
    स्थिती CV_32S:  परत *(स्थिरांक पूर्णांक*)p;
    स्थिती CV_64U:  परत (द्विगुण)*(स्थिरांक uint64_t*)p;
    स्थिती CV_64S:  परत (द्विगुण)*(स्थिरांक int64_t*)p;
    स्थिती CV_16F:  परत (दशांश)*(स्थिरांक hfloat*)p;
    स्थिती CV_16BF: परत (दशांश)*(स्थिरांक bfloat*)p;
    स्थिती CV_32F:  परत *(स्थिरांक दशांश*)p;
    स्थिती CV_64F:  परत *(स्थिरांक द्विगुण*)p;
    पूर्वनिर्धारित:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
ओळीत पूर्णांक readScalarArg(स्थिरांक आदान_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    स्थिरांक uchar* p; पूर्णांक d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    पूर्णांक n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    माप_प्रकार esz = CV_ELEM_SIZE1(d);
    साठी (पूर्णांक i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (माप_प्रकार)i * esz);
    परत n;
}

शून्य convertAndUnrollScalar( स्थिरांक Mat& sc, पूर्णांक buftype, uchar* scbuf, माप_प्रकार blocksize );

#ifdef CV_COLLECT_IMPL_DATA
संरचना ImplCollector
{
    ImplCollector()
    {
        useCollection   = असत्य;
        implFlags       = 0;
    }
    तार्किक useCollection; // enable/disable impl data collection

    पूर्णांक implFlags;
    std::vector<पूर्णांक>    implCode;
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
    पूर्णांक useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    पूर्णांक useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    पूर्णांक useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#जर defined(BUILD_SHARED_LIBS)
#जर defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#किंवाजर defined __GNUC__
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
    स्थिर TYPE* स्थिरांक instance = INITIALIZER; \
    परत RET_मूल्य;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS शून्य releaseTlsStorageThread();

पूर्णांक cv_snprintf(अक्षर* buf, पूर्णांक len, स्थिरांक अक्षर* fmt, ...);
पूर्णांक cv_vsnprintf(अक्षर* buf, पूर्णांक len, स्थिरांक अक्षर* fmt, बहु_सूची args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
