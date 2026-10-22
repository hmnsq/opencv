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

#अगर_अघोषित __OPENCV_PRECOMP_H__
#घोषणा __OPENCV_PRECOMP_H__

#अगर_घोषित _MSC_VER
#  define _SILENCE_CXX17_C_HEADER_DEPRECATION_WARNING
#  define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#पूर्ण_अगर

#अगर_घोषित BUILD_PLUGIN
#समावेश "opencv2/core/utility.hpp"
#अन्यथा  // BUILD_PLUGIN

#समावेश "opencv2/opencv_modules.hpp"
#समावेश "cvconfig.h"

#समावेश "opencv2/core/utility.hpp"
#समावेश "opencv2/core/cuda.hpp"
#समावेश "opencv2/core/opengl.hpp"
#समावेश "opencv2/core/va_intel.hpp"

#समावेश "opencv2/core/private.hpp"
#समावेश "opencv2/core/private.cuda.hpp"
#अगर_घोषित HAVE_OPENCL
#समावेश "opencv2/core/ocl.hpp"
#पूर्ण_अगर

#समावेश <ctype.h>
#समावेश <float.h>
#समावेश <limits.h>
#समावेश <math.h>
#समावेश <stdarg.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

#समावेश <algorithm>
#समावेश <cmath>
#समावेश <cstdlib>
#समावेश <limits>
#समावेश <float.h>
#समावेश <cstring>
#समावेश <cassert>

#घोषणा USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#घोषणा USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#घोषणा USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#घोषणा USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#समावेश "opencv2/core/hal/hal.hpp"
#समावेश "opencv2/core/hal/intrin.hpp"
#समावेश "opencv2/core/neon_utils.hpp"
#समावेश "opencv2/core/vsx_utils.hpp"
#समावेश "hal_replacement.hpp"


namespace cv
{

// -128.f ... 255.f
बाह्य स्थिर भग्न g_8x32fTab[];
#घोषणा CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

बाह्य स्थिर ushort g_8x16uSqrTab[];
#घोषणा CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

बाह्य स्थिर uchar g_Saturate8u[];
#घोषणा CV_FAST_CAST_8U(t)   (निश्चित(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#घोषणा CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#घोषणा CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpNop
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a) स्थिर { वापस saturate_cast<T3>(a); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpSqr
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a) स्थिर { वापस saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpAdd
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a, स्थिर T2 b) स्थिर { वापस saturate_cast<T3>(a + b); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpAddSqr
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a, स्थिर T2 b) स्थिर { वापस saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpSub
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a, स्थिर T2 b) स्थिर { वापस saturate_cast<T3>(a - b); }
};

ढाँचा<typename T1, typename T2=T1, typename T3=T1> काष्ठा OpRSub
{
    प्रकार T1 type1;
    प्रकार T2 type2;
    प्रकार T3 rtype;
    T3 चालक ()(स्थिर T1 a, स्थिर T2 b) स्थिर { वापस saturate_cast<T3>(b - a); }
};

ढाँचा<typename T> काष्ठा OpMin
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक ()(स्थिर T a, स्थिर T b) स्थिर { वापस std::min(a, b); }
};

ढाँचा<typename T> काष्ठा OpMax
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक ()(स्थिर T a, स्थिर T b) स्थिर { वापस std::max(a, b); }
};

ढाँचा<typename T> काष्ठा OpAbsDiff
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक()(T a, T b) स्थिर { वापस a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
ढाँचा<> काष्ठा OpAbsDiff<भग्न>
{
    प्रकार भग्न type1;
    प्रकार भग्न type2;
    प्रकार भग्न rtype;
    भग्न चालक()(भग्न a, भग्न b) स्थिर { वापस std::असल(a - b); }
};
ढाँचा<> काष्ठा OpAbsDiff<द्विगुन>
{
    प्रकार द्विगुन type1;
    प्रकार द्विगुन type2;
    प्रकार द्विगुन rtype;
    द्विगुन चालक()(द्विगुन a, द्विगुन b) स्थिर { वापस std::असल(a - b); }
};

ढाँचा<typename T> काष्ठा OpAnd
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक()( T a, T b ) स्थिर { वापस a & b; }
};

ढाँचा<typename T> काष्ठा OpOr
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक()( T a, T b ) स्थिर { वापस a | b; }
};

ढाँचा<typename T> काष्ठा OpXor
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक()( T a, T b ) स्थिर { वापस a ^ b; }
};

ढाँचा<typename T> काष्ठा OpNot
{
    प्रकार T type1;
    प्रकार T type2;
    प्रकार T rtype;
    T चालक()( T a, T ) स्थिर { वापस ~a; }
};

ढाँचा<> अंतरभूत uchar OpAdd<uchar>::चालक ()(uchar a, uchar b) स्थिर
{ वापस CV_FAST_CAST_8U(a + b); }

ढाँचा<> अंतरभूत uchar OpSub<uchar>::चालक ()(uchar a, uchar b) स्थिर
{ वापस CV_FAST_CAST_8U(a - b); }

ढाँचा<> अंतरभूत लघु OpAbsDiff<लघु>::चालक ()(लघु a, लघु b) स्थिर
{ वापस saturate_cast<लघु>(std::असल(a - b)); }

ढाँचा<> अंतरभूत schar OpAbsDiff<schar>::चालक ()(schar a, schar b) स्थिर
{ वापस saturate_cast<schar>(std::असल(a - b)); }

ढाँचा<> अंतरभूत uchar OpMin<uchar>::चालक ()(uchar a, uchar b) स्थिर { वापस CV_MIN_8U(a, b); }

ढाँचा<> अंतरभूत uchar OpMax<uchar>::चालक ()(uchar a, uchar b) स्थिर { वापस CV_MAX_8U(a, b); }

प्रकार व्योम (*UnaryFunc)(स्थिर uchar* src1, माप_प्रकार step1,
                       uchar* dst, माप_प्रकार step, आकार sz,
                       व्योम*);

प्रकार व्योम (*द्विआधारी_Func)(स्थिर uchar* src1, माप_प्रकार step1,
                       स्थिर uchar* src2, माप_प्रकार step2,
                       uchar* dst, माप_प्रकार step, आकार sz,
                       व्योम*);

प्रकार व्योम (*द्विआधारी_Func_C)(स्थिर व्योम* src1, माप_प्रकार step1,
                       स्थिर व्योम* src2, माप_प्रकार step2,
                       व्योम* dst, माप_प्रकार step, पूर्णांक width, पूर्णांक height,
                       व्योम*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS द्विआधारी_Func getConvertFunc(पूर्णांक sdepth, पूर्णांक ddepth);
CV_EXPORTS द्विआधारी_Func getConvertScaleFunc(पूर्णांक sdepth, पूर्णांक ddepth);
द्विआधारी_Func getCopyMaskFunc(माप_प्रकार esz);

/* default memory block for sparse array elements */
#घोषणा  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#घोषणा  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#घोषणा  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
आकार getContinuousSize2D(Mat& m1, पूर्णांक widthScale=1);
आकार getContinuousSize2D(Mat& m1, Mat& m2, पूर्णांक widthScale=1);
आकार getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, पूर्णांक widthScale=1);

व्योम set_आकार( Mat& m, पूर्णांक _dims, स्थिर पूर्णांक* _sz, स्थिर माप_प्रकार* _steps, bool autoSteps=false );
व्योम finalizeHdr(Mat& m);
पूर्णांक updateContinuityFlag(पूर्णांक flags, पूर्णांक dims, स्थिर पूर्णांक* size, स्थिर माप_प्रकार* step);

काष्ठा NoVec
{
    माप_प्रकार चालक()(स्थिर व्योम*, स्थिर व्योम*, व्योम*, माप_प्रकार) स्थिर { वापस 0; }
};

#घोषणा CV_SPLIT_MERGE_MAX_BLOCK_आकार(cn) ((पूर्णांक_उच्च/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

क्रमागत { BLOCK_आकार = 1024 };

#अगर defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#घोषणा ARITHM_USE_IPP 1
#अन्यथा
#घोषणा ARITHM_USE_IPP 0
#पूर्ण_अगर

अंतरभूत bool checkScalar(स्थिर Mat& sc, पूर्णांक atype, निवेश_Array_2::KindFlag sckind, निवेश_Array_2::KindFlag akind)
{
    अगर( sc.dims > 2 || !sc.isContinuous() )
        वापस false;
    आकार sz = sc.size();
    अगर(sz.width != 1 && sz.height != 1)
        वापस false;
    पूर्णांक cn = CV_MAT_CN(atype);
    अगर( akind == निवेश_Array_2::MATX && sckind != निवेश_Array_2::MATX )
        वापस false;
    वापस sz == आकार(1, 1) || sz == आकार(1, cn) || sz == आकार(cn, 1) ||
           (sz == आकार(1, 4) && sc.type() == CV_64F && cn <= 4);
}

अंतरभूत bool checkScalar(निवेश_Array sc, पूर्णांक atype, निवेश_Array_2::KindFlag sckind, निवेश_Array_2::KindFlag akind)
{
    अगर( sc.dims() > 2 || !sc.isContinuous() )
        वापस false;
    आकार sz = sc.size();
    अगर(sz.width != 1 && sz.height != 1)
        वापस false;
    पूर्णांक cn = CV_MAT_CN(atype);
    अगर( akind == निवेश_Array_2::MATX && sckind != निवेश_Array_2::MATX )
        वापस false;
    वापस sz == आकार(1, 1) || sz == आकार(1, cn) || sz == आकार(cn, 1) ||
           (sz == आकार(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
अंतरभूत bool sameShapeAsArg(स्थिर निवेश_Array_2& sc, स्थिर निवेश_Array_2& arr)
{
    वापस arr.channels() == 1 && sc.same_आकार(arr);
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
अंतरभूत bool isScalarArg(स्थिर निवेश_Array_2& sc, स्थिर निवेश_Array_2& arr)
{
    स्थिर निवेश_Array_2::KindFlag kind = sc.kind();
    स्थिर पूर्णांक cn = arr.channels();
    अगर (kind == निवेश_Array_2::MATX)
    {
        आकार sz = sc.getSz();
        पूर्णांक scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        अगर (scn0 != sz.width + sz.height - 1)
            वापस false;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        वापस scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    अगर (kind == निवेश_Array_2::MAT)
    {
        स्थिर Mat& m = *(स्थिर Mat*)sc.getObj();
        वापस m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    अगर (kind == निवेश_Array_2::UMAT)
    {
        स्थिर UMat& m = *(स्थिर UMat*)sc.getObj();
        वापस m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    वापस false;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
अंतरभूत bool isScalarLikeMat(स्थिर निवेश_Array_2& sc, पूर्णांक cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    पूर्णांक rows, cols, nval;
    bool continuous;
    स्थिर निवेश_Array_2::KindFlag kind = sc.kind();
    अगर (kind == निवेश_Array_2::MAT)
    {
        स्थिर Mat& m = *(स्थिर Mat*)sc.getObj();
        अगर ((m.rows != 1 && m.cols != 1) || m.dims > 2) वापस false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा अगर (kind == निवेश_Array_2::UMAT)
    {
        स्थिर UMat& m = *(स्थिर UMat*)sc.getObj();
        अगर ((m.rows != 1 && m.cols != 1) || m.dims > 2) वापस false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    अन्यथा
        वापस false;
    अगर (!continuous || nval > 4 || (nval != 1 && nval != cn))
        वापस false;
    वापस (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
क्रमागत { EW_SCALAR_BUF_आकार = 4 * माप(द्विगुन) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
अंतरभूत पूर्णांक scalarArgElems(स्थिर निवेश_Array_2& sc, स्थिर uchar*& p, पूर्णांक& d, uchar* scbuf)
{
    स्थिर निवेश_Array_2::KindFlag kind = sc.kind();
    अगर (kind == निवेश_Array_2::MAT)
    {
        स्थिर Mat& m = *(स्थिर Mat*)sc.getObj();
        p = m.तथ्य;
        d = m.depth();
        वापस (पूर्णांक)m.योग() * m.channels();
    }
    अगर (kind == निवेश_Array_2::UMAT)
    {
        स्थिर UMat& u = *(स्थिर UMat*)sc.getObj();
        d = u.depth();
        पूर्णांक n = (पूर्णांक)u.योग() * u.channels();
        CV_Assert(n * (पूर्णांक)CV_ELEM_SIZE1(d) <= (पूर्णांक)EW_SCALAR_BUF_आकार);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        वापस n;
    }
    p = (स्थिर uchar*)sc.getObj();
    d = sc.depth();
    आकार sz = sc.getSz();
    वापस sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
अंतरभूत द्विगुन elemToDouble(पूर्णांक d, स्थिर uchar* p)
{
    चयन (d)
    {
    हाल CV_8U:   वापस *(स्थिर uchar*)p;
    हाल CV_8S:   वापस *(स्थिर schar*)p;
    हाल CV_16U:  वापस *(स्थिर ushort*)p;
    हाल CV_16S:  वापस *(स्थिर लघु*)p;
    हाल CV_32U:  वापस *(स्थिर अचिन्हित*)p;
    हाल CV_32S:  वापस *(स्थिर पूर्णांक*)p;
    हाल CV_64U:  वापस (द्विगुन)*(स्थिर uint64_t*)p;
    हाल CV_64S:  वापस (द्विगुन)*(स्थिर int64_t*)p;
    हाल CV_16F:  वापस (भग्न)*(स्थिर hfloat*)p;
    हाल CV_16BF: वापस (भग्न)*(स्थिर bfloat*)p;
    हाल CV_32F:  वापस *(स्थिर भग्न*)p;
    हाल CV_64F:  वापस *(स्थिर द्विगुन*)p;
    शेष:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
अंतरभूत पूर्णांक readScalarArg(स्थिर निवेश_Array_2& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    स्थिर uchar* p; पूर्णांक d; uchar scbuf[EW_SCALAR_BUF_आकार];
    पूर्णांक n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    माप_प्रकार esz = CV_ELEM_SIZE1(d);
    क्रम (पूर्णांक i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (माप_प्रकार)i * esz);
    वापस n;
}

व्योम convertAndUnrollScalar( स्थिर Mat& sc, पूर्णांक buftype, uchar* scbuf, माप_प्रकार blocksize );

#अगर_घोषित CV_COLLECT_IMPL_तथ्य
काष्ठा ImplCollector
{
    ImplCollector()
    {
        useCollection   = false;
        implFlags       = 0;
    }
    bool useCollection; // enable/disable impl data collection

    पूर्णांक implFlags;
    std::vector<पूर्णांक>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#पूर्ण_अगर

काष्ठा CoreTLSData
{
    CoreTLSData() :
//#ifdef HAVE_OPENCL
        oclExecutionContextInitialized(false), useOpenCL(-1),
//#endif
        useIPP(-1),
        useIPP_NE(-1)
    {}

    RNG rng;
//#ifdef HAVE_OPENCL
    ocl::OpenCLExecutionContext oclExecutionContext;
    bool oclExecutionContextInitialized;
    पूर्णांक useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    पूर्णांक useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    पूर्णांक useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& get_Core_Tls_तथ्य();

#अगर defined(BUILD_SHARED_LIBS)
#अगर defined _WIN32 || defined WINCE
#घोषणा CL_RUNTIME_EXPORT __declspec(dllexport)
#या_अगर defined __GNUC__
#घोषणा CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#अन्यथा
#घोषणा CL_RUNTIME_EXPORT
#पूर्ण_अगर
#अन्यथा
#घोषणा CL_RUNTIME_EXPORT
#पूर्ण_अगर

बाह्य CV_EXPORTS
bool __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#घोषणा CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_मूल्य) \
    अटल TYPE* स्थिर instance = INITIALIZER; \
    वापस RET_मूल्य;

#घोषणा CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#घोषणा CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS व्योम releaseTlsStorageThread();

पूर्णांक cv_snprintf(अक्षर* buf, पूर्णांक len, स्थिर अक्षर* fmt, ...);
पूर्णांक cv_vsnprintf(अक्षर* buf, पूर्णांक len, स्थिर अक्षर* fmt, बहु_सूची args);
}

#पूर्ण_अगर  // BUILD_PLUGIN
#पूर्ण_अगर  // __OPENCV_PRECOMP_H__
