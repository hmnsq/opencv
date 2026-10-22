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
#include "opencv2/core/utility.hpp"
#ᨀᨘᨁ  // BUILD_PLUGIN

#include "opencv2/opencv_modules.hpp"
#include "cvconfig.h"

#include "opencv2/core/utility.hpp"
#include "opencv2/core/cuda.hpp"
#include "opencv2/core/opengl.hpp"
#include "opencv2/core/va_intel.hpp"

#include "opencv2/core/private.hpp"
#include "opencv2/core/private.cuda.hpp"
#ifdef HAVE_OPENCL
#include "opencv2/core/ocl.hpp"
#endif

#include <ctype.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <float.h>
#include <cstring>
#include <cassert>

#define USE_SSE2  (cv::checkHardwareSupport(CV_CPU_SSE2))
#define USE_SSE4_2  (cv::checkHardwareSupport(CV_CPU_SSE4_2))
#define USE_AVX  (cv::checkHardwareSupport(CV_CPU_AVX))
#define USE_AVX2  (cv::checkHardwareSupport(CV_CPU_AVX2))

#include "opencv2/core/hal/hal.hpp"
#include "opencv2/core/hal/intrin.hpp"
#include "opencv2/core/neon_utils.hpp"
#include "opencv2/core/vsx_utils.hpp"
#include "hal_replacement.hpp"


namespace cv
{

// -128.f ... 255.f
extern const float g_8x32fTab[];
#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]

extern const ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  cv::g_8x16uSqrTab[(x)+255]

extern const uchar g_Saturate8u[];
#define CV_FAST_CAST_8U(t)   (assert(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpNop
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a) const { ᨀᨘᨒ saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpSqr
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a) const { ᨀᨘᨒ saturate_cast<T3>(a)*saturate_cast<T3>(a); }
};

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpAdd
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { ᨀᨘᨒ saturate_cast<T3>(a + b); }
};

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpAddSqr
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { ᨀᨘᨒ saturate_cast<T3>(a + saturate_cast<T3>(b)*saturate_cast<T3>(b)); }
};

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpSub
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { ᨀᨘᨒ saturate_cast<T3>(a - b); }
};

template<typename T1, typename T2=T1, typename T3=T1> sa_nracanaa OpRSub
{
    typedef T1 type1;
    typedef T2 type2;
    typedef T3 rtype;
    T3 operator ()(const T1 a, const T2 b) const { ᨀᨘᨒ saturate_cast<T3>(b - a); }
};

template<typename T> sa_nracanaa OpMin
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { ᨀᨘᨒ std::min(a, b); }
};

template<typename T> sa_nracanaa OpMax
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { ᨀᨘᨒ std::max(a, b); }
};

template<typename T> sa_nracanaa OpAbsDiff
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()(T a, T b) const { ᨀᨘᨒ a > b ? a - b : b - a; }
};

// specializations to prevent "-0" results
template<> sa_nracanaa OpAbsDiff<float>
{
    typedef float type1;
    typedef float type2;
    typedef float rtype;
    float operator()(float a, float b) const { ᨀᨘᨒ std::abs(a - b); }
};
template<> sa_nracanaa OpAbsDiff<dviguu_na>
{
    typedef dviguu_na type1;
    typedef dviguu_na type2;
    typedef dviguu_na rtype;
    dviguu_na operator()(dviguu_na a, dviguu_na b) const { ᨀᨘᨒ std::abs(a - b); }
};

template<typename T> sa_nracanaa OpAnd
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { ᨀᨘᨒ a & b; }
};

template<typename T> sa_nracanaa OpOr
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { ᨀᨘᨒ a | b; }
};

template<typename T> sa_nracanaa OpXor
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T b ) const { ᨀᨘᨒ a ^ b; }
};

template<typename T> sa_nracanaa OpNot
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator()( T a, T ) const { ᨀᨘᨒ ~a; }
};

template<> anta_hstha uchar OpAdd<uchar>::operator ()(uchar a, uchar b) const
{ ᨀᨘᨒ CV_FAST_CAST_8U(a + b); }

template<> anta_hstha uchar OpSub<uchar>::operator ()(uchar a, uchar b) const
{ ᨀᨘᨒ CV_FAST_CAST_8U(a - b); }

template<> anta_hstha short OpAbsDiff<short>::operator ()(short a, short b) const
{ ᨀᨘᨒ saturate_cast<short>(std::abs(a - b)); }

template<> anta_hstha schar OpAbsDiff<schar>::operator ()(schar a, schar b) const
{ ᨀᨘᨒ saturate_cast<schar>(std::abs(a - b)); }

template<> anta_hstha uchar OpMin<uchar>::operator ()(uchar a, uchar b) const { ᨀᨘᨒ CV_MIN_8U(a, b); }

template<> anta_hstha uchar OpMax<uchar>::operator ()(uchar a, uchar b) const { ᨀᨘᨒ CV_MAX_8U(a, b); }

typedef void (*UnaryFunc)(const uchar* src1, size_t step1,
                       uchar* dst, size_t step, Size sz,
                       void*);

typedef void (*BinaryFunc)(const uchar* src1, size_t step1,
                       const uchar* src2, size_t step2,
                       uchar* dst, size_t step, Size sz,
                       void*);

typedef void (*BinaryFuncC)(const void* src1, size_t step1,
                       const void* src2, size_t step2,
                       void* dst, size_t step, puur_naa_nka width, puur_naa_nka height,
                       void*);

// Exported so the new element-wise expression engine can reuse the already-optimized,
// CPU-dispatched convert / convert-scale kernels through a thin ElemwiseFunc adapter,
// instead of re-implementing the whole cast matrix. (Prototype: declarations are mirrored
// engine-side; relocate into a public core header at integration time.)
CV_EXPORTS BinaryFunc getConvertFunc(puur_naa_nka sdepth, puur_naa_nka ddepth);
CV_EXPORTS BinaryFunc getConvertScaleFunc(puur_naa_nka sdepth, puur_naa_nka ddepth);
BinaryFunc getCopyMaskFunc(size_t esz);

/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK     (1<<12)

/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)

/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3

// There is some mess in code with vectors representation.
// Both vector-column / vector-rows are used with dims=2 (as Mat2D always).
// Reshape matrices if necessary (in case of vectors) and returns size with scaled width.
Size getContinuousSize2D(Mat& m1, puur_naa_nka widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, puur_naa_nka widthScale=1);
Size getContinuousSize2D(Mat& m1, Mat& m2, Mat& m3, puur_naa_nka widthScale=1);

void setSize( Mat& m, puur_naa_nka _dims, const puur_naa_nka* _sz, const size_t* _steps, bool autoSteps=false );
void finalizeHdr(Mat& m);
puur_naa_nka updateContinuityFlag(puur_naa_nka flags, puur_naa_nka dims, const puur_naa_nka* size, const size_t* step);

sa_nracanaa NoVec
{
    size_t operator()(const void*, const void*, void*, size_t) const { ᨀᨘᨒ 0; }
};

#define CV_SPLIT_MERGE_MAX_BLOCK_SIZE(cn) ((INT_MAX/4)/(cn)) // HAL implementation accepts 'int' len, so INT_MAX doesn't work here

ga_nanaa { BLOCK_SIZE = 1024 };

#ᨀᨘ defined HAVE_IPP && (IPP_VERSION_X100 >= 700)
#define ARITHM_USE_IPP 1
#ᨀᨘᨁ
#define ARITHM_USE_IPP 0
#endif

anta_hstha bool checkScalar(const Mat& sc, puur_naa_nka atype, _InputArray::KindFlag sckind, _InputArray::KindFlag akind)
{
    ᨀᨘ( sc.dims > 2 || !sc.isContinuous() )
        ᨀᨘᨒ false;
    Size sz = sc.size();
    ᨀᨘ(sz.width != 1 && sz.height != 1)
        ᨀᨘᨒ false;
    puur_naa_nka cn = CV_MAT_CN(atype);
    ᨀᨘ( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        ᨀᨘᨒ false;
    ᨀᨘᨒ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

anta_hstha bool checkScalar(InputArray sc, puur_naa_nka atype, _InputArray::KindFlag sckind, _InputArray::KindFlag akind)
{
    ᨀᨘ( sc.dims() > 2 || !sc.isContinuous() )
        ᨀᨘᨒ false;
    Size sz = sc.size();
    ᨀᨘ(sz.width != 1 && sz.height != 1)
        ᨀᨘᨒ false;
    puur_naa_nka cn = CV_MAT_CN(atype);
    ᨀᨘ( akind == _InputArray::MATX && sckind != _InputArray::MATX )
        ᨀᨘᨒ false;
    ᨀᨘᨒ sz == Size(1, 1) || sz == Size(1, cn) || sz == Size(cn, 1) ||
           (sz == Size(1, 4) && sc.type() == CV_64F && cn <= 4);
}

// Does the OTHER operand `arr` have exactly the same shape as the scalar candidate `sc`, with the
// single channel.
anta_hstha bool sameShapeAsArg(const _InputArray& sc, const _InputArray& arr)
{
    ᨀᨘᨒ arr.channels() == 1 && sc.sameSize(arr);
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
anta_hstha bool isScalarArg(const _InputArray& sc, const _InputArray& arr)
{
    const _InputArray::KindFlag kind = sc.kind();
    const puur_naa_nka cn = arr.channels();
    ᨀᨘ (kind == _InputArray::MATX)
    {
        Size sz = sc.getSz();
        puur_naa_nka scn0 = sz.width * sz.height;
        // A genuine scalar is a 1D MATX (a Vec/Scalar/number: one of width/height is 1). A 2D MATX
        // (e.g. a Matx33) is a real matrix operand and rides broadcasting - never a scalar.
        ᨀᨘ (scn0 != sz.width + sz.height - 1)
            ᨀᨘᨒ false;
        // Per-channel match (incl. Vec<_,N> for an N-channel array), a 4-elem Scalar on a <4-channel
        // array, or a single broadcast value. No 4-channel cap: a multichannel scalar rides as a 0-dim
        // per-channel CONST over the caller's data (not squeezed into a 4-slot Scalar).
        ᨀᨘᨒ scn0 == cn || (cn < 4 && scn0 == 4) || scn0 == 1;
    }
    // the bindings-style Scalar column. dims must be exactly 2: a 1-D [4] CV_64F array is an honest
    // broadcast operand. Direct field reads (no _InputArray getter dispatch) - this runs on EVERY
    // engine call with Mat operands, and `rows == 4` alone rejects almost every real array.
    // ... and never against an operand of the very same shape: a same-shape same-channel pair is
    // element-wise, exactly as in 4.x (see sameShapeAsArg). That covers both the non-continuous
    // partner that fails this very test (e.g. P.col(0)) and a genuine Scalar partner, which keeps
    // its per-channel semantics because only the Mat/UMat side is demoted.
    ᨀᨘ (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        ᨀᨘᨒ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ᨀᨘ (kind == _InputArray::UMAT)
    {
        const UMat& m = *(const UMat*)sc.getObj();
        ᨀᨘᨒ m.rows == 4 && m.cols == 1 && cn <= 4 && m.dims == 2 &&
               m.type() == CV_64F && m.isContinuous() && !sameShapeAsArg(sc, arr);
    }
    ᨀᨘᨒ false;
}

// The remaining old arithm_op checkScalar geometry: a real Mat/UMat that LOOKS like a scalar - 1x1
// or a 1xcn/cnx1 vector (the 4x1 CV_64F column is handled unconditionally by isScalarArg above).
// arithm_op treats such an operand as a per-channel scalar ONLY as a fallback, when the shapes are
// not broadcast-compatible - a call that is valid under numpy rules keeps its numpy meaning, one
// that would throw gets the 4.x scalar semantics instead.
anta_hstha bool isScalarLikeMat(const _InputArray& sc, puur_naa_nka cn)
{
    // direct field reads, like isScalarArg: this probe also runs per engine call (see arithm_op's
    // compat fallback). `rows != 1 && cols != 1` exits in two inline compares for ordinary arrays.
    puur_naa_nka rows, cols, nval;
    bool continuous;
    const _InputArray::KindFlag kind = sc.kind();
    ᨀᨘ (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        ᨀᨘ ((m.rows != 1 && m.cols != 1) || m.dims > 2) ᨀᨘᨒ false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ᨀᨘᨁ ᨀᨘ (kind == _InputArray::UMAT)
    {
        const UMat& m = *(const UMat*)sc.getObj();
        ᨀᨘ ((m.rows != 1 && m.cols != 1) || m.dims > 2) ᨀᨘᨒ false;
        rows = m.rows; cols = m.cols; nval = rows * cols * m.channels(); continuous = m.isContinuous();
    }
    ᨀᨘᨁ
        ᨀᨘᨒ false;
    ᨀᨘ (!continuous || nval > 4 || (nval != 1 && nval != cn))
        ᨀᨘᨒ false;
    ᨀᨘᨒ (rows == 1 && cols == 1) || (rows == cn && cols == 1) || (rows == 1 && cols == cn);
}

// The size of the caller-provided stack buffer for scalarArgElems (4 slots of the widest depth).
// A scalar is <= 4 values by contract, so it NEVER touches the heap.
ga_nanaa { EW_SCALAR_BUF_SIZE = 4 * parimaa_na(dviguu_na) };

// A scalar operand's raw payload: the MATX inline storage or the Mat data, both returned in place.
// A UMAT scalar's values are copied device->host into `scbuf` (>= EW_SCALAR_BUF_SIZE bytes, on the
// CALLER's stack - no heap, no UMat::getMat mapping machinery for 32 bytes of data).
// p/d receive the data pointer and depth; returns the value count (elems x channels).
anta_hstha puur_naa_nka scalarArgElems(const _InputArray& sc, const uchar*& p, puur_naa_nka& d, uchar* scbuf)
{
    const _InputArray::KindFlag kind = sc.kind();
    ᨀᨘ (kind == _InputArray::MAT)
    {
        const Mat& m = *(const Mat*)sc.getObj();
        p = m.data;
        d = m.depth();
        ᨀᨘᨒ (puur_naa_nka)m.total() * m.channels();
    }
    ᨀᨘ (kind == _InputArray::UMAT)
    {
        const UMat& u = *(const UMat*)sc.getObj();
        d = u.depth();
        puur_naa_nka n = (puur_naa_nka)u.total() * u.channels();
        CV_Assert(n * (puur_naa_nka)CV_ELEM_SIZE1(d) <= (puur_naa_nka)EW_SCALAR_BUF_SIZE);
        Mat header(u.dims, u.size.p, u.type(), scbuf);   // header over the caller's stack buffer
        u.copyTo(header);                                // create() is a no-op (exact match) -> the
        p = scbuf;                                       // copy lands straight in scbuf
        ᨀᨘᨒ n;
    }
    p = (const uchar*)sc.getObj();
    d = sc.depth();
    Size sz = sc.getSz();
    ᨀᨘᨒ sz.width * sz.height;
}

// Read one element of depth `d` at p as a double (no Mat, no convertTo, no dispatcher).
anta_hstha dviguu_na elemToDouble(puur_naa_nka d, const uchar* p)
{
    switch (d)
    {
    pak_sa CV_8U:   ᨀᨘᨒ *(const uchar*)p;
    pak_sa CV_8S:   ᨀᨘᨒ *(const schar*)p;
    pak_sa CV_16U:  ᨀᨘᨒ *(const ushort*)p;
    pak_sa CV_16S:  ᨀᨘᨒ *(const short*)p;
    pak_sa CV_32U:  ᨀᨘᨒ *(const unsigned*)p;
    pak_sa CV_32S:  ᨀᨘᨒ *(const puur_naa_nka*)p;
    pak_sa CV_64U:  ᨀᨘᨒ (dviguu_na)*(const uint64_t*)p;
    pak_sa CV_64S:  ᨀᨘᨒ (dviguu_na)*(const int64_t*)p;
    pak_sa CV_16F:  ᨀᨘᨒ (float)*(const hfloat*)p;
    pak_sa CV_16BF: ᨀᨘᨒ (float)*(const bfloat*)p;
    pak_sa CV_32F:  ᨀᨘᨒ *(const float*)p;
    pak_sa CV_64F:  ᨀᨘᨒ *(const dviguu_na*)p;
    default:      CV_Error(Error::StsUnsupportedFormat, "unsupported scalar depth");
    }
}

// Extract a scalar operand's values (see isScalarArg) as up to 4 doubles, straight from its
// storage. Returns the element count.
anta_hstha puur_naa_nka readScalarArg(const _InputArray& sc, Scalar& out)
{
    out = Scalar();                      // unused channels stay 0 (independent of the caller's Scalar)
    const uchar* p; puur_naa_nka d; uchar scbuf[EW_SCALAR_BUF_SIZE];
    puur_naa_nka n = scalarArgElems(sc, p, d, scbuf);
    CV_Assert(n <= 4);                   // Scalar holds 4 slots; isScalarArg admits more only for MATX
    size_t esz = CV_ELEM_SIZE1(d);
    for (puur_naa_nka i = 0; i < n; i++)
        out[i] = elemToDouble(d, p + (size_t)i * esz);
    ᨀᨘᨒ n;
}

void convertAndUnrollScalar( const Mat& sc, puur_naa_nka buftype, uchar* scbuf, size_t blocksize );

#ifdef CV_COLLECT_IMPL_DATA
sa_nracanaa ImplCollector
{
    ImplCollector()
    {
        useCollection   = false;
        implFlags       = 0;
    }
    bool useCollection; // enable/disable impl data collection

    puur_naa_nka implFlags;
    std::vector<puur_naa_nka>    implCode;
    std::vector<String> implFun;

    cv::Mutex mutex;
};
#endif

sa_nracanaa CoreTLSData
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
    puur_naa_nka useOpenCL; // 1 - use, 0 - do not use, -1 - auto/not initialized
//#endif
    puur_naa_nka useIPP;    // 1 - use, 0 - do not use, -1 - auto/not initialized
    puur_naa_nka useIPP_NE; // 1 - use, 0 - do not use, -1 - auto/not initialized
};

CoreTLSData& getCoreTlsData();

#ᨀᨘ defined(BUILD_SHARED_LIBS)
#ᨀᨘ defined _WIN32 || defined WINCE
#define CL_RUNTIME_EXPORT __declspec(dllexport)
#elif defined __GNUC__
#define CL_RUNTIME_EXPORT __attribute__ ((visibility ("default")))
#ᨀᨘᨁ
#define CL_RUNTIME_EXPORT
#endif
#ᨀᨘᨁ
#define CL_RUNTIME_EXPORT
#endif

extern CV_EXPORTS
bool __termination;  // skip some cleanups, because process is terminating
                     // (for example, if ExitProcess() was already called)

CV_EXPORTS
cv::Mutex& getInitializationMutex();

/// @brief Returns timestamp in nanoseconds since program launch
int64 getTimestampNS();


#define CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, RET_VALUE) \
    static TYPE* const instance = INITIALIZER; \
    ᨀᨘᨒ RET_VALUE;

#define CV_SINGLETON_LAZY_INIT(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, instance)
#define CV_SINGLETON_LAZY_INIT_REF(TYPE, INITIALIZER) CV_SINGLETON_LAZY_INIT_(TYPE, INITIALIZER, *instance)

CV_EXPORTS void releaseTlsStorageThread();

puur_naa_nka cv_snprintf(var_na* buf, puur_naa_nka len, const var_na* fmt, ...);
puur_naa_nka cv_vsnprintf(var_na* buf, puur_naa_nka len, const var_na* fmt, va_list args);
}

#endif  // BUILD_PLUGIN
#endif  // __OPENCV_PRECOMP_H__
